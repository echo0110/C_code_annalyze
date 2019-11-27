/******************************************************************************
*
*   FILE NAME:
*       rrc_uemh.c
*
*   DESCRIPTION:
*       This header file contains in points of UEMH submodule
*
*   DATE            AUTHOR      REFERENCE       REASON
*   14 May 2009     VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_uemh.h"
#include "rrc_defines.h"
#include "rrc_llim_logger.h"
#include "rrc_common_utils.h"
#include "rrc_uemh_timers.h"
#include "rrc_msg_mgmt.h"
#include "rrc_llim_utils.h"
#include "rrc_intrl_api.h"
#include "rrc_uecc_llim_intf.h"
#include "rrc_phy_intf.h"
#include "rrc_phy_il_parser.h"
#include "rrc_ext_api.h"
#include "rrc_mac_intf.h"
#include "rrc_mac_il_parser.h"
#include "rrc_mac_il_composer.h"
#include "rrc_rlc_intf.h"
#include "rrc_rlc_il_parser.h"
#include "rrc_pdcp_intf.h"
#include "rrc_pdcp_il_parser.h"
#include "rrc_pdcp_il_composer.h"
#include "rrc_s1u_intf.h"
#include "rrc_s1u_il_parser.h"
/* SPR 16087 Start */
#include "rrc_s1u_il_composer.h"
/* SPR 16087 Stop */
#include "uecc_event_queue.h"
#include "rrc_llim_cell_ctx.h"
#include "rrc_common_md.h"
/*SPR 15931 Fix Start*/
/*SPR 15931 Fix Stop*/

/* SPR 16087 Start */
static void uemh_send_s1u_end_marker_req
(
    rrc_uecc_llim_s1u_reconfigure_req_t *p_s1u_reconfigure_req 
);
/* SPR 16087 Stop */
/******************************************************************************
*   FUNCTION NAME: check_uecc_llim_srb_entity
*   INPUT        : rrc_uecc_llim_create_srb_entity_t   *p_srb_entity
*   OUTPUT       : none
*   DESCRIPTION:
*       Function checks data in rrc_uecc_llim_create_srb_entity_t
*
*   RETURNS:
*       RRC_FAILURE - if incorrect data passed
*       RRC_SUCCESS - otherwise
*
******************************************************************************/
static rrc_return_et check_uecc_llim_srb_entity
(
    rrc_uecc_llim_create_srb_entity_t   *p_srb_entity   /* data to be checked */
)
{
    rrc_return_et ret_val = RRC_SUCCESS;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_srb_entity);

    if ( ( !(p_srb_entity->srb_config.bitmask &
                RRM_SRB_CONFIG_AM_CONFIG_PRESENT) ) ||
         ( !(p_srb_entity->srb_config.srb_am_config.bitmask &
                RRM_SRB_AM_CONFIG_EXPLICIT_PRESENT) ) ||
         ( !(p_srb_entity->srb_config.bitmask &
                RRM_SRB_CONFIG_MAC_LC_CONFIG_ENB_PRESENT) ) ||
         ( !(p_srb_entity->srb_config.mac_lc_config_enb.bitmask &
                RRM_MAC_LC_CONFIG_UL_LC_G_ID_PRESENT) ) ||
         ( !(p_srb_entity->srb_config.mac_lc_config_enb.bitmask &
                RRM_MAC_LC_CONFIG_DL_PRIORITY_PRESENT) ))
    {
        ret_val = RRC_FAILURE;
    }

    RRC_LLIM_UT_TRACE_EXIT();

    return ret_val;
}

/*REL 1.2:HANDOVER PROCEDURE START*/
/******************************************************************************
*   FUNCTION NAME: check_uecc_llim_create_ue_entity_req
*   INPUT        : rrc_uecc_llim_create_ue_entity_req_t  *p_cr_ue_req
*   OUTPUT       : none
*   DESCRIPTION  : Function checks data from UECC_LLIM_CREATE_UE_ENTITY_REQ
*
*   RETURNS      : RRC_FAILURE - if incorrect data passed
*                  RRC_SUCCESS - otherwise
*
******************************************************************************/
static rrc_return_et check_uecc_llim_create_ue_entity_req
(
    rrc_uecc_llim_create_ue_entity_req_t  *p_cr_ue_req /* data to be checked */
)
{
    rrc_counter_t drb_index         = RRC_NULL;
    rrm_drb_config_t *p_drb_config  = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    /* Check presents necessary for MAC add_ue_info parameters */
    if ( ( !(p_cr_ue_req->physical_config_dedicated.bitmask
                & RRC_PHY_ANTENNA_INFORMATION_PRESENT) ) ||
         ( !(p_cr_ue_req->physical_config_dedicated.antenna_information.bitmask
                 & RRC_PHY_ANTENNA_INFORMATION_EXPLICIT_VALUE_PRESENT) ) )
    {
        RRC_LLIM_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }

    /* Check for SRB1 configuration */
    if (RRC_FAILURE ==
        check_uecc_llim_srb_entity(&p_cr_ue_req->create_srb1_entity))
    {
        RRC_LLIM_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    /* Check for HO Triggered Configuration*/
    if (p_cr_ue_req->bitmask
                & UECC_LLIM_CREATE_UE_ENTITY_REQ_HO_TRIGGERED_FLAG)  
    {
        /* Check for SRB2 configuration */
        if (p_cr_ue_req->bitmask & 
            UECC_LLIM_CREATE_UE_ENTITY_CREATE_SRB2_INFO_PRESENT)
        {
            if (RRC_FAILURE == check_uecc_llim_srb_entity(&p_cr_ue_req->
                 create_srb2_entity))
            {
                RRC_LLIM_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
        }

        /* Check for DRBs configuration */
        if ( p_cr_ue_req->bitmask & UECC_LLIM_CREATE_DRB_ENTITY_LIST_PRESENT)
        {
            for (drb_index = 0; drb_index < p_cr_ue_req->create_drb_entity_info.
                create_drb_entity_counter; ++drb_index)
           {
                p_drb_config = &p_cr_ue_req->create_drb_entity_info.
                    create_drb_entity[drb_index].uecc_llim_drb_config.drb_config;

                if ( ( !(p_drb_config->bitmask &
                    RRM_DRB_CONFIG_LOGICAL_CHANNEL_IDENTITY_PRESENT) ) ||
                     ( !(p_drb_config->bitmask &
                        RRM_DRB_CONFIG_RLC_CONFIG_PRESENT) ) ||
                    ( !(p_drb_config->bitmask &
                        RRM_DRB_CONFIG_S1U_CONFIG_PRESENT) ) ||
                    ( !(p_drb_config->bitmask &
                        RRM_DRB_CONFIG_MAC_LC_CONFIG_ENB_PRESENT) ) ||
                    ( (p_drb_config->bitmask &
                        RRM_DRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT) ) ||
                    ( (p_drb_config->bitmask &
                        RRM_DRB_CONFIG_RLC_CONFIG_UE_PRESENT) ) )
                {
                    RRC_LLIM_UT_TRACE_EXIT();
                    return RRC_FAILURE;
                }

                /* Specific RLC configuration must be present */
                if ( (RRC_RLC_CONFIG_AM_CONFIG_PRESENT != p_drb_config->
                    rlc_config_enb.rlc_config.bitmask) && 
                    (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT != 
                        p_drb_config->rlc_config_enb.rlc_config.bitmask) &&
                    (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT !=
                        p_drb_config->rlc_config_enb.rlc_config.bitmask) &&
                    (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT !=
                        p_drb_config->rlc_config_enb.rlc_config.bitmask) )
                {
                    RRC_LLIM_UT_TRACE_EXIT();
                    return RRC_FAILURE;
                }

                /* Logical channel group for Uplink side must be present */
                if ( (RRC_RLC_CONFIG_AM_CONFIG_PRESENT == p_drb_config->
                    rlc_config_enb.rlc_config.bitmask) || 
                   (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT ==
                        p_drb_config-> rlc_config_enb.rlc_config.bitmask) ||
                   (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT ==
                        p_drb_config->rlc_config_enb.rlc_config.bitmask) )
                {
                    if ( !(p_drb_config->mac_lc_config_enb.bitmask &
                        RRM_MAC_LC_CONFIG_UL_LC_G_ID_PRESENT) )
                    {
                        RRC_LLIM_UT_TRACE_EXIT();
                        return RRC_FAILURE;
                    }
                }

                /* Priority for Downlink side must be present */
                if ( (RRC_RLC_CONFIG_AM_CONFIG_PRESENT == p_drb_config->
                    rlc_config_enb.rlc_config.bitmask) || 
                    (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT ==
                        p_drb_config-> rlc_config_enb.rlc_config.bitmask) ||
                    (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT ==
                        p_drb_config->rlc_config_enb.rlc_config.bitmask) )
                {
                    if ( !(p_drb_config->mac_lc_config_enb.bitmask &
                        RRM_MAC_LC_CONFIG_DL_PRIORITY_PRESENT) )
                    {
                        RRC_LLIM_UT_TRACE_EXIT();
                        return RRC_FAILURE;
                    }
                }
            }
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: fill_uecc_llim_create_ue_entity_resp
*   INPUT        : rrc_uecc_llim_create_ue_entity_resp_t *p_create_ue_entity_resp
*                  rrc_response_t                        response_code
*                  rrc_uecc_llim_create_ue_entity_req_t  *p_create_ue_entity_req
*   OUTPUT       : none
*   DESCRIPTION  : Function fills UECC_LLIM_CREATE_UE_ENTITY_RESP message 
*                  with given response_code according to UECC_LLIM_CREATE_UE_ENTITY_REQ
*
*   RETURNS      : none
*
******************************************************************************/
static void fill_uecc_llim_create_ue_entity_resp
(
    rrc_uecc_llim_create_ue_entity_resp_t *p_create_ue_entity_resp, 
    /* destination */
    rrc_response_t                        response_code,  /* response code */
    rrc_uecc_llim_create_ue_entity_req_t  *p_create_ue_entity_req   
    /* main source */
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_create_ue_entity_resp);
    RRC_ASSERT(PNULL != p_create_ue_entity_req);

    p_create_ue_entity_resp->cell_index       = p_create_ue_entity_req->cell_index;
    p_create_ue_entity_resp->transaction_id = 
        p_create_ue_entity_req->transaction_id;
    p_create_ue_entity_resp->ue_index       = p_create_ue_entity_req->ue_index;
    p_create_ue_entity_resp->response_code  = response_code;

    if (p_create_ue_entity_req->bitmask & 
        UECC_LLIM_CREATE_UE_ENTITY_CREATE_SRB2_INFO_PRESENT)
    {
        p_create_ue_entity_resp->bitmask |= 
            UECC_LLIM_CREATE_SRB_ENTITY_ERROR_INFO_PRESENT;
        p_create_ue_entity_resp->srb_entity_error_info.lc_id     = 
            p_create_ue_entity_req->create_srb2_entity.lc_id;
        p_create_ue_entity_resp->srb_entity_error_info.response_code  = 
            response_code;
    }
    
    RRC_LLIM_UT_TRACE_EXIT();
}
/*REL 1.2:HANDOVER PROCEDURE END*/

/******************************************************************************
*   FUNCTION NAME: check_uecc_llim_add_lc_req
*   INPUT        : rrc_uecc_llim_add_lc_req_t  *p_add_lc_req
*   OUTPUT       : none
*   DESCRIPTION  : Function checks data in UECC_LLIM_ADD_LC_REQ
*
*   RETURNS      : RRC_FAILURE - if incorrect data passed
*                  RRC_SUCCESS - otherwise
*
******************************************************************************/
static rrc_return_et check_uecc_llim_add_lc_req
(
    rrc_uecc_llim_add_lc_req_t  *p_add_lc_req   /* data to be checked */
)
{
    rrc_counter_t drb_index         = RRC_NULL;
    rrm_drb_config_t *p_drb_config  = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    /* Check for SRB2 configuration */
    if (p_add_lc_req->bitmask & UECC_LLIM_ADD_LC_REQ_CREATE_SRB2_INFO_PRESENT)
    {
        if (RRC_FAILURE == check_uecc_llim_srb_entity(&p_add_lc_req->
            create_srb2_entity))
        {
            RRC_LLIM_TRACE(RRC_FATAL,"SRB INFO Incorrect!!!");
            RRC_LLIM_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }
    }

    /* Check for DRBs configuration */
    for (drb_index = 0; drb_index < p_add_lc_req->create_drb_entity_counter;
        ++drb_index)
    {
        p_drb_config = &p_add_lc_req->create_drb_entity[drb_index].uecc_llim_drb_config.drb_config;

        if ( ( !(p_drb_config->bitmask &
                    RRM_DRB_CONFIG_LOGICAL_CHANNEL_IDENTITY_PRESENT) ) ||
             ( !(p_drb_config->bitmask &
                    RRM_DRB_CONFIG_RLC_CONFIG_PRESENT) ) ||
             ( !(p_drb_config->bitmask &
                    RRM_DRB_CONFIG_S1U_CONFIG_PRESENT) ) ||
             ( !(p_drb_config->bitmask &
                    RRM_DRB_CONFIG_MAC_LC_CONFIG_ENB_PRESENT) ) ||
             ( (p_drb_config->bitmask &
                    RRM_DRB_CONFIG_LOGICAL_CHANNEL_CONFIG_PRESENT) ) ||
             ( (p_drb_config->bitmask &
                    RRM_DRB_CONFIG_RLC_CONFIG_UE_PRESENT) ) )
        {
            RRC_LLIM_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        /* Specific RLC configuration must be present */
        if ( (RRC_RLC_CONFIG_AM_CONFIG_PRESENT != p_drb_config->rlc_config_enb.
              rlc_config.bitmask) &&
             (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT != p_drb_config->
                 rlc_config_enb.rlc_config.bitmask) &&
             (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT !=
                 p_drb_config->rlc_config_enb.rlc_config.bitmask) &&
             (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT !=
                 p_drb_config->rlc_config_enb.rlc_config.bitmask) )
        {
            RRC_LLIM_UT_TRACE_EXIT();
            return RRC_FAILURE;
        }

        /* Logical channel group for Uplink side must be present */
        if ( (RRC_RLC_CONFIG_AM_CONFIG_PRESENT == p_drb_config->rlc_config_enb.
              rlc_config.bitmask) ||
             (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT == p_drb_config->
                 rlc_config_enb.rlc_config.bitmask) ||
             (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_UL_PRESENT ==
                 p_drb_config->rlc_config_enb.rlc_config.bitmask) )
        {
            if ( !(p_drb_config->mac_lc_config_enb.bitmask &
                        RRM_MAC_LC_CONFIG_UL_LC_G_ID_PRESENT) )
            {
                RRC_LLIM_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
        }

        /* Priority for Downlink side must be present */
        if ( (RRC_RLC_CONFIG_AM_CONFIG_PRESENT == p_drb_config->rlc_config_enb.
              rlc_config.bitmask) ||
             (RRC_RLC_CONFIG_UM_BI_DIRECTIONAL_CONFIG_PRESENT == p_drb_config->
                 rlc_config_enb.rlc_config.bitmask) ||
             (RRC_RLC_CONFIG_UM_UNI_DIRECTIONAL_CONFIG_DL_PRESENT ==
                 p_drb_config->rlc_config_enb.rlc_config.bitmask) )
        {
            if ( !(p_drb_config->mac_lc_config_enb.bitmask &
                        RRM_MAC_LC_CONFIG_DL_PRIORITY_PRESENT) )
            {
                RRC_LLIM_UT_TRACE_EXIT();
                return RRC_FAILURE;
            }
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: fill_uecc_llim_add_lc_resp
*   INPUT        : rrc_uecc_llim_add_lc_resp_t *p_add_lc_resp
*                  rrc_response_t              response_code
*                  rrc_uecc_llim_add_lc_req_t  *p_add_lc_req
*   OUTPUT       : none
*   DESCRIPTION  : Function fills UECC_LLIM_ADD_LC_RESP message with given response_code
*                  according to UECC_LLIM_ADD_LC_REQ
*
*   RETURNS      : None
*
******************************************************************************/
static void fill_uecc_llim_add_lc_resp
(
    rrc_uecc_llim_add_lc_resp_t *p_add_lc_resp, /* destination */
    rrc_response_t              response_code,  /* response code */
    rrc_uecc_llim_add_lc_req_t  *p_add_lc_req   /* main source */
)
{
    U16 drb_index = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_add_lc_resp);
    RRC_ASSERT(PNULL != p_add_lc_req);

    p_add_lc_resp->cell_index                   = p_add_lc_req->cell_index;
    p_add_lc_resp->transaction_id               = p_add_lc_req->transaction_id;
    p_add_lc_resp->ue_index                     = p_add_lc_req->ue_index;
    p_add_lc_resp->response_code                = response_code;

    p_add_lc_resp->bitmask                      = 0;
    p_add_lc_resp->create_drb_status_counter    = p_add_lc_req->
        create_drb_entity_counter;

    if (p_add_lc_req->bitmask & UECC_LLIM_ADD_LC_REQ_CREATE_SRB2_INFO_PRESENT)
    {
        p_add_lc_resp->bitmask |= UECC_LLIM_ADD_LC_RESP_SRB2_STATUS_PRESENT;

        p_add_lc_resp->create_srb2_status.lc_id     = p_add_lc_req->
            create_srb2_entity.lc_id;
        p_add_lc_resp->create_srb2_status.response  = response_code;
    }

    for (drb_index = 0; drb_index < p_add_lc_req->create_drb_entity_counter;
        drb_index++)
    {
        p_add_lc_resp->create_drb_status[drb_index].lc_id =
            p_add_lc_req->create_drb_entity[drb_index].uecc_llim_drb_config.drb_config.
                logical_channel_identity;
        p_add_lc_resp->create_drb_status[drb_index].response = response_code;

        p_add_lc_resp->create_drb_status[drb_index].bitmask = 0;
        memset_wrapper(&p_add_lc_resp->create_drb_status[drb_index].s1u_response,
            0, sizeof(p_add_lc_resp->create_drb_status[drb_index].
                s1u_response));
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/*REL 1.2:HANDOVER PROCEDURE START*/
/******************************************************************************
*   FUNCTION NAME: fill_uecc_llim_sn_hfn_status_resp
*   INPUT        : rrc_uecc_llim_sn_hfn_status_resp_t  *p_sn_hfn_status_resp
*                  rrc_response_t                      response_code
*                  rrc_uecc_llim_sn_hfn_status_req_t   *p_sn_hfn_status_req
*   OUTPUT       : none 
*   DESCRIPTION  : Function fills UECC_LLIM_SN_HFN_STATUS_RESP message with given
*                  response_code according to UECC_LLIM_SN_HFN_STATUS_REQ
*
*   RETURNS      : None
*
******************************************************************************/

static void fill_uecc_llim_sn_hfn_status_resp
(
    rrc_uecc_llim_sn_hfn_status_resp_t  *p_sn_hfn_status_resp, /* destination */
    rrc_response_t                      response_code,         /*response code*/
    rrc_uecc_llim_sn_hfn_status_req_t   *p_sn_hfn_status_req   /* main source */
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_sn_hfn_status_resp);
    RRC_ASSERT(PNULL != p_sn_hfn_status_req);

    p_sn_hfn_status_resp->cell_index     = p_sn_hfn_status_req->cell_index;
    p_sn_hfn_status_resp->transaction_id = p_sn_hfn_status_req->transaction_id;
    p_sn_hfn_status_resp->ue_index       = p_sn_hfn_status_req->ue_index;
    p_sn_hfn_status_resp->response_code  = response_code;

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: fill_uecc_llim_mac_i_resp
*   INPUT        : rrc_uecc_llim_mac_i_resp_t  *p_mac_i_resp
*                  rrc_response_t              response_code
*                  rrc_uecc_llim_mac_i_req_t  *p_mac_i_req
*  OUTPUT        : none
*   DESCRIPTION  : Function fills UECC_LLIM_MAC_I_RESP message with given response_code
*                  according to UECC_LLIM_MAC_I_REQ
*
*   RETURNS      : None
*
******************************************************************************/
static void fill_uecc_llim_mac_i_resp
(
    rrc_uecc_llim_mac_i_resp_t  *p_mac_i_resp, /* destination */
    rrc_response_t              response_code,  /* response code */
    rrc_uecc_llim_mac_i_req_t  *p_mac_i_req   /* main source */
)
{

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_mac_i_resp);
    RRC_ASSERT(PNULL != p_mac_i_req);

    p_mac_i_resp->cell_index                   = p_mac_i_req->cell_index;
    p_mac_i_resp->transaction_id               = p_mac_i_req->transaction_id;
    p_mac_i_resp->ue_index                     = p_mac_i_req->ue_index;
    p_mac_i_resp->response_code                = response_code;

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: check_uecc_llim_s1u_reconfigure_req
*   INPUT        : rrc_uecc_llim_s1u_reconfigure_req_t  *p_s1u_reconfigure_req
*   OUTPUT       : none
*   DESCRIPTION  : Function checks data in UECC_LLIM_S1U_RECONFIGURE_REQ
*
*   RETURNS      : RRC_FAILURE - if incorrect data passed
*                  RRC_SUCCESS - otherwise
*
******************************************************************************/
static rrc_return_et check_uecc_llim_s1u_reconfigure_req
(
    rrc_uecc_llim_s1u_reconfigure_req_t  *p_s1u_reconfigure_req  
    /* data to be checked */
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    /* Check for setup sap or rel sap configuration */
    if ( ( !(p_s1u_reconfigure_req->bitmask & 
        RRC_UECC_LLIM_S1U_SETUP_SAP_REQ_LIST_PRESENT ) ) &&
         ( !(p_s1u_reconfigure_req->bitmask & 
        RRC_UECC_LLIM_S1U_REL_SAP_REQ_LIST_PRESENT) ) )
    {
        RRC_LLIM_UT_TRACE_EXIT();
        return RRC_FAILURE;
    }
    RRC_LLIM_UT_TRACE_EXIT();

    return RRC_SUCCESS;
}

/******************************************************************************
*   FUNCTION NAME: fill_uecc_llim_s1u_reconfigure_cnf
*   INPUT        : rrc_uecc_llim_s1u_reconfigure_cnf_t  *p_s1u_reconfigure_cnf
*                  rrc_response_t              response_code
*                  rrc_uecc_llim_s1u_reconfigure_req_t  *p_s1u_reconfigure_req
*   OUTPUT       : none
*   DESCRIPTION  : Function fills UECC_LLIM_S1U_RECONFIGURE_CNF message with given 
*                  response_code according to UECC_LLIM_S1U_RECONFIGURE_REQ
*
*   RETURNS      : None
*
******************************************************************************/
static void fill_uecc_llim_s1u_reconfigure_cnf
(
    rrc_uecc_llim_s1u_reconfigure_cnf_t  *p_s1u_reconfigure_cnf, /*destination*/
    rrc_response_t              response_code,  /* response code */
    rrc_uecc_llim_s1u_reconfigure_req_t  *p_s1u_reconfigure_req  /*main source*/
)
{
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_s1u_reconfigure_cnf);
    RRC_ASSERT(PNULL != p_s1u_reconfigure_req);

    p_s1u_reconfigure_cnf->cell_index        = p_s1u_reconfigure_req->cell_index;
    p_s1u_reconfigure_cnf->transaction_id    = 
        p_s1u_reconfigure_req->transaction_id;
    p_s1u_reconfigure_cnf->ue_index          = p_s1u_reconfigure_req->ue_index;
    p_s1u_reconfigure_cnf->response_code     = response_code;

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: fill_uecc_llim_ue_inactive_time_resp
*   INPUT        : rrc_uecc_llim_ue_inactive_time_resp_t  *p_ue_inactive_time_resp
*                  rrc_response_t              response_code
*                  rrc_uecc_llim_ue_inactive_time_req_t  *p_ue_inactive_time_req
*   OUTPUT       : none
*   DESCRIPTION  : Function fills UECC_LLIM_UE_INACTIVE_TIME_RESP message with given 
*                  response_code according to UECC_LLIM_UE_INACTIVE_TIME_REQ
*
*   RETURNS      : None
*
******************************************************************************/
static void fill_uecc_llim_ue_inactive_time_resp
(
    rrc_uecc_llim_ue_inactive_time_resp_t  *p_ue_inactive_time_resp,
    /*destination*/
    rrc_response_t              response_code,  /* response code */
    rrc_uecc_llim_ue_inactive_time_req_t  *p_ue_inactive_time_req  
    /*main source*/
)
{

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_ue_inactive_time_resp);
    RRC_ASSERT(PNULL != p_ue_inactive_time_req);

    p_ue_inactive_time_resp->cell_index     = p_ue_inactive_time_req->cell_index;
    p_ue_inactive_time_resp->transaction_id = 
        p_ue_inactive_time_req->transaction_id;
    p_ue_inactive_time_resp->ue_index       = p_ue_inactive_time_req->ue_index;
    p_ue_inactive_time_resp->response_code  = response_code;

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: fill_uecc_llim_ho_rach_resource_resp
*   INPUT        : rrc_uecc_llim_ho_rach_resource_resp_t *p_ho_rach_resource_resp
*                  rrc_response_t                        response_code
*                  rrc_uecc_llim_ho_rach_resource_req_t  *p_ho_rach_resource_req
*   OUTPUT       : none
*   DESCRIPTION  : Function fills UECC_LLIM_HO_RACH_RESOURCE_RESP
*                  according to UECC_LLIM_HO_RACH_RESOURCE_REQ
*
*   RETURNS      : None
*
******************************************************************************/
static void fill_uecc_llim_ho_rach_resource_resp
(
    rrc_uecc_llim_ho_rach_resource_resp_t *p_ho_rach_resource_resp, 
    /*destination*/
    rrc_response_t                        response_code, /* response code */
    rrc_uecc_llim_ho_rach_resource_req_t  *p_ho_rach_resource_req   
    /*main source*/
)
{

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_ho_rach_resource_resp);
    RRC_ASSERT(PNULL != p_ho_rach_resource_req);

    p_ho_rach_resource_resp->cell_index     = p_ho_rach_resource_req->cell_index;
    p_ho_rach_resource_resp->transaction_id = 
        p_ho_rach_resource_req->transaction_id;
    p_ho_rach_resource_resp->ue_index       = p_ho_rach_resource_req->ue_index;
    p_ho_rach_resource_resp->response_code  = response_code;

    RRC_LLIM_UT_TRACE_EXIT();
}
/*REL 1.2:HANDOVER PROCEDURE END*/

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_create_ue_entity_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function partially process UECC_LLIM_CREATE_UE_ENTITY_REQ message
*                  and call FSM.
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_create_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
	/* coverity_fix_54711_start */
	rrc_uecc_llim_create_ue_entity_req_t *p_create_ue_entity_req    = PNULL;
	rrc_llim_ue_ctx_t *p_ue_ctx                                     = PNULL;
	rrc_bool_et is_payload_correct                                  = RRC_TRUE;
	rrc_llim_uecc_response_et response_code                         = RRC_LLIM_FAILURE;

	RRC_LLIM_UT_TRACE_ENTER();

	RRC_ASSERT(PNULL != p_api);
	RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

	/* Precondition - p_api should point to right message */
	if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
			(rrc_uecc_llim_create_ue_entity_req_t))
	{
		RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
		RRC_LLIM_UT_TRACE_EXIT();
		return;
	}

	p_create_ue_entity_req = (rrc_uecc_llim_create_ue_entity_req_t *)((U8*
				)p_api + RRC_API_HEADER_SIZE);

        SET_CELL_AND_UE_INDEX(p_create_ue_entity_req->cell_index, p_create_ue_entity_req->ue_index);

	do
	{
		/* Check for correct data */
		if (RRC_FAILURE ==
				check_uecc_llim_create_ue_entity_req(p_create_ue_entity_req))
		{
			is_payload_correct  = RRC_FALSE;
			response_code       = RRC_LLIM_INCORRECT_DATA_PASSED;

			RRC_LLIM_TRACE(RRC_WARNING, "Wrong message data - ignore.");

			break;
		}
		/* Check whether UE's Cell exists or not */
		if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || ( p_rrc_llim_gl_ctx->
					p_p_cell_ctx[p_create_ue_entity_req->cell_index] == PNULL))
		{
			is_payload_correct  = RRC_FALSE;
			response_code       = RRC_LLIM_CELL_NOT_EXIST;

			RRC_LLIM_TRACE(RRC_WARNING, "Cell Does not Exist - ignore.");

			break;
		}


		rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_create_ue_entity_req->ue_index, &p_ue_ctx);


/*SPR_19066_START*/
        if (!(UECC_LLIM_CREATE_UE_ENTITY_REQ_HO_TRIGGERED_FLAG & p_create_ue_entity_req->bitmask))
/*SPR_19066_END*/
		{
			if (PNULL != p_ue_ctx)
			{
				/* Already exist */
				is_payload_correct  = RRC_FALSE;
				response_code       = RRC_LLIM_UE_ALREADY_EXIST;

				break;
			}

			/* Create and add to DB */
			p_ue_ctx = (rrc_llim_ue_ctx_t *)rrc_mem_get(sizeof(rrc_llim_ue_ctx_t));

			if (PNULL == p_ue_ctx)
			{
				is_payload_correct  = RRC_FALSE;
				response_code       = RRC_LLIM_UE_NOT_ENOUGH_MEMORY;

				break;
			}   

			/* Successful case */
			memset_wrapper(p_ue_ctx, 0, sizeof(rrc_llim_ue_ctx_t));

			rrc_llim_set_ue_context(p_rrc_llim_gl_ctx, p_create_ue_entity_req->ue_index, p_ue_ctx);
			/* Fill UE index. Only for logging purposes */
			if (PNULL != p_ue_ctx)
			{
				p_ue_ctx->ue_index = p_create_ue_entity_req->ue_index;
				p_ue_ctx->cell_index = p_create_ue_entity_req->cell_index;

				/* Initialize al pointers */
				p_ue_ctx->curr_op_param.p_create_ue         = PNULL;
				p_ue_ctx->curr_op_param.p_del_ue            = PNULL;
				p_ue_ctx->curr_op_param.p_rcfg_ue           = PNULL;
				p_ue_ctx->curr_op_param.p_cfg_ue_security   = PNULL;
				p_ue_ctx->curr_op_param.p_rach_res_req      = PNULL;
				p_ue_ctx->curr_op_param.p_suspend_ue_req    = PNULL;
				p_ue_ctx->curr_op_param.p_reestablish_ue    = PNULL;
				p_ue_ctx->curr_op_param.p_recfg_ue_security = PNULL;
				p_ue_ctx->curr_op_param.p_resume_ue         = PNULL;
				p_ue_ctx->curr_op_param.p_change_crnti      = PNULL;

				/* Init main IEs of UE context */
				UEMH_FSM_INIT(p_ue_ctx);
			}   
		}
		/* Normal message processing */
		uemh_fsm_process_event( UEMH_EVENT_UECC_CREATE_UE_ENTITY_REQ,
				p_api,
				p_create_ue_entity_req,
				p_ue_ctx,
				p_rrc_llim_gl_ctx);
	} while (0);

	if (RRC_FALSE == is_payload_correct)
	{
		rrc_uecc_llim_create_ue_entity_resp_t create_ue_entity_resp;
		memset_wrapper(&create_ue_entity_resp, 0, sizeof(create_ue_entity_resp));
		fill_uecc_llim_create_ue_entity_resp(&create_ue_entity_resp,
				response_code, p_create_ue_entity_req);

		rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_create_ue_entity_req->ue_index),
				UECC_LLIM_CREATE_UE_ENTITY_RESP,
				sizeof(create_ue_entity_resp),
				&create_ue_entity_resp);
	}

	RRC_LLIM_UT_TRACE_EXIT();
}
/* coverity_fix_54711_stop */

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_delete_ue_entity_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_DELETE_UE_ENTITY_REQ message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_delete_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_delete_ue_entity_req_t *p_delete_ue_entity_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    rrc_bool_et is_payload_correct                      = RRC_TRUE;
    rrc_llim_uecc_response_et response_code             = RRC_LLIM_FAILURE;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_delete_ue_entity_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_delete_ue_entity_req = (rrc_uecc_llim_delete_ue_entity_req_t *)((U8*
        )p_api + RRC_API_HEADER_SIZE);
    
    SET_CELL_AND_UE_INDEX(p_delete_ue_entity_req->cell_index, p_delete_ue_entity_req->ue_index); 
    
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_delete_ue_entity_req->ue_index, &p_ue_ctx);

    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_delete_ue_entity_req->cell_index] == PNULL))
    {
        is_payload_correct  = RRC_FALSE;
        response_code       = RRC_LLIM_CELL_NOT_EXIST;

    }

    /* Search in DB UE with p_delete_ue_entity_req->ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        is_payload_correct  = RRC_FALSE;
        response_code       = RRC_LLIM_UE_IS_ABSENT;
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {

            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_DELETE_UE_ENTITY_REQ,
                    p_api,
                    p_delete_ue_entity_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
   
    if (is_payload_correct == RRC_FALSE )
    {
        rrc_uecc_llim_delete_ue_entity_resp_t delete_ue_entity_resp;

        /*SPR 21160 start*/
        memset_wrapper(&delete_ue_entity_resp, RRC_NULL,
            sizeof(rrc_uecc_llim_delete_ue_entity_resp_t));
        /*SPR 21160 stop*/

        delete_ue_entity_resp.cell_index        = p_delete_ue_entity_req->
            cell_index;
        delete_ue_entity_resp.transaction_id    = p_delete_ue_entity_req->
            transaction_id;
        delete_ue_entity_resp.ue_index          = p_delete_ue_entity_req->
            ue_index;
        delete_ue_entity_resp.response_code     = response_code;

        rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_delete_ue_entity_req->ue_index),
                                    UECC_LLIM_DELETE_UE_ENTITY_RESP,
                                    sizeof(delete_ue_entity_resp),
                                    &delete_ue_entity_resp);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_configure_security_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially processes
*                  UECC_LLIM_CONFIGURE_SECURITY_REQ message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_configure_security_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_configure_security_req_t *p_cfg_security_req  = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx                                 = PNULL;
    rrc_bool_et is_payload_correct                              = RRC_TRUE;
    rrc_llim_uecc_response_et response_code                     = RRC_LLIM_FAILURE;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_configure_security_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_cfg_security_req = (rrc_uecc_llim_configure_security_req_t *)((U8* )p_api
        + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_cfg_security_req->cell_index, p_cfg_security_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_cfg_security_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_cfg_security_req->cell_index] == PNULL))
    {
        is_payload_correct  = RRC_FALSE;
        response_code       = RRC_LLIM_CELL_NOT_EXIST;

    }   
    /* Search in DB UE with p_cfg_security_req->ue_index - should be present */
    else if ((PNULL == (p_rrc_llim_gl_ctx->
                       p_p_cell_ctx[p_cfg_security_req->cell_index])) || 
            (PNULL == p_ue_ctx))
    {
        is_payload_correct  = RRC_FALSE;
        response_code       = RRC_LLIM_FAILURE;
    }
    else if (PNULL != p_ue_ctx)
    {
        /* Normal message processing */
        uemh_fsm_process_event( UEMH_EVENT_UECC_CONFIGURE_SECURITY_REQ,
                p_api,
                p_cfg_security_req,
                p_ue_ctx,
                p_rrc_llim_gl_ctx);
    }
    if (is_payload_correct == RRC_FALSE )
    {
        /* UE context is not found - send response with failure response code */
        rrc_uecc_llim_configure_security_resp_t cfg_security_resp;

        cfg_security_resp.cell_index        = p_cfg_security_req->cell_index;
        cfg_security_resp.transaction_id    = p_cfg_security_req->
            transaction_id;
        cfg_security_resp.ue_index          = p_cfg_security_req->ue_index;
        cfg_security_resp.response_code     = response_code;

        rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_cfg_security_req->ue_index),
                UECC_LLIM_CONFIGURE_SECURITY_RESP,
                sizeof(cfg_security_resp),
                &cfg_security_resp);
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_add_lc_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_ADD_LC_REQ message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_add_lc_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_add_lc_req_t *p_add_lc_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message, removed as buffer size increased */

    p_add_lc_req = (rrc_uecc_llim_add_lc_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_add_lc_req->cell_index, p_add_lc_req->ue_index);

    /* Check for correct data */
    if (RRC_FAILURE == check_uecc_llim_add_lc_req(p_add_lc_req))
    {
        rrc_uecc_llim_add_lc_resp_t add_lc_resp;
        memset_wrapper(&add_lc_resp, 0, sizeof(add_lc_resp));

        fill_uecc_llim_add_lc_resp(&add_lc_resp, RRC_LLIM_INCORRECT_DATA_PASSED,
            p_add_lc_req);

        rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_add_lc_req->ue_index), UECC_LLIM_ADD_LC_RESP,
                                    sizeof(add_lc_resp), &add_lc_resp);

        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message data - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }


    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_add_lc_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_add_lc_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"Cell ctx for id %d not found!",
                p_add_lc_req->cell_index);

    }
    /* Search in DB UE with p_add_lc_req->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                p_add_lc_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {

            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_ADD_LC_REQ,
                    p_api,
                    p_add_lc_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_add_roll_back_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_MODIFY_ROLLBACK_REQ message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_add_roll_back_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_add_rollback_req_t  *p_add_lc_roll_back_req  = PNULL;
    
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_add_rollback_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_add_lc_roll_back_req = (rrc_uecc_llim_add_rollback_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);
    
    SET_CELL_AND_UE_INDEX(p_add_lc_roll_back_req->cell_index, p_add_lc_roll_back_req->ue_index);
   
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_add_lc_roll_back_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_add_lc_roll_back_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_add_lc_roll_back_req->cell_index);
    }

    /* Search in DB UE with p_add_lc_req->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                p_add_lc_roll_back_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_ADD_LC_ROLLBACK_REQ,
                    p_api,
                    p_add_lc_roll_back_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}




/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_modify_roll_back_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_MODIFY_ROLLBACK_REQ message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_modify_roll_back_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_modify_rollback_req_t  *p_modify_lc_roll_back_req  = PNULL;
    
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_modify_rollback_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_modify_lc_roll_back_req = (rrc_uecc_llim_modify_rollback_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_modify_lc_roll_back_req->cell_index, p_modify_lc_roll_back_req->ue_index);
    
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_modify_lc_roll_back_req->ue_index, &p_ue_ctx);


    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_modify_lc_roll_back_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_modify_lc_roll_back_req->cell_index);
    }
    /* Search in DB UE with p_modify_lc_req->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                p_modify_lc_roll_back_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_MODIFY_LC_ROLLBACK_REQ,
                    p_api,
                    p_modify_lc_roll_back_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_modify_lc_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_MODIFY_LC_REQ message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_modify_lc_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_modify_lc_req_t  *p_modify_lc_req  = PNULL;
    
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_modify_lc_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    p_modify_lc_req = (rrc_uecc_llim_modify_lc_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_modify_lc_req->cell_index, p_modify_lc_req->ue_index);
    
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_modify_lc_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_modify_lc_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_modify_lc_req->cell_index);
    }
    /* Search in DB UE with p_modify_lc_req->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                p_modify_lc_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_MODIFY_LC_REQ,
                    p_api,
                    p_modify_lc_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_srb_data_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function process UECC_LLIM_SRB_DATA_REQ message
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_srb_data_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_srb_data_req_t *p_uecc_srb_data_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    p_uecc_srb_data_req = (rrc_uecc_llim_srb_data_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_uecc_srb_data_req->cell_index, p_uecc_srb_data_req->ue_index);

    if (    (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_srb_data_req_t)) ||
            /* We have enough length for mandatory params, check data
             *  buffer size */
            rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_llim_srb_data_req_t) + p_uecc_srb_data_req->
                data_buffer_length)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_uecc_srb_data_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_uecc_srb_data_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_uecc_srb_data_req->cell_index);
    }
    /* Search in DB UE with p_srb_data_req->ue_index - should be present */
    else if (PNULL == p_ue_ctx)
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                p_uecc_srb_data_req->ue_index);
    }
    else
    {
        void *p_msg = PNULL;
        /* Fill RRC_PDCP_SRB_DATA_REQ message */
        rrc_pdcp_srb_data_req_t pdcp_srb_data_req;

        
        if (PNULL != p_ue_ctx)
        {
            memset_wrapper (&pdcp_srb_data_req, RRC_NULL, sizeof(rrc_pdcp_srb_data_req_t));

            pdcp_srb_data_req.transaction_id = p_uecc_srb_data_req->transaction_id;
            pdcp_srb_data_req.ue_index          = p_uecc_srb_data_req->ue_index;
            pdcp_srb_data_req.lc_id             = p_uecc_srb_data_req->lc_id;
            pdcp_srb_data_req.service_requested = p_uecc_srb_data_req->
                service_requested;

            /* Allocate buffer for storing RRC_PDCP_SRB_DATA_REQ message */
            p_msg = rrc_llim_alloc_external_msg(p_uecc_srb_data_req->transaction_id,
                    		RRC_GET_LL_MODULE_ID(RRC_PDCP_1_MODULE_ID,p_uecc_srb_data_req->cell_index),
			RRC_PDCP_MODULE_ID,
                    RRC_PDCP_SRB_DATA_REQ,
                    (U16)(rrc_il_get_rrc_pdcp_srb_data_req_len
                        (&pdcp_srb_data_req) +
                        p_uecc_srb_data_req->
                        data_buffer_length),
                        p_uecc_srb_data_req->cell_index);

            if (PNULL != p_msg)
            {
                U8 *p_curr_pos = (U8*)p_msg + RRC_FULL_INTERFACE_HEADERS_SIZE;
                /* Compose RRC_PDCP_SRB_DATA_REQ */
                rrc_il_compose_rrc_pdcp_srb_data_req(&p_curr_pos,
                        &pdcp_srb_data_req);

                l3_memcpy_wrapper( p_curr_pos,
                        p_uecc_srb_data_req->data_buffer,
                        p_uecc_srb_data_req->data_buffer_length);

                rrc_send_message(p_msg, 
					RRC_GET_LL_MODULE_ID(RRC_PDCP_1_MODULE_ID,p_uecc_srb_data_req->cell_index));
            }
        }
    }



    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_ccch_data_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function process UECC_LLIM_CCCH_DATA_REQ message
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_ccch_data_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_ccch_data_req_t *p_uecc_ccch_data_req = PNULL;
    rrc_mac_ccch_msg_req_t mac_ccch_msg_req;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILED, 
            "Unused variable in fsm llim_gb_context=%p", p_rrc_llim_gl_ctx);    
    /* Precondition - p_api should point to right message */
    p_uecc_ccch_data_req = (rrc_uecc_llim_ccch_data_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_uecc_ccch_data_req->cell_index);

    if (    (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_ccch_data_req_t)) ||
            /* We have enough length for mandatory params, check data
             *  buffer size */
            rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_llim_ccch_data_req_t) + p_uecc_ccch_data_req->
                data_buffer_length)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Special case CCCH */
    {
        void *p_msg = PNULL;
        
        memset_wrapper (&mac_ccch_msg_req, RRC_NULL, sizeof(rrc_mac_ccch_msg_req_t));

        /* Fill RRC_MAC_CCCH_MSG_REQ message */

        mac_ccch_msg_req.rnti = p_uecc_ccch_data_req->rnti;

        /* Allocate buffer for storing RRC_MAC_CCCH_MSG_REQ message */
        p_msg = rrc_llim_alloc_external_msg(RRC_TRANSACTION_ID_ABSENT,
							RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_uecc_ccch_data_req->cell_index),
						RRC_MAC_MODULE_ID,
                                            RRC_MAC_CCCH_MSG_REQ,
                                            (U16)( rrc_il_get_rrc_mac_ccch_msg_req_len(
                                            &mac_ccch_msg_req) +
                                                p_uecc_ccch_data_req->
                                                data_buffer_length),
                                            p_uecc_ccch_data_req->cell_index);

        if (PNULL != p_msg)
        {
            U8 *p_curr_pos = (U8*)p_msg + RRC_FULL_INTERFACE_HEADERS_SIZE;
            /* Compose RRC_MAC_CCCH_MSG_REQ */
            rrc_il_compose_rrc_mac_ccch_msg_req(&p_curr_pos, &mac_ccch_msg_req);

            l3_memcpy_wrapper( p_curr_pos,
                    p_uecc_ccch_data_req->data_buffer,
                    p_uecc_ccch_data_req->data_buffer_length);
            
            rrc_send_message(p_msg, 
				RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_uecc_ccch_data_req->cell_index));
		}
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_ue_con_reest_rej_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function process UECC_LLIM_UE_CON_RE_ESTABLISH_REJ_REQ message
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_ue_con_reest_rej_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_con_re_establish_rej_req_t 
        *p_uecc_con_re_establish_rej_req = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILED, 
            "Unused variable in fsm llim_gb_context=%p", p_rrc_llim_gl_ctx);    
    p_uecc_con_re_establish_rej_req = (rrc_uecc_llim_con_re_establish_rej_req_t*)
        ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_uecc_con_re_establish_rej_req->cell_index);

    /* Precondition - p_api should point to right message */
    if (    (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_con_re_establish_rej_req_t)) ||
            /* We have enough length for mandatory params, check data
             *  buffer size */
            rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_llim_con_re_establish_rej_req_t) + 
            p_uecc_con_re_establish_rej_req->data_buffer_length)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Special case connection reject */
    {
        void *p_msg = PNULL;
        /* Fill RRC_MAC_UE_CON_REJ_REQ message */
        rrc_mac_ue_con_rej_req_t rrc_mac_ue_con_rej_req;

        memset_wrapper (&rrc_mac_ue_con_rej_req, RRC_NULL, sizeof(rrc_mac_ue_con_rej_req_t));

        rrc_mac_ue_con_rej_req.tcrnti = p_uecc_con_re_establish_rej_req->crnti;

        /* Allocate buffer for storing RRC_MAC_UE_CON_REJ_REQ message */
        p_msg = rrc_llim_alloc_external_msg(RRC_TRANSACTION_ID_ABSENT,
							RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_uecc_con_re_establish_rej_req->cell_index),
						RRC_MAC_MODULE_ID,
                                            RRC_MAC_UE_CON_REJ_REQ,
        (U16)(rrc_il_get_rrc_mac_ue_con_rej_req_len(&rrc_mac_ue_con_rej_req) + 
            p_uecc_con_re_establish_rej_req->data_buffer_length),
        p_uecc_con_re_establish_rej_req->cell_index);

        if (PNULL != p_msg)
        {
            U8 *p_curr_pos = (U8*)p_msg + RRC_FULL_INTERFACE_HEADERS_SIZE;
            /* Compose RRC_MAC_UE_CON_REJ_REQ */
            rrc_il_compose_rrc_mac_ue_con_rej_req(&p_curr_pos,
                &rrc_mac_ue_con_rej_req);

            l3_memcpy_wrapper( p_curr_pos,
                    p_uecc_con_re_establish_rej_req->data_buffer,
                    p_uecc_con_re_establish_rej_req->data_buffer_length);

            rrc_send_message(p_msg, 
					RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_uecc_con_re_establish_rej_req->cell_index));
		}
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_ue_con_rej_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function process UECC_LLIM_UE_CON_REJ_REQ message
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_ue_con_rej_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_con_rej_req_t *p_uecc_con_rej_req = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_DETAILED, 
            "Unused variable in fsm llim_gb_context=%p", p_rrc_llim_gl_ctx);       
    p_uecc_con_rej_req = (rrc_uecc_llim_con_rej_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_INDEX(p_uecc_con_rej_req->cell_index);

    /* Precondition - p_api should point to right message */
    if (    (rrc_get_api_buf_size(p_api) < RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_con_rej_req_t)) ||
            /* We have enough length for mandatory params, check data
             *  buffer size */
            rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_llim_con_rej_req_t) + p_uecc_con_rej_req->
                data_buffer_length)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    /* Special case connection reject */
    {
        void *p_msg = PNULL;
        /* Fill RRC_MAC_UE_CON_REJ_REQ message */
        rrc_mac_ue_con_rej_req_t mac_ue_con_rej_req;

        memset_wrapper (&mac_ue_con_rej_req, RRC_NULL, sizeof(rrc_mac_ue_con_rej_req_t));

        mac_ue_con_rej_req.tcrnti = p_uecc_con_rej_req->crnti;

        /* Allocate buffer for storing RRC_MAC_UE_CON_REJ_REQ message */
        p_msg = rrc_llim_alloc_external_msg(RRC_TRANSACTION_ID_ABSENT,
						RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_uecc_con_rej_req->cell_index),
						RRC_MAC_MODULE_ID,
                                            RRC_MAC_UE_CON_REJ_REQ,
                         (U16)(rrc_il_get_rrc_mac_ue_con_rej_req_len(&mac_ue_con_rej_req) + 
                             p_uecc_con_rej_req->data_buffer_length),
                         p_uecc_con_rej_req->cell_index);

        if (PNULL != p_msg)
        {
            U8 *p_curr_pos = (U8*)p_msg + RRC_FULL_INTERFACE_HEADERS_SIZE;
            /* Compose RRC_MAC_UE_CON_REJ_REQ */
            rrc_il_compose_rrc_mac_ue_con_rej_req(&p_curr_pos,
                &mac_ue_con_rej_req);

            l3_memcpy_wrapper( p_curr_pos,
                    p_uecc_con_rej_req->data_buffer,
                    p_uecc_con_rej_req->data_buffer_length);

            rrc_send_message(p_msg, 
				RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_uecc_con_rej_req->cell_index));
		}
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/*REL 1.2:HANDOVER PROCEDURE START*/
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_sn_hfn_status_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_SN_HFN_STATUS_REQ message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_sn_hfn_status_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_sn_hfn_status_req_t *p_sn_hfn_status_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_sn_hfn_status_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_sn_hfn_status_req = (rrc_uecc_llim_sn_hfn_status_req_t*)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_sn_hfn_status_req->cell_index, p_sn_hfn_status_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_sn_hfn_status_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_sn_hfn_status_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_sn_hfn_status_req->cell_index);
    }
    /* Search in DB UE with p_sn_hfn_status_req->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))
    {
        rrc_uecc_llim_sn_hfn_status_resp_t sn_hfn_status_resp;
        memset_wrapper(&sn_hfn_status_resp, 0, sizeof(sn_hfn_status_resp));

        fill_uecc_llim_sn_hfn_status_resp(&sn_hfn_status_resp, 
                RRC_LLIM_UE_IS_ABSENT, p_sn_hfn_status_req);

        rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_sn_hfn_status_req->ue_index),
                UECC_LLIM_SN_HFN_STATUS_RESP,
                sizeof(sn_hfn_status_resp), &sn_hfn_status_resp);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_SN_HFN_STATUS_REQ,
                    p_api,
                    p_sn_hfn_status_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_sn_hfn_status_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_ADD_LC_REQ message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_sn_hfn_status_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_sn_hfn_status_ind_t *p_sn_hfn_status_ind = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_sn_hfn_status_ind_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_sn_hfn_status_ind = (rrc_uecc_llim_sn_hfn_status_ind_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_sn_hfn_status_ind->cell_index, p_sn_hfn_status_ind->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_sn_hfn_status_ind->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_sn_hfn_status_ind->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_sn_hfn_status_ind->cell_index);
    }
    /* Search in DB UE with p_add_lc_req->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_sn_hfn_status_ind->ue_index);
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_SN_HFN_STATUS_IND,
                    p_api,
                    p_sn_hfn_status_ind,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_mac_i_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_MAC_I_REQ message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_mac_i_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_mac_i_req_t *p_mac_i_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_mac_i_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_mac_i_req = (rrc_uecc_llim_mac_i_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_mac_i_req->cell_index, p_mac_i_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_mac_i_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_mac_i_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_mac_i_req->cell_index);
    }
    /* Search in DB UE with p_mac_i_req->ue_index - should be present */
    else if((PNULL == p_ue_ctx))
    {
        rrc_uecc_llim_mac_i_resp_t mac_i_resp;
        memset_wrapper(&mac_i_resp, 0, sizeof(mac_i_resp));

        fill_uecc_llim_mac_i_resp(&mac_i_resp, RRC_LLIM_UE_IS_ABSENT,
                p_mac_i_req);

        rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_mac_i_req->ue_index),
                UECC_LLIM_MAC_I_RESP, 
                sizeof(mac_i_resp), &mac_i_resp);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_MAC_I_REQ,
                    p_api,
                    p_mac_i_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_data_buffer_stop_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_DATA_BUFFER_STOP_IND message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_data_buffer_stop_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_data_buffer_stop_ind_t *p_data_buffer_stop_ind = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_data_buffer_stop_ind_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_data_buffer_stop_ind = (rrc_uecc_llim_data_buffer_stop_ind_t *)
        ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_data_buffer_stop_ind->cell_index, p_data_buffer_stop_ind->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_data_buffer_stop_ind->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_data_buffer_stop_ind->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_data_buffer_stop_ind->cell_index);
    }
    /*Search in DB UE with p_data_buffer_stop_ind->ue_index-should be present*/
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_data_buffer_stop_ind->ue_index);
        RRC_LLIM_UT_TRACE_EXIT();
        return; 
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {

            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_DATA_BUFFER_STOP_IND,
                    p_api,
                    p_data_buffer_stop_ind,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_s1u_reconfigure_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_S1U_RECONFIGURE_REQ message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_s1u_reconfigure_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_s1u_reconfigure_req_t *p_s1u_reconfigure_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_s1u_reconfigure_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_s1u_reconfigure_req = (rrc_uecc_llim_s1u_reconfigure_req_t *)
        ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_s1u_reconfigure_req->cell_index, p_s1u_reconfigure_req->ue_index);

    /* Check for correct data */
    if (RRC_FAILURE == 
        check_uecc_llim_s1u_reconfigure_req(p_s1u_reconfigure_req))
    {
        rrc_uecc_llim_s1u_reconfigure_cnf_t s1u_reconfigure_cnf;
        memset_wrapper(&s1u_reconfigure_cnf, 0, sizeof(s1u_reconfigure_cnf));

         fill_uecc_llim_s1u_reconfigure_cnf(&s1u_reconfigure_cnf, 
            RRC_LLIM_INCORRECT_DATA_PASSED, p_s1u_reconfigure_req);

        rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_s1u_reconfigure_req->ue_index), 
            UECC_LLIM_S1U_RECONFIGURE_CNF,
                           sizeof(s1u_reconfigure_cnf), &s1u_reconfigure_cnf);

        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message data - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_s1u_reconfigure_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_s1u_reconfigure_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_s1u_reconfigure_req->cell_index);
    }
    /* Search in DB UE with p_s1u_reconfigure_req->ue_index-should be present */
    else if ((PNULL == p_ue_ctx))
    {
        rrc_uecc_llim_s1u_reconfigure_cnf_t s1u_reconfigure_cnf;
        memset_wrapper(&s1u_reconfigure_cnf, 0, sizeof(s1u_reconfigure_cnf));

        fill_uecc_llim_s1u_reconfigure_cnf (&s1u_reconfigure_cnf, 
                RRC_LLIM_UE_IS_ABSENT, p_s1u_reconfigure_req);

        rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_s1u_reconfigure_req->ue_index),
                UECC_LLIM_S1U_RECONFIGURE_CNF,
                sizeof(s1u_reconfigure_cnf), &s1u_reconfigure_cnf);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* SPR 16087 Start */
            if(RRC_S1U_END_MARKER_RESERVED_TRANSACTION_ID ==
                    p_s1u_reconfigure_req->transaction_id)
            {
                uemh_send_s1u_end_marker_req(p_s1u_reconfigure_req);
            }
            else
            {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_S1U_RECONFIG_UE_ENTITY_REQ,
                    p_api,
                    p_s1u_reconfigure_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
            /* SPR 16087 Stop */
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_ue_inactive_time_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially processes
*                  UECC_LLIM_UE_INACTIVE_TIME_REQ message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_ue_inactive_time_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_ue_inactive_time_req_t *p_ue_inactive_time_req  = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx                                 = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_ue_inactive_time_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_ue_inactive_time_req = (rrc_uecc_llim_ue_inactive_time_req_t *)
        ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_ue_inactive_time_req->cell_index, p_ue_inactive_time_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_ue_inactive_time_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_ue_inactive_time_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_ue_inactive_time_req->cell_index);
    }

    /*Search in DB UE with p_ue_inactive_time_req->ue_index -should be present*/
    else if ((PNULL == p_ue_ctx))
    {
        /* UE context is not found - send response with failure response code */
        rrc_uecc_llim_ue_inactive_time_resp_t ue_inactive_time_resp;
        memset_wrapper(&ue_inactive_time_resp, 0, sizeof(ue_inactive_time_resp));

        fill_uecc_llim_ue_inactive_time_resp (&ue_inactive_time_resp, 
                RRC_LLIM_UE_IS_ABSENT, p_ue_inactive_time_req);

        rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_ue_inactive_time_req->ue_index),
                UECC_LLIM_UE_INACTIVE_TIME_RESP,
                sizeof(ue_inactive_time_resp),
                &ue_inactive_time_resp);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_UE_INACTIVE_TIME_REQ,
                    p_api,
                    p_ue_inactive_time_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_ho_rach_resource_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_HO_RACH_RESOURCE_REQ message
*                  and call FSM.
*
*   RETURNS     : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_ho_rach_resource_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_ho_rach_resource_resp_t ho_rach_resource_resp;
    rrc_uecc_llim_ho_rach_resource_req_t *p_ho_rach_resource_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_ho_rach_resource_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_ho_rach_resource_req = (rrc_uecc_llim_ho_rach_resource_req_t *)
        ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_ho_rach_resource_req->cell_index, p_ho_rach_resource_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_ho_rach_resource_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_ho_rach_resource_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_ho_rach_resource_req->cell_index);
    }

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_ho_rach_resource_req->ue_index, &p_ue_ctx);

    if (RRC_UECC_LLIM_INTRA_ENB_HO_REQ_PRESENT & p_ho_rach_resource_req->bitmask)
    {
        if ( PNULL == p_ue_ctx )
        {
            RRC_LLIM_TRACE(RRC_WARNING, "Invalid UE index!!");

            memset_wrapper(&ho_rach_resource_resp, 0, sizeof(ho_rach_resource_resp));

            fill_uecc_llim_ho_rach_resource_resp(&ho_rach_resource_resp, 
                    RRC_LLIM_UE_IS_ABSENT, p_ho_rach_resource_req);

            rrc_llim_send_internal_msg( 
                    RRC_GET_UECC_MODULE_ID(ho_rach_resource_resp.ue_index),
                    UECC_LLIM_HO_RACH_RESOURCE_RESP,
                    sizeof(ho_rach_resource_resp), &ho_rach_resource_resp);
        }
        else
        {
            /* Normal message processing */
            uemh_fsm_process_event(UEMH_EVENT_UECC_HO_RACH_RESOURCE_REQ,       
                    p_api,
                    p_ho_rach_resource_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    else
    {
        if ( PNULL != p_ue_ctx )
        {
            RRC_LLIM_TRACE(RRC_WARNING, "Invalid UE index!!");

            memset_wrapper(&ho_rach_resource_resp, 0, sizeof(ho_rach_resource_resp));

            fill_uecc_llim_ho_rach_resource_resp(&ho_rach_resource_resp, 
                    RRC_LLIM_UE_ALREADY_EXIST, p_ho_rach_resource_req);

            rrc_llim_send_internal_msg(
                    RRC_GET_UECC_MODULE_ID(ho_rach_resource_resp.ue_index),
                    UECC_LLIM_HO_RACH_RESOURCE_RESP,
                    sizeof(ho_rach_resource_resp), &ho_rach_resource_resp);
        }
        else
        {
            /* Create and add to DB */
            p_ue_ctx = (rrc_llim_ue_ctx_t *)rrc_mem_get(sizeof(rrc_llim_ue_ctx_t));

            if(PNULL == p_ue_ctx)
            {
                RRC_LLIM_TRACE(RRC_FATAL, "rrc_mem_get failed!!");
                return;
            }

            /* Successful case */
            memset_wrapper(p_ue_ctx, 0, sizeof(*p_ue_ctx));
            rrc_llim_set_ue_context(p_rrc_llim_gl_ctx, p_ho_rach_resource_req->ue_index, p_ue_ctx);
            
            /* Fill UE index. Only for logging purposes */
            p_ue_ctx->ue_index = p_ho_rach_resource_req->ue_index;

            /* CELL INDEX CHANGES START */
            /* Initializing UE context*/
            p_ue_ctx->cell_index = p_ho_rach_resource_req->cell_index;

            /* Initialize al pointers */
            p_ue_ctx->curr_op_param.p_create_ue         = PNULL;
            p_ue_ctx->curr_op_param.p_del_ue            = PNULL;
            p_ue_ctx->curr_op_param.p_rcfg_ue           = PNULL;
            p_ue_ctx->curr_op_param.p_cfg_ue_security   = PNULL;
            p_ue_ctx->curr_op_param.p_rach_res_req      = PNULL;
            p_ue_ctx->curr_op_param.p_suspend_ue_req    = PNULL;
            p_ue_ctx->curr_op_param.p_reestablish_ue    = PNULL;
            p_ue_ctx->curr_op_param.p_recfg_ue_security = PNULL;
            p_ue_ctx->curr_op_param.p_resume_ue         = PNULL;
            p_ue_ctx->curr_op_param.p_change_crnti      = PNULL;

            /* CELL INDEX CHANGES STOP */

            /* Init main IEs of UE context */
            UEMH_FSM_INIT(p_ue_ctx);

            /* Normal message processing */
            uemh_fsm_process_event(UEMH_EVENT_UECC_HO_RACH_RESOURCE_REQ,       
                    p_api,
                    p_ho_rach_resource_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
      
           
   }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_ho_rel_rach_resource_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_HO_REL_RACH_RESOURCE_IND 
*                   message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_ho_rel_rach_resource_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_ho_rach_rel_resource_ind_t *p_ho_rach_rel_resource_ind = 
       PNULL;
    /* SPR 16782 Fix Start */
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    /* SPR 16782 Fix Stop */

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_ho_rach_rel_resource_ind_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_ho_rach_rel_resource_ind = (rrc_uecc_llim_ho_rach_rel_resource_ind_t *)
        ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_ho_rach_rel_resource_ind->cell_index, p_ho_rach_rel_resource_ind->ue_index);

    /* No fsm is invoked , it is forwarded to MAC */
    uemh_send_mac_ho_rel_rach_res_ind (p_ho_rach_rel_resource_ind,
            p_rrc_llim_gl_ctx);

    /* SPR 16782 Fix Start */
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_ho_rach_rel_resource_ind->ue_index, &p_ue_ctx);
    if (PNULL == p_ue_ctx)
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_ho_rach_rel_resource_ind->ue_index);

    }
    else
    {
        /* delete from UE DB and free */
        rrc_llim_set_ue_context(p_rrc_llim_gl_ctx, p_ue_ctx->ue_index,
            PNULL);        

        rrc_mem_free(p_ue_ctx);
        p_ue_ctx = PNULL;
    }
    /* SPR 16782 Fix Stop */

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_re_establish_ue_entity_req_process_msg 
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ 
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_re_establish_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_re_establish_ue_entity_req_t *p_re_establish_ue_entity_req = 
       PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_re_establish_ue_entity_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_re_establish_ue_entity_req = 
        (rrc_uecc_llim_re_establish_ue_entity_req_t *)
            ((U8* )p_api + RRC_API_HEADER_SIZE);
  
    SET_CELL_AND_UE_INDEX(p_re_establish_ue_entity_req->cell_index, p_re_establish_ue_entity_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_re_establish_ue_entity_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_re_establish_ue_entity_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_re_establish_ue_entity_req->cell_index);
    }

    /* Search in DB UE with p_re_establish_ue_entity_req->ue_index - 
       should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_re_establish_ue_entity_req->ue_index);

    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_RE_ESTABLISH_UE_ENTITY_REQ,
                    p_api,
                    p_re_establish_ue_entity_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_reestablish_ue_entity_req_process_msg 
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_RE_ESTABLISH_UE_ENTITY_REQ 
*                   message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_reestablish_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_re_establish_ue_entity_req_t *p_re_establish_ue_entity_req = 
       PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_re_establish_ue_entity_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_re_establish_ue_entity_req = 
        (rrc_uecc_llim_re_establish_ue_entity_req_t *)
            ((U8* )p_api + RRC_API_HEADER_SIZE);
   
    SET_CELL_AND_UE_INDEX(p_re_establish_ue_entity_req->cell_index, p_re_establish_ue_entity_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_re_establish_ue_entity_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_re_establish_ue_entity_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_re_establish_ue_entity_req->cell_index);
    }

    /* Search in DB UE with p_re_establish_ue_entity_req->ue_index - 
       should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_re_establish_ue_entity_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_REESTABLISHMENT_UE_ENTITY_REQ,
                    p_api,
                    p_re_establish_ue_entity_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_resume_ue_entity_req_process_msg 
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_RESUME_UE_ENTITY_REQ 
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_resume_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_resume_ue_entity_req_t *p_resume_ue_entity_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_resume_ue_entity_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_resume_ue_entity_req = 
        (rrc_uecc_llim_resume_ue_entity_req_t *)
            ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_resume_ue_entity_req->cell_index, p_resume_ue_entity_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_resume_ue_entity_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_resume_ue_entity_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_resume_ue_entity_req->cell_index);
    }

    /* Search in DB UE with p_resume_ue_entity_req->ue_index - 
       should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_resume_ue_entity_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event(UEMH_EVENT_UECC_RESUME_UE_ENTITY_REQ,
                    p_api,
                    p_resume_ue_entity_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_reconfig_security_req_process_msg 
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_RECONFIG_SECURITY_REQ 
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_reconfig_security_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_reconfig_security_req_t *p_reconfig_security_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_reconfig_security_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_reconfig_security_req = 
        (rrc_uecc_llim_reconfig_security_req_t *)
            ((U8* )p_api + RRC_API_HEADER_SIZE);
 
    SET_CELL_AND_UE_INDEX(p_reconfig_security_req->cell_index, p_reconfig_security_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_reconfig_security_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_reconfig_security_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_reconfig_security_req->cell_index);
    }

    /* Search in DB UE with p_reconfig_security_req->ue_index - 
       should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_reconfig_security_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event(UEMH_EVENT_UECC_RECONFIG_SECURITY_UE_ENTITY_REQ,
                    p_api,
                    p_reconfig_security_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_change_crnti_req_process_msg 
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_CHANGE_CRNTI_REQ 
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_change_crnti_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_change_crnti_req_t *p_change_crnti_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_change_crnti_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_change_crnti_req = 
        (rrc_uecc_llim_change_crnti_req_t*)
            ((U8* )p_api + RRC_API_HEADER_SIZE);
    
    SET_CELL_AND_UE_INDEX(p_change_crnti_req->cell_index, p_change_crnti_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_change_crnti_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_change_crnti_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_change_crnti_req->cell_index);
    }

    /* Search in DB UE with p_change_crnti_req->ue_index - 
       should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_change_crnti_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event(UEMH_EVENT_UECC_CHANGE_CRNTI_REQ,
                    p_api,
                    p_change_crnti_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/*REL 1.2:HANDOVER PROCEDURE END*/

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_phy_create_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_PHY_CREATE_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_phy_create_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_phy_create_ue_entity_cnf_t phy_create_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_phy_create_ue_entity_cnf(
                                        &phy_create_ue_entity_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PHY->RRC:RRC_PHY_CREATE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(phy_create_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PHY->RRC:RRC_PHY_CREATE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   phy_create_ue_entity_cnf.ue_index, phy_create_ue_entity_cnf.response);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, phy_create_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                phy_create_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PHY_CREATE_UE_ENTITY_CNF,
                    p_api,
                    &phy_create_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_phy_delete_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_PHY_DELETE_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_phy_delete_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_phy_delete_ue_entity_cnf_t phy_delete_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_phy_delete_ue_entity_cnf(
                                        &phy_delete_ue_entity_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PHY->RRC:RRC_PHY_DELETE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(phy_delete_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PHY->RRC:RRC_PHY_DELETE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   phy_delete_ue_entity_cnf.ue_index, phy_delete_ue_entity_cnf.response);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, phy_delete_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                           cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                phy_delete_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PHY_DELETE_UE_ENTITY_CNF,
                    p_api,
                    &phy_delete_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);

        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_phy_reconfig_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_PHY_RECONFIG_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_phy_reconfig_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_phy_reconfig_ue_entity_cnf_t phy_reconfig_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_phy_reconfig_ue_entity_cnf(
                                        &phy_reconfig_ue_entity_cnf,
                                        (U8* )p_api
                                        +
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PHY->RRC:RRC_PHY_RECONFIG_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(phy_reconfig_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PHY->RRC:RRC_PHY_RECONFIG_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   phy_reconfig_ue_entity_cnf.ue_index, phy_reconfig_ue_entity_cnf.response);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, phy_reconfig_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                phy_reconfig_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PHY_RECONFIG_UE_ENTITY_CNF,
                    p_api,
                    &phy_reconfig_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_mac_create_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION: This function partially process RRC_MAC_CREATE_UE_ENTITY_CNF message
*                and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_mac_create_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_mac_create_ue_entity_cnf_t mac_create_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_create_ue_entity_cnf(
                                            &mac_create_ue_entity_cnf,
                                            (U8* )p_api
                                            +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                            rrc_get_api_buf_size(p_api) -
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                            &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:MAC->RRC:RRC_MAC_CREATE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(mac_create_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:MAC->RRC:RRC_MAC_CREATE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   mac_create_ue_entity_cnf.ue_index, mac_create_ue_entity_cnf.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, mac_create_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                mac_create_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_MAC_CREATE_UE_ENTITY_CNF,
                    p_api,
                    &mac_create_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_mac_delete_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_MAC_DELETE_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_mac_delete_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_mac_delete_ue_entity_cnf_t mac_delete_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_delete_ue_entity_cnf(
                                        &mac_delete_ue_entity_cnf,
                                        (U8* )p_api
                                        +
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:MAC->RRC:RRC_MAC_DELETE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(mac_delete_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:MAC->RRC:RRC_MAC_DELETE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   mac_delete_ue_entity_cnf.ue_index, mac_delete_ue_entity_cnf.response );

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, mac_delete_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if( (RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                mac_delete_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_MAC_DELETE_UE_ENTITY_CNF,
                    p_api,
                    &mac_delete_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_mac_reconfigure_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_MAC_RECONFIGURE_UE_ENTITY_CNF
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_mac_reconfigure_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_mac_reconfigure_ue_entity_cnf_t mac_reconfigure_ue_entity_cnf;
    rrc_uecc_llim_meas_config_resp_t  *p_uecc_llim_meas_config_resp = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_reconfigure_ue_entity_cnf(
                                        &mac_reconfigure_ue_entity_cnf,
                                        (U8*)p_api +
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:MAC->RRC:RRC_MAC_RECONFIG_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(mac_reconfigure_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:MAC->RRC:RRC_MAC_RECONFIG_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   mac_reconfigure_ue_entity_cnf.ue_index, mac_reconfigure_ue_entity_cnf.response_code);
    
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, mac_reconfigure_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                mac_reconfigure_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Vimal: meas config changes */
            /* Check for:Is meas config transaction id present */
            if (RRC_TRUE == p_ue_ctx->curr_op_param.meas_config.
                    transaction_id_presence)
            {
                /* check stored transaction id for meas config 
                 * should be matched */
                if (rrc_get_transaction_id(p_api) ==
                        p_ue_ctx->curr_op_param.meas_config.transaction_id)
                {
                    void *p_msg = rrc_llim_alloc_internal_msg(
                            RRC_UECC_MODULE_ID,
                            UECC_LLIM_MEAS_CONFIG_RESP,
                            (U16)(sizeof(rrc_uecc_llim_meas_config_resp_t)));

                    if (PNULL != p_msg)
                    {
                        p_uecc_llim_meas_config_resp = 
                            (rrc_uecc_llim_meas_config_resp_t*)((U8*)p_msg + RRC_API_HEADER_SIZE);

                        /* Transaction id */
                        p_uecc_llim_meas_config_resp->transaction_id = 
                            p_ue_ctx->curr_op_param.meas_config.transaction_id;

                        /* Ue Index */
                        p_uecc_llim_meas_config_resp->ue_index =
                            mac_reconfigure_ue_entity_cnf.ue_index;

                        /* Cell Index */
                        p_uecc_llim_meas_config_resp->cell_index = cell_index;
                        if (RRC_SUCCESS == mac_reconfigure_ue_entity_cnf.response_code)
                        {
                            p_uecc_llim_meas_config_resp->response_code = 
                                RRC_SUCCESS;   
                        }
                        else
                        {
                            p_uecc_llim_meas_config_resp->response_code = RRC_FAILURE;
                        }

                        rrc_send_message(p_msg, RRC_GET_UECC_MODULE_ID
                                (p_uecc_llim_meas_config_resp->ue_index));
                    }
                    p_ue_ctx->curr_op_param.meas_config.
                        transaction_id_presence = RRC_FALSE;
                }
                else 
                {
                    /* ignore */
                    RRC_LLIM_TRACE(RRC_WARNING, 
                            "Message - with other tranaction id ignored"
                            "expecting for meas config mac confirmation");
                }
            }
            else
            {
                /* Normal message processing */
                uemh_fsm_process_event( UEMH_EVENT_MAC_RECONFIG_UE_ENTITY_CNF,
                        p_api,
                        &mac_reconfigure_ue_entity_cnf,
                        p_ue_ctx,
                        p_rrc_llim_gl_ctx);
            }
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_mac_ccch_msg_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function process RRC_MAC_CCCH_MSG_IND message.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_mac_ccch_msg_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_mac_ccch_msg_ind_t  mac_ccch_msg_ind;
    rrc_counter_t           data_buffer_length  = RRC_NULL;
    S32                     length_read         = RRC_NULL;
    rrc_cell_index_t        cell_index          = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_ccch_msg_ind(   &mac_ccch_msg_ind,
                                        (U8* )p_api +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size
                                        (p_api) -
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:MAC->RRC:RRC_MAC_CCCH_MSG_IND:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    RRC_LLIM_TRACE(RRC_INFO,"API:MAC->RRC:RRC_MAC_CCCH_MSG_IND:[RNTI:%u]",
                                mac_ccch_msg_ind.rnti);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
        /*SPR 20775 Fix Start*/
        return;
        /*SPR 20775 Fix Stop*/
    }

    data_buffer_length =(rrc_counter_t)( rrc_get_api_buf_size(p_api) -
        (RRC_FULL_INTERFACE_HEADERS_SIZE + length_read));

    /* Special case CCCH */
    {
        void *p_msg = rrc_llim_alloc_internal_msg(  RRC_UECC_MODULE_ID,
                      UECC_LLIM_CCCH_DATA_IND,
                      (U16)( sizeof
                           (rrc_uecc_llim_ccch_data_ind_t)+ data_buffer_length));

        if (PNULL != p_msg)
        {
            rrc_uecc_llim_ccch_data_ind_t *p_uecc_ccch_data_ind =
              (rrc_uecc_llim_ccch_data_ind_t *)((U8*)p_msg+RRC_API_HEADER_SIZE);
            p_uecc_ccch_data_ind->cell_index = cell_index;
            p_uecc_ccch_data_ind->rnti = mac_ccch_msg_ind.rnti;

            /* CLPC Start */

            p_uecc_ccch_data_ind->timing_advance = mac_ccch_msg_ind.timing_advance;
            p_uecc_ccch_data_ind->sinr = mac_ccch_msg_ind.sinr;

            /* CLPC Stop */

            p_uecc_ccch_data_ind->data_buffer_length = data_buffer_length;

            l3_memcpy_wrapper( p_uecc_ccch_data_ind->data_buffer,
                    (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE + length_read,
                    data_buffer_length);

            rrc_send_message(p_msg, RRC_UECCMD_MODULE_ID);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_rlc_create_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_RLC_CREATE_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_rlc_create_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_rlc_create_ue_entity_cnf_t rlc_create_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_rlc_create_ue_entity_cnf(
                                &rlc_create_ue_entity_cnf,
                                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:RLC->RRC:RRC_RLC_CREATE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    
    SET_UE_INDEX(rlc_create_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:RLC->RRC:RRC_RLC_CREATE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   rlc_create_ue_entity_cnf.ue_index, rlc_create_ue_entity_cnf.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, rlc_create_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                rlc_create_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_RLC_CREATE_UE_ENTITY_CNF,
                    p_api,
                    &rlc_create_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_rlc_reconfig_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_RLC_RECONFIG_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_rlc_reconfig_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_rlc_reconfig_ue_entity_cnf_t rlc_reconfig_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_rlc_reconfig_ue_entity_cnf(
                                    &rlc_reconfig_ue_entity_cnf,
                                    (U8* )p_api+RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    rrc_get_api_buf_size(p_api) -
                                    RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:RLC->RRC:RRC_RLC_RECONFIG_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(rlc_reconfig_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:RLC->RRC:RRC_RLC_RECONFIG_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   rlc_reconfig_ue_entity_cnf.ue_index, rlc_reconfig_ue_entity_cnf.response_code);
 
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, rlc_reconfig_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }
    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                rlc_reconfig_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_RLC_RECONFIG_UE_ENTITY_CNF,
                    p_api,
                    &rlc_reconfig_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_rlc_delete_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_RLC_DELETE_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_rlc_delete_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_rlc_delete_ue_entity_cnf_t rlc_delete_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_rlc_delete_ue_entity_cnf(
                                    &rlc_delete_ue_entity_cnf,
                                    (U8* )p_api+RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    rrc_get_api_buf_size(p_api) -
                                    RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:RLC->RRC:RRC_RLC_DELETE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(rlc_delete_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:RLC->RRC:RRC_RLC_DELETE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   rlc_delete_ue_entity_cnf.ue_index, rlc_delete_ue_entity_cnf.response);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, rlc_delete_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                rlc_delete_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_RLC_DELETE_UE_ENTITY_CNF,
                    p_api,
                    &rlc_delete_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_create_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_PDCP_CREATE_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_create_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_pdcp_cr_ue_entity_cnf_t pdcp_cr_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_cr_ue_entity_cnf(
                                            &pdcp_cr_ue_entity_cnf,
                                            (U8* )p_api +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                            rrc_get_api_buf_size(p_api) -
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                            &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_CREATE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
   
    SET_UE_INDEX(pdcp_cr_ue_entity_cnf.ue_index);


    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_CREATE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   pdcp_cr_ue_entity_cnf.ue_index, pdcp_cr_ue_entity_cnf.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_cr_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                pdcp_cr_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PDCP_CREATE_UE_ENTITY_CNF,
                    p_api,
                    &pdcp_cr_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_delete_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION:
*       This function partially process RRC_PDCP_DELETE_UE_ENTITY_CNF message
*       and call FSM.
*
*   RETURNS:
*       None
*
******************************************************************************/
void rrc_uemh_pdcp_delete_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_pdcp_del_ue_entity_cnf_t pdcp_del_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_del_ue_entity_cnf(
                                &pdcp_del_ue_entity_cnf,
                                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_DELETE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
   
    SET_UE_INDEX(pdcp_del_ue_entity_cnf.ue_index);


    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_DELETE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   pdcp_del_ue_entity_cnf.ue_index, pdcp_del_ue_entity_cnf.response);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_del_ue_entity_cnf.ue_index, &p_ue_ctx);
    
    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                pdcp_del_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PDCP_DELETE_UE_ENTITY_CNF,
                    p_api,
                    &pdcp_del_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_reconfig_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_PDCP_RECONFIG_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_reconfig_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_pdcp_reconfig_ue_entity_cnf_t pdcp_reconfig_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_reconfig_ue_entity_cnf(
                                &pdcp_reconfig_ue_entity_cnf,
                                (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_RECONFIG_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(pdcp_reconfig_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_RECONFIG_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                   pdcp_reconfig_ue_entity_cnf.ue_index, pdcp_reconfig_ue_entity_cnf.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_reconfig_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                pdcp_reconfig_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PDCP_RECONFIG_UE_ENTITY_CNF,
                    p_api,
                    &pdcp_reconfig_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_srb_data_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function process RRC_PDCP_SRB_DATA_IND message.
*
*   RETURNS      :  None
*
******************************************************************************/
void rrc_uemh_pdcp_srb_data_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_pdcp_srb_data_ind_t pdcp_srb_data_ind;
    rrc_counter_t           data_buffer_length  = RRC_NULL;
    S32                     length_read         = RRC_NULL;
    rrc_llim_ue_ctx_t       *p_ue_ctx           = PNULL;
    rrc_cell_index_t        cell_index          = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_srb_data_ind(  &pdcp_srb_data_ind,
                                (U8* )p_api +RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size
                                (p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_SRB_DATA_IND:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_SRB_DATA_IND:[UE:%u]",
                                pdcp_srb_data_ind.ue_index);

    data_buffer_length =(rrc_counter_t)( rrc_get_api_buf_size(p_api) -
        (RRC_FULL_INTERFACE_HEADERS_SIZE + length_read));

    SET_UE_INDEX(pdcp_srb_data_ind.ue_index);
    
    RRC_LLIM_TRACE(RRC_BRIEF,"[UE_INDEX:%u]", pdcp_srb_data_ind.ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_srb_data_ind.ue_index, &p_ue_ctx);
    
    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with p_pdcp_srb_data_ind->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))

    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                pdcp_srb_data_ind.ue_index);
    }
    else
    {
        void *p_msg = rrc_llim_alloc_internal_msg(  RRC_UECC_MODULE_ID,
                UECC_LLIM_SRB_DATA_IND,
                (U16)(sizeof
                    (rrc_uecc_llim_srb_data_ind_t) + data_buffer_length));

        if (PNULL != p_ue_ctx)
        {
            if (PNULL != p_msg)
            {
                rrc_uecc_llim_srb_data_ind_t *p_uecc_srb_data_ind =
                    (rrc_uecc_llim_srb_data_ind_t *)((U8*)p_msg+RRC_API_HEADER_SIZE);

                p_uecc_srb_data_ind->cell_index         = cell_index;
                p_uecc_srb_data_ind->ue_index           = pdcp_srb_data_ind.
                    ue_index;
                p_uecc_srb_data_ind->lc_id              = pdcp_srb_data_ind.lc_id;
                p_uecc_srb_data_ind->data_buffer_length = data_buffer_length;

                l3_memcpy_wrapper( p_uecc_srb_data_ind->data_buffer,
                        (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE + length_read,
                        data_buffer_length);

                rrc_send_message(p_msg, RRC_GET_UECC_MODULE_ID
                        (p_uecc_srb_data_ind->ue_index));
            }
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_srb_data_status_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function process RRC_PDCP_SRB_DATA_STATUS_IND message.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_srb_data_status_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    void *p_msg = PNULL;
    
    rrc_uecc_llim_srb_data_status_ind_t *uecc_pdcp_srb_data_status_ind = PNULL;
   
   rrc_pdcp_srb_data_status_ind_t       pdcp_srb_data_status_ind;
    S32                                 length_read = RRC_NULL;
    rrc_llim_ue_ctx_t                   *p_ue_ctx = PNULL;
    rrc_cell_index_t                    cell_index = RRC_INVALID_CELL_INDEX;
    
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Coverity Fix 83183 Start */
    memset_wrapper(&pdcp_srb_data_status_ind, RRC_NULL,
                   sizeof(rrc_pdcp_srb_data_status_ind_t));
    /* Coverity Fix 83183 Stop */

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_srb_data_status_ind(
                                &pdcp_srb_data_status_ind,
                                (U8* )p_api+RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_SRB_DATA_STATUS_IND:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(pdcp_srb_data_status_ind.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_SRB_DATA_STATUS_IND:[UE:%u]",
                                pdcp_srb_data_status_ind.ue_index);
    
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_srb_data_status_ind.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    if (PNULL != p_ue_ctx)
    {
        /* Check whether UE's Cell exists or not */
        if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
                (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
        {
            RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                    cell_index);
        }

        /* Search in DB UE with pdcp_srb_data_status_ind.ue_index -
         *  should be present */
        else if ((PNULL == p_ue_ctx))
        {
            /* ignore */
            RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                    pdcp_srb_data_status_ind.ue_index);
        }
        else
        {
            rrc_counter_t data_length           = 0;
            rrc_counter_t data_buffer_length    = 0;

            if (pdcp_srb_data_status_ind.optional_elems_present &
                    RRC_PDCP_SRB_DATA_STATUS_ERROR_PRESENT)
            {
                data_length = (rrc_counter_t)(sizeof(U32) + sizeof(rrc_ue_index_t)
                        + sizeof(rrc_lc_id_t)
                        + sizeof(rrc_response_t) + sizeof(rrc_tag_t)
                        + sizeof(rrc_length_t) + sizeof(U16));

                data_buffer_length =(rrc_counter_t) (rrc_get_api_buf_size(p_api) -
                        (RRC_FULL_INTERFACE_HEADERS_SIZE + data_length));
            }

            p_msg = rrc_llim_alloc_internal_msg(  RRC_UECC_MODULE_ID,
                    UECC_LLIM_SRB_DATA_STATUS_IND,
                    (U16) (sizeof
                        (rrc_uecc_llim_srb_data_status_ind_t) + data_buffer_length));

            if (PNULL != p_msg)
            {
                uecc_pdcp_srb_data_status_ind =
                    (rrc_uecc_llim_srb_data_status_ind_t *)((U8*)p_msg +
                            RRC_API_HEADER_SIZE);

                uecc_pdcp_srb_data_status_ind->cell_index       = cell_index;
                uecc_pdcp_srb_data_status_ind->bitmask          = 0;

                uecc_pdcp_srb_data_status_ind->transaction_id   =
                    (rrc_transaction_id_t)pdcp_srb_data_status_ind.transaction_id;
                uecc_pdcp_srb_data_status_ind->ue_index         =
                    pdcp_srb_data_status_ind.ue_index;
                uecc_pdcp_srb_data_status_ind->lc_id            =
                    pdcp_srb_data_status_ind.lc_id;
                uecc_pdcp_srb_data_status_ind->response_code    =
                    pdcp_srb_data_status_ind.response_code;

                if (pdcp_srb_data_status_ind.optional_elems_present &
                        RRC_PDCP_SRB_DATA_STATUS_ERROR_PRESENT)
                {
                    uecc_pdcp_srb_data_status_ind->bitmask |=
                        RRC_UECC_LLIM_DATA_STATUS_ERROR_PRESENT;

                    uecc_pdcp_srb_data_status_ind->srb_data_status_error.response =
                        pdcp_srb_data_status_ind.srb_data_status_error.error_code;
                    uecc_pdcp_srb_data_status_ind->srb_data_status_error.
                        data_buffer_length =
                        data_buffer_length;

                    l3_memcpy_wrapper( uecc_pdcp_srb_data_status_ind->srb_data_status_error
                            .data_buffer,
                            (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE +
                            data_length,
                            data_buffer_length);
                }

                rrc_send_message(p_msg, RRC_GET_UECC_MODULE_ID
                        (uecc_pdcp_srb_data_status_ind->ue_index));
            }
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_s1u_create_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_S1U_CREATE_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_s1u_create_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_s1u_create_ue_entity_cnf_t s1u_create_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t  cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_s1u_create_ue_entity_cnf(
                                    &s1u_create_ue_entity_cnf,
                                    (U8* )p_api+RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    rrc_get_api_buf_size(p_api) -
                                    RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:S1U->RRC:RRC_S1U_CREATE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(s1u_create_ue_entity_cnf.ue_index);
    
    RRC_LLIM_TRACE(RRC_INFO,"API:S1U->RRC:RRC_S1U_CREATE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
                    s1u_create_ue_entity_cnf.ue_index,
                    s1u_create_ue_entity_cnf.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, s1u_create_ue_entity_cnf.ue_index, &p_ue_ctx);
   
    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with s1u_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                s1u_create_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_S1U_CREATE_UE_ENTITY_CNF,
                    p_api,
                    &s1u_create_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_s1u_delete_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_S1U_DELETE_UE_ENTITY_CNF message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_s1u_delete_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_s1u_delete_ue_entity_cnf_t s1u_delete_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_s1u_delete_ue_entity_cnf(
                                    &s1u_delete_ue_entity_cnf,
                                    (U8* )p_api+RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    rrc_get_api_buf_size(p_api) -
                                    RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:S1U->RRC:RRC_S1U_DELETE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(s1u_delete_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:S1U->RRC:RRC_S1U_DELETE_UE_ENTITY_CNF:[UE:%u] with Response:%u",
          s1u_delete_ue_entity_cnf.ue_index, s1u_delete_ue_entity_cnf.response_code);
    
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, s1u_delete_ue_entity_cnf.ue_index, &p_ue_ctx);
    
    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with s1u_delete_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                s1u_delete_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_S1U_DELETE_UE_ENTITY_CNF,
                    p_api,
                    &s1u_delete_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_s1u_reconfigure_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION : This function partially process RRC_S1U_RECONFIGURE_UE_ENTITY_CNF
*                 message and call FSM.
*
*   RETURNS     : None
*
******************************************************************************/
void rrc_uemh_s1u_reconfigure_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_s1u_reconfigure_ue_entity_cnf_t s1u_reconfigure_ue_entity_cnf;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_s1u_reconfigure_ue_entity_cnf(
                                &s1u_reconfigure_ue_entity_cnf,
                                (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:S1U->RRC:RRC_S1U_RECONFIGURE_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(s1u_reconfigure_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:S1U->RRC:RRC_S1U_RECONFIGURE_UE_ENTITY_CNF:[UE:%u]",
                        s1u_reconfigure_ue_entity_cnf.ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, s1u_reconfigure_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with s1u_reconfigure_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                s1u_reconfigure_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_S1U_RECONFIG_UE_ENTITY_CNF,
                    p_api,
                    &s1u_reconfigure_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_oamh_cleanup_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function processes LLIM_OAMH_CLEANUP_REQ message.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_oamh_cleanup_req_process_msg
(
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_llim_ue_ctx_t *p_ue_ctx          = PNULL;
    rrc_ue_index_t ue_index              = RRC_NULL;
    U8        uecc_index = RRC_NULL;              
    U16       total_ue_supported = RRC_NULL;
    U16       cell_index = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    
    /*coverity fix start*/
    /*coverity fix stop*/

    if (p_rrc_llim_gl_ctx->p_p_uecc_ctx != PNULL)
    {
        for (cell_index=0; cell_index<MAX_NUM_CELLS; cell_index++)
        {
            total_ue_supported = (U16)(total_ue_supported + p_rrc_llim_gl_ctx->
                max_num_supported_ue[cell_index]);
        }
        /* For each UECC */
        for(uecc_index=0; uecc_index<p_rrc_llim_gl_ctx->no_of_uecc_instances; 
                uecc_index++)
        {
            if(p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index] != PNULL)
            {
                if(p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index]->p_p_ue_ctx != PNULL)
                {
                    /* Check and clean context of Max supported UE per UECC */
                    for (ue_index = 0; ue_index < total_ue_supported; ue_index++)
                    {
                        p_ue_ctx = p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index]->p_p_ue_ctx[ue_index];

                        if (PNULL != p_ue_ctx)
                        {
                            /* Process this special message */
                            uemh_fsm_process_event( UEMH_EVENT_OAMH_CLEANUP_REQ,/*coverity fix start*/
                                    p_api,
                                    p_api,/*coverity fix stop*/
                                    p_ue_ctx,
                                    p_rrc_llim_gl_ctx);
                        }
                    }
                    rrc_mem_free(p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index]->p_p_ue_ctx);
                    p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index]->p_p_ue_ctx = PNULL;
                }

                rrc_mem_free(p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index]);
                p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index] = PNULL;
            }
        }

        rrc_mem_free(p_rrc_llim_gl_ctx->p_p_uecc_ctx);
        p_rrc_llim_gl_ctx->p_p_uecc_ctx = PNULL;
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_process_timer_msg
*   INPUT        : rrc_timer_t             timer_i
*                  rrc_llim_timer_buf_t    *p_rrc_llim_timer_buf
*                  rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function partially process UEMH timer message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_process_timer_msg
(
    /* Identity of the expired timer */
    rrc_timer_t             timer_id,
    /* Pointer to buffer associated with timer msg */
    rrc_llim_timer_buf_t    *p_rrc_llim_timer_buf,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t       *p_rrc_llim_gl_ctx
)
{
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    rrc_uemh_timer_buf_t *p_uemh_timer_buf = (rrc_uemh_timer_buf_t
        *)p_rrc_llim_timer_buf;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_rrc_llim_timer_buf);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_ASSERT(sizeof(rrc_uemh_timer_buf_t) == sizeof(rrc_llim_timer_buf_t) +
        sizeof(rrc_ue_index_t) + sizeof(uemh_fsm_event_et) + sizeof(rrc_transaction_id_t) +
        sizeof(rrc_cell_index_t));
    RRC_ASSERT(LLIM_SUBMODULE_UEMH == p_uemh_timer_buf->target_submodule);
    
    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_uemh_timer_buf->ue_index, &p_ue_ctx);
    RRC_LLIM_TRACE(RRC_INFO, "rrc_uemh_process_timer_msg: timer id:"
            "%p expired event id: %u,submodule id: %u ",
            timer_id,p_uemh_timer_buf->uemh_event,
            p_uemh_timer_buf->target_submodule);

    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_uemh_timer_buf->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_uemh_timer_buf->cell_index);
    }

    /* We have only ue specific timer event */
    /* Search in DB ue with p_uemh_timer_buf->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                p_uemh_timer_buf->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( p_uemh_timer_buf->uemh_event,
                    timer_id,
                    p_uemh_timer_buf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_s1u_error_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process S1U_ERROR_IND
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_s1u_error_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_s1u_error_ind_t s1u_error_ind;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_s1u_error_ind(
                                &s1u_error_ind,
                                (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:S1U->RRC:RRC_S1U_ERROR_IND:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(s1u_error_ind.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:S1U->RRC:RRC_S1U_ERROR_IND:[UE:%u] LC:%u",
                        s1u_error_ind.ue_index, s1u_error_ind.lc_id);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, s1u_error_ind.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with s1u_error_ind.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING,
                "[S1U_ERROR_IND] ue_index [%i] not found - ignore",
                s1u_error_ind.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_S1U_ERROR_IND,
                    p_api,
                    &s1u_error_ind,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_s1u_path_success_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process S1U_PATH_SUCCESS_IND
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_s1u_path_success_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_s1u_path_success_ind_t s1u_path_success;
    rrc_uecc_llim_s1u_path_success_ind_t uecc_s1u_path_success;
    S32 length_read = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    RRC_LLIM_TRACE(RRC_INFO,
                "API:S1U->RRC:UECC_LLIM_S1U_PATH_SUCCESS_IND: Received; unused variable p_rrc_llim_gl_ctx = %p",p_rrc_llim_gl_ctx);/*SPR 17777 +-*/
    do {
        /* Precondition - p_api should point to right message */
        if (RRC_SUCCESS != rrc_il_parse_rrc_s1u_path_success_ind(
                                &s1u_path_success,
                                (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
        {
            RRC_LLIM_TRACE(RRC_WARNING,
                    "API:S1U->RRC:UECC_LLIM_S1U_PATH_SUCCESS_IND:Bad message - ignore");
            break;
        }


        /* Fill UECC_LLIM_S1U_PATH_SUCCESS_IND */

        /* Pass message to the UECC directly */
        memset_wrapper(&uecc_s1u_path_success, 0,
                   sizeof(rrc_uecc_llim_s1u_path_success_ind_t));


        /* Pass message to the UECC directly */
        if (RRC_SUCCESS != rrc_llim_send_internal_msg(
                                RRC_UECCMD_MODULE_ID,
                                UECC_LLIM_S1U_PATH_SUCCESS_IND,
                                sizeof(rrc_uecc_llim_s1u_path_success_ind_t),
                                &uecc_s1u_path_success))
        {
            RRC_LLIM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_S1U_PATH_SUCCESS_IND] Delivery failed");
        }

    } while (0);

    RRC_LLIM_UT_TRACE_EXIT();
}


/******************************************************************************
*   FUNCTION NAME: rrc_uemh_s1u_path_failure_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process S1U_PATH_FAILURE_IND
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_s1u_path_failure_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_s1u_path_failure_ind_t s1u_path_failure;
    rrc_uecc_llim_s1u_path_failure_ind_t uecc_s1u_path_failure;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Coverity Fix 83184 Start */
    memset_wrapper(&s1u_path_failure, RRC_NULL,
                   sizeof(rrc_s1u_path_failure_ind_t));
    /* Coverity Fix 83184 Stop */

    do {
        /* Precondition - p_api should point to right message */
        if (RRC_SUCCESS != rrc_il_parse_rrc_s1u_path_failure_ind(
                                &s1u_path_failure,
                                (U8*)p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
        {
            RRC_LLIM_TRACE(RRC_WARNING,
                    "API:S1U->RRC:UECC_LLIM_S1U_PATH_FAILURE_IND:Bad message - ignore");
            break;
        }

        /* IPv4 address is printed currently to avoid loop */
        RRC_LLIM_TRACE(RRC_INFO,
                "API:S1U->RRC:UECC_LLIM_S1U_PATH_FAILURE_IND:IP:%u:%u.%u.%u.%u",
                s1u_path_failure.transport_addr.data_length,
                s1u_path_failure.transport_addr.data[0],
                s1u_path_failure.transport_addr.data[1],
                s1u_path_failure.transport_addr.data[2],
                s1u_path_failure.transport_addr.data[3]);

        if (0 == (RRC_TRANSPORT_ADDR_PRESENT & s1u_path_failure.bitmask))
        {
            RRC_LLIM_TRACE(RRC_WARNING,
                    "[S1U_PATH_FAILURE_IND] "
                    "Transport address is absent - ignore");
            break;
        }

        if (0 == s1u_path_failure.transport_addr.data_length)
        {
            RRC_LLIM_TRACE(RRC_WARNING,
                    "[S1U_PATH_FAILURE_IND] "
                    "Invalid Transport address length [%u] - ignore",
                    s1u_path_failure.transport_addr.data_length);
            break;
        }

        /* Fill UECC_LLIM_S1U_PATH_FAILURE_IND */

        /* Pass message to the UECC directly */
        memset_wrapper(&uecc_s1u_path_failure, 0,
                   sizeof(rrc_uecc_llim_s1u_path_failure_ind_t));

        uecc_s1u_path_failure.cell_index = cell_index;
        uecc_s1u_path_failure.ip_address_octet_size =
           (U8) s1u_path_failure.transport_addr.data_length;

        RRC_ASSERT(sizeof(uecc_s1u_path_failure.ip_address) ==
               sizeof(s1u_path_failure.transport_addr.data));

        l3_memcpy_wrapper(uecc_s1u_path_failure.ip_address,
                s1u_path_failure.transport_addr.data,
                sizeof(uecc_s1u_path_failure.ip_address));

        if (RRC_SUCCESS != rrc_llim_send_internal_msg(
                                RRC_UECCMD_MODULE_ID,
                                UECC_LLIM_S1U_PATH_FAILURE_IND,
                                sizeof(rrc_uecc_llim_s1u_path_failure_ind_t),
                                &uecc_s1u_path_failure))
        {
            RRC_LLIM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_S1U_PATH_FAILURE_IND] Delivery failed; unused variable p_rrc_llim_gl_ctx=%p",p_rrc_llim_gl_ctx);/*SPR 17777 +-*/
        }

    } while (0);

    RRC_LLIM_UT_TRACE_EXIT();
}

/*HANDOVER_JANEN_START*/
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_mac_i_resp_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process PDCP_MAC_I_RESP message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_mac_i_resp_process_msg 
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_pdcp_mac_i_resp_t pdcp_mac_i_resp;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_mac_i_resp( 
                                &pdcp_mac_i_resp,
                                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_MAC_I_RESP:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(pdcp_mac_i_resp.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_MAC_I_RESP:[UE:%u] with Response:%u",
                   pdcp_mac_i_resp.ue_index, pdcp_mac_i_resp.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_mac_i_resp.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with pdcp_mac_i_resp.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                pdcp_mac_i_resp.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PDCP_MAC_I_RESP,
                    p_api,
                    &pdcp_mac_i_resp,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/*SPR_19066_START*/
/* Code Removed */
/*SPR_19066_END*/

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_sn_hfn_status_resp_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process PDCP_SN_HFN_STATUS_RESP message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_sn_hfn_status_resp_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_pdcp_sn_hfn_status_resp_t  pdcp_sn_hfn_status_resp;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_sn_hfn_status_resp( 
                                &pdcp_sn_hfn_status_resp,
                                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_SN_HFN_STATUS_RESP:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(pdcp_sn_hfn_status_resp.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_SN_HFN_STATUS_RESP:[UE:%u] with Response:%u",
                   pdcp_sn_hfn_status_resp.ue_index,  pdcp_sn_hfn_status_resp.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_sn_hfn_status_resp.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with pdcp_sn_hfn_status_resp.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                pdcp_sn_hfn_status_resp.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PDCP_SN_HFN_STATUS_RESP,
                    p_api,
                    &pdcp_sn_hfn_status_resp,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_s1u_end_marker_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process RRC_S1U_END_MARKER_IND message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_s1u_end_marker_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_s1u_end_marker_ind_t s1u_end_marker_ind;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_s1u_end_marker_ind(
                                    &s1u_end_marker_ind,
                                    (U8* )p_api+RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    rrc_get_api_buf_size(p_api) -
                                    RRC_FULL_INTERFACE_HEADERS_SIZE,
                                    &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:S1U->RRC:RRC_S1U_END_MARKER_IND:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
  
    SET_UE_INDEX(s1u_end_marker_ind.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:S1U->RRC:RRC_S1U_END_MARKER_IND:[UE:%u] with LC:%u",
                   s1u_end_marker_ind.ue_index, s1u_end_marker_ind.lc_id);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, s1u_end_marker_ind.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with s1u_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                s1u_end_marker_ind.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_S1U_END_MARKER_IND,
                    p_api,
                    &s1u_end_marker_ind,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/*JANEN_END*/

/* ERB RELEASE COMMAND START */
/******************************************************************************
*   FUNCTION NAME : rrc_uemh_uecc_llim_delete_lc_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process UECC_LLIM_DELETE_LC_REQ message
*                  and call FSM.
*
*   RETURNS       :  None
*
******************************************************************************/
void rrc_uemh_uecc_llim_delete_lc_req_process_msg
(
    void*                p_api,  /* Pointer to input API buffer */
    rrc_llim_gl_ctx_t*   p_rrc_llim_gl_ctx  /* Pointer to the LLIM 
                                             * global context data */
)
{
    rrc_llim_ue_ctx_t*              p_ue_ctx = PNULL;
    rrc_uecc_llim_delete_lc_req_t*  p_delete_lc_req = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_delete_lc_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_delete_lc_req = (rrc_uecc_llim_delete_lc_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);
    
    SET_CELL_AND_UE_INDEX(p_delete_lc_req->cell_index, p_delete_lc_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_delete_lc_req->ue_index, &p_ue_ctx);

    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_delete_lc_req->cell_index]))
            {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_delete_lc_req->cell_index);
    }

    /* Search in DB UE with p_add_lc_req->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Invalid Ue Index - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_DELETE_LC_REQ,
                    p_api,
                    p_delete_lc_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/* ERB RELEASE COMMAND END */

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_suspend_ue_entity_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process UECC_LLIM_SUSPEND_UE_ENTITY REQUEST.
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_suspend_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_suspend_ue_entity_req_t* p_ue_suspend_req = PNULL ;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    
    RRC_LLIM_UT_TRACE_ENTER();
    
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    RRC_LLIM_TRACE(RRC_INFO,
            "Inside the Handler for UECC_LLIM_SUSPEND_UE_ENTITY REQUEST ");
       
    p_ue_suspend_req = (rrc_uecc_llim_suspend_ue_entity_req_t*)((U8* )p_api +
       RRC_API_HEADER_SIZE);
    
    SET_CELL_AND_UE_INDEX(p_ue_suspend_req->cell_index, p_ue_suspend_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_ue_suspend_req->ue_index, &p_ue_ctx);

    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_ue_suspend_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_ue_suspend_req->cell_index);
    }
    /* Search in DB UE with p_ue_suspend_req->ue_index -should be present */
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                p_ue_suspend_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {

            uemh_fsm_process_event( UEMH_EVENT_UECC_SUSPEND_UE_ENTITY_REQ,
                    p_api,
                    p_ue_suspend_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }    
    }
    RRC_LLIM_UT_TRACE_EXIT(); 

}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_suspend_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process RRC_PDCP_SUSPEND_UE_ENTITY_CNF .
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_suspend_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_pdcp_suspend_ue_entity_cnf_t pdcp_suspend_ue_entity_cnf  ;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_suspend_ue_entity_cnf(
                                        &pdcp_suspend_ue_entity_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_SUSPEND_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(pdcp_suspend_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_SUSPEND_UE_ENTITY_CNF:[UE:%u] with response:%u",
                   pdcp_suspend_ue_entity_cnf.ue_index, pdcp_suspend_ue_entity_cnf.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_suspend_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with pdcp_suspend_ue_entity_cnf.ue_index - should be
     * present*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                pdcp_suspend_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {

            uemh_fsm_process_event (UEMH_EVENT_PDCP_SUSPEND_UE_ENTITY_CNF, p_api,
                    &pdcp_suspend_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }


    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_re_establish_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF .
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_re_establish_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_pdcp_re_establish_ue_entity_cnf_t pdcp_re_establish_ue_entity_cnf  ;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_re_establish_ue_entity_cnf(
                                        &pdcp_re_establish_ue_entity_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(pdcp_re_establish_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_RE_ESTABLISH_UE_ENTITY_CNF:[UE:%u] with response:%u",
                   pdcp_re_establish_ue_entity_cnf.ue_index, pdcp_re_establish_ue_entity_cnf.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_re_establish_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }
    /* Search in DB UE with pdcp_re_establish_ue_entity_cnf.ue_index - 
     * should be present*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                pdcp_re_establish_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            uemh_fsm_process_event (UEMH_EVENT_PDCP_REESTABLISH_UE_ENTITY_CNF, p_api,
                    &pdcp_re_establish_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }

    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_resume_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process RRC_PDCP_RESUME_UE_ENTITY_CNF .
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_resume_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_pdcp_resume_ue_entity_cnf_t pdcp_resume_ue_entity_cnf  ;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_resume_ue_entity_cnf(
                                        &pdcp_resume_ue_entity_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_RESUME_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(pdcp_resume_ue_entity_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_RESUME_UE_ENTITY_CNF:[UE:%u] with response:%u",
                   pdcp_resume_ue_entity_cnf.ue_index, pdcp_resume_ue_entity_cnf.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_resume_ue_entity_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with pdcp_resume_ue_entity_cnf.ue_index - 
     * should be present*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                pdcp_resume_ue_entity_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {

            uemh_fsm_process_event (UEMH_EVENT_PDCP_RESUME_UE_ENTITY_CNF, p_api,
                    &pdcp_resume_ue_entity_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_phy_change_crnti_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION:
*       This function  process RRC_PHY_CHANGE_CRNTI_CNF.
*       message and call FSM.
*
*   RETURNS:
*       None
*
******************************************************************************/
void rrc_uemh_phy_change_crnti_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_phy_change_crnti_cnf_t phy_change_crnti_cnf;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_phy_change_crnti_cnf(
                                        &phy_change_crnti_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PHY->RRC:RRC_PHY_CHANGE_CRNTI_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(phy_change_crnti_cnf.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:PHY->RRC:RRC_PHY_CHANGE_CRNTI_CNF:[UE:%u] with response:%u",
                    phy_change_crnti_cnf.ue_index, phy_change_crnti_cnf.response);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, phy_change_crnti_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with phy_change_crnti_cnf.ue_index - should be found*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                phy_change_crnti_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            uemh_fsm_process_event (UEMH_EVENT_PHY_CHANGE_CRNTI_CNF, p_api,
                    &phy_change_crnti_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }

    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_mac_change_crnti_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process RRC_MAC_CHANGE_CRNTI_CNF .
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_mac_change_crnti_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_mac_change_crnti_cnf_t mac_change_crnti_cnf;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_change_crnti_cnf(
                                        &mac_change_crnti_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:MAC->RRC:RRC_MAC_CHANGE_CRNTI_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:MAC->RRC:RRC_MAC_CHANGE_CRNTI_CNF:[UE:%u] with response:%u",
                    mac_change_crnti_cnf.ue_index, mac_change_crnti_cnf.response);
    SET_UE_INDEX(mac_change_crnti_cnf.ue_index);


    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, mac_change_crnti_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with mac_change_crnti_cnf.ue_index- should be found*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                mac_change_crnti_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            uemh_fsm_process_event (UEMH_EVENT_MAC_CHANGE_CRNTI_CNF, p_api,
                    &mac_change_crnti_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }

    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_rlc_change_crnti_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process RRC_RLC_CHANGE_CRNTI_CNF .
*                message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_rlc_change_crnti_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_rlc_change_crnti_cnf_t rlc_change_crnti_cnf;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_rlc_change_crnti_cnf(
                                        &rlc_change_crnti_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:RLC->RRC:RRC_RLC_CHANGE_CRNTI_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:RLC->RRC:RRC_RLC_CHANGE_CRNTI_CNF:[UE:%u] with response:%u",
                    rlc_change_crnti_cnf.ue_index, rlc_change_crnti_cnf.response);
    SET_UE_INDEX(rlc_change_crnti_cnf.ue_index);


    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, rlc_change_crnti_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /*Search in DB UE with rlc_change_crnti_cnf.ue_index -should be found*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                rlc_change_crnti_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            uemh_fsm_process_event (UEMH_EVENT_RLC_CHANGE_CRNTI_CNF, p_api,
                    &rlc_change_crnti_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);

        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_change_crnti_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process RRC_PDCP_CHANGE_CRNTI_CNF.
*                  message and call FSM.
*
*   RETURNS     : None
*
******************************************************************************/
void rrc_uemh_pdcp_change_crnti_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_pdcp_change_crnti_cnf_t pdcp_change_crnti_cnf;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_change_crnti_cnf(
                                        &pdcp_change_crnti_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_CHANGE_CRNTI_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_CHANGE_CRNTI_CNF:[UE:%u] with response:%u",
                    pdcp_change_crnti_cnf.ue_index, pdcp_change_crnti_cnf.response);
    SET_UE_INDEX(pdcp_change_crnti_cnf.ue_index);


    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_change_crnti_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }
    /* Search in DB UE with pdcp_change_crnti_cnf.ue_index - 
     * should be found*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                pdcp_change_crnti_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            uemh_fsm_process_event (UEMH_EVENT_PDCP_CHANGE_CRNTI_CNF, p_api,
                    &pdcp_change_crnti_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);

        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_mac_ue_reset_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process RRC_MAC_UE_RESET_CNF .
*                   message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_mac_reset_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
     rrc_mac_reset_ue_entity_cnf_t  rrc_mac_ue_reset_cnf ;
     rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
     S32 length_read = RRC_NULL;
     rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

     RRC_LLIM_UT_TRACE_ENTER();

     RRC_ASSERT(PNULL != p_api);
     RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);     
     
     /* Precondition - p_api should point to right message */
     if (RRC_SUCCESS != rrc_il_parse_rrc_mac_reset_ue_entity_cnf(
                                        &rrc_mac_ue_reset_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
     {
        RRC_LLIM_TRACE(RRC_WARNING, "API:MAC->RRC:RRC_MAC_UE_RESET_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
     }
 
     SET_UE_INDEX(rrc_mac_ue_reset_cnf.ue_index);

     RRC_LLIM_TRACE(RRC_INFO,"API:MAC->RRC:RRC_MAC_UE_RESET_CNF:[UE:%u] with response:%u",
                    rrc_mac_ue_reset_cnf.ue_index, rrc_mac_ue_reset_cnf.response);

     rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, rrc_mac_ue_reset_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
     {
         RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                 cell_index);
     }
     /* Search in DB UE with rrc_mac_ue_reset_cnf.ue_index - 
      * should be found*/
     else if((PNULL == p_ue_ctx))
     {
         RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                 rrc_mac_ue_reset_cnf.ue_index);
     }
     else
     {
         if (PNULL != p_ue_ctx)
         {
             uemh_fsm_process_event (UEMH_EVENT_MAC_RESET_UE_ENTITY_CNF,
                     p_api,
                     &rrc_mac_ue_reset_cnf,
                     p_ue_ctx,
                     p_rrc_llim_gl_ctx);
         }

     }
     RRC_LLIM_UT_TRACE_EXIT();
   
            

}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_rlc_re_establish_ue_entity_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF .
*                   message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_rlc_re_establish_ue_entity_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
     rrc_rlc_re_establish_ue_entity_cnf_t  rrc_rlc_re_establish_ue_entity_cnf;
     rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
     S32 length_read = RRC_NULL;
     rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

     RRC_LLIM_UT_TRACE_ENTER();

     RRC_ASSERT(PNULL != p_api);
     RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);     
     
     /* Precondition - p_api should point to right message */
     if (RRC_SUCCESS != rrc_il_parse_rrc_rlc_re_establish_ue_entity_cnf(
                                        &rrc_rlc_re_establish_ue_entity_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
     {
        RRC_LLIM_TRACE(RRC_WARNING, "API:RLC->RRC:RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
     }

     SET_UE_INDEX(rrc_rlc_re_establish_ue_entity_cnf.ue_index);

     RRC_LLIM_TRACE(RRC_INFO,"API:RLC->RRC:RRC_RLC_RE_ESTABLISH_UE_ENTITY_CNF:[UE:%u] with response:%u",
                    rrc_rlc_re_establish_ue_entity_cnf.ue_index, rrc_rlc_re_establish_ue_entity_cnf.response_code);

     rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, rrc_rlc_re_establish_ue_entity_cnf.ue_index, &p_ue_ctx);

     rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

     SET_CELL_INDEX(cell_index);

     /* Check whether UE's Cell exists or not */
     if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
     {
         RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                 cell_index);
     }
     /* Search in DB UE with rrc_rlc_re_establish_ue_entity_cnf.ue_index - 
      * should be present*/
     else if((PNULL == p_ue_ctx))
     {
         RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                 rrc_rlc_re_establish_ue_entity_cnf.ue_index);
     }
     else
     {
         if (PNULL != p_ue_ctx)
         {
             uemh_fsm_process_event (UEMH_EVENT_RLC_REESTABLISH_UE_ENTITY_CNF,
                     p_api,
                     &rrc_rlc_re_establish_ue_entity_cnf,
                     p_ue_ctx,
                     p_rrc_llim_gl_ctx);
         }
     } 
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_mac_radio_link_failure_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function  process  the RRC_MAC_RADIO
*                   _LINK_FAILURE_IND mesages and call FSM.
*  RETURNS       : None
******************************************************************************/
void rrc_uemh_mac_radio_link_failure_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_mac_radio_link_failure_ind_t  mac_radio_link_failure ;
    rrc_uecc_llim_radio_link_failure_ind_t radio_link_failure ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();
     
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    
        /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_radio_link_failure_ind(
                                        &mac_radio_link_failure,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:MAC->RRC:RRC_MAC_RADIO_LINK_FAILURE:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"RRC_MSG: API:MAC->RRC:RRC_MAC_RADIO_LINK_FAILURE:[UE:%u] with rlf_cause:%u",
                    mac_radio_link_failure.ue_index, mac_radio_link_failure.rlf_cause);
    SET_UE_INDEX(mac_radio_link_failure.ue_index);


    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    radio_link_failure.ue_index = mac_radio_link_failure.ue_index ;
    radio_link_failure.cell_index = cell_index ;
    /* sync changes start */
    radio_link_failure.ue_sync_status = RRC_INVALID_UE_SYNC_NUM ;
        
    /* SPR 15644 fix start */
    /* sync changes stop */
        
    if (RRC_SUCCESS == rrc_llim_send_internal_msg(
            RRC_GET_UECC_MODULE_ID(radio_link_failure.ue_index),
            UECC_LLIM_RADIO_LINK_FAILURE_IND,
            sizeof(rrc_uecc_llim_radio_link_failure_ind_t),
            &radio_link_failure))
    {
            RRC_LLIM_TRACE(RRC_INFO,
                "[UECC_LLIM_SYNC_STATUS_IND] Delivery Success: "
                "ue_index[%d] cell_index[%d] ue_sync_status[%d]", 
                radio_link_failure.ue_index, radio_link_failure.cell_index, 
                radio_link_failure.ue_sync_status );
    }
    else
    {
         RRC_LLIM_TRACE(RRC_WARNING,
                "[UECC_LLIM_SYNC_STATUS_IND] Delivery failed: "
                "ue_index[%d] cell_index[%d] ue_sync_status[%d]", 
                radio_link_failure.ue_index, radio_link_failure.cell_index, 
                radio_link_failure.ue_sync_status );
        /* SPR 15644 fix end */
    }
        
        RRC_LLIM_UT_TRACE_EXIT();
        
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_mac_ue_sync_status_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process  the RRC_MAC_UE_SYNC_STATUS_IND
*                  messages and call FSM.
*
*   RETURNS     : None
*
******************************************************************************/
void rrc_uemh_mac_ue_sync_status_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_mac_ue_sync_status_ind_t  mac_ue_sync_status_ind ;
    rrc_uecc_llim_radio_link_failure_ind_t radio_link_failure ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();
     
    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
        
    /*SYNC_changes Start */
    memset_wrapper (&radio_link_failure, RRC_NULL, 
                sizeof(rrc_uecc_llim_radio_link_failure_ind_t));
    /*SYNC_changes Stop */
    
    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_ue_sync_status_ind(
                                        &mac_ue_sync_status_ind,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:MAC->RRC:RRC_MAC_UE_SYNC_STATUS_IND:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_UE_INDEX(mac_ue_sync_status_ind.ue_index);

    /* SPR 15644 fix start */
    RRC_LLIM_TRACE(RRC_INFO,"RRC_MSG: API:MAC->RRC:RRC_MAC_UE_SYNC_STATUS_IND:[UE:%u] with ue_sync_status:%u",
            mac_ue_sync_status_ind.ue_index,
            mac_ue_sync_status_ind.ue_sync_status);
    /* SPR 15644 fix end */

    if(RRC_MAC_OUT_OF_SYNC_MAX_RETRIES != mac_ue_sync_status_ind.ue_sync_status)
    {
    /*SYNC_changes Start */
        RRC_LLIM_TRACE(RRC_INFO, " MAC in UE SYNC STATUS.");
    }
    /* BZ_813 Fix Start */
    if(RRC_MAC_IN_SYNC_HO != mac_ue_sync_status_ind.ue_sync_status)
    {
    /* BZ_813 Fix Stop */
/* SPR 12512 Fix Start */
        rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);
/* SPR 12512 Fix Stop */

        SET_CELL_INDEX(cell_index);

        radio_link_failure.cell_index = cell_index ;
        radio_link_failure.ue_index = mac_ue_sync_status_ind.ue_index ;
        /* sync changes start */
        radio_link_failure.ue_sync_status =  mac_ue_sync_status_ind.ue_sync_status;
        /* sync changes stop */

        if (RRC_SUCCESS == rrc_llim_send_internal_msg(
                    RRC_GET_UECC_MODULE_ID(radio_link_failure.ue_index),
                    UECC_LLIM_RADIO_LINK_FAILURE_IND,
                    sizeof(rrc_uecc_llim_radio_link_failure_ind_t),
                    &radio_link_failure))
        {
            /* SPR 15644 fix start */
            RRC_LLIM_TRACE(RRC_INFO,
                    "[UECC_LLIM_SYNC_STATUS_IND] Delivery Success: "
                    "ue_index[%d] cell_index[%d] ue_sync_status[%d]", 
                    radio_link_failure.ue_index, radio_link_failure.cell_index,
                    radio_link_failure.ue_sync_status );
        }
        else
        {
            RRC_LLIM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_SYNC_STATUS_IND] Delivery failed: "
                    "ue_index[%d] cell_index[%d] ue_sync_status[%d]", 
                    radio_link_failure.ue_index, radio_link_failure.cell_index,
                    radio_link_failure.ue_sync_status );
            /* SPR 15644 fix end */
        }
    }    
    /*SYNC_changes Stop */
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_rlc_ue_entity_error_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process RRC_RLC_UE_ENTITY_ERROR_IND  mesages and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_rlc_ue_entity_error_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rlc_ue_entity_error_ind_t uecc_radio_link_failure ;
    rrc_uecc_llim_radio_link_failure_ind_t radio_link_failure ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rlc_ue_entity_error_ind(
                                        &uecc_radio_link_failure,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:RLC->RRC:RRC_RLC_UE_ENTITY_ERROR_IND:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    /* SPR 15644 fix start */
    RRC_LLIM_TRACE(RRC_INFO,"RRC_MSG: API:RLC->RRC:RRC_RLC_UE_ENTITY_ERROR_IND:[UE:%u] LC[%u]",
                  uecc_radio_link_failure.ue_index, uecc_radio_link_failure.lc_id );
    /* SPR 15644 fix end */
    SET_UE_INDEX(uecc_radio_link_failure.ue_index);


    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    radio_link_failure.cell_index = cell_index ;
    radio_link_failure.ue_index = uecc_radio_link_failure.ue_index ;
    /* sync changes start */
    radio_link_failure.ue_sync_status = RRC_INVALID_UE_SYNC_NUM; 
    /* sync status stop */
    /* SPR 15644 fix start */
    RRC_LLIM_TRACE(RRC_INFO,
            "[UECC_LLIM_SYNC_STATUS_IND] UE_INDEX = %u and LC_ID = %u",
        uecc_radio_link_failure.ue_index,
        uecc_radio_link_failure.lc_id);
    /* SPR 15644 fix end */

    if (uecc_radio_link_failure.lc_id == RRC_SRB0_LC_ID 
        || uecc_radio_link_failure.lc_id == RRC_SRB1_LC_ID
        || uecc_radio_link_failure.lc_id == RRC_SRB2_LC_ID
        || (uecc_radio_link_failure.lc_id <= (RRC_MAX_NUM_DRB +
           RRC_MAX_NUM_SRB)))
    {
        if (RRC_SUCCESS == rrc_llim_send_internal_msg(
             RRC_GET_UECC_MODULE_ID(uecc_radio_link_failure.ue_index),
             UECC_LLIM_RADIO_LINK_FAILURE_IND,
             sizeof(rrc_uecc_llim_radio_link_failure_ind_t),
             &radio_link_failure))
         {
             /* SPR 15644 fix start */
             RRC_LLIM_TRACE(RRC_INFO,
                    "[UECC_LLIM_SYNC_STATUS_IND] Delivery Success: "
                    "ue_index[%d] cell_index[%d] ue_sync_status[%d]", 
                    radio_link_failure.ue_index, radio_link_failure.cell_index,
                    radio_link_failure.ue_sync_status );
         }
         else
         {
             RRC_LLIM_TRACE(RRC_WARNING,
                    "[UECC_LLIM_SYNC_STATUS_IND] Delivery Failed: "
                    "ue_index[%d] cell_index[%d] ue_sync_status[%d]", 
                    radio_link_failure.ue_index, radio_link_failure.cell_index,
                    radio_link_failure.ue_sync_status );
             /* SPR 15644 fix end */
          }
    }
    else
    {
             RRC_LLIM_TRACE(RRC_WARNING,
                    "LCID's sent by RLC does'nt match with that of SRB'S"
                    " and DRB'S hence Radio Link Failure is not declared ");
    } 
    
       RRC_LLIM_UT_TRACE_EXIT();

}


/******************************************************************************
*   FUNCTION NAME: rrc_uemh_mac_ho_rach_resource_resp
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function partially process RRC_MAC_HO_RACH_RESOURCE_RESP
*                  message and call FSM.
*   RETURNS      : None
******************************************************************************/
void rrc_uemh_mac_ho_rach_resource_resp
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_mac_ho_rach_resource_resp_t ho_rach_resource_resp;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_mac_ho_rach_resource_resp(
                                        &ho_rach_resource_resp,
                                        (U8*)p_api +
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:MAC->RRC:RRC_MAC_HO_RACH_RESOURCE_RESP:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
  
    SET_UE_INDEX(ho_rach_resource_resp.ue_index);

    RRC_LLIM_TRACE(RRC_INFO,"API:MAC->RRC:RRC_MAC_HO_RACH_RESOURCE_RESP:[UE:%u] with response:%u",
                  ho_rach_resource_resp.ue_index, ho_rach_resource_resp.response);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, ho_rach_resource_resp.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }
    /* Search in DB UE with phy_create_ue_entity_cnf.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                ho_rach_resource_resp.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_MAC_HO_RACH_RESOURCE_RESP,
                    p_api,
                    &ho_rach_resource_resp,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: uemh_send_mac_ho_rel_rach_res_ind
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : state UEMH_STATE_UE_ENTITY_IN_SERVICE 
*                  event UEMH_EVENT_UECC_HO_REL_RACH_RESOURCE_IND
*   RETURNS      : None
******************************************************************************/
/* Bug 839 Fix Start */
void uemh_send_mac_ho_rel_rach_res_ind
/* Bug 839 Fix Stop */
(
    void                *p_msg,             /* p_msg */
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_ho_rach_rel_resource_ind_t *p_rrc_uecc_llim_ho_rach_rel_resource_ind=
        (rrc_uecc_llim_ho_rach_rel_resource_ind_t *)p_msg;
    rrc_mac_ho_rel_rach_resource_ind_t          rrc_mac_ho_rel_rach_resource_ind;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);
    memset_wrapper(&rrc_mac_ho_rel_rach_resource_ind, 0, 
                             sizeof(rrc_mac_ho_rel_rach_resource_ind_t));


    /* 2. send RRC_MAC_REL_RACH_RESOURCE_IND */

    rrc_mac_ho_rel_rach_resource_ind.ue_index = 
        p_rrc_uecc_llim_ho_rach_rel_resource_ind->ue_index;
    if (p_rrc_uecc_llim_ho_rach_rel_resource_ind->bitmask &
            RRC_UECC_LLIM_HO_RACH_REL_RESOURCE_IND_CRNTI_PRESENT)
    {
        rrc_mac_ho_rel_rach_resource_ind.optional_elems_present |= 
            RRC_MAC_CRNTI_INFO_PRESENT;
        rrc_mac_ho_rel_rach_resource_ind.crnti_info.crnti = 
            p_rrc_uecc_llim_ho_rach_rel_resource_ind->crnti;
    }

    RRC_LLIM_TRACE(RRC_INFO,"API:RRC->MAC:RRC_MAC_HO_REL_RACH_RESOURCE_IND:[UE:%u]; unused variable=%p",
                  rrc_mac_ho_rel_rach_resource_ind.ue_index,p_rrc_llim_gl_ctx);/*SPR 17777 +-*/

    rrc_mac_il_send_rrc_mac_ho_rel_rach_resource_ind(
            &rrc_mac_ho_rel_rach_resource_ind,
            RRC_LLIM_MODULE_ID,
			RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_rrc_uecc_llim_ho_rach_rel_resource_ind->cell_index),
		p_rrc_uecc_llim_ho_rach_rel_resource_ind->transaction_id,
            p_rrc_uecc_llim_ho_rach_rel_resource_ind->cell_index);

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_meas_config_req_process_msg 
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function partially process UECC_LLIM_MEAS_CONFIG_REQ 
*                  message.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_uecc_llim_meas_config_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_meas_config_req_t *p_meas_config_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    rrc_mac_reconfigure_ue_entity_req_t mac_rcfg_ue_req;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_meas_config_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_meas_config_req = (rrc_uecc_llim_meas_config_req_t*)
        ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_meas_config_req->cell_index, p_meas_config_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_meas_config_req->ue_index, &p_ue_ctx);

    /* Search in DB UE with p_meas_config_req->ue_index - 
       should be present */
    if ((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || ((p_rrc_llim_gl_ctx->
                    p_p_cell_ctx[p_meas_config_req->cell_index]) == PNULL ))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_meas_config_req->cell_index);
    }
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_meas_config_req->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Clear buffer */
            memset_wrapper(&p_ue_ctx->curr_op_param.meas_config, RRC_NULL,
                    sizeof(p_ue_ctx->curr_op_param.meas_config));

            /* Set flag for meas config presence */
            p_ue_ctx->curr_op_param.meas_config.transaction_id_presence = 
                RRC_TRUE;

            /* Store received meas transaction id */
            p_ue_ctx->curr_op_param.meas_config.transaction_id = 
                p_meas_config_req->transaction_id; 

            /* Build and send rrc_mac_reconfigure_ue_entity_req */
            memset_wrapper(&mac_rcfg_ue_req, RRC_NULL, sizeof(mac_rcfg_ue_req));

            /* Assign UE Index */
            mac_rcfg_ue_req.ue_index  = p_meas_config_req->ue_index;
            
            /* Set bitmask for RRC_MAC_RECONFIG_UE_INFO_PRESENT */
            mac_rcfg_ue_req.optional_elems_present |= 
                RRC_MAC_RECONFIG_UE_INFO_PRESENT;


            /* Set bitmask for RRC_MAC_RECONFIG_MEAS_GAP_CONFIG_INFO_PRESENT */
            mac_rcfg_ue_req.reconfig_ue_info.optional_elems_present |= 
                RRC_MAC_RECONFIG_MEAS_GAP_CONFIG_INFO_PRESENT;

            /* Set bitmask if
               RRC_MAC_MEAS_GAP_CONFIG_MEAS_GAP_SETUP_CONFIG_INFO_PRESENT 
               */
            if (p_meas_config_req->meas_config.meas_gap_config.bitmask &
                    MEAS_GAP_CONFIG_SETUP_CONFIG_INFO_PRESENT)
            {

                mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.bitmask |= 
                    RRC_MAC_MEAS_GAP_CONFIG_MEAS_GAP_SETUP_CONFIG_INFO_PRESENT;

                /* It should be -1 for setup */
                mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                    meas_gap_config_type = p_meas_config_req->
                    meas_config.meas_gap_config.meas_gap_config_type;

                /* Check for gap pattern id - 0 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP0_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 0; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset =
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp0;
                }
                /* Check for gap pattern id - 1 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP1_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 1; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset = 
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp1;
                }

                #ifdef ENDC_ENABLED
                /* Check for gap pattern id - 4 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP4_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 4; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset = 
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp4_r15;
                }

                /* Check for gap pattern id - 5 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP5_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 5; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset = 
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp5_r15;
                }

                /* Check for gap pattern id - 6 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP6_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 6; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset = 
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp6_r15;
                }

                /* Check for gap pattern id - 7 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP7_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 7; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset = 
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp7_r15;
                }

                /* Check for gap pattern id - 8 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP8_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 8; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset = 
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp8_r15;
                }

                /* Check for gap pattern id - 9 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP9_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 9; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset = 
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp9_r15;
                }

                /* Check for gap pattern id - 10 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP10_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 10; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset = 
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp10_r15;
                }

                /* Check for gap pattern id - 11 */
                if (p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.bitmask & 
                        MEAS_GAP_CONFIG_GP11_PRESENT)
                {
                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_pattern_id = 11; 

                    mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                        meas_gap_setup_config_info.gap_offset = 
                        p_meas_config_req->meas_config.
                        meas_gap_config.setup_config_info.gp11_r15;
                }
                #endif
            }
            else
            {
                /* It should be - 0 -for release */
                mac_rcfg_ue_req.reconfig_ue_info.meas_gap_config_info.
                    meas_gap_config_type = p_meas_config_req->
                    meas_config.meas_gap_config.meas_gap_config_type;
            }

            RRC_LLIM_TRACE(RRC_INFO,"API:RRC->MAC:RRC_MAC_RECONFIGURE_UE_ENTITY_REQ:[UE:%u]",
                                mac_rcfg_ue_req.ue_index);

            if (RRC_SUCCESS != rrc_mac_il_send_rrc_mac_reconfigure_ue_entity_req(
                            &mac_rcfg_ue_req, RRC_LLIM_MODULE_ID,
                            	RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_meas_config_req->cell_index),
				p_meas_config_req->transaction_id,
                            p_meas_config_req->cell_index))
            {
                RRC_LLIM_TRACE(RRC_WARNING, "Failed to send"
                        "MAC_RECONFIG_UE_ENTITY_REQ(for meas config)");
            }
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/*SPR_19066_START*/
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_inactive_ues_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function  process RRC_PDCP_INACTIVE_UES_IND .
*                  message and call FSM.
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_inactive_ues_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    void *p_msg = PNULL;
    rrc_pdcp_inactive_ues_ind_t pdcp_inactive_ues_ind;
    rrc_uecc_llim_inactive_ues_ind_t  *p_uecc_llim_inactive_ues_ind = PNULL;  
    S32 length_read = RRC_NULL;
    U8  inactive_ue_counter = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_inactive_ues_ind(
                                        &pdcp_inactive_ues_ind,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_INACTIVE_UES_IND:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    
    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_INACTIVE_UES_IND:Num-UE:%u",
                                pdcp_inactive_ues_ind.num_of_inactive_ue);

    /* cell index changes start */
    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);


    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
         return;
    }
    
    SET_CELL_INDEX(cell_index);
    /* cell index changes stop */
    
    /* allocate memory for the message */
    p_msg = rrc_llim_alloc_internal_msg(RRC_UECC_MODULE_ID,
                UECC_LLIM_INACTIVE_UES_IND,
               (U16) (sizeof(rrc_uecc_llim_inactive_ues_ind_t)));

    if (PNULL != p_msg)
    {
        p_uecc_llim_inactive_ues_ind = 
            (rrc_uecc_llim_inactive_ues_ind_t *)((U8*)p_msg +
                                         RRC_API_HEADER_SIZE);

        /* Init Operation */
        memset_wrapper(p_uecc_llim_inactive_ues_ind, RRC_NULL,
            sizeof(rrc_uecc_llim_inactive_ues_ind_t));
    
        /* Fill uecc_llim_inactive_ues_ind and send to UECC */
        p_uecc_llim_inactive_ues_ind->cell_index = cell_index;

        /* Fill number of inactive ue */
        p_uecc_llim_inactive_ues_ind->num_of_inactive_ue =
            pdcp_inactive_ues_ind.num_of_inactive_ue;  
    
        if ((pdcp_inactive_ues_ind.num_of_inactive_ue > 1)|| 
                (pdcp_inactive_ues_ind.num_of_inactive_ue < RRC_MAX_INACTIVE_UES))
        { 
        /* Fill inactive_ue_info */
        for (inactive_ue_counter = RRC_NULL; 
            inactive_ue_counter < pdcp_inactive_ues_ind.num_of_inactive_ue;
            inactive_ue_counter++)
        {  
            p_uecc_llim_inactive_ues_ind->inactive_ue_info[inactive_ue_counter].ue_index = 
            pdcp_inactive_ues_ind.inactive_ue_info[inactive_ue_counter].ue_index;
        }
        rrc_send_message(p_msg, RRC_UECC_1_MODULE_ID);
        }
        else
        {
            RRC_LLIM_TRACE(RRC_WARNING, "Parameter [pdcp_inactive_ues_ind.num_of_inactive_ue]"
                    "should be in range""1 to RRC_MAX_INACTIVE_UES. Incorrect value %u received.",
                    pdcp_inactive_ues_ind.num_of_inactive_ue);

        }
    }
    else 
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Message uecc_llim_inactive_ues_ind memory alloaction failure");
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/*SPR_19066_END*/

/* SPR 4118 Start */

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_notify_integrity_failure
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function process RRC_PDCP_NOTIFY_INTEGRITY_FAILURE message.
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_notify_integrity_failure
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_pdcp_notify_integrity_failure_t pdcp_notify_integrity_failure;
    S32                     length_read         = RRC_NULL;
    rrc_llim_ue_ctx_t       *p_ue_ctx           = PNULL;
    rrc_cell_index_t        cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_notify_integrity_failure(&pdcp_notify_integrity_failure,
                                (U8* )p_api +RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size
                                (p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_NOTIFY_INTEGRITY_FAILURE:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_NOTIFY_INTEGRITY_FAILURE:[UE:%u] with LC:%u",
                   pdcp_notify_integrity_failure.ue_index, pdcp_notify_integrity_failure.lc_id);

    SET_UE_INDEX(pdcp_notify_integrity_failure.ue_index);

    RRC_LLIM_TRACE(RRC_BRIEF,"[UE_INDEX:%u]", pdcp_notify_integrity_failure.ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_notify_integrity_failure.ue_index, &p_ue_ctx);
    
    if(PNULL != p_ue_ctx)
    {
        rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

        SET_CELL_INDEX(cell_index);
        /* SPR:15106 Fix Start */
        void *p_msg = rrc_llim_alloc_internal_msg(  RRC_UECC_MODULE_ID,
                    UECC_LLIM_NOTIFY_INTEGRITY_FAILURE,
                    (U16)(sizeof
                        (rrc_uecc_llim_notify_integrity_failure_t)));
        /* SPR:15106 Fix End */

        if (PNULL != p_msg)
        {
            rrc_uecc_llim_notify_integrity_failure_t *p_uecc_notify_int_fail =
               (rrc_uecc_llim_notify_integrity_failure_t *)((U8*)p_msg+RRC_API_HEADER_SIZE);

            memset_wrapper (p_uecc_notify_int_fail, RRC_NULL, sizeof(rrc_uecc_llim_notify_integrity_failure_t));

            p_uecc_notify_int_fail->cell_index = cell_index;

            p_uecc_notify_int_fail->ue_index = pdcp_notify_integrity_failure.ue_index;

            p_uecc_notify_int_fail->lc_id = pdcp_notify_integrity_failure.lc_id;

            rrc_send_message(p_msg, RRC_GET_UECC_MODULE_ID(pdcp_notify_integrity_failure.ue_index));
        }
    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING,"Invalid ue_index - ignore");
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/* SPR 4118 Stop */

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_count_wraparound_ind_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function  process RRC_PDCP_COUNT_WRAPAROUND_IND.
*                  message and call FSM.
*   RETURNS     : None
*
******************************************************************************/
void rrc_uemh_pdcp_count_wraparound_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_count_wraparound_ind_t uecc_llim_count_wraparound_ind;
    rrc_pdcp_count_wraparound_ind_t pdcp_count_wraparound_ind;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_count_wraparound_ind(
                                        &pdcp_count_wraparound_ind,
                                        (U8* )p_api
                                        + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_COUNT_WRAPAROUND_IND:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_COUNT_WRAPAROUND_IND:[UE:%u]",
                   pdcp_count_wraparound_ind.ue_index);
    SET_UE_INDEX(pdcp_count_wraparound_ind.ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_count_wraparound_ind.ue_index, &p_ue_ctx);
    
    if(PNULL != p_ue_ctx)
    {
        rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);
 
        SET_CELL_INDEX(cell_index);

        /* Init Operation */
        memset_wrapper(&uecc_llim_count_wraparound_ind, RRC_NULL,
                sizeof(rrc_uecc_llim_count_wraparound_ind_t));

        /* 2. send UECC_LLIM_COUNT_WRAPAROUND_IND */
        uecc_llim_count_wraparound_ind.cell_index = cell_index;

        uecc_llim_count_wraparound_ind.ue_index = 
            pdcp_count_wraparound_ind.ue_index;

        rrc_llim_send_internal_msg(RRC_GET_UECC_MODULE_ID(
                   uecc_llim_count_wraparound_ind.ue_index),
                UECC_LLIM_COUNT_WRAPAROUND_IND,
                sizeof(rrc_uecc_llim_count_wraparound_ind_t),
                &uecc_llim_count_wraparound_ind);
    }
    else
    {
        RRC_LLIM_TRACE(RRC_WARNING,"Invalid ue_index - ignore");
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_reset_ue_entity_req_process_msg 
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function partially process UECC_LLIM_RESET_UE_ENTITY_REQ 
*                  message and call FSM.
*   RETURNS      : None
******************************************************************************/
void rrc_uemh_uecc_llim_reset_ue_entity_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_reset_ue_entity_req_t *p_reset_ue_entity_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
                  (rrc_uecc_llim_reset_ue_entity_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_reset_ue_entity_req = 
        (rrc_uecc_llim_reset_ue_entity_req_t *)
            ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_reset_ue_entity_req->cell_index, p_reset_ue_entity_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_reset_ue_entity_req->ue_index, &p_ue_ctx);

    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_reset_ue_entity_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
            p_reset_ue_entity_req->ue_index);
    }
    /* Search in DB UE with p_reset_ue_entity_req->ue_index -should be present */
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
               p_reset_ue_entity_req->ue_index);
    }
    else
    {
        p_ue_ctx->curr_op_param.p_reset_ue = (rrc_uemh_reset_ue_op_param_t *)
            rrc_mem_get(sizeof(rrc_uemh_reset_ue_op_param_t));

        if(PNULL == p_ue_ctx->curr_op_param.p_reset_ue)
        {
            RRC_LLIM_TRACE(RRC_FATAL, "rrc_mem_get failed!!");
            return;
        }

        /* Clear buffer */
        memset_wrapper(p_ue_ctx->curr_op_param.p_reset_ue, 0,
            sizeof(rrc_uemh_reset_ue_op_param_t));
         
        /* Set flag for reset ue presence */
        p_ue_ctx->curr_op_param.p_reset_ue->transaction_id_presence = 
            RRC_TRUE;

        /* Store received transaction id */
        p_ue_ctx->curr_op_param.p_reset_ue->transaction_id = 
            p_reset_ue_entity_req->transaction_id; 
        
        /* Normal message processing */
        uemh_fsm_process_event(UEMH_EVENT_UECC_RESET_UE_ENTITY_REQ,
                                p_api,
                                p_reset_ue_entity_req,
                                p_ue_ctx,
                                p_rrc_llim_gl_ctx);
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/* SPS Start */
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_process_mac_reconfig_complete_ind_msg 
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function partially process UECC_LLIM_MAC_RECONFIG_COMPLETE_IND 
*                  message and call FSM.
*   RETURNS      : None
******************************************************************************/
void rrc_uemh_uecc_llim_process_mac_reconfig_complete_ind_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_mac_reconfig_complete_ind_t *p_uecc_llim_mac_reconfig_complete_ind = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    rrc_mac_reconfig_complete_ind_t    mac_reconfig_complete_ind;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
                  (rrc_uecc_llim_mac_reconfig_complete_ind_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_uecc_llim_mac_reconfig_complete_ind = 
        (rrc_uecc_llim_mac_reconfig_complete_ind_t *)
            ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_uecc_llim_mac_reconfig_complete_ind->cell_index, p_uecc_llim_mac_reconfig_complete_ind->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_uecc_llim_mac_reconfig_complete_ind->ue_index, &p_ue_ctx);

    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_uecc_llim_mac_reconfig_complete_ind->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
            p_uecc_llim_mac_reconfig_complete_ind->ue_index);
    }
    /* Search in DB UE with p_uecc_llim_mac_reconfig_complete_ind->ue_index -should be present */
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
               p_uecc_llim_mac_reconfig_complete_ind->ue_index);
    }
    else
    {

        mac_reconfig_complete_ind.ue_index = p_uecc_llim_mac_reconfig_complete_ind->ue_index;
        mac_reconfig_complete_ind.response = p_uecc_llim_mac_reconfig_complete_ind->response;

        if (RRC_SUCCESS != rrc_mac_il_send_rrc_mac_reconfig_complete_ind(
                &mac_reconfig_complete_ind,
                RRC_LLIM_MODULE_ID,
			RRC_GET_LL_MODULE_ID(RRC_MAC_1_MODULE_ID,p_uecc_llim_mac_reconfig_complete_ind->cell_index),
		0,
                p_uecc_llim_mac_reconfig_complete_ind->cell_index))
        {
            RRC_LLIM_TRACE(RRC_WARNING,"Unable to send rrc_mac_reconfig_complete_ind");
        }

    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/* SPS Stop */

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_drb_count_msg_req_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function process UECC_LLIM_DRB_COUNT_MSB_REQ message
*   RETURNS      : None
******************************************************************************/
void rrc_uemh_uecc_llim_drb_count_msg_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_drb_count_msb_req_t *p_uecc_drb_count_msg_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    void *p_msg = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    p_uecc_drb_count_msg_req = (rrc_uecc_llim_drb_count_msb_req_t *)((U8* )p_api +
        RRC_API_HEADER_SIZE);
    if (    rrc_get_api_buf_size(p_api) != (RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_drb_count_msb_req_t)))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    SET_CELL_AND_UE_INDEX(p_uecc_drb_count_msg_req->cell_index, p_uecc_drb_count_msg_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_uecc_drb_count_msg_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_uecc_drb_count_msg_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_uecc_drb_count_msg_req->cell_index);
    }
    /* Search in DB UE with p_srb_data_req->ue_index - should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                p_uecc_drb_count_msg_req->ue_index);
    }
    else
    {
        /* Fill RRC_PDCP_DRB_COUNT_MSB_REQ message */
        rrc_pdcp_drb_count_msb_req_t pdcp_drb_count_msb_req;

        if (PNULL != p_ue_ctx)
        {
            memset_wrapper (&pdcp_drb_count_msb_req, RRC_NULL, sizeof(rrc_pdcp_drb_count_msb_req_t));

            pdcp_drb_count_msb_req.ue_index          = p_uecc_drb_count_msg_req->ue_index;

            pdcp_drb_count_msb_req.num_drb = p_uecc_drb_count_msg_req->drb_list.drb_count;

            /* klocwork fix */
            if( pdcp_drb_count_msb_req.num_drb  <= MAX_LC_COUNT )
            {
                l3_memcpy_wrapper(pdcp_drb_count_msb_req.drb_list, p_uecc_drb_count_msg_req->drb_list.drb_info,
                    pdcp_drb_count_msb_req.num_drb* sizeof(rrc_pdcp_drb_info_t));
            }
            /* klocwork fix */

            /* Allocate buffer for storing RRC_PDCP_DRB_COUNT_MSB_REQ * message */
            p_msg = rrc_llim_alloc_external_msg(p_uecc_drb_count_msg_req->transaction_id,
				RRC_GET_LL_MODULE_ID(RRC_PDCP_1_MODULE_ID,p_uecc_drb_count_msg_req->cell_index),
			RRC_PDCP_MODULE_ID,
                    RRC_PDCP_DRB_COUNT_MSB_REQ,
                    (U16)(rrc_il_get_rrc_pdcp_drb_count_msb_req_len
                        (&pdcp_drb_count_msb_req)),
                    p_uecc_drb_count_msg_req->cell_index);

            if (PNULL != p_msg)
            {
                U8 *p_curr_pos = (U8*)p_msg + RRC_FULL_INTERFACE_HEADERS_SIZE;
                /* Compose RRC_PDCP_SRB_DATA_REQ */
                if ( RRC_FAILURE == (rrc_il_compose_rrc_pdcp_drb_count_msb_req(&p_curr_pos,
                        &pdcp_drb_count_msb_req)))
                {
                    RRC_LLIM_TRACE(RRC_WARNING,"compose_rrc_pdcp_drb_count_msb_req Fail[%d]",__LINE__);
                }

                rrc_send_message(p_msg, 
				RRC_GET_LL_MODULE_ID(RRC_PDCP_1_MODULE_ID,p_uecc_drb_count_msg_req->cell_index));
		}
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}

/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_drb_msb_resp
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function process RRC_PDCP_DRB_COUNT_MSB_REQ message.
*   RETURNS      : None
******************************************************************************/
void rrc_uemh_pdcp_drb_msb_resp
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_pdcp_drb_count_msb_resp_t pdcp_drb_count_msb_resp;
    S32                     length_read         = RRC_NULL;
    rrc_llim_ue_ctx_t       *p_ue_ctx           = PNULL;
    rrc_cell_index_t        cell_index          = RRC_INVALID_CELL_INDEX;
    U8                      index               = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Coverity Fix 83186 Start */
    memset_wrapper(&pdcp_drb_count_msb_resp, RRC_NULL,
                   sizeof(rrc_pdcp_drb_count_msb_resp_t));
    /* Coverity Fix 83186 Stop */

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_drb_count_msb_resp(  &pdcp_drb_count_msb_resp,
                                (U8* )p_api +RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size
                                (p_api) - RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_PDCP_DRB_COUNT_MSB_RESP:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_PDCP_DRB_COUNT_MSB_RESP:[UE:%u]",
                   pdcp_drb_count_msb_resp.ue_index);
    SET_UE_INDEX(pdcp_drb_count_msb_resp.ue_index);


    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_drb_count_msb_resp.ue_index, &p_ue_ctx);
    
    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE should be present */
    else if ((PNULL == p_ue_ctx))

    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                pdcp_drb_count_msb_resp.ue_index);
    }
    else
    {
        void *p_msg = rrc_llim_alloc_internal_msg(  RRC_UECC_MODULE_ID,
                UECC_LLIM_DRB_COUNT_MSB_RESP,
                (U16)(sizeof(rrc_uecc_llim_drb_count_msb_resp_t)));

        if (PNULL != p_ue_ctx)
        {
            if (PNULL != p_msg)
            {
                rrc_uecc_llim_drb_count_msb_resp_t *p_uecc_drb_count_msb_resp =
                    (rrc_uecc_llim_drb_count_msb_resp_t *)((U8*)p_msg+RRC_API_HEADER_SIZE);

                p_uecc_drb_count_msb_resp->cell_index         = cell_index;
                p_uecc_drb_count_msb_resp->ue_index           = pdcp_drb_count_msb_resp.
                    ue_index;
                p_uecc_drb_count_msb_resp->response           = pdcp_drb_count_msb_resp.
                    response;
                p_uecc_drb_count_msb_resp->transaction_id     = rrc_get_transaction_id(p_api);

                if ( (p_uecc_drb_count_msb_resp->response == RRC_SUCCESS) &&
                        pdcp_drb_count_msb_resp.num_drb != RRC_NULL)
                {
                    p_uecc_drb_count_msb_resp->bitmask |= UECC_LLIM_DRB_COUNTMSB_INFO_LIST_PRESENT;
                    p_uecc_drb_count_msb_resp->drb_countMSB_info_list.drb_count = 
                        pdcp_drb_count_msb_resp.num_drb;
                    for (index=0; index<pdcp_drb_count_msb_resp.num_drb; index++)
                    {
                        p_uecc_drb_count_msb_resp->drb_countMSB_info_list.drb_countMSB_info[index].
                            lc_id = pdcp_drb_count_msb_resp.drb_countMSB_info[index].lc_id;
                        if ( pdcp_drb_count_msb_resp.drb_countMSB_info[index].
                                optional_elems_present &  RRC_PDCP_COUNTMSB_UPLINK_PRESENT)
                        {
                            p_uecc_drb_count_msb_resp->drb_countMSB_info_list.drb_countMSB_info[index].
                                bitmask |= UECC_LLIM_DRB_COUNTMSB_UPLINK_PRESENT;
                            p_uecc_drb_count_msb_resp->drb_countMSB_info_list.drb_countMSB_info[index].
                                countMSB_Uplink = pdcp_drb_count_msb_resp.drb_countMSB_info[index].countMSB_Uplink.
                                countMSB_Uplink;
                        }
                        if ( pdcp_drb_count_msb_resp.drb_countMSB_info[index].
                                optional_elems_present &  RRC_PDCP_COUNTMSB_DOWNLINK_PRESENT)
                        {
                            p_uecc_drb_count_msb_resp->drb_countMSB_info_list.drb_countMSB_info[index].
                                bitmask |= UECC_LLIM_DRB_COUNTMSB_DOWNLINK_PRESENT;
                            p_uecc_drb_count_msb_resp->drb_countMSB_info_list.drb_countMSB_info[index].
                                countMSB_Downlink = pdcp_drb_count_msb_resp.drb_countMSB_info[index].countMSB_Downlink.
                                countMSB_Downlink;
                        }

                    }
                }

                rrc_send_message(p_msg, RRC_GET_UECC_MODULE_ID
                        (p_uecc_drb_count_msb_resp->ue_index));
            }
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}

/*SPR_19066_START*/
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_pdcp_ho_prep_info_resp_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function partially process PDCP_MAC_I_RESP message
*                  and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_pdcp_ho_prep_info_resp_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_pdcp_ho_prep_info_resp_t pdcp_ho_prep_info_resp;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_pdcp_ho_prep_info_resp( 
                                &pdcp_ho_prep_info_resp,
                                (U8* )p_api + RRC_FULL_INTERFACE_HEADERS_SIZE,
                                rrc_get_api_buf_size(p_api) -
                                RRC_FULL_INTERFACE_HEADERS_SIZE,
                                &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:PDCP->RRC:RRC_pdcp_ho_prep_info_resp:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:PDCP->RRC:RRC_pdcp_ho_prep_info_resp:[UE:%u] with Response:%u",
                   pdcp_ho_prep_info_resp.ue_index, pdcp_ho_prep_info_resp.response_code);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, pdcp_ho_prep_info_resp.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /* Search in DB UE with pdcp_ho_prep_info_resp.ue_index -
     *  should be present */
    else if ((PNULL == p_ue_ctx))
    {
        /* ignore */
        RRC_LLIM_TRACE(RRC_WARNING, "ue_index (%i) not found - ignore",
                pdcp_ho_prep_info_resp.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PDCP_HO_PREP_INFO_RESP,
                    p_api,
                    &pdcp_ho_prep_info_resp,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/*SPR_19066_END*/


/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_reest_ue_entity_comp_ind_process_msg 
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*   DESCRIPTION  : This function partially process
*                  UECC_LLIM_RE_ESTABLISH_UE_ENTITY_COMPLETE_IND 
*                  message and call FSM.
*   RETURNS      : None
******************************************************************************/
void rrc_uemh_uecc_llim_reest_ue_entity_comp_ind_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_re_establish_ue_entity_complete_ind_t 
        *p_reest_ue_entity_comp_ind = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
                  (rrc_uecc_llim_re_establish_ue_entity_complete_ind_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_reest_ue_entity_comp_ind = 
        (rrc_uecc_llim_re_establish_ue_entity_complete_ind_t *)
            ((U8* )p_api + RRC_API_HEADER_SIZE);

    SET_CELL_AND_UE_INDEX(p_reest_ue_entity_comp_ind->cell_index, p_reest_ue_entity_comp_ind->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_reest_ue_entity_comp_ind->ue_index, &p_ue_ctx);

    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_reest_ue_entity_comp_ind->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
            p_reest_ue_entity_comp_ind->ue_index);
    }
    /* Search in DB UE with p_reset_ue_entity_req->ue_index -should be present */
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
               p_reest_ue_entity_comp_ind->ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_REESTB_UE_ENTITY_COMP_IND,
                    p_api,
                    p_reest_ue_entity_comp_ind,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/*SPR:6462 start*/
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_rlc_reestab_ue_entity_comp_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process 
*                  RRC_RLC_REESTAB_UE_ENTITY_COMPLETE_CNF_PROCESS_MSG 
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_rlc_reestab_ue_entity_comp_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_rlc_re_establish_ue_entity_complete_cnf_t rlc_reestab_ue_entitiy_comp_cnf;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_rlc_re_establish_ue_entity_complete_cnf(
                                        &rlc_reestab_ue_entitiy_comp_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:RLC->RRC:RRC_RLC_REESTAB_UE_ENTITIY_COMP_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:RLC->RRC:RRC_RLC_REESTAB_UE_ENTITIY_COMP_CNF:[UE:%u] with response:%u",
                   rlc_reestab_ue_entitiy_comp_cnf.ue_index, rlc_reestab_ue_entitiy_comp_cnf.response);

    SET_UE_INDEX(rlc_reestab_ue_entitiy_comp_cnf.ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, rlc_reestab_ue_entitiy_comp_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_INDEX(cell_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /*Search in DB UE with rlc_reestab_ue_entitiy_comp_cnf.ue_index -should be found*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                rlc_reestab_ue_entitiy_comp_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            uemh_fsm_process_event (UEMH_EVENT_UECC_REESTB_UE_ENTITY_COMP_CNF, p_api,
                    &rlc_reestab_ue_entitiy_comp_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);

        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/*spr:6462 stop*/

/*BUG 604 changes start*/
/*BUG 604 changes start*/
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_s1u_intra_enb_data_fwd_req_process_msg 
*
*       This function partially process UECC_LLIM_S1U_INTRA_ENB_DATA_FWD_REQ 
*       message and call FSM.
*
*   RETURNS:
*       None
*
******************************************************************************/
void rrc_uemh_uecc_llim_s1u_intra_enb_data_fwd_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_s1u_intra_enb_data_fwd_req_t *p_intra_enb_data_fwd_req = 
       PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_s1u_intra_enb_data_fwd_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_intra_enb_data_fwd_req = 
        (rrc_uecc_llim_s1u_intra_enb_data_fwd_req_t *)
            ((U8* )p_api + RRC_API_HEADER_SIZE);
  
    SET_CELL_AND_UE_INDEX(p_intra_enb_data_fwd_req->cell_index, p_intra_enb_data_fwd_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_intra_enb_data_fwd_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_intra_enb_data_fwd_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_intra_enb_data_fwd_req->cell_index);
    }

    /* Search in DB UE with p_intra_enb_data_fwd_req->ue_index - 
       should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_intra_enb_data_fwd_req->ue_index);

    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_S1U_INTRA_ENB_DATA_FWD_REQ,
                    p_api,
                    p_intra_enb_data_fwd_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_uecc_llim_s1u_local_path_switch_req_process_msg 
*
*       This function partially process UECC_LLIM_S1U_LOCAL_PATH_SWITCH_REQ 
*       message and call FSM.
*
*   RETURNS:
*       None
*
******************************************************************************/
void rrc_uemh_uecc_llim_s1u_local_path_switch_req_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_uecc_llim_s1u_local_path_switch_req_t *p_s1u_path_switch_req = 
       PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
        (rrc_uecc_llim_s1u_local_path_switch_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_s1u_path_switch_req = 
        (rrc_uecc_llim_s1u_local_path_switch_req_t *)
            ((U8* )p_api + RRC_API_HEADER_SIZE);
  
    SET_CELL_AND_UE_INDEX(p_s1u_path_switch_req->cell_index, p_s1u_path_switch_req->ue_index);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_s1u_path_switch_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if((p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (PNULL == p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_s1u_path_switch_req->cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                p_s1u_path_switch_req->cell_index);
    }

    /* Search in DB UE with p_s1u_path_switch_req->ue_index - 
       should be present */
    else if ((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong ue_index [%u] passed - ignore.",
                p_s1u_path_switch_req->ue_index);

    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_UECC_S1U_LOCAL_PATH_SWITCH_REQ,
                    p_api,
                    p_s1u_path_switch_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_s1u_intra_enb_data_fwd_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process 
*                  S1U_INTRA_ENB_DATA_FWD_CNF 
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_s1u_intra_enb_data_fwd_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_s1u_intra_enb_data_fwd_cnf_t s1u_intra_enb_data_fwd_cnf;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_s1u_intra_enb_data_fwd_cnf(
                                        &s1u_intra_enb_data_fwd_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:S1U->RRC:RRC_S1U_INTRA_ENB_DATA_FWD_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:S1U->RRC:RRC_S1U_INTRA_ENB_DATA_FWD_CNF:[UE:%u] with response:%u",
                   s1u_intra_enb_data_fwd_cnf.ue_index, s1u_intra_enb_data_fwd_cnf.response);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, s1u_intra_enb_data_fwd_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_AND_UE_INDEX(cell_index, s1u_intra_enb_data_fwd_cnf.ue_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /*Search in DB UE with s1u_intra_enb_data_fwd_cnf.ue_index -should be found*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                s1u_intra_enb_data_fwd_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            uemh_fsm_process_event (UEMH_EVENT_UECC_S1U_INTRA_ENB_DATA_FWD_CNF, p_api,
                    &s1u_intra_enb_data_fwd_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);

        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/******************************************************************************
*   FUNCTION NAME: rrc_uemh_s1u_local_path_switch_cnf_process_msg
*   INPUT        : void                *p_api
*                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
*   OUTPUT       : none
*
*   DESCRIPTION  : This function  process 
*                  S1U_LOCAL_PATH_SWITCH_CNF 
*                  message and call FSM.
*
*   RETURNS      : None
*
******************************************************************************/
void rrc_uemh_s1u_local_path_switch_cnf_process_msg
(
    /* Pointer to input API buffer */
    void                *p_api,
    /* Pointer to the LLIM global context data */
    rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
 
    rrc_s1u_local_path_switch_cnf_t s1u_local_path_switch_cnf;
    rrc_llim_ue_ctx_t*   p_ue_ctx = PNULL ;
    S32 length_read = RRC_NULL;
    rrc_cell_index_t cell_index = RRC_INVALID_CELL_INDEX;


    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (RRC_SUCCESS != rrc_il_parse_rrc_s1u_local_path_switch_cnf(
                                        &s1u_local_path_switch_cnf,
                                        (U8* )p_api
                                        +
                                            RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        rrc_get_api_buf_size(p_api) -
                                        RRC_FULL_INTERFACE_HEADERS_SIZE,
                                        &length_read))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "API:S1U->RRC:RRC_S1U_LOCAL_PATH_SWITCH_CNF:Bad message - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }
    RRC_LLIM_TRACE(RRC_INFO,"API:S1U->RRC:RRC_S1U_LOCAL_PATH_SWITCH_CNF:[UE:%u] with response:%u]",
                   s1u_local_path_switch_cnf.ue_index, s1u_local_path_switch_cnf.response);

    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, s1u_local_path_switch_cnf.ue_index, &p_ue_ctx);

    rrc_llim_get_cell_index (p_rrc_llim_gl_ctx, p_api, &cell_index);

    SET_CELL_AND_UE_INDEX(cell_index, s1u_local_path_switch_cnf.ue_index);

    /* Check whether UE's Cell exists or not */
    if((RRC_INVALID_CELL_INDEX == cell_index) || (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || 
            (PNULL == p_rrc_llim_gl_ctx->p_p_cell_ctx[cell_index]))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Cell does not exist cell_index [%u] passed - ignore.",
                cell_index);
    }

    /*Search in DB UE with s1u_local_path_switch_cnf.ue_index -should be found*/
    else if((PNULL == p_ue_ctx))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"UE ctx for id %d not found!",
                s1u_local_path_switch_cnf.ue_index);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            uemh_fsm_process_event (UEMH_EVENT_UECC_S1U_LOCAL_PATH_SWITCH_CNF, p_api,
                    &s1u_local_path_switch_cnf,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);

        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/*BUG 604 changes stop*/

/* SPR 16087 Start */
/******************************************************************************
*   FUNCTION NAME:
*   uemh_send_s1u_end_marker_req
*
*   INPUTS       : rrc_uecc_llim_s1u_reconfigure_req_t *p_s1u_reconfigure_req   
*   OUTPUTS      : None
*   DESCRIPTION  :
*       This function sends delete forwarding tunnel request to S1U.
*
*   RETURNS:
*       None
*
******************************************************************************/
static void uemh_send_s1u_end_marker_req
(
    rrc_uecc_llim_s1u_reconfigure_req_t *p_s1u_reconfigure_req 
)
{
    rrc_s1u_reconfigure_ue_entity_req_t s1u_reconfigure_ue_entity_req;

    rrc_counter_t      lc_cntr;
    rrc_counter_t      tunnel_cntr;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_s1u_reconfigure_req);


    memset_wrapper(&s1u_reconfigure_ue_entity_req, 0, 
                              sizeof(rrc_s1u_reconfigure_ue_entity_req_t));
    
    /* Fill the msg 
    rrc_llim_set_cell_index (&s1u_reconfigure_ue_entity_req, RRC_S1U_MODULE_ID,
            RRC_S1U_RECONFIGURE_UE_ENTITY_REQ,
            p_s1u_reconfigure_req->cell_index); */

    s1u_reconfigure_ue_entity_req.ue_index = p_s1u_reconfigure_req->ue_index;

    if ( p_s1u_reconfigure_req->bitmask &
               RRC_UECC_LLIM_S1U_REL_SAP_REQ_LIST_PRESENT)
    {

        s1u_reconfigure_ue_entity_req.num_rel_sap_req =
            p_s1u_reconfigure_req->rel_sap_req_list.
                                    num_rel_sap_req;

        for ( lc_cntr = 0;
              lc_cntr < s1u_reconfigure_ue_entity_req.num_rel_sap_req;
              lc_cntr++ )
        {

            s1u_reconfigure_ue_entity_req.rel_sap_req[lc_cntr].
            lc_id =
            p_s1u_reconfigure_req->rel_sap_req_list.
            rel_sap_req[lc_cntr].lc_id;

            s1u_reconfigure_ue_entity_req.rel_sap_req[lc_cntr].
            num_release_tunnel_info =
            p_s1u_reconfigure_req->rel_sap_req_list.
            rel_sap_req[lc_cntr].tunnel_count;


            for ( tunnel_cntr = 0;
                  tunnel_cntr < s1u_reconfigure_ue_entity_req.
                  rel_sap_req[lc_cntr].num_release_tunnel_info;
                  tunnel_cntr ++ )
            {

                s1u_reconfigure_ue_entity_req.rel_sap_req[lc_cntr].
                release_tunnel_info[tunnel_cntr].teid_self =
                p_s1u_reconfigure_req->rel_sap_req_list.
                rel_sap_req[lc_cntr].release_tunnel_info[tunnel_cntr].
                teid_self;

                if ( p_s1u_reconfigure_req->rel_sap_req_list.
                     rel_sap_req[lc_cntr].release_tunnel_info[tunnel_cntr].
                     bitmask &
                     RRC_UECC_LLIM_S1U_RELEASE_TUNNEL_INFO_TRANSPORT_ADDRESS_PRESENT )

                {
                    s1u_reconfigure_ue_entity_req.rel_sap_req[lc_cntr].
                    release_tunnel_info[tunnel_cntr].bitmask |=
                    RRC_S1U_IE_RELEASE_TUNNEL_INFO_TRANSPORT_ADDR_PRESENT ;

                    s1u_reconfigure_ue_entity_req.rel_sap_req[lc_cntr].
                    release_tunnel_info[tunnel_cntr].transport_addr.data_length =
                    p_s1u_reconfigure_req->rel_sap_req_list.
                    rel_sap_req[lc_cntr].release_tunnel_info[tunnel_cntr].
                    transport_addr.data_length;

                    l3_memcpy_wrapper(s1u_reconfigure_ue_entity_req.
                          rel_sap_req[lc_cntr].release_tunnel_info[tunnel_cntr].
                          transport_addr.data,
                          p_s1u_reconfigure_req->rel_sap_req_list.
                          rel_sap_req[lc_cntr].release_tunnel_info[tunnel_cntr].
                          transport_addr.data,
                          s1u_reconfigure_ue_entity_req.rel_sap_req[lc_cntr].
                          release_tunnel_info[tunnel_cntr].transport_addr.
                          data_length);
 
                }
            } 
        }
    }
    
    if (!(p_s1u_reconfigure_req->bitmask &
                    RRC_UECC_LLIM_S1U_REL_SAP_REQ_LIST_PRESENT))
    {
        RRC_LLIM_TRACE(RRC_ERROR,
          "Delete forwarding tunnel request received from UECC without "
          "S1U_REL_SAP_REQ_LIST. Ignoring this message.");
        
        RRC_LLIM_UT_TRACE_EXIT();
        return ;
    }

    RRC_LLIM_TRACE(RRC_INFO,"API:RRC->S1U:RRC_S1U_RECONFIGURE_UE_ENTITY_REQ:[UE:%u]",
                   s1u_reconfigure_ue_entity_req.ue_index);

    /* Send the S1U_RECONFIGURE_UE_ENTITY_REQ  msg to S1U */
    rrc_s1u_il_send_rrc_s1u_reconfigure_ue_entity_req(
                               &s1u_reconfigure_ue_entity_req, 
                               RRC_LLIM_MODULE_ID,
                               		RRC_GET_LL_MODULE_ID(RRC_S1U_1_MODULE_ID,p_s1u_reconfigure_req->cell_index),
				   p_s1u_reconfigure_req->transaction_id,
                               p_s1u_reconfigure_req->cell_index);


    RRC_LLIM_UT_TRACE_EXIT();
}
/* SPR 16087 Stop */
/*SPR_19066_START*/
/******************************************************************************
 *   FUNCTION NAME: rrc_uemh_uecc_llim_ho_prep_info_req_process_msg
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function partially process UECC_LLIM_HO_PREP_INFO_REQ
 *                  message and call FSM.
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
void rrc_uemh_uecc_llim_ho_prep_info_req_process_msg
(
 /* Pointer to input API buffer */
 void                *p_api,
 /* Pointer to the LLIM global context data */
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 )
{
    rrc_uecc_llim_ho_prep_info_req_t *p_ho_prep_info_req = PNULL;
    rrc_llim_ue_ctx_t *p_ue_ctx = PNULL;
    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_api);
    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    /* Precondition - p_api should point to right message */
    if (rrc_get_api_buf_size(p_api) != RRC_API_HEADER_SIZE + sizeof
            (rrc_uecc_llim_ho_prep_info_req_t))
    {
        RRC_LLIM_TRACE(RRC_WARNING, "Wrong message size - ignore.");
        RRC_LLIM_UT_TRACE_EXIT();
        return;
    }

    p_ho_prep_info_req = (rrc_uecc_llim_ho_prep_info_req_t *)((U8* )p_api +
            RRC_API_HEADER_SIZE);


    rrc_llim_get_ue_context(p_rrc_llim_gl_ctx, p_ho_prep_info_req->ue_index, &p_ue_ctx);
    /* Check whether UE's Cell exists or not */
    if ( (p_rrc_llim_gl_ctx->p_p_cell_ctx == PNULL) || (p_rrc_llim_gl_ctx->
                p_p_cell_ctx[p_ho_prep_info_req->cell_index] == PNULL))
    {
        RRC_LLIM_TRACE(RRC_WARNING,"CELL ctx for id %d not found!",
                p_ho_prep_info_req->cell_index);
    }
    /* Search in DB UE with p_ho_prep_info_req->ue_index - should be present */
    else if((PNULL == p_ue_ctx))
    {
        rrc_uecc_llim_ho_prep_info_resp_t ho_prep_info_resp;
        memset_wrapper(&ho_prep_info_resp, 0, sizeof(ho_prep_info_resp));

        ho_prep_info_resp.ue_index = p_ho_prep_info_req->ue_index;
        ho_prep_info_resp.response_code = RRC_LLIM_UE_IS_ABSENT;

        rrc_llim_send_internal_msg( RRC_GET_UECC_MODULE_ID(p_ho_prep_info_req->ue_index),
                UECC_LLIM_HO_PREP_INFO_RESP,
                sizeof(ho_prep_info_resp), &ho_prep_info_resp);
    }
    else
    {
        if (PNULL != p_ue_ctx)
        {
            /* Normal message processing */
            uemh_fsm_process_event( UEMH_EVENT_PDCP_HO_PREP_INFO_REQ,
                    p_api,
                    p_ho_prep_info_req,
                    p_ue_ctx,
                    p_rrc_llim_gl_ctx);
        }
    }
    RRC_LLIM_UT_TRACE_EXIT();
}
/*SPR_19066_END*/

/* Bug 4450 Start */
/******************************************************************************
 *   FUNCTION NAME: rrc_uemh_cleanup_ues
 *   INPUT        : void                *p_api
 *                  rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
 *   OUTPUT       : none
 *
 *   DESCRIPTION  : This function processes Ue Cleanup message.
 *
 *   RETURNS      : None
 *
 ******************************************************************************/
void rrc_uemh_cleanup_ues
(
 void                *p_api,
 rrc_llim_gl_ctx_t   *p_rrc_llim_gl_ctx
)
{
    rrc_llim_ue_ctx_t *p_ue_ctx          = PNULL;
    rrc_ue_index_t ue_index              = RRC_NULL;
    U8        uecc_index = RRC_NULL;
    U16       total_ue_supported = RRC_NULL;
    U16       cell_index = RRC_NULL;

    RRC_LLIM_UT_TRACE_ENTER();

    RRC_ASSERT(PNULL != p_rrc_llim_gl_ctx);

    if (p_rrc_llim_gl_ctx->p_p_uecc_ctx != PNULL)
    {
        for (cell_index=0; cell_index<MAX_NUM_CELLS; cell_index++)
        {
            total_ue_supported = (U16)(total_ue_supported + p_rrc_llim_gl_ctx->
                    max_num_supported_ue[cell_index]);
        }
        /* For each UECC */
        for(uecc_index=0; uecc_index<p_rrc_llim_gl_ctx->no_of_uecc_instances;
                uecc_index++)
        {
            if(p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index] != PNULL)
            {
                if(p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index]->p_p_ue_ctx != PNULL)
                {
                    /* Check and clean context of Max supported UE per UECC */
                    for (ue_index = 0; ue_index < total_ue_supported; ue_index++)
                    {
                        p_ue_ctx = p_rrc_llim_gl_ctx->p_p_uecc_ctx[uecc_index]->p_p_ue_ctx[ue_index];

                        if (PNULL != p_ue_ctx)
                        {
                            /* Process this special message */
                            uemh_fsm_process_event( UEMH_EVENT_OAMH_CLEANUP_REQ,/*coverity fix start*/
                                    p_api,
                                    p_api,/*coverity fix stop*/
                                    p_ue_ctx,
                                    p_rrc_llim_gl_ctx);
                        }
                    }
                }
            }
        }
    }

    RRC_LLIM_UT_TRACE_EXIT();
}
/* Bug 4450 End */
