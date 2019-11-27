/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_trace_mcr.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the common trace macros which are to
 *                     be used by various stack modules.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_trace_mcr.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 04:41:36  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/

#ifndef _ROHC_TRACE_MAC_H_
#define _ROHC_TRACE_MAC_H_

#include "rohc_typ.h"
#include "rohc_stats_mcr.h"
#include "lteLog.h"
#include "ltePdcpCmnTools.h"

#define ROHC_LOG(param1,...) 
/* SPR +- 17777 */
#ifdef UE_SIM_TESTING
#define LOG_PDCP_MSG(logId, logLevel, logCategory, globalTick, \
        intValOne, intValTwo, intValThree,intValFour,\
        intValFive,floatValOne,floatValTwo,strValOne,strValTwo)
#endif
/* SPR +- 17777 */
#define ROHC_STACK_TRACE(trace_level, trace_level_str)
#define ROHC_HEX_DUMP(trace_flag, p_mesg, noctets) 
//#define ROHC_LOG(trace_flag, trace_level_str)

#ifdef ROHC_UT_FLAG
#define ROHC_UT_TRACE_LEVEL 3U  
        
/*******************************************************************
Macro : Printing Each function Name while entering/exiting function 
********************************************************************
Parameters :
    fn_name_m   : Funtion Name string 
*******************************************************************/
        
#define ROHC_ENTER_FN(fn_name_m)  \
        ROHC_LOG(LOG_INFO, " Entering Function : %s\n",\
                        fn_name_m)

#define ROHC_EXIT_FN(fn_name_m)  \
        ROHC_LOG(LOG_INFO," Exiting Function : %s\n",\
                        fn_name_m)

#define ROHC_DUMP_CPIB  rohc_dump_cpib
#define ROHC_DUMP_DPIB  rohc_dump_dpib

#else /* ROHC_UT_FLAG*/
#define ROHC_ENTER_FN(fn_name_m)  
#define ROHC_EXIT_FN(fn_name_m)  
#define ROHC_DUMP_CPIB  
#define ROHC_DUMP_DPIB  
#endif /* ROHC_UT_FLAG */ 

#ifdef ROHC_UT_FLAG
#define ROHC_UT_TRACE(trace_str) fprintf trace_str
#else
#define ROHC_UT_TRACE(trace_str)
#endif


/*******************************************************************
Macro : Memory Allocation failure : 
        Print Trace, Increment statistics, call error reporter 
********************************************************************
Parameters :
    pool    : Pool Name string 
*******************************************************************/
#define ROHC_MEM_FAIL(p_rohc_entity_m, pool)     do{\
                    ROHC_STACK_TRACE(ROHC_BRIEF_TRACE, \
                    "ROHC Memory Allocation Failure : %s %d %s",\
                     __FILE__,__LINE__,pool);\
                    ROHC_STATS_INCR(p_rohc_entity_m, PDCP_ROHC_STATS_ERR_MEMGET_FAIL);\
                    ROHC_ERROR_REPORTER(pool);\
                    }while(0)   
        
#endif /* _ROHC_TRACE_MAC_H_ */


