
/************************************************************************************
*
*   FILE NAME  : testStatistic.h
*
*   DESCRIPTION: Statistic unit definition.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc.
*
*************************************************************************************/

#ifndef __TEST_STATISTIC_H__
#define __TEST_STATISTIC_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include <lteTypes.h>

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Statistic items list */
enum StatisticItemIdsT
{
    SIM_STAT_TEST_SUITES_AMOUNT = 0,     /*0*/      /* updated by beginTestSuite command */
    SIM_STAT_TEST_CASES_AMOUNT,          /*1*/      /* updated by beginTestCase command */
    SIM_STAT_TESTS_AMOUNT,               /*2*/      /* updated by sendandcheck command */

    SIM_STAT_TEST_SUITES_SUCCEEDED,      /*3*/     /* updated by endTestSuite commands */
    SIM_STAT_TEST_SUITES_FAILED,         /*4*/

    SIM_STAT_TEST_CASES_SUCCEEDED,       /*5*/      /* updated by endTestCase commands */
    SIM_STAT_TEST_CASES_FAILED,          /*6*/
    
    SIM_STAT_TESTS_SUCCEEDED,            /*7*/      /* updated by sendandcheck command */
    SIM_STAT_TESTS_FAILED,               /*8*/
    SIM_STAT_TESTS_TIMEOUT,              /*9*/

    SIM_STAT_CMD_RUNSCRIPT_AMOUNT,       /*A*/
    SIM_STAT_CMD_SLEEP_AMOUNT,           /*B*/
    SIM_STAT_CMD_ECHO_AMOUNT,            /*C*/
    SIM_STAT_CMD_REDIRECTLOG_AMOUNT,     /*D*/
    SIM_STAT_CMD_SENDANDCHECK_AMOUNT,    /*E*/
    SIM_STAT_CMD_LOADTOSOCK_AMOUNT,      /*F*/
    SIM_STAT_CMD_SENDCMD_AMOUNT,         /*10*/
    SIM_STAT_SLEEP_TIME,                 /*11*/

    SIM_STAT_ITEM_LAST,
};

/************************************************************************************
 * Global variable
 ************************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/************************************************************************************
 * FUNCTION NAME    :   userStatisticCommandCallback
 * DESCRIPTION      :   Runs statistic engine command  
 * PARAMATERS       :   [IN] UInt32 argc - callback arguments counter
 *                      [IN] const UChar8** argv - callback arguments
 * RETURN VALUE     :   [OUT] UInt32 - error code
 ************************************************************************************/
UInt32 userStatisticCommandCallback(UInt32 argc, const UChar8** argv);

/****************************************************************************
 * Function Name  : statisticCommand
 * Inputs         : argv[0] - Statistic command string
                    argv[1].. - Statistic command parameters
 * Outputs        : None.
 * Returns        : None.
 * Description    : Runs statistic engine command 
 ****************************************************************************/
void    statisticCommand( UInt32 argc, const UChar8** argv );

/****************************************************************************
 * Function Name  : statisticUpdate 
 * Inputs         : statItemId - Statistic data item ID
                    statValue - Value to add
 * Outputs        : None.
 * Returns        : None.
 * Description    : Updates statistic data. It adds statValue to the Statistic data 
                    item with ID == statItemId.
 ****************************************************************************/
void    statisticUpdate( UInt16 statItemId, UInt16 statValue );

/****************************************************************************
 * Function Name  : statisticGet 
 * Inputs         : statItemId - Statistic data item ID
                    fsuite - if LTE_TRUE get Test suite data
 * Outputs        : None.
 * Returns        : Statistic data item value.
 * Description    : Get statistic data for the Statistic data item with ID == statItemId.
 ****************************************************************************/
UInt16    statisticGet( UInt16 statItemId, UInt16 fsuite );

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
void    statisticUpdateTestResult( UInt16 testResult );

#endif /*__TEST_STATISTIC_H__*/
