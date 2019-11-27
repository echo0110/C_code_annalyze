#include "sibParsing.h"
#include "sib_type_defines.h"
#include "sib_decoder_intf.h"
#include "rtsrc/rtPrint.h"
#include "rtxsrc/rtxCommon.h"

void print_mib( mib_info_t *mibInfo )
{
    rtxPrintOpenBrace("BCCH_BCH_Message");
    rtxPrintIndent ();
    printf ("dl_Bandwidth = %d\n", mibInfo->dl_band_width);
    rtxPrintOpenBrace ("phich_Config");
    rtxPrintIndent ();
    printf ("phich_Duration = %d\n", mibInfo->phich_config.phich_duration);
    rtxPrintIndent ();
    printf ("phich_Resource = %d\n", mibInfo->phich_config.phich_resource);
    rtxPrintCloseBrace ();
    rtxPrintIndent ();
    rtPrintBitStrBraceText ("systemFrameNumber", FRAME_NO_OCTET_SIZE*8,  mibInfo->systemFrameNo);
    rtxPrintIndent ();
    rtPrintBitStrBraceText ("spare", SPARE_OCTET_SIZE*8,  mibInfo->spare);

}

void print_sib1( sib_type_1_Info_t *sib1Info )
{
    S32     i;
    S32     j;
    char namebuf[512];
    char numbuf[32];

    /* cell_access_related_info */
    rtxPrintOpenBrace("\n\nsystemInformationBlockType1");
    rtxPrintOpenBrace("cellAccessRelatedInfo");
    for( i = 0; i < (sib1Info->cell_access_related_info.plmn_Id_info_list.count); i++)
    {
        rtxUIntToCharStr (i, numbuf, sizeof(numbuf), 0);
        rtxStrJoin (namebuf, sizeof(namebuf), "plmn_IdentityList", "[", numbuf, "]", 0);
        rtxPrintOpenBrace(namebuf);

        rtxPrintOpenBrace("plmn_Identity");
        if((sib1Info->cell_access_related_info.plmn_Id_info_list.plmn_identity_info[i].plmn_identity.presence_bitmask & PLMN_IDENTITY_MCC_PRESENCE_FLAG) == PLMN_IDENTITY_MCC_PRESENCE_FLAG)
        {
            for(j = 0; j < MCC_OCTET_SIZE; j++)
            {
                rtxUIntToCharStr (i, numbuf, sizeof(numbuf), 0);
                rtxStrJoin (namebuf, sizeof(namebuf), "mcc", "[", numbuf, "]", 0);
                rtxPrintIndent ();
                rtxPrintUnsigned (namebuf, sib1Info->cell_access_related_info.plmn_Id_info_list.plmn_identity_info[i].plmn_identity.mcc[j]);
            }
        }
        for( j = 0; j < (sib1Info->cell_access_related_info.plmn_Id_info_list.plmn_identity_info[i].plmn_identity.mnc.count); j++)
        {
            rtxUIntToCharStr (j, numbuf, sizeof(numbuf), 0);
            rtxStrJoin (namebuf, sizeof(namebuf), "mnc", "[", numbuf, "]", 0);
            rtxPrintIndent ();
            rtxPrintUnsigned (namebuf, sib1Info->cell_access_related_info.plmn_Id_info_list.plmn_identity_info[i].plmn_identity.mnc.mnc[j]);
        }
        rtxPrintCloseBrace ();
        rtxPrintIndent ();
        printf ("cell_res_for_operator_use = ");
        printf("%d \n", sib1Info->cell_access_related_info.plmn_Id_info_list.plmn_identity_info[i].cell_res_for_operator_use);
        rtxPrintCloseBrace ();
    }
    rtxPrintIndent ();
    rtPrintBitStrBraceText ("trackingAreaCode", TAC_OCTET_SIZE*8, sib1Info->cell_access_related_info.tac);
    rtxPrintIndent ();
    rtPrintBitStrBraceText ("cellIdentity", CELL_ID_OCTET_SIZE*8, sib1Info->cell_access_related_info.cell_Id);
    rtxPrintIndent ();
    printf("cell_barred = %d\n",sib1Info->cell_access_related_info.cell_barred);
    rtxPrintIndent ();
    printf("intra_freq_reselection = %d\n",sib1Info->cell_access_related_info.intra_freq_reselection);
    rtxPrintIndent ();
    printf("csg_indication = %d\n",sib1Info->cell_access_related_info.csg_indication);
    if((sib1Info->cell_access_related_info.presence_bitmask & CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG) == CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG) 
    {
        rtxPrintIndent ();
        rtPrintBitStrBraceText ("csg_Identity", CSG_ID_OCTET_SIZE*8, sib1Info->cell_access_related_info.csg_identity);
    }
    rtxPrintCloseBrace ();
    /* end of cell_access_related_info */

    /*cell_selection_info */
    rtxPrintOpenBrace("cellSelectionInfo");
    rtxPrintIndent ();
    rtxPrintInteger ("q_RxLevMin",sib1Info->cell_selection_Info.q_rx_lev_min);
    if ((sib1Info->cell_selection_Info.presence_bitmask & CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG) == CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG)
    {
        rtxPrintIndent ();
        rtxPrintInteger ("q_RxLevMinOffset",sib1Info->cell_selection_Info.q_rx_lev_min_offset);
    }
    rtxPrintCloseBrace ();
    /* end of cell_selection_info */

    if ((sib1Info->presence_bitmask & SIB_TYPE_1_P_MAX_PRESENCE_FLAG) == SIB_TYPE_1_P_MAX_PRESENCE_FLAG)
    {
        rtxPrintIndent ();
        rtxPrintInteger ("p_Max",sib1Info->p_max);
    }
    rtxPrintIndent ();
    rtxPrintUnsigned ("freqBandIndicator", sib1Info->freq_band_indicator);
    /* scheduling_info_list */
    for( i = 0; i <(sib1Info->scheduling_info_list.count); i++)
    {
        rtxUIntToCharStr (i, numbuf, sizeof(numbuf), 0);
        rtxStrJoin (namebuf, sizeof(namebuf),"schedulingInfoList", "[", numbuf, "]", 0);
        rtxPrintOpenBrace (namebuf);
        rtxPrintIndent ();
        printf("si_periodicity = %d\n ",sib1Info->scheduling_info_list.sheduling_info[i].si_periodicity);
        for( j = 0; j < (sib1Info->scheduling_info_list.sheduling_info[i].sib_mapping_info.count); j++)
        {
            rtxUIntToCharStr (j, numbuf, sizeof(numbuf), 0);
            rtxStrJoin (namebuf, sizeof(namebuf),"sib_MappingInfo" , "[", numbuf, "]", 0);
            rtxPrintIndent ();
            printf("%s = %d\n",namebuf,sib1Info->scheduling_info_list.sheduling_info[i].sib_mapping_info.sib_type[j]);
        }
        rtxPrintCloseBrace ();
    }
    /* end of scheduling_info_list */

    /* tdd_config */
    if ((sib1Info->presence_bitmask & SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG) == SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG )
    {
        rtxPrintOpenBrace ("tdd_Config");
        rtxPrintIndent ();
        printf("subframeAssignment = %d\n ",sib1Info->tdd_config.sub_frame_assignment);
        rtxPrintIndent ();
        printf("specialSubframePatterns = %d\n ",sib1Info->tdd_config.special_sub_frame_pattern);
        rtxPrintCloseBrace ();
    }
    /* end of tdd_config */
    rtxPrintIndent ();
    printf("si_WindowLength = %d\n ",sib1Info->si_window_length);
    rtxPrintIndent ();
    rtxPrintUnsigned ("systemInfoValueTag", sib1Info->si_value_tag);

    /* sib_type_1_v890 */
    if ((sib1Info->presence_bitmask & SIB_TYPE_1_NON_CRITICAL_EXT_PRESENT_FLAG) == SIB_TYPE_1_NON_CRITICAL_EXT_PRESENT_FLAG )
    {
        rtxPrintOpenBrace ("nonCriticalExtension");
        if ((sib1Info->non_critical_ext_v890.presence_bitmask & SIB_TYPE_1_LATE_NON_CRITICAL_EXT_PRESENT_FLAG)  == SIB_TYPE_1_LATE_NON_CRITICAL_EXT_PRESENT_FLAG )
        {
            rtxPrintIndent ();
            rtxPrintHexStr ("lateNonCriticalExtension",
                    sib1Info->non_critical_ext_v890.late_noncritical_ext.no_octet, 
                    sib1Info->non_critical_ext_v890.late_noncritical_ext.data);
        }
        if ((sib1Info->non_critical_ext_v890.presence_bitmask & SIB_TYPE_1_NON_CRITICAL_EXT_V890_PRESENT_FLAG)  == SIB_TYPE_1_NON_CRITICAL_EXT_V890_PRESENT_FLAG  )
        {
            rtxPrintOpenBrace ("nonCriticalExtension");
            if ((sib1Info->non_critical_ext_v890.non_critical_ext_v920.presence_bitmask & SIB_TYPE_1_IMS_EMERGENCYSUPPORT_R9_PRESENT_FLAG)  == SIB_TYPE_1_IMS_EMERGENCYSUPPORT_R9_PRESENT_FLAG )
            {
                rtxPrintIndent ();
                rtxPrintUnsigned ("ims_EmergencySupport_r9", sib1Info->non_critical_ext_v890.non_critical_ext_v920.ims_emergencysupport_r9);
            }
            if ((sib1Info->non_critical_ext_v890.non_critical_ext_v920.presence_bitmask & SIB_TYPE_1_CELL_SELECTION_INFO_PRESENT_FLAG) == SIB_TYPE_1_CELL_SELECTION_INFO_PRESENT_FLAG)
            {
                rtxPrintOpenBrace ("cellSelectionInfo_v920");
                rtxPrintIndent ();
                rtxPrintInteger ("q_QualMin_r9", sib1Info->non_critical_ext_v890.non_critical_ext_v920.cell_selection_info_v920.q_qual_min_r9);
                if ((sib1Info->non_critical_ext_v890.non_critical_ext_v920.cell_selection_info_v920.presence_bitmask & CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG)  == CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG)
                {
                    rtxPrintIndent ();
                    rtxPrintUnsigned ("q_QualMinOffset_r9", sib1Info->non_critical_ext_v890.non_critical_ext_v920.cell_selection_info_v920.q_qual_min_offset_r9);
                }
                rtxPrintCloseBrace ();
            }
            rtxPrintCloseBrace ();
        }
        if ((sib1Info->non_critical_ext_v890.non_critical_ext_v920.presence_bitmask & SIB_TYPE_1_NONCRITICAL_EXT_PRESENT_FLAG) == SIB_TYPE_1_NONCRITICAL_EXT_PRESENT_FLAG)
        {
            rtxPrintOpenBrace ("nonCriticalExtension");
            rtxPrintCloseBrace ();
        }
        rtxPrintCloseBrace ();
    }
    rtxPrintCloseBrace ();
    /* end of  sib_type_1_v890 */
}

void print_sib2( sib_type_2_Info_t *sib2Info )
{
    S32     i;
    char namebuf[512];
    char numbuf[32];

    rtxPrintOpenBrace ("sib2");
    if ((sib2Info->presence_bitmask & SIB2_ACCESS_BARR_INFO_PRESENCE_FLAG) == SIB2_ACCESS_BARR_INFO_PRESENCE_FLAG )
    {
        rtxPrintOpenBrace ("ac_BarringInfo");
        rtxPrintIndent ();
        rtxPrintBoolean ("ac_BarringForEmergency", sib2Info->access_barring_info.acBarringForEmergency);
        if ((sib2Info->access_barring_info.presence_bitmask & AC_BARR_INFO_MO_SIG_PRESENCE_FLAG) == AC_BARR_INFO_MO_SIG_PRESENCE_FLAG)
        {
            rtxPrintOpenBrace ("ac_BarringForMO_Signalling");
            rtxPrintIndent ();
            printf ("ac_BarringFactor = %d\n", sib2Info->access_barring_info.ac_barring_for_mo_signalling.ac_barring_factor);
            rtxPrintIndent ();
            printf ("ac_BarringTime = %d\n", sib2Info->access_barring_info.ac_barring_for_mo_signalling.ac_barring_time);
            rtxPrintIndent ();
            rtPrintBitStrBraceText ("ac_BarringForSpecialAC", 5,&(sib2Info->access_barring_info.ac_barring_for_mo_signalling.ac_barring_for_special_ac) );
            rtxPrintCloseBrace ();
        }
        if ((sib2Info->access_barring_info.presence_bitmask & AC_BARR_INFO_MO_DATA_PRESENCE_FLAG) == AC_BARR_INFO_MO_DATA_PRESENCE_FLAG)
        {
            rtxPrintOpenBrace ("ac_BarringForMO_Data");
            rtxPrintIndent ();
            printf ("ac_BarringFactor = %d\n", sib2Info->access_barring_info.ac_barring_for_mo_data.ac_barring_factor);
            rtxPrintIndent ();
            printf ("ac_BarringTime = %d\n", sib2Info->access_barring_info.ac_barring_for_mo_data.ac_barring_time);
            rtxPrintIndent ();
            rtPrintBitStrBraceText ("ac_BarringForSpecialAC", 5,&(sib2Info->access_barring_info.ac_barring_for_mo_data.ac_barring_for_special_ac) );
            rtxPrintCloseBrace ();
        }
        rtxPrintCloseBrace ();
    }
    rtxPrintOpenBrace ("radioResourceConfigCommon");
    rtxPrintOpenBrace ("rach_ConfigCommon");
    rtxPrintOpenBrace ("preambleInfo");
    rtxPrintIndent ();
    printf ("numberOfRA_Preambles = %d\n", sib2Info->radio_resource_config_common_sib.rach_config_common.preamble_info.ra_preamble_count);
    if ((sib2Info->radio_resource_config_common_sib.rach_config_common.preamble_info.presence_bitmask & PREAMBLE_INFO_GROUP_A_CONFIG_PRESENCE_FLAG) == PREAMBLE_INFO_GROUP_A_CONFIG_PRESENCE_FLAG )
    {
        rtxPrintOpenBrace ("preamblesGroupAConfig");
        rtxPrintIndent ();
        printf ("sizeOfRA_PreamblesGroupA = %d\n", sib2Info->radio_resource_config_common_sib.rach_config_common.preamble_info.preambles_group_a_config.ra_preambles_group_a_size );
        rtxPrintIndent ();
        printf ("messageSizeGroupA = %d\n", sib2Info->radio_resource_config_common_sib.rach_config_common.preamble_info.preambles_group_a_config.group_a_msg_size );
        rtxPrintIndent ();
        printf ("messagePowerOffsetGroupB = %d\n", sib2Info->radio_resource_config_common_sib.rach_config_common.preamble_info.preambles_group_a_config.group_b_msg_power_offset );
        rtxPrintCloseBrace ();
    }
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("powerRampingParameters");
    rtxPrintIndent ();
    printf ("powerRampingStep = %d\n", sib2Info->radio_resource_config_common_sib.rach_config_common.power_ramping_params.power_ramping_step );
    rtxPrintIndent ();
    printf ("preambleInitialReceivedTargetPower = %d\n", sib2Info->radio_resource_config_common_sib.rach_config_common.power_ramping_params.preamble_init_rec_target_pow );
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("ra_SupervisionInfo");
    rtxPrintIndent ();
    printf ("preambleTransMax = %d\n", sib2Info->radio_resource_config_common_sib.rach_config_common.ra_supervision_info.preamble_trans_max );
    rtxPrintIndent ();
    printf ("ra_ResponseWindowSize = %d\n", sib2Info->radio_resource_config_common_sib.rach_config_common.ra_supervision_info.ra_resp_win_size );
    rtxPrintIndent ();
    printf ("mac_ContentionResolutionTimer = %d\n", sib2Info->radio_resource_config_common_sib.rach_config_common.ra_supervision_info.mac_cont_resol_timer  );
    rtxPrintCloseBrace ();
    rtxPrintIndent ();
    rtxPrintUnsigned ("maxHARQ_Msg3Tx", sib2Info->radio_resource_config_common_sib.rach_config_common.max_harq_msg_3_tx);
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("bcch_Config");
    rtxPrintIndent ();
    printf ("modificationPeriodCoeff = %d\n", sib2Info->radio_resource_config_common_sib.bcch_config.mod_period_coeff  );
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("pcch_Config");
    rtxPrintIndent ();
    printf ("defaultPagingCycle = %d\n", sib2Info->radio_resource_config_common_sib.pcch_config.default_paging_cycle  );
    rtxPrintIndent ();
    printf ("nB = %d\n", sib2Info->radio_resource_config_common_sib.pcch_config.nb  );
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("prach_Config");
    rtxPrintIndent ();
    rtxPrintUnsigned ("rootSequenceIndex",  sib2Info->radio_resource_config_common_sib.prach_config_sib.root_seq_index );
    rtxPrintOpenBrace ("prach_ConfigInfo");
    rtxPrintIndent ();
    rtxPrintUnsigned ("prach_ConfigIndex", sib2Info->radio_resource_config_common_sib.prach_config_sib.prach_config_info.prach_config_index );
    rtxPrintIndent ();
    rtxPrintBoolean ("highSpeedFlag", sib2Info->radio_resource_config_common_sib.prach_config_sib.prach_config_info.high_speed_flag);
    rtxPrintIndent ();
    rtxPrintUnsigned ("zeroCorrelationZoneConfig", sib2Info->radio_resource_config_common_sib.prach_config_sib.prach_config_info.zero_cor_zone_config);
    rtxPrintIndent ();
    rtxPrintUnsigned ("prach_FreqOffset", sib2Info->radio_resource_config_common_sib.prach_config_sib.prach_config_info.prach_freq_offset);
    rtxPrintCloseBrace ();
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("pdsch_ConfigCommon");
    rtxPrintIndent ();
    rtxPrintInteger ("referenceSignalPower", sib2Info->radio_resource_config_common_sib.pdsch_config_common.ref_signal_power);
    rtxPrintIndent ();
    rtxPrintUnsigned ("p_b", sib2Info->radio_resource_config_common_sib.pdsch_config_common.pb);
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("pusch_ConfigCommon");
    rtxPrintOpenBrace ("pusch_ConfigBasic");
    rtxPrintIndent ();
    rtxPrintUnsigned ("n_SB", sib2Info->radio_resource_config_common_sib.pusch_config_common.pusch_config_basic.nsb);
    rtxPrintIndent ();
    printf ("hoppingMode = %d\n", sib2Info->radio_resource_config_common_sib.pusch_config_common.pusch_config_basic.hopping_mode  );
    rtxPrintIndent ();
    rtxPrintUnsigned ("pusch_HoppingOffset", sib2Info->radio_resource_config_common_sib.pusch_config_common.pusch_config_basic.pusch_hopping_offset);
    rtxPrintIndent ();
    rtxPrintBoolean ("enable64QAM", sib2Info->radio_resource_config_common_sib.pusch_config_common.pusch_config_basic.enable_64_qam);
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("ul_ReferenceSignalsPUSCH");
    rtxPrintIndent ();
    rtxPrintBoolean ("groupHoppingEnabled", sib2Info->radio_resource_config_common_sib.pusch_config_common.ul_ref_signals_pusch.group_hopping_enabled );
    rtxPrintIndent ();
    rtxPrintUnsigned ("groupAssignmentPUSCH", sib2Info->radio_resource_config_common_sib.pusch_config_common.ul_ref_signals_pusch.group_assign_pusch);
    rtxPrintIndent ();
    rtxPrintBoolean ("sequenceHoppingEnabled", sib2Info->radio_resource_config_common_sib.pusch_config_common.ul_ref_signals_pusch.seq_hopping_enabled);
    rtxPrintIndent ();
    rtxPrintUnsigned ("cyclicShift", sib2Info->radio_resource_config_common_sib.pusch_config_common.ul_ref_signals_pusch.cyclic_shift) ;
    rtxPrintCloseBrace ();
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("pucch_ConfigCommon");
    rtxPrintIndent ();
    printf ("deltaPUCCH_Shift = %d\n", sib2Info->radio_resource_config_common_sib.pucch_config_common.delta_pucch_shift  );
    rtxPrintIndent ();
    rtxPrintUnsigned ("nRB_CQI", sib2Info->radio_resource_config_common_sib.pucch_config_common.n_rb_cqi);
    rtxPrintIndent ();
    rtxPrintUnsigned ("nCS_AN", sib2Info->radio_resource_config_common_sib.pucch_config_common.n_cs_an);
    rtxPrintIndent ();
    rtxPrintUnsigned ("n1PUCCH_AN", sib2Info->radio_resource_config_common_sib.pucch_config_common.n_one_pucch_an);
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("soundingRS_UL_ConfigCommon");
    if ((sib2Info->radio_resource_config_common_sib.sounding_rs_ul_config_common.presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG) == SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG)
    {
        rtxPrintOpenBrace ("setup");
        rtxPrintIndent ();
        printf ("srs_BandwidthConfig = %d\n", sib2Info->radio_resource_config_common_sib.sounding_rs_ul_config_common.setup.srs_bw_config );
        rtxPrintIndent ();
        printf ("srs_SubframeConfig = %d\n", sib2Info->radio_resource_config_common_sib.sounding_rs_ul_config_common.setup.srs_subframe_config );
        rtxPrintIndent ();
        rtxPrintBoolean ("ackNackSRS_SimultaneousTransmission", sib2Info->radio_resource_config_common_sib.sounding_rs_ul_config_common.setup.ack_nack_srs_simul_trans);
        if ((sib2Info->radio_resource_config_common_sib.sounding_rs_ul_config_common.setup.presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG) == SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG)
        {
            rtxPrintIndent ();
            printf ("srs_MaxUpPts = %d\n", sib2Info->radio_resource_config_common_sib.sounding_rs_ul_config_common.setup.srs_max_up_pts );
        }
        rtxPrintCloseBrace ();
    }
    else if ((sib2Info->radio_resource_config_common_sib.sounding_rs_ul_config_common.presence_bitmask & SOUNDING_RS_UL_CONFIG_COMMON_RELEASE_PRESENCE_FLAG) == SOUNDING_RS_UL_CONFIG_COMMON_RELEASE_PRESENCE_FLAG)
    {
        rtxPrintIndent();
        rtxPrintNull ("release");
    }
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("uplinkPowerControlCommon");

    rtxPrintIndent ();
    rtxPrintInteger ("p0_NominalPUSCH", sib2Info->radio_resource_config_common_sib.uplink_power_control_common.p_zero_nominal_Pusch);
    rtxPrintIndent ();
    printf ("alpha = %d\n", sib2Info->radio_resource_config_common_sib.uplink_power_control_common.alpha );
    rtxPrintIndent ();
    rtxPrintInteger ("p0_NominalPUCCH", sib2Info->radio_resource_config_common_sib.uplink_power_control_common.p_zero_nominal_pucch);
    rtxPrintOpenBrace ("deltaFList_PUCCH");
    rtxPrintIndent ();
    printf ("deltaF_PUCCH_Format1 = %d\n", sib2Info->radio_resource_config_common_sib.uplink_power_control_common.delta_f_list_pucch.delta_f_pucch_format_one);
    rtxPrintIndent ();
    printf ("deltaF_PUCCH_Format1b = %d\n", sib2Info->radio_resource_config_common_sib.uplink_power_control_common.delta_f_list_pucch.delta_f_pucch_format_one_b);
    rtxPrintIndent ();
    printf ("deltaF_PUCCH_Format2 = %d\n", sib2Info->radio_resource_config_common_sib.uplink_power_control_common.delta_f_list_pucch.delta_f_pucch_format_two);
    rtxPrintIndent ();
    printf ("deltaF_PUCCH_Format2a = %d\n", sib2Info->radio_resource_config_common_sib.uplink_power_control_common.delta_f_list_pucch.delta_f_pucch_format_two_a);
    rtxPrintIndent ();
    printf ("deltaF_PUCCH_Format2b = %d\n", sib2Info->radio_resource_config_common_sib.uplink_power_control_common.delta_f_list_pucch.delta_f_pucch_format_two_b);
    rtxPrintCloseBrace ();
    rtxPrintIndent ();
    rtxPrintInteger ("deltaPreambleMsg3", sib2Info->radio_resource_config_common_sib.uplink_power_control_common.delta_preamble_msg_three );
    rtxPrintCloseBrace ();
    rtxPrintIndent ();
    printf ("ul_CyclicPrefixLength = %d\n", sib2Info->radio_resource_config_common_sib.ul_cyc_prefix_len );
    rtxPrintCloseBrace ();  
    rtxPrintOpenBrace ("ue_TimersAndConstants");
    rtxPrintIndent ();
    printf ("t300 = %d\n", sib2Info->ue_timers_and_constants.timer_300 );
    rtxPrintIndent ();
    printf ("t301 = %d\n", sib2Info->ue_timers_and_constants.timer_301);
    rtxPrintIndent ();
    printf ("t310 = %d\n", sib2Info->ue_timers_and_constants.timer_310);
    rtxPrintIndent ();
    printf ("n310 = %d\n", sib2Info->ue_timers_and_constants.timer_n310);
    rtxPrintIndent ();
    printf ("t311 = %d\n", sib2Info->ue_timers_and_constants.timer_311);
    rtxPrintIndent ();
    printf ("n311 = %d\n", sib2Info->ue_timers_and_constants.timer_n311);
    rtxPrintCloseBrace ();  
    rtxPrintOpenBrace ("freqInfo");
    if ((sib2Info->freq_info.presence_bitmask & FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG) == FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG)
    {
        rtxPrintIndent ();
        rtxPrintUnsigned ("ul_CarrierFreq", sib2Info->freq_info.ul_carrier_freq);
    }
    if ((sib2Info->freq_info.presence_bitmask & FREQ_INFO_UL_BW_PRESENCE_FLAG) == FREQ_INFO_UL_BW_PRESENCE_FLAG )
    {
        rtxPrintIndent ();
        printf ("ul_Bandwidth = %d\n", sib2Info->freq_info.ul_bandwidth);
    }
    rtxPrintIndent ();
    rtxPrintUnsigned ("additionalSpectrumEmission", sib2Info->freq_info.add_spectrum_emission );
    rtxPrintCloseBrace ();
    if ((sib2Info->presence_bitmask & SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG)  == SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG )
    {
        for (i = 0; i < sib2Info->mbsfn_subframe_config_list.count; i++) 
        {
            rtxUIntToCharStr (i, numbuf, sizeof(numbuf), 0);
            rtxStrJoin (namebuf, sizeof(namebuf), "mbsfn_SubframeConfigList", "[", numbuf, "]", 0);
            rtxPrintOpenBrace (namebuf);
            rtxPrintIndent ();
            printf ("radioframeAllocationPeriod = %d\n", sib2Info->mbsfn_subframe_config_list.mbsfn_subframe_config[i].radio_frame_alloc_period);
            rtxPrintIndent ();
            rtxPrintUnsigned ("radioframeAllocationOffset", sib2Info->mbsfn_subframe_config_list.mbsfn_subframe_config[i].radio_frame_alloc_offset);
            rtxPrintOpenBrace ("subframeAllocation");
            if ((sib2Info->mbsfn_subframe_config_list.mbsfn_subframe_config[i].subframe_allocation.presence_bitmask & SUBFRAME_ALLOCATION_ONE_FRAME) == SUBFRAME_ALLOCATION_ONE_FRAME) 
            {
                rtxPrintIndent ();
                rtPrintBitStrBraceText ("oneFrame", SUB_FRAME_ALLOC_ONE_FRAME_OCTET_SIZE*8, sib2Info->mbsfn_subframe_config_list.mbsfn_subframe_config[i].subframe_allocation.one_frame);

            }
            if ((sib2Info->mbsfn_subframe_config_list.mbsfn_subframe_config[i].subframe_allocation.presence_bitmask & SUBFRAME_ALLOCATION_FOUR_FRAMES) == SUBFRAME_ALLOCATION_FOUR_FRAMES)
            {
                rtxPrintIndent ();
                rtPrintBitStrBraceText ("fourFrames", SUB_FRAME_ALLOC_FOUR_FRAME_OCTET_SIZE*8, sib2Info->mbsfn_subframe_config_list.mbsfn_subframe_config[i].subframe_allocation.four_frames);

            }
            rtxPrintCloseBrace ();
            rtxPrintCloseBrace ();
        }
    }
    rtxPrintIndent ();
    printf ("timeAlignmentTimerCommon = %d\n", sib2Info->time_align_timer);
    if ((sib2Info->presence_bitmask & SIB_TYPE_2_LATE_NON_CRITICAL_EXT_PRESENT_FLAG) == SIB_TYPE_2_LATE_NON_CRITICAL_EXT_PRESENT_FLAG)
    {
        rtxPrintIndent ();
        rtxPrintHexStr ("lateNonCriticalExtension", sib2Info->late_noncritical_ext.no_octet, sib2Info->late_noncritical_ext.data);
    }                  
    if ((sib2Info->presence_bitmask & SIB_TYPE_2_SSAC_BARRING_VOICE_R9_PRESENT_FLAG) == SIB_TYPE_2_SSAC_BARRING_VOICE_R9_PRESENT_FLAG)
    {
        rtxPrintOpenBrace ("ssac_BarringForMMTEL_Voice_r9");
        rtxPrintIndent ();
        printf ("ac_BarringFactor = %d\n", sib2Info->ssac_barring_for_mmtel_voice_r9.ac_barring_factor);
        rtxPrintIndent ();
        printf ("ac_BarringTime= %d\n", sib2Info->ssac_barring_for_mmtel_voice_r9.ac_barring_time);
        rtxPrintIndent ();
        rtPrintBitStrBraceText ("ac_BarringForSpecialAC", 5,&(sib2Info->ssac_barring_for_mmtel_voice_r9.ac_barring_for_special_ac) );
        rtxPrintCloseBrace ();
    }
    if ((sib2Info->presence_bitmask & SIB_TYPE_2_SSAC_BARRING_VIDEO_R9_PRESENT_FLAG) == SIB_TYPE_2_SSAC_BARRING_VIDEO_R9_PRESENT_FLAG )
    {
        rtxPrintOpenBrace ("ssac_BarringForMMTEL_Video_r9");
        rtxPrintIndent ();
        printf ("ac_BarringFactor = %d\n", sib2Info->ssac_barring_for_mmtel_video_r9.ac_barring_factor);
        rtxPrintIndent ();
        printf ("ac_BarringTime= %d\n", sib2Info->ssac_barring_for_mmtel_video_r9.ac_barring_time);
        rtxPrintIndent ();
        rtPrintBitStrBraceText ("ac_BarringForSpecialAC", 5, &(sib2Info->ssac_barring_for_mmtel_video_r9.ac_barring_for_special_ac) );
        rtxPrintCloseBrace ();
    }
    rtxPrintCloseBrace ();
}

void print_sib3( sib_type_3_Info_t *sib3Info )
{

    rtxPrintOpenBrace ("sib3");
    rtxPrintOpenBrace ("cellReselectionInfoCommon");
    rtxPrintIndent ();
    printf ("q_Hyst = %d\n", sib3Info->timecell_reselect_info_comm.q_hyst);
    if ((sib3Info->timecell_reselect_info_comm.presence_bitmask & CELL_RESELECT_SPEED_DEPEND_RESELECT_PRESENCE_FLAG) == CELL_RESELECT_SPEED_DEPEND_RESELECT_PRESENCE_FLAG)
    {
        rtxPrintOpenBrace ("speedStateReselectionPars");
        rtxPrintOpenBrace ("mobilityStateParameters");
        rtxPrintIndent ();
        printf ("t_Evaluation = %d\n", sib3Info->timecell_reselect_info_comm.speed_depend_reselect.mobility_state_params.t_evaluation);
        rtxPrintIndent ();
        printf ("t_HystNormal = %d\n", sib3Info->timecell_reselect_info_comm.speed_depend_reselect.mobility_state_params.t_hyst_normal);
        rtxPrintIndent ();
        rtxPrintUnsigned ("n_CellChangeMedium", sib3Info->timecell_reselect_info_comm.speed_depend_reselect.mobility_state_params.n_cell_change_medium);
        rtxPrintIndent ();
        rtxPrintUnsigned ("n_CellChangeHigh", sib3Info->timecell_reselect_info_comm.speed_depend_reselect.mobility_state_params.n_cell_change_high);
        rtxPrintCloseBrace ();
        rtxPrintOpenBrace ("q_HystSF");
        rtxPrintIndent ();
        printf ("sf_Medium = %d\n", sib3Info->timecell_reselect_info_comm.speed_depend_reselect.speed_depend_scaling_param_hyst.q_hyst_sf_medium);
        rtxPrintIndent ();
        printf ("sf_High = %d\n", sib3Info->timecell_reselect_info_comm.speed_depend_reselect.speed_depend_scaling_param_hyst.q_hyst_sf_high);
        rtxPrintCloseBrace ();
        rtxPrintCloseBrace ();
        rtxPrintCloseBrace ();
    }
    rtxPrintOpenBrace ("cellReselectionServingFreqInfo");
    if ((sib3Info->cell_reselect_serv_freq_info.presence_bitmask & CELL_RESELECT_NON_INTRA_SEARCH_PRESENCE_FLAG) == CELL_RESELECT_NON_INTRA_SEARCH_PRESENCE_FLAG )
    {
        rtxPrintIndent ();
        rtxPrintUnsigned ("s_NonIntraSearch", sib3Info->cell_reselect_serv_freq_info.s_non_intra_search);
    }
    rtxPrintIndent ();
    rtxPrintUnsigned ("threshServingLow", sib3Info->cell_reselect_serv_freq_info.thresh_serving_low);
    rtxPrintIndent ();
    rtxPrintUnsigned ("cellReselectionPriority", sib3Info->cell_reselect_serv_freq_info.cell_reselect_priority);
    rtxPrintCloseBrace ();
    rtxPrintOpenBrace ("intraFreqCellReselectionInfo");
    rtxPrintIndent ();
    rtxPrintInteger ("q_RxLevMin", sib3Info->intra_freq_cell_reselect_info.q_rx_lev_min);
    if ((sib3Info->intra_freq_cell_reselect_info.presence_bitmask & INTRA_FREQ_CELL_RESELECT_P_MAX_PRESENCE_FLAG) == INTRA_FREQ_CELL_RESELECT_P_MAX_PRESENCE_FLAG)
    {
        rtxPrintIndent ();
        rtxPrintInteger ("p_Max", sib3Info->intra_freq_cell_reselect_info.p_max);
    }
    if ((sib3Info->intra_freq_cell_reselect_info.presence_bitmask & INTRA_FREQ_CELL_RESELECT_INTRA_SEARCH_PRESENCE_FLAG) == INTRA_FREQ_CELL_RESELECT_INTRA_SEARCH_PRESENCE_FLAG) 
    {
        rtxPrintIndent ();
        rtxPrintUnsigned ("s_IntraSearch", sib3Info->intra_freq_cell_reselect_info.s_intra_search);
    }
    if ((sib3Info->intra_freq_cell_reselect_info.presence_bitmask & INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRESENCE_FLAG) == INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRESENCE_FLAG) 
    {
        rtxPrintIndent ();
        printf ("allowedMeasBandwidth = %d\n",  sib3Info->intra_freq_cell_reselect_info.measure_bw);
    }
    rtxPrintIndent ();
    rtxPrintBoolean ("presenceAntennaPort1", sib3Info->intra_freq_cell_reselect_info.presence_antenna_port_1);
    rtxPrintIndent ();
    rtPrintBitStrBraceText ("neighCellConfig", 2, &(sib3Info->intra_freq_cell_reselect_info.neigh_cell_config));
    rtxPrintIndent ();
    rtxPrintUnsigned ("t_ReselectionEUTRA", sib3Info->intra_freq_cell_reselect_info.t_reselec_eutra);
    if ((sib3Info->intra_freq_cell_reselect_info.presence_bitmask & INTRA_FREQ_CELL_RESELECT_EUTRAN_SF_PRESENCE_FLAG) == INTRA_FREQ_CELL_RESELECT_EUTRAN_SF_PRESENCE_FLAG) 
    {
        rtxPrintOpenBrace ("t_ReselectionEUTRA_SF");
        rtxPrintIndent ();
        printf ("sf_Medium = %d\n",  sib3Info->intra_freq_cell_reselect_info.t_reselect_eutra_sf.sf_medium);
        rtxPrintIndent ();
        printf ("sf_High = %d\n",  sib3Info->intra_freq_cell_reselect_info.t_reselect_eutra_sf.sf_high);
        rtxPrintCloseBrace ();
    }
    rtxPrintCloseBrace ();
    if ((sib3Info->presence_bitmask & SIB_TYPE_3_LATE_NON_CRITICAL_EXT_PRESENT_FLAG) == SIB_TYPE_3_LATE_NON_CRITICAL_EXT_PRESENT_FLAG) 
    {
        rtxPrintIndent ();
        rtxPrintHexStr ("lateNonCriticalExtension", sib3Info->late_noncritical_ext.no_octet, sib3Info->late_noncritical_ext.data);
    }
    if ((sib3Info->presence_bitmask & INTRA_SEARCH_V920_PRESENCE_FLAG) == INTRA_SEARCH_V920_PRESENCE_FLAG) 
    {
        rtxPrintOpenBrace ("s_IntraSearch_v920");
        rtxPrintIndent ();
        rtxPrintUnsigned ("s_IntraSearchP_r9", sib3Info->s_intra_search_v920.s_intra_search_p_r9);
        rtxPrintIndent ();
        rtxPrintUnsigned ("s_IntraSearchQ_r9", sib3Info->s_intra_search_v920.s_intra_search_q_r9);
        rtxPrintCloseBrace ();
    }
    if ((sib3Info->presence_bitmask & NON_INTRA_SEARCH_V920_PRESENCE_FLAG) == NON_INTRA_SEARCH_V920_PRESENCE_FLAG) 
    {
        rtxPrintOpenBrace ("s_NonIntraSearch_v920");
        rtxPrintIndent ();
        rtxPrintUnsigned ("s_NonIntraSearchP_r9", sib3Info->s_non_intra_search_v920.s_non_intra_search_p_r9);
        rtxPrintIndent ();
        rtxPrintUnsigned ("s_NonIntraSearchQ_r9", sib3Info->s_non_intra_search_v920.s_non_intra_search_q_r9);
        rtxPrintCloseBrace ();
    }
    if ((sib3Info->presence_bitmask & QUALMIN_R9_PRESENCE_FLAG) == QUALMIN_R9_PRESENCE_FLAG) 
    {
        rtxPrintIndent ();
        rtxPrintInteger ("q_QualMin_r9", sib3Info->q_qualmin_r9);
    }
    if ((sib3Info->presence_bitmask & RESELECTION_THRESHOLDQ_R9_PRESENCE_FLAG) == RESELECTION_THRESHOLDQ_R9_PRESENCE_FLAG) 
    {
        rtxPrintIndent ();
        rtxPrintUnsigned ("threshServingLowQ_r9",  sib3Info->reselection_thresholdq_r9);
    }
    rtxPrintCloseBrace ();
}
void print_sib4( sib_type_4_Info_t *sib4Info )
{
    U32    index              = 0;
    char   namebuf[512];
    char numbuf[32];

    rtxPrintOpenBrace ("sib4");
    if ((sib4Info->presence_bitmask & SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG) == SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG)
    {
        for (index=0; index<sib4Info->intra_freq_neigh_cell_list.count; index++)
        {
            rtxUIntToCharStr (index, numbuf, sizeof(numbuf), 0);
            rtxStrJoin (namebuf, sizeof(namebuf), "intraFreqNeighCellList", "[", numbuf, "]", 0);
            rtxPrintOpenBrace (namebuf);
            rtxPrintIndent ();
            rtxPrintUnsigned ("physCellId", sib4Info->intra_freq_neigh_cell_list.neigh_cell[index].phys_cell_id);
            rtxPrintIndent ();
            printf ("q_OffsetCell = %d\n", sib4Info->intra_freq_neigh_cell_list.neigh_cell[index].q_offset_cell);
            rtxPrintCloseBrace ();
        }

    }
    if ((sib4Info->presence_bitmask & SIB_4_BLACK_LIST_LIST_PRESENCE_FLAG) == SIB_4_BLACK_LIST_LIST_PRESENCE_FLAG)
    {
         for (index=0; index<sib4Info->intra_freq_black_listed_cell_list.count; index++)
         {
              rtxUIntToCharStr (index, numbuf, sizeof(numbuf), 0);
              rtxStrJoin (namebuf, sizeof(namebuf), "intraFreqBlackCellList", "[", numbuf, "]", 0);
              rtxPrintOpenBrace (namebuf);
              rtxPrintIndent ();
              rtxPrintUnsigned ("start", sib4Info->intra_freq_black_listed_cell_list.black_listed_cell[index].start);
              if ((sib4Info->intra_freq_black_listed_cell_list.black_listed_cell[index].presence_bitmask & PHY_CELL_ID_RANGE_PRESENCE_FLAG) == PHY_CELL_ID_RANGE_PRESENCE_FLAG)
              {
                  rtxPrintIndent ();
                  printf ("range = %d\n", sib4Info->intra_freq_black_listed_cell_list.black_listed_cell[index].range);
              }
              rtxPrintCloseBrace ();
         }
    }
    if ((sib4Info->presence_bitmask & SIB_4_PHY_CELL_ID_PRESENCE_FLAG) == SIB_4_PHY_CELL_ID_PRESENCE_FLAG)
    {
         rtxPrintOpenBrace ("csg_PhysCellIdRange");
         rtxPrintIndent ();
         rtxPrintUnsigned ("start",sib4Info->csg_phy_cell_id_range.start);
         if ((sib4Info->csg_phy_cell_id_range.presence_bitmask & PHY_CELL_ID_RANGE_PRESENCE_FLAG) == PHY_CELL_ID_RANGE_PRESENCE_FLAG)
         {
              rtxPrintIndent ();
              printf ("range = %d\n", sib4Info->csg_phy_cell_id_range.range);
         }
         rtxPrintCloseBrace ();

    }
    if ((sib4Info->presence_bitmask & SIB_4_LATE_NON_CRITICAL_EXT_PRESENT_FLAG) == SIB_4_LATE_NON_CRITICAL_EXT_PRESENT_FLAG)
    {
        rtxPrintIndent ();
        rtxPrintHexStr ("lateNonCriticalExtension", sib4Info->late_noncritical_ext.no_octet, sib4Info->late_noncritical_ext.data);
    }                  
    rtxPrintCloseBrace ();
}
void print_sib5( sib_type_5_Info_t *sib5Info )
{
    U32    index              = 0;
    U32    index1              = 0;
    char   namebuf[512];
    char   numbuf[32];

    rtxPrintOpenBrace ("sib5");

    for (index=0; index<sib5Info->inter_freq_carrier_freq_list.count; index++)
    {
        rtxUIntToCharStr (index, numbuf, sizeof(numbuf), 0);
        rtxStrJoin (namebuf, sizeof(namebuf), "interFreqCarrierFreqList", "[", numbuf, "]", 0);
        rtxPrintOpenBrace (namebuf);
        rtxPrintIndent ();
        rtxPrintUnsigned ("dl_CarrierFreq",sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].eutra_dl_carrier_freq);
        rtxPrintIndent ();
        rtxPrintInteger ("q_RxLevMin",sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].qrx_lev_min);
        if ((sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].presence_bitmask & INTER_FREQ_CARR_P_MAX_PRESENCE_FLAG) == INTER_FREQ_CARR_P_MAX_PRESENCE_FLAG)
        {
            rtxPrintIndent ();
            rtxPrintInteger ("p_Max",sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].p_max);
        }
        rtxPrintIndent ();
        rtxPrintUnsigned ("t_ReselectionEUTRA", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].t_reselection_eutran);
        if ((sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].presence_bitmask & INTER_FREQ_CARR_SCALE_PARAM_PRESENCE_FLAG) == INTER_FREQ_CARR_SCALE_PARAM_PRESENCE_FLAG)
        {
            rtxPrintOpenBrace ("t_ReselectionEUTRA_SF");
            rtxPrintIndent ();
            printf ("sf_Medium = %d\n",sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].speed_depend_scal_params.sf_medium);
            rtxPrintIndent ();
            printf ("sf_High = %d\n",sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].speed_depend_scal_params.sf_high);
            rtxPrintCloseBrace ();
        }
        rtxPrintIndent ();
        rtxPrintUnsigned ("threshX_High", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].thresh_x_high);
        rtxPrintIndent ();
        rtxPrintUnsigned ("threshX_Low", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].thresh_x_low);
        rtxPrintIndent ();
        rtxPrintUnsigned ("allowedMeasBandwidth", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].measurement_bandwidth);
        rtxPrintIndent ();
        rtxPrintUnsigned ("presenceAntennaPort1",sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].presence_antenna_port_1);
        if ((sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].presence_bitmask & INTER_FREQ_CARR_CELL_RESELCT_PRIOR_PRESENCE_FLAG) == INTER_FREQ_CARR_CELL_RESELCT_PRIOR_PRESENCE_FLAG)
        {
            rtxPrintIndent ();
            rtxPrintUnsigned ("cellReselectionPriority",sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].cell_reselect_priority);
        }
        rtxPrintIndent ();
        rtPrintBitStrBraceText ("neighCellConfig", 2, &(sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].neigh_cell_config));
        rtxPrintIndent ();
        rtxPrintUnsigned ("q_OffsetFreq",sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].q_offset_freq);
        if ((sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].presence_bitmask & INTER_FREQ_CARR_NEIGH_CELL_LIST_PRESENCE_FLAG) == INTER_FREQ_CARR_NEIGH_CELL_LIST_PRESENCE_FLAG)
        {
            for (index1=0; index1<sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].inter_freq_neigh_cell_list.count; index1++)
            {
                rtxUIntToCharStr (index1, numbuf, sizeof(numbuf), 0);
                rtxStrJoin (namebuf, sizeof(namebuf),"interFreqNeighCellList", "[", numbuf, "]", 0);
                rtxPrintOpenBrace (namebuf);
                rtxPrintIndent ();
                rtxPrintUnsigned ("physCellId", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].inter_freq_neigh_cell_list.neigh_cell[index1].phys_cell_id);
                rtxPrintIndent ();
                printf ("q_OffsetCell = %d\n", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].inter_freq_neigh_cell_list.neigh_cell[index1].q_offset_cell);
                rtxPrintCloseBrace ();
            }

        }
        if ((sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].presence_bitmask & INTER_FREQ_CARR_BLACK_LIST_CELL_LIST_PRESENCE_FLAG) == INTER_FREQ_CARR_BLACK_LIST_CELL_LIST_PRESENCE_FLAG)
        {
            for (index1=0; index1<sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].inter_freq_black_listed_cell_list.count; index1++)
            {
                rtxUIntToCharStr (index1, numbuf, sizeof(numbuf), 0);
                rtxStrJoin (namebuf, sizeof(namebuf), "interFreqBlackCellList", "[", numbuf, "]", 0);
                rtxPrintOpenBrace (namebuf);
                rtxPrintIndent ();
                rtxPrintUnsigned ("start", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].inter_freq_black_listed_cell_list.black_listed_cell[index1].start);
                if ((sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].inter_freq_black_listed_cell_list.black_listed_cell[index].presence_bitmask & PHY_CELL_ID_RANGE_PRESENCE_FLAG) == PHY_CELL_ID_RANGE_PRESENCE_FLAG)
                {
                    rtxPrintIndent ();
                    printf ("range = %d\n", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].inter_freq_black_listed_cell_list.black_listed_cell[index1].range);
                }
                rtxPrintCloseBrace ();
            }
        }
        if ((sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].presence_bitmask & INTER_FREQ_CARR_Q_QUAL_MIN_R9_PRESENCE_FLAG) == INTER_FREQ_CARR_Q_QUAL_MIN_R9_PRESENCE_FLAG)
        {
             rtxPrintIndent ();
             rtxPrintInteger ("q_QualMin_r9", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].q_qual_min_r9);
        }
        if ((sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].presence_bitmask & INTER_FREQ_THRESH_X_Q_R9_PRESENCE_FLAG) == INTER_FREQ_THRESH_X_Q_R9_PRESENCE_FLAG)
        {
             rtxPrintOpenBrace ("threshX_Q_r9");
             rtxPrintIndent ();
             rtxPrintUnsigned ("threshX_HighQ_r9", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].thresh_x_q_r9.thresh_x_high_q_r9);
             rtxPrintIndent ();
             rtxPrintUnsigned ("threshX_LowQ_r9", sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].thresh_x_q_r9.thresh_x_low_q_r9);
        }

        rtxPrintCloseBrace ();
    }
    if ((sib5Info->inter_freq_carrier_freq_list.inter_freq_carrier_freq_list[index].presence_bitmask & SIB_5_LATE_NON_CRITICAL_EXT_PRESENT_FLAG) == SIB_5_LATE_NON_CRITICAL_EXT_PRESENT_FLAG)
    {
        rtxPrintIndent ();
        rtxPrintHexStr ("lateNonCriticalExtension", sib5Info->late_noncritical_ext.no_octet, sib5Info->late_noncritical_ext.data);
    }                  
    rtxPrintCloseBrace ();
}

void print_sib6( sib_type_6_Info_t *sib6Info )
{
    U32    index              = 0;
    char   namebuf[512];
    char   numbuf[32];

    rtxPrintOpenBrace ("sib6");
    if((sib6Info->presence_bitmask & CARRIER_FREQ_LIST_UTRA_FDD) == CARRIER_FREQ_LIST_UTRA_FDD)
    {
        for (index=0; index<sib6Info->carrier_freq_list_UTRA_FDD.count; index++)
        {
            rtxUIntToCharStr (index, numbuf, sizeof(numbuf), 0);
            rtxStrJoin (namebuf, sizeof(namebuf), "carrier_freq_list_UTRA_FDD", "[", numbuf, "]", 0);
            rtxPrintOpenBrace (namebuf);
            rtxPrintIndent ();
            rtxPrintUnsigned ("carrier freq",sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].carrier_freq);
            if((sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].presence_bitmask & CARRIER_FREQ_UTRA_FDD_CELL_RESELECTION_PRIORITY) == CARRIER_FREQ_UTRA_FDD_CELL_RESELECTION_PRIORITY)
            {
                rtxPrintIndent ();
                rtxPrintUnsigned("cell reselection priority",sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].cell_reselection_priority);
            }
            rtxPrintIndent ();
            rtxPrintUnsigned("thresh X High",sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].thresh_X_High);
            rtxPrintIndent ();
            rtxPrintUnsigned("thresh X Low",sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].thresh_X_Low);
            rtxPrintIndent ();
            rtxPrintInteger ("q_RxLevMin",sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].q_Rx_Lev_Min);
            rtxPrintIndent ();
            rtxPrintInteger ("p Max UTRA",sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].p_Max_UTRA);
            rtxPrintIndent ();
            rtxPrintInteger ("q Qual min",sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].q_Qual_min);
            if((sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].presence_bitmask & CARRIER_FREQ_UTRA_FDD_THRESHX_Q_R9) == CARRIER_FREQ_UTRA_FDD_THRESHX_Q_R9)
            {
                rtxPrintIndent ();
                rtxPrintOpenBrace ("thresh X Q r9");
                rtxPrintIndent ();
                rtxPrintUnsigned("threshX_highQ_r9",sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].thresh_X_Q_r9.threshX_highQ_r9);
                rtxPrintUnsigned("threshX_LowQ_r9",sib6Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data[index].thresh_X_Q_r9.threshX_LowQ_r9);
                rtxPrintCloseBrace ();
            }
            rtxPrintCloseBrace ();
        }
    }
    if((sib6Info->presence_bitmask & CARRIER_FREQ_LIST_UTRA_TDD) == CARRIER_FREQ_LIST_UTRA_TDD)
    {
        for (index=0; index<sib6Info->carrier_freq_list_UTRA_TDD.count; index++)
        {
            rtxUIntToCharStr (index, numbuf, sizeof(numbuf), 0);
            rtxStrJoin (namebuf, sizeof(namebuf), "carrier_freq_list_UTRA_TDD", "[", numbuf, "]", 0);
            rtxPrintOpenBrace (namebuf);
            rtxPrintIndent ();
            rtxPrintUnsigned ("carrier freq",sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].carrier_freq);
            if((sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].presence_bitmask & CARRIER_FREQ_UTRA_TDD_CELL_RESELECTION_PRIORITY) == CARRIER_FREQ_UTRA_TDD_CELL_RESELECTION_PRIORITY)
            {
                rtxPrintIndent ();
                rtxPrintUnsigned("cell reselection priority",sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].cell_reselection_priority);
            }
            rtxPrintIndent ();
            rtxPrintUnsigned("thresh X High",sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].thresh_X_High);
            rtxPrintIndent ();
            rtxPrintUnsigned("thresh X Low",sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].thresh_X_Low);
            rtxPrintIndent ();
            rtxPrintInteger ("q_RxLevMin",sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].q_Rx_Lev_Min);
            rtxPrintIndent ();
            rtxPrintInteger ("p Max UTRA",sib6Info->carrier_freq_list_UTRA_TDD.carrier_freq_UTRA_TDD_data[index].p_Max_UTRA);
            rtxPrintCloseBrace ();
        }
    }
    rtxPrintIndent ();
    rtxPrintUnsigned("t reselection UTRA",sib6Info->t_reselection_UTRA);
    if((sib6Info->presence_bitmask & T_RESELECTION_UTRA_SF) == T_RESELECTION_UTRA_SF)
    {
        rtxPrintIndent ();
        rtxPrintOpenBrace ("t_reselection_UTRA_SF");
        rtxPrintUnsigned("sf_medium",sib6Info->t_reselection_UTRA_SF.sf_medium);
        rtxPrintIndent ();
        rtxPrintUnsigned("sf_high",sib6Info->t_reselection_UTRA_SF.sf_high);
        rtxPrintCloseBrace ();
    }
    rtxPrintCloseBrace ();
}
void print_sib9( sib_type_9_Info_t *sib9Info )
{
    rtxPrintOpenBrace ("sib9");
    printf("H eNB Name =  %s\n",sib9Info->hnb_Name);
    rtxPrintCloseBrace ();
}
int main(int argc, char *argv[])
{

    FILE            *fp=NULL;
    U16             noOfBytes=0,index=0; 
    S32             tempVal;
    OSOCTET         buffer[1000];
    return_code  ret_val  = SUCCESS;
    mib_messages_t decoded_mib_msg;
    sib_messages_t decoded_sibs_msg;

    U32  msgType;

    fp=fopen(argv[1],"r");
    if ( fp == NULL)
    {
        printf("Error opening file");
        exit(0);
    }
    while(fscanf(fp,"%d",&tempVal)!=EOF)
    {
        buffer[noOfBytes++]=tempVal;
    }

    fclose(fp);
    while(index<noOfBytes)
    {
        printf("%d ",buffer[index++]);
    }
    printf("N==%d",noOfBytes);
    printf("CHOICE                MSGTYPE");
    printf("\n0                   BCCH_BCH_MSG");
    printf("\n1                   BCCH_DL_MSG");
    printf("\nENTER MSG CHOCE:\n");
    scanf("%d",&msgType);

    switch(msgType)
    {
        case BCCH_BCH_MSG: 
            ret_val = mib_decoder(buffer, &decoded_mib_msg, noOfBytes);
            if(ret_val == SUCCESS)
            {
                print_mib(&(decoded_mib_msg.mib_info));
            }
            break;
        case BCCH_DL_SCH:
            ret_val = sib_and_si_decoder (buffer, &decoded_sibs_msg, noOfBytes);
            if(ret_val == SUCCESS)
            {
                printf("\nConverted SIBS\n\n");
                if ((decoded_sibs_msg.presence_bitmask & SIB_TYPE_1_PRESENCE_FLAG) == SIB_TYPE_1_PRESENCE_FLAG )
                {
                    print_sib1(&(decoded_sibs_msg.sib_type_1_Info));
                }
                if ((decoded_sibs_msg.presence_bitmask & SIB_TYPE_2_PRESENCE_FLAG) == SIB_TYPE_2_PRESENCE_FLAG )
                {
                    print_sib2(&(decoded_sibs_msg.sib_type_2_Info));
                }
                if ((decoded_sibs_msg.presence_bitmask & SIB_TYPE_3_PRESENCE_FLAG) == SIB_TYPE_3_PRESENCE_FLAG )
                {
                    print_sib3(&(decoded_sibs_msg.sib_type_3_Info));
                }
                if ((decoded_sibs_msg.presence_bitmask & SIB_TYPE_4_PRESENCE_FLAG) == SIB_TYPE_4_PRESENCE_FLAG )
                {
                    print_sib4(&(decoded_sibs_msg.sib_type_4_Info));
                }
                if ((decoded_sibs_msg.presence_bitmask & SIB_TYPE_5_PRESENCE_FLAG) == SIB_TYPE_5_PRESENCE_FLAG )
                {
                    print_sib5(&(decoded_sibs_msg.sib_type_5_Info));
                }
                if ((decoded_sibs_msg.presence_bitmask & SIB_TYPE_6_PRESENCE_FLAG) == SIB_TYPE_6_PRESENCE_FLAG )
                {
                    print_sib6(&(decoded_sibs_msg.sib_type_6_Info));
                }
                if ((decoded_sibs_msg.presence_bitmask & SIB_TYPE_9_PRESENCE_FLAG) == SIB_TYPE_9_PRESENCE_FLAG )
                {
                    print_sib9(&(decoded_sibs_msg.sib_type_9_Info));
                }
            }
            break;
        default:
            printf("\nWrong MsgType");
    }
    return 0;
}







