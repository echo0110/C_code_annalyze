/************************************************************************************
*
*   FILE NAME  : testStatistic.c
*
*   DESCRIPTION: Statistic unit implementation.
*
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
#include "lteSimPdcpSimuTableFunc.h"
#include "lteSimPdcpUserUtils.h"
#include "lteSimPdcpTestStatistic.h"

/************************************************************************************
 * Global variable
 ************************************************************************************/

/****************************************************************************
 * Private definitions
 ****************************************************************************/
/* Maximum supported number of failed test suites or test cases */
#define STAT_FAILED_TESTS_MAX_NUMBER   16

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

static UInt32    statisticReset( UInt32 argc, const UChar8** argv );
static UInt32    statisticReport( UInt32 argc, const UChar8** argv );
static UInt32    statisticBeginTestSuite( UInt32 argc, const UChar8** argv );
static UInt32    statisticEndTestSuite( UInt32 argc, const UChar8** argv );
static UInt32    statisticBeginTestCase( UInt32 argc, const UChar8** argv );
static UInt32    statisticEndTestCase( UInt32 argc, const UChar8** argv );
static void    statisticReportFailedItems( const UChar8* caption_p, const UInt16* items_p, UInt16 count );
static void    statisticReportDataItems( const UInt16* items_p );
/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
/* Statistic counters. */
/* There are global counters and only "statisticcmd reset" command can be used for clearing them */
/* They will be printed out by "statisticcmd report" command */
static UInt16 statisticData[SIM_STAT_ITEM_LAST] = { 0 };

/* Test Suite Statistic counters */
/* There are local counters for Test Suite. They will be cleared every time when 
   "statisticcmd begintestsuite" command is called*/
/* They will be printed out by "statisticcmd report,suite" command */
static UInt16 statisticSuiteData[SIM_STAT_ITEM_LAST] = { 0 };

/* Statistic counters captions list for reporting */
static const UChar8* statisticCaption[SIM_STAT_ITEM_LAST] =
{
    "Number of Test suites                =>    ",
    "Number of Test cases                 =>    ",
    "Number of Tests                      =>    ",
    "Number of Test suites succeeded      =>    ",
    "Number of Test suites failed         =>    ",
    "Number of Test cases succeeded       =>    ",
    "Number of Test cases failed          =>    ",
    "Number of Tests succeeded            =>    ",
    "Number of Tests failed               =>    ",
    "Number of Tests timeouts             =>    ",
    "Number of runscript command calls    =>    ",
    "Number of sleep command calls        =>    ",
    "Number of echo command calls         =>    ",
    "Number of redirectlog command calls  =>    ",
    "Number of sendandcheck command calls =>    ",
    "Number of loadtosock command calls   =>    ",
    "Number of sendcmd command calls      =>    ",
    "Sleep time, msec                     =>    ",
};


/* Statistic commands handlers */
static const FuncId stat_table_func[] = 
{
    {"reset",            statisticReset                 },
    {"report",           statisticReport                },
    {"begintestsuite",   statisticBeginTestSuite        },
    {"endtestsuite",     statisticEndTestSuite          },
    {"begintestcase",    statisticBeginTestCase         },
    {"endtestcase",      statisticEndTestCase           },
};

/* Current Test suite state. LTE_FALSE - succeeded*/
static UInt16 statisticTestSuiteState = LTE_TRUE; 
/* Current Test suite number. */
static UInt16 statisticTestSuiteNumber = 0xFFFF;
/* Failed Test suite numbers array. */
static UInt16 statisticTestSuiteNumbers[STAT_FAILED_TESTS_MAX_NUMBER] = { 0 };

LTE_TIME_T testSuiteBeginTime;
LTE_TIME_T testSuiteEndTime;

/* Test case state. LTE_FALSE - succeeded*/
static UInt16 statisticTestCaseState = LTE_TRUE;;   
/* Current Test case number. */
static UInt16 statisticTestCaseNumber = 0xFFFF;
/* Failed Test case numbers array. */
static UInt16 statisticTestCaseNumbers[STAT_FAILED_TESTS_MAX_NUMBER] = { 0 };

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/************************************************************************************
 * FUNCTION NAME    :   userStatisticCommandCallback
 * DESCRIPTION      :   Runs statistic engine command  
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userStatisticCommandCallback(UInt32 argc, const UChar8** argv)
{
    if ( (0 == argc) || (PNULL == argv[0]) )
    {
        PDCP_LOG(LOG_WARNING,"Wrong Format :: Format should be statisticcmd");
        return LTE_FALSE;
    }
    PDCP_LOG( LOG_INFO, "COMMAND[ statisticcmd ] %s", argv[0] );
    statisticCommand( argc, argv );
    return LTE_FALSE;
}

/****************************************************************************
 * Function Name  : statisticReset 
 * Inputs         : argc - callback arguments counter
 *                  argv - callback arguments.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Resets statistic data.
 ****************************************************************************/
UInt32    statisticReset( UInt32 argc, const UChar8** argv )
{
    memSet( statisticData, 0, sizeof(statisticData) );
    statisticTestSuiteState = LTE_TRUE;
    memSet( statisticTestSuiteNumbers, 0, sizeof(statisticTestSuiteNumbers) );
    statisticTestSuiteNumber = 0xFFFF;
    memSet( statisticSuiteData, 0, sizeof(statisticSuiteData) );
    statisticTestCaseState  = LTE_TRUE;
    memSet( statisticTestCaseNumbers, 0, sizeof(statisticTestCaseNumbers) );
    statisticTestCaseNumber = 0xFFFF;
    return LTE_FALSE;
}

/****************************************************************************
 * Function Name  : statisticReport 
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Prints statistic data to log.
 ****************************************************************************/
UInt32    statisticReport( UInt32 argc, const UChar8** argv )
{
    PDCP_LOG( LOG_INFO, "\n\n\n" );
    PDCP_LOG( LOG_INFO, "******************** STATISTICS REPORT START *********************" );
    if ( (2 == argc) && (PNULL != argv[1]) && (0 == strcmp("suite", argv[1])) )
    {   /* Test suite report */
        PDCP_LOG( LOG_INFO, "TEST SUITE :: %u", statisticTestSuiteNumber);
        statisticReportDataItems( statisticSuiteData );
        statisticReportFailedItems( 
            "Failed Test cases :: ", 
            statisticTestCaseNumbers, 
            statisticGet(SIM_STAT_TEST_CASES_FAILED, LTE_TRUE) );
    }
    else 
    {   /* Global report */
        statisticReportDataItems( statisticData );
        statisticReportFailedItems( 
            "Failed Test suites :: ", 
            statisticTestSuiteNumbers, 
            statisticGet(SIM_STAT_TEST_SUITES_FAILED, LTE_FALSE) );
    }
    PDCP_LOG( LOG_INFO, "******************** STATISTICS REPORT END ***********************" );
    return LTE_FALSE;
}

/****************************************************************************
 * Function Name  : statisticUpdate 
 * Inputs         : statItemId - Statistic data item ID
                    statValue - Value to add
 * Outputs        : None.
 * Returns        : None.
 * Description    : Updates statistic data. It adds statValue to the Statistic data 
                    item with ID == statItemId.
 ****************************************************************************/
void    statisticUpdate( UInt16 statItemId, UInt16 statValue )
{
    assert( statItemId < SIM_STAT_ITEM_LAST );
    statisticData[statItemId] += statValue;
    /* Update current test suite statistic */
    statisticSuiteData[statItemId] += statValue;
}

/****************************************************************************
 * Function Name  : statisticGet 
 * Inputs         : statItemId - Statistic data item ID
                    fsuite - if LTE_TRUE get Test suite data
 * Outputs        : None.
 * Returns        : Statistic data item value.
 * Description    : Updates statistic data. It adds statValue to the Statistic data 
                    item with ID == statItemId
 ****************************************************************************/
UInt16    statisticGet( UInt16 statItemId, UInt16 fsuite )
{
    assert( statItemId < SIM_STAT_ITEM_LAST );
    if ( LTE_TRUE == fsuite )
    {
        return statisticSuiteData[statItemId];
    }
    else
    {
        return statisticData[statItemId];
    }
}

/****************************************************************************
 * Function Name  : statisticCommand
 * Inputs         : argv[0] - Statistic command string
                    argv[1].. - Statistic command parameters
                    argc - callback arguments counter
 * Outputs        : None.
 * Returns        : None.
 * Description    : Runs statistic engine command 
 ****************************************************************************/
void    statisticCommand( UInt32 argc, const UChar8** argv )
{
    UInt32      result = 0, i = 0;
    assert( 1 <= argc );
    assert( PNULL != argv[0] );
    userTolower( (UChar8*)argv[0] );
    /* find and run statistic command handler */
    for(i = 0; stat_table_func[i].str_id; i++)
    {
        if ( 0 == strcmp(argv[0], stat_table_func[i].str_id) )
        {
            result = (*stat_table_func[i].pfunc)(argc, (const UChar8**)argv);
            break;
        }
    }
}

/****************************************************************************
 * Function Name  : statisticBeginTestSuite 
 * Inputs         : argc - callback arguments counter
 *                  argv - callback arguments.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Update statistic in case of new Test suite started.
 ****************************************************************************/
UInt32    statisticBeginTestSuite( UInt32 argc, const UChar8** argv )
{
    if ( (2 > argc) || (PNULL == argv[1]) )
    {
        PDCP_LOG( LOG_WARNING ,"Invalid statisticcmd command format [%s]", argv[0]);
        return LTE_FALSE;
    }
    statisticTestSuiteNumber = atoi( argv[1] );
    statisticTestSuiteState = LTE_TRUE;
    /* reset test suite data */
    memSet( statisticSuiteData, 0, sizeof(statisticSuiteData) );
    statisticTestCaseState  = LTE_TRUE;
    memSet( statisticTestCaseNumbers, 0, sizeof(statisticTestCaseNumbers) );
    statisticTestCaseNumber = 0xFFFF;
    testSuiteBeginTime = lteGetSystemTime();
    PDCP_LOG( LOG_INFO, "Start time sec[%u] msec[%u]", testSuiteBeginTime.sec, testSuiteBeginTime.msec);
    return LTE_FALSE;
}

/****************************************************************************
 * Function Name  : statisticEndTestSuite 
 * Inputs         : argc - callback arguments counter
 *                  argv - callback arguments.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Update statistic in case of new Test suite stopped.
 ****************************************************************************/
UInt32    statisticEndTestSuite( UInt32 argc, const UChar8** argv )
{
    if ( LTE_TRUE == statisticTestSuiteState )
    {
        statisticUpdate( SIM_STAT_TEST_SUITES_SUCCEEDED, 1 );
    }
    else
    {
        /* store failed test suite number */
        if ( STAT_FAILED_TESTS_MAX_NUMBER > statisticGet(SIM_STAT_TEST_SUITES_FAILED, LTE_FALSE))
        {
            statisticTestSuiteNumbers[statisticGet(SIM_STAT_TEST_SUITES_FAILED, LTE_FALSE)] = statisticTestSuiteNumber;
        }
        statisticUpdate( SIM_STAT_TEST_SUITES_FAILED, 1 );
    }
    statisticUpdate( SIM_STAT_TEST_SUITES_AMOUNT, 1 );
    testSuiteEndTime = lteGetSystemTime();
    PDCP_LOG( LOG_INFO, "Test Suite end time sec[%u] msec[%u] diff[%u]", testSuiteEndTime.sec, testSuiteEndTime.msec, lteTimeDiff(&testSuiteEndTime, &testSuiteBeginTime) );
    return LTE_FALSE;
}

/****************************************************************************
 * Function Name  : statisticBeginTestCase 
 * Inputs         : argc - callback arguments counter
 *                  argv - callback arguments.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Update statistic in case of new Test case started.
 ****************************************************************************/
UInt32    statisticBeginTestCase( UInt32 argc, const UChar8** argv )
{
    if ( (2 > argc) || (PNULL == argv[1]) )
    {
        PDCP_LOG( LOG_WARNING ,"Invalid statisticcmd command format [%s]", argv[0]);
        return LTE_FALSE;
    }
    statisticTestCaseNumber = atoi( argv[1] );
    statisticTestCaseState = LTE_TRUE;
    return LTE_FALSE;
}

/****************************************************************************
 * Function Name  : statisticEndTestCase
 * Inputs         : argc - callback arguments counter
 *                  argv - callback arguments.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Update statistic in case of new Test case stopped.
 ****************************************************************************/
UInt32    statisticEndTestCase( UInt32 argc, const UChar8** argv )
{
    if ( LTE_TRUE == statisticTestCaseState )
    {
        statisticUpdate( SIM_STAT_TEST_CASES_SUCCEEDED, 1 );
    }
    else
    {
        /* store failed test case number */
        if ( STAT_FAILED_TESTS_MAX_NUMBER > statisticGet(SIM_STAT_TEST_CASES_FAILED, LTE_TRUE) )
        {
            statisticTestCaseNumbers[statisticGet(SIM_STAT_TEST_CASES_FAILED, LTE_TRUE)] = statisticTestCaseNumber;
        }

        statisticUpdate( SIM_STAT_TEST_CASES_FAILED, 1 );
    }
    statisticUpdate( SIM_STAT_TEST_CASES_AMOUNT, 1 );
    return LTE_FALSE;
}

/****************************************************************************
 * Function Name  : statisticUpdateTestResult
 * Inputs         : testResult - the result of test execution.
                                 LTE_FALSE - success
                                 LTE_TRUE
 *                  argv - callback arguments.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Update statistic data after each test execution.
 ****************************************************************************/
void    statisticUpdateTestResult( UInt16 testResult )
{
    if ( LTE_FALSE == testResult )
    { 
        statisticTestSuiteState = testResult;
        statisticTestCaseState = testResult;
    }
}

/****************************************************************************
 * Function Name  : statisticReportFailedItems
 * Inputs         : caption_p - text caption
                    items_p - failed items array
                    count - failed items number
 * Outputs        : None.
 * Returns        : None.
 * Description    : Reports statistic data items with captions from statisticCaption.
 ****************************************************************************/
void    statisticReportFailedItems( const UChar8* caption_p, const UInt16* items_p, UInt16 count )
{
    UInt16 i  = 0;
    UChar8 strBuf[LOG_MAX_STRING_LEN];  
    UChar8 strWork[LOG_MAX_STRING_LEN];
    if ( 0 < count )
    {   /* Report failed items numbers */
        sprintf( strBuf, "\n%s", caption_p);
        if ( STAT_FAILED_TESTS_MAX_NUMBER < count )
        {
            count = STAT_FAILED_TESTS_MAX_NUMBER;
        }
        for ( i = 0; i < count; i++ )   
        {
            sprintf( strWork, "%u; ", items_p[i]);
            strcat( strBuf, strWork );
        }
        PDCP_LOG( LOG_INFO, "%s", strBuf );
    }
}
/****************************************************************************
 * Function Name  : statisticReportDataItems
 * Inputs         : items_p - statistic items array
 * Outputs        : None.
 * Returns        : None.
 * Description    : Reports statistic data items with captions from statisticCaption.
 ****************************************************************************/
void    statisticReportDataItems( const UInt16* items_p )
{
    UInt16 i = 0;
    for ( i = 0; i < SIM_STAT_ITEM_LAST; i++)
    {
        if ( (SIM_STAT_TEST_SUITES_SUCCEEDED == i) ||
             (SIM_STAT_CMD_RUNSCRIPT_AMOUNT == i) )
        {
           PDCP_LOG( LOG_INFO, "\n");
        }
        PDCP_LOG( LOG_INFO, "%s: %u", statisticCaption[i], items_p[i] );
    }
}
#if 0
static UInt8 sendGetStatsReq(UInt8 toLayer)
{
    lteLog(LOG_DETAIL,PNULL,"###### %s()Entry ##### ",__func__);
    UInt8  buffer[32] = {0};
    UInt16 bytesSend = -1;
    UInt16 msgLen = 0;
    UInt32 choice = 0;
    UInt32 ueStartRange = 0;
    UInt32 ueEndRange = MAX_UE;
    UInt8 statsFlag = 0;

    if(MAC_LAYER == toLayer) {
        prepareMsgHeader(&buffer[0], MAC_GET_STATS_REQ, toLayer);
    } 
    else if(RLC_LAYER == toLayer) {
        prepareMsgHeader(&buffer[0], RLC_GET_STATS_REQ, toLayer);
    }
    else if(PDCP_LAYER == toLayer) {
            prepareMsgHeader(&buffer[0], PDCP_GET_STATS_REQ, toLayer);
    }
    msgLen += API_HEADER_LEN;

    if(RLC_LAYER == toLayer) {
       printf("Get TM Stats [0/1]:\n");
        scanf("%d", &choice);
        if(choice) {
            buffer[msgLen++] = GET_TM_STATS_REQ & 0x00FF;
            buffer[msgLen++] = (GET_TM_STATS_REQ & 0xFF00)>>8;
            buffer[msgLen++] = 4 & 0x00FF;
            buffer[msgLen++] = (4 & 0xFF00)>>8;
        }    
        printf("Get UM Stats [0/1]:\n");
        scanf("%d", &choice);
        if(choice) {
            buffer[msgLen++] = GET_UM_STATS_REQ & 0x00FF;
            buffer[msgLen++] = (GET_UM_STATS_REQ & 0xFF00)>>8;
            buffer[msgLen++] = 4 & 0x00FF;
            buffer[msgLen++] = (4 & 0xFF00)>>8;
        }    
        printf("Get AM Stats [0/1]:\n");
        scanf("%d", &choice);
        if(choice) {
            buffer[msgLen++] = GET_AM_STATS_REQ & 0x00FF;
            buffer[msgLen++] = (GET_AM_STATS_REQ & 0xFF00)>>8;
            buffer[msgLen++] = 4 & 0x00FF;
            buffer[msgLen++] = (4 & 0xFF00)>>8;
        }    
        printf("Get UE Stats [0/1]:\n");
        scanf("%d", &choice);
        if(choice) {
            buffer[msgLen++] = GET_UE_STATS_REQ & 0x00FF;
            buffer[msgLen++] = (GET_UE_STATS_REQ & 0xFF00)>>8;
            buffer[msgLen++] = 4 & 0x00FF;
            buffer[msgLen++] = (4 & 0xFF00)>>8;
        }   
/*        printf("Get Load Stats [0/1]:\n");
        scanf("%d", &choice);
        if(choice) {
            buffer[msgLen++] = GET_LOAD_STATS_REQ & 0x00FF;
            buffer[msgLen++] = (GET_LOAD_STATS_REQ & 0xFF00)>>8;
            buffer[msgLen++] = 4 & 0x00FF;
            buffer[msgLen++] = (4 & 0xFF00)>>8;
        }
*/
 
    }
    else if(PDCP_LAYER == toLayer)
    {
       //TODO
      statsFlag = 0 ; 
      printf("Get INTEGRITY Stats [0/1]:\n");
      scanf("%d",&choice);
      while(getchar() != '\n') continue;//To Flush stdin
      if(choice) {
        statsFlag =  1;
        buffer[msgLen++] = PDCP_GET_INTEGRITY_STATS_REQ & 0x00FF;
        buffer[msgLen++] = (PDCP_GET_INTEGRITY_STATS_REQ & 0xFF00)>>8;
        
        buffer[msgLen++] = 4 & 0x00FF;
        buffer[msgLen++] = (4 & 0xFF00)>>8;
     }
      printf("Get ROHC Stats [0/1]:\n");
      scanf("%d",&choice);
      while(getchar() != '\n') continue;//To Flush stdin
      if(choice) {
       statsFlag = 1;
        buffer[msgLen++] = PDCP_GET_ROHC_STATS_REQ  & 0x00FF;
        buffer[msgLen++] = (PDCP_GET_ROHC_STATS_REQ & 0xFF00)>>8;
        
        buffer[msgLen++] = 4 & 0x00FF;
        buffer[msgLen++] = (4 & 0xFF00)>>8;
     }
      printf("Get CIPHERING Stats [0/1]:\n");
      scanf("%d",&choice);
      while(getchar() != '\n') continue;//To Flush stdin
      if(choice) {
        statsFlag =  1;
        buffer[msgLen++] = PDCP_GET_CIPHERING_STATS_REQ & 0x00FF;
        buffer[msgLen++] = (PDCP_GET_CIPHERING_STATS_REQ & 0xFF00)>>8;
        buffer[msgLen++] = 4 & 0x00FF;
        buffer[msgLen++] = (4 & 0xFF00)>>8;
     }
      printf("Get UE Stats [0/1]:\n");
      scanf("%d",&choice);
      while(getchar() != '\n') continue;//To Flush stdin
      if(choice) {
        statsFlag = 1;
        buffer[msgLen++] = PDCP_GET_UE_STATS_REQ & 0x00FF;
        buffer[msgLen++] = (PDCP_GET_UE_STATS_REQ & 0xFF00)>>8;
        buffer[msgLen++] = 8 & 0x00FF;
        buffer[msgLen++] = ( 8 & 0xFF00)>>8;
        UChar8 repeat = 'n';
        do {
            ueStartRange = 0; 
            ueEndRange = MAX_UE;
            UInt8 numFlag ;
            printf("Enter UE Start Range [%u - %u] :\n",MIN_UE,MAX_UE);
            numFlag = 0; 
            numFlag = getRange(&ueStartRange);
            if(numFlag == 2 || numFlag == 0)
            {
                printf("Please enter valid range.\n");
                repeat = 'y';
                continue;
            }

            if(numFlag == 1)
            {
               printf("Enter UE End Range [%u - %u]:\n",MIN_UE,MAX_UE);
              numFlag = getRange(&ueEndRange);
            }
           
            if(numFlag == 1 )
            {
                if(ueStartRange >= MIN_UE && ueStartRange <= MAX_UE)
                {
                    if(ueEndRange >=  MIN_UE && ueEndRange <= MAX_UE)
                    {
                        repeat = 'n';
                        break;
                    }
                    else
                    {
                        printf("Please Enter valid range. \n");
                        printf("Do you want to enter again?[y/n]\n");
                        scanf("%c", &repeat);
                        while(getchar() != '\n') continue;//To Flush stdin
                        if(repeat != 'y' )
                        {
                            if( (ueEndRange - ueStartRange) > UE_MAX_RANGE) {
                                ueEndRange = ueStartRange + ( UE_MAX_RANGE - ueStartRange - 1);
                            }
                            break;
                        }
                    }
                }
                else
                {
                    printf("Please Enter valid range. \n");
                    repeat = 'y';
                    continue;
                }
            }
            else
            {
                printf("Please Enter valid range.\n");
                printf("Do you want to enter again?[y/n]\n");
                scanf("%c", &repeat);
                while(getchar() != '\n') continue;//To Flush stdin
            }
        }while(repeat == 'y');

        buffer[msgLen++] = ueStartRange & 0x00FF;
        buffer[msgLen++] = ( ueStartRange & 0xFF00)>>8;
        
        buffer[msgLen++] =  ueEndRange & 0x00FF;
        buffer[msgLen++] = ( ueEndRange & 0xFF00)>>8;
        
     }
    }//endif(PDCP_LAYER)   

    buffer[(API_HEADER_LEN - 2)] = (msgLen & 0x00FF);
    buffer[(API_HEADER_LEN - 1)] = (msgLen & 0xFF00)>>8;

    if(MAC_LAYER == toLayer) {

        bytesSend = sendMsgToMACbySocket(buffer, msgLen);
        printf("====================================\n");
        printf("MAC Send >>: Statistics Request\n");
//        printf("%d bytes of data sent\n ",bytesSend);
        printf("====================================\n");
    } 
    else if(RLC_LAYER == toLayer) { 
        bytesSend = sendMsgToRLCbySocket(buffer, msgLen);
        printf("====================================\n");
        printf("RLC Send >>: Statistics Request\n");
//        printf("%d bytes of data sent\n ",bytesSend);
        printf("====================================\n");
    }    
    else if(PDCP_LAYER == toLayer) { 
        if(statsFlag == 1)
        {
            bytesSend = sendMsgToPDCPbySocket(buffer, msgLen);
            printf("====================================\n");
            printf("PDCP Send >>: Statistics Request\n");
//            printf("%d bytes of data sent\n ",bytesSend);
            printf("====================================\n");
        }
        else
        {
            printf("====================================\n");
            printf("PDCP Send >>:No Statistics Request\n");
            printf("====================================\n");
            fprintf (stderr,"\nOAMSIM Enter Choice[Press H/h to display menu]:");
        }

    }    
    lteLog(LOG_DETAIL,PNULL,"send  bytes for Setting log level = %d\n",bytesSend);
    return 1;
}
#endif
