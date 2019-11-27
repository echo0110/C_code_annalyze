/************************************************************************************
*
*   FILE NAME  : testStatistic.c
*
*   DESCRIPTION: Statistic unit implementation.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <sys/timeb.h>

#include "simLogger.h"

#include "simu_table_func.h"
#include "userUtils.h"
#include "testStatistic.h"

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
    "Number of Test suites                  ",
    "Number of Test cases                   ",
    "Number of Tests                        ",

    "Number of Test suites succeeded        ",
    "Number of Test suites failed           ",

    "Number of Test cases succeeded         ",
    "Number of Test cases failed            ",


    "Number of Tests succeeded              ",
    "Number of Tests failed                 ",
    "Number of Tests timeouts               ",

    "Number of runscript command calls      ",
    "Number of sleep command calls          ",
    "Number of echo command calls           ",
    "Number of redirectlog command calls    ",
    "Number of sendandcheck command calls   ",
    "Number of loadtosock command calls     ",
    "Number of sendcmd command calls        ",
    "Sleep time, msec                       ",

    "Simulator Warning                      ",
    "Simulator Errors                       "
};


/* Statistic commands handlers */
static const CallbackFuncId stat_table_func[] =
{
    {"reset",            statisticReset                 },
    {"report",           statisticReport                },
    {"begintestsuite",   statisticBeginTestSuite        },
    {"endtestsuite",     statisticEndTestSuite          },
    {"begintestcase",    statisticBeginTestCase         },
    {"endtestcase",      statisticEndTestCase           },
};

/* Current Test suite state. SIM_FALSE - succeeded*/
static UInt16 statisticTestSuiteState = SIM_TRUE;
/* Current Test suite number. */
static UInt16 statisticTestSuiteNumber = 0xFFFF;
/* Failed Test suite numbers array. */
static UInt16 statisticTestSuiteNumbers[STAT_FAILED_TESTS_MAX_NUMBER] = { 0 };

struct timeb testSuiteBeginTime;
struct timeb testSuiteEndTime;

/* Test case state. SIM_FALSE - succeeded*/
static UInt16 statisticTestCaseState = SIM_TRUE;;
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
        SIM_LOG(SIM_LOG_WARNING,"Invalid command format");
        return SIM_FALSE;
    }


    SIM_LOG( SIM_LOG_INFO, "COMMAND[ statisticcmd ] %s", argv[0] );

    statisticCommand( argc, argv );
    return SIM_FALSE;
}

/****************************************************************************
 * Function Name  : statisticReset
 * Inputs         : argc - callback arguments counter
 *                  argv - callback arguments.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Resets statistic data.
 ****************************************************************************/
UInt32 statisticReset( UInt32 argc, const UChar8** argv )
{
    memset( statisticData, 0, sizeof(statisticData) );

    statisticTestSuiteState = SIM_TRUE;
    memset( statisticTestSuiteNumbers, 0, sizeof(statisticTestSuiteNumbers) );
    statisticTestSuiteNumber = 0xFFFF;
    memset( statisticSuiteData, 0, sizeof(statisticSuiteData) );

    statisticTestCaseState  = SIM_TRUE;
    memset( statisticTestCaseNumbers, 0, sizeof(statisticTestCaseNumbers) );
    statisticTestCaseNumber = 0xFFFF;
    return SIM_FALSE;
}

/****************************************************************************
 * Function Name  : statisticInit
 * Inputs         : Initialize statistic Unit
 * Outputs        : None.
 * Returns        : None.
 * Description    : Initialize statistic Unit
 ****************************************************************************/
void statisticInit(void)
{
    statisticReset(0,0);
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
    SIM_LOG( SIM_LOG_INFO, "\n\n\n" );
    SIM_LOG( SIM_LOG_INFO, "**************** Statistic Report Begin *****************" );

    if ( (2 == argc) && (PNULL != argv[1]) && (0 == strcmp("suite", argv[1])) )
    {   /* Test suite report */
        SIM_LOG( SIM_LOG_INFO, "TEST SUITE: %u", statisticTestSuiteNumber);

        statisticReportDataItems( statisticSuiteData );

        statisticReportFailedItems(
            "Failed Test cases : ",
            statisticTestCaseNumbers,
            statisticGet(SIM_STAT_TEST_CASES_FAILED, SIM_TRUE) );
    }
    else
    {   /* Global report */
        statisticReportDataItems( statisticData );

        statisticReportFailedItems(
            "Failed Test suites : ",
            statisticTestSuiteNumbers,
            statisticGet(SIM_STAT_TEST_SUITES_FAILED, SIM_FALSE) );
    }

    SIM_LOG( SIM_LOG_INFO, "**************** Statistic Report End *******************" );

    return SIM_FALSE;
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
                    fsuite - if SIM_TRUE get Test suite data
 * Outputs        : None.
 * Returns        : Statistic data item value.
 * Description    : Updates statistic data. It adds statValue to the Statistic data
                    item with ID == statItemId
 ****************************************************************************/
UInt16    statisticGet( UInt16 statItemId, UInt16 fsuite )
{
    assert( statItemId < SIM_STAT_ITEM_LAST );

    if ( SIM_TRUE == fsuite )
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
    UInt32      i;
    UInt32      result = 0;

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
        SIM_LOG( SIM_LOG_WARNING ,"Invalid statisticcmd command format [%s]", argv[0]);
        return SIM_FALSE;
    }

    statisticTestSuiteNumber = atoi( argv[1] );
    statisticTestSuiteState = SIM_TRUE;

    /* reset test suite data */
    memset( statisticSuiteData, 0, sizeof(statisticSuiteData) );

    statisticTestCaseState  = SIM_TRUE;
    memset( statisticTestCaseNumbers, 0, sizeof(statisticTestCaseNumbers) );
    statisticTestCaseNumber = 0xFFFF;

    ftime( &testSuiteBeginTime );
    SIM_LOG( SIM_LOG_INFO, "Start time sec[%d] msec[%u]", (unsigned long)testSuiteBeginTime.time, testSuiteBeginTime.millitm );
    return SIM_FALSE;
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
    if ( SIM_TRUE == statisticTestSuiteState )
    {
        statisticUpdate( SIM_STAT_TEST_SUITES_SUCCEEDED, 1 );
    }
    else
    {
        /* store failed test suite number */
        if ( STAT_FAILED_TESTS_MAX_NUMBER > statisticGet(SIM_STAT_TEST_SUITES_FAILED, SIM_FALSE) )
        {
            statisticTestSuiteNumbers[statisticGet(SIM_STAT_TEST_SUITES_FAILED, SIM_FALSE)] = statisticTestSuiteNumber;
        }

        statisticUpdate( SIM_STAT_TEST_SUITES_FAILED, 1 );
    }

    statisticUpdate( SIM_STAT_TEST_SUITES_AMOUNT, 1 );

    ftime( &testSuiteEndTime );

    SIM_LOG( SIM_LOG_INFO, "Test Suite end time sec[%d] msec[%u] diff[%u]", (unsigned long)testSuiteEndTime.time, testSuiteEndTime.millitm, simTimeDiff(&testSuiteEndTime, &testSuiteBeginTime) );

    return SIM_FALSE;
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
        SIM_LOG( SIM_LOG_WARNING ,"Invalid statisticcmd command format [%s]", argv[0]);
        return SIM_FALSE;
    }

    statisticTestCaseNumber = atoi( argv[1] );
    statisticTestCaseState = SIM_TRUE;

    return SIM_FALSE;
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
    if ( SIM_TRUE == statisticTestCaseState )
    {
        statisticUpdate( SIM_STAT_TEST_CASES_SUCCEEDED, 1 );
    }
    else
    {
        /* store failed test case number */
        if ( STAT_FAILED_TESTS_MAX_NUMBER > statisticGet(SIM_STAT_TEST_CASES_FAILED, SIM_TRUE) )
        {
            statisticTestCaseNumbers[statisticGet(SIM_STAT_TEST_CASES_FAILED, SIM_TRUE)] = statisticTestCaseNumber;
        }

        statisticUpdate( SIM_STAT_TEST_CASES_FAILED, 1 );
    }

    statisticUpdate( SIM_STAT_TEST_CASES_AMOUNT, 1 );
    return SIM_FALSE;
}

/****************************************************************************
 * Function Name  : statisticUpdateTestResult
 * Inputs         : testResult - the result of test execution.
                                 SIM_FALSE - success
                                 SIM_TRUE
 *                  argv - callback arguments.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Update statistic data after each test execution.
 ****************************************************************************/
void    statisticUpdateTestResult( UInt16 testResult )
{
    if ( SIM_FALSE == testResult )
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
    UInt16 i;
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
        SIM_LOG( SIM_LOG_INFO, "%s", strBuf );
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
    UInt16 i;

    for ( i = 0; i < SIM_STAT_ITEM_LAST; i++)
    {
        if ( (SIM_STAT_TEST_SUITES_SUCCEEDED == i) ||
             (SIM_STAT_CMD_RUNSCRIPT_AMOUNT == i) )
        {
           SIM_LOG( SIM_LOG_INFO, "");
        }

        SIM_LOG( SIM_LOG_INFO, "%s: %u", statisticCaption[i], items_p[i] );
    }
}
