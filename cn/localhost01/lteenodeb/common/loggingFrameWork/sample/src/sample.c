/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: $ sample.c,v  2011/01/27 06:00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :This file contains a sample implementation for using 
 *                    logging framework.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: $ sample.h,c $
 * Initial version
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/
#include<stdio.h>

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "logging.h"
#include "signal_handler.h"
#include<netinet/in.h>
#include "socket_utils.h"
#include "shm_utils.h"
#include "alarm.h"
#include "sample.h"

/****************************************************************************
 Private Definitions
 ****************************************************************************/
#define SAMPLE_MODULE_ID 3
#define SAMPLE_SHARED_MEM_NAME  "SAMPLE_APP"

#define OAM_IP_ADDR "127.0.0.1"
#define OAM_UDP_PORT  6666
#define OAM_MODULE_ID 0

/****************************************************************************
 Private Types
 ****************************************************************************/

 /****************************************************************************
  * Private Function Prototypes
  ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

/****************************************************************************
 * Function Name  : main
 * Inputs         : None
 * Outputs        : None
 * Returns        : U32
 * Description    : Sample to use logging framework and signal handler
 ****************************************************************************/
/* + SPR 17439 */
int main(void)
/* - SPR 17439 */    
{
    /*warning-fix +-*/
   
    /*Initialize logger frwk*/
    logger_init(0, SAMPLE_SHARED_MEM_NAME,MAX_LOG_MEMORY_SIZE);
    
    /*Initialize alarm frwk */
    /* +- SPR 17777 */
    alarm_init(OAM_MODULE_ID,OAM_IP_ADDR,OAM_UDP_PORT);
    /* +- SPR 17777 */

    /*Enable log Level and Log category for sample application*/
    enable_log_level(LOGDEBUG);
    enable_log_category(MAC_L1_INF);

    /*Get log Level and Log Category for sample application */
    printf("Log Level: %u\n",get_log_level());
    printf("Log Category: %llu\n",get_log_category());
    
     /*gcc warning fix +-*/
    /*Send Alarm*/
    do
    {
        ALARM_FMT_MSG(SAMPLE_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,
                CRITICAL_ALARM,"%s","memAlloc Fail");
        ALARM_FMT_MSG(SAMPLE_MODULE_ID, TIMER_START_FAILED_ALARM_ID,
                MAJOR_ALARM, "%s %s","timer start","Fail");
        ALARM_FMT_MSG(SAMPLE_MODULE_ID, TIMER_STOP_FAILED_ALARM_ID, MINOR_ALARM,
                "%s", "Timer Stop Fail");
    }while(0);

    return 0;    
}

//#ifdef LINUX_PC_TEST
void SEND_TO_TEST(unsigned int ulMsgId, char* buf, int len)
{
	return;
}
//#endif

//#ifdef LINUX_PC_TEST
short test_log(unsigned short  usModId,unsigned short logLevel,char*fmt,...)
{
}
//#endif