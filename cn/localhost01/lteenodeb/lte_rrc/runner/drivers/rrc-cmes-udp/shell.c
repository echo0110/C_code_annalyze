/******************************************************************************
 *
 *   FILE NAME:
 *       shell.c
 *
 *   DESCRIPTION:
 *       Here is implementation of CSPL driver. It is responsible for
 *       communication with external entities through sockets.
 *       It manage with messages headers and
 *       determine internal destination RRC module.
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   23 July 2009    VasylS      ---------       Initial
 *
 *   Copyright (c) 2009, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/

#define _GNU_SOURCE


#include <lteMisc.h>
#include <cspl.h>
#include <shell.h>
#include <pthread.h>
#include "rrc_defines.h"
#include "rrc_handlers.h"
#include "rrc_ext_api.h"
#include "rrc_msg_mgmt.h"
#include "rrc_common_utils.h"
#include "sctp_wrappers.h"
#include "s1ap_api.h"
#include "x2ap_api.h"  /* L3-X2AP Integration Activity */
#include "rrc_logging.h"
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
#include "advlogging.h"
#endif
#ifdef LTE_EMBMS_SUPPORTED
#include "m2ap_api.h"  /* L3-X2AP Integration Activity */
#endif
/* Defines */
#define MAX_INET_ADDRESS_LENGTH 20

/* Logger for driver */
#define RRC_DRV_TRACE(log_level, format, ...) \
	RRC_LOG((const S8*)"CMES UDP Driver", log_level, format, ##__VA_ARGS__)

#define INVALID_SOCKET  (-1)
#define SOCKET_ERROR    (-1)

#ifdef LATENCY_TRACE_ON
g_latency_data_t g_latency_data[200];
U8 g_index = 0;
U8 start_logging = 0;
#endif

/* Type */
typedef S32 SOCKET;
/* Function */
extern QDRIVER my_driver[RRC_MAX_THREADS];
extern thread_arg_t thd_arg[RRC_MAX_THREADS];
extern U8 num_active_threads;
extern U8 g_uecc_lthd_id;
extern void * thread_func(void * thread_arg);
extern const  QMANIFEST RrcUeccEntity1;
extern const  QMANIFEST LoggerEntity;

static S32 closesocket(S32 s)
{
	return close_wrapper(s);
}


/*
 *   The `Listener' Structure
 */
typedef struct _listener_t
{
	SOCKET  s;
} listener_t;


/* Functions */
static S32 get_sys_error(void)
{
	return errno;
}

/******************************************************************************
 *   FUNCTION NAME: rrc_create_logger_thread
 *
 *       This function creates LOGGER thread for reading the logs from shared 
 *       memory.
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
rrc_return_et rrc_create_logger_thread
(
 rrc_shared_mem_config_t *p_shared_mem_cfg,
 U16                       health_monitor_duraion
 )
{
	rrc_logging_config_t *rrc_logging_config = RRC_NULL;
	cpu_set_t cpuSetThrd;
/*SPR 22554 Fix Start*/
	U8  threadName[MAX_THREAD_NAME_LEN] = "rrc_logger";
/*SPR 22554 Fix End*/
	rrc_logging_config =(rrc_logging_config_t *) rrc_mem_get(sizeof(rrc_logging_config_t));
	if(RRC_NULL == rrc_logging_config)
	{
		RRC_TRACE(RRC_ERROR, "Mem get failed for rrc_logging_config_t");
		return RRC_FAILURE;
	}
	memset_wrapper( rrc_logging_config, 0, sizeof(rrc_logging_config_t));
	rrc_logging_config->int_log_read_duration = health_monitor_duraion;
	l3_memcpy_wrapper(&rrc_logging_config->shared_mem_cfg,
			p_shared_mem_cfg,
			sizeof(rrc_shared_mem_config_t));

	qvRegister(my_driver[RRC_MAX_THREADS-3], 0, 0, &LoggerEntity, 
			(void*)rrc_logging_config);
	thd_arg[num_active_threads].driver_id = my_driver[RRC_MAX_THREADS-3];
	thd_arg[num_active_threads].p_qwait = (QWAIT*)waitstruct;
	thd_arg[num_active_threads].logical_id = num_active_threads;
	/* creating Driver loop for Logger Thread */
	if (pthread_create(&thd_arg[num_active_threads].tid, 0, thread_func, 
				(void *)&thd_arg[num_active_threads]))
	{
		RRC_TRACE(RRC_WARNING, "Unable to create thread for Logger");
		return RRC_FAILURE;
	}
/*SPR 22554 Fix Start*/
    	/* Set name of the rrc logger thread */
    	if (0 != pthread_setname_np_wrapper(&thd_arg[num_active_threads].tid, threadName))
    	{
        	RRC_TRACE( RRC_WARNING, "Failed to set name of the RRC logger thread \n");
    	}

/*SPR 22554 Fix End*/
	if ((p_shared_mem_cfg->bitmask & RRC_OAM_SHARED_MEM_CORE_NUMBER_PRESENT) &&
			(p_shared_mem_cfg->core_no != RRC_INVALID_CORE_NUM))
	{
		/* Binding Core to each UECC Thread */
		CPU_ZERO(&cpuSetThrd);
		CPU_SET (p_shared_mem_cfg->core_no ,&cpuSetThrd);
		if (pthread_setaffinity_np(thd_arg[num_active_threads].tid, sizeof(cpu_set_t),
					&cpuSetThrd))
		{
			RRC_TRACE(RRC_WARNING, "pthread_setaffinity_np Returned Error"
					"for thread %d", thd_arg[num_active_threads].logical_id);
			return RRC_FAILURE;
		}
		else
		{
			CPU_ZERO(&cpuSetThrd);
			if (0 == pthread_getaffinity_np(thd_arg[num_active_threads].tid, 
						sizeof(cpu_set_t), &cpuSetThrd))
			{
                /* SPR_17377_Warning_fix + */
                               RRC_TRACE(RRC_INFO, "Core Binding is successful for thread %d",
                               thd_arg[num_active_threads].logical_id);
                /* SPR_17377_Warning_fix - */
			}
		}
	}
	num_active_threads++;
	return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: rrc_create_uecc_instances
 *
 *       This function creates threads for UECC instances
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
rrc_return_et rrc_create_uecc_instances
(
 U8              num_of_uecc_instances, /* Number of UECC instances to be created */
 U8              rest_uecc_core_num,    /* Core Number for RRC modules except UECC */
 U8              *core_number           /* Array of Core Numbers */
 )
{
	U8        uecc_index = RRC_NULL;
/*SPR 21653 changes start*/
	U8        uecc1_module_id = RRC_NULL;
/*SPR 21653 changes end*/
	cpu_set_t cpuSetThrd[RRC_MAX_THREADS];
/*SPR 22554 Fix Start*/
	U8 threadName[MAX_THREAD_NAME_LEN] = "rrc_uecc";
/*SPR 22554 Fix End*/

	/* If Number of UECC instances is 1 and Core Number for UECC and rest of RRC
	 * moduleas are same then run a single thread and bind the thread on the available core */
	if ((1 == num_of_uecc_instances) && 
			(core_number[0] == rest_uecc_core_num))
	{
/*SPR 21653 changes start*/
		uecc1_module_id =  RRC_UECC_1_MODULE_ID; 
		qvRegister(my_driver[0], 0, 0, &RrcUeccEntity1, &uecc1_module_id);
/*SPR 21653 changes end*/
		/* If Core Number provided by OAM is a valid number then bind to the
		 * requested core */
		if ( core_number[0] != RRC_INVALID_CORE_NUM)
		{
			CPU_ZERO(&cpuSetThrd[0]);
			CPU_SET (core_number[0] ,&cpuSetThrd[0]);
			if (pthread_setaffinity_np(thd_arg[g_uecc_lthd_id].tid, sizeof(cpu_set_t), &cpuSetThrd[0]))
			{
				RRC_TRACE(RRC_WARNING, "pthread_setaffinity_np Returned Error in L3");
                return RRC_FAILURE;
			}
			else
			{
				CPU_ZERO(&cpuSetThrd[0]);
				if (0 == pthread_getaffinity_np(thd_arg[g_uecc_lthd_id].tid, sizeof(cpu_set_t), &cpuSetThrd[0]))
				{
                /* SPR_17377_Warning_fix + */
                                RRC_TRACE(RRC_INFO,"Core Binding is successful. Core Affinity ");
                /* SPR_17377_Warning_fix - */
				}
			}
		}
	}
	else
	{
		g_uecc_lthd_id = num_active_threads;
		for (uecc_index = 1; uecc_index <= num_of_uecc_instances && 
				num_active_threads < RRC_MAX_THREADS; uecc_index++ )
		{
			thd_arg[num_active_threads].driver_id = my_driver[uecc_index];
			thd_arg[num_active_threads].p_qwait = (QWAIT*)waitstruct;
			thd_arg[num_active_threads].logical_id = num_active_threads;
			/* creating Driver loop for UECC Threads */
			if (pthread_create(&thd_arg[num_active_threads].tid, 0, thread_func, 
						(void *)&thd_arg[num_active_threads]))
			{
				RRC_TRACE(RRC_WARNING, "Unable to create thread for UECC Instance");
				return RRC_FAILURE;
			}
/*SPR 22554 Fix Start*/
    			snprintf((char *)threadName, MAX_THREAD_NAME_LEN, "rrc_uecc_%d",uecc_index);
    			/* Set name of the uecc thread */
    			if (0 != pthread_setname_np_wrapper(&thd_arg[num_active_threads].tid, threadName))
    			{
        			RRC_TRACE( RRC_WARNING, "Failed to set name of the uecc[%d] thread \n",uecc_index);
    			}
/*SPR 22554 Fix End*/

			/* If Core Number provided by OAM is a valid number then bind to the
			 * requested core */
			if ( core_number[uecc_index-1] != RRC_INVALID_CORE_NUM)
			{
				/* Binding Core to each UECC Thread */
				CPU_ZERO(&cpuSetThrd[uecc_index - 1]);
				CPU_SET (core_number[uecc_index - 1] ,&cpuSetThrd[uecc_index - 1]);
				if (pthread_setaffinity_np(thd_arg[num_active_threads].tid, sizeof(cpu_set_t),
							&cpuSetThrd[uecc_index - 1]))
				{
					RRC_TRACE(RRC_WARNING, "pthread_setaffinity_np Returned Error"
							"for thread %d", uecc_index);
					return RRC_FAILURE;
				}
				else
				{
					CPU_ZERO(&cpuSetThrd[uecc_index - 1]);
					if (0 == pthread_getaffinity_np(thd_arg[num_active_threads].tid, 
								sizeof(cpu_set_t), &cpuSetThrd[uecc_index - 1]))
					{
                /* SPR_17377_Warning_fix + */
                                            RRC_TRACE(RRC_INFO,"Core Binding is successful for thread"
                                            " %d",
                                            uecc_index);
                /* SPR_17377_Warning_fix + */
					}
				}
			}
			num_active_threads++;
		}
		/* Assign the core number to the thread on which all
		   the rest of modules are running */
		if ( rest_uecc_core_num != RRC_INVALID_CORE_NUM )
		{
			CPU_ZERO(&cpuSetThrd[num_of_uecc_instances]);
			CPU_SET (rest_uecc_core_num ,&cpuSetThrd[num_of_uecc_instances]);
			if (pthread_setaffinity_np(thd_arg[num_active_threads-1].tid, sizeof(cpu_set_t),
						&cpuSetThrd[num_of_uecc_instances]))
			{
				RRC_TRACE(RRC_WARNING, "pthread_setaffinity_np Returned Error"
						"for thread 0");
                return RRC_FAILURE;
			}
			else
			{
				CPU_ZERO(&cpuSetThrd[num_of_uecc_instances]);
				if (0 == pthread_getaffinity_np(thd_arg[num_active_threads-1].tid, 
							sizeof(cpu_set_t), &cpuSetThrd[num_of_uecc_instances]))
				{
                /* SPR_17377_Warning_fix + */
                                       RRC_TRACE(RRC_INFO,"Core Binding is successful for thread");
                /* SPR_17377_Warning_fix + */
				}
			}
		}

	}

	return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: rrc_destroy_uecc_instances
 *
 *       This function cancel threads created for UECC instances
 *
 *   RETURNS:
 *       None
 *
 ******************************************************************************/
rrc_return_et rrc_destroy_uecc_instances
(
 U8              num_of_uecc_instances /* Number of UECC instances to be created */
 )
{
	U8 uecc_index = RRC_NULL;
	U8 count = g_uecc_lthd_id;

	for (uecc_index = 1; uecc_index <= num_of_uecc_instances &&
			count < RRC_MAX_THREADS; uecc_index++,count++ )
	{
		if ( 0 < thd_arg[count].tid )
		{
			if (pthread_cancel_wrapper(thd_arg[count].tid))
			{
				RRC_TRACE(RRC_WARNING, "Unable to destroy thread for UECC Instance");
				return RRC_FAILURE;
			}
			num_active_threads--;
			memset_wrapper(&thd_arg[count],0,sizeof(thread_arg_t));
		}
	}
	g_uecc_lthd_id = 0;

	return RRC_SUCCESS;
}

static void makeaddress(const S8 *p_str_full_address, struct sockaddr_in *p_address, unsigned long defaultaddress)
{
	const S8 *p_str_port_start = (const S8*)strchr_wrapper((const S8*)p_str_full_address, ':');
	S8 str_inet_addr[MAX_INET_ADDRESS_LENGTH];

	U32 inet_addr_length = 0;

	memset_wrapper(p_address, 0, sizeof(*p_address));
	p_address->sin_family = AF_INET;

	RRC_ASSERT(PNULL != p_str_full_address);

	if (NULL != p_str_port_start)
	{
		inet_addr_length = (U32)(p_str_port_start - p_str_full_address);

		++p_str_port_start;

		if (inet_addr_length >= sizeof(str_inet_addr) - 1)
		{ 
            RRC_DRV_TRACE( RRC_ERROR, 
                    "Too long address - %u. "
                    "Should be up to %d", inet_addr_length, 
                    sizeof(str_inet_addr) - 1 );
			p_address->sin_addr.s_addr = htonl_wrapper(INADDR_NONE);
		}
		else
		{
			l3_memcpy_wrapper(str_inet_addr, p_str_full_address, inet_addr_length);
			str_inet_addr[inet_addr_length] = '\0';
			p_address->sin_addr.s_addr = inet_addr_wrapper((const char *)(str_inet_addr));
		}
	}
	else
	{
		p_str_port_start = p_str_full_address;
		p_address->sin_addr.s_addr = defaultaddress;
	}

	p_address->sin_port = htons_wrapper((U16)(atoi_wrapper((const U8*)(p_str_port_start))));
}

static S32 makeserver(const struct sockaddr_in *p_address)
{
	SOCKET s = RRC_NULL;

	RRC_ASSERT(PNULL != p_address);

	RRC_DRV_TRACE(RRC_INFO, "Make server on address %u.%u.%u.%u:%hu",
			YGETBYTE(p_address->sin_addr.s_addr, 3),
			YGETBYTE(p_address->sin_addr.s_addr, 2),
			YGETBYTE(p_address->sin_addr.s_addr, 1),
			YGETBYTE(p_address->sin_addr.s_addr, 0),
			ntohs_wrapper(p_address->sin_port));

	s = socket_wrapper(PF_INET, SOCK_DGRAM, 0);
	if (INVALID_SOCKET == s)
	{
		RRC_DRV_TRACE(RRC_FATAL, "Couldn't create socket. System error code %d", get_sys_error());
	}
	else
	{
		if (SOCKET_ERROR == bind_wrapper(s, (SockAddr *)p_address, sizeof(*p_address)))
		{
			RRC_DRV_TRACE(RRC_FATAL, "Can't bind socket. System error code %d", get_sys_error());
			closesocket(s);
			s = INVALID_SOCKET;
		}
	}

	return s;
}



static U16 rrc_get_word_from_header(U8 *p_header)
{
	return (U16)((p_header[0] << 8) | p_header[1]);
}

#ifdef RRC_UT_FRAMEWORK 

static void *s_receive(void *S, QMODULE *sender, QMODULE *receiver, signed char *priority)
{
    return NULL;
}

#else

/* using Signed char instead of S8 for priority field to remove warning */
static void *s_receive(void *S, QMODULE *sender, QMODULE *receiver, signed char *priority)
{
	listener_t      *p_l = (listener_t *)S;
	struct timeval  zero;
	fd_set          fd;
	S32             bytes_peeked = RRC_NULL;
	S32             peeked_bytes = RRC_NULL;
	S32             bytes_read = RRC_NULL;
	U8              header[RRC_INTERFACE_API_HEADER_SIZE];
	U16             src_id = RRC_NULL, api_id = RRC_NULL, msg_size = RRC_NULL, dst_id = RRC_NULL;
	U16             int_dst_id = RRC_NULL;
	S8            *msg_buf = PNULL;
	U16             buf_length = RRC_NULL;
	/* coverity_fix_62084_start */
	if(PNULL == p_l)
	{
		RRC_TRACE(RRC_FATAL, "s_receive: p_l contains NULL");
		return 0;
	}
	/* coverity_fix_62084_stop */
	if(p_l->s < 0)
	{
		RRC_TRACE(RRC_FATAL, "s_receive: Invalid file Descriptor");
		ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
		return 0;
	}

	/* Check socket */
	lteFD_ZERO(&fd);

	/* SPR 2792 Start */
#if 0
#ifdef ARICENT_SCTP_IPR
	/* Getting FD SET used by SCTP STACK */
	if (RRC_TRUE == is_sctp_stack_init())
	{
		sig_get_glb_fd_set(&fd);
	}
#endif
#endif	

#ifdef LKSCTP
	/* Getting FD SET for SCTP*/
	fd = sctp_fd;
#endif

	/* SPR 2792 Stop */


	lteFD_SET((S32)p_l->s, &fd);

	zero.tv_sec = 0;
	zero.tv_usec = 10000;/*L3-X2AP Drop2 Integration */

	if (SOCKET_ERROR == select_wrapper(FD_SETSIZE - 1, &fd, 0, 0, &zero))
	{
		RRC_DRV_TRACE(RRC_FATAL, "Problem with select in s_receive. System error code %d", get_sys_error());
		ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
		return 0;
	}
	if (!lteFD_ISSET((S32)p_l->s, &fd))
	{
		/* There are no data for reading on UDP port
		   Checking data on SCTP sockets */
#ifndef RRC_UNIT_TEST_FRAMEWORK
		return l3_sctp_receive(sender, receiver,(S8*)priority);
#endif
	}

	/* Peek interface message header in order to determine message size */
	memset_wrapper(header, 0, sizeof(header));
    {
	bytes_peeked = (S32)recvfrom_wrapper(p_l->s, header, sizeof(header), MSG_PEEK, 0, 0);

	peeked_bytes = bytes_peeked;
	if (SOCKET_ERROR == peeked_bytes)
	{
		{
			RRC_DRV_TRACE(RRC_FATAL, "We couldn't peek incomming message. System error code %d", get_sys_error());
			ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
			return 0;
		}
	}


	if (bytes_peeked < (S32)sizeof(header))
	{
		RRC_DRV_TRACE(RRC_FATAL, "Too short message - ignored");
		ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
		/* Discard whole bad massage and exit */
       /*coverity_108937_start*/
		bytes_read = recvfrom_wrapper(p_l->s, header, sizeof(header), 0, 0, 0);
        if (SOCKET_ERROR == bytes_read)
       {
          RRC_DRV_TRACE(RRC_FATAL, "We have problem with recvfrom. System error code %d", get_sys_error());
          ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
       }
       /*coverity_108937_end*/
       return 0;
	}
   }
	/* Parse header */
	src_id      = rrc_get_word_from_header(header + 2);
	dst_id      = rrc_get_word_from_header(header + 4);
	api_id      = rrc_get_word_from_header(header + 6);
	msg_size    = rrc_get_word_from_header(header + 8);

	/* Check for the Destination module ID*/
	if (RRC_MODULE_ID != dst_id )    
	{
		RRC_DRV_TRACE(RRC_WARNING, "Unknown destination module ID: 0x%X. Message ignored", dst_id);
		/* Discard whole bad massage and exit */
        /*coverity_108937_start*/
        bytes_read =  recvfrom_wrapper(p_l->s, header, sizeof(header), 0, 0, 0);
        if (SOCKET_ERROR == bytes_read)
        {
          RRC_DRV_TRACE(RRC_FATAL, "We have problem with recvfrom. System error code %d", get_sys_error());
          ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
        }
       /*coverity_108937_end*/
       return 0;
	}

	/* Check source entity ID and determine destination internal module ID */
	switch(src_id)
	{
		case RRC_OAM_MODULE_ID:
#ifdef LTE_EMBMS_SUPPORTED
            if(api_id > M2AP_OAM_API_BASE)
            {
                int_dst_id = RRC_M2AP_MODULE_ID;
            }
            else if( api_id > S1AP_OAM_API_BASE)
#else
			if (api_id > S1AP_OAM_API_BASE)
#endif
			{
				int_dst_id = RRC_S1AP_MODULE_ID;
			}
			/* L3-X2AP Integration Activity */
			else if((api_id > X2AP_OAM_API_BASE) && (api_id < S1AP_OAM_API_BASE))  
			{
				int_dst_id = RRC_X2AP_MODULE_ID;
			}
			/* SPR-16334 START */
			else if((api_id >= X2AP_ENB_CONFIG_UPDATE_REQ) && (api_id <= X2AP_ENB_CONFIG_UPDATE_RES))
			{
				int_dst_id = RRC_X2AP_MODULE_ID;
			}
			/* SPR-16334 END */

			else
			{
				int_dst_id = RRC_OAMH_MODULE_ID;
			}
			break;
		case RRC_SON_MODULE_ID:
			{
				if((api_id >= S1AP_ADD_MME_REQ) && (api_id <= S1AP_ADD_MME_RES))
				{
					int_dst_id = RRC_S1AP_MODULE_ID;
				}
				else if((api_id >= X2AP_ADD_ENB_REQ) && (api_id <= X2AP_DEL_ENB_RES))
				{
					int_dst_id = RRC_X2AP_MODULE_ID;
				}
				else if((api_id >= X2AP_ENB_CONFIG_UPDATE_REQ) && (api_id <= X2AP_MOBILITY_SETTING_CHANGE_RESP))
				{
					int_dst_id = RRC_X2AP_MODULE_ID;
				}
			}
			break;
		case RRC_RRM_MODULE_ID:
			if ((api_id == RRC_RRM_CELL_SETUP_REQ) || (api_id == RRC_RRM_CELL_DELETE_REQ) ||
					(api_id == RRC_RRM_CELL_RECONFIGURE_REQ) || 
					(api_id == RRC_RRM_UPDATED_PWS_SI_LIST_RESP) ||
					(api_id == RRC_RRM_CELL_START_REQ) ||
					(api_id == RRC_RRM_CELL_STOP_REQ)) 

			{
				int_dst_id = RRC_CSC_MODULE_ID;
			}
#ifdef LTE_EMBMS_SUPPORTED
			/* SSIT Fix */
			else if ( (api_id ==  RRM_M2AP_MBMS_SF_INFO_RESP) )
            {
                int_dst_id = RRC_M2AP_MODULE_ID;
            }
            /* SPR 18736 fix start */
            else if (api_id == X2AP_RRM_UPDATED_MBMS_SF_INFO_IND)
            {
                int_dst_id = RRC_X2AP_MODULE_ID;
            }
            /* SPR 18736 fix end */
#endif
			else if ( (api_id >= S1AP_RRM_ENB_DIRECT_INFO_TRANSFER) &&
					(api_id <= S1AP_RRM_LPPA_ERROR_IND_REQ) )
			{
				int_dst_id = RRC_S1AP_MODULE_ID;
			}
			/*RSU RRM-X2AP API*/
			else if ((api_id >= X2AP_RRM_RSU_RRM_START_REQ) && (api_id <= X2AP_ENDC_CONFIG_UPDATE_WAIT_IND))
			{
				int_dst_id = RRC_X2AP_MODULE_ID;
			}

			else
			{
				int_dst_id = RRC_UECCMD_MODULE_ID;
			}
			break;

#ifdef LTE_EMBMS_SUPPORTED
		case RRC_PDCP_MODULE_ID:
		case RRC_PHY_MODULE_ID:
			int_dst_id = RRC_LLIM_MODULE_ID;
			break;
		case RRC_RLC_MODULE_ID:
            if (RRC_M2AP_RLC_AREA_CONFIG_RESP == api_id)
            {
                int_dst_id = RRC_M2AP_MODULE_ID;
            }
            else 
            {
                int_dst_id = RRC_LLIM_MODULE_ID;
            }
			break;
		case RRC_MAC_MODULE_ID:
            if ( (api_id >= RRC_M2AP_MAC_AREA_CONFIG_REQ) &&
                (api_id <= RRC_MAC_MAX_API))
            {
                int_dst_id = RRC_M2AP_MODULE_ID;
            }
            else 
            {
                int_dst_id = RRC_LLIM_MODULE_ID;
            }
			break;
		case RRC_S1U_MODULE_ID:
            if ( (api_id >= RRC_M2AP_M1U_SESSION_START_CNF) &&
                 (api_id <= RRC_S1U_MAX_API))
            {
                int_dst_id = RRC_M2AP_MODULE_ID;
            }
            else 
            {
                int_dst_id = RRC_LLIM_MODULE_ID;
            }
			break;
#else
		case RRC_PDCP_MODULE_ID:
		case RRC_PHY_MODULE_ID:
		case RRC_RLC_MODULE_ID:
		case RRC_MAC_MODULE_ID:
		case RRC_S1U_MODULE_ID:
			int_dst_id = RRC_LLIM_MODULE_ID;
			break;

#endif
#ifdef MODE_PROFILER_DEFINED
		case RRC_WRONG_MODULE_ID:
			RRC_DRV_TRACE(RRC_WARNING, "Wrong source module ID: 0x%X. Message ignored", src_id);
			RRC_EXIT(0);
#endif
		default:
			RRC_DRV_TRACE(RRC_WARNING, "Unknown source module ID: 0x%X. Message ignored", src_id);
			/* Discard whole bad massage and exit */
         /*coverity_108937_start*/
           bytes_read =  recvfrom_wrapper(p_l->s, header, sizeof(header), 0, 0, 0);
           if (SOCKET_ERROR == bytes_read)
          {
             RRC_DRV_TRACE(RRC_FATAL, "We have problem with recvfrom. System error code %d", get_sys_error());
             ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
         }
       /*coverity_108937_end*/
       return 0;
	}
/* coverity_64239 start */
	/* We have only peeked the message, so re-read the interface header again plus allocate additional space for RRC API header */
	buf_length = ((U16)RRC_API_HEADER_SIZE + msg_size);
/* coverity_64239 stop */

/* coverity_64239 start */
	/* Check on U16 overflow */
	if ((buf_length < msg_size) || (buf_length < ((U16)RRC_API_HEADER_SIZE + (U16)sizeof(header))))
/* coverity_64239 stop */
	{
		RRC_DRV_TRACE(RRC_FATAL, "Too long message - ignored");
		ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
		/* Discard whole bad massage and exit */
        /*coverity_108937_start*/
        bytes_read =  recvfrom_wrapper(p_l->s, header, sizeof(header), 0, 0, 0);
        if (SOCKET_ERROR == bytes_read)
        {
          RRC_DRV_TRACE(RRC_FATAL, "We have problem with recvfrom. System error code %d", get_sys_error());
          ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
        }
       /*coverity_108937_end*/
		return 0;
	}

	/* Allocate memory for message */
	msg_buf = rrc_msg_mem_get(buf_length);
	if (msg_buf == PNULL)
	{
		/* Not enough memory */
		return 0;
	}
	bytes_read = recvfrom_wrapper(p_l->s, msg_buf + RRC_API_HEADER_SIZE, buf_length - RRC_API_HEADER_SIZE, 0, 0, 0);
	if (SOCKET_ERROR == bytes_read)
	{
		RRC_DRV_TRACE(RRC_FATAL, "We have problem with recvfrom. System error code %d", get_sys_error());
		ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
		rrc_msg_mem_free(msg_buf);
		return 0;
	}

	if (bytes_read != buf_length - RRC_API_HEADER_SIZE)
	{
		RRC_DRV_TRACE(RRC_FATAL, 
				"Couldn't read whole message - ignored, Bytes read : [%d], Length Recvd : [%d]",
				bytes_read,buf_length);
		ALARM_MSG_L3(RRC_MODULE_ID, RECV_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
		rrc_msg_mem_free(msg_buf);
		return 0;
	}

	/* And finally fill the RRC API header */
	rrc_construct_api_header((U8 *)msg_buf, RRC_VERSION_ID, src_id, int_dst_id, api_id, buf_length);

	*sender     = qvGetService(src_id);
	*receiver   = qvGetService(int_dst_id);
	*priority   = 0;

	RRC_ASSERT(0 != *sender);
	RRC_ASSERT(0 != *receiver);
#ifdef LATENCY_TRACE_ON
	struct timespec  tp;
	struct tm      *tm;

	clock_gettime_wrapper(CLOCK_PROCESS_CPUTIME_ID, &tp);
	tm = localtime_wrapper(&tp.tv_sec);

	if (api_id == 100)
	{
		memset_wrapper(g_latency_data, 0, (200 * sizeof(g_latency_data_t)));
		start_logging = 1;
	}
	if (start_logging == 1)
	{
		g_latency_data[g_index].send_rcv_flag = RRC_RECEIVE;
		g_latency_data[g_index].src_mod_id = src_id;
		g_latency_data[g_index].dest_mod_id = int_dst_id;
		g_latency_data[g_index].api_id = api_id;
		l3_memcpy_wrapper(&g_latency_data[g_index].val_time, tm, 
				sizeof(g_latency_data[g_index].val_time)); 
		g_latency_data[g_index].time_u_secs = tp.tv_nsec; 
		g_index++;
	}
#endif


	return msg_buf;
}
#endif

static void s_send(void *S, const void *message, QMODULE sender, QMODULE receiver, 
		void *p_str_dest_full_address, unsigned int size)
{
	listener_t          *p_l = (listener_t *)S;

	struct sockaddr_in  to;

	U32        body_size       = (U32)(size - RRC_API_HEADER_SIZE);
	U32        payload_size    = qvMsgSize(message, 0);
	S32                 buf_length      = (S32)(body_size + payload_size);
	S32                 bytes_sent      = RRC_NULL;

	S8                *msg_buf        = PNULL;

	/* Please make sure that nobody change string p_str_dest_full_address during it processing */
	RRC_ASSERT(PNULL != p_str_dest_full_address);

	if(p_l->s < 0)
	{
		RRC_TRACE(RRC_FATAL, "s_send: Invalid file Descriptor");
		ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
		return;
	}
	makeaddress((const S8 *)p_str_dest_full_address, &to, htonl_wrapper(INADDR_LOOPBACK));
	if (htonl_wrapper(INADDR_NONE) == to.sin_addr.s_addr)
	{
		RRC_DRV_TRACE(RRC_FATAL, "Couldn't convert destination address to sockaddr_in");
		ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
		return;
	}

	/* Allocate buffer for outgoing message */
	msg_buf = (S8 *)rrc_mem_get((rrc_size_t)buf_length);
	if (PNULL == msg_buf)
	{
		/* Not enough memory */
		return;
	}
	/*Coverity_64272_fix_start*/
	/* valgrind fix start */  
	memset_wrapper(msg_buf, 0,((rrc_size_t)buf_length));
	/* valgrind fix stop */  
	/*Coverity_64272_fix_stop*/

	/* Copy body without RRC header */
	l3_memcpy_wrapper(msg_buf, (const U8 *)message + RRC_API_HEADER_SIZE, body_size);
	/* Copy payload */
	qvMsgExtract(message, 0, (U8 *)msg_buf + body_size, payload_size);

	/* RRC send message - update destination in interface header to RRC_MODULE_ID */

	msg_buf[2] = (RRC_MODULE_ID & 0xFF00) >> 8;
	msg_buf[3] = RRC_MODULE_ID & 0x00FF;

	RRC_DRV_TRACE(RRC_DETAILED, "Sending message on addres %u.%u.%u.%u:%hu. Sender: 0x%lX -> reciver: 0x%lX.",
			YGETBYTE(to.sin_addr.s_addr, 3),
			YGETBYTE(to.sin_addr.s_addr, 2),
			YGETBYTE(to.sin_addr.s_addr, 1),
			YGETBYTE(to.sin_addr.s_addr, 0),
			ntohs_wrapper(to.sin_port),
			qvGetServiceName(sender),
			qvGetServiceName(receiver));
    {
        bytes_sent = sendto_wrapper(p_l->s, msg_buf, (size_t)buf_length, 0, (SockAddr *)&to, sizeof(to));
        if (SOCKET_ERROR == bytes_sent)
        {
            RRC_DRV_TRACE(RRC_FATAL, "We have problem with sendto. System error code %d", get_sys_error());
            ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_PARAMTER_ERR_CAUSE_ID);
        }
        else if (bytes_sent != buf_length)
        {
            RRC_DRV_TRACE(RRC_FATAL, "Couldn't send whole message");
            ALARM_MSG_L3(RRC_MODULE_ID, SEND_MSG_FAILED_ALARM_ID,CRITICAL_ALARM,IPC_INVALID_HANDLE_ERR_CAUSE_ID);
        }

        if (PNULL != msg_buf)
        {
            rrc_mem_free(msg_buf);
        }
    }
}

static void *s_open(void *p_str_rrc_full_address)
{
	listener_t *p_l = PNULL;
	struct sockaddr_in address;

	RRC_DRV_TRACE(RRC_INFO, "Initialization of CMES driver");

	RRC_ASSERT(PNULL != p_str_rrc_full_address);

	makeaddress((const S8 *)p_str_rrc_full_address, &address, htonl_wrapper(INADDR_ANY));
	if (htonl_wrapper(INADDR_NONE) != address.sin_addr.s_addr)
	{
		/* Address is OK */
		p_l = (listener_t *)rrc_sys_mem_get(sizeof(listener_t));
		if (PNULL != p_l)
		{
			/* We allocate memory for listener_t */
			p_l->s = makeserver(&address);
			if (INVALID_SOCKET == p_l->s)
			{
				/* Get problems with socket */
				rrc_sys_mem_free(p_l);
				p_l = PNULL;
			}
		}
	}
	else
	{
		RRC_DRV_TRACE(RRC_FATAL, "Couldn't convert server address to sockaddr_in");
	}

	return p_l;
}

static void s_close(void *data)
{
	listener_t *p_l = (listener_t *)data;

	if (PNULL != p_l)
	{
		closesocket(p_l->s);

		rrc_sys_mem_free(p_l);
	}
}

const QSHELL __operations =
{
	PNULL,
	s_send,
	s_receive,
	s_open,
	s_close,
	NULL
};

const QSHELL *operations = &__operations;

#ifdef LINUX_PC_TEST
extern short test_log(U16 usModId, U16 logLevel, char* fmt, ...);
extern U8 qvDeliverForL3Test(QMODULE from, QMODULE to, void *message);
U8 for_adapter_send_data_to_queue(U16 ulSrcModId, U16 ulDstModId, U16 ulMsgId, char *msg, U32 ulMsgLen)
{
    U8  *pucMsgBuf = PNULL;
    U8  ret = 0;
    QMODULE pstSrcModule = qvGetService(ulSrcModId);
    QMODULE pstDstModule = qvGetService(ulDstModId);
    
    test_log(RRC_MODULE_ID, 1, "%s: SRC=%ld[%ld], DST=%ld[%ld], MSG=%d, LEN=%d", 
        __FUNCTION__, ulSrcModId, qvGetServiceName(pstSrcModule), 
        ulDstModId, qvGetServiceName(pstDstModule), ulMsgId, ulMsgLen);
        
    pucMsgBuf = rrc_msg_mem_get(ulMsgLen);
    if (pucMsgBuf == PNULL)
    {
        test_log(RRC_MODULE_ID, 2, "%s: rrc_msg_mem_get failure!!!!!");
        return 1;
    }

    l3_memcpy_wrapper(pucMsgBuf, msg, ulMsgLen);

    ret = qvDeliverForL3Test(pstSrcModule, pstDstModule, pucMsgBuf);
    test_log(RRC_MODULE_ID, 1, "%s: qvDeliverForL3Test RET=%d", __FUNCTION__, ret); 

    return 0;
}
#endif

