/******************************************************************************
*
*   FILE NAME:
*       uecc_llim_utils.c
*
*   DESCRIPTION:
*       This is utilities functions implementation for LLIM interface.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   20 Aug 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "uecc_llim_utils.h"
#include "uecc_defaults.h"
#include "uecc_logger.h"
#include "rrc_phy_intf.h"
#include "rrm_rrc_asn1_convertor.h"
#include "uecc_global_ctx.h"
#include <lteMisc.h>


#ifdef ENDC_ENABLED
#include "rrc_pdcp_il_composer.h"
#include "rrc_common_md.h"
#endif
/****************************************************************************
* Defines
***************************************************************************/

/****************************************************************************
 * Local functions definitions
 ****************************************************************************/
static rrc_return_et uecc_llim_build_MeasObjectToRemoveList(
    uecc_ue_context_t*              p_uecc_ue_context,
    meas_object_to_remove_list_t*   p_rrm_meas_object_to_remove_list,
    MeasObjectToRemoveList*         p_meas_object_to_remove_list
);

static rrc_return_et uecc_llim_build_MeasObjectToAddModList(
    uecc_ue_context_t*              p_uecc_ue_context,
    meas_object_to_add_mod_list_t*  p_rrm_meas_object_to_add_mod_list,
    OSCTXT*                         p_asn1_ctx,
    MeasObjectToAddModList*         p_meas_object_to_add_mod_list
);

static rrc_return_et uecc_llim_build_MeasObjectToAddMod(
    uecc_ue_context_t*              p_uecc_ue_context,
    meas_object_to_add_mod_t*       p_rrm_meas_object_to_add_mod,
    OSCTXT*                         p_asn1_ctx,
    MeasObjectToAddMod*             p_meas_object_to_add_mod
);

static rrc_return_et uecc_llim_build_ReportConfigToRemoveList(
    uecc_ue_context_t*              p_uecc_ue_context,
    report_config_to_remove_list_t* p_rrm_report_config_to_remove_list,
    ReportConfigToRemoveList*       p_report_config_to_remove_list
);

static rrc_return_et uecc_llim_build_ReportConfigToAddModList(
    uecc_ue_context_t*                  p_uecc_ue_context,
    report_config_to_add_mod_list_t*    p_rrm_report_config_to_add_mod_list,
    OSCTXT*                             p_asn1_ctx,
    ReportConfigToAddModList*           p_report_config_to_add_mod_list
);

static rrc_return_et uecc_llim_build_MeasIdToRemoveList(
    uecc_ue_context_t*              p_uecc_ue_context,
    meas_id_to_remove_list_t*       p_rrm_meas_id_to_remove_list,
    MeasIdToRemoveList*             p_meas_id_to_remove_list
);

static rrc_return_et uecc_llim_build_MeasIdToAddModList(
    uecc_gb_context_t           *p_uecc_gb_context,
    meas_id_to_add_mod_list_t*  p_rrm_meas_id_to_add_mod_list,
    OSCTXT*                     p_asn1_ctx,
    MeasIdToAddModList*         p_meas_id_to_add_mod_list
);

static rrc_return_et uecc_llim_build_QuantityConfig(
    uecc_gb_context_t           *p_uecc_gb_context,
    quantity_config_t*  p_rrm_quantity_config,
    OSCTXT*             p_asn1_ctx,
    QuantityConfig*     p_quantity_config
);

static rrc_return_et uecc_llim_build_MeasGapConfig(
    uecc_ue_context_t*              p_uecc_ue_context,
    meas_gap_config_t*              p_rrm_meas_gap_config,
    OSCTXT*                         p_asn1_ctx,
    MeasGapConfig*                  p_meas_gap_config
);

static void uecc_llim_build_PreRegistrationInfoHRPD(
    uecc_gb_context_t               *p_uecc_gb_context,
    pre_registration_info_hrpd_t*   p_rrm_pre_registration_info_hrpd,
    PreRegistrationInfoHRPD*        p_pre_registration_info_hrpd
);

static rrc_return_et uecc_llim_build_MeasConfig_speedStatePars(
    uecc_gb_context_t               *p_uecc_gb_context,
    meas_config_speed_state_pars_t* p_rrm_meas_config_speed_state_pars,
    OSCTXT*                         p_asn1_ctx,
    MeasConfig_speedStatePars*      p_meas_config_speed_state_pars
);

static rrc_return_et uecc_llim_build_MeasObjectCDMA2000(
    uecc_gb_context_t               *p_uecc_gb_context,
    meas_object_cdma2000_t*     p_rrm_meas_object_cdma2000,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectCDMA2000*         p_meas_object_cdma2000
);

static rrc_return_et uecc_llim_build_CellsToAddModListCDMA2000(
    uecc_gb_context_t               *p_uecc_gb_context,
    cells_to_add_mod_list_cdma2000_list_t*
        p_rrm_cells_to_add_mod_list_cdma2000_list,
    OSCTXT*                                 p_asn1_ctx,
    CellsToAddModListCDMA2000*              p_cells_to_add_mod_list_cdma2000
);
static rrc_return_et uecc_llim_build_MeasObjectEUTRA(
    uecc_ue_context_t*          p_uecc_ue_context,
    meas_object_eutra_t*        p_rrm_meas_object_eutra,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectEUTRA*         p_meas_object_eutra
);

static rrc_return_et uecc_llim_build_CellsToAddModListEUTRA(
   uecc_gb_context_t               *p_uecc_gb_context, 
    cells_to_add_mod_list_t*
        p_rrm_cells_to_add_mod_list_eutra_list,
    OSCTXT*                                 p_asn1_ctx,
    CellsToAddModList*              p_cells_to_add_mod_list_eutra
);

static rrc_return_et uecc_llim_build_BlackListedCellsToAddModListEUTRA(
    uecc_gb_context_t               *p_uecc_gb_context,
    black_cells_to_add_mod_list_t*
        p_rrm_black_cells_to_add_mod_list_eutra_list,
    OSCTXT*                                 p_asn1_ctx,
    BlackCellsToAddModList*              p_black_cells_to_add_mod_list_eutra
);

static rrc_return_et uecc_llim_build_MeasObjectUTRA(
    uecc_ue_context_t*          p_uecc_ue_context,
    meas_object_utra_t*     p_rrm_meas_object_utra,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectUTRA*         p_meas_object_utra
);

static rrc_return_et uecc_llim_build_CellsToAddModListUTRA_FDD(
    uecc_gb_context_t               *p_uecc_gb_context,
    cells_to_add_mod_list_utra_fdd_t*
        p_rrm_cells_to_add_mod_list_utra_fdd,
    OSCTXT*                                 p_asn1_ctx,
    MeasObjectUTRA*             p_meas_object_utra
);

static rrc_return_et uecc_llim_build_CellsToAddModListUTRA_TDD(
    uecc_gb_context_t               *p_uecc_gb_context,
    cells_to_add_mod_list_utra_tdd_t*
        p_rrm_cells_to_add_mod_list_utra_tdd,
    OSCTXT*                                 p_asn1_ctx,
    MeasObjectUTRA*             p_meas_object_utra
);

static rrc_return_et uecc_llim_build_ReportConfigToAddMod(
    uecc_ue_context_t*          p_uecc_ue_context,
    report_config_to_add_mod_t* p_rrm_report_config_to_add_mod,
    OSCTXT*                     p_asn1_ctx,
    ReportConfigToAddMod*       p_report_config_to_add_mod
);

static rrc_return_et uecc_llim_build_ReportConfigEUTRA(
    uecc_ue_context_t*          p_uecc_ue_context,
    report_config_eutra_t*      p_rrm_report_config_eutra,
    OSCTXT*                     p_asn1_ctx,
    ReportConfigEUTRA*          p_report_config_eutra,
    rrc_bool_et                 report_cfg_eutra_ext_present,
    report_config_eutra_ext_t*  p_rrm_report_config_eutra_ext
);

static rrc_return_et uecc_llim_build_ReportConfigInterRAT(
    uecc_ue_context_t*          p_uecc_ue_context,
    report_config_interrat_t*   p_rrm_report_config_interrat,
    OSCTXT*                     p_asn1_ctx,
    ReportConfigInterRAT*       p_report_config_interrat
);

static rrc_return_et uecc_llim_build_ReportConfigEUTRA_triggerType_event_eventId
(
    uecc_ue_context_t*                                  p_uecc_ue_context,
    report_config_eutra_trigger_type_event_eventid_t*   p_rrm_event_id,
    OSCTXT*                                             p_asn1_ctx,
    ReportConfigEUTRA_triggerType_event_eventId*        p_event_id
);

static rrc_return_et
 uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId(
    uecc_ue_context_t*                                      p_uecc_ue_context,
    report_config_interrat_trigger_type_event_eventid_t*    p_rrm_event_id,
    OSCTXT*                                                 p_asn1_ctx,
    ReportConfigInterRAT_triggerType_event_eventId*         p_event_id
);

static rrc_return_et uecc_llim_build_ThresholdEUTRA(
    uecc_ue_context_t*  p_uecc_ue_context,
    threshold_eutra_t*  p_rrm_threshold_eutra,
    ThresholdEUTRA*     p_threshold_eutra
);

static rrc_return_et uecc_llim_build_ThresholdUTRA(
    uecc_ue_context_t*  p_uecc_ue_context,
    threshold_utra_t*   p_rrm_threshold_utra,
    ThresholdUTRA*      p_threshold_utra
);

static rrc_return_et uecc_llim_build_b1_Threshold(
    uecc_ue_context_t*                          p_uecc_ue_context,
    report_config_interrat_trigger_type_event_eventid_event_b1_t*
        p_rrm_b1_threshold,
    OSCTXT* p_asn1_ctx,
    ReportConfigInterRAT_triggerType_event_eventId_eventB1_b1_Threshold*
        p_b1_threshold
);

static rrc_return_et
 uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId_eventB2(
    uecc_ue_context_t*  p_uecc_ue_context,
    report_config_interrat_trigger_type_event_eventid_event_b2_t*
        p_rrm_b2_threshold,
    OSCTXT*                                                     p_asn1_ctx,
    ReportConfigInterRAT_triggerType_event_eventId_eventB2*     p_event_b2
);


static rrc_return_et uecc_llim_build_MeasObjectGERAN(
    uecc_ue_context_t*       p_uecc_ue_context,
    meas_object_geran_t*     p_rrm_meas_object_geran,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectGERAN*         p_meas_object_geran
);

/* meas_config change start */
static rrc_return_et uecc_llim_build_CurrMeasObjectToAddModList(
    uecc_ue_context_t*              p_uecc_ue_context,
    uecc_meas_object_to_add_mod_list_t*  p_rrm_curr_meas_object_to_add_mod_list,
    OSCTXT*                         p_asn1_ctx,
    MeasObjectToAddModList*         p_meas_object_to_add_mod_list
);

static rrc_return_et uecc_llim_build_CurrMeasObjectToAddMod(
    uecc_ue_context_t*              p_uecc_ue_context,
    uecc_meas_object_to_add_mod_t*   p_ue_meas_object_to_add_mod,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectToAddMod*         p_meas_object_to_add_mod
);

static rrc_return_et uecc_llim_build_CurrMeasObjectCDMA2000(
    uecc_gb_context_t           *p_uecc_gb_context, 
    meas_object_cdma2000_t*     p_rrm_meas_object_cdma2000,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectCDMA2000*         p_meas_object_cdma2000
);

static rrc_return_et uecc_llim_build_CurrCellsToAddModListCDMA2000(
    uecc_gb_context_t           *p_uecc_gb_context,
    cells_to_add_mod_list_cdma2000_list_t*
        p_rrm_cells_to_add_mod_list_cdma2000_list,
    OSCTXT*                                 p_asn1_ctx,
    CellsToAddModListCDMA2000*              p_cells_to_add_mod_list_cdma2000
);

static rrc_return_et uecc_llim_build_CurrMeasObjectEUTRA(
    uecc_ue_context_t*              p_uecc_ue_context,
    meas_object_eutra_t*     p_rrm_meas_object_eutra,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectEUTRA*         p_meas_object_eutra
);

static rrc_return_et uecc_llim_build_CurrCellsToAddModListEUTRA(
    uecc_gb_context_t               *p_uecc_gb_context, 
    cells_to_add_mod_list_t*   p_rrm_cells_to_add_mod_list_eutra_list,
    OSCTXT*                    p_asn1_ctx,
    CellsToAddModList*         p_cells_to_add_mod_list_eutra
);

static rrc_return_et uecc_llim_build_CurrBlackListedCellsToAddModListEUTRA(
    uecc_gb_context_t               *p_uecc_gb_context,
    black_cells_to_add_mod_list_t* p_rrm_black_cells_to_add_mod_list_eutra_list,
    OSCTXT*                        p_asn1_ctx,
    BlackCellsToAddModList*        p_black_cells_to_add_mod_list_eutra
);

static rrc_return_et uecc_llim_build_CurrMeasObjectUTRA(
    uecc_ue_context_t*              p_uecc_ue_context,
    meas_object_utra_t*         p_rrm_meas_object_utra,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectUTRA*             p_meas_object_utra
);

static rrc_return_et uecc_llim_build_CurrCellsToAddModListUTRA_FDD(
    uecc_gb_context_t                 *p_uecc_gb_context,
    cells_to_add_mod_list_utra_fdd_t* p_rrm_cells_to_add_mod_list_utra_fdd,
    OSCTXT*                           p_asn1_ctx,
    MeasObjectUTRA*                   p_meas_object_utra
);

static rrc_return_et uecc_llim_build_CurrCellsToAddModListUTRA_TDD(
    uecc_gb_context_t                 *p_uecc_gb_context,
    cells_to_add_mod_list_utra_tdd_t* p_rrm_cells_to_add_mod_list_utra_tdd,
    OSCTXT*                           p_asn1_ctx,
    MeasObjectUTRA*                   p_meas_object_utra
);

static rrc_return_et uecc_llim_build_CurrMeasIdToAddModList(
    uecc_gb_context_t           *p_uecc_gb_context,/* SPR 15905 Fix Start */
    curr_meas_id_to_add_mod_list_t*  p_rrm_curr_meas_id_to_add_mod_list,/* SPR 15905 Fix Stop */
    OSCTXT*                     p_asn1_ctx,
    MeasIdToAddModList*         p_meas_id_to_add_mod_list
);

static rrc_return_et uecc_llim_build_CurrReportConfigToAddModList(
   uecc_ue_context_t*                        p_uecc_ue_context, 
    uecc_report_config_to_add_mod_list_t*    p_ue_curr_report_config_to_add_mod_list,
    OSCTXT*                             p_asn1_ctx,
    ReportConfigToAddModList*           p_report_config_to_add_mod_list
);

static rrc_return_et uecc_llim_build_report_configToAddMod(
    uecc_ue_context_t*                        p_uecc_ue_context,
    uecc_report_config_to_add_mod_t* p_rrm_report_config_to_add_mod,
    OSCTXT*                     p_asn1_ctx,
    ReportConfigToAddMod*       p_report_config_to_add_mod
);
/* meas_config change end */

#ifdef ENDC_ENABLED
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_MeasObjectNR_r15
 *   INPUT        : uecc_ue_context_t*       p_uecc_ue_context,
 *                  meas_object_nr_t*        p_rrm_meas_object_nr_r15,
 *                  OSCTXT*                  p_asn1_ctx,
 *                  MeasObjectNR_r15*        p_meas_object_nr_r15
 *   OUTPUT       : none
 *   DESCRIPTION  : This function fills Meas Object NR r15 Parameters
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 ******************************************************************************/
rrc_return_et
uecc_llim_build_MeasObjectNR_r15
(
    uecc_ue_context_t*       p_uecc_ue_context,
	meas_object_nr_t*        p_rrm_meas_object_nr_r15,
	OSCTXT*                  p_asn1_ctx,
	MeasObjectNR_r15*        p_meas_object_nr_r15
);
#endif


/****************************************************************************
 * Functions implementation
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_radio_resource_cfg_dedicated
*   INPUT        : uecc_ue_context_t               *p_uecc_ue_context
*                  RadioResourceConfigDedicated    *p_asn1_rrc
*                  rrm_srb_info_t                  *p_srb_info
*                  OSCTXT                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function builds RadioResourceConfigDedicated
*                  structure for ASN1 encoding from given ue_adm_radio_res_config_t
*
*   RETURNS      : RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_radio_resource_cfg_dedicated(
    uecc_ue_context_t               *p_uecc_ue_context,
    RadioResourceConfigDedicated    *p_asn1_rrc,
    rrm_srb_info_t                  *p_srb_info,
    OSCTXT                          *p_asn1_ctx
)
{
    rrc_return_et result = RRC_FAILURE;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    do
    {
        asn1Init_RadioResourceConfigDedicated(p_asn1_rrc);

        /* SRB_ToAddModList */
        if ( PNULL != p_srb_info )
        {
            p_asn1_rrc->m.srb_ToAddModListPresent = 1;

            /* Generate SRB Information */
            if (RRC_SUCCESS != uecc_llim_build_asn1_srb_to_add_mod_list(
                                    /* CSR 00070192 Fix Start */
                                    p_uecc_ue_context,
                                    /* CSR 00070192 Fix Stop */
                                    &p_asn1_rrc->srb_ToAddModList,
                                    p_srb_info,
                                    p_asn1_ctx))
            {
                break;
            }
        }

        /* RadioResourceConfigDedicated_mac_MainConfig */
        if ( p_uecc_ue_context->m.mac_config_updated )
        {
            if ( p_uecc_ue_context->m.mac_main_config_present)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_INFO,"RCS: mac_MainConfigPresent");
            p_asn1_rrc->m.mac_MainConfigPresent = 1;

            if (RRC_SUCCESS != uecc_llim_build_asn1_mac_main_config(
                                        p_uecc_ue_context,
                                        &(p_uecc_ue_context->mac_config),
                                        &p_asn1_rrc->mac_MainConfig,
                                        p_asn1_ctx))
            {
                break;
            }
        }
            else
            {
                result = RRC_FAILURE;
                break;
            }

        }
        /* ERAB RELEASE COMMAND START */
        /* DRB_ToReleaseList */
        if (PNULL != p_uecc_ue_context->p_curr_proc_data)
        {    
        if (0 < p_uecc_ue_context->p_curr_proc_data->u.
            erab_release_command_data.erab_release_list.counter)
        {
            p_asn1_rrc->m.drb_ToReleaseListPresent = 1; 

            /* Generate DRB Information */
            if (RRC_SUCCESS != uecc_llim_build_asn1_drb_to_release_list(
                                    p_uecc_ue_context,
                                    &p_asn1_rrc->drb_ToReleaseList,
                                    p_asn1_ctx))
                {
                    break;
                }
            }
        }
       /* ERAB RELEASE COMMAND END */

        /* SPS Configuration */
        if ( p_uecc_ue_context->m.sps_config_updated )
        {
            p_asn1_rrc->m.sps_ConfigPresent = 1;

            if (RRC_SUCCESS != uecc_llim_build_asn1_sps_config(
                                        p_uecc_ue_context,
                                        /* SPS Start */
                                        p_uecc_ue_context->p_sps_config,
                                        /* SPS Stop */
                                        &p_asn1_rrc->sps_Config,
                                        p_asn1_ctx))
            {
                break;
            }
        }

        /* PhysicalConfigDedicated */
        if (  p_uecc_ue_context->m.physical_config_dedicated_updated )
        {
            p_asn1_rrc->m.physicalConfigDedicatedPresent = 1;

            /* eMTC changes start */
            if (RRC_SUCCESS != uecc_llim_build_asn1_physical_config_dedicated(
                        p_uecc_ue_context, 
                        &(p_uecc_ue_context->physical_config_dedicated),
                        &p_asn1_rrc->physicalConfigDedicated,
                        p_asn1_ctx)
               )
            {
                break;
            }
        }

        /* CR_220 */
        /* RLF TIMERS & CONSTANTS*/
        if( p_uecc_ue_context->m.rlf_timers_and_constants_r9_present )
        {
            if (PNULL != p_uecc_ue_context->p_rlf_timers_and_constants_r9)
            {
                p_asn1_rrc->m.rlf_TimersAndConstants_r9Present = 1;

                p_asn1_rrc->m._v2ExtPresent = 1;
                if (RRC_SUCCESS != uecc_llim_build_asn1_rlf_timers_and_constants_r9(
                                p_uecc_ue_context, 
                                (p_uecc_ue_context->p_rlf_timers_and_constants_r9),
                                &p_asn1_rrc->rlf_TimersAndConstants_r9,
                                p_asn1_ctx)
                   )
                {
                    break;
                }
            }
        }


        /* main extElem1 */
        rtxDListInit(&p_asn1_rrc->extElem1);

        result = RRC_SUCCESS;
    }
    while(0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_tpc_pucch_pusch_config
*
*   DESCRIPTION: TPC PDCCH Configuration PUCCH and PDCCH Configuration PUSCH 
*
*   RETURNS:     RRC_FAILURE/RRC_SUCCESS
*
******************************************************************************/
static rrc_return_et uecc_llim_build_asn1_tpc_pucch_pusch_config(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_phy_physical_config_dedicated_t const       *p_phy,
    PhysicalConfigDedicated                         *p_asn1_phy,
    OSCTXT                                          *p_asn1_ctx
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    if (p_phy->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT)
    {
        p_asn1_phy->m.tpc_PDCCH_ConfigPUCCHPresent = 1;

        if (p_phy->tpc_pdcch_config_pucch.bitmask &
                RRC_PHY_TPC_PDCCH_CONFIG_PARAM_PRESENT)
        {
            /* TPC PDCCH Configuration PUCCH setup */
            p_asn1_phy->tpc_PDCCH_ConfigPUCCH.t = T_TPC_PDCCH_Config_setup;

            p_asn1_phy->tpc_PDCCH_ConfigPUCCH.u.setup =
                rtxMemAllocType(p_asn1_ctx, TPC_PDCCH_Config_setup);
            if (PNULL == p_asn1_phy->tpc_PDCCH_ConfigPUCCH.u.setup)
            {
                return RRC_FAILURE;                    
            }

            /* tpc_RNTI */
            RRC_ASSERT(sizeof(p_asn1_phy->tpc_PDCCH_ConfigPUCCH.u.setup->
                        tpc_RNTI.data) >=
                    sizeof(p_phy->tpc_pdcch_config_pucch.
                        tpc_pdcch_config_param.tpc_rnti));
            p_asn1_phy->tpc_PDCCH_ConfigPUCCH.u.setup->tpc_RNTI.numbits =
                sizeof(p_phy->tpc_pdcch_config_pucch.
                        tpc_pdcch_config_param.tpc_rnti)*8;
            l3_memcpy_wrapper(
                    p_asn1_phy->tpc_PDCCH_ConfigPUCCH.u.setup->tpc_RNTI.data,
                    p_phy->tpc_pdcch_config_pucch.
                    tpc_pdcch_config_param.tpc_rnti,
                    sizeof(p_phy->tpc_pdcch_config_pucch.
                        tpc_pdcch_config_param.tpc_rnti));

            /* TPC Index */
            if (p_phy->tpc_pdcch_config_pucch.tpc_pdcch_config_param.
                    tpc_index.bitmask & TPC_INDEX_FORMAT_3_PRESENT)
            {
                p_asn1_phy->tpc_PDCCH_ConfigPUCCH.u.setup->tpc_Index.t =
                    T_TPC_Index_indexOfFormat3;

                /* indexOfFormat3 */
                p_asn1_phy->tpc_PDCCH_ConfigPUCCH.u.setup->
                    tpc_Index.u.indexOfFormat3 =
                    p_phy->tpc_pdcch_config_pucch.tpc_pdcch_config_param.
                    tpc_index.index_of_format3;
            }
            else if (p_phy->tpc_pdcch_config_pucch.tpc_pdcch_config_param.
                    tpc_index.bitmask & TPC_INDEX_FORMAT_3A_PRESENT)
            {
                p_asn1_phy->tpc_PDCCH_ConfigPUCCH.u.setup->tpc_Index.t =
                    T_TPC_Index_indexOfFormat3A;

                /* indexOfFormat3A */
                p_asn1_phy->tpc_PDCCH_ConfigPUCCH.u.setup->
                    tpc_Index.u.indexOfFormat3A =
                    p_phy->tpc_pdcch_config_pucch.tpc_pdcch_config_param.
                    tpc_index.index_of_format3a;
            }
            else
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "Invalid tpc_pdcch_config_param.tpc_index.bitmask=%i",
                        p_phy->tpc_pdcch_config_pucch.tpc_pdcch_config_param.
                        tpc_index.bitmask);
            }
        }
        else
        {
            p_asn1_phy->tpc_PDCCH_ConfigPUCCH.t=T_TPC_PDCCH_Config_release;
        }
    }

    if (p_phy->bitmask & RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT)
    {
        p_asn1_phy->m.tpc_PDCCH_ConfigPUSCHPresent = 1;

        if (p_phy->tpc_pdcch_config_pusch.bitmask &
                RRC_PHY_TPC_PDCCH_CONFIG_PARAM_PRESENT)
        {
            /* TPC PDCCH Configuration PUSCH setup */
            p_asn1_phy->tpc_PDCCH_ConfigPUSCH.t = T_TPC_PDCCH_Config_setup;

            p_asn1_phy->tpc_PDCCH_ConfigPUSCH.u.setup =
                rtxMemAllocType(p_asn1_ctx, TPC_PDCCH_Config_setup);
            if (PNULL == p_asn1_phy->tpc_PDCCH_ConfigPUSCH.u.setup)
            {
                return RRC_FAILURE;
            }

            /* tpc_RNTI */
            RRC_ASSERT(sizeof(p_asn1_phy->tpc_PDCCH_ConfigPUSCH.u.
                        setup->tpc_RNTI.data) >=
                    sizeof(p_phy->tpc_pdcch_config_pusch.
                        tpc_pdcch_config_param.tpc_rnti));
            p_asn1_phy->tpc_PDCCH_ConfigPUSCH.u.setup->tpc_RNTI.numbits =
                sizeof(p_phy->tpc_pdcch_config_pusch.
                        tpc_pdcch_config_param.tpc_rnti)*8;
            l3_memcpy_wrapper(
                    p_asn1_phy->tpc_PDCCH_ConfigPUSCH.u.setup->tpc_RNTI.data,
                    p_phy->tpc_pdcch_config_pusch.
                    tpc_pdcch_config_param.tpc_rnti,
                    sizeof(p_phy->tpc_pdcch_config_pusch.
                        tpc_pdcch_config_param.tpc_rnti));

            /* TPC Index */
            if (p_phy->tpc_pdcch_config_pusch.tpc_pdcch_config_param.
                    tpc_index.bitmask & TPC_INDEX_FORMAT_3_PRESENT)
            {
                p_asn1_phy->tpc_PDCCH_ConfigPUSCH.u.setup->tpc_Index.t =
                    T_TPC_Index_indexOfFormat3;

                /* indexOfFormat3 */
                p_asn1_phy->tpc_PDCCH_ConfigPUSCH.u.setup->
                    tpc_Index.u.indexOfFormat3 =
                    p_phy->tpc_pdcch_config_pusch.tpc_pdcch_config_param.
                    tpc_index.index_of_format3;
            }
            else if (p_phy->tpc_pdcch_config_pusch.tpc_pdcch_config_param.
                    tpc_index.bitmask & TPC_INDEX_FORMAT_3A_PRESENT)
            {
                p_asn1_phy->tpc_PDCCH_ConfigPUSCH.u.setup->tpc_Index.t =
                    T_TPC_Index_indexOfFormat3A;

                /* indexOfFormat3A */
                p_asn1_phy->tpc_PDCCH_ConfigPUSCH.u.setup->
                    tpc_Index.u.indexOfFormat3A =
                    p_phy->tpc_pdcch_config_pusch.tpc_pdcch_config_param.
                    tpc_index.index_of_format3a;
            }
            else
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "Invalid tpc_pdcch_config_param.tpc_index.bitmask=%i",
                        p_phy->tpc_pdcch_config_pusch.tpc_pdcch_config_param.
                        tpc_index.bitmask);
            }
        }
        else
        {
            p_asn1_phy->tpc_PDCCH_ConfigPUSCH.t=T_TPC_PDCCH_Config_release;
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_phy_config_dedicated_cqi_reporting
*
*   DESCRIPTION: CQI reporting for ASN1 PhysicalConfigDedicated IE based on
*                default configuration or on configuration provided by RRM.
*
*   RETURNS:  RC_SUCCESS/RRC_FAILURE
******************************************************************************/
static rrc_return_et uecc_llim_build_asn1_phy_config_dedicated_cqi_reporting(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_phy_physical_config_dedicated_t const       *p_phy,
    PhysicalConfigDedicated                         *p_asn1_phy,
    OSCTXT                                          *p_asn1_ctx
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->facility_name,p_uecc_ue_context->p_gb_context->facility_name,
                  RRC_DETAILEDALL,"Unused variable"
                  "p_uecc_ue_context=%p",p_uecc_ue_context);/*SPR 17777 +-*/
    if (p_phy->bitmask & RRC_PHY_CQI_REPORTING_PRESENT)
    {
        p_asn1_phy->m.cqi_ReportConfigPresent = 1;

        if (p_phy->cqi_reporting.bitmask &
                RRC_PHY_CQI_REPORTING_MODE_APERIODIC_PRESENT)
        {
            p_asn1_phy->cqi_ReportConfig.m.cqi_ReportModeAperiodicPresent=1;

            /* cqi_ReportModeAperiodic */
            p_asn1_phy->cqi_ReportConfig.cqi_ReportModeAperiodic =
                p_phy->cqi_reporting.cqi_reporting_mode_aperiodic;
        }

        /* nomPDSCH_RS_EPRE_Offset */
        p_asn1_phy->cqi_ReportConfig.nomPDSCH_RS_EPRE_Offset =
            p_phy->cqi_reporting.nom_pdsch_rs_epre_offset;

        /* cqi_ReportingPeriodic */
        if (p_phy->cqi_reporting.bitmask &
                RRC_PHY_CQI_REPORTING_PERIODIC_PRESENT)
        {
            p_asn1_phy->cqi_ReportConfig.m.cqi_ReportPeriodicPresent = 1;

            if (p_phy->cqi_reporting.cqi_reporting_periodic.bitmask &
                    RRC_PHY_CQI_REPORTING_PERIODIC_PARAM_PRESENT)
            {
                p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.t =
                    T_CQI_ReportPeriodic_setup;

                p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.setup =
                    rtxMemAllocType(p_asn1_ctx, CQI_ReportPeriodic_setup);
                if (PNULL ==
                        p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.setup)
                {
                    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }
                asn1Init_CQI_ReportPeriodic_setup(
                        p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.setup);

                /* cqi_PUCCH_ResourceIndex */
                p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.setup->
                    cqi_PUCCH_ResourceIndex = p_phy->cqi_reporting.cqi_reporting_periodic.
                    cqi_reporting_periodic_param.cqi_pucch_resource_index;

                /* cqi_pmi_config_index*/
                p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.setup->
                    cqi_pmi_ConfigIndex =
                    p_phy->cqi_reporting.cqi_reporting_periodic.
                    cqi_reporting_periodic_param.cqi_pmi_config_index;

                /* CQI Format Indicator */
                if (p_phy->cqi_reporting.cqi_reporting_periodic.
                        cqi_reporting_periodic_param.cqi_format_indicator_periodic.
                        bitmask & RRC_PHY_SUBBAND_CQI_PARAM_PRESENT)
                {
                    /* subbandCQI Format Indicator */
                    p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.
                        setup->cqi_FormatIndicatorPeriodic.t =
                        T_CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_subbandCQI;

                    p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.
                        setup->cqi_FormatIndicatorPeriodic.u.subbandCQI =
                        rtxMemAllocType(p_asn1_ctx,
                                CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_subbandCQI);
                    if (PNULL == p_asn1_phy->cqi_ReportConfig.
                            cqi_ReportPeriodic.u.setup->
                            cqi_FormatIndicatorPeriodic.u.subbandCQI)
                    {
                        RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                        return RRC_FAILURE;
                    }

                    /* k */
                    p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.setup->
                        cqi_FormatIndicatorPeriodic.u.subbandCQI->k =
                        p_phy->cqi_reporting.cqi_reporting_periodic.
                        cqi_reporting_periodic_param.
                        cqi_format_indicator_periodic.subband_cqi_param.k;
                }
                else
                {
                    /* WidebandCQI Format Indicator */
                    p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.setup->
                        cqi_FormatIndicatorPeriodic.t =
                        T_CQI_ReportPeriodic_setup_cqi_FormatIndicatorPeriodic_widebandCQI;
                }

                /* ri_ConfigIndex */
                if (p_phy->cqi_reporting.cqi_reporting_periodic.
                        cqi_reporting_periodic_param.bitmask &
                        RRC_PHY_CQI_RI_CONFIG_INDEX_PRESENT)
                {
                    p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.
                        setup->m.ri_ConfigIndexPresent = 1;

                    p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.
                        setup->ri_ConfigIndex =
                        p_phy->cqi_reporting.cqi_reporting_periodic.
                        cqi_reporting_periodic_param.ri_config_index;
                }

                /* simultaneousAckNackAndCQI */
                p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.u.setup->
                    simultaneousAckNackAndCQI =
                    p_phy->cqi_reporting.cqi_reporting_periodic.
                    cqi_reporting_periodic_param.
                    simultaneous_ack_nack_and_cqi;
            }
            else
            {
                p_asn1_phy->cqi_ReportConfig.cqi_ReportPeriodic.t =
                    T_CQI_ReportPeriodic_release;
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_codebook_subset_restriction_type
*
*   DESCRIPTION: codebook_subset_restriction_type
*
*   RETURNS:   
*
******************************************************************************/
static void uecc_llim_build_asn1_codebook_subset_restriction_type(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_phy_physical_config_dedicated_t const       *p_phy,
    PhysicalConfigDedicated                         *p_asn1_phy,
    OSCTXT                                          *p_asn1_ctx
)
{
    switch(p_phy->
            antenna_information.antenna_information_explicit_value.
            codebook_subset_restriction.type)
    {
        case N2_TX_ANTENNA_TM3:
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.t =
                T_AntennaInfoDedicated_codebookSubsetRestriction_n2TxAntenna_tm3;
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n2TxAntenna_tm3 =
                rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);
            if (PNULL == p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n2TxAntenna_tm3)
            {
                break;
            }
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n2TxAntenna_tm3->
                numbits = 2;
            l3_memcpy_wrapper(p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n2TxAntenna_tm3->data,
                    p_phy->antenna_information.
                    antenna_information_explicit_value.
                    codebook_subset_restriction.value,
                    sizeof(p_asn1_phy->antennaInfo.u.explicitValue->
                        codebookSubsetRestriction.u.n2TxAntenna_tm3->data));
            break;
        case N4_TX_ANTENNA_TM3:
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.t =
                T_AntennaInfoDedicated_codebookSubsetRestriction_n4TxAntenna_tm3;
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n4TxAntenna_tm3 =
                rtxMemAllocType(p_asn1_ctx,
                        ASN1BitStr32);
            if (PNULL == p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n4TxAntenna_tm3)
            {
                break;
            }
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n4TxAntenna_tm3->
                numbits = 4;
            l3_memcpy_wrapper(p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n4TxAntenna_tm3->data,
                    p_phy->antenna_information.
                    antenna_information_explicit_value.
                    codebook_subset_restriction.value,
                    sizeof(p_asn1_phy->antennaInfo.u.explicitValue->
                        codebookSubsetRestriction.u.n4TxAntenna_tm3->data));
            break;
        case N2_TX_ANTENNA_TM4:
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.t =
                T_AntennaInfoDedicated_codebookSubsetRestriction_n2TxAntenna_tm4;
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n2TxAntenna_tm4 =
                rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);
            if (PNULL == p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n2TxAntenna_tm4)
            {
                break;
            }
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n2TxAntenna_tm4->
                numbits = 6;
            l3_memcpy_wrapper(p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n2TxAntenna_tm4->data,
                    p_phy->antenna_information.
                    antenna_information_explicit_value.
                    codebook_subset_restriction.value,
                    sizeof(p_asn1_phy->antennaInfo.u.explicitValue->
                        codebookSubsetRestriction.u.n2TxAntenna_tm4->data));
            break;
        case N4_TX_ANTENNA_TM4:
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.t =
                T_AntennaInfoDedicated_codebookSubsetRestriction_n4TxAntenna_tm4;
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n4TxAntenna_tm4 =
                rtxMemAllocType(p_asn1_ctx,
                        AntennaInfoDedicated_codebookSubsetRestriction_n4TxAntenna_tm4);
            if (PNULL == p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n4TxAntenna_tm4)
            {
                break;
            }
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n4TxAntenna_tm4->
                numbits = 64;
            l3_memcpy_wrapper(p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n4TxAntenna_tm4->data,
                    p_phy->antenna_information.
                    antenna_information_explicit_value.codebook_subset_restriction.value,
                    sizeof(p_asn1_phy->antennaInfo.u.explicitValue->
                        codebookSubsetRestriction.u.n4TxAntenna_tm4->data));
            break;
        case N2_TX_ANTENNA_TM5:
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.t =
                T_AntennaInfoDedicated_codebookSubsetRestriction_n2TxAntenna_tm5;
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n2TxAntenna_tm5 =
                rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);
            if (PNULL == p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n2TxAntenna_tm5)
            {
                break;
            }
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n2TxAntenna_tm5->
                numbits = 4;
            l3_memcpy_wrapper(p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n2TxAntenna_tm5->data,
                    p_phy->antenna_information.
                    antenna_information_explicit_value.
                    codebook_subset_restriction.value,
                    sizeof(p_asn1_phy->antennaInfo.u.explicitValue->
                        codebookSubsetRestriction.u.n2TxAntenna_tm5->data));
            break;
        case N4_TX_ANTENNA_TM5:
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.t =
                T_AntennaInfoDedicated_codebookSubsetRestriction_n4TxAntenna_tm5;
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n4TxAntenna_tm5 =
                rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);
            if (PNULL == p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n4TxAntenna_tm5)
            {
                break;
            }
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n4TxAntenna_tm5->
                numbits = 16;
            l3_memcpy_wrapper(p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n4TxAntenna_tm5->data,
                    p_phy->antenna_information.
                    antenna_information_explicit_value.
                    codebook_subset_restriction.value,
                    sizeof(p_asn1_phy->antennaInfo.u.explicitValue->
                        codebookSubsetRestriction.u.n4TxAntenna_tm5->data));
            break;
        case N2_TX_ANTENNA_TM6:
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.t =
                T_AntennaInfoDedicated_codebookSubsetRestriction_n2TxAntenna_tm6;
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n2TxAntenna_tm6 =
                rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);
            if (PNULL == p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n2TxAntenna_tm6)
            {
                break;
            }
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n2TxAntenna_tm6->
                numbits = 4;
            l3_memcpy_wrapper(p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n2TxAntenna_tm6->data,
                    p_phy->antenna_information.
                    antenna_information_explicit_value.
                    codebook_subset_restriction.value,
                    sizeof(p_asn1_phy->antennaInfo.u.explicitValue->
                        codebookSubsetRestriction.u.n2TxAntenna_tm6->data));
            break;
        case N4_TX_ANTENNA_TM6:
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.t =
                T_AntennaInfoDedicated_codebookSubsetRestriction_n4TxAntenna_tm6;
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n4TxAntenna_tm6 =
                rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);
            if (PNULL == p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n4TxAntenna_tm6)
            {
                break;
            }
            p_asn1_phy->antennaInfo.u.explicitValue->
                codebookSubsetRestriction.u.n4TxAntenna_tm6->
                numbits = 16;
            l3_memcpy_wrapper(p_asn1_phy->antennaInfo.u.explicitValue->
                    codebookSubsetRestriction.u.n4TxAntenna_tm6->data,
                    p_phy->antenna_information.
                    antenna_information_explicit_value.
                    codebook_subset_restriction.value,
                    sizeof(p_asn1_phy->antennaInfo.u.explicitValue->
                        codebookSubsetRestriction.u.n4TxAntenna_tm6->data));
            break;
        default:
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "Invalid codebook_subset_restriction.type=%i",
                    p_phy->antenna_information.
                    antenna_information_explicit_value.
                    codebook_subset_restriction.type);
            /* we can use ASN1OpenType *extElem1 */
    }
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_phy_config_dedicated_antenna_info
*
*   DESCRIPTION: Antenna information for ASN1 PhysicalConfigDedicated IE based on
*                default configuration or on configuration provided by RRM.
*
*   RETURNS:    RRC_FAILURE/RRC_SUCCESS 
*
******************************************************************************/
static rrc_return_et uecc_llim_build_asn1_phy_config_dedicated_antenna_info(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_phy_physical_config_dedicated_t const       *p_phy,
    PhysicalConfigDedicated                         *p_asn1_phy,
    OSCTXT                                          *p_asn1_ctx
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    if (p_phy->bitmask & RRC_PHY_ANTENNA_INFORMATION_PRESENT)
    {
        p_asn1_phy->m.antennaInfoPresent = 1;
        /* SPR 14984 Fix Start */
        if (p_phy->antenna_information.bitmask &
                RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT)
        {
            if(RRC_SUCCESS == uecc_validate_antenna_info (
                (rrc_phy_antenna_information_dedicated_t *)(&(p_phy->antenna_information.
                     antenna_information_explicit_value)), p_uecc_ue_context))
            {
                /* TM1-2 Fix Start */
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
                    "Validation Successful for TM1 and TM2");
            }
                /* TM1-2 Fix End */
            p_asn1_phy->antennaInfo.t =
                T_PhysicalConfigDedicated_antennaInfo_explicitValue;

            p_asn1_phy->antennaInfo.u.explicitValue =
                rtxMemAllocType(p_asn1_ctx, AntennaInfoDedicated);
            if (PNULL == p_asn1_phy->antennaInfo.u.explicitValue)
            {
                return RRC_FAILURE;
            }
            asn1Init_AntennaInfoDedicated(
                    p_asn1_phy->antennaInfo.u.explicitValue);

            /* transmissionMode */
            p_asn1_phy->antennaInfo.u.explicitValue->transmissionMode =
                p_phy->antenna_information.
                antenna_information_explicit_value.transmission_mode;

            /* Code book subset restriction */
            if (p_phy->antenna_information.
                    antenna_information_explicit_value.bitmask &
                    RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
            {
                p_asn1_phy->antennaInfo.u.explicitValue->m.
                    codebookSubsetRestrictionPresent = 1;

                uecc_llim_build_asn1_codebook_subset_restriction_type(
                        p_uecc_ue_context, p_phy, p_asn1_phy ,p_asn1_ctx);
            }

            /* UE Transmit Antenna Selection */
            if (p_phy->antenna_information.
                    antenna_information_explicit_value.
                    ue_transmit_antenna_selection.bitmask &
                    RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT)
            {
                p_asn1_phy->antennaInfo.u.explicitValue->
                    ue_TransmitAntennaSelection.t  =
                    T_AntennaInfoDedicated_ue_TransmitAntennaSelection_setup;

                /* enable */
                p_asn1_phy->antennaInfo.u.explicitValue->
                    ue_TransmitAntennaSelection.u.setup =
                    p_phy->antenna_information.
                    antenna_information_explicit_value.
                    ue_transmit_antenna_selection.
                    ue_transmit_antenna_selection_type;
            }
            else
            {
                p_asn1_phy->antennaInfo.u.explicitValue->
                    ue_TransmitAntennaSelection.t  =
                    T_AntennaInfoDedicated_ue_TransmitAntennaSelection_release;
            }
/* TM1-2 Fix Start */
        //}
/* TM1-2 Fix End */
        }
        /* SPR 14984 Fix Stop */
        else
        {
            p_asn1_phy->antennaInfo.t =
                T_PhysicalConfigDedicated_antennaInfo_defaultValue;
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_schd_req_config_and_cqi_report_v920
*
*   DESCRIPTION: Scheduling request configuration and CQI_REPORT_CONFIG_V920 
*		 for ASN1 PhysicalConfigDedicated IE based on
*                default configuration or on configuration provided by RRM.
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
static rrc_return_et uecc_llim_build_asn1_schd_req_config_and_cqi_report_v920(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_phy_physical_config_dedicated_t const       *p_phy,
    PhysicalConfigDedicated                         *p_asn1_phy,
    OSCTXT                                          *p_asn1_ctx
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->facility_name,p_uecc_ue_context->p_gb_context->facility_name,
                  RRC_DETAILEDALL,"Unused variable"
                  "p_uecc_ue_context=%p",p_uecc_ue_context);/*SPR 17777 +-*/
    if (p_phy->bitmask & RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT)
    {
        p_asn1_phy->m.schedulingRequestConfigPresent = 1;

        if (p_phy->scheduling_request_config.bitmask &
                RRC_PHY_SCHEDULING_REQUEST_CONFIG_PARAM_PRESENT)
        {
            p_asn1_phy->schedulingRequestConfig.t =
                T_SchedulingRequestConfig_setup;

            p_asn1_phy->schedulingRequestConfig.u.setup =
                rtxMemAllocType(p_asn1_ctx, SchedulingRequestConfig_setup);
            if (PNULL == p_asn1_phy->schedulingRequestConfig.u.setup)
            {
                return RRC_FAILURE;
            }

            /* sr_PUCCH_ResourceIndex */
            p_asn1_phy->schedulingRequestConfig.u.setup->
                sr_PUCCH_ResourceIndex =
                p_phy->scheduling_request_config.
                scheduling_request_config_param.sr_pucch_resource_index;

            /* sr_ConfigIndex */
            p_asn1_phy->schedulingRequestConfig.u.setup->sr_ConfigIndex =
                p_phy->scheduling_request_config.
                scheduling_request_config_param.sr_configuration_index;
            /* dsr_TransMax */
            p_asn1_phy->schedulingRequestConfig.u.setup->dsr_TransMax =
                p_phy->scheduling_request_config.
                scheduling_request_config_param.dsr_trans_max;
        }
        else
        {
            p_asn1_phy->schedulingRequestConfig.t =
                T_SchedulingRequestConfig_release;
        }
    }
    /* CR_316And247_START */
    /* CQI_REPORT_CONFIG_V920  */
    /* CqiReportConfig R8 is not sent if CqiReportConfigR10 is present*/
    if (!(p_phy->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT))
    {        
        if (p_phy->bitmask & RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT)
        {
            p_asn1_phy->m._v2ExtPresent = 1;
            p_asn1_phy->m.cqi_ReportConfig_v920Present = 1;

            if (p_phy->cqi_report_config_v920.bitmask &
                    RRC_PHY_CQI_MASK_R9_PRESENT)
            {
                p_asn1_phy->cqi_ReportConfig_v920.m.cqi_Mask_r9Present = 1;

                /* cqi_mask_r9 */
                p_asn1_phy->cqi_ReportConfig_v920.cqi_Mask_r9 =
                    p_phy->cqi_report_config_v920.cqi_mask_r9;
            }
            if (p_phy->cqi_report_config_v920.bitmask &
                    RRC_PHY_PMI_RI_REPORT_R9_PRESENT)
            {
                p_asn1_phy->cqi_ReportConfig_v920.m.pmi_RI_Report_r9Present = 1;

                /* pmi_ri_report_r9 */
                p_asn1_phy->cqi_ReportConfig_v920.pmi_RI_Report_r9 =
                    p_phy->cqi_report_config_v920.pmi_ri_report_r9;
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/******************************************************************************
 * FUNCTION NAME    : uecc_llim_build_asn1_antenna_info_dedicated_v920
 * DESCRIPTION      : This function builds AntennaInfoDedicated_v920
 * RETURNS          : None
 *******************************************************************************/
static void uecc_llim_build_asn1_antenna_info_dedicated_v920(
            uecc_ue_context_t                               *p_uecc_ue_context,
            rrc_phy_physical_config_dedicated_t const       *p_phy,
            PhysicalConfigDedicated                         *p_asn1_phy,
            OSCTXT                                          *p_asn1_ctx
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    if (p_phy->bitmask & RRC_PHY_ANTENNA_INFO_V920_PRESENT)
    {
        p_asn1_phy->m._v2ExtPresent = 1;
        p_asn1_phy->m.antennaInfo_v920Present = 1;
        asn1Init_AntennaInfoDedicated_v920(&p_asn1_phy->antennaInfo_v920);

        /* Code book subset restriction */
        if (p_phy->antenna_info_v920.bitmask &
                RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT)
        {
            p_asn1_phy->antennaInfo_v920.m.
                codebookSubsetRestriction_v920Present = 1;

            switch(p_phy->antenna_info_v920.codebook_subset_restriction_v920.type)
            {
                case N2_TX_ANTENNA_TM8_R9:
                    p_asn1_phy->antennaInfo_v920.codebookSubsetRestriction_v920.t =
                        T_AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_n2TxAntenna_tm8_r9;
                    p_asn1_phy->antennaInfo_v920.codebookSubsetRestriction_v920.
                        u.n2TxAntenna_tm8_r9 =
                        rtxMemAllocType(p_asn1_ctx,
                                ASN1BitStr32);
                    if (PNULL == p_asn1_phy->antennaInfo_v920.
                            codebookSubsetRestriction_v920.u.n2TxAntenna_tm8_r9)
                    {
                        break;
                    }
                    p_asn1_phy->antennaInfo_v920.codebookSubsetRestriction_v920.u.
                        n2TxAntenna_tm8_r9->numbits = 6;
                    l3_memcpy_wrapper(p_asn1_phy->antennaInfo_v920.codebookSubsetRestriction_v920.
                            u.n2TxAntenna_tm8_r9->data,
                            p_phy->antenna_info_v920.codebook_subset_restriction_v920.value,
                            sizeof(p_asn1_phy->antennaInfo_v920.
                                codebookSubsetRestriction_v920.u.n2TxAntenna_tm8_r9->data));
                    break;
                case N4_TX_ANTENNA_TM8_R9:
                    p_asn1_phy->antennaInfo_v920.codebookSubsetRestriction_v920.t =
                        T_AntennaInfoDedicated_v920_codebookSubsetRestriction_v920_n4TxAntenna_tm8_r9;
                    p_asn1_phy->antennaInfo_v920.codebookSubsetRestriction_v920.
                        u.n4TxAntenna_tm8_r9 =
                        rtxMemAllocType(p_asn1_ctx,
                                ASN1BitStr32);
                    if (PNULL == p_asn1_phy->antennaInfo_v920.
                            codebookSubsetRestriction_v920.u.n4TxAntenna_tm8_r9)
                    {
                        break;
                    }
                    p_asn1_phy->antennaInfo_v920.codebookSubsetRestriction_v920.u.
                        n2TxAntenna_tm8_r9->numbits = 32;    
                    l3_memcpy_wrapper(p_asn1_phy->antennaInfo_v920.codebookSubsetRestriction_v920.
                            u.n2TxAntenna_tm8_r9->data,   
                            p_phy->antenna_info_v920.codebook_subset_restriction_v920.value,
                            sizeof(p_asn1_phy->antennaInfo_v920.
                                codebookSubsetRestriction_v920.u.n4TxAntenna_tm8_r9->data));
                    break;

                default:
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                            "Invalid codebook_subset_restriction_v920.type=%i",
                            p_phy->antenna_info_v920.codebook_subset_restriction_v920.
                            type);
                    /* we can use ASN1OpenType *extElem1 */
            }
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
}

/******************************************************************************
 * FUNCTION NAME    :   uecc_llim_build_asn1_pucch_configuration
 * INPUT            :   uecc_ue_context_t                   *p_uecc_ue_context,
 *                      rrc_phy_physical_config_dedicated_t *p_phy,
 *                      PhysicalConfigDedicated             *p_asn1_phy,
 *                      OSCTXT                              *p_asn1_ctx
 * OUTPUT           :   None
 * DESCRIPTION      :   PUCCH Configuration 
 * RETURNS          :   RRC_FAILURE/RRC_SUCCESS
 *******************************************************************************/
static rrc_return_et uecc_llim_build_asn1_pucch_configuration(
            uecc_ue_context_t                               *p_uecc_ue_context,
            rrc_phy_physical_config_dedicated_t const       *p_phy,
            PhysicalConfigDedicated                         *p_asn1_phy,
            OSCTXT                                          *p_asn1_ctx
)
{
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->facility_name,p_uecc_ue_context->p_gb_context->facility_name,
                  RRC_DETAILEDALL,"Unused variable"
                  "p_uecc_ue_context=%p",p_uecc_ue_context);/*SPR 17777 +-*/
    /* PUCCH Configuration */
    if (p_phy->bitmask & RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT)
    {
        p_asn1_phy->m.pucch_ConfigDedicatedPresent = 1;

        /* ackNackRepetition */
        if (p_phy->pucch_configuration_dedicated.bitmask &
                RRC_PHY_ACK_NACK_REPETITION_PARAM_PRESENT)
        {
            p_asn1_phy->pucch_ConfigDedicated.ackNackRepetition.t =
                T_PUCCH_ConfigDedicated_ackNackRepetition_setup;

            p_asn1_phy->pucch_ConfigDedicated.ackNackRepetition.u.setup =
                rtxMemAllocType(p_asn1_ctx,
                        PUCCH_ConfigDedicated_ackNackRepetition_setup);
            if (PNULL == p_asn1_phy->pucch_ConfigDedicated.ackNackRepetition.u.setup)
            {
                return RRC_FAILURE;
            }

            /* repetitionFactor */
            p_asn1_phy->pucch_ConfigDedicated.ackNackRepetition.u.setup->
                repetitionFactor =
                p_phy->pucch_configuration_dedicated.
                ack_nack_repetition_param.factor;

            /* n1PUCCH_AN_Rep */
            p_asn1_phy->pucch_ConfigDedicated.ackNackRepetition.u.setup->
                n1PUCCH_AN_Rep =
                p_phy->pucch_configuration_dedicated.
                ack_nack_repetition_param.an_rep;
        }
        else
        {
            p_asn1_phy->pucch_ConfigDedicated.ackNackRepetition.t =
                T_PUCCH_ConfigDedicated_ackNackRepetition_release;
        }

        /* tdd_AckNackFeedbackMode */
        if (p_phy->pucch_configuration_dedicated.bitmask &
                RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT)
        {
            p_asn1_phy->pucch_ConfigDedicated.m.
                tdd_AckNackFeedbackModePresent = 1;

            /* tddAckNackFeedbackMode */
            p_asn1_phy->pucch_ConfigDedicated.tdd_AckNackFeedbackMode =
                p_phy->pucch_configuration_dedicated.
                tdd_ack_nack_feedback_mode;
        }
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_physical_config_dedicated
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                  rrc_phy_physical_config_dedicated_t const       *p_phy
*                  PhysicalConfigDedicated                         *p_asn1_phy
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION  : Builds ASN1 PhysicalConfigDedicated IE based on
*                  default configuration or on configuration provided by RRM.
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_physical_config_dedicated(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_phy_physical_config_dedicated_t const       *p_phy,
    PhysicalConfigDedicated                         *p_asn1_phy,
    OSCTXT                                          *p_asn1_ctx
)
{
    rrc_return_et                                 result = RRC_FAILURE;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    do
    {
        asn1Init_PhysicalConfigDedicated(p_asn1_phy);

        /* PDSCH Configuration */
        if (p_phy->bitmask & RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT)
        {
            p_asn1_phy->m.pdsch_ConfigDedicatedPresent = 1;

            /* P-a */
            p_asn1_phy->pdsch_ConfigDedicated.p_a =
                p_phy->pdsch_configuration_dedicated.p_a;
        }

        /* PUCCH Configuration */
    	result = uecc_llim_build_asn1_pucch_configuration(
                    p_uecc_ue_context,p_phy,p_asn1_phy,p_asn1_ctx);
        if(RRC_FAILURE==result)
        {
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        /* PUSCH Configuration */
        if (p_phy->bitmask & RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT)
        {
            p_asn1_phy->m.pusch_ConfigDedicatedPresent = 1;

            /* Delta Offset ACK index */
            p_asn1_phy->pusch_ConfigDedicated.betaOffset_ACK_Index =
                p_phy->pusch_configuration_dedicated.beta_offset_ack_index;

            /* Delta Offset RI index */
            p_asn1_phy->pusch_ConfigDedicated.betaOffset_RI_Index =
                p_phy->pusch_configuration_dedicated.beta_offset_ri_index;

            /* Delta Offset CQI index */
            p_asn1_phy->pusch_ConfigDedicated.betaOffset_CQI_Index =
                p_phy->pusch_configuration_dedicated.beta_offset_cqi_index;
        }

        /* Uplink Power Control */
        if (p_phy->bitmask & RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT)
        {
            p_asn1_phy->m.uplinkPowerControlDedicatedPresent = 1;

            /* p0_UePUSCH */
            p_asn1_phy->uplinkPowerControlDedicated.p0_UE_PUSCH =
                p_phy->uplink_power_control_dedicated.p0_ue_pusch;

            /* deltaMCS_Enabled */
            p_asn1_phy->uplinkPowerControlDedicated.deltaMCS_Enabled =
                p_phy->uplink_power_control_dedicated.delta_mcs_enabled;

            /* accumulationEnabled */
            p_asn1_phy->uplinkPowerControlDedicated.accumulationEnabled =
                p_phy->uplink_power_control_dedicated.accumulation_enabled;

            /* p0_uePUCCH */
            p_asn1_phy->uplinkPowerControlDedicated.p0_UE_PUCCH =
                p_phy->uplink_power_control_dedicated.p0_ue_pucch;

            /* pSRS_Offset */
            p_asn1_phy->uplinkPowerControlDedicated.pSRS_Offset =
                p_phy->uplink_power_control_dedicated.p_srs_offset;

            /* filterCoefficient */
            p_asn1_phy->uplinkPowerControlDedicated.filterCoefficient =
                p_phy->uplink_power_control_dedicated.filter_coefficient;
        }

        /* TPC PDCCH Configuration PUCCH and PDCCH Configuration PUSCH */
        result = uecc_llim_build_asn1_tpc_pucch_pusch_config(
                    p_uecc_ue_context,p_phy,p_asn1_phy,p_asn1_ctx);
        if(RRC_FAILURE==result)
        {
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

	/* Bug 4210 changes start */
	/* eICIC_Changes_Start */
	if (p_phy->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT)
        {
            if(RRC_SUCCESS != uecc_llim_build_asn1_cqi_report_config_r10(p_uecc_ue_context,
                    &p_phy->cqi_report_config_r10,
                    &p_asn1_phy->cqi_ReportConfig_r10,
                    p_asn1_ctx))
            {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"failed to build asn cqi"
                            "report config r10");
                    result = RRC_FAILURE;
                    break;
            }
            else
            {
                    p_asn1_phy->m.cqi_ReportConfig_r10Present = 1;
            }
	}
	else if (p_phy->bitmask & RRC_PHY_CQI_REPORTING_PRESENT)
        {
            result = uecc_llim_build_asn1_phy_config_dedicated_cqi_reporting(
                    p_uecc_ue_context,p_phy,p_asn1_phy,p_asn1_ctx); 
            if(RRC_FAILURE==result)
            {
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }
        }
	/* eICIC_Changes_End */
	/* Bug 4210 changes end */

        /* SoundingRsUl_ConfigDedicated */
        if (p_phy->bitmask & RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT)
        {
            p_asn1_phy->m.soundingRS_UL_ConfigDedicatedPresent = 1;

            if (p_phy->sounding_rs_ul_config_dedicated.bitmask &
                    RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PARAM_PRESENT)
            {
                p_asn1_phy->soundingRS_UL_ConfigDedicated.t =
                    T_SoundingRS_UL_ConfigDedicated_setup;

                p_asn1_phy->soundingRS_UL_ConfigDedicated.u.setup =
                    rtxMemAllocType(p_asn1_ctx,
                        SoundingRS_UL_ConfigDedicated_setup);
                if (PNULL == p_asn1_phy->soundingRS_UL_ConfigDedicated.u.setup)
                {
                    break;
                }

                /* srs_Bandwidth */
                p_asn1_phy->soundingRS_UL_ConfigDedicated.u.setup->
                    srs_Bandwidth =
                    p_phy->sounding_rs_ul_config_dedicated.
                        sounding_rs_ul_config_dedicated_param.srs_bandwidth;

                /* srs_HoppingBandwidth */
                p_asn1_phy->soundingRS_UL_ConfigDedicated.u.setup->
                    srs_HoppingBandwidth =
                    p_phy->sounding_rs_ul_config_dedicated.
                    sounding_rs_ul_config_dedicated_param.srs_hopping_bandwidth;

                /* freqDomainPosition */
                p_asn1_phy->soundingRS_UL_ConfigDedicated.u.setup->
                    freqDomainPosition =
                    p_phy->sounding_rs_ul_config_dedicated.
                    sounding_rs_ul_config_dedicated_param.
                    frequency_domain_position;

                /* duration */
                p_asn1_phy->soundingRS_UL_ConfigDedicated.u.setup->duration =
                    p_phy->sounding_rs_ul_config_dedicated.
                        sounding_rs_ul_config_dedicated_param.duration;

                /* srs_ConfigIndex */
                p_asn1_phy->soundingRS_UL_ConfigDedicated.u.setup->
                    srs_ConfigIndex =
                    p_phy->sounding_rs_ul_config_dedicated.
                    sounding_rs_ul_config_dedicated_param.
                    srs_configuration_index;

                /* transmissionComb */
                p_asn1_phy->soundingRS_UL_ConfigDedicated.u.setup->
                    transmissionComb =
                    p_phy->sounding_rs_ul_config_dedicated.
                    sounding_rs_ul_config_dedicated_param.transmission_comb;

                /* cyclicShift */
                p_asn1_phy->soundingRS_UL_ConfigDedicated.u.setup->cyclicShift =
                    p_phy->sounding_rs_ul_config_dedicated.
                        sounding_rs_ul_config_dedicated_param.cyclic_shift;
            }
            else
            {
                p_asn1_phy->soundingRS_UL_ConfigDedicated.t =
                    T_SoundingRS_UL_ConfigDedicated_release;
            }
        }

        /* Antenna Information */
        if (!(p_phy->bitmask & RRC_PHY_ANTENNA_INFO_R10_PRESENT))
        {
            result=uecc_llim_build_asn1_phy_config_dedicated_antenna_info(
                    p_uecc_ue_context,p_phy,p_asn1_phy,p_asn1_ctx);
            /*Coverity 85900 Fix Start*/
            if(RRC_FAILURE==result)
            {
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }
            /*Coverity 85900 Fix End*/
        }

        /* Scheduling Request Configuration */
        result = uecc_llim_build_asn1_schd_req_config_and_cqi_report_v920(
                p_uecc_ue_context,p_phy,p_asn1_phy,p_asn1_ctx);	
        if(RRC_FAILURE==result)
        {
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        if (!(p_phy->bitmask & RRC_PHY_ANTENNA_INFO_R10_PRESENT))
        {
            /* Antenna InfoDedicated V920  */
            uecc_llim_build_asn1_antenna_info_dedicated_v920(
                    p_uecc_ue_context,p_phy,p_asn1_phy,p_asn1_ctx);	
        }

        
        if(RRC_SUCCESS != uecc_llim_build_asn1_phy_ca_config_dedicated(p_uecc_ue_context,
                p_phy,p_asn1_phy,p_asn1_ctx))
        {     
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, 
                    RRC_WARNING,"failed to build asn phy"
                    "ca config dedicated");
            result = RRC_FAILURE;
            break;
        }

        /* physicalConfigDedicated.extElem1 */
        rtxDListInit(&p_asn1_phy->extElem1);

        result = RRC_SUCCESS;
    } while(0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

    return result;
}
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_sps_config
*   INPUT        : uecc_ue_context_t        *p_uecc_ue_context
*                  rrc_sps_config_t const   *p_sps 
*                  SPS_Config               *p_asn1_sps
*                  OSCTXT                   *p_asn1_ctx
*
*   DESCRIPTION:
*                Builds ASN1 SPS_Config IE based on
*                default configuration or on configuration provided by RRM.
*
*   RETURNS:     RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_sps_config(
    uecc_ue_context_t        *p_uecc_ue_context, 
    rrc_sps_config_t const   *p_sps,
    SPS_Config               *p_asn1_sps,
    OSCTXT                   *p_asn1_ctx
)
{
    rrc_return_et          result = RRC_FAILURE;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    do
    {
        asn1Init_SPS_Config(p_asn1_sps);

        /* CRNTI */
        if (p_sps->bitmask & RRC_SPS_CONFIG_C_RNTI_PRESENT)
        {
            p_asn1_sps->m.semiPersistSchedC_RNTIPresent = 1;

            RRC_ASSERT(sizeof(p_asn1_sps->semiPersistSchedC_RNTI.data) >=
                sizeof(p_sps->semi_presist_sched_c_rnti));
            p_asn1_sps->semiPersistSchedC_RNTI.numbits =
                sizeof(p_sps->semi_presist_sched_c_rnti)*8;
            l3_memcpy_wrapper(p_asn1_sps->semiPersistSchedC_RNTI.data,
                p_sps->semi_presist_sched_c_rnti,
                sizeof(p_sps->semi_presist_sched_c_rnti));
        }

        /* SPS DL CONFIGURATION */
        if (p_sps->bitmask & RRC_SPS_CONFIG_SPS_CONFIG_DL_PRESENT)
        {
            p_asn1_sps->m.sps_ConfigDLPresent = 1;

            if (p_sps->sps_config_dl.bitmask & RRC_SPS_CONFIG_DL_PARAM_PRESENT)
            {
                p_asn1_sps->sps_ConfigDL.t = T_SPS_ConfigDL_setup;

                p_asn1_sps->sps_ConfigDL.u.setup =
                    rtxMemAllocType(p_asn1_ctx, SPS_ConfigDL_setup);
                if (PNULL == p_asn1_sps->sps_ConfigDL.u.setup)
                {
                    break;
                }
                asn1Init_SPS_ConfigDL_setup(p_asn1_sps->sps_ConfigDL.u.setup);

                /* semiPersistSchedIntervalDL */
                p_asn1_sps->sps_ConfigDL.u.setup->semiPersistSchedIntervalDL =
                    p_sps->sps_config_dl.sps_config_dl_param.
                        semi_persist_sched_interval_dl;

                /* numberOfConfSPS_Processes */
                p_asn1_sps->sps_ConfigDL.u.setup->numberOfConfSPS_Processes =
                    p_sps->sps_config_dl.sps_config_dl_param.
                        number_of_conf_sps_processes;

                /* n1_PUCCH_AN_PersistentList */
                if ( (p_sps->sps_config_dl.sps_config_dl_param.
                        n1_pucch_an_persist_list.count >
                        ARRSIZE(p_asn1_sps->sps_ConfigDL.u.setup->
                            n1PUCCH_AN_PersistentList.elem)) ||
                      (p_sps->sps_config_dl.sps_config_dl_param.
                          n1_pucch_an_persist_list.count < 1)
                )
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
                            "Wrong n1_pucch_an_persist_list.count value: %i",
                            p_sps->sps_config_dl.sps_config_dl_param.
                            n1_pucch_an_persist_list.count);

                    break;
                }

                p_asn1_sps->sps_ConfigDL.u.setup->n1PUCCH_AN_PersistentList.n =
                    p_sps->sps_config_dl.sps_config_dl_param.
                        n1_pucch_an_persist_list.count;
                RRC_ASSERT(sizeof(p_asn1_sps->sps_ConfigDL.u.setup->
                        n1PUCCH_AN_PersistentList.elem)==
                    sizeof(p_sps->sps_config_dl.sps_config_dl_param.
                        n1_pucch_an_persist_list.n1_pucch_an_persist));
                l3_memcpy_wrapper(p_asn1_sps->sps_ConfigDL.u.setup->
                    n1PUCCH_AN_PersistentList.elem,
                    p_sps->sps_config_dl.sps_config_dl_param.
                        n1_pucch_an_persist_list.n1_pucch_an_persist,
                    sizeof(p_asn1_sps->sps_ConfigDL.u.setup->
                        n1PUCCH_AN_PersistentList.elem));

                /* extElem */
                rtxDListInit(&p_asn1_sps->sps_ConfigDL.u.setup->extElem1);
            }
            else
            {
                p_asn1_sps->sps_ConfigDL.t = T_SPS_ConfigDL_release;
            }
        }

        /* SPS UL Configuration */
        if (p_sps->bitmask & RRC_SPS_CONFIG_SPS_CONFIG_UL_PRESENT)
        {
            p_asn1_sps->m.sps_ConfigULPresent = 1;

            if (p_sps->sps_config_ul.bitmask & RRC_SPS_CONFIG_UL_PARAM_PRESENT)
            {
                p_asn1_sps->sps_ConfigUL.t = T_SPS_ConfigUL_setup;

                p_asn1_sps->sps_ConfigUL.u.setup =
                    rtxMemAllocType(p_asn1_ctx, SPS_ConfigUL_setup);
                if (PNULL == p_asn1_sps->sps_ConfigUL.u.setup)
                {
                    break;
                }
                asn1Init_SPS_ConfigUL_setup(p_asn1_sps->sps_ConfigUL.u.setup);

                /* semiPersistSchedIntervalUL */
                p_asn1_sps->sps_ConfigUL.u.setup->semiPersistSchedIntervalUL =
                    p_sps->sps_config_ul.sps_config_ul_param.
                        semi_persist_sched_interval_ul;

                /* IimplicitReleaseAfter */
                p_asn1_sps->sps_ConfigUL.u.setup->implicitReleaseAfter =
                    p_sps->sps_config_ul.sps_config_ul_param.
                        implicit_release_after;

                /* p0_Persistent */
                if (p_sps->sps_config_ul.sps_config_ul_param.bitmask &
                    RRC_SPS_CONFIG_UL_P_0_PERSISTENT_PRESENT)
                {
                    p_asn1_sps->sps_ConfigUL.u.setup->m.p0_PersistentPresent=1;

                    /* p0_NominalPUSCH_Persistent */
                    p_asn1_sps->sps_ConfigUL.u.setup->p0_Persistent.
                        p0_NominalPUSCH_Persistent =
                        p_sps->sps_config_ul.sps_config_ul_param.
                            p_zero_persistent.p_zero_nominal_pusch_persistent;

                    /* p_zero_ue_pusch_persistent */
                    p_asn1_sps->sps_ConfigUL.u.setup->p0_Persistent.
                        p0_UE_PUSCH_Persistent =
                        p_sps->sps_config_ul.sps_config_ul_param.
                            p_zero_persistent.p_zero_ue_pusch_persistent;
                }

                /* twoIntervalsConfig */
                if (p_sps->sps_config_ul.sps_config_ul_param.bitmask &
                    RRC_SPS_CONFIG_UL_TWO_INTERVALS_CONFIG_PRESENT)
                {
                    p_asn1_sps->sps_ConfigUL.u.setup->m.
                        twoIntervalsConfigPresent = 1;

                    /* twoIntervalsConfig */
                    /* Just bitmask, always 0,
                     * see SPS_ConfigUL_setup_twoIntervalsConfig_Root */
                    /* SPR:5994 start */
                    /* SPS_FIX start */
                    p_asn1_sps->sps_ConfigUL.u.setup->twoIntervalsConfig = RRC_NULL;
                    /* SPS_FIX stop */
                    /* SPR:5994 stop */
                }

                /* extElem */
                rtxDListInit(&p_asn1_sps->sps_ConfigUL.u.setup->extElem1);
            }
            else
            {
                p_asn1_sps->sps_ConfigUL.t = T_SPS_ConfigUL_release;
            }
        }

        result = RRC_SUCCESS;

    } while (0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_mac_main_config
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                  rrm_mac_config_t                                *p_mac_config
*                  RadioResourceConfigDedicated_mac_MainConfig     *p_asn1_mac
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION:   Builds ASN1 MAC-MainConfig IE based on
*                  default configuration or on configuration provided by RRM.
*
*   RETURNS:       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_mac_main_config(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrm_mac_config_t                                *p_mac_config,
    RadioResourceConfigDedicated_mac_MainConfig     *p_asn1_mac,
    OSCTXT                                          *p_asn1_ctx
)
{
	rrc_return_et                       result = RRC_FAILURE;
	rrc_mac_main_config_t const*        p_mac_main_config = PNULL;

	RRC_ASSERT( PNULL != p_mac_config );
	RRC_ASSERT( PNULL != p_asn1_mac );
	RRC_ASSERT( PNULL != p_asn1_ctx );

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do {
		p_mac_main_config = &p_mac_config->mac_main_config;

		p_asn1_mac->t =
			T_RadioResourceConfigDedicated_mac_MainConfig_explicitValue;

		p_asn1_mac->u.explicitValue =
			rtxMemAllocType(p_asn1_ctx, MAC_MainConfig);
		if (PNULL == p_asn1_mac->u.explicitValue)
		{
			break;
		}
		asn1Init_MAC_MainConfig(p_asn1_mac->u.explicitValue);

		/* ul-SCH-Config */
		if (p_mac_main_config->bitmask &
				RRC_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT)
		{
			p_asn1_mac->u.explicitValue->m.ul_SCH_ConfigPresent = 1;

			/* Max HARQ Transmissions */
			if (p_mac_main_config->ul_sch_config.bitmask &
					RRC_UL_SCH_CONFIG_MAX_HARQ_TX_PRESENT)
			{
				p_asn1_mac->u.explicitValue->ul_SCH_Config.m.
					maxHARQ_TxPresent = 1;

				p_asn1_mac->u.explicitValue->ul_SCH_Config.maxHARQ_Tx =
					p_mac_main_config->ul_sch_config.max_harq_tx;
			}

			/* periodicBSR_Timer */
			if (p_mac_main_config->ul_sch_config.bitmask &
					RRC_UL_SCH_CONFIG_PERIODIC_BSR_TIMER_PRESENT)
			{
				p_asn1_mac->u.explicitValue->ul_SCH_Config.m.
					periodicBSR_TimerPresent = 1;

				p_asn1_mac->u.explicitValue->ul_SCH_Config.periodicBSR_Timer =
					p_mac_main_config->ul_sch_config.periodic_bsr_timer;
			}

			/* retxBSR_Timer */
			p_asn1_mac->u.explicitValue->ul_SCH_Config.retxBSR_Timer =
				p_mac_main_config->ul_sch_config.retx_bsr_timer;

			/* ttiBundling */
			p_asn1_mac->u.explicitValue->ul_SCH_Config.ttiBundling =
				p_mac_main_config->ul_sch_config.tti_bundling;
		}

		/* DRX_Config */
		if (p_mac_main_config->bitmask & RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT)
		{
			p_asn1_mac->u.explicitValue->m.drx_ConfigPresent = 1;

			if (p_mac_main_config->drx_config.bitmask &
					RRC_DRX_CONFIG_PARAM_PRESENT)
			{
				p_asn1_mac->u.explicitValue->drx_Config.t = T_DRX_Config_setup;

				p_asn1_mac->u.explicitValue->drx_Config.u.setup =
					rtxMemAllocType(p_asn1_ctx, DRX_Config_setup);
				if (PNULL == p_asn1_mac->u.explicitValue->drx_Config.u.setup)
				{
					break;
				}

				asn1Init_DRX_Config_setup(p_asn1_mac->u.explicitValue->drx_Config.u.setup);
				/* onDurationTimer */
				p_asn1_mac->u.explicitValue->drx_Config.u.setup->
					onDurationTimer =
					p_mac_main_config->drx_config.drx_config_param.
					on_duration_timer;

				/* drx_InactivityTime */
				p_asn1_mac->u.explicitValue->drx_Config.u.setup->
					drx_InactivityTimer =
					p_mac_main_config->drx_config.drx_config_param.
					drx_inactivity_timer;

				/* drx_RetransmissionTimer */
				p_asn1_mac->u.explicitValue->drx_Config.u.setup->
					drx_RetransmissionTimer =
					p_mac_main_config->drx_config.drx_config_param.
					drx_retransmission_timer;

				/* LongDRX_CycleStartOffset */
				switch(p_mac_main_config->drx_config.drx_config_param.
						long_drx_cycle_start_offset.bitmask)
				{
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_10_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf10 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_10;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf10;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_20_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf20 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_20;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf20;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_32_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf32 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_32;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf32;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_40_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf40 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_40;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf40;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_64_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf64 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_64;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf64;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_80_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf80 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_80;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf80;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_128_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf128 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_128;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf128;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_160_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf160 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_160;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf160;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_256_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf256 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_256;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf256;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_320_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf320 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_320;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf320;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_512_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf512 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_512;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf512;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_640_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf640 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_640;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf640;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_1024_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf1024 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_1024;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf1024;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_1280_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf1280 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_1280;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf1280;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_2048_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf2048 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_2048;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf2048;
						break;
					case RRC_LONG_DRX_CYCLE_START_OFFSET_SF_2560_PRESENT:
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.u.sf2560 =
							p_mac_main_config->drx_config.drx_config_param.
							long_drx_cycle_start_offset.sf_2560;
						p_asn1_mac->u.explicitValue->drx_Config.u.setup->
							longDRX_CycleStartOffset.t =
							T_DRX_Config_setup_longDRX_CycleStartOffset_sf2560;
						break;
					default:
						RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
								p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
								"Wrong long_drx_cycle_start_offset.bitmask: %i",
								p_mac_main_config->drx_config.drx_config_param.
								long_drx_cycle_start_offset.bitmask);
				}

				/* short DRX */
				if (p_mac_main_config->drx_config.drx_config_param.bitmask &
						RRC_DRX_CONFIG_SHORT_DRX_PRESENT)
				{
					p_asn1_mac->u.explicitValue->drx_Config.u.setup->m.
						shortDRXPresent = 1;

					p_asn1_mac->u.explicitValue->drx_Config.u.setup->shortDRX.
						shortDRX_Cycle =
						p_mac_main_config->drx_config.drx_config_param.
						short_drx.short_drx_cycle;

					p_asn1_mac->u.explicitValue->drx_Config.u.setup->shortDRX.
						drxShortCycleTimer =
						p_mac_main_config->drx_config.drx_config_param.
						short_drx.short_drx_cycle_timer;
				}
			}
			else
			{
				p_asn1_mac->u.explicitValue->drx_Config.t=T_DRX_Config_release;
			}
		}

		/* timeAlignmentTimerDedicated */
		p_asn1_mac->u.explicitValue->timeAlignmentTimerDedicated =
			p_mac_main_config->time_alignment_timer_dedicated;

		/* MAC_MainConfig_phr_Config */
		if (p_mac_main_config->bitmask & RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT)
		{
			p_asn1_mac->u.explicitValue->m.phr_ConfigPresent = 1;

			if (p_mac_main_config->phr_config.bitmask &
					RRC_PHR_CONFIG_PARAM_PRESENT)
			{
				p_asn1_mac->u.explicitValue->phr_Config.t =
					T_MAC_MainConfig_phr_Config_setup;

				p_asn1_mac->u.explicitValue->phr_Config.u.setup =
					rtxMemAllocType(p_asn1_ctx, MAC_MainConfig_phr_Config_setup);
				if (PNULL == p_asn1_mac->u.explicitValue->phr_Config.u.setup)
				{
					break;
				}

				/* Periodic PHR Timer */
				p_asn1_mac->u.explicitValue->phr_Config.u.setup->
					periodicPHR_Timer =
					p_mac_main_config->phr_config.phr_config_param.
					periodic_phr_timer;

				/* Prohibit PHR Timer */
				p_asn1_mac->u.explicitValue->phr_Config.u.setup->
					prohibitPHR_Timer  =
					p_mac_main_config->phr_config.phr_config_param.
					prohibit_phr_timer;

				/* DL pathloss change */
				p_asn1_mac->u.explicitValue->phr_Config.u.setup->
					dl_PathlossChange =
					p_mac_main_config->phr_config.phr_config_param.
					dl_pathloss_change;
			}
			else
			{
				p_asn1_mac->u.explicitValue->phr_Config.t =
					T_MAC_MainConfig_phr_Config_release;
			}
		}

		/* sr_ProhibitTimer_r9 */
		if (p_mac_main_config->bitmask & RRC_MAC_MAIN_CONFIG_SR_PROHIBIT_TIMER_R9_PRESENT)
		{
			p_asn1_mac->u.explicitValue->m._v2ExtPresent = 1;
			p_asn1_mac->u.explicitValue->m.sr_ProhibitTimer_r9Present = 1;
			p_asn1_mac->u.explicitValue->sr_ProhibitTimer_r9 =
				p_mac_main_config->sr_prohibit_timer_r9;
		}    


		if(p_mac_main_config->bitmask & RRC_MAC_MAIN_CONFIG_V1020_PRESENT)
		{
			p_asn1_mac->u.explicitValue->m._v3ExtPresent = 1;

			p_asn1_mac->u.explicitValue->m.mac_MainConfig_v1020Present = 1;

			if(RRC_SUCCESS != uecc_llim_build_asn1_mac_main_config_v1020(p_uecc_ue_context,
						&p_mac_main_config->mac_main_config_v1020,
						&p_asn1_mac->u.explicitValue->mac_MainConfig_v1020))
                /*SPR 17777 +-*/
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,"failed to build asn mac_main_config_v1020");

				result = RRC_FAILURE;
				break;
			}

		}

		if(p_mac_main_config->bitmask & RRC_MAC_MAIN_CONFIG_V1020_PRESENT)
		{
			p_asn1_mac->u.explicitValue->m._v3ExtPresent = 1;
			p_asn1_mac->u.explicitValue->m.mac_MainConfig_v1020Present = 1;

			if(RRC_SUCCESS != uecc_llim_build_asn1_mac_main_config_v1020(p_uecc_ue_context,
						&p_mac_main_config->mac_main_config_v1020,
						&p_asn1_mac->u.explicitValue->mac_MainConfig_v1020))
                /*SPR 17777 +-*/
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,"failed to build asn mac_main_config_v1020");

				result = RRC_FAILURE;
				break;
			}

		}
		/*MAC_MainConfiguration.extElem*/
		rtxDListInit(&p_asn1_mac->u.explicitValue->extElem1);

		result = RRC_SUCCESS;

	} while (0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return result;
}
/* eICIC ue reconfig changes start */
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_meas_subframe_pattern_pcell_r10
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                  rrc_meas_subframe_pattern_pcell_r10_t           *p_meas_subframe
*                  MeasSubframePatternPCell_r10                    *p_asn1_meas_subframe
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION:   Builds ASN1 rrc_meas_subframe_pattern_pcell_r10 IE based on
*                  configuration provided by RRM.
*
*   RETURNS:       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_meas_subframe_pattern_pcell_r10(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_meas_subframe_pattern_pcell_r10_t           *p_meas_subframe,
    MeasSubframePatternPCell_r10                    *p_asn1_meas_subframe,
    OSCTXT                                          *p_asn1_ctx
)
{
	rrc_return_et                       result = RRC_FAILURE;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_MeasSubframePatternPCell_r10(p_asn1_meas_subframe);
		if(p_meas_subframe->bitmask & MEAS_SUBFRAME_PATTERN_PCELL_R10_SETUP_PRESENT)
		{
			p_asn1_meas_subframe->t = T_MeasSubframePatternPCell_r10_setup;

			p_asn1_meas_subframe->u.setup = rtxMemAllocType(p_asn1_ctx, MeasSubframePattern_r10);
			if (PNULL == p_asn1_meas_subframe->u.setup)
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name,
						RRC_WARNING,"Unable to allocate memory to MeasSubframePattern_r10_setup");
				break;
			}
			else
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name,
						RRC_INFO,"Allocated memory to MeasSubframePattern_r10_setup");
			}
			asn1Init_MeasSubframePattern_r10(p_asn1_meas_subframe->u.setup);

			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name,
					RRC_WARNING,"setup bitmask : %u",p_meas_subframe->meas_subframe_pattern_r10_setup.bitmask);


			if(p_meas_subframe->meas_subframe_pattern_r10_setup.bitmask & RRC_PHY_MEAS_SUBFRAME_PATTERN_FDD_R10)
			{
				p_asn1_meas_subframe->u.setup->t = T_MeasSubframePattern_r10_subframePatternFDD_r10;   

				p_asn1_meas_subframe->u.setup->u.subframePatternFDD_r10 = 
					rtxMemAllocType(p_asn1_ctx, MeasSubframePattern_r10_subframePatternFDD_r10);

				if(PNULL == p_asn1_meas_subframe->u.setup->u.subframePatternFDD_r10)
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name,
							RRC_WARNING,"Unable to allocate memory to subframePatternFDD_r10");
					break;
				}
				else
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name,
							RRC_INFO,"Allocated memory to subframePatternFDD_r10");
				}

				asn1Init_MeasSubframePattern_r10_subframePatternFDD_r10(p_asn1_meas_subframe->
						u.setup->u.subframePatternFDD_r10);

				/* subframePatternFDD_r10 is a bitstring of 40 bits */
				p_asn1_meas_subframe->u.setup->u.subframePatternFDD_r10->numbits = MAX_SUBFRAME_CONFIG_FDD_NUMBITS;

				l3_memcpy_wrapper(p_asn1_meas_subframe->u.setup->u.subframePatternFDD_r10->data,
						p_meas_subframe->meas_subframe_pattern_r10_setup.subframe_pattern_fdd_r10.data,
						MAX_SUBFRAME_PATTERN_FDD);

			}
			else if(p_meas_subframe->meas_subframe_pattern_r10_setup.bitmask & RRC_PHY_MEAS_SUBFRAME_PATTERN_TDD_R10)
			{
				p_asn1_meas_subframe->u.setup->t = T_MeasSubframePattern_r10_subframePatternTDD_r10;

				p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10 =
					rtxMemAllocType(p_asn1_ctx, MeasSubframePattern_r10_subframePatternTDD_r10);

				if(PNULL == p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10)
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name,
							RRC_WARNING,"Unable to allocate memory to subframePatternTDD_r10");
					break;
				}
				else
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name,
							RRC_INFO,"Allocated memory to subframePatternTDD_r10");
				}
				asn1Init_MeasSubframePattern_r10_subframePatternTDD_r10(p_asn1_meas_subframe->
						u.setup->u.subframePatternTDD_r10);

				if(p_meas_subframe->meas_subframe_pattern_r10_setup.subframe_pattern_tdd_r10.bitmask &
						RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG1_5_R10)
				{
					p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->t = 
						T_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig1_5_r10;

					p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->u.subframeConfig1_5_r10 =
						rtxMemAllocType(p_asn1_ctx, ASN1BitStr32);

					if(PNULL == p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->u.subframeConfig1_5_r10)
					{
						RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
								p_uecc_ue_context->p_gb_context->facility_name,
								RRC_WARNING,"Unable to allocate memory to subframeConfig1_5_r10");
						break;

					}
					else
					{
						RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
								p_uecc_ue_context->p_gb_context->facility_name,
								RRC_INFO,"Allocated memory to subframeConfig1_5_r10");
					}
					/* subframeConfig1_5_r10 is a bitstring of 20 bits */
					p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->
						u.subframeConfig1_5_r10->numbits = MAX_SUBFRAME_CONFIG1_5_TDD_NUMBITS;

					/* coverity_fix_69429 */
					l3_memcpy_wrapper(p_asn1_meas_subframe->u.setup->u.
							subframePatternTDD_r10->u.subframeConfig1_5_r10->data,
							p_meas_subframe->meas_subframe_pattern_r10_setup.subframe_pattern_tdd_r10.subframe_config1_5_r10,
							MAX_SUBFRAME_CONFIG1_5_TDD); 
				}
				else if(p_meas_subframe->meas_subframe_pattern_r10_setup.subframe_pattern_tdd_r10.bitmask &
						RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG0_R10)
				{
					p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->t = 
						T_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig0_r10;

					p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->u.subframeConfig0_r10 =
						rtxMemAllocType(p_asn1_ctx, MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig0_r10);

					if(PNULL == p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->u.subframeConfig0_r10)
					{
						RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
								p_uecc_ue_context->p_gb_context->facility_name,
								RRC_WARNING,"Unable to allocate memory to subframeConfig0_r10");
						break;

					}
					else
					{
						RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
								p_uecc_ue_context->p_gb_context->facility_name,
								RRC_INFO,"Allocated memory to subframeConfig0_r10");
					}
					asn1Init_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig0_r10(
							p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->u.subframeConfig0_r10);

					/* subframeConfig0_r10 is a bitstring of 70 bits */
					p_asn1_meas_subframe->u.setup->u.
						subframePatternTDD_r10->u.subframeConfig0_r10->numbits = MAX_SUBFRAME_CONFIG0_TDD_NUMBITS;
					l3_memcpy_wrapper(p_asn1_meas_subframe->u.setup->u.
							subframePatternTDD_r10->u.subframeConfig0_r10->data,
							p_meas_subframe->meas_subframe_pattern_r10_setup.subframe_pattern_tdd_r10.subframe_config0_r10,
							MAX_SUBFRAME_CONFIG0_TDD);
				}
				else if(p_meas_subframe->meas_subframe_pattern_r10_setup.subframe_pattern_tdd_r10.bitmask &
						RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG6_R10)
				{
					p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->t=
						T_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig6_r10;

					p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->u.subframeConfig6_r10 =
						rtxMemAllocType(p_asn1_ctx, MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig6_r10);

					if(PNULL == p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->u.subframeConfig6_r10)
					{
						RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
								p_uecc_ue_context->p_gb_context->facility_name,
								RRC_WARNING,"Unable to allocate memory to subframeConfig6_r10");
						break;
					}
					else
					{
						RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
								p_uecc_ue_context->p_gb_context->facility_name,
								RRC_INFO,"Allocated memory to ubframeConfig6_r10");
					}
					/* subframeConfig6_r10 is a bitstring of 60 bits */
					p_asn1_meas_subframe->u.setup->u.subframePatternTDD_r10->
						u.subframeConfig6_r10->numbits = MAX_SUBFRAME_CONFIG6_TDD_NUMBITS;

					l3_memcpy_wrapper(p_asn1_meas_subframe->u.
							setup->u.subframePatternTDD_r10->u.subframeConfig6_r10->data,
							p_meas_subframe->meas_subframe_pattern_r10_setup.subframe_pattern_tdd_r10.subframe_config6_r10,
							MAX_SUBFRAME_CONFIG6_TDD);
				}
			}
		}
		else if(p_meas_subframe->bitmask && MEAS_SUBFRAME_PATTERN_PCELL_R10_RELEASE_PRESENT)
		{
			p_asn1_meas_subframe->t = T_MeasSubframePatternPCell_r10_release;

			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, 
					RRC_WARNING,
					"MEAS_SUBFRAME_PATTERN_PCELL_R10_RELEASE_PRESENT supported");
		}

		result = RRC_SUCCESS;
	}while(0);
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}
/* eICIC ue reconfig changes stop */

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_srb_to_add_mod
*   INPUT        : uecc_ue_context_t               *p_uecc_ue_context
*                  rrm_srb_config_t const          *p_srb_config
*                  U8                              srb_identity
*                  SRB_ToAddMod                    *p_asn1_srb_elem
*                  OSCTXT                          *p_asn1_ctx
*   DESCRIPTION :  Builds ASN1 SRB-ToAddMod IE based on
*                  configuration provided by RRM.
*
*   RETURNS:       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_srb_to_add_mod(
/* CSR 00070192 Fix Start */
    uecc_ue_context_t               *p_uecc_ue_context,
/* CSR 00070192 Fix Stop */
    rrm_srb_config_t const          *p_srb_config,
    U8                              srb_identity,
    SRB_ToAddMod                    *p_asn1_srb_elem,
    OSCTXT                          *p_asn1_ctx
)
{
    rrc_return_et                       result = RRC_FAILURE;

    RRC_ASSERT( PNULL != p_srb_config );
    RRC_ASSERT( PNULL != p_asn1_srb_elem );
    RRC_ASSERT( PNULL != p_asn1_ctx );

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    do {
        asn1Init_SRB_ToAddMod(p_asn1_srb_elem);

        /* srb_Identity */
        p_asn1_srb_elem->srb_Identity = srb_identity;

        /* RLC Configuration */
        if (p_srb_config->bitmask & RRM_SRB_CONFIG_AM_CONFIG_PRESENT)
        {
            p_asn1_srb_elem->m.rlc_ConfigPresent = 1;

/* SPR 14984 Fix Start */
            if ( ((RRC_SRB1_LC_ID == srb_identity) &&
                 (p_uecc_ue_context->m.default_srb1_rlc_cfg_present)) ||
                ((RRC_SRB2_LC_ID == srb_identity) &&
                 (p_uecc_ue_context->m.default_srb2_rlc_cfg_present)) )
            {
                p_asn1_srb_elem->rlc_Config.t =
                   T_SRB_ToAddMod_rlc_Config_defaultValue;
            }
            else if (p_srb_config->srb_am_config.bitmask &
                RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT)
/* SPR 14984 Fix Stop */
            {
                p_asn1_srb_elem->rlc_Config.t =
                    T_SRB_ToAddMod_rlc_Config_explicitValue;

                p_asn1_srb_elem->rlc_Config.u.explicitValue =
                    rtxMemAllocType(p_asn1_ctx, RLC_Config);
                if (PNULL == p_asn1_srb_elem->rlc_Config.u.explicitValue)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                            "[SRB-ToAddMod] ASN malloc failed");
                    break;
                }

                /* AM mode */
                p_asn1_srb_elem->rlc_Config.u.explicitValue->t=T_RLC_Config_am;

                p_asn1_srb_elem->rlc_Config.u.explicitValue->u.am =
                    rtxMemAllocType(p_asn1_ctx, RLC_Config_am);
                if (PNULL == p_asn1_srb_elem->rlc_Config.u.explicitValue->u.am)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[SRB-ToAddMod] ASN malloc failed");
                    break;
                }

                /* UL */

                /* t_PollRetransmit */
                p_asn1_srb_elem->rlc_Config.u.explicitValue->u.am->ul_AM_RLC.
                    t_PollRetransmit =
                    p_srb_config->srb_am_config.am_config_explicit.ul_am_rlc.
                        t_poll_retransmit;

                /* pollPDU */
                p_asn1_srb_elem->rlc_Config.u.explicitValue->u.am->ul_AM_RLC.
                    pollPDU =
                    p_srb_config->srb_am_config.am_config_explicit.ul_am_rlc.
                        poll_pdu;

                /* pollByte */
                p_asn1_srb_elem->rlc_Config.u.explicitValue->u.am->ul_AM_RLC.
                    pollByte =
                    p_srb_config->srb_am_config.am_config_explicit.ul_am_rlc.
                        poll_byte;

                /* maxRetxThreshold */
                p_asn1_srb_elem->rlc_Config.u.explicitValue->u.am->ul_AM_RLC.
                    maxRetxThreshold =
                    p_srb_config->srb_am_config.am_config_explicit.ul_am_rlc.
                        max_retx_threshold;

                /* DL */

                /* t_Reordering */
                p_asn1_srb_elem->rlc_Config.u.explicitValue->u.am->dl_AM_RLC.
                    t_Reordering =
                    p_srb_config->srb_am_config.am_config_explicit.dl_am_rlc.
                        t_reordering;

                /* t_StatusProhibit */
                p_asn1_srb_elem->rlc_Config.u.explicitValue->u.am->dl_AM_RLC.
                    t_StatusProhibit =
                    p_srb_config->srb_am_config.am_config_explicit.dl_am_rlc.
                        t_status_prohibit;
            }
/* SPR 14984 Fix Start */
            /*else
            {
                p_asn1_srb_elem->rlc_Config.t =
                    T_SRB_ToAddMod_rlc_Config_defaultValue;
            }*/
/* SPR 14984 Fix Stop */
        }


        /* Logical Channel Configuration */
        if (RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT &
                p_srb_config->bitmask )
        {
            p_asn1_srb_elem->m.logicalChannelConfigPresent = 1;

/* SPR 14984 Fix Start */
            if ( ((RRC_SRB1_LC_ID == srb_identity) &&
                 (p_uecc_ue_context->m.default_srb1_lc_cfg_present)) ||
                ((RRC_SRB2_LC_ID == srb_identity) &&
                 (p_uecc_ue_context->m.default_srb2_lc_cfg_present)) )
            {
                p_asn1_srb_elem->logicalChannelConfig.t =
                    T_SRB_ToAddMod_logicalChannelConfig_defaultValue;
            }
            else if (p_srb_config->logical_channel_config.bitmask &
                RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT)
/* SPR 14984 Fix Stop */
            {
                p_asn1_srb_elem->logicalChannelConfig.t =
                    T_SRB_ToAddMod_logicalChannelConfig_explicitValue;

                p_asn1_srb_elem->logicalChannelConfig.u.explicitValue =
                    rtxMemAllocType(p_asn1_ctx, LogicalChannelConfig);
                if (PNULL ==
                    p_asn1_srb_elem->logicalChannelConfig.u.explicitValue)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "[SRB-ToAddMod] ASN malloc failed");
                    break;
                }
                asn1Init_LogicalChannelConfig(
                    p_asn1_srb_elem->logicalChannelConfig.u.explicitValue);

                p_asn1_srb_elem->logicalChannelConfig.u.explicitValue->m.
                    ul_SpecificParametersPresent = 1;

                /* priority */
                p_asn1_srb_elem->logicalChannelConfig.u.explicitValue->
                    ul_SpecificParameters.priority =
                    p_srb_config->logical_channel_config.
                        ul_specific_parameters_explicit.priority;

                /* prioritisedBitRate */
                p_asn1_srb_elem->logicalChannelConfig.u.explicitValue->
                    ul_SpecificParameters.prioritisedBitRate =
                    p_srb_config->logical_channel_config.
                        ul_specific_parameters_explicit.prioritized_bit_rate;

                /* bucketSizeDuration */
                p_asn1_srb_elem->logicalChannelConfig.u.explicitValue->
                    ul_SpecificParameters.bucketSizeDuration =
                    p_srb_config->logical_channel_config.
                        ul_specific_parameters_explicit.bucket_size_duration;

                if (RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT &
                    p_srb_config->logical_channel_config.
                        ul_specific_parameters_explicit.bitmask)
                {
                    p_asn1_srb_elem->logicalChannelConfig.u.explicitValue->
                        ul_SpecificParameters.m.logicalChannelGroupPresent = 1;

                    /* logicalChannelGroup */
                    p_asn1_srb_elem->logicalChannelConfig.u.explicitValue->
                        ul_SpecificParameters.logicalChannelGroup =
                        p_srb_config->logical_channel_config.
                          ul_specific_parameters_explicit.logical_channel_group;
                }
          
                if (RRM_UL_SPECIFIC_PARAMETERS_LC_SR_MASK_R9_PRESENT &
                     p_srb_config->logical_channel_config.bitmask)
                {
                     if (RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT &
                        p_srb_config->logical_channel_config.
                        ul_specific_parameters_explicit.bitmask)
                     {
                         /* SPR 4166 FIX START */
                         p_asn1_srb_elem->logicalChannelConfig.u.explicitValue->
                               m._v2ExtPresent = 1;
                         p_asn1_srb_elem->logicalChannelConfig.u.explicitValue->
                               m.logicalChannelSR_Mask_r9Present = 1;
                         /* SPR 4166 FIX END */

                         p_asn1_srb_elem->logicalChannelConfig.u.explicitValue->
                           logicalChannelSR_Mask_r9 = 
                         p_srb_config->logical_channel_config.lc_sr_mask_r9;
                     }
                     else
                     {
                         RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                 p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
                                 " [SRB Config] logical_channel_config."
                                 "ul_specific_parameters_explicit must be present");
                         break;
                     }  
                }
                /* LogicalChannelConfig.u.explicitValue extElem */
                rtxDListInit(&p_asn1_srb_elem->logicalChannelConfig.u.
                    explicitValue->extElem1);
            }
/* SPR 14984 Fix Start */
            /*else
            {
                p_asn1_srb_elem->logicalChannelConfig.t =
                    T_SRB_ToAddMod_logicalChannelConfig_defaultValue;
            }*/
/* SPR 14984 Fix Stop */
        }

        /* extElem */
        rtxDListInit(&p_asn1_srb_elem->extElem1);

        result = RRC_SUCCESS;

    } while (0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_srb_to_add_mod_list
*   INPUT        : uecc_ue_context_t           *p_uecc_ue_context
*                  SRB_ToAddModList            *p_asn1_list
*                  rrm_srb_info_t const        *p_srb_info
*                  OSCTXT                      *p_asn1_ctx
*   OUTPUT       : None
*   DESCRIPTION  : Builds ASN1 SRB_ToAddModList IE based on
*                  configuration provided by RRM.
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_srb_to_add_mod_list(
/* CSR 00070192 Fix Start */
    uecc_ue_context_t           *p_uecc_ue_context,
/* CSR 00070192 Fix Stop */
    SRB_ToAddModList            *p_asn1_list,
    rrm_srb_info_t const        *p_srb_info,
    OSCTXT                      *p_asn1_ctx
)
{
    SRB_ToAddMod                    *p_asn1_srb_elem = PNULL;
    OSRTDListNode                   *p_node = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    rtxDListInit(p_asn1_list);

    if ( 0 == (p_srb_info->bitmask &
        (RRM_SRB_INFO_SRB1_CONFIG_PRESENT|RRM_SRB_INFO_SRB2_CONFIG_PRESENT) )
    )
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                p_uecc_ue_context->p_gb_context->facility_name, 
                RRC_ERROR,"no any srb, bitmask=%i",p_srb_info->bitmask);
        return RRC_FAILURE;
    }

    if (p_srb_info->bitmask & RRM_SRB_INFO_SRB1_CONFIG_PRESENT)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, SRB_ToAddMod, &p_node,
            &p_asn1_srb_elem);
        if (PNULL == p_node)
        {
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            /* Not enough memory */
            return RRC_FAILURE;
        }

        if ( RRC_SUCCESS != uecc_llim_build_asn1_srb_to_add_mod(
                                /* CSR 00070192 Fix Start */
                                p_uecc_ue_context,
                                /* CSR 00070192 Fix Stop */
                                &p_srb_info->srb1_config,
                                RRC_SRB1_LC_ID,
                                p_asn1_srb_elem,
                                p_asn1_ctx) )
        {
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        /* append node to the list */
        rtxDListAppendNode(p_asn1_list, p_node);
    }

    if (p_srb_info->bitmask & RRM_SRB_INFO_SRB2_CONFIG_PRESENT)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, SRB_ToAddMod, &p_node,
            &p_asn1_srb_elem);
        if (PNULL == p_node)
        {
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            /* Not enough memory */
            return RRC_FAILURE;
        }

        if ( RRC_SUCCESS != uecc_llim_build_asn1_srb_to_add_mod(
                                /* CSR 00070192 Fix Start */
                                p_uecc_ue_context, 
                                /* CSR 00070192 Fix Stop */
                                &p_srb_info->srb2_config,
                                RRC_SRB2_LC_ID,
                                p_asn1_srb_elem,
                                p_asn1_ctx) )
        {
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return RRC_FAILURE;
        }

        /* append node to the list */
        rtxDListAppendNode(p_asn1_list, p_node);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

    return RRC_SUCCESS;
}

/*SPR:7369 start */
/******************************************************************************
*   FUNCTION NAME : uecc_llim_build_asn1_drb_to_add_mod_new
*
*   INPUT         : uecc_ue_context_t *p_uecc_ue_context,
*                   rrm_drb_config_t  *p_drb,
*                   DRB_ToAddMod      *p_asn1_drb_elem,
*                   OSCTXT            *p_asn1_ctx
*   
*   OUTPUT        : None
*
*   DESCRIPTION   : This function fills 'DRB_ToAddMod' from information
*                   of 'rrm_drb_config_t'
*
*   RETURNS       : RRC_SUCCESS/RRC_FAILURE
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_drb_to_add_mod_new(
    uecc_ue_context_t*      p_uecc_ue_context,
    rrm_drb_config_t const  *p_drb,
    DRB_ToAddMod            *p_asn1_drb_elem,
    OSCTXT                  *p_asn1_ctx
)
{
    PDCP_Config* p_pdcp_conf = PNULL;
    RLC_Config* p_rlc_conf = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    asn1Init_DRB_ToAddMod(p_asn1_drb_elem);

    if(p_uecc_ue_context->erb_modify_running == RRC_FALSE)
    {
        if (p_drb->bitmask & RRM_DRB_CONFIG_BEARED_ID_PRESENT)
        {
            p_asn1_drb_elem->m.eps_BearerIdentityPresent = 1;

            /* eps_BearerIdentity */
            p_asn1_drb_elem->eps_BearerIdentity = p_drb->erab_id;
        }
    }

    /* drb_Identity */
    p_asn1_drb_elem->drb_Identity = p_drb->drb_id;

    if(p_uecc_ue_context->erb_modify_running == RRC_FALSE)
    {
        /* PDCP config */
        if (p_drb->bitmask & RRM_DRB_CONFIG_PDCP_CONFIG_PRESENT)
        {
            p_asn1_drb_elem->m.pdcp_ConfigPresent = 1;

            p_pdcp_conf = &p_asn1_drb_elem->pdcp_Config;

            if (p_drb->pdcp_config.bitmask & RRC_PDCP_CONFIG_DISCARD_TIMER_PRESENT)
            {
                p_pdcp_conf->m.discardTimerPresent = 1;

                /* discardTimer */
                p_pdcp_conf->discardTimer = p_drb->pdcp_config.discard_timer;
            }

            /* rlc_AM */
            if (p_drb->pdcp_config.bitmask &
                    RRC_PDCP_CONFIG_RLC_AM_STATUS_REPORT_REQ_PRESENT)
            {
                p_pdcp_conf->m.rlc_AMPresent = 1;

                /* statusReportRequired */
                p_pdcp_conf->rlc_AM.statusReportRequired =
                    p_drb->pdcp_config.rlc_am_status_report_required;
            }

            if (p_drb->pdcp_config.bitmask &
                    RRC_PDCP_CONFIG_RLC_UM_PDCP_SN_SIZE_PRESENT)
            {
                p_pdcp_conf->m.rlc_UMPresent = 1;

                /* pdcp_SN_Size */
                p_pdcp_conf->rlc_UM.pdcp_SN_Size =
                    p_drb->pdcp_config.rlc_um_pdcp_sn_size;
            }

            if (p_drb->pdcp_config.header_compression.bitmask &
                    RRC_PDCP_HEADER_COMPRESSION_ROHC_CONFIG_PRESENT)
            {
                /* ROHC enabled */
                p_pdcp_conf->headerCompression.t =
                    T_PDCP_Config_headerCompression_rohc;

                p_pdcp_conf->headerCompression.u.rohc =
                    rtxMemAllocType(p_asn1_ctx, PDCP_Config_headerCompression_rohc);
                if (PNULL == p_asn1_drb_elem->pdcp_Config.headerCompression.u.rohc)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name,
                            RRC_ERROR,
                            "generate_drb_to_add_mod: ASN malloc failed");
                    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                    return RRC_FAILURE;
                }

                /* maxCID */
                p_pdcp_conf->headerCompression.u.rohc->maxCID =
                    p_drb->pdcp_config.header_compression.rohc_config.max_cid;

                /* profiles */
                p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0001 =
                    p_drb->pdcp_config.header_compression.rohc_config.
                    rohc_profile.profile0x0001;
                p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0002 =
                    p_drb->pdcp_config.header_compression.rohc_config.
                    rohc_profile.profile0x0002;
                p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0003 =
                    p_drb->pdcp_config.header_compression.rohc_config.
                    rohc_profile.profile0x0003;
                p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0004 =
                    p_drb->pdcp_config.header_compression.rohc_config.
                    rohc_profile.profile0x0004;
                p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0006 =
                    p_drb->pdcp_config.header_compression.rohc_config.
                    rohc_profile.profile0x0006;
                p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0101 =
                    p_drb->pdcp_config.header_compression.rohc_config.
                    rohc_profile.profile0x0101;
                p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0102 =
                    p_drb->pdcp_config.header_compression.rohc_config.
                    rohc_profile.profile0x0102;
                p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0103 =
                    p_drb->pdcp_config.header_compression.rohc_config.
                    rohc_profile.profile0x0103;
                p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0104 =
                    p_drb->pdcp_config.header_compression.rohc_config.
                    rohc_profile.profile0x0104;

                /* extElem1 */
                rtxDListInit(&p_pdcp_conf->headerCompression.u.rohc->extElem1);
            }
            else
            {
                /* ROHC disabled */
                p_asn1_drb_elem->pdcp_Config.headerCompression.t =
                    T_PDCP_Config_headerCompression_notUsed;
            }

            /* extElem1 */
            rtxDListInit(&p_pdcp_conf->extElem1);
        }
    }

    if (p_drb->bitmask & RRM_DRB_CONFIG_RLC_CONFIG_UE_PRESENT)
    {
        p_asn1_drb_elem->m.rlc_ConfigPresent = 1;

        p_rlc_conf = &p_asn1_drb_elem->rlc_Config;

        switch(p_drb->rlc_config_ue.bitmask)
        {
        case RRC_RLC_CONFIG_AM_CONFIG_PRESENT:
            /* AM Mode RLC*/

            p_rlc_conf->t = T_RLC_Config_am;

            p_rlc_conf->u.am = rtxMemAllocType(p_asn1_ctx, RLC_Config_am);
            if (PNULL == p_rlc_conf->u.am)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_ERROR,
                    "generate_drb_to_add_mod: ASN malloc failed");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            /*UL*/

            /* t_PollRetransmit */
            p_rlc_conf->u.am->ul_AM_RLC.t_PollRetransmit =
                p_drb->rlc_config_ue.am_config.ul_am_rlc.t_poll_retransmit;

            /* pollPDU */
            p_rlc_conf->u.am->ul_AM_RLC.pollPDU =
                p_drb->rlc_config_ue.am_config.ul_am_rlc.poll_pdu;

            /* pollByte */
            p_rlc_conf->u.am->ul_AM_RLC.pollByte =
                p_drb->rlc_config_ue.am_config.ul_am_rlc.poll_byte;

            /* maxRetxThreshold */
            p_rlc_conf->u.am->ul_AM_RLC.maxRetxThreshold =
                p_drb->rlc_config_ue.am_config.ul_am_rlc.max_retx_threshold;


            /* DL */

            /* t_Reordering */
            p_rlc_conf->u.am->dl_AM_RLC.t_Reordering =
                p_drb->rlc_config_ue.am_config.dl_am_rlc.t_reordering;

            /* t_StatusProhibit */
            p_rlc_conf->u.am->dl_AM_RLC.t_StatusProhibit =
                p_drb->rlc_config_ue.am_config.dl_am_rlc.t_status_prohibit;

            break;

        case RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT:
            /* Bi-Directional */

            p_rlc_conf->t = T_RLC_Config_um_Bi_Directional;

            p_rlc_conf->u.um_Bi_Directional =
                rtxMemAllocType(p_asn1_ctx, RLC_Config_um_Bi_Directional);
            if (PNULL == p_rlc_conf->u.um_Bi_Directional)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_ERROR,
                    "generate_drb_to_add_mod: ASN malloc failed");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            /* UL */

            /* sn_FieldLength */
            p_rlc_conf->u.um_Bi_Directional->ul_UM_RLC.sn_FieldLength =
                p_drb->rlc_config_ue.um_bi_directional_config.ul_um_rlc.
                    sn_field_length;

            /* DL */

            /* sn_FieldLength */
            p_rlc_conf->u.um_Bi_Directional->dl_UM_RLC.sn_FieldLength =
                p_drb->rlc_config_ue.um_bi_directional_config.dl_um_rlc.
                    sn_field_length;

            /* t_Reordering */
            p_rlc_conf->u.um_Bi_Directional->dl_UM_RLC.t_Reordering =
                p_drb->rlc_config_ue.um_bi_directional_config.dl_um_rlc.
                    t_reordering;

            break;

        case RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT:
            /* Uni Directional Upload */

            p_rlc_conf->t = T_RLC_Config_um_Uni_Directional_UL;

            p_rlc_conf->u.um_Uni_Directional_UL =
                rtxMemAllocType(p_asn1_ctx, RLC_Config_um_Uni_Directional_UL);
            if (PNULL == p_rlc_conf->u.um_Uni_Directional_UL)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_ERROR,
                    "generate_drb_to_add_mod: ASN malloc failed");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            /* sn_FieldLength */
            p_rlc_conf->u.um_Uni_Directional_UL->ul_UM_RLC.sn_FieldLength =
                p_drb->rlc_config_ue.um_uni_directional_ul_config.ul_um_rlc.
                    sn_field_length;

            break;

        case RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT:
            /* Uni Directional Download */

            p_rlc_conf->t = T_RLC_Config_um_Uni_Directional_DL;

            p_rlc_conf->u.um_Uni_Directional_DL =
                rtxMemAllocType(p_asn1_ctx, RLC_Config_um_Uni_Directional_DL);
            if (PNULL == p_rlc_conf->u.um_Uni_Directional_DL)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name,
                    RRC_ERROR,
                    "generate_drb_to_add_mod: ASN malloc failed");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            /* sn_FieldLength */
            p_rlc_conf->u.um_Uni_Directional_DL->dl_UM_RLC.sn_FieldLength =
                p_drb->rlc_config_ue.um_uni_directional_dl_config.dl_um_rlc.
                    sn_field_length;

            /* t_Reordering */
            p_rlc_conf->u.um_Uni_Directional_DL->dl_UM_RLC.t_Reordering =
                p_drb->rlc_config_ue.um_uni_directional_dl_config.dl_um_rlc.
                    t_reordering;

            break;

        default:
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                 p_uecc_ue_context->p_gb_context->facility_name,
                 RRC_ERROR,
                "generate_drb_to_add_mod: wrong bitmask=%i",
                p_drb->rlc_config_ue.bitmask);
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return RRC_FAILURE;
        }
    }
    if(p_uecc_ue_context->erb_modify_running == RRC_FALSE)
    {

        if (p_drb->bitmask & RRM_DRB_CONFIG_LOGICAL_CHANNEL_IDENTITY_PRESENT)
        {
            p_asn1_drb_elem->m.logicalChannelIdentityPresent = 1;

            /* logicalChannelIdentity */
            p_asn1_drb_elem->logicalChannelIdentity =
                p_drb->logical_channel_identity;
        }

        if (p_drb->bitmask & RRM_DRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT)
        {
            p_asn1_drb_elem->m.logicalChannelConfigPresent = 1;

            if(p_drb->logical_channel_config.bitmask &
                    RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT)
            {
                RRC_TRACE(RRC_INFO, "UL specific LC Config Explicit values PRESENT");

                p_asn1_drb_elem->logicalChannelConfig.m.ul_SpecificParametersPresent=1;

                if (p_drb->logical_channel_config.ul_specific_parameters_explicit.
                        bitmask & RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT)
                {
                    p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.m.
                        logicalChannelGroupPresent = 1;

                    /* logicalChannelGroup */
                    p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.
                        logicalChannelGroup =
                        p_drb->logical_channel_config.ul_specific_parameters_explicit.
                        logical_channel_group;
                }

                /* priority */
                p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.priority =
                    p_drb->logical_channel_config.ul_specific_parameters_explicit.priority;

                /* prioritisedBitRate */
                p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.
                    prioritisedBitRate =
                    p_drb->logical_channel_config.ul_specific_parameters_explicit.
                    prioritized_bit_rate;

                /* bucketSizeDuration */
                p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.
                    bucketSizeDuration =
                    p_drb->logical_channel_config.ul_specific_parameters_explicit.
                    bucket_size_duration;
            }

            /* lc_sr_mask_r9 */
            if (p_drb->logical_channel_config.bitmask &
                    RRM_UL_SPECIFIC_PARAMETERS_LC_SR_MASK_R9_PRESENT)
            {
                if (p_drb->logical_channel_config.ul_specific_parameters_explicit.
                        bitmask & RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT)
                {
                    p_asn1_drb_elem->logicalChannelConfig.m.
                        _v2ExtPresent = 1;
                    p_asn1_drb_elem->logicalChannelConfig.m.
                        logicalChannelSR_Mask_r9Present = 1;
                    p_asn1_drb_elem->logicalChannelConfig.logicalChannelSR_Mask_r9 =
                        p_drb->logical_channel_config.lc_sr_mask_r9;
                }
                else
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name,
                            RRC_ERROR,
                            "[DRB Config] logicalChannelConfig."
                            "u.explicitValue->m.ul_SpecificParametersPresent"
                            "must be present");     
                }    
            }
            /* extElem1 */
            rtxDListInit(&p_asn1_drb_elem->logicalChannelConfig.extElem1);
        }
    }


    /* extElem */
    rtxDListInit(&p_asn1_drb_elem->extElem1);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

    return RRC_SUCCESS;
}
/*SPR:7369 stop */
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_drb_to_add_mod
*   INPUT        : uecc_ue_context_t           *p_uecc_ue_context
*                  SRB_ToAddModList            *p_asn1_list
*                  rrm_srb_info_t const        *p_srb_info
*                  OSCTXT                      *p_asn1_ctx
*   OUTPUT       : None
*
*   DESCRIPTION  : This function fills 'DRB_ToAddMod' from information
*                  of 'rrm_drb_config_t'
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_drb_to_add_mod(
    uecc_ue_context_t       *p_uecc_ue_context, 
    rrm_drb_config_t const  *p_drb,
    DRB_ToAddMod            *p_asn1_drb_elem,
    OSCTXT                  *p_asn1_ctx
)
{
    PDCP_Config* p_pdcp_conf = PNULL;
    RLC_Config* p_rlc_conf = PNULL;
#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
    U8  erab_idx = RRC_ZERO;
/* OPTION3X Changes End */
#endif

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    asn1Init_DRB_ToAddMod(p_asn1_drb_elem);

#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
    if ( p_uecc_ue_context->p_curr_proc_data
        && (UECC_RRM_DC_BEARER_CHANGE_PROC ==
            p_uecc_ue_context->p_curr_proc_data->t) )
    {
        for(erab_idx = RRC_ZERO;
                erab_idx < p_uecc_ue_context->p_curr_proc_data->u.
                rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_count;
                erab_idx++)
        {
            if((p_drb->bitmask & RRM_DRB_CONFIG_BEARED_ID_PRESENT)
                    &&
                    (p_drb->erab_id == p_uecc_ue_context->p_curr_proc_data->u.
                     rrm_dc_bearer_change_request_data.drb_to_modify_list.
                     drb_config[erab_idx].erab_id)
              )
            {
                break;
            }
        }
    }
/* OPTION3X Changes End */
#endif
    if (p_drb->bitmask & RRM_DRB_CONFIG_BEARED_ID_PRESENT)
    {
        p_asn1_drb_elem->m.eps_BearerIdentityPresent = 1;

        /* eps_BearerIdentity */
        p_asn1_drb_elem->eps_BearerIdentity = p_drb->erab_id;
    }

    /* drb_Identity */
#ifdef ENDC_ENABLED
    if ( p_uecc_ue_context->p_curr_proc_data  //add for bugid 117724
        && (UECC_RRM_DC_BEARER_CHANGE_PROC ==
            p_uecc_ue_context->p_curr_proc_data->t) )
    {
        /*
         *      * Fill new DRB ID received from RRM in DC bearer change req
         *           * from RRM
         *                */
        p_asn1_drb_elem->drb_Identity =
            p_uecc_ue_context->p_curr_proc_data->u.
            rrm_dc_bearer_change_request_data.drb_to_modify_list.
            drb_config[erab_idx].new_drb_id;
    }
    else
#endif
    {
        p_asn1_drb_elem->drb_Identity = p_drb->drb_id;
    }

#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
    if ( !p_uecc_ue_context->p_curr_proc_data
        || (UECC_RRM_DC_BEARER_CHANGE_PROC != p_uecc_ue_context->p_curr_proc_data->t)
            ||
            !(RRM_DRB_CONFIG_DC_BEARER_TYPE_PRESENT & p_drb->bitmask
                &&
                SN_TERMINATED_SPLIT_BEARER == p_drb->dc_bearer_type)
       )
    {
/* OPTION3X Changes End */
#endif
    /* PDCP config */
    if (p_drb->bitmask & RRM_DRB_CONFIG_PDCP_CONFIG_PRESENT)
    {
        p_asn1_drb_elem->m.pdcp_ConfigPresent = 1;

        p_pdcp_conf = &p_asn1_drb_elem->pdcp_Config;

        if (p_drb->pdcp_config.bitmask & RRC_PDCP_CONFIG_DISCARD_TIMER_PRESENT)
        {
            p_pdcp_conf->m.discardTimerPresent = 1;

            /* discardTimer */
            p_pdcp_conf->discardTimer = p_drb->pdcp_config.discard_timer;
        }

        /* rlc_AM */
        if (p_drb->pdcp_config.bitmask &
            RRC_PDCP_CONFIG_RLC_AM_STATUS_REPORT_REQ_PRESENT)
        {
            p_pdcp_conf->m.rlc_AMPresent = 1;

            /* statusReportRequired */
            p_pdcp_conf->rlc_AM.statusReportRequired =
                p_drb->pdcp_config.rlc_am_status_report_required;
        }

        if (p_drb->pdcp_config.bitmask &
            RRC_PDCP_CONFIG_RLC_UM_PDCP_SN_SIZE_PRESENT)
        {
            p_pdcp_conf->m.rlc_UMPresent = 1;

            /* pdcp_SN_Size */
            p_pdcp_conf->rlc_UM.pdcp_SN_Size =
                p_drb->pdcp_config.rlc_um_pdcp_sn_size;
        }

        if (p_drb->pdcp_config.header_compression.bitmask &
            RRC_PDCP_HEADER_COMPRESSION_ROHC_CONFIG_PRESENT)
        {
            /* ROHC enabled */
            p_pdcp_conf->headerCompression.t =
                T_PDCP_Config_headerCompression_rohc;

            p_pdcp_conf->headerCompression.u.rohc =
                rtxMemAllocType(p_asn1_ctx, PDCP_Config_headerCompression_rohc);
            if (PNULL == p_asn1_drb_elem->pdcp_Config.headerCompression.u.rohc)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                        p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                        "generate_drb_to_add_mod: ASN malloc failed");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            /* maxCID */
            p_pdcp_conf->headerCompression.u.rohc->maxCID =
                p_drb->pdcp_config.header_compression.rohc_config.max_cid;

            /* profiles */
            p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0001 =
                p_drb->pdcp_config.header_compression.rohc_config.
                rohc_profile.profile0x0001;
            p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0002 =
                p_drb->pdcp_config.header_compression.rohc_config.
                rohc_profile.profile0x0002;
            p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0003 =
                p_drb->pdcp_config.header_compression.rohc_config.
                rohc_profile.profile0x0003;
            p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0004 =
                p_drb->pdcp_config.header_compression.rohc_config.
                rohc_profile.profile0x0004;
            p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0006 =
                p_drb->pdcp_config.header_compression.rohc_config.
                rohc_profile.profile0x0006;
            p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0101 =
                p_drb->pdcp_config.header_compression.rohc_config.
                rohc_profile.profile0x0101;
            p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0102 =
                p_drb->pdcp_config.header_compression.rohc_config.
                rohc_profile.profile0x0102;
            p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0103 =
                p_drb->pdcp_config.header_compression.rohc_config.
                rohc_profile.profile0x0103;
            p_pdcp_conf->headerCompression.u.rohc->profiles.profile0x0104 =
                p_drb->pdcp_config.header_compression.rohc_config.
                rohc_profile.profile0x0104;

            /* extElem1 */
            rtxDListInit(&p_pdcp_conf->headerCompression.u.rohc->extElem1);
        }
        else
        {
            /* ROHC disabled */
            p_asn1_drb_elem->pdcp_Config.headerCompression.t =
                T_PDCP_Config_headerCompression_notUsed;
        }

        /* extElem1 */
        rtxDListInit(&p_pdcp_conf->extElem1);
    }
#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
    }
/* OPTION3X Changes End */
#endif

    /*As per Jira MENB-49 , it was decided that rlc-config will not be sent in the 2nd rrcConnectionReconfiguration 
     * as NI Ue was not expectibg this message. Since this is only logical channel identity update, MeNB will
     * share the rlcRestablish flag with updated lcid. Modre details in Jira - MENB-49*/
    if (p_drb->bitmask & RRM_DRB_CONFIG_RLC_CONFIG_UE_PRESENT)
    {
        p_asn1_drb_elem->m.rlc_ConfigPresent = 1;

        p_rlc_conf = &p_asn1_drb_elem->rlc_Config;

        switch(p_drb->rlc_config_ue.bitmask)
        {
        case RRC_RLC_CONFIG_AM_CONFIG_PRESENT:
            /* AM Mode RLC*/

            p_rlc_conf->t = T_RLC_Config_am;

            p_rlc_conf->u.am = rtxMemAllocType(p_asn1_ctx, RLC_Config_am);
            if (PNULL == p_rlc_conf->u.am)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,RRC_ERROR,
                        "generate_drb_to_add_mod: ASN malloc failed");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            /*UL*/

            /* t_PollRetransmit */
            p_rlc_conf->u.am->ul_AM_RLC.t_PollRetransmit =
                p_drb->rlc_config_ue.am_config.ul_am_rlc.t_poll_retransmit;

            /* pollPDU */
            p_rlc_conf->u.am->ul_AM_RLC.pollPDU =
                p_drb->rlc_config_ue.am_config.ul_am_rlc.poll_pdu;

            /* pollByte */
            p_rlc_conf->u.am->ul_AM_RLC.pollByte =
                p_drb->rlc_config_ue.am_config.ul_am_rlc.poll_byte;

            /* maxRetxThreshold */
            p_rlc_conf->u.am->ul_AM_RLC.maxRetxThreshold =
                p_drb->rlc_config_ue.am_config.ul_am_rlc.max_retx_threshold;


            /* DL */

            /* t_Reordering */
            p_rlc_conf->u.am->dl_AM_RLC.t_Reordering =
                p_drb->rlc_config_ue.am_config.dl_am_rlc.t_reordering;

            /* t_StatusProhibit */
            p_rlc_conf->u.am->dl_AM_RLC.t_StatusProhibit =
                p_drb->rlc_config_ue.am_config.dl_am_rlc.t_status_prohibit;

            break;

        case RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT:
            /* Bi-Directional */

            p_rlc_conf->t = T_RLC_Config_um_Bi_Directional;

            p_rlc_conf->u.um_Bi_Directional =
                rtxMemAllocType(p_asn1_ctx, RLC_Config_um_Bi_Directional);
            if (PNULL == p_rlc_conf->u.um_Bi_Directional)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "generate_drb_to_add_mod: ASN malloc failed");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            /* UL */

            /* sn_FieldLength */
            p_rlc_conf->u.um_Bi_Directional->ul_UM_RLC.sn_FieldLength =
                p_drb->rlc_config_ue.um_bi_directional_config.ul_um_rlc.
                    sn_field_length;

            /* DL */

            /* sn_FieldLength */
            p_rlc_conf->u.um_Bi_Directional->dl_UM_RLC.sn_FieldLength =
                p_drb->rlc_config_ue.um_bi_directional_config.dl_um_rlc.
                    sn_field_length;

            /* t_Reordering */
            p_rlc_conf->u.um_Bi_Directional->dl_UM_RLC.t_Reordering =
                p_drb->rlc_config_ue.um_bi_directional_config.dl_um_rlc.
                    t_reordering;

            break;

        case RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT:
            /* Uni Directional Upload */

            p_rlc_conf->t = T_RLC_Config_um_Uni_Directional_UL;

            p_rlc_conf->u.um_Uni_Directional_UL =
                rtxMemAllocType(p_asn1_ctx, RLC_Config_um_Uni_Directional_UL);
            if (PNULL == p_rlc_conf->u.um_Uni_Directional_UL)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "generate_drb_to_add_mod: ASN malloc failed");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            /* sn_FieldLength */
            p_rlc_conf->u.um_Uni_Directional_UL->ul_UM_RLC.sn_FieldLength =
                p_drb->rlc_config_ue.um_uni_directional_ul_config.ul_um_rlc.
                    sn_field_length;

            break;

        case RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT:
            /* Uni Directional Download */

            p_rlc_conf->t = T_RLC_Config_um_Uni_Directional_DL;

            p_rlc_conf->u.um_Uni_Directional_DL =
                rtxMemAllocType(p_asn1_ctx, RLC_Config_um_Uni_Directional_DL);
            if (PNULL == p_rlc_conf->u.um_Uni_Directional_DL)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "generate_drb_to_add_mod: ASN malloc failed");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                return RRC_FAILURE;
            }

            /* sn_FieldLength */
            p_rlc_conf->u.um_Uni_Directional_DL->dl_UM_RLC.sn_FieldLength =
                p_drb->rlc_config_ue.um_uni_directional_dl_config.dl_um_rlc.
                    sn_field_length;

            /* t_Reordering */
            p_rlc_conf->u.um_Uni_Directional_DL->dl_UM_RLC.t_Reordering =
                p_drb->rlc_config_ue.um_uni_directional_dl_config.dl_um_rlc.
                    t_reordering;

            break;

        default:
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                "generate_drb_to_add_mod: wrong bitmask=%i",
                p_drb->rlc_config_ue.bitmask);
            RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
            return RRC_FAILURE;
        }
    }

    if (p_drb->bitmask & RRM_DRB_CONFIG_LOGICAL_CHANNEL_IDENTITY_PRESENT)
    {
        p_asn1_drb_elem->m.logicalChannelIdentityPresent = 1;

        /* logicalChannelIdentity */
        #ifdef ENDC_ENABLED
        if ( p_uecc_ue_context->p_curr_proc_data
            && (UECC_RRM_DC_BEARER_CHANGE_PROC == p_uecc_ue_context->p_curr_proc_data->t) )
        {
            /*
             * Fill new LC ID received from RRM in DC bearer change req
             * from RRM
             */
            p_asn1_drb_elem->logicalChannelIdentity =
                p_uecc_ue_context->p_curr_proc_data->u.
                rrm_dc_bearer_change_request_data.drb_to_modify_list.
                drb_config[erab_idx].new_lc_id;

            /*
             * Fill reestablishRLC_r15
             */
            p_asn1_drb_elem->m._v5ExtPresent = RRC_ONE;
            p_asn1_drb_elem->m.rlc_Config_v1510Present = RRC_ONE;
            p_asn1_drb_elem->rlc_Config_v1510.reestablishRLC_r15 = true__113;
        }
        else
        {
            p_asn1_drb_elem->logicalChannelIdentity =
                p_drb->logical_channel_identity;
        }
        #else
        p_asn1_drb_elem->logicalChannelIdentity =
            p_drb->logical_channel_identity;
        #endif
    }

    if (p_drb->bitmask & RRM_DRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT)
    {
        p_asn1_drb_elem->m.logicalChannelConfigPresent = 1;

        p_asn1_drb_elem->logicalChannelConfig.m.ul_SpecificParametersPresent=1;

        if (p_drb->logical_channel_config.ul_specific_parameters_explicit.
        bitmask & RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT)
        {
            p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.m.
                logicalChannelGroupPresent = 1;

            /* logicalChannelGroup */
            p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.
                logicalChannelGroup =
                p_drb->logical_channel_config.ul_specific_parameters_explicit.
                logical_channel_group;
        }

        /* priority */
        p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.priority =
            p_drb->logical_channel_config.ul_specific_parameters_explicit.priority;

        /* prioritisedBitRate */
        p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.
            prioritisedBitRate =
            p_drb->logical_channel_config.ul_specific_parameters_explicit.
            prioritized_bit_rate;

        /* bucketSizeDuration */
        p_asn1_drb_elem->logicalChannelConfig.ul_SpecificParameters.
            bucketSizeDuration =
            p_drb->logical_channel_config.ul_specific_parameters_explicit.
            bucket_size_duration;
       
       /* lc_sr_mask_r9 */
       if (p_drb->logical_channel_config.bitmask &
       RRM_UL_SPECIFIC_PARAMETERS_LC_SR_MASK_R9_PRESENT)
       {
          if (p_drb->logical_channel_config.ul_specific_parameters_explicit.
          bitmask & RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT)
          {
             p_asn1_drb_elem->logicalChannelConfig.m.
                 _v2ExtPresent = 1;
             p_asn1_drb_elem->logicalChannelConfig.m.
                logicalChannelSR_Mask_r9Present = 1;
             p_asn1_drb_elem->logicalChannelConfig.logicalChannelSR_Mask_r9 =
                 p_drb->logical_channel_config.lc_sr_mask_r9;
         }
         else
         {
             RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                     p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
               "[DRB Config] logicalChannelConfig."
               "u.explicitValue->m.ul_SpecificParametersPresent"
               "must be present");     
         }    
      }
        /* extElem1 */
        rtxDListInit(&p_asn1_drb_elem->logicalChannelConfig.extElem1);
    }

    /* extElem */
    rtxDListInit(&p_asn1_drb_elem->extElem1);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

    return RRC_SUCCESS;
}

/*SPR 22036 Fix Start*/
/******************************************************************************
 *   FUNCTION NAME : uecc_llim_build_asn1_modify_drb_to_release_list
 *   INPUT         : uecc_ue_context_t*      p_uecc_ue_context
 *                   DRB_ToAddModList*       p_asn1_list
 *                   OSCTXT*                 p_asn1_ctx
 *   OUTPUT        :  none
 *
 *   DESCRIPTION   : This function fills 'DRB_ToReleaseList' from information
 *                   of 'erab_curr_proc_data_t'
 *
 *   RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_modify_drb_to_release_list(
    uecc_ue_context_t*   p_uecc_ue_context,
    DRB_ToReleaseList*   p_asn1_list,
    OSCTXT*              p_asn1_ctx
)
{
    erab_list_t*            p_erab_list = PNULL;
    U8                      drb_idx = RRC_NULL;
    rrc_counter_t           drb_count = RRC_NULL;
    
    RRC_ASSERT( PNULL != p_uecc_ue_context );
    RRC_ASSERT( PNULL != p_asn1_list );
    RRC_ASSERT( PNULL != p_asn1_ctx );

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    asn1Init_DRB_ToReleaseList(p_asn1_list);
    
    /* point to erab_release_list in erab_release_command_data */
    p_erab_list = &p_uecc_ue_context->p_curr_proc_data->u.erab_modify_request_data.
                      erab_release_command_data.erab_release_list;

    drb_count = p_erab_list->counter;

    if (drb_count > MAX_ERAB_COUNT)
    {
        RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                (p_uecc_ue_context->p_gb_context)->facility_name,
                RRC_ERROR,
            "invalid drb_count=%i",
            drb_count);
        return RRC_FAILURE;
    }
    
    p_asn1_list->n = RRC_NULL;

    for (drb_idx = 0; drb_idx < drb_count && 
                      drb_idx < RRC_MAX_NUM_DRB; drb_idx++)
    {
        if (PNULL != p_uecc_ue_context->p_e_rab_list
                    [p_erab_list->erab_item[drb_idx].e_RAB_ID])
        {
            p_asn1_list->elem[p_asn1_list->n] = p_uecc_ue_context->p_e_rab_list
               [p_erab_list->erab_item[drb_idx].e_RAB_ID]->drb_config.drb_id;
            p_asn1_list->n++;
        }
        else 
        {
            RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                    (p_uecc_ue_context->p_gb_context)->facility_name,
                    RRC_ERROR,
                "p_e_rab_list[erab_id] =="
                "NULL;Unused variable p_asn1_ctx=%p",p_asn1_ctx);
            return RRC_FAILURE;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}

/*SPR 22036 Fix Stop*/

/* ARP CHANGES START */

/******************************************************************************
 *   FUNCTION NAME : uecc_llim_build_asn1_setup_drb_to_release_list
 *   INPUT         : uecc_ue_context_t*      p_uecc_ue_context
 *                   DRB_ToAddModList*       p_asn1_list
 *                   OSCTXT*                 p_asn1_ctx
 *   OUTPUT        :  none
 *
 *   DESCRIPTION   : This function fills 'DRB_ToReleaseList' from information
 *                   of 'erab_curr_proc_data_t'
 *
 *   RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_setup_drb_to_release_list(
    uecc_ue_context_t*   p_uecc_ue_context,
    DRB_ToReleaseList*   p_asn1_list,
    OSCTXT*              p_asn1_ctx
)
{
    erab_list_t*            p_erab_list = PNULL;
    U8                      drb_idx = RRC_NULL;
    rrc_counter_t           drb_count = RRC_NULL;
/* OPTION3X Changes Start */
    uecc_drb_ctx_t          *p_erab = PNULL;
/* OPTION3X Changes End */
    
    RRC_ASSERT( PNULL != p_uecc_ue_context );
    RRC_ASSERT( PNULL != p_asn1_list );
    RRC_ASSERT( PNULL != p_asn1_ctx );

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    asn1Init_DRB_ToReleaseList(p_asn1_list);
#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
    if( UECC_RRM_DC_BEARER_CHANGE_PROC !=
            p_uecc_ue_context->p_curr_proc_data->t)
    {
/* OPTION3X Changes End */
#endif
    /* point to erab_release_list in erab_release_command_data */
    p_erab_list = &p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                      erab_release_command_data.erab_release_list;

    drb_count = p_erab_list->counter;
#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
    }
    else
    {
        drb_count = p_uecc_ue_context->p_curr_proc_data->u.
            rrm_dc_bearer_change_request_data.drb_to_modify_list.
            drb_count;
    }
/* OPTION3X Changes End */
#endif


    if (drb_count > MAX_ERAB_COUNT)
    {
        RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                (p_uecc_ue_context->p_gb_context)->facility_name,
                RRC_ERROR,
            "generate_drb_release_info_list: invalid drb_count=%i",
            drb_count);
        return RRC_FAILURE;
    }
    
    p_asn1_list->n = RRC_NULL;

    for (drb_idx = 0; drb_idx < drb_count && 
                      drb_idx < RRC_MAX_NUM_DRB; drb_idx++)
    {
#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
        if(UECC_RRM_DC_BEARER_CHANGE_PROC !=
                 p_uecc_ue_context->p_curr_proc_data->t)
        {
/* OPTION3X Changes End */
#endif
        p_erab = p_uecc_ue_context->p_e_rab_list
            [p_erab_list->erab_item[drb_idx].e_RAB_ID];
#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
        }
        else
        {
        	/*Logical_Error*/
        	if(p_uecc_ue_context->p_curr_proc_data->u.rrm_dc_bearer_change_request_data.drb_to_modify_list.drb_config[drb_idx].erab_success)
        	{
            p_erab = p_uecc_ue_context->p_e_rab_list
                [p_uecc_ue_context->p_curr_proc_data->u.
                rrm_dc_bearer_change_request_data.drb_to_modify_list.
                    drb_config[drb_idx].erab_id];
        }
            else
            {
            	continue;
            }
        	/*Logical_Error*/
        }
/* OPTION3X Changes End */
#endif
/* OPTION3X Changes Start */
        if ( PNULL != p_erab)
        {
            p_asn1_list->elem[p_asn1_list->n] = p_erab->drb_config.drb_id;
/* OPTION3X Changes End */
            p_asn1_list->n++;
        }
        else 
        {
            RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                    (p_uecc_ue_context->p_gb_context)->facility_name,
                    RRC_ERROR,
                "generate_drb_release_info_list: p_e_rab_list[erab_id] =="
                "NULL;Unused variable p_asn1_ctx=%p",p_asn1_ctx);/*SPR 17777 +-*/
            return RRC_FAILURE;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}
/* ARP CHANGES END */
/* ERB RELEASE COMMAND START */

/******************************************************************************
 *   FUNCTION NAME : uecc_llim_build_asn1_drb_to_release_list
 *   INPUT         : uecc_ue_context_t*      p_uecc_ue_context
 *                   DRB_ToAddModList*       p_asn1_list
 *                   OSCTXT*                 p_asn1_ctx
 *   OUTPUT        : none
 *
 *   DESCRIPTION   : This function fills 'DRB_ToReleaseList' from information
 *                   of 'erab_curr_proc_data_t'
 *
 *   RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_drb_to_release_list(
    uecc_ue_context_t*   p_uecc_ue_context,
    DRB_ToReleaseList*   p_asn1_list,
    OSCTXT*              p_asn1_ctx
)
{
    erab_list_t*            p_erab_list = PNULL;
    U8                      drb_idx = RRC_NULL;
    rrc_counter_t           drb_count = RRC_NULL;
    
    RRC_ASSERT( PNULL != p_uecc_ue_context );
    RRC_ASSERT( PNULL != p_asn1_list );
    RRC_ASSERT( PNULL != p_asn1_ctx );

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    asn1Init_DRB_ToReleaseList(p_asn1_list);
    
    /* point to erab_release_list in erab_release_command_data */
    p_erab_list = &p_uecc_ue_context->p_curr_proc_data->u.
                      erab_release_command_data.erab_release_list;

    drb_count = p_erab_list->counter;

    if (drb_count > MAX_ERAB_COUNT)
    {
        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
            "generate_drb_release_info_list: invalid drb_count=%i",
            drb_count);
        return RRC_FAILURE;
    }
    
    p_asn1_list->n = RRC_NULL;

    for (drb_idx = 0; drb_idx < drb_count && 
                      drb_idx < RRC_MAX_NUM_DRB; drb_idx++)
    {
        if (PNULL != p_uecc_ue_context->p_e_rab_list
                    [p_erab_list->erab_item[drb_idx].e_RAB_ID])
        {
#ifdef ENDC_ENABLED
/* NR_DC Code Change Start */
            /* If DC Bearer Type is present in the RAB Ctxt and
             * DC_Bearer_type is set to SN_TERMINATED_SCG_BEARER,
             * Don't inclued that ERAB in DRB_ToReleaseModList.
             */
            if(RRC_ONE == p_uecc_ue_context->p_e_rab_list
                    [p_erab_list->erab_item[drb_idx].e_RAB_ID]->m.DC_BearerTypePresent
                    &&
                    SN_TERMINATED_SCG_BEARER ==
                    p_uecc_ue_context->p_e_rab_list
                    [p_erab_list->erab_item[drb_idx].e_RAB_ID]->dc_bearer_type
                    )
            {
                continue;
            }
/* NR_DC Code Change Start */
#endif
            p_asn1_list->elem[p_asn1_list->n] = p_uecc_ue_context->p_e_rab_list
               [p_erab_list->erab_item[drb_idx].e_RAB_ID]->drb_config.drb_id;
            p_asn1_list->n++;
        }
        else 
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "generate_drb_release_info_list: p_e_rab_list[erab_id] =="
                    "NULL; Unused variable p_asn1_ctx=%p",p_asn1_ctx);/*SPR 17777+-*/
            return RRC_FAILURE;
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return RRC_SUCCESS;
}
/* ERB RELEASE COMMAND END */
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasConfig
*   INPUT        : uecc_ue_context_t*      p_uecc_ue_context
*                  rrm_meas_config_t*  p_rrm_meas_config
*                  OSCTXT*                 p_asn1_ctx
*                  MeasConfig*         p_meas_config
*   OUTPUT       : none
*
*   DESCRIPTION  : This function builds MeasConfig field in RRCConnectionReconfiguration
*                  message
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_MeasConfig(
    uecc_ue_context_t*  p_uecc_ue_context,
    rrm_meas_config_t*  p_rrm_meas_config,
    OSCTXT*             p_asn1_ctx,
    MeasConfig*         p_meas_config
)
{
	rrc_return_et       result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_meas_config);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_config);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
	/*Bug_13035_fix_start*/
	if (PNULL == p_rrm_meas_config)
	{
		RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
				(p_uecc_ue_context->p_gb_context)->facility_name,
				RRC_ERROR,
				"rrm_meas_config is NULL");
		return RRC_FAILURE;
	}
	/*Bug_13035_fix_stop*/

	do
	{
		asn1Init_MeasConfig(p_meas_config);
		/* MeasObjectToRemoveList */
		if (UE_ADM_MEAS_OBJECT_TO_REMOVE_LIST_PRESENT &
				p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.measObjectToRemoveListPresent = 1;

			result = uecc_llim_build_MeasObjectToRemoveList(
					p_uecc_ue_context,
					&p_rrm_meas_config->meas_object_to_remove_list,
					&p_meas_config->measObjectToRemoveList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}
		/* MeasObjectToAddModList */
		if (UE_ADM_MEAS_OBJECT_TO_ADD_MOD_LIST_PRESENT &
				p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.measObjectToAddModListPresent = 1;

			/* fill list of MeasObjectToAddMod */
			result = uecc_llim_build_MeasObjectToAddModList(
					p_uecc_ue_context, 
					&p_rrm_meas_config->meas_object_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_config->measObjectToAddModList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}
		/* ReportConfigToRemoveList */
		if (UE_ADM_MEAS_REPORT_CONFIG_TO_REMOVE_LIST_PRESENT &
				p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.reportConfigToRemoveListPresent = 1;

			result = uecc_llim_build_ReportConfigToRemoveList(
					p_uecc_ue_context, 
					&p_rrm_meas_config->report_config_to_remove_list,
					&p_meas_config->reportConfigToRemoveList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}
		/* ReportConfigToAddModList */
		if (UE_ADM_MEAS_REPORT_CONFIG_TO_ADD_MOD_LIST_PRESENT &
				p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.reportConfigToAddModListPresent = 1;

			/* fill list of ReportConfigToAddMod */
			result = uecc_llim_build_ReportConfigToAddModList(
					p_uecc_ue_context, 
					&p_rrm_meas_config->report_config_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_config->reportConfigToAddModList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}
		/* MeasIdToRemoveList */
		if (UE_ADM_MEAS_ID_TO_REMOVE_LIST_PRESENT &
				p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.measIdToRemoveListPresent = 1;

			result = uecc_llim_build_MeasIdToRemoveList(
					p_uecc_ue_context,
					&p_rrm_meas_config->meas_id_to_remove_list,
					&p_meas_config->measIdToRemoveList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}
		/* MeasIdToAddModList */
		if (UE_ADM_MEAS_ID_TO_ADD_MOD_LIST_PRESENT &
				p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.measIdToAddModListPresent = 1;

			/* fill list of MeasIdToAddMod */
			result = uecc_llim_build_MeasIdToAddModList(
					p_uecc_ue_context->p_gb_context, 
					&p_rrm_meas_config->meas_id_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_config->measIdToAddModList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}
		/* QuantityConfig */
		if (UE_ADM_MEAS_QUANTITY_CONFIG_PRESENT &
				p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.quantityConfigPresent = 1;

			result = uecc_llim_build_QuantityConfig(
					p_uecc_ue_context->p_gb_context,
					&p_rrm_meas_config->quantity_config,
                    p_asn1_ctx,
					&p_meas_config->quantityConfig);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* MeasGapConfig */
		if (UE_ADM_MEAS_GAP_CONFIG_PRESENT & p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.measGapConfigPresent = 1;

			result = uecc_llim_build_MeasGapConfig(
					p_uecc_ue_context,
					&p_rrm_meas_config->meas_gap_config,
					p_asn1_ctx,
					&p_meas_config->measGapConfig);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* RSRP_Range */
		if (UE_ADM_MEAS_S_MEASURE_PRESENT & p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.s_MeasurePresent = 1;
			p_meas_config->s_Measure = p_rrm_meas_config->s_measure;
		}

		/* PreRegistrationInfoHRPD */
		if (UE_ADM_MEAS_PRE_REGISTRATION_INFO_PRESENT &
				p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.preRegistrationInfoHRPDPresent = 1;

			uecc_llim_build_PreRegistrationInfoHRPD(
					p_uecc_ue_context->p_gb_context,
					&p_rrm_meas_config->pre_registration_info_hrpd,
					&p_meas_config->preRegistrationInfoHRPD);
		}

		/* MeasConfig_speedStatePars */
		if (UE_ADM_MEAS_SPEED_STATE_PARS_PRESENT & p_rrm_meas_config->bitmask)
		{
			p_meas_config->m.speedStateParsPresent = 1;

			result = uecc_llim_build_MeasConfig_speedStatePars(
					p_uecc_ue_context->p_gb_context,
					&p_rrm_meas_config->meas_config_speed_state_pars,
					p_asn1_ctx,
					&p_meas_config->speedStatePars);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

        #ifdef ENDC_ENABLED
		/* FR1 Gap */
		if ( UE_ADM_FR1_GAP_R15_PRESENT & p_rrm_meas_config->bitmask)
		{
			p_meas_config->m._v7ExtPresent = RRC_ONE;
			p_meas_config->m.fr1_Gap_r15Present = RRC_ONE;

            p_meas_config->fr1_Gap_r15 =
                p_rrm_meas_config->fr1_gap_r15;
		}

		/* MGTA */
		if ( UE_ADM_MGTA_R15_PRESENT & p_rrm_meas_config->bitmask)
		{
			p_meas_config->m._v7ExtPresent = RRC_ONE;
			p_meas_config->m.mgta_r15Present = RRC_ONE;

            p_meas_config->mgta_r15 =
                p_rrm_meas_config->mgta_r15;
		}
        #endif

		/* OSRTDList */
		rtxDListInit(&p_meas_config->extElem1);

		result = RRC_SUCCESS;
	}
	while(0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}
/* meas_config change start */
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CurrMeasConfig
 *   INPUT        : uecc_ue_context_t*      p_uecc_ue_context
 *                  OSCTXT*                 p_asn1_ctx
 *                  uecc_ue_curr_meas_config_t*  p_ue_curr_meas_config
 *                  MeasConfig*         p_meas_config
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function builds CurrMeasConfig field in RRCConnectionReconfiguration
 *                  message
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CurrMeasConfig(
		uecc_ue_context_t*  p_uecc_ue_context,
		uecc_ue_curr_meas_config_t*  p_ue_curr_meas_config,
		OSCTXT*             p_asn1_ctx,
		MeasConfig*         p_meas_config
		)
{
	rrc_return_et       result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_ue_curr_meas_config);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_config);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_MeasConfig(p_meas_config);
		/* MeasObjectToAddModList */
		if (p_ue_curr_meas_config->m.curr_meas_object_to_add_mod_list_present == 1)
		{
			p_meas_config->m.measObjectToAddModListPresent = 1;

			/* fill list of MeasObjectToAddMod */
			result = uecc_llim_build_CurrMeasObjectToAddModList(
					p_uecc_ue_context, 
					&p_ue_curr_meas_config->curr_meas_object_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_config->measObjectToAddModList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}
		/* ReportConfigToAddModList */
		if (p_ue_curr_meas_config->m.curr_meas_report_config_to_add_mod_list_present == 1)
		{
			p_meas_config->m.reportConfigToAddModListPresent = 1;

			/* fill list of ReportConfigToAddMod */
			result = uecc_llim_build_CurrReportConfigToAddModList(
					p_uecc_ue_context, 
					&p_ue_curr_meas_config->curr_report_config_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_config->reportConfigToAddModList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}
		/* MeasIdToAddModList */
		if (p_ue_curr_meas_config->m.curr_meas_id_to_add_mod_list_present == 1)
		{
			p_meas_config->m.measIdToAddModListPresent = 1;

			/* fill list of MeasIdToAddMod */
			result = uecc_llim_build_CurrMeasIdToAddModList(
					p_uecc_ue_context->p_gb_context, 
					&p_ue_curr_meas_config->curr_meas_id_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_config->measIdToAddModList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}
		/* QuantityConfig */
		if (p_ue_curr_meas_config->m.curr_meas_quantity_config_present == 1)
		{
			p_meas_config->m.quantityConfigPresent = 1;

			result = uecc_llim_build_QuantityConfig(
					p_uecc_ue_context->p_gb_context,
					&p_ue_curr_meas_config->curr_quantity_config,
					p_asn1_ctx,
					&p_meas_config->quantityConfig);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* MeasGapConfig */
		if (p_ue_curr_meas_config->m.curr_meas_gap_config_present == 1)
		{
			p_meas_config->m.measGapConfigPresent = 1;

			result = uecc_llim_build_MeasGapConfig(
					p_uecc_ue_context, 
					&p_ue_curr_meas_config->curr_meas_gap_config,
					p_asn1_ctx,
					&p_meas_config->measGapConfig);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* RSRP_Range */
		if (p_ue_curr_meas_config->m.curr_meas_s_measure_present == 1)
		{
			p_meas_config->m.s_MeasurePresent = 1;
			p_meas_config->s_Measure = p_ue_curr_meas_config->curr_s_measure;
		}

		/* PreRegistrationInfoHRPD */
		if (p_ue_curr_meas_config->m.curr_meas_pre_registration_info_present == 1)
		{
			p_meas_config->m.preRegistrationInfoHRPDPresent = 1;

			uecc_llim_build_PreRegistrationInfoHRPD(
					p_uecc_ue_context->p_gb_context,
					&p_ue_curr_meas_config->curr_pre_registration_info_hrpd,
					&p_meas_config->preRegistrationInfoHRPD);
		}

		/* MeasConfig_speedStatePars */
		if (p_ue_curr_meas_config->m.curr_meas_speed_state_pars_present == 1)
		{
			p_meas_config->m.speedStateParsPresent = 1;

			result = uecc_llim_build_MeasConfig_speedStatePars(
					p_uecc_ue_context->p_gb_context,
					&p_ue_curr_meas_config->curr_meas_config_speed_state_pars,
					p_asn1_ctx,
					&p_meas_config->speedStatePars);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* OSRTDList */
		rtxDListInit(&p_meas_config->extElem1);

		result = RRC_SUCCESS;
	}
	while(0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}
/* meas_config change end */

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_MeasObjectToRemoveList
 *   INPUT        : uecc_ue_context_t*              p_uecc_ue_context
 *                  meas_object_to_remove_list_t*   p_rrm_meas_object_to_remove_list
 *                  OSCTXT*                         p_asn1_ctx
 *                  MeasObjectToRemoveList*         p_meas_object_to_remove_list
 *   OUTPUT       : none
 *   DESCRIPTION  : This function builds MeasObjectToRemoveList in MeasConfig structure
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_MeasObjectToRemoveList(
		uecc_ue_context_t*              p_uecc_ue_context, 
		meas_object_to_remove_list_t*   p_rrm_meas_object_to_remove_list,
		MeasObjectToRemoveList*         p_meas_object_to_remove_list
		)
{
	U8              i=0;
	rrc_return_et   result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_meas_object_to_remove_list);
	RRC_ASSERT(PNULL != p_meas_object_to_remove_list);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	asn1Init_MeasObjectToRemoveList(p_meas_object_to_remove_list);

	do
	{
		/* check list size */
		if ( (p_rrm_meas_object_to_remove_list->count >
					ARRSIZE(p_meas_object_to_remove_list->elem)) ||
				(p_rrm_meas_object_to_remove_list->count < 1) )
		{
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_MeasObjectToRemoveList "
					"wrong list count [%u]",
					p_rrm_meas_object_to_remove_list->count);

			break;
		}

		p_meas_object_to_remove_list->n=p_rrm_meas_object_to_remove_list->count;

		for (i=0; i < MAX_REPORT_CONFIG_ID; i++)
		{
			p_meas_object_to_remove_list->elem[i] =
				p_rrm_meas_object_to_remove_list->meas_object_id[i];
		}

		result = RRC_SUCCESS;
	}
	while(0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_MeasObjectToAddModList
 *   INPUT        : uecc_ue_context_t*              p_uecc_ue_context
 *                  meas_object_to_add_mod_list_t*  p_rrm_meas_object_to_add_mod_list
 *                  OSCTXT*                         p_asn1_ctx
 *                  MeasObjectToAddModList*         p_meas_object_to_add_mod_list
 *   OUTPUT       : None
 *   DESCRIPTION  : This function builds MeasObjectToAddModList in MeasConfig structure
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_MeasObjectToAddModList(
		uecc_ue_context_t*              p_uecc_ue_context, 
		meas_object_to_add_mod_list_t*  p_rrm_meas_object_to_add_mod_list,
		OSCTXT*                         p_asn1_ctx,
		MeasObjectToAddModList*         p_meas_object_to_add_mod_list
		)
{
	U8 i=0;
	OSRTDListNode*              p_node = PNULL;
	MeasObjectToAddMod*         p_meas_object_to_add_mode = PNULL;
	rrc_return_et               result = RRC_SUCCESS;

	RRC_ASSERT(PNULL != p_rrm_meas_object_to_add_mod_list);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_to_add_mod_list);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    rtxDListInit(p_meas_object_to_add_mod_list);

    for (i=0; i < p_rrm_meas_object_to_add_mod_list->count; i++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, MeasObjectToAddMod,
            &p_node, &p_meas_object_to_add_mode);
        if (PNULL == p_node)
        {
            result = RRC_FAILURE;
            break;
        }

        result = uecc_llim_build_MeasObjectToAddMod(
                p_uecc_ue_context,
                &p_rrm_meas_object_to_add_mod_list->meas_object_to_add_mod[i],
                p_asn1_ctx,
                p_meas_object_to_add_mode);

        if (RRC_FAILURE == result)
        {
            RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index, 
                    (p_uecc_ue_context->p_gb_context)->facility_name, 
                    RRC_WARNING,
                    "uecc_llim_build_MeasObjectToAddModList error while build "
                    "MeasObjectToAddMod [i=%i]"
                    );
            break;
        }

        /* append node to the list */
        rtxDListAppendNode(p_meas_object_to_add_mod_list, p_node);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_CurrMeasObjectToAddModList
*   INPUT        : uecc_ue_context_t*              p_uecc_ue_context
*                  meas_object_to_add_mod_list_t*  p_rrm_curr_meas_object_to_add_mod_list
*                  OSCTXT*                         p_asn1_ctx
*                  MeasObjectToAddModList*         p_meas_object_to_add_mod_list
*   OUTPUT       : None
*
*   DESCRIPTION:
*       This function builds MeasObjectToAddModList in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_CurrMeasObjectToAddModList(
    uecc_ue_context_t*              p_uecc_ue_context,
    uecc_meas_object_to_add_mod_list_t*  p_rrm_curr_meas_object_to_add_mod_list,
    OSCTXT*                         p_asn1_ctx,
    MeasObjectToAddModList*         p_meas_object_to_add_mod_list
)
{
	U8 i=0;
	OSRTDListNode*              p_node = PNULL;
	MeasObjectToAddMod*         p_meas_object_to_add_mode = PNULL;
	rrc_return_et               result = RRC_SUCCESS;
	uecc_meas_object_to_add_mod_t*  p_curr_meas_config = PNULL;

	RRC_ASSERT(PNULL != p_rrm_curr_meas_object_to_add_mod_list);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_to_add_mod_list);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	rtxDListInit(p_meas_object_to_add_mod_list);

	p_curr_meas_config = (uecc_meas_object_to_add_mod_t*)
		ylFirst(p_rrm_curr_meas_object_to_add_mod_list);

	for (i=0; i < MAX_MEAS_OBJECT_ID && p_curr_meas_config; i++)
	{

		if (p_curr_meas_config->meas_object_id != 0)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx, MeasObjectToAddMod,
					&p_node, &p_meas_object_to_add_mode);
			if (PNULL == p_node)
			{
				result = RRC_FAILURE;
				break;
			}

			result = uecc_llim_build_CurrMeasObjectToAddMod(
					p_uecc_ue_context,
					p_curr_meas_config,
					p_asn1_ctx,
					p_meas_object_to_add_mode);

			if (RRC_FAILURE == result)
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,
						"uecc_llim_build_CurrMeasObjectToAddModList error while build "
						"MeasObjectToAddMod [i=%i]"
					      );
				break;
			}

			/* append node to the list */
			rtxDListAppendNode(p_meas_object_to_add_mod_list, p_node);
		}
		p_curr_meas_config = (uecc_meas_object_to_add_mod_t*)
			ylNext((YLNODE*)p_curr_meas_config);
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasObjectToAddMod
*   INPUT        : uecc_ue_context_t*              p_uecc_ue_context
*                  meas_object_to_add_mod_list_t*  p_rrm_meas_object_to_add_mod_list
*                  OSCTXT*                         p_asn1_ctx
*                  MeasObjectToAddMod*         p_meas_object_to_add_mod
*   OUTPUT       : None
*
*   DESCRIPTION:
*       This function builds MeasObjectToAddMod in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_MeasObjectToAddMod(
    uecc_ue_context_t*          p_uecc_ue_context,
    meas_object_to_add_mod_t*   p_rrm_meas_object_to_add_mod,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectToAddMod*         p_meas_object_to_add_mod)
{
	rrc_return_et               result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_meas_object_to_add_mod);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_to_add_mod);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	asn1Init_MeasObjectToAddMod(p_meas_object_to_add_mod);
	switch (p_rrm_meas_object_to_add_mod->meas_object.bitmask)
	{
		/* only CDMA2000 implemented */
		case MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT:
			p_meas_object_to_add_mod->measObjectId =
				p_rrm_meas_object_to_add_mod->meas_object_id;
			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectCDMA2000;

			p_meas_object_to_add_mod->measObject.u.measObjectCDMA2000 =
				rtxMemAllocType(p_asn1_ctx, MeasObjectCDMA2000);
			if (PNULL== p_meas_object_to_add_mod->measObject.u.measObjectCDMA2000)
			{
				break;
			}

			result = uecc_llim_build_MeasObjectCDMA2000(
					p_uecc_ue_context->p_gb_context, 
					&p_rrm_meas_object_to_add_mod->meas_object.meas_object_cdma2000,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectCDMA2000);

			break;

		case MEAS_OBJECT_TO_ADD_EUTRA_PRESENT:
			p_meas_object_to_add_mod->measObjectId =
				p_rrm_meas_object_to_add_mod->meas_object_id;
			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectEUTRA;

			p_meas_object_to_add_mod->measObject.u.measObjectEUTRA =
				rtxMemAllocType(p_asn1_ctx, MeasObjectEUTRA);

			if (PNULL == p_meas_object_to_add_mod->measObject.u.measObjectEUTRA)
			{
				break;
			}

			result = uecc_llim_build_MeasObjectEUTRA(
					p_uecc_ue_context, 
					&p_rrm_meas_object_to_add_mod->meas_object.meas_object_eutra,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectEUTRA);

			break;

		case MEAS_OBJECT_TO_ADD_UTRA_PRESENT:
			p_meas_object_to_add_mod->measObjectId =
				p_rrm_meas_object_to_add_mod->meas_object_id;
			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectUTRA;

			p_meas_object_to_add_mod->measObject.u.measObjectUTRA =
				rtxMemAllocType(p_asn1_ctx, MeasObjectUTRA);

			if (PNULL == p_meas_object_to_add_mod->measObject.u.measObjectUTRA)
			{
				break;
			}

			result = uecc_llim_build_MeasObjectUTRA(
					p_uecc_ue_context, 
					&p_rrm_meas_object_to_add_mod->meas_object.meas_object_utra,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectUTRA);

			break;

		case MEAS_OBJECT_TO_ADD_GERAN_PRESENT:

			p_meas_object_to_add_mod->measObjectId =
				p_rrm_meas_object_to_add_mod->meas_object_id;

			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectGERAN;

			p_meas_object_to_add_mod->measObject.u.measObjectGERAN =
				rtxMemAllocType(p_asn1_ctx, MeasObjectGERAN);

			if (PNULL == p_meas_object_to_add_mod->measObject.u.measObjectGERAN)
			{
				break;
			}
            /* Coverity_fix_81603_start */
			result = uecc_llim_build_MeasObjectGERAN(
					p_uecc_ue_context, 
					&p_rrm_meas_object_to_add_mod->meas_object.meas_object_geran,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectGERAN);

			/* Coverity_fix_81603_stop */
			break;

        #ifdef ENDC_ENABLED
		case MEAS_OBJECT_TO_ADD_NR_PRESENT:

			p_meas_object_to_add_mod->measObjectId =
				p_rrm_meas_object_to_add_mod->meas_object_id;

			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectNR_r15;

			p_meas_object_to_add_mod->measObject.u.measObjectNR_r15 =
				rtxMemAllocType(p_asn1_ctx, MeasObjectNR_r15);

			if (PNULL == p_meas_object_to_add_mod->measObject.u.measObjectNR_r15)
			{
				break;
			}
			result = uecc_llim_build_MeasObjectNR_r15(
					p_uecc_ue_context, 
					&p_rrm_meas_object_to_add_mod->meas_object.meas_object_nr,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectNR_r15);
			if ( RRC_SUCCESS != result )
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
                        "Failure returned by function uecc_llim_build_MeasObjectNR");
            }

			break;
        #endif


		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_MeasObjectToAddMod -> wrong bitmask");
			break;
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_CurrMeasObjectToAddMod
*   INPUTS       : uecc_ue_context_t*                   p_uecc_ue_context
*                  uecc_meas_object_to_add_mod_t*       p_ue_meas_object_to_add_mod,
*                  OSCTXT*                              p_asn1_ctx
*                  MeasObjectToAddMod*                  p_meas_object_to_add_mod
*   OUTPUTS      : None
*   DESCRIPTION:
*       This function builds MeasObjectToAddMod in CurrMeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_CurrMeasObjectToAddMod(
    uecc_ue_context_t*              p_uecc_ue_context,
    uecc_meas_object_to_add_mod_t*   p_ue_meas_object_to_add_mod,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectToAddMod*         p_meas_object_to_add_mod)
{
	rrc_return_et               result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_ue_meas_object_to_add_mod);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_to_add_mod);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	switch (p_ue_meas_object_to_add_mod->meas_object.t)
	{
		/* only CDMA2000 implemented */
		case UECC_MEAS_OBJECT_TO_ADD_CDMA2000_PRESENT:
			p_meas_object_to_add_mod->measObjectId =
				p_ue_meas_object_to_add_mod->meas_object_id;
			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectCDMA2000;

			p_meas_object_to_add_mod->measObject.u.measObjectCDMA2000 =
				rtxMemAllocType(p_asn1_ctx, MeasObjectCDMA2000);
			if (PNULL== p_meas_object_to_add_mod->measObject.u.measObjectCDMA2000)
			{
				break;
			}

			result = uecc_llim_build_CurrMeasObjectCDMA2000(
					p_uecc_ue_context->p_gb_context, 
					&p_ue_meas_object_to_add_mod->meas_object.u.meas_object_cdma2000,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectCDMA2000);

			break;

		case UECC_MEAS_OBJECT_TO_ADD_EUTRA_PRESENT:
			p_meas_object_to_add_mod->measObjectId =
				p_ue_meas_object_to_add_mod->meas_object_id;
			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectEUTRA;

			p_meas_object_to_add_mod->measObject.u.measObjectEUTRA =
				rtxMemAllocType(p_asn1_ctx, MeasObjectEUTRA);

			if (PNULL == p_meas_object_to_add_mod->measObject.u.measObjectEUTRA)
			{
				break;
			}

			result = uecc_llim_build_CurrMeasObjectEUTRA(
					p_uecc_ue_context, 
					&p_ue_meas_object_to_add_mod->meas_object.u.meas_object_eutra,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectEUTRA);

			break;

		case UECC_MEAS_OBJECT_TO_ADD_UTRA_PRESENT:
			p_meas_object_to_add_mod->measObjectId =
				p_ue_meas_object_to_add_mod->meas_object_id;
			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectUTRA;

			p_meas_object_to_add_mod->measObject.u.measObjectUTRA =
				rtxMemAllocType(p_asn1_ctx, MeasObjectUTRA);

			if (PNULL == p_meas_object_to_add_mod->measObject.u.measObjectUTRA)
			{
				break;
			}

			result = uecc_llim_build_CurrMeasObjectUTRA(
					p_uecc_ue_context, 
					&p_ue_meas_object_to_add_mod->meas_object.u.meas_object_utra,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectUTRA);

			break;

		case UECC_MEAS_OBJECT_TO_ADD_GERAN_PRESENT:

			p_meas_object_to_add_mod->measObjectId =
				p_ue_meas_object_to_add_mod->meas_object_id;

			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectGERAN;

			p_meas_object_to_add_mod->measObject.u.measObjectGERAN =
				rtxMemAllocType(p_asn1_ctx, MeasObjectGERAN);

			if (PNULL == p_meas_object_to_add_mod->measObject.u.measObjectGERAN)
			{
				break;
			}

			result = uecc_llim_build_MeasObjectGERAN(
					p_uecc_ue_context,
					&p_ue_meas_object_to_add_mod->meas_object.u.meas_object_geran,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectGERAN);
			/* Coverity ID 81496 Starts */
			if(RRC_SUCCESS != result)
			{RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"Failure returned by function uecc_llim_build_MeasObjectGERAN");
			}
			/* Coverity ID 81496 Ends */


			result = RRC_SUCCESS;

			break;

        #ifdef ENDC_ENABLED
		case MEAS_OBJECT_TO_ADD_NR_PRESENT:

			p_meas_object_to_add_mod->measObjectId =
				p_ue_meas_object_to_add_mod->meas_object_id;

			p_meas_object_to_add_mod->measObject.t =
				T_MeasObjectToAddMod_measObject_measObjectNR_r15;

			p_meas_object_to_add_mod->measObject.u.measObjectNR_r15 =
				rtxMemAllocType(p_asn1_ctx, MeasObjectNR_r15);

			if (PNULL == p_meas_object_to_add_mod->measObject.u.measObjectNR_r15)
			{
				break;
			}
			result = uecc_llim_build_MeasObjectNR_r15(
					p_uecc_ue_context, 
					&p_ue_meas_object_to_add_mod->meas_object.u.meas_object_nr,
					p_asn1_ctx,
					p_meas_object_to_add_mod->measObject.u.measObjectNR_r15);
			if ( RRC_SUCCESS != result )
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
                        "Failure returned by function uecc_llim_build_curr_MeasObjectNR");
            }

			result = RRC_SUCCESS;

			break;
        #endif


		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_MeasObjectToAddMod -> wrong bitmask");
			break;
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_ReportConfigToRemoveList
 *   INPUT        : uecc_ue_context_t*              p_uecc_ue_context
 *                  report_config_to_remove_list_t* p_rrm_report_config_to_remove_list
 *                  OSCTXT*                         p_asn1_ctx
 *                  ReportConfigToRemoveList*       p_report_config_to_remove_list
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds ReportConfigToRemoveList in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_ReportConfigToRemoveList(
		uecc_ue_context_t*              p_uecc_ue_context, 
		report_config_to_remove_list_t* p_rrm_report_config_to_remove_list,
		ReportConfigToRemoveList*       p_report_config_to_remove_list
		)
{
    U8              i=0;
    rrc_return_et   result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_rrm_report_config_to_remove_list);
    RRC_ASSERT(PNULL != p_report_config_to_remove_list);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    asn1Init_ReportConfigToRemoveList(p_report_config_to_remove_list);

    do
    {
        /* check list size */
        if ( (p_rrm_report_config_to_remove_list->count >
                ARRSIZE(p_report_config_to_remove_list->elem)) ||
            (p_rrm_report_config_to_remove_list->count < 1) )
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, 
                    RRC_WARNING,
                    "uecc_llim_build_ReportConfigToRemoveList wrong list "
                    "count [%u]",
                    p_rrm_report_config_to_remove_list->count);
            break;
        }

        p_report_config_to_remove_list->n =
            p_rrm_report_config_to_remove_list->count;

        for (i=0; i < MAX_REPORT_CONFIG_ID; i++)
        {
            p_report_config_to_remove_list->elem[i] =
                p_rrm_report_config_to_remove_list->report_config_id[i];
        }

        result = RRC_SUCCESS;
    }
    while(0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_ReportConfigToAddModList
*   INPUT        : uecc_ue_context_t*                  p_uecc_ue_context
*                  report_config_to_add_mod_list_t*    p_rrm_report_config_to_add_mod_list
*                  OSCTXT*                             p_asn1_ctx
*                  ReportConfigToAddModList*           p_report_config_to_add_mod_list
*   OUTPUT       :  none
*   DESCRIPTION  : This function builds ReportConfigToAddModList in MeasConfig structure
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_ReportConfigToAddModList(
    uecc_ue_context_t*                  p_uecc_ue_context,
    report_config_to_add_mod_list_t*    p_rrm_report_config_to_add_mod_list,
    OSCTXT*                             p_asn1_ctx,
    ReportConfigToAddModList*           p_report_config_to_add_mod_list
)
{
    rrc_return_et           result = RRC_SUCCESS;
    ReportConfigToAddMod*   p_report_config_to_add_mod = PNULL;
    OSRTDListNode*          p_node = PNULL;
    U8 i=0;

    RRC_ASSERT(PNULL != p_rrm_report_config_to_add_mod_list);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_report_config_to_add_mod_list);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    rtxDListInit(p_report_config_to_add_mod_list);

    for (i=0; i < p_rrm_report_config_to_add_mod_list->count; i++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, ReportConfigToAddMod,
            &p_node, &p_report_config_to_add_mod);
        if (PNULL == p_node)
        {
            result = RRC_FAILURE;
            break;
        }

        result = uecc_llim_build_ReportConfigToAddMod(
            p_uecc_ue_context, 
            &p_rrm_report_config_to_add_mod_list->report_config_to_add_mod[i],
            p_asn1_ctx,
            p_report_config_to_add_mod);

        if (RRC_FAILURE == result)
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, 
                RRC_WARNING,
                "uecc_llim_build_ReportConfigToAddModList error while build "
                "ReportConfigToAddMod [i=%i]",
                i);
            break;
        }
        /* append node to the list */
        rtxDListAppendNode(p_report_config_to_add_mod_list, p_node);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_CurrReportConfigToAddModList
*   INPUT        : uecc_ue_context_t*                        p_uecc_ue_context
*                  uecc_report_config_to_add_mod_list_t*    p_ue_curr_report_config_to_add_mod_list
*                  OSCTXT*                             p_asn1_ctx
*                  ReportConfigToAddModList*           p_report_config_to_add_mod_list
*   OUTPUT       : none
*   DESCRIPTION:
*       This function builds ReportConfigToAddModList in CurrMeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_CurrReportConfigToAddModList(
    uecc_ue_context_t*                        p_uecc_ue_context,
    uecc_report_config_to_add_mod_list_t*    p_ue_curr_report_config_to_add_mod_list,
    OSCTXT*                             p_asn1_ctx,
    ReportConfigToAddModList*           p_report_config_to_add_mod_list
)
{
    rrc_return_et           result = RRC_SUCCESS;
    ReportConfigToAddMod*   p_report_config_to_add_mod = PNULL;
    OSRTDListNode*          p_node = PNULL;
    U8 i=0;
    uecc_report_config_to_add_mod_t *p_report_config = PNULL;

    RRC_ASSERT(PNULL != p_ue_curr_report_config_to_add_mod_list);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_report_config_to_add_mod_list);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    rtxDListInit(p_report_config_to_add_mod_list);

    p_report_config = (uecc_report_config_to_add_mod_t*)
        ylFirst(p_ue_curr_report_config_to_add_mod_list);

    for (i=0; i < MAX_REPORT_CONFIG_ID && p_report_config; i++)
    {

      if (p_report_config->report_config_id != 0)
      {
 
        rtxDListAllocNodeAndData(p_asn1_ctx, ReportConfigToAddMod,
            &p_node, &p_report_config_to_add_mod);
        if (PNULL == p_node)
        {
            result = RRC_FAILURE;
            break;
        }

        result = uecc_llim_build_report_configToAddMod(
            p_uecc_ue_context,
            p_report_config,
            p_asn1_ctx,
            p_report_config_to_add_mod);

        if (RRC_FAILURE == result)
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, 
                RRC_WARNING,
                "uecc_llim_build_ReportConfigToAddModList error while build "
                "ReportConfigToAddMod [i=%i]",
                i);
            break;
        }
        /* append node to the list */
        rtxDListAppendNode(p_report_config_to_add_mod_list, p_node);
      }
      p_report_config = (uecc_report_config_to_add_mod_t*)
          ylNext((YLNODE*)p_report_config);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}


/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasIdToRemoveList
*   INPUT        : uecc_ue_context_t*          p_uecc_ue_context
*                  meas_id_to_remove_list_t*   p_rrm_meas_id_to_remove_list
*                  OSCTXT*                     p_asn1_ctx
*                  MeasIdToRemoveList*         p_meas_id_to_remove_list
*   OUTPUT       : None
*   DESCRIPTION:
*       This function builds MeasIdToRemoveList in
*       MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_MeasIdToRemoveList(
    uecc_ue_context_t*          p_uecc_ue_context,
    meas_id_to_remove_list_t*   p_rrm_meas_id_to_remove_list,
    MeasIdToRemoveList*         p_meas_id_to_remove_list
)
{
    U8              i=0;
    rrc_return_et   result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_rrm_meas_id_to_remove_list);
    RRC_ASSERT(PNULL != p_meas_id_to_remove_list);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);


    asn1Init_MeasIdToRemoveList(p_meas_id_to_remove_list);
    do
    {
        /* check list size */
        if ( (p_rrm_meas_id_to_remove_list->count >
                ARRSIZE(p_meas_id_to_remove_list->elem)) ||
            (p_rrm_meas_id_to_remove_list->count < 1) )
        {
            RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                    p_uecc_ue_context->p_gb_context->facility_name, 
                    RRC_WARNING,
                    "uecc_llim_build_MeasIdToRemoveList wrong list count [%u]",
                    p_rrm_meas_id_to_remove_list->count);
            break;
        }

        p_meas_id_to_remove_list->n =
            p_rrm_meas_id_to_remove_list->count;

        for (i=0; i < p_meas_id_to_remove_list->n; i++)
        {
            p_meas_id_to_remove_list->elem[i] =
                p_rrm_meas_id_to_remove_list->meas_id[i];
        }

        result = RRC_SUCCESS;
    }
    while(0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasIdToAddModList
*   INPUT        : uecc_gb_context_t           *p_uecc_gb_context
*                  meas_id_to_add_mod_list_t*  p_rrm_meas_id_to_add_mod_list
*                  OSCTXT*                     p_asn1_ctx
*                  MeasIdToAddModList*         p_meas_id_to_add_mod_list
*   OUTPUT       : None
*   DESCRIPTION:
*       This function builds MeasIdToAddModList in
*       MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_MeasIdToAddModList(
    uecc_gb_context_t           *p_uecc_gb_context,
    meas_id_to_add_mod_list_t*  p_rrm_meas_id_to_add_mod_list,
    OSCTXT*                     p_asn1_ctx,
    MeasIdToAddModList*         p_meas_id_to_add_mod_list
)
{
    rrc_return_et   result = RRC_SUCCESS;
    MeasIdToAddMod* p_meas_id_to_add_mod = PNULL;
    OSRTDListNode*  p_node = PNULL;
    U8 i=0;

    RRC_ASSERT(PNULL != p_rrm_meas_id_to_add_mod_list);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_meas_id_to_add_mod_list);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/
    rtxDListInit(p_meas_id_to_add_mod_list);

    for (i=0; i < MAX_MEAS_ID; i++)
    {
        if (p_rrm_meas_id_to_add_mod_list->meas_id_to_add_mod[i].meas_id != 0)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, MeasIdToAddMod,
            &p_node, &p_meas_id_to_add_mod);
        if (PNULL == p_node)
        {
            result = RRC_FAILURE;
            break;
        }

        p_meas_id_to_add_mod->measId =
            p_rrm_meas_id_to_add_mod_list->meas_id_to_add_mod[i].meas_id;
        p_meas_id_to_add_mod->measObjectId =
            p_rrm_meas_id_to_add_mod_list->meas_id_to_add_mod[i].meas_object_id;
        p_meas_id_to_add_mod->reportConfigId =
            p_rrm_meas_id_to_add_mod_list->meas_id_to_add_mod[i].
                report_config_id;

        /* append node to the list */
        rtxDListAppendNode(p_meas_id_to_add_mod_list, p_node);
    }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_CurrMeasIdToAddModList
*   INPUT        : uecc_gb_context_t           *p_uecc_gb_context
*                  curr_meas_id_to_add_mod_list_t*  p_rrm_meas_id_to_add_mod_list
*                  OSCTXT*                     p_asn1_ctx
*                  MeasIdToAddModList*         p_meas_id_to_add_mod_list
*   OUTPUT       : None
*
*   DESCRIPTION:
*       This function builds MeasIdToAddModList in
*       CurrMeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_CurrMeasIdToAddModList(
    uecc_gb_context_t           *p_uecc_gb_context,/* SPR 15905 Fix Start */
    curr_meas_id_to_add_mod_list_t*  p_rrm_curr_meas_id_to_add_mod_list,/* SPR 15905 Fix Stop */
    OSCTXT*                     p_asn1_ctx,
    MeasIdToAddModList*         p_meas_id_to_add_mod_list
)
{
    rrc_return_et   result = RRC_SUCCESS;
    MeasIdToAddMod* p_meas_id_to_add_mod = PNULL;
    OSRTDListNode*  p_node = PNULL;
    U8 i=0;

    RRC_ASSERT(PNULL != p_rrm_curr_meas_id_to_add_mod_list);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_meas_id_to_add_mod_list);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/

    rtxDListInit(p_meas_id_to_add_mod_list);

    for (i=0; i < MAX_MEAS_ID; i++)
    {
        if (p_rrm_curr_meas_id_to_add_mod_list->meas_id_to_add_mod[i].meas_id != 0)
        {
           rtxDListAllocNodeAndData(p_asn1_ctx, MeasIdToAddMod,
            &p_node, &p_meas_id_to_add_mod);
           if (PNULL == p_node)
           {
            result = RRC_FAILURE;
            break;
           }

           p_meas_id_to_add_mod->measId =
             p_rrm_curr_meas_id_to_add_mod_list->meas_id_to_add_mod[i].meas_id;
           p_meas_id_to_add_mod->measObjectId =
             p_rrm_curr_meas_id_to_add_mod_list->meas_id_to_add_mod[i].meas_object_id;
           p_meas_id_to_add_mod->reportConfigId =
             p_rrm_curr_meas_id_to_add_mod_list->meas_id_to_add_mod[i].
                report_config_id;

           /* append node to the list */
           rtxDListAppendNode(p_meas_id_to_add_mod_list, p_node);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

#ifdef ENDC_ENABLED
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasConfig_endc
*   INPUT        : uecc_ue_context_t*               p_uecc_ue_context
*                  rrc_rrm_endc_meas_config_t*      p_rrm_meas_config
*                  OSCTXT*                          p_asn1_ctx
*                  MeasConfig*                      p_meas_config
*   OUTPUT       : none
*   DESCRIPTION  : This function builds MeasConfig field in
*                  RRCConnectionReconfiguration message
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
******************************************************************************/
rrc_return_et
uecc_llim_build_MeasConfig_endc
(
    uecc_ue_context_t*           p_uecc_ue_context,
    rrc_rrm_endc_meas_config_t*  p_rrm_meas_config,
    OSCTXT*                      p_asn1_ctx,
    MeasConfig*                  p_meas_config
)
{
	rrc_return_et       result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_meas_config);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_config);

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	if (PNULL == p_rrm_meas_config)
	{
		RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
				(p_uecc_ue_context->p_gb_context)->facility_name,
				RRC_ERROR,
				"rrm_meas_config is NULL");
		return RRC_FAILURE;
	}

	do
	{
		asn1Init_MeasConfig(p_meas_config);

		/* MeasObjectToRemoveList */
		{
			p_meas_config->m.measObjectToRemoveListPresent = 1;

			result = uecc_llim_build_MeasObjectToRemoveList(
					p_uecc_ue_context,
					&p_rrm_meas_config->meas_object_to_remove_list,
					&p_meas_config->measObjectToRemoveList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* ReportConfigToRemoveList */
		{
			p_meas_config->m.reportConfigToRemoveListPresent = 1;

			result = uecc_llim_build_ReportConfigToRemoveList(
					p_uecc_ue_context, 
					&p_rrm_meas_config->report_config_to_remove_list,
					&p_meas_config->reportConfigToRemoveList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* MeasIdToRemoveList */
		{
			p_meas_config->m.measIdToRemoveListPresent = 1;

			result = uecc_llim_build_MeasIdToRemoveList(
					p_uecc_ue_context,
					&p_rrm_meas_config->meas_id_to_remove_list,
					&p_meas_config->measIdToRemoveList);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* MeasGapConfig */
		{
			p_meas_config->m.measGapConfigPresent = 1;

			result = uecc_llim_build_MeasGapConfig(
					p_uecc_ue_context,
					&p_rrm_meas_config->meas_gap_config,
					p_asn1_ctx,
					&p_meas_config->measGapConfig);

			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* OSRTDList */
		rtxDListInit(&p_meas_config->extElem1);

		result = RRC_SUCCESS;
	}
	while(0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/*********************************************************************************************
*   FUNCTION NAME: uecc_llim_build_quantity_config_NR_list_r15 
*   INPUT        : uecc_gb_context_t                 *p_uecc_gb_context
*                  quantity_config_nr_list_r15_t     *p_rrm_quantity_config_nr_list_r15
*                  OSCTXT*                            p_asn1_ctx
*                  QuantityConfigNRList_r15          *p_quantity_config_nr_list_r15
*   OUTPUT       : none
*   DESCRIPTION  : This function builds QuantityConfigNRList_r15 in QuantityConfig structure
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
************************************************************************************************/
rrc_return_et
uecc_llim_build_quantity_config_NR_list_r15
(
    uecc_gb_context_t                 *p_uecc_gb_context,
    quantity_config_nr_list_r15_t     *p_rrm_quantity_config_nr_list_r15,
    OSCTXT*                            p_asn1_ctx,
    QuantityConfigNRList_r15          *p_quantity_config_nr_list_r15
)
{
    rrc_return_et           result = RRC_SUCCESS;
    OSRTDListNode*          p_node = PNULL;
    U8                      count = RRC_NULL;
    QuantityConfigNR_r15    *p_quantity_config_nr_r15 = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    rtxDListInit(p_quantity_config_nr_list_r15);

    for ( count = RRC_NULL; count < p_rrm_quantity_config_nr_list_r15->
        count; count++ )
    {
        rtxDListAllocNodeAndData(
                p_asn1_ctx,
                QuantityConfigNR_r15,
                &p_node,
                &p_quantity_config_nr_r15);
        if ( PNULL == p_node )
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name, 
                    RRC_WARNING,
                    "Memory allocation failed for QuantityConfigNR_r15");
            result = RRC_FAILURE;
            break;
        }

        asn1Init_QuantityConfigNR_r15(p_quantity_config_nr_r15);

        p_quantity_config_nr_r15->m.measQuantityRS_IndexNR_r15Present = RRC_NULL;

        p_quantity_config_nr_r15->measQuantityCellNR_r15.filterCoeff_RSRP_r15 =
            p_rrm_quantity_config_nr_list_r15->quantity_config_nr_r15[count].
                meas_quantity_cell_nr_r15.filter_coeff_rsrp_r15;

        p_quantity_config_nr_r15->measQuantityCellNR_r15.filterCoeff_RSRQ_r15 =
            p_rrm_quantity_config_nr_list_r15->quantity_config_nr_r15[count].
                meas_quantity_cell_nr_r15.filter_coeff_rsrq_r15;

        p_quantity_config_nr_r15->measQuantityCellNR_r15.filterCoefficient_SINR_r13 =
            p_rrm_quantity_config_nr_list_r15->quantity_config_nr_r15[count].
                meas_quantity_cell_nr_r15.filter_coefficient_sinr_r13;

        if ( QUANTITY_CONFIG_RS_INDEX_NR_PRESENT &
            p_rrm_quantity_config_nr_list_r15->quantity_config_nr_r15[count].bitmask )
        {
            p_quantity_config_nr_r15->m.measQuantityRS_IndexNR_r15Present = RRC_ONE;

            p_quantity_config_nr_r15->measQuantityRS_IndexNR_r15.filterCoeff_RSRP_r15 =
                p_rrm_quantity_config_nr_list_r15->quantity_config_nr_r15[count].
                    meas_quantity_rs_index_nr_r15.filter_coeff_rsrp_r15;

            p_quantity_config_nr_r15->measQuantityRS_IndexNR_r15.filterCoeff_RSRQ_r15 =
                p_rrm_quantity_config_nr_list_r15->quantity_config_nr_r15[count].
                    meas_quantity_rs_index_nr_r15.filter_coeff_rsrq_r15;

            p_quantity_config_nr_r15->measQuantityRS_IndexNR_r15.filterCoefficient_SINR_r13 =
                p_rrm_quantity_config_nr_list_r15->quantity_config_nr_r15[count].
                    meas_quantity_rs_index_nr_r15.filter_coefficient_sinr_r13;
        }

        rtxDListAppendNode(p_quantity_config_nr_list_r15, p_node);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}
#endif

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_QuantityConfig
*   INPUT        : uecc_gb_context_t           *p_uecc_gb_context
*                  quantity_config_t*  p_rrm_quantity_config
*                  OSCTXT*                     p_asn1_ctx
*                  QuantityConfig*     p_quantity_config
*   OUTPUT       : None
*
*   DESCRIPTION:
*       This function builds QuantityConfig in
*       MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_QuantityConfig(
    uecc_gb_context_t   *p_uecc_gb_context,
    quantity_config_t*  p_rrm_quantity_config,
    OSCTXT*             p_asn1_ctx,    
    QuantityConfig*     p_quantity_config)
{
    rrc_return_et result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_rrm_quantity_config);
    RRC_ASSERT(PNULL != p_quantity_config);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/

    do
    {
        /* only CDMA2000 implemented now */
        if (QUANTITY_CONFIG_CDMA2000_PRESENT & p_rrm_quantity_config->bitmask)
        {
            p_quantity_config->m.quantityConfigCDMA2000Present = 1;

            p_quantity_config->quantityConfigCDMA2000.measQuantityCDMA2000 =
                p_rrm_quantity_config->quantity_config_cdma2000.
                    meas_quantity_cdma2000;

            result = RRC_SUCCESS;
        }

        if (QUANTITY_CONFIG_EUTRA_PRESENT & p_rrm_quantity_config->bitmask)
        {
             p_quantity_config->m.quantityConfigEUTRAPresent = 1;

            p_quantity_config->quantityConfigEUTRA.filterCoefficientRSRP =
                p_rrm_quantity_config->quantity_config_eutra.filter_coefficient_rsrp;

            p_quantity_config->quantityConfigEUTRA.filterCoefficientRSRQ =
                p_rrm_quantity_config->quantity_config_eutra.filter_coefficient_rsrq;
        
            result = RRC_SUCCESS;
        }

        if (QUANTITY_CONFIG_UTRA_PRESENT & p_rrm_quantity_config->bitmask)
        {
            p_quantity_config->m.quantityConfigUTRAPresent = 1;

            p_quantity_config->quantityConfigUTRA.measQuantityUTRA_FDD =
                p_rrm_quantity_config->quantity_config_utra.meas_quantity_utra_fdd;

            p_quantity_config->quantityConfigUTRA.measQuantityUTRA_TDD =
                p_rrm_quantity_config->quantity_config_utra.meas_quantity_utra_tdd;
        
            p_quantity_config->quantityConfigUTRA.filterCoefficient =
                p_rrm_quantity_config->quantity_config_utra.filter_coefficient;
            
        result = RRC_SUCCESS;
            
        }

        if (QUANTITY_CONFIG_GERAN_PRESENT & p_rrm_quantity_config->bitmask)
        {


            p_quantity_config->m.quantityConfigGERANPresent = 1;
            
            p_quantity_config->quantityConfigGERAN.measQuantityGERAN = 
                p_rrm_quantity_config->quantity_config_geran.meas_quantity_geran;
              
            p_quantity_config->quantityConfigGERAN.filterCoefficient = 
                p_rrm_quantity_config->quantity_config_geran.filter_coefficient;
             
        result = RRC_SUCCESS;
        }
        
        if (QUANTITY_CONFIG_UTRA_v1020_PRESENT & p_rrm_quantity_config->bitmask)
        {

            p_quantity_config->m._v2ExtPresent = 1;
            p_quantity_config->m.quantityConfigUTRA_v1020Present = 1;
            
            p_quantity_config->quantityConfigUTRA_v1020.filterCoefficient2_FDD_r10 = 
                p_rrm_quantity_config->quantity_config_UTRA_v1020.filterCoefficient2_FDD_r10;
              
             
        result = RRC_SUCCESS;
        }

        #ifdef ENDC_ENABLED
        if ( QUANTITY_CONFIG_NR_LIST_R15_PRESENT &
            p_rrm_quantity_config->bitmask )
        {

            p_quantity_config->m._v5ExtPresent = RRC_ONE;
            p_quantity_config->m.quantityConfigNRList_r15Present = RRC_ONE;

            result = uecc_llim_build_quantity_config_NR_list_r15(
                    p_uecc_gb_context,
                    &p_rrm_quantity_config->quantity_config_nr_list_r15,
                    p_asn1_ctx,
                    &p_quantity_config->quantityConfigNRList_r15);
            if(RRC_FAILURE == result)
            {
                break;
            }
        }
        #endif

        /* OSRTDList */
        rtxDListInit(&p_quantity_config->extElem1);
    }
    while(0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasGapConfig
*   INPUT        : uecc_ue_context_t*  p_uecc_ue_context
*                  meas_gap_config_t*  p_rrm_meas_gap_config
*                  OSCTXT*             p_asn1_ctx
*                  MeasGapConfig*      p_meas_gap_config
*   OUTPUT       : none
*   DESCRIPTION:
*       This function builds MeasGapConfig in
*       MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_MeasGapConfig(
    uecc_ue_context_t*  p_uecc_ue_context,
    meas_gap_config_t*  p_rrm_meas_gap_config,
    OSCTXT*             p_asn1_ctx,
    MeasGapConfig*      p_meas_gap_config)
{
    rrc_return_et result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_rrm_meas_gap_config);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_meas_gap_config);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    do
    {
        if (MEAS_GAP_CONFIG_SETUP_CONFIG_INFO_PRESENT &
                p_rrm_meas_gap_config->bitmask)
        {
            p_meas_gap_config->t = T_MeasGapConfig_setup;
            p_meas_gap_config->u.setup =
                rtxMemAllocType(p_asn1_ctx, MeasGapConfig_setup);
            if (PNULL == p_meas_gap_config->u.setup)
            {
                break;
            }

            switch(p_rrm_meas_gap_config->setup_config_info.bitmask)
            {
                case MEAS_GAP_CONFIG_GP0_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp0;
                    p_meas_gap_config->u.setup->gapOffset.u.gp0 =
                        p_rrm_meas_gap_config->setup_config_info.gp0;
                    result = RRC_SUCCESS;
                    break;

                case MEAS_GAP_CONFIG_GP1_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp1;
                    p_meas_gap_config->u.setup->gapOffset.u.gp1 =
                        p_rrm_meas_gap_config->setup_config_info.gp1;
                    result = RRC_SUCCESS;
                    break;

                #ifdef ENDC_ENABLED
                case MEAS_GAP_CONFIG_GP4_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp4_r15;

                    p_meas_gap_config->u.setup->gapOffset.u.gp4_r15 =
                        p_rrm_meas_gap_config->setup_config_info.gp4_r15;
                    result = RRC_SUCCESS;
                    break;

                case MEAS_GAP_CONFIG_GP5_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp5_r15;

                    p_meas_gap_config->u.setup->gapOffset.u.gp5_r15 =
                        p_rrm_meas_gap_config->setup_config_info.gp5_r15;
                    result = RRC_SUCCESS;
                    break;

                case MEAS_GAP_CONFIG_GP6_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp6_r15;

                    p_meas_gap_config->u.setup->gapOffset.u.gp6_r15 =
                        p_rrm_meas_gap_config->setup_config_info.gp6_r15;
                    result = RRC_SUCCESS;
                    break;

                case MEAS_GAP_CONFIG_GP7_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp7_r15;

                    p_meas_gap_config->u.setup->gapOffset.u.gp7_r15 =
                        p_rrm_meas_gap_config->setup_config_info.gp7_r15;
                    result = RRC_SUCCESS;
                    break;

                case MEAS_GAP_CONFIG_GP8_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp8_r15;

                    p_meas_gap_config->u.setup->gapOffset.u.gp8_r15 =
                        p_rrm_meas_gap_config->setup_config_info.gp8_r15;
                    result = RRC_SUCCESS;
                    break;

                case MEAS_GAP_CONFIG_GP9_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp9_r15;

                    p_meas_gap_config->u.setup->gapOffset.u.gp9_r15 =
                        p_rrm_meas_gap_config->setup_config_info.gp9_r15;
                    result = RRC_SUCCESS;
                    break;

                case MEAS_GAP_CONFIG_GP10_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp10_r15;

                    p_meas_gap_config->u.setup->gapOffset.u.gp10_r15 =
                        p_rrm_meas_gap_config->setup_config_info.gp10_r15;
                    result = RRC_SUCCESS;
                    break;

                case MEAS_GAP_CONFIG_GP11_PRESENT:
                    p_meas_gap_config->u.setup->gapOffset.t =
                        T_MeasGapConfig_setup_gapOffset_gp11_r15;

                    p_meas_gap_config->u.setup->gapOffset.u.gp11_r15 =
                        p_rrm_meas_gap_config->setup_config_info.gp11_r15;
                    result = RRC_SUCCESS;
                    break;

                #endif
                default:
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
                            p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
                    "uecc_llim_build_MeasGapConfig -> wrong bitmask");
                    break;
            }
        }
        else
        {
            p_meas_gap_config->t = T_MeasGapConfig_release;
            result = RRC_SUCCESS;
        }
    }
    while(0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_PreRegistrationInfoHRPD
*   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
*                  pre_registration_info_hrpd_t*   p_rrm_pre_registration_info_hrpd
*                  OSCTXT*                         p_asn1_ctx
*                  PreRegistrationInfoHRPD*        p_pre_registration_info_hrpd
*   OUTPUT       : None
*   DESCRIPTION:
*       This function builds PreRegistrationInfoHRPD in
*       MeasConfig structure
*
*   RETURNS:
*       None
*
******************************************************************************/
void uecc_llim_build_PreRegistrationInfoHRPD(
    uecc_gb_context_t               *p_uecc_gb_context,
    pre_registration_info_hrpd_t*   p_rrm_pre_registration_info_hrpd,
    PreRegistrationInfoHRPD*        p_pre_registration_info_hrpd
)
{
    RRC_ASSERT(PNULL != p_rrm_pre_registration_info_hrpd);
    RRC_ASSERT(PNULL != p_pre_registration_info_hrpd);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/

    p_pre_registration_info_hrpd->preRegistrationAllowed =
        p_rrm_pre_registration_info_hrpd->pre_registration_allowed;

    if (PRE_REGISTRATION_ZONE_ID_PRESENT &
        p_rrm_pre_registration_info_hrpd->bitmask)
    {
        p_pre_registration_info_hrpd->m.preRegistrationZoneIdPresent = 1;
        p_pre_registration_info_hrpd->preRegistrationZoneId =
            p_rrm_pre_registration_info_hrpd->pre_registration_zone_id;
    }

    if (PRE_REGISTRATION_SECONDARY_ZONE_ID_LIST_PRESENT &
        p_rrm_pre_registration_info_hrpd->bitmask)
    {
        p_pre_registration_info_hrpd->m.
            secondaryPreRegistrationZoneIdListPresent = 1;
        p_pre_registration_info_hrpd->secondaryPreRegistrationZoneIdList.n =
            p_rrm_pre_registration_info_hrpd->
                secondary_pre_registration_zone_id_list.count;
        p_pre_registration_info_hrpd->secondaryPreRegistrationZoneIdList.
            elem[0] =
            p_rrm_pre_registration_info_hrpd->
            secondary_pre_registration_zone_id_list.
                pre_registration_zone_id_hrpd[0];
        p_pre_registration_info_hrpd->secondaryPreRegistrationZoneIdList.
            elem[1] =
            p_rrm_pre_registration_info_hrpd->
                secondary_pre_registration_zone_id_list.
                    pre_registration_zone_id_hrpd[1];
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasConfig_speedStatePars
*   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
*                  meas_config_speed_state_pars_t* p_rrm_meas_config_speed_state_pars
*                  OSCTXT*                         p_asn1_ctx
*                   MeasConfig_speedStatePars*      p_meas_config_speed_state_pars
*   OUTPUT       : none
*   DESCRIPTION:
*       This function builds MeasConfig_speedStatePars in
*       MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_MeasConfig_speedStatePars(
    uecc_gb_context_t               *p_uecc_gb_context,
    meas_config_speed_state_pars_t* p_rrm_meas_config_speed_state_pars,
    OSCTXT*                         p_asn1_ctx,
    MeasConfig_speedStatePars*      p_meas_config_speed_state_pars)
{
    rrc_return_et result = RRC_FAILURE;

    RRC_ASSERT(PNULL != p_rrm_meas_config_speed_state_pars);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_meas_config_speed_state_pars);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/
    do
    {
        if (0 != p_rrm_meas_config_speed_state_pars->bitmask)
        {
            p_meas_config_speed_state_pars->t=T_MeasConfig_speedStatePars_setup;
            p_meas_config_speed_state_pars->u.setup =
                rtxMemAllocType(p_asn1_ctx, MeasConfig_speedStatePars_setup);

            if (PNULL == p_meas_config_speed_state_pars->u.setup)
            {
                break;
            }

            p_meas_config_speed_state_pars->u.setup->mobilityStateParameters.
                t_Evaluation =
                p_rrm_meas_config_speed_state_pars->setup.
                    mobility_state_parameters.t_evaluation;

            p_meas_config_speed_state_pars->u.setup->mobilityStateParameters.
                t_HystNormal =
                p_rrm_meas_config_speed_state_pars->setup.
                    mobility_state_parameters.t_hyst_normal;

            p_meas_config_speed_state_pars->u.setup->mobilityStateParameters.
                n_CellChangeMedium =
                p_rrm_meas_config_speed_state_pars->
                    setup.mobility_state_parameters.m_cell_charge_medium;

            p_meas_config_speed_state_pars->u.setup->mobilityStateParameters.
                n_CellChangeHigh =
                p_rrm_meas_config_speed_state_pars->setup.
                    mobility_state_parameters.m_cell_charge_high;

            p_meas_config_speed_state_pars->u.setup->timeToTrigger_SF.
                sf_Medium =
                p_rrm_meas_config_speed_state_pars->setup.
                    time_to_trigger_sf.sf_medium;

            p_meas_config_speed_state_pars->u.setup->timeToTrigger_SF.
                sf_High =
                p_rrm_meas_config_speed_state_pars->setup.
                    time_to_trigger_sf.sf_high;

            result = RRC_SUCCESS;
        }
        else
        {
            p_meas_config_speed_state_pars->t =
                T_MeasConfig_speedStatePars_release;
            result = RRC_SUCCESS;
        }
    }
    while(0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasObjectCDMA2000
*   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
*                  meas_object_cdma2000_t*     p_rrm_meas_object_cdma2000
*                  OSCTXT*                     p_asn1_ctx
*                  MeasObjectCDMA2000*         p_meas_object_cdma2000
*   OUTPUT       : None
*   DESCRIPTION:
*       This function builds MeasObjectCDMA2000 in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_MeasObjectCDMA2000(
    uecc_gb_context_t               *p_uecc_gb_context,
    meas_object_cdma2000_t*     p_rrm_meas_object_cdma2000,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectCDMA2000*         p_meas_object_cdma2000
)
{
    rrc_return_et   result = RRC_FAILURE;
    U8 i=0;

    RRC_ASSERT(PNULL != p_rrm_meas_object_cdma2000);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_meas_object_cdma2000);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    do
    {
        asn1Init_MeasObjectCDMA2000(p_meas_object_cdma2000);

        /* OSUINT8 */
        if (MEAS_OBJECT_CDMA2000_SEARCH_WINDOW_SIZE_PRESENT &
            p_rrm_meas_object_cdma2000->bitmask)
        {
            p_meas_object_cdma2000->m.searchWindowSizePresent = 1;
            p_meas_object_cdma2000->searchWindowSize =
                p_rrm_meas_object_cdma2000->search_window_size;
        }

        /* CellIndexList */
        if (MEAS_OBJECT_CDMA2000_CELLS_TO_REMOVE_LIST_PRESENT &
            p_rrm_meas_object_cdma2000->bitmask)
        {
            /* check list size */
            if ( (p_rrm_meas_object_cdma2000->cells_to_remove_list.count >
                ARRSIZE(p_meas_object_cdma2000->cellsToRemoveList.elem)) ||
                (p_rrm_meas_object_cdma2000->cells_to_remove_list.count < 1) )
            {
                break;
            }

            p_meas_object_cdma2000->m.cellsToRemoveListPresent = 1;

            p_meas_object_cdma2000->cellsToRemoveList.n =
                p_rrm_meas_object_cdma2000->cells_to_remove_list.count;

            for (i=0; i < p_meas_object_cdma2000->cellsToRemoveList.n; i++)
            {
                p_meas_object_cdma2000->cellsToRemoveList.elem[i] =
                 p_rrm_meas_object_cdma2000->cells_to_remove_list.cell_index[i];
            }
        }

        /* CellsToAddModListCDMA2000 */
        if (MEAS_OBJECT_CDMA2000_CELLS_TO_ADD_MOD_LIST_PRESENT &
            p_rrm_meas_object_cdma2000->bitmask)
        {
            p_meas_object_cdma2000->m.cellsToAddModListPresent = 1;

            /* fill list of CellsToAddModCDMA2000 */
            result = uecc_llim_build_CellsToAddModListCDMA2000(
                        p_uecc_gb_context,  
                        &p_rrm_meas_object_cdma2000->cells_to_add_mod_list,
                        p_asn1_ctx,
                        &p_meas_object_cdma2000->cellsToAddModList);
            if (RRC_FAILURE == result)
            {
                break;
            }
        }

        /* PhysCellIdCDMA2000 */
        if (MEAS_OBJECT_CDMA2000_CELL_FOR_WHICH_TO_REPEORT_CGI_PRESENT &
            p_rrm_meas_object_cdma2000->bitmask)
        {
            p_meas_object_cdma2000->m.cellForWhichToReportCGIPresent = 1;
            p_meas_object_cdma2000->cellForWhichToReportCGI =
                p_rrm_meas_object_cdma2000->cells_for_which_to_report_cgi;
        }

        p_meas_object_cdma2000->cdma2000_Type =
            p_rrm_meas_object_cdma2000->cdma2000_type;

        p_meas_object_cdma2000->carrierFreq.bandClass =
            p_rrm_meas_object_cdma2000->carrier_freq_cdma2000.band_class;
        p_meas_object_cdma2000->carrierFreq.arfcn =
            p_rrm_meas_object_cdma2000->carrier_freq_cdma2000.arfcn;

        p_meas_object_cdma2000->offsetFreq =
            p_rrm_meas_object_cdma2000->offset_freq;

        /* OSRTDList */
        rtxDListInit(&p_meas_object_cdma2000->extElem1);

        result = RRC_SUCCESS;
    }
    while(0);

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_CurrMeasObjectCDMA2000
*   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
*                  meas_object_cdma2000_t*     p_rrm_meas_object_cdma2000
*                  OSCTXT*                     p_asn1_ctx
*                  MeasObjectCDMA2000*         p_meas_object_cdma2000
*   OUTPUT       : None
*
*   DESCRIPTION:
*       This function builds MeasObjectCDMA2000 in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_CurrMeasObjectCDMA2000(
    uecc_gb_context_t           *p_uecc_gb_context,
    meas_object_cdma2000_t*     p_rrm_meas_object_cdma2000,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectCDMA2000*         p_meas_object_cdma2000
)
{
    rrc_return_et   result = RRC_FAILURE;
    U8 i=0;

    RRC_ASSERT(PNULL != p_rrm_meas_object_cdma2000);
    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_meas_object_cdma2000);
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    do
    {
        asn1Init_MeasObjectCDMA2000(p_meas_object_cdma2000);

        /* OSUINT8 */
        if (MEAS_OBJECT_CDMA2000_SEARCH_WINDOW_SIZE_PRESENT &
            p_rrm_meas_object_cdma2000->bitmask)
        {
            p_meas_object_cdma2000->m.searchWindowSizePresent = 1;
            p_meas_object_cdma2000->searchWindowSize =
                p_rrm_meas_object_cdma2000->search_window_size;
        }

        /* CellIndexList */
        if (MEAS_OBJECT_CDMA2000_CELLS_TO_REMOVE_LIST_PRESENT &
            p_rrm_meas_object_cdma2000->bitmask)
        {
            /* check list size */
            if ( (p_rrm_meas_object_cdma2000->cells_to_remove_list.count >
                ARRSIZE(p_meas_object_cdma2000->cellsToRemoveList.elem)) ||
                (p_rrm_meas_object_cdma2000->cells_to_remove_list.count < 1) )
            {
                break;
            }

            p_meas_object_cdma2000->m.cellsToRemoveListPresent = 1;

            p_meas_object_cdma2000->cellsToRemoveList.n =
                p_rrm_meas_object_cdma2000->cells_to_remove_list.count;

            for (i=0; i < p_meas_object_cdma2000->cellsToRemoveList.n; i++)
            {
                p_meas_object_cdma2000->cellsToRemoveList.elem[i] =
                 p_rrm_meas_object_cdma2000->cells_to_remove_list.cell_index[i];
            }
        }

        /* CellsToAddModListCDMA2000 */
        if (MEAS_OBJECT_CDMA2000_CELLS_TO_ADD_MOD_LIST_PRESENT &
            p_rrm_meas_object_cdma2000->bitmask)
        {
            p_meas_object_cdma2000->m.cellsToAddModListPresent = 1;

            /* fill list of CellsToAddModCDMA2000 */
            result = uecc_llim_build_CurrCellsToAddModListCDMA2000(
                        p_uecc_gb_context, 
                        &p_rrm_meas_object_cdma2000->cells_to_add_mod_list,
                        p_asn1_ctx,
                        &p_meas_object_cdma2000->cellsToAddModList);
            if (RRC_FAILURE == result)
            {
                break;
            }
        }

        /* PhysCellIdCDMA2000 */
        if (MEAS_OBJECT_CDMA2000_CELL_FOR_WHICH_TO_REPEORT_CGI_PRESENT &
            p_rrm_meas_object_cdma2000->bitmask)
        {
            p_meas_object_cdma2000->m.cellForWhichToReportCGIPresent = 1;
            p_meas_object_cdma2000->cellForWhichToReportCGI =
                p_rrm_meas_object_cdma2000->cells_for_which_to_report_cgi;
        }

        p_meas_object_cdma2000->cdma2000_Type =
            p_rrm_meas_object_cdma2000->cdma2000_type;

        p_meas_object_cdma2000->carrierFreq.bandClass =
            p_rrm_meas_object_cdma2000->carrier_freq_cdma2000.band_class;
        p_meas_object_cdma2000->carrierFreq.arfcn =
            p_rrm_meas_object_cdma2000->carrier_freq_cdma2000.arfcn;

        p_meas_object_cdma2000->offsetFreq =
            p_rrm_meas_object_cdma2000->offset_freq;

        /* OSRTDList */
        rtxDListInit(&p_meas_object_cdma2000->extElem1);

        result = RRC_SUCCESS;
    }
    while(0);
                
    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/* eICIC meas changes start */
/*******************************************************************************************************
*   FUNCTION NAME: uecc_llim_build_measSubframePatternNeigh_r10
*   INPUT        : uecc_gb_context_t                          *p_uecc_gb_context
*                  rrc_phy_meas_subframe_pattern_fdd_r10_t    *p_rrm_meas_subframe_pattern_fdd_r10
*                  OSCTXT*                                     p_asn1_ctx
*                  MeasSubframePattern_r10                    *p_rrm_meas_subframe_pattern_fdd_r10
*   OUTPUT       : none
*   DESCRIPTION:
*       This function builds measSubframePatternNeigh_r10 in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
*******************************************************************************************************/
rrc_return_et uecc_llim_build_measSubframePatternNeigh_r10(
    uecc_gb_context_t                       *p_uecc_gb_context,
    rrc_phy_meas_subframe_pattern_r10_t     *p_rrm_meas_subframe_pattern_r10,
    OSCTXT*                                  p_asn1_ctx,
    MeasSubframePattern_r10                 *p_meas_subframe_pattern_neigh_r10)
{

    rrc_return_et           result = RRC_SUCCESS;
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);


    if(p_rrm_meas_subframe_pattern_r10->bitmask & 
            RRC_PHY_MEAS_SUBFRAME_PATTERN_FDD_R10)
    {
        p_meas_subframe_pattern_neigh_r10->t = T_MeasSubframePattern_r10_subframePatternFDD_r10;

        p_meas_subframe_pattern_neigh_r10->u.subframePatternFDD_r10 = rtxMemAllocType(p_asn1_ctx,
                MeasSubframePattern_r10_subframePatternFDD_r10);
        if(PNULL == p_meas_subframe_pattern_neigh_r10->u.subframePatternFDD_r10)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name, RRC_WARNING,
                    "Memory allocation failed for subframePatternFDD_r10");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return RRC_FAILURE;                    
        }

        asn1Init_MeasSubframePattern_r10_subframePatternFDD_r10(p_meas_subframe_pattern_neigh_r10->u.subframePatternFDD_r10);

        p_meas_subframe_pattern_neigh_r10->u.subframePatternFDD_r10->numbits = MAX_SUBFRAME_CONFIG_FDD_NUMBITS;

        l3_memcpy_wrapper(p_meas_subframe_pattern_neigh_r10->u.subframePatternFDD_r10->data,
                p_rrm_meas_subframe_pattern_r10->subframe_pattern_fdd_r10.data,
                MAX_SUBFRAME_PATTERN_FDD);
    }
    else if(p_rrm_meas_subframe_pattern_r10->bitmask & 
            RRC_PHY_MEAS_SUBFRAME_PATTERN_TDD_R10)
    {
        p_meas_subframe_pattern_neigh_r10->t = T_MeasSubframePattern_r10_subframePatternTDD_r10;

        p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10 = rtxMemAllocType(p_asn1_ctx,
                MeasSubframePattern_r10_subframePatternTDD_r10);
        if(PNULL == p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                    p_uecc_gb_context->facility_name, RRC_WARNING,
                    "Memory allocation failed for subframePatternTDD_r10");
            RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
            return RRC_FAILURE;                    
        }

        asn1Init_MeasSubframePattern_r10_subframePatternTDD_r10(p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10);

        if(p_rrm_meas_subframe_pattern_r10->subframe_pattern_tdd_r10.bitmask & 
                RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG1_5_R10)
        {
            p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->t = 
                T_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig1_5_r10;

            p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig1_5_r10 = rtxMemAllocType(p_asn1_ctx,
                    ASN1BitStr32);
            if(PNULL == p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig1_5_r10)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name, RRC_WARNING,
                        "Memory allocation failed for subframeConfig1_5_r10");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                return RRC_FAILURE;
            }

            p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig1_5_r10->numbits = MAX_SUBFRAME_CONFIG1_5_TDD_NUMBITS;

            l3_memcpy_wrapper(p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig1_5_r10->data,
                    p_rrm_meas_subframe_pattern_r10->subframe_pattern_tdd_r10.subframe_config1_5_r10,
                    MAX_SUBFRAME_CONFIG1_5_TDD);


        }
        else if(p_rrm_meas_subframe_pattern_r10->subframe_pattern_tdd_r10.bitmask &
                RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG0_R10)
        {
            p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->t = 
                T_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig0_r10;

            p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig0_r10 = rtxMemAllocType(p_asn1_ctx,
                    MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig0_r10);
            if(PNULL == p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig0_r10)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name, RRC_WARNING,
                        "Memory allocation failed for subframeConfig0_r10");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                return RRC_FAILURE;
            }


            asn1Init_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig0_r10(p_meas_subframe_pattern_neigh_r10->
                      u.subframePatternTDD_r10->u.subframeConfig0_r10);

            p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig0_r10->numbits = MAX_SUBFRAME_CONFIG0_TDD_NUMBITS;

            l3_memcpy_wrapper(p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig0_r10->data,
                    p_rrm_meas_subframe_pattern_r10->subframe_pattern_tdd_r10.subframe_config0_r10,
                    MAX_SUBFRAME_CONFIG0_TDD);
        }
        else if(p_rrm_meas_subframe_pattern_r10->subframe_pattern_tdd_r10.bitmask &
                RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG6_R10)
        {
            p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->t = 
                T_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig6_r10;

            p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig6_r10 = rtxMemAllocType(p_asn1_ctx,
                    MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig6_r10);

            if(PNULL == p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig6_r10)
            {
                RRC_UECC_TRACE(p_uecc_gb_context->context_index,
                        p_uecc_gb_context->facility_name, RRC_WARNING,
                        "Memory allocation failed for subframeConfig6_r10");
                RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
                return RRC_FAILURE;
            }

        
            asn1Init_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig6_r10(p_meas_subframe_pattern_neigh_r10->
                                                            u.subframePatternTDD_r10->u.subframeConfig6_r10);

            p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig6_r10->numbits = MAX_SUBFRAME_CONFIG6_TDD_NUMBITS;

            l3_memcpy_wrapper(p_meas_subframe_pattern_neigh_r10->u.subframePatternTDD_r10->u.subframeConfig6_r10->data,
                    p_rrm_meas_subframe_pattern_r10->subframe_pattern_tdd_r10.subframe_config6_r10,
                    MAX_SUBFRAME_CONFIG6_TDD);
        }
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}

/*********************************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasSubframeCellList_r10 
*   INPUT        : uecc_gb_context_t                 *p_uecc_gb_context
*                  meas_subframe_cell_list_r10_t     *p_rrm_meassubframe_cell_list_r10
*                  OSCTXT*                            p_asn1_ctx
*                  MeasSubframeCellList_r10          *p_meassubframe_cell_list_r10
*   OUTPUT       : none
*   DESCRIPTION:
*       This function builds MeasSubframeCellList_r10 in MeasObjectEUTRA structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
************************************************************************************************/
rrc_return_et uecc_llim_build_MeasSubframeCellList_r10(
        uecc_gb_context_t                 *p_uecc_gb_context,
        meas_subframe_cell_list_r10_t     *p_rrm_meassubframe_cell_list_r10,
        OSCTXT*                            p_asn1_ctx,
        MeasSubframeCellList_r10          *p_meassubframe_cell_list_r10)
{
    rrc_return_et result = RRC_SUCCESS;
    PhysCellIdRange*        p_phys_cellid_range;
    OSRTDListNode*          p_node = PNULL;
    U8 count = RRC_NULL;
    RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    
    rtxDListInit(p_meassubframe_cell_list_r10);

    for(count=0; count < p_rrm_meassubframe_cell_list_r10->count; count++)
    {
        rtxDListAllocNodeAndData(
                p_asn1_ctx,
                PhysCellIdRange,
                &p_node,
                &p_phys_cellid_range);
        if (PNULL == p_node)
        {
            RRC_UECC_TRACE(p_uecc_gb_context->context_index, 
                    p_uecc_gb_context->facility_name, 
                    RRC_WARNING,
                    "Memory allocation failed for PhysCellIdRange");
            result = RRC_FAILURE;
            break;
        }

        asn1Init_PhysCellIdRange(p_phys_cellid_range);

        p_phys_cellid_range->m.rangePresent = 0;
        p_phys_cellid_range->start = p_rrm_meassubframe_cell_list_r10->meas_subframe_cells[count].start;

        if(p_rrm_meassubframe_cell_list_r10->meas_subframe_cells[count].presence_bitmask &
                PHY_CELL_ID_RANGE)
        {
            p_phys_cellid_range->m.rangePresent = 1;
            p_phys_cellid_range->range = p_rrm_meassubframe_cell_list_r10->meas_subframe_cells[count].range;
        }

        rtxDListAppendNode(p_meassubframe_cell_list_r10, p_node);
    }

    RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
    return result;
}
/* eICIC meas changes stop */
/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_MeasObjectEUTRA
*   INPUT        : uecc_ue_context_t*          p_uecc_ue_context
*                  meas_object_eutra_t*     p_rrm_meas_object_eutra
*                  OSCTXT*                     p_asn1_ctx
*                  MeasObjectEUTRA*         p_meas_object_eutra
*   OUTPUT       : none
*   DESCRIPTION:
*       This function builds MeasObjectEUTRA in MeasConfig structure
*
*   RETURNS:
*       RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_MeasObjectEUTRA(
    uecc_ue_context_t*          p_uecc_ue_context,
    meas_object_eutra_t*     p_rrm_meas_object_eutra,
    OSCTXT*                     p_asn1_ctx,
    MeasObjectEUTRA*         p_meas_object_eutra)
{
	rrc_return_et   result = RRC_FAILURE;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_meas_object_eutra);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_eutra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_MeasObjectEUTRA(p_meas_object_eutra);
		/* BUG 11440 Fix Start */
		RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
				p_uecc_ue_context->p_gb_context->facility_name, 
				RRC_DETAILED,
				"uecc_llim_build_MeasObjectEUTRA bitmask [%d]",
				p_rrm_meas_object_eutra->bitmask);
		/* BUG 11440 Fix Stop */

		/* CellIndexList */
		if (MEAS_OBJECT_EUTRA_CELLS_TO_REMOVE_LIST_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			/* check list size */
			if ((p_rrm_meas_object_eutra->cells_to_remove_list.count >
						ARRSIZE(p_meas_object_eutra->cellsToRemoveList.elem)) ||
					(p_rrm_meas_object_eutra->cells_to_remove_list.count < 1) )
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name,
						RRC_WARNING,
						"uecc_llim_build_MeasObjectEUTRA wrong list count [%u]",
						p_rrm_meas_object_eutra->cells_to_remove_list.count);
				break; 
			}

			p_meas_object_eutra->m.cellsToRemoveListPresent = 1;

			p_meas_object_eutra->cellsToRemoveList.n =
				p_rrm_meas_object_eutra->cells_to_remove_list.count;

			for (count = 0; count < p_meas_object_eutra->cellsToRemoveList.n; count++)
			{
				p_meas_object_eutra->cellsToRemoveList.elem[count] =
					p_rrm_meas_object_eutra->cells_to_remove_list.cell_index[count];
			}
		}
		/* CellsToAddModListEUTRA */
		if (MEAS_OBJECT_EUTRA_CELLS_TO_ADD_MOD_LIST_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			p_meas_object_eutra->m.cellsToAddModListPresent = 1;

			/* fill list of CellsToAddModEUTRA */
			result = uecc_llim_build_CellsToAddModListEUTRA(
					p_uecc_ue_context->p_gb_context,  
					&p_rrm_meas_object_eutra->cells_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_object_eutra->cellsToAddModList);
			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* BlackCellIndexList */
		if (MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_REMOVE_LIST_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			/* check list size */
			if ((p_rrm_meas_object_eutra->black_cells_to_remove_list.count >
						ARRSIZE(p_meas_object_eutra->blackCellsToRemoveList.elem)) ||
					(p_rrm_meas_object_eutra->black_cells_to_remove_list.count < 1) )
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,
						"uecc_llim_build_MeasObjectEUTRA1 wrong list count [%u]",
						p_rrm_meas_object_eutra->cells_to_remove_list.count);
				break;
			}

			p_meas_object_eutra->m.blackCellsToRemoveListPresent = 1;

			p_meas_object_eutra->blackCellsToRemoveList.n =
				p_rrm_meas_object_eutra->black_cells_to_remove_list.count;

			for (count=0; count < p_meas_object_eutra->blackCellsToRemoveList.n;
					count++)
			{
				p_meas_object_eutra->blackCellsToRemoveList.elem[count] =
					p_rrm_meas_object_eutra->black_cells_to_remove_list.
					cell_index[count];
			}
		}

		/* BlackCellsToAddModListEUTRA */
		if (MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_ADD_MOD_LIST_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			p_meas_object_eutra->m.blackCellsToAddModListPresent = 1;

			/* fill list of blackCellsToAddModEUTRA */
			result = uecc_llim_build_BlackListedCellsToAddModListEUTRA(
					p_uecc_ue_context->p_gb_context, 
					&p_rrm_meas_object_eutra->black_cells_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_object_eutra->blackCellsToAddModList);
			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* PhysCellIdEUTRA */
		if (MEAS_OBJECT_EUTRA_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			p_meas_object_eutra->m.cellForWhichToReportCGIPresent = 1;
			p_meas_object_eutra->cellForWhichToReportCGI =
				p_rrm_meas_object_eutra->cell_for_which_to_report_cgi;
		}
		p_meas_object_eutra->carrierFreq =
			p_rrm_meas_object_eutra->carrier_freq;

		p_meas_object_eutra->allowedMeasBandwidth =
			p_rrm_meas_object_eutra->allowed_meas_bandwidth;

		p_meas_object_eutra->presenceAntennaPort1 =
			p_rrm_meas_object_eutra->presence_antenna_port1;

		p_meas_object_eutra->neighCellConfig.numbits = 2;
		/* SPR 15008 Fix Start*/
		p_meas_object_eutra->neighCellConfig.data[0] =
		    (p_rrm_meas_object_eutra->neigh_cell_config<<6)&0xc0;
		/* SPR 15008 Fix end */

		p_meas_object_eutra->offsetFreq =
			p_rrm_meas_object_eutra->offset_freq;

		if(MEAS_OBJECT_EUTRA_CYCLE_SCELL_R10_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			p_meas_object_eutra->m._v2ExtPresent = 1;
			p_meas_object_eutra->m.measCycleSCell_r10Present = 1;
			p_meas_object_eutra->measCycleSCell_r10 = 
				p_rrm_meas_object_eutra->meas_cycle_scell_r10;
		}
		/* eICIC meas changes start */
		/* measSubframePatternConfigNeigh_r10 */
		if((MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_PRESENT &
					p_rrm_meas_object_eutra->bitmask))
		{
			p_meas_object_eutra->m._v2ExtPresent = 1;
			p_meas_object_eutra->m.measSubframePatternConfigNeigh_r10Present =1;
			if(p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.bitmask &
					MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_SETUP_PRESENT)
			{
				p_meas_object_eutra->measSubframePatternConfigNeigh_r10.t = T_MeasSubframePatternConfigNeigh_r10_setup;
				p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup = rtxMemAllocType(p_asn1_ctx,
						MeasSubframePatternConfigNeigh_r10_setup);              

				asn1Init_MeasSubframePatternConfigNeigh_r10_setup(p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup);

				result = uecc_llim_build_measSubframePatternNeigh_r10(p_uecc_ue_context->p_gb_context,
						&p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.
						meas_subframe_pattern_config_neigh_r10_setup.meas_subframe_pattern_neigh_r10,
						p_asn1_ctx,
						&p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup->measSubframePatternNeigh_r10);   
				if(RRC_SUCCESS != result)
				{
					break;
				}

				if(p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.
						meas_subframe_pattern_config_neigh_r10_setup.bitmask &
						MEAS_SUBFRAME_CELL_LIST_R10_PRESENT)
				{
					p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup->m.measSubframeCellList_r10Present = 1;
					result = uecc_llim_build_MeasSubframeCellList_r10(
							p_uecc_ue_context->p_gb_context,
							&p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.
							meas_subframe_pattern_config_neigh_r10_setup.meas_subframe_cell_list_r10,
							p_asn1_ctx,
							&p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup->measSubframeCellList_r10);
					if(RRC_FAILURE == result)
					{
						break;
					}

				}
			}
            else if(p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.bitmask &
                    MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_RELEASE_PRESENT)
            {
				p_meas_object_eutra->measSubframePatternConfigNeigh_r10.t = T_MeasSubframePatternConfigNeigh_r10_release;
            }
		}
		/* eICIC meas changes stop */

		/* OSRTDList */
		rtxDListInit(&p_meas_object_eutra->extElem1);
		result = RRC_SUCCESS;
	}
	while(0);
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CurrMeasObjectEUTRA
 *   INPUT        : uecc_ue_context_t*          p_uecc_ue_context
 *                  meas_object_eutra_t*     p_rrm_meas_object_eutra
 *                  OSCTXT*                     p_asn1_ctx
 *                  MeasObjectEUTRA*         p_meas_object_eutra
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds MeasObjectEUTRA in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CurrMeasObjectEUTRA(
		uecc_ue_context_t*          p_uecc_ue_context,
		meas_object_eutra_t*        p_rrm_meas_object_eutra,
		OSCTXT*                     p_asn1_ctx,
		MeasObjectEUTRA*         p_meas_object_eutra)
{
	rrc_return_et   result = RRC_FAILURE;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_meas_object_eutra);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_eutra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_MeasObjectEUTRA(p_meas_object_eutra);
		/* CellIndexList */
		if (MEAS_OBJECT_EUTRA_CELLS_TO_REMOVE_LIST_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			/* check list size */
			if ((p_rrm_meas_object_eutra->cells_to_remove_list.count >
						ARRSIZE(p_meas_object_eutra->cellsToRemoveList.elem)) ||
					(p_rrm_meas_object_eutra->cells_to_remove_list.count < 1) )
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name,
						RRC_WARNING,
						"uecc_llim_build_MeasObjectEUTRA wrong list count [%u]",
						p_rrm_meas_object_eutra->cells_to_remove_list.count);
				break; 
			}

			p_meas_object_eutra->m.cellsToRemoveListPresent = 1;

			p_meas_object_eutra->cellsToRemoveList.n =
				p_rrm_meas_object_eutra->cells_to_remove_list.count;

			for (count = 0; count < p_meas_object_eutra->cellsToRemoveList.n; count++)
			{
				p_meas_object_eutra->cellsToRemoveList.elem[count] =
					p_rrm_meas_object_eutra->cells_to_remove_list.cell_index[count];
			}
		}
		/* CellsToAddModListEUTRA */
		if (MEAS_OBJECT_EUTRA_CELLS_TO_ADD_MOD_LIST_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			p_meas_object_eutra->m.cellsToAddModListPresent = 1;

			/* fill list of CellsToAddModEUTRA */
			result = uecc_llim_build_CurrCellsToAddModListEUTRA(
					p_uecc_ue_context->p_gb_context, 
					&p_rrm_meas_object_eutra->cells_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_object_eutra->cellsToAddModList);
			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* BlackCellIndexList */
		if (MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_REMOVE_LIST_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			/* check list size */
			if ((p_rrm_meas_object_eutra->black_cells_to_remove_list.count >
						ARRSIZE(p_meas_object_eutra->blackCellsToRemoveList.elem)) ||
					(p_rrm_meas_object_eutra->black_cells_to_remove_list.count < 1) )
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name,
						RRC_WARNING,
						"uecc_llim_build_MeasObjectEUTRA wrong list count [%u]",
						p_rrm_meas_object_eutra->cells_to_remove_list.count);
				break;
			}

			p_meas_object_eutra->m.blackCellsToRemoveListPresent = 1;

			p_meas_object_eutra->blackCellsToRemoveList.n =
				p_rrm_meas_object_eutra->black_cells_to_remove_list.count;

			for (count=0; count < p_meas_object_eutra->blackCellsToRemoveList.n;
					count++)
			{
				p_meas_object_eutra->blackCellsToRemoveList.elem[count] =
					p_rrm_meas_object_eutra->black_cells_to_remove_list.
					cell_index[count];
			}
		}

		/* BlackCellsToAddModListEUTRA */
		if (MEAS_OBJECT_EUTRA_BLACK_CELLS_TO_ADD_MOD_LIST_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			p_meas_object_eutra->m.blackCellsToAddModListPresent = 1;

			/* fill list of blackCellsToAddModEUTRA */
			result = uecc_llim_build_CurrBlackListedCellsToAddModListEUTRA(
					p_uecc_ue_context->p_gb_context,
					&p_rrm_meas_object_eutra->black_cells_to_add_mod_list,
					p_asn1_ctx,
					&p_meas_object_eutra->blackCellsToAddModList);
			if (RRC_FAILURE == result)
			{
				break;
			}
		}

		/* PhysCellIdEUTRA */
		if (MEAS_OBJECT_EUTRA_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			p_meas_object_eutra->m.cellForWhichToReportCGIPresent = 1;
			p_meas_object_eutra->cellForWhichToReportCGI =
				p_rrm_meas_object_eutra->cell_for_which_to_report_cgi;
		}
		p_meas_object_eutra->carrierFreq =
			p_rrm_meas_object_eutra->carrier_freq;

		p_meas_object_eutra->allowedMeasBandwidth =
			p_rrm_meas_object_eutra->allowed_meas_bandwidth;

		p_meas_object_eutra->presenceAntennaPort1 =
			p_rrm_meas_object_eutra->presence_antenna_port1;

		p_meas_object_eutra->neighCellConfig.numbits = 2;
		/*SPR 15008 Fix Start*/
		p_meas_object_eutra->neighCellConfig.data[0] =
		    (p_rrm_meas_object_eutra->neigh_cell_config<<6)&0xc0;
		/*SPR 15008 Fix End*/

		p_meas_object_eutra->offsetFreq =
			p_rrm_meas_object_eutra->offset_freq;


		if (MEAS_OBJECT_EUTRA_CYCLE_SCELL_R10_PRESENT &
				p_rrm_meas_object_eutra->bitmask)
		{
			p_meas_object_eutra->m._v2ExtPresent = 1;
			p_meas_object_eutra->m.measCycleSCell_r10Present = 1;
			p_meas_object_eutra->measCycleSCell_r10 = 
				p_rrm_meas_object_eutra->meas_cycle_scell_r10;
		}

		/* eICIC meas changes start */
		/* measSubframePatternConfigNeigh_r10 */
		if((MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_PRESENT & 
					p_rrm_meas_object_eutra->bitmask))
		{
			p_meas_object_eutra->m._v2ExtPresent = 1;
			p_meas_object_eutra->m.measSubframePatternConfigNeigh_r10Present =1;
			if(p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.bitmask &
					MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_SETUP_PRESENT)
			{
				p_meas_object_eutra->measSubframePatternConfigNeigh_r10.t = T_MeasSubframePatternConfigNeigh_r10_setup;
				p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup = rtxMemAllocType(p_asn1_ctx,
						MeasSubframePatternConfigNeigh_r10_setup);              

				asn1Init_MeasSubframePatternConfigNeigh_r10_setup(p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup);

				result = uecc_llim_build_measSubframePatternNeigh_r10(p_uecc_ue_context->p_gb_context,
						&p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.
						meas_subframe_pattern_config_neigh_r10_setup.meas_subframe_pattern_neigh_r10,
						p_asn1_ctx,
						&p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup->measSubframePatternNeigh_r10);   
				if(RRC_SUCCESS != result)
				{
					break;
				}

				if(p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.
						meas_subframe_pattern_config_neigh_r10_setup.bitmask &
						MEAS_SUBFRAME_CELL_LIST_R10_PRESENT)
				{
					p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup->m.measSubframeCellList_r10Present = 1;
					result = uecc_llim_build_MeasSubframeCellList_r10(
							p_uecc_ue_context->p_gb_context,
							&p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.
							meas_subframe_pattern_config_neigh_r10_setup.meas_subframe_cell_list_r10,
							p_asn1_ctx,
							&p_meas_object_eutra->measSubframePatternConfigNeigh_r10.u.setup->measSubframeCellList_r10);
					if(RRC_FAILURE == result)
					{
						break;
					}

				}
			}
            else if(p_rrm_meas_object_eutra->meas_subframe_pattern_config_neigh_r10.bitmask &
                    MEAS_SUBFRAME_PATTERN_CONFIG_NEIGH_R10_RELEASE_PRESENT)
            {
				p_meas_object_eutra->measSubframePatternConfigNeigh_r10.t = T_MeasSubframePatternConfigNeigh_r10_release;
            }

		}
		/* eICIC meas changes stop */
		/* OSRTDList */
		rtxDListInit(&p_meas_object_eutra->extElem1);
		result = RRC_SUCCESS;
	}
	while(0);
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}



/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CellsToAddModListCDMA2000
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  cells_to_add_mod_list_cdma2000_list_t*
 *                  p_rrm_cells_to_add_mod_list_cdma2000_list
 *                  OSCTXT*                                 p_asn1_ctx
 *                  CellsToAddModListCDMA2000*              p_cells_to_add_mod_list_cdma2000
 *   OUTPUT      : none
 *   DESCRIPTION:
 *       This function builds CellsToAddModCDMA2000 in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CellsToAddModListCDMA2000(
		uecc_gb_context_t               *p_uecc_gb_context,
		cells_to_add_mod_list_cdma2000_list_t*
		p_rrm_cells_to_add_mod_list_cdma2000_list,
		OSCTXT*                                 p_asn1_ctx,
		CellsToAddModListCDMA2000*              p_cells_to_add_mod_list_cdma2000
		)
{
	rrc_return_et           result = RRC_SUCCESS;
	CellsToAddModCDMA2000*  p_cells_add_mod_cdma2000 = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 i=0;

	RRC_ASSERT(PNULL != p_rrm_cells_to_add_mod_list_cdma2000_list);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_cells_to_add_mod_list_cdma2000);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/
	rtxDListInit(p_cells_to_add_mod_list_cdma2000);
	for (i=0; i < p_rrm_cells_to_add_mod_list_cdma2000_list->count; i++)
	{
		rtxDListAllocNodeAndData(p_asn1_ctx, CellsToAddModCDMA2000,
				&p_node, &p_cells_add_mod_cdma2000);
		if (PNULL == p_node)
		{
			result = RRC_FAILURE;
			break;
		}

		p_cells_add_mod_cdma2000->cellIndex =
			p_rrm_cells_to_add_mod_list_cdma2000_list->
			cells_to_add_mod_cdma2000[i].cell_index;
		p_cells_add_mod_cdma2000->physCellId =
			p_rrm_cells_to_add_mod_list_cdma2000_list->
			cells_to_add_mod_cdma2000[i].phys_cell_id;

		/* append node to the list */
		rtxDListAppendNode(p_cells_to_add_mod_list_cdma2000, p_node);
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CurrCellsToAddModListCDMA2000
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  cells_to_add_mod_list_cdma2000_list_t*
 *                  p_rrm_cells_to_add_mod_list_cdma2000_list
 *                  OSCTXT*                                 p_asn1_ctx
 *                  CellsToAddModListCDMA2000*              p_cells_to_add_mod_list_cdma2000
 *   OUTPUT      : none
 *
 *   DESCRIPTION:
 *       This function builds CellsToAddModCDMA2000 in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CurrCellsToAddModListCDMA2000(
		uecc_gb_context_t           *p_uecc_gb_context,
		cells_to_add_mod_list_cdma2000_list_t*
		p_rrm_cells_to_add_mod_list_cdma2000_list,
		OSCTXT*                                 p_asn1_ctx,
		CellsToAddModListCDMA2000*              p_cells_to_add_mod_list_cdma2000
		)
{
	rrc_return_et           result = RRC_SUCCESS;
	CellsToAddModCDMA2000*  p_cells_add_mod_cdma2000 = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 i=0;

	RRC_ASSERT(PNULL != p_rrm_cells_to_add_mod_list_cdma2000_list);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_cells_to_add_mod_list_cdma2000);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/
	rtxDListInit(p_cells_to_add_mod_list_cdma2000);
	for (i=0; i < MAX_CELL_MEAS; i++)
	{
		if (p_rrm_cells_to_add_mod_list_cdma2000_list->
				cells_to_add_mod_cdma2000[i].cell_index != 0)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx, CellsToAddModCDMA2000,
					&p_node, &p_cells_add_mod_cdma2000);
			if (PNULL == p_node)
			{
				result = RRC_FAILURE;
				break;
			}

			p_cells_add_mod_cdma2000->cellIndex =
				p_rrm_cells_to_add_mod_list_cdma2000_list->
				cells_to_add_mod_cdma2000[i].cell_index;
			p_cells_add_mod_cdma2000->physCellId =
				p_rrm_cells_to_add_mod_list_cdma2000_list->
				cells_to_add_mod_cdma2000[i].phys_cell_id;

			/* append node to the list */
			rtxDListAppendNode(p_cells_to_add_mod_list_cdma2000, p_node);
		}
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CellsToAddModListEUTRA
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  cells_to_add_mod_list_t*   p_rrm_cells_to_add_mod_list_eutra_list
 *                  OSCTXT*                    p_asn1_ctx
 *                  CellsToAddModList*         p_cells_to_add_mod_list_eutra
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds CellsToAddModEUTRA in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CellsToAddModListEUTRA(
		uecc_gb_context_t               *p_uecc_gb_context,
		cells_to_add_mod_list_t*   p_rrm_cells_to_add_mod_list_eutra_list,
		OSCTXT*                    p_asn1_ctx,
		CellsToAddModList*         p_cells_to_add_mod_list_eutra)
{
	rrc_return_et           result = RRC_SUCCESS;
	CellsToAddMod*          p_cells_add_mod_eutra = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_cells_to_add_mod_list_eutra_list);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_cells_to_add_mod_list_eutra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/
	rtxDListInit(p_cells_to_add_mod_list_eutra);
	for (count = 0; count < p_rrm_cells_to_add_mod_list_eutra_list->count; count++)
	{
		rtxDListAllocNodeAndData(
				p_asn1_ctx, CellsToAddMod,
				&p_node, 
				&p_cells_add_mod_eutra);
		if (PNULL == p_node)
		{
			result = RRC_FAILURE;
			break;
		}

		p_cells_add_mod_eutra->cellIndex =
			p_rrm_cells_to_add_mod_list_eutra_list->
			cells_to_add_mod[count].cell_index;
		p_cells_add_mod_eutra->physCellId =
			p_rrm_cells_to_add_mod_list_eutra_list->
			cells_to_add_mod[count].phys_cell_id;
		p_cells_add_mod_eutra->cellIndividualOffset =
			p_rrm_cells_to_add_mod_list_eutra_list->
			cells_to_add_mod[count].cell_individual_offset;

		/* append node to the list */
		rtxDListAppendNode(p_cells_to_add_mod_list_eutra, p_node);
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CurrCellsToAddModListEUTRA
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  cells_to_add_mod_list_t*   p_rrm_cells_to_add_mod_list_eutra_list
 *                  OSCTXT*                    p_asn1_ctx
 *                  CellsToAddModList*         p_cells_to_add_mod_list_eutra
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds CellsToAddModEUTRA in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CurrCellsToAddModListEUTRA(
		uecc_gb_context_t               *p_uecc_gb_context,
		cells_to_add_mod_list_t*   p_rrm_cells_to_add_mod_list_eutra_list,
		OSCTXT*                    p_asn1_ctx,
		CellsToAddModList*         p_cells_to_add_mod_list_eutra)
{
	rrc_return_et           result = RRC_SUCCESS;
	CellsToAddMod*          p_cells_add_mod_eutra = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_cells_to_add_mod_list_eutra_list);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_cells_to_add_mod_list_eutra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/
	rtxDListInit(p_cells_to_add_mod_list_eutra);
	for (count = 0; 
			count < p_rrm_cells_to_add_mod_list_eutra_list->count; 
			count++)
	{
		if (p_rrm_cells_to_add_mod_list_eutra_list->
				cells_to_add_mod[count].cell_index != 0)
		{
			rtxDListAllocNodeAndData(
					p_asn1_ctx, CellsToAddMod,
					&p_node, 
					&p_cells_add_mod_eutra);
			if (PNULL == p_node)
			{
				result = RRC_FAILURE;
				break;
			}

			p_cells_add_mod_eutra->cellIndex =
				p_rrm_cells_to_add_mod_list_eutra_list->
				cells_to_add_mod[count].cell_index;
			p_cells_add_mod_eutra->physCellId =
				p_rrm_cells_to_add_mod_list_eutra_list->
				cells_to_add_mod[count].phys_cell_id;
			p_cells_add_mod_eutra->cellIndividualOffset =
				p_rrm_cells_to_add_mod_list_eutra_list->
				cells_to_add_mod[count].cell_individual_offset;

			/* append node to the list */
			rtxDListAppendNode(p_cells_to_add_mod_list_eutra, p_node);
		}
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}

/*****************************************************************************
 *   FUNCTION NAME: uecc_llim_build_BlackListedCellsToAddModListEUTRA
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  black_cells_to_add_mod_list_t* p_rrm_black_cells_to_add_mod_list_eutra_list
 *                  OSCTXT*                        p_asn1_ctx
 *                  BlackCellsToAddModList*        p_black_cells_to_add_mod_list_eutra
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds CellsToAddModEUTRA in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_BlackListedCellsToAddModListEUTRA(
		uecc_gb_context_t               *p_uecc_gb_context,
		black_cells_to_add_mod_list_t* p_rrm_black_cells_to_add_mod_list_eutra_list,
		OSCTXT*                        p_asn1_ctx,
		BlackCellsToAddModList*        p_black_cells_to_add_mod_list_eutra)
{
	rrc_return_et           result = RRC_SUCCESS;
	BlackCellsToAddMod*     p_black_cells_add_mod_eutra = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 count=0;

	RRC_ASSERT(PNULL != p_rrm_black_cells_to_add_mod_list_eutra_list);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_black_cells_to_add_mod_list_eutra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);
    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/

	rtxDListInit(p_black_cells_to_add_mod_list_eutra);
	for (count=0; count < p_rrm_black_cells_to_add_mod_list_eutra_list->count; count++)
	{
		rtxDListAllocNodeAndData(
				p_asn1_ctx,
				BlackCellsToAddMod,
				&p_node,
				&p_black_cells_add_mod_eutra);
		if (PNULL == p_node)
		{
			result = RRC_FAILURE;
			break;
		}
		/* SPR_6255 Start */
		asn1Init_BlackCellsToAddMod (p_black_cells_add_mod_eutra);
		/* SPR_6255 Stop */

		p_black_cells_add_mod_eutra->cellIndex =
			p_rrm_black_cells_to_add_mod_list_eutra_list->
			black_cells_to_add_mod[count].cell_index;

		p_black_cells_add_mod_eutra->physCellIdRange.m.rangePresent = 0;    

		/* SPR_6255 Start */
		p_black_cells_add_mod_eutra->physCellIdRange.start =
			p_rrm_black_cells_to_add_mod_list_eutra_list->
			black_cells_to_add_mod[count].phys_cell_id_range.start;
		/* SPR_6255 Stop */
		if (PHY_CELL_ID_RANGE & p_rrm_black_cells_to_add_mod_list_eutra_list->
				black_cells_to_add_mod[count].phys_cell_id_range.presence_bitmask)
		{
			p_black_cells_add_mod_eutra->physCellIdRange.m.rangePresent = 1;    


			p_black_cells_add_mod_eutra->physCellIdRange.range =
				p_rrm_black_cells_to_add_mod_list_eutra_list->
				black_cells_to_add_mod[count].phys_cell_id_range.range;
		}   

		/* append node to the list */
		rtxDListAppendNode(p_black_cells_to_add_mod_list_eutra, p_node);
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}
/*****************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CurrBlackListedCellsToAddModListEUTRA
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  black_cells_to_add_mod_list_t* p_rrm_black_cells_to_add_mod_list_eutra_list
 *                  OSCTXT*                        p_asn1_ctx
 *                  BlackCellsToAddModList*        p_black_cells_to_add_mod_list_eutra
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds CellsToAddModEUTRA in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CurrBlackListedCellsToAddModListEUTRA(
		uecc_gb_context_t               *p_uecc_gb_context,
		black_cells_to_add_mod_list_t* p_rrm_black_cells_to_add_mod_list_eutra_list,
		OSCTXT*                        p_asn1_ctx,
		BlackCellsToAddModList*        p_black_cells_to_add_mod_list_eutra)
{
	rrc_return_et           result = RRC_SUCCESS;
	BlackCellsToAddMod*     p_black_cells_add_mod_eutra = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 count=0;

	RRC_ASSERT(PNULL != p_rrm_black_cells_to_add_mod_list_eutra_list);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_black_cells_to_add_mod_list_eutra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable p_uecc_gb_context=%p",p_uecc_gb_context);
	rtxDListInit(p_black_cells_to_add_mod_list_eutra);
	for (count=0; count < MAX_CELL_MEAS; count++)
	{
		if(p_rrm_black_cells_to_add_mod_list_eutra_list->
				black_cells_to_add_mod[count].cell_index != 0)
		{
			rtxDListAllocNodeAndData(
					p_asn1_ctx,
					BlackCellsToAddMod,
					&p_node,
					&p_black_cells_add_mod_eutra);
			if (PNULL == p_node)
			{
				result = RRC_FAILURE;
				break;
			}
			/* SPR_6255 Start */
			asn1Init_BlackCellsToAddMod (p_black_cells_add_mod_eutra);
			/* SPR_6255 Stop */

			p_black_cells_add_mod_eutra->cellIndex =
				p_rrm_black_cells_to_add_mod_list_eutra_list->
				black_cells_to_add_mod[count].cell_index;
			/* SPR_6255 Start */
			p_black_cells_add_mod_eutra->physCellIdRange.start =
				p_rrm_black_cells_to_add_mod_list_eutra_list->
				black_cells_to_add_mod[count].phys_cell_id_range.start;
			/* SPR_6255 Stop */
			if (PHY_CELL_ID_RANGE & p_rrm_black_cells_to_add_mod_list_eutra_list->
					black_cells_to_add_mod[count].phys_cell_id_range.presence_bitmask)
			{
				p_black_cells_add_mod_eutra->physCellIdRange.m.rangePresent = 1;    


				p_black_cells_add_mod_eutra->physCellIdRange.range =
					p_rrm_black_cells_to_add_mod_list_eutra_list->
					black_cells_to_add_mod[count].phys_cell_id_range.range;
			}   

			/* append node to the list */
			rtxDListAppendNode(p_black_cells_to_add_mod_list_eutra, p_node);
		}
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}


/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_MeasObjectUTRA
 *   INPUT        : uecc_ue_context_t*          p_uecc_ue_context
 *                  meas_object_utra_t*         p_rrm_meas_object_utra
 *                  OSCTXT*                     p_asn1_ctx
 *                  MeasObjectUTRA*             p_meas_object_utra
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds MeasObjectUTRA in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_MeasObjectUTRA(
		uecc_ue_context_t*          p_uecc_ue_context,
		meas_object_utra_t*         p_rrm_meas_object_utra,
		OSCTXT*                     p_asn1_ctx,
		MeasObjectUTRA*             p_meas_object_utra)
{
	rrc_return_et   result = RRC_FAILURE;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_meas_object_utra);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_utra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_MeasObjectUTRA(p_meas_object_utra);

		/* CellIndexList */
		if (MEAS_OBJECT_UTRA_CELLS_TO_REMOVE_LIST_PRESENT &
				p_rrm_meas_object_utra->bitmask)
		{
			/* check list size */
			if ((p_rrm_meas_object_utra->cells_to_remove_list.count >
						ARRSIZE(p_meas_object_utra->cellsToRemoveList.elem)) ||
					(p_rrm_meas_object_utra->cells_to_remove_list.count < 1) )
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,
						"uecc_llim_build_MeasObjectUTRA wrong list count [%u]",
						p_rrm_meas_object_utra->cells_to_remove_list.count);
				break;
			}

			p_meas_object_utra->m.cellsToRemoveListPresent = 1;

			p_meas_object_utra->cellsToRemoveList.n =
				p_rrm_meas_object_utra->cells_to_remove_list.count;

			for (count=0; count < p_meas_object_utra->cellsToRemoveList.n; count++)
			{
				p_meas_object_utra->cellsToRemoveList.elem[count] =
					p_rrm_meas_object_utra->cells_to_remove_list.cell_index[count];
			}
		}

		/* CellsToAddModListUTRA */
		if (MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_PRESENT &
				p_rrm_meas_object_utra->bitmask)
		{
			p_meas_object_utra->m.cellsToAddModListPresent = 1;

			if ( MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_FDD_PRESENT &
					p_rrm_meas_object_utra->cells_to_add_mod_list.bitmask )
			{
				/* fill list of CellsToAddModUTRA-FDD */
				result = uecc_llim_build_CellsToAddModListUTRA_FDD(
						p_uecc_ue_context->p_gb_context, 
						&p_rrm_meas_object_utra->cells_to_add_mod_list.
						cells_to_add_mod_list_utra_fdd,
						p_asn1_ctx,
						p_meas_object_utra);
				if (RRC_FAILURE == result)
				{
					break;
				}
			}
			if ( MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_TDD_PRESENT &
					p_rrm_meas_object_utra->cells_to_add_mod_list.bitmask )
			{
				/* fill list of CellsToAddModUTRA-TDD */
				result = uecc_llim_build_CellsToAddModListUTRA_TDD(
						p_uecc_ue_context->p_gb_context, 
						&p_rrm_meas_object_utra->cells_to_add_mod_list.
						cells_to_add_mod_list_utra_tdd,
						p_asn1_ctx,
						p_meas_object_utra);
				if (RRC_FAILURE == result)
				{
					break;
				}
			}
		}

		/* PhysCellIdUTRA */
		if (MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT &
				p_rrm_meas_object_utra->bitmask)
		{
			p_meas_object_utra->m.cellForWhichToReportCGIPresent = 1;

			if (MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_UTRA_FDD_PRESENT &
					p_rrm_meas_object_utra->cell_for_which_to_report_cgi.bitmask)
			{
				p_meas_object_utra->cellForWhichToReportCGI.t = 
					T_MeasObjectUTRA_cellForWhichToReportCGI_utra_FDD;

				p_meas_object_utra->cellForWhichToReportCGI.u.utra_FDD =
					p_rrm_meas_object_utra->cell_for_which_to_report_cgi.utra_fdd;
			}

			if (MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_UTRA_TDD_PRESENT &
					p_rrm_meas_object_utra->cell_for_which_to_report_cgi.bitmask)
			{
				p_meas_object_utra->cellForWhichToReportCGI.t = 
					T_MeasObjectUTRA_cellForWhichToReportCGI_utra_TDD;

				p_meas_object_utra->cellForWhichToReportCGI.u.utra_TDD =
					p_rrm_meas_object_utra->cell_for_which_to_report_cgi.utra_tdd;
			}
		}

		p_meas_object_utra->carrierFreq =
			p_rrm_meas_object_utra->carrier_freq;

		p_meas_object_utra->offsetFreq =
			p_rrm_meas_object_utra->offset_freq;

		/* OSRTDList */
		rtxDListInit(&p_meas_object_utra->extElem1);

		result = RRC_SUCCESS;
	}
	while(0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CurrMeasObjectUTRA
 *   INPUT        : uecc_ue_context_t*          p_uecc_ue_context
 *                  meas_object_utra_t*         p_rrm_meas_object_utra
 *                  OSCTXT*                     p_asn1_ctx
 *                  MeasObjectUTRA*             p_meas_object_utra
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds MeasObjectUTRA in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CurrMeasObjectUTRA(
		uecc_ue_context_t*              p_uecc_ue_context,
		meas_object_utra_t*         p_rrm_meas_object_utra,
		OSCTXT*                     p_asn1_ctx,
		MeasObjectUTRA*             p_meas_object_utra)
{
	rrc_return_et   result = RRC_FAILURE;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_meas_object_utra);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_utra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_MeasObjectUTRA(p_meas_object_utra);

		/* CellIndexList */
		if (MEAS_OBJECT_UTRA_CELLS_TO_REMOVE_LIST_PRESENT &
				p_rrm_meas_object_utra->bitmask)
		{
			/* check list size */
			if ((p_rrm_meas_object_utra->cells_to_remove_list.count >
						ARRSIZE(p_meas_object_utra->cellsToRemoveList.elem)) ||
					(p_rrm_meas_object_utra->cells_to_remove_list.count < 1) )
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,
						"uecc_llim_build_MeasObjectUTRA wrong list count [%u]",
						p_rrm_meas_object_utra->cells_to_remove_list.count);
				break;
			}

			p_meas_object_utra->m.cellsToRemoveListPresent = 1;

			p_meas_object_utra->cellsToRemoveList.n =
				p_rrm_meas_object_utra->cells_to_remove_list.count;

			for (count=0; count < p_meas_object_utra->cellsToRemoveList.n; count++)
			{
				p_meas_object_utra->cellsToRemoveList.elem[count] =
					p_rrm_meas_object_utra->cells_to_remove_list.cell_index[count];
			}
		}

		/* CellsToAddModListUTRA */
		if (MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_PRESENT &
				p_rrm_meas_object_utra->bitmask)
		{
			p_meas_object_utra->m.cellsToAddModListPresent = 1;

			if ( MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_FDD_PRESENT &
					p_rrm_meas_object_utra->cells_to_add_mod_list.bitmask )
			{
				/* fill list of CellsToAddModUTRA-FDD */
				result = uecc_llim_build_CurrCellsToAddModListUTRA_FDD(
						p_uecc_ue_context->p_gb_context, 
						&p_rrm_meas_object_utra->cells_to_add_mod_list.
						cells_to_add_mod_list_utra_fdd,
						p_asn1_ctx,
						p_meas_object_utra);
				if (RRC_FAILURE == result)
				{
					break;
				}
			}
			if ( MEAS_OBJECT_UTRA_CELLS_TO_ADD_MOD_LIST_TDD_PRESENT &
					p_rrm_meas_object_utra->cells_to_add_mod_list.bitmask )
			{
				/* fill list of CellsToAddModUTRA-TDD */
				result = uecc_llim_build_CurrCellsToAddModListUTRA_TDD(
						p_uecc_ue_context->p_gb_context, 
						&p_rrm_meas_object_utra->cells_to_add_mod_list.
						cells_to_add_mod_list_utra_tdd,
						p_asn1_ctx,
						p_meas_object_utra);
				if (RRC_FAILURE == result)
				{
					break;
				}
			}
		}

		/* PhysCellIdUTRA */
		if (MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT &
				p_rrm_meas_object_utra->bitmask)
		{
			p_meas_object_utra->m.cellForWhichToReportCGIPresent = 1;

			if (MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_UTRA_FDD_PRESENT &
					p_rrm_meas_object_utra->cell_for_which_to_report_cgi.bitmask)
			{
				p_meas_object_utra->cellForWhichToReportCGI.t = 
					T_MeasObjectUTRA_cellForWhichToReportCGI_utra_FDD;

				p_meas_object_utra->cellForWhichToReportCGI.u.utra_FDD =
					p_rrm_meas_object_utra->cell_for_which_to_report_cgi.utra_fdd;
			}

			if (MEAS_OBJECT_UTRA_CELL_FOR_WHICH_TO_REPORT_CGI_UTRA_TDD_PRESENT &
					p_rrm_meas_object_utra->cell_for_which_to_report_cgi.bitmask)
			{
				p_meas_object_utra->cellForWhichToReportCGI.t = 
					T_MeasObjectUTRA_cellForWhichToReportCGI_utra_TDD;

				p_meas_object_utra->cellForWhichToReportCGI.u.utra_TDD =
					p_rrm_meas_object_utra->cell_for_which_to_report_cgi.utra_tdd;
			}
		}

		p_meas_object_utra->carrierFreq =
			p_rrm_meas_object_utra->carrier_freq;

		p_meas_object_utra->offsetFreq =
			p_rrm_meas_object_utra->offset_freq;

		/* OSRTDList */
		rtxDListInit(&p_meas_object_utra->extElem1);

		result = RRC_SUCCESS;
	}
	while(0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}


/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CellsToAddModListUTRA_FDD
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  cells_to_add_mod_list_utra_fdd_t* p_rrm_cells_to_add_mod_list_utra_fdd
 *                  OSCTXT*                     p_asn1_ctx
 *                  MeasObjectUTRA*             p_meas_object_utra
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds CellsToAddModUTRA-FDD in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CellsToAddModListUTRA_FDD(
		uecc_gb_context_t               *p_uecc_gb_context,
		cells_to_add_mod_list_utra_fdd_t* p_rrm_cells_to_add_mod_list_utra_fdd,
		OSCTXT*                           p_asn1_ctx,
		MeasObjectUTRA*                   p_meas_object_utra)
{
	rrc_return_et           result = RRC_SUCCESS;
	CellsToAddModUTRA_FDD*  p_cells_add_mod_utra_fdd = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_cells_to_add_mod_list_utra_fdd);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_utra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/
	p_meas_object_utra->cellsToAddModList.t = 
		T_MeasObjectUTRA_cellsToAddModList_cellsToAddModListUTRA_FDD;

	p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_FDD =
		rtxMemAllocType(p_asn1_ctx,
				CellsToAddModListUTRA_FDD);
	/* SPR 12964 Fix Start */
	if (PNULL == p_meas_object_utra->cellsToAddModList.u.
			cellsToAddModListUTRA_FDD)
	{
		RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
		return RRC_FAILURE;
	}
	/* SPR 12964 Fix Stop */

	rtxDListInit(p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_FDD);
	for (count = 0; count < p_rrm_cells_to_add_mod_list_utra_fdd->count; count++)
	{
		rtxDListAllocNodeAndData(
				p_asn1_ctx,
				CellsToAddModUTRA_FDD,
				&p_node, 
				&p_cells_add_mod_utra_fdd);
		if (PNULL == p_node)
		{
			result = RRC_FAILURE;
			break;
		}
		p_cells_add_mod_utra_fdd->cellIndex =
			p_rrm_cells_to_add_mod_list_utra_fdd->
			cells_to_add_mod_utra_fdd[count].cell_index;
		p_cells_add_mod_utra_fdd->physCellId =
			p_rrm_cells_to_add_mod_list_utra_fdd->
			cells_to_add_mod_utra_fdd[count].phys_cell_id;

		/* append node to the list */
		rtxDListAppendNode(p_meas_object_utra->cellsToAddModList.u.
				cellsToAddModListUTRA_FDD,
				p_node);
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CurrCellsToAddModListUTRA_FDD
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  cells_to_add_mod_list_utra_fdd_t* p_rrm_cells_to_add_mod_list_utra_fdd
 *                  OSCTXT*                     p_asn1_ctx
 *                  MeasObjectUTRA*             p_meas_object_utra
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds CellsToAddModUTRA-FDD in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CurrCellsToAddModListUTRA_FDD(
		uecc_gb_context_t                 *p_uecc_gb_context,
		cells_to_add_mod_list_utra_fdd_t* p_rrm_cells_to_add_mod_list_utra_fdd,
		OSCTXT*                           p_asn1_ctx,
		MeasObjectUTRA*                   p_meas_object_utra)
{
	rrc_return_et           result = RRC_SUCCESS;
	CellsToAddModUTRA_FDD*  p_cells_add_mod_utra_fdd = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_cells_to_add_mod_list_utra_fdd);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_utra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/
	p_meas_object_utra->cellsToAddModList.t = 
		T_MeasObjectUTRA_cellsToAddModList_cellsToAddModListUTRA_FDD;

	p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_FDD =
		rtxMemAllocType(p_asn1_ctx,
				CellsToAddModListUTRA_FDD);
	/* SPR 12964 Fix Start */
	if (PNULL == p_meas_object_utra->cellsToAddModList.u.
			cellsToAddModListUTRA_FDD)
	{
		RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
		return RRC_FAILURE;
	}
	/* SPR 12964 Fix Stop */

	rtxDListInit(p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_FDD);
	/*for (count = 0; count < p_rrm_cells_to_add_mod_list_utra_fdd->count; count++)*/
	for (count = 0; count < MAX_CELL_MEAS; count++)
	{
		if (p_rrm_cells_to_add_mod_list_utra_fdd->
				cells_to_add_mod_utra_fdd[count].cell_index != 0)
		{
			rtxDListAllocNodeAndData(
					p_asn1_ctx,
					CellsToAddModUTRA_FDD,
					&p_node, 
					&p_cells_add_mod_utra_fdd);
			if (PNULL == p_node)
			{
				result = RRC_FAILURE;
				break;
			}
			p_cells_add_mod_utra_fdd->cellIndex =
				p_rrm_cells_to_add_mod_list_utra_fdd->
				cells_to_add_mod_utra_fdd[count].cell_index;
			p_cells_add_mod_utra_fdd->physCellId =
				p_rrm_cells_to_add_mod_list_utra_fdd->
				cells_to_add_mod_utra_fdd[count].phys_cell_id;

			/* append node to the list */
			rtxDListAppendNode(p_meas_object_utra->cellsToAddModList.u.
					cellsToAddModListUTRA_FDD,
					p_node);
		}
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CellsToAddModListUTRA_TDD
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  cells_to_add_mod_list_utra_fdd_t* p_rrm_cells_to_add_mod_list_utra_fdd
 *                  OSCTXT*                     p_asn1_ctx
 *                  MeasObjectUTRA*             p_meas_object_utra
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds CellsToAddModUTRA-TDD in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CellsToAddModListUTRA_TDD(
		uecc_gb_context_t                 *p_uecc_gb_context,
		cells_to_add_mod_list_utra_tdd_t* p_rrm_cells_to_add_mod_list_utra_tdd,
		OSCTXT*                           p_asn1_ctx,
		MeasObjectUTRA*                   p_meas_object_utra)
{
	rrc_return_et           result = RRC_SUCCESS;
	CellsToAddModUTRA_TDD*  p_cells_add_mod_utra_tdd = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_cells_to_add_mod_list_utra_tdd);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_utra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/

	p_meas_object_utra->cellsToAddModList.t = 
		T_MeasObjectUTRA_cellsToAddModList_cellsToAddModListUTRA_TDD;

	p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_TDD =
		rtxMemAllocType(
				p_asn1_ctx,
				CellsToAddModListUTRA_TDD);
	/* SPR 12964 Fix Start */
	if (PNULL == p_meas_object_utra->cellsToAddModList.u.
			cellsToAddModListUTRA_TDD)
	{
		RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
		return RRC_FAILURE;
	}
	/* SPR 12964 Fix Stop */

	rtxDListInit(p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_TDD);
	for (count = 0; count < p_rrm_cells_to_add_mod_list_utra_tdd->count; count++)
	{
		rtxDListAllocNodeAndData(p_asn1_ctx, CellsToAddModUTRA_TDD,
				&p_node, &p_cells_add_mod_utra_tdd);
		if (PNULL == p_node)
		{
			result = RRC_FAILURE;
			break;
		}
		p_cells_add_mod_utra_tdd->cellIndex =
			p_rrm_cells_to_add_mod_list_utra_tdd->
			cells_to_add_mod_utra_tdd[count].cell_index;
		p_cells_add_mod_utra_tdd->physCellId =
			p_rrm_cells_to_add_mod_list_utra_tdd->
			cells_to_add_mod_utra_tdd[count].phys_cell_id;

		/* append node to the list */
		rtxDListAppendNode(
				p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_TDD, 
				p_node);
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_CurrCellsToAddModListUTRA_TDD
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  cells_to_add_mod_list_utra_fdd_t* p_rrm_cells_to_add_mod_list_utra_fdd
 *                  OSCTXT*                     p_asn1_ctx
 *                  MeasObjectUTRA*             p_meas_object_utra
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds CellsToAddModUTRA-TDD in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_CurrCellsToAddModListUTRA_TDD(
		uecc_gb_context_t                 *p_uecc_gb_context,
		cells_to_add_mod_list_utra_tdd_t* p_rrm_cells_to_add_mod_list_utra_tdd,
		OSCTXT*                           p_asn1_ctx,
		MeasObjectUTRA*                   p_meas_object_utra)
{
	rrc_return_et           result = RRC_SUCCESS;
	CellsToAddModUTRA_TDD*  p_cells_add_mod_utra_tdd = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8 count = 0;

	RRC_ASSERT(PNULL != p_rrm_cells_to_add_mod_list_utra_tdd);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_utra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);/*SPR 17777 +-*/

	p_meas_object_utra->cellsToAddModList.t = 
		T_MeasObjectUTRA_cellsToAddModList_cellsToAddModListUTRA_TDD;

	p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_TDD =
		rtxMemAllocType(
				p_asn1_ctx,
				CellsToAddModListUTRA_TDD);
	/* SPR 12964 Fix Start */
	if (PNULL == p_meas_object_utra->cellsToAddModList.u.
			cellsToAddModListUTRA_TDD)
	{
		RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
		return RRC_FAILURE;
	}
	/* SPR 12964 Fix Stop */

	rtxDListInit(p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_TDD);
	for (count = 0; count < MAX_CELL_MEAS; count++)
	{
		if (p_rrm_cells_to_add_mod_list_utra_tdd->
				cells_to_add_mod_utra_tdd[count].cell_index != 0)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx, CellsToAddModUTRA_TDD,
					&p_node, &p_cells_add_mod_utra_tdd);
			if (PNULL == p_node)
			{
				result = RRC_FAILURE;
				break;
			}
			p_cells_add_mod_utra_tdd->cellIndex =
				p_rrm_cells_to_add_mod_list_utra_tdd->
				cells_to_add_mod_utra_tdd[count].cell_index;
			p_cells_add_mod_utra_tdd->physCellId =
				p_rrm_cells_to_add_mod_list_utra_tdd->
				cells_to_add_mod_utra_tdd[count].phys_cell_id;

			/* append node to the list */
			rtxDListAppendNode(
					p_meas_object_utra->cellsToAddModList.u.cellsToAddModListUTRA_TDD, 
					p_node);
		}
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_ReportConfigToAddMod
 *   INPUT        : uecc_ue_context_t*          p_uecc_ue_context
 *                  report_config_to_add_mod_t* p_rrm_report_config_to_add_mod
 *                  OSCTXT*                     p_asn1_ctx
 *                  ReportConfigToAddMod*       p_report_config_to_add_mod
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds ReportConfigToAddMod in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_ReportConfigToAddMod(
		uecc_ue_context_t*          p_uecc_ue_context,
		report_config_to_add_mod_t* p_rrm_report_config_to_add_mod,
		OSCTXT*                     p_asn1_ctx,
		ReportConfigToAddMod*       p_report_config_to_add_mod
		)
{
	rrc_return_et               result = RRC_FAILURE;
	rrc_bool_et    report_cfg_eutra_ext_present = RRC_FALSE;

	RRC_ASSERT(PNULL != p_rrm_report_config_to_add_mod);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_report_config_to_add_mod);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	asn1Init_ReportConfigToAddMod(p_report_config_to_add_mod);
	p_report_config_to_add_mod->reportConfigId =
		p_rrm_report_config_to_add_mod->report_config_id;

	switch(p_rrm_report_config_to_add_mod->report_config.bitmask & 0x03)
	{
		case REPORT_CONFIG_EUTRA_PRESENT:
			p_report_config_to_add_mod->reportConfig.t =
				T_ReportConfigToAddMod_reportConfig_reportConfigEUTRA;

			p_report_config_to_add_mod->reportConfig.u.reportConfigEUTRA =
				rtxMemAllocType(p_asn1_ctx, ReportConfigEUTRA);
			if (PNULL ==
					p_report_config_to_add_mod->reportConfig.u.reportConfigEUTRA)
			{
				break;
			}

			if (REPORT_CONFIG_EUTRA_EXT_PRESENT & 
					p_rrm_report_config_to_add_mod->report_config.bitmask)
			{
				report_cfg_eutra_ext_present = RRC_TRUE;
			}

			result = uecc_llim_build_ReportConfigEUTRA(
					p_uecc_ue_context,
					&p_rrm_report_config_to_add_mod->
					report_config.report_config_eutra,
					p_asn1_ctx,
					p_report_config_to_add_mod->reportConfig.u.reportConfigEUTRA,
					report_cfg_eutra_ext_present,
					&p_rrm_report_config_to_add_mod->report_config.report_config_eutra_ext);

			break;

		case REPORT_CONFIG_INTERRAT_PRESENT:
			p_report_config_to_add_mod->reportConfig.t =
				T_ReportConfigToAddMod_reportConfig_reportConfigInterRAT;

			p_report_config_to_add_mod->reportConfig.u.reportConfigInterRAT =
				rtxMemAllocType(p_asn1_ctx, ReportConfigInterRAT);
			if (PNULL ==
					p_report_config_to_add_mod->reportConfig.u.reportConfigInterRAT)
			{
				break;
			}
			/* SPR:7241 start */
			asn1Init_ReportConfigInterRAT(p_report_config_to_add_mod->reportConfig.u.reportConfigInterRAT);
			/* SPR:7241 stop */
			result = uecc_llim_build_ReportConfigInterRAT(
					p_uecc_ue_context,
					&p_rrm_report_config_to_add_mod->
					report_config.report_config_interrat,
					p_asn1_ctx,
					p_report_config_to_add_mod->
					reportConfig.u.reportConfigInterRAT);

			break;

		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_ReportConfigToAddMod -> wrong bitmask");
			break;

	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_report_configToAddMod
 *   INPUT        : uecc_ue_context_t*          p_uecc_ue_context
 *                  report_config_to_add_mod_t* p_rrm_report_config_to_add_mod
 *                  OSCTXT*                     p_asn1_ctx
 *                  ReportConfigToAddMod*       p_report_config_to_add_mod
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds ReportConfigToAddMod in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_report_configToAddMod(
		uecc_ue_context_t*          p_uecc_ue_context,
		uecc_report_config_to_add_mod_t* p_rrm_report_config_to_add_mod,
		OSCTXT*                     p_asn1_ctx,
		ReportConfigToAddMod*       p_report_config_to_add_mod
		)
{
	rrc_return_et               result = RRC_FAILURE;
	rrc_bool_et    report_cfg_eutra_ext_present = RRC_FALSE;

	RRC_ASSERT(PNULL != p_rrm_report_config_to_add_mod);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_report_config_to_add_mod);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	p_report_config_to_add_mod->reportConfigId =
		p_rrm_report_config_to_add_mod->report_config_id;

	switch(p_rrm_report_config_to_add_mod->report_config.bitmask & 0x03)
	{
		case REPORT_CONFIG_EUTRA_PRESENT:
			p_report_config_to_add_mod->reportConfig.t =
				T_ReportConfigToAddMod_reportConfig_reportConfigEUTRA;

			p_report_config_to_add_mod->reportConfig.u.reportConfigEUTRA =
				rtxMemAllocType(p_asn1_ctx, ReportConfigEUTRA);
			if (PNULL ==
					p_report_config_to_add_mod->reportConfig.u.reportConfigEUTRA)
			{
				break;
			}

			if (REPORT_CONFIG_EUTRA_EXT_PRESENT & 
					p_rrm_report_config_to_add_mod->report_config.bitmask)
			{
				report_cfg_eutra_ext_present = RRC_TRUE;
			}

			result = uecc_llim_build_ReportConfigEUTRA(
					p_uecc_ue_context,
					&p_rrm_report_config_to_add_mod->
					report_config.report_config_eutra,
					p_asn1_ctx,
					p_report_config_to_add_mod->reportConfig.u.reportConfigEUTRA,
					report_cfg_eutra_ext_present,
					&p_rrm_report_config_to_add_mod->report_config.report_config_eutra_ext);

			break;

		case REPORT_CONFIG_INTERRAT_PRESENT:
			p_report_config_to_add_mod->reportConfig.t =
				T_ReportConfigToAddMod_reportConfig_reportConfigInterRAT;

			p_report_config_to_add_mod->reportConfig.u.reportConfigInterRAT =
				rtxMemAllocType(p_asn1_ctx, ReportConfigInterRAT);
			if (PNULL ==
					p_report_config_to_add_mod->reportConfig.u.reportConfigInterRAT)
			{
				break;
			}

			result = uecc_llim_build_ReportConfigInterRAT(
					p_uecc_ue_context,
					&p_rrm_report_config_to_add_mod->
					report_config.report_config_interrat,
					p_asn1_ctx,
					p_report_config_to_add_mod->
					reportConfig.u.reportConfigInterRAT);

			break;

		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_ReportConfigToAddMod -> wrong bitmask");
			break;

	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}


/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_ReportConfigEUTRA
 *   INPUT        : uecc_ue_context_t*          p_uecc_ue_context
 *                  report_config_eutra_t*      p_rrm_report_config_eutra
 *                  OSCTXT*                     p_asn1_ctx
 *                  ReportConfigEUTRA*          p_report_config_eutra
 *                  rrc_bool_et                 report_cfg_eutra_ext_present
 *                  report_config_eutra_ext_t*  p_rrm_report_config_eutra_ext
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds ReportConfigEUTRA in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_ReportConfigEUTRA(
		uecc_ue_context_t*          p_uecc_ue_context,
		report_config_eutra_t*      p_rrm_report_config_eutra,
		OSCTXT*                     p_asn1_ctx,
		ReportConfigEUTRA*          p_report_config_eutra,
		rrc_bool_et                 report_cfg_eutra_ext_present,
		report_config_eutra_ext_t*  p_rrm_report_config_eutra_ext
		)
{
	rrc_return_et result = RRC_FAILURE;
	ReportConfigEUTRA_triggerType_event*            p_type_event = PNULL;
	report_config_eutra_trigger_type_event_t*       p_rrm_type_event = PNULL;

	RRC_ASSERT(PNULL != p_rrm_report_config_eutra);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_report_config_eutra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	asn1Init_ReportConfigEUTRA(p_report_config_eutra);
	p_report_config_eutra->triggerQuantity =
		p_rrm_report_config_eutra->trigger_quantity;
	p_report_config_eutra->reportQuantity =
		p_rrm_report_config_eutra->report_quantity;
	p_report_config_eutra->maxReportCells =
		p_rrm_report_config_eutra->max_report_cells;
	p_report_config_eutra->reportInterval =
		p_rrm_report_config_eutra->report_interval;
	p_report_config_eutra->reportAmount =
		p_rrm_report_config_eutra->report_amount;

	/* ReportConfigEUTRA changes start */
	if ((RRC_TRUE == report_cfg_eutra_ext_present) &&
			(p_rrm_report_config_eutra_ext != PNULL) &&
			(REPORT_CFG_EUTRA_REPORT_ADD_NEIGH_MEAS_PRESENT & 
			 p_rrm_report_config_eutra_ext->bitmask))
	{
		p_report_config_eutra->m._v3ExtPresent = 1;
		p_report_config_eutra->m.reportAddNeighMeas_r10Present = 1;  
		p_report_config_eutra->reportAddNeighMeas_r10 =
			p_rrm_report_config_eutra_ext->report_add_neigh_meas_r10;
	}
	/* ReportConfigEUTRA changes stop */

	/* OSRTDList */
	rtxDListInit(&p_report_config_eutra->extElem1);

	switch(p_rrm_report_config_eutra->trigger_type.bitmask)
	{
		case REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_PRESENT:
			p_report_config_eutra->triggerType.t =
				T_ReportConfigEUTRA_triggerType_event;
			p_report_config_eutra->triggerType.u.event =
				rtxMemAllocType(p_asn1_ctx,ReportConfigEUTRA_triggerType_event);
			if (PNULL == p_report_config_eutra->triggerType.u.event)
			{
				break;
			}

			p_rrm_type_event = &p_rrm_report_config_eutra->trigger_type.event;
			p_type_event = p_report_config_eutra->triggerType.u.event;

			p_type_event->hysteresis = p_rrm_type_event->hysteresis;
			p_type_event->timeToTrigger = p_rrm_type_event->time_to_trigger;

			result=uecc_llim_build_ReportConfigEUTRA_triggerType_event_eventId(
					p_uecc_ue_context, 
					&p_rrm_type_event->event_id,
					p_asn1_ctx,
					&p_type_event->eventId);

			break;

		case REPORT_CONFIG_EUTRA_TRIGGER_TYPE_PERIODICAL_PRESENT:
			p_report_config_eutra->triggerType.t =
				T_ReportConfigEUTRA_triggerType_periodical;
			p_report_config_eutra->triggerType.u.periodical =
				rtxMemAllocType(p_asn1_ctx,
						ReportConfigEUTRA_triggerType_periodical);

			if (PNULL == p_report_config_eutra->triggerType.u.periodical)
			{
				break;
			}

			p_report_config_eutra->triggerType.u.periodical->purpose =
				p_rrm_report_config_eutra->trigger_type.periodical.purpose;

			if ((RRC_TRUE == report_cfg_eutra_ext_present) &&
					(p_rrm_report_config_eutra_ext != PNULL))
			{
				if((REPORT_CFG_EUTRA_UE_RXTX_TIME_DIFF_PRESENT &
							p_rrm_report_config_eutra_ext->bitmask) &&
						(RRM_RRC_REPORT_STRONGEST_CELLS == 
						 p_rrm_report_config_eutra->trigger_type.periodical.purpose))
				{
					p_report_config_eutra->m._v2ExtPresent = 1;
					p_report_config_eutra->m.ue_RxTxTimeDiffPeriodical_r9Present = 1;
					p_report_config_eutra->ue_RxTxTimeDiffPeriodical_r9 = 
						p_rrm_report_config_eutra_ext->ue_rxtx_time_diff;
				}
				/*SPR8897fix_start*/
				else if((SI_REQUEST_FOR_HO_R9_EUTRA_PRESENT & 
							p_rrm_report_config_eutra_ext->bitmask)&&
						(RRM_RRC_REPORT_CGI == 
						 p_rrm_report_config_eutra->trigger_type.periodical.purpose))
				{
					p_report_config_eutra->m._v2ExtPresent = 1;
					p_report_config_eutra->m.si_RequestForHO_r9Present = 1;
					p_report_config_eutra->si_RequestForHO_r9 =
						p_rrm_report_config_eutra_ext->si_request_for_ho_r9;
				}
				/*SPR8897fix_stop*/
			}

			result = RRC_SUCCESS;

			break;

		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_ReportConfigEUTRA -> wrong bitmask");
			break;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_ReportConfigInterRAT
 *   INPUT        : uecc_ue_context_t*          p_uecc_ue_context
 *                  report_config_interrat_t*   p_rrm_report_config_interrat
 *                  OSCTXT*                     p_asn1_ctx
 *                  ReportConfigInterRAT*       p_report_config_interrat
 *   OUTPUT       : none               
 *   DESCRIPTION:
 *       This function builds ReportConfigInterRAT in MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_ReportConfigInterRAT(
		uecc_ue_context_t*          p_uecc_ue_context,
		report_config_interrat_t*   p_rrm_report_config_interrat,
		OSCTXT*                     p_asn1_ctx,
		ReportConfigInterRAT*       p_report_config_interrat
		)
{
	rrc_return_et result = RRC_FAILURE;
	ReportConfigInterRAT_triggerType_event*         p_type_event = PNULL;
	report_config_interrat_trigger_type_event_t*    p_rrm_type_event = PNULL;

	RRC_ASSERT(PNULL != p_rrm_report_config_interrat);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_report_config_interrat);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
	/* SPR:10287 start */
	asn1Init_ReportConfigInterRAT (p_report_config_interrat);
	/* SPR:10287 stop */
	p_report_config_interrat->maxReportCells =
		p_rrm_report_config_interrat->max_report_cells;
	p_report_config_interrat->reportInterval =
		p_rrm_report_config_interrat->report_interval;
	p_report_config_interrat->reportAmount =
		p_rrm_report_config_interrat->report_amount;

	/* OSRTDList */
	rtxDListInit(&p_report_config_interrat->extElem1);

	switch(p_rrm_report_config_interrat->trigger_type.bitmask)
	{
		case REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_PRESENT:
			p_report_config_interrat->triggerType.t =
				T_ReportConfigInterRAT_triggerType_event;
			p_report_config_interrat->triggerType.u.event =
				rtxMemAllocType(p_asn1_ctx,
						ReportConfigInterRAT_triggerType_event);
			if (PNULL == p_report_config_interrat->triggerType.u.event)
			{
				break;
			}

			p_rrm_type_event=&p_rrm_report_config_interrat->trigger_type.event;
			p_type_event = p_report_config_interrat->triggerType.u.event;

			p_type_event->hysteresis = p_rrm_type_event->hysteresis;
			p_type_event->timeToTrigger = p_rrm_type_event->time_to_trigger;

			result =
				uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId(
						p_uecc_ue_context, 
						&p_rrm_type_event->event_id,
						p_asn1_ctx,
						&p_type_event->eventId);

			break;

		case REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_PERIODICAL_PRESENT:
			p_report_config_interrat->triggerType.t =
				T_ReportConfigInterRAT_triggerType_periodical;
			p_report_config_interrat->triggerType.u.periodical =
				rtxMemAllocType(p_asn1_ctx,
						ReportConfigInterRAT_triggerType_periodical);

			if (PNULL == p_report_config_interrat->triggerType.u.periodical)
			{
				break;
			}

			p_report_config_interrat->triggerType.u.periodical->purpose =
				p_rrm_report_config_interrat->trigger_type.periodical.purpose;
			/*SPR8897fix_start*/
			if((SI_REQUEST_FOR_HO_R9_INTERRAT_PRESENT & p_rrm_report_config_interrat->bitmask)&&
					/* SPR 12592 Fix Start */
					(reportCGI_1 == p_report_config_interrat->triggerType.u.periodical->purpose))
				/* SPR 12592 Fix Stop */
			{
				p_report_config_interrat->m._v2ExtPresent = 1;
				p_report_config_interrat->m.si_RequestForHO_r9Present = 1;
				p_report_config_interrat->si_RequestForHO_r9 =
					p_rrm_report_config_interrat->si_request_for_ho_r9;
			}
			/*SPR8897fix_stop*/

			result = RRC_SUCCESS;
			break;

		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_ReportConfigInterRAT -> wrong bitmask");
			break;
	}

	if( REPORT_QUANTITY_UTRA_FDD_R10_PRESENT & p_rrm_report_config_interrat->bitmask)
	{
		p_report_config_interrat->m._v3ExtPresent = 1;
		p_report_config_interrat->m.reportQuantityUTRA_FDD_r10Present = 1;
		p_report_config_interrat->reportQuantityUTRA_FDD_r10 =
			p_rrm_report_config_interrat->report_quantity_UTRA_FDD_r10;
	}    

#ifdef ENDC_ENABLED
    if ( REPORT_QUANTITY_CELL_NR_R15_PRESENT &
        p_rrm_report_config_interrat->bitmask )
    {
            RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                    (p_uecc_ue_context->p_gb_context)->facility_name,
                    RRC_DETAILED,
                    "REPORT_QUANTITY_CELL_NR_R15_PRESENT");
        p_report_config_interrat->m._v8ExtPresent = RRC_ONE;
        p_report_config_interrat->m.reportQuantityCellNR_r15Present = RRC_ONE;

        p_report_config_interrat->reportQuantityCellNR_r15.ss_rsrp =
            p_rrm_report_config_interrat->report_quantity_cell_nr_r15.ss_rsrp; 

        p_report_config_interrat->reportQuantityCellNR_r15.ss_rsrq =
            p_rrm_report_config_interrat->report_quantity_cell_nr_r15.ss_rsrq; 

        p_report_config_interrat->reportQuantityCellNR_r15.ss_sinr =
            p_rrm_report_config_interrat->report_quantity_cell_nr_r15.ss_sinr; 
    }

    if ( MAX_REPORT_RS_INDEX_PRESENT &
        p_rrm_report_config_interrat->bitmask )
    {
            RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                    (p_uecc_ue_context->p_gb_context)->facility_name,
                    RRC_DETAILED,
                    "MAX_REPORT_RS_INDEX_PRESENT");
        p_report_config_interrat->m._v8ExtPresent = RRC_ONE;
        p_report_config_interrat->m.maxReportRS_Index_r15Present = RRC_ONE;

        p_report_config_interrat->maxReportRS_Index_r15 =
            p_rrm_report_config_interrat->max_report_rs_index_r15; 
    }

    if ( REPORT_QUANTITY_RS_INDEX_NR_PRESENT &
        p_rrm_report_config_interrat->bitmask )
    {
            RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                    (p_uecc_ue_context->p_gb_context)->facility_name,
                    RRC_DETAILED,
                    "MAX_REPORT_RS_INDEX_PRESENT");
        p_report_config_interrat->m._v8ExtPresent = RRC_ONE;
        p_report_config_interrat->m.reportQuantityRS_IndexNR_r15Present = RRC_ONE;

        p_report_config_interrat->reportQuantityRS_IndexNR_r15.ss_rsrp =
            p_rrm_report_config_interrat->report_quantity_rs_index_nr_r15.ss_rsrp; 

        p_report_config_interrat->reportQuantityRS_IndexNR_r15.ss_rsrq =
            p_rrm_report_config_interrat->report_quantity_rs_index_nr_r15.ss_rsrq; 

        p_report_config_interrat->reportQuantityRS_IndexNR_r15.ss_sinr =
            p_rrm_report_config_interrat->report_quantity_rs_index_nr_r15.ss_sinr; 
    }

    if ( REPORT_RS_INDEX_RESULTS_NR_PRESENT &
        p_rrm_report_config_interrat->bitmask )
    {
            RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                    (p_uecc_ue_context->p_gb_context)->facility_name,
                    RRC_DETAILED,
                    "MAX_REPORT_RS_INDEX_PRESENT");
        p_report_config_interrat->m._v8ExtPresent = RRC_ONE;
        p_report_config_interrat->m.reportRS_IndexResultsNRPresent = RRC_ONE;

        p_report_config_interrat->reportRS_IndexResultsNR =
            p_rrm_report_config_interrat->report_rs_index_results_nr; 
    }

    if ( REPORT_SFTD_MEAS_PRESENT &
        p_rrm_report_config_interrat->bitmask )
    {
            RRC_UECC_TRACE((p_uecc_ue_context->p_gb_context)->context_index,
                    (p_uecc_ue_context->p_gb_context)->facility_name,
                    RRC_DETAILED,
                    "MAX_REPORT_RS_INDEX_PRESENT");
        p_report_config_interrat->m._v8ExtPresent = RRC_ONE;
        p_report_config_interrat->m.reportSFTD_Meas_r15Present = RRC_ONE;

        p_report_config_interrat->reportSFTD_Meas_r15 =
            p_rrm_report_config_interrat->report_sftd_meas_r15; 
    }
#endif
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_ReportConfigEUTRA_triggerType_event_eventId
 *   INPUT        : uecc_ue_context_t*                                  p_uecc_ue_context
 *                  report_config_eutra_trigger_type_event_eventid_t*   p_rrm_event_id
 *                  OSCTXT*                                             p_asn1_ctx
 *                  ReportConfigEUTRA_triggerType_event_eventId*        p_event_id
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds ReportConfigEUTRA_triggerType_event_eventId in
 *       MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_ReportConfigEUTRA_triggerType_event_eventId(
		uecc_ue_context_t*                                  p_uecc_ue_context,
		report_config_eutra_trigger_type_event_eventid_t*   p_rrm_event_id,
		OSCTXT*                                             p_asn1_ctx,
		ReportConfigEUTRA_triggerType_event_eventId*        p_event_id
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_event_id);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_event_id);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	switch(p_rrm_event_id->bitmask)
	{
		case REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A1_PRESENT:
			p_event_id->t=T_ReportConfigEUTRA_triggerType_event_eventId_eventA1;
			p_event_id->u.eventA1 = rtxMemAllocType(p_asn1_ctx,
					ReportConfigEUTRA_triggerType_event_eventId_eventA1);
			if (PNULL == p_event_id->u.eventA1)
			{
				break;
			}

			result = uecc_llim_build_ThresholdEUTRA(
					p_uecc_ue_context, 
					&p_rrm_event_id->event_a1.a1_threshold,
					&p_event_id->u.eventA1->a1_Threshold);
			break;

		case REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A2_PRESENT:
			p_event_id->t=T_ReportConfigEUTRA_triggerType_event_eventId_eventA2;
			p_event_id->u.eventA2 = rtxMemAllocType(p_asn1_ctx,
					ReportConfigEUTRA_triggerType_event_eventId_eventA2);
			if (PNULL == p_event_id->u.eventA2)
			{
				break;
			}

			result = uecc_llim_build_ThresholdEUTRA(
					p_uecc_ue_context, 
					&p_rrm_event_id->event_a2.a2_threshold,
					&p_event_id->u.eventA2->a2_Threshold);
			break;

		case REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A3_PRESENT:
			p_event_id->t=T_ReportConfigEUTRA_triggerType_event_eventId_eventA3;
			p_event_id->u.eventA3 = rtxMemAllocType(p_asn1_ctx,
					ReportConfigEUTRA_triggerType_event_eventId_eventA3);
			if (PNULL == p_event_id->u.eventA3)
			{
				break;
			}

			p_event_id->u.eventA3->a3_Offset =
				p_rrm_event_id->event_a3.a3_offset;
			p_event_id->u.eventA3->reportOnLeave =
				p_rrm_event_id->event_a3.report_on_leave;

			result = RRC_SUCCESS;
			break;

		case REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A4_PRESENT:
			p_event_id->t=T_ReportConfigEUTRA_triggerType_event_eventId_eventA4;
			p_event_id->u.eventA4 = rtxMemAllocType(p_asn1_ctx,
					ReportConfigEUTRA_triggerType_event_eventId_eventA4);
			if (PNULL == p_event_id->u.eventA4)
			{
				break;
			}
			result = uecc_llim_build_ThresholdEUTRA(
					p_uecc_ue_context, 
					&p_rrm_event_id->event_a4.a4_threshold,
					&p_event_id->u.eventA4->a4_Threshold);
			break;

		case REPORT_CONFIG_EUTRA_TRIGGER_TYPE_EVENT_EVENTID_A5_PRESENT:
			p_event_id->t=T_ReportConfigEUTRA_triggerType_event_eventId_eventA5;
			p_event_id->u.eventA5 = rtxMemAllocType(p_asn1_ctx,
					ReportConfigEUTRA_triggerType_event_eventId_eventA5);
			if (PNULL == p_event_id->u.eventA5)
			{
				break;
			}
			result = uecc_llim_build_ThresholdEUTRA(
					p_uecc_ue_context,
					&p_rrm_event_id->event_a5.a5_threshold1,
					&p_event_id->u.eventA5->a5_Threshold1);
			if (RRC_FAILURE == result)
			{
				break;
			}

			result = uecc_llim_build_ThresholdEUTRA(
					p_uecc_ue_context,
					&p_rrm_event_id->event_a5.a5_threshold2,
					&p_event_id->u.eventA5->a5_Threshold2);
			break;

		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_ReportConfigEUTRA_triggerType_event_eventId"
					" - wrong bitmask");
			break;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

#ifdef ENDC_ENABLED
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_b1_Threshold_NR_r15
 *   INPUT        : uecc_ue_context_t*  p_uecc_ue_context
 *                  report_config_interrat_trigger_type_event_eventid_event_b1_nr_t*
 *                      p_rrm_b1_threshold_nr
 *                  OSCTXT*             p_asn1_ctx
 *                  ThresholdNR_r15     *p_b1_threshold_nr
 *   OUTPUT       : none               
 *   DESCRIPTION  : This function builds ThresholdNR B1 in MeasConfig structure
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 ******************************************************************************/
rrc_return_et
uecc_llim_build_b1_Threshold_NR_r15
(
	uecc_ue_context_t*                                              p_uecc_ue_context,
	report_config_interrat_trigger_type_event_eventid_event_b1_nr_t *p_rrm_b1_threshold_nr,
	OSCTXT*                                                         p_asn1_ctx,
	ThresholdNR_r15                                                 *p_b1_threshold_nr
)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_b1_threshold_nr);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_b1_threshold_nr);

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	switch ( p_rrm_b1_threshold_nr->b1_threshold_nr_r15.bitmask )
	{
		case THRESHOLD_NR_R15_RSRP_PRESENT:
			p_b1_threshold_nr->t = T_ThresholdNR_r15_nr_RSRP_r15;

			p_b1_threshold_nr->u.nr_RSRP_r15 =
				p_rrm_b1_threshold_nr->b1_threshold_nr_r15.threshold_rsrp;
			result = RRC_SUCCESS;
		break;

		case THRESHOLD_NR_R15_RSRQ_PRESENT:
			p_b1_threshold_nr->t = T_ThresholdNR_r15_nr_RSRQ_r15;

			p_b1_threshold_nr->u.nr_RSRQ_r15 =
				p_rrm_b1_threshold_nr->b1_threshold_nr_r15.threshold_rsrq;
			result = RRC_SUCCESS;
		break;

		case THRESHOLD_NR_R15_SINR_PRESENT:
			p_b1_threshold_nr->t = T_ThresholdNR_r15_nr_SINR_r15;

			p_b1_threshold_nr->u.nr_SINR_r15 =
				p_rrm_b1_threshold_nr->b1_threshold_nr_r15.threshold_sinr;
			result = RRC_SUCCESS;
		break;

		default:
		RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
				p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
				"uecc_llim_build_b1_Threshold_nr - wrong bitmask");
		break;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_b2_Threshold_NR_r15
 *   INPUT        : uecc_ue_context_t*  p_uecc_ue_context
 *                  threshold_nr_r15_t  *p_rrm_b2_threshold_nr
 *                  OSCTXT*             p_asn1_ctx
 *                  ThresholdNR_r15     *p_b2_threshold_nr
 *   OUTPUT       : none               
 *   DESCRIPTION  : This function builds ThresholdNR B2 in MeasConfig structure
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 ******************************************************************************/
rrc_return_et
uecc_llim_build_b2_Threshold_NR_r15
(
	uecc_ue_context_t*    p_uecc_ue_context,
	threshold_nr_r15_t    *p_rrm_b2_threshold_nr,
	OSCTXT*               p_asn1_ctx,
	ThresholdNR_r15       *p_b2_threshold_nr
)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_b2_threshold_nr);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_b2_threshold_nr);

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	switch ( p_rrm_b2_threshold_nr->bitmask )
	{
		case THRESHOLD_NR_R15_RSRP_PRESENT:
			p_b2_threshold_nr->t = T_ThresholdNR_r15_nr_RSRP_r15;

			p_b2_threshold_nr->u.nr_RSRP_r15 =
				p_rrm_b2_threshold_nr->threshold_rsrp;
			result = RRC_SUCCESS;
		break;

		case THRESHOLD_NR_R15_RSRQ_PRESENT:
			p_b2_threshold_nr->t = T_ThresholdNR_r15_nr_RSRQ_r15;

			p_b2_threshold_nr->u.nr_RSRQ_r15 =
				p_rrm_b2_threshold_nr->threshold_rsrq;
			result = RRC_SUCCESS;
		break;

		case THRESHOLD_NR_R15_SINR_PRESENT:
			p_b2_threshold_nr->t = T_ThresholdNR_r15_nr_SINR_r15;

			p_b2_threshold_nr->u.nr_SINR_r15 =
				p_rrm_b2_threshold_nr->threshold_sinr;
			result = RRC_SUCCESS;
		break;

		default:
		RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
				p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
				"uecc_llim_build_b2_Threshold_nr - wrong bitmask");
		break;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}
#endif

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId
 *   INPUT        : uecc_ue_context_t*                                      p_uecc_ue_contex
 *                  report_config_interrat_trigger_type_event_eventid_t*    p_rrm_event_id
 *                  OSCTXT*                                                 p_asn1_ctx
 *                  ReportConfigInterRAT_triggerType_event_eventId*         p_event_id
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds ReportConfigInterRAT_triggerType_event_eventId in
 *       MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId(
		uecc_ue_context_t*                                      p_uecc_ue_context,
		report_config_interrat_trigger_type_event_eventid_t*    p_rrm_event_id,
		OSCTXT*                                                 p_asn1_ctx,
		ReportConfigInterRAT_triggerType_event_eventId*         p_event_id
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_event_id);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_event_id);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	switch(p_rrm_event_id->bitmask)
	{
		case REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_PRESENT:
			p_event_id->t =
				T_ReportConfigInterRAT_triggerType_event_eventId_eventB1;
			p_event_id->u.eventB1 = rtxMemAllocType(p_asn1_ctx,
					ReportConfigInterRAT_triggerType_event_eventId_eventB1);
			if (PNULL == p_event_id->u.eventB1)
			{
				break;
			}

			result = uecc_llim_build_b1_Threshold(
					p_uecc_ue_context, 
					&p_rrm_event_id->event_b1,
					p_asn1_ctx,
					&p_event_id->u.eventB1->b1_Threshold);
			break;

		case REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_PRESENT:
			p_event_id->t =
				T_ReportConfigInterRAT_triggerType_event_eventId_eventB2;
			p_event_id->u.eventB2 = rtxMemAllocType(p_asn1_ctx,
					ReportConfigInterRAT_triggerType_event_eventId_eventB2);
			if (PNULL == p_event_id->u.eventB2)
			{
				break;
			}

			result =
				uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId_eventB2(
						p_uecc_ue_context ,
						&p_rrm_event_id->event_b2,
						p_asn1_ctx,
						p_event_id->u.eventB2);
			break;

        #ifdef ENDC_ENABLED
		case REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_NR_PRESENT:
			p_event_id->t =
				T_ReportConfigInterRAT_triggerType_event_eventId_eventB1_NR_r15;
			p_event_id->u.eventB1_NR_r15 = rtxMemAllocType(p_asn1_ctx,
					ReportConfigInterRAT_triggerType_event_eventId_eventB1_NR_r15);
			if (PNULL == p_event_id->u.eventB1_NR_r15)
			{
				break;
			}

			result = uecc_llim_build_b1_Threshold_NR_r15(
					p_uecc_ue_context, 
					&p_rrm_event_id->event_b1_nr,
					p_asn1_ctx,
					&p_event_id->u.eventB1_NR_r15->b1_ThresholdNR_r15);

            /*
             * Fill reportOnLeave_r15
             */
            p_event_id->u.eventB1_NR_r15->reportOnLeave_r15 =
                p_rrm_event_id->event_b1_nr.report_on_leave_r15;
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
					"uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId result %d"
					" - wrong bitmask",result);

			break;

		case REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_NR_PRESENT:
			p_event_id->t =
				T_ReportConfigInterRAT_triggerType_event_eventId_eventB2_NR_r15;
			p_event_id->u.eventB2_NR_r15 = rtxMemAllocType(p_asn1_ctx,
					ReportConfigInterRAT_triggerType_event_eventId_eventB2_NR_r15);
			if (PNULL == p_event_id->u.eventB2_NR_r15)
			{
				break;
			}

            /*
             * Fill b2_Threshold1_r15
             */
		    result = uecc_llim_build_ThresholdEUTRA(
				p_uecc_ue_context,
				&p_rrm_event_id->event_b2_nr.b2_threshold1_r15,
				&p_event_id->u.eventB2_NR_r15->b2_Threshold1_r15);

            /*
             * Fill b2_Threshold2NR_r15
             */
            /* coverity_<280117>_fix_start */
            if(result!=RRC_FAILURE )
            {
			result = uecc_llim_build_b2_Threshold_NR_r15(
					p_uecc_ue_context, 
					&p_rrm_event_id->event_b2_nr.b2_threshold2_nr_r15,
					p_asn1_ctx,
					&p_event_id->u.eventB2_NR_r15->b2_Threshold2NR_r15);
            /*
             * Fill reportOnLeave_r15
             */
            p_event_id->u.eventB2_NR_r15->reportOnLeave_r15 =
                p_rrm_event_id->event_b2_nr.report_on_leave_r15;
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_DETAILED,
					"uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId result %d"
					" - wrong bitmask",result);
            }
            /* coverity_<280117>_fix_end */
			break;
            #endif


		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId"
					" - wrong bitmask");
			break;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_ThresholdEUTRA
 *   INPUT        : uecc_ue_context_t*  p_uecc_ue_context
 *                  threshold_eutra_t*  p_rrm_threshold_eutra
 *                  OSCTXT*             p_asn1_ctx
 *                  ThresholdEUTRA*     p_threshold_eutra
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds ThresholdEUTRA in
 *       MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_ThresholdEUTRA(
		uecc_ue_context_t*  p_uecc_ue_context,
		threshold_eutra_t*  p_rrm_threshold_eutra,
		ThresholdEUTRA*     p_threshold_eutra
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_threshold_eutra);
	RRC_ASSERT(PNULL != p_threshold_eutra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);


	switch(p_rrm_threshold_eutra->bitmask)
	{
		case THRESHOLD_EUTRA_RSRP_PRESENT:
			p_threshold_eutra->t = T_ThresholdEUTRA_threshold_RSRP;
			p_threshold_eutra->u.threshold_RSRP =
				p_rrm_threshold_eutra->threshold_rsrp;
			result = RRC_SUCCESS;
			break;

		case THRESHOLD_EUTRA_RSRQ_PRESENT:
			p_threshold_eutra->t = T_ThresholdEUTRA_threshold_RSRQ;
			p_threshold_eutra->u.threshold_RSRQ =
				p_rrm_threshold_eutra->threshold_rsrq;
			result = RRC_SUCCESS;
			break;

		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_ThresholdEUTRA -> wrong bitmask");
			break;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_ThresholdUTRA
 *   INPUT        : uecc_ue_context_t*  p_uecc_ue_context
 *                  threshold_eutra_t*  p_rrm_threshold_eutra
 *                  OSCTXT*             p_asn1_ctx
 *                  ThresholdEUTRA*     p_threshold_eutra
 *   OUTPUT       : none
 *
 *   DESCRIPTION:
 *       This function builds ThresholdUTRA in
 *       MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_ThresholdUTRA(
		uecc_ue_context_t*  p_uecc_ue_context,
		threshold_utra_t*   p_rrm_threshold_utra,
		ThresholdUTRA*      p_threshold_utra
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_threshold_utra);
	RRC_ASSERT(PNULL != p_threshold_utra);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);


	switch(p_rrm_threshold_utra->bitmask)
	{
		case THRESHOLD_UTRA_RSCP_PRESENT:
			p_threshold_utra->t = T_ThresholdUTRA_utra_RSCP;
			p_threshold_utra->u.utra_RSCP=p_rrm_threshold_utra->threshold_rscp;
			result = RRC_SUCCESS;
			break;

		case THRESHOLD_UTRA_ECN0_PRESENT:
			p_threshold_utra->t = T_ThresholdUTRA_utra_EcN0;
			p_threshold_utra->u.utra_EcN0=p_rrm_threshold_utra->threshold_ecn0;
			result = RRC_SUCCESS;
			break;

		default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"uecc_llim_build_ThresholdUTRA -> wrong bitmask");
			break;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_b1_Threshold
 *   INPUT        : uecc_ue_context_t*  p_uecc_ue_context
 *                  report_config_interrat_trigger_type_event_eventid_event_b1_t*
 *                  p_rrm_b1_threshold
 *                  OSCTXT*             p_asn1_ctx
 *                  ReportConfigInterRAT_triggerType_event_eventId_eventB1_b1_Threshold*
 *                  p_b1_threshold
 *   OUTPUT       : none               
 *   DESCRIPTION:
 *       This function builds ThresholdEUTRA in
 *       MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_b1_Threshold(
		uecc_ue_context_t*  p_uecc_ue_context,
		report_config_interrat_trigger_type_event_eventid_event_b1_t*
		p_rrm_b1_threshold,
		OSCTXT*                                                         p_asn1_ctx,
		ReportConfigInterRAT_triggerType_event_eventId_eventB1_b1_Threshold*
		p_b1_threshold
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_b1_threshold);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_b1_threshold);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	switch(p_rrm_b1_threshold->bitmask)
	{
		case
			REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_UTRA_PRESENT:
			p_b1_threshold->t =
			T_ReportConfigInterRAT_triggerType_event_eventId_eventB1_b1_Threshold_b1_ThresholdUTRA;
		p_b1_threshold->u.b1_ThresholdUTRA =
			rtxMemAllocType(p_asn1_ctx, ThresholdUTRA);
		if (PNULL == p_b1_threshold->u.b1_ThresholdUTRA)
		{
			break;
		}

		result = uecc_llim_build_ThresholdUTRA(
				p_uecc_ue_context,
				&p_rrm_b1_threshold->b1_threshold_utra,
				p_b1_threshold->u.b1_ThresholdUTRA);
		break;

		case
			REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_GERAN_PRESENT:
			p_b1_threshold->t =
			T_ReportConfigInterRAT_triggerType_event_eventId_eventB1_b1_Threshold_b1_ThresholdGERAN;
		p_b1_threshold->u.b1_ThresholdGERAN =
			p_rrm_b1_threshold->b1_threshold_geran;
		result = RRC_SUCCESS;
		break;

		case
			REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B1_THRESHOLD_CDMA2000_PRESENT:
			p_b1_threshold->t =
			T_ReportConfigInterRAT_triggerType_event_eventId_eventB1_b1_Threshold_b1_ThresholdCDMA2000;
		p_b1_threshold->u.b1_ThresholdCDMA2000 =
			p_rrm_b1_threshold->b1_threshold_cdma2000;
		result = RRC_SUCCESS;
		break;

		default:
		RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
				p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
				"uecc_llim_build_b1_Threshold - wrong bitmask");
		break;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME:
 *       uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId_eventB2
 *   INPUT        : uecc_ue_context_t*                                              p_uecc_ue_context
 *                  report_config_interrat_trigger_type_event_eventid_event_b2_t*
 *                  p_rrm_b2_threshold
 *                  OSCTXT*                                                         p_asn1_ctx
 *                  ReportConfigInterRAT_triggerType_event_eventId_eventB2*         p_event_b2
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds
 *           ReportConfigInterRAT_triggerType_event_eventId_eventB2 in
 *           MeasConfig structure
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et
uecc_llim_build_ReportConfigInterRAT_triggerType_event_eventId_eventB2(
		uecc_ue_context_t*                                              p_uecc_ue_context,
		report_config_interrat_trigger_type_event_eventid_event_b2_t*
		p_rrm_b2_threshold,
		OSCTXT*                                                         p_asn1_ctx,
		ReportConfigInterRAT_triggerType_event_eventId_eventB2*         p_event_b2
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_ASSERT(PNULL != p_rrm_b2_threshold);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_event_b2);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		result = uecc_llim_build_ThresholdEUTRA(
				p_uecc_ue_context,
				&p_rrm_b2_threshold->b2_threshold_eutra,
				&p_event_b2->b2_Threshold1);

		if (RRC_FAILURE == result)
		{
			break;
		}

		switch(p_rrm_b2_threshold->bitmask)
		{
			case
				REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_UTRA_PRESENT:
				p_event_b2->b2_Threshold2.t =
				T_ReportConfigInterRAT_triggerType_event_eventId_eventB2_b2_Threshold2_b2_Threshold2UTRA;
			p_event_b2->b2_Threshold2.u.b2_Threshold2UTRA =
				rtxMemAllocType(p_asn1_ctx, ThresholdUTRA);
			if (PNULL == p_event_b2->b2_Threshold2.u.b2_Threshold2UTRA)
			{
				break;
			}

			result = uecc_llim_build_ThresholdUTRA(
					p_uecc_ue_context,
					&p_rrm_b2_threshold->b2_threshold_utra,
					p_event_b2->b2_Threshold2.u.b2_Threshold2UTRA);
			break;

			case
				REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_GERAN_PRESENT:
				p_event_b2->b2_Threshold2.t =
				T_ReportConfigInterRAT_triggerType_event_eventId_eventB2_b2_Threshold2_b2_Threshold2GERAN;
			p_event_b2->b2_Threshold2.u.b2_Threshold2GERAN =
				p_rrm_b2_threshold->b2_threshold_geran;
			result = RRC_SUCCESS;
			break;

			case
				REPORT_CONFIG_INTERRAT_TRIGGER_TYPE_EVENT_EVENTID_B2_THRESHOLD_CDMA2000_PRESENT:
				p_event_b2->b2_Threshold2.t =
				T_ReportConfigInterRAT_triggerType_event_eventId_eventB2_b2_Threshold2_b2_Threshold2CDMA2000;
			p_event_b2->b2_Threshold2.u.b2_Threshold2CDMA2000 =
				p_rrm_b2_threshold->b2_threshold_cdma2000;
			result = RRC_SUCCESS;
			break;

			default:
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"%s - wrong bitmask:%d",__FUNCTION__,
					p_rrm_b2_threshold->bitmask);
			break;
		}
	}
	while(0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_get_nas_pdu_from_srb_data_status_ind_msg
 *   INPUT        : uecc_ue_context_t  *p_uecc_ue_context
 *                  OSCTXT             *p_asn1_ctx
 *                  rrc_uecc_llim_srb_data_status_ind_t *p_llim_srb_data_status_ind
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       Get NAS PDU from UECC_LLIM_SRB_DATA_STATUS_IND message if it exists.
 *
 *   RETURNS:
 *       Pointer on NAS PDU
 *       PNULL - if NAS PDU has not been found
 ******************************************************************************/
DedicatedInfoNAS* uecc_llim_get_nas_pdu_from_srb_data_status_ind_msg(
		uecc_ue_context_t  *p_uecc_ue_context,
		OSCTXT             *p_asn1_ctx,
		rrc_uecc_llim_srb_data_status_ind_t *p_llim_srb_data_status_ind
		)
{

	DL_DCCH_Message         dl_dcch_message;
	U8*                     p_data_buffer = PNULL;
	rrc_counter_t           data_buffer_length = 0;
	DedicatedInfoNAS*       p_nas_pdu = PNULL;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);


	/* Decode DL UE ASN1 message from
	 * p_llim_srb_data_status_ind->srb_data_status_error.data_buffer */
	/*****************************/

	/* Init ASN1 context */
	if ( RT_OK != rtInitContext(p_asn1_ctx) )
	{
		/* Drop message */
		RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
				p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
				"[UECC_LLIM_SRB_DATA_STATUS_IND] ASN context initialization failed.");
		RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
		return p_nas_pdu;
	}

	do
	{
		p_data_buffer =
			&p_llim_srb_data_status_ind->srb_data_status_error.data_buffer[0];

		data_buffer_length =
			p_llim_srb_data_status_ind->srb_data_status_error.data_buffer_length;

		/*get pointer on LLIM message body*/
		pu_setBuffer(p_asn1_ctx, p_data_buffer, data_buffer_length,
				ASN_RRC_BUF_TYPE);

		if ( RT_OK != asn1PD_DL_DCCH_Message(p_asn1_ctx, &dl_dcch_message) )
		{
			/* Drop message */
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
					"[UECC_LLIM_SRB_DATA_STATUS_IND] ASN decode failed.");
			break;
		}

		/* Process encoded message */
		if ( (T_DL_DCCH_MessageType_c1 != dl_dcch_message.message.t)
				|| (PNULL == dl_dcch_message.message.u.c1) )
		{
			/* Drop message */
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"[UECC_LLIM_SRB_DATA_STATUS_IND] "
					"DL_DCCH_MessageType invalid field value.");
			break;
		}

		if ( T_DL_DCCH_MessageType_c1_dlInformationTransfer ==
				dl_dcch_message.message.u.c1->t)
		{
			if ((T_DLInformationTransfer_criticalExtensions_c1 !=
						dl_dcch_message.message.u.c1->
						u.dlInformationTransfer->criticalExtensions.t)
					||
					(PNULL == dl_dcch_message.message.u.c1->
					 u.dlInformationTransfer->criticalExtensions.u.c1)
					||
					(
					 T_DLInformationTransfer_criticalExtensions_c1_dlInformationTransfer_r8 !=
					 dl_dcch_message.message.u.c1->
					 u.dlInformationTransfer->criticalExtensions.u.c1->t)
					||
					(PNULL == dl_dcch_message.message.u.c1->
					 u.dlInformationTransfer->
					 criticalExtensions.u.c1->u.dlInformationTransfer_r8)

					||
					(
					 T_DLInformationTransfer_r8_IEs_dedicatedInfoType_dedicatedInfoNAS !=
					 dl_dcch_message.message.u.c1->
					 u.dlInformationTransfer->criticalExtensions.u.c1->
					 u.dlInformationTransfer_r8->dedicatedInfoType.t)
					||
					(PNULL == dl_dcch_message.message.u.c1->
					 u.dlInformationTransfer->criticalExtensions.u.c1->
					 u.dlInformationTransfer_r8->dedicatedInfoType.
					 u.dedicatedInfoNAS)
					)
					{
						/* Drop message */
						RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
								p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
								"[UECC_LLIM_SRB_DATA_STATUS_IND] "
								"Invalid DL NAS transfer UE ASN1 message format.");
						break;
					}

			p_nas_pdu = dl_dcch_message.message.u.c1->
				u.dlInformationTransfer->criticalExtensions.u.c1->
				u.dlInformationTransfer_r8->dedicatedInfoType.
				u.dedicatedInfoNAS;
		}
	}
	while(0);

	if (PNULL == p_nas_pdu)
	{
		/* Free ASN Context */
		rtFreeContext(p_asn1_ctx);
	}

	return p_nas_pdu;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_redirected_carrier_info
 *   INPUT        : uecc_ue_context_t               *p_uecc_ue_context
 *                  rrm_redirected_carrier_info_t   *p_rci
 *                  RedirectedCarrierInfo           *p_asn1_rci
 *                  OSCTXT                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION:
 *                Builds RedirectedCarrierInfo based on
 *                given rrm_redirected_carrier_info_t
 *
 *   RETURNS:     RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_redirected_carrier_info(
		uecc_ue_context_t               *p_uecc_ue_context,
		rrm_redirected_carrier_info_t   *p_rci,
		RedirectedCarrierInfo           *p_asn1_rci,
		OSCTXT                          *p_asn1_ctx
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_RedirectedCarrierInfo(p_asn1_rci);

		if (RRM_REDIRECTED_CARRIER_INFO_EUTRA_PRESENT == p_rci->bitmask)
		{
			p_asn1_rci->t = T_RedirectedCarrierInfo_eutra;

			p_asn1_rci->u.eutra = p_rci->eutra;
		}
		else if (RRM_REDIRECTED_CARRIER_INFO_GERAN_PRESENT == p_rci->bitmask)
		{
			p_asn1_rci->t = T_RedirectedCarrierInfo_geran;
			p_asn1_rci->u.geran =
				rtxMemAllocType(p_asn1_ctx, CarrierFreqsGERAN);
			if (PNULL == p_asn1_rci->u.geran)
			{
				break;
			}

			if (RRC_SUCCESS!=check_and_convert_carrier_freqs_geran(
						p_asn1_ctx,
						p_asn1_rci->u.geran,
						&p_rci->geran))
			{
				break;
			}
		}
		else if (RRM_REDIRECTED_CARRIER_INFO_UTRA_FDD_PRESENT == p_rci->bitmask)
		{
			p_asn1_rci->t = T_RedirectedCarrierInfo_utra_FDD;

			p_asn1_rci->u.utra_FDD = p_rci->utra_fdd;
		}
		else if (RRM_REDIRECTED_CARRIER_INFO_UTRA_TDD_PRESENT == p_rci->bitmask)
		{
			p_asn1_rci->t = T_RedirectedCarrierInfo_utra_TDD;

			p_asn1_rci->u.utra_TDD = p_rci->utra_tdd;
		}
		else if (RRM_REDIRECTED_CARRIER_INFO_CDMA2000_HRPD_PRESENT ==
				p_rci->bitmask)
		{
			p_asn1_rci->t = T_RedirectedCarrierInfo_cdma2000_HRPD;
			p_asn1_rci->u.cdma2000_HRPD =
				rtxMemAllocType(p_asn1_ctx, CarrierFreqCDMA2000);
			if (PNULL == p_asn1_rci->u.cdma2000_HRPD)
			{
				break;
			}

			p_asn1_rci->u.cdma2000_HRPD->bandClass =
				p_rci->cdma2000_hrpd.band_class;

			p_asn1_rci->u.cdma2000_HRPD->arfcn =
				p_rci->cdma2000_hrpd.arfcn;
		}
		else if (RRM_REDIRECTED_CARRIER_INFO_CDMA2000_1XRTT_PRESENT ==
				p_rci->bitmask)
		{
			p_asn1_rci->t = T_RedirectedCarrierInfo_cdma2000_1xRTT;
			p_asn1_rci->u.cdma2000_1xRTT =
				rtxMemAllocType(p_asn1_ctx, CarrierFreqCDMA2000);
			if (PNULL == p_asn1_rci->u.cdma2000_1xRTT)
			{
				break;
			}

			p_asn1_rci->u.cdma2000_1xRTT->bandClass =
				p_rci->cdma2000_1xrtt.band_class;

			p_asn1_rci->u.cdma2000_1xRTT->arfcn =
				p_rci->cdma2000_1xrtt.arfcn;
		}/*CR 713 changes start*/
		else if (RRM_REDIRECTED_CARRIER_INFO_UTRA_TDD_R10_PRESENT ==
				p_rci->bitmask)
		{
			/*if (bitmask_cell_info_list_r9 & RRC_RRM_CELL_INFO_LIST_UTRA_TDD_R10_PRESENT)
			  {*/
			p_asn1_rci->t = T_RedirectedCarrierInfo_utra_TDD_r10;
			p_asn1_rci->u.utra_TDD_r10 = rtxMemAllocType(p_asn1_ctx, 
					CarrierFreqListUTRA_TDD_r10);
			if (PNULL == p_asn1_rci->u.utra_TDD_r10)
			{
				break;
			}
			/*Integration Fix start*/
			p_asn1_rci->u.utra_TDD_r10->n = p_rci->redirected_carrier_info_utra_tdd_r10.count;

			/* klockwork fix */ 
			if ( p_rci->redirected_carrier_info_utra_tdd_r10.count <= MAX_FREQ_UTRA_TDD_R10 )
			{
				l3_memcpy_wrapper(p_asn1_rci->u.utra_TDD_r10->elem, 
						p_rci->redirected_carrier_info_utra_tdd_r10.utra_tdd_r10,
						(sizeof(U16) * p_rci->redirected_carrier_info_utra_tdd_r10.count));
			}


			/*Integration Fix stop*/
			/*}
			  else
			  {
			  RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
			  p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
			  " CellInfoList-r9 is not set to 'utra-TDD-r10'"
			  " RedirectedCarrierInfo cannot be set to 'utra-TDD -r10' - Ignoring Message");
			  result = RRC_FAILURE;
			  break;
			  }*/
		}/*CR 713 changes stop*/
		else
		{
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
					p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
					"%s - wrong bitmask:%d",__FUNCTION__,p_rci->bitmask);
			break;
		}

		result = RRC_SUCCESS;
	} while (0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return result;
}

/******************************************************************************
 *   FUNCTION NAME : uecc_llim_idle_mode_mobility_ctrl_chk_for_eutra_and_geran
 *
 *   INPUT         : uecc_ue_context_t                     *p_uecc_ue_context,
 *                   rrm_idle_mode_mobility_control_info_t *p_immc,
 *                   IdleModeMobilityControlInfo           *p_asn1_immc,
 *                   OSCTXT                                *p_asn1_ctx
 *   
 *   OUTPUT        : None
 *
 *   DESCRIPTION   : Builds IdleModeMobilityControlInfo for EUTRA or GERAN
 *
 *   RETURNS       : RRC_SUCCESS/RRC_FAILURE
 ******************************************************************************/
static rrc_return_et uecc_llim_idle_mode_mobility_ctrl_chk_for_eutra_and_geran(
		uecc_ue_context_t                       *p_uecc_ue_context,
		rrm_idle_mode_mobility_control_info_t   *p_immc,
		IdleModeMobilityControlInfo             *p_asn1_immc,
		OSCTXT                                  *p_asn1_ctx
		)
{
	U16 idx = RRC_NULL;
	OSRTDListNode *p_node = PNULL;
	FreqPriorityEUTRA *p_FreqPriorityEUTRA_elem = PNULL;
	FreqsPriorityGERAN *p_FreqsPriorityGERAN_elem = PNULL;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
	if (RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_EUTRA_PRESENT
			& p_immc->bitmask)
	{
		p_asn1_immc->m.freqPriorityListEUTRAPresent = 1;
		rtxDListInit(&p_asn1_immc->freqPriorityListEUTRA);

		for (idx=0; idx < p_immc->freq_priority_list_eutra.count; idx++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx, FreqPriorityEUTRA, &p_node,
					&p_FreqPriorityEUTRA_elem);
			if (PNULL == p_node)
			{
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				/* Not enough memory */
				return RRC_FAILURE;
			}

			p_FreqPriorityEUTRA_elem->carrierFreq =
				p_immc->freq_priority_list_eutra.freq_priority_eutra[idx].
				carrier_freq;

			p_FreqPriorityEUTRA_elem->cellReselectionPriority =
				p_immc->freq_priority_list_eutra.freq_priority_eutra[idx].
				cell_reselection_priority;

			/* append node to the list */
			rtxDListAppendNode(&p_asn1_immc->freqPriorityListEUTRA, p_node);
		}
	}

	if (RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_GERAN_PRESENT
			& p_immc->bitmask)
	{
		p_asn1_immc->m.freqPriorityListGERANPresent = 1;
		rtxDListInit(&p_asn1_immc->freqPriorityListGERAN);

		for (idx=0; idx < p_immc->freq_priority_list_geran.count; idx++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx, FreqsPriorityGERAN, &p_node,
					&p_FreqsPriorityGERAN_elem);
			if (PNULL == p_node)
			{
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				/* Not enough memory */
				return RRC_FAILURE;
			}

			if (RRC_SUCCESS!=check_and_convert_carrier_freqs_geran(
						p_asn1_ctx,
						&p_FreqsPriorityGERAN_elem->carrierFreqs,
						&p_immc->freq_priority_list_geran.freqs_priority_geran[idx].
						carrier_freq))
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
						"%s:error in check_and_convert_carrier_freqs_geran()",
						__FUNCTION__);

				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				return RRC_FAILURE;
			}

			p_FreqsPriorityGERAN_elem->cellReselectionPriority =
				p_immc->freq_priority_list_geran.freqs_priority_geran[idx].
				cell_reselection_priority;

			/* append node to the list */
			rtxDListAppendNode(&p_asn1_immc->freqPriorityListGERAN, p_node);
		}
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return RRC_SUCCESS;	
}

/******************************************************************************
 *   FUNCTION NAME : uecc_llim_idle_mode_mobility_ctrl_chk_for_hrpd_and_1xrtt
 *
 *   INPUT         : uecc_ue_context_t                     *p_uecc_ue_context,
 *                   rrm_idle_mode_mobility_control_info_t *p_immc,
 *                   IdleModeMobilityControlInfo           *p_asn1_immc,
 *                   OSCTXT                                *p_asn1_ctx
 *   
 *   OUTPUT        : None
 *
 *   DESCRIPTION   : Builds IdleModeMobilityControlInfo for EUTRA or GERAN
 *
 *   RETURNS       : RRC_SUCCESS/RRC_FAILURE
 ******************************************************************************/
static rrc_return_et uecc_llim_idle_mode_mobility_ctrl_chk_for_hrpd_and_1xrtt(
		uecc_ue_context_t                       *p_uecc_ue_context,
		rrm_idle_mode_mobility_control_info_t   *p_immc,
		IdleModeMobilityControlInfo             *p_asn1_immc,
		OSCTXT                                  *p_asn1_ctx
		)
{
	U16 idx = RRC_NULL;
	OSRTDListNode *p_node = PNULL;
	BandClassPriorityHRPD *p_BandClassPriorityHRPD_elem = PNULL;
	BandClassPriority1XRTT *p_BandClassPriority1XRTT_elem = PNULL;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,p_uecc_ue_context->p_gb_context->facility_name 
                       ,RRC_DETAILEDALL, "Unused variable"
                      " p_uecc_ue_context=%p",p_uecc_ue_context);/*SPR 17777 +-*/
	if(RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_BAND_CLASS_PRIORITY_LIST_HRPD_PRESENT
			& p_immc->bitmask)
	{
		p_asn1_immc->m.bandClassPriorityListHRPDPresent = 1;
		rtxDListInit(&p_asn1_immc->bandClassPriorityListHRPD);

		for (idx=0; idx < p_immc->band_class_priority_list_hrpd.count; idx++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx, BandClassPriorityHRPD, &p_node,
					&p_BandClassPriorityHRPD_elem);
			if (PNULL == p_node)
			{
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				/* Not enough memory */
				return RRC_FAILURE;
			}

			p_BandClassPriorityHRPD_elem->bandClass =
				p_immc->band_class_priority_list_hrpd.band_class_priority[idx].
				band_class;

			p_BandClassPriorityHRPD_elem->cellReselectionPriority =
				p_immc->band_class_priority_list_hrpd.band_class_priority[idx].
				cell_reselection_priority;

			/* append node to the list */
			rtxDListAppendNode(&p_asn1_immc->bandClassPriorityListHRPD, p_node);
		}
	}

	if (RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_BAND_CLASS_PRIORITY_LIST_1XRTT_PRESENT
			& p_immc->bitmask)
	{
		p_asn1_immc->m.bandClassPriorityList1XRTTPresent = 1;
		rtxDListInit(&p_asn1_immc->bandClassPriorityList1XRTT);

		for (idx=0; idx < p_immc->band_class_priority_list_1xrtt.count; idx++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx, BandClassPriority1XRTT,
					&p_node, &p_BandClassPriority1XRTT_elem);
			if (PNULL == p_node)
			{
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				/* Not enough memory */
				return RRC_FAILURE;
			}

			p_BandClassPriority1XRTT_elem->bandClass =
				p_immc->band_class_priority_list_1xrtt.band_class_priority[idx].
				band_class;

			p_BandClassPriority1XRTT_elem->cellReselectionPriority =
				p_immc->band_class_priority_list_1xrtt.band_class_priority[idx].
				cell_reselection_priority;

			/* append node to the list */
			rtxDListAppendNode(&p_asn1_immc->bandClassPriorityList1XRTT,
					p_node);
		}
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return RRC_SUCCESS;
}
/******************************************************************************
 *   FUNCTION NAME : uecc_llim_idle_mode_mobility_ctrl_chk_for_eutra_FDD_and_TDD
 *
 *   INPUT         : uecc_ue_context_t                     *p_uecc_ue_context,
 *                   rrm_idle_mode_mobility_control_info_t *p_immc,
 *                   IdleModeMobilityControlInfo           *p_asn1_immc,
 *                   OSCTXT                                *p_asn1_ctx
 *
 *   OUTPUT        : None
 *
 *   DESCRIPTION   : Builds IdleModeMobilityControlInfo for EUTRA or GERAN
 *
 *   RETURNS       : RRC_SUCCESS/RRC_FAILURE
 ******************************************************************************/
static rrc_return_et uecc_llim_idle_mode_mobility_ctrl_chk_for_eutra_FDD_and_TDD(
		uecc_ue_context_t                       *p_uecc_ue_context,
		rrm_idle_mode_mobility_control_info_t   *p_immc,
		IdleModeMobilityControlInfo             *p_asn1_immc,
		OSCTXT                                  *p_asn1_ctx
		)
{
	U16 idx = RRC_NULL;
	OSRTDListNode *p_node = PNULL;
	FreqPriorityUTRA_FDD *p_FreqPriorityUTRA_FDD_elem = PNULL;
	FreqPriorityUTRA_TDD *p_FreqPriorityUTRA_TDD_elem = PNULL;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,p_uecc_ue_context->p_gb_context->facility_name 
                       ,RRC_DETAILEDALL, "Unused variable"
                      " p_uecc_ue_context-=%p",p_uecc_ue_context);/*SPR 17777+-*/
	if (RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_UTRA_FDD_PRESENT
			& p_immc->bitmask)
	{
		p_asn1_immc->m.freqPriorityListUTRA_FDDPresent = 1;
		rtxDListInit(&p_asn1_immc->freqPriorityListUTRA_FDD);

		for (idx=0; idx < p_immc->freq_priority_list_utra_fdd.count; idx++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx, FreqPriorityUTRA_FDD, &p_node,
					&p_FreqPriorityUTRA_FDD_elem);
			if (PNULL == p_node)
			{
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				/* Not enough memory */
				return RRC_FAILURE;
			}

			p_FreqPriorityUTRA_FDD_elem->carrierFreq =
				p_immc->freq_priority_list_utra_fdd.freq_priority_utra_fdd[idx].
				carrier_freq;

			p_FreqPriorityUTRA_FDD_elem->cellReselectionPriority =
				p_immc->freq_priority_list_utra_fdd.freq_priority_utra_fdd[idx].
				cell_reselection_priority;

			/* append node to the list */
			rtxDListAppendNode(&p_asn1_immc->freqPriorityListUTRA_FDD, p_node);
		}
	}

	if (RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_FREQ_PRIORITY_LIST_UTRA_TDD_PRESENT
			& p_immc->bitmask)
	{
		p_asn1_immc->m.freqPriorityListUTRA_TDDPresent = 1;
		rtxDListInit(&p_asn1_immc->freqPriorityListUTRA_TDD);

		for (idx=0; idx < p_immc->freq_priority_list_utra_tdd.count; idx++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx, FreqPriorityUTRA_TDD, &p_node,
					&p_FreqPriorityUTRA_TDD_elem);
			if (PNULL == p_node)
			{
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				/* Not enough memory */
				return RRC_FAILURE;
			}

			p_FreqPriorityUTRA_TDD_elem->carrierFreq =
				p_immc->freq_priority_list_utra_tdd.freq_priority_utra_tdd[idx].
				carrier_freq;

			p_FreqPriorityUTRA_TDD_elem->cellReselectionPriority =
				p_immc->freq_priority_list_utra_tdd.freq_priority_utra_tdd[idx].
				cell_reselection_priority;

			/* append node to the list */
			rtxDListAppendNode(&p_asn1_immc->freqPriorityListUTRA_TDD, p_node);
		}
	}
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_idle_mode_mobility_control
 *   INPUTS       : uecc_ue_context_t                       *p_uecc_ue_context,
 rrm_idle_mode_mobility_control_info_t   *p_immc,
 IdleModeMobilityControlInfo             *p_asn1_immc,
 OSCTXT                                  *p_asn1_ctx
OUTPUTS      : None
 *   DESCRIPTION:
 *                Builds IdleModeMobilityControlInfo based on
 *                given rrm_idle_mode_mobility_control_info_t
 *
 *   RETURNS:     RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_idle_mode_mobility_control(
		uecc_ue_context_t                       *p_uecc_ue_context,
		rrm_idle_mode_mobility_control_info_t   *p_immc,
		IdleModeMobilityControlInfo             *p_asn1_immc,
		OSCTXT                                  *p_asn1_ctx
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	asn1Init_IdleModeMobilityControlInfo(p_asn1_immc);

	result = uecc_llim_idle_mode_mobility_ctrl_chk_for_eutra_and_geran(
			p_uecc_ue_context,p_immc,p_asn1_immc,p_asn1_ctx);
	if(RRC_FAILURE==result)
	{
		RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
		return RRC_FAILURE;
	}

	result = uecc_llim_idle_mode_mobility_ctrl_chk_for_eutra_FDD_and_TDD(
			p_uecc_ue_context,p_immc,p_asn1_immc,p_asn1_ctx);
	if(RRC_FAILURE==result)
	{
		RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
		return RRC_FAILURE;
	}

	result = uecc_llim_idle_mode_mobility_ctrl_chk_for_hrpd_and_1xrtt(
			p_uecc_ue_context,p_immc,p_asn1_immc,p_asn1_ctx);
	if(RRC_FAILURE==result)
	{
		RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
		return RRC_FAILURE;
	}

	if (RRM_IDLE_MODE_MOBILITY_CONTROL_INFO_T320_PRESENT & p_immc->bitmask)
	{
		p_asn1_immc->m.t320Present = 1;

		p_asn1_immc->t320 = p_immc->t320;
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return RRC_SUCCESS;
}


/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_rrc_connection_release_v890_ies
 *   INPUT        : uecc_ue_context_t*  p_uecc_ue_context
 *                  rrm_rrc_connection_release_v890_ies_t   *p_rrm_rcr_v890
 *                  RRCConnectionRelease_v890_IEs           *p_asn1_rcr_v890
 *                  OSCTXT                                  *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION:
 *                Builds RRCConnectionRelease_v890_IEs based on
 *                given rrm_rrc_connection_release_v890_ies_t
 *
 *   RETURNS:     RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_rrc_connection_release_v890_ies(
		uecc_ue_context_t*  p_uecc_ue_context,        /* UE context */
		rrm_rrc_connection_release_v890_ies_t   *p_rrm_rcr_v890,
		RRCConnectionRelease_v890_IEs           *p_asn1_rcr_v890,
		OSCTXT                                  *p_asn1_ctx
		)
{
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	asn1Init_RRCConnectionRelease_v890_IEs(p_asn1_rcr_v890);

	if (RRC_RRM_UE_CONNECTION_RELEASE_IND_RRC_CONN_REL_V920_IES_PRESENT
			& p_rrm_rcr_v890->bitmask)
	{
		p_asn1_rcr_v890->m.nonCriticalExtensionPresent = 1;

		if ( RRC_RRM_CELL_INFO_LIST_R9_PRESENT & 
				p_rrm_rcr_v890->rrc_connection_release_v920_ies.bitmask)
		{

			if (RRC_SUCCESS!=uecc_llim_check_and_convert_cell_info_list_r9(
						p_uecc_ue_context,
						p_asn1_ctx,
						&p_asn1_rcr_v890->nonCriticalExtension.cellInfoList_r9,
						&p_rrm_rcr_v890->rrc_connection_release_v920_ies.
						cell_info_list_r9))
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
						p_uecc_ue_context->p_gb_context->facility_name, RRC_WARNING,
						"uecc_llim_check_and_convert_carrier_freqs_geran Failed");
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				return RRC_FAILURE;
			}
			if ((p_asn1_rcr_v890->nonCriticalExtension.cellInfoList_r9.t == 
						T_RRCConnectionRelease_v920_IEs_cellInfoList_r9_geran_r9) ||
					(p_asn1_rcr_v890->nonCriticalExtension.cellInfoList_r9.t == 
					 T_RRCConnectionRelease_v920_IEs_cellInfoList_r9_utra_FDD_r9) ||
					(p_asn1_rcr_v890->nonCriticalExtension.cellInfoList_r9.t == 
					 T_RRCConnectionRelease_v920_IEs_cellInfoList_r9_utra_TDD_r9) /*CR 713 changes start*/
					|| (p_asn1_rcr_v890->nonCriticalExtension.cellInfoList_r9.t ==
						T_RRCConnectionRelease_v920_IEs_cellInfoList_r9_utra_TDD_r10 ))/*CR 713 changes stop*/
			{
				p_asn1_rcr_v890->nonCriticalExtension.m.cellInfoList_r9Present = 1;
			}
			else
			{
				p_asn1_rcr_v890->nonCriticalExtension.m.cellInfoList_r9Present = 0;
			}
		}
		if ( RRC_RRM_CONNECTION_RELEASE_V1020_PRESENT & 
				p_rrm_rcr_v890->rrc_connection_release_v920_ies.bitmask)
		{
			p_asn1_rcr_v890->nonCriticalExtension.m.nonCriticalExtensionPresent = 1;

			if ( RRC_RRM_EXTENDED_WAIT_TIME_PRESENT & 
					p_rrm_rcr_v890->rrc_connection_release_v920_ies.rrc_connection_release_v1020_ies.bitmask && 
					delayTolerantAccess_v1020 == p_uecc_ue_context->rcs_establishmentCause)
			{
				if ( !((T_s1ap_Cause_radioNetwork ==
								p_uecc_ue_context->share_data.uecc_rcr_data.u.cause.t) &&
							(s1ap_load_balancing_tau_required == p_uecc_ue_context->
							 share_data.uecc_rcr_data.u.cause.u.radioNetwork)) &&
						!((T_s1ap_Cause_radioNetwork ==
								p_uecc_ue_context->share_data.uecc_rcr_data.u.cause.t) &&
							(s1ap_cs_fallback_triggered == p_uecc_ue_context->
							 share_data.uecc_rcr_data.u.cause.u.radioNetwork)))
				{
					p_asn1_rcr_v890->nonCriticalExtension.nonCriticalExtension.m.extendedWaitTime_r10Present = 1;
					p_asn1_rcr_v890->nonCriticalExtension.nonCriticalExtension.extendedWaitTime_r10 = 
						p_rrm_rcr_v890->rrc_connection_release_v920_ies.rrc_connection_release_v1020_ies.extended_wait_time_r10;
					return RRC_SUCCESS;
				}

			}   
		}            

	}

	if ((1==p_uecc_ue_context->m.extended_wait_time_present) &&
			delayTolerantAccess_v1020 == p_uecc_ue_context->rcs_establishmentCause ) 
	{
		if ( !((T_s1ap_Cause_radioNetwork ==
						p_uecc_ue_context->share_data.uecc_rcr_data.u.cause.t) &&
					(s1ap_load_balancing_tau_required == p_uecc_ue_context->
					 share_data.uecc_rcr_data.u.cause.u.radioNetwork)) &&
				!((T_s1ap_Cause_radioNetwork ==
						p_uecc_ue_context->share_data.uecc_rcr_data.u.cause.t) &&
					(s1ap_cs_fallback_triggered == p_uecc_ue_context->
					 share_data.uecc_rcr_data.u.cause.u.radioNetwork)))
		{
			p_asn1_rcr_v890->nonCriticalExtension.m.nonCriticalExtensionPresent = 1;
			p_asn1_rcr_v890->nonCriticalExtension.nonCriticalExtension.m.extendedWaitTime_r10Present = 1;
			p_asn1_rcr_v890->nonCriticalExtension.nonCriticalExtension.extendedWaitTime_r10 = p_uecc_ue_context->extended_wait_time;
		}
	}   


	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
	return RRC_SUCCESS;
}
/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_erb_modify_asn1_drb_to_add_mod_list
 *   INPUT        : uecc_ue_context_t                           *p_ue_context
 *                  DRB_ToAddModList                            *p_asn1_list,
 *                  OSCTXT                                      *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION:    This function fills 'DRB_ToAddModList' from information
 *                   of 'rrm_drb_to_add_info_list_t' for E-RAB Modify request.
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_erb_modify_asn1_drb_to_add_mod_list(
		uecc_ue_context_t                           *p_ue_context,
		DRB_ToAddModList                            *p_asn1_list,
		OSCTXT                                      *p_asn1_ctx
		)
{
	DRB_ToAddMod                    *p_asn1_drb_elem = PNULL;
	OSRTDListNode                   *p_node = PNULL;
	U8                              drb_index = RRC_NULL;
	U16                              e_rab_list_count = 0;

	RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

	rtxDListInit(p_asn1_list);

	e_rab_list_count = p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
		erab_modify_list.erab_modify_list_counter;

	if (e_rab_list_count > MAX_ERAB_COUNT)
	{
		RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index, 
				p_ue_context->p_gb_context->facility_name, RRC_ERROR,
				"generate_drb_info_list: invalid drb_count=%i",
				e_rab_list_count);
		return RRC_FAILURE;
	}

	/* Support Multiple E-RABs */
	for (drb_index=0; drb_index < e_rab_list_count; drb_index++)
	{
		rtxDListAllocNodeAndData(p_asn1_ctx, DRB_ToAddMod,
				&p_node, &p_asn1_drb_elem);
		if (PNULL == p_node)
		{
			RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
					p_ue_context->p_gb_context->facility_name, RRC_ERROR,
					"generate_drb_info_list: ASN malloc failed");
			RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
			return RRC_FAILURE;
		}
		if (RRC_SUCCESS == 
				p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
				erab_modify_list.erab_modify_item[drb_index].status)
		{
			/* SPR:7369 start*/
			if (RRC_SUCCESS !=
					uecc_llim_build_asn1_drb_to_add_mod_new(
						p_ue_context,
						&(p_ue_context->p_curr_proc_data->u.erab_modify_request_data.
							erab_modify_list.erab_modify_item[drb_index].drb_config),
						p_asn1_drb_elem,
						p_asn1_ctx))
				/* SPR:7369 stop*/
			{
				RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);
				return RRC_FAILURE;
			}
			/* append node to the list */
			rtxDListAppendNode(p_asn1_list, p_node);

		}
	}

	RRC_UECC_UT_TRACE_EXIT(p_ue_context->p_gb_context);

	return RRC_SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_ics_asn1_radio_resource_cfg_dedicated
 *   INPUT        : uecc_ue_context_t               *p_uecc_ue_context
 *                  RadioResourceConfigDedicated    *p_asn1_rrc
 *                  rrm_srb_info_t                  *p_srb_info
 *                  OSCTXT                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds RadioResourceConfigDedicated
 *       structure for ASN1 encoding from given ue_adm_radio_res_config_t
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_ics_asn1_radio_resource_cfg_dedicated(
		uecc_ue_context_t               *p_uecc_ue_context,
		RadioResourceConfigDedicated    *p_asn1_rrc,
		rrm_srb_info_t                  *p_srb_info,
		OSCTXT                          *p_asn1_ctx
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_RadioResourceConfigDedicated(p_asn1_rrc);

		/* SRB_ToAddModList */
		/* srb2 config to be sent during ICS 
		 * (if only received from RRM in UE admission response) 
		 * and not in ERAB setup response */
		if (PNULL != p_srb_info &&
				((!(strncmp_wrapper((const S8 *)(p_uecc_ue_context->uecc_fsm.proc_descriptor.
								 p_active_proc->s_id),(const S8 *)"UE_ICS_P",
						    strlen_wrapper((const Char8 *)(p_uecc_ue_context->uecc_fsm.proc_descriptor.
							    p_active_proc->s_id)))) ||
				  !(strncmp_wrapper((const S8 *)(p_uecc_ue_context->uecc_fsm.proc_descriptor.
							  p_active_proc->s_id),(const S8 *)"UE_TRG_HO_P",
						  strlen_wrapper((const Char8 *)(p_uecc_ue_context->uecc_fsm.proc_descriptor.
							  p_active_proc->s_id))))) &&    
				 p_uecc_ue_context->m.srb2_config_present))
		{
			p_asn1_rrc->m.srb_ToAddModListPresent = 1;

			/* Generate SRB Information */
			if (RRC_SUCCESS != uecc_llim_build_asn1_srb_to_add_mod_list(
						/* CSR 00070192 Fix Start */
						p_uecc_ue_context,
						/* CSR 00070192 Fix Stop */
						&p_asn1_rrc->srb_ToAddModList,
						p_srb_info,
						p_asn1_ctx))
			{
				break;
			}
		}

		/* RadioResourceConfigDedicated_mac_MainConfig */
		if(p_uecc_ue_context->ho_info.s1_or_x2_handover == X2_HO ||
				p_uecc_ue_context->ho_info.s1_or_x2_handover == S1_HO ||
				/*BUG 604 changes start*/
				p_uecc_ue_context->ho_info.s1_or_x2_handover == INTER_CELL_HO  ||
				p_uecc_ue_context->ho_info.s1_or_x2_handover == INTRA_CELL_HO )
			/*BUG 604 changes stop*/
		{
			if (p_uecc_ue_context->m.mac_main_config_present)
			{
				p_asn1_rrc->m.mac_MainConfigPresent = 1;

				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index, 
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_INFO,"ICS_RCR: mac_main_config present");
				if (RRC_SUCCESS != uecc_llim_build_asn1_mac_main_config(
							p_uecc_ue_context,
							&(p_uecc_ue_context->mac_config),
							&p_asn1_rrc->mac_MainConfig,
							p_asn1_ctx))
				{
					break;
				}
			}
		}
		else
		{

			if ( p_uecc_ue_context->m.mac_config_updated )
			{
				if (p_uecc_ue_context->m.mac_main_config_present)
				{
					p_asn1_rrc->m.mac_MainConfigPresent = 1;

					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name,
							RRC_INFO,"ICS_RCR: mac_main_config present");
					if (RRC_SUCCESS != uecc_llim_build_asn1_mac_main_config(
								p_uecc_ue_context,
								&(p_uecc_ue_context->mac_config),
								&p_asn1_rrc->mac_MainConfig,
								p_asn1_ctx))
					{
						break;
					}
				}
			}
		}

		/* DRB_ToAddModList */
		if ( 0 < p_uecc_ue_context->e_rab_list_count )
		{
			p_asn1_rrc->m.drb_ToAddModListPresent = 1;

			/* Generate DRB Information */
			if (RRC_SUCCESS != uecc_llim_build_erb_setup_asn1_drb_to_add_mod_list(
						p_uecc_ue_context,
						&p_asn1_rrc->drb_ToAddModList,
						p_asn1_ctx))
			{
				break;
			}

#ifdef ENDC_ENABLED
            /*NR_DC Code Changes Start*/
            if(p_uecc_ue_context->is_sgnb_add_reqd &&
                    RRC_NULL == p_asn1_rrc->drb_ToAddModList.count)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,
                        RRC_INFO,"ICS_RCR: SGNB Addition: All the erAB are SN_TERMINATED_SCG_BEARER");

                p_asn1_rrc->m.drb_ToAddModListPresent = 0;
            }
            /*NR_DC Code Changes Stop*/
#endif
		}

		/* SPS Configuration */
		if ( (p_uecc_ue_context->m.sps_config_updated) && (PNULL != p_uecc_ue_context->p_sps_config))
		{
			p_asn1_rrc->m.sps_ConfigPresent = 1;

			if (RRC_SUCCESS != uecc_llim_build_asn1_sps_config(
						p_uecc_ue_context,
						/* SPS Start */
						p_uecc_ue_context->p_sps_config,
						/* SPS Stop */
						&p_asn1_rrc->sps_Config,
						p_asn1_ctx))
			{
				break;
			}
		}

	/* SPR 20747 Fix Start */
      	else if ((PNULL != p_uecc_ue_context->ho_info.p_ho_info) && 
                  (PNULL != p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info) && 
 		            (RRC_NULL != p_uecc_ue_context->ho_info.p_ho_info->
                      p_trg_ho_info->is_sps_recv_from_src) &&
                       (p_uecc_ue_context->m.is_sps_setup_or_rel_recv))
        {
		     p_asn1_rrc->m.sps_ConfigPresent = 1;

			if (p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_sps_recv_from_src & SRC_SPS_CONFIG_DL_PRESENT)
			{
				p_asn1_rrc->sps_Config.m.sps_ConfigDLPresent = 1;
				p_asn1_rrc->sps_Config.sps_ConfigDL.t = T_SPS_ConfigDL_release;
			}
			if (p_uecc_ue_context->ho_info.p_ho_info->p_trg_ho_info->is_sps_recv_from_src & SRC_SPS_CONFIG_UL_PRESENT)
			{
				p_asn1_rrc->sps_Config.m.sps_ConfigULPresent = 1;
				p_asn1_rrc->sps_Config.sps_ConfigUL.t = T_SPS_ConfigUL_release;
			}

	}
		/* SPR 20747 Fix End */

		/* PhysicalConfigDedicated */
		if (  p_uecc_ue_context->m.physical_config_dedicated_updated )
		{
			p_asn1_rrc->m.physicalConfigDedicatedPresent = 1;

			if (RRC_SUCCESS != uecc_llim_build_asn1_physical_config_dedicated(
						p_uecc_ue_context,
						&(p_uecc_ue_context->physical_config_dedicated),
						&p_asn1_rrc->physicalConfigDedicated,
						p_asn1_ctx)
			   )
			{
				break;
			}
		}

		/* CR_220 */
		/* RLF TIMERS & CONSTANTS*/
		if( p_uecc_ue_context->m.rlf_timers_and_constants_r9_present )
		{
			if (PNULL != p_uecc_ue_context->p_rlf_timers_and_constants_r9)
			{
				p_asn1_rrc->m.rlf_TimersAndConstants_r9Present = 1;

				p_asn1_rrc->m._v2ExtPresent = 1;


				if (RRC_SUCCESS != uecc_llim_build_asn1_rlf_timers_and_constants_r9(
							p_uecc_ue_context, 
							(p_uecc_ue_context->p_rlf_timers_and_constants_r9),
							&p_asn1_rrc->rlf_TimersAndConstants_r9,
							p_asn1_ctx)
				   )
				{
					break;
				}
			}
		} 

		/* main extElem1 */
		rtxDListInit(&p_asn1_rrc->extElem1);
		result = RRC_SUCCESS;
	}
	while(0);
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return result;
}


/* ERAB SETUP START */

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_erb_setup_asn1_radio_resource_cfg_dedicated
 *   INPUT        : uecc_ue_context_t               *p_uecc_ue_context
 *                  RadioResourceConfigDedicated    *p_asn1_rrc
 *                  rrm_srb_info_t                  *p_srb_info
 *                  OSCTXT                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION:
 *       This function builds RadioResourceConfigDedicated
 *       structure for ASN1 encoding from given ue_adm_radio_res_config_t
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_erb_setup_asn1_radio_resource_cfg_dedicated(
		uecc_ue_context_t               *p_uecc_ue_context,
		RadioResourceConfigDedicated    *p_asn1_rrc,
		OSCTXT                          *p_asn1_ctx
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
	do
	{
		asn1Init_RadioResourceConfigDedicated(p_asn1_rrc);
		/* RadioResourceConfigDedicated_mac_MainConfig */
		if (PNULL != p_uecc_ue_context->p_curr_proc_data)
        {
#ifdef ENDC_ENABLED
            /* OPTION3X Changes Start */
            if( UECC_RRM_DC_BEARER_CHANGE_PROC !=
                    p_uecc_ue_context->p_curr_proc_data->t)
            {
                /* OPTION3X Changes End */
#endif
                if (p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                        erab_op_params.m.mac_main_config_present &&
                        p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                        erab_op_params.m.mac_config_present)
                {
                    p_asn1_rrc->m.mac_MainConfigPresent = 1;

                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_INFO,"ERAB_RCR: mac_main_config present");

                    /*SPR_16942_START*/
                    if (RRC_SUCCESS != uecc_llim_build_asn1_mac_main_config(
                                p_uecc_ue_context,
                                &(p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                                    erab_op_params.mac_config),
                                &p_asn1_rrc->mac_MainConfig,
                                p_asn1_ctx))
                        /*SPR_16942_END*/                            
                    {
                        break;
                    }
                }
#ifdef ENDC_ENABLED
                /* OPTION3X Changes Start */
            }
            /* OPTION3X Changes End */
#endif

#ifdef ENDC_ENABLED
            if(UECC_RRM_DC_BEARER_CHANGE_PROC == p_uecc_ue_context->p_curr_proc_data->t)
            {
                p_asn1_rrc->m.drb_ToReleaseListPresent = 1;

                /* Generate DRB Information */
                if (RRC_SUCCESS != uecc_llim_build_asn1_setup_drb_to_release_list(
                            p_uecc_ue_context,
                            &p_asn1_rrc->drb_ToReleaseList,
                            p_asn1_ctx))
                {
                    break;
                }
            }
            else
#endif
            {
                /* ARP CHANGES START */
                /* DRB_ToReleaseList */
                if(RRC_TRUE == 
                        p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.is_erab_del_ongoing)
                {
                    if (0 < p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.
                            erab_release_command_data.erab_release_list.counter)
                    {
                        p_asn1_rrc->m.drb_ToReleaseListPresent = 1; 

                        /* Generate DRB Information */
                        if (RRC_SUCCESS != uecc_llim_build_asn1_setup_drb_to_release_list(
                                    p_uecc_ue_context,
                                    &p_asn1_rrc->drb_ToReleaseList,
                                    p_asn1_ctx))
                        {
                            break;
                        }
                    }
                }
            }
        }

		/* ARP CHANGES END */
		/* DRB_ToAddModList */
		if ( 0 < p_uecc_ue_context->e_rab_list_count )
		{
			p_asn1_rrc->m.drb_ToAddModListPresent = 1;

			/* Generate DRB Information */
            /* coverity fix 20072 */
             if ( PNULL != p_uecc_ue_context->p_curr_proc_data )
             {
                 if (RRC_SUCCESS != uecc_llim_build_erb_setup_asn1_drb_to_add_mod_list(
                             p_uecc_ue_context,
                             &p_asn1_rrc->drb_ToAddModList,
                             p_asn1_ctx))
                 {
                     break;
                 }
             }
             else
             {
                 RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                         p_uecc_ue_context->p_gb_context->facility_name,
                         RRC_ERROR,"ERBSP_RCR: SGNB Addition: current proc data in UE context is NULL");

                 break;
             }
             /* coverity fix 20072 */

#ifdef ENDC_ENABLED
            /*NR_DC Code Changes Start*/
            if(p_uecc_ue_context->is_sgnb_add_reqd &&
                    RRC_NULL == p_asn1_rrc->drb_ToAddModList.count)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name,
                        RRC_INFO,"ERBSP_RCR: SGNB Addition: All the erAB are SN_TERMINATED_SCG_BEARER");

                p_asn1_rrc->m.drb_ToAddModListPresent = 0;
            }
            /*NR_DC Code Changes Stop*/
#endif
		}

		/* SPS Configuration */
		if ( p_uecc_ue_context->m.sps_config_updated )
		{
			p_asn1_rrc->m.sps_ConfigPresent = 1;

			if (RRC_SUCCESS != uecc_llim_build_asn1_sps_config(
						p_uecc_ue_context,
						/* SPS Start */
						p_uecc_ue_context->p_sps_config,
						/* SPS Stop */
						&p_asn1_rrc->sps_Config,
						p_asn1_ctx))
			{
				break;
			}
		}

		/* PhysicalConfigDedicated */
		if (  p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.erab_op_params.m.physical_config_dedicated_present )
		{
			p_asn1_rrc->m.physicalConfigDedicatedPresent = 1;

			if (RRC_SUCCESS != uecc_llim_build_asn1_physical_config_dedicated(
						p_uecc_ue_context,
						&(p_uecc_ue_context->p_curr_proc_data->u.erab_setup_request_data.erab_op_params.physical_config_dedicated),
						&p_asn1_rrc->physicalConfigDedicated,
						p_asn1_ctx)
			   )
			{
				break;
			}
		}


		/* CR_220 */
		/* RLF TIMERS & CONSTANTS*/
		if( p_uecc_ue_context->m.rlf_timers_and_constants_r9_present )
		{
			if ( PNULL != p_uecc_ue_context->p_rlf_timers_and_constants_r9 )
			{
				p_asn1_rrc->m.rlf_TimersAndConstants_r9Present = 1;

				p_asn1_rrc->m._v2ExtPresent = 1;
				if (RRC_SUCCESS != uecc_llim_build_asn1_rlf_timers_and_constants_r9(
							p_uecc_ue_context, 
							(p_uecc_ue_context->p_rlf_timers_and_constants_r9),
							&p_asn1_rrc->rlf_TimersAndConstants_r9,
							p_asn1_ctx)
				   )
				{
					break;
				}
			}
		} 

		/* main extElem1 */
		rtxDListInit(&p_asn1_rrc->extElem1);

		result = RRC_SUCCESS;
	}
	while(0);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return result;
}



/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_erb_setup_asn1_drb_to_add_mod_list
 *   INPUT        : uecc_ue_context_t*      p_uecc_ue_context,
 *                  DRB_ToAddModList*       p_asn1_list,
 *                  OSCTXT*                 p_asn1_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION:    This function fills 'DRB_ToAddModList' from information
 *                   of 'rrm_drb_to_add_info_list_t'
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_erb_setup_asn1_drb_to_add_mod_list(
		uecc_ue_context_t*      p_uecc_ue_context,
		DRB_ToAddModList*       p_asn1_list,
		OSCTXT*                 p_asn1_ctx
		)
{
	DRB_ToAddMod                    *p_asn1_drb_elem = PNULL;
	OSRTDListNode                   *p_node = PNULL;

	/* ICS START */

	U8 erab_id_count = 0;
	U8 erab_counter  = 0;
	U8 erab_id = 0;

	/* ICS STOP */

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	rtxDListInit(p_asn1_list);

	if (p_uecc_ue_context->e_rab_list_count > MAX_ERAB_COUNT)
	{
		RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
				p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
				"generate_drb_info_list: invalid drb_count=%i",
				p_uecc_ue_context->e_rab_list_count);
		return RRC_FAILURE;
	}

	/* ICS STOP */

    /* SPR 18253 Fix Start */
    if ( (p_uecc_ue_context->ho_info.p_ho_info) &&
            (INVALID_HO != p_uecc_ue_context->ho_info.s1_or_x2_handover) )
    /* SPR 18253 Fix Stop */
	{
		for ( erab_counter = 0; erab_counter < MAX_ERAB_COUNT; erab_counter++ )
		{
			if (!p_uecc_ue_context->p_e_rab_list[erab_counter])
			{
				continue;
			}

			rtxDListAllocNodeAndData(p_asn1_ctx, DRB_ToAddMod,
					&p_node, &p_asn1_drb_elem);
			if (PNULL == p_node)
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
						"generate_drb_info_list: ASN malloc failed");
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				return RRC_FAILURE;
			}

			if (RRC_SUCCESS !=
					uecc_llim_build_asn1_drb_to_add_mod(
						p_uecc_ue_context,
						&p_uecc_ue_context->p_e_rab_list[erab_counter]->drb_config,
						p_asn1_drb_elem,
						p_asn1_ctx))
			{
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				return RRC_FAILURE;
			}
			/* append node to the list */
			rtxDListAppendNode(p_asn1_list, p_node);
		}
	}
	else
	{
		if (PNULL != p_uecc_ue_context->p_curr_proc_data)
		{

#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
        if( UECC_RRM_DC_BEARER_CHANGE_PROC !=
                p_uecc_ue_context->p_curr_proc_data->t)
        {
/* OPTION3X Changes End */
#endif
			erab_id_count = p_uecc_ue_context->p_curr_proc_data->u.
				erab_setup_request_data.erab_info_data.
				count;
#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
        }
        else
        {
			erab_id_count = p_uecc_ue_context->p_curr_proc_data->u.
				rrm_dc_bearer_change_request_data.drb_to_modify_list.
				drb_count;
        }
/* OPTION3X Changes End */
#endif
			for ( erab_counter = 0; erab_counter < erab_id_count; erab_counter++ )
			{

				if (
#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
                        (
                         UECC_RRM_DC_BEARER_CHANGE_PROC !=
                         p_uecc_ue_context->p_curr_proc_data->t
                        &&
/* OPTION3X Changes End */
#endif
                        RRC_NO_ERROR != p_uecc_ue_context->p_curr_proc_data->u.
						erab_setup_request_data.erab_info_data.
						erab_info[erab_counter].status
#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
                        )
                        ||
                        (
                         (UECC_RRM_DC_BEARER_CHANGE_PROC ==
                         p_uecc_ue_context->p_curr_proc_data->t)
                         &&
                         ((RRC_TRUE == p_uecc_ue_context->p_curr_proc_data->u.
                         rrm_dc_bearer_change_request_data.drb_to_modify_list.
                         drb_config[erab_counter].erab_failed)
                         ||
                         (RRC_FALSE == p_uecc_ue_context->p_curr_proc_data->u.
                         rrm_dc_bearer_change_request_data.drb_to_modify_list.
                         drb_config[erab_counter].erab_success))

                        )
/* OPTION3X Changes End */
#endif
                   )
				{
					continue;
				}

#ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
                if( UECC_RRM_DC_BEARER_CHANGE_PROC !=
                        p_uecc_ue_context->p_curr_proc_data->t)
                {
/* OPTION3X Changes End */
#endif
				erab_id = p_uecc_ue_context->p_curr_proc_data->u.
					erab_setup_request_data.erab_info_data.
					erab_info[erab_counter].erab_id;
 #ifdef ENDC_ENABLED
/* OPTION3X Changes Start */
                }
                else
                {
                    erab_id = p_uecc_ue_context->p_curr_proc_data->u.
                        rrm_dc_bearer_change_request_data.drb_to_modify_list.
                        drb_config[erab_counter].erab_id;
                }
/* OPTION3X Changes End */
#endif
               
#ifdef ENDC_ENABLED
                /*NR_DC Code Changes Start*/
                /*check if the dc_bearer_type is SN_TERMINATED_SCG_BEARER*/
                if(SN_TERMINATED_SCG_BEARER ==
                        p_uecc_ue_context->p_e_rab_list[erab_id]->dc_bearer_type)
                {
                    continue;
                }
                /*NR_DC Code Changes Stop*/
#endif

				rtxDListAllocNodeAndData(p_asn1_ctx, DRB_ToAddMod,
						&p_node, &p_asn1_drb_elem);
				if (PNULL == p_node)
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
							"generate_drb_info_list: ASN malloc failed");
					RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
					return RRC_FAILURE;
				}
				if (RRC_SUCCESS !=
						uecc_llim_build_asn1_drb_to_add_mod(
							p_uecc_ue_context,
							&p_uecc_ue_context->p_e_rab_list[erab_id]->drb_config,
							p_asn1_drb_elem,
							p_asn1_ctx))
				{
					RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
					return RRC_FAILURE;
				}

				/* append node to the list */
				rtxDListAppendNode(p_asn1_list, p_node);
			}
		}
	}
	/* ICS STOP */

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return RRC_SUCCESS;
}


/* ERAB SETUP STOP */

#ifdef ENDC_ENABLED
/*****************************************************************************
 *   FUNCTION NAME: uecc_llim_build_black_listed_cells_to_add_mod_list_nr_r15
 *   INPUT        : uecc_gb_context_t               *p_uecc_gb_context
 *                  CellsToAddModListNR_r15
 *                      *p_black_cells_to_add_mod_list_r15
 *                  black_cells_to_add_mod_list_r15_t
 *                       *p_rrm_black_cells_to_add_mod_list_r15
 *                  OSCTXT*                        p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : This function builds CellsToAddModNR in MeasConfig structure
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et
uecc_llim_build_black_listed_cells_to_add_mod_list_nr_r15
(
    uecc_gb_context_t                 *p_uecc_gb_context,
	CellsToAddModListNR_r15*          p_black_cells_to_add_mod_list_r15,
	black_cells_to_add_mod_list_r15_t *p_rrm_black_cells_to_add_mod_list_r15,
	OSCTXT*                           p_asn1_ctx
)
{
	rrc_return_et           result = RRC_SUCCESS;
	CellsToAddModNR_r15*    p_black_cells_add_mod_nr = PNULL;
	OSRTDListNode*          p_node = PNULL;
	U8                      count = RRC_NULL;

	RRC_ASSERT(PNULL != p_rrm_black_cells_to_add_mod_list_r15);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_black_cells_to_add_mod_list_r15);

	RRC_UECC_UT_TRACE_ENTER(p_uecc_gb_context);

    RRC_UECC_TRACE(p_uecc_gb_context->context_index, p_uecc_gb_context->facility_name, 
                       RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_gb_context=%p",p_uecc_gb_context);

	rtxDListInit(p_black_cells_to_add_mod_list_r15);

	for ( count = RRC_NULL; count < p_rrm_black_cells_to_add_mod_list_r15->
        count; count++ )
	{
		rtxDListAllocNodeAndData(
				p_asn1_ctx,
				CellsToAddModNR_r15,
				&p_node,
				&p_black_cells_add_mod_nr);
		if ( PNULL == p_node )
		{
			result = RRC_FAILURE;
			break;
		}

		p_black_cells_add_mod_nr->cellIndex_r15 =
			p_rrm_black_cells_to_add_mod_list_r15->
			black_cells_to_add_mod[count].cell_index_r15;

		p_black_cells_add_mod_nr->physCellId_r15 =
			p_rrm_black_cells_to_add_mod_list_r15->
			black_cells_to_add_mod[count].phys_cellid_r15;

		/* append node to the list */
		rtxDListAppendNode(p_black_cells_to_add_mod_list_r15, p_node);
	}

	RRC_UECC_UT_TRACE_EXIT(p_uecc_gb_context);
	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_MeasObjectNR_r15
 *   INPUT        : uecc_ue_context_t*       p_uecc_ue_context,
 *                  meas_object_nr_t*        p_rrm_meas_object_nr_r15,
 *                  OSCTXT*                  p_asn1_ctx,
 *                  MeasObjectNR_r15*        p_meas_object_nr_r15
 *   OUTPUT       : none
 *   DESCRIPTION  : This function fills Meas Object NR r15 Parameters
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 ******************************************************************************/
rrc_return_et
uecc_llim_build_MeasObjectNR_r15
(
    uecc_ue_context_t*       p_uecc_ue_context,
	meas_object_nr_t*        p_rrm_meas_object_nr_r15,
	OSCTXT*                  p_asn1_ctx,
	MeasObjectNR_r15*        p_meas_object_nr_r15
)
{
    U8                  cell_count = RRC_NULL;
    U16                 sftd_count = RRC_NULL;
    MTC_SSB_NR_r15      *p_asn_meas_timing_config = PNULL;
    mtc_ssb_nr_r15_t    *p_rrm_meas_timing_config = PNULL;

	RRC_ASSERT(PNULL != p_rrm_meas_object_nr_r15);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_nr_r15);

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_MeasObjectNR_r15(p_meas_object_nr_r15);

		/* Fill Carrier Frequency */
		p_meas_object_nr_r15->carrierFreq_r15 = 
			p_rrm_meas_object_nr_r15->carrier_freq_r15;

        p_asn_meas_timing_config = &(p_meas_object_nr_r15->
            rs_ConfigSSB_r15.measTimingConfig_r15);
        p_rrm_meas_timing_config = &(p_rrm_meas_object_nr_r15->
            rs_config_ssb_nr_r15.meas_timing_config_r15);

		/* Fill RS config SSB NR periodicity & offset */
        if ( PERIODICITY_AND_OFFSET_SF5_PRESENT &
            p_rrm_meas_timing_config->periodicity_and_offfset_r15.bitmask )
        {
            p_asn_meas_timing_config->periodicityAndOffset_r15.t =
                T_MTC_SSB_NR_r15_periodicityAndOffset_r15_sf5_r15;
 
            p_asn_meas_timing_config->periodicityAndOffset_r15.u.sf5_r15 = 
                p_rrm_meas_timing_config->periodicity_and_offfset_r15.sf5_r15;
        }
        else if ( PERIODICITY_AND_OFFSET_SF10_PRESENT &
            p_rrm_meas_timing_config->periodicity_and_offfset_r15.bitmask )
        {
            p_asn_meas_timing_config->periodicityAndOffset_r15.t =
                T_MTC_SSB_NR_r15_periodicityAndOffset_r15_sf10_r15;
 
            p_asn_meas_timing_config->periodicityAndOffset_r15.u.sf10_r15 = 
                p_rrm_meas_timing_config->periodicity_and_offfset_r15.sf10_r15;
        }
        else if ( PERIODICITY_AND_OFFSET_SF20_PRESENT &
            p_rrm_meas_timing_config->periodicity_and_offfset_r15.bitmask )
        {
            p_asn_meas_timing_config->periodicityAndOffset_r15.t =
                T_MTC_SSB_NR_r15_periodicityAndOffset_r15_sf20_r15;
 
            p_asn_meas_timing_config->periodicityAndOffset_r15.u.sf20_r15 = 
                p_rrm_meas_timing_config->periodicity_and_offfset_r15.sf20_r15;
        }
        else if ( PERIODICITY_AND_OFFSET_SF40_PRESENT &
            p_rrm_meas_timing_config->periodicity_and_offfset_r15.bitmask )
        {
            p_asn_meas_timing_config->periodicityAndOffset_r15.t =
                T_MTC_SSB_NR_r15_periodicityAndOffset_r15_sf40_r15;
 
            p_asn_meas_timing_config->periodicityAndOffset_r15.u.sf40_r15 = 
                p_rrm_meas_timing_config->periodicity_and_offfset_r15.sf40_r15;
        }
        else if ( PERIODICITY_AND_OFFSET_SF80_PRESENT &
            p_rrm_meas_timing_config->periodicity_and_offfset_r15.bitmask )
        {
            p_asn_meas_timing_config->periodicityAndOffset_r15.t =
                T_MTC_SSB_NR_r15_periodicityAndOffset_r15_sf80_r15;
 
            p_asn_meas_timing_config->periodicityAndOffset_r15.u.sf80_r15 = 
                p_rrm_meas_timing_config->periodicity_and_offfset_r15.sf80_r15;
        }
        else if ( PERIODICITY_AND_OFFSET_SF160_PRESENT &
            p_rrm_meas_timing_config->periodicity_and_offfset_r15.bitmask )
        {
            p_asn_meas_timing_config->periodicityAndOffset_r15.t =
                T_MTC_SSB_NR_r15_periodicityAndOffset_r15_sf160_r15;
 
            p_asn_meas_timing_config->periodicityAndOffset_r15.u.sf160_r15 = 
                p_rrm_meas_timing_config->periodicity_and_offfset_r15.sf160_r15;
        }

		/* Fill RS config SSB NR duration */
		p_asn_meas_timing_config->ssb_Duration_r15 = 
			p_rrm_meas_timing_config->ssb_duration_r15;

		/* Fill RS config SSB NR subcarrier spacing SSB */
		p_meas_object_nr_r15->rs_ConfigSSB_r15.subcarrierSpacingSSB_r15 = 
			p_rrm_meas_object_nr_r15->rs_config_ssb_nr_r15.
            subcarrier_spacing_ssb_r15;

		/* Fill Offset Freq */
        p_meas_object_nr_r15->offsetFreq_r15 = 
            p_rrm_meas_object_nr_r15->offset_freq_r15;

		/* Fill Quantity config set */
        p_meas_object_nr_r15->quantityConfigSet_r15 = 
            p_rrm_meas_object_nr_r15->quantity_config_set_r15;

		/* Fill Threshold RS index */
        if ( MEAS_OBJECT_NR_THRESHHOLD_RS_INDEX_PRESENT &
            p_rrm_meas_object_nr_r15->bitmask )
        {
            p_meas_object_nr_r15->m.threshRS_Index_r15Present = RRC_ONE;

            if ( THRESHOLD_LIST_NR_RSRP_PRESENT &
                p_rrm_meas_object_nr_r15->thresh_rs_index_r15.bitmask )
            {
                p_meas_object_nr_r15->threshRS_Index_r15.m.nr_RSRP_r15Present =
                    RRC_ONE;
 
                p_meas_object_nr_r15->threshRS_Index_r15.nr_RSRP_r15 = 
                    p_rrm_meas_object_nr_r15->thresh_rs_index_r15.rsrp_nr_r15;
            }

            if ( THRESHOLD_LIST_NR_RSRQ_PRESENT &
                p_rrm_meas_object_nr_r15->thresh_rs_index_r15.bitmask )
            {
                p_meas_object_nr_r15->threshRS_Index_r15.m.nr_RSRQ_r15Present =
                    RRC_ONE;
 
                p_meas_object_nr_r15->threshRS_Index_r15.nr_RSRQ_r15 = 
                    p_rrm_meas_object_nr_r15->thresh_rs_index_r15.rsrq_nr_r15;
            }

            if ( THRESHOLD_LIST_NR_SINR_PRESENT &
                p_rrm_meas_object_nr_r15->thresh_rs_index_r15.bitmask )
            {
                p_meas_object_nr_r15->threshRS_Index_r15.m.nr_SINR_r15Present =
                    RRC_ONE;
 
                p_meas_object_nr_r15->threshRS_Index_r15.nr_SINR_r15 = 
                    p_rrm_meas_object_nr_r15->thresh_rs_index_r15.sinr_nr_r15;
            }
        }

		/* Fill Max RS index cell Qual */
        if ( MEAS_OBJECT_NR_MAX_RS_IDX_CELL_QUAL_PRESENT &
            p_rrm_meas_object_nr_r15->bitmask )
        {
            p_meas_object_nr_r15->m.maxRS_IndexCellQual_r15Present =
                RRC_ONE; 

            p_meas_object_nr_r15->maxRS_IndexCellQual_r15 = 
                p_rrm_meas_object_nr_r15->max_rs_index_cell_qual_r15;
        }

		/* Fill Black cells to remove list */
        if (  MEAS_OBJECT_NR_BLACK_CELLS_TO_REMOVE_LIST_PRESENT &
            p_rrm_meas_object_nr_r15->bitmask )
        {
            p_meas_object_nr_r15->m.blackCellsToRemoveList_r15Present =
                RRC_ONE; 

            p_meas_object_nr_r15->blackCellsToRemoveList_r15.n =
                p_rrm_meas_object_nr_r15->black_cells_to_remove_list.count;

            for ( cell_count = RRC_NULL; cell_count < p_rrm_meas_object_nr_r15->
                black_cells_to_remove_list.count; cell_count++ )
            { 
                p_meas_object_nr_r15->blackCellsToRemoveList_r15.elem[cell_count] = 
                    p_rrm_meas_object_nr_r15->black_cells_to_remove_list.
                    cell_index[cell_count];
            }
        }

		/* Fill Black cells to add mod list */
        if ( MEAS_OBJECT_NR_BLACK_CELLS_TO_ADD_MOD_LIST_PRESENT &
            p_rrm_meas_object_nr_r15->bitmask )
        {
            p_meas_object_nr_r15->m.blackCellsToAddModList_r15Present =
                RRC_ONE;

            if ( RRC_SUCCESS != uecc_llim_build_black_listed_cells_to_add_mod_list_nr_r15(
                        p_uecc_ue_context->p_gb_context,
                        &p_meas_object_nr_r15->blackCellsToAddModList_r15,
                        &p_rrm_meas_object_nr_r15->black_cells_to_add_mod_list,
                        p_asn1_ctx) )
            {
                break;
            }
        }

		/* Fill cells for which to report SFTD */
        if ( MEAS_OBJECT_NR_CELLS_TO_REPORT_SFTD_PRESENT &
            p_rrm_meas_object_nr_r15->bitmask )
        {
            p_meas_object_nr_r15->m.cellsForWhichToReportSFTD_r15Present =
                RRC_ONE;

            p_meas_object_nr_r15->cellsForWhichToReportSFTD_r15.n =
                p_rrm_meas_object_nr_r15->cells_to_report_SFTD_r15_list.count;

            for ( sftd_count = RRC_NULL; sftd_count < p_rrm_meas_object_nr_r15->
                cells_to_report_SFTD_r15_list.count; sftd_count++ )
            { 
                p_meas_object_nr_r15->cellsForWhichToReportSFTD_r15.elem[sftd_count] = 
                    p_rrm_meas_object_nr_r15->cells_to_report_SFTD_r15_list.
                    cells_to_report_SFTD_r15[sftd_count];
            }
        }

	}while(RRC_NULL);

	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return RRC_SUCCESS;
}
#endif

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_MeasObjectGERAN
 *   INPUT        : uecc_ue_context_t*       p_uecc_ue_context,
 *                  meas_object_geran_t*     p_rrm_meas_object_geran,
 *                  OSCTXT*                     p_asn1_ctx,
 *                  MeasObjectGERAN*         p_meas_object_geran
 *   OUTPUT       : none
 *   DESCRIPTION:    This function fills Meas Object GERAN Parameters
 *
 *   RETURNS:
 *       RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_MeasObjectGERAN(
		uecc_ue_context_t*       p_uecc_ue_context,
		meas_object_geran_t*     p_rrm_meas_object_geran,
		OSCTXT*                     p_asn1_ctx,
		MeasObjectGERAN*         p_meas_object_geran
		)
{
	ExplicitListOfARFCNs *explicitListOfARFCNs = PNULL;
	CarrierFreqsGERAN_followingARFCNs_equallySpacedARFCNs *equallySpacedARFCNs = PNULL;
	CarrierFreqsGERAN_followingARFCNs_variableBitMapOfARFCNs *variableBitMapOfARFCNs = PNULL;



	RRC_ASSERT(PNULL != p_rrm_meas_object_geran);
	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_meas_object_geran);
	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_MeasObjectGERAN(p_meas_object_geran);

		/* Fill Carrier Frequency */

		p_meas_object_geran->carrierFreqs.startingARFCN = 
			p_rrm_meas_object_geran->carrier_freqs.starting_arfcn;

		p_meas_object_geran->carrierFreqs.bandIndicator = 
			p_rrm_meas_object_geran->carrier_freqs.band_indicator;


		if (p_rrm_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask
				& GERAN_EXPL_LIST_OF_ARFCNS_PRESENCE_FLAG)
		{
			explicitListOfARFCNs = 
				rtxMemAllocType(p_asn1_ctx, ExplicitListOfARFCNs);
			/* SPR 12964 Fix Start */
			if (PNULL == explicitListOfARFCNs)
			{
				return RRC_FAILURE;
			}
			/* SPR 12964 Fix Stop */

			explicitListOfARFCNs->n =
				p_rrm_meas_object_geran->carrier_freqs.following_arfcns.
				explicit_list_of_arfcns.count;


			if (explicitListOfARFCNs->n <= 31)
			{
				l3_memcpy_wrapper(explicitListOfARFCNs->elem,
						p_rrm_meas_object_geran->carrier_freqs.following_arfcns.
						explicit_list_of_arfcns.data,
						sizeof(arfcn_value_geran_t)*explicitListOfARFCNs->n);
			}
			else
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
						"Invalid No of Elements explicitListOfARFCNs->elem = %u",
						explicitListOfARFCNs->n);
				return RRC_FAILURE; 
			}

			p_meas_object_geran->carrierFreqs.followingARFCNs.t = 
				T_CarrierFreqsGERAN_followingARFCNs_explicitListOfARFCNs;

			p_meas_object_geran->carrierFreqs.followingARFCNs.u.explicitListOfARFCNs = 
				explicitListOfARFCNs;
		}
		else if (p_rrm_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask
				& GERAN_EQ_SPACED_ARFCNS_PRESENCE_FLAG)
		{

			equallySpacedARFCNs = 
				rtxMemAllocType(p_asn1_ctx, 
						CarrierFreqsGERAN_followingARFCNs_equallySpacedARFCNs);
			/* SPR 12964 Fix Start */
			if (PNULL == equallySpacedARFCNs)
			{
				return RRC_FAILURE;
			}
			/* SPR 12964 Fix Stop */

			equallySpacedARFCNs->arfcn_Spacing = p_rrm_meas_object_geran->
				carrier_freqs.following_arfcns.equally_spaced_arfcns.arfcn_spacing;

			equallySpacedARFCNs->numberOfFollowingARFCNs = p_rrm_meas_object_geran->
				carrier_freqs.following_arfcns.equally_spaced_arfcns.num_of_following_arfcns;

			p_meas_object_geran->carrierFreqs.followingARFCNs.t = 
				T_CarrierFreqsGERAN_followingARFCNs_equallySpacedARFCNs;

			p_meas_object_geran->carrierFreqs.followingARFCNs.u.equallySpacedARFCNs = 
				equallySpacedARFCNs;

		} 
		else if (p_rrm_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask
				& GERAN_VAR_BITMAP_ARFCNS_PRESENCE_FLAG)
		{

			variableBitMapOfARFCNs = 
				rtxMemAllocType(p_asn1_ctx, 
						CarrierFreqsGERAN_followingARFCNs_variableBitMapOfARFCNs);
			/* SPR 12964 Fix Start */
			if (PNULL == variableBitMapOfARFCNs)
			{
				return RRC_FAILURE;
			}
			/* SPR 12964 Fix Stop */

			variableBitMapOfARFCNs->numocts = p_rrm_meas_object_geran->
				carrier_freqs.following_arfcns.var_bitmap_of_arfcns.count;

			l3_memcpy_wrapper(variableBitMapOfARFCNs->data,
					p_rrm_meas_object_geran->carrier_freqs.following_arfcns.
					var_bitmap_of_arfcns.data,
					MAX_VAR_BITMAP_OF_ARFCNS);

			p_meas_object_geran->carrierFreqs.followingARFCNs.t = 
				T_CarrierFreqsGERAN_followingARFCNs_variableBitMapOfARFCNs;

			p_meas_object_geran->carrierFreqs.followingARFCNs.u.variableBitMapOfARFCNs = 
				variableBitMapOfARFCNs;
		}
		else
		{
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
					"Invalid Presence Bitmask of geran_following_arfcns_t = %u",
					p_rrm_meas_object_geran->carrier_freqs.following_arfcns.presence_bitmask);

		}


		/* Fill Offset Frequency */
		p_meas_object_geran->offsetFreq = p_rrm_meas_object_geran->offset_freq;

		p_meas_object_geran->m.ncc_PermittedPresent = 1;

		/* Fill ncc_Permitted */

		p_meas_object_geran->m.ncc_PermittedPresent = 1;

		p_meas_object_geran->ncc_Permitted.numbits = 8; /* FIXED As in 36.331*/

		p_meas_object_geran->ncc_Permitted.data[0] = 
			p_rrm_meas_object_geran->ncc_Permitted;


		/* Fill Physical Cell Id */
		if ( MEAS_OBJECT_GERAN_CELL_FOR_WHICH_TO_REPORT_CGI_PRESENT &
				p_rrm_meas_object_geran->bitmask )
		{
			p_meas_object_geran->m.cellForWhichToReportCGIPresent = 1;    

			/* Fill Physical Cell Id */
			p_meas_object_geran->cellForWhichToReportCGI.networkColourCode.numbits =
				3;
			/*SPR_17668_START*/
			p_meas_object_geran->cellForWhichToReportCGI.networkColourCode.data[0] = 
				(p_rrm_meas_object_geran->cell_for_which_to_report_cgi.network_colour_code <<5)&0xe0;
			/*SPR_17668_STOP*/

			p_meas_object_geran->cellForWhichToReportCGI.baseStationColourCode.numbits
				= 3;
			/*SPR_17668_START*/
			p_meas_object_geran->cellForWhichToReportCGI.baseStationColourCode.data[0] = 
				(p_rrm_meas_object_geran->cell_for_which_to_report_cgi.base_station_colour_code <<5)&0xe0;
			/*SPR_17668_STOP*/

		}
	}while(0);


	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return RRC_SUCCESS;

}

/******************************************************************************
 *   FUNCTION NAME : uecc_llim_build_asn1_rlf_timers_and_constants_r9
 *   INPUT         : uecc_ue_context_t*             p_uecc_ue_context,
 *                   rlf_timers_and_constants_r9_t *p_rlf,
 *                   RLF_TimersAndConstants_r9     *p_asn_rlf,
 *                   OSCTXT                        *p_asn1_ctx
 *   OUTPUT        : none
 *   DESCRIPTION   : This function fills rlf_timers_and_constants_r9 field for 
 *                   radio_resource_config_dedicated.
 *
 *   RETURNS       : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_rlf_timers_and_constants_r9(
		uecc_ue_context_t*             p_uecc_ue_context,
		rlf_timers_and_constants_r9_t *p_rlf,
		RLF_TimersAndConstants_r9     *p_asn_rlf,
		OSCTXT                        *p_asn1_ctx
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	asn1Init_RLF_TimersAndConstants_r9(p_asn_rlf);

	if(RLF_TIMERS_AND_CONSTANTS_SETUP_PRESENT & p_rlf->bitmask)
	{

		p_asn_rlf->t = T_RLF_TimersAndConstants_r9_setup;

		p_asn_rlf->u.setup = 
			rtxMemAllocType(p_asn1_ctx, RLF_TimersAndConstants_r9_setup);

		if (PNULL == p_asn_rlf->u.setup)
		{
			RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
					p_uecc_ue_context->p_gb_context->facility_name, 
					RRC_WARNING,"Memalloc failed.");
			result = RRC_FAILURE;
			return result;
		}

		p_asn_rlf->u.setup->t301_r9 = p_rlf->setup.t301_r9;

		p_asn_rlf->u.setup->t310_r9 = p_rlf->setup.t310_r9;

		p_asn_rlf->u.setup->n310_r9 = p_rlf->setup.n310_r9;

		p_asn_rlf->u.setup->t311_r9 = p_rlf->setup.t311_r9;

		p_asn_rlf->u.setup->n311_r9 = p_rlf->setup.n311_r9;

		//External Element: p_asn_rlf->u.setup->extElem1
		rtxDListInit(&p_asn_rlf->u.setup->extElem1);    


	}
	else
	{
		p_asn_rlf->t = T_RLF_TimersAndConstants_r9_release;
	}


	result = RRC_SUCCESS;
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return result;
}
/******************************************************************************
 *   FUNCTION NAME:   uecc_llim_build_asn1_radio_resource_cfg_dedicated_during_src_ho
 *   INPUT        :   uecc_ue_context_t               *p_uecc_ue_context,
 *                    RadioResourceConfigDedicated    *p_asn1_rrc,
 *                    rrm_srb_info_t                  *p_srb_info,
 *                    OSCTXT                          *p_asn1_ctx
 *   OUTPUT      :    None
 *   DESCRIPTION:
 *       This function builds RadioResourceConfigDedicated
 *       structure for ASN1 encoding from given ue_adm_radio_res_config_t
 *
 *   RETURNS:
 *       RRC_FAILURE/RRC_SUCCESS
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_radio_resource_cfg_dedicated_during_src_ho(
		uecc_ue_context_t               *p_uecc_ue_context,
		RadioResourceConfigDedicated    *p_asn1_rrc,
		rrm_srb_info_t                  *p_srb_info,
		OSCTXT                          *p_asn1_ctx
		)
{
	rrc_return_et result = RRC_FAILURE;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		asn1Init_RadioResourceConfigDedicated(p_asn1_rrc);

		/* SRB_ToAddModList */ 
		if (PNULL != p_srb_info &&
				p_uecc_ue_context->m.srb2_config_present)
		{
			p_asn1_rrc->m.srb_ToAddModListPresent = 1;

			/* Generate SRB Information */
			if (RRC_SUCCESS != uecc_llim_build_asn1_srb_to_add_mod_list(
						/* CSR 00070192 Fix Start */
						p_uecc_ue_context,
						/* CSR 00070192 Fix Stop */
						&p_asn1_rrc->srb_ToAddModList,
						p_srb_info,
						p_asn1_ctx))
			{

				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
						"[s1ap_HandoverRequired] Fail to fill "
						"srb_to_add_mod_list Parameter");
				break;
			}
		}

		/* RadioResourceConfigDedicated_mac_MainConfig */
		if ( p_uecc_ue_context->m.mac_config_present )
		{
			if (p_uecc_ue_context->m.mac_main_config_present)
			{
				p_asn1_rrc->m.mac_MainConfigPresent = 1;

				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_INFO,"[src_ho] mac_main_config present");
				if (RRC_SUCCESS != uecc_llim_build_asn1_mac_main_config(
							p_uecc_ue_context,
							&(p_uecc_ue_context->mac_config),
							&p_asn1_rrc->mac_MainConfig,
							p_asn1_ctx))
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
							"[s1ap_HandoverRequired] Fail to fill "
							"mac_main_config Parameter");
					break;
				}
			}
		}

		/* DRB_ToAddModList */
		if ( 0 < p_uecc_ue_context->e_rab_list_count )
		{
			p_asn1_rrc->m.drb_ToAddModListPresent = 1;

			/* Generate DRB Information */
			if (RRC_SUCCESS != uecc_llim_build_erb_setup_asn1_drb_to_add_mod_list(
						p_uecc_ue_context,
						&p_asn1_rrc->drb_ToAddModList,
						p_asn1_ctx))
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
						"[s1ap_HandoverRequired] Fail to fill "
						"drb_to_add_mod_list Parameter");
				break;
			}
		}

		/* SPS Configuration */
		if ( p_uecc_ue_context->m.sps_config_present )
		{
			p_asn1_rrc->m.sps_ConfigPresent = 1;

			if (RRC_SUCCESS != uecc_llim_build_asn1_sps_config(
						p_uecc_ue_context,
						/* SPS Start */
						p_uecc_ue_context->p_sps_config,
						/* SPS Stop */
						&p_asn1_rrc->sps_Config,
						p_asn1_ctx))
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
						"[s1ap_HandoverRequired] Fail to fill "
						"sps_config Parameter");
				break;
			}
		}

		/* PhysicalConfigDedicated */
		if (  p_uecc_ue_context->m.physical_config_dedicated_present )
		{
			p_asn1_rrc->m.physicalConfigDedicatedPresent = 1;

			if (RRC_SUCCESS != uecc_llim_build_asn1_physical_config_dedicated(
						p_uecc_ue_context,
						&(p_uecc_ue_context->physical_config_dedicated),
						&p_asn1_rrc->physicalConfigDedicated,
						p_asn1_ctx)
			   )
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
						"[s1ap_HandoverRequired] Fail to fill "
						"physical_config_dedicated Parameter");
				break;
			}
		}

		/* CR_220 */
		/* RLF TIMERS & CONSTANTS*/
		if( p_uecc_ue_context->m.rlf_timers_and_constants_r9_present )
		{
			if (PNULL != p_uecc_ue_context->p_rlf_timers_and_constants_r9)
			{
				p_asn1_rrc->m.rlf_TimersAndConstants_r9Present = 1;

				p_asn1_rrc->m._v2ExtPresent = 1;


				if (RRC_SUCCESS != uecc_llim_build_asn1_rlf_timers_and_constants_r9(
							p_uecc_ue_context, 
							(p_uecc_ue_context->p_rlf_timers_and_constants_r9),
							&p_asn1_rrc->rlf_TimersAndConstants_r9,
							p_asn1_ctx)
				   )
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name, RRC_ERROR,
							"[s1ap_HandoverRequired] Fail to fill "
							"rlf_timers_and_constants_r9 Parameter");
					break;
				}
			}
		} 

		/* main extElem1 */
		rtxDListInit(&p_asn1_rrc->extElem1);
		result = RRC_SUCCESS;
	}
	while(0);
	RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);

	return result;
}


/* SPS Start */
/*****************************************************************************
 * Function Name  : uecc_llim_fill_sps_config 
 * Inputs         : rrc_sps_config_t        *p_uecc_sps_config
 *                  rrc_sps_config_t         *p_llim_sps_config 
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills sps config
 ********************************************************************************/
void uecc_llim_fill_sps_config(
		rrc_sps_config_t        *p_uecc_sps_config,
		rrc_sps_config_t         *p_llim_sps_config)
{

	if (p_uecc_sps_config->bitmask &
			RRC_SPS_CONFIG_SPS_CONFIG_DL_PRESENT)
	{
		p_llim_sps_config->bitmask |=
			RRC_SPS_CONFIG_SPS_CONFIG_DL_PRESENT;
		p_llim_sps_config->sps_config_dl =
			p_uecc_sps_config->sps_config_dl;
	}
	if (p_uecc_sps_config->bitmask &
			RRC_SPS_CONFIG_SPS_CONFIG_UL_PRESENT)
	{
		p_llim_sps_config->bitmask |=
			RRC_SPS_CONFIG_SPS_CONFIG_UL_PRESENT;
		p_llim_sps_config->sps_config_ul =
			p_uecc_sps_config->sps_config_ul;
	}


}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_add_modify_scell_config
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_scell_to_add_mod_list_t                     *p_scell_add_modify
 *                  SCellToAddModList_r10                           *p_asn1_scell_add_modify_list,
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 SCellToAddModList_r10 IE based on
 *                  default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_add_modify_scell_config
(
 uecc_ue_context_t               *p_uecc_ue_context,
 rrc_scell_to_add_mod_list_t     *p_scell_add_modify,
 SCellToAddModList_r10           *p_asn1_scell_add_modify_list,
 OSCTXT                          *p_asn1_ctx
 )
{

	rrc_return_et      result = RRC_SUCCESS;
	SCellToAddMod_r10  *p_asn1_scell_add_modify = PNULL;
	OSRTDListNode      *p_node = PNULL;
	U8                 count = RRC_NULL;

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_scell_add_modify);
	RRC_ASSERT(PNULL != p_asn1_scell_add_modify_list);


	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	do
	{
		for(count = 0; count < p_scell_add_modify->count; count++)
		{
			rtxDListAllocNodeAndData(p_asn1_ctx,
					SCellToAddMod_r10,
					&p_node,
					&p_asn1_scell_add_modify);
			if (PNULL == p_node)
			{
				RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
				/* Not enough memory */
				result = RRC_FAILURE;
				break;
			}

			asn1Init_SCellToAddMod_r10(p_asn1_scell_add_modify);

			p_asn1_scell_add_modify->sCellIndex_r10 = p_scell_add_modify->rrc_scell_to_add_mod[count].scellIndex;

			if(p_scell_add_modify->rrc_scell_to_add_mod[count].bitmask & 
					RRM_SCELL_ADD_MOD_CELL_IDENTIFICATION_PRESENT)
			{
				p_asn1_scell_add_modify->m.cellIdentification_r10Present = 1;
				p_asn1_scell_add_modify->cellIdentification_r10.physCellId_r10 = 
					p_scell_add_modify->rrc_scell_to_add_mod[count].scell_Id.phy_cell_id;
				p_asn1_scell_add_modify->cellIdentification_r10.dl_CarrierFreq_r10 =
					p_scell_add_modify->rrc_scell_to_add_mod[count].scell_Id.dl_carrier_freq;
			}
			if(p_scell_add_modify->rrc_scell_to_add_mod[count].bitmask & 
					RRM_SCELL_ADD_MOD_RADIO_RES_CONFIG_COMMON_SCELL_PRESENT)
			{
				p_asn1_scell_add_modify->m.radioResourceConfigCommonSCell_r10Present = 1;

				result = uecc_llim_build_asn1_radio_res_config_common_scell(
						p_uecc_ue_context,
						&p_scell_add_modify->rrc_scell_to_add_mod[count].
						radio_res_config_common_scell,
						&p_asn1_scell_add_modify->radioResourceConfigCommonSCell_r10);
                /*SPR 17777 +-*/
				if ( RRC_SUCCESS != result )
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name, 
							RRC_WARNING,"failed to build asn radio"
							"resource config common scell ");
					result = RRC_FAILURE;
					break;
				}
			}
			if(p_scell_add_modify->rrc_scell_to_add_mod[count].bitmask & 
					RRM_SCELL_ADD_MOD_RADIO_RES_CONFIG_DEDICATED_SCELL_PRESENT)
			{
				p_asn1_scell_add_modify->m.radioResourceConfigDedicatedSCell_r10Present = 1;


				result = uecc_llim_build_asn1_radio_res_config_dedicated_scell(
						p_uecc_ue_context,&p_scell_add_modify->rrc_scell_to_add_mod[count].
						radio_res_config_dedicated_scell,&p_asn1_scell_add_modify->
						radioResourceConfigDedicatedSCell_r10,p_asn1_ctx);
				if ( RRC_SUCCESS != result )                    
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name, 
							RRC_WARNING,"failed to build asn radio"
							"resource config dedicated scell ");
					result = RRC_FAILURE;
					break;
				}
			}
			if (0 == rtxDListAppend(p_asn1_ctx,p_asn1_scell_add_modify_list, (void*)p_asn1_scell_add_modify))
			{
				/* Coverity 61051 fix */
				return (rrc_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM); 
			}
		}
	}while(0);

	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_rel_scell_config
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_scell_to_release_list_t                     *p_scell_rel
 *                   SCellToReleaseList_r10                         *p_asn1_scell_rel
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 SCellToReleaseList_r10 IE based on
 *                  default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_rel_scell_config
(
 uecc_ue_context_t               *p_uecc_ue_context,
 rrc_scell_to_release_list_t     *p_scell_rel,
 SCellToReleaseList_r10          *p_asn1_scell_rel
 /*SPR 17777 +-*/
 )
{
	rrc_return_et      result = RRC_SUCCESS;
	U8                 count = 0;

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_scell_rel);
    /*SPR 17777 +-*/

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,p_uecc_ue_context->p_gb_context->facility_name 
                       ,RRC_DETAILEDALL, "Unused variable"
                       "p_uecc_ue_context=%p",p_uecc_ue_context);/*SPR 17777 +-*/
	do
	{
		asn1Init_SCellToReleaseList_r10(p_asn1_scell_rel);
		p_asn1_scell_rel->n = p_scell_rel->count;
		for(count = 0; count < p_asn1_scell_rel->n; count++)
		{
			p_asn1_scell_rel->elem[count] = p_scell_rel->rrc_scell_to_release[count].
				scellIndex;
		}
	}while(0);

	return result;

}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_mac_main_config_v1020
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_mac_main_config_v1020_t const               *p_mac_main_config_v1020
 *                  MAC_MainConfig_mac_MainConfig_v1020             *p_mac_MainConfig_v1020
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 MAC_MainConfig_mac_MainConfig_v1020 IE based on
 *                  default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/

rrc_return_et uecc_llim_build_asn1_mac_main_config_v1020
(
 uecc_ue_context_t                   *p_uecc_ue_context,
 rrc_mac_main_config_v1020_t const   *p_mac_main_config_v1020,
 MAC_MainConfig_mac_MainConfig_v1020 *p_mac_MainConfig_v1020
 /*SPR 17777 +-*/
 )
{
	rrc_return_et result = RRC_SUCCESS;

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_mac_main_config_v1020);
	RRC_ASSERT(PNULL != p_mac_MainConfig_v1020);

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);
	do
	{
		asn1Init_MAC_MainConfig_mac_MainConfig_v1020(p_mac_MainConfig_v1020);
		if(p_mac_main_config_v1020->bitmask &
				RRC_MAC_MAIN_CONFIG_V1020_SCELL_DEACTIVATION_TIMER_PRESENT)
		{
			if(RRC_RADIO_FRAME_INFINITY != p_mac_main_config_v1020->
					scell_deactivation_timer &&
					RRC_TRUE == p_uecc_ue_context->m.scell_config_present)
			{
				p_mac_MainConfig_v1020->m.sCellDeactivationTimer_r10Present = 1;
				p_mac_MainConfig_v1020->sCellDeactivationTimer_r10 = 
					p_mac_main_config_v1020->scell_deactivation_timer;
			}
		}
		if(p_mac_main_config_v1020->bitmask &
				RRC_MAC_MAIN_CONFIG_V1020_EXTENDED_BSR_SIZES_PRESENT)
		{
			p_mac_MainConfig_v1020->m.extendedBSR_Sizes_r10Present = 1;
			p_mac_MainConfig_v1020->extendedBSR_Sizes_r10 = 
				p_mac_main_config_v1020->extended_bsr_sizes;
		}
		if(p_mac_main_config_v1020->bitmask &
				RRC_MAC_MAIN_CONFIG_V1020_EXTENDED_PHR_PRESENT)
		{
			p_mac_MainConfig_v1020->m.extendedPHR_r10Present = 1;
			p_mac_MainConfig_v1020->extendedPHR_r10 = 
				p_mac_main_config_v1020->extended_phr;
		}
	}while(0);

	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_radio_res_config_common_scell
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_radio_res_config_common_scell_t             *p_radio_res_config_common_scell
 *                  RadioResourceConfigCommonSCell_r10              *p_asn1_radio_res_config_common_scell
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 RadioResourceConfigCommonSCell_r10 IE based on
 *                  default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_radio_res_config_common_scell
(
 uecc_ue_context_t                   *p_uecc_ue_context,
 rrc_radio_res_config_common_scell_t *p_radio_res_config_common_scell,
 RadioResourceConfigCommonSCell_r10  *p_asn1_radio_res_config_common_scell
 /*SPR 17777 +-*/
 )
{
	rrc_return_et result = RRC_SUCCESS;
	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_radio_res_config_common_scell);
	RRC_ASSERT(PNULL != p_asn1_radio_res_config_common_scell);

    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->facility_name,p_uecc_ue_context->p_gb_context->facility_name,
                          RRC_DETAILEDALL,"Unused variable"
                          "p_uecc_ue_context=%p",p_uecc_ue_context);/*SPR 17777+-*/
	do
	{
		p_asn1_radio_res_config_common_scell->nonUL_Configuration_r10.dl_Bandwidth_r10 =
			p_radio_res_config_common_scell->
			radio_res_common_scell_non_ul_config.dl_bandwidth;

		p_asn1_radio_res_config_common_scell->nonUL_Configuration_r10.antennaInfoCommon_r10.
			antennaPortsCount = p_radio_res_config_common_scell->
			radio_res_common_scell_non_ul_config.antenna_port_count;

		p_asn1_radio_res_config_common_scell->nonUL_Configuration_r10.phich_Config_r10.
			phich_Duration = 
			p_radio_res_config_common_scell->
			radio_res_common_scell_non_ul_config.phich_config.phich_duration;

		p_asn1_radio_res_config_common_scell->
			nonUL_Configuration_r10.phich_Config_r10.phich_Resource = 
			p_radio_res_config_common_scell->
			radio_res_common_scell_non_ul_config.phich_config.phich_resource;

		p_asn1_radio_res_config_common_scell->
			nonUL_Configuration_r10.pdsch_ConfigCommon_r10.referenceSignalPower = 
			p_radio_res_config_common_scell->
			radio_res_common_scell_non_ul_config.pdsch_config_common.ref_signal_power;

		p_asn1_radio_res_config_common_scell->
			nonUL_Configuration_r10.pdsch_ConfigCommon_r10.p_b = 
			p_radio_res_config_common_scell->
			radio_res_common_scell_non_ul_config.pdsch_config_common.pb;
		if(p_radio_res_config_common_scell->
				radio_res_common_scell_non_ul_config.bitmask & 
				RRM_SCELL_COMMOM_NON_UL_TDD_CONFIG_PRESENT)
		{
			p_asn1_radio_res_config_common_scell->
				nonUL_Configuration_r10.m.tdd_Config_r10Present = 1;

			p_asn1_radio_res_config_common_scell->
				nonUL_Configuration_r10.tdd_Config_r10.subframeAssignment = 
				p_radio_res_config_common_scell->
				radio_res_common_scell_non_ul_config.tdd_config.sub_frame_assignment;

			p_asn1_radio_res_config_common_scell->
				nonUL_Configuration_r10.tdd_Config_r10.specialSubframePatterns = 
				p_radio_res_config_common_scell->
				radio_res_common_scell_non_ul_config.tdd_config.special_sub_frame_pattern;
		}
	}while(0);

	return result;
}


/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_radio_res_config_dedicated_scell
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_radio_res_config_dedicated_scell_t          *p_radio_res_config_dedicated_scell
 *                  RadioResourceConfigDedicatedSCell_r10           *p_asn1_radio_res_config_dedicated_scell
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 RadioResourceConfigDedicatedSCell_r10 IE based on
 *                  default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et   uecc_llim_build_asn1_radio_res_config_dedicated_scell
(
 uecc_ue_context_t               *p_uecc_ue_context,
 rrc_radio_res_config_dedicated_scell_t *p_radio_res_config_dedicated_scell,
 RadioResourceConfigDedicatedSCell_r10 *p_asn1_radio_res_config_dedicated_scell,
 OSCTXT                                *p_asn1_ctx
 )
{
	rrc_return_et result = RRC_SUCCESS;
	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_radio_res_config_dedicated_scell);
	RRC_ASSERT(PNULL != p_asn1_radio_res_config_dedicated_scell);

	do
	{

		p_asn1_radio_res_config_dedicated_scell->m.
			physicalConfigDedicatedSCell_r10Present = 1;

		asn1Init_PhysicalConfigDedicatedSCell_r10(&p_asn1_radio_res_config_dedicated_scell->
				physicalConfigDedicatedSCell_r10);

		if(p_radio_res_config_dedicated_scell->bitmask &
				RRM_PHYSICAL_CONFIG_DEDICATED_SCELL_CONFIG_PRESENT)
		{
			p_asn1_radio_res_config_dedicated_scell->physicalConfigDedicatedSCell_r10
				.m.nonUL_Configuration_r10Present = 1;

			if(p_radio_res_config_dedicated_scell->
					physical_config_dedicated_scell.bitmask &   
					RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_NON_UL_CONFIG_PRESENT)
			{
				if(RRC_SUCCESS != uecc_llim_build_asn1_radio_res_config_dedicated_scell_non_ul(p_uecc_ue_context,
							&p_radio_res_config_dedicated_scell->physical_config_dedicated_scell.
							radio_res_dedicated_scell_non_ul_config,
							&p_asn1_radio_res_config_dedicated_scell->physicalConfigDedicatedSCell_r10.nonUL_Configuration_r10,
							p_asn1_ctx))
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name, 
							RRC_WARNING,"failed to build radio resource"
							"config dedicated scell non ul");
					result = RRC_FAILURE;
					break;
				}
			}
			if(p_radio_res_config_dedicated_scell->
					physical_config_dedicated_scell.bitmask &
					RRM_RADIO_RES_CONFIG_DEDICATED_SCELL_UL_CONFIG_PRESENT)
			{
				p_asn1_radio_res_config_dedicated_scell->physicalConfigDedicatedSCell_r10
					.m.ul_Configuration_r10Present = 1;

				if(RRC_SUCCESS != uecc_llim_build_asn1_radio_res_config_dedicated_scell_ul(p_uecc_ue_context,
							&p_radio_res_config_dedicated_scell->physical_config_dedicated_scell.
							radio_res_dedicated_scell_ul_config,
							&p_asn1_radio_res_config_dedicated_scell->physicalConfigDedicatedSCell_r10.ul_Configuration_r10,
							p_asn1_ctx))
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name, 
							RRC_WARNING,"failed to build radio resource config"
							"dedicated scell ul");
					result = RRC_FAILURE;
					break;
				}
			}
		}
	}while(0);

	return result;

}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_radio_res_config_dedicated_scell_non_ul
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_radio_res_dedicated_scell_non_ul_config_t   *p_rrc_radio_res_dedicated_scell_non_ul_config
 *                  PhysicalConfigDedicatedSCell_r10_nonUL_Configuration_r10
 *                                                                  *p_asn1_physical_dedicated_scell_non_ul_config
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 PhysicalConfigDedicatedSCell_r10_nonUL_Configuration_r10
 *                  IE based on default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_radio_res_config_dedicated_scell_non_ul
(
 uecc_ue_context_t               *p_uecc_ue_context,
 rrc_radio_res_dedicated_scell_non_ul_config_t *p_rrc_radio_res_dedicated_scell_non_ul_config,
 PhysicalConfigDedicatedSCell_r10_nonUL_Configuration_r10 *p_asn1_physical_dedicated_scell_non_ul_config,
 OSCTXT                                *p_asn1_ctx
 )
{

	rrc_return_et result = RRC_SUCCESS;

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_rrc_radio_res_dedicated_scell_non_ul_config);
	RRC_ASSERT(PNULL != p_asn1_physical_dedicated_scell_non_ul_config);

	do
	{
		if(p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask &
				RRM_SCELL_DEDICATED_NON_UL_ANTENNA_INFO_PRESENT)
		{
			p_asn1_physical_dedicated_scell_non_ul_config->m.antennaInfo_r10Present = 1;

			if(RRC_SUCCESS != uecc_llim_build_asn1_antenna_info_dedicated_r10(p_uecc_ue_context,
						&p_rrc_radio_res_dedicated_scell_non_ul_config->antenna_info_dedicated_r10,
						&p_asn1_physical_dedicated_scell_non_ul_config->antennaInfo_r10,
						p_asn1_ctx))
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,"failed to build antenna info"
						"dedicated  r10");
				result = RRC_FAILURE;
				break;
			}

		}
		if(p_rrc_radio_res_dedicated_scell_non_ul_config->bitmask &
				RRM_SCELL_DEDICATED_NON_UL_PDSCH_CONFIG_DEDICATED_PRESENT)
		{
			p_asn1_physical_dedicated_scell_non_ul_config->m.pdsch_ConfigDedicated_r10Present = 1;
			p_asn1_physical_dedicated_scell_non_ul_config->pdsch_ConfigDedicated_r10.p_a = 
				p_rrc_radio_res_dedicated_scell_non_ul_config->pdsch_configuration_dedicated.p_a;
		}
	}while(0);

	return result;

}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_radio_res_config_dedicated_scell_ul
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_radio_res_dedicated_scell_ul_config_t       *p_rrc_radio_res_dedicated_scell_ul_config
 *                  PhysicalConfigDedicatedSCell_r10_ul_Configuration_r10
 *                                                                  *p_ul_Configuration_r10
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 PhysicalConfigDedicatedSCell_r10_ul_Configuration_r10 
 *                  IE based on default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_radio_res_config_dedicated_scell_ul
(
 uecc_ue_context_t               *p_uecc_ue_context,
 rrc_radio_res_dedicated_scell_ul_config_t *p_rrc_radio_res_dedicated_scell_ul_config,
 PhysicalConfigDedicatedSCell_r10_ul_Configuration_r10 *p_ul_Configuration_r10,
 OSCTXT                                *p_asn1_ctx
 )
{
	rrc_return_et result = RRC_SUCCESS;
	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_rrc_radio_res_dedicated_scell_ul_config);
	RRC_ASSERT(PNULL != p_ul_Configuration_r10);

    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->facility_name,p_uecc_ue_context->p_gb_context->facility_name,
                          RRC_DETAILEDALL,"Unused variable"
                          "p_uecc_ue_context=%p",p_uecc_ue_context);/*SPR 17777+-*/
	do{
		if(p_rrc_radio_res_dedicated_scell_ul_config->bitmask &
				RRM_SCELL_DEDICATED_UL_CQI_CONFIG_SCELL_PRESENT)
		{
			p_ul_Configuration_r10->m.cqi_ReportConfigSCell_r10Present = 1;

			if(p_rrc_radio_res_dedicated_scell_ul_config->
					cqi_report_config_scell.bitmask &
					RRM_SCELL_CQI_REPORT_CONFIG_APERIODIC_PRESENT)
			{
				p_ul_Configuration_r10->cqi_ReportConfigSCell_r10.m.cqi_ReportModeAperiodic_r10Present = 1;
				p_ul_Configuration_r10->cqi_ReportConfigSCell_r10.cqi_ReportModeAperiodic_r10 = 
					p_rrc_radio_res_dedicated_scell_ul_config->cqi_report_config_scell.cqi_reporting_mode_aperiodic;
			}

			p_ul_Configuration_r10->cqi_ReportConfigSCell_r10.nomPDSCH_RS_EPRE_Offset_r10 =  
				p_rrc_radio_res_dedicated_scell_ul_config->cqi_report_config_scell.nomPDSCH_RS_EPRE_Offset;

			if(p_rrc_radio_res_dedicated_scell_ul_config->
					cqi_report_config_scell.bitmask &
					RRM_SCELL_CQI_REPORT_CONFIG_PERIODIC_PRESENT)
			{
				p_ul_Configuration_r10->cqi_ReportConfigSCell_r10.m.cqi_ReportPeriodicSCell_r10Present = 1;

				if(RRC_SUCCESS != uecc_llim_build_asn1_cqi_report_config_periodic_r10(p_uecc_ue_context,  
							&p_rrc_radio_res_dedicated_scell_ul_config->cqi_report_config_scell.
							cqi_report_periodic_r10,
							&p_ul_Configuration_r10->cqi_ReportConfigSCell_r10.
							cqi_ReportPeriodicSCell_r10,
							p_asn1_ctx))
				{
					RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
							p_uecc_ue_context->p_gb_context->facility_name, 
							RRC_WARNING,"failed to build cqi report config"
							"periodic r10");
					result = RRC_FAILURE;
					break;
				}
			}
			if(p_rrc_radio_res_dedicated_scell_ul_config->cqi_report_config_scell.bitmask &
					RRM_SCELL_CQI_REPORT_CONFIG_PMI_RI_REPORT_PRESENT)
			{
				p_ul_Configuration_r10->cqi_ReportConfigSCell_r10.m.pmi_RI_Report_r10Present = 1;
				p_ul_Configuration_r10->cqi_ReportConfigSCell_r10.pmi_RI_Report_r10 =
					p_rrc_radio_res_dedicated_scell_ul_config->cqi_report_config_scell.cqi_report_pmi_ri_report_r10;

			}
		}
	}while(0);

	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_antenna_info_ul_r10
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_antenna_info_ul_r10_t const                 *p_rrc_antenna_info_ul_r10
 *                  AntennaInfoUL_r10                               *p_asn1_antenna_info_ul_r10
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 AntennaInfoUL_r10 IE based on
 *                  default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_antenna_info_ul_r10
(
 uecc_ue_context_t                 *p_uecc_ue_context,
 rrc_antenna_info_ul_r10_t const   *p_rrc_antenna_info_ul_r10,
 AntennaInfoUL_r10                 *p_asn1_antenna_info_ul_r10
 /*SPR 17777 +-*/
 )
{
	rrc_return_et result = RRC_SUCCESS;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_rrc_antenna_info_ul_r10);
	RRC_ASSERT(PNULL != p_asn1_antenna_info_ul_r10);
    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,p_uecc_ue_context->p_gb_context->facility_name,
                         RRC_DETAILEDALL,"Unused variable"
                         "p_uecc_ue_context=%p",p_uecc_ue_context);/*SPR 17777+-*/
	do
	{
		if(p_rrc_antenna_info_ul_r10->bitmask &
				RRC_ANTENNA_INFO_UL_R10_TRANSMISSION_MODE_PRESENT)
		{
			p_asn1_antenna_info_ul_r10->m.transmissionModeUL_r10Present = 1;
			p_asn1_antenna_info_ul_r10->transmissionModeUL_r10 =
				p_rrc_antenna_info_ul_r10->transmission_mode_ul;
		}
		if(p_rrc_antenna_info_ul_r10->bitmask &
				RRC_ANTENNA_INFO_UL_R10_FOUR_ANTENNA_PORT_ACTIVATED_PRESENT)
		{
			p_asn1_antenna_info_ul_r10->m.fourAntennaPortActivated_r10Present = 1;
			p_asn1_antenna_info_ul_r10->fourAntennaPortActivated_r10 =
				p_rrc_antenna_info_ul_r10->four_antenna_port_activated;
		}
	}while(0);

	return result;

}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_cqi_report_config_r10
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_cqi_report_config_r10_t const               *p_rrc_cqi_report_config_r10
 *                  CQI_ReportConfig_r10                            *p_asn1_cqi_report_config_r10
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 CQI_ReportConfig_r10 IE based on
 *                  default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_cqi_report_config_r10
(
 uecc_ue_context_t                  *p_uecc_ue_context,
 rrc_cqi_report_config_r10_t const  *p_rrc_cqi_report_config_r10,
 CQI_ReportConfig_r10               *p_asn1_cqi_report_config_r10,
 OSCTXT                             *p_asn1_ctx
 )
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_rrc_cqi_report_config_r10);
    RRC_ASSERT(PNULL != p_asn1_cqi_report_config_r10);

    do{
        if(p_rrc_cqi_report_config_r10->bitmask &
                RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_APERIODIC_R10_PRESENT)
        {
            p_asn1_cqi_report_config_r10->m.cqi_ReportAperiodic_r10Present = 1;

            uecc_llim_build_asn1_cqi_report_config_aperiodic_r10(p_uecc_ue_context,
                    &p_rrc_cqi_report_config_r10->cqi_report_aperiodic_r10,
                    &p_asn1_cqi_report_config_r10->cqi_ReportAperiodic_r10,
                    p_asn1_ctx);
        }

        p_asn1_cqi_report_config_r10->nomPDSCH_RS_EPRE_Offset = 
            p_rrc_cqi_report_config_r10->nomPDSCH_rs_epre_offset;

        if(p_rrc_cqi_report_config_r10->bitmask &
                RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_PERIODIC_R10_PRESENT)
        {
            p_asn1_cqi_report_config_r10->m.cqi_ReportPeriodic_r10Present = 1;
            if(RRC_SUCCESS != uecc_llim_build_asn1_cqi_report_config_periodic_r10(p_uecc_ue_context,  
                        &p_rrc_cqi_report_config_r10->cqi_report_periodic_r10,
                        &p_asn1_cqi_report_config_r10->cqi_ReportPeriodic_r10,
                        p_asn1_ctx))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"failed to build cqi report config"
                        "periodic r10");
                result = RRC_FAILURE;
                break;
            }
        }
        if(p_rrc_cqi_report_config_r10->bitmask &
                RRC_PHY_CQI_REPORT_CONFIG_R10_PMI_RI_REPORT_R9_PRESENT)
        {
            if(((p_rrc_cqi_report_config_r10->bitmask &
                            RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_APERIODIC_R10_PRESENT)
                        && (p_rrc_cqi_report_config_r10->
                            cqi_report_aperiodic_r10.bitmask & 
                            RRC_PHY_CQI_REPORT_APERIODIC_R10_SETUP_PRESENT)) ||
                    ((p_rrc_cqi_report_config_r10->bitmask &
                      RRC_PHY_CQI_REPORT_CONFIG_R10_REPORT_PERIODIC_R10_PRESENT)
                     && (p_rrc_cqi_report_config_r10->
                         cqi_report_periodic_r10.bitmask &
                         RRC_PHY_CQI_REPORT_PERIODIC_R10SETUP_PRESENT)))
            {
                p_asn1_cqi_report_config_r10->m.pmi_RI_Report_r9Present = 1;
                p_asn1_cqi_report_config_r10->pmi_RI_Report_r9 = 
                    p_rrc_cqi_report_config_r10->cqi_report_pmi_ri_report_r10;
            }
        }
        if(p_rrc_cqi_report_config_r10->bitmask &
                RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAME_PATTERN_CONFIG_R10_PRESENT)
        {
            p_asn1_cqi_report_config_r10->m.csi_SubframePatternConfig_r10Present = 1;

            if(RRC_SUCCESS != 
                    uecc_llim_build_asn1_cqi_report_config_csi_subframe_pattern_r10(
                        p_uecc_ue_context, 
                        &p_rrc_cqi_report_config_r10->csi_subframePattern_r10,
                        &p_asn1_cqi_report_config_r10->csi_SubframePatternConfig_r10,
                        p_asn1_ctx))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"failed to build cqi report config"
                        "csi subframe pattern");
                result = RRC_FAILURE;
                break;
            }
            /* SPR 14145 start */
            if(RELEASE == p_rrc_cqi_report_config_r10->csi_subframePattern_r10.request_type && 
                    (SETUP == p_rrc_cqi_report_config_r10->cqi_report_periodic_r10.request_type) &&
                    !(p_rrc_cqi_report_config_r10->cqi_report_periodic_r10.setup.bitmask & 
                        RRC_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_PRESENT))
            {
                p_asn1_cqi_report_config_r10->cqi_ReportPeriodic_r10.u.setup->m.csi_ConfigIndex_r10Present = 1;
                p_asn1_cqi_report_config_r10->cqi_ReportPeriodic_r10.u.setup->csi_ConfigIndex_r10.t
                    = T_CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_release;
            }
            /* SPR 14145 end */
        }
    }while(0);

    /* SPR 14145 start */
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    /* SPR 14145 end */
    return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_cqi_report_config_aperiodic_r10
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_phy_cqi_report_aperiodic_r10_t const        *p_rrc_cqi_report_config_r10
 *                  CQI_ReportAperiodic_r10                         *p_asn1_cqi_report_config_r10
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 CQI_ReportAperiodic_r10 IE based on
 *                  default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_cqi_report_config_aperiodic_r10
(
 uecc_ue_context_t                        *p_uecc_ue_context,
 rrc_phy_cqi_report_aperiodic_r10_t const *p_rrc_cqi_report_config_r10,
 CQI_ReportAperiodic_r10                  *p_asn1_cqi_report_config_r10,
 OSCTXT                                   *p_asn1_ctx
 )
{
	rrc_return_et result = RRC_SUCCESS;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_rrc_cqi_report_config_r10);
	RRC_ASSERT(PNULL != p_asn1_cqi_report_config_r10);
	do{
		if(p_rrc_cqi_report_config_r10->bitmask &
				RRC_PHY_CQI_REPORT_APERIODIC_R10_SETUP_PRESENT)
		{
			p_asn1_cqi_report_config_r10->t = T_CQI_ReportAperiodic_r10_setup;
			p_asn1_cqi_report_config_r10->u.setup = 
				rtxMemAllocType(p_asn1_ctx,CQI_ReportAperiodic_r10_setup);

			if(PNULL == 
					p_asn1_cqi_report_config_r10->u.setup)
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,"Memalloc failed.");
				result = RRC_FAILURE;
				break;
			}

			p_asn1_cqi_report_config_r10->u.setup->cqi_ReportModeAperiodic_r10 = 
				p_rrc_cqi_report_config_r10->setup.cqi_reporting_mode_aperiodic;

			if(p_rrc_cqi_report_config_r10->setup.bitmask &
					RRC_CQI_REPORT_APERIODIC_R10_SETUP_APERIODIC_CSI_TRIGGER_PRESENT)
			{
				p_asn1_cqi_report_config_r10->u.setup->m.
					aperiodicCSI_Trigger_r10Present = 1;

				p_asn1_cqi_report_config_r10->u.setup->
					aperiodicCSI_Trigger_r10.trigger1_r10.numbits = 8;

				p_asn1_cqi_report_config_r10->u.setup->
					aperiodicCSI_Trigger_r10.trigger1_r10.data[0] =
					p_rrc_cqi_report_config_r10->setup.
					aperiodic_csi_trigger.trigger1;

				p_asn1_cqi_report_config_r10->u.setup->
					aperiodicCSI_Trigger_r10.trigger2_r10.numbits = 8;

				p_asn1_cqi_report_config_r10->u.setup->
					aperiodicCSI_Trigger_r10.trigger2_r10.data[0] =
					p_rrc_cqi_report_config_r10->setup.
					aperiodic_csi_trigger.trigger2;

			}
		}
		else
		{
			p_asn1_cqi_report_config_r10->t = T_CQI_ReportAperiodic_r10_release;
		}

	}while(0);

	return result;

}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_cqi_report_config_csi_subframe_pattern_r10
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_phy_cqi_report_csi_subframePattern_r10_t const
 *                                                                  *p_csi_pattern_r10
 *                  CQI_ReportConfig_r10_csi_SubframePatternConfig_r10
 *                                                                  *p_asn1_csi_pattern_r10
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 CQI_ReportConfig_r10_csi_SubframePatternConfig_r10
 *                  IE based on default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_cqi_report_config_csi_subframe_pattern_r10
(
 uecc_ue_context_t                                  *p_uecc_ue_context,
 rrc_phy_cqi_report_csi_subframePattern_r10_t const *p_csi_pattern_r10,
 CQI_ReportConfig_r10_csi_SubframePatternConfig_r10 *p_asn1_csi_pattern_r10,
 OSCTXT                                             *p_asn1_ctx
 )
{
	rrc_return_et result = RRC_SUCCESS;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_csi_pattern_r10);
	RRC_ASSERT(PNULL != p_asn1_csi_pattern_r10);

	do{
		if((p_csi_pattern_r10->bitmask &
				RRC_PHY_CQI_REPORT_CONFIG_R10_CSI_SUBFRAMEPATTERN_CONFIG_SETUP) && 
                (SETUP == p_csi_pattern_r10->request_type))
		{
			p_asn1_csi_pattern_r10->t = 
				T_CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_setup;
			p_asn1_csi_pattern_r10->u.setup = 
				rtxMemAllocType(p_asn1_ctx,CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_setup);

			if(PNULL == 
					p_asn1_csi_pattern_r10->u.setup)
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,"Memalloc failed.");
				result = RRC_FAILURE;
				break;
			}

			result = uecc_llim_build_asn1_phy_meas_subframe_pattern(p_uecc_ue_context,
					&p_csi_pattern_r10->csi_subframe_pattern_config_r10_setup.csi_meas_subframe_set1_r10,
					&p_asn1_csi_pattern_r10->u.setup->csi_MeasSubframeSet1_r10,
					p_asn1_ctx);
			if(RRC_FAILURE == result)
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,"Failed to build subframe pattern.");
				result = RRC_FAILURE;
				break;
			}

			result = uecc_llim_build_asn1_phy_meas_subframe_pattern(p_uecc_ue_context,
					&p_csi_pattern_r10->csi_subframe_pattern_config_r10_setup.csi_meas_subframe_set2_r10,
					&p_asn1_csi_pattern_r10->u.setup->csi_MeasSubframeSet2_r10,
					p_asn1_ctx);
			if(RRC_FAILURE == result)
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,"Failed to build subframe pattern.");
				result = RRC_FAILURE;
				break;
			}
		}
        else if(RELEASE == p_csi_pattern_r10->request_type)
        {
            p_asn1_csi_pattern_r10->t = T_CQI_ReportConfig_r10_csi_SubframePatternConfig_r10_release;
        }

	}while(0);

	return result;
}

/******************************************************************************
 *   FUNCTION NAME: uecc_llim_build_asn1_physical_config_dedicated
 *   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
 *                  rrc_phy_meas_subframe_pattern_r10_t const       *p_phy_meas_subframe_pattern
 *                  MeasSubframePattern_r10                         *p_asn1_meas_subframe_pattern
 *                  OSCTXT                                          *p_asn1_ctx
 *   OUTPUT       : none
 *   DESCRIPTION  : Builds ASN1 MeasSubframePattern_r10 IE based on
 *                  default configuration or on configuration provided by RRM.
 *
 *   RETURNS      : RRC_SUCCESS/RRC_FAILURE
 *
 ******************************************************************************/
rrc_return_et uecc_llim_build_asn1_phy_meas_subframe_pattern
(
 uecc_ue_context_t                          *p_uecc_ue_context,
 rrc_phy_meas_subframe_pattern_r10_t const  *p_phy_meas_subframe_pattern,
 MeasSubframePattern_r10                    *p_asn1_meas_subframe_pattern,
 OSCTXT                                     *p_asn1_ctx
 )
{
	rrc_return_et result = RRC_SUCCESS;

	RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

	RRC_ASSERT(PNULL != p_uecc_ue_context);
	RRC_ASSERT(PNULL != p_phy_meas_subframe_pattern);
	RRC_ASSERT(PNULL != p_asn1_meas_subframe_pattern);
	do{
		if(p_phy_meas_subframe_pattern->bitmask &
				RRC_PHY_MEAS_SUBFRAME_PATTERN_FDD_R10)
		{
			p_asn1_meas_subframe_pattern->t = 
				T_MeasSubframePattern_r10_subframePatternFDD_r10;
			p_asn1_meas_subframe_pattern->u.subframePatternFDD_r10 = 
				rtxMemAllocType(p_asn1_ctx,MeasSubframePattern_r10_subframePatternFDD_r10);

			if(PNULL == 
					p_asn1_meas_subframe_pattern->u.subframePatternFDD_r10)
			{
				RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
						p_uecc_ue_context->p_gb_context->facility_name, 
						RRC_WARNING,"Memalloc failed.");
				result = RRC_FAILURE;
				break;
			}
			p_asn1_meas_subframe_pattern->u.subframePatternFDD_r10->numbits = 
				MAX_SUBFRAME_CONFIG_FDD_NUMBITS;                                    // eICIC ue reconfig changes 
			l3_memcpy_wrapper(&p_asn1_meas_subframe_pattern->u.subframePatternFDD_r10->data,
					&p_phy_meas_subframe_pattern->subframe_pattern_fdd_r10.data,
					MAX_SUBFRAME_PATTERN_FDD);
		}
		else if(p_phy_meas_subframe_pattern->bitmask &
				RRC_PHY_MEAS_SUBFRAME_PATTERN_TDD_R10)
		{
			p_asn1_meas_subframe_pattern->t = 
				T_MeasSubframePattern_r10_subframePatternTDD_r10;

			p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10 = 
				rtxMemAllocType(p_asn1_ctx,MeasSubframePattern_r10_subframePatternTDD_r10);

			if(PNULL == 
					p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10)
			{
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"Memalloc failed.");
                result = RRC_FAILURE;
                break;
            }
            if(p_phy_meas_subframe_pattern->subframe_pattern_tdd_r10.bitmask &
                    RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG1_5_R10)
            {
                p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->t = 
                    T_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig1_5_r10;
                p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->u.subframeConfig1_5_r10 = 
                    rtxMemAllocType(p_asn1_ctx,ASN1BitStr32);

                if(PNULL == 
                        p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->u.subframeConfig1_5_r10)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"Memalloc failed.");
                    result = RRC_FAILURE;
                    break;
                }
                
                /* subframeConfig1_5_r10 is a BIT STRING of 20 bits*/
                p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->
                    u.subframeConfig1_5_r10->numbits = MAX_SUBFRAME_CONFIG1_5_TDD_NUMBITS;

                l3_memcpy_wrapper(&p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->
                        u.subframeConfig1_5_r10->data,
                        &p_phy_meas_subframe_pattern->subframe_pattern_tdd_r10.
                        subframe_config1_5_r10,
                        MAX_SUBFRAME_CONFIG1_5_TDD);

            }
            else if(p_phy_meas_subframe_pattern->subframe_pattern_tdd_r10.bitmask &
                    RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG0_R10)
            {
                p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->t = 
                    T_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig0_r10;
                p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->u.subframeConfig0_r10 = 
                    rtxMemAllocType(p_asn1_ctx,MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig0_r10);

                if(PNULL == 
                        p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->u.subframeConfig0_r10)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"Memalloc failed.");
                    result = RRC_FAILURE;
                    break;
                }
                /* subframeConfig0_r10 is a BIT STRING of 70 bits*/
                p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->
                    u.subframeConfig0_r10->numbits = MAX_SUBFRAME_CONFIG0_TDD_NUMBITS;

                l3_memcpy_wrapper(&p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->
                        u.subframeConfig0_r10->data,
                        &p_phy_meas_subframe_pattern->subframe_pattern_tdd_r10.
                        subframe_config0_r10,
                        MAX_SUBFRAME_CONFIG0_TDD);
            }
            else if(p_phy_meas_subframe_pattern->subframe_pattern_tdd_r10.bitmask &
                    RRC_PHY_SUBFRAME_PATTERNTDD_SUBFRAME_CONFIG6_R10)
            {
                p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->t = 
                    T_MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig6_r10;
                p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->u.subframeConfig6_r10 = 
                    rtxMemAllocType(p_asn1_ctx,MeasSubframePattern_r10_subframePatternTDD_r10_subframeConfig6_r10);

                if(PNULL == 
                        p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->u.subframeConfig6_r10)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"Memalloc failed.");
                    result = RRC_FAILURE;
                    break;
                }
                /* subframeConfig6_r10 is a BIT STRING of 60 bits*/
                p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->
                    u.subframeConfig6_r10->numbits = MAX_SUBFRAME_CONFIG6_TDD_NUMBITS;

                l3_memcpy_wrapper(&p_asn1_meas_subframe_pattern->u.subframePatternTDD_r10->
                        u.subframeConfig6_r10->data,
                        &p_phy_meas_subframe_pattern->subframe_pattern_tdd_r10.
                        subframe_config6_r10,
                        MAX_SUBFRAME_CONFIG6_TDD);
            }
            else
            {
                break;
            }
        }
        else
        {
            break;
        }

    }while(0);

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_additional_spectrum_ca_r10
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                  rrc_additional_spectrum_emission_ca_r10_t  const
*                                                                  *p_rrc_spctrm_emison_ca
*                  PhysicalConfigDedicated_additionalSpectrumEmissionCA_r10
*                                                                  *p_asn_spctrm_emison_ca
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION  : Builds ASN1 PhysicalConfigDedicated_additionalSpectrumEmissionCA_r10 
*                  IE based on default configuration or on configuration provided by RRM.
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_additional_spectrum_ca_r10
(
    uecc_ue_context_t                                        *p_uecc_ue_context,
    rrc_additional_spectrum_emission_ca_r10_t  const         *p_rrc_spctrm_emison_ca,
    PhysicalConfigDedicated_additionalSpectrumEmissionCA_r10 *p_asn_spctrm_emison_ca,
    OSCTXT                                                   *p_asn1_ctx
)
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_rrc_spctrm_emison_ca);
    RRC_ASSERT(PNULL != p_asn_spctrm_emison_ca);

    do
    {
        if(p_rrc_spctrm_emison_ca->bitmask &
                RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PCELL_PRESENT)
        {
            p_asn_spctrm_emison_ca->t = 
                T_PhysicalConfigDedicated_additionalSpectrumEmissionCA_r10_setup;
            p_asn_spctrm_emison_ca->u.setup = 
                rtxMemAllocType(p_asn1_ctx,PhysicalConfigDedicated_additionalSpectrumEmissionCA_r10_setup);

            if(PNULL == 
                    p_asn_spctrm_emison_ca->u.setup)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"Memalloc failed.");
                result = RRC_FAILURE;
                break;
            }
            p_asn_spctrm_emison_ca->u.setup->additionalSpectrumEmissionPCell_r10 = 
                p_rrc_spctrm_emison_ca->additional_spectrum_emission_pcell;

        }

    }while(0);

    return result;

}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_physical_config_dedicated
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                  rrc_pucch_channel_selection_config_v1020_t const
*                                                                  *p_rrc_channel_selection
*                  PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10
*                                                                  *p_asn_channel_selection
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION  : Builds ASN1 PhysicalConfigDedicated IE based on
*                  default configuration or on configuration provided by RRM.
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_pucch_format_channel_selection_r10
(
    uecc_ue_context_t                                *p_uecc_ue_context,
    rrc_pucch_channel_selection_config_v1020_t const *p_rrc_channel_selection,
    PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10
                                                      *p_asn_channel_selection,
    OSCTXT                                            *p_asn1_ctx
)
{
    rrc_return_et result = RRC_SUCCESS;
    OSRTDListNode *p_node = PNULL;
    U8            count = 0;
    U8            count1 = 0;
    N1PUCCH_AN_CS_r10 *p_asn1_pucch_and_cs_r10 = PNULL;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_rrc_channel_selection);
    RRC_ASSERT(PNULL != p_asn_channel_selection);

    do{
        if(p_rrc_channel_selection->bitmask &
                RRC_N1PUCCH_AN_CS_R10_PRESENT)
        {
            p_asn_channel_selection->m.n1PUCCH_AN_CS_r10Present = 1;
            if(p_rrc_channel_selection->channel_selection_n1_pucch_an_cs_r10.
                    bitmask & RRC_N1PUCCH_AN_CS_LIST_R10_PRESENT)
            {
                p_asn_channel_selection->n1PUCCH_AN_CS_r10.t = 
                    T_PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_setup;
                p_asn_channel_selection->n1PUCCH_AN_CS_r10.u.setup = 
                    rtxMemAllocType(p_asn1_ctx,
                            PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_setup);

                if(PNULL == 
                        p_asn_channel_selection->n1PUCCH_AN_CS_r10.u.setup)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"Memalloc failed.");
                    result = RRC_FAILURE;
                    break;
                }

                asn1Init_PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10_n1PUCCH_AN_CS_r10_setup(
                            p_asn_channel_selection->n1PUCCH_AN_CS_r10.u.setup);

                for(count = 0;count < p_rrc_channel_selection->channel_selection_n1_pucch_an_cs_r10.
                        n1pucch_an_cs_list_r10.count;count++)
                {
                    rtxDListAllocNodeAndData(p_asn1_ctx,
                            N1PUCCH_AN_CS_r10,
                            &p_node,
                            &p_asn1_pucch_and_cs_r10);

                    if (PNULL == p_node)
                    {
                        RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
                        /* Not enough memory */
                        return RRC_FAILURE;
                    }
                    asn1Init_N1PUCCH_AN_CS_r10(p_asn1_pucch_and_cs_r10);

                    p_asn1_pucch_and_cs_r10->n = p_rrc_channel_selection->channel_selection_n1_pucch_an_cs_r10.
                        n1pucch_an_cs_list_r10.n1_pucch_an_cs[count].count;
                    for(count1 = 0;count1 < p_asn1_pucch_and_cs_r10->n; count1++)
                    {
                        p_asn1_pucch_and_cs_r10->elem[count1] = 
                            p_rrc_channel_selection->channel_selection_n1_pucch_an_cs_r10.
                            n1pucch_an_cs_list_r10.n1_pucch_an_cs[count].n1_pucch_an_cs[count1];
                    }
                    if (0 == rtxDListAppend(p_asn1_ctx,
                                &p_asn_channel_selection->n1PUCCH_AN_CS_r10.u.setup->n1PUCCH_AN_CS_List_r10,
                                (void*)p_asn1_pucch_and_cs_r10))
                    {
                        /* Coverity Fix 61050  */
                        return (rrc_return_et)LOG_RTERR(p_asn1_ctx, RTERR_NOMEM); 
                    }
                }
            }
        }
    }while(0);

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_antenna_info_dedicated_r10
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                  rrc_antenna_info_dedicated_r10_t const          *p_rrc_antenna_info_dedicate_r10
*                  AntennaInfoDedicated_r10                        *p_asn1_antenna_info_dedicated_r10
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION  : Builds ASN1 AntennaInfoDedicated_r10 IE based on
*                  default configuration or on configuration provided by RRM.
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_antenna_info_dedicated_r10
(
    uecc_ue_context_t                         *p_uecc_ue_context,
    rrc_antenna_info_dedicated_r10_t const    *p_rrc_antenna_info_dedicate_r10,
    AntennaInfoDedicated_r10                  *p_asn1_antenna_info_dedicated_r10,
    OSCTXT                                    *p_asn1_ctx
)
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_rrc_antenna_info_dedicate_r10);
    RRC_ASSERT(PNULL != p_asn1_antenna_info_dedicated_r10);
    do{
        p_asn1_antenna_info_dedicated_r10->transmissionMode_r10 =
            p_rrc_antenna_info_dedicate_r10->transmission_mode;

        if(p_rrc_antenna_info_dedicate_r10->bitmask &
                RRM_CODEBOOK_SUBSET_RESTRICTION_PRESENT)
        {
            p_asn1_antenna_info_dedicated_r10->m.
                codebookSubsetRestriction_r10Present = 1;

            p_asn1_antenna_info_dedicated_r10->codebookSubsetRestriction_r10.numbits =
                p_rrc_antenna_info_dedicate_r10->
                codebook_subset_restriction_r10.num_bits;

            p_asn1_antenna_info_dedicated_r10->codebookSubsetRestriction_r10.data =
                rtxMemAllocZ(p_asn1_ctx,((p_rrc_antenna_info_dedicate_r10->
                                codebook_subset_restriction_r10.num_bits + RRC_OCTET_WINDOW)/RRC_OCTET_SIZE));
            if(PNULL == 
                    p_asn1_antenna_info_dedicated_r10->codebookSubsetRestriction_r10.data)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"Memalloc failed.");
                result = RRC_FAILURE;
                break;
            }

            l3_memcpy_wrapper((void *)(p_asn1_antenna_info_dedicated_r10->codebookSubsetRestriction_r10.data),
                    (&p_rrc_antenna_info_dedicate_r10->
                     codebook_subset_restriction_r10.value),
                    (p_rrc_antenna_info_dedicate_r10->
                     codebook_subset_restriction_r10.num_bits + RRC_OCTET_WINDOW)/RRC_OCTET_SIZE);
        }
        if(p_rrc_antenna_info_dedicate_r10->ue_transmit_antenna_selection.
                bitmask & RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT)
        {
            p_asn1_antenna_info_dedicated_r10->ue_TransmitAntennaSelection.t = 
                T_AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_setup;

            p_asn1_antenna_info_dedicated_r10->ue_TransmitAntennaSelection.u.setup = 
                p_rrc_antenna_info_dedicate_r10->
                ue_transmit_antenna_selection.ue_transmit_antenna_selection_type;
        }
        else
        {
            p_asn1_antenna_info_dedicated_r10->ue_TransmitAntennaSelection.t = 
                T_AntennaInfoDedicated_r10_ue_TransmitAntennaSelection_release;

        }
        
    }while(0);

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_cqi_report_config_periodic_r10
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                  rrc_phy_cqi_report_periodic_r10_t const         *p_rrc_phy_cqi_report_periodic
*                  CQI_ReportPeriodic_r10                          *p_asn_phy_cqi_report_periodic
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION  : Builds ASN1 PhysicalConfigDedicated IE based on
*                  default configuration or on configuration provided by RRM.
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_cqi_report_config_periodic_r10
(
    uecc_ue_context_t                         *p_uecc_ue_context,
    rrc_phy_cqi_report_periodic_r10_t const   *p_rrc_phy_cqi_report_periodic, 
    CQI_ReportPeriodic_r10                    *p_asn_phy_cqi_report_periodic,
    OSCTXT                                    *p_asn1_ctx
)
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_rrc_phy_cqi_report_periodic);
    RRC_ASSERT(PNULL != p_asn_phy_cqi_report_periodic);

    do{
        if((RRC_TRUE == p_rrc_phy_cqi_report_periodic->request_type) && 
                (p_rrc_phy_cqi_report_periodic->bitmask &
                RRC_PHY_CQI_REPORT_PERIODIC_R10SETUP_PRESENT))
        {
            p_asn_phy_cqi_report_periodic->t = T_CQI_ReportPeriodic_r10_setup;
            p_asn_phy_cqi_report_periodic->u.setup =
                rtxMemAllocType(p_asn1_ctx,CQI_ReportPeriodic_r10_setup);

            if(PNULL == 
                    p_asn_phy_cqi_report_periodic->u.setup)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"Memalloc failed.");
                result = RRC_FAILURE;
                break;

            }

            asn1Init_CQI_ReportPeriodic_r10_setup(
                    p_asn_phy_cqi_report_periodic->u.setup);


            p_asn_phy_cqi_report_periodic->u.setup->cqi_PUCCH_ResourceIndex_r10 = 
                p_rrc_phy_cqi_report_periodic->setup.cqi_pucch_resource_index;

            if(p_rrc_phy_cqi_report_periodic->setup.bitmask &
                    RRC_CQI_REPORT_PERIODIC_R10_SETUP_PUCCH_REPORT_INDEX_P1_PRESENT)
            {
                p_asn_phy_cqi_report_periodic->u.setup->
                    m.cqi_PUCCH_ResourceIndexP1_r10Present = 1;
                p_asn_phy_cqi_report_periodic->u.setup->
                    cqi_PUCCH_ResourceIndexP1_r10 = 
                    p_rrc_phy_cqi_report_periodic->setup.cqi_pucch_resource_index_p1;
            }

            p_asn_phy_cqi_report_periodic->u.setup->cqi_pmi_ConfigIndex = 
                p_rrc_phy_cqi_report_periodic->setup.cqi_pmi_config_index;

            if(RRC_SUCCESS != uecc_llim_build_asn1_cqi_format_ind_periodic(
                    p_uecc_ue_context,
                    &p_rrc_phy_cqi_report_periodic->setup.
                    cqi_format_ind_periodic,
                    &p_asn_phy_cqi_report_periodic->u.
                    setup->cqi_FormatIndicatorPeriodic_r10,
                    p_asn1_ctx))
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"failed to build cqi formati ind"
                        "periodic r10");
                result = RRC_FAILURE;
                break;
            }
            if(p_rrc_phy_cqi_report_periodic->setup.bitmask &
                    RRC_CQI_REPORT_PERIODIC_R10_SETUP_RI_CONFIG_INDEX_PRESENT)
            {
                p_asn_phy_cqi_report_periodic->u.
                    setup->m.ri_ConfigIndexPresent = 1;
                p_asn_phy_cqi_report_periodic->u.
                    setup->ri_ConfigIndex = 
                    p_rrc_phy_cqi_report_periodic->setup.ri_config_index;
            }
            p_asn_phy_cqi_report_periodic->u.
                setup->simultaneousAckNackAndCQI = 
                p_rrc_phy_cqi_report_periodic->setup.simultaneous_ack_nack_cqi;
            if(p_rrc_phy_cqi_report_periodic->setup.bitmask &
                    RRC_CQI_REPORT_PERIODIC_R10_SETUP_CQI_MASK_PRESENT)
            {
                p_asn_phy_cqi_report_periodic->u.
                    setup->m.cqi_Mask_r9Present = 1;
                p_asn_phy_cqi_report_periodic->u.
                    setup->cqi_Mask_r9 = p_rrc_phy_cqi_report_periodic->setup.cqi_mask_r9;
            }
            if(p_rrc_phy_cqi_report_periodic->setup.bitmask &
                    RRC_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_PRESENT)
            {
                p_asn_phy_cqi_report_periodic->u.
                    setup->m.csi_ConfigIndex_r10Present = 1;
                if(p_rrc_phy_cqi_report_periodic->setup.csi_config_index.bitmask &
                        RRC_PHY_CQI_REPORT_PERIODIC_R10_SETUP_CSI_CONFIG_INDEX_SETUP_PRESENT)
                {
                    p_asn_phy_cqi_report_periodic->u.
                        setup->csi_ConfigIndex_r10.t = T_CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_setup;
                    p_asn_phy_cqi_report_periodic->u.
                        setup->csi_ConfigIndex_r10.u.setup = 
                        rtxMemAllocType(p_asn1_ctx,CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_setup);

                    if(PNULL == 
                            p_asn_phy_cqi_report_periodic->u.
                            setup->csi_ConfigIndex_r10.u.setup)
                    {
                        RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                                p_uecc_ue_context->p_gb_context->facility_name, 
                                RRC_WARNING,"Memalloc failed.");
                        result = RRC_FAILURE;
                        break;
                    }
                    asn1Init_CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_setup(
                            p_asn_phy_cqi_report_periodic->u.
                            setup->csi_ConfigIndex_r10.u.setup);

                    p_asn_phy_cqi_report_periodic->u.
                        setup->csi_ConfigIndex_r10.u.setup->cqi_pmi_ConfigIndex2_r10 = 
                        p_rrc_phy_cqi_report_periodic->setup.csi_config_index.setup.cqi_pmi_config_index2;

                    if(p_rrc_phy_cqi_report_periodic->setup.csi_config_index.setup.bitmask & 
                            RRC_PHY_CQI_PERIODIC_R10_SETUP_CSI_RI_CONFIG_INDEX2_PRESENT)
                    {
                        p_asn_phy_cqi_report_periodic->u.
                            setup->csi_ConfigIndex_r10.u.setup->m.ri_ConfigIndex2_r10Present = 1;
                        p_asn_phy_cqi_report_periodic->u.
                            setup->csi_ConfigIndex_r10.u.setup->ri_ConfigIndex2_r10 = 
                            p_rrc_phy_cqi_report_periodic->setup.csi_config_index.setup.ri_config_index2;
                    }
                }
        /*CA_TC017_FIX_START*/
                else
                {
                    p_asn_phy_cqi_report_periodic->u.
                        setup->csi_ConfigIndex_r10.t = 
                        T_CQI_ReportPeriodic_r10_setup_csi_ConfigIndex_r10_release;
                }
        /*CA_TC017_FIX_START*/
            }
        }
        /*CA_TC013_FIX_START*/
        else if(RRC_FALSE == p_rrc_phy_cqi_report_periodic->request_type)
        {
            p_asn_phy_cqi_report_periodic->t = T_CQI_ReportPeriodic_r10_release;
        }
        else
        {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"Invalid request received");
                result = RRC_FAILURE;
                break;

        }
        /*CA_TC013_FIX_END*/
    }while(0);

    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_phy_ca_config_dedicated
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                   rrc_phy_physical_config_dedicated_t const      *p_phy
*                  PhysicalConfigDedicated                         *p_asn1_phy
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION  : Builds ASN1 PhysicalConfigDedicated IE based on
*                  default configuration or on configuration provided by RRM.
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_phy_ca_config_dedicated(
    uecc_ue_context_t                               *p_uecc_ue_context,
    rrc_phy_physical_config_dedicated_t const       *p_phy,
    PhysicalConfigDedicated                         *p_asn1_phy,
    OSCTXT                                          *p_asn1_ctx
)
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_phy);
    RRC_ASSERT(PNULL != p_asn1_phy);

    do{
        if (p_phy->bitmask & RRC_PHY_ANTENNA_INFO_R10_PRESENT)
        {
            p_asn1_phy->m.antennaInfo_r10Present = 1;

            if(p_phy->antenna_info_r10.bitmask &
                    RRC_ANTENNA_INFO_R10_EXPLICIT_VALUE_PRESENT)
            {
                p_asn1_phy->antennaInfo_r10.t = 
                    T_PhysicalConfigDedicated_antennaInfo_r10_explicitValue_r10;
                p_asn1_phy->antennaInfo_r10.u.explicitValue_r10 =
                    rtxMemAllocType(p_asn1_ctx,AntennaInfoDedicated_r10);

                if(PNULL == 
                        p_asn1_phy->antennaInfo_r10.u.explicitValue_r10)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"Memalloc failed.");
                    result = RRC_FAILURE;
                    break;
                }

                asn1Init_AntennaInfoDedicated_r10(p_asn1_phy->antennaInfo_r10.u.explicitValue_r10);

                if(RRC_SUCCESS != uecc_llim_build_asn1_antenna_info_dedicated_r10(p_uecc_ue_context,
                        &p_phy->antenna_info_r10.explicit_value,
                        p_asn1_phy->antennaInfo_r10.u.explicitValue_r10,
                        p_asn1_ctx))
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"failed to build asn anteena"
                            "info dedicated r10");
                    result = RRC_FAILURE;
                    break;
                }
            }
        }
        if (p_phy->bitmask & RRC_PHY_ANTENNA_INFO_UL_R10_PRESENT)
        {
            p_asn1_phy->m.antennaInfoUL_r10Present = 1;
            if(RRC_SUCCESS != uecc_llim_build_asn1_antenna_info_ul_r10(p_uecc_ue_context,
                    &p_phy->antenna_info_ul_r10,
                    &p_asn1_phy->antennaInfoUL_r10))
                /*SPR 17777 +-*/
            {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"failed to build asn anteena"
                            "info ul r10");
                    result = RRC_FAILURE;
                    break;
            }
        }
        if (p_phy->bitmask & RRC_PHY_CQI_REPORT_CONFIG_R10_PRESENT)
        {
            p_asn1_phy->m.cqi_ReportConfig_r10Present = 1;
            if(RRC_SUCCESS != uecc_llim_build_asn1_cqi_report_config_r10(p_uecc_ue_context,
                    &p_phy->cqi_report_config_r10,
                    &p_asn1_phy->cqi_ReportConfig_r10,
                    p_asn1_ctx))
            {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"failed to build asn cqi"
                            "report config r10");
                    result = RRC_FAILURE;
                    break;
            }
        }
        if (p_phy->bitmask & RRC_PHY_CONFIG_DEDICATED_EXTENDED_PRESENT)
        {
            if(p_phy->phy_physical_config_dedicated_extended.bitmask &
                    RRC_PHY_ADDITIONAL_SPECTRUM_EMISSION_CA_R10_PRESENT)
            {
                p_asn1_phy->m._v4ExtPresent = 1;

                p_asn1_phy->m.additionalSpectrumEmissionCA_r10Present = 1;
                if(RRC_SUCCESS != uecc_llim_build_asn1_additional_spectrum_ca_r10(p_uecc_ue_context,
                        &p_phy->phy_physical_config_dedicated_extended.additional_spectrum_emission_ca,
                        &p_asn1_phy->additionalSpectrumEmissionCA_r10,
                        p_asn1_ctx))
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"failed to build asn additional"
                            "spectrum emission r10");
                    result = RRC_FAILURE;
                    break;
                }
            }
            if(p_phy->phy_physical_config_dedicated_extended.bitmask &
                    RRC_PHY_PUCCH_CHANNEL_SELECTION_CONFIG_R10_PRESENT)
            {
                p_asn1_phy->m.pucch_ConfigDedicated_v1020Present = 1;
                p_asn1_phy->pucch_ConfigDedicated_v1020.m.pucch_Format_r10Present = 1;
                p_asn1_phy->pucch_ConfigDedicated_v1020.pucch_Format_r10.t = 
                    T_PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10;
                p_asn1_phy->pucch_ConfigDedicated_v1020.pucch_Format_r10.u.channelSelection_r10 =
                    rtxMemAllocType(p_asn1_ctx,PUCCH_ConfigDedicated_v1020_pucch_Format_r10_channelSelection_r10);

                if(PNULL == 
                        p_asn1_phy->pucch_ConfigDedicated_v1020.pucch_Format_r10.u.channelSelection_r10)
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"Memalloc failed.");
                    result = RRC_FAILURE;
                    break;
                }
                if(RRC_SUCCESS != uecc_llim_build_asn1_pucch_format_channel_selection_r10(p_uecc_ue_context,
                        &p_phy->phy_physical_config_dedicated_extended.
                        pucch_channel_selection_config_v1020,
                        p_asn1_phy->pucch_ConfigDedicated_v1020.
                        pucch_Format_r10.u.channelSelection_r10,
                        p_asn1_ctx))
                {
                    RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                            p_uecc_ue_context->p_gb_context->facility_name, 
                            RRC_WARNING,"failed to build pucch channel"
                            "selection config r10");
                    result = RRC_FAILURE;
                    break;
                }
            }
        }
    }while(0);

    if ((p_asn1_phy->m.antennaInfo_r10Present == 1) || 
        (p_asn1_phy->m.antennaInfoUL_r10Present == 1) ||
        (p_asn1_phy->m.cqi_ReportConfig_r10Present == 1) ||
        (p_asn1_phy->m.pucch_ConfigDedicated_v1020Present == 1))
    {
        p_asn1_phy->m._v3ExtPresent = 1;
    }
    RRC_UECC_UT_TRACE_EXIT(p_uecc_ue_context->p_gb_context);
    return result;
}

/******************************************************************************
*   FUNCTION NAME: uecc_llim_build_asn1_cqi_format_ind_periodic
*   INPUT        : uecc_ue_context_t                               *p_uecc_ue_context
*                  rrc_phy_cqi_format_ind_periodic_r10_t const     *p_rrc_cqi_format_ind_periodic
*                  CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10
*                                                                  *p_asn_cqi_format_ind_periodic
*                  OSCTXT                                          *p_asn1_ctx
*   OUTPUT       : none
*   DESCRIPTION  : Builds ASN1 CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10
*                  IE based on default configuration or on configuration provided by RRM.
*
*   RETURNS      : RRC_SUCCESS/RRC_FAILURE
*
******************************************************************************/
rrc_return_et uecc_llim_build_asn1_cqi_format_ind_periodic
(
    uecc_ue_context_t                            *p_uecc_ue_context,
    rrc_phy_cqi_format_ind_periodic_r10_t const  *p_rrc_cqi_format_ind_periodic,
    CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10
                                                 *p_asn_cqi_format_ind_periodic,
    OSCTXT                                       *p_asn1_ctx
)
{
    rrc_return_et result = RRC_SUCCESS;

    RRC_UECC_UT_TRACE_ENTER(p_uecc_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_uecc_ue_context);
    RRC_ASSERT(PNULL != p_rrc_cqi_format_ind_periodic);
    RRC_ASSERT(PNULL != p_asn_cqi_format_ind_periodic);

    do
    {
        if(p_rrc_cqi_format_ind_periodic->bitmask
                & RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_PRESENT)
        {
            p_asn_cqi_format_ind_periodic->t  
                = T_CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10;

            p_asn_cqi_format_ind_periodic->u.widebandCQI_r10 = 
                rtxMemAllocType(p_asn1_ctx,CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10);

            if(PNULL == 
                    p_asn_cqi_format_ind_periodic->u.widebandCQI_r10)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"Memalloc failed.");
                result = RRC_FAILURE;
                break;
            }
            asn1Init_CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_widebandCQI_r10(
                    p_asn_cqi_format_ind_periodic->u.widebandCQI_r10);
            if(p_rrc_cqi_format_ind_periodic->
                    cqi_format_ind_wideband.bitmask &
                    RRC_PHY_CQI_FORMAT_IND_PERIODIC_WIDEBAND_CQI_REPORT_MODE_PRESENT)
            {
                p_asn_cqi_format_ind_periodic->u.widebandCQI_r10->
                    m.csi_ReportMode_r10Present = 1;

                p_asn_cqi_format_ind_periodic->u.widebandCQI_r10->csi_ReportMode_r10 = 
                    p_rrc_cqi_format_ind_periodic->
                    cqi_format_ind_wideband.csi_report_mode;
            }

        }
        else if(p_rrc_cqi_format_ind_periodic->bitmask
                & RRC_PHY_CQI_FORMAT_IND_PERIODIC_SUBBAND_CQI_PRESENT)
        {
            p_asn_cqi_format_ind_periodic->t  
                = T_CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10;

            p_asn_cqi_format_ind_periodic->u.subbandCQI_r10 = 
                rtxMemAllocType(p_asn1_ctx,CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10);

            if(PNULL == 
                    p_asn_cqi_format_ind_periodic->u.subbandCQI_r10)
            {
                RRC_UECC_TRACE(p_uecc_ue_context->p_gb_context->context_index,
                        p_uecc_ue_context->p_gb_context->facility_name, 
                        RRC_WARNING,"Memalloc failed.");
                result = RRC_FAILURE;
                break;
            }

            memset_wrapper( p_asn_cqi_format_ind_periodic->
                    u.subbandCQI_r10 , RRC_NULL,
                    sizeof (CQI_ReportPeriodic_r10_setup_cqi_FormatIndicatorPeriodic_r10_subbandCQI_r10));

            p_asn_cqi_format_ind_periodic->u.subbandCQI_r10->k = 
                p_rrc_cqi_format_ind_periodic->cqi_format_ind_subband.k;

            p_asn_cqi_format_ind_periodic->u.subbandCQI_r10->periodicityFactor_r10 = 
                p_rrc_cqi_format_ind_periodic->cqi_format_ind_subband.periodicy_factor;

        }
    }while(0);

    return result;
}




#if  defined(ENDC_ENABLED)
/******************************************************************************
 * FUNCTION NAME : uecc_llim_build_and_send_pdcp_lwa_traffic_state_update_ind 
 *
 * INPUT         : uecc_ue_context_t*  p_ue_context
 *                 lwa_traffic_ind_et  flag
 *
 * OUTPUT        : None
 *
 * DESCRIPTION   : This function build and send indication to PDCP to 
 *                 START/STOP LWA Traffic.
 *
 * RETURNS       : None
 ******************************************************************************/
void uecc_llim_build_and_send_pdcp_lwa_traffic_state_update_ind(
    uecc_ue_context_t*  p_ue_context,
    lwa_traffic_ind_et  flag)
{
    #ifdef ENDC_ENABLED
    /*OPTION3X Changes Start*/
    rrc_drb_to_modify_list_t    *p_drb_to_modify_list = PNULL;
    /*OPTION3X Changes Stop*/
    #else
    rrm_erab_to_be_reconfigured_item_list_t* 
                                       p_erb_to_reconfig_list       = PNULL;
    erab_reconfigure_status_list_t*    p_erab_reconfig_status_list  = PNULL;
    uecc_drb_ctx_t*                    p_erab_ctx                   = PNULL;    
    U8                                 erb_id                       = RRC_NULL;
    #endif
    rrc_pdcp_update_lwa_traffic_ind_t  pdcp_update_lwa_traffic_ind;
    U8                                 drb_index                    = RRC_NULL;
    U8                                 modified_drb_count           = RRC_NULL;

    RRC_UECC_UT_TRACE_ENTER(p_ue_context->p_gb_context);

    RRC_ASSERT(PNULL != p_ue_context);

    #ifndef ENDC_ENABLED
    /* Fetch pointer to ERAB to be reconfigured list */
    p_erb_to_reconfig_list = &(p_ue_context->p_curr_proc_data->u.
                                rrm_erab_reconfig_request_data.
                                erab_to_be_reconfigured_item_list);

    /* Fetch pointer to ERAB to be reconfigured status list */
    p_erab_reconfig_status_list = &(p_ue_context->p_curr_proc_data->u.
                                    rrm_erab_reconfig_request_data.
                                    erab_reconfig_status_list);

    /* Fetch the count of DRBs which are modified */
    modified_drb_count = p_erb_to_reconfig_list->count;
    #else
    /* Fetch pointer to ERAB to be reconfigured list */
    p_drb_to_modify_list = &(p_ue_context->p_curr_proc_data->u.
                                rrm_dc_bearer_change_request_data.
                                drb_to_modify_list);
    /* Fetch the count of DRBs which are modified */
    modified_drb_count = p_drb_to_modify_list->drb_count;
    #endif

    /* Populate traffic state i.e. whether to START/STOP the traffic */
    pdcp_update_lwa_traffic_ind.traffic_ind = (U16)flag;

    /* Populate UE index */
    pdcp_update_lwa_traffic_ind.ue_index    = p_ue_context->ue_index;

    /* Traverse through the list of DRBs and for each DRB populate LcId
     * in rrc_pdcp_update_lwa_traffic_ind_t. */
    for (drb_index = 0; drb_index < modified_drb_count; drb_index++)
    {
        #ifndef ENDC_ENABLED
        /* Fetch ERAB Id for this DRB */
        erb_id = p_erb_to_reconfig_list->erab_to_be_reconfigured[drb_index].erab_id;

        /* Fetch ERAB context stored in UE context */
        p_erab_ctx =  p_ue_context->p_e_rab_list[erb_id];

        /* Populate LcId from ERAB context */
        pdcp_update_lwa_traffic_ind.lc_id
                   = p_erab_ctx->drb_config.logical_channel_identity;
        #else
		/*Logical_Error*/
        /* Populate LcId from ERAB context */
        if((RRC_TRUE == p_drb_to_modify_list->drb_config[drb_index].erab_success) &&
        	(RRC_FALSE == p_drb_to_modify_list->drb_config[drb_index].erab_failed))
        {
        pdcp_update_lwa_traffic_ind.lc_id
                   = p_drb_to_modify_list->drb_config[drb_index].old_lc_id;
        }
        else
        {
        	continue;
        }
		/*Logical_Error*/
        #endif

    /* Send PDCP_STOP_LWA_TRAFFIC_IND */
        if (RRC_SUCCESS == rrc_pdcp_il_send_rrc_pdcp_update_lwa_traffic_ind(
            &pdcp_update_lwa_traffic_ind,
            RRC_LLIM_MODULE_ID,
            RRC_GET_LL_MODULE_ID(RRC_PDCP_1_MODULE_ID,p_ue_context->cell_index),
            uecc_ue_ctx_get_op_transction_id(p_ue_context),
                            p_ue_context->cell_index))
        {
            #ifndef ENDC_ENABLED
            /* Update the status in status list */
            p_erab_reconfig_status_list->erab_reconfig_status[erb_id].response
                                    = RRC_SUCCESS;
            #endif
        }
        else
        {
            RRC_UECC_TRACE(p_ue_context->p_gb_context->context_index,
                    p_ue_context->p_gb_context->facility_name, RRC_ERROR,
                    "Sending of PDCP_STOP_LWA_TRAFFIC_IND returned FAILURE");
        }
    }
}
#endif
