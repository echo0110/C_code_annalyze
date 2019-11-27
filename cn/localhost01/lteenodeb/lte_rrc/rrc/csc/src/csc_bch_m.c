/******************************************************************************
*
*   FILE NAME:
*       csc_bch_m.c
*
*   DESCRIPTION:
*       BCH manager check and ASN.1 encode MIB, SIB1 and SIs.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   04 June 2009    VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "csc_bch_m.h"
#include "csc_logger.h"
#include "rrc_asn_enc_dec.h"
#include "csc_bch_m_sibs.h"
#include "csc_cell_ctx.h"
#include "rrm_rrc_asn1_convertor.h"
#include "csc_cell_m_fsm.h"
#include "rtpersrc/asn1per.h"
#include "rrc_logging.h"
#include "rrc_defines.h"


/*
*   Defines
*/
#define ARRSIZE(array_name)     (sizeof(array_name) / sizeof(array_name[0]))

#define MIB_PERIODICITY         4
#define SFN_2_MIB_SFN_SHIFT     2

#define SF_UPPER_LIMIT          (SFN_UPPER_LIMIT * 10)

/* Number of bits for MIB and SIBs IEs */
#define TAC_NBITS           16
#define CELL_ID_NBITS       28
#define CSG_ID_NBITS        27
#define ONE_FRAME_NBITS     6
#define FOUR_FRAME_NBITS    24
#define N_CELL_CFG_NBITS    2
#define SYN_SYS_TIME_NBITS  39
#define ASYN_SYS_TIME_NBITS 49
#define MAX_INT_NBITS       32
#define MAX_LONG_NBITS      64
#define CSFB_REG_PARAM_1_XRTT_SID_NBITS 15
#define CSFB_REG_PARAM_1_XRTT_NID_NBITS 16
#define CSFB_REG_REG_PERIOD_NBITS       7
#define CSFB_REG_REG_ZONE_NBITS         12
#define CSFB_REG_TOTAL_ZONE_NBITS       3
#define CSFB_REG_ZONE_TIMER_NBITS       3
#define NCC_PERMITTED_NBITS             8
  /*CR 491 Changes */
#define MAX_SIB_DCI_FORMAT_1A_SIZE      277
#define MAX_SIB_DCI_FORMAT_1C_SIZE      217 
  /*CR 491 Changes */
/*cdma_fix bug 10186 start*/
/*
 * 12288 chips = 1 SFN
 * 1 chip = 0.000813802 msec
 * 8 chips = 0.006510417 msec/6.510416667 micro seconds
 * */
#define CDMA_8_CHIPS_TIME_UNIT           6.510416667 /*in microseconds*/
/*cdma_fix bug 10186 stop */
/*
*   Types
*/

typedef struct _asn1_sibs_t
{
    SystemInformationBlockType2     asn1_sib2;
    SystemInformationBlockType3     asn1_sib3;
    SystemInformationBlockType4     asn1_sib4;
    SystemInformationBlockType5     asn1_sib5;
    SystemInformationBlockType6     asn1_sib6;
    SystemInformationBlockType7     asn1_sib7;
    SystemInformationBlockType8     asn1_sib8;
    SystemInformationBlockType9     asn1_sib9;
    SystemInformationBlockType10    asn1_sib10;
#ifdef LTE_EMBMS_SUPPORTED
    SystemInformationBlockType13_r9 asn1_sib13;
#endif
} asn1_sibs_t;
 

/******************************************************************************
*   FUNCTION NAME: check_sib_size
*   INPUT        : csc_cell_ctx_t                       *p_cell_ctx
*                  U16                                  si_msg_buf_index
*   OUTPUT       : none
*   DESCRIPTION  : This function check encoded sib message size
*
*   RETURNS      : RRC_SUCCESS - if SIB size valid
*                  RRC_FAILURE - if ISB size invalid
*
******************************************************************************/
 rrc_return_et check_sib_size(
 /* Cell Context */
 csc_cell_ctx_t                       *p_cell_ctx ,
 /*SI Msg Buf Index */
 U16                                  si_msg_buf_index )
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_cell_ctx);

    if( RRC_RBS_PER_TBS_DCI_1A == p_cell_ctx->cfg_cell_param.cell_config_param.
            mac_config.dci_format_for_si_msgs ) 
    {
        if( MAX_SIB_DCI_FORMAT_1A_SIZE < si_msg_buf_index )
        {
            return RRC_FAILURE;
        }
    }
    else
    {
        if( MAX_SIB_DCI_FORMAT_1C_SIZE < si_msg_buf_index )
        {
            return RRC_FAILURE;
        }
    }
    return RRC_SUCCESS;
    RRC_CSC_UT_TRACE_EXIT();
}


extern sib8_sfn_list_t sib8_sfn_list;
U16 fill_si_periodicity_in_sfn (U8  si_periodicity);
/*
*   Check and convert functions for all SIBs
*/
/******************************************************************************
*   FUNCTION NAME: check_and_convert_sib1_cell_ari
*   INPUT        : OSCTXT                                                      *pctxt
*                  SystemInformationBlockType1_cellAccessRelatedInfo           *p_asn1_cell_ari
*                  cell_access_related_info_t                                  *p_cell_ari
*   OUTPUT       : none
*   DESCRIPTION  : This function converts cell_access_related_info_t to
*                  SystemInformationBlockType1_cellAccessRelatedInformation
*                  and checks if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et check_and_convert_sib1_cell_ari
(
    /* for memory allocation */
    OSCTXT                                                      *pctxt,
    /* destination */
    SystemInformationBlockType1_cellAccessRelatedInfo           *p_asn1_cell_ari
        ,
    /* source */
    cell_access_related_info_t                                  *p_cell_ari
)
{
    U8                          i               = RRC_NULL;
    U8                          digit           = RRC_NULL;
    OSRTDListNode               *p_node         = PNULL;
    PLMN_IdentityInfo           *p_asn1_l_elem  = PNULL;
    plmn_identity_info_t        *p_l_elem       = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    /* Init optional element to absent */
    p_asn1_cell_ari->m.csg_IdentityPresent = 0;

    /* PLMN-IdentityList */
    if (p_cell_ari->plmn_Id_info_list.count > ARRSIZE(p_cell_ari->
        plmn_Id_info_list.plmn_identity_info))
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Wrong size of PLMN-IdentityList: current=%u, RRM max=%u",
            p_cell_ari->plmn_Id_info_list.count,
            ARRSIZE(p_cell_ari->plmn_Id_info_list.plmn_identity_info));
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    rtxDListInit(&p_asn1_cell_ari->plmn_IdentityList);
    for (i = 0; i < p_cell_ari->plmn_Id_info_list.count; i++)
    {
        rtxDListAllocNodeAndData(pctxt, PLMN_IdentityInfo, &p_node,
            &p_asn1_l_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            return RRC_FAILURE;
        }
        asn1Init_PLMN_IdentityInfo(p_asn1_l_elem);
        p_l_elem = p_cell_ari->plmn_Id_info_list.plmn_identity_info + i;

        /* Check MCC condition: mcc must be present in the first
         *  PLMN-IdentityList item */
        if (    (0 == i) &&
                (0 == (p_l_elem->plmn_identity.presence_bitmask &
                    PLMN_IDENTITY_MCC_PRESENCE_FLAG)))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Mcc must be present in the first PLMN"
                "-IdentityList item");
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        /* Init optional element to absent */
        p_asn1_l_elem->plmn_Identity.m.mccPresent = 0;

        if (p_l_elem->plmn_identity.presence_bitmask &
            PLMN_IDENTITY_MCC_PRESENCE_FLAG)
        {
            p_asn1_l_elem->plmn_Identity.m.mccPresent = 1;

            p_asn1_l_elem->plmn_Identity.mcc.n = ARRSIZE(p_l_elem->plmn_identity
                .mcc);
            for (digit = 0; digit < ARRSIZE(p_l_elem->plmn_identity.mcc);
                digit++)
            {
                p_asn1_l_elem->plmn_Identity.mcc.elem[digit] = p_l_elem->
                    plmn_identity.mcc[digit];
            }
        }

        if (    (p_l_elem->plmn_identity.mnc.count > ARRSIZE(p_asn1_l_elem->
            plmn_Identity.mnc.elem)) ||
                (p_l_elem->plmn_identity.mnc.count > ARRSIZE(p_l_elem->
                    plmn_identity.mnc.mnc)))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Too many MNC digits: current=%u, RRM max=%u, ASN.1 max=%u",
                p_l_elem->plmn_identity.mnc.count,
                ARRSIZE(p_l_elem->plmn_identity.mnc.mnc),
                ARRSIZE(p_asn1_l_elem->plmn_Identity.mnc.elem));
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        p_asn1_l_elem->plmn_Identity.mnc.n = p_l_elem->plmn_identity.mnc.count;
        for (digit = 0; digit < p_l_elem->plmn_identity.mnc.count; digit++)
        {
            p_asn1_l_elem->plmn_Identity.mnc.elem[digit] = p_l_elem->
                plmn_identity.mnc.mnc[digit];
        }

        p_asn1_l_elem->cellReservedForOperatorUse = p_l_elem->
            cell_res_for_operator_use;

        rtxDListAppendNode(&p_asn1_cell_ari->plmn_IdentityList, p_node);
    }

    p_asn1_cell_ari->trackingAreaCode.numbits = TAC_NBITS;
    l3_memcpy_wrapper( p_asn1_cell_ari->trackingAreaCode.data,
            p_cell_ari->tac,
            sizeof(p_cell_ari->tac));

    p_asn1_cell_ari->cellIdentity.numbits = CELL_ID_NBITS;
    l3_memcpy_wrapper( p_asn1_cell_ari->cellIdentity.data,
            p_cell_ari->cell_Id,
            sizeof(p_asn1_cell_ari->cellIdentity.data));

    p_asn1_cell_ari->cellBarred             = p_cell_ari->cell_barred;
    p_asn1_cell_ari->intraFreqReselection   = p_cell_ari->
        intra_freq_reselection;

    if (RRC_FALSE == p_cell_ari->csg_indication)
    {
        p_asn1_cell_ari->csg_Indication = FALSE;
    }
    else
    {
        p_asn1_cell_ari->csg_Indication = TRUE;
    }

    if (    (RRC_TRUE == p_cell_ari->csg_indication) &&
            (0 == (p_cell_ari->presence_bitmask &
                CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG)))
    {
        /* Logical check: we should have p_cell_ari->csg_identity
         *  if p_cell_ari->csg_indication is TRUE */
        RRC_CSC_TRACE(RRC_WARNING, "We should have p_cell_ari->csg_identity if"
            " p_cell_ari->csg_indication is TRUE");
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    if (p_cell_ari->presence_bitmask & CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG)
    {
        p_asn1_cell_ari->m.csg_IdentityPresent = 1;

        p_asn1_cell_ari->csg_Identity.numbits = CSG_ID_NBITS;
        l3_memcpy_wrapper( p_asn1_cell_ari->csg_Identity.data,
                p_cell_ari->csg_identity,
                sizeof(p_asn1_cell_ari->csg_Identity.data));
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: check_and_convert_sib1_sched_inf
*   INPUT        : OSCTXT                      *pctxt
*                  SchedulingInfoList          *p_asn1_sched_inf
*                  scheduling_info_list_t       *p_sched_inf
*                  rrc_bitmask_t               presence_bitmask
*                  csc_cell_ctx_t          *p_cell_ctx
*   OUTPUT      :  none
*   DESCRIPTION : This function converts scheduling_info_list_t to
*                 SchedulingInformation_element
*                 and checks if input data are valid.
*                 This function checks number of SIs, si_Periodicity range,
*                 number of SIBs in SI and make sure that only known and present in
*                 message from RRM SIBs are mapped only once to SIs.
*
*   RETURNS     : RRC_SUCCESS - if all data are valid
*                 RRC_FAILURE - if data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et check_and_convert_sib1_sched_inf
(
    OSCTXT                      *pctxt,             /* for memory allocation */
    SchedulingInfoList          *p_asn1_sched_inf,  /* destination */
 scheduling_info_list_t       *p_sched_inf,       /* source */
    /* SIBs present in message from RRM */
    rrc_bitmask_t               presence_bitmask,
    csc_cell_ctx_t          *p_cell_ctx   /* cell context */    
)
{
    U8                              i               = RRC_NULL;
    U8                              SIB             = RRC_NULL;
    OSRTDListNode                   *p_node         = PNULL;
    SchedulingInfo                  *p_asn1_l_elem  = PNULL;
  scheduling_info_t                *p_l_elem       = PNULL;
    /* We don't have mapped SIBs at the beginning */
    U16                             mapped_sibs_mask= 0;
    U16                 per_si_sibs_bitmask = 0;

    RRC_CSC_UT_TRACE_ENTER();

    if (    (p_sched_inf->count < 1) ||
            (p_sched_inf->count > MAX_SI_MESSAGE) ||
      (p_sched_inf->count > ARRSIZE(p_sched_inf->scheduling_info)))
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Wrong number of SIs=%u. Should be 1..min(RRM=%u, ASN.1=%u)",
            p_sched_inf->count,
        ARRSIZE(p_sched_inf->scheduling_info),
            MAX_SI_MESSAGE);
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    
 
    
    /* Store the SI Mapping count in cell context */
    if(PNULL != p_cell_ctx->p_temp_si_schdl_info)
    {
        p_cell_ctx->p_temp_si_schdl_info->schdl_count = p_sched_inf->count;
    }
    
    rtxDListInit(p_asn1_sched_inf);
    for (i = 0; (i < p_sched_inf->count) && 
                (i < MAX_SCHDLING_COUNT); i++)
    {
        rtxDListAllocNodeAndData(pctxt, SchedulingInfo, &p_node,
            &p_asn1_l_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            return RRC_FAILURE;
        }
        asn1Init_SchedulingInfo(p_asn1_l_elem);

    p_l_elem = p_sched_inf->scheduling_info + i;

        /* Make sure that SIs scheduling information is OK */
        /* p_l_elem->si_periodicity should be defined in term of
         *  SchedulingInformation_element_si_Periodicity_Root */
        if (p_l_elem->si_periodicity > rf512)
        {
            RRC_CSC_TRACE(RRC_WARNING, "Wrong periodicity of SI=%u. Should be "
                "up to rf512", p_l_elem->si_periodicity);
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        p_asn1_l_elem->si_Periodicity = p_l_elem->si_periodicity;

        /* Check number of SIBs in this SI */
        if (    (p_l_elem->sib_mapping_info.count > MAX_SIB_MESSAGE_1) ||
                (p_l_elem->sib_mapping_info.count > ARRSIZE(p_l_elem->
                    sib_mapping_info.sib_type)))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Wrong number of SIBs in SI=%u. Should be 0..min(RRM=%u, ASN.1"
                    "=%u)",
                p_l_elem->sib_mapping_info.count,
                ARRSIZE(p_l_elem->sib_mapping_info.sib_type),
                MAX_SIB_MESSAGE_1);
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        RRC_ASSERT(MAX_SIB_MESSAGE_1 == ARRSIZE(p_asn1_l_elem->sib_MappingInfo.
            elem));

        /* mapped_sibs_mask should be big enough to hold all possible SIBs.*/
        RRC_ASSERT(sibType12_v920 < 16);
        /* Here we relay on strict connection between rrm_sib_type_et and
         *  _PRESENCE_FLAG for SIBs in message from RRM! */
        RRC_ASSERT(CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG == (1<<
            RRM_RRC_SIB_TYPE_3));
    RRC_ASSERT(CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG == (1<<
          RRM_RRC_SIB_TYPE_9));
        p_asn1_l_elem->sib_MappingInfo.n = p_l_elem->sib_mapping_info.count;

    /* Initialise the SI_SIBS_BITMASK */
    per_si_sibs_bitmask = 0;
        for (SIB = 0; SIB < p_l_elem->sib_mapping_info.count; SIB++)
        {
            /* Make sure that SIs scheduling information is OK */
            /* p_l_elem->sib_mapping_info.sib_type[SIB] should be defined in
             *  term of SIB_Type_Root */
            if (p_l_elem->sib_mapping_info.sib_type[SIB] > sibType13_v920)
            {
                RRC_CSC_TRACE(RRC_WARNING,
                    "Wrong SIB=%u in SI. Should be up to sibType13_v920",
                    p_l_elem->sib_mapping_info.sib_type[SIB]);
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            if (mapped_sibs_mask & (1 << p_l_elem->sib_mapping_info.
                sib_type[SIB]))
            {
                RRC_CSC_TRACE(RRC_WARNING,
                    "Duplicate SIB=%u mapping to SI(s)",
                    p_l_elem->sib_mapping_info.sib_type[SIB]);
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            if(CELL_M_STATE_IDLE == p_cell_ctx->current_state)
            {
                if (0 == (presence_bitmask & (1 << p_l_elem->sib_mapping_info.
                                sib_type[SIB])))
                {
                    RRC_CSC_TRACE(RRC_WARNING,
                            "Absents in message from RRM SIB=%u mapped to SI",
                            p_l_elem->sib_mapping_info.sib_type[SIB]);
                    RRC_CSC_UT_TRACE_EXIT();
                    return RRC_FAILURE;
                }
            }
            else
            {
                /* SPR 18964 changes start */
#ifdef LTE_EMBMS_SUPPORTED            
                if (CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG & p_cell_ctx->cfg_cell_param.presence_bitmask) 
                {
                    if (0 == (presence_bitmask & (9 << p_l_elem->sib_mapping_info.sib_type[SIB])))
                    {
                        RRC_CSC_TRACE(RRC_WARNING,
                                "Absents in message from RRM SIB=%u mapped to SI",
                                p_l_elem->sib_mapping_info.sib_type[SIB]+ 3);
                        RRC_CSC_UT_TRACE_EXIT();
                        return RRC_FAILURE;
                    }
                }
                else
                {
                    if (0 == (presence_bitmask & (8 << p_l_elem->sib_mapping_info.sib_type[SIB])))
                    {
                        RRC_CSC_TRACE(RRC_WARNING,
                                "Absents in message from RRM SIB=%u mapped to SI",
                                p_l_elem->sib_mapping_info.sib_type[SIB]+ 3);
                        RRC_CSC_UT_TRACE_EXIT();
                        return RRC_FAILURE;
                    }

                }
#else
                if (0 == (presence_bitmask & (8 << p_l_elem->sib_mapping_info.sib_type[SIB])))
                {
                    RRC_CSC_TRACE(RRC_WARNING,
                            "Absents in message from RRM SIB=%u mapped to SI",
                            p_l_elem->sib_mapping_info.sib_type[SIB]+ 3);
                    RRC_CSC_UT_TRACE_EXIT();
                    return RRC_FAILURE;
                }

#endif                
                /* SPR 18964 changes end */
            }

        per_si_sibs_bitmask |= (U16)(8 << p_l_elem->sib_mapping_info.sib_type[SIB]);    
            mapped_sibs_mask |= (U16)(1 << p_l_elem->sib_mapping_info.sib_type[SIB]);
            p_asn1_l_elem->sib_MappingInfo.elem[SIB] = p_l_elem->
                sib_mapping_info.sib_type[SIB];
    }
    if(PNULL != p_cell_ctx->p_temp_si_schdl_info)
    {
	    /* Store the Si_sibs_bitmask in cell ctx*/
	    p_cell_ctx->p_temp_si_schdl_info->si_bitmask[i] = per_si_sibs_bitmask;

	    /* Storing contents for Cell Reconfiguration */
	    /* This will be used during Cell Reconfiguration when sib1 will be absent*/
	    if (CELL_M_STATE_IDLE == p_cell_ctx->current_state)
	    {
		    p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask |= per_si_sibs_bitmask;
	    }
    }
    
    rtxDListAppendNode(p_asn1_sched_inf, p_node);
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib1
*   INPUT        : OSCTXT                      *pctxt
*                  SystemInformationBlockType1 *p_asn1_sib1
*                  sib_type_1_Info_t           *p_sib1
*                  rrc_bitmask_t               presence_bitmask
*                  csc_cell_ctx_t              *p_cell_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function converts sib_type_1_Info_t to
*                  SystemInformationBlockType1
*                  and checks if input data are valid.
*                  Full check of SchedulingInformation_element and si_WindowLength is
*                  out of scope of this function.
*                  This function check si_WindowLength range.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data are invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib1
(
    OSCTXT                      *pctxt,             /* for memory allocation */
    SystemInformationBlockType1 *p_asn1_sib1,       /* destination */
    sib_type_1_Info_t           *p_sib1,            /* source */
    /* SIBs present in message from RRM */
    rrc_bitmask_t               presence_bitmask,
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
)
{
	/*CR 970 changes start*/
	U8  late_non_critical_extension_buff[MAX_ASN_BUFF_LENGTH];
	SystemInformationBlockType1_v8h0_IEs    sib_1_v8h0_ies;
	OSCTXT      asn1_ctx;
	U32         encodedmsg_len = 0;
	U8          index = RRC_NULL;;
	S32         enc_res = RRC_NULL;

	memset_wrapper(late_non_critical_extension_buff, RRC_NULL, 
			sizeof(late_non_critical_extension_buff));

	/*CR 970 changes stop*/
	RRC_CSC_UT_TRACE_ENTER();

	/*Init*/
	asn1Init_SystemInformationBlockType1(p_asn1_sib1);     

	/*Init*/
	asn1Init_SystemInformationBlockType1(p_asn1_sib1);

	/* Init optional element to absent */
	p_asn1_sib1->m.p_MaxPresent                 = 0;
	p_asn1_sib1->m.tdd_ConfigPresent            = 0;
	p_asn1_sib1->m.nonCriticalExtensionPresent  = 0;
	/*KlockWork fix*/
	memset_wrapper(&sib_1_v8h0_ies, RRC_NULL,
			sizeof(SystemInformationBlockType1_v8h0_IEs));
	/*KlockWork fix*/

	if (RRC_SUCCESS != check_and_convert_sib1_cell_ari( pctxt,
				&p_asn1_sib1->
				cellAccessRelatedInfo,
				&p_sib1->
				cell_access_related_info))
	{
		RRC_CSC_UT_TRACE_EXIT();
		return RRC_FAILURE;
	}

	/* Fill p_asn1_sib1->cellSelectionInfo */
	/* Init optional element to absent */
	p_asn1_sib1->cellSelectionInfo.m.q_RxLevMinOffsetPresent = 0;

	p_asn1_sib1->cellSelectionInfo.q_RxLevMin = p_sib1->cell_selection_Info.
		q_rx_lev_min;
	if (p_sib1->cell_selection_Info.presence_bitmask &
			CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG)
	{
		p_asn1_sib1->cellSelectionInfo.m.q_RxLevMinOffsetPresent = 1;

		p_asn1_sib1->cellSelectionInfo.q_RxLevMinOffset = p_sib1->
			cell_selection_Info.q_rx_lev_min_offset;
	}

	if (p_sib1->presence_bitmask & SIB_TYPE_1_P_MAX_PRESENCE_FLAG)
	{
		p_asn1_sib1->m.p_MaxPresent = 1;

		p_asn1_sib1->p_Max = p_sib1->p_max;
	}

	if (p_sib1->presence_bitmask & SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG)
	{
		p_asn1_sib1->m.tdd_ConfigPresent = 1;

		p_asn1_sib1->tdd_Config.subframeAssignment = p_sib1->tdd_config.
			sub_frame_assignment;
		p_asn1_sib1->tdd_Config.specialSubframePatterns = p_sib1->tdd_config.
			special_sub_frame_pattern;
	}
     p_asn1_sib1->freqBandIndicator = p_sib1->freq_band_indicator;

	if (RRC_SUCCESS != check_and_convert_sib1_sched_inf(    pctxt,
				&p_asn1_sib1->
				schedulingInfoList,
				&p_sib1->
				scheduling_info_list,
				presence_bitmask, p_cell_ctx))
	{
		RRC_CSC_UT_TRACE_EXIT();
		return RRC_FAILURE;
	}

	/* tdd-Configuration is absent in release 0.5 */

	/* Make sure that SIs scheduling information is OK */
	/* p_sib1->si_window_length should be defined in term of
	 *  SystemInformationBlockType1_si_WindowLength_Root */
	if (  p_sib1->si_window_length > ms40) 
	{
		RRC_CSC_TRACE(RRC_WARNING,
				"Wrong SI window length=%u. Should be up to "
				"RRM_RRC_SI_WINDOW_LEN_MS_40==ms40",
				p_sib1->si_window_length);
		RRC_CSC_UT_TRACE_EXIT();
		return RRC_FAILURE;
	}
	p_asn1_sib1->si_WindowLength = p_sib1->si_window_length;

	p_asn1_sib1->systemInfoValueTag = p_sib1->si_value_tag;

	if ( p_sib1->presence_bitmask & 
			SIB_TYPE_1_NON_CRITICAL_EXTENSION_PRESENT_FLAG )
	{
		p_asn1_sib1->m.nonCriticalExtensionPresent = 1;
		p_asn1_sib1->nonCriticalExtension.m.lateNonCriticalExtensionPresent = 0;
		p_asn1_sib1->nonCriticalExtension.m.nonCriticalExtensionPresent = 0;
		if ( p_sib1->non_critical_extension.presence_bitmask & 
				SIB_TYPE_1_V890_IES_NON_CROTICAL_EXTENSION_PRESENT_FLAG )
		{
			p_asn1_sib1->nonCriticalExtension.m.nonCriticalExtensionPresent = 1;
			p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
				m.ims_EmergencySupport_r9Present = 0;
			p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
				m.cellSelectionInfo_v920Present = 0;
			p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
				m.nonCriticalExtensionPresent = 0;
			if (p_sib1->non_critical_extension.non_critical_extension.\
					presence_bitmask & 
					SIB_TYPE_1_V920_IES_IMS_EMERGENCY_SUPPORT_PRESENT_FLAG)
			{
				p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
					m.ims_EmergencySupport_r9Present = 1;
				p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
					ims_EmergencySupport_r9 = p_sib1->non_critical_extension.\
					non_critical_extension.ims_emergency_support;
                                /* SPR_20061_Fix_Start */
                                /* Code Related to check of Null Integrity support has been removed as
                                 * check is not required to allow "authenticated IMS Emergency" calls */   
                                /* SPR_20061_Fix_End */
			}
			/*cr_398: Cell reselection enhancements*/
			if ((RRC_TRUE == p_cell_ctx->sib3_thresh_serving_low_q_flag) &&
					!(SIB_TYPE_1_V920_IES_CELL_SELECTION_INFO_V920_PRESENT_FLAG &
						p_sib1->non_critical_extension.non_critical_extension.presence_bitmask))
			{
				if (RRC_TRUE == p_cell_ctx->sib1_cellSelectionInfo_v920_flag)
				{
					RRC_CSC_TRACE(RRC_INFO,
							"The field  cellSelectionInfo_v920 is already sent "
							"during Cell Setup..");
				}
				else
				{
					RRC_CSC_TRACE(RRC_WARNING,
							"Wrong Configuration: sib3:threshServingLowQ is present "
							"without sib1:CellSelectionInfo-v920");
					RRC_CSC_UT_TRACE_EXIT();
					return RRC_FAILURE;
				}

			}
			else if (SIB_TYPE_1_V920_IES_CELL_SELECTION_INFO_V920_PRESENT_FLAG 
					& p_sib1->non_critical_extension.non_critical_extension.
					presence_bitmask)
			{
				p_cell_ctx->sib1_cellSelectionInfo_v920_flag = RRC_TRUE;

				p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
					m.cellSelectionInfo_v920Present = 1;

				p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
					cellSelectionInfo_v920.m.q_QualMinOffset_r9Present = 0;

				p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
					cellSelectionInfo_v920.q_QualMin_r9 = p_sib1->non_critical_extension.
					non_critical_extension.cell_selection_info_v920.q_qual_min_r9; 

				if (p_sib1->non_critical_extension.non_critical_extension.
						cell_selection_info_v920.presence_bitmask & 
						CELL_SELECTION_INFO_Q_QUAL_MIN_OFFSET_R9_PRESENT_FLAG)
				{
					p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
						cellSelectionInfo_v920.m.q_QualMinOffset_r9Present = 1;

					p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
						cellSelectionInfo_v920.q_QualMinOffset_r9 = p_sib1->non_critical_extension.
						non_critical_extension.cell_selection_info_v920.q_qual_min_offset_r9;
				}
			}
		}
		/* CR 970 changes start*/
		if ( p_sib1->non_critical_extension.presence_bitmask & 
				SIB_TYPE_1_V890_IES_LATE_NON_CRITICAL_EXTENSION_PRESENT_FLAG)
		{
			p_asn1_sib1->nonCriticalExtension.m.lateNonCriticalExtensionPresent = 1;

			if (p_sib1->non_critical_extension.late_non_critical_extension.bitmask &
					SIB_TYPE_1_v8h0_IES_MULTI_BAND_INFO_LIST_PRESENT)
			{
				/*valgrind_fix start*/
				memset_wrapper(&sib_1_v8h0_ies, RRC_NULL, 
						sizeof(SystemInformationBlockType1_v8h0_IEs));
				/*valgrind_fix stop*/

				sib_1_v8h0_ies.m.multiBandInfoListPresent = 1;
				sib_1_v8h0_ies.m.nonCriticalExtensionPresent = 0;

				sib_1_v8h0_ies.multiBandInfoList.n = (U32)p_sib1->non_critical_extension.
					late_non_critical_extension.multi_band_info_list.count;

				for(index = 0; index < sib_1_v8h0_ies.multiBandInfoList.n; index++)
				{
					sib_1_v8h0_ies.multiBandInfoList.elem[index] = p_sib1->non_critical_extension.
						late_non_critical_extension.multi_band_info_list.freq_band_indicator[index];
				}
			}

			if (RT_OK != rtInitContext(&asn1_ctx))
			{
				RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
				return RRC_FAILURE;
			}

			pu_setBuffer(&asn1_ctx, late_non_critical_extension_buff, 
					MAX_ASN_BUFF_LENGTH, ASN_RRC_BUF_TYPE);

			enc_res = asn1PE_SystemInformationBlockType1_v8h0_IEs(&asn1_ctx, &sib_1_v8h0_ies);
			if(RT_OK !=  enc_res)
			{
				RRC_CSC_TRACE(RRC_ERROR,
						"ASN1 encoding failed.");
			}
			else
			{
				encodedmsg_len = (U32)pe_GetMsgLen(&asn1_ctx);
				p_asn1_sib1->nonCriticalExtension.lateNonCriticalExtension.data = 
					(OSOCTET*)rtxMemAlloc(pctxt, encodedmsg_len);
                /* SPR 12964 Fix Start */
                if(PNULL == p_asn1_sib1->nonCriticalExtension.lateNonCriticalExtension.data)
                {
                   RRC_CSC_TRACE(RRC_ERROR, "Unable to allocate memory.");
                   rtFreeContext(&asn1_ctx);
                   RRC_CSC_UT_TRACE_EXIT();
                   return RRC_FAILURE;

                }
                /* SPR 12964 Fix Stop */

				l3_memcpy_wrapper((void *)p_asn1_sib1->nonCriticalExtension.lateNonCriticalExtension.data, 
						late_non_critical_extension_buff , encodedmsg_len);

				p_asn1_sib1->nonCriticalExtension.lateNonCriticalExtension.numocts = 
					encodedmsg_len;
			}
			/*valgrind_fix start*/
			rtFreeContext(&asn1_ctx);
			/*valgrind_fix stop*/
		}
		/* CR 970 changes stop*/
	}


	RRC_CSC_UT_TRACE_EXIT();
	return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib1_pws
*   INPUT        : OSCTXT                      *pctxt
*                  SystemInformationBlockType1 *p_asn1_sib1
*                  sib_type_1_Info_t           *p_sib1
*                  rrc_bitmask_t               presence_bitmask
*                  rrc_bool_et                 is_pws_req
*                  csc_cell_ctx_t          *p_cell_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function converts sib_type_1_Info_t to
*                  SystemInformationBlockType1
*                  and checks if input data are valid.
*                  Full check of SchedulingInformation_element and si_WindowLength is
*                  out of scope of this function.
*                  This function check si_WindowLength range.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data are invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib1_pws
(
    OSCTXT                      *pctxt,             /* for memory allocation */
    SystemInformationBlockType1 *p_asn1_sib1,       /* destination */
    sib_type_1_Info_t           *p_sib1,            /* source */
    /* SIBs present in message from RRM */
    rrc_bitmask_t               presence_bitmask,
    rrc_bool_et                 is_pws_req, /*Flag for PWS or updated PWS */
    csc_cell_ctx_t          *p_cell_ctx    /* cell context */   
)
{
	/*SPR 14343 Fix Start*/
	U8  late_non_critical_extension_buff[MAX_ASN_BUFF_LENGTH];
	SystemInformationBlockType1_v8h0_IEs    sib_1_v8h0_ies;
	OSCTXT      asn1_ctx;
	U32         encodedmsg_len = 0;
	U8          index = RRC_NULL;;
	S32         enc_res = RRC_NULL;

	memset_wrapper(late_non_critical_extension_buff, RRC_NULL, 
			sizeof(late_non_critical_extension_buff));

	/*SPR 14343 Fix Stop*/
    RRC_CSC_UT_TRACE_ENTER();

    /*Init*/
    asn1Init_SystemInformationBlockType1(p_asn1_sib1);

    /* Init optional element to absent */
    p_asn1_sib1->m.p_MaxPresent                 = 0;
    p_asn1_sib1->m.tdd_ConfigPresent            = 0;
    p_asn1_sib1->m.nonCriticalExtensionPresent  = 0;

    if (RRC_SUCCESS != check_and_convert_sib1_cell_ari( pctxt,
                                                &p_asn1_sib1->
                                                    cellAccessRelatedInfo,
                                                &p_sib1->
                                                    cell_access_related_info))
    {
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* Fill p_asn1_sib1->cellSelectionInfo */
    /* Init optional element to absent */
    p_asn1_sib1->cellSelectionInfo.m.q_RxLevMinOffsetPresent = 0;

    p_asn1_sib1->cellSelectionInfo.q_RxLevMin = p_sib1->cell_selection_Info.
        q_rx_lev_min;
    if (p_sib1->cell_selection_Info.presence_bitmask &
        CELL_SELECT_INFO_Q_RX_LEV_MIN_OFFSET_PRESENCE_FLAG)
    {
        p_asn1_sib1->cellSelectionInfo.m.q_RxLevMinOffsetPresent = 1;

        p_asn1_sib1->cellSelectionInfo.q_RxLevMinOffset = p_sib1->
            cell_selection_Info.q_rx_lev_min_offset;
    }

    if (p_sib1->presence_bitmask & SIB_TYPE_1_P_MAX_PRESENCE_FLAG)
    {
        p_asn1_sib1->m.p_MaxPresent = 1;

        p_asn1_sib1->p_Max = p_sib1->p_max;
    }

    if (p_sib1->presence_bitmask & SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG)
    {
        p_asn1_sib1->m.tdd_ConfigPresent = 1;

        p_asn1_sib1->tdd_Config.subframeAssignment = p_sib1->tdd_config.
        sub_frame_assignment;
        p_asn1_sib1->tdd_Config.specialSubframePatterns = p_sib1->tdd_config.
        special_sub_frame_pattern;
    }
    p_asn1_sib1->freqBandIndicator = p_sib1->freq_band_indicator;
    if ( RRC_TRUE == is_pws_req )
    {
        if (RRC_SUCCESS != check_and_convert_sib1_sched_inf(    pctxt,
                                                    &p_asn1_sib1->
                                                        schedulingInfoList,
                                                    &(p_cell_ctx->p_enb_pws_req->
                                                    scheduling_info_list),
                                                    presence_bitmask, p_cell_ctx))
        {
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
    }
    else
    {
        if (RRC_SUCCESS != check_and_convert_sib1_sched_inf(    pctxt,
                                           &p_asn1_sib1->schedulingInfoList,
                                           &(p_sib1->scheduling_info_list),/*ETWS crash fix*/
                                           presence_bitmask, p_cell_ctx))
        {
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
    }
    /* tdd-Configuration is absent in release 0.5 */

    /* Make sure that SIs scheduling information is OK */
    /* p_sib1->si_window_length should be defined in term of
     *  SystemInformationBlockType1_si_WindowLength_Root */
    if (  p_sib1->si_window_length > ms40)
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Wrong SI window length=%u. Should be up to "
                "RRM_RRC_SI_WINDOW_LEN_MS_40==ms40",
            p_sib1->si_window_length);
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    p_asn1_sib1->si_WindowLength = p_sib1->si_window_length;

    p_asn1_sib1->systemInfoValueTag = p_sib1->si_value_tag;

    if ( p_sib1->presence_bitmask & 
                 SIB_TYPE_1_NON_CRITICAL_EXTENSION_PRESENT_FLAG )
    {
        p_asn1_sib1->m.nonCriticalExtensionPresent = 1;
        p_asn1_sib1->nonCriticalExtension.m.lateNonCriticalExtensionPresent = 0;
        p_asn1_sib1->nonCriticalExtension.m.nonCriticalExtensionPresent = 0;
        if ( p_sib1->non_critical_extension.presence_bitmask & 
                       SIB_TYPE_1_V890_IES_NON_CROTICAL_EXTENSION_PRESENT_FLAG )
        {
            p_asn1_sib1->nonCriticalExtension.m.nonCriticalExtensionPresent = 1;
            p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
                  m.ims_EmergencySupport_r9Present = 0;
            p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
                  m.cellSelectionInfo_v920Present = 0;
            p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
                  m.nonCriticalExtensionPresent = 0;
            if (p_sib1->non_critical_extension.non_critical_extension.\
                  presence_bitmask & 
                    SIB_TYPE_1_V920_IES_IMS_EMERGENCY_SUPPORT_PRESENT_FLAG)
            {
                p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
                  m.ims_EmergencySupport_r9Present = 1;
                p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.\
                  ims_EmergencySupport_r9 = p_sib1->non_critical_extension.\
                     non_critical_extension.ims_emergency_support;
                /* SPR_20061_Fix_Start */
                /* Code Related to check of Null Integrity support has been removed as
                 * check is not required to allow "authenticated IMS Emergency" calls */   
                /* SPR_20061_Fix_End */
            } 

	        /*SPR 14343 Fix Start*/
			if ((RRC_TRUE == p_cell_ctx->sib3_thresh_serving_low_q_flag) &&
					!(SIB_TYPE_1_V920_IES_CELL_SELECTION_INFO_V920_PRESENT_FLAG &
						p_sib1->non_critical_extension.non_critical_extension.presence_bitmask))
			{
				if (RRC_TRUE == p_cell_ctx->sib1_cellSelectionInfo_v920_flag)
				{
					RRC_CSC_TRACE(RRC_INFO,
							"The field  cellSelectionInfo_v920 is already sent "
							"during Cell Setup..");
				}
				else
				{
					RRC_CSC_TRACE(RRC_WARNING,
							"Wrong Configuration: sib3:threshServingLowQ is present "
							"without sib1:CellSelectionInfo-v920");
					RRC_CSC_UT_TRACE_EXIT();
					return RRC_FAILURE;
				}

			}
			else if (SIB_TYPE_1_V920_IES_CELL_SELECTION_INFO_V920_PRESENT_FLAG 
					& p_sib1->non_critical_extension.non_critical_extension.
					presence_bitmask)
			{
				p_cell_ctx->sib1_cellSelectionInfo_v920_flag = RRC_TRUE;

				p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
					m.cellSelectionInfo_v920Present = 1;

				p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
					cellSelectionInfo_v920.m.q_QualMinOffset_r9Present = 0;

				p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
					cellSelectionInfo_v920.q_QualMin_r9 = p_sib1->non_critical_extension.
					non_critical_extension.cell_selection_info_v920.q_qual_min_r9; 

				if (p_sib1->non_critical_extension.non_critical_extension.
						cell_selection_info_v920.presence_bitmask & 
						CELL_SELECTION_INFO_Q_QUAL_MIN_OFFSET_R9_PRESENT_FLAG)
				{
					p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
						cellSelectionInfo_v920.m.q_QualMinOffset_r9Present = 1;

					p_asn1_sib1->nonCriticalExtension.nonCriticalExtension.
						cellSelectionInfo_v920.q_QualMinOffset_r9 = p_sib1->non_critical_extension.
						non_critical_extension.cell_selection_info_v920.q_qual_min_offset_r9;
				}
			}
		}
		if ( p_sib1->non_critical_extension.presence_bitmask & 
				SIB_TYPE_1_V890_IES_LATE_NON_CRITICAL_EXTENSION_PRESENT_FLAG)
		{
			p_asn1_sib1->nonCriticalExtension.m.lateNonCriticalExtensionPresent = 1;

			if (p_sib1->non_critical_extension.late_non_critical_extension.bitmask &
					SIB_TYPE_1_v8h0_IES_MULTI_BAND_INFO_LIST_PRESENT)
			{
				memset_wrapper(&sib_1_v8h0_ies, RRC_NULL, 
						sizeof(SystemInformationBlockType1_v8h0_IEs));

				sib_1_v8h0_ies.m.multiBandInfoListPresent = 1;
				sib_1_v8h0_ies.m.nonCriticalExtensionPresent = 0;

				sib_1_v8h0_ies.multiBandInfoList.n = (U32)p_sib1->non_critical_extension.
					late_non_critical_extension.multi_band_info_list.count;

				for(index = 0; index < sib_1_v8h0_ies.multiBandInfoList.n; index++)
				{
					sib_1_v8h0_ies.multiBandInfoList.elem[index] = p_sib1->non_critical_extension.
						late_non_critical_extension.multi_band_info_list.freq_band_indicator[index];
				}
			}

			if (RT_OK != rtInitContext(&asn1_ctx))
			{
				RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
				return RRC_FAILURE;
			}

			pu_setBuffer(&asn1_ctx, late_non_critical_extension_buff, 
					MAX_ASN_BUFF_LENGTH, ASN_RRC_BUF_TYPE);

			enc_res = asn1PE_SystemInformationBlockType1_v8h0_IEs(&asn1_ctx, &sib_1_v8h0_ies);
			if(RT_OK !=  enc_res)
			{
				RRC_CSC_TRACE(RRC_ERROR,
						"ASN1 encoding failed.");
			}
			else
			{
				encodedmsg_len = (U32)pe_GetMsgLen(&asn1_ctx);
				p_asn1_sib1->nonCriticalExtension.lateNonCriticalExtension.data = 
					(OSOCTET*)rtxMemAlloc(pctxt, encodedmsg_len);
                if(PNULL == p_asn1_sib1->nonCriticalExtension.lateNonCriticalExtension.data)
                {
                   RRC_CSC_TRACE(RRC_ERROR, "Unable to allocate memory.");
                   rtFreeContext(&asn1_ctx);
                   RRC_CSC_UT_TRACE_EXIT();
                   return RRC_FAILURE;

                }

				l3_memcpy_wrapper((void *)p_asn1_sib1->nonCriticalExtension.lateNonCriticalExtension.data, 
						late_non_critical_extension_buff , encodedmsg_len);

				p_asn1_sib1->nonCriticalExtension.lateNonCriticalExtension.numocts = 
					encodedmsg_len;
			}
			rtFreeContext(&asn1_ctx);
		}
	    /*SPR 14343 Fix Stop*/
    } 
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: fill_access_class_barring_info
*   INPUT        : AC_BarringConfig                *p_asn1_access_class
*                  access_class_barring_Info_t     *p_access_class
*   OUTPUT       : none
*   DESCRIPTION  : This function converts access_class_barring_Info_t to
*                  AC_BarringConfig (previously named AccessClassBarringInformation)
*                  and checks whether input data are valid.
*
*   RETURNS      : RRC_SUCCESS - all data are valid
*                  RRC_FAILURE - data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et fill_access_class_barring_info
(
    AC_BarringConfig                *p_asn1_access_class,   /* destination */
    access_class_barring_Info_t     *p_access_class         /* source */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_access_class);
    RRC_ASSERT(PNULL != p_access_class);

    p_asn1_access_class->ac_BarringFactor = p_access_class->ac_barring_factor;
    p_asn1_access_class->ac_BarringTime   = p_access_class->ac_barring_time;

    p_asn1_access_class->ac_BarringForSpecialAC.numbits = MAX_AC_VALUE;
    p_asn1_access_class->ac_BarringForSpecialAC.data[0] = p_access_class->
        ac_barring_for_special_ac;
    
    /* cr_524 ac_BarringFactor value check */
    if ((p_asn1_access_class->ac_BarringFactor != p00 ) && 
        (p_access_class->ac_barring_for_special_ac != RRC_NULL ))
    {
        RRC_CSC_TRACE(RRC_ERROR,
            "ac-BarringFactor: Values other than p00 can only be set if all bits of "
            "the corresponding ac-BarringForSpecialAC are set to 0.");
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: fill_radio_resource_config_common
*   INPUT        : OSCTXT                              *p_asn1_ctx
*                  RadioResourceConfigCommonSIB        *p_asn1_cc
*                  radio_resource_config_common_sib_t  *p_cc
*                  duplexing_mode_et                    duplexing_mode
*   OUTPUT       : none
*   DESCRIPTION  : This function converts radio_resource_config_common_sib_t to
*                  RadioResourceConfigCommonSIB
*                  and checks whether input data are valid.
*
*   RETURNS      : RRC_SUCCESS - all data are valid
*                  RRC_FAILURE - data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et fill_radio_resource_config_common
(
 /* for memory allocation */
 OSCTXT                              *p_asn1_ctx,
 RadioResourceConfigCommonSIB        *p_asn1_cc,     /* destination */
 /*** TDD changes start ***/
 radio_resource_config_common_sib_t  *p_cc,           /* source */
 duplexing_mode_et                    duplexing_mode
 /*** TDD changes stop ***/

)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_asn1_cc);
    RRC_ASSERT(PNULL != p_cc);

    /* - rach_Configuration */
    /* -- rach_Configuration/preambleInformation */
    p_asn1_cc->rach_ConfigCommon.preambleInfo.numberOfRA_Preambles =
        p_cc->rach_config_common.preamble_info.ra_preamble_count;

    p_asn1_cc->rach_ConfigCommon.preambleInfo.m.preamblesGroupAConfigPresent = 0;

    if (p_cc->rach_config_common.preamble_info.presence_bitmask &
        PREAMBLE_INFO_GROUP_A_CONFIG_PRESENCE_FLAG)
    {
        p_asn1_cc->rach_ConfigCommon.preambleInfo.m.preamblesGroupAConfigPresent = 1;

        p_asn1_cc->rach_ConfigCommon.preambleInfo.preamblesGroupAConfig.
            sizeOfRA_PreamblesGroupA =
            p_cc->rach_config_common.preamble_info.preambles_group_a_config.
                ra_preambles_group_a_size;

        p_asn1_cc->rach_ConfigCommon.preambleInfo.preamblesGroupAConfig.
            messageSizeGroupA =
            p_cc->rach_config_common.preamble_info.preambles_group_a_config.
                group_a_msg_size;

        p_asn1_cc->rach_ConfigCommon.preambleInfo.preamblesGroupAConfig.
            messagePowerOffsetGroupB =
            p_cc->rach_config_common.preamble_info.preambles_group_a_config.
                group_b_msg_power_offset;

        rtxDListInit(&p_asn1_cc->rach_ConfigCommon.preambleInfo.preamblesGroupAConfig.
            extElem1);
    }

    /* -- rach_Configuration/powerRampingParameters */
    p_asn1_cc->rach_ConfigCommon.powerRampingParameters.powerRampingStep =
        p_cc->rach_config_common.power_ramping_params.power_ramping_step;

    p_asn1_cc->rach_ConfigCommon.powerRampingParameters.
        preambleInitialReceivedTargetPower =
        p_cc->rach_config_common.power_ramping_params.
            preamble_init_rec_target_pow;

    /* -- rach_Configuration/ra_SupervisionInformation */
    p_asn1_cc->rach_ConfigCommon.ra_SupervisionInfo.preambleTransMax =
        p_cc->rach_config_common.ra_supervision_info.preamble_trans_max;

    p_asn1_cc->rach_ConfigCommon.ra_SupervisionInfo.ra_ResponseWindowSize =
        p_cc->rach_config_common.ra_supervision_info.ra_resp_win_size;

    p_asn1_cc->rach_ConfigCommon.ra_SupervisionInfo.mac_ContentionResolutionTimer =
        p_cc->rach_config_common.ra_supervision_info.mac_cont_resol_timer;

    p_asn1_cc->rach_ConfigCommon.maxHARQ_Msg3Tx =
        p_cc->rach_config_common.max_harq_msg_3_tx;

    rtxDListInit(&p_asn1_cc->rach_ConfigCommon.extElem1);

    /* - bcch_Configuration */
    p_asn1_cc->bcch_Config.modificationPeriodCoeff =
        p_cc->bcch_config.mod_period_coeff;

    /* - pcch_Configuration */
    p_asn1_cc->pcch_Config.defaultPagingCycle =
        p_cc->pcch_config.default_paging_cycle;

    p_asn1_cc->pcch_Config.nB =
        p_cc->pcch_config.nb;

    /* - prach_Configuration */
    p_asn1_cc->prach_Config.rootSequenceIndex =
        p_cc->prach_config_sib.root_seq_index;

    /* -- prach_Configuration/prach_ConfigInfo */
    p_asn1_cc->prach_Config.prach_ConfigInfo.prach_ConfigIndex =
        p_cc->prach_config_sib.prach_config_info.prach_config_index;

    p_asn1_cc->prach_Config.prach_ConfigInfo.highSpeedFlag =
        p_cc->prach_config_sib.prach_config_info.high_speed_flag;

    p_asn1_cc->prach_Config.prach_ConfigInfo.zeroCorrelationZoneConfig =
        p_cc->prach_config_sib.prach_config_info.zero_cor_zone_config;

    p_asn1_cc->prach_Config.prach_ConfigInfo.prach_FreqOffset =
        p_cc->prach_config_sib.prach_config_info.prach_freq_offset;
    
/* - pdsch_Configuration */
    p_asn1_cc->pdsch_ConfigCommon.referenceSignalPower =
        p_cc->pdsch_config_common.ref_signal_power;

    p_asn1_cc->pdsch_ConfigCommon.p_b =
        p_cc->pdsch_config_common.pb;

    /* - pusch_Configuration */

    /* -- pusch_Configuration/pusch_ConfigBasic */
    p_asn1_cc->pusch_ConfigCommon.pusch_ConfigBasic.n_SB =
        p_cc->pusch_config_common.pusch_config_basic.nsb;

    p_asn1_cc->pusch_ConfigCommon.pusch_ConfigBasic.hoppingMode =
        p_cc->pusch_config_common.pusch_config_basic.hopping_mode;

    p_asn1_cc->pusch_ConfigCommon.pusch_ConfigBasic.pusch_HoppingOffset =
        p_cc->pusch_config_common.pusch_config_basic.pusch_hopping_offset;

    p_asn1_cc->pusch_ConfigCommon.pusch_ConfigBasic.enable64QAM =
        p_cc->pusch_config_common.pusch_config_basic.enable_64_qam;

    /* -- pusch_Configuration/ul_ReferenceSignalsPUSCH */
    p_asn1_cc->pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupHoppingEnabled =
        p_cc->pusch_config_common.ul_ref_signals_pusch.group_hopping_enabled;

    p_asn1_cc->pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.groupAssignmentPUSCH =
        p_cc->pusch_config_common.ul_ref_signals_pusch.group_assign_pusch;

    p_asn1_cc->pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.sequenceHoppingEnabled =
        p_cc->pusch_config_common.ul_ref_signals_pusch.seq_hopping_enabled;

    p_asn1_cc->pusch_ConfigCommon.ul_ReferenceSignalsPUSCH.cyclicShift =
        p_cc->pusch_config_common.ul_ref_signals_pusch.cyclic_shift;

    /* - pucch_Configuration */
    p_asn1_cc->pucch_ConfigCommon.deltaPUCCH_Shift =
        p_cc->pucch_config_common.delta_pucch_shift;

    p_asn1_cc->pucch_ConfigCommon.nRB_CQI =
        p_cc->pucch_config_common.n_rb_cqi;

    p_asn1_cc->pucch_ConfigCommon.nCS_AN =
        p_cc->pucch_config_common.n_cs_an;

    p_asn1_cc->pucch_ConfigCommon.n1PUCCH_AN =
        p_cc->pucch_config_common.n_one_pucch_an;
   
    /* - soundingRS_UL_ConfigCommon */
    if (p_cc->sounding_rs_ul_config_common.presence_bitmask &
        SOUNDING_RS_UL_CONFIG_COMMON_SETUP_PRESENCE_FLAG)
    {
        p_asn1_cc->soundingRS_UL_ConfigCommon.t = T_SoundingRS_UL_ConfigCommon_setup;
        p_asn1_cc->soundingRS_UL_ConfigCommon.u.setup =
            rtxMemAllocType(p_asn1_ctx, SoundingRS_UL_ConfigCommon_setup);

        if (PNULL == p_asn1_cc->soundingRS_UL_ConfigCommon.u.setup)
        {
            /* Not enough memory */
            return RRC_FAILURE;
        }

        p_asn1_cc->soundingRS_UL_ConfigCommon.u.setup->m.srs_MaxUpPtsPresent = 0;

        p_asn1_cc->soundingRS_UL_ConfigCommon.u.setup->srs_BandwidthConfig =
            p_cc->sounding_rs_ul_config_common.setup.srs_bw_config;

        p_asn1_cc->soundingRS_UL_ConfigCommon.u.setup->srs_SubframeConfig =
            p_cc->sounding_rs_ul_config_common.setup.srs_subframe_config;

        p_asn1_cc->soundingRS_UL_ConfigCommon.u.setup->
            ackNackSRS_SimultaneousTransmission =
            p_cc->sounding_rs_ul_config_common.setup.ack_nack_srs_simul_trans;

        if(p_cc->sounding_rs_ul_config_common.setup.presence_bitmask &
                SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG)
        {
            /*** TDD changes start ***/
            if (DUPLEXING_MODE_TDD == duplexing_mode)
            {

                p_asn1_cc->soundingRS_UL_ConfigCommon.u.setup->m.srs_MaxUpPtsPresent = 1;
                p_asn1_cc->soundingRS_UL_ConfigCommon.u.setup->srs_MaxUpPts =
                    p_cc->sounding_rs_ul_config_common.setup.srs_max_up_pts;
            }
            else
            {
                /* Re-Setting the bitmask in case of FDD*/
                p_cc->sounding_rs_ul_config_common.setup.presence_bitmask
                    ^=
                    SOUNDING_RS_UL_CONFIG_COMMON_SETUP_SRS_MAX_UP_PTS_PRESENCE_FLAG;

                RRC_CSC_TRACE(RRC_WARNING,"srs_MaxUpPts must be absent for FDD mode");
            }
            /*** TDD changes stop ***/

        }
    }
    else
    {
        p_asn1_cc->soundingRS_UL_ConfigCommon.t =
            T_SoundingRS_UL_ConfigCommon_release;
    }

    /* - uplinkPowerControl */
    p_asn1_cc->uplinkPowerControlCommon.p0_NominalPUSCH =
        p_cc->uplink_power_control_common.p_zero_nominal_Pusch;

    p_asn1_cc->uplinkPowerControlCommon.alpha =
        p_cc->uplink_power_control_common.alpha;

    p_asn1_cc->uplinkPowerControlCommon.p0_NominalPUCCH =
        p_cc->uplink_power_control_common.p_zero_nominal_pucch;

    p_asn1_cc->uplinkPowerControlCommon.deltaPreambleMsg3 =
        p_cc->uplink_power_control_common.delta_preamble_msg_three;

    /* -- uplinkPowerControl/deltaFList_PUCCH */
    p_asn1_cc->uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1 =
        p_cc->uplink_power_control_common.delta_f_list_pucch.
            delta_f_pucch_format_one;

    p_asn1_cc->uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format1b =
        p_cc->uplink_power_control_common.delta_f_list_pucch.
            delta_f_pucch_format_one_b;

    p_asn1_cc->uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2 =
        p_cc->uplink_power_control_common.delta_f_pucch_format_two.
            delta_f_pucch_format_two;

    p_asn1_cc->uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2a =
        p_cc->uplink_power_control_common.delta_f_pucch_format_two.
            delta_f_pucch_format_two_a;

    p_asn1_cc->uplinkPowerControlCommon.deltaFList_PUCCH.deltaF_PUCCH_Format2b =
        p_cc->uplink_power_control_common.delta_f_pucch_format_two.
            delta_f_pucch_format_two_b;

    p_asn1_cc->ul_CyclicPrefixLength =
        p_cc->ul_cyc_prefix_len;

    if (p_cc->presence_bitmask & RRC_CONFIG_COMMON_SIB_POWER_CONTROL_CMN_V1020)
    {
       p_asn1_cc->m._v2ExtPresent = 1;
       p_asn1_cc->m.uplinkPowerControlCommon_v1020Present = 1;

       p_asn1_cc->uplinkPowerControlCommon_v1020.deltaF_PUCCH_Format3_r10 =
           p_cc->uplink_power_control_common_v1020.pucch_format3_r10;

       p_asn1_cc->uplinkPowerControlCommon_v1020.deltaF_PUCCH_Format1bCS_r10 = 
           p_cc->uplink_power_control_common_v1020.pucch_format1_bcs_r10;

    }

    rtxDListInit(&p_asn1_cc->extElem1);

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: fill_subframe_config
*   INPUT        : OSCTXT                          *p_asn1_ctx
*                  MBSFN_SubframeConfigList        *p_asn1_sc
*                  mbsfn_subframe_config_list_t    *p_sc
*   OUTPUT       : none
*   DESCRIPTION  : This function converts mbsfn_subframe_config_list_t to
*                  MBSFN_SubframeConfiguration
*                  and checks whether input data are valid.
*
*   RETURNS      : RRC_SUCCESS - all data are valid
*                  RRC_FAILURE - data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et fill_subframe_config
(
    OSCTXT                          *p_asn1_ctx,    /* for memory allocation */
    MBSFN_SubframeConfigList        *p_asn1_sc,     /* destination */
    mbsfn_subframe_config_list_t    *p_sc           /* source */
)
{
    rrc_counter_t subframe_index                        = RRC_NULL;
    MBSFN_SubframeConfig                *p_asn1_sc_elem = PNULL;
    OSRTDListNode *p_asn1_list_node                     = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_asn1_sc);
    RRC_ASSERT(PNULL != p_sc);

    if ((p_sc->count < 1) || (p_sc->count > ARRSIZE(p_sc->
        mbsfn_subframe_config)) || (p_sc->count > ASN1V_maxMBSFN_Allocations))
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Incorrect subframe config list count=%u. Should be 1..min(RRM=%u,"
                " ASN.1=%u)",
            p_sc->count,
            ARRSIZE(p_sc->mbsfn_subframe_config),
            ASN1V_maxMBSFN_Allocations);
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* Fill MBSFN_SubframeConfiguration */
    rtxDListInit(p_asn1_sc);

    for (subframe_index = 0; subframe_index < p_sc->count; subframe_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, MBSFN_SubframeConfig,
            &p_asn1_list_node, &p_asn1_sc_elem);

        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            return RRC_FAILURE;
        }
        asn1Init_MBSFN_SubframeConfig(p_asn1_sc_elem);  
        /* Fill MBSFN_SubframeConfiguration_element */
        p_asn1_sc_elem->radioframeAllocationPeriod =
            p_sc->mbsfn_subframe_config[subframe_index].
                radio_frame_alloc_period;

        p_asn1_sc_elem->radioframeAllocationOffset =
            p_sc->mbsfn_subframe_config[subframe_index].
                radio_frame_alloc_offset;

        /* - subframeAllocation */

        if (SUBFRAME_ALLOCATION_ONE_FRAME == p_sc->
        mbsfn_subframe_config[subframe_index].subframe_allocation.
            presence_bitmask)
        {
            p_asn1_sc_elem->subframeAllocation.t =
                T_MBSFN_SubframeConfig_subframeAllocation_oneFrame;
            p_asn1_sc_elem->subframeAllocation.u.oneFrame =
                rtxMemAllocType(p_asn1_ctx,
                    ASN1BitStr32);
            if(p_asn1_sc_elem->subframeAllocation.u.oneFrame == PNULL)
            {
                return RRC_FAILURE;
            }
            memset_wrapper(p_asn1_sc_elem->subframeAllocation.u.oneFrame, RRC_NULL, sizeof(ASN1BitStr32));


            p_asn1_sc_elem->subframeAllocation.u.oneFrame->numbits =
                ONE_FRAME_NBITS;
            l3_memcpy_wrapper(p_asn1_sc_elem->subframeAllocation.u.oneFrame->data,
                p_sc->mbsfn_subframe_config[subframe_index].subframe_allocation.
                    one_frame,
                sizeof(p_sc->mbsfn_subframe_config[subframe_index].
                    subframe_allocation.one_frame));
        }
        else if (SUBFRAME_ALLOCATION_FOUR_FRAMES == p_sc->
        mbsfn_subframe_config[subframe_index].subframe_allocation.
            presence_bitmask)
        {
            p_asn1_sc_elem->subframeAllocation.t =
                T_MBSFN_SubframeConfig_subframeAllocation_fourFrames;
            p_asn1_sc_elem->subframeAllocation.u.fourFrames =
                rtxMemAllocType(p_asn1_ctx,
                    ASN1BitStr32);
            if(p_asn1_sc_elem->subframeAllocation.u.fourFrames == PNULL)
            {
                return RRC_FAILURE;
            }
            memset_wrapper(p_asn1_sc_elem->subframeAllocation.u.fourFrames, RRC_NULL, sizeof(ASN1BitStr32));

            p_asn1_sc_elem->subframeAllocation.u.fourFrames->numbits =
                FOUR_FRAME_NBITS;
            l3_memcpy_wrapper(p_asn1_sc_elem->subframeAllocation.u.fourFrames->data,
                p_sc->mbsfn_subframe_config[subframe_index].subframe_allocation.
                    four_frames,
                sizeof(p_sc->mbsfn_subframe_config[subframe_index].
                    subframe_allocation.four_frames));
        }
        else
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Incorrect subframe_allocation.presence_bitmask=%u",
                p_sc->mbsfn_subframe_config[subframe_index].subframe_allocation.
                    presence_bitmask);
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        rtxDListAppendNode(p_asn1_sc, p_asn1_list_node);
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib2
*   INPUT        : OSCTXT                      *p_asn1_ctx
*                  SystemInformationBlockType2 *p_asn1_sib2
*                  sib_type_2_Info_t           *p_sib2
*                  sib_type_1_Info_t           *p_sib1
*   OUTPUT       : none
*   DESCRIPTION  : This function converts sib_type_2_Info_t to
*                  SystemInformationBlockType2
*                  and checks whether input data are valid.
*
*   RETURNS      : RRC_SUCCESS - all data are valid
*                  RRC_FAILURE - data are invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib2
(
 OSCTXT                      *p_asn1_ctx,    /* for memory allocation */
 SystemInformationBlockType2 *p_asn1_sib2,   /* destination */
 /*** TDD changes start ***/
 sib_type_2_Info_t           *p_sib2,         /* source */
 sib_type_1_Info_t           *p_sib1         /* For TDD configuration */
 /*** TDD changes stop ***/

 )
{
	rrc_return_et ret_value = RRC_SUCCESS;
	/*** TDD changes start ***/
	duplexing_mode_et duplexing_mode = DUPLEXING_MODE_FDD;
	/*** TDD changes stop ***/

	/* CR 970 changes start*/
	SystemInformationBlockType2_v8h0_IEs    sib_2_v8h0_ies;
	U8      index = RRC_NULL;
	OSCTXT  asn1_ctx;
	U32     encodedmsg_len = 0;
	U8      late_non_critical_extension_buff[MAX_ASN_BUFF_LENGTH];
	S32     enc_res = RRC_NULL;

	memset_wrapper(&sib_2_v8h0_ies, RRC_NULL, sizeof(SystemInformationBlockType2_v8h0_IEs));
	memset_wrapper(late_non_critical_extension_buff, RRC_NULL, 
			sizeof(late_non_critical_extension_buff));
	/* CR 970 changes stop*/

	RRC_CSC_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_asn1_ctx);
	RRC_ASSERT(PNULL != p_sib2);
	/*** TDD changes start ***/
	RRC_ASSERT(PNULL != p_sib1);
	/*** TDD changes stop ***/

	RRC_ASSERT(PNULL != p_asn1_sib2);

	/*Init*/
	asn1Init_SystemInformationBlockType2(p_asn1_sib2);

	p_asn1_sib2->m.ac_BarringInfoPresent      = 0;
	p_asn1_sib2->m.mbsfn_SubframeConfigListPresent = 0;
	p_asn1_sib2->m._v3ExtPresent = 0;
	p_asn1_sib2->m.ssac_BarringForMMTEL_Voice_r9Present = 0;
	p_asn1_sib2->m.ssac_BarringForMMTEL_Video_r9Present = 0;

	/* - accessBarringInformation */

	if (SIB2_ACCESS_BARR_INFO_PRESENCE_FLAG & p_sib2->presence_bitmask)
	{
		p_asn1_sib2->m.ac_BarringInfoPresent = 1;

		p_asn1_sib2->ac_BarringInfo.m.ac_BarringForMO_SignallingPresent       =
			0;
		p_asn1_sib2->ac_BarringInfo.m.ac_BarringForMO_DataPresent             =
			0;

		if (p_sib2->access_barring_info.acBarringForEmergency > RRC_TRUE)
		{
			RRC_CSC_TRACE(RRC_WARNING,
					"Incorrect p_sib2->access_barring_info.acBarringForEmergency "
					"value=%u received",
					p_sib2->access_barring_info.acBarringForEmergency);
			RRC_CSC_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
		p_asn1_sib2->ac_BarringInfo.ac_BarringForEmergency =
			p_sib2->access_barring_info.acBarringForEmergency;

		if (AC_BARR_INFO_MO_SIG_PRESENCE_FLAG & p_sib2->access_barring_info.
				presence_bitmask)
		{
			p_asn1_sib2->ac_BarringInfo.m.ac_BarringForMO_SignallingPresent = 1;

			ret_value = fill_access_class_barring_info(
					&p_asn1_sib2->ac_BarringInfo.ac_BarringForMO_Signalling,
					&p_sib2->access_barring_info.ac_barring_for_mo_signalling);

			if (RRC_FAILURE == ret_value)
			{
				RRC_CSC_UT_TRACE_EXIT();
				return RRC_FAILURE;
			}
		}

		if (AC_BARR_INFO_MO_DATA_PRESENCE_FLAG & p_sib2->access_barring_info.
				presence_bitmask)
		{
			p_asn1_sib2->ac_BarringInfo.m.ac_BarringForMO_DataPresent = 1;

			ret_value = fill_access_class_barring_info(
					&p_asn1_sib2->ac_BarringInfo.ac_BarringForMO_Data,
					&p_sib2->access_barring_info.ac_barring_for_mo_data);

			if (RRC_FAILURE == ret_value)
			{
				RRC_CSC_UT_TRACE_EXIT();
				return RRC_FAILURE;
			}
		}
	}

	/*** TDD changes start ***/
	if(SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG & p_sib1->presence_bitmask)
	{
		duplexing_mode = DUPLEXING_MODE_TDD;
	}
	/* - radioResourceConfigCommon */
	ret_value = fill_radio_resource_config_common(p_asn1_ctx,
			&p_asn1_sib2->radioResourceConfigCommon, &p_sib2->
			radio_resource_config_common_sib,duplexing_mode);

	if (RRC_FAILURE == ret_value)
	{
		RRC_CSC_UT_TRACE_EXIT();
		return RRC_FAILURE;
	}
	/*** TDD changes stop ***/


	/* - ue_TimersAndConstants */

	p_asn1_sib2->ue_TimersAndConstants.t300 = p_sib2->ue_timers_and_constants.
		timer_300;
	p_asn1_sib2->ue_TimersAndConstants.t301 = p_sib2->ue_timers_and_constants.
		timer_301;
	p_asn1_sib2->ue_TimersAndConstants.t310 = p_sib2->ue_timers_and_constants.
		timer_310;
	p_asn1_sib2->ue_TimersAndConstants.n310 = p_sib2->ue_timers_and_constants.
		timer_n310;
	p_asn1_sib2->ue_TimersAndConstants.t311 = p_sib2->ue_timers_and_constants.
		timer_311;
	p_asn1_sib2->ue_TimersAndConstants.n311 = p_sib2->ue_timers_and_constants.
		timer_n311;

	rtxDListInit(&p_asn1_sib2->ue_TimersAndConstants.extElem1);

	/* - frequencyInformation */

	p_asn1_sib2->freqInfo.m.ul_CarrierFreqPresent    = 0;
	p_asn1_sib2->freqInfo.m.ul_BandwidthPresent = 0;

	/*** TDD changes start ***/

	if (SIB_TYPE_1_TDD_CONFIG_PRESENT_FLAG & p_sib1->presence_bitmask )
	{
		if (FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG & p_sib2->freq_info.
				presence_bitmask &&
				FREQ_INFO_UL_BW_PRESENCE_FLAG & p_sib2->freq_info.presence_bitmask)
		{
			RRC_CSC_TRACE(RRC_WARNING,"ul-CarrierFreq and ul-Bandwidth must be"\
					"absent for TDD");
		}
		else
		{
			RRC_CSC_TRACE(RRC_INFO,"ul-CarrierFreq and ul-Bandwidth absent for TDD");
		}
	}
	else
	{
		if (FREQ_INFO_UL_CARRER_FREQ_PRESENCE_FLAG & p_sib2->freq_info.
				presence_bitmask)
		{
			p_asn1_sib2->freqInfo.m.ul_CarrierFreqPresent = 1;
			p_asn1_sib2->freqInfo.ul_CarrierFreq = p_sib2->freq_info.
				ul_carrier_freq;
		}

		if (FREQ_INFO_UL_BW_PRESENCE_FLAG & p_sib2->freq_info.presence_bitmask)
		{
			p_asn1_sib2->freqInfo.m.ul_BandwidthPresent = 1;
			p_asn1_sib2->freqInfo.ul_Bandwidth = p_sib2->freq_info.ul_bandwidth;
		}
	}

	p_asn1_sib2->freqInfo.additionalSpectrumEmission =
		p_sib2->freq_info.add_spectrum_emission;

	/* - mbsfn_SubframeConfiguration */
	if (SIB2_MBSFN_SUBFRAME_CONF_LIST_PRESENCE_FLAG & p_sib2->presence_bitmask)
	{
		p_asn1_sib2->m.mbsfn_SubframeConfigListPresent = 1;

		ret_value = fill_subframe_config(p_asn1_ctx,
				&p_asn1_sib2->mbsfn_SubframeConfigList, &p_sib2->
				mbsfn_subframe_config_list);

		if (RRC_FAILURE == ret_value)
		{
			RRC_CSC_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
	}

	p_asn1_sib2->timeAlignmentTimerCommon = p_sib2->time_align_timer;

	/*cr_345 change*/
	if (SIB2_SSAC_BARRING_FOR_MMTEL_VOICE_PRESENCE_FLAG & 
			p_sib2->presence_bitmask)
	{
		p_asn1_sib2->m._v3ExtPresent = 1;
		p_asn1_sib2->m.ssac_BarringForMMTEL_Voice_r9Present = 1;

		ret_value = fill_access_class_barring_info(
				&p_asn1_sib2->ssac_BarringForMMTEL_Voice_r9,
				&p_sib2->ssac_barring_for_mmtel_voice_r9);

		if (RRC_FAILURE == ret_value)
		{
			RRC_CSC_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
	}

	if (SIB2_SSAC_BARRING_FOR_MMTEL_VIDEO_PRESENCE_FLAG & 
			p_sib2->presence_bitmask)
	{
		p_asn1_sib2->m._v3ExtPresent = 1;
		p_asn1_sib2->m.ssac_BarringForMMTEL_Video_r9Present = 1;

		ret_value = fill_access_class_barring_info(
				&p_asn1_sib2->ssac_BarringForMMTEL_Video_r9,
				&p_sib2->ssac_barring_for_mmtel_video_r9);

		if (RRC_FAILURE == ret_value)
		{
			RRC_CSC_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
	}
	/* CR 477 Changes Start */
	if (SIB2_AC_BARRING_FOR_CSFB_PRESENCE_FLAG &
			p_sib2->presence_bitmask)
	{
		p_asn1_sib2->m._v4ExtPresent = 1;
		p_asn1_sib2->m.ac_BarringForCSFB_r10Present = 1;

		ret_value = fill_access_class_barring_info(
				&p_asn1_sib2->ac_BarringForCSFB_r10,
				&p_sib2->ac_barring_for_csfb_r10);

		if (RRC_FAILURE == ret_value)
		{
			RRC_CSC_UT_TRACE_EXIT();
			return RRC_FAILURE;
		}
	}
	/* CR 477 Changes End */

	/* CR 970 changes start*/
	if (SIB2_LATE_NON_CRITICAL_EXTENSION_PRESENT &
			p_sib2->presence_bitmask)
	{
		p_asn1_sib2->m.lateNonCriticalExtensionPresent = 1;

		if (p_sib2->sib_type_2_v8h0_ies.bitmask &
				SIB_2_MULTI_BAND_INFO_LIST_PRESENCE_FLAG)
		{
			sib_2_v8h0_ies.m.multiBandInfoListPresent = 1;
			sib_2_v8h0_ies.m.nonCriticalExtensionPresent = 0;

			sib_2_v8h0_ies.multiBandInfoList.n = p_sib2->sib_type_2_v8h0_ies.
				multi_band_info_list.count;

			for (index = 0; index < sib_2_v8h0_ies.multiBandInfoList.n; index++)
			{
				sib_2_v8h0_ies.multiBandInfoList.elem[index] = p_sib2->
					sib_type_2_v8h0_ies.multi_band_info_list.add_spectrum_emission[index];
			}
		}

		if (RT_OK != rtInitContext(&asn1_ctx))
		{
			RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
			return RRC_FAILURE;
		}

		pu_setBuffer(&asn1_ctx, late_non_critical_extension_buff,
				MAX_ASN_BUFF_LENGTH, ASN_RRC_BUF_TYPE);

		enc_res = asn1PE_SystemInformationBlockType2_v8h0_IEs(&asn1_ctx, &sib_2_v8h0_ies);
		if(RT_OK !=  enc_res)
		{
			RRC_CSC_TRACE(RRC_ERROR,
					"ASN1 encoding failed.");
		}
		else
		{
			encodedmsg_len = (U32)pe_GetMsgLen(&asn1_ctx);
			p_asn1_sib2->lateNonCriticalExtension.data = (OSOCTET*)rtxMemAlloc(p_asn1_ctx, 
					encodedmsg_len);
            /* SPR 12964 Fix Start */
            if(PNULL == p_asn1_sib2->lateNonCriticalExtension.data)
            {
                RRC_CSC_TRACE(RRC_ERROR, "Unable to allocate memory.");
                rtFreeContext(&asn1_ctx);
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;

            }
            /* SPR 12964 Fix Stop */

			l3_memcpy_wrapper((void *)p_asn1_sib2->lateNonCriticalExtension.data, 
					late_non_critical_extension_buff , encodedmsg_len);

			p_asn1_sib2->lateNonCriticalExtension.numocts = encodedmsg_len;
		}
		/*valgrind_fix start*/
		rtFreeContext(&asn1_ctx);
		/*valgrind_fix stop*/
	}
	/* CR 970 changes stop*/

	rtxDListInit(&p_asn1_sib2->extElem1);



	RRC_CSC_UT_TRACE_EXIT();
	return ret_value;
}

/******************************************************************************
*   FUNCTION NAME: check_and_convert_physical_cell_identity_and_range
*   INPUT        : OSCTXT                          *pctxt
*                  PhysCellIdRange                 *p_asn1_data
*                  phy_cell_id_range_t             *p_data
*   OUTPUT       : none
*   DESCRIPTION  : This function convert phy_cell_id_range_t to
*                  PhysCellIdRange
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et check_and_convert_physical_cell_identity_and_range
(
    PhysCellIdRange                 *p_asn1_data,   /* destination */
    phy_cell_id_range_t             *p_data         /* source */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    p_asn1_data->m.rangePresent = 0;
    p_asn1_data->start = p_data->start;

    if (p_data->presence_bitmask & PHY_CELL_ID_RANGE)
    {
        p_asn1_data->m.rangePresent = 1;
        p_asn1_data->range = p_data->range;
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib3
*   INPUT        : OSCTXT                      *pctxt
*                  SystemInformationBlockType3 *p_asn1_sib3
*                  sib_type_3_Info_t           *p_sib3
*   OUTPUT       : none
*   DESCRIPTION  : This function convert sib_type_3_Info_t to
*                  SystemInformationBlockType3
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib3
(
    SystemInformationBlockType3 *p_asn1_sib3,   /* destination */
    sib_type_3_Info_t           *p_sib3         /* source */
)
{
    /*SPR 4355 Start*/
    U8                          neigh_cell_config;   
    /*SPR 4355 End*/

    RRC_CSC_UT_TRACE_ENTER();

    if ((PNULL == p_asn1_sib3) || (PNULL == p_sib3))
    {
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /*Init*/
    asn1Init_SystemInformationBlockType3(p_asn1_sib3);

    /*cellReselectionInfoCommon*/
    p_asn1_sib3->cellReselectionInfoCommon.m.speedStateReselectionParsPresent =
        0;

    p_asn1_sib3->cellReselectionInfoCommon.q_Hyst =
        p_sib3->timecell_reselect_info_comm.q_hyst;

    if (p_sib3->timecell_reselect_info_comm.presence_bitmask &
        CELL_RESELECT_SPEED_DEPEND_RESELECT_PRESENCE_FLAG)
    {
        p_asn1_sib3->cellReselectionInfoCommon.m.
            speedStateReselectionParsPresent = 1;

        p_asn1_sib3->cellReselectionInfoCommon.speedStateReselectionPars.
            mobilityStateParameters.t_Evaluation =
            p_sib3->timecell_reselect_info_comm.speed_depend_reselect.
                mobility_state_params.t_evaluation;
        if (RRM_RRC_T_EVAL_S_240 < p_sib3->timecell_reselect_info_comm.
                speed_depend_reselect.mobility_state_params.t_evaluation)
        {
            RRC_CSC_TRACE(RRC_WARNING,
                    "Value of t_evaluation=%u ",
                    p_sib3->timecell_reselect_info_comm.
                    speed_depend_reselect.mobility_state_params.t_evaluation);
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }   

        p_asn1_sib3->cellReselectionInfoCommon.speedStateReselectionPars.
            mobilityStateParameters.t_HystNormal =
            p_sib3->timecell_reselect_info_comm.speed_depend_reselect.
                mobility_state_params.t_hyst_normal;

        p_asn1_sib3->cellReselectionInfoCommon.speedStateReselectionPars.
            mobilityStateParameters.n_CellChangeMedium =
            p_sib3->timecell_reselect_info_comm.speed_depend_reselect.
                mobility_state_params.n_cell_change_medium;

        p_asn1_sib3->cellReselectionInfoCommon.speedStateReselectionPars.
            mobilityStateParameters.n_CellChangeHigh =
            p_sib3->timecell_reselect_info_comm.speed_depend_reselect.
                mobility_state_params.n_cell_change_high;

        /* speedDependentScalingParametersHyst */
        p_asn1_sib3->cellReselectionInfoCommon.speedStateReselectionPars.
            q_HystSF.sf_Medium =
            p_sib3->timecell_reselect_info_comm.speed_depend_reselect.
                speed_depend_scaling_param_hyst.q_hyst_sf_medium;

        p_asn1_sib3->cellReselectionInfoCommon.speedStateReselectionPars.
            q_HystSF.sf_High =
            p_sib3->timecell_reselect_info_comm.speed_depend_reselect.
                speed_depend_scaling_param_hyst.q_hyst_sf_high;
    }

    /* cellReselectionServingFreqInfo */
    p_asn1_sib3->cellReselectionServingFreqInfo.m.s_NonIntraSearchPresent
        = 0;

    if (p_sib3->cell_reselect_serv_freq_info.presence_bitmask &
        CELL_RESELECT_NON_INTRA_SEARCH_PRESENCE_FLAG)
    {
        p_asn1_sib3->cellReselectionServingFreqInfo.m.s_NonIntraSearchPresent =
            1;
        p_asn1_sib3->cellReselectionServingFreqInfo.s_NonIntraSearch =
            p_sib3->cell_reselect_serv_freq_info.s_non_intra_search;
    }

    p_asn1_sib3->cellReselectionServingFreqInfo.threshServingLow =
        p_sib3->cell_reselect_serv_freq_info.thresh_serving_low;

    p_asn1_sib3->cellReselectionServingFreqInfo.cellReselectionPriority =
        p_sib3->cell_reselect_serv_freq_info.cell_reselect_priority;

    /* intraFreqCellReselectionInfo */
    p_asn1_sib3->intraFreqCellReselectionInfo.m.p_MaxPresent
        = 0;
    p_asn1_sib3->intraFreqCellReselectionInfo.m.s_IntraSearchPresent
        = 0;
    p_asn1_sib3->intraFreqCellReselectionInfo.m.allowedMeasBandwidthPresent
        = 0;
    p_asn1_sib3->intraFreqCellReselectionInfo.m.t_ReselectionEUTRA_SFPresent
        = 0;

    p_asn1_sib3->intraFreqCellReselectionInfo.q_RxLevMin =
        p_sib3->intra_freq_cell_reselect_info.q_rx_lev_min;

    if (p_sib3->intra_freq_cell_reselect_info.presence_bitmask &
        INTRA_FREQ_CELL_RESELECT_P_MAX_PRESENCE_FLAG)
    {
        p_asn1_sib3->intraFreqCellReselectionInfo.m.p_MaxPresent = 1;
        p_asn1_sib3->intraFreqCellReselectionInfo.p_Max =
            p_sib3->intra_freq_cell_reselect_info.p_max;
    }

    if (p_sib3->intra_freq_cell_reselect_info.presence_bitmask &
        INTRA_FREQ_CELL_RESELECT_INTRA_SEARCH_PRESENCE_FLAG)
    {
        p_asn1_sib3->intraFreqCellReselectionInfo.m.s_IntraSearchPresent = 1;
        p_asn1_sib3->intraFreqCellReselectionInfo.s_IntraSearch =
            p_sib3->intra_freq_cell_reselect_info.s_intra_search;
    }

    if (p_sib3->intra_freq_cell_reselect_info.presence_bitmask &
        INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRESENCE_FLAG)
    {
        p_asn1_sib3->intraFreqCellReselectionInfo.m.allowedMeasBandwidthPresent
            = 1;
        p_asn1_sib3->intraFreqCellReselectionInfo.allowedMeasBandwidth =
            p_sib3->intra_freq_cell_reselect_info.measure_bw;
    }

    p_asn1_sib3->intraFreqCellReselectionInfo.presenceAntennaPort1 =
        p_sib3->intra_freq_cell_reselect_info.presence_antenna_port_1;

    p_asn1_sib3->intraFreqCellReselectionInfo.neighCellConfig.numbits =
        N_CELL_CFG_NBITS;

    /*SPR 4355 Start*/
    neigh_cell_config = p_sib3->intra_freq_cell_reselect_info.neigh_cell_config << 6;
    p_asn1_sib3->intraFreqCellReselectionInfo.neighCellConfig.data[0] =
        neigh_cell_config & 0xc0; /* 2 bits */
    /*SPR 4355 End*/

    p_asn1_sib3->intraFreqCellReselectionInfo.t_ReselectionEUTRA =
        p_sib3->intra_freq_cell_reselect_info.t_reselec_eutra;

    if (p_sib3->intra_freq_cell_reselect_info.presence_bitmask &
        INTRA_FREQ_CELL_RESELECT_EUTRAN_SF_PRESENCE_FLAG)
    {
        p_asn1_sib3->intraFreqCellReselectionInfo.m.t_ReselectionEUTRA_SFPresent
            = 1;

        p_asn1_sib3->intraFreqCellReselectionInfo.t_ReselectionEUTRA_SF.
            sf_Medium =
            p_sib3->intra_freq_cell_reselect_info.t_reselect_eutra_sf.sf_medium;

        p_asn1_sib3->intraFreqCellReselectionInfo.t_ReselectionEUTRA_SF.sf_High
            =
            p_sib3->intra_freq_cell_reselect_info.t_reselect_eutra_sf.sf_high;
    }
    /*cr_398:Cell reselection enhancements*/
   
     /*Fill SystemInformationBlockType3_s_IntraSearch_v920*/
    if (p_sib3->presence_bitmask &
        SIB_3_INTRA_SEARCH_V920_PRESENCE_FLAG)
    {
      p_asn1_sib3->m._v3ExtPresent = 1;   
      p_asn1_sib3->m.s_IntraSearch_v920Present = 1;
      p_asn1_sib3->s_IntraSearch_v920.s_IntraSearchP_r9 = 
          p_sib3->sib3_intra_search_v920.s_intra_search_p_r9;
      
      p_asn1_sib3->s_IntraSearch_v920.s_IntraSearchQ_r9 = 
          p_sib3->sib3_intra_search_v920.s_intra_search_q_r9;
    }

    /*Fill SystemInformationBlockType3_s_NonIntraSearch_v920*/
    if (p_sib3->presence_bitmask &
        SIB_3_NON_INTRA_SEARCH_V920_PRESENCE_FLAG)
    {
      p_asn1_sib3->m._v3ExtPresent = 1;   
      p_asn1_sib3->m.s_NonIntraSearch_v920Present = 1;
      p_asn1_sib3->s_NonIntraSearch_v920.s_NonIntraSearchP_r9 = 
          p_sib3->sib3_non_intra_search_v920.s_non_intra_search_p_r9;
      
      p_asn1_sib3->s_NonIntraSearch_v920.s_NonIntraSearchQ_r9 = 
          p_sib3->sib3_non_intra_search_v920.s_non_intra_search_q_r9;
    }
    
    /*Fill Q_QualMin_r9*/
    if (p_sib3->presence_bitmask &
        SIB_3_Q_QUAL_MIN_R9_PRESENCE_FLAG)
    {
      p_asn1_sib3->m._v3ExtPresent = 1;   
      p_asn1_sib3->m.q_QualMin_r9Present = 1;
      p_asn1_sib3->q_QualMin_r9 = p_sib3->q_qual_min_r9;
    }
    
    /*Fill threshServingLowQ_r9*/
    if (p_sib3->presence_bitmask &
        SIB_3_THRESH_SERVING_LOW_Q_R9_PRESENCE_FLAG)
    {
      p_asn1_sib3->m._v3ExtPresent = 1;   
      p_asn1_sib3->m.threshServingLowQ_r9Present = 1;
      p_asn1_sib3->threshServingLowQ_r9 = p_sib3->thresh_serving_low_q_r9;
    }
    
    rtxDListInit(&p_asn1_sib3->extElem1);

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

#ifdef LTE_EMBMS_SUPPORTED
/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib13
*   INPUT        : OSCTXT                               *pctxt
*                  SystemInformationBlockType13_r9      *p_asn1_sib13
*                  sib_type_13_Info_t                   *p_sib13 
*   OUTPUT       : none               
*   DESCRIPTION  : This function convert sib_type_13_Info_t to
*                  SystemInformationBlockType13
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
/*
MBMS-NotificationConfig-r9 ::= SEQUENCE {
    notificationRepetitionCoeff-r9 ENUMERATED {n2, n4},
    notificationOffset-r9 INTEGER (0..10),
    notificationSF-Index-r9 INTEGER (1..6)
}
MBSFN-AreaInfoList-r9 ::= SEQUENCE (SIZE(1..maxMBSFN-Area)) OF MBSFN-AreaInfo-r9
MBSFN-AreaInfo-r9 ::= SEQUENCE {
    mbsfn-AreaId-r9 INTEGER (0..255),
    non-MBSFNregionLength ENUMERATED {s1, s2},
    notificationIndicator-r9 INTEGER (0..7),
    mcch-Config-r9 SEQUENCE {
        mcch-RepetitionPeriod-r9 ENUMERATED {rf32, rf64, rf128, rf256},
        mcch-Offset-r9 INTEGER (0..10),
        mcch-ModificationPeriod-r9 ENUMERATED {rf512, rf1024},
        sf-AllocInfo-r9 BIT STRING (SIZE(6)),
        signallingMCS-r9 ENUMERATED {n2, n7, n13, n19}
    },
    ...
}
SystemInformationBlockType13-r9 ::= SEQUENCE {
    mbsfn-AreaInfoList-r9 MBSFN-AreaInfoList-r9,
    notificationConfig-r9 MBMS-NotificationConfig-r9,
    lateNonCriticalExtension OCTET STRING OPTIONAL, -- Need OP
    ...
}*/
rrc_return_et bch_m_check_and_convert_sib13
(
    OSCTXT                           *pctxt,         /* for memory allocation */
    SystemInformationBlockType13_r9  *p_asn1_sib13,   /* destination */
    sib_type_13_info_t               *p_sib13         /* source */
)
{
    rrc_return_et        result                 = RRC_SUCCESS; 
    OSRTDListNode       *p_node                 = PNULL;
    MBSFN_AreaInfo_r9   *p_mbsfn_area_info_elem = PNULL;
    U8                   index        = RRC_NULL;

    RRC_CSC_UT_TRACE_ENTER();

    /*Init*/
    asn1Init_SystemInformationBlockType13_r9(p_asn1_sib13);

    rtxDListInit(&p_asn1_sib13->mbsfn_AreaInfoList_r9);

    /*composing  MBSFN-AreaInfoList-r9*/
    for(index = RRC_NULL ; 
            index < p_sib13->num_valid_mbsfn_area_info; 
            index++)
    {
        /*Allocating Memory for Node and Data*/
        rtxDListAllocNodeAndData(pctxt, 
                MBSFN_AreaInfo_r9, 
                &p_node,
                &p_mbsfn_area_info_elem);

        if(PNULL == p_node)
        {
            RRC_CSC_TRACE(RRC_FATAL,"ASN:Memory Allocation failed");
            result = RRC_FAILURE;
            RRC_CSC_UT_TRACE_EXIT();
            return result;

        }
        /* Intializing MBSFN_AreaInfo*/
        asn1Init_MBSFN_AreaInfo_r9(p_mbsfn_area_info_elem);

        /*composing mbsfn-AreaId-r9 */
        p_mbsfn_area_info_elem->mbsfn_AreaId_r9 = 
            p_sib13->m2ap_mbsfn_area_info_list[index].area_id;

        /*composing non-MBSFNregionLength*/
        p_mbsfn_area_info_elem->non_MBSFNregionLength  = 
            p_sib13->m2ap_mbsfn_area_info_list[index].pdcch_length;

        /*Composing notificationIndicator-r9*/
        p_mbsfn_area_info_elem->notificationIndicator_r9  = 
            p_sib13->m2ap_mbsfn_area_info_list[index].notification_indicator_r9;

        /*composing mcch-RepetitionPeriod-r9*/
        p_mbsfn_area_info_elem->mcch_Config_r9.mcch_RepetitionPeriod_r9  = 
            p_sib13->m2ap_mbsfn_area_info_list[index].mcch_repetition_period;

        /*composing mcch-Offset-r9*/
        p_mbsfn_area_info_elem->mcch_Config_r9.mcch_Offset_r9 = 
            p_sib13->m2ap_mbsfn_area_info_list[index].mcch_offset;

        /*composing mcch-ModificationPeriod-r9*/
        p_mbsfn_area_info_elem->mcch_Config_r9.mcch_ModificationPeriod_r9 =
            p_sib13->m2ap_mbsfn_area_info_list[index].mcch_modification_period;

        /*composing sf-AllocInfo-r9*/
        p_mbsfn_area_info_elem->mcch_Config_r9.sf_AllocInfo_r9.numbits = 
            RRC_SIX;
        
        p_mbsfn_area_info_elem->mcch_Config_r9.sf_AllocInfo_r9.data[RRC_ZERO]=
            p_sib13->m2ap_mbsfn_area_info_list[index].sf_alloc_info;

        /*composing signallingMCS-r9*/ 
        p_mbsfn_area_info_elem->mcch_Config_r9.signallingMCS_r9 =
            p_sib13->m2ap_mbsfn_area_info_list[index].signalling_mcs;

        rtxDListAppendNode(&p_asn1_sib13->mbsfn_AreaInfoList_r9, p_node);

    }
    /*Composing MBMS-NotificationConfig-r9*/
    p_asn1_sib13->notificationConfig_r9.notificationRepetitionCoeff_r9 =
        p_sib13->m2ap_notification_config.notification_repetition_coeff;

    /*composing notificationOffset-r9*/
    p_asn1_sib13->notificationConfig_r9.notificationOffset_r9 =
        p_sib13->m2ap_notification_config.notification_offset;

    /*composing notificationSF-Index-r9*/
    p_asn1_sib13->notificationConfig_r9.notificationSF_Index_r9 =
        p_sib13->m2ap_notification_config.notification_sf_Index;


    RRC_CSC_UT_TRACE_EXIT();
    return result;
}
#endif

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib4
*   INPUT        : OSCTXT                      *pctxt
*                  SystemInformationBlockType4 *p_asn1_sib4
*                  sib_type_4_Info_t           *p_sib4 
*   OUTPUT       : none               
*   DESCRIPTION  : This function convert sib_type_4_Info_t to
*                  SystemInformationBlockType4
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
/*
SystemInformationBlockType4 ::=     SEQUENCE {
    intraFreqNeighCellList              IntraFreqNeighCellList
          OPTIONAL,   -- Need OR
    intraFreqBlackCellList              IntraFreqBlackCellList
                  OPTIONAL,   -- Need OR
    csg-PhysCellIdRange                 PhysCellIdRange
                 OPTIONAL,   -- Cond CSG
    ...
}
IntraFreqNeighCellList ::=      SEQUENCE (SIZE (1..maxCellIntra)) OF
            IntraFreqNeighCellInfo
IntraFreqNeighCellInfo ::=      SEQUENCE {
    physCellId                              PhysCellId,
    q-OffsetCell                            Q-OffsetRange,
    ...
}
IntraFreqBlackCellList ::=      SEQUENCE (SIZE (1..maxCellBlack)) OF
            PhysCellIdRange
*/
rrc_return_et bch_m_check_and_convert_sib4
(
    OSCTXT                      *pctxt,         /* for memory allocation */
    SystemInformationBlockType4 *p_asn1_sib4,   /* destination */
    sib_type_4_Info_t           *p_sib4         /* source */
)
{
    U8 i = RRC_NULL;
    OSRTDListNode *p_node = PNULL;

    IntraFreqNeighCellInfo *p_neighbouring_cell_elem = PNULL;

    PhysCellIdRange *p_blacklisted_cell_elem         = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    /*Init*/
    asn1Init_SystemInformationBlockType4(p_asn1_sib4);

    p_asn1_sib4->m.intraFreqNeighCellListPresent = 0;
    p_asn1_sib4->m.intraFreqBlackCellListPresent = 0;
    p_asn1_sib4->m.csg_PhysCellIdRangePresent    = 0;

    if (p_sib4->presence_bitmask & SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG)
    {
        p_asn1_sib4->m.intraFreqNeighCellListPresent = 1;

        if (p_sib4->intra_freq_neigh_cell_list.count > ARRSIZE(p_sib4->
            intra_freq_neigh_cell_list.neigh_cell))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Too big p_sib4->intra_freq_neigh_cell_list.count: current=%u,"
                    " RRM max=%u",
                p_sib4->intra_freq_neigh_cell_list.count,
                ARRSIZE(p_sib4->intra_freq_neigh_cell_list.neigh_cell));
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        rtxDListInit(&p_asn1_sib4->intraFreqNeighCellList);
        for (i = 0; i < p_sib4->intra_freq_neigh_cell_list.count; i++)
        {
            rtxDListAllocNodeAndData(pctxt, IntraFreqNeighCellInfo, &p_node,
                &p_neighbouring_cell_elem);
            if (PNULL == p_node)
            {
                /* Not enough memory */
                return RRC_FAILURE;
            }
            asn1Init_IntraFreqNeighCellInfo(p_neighbouring_cell_elem);
            p_neighbouring_cell_elem->physCellId   = p_sib4->
                intra_freq_neigh_cell_list.neigh_cell[i].phys_cell_id;
            p_neighbouring_cell_elem->q_OffsetCell = p_sib4->
                intra_freq_neigh_cell_list.neigh_cell[i].q_offset_cell;

            rtxDListInit(&p_neighbouring_cell_elem->extElem1);

            rtxDListAppendNode(&p_asn1_sib4->intraFreqNeighCellList, p_node);
        }
    }

    if (p_sib4->presence_bitmask & SIB_4_BLACK_LIST_LIST_PRESENCE_FLAG)
    {
        p_asn1_sib4->m.intraFreqBlackCellListPresent = 1;

        if (p_sib4->intra_freq_black_listed_cell_list.count > ARRSIZE(p_sib4->
            intra_freq_black_listed_cell_list.black_listed_cell))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Too big p_sib4->intra_freq_black_listed_cell_list.count: "
                    "current=%u, RRM max=%u",
                p_sib4->intra_freq_black_listed_cell_list.count,
                ARRSIZE(p_sib4->intra_freq_black_listed_cell_list.
                    black_listed_cell));
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        rtxDListInit(&p_asn1_sib4->intraFreqBlackCellList);
        for (i = 0; i < p_sib4->intra_freq_black_listed_cell_list.count; i++)
        {
            rtxDListAllocNodeAndData(pctxt, PhysCellIdRange, &p_node,
                &p_blacklisted_cell_elem);
            if (PNULL == p_node)
            {
                /* Not enough memory */
                return RRC_FAILURE;
            }
            asn1Init_PhysCellIdRange(p_blacklisted_cell_elem);
            if (RRC_SUCCESS !=
                check_and_convert_physical_cell_identity_and_range(
                p_blacklisted_cell_elem,
                &p_sib4->intra_freq_black_listed_cell_list.black_listed_cell[i])
            )
            {
                return RRC_FAILURE;
            }

            rtxDListAppendNode(&p_asn1_sib4->intraFreqBlackCellList, p_node);
        }
    }

    if (p_sib4->presence_bitmask & SIB_4_PHY_CELL_ID_PRESENCE_FLAG)
    {
        p_asn1_sib4->m.csg_PhysCellIdRangePresent = 1;

        if (RRC_SUCCESS != check_and_convert_physical_cell_identity_and_range
            (&p_asn1_sib4->csg_PhysCellIdRange,
            &p_sib4->csg_phy_cell_id_range)
        )
        {
            return RRC_FAILURE;
        }
    }

    rtxDListInit(&p_asn1_sib4->extElem1);

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib5
*   INPUT        : OSCTXT                      *pctxt
*                  SystemInformationBlockType5 *p_asn1_sib5
*                  sib_type_5_Info_t           *p_sib5
*                  csc_cell_ctx_t              *p_cell_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function convert sib_type_5_Info_t to
*                  SystemInformationBlockType5
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib5
(
    OSCTXT                      *pctxt,         /* for memory allocation */
    SystemInformationBlockType5 *p_asn1_sib5,   /* destination */
    sib_type_5_Info_t           *p_sib5,         /* source */
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
)
{
    U8 i = RRC_NULL;
    U8 j = RRC_NULL;
    /*SPR 4355 Start*/
    U8     neigh_cell_config;   
    /*SPR 4355 End*/
    OSRTDListNode *p_node                       = PNULL;
    InterFreqCarrierFreqInfo *p_elem            = PNULL;
    inter_freq_carrier_freq_t *p_src_elem  = PNULL;

    OSRTDListNode *p_node_neighbouring          = PNULL;
    InterFreqNeighCellInfo *p_elem_neighbouring = PNULL;

    OSRTDListNode *p_node_blacklisted           = PNULL;
    PhysCellIdRange *p_elem_blacklisted         = PNULL;

    /* CR 970 changes start*/
    SystemInformationBlockType5_v8h0_IEs    sib_5_v8h0_ies;
    OSCTXT  asn1_ctx;
    U8      index = RRC_NULL;
    OSRTDListNode *p_node_1 = PNULL;
    U32     encodedmsg_len = 0;
    U8      late_non_critical_extension_buff[MAX_ASN_BUFF_LENGTH];
    S32     enc_res = RRC_NULL;
    InterFreqCarrierFreqInfo_v8h0   *p_elem_1 = PNULL; 
    inter_freq_carrier_freq_info_v8h0_t *p_src_elem_1 = PNULL;

    memset_wrapper(late_non_critical_extension_buff, RRC_NULL, 
            sizeof(late_non_critical_extension_buff));
    memset_wrapper(&sib_5_v8h0_ies, RRC_NULL, sizeof(SystemInformationBlockType5_v8h0_IEs));

    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
        return RRC_FAILURE;
    }
    /* CR 970 changes stop*/
    RRC_CSC_UT_TRACE_ENTER();

    /*Init*/
    asn1Init_SystemInformationBlockType5(p_asn1_sib5);

    if (p_sib5->inter_freq_carrier_freq_list_list.count > ARRSIZE(p_sib5->
        inter_freq_carrier_freq_list_list.inter_freq_carrier_freq_list))
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Too big p_sib5->inter_freq_carrier_freq_list_list.count: current"
                "=%u, RRM max=%u",
            p_sib5->inter_freq_carrier_freq_list_list.count,
            ARRSIZE(p_sib5->inter_freq_carrier_freq_list_list.
                inter_freq_carrier_freq_list));
        /*SPR_18125_START*/
        rtFreeContext(&asn1_ctx);
        /*SPR_18125_END*/
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    rtxDListInit(&p_asn1_sib5->interFreqCarrierFreqList);
    for (i = 0; i < p_sib5->inter_freq_carrier_freq_list_list.count; i++)
    {
        rtxDListAllocNodeAndData(pctxt,InterFreqCarrierFreqInfo, &p_node,
            &p_elem);
        if (PNULL == p_node)
        {
            /* Not enough memory */
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            return RRC_FAILURE;
        }
        asn1Init_InterFreqCarrierFreqInfo(p_elem); 
        rtxDListInit(&p_elem->extElem1);
        p_src_elem = &(p_sib5->inter_freq_carrier_freq_list_list.
            inter_freq_carrier_freq_list[i]);

        p_elem->m.p_MaxPresent                              = 0;
        p_elem->m.t_ReselectionEUTRA_SFPresent              = 0;
        p_elem->m.cellReselectionPriorityPresent            = 0;
        p_elem->m.interFreqNeighCellListPresent             = 0;
        p_elem->m.interFreqBlackCellListPresent             = 0;

        p_elem->dl_CarrierFreq = p_src_elem->eutra_dl_carrier_freq;
        p_elem->q_RxLevMin = p_src_elem->qrx_lev_min;

        if (p_src_elem->presence_bitmask & INTER_FREQ_CARR_P_MAX_PRESENCE_FLAG)
        {
            p_elem->m.p_MaxPresent = 1;
            p_elem->p_Max = p_src_elem->p_max;
        }

        p_elem->t_ReselectionEUTRA = p_src_elem->t_reselection_eutran;

        if (p_src_elem->presence_bitmask &
            INTER_FREQ_CARR_SCALE_PARAM_PRESENCE_FLAG)
        {
            p_elem->m.t_ReselectionEUTRA_SFPresent = 1;

            p_elem->t_ReselectionEUTRA_SF.sf_Medium =
                p_src_elem->speed_depend_scal_params.sf_medium;

            p_elem->t_ReselectionEUTRA_SF.sf_High =
                p_src_elem->speed_depend_scal_params.sf_high;
        }

        p_elem->threshX_High            = p_src_elem->thresh_x_high;
        p_elem->threshX_Low             = p_src_elem->thresh_x_low;
        p_elem->allowedMeasBandwidth    = p_src_elem->measurement_bandwidth;
        p_elem->presenceAntennaPort1    = p_src_elem->presence_antenna_port_1;

        if (p_src_elem->presence_bitmask &
            INTER_FREQ_CARR_CELL_RESELCT_PRIOR_PRESENCE_FLAG)
        {
            p_elem->m.cellReselectionPriorityPresent = 1;
            p_elem->cellReselectionPriority = p_src_elem->
                cell_reselect_priority;
        }

        p_elem->neighCellConfig.numbits = N_CELL_CFG_NBITS;
       
        /*SPR 4355 Start*/
        neigh_cell_config = p_src_elem->neigh_cell_config << 6;
        p_elem->neighCellConfig.data[0] = neigh_cell_config & 0xc0; /* 2 bits */
        /*SPR 4355 End*/

        p_elem->q_OffsetFreq = p_src_elem->q_offset_freq;

        if (p_src_elem->presence_bitmask &
            INTER_FREQ_CARR_NEIGH_CELL_LIST_PRESENCE_FLAG)
        {
            p_elem->m.interFreqNeighCellListPresent = 1;

            /* InterFreqNeighbouringCellList */
            if (p_src_elem->inter_freq_neigh_cell_list.count > ARRSIZE
                (p_src_elem->inter_freq_neigh_cell_list.neigh_cell))
            {
                RRC_CSC_TRACE(RRC_WARNING,
                    "Too big inter_freq_neigh_cell_list.count: current=%u, RRM"
                        " max=%u",
                    p_src_elem->inter_freq_neigh_cell_list.count,
                    ARRSIZE(p_src_elem->inter_freq_neigh_cell_list.neigh_cell));
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            rtxDListInit(&p_elem->interFreqNeighCellList);
            for (j = 0; j < p_src_elem->inter_freq_neigh_cell_list.count; j++)
            {
                rtxDListAllocNodeAndData(pctxt,InterFreqNeighCellInfo,
                    &p_node_neighbouring,&p_elem_neighbouring);
                if (PNULL == p_node_neighbouring)
                {
                    /* Not enough memory */
                    /*SPR_18125_START*/
                    rtFreeContext(&asn1_ctx);
                    /*SPR_18125_END*/
                    return RRC_FAILURE;
                }
                p_elem_neighbouring->physCellId = p_src_elem->
                    inter_freq_neigh_cell_list.neigh_cell[j].phys_cell_id;
                p_elem_neighbouring->q_OffsetCell = p_src_elem->
                    inter_freq_neigh_cell_list.neigh_cell[j].q_offset_cell;

                rtxDListAppendNode(&p_elem->interFreqNeighCellList,
                    p_node_neighbouring);
            }
        }

        if (p_src_elem->presence_bitmask &
            INTER_FREQ_CARR_BLACK_LIST_CELL_LIST_PRESENCE_FLAG)
        {
            p_elem->m.interFreqBlackCellListPresent = 1;

            /*InterFreqBlacklistedCellList*/
            if (p_src_elem->inter_freq_black_listed_cell_list.count > ARRSIZE
                (p_src_elem->inter_freq_black_listed_cell_list.black_listed_cell))
            {
                RRC_CSC_TRACE(RRC_WARNING,
                    "Too big inter_freq_black_listed_cell_list.count: current"
                        "=%u, RRM max=%u",
                    p_src_elem->inter_freq_black_listed_cell_list.count,
                    ARRSIZE(p_src_elem->inter_freq_black_listed_cell_list.
                        black_listed_cell));
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            rtxDListInit(&p_elem->interFreqBlackCellList);
            for (j = 0; j < p_src_elem->inter_freq_black_listed_cell_list.count;
                j++)
            {
                rtxDListAllocNodeAndData(pctxt, PhysCellIdRange,
                    &p_node_blacklisted, &p_elem_blacklisted);
                if (PNULL == p_node_blacklisted)
                {
                    /* Not enough memory */
                    /*SPR_18125_START*/
                    rtFreeContext(&asn1_ctx);
                    /*SPR_18125_END*/
                    return RRC_FAILURE;
                }
                asn1Init_PhysCellIdRange(p_elem_blacklisted);
                if (RRC_SUCCESS !=
                    check_and_convert_physical_cell_identity_and_range(
                    p_elem_blacklisted,
                    &p_src_elem->inter_freq_black_listed_cell_list.
                        black_listed_cell[j])
                )
                {
                    /*SPR_18125_START*/
                    rtFreeContext(&asn1_ctx);
                    /*SPR_18125_END*/
                    return RRC_FAILURE;
                }

                rtxDListAppendNode(&p_elem->interFreqBlackCellList,
                    p_node_blacklisted);
            }
        }

        /*cr_398:Cell reselection enhancements*/

        /*Fill Q_QualMin_r9*/
        if (p_src_elem->presence_bitmask &
            INTER_FREQ_CARR_Q_QUAL_MIN_R9_PRESENCE_FLAG)
        {
            p_elem->m._v2ExtPresent = 1;
            p_elem->m.q_QualMin_r9Present = 1;
       
            p_elem->q_QualMin_r9 = p_src_elem->q_qual_min_r9;
        }
        /*Fill InterFreqCarrierFreqInfo_threshX_Q_r9*/
        if (RRC_TRUE == p_cell_ctx->sib3_thresh_serving_low_q_flag) 
        {
            if(INTER_FREQ_THRESH_X_Q_R9_PRESENCE_FLAG &
                  p_src_elem->presence_bitmask)
            {
               p_elem->m._v2ExtPresent = 1;
               p_elem->m.threshX_Q_r9Present = 1;
           
               p_elem->threshX_Q_r9.threshX_HighQ_r9 =
                  p_src_elem->thresh_x_q_r9.thresh_x_high_q_r9;
            
                p_elem->threshX_Q_r9.threshX_LowQ_r9 =
                  p_src_elem->thresh_x_q_r9.thresh_x_low_q_r9;
            }
            else
            {
              RRC_CSC_TRACE(RRC_WARNING,
                  "Wrong Configuration: sib3:threshServingLowQ is present "
                  "without sib5:threshX-Q-r9");
              /*SPR_18125_START*/
              rtFreeContext(&asn1_ctx);
              /*SPR_18125_END*/
              RRC_CSC_UT_TRACE_EXIT();
              return RRC_FAILURE;
            }
        }
        else if(INTER_FREQ_THRESH_X_Q_R9_PRESENCE_FLAG &
               p_src_elem->presence_bitmask)
        {
           RRC_CSC_TRACE(RRC_WARNING,
            "Wrong Configuration: sib5:threshX-Q-r9 is present "
            "without sib3:threshServingLowQ");
    /*SPR_18202_FIX_START*/
           rtFreeContext(&asn1_ctx);
           RRC_CSC_UT_TRACE_EXIT();
           return RRC_FAILURE;
    /*SPR_18202_FIX_STOP*/
        }
        rtxDListAppendNode(&p_asn1_sib5->interFreqCarrierFreqList, p_node);
    }

    /* CR 970 changes start*/
    if (p_sib5->bitmask & SIB_TYPE_5_v8h0_IES_PRESENT)
    {
        p_asn1_sib5->m.lateNonCriticalExtensionPresent = 1;
        
        if (p_sib5->sib_5_v8h0_ies.bitmask & 
                SIB_5_v8h0_INTER_FREQ_CARRIER_FREQ_LIST_PRESENT)
        {
            sib_5_v8h0_ies.m.interFreqCarrierFreqList_v8h0Present = 1;
            sib_5_v8h0_ies.m.nonCriticalExtensionPresent = 0;
            
            rtxDListInit(&sib_5_v8h0_ies.interFreqCarrierFreqList_v8h0);

            for(index = 0; index < p_sib5->sib_5_v8h0_ies.
                    inter_freq_carrier_freq_list_v8h0.count; index++)
            {
                rtxDListAllocNodeAndData(&asn1_ctx, InterFreqCarrierFreqInfo_v8h0,
                        &p_node_1, &p_elem_1);
                if (PNULL == p_node_1)
                {
                    /* Not enough memory */
                    /*SPR_18125_START*/
                    rtFreeContext(&asn1_ctx);
                    /*SPR_18125_END*/
                    return RRC_FAILURE;
                }

                asn1Init_InterFreqCarrierFreqInfo_v8h0(p_elem_1);

                p_src_elem_1 = &(p_sib5->sib_5_v8h0_ies.inter_freq_carrier_freq_list_v8h0.
                    inter_freq_carrier_freq_info_v8h0[index]);

                if (p_src_elem_1->bitmask & 
                        INTER_FREQ_CARRIER_FREQ_v8h0_MULTI_BAND_INFO_LIST_PRESENT)
                {
                    p_elem_1->m.multiBandInfoListPresent = 1;
                   
                    p_elem_1->multiBandInfoList.n = (U32)p_src_elem_1->
                        multi_band_info_list.count;

                    for (i = 0 ; i < p_src_elem_1->multi_band_info_list.count; i++)
                    {
                        p_elem_1->multiBandInfoList.elem[i] = p_src_elem_1->
                            multi_band_info_list.freq_band_indicator[i];
                    }
                }
                rtxDListAppendNode(&sib_5_v8h0_ies.interFreqCarrierFreqList_v8h0,
                        p_node_1);
            }

            pu_setBuffer(&asn1_ctx, late_non_critical_extension_buff,
                MAX_ASN_BUFF_LENGTH, ASN_RRC_BUF_TYPE);
            
            enc_res = asn1PE_SystemInformationBlockType5_v8h0_IEs(&asn1_ctx, &sib_5_v8h0_ies);
            if(RT_OK !=  enc_res)
            {
                RRC_CSC_TRACE(RRC_ERROR,
                        "ASN1 encoding failed.");
            }
            else
            {
                encodedmsg_len = (U32)pe_GetMsgLen(&asn1_ctx);
                p_asn1_sib5->lateNonCriticalExtension.data = (OSOCTET*)rtxMemAlloc(pctxt, 
                        encodedmsg_len);
                /* SPR 12964 Fix Start */
                if(PNULL == p_asn1_sib5->lateNonCriticalExtension.data)
                {
                    RRC_CSC_TRACE(RRC_ERROR, "Unable to allocate memory.");
                    rtFreeContext(&asn1_ctx);
                    RRC_CSC_UT_TRACE_EXIT();
                    return RRC_FAILURE;

                }
                /* SPR 12964 Fix Stop */

                l3_memcpy_wrapper((void *)p_asn1_sib5->lateNonCriticalExtension.data, 
                        late_non_critical_extension_buff , encodedmsg_len);

                p_asn1_sib5->lateNonCriticalExtension.numocts = encodedmsg_len;
            }
        }
    }

    /* CR 970 changes stop*/
    rtxDListInit(&p_asn1_sib5->extElem1);

    /*valgrind_fix start*/
    rtFreeContext(&asn1_ctx);
    /*valgrind_fix stop*/

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib6
*   INPUT        : OSCTXT                      *p_asn1_ctx
*                  SystemInformationBlockType6 *p_asn1_sib6
*                  sib_type_6_Info_t           *p_sib6
*                  csc_cell_ctx_t              *p_cell_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function convert sib_type_6_Info_t to
*                  SystemInformationBlockType6
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib6
(
    OSCTXT                      *p_asn1_ctx,    /* for memory allocation */
    SystemInformationBlockType6 *p_asn1_sib6,   /* destination */
    sib_type_6_Info_t           *p_sib6,         /* source */
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
)
{
    rrc_counter_t   index = RRC_NULL;
    /* CR 970 changes start*/
    OSCTXT  asn1_ctx;
    U8      count = RRC_NULL;
    U32     encodedmsg_len = 0;
    U8      late_non_critical_extension_buff[MAX_ASN_BUFF_LENGTH];
    S32     enc_res = RRC_NULL;
    SystemInformationBlockType6_v8h0_IEs    sib_6_v8h0_ies;
    OSRTDListNode *p_node_1 = PNULL;
    CarrierFreqInfoUTRA_FDD_v8h0 *p_elem_1 = PNULL;
    carrier_freq_info_utra_fdd_v8h0_t *p_src_elem_1 = PNULL;

    memset_wrapper(late_non_critical_extension_buff, RRC_NULL,
            sizeof(late_non_critical_extension_buff));
    memset_wrapper(&sib_6_v8h0_ies, RRC_NULL, sizeof(SystemInformationBlockType6_v8h0_IEs));
    
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
        return RRC_FAILURE;
    }

    /* CR 970 changes stop*/
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_sib6);
    RRC_ASSERT(PNULL != p_asn1_sib6);

    /*Init*/
    asn1Init_SystemInformationBlockType6(p_asn1_sib6);

    /* Fill ASN.1 SIB6 message */

    p_asn1_sib6->m.carrierFreqListUTRA_FDDPresent = 0;
    p_asn1_sib6->m.carrierFreqListUTRA_TDDPresent = 0;
    p_asn1_sib6->m.t_ReselectionUTRA_SFPresent    = 0;
    /* carrierFreqListUTRA_FDD */
    if (SIB_6_CARRIER_FREQ_LIST_UTRA_FDD_PRESENCE_FLAG & p_sib6->
        presence_bitmask)
    {
        p_asn1_sib6->m.carrierFreqListUTRA_FDDPresent = 1;

        if ((p_sib6->carrier_freq_list_utra_fdd.count < 1) ||
            (p_sib6->carrier_freq_list_utra_fdd.count > ARRSIZE(p_sib6->
                carrier_freq_list_utra_fdd.data)))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Incorrect carrier_freq_list_utra_fdd_t::count=%u. Should be 1"
                    "..%u",
                p_sib6->carrier_freq_list_utra_fdd.count,
                ARRSIZE(p_sib6->carrier_freq_list_utra_fdd.data)
            );
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        rtxDListInit(&p_asn1_sib6->carrierFreqListUTRA_FDD);

        for (index = 0; index < p_sib6->carrier_freq_list_utra_fdd.count;
            index++)
        {
            CarrierFreqUTRA_FDD     *p_asn1_fdd_elem  = PNULL;
            OSRTDListNode           *p_asn1_list_node = PNULL;
            carrier_freq_utra_fdd_t *p_fdd            = &(p_sib6->
                carrier_freq_list_utra_fdd.data[index]);

            rtxDListAllocNodeAndData(p_asn1_ctx, CarrierFreqUTRA_FDD,
                &p_asn1_list_node, &p_asn1_fdd_elem);
            if (PNULL == p_asn1_list_node)
            {
                /* Not enough memory */
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            asn1Init_CarrierFreqUTRA_FDD(p_asn1_fdd_elem);
            p_asn1_fdd_elem->m.cellReselectionPriorityPresent = 0;
            /* carrierFreq */
            p_asn1_fdd_elem->carrierFreq = p_fdd->carrier_freq;
            /* cellReselectionPriority */
            if (p_fdd->presence_bitmask &
                CR_FREQ_UTRA_FDD_CELL_RESEL_PRI_PRESENCE_FLAG)
            {
                p_asn1_fdd_elem->m.cellReselectionPriorityPresent = 1;
                p_asn1_fdd_elem->cellReselectionPriority = p_fdd->
                    cell_reselection_priority;
            }
            p_asn1_fdd_elem->threshX_High = p_fdd->thresh_x_high;
            p_asn1_fdd_elem->threshX_Low  = p_fdd->thresh_x_low;
            p_asn1_fdd_elem->q_RxLevMin   = p_fdd->q_rx_lev_min;
            p_asn1_fdd_elem->p_MaxUTRA    = p_fdd->p_max_utra;
            p_asn1_fdd_elem->q_QualMin    = p_fdd->q_qual_min;

            /*cd_398:Cell reselection enhancements*/
            /*Fill InterFreqCarrierFreqInfo_threshX_Q_r9*/
	        /* SPR 16454 Fix Start */
            if (CR_FREQ_UTRA_FDD_THRESH_X_Q_R9_PRESENCE_FLAG &
                p_fdd->presence_bitmask)
            {
		        if (RRC_TRUE == p_cell_ctx->sib3_thresh_serving_low_q_flag)
		        {
                    p_asn1_fdd_elem->m._v2ExtPresent = 1;
                    p_asn1_fdd_elem->m.threshX_Q_r9Present = 1;
           
                    p_asn1_fdd_elem->threshX_Q_r9.threshX_HighQ_r9 =
                        p_fdd->thresh_x_q_r9.thresh_x_high_q_r9;
            
                    p_asn1_fdd_elem->threshX_Q_r9.threshX_LowQ_r9 =
                        p_fdd->thresh_x_q_r9.thresh_x_low_q_r9;
                }
                else 
                {
                    RRC_CSC_TRACE(RRC_WARNING,
			        "Wrong Configuration: sib6:threshX-Q-r9 is present"
			        "without sib3:threshServingLowQ");
                    /*SPR_18125_START*/
                    rtFreeContext(&asn1_ctx);
                    /*SPR_18125_END*/
                    RRC_CSC_UT_TRACE_EXIT();
                    return RRC_FAILURE;
                }
            }   
   	        /* SPR 16454 Fix Stop */
    /*SPR_18202_FIX_START*/
            else if (RRC_TRUE == p_cell_ctx->sib3_thresh_serving_low_q_flag)
            {
                RRC_CSC_TRACE(RRC_WARNING,
			    "Wrong Configuration: sib3:threshServingLowQ is present"
			    "without sib6:threshX-Q-r9");
                rtFreeContext(&asn1_ctx);
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
    /*SPR_18202_FIX_STOP*/
 
            rtxDListInit(&p_asn1_fdd_elem->extElem1);
            rtxDListAppendNode(&p_asn1_sib6->carrierFreqListUTRA_FDD,
                p_asn1_list_node);
        }
    }
    /* carrierFreqListUTRA_TDD */
    if (SIB_6_CARRIER_FREQ_LIST_UTRA_TDD_PRESENCE_FLAG & p_sib6->
        presence_bitmask)
    {
        p_asn1_sib6->m.carrierFreqListUTRA_TDDPresent = 1;

        if ((p_sib6->carrier_freq_list_utra_tdd.count < 1) ||
            (p_sib6->carrier_freq_list_utra_tdd.count > ARRSIZE(p_sib6->
                carrier_freq_list_utra_tdd.data)))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Incorrect carrier_freq_list_utra_tdd_t::count=%u. Should be 1"
                    "..%u",
                p_sib6->carrier_freq_list_utra_tdd.count,
                ARRSIZE(p_sib6->carrier_freq_list_utra_tdd.data)
            );
            /*SPR_18125_START*/
            rtFreeContext(&asn1_ctx);
            /*SPR_18125_END*/
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        rtxDListInit(&p_asn1_sib6->carrierFreqListUTRA_TDD);

        for (index = 0; index < p_sib6->carrier_freq_list_utra_tdd.count;
            index++)
        {
            CarrierFreqUTRA_TDD     *p_asn1_tdd_elem  = PNULL;
            OSRTDListNode           *p_asn1_list_node = PNULL;
            carrier_freq_utra_tdd_t *p_tdd            = &(p_sib6->
                carrier_freq_list_utra_tdd.data[index]);

            rtxDListAllocNodeAndData(p_asn1_ctx, CarrierFreqUTRA_TDD,
                &p_asn1_list_node, &p_asn1_tdd_elem);
            if (PNULL == p_asn1_list_node)
            {
                /* Not enough memory */
                /*SPR_18125_START*/
                rtFreeContext(&asn1_ctx);
                /*SPR_18125_END*/
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            asn1Init_CarrierFreqUTRA_TDD(p_asn1_tdd_elem);
            p_asn1_tdd_elem->m.cellReselectionPriorityPresent = 0;
            /* carrierFreq */
            p_asn1_tdd_elem->carrierFreq = p_tdd->carrier_freq;
            /* cellReselectionPriority */
            if (p_tdd->presence_bitmask &
                CR_FREQ_UTRA_TDD_CELL_RESEL_PRI_PRESENCE_FLAG)
            {
                p_asn1_tdd_elem->m.cellReselectionPriorityPresent = 1;
                p_asn1_tdd_elem->cellReselectionPriority = p_tdd->
                    cell_reselection_priority;
            }
            p_asn1_tdd_elem->threshX_High = p_tdd->thresh_x_high;
            p_asn1_tdd_elem->threshX_Low  = p_tdd->thresh_x_low;
            p_asn1_tdd_elem->q_RxLevMin   = p_tdd->q_rx_lev_min;
            p_asn1_tdd_elem->p_MaxUTRA    = p_tdd->p_max_utra;
            rtxDListInit(&p_asn1_tdd_elem->extElem1);

            rtxDListAppendNode(&p_asn1_sib6->carrierFreqListUTRA_TDD,
                p_asn1_list_node);
        }
    }
    /* t_ReselectionUTRA */
    p_asn1_sib6->t_ReselectionUTRA = p_sib6->t_resel_utra;
    /* t_ReselectionUTRA_SF */
    if (SIB_6_T_RESELECTION_UTRA_SF_PRESENCE_FLAG & p_sib6->presence_bitmask)
    {
        p_asn1_sib6->m.t_ReselectionUTRA_SFPresent  = 1;
        p_asn1_sib6->t_ReselectionUTRA_SF.sf_Medium = p_sib6->t_resel_utra_sf.
            sf_medium;
        p_asn1_sib6->t_ReselectionUTRA_SF.sf_High   = p_sib6->t_resel_utra_sf.
            sf_high;
    }
    
    /* CR 970 changes start*/
    if(SIB_6_LATE_NON_CRITICAL_EXTENSION_PRESENCE_FLAG & p_sib6->presence_bitmask)
    {
        p_asn1_sib6->m.lateNonCriticalExtensionPresent = 1;
        
        if (p_sib6->sib_6_v8h0_ies.bitmask & SIB_6_v8h0_CARRIER_FREQ_LIST_UTRA_FDD_PRESENT)
        {
            sib_6_v8h0_ies.m.carrierFreqListUTRA_FDD_v8h0Present = 1;
            sib_6_v8h0_ies.m.nonCriticalExtensionPresent = 0;
            
            rtxDListInit(&sib_6_v8h0_ies.carrierFreqListUTRA_FDD_v8h0);

            for (index = 0;index < p_sib6->sib_6_v8h0_ies.carrier_freq_list_utra_fdd_v8h0.
                    count; index++)
            {
                rtxDListAllocNodeAndData(&asn1_ctx, CarrierFreqInfoUTRA_FDD_v8h0, 
                        &p_node_1, &p_elem_1);
                if (PNULL == p_node_1)
                {
                    /* Not enough memory */
                    /*SPR_18125_START*/
                    rtFreeContext(&asn1_ctx);
                    /*SPR_18125_END*/
                    return RRC_FAILURE;
                }

                asn1Init_CarrierFreqInfoUTRA_FDD_v8h0(p_elem_1);

                p_src_elem_1 = &(p_sib6->sib_6_v8h0_ies.carrier_freq_list_utra_fdd_v8h0.
                        carrier_freq_info_utra_fdd_v8h0[index]);

                if (p_src_elem_1->bitmask &
                        CARRIER_FREQ_INFO_UTRA_FDD_v8h0_MULTI_BAND_INFO_LIST_PRESENT)
                {
                    p_elem_1->m.multiBandInfoListPresent = 1;

                    p_elem_1->multiBandInfoList.n = (U32)p_src_elem_1->multi_band_info_list.
                        count;

                    for (count = 0; count < p_elem_1->multiBandInfoList.n; count++)
                    {
                        p_elem_1->multiBandInfoList.elem[count] = p_src_elem_1->
                            multi_band_info_list.freq_band_indicator_utra_fdd[count];
                    }
                }
                rtxDListAppendNode(&sib_6_v8h0_ies.carrierFreqListUTRA_FDD_v8h0, 
                        p_node_1);
            }

            pu_setBuffer(&asn1_ctx, late_non_critical_extension_buff,
                    MAX_ASN_BUFF_LENGTH, ASN_RRC_BUF_TYPE);
            
            enc_res = asn1PE_SystemInformationBlockType6_v8h0_IEs(&asn1_ctx, &sib_6_v8h0_ies);
            if(RT_OK !=  enc_res)
            {
                RRC_CSC_TRACE(RRC_ERROR,
                        "ASN1 encoding failed.");
            }
            else
            {
                encodedmsg_len = (U32)pe_GetMsgLen(&asn1_ctx);
                p_asn1_sib6->lateNonCriticalExtension.data = (OSOCTET*)rtxMemAlloc(p_asn1_ctx, 
                        encodedmsg_len);
                /* SPR 12964 Fix Start */
                if(PNULL == p_asn1_sib6->lateNonCriticalExtension.data)
                {
                    RRC_CSC_TRACE(RRC_ERROR, "Unable to allocate memory.");
                    rtFreeContext(&asn1_ctx);
                    RRC_CSC_UT_TRACE_EXIT();
                    return RRC_FAILURE;

                }
                /* SPR 12964 Fix Stop */

                l3_memcpy_wrapper((void *)p_asn1_sib6->lateNonCriticalExtension.data, 
                        late_non_critical_extension_buff , encodedmsg_len);

                p_asn1_sib6->lateNonCriticalExtension.numocts = encodedmsg_len;
            }
        }
    }
    /* CR 970 changes stop*/

    rtxDListInit(&p_asn1_sib6->extElem1);

/*valgrind_fix start*/
    rtFreeContext(&asn1_ctx);
/*valgrind_fix stop*/

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib7
*   INPUT        : OSCTXT                      *p_asn1_ctx
*                  SystemInformationBlockType7 *p_asn1_sib7
*                  sib_type_7_Info_t           *p_sib7
*   OUTPUT       : none
*   DESCRIPTION  : This function convert sib_type_7_Info_t to
*                  SystemInformationBlockType7
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib7
(
    OSCTXT                      *p_asn1_ctx,    /* for memory allocation */
    SystemInformationBlockType7 *p_asn1_sib7,   /* destination */
    sib_type_7_Info_t           *p_sib7         /* source */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_sib7);
    RRC_ASSERT(PNULL != p_asn1_sib7);

    /*Init*/
    asn1Init_SystemInformationBlockType7(p_asn1_sib7);

    /* Fill ASN.1 SIB7 message */

    p_asn1_sib7->m.t_ReselectionGERAN_SFPresent = 0;
    p_asn1_sib7->m.carrierFreqsInfoListPresent  = 0;
    /* t_ReselectionGERAN */
    p_asn1_sib7->t_ReselectionGERAN = p_sib7->t_reselection_geran;
    /* t_ReselectionGERAN_SF */
    if (SIB_7_T_RESEL_GERAN_SF_PRESENCE_FLAG & p_sib7->presence_bitmask)
    {
        p_asn1_sib7->m.t_ReselectionGERAN_SFPresent = 1;

        p_asn1_sib7->t_ReselectionGERAN_SF.sf_Medium = p_sib7->
            t_reselect_geran_sf.sf_medium;
        p_asn1_sib7->t_ReselectionGERAN_SF.sf_High   = p_sib7->
            t_reselect_geran_sf.sf_high;
    }
    /* carrierFreqsInfoList */
    if (SIB_7_CR_FREQS_INFO_LST_PRESENCE_FLAG & p_sib7->presence_bitmask)
    {
        rrc_counter_t          cfi_index        = RRC_NULL;
        CarrierFreqsInfoGERAN *p_asn1_cfi_elem  = PNULL;
        OSRTDListNode         *p_asn1_list_node = PNULL;

        p_asn1_sib7->m.carrierFreqsInfoListPresent = 1;

        if ((p_sib7->carrier_freqs_info_lst.count < 1) ||
            (p_sib7->carrier_freqs_info_lst.count > ARRSIZE(p_sib7->
                carrier_freqs_info_lst.cr_freq_info)))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Incorrect carrier_freqs_info_list_geran_t::count=%u. Should "
                    "be 1..%u",
                p_sib7->carrier_freqs_info_lst.count,
                ARRSIZE(p_sib7->carrier_freqs_info_lst.cr_freq_info)
            );
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        /*# carrierFreqsInfoList */
        rtxDListInit(&p_asn1_sib7->carrierFreqsInfoList);
        for (cfi_index = 0; cfi_index < p_sib7->carrier_freqs_info_lst.count;
            cfi_index++)
        {
            carrier_freqs_info_geran_t *p_cfi = &(p_sib7->carrier_freqs_info_lst
                .cr_freq_info[cfi_index]);

            rtxDListAllocNodeAndData(p_asn1_ctx, CarrierFreqsInfoGERAN,
                &p_asn1_list_node, &p_asn1_cfi_elem);
            if (PNULL == p_asn1_list_node)
            {
                /* Not enough memory */
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
            asn1Init_CarrierFreqsInfoGERAN(p_asn1_cfi_elem);
            /*## carrierFreqs */
            if (RRC_SUCCESS !=
                check_and_convert_carrier_freqs_geran(
                            p_asn1_ctx,
                            &p_asn1_cfi_elem->carrierFreqs,
                            &p_cfi->carrier_freqs
                            ))
            {
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }

            /*## commonInfo */
            p_asn1_cfi_elem->commonInfo.m.cellReselectionPriorityPresent = 0;
            p_asn1_cfi_elem->commonInfo.m.p_MaxGERANPresent              = 0;
            /*### cellReselectionPriority */
            if (CRFI_GERAN_CELL_RESEL_PRI_PRESENCE_FLAG & p_cfi->common_info.
                presence_bitmask)
            {
                p_asn1_cfi_elem->commonInfo.m.cellReselectionPriorityPresent =
                    1;
                p_asn1_cfi_elem->commonInfo.cellReselectionPriority = p_cfi->
                    common_info.cell_reselection_priority;
            }
            p_asn1_cfi_elem->commonInfo.ncc_Permitted.numbits =
                NCC_PERMITTED_NBITS;
            l3_memcpy_wrapper(p_asn1_cfi_elem->commonInfo.ncc_Permitted.data,
                   p_cfi->common_info.ncc_permitted,
                   sizeof(p_cfi->common_info.ncc_permitted));
            /*### q_RxLevMin*/
            p_asn1_cfi_elem->commonInfo.q_RxLevMin = p_cfi->common_info.
                q_rx_lev_min;
            /*### p_MaxGERAN*/
            if (CRFI_GERAN_P_MAX_GERAN_PRESENCE_FLAG & p_cfi->common_info.
                presence_bitmask)
            {
                p_asn1_cfi_elem->commonInfo.m.p_MaxGERANPresent = 1;
                p_asn1_cfi_elem->commonInfo.p_MaxGERAN = p_cfi->common_info.
                    p_max_geran;
            }
            /*### threshX_High*/
            p_asn1_cfi_elem->commonInfo.threshX_High = p_cfi->common_info.
                thresh_x_high;
            /*### threshX_Low*/
            p_asn1_cfi_elem->commonInfo.threshX_Low  = p_cfi->common_info.
                thresh_x_low;

            /*## extElem1 */
            rtxDListInit(&p_asn1_cfi_elem->extElem1);
            rtxDListAppendNode(&p_asn1_sib7->carrierFreqsInfoList,
                p_asn1_list_node);
        }
    }

    rtxDListInit(&p_asn1_sib7->extElem1);

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: check_and_convert_sys_time_info_cdma2000
*   INPUT        : OSCTXT                      *pctxt
*                  SystemTimeInfoCDMA2000      *p_asn1_time
*                  csc_cell_ctx_t              *p_cell_ctx
*                  U8                          si_segment_counter
*                  U64*                        cdma_sys_time
*   OUTPUT       : none
*   DESCRIPTION  : This function convert sys_time_info_cdma2000_t to
*                  SystemTimeInfoCDMA2000 for SIB_8
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et check_and_convert_sys_time_info_cdma2000
(
    OSCTXT                      *pctxt,         /* for memory allocation */
    SystemTimeInfoCDMA2000      *p_asn1_time,   /* destination */
    csc_cell_ctx_t              *p_cell_ctx,    /* cell context */
    U8                          si_segment_counter,
    U64*                        cdma_sys_time
)
{
    RRC_CSC_UT_TRACE_ENTER();
    /* Bug 12190 Fix Start */ 
    U8 bitPos = RRC_NULL;
    /* Bug 12190 Fix Stop */ 
    U8* p_buf;
    /* Bug 12190 Fix Start */ 
    U8** p_p_buf;
    U8 *p_bitPos;
    /* Bug 12190 Fix Stop */ 
    U32 counter = RRC_NULL;
    U32 src1 = RRC_NULL;
    U32 src2 = RRC_NULL;
    U64 time_for_n_segment = RRC_NULL;
    U64 cdma_base_time = RRC_NULL;

    p_asn1_time->cdma_EUTRA_Synchronisation = p_cell_ctx->nw_in_sync;

    if (p_cell_ctx->nw_in_sync)
    {
        p_asn1_time->cdma_SystemTime.t =
             T_SystemTimeInfoCDMA2000_cdma_SystemTime_synchronousSystemTime;
        p_asn1_time->cdma_SystemTime.u.synchronousSystemTime = rtxMemAllocType(pctxt,
                     SystemTimeInfoCDMA2000_cdma_SystemTime_synchronousSystemTime);
        if (PNULL == p_asn1_time->cdma_SystemTime.u.synchronousSystemTime)
        {
            /* Not enough memory */
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        memset_wrapper(p_asn1_time->cdma_SystemTime.u.synchronousSystemTime->data,
                0,
                sizeof(p_asn1_time->cdma_SystemTime.u.synchronousSystemTime->data));
        p_buf = p_asn1_time->cdma_SystemTime.u.synchronousSystemTime->data;
        p_asn1_time->cdma_SystemTime.u.synchronousSystemTime->numbits = SYN_SYS_TIME_NBITS;
        if(RRC_TRUE != p_cell_ctx->cell_setup_done)
        {
            *cdma_sys_time = 0;
            si_segment_counter = 1;
        }
        else
        {
            /*cdma_fix bug 10186 start*/
            /** Converting the system time stored in cell_ctx from microsec into SFN unit 
             *   1 microsecond = 10*1000 SFN  */

             *cdma_sys_time = (U64)( ( (p_cell_ctx->cdma_base_time)/(1000*SF_MAX_LIMIT)) + 
                                (p_cell_ctx->sib8_periodicity * si_segment_counter) );
            
            RRC_CSC_TRACE(RRC_DETAILED,"SYNC CDMA System Time  [%llu]", *cdma_sys_time);
             /*cdma_fix bug 10186 stop*/

            /*SPR_6293 Start*/
            *cdma_sys_time = (*cdma_sys_time << (MAX_LONG_NBITS - SYN_SYS_TIME_NBITS)) >> (MAX_LONG_NBITS - SYN_SYS_TIME_NBITS);
            /*SPR_6293 Stop*/
            RRC_CSC_TRACE(RRC_DETAILED,"SYNC CDMA System Time  [%llu]", *cdma_sys_time);

            src1 = (U32)(*cdma_sys_time >> MAX_INT_NBITS);

            RRC_TRACE(RRC_DETAILED,"valueof src1=[%d]\n",src1);

            src2 = (U32)(*cdma_sys_time << MAX_INT_NBITS >> MAX_INT_NBITS);

            RRC_TRACE(RRC_DETAILED,"valueof src2 = [%d]\n",src2);

        }

        /* CDMA time (MAX 39 bits) packed into Sync System time in two parts
        last 7 bits and first 32 bits */
        /* Bug 12190 Fix Start */ 
        p_p_buf = (U8 **)&p_buf;

        p_bitPos = (U8 *)&bitPos;
        /* SPR 13845 Fix Start */      
        SETNBIT( p_p_buf, /*p_buf*/
                p_bitPos,
                src2,
                MAX_INT_NBITS, /*width*/
                PNULL);
        SETNBIT( p_p_buf, /*p_buf*/
                p_bitPos,
                src1,
                (SYN_SYS_TIME_NBITS - MAX_INT_NBITS), /*width*/
                PNULL);
        /* SPR 13845 Fix Stop */      
        /* Bug 12190 Fix Stop */ 
/* valgrind_fix */ 
        for(counter = 0 ;counter < sizeof(p_asn1_time->cdma_SystemTime.u.synchronousSystemTime->data);counter++)
        {
            RRC_TRACE(RRC_DETAILED,"Sync data[%d] = [%02x]\n",counter,
                    p_asn1_time->cdma_SystemTime.u.synchronousSystemTime->data[counter]);

        }
        RRC_CSC_TRACE(RRC_INFO,"SYNC CDMA System Time [%llu]", *cdma_sys_time);
/* valgrind_fix */ 
    }
    else
    {
        p_asn1_time->cdma_SystemTime.t =
             T_SystemTimeInfoCDMA2000_cdma_SystemTime_asynchronousSystemTime;
        p_asn1_time->cdma_SystemTime.u.asynchronousSystemTime = rtxMemAllocType(pctxt,
                                SystemTimeInfoCDMA2000_cdma_SystemTime_asynchronousSystemTime);
        if (PNULL == p_asn1_time->cdma_SystemTime.u.asynchronousSystemTime)
        {
            /* Not enough memory */
            return RRC_FAILURE;
        }

        memset_wrapper(p_asn1_time->cdma_SystemTime.u.asynchronousSystemTime->data,
                0,
                sizeof(p_asn1_time->cdma_SystemTime.u.asynchronousSystemTime->data));
        p_buf = p_asn1_time->cdma_SystemTime.u.asynchronousSystemTime->data;
        
        p_asn1_time->cdma_SystemTime.u.asynchronousSystemTime->numbits = ASYN_SYS_TIME_NBITS;
        if(RRC_TRUE != p_cell_ctx->cell_setup_done)
        {
            *cdma_sys_time = 0;
            si_segment_counter = 1;
        }
        else
        {
            /*cdma_fix bug 10186 start*/
            /** Converting the system time stored in cell_ctx in unit of 8 CDMA
             * chips = 6.510416667 micro-seconds */

            cdma_base_time = (p_cell_ctx->cdma_base_time / CDMA_8_CHIPS_TIME_UNIT);
            
            /* Additional time to be added for  nth segment is calculated as 
             * 1. First converted the
              (p_cell_ctx->sib8_periodicity * si_segment_counter) hat is interms of SFN  into
             * milliseconds by multiplying with SF_MAX_LIMIT
             * 2. Now converted from milli seconds to micro seconds 
             * 3. Then divided by 6.510416667 to convert in format of 8 chips */

            time_for_n_segment = (p_cell_ctx->sib8_periodicity * si_segment_counter) * SF_MAX_LIMIT ; /*converting into miliseconds*/
            time_for_n_segment = time_for_n_segment * 1000 ; /*converting to microseconds*/
            time_for_n_segment = time_for_n_segment /CDMA_8_CHIPS_TIME_UNIT; /*converting into unit of 8 CDMA chips or 6.510416667 microseconds*/
            *cdma_sys_time = (cdma_base_time + time_for_n_segment);
            RRC_CSC_TRACE(RRC_DETAILED,"ASYNC CDMA System Time  [%llu]", *cdma_sys_time);
            /*cdma_fix bug 10186 stop*/
            *cdma_sys_time = *cdma_sys_time << (MAX_LONG_NBITS - ASYN_SYS_TIME_NBITS) >> (MAX_LONG_NBITS - ASYN_SYS_TIME_NBITS);
            RRC_CSC_TRACE(RRC_DETAILED,"ASYNC CDMA System Time  [%llu]", *cdma_sys_time);

            src1 = (U32)(*cdma_sys_time >> MAX_INT_NBITS);

            RRC_TRACE(RRC_DETAILED,"Value of src1=[%d]\n",src1);

            src2 = (U32)(*cdma_sys_time << MAX_INT_NBITS >> MAX_INT_NBITS);

            RRC_TRACE(RRC_DETAILED,"value of src2 = [%d]\n",src2);

        }

        /* CDMA time (MAX 49 bits) packed into Async System time in two parts
        last 17 bits and first 32 bits */
        /* Bug 12190 Fix Start */ 
        p_p_buf = (U8 **)&p_buf;

        p_bitPos = (U8 *)&bitPos;      

        SETNBIT( p_p_buf, /*p_buf*/
                p_bitPos,
                src1,
                (ASYN_SYS_TIME_NBITS - MAX_INT_NBITS), /*width*/
                PNULL);
        SETNBIT( p_p_buf, /*p_buf*/
                p_bitPos,
                src2,
                MAX_INT_NBITS, /*width*/
                PNULL);
        /* Bug 12190 Fix Stop */ 
        for(counter = 0 ;
                counter < sizeof(p_asn1_time->cdma_SystemTime.u.asynchronousSystemTime->data);
                counter++)
        {
            RRC_TRACE(RRC_DETAILED,"Async data[%d] = [%d]\n",counter,
                    p_asn1_time->cdma_SystemTime.u.asynchronousSystemTime->data[counter]);
        }
        RRC_CSC_TRACE(RRC_INFO,"ASYNC CDMA System Time [%llu]", *cdma_sys_time);
    }
    /*when last sib8 segment is calculated, then overwrite the previous
     * cdma_base_time with the one generated for MAX_SIB8_INSTANCES sib8 segment*/
    if(MAX_SIB8_INSTANCES == si_segment_counter)
    {
        p_cell_ctx->cdma_base_time = *cdma_sys_time;
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: fill_band_class_list_cdma_2000
*   INPUT        : OSCTXT                       *p_asn1_ctx
*                  BandClassListCDMA2000        *p_asn1_bcl
*                  band_class_list_cdma2000_t   *p_bcl
*   OUTPUT       : none
*   DESCRIPTION  : This function converts band_class_list_cdma2000_t to
*                  BandClassListCDMA2000
*                  and checks whether input data are valid.
*
*   RETURNS      : RRC_SUCCESS - all data are valid
*                  RRC_FAILURE - data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et fill_band_class_list_cdma_2000
(
    OSCTXT                       *p_asn1_ctx,    /* for memory allocation */
    BandClassListCDMA2000        *p_asn1_bcl,    /* destination */
    band_class_list_cdma2000_t   *p_bcl          /* source */
)
{
    rrc_counter_t          bc_info_index    = RRC_NULL;
    BandClassInfoCDMA2000 *p_asn1_bcl_elem  = PNULL;
    OSRTDListNode         *p_asn1_list_node = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_asn1_bcl);
    RRC_ASSERT(PNULL != p_bcl);


    if ((p_bcl->count < 1) || (p_bcl->count > ARRSIZE(p_bcl->
        band_class_info_cdma2000)))
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Incorrect band_class_list_cdma2000_t count=%u. Should be 1..%u",
            p_bcl->count,
            ARRSIZE(p_bcl->band_class_info_cdma2000)
        );
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
                                       
    /* Fill BandClassInfoCDMA2000 */
    rtxDListInit(p_asn1_bcl);
    for (bc_info_index = 0; bc_info_index < p_bcl->count; bc_info_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, BandClassInfoCDMA2000,
            &p_asn1_list_node, &p_asn1_bcl_elem);

        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        asn1Init_BandClassInfoCDMA2000(p_asn1_bcl_elem);
        p_asn1_bcl_elem->m.cellReselectionPriorityPresent = 0;
        /* bandClass */
        p_asn1_bcl_elem->bandClass  = p_bcl->
            band_class_info_cdma2000[bc_info_index].band_class;
        /* cellReselectionPriority */
        if (p_bcl->band_class_info_cdma2000[bc_info_index].presence_bitmask &
            BAND_CLASS_INFO_CDMA2000_CELL_RESELECT_PRI_PRESENCE_FLAG )
        {
            p_asn1_bcl_elem->m.cellReselectionPriorityPresent = 1;
            p_asn1_bcl_elem->cellReselectionPriority = p_bcl->
              band_class_info_cdma2000[bc_info_index].cell_reselection_priority;
        }
        p_asn1_bcl_elem->threshX_High = p_bcl->
            band_class_info_cdma2000[bc_info_index].thresh_x_high;
        p_asn1_bcl_elem->threshX_Low  = p_bcl->
            band_class_info_cdma2000[bc_info_index].thresh_x_low;

        rtxDListInit(&(p_asn1_bcl_elem->extElem1));

        rtxDListAppendNode(p_asn1_bcl, p_asn1_list_node);
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: fill_neigh_cells_per_freq_list
*   INPUT        : OSCTXT                                    *p_asn1_ctx
*                  NeighCellsPerBandclassListCDMA2000        *p_asn1_ncbcl
*                  neigh_cells_per_bandclass_list_cdma2000_t *p_ncbcl
*   OUTPUT       : none
*   DESCRIPTION  : This function converts neigh_cells_per_bandclass_list_cdma2000_t to
*                  NeighCellsPerBandclassListCDMA2000
*                  and checks whether input data are valid.
*
*   RETURNS      : RRC_SUCCESS - all data are valid
*                  RRC_FAILURE - data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et fill_neigh_cells_per_freq_list
(
    /* for memory allocation */
    OSCTXT                                    *p_asn1_ctx,
    NeighCellsPerBandclassListCDMA2000        *p_asn1_ncbcl, /* destination */
    neigh_cells_per_bandclass_list_cdma2000_t *p_ncbcl       /* source */
)
{
    rrc_counter_t                   ncbc_index        = RRC_NULL;
    rrc_counter_t                   id_index          = RRC_NULL;
    NeighCellsPerBandclassCDMA2000 *p_asn1_ncbcl_elem = PNULL;
    OSRTDListNode                  *p_asn1_list_node  = PNULL;
    phys_cell_id_list_cdma2000_t   *p_cell_id_list    = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    if ((p_ncbcl->count < 1) || (p_ncbcl->count > ARRSIZE(p_ncbcl->
        neigh_cells_per_bandclass_cdma2000)))
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Incorrect neigh_cells_per_bandclass_list_cdma2000_t count=%u. "
                "Should be 1..%u",
            p_ncbcl->count,
            ARRSIZE(p_ncbcl->neigh_cells_per_bandclass_cdma2000)
        );
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* Fill NeighCellCDMA2000 */
    rtxDListInit(p_asn1_ncbcl);
    for (ncbc_index = 0; ncbc_index < p_ncbcl->count; ncbc_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, NeighCellsPerBandclassCDMA2000,
            &p_asn1_list_node, &p_asn1_ncbcl_elem);
        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        asn1Init_NeighCellsPerBandclassCDMA2000(p_asn1_ncbcl_elem);
        /* arfcn */
        p_asn1_ncbcl_elem->arfcn = p_ncbcl->
            neigh_cells_per_bandclass_cdma2000[ncbc_index].arfcn;
        /* physCellIdList */
        p_cell_id_list = &(p_ncbcl->
            neigh_cells_per_bandclass_cdma2000[ncbc_index].phys_cell_id_list);

        if ((p_cell_id_list->count < 1) ||
            (p_cell_id_list->count > ARRSIZE(p_cell_id_list->
                phys_cell_id_cdma2000)))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Incorrect phys_cell_id_list_cdma2000_t count=%u. Should be 1."
                    ".%u",
                p_cell_id_list->count,
                ARRSIZE(p_cell_id_list->phys_cell_id_cdma2000)
            );
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        p_asn1_ncbcl_elem->physCellIdList.n = p_cell_id_list->count;
        for (id_index = 0; id_index < p_cell_id_list->count; id_index++)
        {
            p_asn1_ncbcl_elem->physCellIdList.elem[id_index] = p_cell_id_list->
                phys_cell_id_cdma2000[id_index];
        }
        rtxDListAppendNode(p_asn1_ncbcl, p_asn1_list_node);
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: fill_neigh_cell_list_cdma_2000
*   INPUT        : OSCTXT                       *p_asn1_ctx
*                  NeighCellListCDMA2000        *p_asn1_ncl
*                  neigh_cell_list_cdma2000_t   *p_ncl
*   OUTPUT       : none
*   DESCRIPTION  : This function converts neigh_cell_list_cdma2000_t to
*                  NeighCellListCDMA2000
*                  and checks whether input data are valid.
*
*   RETURNS      : RRC_SUCCESS - all data are valid
*                  RRC_FAILURE - data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et fill_neigh_cell_list_cdma_2000
(
    OSCTXT                       *p_asn1_ctx,    /* for memory allocation */
    NeighCellListCDMA2000        *p_asn1_ncl,    /* destination */
    neigh_cell_list_cdma2000_t   *p_ncl          /* source */
)
{
    rrc_counter_t          nc_info_index    = RRC_NULL;
    NeighCellCDMA2000     *p_asn1_ncl_elem  = PNULL;
    OSRTDListNode         *p_asn1_list_node = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_asn1_ncl);
    RRC_ASSERT(PNULL != p_ncl);

    if ((p_ncl->count < 1) || (p_ncl->count > ARRSIZE(p_ncl->
        neigh_cell_cdma2000)))
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Incorrect neigh_cell_list_cdma2000_t count=%u. Should be 1..%u",
            p_ncl->count,
            ARRSIZE(p_ncl->neigh_cell_cdma2000)
        );
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* Fill NeighCellCDMA2000 */
    rtxDListInit(p_asn1_ncl);
    for (nc_info_index = 0; nc_info_index < p_ncl->count; nc_info_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, NeighCellCDMA2000,
            &p_asn1_list_node, &p_asn1_ncl_elem);
        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        asn1Init_NeighCellCDMA2000(p_asn1_ncl_elem);
        p_asn1_ncl_elem->bandClass = p_ncl->neigh_cell_cdma2000[nc_info_index].
            band_class;
        if (RRC_SUCCESS !=
            fill_neigh_cells_per_freq_list(
                        p_asn1_ctx,
                        &p_asn1_ncl_elem->neighCellsPerFreqList,
                        &p_ncl->neigh_cell_cdma2000[nc_info_index].
                            neigh_cells_per_freq_list
                        ))
        {
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        rtxDListAppendNode(p_asn1_ncl, p_asn1_list_node);
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: check_and_convert_sib8_cell_reselection_params_cdma2000
*   INPUT        : OSCTXT                             *pctxt
*                  CellReselectionParametersCDMA2000  *p_asn1_data
*                  cell_reselection_params_cdma2000_t *p_data
*   OUTPUT       : none
*   DESCRIPTION  : This function convert cell_reselection_params_cdma2000_t to
*                  CellReselectionParametersCDMA2000 for SIB_8
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et check_and_convert_sib8_cell_reselection_params_cdma2000
(
    OSCTXT                             *pctxt,       /* for memory allocation */
    CellReselectionParametersCDMA2000  *p_asn1_data, /* destination */
    cell_reselection_params_cdma2000_t *p_data       /* source */
)
{
    RRC_CSC_UT_TRACE_ENTER();
    p_asn1_data->m.t_ReselectionCDMA2000_SFPresent = 0;
    /* bandClassList */
    if (RRC_SUCCESS !=
        fill_band_class_list_cdma_2000(pctxt, &(p_asn1_data->bandClassList), 
            &(p_data->band_class_list)))
    {
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* neighCellList */
    if (RRC_SUCCESS !=
        fill_neigh_cell_list_cdma_2000(pctxt, &(p_asn1_data->neighCellList),
            &(p_data->neigh_cell_list)))
    {
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* t_ReselectionCDMA2000 */
    p_asn1_data->t_ReselectionCDMA2000 = p_data->t_reselection_cdma2000;
    /* t_ReselectionCDMA2000_SF */
    if(p_data->presence_bitmask & CELL_RESELECT_CDMA2000_SF_PRESENCE_FLAG)
    {
        p_asn1_data->m.t_ReselectionCDMA2000_SFPresent  = 1;
        p_asn1_data->t_ReselectionCDMA2000_SF.sf_Medium = p_data->
            t_reselection_cdma2000_sf.sf_medium;
        p_asn1_data->t_ReselectionCDMA2000_SF.sf_High   = p_data->
            t_reselection_cdma2000_sf.sf_high;
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}
 
/******************************************************************************
*   FUNCTION NAME: csc_fill_neigh_cells_per_freq_list_v920
*   INPUT        : OSCTXT                                    *p_asn1_ctx
*                  NeighCellsPerBandclassListCDMA2000_v920        *p_asn1_ncbcl_v920
*                  neigh_cells_per_bandclass_list_cdma2000_v920_t *p_ncbcl_v920
*   OUTPUT       : none
*   DESCRIPTION  : This function converts neigh_cells_per_bandclass_list_cdma2000_v920_t to
*                  NeighCellsPerBandclassListCDMA2000_v920
*                  and checks whether input data are valid.
*
*   RETURNS      : RRC_SUCCESS - all data are valid
*                  RRC_FAILURE - data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et csc_fill_neigh_cells_per_freq_list_v920
(
    /* for memory allocation */
    OSCTXT                                    *p_asn1_ctx,
    NeighCellsPerBandclassListCDMA2000_v920        *p_asn1_ncbcl_v920, /* destination */
    neigh_cells_per_bandclass_list_cdma2000_v920_t *p_ncbcl_v920       /* source */
)
{
    rrc_counter_t                   ncbc_index        = RRC_NULL;
    rrc_counter_t                   id_index          = RRC_NULL;
    NeighCellsPerBandclassCDMA2000_v920 *p_asn1_ncbcl_elem_v920 = PNULL;
    OSRTDListNode                  *p_asn1_list_node  = PNULL;
    phys_cell_id_list_cdma2000_v920_t   *p_cell_id_list_v920    = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    if ((p_ncbcl_v920->count < 1) || (p_ncbcl_v920->count > ARRSIZE(p_ncbcl_v920->
        neigh_cells_per_bandclass_cdma2000_v920)))
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Incorrect neigh_cells_per_bandclass_list_cdma2000_v920_t count=%u. "
                "Should be 1..%u",
            p_ncbcl_v920->count,
            ARRSIZE(p_ncbcl_v920->neigh_cells_per_bandclass_cdma2000_v920)
        );
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* Fill NeighCellCDMA2000_v920 */
    rtxDListInit(p_asn1_ncbcl_v920);
    for (ncbc_index = 0; ncbc_index < p_ncbcl_v920->count; ncbc_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, NeighCellsPerBandclassCDMA2000_v920,
            &p_asn1_list_node, &p_asn1_ncbcl_elem_v920);
        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        asn1Init_NeighCellsPerBandclassCDMA2000_v920(p_asn1_ncbcl_elem_v920);
        /* physCellIdList_v920 */
        p_cell_id_list_v920 = &(p_ncbcl_v920->
            neigh_cells_per_bandclass_cdma2000_v920[ncbc_index].phys_cell_id_list_v920);

        if ((p_cell_id_list_v920->count < 1) ||
            (p_cell_id_list_v920->count > ARRSIZE(p_cell_id_list_v920->
                phys_cell_id_cdma2000_v920)))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                "Incorrect phys_cell_id_list_cdma2000_v920_t count=%u. Should be 1."
                    ".%u",
                p_cell_id_list_v920->count,
                ARRSIZE(p_cell_id_list_v920->phys_cell_id_cdma2000_v920)
            );
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        p_asn1_ncbcl_elem_v920->physCellIdList_v920.n = p_cell_id_list_v920->count;
        for (id_index = 0; id_index < p_cell_id_list_v920->count; id_index++)
        {
            p_asn1_ncbcl_elem_v920->physCellIdList_v920.elem[id_index] =
p_cell_id_list_v920->
                phys_cell_id_cdma2000_v920[id_index];
        }
        rtxDListAppendNode(p_asn1_ncbcl_v920, p_asn1_list_node);
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: csc_fill_neigh_cell_list_cdma_2000_v920
*   INPUT        : OSCTXT                                    *p_asn1_ctx
*                  NeighCellsPerBandclassListCDMA2000_v920        *p_asn1_ncbcl_v920
*                  neigh_cells_per_bandclass_list_cdma2000_v920_t *p_ncbcl_v920
*   OUTPUT       : none
*
*   DESCRIPTION  : This function converts neigh_cell_list_cdma2000_v920_t to
*                  NeighCellListCDMA2000_v920
*                  and checks whether input data are valid.
*
*   RETURNS      : RRC_SUCCESS - all data are valid
*                  RRC_FAILURE - data are invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et csc_fill_neigh_cell_list_cdma_2000_v920
(
    OSCTXT                       *p_asn1_ctx,    /* for memory allocation */
    NeighCellListCDMA2000_v920        *p_asn1_ncl_v920,    /* destination */
    neigh_cell_list_cdma2000_v920_t   *p_ncl_v920          /* source */
)
{
    rrc_counter_t          nc_info_index    = RRC_NULL;
    NeighCellCDMA2000_v920     *p_asn1_ncl_elem_v920  = PNULL;
    OSRTDListNode         *p_asn1_list_node = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_asn1_ctx);
    RRC_ASSERT(PNULL != p_asn1_ncl_v920);
    RRC_ASSERT(PNULL != p_ncl_v920);

    if ((p_ncl_v920->count < 1) || (p_ncl_v920->count > ARRSIZE(p_ncl_v920->
        neigh_cell_cdma2000_v920)))
    {
        RRC_CSC_TRACE(RRC_WARNING,
            "Incorrect neigh_cell_list_cdma2000_v920_t count=%u. Should be 1..%u",
            p_ncl_v920->count,
            ARRSIZE(p_ncl_v920->neigh_cell_cdma2000_v920)
        );
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* Fill NeighCellCDMA2000 */
    rtxDListInit(p_asn1_ncl_v920);
    for (nc_info_index = 0; nc_info_index < p_ncl_v920->count; nc_info_index++)
    {
        rtxDListAllocNodeAndData(p_asn1_ctx, NeighCellCDMA2000_v920,
            &p_asn1_list_node, &p_asn1_ncl_elem_v920);
        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        asn1Init_NeighCellCDMA2000_v920(p_asn1_ncl_elem_v920);
        if (RRC_SUCCESS !=
            csc_fill_neigh_cells_per_freq_list_v920(
                        p_asn1_ctx,
                        &p_asn1_ncl_elem_v920->neighCellsPerFreqList_v920,
                        &p_ncl_v920->neigh_cell_cdma2000_v920[nc_info_index].
                            neigh_cells_per_freq_list_v920
                        ))
        {
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        rtxDListAppendNode(p_asn1_ncl_v920, p_asn1_list_node);
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: csc_check_and_convert_sib8_cell_reselection_params_cdma2000_v920
*   INPUT        : OSCTXT                                    *p_asn1_ctx
*                  NeighCellsPerBandclassListCDMA2000_v920        *p_asn1_ncbcl_v920
*                  neigh_cells_per_bandclass_list_cdma2000_v920_t *p_ncbcl_v920
*   OUTPUT       : none
*
*   DESCRIPTION  : This function convert cell_reselection_params_cdma2000_v920_t to
*                  CellReselectionParametersCDMA2000_v920 for SIB_8
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
static rrc_return_et csc_check_and_convert_sib8_cell_reselection_params_cdma2000_v920
(
    OSCTXT                             *pctxt,       /* for memory allocation */
    CellReselectionParametersCDMA2000_v920  *p_asn1_data_v920, /* destination */
    cell_reselection_params_cdma2000_v920_t *p_data_v920       /* source */
)
{
    RRC_CSC_UT_TRACE_ENTER();
    /* neighCellList */
    if (RRC_SUCCESS !=
        csc_fill_neigh_cell_list_cdma_2000_v920(pctxt,
          &(p_asn1_data_v920->neighCellList_v920), &
            (p_data_v920->neigh_cell_list_v920)))
    {
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib8
*   INPUT        : OSCTXT                      *pctxt
*                  SystemInformationBlockType8 *p_asn1_sib8
*                  sib_type_8_Info_t           *p_sib8
*                  csc_cell_ctx_t              *p_cell_ctx
*                  U8                          si_segment_counter
*   OUTPUT       : none
*   DESCRIPTION  : This function convert sib_type_8_Info_t to
*                  SystemInformationBlockType8
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib8
(
    OSCTXT                      *pctxt,         /* for memory allocation */
    SystemInformationBlockType8 *p_asn1_sib8,   /* destination */
    sib_type_8_Info_t           *p_sib8,         /* source */
    csc_cell_ctx_t              *p_cell_ctx,    /* cell context */
    U8                          si_segment_counter /*segment counter*/
)
{
    U64 cdma_sys_time = RRC_NULL;
    /*SPR_6293 Start*/
    U64 long_code_state = RRC_NULL;
    /* Bug 12190 Fix Start */ 
    U8 bitPos = RRC_NULL;
    /* Bug 12190 Fix Stop */ 
    U8* p_buf  = PNULL;
    U32 src1 = RRC_NULL;
    U32 src2 = RRC_NULL;
    /*SPR_6293 Stop*/
    /* Bug 12190 Fix Start */ 
    U8** p_p_buf;
    U8 *p_bitPos;
    /* Bug 12190 Fix Stop */ 
  
   
    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_sib8);
    RRC_ASSERT(PNULL != p_asn1_sib8);

    /*Init*/
    asn1Init_SystemInformationBlockType8(p_asn1_sib8);

    /* Fill ASN.1 SIB8 message */

    p_asn1_sib8->m.systemTimeInfoPresent   = 0;
    p_asn1_sib8->m.searchWindowSizePresent = 0;
    p_asn1_sib8->m.parametersHRPDPresent   = 0;
    p_asn1_sib8->m.parameters1XRTTPresent  = 0;
    p_asn1_sib8->m.lateNonCriticalExtensionPresent = 0;
    p_asn1_sib8->m._v3ExtPresent = 0;
    p_asn1_sib8->m.csfb_SupportForDualRxUEs_r9Present = 0;
    p_asn1_sib8->m.csfb_RegistrationParam1XRTT_v920Present = 0;
    p_asn1_sib8->m.ac_BarringConfig1XRTT_r9Present = 0;
    
    p_asn1_sib8->m.cellReselectionParametersHRPD_v920Present = 0;
    p_asn1_sib8->m.cellReselectionParameters1XRTT_v920Present = 0;
    /*CDMA2000_CSFB:HO-start*/ 
    p_asn1_sib8->m.csfb_RegistrationParam1XRTT_v920Present = 0;
    /*CDMA2000_CSFB:HO-end*/ 

    /* systemTimeInfo */
    if (SIB_8_SYS_TIME_INFO_PRESENCE_FLAG & p_sib8->presence_bitmask)
    {
        p_asn1_sib8->m.systemTimeInfoPresent = 1;

/*sys_time_fix*/
        /*if(SYS_TIME_INFO_CDMA2000_SYNC_SYS_TIME_PRESENCE_FLAG & 
               p_sib8->sys_time_info.presence_bitmask) */
        if(RRC_TRUE == p_sib8->sys_time_info.cdma_eutra_sync)
        {
            p_cell_ctx->nw_in_sync = RRC_TRUE;
        }
        else
        {
            p_cell_ctx->nw_in_sync = RRC_FALSE;
        }
/*sys_time_fix*/

        if (RRC_SUCCESS !=
                check_and_convert_sys_time_info_cdma2000( pctxt,
                    &p_asn1_sib8->systemTimeInfo, p_cell_ctx, 
                    si_segment_counter, &cdma_sys_time ))
        {
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
        RRC_CSC_TRACE(RRC_INFO,"Valid CDMA System Time is being sent");
    }

    /* searchWindowSize */
    if (SIB_8_SEARCH_WIN_SIZE_PRESENCE_FLAG & p_sib8->presence_bitmask)
    {
        p_asn1_sib8->m.searchWindowSizePresent = 1;
        p_asn1_sib8->searchWindowSize = p_sib8->search_window_size;
    }
    /* parametersHRPD */
    if (SIB_8_PARAMS_HRPD_PRESENCE_FLAG & p_sib8->presence_bitmask)
    {
        p_asn1_sib8->m.parametersHRPDPresent = 1;
        p_asn1_sib8->parametersHRPD.m.cellReselectionParametersHRPDPresent = 0;
        /*# preRegistrationInfoHRPD */
        p_asn1_sib8->parametersHRPD.preRegistrationInfoHRPD.m.
            preRegistrationZoneIdPresent = 0;
        p_asn1_sib8->parametersHRPD.preRegistrationInfoHRPD.m.
            secondaryPreRegistrationZoneIdListPresent = 0;

        /*## preRegistrationAllowed */
        p_asn1_sib8->parametersHRPD.preRegistrationInfoHRPD.
            preRegistrationAllowed =
                                p_sib8->params_hrpd.pre_reg_info_hrpd.
                                    pre_reg_allowed;
        if (p_sib8->params_hrpd.pre_reg_info_hrpd.pre_reg_allowed &&
            !(p_sib8->params_hrpd.pre_reg_info_hrpd.presence_bitmask &
              PRE_REG_INFO_HRPD_ZONE_ID_PRESENCE_FLAG) )
        {
                RRC_CSC_TRACE(RRC_WARNING,
                    "struct PreRegistrationInfoHRPD. preRegistrationAllowed is"
                        " TRUE"
                    "and preRegistrationZoneId should be MANDATORY.");
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
        }
        /*## preRegistrationZoneId */
        if (p_sib8->params_hrpd.pre_reg_info_hrpd.presence_bitmask &
            PRE_REG_INFO_HRPD_ZONE_ID_PRESENCE_FLAG)
        {
            p_asn1_sib8->parametersHRPD.preRegistrationInfoHRPD.m.
                preRegistrationZoneIdPresent = 1;
            p_asn1_sib8->parametersHRPD.preRegistrationInfoHRPD.
                preRegistrationZoneId =
                p_sib8->params_hrpd.pre_reg_info_hrpd.pre_reg_zone_id;
        }
        /*## secondaryPreRegistrationZoneIdList */
        if (p_sib8->params_hrpd.pre_reg_info_hrpd.presence_bitmask &
            PRE_REG_INFO_HRPD_SECONDARY_LST_PRESENCE_FLAG)
        {
            PreRegistrationInfoHRPD *pElem =
                 &(p_asn1_sib8->parametersHRPD.preRegistrationInfoHRPD);
            pElem->m.secondaryPreRegistrationZoneIdListPresent = 1;
            pElem->secondaryPreRegistrationZoneIdList.n = p_sib8->params_hrpd.
                pre_reg_info_hrpd.secondary_list.count;
            l3_memcpy_wrapper(pElem->secondaryPreRegistrationZoneIdList.elem,
                   p_sib8->params_hrpd.pre_reg_info_hrpd.secondary_list.
                       pre_reg_zone_id,
                   sizeof(pElem->secondaryPreRegistrationZoneIdList.elem));
        }
        /*# cellReselectionParametersHRPD */
        if (p_sib8->params_hrpd.presence_bitmask &
            PARAMS_HRPD_CELL_RESELECTION_PARAMS_PRESENCE_FLAG)
        {
            p_asn1_sib8->parametersHRPD.m.cellReselectionParametersHRPDPresent =
                1;
            if (RRC_SUCCESS !=
                check_and_convert_sib8_cell_reselection_params_cdma2000(
                              pctxt,
                              &(p_asn1_sib8->parametersHRPD.
                                  cellReselectionParametersHRPD),
                              &(p_sib8->params_hrpd.
                                  cell_reselection_params_hrpd) ) )
            {
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
        }
    }
    /* parameters1XRTT */
    if (SIB_8_PARAMS_1_XRTT_PRESENCE_FLAG & p_sib8->presence_bitmask)
    {
        p_asn1_sib8->m.parameters1XRTTPresent = 1;

        p_asn1_sib8->parameters1XRTT.m.csfb_RegistrationParam1XRTTPresent    =
            0;
        p_asn1_sib8->parameters1XRTT.m.longCodeState1XRTTPresent             =
            0;
        p_asn1_sib8->parameters1XRTT.m.cellReselectionParameters1XRTTPresent =
            0;
        /*# csfb_RegistrationParam1XRTT */
        if (p_sib8->params_1_xrtt.presence_bitmask &
            CFSB_REG_PARAM_1_XRTT_PRESENCE_FLAG)
        {
            CSFB_RegistrationParam1XRTT *pElemXRTT =
                     &(p_asn1_sib8->parameters1XRTT.
                         csfb_RegistrationParam1XRTT);
            p_asn1_sib8->parameters1XRTT.m.csfb_RegistrationParam1XRTTPresent =
                1;
            /*## sid */
            pElemXRTT->sid.numbits = CSFB_REG_PARAM_1_XRTT_SID_NBITS;
            l3_memcpy_wrapper(pElemXRTT->sid.data, p_sib8->params_1_xrtt.
                csfb_reg_param_1_xrtt.sid,
                   sizeof(p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.sid));
            /*## nid */
            pElemXRTT->nid.numbits = CSFB_REG_PARAM_1_XRTT_NID_NBITS;
            l3_memcpy_wrapper(pElemXRTT->nid.data, p_sib8->params_1_xrtt.
                csfb_reg_param_1_xrtt.nid,
                   sizeof(p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.nid));
            /*## multipleSID */
            pElemXRTT->multipleSID = p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt
                .multiple_sid;
            /*## multipleNID */
            pElemXRTT->multipleNID = p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt
                .multiple_nid;
            /*## homeReg */
            pElemXRTT->homeReg     = p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt
                .home_reg;
            /*## foreignSIDReg */
            pElemXRTT->foreignSIDReg = p_sib8->params_1_xrtt.
                csfb_reg_param_1_xrtt.foreign_sid_reg;
            /*## foreignNIDReg */
            pElemXRTT->foreignNIDReg = p_sib8->params_1_xrtt.
                csfb_reg_param_1_xrtt.foreign_nid_reg;
            /*## parameterReg */
            pElemXRTT->parameterReg  = p_sib8->params_1_xrtt.
                csfb_reg_param_1_xrtt.parame_reg;
            /*## powerUpReg */
            pElemXRTT->powerUpReg    = p_sib8->params_1_xrtt.
                csfb_reg_param_1_xrtt.power_up_reg;
            /*## registrationPeriod */
            pElemXRTT->registrationPeriod.numbits = CSFB_REG_REG_PERIOD_NBITS;
            l3_memcpy_wrapper(pElemXRTT->registrationPeriod.data,
                   p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.reg_period,
                   sizeof(p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.reg_period));
            /*## registrationZone */
            pElemXRTT->registrationZone.numbits = CSFB_REG_REG_ZONE_NBITS;
            l3_memcpy_wrapper(pElemXRTT->registrationZone.data,
                   p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.reg_zone,
                   sizeof(p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.reg_zone));
            /*## totalZone */
            pElemXRTT->totalZone.numbits = CSFB_REG_TOTAL_ZONE_NBITS;
            l3_memcpy_wrapper(pElemXRTT->totalZone.data,
                   p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.total_zone,
                   sizeof(p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.total_zone));
            /*## zoneTimer */
            pElemXRTT->zoneTimer.numbits = CSFB_REG_ZONE_TIMER_NBITS;
            l3_memcpy_wrapper(pElemXRTT->zoneTimer.data,
                   p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.zone_timer,
                   sizeof(p_sib8->params_1_xrtt.csfb_reg_param_1_xrtt.zone_timer));
        }
        /*# longCodeState1XRTT */
        if (p_sib8->params_1_xrtt.presence_bitmask &
                LONG_CODE_STATE_1_XRTT_PRESENCE_FLAG)
        {
            /*SPR_6293 Start*/
            if((RRC_TRUE != p_cell_ctx->cell_setup_done) || 
                    !(SIB_8_SYS_TIME_INFO_PRESENCE_FLAG & p_sib8->presence_bitmask))
            {
                p_asn1_sib8->parameters1XRTT.m.longCodeState1XRTTPresent = 1;
                p_asn1_sib8->parameters1XRTT.longCodeState1XRTT.numbits =
                    LONG_CODE_STATE_1_XRTT_NBITS;
                memset_wrapper(p_asn1_sib8->parameters1XRTT.longCodeState1XRTT.data,0,
                        sizeof(p_asn1_sib8->parameters1XRTT.longCodeState1XRTT.data));
            }
            else
            {
                p_asn1_sib8->parameters1XRTT.m.longCodeState1XRTTPresent = 1;
                p_buf = p_asn1_sib8->parameters1XRTT.longCodeState1XRTT.data;
                p_asn1_sib8->parameters1XRTT.longCodeState1XRTT.numbits =
                    LONG_CODE_STATE_1_XRTT_NBITS;
                long_code_state = (cdma_sys_time/10)*10+320; /*As defined in RRC
                                                               spec 36.331 section 6.3.1*/

                src1 = (U32)(long_code_state >> MAX_INT_NBITS);
                src2 = (U32)(long_code_state << MAX_INT_NBITS >> MAX_INT_NBITS);
                /* long code state (MAX 42 bits) packed into two parts
                   last 10 bits and first 32 bits */
                /* Bug 12190 Fix Start */ 
                p_p_buf = (U8 **)&p_buf;

                p_bitPos = (U8 *)&bitPos;      

                SETNBIT( p_p_buf, /*p_buf*/
                        p_bitPos,
                        src1,
                        (LONG_CODE_STATE_1_XRTT_NBITS - MAX_INT_NBITS),
                        /*width*/
                        PNULL);
                SETNBIT( p_p_buf, /*p_buf*/
                        p_bitPos,
                        src2,
                        MAX_INT_NBITS, /*width*/
                        PNULL);
                /* Bug 12190 Fix Stop */ 

                RRC_CSC_TRACE(RRC_DETAILED,"Long Code State [%llu]",
                        long_code_state);
            }
            /*SPR_6293 Stop*/
        }
        /*# cellReselectionParameters1XRTT*/
        if (p_sib8->params_1_xrtt.presence_bitmask &
            CELL_RESELECTION_PARAM_1_XRTT_PRESENCE_FLAG)
        {
            p_asn1_sib8->parameters1XRTT.m.cellReselectionParameters1XRTTPresent
                = 1;
            if (RRC_SUCCESS !=
                check_and_convert_sib8_cell_reselection_params_cdma2000(
                              pctxt,
                              &(p_asn1_sib8->parameters1XRTT.
                                  cellReselectionParameters1XRTT),
                              &(p_sib8->params_1_xrtt.
                                  cell_reselection_params_1_xrtt) ) )
            {
                RRC_CSC_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
        }
    }
    
    /*#cellReselectionParametersHRPD_v920*/

    if (SIB_8_CELL_RESEL_PARAMS_HRPD_V920_PRESENCE_FLAG & 
              p_sib8->presence_bitmask)
    {
      p_asn1_sib8->m._v3ExtPresent = 1;
      if(PARAMS_HRPD_CELL_RESELECTION_PARAMS_PRESENCE_FLAG &
              p_sib8->params_hrpd.presence_bitmask)
      {
        p_asn1_sib8->m.cellReselectionParametersHRPD_v920Present = 1;

        if (RRC_SUCCESS !=
          csc_check_and_convert_sib8_cell_reselection_params_cdma2000_v920(
            pctxt,
            &(p_asn1_sib8->cellReselectionParametersHRPD_v920),
            &(p_sib8->cell_reselection_params_hrpd_v920) ) )
        {
          RRC_CSC_UT_TRACE_EXIT();
          return RRC_FAILURE;
        }
      }
      else
      {
        RRC_CSC_TRACE(RRC_WARNING,
            "cellReselectionParametersHRPD is missing so "
            "cellReselectionParametersHRPD_v920 is not filling");
      }
    }

    /*#cellReselectionParameters1XRTT_v920*/
    if (SIB_8_CELL_RESEL_PARAMS_1_XRTT_V920_PRESENCE_FLAG & 
        p_sib8->presence_bitmask) 
    {
      p_asn1_sib8->m._v3ExtPresent = 1;
      if(CELL_RESELECTION_PARAM_1_XRTT_PRESENCE_FLAG  &
        p_sib8->params_1_xrtt.presence_bitmask)
      {
        p_asn1_sib8->m.cellReselectionParameters1XRTT_v920Present = 1;

        if (RRC_SUCCESS !=
          csc_check_and_convert_sib8_cell_reselection_params_cdma2000_v920(
            pctxt,
            &(p_asn1_sib8->cellReselectionParameters1XRTT_v920),
            &(p_sib8->cell_reselection_params_1_xrtt_v920) ) )
        {
          RRC_CSC_UT_TRACE_EXIT();
          return RRC_FAILURE;
        }
      }
      else
      {
        RRC_CSC_TRACE(RRC_WARNING,
            "cellReselectionParameters1XRTT is missing so "
            "cellReselectionParameters1XRTT_v920 is not filling");
      }
    }

    /*cr_309 csfb_SupportForDualRxUEs_r9 */
    if (SIB_8_CSFB_SUPPORT_FOR_DUAL_RX_UES_R9_PRESENCE_FLAG & 
            p_sib8->presence_bitmask) 
    {
        p_asn1_sib8->m._v3ExtPresent = 1;
        p_asn1_sib8->m.csfb_SupportForDualRxUEs_r9Present = 1;
        p_asn1_sib8->csfb_SupportForDualRxUEs_r9 = 
            p_sib8->csfb_support_for_dual_rx_ues_r9;
    }
   /*cr_374 AC_BarringConfig1XRTT_r9 */
    if (SIB_8_AC_BARRING_CONFIG_1_XRTT_R9_PRESENCE_FLAG & 
        p_sib8->presence_bitmask) 
    {
        p_asn1_sib8->m._v3ExtPresent = 1;
        p_asn1_sib8->m.ac_BarringConfig1XRTT_r9Present = 1;
      
        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_Barring0to9_r9 = 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_0_to_9_r9;

        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_Barring10_r9 = 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_10_r9;

        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_Barring11_r9 = 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_11_r9;

        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_Barring12_r9 = 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_12_r9;

        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_Barring13_r9= 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_13_r9;

        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_Barring14_r9 = 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_14_r9;

        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_Barring15_r9 = 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_15_r9;
        
        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_BarringMsg_r9 = 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_msg_r9;

        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_BarringReg_r9 = 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_reg_r9;

        p_asn1_sib8->ac_BarringConfig1XRTT_r9.ac_BarringEmg_r9 = 
            p_sib8->ac_barring_config_1_xrtt_r9.ac_barring_emg_r9;
    }
    /*CDMA2000_CSFB:HO-start*/ 
    /*csfb_RegistrationParam1XRTT_v920*/
    if (SIB_8_CSFB_REG_PARAM_1_XRTT_R9_PRESENCE_FLAG & 
            p_sib8->presence_bitmask) 
    {
        p_asn1_sib8->m._v3ExtPresent = 1;
        p_asn1_sib8->m.csfb_RegistrationParam1XRTT_v920Present = 1;
        p_asn1_sib8->csfb_RegistrationParam1XRTT_v920.powerDownReg_r9 = 
            p_sib8->csfb_registration_param_1xrtt_v920;
    }
    /*CDMA2000_CSFB:HO-end*/ 
    
    /* CR 487 changes start*/
    if (SIB_8_CSFB_DUAL_RX_TX_SUPPORT_R10_PRESENCE_FLAG &
            p_sib8->presence_bitmask)
    {
        p_asn1_sib8->m._v4ExtPresent = 1;
        p_asn1_sib8->m.csfb_DualRxTxSupport_r10Present = 1;
        p_asn1_sib8->csfb_DualRxTxSupport_r10 = p_sib8->
            csfb_dual_rx_tx_support_r10;
    }
    /* CR 487 changes stop*/


    rtxDListInit(&p_asn1_sib8->extElem1);

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib9
*   INPUT        : OSCTXT                      *pctxt
*                  SystemInformationBlockType9 *p_asn1_sib9
*                  sib_type_9_Info_t           *p_sib9
*   OUTPUT       : none
*   DESCRIPTION  : This function convert sib_type_9_Info_t to
*                  SystemInformationBlockType9
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib9
(
    SystemInformationBlockType9 *p_asn1_sib9,   /* destination */
    sib_type_9_Info_t           *p_sib9         /* source */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    /*Init*/
    asn1Init_SystemInformationBlockType9(p_asn1_sib9);

    p_asn1_sib9->m.hnb_NamePresent = 0;

    if (p_sib9->presence_bitmask & SIB_9_HNB_ID_PRESENCE_FLAG)
    {
        p_asn1_sib9->m.hnb_NamePresent = 1;

        if (    (p_sib9->size_of_hnb_id < 1) ||
                (p_sib9->size_of_hnb_id > sizeof(p_sib9->hnb_id)) ||
                (p_sib9->size_of_hnb_id > sizeof(p_asn1_sib9->hnb_Name.data)))
        {
            RRC_CSC_TRACE(RRC_WARNING,
                    "Wrong value of p_sib9->size_of_hnb_id=%u. Should be 1.."
                        "min(RRM=%u, ASN.1=%u)",
                    p_sib9->size_of_hnb_id,
                    sizeof(p_sib9->hnb_id),
                    sizeof(p_asn1_sib9->hnb_Name.data));
            RRC_CSC_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        p_asn1_sib9->hnb_Name.numocts = p_sib9->size_of_hnb_id;
        l3_memcpy_wrapper(p_asn1_sib9->hnb_Name.data, p_sib9->hnb_id, p_sib9->
            size_of_hnb_id);
    }
    rtxDListInit(&p_asn1_sib9->extElem1);

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib10
*   INPUT        : OSCTXT                          *pctxt
*                  SystemInformationBlockType10    *p_asn1_sib10
*                  sib_type_10_Info_t              *p_sib10
*   OUTPUT       : none
*   DESCRIPTION  : This function convert sib_type_10_Info_t to
*                  SystemInformationBlockType10
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib10
(
    SystemInformationBlockType10    *p_asn1_sib10,  /* destination */
    sib_type_10_Info_t              *p_sib10        /* source */
)
{
    RRC_CSC_UT_TRACE_ENTER();

    if ((PNULL == p_asn1_sib10) || (PNULL == p_sib10))
    {
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /*Init*/
    asn1Init_SystemInformationBlockType10(p_asn1_sib10);

    p_asn1_sib10->m.dummyPresent = 0;

    p_asn1_sib10->messageIdentifier.numbits = sizeof(p_sib10->msg_id) * 8;
    l3_memcpy_wrapper(p_asn1_sib10->messageIdentifier.data, p_sib10->msg_id, sizeof
        (p_sib10->msg_id));

    p_asn1_sib10->serialNumber.numbits = sizeof(p_sib10->serial_number) * 8;
    l3_memcpy_wrapper(p_asn1_sib10->serialNumber.data, p_sib10->serial_number, sizeof
        (p_sib10->serial_number));

    p_asn1_sib10->warningType.numocts = WARNING_TYPE_OCTET_SIZE /* 2 */;
    if ((p_asn1_sib10->warningType.numocts) > 2)
    {
        RRC_CSC_TRACE(RRC_WARNING,
                "value of warningType.numocts should not greater than 2");
        RRC_CSC_UT_TRACE_EXIT();
        return RRC_FAILURE;
    } 
    l3_memcpy_wrapper(p_asn1_sib10->warningType.data, p_sib10->warning_type, sizeof
        (p_sib10->warning_type));

    if (p_sib10->presence_bitmask & SIB_10_WARN_SEC_INFO_PRESENCE_FLAG)
    {
        p_asn1_sib10->m.dummyPresent = 1;
        /*CID 109524 +*/
        p_asn1_sib10->dummy.numocts = sizeof(p_sib10->
            warning_security_info) /* 50 */;
        l3_memcpy_wrapper(p_asn1_sib10->dummy.data, p_sib10->
            warning_security_info, sizeof(p_sib10->warning_security_info));
        /*CID 109524 -*/
    }

    rtxDListInit(&p_asn1_sib10->extElem1);

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib11
*   INPUT        : OSCTXT                        *pctxt
*                  SystemInformationBlockType11  *p_asn1_sib11
*                  sib_type_11_Info_t              *p_sib11
*                  U8                            segment_number
*                  U16                            warning_msg_segment_size
*                  U32                           warning_msg_segment_type
*   OUTPUT       : none
*   DESCRIPTION  : This function convert sib_type_11_Info_t to
 *                 sib11_info
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib11
(
    OSCTXT                        *pctxt,         /* for memory allocation */
    SystemInformationBlockType11  *p_asn1_sib11,  /* destination */
    sib_type_11_Info_t              *p_sib11,        /* source */
    U8                            segment_number,  /* Segment number */
    U16                            warning_msg_segment_size, /* warning msg segment size */
    /* Bug_15359 fix starts */
    U32                           warning_msg_segment_type, /* warning msg segment type */
    U16                            segment_size /* last segment start */ 
    /* Bug_15359 fix ends */
)
{
  U8                            *p_warning_msg = PNULL;

  RRC_CSC_UT_TRACE_ENTER();
	/* Bug_15359 fix starts */
	p_warning_msg = p_sib11->warning_msg + ((segment_number-1)*segment_size); 
	/* Bug_15359 fix ends */

  asn1Init_SystemInformationBlockType11(p_asn1_sib11);

  p_asn1_sib11->m.dataCodingSchemePresent = 0;

  /* Filling the message id */
  p_asn1_sib11->messageIdentifier.numbits = sizeof(p_sib11->msg_id) * 8;
  l3_memcpy_wrapper(p_asn1_sib11->messageIdentifier.data, p_sib11->msg_id, sizeof
        (p_sib11->msg_id));

  /* Filling the serial number */
  p_asn1_sib11->serialNumber.numbits = sizeof(p_sib11->serial_number) * 8;
  l3_memcpy_wrapper(p_asn1_sib11->serialNumber.data, p_sib11->serial_number,
        sizeof(p_sib11->serial_number));

  /* Filling the warning segment type*/
  p_asn1_sib11->warningMessageSegmentType = warning_msg_segment_type;

  /* Filling the segment count*/
  p_asn1_sib11->warningMessageSegmentNumber = (OSUINT8)(segment_number - 1);

  p_asn1_sib11->warningMessageSegment.numocts = warning_msg_segment_size;

  p_asn1_sib11->warningMessageSegment.data =
        rtxMemAlloc(pctxt, p_asn1_sib11->
              warningMessageSegment.numocts);
  if (PNULL == p_asn1_sib11->warningMessageSegment.data)
  {
    /* Not enough memory */
    return RRC_FAILURE;
  }
  l3_memcpy_wrapper((OSOCTET *)p_asn1_sib11->
        warningMessageSegment.data,
        p_warning_msg,
        p_asn1_sib11->warningMessageSegment.numocts);

  /* SPR 12475 Fix Start */
  if ( (p_sib11->presence_bitmask & SIB_11_DATA_COD_SCHEME_PRESENCE_FLAG) &&
          (1 == segment_number) )
  /* SPR 12475 Fix Stop */
  {
    p_asn1_sib11->m.dataCodingSchemePresent = 1;
    p_asn1_sib11->dataCodingScheme.numocts = sizeof(p_sib11->
            data_coding_scheme) /* 1 */;

    l3_memcpy_wrapper(p_asn1_sib11->dataCodingScheme.data, p_sib11->
         data_coding_scheme, sizeof(p_sib11->data_coding_scheme));
  }
  rtxDListInit(&p_asn1_sib11->extElem1);
  RRC_CSC_UT_TRACE_EXIT();
  return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_check_and_convert_sib12
*   INPUT        : OSCTXT                        *pctxt
*                  SystemInformationBlockType11  *p_asn1_sib11
*                  sib_type_11_Info_t              *p_sib11
*                  U8                            segment_number
*                  U16                            warning_msg_segment_size
*                  U32                           warning_msg_segment_type
*   OUTPUT       : none
*
*   DESCRIPTION  : This function convert sib_type_12_Info_t to
 *                 sib12_info
*                  and check if input data are valid.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid
*                  RRC_FAILURE - if data is invalid or if there are not enough memory
*
******************************************************************************/
rrc_return_et bch_m_check_and_convert_sib12
(
    OSCTXT                          *pctxt,         /* for memory allocation */
    SystemInformationBlockType12_r9 *p_asn1_sib12,  /* destination */
    sib_type_12_Info_t              *p_sib12,        /* source */
    U8                            segment_number,  /* Segment number */
    U16                            warning_msg_segment_size, /* warning msg segment size */
    U32                           warning_msg_segment_type /* warning msg segment type */
)
{
  U8                            *p_warning_msg = PNULL;

  RRC_CSC_UT_TRACE_ENTER();

  p_warning_msg = p_sib12->warning_msg + ((segment_number-1)*warning_msg_segment_size);

  /*Init*/
  asn1Init_SystemInformationBlockType12_r9(p_asn1_sib12);

  p_asn1_sib12->m.dataCodingScheme_r9Present = 0;

  /* Filling the message id */
  p_asn1_sib12->messageIdentifier_r9.numbits = sizeof(p_sib12->msg_id) * 8;
  l3_memcpy_wrapper(p_asn1_sib12->messageIdentifier_r9.data, p_sib12->msg_id, sizeof
        (p_sib12->msg_id));

  /* Filling the serial number */
  p_asn1_sib12->serialNumber_r9.numbits = sizeof(p_sib12->serial_number) * 8;
  l3_memcpy_wrapper(p_asn1_sib12->serialNumber_r9.data, p_sib12->serial_number,
        sizeof(p_sib12->serial_number));

  /* Filling the warning segment type*/
  p_asn1_sib12->warningMessageSegmentType_r9 = warning_msg_segment_type;

  /* Filling the segment count*/
  p_asn1_sib12->warningMessageSegmentNumber_r9 = (OSUINT8)(segment_number - 1);

  p_asn1_sib12->warningMessageSegment_r9.numocts = warning_msg_segment_size;

  p_asn1_sib12->warningMessageSegment_r9.data =
        rtxMemAlloc(pctxt, p_asn1_sib12->
              warningMessageSegment_r9.numocts);
  if (PNULL == p_asn1_sib12->warningMessageSegment_r9.data)
  {
    /* Not enough memory */
    return RRC_FAILURE;
  }
  l3_memcpy_wrapper((OSOCTET *)p_asn1_sib12->
        warningMessageSegment_r9.data,
        p_warning_msg,
        p_asn1_sib12->warningMessageSegment_r9.numocts);


  /* SPR 12475 Fix Start */
  if ( (p_sib12->presence_bitmask & SIB_12_DATA_COD_SCHEME_PRESENCE_FLAG) &&
             (1 == segment_number) )
  /* SPR 12475 Fix Stop */
  {
    p_asn1_sib12->m.dataCodingScheme_r9Present = 1;
    p_asn1_sib12->dataCodingScheme_r9.numocts = sizeof(p_sib12->
            data_coding_scheme) /* 1 */;

    l3_memcpy_wrapper(p_asn1_sib12->dataCodingScheme_r9.data, p_sib12->
         data_coding_scheme, sizeof(p_sib12->data_coding_scheme));
  }
  rtxDListInit(&p_asn1_sib12->extElem1);
  RRC_CSC_UT_TRACE_EXIT();
  return RRC_SUCCESS;
}

/*
*   Special check function
*/
/******************************************************************************
*   FUNCTION NAME: si_window_length_2_sf
*   INPUT        : U8 si_window_length
*   OUTPUT       : none
*   DESCRIPTION  : This function convert si_window_length from enum representation
*                  (ASN.1 == RRM) to sf == ms
*   RETURNS      : si_window_length in subframes
******************************************************************************/
static U8 si_window_length_2_sf
(
    U8 si_window_length
)
{
    switch (si_window_length)
    {
    case RRM_RRC_SI_WINDOW_LEN_MS_1:
        return 1;

    case RRM_RRC_SI_WINDOW_LEN_MS_2:
        return 2;

    case RRM_RRC_SI_WINDOW_LEN_MS_5:
        return 5;

    case RRM_RRC_SI_WINDOW_LEN_MS_10:
        return 10;

    case RRM_RRC_SI_WINDOW_LEN_MS_15:
        return 15;

    case RRM_RRC_SI_WINDOW_LEN_MS_20:
        return 20;

    case RRM_RRC_SI_WINDOW_LEN_MS_40:
        return 40;

    default:
        RRC_ASSERT(0);
        RRC_CSC_TRACE(RRC_ERROR, "Unexpected value of si_window_length. "
            "Treated as 2ms.");
        return 2;
    }
}

/******************************************************************************
*   FUNCTION NAME: check_sis_scheduling
*   INPUT        : sib_type_1_Info_t *p_sib1
*   OUTPUT       : none  
*   DESCRIPTION  : This function fully checks SIs scheduling
*                  (in additional to already done in bch_m_check_and_convert_sib1)
*                  in order to encode_sis function could just relay on it
*                  and sets proper sfn and sf for all SIs.
*                  All calculation done in sf == ms; 1 sfn == 10 sf.
*
*                  Precondition: it should be already checked:
*                  si_WindowLength range,
*                  number of SIs, si_Periodicity range for each SI,
*                  number of SIBs in SI and that only known (up to SIB11) and present in
*                  message from RRM (see rrc_rrm_cell_setup_req_t.presence_bitmask)
*                  SIBs are mapped only once to SIs.
*
*                  RRC_FAILURE - if SIs scheduling is not OK
*
******************************************************************************/
static rrc_return_et check_sis_scheduling
(
    /* all SIs scheduling information is in SIB1 */
    sib_type_1_Info_t *p_sib1
)
{
    U8                      si                  = RRC_NULL;
    U16                     x                   = RRC_NULL;
    U16                     sf                  = RRC_NULL;
    U16                     si_periodicity_sf   = RRC_NULL;
    U8                      si_window_length_sf = si_window_length_2_sf(p_sib1->
        si_window_length);
  scheduling_info_list_t   *p_sched_inf        = &p_sib1->scheduling_info_list;
    rrc_bool_et             is_sched_ok         = RRC_TRUE;
    static U8                      time_line[SF_UPPER_LIMIT];

    RRC_CSC_UT_TRACE_ENTER();

    /* time_line - initially free. 0 - free, 1 busy */
    memset_wrapper(time_line, 0, sizeof(time_line));

    /* See TS 36.331-840, p20, p27 */

    /* Already should be checked */
  if(p_sched_inf->count > ARRSIZE(p_sched_inf->scheduling_info))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Scheduling information count is greater"
        " than Arrsize %d ",ARRSIZE(p_sched_inf->scheduling_info));
        return RRC_FAILURE;
    }

    if(p_sched_inf->count < 1)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Scheduling information count is less than 1");
        return RRC_FAILURE;
    }

    for (si = 0; (si < p_sched_inf->count) && (RRC_TRUE == is_sched_ok); si++)
    {
        /* The first sf, where this SI is scheduled */
        x = (U16)(si * si_window_length_sf);
        /* Convert si_periodicity to sf */
        si_periodicity_sf = (U16)((8 << p_sched_inf->scheduling_info[si].
            si_periodicity) * 10);
        /* Check possibility of: the SI-window starts ... in the radio frame for
         *  which SFN mod T = FLOOR(x/10), where T is the si-Periodicity */
        if (x >= si_periodicity_sf)
        {
            is_sched_ok = RRC_FALSE;
            break;
        }

        /* Check that: SI-windows of different SI messages do not overlap. */
        /* And mark all sf where this SI is scheduled as busy */
        for (; (x < SF_UPPER_LIMIT) && (RRC_TRUE == is_sched_ok); x = (U16)(x +
            si_periodicity_sf))
        {
            for (sf = x; (sf < x + si_window_length_sf) && (sf <
                SF_UPPER_LIMIT); ++sf)
            {
                if (time_line[sf])
                {
                    /* Already busy */
                    is_sched_ok = RRC_FALSE;
                    break;
                }
                else
                {
                    /* Mark sf as busy */
                    time_line[sf] = 1;
                }
            }
        }
    }

    /*
    * Special case: E-UTRAN should configure an SI-window of 1ms only if
    * all SIs are scheduled before sub-frame #5 in radio frames
    *  for which SFN mod 2 = 0.
    * I.e. with periodicity SF==20, from sub-frame #5 to sub-frame #9 (next SFN)
    * SIs shouldn't be scheduled.
    */
    if (1 == si_window_length_sf)
    {
        for (x = 5; (x < SF_UPPER_LIMIT) && (RRC_TRUE == is_sched_ok); x = (U16)(x + 20))
        {
            for (sf = x; (sf < x + 5) && (sf < SF_UPPER_LIMIT); ++sf)
            {
                if (time_line[sf])
                {
                    /* Busy */
                    is_sched_ok = RRC_FALSE;
                    break;
                }
            }
        }
    }

    RRC_CSC_UT_TRACE_EXIT();
    if(is_sched_ok == RRC_TRUE)
    {
        return(RRC_SUCCESS);
    }
    else
    {
        return(RRC_FAILURE);
    }
}

/*
*   Here we have encode functions
*/
/******************************************************************************
*   FUNCTION NAME: encode_mib
*   INPUT        : OSCTXT                          *pctxt
*                  rrc_csc_llim_config_cell_req_t  *p_llim_cfg_cell_req,
*                  mib_info_t                      *p_mib_info
*                  csc_cell_ctx_t                  *p_cell_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function encodes MIBs for all possible values sfn where MIB is
*                  scheduled.
*   RETURNS      : RRC_SUCCESS - if successfully encodes all MIBs.
*                  RRC_FAILURE - otherwise
*
******************************************************************************/
static rrc_return_et encode_mib
(
    /* for memory allocation */
    OSCTXT                          *pctxt,
    rrc_csc_llim_config_cell_req_t  *p_llim_cfg_cell_req,   /* destination */
    mib_info_t                      *p_mib_info,             /* source */
    csc_cell_ctx_t                  *p_cell_ctx
)
{
    rrc_return_et ret_val               = RRC_SUCCESS;
    rrc_mac_mib_msg_req_t *p_dst_mib    = PNULL;
    S32 enc_res                         = RRC_NULL;
    U8 mib_sfn                          = RRC_NULL;
    /* i_mib should be able to hold RRC_MIB_MSGS */
    U16 i_mib                           = RRC_NULL;
    BCCH_BCH_Message asn1_mib_msg;
    U32 rrc_mib_print_flag = 0;
    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
    U32                     encoded_msg_len = 0;
    LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT *p_event_rrc_protocol_err = PNULL;


    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_llim_cfg_cell_req);
    RRC_ASSERT(PNULL != p_mib_info);

    /* In this case we always have the same number of MIBs during
     *  SFN_UPPER_LIMIT and in the same places */
    RRC_ASSERT(0 == (SFN_UPPER_LIMIT % MIB_PERIODICITY));
    /* Output message should be able to carry all MIBs */
    RRC_ASSERT(RRC_MIB_MSGS == (SFN_UPPER_LIMIT / MIB_PERIODICITY));
    RRC_ASSERT(RRC_MIB_MSGS == ARRSIZE(p_llim_cfg_cell_req->mib_msg_req));
    p_llim_cfg_cell_req->mib_msg_req_counter = RRC_MIB_MSGS;
    /* eMTC changes stop */
    
    /* Fill MIB with values from p_mib_info */
    memset_wrapper(&asn1_mib_msg, 0, sizeof(asn1_mib_msg));

    asn1_mib_msg.message.dl_Bandwidth                   = p_mib_info->
        dl_band_width;
    asn1_mib_msg.message.phich_Config.phich_Duration    = p_mib_info->
        phich_config.phich_duration;
    asn1_mib_msg.message.phich_Config.phich_Resource    = p_mib_info->
        phich_config.phich_resource;

    asn1_mib_msg.message.systemFrameNumber.numbits      = 8;
    asn1_mib_msg.message.systemFrameNumber.data[0]      = RRC_NULL;

    asn1_mib_msg.message.schedulingInfoSIB1_BR_r13  = 0;
    asn1_mib_msg.message.spare.numbits                  = 4;
    asn1_mib_msg.message.spare.data[0]                  = 0;

    for (i_mib = 0; i_mib < RRC_MIB_MSGS; i_mib++)
    {
        /* MIB is scheduled in subframe #0 of radio frames for which
         *  the SFN mod 4 = 0 */
        /* 0, MIB_PERIODICITY, 2 * MIB_PERIODICITY, etc. */
        p_dst_mib = p_llim_cfg_cell_req->mib_msg_req + i_mib;

        p_dst_mib->sfn  = (rrc_sfn_t)(i_mib * MIB_PERIODICITY);

        /* In MIB we store 8 (from 10) most significant bits of the SFN */
        mib_sfn = (U8)(p_dst_mib->sfn >> SFN_2_MIB_SFN_SHIFT);

        /* Fill MIB SFN */
        asn1_mib_msg.message.systemFrameNumber.data[0] = mib_sfn;

        /* Encode MIB */
        pu_setBuffer(pctxt, p_dst_mib->mib_msg_buf, sizeof(p_dst_mib->
            mib_msg_buf), ASN_RRC_BUF_TYPE);

        enc_res = asn1PE_BCCH_BCH_Message(pctxt, &asn1_mib_msg);

        if (RT_OK != enc_res)
        {
            RRC_CSC_TRACE(RRC_WARNING, "Can't encode MIB #%u", mib_sfn);

            ret_val= RRC_FAILURE;
                
	    /*Generating Protocol Event LOCAL_EVENT_RRC_PROTOCOL_ERROR*/
            p_event_rrc_protocol_err = rrc_mem_get(sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));
            if (PNULL != p_event_rrc_protocol_err)
            {
                memset_wrapper(p_event_rrc_protocol_err, RRC_NULL, 
                    sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));

                p_event_rrc_protocol_err->header.length = 
                    sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT);
                p_event_rrc_protocol_err->header.event_id = LOCAL_EVENT_RRC_PROTOCOL_ERROR;

                l3_strncpy_wrapper(((S8 *)(p_event_rrc_protocol_err->EVENT_PARAM_RRC_MESSAGE_DISCARDED)),
                    ((const S8 *)"MASTER INFORMATION BLOCK"), RRC_MESSAGE_DISCARDED_SIZE);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_rrc_protocol_err);
            }
            break;
        }
        else
        {
            if(rrc_mib_print_flag == 0)
            {
                /* Generating Protocol Event LOCAL_RRC_MASTER_INFORMATION_BLOCK */    
                encoded_msg_len = (U32)pe_GetMsgLen(pctxt);

                rrc_mib_print_flag++;
                rrc_asn1PrtToStr_BCCH_BCH_Message(RRC_ASN,"BCCH_BCH_Message", 
                  &asn1_mib_msg, "MasterInformationBlock", p_dst_mib->mib_msg_buf, encoded_msg_len);

                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));
                    p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + 
                            encoded_msg_len);
                    p_event_header->event_id = LOCAL_RRC_MASTER_INFORMATION_BLOCK;
                    p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
                    p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;


                    l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID, p_cell_ctx->cfg_cell_param.
                            sib_type_1_Info.cell_access_related_info.cell_Id, sizeof(U32));

                    p_event_header->EVENT_PARAM_EUTRANCELL_FROID = p_event_header->
                        EVENT_PARAM_CELL_ID;

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)p_dst_mib->mib_msg_buf,
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }

                /* Generating Protocol Event LOCAL_RRC_MASTER_INFORMATION_BLOCK */    
                encoded_msg_len = (U32)pe_GetMsgLen(pctxt);
                p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len);
                if (PNULL != p_event_header)
                {
                    memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                                encoded_msg_len));
                    p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + 
                            encoded_msg_len);
                    p_event_header->event_id = LOCAL_RRC_MASTER_INFORMATION_BLOCK;
                    p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
                    p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;


                    l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID, p_cell_ctx->cfg_cell_param.
                            sib_type_1_Info.cell_access_related_info.cell_Id, sizeof(U32));

                    p_event_header->EVENT_PARAM_EUTRANCELL_FROID = p_event_header->
                        EVENT_PARAM_CELL_ID;

                    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                                sizeof(EVENT_EXTERNAL_HEADER)),
                            (const void*)p_dst_mib->mib_msg_buf,
                            encoded_msg_len);

                    rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
                }
            }
            
            p_dst_mib->mib_msg_buf_counter = (rrc_counter_t)pe_GetMsgLen(pctxt);
            RRC_ASSERT(p_dst_mib->mib_msg_buf_counter <= sizeof(p_dst_mib->
                mib_msg_buf));

            RRC_CSC_TRACE(RRC_DETAILED, "MIB #%u encoded, ASN.1 size: %u, %s",
                mib_sfn, p_dst_mib->mib_msg_buf_counter, change_data_to_str(p_dst_mib->mib_msg_buf_counter, p_dst_mib->mib_msg_buf));
        }
    }
    /* eMTC changes stop */

    RRC_CSC_UT_TRACE_EXIT();
    return ret_val;
}

/******************************************************************************
*   FUNCTION NAME: encode_mib_recfg
*   INPUT        : OSCTXT                          *pctxt
*                  rrc_csc_llim_reconfig_cell_req_t  *p_llim_recfg_cell_req
*                  mib_info_t                      *p_mib_info
*                  csc_cell_ctx_t                  *p_cell_ctx
*   DESCRIPTION  : This function encodes MIBs for all possible values sfn where MIB is
*                  scheduled.
*
*   RETURNS      : RRC_SUCCESS - if successfully encodes all MIBs.
*                  RRC_FAILURE - otherwise
*
******************************************************************************/
static rrc_return_et encode_mib_recfg
(
    /* for memory allocation */
    OSCTXT                          *pctxt,
    rrc_csc_llim_reconfig_cell_req_t  *p_llim_recfg_cell_req,   /* destination */
    mib_info_t                      *p_mib_info,             /* source */
    csc_cell_ctx_t                  *p_cell_ctx
)
{
    rrc_return_et ret_val               = RRC_SUCCESS;
    rrc_mac_mib_msg_req_t *p_dst_mib    = PNULL;
    int enc_res                         = RRC_NULL;
    U8 mib_sfn                          = RRC_NULL;
    /* i_mib should be able to hold RRC_MIB_MSGS */
    U16 i_mib                           = RRC_NULL;
    BCCH_BCH_Message asn1_mib_msg;
    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
    U32                     encoded_msg_len = 0;
    LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT *p_event_rrc_protocol_err = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_llim_recfg_cell_req);
    RRC_ASSERT(PNULL != p_mib_info);
    RRC_ASSERT(PNULL != p_cell_ctx);

    /* In this case we always have the same number of MIBs during
     *  SFN_UPPER_LIMIT and in the same places */
    RRC_ASSERT(0 == (SFN_UPPER_LIMIT % MIB_PERIODICITY));
    /* Output message should be able to carry all MIBs */
    RRC_ASSERT(RRC_MIB_MSGS == (SFN_UPPER_LIMIT / MIB_PERIODICITY));
    RRC_ASSERT(RRC_MIB_MSGS == ARRSIZE(p_llim_recfg_cell_req->mib_msg_req));

    p_llim_recfg_cell_req->mib_msg_req_counter = RRC_MIB_MSGS;
    
    /* Fill MIB with values from p_mib_info */
    memset_wrapper(&asn1_mib_msg, 0, sizeof(asn1_mib_msg));

    asn1_mib_msg.message.dl_Bandwidth                   = p_mib_info->
        dl_band_width;
    asn1_mib_msg.message.phich_Config.phich_Duration    = p_mib_info->
        phich_config.phich_duration;
    asn1_mib_msg.message.phich_Config.phich_Resource    = p_mib_info->
        phich_config.phich_resource;

    asn1_mib_msg.message.systemFrameNumber.numbits      = 8;
    asn1_mib_msg.message.systemFrameNumber.data[0]      = RRC_NULL;

    asn1_mib_msg.message.schedulingInfoSIB1_BR_r13  = 0;
    asn1_mib_msg.message.spare.numbits                  = 4;
    asn1_mib_msg.message.spare.data[0]                  = 0;

    for (i_mib = 0; i_mib < RRC_MIB_MSGS; i_mib++)
    {
        /* MIB is scheduled in subframe #0 of radio frames for which
         *  the SFN mod 4 = 0 */
        /* 0, MIB_PERIODICITY, 2 * MIB_PERIODICITY, etc. */
        p_dst_mib = p_llim_recfg_cell_req->mib_msg_req + i_mib;

        p_dst_mib->sfn  = (rrc_sfn_t)(i_mib * MIB_PERIODICITY);

        /* In MIB we store 8 (from 10) most significant bits of the SFN */
        mib_sfn = (U8)(p_dst_mib->sfn >> SFN_2_MIB_SFN_SHIFT);

        /* Fill MIB SFN */
        asn1_mib_msg.message.systemFrameNumber.data[0] = mib_sfn;
        /* Encode MIB */
        pu_setBuffer(pctxt, p_dst_mib->mib_msg_buf, sizeof(p_dst_mib->
            mib_msg_buf), ASN_RRC_BUF_TYPE);

        enc_res = asn1PE_BCCH_BCH_Message(pctxt, &asn1_mib_msg);

        if (RT_OK != enc_res)
        {
            RRC_CSC_TRACE(RRC_WARNING, "Can't encode MIB #%u", mib_sfn);

            ret_val= RRC_FAILURE;
	        
            /*Generating Protocol Event LOCAL_EVENT_RRC_PROTOCOL_ERROR*/
            p_event_rrc_protocol_err = rrc_mem_get(sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));
            if (PNULL != p_event_rrc_protocol_err)
            {
                memset_wrapper(p_event_rrc_protocol_err, RRC_NULL, 
                    sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));

                p_event_rrc_protocol_err->header.length = 
                    sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT);
                p_event_rrc_protocol_err->header.event_id = LOCAL_EVENT_RRC_PROTOCOL_ERROR;

                l3_strncpy_wrapper(((S8 *)(p_event_rrc_protocol_err->EVENT_PARAM_RRC_MESSAGE_DISCARDED)),
                    (const S8 *)"MASTER INFORMATION BLOCK", RRC_MESSAGE_DISCARDED_SIZE);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_rrc_protocol_err);
            }
            break;
        }
        else
        {
            /* Generating Protocol Event LOCAL_RRC_MASTER_INFORMATION_BLOCK */    
            encoded_msg_len = (U32)pe_GetMsgLen(pctxt);

            rrc_asn1PrtToStr_BCCH_BCH_Message(RRC_ASN, "BCCH_BCH_Message", 
                    &asn1_mib_msg, "MasterInformationBlock", p_dst_mib->mib_msg_buf,
                    encoded_msg_len);

            p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                    encoded_msg_len);
            if (PNULL != p_event_header)
            {
                memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                            encoded_msg_len));
                p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + 
                        encoded_msg_len);
                p_event_header->event_id = LOCAL_RRC_MASTER_INFORMATION_BLOCK;
                p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
                p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;

                if (PNULL != p_cell_ctx)
                {
                    l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID, p_cell_ctx->cfg_cell_param.
                            sib_type_1_Info.cell_access_related_info.cell_Id, sizeof(U32));
                }

                p_event_header->EVENT_PARAM_EUTRANCELL_FROID = p_event_header->
                    EVENT_PARAM_CELL_ID;

                l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                            sizeof(EVENT_EXTERNAL_HEADER)),
                        (const void*)p_dst_mib->mib_msg_buf,
                        encoded_msg_len);

                rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
            }
            /* Setting Bitmask */
            p_llim_recfg_cell_req->bitmask |= RRC_CSC_MIB_MSG_REQ_PRESENT;
            
            p_dst_mib->mib_msg_buf_counter = (rrc_counter_t)pe_GetMsgLen(pctxt);
            RRC_ASSERT(p_dst_mib->mib_msg_buf_counter <= sizeof(p_dst_mib->
                mib_msg_buf));

            RRC_CSC_TRACE(RRC_DETAILED, "MIB #%u encoded, ASN.1 size: %u, %s",
                mib_sfn, p_dst_mib->mib_msg_buf_counter, change_data_to_str(p_dst_mib->mib_msg_buf_counter, p_dst_mib->mib_msg_buf));
        }
    }

    RRC_CSC_UT_TRACE_EXIT();
    return ret_val;
}
/******************************************************************************
*   FUNCTION NAME: encode_sib1
*   INPUT        : OSCTXT                          *pctxt
*                  rrc_mac_sibtype1_msg_req_t      *p_sib1_msg_req
*                  SystemInformationBlockType1     *p_asn1_sib1
*                  csc_cell_ctx_t          *p_cell_ctx
*   OUTPUT      :  none
*   DESCRIPTION : This function encodes SIB1.
*
*   RETURNS     : RRC_SUCCESS - if successfully encode SIB1.
*                 RRC_FAILURE - otherwise
*
******************************************************************************/
static rrc_return_et encode_sib1
(
    /* for memory allocation */
    OSCTXT                          *pctxt,
    rrc_mac_sibtype1_msg_req_t      *p_sib1_msg_req,    /* destination */
    SystemInformationBlockType1     *p_asn1_sib1,        /* source */
    csc_cell_ctx_t          *p_cell_ctx     /* csc cell context */
)
{
    rrc_return_et ret_val   = RRC_FAILURE;
    S32 enc_res             = RRC_NULL;
    BCCH_DL_SCH_Message asn1_sib1_msg;
    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
    U32                     encoded_msg_len = 0;
    LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT *p_event_rrc_protocol_err = PNULL;

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_sib1_msg_req);
    RRC_ASSERT(PNULL != p_asn1_sib1);

    /* Fill SI Mapping indication */
    if (CELL_M_STATE_IDLE == p_cell_ctx->current_state)
    {
        /* This is required initially at cell setup */
        p_sib1_msg_req->si_mapping_changed = SI_MAPPING_CHANGED;
    }
    else
    {
        p_sib1_msg_req->si_mapping_changed = p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag;
    }

    /* SIB1 is scheduled in subframe #5 of radio frames
     *  for which the SFN mod 8 = 0 */
    p_sib1_msg_req->sfn                         = 0;

    /* Fill ASN.1 SIB1 message */
    memset_wrapper(&asn1_sib1_msg, 0, sizeof(asn1_sib1_msg));

    asn1_sib1_msg.message.t     = T_BCCH_DL_SCH_MessageType_c1;
    asn1_sib1_msg.message.u.c1  = rtxMemAllocType(pctxt,
        BCCH_DL_SCH_MessageType_c1);

    if (PNULL == asn1_sib1_msg.message.u.c1)
    {
        /* Not enough memory */
        return RRC_FAILURE;
    }

    asn1_sib1_msg.message.u.c1->t =
        T_BCCH_DL_SCH_MessageType_c1_systemInformationBlockType1;
    asn1_sib1_msg.message.u.c1->u.systemInformationBlockType1 = p_asn1_sib1;

    /* Encode SIB1 */
    pu_setBuffer(pctxt, p_sib1_msg_req->sibtype1_msg_buf, sizeof(p_sib1_msg_req
        ->sibtype1_msg_buf), ASN_RRC_BUF_TYPE);

    enc_res = asn1PE_BCCH_DL_SCH_Message(pctxt, &asn1_sib1_msg);
    
    if (RT_OK != enc_res)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Can't encode SIB1");

        ret_val = RRC_FAILURE;
        /*Generating Protocol Event LOCAL_EVENT_RRC_PROTOCOL_ERROR*/
        p_event_rrc_protocol_err = rrc_mem_get(sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));
        if (PNULL != p_event_rrc_protocol_err)
        {
            memset_wrapper(p_event_rrc_protocol_err, RRC_NULL, 
                sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));

            p_event_rrc_protocol_err->header.length = 
                sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT);
            p_event_rrc_protocol_err->header.event_id = LOCAL_EVENT_RRC_PROTOCOL_ERROR;

            l3_strncpy_wrapper((S8 *)((p_event_rrc_protocol_err->EVENT_PARAM_RRC_MESSAGE_DISCARDED)),
                (const S8 *)"RRC SYSTEM INFORMATION BLOCK TYPE 1", RRC_MESSAGE_DISCARDED_SIZE);

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_rrc_protocol_err);
        }
    }
    else
    {
	    /* Generating Protocol Event LOCAL_RRC_SYSTEM_INFORMATION_BLOCK_TYPE_1 */    
        encoded_msg_len = (U32)pe_GetMsgLen(pctxt);

        rrc_asn1PrtToStr_BCCH_DL_SCH_Message (RRC_ASN, "BCCH_DL_SCH_Message",
                &asn1_sib1_msg, "SystemInformationBlockType1", p_sib1_msg_req->sibtype1_msg_buf,
                encoded_msg_len);

        p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                encoded_msg_len);
        if (PNULL != p_event_header)
        {
            memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                      encoded_msg_len));
	    p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + 
	  	      encoded_msg_len);
	    p_event_header->event_id = LOCAL_RRC_SYSTEM_INFORMATION_BLOCK_TYPE_1;
	    p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
	    p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;
		
		    
	    l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID, p_cell_ctx->cfg_cell_param.
         	      sib_type_1_Info.cell_access_related_info.cell_Id, sizeof(U32));
    
	    p_event_header->EVENT_PARAM_EUTRANCELL_FROID = p_event_header->
          	      EVENT_PARAM_CELL_ID;
                    
	    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                      sizeof(EVENT_EXTERNAL_HEADER)),
                      (const void*)p_sib1_msg_req->sibtype1_msg_buf,
                      encoded_msg_len);

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
        }
        p_sib1_msg_req->sibtype1_msg_buf_counter = (rrc_counter_t)pe_GetMsgLen
            (pctxt);
        RRC_ASSERT(p_sib1_msg_req->sibtype1_msg_buf_counter <= sizeof
            (p_sib1_msg_req->sibtype1_msg_buf));

        RRC_CSC_TRACE(RRC_INFO, "SIB1 encoded, ASN.1 size: %u, %s",
            p_sib1_msg_req->sibtype1_msg_buf_counter, change_data_to_str(p_sib1_msg_req->sibtype1_msg_buf_counter, p_sib1_msg_req->sibtype1_msg_buf));

        ret_val = RRC_SUCCESS;
    }

    /*CR 491 Changes */
    if( RRC_FAILURE == check_sib_size(p_cell_ctx,p_sib1_msg_req->sibtype1_msg_buf_counter))
    {
        RRC_CSC_TRACE(RRC_INFO, "SIB1 encoded, ASN.1 size  exceeding Limits"); 
        ret_val = RRC_FAILURE;
    }
    /*CR 491 Changes */
    RRC_CSC_UT_TRACE_EXIT();
    return ret_val;
}


/******************************************************************************
 *   FUNCTION NAME: update_sib8_sfn_list
 *   INPUT        : U16 start_sib8_sfn
 *                  U16 periodicity
 *   OUTPUT       : none
 *   DESCRIPTION  : to update the SIB8 in sib8_sfn_list data structure
 *
 *   RETURNS      : None
 *
 * ******************************************************************************/
static void update_sib8_sfn_list(U16 start_sib8_sfn, U16 periodicity)
{
    U16 ctr = 0;
    U16 temp_sfn = 0;

    RRC_CSC_UT_TRACE_ENTER();

    /*fill element at 0th index to avoid the break condition of
     *      * for loop*/
    sib8_sfn_list.sib8_sfn[0] = start_sib8_sfn;
    sib8_sfn_list.num_of_elements = 1;

    for(ctr=1; ctr < MAX_SIB_INSTANCES; ctr++)
    {
        temp_sfn = ((start_sib8_sfn + (periodicity*ctr)%SFN_UPPER_LIMIT)%SFN_UPPER_LIMIT);
        if(temp_sfn == start_sib8_sfn)
            break;
        sib8_sfn_list.sib8_sfn[ctr] = temp_sfn;
        sib8_sfn_list.num_of_elements++;
    }
    for(ctr=0; ctr<sib8_sfn_list.num_of_elements;ctr++)
    {
        RRC_CSC_TRACE(RRC_INFO,"ctr=%d sfn=%d", ctr,sib8_sfn_list.sib8_sfn[ctr]);
    }
    RRC_CSC_TRACE(RRC_INFO,"Num of sib8_sfn = %d", sib8_sfn_list.num_of_elements);
    RRC_CSC_UT_TRACE_EXIT();
}

/******************************************************************************
 *   FUNCTION NAME: encode_si_proc_si_trans_info_list
 *
 *   DESCRIPTION:
 *       This function encode SI.
 *
 *   RETURNS:
 *       Void
 *
 ******************************************************************************/
static void encode_si_proc_si_trans_info_list
(
 /*si_transmission_info*/
 rrc_si_transmission_info_list_t       *p_si_transmission_info_list,
 /* order of SI in schedulingInformation. 1 based as in TS */
 U8                                  	si_n,
 rrc_si_msg_info_t               		*p_si_msg_info,
 /* si_window_length in sf == ms */
 U8                                  	si_window_length_sf,
 rrc_return_et       					*p_ret_val
)
{
  U8      index = RRC_INVALID_SI_INDEX;
  U8      si_trans_count = RRC_NULL;
  U8      offset_num = RRC_NULL;
  U8      count = RRC_NULL;
  U8      offset_zero_loop = RRC_NULL;
  rrc_bool_et  si_index_matched = RRC_FALSE;
  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != p_si_msg_info);

  if (PNULL != p_si_transmission_info_list)
  {
    for (si_trans_count = 0 ; si_trans_count <
	p_si_transmission_info_list->count ; si_trans_count++)
    {
      index = p_si_transmission_info_list->si_transmission[
	si_trans_count].si_index;

      /*Validate 1: match the si_index*/
      if (index == (si_n - 1))
      {
	RRC_CSC_TRACE(RRC_INFO, 
	    "si_index is matching with "
	    "scheduling info list count pressent in sib1");
	si_index_matched = RRC_TRUE;
	break;
      }
    }
    if (RRC_TRUE == si_index_matched)
    {
      if (!(RRC_RRM_NUM_OF_TRANSMISSIONS_PRESENT &
	    p_si_transmission_info_list->si_transmission[si_trans_count].bitmask) 
	  &&
	  !(RRC_RRM_SI_START_OFFSET_INFO_PRESENT &
	    p_si_transmission_info_list->si_transmission[si_trans_count].bitmask))
      {
	RRC_CSC_TRACE(RRC_INFO, 
	    "SI Index is present but neither SI_TRANSMISSION and nor "
	    "SI_START_OFFSET is present: ignore it");
      }

      /*Fill num_of_transmission*/
      if (RRC_RRM_NUM_OF_TRANSMISSIONS_PRESENT &
	  p_si_transmission_info_list->si_transmission[si_trans_count].bitmask)
      {
	p_si_msg_info->num_of_transmission =
	  p_si_transmission_info_list-> 
	  si_transmission[si_trans_count].num_of_transmissions;
      }

      if (RRC_RRM_SI_START_OFFSET_INFO_PRESENT &
	  p_si_transmission_info_list->si_transmission[si_trans_count].bitmask)
      {
	p_si_msg_info->bitmask |= 
	  RRC_CSC_LLIM_SI_START_OFFSET_INFO_PRESENT;

	p_si_msg_info->si_start_offset_counter =
	  p_si_transmission_info_list->
	  si_transmission[si_trans_count].si_start_offset_info.
	  offset_count;

	if (RRC_RRM_NUM_OF_TRANSMISSIONS_PRESENT &
	    p_si_transmission_info_list->si_transmission[si_trans_count].bitmask)
	{
	  if (p_si_msg_info->si_start_offset_counter !=
	      p_si_msg_info->num_of_transmission)
	  {
	    RRC_CSC_TRACE(RRC_WARNING, 
		"Mapping of numOfTransmission and count of  "
		"si_start_offset is not correct");
	    *p_ret_val = RRC_FAILURE;
	  }
	}
	else if (p_si_msg_info->si_start_offset_counter > 1) 
	{
	  RRC_CSC_TRACE(RRC_WARNING, 
	      "value of si_start_offset should not be greater than one, " 
	      "if num_of_transmission is not present");
	  *p_ret_val = RRC_FAILURE;
	}

	if (RRC_FAILURE != *p_ret_val)
	{
	  for (offset_num = 0 ; offset_num < 
	      p_si_transmission_info_list->si_transmission[si_trans_count].
	      si_start_offset_info.offset_count ; offset_num ++)
	  {
	    if (RRC_FAILURE == *p_ret_val)
	    {
	      break;
	    }
	    /*Validate 2: offset value should be in
	     * incrememnt order*/
	    for (count = offset_num + 1 ; count < 
		p_si_transmission_info_list-> 
		si_transmission[si_trans_count].
		si_start_offset_info.offset_count ; count ++)
	    {
	      if (p_si_transmission_info_list->
		  si_transmission[si_trans_count].si_start_offset_info.
		  si_start_offset[offset_num].si_start_offset >
		  p_si_transmission_info_list->
		  si_transmission[si_trans_count].si_start_offset_info.
		  si_start_offset[count].si_start_offset)
	      {
		RRC_CSC_TRACE(RRC_WARNING, 
		    "Received Offset value is not in increment order : "
		    "wrong value");
		*p_ret_val = RRC_FAILURE;
		break;
	      }

	    }
	    /*Validate 3: offset value should not be gretaer 
	     *than si_window_length*/ 
	    if ((RRC_FAILURE != *p_ret_val) &&
		((p_si_transmission_info_list->si_transmission[si_trans_count].
		  si_start_offset_info.offset_count -1) == offset_num) &&
		(p_si_transmission_info_list->
		 si_transmission[si_trans_count].si_start_offset_info.
		 si_start_offset[
		 p_si_transmission_info_list->si_transmission[si_trans_count].
		 si_start_offset_info.offset_count -1].
		 si_start_offset > 
		 si_window_length_sf))
	    {
	      RRC_CSC_TRACE(RRC_WARNING, 
		  "offset value should not be greater than si window length");
	      *p_ret_val = RRC_FAILURE;
	    }
	    if ( RRC_FAILURE != *p_ret_val )
	    {
	      /*Filling SI START OFFSET*/
	      p_si_msg_info-> 
		si_start_offset.si_start_offset[offset_num] =
		p_si_transmission_info_list->
		si_transmission[si_trans_count].si_start_offset_info.
		si_start_offset[offset_num].si_start_offset;
	    }
	  }
	  /*Appending the value zero for rest of the si_start_offset*/
	  if ((RRC_FAILURE != *p_ret_val) && (offset_num < MAX_SI_START_OFFSET))
	  {
	    for (offset_zero_loop = offset_num ; 
		offset_zero_loop < MAX_SI_START_OFFSET ;
		offset_zero_loop++)
	    {
	      p_si_msg_info-> 
		si_start_offset.si_start_offset[offset_zero_loop] = RRC_NULL;
	    }
	  }
	}
      }
    }
  }
  RRC_CSC_UT_TRACE_EXIT();
  return;
}

/******************************************************************************
 *   FUNCTION NAME: encode_new_mapping_si_fill_sib_8_to_sis
 *
 *   DESCRIPTION:
 *       This function fills SIB8 in SI.
 *
 *   RETURNS:
 *       Pointer to OSCTXT
 *
 ******************************************************************************/
static OSCTXT*  encode_new_mapping_si_fill_sib_8_to_sis
(
 /* for memory allocation */
 OSCTXT                              		*pctxt,
 /* Cell Context */
 csc_cell_ctx_t                       		*p_cell_ctx,
 /* destination */
 rrc_si_msg_info_t               		*p_si_msg_info,
 /* si-Periodicity and SIBs */
 new_scheduling_info_t                    	*p_scheduling_info,
 SystemInformation_r8_IEs_sib_TypeAndInfo_element *p_asn1_sib_elem,
 asn1_sibs_t                         		*p_asn1_sibs,
 U8 						*p_si_segment_count,
 U8 						*p_si_segment_counter,
 rrc_return_et					*p_ret_val
)
{
  OSCTXT                              		*temp_pctxt=PNULL;
  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != pctxt);
  RRC_ASSERT(PNULL != p_si_msg_info);
  RRC_ASSERT(PNULL != p_asn1_sib_elem);
  RRC_ASSERT(PNULL != p_cell_ctx);
  U16     periodicity_in_sfn = RRC_NULL;

  if(1 == *p_si_segment_count) /*do the following operations only once*/
  {
    /*Update the SIB8 SFN List */
    periodicity_in_sfn = fill_si_periodicity_in_sfn(p_scheduling_info->si_periodicity);
    update_sib8_sfn_list(p_cell_ctx->start_sib8_sfn, periodicity_in_sfn);

    p_cell_ctx->start_sib8_sfn = p_si_msg_info->starting_sfn;
    p_cell_ctx->start_sib8_sf = p_si_msg_info->starting_sf;
    p_cell_ctx->sib8_periodicity = periodicity_in_sfn;
  }
  if(p_cell_ctx->sib_modify_source == RRM_TRIGGERED_SIB8_NO_MODIFY)
  {
    p_asn1_sib_elem->u.sib8 = &p_asn1_sibs->asn1_sib8;
    p_si_msg_info->rrc_mac_sched_reqd_for_sib8 = SIB8_NO_CHANGE;
  }          
  else
  {
    if(1 == *p_si_segment_count) /*do the following operations only once*/
    {
      if(p_cell_ctx->sib_modify_source == RRM_TRIGGERED_SIB8_MODIFY)
      {
	/*discard existing SIB8 buffer and apply the new SIB8 buffer starting from the SFN/SF specified*/
	p_si_msg_info->rrc_mac_sched_reqd_for_sib8 = SIB8_FLUSH_OLD_BUFFER_APPLY_NEW_BUFFER;
	*p_si_segment_count = MAX_SIB8_INSTANCES;

	/* In case of RRM_TRIGGERED_SIB8_MODIFY, no need to request SFN, SF & Timestamp from 
	 * L2. Rather use the last updated SFN, SF & Timestamp updated in p_cell_ctx
	 * for caliberating CDMA system time. Calling the following function will update the cdma_base_time
	 * in p_cell_ctx with the one which needs to be broadcasted in next SIB8 instance after keeping a safe
	 * buffer so that L2 can process the same */
	{
	  rrc_csc_llim_config_cell_resp_t config_cell_resp;
	  memset_wrapper(&config_cell_resp, 0, sizeof(rrc_csc_llim_config_cell_resp_t));
	  config_cell_resp.bitmask |= RRC_CSC_LLIM_SFN_INFO_PRESENT 
	    | RRC_CSC_LLIM_SF_INFO_PRESENT 
	    | RRC_CSC_LLIM_TIMESTAMP_INFO_PRESENT;
	  config_cell_resp.sfn = p_cell_ctx->sfn;
	  config_cell_resp.sf = p_cell_ctx->sf;
	  config_cell_resp.timestamp = p_cell_ctx->timestamp;

	  if(RRC_SUCCESS != resume_high_priority_thread(p_cell_ctx, 
		config_cell_resp.sfn, 
		config_cell_resp.sf,
		config_cell_resp.timestamp))
	  {
	    RRC_CSC_TRACE(RRC_INFO, \
		"Couldn't generate the CDMA Base Time in %s",__FUNCTION__);
	  }
	  else
	  {
	    RRC_CSC_TRACE(RRC_INFO, \
		"CDMA Base Time generated successfully in %s",__FUNCTION__);
	  }
	}
      }
      else if(p_cell_ctx->sib_modify_source == CSC_TRIGGERED_SIB8_MODIFY)
      {
	/*SI_MSG to contain the set of new MAX_SIB8_INSTANCES SIB8 segments*/
	p_si_msg_info->rrc_mac_sched_reqd_for_sib8 = SIB8_NEW_BUFFER;
	*p_si_segment_count = MAX_SIB8_INSTANCES;
      }				  
    }

    temp_pctxt = pctxt;
    p_asn1_sib_elem->u.sib8 =
      rtxMemAllocType(temp_pctxt, SystemInformationBlockType8);
    /* SPR 12964 Fix Start */
    if (PNULL == p_asn1_sib_elem->u.sib8)
    {
        /* Not enough memory */
        *p_ret_val = RRC_FAILURE;
        RRC_CSC_UT_TRACE_EXIT();
        return temp_pctxt;
    }
    /* SPR 12964 Fix Stop */

    if(RRC_FAILURE == bch_m_check_and_convert_sib8(temp_pctxt,
	  p_asn1_sib_elem->u.sib8,
	  &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_8_Info,
	  p_cell_ctx, *p_si_segment_counter))
    {
      RRC_CSC_TRACE(RRC_WARNING, "Can't encode SIB8");
	*p_ret_val = RRC_FAILURE;
      return temp_pctxt;
    }
  }
  RRC_CSC_UT_TRACE_EXIT();
  return temp_pctxt;
}

/******************************************************************************
 *   FUNCTION NAME: encode_new_mapping_si_fill_sibs_to_si
 *
 *   DESCRIPTION:
 *       This function fills SIBs in SIs.
 *
 *   RETURNS:
 *       Pointer to OSCTXT
 *
 ******************************************************************************/
static OSCTXT* encode_new_mapping_si_fill_sibs_to_si
(
 /* for memory allocation */
 OSCTXT                              		*pctxt,
 /* Cell Context */
 csc_cell_ctx_t                       		*p_cell_ctx,
 /* destination */
 rrc_si_msg_info_t               		*p_si_msg_info,
 /* source, all SIBs in ASN.1 format */
 asn1_sibs_t                         		*p_asn1_sibs,
 /* si-Periodicity and SIBs */
 new_scheduling_info_t                    	*p_scheduling_info,
 SystemInformation_r8_IEs_sib_TypeAndInfo_element *p_asn1_sib_elem,
 /* SIB11 information */
 sib_type_11_t                        		*p_sib_type_11,
 /*SIB12 information*/  
 cmas_info_t                          		*p_cmas_info_node,
 U8 						*p_si_segment_count,
 U8 						*p_si_segment_counter,
 U8 						sib_mapping_index,
 rrc_return_et					*p_ret_val
)
{
  OSCTXT                              		*temp_pctxt = PNULL;
  U16 warning_msg_segment_size = RRC_NULL;
  U32 warning_msg_segment_type = RRC_NULL;
  /* Bug_15359 fix starts */
  U16 segment_size = RRC_NULL;
  /* Bug_15359 fix ends */
  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != pctxt);
  RRC_ASSERT(PNULL != p_si_msg_info);
  RRC_ASSERT(PNULL != p_asn1_sibs);
  RRC_ASSERT(PNULL != p_scheduling_info);
  RRC_ASSERT(PNULL != p_cell_ctx);

  switch (p_scheduling_info->sib_mapping_info.sib_type[sib_mapping_index])
  {
    case RRM_RRC_SIB_TYPE_3:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib3;
      p_asn1_sib_elem->u.sib3 = &p_asn1_sibs->asn1_sib3;

      break;
    case RRM_RRC_SIB_TYPE_4:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib4;
      p_asn1_sib_elem->u.sib4 = &p_asn1_sibs->asn1_sib4;

      break;
    case RRM_RRC_SIB_TYPE_5:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib5;
      p_asn1_sib_elem->u.sib5 = &p_asn1_sibs->asn1_sib5;

      break;
    case RRM_RRC_SIB_TYPE_6:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib6;
      p_asn1_sib_elem->u.sib6 = &p_asn1_sibs->asn1_sib6;

      break;
    case RRM_RRC_SIB_TYPE_7:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib7;
      p_asn1_sib_elem->u.sib7 = &p_asn1_sibs->asn1_sib7;

      break;
    case RRM_RRC_SIB_TYPE_8:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib8;
      temp_pctxt = encode_new_mapping_si_fill_sib_8_to_sis(pctxt, p_cell_ctx, p_si_msg_info, p_scheduling_info,
	  p_asn1_sib_elem, p_asn1_sibs, p_si_segment_count, p_si_segment_counter, p_ret_val);
      if (RRC_FAILURE == *p_ret_val)
	return temp_pctxt;
      break;
    case RRM_RRC_SIB_TYPE_9:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib9;
      p_asn1_sib_elem->u.sib9 = &p_asn1_sibs->asn1_sib9;

      break;
    case RRM_RRC_SIB_TYPE_10:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib10;
      p_asn1_sib_elem->u.sib10 = &p_asn1_sibs->asn1_sib10;
      if (PNULL != p_cell_ctx->etws_info)
      {
	fill_si_periodicity (&p_cell_ctx->etws_info->sib10_si_periodicity,
	    &p_scheduling_info->si_periodicity);
      }

      break;
    case RRM_RRC_SIB_TYPE_11:
      if (PNULL != p_sib_type_11)
      {
	temp_pctxt = pctxt;
	p_asn1_sib_elem->t =
	  T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib11;
	if (*p_si_segment_counter < p_sib_type_11->num_of_segments)
	{
	  warning_msg_segment_type = notLastSegment;
	  warning_msg_segment_size = (U16)((p_sib_type_11->sib_type_11_Info.warning_msg_size)/
	      (p_sib_type_11->num_of_segments));
	  if (RRC_NULL != ((p_sib_type_11->sib_type_11_Info.warning_msg_size)/
		(p_sib_type_11->num_of_segments)))
	  {
	    warning_msg_segment_size++;
	  }
           /* Bug_15359 fix starts */
	   segment_size = warning_msg_segment_size; 
           /* Bug_15359 fix ends */
	}
	else
	{
	  warning_msg_segment_type = lastSegment;
	  warning_msg_segment_size = (U16)((p_sib_type_11->sib_type_11_Info.warning_msg_size)/
	      (p_sib_type_11->num_of_segments));
	  if (RRC_NULL != ((p_sib_type_11->sib_type_11_Info.warning_msg_size)/
		(p_sib_type_11->num_of_segments)))
	  {
	    warning_msg_segment_size++;
	  }
          /* Bug_15359 fix starts */
	   segment_size = warning_msg_segment_size; 
          /* Bug_15359 fix ends */
	  warning_msg_segment_size =  (U16)(p_sib_type_11->sib_type_11_Info.warning_msg_size -
	      (warning_msg_segment_size * (p_sib_type_11->num_of_segments - 1)));
	}
	p_asn1_sib_elem->u.sib11 =
	  rtxMemAllocType(temp_pctxt, SystemInformationBlockType11);
    /* SPR 12964 Fix Start */
    if (PNULL == p_asn1_sib_elem->u.sib11)
    {
        /* Not enough memory */
        *p_ret_val = RRC_FAILURE;
        RRC_CSC_UT_TRACE_EXIT();
        return temp_pctxt;
    }
    /* SPR 12964 Fix Stop */
	if (RRC_FAILURE == bch_m_check_and_convert_sib11(
	      temp_pctxt,
	      p_asn1_sib_elem->u.sib11,
	      &p_sib_type_11->sib_type_11_Info,
	      *p_si_segment_counter,
	      warning_msg_segment_size,
              /* Bug_15359 fix starts */
	      warning_msg_segment_type, segment_size))
              /* Bug_15359 fix ends */
	{
	  RRC_CSC_TRACE(RRC_WARNING, "Can't encode SIB11");
	  *p_ret_val = RRC_FAILURE;
	  return temp_pctxt;
	}
	*p_si_segment_count = p_sib_type_11->num_of_segments;
	if (PNULL != p_cell_ctx->etws_info)
	{
	  fill_si_periodicity (&p_cell_ctx->etws_info->sib11_si_periodicity,
	      &p_scheduling_info->si_periodicity);
	}
      }
      break;
    case RRM_RRC_SIB_TYPE_12:
      if (PNULL != p_cmas_info_node)
      {
	if (PNULL == temp_pctxt)
	{
	  temp_pctxt = pctxt;
	}
	p_asn1_sib_elem->t =
	  T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib12_v920;
	if (*p_si_segment_counter < p_cmas_info_node->num_of_segments)
	{
	  warning_msg_segment_type = notLastSegment;
	  warning_msg_segment_size = (U16)((p_cmas_info_node->sib12_info.warning_msg_size)/
	      (p_cmas_info_node->num_of_segments));
	  if (RRC_NULL != ((p_cmas_info_node->sib12_info.warning_msg_size)/
		(p_cmas_info_node->num_of_segments)))
	  {
	    warning_msg_segment_size++;
	  }
	}
	else
	{
	  warning_msg_segment_type = lastSegment;
	  warning_msg_segment_size = (U16)((p_cmas_info_node->sib12_info.warning_msg_size)/
	      (p_cmas_info_node->num_of_segments));
	  if (RRC_NULL != ((p_cmas_info_node->sib12_info.warning_msg_size)/
		(p_cmas_info_node->num_of_segments)))
	  {
	    warning_msg_segment_size++;
	  }
	  warning_msg_segment_size = (U16)(p_cmas_info_node->sib12_info.warning_msg_size -
	      (warning_msg_segment_size * (p_cmas_info_node->num_of_segments - 1)));
	}
	p_asn1_sib_elem->u.sib12_v920 =
	  rtxMemAllocType(temp_pctxt, SystemInformationBlockType12_r9);
    /* SPR 12964 Fix Start */
    if (PNULL == p_asn1_sib_elem->u.sib12_v920)
    {
        /* Not enough memory */
        *p_ret_val = RRC_FAILURE;
        RRC_CSC_UT_TRACE_EXIT();
        return temp_pctxt;
    }
    /* SPR 12964 Fix Stop */
	if (RRC_FAILURE == bch_m_check_and_convert_sib12(
	      temp_pctxt,
	      p_asn1_sib_elem->u.sib12_v920,
	      &p_cmas_info_node->sib12_info,
	      *p_si_segment_counter,
	      warning_msg_segment_size,
	      warning_msg_segment_type))
	{
	  RRC_CSC_TRACE(RRC_WARNING, "Can't encode SIB12");
	  *p_ret_val = RRC_FAILURE;
	  return temp_pctxt;
	}
	*p_si_segment_count = p_cmas_info_node->num_of_segments;
	p_si_msg_info->num_of_repetitions = p_cmas_info_node->
	  num_of_repetitions;
	fill_si_periodicity (&p_cmas_info_node->sib12_si_periodicity,
	    &p_scheduling_info->si_periodicity);
      }
      break;
#ifdef LTE_EMBMS_SUPPORTED
    case RRM_RRC_SIB_TYPE_13:
      {
          p_asn1_sib_elem->t =
              T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib13_v920;
          p_asn1_sib_elem->u.sib13_v920 = &p_asn1_sibs->asn1_sib13;
          break;
      }
#endif
    default:
      RRC_ASSERT(0);
      break;
  }
  RRC_CSC_UT_TRACE_EXIT();
  return temp_pctxt;
}

/******************************************************************************
 *   FUNCTION NAME: encode_si_with_new_mapping
 *   INPUT        : OSCTXT                              *pctxt 
 *                  csc_cell_ctx_t                       *p_cell_ctx
 *                  rrc_si_msg_info_t               *p_si_msg_info
 *                  U8                                  *p_si_msg_buf
 *                  U16                                 *p_si_msg_buf_inde
 *                  asn1_sibs_t                         *p_asn1_sibs
 *                  new_scheduling_info_t                    *p_scheduling_info
 *                  U8                                  si_n
 *                  U8                                  si_window_length_sf
 *                  sib_type_11_t                        *p_sib_type_11
 *                  cmas_info_t                          *p_cmas_info_node
 *                  rrc_si_transmission_info_list_t       *p_si_transmission_info_list
 *   OUTPUT       : none
 *   DESCRIPTION  : This function encode SI.
 *
 *   RETURNS      : RRC_SUCCESS - if successfully encode SI.
 *                  RRC_FAILURE - otherwise
 *
 ******************************************************************************/
static rrc_return_et encode_si_with_new_mapping
(
 /* for memory allocation */
 OSCTXT                              *pctxt,
 /* Cell Context */
 csc_cell_ctx_t                       *p_cell_ctx,
 /* destination */
 rrc_si_msg_info_t               *p_si_msg_info,
 /* destination */
 U8                                  *p_si_msg_buf,
 /*SI Msg Buf Index */
 U16                                 *p_si_msg_buf_index,
 /* source, all SIBs in ASN.1 format */
 asn1_sibs_t                         *p_asn1_sibs,
 /* si-Periodicity and SIBs */
 new_scheduling_info_t                    *p_scheduling_info,
 /* order of SI in schedulingInformation. 1 based as in TS */
 U8                                  si_n,
 /* si_window_length in sf == ms */
 U8                                  si_window_length_sf,
 /* SIB11 information */
 sib_type_11_t                        *p_sib_type_11,
 /*SIB12 information*/  
 cmas_info_t                          *p_cmas_info_node,
 /*si_transmission_info*/
 rrc_si_transmission_info_list_t       *p_si_transmission_info_list
#ifdef LTE_EMBMS_SUPPORTED
 ,sib13_scheduling_info_t *sib13_scheduling_info_p
#endif
 )
{
    OSCTXT      *temp_pctxt = PNULL;
    rrc_return_et       ret_val = RRC_SUCCESS;
    BCCH_DL_SCH_Message     asn1_si_msg;
    SystemInformation_r8_IEs    *p_asn1_si_r8_ies = PNULL;
    SystemInformation_r8_IEs_sib_TypeAndInfo_element *p_asn1_sib_elem = PNULL;
    OSRTDListNode *p_asn1_list_node  = PNULL;
    int enc_res = RRC_NULL;
    U8 sib_mapping_index   = RRC_NULL;
    U8 si_segment_counter = 1;
    U8 si_segment_count = 1;
    rrc_bool_et  is_sib11_present = RRC_FALSE;
    rrc_bool_et  is_sib12_present = RRC_FALSE;
    U16 sib_length = RRC_NULL;
    rrc_return_et       ret = RRC_SUCCESS;
    /* SPR 11229 Fix Start */
    U8        sib8_offset = 0;
    /* SPR 11229 Fix Start */

    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
    U32                     encoded_msg_len = 0;
    LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT *p_event_rrc_protocol_err = PNULL;

  U16 x = (U16)((si_n - 1) * si_window_length_sf);

  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != pctxt);
  RRC_ASSERT(PNULL != p_si_msg_info);
  RRC_ASSERT(PNULL != p_asn1_sibs);
  RRC_ASSERT(PNULL != p_scheduling_info);

  /* to store params we have to know a pointer to rrc_mac_si_msg_info_t
   *  inside asn1PE_xxx functions */
  pctxt->pUserData = (OSVoidPtr*)p_si_msg_info;

  /* Fill SI Index as available in SIB1 (subtracting 1, because it was incrementted while
     sending to encode)*/

  /* SI Mapping NOT changed */    
  p_si_msg_info->si_index = p_scheduling_info->si_index;

  /* convert si_periodicity in MAC format according to
   *  LTE_MAC_Rel_1.2_API_Manual_Rev_2.4.doc */

  p_si_msg_info->periodicity  = (U8)(p_scheduling_info->si_periodicity + 1);
  /* See TS 36.331-840, p27 */
  p_si_msg_info->starting_sfn = x / 10;
  p_si_msg_info->starting_sf  =(rrc_sf_t)( x % 10);
  /* If next ASSERT risen - we have BUG in check_sis_scheduling function */
  /* convert si_periodicity to radio frames */
  RRC_ASSERT(p_si_msg_info->starting_sfn < (8 << p_scheduling_info->
        si_periodicity));
  /* In case SIB is of single type, it has to repeat itself only */
  p_si_msg_info->num_of_repetitions = 0;

  p_si_msg_info->rrc_mac_sched_reqd_for_sib8 = NO_SIB8;
  p_si_msg_info->bitmask |= RRC_CSC_LLIM_TIMESTAMP_REQUIRED_PRESENT;

  encode_si_proc_si_trans_info_list(p_si_transmission_info_list, si_n, 
	p_si_msg_info, si_window_length_sf, &ret_val);

   if ( RRC_FAILURE == ret_val )
   {
       RRC_CSC_UT_TRACE_EXIT();
       return ret_val;
   }

  /* Reset si_msg_param_counter */
  p_si_msg_info->si_msg_param_counter = 0;

  for (si_segment_counter = 1; si_segment_counter <= si_segment_count; 
      si_segment_counter++)
  {
    /* Fill SI message */
    memset_wrapper(&asn1_si_msg, 0, sizeof(asn1_si_msg));

    asn1_si_msg.message.t       = T_BCCH_DL_SCH_MessageType_c1;
    asn1_si_msg.message.u.c1    = rtxMemAllocType(pctxt,
      BCCH_DL_SCH_MessageType_c1);

    if (PNULL == asn1_si_msg.message.u.c1)
    {
      /* Not enough memory */
      return RRC_FAILURE;
    }

    asn1_si_msg.message.u.c1->t =
      T_BCCH_DL_SCH_MessageType_c1_systemInformation;
    asn1_si_msg.message.u.c1->u.systemInformation = rtxMemAllocType(pctxt,
      SystemInformation);

    if (PNULL == asn1_si_msg.message.u.c1->u.systemInformation)
    {
      /* Not enough memory */
      return RRC_FAILURE;
    }

    asn1_si_msg.message.u.c1->u.systemInformation->criticalExtensions.t =
      T_SystemInformation_criticalExtensions_systemInformation_r8;
    p_asn1_si_r8_ies = rtxMemAllocType(pctxt, SystemInformation_r8_IEs);

    if (PNULL == p_asn1_si_r8_ies)
    {
      /* Not enough memory */
      return RRC_FAILURE;
    }

    asn1_si_msg.message.u.c1->u.systemInformation->criticalExtensions.u.
       systemInformation_r8 = p_asn1_si_r8_ies;
    p_asn1_si_r8_ies->m.nonCriticalExtensionPresent = 0;

    /* Init sib_TypeAndInfo list */
    rtxDListInit(&p_asn1_si_r8_ies->sib_TypeAndInfo);

    /* Special case in 36.331. If we receive value 1 we should add SIB2 to SI */
    if(1 == p_si_msg_info->si_index)
    {
      /* Allocate node for SystemInformationBlock2 */
      rtxDListAllocNodeAndData (pctxt,
        SystemInformation_r8_IEs_sib_TypeAndInfo_element,
        &p_asn1_list_node, &p_asn1_sib_elem);

      if (PNULL == p_asn1_list_node)
      {
        /* Not enough memory */
        return RRC_FAILURE;
      }

      /* Fill SystemInformationBlockType2 */
      p_asn1_sib_elem->t =
        T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib2;
      p_asn1_sib_elem->u.sib2 = &p_asn1_sibs->asn1_sib2;

      rtxDListAppendNode(&p_asn1_si_r8_ies->sib_TypeAndInfo,
          p_asn1_list_node);
    }

    for (sib_mapping_index = 0; sib_mapping_index < p_scheduling_info->
        sib_mapping_info.count; sib_mapping_index++)
    {
        if ( RRM_RRC_SIB_TYPE_11 == 
              p_scheduling_info->sib_mapping_info.
                        sib_type[sib_mapping_index] )
        {
            is_sib11_present = RRC_TRUE;
        }    
               
        if ( RRM_RRC_SIB_TYPE_12 == 
              p_scheduling_info->sib_mapping_info.
                        sib_type[sib_mapping_index] )
        {
            is_sib12_present = RRC_TRUE;
        }    
#ifdef LTE_EMBMS_SUPPORTED
        if ( RRM_RRC_SIB_TYPE_13 == 
              p_scheduling_info->sib_mapping_info.
                        sib_type[sib_mapping_index] )
        {
            /* SPR 18439 Fix Start */
            sib13_scheduling_info_p->si_index = si_n;
            /* SPR 18439 Fix End */
        }
#endif
               
        /* SPR 11229 Fix Start */
        if ( RRM_RRC_SIB_TYPE_8 == 
              p_scheduling_info->sib_mapping_info.
                        sib_type[sib_mapping_index] )
        {
            /* Incrementing SIB8 offset with SI Window's boundary SFN */
            p_cell_ctx->sib8_si_window_offset = 
                     si_window_length_sf / 10;

            /* Incrementing SIB8 offset with SI window's boundary SF
               and SI's starting SF */
            sib8_offset = (si_window_length_sf % 10) +
                (p_si_msg_info->starting_sf);

            if (sib8_offset > 0)
            {
                p_cell_ctx->sib8_si_window_offset++;

                if (sib8_offset > 10) /* Needs to increment by 2 SFN */
                {
                    p_cell_ctx->sib8_si_window_offset++;
                }
            }
        }    
        /* SPR 11229 Fix Start */
    }
    
    if ( (RRC_TRUE == is_sib11_present) &&
         (RRC_TRUE == is_sib12_present) &&
         (PNULL != p_cmas_info_node) &&
         (PNULL != p_sib_type_11) )
    {    
       /* No. of segments of SIB11 and SIB12 in same SI are not equal */ 
       if ( p_sib_type_11->num_of_segments !=
                p_cmas_info_node->num_of_segments )
       {
           return RRC_FAILURE;
       }    
    }
    
    /* Find SIBs which will be added to SI in
     *  p_scheduling_info->sib_mapping_info */
    for (sib_mapping_index = 0; sib_mapping_index < p_scheduling_info->
        sib_mapping_info.count; sib_mapping_index++)
    {
      /* Allocate node for SystemInformationBlockX */
      rtxDListAllocNodeAndData(pctxt,
          SystemInformation_r8_IEs_sib_TypeAndInfo_element,
          &p_asn1_list_node, &p_asn1_sib_elem);

      if (PNULL == p_asn1_list_node)
      {
        /* Not enough memory */
        return RRC_FAILURE;
      }

      asn1Init_SystemInformation_r8_IEs_sib_TypeAndInfo_element(p_asn1_sib_elem);
      /* Fill SystemInformationBlockTypeX */

      temp_pctxt = encode_new_mapping_si_fill_sibs_to_si(pctxt, p_cell_ctx, p_si_msg_info, p_asn1_sibs, 
	                 p_scheduling_info, p_asn1_sib_elem,
	                 p_sib_type_11, p_cmas_info_node, &si_segment_count, 
                         &si_segment_counter, sib_mapping_index, &ret);
      if (RRC_FAILURE == ret)
	return RRC_FAILURE;

      rtxDListAppendNode(&p_asn1_si_r8_ies->sib_TypeAndInfo,
          p_asn1_list_node);
    }
    
    /* Encode SI */
    if (PNULL != temp_pctxt)
    {
      pu_setBuffer(temp_pctxt, p_si_msg_buf + *p_si_msg_buf_index,
          MAX_SI_MSG_LENGTH,
          ASN_RRC_BUF_TYPE);
      enc_res = asn1PE_BCCH_DL_SCH_Message(temp_pctxt, &asn1_si_msg);
    }
    else
    {
      pu_setBuffer(pctxt, p_si_msg_buf + *p_si_msg_buf_index,
          MAX_SI_MSG_LENGTH,
          ASN_RRC_BUF_TYPE);
      enc_res = asn1PE_BCCH_DL_SCH_Message(pctxt, &asn1_si_msg);
    }
    if (RT_OK != enc_res)
    {
      /* SPR 20044 Fix Start*/
      char buff[500];
      rtxErrGetTextBuf(pctxt, buff ,500);
      RRC_CSC_TRACE(RRC_WARNING, "Can't encode SI:%s", buff);
      /* SPR 20044 Fix Stop*/

      ret_val = RRC_FAILURE;
      /*Generating Protocol Event LOCAL_EVENT_RRC_PROTOCOL_ERROR*/
      p_event_rrc_protocol_err = rrc_mem_get(sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));
      if (PNULL != p_event_rrc_protocol_err)
      {
          memset_wrapper(p_event_rrc_protocol_err, RRC_NULL, 
              sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));

          p_event_rrc_protocol_err->header.length = 
              sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT);
          p_event_rrc_protocol_err->header.event_id = LOCAL_EVENT_RRC_PROTOCOL_ERROR;

          l3_strncpy_wrapper((S8 *)(p_event_rrc_protocol_err->EVENT_PARAM_RRC_MESSAGE_DISCARDED),
              (const S8 *)"RRC SYSTEM INFORMATION", RRC_MESSAGE_DISCARDED_SIZE);

          rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_rrc_protocol_err);
      }
      break;
    }
    else
    {
	/* Generating Protocol Event LOCAL_RRC_SYSTEM_INFORMATION */    
        encoded_msg_len = (U32)pe_GetMsgLen(pctxt);

        rrc_asn1PrtToStr_BCCH_DL_SCH_Message (RRC_ASN, "BCCH_DL_SCH_Message",
                &asn1_si_msg, "SystemInformation", p_si_msg_buf, encoded_msg_len);
	
        p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                encoded_msg_len);
        if (PNULL != p_event_header)
        {
            memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                      encoded_msg_len));
	    p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + 
	  	      encoded_msg_len);
	    p_event_header->event_id = LOCAL_RRC_SYSTEM_INFORMATION;
	    p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
	    p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;
		
		    
	    l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID, p_cell_ctx->cfg_cell_param.
         	      sib_type_1_Info.cell_access_related_info.cell_Id, sizeof(U32));
    
	    p_event_header->EVENT_PARAM_EUTRANCELL_FROID = p_event_header->
          	      EVENT_PARAM_CELL_ID;
                    
	    l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                      sizeof(EVENT_EXTERNAL_HEADER)),
                      (const void*)p_si_msg_buf,
                      encoded_msg_len);

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
        }

      p_si_msg_info->si_segment_data[si_segment_counter - 1].
        starting_offset = *p_si_msg_buf_index;
      if (PNULL != temp_pctxt)
      {
        *p_si_msg_buf_index = (U16)(*p_si_msg_buf_index + 
                 pe_GetMsgLen(temp_pctxt));
        sib_length = pe_GetMsgLen(temp_pctxt);
      }
      else
      {
        *p_si_msg_buf_index = (U16)(*p_si_msg_buf_index + 
                 pe_GetMsgLen(pctxt));
        sib_length =  pe_GetMsgLen(pctxt);
      }

      RRC_ASSERT(*p_si_msg_buf_index <= MAX_SI_MSG_LENGTH);

      RRC_CSC_TRACE(RRC_INFO, "SI encoded, ASN.1 size: %u, %s", 
          *p_si_msg_buf_index, change_data_to_str(*p_si_msg_buf_index, p_si_msg_buf));

    }
    /*SPR 7522 start */
    if(RRM_RRC_SIB_TYPE_8 == p_scheduling_info->sib_mapping_info.sib_type[sib_mapping_index])
    {
    /*SPR 7522 stop */
    /*SPR_6664 Start*/
	    p_si_msg_info->si_msg_param_counter++;
    /*SPR_6664 Stop*/
    }
    temp_pctxt=PNULL;
  }
  p_si_msg_info->num_of_segment = si_segment_count;
  p_si_msg_info->si_length = *p_si_msg_buf_index;
  /*CR 491 Changes */
  if( RRC_FAILURE == check_sib_size(p_cell_ctx,sib_length))
  {
    RRC_CSC_TRACE(RRC_INFO, "SI encoded, ASN.1 size  exceeding Limits"); 
    ret_val = RRC_FAILURE;
  }
  /*CR 491 Changes */

  RRC_CSC_UT_TRACE_EXIT();
  return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: encode_pws_si_fill_sib_8_to_sis
 *
 *   DESCRIPTION:
 *       This function fills SIB8 in SI.
 *
 *   RETURNS:
 *       Pointer to OSCTXT
 *
 ******************************************************************************/
static OSCTXT*  encode_pws_si_fill_sib_8_to_sis
(
 /* for memory allocation */
 OSCTXT                              		*pctxt,
 /* Cell Context */
 csc_cell_ctx_t                       		*p_cell_ctx,
 /* destination */
 rrc_si_msg_info_t               		*p_si_msg_info,
 /* si-Periodicity and SIBs */
 scheduling_info_t                    	*p_scheduling_info,
 SystemInformation_r8_IEs_sib_TypeAndInfo_element *p_asn1_sib_elem,
 asn1_sibs_t                         		*p_asn1_sibs,
 U8 						*p_si_segment_count,
 U8 						*p_si_segment_counter,
 rrc_return_et					*p_ret_val
)
{
  OSCTXT                              		*temp_pctxt=PNULL;
  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != pctxt);
  RRC_ASSERT(PNULL != p_si_msg_info);
  RRC_ASSERT(PNULL != p_asn1_sib_elem);
  RRC_ASSERT(PNULL != p_cell_ctx);
  U16     periodicity_in_sfn = RRC_NULL;

  /*As this function is being called in Cell Setup and Cell Reconfiguration both. So in case
    of cell setup is being done, very first instance before cell setup will go as it goes before.
    This will be the default instance as sent by RRM with segment count= 1 */
  if(1 == *p_si_segment_count) /*do the following operations only once*/
  {
    /*Update the SIB8 SFN List */

    periodicity_in_sfn = fill_si_periodicity_in_sfn(p_scheduling_info->si_periodicity);
    update_sib8_sfn_list(p_cell_ctx->start_sib8_sfn, periodicity_in_sfn);

    p_cell_ctx->start_sib8_sfn = p_si_msg_info->starting_sfn;
    p_cell_ctx->start_sib8_sf = p_si_msg_info->starting_sf;
    p_cell_ctx->sib8_periodicity = periodicity_in_sfn;
  }
  if(RRC_FALSE == p_cell_ctx->cell_setup_done)
  {
    p_asn1_sib_elem->u.sib8 = &p_asn1_sibs->asn1_sib8;
  }
  else
  {
    if(p_cell_ctx->sib_modify_source == RRM_TRIGGERED_SIB8_NO_MODIFY)
    {
      p_asn1_sib_elem->u.sib8 = &p_asn1_sibs->asn1_sib8;
      p_si_msg_info->rrc_mac_sched_reqd_for_sib8 = SIB8_NO_CHANGE;
    }          
    else
    {
      if(1 == *p_si_segment_count) /*do the following operations only once*/
      {

	if(p_cell_ctx->sib_modify_source == RRM_TRIGGERED_SIB8_MODIFY)
	{
	  /*discard existing SIB8 buffer and apply the new SIB8 buffer starting from the SFN/SF specified*/
	  p_si_msg_info->rrc_mac_sched_reqd_for_sib8 = SIB8_FLUSH_OLD_BUFFER_APPLY_NEW_BUFFER;
	  *p_si_segment_count = MAX_SIB8_INSTANCES;

	  /* In case of RRM_TRIGGERED_SIB8_MODIFY, no need to request SFN, SF & Timestamp from 
	   * L2. Rather use the last updated SFN, SF & Timestamp updated in p_cell_ctx
	   * for caliberating CDMA system time. Calling the following function will update the cdma_base_time
	   * in p_cell_ctx with the one which needs to be broadcasted in next SIB8 instance after keeping a safe
	   * buffer so that L2 can process the same */
	  {
	    rrc_csc_llim_config_cell_resp_t config_cell_resp;
	    memset_wrapper(&config_cell_resp, 0, sizeof(rrc_csc_llim_config_cell_resp_t));
	    config_cell_resp.bitmask |= RRC_CSC_LLIM_SFN_INFO_PRESENT 
	      | RRC_CSC_LLIM_SF_INFO_PRESENT 
	      | RRC_CSC_LLIM_TIMESTAMP_INFO_PRESENT;
	    config_cell_resp.sfn = p_cell_ctx->sfn;
	    config_cell_resp.sf = p_cell_ctx->sf;
	    config_cell_resp.timestamp = p_cell_ctx->timestamp;

	    if(RRC_SUCCESS != resume_high_priority_thread(p_cell_ctx, 
		  config_cell_resp.sfn, 
		  config_cell_resp.sf,
		  config_cell_resp.timestamp))
	    {
	      RRC_CSC_TRACE(RRC_INFO, \
		  "Couldn't generate the CDMA Base Time in %s",__FUNCTION__);
	    }
	    else
	    {
	      RRC_CSC_TRACE(RRC_INFO, \
		  "CDMA Base Time generated successfully in %s",__FUNCTION__);
	    }
	  }
	}
	else if(p_cell_ctx->sib_modify_source == CSC_TRIGGERED_SIB8_MODIFY)
	{
	  /*SI_MSG to contain the set of new MAX_SIB8_INSTANCES SIB8 segments*/
	  p_si_msg_info->rrc_mac_sched_reqd_for_sib8 = SIB8_NEW_BUFFER;
	  *p_si_segment_count = MAX_SIB8_INSTANCES;
	}
      }

      temp_pctxt = pctxt;
      p_asn1_sib_elem->u.sib8 =
	rtxMemAllocType(temp_pctxt, SystemInformationBlockType8);
    /* SPR 12964 Fix Start */
    if (PNULL == p_asn1_sib_elem->u.sib8)
    {
        /* Not enough memory */
        *p_ret_val = RRC_FAILURE;
        RRC_CSC_UT_TRACE_EXIT();
        return temp_pctxt;
    }
    /* SPR 12964 Fix Stop */

      if (p_cell_ctx->recfg_cell_param.broadcast_info.presence_bitmask &
	  CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
      {
	if(RRC_FAILURE == bch_m_check_and_convert_sib8(temp_pctxt,
	      p_asn1_sib_elem->u.sib8,
	      &p_cell_ctx->recfg_cell_param.broadcast_info.sib_type_8_Info,
	      p_cell_ctx, *p_si_segment_counter))
	{
	  RRC_CSC_TRACE(RRC_WARNING, "Can't encode SIB8");
	  *p_ret_val = RRC_FAILURE;
	  return temp_pctxt;
	}
      }
      else if(p_cell_ctx->cfg_cell_param.presence_bitmask & CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
      {
	if(RRC_FAILURE == bch_m_check_and_convert_sib8(temp_pctxt,
	      p_asn1_sib_elem->u.sib8,
	      &p_cell_ctx->cfg_cell_param.sib_type_8_Info,
	      p_cell_ctx, *p_si_segment_counter))
	{
	  RRC_CSC_TRACE(RRC_WARNING, "Can't encode SIB8");
	  *p_ret_val = RRC_FAILURE;
	  return temp_pctxt;
	}
      }
    }
  }
  return temp_pctxt;
}

/******************************************************************************
 *   FUNCTION NAME: encode_pws_si_fill_sibs_to_si
 *
 *   DESCRIPTION:
 *       This function fills SIBs in SIs.
 *
 *   RETURNS:
 *       Pointer to OSCTXT
 *
 ******************************************************************************/
static OSCTXT* encode_pws_si_fill_sibs_to_si
(
 /* for memory allocation */
 OSCTXT                              		*pctxt,
 /* Cell Context */
 csc_cell_ctx_t                       		*p_cell_ctx,
 /* destination */
 rrc_si_msg_info_t               		*p_si_msg_info,
 /* source, all SIBs in ASN.1 format */
 asn1_sibs_t                         		*p_asn1_sibs,
 /* si-Periodicity and SIBs */
 scheduling_info_t                    		*p_scheduling_info,
 SystemInformation_r8_IEs_sib_TypeAndInfo_element *p_asn1_sib_elem,
 /* SIB11 information */
 sib_type_11_t                        		*p_sib_type_11,
 /*SIB12 information*/  
 cmas_info_t                          		*p_cmas_info_node,
 U8 						*p_si_segment_count,
 U8 						*p_si_segment_counter,
 U8 						sib_mapping_index,
 rrc_return_et					*p_ret_val
)
{
  OSCTXT                              		*temp_pctxt = PNULL;
  U16 warning_msg_segment_size = RRC_NULL;
  U32 warning_msg_segment_type = RRC_NULL;
  /* Bug_15359 fix starts */
  U16 segment_size = RRC_NULL; 
  /* Bug_15359 fix ends */
  RRC_CSC_UT_TRACE_ENTER();

  RRC_ASSERT(PNULL != pctxt);
  RRC_ASSERT(PNULL != p_si_msg_info);
  RRC_ASSERT(PNULL != p_asn1_sibs);
  RRC_ASSERT(PNULL != p_scheduling_info);
  RRC_ASSERT(PNULL != p_cell_ctx);

  switch (p_scheduling_info->sib_mapping_info.
      sib_type[sib_mapping_index])
  {
    case RRM_RRC_SIB_TYPE_3:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib3;
      p_asn1_sib_elem->u.sib3 = &p_asn1_sibs->asn1_sib3;

      break;
    case RRM_RRC_SIB_TYPE_4:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib4;
      p_asn1_sib_elem->u.sib4 = &p_asn1_sibs->asn1_sib4;

      break;
    case RRM_RRC_SIB_TYPE_5:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib5;
      p_asn1_sib_elem->u.sib5 = &p_asn1_sibs->asn1_sib5;

      break;
    case RRM_RRC_SIB_TYPE_6:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib6;
      p_asn1_sib_elem->u.sib6 = &p_asn1_sibs->asn1_sib6;

      break;
    case RRM_RRC_SIB_TYPE_7:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib7;
      p_asn1_sib_elem->u.sib7 = &p_asn1_sibs->asn1_sib7;

      break;
    case RRM_RRC_SIB_TYPE_8:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib8;

      temp_pctxt = encode_pws_si_fill_sib_8_to_sis(pctxt, p_cell_ctx, p_si_msg_info, p_scheduling_info,
	  p_asn1_sib_elem, p_asn1_sibs, p_si_segment_count, p_si_segment_counter, p_ret_val);
      if (RRC_FAILURE == *p_ret_val)
	return temp_pctxt;

      break;
    case RRM_RRC_SIB_TYPE_9:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib9;
      p_asn1_sib_elem->u.sib9 = &p_asn1_sibs->asn1_sib9;

      break;
    case RRM_RRC_SIB_TYPE_10:
      p_asn1_sib_elem->t =
	T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib10;
      p_asn1_sib_elem->u.sib10 = &p_asn1_sibs->asn1_sib10;
      if (PNULL != p_cell_ctx->etws_info)
      {
	fill_si_periodicity (&p_cell_ctx->etws_info->sib10_si_periodicity,
	    &p_scheduling_info->si_periodicity);
      }
      break;
    case RRM_RRC_SIB_TYPE_11:
      if (PNULL != p_sib_type_11)
      {
	temp_pctxt = pctxt;
	p_asn1_sib_elem->t =
	  T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib11;
	if (*p_si_segment_counter < p_sib_type_11->num_of_segments)
	{
	  warning_msg_segment_type = notLastSegment;
	  warning_msg_segment_size = (U16)((p_sib_type_11->sib_type_11_Info.warning_msg_size)/
	      (p_sib_type_11->num_of_segments));
	  if (RRC_NULL != ((p_sib_type_11->sib_type_11_Info.warning_msg_size)/
		(p_sib_type_11->num_of_segments)))
	  {
	    warning_msg_segment_size++;
	  }
           /* Bug_15359 fix starts */
	   segment_size = warning_msg_segment_size;
           /* Bug_15359 fix ends */
	}
	else
	{
	  warning_msg_segment_type = lastSegment;
	  warning_msg_segment_size = (U16)((p_sib_type_11->sib_type_11_Info.warning_msg_size)/
	      (p_sib_type_11->num_of_segments));
	  if (RRC_NULL != ((p_sib_type_11->sib_type_11_Info.warning_msg_size)/
		(p_sib_type_11->num_of_segments)))
	  {
	    warning_msg_segment_size++;
	  }
           /* Bug_15359 fix starts */
	   segment_size = warning_msg_segment_size; 
           /* Bug_15359 fix ends */
	  warning_msg_segment_size =  (U16)(p_sib_type_11->sib_type_11_Info.warning_msg_size -
	      (warning_msg_segment_size * (p_sib_type_11->num_of_segments - 1)));
	}
	p_asn1_sib_elem->u.sib11 =
	  rtxMemAllocType(temp_pctxt, SystemInformationBlockType11);
    /* SPR 12964 Fix Start */
    if (PNULL == p_asn1_sib_elem->u.sib11)
    {
        /* Not enough memory */
        *p_ret_val = RRC_FAILURE;
        RRC_CSC_UT_TRACE_EXIT();
        return temp_pctxt;
    }
    /* SPR 12964 Fix Stop */
	if (RRC_FAILURE == bch_m_check_and_convert_sib11(
	      temp_pctxt,
	      p_asn1_sib_elem->u.sib11,
	      &p_sib_type_11->sib_type_11_Info,
	      *p_si_segment_counter,
	      warning_msg_segment_size,
              /* Bug_15359 fix starts */
	      warning_msg_segment_type, segment_size)) 
              /* Bug_15359 fix ends */
	{
	  RRC_CSC_TRACE(RRC_WARNING, "Can't encode SIB11");
	  *p_ret_val = RRC_FAILURE;
	  return temp_pctxt;
	}
	if (PNULL != p_cell_ctx->etws_info)
	{
	  fill_si_periodicity (&p_cell_ctx->etws_info->sib11_si_periodicity,
	      &p_scheduling_info->si_periodicity);
	}
	*p_si_segment_count = p_sib_type_11->num_of_segments;
      }
      break;
    case RRM_RRC_SIB_TYPE_12:
      if(PNULL != p_cmas_info_node)
      {
	if (PNULL == temp_pctxt)
	{
	  temp_pctxt = pctxt;
	}
	p_asn1_sib_elem->t =
	  T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib12_v920;
	if (*p_si_segment_counter < p_cmas_info_node->num_of_segments)
	{
	  warning_msg_segment_type = notLastSegment;
	  warning_msg_segment_size = (U16)((p_cmas_info_node->sib12_info.warning_msg_size)/
	      (p_cmas_info_node->num_of_segments));
	  if (RRC_NULL != ((p_cmas_info_node->sib12_info.warning_msg_size)/
		(p_cmas_info_node->num_of_segments)))
	  {
	    warning_msg_segment_size++;
	  }
	}
	else
	{
	  warning_msg_segment_type = lastSegment;
	  warning_msg_segment_size = (U16)((p_cmas_info_node->sib12_info.warning_msg_size)/
	      (p_cmas_info_node->num_of_segments));
	  if (RRC_NULL != ((p_cmas_info_node->sib12_info.warning_msg_size)/
		(p_cmas_info_node->num_of_segments)))
	  {
	    warning_msg_segment_size++;
	  }
	  warning_msg_segment_size = (U16)(p_cmas_info_node->sib12_info.warning_msg_size -
	      (warning_msg_segment_size * (p_cmas_info_node->num_of_segments - 1)));
	}
	p_asn1_sib_elem->u.sib12_v920 =
	  rtxMemAllocType(temp_pctxt, SystemInformationBlockType12_r9);
    /* SPR 12964 Fix Start */
    if (PNULL == p_asn1_sib_elem->u.sib12_v920)
    {
        /* Not enough memory */
        *p_ret_val = RRC_FAILURE;
        RRC_CSC_UT_TRACE_EXIT();
        return temp_pctxt;
    }
    /* SPR 12964 Fix Stop */
	if (RRC_FAILURE == bch_m_check_and_convert_sib12(
	      temp_pctxt,
	      p_asn1_sib_elem->u.sib12_v920,
	      &p_cmas_info_node->sib12_info,
	      *p_si_segment_counter,
	      warning_msg_segment_size,
	      warning_msg_segment_type))
	{
	  RRC_CSC_TRACE(RRC_WARNING, "Can't encode SIB12");
	  *p_ret_val = RRC_FAILURE;
	  return temp_pctxt;
	}
	*p_si_segment_count = p_cmas_info_node->num_of_segments;
	p_si_msg_info->num_of_repetitions = p_cmas_info_node->
	  num_of_repetitions;
	fill_si_periodicity (&p_cmas_info_node->sib12_si_periodicity,
	    &p_scheduling_info->si_periodicity);
      }
      break;
#ifdef LTE_EMBMS_SUPPORTED
    case RRM_RRC_SIB_TYPE_13:
      {
          p_asn1_sib_elem->t =
              T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib13_v920;
          p_asn1_sib_elem->u.sib13_v920 = &p_asn1_sibs->asn1_sib13;
          break;
      }
#endif
    default:
      RRC_ASSERT(0);
      break;
  }
  RRC_CSC_UT_TRACE_EXIT();
  return temp_pctxt;
}

/******************************************************************************
 *   FUNCTION NAME: encode_pws_si
 *   INPUT        : OSCTXT                              *pctxt
 *                  csc_cell_ctx_t                       *p_cell_ctx
 *                  rrc_si_msg_info_t                   *p_si_msg_info
 *                  U8                                  *p_si_msg_buf
 *                  U16                                 *p_si_msg_buf_index
 *                  asn1_sibs_t                         *p_asn1_sibs
 *                  scheduling_info_t                    *p_scheduling_info
 *                  U8                                  si_n
 *                  U8                                  si_window_length_sf
 *                  sib_type_11_t                        *p_sib_type_11
 *                  cmas_info_t                          *p_cmas_info_node
 *                  rrc_si_transmission_info_list_t       *p_si_transmission_info_list
 *   OUTPUT       : none
 *   DESCRIPTION  : This function encode SI.
 *
 *   RETURNS      : RRC_SUCCESS - if successfully encode SI.
 *                  RRC_FAILURE - otherwise
 *
 ******************************************************************************/
static rrc_return_et encode_pws_si
(
 /* for memory allocation */
 OSCTXT                              *pctxt,
 /* Cell Context */
 csc_cell_ctx_t                       *p_cell_ctx,
 /* destination */
 rrc_si_msg_info_t                   *p_si_msg_info,
 /* destination */
 U8                                  *p_si_msg_buf,
 /*SI Msg Buf Index */
 U16                                 *p_si_msg_buf_index,
 /* source, all SIBs in ASN.1 format */
 asn1_sibs_t                         *p_asn1_sibs,
 /* si-Periodicity and SIBs */
 scheduling_info_t                    *p_scheduling_info,
 /* order of SI in schedulingInformation. 1 based as in TS */
 U8                                  si_n,
 /* si_window_length in sf == ms */
 U8                                  si_window_length_sf,
 /* SIB11 information */
 sib_type_11_t                        *p_sib_type_11,
 /*SIB12 information*/  
 cmas_info_t                          *p_cmas_info_node,
/*si_transmission_info*/
rrc_si_transmission_info_list_t       *p_si_transmission_info_list
/* SPR 17946 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
 ,sib13_scheduling_info_t             *p_sib13_scheduling_info
#endif
/* SPR 17946 Fix End */
)
{
    OSCTXT  *temp_pctxt = PNULL;
    rrc_return_et  ret_val = RRC_SUCCESS;
    BCCH_DL_SCH_Message asn1_si_msg;
    SystemInformation_r8_IEs  *p_asn1_si_r8_ies = PNULL;
    SystemInformation_r8_IEs_sib_TypeAndInfo_element *p_asn1_sib_elem = PNULL;
    OSRTDListNode *p_asn1_list_node  =  PNULL;
    S32  enc_res =  RRC_NULL;
    U8   sib_mapping_index  = RRC_NULL;
    U8  si_segment_counter = 1;
    U8  si_segment_count = 1;
    rrc_bool_et  is_sib11_present = RRC_FALSE;
    rrc_bool_et  is_sib12_present = RRC_FALSE;
    /* SPR 11229 Fix Start */
    U8        sib8_offset = 0;
    /* SPR 11229 Fix Start */

    rrc_return_et       ret = RRC_SUCCESS;

    EVENT_EXTERNAL_HEADER   *p_event_header = PNULL;
    U32                     encoded_msg_len = 0;
    U16                     sib_length = RRC_NULL;
    LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT *p_event_rrc_protocol_err = PNULL;

    U16 x = (U16)((si_n - 1) * si_window_length_sf);

    RRC_CSC_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != pctxt);
    RRC_ASSERT(PNULL != p_si_msg_info);
    RRC_ASSERT(PNULL != p_si_msg_buf);
    RRC_ASSERT(PNULL != p_asn1_sibs);
    RRC_ASSERT(PNULL != p_scheduling_info);

    /* to store params we have to know a pointer to rrc_si_msg_info_t
     *  inside asn1PE_xxx functions */
    pctxt->pUserData = (OSVoidPtr*)p_si_msg_info;
    
  /* Fill SI Index as available in SIB1 
     (subtracting 1, because it was incrementted while
                sending to encode)*/
    p_si_msg_info->si_index = si_n ;
    
    /* convert si_periodicity in MAC format according to
     *  LTE_MAC_Rel_1.2_API_Manual_Rev_2.4.doc */
    p_si_msg_info->periodicity  = (U8)(p_scheduling_info->si_periodicity + 1);
    /* See TS 36.331-840, p27 */
    p_si_msg_info->starting_sfn = x / 10;
    p_si_msg_info->starting_sf  = (U8)(x % 10);
    /* If next ASSERT risen - we have BUG in check_sis_scheduling function */
    /* convert si_periodicity to radio frames */
    RRC_ASSERT(p_si_msg_info->starting_sfn < (8 << p_scheduling_info->
        si_periodicity));
    /* In case SIB is of single type, it has to repeat itself only */
   p_si_msg_info->num_of_repetitions = 0;

  p_si_msg_info->rrc_mac_sched_reqd_for_sib8 = NO_SIB8;
  p_si_msg_info->bitmask |= RRC_CSC_LLIM_TIMESTAMP_REQUIRED_PRESENT;

  encode_si_proc_si_trans_info_list(p_si_transmission_info_list, si_n, 
	p_si_msg_info, si_window_length_sf, &ret_val);

   if ( RRC_FAILURE == ret_val )
   {
       RRC_CSC_UT_TRACE_EXIT();
       return ret_val;
   }

   /* Reset si_msg_param_counter */
   p_si_msg_info->si_msg_param_counter = 0;
   for (si_segment_counter = 1; si_segment_counter <= si_segment_count; 
           si_segment_counter++)
   {
    /* Fill SI message */
    memset_wrapper(&asn1_si_msg, 0, sizeof(asn1_si_msg));

    asn1_si_msg.message.t       = T_BCCH_DL_SCH_MessageType_c1;
    asn1_si_msg.message.u.c1    = rtxMemAllocType(pctxt,
        BCCH_DL_SCH_MessageType_c1);

    if (PNULL == asn1_si_msg.message.u.c1)
    {
        /* Not enough memory */
        return RRC_FAILURE;
    }

    asn1_si_msg.message.u.c1->t =
        T_BCCH_DL_SCH_MessageType_c1_systemInformation;
    asn1_si_msg.message.u.c1->u.systemInformation = rtxMemAllocType(pctxt,
        SystemInformation);

    if (PNULL == asn1_si_msg.message.u.c1->u.systemInformation)
    {
        /* Not enough memory */
        return RRC_FAILURE;
    }

    asn1_si_msg.message.u.c1->u.systemInformation->criticalExtensions.t =
        T_SystemInformation_criticalExtensions_systemInformation_r8;
    p_asn1_si_r8_ies = rtxMemAllocType(pctxt, SystemInformation_r8_IEs);

    if (PNULL == p_asn1_si_r8_ies)
    {
        /* Not enough memory */
        return RRC_FAILURE;
    }

    asn1_si_msg.message.u.c1->u.systemInformation->criticalExtensions.u.
        systemInformation_r8 = p_asn1_si_r8_ies;
    p_asn1_si_r8_ies->m.nonCriticalExtensionPresent = 0;

    /* Init sib_TypeAndInfo list */
    rtxDListInit(&p_asn1_si_r8_ies->sib_TypeAndInfo);

    /* Special case in 36.331. If we receive value 1 we should add SIB2 to SI */
    if(1 == p_si_msg_info->si_index)
    {
        /* Allocate node for SystemInformationBlock2 */
        rtxDListAllocNodeAndData (pctxt,
            SystemInformation_r8_IEs_sib_TypeAndInfo_element,
                &p_asn1_list_node, &p_asn1_sib_elem);

        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            return RRC_FAILURE;
        }
        asn1Init_SystemInformation_r8_IEs_sib_TypeAndInfo_element(p_asn1_sib_elem);
        /* Fill SystemInformationBlockType2 */
        p_asn1_sib_elem->t =
            T_SystemInformation_r8_IEs_sib_TypeAndInfo_element_sib2;
        p_asn1_sib_elem->u.sib2 = &p_asn1_sibs->asn1_sib2;

        rtxDListAppendNode(&p_asn1_si_r8_ies->sib_TypeAndInfo,
            p_asn1_list_node);
    }

    for (sib_mapping_index = 0; sib_mapping_index < p_scheduling_info->
        sib_mapping_info.count; sib_mapping_index++)
    {
        if ( RRM_RRC_SIB_TYPE_11 == 
              p_scheduling_info->sib_mapping_info.
                        sib_type[sib_mapping_index] )
        {
            is_sib11_present = RRC_TRUE;
        }    
               
        if ( RRM_RRC_SIB_TYPE_12 == 
              p_scheduling_info->sib_mapping_info.
                        sib_type[sib_mapping_index] )
        {
            is_sib12_present = RRC_TRUE;
        }    
#ifdef LTE_EMBMS_SUPPORTED
        if ( RRM_RRC_SIB_TYPE_13 == 
              p_scheduling_info->sib_mapping_info.
                        sib_type[sib_mapping_index] )
        {
            if(PNULL != p_sib13_scheduling_info)
            {
                /* SPR 18439 Fix Start */
                p_sib13_scheduling_info->si_index = si_n;
                /* SPR 18439 Fix End */
            }
        }
#endif
               
        /* SPR 11229 Fix Start */
        if ( RRM_RRC_SIB_TYPE_8 == 
              p_scheduling_info->sib_mapping_info.
                        sib_type[sib_mapping_index] )
        {
            /* Incrementing SIB8 offset with SI Window's boundary SFN */
            p_cell_ctx->sib8_si_window_offset = 
                     si_window_length_sf / 10;

            /* Incrementing SIB8 offset with SI window's boundary SF
               and SI's starting SF */
            sib8_offset = (si_window_length_sf % 10) +
                (p_si_msg_info->starting_sf);

            if (sib8_offset > 0)
            {
                p_cell_ctx->sib8_si_window_offset++;

                if (sib8_offset > 10) /* Needs to increment by 2 SFN */
                {
                    p_cell_ctx->sib8_si_window_offset++;
                }
            }
        }    
        /* SPR 11229 Fix Start */
    }
    
    if ( (RRC_TRUE == is_sib11_present) &&
         (RRC_TRUE == is_sib12_present) &&
         (PNULL != p_cmas_info_node) &&
         (PNULL != p_sib_type_11) )
    {    
       /* No. of segments of SIB11 and SIB12 in same SI are not equal */ 
       if ( p_sib_type_11->num_of_segments !=
                p_cmas_info_node->num_of_segments )
       {
           return RRC_FAILURE;
       }    
    }
    
    /* Find SIBs which will be added to SI in
     *  p_scheduling_info->sib_mapping_info */
    for (sib_mapping_index = 0; sib_mapping_index < p_scheduling_info->
        sib_mapping_info.count; sib_mapping_index++)
    {
        /* Allocate node for SystemInformationBlockX */
        rtxDListAllocNodeAndData(pctxt,
            SystemInformation_r8_IEs_sib_TypeAndInfo_element,
                &p_asn1_list_node, &p_asn1_sib_elem);

        if (PNULL == p_asn1_list_node)
        {
            /* Not enough memory */
            return RRC_FAILURE;
        }
        asn1Init_SystemInformation_r8_IEs_sib_TypeAndInfo_element(p_asn1_sib_elem);
        /* Fill SystemInformationBlockTypeX */

        temp_pctxt = encode_pws_si_fill_sibs_to_si(pctxt, p_cell_ctx, p_si_msg_info, p_asn1_sibs, 
                     	p_scheduling_info, p_asn1_sib_elem,
                	p_sib_type_11, p_cmas_info_node, &si_segment_count, &si_segment_counter, sib_mapping_index, &ret);
        if (RRC_FAILURE == ret)
         	return RRC_FAILURE;

        rtxDListAppendNode(&p_asn1_si_r8_ies->sib_TypeAndInfo,
            p_asn1_list_node);
    }

    /* Encode SI */
    if (PNULL != temp_pctxt)
    {
      pu_setBuffer(temp_pctxt, p_si_msg_buf + *p_si_msg_buf_index,
          MAX_SI_MSG_LENGTH,
          ASN_RRC_BUF_TYPE);
      enc_res = asn1PE_BCCH_DL_SCH_Message(temp_pctxt, &asn1_si_msg);
    }
    else
    {
      pu_setBuffer(pctxt, p_si_msg_buf + *p_si_msg_buf_index,
          MAX_SI_MSG_LENGTH,
          ASN_RRC_BUF_TYPE);
      enc_res = asn1PE_BCCH_DL_SCH_Message(pctxt, &asn1_si_msg);
    }
    if (RT_OK != enc_res)
    {
        RRC_CSC_TRACE(RRC_WARNING, "Can't encode SI");

        ret_val = RRC_FAILURE;
        
	/*Generating Protocol Event LOCAL_EVENT_RRC_PROTOCOL_ERROR*/
        p_event_rrc_protocol_err = rrc_mem_get(sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));
        if (PNULL != p_event_rrc_protocol_err)
        {
            memset_wrapper(p_event_rrc_protocol_err, RRC_NULL, 
                    sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT));

            p_event_rrc_protocol_err->header.length = 
                sizeof(LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT);
            p_event_rrc_protocol_err->header.event_id = LOCAL_EVENT_RRC_PROTOCOL_ERROR;

            l3_strncpy_wrapper((S8 *)(p_event_rrc_protocol_err->EVENT_PARAM_RRC_MESSAGE_DISCARDED),
                    (const S8 *)"RRC SYSTEM INFORMATION", RRC_MESSAGE_DISCARDED_SIZE);

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_rrc_protocol_err);
        }
        break;
      
    }
    else
    {
        /* Generating Protocol Event LOCAL_RRC_SYSTEM_INFORMATION */    
        encoded_msg_len = (U32)pe_GetMsgLen(pctxt);

        rrc_asn1PrtToStr_BCCH_DL_SCH_Message (RRC_ASN, "BCCH_DL_SCH_Message",
                &asn1_si_msg, "SystemInformation", (p_si_msg_buf + *p_si_msg_buf_index),
                encoded_msg_len);

        p_event_header = rrc_mem_get(sizeof(EVENT_EXTERNAL_HEADER) +
                encoded_msg_len);
        if (PNULL != p_event_header)
        {
            memset_wrapper(p_event_header, RRC_NULL, (sizeof(EVENT_EXTERNAL_HEADER) +
                        encoded_msg_len));
            p_event_header->length = (sizeof(EVENT_EXTERNAL_HEADER) + 
                    encoded_msg_len);
            p_event_header->event_id = LOCAL_RRC_SYSTEM_INFORMATION;
            p_event_header->EVENT_PARAM_MESSAGE_DIRECTION = MESSAGE_DIRECTION_SEND;
            p_event_header->EVENT_PARAM_L3MESSAGE_LENGTH = encoded_msg_len;


            l3_memcpy_wrapper(&p_event_header->EVENT_PARAM_CELL_ID, p_cell_ctx->cfg_cell_param.
                    sib_type_1_Info.cell_access_related_info.cell_Id, sizeof(U32));

            p_event_header->EVENT_PARAM_EUTRANCELL_FROID = p_event_header->
                EVENT_PARAM_CELL_ID;

            l3_memcpy_wrapper((void*)((U8*)p_event_header + 
                        sizeof(EVENT_EXTERNAL_HEADER)),
                    (const void*)p_si_msg_buf + *p_si_msg_buf_index,
                    encoded_msg_len);

            rrc_generate_protocol_events(RRC_PROTOCOL_EVENT, p_event_header);
        }
        p_si_msg_info->si_segment_data[si_segment_counter - 1].
        starting_offset = *p_si_msg_buf_index;
      if (PNULL != temp_pctxt)
      {
        *p_si_msg_buf_index = (U16)(*p_si_msg_buf_index +
                 pe_GetMsgLen(temp_pctxt));
        sib_length = pe_GetMsgLen(temp_pctxt);
      }
      else
      {
        *p_si_msg_buf_index = (U16)(*p_si_msg_buf_index +
                 pe_GetMsgLen(pctxt));
        sib_length = pe_GetMsgLen(pctxt);
      }

      RRC_ASSERT(*p_si_msg_buf_index <= MAX_SI_MSG_LENGTH);

      RRC_CSC_TRACE(RRC_INFO, "SI encoded, ASN.1 size: %u, %s", 
          *p_si_msg_buf_index, change_data_to_str(*p_si_msg_buf_index, p_si_msg_buf));


    }
    /*SPR 7522 start */
    if(RRM_RRC_SIB_TYPE_8 == p_scheduling_info->sib_mapping_info.sib_type[sib_mapping_index])
    {
    /*SPR 7522 stop */
    /*SPR_6664 Start*/
	    p_si_msg_info->si_msg_param_counter++;
    /*SPR_6664 Stop*/
    }
    temp_pctxt=PNULL;
  }
  p_si_msg_info->num_of_segment = si_segment_count;
  p_si_msg_info->si_length = *p_si_msg_buf_index;
  /*CR 491 Changes */
  if( RRC_FAILURE == check_sib_size(p_cell_ctx,sib_length))
  {
    RRC_CSC_TRACE(RRC_INFO, "SI encoded, ASN.1 size  exceeding Limits"); 
    ret_val = RRC_FAILURE;
  }
  /*CR 491 Changes */

  RRC_CSC_UT_TRACE_EXIT();
  return ret_val;
}

/******************************************************************************
 *   FUNCTION NAME: encode_sis
 *   INPUT        : OSCTXT                              *pctxt
 *                  rrc_si_msg_req_t                *p_si_msg_req
 *                  asn1_sibs_t                         *p_asn1_sibs
 *                  scheduling_info_list_t               *p_scheduling_info_list
 *                  csc_cell_ctx_t                       *p_cell_ctx
 *                  U8                                   si_window_length
 *                  sib_type_11_t                        *p_sib_type_11
 *                  rrc_si_transmission_info_list_t       *p_si_transmission_info_list
 *   OUTPUT       : none
 *   DESCRIPTION  : This function encode SIs to be broadcasted.
 *
 *   RETURNS      : RRC_SUCCESS - if successfully encode SIs.
 *                  RRC_FAILURE - otherwise
 *
 ******************************************************************************/
static rrc_return_et encode_sis
(
 /* for memory allocation */
 OSCTXT                              *pctxt,
 /* destination */
 rrc_si_msg_req_t                *p_si_msg_req,
 /* source, all SIBs in ASN.1 format */
 asn1_sibs_t                         *p_asn1_sibs,
 /* SIs SchedulingInformation */
 scheduling_info_list_t               *p_scheduling_info_list,
 /* Cell context */
 csc_cell_ctx_t                       *p_cell_ctx,
 /* SI Window Length */
 U8                                   si_window_length,
 /* SIB 11 information */
 sib_type_11_t                        *p_sib_type_11,
/*si_transmission_info*/
rrc_si_transmission_info_list_t       *p_si_transmission_info_list
/* SPR 17946 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
,sib13_scheduling_info_t              *sib13_scheduling_info_p
#endif
/* SPR 17946 Fix End */
 )
{
  rrc_return_et ret_val = RRC_FAILURE;
  U8 si = RRC_NULL;
  scheduling_info_t *p_scheduling_info = RRC_NULL;
  U8 si_msg_index = 0;
  U16 si_msg_buf_index = 0;
  U8 sib_map_index = 0;
  rrc_bool_et sib12_present = RRC_FALSE;
  cmas_info_t *cmas_info_node      = PNULL;

  RRC_CSC_UT_TRACE_ENTER();

  p_si_msg_req->si_window_size        =   si_window_length_2_sf
    (si_window_length);

  if (p_scheduling_info_list->count > ARRSIZE(p_si_msg_req->si_msg_info))
  {
    RRC_CSC_TRACE(RRC_WARNING, "Too much SIs for outgoing message.");

    ret_val = RRC_FAILURE;
  }
  else
  {
    RRC_ASSERT(p_scheduling_info_list->count >= 1);

    if((PNULL != p_si_transmission_info_list) && 
		  (p_scheduling_info_list->count < 
		p_si_transmission_info_list->count)) 
    {
          RRC_CSC_TRACE(RRC_WARNING, 
            "The count of scheduling_info_list- [%d], is less than  "
            "transmission_info count - [%d]",p_scheduling_info_list->count,
            p_si_transmission_info_list->count);
    }

    for (si = 0; si < p_scheduling_info_list->count; si++)
    {
      sib12_present = RRC_FALSE;
      if (MAX_SI_MESSAGE <= si)
      {
          ret_val = RRC_FAILURE;
          return ret_val;
      }
      p_scheduling_info = (scheduling_info_t *)(p_scheduling_info_list->scheduling_info + si);
      for (sib_map_index = 0; sib_map_index <
                 p_scheduling_info->sib_mapping_info.count; sib_map_index++)
      {
        if ( RRM_RRC_SIB_TYPE_12 == p_scheduling_info->sib_mapping_info. \
                     sib_type[sib_map_index] )
        {
          sib12_present = RRC_TRUE;
        }
      }
      if ( RRC_TRUE == sib12_present )
      {
        for ( cmas_info_node = (cmas_info_t *)(st_get_first_node
                ((const search_tree_t *)&(p_cell_ctx->cmas_info_list)));
                 cmas_info_node; cmas_info_node = (cmas_info_t *)(st_get_next_node
                ((const search_tree_node_t *)cmas_info_node)))
        {
          ret_val = encode_pws_si(    pctxt,
              p_cell_ctx,
              &p_si_msg_req->si_msg_info[si_msg_index],
              p_si_msg_req->si_msg_buf,
              &si_msg_buf_index,
              p_asn1_sibs,
              p_scheduling_info,
              (U8)(si + 1),
              si_window_length_2_sf(si_window_length),
              p_sib_type_11,
              cmas_info_node,
              p_si_transmission_info_list
              /* SPR 17946 Fix Start */
              #ifdef LTE_EMBMS_SUPPORTED
              ,sib13_scheduling_info_p
              #endif
              /* SPR 17946 Fix End */
              );
          if (RRC_FAILURE == ret_val)
          {
            return ret_val;
          }
          si_msg_index++;
        }
      }
      else
      {
        ret_val = encode_pws_si(    pctxt,
              p_cell_ctx,
              &p_si_msg_req->si_msg_info[si_msg_index],
              p_si_msg_req->si_msg_buf,
              &si_msg_buf_index,
              p_asn1_sibs,
              p_scheduling_info,
              (U8)(si + 1),
              si_window_length_2_sf(si_window_length),
              p_sib_type_11,
              PNULL,
              p_si_transmission_info_list
              /* SPR 17946 Fix Start */
              #ifdef LTE_EMBMS_SUPPORTED
              ,sib13_scheduling_info_p
              #endif
              /* SPR 17946 Fix End */
              );
         if (RRC_FAILURE == ret_val)
         {
            return ret_val;
         }
         si_msg_index++;
      }
 
      if (RRC_MAX_SI_MSGS < si_msg_index)
      {
          ret_val = RRC_FAILURE;
          return ret_val;
      }
    }
    p_si_msg_req->num_si_message = si_msg_index;
  }
  RRC_CSC_UT_TRACE_EXIT();
  return ret_val;
}


/******************************************************************************
 *   FUNCTION NAME: encode_sis_pws
 *   INPUT        : OSCTXT                              *pctxt
 *                  rrc_si_msg_req_t                *p_si_msg_req
 *                  asn1_sibs_t                         *p_asn1_sibs
 *                  scheduling_info_list_t               *p_scheduling_info_list
 *                  csc_cell_ctx_t                       *p_cell_ctx
 *                  U8                                   si_window_length
 *                  sib_type_11_t                        *p_sib_type_11
 *                  rrc_si_transmission_info_list_t       *p_si_transmission_info_list
 *   OUTPUT       : none
 *   DESCRIPTION  : This function encode SIs to be broadcasted.
 *
 *   RETURNS      : RRC_SUCCESS - if successfully encode SIs.
 *                  RRC_FAILURE - otherwise
 *
 ******************************************************************************/
static rrc_return_et encode_sis_pws
(
 /* for memory allocation */
 OSCTXT                              *pctxt,
 /* destination */
 rrc_si_msg_req_t                *p_si_msg_req,
 /* source, all SIBs in ASN.1 format */
 asn1_sibs_t                         *p_asn1_sibs,
 /* SdpIs SchedulingInformation */
 scheduling_info_list_t               *p_scheduling_info_list,
 /* Cell context */
 csc_cell_ctx_t                       *p_cell_ctx,
 /* SI Window Length */
 U8                                   si_window_length,
 /* SIB 11 information */
 sib_type_11_t                        *p_sib_type_11,
/*si_transmission_info*/
rrc_si_transmission_info_list_t       *p_si_transmission_info_list
/* SPR 17946 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
,sib13_scheduling_info_t              *p_sib13_scheduling_info
#endif
/* SPR 17946 Fix End */
 )
{
  rrc_return_et ret_val = RRC_FAILURE;
  U8 si = RRC_NULL;
  scheduling_info_t *p_scheduling_info = RRC_NULL;
  U8 si_msg_index = 0;
  U8 sib_map_index = 0;
  rrc_bool_et sib12_present = RRC_FALSE;
  cmas_info_t *cmas_info_node      = PNULL;
  U32 cmas_key = RRC_NULL;
  U16 si_msg_buf_index = 0;

  RRC_CSC_UT_TRACE_ENTER();

  p_si_msg_req->si_window_size        =   si_window_length_2_sf
    (si_window_length);


  if (p_scheduling_info_list->count > ARRSIZE(p_si_msg_req->si_msg_info))
  {
    RRC_CSC_TRACE(RRC_WARNING, "Too much SIs for outgoing message.");

    ret_val = RRC_FAILURE;
  }
  else
  {
    RRC_ASSERT(p_scheduling_info_list->count >= 1);

    if((PNULL != p_si_transmission_info_list) && 
		  (p_scheduling_info_list->count < 
		p_si_transmission_info_list->count)) 
    {
          RRC_CSC_TRACE(RRC_WARNING, 
            "The count of scheduling_info_list- [%d], is less than  "
            "transmission_info count - [%d]",p_scheduling_info_list->count,
            p_si_transmission_info_list->count);
    }

    for (si = 0; si < p_scheduling_info_list->count; si++)
    {
      sib12_present = RRC_FALSE;
      if (MAX_SI_MESSAGE <= si)
      {
          ret_val = RRC_FAILURE;
          return ret_val;
      }
      p_scheduling_info = (scheduling_info_t *)(p_scheduling_info_list->scheduling_info + si);
      for (sib_map_index = 0; sib_map_index <
                 p_scheduling_info->sib_mapping_info.count; sib_map_index++)
      {
        if (MAX_SIB_MESSAGE_1 <= sib_map_index)
        {
          ret_val = RRC_FAILURE;
          return ret_val;
        }
        if ( RRM_RRC_SIB_TYPE_12 == p_scheduling_info->sib_mapping_info. \
                     sib_type[sib_map_index] )
        {
          sib12_present = RRC_TRUE;
        }
      }
      if ( RRC_TRUE == sib12_present )
      {
        if ( RRC_TRUE == p_cell_ctx->cmas_flag )
        {
          cmas_key = (U32)(p_cell_ctx->p_enb_pws_req->message_identifier[1] << 24);
          cmas_key |= (U32)(p_cell_ctx->p_enb_pws_req->message_identifier[0] << 16);
          cmas_key |= (U32)(p_cell_ctx->p_enb_pws_req->serial_number[1] << 8);
          cmas_key |= (U32)(p_cell_ctx->p_enb_pws_req->serial_number[0]);
          cmas_info_node = (cmas_info_t *)(st_find_tree_node(
                  (const search_tree_t *)&(p_cell_ctx->cmas_info_list),
                  (const void*)&cmas_key));
          if (PNULL != cmas_info_node)
          {
            ret_val = encode_pws_si(    pctxt,
              p_cell_ctx,
              &p_si_msg_req->si_msg_info[si_msg_index],
              p_si_msg_req->si_msg_buf,
              &si_msg_buf_index,
              p_asn1_sibs,
              p_scheduling_info,
              (U8)(si + 1),
              si_window_length_2_sf(si_window_length),
              p_sib_type_11,
              cmas_info_node,
              p_si_transmission_info_list
             /* SPR 17946 Fix Start */
              #ifdef LTE_EMBMS_SUPPORTED
              ,p_sib13_scheduling_info
              #endif
             /* SPR 17946 Fix End */
              );

            if (RRC_FAILURE == ret_val)
            {
              return ret_val;
            }
            si_msg_index++;
          }
        }
        for ( cmas_info_node = (cmas_info_t *)(st_get_first_node
                ((const search_tree_t *)&(p_cell_ctx->cmas_info_list)));
                 cmas_info_node; cmas_info_node = (cmas_info_t *)(st_get_next_node
                ((const search_tree_node_t *)cmas_info_node)))
        {
          if (cmas_info_node->cmas_key != cmas_key)
          {
            ret_val = encode_pws_si(    pctxt,
              p_cell_ctx,
              &p_si_msg_req->si_msg_info[si_msg_index],
              p_si_msg_req->si_msg_buf,
              &si_msg_buf_index,
              p_asn1_sibs,
              p_scheduling_info,
              (U8)(si + 1),
              si_window_length_2_sf(si_window_length),
              p_sib_type_11,
              cmas_info_node,
              p_si_transmission_info_list
             /* SPR 17946 Fix Start */
              #ifdef LTE_EMBMS_SUPPORTED
              ,p_sib13_scheduling_info
              #endif
              );
             /* SPR 17946 Fix End */
            if (RRC_FAILURE == ret_val)
            {
              return ret_val;
            }
            si_msg_index++;
          }
        }
      }
      else
      {
        ret_val = encode_pws_si(    pctxt,
              p_cell_ctx,
              &p_si_msg_req->si_msg_info[si_msg_index],
              p_si_msg_req->si_msg_buf,
              &si_msg_buf_index,
              p_asn1_sibs,
              p_scheduling_info,
              (U8)(si + 1),
              si_window_length_2_sf(si_window_length),
              p_sib_type_11,
              PNULL,
              p_si_transmission_info_list
             /* SPR 17946 Fix Start */
              #ifdef LTE_EMBMS_SUPPORTED
              ,p_sib13_scheduling_info
              #endif
              );
             /* SPR 17946 Fix End */
         if (RRC_FAILURE == ret_val)
         {
            return ret_val;
         }
         si_msg_index++;
      }
      if (RRC_MAX_SI_MSGS < si_msg_index)
      {
          ret_val = RRC_FAILURE;
          return ret_val;
      }
    }
    p_si_msg_req->num_si_message = si_msg_index;
  }

  RRC_CSC_UT_TRACE_EXIT();
  return ret_val;
}


/******************************************************************************
*   FUNCTION NAME: encode_sis_with_new_si
 *   INPUT        : OSCTXT                              *pctxt
 *                  rrc_si_msg_req_t                *p_si_msg_req
 *                  asn1_sibs_t                         *p_asn1_sibs
 *                  scheduling_info_list_t               *p_scheduling_info_list
 *                  csc_cell_ctx_t                       *p_cell_ctx
 *                  U8                                   si_window_length
 *                  sib_type_11_t                        *p_sib_type_11
 *                  rrc_si_transmission_info_list_t       *p_si_transmission_info_list
 *   OUTPUT       : none
 *   DESCRIPTION  : This function encode SIs to be broadcasted.
*    RETURNS      : RRC_SUCCESS - if successfully encode SIs.
*                   RRC_FAILURE - otherwise
*
******************************************************************************/
static rrc_return_et encode_sis_with_new_si
(
    /* for memory allocation */
    OSCTXT                              *pctxt,
    /* destination */
    rrc_si_msg_req_t                *p_si_msg_req,
    /* source, all SIBs in ASN.1 format */
    asn1_sibs_t                         *p_asn1_sibs,
    /* SIs SchedulingInformation */
     new_scheduling_info_list_t         *p_scheduling_info_list,
    /* Cell Context */
     csc_cell_ctx_t                       *p_cell_ctx,
    /* si_window_length */
    U8                                  si_window_length,
    /* SIB 11 information */
    sib_type_11_t                        *p_sib_type_11,
   /*si_transmission_info*/
   rrc_si_transmission_info_list_t       *p_si_transmission_info_list
#ifdef LTE_EMBMS_SUPPORTED
 ,sib13_scheduling_info_t *sib13_scheduling_info_p
#endif
)
{
    rrc_return_et ret_val = RRC_FAILURE;
    new_scheduling_info_t *p_scheduling_info = RRC_NULL;
    U8 si_msg_index = 0;
    U8 si = RRC_NULL;
    U8 sib_map_index = 0;
    rrc_bool_et sib12_present = RRC_FALSE;
    cmas_info_t *cmas_info_node      = PNULL;
    U16         si_msg_buf_index = 0;

    RRC_CSC_UT_TRACE_ENTER();

    p_si_msg_req->si_window_size        =   si_window_length_2_sf
        (si_window_length);

    if (p_scheduling_info_list->count > ARRSIZE(p_si_msg_req->si_msg_info))
    {
      RRC_CSC_TRACE(RRC_WARNING, "Too much SIs for outgoing message.");

      ret_val = RRC_FAILURE;
    }
    else
    {
      RRC_ASSERT(p_scheduling_info_list->count >= 1);
      
      if((PNULL != p_si_transmission_info_list) && 
		  (p_scheduling_info_list->count < 
		p_si_transmission_info_list->count)) 
      {
          RRC_CSC_TRACE(RRC_WARNING, 
            "The count of scheduling_info_list- [%d], is less than  "
            "transmission_info count - [%d]",p_scheduling_info_list->count,
            p_si_transmission_info_list->count);
      }

      for (si = 0; si < p_scheduling_info_list->count; si++)
      {
        sib12_present = RRC_FALSE;
        if (MAX_SI_MESSAGE <= si)
        {
          ret_val = RRC_FAILURE;
          return ret_val;
        }
        p_scheduling_info = (new_scheduling_info_t *)(p_scheduling_info_list->scheduling_info + si);
        for (sib_map_index = 0; sib_map_index <
                 p_scheduling_info->sib_mapping_info.count; sib_map_index++)
        {
          if ( RRM_RRC_SIB_TYPE_12 == p_scheduling_info->sib_mapping_info. \
                     sib_type[sib_map_index] )
          {
            sib12_present = RRC_TRUE;
          }
        }
        if ( RRC_TRUE == sib12_present )
        {
          for ( cmas_info_node = (cmas_info_t *)(st_get_first_node
                ((const search_tree_t *)&(p_cell_ctx->cmas_info_list)));
                 cmas_info_node; cmas_info_node = (cmas_info_t *)(st_get_next_node
                ((const search_tree_node_t *)cmas_info_node)))
          {
              ret_val = encode_si_with_new_mapping(    pctxt,
                                    p_cell_ctx,
                                    p_si_msg_req->si_msg_info + si_msg_index,
                                    p_si_msg_req->si_msg_buf,
                                    &si_msg_buf_index,
                                    p_asn1_sibs,
                                    p_scheduling_info,
                                    (U8)(si + 1),
                                    si_window_length_2_sf(si_window_length),
                                    p_sib_type_11,
                                    cmas_info_node,
                                    p_si_transmission_info_list
                                    #ifdef LTE_EMBMS_SUPPORTED
                                    ,sib13_scheduling_info_p
                                    #endif
                                    );
              if (RRC_FAILURE == ret_val)
              {
                return ret_val;
              }
              si_msg_index++;
          }
        }
        else
        {
          ret_val = encode_si_with_new_mapping(    pctxt,
                                    p_cell_ctx,
                                    p_si_msg_req->si_msg_info + si_msg_index,
                                    p_si_msg_req->si_msg_buf,
                                    &si_msg_buf_index,
                                    p_asn1_sibs,
                                    p_scheduling_info,
                                    (U8)(si + 1),
                                    si_window_length_2_sf(si_window_length),
                                    p_sib_type_11,
                                    PNULL,
                                    p_si_transmission_info_list
                                    #ifdef LTE_EMBMS_SUPPORTED
                                    ,sib13_scheduling_info_p
                                    #endif
                                    );
          if (RRC_FAILURE == ret_val)
          {
            return ret_val;
          }
          si_msg_index++;
        }
        if (RRC_MAX_SI_MSGS < si_msg_index)
        {
          ret_val = RRC_FAILURE;
          return ret_val;
        }
      }
      p_si_msg_req->num_si_message = si_msg_index;
    }

    RRC_CSC_UT_TRACE_EXIT();
    return ret_val;
}



/******************************************************************************
*   FUNCTION NAME: bch_m_mib_sib1_sis
*   INPUT        : rrc_csc_llim_config_cell_req_t  *p_llim_cfg_cell_req
*                  rrc_rrm_cell_setup_req_t        *p_cfg_cell_param
*                  csc_cell_ctx_t          *p_cell_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function check and ASN.1 encode MIB, SIB1 and SIs.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid and MIB and SIs successfully encode
*                  RRC_FAILURE -   if data are invalid, if there are not enough memory,
*                  if there are encoding problem
*
******************************************************************************/
rrc_return_et bch_m_mib_sib1_sis
(
    rrc_csc_llim_config_cell_req_t  *p_llim_cfg_cell_req,   /* out message */
    rrc_rrm_cell_setup_req_t        *p_cfg_cell_param,       /* in message */
    csc_cell_ctx_t          *p_cell_ctx
)
{
    rrc_return_et   ret_val = RRC_FAILURE;
    OSCTXT          asn1_ctx;

    SystemInformationBlockType1 asn1_sib1;
    asn1_sibs_t                 asn1_sibs;
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list = PNULL;

    RRC_CSC_UT_TRACE_ENTER();
    if (p_cfg_cell_param->cell_config_param.mac_config.bitmask &
            RRC_RRM_SI_TRANSMISSION_INFO_PRESENT)
    {
        p_rrc_si_transmission_info_list = &p_cfg_cell_param->cell_config_param.
                mac_config.si_transmission_info;
    }

    /* Precondition - we should be able to init ASN.1 context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
        return RRC_FAILURE;
    }

    /*cr_398 change*/
    if ((CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG & 
        p_cfg_cell_param->presence_bitmask) && 
        (SIB_3_THRESH_SERVING_LOW_Q_R9_PRESENCE_FLAG &
        p_cfg_cell_param->sib_type_3_Info.presence_bitmask))
    {
        p_cell_ctx->sib3_thresh_serving_low_q_flag = RRC_TRUE;
    }
    if (RRC_FAILURE == bch_m_check_and_convert_sib1(&asn1_ctx,
                                                    &asn1_sib1,
                                                    &p_cfg_cell_param->
                                                      sib_type_1_Info,
                                                    p_cfg_cell_param->
                                                        presence_bitmask, p_cell_ctx))
    {
        ret_val = RRC_FAILURE;
    }
    else if (RRC_FAILURE == check_sis_scheduling(&p_cfg_cell_param->
      sib_type_1_Info))
    {
        RRC_CSC_TRACE(RRC_WARNING, "SIs scheduling is incorrect");
        ret_val = RRC_FAILURE;
    } 
    /*** TDD changes start ***/
    else if (RRC_FAILURE == bch_m_check_and_convert_sib2(&asn1_ctx, &asn1_sibs.
        asn1_sib2, &p_cfg_cell_param->sib_type_2_Info,
        &p_cfg_cell_param->sib_type_1_Info))
    {
        ret_val = RRC_FAILURE;
    }
    /*** TDD changes stop ***/
    else if (   (p_cfg_cell_param->presence_bitmask &
        CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG) &&
                (RRC_FAILURE == bch_m_check_and_convert_sib3(
                                                   &asn1_sibs.asn1_sib3, 
                                                   &p_cfg_cell_param->
                                                   sib_type_3_Info)))
    {
        ret_val = RRC_FAILURE;
    }
    else if (   (p_cfg_cell_param->presence_bitmask &
        CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG) &&
                (RRC_FAILURE == bch_m_check_and_convert_sib4(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib4, 
                                                   &p_cfg_cell_param->
                                                   sib_type_4_Info)))
    {
        ret_val = RRC_FAILURE;
    }
    else if (   (p_cfg_cell_param->presence_bitmask &
        CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG) &&
                (RRC_FAILURE == bch_m_check_and_convert_sib5(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib5, 
                                                   &p_cfg_cell_param->
                                                   sib_type_5_Info,
                                                   p_cell_ctx)))
    {
        ret_val = RRC_FAILURE;
    }
    else if (   (p_cfg_cell_param->presence_bitmask &
        CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG) &&
                (RRC_FAILURE == bch_m_check_and_convert_sib6(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib6, 
                                                   &p_cfg_cell_param->
                                                   sib_type_6_Info,
                                                   p_cell_ctx)))
    {
        ret_val = RRC_FAILURE;
    }
    else if (   (p_cfg_cell_param->presence_bitmask &
        CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG) &&
                (RRC_FAILURE == bch_m_check_and_convert_sib7(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib7, 
                                                   &p_cfg_cell_param->
                                                   sib_type_7_Info)))
    {
        ret_val = RRC_FAILURE;
    }
    else if (   (p_cfg_cell_param->presence_bitmask &
        CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG) &&
                (RRC_FAILURE == bch_m_check_and_convert_sib8(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib8, 
                                                   &p_cfg_cell_param->
                                                   sib_type_8_Info,p_cell_ctx,1)))
    {
        ret_val = RRC_FAILURE;
    }

    else if (   (p_cfg_cell_param->presence_bitmask &
        CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG) &&
                (RRC_FAILURE == bch_m_check_and_convert_sib9(
                                                   &asn1_sibs.asn1_sib9, 
                                                   &p_cfg_cell_param->
                                                   sib_type_9_Info)))
    {
        ret_val = RRC_FAILURE;
    }
#ifdef LTE_EMBMS_SUPPORTED
    else if (   (p_cfg_cell_param->presence_bitmask &
        CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG) &&
                (RRC_FAILURE == bch_m_check_and_convert_sib13(
                                                   &asn1_ctx,
                                                   &asn1_sibs.asn1_sib13, 
                                                   &p_cfg_cell_param->
                                                   sib_type_13_Info)))
    {
        ret_val = RRC_FAILURE;
    }
#else
    else if (RRC_FAILURE == encode_mib(&asn1_ctx, p_llim_cfg_cell_req,
        &p_cfg_cell_param->mib_info,p_cell_ctx))
    {
        ret_val = RRC_FAILURE;
    }
#endif
    else if (RRC_FAILURE == encode_sib1(&asn1_ctx, &p_llim_cfg_cell_req->
        sib1_msg_req, &asn1_sib1, p_cell_ctx))
    {
        ret_val = RRC_FAILURE;
    }
    else if (RRC_FAILURE == encode_sis(&asn1_ctx,
                                       &p_llim_cfg_cell_req->si_msg_req,
                                       &asn1_sibs,
                                       &p_cfg_cell_param->sib_type_1_Info.
                                        scheduling_info_list,
                                       p_cell_ctx,
                                       p_cfg_cell_param->sib_type_1_Info.
                                            si_window_length,
                                       PNULL,
                                       p_rrc_si_transmission_info_list 
                                      /* SPR 17946 Fix Start */
                                       #ifdef LTE_EMBMS_SUPPORTED
                                       ,&(p_llim_cfg_cell_req->llim_cell_param.
                                       cell_config_param.mac_config.sib13_scheduling_info)
                                       #endif
                                        ))
                                      /* SPR 17946 Fix End */
    {
        ret_val = RRC_FAILURE;
    }
    else
    {
        ret_val = RRC_SUCCESS;
    }
    rtFreeContext(&asn1_ctx);

    RRC_CSC_UT_TRACE_EXIT();
    return ret_val;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_sib2_to_sib6_sis_recfg
*
*   DESCRIPTION:
*       This function ASN.1 encode SIs - SIB2, SIB3, SIB4, SIB5, SIB6.
*
*   RETURNS:
*       RRC_SUCCESS - if all SIB2 to SIB 6 are encoded successfully.
*       RRC_FAILURE -   if data are invalid, if there are not enough memory,
*                       if there are encoding problem
*
******************************************************************************/
rrc_return_et bch_m_sib2_to_sib6_sis_recfg
(
    OSCTXT          		*p_asn1_ctx,
    rrc_rrm_cell_reconfig_req_t	*p_recfg_cell_param,
    asn1_sibs_t                 *p_asn1_sibs,
    sib_type_1_Info_t           *p_sib1_info,
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
)
{
    RRC_CSC_UT_TRACE_ENTER();

    if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                CELL_RECONFIG_REQ_API_SIB_2_INFO_PRESENCE_FLAG) )
    {
        if (RRC_FAILURE == bch_m_check_and_convert_sib2(p_asn1_ctx, &p_asn1_sibs->
                    asn1_sib2, &p_recfg_cell_param->broadcast_info.sib_type_2_Info,
                    p_sib1_info))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB2 from Reconfig Req");
            return RRC_FAILURE;
        }
        /* eMTC changes stop */
    }
    else
    {
        /* Cov_122412_Fix_Start */
        if (RRC_FAILURE == bch_m_check_and_convert_sib2(p_asn1_ctx, &p_asn1_sibs->
                    asn1_sib2, 
                    &p_cell_ctx->cfg_cell_param.sib_type_2_Info,
                    p_sib1_info))
        {
            RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB2");
            return RRC_FAILURE;
        }
        /* Cov_122412_Fix_End */
    }
     /*** TDD changes stop ***/
    if ( CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG) )
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib3(&p_asn1_sibs->
                        asn1_sib3, &p_recfg_cell_param->broadcast_info.sib_type_3_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB3 from Reconfig Req");
                return RRC_FAILURE;
            }
        }
        else if (p_cell_ctx->cfg_cell_param.presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib3(&p_asn1_sibs->
                        asn1_sib3, 
                        &p_cell_ctx->cfg_cell_param.sib_type_3_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB3");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB3 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG) )
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib4(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib4, &p_recfg_cell_param->broadcast_info.sib_type_4_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB4 from Reconfig Req");
                return RRC_FAILURE;
            }
        }
        else if (p_cell_ctx->cfg_cell_param.presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib4(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib4, 
                        &p_cell_ctx->cfg_cell_param.sib_type_4_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB4");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB4 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG) )
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib5(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib5,
                        &p_recfg_cell_param->broadcast_info.sib_type_5_Info,p_cell_ctx))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB5 from Reconfig Req");
                return RRC_FAILURE;
            }
        }
        else if (p_cell_ctx->cfg_cell_param.presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib5(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib5, 
                        &p_cell_ctx->cfg_cell_param.sib_type_5_Info,p_cell_ctx))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB5");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB5 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG) )
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib6(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib6,
                        &p_recfg_cell_param->broadcast_info.sib_type_6_Info,p_cell_ctx))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB6 from Reconfig Req");
                return RRC_FAILURE;
            }
        }
        else if (p_cell_ctx->cfg_cell_param.presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib6(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib6, 
                        &p_cell_ctx->cfg_cell_param.sib_type_6_Info,p_cell_ctx))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB6");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB6 Info is not available");
                return RRC_FAILURE;
        }
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_sib7_to_sib9_sis_recfg
*
*   DESCRIPTION:
*       This function ASN.1 encode SIs - SIB7, SIB8, SIB9.
*
*   RETURNS:
*       RRC_SUCCESS - if all SIB7 to SIB 9 are encoded successfully.
*       RRC_FAILURE -   if data are invalid, if there are not enough memory,
*                       if there are encoding problem
*
******************************************************************************/
rrc_return_et bch_m_sib7_to_sib9_sis_recfg
(
    OSCTXT          		*p_asn1_ctx,
    rrc_rrm_cell_reconfig_req_t	*p_recfg_cell_param,
    asn1_sibs_t                 *p_asn1_sibs,
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
)
{
    RRC_CSC_UT_TRACE_ENTER();

    if ( CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG) )
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib7(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib7, &p_recfg_cell_param->broadcast_info.sib_type_7_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB7 from Reconfig Req");
                return RRC_FAILURE;
            }
        }
        else if (p_cell_ctx->cfg_cell_param.presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib7(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib7, 
                        &p_cell_ctx->cfg_cell_param.sib_type_7_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB7");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB7 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG) )
        {
            if(p_cell_ctx->sib_modify_source == RRM_TRIGGERED_SIB8_NO_MODIFY)
            {
                if (RRC_FAILURE == bch_m_check_and_convert_sib8(p_asn1_ctx, &p_asn1_sibs->
                            asn1_sib8, &p_recfg_cell_param->broadcast_info.sib_type_8_Info,                                                
                            p_cell_ctx, 1))
                {
                    RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB8 from Reconfig Req");
                    return RRC_FAILURE;
                }
            }
            else
            {
                RRC_CSC_TRACE(RRC_WARNING, 
                "RRM_TRIGGERED_SIB8_MODIFY to be handle later");
            }
        }
        else if (p_cell_ctx->cfg_cell_param.presence_bitmask &
                CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
        {
            if(p_cell_ctx->sib_modify_source == RRM_TRIGGERED_SIB8_MODIFY)
            {
                RRC_CSC_TRACE(RRC_WARNING, 
                        "RRM_TRIGGERED_SIB8_MODIFY to be handle later");
            }
            else
            {
                if (RRC_FAILURE == bch_m_check_and_convert_sib8(p_asn1_ctx, &p_asn1_sibs->
                            asn1_sib8, 
                            &p_cell_ctx->cfg_cell_param.sib_type_8_Info, p_cell_ctx, 1))
                {
                    RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB8");
                    return RRC_FAILURE;
                }
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB8 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG) )
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib9(&p_asn1_sibs->
                        asn1_sib9, &p_recfg_cell_param->broadcast_info.sib_type_9_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB9 from Reconfig Req");
                return RRC_FAILURE;
            }
        }
        else if (p_cell_ctx->cfg_cell_param.presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib9(&p_asn1_sibs->
                        asn1_sib9, 
                        &p_cell_ctx->cfg_cell_param.sib_type_9_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB9");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB9 Info is not available");
                return RRC_FAILURE;
        }
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

#ifdef LTE_EMBMS_SUPPORTED
/******************************************************************************
*   FUNCTION NAME: bch_m_sib13_sis_recfg
*
*   DESCRIPTION:
*       This function ASN.1 encode SIs - SIB13
*
*   RETURNS:
*       RRC_SUCCESS - if SIB13 encoded successfully.
*       RRC_FAILURE -   if data are invalid, if there are not enough memory,
*                       if there are encoding problem
*
******************************************************************************/
rrc_return_et bch_m_sib13_sis_recfg
(
    OSCTXT          		*p_asn1_ctx,
    rrc_rrm_cell_reconfig_req_t	*p_recfg_cell_param,
    asn1_sibs_t                 *p_asn1_sibs,
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
)
{
    RRC_CSC_UT_TRACE_ENTER();
    /*To Do: Check Value of below macro, it should be 0x2000*/
    if ( CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if ( (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG) )
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib13(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib13,
                        &p_recfg_cell_param->broadcast_info.sib_type_13_info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB13 from Reconfig Req");
                return RRC_FAILURE;
            }
        }
        else if (p_cell_ctx->cfg_cell_param.presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib13(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib13, 
                        &p_cell_ctx->cfg_cell_param.sib_type_13_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB13");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB13 Info is not available");
                return RRC_FAILURE;
        }
    }

    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}
#endif

/******************************************************************************
*   FUNCTION NAME: bch_m_mib_sib1_sis_recfg
*   INPUT        : rrc_csc_llim_reconfig_cell_req_t  *p_llim_recfg_cell_req
*                  rrc_rrm_cell_reconfig_req_t        *p_recfg_cell_param
*                  csc_cell_ctx_t              *p_cell_ctx
*   OUTPUT       : none               
*   DESCRIPTION  : This function check and ASN.1 encode MIB, SIB1 and SIs.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid and MIB and SIs successfully encode
*                  RRC_FAILURE -   if data are invalid, if there are not enough memory,
*                  if there are encoding problem
*
******************************************************************************/
rrc_return_et bch_m_mib_sib1_sis_recfg
(
    rrc_csc_llim_reconfig_cell_req_t  *p_llim_recfg_cell_req,   /* out message */
    rrc_rrm_cell_reconfig_req_t        *p_recfg_cell_param,       /* in message */
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
)
{
    rrc_return_et   ret_val = RRC_SUCCESS;
    OSCTXT          asn1_ctx;

    SystemInformationBlockType1 asn1_sib1;
    asn1_sibs_t                 asn1_sibs;
    sib_type_11_t               *sib_type_11 = PNULL;
    rrc_bitmask_t               presence_bitmask = RRC_NULL;
    sib_type_1_Info_t           *p_sib1_info = PNULL;
    
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list = PNULL;
    

    RRC_CSC_UT_TRACE_ENTER();

    /* Precondition - we should be able to init ASN.1 context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
        return RRC_FAILURE;
    }
    /* SPR 23797 fix start */
    /* Encoding of SIB2-br has been moved above encoding fo SIB1-br, as TBsize of 
     * SI messages is sent in SIB1-br so that should be calculated before encoding 
     * of SIB1-br */
    if(PNULL != p_cell_ctx) /* Coverity Fix 147602 start*/
    {
        /*cr_398 change*/
        if ((CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG &
                    p_recfg_cell_param->broadcast_info.presence_bitmask) && 
                (SIB_3_THRESH_SERVING_LOW_Q_R9_PRESENCE_FLAG & 
                 p_recfg_cell_param->broadcast_info.sib_type_3_Info.presence_bitmask))
        {
            p_cell_ctx->sib3_thresh_serving_low_q_flag = RRC_TRUE;
        }

        /*coverity_64270_fix_start*/
        if (PNULL != p_cell_ctx->p_temp_si_schdl_info)
        {
            if (PNULL != p_cell_ctx->pws_broadcast_config)
            {
                presence_bitmask = (rrc_bitmask_t)(p_recfg_cell_param->broadcast_info.
                        presence_bitmask |
                        p_cell_ctx->pws_broadcast_config->bitmask |
                        p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask);
            }
            else
            {
                presence_bitmask = p_recfg_cell_param->broadcast_info.presence_bitmask |
                    p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask;
            }
        }
        /*coverity_64270_fix_stop*/
        /* eMTC changes stop */
        if (p_recfg_cell_param->broadcast_info.presence_bitmask &
                CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)
        {
            p_sib1_info = &(p_recfg_cell_param->broadcast_info.sib_type_1_Info);
            if(RRC_FAILURE == bch_m_check_and_convert_sib1(&asn1_ctx,
                        &asn1_sib1,
                        &p_recfg_cell_param->broadcast_info.sib_type_1_Info,
                        presence_bitmask,
                        p_cell_ctx))
            {
                rtFreeContext(&asn1_ctx);
                return RRC_FAILURE;
            }
            else if (RRC_FAILURE == check_sis_scheduling(
                        &p_recfg_cell_param->broadcast_info.
                        sib_type_1_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "SIs scheduling is incorrect");
                rtFreeContext(&asn1_ctx);
                return RRC_FAILURE;
            }
        }
        else 
        {
            p_sib1_info = &(p_cell_ctx->cfg_cell_param.sib_type_1_Info);
            if (RRC_FAILURE == bch_m_check_and_convert_sib1(&asn1_ctx,
                        &asn1_sib1,
                        &p_cell_ctx->cfg_cell_param.
                        sib_type_1_Info,
                        presence_bitmask,
                        p_cell_ctx))
            {
                rtFreeContext(&asn1_ctx);
                return RRC_FAILURE;
            }
            else if (RRC_FAILURE == check_sis_scheduling(&p_cell_ctx->cfg_cell_param.
                        sib_type_1_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "SIs scheduling is incorrect");
                rtFreeContext(&asn1_ctx);
                return RRC_FAILURE;
            }
        }
        if (RRC_FAILURE == bch_m_sib2_to_sib6_sis_recfg(&asn1_ctx, 
                    p_recfg_cell_param, &asn1_sibs, p_sib1_info, p_cell_ctx))
        {
            rtFreeContext(&asn1_ctx);
            return RRC_FAILURE;
        }

        if (RRC_FAILURE == bch_m_sib7_to_sib9_sis_recfg(&asn1_ctx, 
                    p_recfg_cell_param, &asn1_sibs, p_cell_ctx))
        {
            rtFreeContext(&asn1_ctx);
            return RRC_FAILURE;
        }
        /* eMTC changes stop */
#ifdef LTE_EMBMS_SUPPORTED
        if (RRC_FAILURE == bch_m_sib13_sis_recfg(&asn1_ctx, 
                    p_recfg_cell_param, &asn1_sibs, p_cell_ctx))
        {
            rtFreeContext(&asn1_ctx);
            return RRC_FAILURE;
        }
#endif
        if ( PNULL != p_cell_ctx->pws_broadcast_config)
        {
            if (((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_broadcast_config->
                        bitmask ) && (RRC_FAILURE == bch_m_check_and_convert_sib10(
                                &asn1_sibs.asn1_sib10,
                                &p_cell_ctx->pws_broadcast_config->sib_type_10_Info)))
            {
                ret_val = RRC_FAILURE;
            }
        }

        if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_MIB_INFO_PRESENCE_FLAG) &&
                (RRC_FAILURE == encode_mib_recfg(&asn1_ctx, p_llim_recfg_cell_req,
                                                 &p_recfg_cell_param->broadcast_info.mib_info,p_cell_ctx)))
        {
            ret_val = RRC_FAILURE;
        }
        else if (   (p_recfg_cell_param->broadcast_info.presence_bitmask &
                    CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG) )
        {
            if(RRC_FAILURE == encode_sib1(&asn1_ctx, &p_llim_recfg_cell_req->
                        sib1_msg_req, &asn1_sib1, p_cell_ctx))  
            {
                ret_val = RRC_FAILURE;
            }
            else
            {
                p_llim_recfg_cell_req->bitmask |= RRC_CSC_SIB_TYPE_1_MSG_PRESENT;
            }
        }
        if ((p_recfg_cell_param->presence_bitmask & 
                    CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG) &&
                (p_recfg_cell_param->non_broadcast_info.presence_bitmask &
                 CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG) &&
                (p_recfg_cell_param->non_broadcast_info.mac_reconfig_params.bitmask &
                 RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT) )
        {
            p_rrc_si_transmission_info_list =
                &p_recfg_cell_param->non_broadcast_info.
                mac_reconfig_params.si_transmission_info;
        }
        if(RRC_SUCCESS == ret_val)
        {
            /* eMTC changes stop */
            if(SI_MAPPING_CHANGED == p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag)
            { 
                if (PNULL != p_cell_ctx->etws_info)
                {
                    sib_type_11 = &p_cell_ctx->etws_info->sib11_info;
                }
                /* SI Mapping Changed */    
                if (RRC_FAILURE == encode_sis( &asn1_ctx,
                            &p_llim_recfg_cell_req->si_msg_req,
                            &asn1_sibs,
                            &p_sib1_info->scheduling_info_list,
                            p_cell_ctx,
                            p_sib1_info->si_window_length,
                            sib_type_11,
                            p_rrc_si_transmission_info_list
                            /* SPR 17946 Fix Start */
#ifdef LTE_EMBMS_SUPPORTED
                            ,&(p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib13_scheduling_info)
#endif
                            ))
                    /* SPR 17946 Fix End */
                {
                    ret_val = RRC_FAILURE;
                }
                else
                {
                    p_llim_recfg_cell_req->bitmask |= RRC_CSC_SI_MSG_REQ_PRESENT;
                }
            }

            else 
            {
                /* SI Mapping NOT Changed */
                if(p_recfg_cell_param->broadcast_info.presence_bitmask &
                        CELL_RECONFIG_REQ_API_SIB_1_INFO_PRESENCE_FLAG)
                {
                    if (PNULL != p_cell_ctx->etws_info)
                    {
                        sib_type_11 = &p_cell_ctx->etws_info->sib11_info;
                    }
                    if (RRC_FAILURE == encode_sis_with_new_si( &asn1_ctx,
                                &p_llim_recfg_cell_req->si_msg_req,
                                &asn1_sibs,
                                &p_cell_ctx->p_temp_si_schdl_info->new_schdl_info,
                                p_cell_ctx,
                                p_sib1_info->si_window_length,
                                sib_type_11,
                                p_rrc_si_transmission_info_list
#ifdef LTE_EMBMS_SUPPORTED
                                ,&(p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib13_scheduling_info)
#endif
                                ))
                    {
                        ret_val = RRC_FAILURE;
                    }
                    else
                    {
                        p_llim_recfg_cell_req->bitmask |= RRC_CSC_SI_MSG_REQ_PRESENT;
                    }
                }
                else
                {
                    if (PNULL != p_cell_ctx->pws_broadcast_config)
                    {
                        sib_type_11 = &p_cell_ctx->pws_broadcast_config->sib11_info;
                    }
                    if (PNULL != p_cell_ctx->etws_info)
                    {
                        sib_type_11 = &p_cell_ctx->etws_info->sib11_info;
                    }
                    if (RRC_FAILURE == encode_sis_with_new_si( &asn1_ctx,
                                &p_llim_recfg_cell_req->si_msg_req,
                                &asn1_sibs,
                                &p_cell_ctx->p_temp_si_schdl_info->new_schdl_info,
                                p_cell_ctx,
                                p_sib1_info->si_window_length,
                                sib_type_11,
                                p_rrc_si_transmission_info_list
#ifdef LTE_EMBMS_SUPPORTED
                                ,&(p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib13_scheduling_info)
#endif
                                ))
                    {
                        ret_val = RRC_FAILURE;
                    }
                    else
                    {
                        p_llim_recfg_cell_req->bitmask |= RRC_CSC_SI_MSG_REQ_PRESENT;
                    }
                }
            }
            /* SPR 22979 fix start */
            if ( PNULL != p_cell_ctx->pws_broadcast_config)
            {
                if (((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_broadcast_config->
                            bitmask ) && (RRC_FAILURE == bch_m_check_and_convert_sib10(
                                    &asn1_sibs.asn1_sib10,
                                    &p_cell_ctx->pws_broadcast_config->sib_type_10_Info)))
                {
                    ret_val = RRC_FAILURE;
                } 
            }

            if ((p_recfg_cell_param->presence_bitmask & 
                        CELL_RECONFIG_REQ_API_NON_BROADCAST_INFO_PRESENCE_FLAG) &&
                    (p_recfg_cell_param->non_broadcast_info.presence_bitmask &
                     CELL_RECONFIG_REQ_API_MAC_RECONFIG_INFO_PRESENCE_FLAG) &&
                    (p_recfg_cell_param->non_broadcast_info.mac_reconfig_params.bitmask &
                     RRC_RRM_RECONFIG_SI_TRANSMISSION_INFO_PRESENT) )
            {
                p_rrc_si_transmission_info_list =
                    &p_recfg_cell_param->non_broadcast_info.
                    mac_reconfig_params.si_transmission_info;
            }
            /* SPR 22979 fix stop */
        }
    }
    /* SPR 23797 fix end */
    rtFreeContext(&asn1_ctx);

    RRC_CSC_UT_TRACE_EXIT();
 
    return ret_val;
}

/******************************************************************************
*   FUNCTION NAME: bch_m_update_si_transmission_info_sib4_to_sib9_recfg
*
*   DESCRIPTION:
*       This function ASN.1 encoding of SIB 4 to SIB 9
*
*   RETURNS:
*       RRC_SUCCESS - if SIB 4 to SIB 9 are successfully encoded.
*       RRC_FAILURE -   if data are invalid, if there are not enough memory,
*                       if there are encoding problem
*
******************************************************************************/
rrc_return_et bch_m_update_si_transmission_info_sib4_to_sib9_recfg
(
    OSCTXT          *p_asn1_ctx,
    asn1_sibs_t     *p_asn1_sibs,
    rrc_rrm_cell_setup_req_t          *p_cfg_cell_param,
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
)
{
    RRC_CSC_UT_TRACE_ENTER();
    if ( CELL_RECONFIG_REQ_API_SIB_4_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (p_cfg_cell_param->presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib4(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib4, 
                        &p_cfg_cell_param->sib_type_4_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB4");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB4 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_5_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (p_cfg_cell_param->presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib5(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib5, 
                        &p_cfg_cell_param->sib_type_5_Info,p_cell_ctx))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB5");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB5 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_6_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (p_cfg_cell_param->presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib6(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib6, 
                        &p_cfg_cell_param->sib_type_6_Info,p_cell_ctx))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB6");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB6 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_7_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (p_cfg_cell_param->presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib7(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib7, 
                        &p_cfg_cell_param->sib_type_7_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB7");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB7 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_8_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (p_cfg_cell_param->presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib8(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib8, 
                        &p_cfg_cell_param->sib_type_8_Info, p_cell_ctx, 1))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB8");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB8 Info is not available");
                return RRC_FAILURE;
        }
    }
    if ( CELL_RECONFIG_REQ_API_SIB_9_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (p_cfg_cell_param->presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib9(&p_asn1_sibs->
                        asn1_sib9, 
                        &p_cfg_cell_param->sib_type_9_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB9");
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB9 Info is not available");
                return RRC_FAILURE;
        }
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}
#ifdef LTE_EMBMS_SUPPORTED
/******************************************************************************
*   FUNCTION NAME: bch_m_update_si_transmission_info_sib13_recfg
*
*   DESCRIPTION:
*       This function ASN.1 encoding of SIB 13
*
*   RETURNS:
*       RRC_SUCCESS - if SIB 13 successfully encoded.
*       RRC_FAILURE -   if data are invalid, if there are not enough memory,
*                       if there are encoding problem
*
******************************************************************************/
rrc_return_et bch_m_update_si_transmission_info_sib13_recfg
(
    OSCTXT          *p_asn1_ctx,
    asn1_sibs_t     *p_asn1_sibs,
    rrc_rrm_cell_setup_req_t          *p_cfg_cell_param,
    csc_cell_ctx_t              *p_cell_ctx    /* cell context */   
)
{
    RRC_CSC_UT_TRACE_ENTER();
    if ( CELL_RECONFIG_REQ_API_SIB_13_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (p_cfg_cell_param->presence_bitmask &
                CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib13(p_asn1_ctx, &p_asn1_sibs->
                        asn1_sib13, 
                        &p_cfg_cell_param->sib_type_13_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB13");
                return RRC_FAILURE;
            }
        }
        else
        {
            RRC_CSC_TRACE(RRC_WARNING, "SIB13 Info is not available");
            return RRC_FAILURE;
        }
    }
    RRC_CSC_UT_TRACE_EXIT();
    return RRC_SUCCESS;
}

#endif

/******************************************************************************
*   FUNCTION NAME: bch_m_update_si_transmission_info_sis_recfg
*   INPUT        : rrc_csc_llim_reconfig_cell_req_t  *p_llim_recfg_cell_req
*                  rrc_rrm_cell_setup_req_t          *p_cfg_cell_param
*                  csc_cell_ctx_t              *p_cell_ctx
*                  rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
*   OUTPUT       : none               
*   DESCRIPTION  : This function check and ASN.1 encode MIB, SIB1 and SIs.
*
*   RETURNS      : RRC_SUCCESS - if all data are valid and MIB and SIs successfully encode
*                  RRC_FAILURE -   if data are invalid, if there are not enough memory,
*                  if there are encoding problem
*
******************************************************************************/
rrc_return_et bch_m_update_si_transmission_info_sis_recfg
(
    rrc_csc_llim_reconfig_cell_req_t  *p_llim_recfg_cell_req,   /* out message */
    rrc_rrm_cell_setup_req_t          *p_cfg_cell_param,       /* in message */
    csc_cell_ctx_t              *p_cell_ctx,    /* cell context */   
    rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
)
{
    rrc_return_et   ret_val = RRC_SUCCESS;
    OSCTXT          asn1_ctx;

    SystemInformationBlockType1 asn1_sib1;
    asn1_sibs_t                 asn1_sibs;
    sib_type_11_t               *sib_type_11 = PNULL;
    rrc_bitmask_t               presence_bitmask = RRC_NULL;
    sib_type_1_Info_t           *p_sib1_info = PNULL;
    
    
    RRC_CSC_UT_TRACE_ENTER();

    /* Precondition - we should be able to init ASN.1 context */
    if (RT_OK != rtInitContext(&asn1_ctx))
    {
        RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
        return RRC_FAILURE;
    }
    /*cr_398 change*/
    if ((CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG & 
        p_cfg_cell_param->presence_bitmask) && 
        (SIB_3_THRESH_SERVING_LOW_Q_R9_PRESENCE_FLAG &
        p_cfg_cell_param->sib_type_3_Info.presence_bitmask))
    {
        p_cell_ctx->sib3_thresh_serving_low_q_flag = RRC_TRUE;
    }

    if (PNULL != p_cell_ctx->pws_broadcast_config)
    {
       presence_bitmask = (rrc_bitmask_t)(p_cfg_cell_param->
                            presence_bitmask |
                            p_cell_ctx->pws_broadcast_config->bitmask |
                            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask);
    }
    else
    {
       presence_bitmask = p_cfg_cell_param->presence_bitmask |
                           p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask;
    }

    p_sib1_info = &(p_cfg_cell_param->sib_type_1_Info);
    if (RRC_FAILURE == bch_m_check_and_convert_sib1(&asn1_ctx,
                &asn1_sib1,
                &p_cfg_cell_param->
                sib_type_1_Info,
                presence_bitmask,
                p_cell_ctx))
    {
        rtFreeContext(&asn1_ctx);
        return RRC_FAILURE;
    }
    else if (RRC_FAILURE == check_sis_scheduling(&p_cfg_cell_param->
                sib_type_1_Info))
    {
        RRC_CSC_TRACE(RRC_WARNING, "SIs scheduling is incorrect");
        rtFreeContext(&asn1_ctx);
        return RRC_FAILURE;
    } 
    /*** TDD changes start ***/
    if (RRC_FAILURE == bch_m_check_and_convert_sib2(&asn1_ctx, &asn1_sibs.
                asn1_sib2, 
                &p_cfg_cell_param->sib_type_2_Info,
                p_sib1_info))
    {
        RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB2");
        rtFreeContext(&asn1_ctx);
        return RRC_FAILURE;
    }
    /*** TDD changes stop ***/
    if ( CELL_RECONFIG_REQ_API_SIB_3_INFO_PRESENCE_FLAG &
            p_cell_ctx->p_temp_si_schdl_info->curr_schdl_sibs_bitmask )
    {
        if (p_cfg_cell_param->presence_bitmask &
                   CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG)
        {
            if (RRC_FAILURE == bch_m_check_and_convert_sib3(&asn1_sibs.
                        asn1_sib3, 
                        &p_cfg_cell_param->sib_type_3_Info))
            {
                RRC_CSC_TRACE(RRC_WARNING, "Unable to fill SIB3");
                rtFreeContext(&asn1_ctx);
                return RRC_FAILURE;
            }
        }
        else
        {
                RRC_CSC_TRACE(RRC_WARNING, "SIB3 Info is not available");
                rtFreeContext(&asn1_ctx);
                return RRC_FAILURE;
        }
    }

    if (RRC_FAILURE == bch_m_update_si_transmission_info_sib4_to_sib9_recfg(&asn1_ctx, &asn1_sibs,
                p_cfg_cell_param, p_cell_ctx))
    {
        rtFreeContext(&asn1_ctx);
        return RRC_FAILURE;
    }
#ifdef LTE_EMBMS_SUPPORTED
    if (RRC_FAILURE == bch_m_update_si_transmission_info_sib13_recfg(&asn1_ctx, &asn1_sibs,
                p_cfg_cell_param, p_cell_ctx))
    {
        rtFreeContext(&asn1_ctx);
        return RRC_FAILURE;
    }
#endif

    if ( PNULL != p_cell_ctx->pws_broadcast_config)
    {
        if (((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_broadcast_config->
                   bitmask ) && (RRC_FAILURE == bch_m_check_and_convert_sib10(
                      &asn1_sibs.asn1_sib10,
                      &p_cell_ctx->pws_broadcast_config->sib_type_10_Info)))
        {
            ret_val = RRC_FAILURE;
        }
    }

    if (RRC_FAILURE == encode_mib_recfg(&asn1_ctx, p_llim_recfg_cell_req,
            &p_cfg_cell_param->mib_info,p_cell_ctx))
    {
        ret_val = RRC_FAILURE;
    }
    else if(RRC_FAILURE == encode_sib1(&asn1_ctx, &p_llim_recfg_cell_req->
                sib1_msg_req, &asn1_sib1, p_cell_ctx))
    {
        ret_val = RRC_FAILURE;

    }
    else
    {
        p_llim_recfg_cell_req->bitmask |= RRC_CSC_SIB_TYPE_1_MSG_PRESENT;
    }
    
    if(RRC_SUCCESS == ret_val)
    {

        if(SI_MAPPING_NOT_CHANGED == p_cell_ctx->p_temp_si_schdl_info->si_mapping_flag)
        { 
            if (PNULL != p_cell_ctx->pws_broadcast_config)
            {
                sib_type_11 = &p_cell_ctx->pws_broadcast_config->sib11_info;
            }
            else if (PNULL != p_cell_ctx->etws_info)
            {
                sib_type_11 = &p_cell_ctx->etws_info->sib11_info;
            }
            if (RRC_FAILURE == encode_sis_with_new_si( &asn1_ctx,
                        &p_llim_recfg_cell_req->si_msg_req,
                        &asn1_sibs,
                        &p_cell_ctx->p_temp_si_schdl_info->new_schdl_info,
                        p_cell_ctx,
                        p_sib1_info->si_window_length,
                        sib_type_11,
                        p_rrc_si_transmission_info_list
#ifdef LTE_EMBMS_SUPPORTED
                        ,&(p_llim_recfg_cell_req->cell_reconfig.broadcast_info.sib13_scheduling_info)
#endif
                        ))
            {
                ret_val = RRC_FAILURE;
            }
            else
            {
                p_llim_recfg_cell_req->bitmask |= RRC_CSC_SI_MSG_REQ_PRESENT;
            }
        }
    }
   rtFreeContext(&asn1_ctx);

    RRC_CSC_UT_TRACE_EXIT();
 
    return ret_val;
}


/*****************************************************************************
 * Function Name  : update_si_scheduling_info 
 * Inputs         : csc_cell_ctx_t *p_cell_ctx
 * Outputs        : None
 * Returns        : None
 * Description    : This function updates s1 scheduling info
 ********************************************************************************/
void update_si_scheduling_info(csc_cell_ctx_t *p_cell_ctx)
{

    RRC_CSC_UT_TRACE_ENTER();
    RRC_ASSERT(PNULL != p_cell_ctx);
    
  l3_memcpy_wrapper(&p_cell_ctx->si_schdl_info,p_cell_ctx->
      p_temp_si_schdl_info, sizeof(si_schdl_info_t));

    rrc_mem_free(p_cell_ctx->p_temp_si_schdl_info);
    p_cell_ctx->p_temp_si_schdl_info = PNULL;
    
}

/******************************************************************************
 *   FUNCTION NAME: bch_m_sib1_sis_pws
 *   INPUT        : rrc_csc_llim_pws_req_t  *p_llim_pws_req
 *                  scheduling_info_list_t  *p_scheduling_info_list
 *                  csc_cell_ctx_t          *p_cell_ctx
 *                  rrc_bool_et             is_kill_req
 *                  rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
 *   OUTPUT       : none
 *   DESCRIPTION  : This function check and ASN.1 encode SIB1 and SIs for write-replace
 *                  warning request received from eNB manager (via MME).
 *
 *   RETURNS      : RRC_SUCCESS - if all data are valid and MIB and SIs successfully encode
 *                  RRC_FAILURE -   if data are invalid, if there are not enough memory,
 *                  if there are encoding problem
 *
 ******************************************************************************/
rrc_return_et bch_m_sib1_sis_pws
(
 rrc_csc_llim_pws_req_t  *p_llim_pws_req,   /* out message */
 scheduling_info_list_t  *p_scheduling_info_list, /*in scheduling info */
 csc_cell_ctx_t          *p_cell_ctx,    /* cell context */   
 rrc_bool_et             is_kill_req,
 rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
 )
{
  rrc_return_et   ret_val = RRC_SUCCESS;
  OSCTXT          asn1_ctx;

  SystemInformationBlockType1 asn1_sib1;
  asn1_sibs_t                 asn1_sibs;
  sib_type_1_Info_t             sib1_info_pws ;
 sib_type_11_t               *sib_type_11 = PNULL;

  RRC_CSC_UT_TRACE_ENTER();

  /* Precondition - we should be able to init ASN.1 context */
  if (RT_OK != rtInitContext(&asn1_ctx))
  {
    RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
    return RRC_FAILURE;
  }

  sib1_info_pws = 
      p_cell_ctx->cfg_cell_param.sib_type_1_Info;

  l3_memcpy_wrapper  (&(p_cell_ctx->cfg_cell_param.sib_type_1_Info.scheduling_info_list),
               p_scheduling_info_list,
               sizeof (scheduling_info_list_t));

  if (RRC_FALSE == is_kill_req)
  {
      if ( (RRC_FAILURE == bch_m_check_and_convert_sib1_pws(&asn1_ctx,
                      &asn1_sib1,
                      &sib1_info_pws,
                      (rrc_bitmask_t)((p_cell_ctx->cfg_cell_param.presence_bitmask << 3) |
                       p_cell_ctx->pws_sibs_bitmask),
                      RRC_TRUE,
                      p_cell_ctx)))
      {
          ret_val = RRC_FAILURE;
          rtFreeContext(&asn1_ctx);
          RRC_CSC_UT_TRACE_EXIT();
          return ret_val;
      }
  }
  else
  {
      if ( (RRC_FAILURE == bch_m_check_and_convert_sib1_pws(&asn1_ctx,
                      &asn1_sib1,
                      &sib1_info_pws,
                      (rrc_bitmask_t)((p_cell_ctx->cfg_cell_param.presence_bitmask << 3) |
                       p_cell_ctx->pws_sibs_bitmask),
                      RRC_FALSE,
                      p_cell_ctx)))
      {
          ret_val = RRC_FAILURE;
          rtFreeContext(&asn1_ctx);
          RRC_CSC_UT_TRACE_EXIT();
          return ret_val;
      }
  }
  if (RRC_FAILURE == check_sis_scheduling(
        &sib1_info_pws))

  {
    RRC_CSC_TRACE(RRC_WARNING, "SIs scheduling is incorrect");
    ret_val = RRC_FAILURE;
  }
  else if (RRC_FAILURE == bch_m_check_and_convert_sib2(&asn1_ctx, &asn1_sibs.
        asn1_sib2, 
        &p_cell_ctx->cfg_cell_param.sib_type_2_Info,
        &sib1_info_pws))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib3(&asn1_sibs.asn1_sib3, 
                                                   &p_cell_ctx->
                                                   cfg_cell_param.
                                                   sib_type_3_Info)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib4(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib4, 
                                                   &p_cell_ctx->
                                                   cfg_cell_param.
                                                   sib_type_4_Info)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib5(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib5, 
                                                   &p_cell_ctx->
                                                   cfg_cell_param.
                                                   sib_type_5_Info,
                                                   p_cell_ctx)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib6(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib6, 
                                                   &p_cell_ctx->
                                                   cfg_cell_param.
                                                   sib_type_6_Info,
                                                   p_cell_ctx)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib7(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib7, 
                                                   &p_cell_ctx->
                                                   cfg_cell_param.
                                                   sib_type_7_Info)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib8(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib8, 
                                                   &p_cell_ctx->
                                                   cfg_cell_param.
                                                   sib_type_8_Info,
                                                   p_cell_ctx, 1)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib9(&asn1_sibs.asn1_sib9, 
                                                   &p_cell_ctx->
                                                   cfg_cell_param.
                                                   sib_type_9_Info)))
  {
    ret_val = RRC_FAILURE;
  }
#ifdef LTE_EMBMS_SUPPORTED
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib13(
                                                   &asn1_ctx,
                                                   &asn1_sibs.asn1_sib13, 
                                                   &p_cell_ctx->
                                                   cfg_cell_param.
                                                   sib_type_13_Info)))
  {
    ret_val = RRC_FAILURE;
  }
#endif
  if ( PNULL != p_cell_ctx->etws_info )
  {
      if (  ((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask) && 
          (RRC_FAILURE == bch_m_check_and_convert_sib10(
          &asn1_sibs.asn1_sib10,&(p_cell_ctx->etws_info->sib10_info))))
      {
          ret_val = RRC_FAILURE;
      }
  }
  if(RRC_FAILURE == encode_sib1(&asn1_ctx,
        &p_llim_pws_req->sib1_msg_req, &asn1_sib1, p_cell_ctx))  
  {
    ret_val = RRC_FAILURE;

  }
  if(RRC_SUCCESS == ret_val)
  {
    if (PNULL != p_cell_ctx->etws_info)
    {
        sib_type_11 = &p_cell_ctx->etws_info->sib11_info;
    }
    if ( RRC_FALSE == is_kill_req )
    {
        /* SI Mapping Changed */    
        if (RRC_FAILURE == encode_sis_pws( &asn1_ctx,
             &p_llim_pws_req->si_msg_req,
             &asn1_sibs,
             p_scheduling_info_list,
             p_cell_ctx,
             p_cell_ctx->cfg_cell_param.sib_type_1_Info.
             si_window_length,
             sib_type_11,
             p_rrc_si_transmission_info_list
            /* SPR 17946 Fix Start */
             #ifdef LTE_EMBMS_SUPPORTED
             ,PNULL
             #endif
             ))
            /* SPR 17946 Fix End */
        {
           ret_val = RRC_FAILURE;
        }
    }
    else
    {
       /* SI Mapping Changed */    
       if (RRC_FAILURE == encode_sis( &asn1_ctx,
             &p_llim_pws_req->si_msg_req,
             &asn1_sibs,
             p_scheduling_info_list,
             p_cell_ctx,
             p_cell_ctx->cfg_cell_param.sib_type_1_Info.
             si_window_length,
             sib_type_11,
             p_rrc_si_transmission_info_list
            /* SPR 17946 Fix Start */
             #ifdef LTE_EMBMS_SUPPORTED
             ,PNULL
             #endif
             ))
            /* SPR 17946 Fix End */
       {
          ret_val = RRC_FAILURE;
       }
    }
  }
  rtFreeContext(&asn1_ctx);


  RRC_CSC_UT_TRACE_EXIT();

  return ret_val;
}


/******************************************************************************
 *   FUNCTION NAME: bch_m_sib1_sis_updated_pws
 *   INPUT        : rrc_csc_llim_updated_pws_si_req_t  *p_llim_pws_req
 *                  csc_cell_ctx_t                     *p_cell_ctx
 *                  rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
 *   OUTPUT       : none
 *   DESCRIPTION  : This function check and ASN.1 encode SIB1 and SIs after write-replace
 *                  warning request received from eNB manager (via MME).
 *
 *   RETURNS      : RRC_SUCCESS - if all data are valid and SIs successfully encode
 *                  RRC_FAILURE -   if data are invalid, if there are not enough memory,
 *                  if there are encoding problem
 *
 ******************************************************************************/
rrc_return_et bch_m_sib1_sis_updated_pws
(
 rrc_csc_llim_updated_pws_si_req_t  *p_llim_pws_req, /* out message */
 csc_cell_ctx_t                     *p_cell_ctx,    /* cell context */   
  rrc_si_transmission_info_list_t *p_rrc_si_transmission_info_list
 )
{
  rrc_return_et   ret_val = RRC_SUCCESS;
  OSCTXT          asn1_ctx;

  SystemInformationBlockType1 asn1_sib1;
  asn1_sibs_t                 asn1_sibs;
  sib_type_11_t               *sib_type_11 = PNULL;

  RRC_CSC_UT_TRACE_ENTER();

  /* Precondition - we should be able to init ASN.1 context */
  if (RT_OK != rtInitContext(&asn1_ctx))
  {
    RRC_CSC_TRACE(RRC_ERROR, "Couldn't init ASN.1 context");
    return RRC_FAILURE;
  }

  /* Need to call function for making sib_type11 and for sib_type10 */

  if ( (RRC_FAILURE == bch_m_check_and_convert_sib1_pws(&asn1_ctx,
          &asn1_sib1,
          &p_cell_ctx->cfg_cell_param.sib_type_1_Info,
          (rrc_bitmask_t)((p_cell_ctx->cfg_cell_param.presence_bitmask << 3) |
          p_cell_ctx->pws_sibs_bitmask),
          RRC_FALSE,
          p_cell_ctx)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (RRC_FAILURE == check_sis_scheduling(
        &p_cell_ctx->cfg_cell_param.sib_type_1_Info))

  {
    RRC_CSC_TRACE(RRC_WARNING, "SIs scheduling is incorrect");
    ret_val = RRC_FAILURE;
  }
  else if (RRC_FAILURE == bch_m_check_and_convert_sib2(&asn1_ctx, &asn1_sibs.
        asn1_sib2, 
        &p_cell_ctx->cfg_cell_param.sib_type_2_Info,
        &p_cell_ctx->cfg_cell_param.sib_type_1_Info))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_3_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib3(&asn1_sibs.asn1_sib3, 
                                                   &p_cell_ctx->cfg_cell_param.
                                                   sib_type_3_Info)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_4_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib4(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib4, 
                                                   &p_cell_ctx->cfg_cell_param.
                                                   sib_type_4_Info)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_5_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib5(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib5, 
                                                   &p_cell_ctx->cfg_cell_param.
                                                   sib_type_5_Info,
                                                   p_cell_ctx)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_6_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib6(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib6,
                                                   &p_cell_ctx->cfg_cell_param.
                                                   sib_type_6_Info,
                                                   p_cell_ctx)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_7_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib7(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib7, 
                                                   &p_cell_ctx->cfg_cell_param.
                                                   sib_type_7_Info)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_8_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib8(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib8, 
                                                   &p_cell_ctx->cfg_cell_param.
                                                   sib_type_8_Info,
                                                   p_cell_ctx, 1)))
  {
    ret_val = RRC_FAILURE;
  }
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_9_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib9(&asn1_sibs.asn1_sib9, 
                                                   &p_cell_ctx->cfg_cell_param.
                                                   sib_type_9_Info)))
  {
    ret_val = RRC_FAILURE;
  }

#ifdef LTE_EMBMS_SUPPORTED
  else if (   (p_cell_ctx->cfg_cell_param.presence_bitmask &
        CELL_SETUP_REQ_API_SIB_13_INFO_PRESENCE_FLAG) &&
      (RRC_FAILURE == bch_m_check_and_convert_sib13(&asn1_ctx,
                                                   &asn1_sibs.asn1_sib13, 
                                                   &p_cell_ctx->cfg_cell_param.
                                                   sib_type_13_Info)))
  {
    ret_val = RRC_FAILURE;
  }
#endif

  if (PNULL != p_cell_ctx->etws_info)
  {
      if (  ((8 << RRM_RRC_SIB_TYPE_10) & p_cell_ctx->pws_sibs_bitmask) && 
          (RRC_FAILURE == bch_m_check_and_convert_sib10(
          &asn1_sibs.asn1_sib10,&(p_cell_ctx->etws_info->sib10_info))))
      {
          ret_val = RRC_FAILURE;
      }
  }
  if(RRC_FAILURE == encode_sib1(&asn1_ctx,
        &p_llim_pws_req->sib1_msg_req, &asn1_sib1, p_cell_ctx))  
  {
    ret_val = RRC_FAILURE;

  }
  if(RRC_SUCCESS == ret_val)
  {
    if(PNULL != p_cell_ctx->etws_info)
    {
       sib_type_11 = &p_cell_ctx->etws_info->sib11_info;
    }
    /* SI Mapping Changed */    
    if (RRC_FAILURE == encode_sis( &asn1_ctx,
          &p_llim_pws_req->si_msg_req,
          &asn1_sibs,
          &p_cell_ctx->cfg_cell_param.sib_type_1_Info.
          scheduling_info_list,
          p_cell_ctx,
          p_cell_ctx->cfg_cell_param.sib_type_1_Info.
          si_window_length,
          sib_type_11,
          p_rrc_si_transmission_info_list
         /* SPR 17946 Fix Start */
          #ifdef LTE_EMBMS_SUPPORTED
          ,PNULL
          #endif
          ))
         /* SPR 17946 Fix End */
    {
      ret_val = RRC_FAILURE;
    }
  }
  rtFreeContext(&asn1_ctx);


  RRC_CSC_UT_TRACE_EXIT();

  return ret_val;
}

/******************************************************************************
 *     FUNCTION NAME :      fill_si_periodicity
 *     Input         :      U16 *pws_si_periodicity
 *                          U8  *si_periodicity      
 *     Output        :      None
 *     REturns       :      none
 *     DESCRIPTION   :      This function fills the si periodicity 
 *                          in terms of milli seconds.
 *******************************************************************************/
void fill_si_periodicity (U16 *pws_si_periodicity,
                          U8  *si_periodicity)
{
    RRC_CSC_UT_TRACE_ENTER();

    switch (*si_periodicity)
    {
        case RRM_RRC_RF_8:
           *pws_si_periodicity = 80;
           break;
        
        case RRM_RRC_RF_16:
           *pws_si_periodicity = 160;
           break;

        case RRM_RRC_RF_32:
           *pws_si_periodicity = 320;
           break;

        case RRM_RRC_RF_64:
           *pws_si_periodicity = 640;
           break;

        case RRM_RRC_RF_128:
           *pws_si_periodicity = 1280;
           break;

        case RRM_RRC_RF_256:
           *pws_si_periodicity = 2560;
           break;

        case RRM_RRC_RF_512:
           *pws_si_periodicity = 5120;
           break;
       
        default:
           RRC_TRACE (RRC_WARNING, "SI Periodicity is not correct");
           break;
    }

    RRC_CSC_UT_TRACE_EXIT();
}




/******************************************************************************
 *     FUNCTION NAME :      fill_si_periodicity_in_sfn
 *     Input         :      U8  si_periodicity      
 *     Output        :      None
 *     REturns       :      periodicity_in_sfn
 *     DESCRIPTION   :      This function fills the si periodicity
 *                          in terms of SFN.
*******************************************************************************/
U16 fill_si_periodicity_in_sfn (U8  si_periodicity)
{
    U16 periodicity_in_sfn = 0;

    switch (si_periodicity)
    {
        case RRM_RRC_RF_8:
           periodicity_in_sfn = 8;
           break;
        
        case RRM_RRC_RF_16:
           periodicity_in_sfn = 16;
           break;

        case RRM_RRC_RF_32:
           periodicity_in_sfn = 32;
           break;

        case RRM_RRC_RF_64:
           periodicity_in_sfn = 64;
           break;

        case RRM_RRC_RF_128:
           periodicity_in_sfn = 128;
           break;

        case RRM_RRC_RF_256:
           periodicity_in_sfn = 256;
           break;

        case RRM_RRC_RF_512:
           periodicity_in_sfn = 512;
           break;
       
        default:
           RRC_TRACE (RRC_WARNING, "SI Periodicity is not correct");
           break;
    }

    return periodicity_in_sfn;
}




/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_BCCH_DL_SCH_Message
*   INPUT        : U32 log_level
*                  const char* name
*                  BCCH_DL_SCH_Message* pvalue
*   OUTPUT       : none
*   DESCRIPTION  : This function prints ASN BCCH_DL_SCH_Message using RRC logging mechanism.
*
*   RETURNS      : void
*
*****************************************************************************/
void rrc_asn1PrtToStr_BCCH_DL_SCH_Message(U32 log_level, const char* name, 
                                        BCCH_DL_SCH_Message* pvalue, const char*
                                        msgName, U8 *p_buff, U16 msgLen) 
{
    int rrc_result =0;

    if (rrc_get_loglevel() >= log_level)
    {
        if( name != NULL && pvalue !=NULL)
        {
            S8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

            if(pBuff != NULL)
            {
                memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);
                rrc_result = asn1PrtToStr_BCCH_DL_SCH_Message(name, pvalue, 
                                    (char*)pBuff, MAX_ASN_PRINT_BUFF_SIZE);  /* Converted to char* as required by ASN function */
                if(rrc_result == 0)
                {/* SPR 13700 Fix start */
                    RRC_CSC_TRACE_ASN((S32)log_level, "%s\n", pBuff);/* SPR 13700 Fix stop */
                }
                else
                {
                    RRC_CSC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased\n");
                }
                rrc_mem_free(pBuff);
            }
        }
        else
        {
            RRC_CSC_TRACE(RRC_WARNING, "BCCH_DL_SCH_Message Info to be printed is missing\n");
        }
    }
    RRC_LOG_ASN_HEX_DUMP(name, msgName, p_buff, msgLen);
}



/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_BCCH_BCH_Message
*   INPUT        : U32 log_level
*                  const char* name
*                  BCCH_BCH_Message* pvalue
*   OUTPUT       : none
*   DESCRIPTION  : This function prints ASN BCCH_BCH_Message using RRC logging mechanism.
*
*   RETURNS      : void
*
*****************************************************************************/
void rrc_asn1PrtToStr_BCCH_BCH_Message(U32 log_level, const char* name, 
                                        BCCH_BCH_Message* pvalue, const char*
                                        msgName, U8 *p_buff, U16 msgLen)
{
    int rrc_result =0;

    if (rrc_get_loglevel() >= log_level)
    {
        if( name != NULL && pvalue !=NULL)
        {
            U8* pBuff = rrc_mem_get(MAX_ASN_PRINT_BUFF_SIZE);

            if(pBuff != NULL)
            {
                memset_wrapper(pBuff,0, MAX_ASN_PRINT_BUFF_SIZE);
                rrc_result = asn1PrtToStr_BCCH_BCH_Message(name, pvalue, 
                                    (char*)pBuff, MAX_ASN_PRINT_BUFF_SIZE); /* Converted to char* as required by ASN function */
                if(rrc_result == 0)
                {/* SPR 13700 Fix start */
                    RRC_CSC_TRACE_ASN((S32)log_level, "%s\n", pBuff);/* SPR 13700 Fix stop */
                }
                else
                {
                    RRC_CSC_TRACE(RRC_WARNING,"Buffer size for ASN needs to be increased\n");
                }
                rrc_mem_free(pBuff);
            }
        }
        else
        {
            RRC_CSC_TRACE(RRC_WARNING, "BCCH_BCH_Message Info to be printed is missing\n");
        }
    }
    RRC_LOG_ASN_HEX_DUMP(name, msgName, p_buff, msgLen);
}

