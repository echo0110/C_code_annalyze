/************************************************************************************
*
*   FILE NAME  : callbacksSockets.c 
*
*   DESCRIPTION: External scheduler callbacks implementation for sockets related  commands.
*                "loadtosock", "sendandcheck", "sendcmd" - command callbacks definitions

*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/

#include <assert.h>

#include "lteSimPdcpEsLteFramework.h"

#include <ltePdcpLteDefs.h>

#include "lteSimPdcpCallBacksSockets.h"
#include "lteSimPdcpUserUtils.h"
#include "ltePdcpCmnTools.h"
#include "lteSimPdcpSockTools.h"
#include "lteSimPdcpSimuScheduler.h"
#include "lteSimPdcpTestStatistic.h"
#include "ltePdcpTlvUtils.h"
#include "lteLayer2CommanTypes.h"
/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

void loadToSockCmd(const UChar8* cfg_fname);
void sendAndCheckFileCmd(const UChar8* cfg_fname,SInt32 timeout,const UChar8* check_fname, UInt16 timeoutAllowed);
UInt16 checkResponseMsg(const UInt8* buf_p, UInt32 length, const UInt8* checkMsg_p, UInt16 checkSize );

/****************************************************************************
 *   Exported Variables
*****************************************************************************/
extern LTE_SEM es_sem_notify;
#ifndef MULTI_UE_ENHANCEMENT
 extern UInt16 ueIndex;
#else
 #define CRNTI_START        51
 extern SInt16 ueDetached;
 extern SInt16 ueIndex;
#endif
#ifdef MULTI_UE_ENHANCEMENT_FRWK
 extern UInt16 crntiInd_g;/*Storing MAC Indicated CRNTI*/
 extern UInt16 ueIndexRel_g;
#endif
/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/************************************************************************************
 * FUNCTION NAME    :   userToSockCallback
 * DESCRIPTION      :   "loadtosock" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userToSockCallback(UInt32 argc, const UChar8** argv)
{
    if ( 1 != argc && PNULL == argv)
    {
        PDCP_LOG( LOG_WARNING, "Wrong format for loadtosock :: Correct format is loadtosock <cfg file>'");
        return LTE_FALSE;
    }
    PDCP_LOG( LOG_INFO, "COMMAND[ loadtosock ] %s", argv[0] ); 
    loadToSockCmd(argv[0]);
    statisticUpdate( SIM_STAT_CMD_LOADTOSOCK_AMOUNT, 1 );
    return LTE_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userSendAndCheckFileCallback
 * DESCRIPTION      :   "sendandcheck" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSendAndCheckFileCallback(UInt32 argc, const UChar8** argv)
{
    if (( 3 > argc ) && ( 4 < argc ))
    {
       PDCP_LOG( LOG_WARNING, "Wrong Format for sendandcheck :: Correct Format is 'sendandcheck <cfg file>,<time (ms)>,< rsp file >[,<timeoutAllowed(1,0)>]'");
        return LTE_FALSE;
    }
    if ( 3 == argc )
    {
        PDCP_LOG( LOG_INFO, "COMMAND[ sendandcheck ] %s,%s,%s", argv[0], argv[1], argv[2] ); 
        sendAndCheckFileCmd( argv[0], atoi(argv[1]), argv[2], LTE_FALSE );
    }
    else
    {
        PDCP_LOG( LOG_INFO, "COMMAND[ sendandcheck ] %s,%s,%s", argv[0], argv[1], argv[2], argv[3] ); 
        sendAndCheckFileCmd( argv[0], atoi(argv[1]), argv[2], atoi(argv[3]) );
    }
    statisticUpdate( SIM_STAT_CMD_SENDANDCHECK_AMOUNT, 1 );
    statisticUpdate( SIM_STAT_TESTS_AMOUNT, 1 );
    return LTE_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   sendSrbCallback
 * DESCRIPTION      :   "sendsrbCallback" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   void
 ************************************************************************************/
UInt32 sendSrbCallback(UInt32 argc, const UChar8 **argv)
{
    if (1 != argc && PNULL == argv)
    {
        PDCP_LOG(LOG_DUMP, "Wrong Format for SendSrbData :: Correct Format is 'sendsrbdata <cfg file>'");
        return LTE_FALSE;
    }
    loadToSockCmd(argv[0]); 
    statisticUpdate( SIM_STAT_CMD_LOADTOSOCK_AMOUNT, 1 );
    return LTE_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   loadToSockCmd
 * DESCRIPTION      :   "loadtosock" callback implementation
 * PARAMATERS       :   [IN] const UChar8* cfg_fname - name of configuration file for
 *                      this callback
 * RETURN VALUE     :   None
 ************************************************************************************/
void loadToSockCmd(const UChar8* cfg_fname)
{
    UInt8 tlvMsg_p[SOCKET_BUFFER_SIZE] = {0};
    TlvHeader send_tlv = {0};
    UChar8 *buffer_p = PNULL;
    UInt16 tlvSize = 0 , counter = 0;
    assert(buffer_p = (UChar8 *)prepareTlv(cfg_fname, tlvMsg_p, &tlvSize));
    send_tlv.transactionId = LTE_GET_U16BIT(&buffer_p[counter]);
    send_tlv.sourceModuleId = LTE_GET_U16BIT(&buffer_p[counter + 2]);
    send_tlv.destinationModuleId = LTE_GET_U16BIT(&buffer_p[counter + 4]);
    send_tlv.apiId = LTE_GET_U16BIT(&buffer_p[counter + 6]);
    send_tlv.length = LTE_GET_U16BIT(&buffer_p[counter + 8]);
    userPrintRawBuffer("OAM-RRC SEND\t", (UInt8*)buffer_p, tlvSize, 0);
    PDCP_LOG(LOG_DUMP,"OAM-RRC SEND\t: tlvLength[%u] fileLength[%u]", send_tlv.length, tlvSize);    
    if ( WriteToSocket((UInt8*)&tlvMsg_p, tlvSize, send_tlv.destinationModuleId ) != tlvSize )
        PDCP_LOG( LOG_WARNING, "%s:Problem sending to PDCP socket", __FUNCTION__ );
}

/************************************************************************************
 * FUNCTION NAME    :   sendAndCheckFileCmd
 * DESCRIPTION      :   "sendandcheck" callback implementation
 * PARAMATERS       :   [IN] cfg_fname - .cfg filename to send
 *						[IN] timeout - time (ms) to wait for response
 *						[IN] check_fname - .res filename to compare response with
 *                      [IN] timeoutAllowed - if 1 - Timeout should be interpreted as SUCCESS
 * RETURN VALUE     :   None
 ************************************************************************************/
void sendAndCheckFileCmd(const UChar8* cfg_fname,SInt32 timeout,const UChar8* check_fname, UInt16 timeoutAllowed)
{
    UInt8  tlvCheckMsg_p[SOCKET_BUFFER_SIZE] = {0};
    UInt16 tlvCheckSize = 0;
    LTE_TIME_T t1,t2;
    UInt8 buf[SOCKET_BUFFER_SIZE]= {0};
    UInt32  length = 0;
    UInt16  timeOutFlag = LTE_TRUE;
    UInt16  testResult = LTE_TRUE;
    timeout*=TIMEOUT_SIMU;
    SInt32 semValue = 0;	
    semGetVal(&es_sem_notify,&semValue);
   if(semValue){
        semWait(&es_sem_notify);
    }

    /* read TLV response message for checking */
    if ( PNULL == prepareTlv(check_fname, tlvCheckMsg_p, &tlvCheckSize) )
    {
        PDCP_LOG( LOG_WARNING, "Error reading .res from '%s'", check_fname );
        return;
    }
    /*clear previous packets on sockets*/
    while( 0 != ReadFromSocket( buf, OAM_MODULE_ID ));
    if(0 != strcmp(".",cfg_fname))
         loadToSockCmd(cfg_fname);
    t1 = lteGetSystemTime();
    do
    {
        /*check if we have msg on OAM port*/
        length = ReadFromSocket( buf, OAM_MODULE_ID );
        /* check received message */
        if ( length > 0  )
        {
            if (checkResponseMsg( buf, length, tlvCheckMsg_p, tlvCheckSize ))
            {
                PDCP_LOG(LOG_INFO, "************** CHECK RESPONSE [ PASSED ] ******");
                statisticUpdate( SIM_STAT_TESTS_SUCCEEDED, 1 );
            }
            else
            {
                PDCP_LOG(LOG_INFO, "************** CHECK RESPONSE [ FAILED ] ******");
                statisticUpdate( SIM_STAT_TESTS_FAILED, 1 );
            }
            timeOutFlag = LTE_FALSE;
            break;
	}
        t2 = lteGetSystemTime();
    }while( lteTimeDiff( &t2, &t1 ) < timeout );

    if ( LTE_TRUE == timeOutFlag )
    {
        if ( LTE_FALSE == timeoutAllowed )
        {
            PDCP_LOG(LOG_INFO, "**************** CHECK RESPONSE [ TIMEOUT ] ******");
            statisticUpdate( SIM_STAT_TESTS_TIMEOUT, 1 );
            testResult = LTE_FALSE;
        }
        else
        {
            PDCP_LOG(LOG_INFO, "**************** CHECK RESPONSE [ PASSED BUT TIMEOUT ] ******");
            statisticUpdate( SIM_STAT_TESTS_SUCCEEDED, 1 );
        }
    }
    statisticUpdateTestResult( testResult );
    if(semValue){
	    semPost(&es_sem_notify);
    }
}

/************************************************************************************
 * FUNCTION NAME    :   checkResponseMsg
 * DESCRIPTION      :   It compares received TLV message with predefined etalon response message.
 * PARAMATERS       :   const UInt8* buf_p, UInt32 length, const UChar8* checkMsg_p, UInt16 checkSize
 * RETURN VALUE     :   LTE_FALSE/LTE_TRUE
 ************************************************************************************/
UInt16 checkResponseMsg(const UInt8* buf_p, UInt32 length, const UInt8* checkMsg_p, UInt16 checkSize )
{
    UInt16 result = LTE_FALSE;
    userPrintRawBuffer( "OAM-RRC RECV", buf_p, length, 0 );
    UInt16 srcModuleId = 0;
    srcModuleId = LTE_GET_U16BIT(&buf_p[2]);
    if ( ( length < sizeof(TlvHeader) ) ||
         ( length != LTE_GET_U16BIT(&buf_p[8] )))
    {
        /* incorrect packet length */
        PDCP_LOG(LOG_WARNING, "CHECK [Incorrect Pkt Len [%i] TLVlength[%i]]", length, LTE_GET_U16BIT(&buf_p[8]));
    }  
    else if ( (PDCP_MODULE_ID != srcModuleId) && (MAC_MODULE_ID != srcModuleId) && (RLC_MODULE_ID != srcModuleId ) && (TRAFFIC_GEN_MODULE_ID != srcModuleId) && RRC_MODULE_ID != srcModuleId && PACKET_RELAY_MODULE_ID != srcModuleId) 
    {
        /* incorrect module id - drop it */
	PDCP_LOG(LOG_WARNING, "CHECK [Source Module Id [%i] is not PDCP]", srcModuleId) ;
    }
    else if ( length != checkSize )
    {
        /* incorrect packet length */
        PDCP_LOG(LOG_INFO, "CHECK [Packet Len != Result Len].. length[%i] checkSize[%i]", length, checkSize);
    }
    else
    {
        PDCP_LOG(LOG_INFO, "OAM-RRC RECV: AppId[%i], udpLen[%i]", LTE_GET_U16BIT(&buf_p[6]), length);
	if(!memCmp( checkMsg_p, buf_p, checkSize ) )
	    result = LTE_TRUE;
        else
	    userPrintRawBuffer("CHECK [Should be ]", checkMsg_p, checkSize, 0);
    }
    return result;
}

/************************************************************************************
 * FUNCTION NAME    :   userSendCommand
 * DESCRIPTION      :   Send command line to PDCP host.
                        "sendcmd" callback implementation
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSendCommand(UInt32 argc, const UChar8** argv)
{
    UInt32  length = 0;
    if ( 1 != argc && PNULL != argv)
    {
        PDCP_LOG( LOG_WARNING, "Wrong parameters. Should be 'sendcmd <\"commandLine\">'");
        return LTE_FALSE;
    }
    if ((length = (UInt32)strlen( argv[0])))
    {
        PDCP_LOG( LOG_INFO, "COMMAND[ sendcmd ] %s", argv[0] ); 
        UInt16 moduleId = PDCP_MODULE_CMD_ID;
        if( length+1 != WriteToSocket( (UInt8*)argv[0], length+1,moduleId ) )
        {
           PDCP_LOG( LOG_WARNING, "%s:Error writting to socket",__FUNCTION__);
        }
    }
    statisticUpdate( SIM_STAT_CMD_SENDCMD_AMOUNT, 1 );
    return LTE_FALSE;
}
