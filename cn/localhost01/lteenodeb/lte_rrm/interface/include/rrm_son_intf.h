/*! \file rrm_son_intf.h
 *  \date   07 Mar 2012
 *  \brief  This file contains the rrm son interface
 *                          definition.
 */

/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */


#ifndef _RRM_SON_INTF_H
#define _RRM_SON_INTF_H

#include <stacklayer.h>
#include "rrm_api_defines.h"
#include "rrm_api_types.h"
/*! \enum rrm_son_message_req_et
 *  \brief An enum that contains the rrm son message request
 */


typedef enum 
{
    RRM_SON_REGISTER_REQ = RRM_SON_API_BASE,
    RRM_SON_DEREGISTER_REQ,
    RRM_SON_SET_ATTRIBUTE_REQ,
    RRM_SON_CELL_SWITCH_ON_REQ,
    RRM_SON_CELL_SWITCH_OFF_REQ,
    RRM_SON_NMM_PREPARE_REQ,
    RRM_SON_NMM_COMPLETE_REQ,
    /* RRM_SON_APPS_CARRIER_FREQ_AND_DL_POWER_PARAMS_REQ, */
    RRM_SON_MEAS_CONFIG_REQ,
    RRM_SON_TNL_DISCOVERY_REQ,
    RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ,
    RRM_SON_RACH_CONFIG_REQ,
    /*SPR 10329:start*/
    RRM_SON_ENB_CONFIG_UPDATE_IND,
    /*SPR 10329:end*/
    RRM_SON_MAX_API
    /*This should always be the last value of enum*/
}rrm_son_message_req_et;
/*! \enum rrm_son_message_resp_et
 *  \brief An enum that contains rrm son response
 */
typedef enum 
{
    RRM_SON_REGISTER_RESP = SON_RRM_API_BASE,
    RRM_SON_DEREGISTER_RESP,
    RRM_SON_SET_ATTRIBUTE_RESP,
    RRM_SON_CELL_SWITCH_ON_RESP,
    RRM_SON_CELL_SWITCH_OFF_RESP,
    RRM_SON_CELL_STATE_CHANGE_IND,
    RRM_SON_NUM_ACTIVE_UE_REPORT,
    RRM_UE_COUNT_THRESHOLD_REACHED_IND,
    RRM_SON_NMM_PREPARE_RESP,
    RRM_SON_NMM_COMPLETE_RESP,
    /* RRM_SON_APPS_CARRIER_FREQ_AND_DL_POWER_PARAMS_RESP, */
    RRM_SON_MEAS_CONFIG_RESP,
    RRM_SON_MEAS_RESULTS_IND,
    RRM_SON_HO_REPORT,
    RRM_SON_TNL_DISCOVERY_RESP,
    RRM_SON_HO_ATTEMPT_IND,              
    RRM_SON_HO_FAILURE_REPORT,          
    RRM_SON_LOAD_REPORT_IND,
    RRM_SON_HO_EXPIRE_IND, 
    RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP,
    /* RACH_OPTIMIZATION_CHANGES_START */
    RRM_SON_RACH_CONFIG_RESP,
    RRM_SON_RACH_UE_INFO_IND,
    RRM_SON_L2_RACH_PERF_REPORT,
    /* RACH_OPTIMIZATION_CHANGES_END */
    /** eICIC feature changes start */
    RRM_SON_EICIC_ENABLE_DISABLE_IND,
/* SPR 22248 MBMS Changes Start */
    /* CID 92819:start */
    RRM_SON_MBMS_SF_INFO_IND,
    /* CID 92819:end */
/* SPR 22248 MBMS Changes Stop */
    /** eICIC feature changes end */
    RRM_SON_RESP_MAX_API
}rrm_son_message_resp_et;
/*! \enum rrm_object_type_et
 *  \brief An enum that specifies the object type
*/
typedef enum 
{
    RRM_OBJ_CELL = 1,
    /*This should always be the last value of enum*/
    RRM_OBJ_UNDEF
} rrm_object_type_et;
/*! \enum rrm_attr_id_et
 *  \brief An enum that specifies the attribute id
 */

typedef enum
{
    RRM_ATTR_CELL_STATE = 1,
    RRM_ATTR_ACTIVE_UE_COUNT_REPORT,     
    RRM_ATTR_ACTIVE_UE_THRESHOLD,
    RRM_ATTR_S1_MSG_NOTIFICATION,
    RRM_ATTR_MEAS_UE_COUNT,
    RRM_ATTR_MEAS_REPORTING_INTERVAL,
    RRM_ATTR_HO_REPORT,
    RRM_ATTR_NEIGHBOR_LOAD_REPORT,
    RRM_ATTR_LOAD_REPORT,
    RRM_ATTR_HO_ATTEMPT_IND,
    RRM_ATTR_HO_FAILURE_REPORT,
    /* RRM ES Start */
    RRM_ATTR_SRV_CELL_LOAD_REPORT,
    /* RRM ES End */
    /* SPR 20653 Fix Start */
    RRM_ATTR_UE_THROUGHPUT_DL_ANR_THRESHOLD,
    RRM_ATTR_UE_THROUGHPUT_UL_ANR_THRESHOLD,
    /* SPR 20653 Fix End */
    /*This should always be the last value of enum*/
    RRM_ATTR_UNDEF 
}rrm_attr_id_et;

#define RRM_ATTR_LIST_SIZE (RRM_ATTR_UNDEF - 1) 

/*! \enum rrm_trigger_id_et
 *  \brief An enum that specifies the trigger id
*/

typedef enum 
{
    RRM_TRGR_PERIODIC = 1,
    RRM_TRGR_THRESHOLD,
    RRM_TRGR_EVENT,
    /*This should always be the last value of enum*/
    RRM_TRGR_UNDEF
}rrm_trigger_id_et;

#define RRM_TRGR_LIST_SIZE    (RRM_TRGR_UNDEF - 1) 

/*! \enum rrm_switchoff_et
*   \brief An enum that specifies the switchoff mode
*/
typedef enum 
{
    RRM_SO_GRACEFUL = 1,
    RRM_SO_FORCEFUL,
    /*This should always be the last value of enum*/
    RRM_SO_UNDEF
}rrm_switchoff_et;
/*! \enum rrm_cell_operational_state_et
 *  \brief An enum that defines whether a cell is operational
 */
typedef enum 
{
    RRM_CELL_OPERATIONAL = 1,
    RRM_CELL_UNOPERATIONAL,
    RRM_CELL_DELETED,
    /*This should always be the last value of enum*/
    RRM_CELL_STATE_UNDEFINED 
}rrm_cell_operational_state_et;
/*! \enum rrm_threshold_reached_cause_et
 *  \brief An eneum that specifies threshhold cause
*/
typedef enum
{
    RRM_ABOVE_TO_BELOW_THRESHOLD,
    RRM_BELOW_TO_ABOVE_THRESHOLD
}rrm_threshold_reached_cause_et;

/*! \enum rrm_cell_state_change_cause_et
*  \brief An enum that specifies the Cell State Change Cause.
*/
typedef enum 
{
    RRM_CELL_ADD = 1,
    RRM_CELL_DELETE,
    RRM_CELL_STOP,
    RRM_CELL_START,
    /*This should always be the last value of enum*/
    RRM_CELL_STATE_CHANGE_CAUSE_UNDEFINED 
}rrm_cell_state_change_cause_et;

/*! \enum rrm_son_cdma_band_class_et
*  \brief An enum that specifies the CDMA2000 band class
 */
typedef enum
{
    RRM_SON_BAND_CLASS_BC_0,
    RRM_SON_BAND_CLASS_BC_1,
    RRM_SON_BAND_CLASS_BC_2,
    RRM_SON_BAND_CLASS_BC_3,
    RRM_SON_BAND_CLASS_BC_4,
    RRM_SON_BAND_CLASS_BC_5,
    RRM_SON_BAND_CLASS_BC_6,
    RRM_SON_BAND_CLASS_BC_7,
    RRM_SON_BAND_CLASS_BC_8,
    RRM_SON_BAND_CLASS_BC_9,
    RRM_SON_BAND_CLASS_BC_10,
    RRM_SON_BAND_CLASS_BC_11,
    RRM_SON_BAND_CLASS_BC_12,
    RRM_SON_BAND_CLASS_BC_13,
    RRM_SON_BAND_CLASS_BC_14,
    RRM_SON_BAND_CLASS_BC_15,
    RRM_SON_BAND_CLASS_BC_16,
    RRM_SON_BAND_CLASS_BC_17,
    RRM_SON_BAND_CLASS_BC18_V9A0,
    RRM_SON_BAND_CLASS_BC19_V9A0,
    RRM_SON_BAND_CLASS_BC20_V9A0,
    RRM_SON_BAND_CLASS_BC21_V9A0
}rrm_son_cdma_band_class_et;

/** eICIC feature changes start */
typedef enum
{
    RRM_SON_EICIC_DEACTIVATED ,
    RRM_SON_ACT_AS_AGGRESSOR,
    RRM_SON_ACT_AS_VICTIM,
    RRM_SON_ACT_AS_AGGRESSOR_AND_VICTIM
}rrm_son_eicic_node_type_et;
/** eICIC feature changes end */

/*! \ rrm_cell_object_id_t 
 *  \brief RRM OBJECT ID STRUCT
 *  \param cell_id   Cell ID
 */
typedef struct _rrm_object_id 
{
    rrm_oam_eutran_global_cell_id_t   cell_id;		/*^ M, 0, N, 0, 0 ^*/
}rrm_cell_object_id_t;



#define RRM_CELL_ID_PRESENT 0x0001
/*! \ rrm_object_id_t 
 *  \brief RRM OBJECT ID
 *  \param object_id    Object ID
 */
typedef struct _rrm_object_id_t
{
    rrm_cell_object_id_t object_id;  	/*^ M, 0, N, 0, 0 ^*/	
}rrm_object_id_t;

/*! \ rrm_trgr_periodic_value_t 
 *  \brief RRM TRIGGER PERIODIC VALUE
 *  \param interval  TRIGGER PERIODIC Interval Value
 */
typedef struct _trgr_periodic_value_t
{
    U32      interval;		/*^ M, 0, N, 0, 0 ^*/
}rrm_trgr_periodic_value_t;

/*! \ rrm_trgr_threshold_value_t 
 *  \brief RRM TRIGGER THRESHOLD VALUE
 *  \param count TRIGGER THRESHOLD COUNT VALUE
 */
typedef struct _rrm_trgr_threshold_value_t
{
    U16       count; 		/*^ M, 0, N, 0, 0 ^*/
}rrm_trgr_threshold_value_t;

/*! \ rrm_trgr_event_value_t
 *  \brief RRM TRIGGER EVENT VALUE
 *  \param enable  Trigger Event Value
 */
typedef struct _rrm_trgr_event_value_t
{
    U8       enable;		/*^ M, 0, N, 0, 0 ^*/
}rrm_trgr_event_value_t;

#define RRM_TRIGGER_TYPE_PERIODIC 0x0001
#define RRM_TRIGGER_TYPE_THRESHOLD 0x0002
#define RRM_TRIGGER_TYPE_EVENT 0x0004
/*! \ rrm_trigger_t 
 *  \brief RRM TRIGGER Structure
 *  \param bitmask             Bitmask
 *  \param id                  Number of Triggers
 *  \param rrm_trigger_type    Triggers
 */
typedef struct _rrm_trigger_t
{
    rrm_bitmask_t                bitmask;		/*^ BITMASK ^*/
    rrm_trigger_id_et           id;			/*^ M, 0, N, 0, 0 ^*/
    rrm_trgr_periodic_value_t   periodic;	/*^ O, RRM_TRIGGER_TYPE_PERIODIC, N , 0, 0 ^*/
    rrm_trgr_threshold_value_t  threshold;	/*^ O, RRM_TRIGGER_TYPE_THRESHOLD, N , 0, 0 ^*/
    rrm_trgr_event_value_t      event;		/*^ O, RRM_TRIGGER_TYPE_EVENT, N , 0, 0 ^*/
}rrm_trigger_t;

/*! \ rrm_attribute_t 
 *  \brief RRM ATTRIBUTE Structure
 *  \param id                     Attribute Id
 *  \param trgr_list_size         Number of Triggers
 *  \param trgr_list              Triggers
 */
typedef struct _rrm_attribute_t
{
    rrm_attr_id_et      id;				/*^ M, 0, N, 0, 0 ^*/
    U8                  trgr_list_size;                	/*^ M, 0, N, 0, 0 ^*/			/* will be zero in case of deregister */
    rrm_trigger_t       trgr_list[RRM_TRGR_LIST_SIZE];  /*^ M, O, OCTET_STRING, VARIABLE ^*/	/*Number of valid values = trgr_list_size*/
    U32             value;                          /*^ M, 0, N, 0, 0 ^*/
}rrm_attribute_t;

/*! \ object_t 
 *  \brief OBJECT Structure
 *  \param type            Object Type
 *  \param id              Object Id
 *  \param attr_list_size  Number of Attributes
 *  \param p_attr_lst      Attributes
 */
typedef struct _object_t
{
     rrm_object_type_et  type;				/*^ M, 0, N, 0, 0 ^*/
     rrm_object_id_t    id;				/*^ M, 0, N, 0, 0 ^*/
     U8                 attr_list_size;			/*^ M, 0, N, 0, 0 ^*/
     rrm_attribute_t    p_attr_lst[RRM_ATTR_LIST_SIZE]; /*^ M, O, OCTET_STRING, VARIABLE ^*/	/*Number of valid values = attr_list_size*/
}object_t;

/*! \ rrm_son_register_deregister_format_t 
 *  \brief RRM SON REGISTER DEREGISTER REQ Structure
 *  \param object             Object
 */
typedef struct _rrm_son_register_deregister_format_t
{
     object_t           object;		 /*^ M, 0, N, 0, 0 ^*/         
}rrm_son_register_deregister_format_t;


/*! \ rrm_son_register_req_t 
 *  \brief RRM SON REGISTER REQ Structure
 *  \param object             Object
 */
typedef struct _rrm_son_register_req_t
{
	rrm_son_register_deregister_format_t register_req;	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_register_req_t;	/*^ API, RRM_SON_REGISTER_REQ ^*/

/*! \ rrm_son_deregister_req_t 
 *  \brief RRM SON DEREGISTER REQ Structure
 *  \param object             Object
 */
typedef struct _rrm_son_deregister_req_t
{
	rrm_son_register_deregister_format_t deregister_req;	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_deregister_req_t;	/*^ API,  RRM_SON_DEREGISTER_REQ ^*/

/*! \ rrm_son_set_attribute_value_req_t
 *  \brief RRM SON SET ATTRIBUTE VALUE Structure
 *  \param type            Object Type
 *  \param id              Object Id
 *  \param attr            Attribute
 */
typedef struct _rrm_son_set_attribute_value_req_t
{
    /* SPR 20653 Fix Start */
    object_t           object;		 /*^ M, 0, N, 0, 0 ^*/         
    /* SPR 20653 Fix End */
}rrm_son_set_attribute_value_req_t;	/*^ API,  RRM_SON_SET_ATTRIBUTE_REQ ^*/

/*Common structure for register, deregister and set attribute response */
/*! \ rrm_son_registration_based_res_t
 *  \brief RRM SON REGISTRATION BASED RESPONSE Structure
 *  \param object_id              Object Id
 *  \param result                 SUCCESS OR FAILURE
 *  \param error_code             Error Code
 */
typedef struct _rrm_son_registration_based_res_t
{
    rrm_object_id_t     id;		/*^ M, 0, N, 0, 0 ^*/
    rrm_return_et       result;        	/*^ M, 0, N, 0, 0 ^*/            /* Result */
    rrm_error_et   error_code;	/*^ M, 0, N, 0, 0 ^*/	         /* Error Code */
}rrm_son_registration_based_res_t;

/*Common structure for Non registration based response - cell switch off/on, nmm prepare and complete resp*/
/*! \ rrm_son_non_registration_based_res_t
 *  \brief RRM SON NON REGISTRATION BASED RESPONSE Structure
 *  \param cell_id                CGI of cell
 *  \param result                 SUCCESS OR FAILURE
 *  \param error_code             Error Code
 */
typedef struct _rrm_son_non_registration_based_res_t
{
    rrm_oam_eutran_global_cell_id_t   cell_id;		/*^ M, 0, N, 0, 0 ^*/
    rrm_return_et          result;              /*^ M, 0, N, 0, 0 ^*/   /* Result */
    rrm_error_et      error_code;          /*^ M, 0, N, 0, 0 ^*/   /* Error Code */
}rrm_son_non_registration_based_res_t;

/*! \ rrm_son_cell_switch_on_res_t
 *  \brief RRM SON CELL SWITCHON RESPONSE Structure
 *  \param cell_switchon_res	  Cell switch on response
 */
typedef struct _rrm_son_cell_switch_on_res_t
{
    rrm_son_non_registration_based_res_t cell_switchon_res;	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_cell_switch_on_res_t;	/*^ API,  RRM_SON_CELL_SWITCH_ON_RESP ^*/

/*! \ rrm_son_cell_switch_off_res_t
 *  \brief RRM SON CELL SWITCHOFF RESPONSE Structure
 *  \param cell_switchoff_res	  Cell switch off response
 */
typedef struct _rrm_son_cell_switch_off_res_t
{
    rrm_son_non_registration_based_res_t cell_switchoff_res;	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_cell_switch_off_res_t;	/*^ API,  RRM_SON_CELL_SWITCH_OFF_RESP ^*/

/*! \ rrm_son_registration_res_t
 *  \brief RRM REGISTRATION RESPONSE Structure
 *  \param registration_resp      Registration Resp
 */
typedef struct _rrm_son_registration_res_t
{
	rrm_son_registration_based_res_t    registration_resp;	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_registration_res_t;	/*^ API,  RRM_SON_REGISTER_RESP ^*/

/*! \ rrm_son_deregistration_res_t
 *  \brief RRM DEREGISTRATION RESPONSE Structure
 *  \param deregistration_resp      Deregistration Resp
 */
typedef struct _rrm_son_deregistration_res_t
{
	rrm_son_registration_based_res_t    deregistration_resp;	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_deregistration_res_t;	/*^ API,  RRM_SON_DEREGISTER_RESP ^*/

/*! \ rrm_son_set_attr_res_t
 *  \brief RRM SET ATTRIBUTE RESPONSE Structure
 *  \param set_attr_resp      Set Attribute Resp
 */
typedef struct _rrm_son_set_attr_res_t
{
	rrm_son_registration_based_res_t    set_attr_resp;	/*^ M, 0, N, 0, 0 ^*/
	rrm_attr_id_et       				id;	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_set_attr_res_t;	/*^ API,  RRM_SON_SET_ATTRIBUTE_RESP ^*/

/*! \ rrm_son_active_ue_cnt_report_t
 *  \brief RRM ES ACTIVE UE COUNT REPORT Structure
 *  \param cell_id                CGI of cell
 *  \param ue_cnt                 Number of active users
 */
typedef struct _rrm_son_active_ue_cnt_report_t
{
    rrm_oam_eutran_global_cell_id_t   cell_id;		/*^ M, 0, N, 0, 0 ^*/
    /* +- SPR 18268 */
    U16            ue_cnt;             		/*^ M, 0, N, 0, 0 ^*/
    /* +- SPR 18268 */
}rrm_son_active_ue_cnt_report_t;	/*^ API,  RRM_SON_NUM_ACTIVE_UE_REPORT ^*/

/*! \ rrm_ue_count_threshold_hit_ind_t
 *  \brief RRM UE COUNT THRESHOLD HIT INDICATION Structure
 *  \param cell_id                CGI of cell
 *  \param threshold_reached_cause  Above to below OR below to above 
 */
typedef struct _rrm_ue_count_threshold_hit_ind_t
{
   rrm_oam_eutran_global_cell_id_t        		 cell_id;   	/*^ M, 0, N, 0, 0 ^*/       
   rrm_threshold_reached_cause_et      cause;			/*^ M, 0, N, 0, 0 ^*/
}rrm_ue_count_threshold_hit_ind_t;	/*^ API,  RRM_UE_COUNT_THRESHOLD_REACHED_IND ^*/

/*! \ rrm_son_cell_switch_off_req_t 
 *  \brief RRM ES Cell Switch off request Structure    
 *  \param cell_id                CGI of cell
 *  \param switchoff_type          Forced or graceful switchoff
 *  \param switchoff_time	  time to switch off
 */
typedef struct _rrm_son_cell_switch_off_req_t
{
    rrm_oam_eutran_global_cell_id_t    cell_id; 		/*^ M, 0, N, 0, 0 ^*/   
    rrm_switchoff_et            switchoff_type;		/*^ M, 0, N, 0, 0 ^*/
    U32				switchoff_time;		/*^ M, 0, N, 0, 0 ^*/
}rrm_son_cell_switch_off_req_t;		/*^ API,  RRM_SON_CELL_SWITCH_OFF_REQ ^*/

/*! \  rrm_son_cell_switch_on_req_t 
 *  \brief RRM ES Cell Switch on request Structure    
 *  \param cell_id                CGI of cell
 */

typedef struct _rrm_son_cell_switch_on_req_t
{
    rrm_oam_eutran_global_cell_id_t       cell_id;    		/*^ M, 0, N, 0, 0 ^*/
}rrm_son_cell_switch_on_req_t;		/*^ API,  RRM_SON_CELL_SWITCH_ON_REQ ^*/

/*
 *  RRM_SON_CELL_STATE_CHANGE_IND  
 */
/*! \ rrm_son_cell_state_change_ind_t
 *  \brief Cell State Change Notification
 *  \param cell_id                          CGI of cell
 *  \param cell_state                       Cell State
 *  \param cell_state_change_cause          Cell State Change Cause
 */
typedef struct _rrm_son_cell_state_change_ind_t
{
    rrm_oam_eutran_global_cell_id_t                cell_id;		/*^ M, 0, N, 0, 0 ^*/
    rrm_cell_operational_state_et   	cell_state;      	/*^ M, 0, N, 0, 0 ^*/
    rrm_cell_state_change_cause_et  cell_state_change_cause; 	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_cell_state_change_ind_t;	/*^ API,  RRM_SON_CELL_STATE_CHANGE_IND ^*/

/*Common structure for nmm prepare and complete resp*/
/*! \ rrm_son_nmm_res_t
 *  \brief RRM SON NMM PREPARE AND COMPLETE RESPONSE Structure
 *  \param result                 SUCCESS OR FAILURE
 *  \param error_code             Error Code
 */
typedef struct _rrm_son_nmm_res_t
{
    rrm_return_et          result;              /*^ M, 0, N, 0, 0 ^*/   /* Result */
    rrm_error_et      error_code;          /*^ M, 0, N, 0, 0 ^*/   /* Error Code */
}rrm_son_nmm_res_t;		

/*! \  rrm_son_nmm_prepare_res_t
 *  \brief RRM SON NMM PREPARE RESPONSE Structure
 *  \param prepare_res	  NMM Prepare response
 */
typedef struct _rrm_son_nmm_prepare_res_t
{
    rrm_son_nmm_res_t prepare_res;	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_nmm_prepare_res_t;	/*^ API,  RRM_SON_NMM_PREPARE_RESP ^*/

/*! \  rrm_son_nmm_complete_res_t
 *  \brief RRM SON NMM COMPLETE RESPONSE Structure
 *  \param complete_res	  NMM complete response
 */
typedef struct _rrm_son_nmm_complete_res_t
{
    rrm_son_nmm_res_t complete_res;	/*^ M, 0, N, 0, 0 ^*/
}rrm_son_nmm_complete_res_t; 	/*^ API,  RRM_SON_NMM_COMPLETE_RESP^*/

/* UE_MEAS CHANGES : STARTS */
/*! \ rrm_son_speed_state_scale_factors_t
 *  \brief This struct contains speed state scale factors
 *  \param sf_medium scale factor med
 *  \param sf_high   scale factor high
 */
typedef struct _rrm_son_speed_state_scale_factors_t
{
    rrm_speed_state_scale_factors_sf_medium_et sf_medium;   
    /*^ M, 0, H, 0, 3 ^*/
    rrm_speed_state_scale_factors_sf_high_et   sf_high;     
    /*^ M, 0, H, 0, 3 ^*/
}rrm_son_speed_state_scale_factors_t;

#define RRM_SON_EUTRAN_MEAS_BANDWIDTH_PRESENT                0x01
#define RRM_SON_EUTRAN_PRESENCE_ANTENNAE_PORT_PRESENT        0x02
#define RRM_SON_EUTRAN_NEIGHBOR_CELL_CONFIG_PRESENT          0x04
#define RRM_SON_EUTRAN_OFFSET_FREQUENCY_PRESENT              0x08
#define RRM_SON_EUTRAN_Q_RX_LEV_MIN_PRESENT                  0x10
#define RRM_SON_EUTRAN_P_MAX_PRESENT                         0x20
#define RRM_SON_EUTRAN_RESELECTION_PRESENT                   0x40
#define RRM_SON_EUTRAN_RESELECTION_SF_PRESENT                0x80
#define RRM_SON_EUTRAN_THRESHOLD_HIGH_PRESENT                0x100
#define RRM_SON_EUTRAN_THRESHOLD_LOW_PRESENT                 0x200
#define RRM_SON_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT     0x400
#define RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT           0x800
#define RRM_SON_EUTRAN_Q_QUAL_MIN_R9                         0x1000
#define RRM_SON_EUTRAN_THRESHX_HIGH_Q_R9                     0x2000
#define RRM_SON_EUTRAN_THRESHX_LOW_Q_R9                      0x4000
/* OFFSET FREQ CHANGES START */
#define RRM_SON_EUTRAN_CM_OFFSET_FREQUENCY_PRESENT           0x8000
/* OFFSET FREQ CHANGES END */
/*! \ rrm_son_meas_config_eutran_t
 *  \brief This structure measures rrm son config eutran
 *  \param bitmask bitmask
 *  \param earfcn 
 *  \param meas_bandwidth measured bw
 *  \param presence_antenna_port presence antenna port
 *  \param neighbor_cell_config  neighbor cell config
 *  \param offset_frequency      offset frequency
 *  \param q_rx_lev_min          q rx minimum level
 *  \param p_max                 RRM_SON_EUTRAN_P_MAX_PRESENT
 *  \param t_reselection         reselection 
 *  \param t_reselection_sf      reselection scale factor
 *  \param threshX_high          threshold high
 *  \param threshX_low           threshold low
 *  \param cell_reselection_priority  cell reselection priority
 *  \param cells_for_which_to_report_cgi_size cells for which to report cgi size
 *  \param cells_for_which_to_report_cgi[MAX_SON_MEAS_CONFIG_OBJECT] cells for which to report cgi
 *  \param q_qualmin_r9 
 *  \param threshx_highq_r9
 *  \param threshx_lowq_r9
 */ 
typedef struct _rrm_son_meas_config_eutran_t
{
    rrm_bitmask_t                   bitmask;
    /*^ BITMASK ^*/
    U32                             earfcn;                    
    /*^ M, 0, N, 0, 0 ^*/
    rrm_allowed_meas_bandwidth_et   meas_bandwidth;            
    /*^ O, RRM_SON_EUTRAN_MEAS_BANDWIDTH_PRESENT, H, 0, 5 ^*/
    U8                              presence_antenna_port;     
    /*^ O, RRM_SON_EUTRAN_PRESENCE_ANTENNAE_PORT_PRESENT, H, 0, 1 ^*/
    U8                              neighbor_cell_config;
    /*^ O, RRM_SON_EUTRAN_NEIGHBOR_CELL_CONFIG_PRESENT, N, 0, 0 ^*/
    rrm_q_offsetrange_et            offset_frequency;          
    /*^ O, RRM_SON_EUTRAN_OFFSET_FREQUENCY_PRESENT, H, 0, 30 ^*/
    S8                              q_rx_lev_min;              
    /*^ O, RRM_SON_EUTRAN_Q_RX_LEV_MIN_PRESENT, B, -70, -22 ^*/
    S8                              p_max;                     
    /*^ O, RRM_SON_EUTRAN_P_MAX_PRESENT, B, -30, 33 ^*/
    U8                              t_reselection;             
    /*^ O, RRM_SON_EUTRAN_RESELECTION_PRESENT, H, 0, 7 ^*/
    rrm_son_speed_state_scale_factors_t t_reselection_sf;          
    /*^ O, RRM_SON_EUTRAN_RESELECTION_SF_PRESENT, N, 0, 0 ^*/
    U8                              threshX_high;              
    /*^ O, RRM_SON_EUTRAN_THRESHOLD_HIGH_PRESENT, H, 0, 31 ^*/
    U8                              threshX_low;               
    /*^ O, RRM_SON_EUTRAN_THRESHOLD_LOW_PRESENT, H, 0, 31 ^*/
    U8                              cell_reselection_priority; 
    /*^ O, RRM_SON_EUTRAN_CELL_RESELECTION_PRIORITY_PRESENT, H, 0, 7 ^*/
    U8                              cells_for_which_to_report_cgi_size;      
/* SPR-18445 start */
    /*^ O, RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT, B, 1, MAX_SON_MEAS_CONFIG_OBJECT ^*/
/* SPR-18445 stop */
    U16                             cells_for_which_to_report_cgi[MAX_SON_MEAS_CONFIG_OBJECT];   
    /*^ O, RRM_SON_EUTRAN_CELLS_TO_REPORT_CGI_PRESENT, OCTET_STRING, VARIABLE ^*/
    S8                              q_qualmin_r9;
    /*^ O, RRM_SON_EUTRAN_Q_QUAL_MIN_R9, B, -34, -3 ^*/
    U8                              threshx_highq_r9;
    /*^ O, RRM_SON_EUTRAN_THRESHX_HIGH_Q_R9, H, 0, 31 ^*/
    U8                              threshx_lowq_r9;
    /*^ O, RRM_SON_EUTRAN_THRESHX_LOW_Q_R9, H, 0, 31 ^*/
    /* OFFSET FREQ CHANGES START */
    rrm_q_offsetrange_et            cm_offset_frequency;
    /*^ O, RRM_SON_EUTRAN_CM_OFFSET_FREQUENCY_PRESENT, H, 0, 30 ^*/
    /* OFFSET FREQ CHANGES END */

}rrm_son_meas_config_eutran_t;

#define RRM_SON_UTRAN_PCI_FDD_RANGE_PRESENT 0x01
/*! \  rrm_son_utran_pci_fdd_range_t
 *  \brief This struct contains info about fdd range
 *  \param bitmask bitmask
 *  \param start start
 *  \param range range 
*/
typedef struct _rrm_son_utran_pci_fdd_range_t
{
    rrm_bitmask_t      bitmask;  /*^ BITMASK ^*/
    U16                start;    
    /*^ M, 0, H, 0, 511 ^*/
    U16                range;    
    /*^ O, RRM_SON_UTRAN_PCI_FDD_RANGE_PRESENT, B, 2, 512 ^*/
}rrm_son_utran_pci_fdd_range_t;

#define RRM_SON_UTRAN_OFFSET_FREQUENCY_PRESENT                0x01
#define RRM_SON_UTRAN_CELL_RESELECTION_PRIORITY_PRESENT       0x02
#define RRM_SON_UTRAN_THRESHOLD_HIGH_PRESENT                  0x04
#define RRM_SON_UTRAN_THRESHOLD_LOW_PRESENT                   0x08
#define RRM_SON_UTRAN_Q_RX_LEV_MIN_PRESENT                    0x10
#define RRM_SON_UTRAN_P_MAX_PRESENT                           0x20
#define RRM_SON_UTRAN_Q_QUAL_MIN_PRESENT                      0x40
#define RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT          0x80
#define RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT             0x100  
#define RRM_SON_UTRAN_THRESHX_HIGH_Q_R9                       0x200
#define RRM_SON_UTRAN_THRESHX_LOW_Q_R9                        0x400
/*! \ rrm_son_meas_config_utran_fdd_t
 *  \brief This struct measures config utran fdd
 *  \param bitmask bitmask
 *  \param uarfcn  uarfcn
 *  \param offset_frequency offset frequency
 *  \param cell_reselection_priority cell reselection priority
 *  \param threshX_high high threshold
 *  \param threshX_low low threshold
 *  \param q_rx_lev_min rx minimum level
 *  \param p_max  p_max
 *  \param q_qual_min q qual minimum
 *  \param csg_allowed_reporting_cell_list_size
 *  \param csg_allowed_reporting_cell_list[MAX_PCI_RANGE]
 *  \param cells_for_which_to_report_cgi_size
 *  \param cells_for_which_to_report_cgi[MAX_SON_MEAS_CONFIG_OBJECT] cells for which to report cgi
 *  \param threshx_highq_r9 
 *  \param threshx_lowq_r9
*/
typedef struct _rrm_son_meas_config_utran_fdd_t
{
    rrm_bitmask_t                   bitmask;                   /*^ BITMASK ^*/
    U16                             uarfcn;                    
    /*^ M, 0, H, 0,16383  ^*/
    S8                              offset_frequency;          
    /*^ O, RRM_SON_UTRAN_OFFSET_FREQUENCY_PRESENT, B, -15, 15 ^*/
    U8                              cell_reselection_priority; 
    /*^ O, RRM_SON_UTRAN_CELL_RESELECTION_PRIORITY_PRESENT, H, 0, 7 ^*/
    U8                              threshX_high;              
    /*^ O, RRM_SON_UTRAN_THRESHOLD_HIGH_PRESENT, H, 0, 31 ^*/
    U8                              threshX_low;               
    /*^ O, RRM_SON_UTRAN_THRESHOLD_LOW_PRESENT, H, 0, 31 ^*/
    S8                              q_rx_lev_min;              
    /*^ O, RRM_SON_UTRAN_Q_RX_LEV_MIN_PRESENT, B, -60, -13 ^*/
    S8                              p_max;                     
    /*^ O, RRM_SON_UTRAN_P_MAX_PRESENT, B, -50, 33 ^*/
    S8                              q_qual_min;                
    /*^ O, RRM_SON_UTRAN_Q_QUAL_MIN_PRESENT, B, -24, 0 ^*/
    U8                              csg_allowed_reporting_cell_list_size;

/*SPR_17664_start*/
    /*^ O, RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT , B, 1, MAX_PCI_RANGE ^*/

/*SPR_17664_start*/
    rrm_son_utran_pci_fdd_range_t   csg_allowed_reporting_cell_list[MAX_PCI_RANGE];
    /*^ O, RRM_SON_UTRAN_FDD_CSG_REPORTING_CELL_PRESENT , OCTET_STRING, VARIABLE ^*/
    U8                              cells_for_which_to_report_cgi_size;      

/*SPR_17664_start*/
    /*^ O, RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT, B, 1, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    U16                             cells_for_which_to_report_cgi[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ O, RRM_SON_UTRAN_CELLS_TO_REPORT_CGI_PRESENT, OCTET_STRING, VARIABLE ^*/
    U8                              threshx_highq_r9;
    /*^ O, RRM_SON_UTRAN_THRESHX_HIGH_Q_R9, H, 0, 31 ^*/
    U8                              threshx_lowq_r9;
    /*^ O, RRM_SON_UTRAN_THRESHX_LOW_Q_R9, H, 0, 31 ^*/
}rrm_son_meas_config_utran_fdd_t;

/* TDD Support SON Start */
#define RRM_SON_UTRAN_TDD_OFFSET_FREQUENCY_PRESENT                0x01
#define RRM_SON_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT       0x02
#define RRM_SON_UTRAN_TDD_THRESHOLD_HIGH_PRESENT                  0x04
#define RRM_SON_UTRAN_TDD_THRESHOLD_LOW_PRESENT                   0x08
#define RRM_SON_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT                    0x10
#define RRM_SON_UTRAN_TDD_P_MAX_PRESENT                           0x20
#define RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT             0x40  
/*! \ rrm_son_meas_config_utran_tdd_t
 *  \brief This struct measures config utran tdd
*/
typedef struct _rrm_son_meas_config_utran_tdd_t
{
    rrm_bitmask_t                   bitmask;                   /*^ BITMASK ^*/
    U16                             uarfcn;                    
    /*^ M, 0, H, 0,16383  ^*/
    S8                              offset_frequency;          
    /*^ O, RRM_SON_UTRAN_TDD_OFFSET_FREQUENCY_PRESENT, B, -15, 15 ^*/
    U8                              cell_reselection_priority; 
    /*^ O, RRM_SON_UTRAN_TDD_CELL_RESELECTION_PRIORITY_PRESENT, H, 0, 7 ^*/
    U8                              threshX_high;              
    /*^ O, RRM_SON_UTRAN_TDD_THRESHOLD_HIGH_PRESENT, H, 0, 31 ^*/
    U8                              threshX_low;               
    /*^ O, RRM_SON_UTRAN_TDD_THRESHOLD_LOW_PRESENT, H, 0, 31 ^*/
    S8                              q_rx_lev_min;              
    /*^ O, RRM_SON_UTRAN_TDD_Q_RX_LEV_MIN_PRESENT, B, -60, -13 ^*/
    S8                              p_max;                     
    /*^ O, RRM_SON_UTRAN_TDD_P_MAX_PRESENT, B, -50, 33 ^*/
    U8                              cells_for_which_to_report_cgi_size;      

/*SPR_17664_start*/
    /*^ O, RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT, B, 1, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    U16                             cells_for_which_to_report_cgi[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ O, RRM_SON_UTRAN_TDD_CELLS_TO_REPORT_CGI_PRESENT, OCTET_STRING, VARIABLE ^*/
}rrm_son_meas_config_utran_tdd_t;
/* TDD Support SON End */

#define RRM_SON_UTRAN_RESELECTION_PRESENT       0x01
#define RRM_SON_UTRAN_RESELECTION_SF_PRESENT    0x02
/*! \ rrm_son_meas_config_utran_t
 *  \brief This struct measures utran configuration
 *  \param bitmask bitmask
 *  \param utran_fdd_list_size utran fdd list size
 *  \param utran_fdd_list[MAX_SON_MEAS_CONFIG_OBJECT] utran fdd list
 *  \param utran_tdd_list[MAX_SON_MEAS_CONFIG_OBJECT] utran tdd list
 *  \param t_reselection reselection
 *  \param t_reselection_sf reselection scale factor
*/
typedef struct _rrm_son_meas_config_utran_t
{
    rrm_bitmask_t                   bitmask;                                        /*^ BITMASK ^*/
    U8                              utran_fdd_list_size;                            
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End */
    rrm_son_meas_config_utran_fdd_t utran_fdd_list[MAX_SON_MEAS_CONFIG_OBJECT]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                              utran_tdd_list_size;                            
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End */
    rrm_son_meas_config_utran_tdd_t utran_tdd_list[MAX_SON_MEAS_CONFIG_OBJECT]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                              t_reselection;                                  
    /*^ O, RRM_SON_UTRAN_RESELECTION_PRESENT, H, 0, 7 ^*/
    rrm_son_speed_state_scale_factors_t     t_reselection_sf;                           
    /*^ O, RRM_SON_UTRAN_RESELECTION_SF_PRESENT, N, 0, 0 ^*/
}rrm_son_meas_config_utran_t;

#define RRM_SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE   31
#define RRM_SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP 16
/*! \ rrm_son_geran_physical_cell_id_t
 *  \brief This struct generate physicall cell id
 *  \param ncc  ncc
 *  \param bscc bscc
*/
typedef struct _rrm_son_geran_physical_cell_id_t
{
    U8          ncc;
    /*^ M, 0, H, 0, 7 ^*/
    U8          bscc;
    /*^ M, 0, H, 0, 7 ^*/
}rrm_son_geran_physical_cell_id_t;

#define RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT       0x01
#define RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT     0x02
#define RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT   0x04
/*! \ rrm_son_geran_following_arfcns_t
 *  \brief This struct generate following arfcns
 *  \param bitmask bitmask
 *  \param exp_arfcn_list_size exp arfcn list size
 *  \param exp_arfcn_list[RRM_SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE] exp arfcn list
 *  \param arfcn_spacing arfcn spacing
 *  \param num_of_following_arfcn num of following arfcn
 *  \param arfcn_bmp_list_size arfcn bmp list size
 *  \param arfcn_bmp_list[RRM_SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP] arfcn bmp list
*/
typedef struct _rrm_son_geran_following_arfcns_t
{
   rrm_bitmask_t        bitmask;/*^ BITMASK ^*/
   U8                   exp_arfcn_list_size;

/*SPR_17664_start*/
   /*^ O, RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT, B, 1, RRM_SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE ^*/

/*SPR_17664_end*/
   U16                  exp_arfcn_list[RRM_SON_MAX_GERAN_EXP_ARFCN_LIST_SIZE];
   /*^ O, RRM_SON_MC_GERAN_EXP_ARFCN_SET_PRESENT, OCTET_STRING, VARIABLE ^*/
   U8                   arfcn_spacing;
   /*^ O, RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT, B, 1, 8  ^*/
   U8                   num_of_following_arfcn;
   /*^ O, RRM_SON_MC_GERAN_EQ_SP_ARFCN_SET_PRESENT, H, 0, 31 ^*/
   U8                   arfcn_bmp_list_size;

/*SPR_17664_start*/
   /*^ O, RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT, B, 1, RRM_SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP^*/

/*SPR_17664_end*/
   U8                   arfcn_bmp_list[RRM_SON_MAX_NUM_OF_OCTETS_IN_VAR_BITMAP];
   /*^ O, RRM_SON_MC_GERAN_ARFCN_BITMAP_SET_PRESSENT, OCTET_STRING, VARIABLE ^*/
}rrm_son_geran_following_arfcns_t;

/*! \ rrm_son_geran_carrier_freqs_t
 *  \brief This struct generate carrier freq
 *  \param starting_arfcn starting arfcn
 *  \param band_ind band index
 *  \param following_arfcns following arfcns
*/
typedef struct _rrm_son_geran_carrier_freqs_t
{
    U16                               starting_arfcn;
    /*^ M, 0, H, 0, 1023 ^*/
    rrm_son_geran_band_indicator_et   band_ind;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_son_geran_following_arfcns_t  following_arfcns;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_son_geran_carrier_freqs_t;

#define RRM_SON_GERAN_CFI_CAR_FREQS_PRESENT                       0x01
#define RRM_SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT       0x02
#define RRM_SON_GERAN_CFI_NCC_PERMITTED_PRESENT                   0x04
#define RRM_SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT                    0x08 
#define RRM_SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT           0x10
#define RRM_SON_GERAN_CFI_THRESHX_HIGH_PRESENT                    0x20
#define RRM_SON_GERAN_CFI_THRESHX_LOW_PRESENT                     0x40
#define RRM_SON_GERAN_CFI_OFFSET_FREQ_PRESENT                     0x80
#define RRM_SON_GERAN_CFI_MEAS_OBJ_ID_PRESENT                     0x100
#define RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT             0x200
/*! \ rrm_son_geran_carrier_freq_info_t
 *  \brief This struct henerate carrier frquency info
 * \param bitmask bitmask
 * \param geran_car_freqs geran carrier frequency
 * \param cell_reselection_priority cell reselection frequency
 * \param ncc_permitted ncc permitted
 * \param q_rx_lev_min q rx level minimum
 * \param p_max_geran max geran
 * \param  threshX_high threshold high
 * \param threshX_low
 * \param offset_frequency
 * \param meas_obj_id
 * \param cells_for_which_to_report_cgi_size
 * \param cells_for_which_to_report_cgi[MAX_SON_MEAS_CONFIG_OBJECT]
*/

typedef struct _rrm_son_geran_carrier_freq_info_t
{
    rrm_bitmask_t                    bitmask;  /*^ BITMASK ^*/
    rrm_son_geran_carrier_freqs_t    geran_car_freqs;
        /*^ O, RRM_SON_GERAN_CFI_CAR_FREQS_PRESENT, N, 0, 0 ^*/                   
    U8                              cell_reselection_priority;
        /*^ O, RRM_SON_GERAN_CFI_CELL_RESELECTION_PRIORITY_PRESENT, M, 0, 7 ^*/
    U8                              ncc_permitted;
        /*^ O, RRM_SON_GERAN_CFI_NCC_PERMITTED_PRESENT, N, 0, 0 ^*/ 
    U8                              q_rx_lev_min;
        /*^ O,RRM_SON_GERAN_CFI_Q_RX_LEV_MIN_PRESENT, H, 0, 45 ^*/ 
    U8                              p_max_geran;
        /*^ O, RRM_SON_GERAN_CFI_MAX_ALLOWED_TRANS_PWR_PRESENT, H, 0, 39 ^*/
    U8                              threshX_high;
        /*^ O, RRM_SON_GERAN_CFI_THRESHX_HIGH_PRESENT, H, 0, 31 ^*/
    U8                              threshX_low;
        /*^ O, RRM_SON_GERAN_CFI_THRESHX_LOW_PRESENT, H, 0, 31 ^*/
    S8                              offset_frequency;
        /*^ O, RRM_SON_GERAN_CFI_OFFSET_FREQ_PRESENT, B, -15, 15 ^*/
    U8                               meas_obj_id;
        /*^ O, RRM_SON_GERAN_CFI_MEAS_OBJ_ID_PRESENT, B, 1, 12 ^*/
    U8                              cells_for_which_to_report_cgi_size;      

/*SPR_17664_start*/
        /*^ O, RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT, B, 1, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    rrm_son_geran_physical_cell_id_t        cells_for_which_to_report_cgi[MAX_SON_MEAS_CONFIG_OBJECT];
        /*^ O, RRM_SON_GERAN_CFI_CELLS_TO_REPORT_CGI_PRESENT, OCTET_STRING, VARIABLE ^*/
}rrm_son_geran_carrier_freq_info_t;


#define RRM_SON_MC_GERAN_T_RESELECTION_PRESENT               0x01
#define RRM_SON_MC_GERAN_T_RESELECTION_SF_PRESENT            0x02
/*! \ rrm_son_meas_config_geran_t
 *  \brief This struct measures config geran
 *  \param bitmask bitmask
 *  \param geran_freq_info_list_size  frequency info list size
 *  \param t_reselection reselection
 *  \param t_reselection_sf reselection scale factor
*/
typedef struct _rrm_son_meas_config_geran_t
{
    rrm_bitmask_t                  bitmask;   /*^ BITMASK ^*/
    U8                                  geran_freq_info_list_size;                                
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End */
    rrm_son_geran_carrier_freq_info_t   geran_freq_info_list[MAX_SON_MEAS_CONFIG_OBJECT];     
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                  t_reselection;                                      
    /*^ O, RRM_SON_MC_GERAN_T_RESELECTION_PRESENT, H, 0, 7 ^*/
    rrm_son_speed_state_scale_factors_t t_reselection_sf;                                   
    /*^ O, RRM_SON_MC_GERAN_T_RESELECTION_SF_PRESENT, N, 0, 0 ^*/        
}rrm_son_meas_config_geran_t;

#define RRM_SON_CDMA_PRE_REGISTRATION_INFO_HRPD_PRE_REG_ZONE_PRESENT            0x01
#define RRM_SON_CDMA_PRE_REGISTRATION_INFO_HRPD_SEC_PRE_REG_ZONE_LIST_PRESET     0x02

#define SON_CDMA_SEC_P_REG_LIST_SIZE    2
/*! \ rrm_son_cdma_preRegistrationInfoHRPD_t
 *  \brief This struct contains son cdma preRegistration info
 *  \param bitmask bitmask
 *  \param preRegistrationAllowed whether preregistration is allowed or not
 *  \param PreRegistrationZoneIdHRPD preregistration zone
 *  \param SecondaryPreRegistrationZoneIdListHRPD[SON_CDMA_SEC_P_REG_LIST_SIZE] preregistration zone id list
*/
typedef struct _rrm_son_cdma_preRegistrationInfoHRPD_t
{
    /*SPR 22325 +-*/
    rrm_bitmask_t	bitmask;                     /*^ BITMASK ^*/
    rrm_bool_et             preRegistrationAllowed;
    /*^ M, 0, N, 0, 0 ^*/
    U32             PreRegistrationZoneIdHRPD;
    /*^ O, RRM_SON_CDMA_PRE_REGISTRATION_INFO_HRPD_PRE_REG_ZONE_PRESENT, H, 0, 255 ^*/ 
    U32             SecondaryPreRegistrationZoneIdListHRPD[SON_CDMA_SEC_P_REG_LIST_SIZE];
    /*^ O, RRM_SON_CDMA_PRE_REGISTRATION_INFO_HRPD_SEC_PRE_REG_ZONE_LIST_PRESET, H, 0, 255 ^*/
}rrm_son_cdma_preRegistrationInfoHRPD_t;

#define RRM_SON_CDMA_BANDCLASSLIST_CDMA2000_BAND_CLASS_PRESET         0x01
/*! \  rrm_son_cdma_BandClassListCDMA2000_t
 *  \brief This struct contains info about cdma band class list
 *  \param bitmask bitmask
 *  \param bandClass bandClass
 *  \param cellReselectionPriority cell Reselection Priority
 *  \param thresholdx_high thresholdx high
 *  \param thresholdx_low  thresholdx low
*/
typedef struct _rrm_son_cdma_BandClassListCDMA2000_t
{
    /*SPR 22325 +-*/
    rrm_bitmask_t	bitmask;                     /*^ BITMASK ^*/
    rrm_son_cdma_band_class_et              bandClass;
    /*^ M, 0, H, 0, 31 ^*/            
    U32                                     cellReselectionPriority;
    /*^ O, RRM_SON_CDMA_BANDCLASSLIST_CDMA2000_BAND_CLASS_PRESET, H, 0, 7 ^*/
    U32                                     thresholdx_high;
    /*^ M, 0, H, 0, 63 ^*/
    U32                                     thresholdx_low;
    /*^ M, 0, H, 0, 63 ^*/
}rrm_son_cdma_BandClassListCDMA2000_t;

#define ARFCN_CDMA_SIZE                 2047
#define PHYSICALCELLIDCDMA2000          511
/*! \ rrm_son_cdma_NeighCellsPerBandclassCDMA2000_t
 *  \brief This struct contains info about the neighbouring cells per band class
 *  \param  arfcn_valCDMA[ARFCN_CDMA_SIZE]
 *  \param  pci_cdma[PHYSICALCELLIDCDMA2000]
*/
typedef struct _rrm_son_cdma_NeighCellsPerBandclassCDMA2000_t
{
    U32                                     arfcn_valCDMA[ARFCN_CDMA_SIZE];
    /*^ M, 0, H, 0, 2047 ^*/
    U32                                     pci_cdma[PHYSICALCELLIDCDMA2000];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_son_cdma_NeighCellsPerBandclassCDMA2000_t;

#define CDMA_NEG_CELL_BAND_CLASS_SIZE   16
/*! \  rrm_son_cdma_NeighCellsPerBandclassListCDMA2000_t
 *  \brief This struct contains info about the neighbouring cells per band class list
 *  \param neigh_cell_band_list_size
 *  \param  NeighCellsPerBandclassCDMA2000[CDMA_NEG_CELL_BAND_CLASS_SIZE]
*/
typedef struct _rrm_son_cdma_NeighCellsPerBandclassListCDMA2000_t
{
    U8                                      neigh_cell_band_list_size;

/*SPR_17664_start*/
    /*^ M, 0, H, 0, CDMA_NEG_CELL_BAND_CLASS_SIZE ^*/

/*SPR_17664_end*/
    rrm_son_cdma_NeighCellsPerBandclassCDMA2000_t   NeighCellsPerBandclassCDMA2000[CDMA_NEG_CELL_BAND_CLASS_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_son_cdma_NeighCellsPerBandclassListCDMA2000_t;

/*! \ rrm_son_cdma_NeighCellCDMA2000_t
 *  \brief This struct contains info about the neighbouring cells
 *  \param bandClass band class
 *  \param NeighCellsPerBandclassListCDMA2000  Neighbour Cells Per Bandclass List CDMA
*/
typedef struct _rrm_son_cdma_NeighCellCDMA2000_t
{
    rrm_son_cdma_band_class_et                          bandClass;
    /*^ M, 0, H, 0, 31 ^*/
    rrm_son_cdma_NeighCellsPerBandclassListCDMA2000_t   NeighCellsPerBandclassListCDMA2000;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_son_cdma_NeighCellCDMA2000_t;
/*! \ rrm_son_cdma_NeighCellListCDMA2000_t
 *  \brief This struct contains info about the neighbouring cells list
 *  \param nbr_cell_list_size neighbour cell list size
 *  \param NeighCellCDMA2000[CDMA_NEG_CELL_BAND_CLASS_SIZE] 
*/
typedef struct _rrm_son_cdma_NeighCellListCDMA2000_t
{
    U8                          nbr_cell_list_size;
                 
/*SPR_17664_start*/
    /*^ M, 0, H, 0, CDMA_NEG_CELL_BAND_CLASS_SIZE ^*/

/*SPR_17664_end*/
    rrm_son_cdma_NeighCellCDMA2000_t    NeighCellCDMA2000[CDMA_NEG_CELL_BAND_CLASS_SIZE];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_son_cdma_NeighCellListCDMA2000_t;
/* ! \enum rrm_son_cdma_sf_et
 *   \brief rrm son sf info
*/
typedef enum 
{
    son_oDot25,
    son_oDot5,
    son_oDot75,
    son_lDot0
}rrm_son_cdma_sf_et;

/*! \ rrm_son_cdma_SpeedStateScaleFactors_t
 *  \brief this struct contains info about cdma speed sf
 *  \param m_sf med scal factor
 *  \param h_sf high scal factor
*/
typedef struct _rrm_son_cdma_SpeedStateScaleFactors_t
{
    rrm_son_cdma_sf_et  m_sf;
    /*^ M, 0, H, 0, 3 ^*/
    rrm_son_cdma_sf_et  h_sf;
    /*^ M, 0, H, 0, 3 ^*/
}rrm_son_cdma_SpeedStateScaleFactors_t;

#define RRM_SON_CDMA_CELLRESELECTIONPARAMHRPD_RESELECT_SF    0x01
/*! \ rrm_son_cdma_cellReselectionParametersHRPD_t
 *  \brief This struct contains info about cell reselection parameters
 *  \param bitmask bitmask
 *  \param bandClassList bandClasslist
 *  \param neighCellList neighbour cell list
 *  \param reselection reselection
 *  \param reselectionCDMA2000_sf reselction sf
*/
typedef struct _rrm_son_cdma_cellReselectionParametersHRPD_t
{
    /*SPR 22325 +-*/
    rrm_bitmask_t	bitmask;                     /*^ BITMASK ^*/
    rrm_son_cdma_BandClassListCDMA2000_t    bandClassList;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_son_cdma_NeighCellListCDMA2000_t    neighCellList;
    /*^ M, 0, N, 0, 0 ^*/
    U32                                 reselection;
    /*^ M, 0, H, 0, 7 ^*/
    rrm_son_cdma_SpeedStateScaleFactors_t   reselectionCDMA2000_sf;
    /*^ O, RRM_SON_CDMA_CELLRESELECTIONPARAMHRPD_RESELECT_SF, N, 0, 0 ^*/
}rrm_son_cdma_cellReselectionParametersHRPD_t;

#define RRM_SON_CDMA_PARA_HRPD_RESELECTION_PARAM_HRPD_PRESET    0x01
/*! \  rrm_son_cdma_para_hrpd_t
 *  \brief This struct contains registeration params
 *  \param bitmask bitmask
 *  \param preRegistrationInfoHRPD pre Registration Info HRPD
 *  \param cellReselectionParametersHRPD cell Reselection Parameters HRPD
*/
typedef struct _rrm_son_cdma_para_hrpd_t
{
    /*SPR 22325 +-*/
    rrm_bitmask_t	bitmask;                     /*^ BITMASK ^*/
    rrm_son_cdma_preRegistrationInfoHRPD_t          preRegistrationInfoHRPD;
    /*^ M, 0, N, 0, 0  ^*/     
    rrm_son_cdma_cellReselectionParametersHRPD_t    cellReselectionParametersHRPD;
    /*^ O, RRM_SON_CDMA_PARA_HRPD_RESELECTION_PARAM_HRPD_PRESET, N, 0, 0 ^*/
}rrm_son_cdma_para_hrpd_t;

#define CDMA_SID_BYTE                   16
#define CDMA_NID_BYTE                   16
#define CDMA_REG_ZONE                   2
/*! \  rrm_son_cdma_csfb_registration_param_1XRTT_t
 *  \brief This struct contains info about cdma csfb registration params
 *  \param sid[CDMA_SID_BYTE] sid
 *  \param nid[CDMA_NID_BYTE] nid
 *  \param multiple_sid multiple sid
 *  \param multiple_nid multiple nid
 *  \param home_reg     home reg
 *  \param foreign_sid_reg foreign sid reg
 *  \param foreign_nid_reg foreign nid reg
 *  \param parameter_reg parameter reg
 *  \param power_up_reg power up reg
 *  \param reg_period  reg period 
 *  \param reg_zone[CDMA_REG_ZONE] reg zone
 *  \param total_zone total zone
 *  \param zone_timer zone timer
*/
typedef struct _son_cdma_csfb_registration_param_1XRTT_t
{
    U8                                          sid[CDMA_SID_BYTE];
    /*^ M, 0, N, 0, 0 ^*/
    U8                                          nid[CDMA_NID_BYTE];
    /*^ M, 0, N, 0, 0 ^*/
    rrm_bool_et                                 multiple_sid;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 multiple_nid;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 home_reg;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 foreign_sid_reg;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 foreign_nid_reg;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 parameter_reg;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 power_up_reg;
    /*^ M, 0, H, 0, 1 ^*/
    U8                                          reg_period;
    /*^ M, 0, N, 0, 0 ^*/
    U8                                          reg_zone[CDMA_REG_ZONE];
    /*^ M, 0, N, 0, 0 ^*/
    U8                                          total_zone;
    /*^ M, 0, N, 0, 0 ^*/
    U8                                          zone_timer;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_son_cdma_csfb_registration_param_1XRTT_t;

#define RRM_SON_CDMA_PARA_XRTT_CSFB_REG_PARA_PRESENT              0x01
#define RRM_SON_CDMA_PARA_XRTT_LONG_CODE_STATE_XRTT_PRESENT       0x02
#define RRM_SON_CDMA_PARA_XRTT_CELL_RESELECT_PRESET               0x04

#define SON_CDMA_CODE_STATE_SIZE        6
/*! \ rrm_son_cdma_para_xrtt_t
 *  \brief   This struct contains info about son cdma params
 *  \param   bitmask bitmask
 *  \param   csfb_registrationparam1XRTT 
 *  \param   cellReselectionParametersHRPD cell Reselection Parameters
*/
typedef struct _rrm_son_cdma_para_xrtt_t
{
    rrm_bitmask_t                                   bitmask;        /*^ BITMASK ^*/
    rrm_son_cdma_csfb_registration_param_1XRTT_t    csfb_registrationparam1XRTT;
    /*^ O, RRM_SON_CDMA_PARA_XRTT_CSFB_REG_PARA_PRESENT, N, 0, 0^*/
    U8                                          longCodeState1XRTT[SON_CDMA_CODE_STATE_SIZE];
    /*^ O, RRM_SON_CDMA_PARA_XRTT_LONG_CODE_STATE_XRTT_PRESENT, OCTET_STRING, FIXED ^*/
    rrm_son_cdma_cellReselectionParametersHRPD_t    cellReselectionParametersHRPD;
    /*^ O, RRM_SON_CDMA_PARA_XRTT_CELL_RESELECT_PRESET, N, 0, 0^*/
}rrm_son_cdma_para_xrtt_t;

#define RRM_SON_CDMA_FREQ_OFFSET_FREQ_PRESENT  0x01
#define RRM_SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT   0x02
#define RRM_SON_CDMA_RESELECTION_PRIORITY_PRESENT   0x04
#define RRM_SON_CDMA_CFI_MEAS_OBJ_ID_PRESENT    0x08
#define RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT    0x10
/*! \ rrm_son_cdma_carrier_freq_info_t
 * \brief This struct contains son cdma carrier frequency information
 * \param bitmask
 * \param cdma_type cdma type 
 * \param band_class band class
 * \param arfcn
 * \param threshX_high threshX high
 * \param threshX low  threshX low 
 * \param offset_freq  offset frequency
 * \param srch_wnd_size srch_wnd_size
 * \param cell_reselection_priority cell reselection priority
 * \param meas_obj_id meas obj id
 * \param cells_for_which_to_report_cgi_size cells for which to report cgi size
 * \param cells_for_which_to_report_cgi[MAX_SON_MEAS_CONFIG_OBJECT] cells_for_which_to_report_cgi
*/ 
typedef struct _rrm_son_cdma_carrier_freq_info_t
{
    rrm_bitmask_t                           bitmask;/*^ BITMASK ^*/
    rrm_son_cdma2000_type_et                cdma_type;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_son_cdma_band_class_et              band_class;
    /*^ M, 0, H, 0, 31 ^*/
    U16                             arfcn;
    /*^ M, 0, H, 0, 2047 ^*/
   U8                              threshX_high;
    /*^ M, 0, H, 0, 63 ^*/
    U8                              threshX_low;
    /*^ M, 0, H, 0, 63 ^*/
    S8                              offset_freq;
    /*^ O,RRM_SON_CDMA_FREQ_OFFSET_FREQ_PRESENT,B, -15, 15 ^*/
    U8                              srch_wnd_size;
    /*^ O, RRM_SON_CDMA_FREQ_CDMA_SRCH_WNDO_SIZE_PRESENT, H, 0, 15 ^*/
   U8                              cell_reselection_priority;
    /*^ O, RRM_SON_CDMA_RESELECTION_PRIORITY_PRESENT, H, 0, 7 ^*/
    U8                               meas_obj_id;
    /*^ O, RRM_SON_CDMA_CFI_MEAS_OBJ_ID_PRESENT, B, 1, 12 ^*/
    U8                              cells_for_which_to_report_cgi_size;

/*SPR_17664_start*/
    /*^ O, RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT, B, 1, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    U16                             cells_for_which_to_report_cgi[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ O, RRM_SON_CDMA_CFI_CELLS_TO_REPORT_CGI_PRESENT, OCTET_STRING, VARIABLE ^*/

}rrm_son_cdma_carrier_freq_info_t;

#define RRM_SON_MAX_CDMA_SYS_TIME 7
/*! \ rrm_son_cdma_sys_time_info_t
 *  \brief This struct is used for SON CDMA System time information 
 *  \param is_cdma_nw_sync 
 *  \param cdma_sys_time_size
 *  \param cdma_sys_time[RRM_SON_MAX_CDMA_SYS_TIME]
 */
typedef struct _rrm_son_cdma_sys_time_info_t
{
    rrm_bool_et is_cdma_nw_sync;
    /*^ M, 0,H,0,1^*/
    U8      cdma_sys_time_size;

/*SPR_17664_start*/
    /*^ M, 0,B,5,RRM_SON_MAX_CDMA_SYS_TIME ^*/

/*SPR_17664_start*/
    U8      cdma_sys_time[RRM_SON_MAX_CDMA_SYS_TIME];
    /*^ M, 0,OCTET_STRING,VARIABLE ^*/
}rrm_son_cdma_sys_time_info_t;


#define RRM_SON_HRPD_PRE_REG_ZONE_ID_PRESENT      0x01
#define RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT  0x02
#define RRM_SON_MAX_SEC_PRE_REG_ZONES        2
/*! \ rrm_son_pre_reg_hrpd_info_t
 *  \brief This struct is used for SON pre register  HRPD info  
 *  \param bitmask
 *  \param pre_reg_allowed
 *  \param pre_reg_zone_id
 *  \param scndry_pre_reg_zone_id_list_size
 *  \param scndry_pre_reg_zone_id_list[RRM_SON_MAX_SEC_PRE_REG_ZONES]
 */
typedef struct _rrm_son_pre_reg_hrpd_info_t
{
    rrm_bitmask_t        bitmask;            /*^ BITMASK ^*/
    rrm_bool_et          pre_reg_allowed;
    /*^ M, 0, H, 0, 1 ^*/
    U8               pre_reg_zone_id;
    /*^ O, RRM_SON_HRPD_PRE_REG_ZONE_ID_PRESENT,N, 0,0 ^*/
    U8               scndry_pre_reg_zone_id_list_size;
/*SPR_17664_start*/
/* SPR-18445 start */
    /*^ O, RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT,B,1,RRM_SON_MAX_SEC_PRE_REG_ZONES ^*/
/* SPR-18445 stop */

    U8               scndry_pre_reg_zone_id_list[RRM_SON_MAX_SEC_PRE_REG_ZONES];

/*SPR_17664_end*/
    /*^ O, RRM_SON_HRPD_SEC_PRE_REG_ZONE_ID_PRESENT,OCTET_STRING,VARIABLE ^*/
}rrm_son_pre_reg_hrpd_info_t;

/*! \ rrm_son_cdma_csfb_reg_param_1XRTT_t
 *  \brief This struct is used for 
 *  \param sid
 *  \param nid
 *  \param multiple_sid
 *  \param multiple_nid
 *  \param home_reg
 *  \param foreign_sid_reg
 *  \param foreign_nid_reg
 *  \param parameter_reg
 *  \param power_up_reg
 *  \param reg_period
 *  \param reg_zone
 *  \param total_zone
 *  \param zone_timer
 */
typedef struct _rrm_son_cdma_csfb_reg_param_1XRTT_t
{
    U16                                      sid;
    /*^ M, 0, N, 0, 0 ^*/
    U16                                      nid;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_bool_et                                 multiple_sid;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 multiple_nid;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 home_reg;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 foreign_sid_reg;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 foreign_nid_reg;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 parameter_reg;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_bool_et                                 power_up_reg;
    /*^ M, 0, H, 0, 1 ^*/
    U8                                      reg_period;
    /*^ M, 0, N, 0, 0 ^*/
    U16                                     reg_zone;
    /*^ M, 0, N, 0, 0 ^*/
    U8                                      total_zone;
    /*^ M, 0, N, 0, 0 ^*/
    U8                                      zone_timer;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_son_cdma_csfb_reg_param_1XRTT_t;

#define RRM_SON_CDMA_1XRTT_REG_PARAMS_PRESENT 0x01
#define RRM_SON_CDMA_1XRTT_LONG_CODE_PRESENT  0x02
#define RRM_SON_CDMA_1XRTT_RESELECTION_SF_PRESENT   0x04

#define RRM_SON_MAX_LONG_CODE_1XRTT_SIZE     6
/*! \ rrm_son_cdma_1xrtt_info_t
 *  \brief This struct is used for SON CDMA 1xrtt info
 *  \param bitmask
 *  \param t_reselection
 *  \param xrtt_reg_params
 *  \param cdma_1xrtt_long_code[RRM_SON_MAX_LONG_CODE_1XRTT_SIZE]
 *  \param t_reselection_sf
 */
typedef struct _rrm_son_cdma_1xrtt_info_t
{
    rrm_bitmask_t                       bitmask; /*^ BITMASK ^*/
    U8                              t_reselection;
    /*^ M, 0 , H, 0, 7 ^*/
    rrm_son_cdma_csfb_reg_param_1XRTT_t xrtt_reg_params;
    /*^ O, RRM_SON_CDMA_1XRTT_REG_PARAMS_PRESENT, N, 0, 0^*/
    U8                          cdma_1xrtt_long_code[RRM_SON_MAX_LONG_CODE_1XRTT_SIZE];
    /*^ O, RRM_SON_CDMA_1XRTT_LONG_CODE_PRESENT, OCTET_STRING, FIXED ^*/
    rrm_son_speed_state_scale_factors_t     t_reselection_sf;
    /*^ O, RRM_SON_CDMA_1XRTT_RESELECTION_SF_PRESENT, N, 0, 0 ^*/
}rrm_son_cdma_1xrtt_info_t;

/*! \ rrm_son_cdma_csfb_reg_param_1xrtt_r9_t
 *  \brief This struct is used for CDMA CSFB reg params 1xrtt
 *  \param bitmask
 *  \param power_down_reg_r9
 */
typedef struct _rrm_son_cdma_csfb_reg_param_1xrtt_r9_t
{
    rrm_bitmask_t bitmask;    /*^ BITMASK ^*/
    rrm_bool_et power_down_reg_r9;
    /*^ M, 0, H, 0, 1^*/
}rrm_son_cdma_csfb_reg_param_1xrtt_r9_t;


/*! \ rrm_son_ac_barring_config_1xrtt_t 
 *  \brief This struct is used for son access barring config 1xrtt
 *  \param ac_barring_0_to_9_r9
 *  \param ac_barring_10_r9
 *  \param ac_barring_11_r9
 *  \param ac_barring_12_r9
 *  \param ac_barring_13_r9
 *  \param ac_barring_14_r9
 *  \param ac_barring_15_r9
 *  \param ac_barring_msg_r9
 *  \param ac_barring_reg_r9
 *  \param ac_barring_emg_r9
 */
typedef struct _rrm_son_ac_barring_config_1xrtt_t
{
    rrm_bitmask_t bitmask;    /*^ BITMASK ^*/
    U8  ac_barring_0_to_9_r9;
    /*^ M, 0, H, 0, 63 ^*/
    U8  ac_barring_10_r9;
    /*^ M, 0, H, 0, 7 ^*/
    U8  ac_barring_11_r9;
    /*^ M, 0, H, 0, 7 ^*/
    U8  ac_barring_12_r9;
    /*^ M, 0, H, 0, 7 ^*/
    U8  ac_barring_13_r9;
    /*^ M, 0, H, 0, 7 ^*/
    U8  ac_barring_14_r9;
    /*^ M, 0, H, 0, 7 ^*/
    U8  ac_barring_15_r9;
    /*^ M, 0, H, 0, 7 ^*/
    U8  ac_barring_msg_r9;
    /*^ M, 0, H, 0, 7 ^*/
    U8  ac_barring_reg_r9;
    /*^ M, 0, H, 0, 7 ^*/
    U8  ac_barring_emg_r9;
    /*^ M, 0, H, 0, 7 ^*/
}rrm_son_ac_barring_config_1xrtt_t;
#define RRM_SON_CDMA_HRPD_REG_PARAMS_PRESENT 0x01
#define RRM_SON_CDMA_HRPD_RESELECTION_SF_PRESENT 0x02
/*! \ rrm_son_cdma_hrpd_info_t
 *  \brief This struct is used for son HRPD info
 *  \param bitmask
 *  \param hrpd_reg_params
 *  \param t_reselection
 *  \param t_reselection_sf
 */
typedef struct _rrm_son_cdma_hrpd_info_t
{
    rrm_bitmask_t                   bitmask; /*^ BITMASK ^*/
    rrm_son_pre_reg_hrpd_info_t hrpd_reg_params;
    /*^ O, RRM_SON_CDMA_HRPD_REG_PARAMS_PRESENT, N, 0, 0^*/
    U8                              t_reselection;
    /*^ M, 0, H, 0, 7 ^*/
    rrm_son_speed_state_scale_factors_t     t_reselection_sf;
    /*^ O, RRM_SON_CDMA_HRPD_RESELECTION_SF_PRESENT, N, 0, 0 ^*/

}rrm_son_cdma_hrpd_info_t;


#define RRM_SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT          0x01
#define RRM_SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT         0x02
#define RRM_SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT      0x04
#define RRM_SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT            0x08
#define RRM_SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT    0x10
#define RRM_SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT        0x20
#define RRM_SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT     0x40


#define SON_MAX_CDMA_OBJECT             8
#define RRM_CDMA_MAX_NBR_CELL_SIZE      8
/*! \ rrm_son_meas_config_cdma_t
 *  \brief This struct is used for son meas config CDMA info
 *  \param bitmask
 *  \param cdma_freq_info_list_size
 *  \param cdma_freq_info_list[MAX_SON_MEAS_CONFIG_OBJECT]
 *  \param sys_time_info
 *  \param srch_wnd_size
 *  \param hrpd_info
 *  \param cdma_1xrtt_info
 *  \param csfb_support_for_dual_Rx_Ues_r9
 *  \param csfb_registration_param_1xrtt_r9
 *  \param ac_barring_config_1xrtt_r9
 *  \param meas_obj_id
 *  \param cdma_meas_list_size
 *  \param cells_for_which_to_report_cgi[RRM_CDMA_MAX_NBR_CELL_SIZE]
 */
typedef struct _rrm_son_meas_config_cdma_t
{
    rrm_bitmask_t                      bitmask;            /*^ BITMASK ^*/
    U8                              cdma_freq_info_list_size;       
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0,H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End */
    /* Bug Fix 792 */
    rrm_son_cdma_carrier_freq_info_t        cdma_freq_info_list[MAX_SON_MEAS_CONFIG_OBJECT]; 
     /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    rrm_son_cdma_sys_time_info_t            sys_time_info;              
    /*^ O, RRM_SON_MEAS_CONFIG_CDMA_SYS_TIME_INFO_PRESENT, N, 0, 0^*/
    U8                              srch_wnd_size;             
    /*^ O, RRM_SON_MEAS_CONFIG_CDMA_SRCH_WNDO_SIZE_PRESENT, H, 0, 15 ^*/
    rrm_son_cdma_hrpd_info_t             hrpd_info;          
    /*^ O, RRM_SON_MEAS_CONFIG_CDMA_PRE_REG_HRPD_INFO_PRESENT, N, 0, 0^*/
    rrm_son_cdma_1xrtt_info_t               cdma_1xrtt_info;
    /*^ O, RRM_SON_MEAS_CONFIG_CDMA_1X_RTT_INFO_PRESENT, N, 0, 0^*/
    rrm_bool_et                         csfb_support_for_dual_Rx_Ues_r9;
    /*^ O, RRM_SON_MEAS_CONFIG_CSFB_SUPPORT_FOR_DUAL_RX_UES_PRESENT, H, 0, 1^*/
    rrm_son_cdma_csfb_reg_param_1xrtt_r9_t csfb_registration_param_1xrtt_r9;
    /*^ O, RRM_SON_MEAS_CONFIG_CSFB_REG_PARAM_1XRTT_PRESENT, N, 0, 0^*/
    rrm_son_ac_barring_config_1xrtt_t       ac_barring_config_1xrtt_r9;
    /*^ O, RRM_SON_MEAS_CONFIG_AC_BARRING_CONFIG_1XRTT_PRESENT, N, 0, 0^*/
}rrm_son_meas_config_cdma_t;

/*! \ rrm_son_inter_rat_meas_config_t
 *  \brief This struct is used for inter rat meas config info
 *  \param utran_config_list rrm_son_meas_config_utran_t
 *  \param geran_config_list rrm_son_meas_config_geran_t
 *  \param cdma_config_list rrm_son_meas_config_cdma_t
 */
typedef struct _rrm_son_inter_rat_meas_config_t
{
    rrm_son_meas_config_utran_t     utran_config_list;
        /*^ M, 0, N, 0, 0 ^*/
    rrm_son_meas_config_geran_t     geran_config_list;
    /*^ M,0, N, 0, 0 ^*/
    rrm_son_meas_config_cdma_t      cdma_config_list;
    /*^ M,0, N, 0, 0 ^*/
}rrm_son_inter_rat_meas_config_t;


#define RRM_SON_GAP_OFFSET_GP0_PRESENT 0x01
#define RRM_SON_GAP_OFFSET_GP1_PRESENT 0x02
/*! \ rrm_son_setup_meas_gap_config_t
 *  \brief This struct is used for SON setup meas gap config information
 *  \param bitmask rrm_bitmask_t
 *  \param gap_offset_gp0 GAP offset for group 0
 *  \param gap_offset_gp1 GAP offset for group 1
 */
typedef struct _rrm_son_setup_meas_gap_config_t
{
    rrm_bitmask_t                        bitmask;/*^ BITMASK ^*/

    U8                         gap_offset_gp0;
    /*^ O, RRM_SON_GAP_OFFSET_GP0_PRESENT,H, 0, 39 ^*/

    U8                         gap_offset_gp1;
    /*^ O, RRM_SON_GAP_OFFSET_GP1_PRESENT,H, 0, 79 ^*/

}rrm_son_setup_meas_gap_config_t;


#define RRM_SON_SETUP_GAP_CONFIG_PRESENT   0x01
/*! \ rrm_son_gap_config_t
 *  \brief This struct is used for SON gap config information
 *  \param bitmask rrm_bitmask_t
 *  \param setup_gap_config rrm_son_setup_meas_gap_config_t
 */
typedef struct _rrm_son_gap_config_t
{
   rrm_bitmask_t                      bitmask; /*^ BITMASK ^*/
   rrm_son_setup_meas_gap_config_t    setup_gap_config;
   /*^ O, RRM_SON_SETUP_GAP_CONFIG_PRESENT,N, 0, 0 ^*/

}rrm_son_gap_config_t;

#define RRM_SON_EUTRAN_GAP_CONFIG_PRESENT   0x01
#define RRM_SON_UTRAN_GAP_CONFIG_PRESENT    0x02
#define RRM_SON_GERAN_GAP_CONFIG_PRESENT    0x04
#define RRM_SON_CDMA2000_GAP_CONFIG_PRESENT 0x08
/*! \ rrm_son_meas_gap_config_t
 *  \brief This struct is used for Meas gap config information
 *  \param bitmask
 *  \param eutran_gap_config
 *  \param utran_gap_config
 *  \param geran_gap_config
 *  \param cdma2000_gap_config
 */
typedef struct _rrm_son_meas_gap_config_t
{
   rrm_bitmask_t                bitmask; /*^ BITMASK ^*/
   rrm_son_gap_config_t   eutran_gap_config;
   /*^ O, RRM_SON_EUTRAN_GAP_CONFIG_PRESENT,N, 0, 0 ^*/
   rrm_son_gap_config_t   utran_gap_config;
   /*^ O, RRM_SON_UTRAN_GAP_CONFIG_PRESENT,N, 0, 0 ^*/
   rrm_son_gap_config_t   geran_gap_config;
   /*^ O, RRM_SON_GERAN_GAP_CONFIG_PRESENT,N, 0, 0 ^*/
   rrm_son_gap_config_t   cdma2000_gap_config;
   /*^ O, RRM_SON_CDMA2000_GAP_CONFIG_PRESENT,N, 0, 0 ^*/

}rrm_son_meas_gap_config_t;


#define RRM_SON_UE_INDEX_PRESENT          0x01
#define RRM_SON_MEAS_GAP_CONFIG_PRESENT   0x02
/*! \ rrm_son_meas_config_req_t
 *  \brief This struct is used for SON measurement Config request
 *  \param bitmask
 *  \param cell_id
 *  \param ue_index
 *  \param eutran_config_list_size
 *  \param eutran_config_list[MAX_SON_MEAS_CONFIG_OBJECT]
 *  \param inter_rat_meas_config
 *  \param meas_gap_config
 */
typedef struct _rrm_son_meas_config_req_t
{
    rrm_bitmask_t                             bitmask;
    /*^ BITMASK ^*/
    rrm_oam_eutran_global_cell_id_t cell_id;
    /*^ M, 0, N, 0, 0 ^*/
    U16                             ue_index;
    /*^ O, RRM_SON_UE_INDEX_PRESENT, N, 0, 0 ^*/
    U8                              eutran_config_list_size;
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End */
    rrm_son_meas_config_eutran_t    eutran_config_list[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    rrm_son_inter_rat_meas_config_t  inter_rat_meas_config;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_son_meas_gap_config_t        meas_gap_config;
    /*^ O, RRM_SON_MEAS_GAP_CONFIG_PRESENT, N, 0, 0 ^*/
}rrm_son_meas_config_req_t;
/*^ API, RRM_SON_MEAS_CONFIG_REQ ^*/
/*! \ rrm_son_meas_config_res_t
 *  \brief This struct is used for SON meas config response
 *  \param meas_config_res
 */
typedef struct _rrm_son_meas_config_res_t
{
    rrm_son_non_registration_based_res_t meas_config_res; /*^ M, 0, N, 0, 0 ^*/
}rrm_son_meas_config_res_t;
/*^ API, RRM_SON_MEAS_CONFIG_RESP ^*/


/*! \ rrm_son_meas_config_obj_remove_req_t
 *  \brief This struct is used for deleting measurement Objects
 *  \param bitmask
 *  \param cell_id 
 *  \param earfcn_remove_list_size  No of Eutran Objects to be removed
 *  \param earfcn_remove_list       List of Eutran Objects
 *  \param uarfcn_remove_list_size  No of Utran Objects to be removed 
 *  \param uarfcn_remove_list       List of Utran Objects
 *  \param geran_remove_list_size   No of Geran Objects to be removed
 *  \param geran_remove_list        List of Geran Objects
 */
typedef struct _rrm_son_meas_config_obj_remove_req_t
{
    rrm_oam_eutran_global_cell_id_t cell_id;
    /*^ M, 0, N, 0, 0 ^*/
    U8                               earfcn_remove_list_size;
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End */
    U16                              earfcn_remove_list[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                               uarfcn_fdd_remove_list_size;
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End*/
    U16                              uarfcn_fdd_remove_list[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                               uarfcn_tdd_remove_list_size;
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End*/
    U16                              uarfcn_tdd_remove_list[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                               geran_remove_list_size;
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End*/
    rrm_son_geran_carrier_freqs_t    geran_remove_list[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                               cdma_remove_list_size;
    /* SPR 12929 Fix Start */

/*SPR_17664_start*/
    /*^ M, 0, H, 0, MAX_SON_MEAS_CONFIG_OBJECT ^*/

/*SPR_17664_end*/
    /* SPR 12929 Fix End*/
    rrm_son_cdma_carrier_freq_info_t cdma_remove_list[MAX_SON_MEAS_CONFIG_OBJECT];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_son_meas_config_obj_remove_req_t;
/*^ API, RRM_SON_MEAS_CONFIG_OBJ_REMOVE_REQ ^*/


/*! \ rrm_son_meas_config_obj_remove_res_t
 *  \brief This struct is used for SON meas config object removal response
 *  \param meas_config_obj_remove_res
 */
typedef struct _rrm_son_meas_config_obj_remove_res_t
{
    rrm_son_non_registration_based_res_t meas_config_obj_remove_res; /*^ M, 0, N, 0, 0 ^*/
}rrm_son_meas_config_obj_remove_res_t;
/*^ API, RRM_SON_MEAS_CONFIG_OBJ_REMOVE_RESP ^*/

#define RRM_SON_EUTRAN_NC_CGI_PRESENT   0x01
#define RRM_SON_EUTRAN_NC_PLMN_PRESENT  0x02
#define RRM_SON_EUTRAN_NC_TAC_PRESENT   0x04
#define RRM_SON_EUTRAN_NC_CSG_PRESENT   0x08
/*csg start*/
#define RRM_SON_EUTRAN_NC_CSG_MEM_STATUS_PRESENT 0x10
/*csg end*/
/*! \  rrm_son_eutran_neighbor_cell_t
 *  \brief This struct contains info about neighbour cell eutran
 *  \param bitmask bitmask
 *  \param pci pci
 *  \param cgi cgi
 *  \param num_plmn_id num of plmn id
 *  \param plmn_id[MAX_PLMN_ID_LIST2] plmn id
 *  \param tac[MAX_TAC_SIZE] 
 *  \param csg_identity[MAX_CELL_IDENTITY_OCTETS]
 *  \param rsrp rsrp
 *  \param rsrq rsrq
 *  \param  csg_membershipStatus csq membership status
*/
typedef struct _rrm_son_eutran_neighbor_cell_t
{
    rrm_bitmask_t                   bitmask;    /*^ BITMASK ^*/
    U16                             pci;                                
    /*^ M, 0, H, 0, 503 ^*/
    rrm_oam_eutran_global_cell_id_t cgi;                                
    /*^ O, RRM_SON_EUTRAN_NC_CGI_PRESENT, N, 0, 0 ^*/
    U8                              num_plmn_id;                        
/* SPR-18445 start */
    /*^ O, RRM_SON_EUTRAN_NC_PLMN_PRESENT, B, 1, MAX_PLMN_ID_LIST2 ^*/
/* SPR-18445 stop */
    rrm_oam_cell_plmn_info_t        plmn_id[MAX_PLMN_ID_LIST2];          
    /*^ O, RRM_SON_EUTRAN_NC_PLMN_PRESENT, OCTET_STRING, VARIABLE ^*/
    U8                              tac[MAX_TAC_SIZE] ;           
    /*^ O, RRM_SON_EUTRAN_NC_TAC_PRESENT, OCTET_STRING, FIXED ^*/ 
    U8                              csg_identity[MAX_CELL_IDENTITY_OCTETS];
    /*^ O, RRM_SON_EUTRAN_NC_CSG_PRESENT, OCTET_STRING, FIXED ^*/
    U8                              rsrp;                               
    /*^ M, 0, H, 0, 128 ^*/
    U8                              rsrq;                               
    /*^ M, 0, H, 0, 34 ^*/
    /*csg start*/
    rrm_csg_membership_status_et    csg_membershipStatus;  
    /*Start: SPR 8881*/
    /*^ O, RRM_SON_EUTRAN_NC_CSG_MEM_STATUS_PRESENT, H, 0, 2 ^*/
    /*End: SPR 8881*/
    /*csg end*/
}rrm_son_eutran_neighbor_cell_t;
/*! \  rrm_son_eutran_meas_results_t
 *  \brief This struct contains rrm son measurement results
 *  \param earfcnearfcn
 *  \param neighbor_cell_list_size neighbour cell list size
 *  \param neighbor_cell_list[RRM_MAX_CELLS_REPORTED] neighbour cell list
*/
typedef struct _rrm_son_eutran_meas_results_t
{
    U16                             earfcn;                             
    /*^ M, 0, N, 0, 0 ^*/
    U8                              neighbor_cell_list_size;   
        
/*SPR_17664_start*/
    /*^ M, 0, H, 0, RRM_MAX_CELLS_REPORTED ^*/

/*SPR_17664_end*/
    rrm_son_eutran_neighbor_cell_t  neighbor_cell_list[RRM_MAX_CELLS_REPORTED]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_son_eutran_meas_results_t;

/*! \ rrm_son_utran_physical_cell_id_t
 *  \brief Physical Cell ID
 *  \param pci_fdd  PCI (FDD Mode)
 *  \param pci_tdd PCI (TDD Mode)
 */
#define RRM_PCI_FDD_PRESENT                  0x01
#define RRM_PCI_TDD_PRESENT                  0x02
typedef struct _rrm_son_utran_physical_cell_id_t
{
    rrm_bitmask_t                bitmask;        /*^ BITMASK ^*/
    U16                pci_fdd;
    /*^ O, RRM_PCI_FDD_PRESENT, H, 0, 511 ^*/
    U8                 pci_tdd;
    /*^ O, RRM_PCI_TDD_PRESENT, H, 0, 127 ^*/
}rrm_son_utran_physical_cell_id_t;

/*! \ rrm_son_utran_geran_global_cell_id_t
 *  \brief Global Cell ID
 *  \param plmn_id          PLMN Id
 *  \param cell_id          Cell Identity
 */
typedef struct _rrm_son_utran_geran_global_cell_id_t
{
    rrm_rat_type_et             rat_type;       
    /*^ M, 0, N, 1, 3 ^*/
    rrm_oam_cell_plmn_info_t   plmn_id;
    /*^ M, 0, N, 0, 0 ^*/
    U8                         cell_identity[MAX_CELL_IDENTITY_OCTETS];
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_son_utran_geran_global_cell_id_t;


#define RRM_SON_UTRAN_NC_CGI_PRESENT    0x01
#define RRM_SON_UTRAN_NC_LAC_PRESENT    0x02
#define RRM_SON_UTRAN_NC_RAC_PRESENT    0x04
#define RRM_SON_UTRAN_NC_PLMN_PRESENT   0x08
#define RRM_SON_UTRAN_NC_CSG_PRESENT    0x10
/*csg start*/
#define RRM_SON_UTRAN_NC_CSG_MEM_STATUS_PRESENT 0x20
/*csg end*/
/*! \ rrm_son_utran_neighbor_cell_t
 *  \brief This struct contains info about rrm son neighbour cell
 *  \param bitmask bitmask
 *  \param pci pci
 *  \param cgi cgi
 *  \param lac[MAX_LAC_SIZE] 
 *  \param rac rac
 *  \param num_plmn_id number of plmn id
 *  \param plmn_id[MAX_PLMN_ID_LIST2] plmn id
 *  \param csg_identity[MAX_CELL_IDENTITY_OCTETS] csg identity
 *  \param rscp
 *  \param ecno ecno
 *  \param csg_membershipStatus csg membership status
*/
typedef struct _rrm_son_utran_neighbor_cell_t
{
    rrm_bitmask_t                                 bitmask;    /*^ BITMASK ^*/
    rrm_son_utran_physical_cell_id_t        pci;                                
    /*^ M, 0, N, 0, 0 ^*/ 
    rrm_son_utran_geran_global_cell_id_t    cgi;                                
    /*^ O, RRM_SON_UTRAN_NC_CGI_PRESENT, N, 0, 0 ^*/
    U8                                  lac[MAX_LAC_SIZE];            
    /*^ O, RRM_SON_UTRAN_NC_LAC_PRESENT, OCTET_STRING, FIXED ^*/
    U8                                  rac;                                
    /*^ O, RRM_SON_UTRAN_NC_RAC_PRESENT, N, 0, 0 ^*/
    U8                                  num_plmn_id;                        
/* SPR-18445 start */
    /*^ O, RRM_SON_UTRAN_NC_PLMN_PRESENT, B, 1, MAX_PLMN_ID_LIST2 ^*/
/* SPR-18445 stop */
    rrm_oam_cell_plmn_info_t                plmn_id[MAX_PLMN_ID_LIST2];          
    /*^ O, RRM_SON_UTRAN_NC_PLMN_PRESENT, OCTET_STRING, VARIABLE ^*/
    U8                                  csg_identity[MAX_CELL_IDENTITY_OCTETS];
    /*^ O, RRM_SON_UTRAN_NC_CSG_PRESENT, OCTET_STRING, FIXED ^*/
    S8                                  rscp;                               
    /*^ M, 0, B, -5, 91 ^*/
    U8                                  ecno;                               
    /*^ M, 0, H, 0, 49 ^*/
    /*csg start*/
    rrm_csg_membership_status_et    csg_membershipStatus;
    /*^ O, RRM_SON_UTRAN_NC_CSG_MEM_STATUS_PRESENT, H, 0, 0 ^*/
    /*csg end*/
}rrm_son_utran_neighbor_cell_t;
/*! \ rrm_son_utran_meas_results_t
 *  \brief this struct contains rrm son utran measurement results
 *  \param uarfcn
 *  \param neighbor_cell_list_size neighbour cell list size
 *  \param neighbor_cell_list[RRM_MAX_CELLS_REPORTED]
*/
typedef struct _rrm_son_utran_meas_results_t
{
    U16                         uarfcn;                                     
    /*^ M, 0, H, 0, 16383 ^*/
    U8                          neighbor_cell_list_size;                         

/*SPR_17664_start*/
    /*^ M, 0, H, 0, RRM_MAX_CELLS_REPORTED ^*/

/*SPR_17664_end*/
    rrm_son_utran_neighbor_cell_t   neighbor_cell_list[RRM_MAX_CELLS_REPORTED]; 
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_son_utran_meas_results_t;

#define RRM_SON_GERAN_MR_IND_CGI_INFO_PRESENT  0x01
#define RRM_SON_GERAN_MR_IND_RAC_PRESENT       0x02
/*! \ rrm_son_geran_neighbor_cell_t
 *  \brief This struct contains info about neighbour cell
 *  \param bitmask bitmask
 *  \param arfcn arfcn
 *  \param band_ind band index
 *  \param pci pci
 *  \param cgi cgi
 *  \param rac rac
 *  \param rssi rssi
*/
typedef struct _rrm_son_geran_neighbor_cell_t
{
   rrm_bitmask_t                                      bitmask;
    /*^ BITMASK ^*/
   U16                                      arfcn;
   /*^ M, 0, H, 0, 1023 ^*/
   rrm_son_geran_band_indicator_et          band_ind;
   /*^ M, 0, H, 0, 1 ^*/
   rrm_son_geran_physical_cell_id_t         pci;
   /*^ M, 0, N, 0, 0 ^*/
   rrm_son_utran_geran_global_cell_id_t     cgi;          
   /*^ O, RRM_SON_GERAN_MR_IND_CGI_INFO_PRESENT, N , 0, 0 ^*/
   U8                                       rac;
   /*^ O, RRM_SON_GERAN_MR_IND_RAC_PRESENT  , N , 0, 0 ^*/
   U8                                       rssi;
   /*^ M, 0, H, 0, 63 ^*/
}rrm_son_geran_neighbor_cell_t;

#define RRM_SON_GERAN_MR_MEAS_OBJ_ID_PRESENT                     0x01
/*! \ rrm_son_geran_meas_results_t
 *  \brief This struct contains geran measurment results
 *  \param bitmask bitmask
 *  \param neighbor_cell_list_size neighbor_cell_list_size
 *  \param neighbor_cell_list[RRM_MAX_CELLS_REPORTED] neighbor_cell_list
*/
typedef struct _rrm_son_geran_meas_results_t
{
   rrm_bitmask_t                              bitmask;
   /*^ BITMASK ^*/
   U8                               meas_obj_id;
   /*^ O, RRM_SON_GERAN_MR_MEAS_OBJ_ID_PRESENT, B, 1, 12 ^*/
   U8                               neighbor_cell_list_size;                         
  
/*SPR_17664_start*/
   /*^ M, 0, H, 1, RRM_MAX_CELLS_REPORTED ^*/

/*SPR_17664_end*/
   rrm_son_geran_neighbor_cell_t    neighbor_cell_list[RRM_MAX_CELLS_REPORTED]; 
   /*^ M, 0, OCTET_STRING, VARIABLE ^*/

}rrm_son_geran_meas_results_t;

#define RRM_SON_CELL_GLOBAL_ID_1X_RTT_SIZE    6
#define RRM_SON_CELL_GLOBAL_ID_HRPD_SIZE                       16

#define RRM_SON_CGI_1X_RTT_PRESENT 0x01
#define RRM_SON_CGI_HRPD_PRESENT 0x02
/*! \ rrm_son_cdma_global_cell_id_t
 *  \brief This struct contains cdma global cell id
 *  \param bitmask bitmask
 *  \param cell_global_id_1X_RTT [RRM_SON_CELL_GLOBAL_ID_1X_RTT_SIZE] cell_global_id_1X_RTT
 *  \param cell_global_id_HRPD [RRM_SON_CELL_GLOBAL_ID_HRPD_SIZE]     cell_global_id_HRPD
*/
typedef struct _rrm_son_cdma_global_cell_id_t
{
    rrm_bitmask_t                               bitmask; /*^ BITMASK ^*/
    U8                cell_global_id_1X_RTT [RRM_SON_CELL_GLOBAL_ID_1X_RTT_SIZE];
    /*^ O, RRM_SON_CGI_1X_RTT_PRESENT, OCTET_STRING, FIXED ^*/
    U8                 cell_global_id_HRPD [RRM_SON_CELL_GLOBAL_ID_HRPD_SIZE];
    /*^ O, RRM_SON_CGI_HRPD_PRESENT, OCTET_STRING, FIXED ^*/
}rrm_son_cdma_global_cell_id_t;

/*! \ rrm_son_cdma_neighbor_cell_t
 *  \brief This struct contains cdma neighour cell info
 *  \param bitmask bitmask
 *  \param arfcn arfcn
 *  \param band_class band class
 *  \param pci pci
 *  \param cgi cgi
*/


#define RRM_SON_CDMA_MEAS_IND_CGI_INFO_PRESENT  0x01
#define RRM_SON_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT     0x02
typedef struct _rrm_son_cdma_neighbor_cell_t
{
   rrm_bitmask_t                                      bitmask;
    /*^ BITMASK ^*/
   U16                                      arfcn;
   /*^ M, 0, H, 0, 1023 ^*/
   rrm_son_cdma_band_class_et               band_class;
   /*^ M, 0, H, 0, 31 ^*/
   U16                                      pci;
   /*^ M, 0, N, 0, 0 ^*/
   rrm_son_cdma_global_cell_id_t            cgi;
   /*^ O, RRM_SON_CDMA_MEAS_IND_CGI_INFO_PRESENT, N , 0, 0 ^*/
   U16           pilot_pn_phase;
   /*^ O, RRM_SON_MEAS_RESULT_CDMA2000_PILOT_PN_PHASE_PRESENT, H, 0, 32767 ^*/
   U8            pilot_strength;
   /*^ M, 0, H, 0, 63 ^*/
}rrm_son_cdma_neighbor_cell_t;

#define RRM_SON_CDMA_MEAS_RESULT_OBJ_ID_PRESENT 0x01
/*! \ rrm_son_cdma_meas_results_t
 *  \brief this struct contains meas results
 *  \param bitmask bitmask
 *  \param meas_obj_id meas_obj_id
 *  \param neighbor_cell_list_size neighbor cell list size
 *  \param neighbor_cell_list[RRM_MAX_CELLS_REPORTED] neighbor_cell_list
*/ 
typedef struct _rrm_son_cdma_meas_results_t
{
   rrm_bitmask_t                              bitmask;
   /*^ BITMASK ^*/
   U8                               meas_obj_id;
   /*^ O, RRM_SON_CDMA_MEAS_RESULT_OBJ_ID_PRESENT, B, 1, 12 ^*/
   U8                               neighbor_cell_list_size;

/*SPR_17664_start*/
   /*^ M, 0, H, 1, RRM_MAX_CELLS_REPORTED ^*/

/*SPR_17664_end*/
   rrm_son_cdma_neighbor_cell_t     neighbor_cell_list[RRM_MAX_CELLS_REPORTED];
   /*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
}rrm_son_cdma_meas_results_t;

/*! \rrm_son_meas_results_ecid_r9_t
 *  \brief this struct contains meas results ecid r9
 *  \param ue_rxtx_time_diff
 *  \param current_sfn_r9
 */
typedef struct _rrm_son_meas_results_ecid_r9_t
{
    U16    ue_rxtx_time_diff;
    U8     current_sfn_r9[ECID_CURRENT_SFN_OCTET_SIZE];
}rrm_son_meas_results_ecid_r9_t;

/*! \ rrm_son_meas_results_ind_t
 *  \brief this struct contains meas results index
 *  \param ue_index ue_index
 *  \param eutran_meas_list_size eutran_meas_list_size
 *  \param eutran_meas_list[RRM_MAX_CELLS_REPORTED] eutran_meas_list
 *  \param utran_meas_list_size utran meas list size
 *  \param utran_meas[RRM_MAX_CELLS_REPORTED] utran_meas
 *  \param geran_meas_list_size geran_meas_list_size
 *  \param cdma_meas_list_size cdma meas list size
 *  \param cdma_meas[RRM_MAX_CELLS_REPORTED] cdma_meas
*/
typedef struct _rrm_son_meas_results_ind_t
{
    rrm_oam_eutran_global_cell_id_t cell_id;
    /*^ M, 0, N, 0, 0 ^*/
    U16                             ue_index;
    /* SPR 15698 Start */
    rrmc_meas_report_event_et       meas_report_event;
    /* SPR 15698 End */
    /*^ M, 0, H, 0, 599 ^*/
    U8                              eutran_meas_list_size;
    
/*SPR_17664_start*/

    /*^ M, 0, H, 0, RRM_MAX_CELLS_REPORTED ^*/

/*SPR_17664_end*/
    rrm_son_eutran_meas_results_t   eutran_meas_list[RRM_MAX_CELLS_REPORTED];  
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
    U8                              utran_meas_list_size;    

/*SPR_17664_start*/
    /*^ M, 0, H, 0, RRM_MAX_CELLS_REPORTED ^*/

/*SPR_17664_end*/
    rrm_son_utran_meas_results_t    utran_meas[RRM_MAX_CELLS_REPORTED];        
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                              geran_meas_list_size;

/*SPR_17664_start*/
    /*^ M, 0, H, 0, RRM_MAX_CELLS_REPORTED ^*/

/*SPR_17664_end*/
    rrm_son_geran_meas_results_t    geran_meas[RRM_MAX_CELLS_REPORTED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
    U8                              cdma_meas_list_size;

/*SPR_17664_start*/
    /*^ M, 0, H, 0, RRM_MAX_CELLS_REPORTED ^*/

/*SPR_17664_start*/
    rrm_son_cdma_meas_results_t     cdma_meas[RRM_MAX_CELLS_REPORTED];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/ /* SPR 8878: ecid_r9 removed */
}rrm_son_meas_results_ind_t;
/*^ API, RRM_SON_MEAS_RESULTS_IND ^*/

/* UE_MEAS CHANGES : ENDS */
/* ! \enum rrm_ho_cause_et
 *   \brief An enum that gives ho cause
*/
typedef enum
{
    RRM_HO_TOO_EARLY = 0,
    RRM_HO_TO_WRONG_CELL,
    RRM_HO_TOO_LATE
}rrm_ho_cause_et;

#define RRM_SON_OFFSET_ATTR_OFC_PRESENT 0x01
#define RRM_SON_OFFSET_ATTR_OCS_PRESENT 0x02
#define RRM_SON_OFFSET_ATTR_OFN_PRESENT 0x04
#define RRM_SON_OFFSET_ATTR_OFF_PRESENT 0x08
#define RRM_SON_OFFSET_ATTR_HYS_PRESENT 0x10
/*! \ rrm_son_offset_attr_t
 *  \brief This struct is used for SON offset attribute info
 *  \param Bitmask 
 *  \param ofs
 *  \param ocs
 *  \param ofn
 *  \param off
 *  \param hys
 */
typedef struct _rrm_son_offset_attr_t
{
    rrm_bitmask_t             bitmask;
    /*^ BITMASK ^*/
    S8    ofs;
    /*^ O, RRM_SON_OFFSET_ATTR_OFC_PRESENT, B, -24, 24 ^*/
    S8    ocs;
    /*^ O, RRM_SON_OFFSET_ATTR_OCS_PRESENT, B, -24, 24 ^*/
    S8    ofn;
    /*^ O, RRM_SON_OFFSET_ATTR_OFN_PRESENT, B, -24, 24 ^*/
    S8    off;
    /*^ O, RRM_SON_OFFSET_ATTR_OFF_PRESENT, B, -24, 24 ^*/
    U8    hys;
    /*^ O, RRM_SON_OFFSET_ATTR_HYS_PRESENT, H, 0, 30 ^*/
}rrm_son_offset_attr_t;

#define RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT  0x01
#define RRM_SON_EUTRAN_STRENGTH_RSRQ_PRESENT  0x02
#define RRM_SON_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT   0x04
/*! \ rrm_son_eutran_strength_t
 *  \brief This struct gives eutran strength
 *  \param bitmask bitmask
 *  \param rsrp rsrp
 *  \param rsrq rsrq
 *  \param offset_attr offset attribute
 */
typedef struct _rrm_son_eutran_strength_t
{
    rrm_bitmask_t          bitmask;/*^ BITMASK ^*/
    U8                     rsrp;
    /*^ O, RRM_SON_EUTRAN_STRENGTH_RSRP_PRESENT, H, 0, 128 ^*/
    U16                    rsrq;
    /*^ O, RRM_SON_EUTRAN_STRENGTH_RSRQ_PRESENT, H, 0, 34 ^*/
    rrm_son_offset_attr_t  offset_attr;
    /*^ O, RRM_SON_EUTRAN_STRENGTH_OFFSET_ATT_PRESENT, N, 0, 0 ^*/
}rrm_son_eutran_strength_t;

#define RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT  0x01
#define RRM_SON_UTRAN_STRENGTH_ECNO_PRESENT  0x02
/*! \ rrm_son_utran_strength_t
 *  \brief This struct is used for  UTRAN strength info
 *  \param bitmask
 *  \param rscp
 *  \param ecno
 */
typedef struct _rrm_son_utran_strength_t
{
    rrm_bitmask_t          bitmask;/*^ BITMASK ^*/
    S8    rscp;
    /*^ O, RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT, B, -5, 91 ^*/
    U16   ecno;
    /*^ O, RRM_SON_UTRAN_STRENGTH_ECNO_PRESENT, H, 0, 49 ^*/

}rrm_son_utran_strength_t;

#define RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT 0x01
/*! \ rrm_son_geran_strength_t
 *  \brief This struct is used for GERAN strength info 
 *  \param bitmask Bitmask Value
 *  \param rssi
 */
typedef struct _rrm_son_geran_strength_t
{
    rrm_bitmask_t          bitmask;/*^ BITMASK ^*/
    U8    rssi;
    /*^ O, RRM_SON_UTRAN_STRENGTH_RSCP_PRESENT, H, 0, 63 ^*/
}rrm_son_geran_strength_t;

#define RRM_SON_EUTRAN_CELL_STRENGTH_PRESENT 0x01
#define RRM_SON_UTRAN_CELL_STRENGTH_PRESENT  0x02
#define RRM_SON_GERAN_CELL_STRENGTH_PRESENT  0x04
/*! \ rrm_son_meas_strength_t
 *  \brief  This struct measures strength
 *  \param bitmask bit mask
 *  \param eutran_cell_strength eutran strength
 *  \param utran_cell_strength utran strength
 *  \param geran_cell_strength geran strength
*/
typedef struct _rrm_son_meas_strength_t
{
    rrm_bitmask_t             bitmask;
    /*^ BITMASK ^*/
    rrm_son_eutran_strength_t eutran_cell_strength;
    /*^ O, RRM_SON_EUTRAN_CELL_STRENGTH_PRESENT, N, 0, 0 ^*/
    rrm_son_utran_strength_t  utran_cell_strength;
    /*^ O, RRM_SON_UTRAN_CELL_STRENGTH_PRESENT, N, 0, 0 ^*/
    rrm_son_geran_strength_t  geran_cell_strength;
    /*^ O, RRM_SON_GERAN_CELL_STRENGTH_PRESENT, N, 0, 0 ^*/

}rrm_son_meas_strength_t;

#define HO_REPORT_RE_EST_CGI_PRESENT             0x01
#define HO_REPORT_SERVING_MEAS_STRENGTH_PRESENT  0x02 
#define HO_REPORT_TARGET_MEAS_STRENGTH_PRESENT   0x04
#define HO_REPORT_OFFSET_ATTR_PRESENT            0x08
/*! \ rrm_son_ho_report_t
 *  \brief This struct conatins ho report
 * \param bitmask bitmask
* \param src_cgi src cgi
* \param trgt cgi target cgi
* \param result result
* \param restablishment_cgi restablishment cgi
* \param serving_meas_strength
* \param target_meas_strength
* \param  offset_attr offset attribute
*/
typedef struct _rrm_son_ho_report_t
{
    rrm_bitmask_t                                   bitmask;/*^ BITMASK ^*/
    rrm_oam_eutran_global_cell_id_t                 src_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_oam_global_cell_id_t                        trgt_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_return_et                                   result;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_oam_global_cell_id_t                        restablishment_cgi;
    /*^ O, HO_REPORT_RE_EST_CGI_PRESENT, N, 0, 0 ^*/
    rrm_son_eutran_strength_t                       serving_meas_strength;
    /*^ O, HO_REPORT_SERVING_MEAS_STRENGTH_PRESENT, N, 0, 0 ^*/
    rrm_son_meas_strength_t                         target_meas_strength;
    /*^ O, HO_REPORT_TARGET_MEAS_STRENGTH_PRESENT, N, 0, 0 ^*/
    rrm_son_offset_attr_t                           offset_attr;
    /*^ O, HO_REPORT_OFFSET_ATTR_PRESENT, N, 0, 0 ^*/
}rrm_son_ho_report_t;
/*^ API, RRM_SON_HO_REPORT ^*/

/* RRM_SON_LOAD_REPORT_IND : Start */
/*! \ enum rrm_son_rs_load_lvl_et
 *  \brief This enum gives load levels
 */ 
typedef enum
{
    RRM_SON_OVERLOAD_LVL,
    RRM_SON_HIGH_LOAD_LVL,
    RRM_SON_MID_LOAD_LVL,
    RRM_SON_LOW_LOAD_LVL,

    RRM_INVALID_LOAD_LVL
}rrm_son_rs_load_lvl_et;
/*! \ rrm_son_hw_load_ind_t
 *  \brief This struct contains load index
 *  \param dl downlink
 *  \param ul uplink
*/
typedef struct _rrm_son_hw_load_ind_t
{
    rrm_son_rs_load_lvl_et    dl;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_son_rs_load_lvl_et    ul;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_son_hw_load_ind_t;
/*! \ rrm_son_s1_tnl_load_ind_t
 *  \brief This struct is used for son s1 load level indication
 *  \param dl Downlink load level
 *  \param ul Uplink load level
 */
typedef struct _rrm_son_s1_tnl_load_ind_t
{
    rrm_son_rs_load_lvl_et    dl;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_son_rs_load_lvl_et    ul;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_son_s1_tnl_load_ind_t;
/*! \ rrm_son_rrs_load_ind_t
 *  \brief This struct is used for 
 *  \param dl_gbr_prb_usage
 *  \param ul_gbr_prb_usage
 *  \param dl_non_gbr_prb_usage
 *  \param ul_non_gbr_prb_usage
 *  \param dl_total_prb_usage
 *  \param ul_total_prb_usage
 */
typedef struct _rrm_son_rrs_load_ind_t
{
    U8    dl_gbr_prb_usage;
    /*^ M, 0, H, 0, 100 ^*/
    U8    ul_gbr_prb_usage;
    /*^ M, 0, H, 0, 100 ^*/
    U8    dl_non_gbr_prb_usage;
    /*^ M, 0, H, 0, 100 ^*/
    U8    ul_non_gbr_prb_usage;
    /*^ M, 0, H, 0, 100 ^*/
    U8    dl_total_prb_usage;
    /*^ M, 0, H, 0, 100 ^*/
    U8    ul_total_prb_usage;
    /*^ M, 0, H, 0, 100 ^*/
}rrm_son_rrs_load_ind_t;

#define RRM_SON_CELL_CAP_CLASS_VAL_PRESENT 0x01
/*! \ rrm_son_comp_avl_cap_t
 *  \brief This struct is used for  AVL cap info
 *  \param bitmask
 *  \param  cell_cap_class_val
 *  \param cap_val
 */
typedef struct _rrm_son_comp_avl_cap_t
{
    rrm_bitmask_t   bitmask;            /*^ BITMASK ^*/
    U8              cell_cap_class_val;
    /*^ O, RRM_SON_CELL_CAP_CLASS_VAL_PRESENT, B, 1, 100 ^*/
    U8              cap_val;
    /*^ M, 0, H, 0, 100 ^*/
}rrm_son_comp_avl_cap_t;
/*! \ rrm_son_comp_avl_cap_grp_t
 *  \brief This struct is used for  AVL CAP group
 *  \param dl_comp_avl_cap
 *  \param ul_comp_avl_cap
 */
typedef struct _rrm_son_comp_avl_cap_grp_t
{
    rrm_son_comp_avl_cap_t  dl_comp_avl_cap;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_son_comp_avl_cap_t  ul_comp_avl_cap;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_son_comp_avl_cap_grp_t;

/** eICIC feature changes start */
#define RRM_SON_TDD_FDD_CONFIG_PRESENT      0x01
/* ! \rrm_son_abs_status_t
 *  \brief  This struct get the ABS status configuration form OAM.
 *  \param  bitmask bitmask related infortion.
 *  \param  dl_abs_status Percentage usage of ABS resources
 *  \param  tdd_fdd_config TDD configuration of the serving cell. 
 *  \param  usable_abs_pattern_info_tdd Usage pattern reported from neighbor.In case of serving 
 *          cell it will be the value presently being applied on the serving cell
 */
typedef struct _rrm_son_abs_status_t
{
    rrm_bitmask_t                       bitmask;                    /*^ BITMASK ^*/
    U8                                  dl_abs_status;              /*^ M, 0, H, 0, 100 ^*/
    U8                                  tdd_fdd_config;             /*^ O, RRM_SON_TDD_FDD_CONFIG_PRESENT, H, 0, 3 ^*/
    U8                                  usable_abs_pattern_info_tdd[RRM_MAX_PATTERN_BYTE];         
                                                                    /*^ M, 0, OCTET_STRING, FIXED ^*/

}rrm_son_abs_status_t;
/** eICIC feature changes end */
#define RRM_SON_CELL_LOAD_INFO_HW_LOAD_PRESENT      0x01
#define RRM_SON_CELL_LOAD_INFO_S1_TNL_LOAD_PRESENT  0x02
#define RRM_SON_CELL_LOAD_INFO_RRS_PRESENT          0x04
#define RRM_SON_COMP_AVL_CAP_GRP_PRESENT            0x08
/** eICIC feature changes start */
#define RRM_SON_ABS_STATUS_PRESENT                  0x10
/** eICIC feature changes end */
/*! \ rrm_son_cell_load_info_t
 *  \brief This struct is used for SON Cell Load Information
 *  \param bitmask
 *  \param g_cell_id
 *  \param hw_load
 *  \param s1_tnl_load
 *  \param rrs
 *  \param comp_avl_cap_grp
 */
typedef struct _rrm_son_cell_load_info_t
{
    rrm_bitmask_t                    bitmask;
    /*^ BITMASK ^*/
    rrm_oam_eutran_global_cell_id_t  g_cell_id;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_son_hw_load_ind_t                hw_load;
    /*^ O, RRM_SON_CELL_LOAD_INFO_HW_LOAD_PRESENT, N, 0, 0 ^*/
    rrm_son_s1_tnl_load_ind_t            s1_tnl_load;
    /*^ O, RRM_SON_CELL_LOAD_INFO_S1_TNL_LOAD_PRESENT, N, 0, 0 ^*/
    rrm_son_rrs_load_ind_t               rrs;
    /*^ O, RRM_SON_CELL_LOAD_INFO_RRS_PRESENT, N, 0, 0 ^*/
    rrm_son_comp_avl_cap_grp_t           comp_avl_cap_grp;
    /*^ O, RRM_SON_COMP_AVL_CAP_GRP_PRESENT, N, 0, 0 ^*/
    /** eICIC feature changes start */
    rrm_son_abs_status_t                abs_status;
    /*^ O, RRM_SON_ABS_STATUS_PRESENT, N, 0, 0 ^*/
    /** eICIC feature changes end */

}rrm_son_cell_load_info_t;
/*! \ rrm_son_load_report_ind_t
 *  \brief This struct contains load report index
 *  \param serv_cell_load_info serveer cell load info
 *  \param count count
 *  \param neigh_cell_load_info[RRM_MAX_NUM_NGBR_CELLS] neigh_cell_load_info
*/

typedef struct _rrm_son_load_report_ind_t
{
    rrm_son_cell_load_info_t    serv_cell_load_info;
    /*^ M, 0, N, 0, 0 ^*/
    U8                      count;
   
/*SPR_17664_start*/
    /*^ M, 0, H, 0, RRM_MAX_NUM_NGBR_CELLS ^*/

/*SPR_17664_end*/
    rrm_son_cell_load_info_t    neigh_cell_load_info[RRM_MAX_NUM_NGBR_CELLS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_son_load_report_ind_t;
/*^ API, RRM_SON_LOAD_REPORT_IND ^*/

/* MRO : Start */
/*! \ rrm_son_ho_attempt_ind_t
 *  \brief This struct contains ho attempt index
 *  \param src_cgi source cgi
 *  \param trgt_cgi target cgui
 *  \param attempt_count attempt count
*/
typedef struct _rrm_son_ho_attempt_ind_t
{
   rrm_oam_eutran_global_cell_id_t src_cgi;
    /*^ M, 0, N, 0, 0 ^*/
   rrm_oam_eutran_global_cell_id_t trgt_cgi;
    /*^ M, 0, N, 0, 0 ^*/
   /* This count will be set to 1 in present release */
   U8                              attempt_count;
    /*^ M, 0, N, 0, 0 ^*/
}rrm_son_ho_attempt_ind_t;
/*^ API, RRM_SON_HO_ATTEMPT_IND ^*/

#define HO_FAILURE_REPORT_RE_EST_CGI_PRESENT 0x01
/*! \ rrm_son_ho_failure_report_t
 *  \brief This struct gives the ho failure report
 *  \param bitmask bitmask 
 *  \param src_cgi sorce cgi
 *  \param trgt_cgi target cgi
 *  \param restablishment_cgi restablishment cgi
 *  \param ho_cause ho cause
*/
typedef struct _rrm_son_ho_failure_report_t
{
    rrm_bitmask_t                    bitmask;
    /*^ BITMASK ^*/
    rrm_oam_eutran_global_cell_id_t  src_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_oam_eutran_global_cell_id_t  trgt_cgi;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_oam_eutran_global_cell_id_t  restablishment_cgi;
    /*^ O, HO_FAILURE_REPORT_RE_EST_CGI_PRESENT, N, 0, 0 ^*/
    rrm_ho_cause_et                  ho_cause;
    /*^ M, 0, H, 0, 2 ^*/
}rrm_son_ho_failure_report_t;
/*^ API, RRM_SON_HO_FAILURE_REPORT ^*/

/* MRO : end */

/* TNL_DISCOVERY : START */
/*! \ rrm_son_tnl_discovery_req_t
 *  \brief This struct contains the info about tnl discovery request
 *  \param target_enb_id target enb id
 *  \param tac[MAX_TAC_SIZE] tac
*/
typedef struct _rrm_son_tnl_discovery_req_t
{
    rrm_global_enb_id_t     target_enb_id;
    /*^ M, 0, N, 0, 0 ^*/
    U8                      tac[MAX_TAC_SIZE] ;
    /*^ M, 0, OCTET_STRING, FIXED ^*/
}rrm_son_tnl_discovery_req_t;

/*^ API, RRM_SON_TNL_DISCOVERY_REQ ^*/
#define MAX_NO_ENB_X2_TLAS    2

/*! \ rrm_son_tnl_discovery_res_t
 *  \brief   This struct contains info about the tnl discovery result 
 *  \param result result
 *  \param error_code error_code
 *  \param target_enb_id target enb id
 *  \param enb_tnl_address_list_size enb tnl address list size
 *  \param enb_tnl_address_list[MAX_NO_ENB_X2_TLAS] enb_tnl_address_list
*/
typedef struct _rrm_son_tnl_discovery_res_t
{
    rrm_return_et           result;
    /*^ M, 0, H, 0, 1 ^*/
    rrm_error_et            error_code;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_global_enb_id_t     target_enb_id;
    /*^ M, 0, N, 0, 0 ^*/
    U8                      enb_tnl_address_list_size;

/*SPR_17664_start*/
    /*^M,0,H,0,MAX_NO_ENB_X2_TLAS^*/

/*SPR_17664_end*/
    rrm_enb_tnl_address_t   enb_tnl_address_list[MAX_NO_ENB_X2_TLAS];
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_son_tnl_discovery_res_t;
/*^ API, RRM_SON_TNL_DISCOVERY_RESP ^*/

/* TNL_DISCOVERY : ENDS */

/* RACH_OPTIMIZATION_CHANGES_START */

#define STRONG_CELL_INFO_COUNT        3
#define MAX_NUMBER_OF_PRACH_SAMPLES   1000
#define MAX_TA_RANGE                  12

typedef enum{
    RRC_CONNECTED_UE = 1,
    RRC_RE_ESTABLISHMENT_UE,
    IN_SYNC_IND_UE
}prach_cause_et;

#define L2_REPORT_PERIODICITY_PRESENT    0x01
#define UE_REPORT_PERIODICITY_PRESENT    0x02
#define NUM_STRONGEST_CELLS_PRESENT      0x04
#define RACH_SAMPLE_COUNT_PRESENT        0x08

typedef struct _rrm_son_rach_config_req_t
{
    rrm_bitmask_t                       bitmask;           /*^ BITMASK ^*/
    rrm_oam_eutran_global_cell_id_t     cell_id;           /*^ M, 0, N, 0, 0 ^*/ 
    U16                                 l2_reports_periodicity;    /*^ O, L2_REPORT_PERIODICITY_PRESENT, H, 0, 600 ^*/
    U16                                 ue_reports_periodicity;    /*^ O, UE_REPORT_PERIODICITY_PRESENT, H, 0, 600 ^*/
    U16                                 rach_sample_count; /*^ O, RACH_SAMPLE_COUNT_PRESENT, H, 0, 1000 ^*/
    U8                                  num_of_strongest_cells;    /*^ O, NUM_STRONGEST_CELLS_PRESENT, H, 0, 3 ^*/ 
}rrm_son_rach_config_req_t;
/*^ API, RRM_SON_RACH_CONFIG_REQ ^*/

typedef struct _rrm_son_rach_config_resp_t
{
    rrm_oam_eutran_global_cell_id_t      cell_id;         /*^ M, 0, N, 0, 0 ^*/    
    rrm_return_et                        result;          /*^ M, 0, N, 0, 0 ^*/
    rrm_error_et                         error_code;      /*^ M, 0, N, 0, 0 ^*/
}rrm_son_rach_config_resp_t;
/*^ API, RRM_SON_RACH_CONFIG_RESP ^*/

typedef enum
{
    cell_centre,
    cell_edge
}ue_location_et;

typedef struct _strong_cell_info_t
{
    U16     pci;    /*^ M, 0, H, 0, 503 ^*/
    U8      rsrp;   /*^ M, 0, H, 0, 128 ^*/
}strong_cell_info_t;

typedef struct _rrm_ue_prach_data_info_t
{
    prach_cause_et        cause;                    /*^ M, 0, B, 1, 3 ^*/
    U16                   number_of_preambles_sent;  /*^ M, 0, N, 0, 0 ^*/
    rrm_bool_et           contention_detected;  /*^ M, 0, N, 0, 1 ^*/
    ue_location_et        ue_location;              /*^ M, 0, H, 0, 1 ^*/

/*SPR_17664_start*/
    U8                    strong_cell_info_count;  /*^ M, 0, H, 0, STRONG_CELL_INFO_COUNT ^*/

/*SPR_17664_end*/
    strong_cell_info_t    strong_cell_info[STRONG_CELL_INFO_COUNT];   /*^ M, 0, OCTET_STRING, VARIABLE ^*/
}rrm_ue_prach_data_info_t;

typedef struct _rrm_son_rach_ue_info_ind_t
{
    rrm_oam_eutran_global_cell_id_t cell_id;         /*^ M, 0, N, 0, 0 ^*/

/*SPR_17664_start*/
/* SPR-18445 start */
    U16                             count;     /*^ M, 0, H, 0, MAX_NUMBER_OF_PRACH_SAMPLES ^*/
/* SPR-18445 stop */
   
/*SPR_17664_end*/
    rrm_ue_prach_data_info_t        rach_data[MAX_NUMBER_OF_PRACH_SAMPLES];   
    /*^ M, 0, OCTET_STRING, VARIABLE ^*/
} rrm_son_rach_ue_info_ind_t; 
/*^ API, RRM_SON_RACH_UE_INFO_IND ^*/

typedef struct _rrm_son_l2_rach_perf_report_t
{
    rrm_oam_eutran_global_cell_id_t cell_id;    /*^ M, 0, N, 0, 0 ^*/
    U16                             num_of_l2_perf_accumulated_reports; /*^ M, 0, N, 0, 0 ^*/
    U64                             total_rcvd_msg3_random_access_preambles_cbra;  /*^ M, 0, N, 0, 0 ^*/
    U64                             total_rcvd_msg3_random_access_preambles_cfra; /*^ M, 0, N, 0, 0 ^*/
    U64                             total_failed_random_access_preambles_msg2_cbra; /*^ M, 0, N, 0, 0 ^*/
    U64                             total_failed_random_access_preambles_msg2_cfra;  /*^ M, 0, N, 0, 0 ^*/
    U64                             total_allocated_random_access_preambles;    /*^ M, 0, N, 0, 0 ^*/
    U64                             total_succ_random_access_ta[MAX_TA_RANGE];  /*^ M, 0, OCTET_STRING, FIXED ^*/
    U64                             total_unassigned_false_cfra;  /*^ M, 0, N, 0, 0 ^*/
    U64                             total_unassigned_sum_cfra;  /*^ M, 0, N, 0, 0 ^*/
    /* SPR-18438 START */
    U64                             num_random_access_preambles_group_a; /*^ M, 0, N, 0, 0 ^*/
    U64                             num_random_access_preambles_group_b; /*^ M, 0, N, 0, 0 ^*/
    /* SPR-18438 END */
}rrm_son_l2_rach_perf_report_t;
/*^ API, RRM_SON_L2_RACH_PERF_REPORT ^*/

/* RACH_OPTIMIZATION_CHANGES_END */

/*SPR 10329:start*/
typedef struct _rrm_son_gu_group_id_list_t
{
    rrm_bitmask_t           bitmask; /*^ BITMASK ^*/

/*SPR_17664_start*/
    U8                      num_gu_group_id; /*^ M,0,B,1,RRM_MME_MAX_POOLS ^*/

/*SPR_17664_end*/
    rrm_oam_gu_group_id_t   gu_id_list[RRM_MME_MAX_POOLS];   /*^ M,0,OCTET_STRING,VARIABLE ^*/
}rrm_son_gu_group_id_list_t;

#define RRM_SON_GU_GROUP_ID_TO_ADD_LIST_PRESENT      0x01
#define RRM_SON_GU_GROUP_ID_TO_DELETE_LIST_PRESENT   0x02
/*! \ rrm_son_enb_config_update_ind_t
 *  \brief This struct contains GU Group ID Info of nbr eNB 
 *  \param nbr_enb_id  Neighbor eNodeB ID 
 *  \param add_gu_id   GU Group Id To Add List
 *  \param delete_gu_id  GU Group Id To Delete List
*/

typedef struct _rrm_son_enb_config_update_ind_t
{
    rrm_bitmask_t             bitmask;
    /*^ BITMASK ^*/
    rrm_global_enb_id_t       nbr_enb_id;
    /*^ M, 0, N, 0, 0 ^*/
    rrm_son_gu_group_id_list_t     add_gu_id; 
    /*^ O, RRM_SON_GU_GROUP_ID_TO_ADD_LIST_PRESENT, N, 0, 0 ^*/
    rrm_son_gu_group_id_list_t   delete_gu_id;    
    /*^ O, RRM_SON_GU_GROUP_ID_TO_DELETE_LIST_PRESENT, N, 0, 0 ^*/
}rrm_son_enb_config_update_ind_t;
/*^ API, RRM_SON_ENB_CONFIG_UPDATE_IND ^*/

/*SPR 10329:end*/

/** eICIC feature changes start */
/*! rrm_son_eicic_enable_disable_t
 * \brief RRM sends this API to SON to update the status of eICIC feature on RRM
 * \param bitmask N/A
 * \param cell_id LTE cell id
 * \param eicic_status status at RRM disable/enable eicic
 */
typedef struct _rrm_son_eicic_enable_disable_ind_t
{
    rrm_bitmask_t                     bitmask;      /*^ BITMASK ^*/          
    rrm_oam_eutran_global_cell_id_t   cell_id;		/*^ M, 0, N, 0, 0 ^*/
    rrm_son_eicic_node_type_et        eicic_provisioned_type;  /*^ M, 0, H, 0, 3 ^*/ 
}rrm_son_eicic_enable_disable_ind_t;
/*^ API, RRM_SON_EICIC_ENABLE_DISABLE_IND ^*/
/** eICIC feature changes end */
/* SPR 22248 MBMS Changes Start */

/*! rrm_cell_sib2_mbsfn_data_t 
 * \brief RRM sends this API to SON to provide information regarding the MBMS MTCH configuration 
 * \param rrm_mbsfn_subframe_config_list defines the subframes that are reserved for MBSFN in downlink 
 */

#define RRM_SON_MBMS_CONFIG_APPLY_PRESENT      0x01 
typedef struct _rrm_cell_sib2_mbsfn_data_t
{
    rrm_bitmask_t                       bitmask; /*^ BITMASK ^*/
    rrm_oam_eutran_global_cell_id_t     cgi;    /*^ M, 0, N, 0, 0 ^*/
    rrm_mbsfn_subframe_config_list_t    rrm_mbsfn_subframe_config_list;  
        /*^ O, RRM_SON_MBMS_CONFIG_APPLY_PRESENT, N, 0, 0 ^*/ 
}rrm_cell_sib2_mbsfn_data_t;

/*! rrm_son_mbms_sf_info_ind 
 * \brief RRM sends this API to SON to provide information regarding the MBMS MTCH configuration 
 * \param rrm_mbsfn_subframe_config_list defines the subframes that are reserved for MBSFN in downlink 
 */

typedef struct _rrm_son_mbms_sf_info_ind_t
{
    U8                          count;                /*^ M, 0, B, 1, 8 ^*/
    rrm_cell_sib2_mbsfn_data_t  cell_sib2_mbsfn_data[RRM_MAX_NUM_CELLS]; /*^ M, 0, OCTET_STRING, VARIABLE ^*/ 
}rrm_son_mbms_sf_info_ind_t;
/*^ API, RRM_SON_MBMS_SF_INFO_IND ^*/
/* SPR 22248 MBMS Changes Stop */
#endif /* _RRM_SON_INTF_H*/
