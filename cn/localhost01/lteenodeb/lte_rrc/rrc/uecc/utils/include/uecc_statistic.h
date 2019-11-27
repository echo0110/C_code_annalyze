/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: uecc_statistic.h,v 1.2 2010/01/04 16:10:05 ukr15916 Exp $
 ****************************************************************************
 *
 *  File Description : UECC statistic
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: uecc_statistic.h,v $
 * Revision 1.2  2010/01/04 16:10:05  ukr15916
 * no message
 *
 * Revision 1.1.6.4  2009/12/26 12:09:52  gur21006
 * Warnings removed
 *
 * Revision 1.1.6.3  2009/12/15 11:54:43  ukr15916
 * srb0 is added during status (srb_num+1)
 *
 * Revision 1.1.6.2  2009/12/08 11:22:51  ukr16032
 * UECC - inspection log items LG_20, LG_23, LG_24, LG_26, LG_27, LG_28, LG_29,
 *  LG_34, LG_40, LG_41 fixed
 *
 * Revision 1.1.6.1  2009/11/26 18:33:49  ukr16018
 * Merge S1AP and RRC (from branch dev_rel_1_0).
 *
 * Revision 1.1.4.10  2009/11/24 11:32:30  ukr15916
 * updated for dev_rel_1_0
 *
 * Revision 1.1.4.9.2.2  2009/11/24 08:29:05  ukr15916
 * Reviewed
 *
 *
 ****************************************************************************/

#ifndef UECC_STATISTIC_H
#define UECC_STATISTIC_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "rrc_defines.h"
#include "rrc_asn_enc_dec.h"
#include "rrc_oam_intf.h"
#include "uecc_ue_timer_utils.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

#define UECC_STATUS_INITIAL_SRB_NUM 1
#define UECC_STATUS_INITIAL_DRB_NUM 0

/****************************************************************************
 * Data types definitions
 ****************************************************************************/

struct _uecc_gb_context_t;
struct _uecc_ue_context_t;
struct _rrc_csc_context_t;
#define RRC_UPDATE_PERFORMANCE_COUNTER(p_uecc_gb_context,COUNTER_NAME, INDEX, HIGHER_RANGE_CHECK) \
    if (INDEX > HIGHER_RANGE_CHECK) \
{ RRC_UECC_TRACE( p_uecc_gb_context->context_index, \
        p_uecc_gb_context->facility_name, RRC_WARNING, \
        "%s : Wrong Range %d", __FUNCTION__, INDEX); } \
else \
{ COUNTER_NAME++; \
 RRC_UECC_TRACE(p_uecc_gb_context->context_index, \
         p_uecc_gb_context->facility_name, RRC_DETAILED, \
         "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME); }
#define RRC_UPDATE_PERFORMANCE_COUNTER_S1AP_CAUSE(p_ue_context,COUNTER_NAME, S1AP_CAUSE) \
   switch (S1AP_CAUSE.t) \
   { case T_s1ap_Cause_radioNetwork: \
       if (S1AP_CAUSE.u.radioNetwork > (RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX - 1)) \
       { RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
               "%s : Wrong Range %d", __FUNCTION__, S1AP_CAUSE.u.radioNetwork); }\
       else { COUNTER_NAME##_radioNetwork[S1AP_CAUSE.u.radioNetwork]++; \
       RRC_UECC_TRACE( p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
               "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME##_radioNetwork[S1AP_CAUSE.u.radioNetwork]);}\
       break; \
     case T_s1ap_Cause_transport: \
       if (S1AP_CAUSE.u.transport > (RRC_S1AP_CAUSE_TRANSPORT_INDEX - 1)) \
       { RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
               "%s : Wrong Range %d", __FUNCTION__, S1AP_CAUSE.u.transport); }\
       else { COUNTER_NAME##_transport[S1AP_CAUSE.u.transport]++; \
       RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
               "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME##_transport[S1AP_CAUSE.u.transport]);}\
       break; \
     case T_s1ap_Cause_nas: \
       if (S1AP_CAUSE.u.nas > (RRC_S1AP_CAUSE_NAS_INDEX - 1)) \
       { RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
               "%s : Wrong Range %d", __FUNCTION__, S1AP_CAUSE.u.nas); }\
       else { COUNTER_NAME##_nas[S1AP_CAUSE.u.nas]++; \
       RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
               "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME##_nas[S1AP_CAUSE.u.nas]);}\
       break; \
     case T_s1ap_Cause_protocol:\
       if (S1AP_CAUSE.u.protocol > (RRC_S1AP_CAUSE_PROTOCOL_INDEX - 1)) \
       { RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
               "%s : Wrong Range %d", __FUNCTION__, S1AP_CAUSE.u.protocol); }\
       else { COUNTER_NAME##_protocol[S1AP_CAUSE.u.protocol]++; \
       RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
               "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME##_protocol[S1AP_CAUSE.u.protocol]);} \
       break; \
     case T_s1ap_Cause_misc:\
       if (S1AP_CAUSE.u.misc > (RRC_S1AP_CAUSE_MISC_INDEX - 1)) \
       { RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
               "%s : Wrong Range %d", __FUNCTION__, S1AP_CAUSE.u.misc); }\
       else { COUNTER_NAME##_misc[S1AP_CAUSE.u.misc]++; \
       RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
               "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME##_misc[S1AP_CAUSE.u.misc]);} \
       break; \
     default: \
         RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
                 p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
                 "%s : Wrong Range %d", __FUNCTION__, S1AP_CAUSE.t); }

#define RRC_UPDATE_PERFORMANCE_COUNTER_X2AP_CAUSE(p_ue_context, COUNTER_NAME, X2AP_CAUSE) \
   switch (X2AP_CAUSE.t) \
   { case T_x2ap_Cause_radioNetwork: \
       if (X2AP_CAUSE.u.radioNetwork > (RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX - 1)) \
       { RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
               "%s : Wrong Range %d", __FUNCTION__, X2AP_CAUSE.u.radioNetwork); }\
       else { COUNTER_NAME##_radioNetwork[X2AP_CAUSE.u.radioNetwork]++; \
       RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
               "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME##_radioNetwork[X2AP_CAUSE.u.radioNetwork]);}\
       break; \
     case T_x2ap_Cause_transport: \
       if (X2AP_CAUSE.u.transport > (RRC_S1AP_CAUSE_TRANSPORT_INDEX - 1)) \
       { RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
               "%s : Wrong Range %d", __FUNCTION__, X2AP_CAUSE.u.transport); }\
       else { COUNTER_NAME##_transport[X2AP_CAUSE.u.transport]++; \
       RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
               "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME##_transport[X2AP_CAUSE.u.transport]);}\
       break; \
     case T_x2ap_Cause_protocol:\
       if (X2AP_CAUSE.u.protocol > (RRC_S1AP_CAUSE_PROTOCOL_INDEX - 1)) \
       { RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
               "%s : Wrong Range %d", __FUNCTION__, X2AP_CAUSE.u.protocol); }\
       else { COUNTER_NAME##_protocol[X2AP_CAUSE.u.protocol]++; \
       RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
               "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME##_protocol[X2AP_CAUSE.u.protocol]);} \
       break; \
     case T_x2ap_Cause_misc:\
       if (X2AP_CAUSE.u.misc > (RRC_S1AP_CAUSE_MISC_INDEX - 1)) \
       { RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
               "%s : Wrong Range %d", __FUNCTION__, X2AP_CAUSE.u.misc); }\
       else { COUNTER_NAME##_misc[X2AP_CAUSE.u.misc]++; \
       RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
               p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
               "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME##_misc[X2AP_CAUSE.u.misc]);} \
       break; \
     default: \
         RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
                 p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
                 "%s : Wrong Range %d", __FUNCTION__, X2AP_CAUSE.t); }


#define RRC_CALC_PERFORMANCE_COUNTER_SUM_S1AP_CAUSE(COUNTER_NAME, INDEX) \
    for (INDEX = 0; INDEX < RRC_S1AP_CAUSE_RADIO_NETWORK_INDEX; INDEX++)\
{COUNTER_NAME##_sum += COUNTER_NAME##_cause_radioNetwork[INDEX];} \
for (INDEX = 0; INDEX < RRC_S1AP_CAUSE_TRANSPORT_INDEX;INDEX++) \
{COUNTER_NAME##_sum += COUNTER_NAME##_cause_transport[INDEX];} \
for (INDEX = 0; INDEX < RRC_S1AP_CAUSE_NAS_INDEX;INDEX++) \
{COUNTER_NAME##_sum += COUNTER_NAME##_cause_nas[INDEX];} \
for (INDEX = 0; INDEX < RRC_S1AP_CAUSE_PROTOCOL_INDEX;INDEX++) \
{COUNTER_NAME##_sum += COUNTER_NAME##_cause_protocol[INDEX];} \
for (INDEX = 0; INDEX < RRC_S1AP_CAUSE_MISC_INDEX;INDEX++) \
{COUNTER_NAME##_sum += COUNTER_NAME##_cause_misc[INDEX];}

#define RRC_GB_START_TIME_PERFORMANCE_COUNTER(p_uecc_gb_context,COUNTER_NMAE, COUNTER_TYPE)\
    if (RRC_GB_MAX_TIMER_STAT_TYPE <= COUNTER_TYPE)\
    {RRC_UECC_TRACE(p_uecc_gb_context->context_index, \
            p_uecc_gb_context->facility_name, RRC_WARNING, \
            "%s : Wrong Range %d", __FUNCTION__, COUNTER_TYPE);} \
else {gettimeofday_wrapper(&COUNTER_NMAE.start_time, PNULL); \
RRC_UECC_TRACE(p_uecc_gb_context->context_index, \
        p_uecc_gb_context->facility_name, RRC_DETAILED, \
        "%s: Counter Start Time: %s = %d",__FUNCTION__, #COUNTER_NMAE, COUNTER_NMAE.start_time);}

#define RRC_GB_STOP_TIME_PERFORMANCE_COUNTER(p_uecc_gb_context,COUNTER_NAME, COUNTER_TYPE)\
    if (RRC_GB_MAX_TIMER_STAT_TYPE <= COUNTER_TYPE)\
    {RRC_UECC_TRACE(p_uecc_gb_context->context_index, \
            p_uecc_gb_context->facility_name, RRC_WARNING, \
            "%s : Wrong Range %d", __FUNCTION__, COUNTER_TYPE);} \
    else {gettimeofday_wrapper(&COUNTER_NAME.end_time,PNULL); \
    RRC_UECC_TRACE(p_uecc_gb_context->context_index, \
            p_uecc_gb_context->facility_name, RRC_DETAILED, \
            "%s: Counter Stop Time: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME.end_time);}

#define RRC_UE_START_TIME_PERFORMANCE_COUNTER(p_ue_context,COUNTER_NAME, COUNTER_TYPE)\
    if (RRC_UE_MAX_TIMER_STAT_TYPE <= COUNTER_TYPE)\
    {RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
            p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
            "%s : Wrong Range %d", __FUNCTION__, COUNTER_TYPE);} \
else {gettimeofday_wrapper(&COUNTER_NAME.start_time, PNULL); \
RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
        p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
        "%s: Counter Start Time: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME.start_time);}

#define RRC_UE_STOP_TIME_PERFORMANCE_COUNTER(p_ue_context,COUNTER_NAME, COUNTER_TYPE)\
    if (RRC_UE_MAX_TIMER_STAT_TYPE <= COUNTER_TYPE)\
    {RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
            p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
            "%s : Wrong Range %d", __FUNCTION__, COUNTER_TYPE);} \
    else {gettimeofday_wrapper(&COUNTER_NAME.end_time,PNULL);\
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
            p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
            "%s: Counter Stop Time: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME.end_time);}

#define RRC_CALCULATE_TIMEDIFF_AND_UPDATE_COUNTER(p_uecc_gb_context,TIMER_VAL, COUNTER_NAME, INDEX, HIGHER_RANGE_CHECK)\
    if (INDEX > HIGHER_RANGE_CHECK) \
{RRC_UECC_TRACE(p_uecc_gb_context->context_index, \
        p_uecc_gb_context->facility_name, RRC_WARNING, \
        "%s : Wrong Range %d", __FUNCTION__, INDEX); } \
else \
{COUNTER_NAME+= (U32)(((TIMER_VAL.end_time.tv_usec - TIMER_VAL.start_time.tv_usec)/1000) +\
    (1000 * (TIMER_VAL.end_time.tv_sec - TIMER_VAL.start_time.tv_sec))); \
RRC_UECC_TRACE(p_uecc_gb_context->context_index, \
        p_uecc_gb_context->facility_name, RRC_DETAILED, \
        "%s: Counter DIFF: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME);}

#define RRC_UPDATE_MAX_TIME_COUNTER(p_ue_context,TIMER_VAL, COUNTER_NAME, INDEX, HIGHER_RANGE_CHECK)\
    if (INDEX > HIGHER_RANGE_CHECK) \
{RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
        p_ue_context->p_gb_context->facility_name, RRC_WARNING, \
        "%s : Wrong Range %d", __FUNCTION__, INDEX); } \
else \
{if(COUNTER_NAME < (U32)(TIMER_VAL.end_time.tv_usec - TIMER_VAL.start_time.tv_usec +\
       1000000 * (TIMER_VAL.end_time.tv_sec - TIMER_VAL.start_time.tv_sec))) \
    {COUNTER_NAME = (U32)(TIMER_VAL.end_time.tv_usec - TIMER_VAL.start_time.tv_usec +\
        1000000 * (TIMER_VAL.end_time.tv_sec - TIMER_VAL.start_time.tv_sec));\
    RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, \
            p_ue_context->p_gb_context->facility_name, RRC_DETAILED, \
            "%s: Counter MAX: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME);}}
/* BUG_11901 start */
#define RRC_UPDATE_PERFORMANCE_COUNTER_WITH_DECREMENT(p_uecc_gb_context, COUNTER_NAME, INDEX, HIGHER_RANGE_CHECK) \
    if (INDEX > HIGHER_RANGE_CHECK) \
{ RRC_UECC_TRACE(p_uecc_gb_context->context_index, \
        p_uecc_gb_context->facility_name, RRC_WARNING, \
        "%s : Wrong Range %d", __FUNCTION__, INDEX); } \
else \
{ if (0 < COUNTER_NAME) \
 {COUNTER_NAME--; \
 RRC_UECC_TRACE(p_uecc_gb_context->context_index, \
         p_uecc_gb_context->facility_name, RRC_DETAILED, \
         "%s: Counter: %s = %d",__FUNCTION__, #COUNTER_NAME, COUNTER_NAME);} }
/* BUG_11901 stop */
/* For all QCI Values greater than 15, update the performance index array at 0th
 * location */
#define RRC_UPDATED_QCI_INDEX(qci_value) ((qci_value <= RRC_MAX_QCI_NON_GBR_LIMIT)?qci_value:0)
    


typedef enum 
{
    RRC_GB_AVG_NUM_SIM_ERABS,
    RRC_GB_MAX_TIMER_STAT_TYPE
} rrc_timer_gb_stats_type_et;

typedef enum
{
    RRC_UE_RRC_CONN_SETUP_TIME,
    RRC_UE_ERAB_SETUP_TIME,
    RRC_UE_MAX_TIMER_STAT_TYPE

} rrc_timer_ue_stats_type_et;

typedef struct _rrc_timer_stats_t
{
    struct timeval start_time;
    struct timeval end_time;
} rrc_timer_stats_t;

typedef struct
{
    /* Number of UEs per cell */
    U32 rrc_num_of_ue_per_cell;

    /* Number of SRB1 per cell */
    U32 rrc_num_of_srb1_per_cell;


    /* Number of SRB2 per cell */
    U32 rrc_num_of_srb2_per_cell;

    /* Number of DRB per cell */
    U32 rrc_num_of_drb_per_cell;
} rrc_cell_status_t;

/* Data belonged to UE context */
typedef struct
{
    /* Number of SRB1 per UE */
    U8 ue_num_of_srb1;

    /* Number of SRB2 per UE */
    U8 ue_num_of_srb2;

    /* Number of DRB per UE */
    U8 ue_num_of_drb;

    /* RRC_IDLE=0, RRC_CONNECTED=1*/
    U8 ue_state;

} rrc_ue_status_t;



/* Global statistic for UECC (singleton) */
typedef struct
{
    rrc_timer_stats_t     rrc_timer_statistic[RRC_GB_MAX_TIMER_STAT_TYPE];
    rrc_cell_status_t     rrc_cell_status;
} rrc_uecc_statistic_t;

typedef struct
{
    rrc_timer_stats_t ue_timer_stats[RRC_UE_MAX_TIMER_STAT_TYPE];
} rrc_uecc_ue_statistic_t;

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_status_cell_engine_init
*
*   DESCRIPTION:
*       Inits statistic engine.
*
*   RETURNS: RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_status_cell_engine_init(
    struct _rrc_csc_context_t*  p_csc_context,
    struct _uecc_gb_context_t*  p_uecc_gb_context
);


/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_init
*
*   DESCRIPTION:
*       Init all statistic data in specified ue
*
*   RETURNS: None
*
******************************************************************************/
void uecc_status_ue_init(
    struct _uecc_ue_context_t*  p_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_release
*
*   DESCRIPTION:
*       Releases statistic for UE
*
*   RETURNS: None
*
******************************************************************************/
void uecc_status_ue_release(
    struct _uecc_ue_context_t*  p_ue_context
);

/******************************************************************************
*   FUNCTION NAME: uecc_statistic_cell_update_num_of_conn_req
*
*   DESCRIPTION:
*       Updates Number of RRC Connection Requested with category of
*       establishment cause
*
*   RETURNS: None
*
******************************************************************************/
void uecc_statistic_cell_update_num_of_conn_req(
    rrc_uecc_statistic_t*       p_rrc_uecc_statistic,
    EstablishmentCause          est_cause
);

/******************************************************************************
*   FUNCTION NAME: uecc_statistic_cell_update_num_of_conn_rej
*
*   DESCRIPTION:
*       Updates Number of RRC Connection Rejected
*
*   RETURNS: None
*
******************************************************************************/
void uecc_statistic_cell_update_num_of_conn_rej(
    rrc_uecc_statistic_t*   p_rrc_uecc_statistic
);

/******************************************************************************
*   FUNCTION NAME: uecc_statistic_cell_update_num_of_conn_establ
*
*   DESCRIPTION:
*       Updates Number of RRC Connection Successfully established
*
*   RETURNS: None
*
******************************************************************************/
void uecc_statistic_cell_update_num_of_conn_establ(
    rrc_uecc_statistic_t*   p_rrc_uecc_statistic
);

/******************************************************************************
*   FUNCTION NAME: uecc_statistic_cell_update_num_of_conn_rel
*
*   DESCRIPTION:
*       Updates Number of RRC Connection Released with category of
*       release cause
*
*   RETURNS: None
*
******************************************************************************/
void uecc_statistic_cell_update_num_of_conn_rel(
    rrc_uecc_statistic_t*   p_rrc_uecc_statistic,
    ReleaseCause            cause
);

/******************************************************************************
*   FUNCTION NAME: uecc_statistic_cell_update_num_of_conn_reconf_msg
*
*   DESCRIPTION:
*       Updates Number of RRC Connection Reconfiguration messages
*
*   RETURNS: None
*
******************************************************************************/
void uecc_statistic_cell_update_num_of_conn_reconf_msg(
    rrc_uecc_statistic_t*   p_rrc_uecc_statistic
);

/******************************************************************************
*   FUNCTION NAME: uecc_status_cell_update_num_of_ue
*
*   DESCRIPTION:
*       Updates Number of UE for CELL
*
*   RETURNS: None
*
******************************************************************************/
void uecc_status_cell_update_num_of_ue(
   struct _uecc_gb_context_t*  p_uecc_gb_context,  
    rrc_uecc_statistic_t*   p_rrc_uecc_statistic,
    rrc_bool_et             is_incr
);

/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_update_num_of_srb1
*
*   DESCRIPTION:
*       Updates Number of SRB1 for specified UE
*
*   RETURNS: None
*
******************************************************************************/
void uecc_status_ue_update_num_of_srb1(
    struct _uecc_ue_context_t*  p_ue_context
);


/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_update_num_of_srb2
*
*   DESCRIPTION:
*       Updates Number of SRB2 for specified UE
*
*   RETURNS: None
*
******************************************************************************/
void uecc_status_ue_update_num_of_srb2(
    struct _uecc_ue_context_t*  p_ue_context
);


/******************************************************************************
*   FUNCTION NAME: uecc_status_ue_update_num_of_drb
*
*   DESCRIPTION:
*       Updates Number of DRB for specified UE
*
*   RETURNS: None
*
******************************************************************************/
void uecc_status_ue_update_num_of_drb(
    struct _uecc_ue_context_t*  p_ue_context,
    S8                          count_of_drb
);

#endif /* UECC_STATISTIC_H */

