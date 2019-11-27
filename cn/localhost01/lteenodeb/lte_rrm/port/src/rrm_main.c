/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_main.c $
 *
 ******************************************************************************
 *
 * File Description: This file contains contains the main function function of 
 *                   the RRM binary. It registers various RRM CSPL modules.
 *        
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   3 Mar, 2012     ---------   Initial
 *
 ******************************************************************************/

/**************************************************************************** 
 *  Standard Library Includes : shall not be done 
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <cspl.h>
#include "rrm_defines.h"
#include "rrm_utils.h"
#include <pthread.h>

#ifdef LINUX_PC_TEST
#include <signal.h>
#include "TestFrameApi.h"
short adapter_hdl_msg_callback(char *buf, long len);
short adapter_init_socket(void);
void adapter_register_send_data_to_queue(pfun_adapter_send_data_to_queue pfun);
extern U8 for_adapter_send_data_to_queue(U16 ulSrcModId, U16 ulDstModId, U16 ulMsgId, char *msg, U32 ulMsgLen);

void rrm_handleFatalSignal(int signum)
{
	signal(signum, SIG_DFL);
	fprintf(stderr,"rrm_handleFatalSignal----quit!\n");
	exit(0);

	return ;
}
void rrm_signal_handler()
{
      signal(SIGPIPE, SIG_IGN);

	signal(SIGQUIT, rrm_handleFatalSignal);
	signal(SIGILL, rrm_handleFatalSignal);
	signal(SIGABRT, rrm_handleFatalSignal);
	signal(SIGFPE, rrm_handleFatalSignal);
	signal(SIGSEGV, rrm_handleFatalSignal);
	signal(SIGBUS, rrm_handleFatalSignal);
	signal(SIGINT, rrm_handleFatalSignal);
	signal(SIGKILL, rrm_handleFatalSignal);
	signal(SIGTERM, rrm_handleFatalSignal);
}
#endif

#ifdef RRM_UT_FRAMEWORK
#include "rrm_ut_stub_db.h"
#else
#define RRM_UT_STUB_THRD  RRM_ZERO
#endif
U8* p_config_file;

rrm_l2_msgq_intf_data      *p_rrm_l2_msgq_intf_glb_data = RRM_PNULL;
rrm_pdcp_msgq_intf_data    *p_rrm_pdcp_msgq_intf_glb_data = RRM_PNULL;

#if (RRM_ADV_LOGGER == LOGGER_TYPE)
AdvL_ContextIndex g_main_advl_ctxt;
#endif

/* Changes for logging enhancement */
pthread_key_t cell_and_ue_index_key;

void set_cell_index(U8 cell_index)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info == NULL)
    {
        info = (cell_and_ue_index*) malloc(sizeof(cell_and_ue_index));
        if(NULL == info){/*cov 96724 fix*/
            return;
        }
    }
    
    info->cell_index = cell_index;
    pthread_setspecific(cell_and_ue_index_key, info);
}

void set_ue_index(U16 ue_index)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info == NULL)
    {
        info = (cell_and_ue_index*) malloc(sizeof(cell_and_ue_index));
        if(NULL == info){/*cov 96756 fix*/
            return;
        }

    }

    info->ue_index = ue_index;
    pthread_setspecific(cell_and_ue_index_key, info);
}

void set_cell_and_ue_index(U8 cell_index, U16 ue_index)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info == NULL)
    {
        info = (cell_and_ue_index*) malloc(sizeof(cell_and_ue_index));
        if(NULL == info){/*cov 96714 fix*/
            return;
        }

    }

    info->cell_index = cell_index;
    info->ue_index = ue_index;
    pthread_setspecific(cell_and_ue_index_key, info);
}

U8 get_cell_index(void)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info)
    {
        return info->cell_index;
    }
    return 0xff;
}

U16 get_ue_index(void)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info != NULL)
    {
        return info->ue_index;
    }
    return 0xffff;
}

// destructor
void free_cell_and_ue_index(void* info)
{
    free((cell_and_ue_index*)info);
}

/* Utility function to split addr to ip and port */
/*****************************************************************************
 * Function Name  : main 
 * Inputs         : void 
 * Outputs        : void
 * Returns        : 
 * Description    : This function is the starting point of RRM
 ****************************************************************************/
int main(int argc, char** argv)
{
    pthread_key_create(&cell_and_ue_index_key, free_cell_and_ue_index);
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);

    rrm_void_t *listener = RRM_PNULL;
    cpu_set_t       cpuSetSch;
    unsigned int    coreNum = 0;
    thrd_arg_t      thd_arg[RRM_MAX_THRD + RRM_UT_STUB_THRD];
   

    if ((argc < 2) || (argc > 3)) 
    {
        fprintf(stderr,"Usage is : ./lte_rrm <core num to bind> <configuration file> >\n");
        return 0;
    }
    if ((argc > 2) && (argc < 4))
    {
        /* Setting Configutation file */
        p_config_file = ((U8*)argv[2]);
    }
    else
    {
        p_config_file = RRM_PNULL;
    }

    coreNum = RRM_ATOI(argv[1]);
    /*  coverity fix :56718 */
    if (coreNum >= (RRM_EIGHT * sizeof(cpuSetSch)))
    {
        fprintf(stderr,"Invalid core number[%d]\n", coreNum);
        return 0;
    }
    RRM_CPU_ZERO(&cpuSetSch);
    RRM_CPU_SET(coreNum ,&cpuSetSch);
    pid_t p = 0;
    if (0 != RRM_SCHED_SETAFFINITY(p, sizeof(cpu_set_t), &cpuSetSch))
    {
        fprintf(stderr, " sched_setaffinity Returned Error in RRM %s Core not Binded", argv[1]);
        rrm_abort();
    }

    /* Init the CSPL OS */
    qvSimpleInit(&rrm_os);
#ifdef RRM_UT_FRAMEWORK
    stub_init_suit();
#endif
    /*Initializing the memory for RRM-L2 Message Q interface Data Structure */
    p_rrm_l2_msgq_intf_glb_data = (rrm_l2_msgq_intf_data *)rrm_malloc(sizeof(rrm_l2_msgq_intf_data));
    if (PNULL == p_rrm_l2_msgq_intf_glb_data)
    {
        fprintf(stderr, "Memory not available for  RRM-L2 Message Q interface Data Structure" );
        rrm_abort();
    }
    p_rrm_pdcp_msgq_intf_glb_data = (rrm_pdcp_msgq_intf_data *)rrm_malloc(sizeof(rrm_pdcp_msgq_intf_data));
    if (PNULL == p_rrm_pdcp_msgq_intf_glb_data)
    {
        fprintf(stderr, "Memory not available for  RRM-L2 Message Q interface Data Structure" );
        rrm_abort();
    }
    /* qvSimpleInit(&rrm_os) has been moved to top for proper initialization of pools 
     * before doing any task */
#if (RRM_ADV_LOGGER == LOGGER_TYPE)
    /* Initialize the advance logger and set its level to RRM_ERROR for
       any log for common code (not specific to any module). */	
      /*CID 86868 Start*/
      if(LogSuccess != init_log(RRM_LOG_CTXT_FILE, DEFAULT_LOG_FILE, &g_main_advl_ctxt))
      {
        fprintf(stderr, "Log Init Unsuccessful" );
      }
      /*CID 86968 End*/
      /*coverity 16553, aditya, rel2.0.1, RRM_OAM_ERROR changed to LogAlert enum value*/
      set_loglevel_limit(g_main_advl_ctxt, LogAlert);
#elif (RRM_INTERNAL_LOGGER == LOGGER_TYPE)
    /* Set global log level to RRM_LOG_LEVEL_NONE so that CSPL does not
       print any log for common code (not specific to any module). This
        code report any error using SON_ENCAPS_WARNING and  SON_PANIC*/
       qvLogLevel(RRM_LOG_LEVEL_NONE);
#endif
                            

    /* Set global log level to RRM_LOG_LEVEL_NONE so that CSPL prints all the        
       error logs for common code (not specific to any module) */

    /* coverity, tainted error removed, RRM_TRACE_MACRO used for trace file, Aditya*/
    /* log file */

    RRM_UT_TRACE_ENTER();

    /*Open the channel for communication services*/
    listener = qvOpen(&rrm_shell, RRM_PNULL);
    if (listener == RRM_PNULL)
    {
        /* Couldn't initialize connection to external entities */
        qvLog( 0, "qvOpen failed" );
        return RRM_FAILURE;
    }

    /* Set RRC/OAM/SON as the external destination */
    qvSetDestination(RRC_MODULE_ID, 
            &rrm_shell, 
            listener,
            0);

    qvSetDestination(RRM_OAM_MODULE_ID,
            &rrm_shell,
            listener,
            0);

    qvSetDestination(RRM_SON_MODULE_ID,
            &rrm_shell,
            listener,
            0);

    qvSetDestination(RRM_SON_APPS_MODULE_ID,
            &rrm_shell,
            listener,
            0);

    qvSetDestination(RRM_SON_ANR_MODULE_ID,
            &rrm_shell,
            listener,
            0);

    qvSetDestination(RRM_SON_ES_MODULE_ID,
            &rrm_shell,
            listener,
            0);

    qvSetDestination(RRM_SON_NMM_MODULE_ID,
            &rrm_shell,
            listener,
            0);

    qvSetDestination(RRM_SON_MLB_MODULE_ID,
            &rrm_shell,
            listener,
            0);

    qvSetDestination(RRM_SON_MRO_MODULE_ID,
            &rrm_shell,
            listener,
            0);

    qvSetDestination(RRM_SON_RACH_OPT_MODULE_ID,
            &rrm_shell,
            listener,
            0);

    qvSetDestination(RRM_L2_MODULE_ID,
            &rrm_shell,
            listener,
            0);
    qvSetDestination(PDCP_MODULE_ID,
                &rrm_shell,
                listener,
                0);
    /** Register the modules RRM_CELLM, RRM_UEM, RRM_MIF, Logger */ 
    thd_arg[RRM_LOGGER_THRD].driver_id = qvNewDriver();
    thd_arg[RRM_LOGGER_THRD].p_qwait = &rrm_waitstruct;
    thd_arg[RRM_LOGGER_THRD].logical_thid = RRM_LOGGER_THRD;
    rrm_register_modules(&thd_arg[RRM_LOGGER_THRD]);

#ifdef LINUX_PC_TEST
	adapter_init_socket();
	TestInitFrame("192.168.201.31", 54401);
	TestFrameRegisterMsgHandler(adapter_hdl_msg_callback);
	adapter_register_send_data_to_queue(for_adapter_send_data_to_queue);
	rrm_signal_handler();
#endif

#ifdef RRM_IN_MEM_LOGGER
    lgr_logger_register_thread(RRM_MAIN_THRD);
#endif
#ifdef RRM_UT_FRAMEWORK
    qvSetDestination(RRM_MODULE_ID,
                &rrm_shell,
                listener,
                0);
    /* RRC Stub */
    thd_arg[RRM_MAX_THRD + RRM_ONE].driver_id = qvNewDriver();
    thd_arg[RRM_MAX_THRD + RRM_ONE].p_qwait = &rrm_waitstruct;
    thd_arg[RRM_MAX_THRD + RRM_ONE].logical_thid = RRM_MAX_THRD + RRM_ONE;
    stub_rrc_register(&thd_arg[RRM_MAX_THRD + RRM_ONE]);
   
    /* L2 Stub */
    thd_arg[RRM_MAX_THRD + RRM_TWO].driver_id = qvNewDriver();
    thd_arg[RRM_MAX_THRD + RRM_TWO].p_qwait = &rrm_waitstruct;
    thd_arg[RRM_MAX_THRD + RRM_TWO].logical_thid = RRM_MAX_THRD + RRM_TWO;
    stub_mac_register(&thd_arg[RRM_MAX_THRD + RRM_TWO]);

    /* SON Stub */
    thd_arg[RRM_MAX_THRD + RRM_THREE].driver_id = qvNewDriver();
    thd_arg[RRM_MAX_THRD + RRM_THREE].p_qwait = &rrm_waitstruct;
    thd_arg[RRM_MAX_THRD + RRM_THREE].logical_thid = RRM_MAX_THRD + RRM_THREE;
    stub_son_register(&thd_arg[RRM_MAX_THRD + RRM_THREE]);
    
    /* PDCP Stub */
    thd_arg[RRM_MAX_THRD + RRM_FOUR].driver_id = qvNewDriver();
    thd_arg[RRM_MAX_THRD + RRM_FOUR].p_qwait = &rrm_waitstruct;
    thd_arg[RRM_MAX_THRD + RRM_FOUR].logical_thid = RRM_MAX_THRD + RRM_FOUR;
    stub_pdcp_register(&thd_arg[RRM_MAX_THRD + RRM_FOUR]);
    
    /* OAM Stub */
    thd_arg[RRM_MAX_THRD].driver_id = qvNewDriver();
    thd_arg[RRM_MAX_THRD].p_qwait = &rrm_waitstruct;
    thd_arg[RRM_MAX_THRD].logical_thid = RRM_MAX_THRD;
    stub_oam_register(&thd_arg[RRM_MAX_THRD]);

#endif
    /* RUN the system */ 
    qvRun(&rrm_shell, listener, &rrm_waitstruct, 0);
    RRM_UT_TRACE_EXIT();
    return RRM_SUCCESS;
}

