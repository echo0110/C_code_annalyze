/******************************************************************************
 * *
 * *  ARICENT -
 * *
 * *  Copyright (C) 2015 Aricent Inc. All Rights Reserved.
 * *
 * ******************************************************************************
 * *
 * *  File Name: lteMacEMbmsSchedulerStrategy.c
 * *
 * ******************************************************************************
 * *
 * *  File Description : This file includes the functionality of eMbms M2AP interface 
 * *                     API handling as well as M1U related information
 * 
 * *****************************************************************************/

/******************************************************************************
 *  * Standard Library Includes
 *   *****************************************************************************/



/******************************************************************************
 *  * Project Includes
 *   *****************************************************************************/
#include<lteRelayM1uTeidSearchTree.h>
#include<lteRelayM2Sap.h>
#include "lteMemPool.h"
#include "lteMisc.h"
/******************************************************************************
 *  * Exported Variables
 * *****************************************************************************/

/******************************************************************************
 *  * Exported Definitions
 * ****************************************************************************/

/******************************************************************************
 *   Private Definitions
 * ****************************************************************************/



/*Global Mip list which store's multicasetip list */
LTE_LIST m1uMipList_g;
LTE_LIST tunnelList_g;
extern void* udpSendToS1ap(Char8 *buff_p, int len);
extern LP_ConfigFileFormat configData;
extern SInt32 sockRxEmbms_g;
extern SInt32 sockRx6Embms_g;
STATIC inline  UInt8 parseIpfrombuf(UInt8 *ipStr,UInt8 *msg_p,UInt8 ipBytes,UInt16 *current);
STATIC inline  UInt8 createMipandInsertinList(M1UMipNodeInfo **mIpNode_p,UInt8 *mIp,UInt8 *sIp,UInt8 ipBytes);
STATIC inline  UInt8 createAndInsertNodeInTeidTree(TEIDAreaLcTreeInfo **teidAreaLcInfo,UInt32 teid);
STATIC inline  UInt8 createTeidNodeandInsertInMipList(M1UMipNodeInfo *mIpNode_p,UInt32 teid);
STATIC inline   void  prepareM1UAreaResponse(UInt8 *respMsg,M1UCreateAreaReq **sessionSapReq_p,UInt16 *currentPos_p,UInt32 count);
STATIC inline  void convertIpBytetoString(Char8 *ipStr,UInt8 *msg_p,UInt8 ipBytes);
STATIC inline UInt8 checkforDuplicateSessionInfo(M1UMipNodeInfo *mIpNode_p,UInt32 teid);
STATIC void deleteCompleteTeIdNodeFromTree(UInt32 teId);
STATIC UInt32 deleteAllSessions(void);
STATIC TunnelInfoNode* findTunnelInfoNode(TunnelInfo* tunnelInfo_p);
STATIC void deleteTunnelInfoNode(TunnelInfo* tunnelInfo_p);
STATIC void initM1uAreaContext(void);
/*SPR 18749 fix start*/
extern UInt8 mcchModificationPeriodCounter(UInt8 areaIndex);
/*SPR 18749 fix end*/


/*Teid Global tree*/
TEIDSearchTree teidTree_g;
#ifdef MAC_AUT_TEST
UInt8 aut_trigger_test_g;
#endif
relayMcchMap relayMcchMapIndex_g[MAX_AREA_ID][MCCHMAPINDEX+1];  
M1uAreaContext M1uAreaContext_g[MAX_AREA_ID][MAX_EMBMS_LCID];

/****************************************************************************
 * Function Name  : initPacketRelayEmbmsStructures 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function initialises embms structures 
 ****************************************************************************/

void initPacketRelayEmbmsStructures()
{
    initmIpList();
    teidTreeInit(&teidTree_g);
    initM1uAreaContext();
}
/****************************************************************************
 * Function Name  : AddAreaLcListFromTeidNode 
 * Inputs         : sessionSapReq_p - areaLclistinfo &teid infomration 
 * Outputs        : 
 * Returns        :
 * Description    : This function process the following 
 * 		   1. Fetch teid node from teidtree with key as TEID
 * 		   2. Add AreaLclist with existing list info
 * 		     
 ****************************************************************************/
UInt32 AddAreaLcListFromTeidNode(
        M1UCreateAreaReq* sessionSapReq_p) 
{

    UInt8 indx =0;
    UInt8 count =0;
    TEIDAreaLcTreeInfo* teidTreeNode_p =PNULL;
    TEIDInfo *teidInfoNode_p=PNULL;
    UInt8 nodeFound=LTE_FALSE;
    teidTreeNode_p = (TEIDAreaLcTreeInfo *)teidFindTreeNode(&teidTree_g,
            &sessionSapReq_p->tunnel.teid);

    if(PNULL != teidTreeNode_p)
    {
        teidInfoNode_p=(TEIDInfo *)getFirstListNode(&(teidTreeNode_p->teidInfoList));
        if(PNULL == teidInfoNode_p)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"No node present in TE-ID info list\n");
            return RELAY_GTPU_FAILURE;
        }

        while(teidInfoNode_p)
        {
            if((!memCmp(teidInfoNode_p->mIp,sessionSapReq_p->tunnel.mIp,RELAY_ADDR_LEN_IPV6)) && \
                    (!memCmp(teidInfoNode_p->sIp,sessionSapReq_p->tunnel.sIp,RELAY_ADDR_LEN_IPV6) ))
            {
                indx = teidInfoNode_p->listCount;

                /*Update indx with listcount intially listcout value will be zero 
                 * later on it update with current list count on further updation*/
                sessionSapReq_p->listCount += indx;
                for(;indx < sessionSapReq_p->listCount ; indx++)
                {
                    teidInfoNode_p->areaLcList[indx].areaId = sessionSapReq_p->sessionSapAreaLcInfo[count].areaId;
                    teidInfoNode_p->areaLcList[indx].lcId = sessionSapReq_p->sessionSapAreaLcInfo[count].lcId;
                    teidInfoNode_p->listCount++;
                    count++;
                }
                nodeFound=LTE_TRUE;
                break;
            }
            else
            {
                teidInfoNode_p = ( TEIDInfo *)getNextListNode(&(teidInfoNode_p->teidAnchor));
            }
        }
        if(LTE_FALSE == nodeFound)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"<MIP,SIP> node not found in TE-ID info list\n");
            return RELAY_GTPU_FAILURE;
        }
    }
    else
    {
       LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"TE ID:%u Node Not found in TE-ID tree\n",\
		       sessionSapReq_p->tunnel.teid);

        return RELAY_GTPU_FAILURE;
    }
    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : processM1uSessionStartRequest
 * Inputs         : sessionReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP M1U session start request .
 * 		    1. Parse the received buffer 
 * 		    2. store it in to local buffer 
 * 		    3. process the received Mip and teid and update in to Miptable 
 * 		    along witn teid list
 * 		    4. Update teid as a key in to teidtree 
 ****************************************************************************/
UInt32 processM1uSessionStartRequest(UInt8 *sessionReq_p, 
        UInt16 msgLen)
{
    UInt8 respMsg[M1U_RESP_MSG_SIZE] = {0};
    UInt8  currentPos = 0;
    UInt16 current = 0;   
    UInt16 transIdForRsp = 0, apiId = 0; 
    UInt32 ret = RELAY_GTPU_SUCCESS;
    UInt8 cellIndex;
    TunnelInfo* sessionSapReq_p = PNULL;
    TunnelInfoNode* tunnelInfoNode_p=PNULL;
    /*Starting 10bytes is header part parsing message below for Transaction and apidi*/
    /*Extracting Transaction identifer */
    transIdForRsp = RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
    cellIndex = sessionReq_p[API_HEADER_CELL_INDEX_OFFSET];

    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"RRC->PR SESSION_START_REQUEST\n");
    /*Allocate memory*/
    do
    {
        if(msgLen < M1U_SESSION_START_MIN_LEN)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_START_REQUEST:"\
                                           "Incorrect length:%u received min expected length:%u\n",\
                                           msgLen,M1U_SESSION_START_MIN_LEN);
            ret = RELAY_GTPU_FAILURE;
            break;
        }
        sessionSapReq_p =  (TunnelInfo*)getMemFromPool(sizeof( TunnelInfo),NULL);

        if(PNULL == sessionSapReq_p )
        {
            LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Mem alloc failed for sessionSapReq_p\n");
            ret=RELAY_GTPU_FAILURE;
            break;
        }

        /*Adding memSet */
        memSet(sessionSapReq_p,0,sizeof(TunnelInfo));
        current = API_HDR;
        /*Current position has teid of 4 bytes*/
        sessionSapReq_p->teid = RELAY_GTPU_GET_U32BIT(&sessionReq_p[current]);
        current +=4;

        /*current position has ipBytes of 1 bytes */
        sessionSapReq_p->ipBytes = sessionReq_p[current];
        current+=1;
        /*current position has M.ip */
        /*Parse Ip*/
        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"TeID:%u ipBytes:%u (4 for IPv4, 16 for IPv6\n",\
                                      sessionSapReq_p->teid,sessionSapReq_p->ipBytes);

        if(RELAY_GTPU_SUCCESS != parseIpfrombuf(sessionSapReq_p->mIp,\
                    &sessionReq_p[current],\
                    sessionSapReq_p->ipBytes,\
                    &current))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_START_REQUEST:" \
                                          "Parsing Failed for destination multicast IP\n");
            ret = RELAY_GTPU_FAILURE;
            break;
            // logging
        }

        /* Parse Source IP*/
        if(RELAY_GTPU_SUCCESS != parseIpfrombuf(sessionSapReq_p->sIp,\
                    &sessionReq_p[current],\
                    sessionSapReq_p->ipBytes,\
                    &current))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_START_REQUEST:"\
                                          "Parsing Failed for source IP\n");
            ret = RELAY_GTPU_FAILURE;
            break;
            // logging
        }

        /* 	Trigger to received MulticastIp whether to join or not 
         * 	1. First check if Mip and SIP list count is zero then initalize the mip list and 
         * 	   update the teid in to Mip list teidqueue then send Multicastip joining trigger,
         * 	   otherwise proceed to next step
         * 	2. Check received Mip is already present in existing list or not 
         * 	   if present update the teid node in to that MIP list, since thie MulticastIp is 
         * 	   already present so,need not to send  Multicastip joining trigger.
         * 	3. If Mip is not found in that list then creat Mip node and update in to 
         * 	   Mip list also update teid in to that newly created mipnode then send  Multicastip 
         * 	   joining trigger     */

        if(RELAY_GTPU_SUCCESS != processMipAndTeid(sessionSapReq_p->mIp,sessionSapReq_p->sIp,sessionSapReq_p->ipBytes,
                    sessionSapReq_p->teid,\
                    ADD_TRIGGER))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Processing Failed SESSION_START_REQUEST:"\
                                          "processMipAndTeid returns Failure\n");
            ret = RELAY_GTPU_FAILURE;
            break;
        }

        /*Add teid in to Teid Tree */
        if(RELAY_GTPU_SUCCESS != addNodeToTeidTree(sessionSapReq_p))
        {
            /*Coverity :97607 fix start*/
            ret = processMipAndTeid(sessionSapReq_p->mIp,sessionSapReq_p->sIp,sessionSapReq_p->ipBytes,\
                    sessionSapReq_p->teid,DEL_TRIGGER);
	     
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Processing Failed SESSION_START_REQUEST:"\
                                          "TEID node insertion in TEID tree failed ret value %d\n",ret); 
            /*Coverity :97607 fix end*/
            ret = RELAY_GTPU_FAILURE;
        }
    }while(0);

    if(ret != RELAY_GTPU_FAILURE)
    {
        tunnelInfoNode_p=(TunnelInfoNode*)getMemFromPool(sizeof(TunnelInfoNode),PNULL);
        if(tunnelInfoNode_p)
        {
            memCpy(&(tunnelInfoNode_p->tunnel),sessionSapReq_p,sizeof(TunnelInfo));
            listInsertNode(&tunnelList_g,&(tunnelInfoNode_p->nodeAnchor));
        }    
    }    
    /*Send response back to M1U*/
    currentPos = API_HDR; 
    /**/
    apiId = M1U_SESSION_START_CNF;
    respMsg[currentPos] = ret;  /* Cause */
    currentPos++;

    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"GTPUPR->RRC M1U_SESSION_START_CNF Response:%u\n",ret);

    /* Append API Header in front of message for sending to S1-AP */
    appendApiHeader(RELAY_S1AP_MODULE_ID, apiId, transIdForRsp, currentPos, respMsg,cellIndex);
    /* Sending to M2-AP */
    udpSendToS1ap((char*)respMsg, currentPos);
    freeMemPool(sessionSapReq_p);
    return ret;
}
/****************************************************************************
 * Function Name  : addNodeToTeidTree
 * Inputs         : sessionSapReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function adds node to teid Tree .
 ****************************************************************************/
UInt8 addNodeToTeidTree(TunnelInfo* sessionSapReq_p)
{
    TEIDAreaLcTreeInfo *teidAreaLcInfo_p=PNULL;
    /*Add teid in to Teid Tree */
    TEIDInfo *teidInfo_p=PNULL;
    teidInfo_p = (TEIDInfo *)getMemFromPool(sizeof(TEIDInfo),NULL); //create teidInfo node to be inserted
    if(PNULL == teidInfo_p)
    {
        LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Mem alloc failed for teidInfo_p\n");
        return RELAY_GTPU_FAILURE;
    }
    memCpy(teidInfo_p->mIp,sessionSapReq_p->mIp,RELAY_ADDR_LEN_IPV6);
    memCpy(teidInfo_p->sIp,sessionSapReq_p->sIp,RELAY_ADDR_LEN_IPV6);
    teidInfo_p->listCount=0;
    if(!teidTreeNodeCount(&teidTree_g))//if node count is zero in tree
    {  
        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Adding first TEID Node in TEID tree\n");
        if(RELAY_GTPU_SUCCESS != createAndInsertNodeInTeidTree(&teidAreaLcInfo_p,sessionSapReq_p->teid))
        {
            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"TEID Node insertion in  TEID tree failed\n");
            return RELAY_GTPU_FAILURE;
        }
        listInsertNode(&(teidAreaLcInfo_p->teidInfoList),&(teidInfo_p->teidAnchor));

    }
    else
    {

        teidAreaLcInfo_p= (TEIDAreaLcTreeInfo *)teidFindTreeNode(&teidTree_g,
                &sessionSapReq_p->teid);
        if (PNULL == teidAreaLcInfo_p )//if the node with teid to be inserted is not present in tree
        {
            if(RELAY_GTPU_SUCCESS != createAndInsertNodeInTeidTree(&teidAreaLcInfo_p,sessionSapReq_p->teid))
            {
                LOG_GTPUPR_INFO(L2_EMBMS_CAT,"TEID Node insertion in  TEID tree failed\n");
                return RELAY_GTPU_FAILURE;
            }
        }
        listInsertNode(&(teidAreaLcInfo_p->teidInfoList),&(teidInfo_p->teidAnchor));

    }
    return RELAY_GTPU_SUCCESS;
}
/****************************************************************************
 * Function Name  : processM1uSessionStopRequest
 * Inputs         : sessionReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP M1U session stop request .
 ****************************************************************************/
UInt32 processM1uSessionStopRequest(UInt8 *sessionReq_p, UInt16 msgLen)
{
    UInt8 respMsg[M1U_RESP_MSG_SIZE] = {0};
    UInt8  currentPos = 0;
    UInt16 current = 0;   
    UInt16 transIdForRsp = 0, apiId = 0;
    UInt32 resp=RELAY_GTPU_SUCCESS;
    UInt16 tag,length;
    UInt8 listCount=0,count=0;
    UInt8 cellIndex;
    TunnelInfo* sessionSapReq_p;
    
    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"RRC->PR SESSION_STOP_REQUEST\n");

    sessionSapReq_p=  (TunnelInfo*)getMemFromPool(sizeof( TunnelInfo),NULL);
    if(PNULL == sessionSapReq_p)
    {
        LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Mem alloc failed for sessionSapReq_p while parsing"\
                                     "Session Stop request\n");
        return RELAY_GTPU_FAILURE;
    }
    memSet(sessionSapReq_p,0,sizeof(TunnelInfo));

    /*Starting 10bytes is header part parsing message below for Transaction and apiId*/
    /*Extracting Transaction identifer */
    transIdForRsp = RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
    cellIndex = sessionReq_p[API_HEADER_CELL_INDEX_OFFSET];
    current = API_HDR;
    listCount=sessionReq_p[current++];
    if(msgLen < M1U_SESSION_STOP_MIN_LEN)
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_STOP_REQUEST:"\
                                      "Incorrect length:%u received min expected length:%u\n",\
			msgLen,M1U_SESSION_STOP_MIN_LEN);
        resp = RELAY_GTPU_FAILURE;
       
    }
    else if( listCount > MAX_EMBMS_SESSIONS)
    {
        //logging
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_STOP_REQUEST:"\
                                      "Invalid List Count:%u Expected Range:[1-%u]\n",\
                         listCount,MAX_EMBMS_SESSIONS);
        resp = RELAY_GTPU_FAILURE;
    }
    else if (listCount == 0)
    {
        resp = deleteAllSessions();
        if(resp == RELAY_GTPU_FAILURE)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"deleteAllSessions returns failure");
        }    
            
    }    
    else
    {
        while( count < listCount)
        {
            tag=RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
            if(tag != TUNNEL_INFO)
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_STOP_REQUEST:"\
                                              "Invalid Tag:%u  Expected Tag TUNNEL_INFO:[%u]\n",\
				tag,TUNNEL_INFO);
                resp = RELAY_GTPU_FAILURE;
                break;
            }
            current+=2;
            length=RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
            if((TUNNEL_INFO_IPV4_LENGTH != length) && (TUNNEL_INFO_IPV6_LENGTH != length))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_STOP_REQUEST:"\
                                              "Invalid Length:%u  Expected Length:[%u or %u]\n",\
				length,TUNNEL_INFO_IPV4_LENGTH,TUNNEL_INFO_IPV6_LENGTH);
                resp = RELAY_GTPU_FAILURE;
                break;
            }
            current+=2;
            /*Current position has teid of 4 bytes*/
            sessionSapReq_p->teid = RELAY_GTPU_GET_U32BIT(&sessionReq_p[current]);
            current +=4;
            /*current position has ipBytes of 1 bytes */
            sessionSapReq_p->ipBytes = sessionReq_p[current ++];

            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"TeID:%u ipBytes:%u (4 for IPv4, 16 for IPv6\n",\
			    sessionSapReq_p->teid,sessionSapReq_p->ipBytes);

            if( RELAY_GTPU_FAILURE == parseIpfrombuf(sessionSapReq_p->mIp,\
                        &sessionReq_p[current],\
                        sessionSapReq_p->ipBytes,\
                        &current))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_STOP_REQUEST:" \
				                              "Parsing Failed for destination multicast IP\n");
                resp= RELAY_GTPU_FAILURE ;
                break;
            }
            if(RELAY_GTPU_FAILURE == parseIpfrombuf(sessionSapReq_p->sIp,\
                        &sessionReq_p[current],\
                        sessionSapReq_p->ipBytes,\
                        &current))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_STOP_REQUEST:" \
				                              "Parsing Failed for source IP\n");
                resp= RELAY_GTPU_FAILURE ;
                break;
            }
            /*Trigger to disjoining request based on received TEID*/
            /* Trigger to received MulticastIp whether to Disjoin or not, based on the trigger
             * 	passed as an argument accordingly function behave 
             * 	1. First , retreive the first node from the MIP list and traverse the list 
             * 	   with received MulticaseIP
             * 	2. After Mipnode found from the MIPlist check the received Teid with in
             * 	   that teid queue 
             *      3. After finding teid node from the teidqueue , delete the teid node from 
             *         teidqueue
             *      4 .Finally check whether teidqueue count is  zero then send disjoining trigger */

            if(RELAY_GTPU_FAILURE == processMipAndTeid(sessionSapReq_p->mIp,sessionSapReq_p->sIp,
                        sessionSapReq_p->ipBytes,
                        sessionSapReq_p->teid,
                        DEL_TRIGGER))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Processing Failed SESSION_STOP_REQUEST:"\
				                              "processMipAndTeid returns Failure\n");

            }
            /* Delete teid from the teidTree
             * 1. find the node from TeidTree
             * 2. if teidnode found then delete from the teidtree*/
            if(RELAY_GTPU_SUCCESS != deleteNodeFromTeidTree(sessionSapReq_p))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Processing Failed SESSION_STOP_REQUEST:"\
				                              "deleteNodeFromTeidTree returns Failure\n");
            }

            deleteTunnelInfoNode(sessionSapReq_p);
            count++;
        }
    }
    /*Send response back to M1U*/
    currentPos = API_HDR; 

    apiId = M1U_SESSION_STOP_CNF;
    respMsg[currentPos] = resp;  /* Cause */
    currentPos++;

    /* Append API Header in front of message for sending to S1-AP */
    appendApiHeader(RELAY_S1AP_MODULE_ID, apiId, transIdForRsp, currentPos, respMsg,cellIndex);

    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"GTPUPR->RRC M1U_SESSION_STOP_CNF Response:%u\n",resp);
    /* Sending to M2-AP */
    udpSendToS1ap((char*)respMsg, currentPos);
    freeMemPool(sessionSapReq_p);
    return resp;
}

/****************************************************************************
 * Function Name  : deleteNodeFromTeidTree
 * Inputs         : sessionSapReq_p
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function deletes node:-
 *                  1.This deletes node from teidinfo list present in teid tree
 *                  2.If teidinfo list has zero elements after removal that node is
 *                    deleted from teid tree.
 ****************************************************************************/
UInt8 deleteNodeFromTeidTree(TunnelInfo *sessionSapReq_p)
{
    TEIDAreaLcTreeInfo *teidTreeNode_p = PNULL;
    TEIDInfo *teidInfoNode_p=PNULL;
    UInt8 nodeFound=LTE_FALSE;
    UInt8 count=0;
    UInt8 areaIndex=0;
    UInt8 lcId=0;

    teidTreeNode_p = (TEIDAreaLcTreeInfo *)teidFindTreeNode(&teidTree_g,
            &sessionSapReq_p->teid); //find the teid

    if(PNULL != teidTreeNode_p)
    {
        teidInfoNode_p=(TEIDInfo *)getFirstListNode(&(teidTreeNode_p->teidInfoList));//get the first node
        if(PNULL == teidInfoNode_p)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"No node present in TE-ID info list\n");
            return RELAY_GTPU_FAILURE;

        }
        while(teidInfoNode_p)
        {
            if((!memCmp(teidInfoNode_p->mIp,sessionSapReq_p->mIp,RELAY_ADDR_LEN_IPV6)) && \
                    (!memCmp(teidInfoNode_p->sIp,sessionSapReq_p->sIp,RELAY_ADDR_LEN_IPV6) ))//compare mip and sip present in teid info node
            {
                listDeleteNode(&(teidTreeNode_p->teidInfoList),&(teidInfoNode_p->teidAnchor));
                for(count=0;count<teidInfoNode_p->listCount;count++)
                {
                    areaIndex=teidInfoNode_p->areaLcList[count].areaId;
                    lcId=teidInfoNode_p->areaLcList[count].lcId;
                    M1uAreaContext_g[areaIndex][lcId].tunnelInfoNode_p=PNULL;
                    M1uAreaContext_g[areaIndex][lcId].valid=LTE_FALSE;
                }    
                freeMemPool(teidInfoNode_p);
                nodeFound=LTE_TRUE;
                LOG_GTPUPR_INFO(L2_EMBMS_CAT,"<MIP,SIP> node found and deleted from TE-ID Info List\n");
                break;
            }
            else
            {

                teidInfoNode_p = ( TEIDInfo *)getNextListNode(&(teidInfoNode_p->teidAnchor));
            }

        }
        if(LTE_FALSE == nodeFound)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Requested node not found in TE-ID info list\n");
            return RELAY_GTPU_FAILURE;
        }
        if(!listCount(&(teidTreeNode_p->teidInfoList)))
        {
            teidDeleteTreeNode(&teidTree_g,&(teidTreeNode_p)->teidAnchor);
            freeMemPool(&(teidTreeNode_p)->teidAnchor);
            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"TE-ID Node deleted from TE-ID tree\n");
        }

    }
    else
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"TE ID Node Not found in TE-ID tree "\
		                              "Session stop has been received for the TEID:%u for"
		                              "session start is not received\n",sessionSapReq_p->teid);
       return RELAY_GTPU_FAILURE;
    }
    return RELAY_GTPU_SUCCESS;
}
/****************************************************************************
 * Function Name  : processM1uSessionUpdateRequest
 * Inputs         : sessionReq_p,
 *                  msgLen
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP M1U session Update request .
 ****************************************************************************/
UInt32 processM1uSessionUpdateRequest(UInt8 *sessionReq_p, UInt16 msgLen)
{
    UInt8 respMsg[20] = {0};
    UInt8  currentPos = 0;
    UInt8 cellIndex;
    UInt16 tag;
    UInt16 taglen;
    UInt16 current = 0;   
    UInt16 transIdForRsp = 0, apiId = 0 ; 
    UInt32 ret = RELAY_GTPU_SUCCESS;
    /*Starting 10bytes is header part parsing message below for Transaction and apidi*/
    /*Extracting Transaction identifer */
    transIdForRsp = RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
    cellIndex = sessionReq_p[API_HEADER_CELL_INDEX_OFFSET];

    TunnelInfo* 	sessionSapReq_p = PNULL;
    TunnelInfo* 	oldSessionSapReq_p = PNULL;

    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"RRC->PR SESSION_UPDATE_REQUEST\n");

    do
    {
        if(msgLen < M1U_SESSION_UPDATE_MIN_LEN)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_UPDATE_REQUEST:"\
                                          "Incorrect length:%u received min expected length:%u\n",\
                                          msgLen,M1U_SESSION_UPDATE_MIN_LEN);

            ret = RELAY_GTPU_FAILURE;
            break;
        }
        sessionSapReq_p= (TunnelInfo *)getMemFromPool(sizeof( TunnelInfo),PNULL);
        if( PNULL == sessionSapReq_p )
        {
            LOG_GTPUPR_FATAL(L2_EMBMS_CAT,"Parsing Failed SESSION_UPDATE_REQUEST:"\
                                          "Mem alloc failed for sessionSapReq_p\n");
            ret = RELAY_GTPU_FAILURE;
            break;
        }
        oldSessionSapReq_p= (TunnelInfo *)getMemFromPool(sizeof( TunnelInfo),PNULL);
        if( PNULL == oldSessionSapReq_p  )
        {
            LOG_GTPUPR_FATAL(L2_EMBMS_CAT,"Parsing Failed SESSION_UPDATE_REQUEST:"\
                                          "Mem alloc failed for oldSessionSapReq_p\n");
            ret = RELAY_GTPU_FAILURE;
            break;
        }

        /*Adding memSet */
        memSet(sessionSapReq_p,0,sizeof(TunnelInfo));
        memSet(oldSessionSapReq_p,0,sizeof(TunnelInfo));
        current = API_HDR;
        /*Current position has teid of 4 bytes*/
        sessionSapReq_p->teid = RELAY_GTPU_GET_U32BIT(&sessionReq_p[current]);
        current +=4;
        /*current position has ipBytes of 1 bytes */
        sessionSapReq_p->ipBytes = sessionReq_p[current];
        current+=1;

        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"New Session Info\n"\
                                     "TeID:%u ipBytes:%u (4 for IPv4, 16 for IPv6\n",\
                                     sessionSapReq_p->teid,sessionSapReq_p->ipBytes);

        if(RELAY_GTPU_FAILURE == parseIpfrombuf(sessionSapReq_p->mIp,
                    &sessionReq_p[current],
                    sessionSapReq_p->ipBytes,
                    &current))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_UPDATE_REQUEST:" \
                                          "Parsing Failed for New destination multicast IP\n");
            ret=RELAY_GTPU_FAILURE;
            break;
        }

        if(RELAY_GTPU_FAILURE== parseIpfrombuf(sessionSapReq_p->sIp,
                    &sessionReq_p[current],
                    sessionSapReq_p->ipBytes,
                    &current))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_UPDATE_REQUEST:" \
                                          "Parsing Failed for New source IP\n");
            ret=RELAY_GTPU_FAILURE;
            break;
        }

        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Parsing done for New Seesion Info. Now parsing "\
                                     "old session info\n");

        tag=RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
        current += 2;
        taglen=RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
        current += 2;
        if ( TUNNEL_INFO  != tag )
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_UPDATE_REQUEST:"\
                                          "Invalid Tag:%u  Expected Tag TUNNEL_INFO:[%u]\n",\
                                          tag,TUNNEL_INFO);
            ret = RELAY_GTPU_FAILURE;
            break;
        }
        if((TUNNEL_INFO_IPV4_LENGTH != taglen) && (TUNNEL_INFO_IPV6_LENGTH != taglen))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_UPDATE_REQUEST:"\
                                          "Invalid Length:%u  Expected Length:[%u or %u]\n",\
                                          taglen,TUNNEL_INFO_IPV4_LENGTH,TUNNEL_INFO_IPV6_LENGTH);

            ret = RELAY_GTPU_FAILURE;
            break;
        }

        oldSessionSapReq_p->teid = RELAY_GTPU_GET_U32BIT(&sessionReq_p[current]);
        current +=4;

        oldSessionSapReq_p->ipBytes = sessionReq_p[current ++];

        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Old Session Info\n"
                                     "TeID:%u ipBytes:%u (4 for IPv4, 16 for IPv6\n",\
                                     oldSessionSapReq_p->teid,oldSessionSapReq_p->ipBytes);

        if(RELAY_GTPU_FAILURE == parseIpfrombuf(oldSessionSapReq_p->mIp,
                    &sessionReq_p[current],
                    oldSessionSapReq_p->ipBytes,
                    &current))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_UPDATE_REQUEST:" \
                                          "Parsing Failed for Old destination multicast IP\n");
            ret = RELAY_GTPU_FAILURE;
            break;
        }
        if(RELAY_GTPU_FAILURE==parseIpfrombuf(oldSessionSapReq_p->sIp,
                    &sessionReq_p[current],
                    oldSessionSapReq_p->ipBytes,
                    &current))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed SESSION_UPDATE_REQUEST:" \
                                          "Parsing Failed for Old source IP\n");
            ret = RELAY_GTPU_FAILURE;
            break;
        }
        /*SPR 18111 fix */
        if(RELAY_GTPU_SUCCESS != updateMipAndTeid((UInt8 *)oldSessionSapReq_p->mIp,(UInt8 *)oldSessionSapReq_p->sIp,\
                    (UInt8 *) sessionSapReq_p->mIp,\
                    (UInt8 *) sessionSapReq_p->sIp,\
                    sessionSapReq_p->ipBytes,\
                    oldSessionSapReq_p->ipBytes,\
                    (UInt32 ) oldSessionSapReq_p->teid,\
                    (UInt32 ) sessionSapReq_p->teid ))
        /*SPR 18111 fix */
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Processing Failed SESSION_UPDATE_REQUEST:" \
                                          "updateMipAndTeid returned failure\n");
            ret = RELAY_GTPU_FAILURE;
            break;
        }
        /*Check whether old and new teid same or not if not then remove old teid from teid tree
         * 	and add new teid in to teid tree*/	
        if(RELAY_GTPU_SUCCESS != updateTeidTree(sessionSapReq_p,oldSessionSapReq_p))
        {
            //logging
            /*Coverity :97608 fix start*/
            ret = processMipAndTeid(sessionSapReq_p->mIp,sessionSapReq_p->sIp,sessionSapReq_p->ipBytes,
                    sessionSapReq_p->teid,
                    DEL_TRIGGER);
	    if (RELAY_GTPU_FAILURE == ret)
	    {
            	LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Processing Failed SESSION_UPDATE_REQUEST:" \
                	                          "processMipAndTeid failed in DEL_TRIGGER \n");
	    }
            ret = processMipAndTeid(oldSessionSapReq_p->mIp,oldSessionSapReq_p->sIp,oldSessionSapReq_p->ipBytes,
                    oldSessionSapReq_p->teid,
                    ADD_TRIGGER);

            /*Coverity :97608 fix end*/
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Processing Failed SESSION_UPDATE_REQUEST:" \
                                          "updateTeidTree returned failure. Restored Old Info ADD_TRIGGER ret value %d\n", ret);

            ret = RELAY_GTPU_FAILURE;
            break;
        }
        /*Trigger to M.ip */

    }while(0);
    /*Send response back to M1U*/
    currentPos = API_HDR; 


    apiId = M1U_SESSION_UPDATE_CNF ;

    respMsg[currentPos] = ret;  /* Cause */
    currentPos++;

    /* Append API Header in front of message for sending to S1-AP */
    appendApiHeader(RELAY_S1AP_MODULE_ID, apiId, transIdForRsp, currentPos, respMsg,cellIndex);

    /* Sending to M2-AP */
    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"GTPUPR->RRC M1U_SESSION_UPDATE_CNF Response:%u\n",ret);
    udpSendToS1ap((char*)respMsg, currentPos);

/*   Coverty Fix : 92761 Start*/
    if (sessionSapReq_p)
    {
    	freeMemPool(sessionSapReq_p);
    }
/*   Coverty Fix : 92761 End*/
    freeMemPool(oldSessionSapReq_p);
    return ret;
}
/****************************************************************************
 * Function Name  : updateTeidTree
 * Inputs         : TunnelInfo *newSessionSapReq_p 
 *                  TunnelInfo *oldSessionSapReq_p
 * Outputs        : None 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function updates TeidTree based on new and old session info
 ****************************************************************************/
UInt8 updateTeidTree(TunnelInfo *newSessionSapReq_p,TunnelInfo *oldSessionSapReq_p)
{
    TEIDAreaLcTreeInfo *teidTreeNode_p = PNULL;
    TEIDAreaLcTreeInfo *teidAreaLcInfo_p = PNULL;
    TEIDInfo *teidInfoNode_p=PNULL;
    TEIDInfo *teidInfo_p=PNULL;
    UInt8 nodeFound=LTE_FALSE;
    AreaLcInfoList oldAreaLcList[MAX_NUM_CELL]; // structure to store the exisiting area Lc mapping
    UInt8 oldListCount=0;
    UInt8 count=0;
    TunnelInfoNode* tunnelInfoNode_p=PNULL;
    /*
     * find teid tree node based on old teid ,sip,mip and store the area Lc mapping
     * and create a node of new teid nand insert in tree.
     * if newTeid and oldTeid are same then just update mip and sip of the old node from 
     * the new one
     * It returns if old teid is not found
     */
    teidTreeNode_p = (TEIDAreaLcTreeInfo *)teidFindTreeNode(&teidTree_g,
            &oldSessionSapReq_p->teid);
    if(PNULL != teidTreeNode_p)
    {
        teidInfoNode_p=(TEIDInfo *)getFirstListNode(&(teidTreeNode_p->teidInfoList));
        if(PNULL == teidInfoNode_p)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"No node present in TE-ID info list\n");
            return RELAY_GTPU_FAILURE;

        }
        while(teidInfoNode_p)
        {
            if((!memCmp(teidInfoNode_p->mIp,oldSessionSapReq_p->mIp,RELAY_ADDR_LEN_IPV6)) && \
                    (!memCmp(teidInfoNode_p->sIp,oldSessionSapReq_p->sIp,RELAY_ADDR_LEN_IPV6) ))
            {
                if(oldSessionSapReq_p->teid == newSessionSapReq_p->teid) // when old and new teid are same
                {
                    memCpy(teidInfoNode_p->mIp,newSessionSapReq_p->mIp,RELAY_ADDR_LEN_IPV6);
                    memCpy(teidInfoNode_p->sIp,newSessionSapReq_p->sIp,RELAY_ADDR_LEN_IPV6);
                    tunnelInfoNode_p=findTunnelInfoNode(oldSessionSapReq_p);
                    if(tunnelInfoNode_p != PNULL)
                    {
                        memCpy(&(tunnelInfoNode_p->tunnel),newSessionSapReq_p,sizeof(TunnelInfo));
                    }    
                    nodeFound=LTE_TRUE;
                    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"New TE-ID and Old TE-ID are same. "\
                                                 "Overwrite Old <MIP,SIP> with New <MIP,SIP> in TE-ID info node\n");
                    break;
                }
                oldListCount=teidInfoNode_p->listCount;
                for(count=0;count<oldListCount;count++)
                {
                    oldAreaLcList[count].areaId=teidInfoNode_p->areaLcList[count].areaId;
                    oldAreaLcList[count].lcId=teidInfoNode_p->areaLcList[count].lcId;
                }
                listDeleteNode(&(teidTreeNode_p->teidInfoList),&(teidInfoNode_p->teidAnchor));
                freeMemPool(teidInfoNode_p);
                nodeFound=LTE_TRUE;
                LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Deleting Old <MIP,SIP> Node.\n");
                break;
            }
            else
            {

                teidInfoNode_p = ( TEIDInfo *)getNextListNode(&(teidInfoNode_p->teidAnchor));
            }

        }
        if(LTE_FALSE == nodeFound)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"old Session Info not found\n");
            return RELAY_GTPU_FAILURE;
        }
        if(!listCount(&(teidTreeNode_p->teidInfoList)))
        {
            teidDeleteTreeNode(&teidTree_g,&(teidTreeNode_p)->teidAnchor);
            freeMemPool(&(teidTreeNode_p)->teidAnchor);
        }
    }
    else
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Old TE-ID node not found in TE-ID tree\n");
        return RELAY_GTPU_FAILURE;
    }
    if(oldSessionSapReq_p->teid != newSessionSapReq_p->teid)
    {
        /*Add teid in to Teid Tree */
        teidInfo_p= (TEIDInfo *)getMemFromPool(sizeof(TEIDInfo),PNULL);
        if(PNULL == teidInfo_p)
        {
            LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Mem alloc failed for teidInfo_p\n");
            return RELAY_GTPU_FAILURE;
        }
        memSet(teidInfo_p->mIp,0,RELAY_ADDR_LEN_IPV6);
        memCpy(teidInfo_p->mIp,newSessionSapReq_p->mIp,RELAY_ADDR_LEN_IPV6);
        memSet(teidInfo_p->sIp,0,RELAY_ADDR_LEN_IPV6);
        memCpy(teidInfo_p->sIp,newSessionSapReq_p->sIp,RELAY_ADDR_LEN_IPV6);
        teidInfo_p->listCount=oldListCount;
        for(count=0;count<oldListCount;count++)
        {
            teidInfo_p->areaLcList[count].areaId=oldAreaLcList[count].areaId;
            teidInfo_p->areaLcList[count].lcId=oldAreaLcList[count].lcId;
        }


        if(!teidTreeNodeCount(&teidTree_g))
        {   //create a function 
            if(RELAY_GTPU_SUCCESS != createAndInsertNodeInTeidTree(&teidAreaLcInfo_p,\
                        newSessionSapReq_p->teid))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"TEID Node insertion in  TEID tree failed\n");
                freeMemPool(teidInfo_p);
                return RELAY_GTPU_FAILURE;
            }
            listInsertNode(&(teidAreaLcInfo_p->teidInfoList),&(teidInfo_p->teidAnchor));

        }
        else
        {

            teidAreaLcInfo_p= (TEIDAreaLcTreeInfo *)teidFindTreeNode(&teidTree_g,
                    &newSessionSapReq_p->teid);
            if (PNULL == teidAreaLcInfo_p )
            {
                if(RELAY_GTPU_SUCCESS != createAndInsertNodeInTeidTree(&teidAreaLcInfo_p,\
                            newSessionSapReq_p->teid))
                {
                    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"TEID Node insertion in  TEID tree failed\n");
                    freeMemPool(teidInfo_p);
                    return RELAY_GTPU_FAILURE;
                }
            }
            listInsertNode(&(teidAreaLcInfo_p->teidInfoList),&(teidInfo_p->teidAnchor));

        }
        tunnelInfoNode_p=findTunnelInfoNode(oldSessionSapReq_p);
        if(tunnelInfoNode_p != PNULL)
        {
            memCpy(&(tunnelInfoNode_p->tunnel),newSessionSapReq_p,sizeof(TunnelInfo));
        }    
    }
    return RELAY_GTPU_SUCCESS;
}
/****************************************************************************
 * Function Name  : createAndInsertNodeInTeidTree
 * Inputs         : TEIDAreaLcTreeInfo **teidAreaLcInfo_p 
 *                  UInt32 teid
 * Outputs        : None 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion creates and inserts node in teid tree and init the list 
 ****************************************************************************/
STATIC inline  UInt8 createAndInsertNodeInTeidTree(TEIDAreaLcTreeInfo **teidAreaLcInfo_p,UInt32 teid)
{
    UInt8 resp=RELAY_GTPU_SUCCESS;
    *teidAreaLcInfo_p=(TEIDAreaLcTreeInfo *)getMemFromPool(sizeof(TEIDAreaLcTreeInfo),PNULL);
    if(PNULL == *teidAreaLcInfo_p)
    {
        LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Mem alloc failed for <TEID,AREA,LCID> node\n");
        resp=RELAY_GTPU_FAILURE;
        return resp;
    }
    (*teidAreaLcInfo_p)->teid=teid;
    teidInsertTreeNode (&teidTree_g,&((*teidAreaLcInfo_p)->teidAnchor));  
    /*SPR 18146 fix start*/
    queueInit(&((*teidAreaLcInfo_p)->teidInfoList));
    /*SPR 18146 fix end*/
    return resp;
}

/****************************************************************************
 * Function Name  : processM1uCreateAreaEntityReq
 * Inputs         : UInt8 *sessionReq_p 
 *                  UInt16 msgLen
 * Outputs        : None 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP M1U Create Area Entity request.
 * 		    1. parse the data buffer received from RRC
 * 		    2. Update the areaid&LC list
 * 		    3. fetch the teidnode by using teid Key from teidtree
 * 		    4. insert areaid&LC list in teidNode.
 ****************************************************************************/
UInt32 processM1uCreateAreaEntityReq(UInt8 *sessionReq_p, UInt16 msgLen)
{
    UInt8 respMsg[1000] = {0}; 
    UInt8 cellIndex;
    UInt16 currentPos = 0;
    UInt16 tag = 0 ,idx =0;
    UInt16 transIdForRsp = 0, apiId = 0 ,tagLen=0 ,current =0; 
    UInt32 ret = RELAY_GTPU_SUCCESS;
    UInt32 count=0;
    UInt32 configList=0;
    UInt8  index=0;
    UInt8 teidConfigInfolength=0;
    M1UCreateAreaReq* sessionSapReq_p[MAX_EMBMS_SESSIONS];
    mcchMapAreaIdNode *mcchMapAreaIdNode_p=PNULL;
    UInt8 areaIndex=0;
    UInt8 deleteAreaList[MAX_AREA_ID];
    UInt8 areaCount=0;
    UInt8 numAreaCount=0;
    TunnelInfoNode* tunnelInfoNode_p=PNULL;
    UInt8 deleteAreaFlag[MAX_AREA_ID];
    UInt8 nodeCount=0;
    UInt16 mcchUpdateTime=0;
    UInt16 currentUpdateTime=0;
    /*Adding memSet */
    memSet(sessionSapReq_p,0,MAX_EMBMS_SESSIONS);
    memSet(deleteAreaFlag,LTE_FALSE,MAX_AREA_ID);
    /* Starting 16bytes is header part parsing message below for TransactionId and apId */
    /* Extracting Transaction identifer */

    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"RRC->PR M1U_CREATE_AREA_ENTITY_REQ\n");

    transIdForRsp = RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
    cellIndex = sessionReq_p[API_HEADER_CELL_INDEX_OFFSET];
    current = API_HDR;
    if(msgLen < M1U_CREATE_AREA_MIN_LEN)
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                                      "Incorrect length:%u received min expected length:%u\n",\
                                      msgLen,M1U_CREATE_AREA_MIN_LEN);
        ret = RELAY_GTPU_FAILURE;

    }
    else
    {
        /* SPR 18458 Fix Start */

        mcchUpdateTime = RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
        current+=2;
        
        configList=sessionReq_p[current];
        current+=1;

        numAreaCount=sessionReq_p[current];
        current+=1;

        if(configList ==0 && numAreaCount == 0)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                    "configList and numAreaCount both are 0\n");
            ret = RELAY_GTPU_FAILURE;
        }


        if(mcchUpdateTime>MCCHMAPINDEX)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                    "Invalid mcchUpdateTime:%u Expected Range:[1-%u]\n",\
                    mcchUpdateTime,MCCHMAPINDEX);
            ret = RELAY_GTPU_FAILURE;
            configList=0;
            numAreaCount=0;
        }    
        /*SPR 18749 fix start*/
        while(areaCount < numAreaCount)
        {  
            deleteAreaList[areaCount]=sessionReq_p[current];
            current+=1;
            areaIndex=deleteAreaList[areaCount];
            currentUpdateTime =  mcchModificationPeriodCounter(areaIndex);
            if(currentUpdateTime == mcchUpdateTime)
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                        "currentUpdateTime:%u requestedmcchUpdateTime:%u same\n",\
                        currentUpdateTime,mcchUpdateTime);
                ret = RELAY_GTPU_FAILURE;
                configList=0;
                numAreaCount=0;

            }
            areaCount++;
        }
        /*SPR 18749 fix end*/

        /* SPR 18458 Fix End */

        while(count < configList)
        {
            if(configList > MAX_EMBMS_SESSIONS)
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                        "Invalid List Count:%u Expected Range:[1-%u]\n",\
                        configList,MAX_EMBMS_SESSIONS);

                ret = RELAY_GTPU_FAILURE;
                break;
            }
            sessionSapReq_p[count]=(M1UCreateAreaReq *)getMemFromPool(sizeof( M1UCreateAreaReq),PNULL);
            if(PNULL ==  sessionSapReq_p[count])
            {
                LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                        "Mem alloc failed for sessionSapReq_p\n");
                ret =  RELAY_GTPU_FAILURE;
                break;
            }
            /*SPR 18806 fix start*/
            memSet(sessionSapReq_p[count],0,sizeof(M1UCreateAreaReq));
            /*SPR 18806 fix end*/
            tag=RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
            if(TEID_CONFIG_INFO_LIST != tag)
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                        "Invalid Tag:%u  Expected Tag TEID_CONFIG_INFO_LIST:[%u]\n",\
                        tag,TEID_CONFIG_INFO_LIST);
                freeMemPool(sessionSapReq_p[count]);
                ret = RELAY_GTPU_FAILURE;
                break;
            }
            current+=2;
            teidConfigInfolength = LTE_GET_U16BIT(&sessionReq_p[current]);
            current += 2;
            if( teidConfigInfolength >  msgLen )
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                        "Invalid TEID_CONFIG_INFO_LIST Length:%u msgLen:%u \n",\
                        teidConfigInfolength,msgLen);
                freeMemPool(sessionSapReq_p[count]);
                ret = RELAY_GTPU_FAILURE;
                break;
            }
            /*Current position has teid of 4 bytes*/
            sessionSapReq_p[count]->tunnel.teid = RELAY_GTPU_GET_U32BIT(&sessionReq_p[current]);
            current +=4;

            sessionSapReq_p[count]->tunnel.ipBytes=sessionReq_p[current];
            current+=1;

            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"TeID:%u ipBytes:%u (4 for IPv4, 16 for IPv6\n",\
                    sessionSapReq_p[count]->tunnel.teid,\
                    sessionSapReq_p[count]->tunnel.ipBytes);

            if(RELAY_GTPU_SUCCESS != parseIpfrombuf(sessionSapReq_p[count]->tunnel.mIp,
                        &sessionReq_p[current],
                        sessionSapReq_p[count]->tunnel.ipBytes,
                        &current))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:" \
                        "Parsing Failed for destination multicast IP\n");
                freeMemPool(sessionSapReq_p[count]);
                ret = RELAY_GTPU_FAILURE ;
                break;

            }
            if(RELAY_GTPU_SUCCESS != parseIpfrombuf(sessionSapReq_p[count]->tunnel.sIp,
                        &sessionReq_p[current],
                        sessionSapReq_p[count]->tunnel.ipBytes,
                        &current))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:" \
                        "Parsing Failed for source IP\n");
                freeMemPool(sessionSapReq_p[count]);
                ret = RELAY_GTPU_FAILURE ;
                break;

            }
            sessionSapReq_p[count]->listCount = sessionReq_p[current];
            current +=1;
            /*Coverity :93766 fix start*/
            /* SPR 18387 fix start */
            if (sessionSapReq_p[count]->listCount > MAX_AREA_LIST_COUNT)
                /* SPR 18387 fix end */    
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"List count exceeds limit M1U_CREATE_AREA_ENTITY_REQ \n");
                freeMemPool(sessionSapReq_p[count]);
                ret = RELAY_GTPU_FAILURE ;
                break;
            }
            /*Coverity :93766 fix start*/
            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"<AREA,LC> List Count:%u\n",sessionSapReq_p[count]->listCount);

            for(idx=0;idx < sessionSapReq_p[count]->listCount ; idx++)
            {
                tag = LTE_GET_U16BIT(&sessionReq_p[current]);
                current += 2;
                tagLen = LTE_GET_U16BIT(&sessionReq_p[current]);
                current += 2;

                if( tagLen != AREA_LC_LIST_LEN )
                {
                    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                            "Invalid AREA_LC_LIST_LEN:%u Expected Len:%u \n",\
                            tagLen,AREA_LC_LIST_LEN);
                    freeMemPool(sessionSapReq_p[count]);
                    ret = RELAY_GTPU_FAILURE;
                    break;
                }
                /* Parse AREA_LC_LIST TLV */
                if(tag == AREA_LC_LIST)
                {
                    //correct
                    //   current+=2;
                    if(RELAY_GTPU_SUCCESS!= parseSessionSapAreaLcInfo(
                                sessionReq_p,
                                &tagLen,
                                &(sessionSapReq_p[count]->sessionSapAreaLcInfo[idx]),
                                &current
                                ))
                    {
                        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                                "parseSessionSapAreaLcInfo returns failure\n");
                        freeMemPool(sessionSapReq_p[count]);
                        ret = RELAY_GTPU_FAILURE;
                        break;
                    }
                } 
                else
                {
                    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                            "Invalid Tag:%u  Expected Tag AREA_LC_LIST:[%u]\n",\
                            tag,AREA_LC_LIST);
                    current+=2;
                    freeMemPool(sessionSapReq_p[count]);
                    ret = RELAY_GTPU_FAILURE;
                    break;
                }
            }
            if(RELAY_GTPU_FAILURE != ret)
            {
                /* Update Area Lc list in teid tree map 
                 * when new arealclist receive from upper layer accordingly insert that in tree node based on teid(key)and Mip and SIP
                 *	  |Areaid|Lcid| 	
                 * sourceIpandMIp --|AreaId|Lcid|
                 * 	  |AreaId|Lcid|
                 *  may be later on if any new list addition happend in upperlayer again same api
                 *  trigger list will be updated with existing list mapped to corresponding teid */
        /*SPR 18749 fix start*/
                for(index=0;index<sessionSapReq_p[count]->listCount;index++)
                {
                    areaIndex=sessionSapReq_p[count]->sessionSapAreaLcInfo[index].areaId;  

                    currentUpdateTime =  mcchModificationPeriodCounter(areaIndex);
                    if(currentUpdateTime == mcchUpdateTime)
                    {
                        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                                "currentUpdateTime:%u requestedmcchUpdateTime:%u same\n",\
                                currentUpdateTime,mcchUpdateTime);
                        ret = RELAY_GTPU_FAILURE;
                        configList=0;
                        numAreaCount=0;
                        break;

                    }
                }
                if(RELAY_GTPU_FAILURE == ret)
                {
                    break;
                }

        /*SPR 18749 fix end*/
            }
            else
            {
                break;
            }
            count++;

        }
        /*SPR 18749 fix start*/
        if(RELAY_GTPU_FAILURE != ret)
        {
            /*   For areas all LCs needs to be deleted*/
            areaCount=0;
            while(areaCount < numAreaCount)
            {  

                areaIndex = deleteAreaList[areaCount];
                /*coverity :100005 fix start */
                LTE_QUEUE* mcchMapAreaIdQueue_p=&(relayMcchMapIndex_g[areaIndex][mcchUpdateTime].mcchMapAreaIdQueue);
                /*coverity :100005 fix end */
                nodeCount=queueCount(mcchMapAreaIdQueue_p);
                while(nodeCount)
                {
                    mcchMapAreaIdNode_p = (mcchMapAreaIdNode *)DEQUEUE_AREA_NODE(mcchMapAreaIdQueue_p);
                    if (PNULL == mcchMapAreaIdNode_p)
                    {
                        /*No data is left on index to schedule */
                        break;
                    }
                    freeMemPool(mcchMapAreaIdNode_p);
                    nodeCount--;
                }  

                mcchMapAreaIdNode_p = (mcchMapAreaIdNode *)
                    getMemFromPool(sizeof(mcchMapAreaIdNode), PNULL);

                /****Insert Dummy Node*****/
                if(mcchMapAreaIdNode_p)
                {    
                    mcchMapAreaIdNode_p->lcId=MAX_EMBMS_LCID;
                    ENQUEUE_AREA_NODE(mcchMapAreaIdQueue_p,&(mcchMapAreaIdNode_p->mcchMapAreaIdAnchor));
                }
                areaCount++;
            }
            count=0;
            while(count<configList)
            {
                for(index=0;index<sessionSapReq_p[count]->listCount;index++)
                {
                    areaIndex=sessionSapReq_p[count]->sessionSapAreaLcInfo[index].areaId;   
                    LTE_QUEUE* mcchMapAreaIdQueue_p=&(relayMcchMapIndex_g[areaIndex][mcchUpdateTime].mcchMapAreaIdQueue);

                    if(deleteAreaFlag[areaIndex] == LTE_FALSE)
                    {    
                        nodeCount=queueCount(mcchMapAreaIdQueue_p);
                        while(nodeCount)
                        {
                            mcchMapAreaIdNode_p = (mcchMapAreaIdNode *)DEQUEUE_AREA_NODE(mcchMapAreaIdQueue_p);
                            if (PNULL == mcchMapAreaIdNode_p)
                            {
                                /*No data is left on index to schedule */
                                break;
                            }
                            freeMemPool(mcchMapAreaIdNode_p);
                            nodeCount--;
                        }
                        deleteAreaFlag[areaIndex]=LTE_TRUE;
                    }

                    mcchMapAreaIdNode_p = (mcchMapAreaIdNode *) 
                        getMemFromPool(sizeof(mcchMapAreaIdNode), PNULL);

                    mcchMapAreaIdNode_p->lcId=sessionSapReq_p[count]->sessionSapAreaLcInfo[index].lcId;
                    tunnelInfoNode_p=findTunnelInfoNode(&(sessionSapReq_p[count]->tunnel));
                    if(tunnelInfoNode_p)
                    {    
                        mcchMapAreaIdNode_p->tunnelInfoNode_p=tunnelInfoNode_p;
                        ENQUEUE_AREA_NODE(mcchMapAreaIdQueue_p,
                                &(mcchMapAreaIdNode_p->mcchMapAreaIdAnchor));
                    }

                }
                count++;
            }
        }
        /*SPR 18749 fix end*/


    }


    /*Send response back to M1U*/
    currentPos = API_HDR; 

    apiId = M1U_CREATE_AREA_ENTITY_CNF;
    respMsg[currentPos] = ret;  /* Cause */
    currentPos++;
    prepareM1UAreaResponse(respMsg,sessionSapReq_p,&currentPos,count);

    /* Append API Header in front of message for sending to S1-AP */
    appendApiHeader(RELAY_S1AP_MODULE_ID, apiId, transIdForRsp, currentPos, respMsg,cellIndex);

    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"GTPUPR->RRC M1U_CREATE_AREA_ENTITY_CNF Response:%u\n",ret);
    /* Sending to M2-AP */
    udpSendToS1ap((char*)respMsg, currentPos);
    while(count>0)
    {
        count--;
        freeMemPool(sessionSapReq_p[count]);
    }
    return ret;
}
/****************************************************************************
 * Function Name  : prepareM1UAreaResponse
 * Inputs         : UInt8 *respMsg 
 *                  M1UCreateAreaReq **sessionSapReq_p
 *                  UInt16 *currentPos_p
 *                  UInt32 count
 * Outputs        : None 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion prepares response for create and delete area request
 ****************************************************************************/
STATIC inline   void  prepareM1UAreaResponse(UInt8 *respMsg,M1UCreateAreaReq **sessionSapReq_p,UInt16 *currentPos_p,UInt32 count)
{
    UInt16 currentPos=*currentPos_p;
    RELAY_GTPU_SET_U32BIT(&respMsg[currentPos],count);
    currentPos+=4;
    while(count > 0)
    {
        count--; 
        RELAY_GTPU_SET_U16BIT(&respMsg[currentPos],TUNNEL_INFO);//tag
        currentPos+=2;
        if(RELAY_ADDR_LEN_IPV4 == sessionSapReq_p[count]->tunnel.ipBytes)
        {
            RELAY_GTPU_SET_U16BIT(&respMsg[currentPos],TUNNEL_INFO_IPV4_LENGTH);//IPV4 length
            currentPos+=2;
            RELAY_GTPU_SET_U32BIT(&respMsg[currentPos],sessionSapReq_p[count]->tunnel.teid);//teid length
            currentPos+=4;
            respMsg[currentPos]=sessionSapReq_p[count]->tunnel.ipBytes;//mip
            currentPos++;
            memCpy(&respMsg[currentPos],sessionSapReq_p[count]->tunnel.mIp,RELAY_ADDR_LEN_IPV4);
            currentPos+=RELAY_ADDR_LEN_IPV4;
            memCpy(&respMsg[currentPos],sessionSapReq_p[count]->tunnel.sIp,RELAY_ADDR_LEN_IPV4);
            currentPos+=RELAY_ADDR_LEN_IPV4;
        }
        else
        {
            RELAY_GTPU_SET_U16BIT(&respMsg[currentPos],TUNNEL_INFO_IPV6_LENGTH);//IPV4 length
            currentPos+=2;
            RELAY_GTPU_SET_U32BIT(&respMsg[currentPos],sessionSapReq_p[count]->tunnel.teid);//teid length
            currentPos+=4;
            respMsg[currentPos]=sessionSapReq_p[count]->tunnel.ipBytes;
            currentPos++;
            memCpy(&respMsg[currentPos],sessionSapReq_p[count]->tunnel.mIp,RELAY_ADDR_LEN_IPV6);
            currentPos+=RELAY_ADDR_LEN_IPV6;
            memCpy(&respMsg[currentPos],sessionSapReq_p[count]->tunnel.sIp,RELAY_ADDR_LEN_IPV6);
            currentPos+=RELAY_ADDR_LEN_IPV6;

        }

    }
    *currentPos_p=currentPos;
}

/****************************************************************************
 * Function Name  : processM1uDeleteAreaEntityReq
 * Inputs         : UInt8 *sessionReq_p 
 *                  UInt16 msgLen
 * Outputs        : None 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP M1U Delete Area Entity request.
 * 		    1. parse the data buffer received from RRC
 * 		    2. Update the areaid&LC list
 * 		    3. fetch the teidnode by using teid Key from teidtree
 * 		    4. delete areaid&LC list from teidNode.
 ****************************************************************************/

UInt32 processM1uDeleteAreaEntityReq(UInt8 *sessionReq_p, UInt16 msgLen)
{
    UInt8 respMsg[1000] = {0};
    UInt8 cellIndex;
    UInt16 currentPos = 0;
    UInt16 tag = 0 ,idx =0,configList=0;
    UInt16 transIdForRsp = 0, apiId = 0 ,tagLen=0 ,current =0; 
    UInt32 ret = RELAY_GTPU_SUCCESS;
    UInt32 count=0;
    UInt8 teidConfigInfolength=0;
    M1UCreateAreaReq* sessionSapReq_p[MAX_EMBMS_SESSIONS];
    UInt8 areaDeleteFlag[MAX_AREA_ID];
    UInt8 areaIndex=0;
    UInt8 lcId=0;
    UInt8 configCount=0;
    UInt16 mcchUpdateTime=0;
    UInt8 nodeCount=0;
    /*Adding memSet */
    memSet(sessionSapReq_p,0,MAX_EMBMS_SESSIONS);
    memSet(areaDeleteFlag,LTE_FALSE,MAX_AREA_ID);
    

    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"RRC->PR M1U_DELETE_AREA_ENTITY_REQ\n");
    /* Starting 16bytes is header part parsing message below for TransactionId and apId */
    /* Extracting Transaction identifer */
    transIdForRsp = RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
    cellIndex = sessionReq_p[API_HEADER_CELL_INDEX_OFFSET];
    current = API_HDR;
    if(msgLen < M1U_DELETE_AREA_MIN_LEN)
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_DELETE_AREA_ENTITY_REQ:"\
                                      "Incorrect length:%u received min expected length:%u\n",\
                                      msgLen,M1U_DELETE_AREA_MIN_LEN);
        ret = RELAY_GTPU_FAILURE;

    }
    else
    {
    configList=RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
    current+=2;
    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"List Count:%u\n",configList);
    while(count < configList)
    {
/*Coverity :93771 fix start*/
        if(configList > MAX_EMBMS_SESSIONS)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_CREATE_AREA_ENTITY_REQ:"\
                                          "Invalid List Count:%u Expected Range:[1-%u]\n",\
                                          configList,MAX_EMBMS_SESSIONS);

            ret = RELAY_GTPU_FAILURE;
            break;
        }
/*Coverity :93771 fix end*/

        sessionSapReq_p[count]=(M1UCreateAreaReq *)getMemFromPool(sizeof( M1UCreateAreaReq),PNULL);
        if(PNULL == sessionSapReq_p[count])
        {
            LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Parsing Failed M1U_DELETE_AREA_ENTITY_REQ:"\
                                         "Mem alloc failed for sessionSapReq_p\n");
            return RELAY_GTPU_FAILURE;
        }
        tag=RELAY_GTPU_GET_U16BIT(&sessionReq_p[current]);
        if(TEID_CONFIG_INFO_LIST != tag)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_DELETE_AREA_ENTITY_REQ:"\
                                          "Invalid Tag:%u  Expected Tag TEID_CONFIG_INFO_LIST:[%u]\n",\
                                          tag,TEID_CONFIG_INFO_LIST);   
            freeMemPool(sessionSapReq_p[count]);
            ret = RELAY_GTPU_FAILURE;
            break;
        }
        current+=2;
        teidConfigInfolength = LTE_GET_U16BIT(sessionReq_p + current);
        current += 2;
        if( teidConfigInfolength >  msgLen )
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_DELETE_AREA_ENTITY_REQ:"\
                                          "Invalid TEID_CONFIG_INFO_LIST Length:%u msgLen:%u \n",\
                                          teidConfigInfolength,msgLen);
            freeMemPool(sessionSapReq_p[count]);
            ret = RELAY_GTPU_FAILURE;
            break;
        }
        /*Current position has teid of 4 bytes*/
        sessionSapReq_p[count]->tunnel.teid = RELAY_GTPU_GET_U32BIT(&sessionReq_p[current]);
        current +=4;

        sessionSapReq_p[count]->tunnel.ipBytes=sessionReq_p[current];
        current+=1;

        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"TeID:%u ipBytes:%u (4 for IPv4, 16 for IPv6\n",\
                                     sessionSapReq_p[count]->tunnel.teid,\
                                     sessionSapReq_p[count]->tunnel.ipBytes);

        if(RELAY_GTPU_SUCCESS != parseIpfrombuf(sessionSapReq_p[count]->tunnel.mIp,
                    &sessionReq_p[current],
                    sessionSapReq_p[count]->tunnel.ipBytes,
                    &current))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_DELETE_AREA_ENTITY_REQ:" \
                                          "Parsing Failed for destination multicast IP\n");
            freeMemPool(sessionSapReq_p[count]);
            ret = RELAY_GTPU_FAILURE ;
            break;

        }
        if(RELAY_GTPU_SUCCESS != parseIpfrombuf(sessionSapReq_p[count]->tunnel.sIp,
                    &sessionReq_p[current],
                    sessionSapReq_p[count]->tunnel.ipBytes,
                    &current))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_DELETE_AREA_ENTITY_REQ:" \
                                          "Parsing Failed for source IP\n");
            freeMemPool(sessionSapReq_p[count]);
            ret = RELAY_GTPU_FAILURE ;
            break;

        }
        sessionSapReq_p[count]->listCount = *(sessionReq_p+current);
        current +=1;

        /*Coverity :93766 fix start*/
        /* SPR 18387 fix start */
        if (sessionSapReq_p[count]->listCount >  MAX_AREA_LIST_COUNT)
        /* SPR 18387 fix end */
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"List count exceeds limit M1U_CREATE_AREA_ENTITY_REQ \n");
            freeMemPool(sessionSapReq_p[count]);
            ret = RELAY_GTPU_FAILURE ;
            break;
        }
        /*Coverity :93766 fix start*/
        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"<AREA,LC> List Count:%u\n",\
                                     sessionSapReq_p[count]->listCount);

        for(idx=0;idx < sessionSapReq_p[count]->listCount ; idx++)
        {
            tag = LTE_GET_U16BIT(sessionReq_p + current);
            current += 2;
            tagLen = LTE_GET_U16BIT(sessionReq_p + current);
            current += 2;

            if( tagLen != AREA_LC_LIST_LEN )
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_DELETE_AREA_ENTITY_REQ:"\
                                              "Invalid AREA_LC_LIST_LEN:%u Expected Len:%u \n",\
                                              tagLen,AREA_LC_LIST_LEN);  
                freeMemPool(sessionSapReq_p[count]);
                ret = RELAY_GTPU_FAILURE;
                break;
            }
            /* Parse AREA_LC_LIST TLV */
            if(tag == AREA_LC_LIST)
            {
                if(RELAY_GTPU_SUCCESS!= parseSessionSapAreaLcInfo(
                            sessionReq_p,
                            &tagLen,
                            &(sessionSapReq_p[count]->sessionSapAreaLcInfo[idx]),
                            &current
                            ))
                {
                    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_DELETE_AREA_ENTITY_REQ:"\
                                                  "parseSessionSapAreaLcInfo returns failure\n");
                    freeMemPool(sessionSapReq_p[count]);
                    ret = RELAY_GTPU_FAILURE;
                    break;
                } 
            } 
            else
            {
                //inorder to go to next TLV
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed M1U_DELETE_AREA_ENTITY_REQ:"\
                                              "Invalid Tag:%u  Expected Tag AREA_LC_LIST:[%u]\n",\
                                              tag,AREA_LC_LIST);
                current+=2;
                freeMemPool(sessionSapReq_p[count]);
                ret = RELAY_GTPU_FAILURE;
                break;
            }
        }
/* SPR 18458 Fix Start */
        if(RELAY_GTPU_FAILURE != ret)
	{

		/* TeidNode will have listcount information like how many AreaId&LC list map present in TeidNode 
		 *	  |Areaid1|Lcid1| 	
		 * teid --|AreaId2|Lcid1|
		 * 	  |AreaId3|Lcid1| 
		 * based on the information received from RRC teidnode map (areaid&LClist) will get updated (with deleted info)
		 *	  
		 *	  |Areaid1|Lcid1| 	
		 * teid --|AreaId2|Lcid1| */ 

            if(RELAY_GTPU_FAILURE == DeleteAreaLclistFromTeidNode(sessionSapReq_p[count]))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Processing Failed M1U_DELETE_AREA_ENTITY_REQ:"\
                        "DeleteAreaLclistFromTeidNode returned failure\n");
                freeMemPool(sessionSapReq_p[count]);
                ret = RELAY_GTPU_FAILURE;
                break;
            }
            /* Remove all future LC configuration*/
            for(configCount=0; configCount<sessionSapReq_p[count]->listCount;configCount++)
            {
                areaIndex=sessionSapReq_p[count]->sessionSapAreaLcInfo[configCount].areaId;
                lcId=sessionSapReq_p[count]->sessionSapAreaLcInfo[configCount].lcId;
                if(areaDeleteFlag[areaIndex] == LTE_FALSE)
                {
                    for(mcchUpdateTime=0;mcchUpdateTime<=MCCHMAPINDEX;mcchUpdateTime++)
                    {
                        LTE_QUEUE* mcchMapAreaIdQueue_p=&(relayMcchMapIndex_g[areaIndex][mcchUpdateTime].mcchMapAreaIdQueue);
                        nodeCount=queueCount(mcchMapAreaIdQueue_p);
                        while(nodeCount)
                        {
                            mcchMapAreaIdNode *mcchMapAreaIdNode_p = (mcchMapAreaIdNode *)DEQUEUE_AREA_NODE(mcchMapAreaIdQueue_p);
                            if (PNULL == mcchMapAreaIdNode_p)
                            {
                                /*No data is left on index to schedule */
                                break;
                            }
                            freeMemPool(mcchMapAreaIdNode_p);
                            nodeCount--;
                        }
                    }
                    areaDeleteFlag[areaIndex]=LTE_TRUE;
                }    
                M1uAreaContext_g[areaIndex][lcId].tunnelInfoNode_p=PNULL;
                M1uAreaContext_g[areaIndex][lcId].valid = LTE_FALSE;
            }    
                
	}
/* SPR 18458 Fix End */
        else
        {
            break;
        }
        count++;

        }
    }

    /*Send response back to M1U*/
    currentPos = API_HDR; 
    apiId = M1U_DELETE_AREA_ENTITY_CNF;
    respMsg[currentPos] = ret;  /* Cause */
    currentPos++;
    /* prepare response*/
    prepareM1UAreaResponse(respMsg,sessionSapReq_p,&currentPos,count);

    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"GTPUPR->RRC M1U_DELETE_AREA_ENTITY_CNF Response:%u\n",ret);

    /* Append API Header in front of message for sending to S1-AP */
    appendApiHeader(RELAY_S1AP_MODULE_ID, apiId, transIdForRsp, currentPos, respMsg,cellIndex);
 /* Sending to M2-AP */
    udpSendToS1ap((char*)respMsg, currentPos);
/* SPR 18458 Fix Start */
    while(count>0 && (mcchUpdateTime>MCCHMAPINDEX))
/* SPR 18458 Fix End */
    {
        count--;
        freeMemPool(sessionSapReq_p[count]);
    }
    return ret;
}
/* processM1uDeleteAreaEntityReq Ends */


/* parseSessionSapAreaLcInfo Starts */      
/****************************************************************************
 * Function Name  : parseSessionSapAreaLcInfo
 * Inputs         : UInt8 *sessionReq_p 
 *                  UInt16 *tagLen
 *                  SessionSapAreaLcInfo *sessionSapAreaLcInfo_p,
 *                  UInt16 *current_p
 * Outputs        : None 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion process SAP M1U LC INFO for 
 *                  Create Area Entity request API.
 ****************************************************************************/
UInt32 parseSessionSapAreaLcInfo(
        UInt8 *sessionReq_p,
        UInt16 *tagLen_p,
        M1USessionAreaLcInfo *sessionSapAreaLcInfo_p, 
        UInt16 *current_p
        )
{
    /* Declaration and initialisation of local variables */
    UInt16 current              = *current_p;
    //UInt16 ret                  = RELAY_GTPU_SUCCESS;
    UInt8 lcId                  = 0;
    UInt8 areaId                = 0;
    UInt8 tagLen                = *tagLen_p;

    /* Read and Validate the AREA Index */
    areaId = *(sessionReq_p+current);
    /* Coverity 93752 Fix */
    if( areaId > MAX_AREA_ID ){
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Invalid AreaIndex:%u Received\n",areaId);
        return RELAY_GTPU_FAILURE;
    }
    sessionSapAreaLcInfo_p->areaId = areaId;
    current++;
    tagLen -=1;

    lcId = *(sessionReq_p+current);
    if( lcId > MAX_EMBMS_LCID )
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Invalid lcId:%u Received\n",lcId);
        return RELAY_GTPU_FAILURE;
    }
    current++;
    sessionSapAreaLcInfo_p->lcId =lcId;
    *current_p = current;

    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"AreaIndex:%u LCID:%u\n",areaId,lcId);

    return RELAY_GTPU_SUCCESS;
}
/* parseSessionSapAreaLcInfo Ends */      

/****************************************************************************
 * Function Name  : initM1uAreaContext
 * Inputs         : None
 * Outputs        : None
 * Returns        :
 * Description    : This funtion init M1U area context .
 ****************************************************************************/
STATIC void initM1uAreaContext(void)
{
    UInt8 areaIndex=0;
    UInt8 lcId=0;
    UInt16 index=0;
    for(areaIndex=0;areaIndex<MAX_AREA_ID;areaIndex++)
    {
        for (index = 0;index <= MCCHMAPINDEX ;index++) 
        {
            queueInit(&relayMcchMapIndex_g[areaIndex][index].mcchMapAreaIdQueue);
        }
        for(lcId=0;lcId<MAX_EMBMS_LCID;lcId++)
        {
            M1uAreaContext_g[areaIndex][lcId].tunnelInfoNode_p = PNULL;
            M1uAreaContext_g[areaIndex][lcId].valid = LTE_FALSE;
        }
    }
}

/****************************************************************************
 * Function Name  : initMipList
 * Inputs         : None
 * Outputs        : None
 * Returns        :
 * Description    : This funtion init mip list .
 ****************************************************************************/

void initmIpList()  

{
    /*SPR 18146 fix start*/
    queueInit(& (m1uMipList_g));
    listInit(&tunnelList_g);

    /*SPR 18146 fix end*/

}
/****************************************************************************
 * Function Name  : triggerForMIpJoining 
 * Inputs         : mIp - nulticastIp
 *                  sIp- sourceIp
 * 		            ipBytes
 * Outputs        : 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE 
 * Description    : This function process disjoining of multicast ip
 ****************************************************************************/

STATIC inline  UInt8 triggerForMIpJoining(UInt8 *mIp,UInt8 *sIp, UInt8 ipBytes)
{
    UInt8 ret=RELAY_GTPU_SUCCESS;
    Char8 mipStr[IPV6_STRING_LEN];
    Char8 sipStr[IPV6_STRING_LEN];
    struct group_source_req group_source_req;
    group_source_req.gsr_interface = 0;
    convertIpBytetoString(mipStr,mIp,ipBytes);
    convertIpBytetoString(sipStr,sIp,ipBytes);
#ifdef MAC_AUT_TEST
    if(!aut_trigger_test_g)
    return ret;
#endif
    if(RELAY_ADDR_LEN_IPV4 == ipBytes )
    {
        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Joining IPv4 Destination Multicast IP: %u.%u.%u.%u\n",\
                                     mIp[0],mIp[1],mIp[2],mIp[3]);
        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"IPv4 Source IP: %u.%u.%u.%u\n",\
                                     sIp[0],sIp[1],sIp[2],sIp[3]);
        struct sockaddr_in *group;
        struct sockaddr_in *source;
        group=(struct sockaddr_in *)&group_source_req.gsr_group;
        source=(struct sockaddr_in *)&group_source_req.gsr_source;
        group->sin_family = AF_INET;
/*Coverity :99487 fix start*/
        if(inet_pton_wrapper(AF_INET,mipStr,(void*)&group->sin_addr.s_addr) != RELAY_GTPU_SUCCESS)
	{
	    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv6 failed inet_pton_wrapper returned failure\n");
	    return RELAY_GTPU_FAILURE;
	}
/*Coverity :99487 fix end*/
        group->sin_port = 0;
        source->sin_family = AF_INET;
/*Coverity :99487 fix start*/
        if(inet_pton_wrapper(AF_INET,sipStr,&source->sin_addr.s_addr) != RELAY_GTPU_SUCCESS)
	{
	    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv6 failed inet_pton_wrapper returned failure\n");
	    return RELAY_GTPU_FAILURE;
	}
/*Coverity :99487 fix end*/
        source->sin_port = 0;
        /* now just enter a read-print loop */
        if(setsockopt_wrapper(sockRxEmbms_g,IPPROTO_IP,MCAST_JOIN_SOURCE_GROUP, &group_source_req,
                    sizeof(group_source_req))<0)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Joining of IPv4 failed\n");
            perror("setsockopt_wrapper");
            ret = RELAY_GTPU_FAILURE;
        }
    }
    else if(RELAY_ADDR_LEN_IPV6 == ipBytes)
    {
        struct sockaddr_in6 *group;
        struct sockaddr_in6 *source;
        group=(struct sockaddr_in6*)&group_source_req.gsr_group;
        source=(struct sockaddr_in6*)&group_source_req.gsr_source;
        group->sin6_family = AF_INET6;
/*Coverity :99487 fix start*/
        if(inet_pton_wrapper(AF_INET6,mipStr,(void*)&group->sin6_addr.s6_addr) != RELAY_GTPU_SUCCESS)
	{
	    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv6 failed inet_pton_wrapper returned failure\n");
	    return RELAY_GTPU_FAILURE;
	}
/*Coverity :99487 fix end*/
        group->sin6_port = 0;
        source->sin6_family = AF_INET6;
/*Coverity :99487 fix start*/
        if(inet_pton_wrapper(AF_INET6,sipStr,&source->sin6_addr.s6_addr) != RELAY_GTPU_SUCCESS)
	{
	    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv6 failed inet_pton_wrapper returned failure\n");
	    return RELAY_GTPU_FAILURE;
	}
/*Coverity :99487 fix end*/
        source->sin6_port = 0;
        /* now just enter a read-print loop */
        if(setsockopt_wrapper(sockRx6Embms_g,IPPROTO_IPV6,MCAST_JOIN_SOURCE_GROUP, &group_source_req,
                    sizeof(group_source_req))<0)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Joining of IPv6 failed\n");
            perror("setsockopt_wrapper");
            ret = RELAY_GTPU_FAILURE;
        }

    }
    else
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Joining Failed due to incorrect ipBytes:%u\n",ipBytes);
        ret = RELAY_GTPU_FAILURE;
    }
    return ret;

}
/****************************************************************************
 * Function Name  : triggerForMIpDisJoining 
 * Inputs         : mIp - nulticastIp
 *                  sIp- sourceIp
 * 		            ipBytes
 * Outputs        : 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE 
 * Description    : This function process disjoining of multicast ip
 ****************************************************************************/
STATIC inline  UInt8 triggerForMIpDisJoining(UInt8 *mIp,UInt8 *sIp,UInt8 ipBytes)
{
    UInt8 ret=RELAY_GTPU_SUCCESS;
    Char8 mipStr[IPV6_STRING_LEN];
    Char8 sipStr[IPV6_STRING_LEN];
    struct group_source_req group_source_req;
    group_source_req.gsr_interface = 0;
    convertIpBytetoString(mipStr,mIp,ipBytes);
    convertIpBytetoString(sipStr,sIp,ipBytes);
#ifdef MAC_AUT_TEST
    if(!aut_trigger_test_g)
    return ret;
#endif
    if(RELAY_ADDR_LEN_IPV4 == ipBytes )
    {
        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Disjoining IPv4 Destination Multicast IP: %u.%u.%u.%u\n",\
                                      mIp[0],mIp[1],mIp[2],mIp[3]);
        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"IPv4 Source IP: %u.%u.%u.%u\n",\
                                      sIp[0],sIp[1],sIp[2],sIp[3]);
        struct sockaddr_in *group;
        struct sockaddr_in *source;
        group=(struct sockaddr_in *)&group_source_req.gsr_group;
        source=(struct sockaddr_in *)&group_source_req.gsr_source;
        group->sin_family = AF_INET;
/*Coverity :99476 fix start*/
        if(inet_pton_wrapper(AF_INET,mipStr,(void*)&group->sin_addr.s_addr) != RELAY_GTPU_SUCCESS)
	{
	    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv6 failed inet_pton_wrapper returned failure\n");
	    return RELAY_GTPU_FAILURE;
	}
/*Coverity :99476 fix end*/
        group->sin_port = 0;
        source->sin_family = AF_INET;
/*Coverity :99476 fix start*/
        if(inet_pton_wrapper(AF_INET,sipStr,&source->sin_addr.s_addr) != RELAY_GTPU_SUCCESS)
	{
	    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv6 failed inet_pton_wrapper returned failure\n");
	    return RELAY_GTPU_FAILURE;
	}
/*Coverity :99476 fix end*/
        source->sin_port = 0;
        /* now just enter a read-print loop */
        if(setsockopt_wrapper(sockRxEmbms_g,IPPROTO_IP,MCAST_LEAVE_SOURCE_GROUP, &group_source_req,
                    sizeof(group_source_req))<0)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv4 failed\n");
            perror("setsockopt_wrapper");
            ret = RELAY_GTPU_FAILURE;
        }
    }
    else if(RELAY_ADDR_LEN_IPV6 == ipBytes)
    {
        struct sockaddr_in6 *group;
        struct sockaddr_in6 *source;
        group=(struct sockaddr_in6*)&group_source_req.gsr_group;
        source=(struct sockaddr_in6*)&group_source_req.gsr_source;
        group->sin6_family = AF_INET6;
/*Coverity :99476 fix start*/
        if(inet_pton_wrapper(AF_INET6,mipStr,(void*)&group->sin6_addr.s6_addr) != RELAY_GTPU_SUCCESS)
	{
	    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv6 failed inet_pton_wrapper returned failure\n");
	    return RELAY_GTPU_FAILURE;
	}
/*Coverity :99476 fix end*/
        group->sin6_port = 0;
        source->sin6_family = AF_INET6;
/*Coverity :99476 fix start*/
        if(inet_pton_wrapper(AF_INET6,sipStr,&source->sin6_addr.s6_addr) != RELAY_GTPU_SUCCESS)
	{
	    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv6 failed inet_pton_wrapper returned failure\n");
	    return RELAY_GTPU_FAILURE;
	}
/*Coverity :99476 fix end*/
        source->sin6_port = 0;
        /* now just enter a read-print loop */
        if(setsockopt_wrapper(sockRx6Embms_g,IPPROTO_IPV6,MCAST_LEAVE_SOURCE_GROUP, &group_source_req,
                    sizeof(group_source_req))<0)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoining of IPv6 failed\n");
            perror("setsockopt_wrapper");
            ret = RELAY_GTPU_FAILURE;
        }
    }
    else
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Disjoin Failed due to incorrect ipBytes:%u\n",ipBytes);
        ret= RELAY_GTPU_FAILURE;
    }
    return ret;

}
/****************************************************************************
 * Function Name  : processMipAndTeid 
 * Inputs         : mIp - nulticastIp
 *                  sIp - sourceIp
 *                  ipBytes
 * 		            teid - tunnelidentifier
 * 		            trigger - Either ADD/DEL
 * Outputs        : 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function process for triggering joining or disjoing 
 * 		            request for MulticastIp 
 ****************************************************************************/
UInt8 processMipAndTeid(UInt8 *mIp,
        UInt8 *sIp,
        UInt8  ipBytes,
        UInt32 teid,
        UInt8 trigger)
{
    M1UMipNodeInfo *mIpNode_p 	= PNULL;
    TeidNode *teidNode_p 	= PNULL;
    UInt8 nodeFound = LTE_FALSE ,mIpNodeFound =LTE_FALSE;
    UInt8 teidFound=LTE_FALSE;
    UInt8 duplicateNode=LTE_FALSE;
    UInt8 ret=RELAY_GTPU_SUCCESS;
    /*For Add trigger */
    if(ADD_TRIGGER == trigger)
    {
        if(!(UInt8)listCount(&(m1uMipList_g)))
        {
            /*Trigger for M.ip joining Mecanisam*/
            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Add Trigger:First Node for <MIP,SIP> List\n");

            if (RELAY_GTPU_FAILURE == triggerForMIpJoining(mIp,sIp,ipBytes))
            {
                //logging
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Add Trigger:MULTICAST IP Joining failed\n");
                return RELAY_GTPU_FAILURE;
            }

            /*List is empty so adding first entry */
            if(RELAY_GTPU_SUCCESS !=createMipandInsertinList(&mIpNode_p,mIp,sIp,ipBytes))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Add trigger:MIP node insertion failure\n");
                return RELAY_GTPU_FAILURE;
            }
            if(RELAY_GTPU_SUCCESS !=createTeidNodeandInsertInMipList(mIpNode_p,teid))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Add Trigger:TEID node insertion failure\n");
                return RELAY_GTPU_FAILURE;
            }

        }
        else
        {
            mIpNode_p = (M1UMipNodeInfo *)getFirstListNode(&(m1uMipList_g));

            if(PNULL == mIpNode_p)
            {
                LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Add Trigger:MIP Node Not Present or <MIP,SIP> List is empty\n");
                return RELAY_GTPU_FAILURE;
            }

            //LTE_LOG(LOG_INFO,PNULL,"No of nodes in List = %d\n", listCount(list_p));

            while(mIpNode_p)
            {
                if ((!memCmp(mIp,mIpNode_p->mIp,RELAY_ADDR_LEN_IPV6)) && \
                        (!memCmp(sIp,mIpNode_p->sIp,RELAY_ADDR_LEN_IPV6)))
                {
                    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Add Trigger:<MIP,SIP> Node present in  <MIP,SIP> List\n");
                    duplicateNode= checkforDuplicateSessionInfo(mIpNode_p,teid);
                    if(LTE_FALSE != duplicateNode)
                    {
                        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Add trigger:Duplicate Node found for <TEID,MIP,SIP>\n");
                        return RELAY_GTPU_FAILURE;
                    }
                    if(RELAY_GTPU_SUCCESS !=createTeidNodeandInsertInMipList(mIpNode_p,teid))
                    {
                        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Add trigger:TEID node insertion failure\n");
                        return RELAY_GTPU_FAILURE;
                    }
                    LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Add trigger:TEID:%u added  in  <MIP,SIP> Node\n",teid);
                    nodeFound=LTE_TRUE;
                    break;

                }
                else
                {
                    mIpNode_p = (M1UMipNodeInfo *)getNextListNode(&(mIpNode_p->mIpAnchor));
                }
            }
            if(nodeFound !=LTE_TRUE)
            {
                ret=triggerForMIpJoining(mIp,sIp,ipBytes);
                if(RELAY_GTPU_SUCCESS != ret )
                {
                    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Add trigger:MULICAST IP Joining failed\n");
                    return RELAY_GTPU_FAILURE;
                }
                /*List is empty so adding first entry */
                if(RELAY_GTPU_SUCCESS !=createMipandInsertinList(&mIpNode_p,mIp,sIp,ipBytes))
                {
                    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Add trigger:MIP node insertion failure\n");
                    return RELAY_GTPU_FAILURE;
                }
                if(RELAY_GTPU_SUCCESS !=createTeidNodeandInsertInMipList(mIpNode_p,teid))
                {
                    LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Add trigger:TEID node insertion failure\n");
                    return RELAY_GTPU_FAILURE;
                }
                LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Add trigger:New <MIP,SIP> Node inserted in  <MIP,SIP> List\n");
            }       
        }
    }
    else if(DEL_TRIGGER == trigger)
    {
        mIpNode_p = (M1UMipNodeInfo *)getFirstListNode(&(m1uMipList_g));
        if( PNULL == mIpNode_p)
        {
            LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Delete Trigger: MIP Node Not Present or <MIP,SIP> List is empty\n");
            return RELAY_GTPU_FAILURE;
        }
        while(mIpNode_p)
        {
            if ((!memCmp(mIp,mIpNode_p->mIp,RELAY_ADDR_LEN_IPV6)) && \
                    (!memCmp(sIp,mIpNode_p->sIp,RELAY_ADDR_LEN_IPV6)))
            {
                mIpNodeFound=LTE_TRUE;
                break;
            }
            else
            {
                mIpNode_p = (M1UMipNodeInfo *)getNextListNode(&(mIpNode_p->mIpAnchor));
                //mIpNode_p = mIpNode_p->next;
            }
        }
        if(mIpNodeFound == LTE_TRUE)
        {
            teidNode_p = (TeidNode *)getFirstListNode(&(mIpNode_p)->teIdQueue);
            if(PNULL == teidNode_p)
            {
                LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Delete Trigger: MIP Node TeID Queue is empty\n");
                return RELAY_GTPU_FAILURE;
            }
        }
        while(teidNode_p)
        {
            if(teidNode_p->teid ==teid  )
            {
                delQueueNode(&(mIpNode_p)->teIdQueue,&(teidNode_p->qN));
                freeMemPool(teidNode_p);
                teidFound =LTE_TRUE;
                LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Delete Trigger: TeID:%u Deleted\n",teid);
                break;
            }
            else
            {
                teidNode_p = ( TeidNode*)getNextListNode(&(teidNode_p->qN));
                //teidNode_p = teidNode_p->next;
            }
        }
        if(LTE_FALSE == teidFound)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Delete Trigger: TeID:%u Not Found\n",teid);
            return RELAY_GTPU_FAILURE;
        }
        if((queueCount(&(mIpNode_p)->teIdQueue)) == 0)
        {
            /* Coverity 93777 Fix End */
            /*delete Mip from the m.ip list and also raise trigger to disjoing from socket */
            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Delete Trigger:No More TEID present in <MIP,SIP> node. Raise trigger for disjoining\n");
            if(RELAY_GTPU_FAILURE == triggerForMIpDisJoining(mIp,sIp,ipBytes))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Delete Trigger:MULTICAST IP disjoin failed\n");
                return RELAY_GTPU_FAILURE;
            }
            listDeleteNode(&(m1uMipList_g),&(mIpNode_p->mIpAnchor));
            freeMemPool(mIpNode_p);
        }

    }
    else
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"INVALID Trigger: Expected: ADD_TRIGGER/DELETE_TRIGGER\n");
        return RELAY_GTPU_FAILURE;

    }
    return RELAY_GTPU_SUCCESS;
}				
/****************************************************************************
 * Function Name  : checkforDuplicateSessionInfo 
 * Inputs         : mIpNode_p - Pointer to Mip node
 *                  teid-Teid to be inserted in Teid tree
 * Outputs        : 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function checks for duplicate session Info
 * 		     
 ****************************************************************************/
STATIC inline UInt8 checkforDuplicateSessionInfo(M1UMipNodeInfo *mIpNode_p,UInt32 teid)
{
    TeidNode *teidNode_p;
    teidNode_p = (TeidNode *)getFirstListNode(&(mIpNode_p)->teIdQueue);
    if(PNULL == teidNode_p)
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"No node present in TE-ID info list\n");
        return RELAY_GTPU_FAILURE;
    }
    while(teidNode_p)
    {
        if(teidNode_p->teid ==teid  )
        {
            return LTE_TRUE;
        }
        else
        {
            teidNode_p = ( TeidNode*)getNextListNode(&(teidNode_p->qN));
            //teidNode_p = teidNode_p->next;
        }
    }
    //duplicate does not exist
    return LTE_FALSE;

}
/****************************************************************************
 * Function Name  : createTeidNodeandInsertInMipList 
 * Inputs         : mIpNode_p - Pointer to Mip node
 *                  teid-Teid to be inserted in Teid tree
 * Outputs        : 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function creates teid node and inserts in Mip list
 * 		     
 ****************************************************************************/
STATIC inline  UInt8 createTeidNodeandInsertInMipList(M1UMipNodeInfo *mIpNode_p,UInt32 teid)
{
    TeidNode *teidNode_p=PNULL;
    teidNode_p=(TeidNode *)getMemFromPool(sizeof(TeidNode),PNULL);
    if(PNULL == teidNode_p)
    {
        LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Mem alloc failed TEID Node for addition MIP Node of"\
                                     "<MIP,SIP> list\n");
        return RELAY_GTPU_FAILURE;
    }
    teidNode_p->teid =teid;
    /*enQueue first teid node in to list */
    enQueue(&(mIpNode_p)->teIdQueue,&teidNode_p->qN);
    return RELAY_GTPU_SUCCESS;
}
/****************************************************************************
 * Function Name  : createMipandInsertinList 
 * Inputs         : mIp - nulticastIp
 * 		    teid - tunnelidentifier
 * 		    trigger - Either ADD/DEL
 * Outputs        : 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function process for triggering joining or disjoing 
 * 		    request for MulticastIp 
 * 		     
 ****************************************************************************/
STATIC inline  UInt8 createMipandInsertinList(M1UMipNodeInfo **mIpNode_p,UInt8 *mIp,UInt8 *sIp,UInt8 ipBytes)
{
    *mIpNode_p=(M1UMipNodeInfo *)getMemFromPool(sizeof(M1UMipNodeInfo),PNULL);
    if(PNULL == (*mIpNode_p))
    {
        LOG_GTPUPR_FATAL(L2_SYS_FAIL,"Mem alloc failed MIP Node for adding in <MIP,SIP> list\n");
        return RELAY_GTPU_FAILURE;
    }
    memCpy((*mIpNode_p)->mIp,mIp,RELAY_ADDR_LEN_IPV6);
    memCpy((*mIpNode_p)->sIp,sIp,RELAY_ADDR_LEN_IPV6);
    (*mIpNode_p)->ipBytes=ipBytes;
    listInsertNode(&(m1uMipList_g),&((*mIpNode_p)->mIpAnchor));
    /*Intialize the teidqueue of MipNode structure*/
    /*SPR 18146 fix start*/
    queueInit((&(*mIpNode_p)->teIdQueue));
    /*SPR 18146 fix end*/
    return RELAY_GTPU_SUCCESS;

}
/****************************************************************************
 * Function Name  : UpdateMipAndTeid 
 * Inputs         : UInt8 *oldMip,
 *                   UInt8 *oldSip,
 *                  UInt8 *newMip,
 *                  UInt8 *newSip,
 *                  UInt8 ipBytes,
 *                  UInt32 oldTeid,
 *                 UInt32 newTeid
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function updates Mip and SIp list for joining and disjoining 
 * 		     
 ****************************************************************************/
        /*SPR 18111 fix */
UInt8 updateMipAndTeid(UInt8 *oldMip,UInt8 *oldSip,
        UInt8 *newMip,
        UInt8 *newSip,
        UInt8 newIpBytes,
        UInt8 oldIpBytes,
        UInt32 oldTeid,
        UInt32 newTeid)
{
        /*SPR 18111 fix */
    UInt8 teidFound = LTE_FALSE;
    UInt32 ret = RELAY_GTPU_SUCCESS;
    M1UMipNodeInfo *mIpNode_p = PNULL;
    TeidNode *teidNode_p =PNULL;
    UInt8  count = (UInt8)listCount(&(m1uMipList_g));	

    if(!count)
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"No Node present in M1U_MIP_LIST\n"); 
        return RELAY_GTPU_FAILURE;
    }
    else
    {

        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"No of nodes in List = %u\n",count);
        /*Check if new and old Mip,SIp are same then update teid in Mip list */
        if(((!memCmp(oldMip,newMip,RELAY_ADDR_LEN_IPV6)) &&
                    (!memCmp(oldSip,newSip,RELAY_ADDR_LEN_IPV6))))
        {
            mIpNode_p = (M1UMipNodeInfo *)getFirstListNode(&(m1uMipList_g));
            while(mIpNode_p)
            {
                if ((!memCmp(oldMip,mIpNode_p->mIp,RELAY_ADDR_LEN_IPV6)) && \
                        (!memCmp(oldSip,mIpNode_p->sIp,RELAY_ADDR_LEN_IPV6)))
                {
                    teidNode_p =  (TeidNode *)getFirstListNode(&(mIpNode_p)->teIdQueue);
                    while(teidNode_p)
                    {
                        if(teidNode_p->teid == newTeid )
                        {
                            /*dont need to add teid */
                            teidFound = LTE_TRUE;
                            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Old and New Session Info Are Same. "\
                                                         "No Updation Required\n");
                            break;
                        }

                        else
                        {
                            teidNode_p = ( TeidNode*)getNextListNode(&(teidNode_p->qN));
                            //teidNode_p = teidNode_p->next;
                        }
                    }
                    if(!teidFound)
                    {
                        
                        if(RELAY_GTPU_SUCCESS !=createTeidNodeandInsertInMipList(mIpNode_p,newTeid))
                        {
                            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Add Trigger:TEID node insertion failure\n");
                            return RELAY_GTPU_FAILURE;
                        }
                        /*Send trigger to delete the olde teid node from mip list*/
                        /*Since the old teid and new teid is not same so delete the old teid from oldmip list*/
        /*SPR 18111 fix */
                        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"New TE-ID added in <MIP,SIP> Node\n");
                        ret = processMipAndTeid(oldMip,oldSip,oldIpBytes,
                                oldTeid,
                                DEL_TRIGGER);
        /*SPR 18111 fix */
                        if(RELAY_GTPU_FAILURE == ret)
                        {
                            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"processMipAndTeid returns Failure\n");
                            return RELAY_GTPU_FAILURE;
                        }
                        LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Old TE-ID deleted from <MIP,SIP> Node\n");
                    }
                    break;
                }
                else
                {
                    mIpNode_p = (M1UMipNodeInfo *)getNextListNode(&(mIpNode_p->mIpAnchor));

                    //mIpNode_p = (LTE_LIST_NODE *)(mIpNode_p->mIpAnchor)->next;
                }
            }
        }
        else
        {
        /*SPR 18111 fix */
            ret = processMipAndTeid(oldMip,oldSip,oldIpBytes,
                    oldTeid,
                    DEL_TRIGGER);
        /*SPR 18111 fix */
            if(RELAY_GTPU_FAILURE == ret)
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Old TE-ID deleted from <MIP,SIP> Node\n");
                return RELAY_GTPU_FAILURE;
            }
            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Old TE-ID deleted from <MIP,SIP> Node\n");
            /*both multicast ip's are different so ,
             * 			1. add new Mip and SIP in MIP list  and update new teid in to that queue
             * 			2. del oldteid from oldmip list and delete the teid node from that queue */
            /*SPR 18111 fix */
            ret = processMipAndTeid(newMip,newSip,newIpBytes,
                    newTeid,
                    ADD_TRIGGER);
            /*SPR 18111 fix */
            if(RELAY_GTPU_FAILURE == ret)
            {
                /*SPR 18111 fix */
                processMipAndTeid(oldMip,oldSip,oldIpBytes,
                        oldTeid,
                        ADD_TRIGGER);
                /*SPR 18111 fix */
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"processMipAndTeid returns Failure. "
                                              "Restored old info\n");
                return RELAY_GTPU_FAILURE;
            }
            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"New TE-ID added in <MIP,SIP> Node\n");

        }
    }
    return RELAY_GTPU_SUCCESS;
}

/****************************************************************************
 * Function Name  : DeleteAreaLclistFromTeidNode 
 * Inputs         : sessionSapReq_p - areaLclistinfo &teid infomration 
 * Outputs        : 
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function process the following 
 * 		            1. Fetch teid node from teidtree with key as TEID
 * 		            2. Delete AreaLclist from the existing list info
 ****************************************************************************/
UInt32 DeleteAreaLclistFromTeidNode (
        M1UCreateAreaReq* sessionSapReq_p) 
{

    /* Coverity Fix 93776 */
    UInt8 indx =0 ,indx1=0 , areaIdFound=0,teidNodeListCount = 0;
    TEIDAreaLcTreeInfo* teidTreeNode_p =PNULL;
    TEIDInfo *teidInfoNode_p=PNULL;
    UInt8 nodeFound=LTE_FALSE;


    teidTreeNode_p = (TEIDAreaLcTreeInfo *)teidFindTreeNode(&teidTree_g,
            &sessionSapReq_p->tunnel.teid);

    if(PNULL != teidTreeNode_p)
    {
        teidInfoNode_p=(TEIDInfo *)getFirstListNode(&(teidTreeNode_p->teidInfoList));
        if(PNULL == teidInfoNode_p)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"No node present in TE-ID info list\n");
            return RELAY_GTPU_FAILURE;

        }
        while(teidInfoNode_p)
        {
            if((!memCmp(teidInfoNode_p->mIp,sessionSapReq_p->tunnel.mIp,RELAY_ADDR_LEN_IPV6)) && \
                    (!memCmp(teidInfoNode_p->sIp,sessionSapReq_p->tunnel.sIp,RELAY_ADDR_LEN_IPV6) ))
            {

                teidNodeListCount = teidInfoNode_p->listCount;
                for(;indx < sessionSapReq_p->listCount; indx ++)
                {
                    for(indx1 = 0;indx1 < teidNodeListCount ; indx1++)
                    {
                        if(sessionSapReq_p->sessionSapAreaLcInfo[indx].areaId == teidInfoNode_p->areaLcList[indx1].areaId)//resets the mapping
                        {
                            areaIdFound ++;
                            if(indx1 != teidNodeListCount-1)
                            {
                                teidInfoNode_p->areaLcList[indx1].areaId=teidInfoNode_p->areaLcList[teidNodeListCount-1].areaId;
                                teidInfoNode_p->areaLcList[indx1].lcId=teidInfoNode_p->areaLcList[teidNodeListCount-1].lcId;
                            }
                            teidInfoNode_p->areaLcList[teidNodeListCount-1].areaId=MAX_AREA_ID;
                            teidInfoNode_p->areaLcList[teidNodeListCount-1].lcId=MAX_EMBMS_LCID;
                            teidNodeListCount--;
                            teidInfoNode_p->listCount--;
                            break;
                        }
                    }
                }
                if(areaIdFound != sessionSapReq_p->listCount)
                {
                    /*That means as many requested lclist areaid's are not found with existing list */
                    return RELAY_GTPU_SUCCESS;
                }
                nodeFound=LTE_TRUE;
                break;
            }
            else
            {
                teidInfoNode_p = ( TEIDInfo *)getNextListNode(&(teidInfoNode_p->teidAnchor));

            }
        }
        if(LTE_FALSE == nodeFound)
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"<AREA,LC> node not found in TE-ID info list\n");
            return RELAY_GTPU_FAILURE;
        }

    }
    else
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"TE ID:%u Node Not found in TE-ID tree\n",\
                                      sessionSapReq_p->tunnel.teid);
        return RELAY_GTPU_FAILURE;
    }

    return RELAY_GTPU_SUCCESS;
}
/****************************************************************************
 * Function Name  : parseIpfrombuf
 * Inputs         : ipStr
 *                  msg_p
 *                  ipBytes
 *                  current
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This function parses Ip from buffer recieved depending on the ipBytes 
 ***************************************************************************/
STATIC inline  UInt8 parseIpfrombuf(UInt8 *ipStr,UInt8 *msg_p,UInt8 ipBytes,UInt16 *current)
{
    UInt8 ret = RELAY_GTPU_SUCCESS;
    UInt8 pos=0;
    memSet(ipStr,0,RELAY_ADDR_LEN_IPV6);
    if(RELAY_ADDR_LEN_IPV4 == ipBytes)
    {
        memCpy(ipStr,msg_p,RELAY_ADDR_LEN_IPV4);
        pos+=RELAY_ADDR_LEN_IPV4;
    }
    else if(RELAY_ADDR_LEN_IPV6 == ipBytes)
    {
        memCpy(ipStr,msg_p,RELAY_ADDR_LEN_IPV6);
        pos+=RELAY_ADDR_LEN_IPV6;
    }
    else
    {
        LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Parsing Failed: Invalid ipBytes:%u Received Expected Value:[4 or 16]\n",\
                                       ipBytes);
        ret = RELAY_GTPU_FAILURE;
    }
    *current+=pos;
    return ret;

}
/****************************************************************************
 * Function Name  : convertIpBytetoString
 * Inputs         : ipStr,
 *                  msg_p,
 *                  ipBytes
 * Outputs        : None
 * Returns        : None
 * Description    : This function converts ip in byte form to string form.
 ***************************************************************************/
STATIC inline  void convertIpBytetoString(Char8 *ipStr,UInt8 *msg_p,UInt8 ipBytes)
{
    memSet(ipStr,0,IPV6_STRING_LEN);
    if(RELAY_ADDR_LEN_IPV4 == ipBytes)
    {
        snprintf_wrapper(ipStr,IPV6_STRING_LEN,"%u.%u.%u.%u",msg_p[0],msg_p[1],\
                msg_p[2],msg_p[3]);
    }
    else
    {
        /*SPR 18111 fix */
        snprintf_wrapper(ipStr,IPV6_STRING_LEN,"%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x:%02x%02x",msg_p[0],msg_p[1],msg_p[2],msg_p[3],\
                msg_p[4],msg_p[5],msg_p[6],msg_p[7],\
                msg_p[8],msg_p[9],msg_p[10],msg_p[11],\
                msg_p[12],msg_p[13],msg_p[14],msg_p[15]);
        /*SPR 18111 fix */
    }
}
/****************************************************************************
 * Function Name  : deleteCompleteTeIdNodeFromTree
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function delete teId completely from TeId tree
 ***************************************************************************/
STATIC void deleteCompleteTeIdNodeFromTree(UInt32 teId)
{
    TEIDAreaLcTreeInfo *teidTreeNode_p = PNULL;
    TEIDInfo *teidInfoNode_p=PNULL;
    UInt8 listCount=0;
    UInt8 areaIndex=0;
    UInt8 lcId=0;

    teidTreeNode_p = (TEIDAreaLcTreeInfo *)teidFindTreeNode(&teidTree_g,&teId); //find the teid
    if(PNULL != teidTreeNode_p)
    {
        teidInfoNode_p=(TEIDInfo *)listPopNode(&(teidTreeNode_p->teidInfoList));
        while(teidInfoNode_p != PNULL)
        {
            for(listCount=0;listCount<teidInfoNode_p->listCount;listCount++)
            {
                areaIndex=teidInfoNode_p->areaLcList[listCount].areaId;
                lcId=teidInfoNode_p->areaLcList[listCount].lcId;
                M1uAreaContext_g[areaIndex][lcId].valid=LTE_FALSE;
                M1uAreaContext_g[areaIndex][lcId].tunnelInfoNode_p=PNULL;
            }    
            freeMemPool(teidInfoNode_p);
            teidInfoNode_p=(TEIDInfo *)listPopNode(&(teidTreeNode_p->teidInfoList));//get the first node
        }
        teidDeleteTreeNode(&teidTree_g,&(teidTreeNode_p)->teidAnchor);
        freeMemPool(&(teidTreeNode_p)->teidAnchor);
    }
}    


/****************************************************************************
 * Function Name  : deleteAllSessions
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function stops all sessions
 ***************************************************************************/
STATIC UInt32 deleteAllSessions(void)
{
    M1UMipNodeInfo *mIpNode_p=PNULL;
    TeidNode *teidNode_p 	= PNULL;
    TunnelInfoNode* tunnelInfoNode_p=PNULL;

    mIpNode_p = (M1UMipNodeInfo *)listPopNode(&(m1uMipList_g));
    while(mIpNode_p != PNULL)
    {
        teidNode_p=(TeidNode*)deQueue(&(mIpNode_p)->teIdQueue);    
        while(teidNode_p != PNULL)
        {
            deleteCompleteTeIdNodeFromTree(teidNode_p->teid);
            freeMemPool(teidNode_p);
            teidNode_p=(TeidNode*)deQueue(&(mIpNode_p)->teIdQueue);   
        }    
        if(RELAY_GTPU_FAILURE == triggerForMIpDisJoining(mIpNode_p->mIp,\
                                                         mIpNode_p->sIp,\
                                                         mIpNode_p->ipBytes))
        {
            LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"Delete Trigger:MULTICAST IP disjoin failed\n");
            return RELAY_GTPU_FAILURE;
        }
        freeMemPool(mIpNode_p);
        mIpNode_p = (M1UMipNodeInfo *)listPopNode(&(m1uMipList_g));
    }    
    tunnelInfoNode_p=(TunnelInfoNode*)listPopNode(&tunnelList_g);
    while(tunnelInfoNode_p != PNULL)
    {
        freeMemPool(tunnelInfoNode_p);
        tunnelInfoNode_p=(TunnelInfoNode*)listPopNode(&tunnelList_g);
    }    
    return RELAY_GTPU_SUCCESS;
}


/****************************************************************************
 * Function Name  : deleteM1uAreaLcNode
 * Inputs         : UInt8 areaIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function deletes area lc context from teId node
 ***************************************************************************/
void deleteM1uAreaLcNode(UInt8 areaIndex) 
{
    UInt8 lcId=0;
    M1UCreateAreaReq sessionSapReq;
    for(lcId=0;lcId<MAX_EMBMS_LCID;lcId++)
    {
        if(M1uAreaContext_g[areaIndex][lcId].valid == LTE_TRUE)
        {
            memCpy(&(sessionSapReq.tunnel),&(M1uAreaContext_g[areaIndex][lcId].tunnelInfoNode_p->tunnel),sizeof(TunnelInfo));
            sessionSapReq.listCount=1;
            sessionSapReq.sessionSapAreaLcInfo[0].areaId=areaIndex;           
            sessionSapReq.sessionSapAreaLcInfo[0].lcId=lcId;           
            if(RELAY_GTPU_FAILURE == DeleteAreaLclistFromTeidNode(&sessionSapReq))
            {
                LOG_GTPUPR_ERROR(L2_EMBMS_CAT,"DeleteAreaLclistFromTeidNode returned failure\n");
            }
            M1uAreaContext_g[areaIndex][lcId].valid = LTE_FALSE;
            M1uAreaContext_g[areaIndex][lcId].tunnelInfoNode_p=PNULL;
        }    
    }    
}    

/****************************************************************************
 * Function Name  : addM1uAreaLcNode
 * Inputs         : UInt8 areaIndex
 * Outputs        : None
 * Returns        : None
 * Description    : This function add area lc node into teId Tree
 ***************************************************************************/
void addM1uAreaLcNode(UInt8 areaIndex,mcchMapAreaIdNode *mcchMapAreaIdNode_p) 
{
    UInt8 lcId=0;
    M1UCreateAreaReq sessionSapReq;
    lcId=mcchMapAreaIdNode_p->lcId;
    if(M1uAreaContext_g[areaIndex][lcId].valid == LTE_FALSE)
    {
        memCpy(&(sessionSapReq.tunnel),&(mcchMapAreaIdNode_p->tunnelInfoNode_p->tunnel),sizeof(TunnelInfo));
        sessionSapReq.listCount=1;
        sessionSapReq.sessionSapAreaLcInfo[0].areaId=areaIndex;           
        sessionSapReq.sessionSapAreaLcInfo[0].lcId=lcId;           
        if(RELAY_GTPU_FAILURE == AddAreaLcListFromTeidNode(&sessionSapReq))
        {
            LOG_GTPUPR_INFO(L2_EMBMS_CAT,"Not able to add <AREA-ID,LC-ID> in teId tree\n");
        }
        else
        {
            M1uAreaContext_g[areaIndex][lcId].tunnelInfoNode_p=mcchMapAreaIdNode_p->tunnelInfoNode_p;
            M1uAreaContext_g[areaIndex][lcId].valid = LTE_TRUE;
        }    

    }    
}

/****************************************************************************
 * Function Name  : findTunnelInfoNode 
 * Inputs         : TunnelInfo* tunnelInfo_p
 * Outputs        : None
 * Returns        : TunnelInfoNode* tunnelInfoNode_p
 * Description    : This function delete Tunnel Node for Tunnel List
 ***************************************************************************/
STATIC TunnelInfoNode* findTunnelInfoNode(TunnelInfo* tunnelInfo_p)
{
      TunnelInfoNode* tunnelInfoNode_p=PNULL;
      tunnelInfoNode_p=(TunnelInfoNode*)getFirstListNode(&tunnelList_g);
      while(tunnelInfoNode_p != PNULL)
      {
          if(!memCmp(&(tunnelInfoNode_p->tunnel),tunnelInfo_p,sizeof(TunnelInfo)))
          {
             return tunnelInfoNode_p;
          }    
          tunnelInfoNode_p=(TunnelInfoNode*)getNextListNode((LTE_LIST_NODE*)tunnelInfoNode_p);
      }
      return PNULL;
}    


/****************************************************************************
 * Function Name  : deleteTunnelInfoNode 
 * Inputs         : TunnelInfo* tunnelInfo_p
 * Outputs        : None
 * Returns        : None
 * Description    : This function delete Tunnel Node for Tunnel List
 ***************************************************************************/
STATIC void deleteTunnelInfoNode(TunnelInfo* tunnelInfo_p)
{
      TunnelInfoNode* tunnelInfoNode_p=PNULL;
      tunnelInfoNode_p=findTunnelInfoNode(tunnelInfo_p);
      if(tunnelInfoNode_p != PNULL)
      {
          listDeleteNode(&tunnelList_g,(LTE_LIST_NODE*)tunnelInfoNode_p);
      }    
}    

