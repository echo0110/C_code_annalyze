/************************************************************************************
*
*   FILE NAME  : sock_tools.c
*
*   DESCRIPTION: functions for transferring messages via UDP sockets.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/
#include "lteSimPdcpEsLteFramework.h"

#include <ltePdcpLteDefs.h>
#include "lteSimPdcpSockTools.h"
#include "lteSimPdcpUserUtils.h"
#include "lteSimPdcpTestStatistic.h"
#include "lteLayer2CommanTypes.h"
/************************************************************************************
 * Local definitions
 ************************************************************************************/

#define RRC_MODULE_HOST_NAME "127.0.0.1"
#define OAM_MODULE_HOST_NAME "127.0.0.1"
#define PDCP_MODULE_HOST_NAME "127.0.0.1"
#define PDCP_MODULE_CMD_HOST_NAME "127.0.0.1"
#define RLC_MODULE_HOST_NAME "127.0.0.1"
#define MAC_MODULE_HOST_NAME "127.0.0.1"
#define TRAFFIC_GEN_MODULE_HOST_NAME "127.0.0.1"
#define PACKET_RELAY_MODULE_HOST_NAME "127.0.0.1"
#define S1AP_MODULE_HOST_NAME "127.0.0.1"
#define RRM_MODULE_HOST_NAME "127.0.0.1"
#define PHY_MODULE_HOST_NAME "127.0.0.1"

#define RRC_MODULE_PORT 3211
#define OAM_MODULE_PORT 3212
#define PDCP_MODULE_PORT 3210
#define PDCP_MODULE_CMD_PORT 3213
#define RLC_MODULE_PORT 3214
#define MAC_MODULE_PORT 3216
#define TRAFFIC_GEN_MODULE_PORT 3315
#define PACKET_RELAY_OAM_PORT 9908 
#define PACKET_RELAY_S1AP_PORT 5656 
#define S1AP_MODULE_PORT 3318 
#define RRM_MODULE_PORT 3319 
#define PHY_MODULE_PORT 3320 

/* Value define for OAM-RRC Comm API */
#define MAX_EXT_MODULES 4
#define TCP_TRANSPORT_MODE 0
#define UDP_TRANSPORT_MODE 1

/* OAM-RRC Communication Request Transaction ID */
#define RRC_OAM_COMMUNICATION_TRNS_ID 0
#define INTG_TESTING 1
/************************************************************************************
 * Local variables
 ************************************************************************************/
static UInt16 attachUeIndexStartPoint_g;
extern int rohc_profile;
extern int rlc_mode_drbs;
/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

static UChar8 rrcModuleHostName[HOSTNAME_MAX_LEN]=RRC_MODULE_HOST_NAME;
static UChar8 oamModuleHostName[HOSTNAME_MAX_LEN]=OAM_MODULE_HOST_NAME;
static UChar8 pdcpModuleHostName[HOSTNAME_MAX_LEN]=PDCP_MODULE_HOST_NAME;
static UChar8 pdcpModuleCmdHostName[HOSTNAME_MAX_LEN]=PDCP_MODULE_CMD_HOST_NAME;
static UChar8 rlcModuleHostName[HOSTNAME_MAX_LEN]=RLC_MODULE_HOST_NAME;
static UChar8 macModuleHostName[HOSTNAME_MAX_LEN]=MAC_MODULE_HOST_NAME;
static UChar8 trafficGenModuleHostName[HOSTNAME_MAX_LEN]=TRAFFIC_GEN_MODULE_HOST_NAME;
static UChar8 packetRelayModuleHostName[HOSTNAME_MAX_LEN]=PACKET_RELAY_MODULE_HOST_NAME;
static UChar8 s1apModuleHostName[HOSTNAME_MAX_LEN]=S1AP_MODULE_HOST_NAME;
static UChar8 rrmModuleHostName[HOSTNAME_MAX_LEN]=RRM_MODULE_HOST_NAME;
static UChar8 phyModuleHostName[HOSTNAME_MAX_LEN]=RRM_MODULE_HOST_NAME;

static UInt16 rrcModulePort=RRC_MODULE_PORT;
static UInt16 oamModulePort=OAM_MODULE_PORT;
static UInt16 pdcpModulePort=PDCP_MODULE_PORT;
static UInt16 pdcpModuleCmdPort=PDCP_MODULE_CMD_PORT;
static UInt16 rlcModulePort=RLC_MODULE_PORT;
static UInt16 macModulePort=MAC_MODULE_PORT;
static UInt16 trafficGenModulePort=TRAFFIC_GEN_MODULE_PORT;
static UInt16 packetRelayModulePort=PACKET_RELAY_OAM_PORT;
static UInt16 packetRelayS1APModulePort=PACKET_RELAY_S1AP_PORT;
static UInt16 s1apModulePort=S1AP_MODULE_PORT;
static UInt16 rrmModulePort=RRM_MODULE_PORT;
static UInt16 phyModulePort=RRM_MODULE_PORT;

static UInt16 wiresharkPort=7580;
static UChar8 wiresharkIP[HOSTNAME_MAX_LEN]="172.16.81.4";

static struct sockaddr_in rrcAddrRx;
static struct sockaddr_in rrcAddrTx;
static struct sockaddr_in rrmAddrRx;
static struct sockaddr_in rrmAddrTx;
static struct sockaddr_in oamAddrRx;
static struct sockaddr_in pdcpAddrTx; 
static struct sockaddr_in pdcpAddrRx;
static struct sockaddr_in pdcpAddrTxCmd; 
static struct sockaddr_in pdcpAddrRxCmd;
static struct sockaddr_in rlcAddrTx; 
static struct sockaddr_in rlcAddrRx;
static struct sockaddr_in macAddrTx; 
static struct sockaddr_in macAddrRx;
static struct sockaddr_in trafficGenAddrTx;
static struct sockaddr_in packetRelayAddrRx;
static struct sockaddr_in packetRelayAddrTx;

/* socket file descriptors */
static SocketType rrcSocketRx=SOCKET_ERROR;
static SocketType rrcSocketTx=SOCKET_ERROR;
static SocketType oamSocketRx=SOCKET_ERROR;
static SocketType pdcpSocketRx=SOCKET_ERROR;
static SocketType pdcpSocketTx=SOCKET_ERROR;
static SocketType pdcpSocketRxCmd=SOCKET_ERROR;
static SocketType pdcpSocketTxCmd=SOCKET_ERROR;
static SocketType rlcSocketRx=SOCKET_ERROR;
static SocketType rlcSocketTx=SOCKET_ERROR;
static SocketType macSocketRx=SOCKET_ERROR;
static SocketType macSocketTx=SOCKET_ERROR;
static SocketType trafficGenSocketTx=SOCKET_ERROR;
static SocketType rrmSocketTx=SOCKET_ERROR;
static SocketType rrmSocketRx=SOCKET_ERROR;
static SocketType packetRelaySocketRx=SOCKET_ERROR;
static SocketType packetRelaySocketTx=SOCKET_ERROR;
static SocketType s1apSocketTx=SOCKET_ERROR;

/*block mode status*/
static UInt8 rrcSocketRxBlock=LTE_FALSE;
static UInt8 rrmSocketRxBlock=LTE_FALSE;
static UInt8 oamSocketRxBlock=LTE_FALSE;
static UInt8 pdcpSocketRxBlock=LTE_FALSE;
static UInt8 pdcpSocketRxCmdBlock=LTE_TRUE;
static UInt8 rlcSocketRxBlock=LTE_FALSE;
static UInt8 macSocketRxBlock=LTE_FALSE;
static UInt8 packetRelaySocketRxBlock=LTE_FALSE;


#define RRC_UE_IDLE                   0
#define RRC_UE_CONNECTED              1
#ifdef ZPPP_INTEGRATION
#define RRC_NAS_AUTH_COMPLETED          2
#define RRC_NAS_SEC_COMPLETED         3
#define RRC_SECURITY_COMPLETED        4
#define RRC_UE_CAPABILITY_COMPLETED   5
#define RRC_UE_RECONFIGURATION_COMPLETED       6 // Dev added
#define RRC_UE_ACTIVE                 7
#else
#define RRC_SECURITY_COMPLETED        2
#define RRC_UE_CAPABILITY_COMPLETED   3
#define RRC_UE_ACTIVE                 4
#endif

UInt8 state = RRC_UE_IDLE;
UInt8 l3InitFlag_g = 0;
UInt8 delayS1APProvReq_g = 0;
#ifdef MULTI_UE_ENHANCEMENT
  SInt16 ueIndex = -1;
  SInt16 ueDetached = 0;
 #ifdef MULTI_UE_ENHANCEMENT_FRWK
  UInt16 ueIndexRel_g = 0;
  UInt16 crntiInd_g   = 51;
 #endif
#endif
/***** static functions declaration *****/

/* returns UDP port number for given moduleId */
static UInt16 GetModulePortNum(Int moduleId);
/* returns host name (string) for given moduleId */
static UChar8 *GetModuleHostName(Int moduleId);

/* returns RX socket file descriptor for given moduleId */
static SocketType GetRxSocketFD(Int moduleId);
/* Stores RX socket file descriptor for given moduleId */
static void SetRxSocketFD(Int moduleId,SocketType fd);
/* returns TX socket file descriptor for given moduleId */
static SocketType GetTxSocketFD(Int moduleId);
/* Stores TX socket file descriptor for given moduleId */
static void SetTxSocketFD(Int moduleId,SocketType fd);
/* returns blocking mode for given moduleId */
static UInt8 GetModuleBlockingMode(Int moduleId);

/* returns pointer to RX socket address structure (sockaddr_in) for given moduleId */
static struct sockaddr_in *GetRxSocketAddr(Int moduleId);
/* returns pointer to TX socket address structure (sockaddr_in) for given moduleId */
static struct sockaddr_in *GetTxSocketAddr(Int moduleId);

/* close socket file descriptors (RX/TX) for given moduleId */
static void CloseSocket(Int moduleId);

/* intialize UDP socket with address/port */
static SocketType InitSocketAddress(void *host_name,struct sockaddr_in *s,UInt16 port,UInt8 isBlocking);

/* prepare message for RRC then send pepared message to RRC */
void rrcMessageHandling(UInt16 dstModuleId, UInt8 *msg_p, UInt32 msgLength, UInt16 apiId);
void oamMessageHandling(UInt16 dstModuleId, UInt8 *msg_p, UInt32 msgLength, UInt16 apiId);
void rrcIndComProvReqHandling(UInt8 *msgBuf_p);
void rrcOamComInfoRespHandling(UInt8 *msgBuf_p, UInt32 msgLength);
void rrcOamProvRespHandling(UInt8 *msgBuff_p, UInt32 msgLength);
void s1apIndProvReqHandling();
void s1apOamProvRespHandling(UInt8 *msgBuff_p, UInt32 msgLength);

/* extern functions */
extern void loadToSockCmd(const UChar8* cfg_fname);
extern UInt16 checkResponseMsg(const UInt8* buf_p, UInt32 length, const UInt8* checkMsg_p, UInt16 checkSize );
extern void statisticUpdate( UInt16 statItemId, UInt16 statValue );
extern UInt32 userRunScriptCallback(UInt32 argc, const UChar8** argv);

/**********************************************************************************
 * FUNCTION NAME    : CloseSockets
 * DESCRIPTION      : Closes sockets and deinitializes socket engine.
					  No other socket functions should be called after call to CloseSockets.
 * PARAMATERS       :
 * RETURN VALUE     :
 ************************************************************************************/
void CloseSockets(void)
{
	CloseSocket(RRC_MODULE_ID);
	CloseSocket(OAM_MODULE_ID);
	CloseSocket(PDCP_MODULE_ID);
	CloseSocket(PDCP_MODULE_CMD_ID);
	CloseSocket(RRM_MODULE_ID);
	CloseSocket(PACKET_RELAY_MODULE_ID);
	CloseSocket(TRAFFIC_GEN_MODULE_ID);
}

/************************************************************************************
 * FUNCTION NAME    : ReadSocketsConfig
 * DESCRIPTION      : Reads sockets config (host names and port numbers) from config file
 * PARAMATERS       : UChar8 *fn - file name of config file
 * RETURN VALUE     : void
 ************************************************************************************/
void ReadSocketsConfig(UChar8 *fn)
{
    Int i=0;
    UChar8 **strings=userGetListStringsFromCfgFile(fn);
    UChar8 *val=PNULL;
    if(PNULL!=strings)
    {
        PDCP_LOG(LOG_DUMP,"***** Reading Socket Configuration from file '%s'*****",fn);
        for(i=0; strings[i]; i++)
	{
/*SPR 21653 changes start*/

	if ((val=userIniKeyPresent("UE_L3_IP_ADDRESS",strings[i])))
                        {
                                memCpy(rrcModuleHostName,val,(strlen(val) + 1));
                        }
                        else if ((val=userIniKeyPresent("UE_L3_RX_PORT",strings[i])))
                        {
                                rrcModulePort=atoi(val);
                        }
                        else if ((val=userIniKeyPresent("UE_OAM_IP_ADDRESS",strings[i])))
                        {
                                memCpy(oamModuleHostName,val,(strlen(val) + 1));
                        }
                        else if ((val=userIniKeyPresent("UE_OAM_RX_PORT",strings[i])))
                        {
                                oamModulePort=atoi(val);
                        }
                        else if ((val=userIniKeyPresent("UE_PDCP_RX_PORT",strings[i])))
                        {
                                pdcpModulePort=atoi(val);
                        }
                        else if ((val=userIniKeyPresent("UE_RLC_RX_PORT",strings[i])))
                        {
                                rlcModulePort=atoi(val);
                        }
                        else if ((val=userIniKeyPresent("UE_MACEMULATOR_RX_PORT",strings[i])))
                        {
                                macModulePort=atoi(val);
                        }
                        else if ((val=userIniKeyPresent("UE_L2_IP_ADDRESS",strings[i])))
                        {
                                memCpy(pdcpModuleHostName,val,(strlen(val) + 1));
                                memCpy(pdcpModuleCmdHostName,val,(strlen(val) + 1));
                                memCpy(macModuleHostName,val,(strlen(val) + 1));
                                memCpy(rlcModuleHostName,val,(strlen(val) + 1));
                        }
                        else if ((val=userIniKeyPresent("WIRESHARK_PORT",strings[i])))
                        {
                                wiresharkPort=atoi(val);
                        }
                        else if ((val=userIniKeyPresent("WIRESHARK_IP",strings[i])))
                        {
                                strcpy(wiresharkIP,val);
                        }
                        else if ((val=userIniKeyPresent("UL_TG_RX_PORT",strings[i])))
                        {
                                trafficGenModulePort=atoi(val);
                        }
                        else if ((val=userIniKeyPresent("UL_TG_IP_ADDRESS",strings[i])))
                        {
                                memCpy(trafficGenModuleHostName,val,(strlen(val) + 1));
                        }

/*SPR 21653 changes end*/
        }
        userFreeListStrings(strings);
        PDCP_LOG(LOG_DUMP,"***** Reading Socket Configuration done*****");
    }
}

/************************************************************************************
 * FUNCTION NAME    : ReadFromSocket
 * DESCRIPTION      : Reads message from RX UDP socket for given moduleId
 * PARAMATERS       : buf - data from socket will be placed here. Should have RX_BUFFER_SIZE length.
					  moduleId - determines module to read from RX socket
 * RETURN VALUE     : Length of read message.
					  If there is no message it returns 0 (no wait for a message).
					  If message length != TLV header length it returns 0.
 ************************************************************************************/
UInt32 ReadFromSocket(UInt8 *buf, UInt16 moduleId)
{
    SInt32 bytesRead = -1;
    UInt32 addrLen = sizeof(struct sockaddr_in);
    SocketType fd = GetRxSocketFD(moduleId);
    struct sockaddr_in *pSockAddr = GetRxSocketAddr(moduleId);
    if ( SOCKET_ERROR == fd )
	{
		return 0; /* socket is not initialized */
	}
   
	if(LTE_TRUE==GetModuleBlockingMode(moduleId))
	{
		bytesRead = recvfrom(fd, buf, SOCKET_BUFFER_SIZE,SOCKET_FLAGS_WAIT,
				(struct sockaddr *)pSockAddr, &addrLen);
	}
	else
	{
		bytesRead = recvfrom(fd, buf, SOCKET_BUFFER_SIZE,SOCKET_FLAGS,
            (struct sockaddr *)pSockAddr, &addrLen);
    }

    if (bytesRead > 0)
    {
      UInt16 destModuleId = 0, srcModuleId = 0;
      UInt16 apiId = 0;
      
      destModuleId = LTE_GET_U16BIT(&buf[2]);
      srcModuleId = LTE_GET_U16BIT(&buf[4]);
      apiId = LTE_GET_U16BIT(&buf[6]);

      fprintf(stderr,"destModId = %d, srcModId = %d, apiId = %d, bytesRead = %d\n", 
                      destModuleId, srcModuleId, apiId, bytesRead);
      if ((srcModuleId == OAM_MODULE_ID) 
            && ((apiId == RRC_OAM_INIT_IND)||(apiId == S1AP_OAM_INIT_IND)||
            (apiId == RRC_OAM_COMMUNICATION_INFO_RESP)||(apiId == RRC_OAM_PROVISION_RESP) 
            || (apiId == S1AP_OAM_PROVISION_RESP))) 
      {
          fprintf(stderr,"Handling message for OAM\n");
          oamMessageHandling(srcModuleId, buf, bytesRead, apiId);
      }
      else if ((srcModuleId == RRC_MODULE_ID) 
            && (((destModuleId == MAC_MODULE_ID) && (apiId == MAC_CCCH_MSG_REQ))
            || ((destModuleId == PDCP_MODULE_ID) && (apiId == PDCP_SRB_DATA_IND))))
      {
          fprintf(stderr,"Handling message for RRC\n");
          rrcMessageHandling(srcModuleId, buf, bytesRead, apiId);
      }
#if 0
      if (destModuleId == PDCP_MODULE_ID || destModuleId == RLC_MODULE_ID ||destModuleId == MAC_MODULE_ID ||destModuleId == TRAFFIC_GEN_MODULE_ID || destModuleId == RRC_MODULE_ID || destModuleId == PACKET_RELAY_MODULE_ID || destModuleId == S1AP_MODULE_ID || destModuleId == RRM_MODULE_ID)
      {
        SocketType fd = GetTxSocketFD(destModuleId);
        PDCP_LOG(LOG_DETAIL,"bytes read from scoket \n");
        sendToWireshark(fd,buf,bytesRead);
      }
#endif      
    }
    if( bytesRead <= 0)
    {
       return 0; /*if no message, bytesRead will be -1 */
    }
    fprintf(stderr,"bytesRead returned = %d\n", bytesRead);
    return bytesRead;
}
/************************************************************************************
 * FUNCTION NAME    : WriteToSocket
 * DESCRIPTION      : Writes information from buffer with length bufferLen to UDP socket
 * PARAMATERS       : buffer,bufferLen
					  moduleId - determines module to write to TX socket
 * RETURN VALUE     : Number of really transferred bytes. If OK should be == bufferLen
 ************************************************************************************/
UInt32 WriteToSocket(UInt8 *buffer,UInt32 bufferLen, UInt16 moduleId)
{
	SocketType fd=GetTxSocketFD(moduleId);
	struct sockaddr_in *pSockAddr=GetTxSocketAddr(moduleId);
    Int res_len=sendto(fd,buffer,bufferLen,0,(struct sockaddr*)pSockAddr,sizeof(struct sockaddr_in));
    if(res_len!=bufferLen)
	{
		PDCP_LOG(LOG_DETAIL,"WriteToSocket res_len=%i",res_len);
	}
   
    /* To Send to wireshark */
    if(moduleId == PDCP_MODULE_ID || moduleId == RLC_MODULE_ID ||moduleId == MAC_MODULE_ID || moduleId == RRC_MODULE_ID || moduleId == PACKET_RELAY_MODULE_ID || moduleId == S1AP_MODULE_ID || moduleId == RRM_MODULE_ID)
      sendToWireshark(fd,buffer,bufferLen);
 	return res_len;
}
void sendToWireshark(SocketType fd,UInt8 *buffer, UInt32 bufferLen)
{
#if 0
    struct hostent *host_p = NULL;
    struct sockaddr_in pWiresharkSockAddr;
    host_p= (struct hostent *) gethostbyname(wiresharkIP);
    pWiresharkSockAddr.sin_family = AF_INET;
    pWiresharkSockAddr.sin_port = htons(wiresharkPort);
    pWiresharkSockAddr.sin_addr = *((struct in_addr *)host_p->h_addr);

    Int ws_len=sendto(fd,buffer,bufferLen,0,(struct sockaddr*)&pWiresharkSockAddr,sizeof(struct sockaddr_in));
    if(ws_len!=bufferLen)
    {
          PDCP_LOG(LOG_DETAIL,"WriteToSocket WIRESHARK  ws_len=%i",ws_len);
    }else
    {
      PDCP_LOG(LOG_DETAIL,"Send To WireShark\n");
    }
#endif

}

#ifdef ZPPP_INTEGRATION 
#ifdef TWO_UE_WORK
extern zppp_context zppp_flag_g[2];
static UInt8 firstIdx_g;
UInt16 detachStartPoint_g;
#endif
#endif
/****************************************************************************
 * Function Name  : eNodebRRCMsgHandler
 * Inputs         : inMsg_p
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function prepare response message for RRC request.
 ****************************************************************************/
void  eNodebRRCMsgHandler(UInt8 *bufMsg_p, UInt32 length)
{
#ifdef TDD_CONFIG
const UChar8 * file1Name_p = "../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_create_ue_tdd.scr";
#else
const UChar8 * file1Name_p = "../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_create_ue.scr";
#endif
#ifdef TWO_UE_WORK
   if ( zppp_flag_g[0].inUseFlag == 2 )
   {
       fprintf(stderr,"\n sending pdcp_srb_data_ind_ConnectionSetupComplete.cfg buffer with ueIndex = %d\n",ueIndex);
       const UChar8 *file2Name_p ="../cfg/oamrrc/pdcp_srb_data_ind_ConnectionSetupComplete.cfg";
       zppp_flag_g[0].inUseFlag = 3; 
       zppp_flag_g[0].ueIndex = ueIndex;
       loadToSockCmd(file2Name_p);
       firstIdx_g = 1;
   }
   else if ( zppp_flag_g[1].inUseFlag == 2)
   {
       fprintf(stderr,"\n sending pdcp_srb_data_ind_ConnectionSetupComplete_1.cfg buffer with ueIndex = %d\n",ueIndex);
       const UChar8 *file3Name_p ="../cfg/oamrrc/pdcp_srb_data_ind_ConnectionSetupComplete_1.cfg";
       zppp_flag_g[1].inUseFlag = 3; 
       zppp_flag_g[1].ueIndex = ueIndex;
       loadToSockCmd(file3Name_p);
       firstIdx_g = 2;
   }
   else
   {
       fprintf(stderr,"\n\n\n zppp_flag_g[0].inUseFlag = %d zppp_flag_g[1].inUseFlag = %d\n\n",zppp_flag_g[0].inUseFlag,zppp_flag_g[1].inUseFlag);
       UChar8 fileName[100] ="../cfg/oamrrc/pdcp_srb_data_ind_ConnectionSetupComplete_%d.cfg";
       sprintf(fileName,fileName,ueIndex);
       fprintf(stderr,"\n sending %s buffer with ueIndex = %d\n",fileName,ueIndex);
       loadToSockCmd(fileName);
   }
#else
   userRunScriptCallback(1,&file1Name_p);
   const UChar8 *file2Name_p ="../cfg/oamrrc/pdcp_srb_data_ind_ConnectionSetupComplete.cfg";
   loadToSockCmd(file2Name_p);
   /* To configure integrity */
   const UChar8 *fileName_p = "../cfg/oamrrc/scripts/scr/rrc/pdcp_reconfig_ue_integrity_EIA1.scr";
   userRunScriptCallback(1, &fileName_p);
#endif
}

#ifdef ZPPP_INTEGRATION 
extern SInt32 sockTxRx_g;
#ifdef TWO_UE_WORK
extern SInt32 sockTxRx_1_g;
#endif

void handleZpppControlPacket(UInt8 *buf,SInt32 length)
{
    UChar8 *fileName_p = PNULL;
    fprintf(stderr," 0x%x 0x%x\n",buf[0],buf[1]);
#ifdef TWO_UE_WORK
    if ( zppp_flag_g[0].inUseFlag == 1 )
    {
#endif
        if ( buf[0] == 0x01 && buf[1] == 0x00 )
        {
            fileName_p = "../cfg/oamrrc/mac_ccch_msg_ind_ConnectionRequest.cfg";
            fprintf(stderr,"atach triggerd from Ist zppp\n");
            loadToSockCmd(fileName_p);
#ifdef TWO_UE_WORK
	    zppp_flag_g[0].inUseFlag = 2; // in processing
#endif
        }
        else if ( buf[0] == 0x01 && buf[1] == 0x01 )
        {
            fileName_p = "../cfg/oamrrc/pdcp_srb_data_ind_Detach.cfg";
            //if ( RRC_UE_ACTIVE == state ) //uncommented by dev
            {
#ifdef TWO_UE_WORK
		zppp_flag_g[0].inUseFlag = 5; // detach processing
#endif
                loadToSockCmd(fileName_p);
#ifdef TWO_UE_WORK
		fprintf(stderr,"detach triggerd from Ist zppp zppp_flag_g[0].inUseFlag  = %d file = %s\n",zppp_flag_g[0].inUseFlag,fileName_p);
#endif
            }

        }
#ifdef TWO_UE_WORK
    }
    else if ( zppp_flag_g[1].inUseFlag == 1 )
    {
        if ( buf[0] == 0x01 && buf[1] == 0x00 )
        {
            fileName_p = "../cfg/oamrrc/mac_ccch_msg_ind_ConnectionRequest.cfg";
            fprintf(stderr,"atach triggerd from 2nd zppp\n");
            loadToSockCmd(fileName_p);
	    zppp_flag_g[1].inUseFlag = 2; // attach processing
        }
        else if ( buf[0] == 0x01 && buf[1] == 0x01 )
        {
            fileName_p = "../cfg/oamrrc/pdcp_srb_data_ind_Detach_1.cfg";
            //if ( RRC_UE_ACTIVE == state )
            {
		zppp_flag_g[1].inUseFlag = 5; // detach processing
                loadToSockCmd(fileName_p);
		fprintf(stderr,"detach triggerd from 2nd zppp zppp_flag_g[1].inUseFlag  = %d file = %s\n",zppp_flag_g[1].inUseFlag,fileName_p);
            }

        }
    }
#endif
}

void sendAttachAcceptToZpp( void )
{
    UChar8 buf[2+2+4+4] = {0};
    UInt32 pdnAddr1 = 0,pdnAddr2 = 0;
    UInt32 pdnAddr =0,dnsAddr = 0;
    SInt32 byteSent = 0;
    buf[0] = 0x01;
    buf[1] = 0x00;
#ifdef TWO_UE_WORK
    fprintf(stderr,"\n\n\n (inUseFlag = %d ueIndex = %d pdnaddr = %d) 1--(inuseFlag = %d ueIndex = %d pdnaddr = %d)\n",zppp_flag_g[0].inUseFlag,zppp_flag_g[0].ueIndex, zppp_flag_g[0].pdnAddr,zppp_flag_g[1].inUseFlag,zppp_flag_g[1].ueIndex, zppp_flag_g[1].pdnAddr);

    if ( (zppp_flag_g[0].pdnAddr != 0) && (zppp_flag_g[1].pdnAddr != 0) )
    {
        fprintf(stderr,"both the UE's laptop are connected\n");
        //attachUeIndexEndPoint_g = ueIndex;
        detachStartPoint_g = ueIndex;
        return;
    }
    pdnAddr1 = inet_addr("10.10.10.1");
    pdnAddr2 = inet_addr("10.10.10.2");
    if ( (zppp_flag_g[0].inUseFlag == 3) && (zppp_flag_g[1].inUseFlag == 6))
    {
        if ( zppp_flag_g[1].pdnAddr == pdnAddr1)
        {
        pdnAddr = pdnAddr2;
        }
        else
        {
        pdnAddr = pdnAddr1;
        }
    memcpy(&buf[2],&zppp_flag_g[0].ueIndex,sizeof(UInt16));
        zppp_flag_g[0].inUseFlag = 4;
        zppp_flag_g[0].pdnAddr = pdnAddr;
    }
    else if ( (zppp_flag_g[0].inUseFlag == 6) && (zppp_flag_g[1].inUseFlag == 3))
    {
        if ( zppp_flag_g[0].pdnAddr == pdnAddr1)
        {
        pdnAddr = pdnAddr2;
        }
        else
        {
        pdnAddr = pdnAddr1;
        }
    memcpy(&buf[2],&zppp_flag_g[1].ueIndex,sizeof(UInt16));
        zppp_flag_g[1].inUseFlag = 4;
        zppp_flag_g[1].pdnAddr = pdnAddr;
    }
    else 
    {
        if (firstIdx_g == 1)
        {
	   memcpy(&buf[2],&zppp_flag_g[0].ueIndex,sizeof(UInt16));
           pdnAddr = inet_addr("10.10.10.1");
           zppp_flag_g[0].inUseFlag = 4;
           firstIdx_g = 0;
           zppp_flag_g[0].pdnAddr = pdnAddr;
        }
        else
        {
           memcpy(&buf[2],&zppp_flag_g[1].ueIndex,sizeof(UInt16));
           pdnAddr = inet_addr("10.10.10.1");
           zppp_flag_g[1].inUseFlag = 4;
           zppp_flag_g[1].pdnAddr = pdnAddr;
           firstIdx_g = 0;
        }
        fprintf(stderr," --- 1st flag %d -- 2nd flag = %d\n",zppp_flag_g[0].inUseFlag,zppp_flag_g[1].inUseFlag);
    }
#if 0
    else
    {
        fprintf(stderr," --- 1st flag %d -- 2nd flag = %d\n",zppp_flag_g[0].inUseFlag,zppp_flag_g[1].inUseFlag);
    memcpy(&buf[2],&zppp_flag_g[1].ueIndex,sizeof(UInt16));
    pdnAddr = inet_addr("10.10.10.1");
        zppp_flag_g[1].inUseFlag = 4;
    zppp_flag_g[0].inUseFlag = 4;
    }
#endif
    dnsAddr = inet_addr("202.56.230.5");
    memcpy(&buf[2+2],&pdnAddr,sizeof(UInt32));
    memcpy(&buf[2+2+4],&dnsAddr,sizeof(UInt32));
    if ( zppp_flag_g[0].inUseFlag == 4 )
    {
    fprintf(stderr,"attach accept packet sent to Ist zppp on fd = %d\n",zppp_flag_g[0].fd);
        byteSent = send(zppp_flag_g[0].fd,buf,sizeof(buf),0);
    if ( byteSent != sizeof(buf) || byteSent < 0)
    {
        fprintf(stderr,"bytesSent = %d\n",byteSent);
    ltePanic("Error No- %d Desp: %s \n",errno,strerror(errno));
    }
        zppp_flag_g[0].inUseFlag = 6;
        
    }
    else if ( zppp_flag_g[1].inUseFlag == 4 )
    {
    fprintf(stderr,"attach accept packet sent to 2nd zppp on fd = %d\n",zppp_flag_g[1].fd);
        byteSent = send(zppp_flag_g[1].fd,buf,sizeof(buf),0);
    if ( byteSent != sizeof(buf) || byteSent < 0)
    {
        fprintf(stderr,"bytesSent = %d\n",byteSent);
    ltePanic("Error No- %d Desp: %s \n",errno,strerror(errno));
    }
        zppp_flag_g[1].inUseFlag = 6;
    }
#else

    pdnAddr = inet_addr("10.10.10.1");
    memcpy(&buf[2],&ueIndex,sizeof(UInt16));
    dnsAddr = inet_addr("202.56.230.5");


    fprintf(stderr,"\n ---------- ueIndex = %d ------\n",ueIndex);
    memcpy(&buf[2+2],&pdnAddr,sizeof(UInt32));
    memcpy(&buf[2+2+4],&dnsAddr,sizeof(UInt32));
    byteSent = send(sockTxRx_g,buf,sizeof(buf),0);
    if ( byteSent != sizeof(buf) || byteSent < 0)
    {
        fprintf(stderr,"bytesSent = %d\n",byteSent);
    ltePanic("Error No- %d Desp: %s \n",errno,strerror(errno));
    }
    fprintf(stderr,"attach accept packet sent to zppp\n");
#endif


}
void sendDetachAcceptToZpp( void )
{
    UChar8 buf[2] = {0};
    UInt32 pdnAddr =0,dnsAddr = 0;
    SInt32 byteSent = 0;
    buf[0] = 0x01;
    buf[1] = 0x01;
#ifdef TWO_UE_WORK
    if ( zppp_flag_g[0].inUseFlag == 7 )
    {
        byteSent = send(zppp_flag_g[0].fd,buf,sizeof(buf),0);
        if ( byteSent != sizeof(buf) || byteSent < 0)
        {
            fprintf(stderr,"bytesSent = %d\n",byteSent);
            ltePanic("Error No- %d Desp: %s \n",errno,strerror(errno));
        }
        zppp_flag_g[0].inUseFlag = 0;
        zppp_flag_g[0].pdnAddr = 0;
        fprintf(stderr," detach accept packet sent to Ist zppp on fd = %d\n",zppp_flag_g[0].fd);
    }
    else if ( zppp_flag_g[1].inUseFlag == 7 )
    {
        byteSent = send(zppp_flag_g[1].fd,buf,sizeof(buf),0);
        if ( byteSent != sizeof(buf) || byteSent < 0)
        {
            fprintf(stderr,"bytesSent = %d\n",byteSent);
            ltePanic("Error No- %d Desp: %s \n",errno,strerror(errno));
        }
        zppp_flag_g[1].inUseFlag = 0;
        zppp_flag_g[1].pdnAddr = 0;
        fprintf(stderr," detach accept packet sent to 2nd zppp on fd = %d\n",zppp_flag_g[1].fd);
    }
    else
    {
        fprintf(stderr,"\n\n\n zppp_flag_g[0].inUseFlag = %d zppp_flag_g[1].inUseFlag = %d\n\n",zppp_flag_g[0].inUseFlag,zppp_flag_g[1].inUseFlag);
        if ( attachUeIndexStartPoint_g != detachStartPoint_g )
            detachStartPoint_g--;
    }

#else
    byteSent = send(sockTxRx_g,buf,sizeof(buf),0);
    if ( byteSent != sizeof(buf) || byteSent < 0)
    {
        fprintf(stderr,"bytesSent = %d\n",byteSent);
        ltePanic("Error No- %d Desp: %s \n",errno,strerror(errno));
    }
    fprintf(stderr," detach accept packet sent to zppp\n");
#endif
}
void  handleNASAuthenticationReq(UInt8 *bufMsg_p, UInt32 length)
{
   UChar8 *fileName_p = PNULL;
   fileName_p ="../cfg/oamrrc/pdcp_srb_data_ind_AuthenticationResponse.cfg";
   printf("\n TWEAK - Sending Auth response with UL-NAS Transfer \nFile [%s]", fileName_p);
   loadToSockCmd(fileName_p);
   state = RRC_NAS_AUTH_COMPLETED;
}
void  handleNasSecurityMode (UInt8 *bufMsg_p, UInt32 length)
{
   UChar8 *fileName_p = PNULL;
   fileName_p = PNULL;
   fileName_p ="../cfg/oamrrc/pdcp_srb_data_ind_SecurityModeComplete.cfg";
   printf("\n TWEAK - Sending Security mode response with UL-NAS Transfer \nFile [%s]", fileName_p);
   loadToSockCmd(fileName_p);
   state = RRC_NAS_SEC_COMPLETED;
}
#endif
void  handleSecurityModeCommand(UInt8 *bufMsg_p, UInt32 length)
{
   UChar8 *fileName_p = PNULL;

   fileName_p = "../cfg/oamrrc/scripts/scr/rrc/pdcp_reconfig_ue_ciphering_UL_EEA1.scr";
   userRunScriptCallback(1, &fileName_p);

   fileName_p ="../cfg/oamrrc/pdcp_srb_data_ind_SecurityModeComplete.cfg";
   loadToSockCmd(fileName_p);


   //state = RRC_SECURITY_COMPLETED;
   state = RRC_UE_CAPABILITY_COMPLETED;
#ifdef ZPPP_INTEGRATION
   UChar8 *fileName_p = PNULL;
   fileName_p ="../cfg/oamrrc/pdcp_srb_data_ind_SecurityModeComplete_rrc.cfg";
   printf("\n TWEAK - Sending Security mode response of rrc \nFile [%s]\n", fileName_p);
   loadToSockCmd(fileName_p);
   state = RRC_SECURITY_COMPLETED;
#endif
}


void  handleUeCapabilityEnquiry(UInt8 *bufMsg_p, UInt32 length)
{
   UChar8 *fileName_p = PNULL;

   fileName_p ="../cfg/oamrrc/pdcp_srb_data_ind_capabilityInformation.cfg";
   loadToSockCmd(fileName_p);
   state = RRC_UE_CAPABILITY_COMPLETED;
}

void  handleReconfigRequest(UInt8 *bufMsg_p, UInt32 length)
{
   const UChar8 *fileName_p ="../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_reconfig_ue_withoutROHC.scr";

   /*ciphering srb reconfig for DL*/
   fileName_p = "../cfg/oamrrc/scripts/scr/rrc/pdcp_reconfig_ue_ciphering_DL_EEA1.scr";
   userRunScriptCallback(1, &fileName_p);

   fprintf(stderr,"rohc profile is %d \n ", rohc_profile);
   if (rlc_mode_drbs == 0)
   {
        fprintf(stderr,"RLC Mode is RLC_UM \n");
        if(rohc_profile < 0)
             fileName_p ="../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_reconfig_ue_withoutROHC.scr";
        else if(rohc_profile == 0)
             fileName_p ="../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_reconfig_ue_rohcprofile0.scr";
        else if(rohc_profile == 2)
             fileName_p ="../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_reconfig_ue_rohcprofile2.scr";
   }
   else if(rlc_mode_drbs == 1)
   {
        fprintf(stderr,"RLC Mode is RLC_AM \n");
        if(rohc_profile < 0)
             fileName_p ="../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_reconfig_ue_withoutROHC_am.scr";
        else if(rohc_profile == 0)
             fileName_p ="../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_reconfig_ue_rohcprofile0_am.scr";
        else if(rohc_profile == 2)
             fileName_p ="../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_reconfig_ue_rohcprofile2_am.scr";
   }
   userRunScriptCallback(1,&fileName_p);
   fileName_p ="../cfg/oamrrc/pdcp_srb_data_ind_ConnectionReconfigurationComplete.cfg";
   loadToSockCmd(fileName_p);
#ifndef ZPPP_INTEGRATION
   state = RRC_UE_ACTIVE;
#else
   state = RRC_UE_RECONFIGURATION_COMPLETED;
#endif
}

void  handleNASAttachComplete(UInt8 *bufMsg_p, UInt32 length)
{
   UChar8 *fileName_p = PNULL;
   fileName_p ="../cfg/oamrrc/pdcp_srb_data_ind_AttachComplete.cfg";
   loadToSockCmd(fileName_p);
   state = RRC_UE_ACTIVE;
#ifdef ZPPP_INTEGRATION
   /* send attach accept to zppp */
   sendAttachAcceptToZpp();
#endif

}

void  handleReleaseRequest(UInt8 *bufMsg_p, UInt32 length)
{
   const UChar8 * fileName_p = "../cfg/oamrrc/scripts/scr/rrc/ueSide_layer2_delete_ue.scr";

#ifdef MULTI_UE_ENHANCEMENT_FRWK
   ueIndexRel_g  = LTE_GET_U16BIT(&bufMsg_p[10]);
#endif
   userRunScriptCallback(1,&fileName_p);
   state = RRC_UE_IDLE;
#ifdef ZPPP_INTEGRATION
   sendDetachAcceptToZpp();
#endif
}

/****************************************************************************
 * Function Name  : oamMessageHandling
 * Inputs         : inMsg_p
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function prepare response message for OAM request.
 ****************************************************************************/
void oamMessageHandling(UInt16 dstModuleId, UInt8 *bufMsg_p, UInt32 length, UInt16 apiId)
{
    switch(apiId)
    {
        case RRC_OAM_INIT_IND:
            fprintf(stderr,"\n\nRRC_OAM_INIT_IND Received\n\n");
            rrcIndComProvReqHandling(bufMsg_p);
            break;

        case S1AP_OAM_INIT_IND:
            fprintf(stderr,"\n\nS1AP_OAM_INIT_IND Received\n\n");
            delayS1APProvReq_g++;
            if(delayS1APProvReq_g == 2) {
                s1apIndProvReqHandling();
                fprintf(stderr,"\n\nSent S1AP_OAM_PROVISION_REQ\n\n");
                delayS1APProvReq_g = 0;
            }    
            break;

        case RRC_OAM_COMMUNICATION_INFO_RESP:
            fprintf(stderr,"\n\nRRC_OAM_COMMUNICATION_INFO_RESP Received\n\n");
            rrcOamComInfoRespHandling(bufMsg_p, length);
            break;

        case RRC_OAM_PROVISION_RESP:
            fprintf(stderr,"\n\nRRC_OAM_PROVISION_RESP Received\n\n");
            rrcOamProvRespHandling(bufMsg_p, length);

            delayS1APProvReq_g++;
            if(delayS1APProvReq_g == 2) {
                s1apIndProvReqHandling();
                fprintf(stderr,"\n\nSent S1AP_OAM_PROVISION_REQ\n\n");
                delayS1APProvReq_g = 0;
            }
            break;
            
        case S1AP_OAM_PROVISION_RESP:
            fprintf(stderr,"\n\nS1AP_OAM_PROVISION_RESP Received\n\n");
            s1apOamProvRespHandling(bufMsg_p, length);
            break;
            
        default :
            fprintf(stderr,"\n\nWARNING: unsupported API [%d] received for OAM........!! \n",apiId);
            break;
    }
}





/****************************************************************************
 * Function Name  : rrcMessageHandling
 * Inputs         : inMsg_p
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function prepare response message for RRC request.
 ****************************************************************************/
void rrcMessageHandling(UInt16 dstModuleId, UInt8 *bufMsg_p, UInt32 length, UInt16 apiId)
{

    fprintf(stderr, "\nInside rrcMessageHandling function\n");
    switch(apiId)
    {
#if 0      
        case RRC_OAM_INIT_IND:
//            if(dstModuleId == OAM_MODULE_ID) {
                fprintf(stderr,"\n\nRRC_OAM_INIT_IND Received\n\n");
                rrcIndComProvReqHandling(bufMsg_p);
  //          }    
            break;

        case S1AP_OAM_INIT_IND:
    //        if(dstModuleId == OAM_MODULE_ID) {
                fprintf(stderr,"\n\nS1AP_OAM_INIT_IND Received\n\n");
                delayS1APProvReq_g++;
                if(delayS1APProvReq_g == 2) {
                    s1apIndProvReqHandling();
                    fprintf(stderr,"\n\nSent S1AP_OAM_PROVISION_REQ\n\n");
                    delayS1APProvReq_g = 0;
      //          }
            }    
            break;

        case RRC_OAM_COMMUNICATION_INFO_RESP:
        //    if(dstModuleId == OAM_MODULE_ID) {
                fprintf(stderr,"\n\nRRC_OAM_COMMUNICATION_INFO_RESP Received\n\n");
                rrcOamComInfoRespHandling(bufMsg_p, length);
          //  }    
            break;

        case RRC_OAM_PROVISION_RESP:
           // if(dstModuleId == OAM_MODULE_ID) {
                fprintf(stderr,"\n\nRRC_OAM_PROVISION_RESP Received\n\n");
                rrcOamProvRespHandling(bufMsg_p, length);

                delayS1APProvReq_g++;
                if(delayS1APProvReq_g == 2) {
                    s1apIndProvReqHandling();
                    fprintf(stderr,"\n\nSent S1AP_OAM_PROVISION_REQ\n\n");
                    delayS1APProvReq_g = 0;
                }
          //  }    
            break;
            
        case S1AP_OAM_PROVISION_RESP:
            //if(dstModuleId == OAM_MODULE_ID) {
                fprintf(stderr,"\n\nS1AP_OAM_PROVISION_RESP Received\n\n");
                s1apOamProvRespHandling(bufMsg_p, length);
           // }    
            break;
#endif            
        case MAC_CCCH_MSG_REQ:
                fprintf(stderr,"\n\nMAC_CCCH_MSG_REQ Received\n\n");
                crntiInd_g = LTE_GET_U16BIT(&bufMsg_p[10]);/*Fetch the CCH Indicated CRNTI*/

                ueIndex = ueIndex + 1;
                if(ueIndex >= LTE_MAX_UE_ID )
                {
                  ueIndex = 0;
                  ueDetached = 0;
                }
#ifdef TWO_UE_WORK
                 
                if ( (zppp_flag_g[0].pdnAddr != 0) && (zppp_flag_g[1].pdnAddr != 0) && (attachUeIndexStartPoint_g == 0) )
                {
                    attachUeIndexStartPoint_g = ueIndex;
                }
#endif
                state = RRC_UE_CONNECTED;
                eNodebRRCMsgHandler(bufMsg_p, length);
            break;
        case PDCP_SRB_DATA_IND:
            fprintf(stderr,"\n\nPDCP_SRB_DATA_IND Received\n\n");
            if(RRC_UE_CONNECTED == state) 
#ifdef ZPPP_INTEGRATION
            {
                fprintf(stderr,"Handling Authentication Message\n");
                handleNASAuthenticationReq(bufMsg_p, length);
            } 
            else if (RRC_NAS_AUTH_COMPLETED  == state) {
                fprintf(stderr,"\n\nHandling NAS Security Request\n\n");
                handleNasSecurityMode(bufMsg_p, length);
            }
            else if (RRC_NAS_SEC_COMPLETED  == state) {
                fprintf(stderr,"\n\nHandling Security Mode \n\n");
                handleSecurityModeCommand(bufMsg_p, length);
            }
#else
            {
                fprintf(stderr,"Handling Security Mode Command\n");
                handleSecurityModeCommand(bufMsg_p, length);
            } 
#endif
            else if (RRC_SECURITY_COMPLETED == state) {
                fprintf(stderr,"\n\nHandling UE Capability Enquiry Request\n\n");
                handleUeCapabilityEnquiry(bufMsg_p, length);
            }
            else if (RRC_UE_CAPABILITY_COMPLETED == state) {
                fprintf(stderr,"\n\nHandling RRC Reconfiguration Request\n\n");
                handleReconfigRequest(bufMsg_p, length);
            }
#ifdef ZPPP_INTEGRATION
            else if (RRC_UE_RECONFIGURATION_COMPLETED == state){
                fprintf(stderr,"\n\nHandling NAS Attach Complete Request\n\n");    
                handleNASAttachComplete(bufMsg_p, length);
            }     
#endif
            else if (RRC_UE_ACTIVE == state) {
                fprintf(stderr,"\n\nHandling RRC Release command\n\n");
                handleReleaseRequest(bufMsg_p, length);
            }
            else {
                fprintf(stderr,"\n\nWARNING::: Invalid state, ignoring message\n\n");
            }  
            break;

        default :
            fprintf(stderr,"\n\nWARNING: unsupported API [%d] received from RRC........!! \n",apiId);
            break;
    }
    fprintf(stderr, "Exiting rrcMessageHandling function\n");
}

/****************************************************************************
 * Function Name  : sendIndToRrm
 * Inputs         : buffer
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function send message to RRM.
 ****************************************************************************/

UInt16 sendIndToRrm(UInt8 *buffer)
{
    UInt16 current = 0;
    if(buffer)
    {
        LTE_SET_U16BIT(&buffer[current],RRC_OAM_COMMUNICATION_TRNS_ID);
        current +=2;
        LTE_SET_U16BIT(&buffer[current],OAM_MODULE_ID);
        current +=2;
        LTE_SET_U16BIT(&buffer[current],RRM_MODULE_ID);
        current +=2;
        LTE_SET_U16BIT(&buffer[current],0x50);
        current +=2;
        LTE_SET_U16BIT(&buffer[current],current +2);
        current +=2;
    }
    return current;
}

/****************************************************************************
 * Function Name  : s1apIndProvReqHandling
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function sends provision request to S1AP. 
 ****************************************************************************/
void s1apIndProvReqHandling()
{
    /* RRC-S1AP Provision Req message Sending */
    /* CFG Script file name for S1AP cfg file for Porvision Req API */
    UChar8 * filename_p="scripts/oam/s1ap/oam_s1ap_prov_info.cfg";
    loadToSockCmd(filename_p);
}

/****************************************************************************
 * Function Name  : rrcIndComProvReqHandling
 * Inputs         : inMsg_p
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function first send confirmation for rrc indication 
                    message than send preapares oam-rrc commucnication request
                    and send this request, after sending this request this 
                    functions sends provision request.
 ****************************************************************************/
void rrcIndComProvReqHandling(UInt8 *inMsg_p)
{
    UInt8 outMsg[SOCKET_BUFFER_SIZE] = {0};
    UInt16 current = 0;
    UInt16 apiId = 0;
    UInt32 transId = 0;
    UChar8 tempHostName[HOSTNAME_MAX_LEN];
    UChar8 *token_p;

    outMsg[current] = inMsg_p[current];
    current += 1;
    outMsg[current] = inMsg_p[current];
    current += 1;
    outMsg[current] = inMsg_p[current+2];
    current +=1;
    outMsg[current] = inMsg_p[current+2];
    current += 1;
    outMsg[current] = inMsg_p[current-2];
    current += 1;
    outMsg[current] = inMsg_p[current-2];
    current += 1;
    /*apiId  = LTE_GET_U16BIT(&inMsg_p[current]);
    apiId += 1;
    LTE_SET_U16BIT(&outMsg[current],apiId);
    current += 2;
    //LTE_SET_U16BIT(&outMsg[current], current + 2);
    current += 2;
    fprintf(stderr," Send buffer to RRC is %d bytes api Id = %d\n", current, apiId); 
    WriteToSocket(outMsg, current, RRC_MODULE_ID);*/
    current = 0;
    /* Prepare message for OAM-RRC Comm Info Req */
    apiId = RRC_OAM_COMMUNICATION_INFO_REQ;
    transId = RRC_OAM_COMMUNICATION_TRNS_ID;
    LTE_SET_U16BIT(&outMsg[current],transId);
    current += 6;
    LTE_SET_U16BIT(&outMsg[current],apiId);
    current += 4; 
    /* After header correction appending Comm API specific information */
    /*outMsg[current] = MAX_EXT_MODULES; 
    current += 1;*/

    /* Appending buffer for Packet Relay entity information */
    outMsg[current] = 0x00;
    current += 1;
    outMsg[current] = 0x01;
    current += 1;
    outMsg[current] = 0x04; 
    current += 1;
    outMsg[current] = UDP_TRANSPORT_MODE;
    current += 1;
    memset(tempHostName, 0, HOSTNAME_MAX_LEN+1);
//    strcpy(tempHostName, s1apModuleHostName);
    strcpy(tempHostName, packetRelayModuleHostName);
    token_p = strtok(tempHostName, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    //LTE_SET_U16BIT(&outMsg[current],s1apModulePort);
    LTE_SET_U16BIT(&outMsg[current], packetRelayS1APModulePort);
    current += 2;

    /* Appending buffer for RRM entity information */
    outMsg[current] = 0x00;
    current += 1;
    outMsg[current] = 0x01;
    current += 1;
    outMsg[current] = 0x05;
    current += 1;
    outMsg[current] = UDP_TRANSPORT_MODE;
    current += 1;
    memset(tempHostName, 0, HOSTNAME_MAX_LEN+1);
    strcpy(tempHostName, rrmModuleHostName);
    token_p = strtok(tempHostName, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    LTE_SET_U16BIT(&outMsg[current],rrmModulePort);
    current += 2;
    
    /* Appending buffer for PHY entity information */
    outMsg[current] = 0x00;
    current += 1;
    outMsg[current] = 0x01;
    current += 1;
    outMsg[current] = 0x00;
    current += 1;
    outMsg[current] = UDP_TRANSPORT_MODE;
    current += 1;
    memset(tempHostName, 0, HOSTNAME_MAX_LEN+1);
    strcpy(tempHostName, phyModuleHostName);
    token_p = strtok(tempHostName, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    LTE_SET_U16BIT(&outMsg[current],phyModulePort);
    current += 2;

    /* Appending buffer for RLC Entity information */
    outMsg[current] = 0x00;
    current += 1;
    outMsg[current] = 0x01;
    current += 1;
    outMsg[current] = 0x02;
    current += 1;
    outMsg[current] = UDP_TRANSPORT_MODE;
    current += 1;
    memset(tempHostName, 0, HOSTNAME_MAX_LEN+1);
    strcpy(tempHostName, rlcModuleHostName);
    token_p = strtok(tempHostName, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    LTE_SET_U16BIT(&outMsg[current],rlcModulePort);
    current += 2;

    /* Appending buffer for MAC Entity information */
    outMsg[current] = 0x00;
    current += 1;
    outMsg[current] = 0x01;
    current += 1;
    outMsg[current] = 0x01;
    current += 1;
    outMsg[current] = UDP_TRANSPORT_MODE;
    current += 1;
    memset(tempHostName, 0, HOSTNAME_MAX_LEN+1);
    strcpy(tempHostName, macModuleHostName);
    token_p = strtok(tempHostName, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    LTE_SET_U16BIT(&outMsg[current],macModulePort);
    current += 2;

    /* Appending buffer for PDCP entity information */
    outMsg[current] = 0x00;
    current += 1;
    outMsg[current] = 0x01;
    current += 1;
    outMsg[current] = 0x03;
    current += 1;
    outMsg[current] = UDP_TRANSPORT_MODE;
    current += 1;
    memset(tempHostName, 0, HOSTNAME_MAX_LEN+1);
    strcpy(tempHostName, pdcpModuleHostName);
    token_p = strtok(tempHostName, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    token_p = strtok(NULL, ".");
    outMsg[current] = atoi(token_p);
    current += 1;
    LTE_SET_U16BIT(&outMsg[current],pdcpModulePort);
    current += 2;					/* total buffer 71 */
    /* Filling buffer length inforamtion in sending Comm API buffer */
    LTE_SET_U16BIT(&outMsg[8],current);
    //fprintf(stderr," Send buffer to RRC is %d bytes", current);
    WriteToSocket(outMsg, current, RRC_MODULE_ID);
    fprintf(stderr,"\n\nSent RRC_OAM_COMMUNICATION_INFO_REQ\n\n");
     
    /* RRC-OAM Provision Req message Sending */
    /* CFG Script file name for RRC cfg file for Porvision Req API */
    UChar8 * filename_p="scripts/oam/rrc/oam_rrc_prov_info.cfg";
    loadToSockCmd(filename_p);
    fprintf(stderr,"\n\nSent RRC_OAM_PROVISION_REQ\n\n");
}


void rrcOamComInfoRespHandling(UInt8 *recvMsg_p, UInt32 msgLength)
{
    UInt8  tlvCheckMsg_p[SOCKET_BUFFER_SIZE] = {0};
    UInt16 tlvCheckSize = 0;
    /* CFG Script file name for RRC cfg file for Porvision Req API */
    UChar8 * check_fname="scripts/oam/rrc/oam_rrc_comm_info.res";


    /* read TLV response message for checking */
    if ( PNULL == prepareTlv(check_fname, tlvCheckMsg_p, &tlvCheckSize) )
    {
        PDCP_LOG( LOG_WARNING, "Error reading .res from '%s'", check_fname );
        return;
    }

    /* check received message */
    if (checkResponseMsg( recvMsg_p, msgLength, tlvCheckMsg_p, tlvCheckSize ))
    {
        PDCP_LOG(LOG_INFO, "************** CHECK RESPONSE [ PASSED ] ******");
        statisticUpdate( SIM_STAT_TESTS_SUCCEEDED, 1 );
    }
    else
    {
        PDCP_LOG(LOG_INFO, "************** CHECK RESPONSE [ FAILED ] ******");
        statisticUpdate( SIM_STAT_TESTS_FAILED, 1 );
    }
}

void s1apOamProvRespHandling(UInt8 *recvMsg_p, UInt32 msgLength)
{
    UInt8  tlvCheckMsg_p[SOCKET_BUFFER_SIZE] = {0};
    UInt16 tlvCheckSize = 0;
    UInt8 outMsg[20], current = 0;

    /* CFG Script file name for S1AP cfg file for Porvision Resp API */
    UChar8 * check_fname="scripts/oam/response/s1ap_oam_prov_resp.res";


    /* read TLV response message for checking */
    if ( PNULL == prepareTlv(check_fname, tlvCheckMsg_p, &tlvCheckSize) )
    {
        PDCP_LOG( LOG_WARNING, "Error reading .res from '%s'", check_fname );
        return;
    }

    /* check received message */
    if (checkResponseMsg( recvMsg_p, msgLength, tlvCheckMsg_p, tlvCheckSize ))
    {
        PDCP_LOG(LOG_INFO, "************** CHECK RESPONSE [ PASSED ] ******");
        statisticUpdate( SIM_STAT_TESTS_SUCCEEDED, 1 );
        l3InitFlag_g++;
        if(2 == l3InitFlag_g) {
            current  = sendIndToRrm(&outMsg[0]);
            WriteToSocket(&outMsg[0],current,RRM_MODULE_ID);
            fprintf(stderr,"\n\nSent Indication to RRM\n\n");
            l3InitFlag_g = 0;
        }    
    }
    else
    {
        PDCP_LOG(LOG_INFO, "************** CHECK RESPONSE [ FAILED ] ******");
        statisticUpdate( SIM_STAT_TESTS_FAILED, 1 );
    }

}


void rrcOamProvRespHandling(UInt8 *recvMsg_p, UInt32 msgLength)
{

    UInt8  tlvCheckMsg_p[SOCKET_BUFFER_SIZE] = {0};
    UInt16 tlvCheckSize = 0;
    UInt8 outMsg[20], current = 0;

    /* CFG Script file name for RRC cfg file for Porvision Req API */
    UChar8 * check_fname="scripts/oam/response/rrc_oam_prov_info.res";


    /* read TLV response message for checking */
    if ( PNULL == prepareTlv(check_fname, tlvCheckMsg_p, &tlvCheckSize) )
    {
        PDCP_LOG( LOG_WARNING, "Error reading .res from '%s'", check_fname );
        return;
    }

    /* check received message */
    if (checkResponseMsg( recvMsg_p, msgLength, tlvCheckMsg_p, tlvCheckSize ))
    {
        PDCP_LOG(LOG_INFO, "************** CHECK RESPONSE [ PASSED ] ******");
        statisticUpdate( SIM_STAT_TESTS_SUCCEEDED, 1 );
        l3InitFlag_g++;
        if(2 == l3InitFlag_g) {
            current  = sendIndToRrm(&outMsg[0]);
            WriteToSocket(&outMsg[0],current,RRM_MODULE_ID);
            fprintf(stderr,"\n\nSent Indication to RRM after RRC message\n\n");
            l3InitFlag_g = 0;
        }    
    }
    else
    {
        PDCP_LOG(LOG_INFO, "************** CHECK RESPONSE [ FAILED ] ******");
        statisticUpdate( SIM_STAT_TESTS_FAILED, 1 );
    }

}

/***** static functions *****/

/* returns UDP port number for given moduleId */
static UInt16 GetModulePortNum(Int moduleId)
{
    switch(moduleId)
    {
        case TRAFFIC_GEN_MODULE_ID:
            return trafficGenModulePort;
        case RRC_MODULE_ID:
            return rrcModulePort;
        case OAM_MODULE_ID:
            return oamModulePort;
        case PDCP_MODULE_ID:
            return pdcpModulePort;
        case RLC_MODULE_ID:
            return rlcModulePort;
        case MAC_MODULE_ID:
        case 8: /* SIM_CHANGES_GK */
            return macModulePort;
        case PDCP_MODULE_CMD_ID:
            return pdcpModuleCmdPort;
        case PACKET_RELAY_MODULE_ID:
            return packetRelayModulePort;
/*        case S1AP_MODULE_ID:
            return s1apModulePort;
*/            
        case RRM_MODULE_ID:
            return rrmModulePort;
        default:
            return 0;
    }
}

/* returns host name (string) for given moduleId */
static UChar8 *GetModuleHostName(Int moduleId)
{
    switch(moduleId)
    {
        case TRAFFIC_GEN_MODULE_ID:
            return trafficGenModuleHostName;
        case RRC_MODULE_ID:
            return rrcModuleHostName;
        case OAM_MODULE_ID:
            return oamModuleHostName;
        case PDCP_MODULE_ID:
            return pdcpModuleHostName;
        case PDCP_MODULE_CMD_ID:
            return pdcpModuleCmdHostName;
        case RLC_MODULE_ID:
            return rlcModuleHostName;
        case MAC_MODULE_ID:
        case 8: /* SIM_CHANGES_GK */
            return macModuleHostName;
        case PACKET_RELAY_MODULE_ID:
            return packetRelayModuleHostName;
/*            
        case S1AP_MODULE_ID:
            return s1apModuleHostName;
*/            
        case RRM_MODULE_ID:
            return rrmModuleHostName;
        default:
            return PNULL;
    }
}

/* returns RX socket file descriptor for given moduleId */
static SocketType GetRxSocketFD(Int moduleId)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            return rrcSocketRx;
        case OAM_MODULE_ID:
            return oamSocketRx;
        case PDCP_MODULE_ID:
            return pdcpSocketRx;
        case PDCP_MODULE_CMD_ID:
            return pdcpSocketRxCmd;
        case RLC_MODULE_ID:
            return rlcSocketRx;
        case MAC_MODULE_ID:
        case 8: /* SIM_CHANGES_GK */
            return macSocketRx;
        case PACKET_RELAY_MODULE_ID:
            return packetRelaySocketRx;
        case RRM_MODULE_ID:
            return rrmSocketRx;
        default:
            return SOCKET_ERROR;
    }
}

/* Stores RX socket file descriptor for given moduleId */
static void SetRxSocketFD(Int moduleId,SocketType fd)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            rrcSocketRx=fd;
            break;
        case OAM_MODULE_ID:
            oamSocketRx=fd;
            break;
        case PDCP_MODULE_ID:
            pdcpSocketRx=fd;
            break;
        case PDCP_MODULE_CMD_ID:
            pdcpSocketRxCmd=fd;
            break;
        case PACKET_RELAY_MODULE_ID:
            packetRelaySocketRx=fd;
            break;
        case RRM_MODULE_ID:
            packetRelaySocketRx=fd;
            break;
        default:
            PDCP_LOG(LOG_CRITICAL,"SetRxSocketFD: Wrong moduleId");
    }
}

/* returns TX socket file descriptor for given moduleId */
static SocketType GetTxSocketFD(Int moduleId)
{
    switch(moduleId)
    {
        case TRAFFIC_GEN_MODULE_ID:
            return trafficGenSocketTx;
        case PDCP_MODULE_ID:
            return pdcpSocketTx;
        case PDCP_MODULE_CMD_ID:
            return pdcpSocketTxCmd;
        case RLC_MODULE_ID:
            return rlcSocketTx;
        case MAC_MODULE_ID:
        case 8: /* SIM_CHANGES_GK */
            return macSocketTx;
        case RRC_MODULE_ID:
            return rrcSocketTx;
        case PACKET_RELAY_MODULE_ID:
            return packetRelaySocketTx;
        /*        
        case S1AP_MODULE_ID:
            return s1apSocketTx;
        */        
        case RRM_MODULE_ID:
            return rrmSocketTx;
        default:
            return SOCKET_ERROR;
    }
}

/* Stores TX socket file descriptor for given moduleId */
static void SetTxSocketFD(Int moduleId,SocketType fd)
{
    switch(moduleId)
    {
        case TRAFFIC_GEN_MODULE_ID:
            trafficGenSocketTx = fd;     
            break;
        case PDCP_MODULE_ID:
            pdcpSocketTx=fd;
            break;
        case PDCP_MODULE_CMD_ID:
            pdcpSocketTxCmd = fd;
            break;
        case RLC_MODULE_ID:
            rlcSocketTx=fd;
            break;
        case MAC_MODULE_ID:
        case 8: /* SIM_CHANGES_GK */
            macSocketTx=fd;
            break;
        case RRC_MODULE_ID:
            rrcSocketTx=fd;
            break;
        case PACKET_RELAY_MODULE_ID:
            packetRelaySocketTx=fd;
            break;
/*            
        case S1AP_MODULE_ID:
            s1apSocketTx=fd;
            break;
*/            
        case RRM_MODULE_ID:
            rrmSocketTx=fd;
            break;
        default:
            PDCP_LOG(LOG_CRITICAL,"SetTxSocketFD: Wrong moduleId");    
    }
}

/* returns pointer to RX socket address structure (sockaddr_in) for given moduleId */
static struct sockaddr_in *GetRxSocketAddr(Int moduleId)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            return &rrcAddrRx;
        case OAM_MODULE_ID:
            return &oamAddrRx;
        case PDCP_MODULE_ID:
            return &pdcpAddrRx;
        case PDCP_MODULE_CMD_ID:
            return &pdcpAddrRxCmd;
        case RLC_MODULE_ID:
            return &rlcAddrRx;
        case MAC_MODULE_ID:
        case 8: /* SIM_CHANGES_GK */
            return &macAddrRx;
        case PACKET_RELAY_MODULE_ID:
            return &packetRelayAddrRx;
        case RRM_MODULE_ID:
            return &rrmAddrRx;
        default:
            return PNULL;
    }
}

/* returns pointer to TX socket address structure (sockaddr_in) for given moduleId */
static struct sockaddr_in *GetTxSocketAddr(Int moduleId)
{
    switch(moduleId)
    {
        case TRAFFIC_GEN_MODULE_ID:
            return &trafficGenAddrTx;
        case PDCP_MODULE_ID:
            fprintf(stderr,"sending packet to PDCP\n");
            return &pdcpAddrTx;
        case PDCP_MODULE_CMD_ID:
            return &pdcpAddrTxCmd;
        case RLC_MODULE_ID:
            fprintf(stderr,"sending packet to RLC\n");
            return &rlcAddrTx;
        case MAC_MODULE_ID:
        case 8:    
            fprintf(stderr,"sending packet to MAC\n");
            return &macAddrTx;
        case RRC_MODULE_ID:
            return &rrcAddrTx;
        case PACKET_RELAY_MODULE_ID:
            return &packetRelayAddrTx;
            /*        
                      case S1AP_MODULE_ID:
                      return &s1apAddrTx;
             */        
        case RRM_MODULE_ID:
            return &rrmAddrTx;
        default:
            return PNULL;
    }
}

/* returns blocking mode for given moduleId */
static UInt8 GetModuleBlockingMode(Int moduleId)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            return rrcSocketRxBlock;
        case OAM_MODULE_ID:
            return oamSocketRxBlock;
        case PDCP_MODULE_ID:
            return pdcpSocketRxBlock;
        case PDCP_MODULE_CMD_ID:
            return pdcpSocketRxCmdBlock;
        case RLC_MODULE_ID:
            return rlcSocketRxBlock;
        case MAC_MODULE_ID:
            return macSocketRxBlock;
        case PACKET_RELAY_MODULE_ID:
            return packetRelaySocketRxBlock;
        case RRM_MODULE_ID:
            return rrmSocketRxBlock;
        default:
            return 0;
    }
}

/* returns blocking mode for given moduleId */
static void SetModuleBlockingMode(Int moduleId,UInt8 isBlocking)
{
    switch(moduleId)
    {
        case RRC_MODULE_ID:
            rrcSocketRxBlock=isBlocking;
            break;
        case OAM_MODULE_ID:
            oamSocketRxBlock=isBlocking;
            break;
        case PDCP_MODULE_ID:
            pdcpSocketRxBlock=isBlocking;
            break;
        case PDCP_MODULE_CMD_ID:
            pdcpSocketRxCmdBlock=isBlocking;
            break;
        case RLC_MODULE_ID:
            rlcSocketRxBlock=isBlocking;
            break;
        case MAC_MODULE_ID:
            macSocketRxBlock=isBlocking;
            break;
        case PACKET_RELAY_MODULE_ID:
            packetRelaySocketRxBlock=isBlocking;
            break;
        case RRM_MODULE_ID:
            rrmSocketRxBlock=isBlocking;
            break;
        default:
            PDCP_LOG(LOG_CRITICAL,"%s: Wrong moduleId",__FUNCTION__);  
    }
}

/* close socket file descriptors (RX/TX) for given moduleId */
static void CloseSocket(Int moduleId)
{
	SocketType s=SOCKET_ERROR;
	s=GetTxSocketFD(moduleId);
	if(s!=SOCKET_ERROR)
	{
		SetTxSocketFD(moduleId,SOCKET_ERROR);
		close(s);
	}
	
	s=GetRxSocketFD(moduleId);
	if(s!=SOCKET_ERROR)
	{
		SetRxSocketFD(moduleId,SOCKET_ERROR);
		close(s);
	}
}

/****************************************************************************
 * Function Name  : InitSocketAddress
 * Inputs         : Host name, sockaddr_in, Port Number, isBlocking
 * Outputs        : 
 * Returns        : Socket file descriptor or SOCKET_ERROR
 * Variables      : 
 * Description    : to intialize UDP socket with address/port
 ****************************************************************************/
static SocketType InitSocketAddress(void *host_name,struct sockaddr_in *s,UInt16 port,UInt8 isBlocking)
{
    SocketType sockfd=SOCKET_ERROR;
    struct hostent *host_p=(struct hostent *)gethostbyname(host_name);
    if(SOCKET_ERROR == (sockfd = socket(PF_INET, SOCK_DGRAM, 0)))
    {
		PDCP_LOG(LOG_CRITICAL,"InitSocketAddress: creating socket error");
        return SOCKET_ERROR;
    }
    (*s).sin_family = PF_INET;
    (*s).sin_port = htons(port);
    (*s).sin_addr = *((struct in_addr *)host_p->h_addr);
	memSet(&((*s).sin_zero),0,8);
    return sockfd;
}

/****************************************************************************
 * Function Name  : InitTxSocket
 * Inputs         : moduleId - module Id
 * Outputs        : 
 * Returns        : Returns socket file descriptor or SOCKET_ERROR
 * Variables      : 
 * Description    : init TX socket for given moduleId.
 ****************************************************************************/
SocketType InitTxSocket(Int moduleId)
{
	SocketType fd=SOCKET_ERROR;
	PDCP_LOG(LOG_DETAIL,"Initializing TX socket. Host='%s',Port=%i",GetModuleHostName(moduleId),GetModulePortNum(moduleId));   
	
    fd=InitSocketAddress(GetModuleHostName(moduleId),GetTxSocketAddr(moduleId),GetModulePortNum(moduleId),LTE_FALSE);
    SetTxSocketFD(moduleId,fd);
    
    return fd;
}

/****************************************************************************
 * Function Name  : InitRxSocket
 * Inputs         : moduleId - module Id, isBlocking
 * Outputs        : 
 * Returns        : Returns socket file descriptor or SOCKET_ERROR
 * Variables      : 
 * Description    : init RX socket for given moduleId.
 ****************************************************************************/
SocketType InitRxSocket(Int moduleId, UInt8 isBlocking)
{
    SocketType fd=SOCKET_ERROR;
    struct sockaddr_in *pSockAddr=GetRxSocketAddr(moduleId);
    PDCP_LOG(LOG_DETAIL,"Initializing RX socket. Host='%s',Port=%i\n",GetModuleHostName(moduleId),GetModulePortNum(moduleId));   
	SetModuleBlockingMode(moduleId,isBlocking);
     fd=InitSocketAddress(GetModuleHostName(moduleId),pSockAddr,GetModulePortNum(moduleId),isBlocking);
    fprintf(stderr,"Initializing RX socket. for PDCP Host='%s',Port=%i fd = %d\n",GetModuleHostName(moduleId),GetModulePortNum(moduleId),fd);   
	if(SOCKET_ERROR==fd)
	{
		return SOCKET_ERROR;
	}
	int yes = 1;
	if (setsockopt(fd,SOL_SOCKET,SO_REUSEADDR,&yes,sizeof(yes)) < 0) {
       perror("Reusing ADDR failed");
       exit(1);
       }


	/* Bind the port to specific socket for listening */
    if(SOCKET_ERROR==bind(fd,(struct sockaddr*)pSockAddr,sizeof(struct sockaddr_in)))
    {
        PDCP_LOG(LOG_CRITICAL,"Bind Failed for Rx socket");
        return SOCKET_ERROR;
    }
	SetRxSocketFD(moduleId,fd);    
    return fd;
}
