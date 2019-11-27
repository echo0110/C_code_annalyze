/*! \file rrm_utils.h
 *  \brief This file contains the enums, struct, constants definitions for RRM APPS.
 *  \date January 20, 2012
 *  \author gur21481
  */
/*!

*  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}

*/

#ifndef _RRM_UTILS_H_
#define _RRM_UTILS_H_

#ifdef _MSC_VER
#include "pc_test.h"
#else
/*
* Header File Includes
*/

/*! \headerfile cspl.h <>
 */
#include <cspl.h>

/*! \headerfile stacklayer.h <>
 */
#include <stacklayer.h>

/*! \headerfile string.h <>
 */
#include <string.h>

/*! \headerfile mqueue.h <>
 */
#include    <mqueue.h>
 
/*! \headerfile math.h <>
 */
#include    <math.h>
/*! \headerfile tgmath.h <>
 */
#include    <tgmath.h>
/*! \headerfile sys/timeb.h <>
 */
#include <sys/timeb.h>
/*! \headerfile rrm_defines.h <>
 */
#include <rrm_defines.h>

/*! \headerfile lteLogger.h <>
 */
#include <lteLogger.h>

/*! \headerfile rrm_oam_defines.h <>
 */
#include <rrm_oam_defines.h>
/*! \headerfile sched.h <>
 */

#include <sched.h>
#ifndef OAM_IPR
#include "rrm_l2_intf.h"
#endif

/* Carrier Aggregation start*/
#include "rrm_cell_rrmim_intf.h"
/* Carrier Aggregation end */


void set_cell_and_ue_index(U8 cell_index, U16 ue_index);
void set_cell_index(U8 cell_index);
void set_ue_index(U16 ue_index);
U8   get_cell_index(void);
U16  get_ue_index(void);

/* SPR 20636 Changes Start */
#ifdef __x86_64__
#define U32_U32Long_Bit U32Long
#else
#define U32_U32Long_Bit U32
#endif
/* SPR 20636 Changes End */

#define GET_CELL_INDEX()      get_cell_index()
#define GET_UE_INDEX()        get_ue_index()
#define SET_CELL_INDEX(value) set_cell_index(value)
#define SET_UE_INDEX(value)   set_ue_index(value)
#define SET_CELL_AND_UE_INDEX(cell_index, ue_index) set_cell_and_ue_index(cell_index, ue_index)

void rrm_abort(void);
/*! \fn rrm_void_t rrm_msg_mem_get(size_t size)
 *  \brief It returns memory buffer from memory pool.
 *  \param size     Size of buffer which will be allocated
 */

rrm_void_t *
rrm_msg_mem_get
(
    size_t size    
);

/*SPR 15436 start*/
void
rrm_quick_sort
(
	void *p_base,
	size_t num,
	size_t size,
	int (*p_comp_func)(const void *,const void *)
);
/*SPR 15436 End*/
/* Platform-dependent defines */
/*! \def RRM_MEMSET
*/
#define RRM_MEMSET 					memset
/* ! \def RRM_MEMCPY
*/
#define RRM_MEMCPY 					memcpy
/* ! \def RRM_MEMCMP
*/
#define RRM_MEMCMP 					memcmp

#define RRM_MEMMOVE 			    memmove
/* ! \def RRM_STRNCMP(X,Y,Z)
*/
#define RRM_STRNCMP(X,Y,Z)    		strncmp((const char*)X,(const char*)Y,Z)
/* ! \def RRM_STRCAT(X,Y)
*/
#define RRM_STRCAT(X,Y)       		strcat((char*)X,(char*)Y)
/* ! \def RRM_STRNCAT(X,Y,Z)
*/
#define RRM_STRNCAT(X,Y,Z)    		strncat((char*)X,(const char*)Y,Z)
/* ! \def RRM_STRTOK(X,Y)
*/
#define RRM_STRTOK(X,Y)       		strtok((char*)X,(const char*)Y)
/* ! \def RRM_STRCPY(X,Y)
*/
#define RRM_STRCPY(X,Y)       		strcpy((char*)X,(const char*)Y)
/* ! \def RRM_STRNCPY(X,Y,Z)
*/
#define RRM_STRNCPY(X,Y,Z)    		strncpy((char*)X,(const char*)Y,Z)
/* ! \def RRM_STRLEN(X)
*/
#define RRM_STRLEN(X)         		strlen((const char*)X)
/* ! \def RRM_STRFTIME(A,B,C,D)
*/
#define RRM_STRFTIME(A,B,C,D) 		strftime((char*)A,B,C,D)
/* ! \def RRM_SPRINTF(X,Y,Z)
*/
#define RRM_SPRINTF(X,Y,Z)    		sprintf((char*)X,Y,Z)
/* ! \def RRM_SNPRINTF(A,B,C,D)
*/
#define RRM_SNPRINTF(A,B,C,D) 		snprintf((char*)A,B,C,D)
/* ! \def RRM_QVLOGVA(X,Y,Z)
*/
#define RRM_QVLOGVA(X,Y,Z)    		qvLogVa(X,(const char*)Y,Z)
/* ! \def  RRM_PUTS(X)
*/
#define RRM_PUTS(X)           		puts((const char*)X)
/* ! \def RRM_FOPEN(X,Y)
*/
#define RRM_FOPEN(X,Y)        		fopen((const char*)X,(const char*)Y)
/* ! \def RRM_STRCMP(X,Y)
*/
#define RRM_STRCMP(X,Y)    			strcmp((char*)X,(char*)Y)
/* Start Porting Activity */
/* ! \def RRM_FPRINTF
*/
#define RRM_FPRINTF         		fprintf

/* ! \def RRM_PRINTF
*/
#define RRM_PRINTF         		    printf

/* ! \def RRM_FLOOR
*/
#define RRM_FLOOR         		    floor

/* ! \def RRM_FLOORF
*/
#define RRM_FLOORF         		    floorf

/* ! \def RRM_CEIL
*/
#define RRM_CEIL         		    ceil

/* ! \def RRM_CEILF
*/
#define RRM_CEILF        		    ceilf

/* ! \def RRM_POW
*/
#define RRM_POW         		    pow

/* ! \def RRM_LOG10
*/
#define RRM_LOG10         		    log10

/* ! \def RRM_GETTIMEOFDAY          
*/
#define RRM_GETTIMEOFDAY   		    gettimeofday

/* ! \def RRM_FFLUSH          
*/
#define RRM_FFLUSH       		    fflush

/* ! \def RRM_CPU_ZERO 
*/
#define RRM_CPU_ZERO     		    CPU_ZERO

/* ! \def RRM_CPU_SET 
*/
#define RRM_CPU_SET     		    CPU_SET

/* ! \def RRM_NTOHL                 
*/
#define RRM_NTOHL                   ntohl
#define RRM_HTONL                   htonl

/* ! \def RRM_NTOHS                 
*/
#define RRM_NTOHS                   ntohs

/* ! \def RRM_MATH_LOG            
*/
#define RRM_MATH_LOG                log

/* ! \def RRM_FGETS               
*/
#define RRM_FGETS                   fgets

/* ! \def RRM_EXIT               
*/
#define RRM_EXIT                    exit

/* ! \def RRM_VSNPRINTF
*/
#define RRM_VSNPRINTF         		vsnprintf

/* ! \def RRM_VA_START
*/
#define RRM_VA_START         		va_start

/* ! \def RRM_VA_END
*/
#define RRM_VA_END         		    va_end

/* ! \def RRM_ATOI
*/
#define RRM_ATOI                    atoi

/* ! \def RRM_FTIME
*/
#define RRM_FTIME                   ftime

/* ! \def RRM_TIME
*/
#define RRM_TIME                    time

/* ! \def RRM_LOCALTIME
*/
#define RRM_LOCALTIME               localtime

/* ! \def RRM_SHMGET
*/
#define RRM_SHMGET                  shmget

/* ! \def RRM_SCHED_SETAFFINITY
*/
#define RRM_SCHED_SETAFFINITY       sched_setaffinity
/* End Porting Activity */
#endif//end _MSC_VER

/** Added Macros for MIF */
#define RRM_MESSAGE_API_START       		6000
#define RRM_OAM_INIT_IND            		(RRM_MESSAGE_API_START + 1)
#define RRM_OAM_INIT_CNF            		(RRM_MESSAGE_API_START + 2)
#define RRM_OAM_SHUTDOWN_REQ        		(RRM_MESSAGE_API_START + 3)
#define RRM_OAM_LOG_ENABLE_DISABLE_REQ 		(RRM_MESSAGE_API_START + 4)
#define RRM_OAM_LOG_ENABLE_DISABLE_RESP 	(RRM_MESSAGE_API_START + 5)
#define RRM_OAM_SET_LOG_LEVEL_REQ   		(RRM_MESSAGE_API_START + 6)
#define RRM_OAM_SET_LOG_LEVEL_RESP  		(RRM_MESSAGE_API_START + 7)
#define RRM_OAM_CELL_CONFIG_REQ          	(RRM_MESSAGE_API_START + 8)
#define RRM_OAM_CELL_CONFIG_RESP       		(RRM_MESSAGE_API_START + 9)
#define RRM_OAM_CELL_RECONFIG_REQ   		(RRM_MESSAGE_API_START + 10)
#define RRM_OAM_CELL_RECONFIG_RESP     		(RRM_MESSAGE_API_START + 11)
#define RRM_OAM_CELL_DELETE_REQ     		(RRM_MESSAGE_API_START + 12)
#define RRM_OAM_CELL_DELETE_RESP    		(RRM_MESSAGE_API_START + 13)
#define RRM_OAM_GENERIC_RESP        		(RRM_MESSAGE_API_START + 14)
#define RRM_OAM_INIT_CONFIG_REQ     		(RRM_MESSAGE_API_START + 15)
#define RRM_OAM_INIT_CONFIG_RESP            (RRM_MESSAGE_API_START + 16)
#define RRM_OAM_CELL_START_REQ              (RRM_MESSAGE_API_START + 17)
#define RRM_OAM_CELL_STOP_REQ               (RRM_MESSAGE_API_START + 18)
#define RRM_OAM_RESUME_SERVICE_REQ          (RRM_MESSAGE_API_START + 19)
#define RRM_OAM_RESUME_SERVICE_RESP         (RRM_MESSAGE_API_START + 20)
#define RRM_OAM_CELL_START_RESP                  (RRM_MESSAGE_API_START + 21)
#define RRM_OAM_CELL_STOP_RESP                   (RRM_MESSAGE_API_START + 22)
//#define RRM_OAM_CELL_DELETE_RESP                 (RRM_MESSAGE_API_START + 23)
#define RRM_OAM_RAC_ENABLE_DISABLE_REQ      (RRM_MESSAGE_API_START + 24)
#define RRM_OAM_RAC_ENABLE_DISABLE_RESP     (RRM_MESSAGE_API_START + 25)
#define RRM_OAM_SHUTDOWN_RESP               (RRM_MESSAGE_API_START + 26)
#define RRM_OAM_CELL_CONTEXT_PRINT_REQ      (RRM_MESSAGE_API_START + 27)
/*STATIC ICCI START*/
#define RRM_OAM_CARRIER_FREQ_AND_DL_TX_POWER_REQ    (RRM_MESSAGE_API_START + 28)
#define RRM_OAM_CARRIER_FREQ_AND_DL_TX_POWER_RESP   (RRM_MESSAGE_API_START + 29)
/*STATIC ICCI ENDS*/
/*UE RELEASE FROM OAM STRAT*/
#define RRM_OAM_UE_RELEASE_REQ       (RRM_MESSAGE_API_START + 30)
/* Soft Lock APIs Start */
#define RRM_OAM_CELL_BLOCK_REQ       (RRM_MESSAGE_API_START + 33)
#define RRM_OAM_CELL_BLOCK_RESP      (RRM_MESSAGE_API_START + 34)
#define RRM_OAM_CELL_UNBLOCK_CMD     (RRM_MESSAGE_API_START + 35)
#define RRM_OAM_READY_FOR_CELL_BLOCK_IND (RRM_MESSAGE_API_START + 36)
/* Soft Lock APIs End */

#define RRM_OAM_CELL_ECN_CAPACITY_ENHANCE_REQ (RRM_MESSAGE_API_START + 37) 
#define RRM_OAM_CELL_ECN_CAPACITY_ENHANCE_RESP (RRM_MESSAGE_API_START + 38)

/*#define RRM_OAM_PROC_SUPERVISION_RESP   (RRM_MESSAGE_API_START + 39)*/
/*UE RELEASE FROM OAM ENDS */

#define RRM_OAM_CELL_UPDATE_REQ             (RRM_MESSAGE_API_START + 40)
#define RRM_OAM_CELL_UPDATE_RESP            (RRM_MESSAGE_API_START + 41)
#define RRM_OAM_GET_VER_ID_REQ              (RRM_MESSAGE_API_START + 42)
#define RRM_OAM_GET_VER_ID_RESP             (RRM_MESSAGE_API_START + 43)
/* Free API IDs : (RRM_MESSAGE_API_START + 44), (RRM_MESSAGE_API_START + 45) */
/* Event Notification API */
#define RRM_OAM_EVENT_NOTIFICATION          (RRM_MESSAGE_API_START + 46)
/* Free API IDs : (RRM_MESSAGE_API_START + 47), (RRM_MESSAGE_API_START + 48) */
#define RRM_OAM_LOAD_CONFIG_REQ             (RRM_MESSAGE_API_START + 49)
#define RRM_OAM_LOAD_CONFIG_RESP            (RRM_MESSAGE_API_START + 50)
#define RRM_OAM_LOAD_REPORT_IND             (RRM_MESSAGE_API_START + 51)
#define RRM_OAM_EVENT_CONFIG_REQ            (RRM_MESSAGE_API_START + 52)
#define RRM_OAM_EVENT_CONFIG_RESP           (RRM_MESSAGE_API_START + 53)
#define RRM_PLATFORM_LOAD_IND                (RRM_MESSAGE_API_START + 55)
#define RRM_OAM_CONFIG_KPI_REQ              (RRM_MESSAGE_API_START +56) 
#define RRM_OAM_CONFIG_KPI_RESP             (RRM_MESSAGE_API_START +57) 
#define RRM_OAM_GET_KPI_REQ                 (RRM_MESSAGE_API_START +58) 
#define RRM_OAM_GET_KPI_RESP                (RRM_MESSAGE_API_START +59) 
#define RRM_OAM_KPI_IND                     (RRM_MESSAGE_API_START +60) 
/* health monitoring changes start*/
#define RRM_OAM_CHK_HEALTH_REQ              (RRM_MESSAGE_API_START +61)
#define RRM_OAM_CHK_HEALTH_RESP             (RRM_MESSAGE_API_START +62)
/* health monitoring changes end */
/* Carrier_Aggregation_Start */
#define RRM_OAM_ENB_CONFIG_REQ              (RRM_MESSAGE_API_START +63)
#define RRM_OAM_ENB_CONFIG_RESP             (RRM_MESSAGE_API_START +64)
/* Carrier_Aggregation_End */
/* SPR_13117_FIX_START */
#define RRM_OAM_GET_LOG_LEVEL_REQ           (RRM_MESSAGE_API_START +65)
#define RRM_OAM_GET_LOG_LEVEL_RESP          (RRM_MESSAGE_API_START +66)
/* SPR_13117_FIX_END */
/*spr 22474 start*/
#define RRM_OAM_GET_DEBUG_INFO_REQ           (RRM_MESSAGE_API_START +67)
#define RRM_OAM_GET_DEBUG_INFO_RESP          (RRM_MESSAGE_API_START +68)
/*spr 22474 end*/


#define RRM_MESSAGE_API_END                 (RRM_MESSAGE_API_START + 100)


/* SPR_6490 FIX */
#define RRM_VER_ID            		        "LTE_RRM_VER_2.1.1" 
#define RRM_OAM_API_BASE            		RRM_MESSAGE_API_START
#define RRM_OAM_MAX_API             		RRM_MESSAGE_API_END
/********************************************************************
 * RMIF APIs
 *******************************************************************/
#define RMIF_CELL_API_REQ_BASE       		500
#define RMIF_CELL_API_RESP_BASE      		600
#define RMIF_UE_API_REQ_BASE         		700
#define RMIF_UE_API_RESP_BASE        		800
#define RMCM_UEM_API_REQ_BASE         		900
#define RMCM_UEM_API_RESP_BASE        		1000
/* Carrier_Aggregation_Start */
#define RMCM_CELL_API_REQ_BASE              1100
#define RMCM_CELL_API_RESP_BASE             1200
/* Carrier_Aggregation_End */

/* UE MEAS CHANGES : STARTS */

/*! \def RRM_MIN_TRANSACTION_ID
 *  \brief Macro storing the value of minimum transaction Identifier
    generated by RRM
 */
#define RRM_MIN_TRANSACTION_ID  0

/*! \def RRM_MAX_TRANSACTION_ID
 *  \brief Macro storing the value of maximum transaction Identifier
    generated by RRM
 */
#define RRM_MAX_TRANSACTION_ID  0xFFFF
#define RRM_ROUNDING_FACTOR     0.5

#define RRM_SHIFT_SEVEN_BYTES    56
#define RRM_SHIFT_SIX_BYTES      48
#define RRM_SHIFT_FIVE_BYTES     40
#define RRM_SHIFT_FOUR_BYTES     32
#define RRM_SHIFT_THREE_BYTES    24
#define RRM_SHIFT_TWO_BYTES      16 
#define RRM_SHIFT_ONE_BYTE        8
#define RRM_SHIFT_ONE_NIBBLE      4  

/*! \def SPID_FOR_CDMA
 *  \valid range for spid for cdma  0 < spid_for_cdma_val < 254
 */
#define RRM_MAX_SPID_FOR_CDMA    253
#define RRM_MIN_SPID_FOR_CDMA      1
 /* fix 866 start */
#define RRM_MAX_S_MEAURE         97
 /* fix 866 end */


/* UE MEAS CHANGES : ENDS */
/* SPR 18145 Fix Start */
extern const S8* p_rrm_utils_facility_name;
extern const S8* p_rrm_cellm_facility_name;
extern const S8* p_rrm_mif_facility_name;
extern const S8* p_rrm_uem_facility_name;
extern const S8* p_rrm_event_facility_name;
extern const S8* p_rrm_g_cellm_facility_name;
/* SPR21623 Changes Start */
extern const S8 * UE_FSM_STATE[];
/* SPR21623 Changes end */
/* SPR 18145 Fix End */

/*! \enum rrm_cm_uem_message_req_e
 *  \brief This enum contains rrm cm uem message request params
 */
typedef enum _rrm_cm_uem_message_req_e
{
     RRMUEM_CM_DELETE_ALL_UE_CONTEXT_REQ = RMCM_UEM_API_REQ_BASE,
    /*SPR-695 Fix Starts*/
     RRMUEM_CM_DELETE_LOW_PRIORITY_UE_REQ,
    /*SPR-695 Fix Ends*/
     /*  DYNAMIC ICIC CHANGES START  */
     RRMUEM_CM_UPDATE_UE_INFO_REQ,
     /*  DYNAMIC ICIC CHANGES END  */
    /* eICIC_PHASE_1_2_CHANGES_START */
     RRMUEM_CM_GET_AGGRESSOR_REQ,        /* rrmuem_cm_get_aggressor_req_t */ 
    /* eICIC_PHASE_1_2_CHANGES_END */
    /** eICIC_PHASE_1_2_CHANGES_START */
     RRMUEM_CELLM_EICIC_PROVISION_RECONFIG_REQ,  /*rrmuem_cellm_eicic_provision_reconfig_req_t*/ 
/*Bug 831 Changes start*/
     RRMUEM_CELLM_EICIC_RESET_ABS_REQ
/*Bug 831 Changes End*/
    /** eICIC_PHASE_1_2_CHANGES_END */
}rrm_cm_uem_message_req_e;
/* ! \def enum rrm_mif_cellm_message_req_e
 *   \brief This enum contains the rrm mif celm message request params
*/
typedef enum _rrm_mif_cellm_message_req_e
{
	RRMCM_RMIF_LOG_ENABLE_DISABLE_REQ = RMIF_CELL_API_REQ_BASE,
	RRMCM_RMIF_SET_LOG_LEVEL_REQ,
	RRMCM_RMIF_INIT_CONFIG_REQ,
	RRMCM_RMIF_CELL_CONFIG_REQ,
	RRMCM_RMIF_CELL_RECONFIG_REQ,
	RRMCM_RMIF_CELL_DELETE_REQ,
	RRMCM_RMIF_CELL_START_REQ,
	RRMCM_RMIF_CELL_STOP_REQ,
    RRMCM_RMIF_CELL_UPDATE_REQ,
	RRMCM_RMIF_CELL_START_ADMISSION_REQ,
	RRMCM_RMIF_CELL_STOP_ADMISSION_REQ,
	RRMCM_RMIF_CELL_SET_ATTRIBUTE_FROM_ANR_REQ,
	RRMCM_RMIF_CELL_SET_ATTRIBUTE_FROM_ES_REQ,
	RRMCM_RMIF_CELL_REGISTER_FROM_ANR_REQ,
	RRMCM_RMIF_CELL_REGISTER_FROM_ES_REQ,
	RRMCM_RMIF_RAC_ENABLE_DISABLE_REQ,
	RRMCM_RMIF_CELL_DEREGISTER_FROM_ANR_REQ,
	RRMCM_RMIF_CELL_DEREGISTER_FROM_ES_REQ,
	RRMCM_RMIF_NMM_PREPARE_REQ,
	RRMCM_RMIF_NMM_COMPLETE_REQ,
	RRMCM_RMIF_CELL_CONTEXT_PRINT_REQ,
    RRMCM_RMIF_MEAS_CONFIG_FROM_ANR_REQ,
    RRMCM_RMIF_UPDATED_NRT_INFO_FROM_ANR_REQ,
    RRMCM_RMIF_TNL_DISCOVERY_FROM_ANR_REQ,
    RRMCM_RMIF_CELL_ECN_CAPACITY_REQ,
/* RRM_MAC_RECONF_SCHEDULER_CHANGES_START */
    RRMCM_RMIF_MAC_RECONF_SCHEDULE_IND,
/* RRM_MAC_RECONF_SCHEDULER_CHANGES_ENDS */
    /* Code for MLB start */
    RRMCM_RMIF_LOAD_CONFIG_REQ,
    RRMCM_RMIF_LOAD_CONFIG_RESP,
    RRMCM_RMIF_CELL_REGISTER_FROM_MLB_REQ,
    RRMCM_RMIF_CELL_DEREGISTER_FROM_MLB_REQ,
	RRMCM_RMIF_CELL_SET_ATTRIBUTE_FROM_MLB_REQ,
    RRMCM_RMIF_CELL_REGISTER_FROM_MRO_REQ,
    RRMCM_RMIF_CELL_DEREGISTER_FROM_MRO_REQ,
    /* MRO changes start */
    RRMCM_RMIF_CELL_SET_ATTRIBUTE_FROM_MRO_REQ,
    /* MRO changes end */
    RRMCM_RMIF_CELL_PLATFORM_IND,
    /* Code for MLB end */
    RRMCM_RMIF_TTT_UPDATE_IND_REQ,
    RRMCM_RMIF_CONFIG_KPI_REQ,
    RRMCM_RMIF_GET_KPI_REQ,
    RRMCM_RMIF_CELL_STOP_ADMISSION_REQ_FOR_CELL_BLOCK,
    RRMCM_RMIF_CELL_START_ADMISSION_REQ_FOR_CELL_UNBLOCK,
    RRMCM_RMIF_MEAS_CONFIG_OBJ_REMOVE_REQ,
    /* RACH_OPTIMIZATION_CHANGES_START */
    RRMCM_RMIF_RACH_INFO_IND,
    /* RACH_OPTIMIZATION_CHANGES_END */
    /* Carrier_Aggregation_Start */
    RRMCM_RMIF_ENB_CONFIG_REQ,
    /* Carrier_Aggregation_End */

    /* Spr 16142 Fix Start */
    RRMCM_RMIF_ENB_CONFIG_UPDATE_IND,
    /*SPR 15180 Fix Stop*/
    RRMCM_RMIF_STOP_TNL_DISCOVERY_FROM_ANR_REQ
    /* Spr 16142 Fix End */
}rrm_mif_cellm_message_req_e;
/* ! \def enum rrm_mif_cellm_message_resp_e
*   \brief This enum contains the rrm mif cellm message response params
*/
typedef enum _rrm_mif_cellm_message_resp_e
{
 	RRMCM_RMIF_LOG_ENABLE_DISABLE_RESP = RMIF_CELL_API_RESP_BASE,
 	RRMCM_RMIF_SET_LOG_LEVEL_RESP,
 	RRMCM_RMIF_INIT_CONFIG_RESP,
 	RRMCM_RMIF_CELL_CONFIG_RESP,   
 	RRMCM_RMIF_CELL_RECONFIG_RESP, 
 	RRMCM_RMIF_CELL_DELETE_RESP,   
 	RRMCM_RMIF_CELL_START_RESP,    
 	RRMCM_RMIF_CELL_STOP_RESP,   
    RRMCM_RMIF_CELL_UPDATE_RESP,  
 	RRMCM_RMIF_CELL_START_ADMISSION_RESP,
 	RRMCM_RMIF_CELL_STOP_ADMISSION_RESP,
 	RRMCM_RMIF_CELL_SET_ATTRIBUTE_RESP,
 	RRMCM_RMIF_CELL_REGISTER_RESP,
 	RRMCM_RMIF_CELL_DEREGISTER_RESP,
 	RRMCM_RMIF_GENERIC_RESP,
 	RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ANR_IND,
 	RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ANR_IND,
 	RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ANR_IND,
 	RRMCM_RMIF_CELL_STATE_CHANGE_FOR_ES_IND,
 	RRMCM_RMIF_CELL_ACTIVE_UE_COUNT_REPORT_FOR_ES_IND,
 	RRMCM_RMIF_CELL_ACTIVE_THRESHOLD_REACHED_REPORT_FOR_ES_IND,
	RRMCM_RMIF_RAC_ENABLE_DISABLE_RES,
 	RRMCM_RMIF_INIT_IND,
 	RRMCM_RMIF_NMM_PREPARE_RESP,
 	RRMCM_RMIF_NMM_COMPLETE_RESP,
 	RRMCM_RMIF_MEAS_CONFIG_RESP,
 	RRMCM_RMIF_UPDATED_NRT_INFO_FROM_ANR_RES,
    RRMCM_RMIF_TNL_DISCOVERY_FROM_ANR_RES,
    RRMCM_RMIF_CELL_ECN_CAPACITY_RESP,
    /* MLB Changes start */
    RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MLB_IND,
    /* MLB Changes end */
    RRMCM_RMIF_CELL_STATE_CHANGE_FOR_MRO_IND,
    RRMCM_RMIF_CONFIG_KPI_RESP,
    RRMCM_RMIF_GET_KPI_RESP,
    RRMCM_RMIF_KPI_IND,
    RRMCM_RMIF_MEAS_CONFIG_OBJ_REMOVE_RESP,
    /* RACH_OPTIMIZATION_CHANGES_START */
    RRMCM_RMIF_RACH_L2_REPORT,
    RRMCM_RMIF_CELL_STATE_CHANGE_FOR_RACH_OPTI,
    /* RACH_OPTIMIZATION_CHANGES_END */
    /* Carrier_Aggregation_Start */
    RRMCM_RMIF_ENB_CONFIG_RESP
    /* Carrier_Aggregation_End */
}rrm_mif_cellm_message_resp_e;
/* ! \def enum rrm_mif_uem_message_resp_e
 *   \brief This enum contains the rrm mif uem message response params
*/
typedef enum _rrm_mif_uem_message_resp_e
{
    RRMUEM_RMIF_INIT_IND = RMIF_UE_API_RESP_BASE,
    RRMUEM_RMIF_INIT_CONFIG_RES,
    RRMUEM_RMIF_EMERGENCY_CALL_ACTIVE_RES,
    RRMUEM_RMIF_SET_LOG_LEVEL_RES,
    RRMUEM_RMIF_LOG_ENABLE_DISABLE_RES,
    RRMUEM_RMIF_MEAS_CONFIG_RES,
    RRMUEM_RMIF_SNR_REPORT,
    RRMUEM_RMIF_MEAS_RESULTS_IND,
    RRMUEM_RMIF_HO_REPORT,
    RRMUEM_RMIF_NO_ACTIVE_UE_IND,
    RRMUEM_RMIF_NON_EMRGNCY_ACTIVE_CALLS_HO_RESP,
    RRMUEM_RMIF_EVENT_NOTIFICATION_IND,
    /* MRO changes start */
    RRMUEM_RMIF_HO_ATTEMPT_IND,
    RRMUEM_RMIF_HO_FAILURE_REPORT,
    /* MRO changes end */
    RRMUEM_RMIF_EVENT_CONFIG_RESP,
    /* RACH_OPTIMIZATION_CHANGES_START */
    RRMUEM_RMIF_RACH_REPORT,
    /* RACH_OPTIMIZATION_CHANGES_END */
    /* SPR_13117_FIX_START */
    RRMUEM_RMIF_GET_LOG_LEVEL_RESP
    /* SPR_13117_FIX_END */

}rrm_mif_uem_message_resp_e;

/* ! \def rrm_mif_uem_message_req_e
*   \brief This enum contains the rrm mif uem message request params
*/
typedef enum _rrm_mif_uem_message_req_e
{
    RRMUEM_RMIF_INIT_CONFIG_REQ = RMIF_UE_API_REQ_BASE,
    RRMUEM_RMIF_EMERGENCY_CALL_ACTIVE_REQ,
    RRMUEM_RMIF_SET_LOG_LEVEL_REQ,
    RRMUEM_RMIF_LOG_ENABLE_DISABLE_REQ,
    RRMUEM_RMIF_GET_SNR_REPORT,
    RRMUEM_RMIF_UE_REL_REQ,
    RRMUEM_RMIF_RELEASE_ALL_EXISTING_UE_FORCEFULLY_REQ,
    RRMUEM_RMIF_NON_EMRGNCY_ACTIVE_CALLS_HO_REQ,
    RRMUEM_RMIF_EVENT_CONFIG_REQ,
    /* RACH_OPTIMIZATION_CHANGES_START */
    RRMUEM_RMIF_RACH_INFO_IND,
    /* RACH_OPTIMIZATION_CHANGES_END */
    /* SPR_13117_FIX_START */
    RRMUEM_RMIF_GET_LOG_LEVEL_REQ
    /* SPR_13117_FIX_END */

}rrm_mif_uem_message_req_e;

/* ! \def rrm_cellm_uem_message_ind_e
*   \brief This enum contains the rrm celm uem message index 
*/
typedef enum _rrm_cellm_uem_message_ind_e
{
    RRMUEM_CM_SET_UNSET_ECN_IND,
    RRMUEM_CM_UNSET_ECN_FOR_ALL_UE_IND,
    RRMUEM_CM_REDIRECT_REL_IND,
    /* SPR 16406 Start */
    RRMUEM_CM_PRB_THRESHOLD_REACHED_IND,
    RRMUEM_CM_AWAITED_HO_TRIGGER_IND,
	/* SPR 16406 End */
    RRMUEM_CM_CELL_INFO_CHNANGE_IND,
    RRMUEM_CM_SCELL_DEACT_ON_LOAD,
    RRMUEM_CM_SCELL_DEACT_ON_CA_TO_NON_CA,
    /* eICIC_PHASE_1_2_CHANGES_START */
    RRMUEM_CM_SCHEDULE_VICTIM_UE_IND   /* rrmuem_cm_schedule_victim_ue_ind_t */
    /* eICIC_PHASE_1_2_CHANGES_END */
#ifdef LTE_EMBMS_SUPPORTED  
    ,RRMUEM_CM_MBMS_RECONFIG_UE_IND   /* rrmuem_cm_mbms_reconfig_ue_ind_t */
#endif
}rrm_cellm_uem_message_ind_e;
/* ! \def rrm_cm_uem_message_resp_e
 *   \brief This enum contains rrm cm uem message response params
*/
typedef enum _rrm_cm_uem_message_resp_e
{
    RRMUEM_CM_DELETE_ALL_UE_CONTEXT_RESP = RMCM_UEM_API_RESP_BASE,
    /*  DYNAMIC ICIC CHANGES START  */
    RRMUEM_CM_UPDATE_UE_INFO_RESP, 
    /*  DYNAMIC ICIC CHANGES END  */
    /* CA_Stage3_Change: Start */
    RRMUEM_CM_CELL_INFO_CHANGE_RESP,
    /* CA_Stage3_Change: End */
    /* BUG_11648_FIX_START */
    RRMUEM_CM_CELL_STOP_RESP,
    /* BUG_11648_FIX_END */
    /* eICIC_PHASE_1_2_CHANGES_START */
    RRMUEM_CM_GET_AGGRESSOR_RESP,         /* rrmuem_cm_get_aggressor_resp_t */
    /* eICIC_PHASE_1_2_CHANGES_END */
    /** eICIC_PHASE_1_2_CHANGES_START */
    RRMUEM_CELLM_EICIC_PROVISION_RECONFIG_RESP /*rrmuem_cellm_eicic_provision_reconfig_resp_t*/
    /** eICIC_PHASE_1_2_CHANGES_END */

}rrm_cm_uem_message_resp_e;

/* Carrier_Aggregation_Start */
typedef enum _rrmcm_cell_message_req_e
{
    RRMCM_CELL_LOAD_ACTN_SCELL_DEACT_IND = RMCM_CELL_API_REQ_BASE,
    RRM_CELL_LOAD_MULTICAST_INFO_IND,
    RRM_CELL_CA_CONFIG_CHANGE_MULTICAST_INFO_IND,
/* CA_Stage3_Change: Start */
    RRM_CELL_CA_CONFIG_CHANGE_MULTICAST_INFO_IND_RESP
/* CA_Stage3_Change: End */
}rrmcm_cell_message_req_e;
/* Carrier_Aggregation_End */

typedef enum _rrm_mif_logger_message_ind_e
{
    RMIF_LOGGER_INIT_IND
}rrm_mif_logger_message_ind_e;

typedef struct _rrm_logger_ctxt_t
{
    U32 wait_interval;
}rrm_logger_ctxt_t;

typedef struct _rrm_logger_init_cfg
{
    U32 cpu_bitmap;
    U16 wait_interval;
}rrm_logger_init_cfg_t;
/* ! \def RRMUEM_RMIF_SHUTDOWN_RESP
*/


#ifndef _MSC_VER

#define RMIF_MAX_API RRMUEM_RMIF_SHUTDOWN_RESP
/* ! \def RRMCM_RRMUEM_API_BASE
*/
#define RRMCM_RRMUEM_API_BASE                   0x300
/* ! \def RRMCM_RRMUEM_MAX_API
*/
#define RRMCM_RRMUEM_MAX_API            		(RRMCM_RRMUEM_API_BASE + 100)
/* Measurment Re-config END */

/* Trace Procedure Code */
/* ! \def NORMAL
*/
#define NORMAL 									10 

#ifdef WIN32
/* ! \def RRM_STRDUP
*/
#define RRM_STRDUP _strdup
#else
/* ! \def RRM_STRDUP(X)
*/
#define RRM_STRDUP(X) strdup((char*)X)
#endif

/*
*   logging
*/

#define RRM										4
#define RRM_MODULE_ID_ARR 3
#ifdef RRM_ADV_LOGGER
    #define LOGGER_TYPE RRM_ADV_LOGGER
#endif

#ifdef RRM_INTERNAL_LOGGER
    #define LOGGER_TYPE RRM_INTERNAL_LOGGER
#endif

#ifdef RRM_IN_MEM_LOGGER
    #define LOGGER_TYPE RRM_IN_MEM_LOGGER
#endif
#define RRM_LOG_LEVEL_NONE  QVLOG_NONE

#define RRM_MIN_MODULE_ID_FOR_ADV_LOGGER 21
/* SPR 21622 start */
#define DEFAULT_LOG_FILE                "rrm_main.log"
/* SPR 21622 end */
#if (RRM_ADV_LOGGER == LOGGER_TYPE)
    /*
     *  Advanced logger
     */
    
    /*! \headerfile advlogging.h <>
     */
    #include <advlogging.h>

    /*! \fn rrm_void_t rrm_set_module_loglevel_limit(rrm_oam_log_level_et log_level)
     *  \brief This function sets the module log level to advance logger
     *  \param log_level    Log level to be used(RRM_ERROR, RRM_BRIEF etc.)
     */
    rrm_void_t
    rrm_set_module_loglevel_limit
    (
         rrm_oam_log_level_et log_level
    );
#ifndef OAM_IPR
    extern AdvL_ContextIndex        g_advl_ctxt[RRM_MODULE_ID_ARR];//[RRM_MAX_INT_MODULE_ID];
    extern AdvL_ContextIndex        g_main_advl_ctxt;


    /*! \def SET_MODULE_LOG_LEVEL(log_level)
     *  \brief Macro defining the function to set module's log level
     */
    #define SET_MODULE_LOG_LEVEL(log_level)\
        rrm_set_module_loglevel_limit(log_level);
#endif
    
    /*! \def RRM_LOG_CTXT_CONSOLE
     *  \brief Macro to set the logs to appear on console
     */
    #define RRM_LOG_CTXT_CONSOLE    LogTarget_Console

    /*! \def RRM_LOG_CTXT_FILE
     *  \brief Macro to set the logs to appear in a file
     */
    #define RRM_LOG_CTXT_FILE       LogTarget_File

    /*! \def RRM_CRITICAL
     *  \brief Macro defining log level for CRITCAL logs/traces
     */
    #define RRM_CRITICAL             LogFatal
    
    /*! \def RRM_ERROR
     *  \brief Macro defining log level for ERROR logs/traces
     */
    #define RRM_ERROR               LogError
    
    /*! \def RRM_WARNING
     *  \brief Macro defining log level for WARNING logs/traces
     */
    #define RRM_WARNING             LogWarning
    
    /*! \def RRM_INFO
     *  \brief Macro defining log level for INFO logs/traces
     */
    #define RRM_INFO             LogInfo
    
    /*! \def RRM_BRIEF
     *  \brief Macro defining log level for BRIEF logs/traces
     */
    #define RRM_BRIEF               LogBrief
    
    /*! \def RRM_DETAILED
     *  \brief Macro defining log level for DETAILED logs/traces
     */
    #define RRM_DETAILED            LogDetailed

    /*! \def RRM_DETAILEDALL
     *  \brief Macro defining log level for COMP_PARSE logs/traces
     */
    /* SPR 21251 Start */
    #define RRM_DETAILEDALL          LogDebug
    /* SPR 21251 End */

    /*! \def RRM_INIT_LOG(log_file_name) 
     *  \brief Macro to be used to initialize advance logging.
     */
	#define RRM_INIT_LOG(log_file_name) rrm_init_log(log_file_name) 
    
    /*! \def RRM_LOG(is_log_enable, facility_name, log_level, format, ...)
     *  \brief Macro to be used for logging. Its first argument tells if the 
     *      corresponding log is be printed or not. Second argument gives the 
     *      information about the facility using the log. Third argument gives
     *      the log level(INFO, WARNING etc.). Forth argument is used to give 
     *      any nformation related to the event using the log.
     */

extern short test_log(U16 usModId, U16 logLevel, char* fmt, ...);
     
#ifdef LINUX_PC_TEST
	#define RRM_LOG(is_log_enable, facility_name, log_level, format, ...) \
		{\
			test_log(0,log_level,format,##__VA_ARGS__);\
		}
#else
    #define RRM_LOG(is_log_enable, facility_name, log_level, format, ...) \
        {\
            if ((U32)RRM_OAM_LOG_ON == (U32)is_log_enable)\
            {\
                U8 module_id = 0;\
                module_id = qvGetServiceName(qvSelf());\
                if (( module_id >= RRM_MIN_MODULE_ID_FOR_ADV_LOGGER) && (0x64 != module_id))\
                {\
                    write_log_message(g_advl_ctxt[module_id - \
                        RRM_MIN_MODULE_ID_FOR_ADV_LOGGER], log_level, __FILE__, \
                        __LINE__, __func__ , (char *)facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), format, \
                        ##__VA_ARGS__);\
                }\
            }\
        }        
#endif

    /*! \def RRM_PANIC(format, ...)
     *  \brief Macro to be used by the encapsulation code for aborting the 
     *      process in error situation. Its argument is used to give any 
     *      information related to the event using the log.
     */
    #define RRM_PANIC(format, ...) \
        {\
            write_log_message(g_main_advl_ctxt, RRM_ERROR, __FILE__, \
                __LINE__, __func__ , (char *)rrm_log_facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), format, \
                ##__VA_ARGS__);\
            rrm_abort();\
        }
    
    /*! \def RRM_ENCAPS_WARNING(format, ...)
     *  \brief Macro to be used by the encapsulation code to print some warning.
     *      Its argument is used to give any information related to the event 
     *      using the log.
     */
    #define RRM_ENCAPS_WARNING(format, ...) \
        {\
            write_log_message(g_main_advl_ctxt, RRM_ERROR, __FILE__, \
                __LINE__, __func__ , (char *)rrm_log_facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), format, \
                ##__VA_ARGS__);\
        }
/* SPR_17377_Warning_fix + */
    void rrm_trace_dbg(
            U32 is_log_enable, const S8* facility_name,U8 log_level,
            const char* format, ...);

#define TRACE_DBG(is_log_enable,facility_name,log_level,format, ...)   rrm_trace_dbg(is_log_enable,facility_name, log_level, format, ## __VA_ARGS__);
#ifdef ATTRIBUTE_COMP
#define RRM_TRACE TRACE_DBG
#else
/* SPR_17377_Warning_fix - */

    
    /*! \def RRM_TRACE(is_log_enable, log_level, format, ...)
     *  \brief Macro to be used for tracing. It is same as RRM_LOG macro 
     *      but with a default facility name(LTE_RRM).
     */
    #define RRM_TRACE(is_log_enable, facility_name, log_level, format, ...) \
        RRM_LOG(is_log_enable, rrm_log_facility_name, log_level, format, \
        ##__VA_ARGS__)
/* SPR_17377_Warning_fix + */
#endif
/* SPR_17377_Warning_fix - */

#elif (LOGGER_TYPE == RRM_INTERNAL_LOGGER)
    /*
    *   Internal logger
    */
    
    /*! \fn set_module_log_level(rrm_oam_log_level_et log_level)
     *  \brief  Set the Module Log Level
     *  \param  log_level

     */
#define  LOG_TYPE          1
#define  FILE_LOGGING      2

	#define RRM_INIT_LOG(log_file_name) 
    void rrm_trace_message(S32 log_level,const S8 *facility_name,
        const S8* format,...);
    void rrm_set_loglevel(U8 new_log_level);
    void set_module_log_level ( rrm_oam_log_level_et log_level);

    #define RRM_CRITICAL       	QVLOG_CRITICAL
    #define RRM_ERROR       	QVLOG_MAJOR
    #define RRM_WARNING     	QVLOG_MINOR
    #define RRM_INFO         	QVLOG_INFO
    #define RRM_BRIEF       	QVLOG_INFO
    #define RRM_DETAILED    	QVLOG_DETAIL
    #define RRM_DETAILEDALL     QVLOG_DUMP


   /*! \def RRM_PANIC(format, ...)
    *  \brief Macro to be used by the encapsulation code for aborting the process
    *      in error situation. Its argument is used to give any information
    *      related to the event using the log.
    */

    #define RRM_PANIC(format, ...) qvPanic((const S8)format, ##__VA_ARGS__)

   /*! \def RRM_ENCAPS_WARNING (format, ...)
    *  \brief Macro to be used by the encapsulation code to print some warning.
    *         Its argument is used to give any information related to the event
    *         using the log.
    */
    #define RRM_ENCAPS_WARNING(format, ...) qvWarning((const S8*)format,\
                                                      ##__VA_ARGS__)
    
    #define RRM_LOG(is_log_enable, facility_name, log_level, format, ...) \
        if ((U32)RRM_TRUE == (U32)is_log_enable) rrm_trace_message \
           (log_level, facility_name, (const S8*)format, ##__VA_ARGS__)
/* SPR_17377_Warning_fix + */
    void rrm_trace_dbg(
            U32 is_log_enable, const S8* facility_name,U8 log_level,
            const char* format, ...) __attribute__((format (printf, 4, 5)));

#define TRACE_DBG(is_log_enable,facility_name,log_level,format, ...)   rrm_trace_dbg(is_log_enable,facility_name, log_level, format, ## __VA_ARGS__);
#ifdef ATTRIBUTE_COMP
#define RRM_TRACE TRACE_DBG
#else
/* SPR_17377_Warning_fix - */

    
    #define RRM_TRACE(is_log_enable, facility_name, log_level, format, ...) \
        RRM_LOG(is_log_enable, facility_name, log_level, format, ##__VA_ARGS__)
/* SPR_17377_Warning_fix + */
#endif
/* SPR_17377_Warning_fix - */

    #define SET_MODULE_LOG_LEVEL(log_level)\
        set_module_log_level(log_level);
    
#elif (LOGGER_TYPE == RRM_IN_MEM_LOGGER)
rrm_void_t set_module_log_level ( rrm_oam_log_level_et log_level);
rrm_void_t *rrm_thread_start(void * thread_arg);


	#define RRM_INIT_LOG(log_file_name) 

/* SPR 21275 changes start */
    #define RRM_CRITICAL       	LGR_CRITICAL
    #define RRM_ERROR       	LGR_ERROR
    #define RRM_WARNING     	LGR_WARNING
    #define RRM_INFO         	LGR_INFO
    #define RRM_BRIEF       	LGR_BRIEF
    #define RRM_DETAILED    	LGR_DETAILED
    #define RRM_DETAILEDALL     LGR_DEBUG
/* SPR 21275 changes end */

/* SPR 21275 changes start */
/* SPR 21275 changes end */
/* SPR 20585 Fix Start */
    #define RRM_LOG(is_log_enable, facility_name, log_level, format, ...) \
        {\
            if ((U32)RRM_OAM_LOG_ON == (U32)is_log_enable)\
            {\
                QMODULE module;\
                module = qvSelf();\
                if (module && log_level <= qvGetModuleLogLevel(module)){\
		/* SPR 21820 Start */\
                print_log_message(log_level, __FILE__, __LINE__, __func__,\
                        (char *)facility_name, GET_CELL_INDEX(),GET_UE_INDEX(), LGR_FALSE, format, ##__VA_ARGS__);}\
		/* SPR 21820 End */\
            }\
        }        

   /*! \def RRM_PANIC(format, ...)
    *  \brief Macro to be used by the encapsulation code for aborting the process
    *      in error situation. Its argument is used to give any information
    *      related to the event using the log.
    */

    #define RRM_PANIC(format, ...) \
        {\
            QMODULE module;\
            module = qvSelf();\
            if (module && RRM_ERROR <= qvGetModuleLogLevel(module)){\
	    /* SPR 21820 Start */\
            print_log_message(RRM_ERROR, __FILE__, __LINE__, __func__,\
                    (char *)rrm_log_facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), LGR_FALSE, format, ##__VA_ARGS__);}\
	    /* SPR 21820 End */\
        }

   /*! \def RRM_ENCAPS_WARNING (format, ...)
    *  \brief Macro to be used by the encapsulation code to print some warning.
    *         Its argument is used to give any information related to the event
    *         using the log.
    */
    #define RRM_ENCAPS_WARNING(format, ...) \
        {\
            QMODULE module;\
            module = qvSelf();\
            if (module && RRM_WARNING <= qvGetModuleLogLevel(module)){\
	    /* SPR 21820 Start */\
            print_log_message(RRM_WARNING, __FILE__, __LINE__, __func__,\
                    (char *)rrm_log_facility_name, GET_CELL_INDEX(), GET_UE_INDEX(), LGR_FALSE, format, ##__VA_ARGS__);}\
	    /* SPR 21820 End */\
        }
/* SPR 20585 Fix End */
    
    #define RRM_TRACE(is_log_enable, facility_name, log_level, format, ...) \
        RRM_LOG(is_log_enable, facility_name, log_level, format, ##__VA_ARGS__)

    #define SET_MODULE_LOG_LEVEL(log_level)\
        set_module_log_level(log_level)
    
#elif (LOGGER_TYPE > RRM_ADV_LOGGER)
    #error Wrong logger type specified!
#else
    #define RRM_LOG(is_log_enable, facility_name, log_level, format, ...)
#endif
#define RRM_COMP_PARSE RRM_DETAILEDALL 


 /* SPR21623 Changes Start */
#define RRM_SET_UE_PROCEDURE_STATE(CURRENT_PROCEDURE, STATE) \
    rrm_set_ue_proc_state(CURRENT_PROCEDURE, STATE)

#define UEM_FSM_SET_STATE(P_UE_CONTEXT, NEW_STATE) \
    rrm_ue_set_state(P_UE_CONTEXT,NEW_STATE)
 /* SPR21623 Changes End */

extern const S8 *rrm_log_facility_name;
/*
*   UT Trace
*/
/* This will be used when debugging flag is ON. In this logging mode will
    be considered as enabled. */
/*! \def RRM_DEBUG_TRACE(log_level, format, ...)
 *  \brief Macro to be used for debugging purpose traces.
 */
#ifdef RRM_DEBUG
    #define RRM_DEBUG_TRACE(log_level, format, ...) \
        RRM_TRACE(RRM_TRUE, rrm_log_facility_name, log_level, format, \
        ##__VA_ARGS__)
    
/*++SPR 20009 */
        #define RRM_UT_TRACE_ENTER() \
     			RRM_DEBUG_TRACE(RRM_DETAILEDALL, "FUNC_ENTER: LINE %d FUNCTION %s",__LINE__,__FUNCTION__)
    
        #define RRM_UT_TRACE_EXIT() \
           		RRM_DEBUG_TRACE(RRM_DETAILEDALL, "FUNC_EXIT: LINE %d, FUNCTION %s",__LINE__,__FUNCTION__) 
/*--SPR 20009 */

    #define RRM_FSM_INIT_TRACE(fsm_name, state_name) \
        RRM_TRACE(RRM_BRIEF, "[%s] initialised, initial state [%s]", \
                fsm_name, state_name)

    #define RRM_FSM_SET_STATE_TRACE(fsm_name, state_name) \
        RRM_TRACE(RRM_BRIEF, "[%s] state changed to [%s]", \
                fsm_name, state_name)

    #define RRM_FSM_EVENT_TRACE(fsm_name, event_name, state_name) \
        RRM_TRACE(RRM_BRIEF, "[%s] event [%s] received in state [%s]", \
                fsm_name, event_name, state_name)
#else
    #define RRM_UT_TRACE_ENTER()
    #define RRM_UT_TRACE_EXIT()
    #define RRM_FSM_INIT_TRACE(fsm_name, state_name)

    #define RRM_FSM_SET_STATE_TRACE(fsm_name, state_name)

    #define RRM_FSM_EVENT_TRACE(fsm_name, event_name, state_name)
#endif /* RRM_DEBUG */
/*
*   ASSERT
*/
#ifdef RRM_DEBUG
void
rrm_assert
(
    void* file,
    U32 line,
    void* expression
);
#define RRM_ASSERT(exp) (void)((exp) || (rrm_assert(__FILE__,__LINE__,#exp),0))
#else
#define RRM_ASSERT(exp)
#endif /* RRM_DEBUG */

/****************************************************************************
 * Macro to log message hex dump
 ****************************************************************************/
#define RRM_LOG_HEX_DUMP(direction, msgBuf, msgLength, moduleId, apiId, version)
/* Bit Masks */
#define RRM_BIT0_MASK   0x00000001U
#define RRM_BIT1_MASK   0x00000002U
#define RRM_BIT2_MASK   0x00000004U
#define RRM_BIT3_MASK   0x00000008U
#define RRM_BIT4_MASK   0x00000010U
#define RRM_BIT5_MASK   0x00000020U
#define RRM_BIT6_MASK   0x00000040U
#define RRM_BIT7_MASK   0x00000080U
#define RRM_BIT8_MASK   0x00000100U
#define RRM_BIT9_MASK   0x00000200U
#define RRM_BIT10_MASK  0x00000400U
#define RRM_BIT11_MASK  0x00000800U
#define RRM_BIT12_MASK  0x00001000U
#define RRM_BIT13_MASK  0x00002000U
#define RRM_BIT14_MASK  0x00004000U
#define RRM_BIT15_MASK  0x00008000U
#define RRM_BIT16_MASK  0x00010000U
#define RRM_BIT17_MASK  0x00020000U
#define RRM_BIT18_MASK  0x00040000U
#define RRM_BIT19_MASK  0x00080000U
#define RRM_BIT20_MASK  0x00100000U
#define RRM_BIT21_MASK  0x00200000U
#define RRM_BIT22_MASK  0x00400000U
#define RRM_BIT23_MASK  0x00800000U
#define RRM_BIT24_MASK  0x01000000U
#define RRM_BIT25_MASK  0x02000000U
#define RRM_BIT26_MASK  0x04000000U
#define RRM_BIT27_MASK  0x08000000U
#define RRM_BIT28_MASK  0x10000000U
#define RRM_BIT29_MASK  0x20000000U
#define RRM_BIT30_MASK  0x40000000U
#define RRM_BIT31_MASK  0x80000000U

/* Bit Mask Map 0 -> 1, 2 -> 2, 3 -> 4, 4 -> 8 etc. */
extern U32 bit_masks_map[32];

/* Bit array operations */
#define RRM_BIT_SET(X, Y)      X |= bit_masks_map[Y]
#define RRM_BIT_UNSET(X, Y)    X &= ~bit_masks_map[Y]

/* CID 30649*/
#define MAX_NUM_OF_SUPPORTED_MQ     2
#define SET_PERMISSION                   0666
/* SPR 20712 Fix Start */
/* Code deleted */
/* SPR 20712 Fix End */
/* SPR 19288 change start */
/* SPR 19288 change end */
/*Load balancing: Max action value*/
/* Carrier_Aggregation_Start */
/*ut_bug_13978_fix_start*/
#define RRM_MAX_ACTION_VALUE            4095
/*ut_bug_13978_fix_end*/
/* Carrier_Aggregation_End */
/******************************************************************************
*   Memory management functions
******************************************************************************/

extern const QSYSOP rrm_os;
extern const QWAIT rrm_waitstruct;
extern const QSHELL rrm_shell;

extern const QMANIFEST rrm_manifest_mgmt_if;
extern const QMANIFEST rrm_manifest_apps;
extern const QMANIFEST rrm_manifest_nmm;

/*! \typedef struct rrm_l2_msgq_intf_data
 *  \brief
 *  \param  mqd_send_to_mac[MAX_NUM_OF_SUPPORTED_MQ]
 *  \param  mqd_recv_from_mac[MAX_NUM_OF_SUPPORTED_MQ]
 */
typedef struct _rrm_l2_msgq_intf_data
{
    mqd_t  mqd_send_to_mac[MAX_NUM_OF_SUPPORTED_MQ];
    mqd_t  mqd_recv_from_mac[MAX_NUM_OF_SUPPORTED_MQ];
    struct mq_attr message_attributes;
}rrm_l2_msgq_intf_data;

typedef struct
{
    mqd_t  mqd_send_to_pdcp[MAX_NUM_OF_SUPPORTED_MQ];
    mqd_t  mqd_recv_from_pdcp[MAX_NUM_OF_SUPPORTED_MQ];
    struct mq_attr message_attributes;
}rrm_pdcp_msgq_intf_data;

typedef enum
{
    RRM_MAIN_THRD,
    RRM_LOGGER_THRD,
    RRM_MAX_THRD
}rrm_thread_id_et;
/*! \typedef struct thrd_arg_t
 *  \brief
 *  \param sd_udp                                       UDP socket descriptor
 */
typedef struct thread_arg
{
    QDRIVER         driver_id;
    const QWAIT     *p_qwait;
    lgr_thread_id_t logical_thid;
}thrd_arg_t;
/*! \typedef struct rrm_listener_t
 *  \brief
 *  \param sd_udp                                       UDP socket descriptor
 */
typedef struct _rrm_listener_t
{
    int   sd_l2_trace;
    int   sd_udp_rrm;
    /*SPR 20856 +-*/
}rrm_listener_t;

#define RRM_MEM_FREE(p_buffer) \
        { \
            rrm_mem_free(p_buffer); \
           p_buffer = RRM_PNULL;\
        }

#define RRM_MSG_MEM_FREE(p_buffer) \
        { \
            rrm_msg_mem_free(p_buffer); \
           p_buffer = RRM_PNULL;\
        }
/*! \fn rrm_void_t rrm_init_log(const S8 * p_log_file_name);
*  \brief This function is used to initialize advance logger
*  \param p_log_file_name  Pointer to the log file name
*/
rrm_void_t
rrm_init_log
(
      const S8 * p_log_file_name
);

/*SPR 20172 Fix Start*/
/*Code Deleted*/
/*SPR 20172 Fix End*/
/*! \fn rrm_void_t rrm_msg_mem_get(size_t size)
 *  \brief It returns memory buffer from memory pool.
 *  \param size     Size of buffer which will be allocated
 */
rrm_void_t *
rrm_msg_mem_get
(
    size_t size    
);

/*CA STAGE 2*/
typedef struct
{
	/*SPR 20908 Start*/
        U16    port;
        U8     ipaddr[4];
	/*SPR 20908 Start*/
}rrm_ext_comm_info;

typedef struct
{

/*SPR 20856 Start*/
      rrm_ext_comm_info   ext_comm_info[4];
/*SPR 20856 End*/

     /*CA STAGE 2*/
      U8                  rrm_no_of_l2_instances;    
     /* SPR 12537 start */
      S32                  rrm_l2_trace_port;
     /* SPR 12537 end */
}rrm_ext_communication_data;


/*! \fn rrm_void_t rrm_msg_mem_free(rrm_void_t *p_buffer)
 *  \brief It frees memory buffer allocated to memory pool.
 *  \param p_buffer Pointer to buffer which will be freed
 */
rrm_void_t
rrm_msg_mem_free
(
    rrm_void_t *p_buffer
);

void*
rrm_sys_mem_get
(
    rrm_size_t size
);
/*! \fn rrm_void_t rrm_register_modules(rrm_void_t)
 *  \brief It registers various RRM modules with CSPL.
*/

rrm_void_t
rrm_register_modules
(
    thrd_arg_t *p_thrd_arg
 );

/*! \fn rrm_void_t rrm_construct_cspl_header(rrm_void_t *p_buf,U8 version,U16 from,U16 to,U16 api_id,U8 priority,U16 payloadlen)
 *  \brief It is used to create the CSPL Header of the message
 *  \param p_buf        Pointer to the buffer to which header is appended
 *  \param version      version of API
 *  \param from         Source module Id
 *  \param to           Destination module Id
 *  \param api_id       API Identifier
 *  \param priority     Priority of the message
 *  \param payloadlen   lenght of the payload
 */
rrm_void_t
rrm_construct_cspl_header
(
    rrm_void_t  *p_buf,
    U8          version,
    U16         from,
    U16         to,
    U16         api_id,
    U8          priority,
    U16         payloadlen
);
/*! \fn rrm_void_t rrm_parse_cspl_header(rrm_void_t  *p_buf, STACKAPIHDR *p_cspl_hdr)
 *  \brief It is used to parse the CSPL Header and populate the STACKAPIHDR structure
 *  \param p_buf        Pointer the CSPL header
 *  \param p_cspl_hdr   Pointer to the STACKAPIHDR structure to be populated
 */
rrm_void_t
rrm_parse_cspl_header
(
    rrm_void_t  *p_buf,
    STACKAPIHDR *p_cspl_hdr
);

/*SPR 17777 +-*/

/*SPR 20172 FIX START*/
#define rrm_send_message(p_msg,dst_module) do {\
	rrm_send_message_ex(p_msg,dst_module);\
	p_msg = PNULL;\
}while(0)
void
rrm_send_message_ex
/*SPR 20172 FIX END*/
(
    void *p_msg,
    rrm_module_id_t dst_module
);

void*
rrm_mem_get
(
    rrm_size_t size
);

void
rrm_mem_free
(
    void *p_buffer
);


/******************************************************************************
*   Timer management functions
******************************************************************************/

/* CSR-58333-fix start*/
/* rrm_start_timer, rrm_stop_timer and rrm_get_timer_data function prototypes are deteled */
/* CSR-58333-fix end*/

/******************************************************************************
*   File management functions
******************************************************************************/

void
rrm_free
(
    void * p_var
);

float
rrm_ceil
(
   float var 
);
/* SPR 19337 : CID 11193 fix start */
FILE *
rrm_fopen
(
    const char* p_filename,
    const char* p_mode
);

/* SPR 19337 : CID 11193 fix stop */

S32 
rrm_fclose
(
    FILE *fp
);

void
rrm_construct_api_header
(
    U8                  *p_header,      /* RRM header */
    U8                  version_id,     /* API version Id */
    rrm_module_id_t     src_module_id,  /* Source module Id */
    rrm_module_id_t     dst_module_id,  /* Destination module Id */
    U16                 api_id,         /* API Id */
    U16                 api_buf_size    /* API buffer size */
    
   
);

void
rrm_construct_interface_api_header
(
 U8                  *p_header,      /* RRM interface header */
 U16                 transaction_id, /* Interface transaction identifier */
 rrm_module_id_t     src_module_id,  /* Source module identifier */
 rrm_module_id_t     dst_module_id,  /* Destination module identifier */
 U16                 api_id,         /* API identifier */
 U16                 api_buf_size,    /* API buffer size */
 rrm_cell_index_t    cell_index      /* Cell Index */
 );

U16
rrm_get_api_buf_size
(
    U8 *p_header /* RRM header */
);

U16
rrm_get_api_id
(
    rrm_void_t *p_header
);


rrm_module_id_t
rrm_get_src_module_id
(
 void *p_header /* RRM header */
 );


U16
rrm_get_word_from_header
(
    U8 *p_header
);

rrm_cell_index_t
rrm_get_cell_id_from_header
(
 U8 *p_header
 );

/*! \fn U16 rrm_generate_txn_id()
 *  \brief It is used to generate the transaction Id
   
 */
/* + SPR 17439 */
U16 rrm_generate_txn_id(void); 
/* - SPR 17439 */

/* Fix for CSR 58972 start*/
U8 rrm_get_enb_type_from_access_mode ( S32  cell_access_mode
        );
/* Fix for CSR 58972 end*/

/* SPR-9346-fix start */
/* + SPR 17439 */
rrm_return_et
is_big_endian(void);
/* - SPR 17439 */


#define RRM_12BIT_MAX_VALUE                                           4095
#define RRM_BYTE_SIZE_IN_BITS                                            8
#define RRM_NIBBLE_SIZE_IN_BITS                                          4
#define RRM_EXTENDED_RNCID_BITMASK                              0x80000000
#define RRM_UMTS_CID_MIN_NUM_BITS                                       12

void rrm_encode_rncid_to_cell_identity(
        U8 *p_cell_identity,
        U16 rncid);


void rrm_encode_cid_to_cell_identity(
        U8 *p_cell_identity,
        U16 cid);
/* SPR-9346-fix end */

/*bug_1016_start*/

void* rrm_ue_get_timer_data(QTIMER p_timerId);

/*bug_1016_end*/

extern rrm_l2_msgq_intf_data      *p_rrm_l2_msgq_intf_glb_data;
void *rrm_malloc(unsigned int size);
extern rrm_pdcp_msgq_intf_data *p_rrm_pdcp_msgq_intf_glb_data; 

#define RRM_RRC_MSG_PARSE_FAILURE 10
#define RRM_RRC_MSG_SEND_FAIL	   11
#define RRM_RRC_MSG_SUCCESS	 12

/* COMP_WARN_1_FEB:compilation warning removal */
void *rrm_malloc(unsigned int size);
rrm_return_et rrm_oam_check_thread_status(U16 periodicity);
rrm_void_t rrm_oam_set_status_for_health(rrm_thread_id_t thread_id);
/* bug id 7755 fix end */

/*CA STAGE 2*/
U8 rrm_map_cell_id_to_instance_id(U8 cell_index);
/*SPR 20908 Start*/
void rrm_map_cell_id_to_ip_port(U8 cell_index,rrm_oam_l2_comm_info_t *l2_comm_info,U8 src_module_id);
/*SPR 20908 End*/


/*SPR 20908 +-*/

//added on 25 Oct 2013
extern S32 rrm_length_read_g;
/*SPR 19327 Fix Start*/                       
typedef enum _rrm_utra_cell_type              
{                                             
    INVALID_CELL = RRM_ZERO,                  
    UTRA_FDD,                                 
    UTRA_TDD                                  
}rrm_utra_cell_type_et;                       
/*SPR 19327 Fix Stop*/                        

/* Carrier Aggregation start*/

#define RRM_CA_CELL_DELETE_BITMASK  0x01
#define RRM_CA_CELL_STOP_BITMASK    0x02
#define RRM_CA_CELL_CA_TO_NON_CA    0x04
#define RRM_CA_CELL_STOP_ADM_BITMASK  0x08

void
rrm_memcpy_rrm_global_cell_id( rrm_global_cell_id_t *p_dst, 
                               rrm_global_cell_id_t *p_src);
/* Carrier Aggregation end*/
/* ! \def RRM_FSEEK               
*/
#define RRM_FSEEK fseek
/* SPR 13383 changes start */
#define RRM_BANDWIDTH_1_4MHZ    0x01  
#define RRM_BANDWIDTH_3MHZ      0x02
#define RRM_BANDWIDTH_5MHZ      0x04
#define RRM_BANDWIDTH_10MHZ     0x08
#define RRM_BANDWIDTH_15MHZ     0x10
#define RRM_BANDWIDTH_20MHZ     0x20



#define RRM_BANDWIDTH_CLASS_A     0x01
#define RRM_BANDWIDTH_CLASS_B     0x02
#define RRM_BANDWIDTH_CLASS_C     0x04
#define RRM_BANDWIDTH_CLASS_D     0x08
#define RRM_BANDWIDTH_CLASS_E     0x10
#define RRM_BANDWIDTH_CLASS_F     0x20


#define RRM_CARRIER_COMPONENT_1_BITMASK     0x01
#define RRM_CARRIER_COMPONENT_2_BITMASK     0x02
/* SPR 13383 changes end */

/* SPR 16477 Fix Start */
rrm_return_et
rrm_plmnid_from_mcc_mnc
(
    U8                          *p_out_plmn, 
    rrm_oam_cell_plmn_info_t    *p_input_plmn
);
/* SPR 16477 Fix Stop */

/* SPR_17763_FIX_START */
U8
rrm_is_newer_node_getting_inserted
(
    struct timeval *p_node_time,
    struct timeval *p_temp_node_time
);
/* SPR 21446 start */
const char *rrm_get_ho_type_string(rrc_handover_type_et ho_type);
const char *rrm_get_ue_fsm_procedure_state_string(U32 state);
const char *rrm_map_trigger_quantity(U32 trig_quantity);
const char *rrm_map_meas_event(U32 meas_event);
/* SPR 21446 end */
/* SPR_17763_FIX_END */

/*SPR 21419 Fix Start*/
/*! \fn	 QTIMER
 * rrm_module_timer_start(rrm_module_id_t module, QTIME *duration, void *p_timer_data, U16 timer_data_size, rrm_bool_et is_repeated,S32 *Err)
 *
 *  \brief		This function start the timer for any task of destination module
 */
QTIMER
rrm_module_timer_start(rrm_module_id_t module, QTIME *duration, void *p_timer_data, U16 timer_data_size, rrm_bool_et is_repeated,S32 *Err);

/*! \fn	rrm_return_et rrm_module_timer_stop(QTIMER p_timerId, QTIME *time, S32 *Err) 
 *  \brief		This function stop the timer for any task of destination module
 */
rrm_return_et
rrm_module_timer_stop(QTIMER p_timerId, QTIME *time, S32 *Err);

/*! \fn	void* cell_get_timer_data(QTIMER p_timerId) 
 *  \brief		This function get timer data and memfree it 
 */

void* cell_get_timer_data(QTIMER p_timerId);

/*SPR 21419 Fix Stop*/



/*SPR 22554 Fix Start*/
U16
rrm_pthread_setname_np
(
pthread_t *thread,
U8 *threadName
);
/*SPR 22554 Fix End*/
#endif // _MSC_VER

#endif // _RRM_UTILS_H_
