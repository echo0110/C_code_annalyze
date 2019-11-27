/******************************************************************************
*
*   FILE NAME:
*       uecc_defaults.c
*
*   DESCRIPTION:
*       This is the defaults values for RRC protocol IEs.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   19 Aug 2009     GrygoriyS   ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "rrc_phy_intf.h"
#include "uecc_defaults.h"
#include "uecc_common.h"
#include "rrc_asn_enc_dec.h"
/* SPR 14984 Fix Start */
#include "uecc_global_ctx.h"
/* SPR 14984 Fix Stop */


/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Global variables
 ****************************************************************************/

/* 3GPP TS 36331-850 - 9.2.1.1   SRB configurations - SRB1 */
/* RRC-RRM interface -- rrm_srb_config_t IE*/
const rrm_srb_config_t srb1_config_default =
{

    .bitmask = RRM_SRB_CONFIG_AM_CONFIG_PRESENT
               |RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT,

    .srb_am_config =
    {
        .bitmask = RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT,
        .am_config_explicit =
        {
            .ul_am_rlc =
            {
                .t_poll_retransmit = RRC_POLL_RETRAS_MS_45,
                .poll_pdu = RRC_POLL_PDU_INFINITY,
                .poll_byte = RRC_POLL_BYTE_KB_INFINITY,
                .max_retx_threshold = RRC_MAX_RETRANS_THRESH_4,
            },
            .dl_am_rlc =
            {
                .t_reordering = RRC_T_REORDER_MS_35,
                .t_status_prohibit = RRC_T_STATUS_PROHB_MS_0,
            },
        },
    },

    /* Logical channel configuration */
    .logical_channel_config =
    {
        .bitmask = RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT
                   |RRM_UL_SPECIFIC_PARAMETERS_LC_SR_MASK_R9_PRESENT,

        .ul_specific_parameters_explicit =
        {
            .bitmask = RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT,
            .priority = 1,
            .prioritized_bit_rate = RRC_BIT_RATE_INFINITY,
            .bucket_size_duration = RRC_BUCKET_SIZE_MS_50,  /* N/A */
            .logical_channel_group = 0,
        },
    },

    /* eNB MAC LC configuration - Not used */
    .mac_lc_config_enb =
    {
        .bitmask = 0,
        .ul_lc_g_id = 0,
        .dl_lch_priority = 0
    },
};

/* 3GPP TS 36331-850 - 9.2.1.2   SRB configurations - SRB2 */
/* RRC-RRM interface -- rrm_srb_config_t IE*/
const rrm_srb_config_t srb2_config_default =
{
    .bitmask = RRM_SRB_CONFIG_AM_CONFIG_PRESENT
               |RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT,

    .srb_am_config =
    {
        .bitmask = RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT,
        .am_config_explicit =
        {
            .ul_am_rlc =
            {
                .t_poll_retransmit = RRC_POLL_RETRAS_MS_45,
                .poll_pdu = RRC_POLL_PDU_INFINITY,
                .poll_byte = RRC_POLL_BYTE_KB_INFINITY,
                .max_retx_threshold = RRC_MAX_RETRANS_THRESH_4,
            },
            .dl_am_rlc =
            {
                .t_reordering = RRC_T_REORDER_MS_35,
                .t_status_prohibit = RRC_T_STATUS_PROHB_MS_0,
            },
        },
    },

    /* Logical channel configuration */
    .logical_channel_config =
    {
        .bitmask = RRM_UL_SPECIFIC_PARAMETERS_EXPLICIT_PRESENT
                   |RRM_UL_SPECIFIC_PARAMETERS_LC_SR_MASK_R9_PRESENT,

        .ul_specific_parameters_explicit =
        {
            .bitmask = RRC_UL_SPECIFIC_PARAMETERS_LOGICAL_CH_GROUP_PRESENT,
            .priority = 3,
            .prioritized_bit_rate = RRC_BIT_RATE_INFINITY,
            .bucket_size_duration = RRC_BUCKET_SIZE_MS_50,  /* N/A */
            .logical_channel_group = 0,
        },
    },

    /* eNB MAC LC configuration - Not used */
    .mac_lc_config_enb =
    {
        .bitmask = 0,
        .ul_lc_g_id = 0,
        .dl_lch_priority = 0
    },
};

/* 3GPP TS 36331-850 - 9.2.2 Default MAC main configuration */
/* RRC-RRM interface -- rrm_mac_config_t IE*/
const rrm_mac_config_t mac_config_default =
{
    .bitmask = RRM_MAC_CONFIG_MAC_MAIN_CONFIG_PRESENT,

    .mac_main_config =
    {
        .bitmask = RRC_MAC_MAIN_CONFIG_UL_SCH_CONFIG_PRESENT
                   |RRC_MAC_MAIN_CONFIG_DRX_CONFIG_PRESENT
                   |RRC_MAC_MAIN_CONFIG_PHR_CONFIG_PRESENT,

        .ul_sch_config =
        {
            .bitmask = RRC_UL_SCH_CONFIG_MAX_HARQ_TX_PRESENT
                       |RRC_UL_SCH_CONFIG_PERIODIC_BSR_TIMER_PRESENT,
            /* maxHARQ-tx */
            .max_harq_tx = RRC_MAX_HARQ_TX_N_5,
            /* periodicBSR-Timer - infinity */
            .periodic_bsr_timer = RRC_PERIODIC_BSR_TIMER_INFINITY,
            /* retxBSR-Timer */
            .retx_bsr_timer = RRC_RETX_BSR_TIMER_SF_2560,
            /* ttiBundling */
            .tti_bundling = RRC_FALSE,
        },

        /* drx-Config - release */
        .drx_config =
        {
            .bitmask = 0,
        },

        /* field is not from TS and it is not used in LL configuration */
        .time_alignment_timer_dedicated = RRM_RRC_TIME_ALLIGN_TIMER_SF_750,

        /* phr-Config - release */
        .phr_config =
        {
            .bitmask = 0,
        },
    },

    .mac_main_config_extensions = {0,0,0,0,0,0,0,0,0,0,0,0,0,0,0} /* Default values not supported */ //Deepika
};

/* 3GPP TS 36331-850 - 9.2.3 Default semi-persistent scheduling configuration */
/* RRC-RRM interface -- rrc_sps_config_t IE*/
const rrc_sps_config_t sps_config_default =
{
    .bitmask = RRC_SPS_CONFIG_SPS_CONFIG_DL_PRESENT
               |RRC_SPS_CONFIG_SPS_CONFIG_UL_PRESENT,

    /*semi_presist_sched_c_rnti - not used */
    .semi_presist_sched_c_rnti = {0},

    /* sps-ConfigDL - release */
    .sps_config_dl =
    {
        .bitmask = 0,
    },

    /* sps-ConfigUL - release */
    .sps_config_ul =
    {
        .bitmask = 0,
    },
};

/* 3GPP TS 36331-850 - 9.2.4   Default physical channel configuration */
/* RRC-RRM interface -- rrc_phy_physical_config_dedicated_t IE*/
rrc_phy_physical_config_dedicated_t physical_config_dedicated_default =
{

    .bitmask = RRC_PHY_PDSCH_CONFIGURATION_DEDICATED_PRESENT
               |RRC_PHY_PUCCH_CONFIGURATION_DEDICATED_PRESENT
               |RRC_PHY_PUSCH_CONFIGURATION_DEDICATED_PRESENT
               |RRC_PHY_UPLINK_POWER_CONTROL_DEDICATED_PRESENT
               |RRC_PHY_TPC_PDCCH_CONFIG_PUCCH_PRESENT
               |RRC_PHY_TPC_PDCCH_CONFIG_PUSCH_PRESENT
               |RRC_PHY_CQI_REPORTING_PRESENT
               |RRC_PHY_SOUNDING_RS_UL_CONFIG_DEDICATED_PRESENT
               |RRC_PHY_ANTENNA_INFORMATION_PRESENT
               |RRC_PHY_SCHEDULING_REQUEST_CONFIG_PRESENT
               |RRC_PHY_CQI_REPORT_CONFIG_V920_PRESENT   
               |RRC_PHY_ANTENNA_INFO_V920_PRESENT,       

    /* PDSCH-ConfigDedicated */
    .pdsch_configuration_dedicated =
    {
        .p_a = PDSCH_CONFIGURATION_DEDICATED_P_A_DB0,
    },

    /* PUCCH-ConfigDedicated */
    .pucch_configuration_dedicated =
    {
        .bitmask = RRC_PHY_TDD_ACK_NACK_FEEDBACK_MODE_PRESENT,
        .tdd_ack_nack_feedback_mode = TDD_ACK_NACK_FEEDBACK_MODE_BUNDLING,
        /* ackNackRepetition - release */
        .ack_nack_repetition_param = {0,0},
    },

    /* PUSCH-ConfigDedicated */
    .pusch_configuration_dedicated =
    {
        .beta_offset_ack_index = 10,
        .beta_offset_ri_index = 12,
        .beta_offset_cqi_index = 15,
    },

    /* UplinkPowerControlDedicated */
    .uplink_power_control_dedicated =
    {
        .p0_ue_pusch = 0,
        .delta_mcs_enabled = DELTA_MCS_EN0,
        .accumulation_enabled = RRC_TRUE,
        .p0_ue_pucch = 0,
        .p_srs_offset = 7,
        .filter_coefficient = RRC_FC4,
    },

    /* tpc-pdcch-ConfigPUCCH - release */
    .tpc_pdcch_config_pucch =
    {
        .bitmask = 0,
    },

    /* tpc-pdcch-ConfigPUSCH - release */
    .tpc_pdcch_config_pusch =
    {
        .bitmask = 0,
    },

    /* CQI-ReportConfig - release */
    .cqi_reporting =
    {
        .bitmask = RRC_PHY_CQI_REPORTING_PERIODIC_PRESENT,
        /* Default value isn't defined in TS,
         * but this filed is mandatory */
        /* see 3GPP TS 36331-850 6.3.2 - CQI-ReportConfig */
        .nom_pdsch_rs_epre_offset = 0,

        /* CQI-ReportPeriodic - release */
        .cqi_reporting_periodic =
        {
            .bitmask = 0,
        },
    },

    /* SoundingRS-UL-ConfigDedicated - release */
    .sounding_rs_ul_config_dedicated =
    {
        .bitmask = 0,
    },

    /* AntennaInfoDedicated */
    .antenna_information =
    {
        .bitmask = RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT,
        .antenna_information_explicit_value =
        {
            .bitmask = 0,
            .transmission_mode = TRANSMISSION_MODE_TM1,
            .codebook_subset_restriction = /* N/A, bitmask not set */
            {
                .type = 0,
                .value = {0,0,0,0,0,0,0,0},
            },
            .ue_transmit_antenna_selection = /* release */
            {
                .bitmask = 0,
                .ue_transmit_antenna_selection_type = 0, /*bitmask not set*/
            },
        },
    },

    /* SchedulingRequestConfig - release */
    .scheduling_request_config =
    {
        .bitmask = 0,
    },
    /* cr_316 and 247 CQI-ReportConfig - release */
    .cqi_report_config_v920 =
    {
        .bitmask = RRC_PHY_CQI_MASK_R9_PRESENT
                   |RRC_PHY_PMI_RI_REPORT_R9_PRESENT,
        .cqi_mask_r9 = 0,
        .pmi_ri_report_r9 = 0,
    },

    /* AntennaInfoV920 */
    .antenna_info_v920 =
    {
        .bitmask = RRC_PHY_CODEBOOK_SUBSET_RESTRICTION_V920_PRESENT,
        .codebook_subset_restriction_v920 =
        {
            .type = 0,
            .value = {0,0,0,0},
        },
    },
};
/* SPR 14984 Fix Start */
/****************************************************************************
 * Function Name  : uecc_validate_phsical_config_dedicated
 * Inputs         : rrc_phy_physical_config_dedicated_t 
 *                  - pointer to Physical config dedicated
 * Outputs        : None                                                               
 * Returns        : Success/Failure                                                                    
 * Description    : Validate Antenna Info for default value
 *           
 *****************************************************************************/
rrc_return_et uecc_validate_antenna_info(
         rrc_phy_antenna_information_dedicated_t *p_antenna_info,
         uecc_ue_context_t      *p_uecc_ue_context
        )
{
    U8            num_of_antennas = 0;
    rrc_bool_et   result = RRC_FALSE; 

    if ((PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context) &&
          (PNULL != p_uecc_ue_context->p_gb_context->p_p_csc_context
           [p_uecc_ue_context->cell_index]) && (PNULL != p_uecc_ue_context->
             p_gb_context->p_p_csc_context[p_uecc_ue_context->cell_index]->p_csc_init_setup_ind))
    {
        num_of_antennas = p_uecc_ue_context->p_gb_context->p_p_csc_context[p_uecc_ue_context->
                              cell_index]->p_csc_init_setup_ind->num_of_antennas;

        if (((RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT & 
            p_antenna_info->ue_transmit_antenna_selection.bitmask) &&
            (0 == p_antenna_info->ue_transmit_antenna_selection.
            ue_transmit_antenna_selection_type)) || 
          (!(RRC_PHY_UE_TRANSMIT_ANTENNA_SELECTION_TYPE_PRESENT &
             p_antenna_info->ue_transmit_antenna_selection.bitmask)))
        {
            result = RRC_TRUE;
        }

        if((result == RRC_TRUE) && 
            (TRANSMISSION_MODE_TM1 == p_antenna_info->transmission_mode) &&
            (1 == num_of_antennas))
        {
            return RRC_SUCCESS; 
        } 
        else if ((result == RRC_TRUE) && 
            (TRANSMISSION_MODE_TM2 == p_antenna_info->transmission_mode) &&
            (1 < num_of_antennas))
        {
            return RRC_SUCCESS; 
        } 
        else
        {
            return RRC_FAILURE;
        }
    }
    else
    {
        return RRC_FAILURE;
    }


}
/* SPR 14984 Fix Stop */


/****************************************************************************
 * Function Name  : uecc_defaults_apply_srb_config
 * Inputs         : rrm_srb_config_t *p_srb_config - pointer to destination
 *                                                   SRB configuration
 *                  rrm_srb_config_t *p_srb_config_default - pointer to default
 *                                                 SRB configuration
 * Outputs        : None
 * Returns        : None
 * Description    : Applies default SRB configuration.
 *                  update UE context.
 ****************************************************************************/
void uecc_defaults_apply_srb_config(
        rrm_srb_config_t *p_srb_config,
/* SPR 14984 Fix Start */
        rrm_srb_config_t const *p_srb_config_default,
        uecc_ue_context_t      *p_uecc_ue_context,
        U8                     srb_identity
/* SPR 14984 Fix Stop */
)
{
   if (!(RRM_SRB_CONFIG_AM_CONFIG_PRESENT & p_srb_config->bitmask))
   {
       p_srb_config->bitmask |= RRM_SRB_CONFIG_AM_CONFIG_PRESENT;
       p_srb_config->srb_am_config = p_srb_config_default->srb_am_config;
       /* SPR 14984 Fix Start */
       if (RRC_SRB1_LC_ID == srb_identity)
       {
           p_uecc_ue_context->m.default_srb1_rlc_cfg_present = 1;
       }
       else if (RRC_SRB2_LC_ID == srb_identity)
       {
           p_uecc_ue_context->m.default_srb2_rlc_cfg_present = 1;
       }
       /* SPR 14984 Fix Stop */
   }

       /* SPR 14984 Fix Start*/ 
    if ((!(RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT & p_srb_config->bitmask)) ||
          (p_uecc_ue_context->m.default_srb1_rlc_cfg_present) ||
          (p_uecc_ue_context->m.default_srb2_rlc_cfg_present))
   {
       p_srb_config->bitmask |= RRM_SRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT;
       p_srb_config->logical_channel_config =
            p_srb_config_default->logical_channel_config;
       if ((RRC_SRB1_LC_ID == srb_identity) && 
               (p_uecc_ue_context->m.default_srb1_rlc_cfg_present))
       {
           p_uecc_ue_context->m.default_srb1_lc_cfg_present = 1;
       }
       else if ((RRC_SRB2_LC_ID == srb_identity) && 
                  (p_uecc_ue_context->m.default_srb2_rlc_cfg_present))
       {
           p_uecc_ue_context->m.default_srb2_lc_cfg_present = 1;
       }
        /* SPR 14984 Fix Stop */
   }
}


