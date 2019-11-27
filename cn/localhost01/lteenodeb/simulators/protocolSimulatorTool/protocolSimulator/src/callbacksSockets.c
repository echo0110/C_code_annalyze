/************************************************************************************
*
*   FILE NAME  : callbacksSockets.c
*
*   DESCRIPTION: External scheduler callbacks implementation for sockets related  commands.
*                "loadtosock", "sendandcheck", "sendcmd" - command callbacks definitions

*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/

#include <assert.h>
#include <stdlib.h>
#include <string.h>

#include "simLogger.h"

#include "callbacksSockets.h"
#include "userUtils.h"
#include "sock_tools.h"
#include "simuScheduler.h"
#include "testStatistic.h"

/****************************************************************************
 * Global Variables
 ****************************************************************************/

Int isReadSocketTaskEnabled=SIM_TRUE;

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static void loadToSockCmd(const UChar8* cfg_fname, UInt32 is_true);

static void sendAndCheckFileCmd(
	const UChar8* cfg_fname,
	SInt32 timeout,
	const UChar8* check_fname,
	UInt16 timeoutAllowed,
	sim_socket_type_et sock_type
);

static void listenSockCmd(Int listen_enabled);
static UInt16 checkResponseMsg(const UInt8* buf_p, UInt32 length, const UInt8* checkMsg_p, UInt16 checkSize, const UInt8* checkMask_p);

static UInt16 memCmpWithMask(
	const UInt8* checkMsg_p,
	const UInt8* buf_p,
	UInt16 checkSize,
	const UInt8* checkMask_p
);

static void emptySocksCmd(
    sim_socket_type_et sock_type
);

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
    if ( (argc < 1) || (argc > 2) )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'loadtosock <filename.cfg>[,nolisten]'");
        return SIM_FALSE;
    }
    assert(argv);

    if(2==argc)
    {
    	userTolower((UChar8*)argv[1]);
    	if(0 == strcmp("nolisten",argv[1]))
    	{
    		SIM_LOG( SIM_LOG_INFO, "COMMAND[ loadtosock ] %s, nolisten", argv[0] );
    	    listenSockCmd(SIM_FALSE);
    	}
    	else
    	{
    		SIM_LOG( SIM_LOG_WARNING, "Wrong second parameter in loadtosock, should be 'nolisten'" );
    		return SIM_FALSE;
    	}
    }
    else
    {
    	SIM_LOG( SIM_LOG_INFO, "COMMAND[ loadtosock ] %s", argv[0] );
    	listenSockCmd(SIM_TRUE);
    }
    loadToSockCmd(argv[0], SIM_SOCKET_TYPE_UDP);

    statisticUpdate( SIM_STAT_CMD_LOADTOSOCK_AMOUNT, 1 );
    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userToSockSctpCallback
 * DESCRIPTION      :   "loadtosock_sctp" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userToSockSctpCallback(UInt32 argc, const UChar8** argv)
{
    if ( argc != 1 )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'loadtosock_sctp <filename.cfg>'");
        return SIM_FALSE;
    }
    assert(argv);

   	SIM_LOG( SIM_LOG_INFO, "COMMAND[ loadtosock_SCTP ] %s", argv[0] );
    loadToSockCmd(argv[0], SIM_SOCKET_TYPE_SCTP);

    statisticUpdate( SIM_STAT_CMD_LOADTOSOCK_AMOUNT, 1 );
    return SIM_FALSE;
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
    if (( argc < 3 ) || ( argc > 4 ))
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'sendandcheck { <sendfile.cfg> | - },<time to wait(ms)>,<checkfile.res>[,<timeoutAllowed(1,0)>]'");
        return SIM_FALSE;
    }
    assert(argv);

    if ( 3 == argc )
    {
        SIM_LOG( SIM_LOG_INFO, "COMMAND[ sendandcheck ] %s,%s,%s", argv[0], argv[1], argv[2] );
        sendAndCheckFileCmd( argv[0], atoi(argv[1]), argv[2], SIM_FALSE, SIM_SOCKET_TYPE_UDP );
    }
    else if(4 == argc)
    {
        SIM_LOG( SIM_LOG_INFO, "COMMAND[ sendandcheck ] %s,%s,%s,%s", argv[0], argv[1], argv[2], argv[3] );
        sendAndCheckFileCmd( argv[0], atoi(argv[1]), argv[2], atoi(argv[3]),SIM_SOCKET_TYPE_UDP );
    }

    statisticUpdate( SIM_STAT_CMD_SENDANDCHECK_AMOUNT, 1 );
    statisticUpdate( SIM_STAT_TESTS_AMOUNT, 1 );
    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userSendAndCheckFileSctpCallback
 * DESCRIPTION      :   "sendandcheck_sctp" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userSendAndCheckFileSctpCallback(UInt32 argc, const UChar8** argv)
{
    if (( argc < 3 ) || ( argc > 4 ))
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'sendandcheck_sctp { <sendfile.cfg> | - },<time to wait(ms)>,<checkfile.res>[,<timeoutAllowed(1,0)>]'");
        return SIM_FALSE;
    }
    assert(argv);

    if ( 3 == argc )
    {
    	SIM_LOG( SIM_LOG_INFO, "COMMAND[ sendandcheck_SCTP ] %s,%s,%s", argv[0], argv[1], argv[2] );
    	sendAndCheckFileCmd( argv[0], atoi(argv[1]), argv[2], SIM_FALSE, SIM_SOCKET_TYPE_SCTP );
    }
    else if(4 == argc)
    {
        SIM_LOG( SIM_LOG_INFO, "COMMAND[ sendandcheck_SCTP ] %s,%s,%s,%s", argv[0], argv[1], argv[2], argv[3] );
        sendAndCheckFileCmd( argv[0], atoi(argv[1]), argv[2], atoi(argv[3]),SIM_SOCKET_TYPE_SCTP );
    }

    statisticUpdate( SIM_STAT_CMD_SENDANDCHECK_AMOUNT, 1 );
    statisticUpdate( SIM_STAT_TESTS_AMOUNT, 1 );
    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userListenSockCallback
 * DESCRIPTION      :   "listensock" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userListenSockCallback(UInt32 argc, const UChar8** argv)
{
    if (argc > 1)
    {
        SIM_LOG(SIM_LOG_WARNING, "Wrong parameters. Should be 'listensock [on|off]'. Default is ON");
        return SIM_FALSE;
    }

    if ( 0 == argc )
    {
        SIM_LOG(SIM_LOG_INFO, "COMMAND[ listensock ]");
        listenSockCmd(SIM_TRUE);
    }
    else
    {
        assert(argv);

        SIM_LOG(SIM_LOG_INFO, "COMMAND[ listensock ] %s", argv[0]);
		userTolower((UChar8*)argv[0]);
        if(0!=strcmp("off",argv[0]))
        {
            listenSockCmd(SIM_TRUE);
        }
        else
        {
            listenSockCmd(SIM_FALSE);
        }
    }

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   loadToSockCmd
 * DESCRIPTION      :   "loadtosock/loadtosock_sctp" callback implementation
 * PARAMATERS       :   [IN] const UChar8* cfg_fname - name of configuration file for
 *                      this callback
 *                      sim_socket_type_et sock_type
 * RETURN VALUE     :   None
 ************************************************************************************/
static void loadToSockCmd(const UChar8* cfg_fname, sim_socket_type_et sock_type)
{
    UInt8        tlvMsg_p[SOCKET_BUFFER_SIZE];
	UInt8*       send_tlv_p=PNULL;
	UInt16 tlvSize = 0;

    send_tlv_p = (UInt8*)prepareTlv(cfg_fname, tlvMsg_p, &tlvSize, PNULL);
	if(PNULL == send_tlv_p)
	{
		SIM_LOG( SIM_LOG_WARNING, "Error reading .cfg from '%s'", cfg_fname);
		return;
	}

    SIM_LOG(SIM_LOG_DUMP,"SEND\t: fileLength[%u]", tlvSize);
	userPrintRawBuffer("SEND\t", send_tlv_p, tlvSize, 0);

    if ( WriteToSocket(sock_type, (UInt8*)send_tlv_p, tlvSize) != tlvSize )
	{
		SIM_LOG( SIM_LOG_WARNING, "%s:Problem sending to socket", __FUNCTION__ );
	}
}

/************************************************************************************
 * FUNCTION NAME    :   sendAndCheckFileCmd
 * DESCRIPTION      :   "sendandcheck" callback implementation
 * PARAMATERS       :   [IN] cfg_fname - .cfg filename to send
 *                      [IN] timeout - time (ms) to wait for response
 *                      [IN] check_fname - .res filename to compare response with
 *                      [IN] timeoutAllowed - if 1 - Timeout should be interpreted as SUCCESS
 * RETURN VALUE     :   None
 ************************************************************************************/
static void sendAndCheckFileCmd(
	const UChar8* cfg_fname,
	SInt32 timeout,
	const UChar8* check_fname,
	UInt16 timeoutAllowed,
	sim_socket_type_et sock_type
)
{
    UInt8           tlvCheckMsg_p[SOCKET_BUFFER_SIZE];
    UInt16          tlvCheckSize = 0;
    UInt8           tlvCheckMask_p[SOCKET_BUFFER_SIZE];

    UInt8           buf[SOCKET_BUFFER_SIZE];
    UInt32          length = 0;
    UInt16          timeOutFlag = SIM_TRUE;
    UInt16          testResult = SIM_TRUE;

    timeout*=TIMEOUT_SIMU;

    esLock();

    if(SIM_SOCKET_TYPE_UDP==sock_type)
    {
    	listenSockCmd(SIM_FALSE);
    }

    /* read TLV response message for checking */
    if ( PNULL == prepareTlv(check_fname, tlvCheckMsg_p, &tlvCheckSize, tlvCheckMask_p) )
    {
        SIM_LOG( SIM_LOG_WARNING, "Error reading .res from '%s'", check_fname );
        esUnlock();
        return;
    }

    if(0!=strcmp("-",cfg_fname))
    /*Have some msg to send before checking*/
    {
        /*clear previous packets on sockets*/
		while( 0 != ReadFromSocket( sock_type, buf ,0) );

        /* read and send TLV message*/
		loadToSockCmd(cfg_fname, sock_type);
    }

    /*check if we have msg on socket port*/
    length = ReadFromSocket( sock_type, buf ,timeout);

    /* check received message */
    if ( length > 0  )
    {
        testResult = checkResponseMsg( buf, length, tlvCheckMsg_p, tlvCheckSize, tlvCheckMask_p );
        if ( SIM_TRUE ==  testResult )
        {
            SIM_LOG(SIM_LOG_INFO, "*********************** CHECK \t\t[ PASSED ] ******");
            statisticUpdate( SIM_STAT_TESTS_SUCCEEDED, 1 );
        }
        else
        {
            SIM_LOG(SIM_LOG_INFO, "*********************** CHECK \t\t[ FAILED ] ******");
            statisticUpdate( SIM_STAT_TESTS_FAILED, 1 );
        }

        timeOutFlag = SIM_FALSE;
    }

    if ( SIM_TRUE == timeOutFlag )
    {
	    if ( SIM_FALSE == timeoutAllowed )
        {
            SIM_LOG(SIM_LOG_INFO, "*********************** CHECK \t\t[ TIMEOUT ] ******");
            statisticUpdate( SIM_STAT_TESTS_TIMEOUT, 1 );
            testResult = SIM_FALSE;
        }
        else
        {
            SIM_LOG(SIM_LOG_INFO, "*********************** CHECK \t\t[ PASSED WITH TIMEOUT ] ******");
            statisticUpdate( SIM_STAT_TESTS_SUCCEEDED, 1 );
        }
    }

    statisticUpdateTestResult( testResult );

    esUnlock();
}

/************************************************************************************
 * FUNCTION NAME    :   listenSockCmd
 * DESCRIPTION      :   "listensock" action
 * PARAMATERS       :   [IN] Int listen_enabled - SIM_TRUE/SIM_FALSE
 * RETURN VALUE     :   None
 ************************************************************************************/
static void listenSockCmd(Int listen_enabled)
{
    if(isReadSocketTaskEnabled!=listen_enabled)
    {
        isReadSocketTaskEnabled=listen_enabled;
        if(SIM_TRUE==listen_enabled)
        {
            SIM_LOG(SIM_LOG_INFO, "Listening on socket ENABLED");
        }
        else
        {
            SIM_LOG(SIM_LOG_INFO, "Listening on socket DISABLED");
        }
    }
}

/************************************************************************************
 * FUNCTION NAME    :   memCmpWithMask
 * DESCRIPTION      :   It compares memory areas (with mask).
 * PARAMATERS       :   const UChar8* checkMsg_p,
 * 						const UInt8* buf_p,
 *  					UInt16 checkSize,
 *  					const UChar8* checkMask_p
 * RETURN VALUE     :   0 = equal, else <> 0
 ************************************************************************************/
static UInt16 memCmpWithMask(
	const UInt8* checkMsg_p,
	const UInt8* buf_p,
	UInt16 checkSize,
	const UInt8* checkMask_p
)
{
	UInt16 k=0;

	if(PNULL==checkMask_p)
	{
		return memcmp(checkMsg_p,buf_p,checkSize);
	}
	else
	{
		for (k=0;k<checkSize;k++)
		{
			if( (checkMsg_p[k]&checkMask_p[k]) != (buf_p[k]&checkMask_p[k]) )
			{
				/* not equal*/
				return 1;
			}
		}
		/*equal*/
		return 0;
	}
}

/************************************************************************************
 * FUNCTION NAME    :   checkResponseMsg
 * DESCRIPTION      :   It compares received TLV message with predefined etalon response message.
 * PARAMATERS       :   const UInt8* buf_p,
 *  					UInt32 length,
 *  					const UChar8* checkMsg_p,
 *  					UInt16 checkSize,
 *  					const UChar8* checkMask_p,
 * RETURN VALUE     :   SIM_FALSE/SIM_TRUE
 ************************************************************************************/
static UInt16 checkResponseMsg(
	const UInt8* buf_p,
	UInt32 length,
	const UInt8* checkMsg_p,
	UInt16 checkSize,
	const UInt8* checkMask_p
)
{
    UInt16 result = SIM_FALSE;

    SIM_LOG(SIM_LOG_INFO, "RECV: udpLen[%i]", length);
    userPrintRawBuffer( "RECV", buf_p, length, 0 );

    if ( length != checkSize )
    {
        /* incorrect packet length */
        SIM_LOG(SIM_LOG_INFO, "CHECK: Packet length isn't equal result length. length[%i] checkSize[%i]", length, checkSize);
    }
    else
    {
        if( 0 == memCmpWithMask(checkMsg_p, buf_p, checkSize, checkMask_p) )
        {
            result = SIM_TRUE;
        }
        else
        {
            userPrintRawBufferMasked("CHECK: Should be", checkMsg_p, checkSize, 0, checkMask_p);
        }
    }

    return result;
}

/************************************************************************************
 * FUNCTION NAME    :   userListenAndAccept
 * DESCRIPTION      :   "listenandaccept" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userListenAndAccept(UInt32 argc, const UChar8** argv)
{
    if ( argc != 1 )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'listenandaccept sctp'");
        return SIM_FALSE;
    }
    assert(argv);

    SIM_LOG( SIM_LOG_INFO, "COMMAND[ listenandaccept ] %s", argv[0] );

    userTolower((UChar8*)argv[0]);
    if(0!=strcmp("sctp",argv[0]))
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameter in listenandaccept, should be 'sctp'" );
        return SIM_FALSE;
    }

    if ( SOCKET_ERROR == WaitForConnectionOnSocket(SIM_SOCKET_TYPE_SCTP) )
    {
        SIM_LOG( SIM_LOG_WARNING, "%s:Problem accepting connection", __FUNCTION__ );
    }

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   userWaitResetSctp
 * DESCRIPTION      :   "waitReset_sctp" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userWaitResetSctp(UInt32 argc, const UChar8** argv)
{
    if ( argc != 0 )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'waitreset_sctp'");
        return SIM_FALSE;
    }

    SIM_LOG( SIM_LOG_INFO, "COMMAND[ waitreset_SCTP ]" );

    sctpWaitReset();

    SIM_LOG( SIM_LOG_INFO, "SCTP reset processed OK" );

    return SIM_FALSE;
}

/************************************************************************************
 * FUNCTION NAME    :   emptySocksCmd
 * DESCRIPTION      :   "emptySocks" callback implementation
 * PARAMATERS       :   [IN] sim_socket_type_et sock_type
 * RETURN VALUE     :   None
 ************************************************************************************/
static void emptySocksCmd(
    sim_socket_type_et sock_type
)
{
    UInt8           buf[SOCKET_BUFFER_SIZE];

    esLock();

    /*clear previous packets on sockets*/
    while( 0 != ReadFromSocket( sock_type, buf ,0) );

    esUnlock();
}

/************************************************************************************
 * FUNCTION NAME    :   userEmptySocksCallback
 * DESCRIPTION      :   "emptySocks" callback function
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userEmptySocksCallback(UInt32 argc, const UChar8** argv)
{
    if ( argc != 0 )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be 'emptySocks'");
        return SIM_FALSE;
    }

    SIM_LOG( SIM_LOG_INFO, "COMMAND[ emptySocks ]" );

    emptySocksCmd(SIM_SOCKET_TYPE_UDP);
    emptySocksCmd(SIM_SOCKET_TYPE_SCTP);

    return SIM_FALSE;
}
