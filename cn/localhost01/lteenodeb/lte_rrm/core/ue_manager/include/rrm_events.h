/******************************************************************************
 *
 *   FILE NAME: rrm_events.h
 *
 *   DESCRIPTION:
 *       This file contains 
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   20 Nov 2012     gur19685    ---------       Initial
 *
 *   Copyright (c) 2012, Aricent Inc. All Rights Reserved
 *
 ******************************************************************************/

#ifndef _RRM_EVENTS_H_
#define _RRM_EVENTS_H_

/*External header files to be included*/
#include "rrm_event_defines.h"
#include "rrc_defines.h"
#include "rrm_defines.h"
#include "rrm_utils.h"
#include "rrm_uem_rrmim_intf.h"
#include "rrm_rrc_ue_intf.h"

/*+Fix for internal logger compilation error +*/
extern S8 *p_g_rrm_event_facility_name;
/*-Fix for internal logger compilation error -*/
/**********************************************************
    		  EVENT APIs START
 **********************************************************/

/*! \typedef  rrm_event_time_stamp_t
 *  \brief    Time Stamp Format
 */
typedef struct _rrm_event_time_stamp_t
{
  U16   year;
  U16   month;
  U16   day;
  U16   hour;
  U16   min;
  U16   sec;
}rrm_event_time_stamp_t;

/*! \typedef  rrm_event_header_t
 *  \brief    common event header
 */
typedef struct _rrm_event_header_t
{
  rrm_event_time_stamp_t   time_stamp;
  U32   event_class;
  U32   event_sub_class;
  /* coverity : CID 29667*/
  rrm_event_message_id_et event_id;
}rrm_event_header_t;

/*! \typedef rrm_event_local_rrc_setup_ready_t
 *  \brief   
 */
typedef struct _rrm_event_local_rrc_setup_ready_t
{
  rrm_event_header_t  msg_header;
}rrm_event_local_rrc_setup_ready_t;

/*! \typedef  rrm_event_local_erab_setup_attempt_t 
 *  \brief    The event is triggered at reception of erab stup request 
 *            and sent per E-RAB in the request 
 */
typedef struct  _rrm_event_local_erab_setup_attempt_t
{
     rrm_event_header_t    msg_header;
     U8                    cell_id;
     U8                    requested_qci[MAX_ERAB_ID];  /* Requested QCI for each ERAB */
     U8                    priority_level[MAX_ERAB_ID];  /* ARP Priority Level for each ERAB */
     U64                   ul_admitted_gbr; /* Requested UL bit rate for GBR bearers */
     U64                   dl_admitted_gbr; /* Requested DL bit rate for GBR bearers */
     U8                    preemption_capability[MAX_ERAB_ID]; /* ARP PCI for each E-RAB */
     U8                    preemption_vulnerability[MAX_ERAB_ID]; /* ARP PVI  for each E-RAB */
}rrm_event_local_erab_setup_attempt_t;

/*! \typedef  rrm_event_local_erab_setup_ready_t
 *  \brief    The event is triggered at reception of erab stup confirm
 *            and sent per E-RAB in the request.
 */
typedef struct _rrm_event_local_erab_setup_ready_t
{
   rrm_event_header_t      msg_header;
   U8                      cell_id;
   U8                      erab_setup_result[MAX_ERAB_ID]; /* Setup result for each ERAB */
   U64                     ul_admitted_gbr; /* Requested UL bit rate for GBR bearers */
   U64                     dl_admitted_gbr; /* Requested DL bit rate for GBR bearers */
}rrm_event_local_erab_setup_ready_t;

/*! \typedef  rrm_event_local_erab_admission_reject_t
 *  \brief    The event is triggered when ERAB setup 
 *            Request was rejected by RRM. 
 */
typedef struct _rrm_event_local_erab_admission_reject_t
{
   rrm_event_header_t       msg_header;
   U8                       cell_id;
   U8                       erab_id;
   U8                       erab_setup_result; /* Setup result for each ERAB */
}rrm_event_local_erab_admission_reject_t;



/*! \typedef  rrm_event_local_erab_drb_release_t
 *  \brief    The event is triggered when a DRB has 
 *            successfully been released.
 */
typedef struct _rrm_event_local_erab_drb_release_t
{
  rrm_event_header_t        msg_header;
  U8                        cell_id;
  U8                        erab_id_released[MAX_ERAB_ID];
}rrm_event_local_erab_drb_release_t;



/*! \typedef  rrm_event_local_erab_release_attempt_t
 *  \brief    The event is triggered at the 
 *            reception of a erab release request.
 */
typedef struct _rrm_event_local_erab_release_attempt_t
{
  rrm_event_header_t        msg_header;
  U8                        cell_id;
  U8                        erab_id_to_release[MAX_ERAB_ID]; /* ERAB Ids that was released */
  U8                        cause_group[MAX_ERAB_ID];
  U8                        cause_value[MAX_ERAB_ID];
}rrm_event_local_erab_release_attempt_t;

/*! \typedef  rrm_event_local_erab_release_ready_t
 *  \brief    The event is triggered at the
 *	       reception of a erab release confirm.
 */
typedef struct _rrm_event_local_erab_release_ready_t
{
  rrm_event_header_t        msg_header;
  U8                        cell_id;
  U8                        succ_erab_id[MAX_ERAB_ID]; /* position indicates UE ERAB Ids that was released */
}rrm_event_local_erab_release_ready_t;

/*! \typedef  rrm_event_local_cell_setup_reject_t
 *  \brief    The event is triggered when cell setup 
 *            procedure fails.
 */
typedef struct _rrm_event_local_cell_setup_reject_t
{
  rrm_event_header_t              msg_header;
  U8                              cell_id;
  rrm_cell_setup_fail_cause_et    fail_cause; 
}rrm_event_local_cell_setup_reject_t;

/*! \typedef  rrm_event_local_event_ue_capability_t
 *  \brief    The event is triggered at the reception 
 *            of ue capability indiaction.
 */
typedef struct _rrm_event_local_event_ue_capability_t
{
 rrm_event_header_t             msg_header;
 U8                             cell_id;
// rrc_access_stratum_release_et  access_stratum_release;
 U8                             access_stratum_release;
 U8                             ue_category;
 U8                             fgi_bitmap[4];
}rrm_event_local_event_ue_capability_t;


/*! \typedef  rrm_event_local_ue_capability_fgi_reject_t
 *  \brief    The event is triggered when erab setup 
 *            reject due to FGI bits.
 */
typedef struct _rrm_event_local_ue_capability_fgi_reject_t
{
  rrm_event_header_t         msg_header;
  U8                         cell_id;
  /*Start:Bug 762*/
  U8                         fgi_bits[RRM_FGI_BYTES];
  /*End:Bug 762*/
}rrm_event_local_ue_capability_fgi_reject_t;


/*! \typedef  rrm_event_local_erab_modify_attempt_t
 *  \brief    The event is triggered when erab modify 
 *            request is received.
 */
typedef struct _rrm_event_local_erab_modify_attempt_t
{
  rrm_event_header_t      msg_header;
  U8                      cell_id;
  U8                      rqstd_qci[MAX_ERAB_ID];
  /*Not required Since we need to send result in erab_modify_ready event
  U8                      modify_result[MAX_ERAB_ID;*/
}rrm_event_local_erab_modify_attempt_t;

/*! \typedef  rrm_event_erab_modify_ready_t
 *  \brief    The event is triggered when erab modify request 
 *            message has been processed.
 */
typedef struct _rrm_event_erab_modify_ready_t
{
  rrm_event_header_t      msg_header;
  U8                      cell_id;
  U8                      modify_result[MAX_ERAB_ID];
}rrm_event_erab_modify_ready_t;

/*! \typedef  rrm_event_erab_drb_allocated_t
 *  \brief    The event is triggered when a 
 *            DRB has successfully been allocated
 */
typedef struct _rrm_event_erab_drb_allocated_t
{
  rrm_event_header_t      msg_header;
  U8                      cell_id;
  U8                      drb_allocated[MAX_ERAB_ID];
}rrm_event_erab_drb_allocated_t;

/*! \typedef  rrm_event_enb_config_transfer_t
 *  \brief    when the S1 message "eNB CONFIGURATION TRANSFER" 
 *            is sent by RRM towards MME
 */
typedef struct _rrm_event_enb_config_transfer
{
   rrm_event_header_t    msg_header;
   U8                    cell_id;
}rrm_event_enb_config_transfer_t;

/*! \typedef  rrm_event_mme_config_transfer_t
 *  \brief    triggered when the S1 message 
 *            "MME CONFIGURATION TRANSFER" is received
 */
typedef struct _rrm_event_mme_config_transfer
{
   rrm_event_header_t   msg_header;
   U8                   cell_id;
}rrm_event_mme_config_transfer_t;


/*! \typedef  rrm_event_srb1_setup_reject_t
 *  \brief    triggered in case of a hand-in and if the SRB1 can not be
 *            established for the UE due to resource constrain
 */
typedef struct _rrm_event_srb1_setup_reject
{
    rrm_event_header_t 	   msg_header;
    U8                     cell_id;
    rrc_handover_type_et   ho_type;   //handover type enum is defined in rrc_defines.h 
}rrm_event_srb1_setup_reject_t;


/*! \typedef rrm_event_erab_setup_qci_t
 *  \brief   triggered when ERAB setup Request arrives at RRM
 */
typedef struct _rrm_event_erab_setup_qci
{
   rrm_event_header_t 	msg_header;
   U8                   cell_id;
   U8  	                erab_id;
   U8  		        qci;
}rrm_event_erab_setup_qci_t;



/*! \typedef  rrm_event_meas_config_a1_t
 *  \brief    triggered whenever a UE is configured through the 
 *            "RRC Connection Reconfiguration message" to report 
 *             measurements on EUTRA of type Event A1
 */
typedef struct _rrm_event_meas_config_a1_t
{
     rrm_event_header_t             msg_header;
     U8                             cell_id;
//   U8                             measurement_id;
     U8                             meas_object_id;
//   rrm_meas_requester_et          meas_requester;
//   U8                             eutra_threshold1;
     rrm_time_to_trigger_et         time_to_trigger;
     U8                             hysteresis;
//   rrm_meas_trigger_quantity_et   trigger_quantity;
     rrm_report_quantity_et         report_quantity;
     U8                             max_report_cells;
     rrm_meas_report_interval_et    report_interval;
     rrm_meas_report_amount_et      report_amount;
}rrm_event_meas_config_a1_t;

/*! \typedef  rrm_event_meas_config_a2_t
 *  \brief    triggered whenever a UE is configured through the
 *            "RRC Connection Reconfiguration message" to report
 *            measurements on EUTRA of type Event A2
 */
typedef struct  _rrm_event_meas_config_a2_t
{
     rrm_event_header_t             msg_header;
     U8                             cell_id;
//     U8                             measurement_id;
     U8                             meas_object_id;
//     rrm_meas_requester_et          meas_requester;
//     U8                             eutra_threshold1;
     rrm_time_to_trigger_et         time_to_trigger;
     U8                             hysteresis;
//     rrm_meas_trigger_quantity_et   trigger_quantity;
     rrm_report_quantity_et         report_quantity;
     U8                             max_report_cells;
     rrm_meas_report_interval_et    report_interval;
     rrm_meas_report_amount_et      report_amount;
}rrm_event_meas_config_a2_t;


/*! \typedef  rrm_event_meas_config_a3_t
 *  \brief    triggered whenever a UE is configured through the
 *            "RRC Connection Reconfiguration message" to report
 *            measurements on EUTRA of type Event A3
 */
typedef struct  _rrm_event_meas_config_a3_t
{
     rrm_event_header_t             msg_header;
     U8                             cell_id;
     U8                             measurement_id;
     U8                             meas_object_id;
//     rrm_meas_requester_et          meas_requester;
     S8                             event_a3_offset;
     rrm_time_to_trigger_et         time_to_trigger;
     U8                             hysteresis;
//     rrm_meas_trigger_quantity_et   trigger_quantity;
     rrm_report_quantity_et         report_quantity;
     U8                             max_report_cells;
     rrm_meas_report_interval_et    report_interval;
     rrm_meas_report_amount_et      report_amount;
}rrm_event_meas_config_a3_t;

/*! \typedef  rrm_event_meas_config_a4_t
 *  \brief    triggered whenever a UE is configured through the
 *            "RRC Connection Reconfiguration message" to report
 *            measurements on EUTRA of type Event A4
 */
typedef struct  _rrm_event_meas_config_a4_t
{
     rrm_event_header_t             msg_header;
     U8                             cell_id;
     U8                             measurement_id;
     U8                             meas_object_id;
//     rrm_meas_requester_et          meas_requester;
//     U8                             eutra_threshold1;
     rrm_time_to_trigger_et         time_to_trigger;
     U8                             hysteresis;
//     rrm_meas_trigger_quantity_et   trigger_quantity;
     rrm_report_quantity_et         report_quantity;
     U8                             max_report_cells;
     rrm_meas_report_interval_et    report_interval;
     rrm_meas_report_amount_et      report_amount;
}rrm_event_meas_config_a4_t;


/*! \typedef  rrm_event_meas_config_a5_t
 *  \brief    triggered whenever a UE is configured through the
 *            "RRC Connection Reconfiguration message" to report
 *            measurements on EUTRA of type Event A5
 */
typedef struct _rrm_event_meas_config_a5_t
{
     rrm_event_header_t             msg_header;
     U8                             cell_id;
//     U8                             measurement_id;
//     U8                             meas_object_id;
//     rrm_meas_requester_et          meas_requester;
//     U8                             eutra_threshold1;
//     U8                             eutra_threshold2;
     rrm_time_to_trigger_et         time_to_trigger;
     U8                             hysteresis;
//     rrm_meas_trigger_quantity_et   trigger_quantity;
     rrm_report_quantity_et         report_quantity;
//     U16                            arfcn;
     U8                             max_report_cells;
     rrm_meas_report_interval_et    report_interval;
     rrm_meas_report_amount_et      report_amount;
}rrm_event_meas_config_a5_t;

/*! \typedef  rrm_event_local_erab_release_info_t
 *  \brief    The event is triggered when all DRBS 
 *            have been released for a request & 
 *            RRM received rab release cnf message
 */
typedef struct _rrm_event_local_erab_release_info_t
{
  rrm_event_header_t  msg_header;
  U8                  cell_id;
  U8                  succful_released_erab_count; /* Number of successfully released ERABs. */
  U8                  requested_qci[MAX_ERAB_ID];  /* Array with QCI values for each ERAB in a release */  
  U8                  priority_level[MAX_ERAB_ID]; /* ARP Priority Level for each ERAB in the request */
  U8                  preemption_capability[MAX_ERAB_ID];    /* ARP PCI  for each E-RAB in the request */
  U8                  preemption_vulnerability[MAX_ERAB_ID]; /* ARP PVI  for each E-RAB in the request */
}rrm_event_local_erab_release_info_t;

/*! \typedef  rrm_event_local_erab_setup_info_t
 *  \brief    This event is triggered when transport 
 *            and DRBs has been allocated for each ERAB 
 *            in the request.
 */
typedef struct _rrm_event_local_erab_setup_info_t
{
  rrm_event_header_t   msg_header;
  U8                   cell_id;
  U8                   erab_setup_result[MAX_ERAB_ID]; /*element value indicates the result for each E-RAB */   
}rrm_event_local_erab_setup_info_t;

/*! \typedef  rrm_event_local_ho_in_prep_attempt_t
 *  \brief    The event is triggered when an incoming
 *            handover preparation is received.
 */
typedef struct _rrm_event_local_ho_in_prep_attempt_t
{
  rrm_event_header_t    msg_header;
  U8                    cell_id;
  rrc_handover_type_et  ho_type;  /* type of handover */
  U8                    ho_in_prep_erab[MAX_ERAB_ID]; /* Position indicates the E-RAB Id was included in the request */
  U8                    priority_level[MAX_ERAB_ID]; /* ARP Priority Level for each ERAB in the request */
  U8                    preemption_capability[MAX_ERAB_ID]; /* ARP PCI  for each E-RAB in the request */
  U8                    preemption_vulnerability[MAX_ERAB_ID]; /* ARP PVI  for each E-RAB in the request */
}rrm_event_local_ho_in_prep_attempt_t;

/*! \typedef  rrm_event_local_ho_in_prep_failure_t
 *  \brief    The event is triggered when an incoming 
 *            handover preparation failure is received.
 */
typedef struct _rrm_event_local_ho_in_prep_failure_t
{
  rrm_event_header_t         msg_header;
  U8                         cell_id;
  rrm_event_ho_src_rat_et    ho_src;
  rrm_event_ho_prep_fail_cause_et  ho_prep_fail_cause; /*Cause value for HO Prep fail */
}rrm_event_local_ho_in_prep_failure_t;


/*! \typedef  rrm_event_ho_out_prep_attempt_t
 *  \brief    When an outgoing handover preparation 
 *            UE task is requested
 */
typedef struct _rrm_event_ho_out_prep_attempt_t
{
     rrm_event_header_t               msg_header;
     U8                               cell_id;
//     U8                               cellrelation_froid;
     rrm_ho_source_target_type_et     ho_source_target_rat;
     rrm_ho_target_selection_type_et  ho_target_selection_type;
//     U8                               neighbour_cgi[8];
     rrm_ho_type_et                   ho_type;
     rrmc_meas_report_event_et        ho_prep_attempt_cause;
}rrm_event_ho_out_prep_attempt_t;


/*! \typedef  rrm_event_ho_out_prep_failed_t
 *  \brief    When the outgoing handover preparation fails
 */
typedef struct _rrm_event_ho_out_prep_failed_t
{
     rrm_event_header_t                msg_header;
     U8                                cell_id;
//   U8                                cellrelation_froid;
//   U8                                neighbour_cgi[8];
     U8                                ho_prep_fail_cause_type;
     U16                               ho_prep_fail_cause_value;
     rrm_ho_target_selection_type_et   ho_target_selection_type;
}rrm_event_ho_out_prep_failed_t;


/*! \typedef  rrm_event_ho_out_prep_info_t
 *  \brief    When an outgoing handover 
 *            request is to be sent
 */
typedef struct _rrm_event_ho_out_prep_info_t
{
     rrm_event_header_t              msg_header;
     U8                              cell_id;
     U16                             erab_bitmap;
     U8                              num_drab_requested;
     U16                             erab_req_bitmap;
}rrm_event_ho_out_prep_info_t;


/*! \typedef  rrm_event_ho_out_exec_attempt_t
 *  \brief    When an outgoing handover execution
 *            UE task is requested
 */
typedef struct _rrm_event_ho_out_exec_attempt_t
{
     rrm_event_header_t             msg_header;
     U8                             cell_id;
     U8                             cellrelation_froid;
     U8                             ho_target_rat;
     U8                             ho_target_selection_type;
     U8                             neighbour_cgi[8];
     U8                             ho_type;
     U8                             drx_config_index;
     U8                             erab_fail_bitmap;
     U8                             ho_packet_forward;
}rrm_event_ho_out_exec_attempt_t;


/*! \typedef  rrm_event_ho_out_exec_failed_t
 *  \brief    When an outgoing handover execution fails
 */
typedef struct _rrm_event_ho_out_exec_failed_t
{
     rrm_event_header_t               msg_header;
     U8                               cell_id;
//   U8                               cellrelation_froid;
//   U8                               ho_target_rat;
//   U8                               neighbour_cgi[8];
     U8                               ho_exec_fail_cause_type;
     U16                              ho_exec_fail_cause_value;
     rrm_ho_target_selection_type_et  ho_target_selection_type;

}rrm_event_ho_out_exec_failed_t;


/*! \typedef  rrm_event_ho_out_exec_complete_t
 *  \brief    When an outgoing handover execution 
 *            UE task is confirmed 
 */
typedef struct _rrm_event_ho_out_exec_complete_t
{
     rrm_event_header_t              msg_header;
     U8                              cell_id;
//     U8                            cellrelation_froid;
//     U8                            neighbour_cgi[8];
}rrm_event_ho_out_exec_complete_t;

/*! \typedef  rrm_event_local_ho_in_exec_complete_t
 *  \brief    The event is triggered when
 */
typedef struct _rrm_event_local_ho_in_exec_complete_t
{
  rrm_event_header_t     msg_header;
  U8                     cell_id;
}rrm_event_local_ho_in_exec_complete_t;


/*! \typedef  rrm_event_local_ho_in_prep_complete_t
 *  \brief    The event is triggered when
 *            
 */
typedef struct _rrm_event_local_ho_in_prep_complete_t
{
  rrm_event_header_t   msg_header;
  U8                   cell_id;
  U16                  erab_bitmap;      /*Bitmap pointing out UE ERAB Ids. (LSB)Bit position 0=ERAB Id 0*/
  U8                   erab_admtd_count; /* The number of DRBs received */
}rrm_event_local_ho_in_prep_complete_t;


/*! \typedef  rrm_event_local_ho_in_exec_attempt_t
 *  \brief    The event is triggered HO Admission Resp 
 *            is send successfully to RRC (Only Success case).
 */
typedef struct _rrm_event_local_ho_in_exec_attempt_t
{
  rrm_event_header_t     msg_header;
  U8                     cell_id;
  rrc_handover_type_et   ho_type;  /* type of handover */
}rrm_event_local_ho_in_exec_attempt_t;

/*! \typedef  rrm_event_local_ho_in_exec_failed_t
 *  \brief    The event is triggered when
 */
typedef struct _rrm_event_local_ho_in_exec_failed_t
{
  rrm_event_header_t   msg_header;
  U8                   cell_id;
  U8                   fail_cause;
  U8                   rat_type;
}rrm_event_local_ho_in_exec_failed_t;

/*! \typedef  rrm_event_ho_ue_detected_t
 *  \brief    When an rrc_connection_reconfig complete
 *            for target eNB
 */
typedef struct _rrm_event_ho_ue_detected_t
{
    rrm_event_header_t               msg_header;
    U8                               cell_id;
    /*U8                               random_access_type;*/
}rrm_event_ho_ue_detected_t;


/*! \typedef  rrm_event_ho_out_exec_info_t
 *  \brief    When an rrc_connection_reconfig complete
 *            for target eNB
 */
typedef struct _rrm_event_ho_out_exec_info_t
{

    rrm_event_header_t               msg_header;
    U8                               cell_id;
    U16                              ho_out_prep_erab_req_bitmap;
    U16                              ho_out_exec_erab_req_bitmap;
/*    U8                               drx_config_index;*/

}rrm_event_ho_out_exec_info_t;


/*! \typedef rrm_event_meas_config_b2_geran_t
 *  \brief  RRM shall trigger this event whenever a UE is configured
 *          through the "RRC Connection Reconfiguration message" to
 *          report measurements on GERAN of type Event B2.
 */
typedef struct _rrm_event_meas_config_b2_geran_t
{
     rrm_event_header_t             msg_header;
     U8                             cell_id;
     U8                             measurement_id;
     U8                             meas_object_id;
/*   rrm_meas_requester_et          meas_requester;*/
     U8                             eutra_threshold1;
     U8                             geran_threshold2;
     rrm_time_to_trigger_et         time_to_trigger;
     U8                             hysteresis;
     rrm_trigger_quantity_et        trigger_quantity;
/*   rrm_band_indicator_geran_et    geran_frequency_group;*/
     U8                             max_report_cells;
     rrm_meas_report_interval_et    report_interval;
     rrm_meas_report_amount_et      report_amount;

}rrm_event_meas_config_b2_geran_t;


/*! \typedef rrm_event_meas_config_b2_utra_t
 *  \brief  RRM shall trigger this event whenever a UE is configured
 *          through the "RRC Connection Reconfiguration message" to
 *          report measurements on UTRA of type Event B2.
 */
typedef struct _rrm_event_meas_config_b2_utra_t
{
     rrm_event_header_t                msg_header;
     U8                                cell_id;
     U8                                measurement_id;
     U8                                meas_object_id;
/*   rrm_meas_requester_et             meas_requester;*/
     U8                                eutra_threshold1;
     S8                                threshold2_rscp;
     U8                                threshold2_ecn0;
     rrm_time_to_trigger_et            time_to_trigger;
     U8                                hysteresis;
     rrm_trigger_quantity_et           trigger_quantity;
     rrm_trigger_quantity_utra_et      trigger_quantity_utra;
/*   U16                               arfcn_dl;*/
     U8                                max_report_cells;
     rrm_meas_report_interval_et       report_interval;
     rrm_meas_report_amount_et         report_amount;

}rrm_event_meas_config_b2_utra_t;


/*! \typedef rrm_event_ue_meas_abort_t
 *  \brief  RRM shall generate this event whenever an ongoing
 *          UE measurement in a UE is stopped.
 */
typedef struct _rrm_event_ue_meas_abort_t
{
     rrm_event_header_t             msg_header;
     U8                             cell_id;
     U8                             measurement_id;

}rrm_event_ue_meas_abort_t;

/*-----MRO FEATURE SPECIFIC EVENTS - STARTS-----*/

/*! \typedef rrm_event_too_early_ho_t
 *  \brief   This shall be triggered by RRM
 *           when it detects a too early HO
 */
typedef struct  _rrm_event_too_early_ho_t
{
     rrm_event_header_t                msg_header;
     U8                                cell_id;
     rrm_rat_type_et                   target_rat_type;
}rrm_event_too_early_ho_t;

/*! \typedef rrm_event_too_late_ho_t
 *  \brief   This shall be triggered by RRM
 *           when it detects a too late HO
 */
typedef struct  _rrm_event_too_late_ho_t
{
     rrm_event_header_t                msg_header;
     U8                                cell_id;
     rrm_rat_type_et                   target_rat_type;
/*     rrm_ho_initiated_before_rlf_et    ho_initiated;*/
}rrm_event_too_late_ho_t;


/*! \typedef rrm_event_ho_wrong_cell_t
 *  \brief   This shall be triggered by RRM
 *           when it detects HO to a wrong cell
 */
typedef struct  _rrm_event_ho_wrong_cell_t
{
     rrm_event_header_t                msg_header;
     U8                                cell_id;
     rrm_rat_type_et                   target_rat_type;
/*     rrm_ho_completed_before_rlf_et    ho_completed;*/
}rrm_event_ho_wrong_cell_t;


/*! \typedef rrm_event_ho_wrong_cell_reest_t
 *  \brief   This shall be triggered by RRM when it detects HO
 *           to a wrong cell and it provides the identity of the
 *           cell where the RRC connection reestblishment occurred
 */
typedef struct  _rrm_event_ho_wrong_cell_reest_t
{
     rrm_event_header_t                msg_header;
     U8                                cell_id;
     rrm_rat_type_et                   target_rat_type;
}rrm_event_ho_wrong_cell_reest_t;

/*! \typedef rrm_event_x2_rlf_indication_t
 *  \brief   This shall be triggered by RRM
 *           when it receives the X2 RLF indication
 *           from a peer eNB
 */
typedef struct  _rrm_event_x2_rlf_indication_t
{
     rrm_event_header_t                msg_header;
}rrm_event_x2_rlf_indication_t;

/*-----MRO FEATURE SPECIFIC EVENTS - ENDS-----*/

/*----- PERIODICAL MEASUREMENT SPECIFIC EVENTS - START ----*/

/*! \typedef rrm_event_meas_config_periodical_cdma_t
 *  \brief   When a UE is to be configured to
 *           report measurements on CDMA with
 *           Report Configuration of type Periodical
 */
typedef struct  _rrm_event_meas_config_periodical_cdma_t
{
     rrm_event_header_t                                msg_header;
     U8                                                cell_id;
     U8                                                measurement_id;
     U8                                                meas_object_id;
     /*rrm_meas_requester_et                           meas_requester;*/
     rrm_trigger_type_periodical_purpose_et            periodical_purpose;
     /*U8                                              geran_frequency_group;*/
     U8                                                event_param_max_report_cells;
     rrm_meas_report_interval_et                       report_interval;
     rrm_meas_report_amount_et                         report_amount;
}rrm_event_meas_config_periodical_cdma_t;

/*! \typedef rrm_event_meas_config_periodical_geran_t
 *  \brief   When a UE is to be configured to
 *           report measurements on GERAN with
 *           Report Configuration of type Periodical
 */
typedef struct  _rrm_event_meas_config_periodical_geran_t
{
     rrm_event_header_t                                msg_header;
     U8                                                cell_id;
     U8                                                measurement_id;
     U8                                                meas_object_id;
     /*rrm_meas_requester_et                           meas_requester;*/
     rrm_trigger_type_periodical_purpose_et            periodical_purpose;
     /*U8                                              geran_frequency_group;*/
     U8                                                event_param_max_report_cells;
     rrm_meas_report_interval_et                       report_interval;
     rrm_meas_report_amount_et                         report_amount;
}rrm_event_meas_config_periodical_geran_t;


/*! \typedef rrm_event_meas_config_periodical_utra_t
 *  \brief   When a UE is to be configured to
 *           report measurements on UTRAN with
 *           Report Configuration of type Periodical
 */
typedef struct  _rrm_event_meas_config_periodical_utra_t
{
     rrm_event_header_t                                 msg_header;
     U8                                                 cell_id;
     U8                                                 measurement_id;
     U8                                                 meas_object_id;
     /*rrm_meas_requester_et                            meas_requester;*/
     rrm_trigger_type_periodical_purpose_et             periodical_purpose;
     /*U16                                              utra_arfcn_value_dl;*/
     U8                                                 event_param_max_report_cells;
     rrm_meas_report_interval_et                        report_interval;
     rrm_meas_report_amount_et                          report_amount;
}rrm_event_meas_config_periodical_utra_t;


/*! \typedef rrm_event_meas_config_periodical_eutran_t
 *  \brief   When a UE is to be configured to
 *           report measurements on EUTRAN with
 *           Report Configuration of type Periodical
 */
typedef struct  _rrm_event_meas_config_periodical_eutran_t
{
     rrm_event_header_t                                 msg_header;
     U8                                                 cell_id;
     U8                                                 measurement_id;
     U8                                                 meas_object_id;
     /*rrm_meas_requester_et                            meas_requester;*/
     rrm_trigger_type_periodical_purpose_et             periodical_purpose;
     U8                                                 event_param_max_report_cells;
     rrm_meas_report_interval_et                        report_interval;
     rrm_meas_report_amount_et                          report_amount;
}rrm_event_meas_config_periodical_eutran_t;
/*----- PERIODICAL MEASUREMENT SPECIFIC EVENTS - END ----*/

#define  RRM_ERROR_EVENT_CAUSE_PRESENT      0x01
#define  RRM_ERROR_EVENT_SEVERITY_PRESENT   0x02
#define  RRM_ERROR_EVENT_DATA_PRESENT       0x04
typedef struct _rrm_event_error_info_t
{
  rrm_bitmask_t               bitmask;
  rrm_error_event_type_et     error_type;
  rrm_error_event_cause_et    error_cause; /*OPTIONAL*/
  rrm_error_event_severity_et severity;    /*OPTIONAL*/
  U8                          associtated_data[RRM_ERR_EVENT_MAX_ASSOCIATED_DATA_LEN]; /*OPTIONAL*/
}rrm_event_error_info_t;
/**********************************************************
                  EVENT APIs END
 **********************************************************/


/*Function Prototypes*/
/***********************************************************************
 * Function Name  : rrm_fill_time_stamp
 * Inputs         : time_stamp
 * Returns        : void
 * Description    : This function will fill the timestamp
**********************************************************************/
void
rrm_fill_time_stamp(rrm_event_time_stamp_t  *time_stamp);


/***********************************************************************
 * Function Name  : rrm_fill_event_notification_header
 * Inputs         :
 * Returns        : void
 * Description    : This function will fill the event notification header
**********************************************************************/
/* Coverity_ID : 54431 */
void
rrm_fill_event_notification_header(rrm_event_header_t  *p_event_header,
                                   rrm_event_message_id_et    event_id);

/***********************************************************************
 * Function Name  : rrm_find_protocol_event_index
 * Inputs         : event_id
 * Returns        : U8
 * Description    : This function find protocol event index in the 
 *                  global context.
**********************************************************************/
U8 rrm_find_protocol_event_index(U16  event_id);


/****** protocol event raise function fucntion prototypes START ********/
void rrm_raise_event_ue_cap_fgi_reject(void);/*SPR 17777 +-*/
void rrm_raise_event_erab_adm_rej(U8                          erab_id,
                                  rrm_erab_adm_rej_result_et  reject_result);
                                  /*SPR 17777 +-*/

void rrm_raise_event_rrc_setup_ready(void);/*SPR 17777 +-*/ 
void rrm_raise_event_erab_setup_attempt(
                                        /*SPR 17777 +-*/
                                        rrc_rrm_erb_setup_req_t  *p_erb_setup_req);

void rrm_raise_event_erab_modify_attempt(
                                        /*SPR 17777 +-*/
                                         rrc_rrm_erab_modify_req_t *rrc_ue_erb_modify_req);

void rrm_raise_event_erab_setup_ready(rrm_ue_context_t  *p_ue_context);
void rrm_raise_event_erab_modify_ready(rrm_ue_context_t *p_rrm_ue_context);
void rrm_raise_event_erab_drb_release_attempt(
     /*SPR 17777 +-*/
                                              rrc_rrm_erb_release_req_t *rrc_ue_erb_release_req);
void rrm_raise_event_erab_release_ready(rrm_ue_context_t *p_rrm_ue_context);
void rrm_raise_event_erab_drb_release(
                                      rrc_rrm_erb_release_cnf_t *rrc_ue_erb_release_cnf);

void rrm_raise_event_ue_capability(
     /*SPR 17777 +-*/
                                   U8               access_stratum_release,
                                   U8               ue_category,
                                   U8               fgi_bitmap[4]);

void rrm_raise_event_cell_setup_reject(rrm_cell_setup_fail_cause_et fail_cause);
void rrm_raise_event_erab_drb_allocated(
                                        rrc_rrm_erb_setup_cnf_t *rrc_ue_erb_setup_cnf);

/* + SPR 17439 */
void rrm_raise_event_enb_config_transfer(void);
void rrm_raise_event_mme_config_transfer(void);
/* - SPR 17439 */
void rrm_raise_event_srb1_setup_reject(U8               ho_type);
     /*SPR 17777 +-*/

void rrm_raise_event_erab_setup_qci(U8               erab_id,
                                    U8               qci);
     /*SPR 17777 +-*/


void rrm_raise_event_meas_config_a1( rrm_ue_context_t *p_rrm_ue_context);
void rrm_raise_event_meas_config_a2( rrm_ue_context_t *p_rrm_ue_context);
void rrm_raise_event_meas_config_a3( rrm_ue_context_t *p_rrm_ue_context);
void rrm_raise_event_meas_config_a4( rrm_ue_context_t *p_rrm_ue_context);
void rrm_raise_event_meas_config_a5( rrm_ue_context_t *p_rrm_ue_context);
void rrm_raise_event_erab_release_info(rrm_ue_context_t             *p_rrm_ue_context,
                                       rrm_ue_proc_erab_cnf_data_t  *p_rrm_ue_proc_erab_cnf_data);

void rrm_raise_event_erab_setup_info(rrm_ue_context_t    *p_rrm_ue_context);
void rrm_raise_event_ho_in_prep_attempt(rrc_rrm_ue_ho_adm_req_t   *p_ho_adm_req_msg);
void rrm_raise_event_ho_in_prep_failure(rrm_ue_context_t    *p_rrm_ue_context,
                                        U8                   error_cause);

void rrm_raise_event_ho_out_prep_attempt(rrm_ue_context_t      *p_rrm_ue_context,
                                         rrc_rrm_ho_required_t *p_rrc_rrm_ho_required);

void rrm_raise_event_ho_out_prep_failed(rrm_ue_context_t *p_rrm_ue_context);
void rrm_raise_event_ho_out_prep_info(rrm_ue_context_t *p_rrm_ue_context,
                                      rrm_ue_proc_ho_command_request_t *p_ho_command_request);
void rrm_raise_event_ho_out_exec_attempt(void);
     /*SPR 17777 +-*/

void rrm_raise_event_ho_out_exec_failed(rrm_ue_context_t *p_rrm_ue_context);
     /*SPR 17777 +-*/
void rrm_raise_event_ho_out_exec_complete(void);
void rrm_raise_event_ho_in_exec_complete(void);
     /*SPR 17777 +-*/
void rrm_raise_event_ho_in_prep_complete(rrm_ue_context_t *p_ue_cntx);
void rrm_raise_event_ho_in_exec_attempt(rrm_ue_context_t *p_ue_cntxt);
void rrm_raise_event_ho_in_exec_failed(rrm_ue_context_t *p_ue_ctxt);
void rrm_raise_event_meas_config_b2_geran(
     /*SPR 17777 +-*/
                                          report_config_to_add_mod_t    *p_report_config_to_add_mod,
                                          rrm_trigger_quantity_et        eutran_trigger_quantity,
                                          U8                             eutra_thresh_rsrx);

void rrm_raise_event_meas_config_b2_utra(
     /*SPR 17777 +-*/
                                         report_config_to_add_mod_t *p_report_config_to_add_mod,
                                         rrm_trigger_quantity_et     eutran_trigger_quantity,
                                         U8                          eutra_thresh_rsrx);

void rrm_raise_event_ue_meas_abort(
     /*SPR 17777 +-*/
                                   U8                 meas_id);

void rrm_raise_event_too_early_ho(rrm_ue_context_t  *p_ue_context);

void rrm_raise_event_too_late_ho(rrm_ue_context_t  *p_ue_context);

void rrm_raise_event_ho_wrong_cell(rrm_ue_context_t  *p_ue_context);

void rrm_raise_event_ho_wrong_cell_reest(rrm_ue_context_t  *p_ue_context);

void rrm_raise_event_x2_rlf_indication(void);

rrm_rat_type_et rrm_fill_ho_target_rat_info(rrm_ue_context_t  *p_ue_context);

void rrm_raise_event_meas_config_periodical_geran(rrc_rrm_meas_config_req_t *p_rrc_rrm_meas_config_req,
                                                  rrm_cell_index_t           cell_index);

void rrm_raise_event_meas_config_periodical_utra(rrc_rrm_meas_config_req_t  *p_rrc_rrm_meas_config_req,
                                                 rrm_cell_index_t            cell_index);

void rrm_raise_event_meas_config_periodical_eutra(rrc_rrm_meas_config_req_t  *p_rrc_rrm_meas_config_req,
                                                  rrm_cell_index_t               cell_index);
void
rrm_raise_event_ho_out_exec_info(const rrm_ue_context_t    *p_rrm_ue_context);

void
rrm_raise_event_ho_ue_detected(void);/*SPR 17777 +-*/

/****** protocol event raise function fucntion prototypes END ********/

/*****  Protocol event composer function prototypes START *****/
rrm_return_et
rrm_compose_local_event_erab_setup_attempt
(
    U8  *pp_buffer,
    rrm_event_local_erab_setup_attempt_t *p_rrm_event_local_erab_setup_attempt
);

rrm_return_et
rrm_compose_local_event_ue_cap_fgi_reject
(
    U8  *pp_buffer,
    rrm_event_local_ue_capability_fgi_reject_t *p_rrm_event_local_ue_capability_fgi_reject
);

rrm_return_et
rrm_compose_local_event_erab_adm_rej
(
    U8  *pp_buffer,
    rrm_event_local_erab_admission_reject_t *p_rrm_event_local_erab_admission_reject
);

rrm_return_et
rrm_compose_local_event_erab_modify_attempt
(
    U8  *pp_buffer,
    rrm_event_local_erab_modify_attempt_t *p_rrm_event_local_erab_modify_attempt
);

rrm_return_et
rrm_compose_local_event_erab_setup_ready
(
    U8  *pp_buffer,
    rrm_event_local_erab_setup_ready_t *p_rrm_event_local_erab_setup_ready
);

rrm_return_et
rrm_compose_local_event_erab_modify_ready
(
    U8  *pp_buffer,
    rrm_event_erab_modify_ready_t *p_rrm_event_erab_modify_ready
);

rrm_return_et
rrm_compose_local_event_erab_drb_release_attempt
(
    U8  *pp_buffer,
    rrm_event_local_erab_release_attempt_t *p_rrm_event_local_erab_release_attempt
);

rrm_return_et
rrm_compose_local_event_erab_release_ready
(
    U8  *pp_buffer,
    rrm_event_local_erab_release_ready_t *p_rrm_event_local_erab_release_ready
);

rrm_return_et
rrm_compose_local_event_erab_drb_release
(
    U8  *pp_buffer,
    rrm_event_local_erab_drb_release_t *p_rrm_event_local_erab_drb_release
);

rrm_return_et
rrm_compose_local_event_ue_capability
(
    U8  *pp_buffer,
    rrm_event_local_event_ue_capability_t *p_rrm_event_local_event_ue_capability
);

rrm_return_et
rrm_compose_local_event_cell_setup_reject
(
    U8  *pp_buffer,
    rrm_event_local_cell_setup_reject_t *p_rrm_event_local_cell_setup_reject
);

rrm_return_et
rrm_compose_local_event_erab_drb_allocated
(
    U8  *pp_buffer,
    rrm_event_erab_drb_allocated_t *p_rrm_event_erab_drb_allocated
);

rrm_return_et
rrm_compose_local_event_srb1_setup_reject
(
    U8  *pp_buffer,
    rrm_event_srb1_setup_reject_t *p_rrm_event_srb1_setup_reject
);

rrm_return_et
rrm_compose_local_event_erab_setup_qci
(
    U8  *pp_buffer,
    rrm_event_erab_setup_qci_t *p_rrm_event_erab_setup_qci
);


rrm_return_et
rrm_compose_local_event_meas_config_a1
(
    U8  *p_buffer,
    rrm_event_meas_config_a1_t *p_rrm_event_meas_config_a1
);

rrm_return_et
rrm_compose_local_event_meas_config_a2
(
    U8  *p_buffer,
    rrm_event_meas_config_a2_t *p_rrm_event_meas_config_a2
);

rrm_return_et
rrm_compose_local_event_meas_config_a3
(
    U8  *p_buffer,
    rrm_event_meas_config_a3_t *p_rrm_event_meas_config_a3
);

rrm_return_et
rrm_compose_local_event_meas_config_a4
(
    U8  *p_buffer,
    rrm_event_meas_config_a4_t *p_rrm_event_meas_config_a4
);

rrm_return_et
rrm_compose_local_event_meas_config_a5
(
    U8  *p_buffer,
    rrm_event_meas_config_a5_t *p_rrm_event_meas_config_a5
);


rrm_return_et
rrm_compose_local_event_ho_out_prep_attempt
(
    U8  *p_buffer,
    rrm_event_ho_out_prep_attempt_t *p_rrm_event_ho_out_prep_attempt
);

rrm_return_et
rrm_compose_local_event_ho_out_prep_failed
(
    U8  *p_buffer,
    rrm_event_ho_out_prep_failed_t *p_rrm_event_ho_out_prep_failed
);

rrm_return_et
rrm_compose_local_event_ho_out_exec_failed
(
    U8  *p_buffer,
    rrm_event_ho_out_exec_failed_t *p_rrm_event_ho_out_exec_failed
);

rrm_return_et
rrm_compose_local_event_ho_out_prep_info
(
    U8  *p_buffer,
    rrm_event_ho_out_prep_info_t *p_rrm_event_ho_out_prep_info
);

rrm_return_et
rrm_compose_local_event_erab_release_info
(
  U8  *p_buffer,
  rrm_event_local_erab_release_info_t  *p_rrm_event_local_erab_release_info
);

rrm_return_et
rrm_compose_local_event_erab_setup_info
(
  U8  *p_buffer,
  rrm_event_local_erab_setup_info_t  *p_rrm_event_local_erab_setup_info
);

rrm_return_et
rrm_compose_local_event_ho_in_prep_attempt
(
  U8  *p_buffer,
  rrm_event_local_ho_in_prep_attempt_t   *p_rrm_event_local_ho_in_prep_attempt
);


rrm_return_et
rrm_compose_local_event_ho_in_prep_failure
(
    U8  *p_buffer,
    rrm_event_local_ho_in_prep_failure_t  *p_rrm_event_local_ho_in_prep_failure
);


rrm_return_et
rrm_compose_local_event_ho_out_exec_info
(
   U8  *p_buffer,
   rrm_event_ho_out_exec_info_t  *p_rrm_event_ho_out_exec_info
);

rrm_return_et
rrm_compose_local_event_meas_config_b2_geran
(
    U8  *p_buffer,
    rrm_event_meas_config_b2_geran_t  *p_rrm_event_meas_config_b2_geran
);

rrm_return_et
rrm_compose_local_event_meas_config_b2_utra
(
    U8  *p_buffer,
    rrm_event_meas_config_b2_utra_t  *p_rrm_event_meas_config_b2_utra
);

rrm_return_et
rrm_compose_local_event_ue_meas_abort
(
    U8  *p_buffer,
    rrm_event_ue_meas_abort_t  *p_rrm_event_ue_meas_abort
);

rrm_return_et
rrm_compose_local_event_too_early_ho
(
    U8  *p_buffer,
    rrm_event_too_early_ho_t  *p_rrm_event_too_early_ho
);

rrm_return_et
rrm_compose_local_event_too_late_ho
(
    U8  *p_buffer,
    rrm_event_too_late_ho_t  *p_rrm_event_too_late_ho
);

rrm_return_et
rrm_compose_local_event_ho_wrong_cell
(
    U8  *p_buffer,
    rrm_event_ho_wrong_cell_t  *p_rrm_event_ho_wrong_cell
);

rrm_return_et
rrm_compose_local_event_ho_wrong_cell_reest
(
    U8  *p_buffer,
    rrm_event_ho_wrong_cell_reest_t  *p_rrm_event_ho_wrong_cell_reest
);

rrm_return_et
rrm_compose_local_event_meas_config_periodical_utra
(
    U8  *p_buffer,
    rrm_event_meas_config_periodical_utra_t  *p_rrm_event_meas_config_periodical_utra
);

rrm_return_et
rrm_compose_local_event_meas_config_periodical_geran
(
    U8  *p_buffer,
    rrm_event_meas_config_periodical_geran_t  *p_rrm_event_meas_config_periodical_geran
);

rrm_return_et
rrm_compose_local_event_meas_config_periodical_eutran
(
    U8  *p_buffer,
    rrm_event_meas_config_periodical_eutran_t  *p_rrm_event_meas_config_periodical_eutran
);

rrm_return_et
rrm_compose_error_event_info(U8                      *p_buffer,
                             rrm_event_error_info_t  *p_err_info);


/*****  Protocol event composer function prototypes ENDs *****/
#endif /*_RRM_EVENTS_H_ */
