/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteRelayGtpuSap.c,v 1.2 2010/02/18 07:50:23 gur19479 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains variable declarations for Packet
 *                   : Relay and GTP-U Interface.
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRelayGtpuSap.c,v $
 * Revision 1.2  2010/02/18 07:50:23  gur19479
 * updated for optimization
 *
 * Revision 1.1.2.2  2010/01/20 22:34:12  gur19479
 * updated for Peer GTP Packet Handing
 *
 * Revision 1.1.2.1  2009/11/25 09:57:08  cm_lteenodeb
 * code checkin
 *
 *
 *****************************************************************************/

/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/


/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "lteRelay.h"
#include "lteRelayGtpuSap.h"
#include "lteRelayCommon.h"

//#include "cspl.h"
/******************************************************************************
  Private Definitions
 *****************************************************************************/
//static UInt32 allocateTransId(void);
/******************************************************************************
  Private Types
 *****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

//extern egtpu_global_t *p_global;
//extern void egtpu_process_msg (egtpu_global_t *, U8bit *);
extern UeContextForRsp * relayUeCnxtMngmntForTransId_g[RELAY_GTPU_MAX_TRANS_ID];

/****************************************************************************
 * Functions implementation
 *****************************************************************************/


/****************************************************************************
 * Function Name  : handleGtpuSetupSapReq
 * Inputs         : setupSapReq_p - Pointer to relaySetupSapReq,
 *                  transId - Transaction ID
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion handle SAP setup request.
 ****************************************************************************/

UInt32 handleGtpuSetupSapReq(relaySetupSapReq *setupSapReq_p, UInt32 transId)
{
    UInt8 *gtpuSetupMsg = PNULL;
    gtpuSetupMsg = (UInt8 *)qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR);
    /*+COVERITY 5.3.0 - 32129*/
    if ( PNULL == gtpuSetupMsg )
    {
        return RELAY_GTPU_FAILURE;   
    }
    /*-COVERITY 5.3.0 - 32129*/
    UInt8 startVal = 0;
#if  defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
    UInt8 selfTunnelIdPresent = 0;
#endif
    UInt32 current = CSPL_HDR;
    /* SPR 10787 Fix start */
	gtpuSetupMsg[current++] = RELAY_GTPU_IE_CELL_DATA_I;
    gtpuSetupMsg[current++] = setupSapReq_p->cellIndex;
    /* SPR 10787 Fix end */
    
    /*SPR 2952 Changes Start*/
    if (S1U_IE_TEID_SELF == setupSapReq_p->teidSelf.tag)
    {
        gtpuSetupMsg[current++] = RELAY_GTPU_IE_TEID_DATA_I;
        RELAY_GTPU_SET_U32BIT(&gtpuSetupMsg[current],setupSapReq_p->teidSelf.tunnelId);
        current +=4;
#if  defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
        selfTunnelIdPresent = 1;
#endif
    }
    /*SPR 2952 Changes End*/
    else
    {
        gtpuSetupMsg[current++] = RELAY_GTPU_IE_TRANSACTION_ID;
        RELAY_GTPU_SET_U32BIT(&gtpuSetupMsg[current],transId);
        current +=4;
    }
    if ( S1U_IE_TUNNEL_INFO == setupSapReq_p->sapFlag.tag )
    {
        gtpuSetupMsg[current++] = RELAY_GTPU_IE_SAP_FLAG;
#if  defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
		if(setupSapReq_p->sapFlag.value == S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG)
		{
        	gtpuSetupMsg[current++] = (setupSapReq_p->sapFlag.value & 0xff);
		}
		else
		{
            gtpuSetupMsg[current++] = (setupSapReq_p->sapFlag.value & 0x0f);
        }
#else
        gtpuSetupMsg[current++] = (setupSapReq_p->sapFlag.value & 0x0f);
#endif
    }
    if ( S1U_IE_QOS_PROFILE == setupSapReq_p->qosProfile.tag )
    {
        gtpuSetupMsg[current++] = RELAY_GTPU_IE_QOS_PROFILE;
        //Tweak of len
        RELAY_GTPU_SET_U16BIT(&gtpuSetupMsg[current],(setupSapReq_p->qosProfile.length-5));
        current +=2;
        for (startVal = 0; startVal <(setupSapReq_p->qosProfile.length-5); startVal++)
        {
            if ( 5 == startVal )
                gtpuSetupMsg[current++] = 0x20;
            else
                gtpuSetupMsg[current++] = setupSapReq_p->qosProfile.data[startVal];
        }
        /* SPR 4169 Fix Start */
        RELAY_GTPU_SET_U32BIT(&gtpuSetupMsg[current],setupSapReq_p->qosId);
        current +=4;
        /* SPR 4169 Fix End */
    }
    if (RELAY_GTPU_IE_SEQ_NUM == setupSapReq_p->seqNumUplink.tag)
    {
        gtpuSetupMsg[current++] = setupSapReq_p->seqNumUplink.tag;
        RELAY_GTPU_SET_U16BIT(&gtpuSetupMsg[current],setupSapReq_p->seqNumUplink.SquenceNo);
        current +=2;
        gtpuSetupMsg[current++] = setupSapReq_p->seqNumDnlink.tag;
        RELAY_GTPU_SET_U16BIT(&gtpuSetupMsg[current],setupSapReq_p->seqNumDnlink.SquenceNo);
        current +=2;
    }

    /*SPR #2431 fix Start*/ 
    if(setupSapReq_p->sapFlag.value & PR_EGTPU_TX_SAP 
#if defined (ENDC_ENABLED)  && !defined(UE_SIM_TESTING)
            || (S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG == setupSapReq_p->sapFlag.value)
#endif
            )
    {
        /*SPR #2431 fix End*/ 
#if   defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
        if (((setupSapReq_p->sapFlag.value == S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG) &&
                    (selfTunnelIdPresent))||
                (setupSapReq_p->sapFlag.value != S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG))
        {

#endif
        if (S1U_IE_TEID_PEER == setupSapReq_p->teidPeer.tag)
        {
            gtpuSetupMsg[current++] = RELAY_GTPU_IE_TEID_DATA_I;
            RELAY_GTPU_SET_U32BIT(&gtpuSetupMsg[current],setupSapReq_p->teidPeer.tunnelId);
            current +=4;
        }
        if (S1U_IE_GSN_ADDR == setupSapReq_p->transportAddr.tag)
        {
            gtpuSetupMsg[current++] = RELAY_GTPU_IE_GSN_ADDR;
            //Tweak ..need to ask
            RELAY_GTPU_SET_U16BIT(&gtpuSetupMsg[current],setupSapReq_p->transportAddr.length-4);
            current +=2;
            for (startVal = 0; startVal <(setupSapReq_p->transportAddr.length-4); startVal++)
            { 
                gtpuSetupMsg[current++] = setupSapReq_p->transportAddr.data[startVal];
            }
        }
#if   defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
        }
#endif
        /*SPR #2431 fix Start*/ 
    }   
    /*SPR #2431 fix End*/ 

    if (S1U_IE_SEQ_DISABLE == setupSapReq_p->seqFlag.tag)
    {
        gtpuSetupMsg[current++] = RELAY_GTPU_IE_SEQ_DISABLE;
        gtpuSetupMsg[current++] = setupSapReq_p->seqFlag.value;
    }
    if (S1U_IE_REORDERING_REQD == setupSapReq_p->reorderingReqd.tag)
    {
        gtpuSetupMsg[current++] = RELAY_GTPU_IE_REORDERING_REQD;
        gtpuSetupMsg[current++] = setupSapReq_p->reorderingReqd.value;
    }
    /* INTRA_ENB_HO Fix Start */
    if (S1U_IE_PDCP_EXT_FLAG_TAG == setupSapReq_p->pdcpextFlag.tag)
    {
        gtpuSetupMsg[current++] = RELAY_GTPU_IE_PDCP_EXT_FLAG;
        gtpuSetupMsg[current++] = setupSapReq_p->pdcpextFlag.value;
    }

    /* INTRA_ENB_HO Fix End */
    /*SPR #2431 fix Start*/ 
    /*START:LWA:DATA:FIX*/
    if((((setupSapReq_p->sapFlag.value & PR_EGTPU_RX_SAP) &&
            (setupSapReq_p->sapFlag.value & PR_EGTPU_FWD_SAP)))
#if defined(ENDC_ENABLED) && !defined(UE_SIM_TESTING)
          || ((setupSapReq_p->sapFlag.value == S1U_SN_TERMINTED_SPLIT_BEARER_SAP_FLAG) &&
              (!selfTunnelIdPresent))
#endif
      )
     /*END:LWA:DATA:FIX*/
    {
        gtpuSetupMsg[current++] = RELAY_GTPU_IE_GSN_ADDR;
        if (IPV4_ADDR_PRESENT & myAddrType)
        {
            RELAY_GTPU_SET_U16BIT(&gtpuSetupMsg[current],RELAY_ADDR_LEN_IPV4);
            current +=2;
            for (startVal = 0; startVal < RELAY_ADDR_LEN_IPV4; startVal++)
            {
                gtpuSetupMsg[current++] = myAddr4[startVal];
            }
        }
        else if (IPV6_ADDR_PRESENT & myAddrType)
        {
            RELAY_GTPU_SET_U16BIT(&gtpuSetupMsg[current],RELAY_ADDR_LEN_IPV6);
            current +=2;
            for (startVal = 0; startVal < RELAY_ADDR_LEN_IPV6; startVal++)
            {
                gtpuSetupMsg[current++] = myAddr6[startVal];
            }
        }
    }
    /*SPR #2431 fix End*/ 

    appendCsplHeader(EI_S1AP_MODULE_ID,GTPU_SETUP_SAP_REQ,current,gtpuSetupMsg);
    
    egtpu_process_msg(p_global, gtpuSetupMsg);

    return transId;
}

/****************************************************************************
 * Function Name  : handleGtpuReleaseSapReq
 * Inputs         : relSapReq_p - Pointer to gtpuRelSapReq
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion handle SAP Release request.
 ****************************************************************************/

UInt32 handleGtpuReleaseSapReq 
(
    gtpuRelSapReq *relSapReq_p
)
{
    UInt8 *gtpuRelMsg = PNULL;
    gtpuRelMsg = qvMsgAllocBySize (MAX_API_LEN + CSPL_HDR); 
    /* +COVERITY 32128 */
    if (!gtpuRelMsg)
    {
        return RELAY_GTPU_FAILURE;
    }
    /* +COVERITY 32128 */
    UInt32 counter = CSPL_HDR;

    gtpuRelMsg[counter++] = RELAY_GTPU_IE_TEID_DATA_I;
    if (relSapReq_p->teidSelf.tunnelId)
    {    
        RELAY_GTPU_SET_U32BIT(&gtpuRelMsg[counter],relSapReq_p->teidSelf.tunnelId);
    }    
    if (relSapReq_p->teidSelf.tunnelDlFwdId)
    {    
        RELAY_GTPU_SET_U32BIT(&gtpuRelMsg[counter],relSapReq_p->teidSelf.tunnelDlFwdId);
    }   
    if (relSapReq_p->teidSelf.tunnelUlFwdId)
    {    
        RELAY_GTPU_SET_U32BIT(&gtpuRelMsg[counter],relSapReq_p->teidSelf.tunnelUlFwdId);
    }   
    counter +=4;
    appendCsplHeader(EI_S1AP_MODULE_ID,GTPU_RELEASE_SAP_REQ,counter,gtpuRelMsg);
    egtpu_process_msg(p_global, gtpuRelMsg);
    
    return RELAY_GTPU_SUCCESS; 
}

/* + SPR 20276 */
/* - SPR 20276 */
/****************************************************************************
 * Function Name  : processPeerGtpuDataInd
 * Inputs         : gtpuDataMsg_p - Pointer to GTPU data message,
 *                  transId - Transaction ID,
 *                  totalLen - Total Length
 * Outputs        : None
 * Returns        : RELAY_GTPU_SUCCESS/RELAY_GTPU_FAILURE
 * Description    : This funtion handle Peer GTPU Data Indication.
 ****************************************************************************/
UInt32 processPeerGtpuDataInd
(
    UInt8 *gtpuDataMsg_p,
    UInt32 transId,
    UInt32 totalLen
)
{
    g_transId = transId;
    appendCsplHeaderForDL(EI_UDP_MODULE_ID,GTPU_UNITDATA_IND,totalLen,gtpuDataMsg_p);

    egtpu_process_msg(p_global, gtpuDataMsg_p);
    return RELAY_GTPU_SUCCESS; 
}
