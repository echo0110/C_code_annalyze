/*********************************************************************
 *
 *  FILE NAME   : sib_decoder.c
 *
 *  DESCRIPTION : File contains the sib decoder implementaion.
 *
 *  REVISION HISTORY :

 *
 *  DATE              Name           Reference               Comments
 *  Feb 1, 2011      Geetanjali       --------               Initial
 *  May 18,2012      Reetesh                                 Added support
 *                                                           of LTE sib 6, 9
 *
 *  Copyright (c) 2011, Aricent Inc.
 *
 *******************************************************************/
#include "sib_type_defines.h" 
#include "sib_decoder_intf.h"
#include "sibParsing.h"

/*******************************************************************************
 *    FUNCTION NAME: convert_mib
 * 
 *    DESCRIPTION:
 *        This function converts the MasterInformationBlock BCCH_BCH_Message
 *        into mib_info_t and checks whether input data are valid.
 * 
 *    RETURNS:
 *        SUCCESS - all data are valid
 *        FAILURE - data are invalid or if there are not enough memory
 * 
 ********************************************************************************/

return_code convert_mib
(
 BCCH_BCH_Message   *bcchMsg,
 mib_info_t         *mibInfo
 )
{
    return_code ret_val      = SUCCESS;

    if (bcchMsg == NULL)
    {
        return FAILURE;
    }

    memset(mibInfo, 0, sizeof(mib_info_t));

    mibInfo->dl_band_width = bcchMsg->message.dl_Bandwidth;
    mibInfo->phich_config.phich_duration = 
        bcchMsg->message.phich_Config.phich_Duration;
    mibInfo->phich_config.phich_resource =
        bcchMsg->message.phich_Config.phich_Resource;

    memcpy (&(mibInfo->systemFrameNo)
            ,bcchMsg->message.systemFrameNumber.data,
            sizeof((mibInfo)->systemFrameNo));

    memcpy (&(mibInfo->spare),
            bcchMsg->message.spare.data,
            sizeof(mibInfo->spare));

    return ret_val;
}

/******************************************************************************
 *    FUNCTION NAME: convert_cell_access_rel_info
 * 
 *    DESCRIPTION:
 *             This function converts cell_access_related_info_t to
 *             sib_type_1_info_t_cellAccessRelatedInformation and 
 *             checks if input data are valid.
 * 
 ********************************************************************************/

void convert_cell_access_rel_info
( 
 SystemInformationBlockType1   *sib1Msg,
 sib_type_1_Info_t             *sib1Info
 )
{
    U8                    index        = 0;
    OSRTDListNode         *pnode;
    PLMN_IdentityInfo     *pdata0;

    memcpy (sib1Info->cell_access_related_info.tac,
            sib1Msg->cellAccessRelatedInfo.trackingAreaCode.data,
            sizeof(sib1Info->cell_access_related_info.tac));

    memcpy (sib1Info->cell_access_related_info.cell_Id,
            sib1Msg->cellAccessRelatedInfo.cellIdentity.data,
            sizeof(sib1Info->cell_access_related_info.cell_Id));

    sib1Info->cell_access_related_info.cell_barred =
        sib1Msg->cellAccessRelatedInfo.cellBarred;
    sib1Info->cell_access_related_info.intra_freq_reselection =
        sib1Msg->cellAccessRelatedInfo.intraFreqReselection;
    sib1Info->cell_access_related_info.csg_indication = 
        sib1Msg->cellAccessRelatedInfo.csg_Indication;

    if (sib1Msg->cellAccessRelatedInfo.m.csg_IdentityPresent == 1)
    {
        sib1Info->cell_access_related_info.presence_bitmask |=
            CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG; 

        memcpy (sib1Info->cell_access_related_info.csg_identity, 
                sib1Msg->cellAccessRelatedInfo.csg_Identity.data,
                sizeof(sib1Info->cell_access_related_info.csg_identity));
    }

    sib1Info->cell_access_related_info.plmn_Id_info_list.count = 
        sib1Msg->cellAccessRelatedInfo.plmn_IdentityList.count;

    pnode = sib1Msg->cellAccessRelatedInfo.plmn_IdentityList.head;

    for (index = 0; index < (sib1Info->cell_access_related_info.
                plmn_Id_info_list.count); index++)
    {
        pdata0 = (PLMN_IdentityInfo*) pnode->data;

        if (pdata0->plmn_Identity.m.mccPresent == 1)
        {
            sib1Info->cell_access_related_info.plmn_Id_info_list.
                plmn_identity_info[index].plmn_identity.presence_bitmask |=
                PLMN_IDENTITY_MCC_PRESENCE_FLAG;

            memcpy (sib1Info->cell_access_related_info.plmn_Id_info_list.
                    plmn_identity_info[index].plmn_identity.mcc,
                    pdata0->plmn_Identity.mcc.elem,
                    sizeof(U8)*MCC_OCTET_SIZE);
        }
        sib1Info->cell_access_related_info.plmn_Id_info_list.
            plmn_identity_info[index].plmn_identity.mnc.count = 
            pdata0->plmn_Identity.mnc.n;

        memcpy (sib1Info->cell_access_related_info.plmn_Id_info_list.
                plmn_identity_info[index].plmn_identity.mnc.mnc,
                pdata0->plmn_Identity.mnc.elem,sizeof(U8)*MAX_MNC_OCTET_SIZE);

        sib1Info->cell_access_related_info.plmn_Id_info_list.
            plmn_identity_info[index].cell_res_for_operator_use =
            pdata0->cellReservedForOperatorUse;

        pnode = pnode->next;
    }
}

/******************************************************************************
 *    FUNCTION NAME: convert_scheduling_info_list
 * 
 *    DESCRIPTION:
 *        This function converts SchedulingInfoList to
 *        scheduling_info_list_t element and checks if input data are valid.
 *        This function checks number of SIs, si_Periodicity range.
 * 
 *******************************************************************************/

void convert_scheduling_info_list
( 
 SchedulingInfoList         *asn_sch_info_list,
 scheduling_info_list_t     *sch_info_list
 )
{
    SchedulingInfo   *pdata0;
    OSRTDListNode    *pnode;
    U32              index;
    U32 idx = 0;

    sch_info_list->count = asn_sch_info_list->count;

    pnode = asn_sch_info_list->head;

    for (index = 0; index <  sch_info_list->count; index++)
    {
        pdata0 = (SchedulingInfo*) pnode->data;
        sch_info_list->sheduling_info[index].si_periodicity =
            pdata0->si_Periodicity;
        sch_info_list->sheduling_info[index].sib_mapping_info.count =
            pdata0->sib_MappingInfo.n;

        /* SPR 12568 START */
        for (idx = 0; idx < sch_info_list->sheduling_info[index].sib_mapping_info.count; idx++)
        {
            sch_info_list->sheduling_info[index].sib_mapping_info.sib_type[idx]
                            = pdata0->sib_MappingInfo.elem[idx];
        }

        /* SPR 12568 STOP */

        pnode = pnode->next;
    }

}

/******************************************************************************
 *    FUNCTION NAME: convert_sib_type_1_v890
 * 
 *    DESCRIPTION:
 *        This function converts SystemInformationBlock_v890_IEs to
 *        sib_type_1_v890_ie and checks if input data 
 *        are valid.
 * 
 *    RETURNS:
 *        SUCCESS - if all data are valid
 *        FAILURE - if data are invalid or if there are not enough memory
 * 
 *******************************************************************************/

return_code convert_sib_type_1_v890
( 
 SystemInformationBlockType1_v890_IEs             *asn_sib1_non_criti_ext,
 sib_type_1_v890_ie_t                             *sib1_v890_ie
 )
{
    return_code   ret_val      = SUCCESS;

    sib1_v890_ie->late_noncritical_ext.data = NULL;

    if (asn_sib1_non_criti_ext->m.lateNonCriticalExtensionPresent == 1)
    {
        sib1_v890_ie->presence_bitmask |= 
            SIB_TYPE_1_LATE_NON_CRITICAL_EXT_PRESENT_FLAG;
        sib1_v890_ie->late_noncritical_ext.no_octet =
            asn_sib1_non_criti_ext->lateNonCriticalExtension.numocts;
        sib1_v890_ie->late_noncritical_ext.data =
            malloc(sizeof(U8)*asn_sib1_non_criti_ext->lateNonCriticalExtension.numocts);    
        if (sib1_v890_ie->late_noncritical_ext.data == NULL )
        {
            printf("\nMemory Allocation Failure");
            ret_val = FAILURE;
            return ret_val;
        }
        memcpy (sib1_v890_ie->late_noncritical_ext.data,
                asn_sib1_non_criti_ext->lateNonCriticalExtension.data, 
                sib1_v890_ie->late_noncritical_ext.no_octet*(sizeof(U8)));
    }
    if (asn_sib1_non_criti_ext->m.nonCriticalExtensionPresent == 1)
    {
        sib1_v890_ie->presence_bitmask |= 
            SIB_TYPE_1_NON_CRITICAL_EXT_V890_PRESENT_FLAG;

        if (asn_sib1_non_criti_ext->nonCriticalExtension.m.
                ims_EmergencySupport_r9Present == 1)
        {
            sib1_v890_ie->non_critical_ext_v920.presence_bitmask |=
                SIB_TYPE_1_IMS_EMERGENCYSUPPORT_R9_PRESENT_FLAG;
            sib1_v890_ie->non_critical_ext_v920.ims_emergencysupport_r9 = 
                asn_sib1_non_criti_ext->nonCriticalExtension.ims_EmergencySupport_r9;
        }

        if (asn_sib1_non_criti_ext->nonCriticalExtension.m.
                cellSelectionInfo_v920Present == 1)
        {
            sib1_v890_ie->non_critical_ext_v920.presence_bitmask |=
                SIB_TYPE_1_CELL_SELECTION_INFO_PRESENT_FLAG;

            if (asn_sib1_non_criti_ext->nonCriticalExtension.
                    cellSelectionInfo_v920.m.q_QualMinOffset_r9Present == 1)
            {
                sib1_v890_ie->non_critical_ext_v920.cell_selection_info_v920.
                    presence_bitmask |= SIB_TYPE_1_QUAL_MIN_OFFSET_R9_PRESENT_FLAG;

                sib1_v890_ie->non_critical_ext_v920.cell_selection_info_v920.
                    q_qual_min_offset_r9 = asn_sib1_non_criti_ext->nonCriticalExtension.
                    cellSelectionInfo_v920.q_QualMinOffset_r9;
            }

            sib1_v890_ie->non_critical_ext_v920.cell_selection_info_v920.
                q_qual_min_r9 = asn_sib1_non_criti_ext->nonCriticalExtension.
                cellSelectionInfo_v920.q_QualMin_r9;
        }
        if (asn_sib1_non_criti_ext->nonCriticalExtension.m.
                nonCriticalExtensionPresent == 1)
        {
            sib1_v890_ie->non_critical_ext_v920.presence_bitmask |= 
                SIB_TYPE_1_NONCRITICAL_EXT_PRESENT_FLAG;

            sib1_v890_ie->non_critical_ext_v920.noncritical_ext =
                asn_sib1_non_criti_ext->nonCriticalExtension.
                nonCriticalExtension. __dummy__;
        }

    }
    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: convert_sib1
 *
 *   DESCRIPTION:
 *       This function converts SystemInformationBlockType1 to
 *       sib_type_1_Info_t and checks if input data are valid.
 *       Full check of SchedulingInformation_element and si_WindowLength is
 *       out of scope of this function.
 *       This function check si_WindowLength range.
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data are invalid or if there are not enough memory
 *
 *******************************************************************************/

return_code convert_sib1
(
 SystemInformationBlockType1   *sib1Msg,
 sib_type_1_Info_t             *sib1Info
 )
{
    return_code   ret_val      = SUCCESS;

    if (sib1Msg == NULL)
    {
        return FAILURE;
    }

    memset ( sib1Info, 0, sizeof(sib_type_1_Info_t));

    convert_cell_access_rel_info(sib1Msg, sib1Info);

    sib1Info->cell_selection_Info.q_rx_lev_min =
        sib1Msg->cellSelectionInfo.q_RxLevMin;

    if (sib1Msg->cellSelectionInfo.m.q_RxLevMinOffsetPresent == 1)
    {
        sib1Info->cell_selection_Info.presence_bitmask |=
            CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG; 

        sib1Info->cell_selection_Info.q_rx_lev_min_offset = 
            sib1Msg->cellSelectionInfo.q_RxLevMinOffset;
    }

    if (sib1Msg->m.p_MaxPresent == 1)
    {
        sib1Info->presence_bitmask |= SIB_TYPE_1_P_MAX_PRESENCE_FLAG; 
        sib1Info->p_max =  sib1Msg->p_Max;
    }

    sib1Info->freq_band_indicator =  sib1Msg->freqBandIndicator;
    sib1Info->si_window_length =  sib1Msg->si_WindowLength;
    sib1Info->si_value_tag =  sib1Msg->systemInfoValueTag;

    convert_scheduling_info_list (&(sib1Msg->schedulingInfoList), 
            &(sib1Info->scheduling_info_list));

    if (sib1Msg->m.tdd_ConfigPresent == 1)
    {
        sib1Info->presence_bitmask |= SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG;
        sib1Info->tdd_config.sub_frame_assignment =
            sib1Msg->tdd_Config.subframeAssignment;

        sib1Info->tdd_config.special_sub_frame_pattern =
            sib1Msg->tdd_Config.specialSubframePatterns;
    }

    if (sib1Msg->m.nonCriticalExtensionPresent == 1)
    {
        sib1Info->presence_bitmask |= SIB_TYPE_1_NON_CRITICAL_EXT_PRESENT_FLAG;

        ret_val = convert_sib_type_1_v890(&(sib1Msg->nonCriticalExtension),
                &(sib1Info->non_critical_ext_v890));
    }


    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: convert_access_barring_info
 *
 *   DESCRIPTION:
 *       This function converts SystemInformationBlockType2_ac_BarringInfo
 *       to access_barring_info_t
 *       (previously named AccessClassBarringInformation)
 *       and checks whether input data are valid.
 *
 ********************************************************************************/

void convert_access_barring_info
( 
 SystemInformationBlockType2_ac_BarringInfo        *asn_access_bar_info,
 access_barring_info_t                             *access_bar_info
 )
{

    access_bar_info->acBarringForEmergency = 
        asn_access_bar_info->ac_BarringForEmergency;

    if (asn_access_bar_info->m.ac_BarringForMO_SignallingPresent == 1)
    {
        access_bar_info->presence_bitmask |= AC_BARR_INFO_MO_SIG_PRESENCE_FLAG;

        access_bar_info->ac_barring_for_mo_signalling.ac_barring_factor = 
            asn_access_bar_info->ac_BarringForMO_Signalling.ac_BarringFactor;

        access_bar_info->ac_barring_for_mo_signalling.ac_barring_time =
            asn_access_bar_info->ac_BarringForMO_Signalling.ac_BarringTime;

        access_bar_info->ac_barring_for_mo_signalling.
            ac_barring_for_special_ac = asn_access_bar_info->
            ac_BarringForMO_Signalling.ac_BarringForSpecialAC.data[0];
    }
    if (asn_access_bar_info->m.ac_BarringForMO_DataPresent == 1)
    {
        access_bar_info->presence_bitmask |= AC_BARR_INFO_MO_DATA_PRESENCE_FLAG;

        access_bar_info->ac_barring_for_mo_data.ac_barring_factor = 
            asn_access_bar_info->ac_BarringForMO_Data.ac_BarringFactor;

        access_bar_info->ac_barring_for_mo_data.ac_barring_time = 
            asn_access_bar_info->ac_BarringForMO_Data.ac_BarringTime;

        access_bar_info->ac_barring_for_mo_data.ac_barring_for_special_ac =
            asn_access_bar_info->ac_BarringForMO_Data.ac_BarringForSpecialAC.data[0];
    }
}

/******************************************************************************
 *   FUNCTION NAME: convert_radio_resource_config
 *
 *   DESCRIPTION:
 *       This function converts RadioResourceConfigCommonSIB to
 *       radio_resource_config_common_sib_t
 *       and checks whether input data are valid.
 *
 *******************************************************************************/

void convert_radio_resource_config
( 
 RadioResourceConfigCommonSIB          *asn_radio_res_config,
 radio_resource_config_common_sib_t    *radio_res_config
 )
{
    radio_res_config->rach_config_common.preamble_info.ra_preamble_count = 
        asn_radio_res_config->rach_ConfigCommon.preambleInfo.numberOfRA_Preambles;

    if (asn_radio_res_config->rach_ConfigCommon.preambleInfo.m.
            preamblesGroupAConfigPresent == 1)
    {
        radio_res_config->rach_config_common.preamble_info.presence_bitmask |=
            PREAMBLE_INFO_GROUP_A_CONFIG_PRESENCE_FLAG;

        radio_res_config->rach_config_common.preamble_info.
            preambles_group_a_config.ra_preambles_group_a_size = 
            asn_radio_res_config->rach_ConfigCommon.preambleInfo.
            preamblesGroupAConfig.sizeOfRA_PreamblesGroupA;

        radio_res_config->rach_config_common.preamble_info.
            preambles_group_a_config.group_a_msg_size = 
            asn_radio_res_config->rach_ConfigCommon.preambleInfo.
            preamblesGroupAConfig.messageSizeGroupA;

        radio_res_config->rach_config_common.preamble_info.
            preambles_group_a_config.group_b_msg_power_offset = 
            asn_radio_res_config->rach_ConfigCommon.preambleInfo.
            preamblesGroupAConfig.messagePowerOffsetGroupB;
    }

    radio_res_config->rach_config_common.power_ramping_params.
        power_ramping_step = asn_radio_res_config->rach_ConfigCommon.
        powerRampingParameters.powerRampingStep;

    radio_res_config->rach_config_common.power_ramping_params.
        preamble_init_rec_target_pow = asn_radio_res_config->rach_ConfigCommon.
        powerRampingParameters.preambleInitialReceivedTargetPower;

    radio_res_config->rach_config_common.ra_supervision_info.
        preamble_trans_max = asn_radio_res_config->rach_ConfigCommon.
        ra_SupervisionInfo.preambleTransMax;
    radio_res_config->rach_config_common.ra_supervision_info.
        ra_resp_win_size = asn_radio_res_config->rach_ConfigCommon.
        ra_SupervisionInfo.ra_ResponseWindowSize;

    radio_res_config->rach_config_common.ra_supervision_info.
        mac_cont_resol_timer = asn_radio_res_config->rach_ConfigCommon.
        ra_SupervisionInfo.mac_ContentionResolutionTimer;

    radio_res_config->rach_config_common.max_harq_msg_3_tx = 
        asn_radio_res_config->rach_ConfigCommon.maxHARQ_Msg3Tx;

    radio_res_config->bcch_config.mod_period_coeff = 
        asn_radio_res_config->bcch_Config.modificationPeriodCoeff;

    radio_res_config->pcch_config.default_paging_cycle = 
        asn_radio_res_config->pcch_Config.defaultPagingCycle;
    radio_res_config->pcch_config.nb = 
        asn_radio_res_config->pcch_Config.nB;

    radio_res_config->prach_config_sib.root_seq_index = 
        asn_radio_res_config->prach_Config.rootSequenceIndex;
    radio_res_config->prach_config_sib.prach_config_info.prach_config_index = 
        asn_radio_res_config->prach_Config.prach_ConfigInfo.prach_ConfigIndex;
    radio_res_config->prach_config_sib.prach_config_info.high_speed_flag = 
        asn_radio_res_config->prach_Config.prach_ConfigInfo.highSpeedFlag;

    radio_res_config->prach_config_sib.prach_config_info.
        zero_cor_zone_config = asn_radio_res_config->prach_Config.
        prach_ConfigInfo.zeroCorrelationZoneConfig;

    radio_res_config->prach_config_sib.prach_config_info.prach_freq_offset = 
        asn_radio_res_config->prach_Config.prach_ConfigInfo.prach_FreqOffset;

    radio_res_config->ul_cyc_prefix_len = 
        asn_radio_res_config->ul_CyclicPrefixLength;

    radio_res_config->pdsch_config_common.ref_signal_power = 
        asn_radio_res_config->pdsch_ConfigCommon.referenceSignalPower;
    radio_res_config->pdsch_config_common.pb = 
        asn_radio_res_config->pdsch_ConfigCommon.p_b;

    radio_res_config->pusch_config_common.pusch_config_basic.nsb = 
        asn_radio_res_config->pusch_ConfigCommon.pusch_ConfigBasic.n_SB;
    radio_res_config->pusch_config_common.pusch_config_basic.hopping_mode = 
        asn_radio_res_config->pusch_ConfigCommon.pusch_ConfigBasic.hoppingMode;

    radio_res_config->pusch_config_common.pusch_config_basic.
        pusch_hopping_offset = asn_radio_res_config->pusch_ConfigCommon.
        pusch_ConfigBasic.pusch_HoppingOffset;

    radio_res_config->pusch_config_common.pusch_config_basic.enable_64_qam = 
        asn_radio_res_config->pusch_ConfigCommon.pusch_ConfigBasic.enable64QAM;

    radio_res_config->pusch_config_common.ul_ref_signals_pusch.
        group_hopping_enabled = asn_radio_res_config->pusch_ConfigCommon.
        ul_ReferenceSignalsPUSCH.groupHoppingEnabled;

    radio_res_config->pusch_config_common.ul_ref_signals_pusch.
        group_assign_pusch = asn_radio_res_config->pusch_ConfigCommon.
        ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH;

    radio_res_config->pusch_config_common.ul_ref_signals_pusch.
        seq_hopping_enabled = asn_radio_res_config->pusch_ConfigCommon.
        ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled;

    radio_res_config->pusch_config_common.ul_ref_signals_pusch.
        cyclic_shift = asn_radio_res_config->pusch_ConfigCommon.
        ul_ReferenceSignalsPUSCH.cyclicShift;

    radio_res_config->pucch_config_common.delta_pucch_shift = 
        asn_radio_res_config->pucch_ConfigCommon.deltaPUCCH_Shift;
    radio_res_config->pucch_config_common.n_rb_cqi = 
        asn_radio_res_config->pucch_ConfigCommon.nRB_CQI;
    radio_res_config->pucch_config_common.n_cs_an = 
        asn_radio_res_config->pucch_ConfigCommon.nCS_AN;
    radio_res_config->pucch_config_common.n_one_pucch_an = 
        asn_radio_res_config->pucch_ConfigCommon.n1PUCCH_AN;

    if (asn_radio_res_config->soundingRS_UL_ConfigCommon.t == 2)
    {
        radio_res_config->sounding_rs_ul_config_common.presence_bitmask |= 
            SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG;

        radio_res_config->sounding_rs_ul_config_common.setup.
            srs_bw_config =  asn_radio_res_config->soundingRS_UL_ConfigCommon.
            u.setup->srs_BandwidthConfig;
        radio_res_config->sounding_rs_ul_config_common.setup.
            srs_subframe_config =  asn_radio_res_config->soundingRS_UL_ConfigCommon.
            u.setup->srs_SubframeConfig;
        radio_res_config->sounding_rs_ul_config_common.setup.
            ack_nack_srs_simul_trans = asn_radio_res_config->
            soundingRS_UL_ConfigCommon.u.setup->
            ackNackSRS_SimultaneousTransmission;

        if (asn_radio_res_config->soundingRS_UL_ConfigCommon.u.
                setup->m.srs_MaxUpPtsPresent == 1)
        {
            radio_res_config->sounding_rs_ul_config_common.setup.presence_bitmask
                |= SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG ;
            radio_res_config->sounding_rs_ul_config_common.setup.
                srs_max_up_pts = asn_radio_res_config->soundingRS_UL_ConfigCommon.
                u.setup->srs_MaxUpPts;
        }
    }
    else
    {
        radio_res_config->sounding_rs_ul_config_common.presence_bitmask 
            |= SOUNDING_RS_UL_CONFIG_COMMON_RELEASE_PRESENCE_FLAG;
    }
    radio_res_config->uplink_power_control_common.p_zero_nominal_Pusch = 
        asn_radio_res_config->uplinkPowerControlCommon.p0_NominalPUSCH;

    radio_res_config->uplink_power_control_common.alpha = 
        asn_radio_res_config->uplinkPowerControlCommon.alpha;

    radio_res_config->uplink_power_control_common.p_zero_nominal_pucch = 
        asn_radio_res_config->uplinkPowerControlCommon.p0_NominalPUCCH;

    radio_res_config->uplink_power_control_common.delta_f_list_pucch.
        delta_f_pucch_format_one = asn_radio_res_config->
        uplinkPowerControlCommon .deltaFList_PUCCH.deltaF_PUCCH_Format1;

    radio_res_config->uplink_power_control_common.delta_f_list_pucch.
        delta_f_pucch_format_one_b = asn_radio_res_config->
        uplinkPowerControlCommon .deltaFList_PUCCH.deltaF_PUCCH_Format1b;

    radio_res_config->uplink_power_control_common.delta_f_list_pucch.
        delta_f_pucch_format_two = asn_radio_res_config->
        uplinkPowerControlCommon .deltaFList_PUCCH.deltaF_PUCCH_Format2;

    radio_res_config->uplink_power_control_common.delta_f_list_pucch.
        delta_f_pucch_format_two_a = asn_radio_res_config->
        uplinkPowerControlCommon .deltaFList_PUCCH.deltaF_PUCCH_Format2a;

    radio_res_config->uplink_power_control_common.delta_f_list_pucch.
        delta_f_pucch_format_two_b = asn_radio_res_config->
        uplinkPowerControlCommon.  deltaFList_PUCCH.deltaF_PUCCH_Format2b;

    radio_res_config->uplink_power_control_common.delta_preamble_msg_three = 
        asn_radio_res_config->uplinkPowerControlCommon.deltaPreambleMsg3;

}

/******************************************************************************
 *   FUNCTION NAME: fill_subframe_config
 *
 *   DESCRIPTION:
 *       This function converts MBSFN_SubframeConfigList to
 *       mbsfn_subframe_config_list_t
 *       and checks whether input data are valid.
 *
 *******************************************************************************/

void convert_mbsfn_subframe_config_list
( 
 MBSFN_SubframeConfigList          *asn_mbsfn_subframe_list,
 mbsfn_subframe_config_list_t      *mbsfn_subframe_list
 )
{
    OSRTDListNode  *pnode    =NULL; 
    MBSFN_SubframeConfig* pdata0;
    U32            index;

    mbsfn_subframe_list->count = asn_mbsfn_subframe_list->count;
    pnode =  asn_mbsfn_subframe_list->head;
    for(index=0; index< mbsfn_subframe_list->count; index++)
    {
        pdata0 = (MBSFN_SubframeConfig*)pnode->data;
        mbsfn_subframe_list->mbsfn_subframe_config[index].
            radio_frame_alloc_period = pdata0->radioframeAllocationPeriod;

        mbsfn_subframe_list->mbsfn_subframe_config[index].
            radio_frame_alloc_offset = pdata0->radioframeAllocationOffset;

        if (pdata0->subframeAllocation.t == 1)
        {
            mbsfn_subframe_list->mbsfn_subframe_config[index].
                subframe_allocation.presence_bitmask |=
                SUBFRAME_ALLOCATION_ONE_FRAME;

            memcpy (mbsfn_subframe_list->mbsfn_subframe_config[index].
                    subframe_allocation.one_frame, pdata0->subframeAllocation.
                    u.oneFrame->data,
                    SUB_FRAME_ALLOC_ONE_FRAME_OCTET_SIZE*sizeof(U8));
        }
        if (pdata0->subframeAllocation.t == 2)
        {
            mbsfn_subframe_list->mbsfn_subframe_config[index].
                subframe_allocation.presence_bitmask
                |= SUBFRAME_ALLOCATION_FOUR_FRAMES;

            memcpy (mbsfn_subframe_list->mbsfn_subframe_config[index].
                    subframe_allocation.four_frames,
                    pdata0->subframeAllocation.u.fourFrames->data,
                    SUB_FRAME_ALLOC_FOUR_FRAME_OCTET_SIZE*sizeof(U8));
        }
        pnode = pnode->next;
    }

}

/******************************************************************************
 *   FUNCTION NAME: bch_m_check_and_convert_sib2
 *
 *   DESCRIPTION:
 *       This function converts SystemInformationBlockType2 to
 *       sib_type_2_Info_t
 *       and checks whether input data are valid.
 *
 *   RETURNS:
 *       SUCCESS - all data are valid
 *       FAILURE - data are invalid or if there are not enough memory
 *
 *******************************************************************************/

return_code convert_sib2
(
 SystemInformationBlockType2   *sib2Msg,
 sib_type_2_Info_t             *sib2Info
 )
{
    return_code   ret_val      = SUCCESS;

    if (sib2Msg == NULL)
    {
        return FAILURE;
    }

    memset (sib2Info, 0, sizeof(sib_type_2_Info_t));

    if (sib2Msg->m.ac_BarringInfoPresent == 1)
    {
        sib2Info->presence_bitmask |= SIB2_ACCESS_BARR_INFO_PRESENCE_FLAG; 
        convert_access_barring_info(&(sib2Msg->ac_BarringInfo),
                &(sib2Info->access_barring_info));
    }

    convert_radio_resource_config(&(sib2Msg->radioResourceConfigCommon),
            &(sib2Info->radio_resource_config_common_sib));

    sib2Info->ue_timers_and_constants.timer_300 = 
        sib2Msg->ue_TimersAndConstants.t300;
    sib2Info->ue_timers_and_constants.timer_301 = 
        sib2Msg->ue_TimersAndConstants.t301;
    sib2Info->ue_timers_and_constants.timer_310 =
        sib2Msg->ue_TimersAndConstants.t310;
    sib2Info->ue_timers_and_constants.timer_n310 = 
        sib2Msg->ue_TimersAndConstants.n310;
    sib2Info->ue_timers_and_constants.timer_311 = 
        sib2Msg->ue_TimersAndConstants.t311;
    sib2Info->ue_timers_and_constants.timer_n311 = 
        sib2Msg->ue_TimersAndConstants.n311;

    if (sib2Msg->freqInfo.m.ul_CarrierFreqPresent == 1)
    {
        sib2Info->freq_info.presence_bitmask |= 
            FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG;
        sib2Info->freq_info.ul_carrier_freq =
            sib2Msg->freqInfo.ul_CarrierFreq;
    }

    if (sib2Msg->freqInfo.m.ul_BandwidthPresent == 1)
    {
        sib2Info->freq_info.presence_bitmask |= 
            FREQ_INFO_UL_BW_PRESENCE_FLAG;
        sib2Info->freq_info.ul_bandwidth = 
            sib2Msg->freqInfo.ul_Bandwidth;
    }
    sib2Info->freq_info.add_spectrum_emission =
        sib2Msg->freqInfo.additionalSpectrumEmission;

    if (sib2Msg->m.mbsfn_SubframeConfigListPresent == 1)
    {
        sib2Info->presence_bitmask |= 
            SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG;
        convert_mbsfn_subframe_config_list(&(sib2Msg->mbsfn_SubframeConfigList)
                , &(sib2Info->mbsfn_subframe_config_list));
    }

    sib2Info->time_align_timer = sib2Msg->timeAlignmentTimerCommon;

    sib2Info->late_noncritical_ext.data = NULL;

    if (sib2Msg->m.lateNonCriticalExtensionPresent == 1)
    {
        sib2Info->presence_bitmask |= 
            SIB_TYPE_2_LATE_NON_CRITICAL_EXT_PRESENT_FLAG;
        sib2Info->late_noncritical_ext.no_octet =
            sib2Msg->lateNonCriticalExtension.numocts;
        sib2Info->late_noncritical_ext.data =
            malloc(sizeof(U8)* sib2Msg->lateNonCriticalExtension.numocts);    
        if (sib2Info->late_noncritical_ext.data == NULL)
        {
            printf("\nMemory Allocation Failure");
            ret_val = FAILURE;
            return ret_val;
        }
        memcpy (sib2Info->late_noncritical_ext.data,
                sib2Msg->lateNonCriticalExtension.data, 
                sib2Info->late_noncritical_ext.no_octet*(sizeof(U8)));
    }

    if (sib2Msg->m.ssac_BarringForMMTEL_Voice_r9Present == 1)
    {
        sib2Info->presence_bitmask |=
            SIB_TYPE_2_SSAC_BARRING_VOICE_R9_PRESENT_FLAG; 
        sib2Info->ssac_barring_for_mmtel_voice_r9.ac_barring_factor =
            sib2Msg->ssac_BarringForMMTEL_Voice_r9.ac_BarringFactor;
        sib2Info->ssac_barring_for_mmtel_voice_r9.ac_barring_time = 
            sib2Msg->ssac_BarringForMMTEL_Voice_r9.ac_BarringTime;
        sib2Info->ssac_barring_for_mmtel_voice_r9.ac_barring_for_special_ac = 
            sib2Msg->ssac_BarringForMMTEL_Voice_r9.ac_BarringForSpecialAC.data[0];
    }
    if (sib2Msg->m.ssac_BarringForMMTEL_Video_r9Present == 1)
    {
        sib2Info->presence_bitmask |= 
            SIB_TYPE_2_SSAC_BARRING_VIDEO_R9_PRESENT_FLAG; 
        sib2Info->ssac_barring_for_mmtel_video_r9.ac_barring_factor =
            sib2Msg->ssac_BarringForMMTEL_Video_r9.ac_BarringFactor;
        sib2Info->ssac_barring_for_mmtel_video_r9.ac_barring_time = 
            sib2Msg->ssac_BarringForMMTEL_Video_r9.ac_BarringTime;
        sib2Info->ssac_barring_for_mmtel_video_r9.ac_barring_for_special_ac = 
            sib2Msg->ssac_BarringForMMTEL_Video_r9.ac_BarringForSpecialAC.data[0];
    }

    return ret_val;

}

/*********************************************************************************************
 *   FUNCTION NAME: convert_cell_reselect_info
 *
 *   DESCRIPTION:
 *       This function convert
 *       SystemInformationBlockType3_cellReselectionInfoCommon to
 *       cell_reselect_info_comm_t and check if input data are valid.
 *
 *********************************************************************************************/

void convert_cell_reselect_info
( 
 SystemInformationBlockType3_cellReselectionInfoCommon    *asn_cell_reselect_info,
 cell_reselect_info_comm_t                                *cell_reselect_info
 )
{
    cell_reselect_info->q_hyst = asn_cell_reselect_info->q_Hyst;
    if (asn_cell_reselect_info->m.speedStateReselectionParsPresent == 1)
    {
        cell_reselect_info->presence_bitmask |= 
            CELL_RESELECT_SPEED_DEPEND_RESELECT_PRESENCE_FLAG;
        cell_reselect_info->speed_depend_reselect.mobility_state_params.
            t_evaluation = asn_cell_reselect_info->speedStateReselectionPars.
            mobilityStateParameters.t_Evaluation;
        cell_reselect_info->speed_depend_reselect.mobility_state_params.
            t_hyst_normal = asn_cell_reselect_info->speedStateReselectionPars.
            mobilityStateParameters.t_HystNormal;
        cell_reselect_info->speed_depend_reselect.mobility_state_params.
            n_cell_change_medium = asn_cell_reselect_info->speedStateReselectionPars.
            mobilityStateParameters.n_CellChangeMedium;
        cell_reselect_info->speed_depend_reselect.mobility_state_params.
            n_cell_change_high = asn_cell_reselect_info->speedStateReselectionPars.
            mobilityStateParameters.n_CellChangeHigh;
        cell_reselect_info->speed_depend_reselect.speed_depend_scaling_param_hyst.
            q_hyst_sf_medium = asn_cell_reselect_info->speedStateReselectionPars.
            q_HystSF.sf_Medium;
        cell_reselect_info->speed_depend_reselect.speed_depend_scaling_param_hyst.
            q_hyst_sf_high = asn_cell_reselect_info->speedStateReselectionPars.
            q_HystSF.sf_High;
    }
}

/*********************************************************************************************
 *   FUNCTION NAME: convert_intra_freq_cell_reselect
 *
 *   DESCRIPTION:
 *       This function convert
 *       SystemInformationBlockType3_intraFreqCellReselectionInfo to
 *       intra_freq_cell_reselect_info_t and check if input data are valid.
 *
 * **********************************************************************************************/

void convert_intra_freq_cell_reselect
( 
 SystemInformationBlockType3_intraFreqCellReselectionInfo     *asn_intra_freq_cell_reselect,
 intra_freq_cell_reselect_info_t                              *intra_freq_cell_reselect
 )
{
    intra_freq_cell_reselect->q_rx_lev_min = 
        asn_intra_freq_cell_reselect->q_RxLevMin;
    if (asn_intra_freq_cell_reselect->m.p_MaxPresent == 1)
    {
        intra_freq_cell_reselect->presence_bitmask |= 
            INTRA_FREQ_CELL_RESELECT_P_MAX_PRESENCE_FLAG;
        intra_freq_cell_reselect->p_max =
            asn_intra_freq_cell_reselect->p_Max;
    }
    if (asn_intra_freq_cell_reselect->m.s_IntraSearchPresent == 1)
    {
        intra_freq_cell_reselect->presence_bitmask |= 
            INTRA_FREQ_CELL_RESELECT_INTRA_SEARCH_PRESENCE_FLAG;
        intra_freq_cell_reselect->s_intra_search =
            asn_intra_freq_cell_reselect->s_IntraSearch;
    }
    if (asn_intra_freq_cell_reselect->m.allowedMeasBandwidthPresent == 1)
    {
        intra_freq_cell_reselect->presence_bitmask |= 
            INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRESENCE_FLAG;
        intra_freq_cell_reselect->measure_bw = 
            asn_intra_freq_cell_reselect->allowedMeasBandwidth;
    }
    intra_freq_cell_reselect->presence_antenna_port_1 = 
        asn_intra_freq_cell_reselect->presenceAntennaPort1;
    intra_freq_cell_reselect->neigh_cell_config = 
        asn_intra_freq_cell_reselect->neighCellConfig.data[0];
    intra_freq_cell_reselect->t_reselec_eutra =
        asn_intra_freq_cell_reselect->t_ReselectionEUTRA;

    if (asn_intra_freq_cell_reselect->m.t_ReselectionEUTRA_SFPresent == 1)
    {
        intra_freq_cell_reselect->presence_bitmask |= 
            INTRA_FREQ_CELL_RESELECT_EUTRAN_SF_PRESENCE_FLAG;
        intra_freq_cell_reselect->t_reselect_eutra_sf.sf_medium = 
            asn_intra_freq_cell_reselect->t_ReselectionEUTRA_SF.sf_Medium;
        intra_freq_cell_reselect->t_reselect_eutra_sf.sf_high = 
            asn_intra_freq_cell_reselect->t_ReselectionEUTRA_SF.sf_High;
    }
}

/******************************************************************************
 *   FUNCTION NAME: convert_sib3
 *
 *   DESCRIPTION:
 *       This function convert SystemInformationBlockType3 to
 *       sib_type_3_Info_t
 *       and check if input data are valid.
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data is invalid or if there are not enough memory
 *
 *******************************************************************************/

return_code convert_sib3
(
 SystemInformationBlockType3   *sib3Msg,
 sib_type_3_Info_t             *sib3Info
 )
{
    return_code   ret_val      = SUCCESS;

    if (sib3Msg == NULL)
    {
        return FAILURE;
    }

    memset (sib3Info, 0, sizeof(sib_type_3_Info_t));

    convert_cell_reselect_info (&(sib3Msg->cellReselectionInfoCommon),
            &(sib3Info->timecell_reselect_info_comm));

    if (sib3Msg->cellReselectionServingFreqInfo.m.s_NonIntraSearchPresent == 1)
    {
        sib3Info->cell_reselect_serv_freq_info.presence_bitmask |= 
            CELL_RESELECT_NON_INTRA_SEARCH_PRESENCE_FLAG; 
        sib3Info->cell_reselect_serv_freq_info.s_non_intra_search = 
            sib3Msg->cellReselectionServingFreqInfo.s_NonIntraSearch;
    }
    sib3Info->cell_reselect_serv_freq_info.thresh_serving_low = 
        sib3Msg->cellReselectionServingFreqInfo.threshServingLow;
    sib3Info->cell_reselect_serv_freq_info.cell_reselect_priority = 
        sib3Msg->cellReselectionServingFreqInfo.cellReselectionPriority;

    convert_intra_freq_cell_reselect (&(sib3Msg->intraFreqCellReselectionInfo),
            &(sib3Info->intra_freq_cell_reselect_info));

    sib3Info->late_noncritical_ext.data = NULL;

    if (sib3Msg->m.lateNonCriticalExtensionPresent == 1)
    {
        sib3Info->presence_bitmask |= 
            SIB_TYPE_3_LATE_NON_CRITICAL_EXT_PRESENT_FLAG;
        sib3Info->late_noncritical_ext.no_octet =
            sib3Msg->lateNonCriticalExtension.numocts;
        sib3Info->late_noncritical_ext.data = 
            malloc(sizeof(U8)* sib3Msg->lateNonCriticalExtension.numocts);    
        if (sib3Info->late_noncritical_ext.data == NULL )
        {
            printf("\nMemory Allocation Failure");
            ret_val = FAILURE;
            return ret_val;
        }
        memcpy (sib3Info->late_noncritical_ext.data,
                sib3Msg->lateNonCriticalExtension.data, 
                sib3Info->late_noncritical_ext.no_octet*(sizeof(U8)));
    }
    if (sib3Msg->m.s_IntraSearch_v920Present == 1)
    {
        sib3Info->presence_bitmask |= 
            INTRA_SEARCH_V920_PRESENCE_FLAG;
        sib3Info->s_intra_search_v920.s_intra_search_p_r9 = 
            sib3Msg->s_IntraSearch_v920.s_IntraSearchP_r9;
        sib3Info->s_intra_search_v920.s_intra_search_q_r9 =
            sib3Msg->s_IntraSearch_v920.s_IntraSearchQ_r9;
    }

    if (sib3Msg->m.s_NonIntraSearch_v920Present == 1)
    {
        sib3Info->presence_bitmask |=
            NON_INTRA_SEARCH_V920_PRESENCE_FLAG;
        sib3Info->s_non_intra_search_v920.s_non_intra_search_p_r9 = 
            sib3Msg->s_NonIntraSearch_v920.s_NonIntraSearchP_r9;
        sib3Info->s_non_intra_search_v920.s_non_intra_search_q_r9 =
            sib3Msg->s_NonIntraSearch_v920.s_NonIntraSearchQ_r9;
    }

    if (sib3Msg->m.q_QualMin_r9Present == 1)
    {
        sib3Info->presence_bitmask |= QUALMIN_R9_PRESENCE_FLAG;
        sib3Info->q_qualmin_r9 = sib3Msg->q_QualMin_r9;
    }

    if (sib3Msg->m.threshServingLowQ_r9Present == 1)
    {
        sib3Info->presence_bitmask |= RESELECTION_THRESHOLDQ_R9_PRESENCE_FLAG;
        sib3Info->reselection_thresholdq_r9 = sib3Msg->threshServingLowQ_r9; 
    }

    return ret_val;

}

/******************************************************************************
 *   FUNCTION NAME: convert_neigh_cell_list
 *
 *   DESCRIPTION:
 *       This function convert IntraFreqNeighCellList to
 *       neigh_cell_list_t.
 *
 ********************************************************************************/

void convert_neigh_cell_list
( 
 IntraFreqNeighCellList            *asn_neigh_cell_list,
 neigh_cell_list_t                 *neigh_cell_list
 )
{
    IntraFreqNeighCellInfo   *pdata;
    OSRTDListNode            *pnode;
    S32                      index;

    neigh_cell_list->count = asn_neigh_cell_list->count;
    pnode =  asn_neigh_cell_list->head;
    for (index=0; index<neigh_cell_list->count; index++)
    {
        pdata = (IntraFreqNeighCellInfo*)pnode->data;
        neigh_cell_list->neigh_cell[index].phys_cell_id = pdata->physCellId;
        neigh_cell_list->neigh_cell[index].q_offset_cell = pdata->q_OffsetCell;
        pnode = pnode->next;
    }
}

/******************************************************************************
 *   FUNCTION NAME: convert_black_cell_list
 *
 *   DESCRIPTION:
 *       This function convert IntraFreqBlackCellList to
 *       black_listed_cell_list_t
 *       and check if input data are valid.
 *
 *********************************************************************************/

void convert_black_cell_list
( 
 IntraFreqBlackCellList            *asn_black_cell_list,
 black_listed_cell_list_t          *black_cell_list
 )
{
    PhysCellIdRange          *pdata;
    OSRTDListNode            *pnode;
    S32                      index;

    black_cell_list->count = asn_black_cell_list->count;
    pnode =  asn_black_cell_list->head;
    for (index=0; index<black_cell_list->count; index++)
    {
        pdata = (PhysCellIdRange*)pnode->data;
        black_cell_list->black_listed_cell[index].start = pdata->start;
        if ( pdata->m.rangePresent == 1)
        {
            black_cell_list->black_listed_cell[index].presence_bitmask |=
                PHY_CELL_ID_RANGE_PRESENCE_FLAG;
            black_cell_list->black_listed_cell[index].range = pdata->range;
        }
        pnode = pnode->next;
    }
}

/******************************************************************************
 *   FUNCTION NAME: convert_sib4
 *
 *   DESCRIPTION:
 *       This function convert SystemInformationBlockType4 to
 *       sib_type_4_Info_t
 *       and check if input data are valid.
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data is invalid or if there are not enough memory
 *
 ********************************************************************************/

return_code convert_sib4
(
 SystemInformationBlockType4   *sib4Msg,        
 sib_type_4_Info_t             *sib4Info       
 )
{
    return_code   ret_val      = SUCCESS;

    if (sib4Msg == NULL)
    {
        return FAILURE;
    }

    memset (sib4Info, 0, sizeof(sib_type_4_Info_t));

    if (sib4Msg->m.intraFreqNeighCellListPresent == 1)
    {
        sib4Info->presence_bitmask |= SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG; 
        convert_neigh_cell_list (&(sib4Msg->intraFreqNeighCellList), 
                &(sib4Info->intra_freq_neigh_cell_list)); 
    }
    if (sib4Msg->m.intraFreqBlackCellListPresent == 1)
    {
        sib4Info->presence_bitmask |= SIB_4_BLACK_LIST_LIST_PRESENCE_FLAG; 
        convert_black_cell_list (&(sib4Msg->intraFreqBlackCellList), 
                &(sib4Info->intra_freq_black_listed_cell_list)); 
    }
    if (sib4Msg->m.csg_PhysCellIdRangePresent == 1)
    {
        sib4Info->presence_bitmask |= SIB_4_PHY_CELL_ID_PRESENCE_FLAG; 
        sib4Info->csg_phy_cell_id_range.start =
            sib4Msg->csg_PhysCellIdRange.start;
        if (sib4Msg->csg_PhysCellIdRange.m.rangePresent == 1)
        {
            sib4Info->csg_phy_cell_id_range.presence_bitmask |=
                PHY_CELL_ID_RANGE_PRESENCE_FLAG;
            sib4Info->csg_phy_cell_id_range.range = 
                sib4Msg->csg_PhysCellIdRange.range;
        }
    }
    sib4Info->late_noncritical_ext.data = NULL;

    if (sib4Msg->m.lateNonCriticalExtensionPresent == 1)
    {
        sib4Info->presence_bitmask |= 
            SIB_4_LATE_NON_CRITICAL_EXT_PRESENT_FLAG;
        sib4Info->late_noncritical_ext.no_octet = 
            sib4Msg->lateNonCriticalExtension.numocts;
        sib4Info->late_noncritical_ext.data =  
            malloc(sizeof(U8)* sib4Msg->lateNonCriticalExtension.numocts);    
        if (sib4Info->late_noncritical_ext.data == NULL)
        {
            printf("\nMemory Allocation Failure");
            ret_val = FAILURE;
            return ret_val;
        }
        memcpy (sib4Info->late_noncritical_ext.data,
                sib4Msg->lateNonCriticalExtension.data, 
                sib4Info->late_noncritical_ext.no_octet*(sizeof(U8)));
    }

    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: convert_sib5
 *
 *   DESCRIPTION:
 *       This function convert SystemInformationBlockType5 to
 *       sib_type_5_Info_t
 *       and check if input data are valid.
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data is invalid or if there are not enough memory
 *
 *******************************************************************************/

return_code convert_sib5
(
 SystemInformationBlockType5   *sib5Msg,
 sib_type_5_Info_t             *sib5Info
 )
{
    InterFreqCarrierFreqInfo  *pdata;
    OSRTDListNode             *pnode;
    S32                       index;
    return_code             ret_val  = SUCCESS;


    if (sib5Msg == NULL)
    {
        return FAILURE;
    }

    memset (sib5Info, 0, sizeof(sib_type_5_Info_t));

    sib5Info->inter_freq_carrier_freq_list.count =
        sib5Msg->interFreqCarrierFreqList.count;
    pnode =  sib5Msg->interFreqCarrierFreqList.head;
    for (index=0; index<(sib5Info->inter_freq_carrier_freq_list.count); index++)
    {
        pdata = (InterFreqCarrierFreqInfo*)pnode->data;

        sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
            eutra_dl_carrier_freq = pdata->dl_CarrierFreq;

        sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
            qrx_lev_min = pdata->q_RxLevMin;
        if (pdata->m.p_MaxPresent == 1)
        {
            sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
                presence_bitmask |= INTER_FREQ_CARR_P_MAX_PRESENCE_FLAG;
            sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
                p_max = pdata->p_Max;
        }
        sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
            t_reselection_eutran= pdata->t_ReselectionEUTRA;
        if (pdata->m.t_ReselectionEUTRA_SFPresent == 1)
        {
            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].presence_bitmask |=
                INTER_FREQ_CARR_SCALE_PARAM_PRESENCE_FLAG;

            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].speed_depend_scal_params.
                sf_medium = pdata->t_ReselectionEUTRA_SF.sf_Medium;

            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].speed_depend_scal_params.
                sf_high = pdata->t_ReselectionEUTRA_SF.sf_High;
        }
        sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
            thresh_x_high = pdata->threshX_High;
        sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
            thresh_x_low = pdata->threshX_Low;
        sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
            measurement_bandwidth = pdata->allowedMeasBandwidth;
        sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
            presence_antenna_port_1 = pdata->presenceAntennaPort1;
        if (pdata->m.cellReselectionPriorityPresent == 1)
        {
            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].presence_bitmask |=
                INTER_FREQ_CARR_CELL_RESELCT_PRIOR_PRESENCE_FLAG;
            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].cell_reselect_priority =
                pdata->cellReselectionPriority;
        }
        sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
            neigh_cell_config = pdata->neighCellConfig.data[0];;
        sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].
            q_offset_freq = pdata->q_OffsetFreq;
        if (pdata->m.interFreqNeighCellListPresent == 1)
        {
            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].presence_bitmask |=
                INTER_FREQ_CARR_NEIGH_CELL_LIST_PRESENCE_FLAG;

            convert_neigh_cell_list ( &(pdata->interFreqNeighCellList), 
                    &(sib5Info->inter_freq_carrier_freq_list.
                        inter_freq_carrier_freq_list[index].
                        inter_freq_neigh_cell_list)); 
        }
        if (pdata->m.interFreqBlackCellListPresent == 1)
        {
            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].presence_bitmask |=
                INTER_FREQ_CARR_BLACK_LIST_CELL_LIST_PRESENCE_FLAG;

            convert_black_cell_list ( &(pdata->interFreqBlackCellList),
                    &(sib5Info->inter_freq_carrier_freq_list.
                        inter_freq_carrier_freq_list[index].
                        inter_freq_black_listed_cell_list)); 
        }
        if (pdata->m.q_QualMin_r9Present == 1)
        {
            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].presence_bitmask |= 
                INTER_FREQ_CARR_Q_QUAL_MIN_R9_PRESENCE_FLAG;

            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].
                q_qual_min_r9 = pdata->q_QualMin_r9;
        }
        if (pdata->m.threshX_Q_r9Present == 1)
        {
            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].
                presence_bitmask |= INTER_FREQ_THRESH_X_Q_R9_PRESENCE_FLAG;

            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].thresh_x_q_r9.
                thresh_x_high_q_r9 = pdata->threshX_Q_r9.threshX_HighQ_r9;
            sib5Info->inter_freq_carrier_freq_list.
                inter_freq_carrier_freq_list[index].thresh_x_q_r9.
                thresh_x_low_q_r9 = pdata->threshX_Q_r9.threshX_LowQ_r9;
        }
        pnode = pnode->next;
    }

    sib5Info->late_noncritical_ext.data = NULL;

    if (sib5Msg->m.lateNonCriticalExtensionPresent == 1)
    {
        sib5Info->presence_bitmask |= 
            SIB_5_LATE_NON_CRITICAL_EXT_PRESENT_FLAG;
        sib5Info->late_noncritical_ext.no_octet =
            sib5Msg->lateNonCriticalExtension.numocts;
        sib5Info->late_noncritical_ext.data =
            malloc(sizeof(U8)* sib5Msg->lateNonCriticalExtension.numocts);    
        if (sib5Info->late_noncritical_ext.data == NULL)
        {
            printf("\nMemory Allocation Failure");
            ret_val = FAILURE;
            return ret_val;
        }
        memcpy (sib5Info->late_noncritical_ext.data,
                sib5Msg->lateNonCriticalExtension.data, 
                sib5Info->late_noncritical_ext.no_octet*(sizeof(U8)));
    }

    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: convert_sib6
 *
 *   DESCRIPTION:
 *       This function convert SystemInformationBlockType6 to
 *       sib_type_6_Info_t
 *       and check if input data are valid.
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data is invalid or if there are not enough memory
 *
 *******************************************************************************/

return_code convert_sib6
(
 SystemInformationBlockType6   *sib6Msg,
 sib_type_6_Info_t             *sib6Info
 )
{
    //RKG
    CarrierFreqUTRA_FDD       *pdata_fdd;
    CarrierFreqUTRA_TDD       *pdata_tdd;
    OSRTDListNode             *pnode;
    U32                       index;
    if (sib6Msg == NULL)
    {
        return FAILURE;
    }

    memset (sib6Info, 0, sizeof(sib_type_6_Info_t));
    if(sib6Msg->m.carrierFreqListUTRA_FDDPresent == 1)
    {
        sib6Info->carrier_freq_list_UTRA_FDD.count =
            sib6Msg->carrierFreqListUTRA_FDD.count;
        pnode = sib6Msg->carrierFreqListUTRA_FDD.head; 
        for (index=0; index<(sib6Info->carrier_freq_list_UTRA_FDD.count); index++)
        {
            pdata_fdd = (CarrierFreqUTRA_FDD *)pnode->data;
            sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].carrier_freq 
                = pdata_fdd->carrierFreq;
            if(pdata_fdd->m.cellReselectionPriorityPresent == 1)
            {
                sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index]
                    .cell_reselection_priority = pdata_fdd->cellReselectionPriority;
                sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].
                    presence_bitmask |= CARRIER_FREQ_UTRA_FDD_CELL_RESELECTION_PRIORITY;
            }
            sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].
                thresh_X_High = pdata_fdd->threshX_High;
            sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].
                thresh_X_Low = pdata_fdd->threshX_Low;
            sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].
                q_Rx_Lev_Min = pdata_fdd->q_RxLevMin;
            sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].
                p_Max_UTRA = pdata_fdd->p_MaxUTRA;
            sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].
                q_Qual_min = pdata_fdd->q_QualMin;
            if(pdata_fdd->m.threshX_Q_r9Present == 1)
            {
                sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].
                    thresh_X_Q_r9.threshX_highQ_r9 = pdata_fdd->threshX_Q_r9.threshX_HighQ_r9;
                sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].
                    thresh_X_Q_r9.threshX_LowQ_r9 = pdata_fdd->threshX_Q_r9.threshX_LowQ_r9;
                sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].
                    presence_bitmask |= CARRIER_FREQ_UTRA_FDD_THRESHX_Q_R9;
            }

        }
        sib6Info->presence_bitmask |= CARRIER_FREQ_LIST_UTRA_FDD;
    }
    /* Start with decoding of CarrierFreqUTRA_TDD*/
    if(sib6Msg->m.carrierFreqListUTRA_TDDPresent == 1)
    {
        sib6Info->carrier_freq_list_UTRA_TDD.count =
            sib6Msg->carrierFreqListUTRA_TDD.count;
        pnode = sib6Msg->carrierFreqListUTRA_TDD.head; 
        for (index=0; index<(sib6Info->carrier_freq_list_UTRA_TDD.count); index++)
        {
            pdata_tdd = (CarrierFreqUTRA_TDD *)pnode->data;
            sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].carrier_freq 
                = pdata_tdd->carrierFreq;
            if(pdata_tdd->m.cellReselectionPriorityPresent == 1)
            {
                sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index]
                    .cell_reselection_priority = pdata_tdd->cellReselectionPriority;
                sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].
                    presence_bitmask |= CARRIER_FREQ_UTRA_TDD_CELL_RESELECTION_PRIORITY;
            }
            sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].
                thresh_X_High = pdata_tdd->threshX_High;
            sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].
                thresh_X_Low = pdata_tdd->threshX_Low;
            sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].
                q_Rx_Lev_Min = pdata_tdd->q_RxLevMin;
            sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].
                p_Max_UTRA = pdata_tdd->p_MaxUTRA;
        }
        sib6Info->presence_bitmask |= CARRIER_FREQ_LIST_UTRA_TDD;
    }
    sib6Info->t_reselection_UTRA = sib6Msg->t_ReselectionUTRA; 
    if(sib6Msg->m.t_ReselectionUTRA_SFPresent == 1)
    {
        sib6Info->t_reselection_UTRA_SF.sf_medium = sib6Msg->t_ReselectionUTRA_SF.sf_Medium;
        sib6Info->t_reselection_UTRA_SF.sf_high = sib6Msg->t_ReselectionUTRA_SF.sf_High;
        sib6Info->presence_bitmask |= T_RESELECTION_UTRA_SF;
    }

    return SUCCESS;
}

/******************************************************************************
 *   FUNCTION NAME: convert_sib9
 *
 *   DESCRIPTION:
 *       This function convert SystemInformationBlockType9 to
 *       sib_type_9_Info_t
 *       and check if input data are valid.
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data is invalid or if there are not enough memory
 *
 *******************************************************************************/

return_code convert_sib9
(
 SystemInformationBlockType9   *sib9Msg,
 sib_type_9_Info_t             *sib9Info
 )
{
    return_code retval = FAILURE;
    if (sib9Msg == NULL)
    {
        return retval;
    }

    memset (sib9Info, 0, sizeof(sib_type_9_Info_t));
    if(sib9Msg->m.hnb_NamePresent == 1)
    {
        sib9Info->count = sib9Msg->hnb_Name.numocts; 
        memcpy(sib9Info->hnb_Name, sib9Msg->hnb_Name.data, sib9Info->count);
        retval = SUCCESS;
    }
    return retval;
}
/******************************************************************************
 *   FUNCTION NAME: check_and_decode_sibs
 *
 *   DESCRIPTION:
 *       This function decode BCCH_DL_SCH_Message to
 *       sib_messges_t
 *       and check if input data are valid.
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data is invalid or if there are not enough memory
 *
 *******************************************************************************/
S32 check_and_decode_sibs
(
 BCCH_DL_SCH_Message      *asn_decoded_sibs,
 sib_messages_t            *decoded_buf
 )
{
    return_code     ret_val   = SUCCESS;
    OSRTDListNode     *pnode;
    U8                index;

    SystemInformation_r8_IEs_sib_TypeAndInfo_element *pdata;

    if (asn_decoded_sibs->message.t == T_BCCH_DL_SCH_MessageType_c1 &&
            asn_decoded_sibs->message.u.c1->t == 
            T_BCCH_DL_SCH_MessageType_c1_systemInformationBlockType1)
    {
        ret_val =  convert_sib1 ((asn_decoded_sibs->message.u.c1->u.
                    systemInformationBlockType1),
                &(decoded_buf->sib_type_1_Info));    
        if (ret_val == SUCCESS)
        {
            decoded_buf->presence_bitmask |= SIB_TYPE_1_PRESENCE_FLAG;
        }
        else
        {
            return ret_val;
        }
    }
    if (asn_decoded_sibs->message.t == T_BCCH_DL_SCH_MessageType_c1 && 
            asn_decoded_sibs->message.u.c1->t == 
            T_BCCH_DL_SCH_MessageType_c1_systemInformation && 
            asn_decoded_sibs->message.u.c1->u.systemInformation ->
            criticalExtensions.t ==
            T_SystemInformation_criticalExtensions_systemInformation_r8 )
    { 
        pnode = asn_decoded_sibs->message.u.c1->u.systemInformation->
            criticalExtensions.u.systemInformation_r8->sib_TypeAndInfo.head;

        for (index=0; index<(asn_decoded_sibs->message.u.c1->u.
                    systemInformation->criticalExtensions.u.systemInformation_r8->
                    sib_TypeAndInfo.count); index++)
        {
            pdata = ((SystemInformation_r8_IEs_sib_TypeAndInfo_element*)pnode->data);
            if (pdata->t == 
                    T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib2)
            {
                ret_val =  convert_sib2 (pdata->u.sib2,
                        &(decoded_buf->sib_type_2_Info));    
                if (ret_val == SUCCESS)
                {
                    decoded_buf->presence_bitmask |= SIB_TYPE_2_PRESENCE_FLAG;
                }
                else
                {
                    return ret_val;
                }
            }
            if (pdata->t ==
                    T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib3)
            {
                ret_val =  convert_sib3 (pdata->u.sib3,
                        &(decoded_buf->sib_type_3_Info));    
                if (ret_val == SUCCESS)
                {
                    decoded_buf->presence_bitmask |= SIB_TYPE_3_PRESENCE_FLAG;
                }
                else
                {
                    return ret_val;
                }
            }
            if (pdata->t == 
                    T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib4)
            {
                ret_val =  convert_sib4 (pdata->u.sib4,
                        &(decoded_buf->sib_type_4_Info));    
                if (ret_val == SUCCESS)
                {
                    decoded_buf->presence_bitmask |= SIB_TYPE_4_PRESENCE_FLAG;
                }
                else
                {
                    return ret_val;
                }
            }
            if (pdata->t == 
                    T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib5)
            {
                ret_val = convert_sib5 (pdata->u.sib5,
                        &(decoded_buf->sib_type_5_Info));    
                if (ret_val == SUCCESS)
                {
                    decoded_buf->presence_bitmask |= SIB_TYPE_5_PRESENCE_FLAG;
                }
                else
                {
                    return ret_val;
                }
            }
            if (pdata->t == 
                    T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib6)
            {
                ret_val = convert_sib6 (pdata->u.sib6,
                        &(decoded_buf->sib_type_6_Info));    
                if (ret_val == SUCCESS)
                {
                    decoded_buf->presence_bitmask |= SIB_TYPE_6_PRESENCE_FLAG;
                }
                else
                {
                    return ret_val;
                }
            }
            if (pdata->t == 
                    T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib9)
            {
                ret_val = convert_sib9 (pdata->u.sib9,
                        &(decoded_buf->sib_type_9_Info));    
                if (ret_val == SUCCESS)
                {
                    decoded_buf->presence_bitmask |= SIB_TYPE_9_PRESENCE_FLAG;
                }
                else
                {
                    return ret_val;
                }
            }
            pnode = pnode->next;
        }
    }
    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: mib_decoder
 *
 *   DESCRIPTION:
 *       This is interface function for mib decoding
 *       It converts encoded mib data into user mib_messges_t
 *       structure.
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data is invalid or if there are not enough memory
 *
 *******************************************************************************/
U8 mib_decoder
(
 U8                *encoded_mib_msg,
 mib_messages_t    *decoded_mib_msg,
 U16               encoded_mib_len
 )
{
    BCCH_BCH_Message   asn_decoded_mib;
    OSCTXT             ctxt;
    S32                stat         = 0;
    return_code      ret_val      = SUCCESS;

    decoded_mib_msg->presence_bitmask = 0;

    if (rtInitContext (&ctxt) != 0) 
    {
        printf("ASN context initialisation failed \n");
        return FAILURE;
    }

    pu_setBuffer (&ctxt, encoded_mib_msg, encoded_mib_len, FALSE);

    stat = asn1PD_BCCH_BCH_Message(&ctxt, &asn_decoded_mib);
    if (stat != RT_OK)
    {
        printf("Can't decode MIB Error: %d", stat);
        return FAILURE;
    }
    ret_val =  convert_mib (&asn_decoded_mib, &(decoded_mib_msg->mib_info));
    if (ret_val == SUCCESS)
    {
        decoded_mib_msg->presence_bitmask |= MIB_PRESENCE_FLAG;
    }
    asn1Print_BCCH_BCH_Message("BCCH_BCH_Message", &asn_decoded_mib);

    rtFreeContext(&ctxt);
    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: sib_and_si_decoder
 *
 *   DESCRIPTION:
 *       This is interface function for sib decoding
 *       It converts encoded  sib data into user sib_messages_t
 *       structure.
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data is invalid 
 *
 *******************************************************************************/
U8 sib_and_si_decoder 
(
 U8                *encoded_sib_or_si_msg,
 sib_messages_t    *decoded_sibs_msg,
 U16               encoded_sib_or_si_len
 )
{
    BCCH_DL_SCH_Message    asn_decoded_sibs; 
    OSCTXT                 ctxt;
    S32                    stat              = 0;
    U8          ret_val           = 0;

    decoded_sibs_msg->presence_bitmask = 0;

    if (rtInitContext (&ctxt) != 0) 
    {
        printf("ASN context initialisation failed \n");
        return FAILURE;
    }

    pu_setBuffer (&ctxt, encoded_sib_or_si_msg, encoded_sib_or_si_len, FALSE);

    stat =  asn1PD_BCCH_DL_SCH_Message(&ctxt, &asn_decoded_sibs);
    if (stat != RT_OK)
    {
        printf("Can't decode SIB Error: %d", stat);
        return  FAILURE;
    }
    ret_val = (U8)(check_and_decode_sibs(&asn_decoded_sibs, decoded_sibs_msg));
    asn1Print_BCCH_DL_SCH_Message("BCCH_DL_SCH_Message", &asn_decoded_sibs);

    rtFreeContext(&ctxt);
    return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: sib_and_si_mem_free
 *
 *   DESCRIPTION:
 *       This is interface function for freeing up
 *       any allocated dynamic memory for structure 
 *       sib_messages_t
 *
 *   RETURNS:
 *       SUCCESS - if all data are valid
 *       FAILURE - if data is invalid 
 *
 *******************************************************************************/
U8 sib_and_si_mem_free 
(
 sib_messages_t    *decoded_sibs_msg
)
{
    if ( decoded_sibs_msg == NULL )
    {
        printf("Received Pointer is NULL");
        return  FAILURE;
    }
    if ( (decoded_sibs_msg->presence_bitmask & SIB_TYPE_1_PRESENCE_FLAG) && 
            (decoded_sibs_msg->sib_type_1_Info.presence_bitmask &
             SIB_TYPE_1_NON_CRITICAL_EXT_PRESENT_FLAG ) &&
            (decoded_sibs_msg->sib_type_1_Info.non_critical_ext_v890.
             presence_bitmask & SIB_TYPE_1_LATE_NON_CRITICAL_EXT_PRESENT_FLAG))

    {
        if ( decoded_sibs_msg->sib_type_1_Info.non_critical_ext_v890.
            late_noncritical_ext.data != NULL)
        {
            free (decoded_sibs_msg->sib_type_1_Info.non_critical_ext_v890.
                    late_noncritical_ext.data);
        }

    }
    if ((decoded_sibs_msg->presence_bitmask & SIB_TYPE_2_PRESENCE_FLAG) &&
            (decoded_sibs_msg->sib_type_2_Info.presence_bitmask &
             SIB_TYPE_2_LATE_NON_CRITICAL_EXT_PRESENT_FLAG))
    {
        if ( decoded_sibs_msg->sib_type_2_Info.late_noncritical_ext.
            data != NULL)
        {
            free (decoded_sibs_msg->sib_type_2_Info.
                    late_noncritical_ext.data);
        }
    }
    if ((decoded_sibs_msg->presence_bitmask & SIB_TYPE_3_PRESENCE_FLAG) &&
            (decoded_sibs_msg->sib_type_3_Info.presence_bitmask &
             SIB_TYPE_3_LATE_NON_CRITICAL_EXT_PRESENT_FLAG))
    {
        if ( decoded_sibs_msg->sib_type_3_Info.late_noncritical_ext.
            data != NULL)
        {
            free (decoded_sibs_msg->sib_type_3_Info.
                    late_noncritical_ext.data);
        }
    }
    if ((decoded_sibs_msg->presence_bitmask & SIB_TYPE_4_PRESENCE_FLAG) &&
            (decoded_sibs_msg->sib_type_4_Info.presence_bitmask &
             SIB_4_LATE_NON_CRITICAL_EXT_PRESENT_FLAG))
    {
        if ( decoded_sibs_msg->sib_type_4_Info.late_noncritical_ext.
            data != NULL)
        {
            free (decoded_sibs_msg->sib_type_4_Info.
                    late_noncritical_ext.data);
        }
    }
    if ((decoded_sibs_msg->presence_bitmask & SIB_TYPE_5_PRESENCE_FLAG) &&
            (decoded_sibs_msg->sib_type_5_Info.presence_bitmask &
             SIB_5_LATE_NON_CRITICAL_EXT_PRESENT_FLAG))
    {
        if ( decoded_sibs_msg->sib_type_5_Info.late_noncritical_ext.
            data != NULL)
        {
            free (decoded_sibs_msg->sib_type_5_Info.
                    late_noncritical_ext.data);
        }
    }

    return SUCCESS;

}
