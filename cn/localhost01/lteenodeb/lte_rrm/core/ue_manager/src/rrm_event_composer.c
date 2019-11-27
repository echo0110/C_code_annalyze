/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_event_composer.c $
 *
 *******************************************************************************
 *
 * File Description: This file have the information about processing
 *                   event messages.
 *
 *
 *******************************************************************************
 *Revision Details
 *----------------------
 *   DATE             AUTHOR      REFERENCE       REASON
 *   10 Jan, 2012     gur19685     Initial
 ******************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrm_ue_context.h"
#include "rrm_events.h"
#include "rrm_api_types.h"
#include "rrm_event_defines.h"
#include "rrm_defines.h"

/****************************************************************************
  Function Prototypes 
 ****************************************************************************/
rrm_return_et rrm_cp_pack_U16 (
        void    *p_dest,
        void    *p_src,
        char    *varname);
rrm_return_et rrm_cp_pack_U8(
        void    *p_dest,
        void    *p_src,
        char    *varname);
rrm_return_et rrm_cp_pack_U32(
        void    *p_dest,
        void    *p_src,
        char    *varname);

rrm_return_et rrm_cp_pack_U64(
        void    *p_dest,
        void    *p_src,
        char    *varname);

rrm_return_et rrm_cp_pack_S8(
        void    *p_dest,
        void    *p_src,
        char    *varname);

rrm_return_et rrm_cp_pack_S32(
        void    *p_dest,
        void    *p_src,
        S8    *varname);

/****************************************************************************
  Private Types
 ****************************************************************************/
/****************************************************************************
 * Exported Variables
 ****************************************************************************/


/******************  Event Composer Functions ****************/
rrm_return_et
rrm_compose_local_event_erab_setup_attempt
(
 U8  *p_buffer,
 rrm_event_local_erab_setup_attempt_t *p_rrm_event_local_erab_setup_attempt
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_erab_setup_attempt != PNULL);
    U8   index = RRM_NULL;

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_setup_attempt->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_erab_setup_attempt->cell_id);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        /* Compose parameter of basic type */
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_setup_attempt->requested_qci[index], "requested_qci[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_setup_attempt->requested_qci[index]);
    }

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    { 
        /* Compose parameter of basic type */
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_setup_attempt->priority_level[index], "priority_level[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_setup_attempt->priority_level[index]);
    }

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(p_buffer, &p_rrm_event_local_erab_setup_attempt->ul_admitted_gbr, "ul_admitted_gbr");
    p_buffer += sizeof(p_rrm_event_local_erab_setup_attempt->ul_admitted_gbr);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(p_buffer, &p_rrm_event_local_erab_setup_attempt->dl_admitted_gbr, "dl_admitted_gbr");
    p_buffer += sizeof(p_rrm_event_local_erab_setup_attempt->dl_admitted_gbr);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        /* Compose parameter of basic type */
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_setup_attempt->preemption_capability[index], "preemption_capability[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_setup_attempt->preemption_capability[index]);
    }

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        /* Compose parameter of basic type */
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_setup_attempt->preemption_vulnerability[index], "preemption_vulnerability[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_setup_attempt->preemption_vulnerability[index]);
    }
    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_ue_cap_fgi_reject
(
 U8  *p_buffer,
 rrm_event_local_ue_capability_fgi_reject_t *p_rrm_event_local_ue_capability_fgi_reject
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_ue_capability_fgi_reject != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_ue_capability_fgi_reject->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_ue_capability_fgi_reject->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_local_ue_capability_fgi_reject->fgi_bits, "fgi_bits");
    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_erab_adm_rej
(
 U8  *p_buffer,
 rrm_event_local_erab_admission_reject_t *p_rrm_event_local_erab_admission_reject
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_erab_admission_reject != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_admission_reject->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_erab_admission_reject->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_admission_reject->erab_id, "erab_id");
    p_buffer += sizeof(p_rrm_event_local_erab_admission_reject->erab_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_admission_reject->erab_setup_result, "erab_setup_result");
    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_erab_modify_attempt
(
 U8  *p_buffer,
 rrm_event_local_erab_modify_attempt_t *p_rrm_event_local_erab_modify_attempt
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_erab_modify_attempt != PNULL);
    U8   index = RRM_NULL;   

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_modify_attempt->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_erab_modify_attempt->cell_id);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    { 
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_modify_attempt->rqstd_qci[index], "rqstd_qci[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_modify_attempt->rqstd_qci[index]);
    }

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_erab_setup_ready
(
 U8  *p_buffer,
 rrm_event_local_erab_setup_ready_t *p_rrm_event_local_erab_setup_ready
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_erab_setup_ready != PNULL);
    U8   index = RRM_NULL;

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_setup_ready->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_erab_setup_ready->cell_id);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_setup_ready->erab_setup_result[index], "erab_setup_result[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_setup_ready->erab_setup_result[index]);
    }
    /* Compose parameter of basic type */
    rrm_cp_pack_U64(p_buffer, &p_rrm_event_local_erab_setup_ready->ul_admitted_gbr, "ul_admitted_gbr");
    p_buffer += sizeof(p_rrm_event_local_erab_setup_ready->ul_admitted_gbr);

    /* Compose parameter of basic type */
    rrm_cp_pack_U64(p_buffer, &p_rrm_event_local_erab_setup_ready->dl_admitted_gbr, "dl_admitted_gbr");

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_erab_modify_ready
(
 U8  *p_buffer,
 rrm_event_erab_modify_ready_t *p_rrm_event_erab_modify_ready
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_erab_modify_ready != PNULL);
    U8   index = RRM_NULL;

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_erab_modify_ready->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_erab_modify_ready->cell_id);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_erab_modify_ready->modify_result[index], "modify_result[index]");
        p_buffer += sizeof(p_rrm_event_erab_modify_ready->modify_result[index]);
    }

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_erab_drb_release_attempt
(
 U8  *p_buffer,
 rrm_event_local_erab_release_attempt_t *p_rrm_event_local_erab_release_attempt
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_erab_release_attempt != PNULL);
    U8   index = RRM_NULL;

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_attempt->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_erab_release_attempt->cell_id);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_attempt->erab_id_to_release[index], "erab_id_to_release[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_release_attempt->erab_id_to_release[index]);
    }

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_attempt->cause_group[index], "cause_group[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_release_attempt->cause_group[index]);
    }

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_attempt->cause_value[index], "cause_value[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_release_attempt->cause_value[index]);
    }

    return RRM_SUCCESS;
}



rrm_return_et
rrm_compose_local_event_erab_release_ready
(
 U8  *p_buffer,
 rrm_event_local_erab_release_ready_t *p_rrm_event_local_erab_release_ready
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_erab_release_ready != PNULL);
    U8   index = RRM_NULL;

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_ready->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_erab_release_ready->cell_id);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_ready->succ_erab_id[index], "succ_erab_id[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_release_ready->succ_erab_id[index]);
    }

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_erab_drb_release
(
 U8  *p_buffer,
 rrm_event_local_erab_drb_release_t *p_rrm_event_local_erab_drb_release
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_erab_drb_release != PNULL);
    U8   index = RRM_NULL;

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_drb_release->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_erab_drb_release->cell_id);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_drb_release->erab_id_released[index], "erab_id_released[index]");
        p_buffer += sizeof(p_rrm_event_local_erab_drb_release->erab_id_released[index]);
    }

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_ue_capability
(
 U8  *p_buffer,
 rrm_event_local_event_ue_capability_t *p_rrm_event_local_event_ue_capability
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_event_ue_capability != PNULL);
    U8   index = RRM_NULL;  

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_event_ue_capability->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_event_ue_capability->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_event_ue_capability->access_stratum_release, "access_stratum_release");
    p_buffer += sizeof(p_rrm_event_local_event_ue_capability->access_stratum_release);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_event_ue_capability->ue_category, "ue_category");
    p_buffer += sizeof(p_rrm_event_local_event_ue_capability->ue_category);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index < RRM_FOUR; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_event_ue_capability->fgi_bitmap[index], "fgi_bitmap[index]");
        p_buffer += sizeof(p_rrm_event_local_event_ue_capability->fgi_bitmap[index]);
    }

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_cell_setup_reject
(
 U8  *p_buffer,
 rrm_event_local_cell_setup_reject_t *p_rrm_event_local_cell_setup_reject
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_cell_setup_reject != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_cell_setup_reject->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_cell_setup_reject->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_local_cell_setup_reject->fail_cause, "fail_cause");

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_erab_drb_allocated
(
 U8  *p_buffer,
 rrm_event_erab_drb_allocated_t *p_rrm_event_erab_drb_allocated
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_erab_drb_allocated != PNULL);
    U8   index = RRM_NULL;

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_erab_drb_allocated->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_erab_drb_allocated->cell_id);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index <MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_erab_drb_allocated->drb_allocated[index], "drb_allocated[index]");
        p_buffer += sizeof(p_rrm_event_erab_drb_allocated->drb_allocated[index]);
    }

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_srb1_setup_reject
(
 U8  *p_buffer,
 rrm_event_srb1_setup_reject_t *p_rrm_event_srb1_setup_reject
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_srb1_setup_reject != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer,&p_rrm_event_srb1_setup_reject->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_srb1_setup_reject->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_srb1_setup_reject->ho_type, "ho_type");

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_erab_setup_qci
(
 U8  *p_buffer,
 rrm_event_erab_setup_qci_t *p_rrm_event_erab_setup_qci
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_erab_setup_qci != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_erab_setup_qci->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_erab_setup_qci->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_erab_setup_qci->erab_id, "erab_id");
    p_buffer += sizeof(p_rrm_event_erab_setup_qci->erab_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_erab_setup_qci->qci, "qci");

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_meas_config_a1
(
 U8  *p_buffer,
 rrm_event_meas_config_a1_t *p_rrm_event_meas_config_a1
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_a1 != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a1->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a1->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a1->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a1->meas_object_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a1->time_to_trigger, "time_to_trigger");
    p_buffer += sizeof(p_rrm_event_meas_config_a1->time_to_trigger);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a1->hysteresis, "hysteresis");
    p_buffer += sizeof(p_rrm_event_meas_config_a1->hysteresis);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a1->report_quantity, "report_quantity");
    p_buffer += sizeof(p_rrm_event_meas_config_a1->report_quantity);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a1->max_report_cells, "max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_a1->max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a1->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_a1->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a1->report_amount, "report_amount");

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_meas_config_a2
(
 U8  *p_buffer,
 rrm_event_meas_config_a2_t *p_rrm_event_meas_config_a2
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_a2 != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a2->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a2->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a2->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a2->meas_object_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a2->time_to_trigger, "time_to_trigger");
    p_buffer += sizeof(p_rrm_event_meas_config_a2->time_to_trigger);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a2->hysteresis, "hysteresis");
    p_buffer += sizeof(p_rrm_event_meas_config_a2->hysteresis);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a2->report_quantity, "report_quantity");
    p_buffer += sizeof(p_rrm_event_meas_config_a2->report_quantity);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a2->max_report_cells, "max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_a2->max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a2->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_a2->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a2->report_amount, "report_amount");

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_meas_config_a3
(
 U8  *p_buffer,
 rrm_event_meas_config_a3_t *p_rrm_event_meas_config_a3
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_a3 != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a3->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a3->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a3->measurement_id, "measurement_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a3->measurement_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a3->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a3->meas_object_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(p_buffer, &p_rrm_event_meas_config_a3->event_a3_offset, "event_a3_offset");
    p_buffer += sizeof(p_rrm_event_meas_config_a3->event_a3_offset);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a3->time_to_trigger, "time_to_trigger");
    p_buffer += sizeof(p_rrm_event_meas_config_a3->time_to_trigger);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a3->hysteresis, "hysteresis");
    p_buffer += sizeof(p_rrm_event_meas_config_a3->hysteresis);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a3->report_quantity, "report_quantity");
    p_buffer += sizeof(p_rrm_event_meas_config_a3->report_quantity);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a3->max_report_cells, "max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_a3->max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a3->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_a3->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a3->report_amount, "report_amount");

    return RRM_SUCCESS;
}



rrm_return_et
rrm_compose_local_event_meas_config_a4
(
 U8  *p_buffer,
 rrm_event_meas_config_a4_t *p_rrm_event_meas_config_a4
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_a4 != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a4->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a4->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a4->measurement_id, "measurement_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a4->measurement_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a4->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a4->meas_object_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a4->time_to_trigger, "time_to_trigger");
    p_buffer += sizeof(p_rrm_event_meas_config_a4->time_to_trigger);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a4->hysteresis, "hysteresis");
    p_buffer += sizeof(p_rrm_event_meas_config_a4->hysteresis);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a4->report_quantity, "report_quantity");
    p_buffer += sizeof(p_rrm_event_meas_config_a4->report_quantity);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a4->max_report_cells, "max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_a4->max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a4->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_a4->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a4->report_amount, "report_amount");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_meas_config_a5
(
 U8  *p_buffer,
 rrm_event_meas_config_a5_t *p_rrm_event_meas_config_a5
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_a5 != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a5->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_a5->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a5->time_to_trigger, "time_to_trigger");
    p_buffer += sizeof(p_rrm_event_meas_config_a5->time_to_trigger);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a5->hysteresis, "hysteresis");
    p_buffer += sizeof(p_rrm_event_meas_config_a5->hysteresis);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a5->report_quantity, "report_quantity");
    p_buffer += sizeof(p_rrm_event_meas_config_a5->report_quantity);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_a5->max_report_cells, "max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_a5->max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a5->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_a5->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_a5->report_amount, "report_amount");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_ho_out_prep_attempt
(
 U8  *p_buffer,
 rrm_event_ho_out_prep_attempt_t *p_rrm_event_ho_out_prep_attempt
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_ho_out_prep_attempt != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_out_prep_attempt->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_attempt->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_ho_out_prep_attempt->ho_source_target_rat, "ho_source_target_rat");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_attempt->ho_source_target_rat);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_ho_out_prep_attempt->ho_target_selection_type, "ho_target_selection_type");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_attempt->ho_target_selection_type);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_ho_out_prep_attempt->ho_type, "ho_type");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_attempt->ho_type);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_ho_out_prep_attempt->ho_prep_attempt_cause, "ho_prep_attempt_cause");

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_ho_out_prep_failed
(
 U8  *p_buffer,
 rrm_event_ho_out_prep_failed_t *p_rrm_event_ho_out_prep_failed
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_ho_out_prep_failed != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_out_prep_failed->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_failed->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_out_prep_failed->ho_prep_fail_cause_type, "ho_prep_fail_cause_type");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_failed->ho_prep_fail_cause_type);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(p_buffer, &p_rrm_event_ho_out_prep_failed->ho_prep_fail_cause_value, "ho_prep_fail_cause_value");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_failed->ho_prep_fail_cause_value);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_ho_out_prep_failed->ho_target_selection_type, "ho_target_selection_type");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_ho_out_exec_failed
(
 U8  *p_buffer,
 rrm_event_ho_out_exec_failed_t *p_rrm_event_ho_out_exec_failed
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_ho_out_exec_failed != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_out_exec_failed->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_ho_out_exec_failed->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_out_exec_failed->ho_exec_fail_cause_type, "ho_exec_fail_cause_type");
    p_buffer += sizeof(p_rrm_event_ho_out_exec_failed->ho_exec_fail_cause_type);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(p_buffer, &p_rrm_event_ho_out_exec_failed->ho_exec_fail_cause_value, "ho_exec_fail_cause_value");
    p_buffer += sizeof(p_rrm_event_ho_out_exec_failed->ho_exec_fail_cause_value);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_ho_out_exec_failed->ho_target_selection_type, "ho_target_selection_type");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_ho_out_prep_info
(
 U8  *p_buffer,
 rrm_event_ho_out_prep_info_t *p_rrm_event_ho_out_prep_info
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_ho_out_prep_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_out_prep_info->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_info->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(p_buffer, &p_rrm_event_ho_out_prep_info->erab_bitmap, "erab_bitmap");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_info->erab_bitmap);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_out_prep_info->num_drab_requested, "num_drab_requested");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_info->num_drab_requested);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(p_buffer, &p_rrm_event_ho_out_prep_info->erab_req_bitmap, "erab_req_bitmap");
    p_buffer += sizeof(p_rrm_event_ho_out_prep_info->erab_req_bitmap);

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_erab_release_info
(
 U8  *p_buffer,
 rrm_event_local_erab_release_info_t  *p_rrm_event_local_erab_release_info
 )
{
    U8  index = RRM_ZERO;
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_erab_release_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_info->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_erab_release_info->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_info->succful_released_erab_count, "succful_released_erab_count");
    p_buffer += sizeof(p_rrm_event_local_erab_release_info->succful_released_erab_count);

    for(index = RRM_ZERO; index<MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_info->requested_qci[index], "requested_qci");
        p_buffer += sizeof(p_rrm_event_local_erab_release_info->requested_qci[index]);
    }

    for(index = RRM_ZERO; index<MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_info->priority_level[index], "priority_level");
        p_buffer += sizeof(p_rrm_event_local_erab_release_info->priority_level[index]);
    }

    for(index = RRM_ZERO; index<MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_info->preemption_capability[index], "preemption_capability");
        p_buffer += sizeof(p_rrm_event_local_erab_release_info->preemption_capability[index]);
    }

    for(index = RRM_ZERO; index<MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_release_info->preemption_vulnerability[index], "preemption_vulnerability");
        p_buffer += sizeof(p_rrm_event_local_erab_release_info->preemption_vulnerability[index]);
    }

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_erab_setup_info
(
 U8  *p_buffer,
 rrm_event_local_erab_setup_info_t  *p_rrm_event_local_erab_setup_info
 )
{
    U8  index = RRM_ZERO;

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_erab_setup_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_setup_info->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_erab_setup_info->cell_id);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index<MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_erab_setup_info->erab_setup_result[index], "erab_setup_result");
        p_buffer += sizeof(p_rrm_event_local_erab_setup_info->erab_setup_result[index]);
    }

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_ho_in_prep_attempt
(
 U8  *p_buffer,
 rrm_event_local_ho_in_prep_attempt_t   *p_rrm_event_local_ho_in_prep_attempt
 )
{
    U8  index = RRM_ZERO;
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_ho_in_prep_attempt != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_ho_in_prep_attempt->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_ho_in_prep_attempt->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_local_ho_in_prep_attempt->ho_type, "ho_type");
    p_buffer += sizeof(p_rrm_event_local_ho_in_prep_attempt->ho_type);

    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index<MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_ho_in_prep_attempt->ho_in_prep_erab[index], "ho_in_prep_erab");
        p_buffer += sizeof(p_rrm_event_local_ho_in_prep_attempt->ho_in_prep_erab[index]);
    }


    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index<MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_ho_in_prep_attempt->priority_level[index], "priority_level");
        p_buffer += sizeof(p_rrm_event_local_ho_in_prep_attempt->priority_level[index]);
    }


    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index<MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_ho_in_prep_attempt->preemption_capability[index], "preemption_capability");
        p_buffer += sizeof(p_rrm_event_local_ho_in_prep_attempt->preemption_capability[index]);
    }


    /* Compose parameter of basic type */
    for(index = RRM_ZERO; index<MAX_ERAB_ID; index++)
    {
        rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_ho_in_prep_attempt->preemption_vulnerability[index], "preemption_vulnerability");
        p_buffer += sizeof(p_rrm_event_local_ho_in_prep_attempt->preemption_vulnerability[index]);
    }

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_ho_in_prep_failure
(
 U8  *p_buffer,
 rrm_event_local_ho_in_prep_failure_t  *p_rrm_event_local_ho_in_prep_failure
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_local_ho_in_prep_failure != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_local_ho_in_prep_failure->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_local_ho_in_prep_failure->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_local_ho_in_prep_failure->ho_src, "ho_src");
    p_buffer += sizeof(p_rrm_event_local_ho_in_prep_failure->ho_src);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_local_ho_in_prep_failure->ho_prep_fail_cause, "ho_prep_fail_cause");

    return RRM_SUCCESS;
}


rrm_return_et
rrm_compose_local_event_ho_out_exec_info
(
 U8  *p_buffer,
 rrm_event_ho_out_exec_info_t  *p_rrm_event_ho_out_exec_info
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_ho_out_exec_info != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_out_exec_info->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_ho_out_exec_info->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(p_buffer, &p_rrm_event_ho_out_exec_info->ho_out_prep_erab_req_bitmap, "ho_out_prep_erab_req_bitmap");
    p_buffer += sizeof(p_rrm_event_ho_out_exec_info->ho_out_prep_erab_req_bitmap);

    /* Compose parameter of basic type */
    rrm_cp_pack_U16(p_buffer, &p_rrm_event_ho_out_exec_info->ho_out_exec_erab_req_bitmap, "ho_out_exec_erab_req_bitmap");
    p_buffer += sizeof(p_rrm_event_ho_out_exec_info->ho_out_exec_erab_req_bitmap);

    /* Compose parameter of basic type  :gur30485 */
    /*    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_out_exec_info->drx_config_index, "drx_config_index");
          p_buffer += sizeof(p_rrm_event_ho_out_exec_info->drx_config_index);*/

    return RRM_SUCCESS;    
}


rrm_return_et
rrm_compose_local_event_ho_in_prep_complete
(
 U8   *p_buffer, 
 rrm_event_local_ho_in_prep_complete_t  *p_ho_in_prep_comp
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_ho_in_prep_comp != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_ho_in_prep_comp->cell_id, "cell_id");
    p_buffer += sizeof(p_ho_in_prep_comp->cell_id);

    rrm_cp_pack_U16(p_buffer, &p_ho_in_prep_comp->erab_bitmap, "erab_bitmap");
    p_buffer += sizeof(p_ho_in_prep_comp->erab_bitmap);

    rrm_cp_pack_U8(p_buffer, &p_ho_in_prep_comp->erab_admtd_count, "erab_admtd_count");

    return RRM_SUCCESS;
}



rrm_return_et
rrm_compose_local_event_ho_in_exec_attempt
(
 U8    *p_buffer, 
 rrm_event_local_ho_in_exec_attempt_t  *p_ho_in_exec_attempt
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_ho_in_exec_attempt != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_ho_in_exec_attempt->cell_id, "cell_id");
    p_buffer += sizeof(p_ho_in_exec_attempt->cell_id);

    rrm_cp_pack_U16(p_buffer, &p_ho_in_exec_attempt->ho_type, "ho_type");

    return RRM_SUCCESS;
}



rrm_return_et
rrm_compose_local_event_ho_in_exec_failed
(
 U8   *p_buffer, 
 rrm_event_local_ho_in_exec_failed_t  *p_ho_in_exec_fail
 )
{
    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_ho_in_exec_fail != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_ho_in_exec_fail->cell_id, "cell_id");
    p_buffer += sizeof(p_ho_in_exec_fail->cell_id);

    rrm_cp_pack_U16(p_buffer, &p_ho_in_exec_fail->fail_cause, "fail_cause");
    p_buffer += sizeof(p_ho_in_exec_fail->fail_cause);

    rrm_cp_pack_U8(p_buffer, &p_ho_in_exec_fail->rat_type, "rat_type");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_meas_config_b2_geran
(
 U8  *p_buffer,
 rrm_event_meas_config_b2_geran_t  *p_rrm_event_meas_config_b2_geran
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_b2_geran != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_geran->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_geran->measurement_id, "measurement_id");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->measurement_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_geran->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->meas_object_id);

    /* Compose parameter of basic type */
    /*rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_geran->meas_requester, "meas_requester");
      p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->meas_requester);*/

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_geran->eutra_threshold1, "eutra_threshold1");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->eutra_threshold1);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_geran->geran_threshold2, "geran_threshold2");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->geran_threshold2);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_geran->time_to_trigger, "time_to_trigger");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->time_to_trigger);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_geran->hysteresis, "hysteresis");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->hysteresis);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_geran->trigger_quantity, "trigger_quantity");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->trigger_quantity);

    /* Compose parameter of basic type */
    /*rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_geran->geran_frequency_group, "geran_frequency_group");
      p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->geran_frequency_group);*/

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_geran->max_report_cells, "max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_geran->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_geran->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_geran->report_amount, "report_amount");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_meas_config_b2_utra
(
 U8  *p_buffer,
 rrm_event_meas_config_b2_utra_t  *p_rrm_event_meas_config_b2_utra
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_b2_utra != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_utra->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_utra->measurement_id, "measurement_id");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->measurement_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_utra->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->meas_object_id);

    /* Compose parameter of basic type */
    /*rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_utra->meas_requester, "meas_requester");
      p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->meas_requester);*/

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_utra->eutra_threshold1, "eutra_threshold1");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->eutra_threshold1);

    /* Compose parameter of basic type */
    rrm_cp_pack_S8(p_buffer, &p_rrm_event_meas_config_b2_utra->threshold2_rscp, "threshold2_rscp");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->threshold2_rscp);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_utra->threshold2_ecn0, "threshold2_ecn0");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->threshold2_ecn0);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_utra->time_to_trigger, "time_to_trigger");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->time_to_trigger);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_utra->hysteresis, "hysteresis");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->hysteresis);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_utra->trigger_quantity, "trigger_quantity");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->trigger_quantity);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_utra->trigger_quantity_utra, "trigger_quantity_utra");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->trigger_quantity_utra);

    /* Compose parameter of basic type */
    /*rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_utra->arfcn_dl, "arfcn_dl");
      p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->arfcn_dl);*/

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_b2_utra->max_report_cells, "max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_utra->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_b2_utra->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_b2_utra->report_amount, "report_amount");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_ue_meas_abort
(
 U8  *p_buffer,
 rrm_event_ue_meas_abort_t  *p_rrm_event_ue_meas_abort
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_ue_meas_abort != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ue_meas_abort->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_ue_meas_abort->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ue_meas_abort->measurement_id, "measurement_id");
    /*p_buffer += sizeof(p_rrm_event_ue_meas_abort->measurement_id);*/

    /* Compose parameter of basic type */
    /* rrm_cp_pack_U32(p_buffer, &p_rrm_event_ue_meas_abort->meas_requester, "meas_requester");
       p_buffer += sizeof(p_rrm_event_ue_meas_abort->meas_requester);*/

    /* Compose parameter of basic type */
    /*rrm_cp_pack_U32(p_buffer, &p_rrm_event_ue_meas_abort->meas_holder, "meas_holder");
      p_buffer += sizeof(p_rrm_event_ue_meas_abort->meas_holder);*/

    /* Compose parameter of basic type */
    /* rrm_cp_pack_U32(p_buffer, &p_rrm_event_ue_meas_abort->cause, "cause");*/

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_too_early_ho
(
 U8  *p_buffer,
 rrm_event_too_early_ho_t  *p_rrm_event_too_early_ho
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_too_early_ho != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_too_early_ho->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_too_early_ho->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_too_early_ho->target_rat_type, "target_rat_type");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_too_late_ho
(
 U8  *p_buffer,
 rrm_event_too_late_ho_t  *p_rrm_event_too_late_ho
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_too_late_ho != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_too_late_ho->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_too_late_ho->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_too_late_ho->target_rat_type, "target_rat_type");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_ho_wrong_cell
(
 U8  *p_buffer,
 rrm_event_ho_wrong_cell_t  *p_rrm_event_ho_wrong_cell
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_ho_wrong_cell != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_wrong_cell->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_ho_wrong_cell->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_wrong_cell->target_rat_type, "target_rat_type");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_ho_wrong_cell_reest
(
 U8  *p_buffer,
 rrm_event_ho_wrong_cell_reest_t  *p_rrm_event_ho_wrong_cell_reest
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_ho_wrong_cell_reest != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_wrong_cell_reest->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_ho_wrong_cell_reest->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_ho_wrong_cell_reest->target_rat_type, "target_rat_type");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_meas_config_periodical_eutran
(
 U8  *p_buffer,
 rrm_event_meas_config_periodical_eutran_t  *p_rrm_event_meas_config_periodical_eutran
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_periodical_eutran != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_eutran->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_eutran->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_eutran->measurement_id, "measurement_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_eutran->measurement_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_eutran->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_eutran->meas_object_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_eutran->periodical_purpose, "periodical_purpose");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_eutran->periodical_purpose);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_eutran->event_param_max_report_cells, "event_param_max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_eutran->event_param_max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_eutran->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_eutran->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_eutran->report_amount, "report_amount");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_meas_config_periodical_utra
(
 U8  *p_buffer,
 rrm_event_meas_config_periodical_utra_t  *p_rrm_event_meas_config_periodical_utra
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_periodical_utra != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_utra->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_utra->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_utra->measurement_id, "measurement_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_utra->measurement_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_utra->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_utra->meas_object_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_utra->periodical_purpose, "periodical_purpose");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_utra->periodical_purpose);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_utra->event_param_max_report_cells, "event_param_max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_utra->event_param_max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_utra->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_utra->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_utra->report_amount, "report_amount");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_meas_config_periodical_geran
(
 U8  *p_buffer,
 rrm_event_meas_config_periodical_geran_t  *p_rrm_event_meas_config_periodical_geran
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_periodical_geran != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_geran->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_geran->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_geran->measurement_id, "measurement_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_geran->measurement_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_geran->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_geran->meas_object_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_geran->periodical_purpose, "periodical_purpose");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_geran->periodical_purpose);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_geran->event_param_max_report_cells, "event_param_max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_geran->event_param_max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_geran->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_geran->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_geran->report_amount, "report_amount");

    return RRM_SUCCESS;
}

rrm_return_et
rrm_compose_local_event_meas_config_periodical_cdma
(
 U8  *p_buffer,
 rrm_event_meas_config_periodical_cdma_t  *p_rrm_event_meas_config_periodical_cdma
 )
{

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_rrm_event_meas_config_periodical_cdma != PNULL);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_cdma->cell_id, "cell_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_cdma->cell_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_cdma->measurement_id, "measurement_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_cdma->measurement_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_cdma->meas_object_id, "meas_object_id");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_cdma->meas_object_id);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_cdma->periodical_purpose, "periodical_purpose");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_cdma->periodical_purpose);

    /* Compose parameter of basic type */
    rrm_cp_pack_U8(p_buffer, &p_rrm_event_meas_config_periodical_cdma->event_param_max_report_cells, "event_param_max_report_cells");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_cdma->event_param_max_report_cells);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_cdma->report_interval, "report_interval");
    p_buffer += sizeof(p_rrm_event_meas_config_periodical_cdma->report_interval);

    /* Compose parameter of basic type */
    rrm_cp_pack_U32(p_buffer, &p_rrm_event_meas_config_periodical_cdma->report_amount, "report_amount");

    return RRM_SUCCESS;
}




    rrm_return_et
rrm_compose_error_event_info(U8                      *p_buffer, 
        rrm_event_error_info_t  *p_err_info)
{
    U8 loop = RRM_ZERO;

    RRM_ASSERT(p_buffer != PNULL);
    RRM_ASSERT(p_err_info != PNULL);

    rrm_cp_pack_U32(p_buffer, &p_err_info->bitmask, "Bitmask");
    p_buffer += sizeof(p_err_info->bitmask);

    rrm_cp_pack_U32(p_buffer, &p_err_info->error_type, "error_type");
    p_buffer += sizeof(p_err_info->error_type);

    if (p_err_info->bitmask & RRM_ERROR_EVENT_CAUSE_PRESENT)
    {
        rrm_cp_pack_U32(p_buffer, &p_err_info->error_cause, "error_cause");
        p_buffer += sizeof(p_err_info->error_cause);
    }

    if (p_err_info->bitmask & RRM_ERROR_EVENT_SEVERITY_PRESENT)
    {
        rrm_cp_pack_U32(p_buffer, &p_err_info->severity, "severity");
        p_buffer += sizeof(p_err_info->severity);
    }


    if (p_err_info->bitmask & RRM_ERROR_EVENT_DATA_PRESENT)
    {
        for (loop = RRM_ZERO; loop < RRM_ERR_EVENT_MAX_ASSOCIATED_DATA_LEN; loop++)
        {
            rrm_cp_pack_U8(p_buffer, &p_err_info->associtated_data[loop], "associtated_data[]");
            *p_buffer += sizeof(U8);
        }
    }

    return RRM_SUCCESS;
}

