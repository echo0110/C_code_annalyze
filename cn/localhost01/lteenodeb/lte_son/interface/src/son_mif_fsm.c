/****************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * File Details
 * ------------
 *  $Id: son_mif_fsm.c gur25767  Exp$
 *
 ****************************************************************************
 *
 * File Description: This file comtains the SON MgmtIfH FSM
 *
 ****************************************************************************
 * Revision Details
 *----------------------
 *   DATE            AUTHOR       REFERENCE       REASON
 *   Mar, 2012       Ritika       Initial         Initial
 *   Dec, 2012       Ravi         MRO Support     Added Code for MRO
 *   Jan, 2013       Sajida       MLB Support     Added Code for MLB
 *   June, 2014      Shilpi       SON Generic     SPR 11639 Fix
 *   Aug, 2014       Shilpi                       SPR 13251 Fix
 ******************************************************************************/

/****************************************************************************
* Project Includes
****************************************************************************/

#include <son_utils.h>
#include <son_defines.h>
#include <son_mif_fsm.h>
#include <son_oam_intf.h>
#include <son_oam_parser.h>
#include <son_oam_composer.h>

/****************************************************************************
* Global Variables
****************************************************************************/

/* Variable storing the SMIF name for logging */
extern const son_8 *p_son_mif_facility_name;
const son_8 *smif_fsm_name = "MIF_FSM";

/* Global variable storing the status (Enabled/Disabled) of SMIF logging */
extern son_oam_log_on_off_et g_mif_log_on_off;

extern const QSHELL son_shell;
extern son_void_t *p_listener;

/**
 * Private Variables
 */
const S8 *SMIF_FSM_STATES_NAMES[] =
{
    (const S8 *)"SMIF_NULL",
    (const S8 *)"SMIF_STATE_INIT",
    (const S8 *)"SMIF_STATE_W_FOR_SHUTDOWN_RESP",
    (const S8 *)"SMIF_STATE_ACTIVE",
    (const S8 *)"SMIF_NUM_OF_STATES",
};

const S8 *SMIF_FSM_EVENTS_NAMES[] =
{
    (const S8 *)"SMIF_EVENT_SONNMM_INIT_IND",
    (const S8 *)"SMIF_EVENT_SONAPPS_INIT_IND",
    (const S8 *)"SMIF_EVENT_SONANR_INIT_IND",
    (const S8 *)"SMIF_EVENT_SONES_INIT_IND",
    (const S8 *)"SMIF_EVENT_SONX2_INIT_IND",
    (const S8 *)"SMIF_EVENT_SONMRO_INIT_IND",
    (const S8 *)"SMIF_EVENT_SONMLB_INIT_IND",
/* RACH OPT CHANGES : STARTS */
    (const S8 *)"SMIF_EVENT_SONRACH_OPT_INIT_IND",
/* RACH OPT CHANGES : ENDS */
    (const S8 *)"SMIF_EVENT_OAM_INIT_CNF",
    (const S8 *)"SMIF_EVENT_SONNMM_INIT_CONFIG_RESP",
    (const S8 *)"SMIF_EVENT_SONAPPS_INIT_CONFIG_RESP",
    (const S8 *)"SMIF_EVENT_SONANR_INIT_CONFIG_RESP",
    (const S8 *)"SMIF_EVENT_SONES_INIT_CONFIG_RESP",
    (const S8 *)"SMIF_EVENT_SONX2_INIT_CONFIG_RESP",
    (const S8 *)"SMIF_EVENT_SONMRO_INIT_CONFIG_RESP",
    (const S8 *)"SMIF_EVENT_SONMLB_INIT_CONFIG_RESP",
/* RACH OPT CHANGES : STARTS */
    (const S8 *)"SMIF_EVENT_SONRACH_OPT_INIT_CONFIG_RESP",
/* RACH OPT CHANGES : ENDS */
    (const S8 *)"SMIF_EVENT_SHUTDOWN_REQ",
    (const S8 *)"SMIF_EVENT_SONNMM_SHUTDOWN_RESP",
    (const S8 *)"SMIF_EVENT_SONAPPS_SHUTDOWN_RESP",
    (const S8 *)"SMIF_EVENT_SONANR_SHUTDOWN_RESP",
    (const S8 *)"SMIF_EVENT_SONES_SHUTDOWN_RESP",
    (const S8 *)"SMIF_EVENT_SONX2_SHUTDOWN_RESP",
    (const S8 *)"SMIF_EVENT_SONMRO_SHUTDOWN_RESP",
    (const S8 *)"SMIF_EVENT_SONMLB_SHUTDOWN_RES",
/* RACH OPT CHANGES : STARTS */
    (const S8 *)"SMIF_EVENT_SONRACH_OPT_SHUTDOWN_RES",
/* RACH OPT CHANGES : ENDS */
    /* Rel 3.0 Cell delete support changes Start */
    (const S8 *)"SMIF_EVENT_SONANR_CELL_DELETE_RES",
    (const S8 *)"SMIF_EVENT_SONES_CELL_DELETE_RES",
#ifdef SON_MRO_FEATURE
    (const S8 *)"SMIF_EVENT_SONMRO_CELL_DELETE_RES",
#endif
    (const S8 *)"SMIF_EVENT_SONMLB_CELL_DELETE_RES",
    (const S8 *)"SMIF_EVENT_SONRACH_OPT_CELL_DELETE_RES",
    /* Rel 3.0 Cell delete support changes Stop */
    /* SPR-13251 Fix Starts */
    (const S8 *)"SMIF_EVENT_SONNMM_GET_LOG_LEVEL_RES",
    (const S8 *)"SMIF_EVENT_SONAPPS_GET_LOG_LEVEL_RES",
    (const S8 *)"SMIF_EVENT_SONANR_GET_LOG_LEVEL_RES",
    (const S8 *)"SMIF_EVENT_SONES_GET_LOG_LEVEL_RES",
#ifdef SON_MRO_FEATURE
    (const S8 *)"SMIF_EVENT_SONMRO_GET_LOG_LEVEL_RES",
#endif
    (const S8 *)"SMIF_EVENT_SONMLB_GET_LOG_LEVEL_RES",
    (const S8 *)"SMIF_EVENT_SONRACH_OPT_GET_LOG_LEVEL_RES",
    /* SPR-13251 Fix Ends */

    (const S8 *)"SMIF_NUM_OF_EVENTS",
    (const S8 *)"SMIF_INVALID_EVENT"
};

/*****************************************************************************
 * Function Name  : son_mif_send_internal_msg
 * Inputs         : Destination module identifier
 *            API Id
 *            Size of message (payload)
 *            Message specific info (payload)
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function sends message to SON internal modules
 ****************************************************************************/
son_return_et
son_mif_send_internal_msg
(
    son_module_id_et dst_module_id,
    U16 api_id,
    U16 msg_size,
    void         *p_msg
)
{
    U8 *p_api          = SON_PNULL;
    U16 msg_api_length = (U16)(msg_size + SON_API_HEADER_SIZE);

    SON_UT_TRACE_ENTER();

    /* Allocate buffer */
    p_api = son_msg_mem_get(msg_api_length);

    if (SON_PNULL == p_api)
    {
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    SON_MEMSET(p_api, 0, msg_api_length);

    son_construct_cspl_header(p_api, SON_VERSION_ID, SON_MIF_MODULE_ID,
        dst_module_id, api_id, 0, msg_size);
    if (SON_PNULL != p_msg)
    {
        SON_MEMCPY(p_api + SON_API_HEADER_SIZE, p_msg, msg_size);
    }

    /* Send message to destination module*/
    son_send_msg(p_api, dst_module_id);

    SON_UT_TRACE_EXIT();

    return SON_SUCCESS;
} /* son_mif_send_internal_msg */
/*SPR_19279_START*/
son_return_et
son_mif_send_internal_cell_info_ind_msg
(
    son_module_id_et dst_module_id,
    U16 api_id,
    U16 msg_size,
    void         *p_msg,
    U8 cell_index
)
{
    U8 *p_api          = SON_PNULL;
    U16 msg_api_length = (U16)(msg_size + SON_API_HEADER_SIZE + 1);

    SON_UT_TRACE_ENTER();

    /* Allocate buffer */
    p_api = son_msg_mem_get(msg_api_length);

    if (SON_PNULL == p_api)
    {
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    son_construct_cspl_header(p_api, SON_VERSION_ID, SON_MIF_MODULE_ID,
        dst_module_id, api_id, 0, msg_size+1);

    if (SON_PNULL != p_msg)
    {
        SON_MEMCPY(p_api + SON_API_HEADER_SIZE, p_msg, msg_size);
        SON_MEMCPY(p_api + SON_API_HEADER_SIZE + msg_size, &cell_index, 1);
    }

    /* Send message to destination module*/
    son_send_msg(p_api, dst_module_id);

    SON_UT_TRACE_EXIT();

    return SON_SUCCESS;
} /* son_mif_send_internal_msg */
/*SPR_19279_END*/


/*****************************************************************************
 * Function Name  : son_mif_reset_context
 * Inputs         : Pointer to the SMIF global context
 * Outputs        : None
 * Returns        : void
 * Description    : This function resets the SON MIF global context
 ****************************************************************************/
void
son_mif_reset_context
(
    son_mif_gb_context_t *p_smif_gb_context
)
{
    SON_UT_TRACE_ENTER();

    p_smif_gb_context->init_ind             = 0;
    p_smif_gb_context->init_config_resp_status = 0;
    p_smif_gb_context->shutdown_status      = 0;
    p_smif_gb_context->oam_init_ind_duration = SMIF_OAM_INIT_IND_TIMER_DURATION;
    p_smif_gb_context->oam_init_ind_retry_count = 0;
    /* Rel 3.0 Cell delete support changes Start */
    p_smif_gb_context->cell_delete_resp_status = 0;
    p_smif_gb_context->cell_delete_req_in_process = 0;
    /* Rel 3.0 Cell delete support changes Stop */

    /* If the timer for SON_OAM_INIT_IND is running, then stop it */
    if (SON_PNULL != p_smif_gb_context->oam_init_ind_timer)
    {
        son_stop_timer(p_smif_gb_context->oam_init_ind_timer);
        p_smif_gb_context->oam_init_ind_timer = SON_PNULL;
    }

    /* If the timer for Shutdown responses from SON internal modules is running
     * then stop it */
    if (SON_PNULL != p_smif_gb_context->son_shutdown_timer)
    {
        son_stop_timer(p_smif_gb_context->son_shutdown_timer);
        p_smif_gb_context->son_shutdown_timer = SON_PNULL;
    }

    /* Rel 3.0 Cell delete support changes Start */
    SON_MEMSET(&p_smif_gb_context->cell_delete_res, 0,
            sizeof(son_cell_delete_resp_t));
    /* Rel 3.0 Cell delete support changes Stop */

    /* Reset the FSM state to SMIF_NULL */
    SMIF_FSM_INIT(p_smif_gb_context);

    /* SPR-13251 Fix Starts */
    SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
            sizeof(son_oam_get_log_level_resp_t));
    p_smif_gb_context->expected_get_log_level_resp = SON_NULL;        
    p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
    /* SPR-13251 Fix Ends */
    SON_UT_TRACE_EXIT();
} /* son_mif_reset_context */

/*****************************************************************************
 * Function Name  : son_mif_check_for_smif_init_state
 * Inputs         : Pointer to the SMIF global context
 * Outputs        : None
 * Returns        : void
 * Description    : This function checks preconditions for switching FSM to
 *            SMIF_STATE_INIT state and perform state's switching
 *            if preconditions pass
 ****************************************************************************/
static void
son_mif_check_for_smif_init_state
(
    son_mif_gb_context_t *p_smif_gb_context
)
{
    son_timer_buf_t timer_data;

    SON_UT_TRACE_ENTER();

    /* Check if init_indication messages are received from all SON internal
     * modules(NMM, APPS, ANR, ES, X2) */
    if ((p_smif_gb_context->init_ind & SMIF_SONNMM_INIT_RECD)
        && (p_smif_gb_context->init_ind & SMIF_SONAPPS_INIT_RECD)
        && (p_smif_gb_context->init_ind & SMIF_SONANR_INIT_RECD)
        && (p_smif_gb_context->init_ind & SMIF_SONES_INIT_RECD)
        && (p_smif_gb_context->init_ind & SMIF_SONX2_INIT_RECD)
#ifdef SON_MRO_FEATURE
        && (p_smif_gb_context->init_ind & SMIF_SONMRO_INIT_RECD)
#endif
        && (p_smif_gb_context->init_ind & SMIF_SONMLB_INIT_RECD)
/* RACH OPT CHANGES : STARTS */
        && (p_smif_gb_context->init_ind & SMIF_SONRACH_OPT_INIT_RECD))
/* RACH OPT CHANGES : ENDS */
    {
        /* If oam_init_ind_timer does not exist */
        if (SON_PNULL == p_smif_gb_context->oam_init_ind_timer)
        {
            /* Send SON_OAM_INIT_IND to OAM */
            son_oam_send_son_oam_init_ind(
                SON_MIF_MODULE_ID,
                SON_NULL);

            timer_data.timer_type = SON_INIT_IND_TIMER;
            /* Start repeated timer for sending SON_OAM_INIT_IND to OAM*/
            p_smif_gb_context->oam_init_ind_timer =
                son_start_timer(p_smif_gb_context->oam_init_ind_duration,
                    &timer_data,
                    sizeof(timer_data),
                    SON_TRUE);

            /* Increment the retry count */
            p_smif_gb_context->oam_init_ind_retry_count++;
        }
        else
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
                "oam_init_ind_timer already exist!");
        }

        /*Reset current state's init inidication flag*/
        p_smif_gb_context->init_ind = 0;

        /*Change state*/
        SMIF_SET_FSM_STATE(p_smif_gb_context, SMIF_STATE_INIT);

        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "Init indication received from all internal modules. State changed to "
            "%s", SMIF_FSM_STATES_NAMES[SMIF_GET_FSM_STATE(p_smif_gb_context)]);
    }

    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "Init indication not received from all internal modules. State not "
            "changed!");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_check_for_smif_init_state */

/*****************************************************************************
 * Function Name  : son_mif_sonnmm_init_ind_handler
 * Inputs         : Pointer to the SMIF global context
 *                    Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONNMM_MIF_INIT_IND msg
 ****************************************************************************/
static void
son_mif_sonnmm_init_ind_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void             *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of init indication from NMM in SMIF global
     * context */
    p_smif_gb_context->init_ind |= SMIF_SONNMM_INIT_RECD;

    /* Check if init indication has been received for all SON internal modules
     * then change the FSM state*/
    son_mif_check_for_smif_init_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonapps_init_ind_handler
 * Inputs         : Pointer to the SMIF global context
 *                    Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONAPPS_SMIF_INIT_IND msg
 ****************************************************************************/
static void
son_mif_sonapps_init_ind_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of init indication from APPS in SMIF
     * global context */
    p_smif_gb_context->init_ind |= SMIF_SONAPPS_INIT_RECD;

    /* Check if init indication has been received for all SON internal modules
     * then change the FSM state*/
    son_mif_check_for_smif_init_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonanr_init_ind_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONANR_SMIF_INIT_IND msg
 ****************************************************************************/
static void
son_mif_sonanr_init_ind_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_DETAILEDALL,
        "Unused variable p_msg=%p",p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of init indication from ANR in SMIF global
     * context */
    p_smif_gb_context->init_ind |= SMIF_SONANR_INIT_RECD;

    /* Check if init indication has been received for all SON internal modules
     * then change the FSM state*/
    son_mif_check_for_smif_init_state(p_smif_gb_context);
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sones_init_ind_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONES_SMIF_INIT_IND msg
 ****************************************************************************/
static void
son_mif_sones_init_ind_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of init indication from ES in SMIF global
     * context */
    p_smif_gb_context->init_ind |= SMIF_SONES_INIT_RECD;

    /* Check if init indication has been received for all SON internal modules
     * then change the FSM state*/
    son_mif_check_for_smif_init_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonx2_init_ind_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONX2_SMIF_INIT_IND msg
 ****************************************************************************/
static void
son_mif_sonx2_init_ind_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of init indication from X2 in SMIF global
     * context */
    p_smif_gb_context->init_ind |= SMIF_SONX2_INIT_RECD;

    /* Check if init indication has been received for all SON internal modules
     * then change the FSM state*/
    son_mif_check_for_smif_init_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

#ifdef SON_MRO_FEATURE

/*****************************************************************************
 * Function Name  : son_mif_sonmro_init_ind_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONMRO_SMIF_INIT_IND msg
 ****************************************************************************/
static void
son_mif_sonmro_init_ind_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of init indication from MRO in SMIF global
     * context */
    p_smif_gb_context->init_ind |= SMIF_SONMRO_INIT_RECD;

    /* Check if init indication has been received for all SON internal modules
     * then change the FSM state*/
    son_mif_check_for_smif_init_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

#endif /* ifdef SON_MRO_FEATURE */

/*****************************************************************************
 * Function Name  : son_mif_sonmlb_init_ind_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONMLB_SMIF_INIT_IND msg
 ****************************************************************************/
static void
son_mif_sonmlb_init_ind_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of init indication from MLB in SMIF global
     * context */
    p_smif_gb_context->init_ind |= SMIF_SONMLB_INIT_RECD;

    /* Check if init indication has been received for all SON internal modules
     * then change the FSM state*/
    son_mif_check_for_smif_init_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/* RACH OPT CHANGES : STARTS */
/*****************************************************************************
 * Function Name  : son_mif_sonrach_opt_init_ind_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for RACH OPT SMIF_INIT_IND msg
 ****************************************************************************/
static void
son_mif_sonrach_opt_init_ind_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of init indication from RACH OPT in 
       SMIF global context */
    p_smif_gb_context->init_ind |= SMIF_SONRACH_OPT_INIT_RECD;

    /* Check if init indication has been received for all SON internal modules
     * then change the FSM state*/
    son_mif_check_for_smif_init_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}
/* RACH OPT CHANGES : ENDS */

/*****************************************************************************
 * Function Name  : son_mif_oam_init_cnf_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for the event
 *                  SMIF_EVENT_OAM_INIT_CNF i.e. the receipt of
 *                  SON_OAM_INIT_CONFIG_RESP msg from OAM which serves as the
 *                  confirmation for SON_OAM_INIT_IND msg sent to OAM from SON.
 *                    Upon occurence of this event, SON stops sending
 *                  SON_OAM_INIT_IND to OAM
 ****************************************************************************/
static void
son_mif_oam_init_cnf_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();

    /* Stop oam_init_ind timer if exist */
    if (SON_PNULL != p_smif_gb_context->oam_init_ind_timer)
    {
        son_stop_timer(p_smif_gb_context->oam_init_ind_timer);
        p_smif_gb_context->oam_init_ind_timer = SON_PNULL;
    }

    son_mif_oam_init_config_req(p_msg, p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_oam_shutdown_req_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SON_OAM_SHUTDOWN_REQ msg
 ****************************************************************************/
static void
son_mif_oam_shutdown_req_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    son_oam_shutdown_req_t shutdown_req;
    son_timer_buf_t timer_data;
    S32 length_read = SON_NULL;
    STACKAPIHDR cspl_hdr = {0};

    /* Get the API Id and source module ID */
    son_parse_cspl_header(p_msg, &cspl_hdr);

    SON_UT_TRACE_ENTER();

    if (SON_SUCCESS !=
        son_parse_son_oam_shutdown_req(
            &shutdown_req,
            (U8 *)p_msg + SON_FULL_INTERFACE_HEADERS_SIZE,
            cspl_hdr.paramlen - EXT_MSG_API_HDR_SIZE,
            &length_read))
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
            "Bad message - ignore.");
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Stop oam_init_ind timer if exist */
    if (SON_PNULL != p_smif_gb_context->oam_init_ind_timer)
    {
        son_stop_timer(p_smif_gb_context->oam_init_ind_timer);
        p_smif_gb_context->oam_init_ind_timer = SON_PNULL;
    }

    /* Start shutdown_timer, in Graceful Case only */
    if (SON_GRACEFUL_SHUTDOWN == shutdown_req.shutdown_type)
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "Graceful Shutdown received from OAM : Start Shutdown Timer");

        timer_data.timer_type = SON_SHUTDOWN_TIMER;
        p_smif_gb_context->son_shutdown_timer = son_start_timer(
            (U16)shutdown_req.time_to_shutdown,
            &timer_data,
            sizeof(timer_data),
            SON_FALSE);
    }
    else
    {
        /* No Need to start timer */
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "Forceful Shutdown received from OAM");
    }

    /* Send shutdown request message to internal SON modules */
    son_mif_send_internal_msg(SON_NMM_MODULE_ID,
        SMIF_SHUTDOWN_REQ,
        0,
        SON_PNULL);

    son_mif_send_internal_msg(SON_APPS_MODULE_ID,
        SMIF_SHUTDOWN_REQ,
        0,
        SON_PNULL);

    son_mif_send_internal_msg(SON_ANR_MODULE_ID,
        SMIF_SHUTDOWN_REQ,
        0,
        SON_PNULL);

    son_mif_send_internal_msg(SON_ES_MODULE_ID,
        SMIF_SHUTDOWN_REQ,
        0,
        SON_PNULL);
    son_mif_send_internal_msg(SON_X2_MODULE_ID,
        SMIF_SHUTDOWN_REQ,
        0,
        SON_PNULL);
#ifdef SON_MRO_FEATURE
    son_mif_send_internal_msg(SON_MRO_MODULE_ID,
        SMIF_SHUTDOWN_REQ,
        0,
        SON_PNULL);
#endif
    son_mif_send_internal_msg(SON_MLB_MODULE_ID,
        SMIF_SHUTDOWN_REQ,
        0,
        SON_PNULL);
/* RACH OPT CHANGES : STARTS */
    son_mif_send_internal_msg(SON_RACH_OPT_MODULE_ID,
        SMIF_SHUTDOWN_REQ,
        0,
        SON_PNULL);
/* RACH OPT CHANGES : ENDS */

    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF,
        "Shutdown Request send to all SON internal modules");
    /*Change state to wait for Shutdown responses from internal modules*/
    SMIF_SET_FSM_STATE(p_smif_gb_context, SMIF_STATE_W_FOR_SHUTDOWN_RESP);

    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
        "State changed to %s",
        SMIF_FSM_STATES_NAMES[SMIF_GET_FSM_STATE(p_smif_gb_context)]);

    SON_UT_TRACE_EXIT();
} /* son_mif_oam_shutdown_req_handler */

/*****************************************************************************
 * Function Name  : son_mif_check_for_smif_active_state
 * Inputs         : Pointer to the SMIF global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks preconditions for switching FSM to
 *                  SMIF_STATE_ACTIVE state and perform state's switching
 *                  if preconditions are satistfied
 ****************************************************************************/
static void
son_mif_check_for_smif_active_state
(
    son_mif_gb_context_t *p_smif_gb_context
)
{
    SON_UT_TRACE_ENTER();

    /* Check if smif_init_config_resp messages received from all SON modules */
    if ((p_smif_gb_context->init_config_resp_status &
         SMIF_SONNMM_INIT_CONFIG_RESP_RECD)
        && (p_smif_gb_context->init_config_resp_status &
            SMIF_SONAPPS_INIT_CONFIG_RESP_RECD)
        && (p_smif_gb_context->init_config_resp_status &
            SMIF_SONANR_INIT_CONFIG_RESP_RECD)
        && (p_smif_gb_context->init_config_resp_status &
            SMIF_SONES_INIT_CONFIG_RESP_RECD)
#ifdef SON_MRO_FEATURE
        && (p_smif_gb_context->init_config_resp_status &
            SMIF_SONMRO_INIT_CONFIG_RESP_RECD)
#endif
        && (p_smif_gb_context->init_config_resp_status &
            SMIF_SONMLB_INIT_CONFIG_RESP_RECD)
/* RACH OPT CHANGES : STARTS */
        && (p_smif_gb_context->init_config_resp_status &
            SMIF_SONRACH_OPT_INIT_CONFIG_RESP_RECD))
/* RACH OPT CHANGES : ENDS */
    {
        /*Reset current state's flags*/
        p_smif_gb_context->init_config_resp_status = 0;
        /*Change state*/
        SMIF_SET_FSM_STATE(p_smif_gb_context, SMIF_STATE_ACTIVE);

        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "Init config response received from all internal modules. State "
            "changed to %s",
            SMIF_FSM_STATES_NAMES[SMIF_GET_FSM_STATE(p_smif_gb_context)]);
    }

    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "Init config response not received from all internal modules. State "
            "not changed!");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_check_for_smif_active_state */

/*****************************************************************************
 * Function Name  : son_mif_check_for_shutdown_timer_stop
 * Inputs         : Pointer to the SMIF global context
 * Outputs        : None
 * Returns        : None
 * Description    : This function checks preconditions for stoping the
 *                  shutdown timer and stop the timer if preconditions are
 *            satisfied
 ****************************************************************************/
static void
son_mif_check_for_shutdown_timer_stop
(
    son_mif_gb_context_t *p_smif_gb_context
)
{
    son_oam_shutdown_resp_t shutdown_resp;

    SON_UT_TRACE_ENTER();

    /* Check if shutdown_resp messages received from all SON modules */
    if ((p_smif_gb_context->shutdown_status & SMIF_SONNMM_SHUTDOWN_RECD)
        && (p_smif_gb_context->shutdown_status & SMIF_SONAPPS_SHUTDOWN_RECD)
        && (p_smif_gb_context->shutdown_status & SMIF_SONANR_SHUTDOWN_RECD)
        && (p_smif_gb_context->shutdown_status & SMIF_SONES_SHUTDOWN_RECD)
        && (p_smif_gb_context->shutdown_status & SMIF_SONX2_SHUTDOWN_RECD)
#ifdef SON_MRO_FEATURE
        && (p_smif_gb_context->shutdown_status & SMIF_SONMRO_SHUTDOWN_RECD)
#endif
        && (p_smif_gb_context->shutdown_status & SMIF_SONMLB_SHUTDOWN_RECD)
/* RACH OPT CHANGES : STARTS */
        && (p_smif_gb_context->shutdown_status & SMIF_SONRACH_OPT_SHUTDOWN_RECD))
/* RACH OPT CHANGES : ENDS */
    {
        /*Reset current state's flags*/
        p_smif_gb_context->shutdown_status = 0;
        /*Change state*/
        SMIF_SET_FSM_STATE(p_smif_gb_context, SMIF_NULL);

        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "Shutdown response received from all internal modules. State "
            "changed to %s",
            SMIF_FSM_STATES_NAMES[SMIF_GET_FSM_STATE(p_smif_gb_context)]);

        /* Stop shutdown_resp timer if exist */
        if (SON_PNULL != p_smif_gb_context->son_shutdown_timer)
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_DETAILED,
                "Shutdown response timer stopped");
            son_stop_timer(p_smif_gb_context->son_shutdown_timer);
            p_smif_gb_context->son_shutdown_timer = SON_PNULL;

            SON_MEMSET(&shutdown_resp, 0, sizeof(shutdown_resp));

            /* Send Response to OAM - Success/Failure - In Graceful case only */
            shutdown_resp.result = SON_SUCCESS;

            /*If shutdown resp is received from all modules then send response
             * to OAM */
            son_oam_send_son_oam_shutdown_resp(
                &shutdown_resp,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                SON_NULL,
                SON_NULL);

        }
        else
        {
            SON_LOG( g_mif_log_on_off, p_son_mif_facility_name, SON_BRIEF,
                "No need to send SHUTDOWN_RESP to OAM ");
        }

        /* Close all the sockets and free the listener structure memory
         * before terminating */
        qvClose(&son_shell, p_listener);

        /* Exit Here */
        SON_UT_TRACE_EXIT();
        son_exit(0);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_DETAILED,
            "Shutdown response not received from all internal modules.");
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_check_for_shutdown_timer_stop */

/*****************************************************************************
 * Function Name  : son_mif_sonnmm_init_config_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for
 *                  SONNMM_MIF_INIT_CONFIG_RESP msg
 ****************************************************************************/
static void
son_mif_sonnmm_init_config_resp_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();

    son_mif_init_config_res(p_msg, p_smif_gb_context,
        SMIF_SONNMM_INIT_CONFIG_RESP_RECD);

    /* Check if init config responses have been received from all SON internal
     * modules then change the FSM state*/
    son_mif_check_for_smif_active_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonapps_init_config_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for
 * SONAPPS_SMIF_INIT_CONFIG_RES msg
 ****************************************************************************/
static void
son_mif_sonapps_init_config_resp_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();

    son_mif_init_config_res(p_msg, p_smif_gb_context, SMIF_SONAPPS_INIT_CONFIG_RESP_RECD);

    /* Check if init_config_res messages are received from all SON internal
     * modules(NMM, APPS, ANR, ES, X2) */
    son_mif_check_for_smif_active_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonanr_init_config_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for
 *                  SONANR_SMIF_INIT_CONFIG_RESP msg
 ****************************************************************************/
static void
son_mif_sonanr_init_config_resp_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();

    son_mif_init_config_res(p_msg, p_smif_gb_context,
        SMIF_SONANR_INIT_CONFIG_RESP_RECD);

    /* Check if init_config_res messages are received from all SON internal
     * modules(NMM, APPS, ANR, ES, X2) */
    son_mif_check_for_smif_active_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sones_init_config_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for
 *                  SONES_SMIF_INIT_CONFIG_RESP msg
 ****************************************************************************/
static void
son_mif_sones_init_config_resp_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();

    son_mif_init_config_res(p_msg, p_smif_gb_context,
        SMIF_SONES_INIT_CONFIG_RESP_RECD);

    /* Check if init_config_res messages are received from all SON internal
     * modules(NMM, APPS, ANR, ES, X2) */
    son_mif_check_for_smif_active_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonx2_init_config_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for
 *                  SONX2_SMIF_INIT_CONFIG_RESP msg
 ****************************************************************************/
static void
son_mif_sonx2_init_config_resp_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();

    son_mif_init_config_res(p_msg, p_smif_gb_context,
        SMIF_SONX2_INIT_CONFIG_RESP_RECD);

    /* Check if init_config_res messages are received from all SON internal
     * modules(NMM, APPS, ANR, ES, X2) */
    son_mif_check_for_smif_active_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

#ifdef SON_MRO_FEATURE

/*****************************************************************************
 * Function Name  : son_mif_sonmro_init_config_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for
 *                  SONMRO_SMIF_INIT_CONFIG_RESP msg
 ****************************************************************************/
static void
son_mif_sonmro_init_config_resp_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();

    son_mif_init_config_res(p_msg, p_smif_gb_context,
        SMIF_SONMRO_INIT_CONFIG_RESP_RECD);

    /* Check if init_config_res messages are received from all SON internal
     * modules(NMM, APPS, ANR, ES, X2, MRO, RACH OPT) */
    son_mif_check_for_smif_active_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

#endif /* ifdef SON_MRO_FEATURE */

/*****************************************************************************
 * Function Name  : son_mif_sonmlb_init_config_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for
 *                  SONMLB_SMIF_INIT_CONFIG_RESP msg
 ****************************************************************************/
static void
son_mif_sonmlb_init_config_resp_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();

    son_mif_init_config_res(p_msg, p_smif_gb_context,
        SMIF_SONMLB_INIT_CONFIG_RESP_RECD);

    /* Check if init_config_res messages are received from all SON internal
     * modules(NMM, APPS, ANR, ES, X2, MRO , MLB, RACH OPT) */
    son_mif_check_for_smif_active_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/* RACH OPT CHANGES : STARTS */
/*****************************************************************************
 * Function Name  : son_mif_sonrach_opt_init_config_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SMIF_INIT_CONFIG_RES
 *                  message
 ****************************************************************************/
static void
son_mif_sonrach_opt_init_config_resp_handler
(
    son_mif_gb_context_t   *p_smif_gb_context,
    void                    *p_msg
)
{
    SON_UT_TRACE_ENTER();

    son_mif_init_config_res(p_msg, p_smif_gb_context,
            SMIF_SONRACH_OPT_INIT_CONFIG_RESP_RECD);

    /* Check if init_config_res messages are received from all SON internal
     * modules(NMM, APPS, ANR, ES, X2, MRO, RACH OPT) */
    son_mif_check_for_smif_active_state(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}
/* RACH OPT CHANGES : ENDS */

/*****************************************************************************
 * Function Name  : son_mif_sonnmm_shutdown_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONNMM_MIF_SHUTDOWN_RESP
 *                  msg
 ****************************************************************************/
static void
son_mif_sonnmm_shutdown_resp_handler
(
    son_mif_gb_context_t  *p_smif_gb_context,
    void                   *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of shutdown from NMM in SMIF global
     * context */
    p_smif_gb_context->shutdown_status |= SMIF_SONNMM_SHUTDOWN_RECD;

    /* Check if shutdown response messages have been received from
     * all SON internal modules(NMM, APPS, ANR, ES, X2) and stop the shutdown
     * timer*/
    son_mif_check_for_shutdown_timer_stop(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonapps_shutdown_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONAPPS_SMIF_SHUTDOWN_RESP
 *                  msg
 ****************************************************************************/
static void
son_mif_sonapps_shutdown_resp_handler
(
    son_mif_gb_context_t  *p_smif_gb_context,
    void                   *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of shutdown from APPS in SMIF global
     * context */
    p_smif_gb_context->shutdown_status |= SMIF_SONAPPS_SHUTDOWN_RECD;

    /* Check if shutdown response messages have been received from
     * all SON internal modules(NMM, APPS, ANR, ES, X2) and stop the shutdown
     * timer*/
    son_mif_check_for_shutdown_timer_stop(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonanr_shutdown_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONANR_MIF_SHUTDOWN_RESP
 *                  msg
 ****************************************************************************/
static void
son_mif_sonanr_shutdown_resp_handler
(
    son_mif_gb_context_t  *p_smif_gb_context,
    void                   *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of shutdown from ANR in SMIF global
     * context */
    p_smif_gb_context->shutdown_status |= SMIF_SONANR_SHUTDOWN_RECD;

    /* Check if shutdown response messages have been received from
     * all SON internal modules(NMM, APPS, ANR, ES, X2) and stop the shutdown
     * timer*/
    son_mif_check_for_shutdown_timer_stop(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sones_shutdown_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONES_MIF_SHUTDOWN_RESP msg
 ****************************************************************************/
static void
son_mif_sones_shutdown_resp_handler
(
    son_mif_gb_context_t  *p_smif_gb_context,
    void                   *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of shutdown from ES in SMIF global context*/
    p_smif_gb_context->shutdown_status |= SMIF_SONES_SHUTDOWN_RECD;

    /* Check if shutdown response messages have been received from
     * all SON internal modules(NMM, APPS, ANR, ES, X2) and stop the shutdown
     * timer*/
    son_mif_check_for_shutdown_timer_stop(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonx2_shutdown_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONX2_MIF_SHUTDOWN_RESP msg
 ****************************************************************************/
static void
son_mif_sonx2_shutdown_resp_handler
(
    son_mif_gb_context_t  *p_smif_gb_context,
    void                   *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of shutdown from X2 in SMIF global context*/
    p_smif_gb_context->shutdown_status |= SMIF_SONX2_SHUTDOWN_RECD;

    /* Check if shutdown response messages have been received from
     * all SON internal modules(NMM, APPS, ANR, ES, X2) and stop the shutdown
     * timer*/
    son_mif_check_for_shutdown_timer_stop(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

#ifdef SON_MRO_FEATURE

/*****************************************************************************
 * Function Name  : son_mif_sonmro_shutdown_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONMRO_MIF_SHUTDOWN_RESP
 * msg
 ****************************************************************************/
static void
son_mif_sonmro_shutdown_resp_handler
(
    son_mif_gb_context_t  *p_smif_gb_context,
    void                   *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of shutdown from X2 in SMIF global context*/
    p_smif_gb_context->shutdown_status |= SMIF_SONMRO_SHUTDOWN_RECD;

    /* Check if shutdown response messages have been received from
     * all SON internal modules(NMM, APPS, ANR, ES, X2, MRO, RACH OPT) and 
     stop the shutdown timer */
    son_mif_check_for_shutdown_timer_stop(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

#endif /* ifdef SON_MRO_FEATURE */

/*****************************************************************************
 * Function Name  : son_mif_sonmlb_shutdown_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SONMLB_MIF_SHUTDOWN_RESP
 * msg
 ****************************************************************************/
static void
son_mif_sonmlb_shutdown_resp_handler
(
    son_mif_gb_context_t  *p_smif_gb_context,
    void                   *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of shutdown from X2 in SMIF global context*/
    p_smif_gb_context->shutdown_status |= SMIF_SONMLB_SHUTDOWN_RECD;

    /* Check if shutdown response messages have been received from
     * all SON internal modules(NMM, APPS, ANR, ES, X2, MRO ,MLB, RACH OPT) 
     and stop the shutdown timer */
    son_mif_check_for_shutdown_timer_stop(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/* Rel 3.0 Cell delete support changes Start */
/*****************************************************************************
  * Function Name  : son_mif_chk_and_send_cell_delete_resp
  * Inputs         : p_smif_gb_context - SMIF global context pointer
  * Outputs        : None
  * Returns        : None
  * Description    : This function constructs and sends SON_OAM_CELL_DELETE_RESP
  ****************************************************************************/
static void son_mif_chk_and_send_cell_delete_resp
(
 son_mif_gb_context_t*  p_smif_gb_context
 )
{
    son_oam_cell_delete_resp_t   cell_del_res;

    SON_UT_TRACE_ENTER();

    /* Check if cell delete resp is received from all the modules */
    if ((p_smif_gb_context->cell_delete_resp_status & 
                SMIF_SONANR_CELL_DELETE_RESP_RECD)
            && (p_smif_gb_context->cell_delete_resp_status & 
                SMIF_SONES_CELL_DELETE_RESP_RECD)
            && (p_smif_gb_context->cell_delete_resp_status & 
                SMIF_SONMLB_CELL_DELETE_RESP_RECD)
#ifdef SON_MRO_FEATURE
            && (p_smif_gb_context->cell_delete_resp_status & 
                SMIF_SONMRO_CELL_DELETE_RESP_RECD)
#endif
            && (p_smif_gb_context->cell_delete_resp_status & 
                SMIF_SONRACH_OPT_CELL_DELETE_RESP_RECD)
       )
    {
	/* SPR 19735 Fix +- */

        /* Populate the resp structure */
        SON_MEMCPY(&cell_del_res,
                &p_smif_gb_context->cell_delete_res.cell_delete_resp,
                sizeof(son_oam_cell_delete_resp_t));

        /* Reset the cell delete resp status flag and structure from SMIF 
           global  context */
        p_smif_gb_context->cell_delete_req_in_process = SON_NULL;
        p_smif_gb_context->cell_delete_resp_status = 0;

	/* SPR 19735 Fix +- */
        if (SON_SUCCESS != son_oam_send_son_oam_cell_delete_resp(
                &cell_del_res,
                SON_MIF_MODULE_ID,
                SON_OAM_MODULE_ID,
                p_smif_gb_context->cell_delete_res.transaction_id,
                SON_NULL))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "Failed to send SON_OAM_CELL_DELETE_RESP to OAM");
        }
	/* SPR 19735 Fix Start */
        SON_MEMSET(&p_smif_gb_context->cell_delete_res, 0,
                sizeof(son_cell_delete_resp_t));
	/* SPR 19735 Fix End */
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonanr_cell_delete_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONANR_CELL_DELETE_RES msg
 ****************************************************************************/
static void son_mif_sonanr_cell_delete_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    son_oam_cell_delete_resp_t  *p_in_message   = SON_PNULL;
    son_u8                  idx             = 0;

    SON_UT_TRACE_ENTER();

    p_in_message = (son_oam_cell_delete_resp_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    /* Set the flag for receipt of cell delete resp from ANR in SMIF global 
       context */
    p_smif_gb_context->cell_delete_resp_status |= 
        SMIF_SONANR_CELL_DELETE_RESP_RECD;

    if (!(SMIF_SONANR_CELL_DELETE_RESP_RECD == 
                p_smif_gb_context->cell_delete_resp_status))
    {
        /* Process the message and save it in SMIF global context */
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
                cell_delete_resp.cell_delete_status_list_size; idx ++)
        {
            if (SON_PARTIAL_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx].result = SON_PARTIAL_SUCCESS;
            }
            else if (SON_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                if (SON_PARTIAL_SUCCESS != p_smif_gb_context->cell_delete_res.
                        cell_delete_resp.cell_delete_status_list[idx].result)
                {
                    p_smif_gb_context->cell_delete_res.cell_delete_resp.
                        cell_delete_status_list[idx].result = SON_SUCCESS;
                }
            }
        }
    }
    else
    {
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
        cell_delete_resp.cell_delete_status_list_size; idx ++)
        {
            SON_MEMCPY(&p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx],
                    &p_in_message->cell_delete_status_list[idx],
                    sizeof(son_cell_delete_status_t));
        }
    }

    son_mif_chk_and_send_cell_delete_resp(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sones_cell_delete_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONES_CELL_DELETE_RES msg
 ****************************************************************************/
static void son_mif_sones_cell_delete_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    son_oam_cell_delete_resp_t  *p_in_message   = SON_PNULL;
    son_u8                  idx             = 0;

    SON_UT_TRACE_ENTER();

    p_in_message = (son_oam_cell_delete_resp_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    /* Set the flag for receipt of cell delete resp from ES in SMIF global 
       context */
    p_smif_gb_context->cell_delete_resp_status |= 
        SMIF_SONES_CELL_DELETE_RESP_RECD;

    if (!(SMIF_SONES_CELL_DELETE_RESP_RECD == p_smif_gb_context->
                cell_delete_resp_status))
    {
        /* Process the message and save it in SMIF global context */
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
                cell_delete_resp.cell_delete_status_list_size; idx ++)
        {
            if (SON_PARTIAL_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx].result = SON_PARTIAL_SUCCESS;
            }
            else if (SON_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                if (SON_PARTIAL_SUCCESS != p_smif_gb_context->cell_delete_res.
                        cell_delete_resp.cell_delete_status_list[idx].result)
                {
                    p_smif_gb_context->cell_delete_res.cell_delete_resp.
                        cell_delete_status_list[idx].result = SON_SUCCESS;
                }
            }
        }
    }
    else
    {
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
                cell_delete_resp.cell_delete_status_list_size; idx ++)
        {
            SON_MEMCPY(&p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx],
                    &p_in_message->cell_delete_status_list[idx],
                    sizeof(son_cell_delete_status_t));
        }
    }

    son_mif_chk_and_send_cell_delete_resp(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}


/* SPR-11639 Fix Starts */
#ifdef SON_MRO_FEATURE
/* SPR-11639 Fix Ends */
/*****************************************************************************
 * Function Name  : son_mif_sonmro_cell_delete_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONMRO_CELL_DELETE_RES msg
 ****************************************************************************/
static void son_mif_sonmro_cell_delete_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    son_oam_cell_delete_resp_t  *p_in_message   = SON_PNULL;
    son_u8                  idx             = 0;

    SON_UT_TRACE_ENTER();

    p_in_message = (son_oam_cell_delete_resp_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    /* Set the flag for receipt of cell delete resp from MRO in SMIF global 
       context */
    p_smif_gb_context->cell_delete_resp_status |= 
        SMIF_SONMRO_CELL_DELETE_RESP_RECD;

    if (!(SMIF_SONMRO_CELL_DELETE_RESP_RECD == 
                p_smif_gb_context->cell_delete_resp_status))
    {
        /* Process the message and save it in SMIF global context */
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
                cell_delete_resp.cell_delete_status_list_size; idx ++)
        {
            if (SON_PARTIAL_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx].result = SON_PARTIAL_SUCCESS;
            }
            else if (SON_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                if (SON_PARTIAL_SUCCESS != p_smif_gb_context->cell_delete_res.
                        cell_delete_resp.cell_delete_status_list[idx].result)
                {
                    p_smif_gb_context->cell_delete_res.cell_delete_resp.
                        cell_delete_status_list[idx].result = SON_SUCCESS;
                }
            }
        }
    }
    else
    {
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
                cell_delete_resp.cell_delete_status_list_size; idx ++)
        {
            SON_MEMCPY(&p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx],
                    &p_in_message->cell_delete_status_list[idx],
                    sizeof(son_cell_delete_status_t));
        }
    }

    son_mif_chk_and_send_cell_delete_resp(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}
/* SPR-11639 Fix Starts */
#endif /* ifdef SON_MRO_FEATURE */
/* SPR-11639 Fix Ends */

/*****************************************************************************
 * Function Name  : son_mif_sonmlb_cell_delete_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONMLB_CELL_DELETE_RES msg
 ****************************************************************************/
static void son_mif_sonmlb_cell_delete_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    son_oam_cell_delete_resp_t  *p_in_message   = SON_PNULL;
    son_u8                  idx             = 0;

    SON_UT_TRACE_ENTER();

    p_in_message = (son_oam_cell_delete_resp_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    /* Set the flag for receipt of cell delete resp from MLB in SMIF global 
       context */
    p_smif_gb_context->cell_delete_resp_status |= 
        SMIF_SONMLB_CELL_DELETE_RESP_RECD;

    if (!(SMIF_SONMLB_CELL_DELETE_RESP_RECD == p_smif_gb_context->
                cell_delete_resp_status))
    {
        /* Process the message and save it in SMIF global context */
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
                cell_delete_resp.cell_delete_status_list_size; idx ++)
        {
            if (SON_PARTIAL_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx].result = SON_PARTIAL_SUCCESS;
            }
            else if (SON_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                if (SON_PARTIAL_SUCCESS != p_smif_gb_context->cell_delete_res.
                        cell_delete_resp.cell_delete_status_list[idx].result)
                {
                    p_smif_gb_context->cell_delete_res.cell_delete_resp.
                        cell_delete_status_list[idx].result = SON_SUCCESS;
                }
            }
        }
    }
    else
    {
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
                cell_delete_resp.cell_delete_status_list_size; idx ++)
        {
            SON_MEMCPY(&p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx],
                    &p_in_message->cell_delete_status_list[idx],
                    sizeof(son_cell_delete_status_t));
        }
    }

    son_mif_chk_and_send_cell_delete_resp(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonrach_opt_cell_delete_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONRACH_OPT_CELL_DELETE_RES msg
 ****************************************************************************/
static void son_mif_sonrach_opt_cell_delete_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    son_oam_cell_delete_resp_t  *p_in_message   = SON_PNULL;
    son_u8                  idx             = 0;

    SON_UT_TRACE_ENTER();

    p_in_message = (son_oam_cell_delete_resp_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    /* Set the flag for receipt of cell delete resp from RACH-OPT in SMIF global 
       context */
    p_smif_gb_context->cell_delete_resp_status |= 
        SMIF_SONRACH_OPT_CELL_DELETE_RESP_RECD;

    if (!(SMIF_SONRACH_OPT_CELL_DELETE_RESP_RECD == 
                p_smif_gb_context->cell_delete_resp_status))
    {
        /* Process the message and save it in SMIF global context */
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
                cell_delete_resp.cell_delete_status_list_size; idx++)
        {
            if (SON_PARTIAL_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx].result = SON_PARTIAL_SUCCESS;
            }
            else if (SON_SUCCESS == 
                    p_in_message->cell_delete_status_list[idx].result)
            {
                if (SON_PARTIAL_SUCCESS != p_smif_gb_context->cell_delete_res.
                        cell_delete_resp.cell_delete_status_list[idx].result)
                {
                    p_smif_gb_context->cell_delete_res.cell_delete_resp.
                        cell_delete_status_list[idx].result = SON_SUCCESS;
                }
            }
        }
    }
    else
    {
        for (idx = 0; idx < p_smif_gb_context->cell_delete_res.
                cell_delete_resp.cell_delete_status_list_size; idx++)
        {
            SON_MEMCPY(&p_smif_gb_context->cell_delete_res.cell_delete_resp.
                    cell_delete_status_list[idx],
                    &p_in_message->cell_delete_status_list[idx],
                    sizeof(son_cell_delete_status_t));
        }
    }

    son_mif_chk_and_send_cell_delete_resp(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}

/* Rel 3.0 Cell delete support changes Stop */

/* SPR-13251 Fix Starts */
/*****************************************************************************
 * Function Name  : son_mif_sonnmm_get_log_level_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONNMM_GET_LOG_LEVEL_RES msg
 ****************************************************************************/
static void son_mif_sonnmm_get_log_level_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    smif_get_log_level_res_t *p_in_message   = SON_PNULL;
    son_u8 index = 0;
    SON_UT_TRACE_ENTER();

    p_in_message = (smif_get_log_level_res_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    if (1 == p_smif_gb_context->expected_get_log_level_resp)
    {
        son_mif_oam_send_get_log_level_resp(
                p_in_message->transaction_id,
                SON_SUCCESS, SON_NO_ERROR, SON_NMM_MODULE_ID, 
                p_in_message->log_level);
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;        
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Store the result from NMM module in the SMIF global context */
    index = SON_NMM_MODULE_ID - SON_MIN_MODULE_ID;

    if (SON_LOG_LEVEL_NONE == p_in_message->log_level)
    {
        p_smif_gb_context->get_log_level_resp.result = SON_PARTIAL_SUCCESS;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_ERR_LOGGING_DISABLED;
    }
    else
    {
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_NO_ERROR;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            bitmask |= SON_MODULE_LOG_LEVEL_PRESENT;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            log_level = (son_oam_log_level_et)p_in_message->log_level;
        if (SON_PARTIAL_SUCCESS != p_smif_gb_context->get_log_level_resp.result)
        {
            p_smif_gb_context->get_log_level_resp.result = SON_SUCCESS;
        }
    }
    p_smif_gb_context->get_log_level_resp.module_log_level[index].
        module_id = SON_NMM_MODULE_ID;
    p_smif_gb_context->get_log_level_resp.num_of_modules++;

    /* Check if get log level response is received from all the modules. 
       If yes then send set log level response to OAM */
    if (p_smif_gb_context->expected_get_log_level_resp == 
            p_smif_gb_context->get_log_level_resp.num_of_modules)
    {
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        if (SON_SUCCESS != son_oam_send_son_oam_get_log_level_resp(
                    &p_smif_gb_context->get_log_level_resp,
                    SON_MODULE_ID,
                    SON_OAM_MODULE_ID,
                    p_in_message->transaction_id,
                    SON_NULL))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "Failed to send SON_OAM_GET_LOG_LEVEL_RESP to OAM");
        }
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonapps_get_log_level_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONAPPS_GET_LOG_LEVEL_RES msg
 ****************************************************************************/
static void son_mif_sonapps_get_log_level_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    smif_get_log_level_res_t *p_in_message   = SON_PNULL;
    son_u8 index = 0;
    SON_UT_TRACE_ENTER();

    p_in_message = (smif_get_log_level_res_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    if (1 == p_smif_gb_context->expected_get_log_level_resp)
    {
        son_mif_oam_send_get_log_level_resp(
                p_in_message->transaction_id,
                SON_SUCCESS, SON_NO_ERROR, SON_APPS_MODULE_ID, 
                p_in_message->log_level);
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;        
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Store the result from RACH module in the SMIF global context */
    index = SON_APPS_MODULE_ID - SON_MIN_MODULE_ID;

    if (SON_LOG_LEVEL_NONE == p_in_message->log_level)
    {
        p_smif_gb_context->get_log_level_resp.result = SON_PARTIAL_SUCCESS;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_ERR_LOGGING_DISABLED;
    }
    else
    {
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_NO_ERROR;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            bitmask |= SON_MODULE_LOG_LEVEL_PRESENT;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            log_level = (son_oam_log_level_et)p_in_message->log_level;
        if (SON_PARTIAL_SUCCESS != p_smif_gb_context->get_log_level_resp.result)
        {
            p_smif_gb_context->get_log_level_resp.result = SON_SUCCESS;
        }
    }
    p_smif_gb_context->get_log_level_resp.module_log_level[index].
        module_id = SON_APPS_MODULE_ID;
    p_smif_gb_context->get_log_level_resp.num_of_modules++;

    /* Check if get log level response is received from all the modules. 
       If yes then send set log level response to OAM */
    if (p_smif_gb_context->expected_get_log_level_resp == 
            p_smif_gb_context->get_log_level_resp.num_of_modules)
    {
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        if (SON_SUCCESS != son_oam_send_son_oam_get_log_level_resp(
                    &p_smif_gb_context->get_log_level_resp,
                    SON_MODULE_ID,
                    SON_OAM_MODULE_ID,
                    p_in_message->transaction_id,
                    SON_NULL))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "Failed to send SON_OAM_GET_LOG_LEVEL_RESP to OAM");
        }
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonanr_get_log_level_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONANR_GET_LOG_LEVEL_RES msg
 ****************************************************************************/
static void son_mif_sonanr_get_log_level_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    smif_get_log_level_res_t *p_in_message   = SON_PNULL;
    son_u8 index = 0;
    SON_UT_TRACE_ENTER();

    p_in_message = (smif_get_log_level_res_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    if (1 == p_smif_gb_context->expected_get_log_level_resp)
    {
        son_mif_oam_send_get_log_level_resp(
                p_in_message->transaction_id,
                SON_SUCCESS, SON_NO_ERROR, SON_ANR_MODULE_ID, 
                p_in_message->log_level);
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;        
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Store the result from RACH module in the SMIF global context */
    index = SON_ANR_MODULE_ID - SON_MIN_MODULE_ID;

    if (SON_LOG_LEVEL_NONE == p_in_message->log_level)
    {
        p_smif_gb_context->get_log_level_resp.result = SON_PARTIAL_SUCCESS;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_ERR_LOGGING_DISABLED;
    }
    else
    {
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_NO_ERROR;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            bitmask |= SON_MODULE_LOG_LEVEL_PRESENT;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            log_level = (son_oam_log_level_et)p_in_message->log_level;
        if (SON_PARTIAL_SUCCESS != p_smif_gb_context->get_log_level_resp.result)
        {
            p_smif_gb_context->get_log_level_resp.result = SON_SUCCESS;
        }
    }
    p_smif_gb_context->get_log_level_resp.module_log_level[index].
        module_id = SON_ANR_MODULE_ID;
    p_smif_gb_context->get_log_level_resp.num_of_modules++;

    /* Check if get log level response is received from all the modules. 
       If yes then send set log level response to OAM */
    if (p_smif_gb_context->expected_get_log_level_resp == 
            p_smif_gb_context->get_log_level_resp.num_of_modules)
    {
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        if (SON_SUCCESS != son_oam_send_son_oam_get_log_level_resp(
                    &p_smif_gb_context->get_log_level_resp,
                    SON_MODULE_ID,
                    SON_OAM_MODULE_ID,
                    p_in_message->transaction_id,
                    SON_NULL))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "Failed to send SON_OAM_GET_LOG_LEVEL_RESP to OAM");
        }
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sones_get_log_level_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONES_GET_LOG_LEVEL_RES msg
 ****************************************************************************/
static void son_mif_sones_get_log_level_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    smif_get_log_level_res_t *p_in_message   = SON_PNULL;
    son_u8 index = 0;
    SON_UT_TRACE_ENTER();

    p_in_message = (smif_get_log_level_res_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    if (1 == p_smif_gb_context->expected_get_log_level_resp)
    {
        son_mif_oam_send_get_log_level_resp(
                p_in_message->transaction_id,
                SON_SUCCESS, SON_NO_ERROR, SON_ES_MODULE_ID, 
                p_in_message->log_level);
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;        
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Store the result from RACH module in the SMIF global context */
    index = SON_ES_MODULE_ID - SON_MIN_MODULE_ID;

    if (SON_LOG_LEVEL_NONE == p_in_message->log_level)
    {
        p_smif_gb_context->get_log_level_resp.result = SON_PARTIAL_SUCCESS;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_ERR_LOGGING_DISABLED;
    }
    else
    {
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_NO_ERROR;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            bitmask |= SON_MODULE_LOG_LEVEL_PRESENT;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            log_level = (son_oam_log_level_et)p_in_message->log_level;
        if (SON_PARTIAL_SUCCESS != p_smif_gb_context->get_log_level_resp.result)
        {
            p_smif_gb_context->get_log_level_resp.result = SON_SUCCESS;
        }
    }
    p_smif_gb_context->get_log_level_resp.module_log_level[index].
        module_id = SON_ES_MODULE_ID;
    p_smif_gb_context->get_log_level_resp.num_of_modules++;

    /* Check if get log level response is received from all the modules. 
       If yes then send set log level response to OAM */
    if (p_smif_gb_context->expected_get_log_level_resp == 
            p_smif_gb_context->get_log_level_resp.num_of_modules)
    {
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        if (SON_SUCCESS != son_oam_send_son_oam_get_log_level_resp(
                    &p_smif_gb_context->get_log_level_resp,
                    SON_MODULE_ID,
                    SON_OAM_MODULE_ID,
                    p_in_message->transaction_id,
                    SON_NULL))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "Failed to send SON_OAM_GET_LOG_LEVEL_RESP to OAM");
        }
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonmro_get_log_level_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONMRO_GET_LOG_LEVEL_RES msg
 ****************************************************************************/
static void son_mif_sonmro_get_log_level_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    smif_get_log_level_res_t *p_in_message   = SON_PNULL;
    son_u8 index = 0;
    SON_UT_TRACE_ENTER();

    p_in_message = (smif_get_log_level_res_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    if (1 == p_smif_gb_context->expected_get_log_level_resp)
    {
        son_mif_oam_send_get_log_level_resp(
                p_in_message->transaction_id,
                SON_SUCCESS, SON_NO_ERROR, SON_MRO_MODULE_ID, 
                p_in_message->log_level);
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;        
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Store the result from RACH module in the SMIF global context */
    index = SON_MRO_MODULE_ID - SON_MIN_MODULE_ID;

    if (SON_LOG_LEVEL_NONE == p_in_message->log_level)
    {
        p_smif_gb_context->get_log_level_resp.result = SON_PARTIAL_SUCCESS;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_ERR_LOGGING_DISABLED;
    }
    else
    {
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_NO_ERROR;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            bitmask |= SON_MODULE_LOG_LEVEL_PRESENT;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            log_level = (son_oam_log_level_et)p_in_message->log_level;
        if (SON_PARTIAL_SUCCESS != p_smif_gb_context->get_log_level_resp.result)
        {
            p_smif_gb_context->get_log_level_resp.result = SON_SUCCESS;
        }
    }
    p_smif_gb_context->get_log_level_resp.module_log_level[index].
        module_id = SON_MRO_MODULE_ID;
    p_smif_gb_context->get_log_level_resp.num_of_modules++;

    /* Check if get log level response is received from all the modules. 
       If yes then send set log level response to OAM */
    if (p_smif_gb_context->expected_get_log_level_resp == 
            p_smif_gb_context->get_log_level_resp.num_of_modules)
    {
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        if (SON_SUCCESS != son_oam_send_son_oam_get_log_level_resp(
                    &p_smif_gb_context->get_log_level_resp,
                    SON_MODULE_ID,
                    SON_OAM_MODULE_ID,
                    p_in_message->transaction_id,
                    SON_NULL))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "Failed to send SON_OAM_GET_LOG_LEVEL_RESP to OAM");
        }
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonmlb_get_log_level_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONMLB_GET_LOG_LEVEL_RES msg
 ****************************************************************************/
static void son_mif_sonmlb_get_log_level_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    smif_get_log_level_res_t *p_in_message   = SON_PNULL;
    son_u8 index = 0;
    SON_UT_TRACE_ENTER();

    p_in_message = (smif_get_log_level_res_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    if (1 == p_smif_gb_context->expected_get_log_level_resp)
    {
        son_mif_oam_send_get_log_level_resp(
                p_in_message->transaction_id,
                SON_SUCCESS, SON_NO_ERROR, SON_MLB_MODULE_ID, 
                p_in_message->log_level);
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;        
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Store the result from RACH module in the SMIF global context */
    index = SON_MLB_MODULE_ID - SON_MIN_MODULE_ID;

    if (SON_LOG_LEVEL_NONE == p_in_message->log_level)
    {
        p_smif_gb_context->get_log_level_resp.result = SON_PARTIAL_SUCCESS;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_ERR_LOGGING_DISABLED;
    }
    else
    {
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_NO_ERROR;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            bitmask |= SON_MODULE_LOG_LEVEL_PRESENT;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            log_level = (son_oam_log_level_et)p_in_message->log_level;
        if (SON_PARTIAL_SUCCESS != p_smif_gb_context->get_log_level_resp.result)
        {
            p_smif_gb_context->get_log_level_resp.result = SON_SUCCESS;
        }
    }
    p_smif_gb_context->get_log_level_resp.module_log_level[index].
        module_id = SON_MLB_MODULE_ID;
    p_smif_gb_context->get_log_level_resp.num_of_modules++;

    /* Check if get log level response is received from all the modules. 
       If yes then send set log level response to OAM */
    if (p_smif_gb_context->expected_get_log_level_resp == 
            p_smif_gb_context->get_log_level_resp.num_of_modules)
    {
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        if (SON_SUCCESS != son_oam_send_son_oam_get_log_level_resp(
                    &p_smif_gb_context->get_log_level_resp,
                    SON_MODULE_ID,
                    SON_OAM_MODULE_ID,
                    p_in_message->transaction_id,
                    SON_NULL))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "Failed to send SON_OAM_GET_LOG_LEVEL_RESP to OAM");
        }
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_mif_sonrach_opt_get_log_level_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONRACH_OPT_GET_LOG_LEVEL_RES msg
 ****************************************************************************/
static void son_mif_sonrach_opt_get_log_level_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    smif_get_log_level_res_t *p_in_message   = SON_PNULL;
    son_u8 index = 0;
    SON_UT_TRACE_ENTER();

    p_in_message = (smif_get_log_level_res_t *)((U8*)p_msg +
            SON_API_HEADER_SIZE);

    if (1 == p_smif_gb_context->expected_get_log_level_resp)
    {
        son_mif_oam_send_get_log_level_resp(
                p_in_message->transaction_id,
                SON_SUCCESS, SON_NO_ERROR, SON_RACH_OPT_MODULE_ID, 
                p_in_message->log_level);
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;        
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
        SON_UT_TRACE_EXIT();
        return;
    }

    /* Store the result from RACH module in the SMIF global context */
    index = SON_RACH_OPT_MODULE_ID - SON_MIN_MODULE_ID;

    if (SON_LOG_LEVEL_NONE == p_in_message->log_level)
    {
        p_smif_gb_context->get_log_level_resp.result = SON_PARTIAL_SUCCESS;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_ERR_LOGGING_DISABLED;
    }
    else
    {
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            error_code = SON_NO_ERROR;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            bitmask |= SON_MODULE_LOG_LEVEL_PRESENT;
        p_smif_gb_context->get_log_level_resp.module_log_level[index].
            log_level = (son_oam_log_level_et)p_in_message->log_level;
        if (SON_PARTIAL_SUCCESS != p_smif_gb_context->get_log_level_resp.result)
        {
            p_smif_gb_context->get_log_level_resp.result = SON_SUCCESS;
        }
    }
    p_smif_gb_context->get_log_level_resp.module_log_level[index].
        module_id = SON_RACH_OPT_MODULE_ID;
    p_smif_gb_context->get_log_level_resp.num_of_modules++;

    /* Check if get log level response is received from all the modules. 
       If yes then send set log level response to OAM */
    if (p_smif_gb_context->expected_get_log_level_resp == 
            p_smif_gb_context->get_log_level_resp.num_of_modules)
    {
        p_smif_gb_context->expected_get_log_level_resp = SON_NULL;
        p_smif_gb_context->get_log_level_req_in_process = SON_NULL;
        if (SON_SUCCESS != son_oam_send_son_oam_get_log_level_resp(
                    &p_smif_gb_context->get_log_level_resp,
                    SON_MODULE_ID,
                    SON_OAM_MODULE_ID,
                    p_in_message->transaction_id,
                    SON_NULL))
        {
            SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
                    "Failed to send SON_OAM_GET_LOG_LEVEL_RESP to OAM");
        }
        SON_MEMSET(&p_smif_gb_context->get_log_level_resp, 0, 
                sizeof(son_oam_get_log_level_resp_t));
    }

    SON_UT_TRACE_EXIT();
}
/* SPR-13251 Fix Ends */

/* RACH OPT CHANGES : STARTS */
/*****************************************************************************
 * Function Name  : son_mif_sonrach_opt_shutdown_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for SMIF_SHUTDOWN_RES
 *                  message
 ****************************************************************************/
static void
son_mif_sonrach_opt_shutdown_resp_handler
(
    son_mif_gb_context_t  *p_smif_gb_context,
    void                   *p_msg
)
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    /* Set the flag for the receipt of shutdown from X2 in SMIF global context*/
    p_smif_gb_context->shutdown_status |= SMIF_SONRACH_OPT_SHUTDOWN_RECD;

    /* Check if shutdown response messages have been received from
     * all SON internal modules(NMM, APPS, ANR, ES, X2, MRO, RACH OPT) and 
     stop the shutdown timer */
    son_mif_check_for_shutdown_timer_stop(p_smif_gb_context);

    SON_UT_TRACE_EXIT();
}
/* RACH OPT CHANGES : ENDS */

/* SPR_19619 start */
/*****************************************************************************
 * Function Name  : son_mif_sones_peer_cell_activation_resp_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : son_void_et
 * Description    : This is the handler function for
 *                  SMIF_EVENT_SONRACH_OPT_GET_LOG_LEVEL_RES msg
 ****************************************************************************/
static void son_mif_sones_peer_cell_activation_resp_handler
(
 son_mif_gb_context_t   *p_smif_gb_context,
 void                   *p_msg
 )
{
    SON_UT_TRACE_ENTER();


    son_mif_peer_cell_activation_res(p_msg, p_smif_gb_context, SMIF_SONES_PEER_CELL_ACTIVATION_RESP_RECD);


    SON_UT_TRACE_EXIT();
}

/* SPR_19619 stop */

/*****************************************************************************
 * Function Name  : son_mif_invalid_event_handler
 * Inputs         : Pointer to the SMIF global context
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is the handler function for any invalid event for the
 *                    current state
 ****************************************************************************/
static void
son_mif_invalid_event_handler
(
    son_mif_gb_context_t  *p_smif_gb_context,
    void                   *p_msg
)
{
    smif_state_et state = SMIF_GET_FSM_STATE(p_smif_gb_context);

    SON_UT_TRACE_ENTER();

    /*SPR 17777 +-*/
    SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_WARNING,
        "Invalid Event, state %u", state);
    LTE_GCC_UNUSED_PARAM(p_msg);
    /*SPR 17777 +-*/

    SON_UT_TRACE_EXIT();
}

/*
 * SMIF FSM handlers mapping
 */
static smif_fsm_ret smif_fsm[SMIF_NUM_OF_STATES][SMIF_NUM_OF_EVENTS] =
{
    /* State SMIF_NULL */
    {
        son_mif_sonnmm_init_ind_handler,
        /* SMIF_EVENT_SONNMM_INIT_IND */
        son_mif_sonapps_init_ind_handler,
        /* SMIF_EVENT_SONAPPS_INIT_IND */
        son_mif_sonanr_init_ind_handler,
        /* SMIF_EVENT_SONANR_INIT_IND */
        son_mif_sones_init_ind_handler,
        /* SMIF_EVENT_SONES_INIT_IND */
        son_mif_sonx2_init_ind_handler,
        /* SMIF_EVENT_SONX2_INIT_IND */
#ifdef SON_MRO_FEATURE
        son_mif_sonmro_init_ind_handler,
        /* SMIF_EVENT_SONMRO_INIT_IND */
#endif
        son_mif_sonmlb_init_ind_handler,
        /* SMIF_EVENT_SONMLB_INIT_IND */
/* RACH OPT CHANGES : STARTS */
        son_mif_sonrach_opt_init_ind_handler,
        /* SMIF_EVENT_SONRACH_OPT_INIT_IND */
/* RACH OPT CHANGES : ENDS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_OAM_INIT_CNF */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONX2_INIT_CONFIG_RESP */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_INIT_CONFIG_RESP */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_INIT_CONFIG_RESP */
/* RACH OPT CHANGES : STARTS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_INIT_CONFIG_RESP */
/* RACH OPT CHANGES : ENDS */
        son_mif_oam_shutdown_req_handler,
        /* SMIF_EVENT_SHUTDOWN_REQ */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONX2_SHUTDOWN_RESP */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_SHUTDOWN_RESP */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_SHUTDOWN_RESP */
/* RACH OPT CHANGES : STARTS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_SHUTDOWN_RESP */
/* RACH OPT CHANGES : ENDS */
/* Rel 3.0 Cell delete support changes Start */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_CELL_DELETE_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_CELL_DELETE_RES */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_CELL_DELETE_RES */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_CELL_DELETE_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_CELL_DELETE_RES */
/* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_GET_LOG_LEVEL_RES */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_GET_LOG_LEVEL_RES */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_GET_LOG_LEVEL_RES */
        /* SPR-13251 Fix Ends */
        /* SPR_19619 start */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_PEER_CELL_ACTIVATION_RES */
        /* SPR_19619 stop */

    },

    /* State SMIF_STATE_INIT */
    {
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONX2_INIT_IND */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_INIT_IND */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_INIT_IND */
/* RACH OPT CHANGES : STARTS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_INIT_IND */
/* RACH OPT CHANGES : ENDS */
        son_mif_oam_init_cnf_handler,
        /* SMIF_EVENT_OAM_INIT_CNF */
        son_mif_sonnmm_init_config_resp_handler,
        /* SMIF_EVENT_SONNMM_INIT_CONFIG_RESP */
        son_mif_sonapps_init_config_resp_handler,
        /* SMIF_EVENT_SONAPPS_INIT_CONFIG_RESP */
        son_mif_sonanr_init_config_resp_handler,
        /* SMIF_EVENT_SONANR_INIT_CONFIG_RESP */
        son_mif_sones_init_config_resp_handler,
        /* SMIF_EVENT_SONES_INIT_CONFIG_RESP */
        son_mif_sonx2_init_config_resp_handler,
        /* SMIF_EVENT_SONX2_INIT_CONFIG_RESP */
#ifdef SON_MRO_FEATURE
        son_mif_sonmro_init_config_resp_handler,
        /* SMIF_EVENT_SONMRO_INIT_CONFIG_RESP */
#endif
        son_mif_sonmlb_init_config_resp_handler,
        /* SMIF_EVENT_SONMLB_INIT_CONFIG_RESP */
/* RACH OPT CHANGES : STARTS */
        son_mif_sonrach_opt_init_config_resp_handler,
        /* SMIF_EVENT_SONRACH_OPT_INIT_CONFIG_RESP */
/* RACH OPT CHANGES : ENDS */
        son_mif_oam_shutdown_req_handler,
        /* SMIF_EVENT_SHUTDOWN_REQ */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONX2_SHUTDOWN_RESP */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_SHUTDOWN_RESP */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_SHUTDOWN_RESP */
/* RACH OPT CHANGES : STARTS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_SHUTDOWN_RESP */
/* RACH OPT CHANGES : ENDS */
/* Rel 3.0 Cell delete support changes Start */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_CELL_DELETE_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_CELL_DELETE_RES */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
#endif
        /* SMIF_EVENT_SONMRO_CELL_DELETE_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_CELL_DELETE_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_CELL_DELETE_RES */
/* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_GET_LOG_LEVEL_RES */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_GET_LOG_LEVEL_RES */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_GET_LOG_LEVEL_RES */
        /* SPR-13251 Fix Ends */
        /* SPR_19619 start */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_PEER_CELL_ACTIVATION_RES */
        /* SPR_19619 stop */


    },

    /* State SMIF_STATE_W_FOR_SHUTDOWN_RESP */
    {
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONX2_INIT_IND */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_INIT_IND */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_INIT_IND */
/* RACH OPT CHANGES : STARTS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_INIT_IND */
/* RACH OPT CHANGES : ENDS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_OAM_INIT_CNF */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONX2_INIT_CONFIG_RESP */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_INIT_CONFIG_RESP */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_INIT_CONFIG_RESP */
/* RACH OPT CHANGES : STARTS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_INIT_CONFIG_RESP */
/* RACH OPT CHANGES : ENDS */
        son_mif_oam_shutdown_req_handler,
        /* SMIF_EVENT_SHUTDOWN_REQ */
        son_mif_sonnmm_shutdown_resp_handler,
        /* SMIF_EVENT_SONNMM_SHUTDOWN_RESP */
        son_mif_sonapps_shutdown_resp_handler,
        /* SMIF_EVENT_SONAPPS_SHUTDOWN_RESP */
        son_mif_sonanr_shutdown_resp_handler,
        /* SMIF_EVENT_SONANR_SHUTDOWN_RESP */
        son_mif_sones_shutdown_resp_handler,
        /* SMIF_EVENT_SONES_SHUTDOWN_RESP */
        son_mif_sonx2_shutdown_resp_handler,
        /* SMIF_EVENT_SONES_SHUTDOWN_RESP */
#ifdef SON_MRO_FEATURE
        son_mif_sonmro_shutdown_resp_handler,
        /* SMIF_EVENT_SONMRO_SHUTDOWN_RESP */
#endif
        son_mif_sonmlb_shutdown_resp_handler,
        /* SMIF_EVENT_SONMLB_SHUTDOWN_RESP */
/* RACH OPT CHANGES : STARTS */
        son_mif_sonrach_opt_shutdown_resp_handler,
        /* SMIF_EVENT_SONRACH_OPT_SHUTDOWN_RESP */
/* RACH OPT CHANGES : ENDS */
        son_mif_invalid_event_handler,
/* Rel 3.0 Cell delete support changes Start */
            /* SMIF_EVENT_SONANR_CELL_DELETE_RES */
        son_mif_invalid_event_handler,
            /* SMIF_EVENT_SONES_CELL_DELETE_RES */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
            /* SMIF_EVENT_SONMRO_CELL_DELETE_RES */
#endif
        son_mif_invalid_event_handler,
            /* SMIF_EVENT_SONMLB_CELL_DELETE_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_CELL_DELETE_RES */
/* Rel 3.0 Cell delete support changes Stop */
        /* SPR-13251 Fix Starts */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_GET_LOG_LEVEL_RES */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_GET_LOG_LEVEL_RES */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_GET_LOG_LEVEL_RES */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_GET_LOG_LEVEL_RES */
        /* SPR-13251 Fix Ends */
        /* SPR_19619 start */ 
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_PEER_CELL_ACTIVATION_RES */
        /* SPR_19619 stop */ 

    },

    /* State SMIF_STATE_ACTIVE */
    {
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_INIT_IND */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONX2_INIT_IND */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_INIT_IND */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_INIT_IND */
/* RACH OPT CHANGES : STARTS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_INIT_IND */
/* RACH OPT CHANGES : ENDS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_OAM_INIT_CNF */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_INIT_CONFIG_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONX2_INIT_CONFIG_RESP */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_INIT_CONFIG_RESP */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_INIT_CONFIG_RESP */
/* RACH OPT CHANGES : STARTS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_INIT_CONFIG_RESP */
/* RACH OPT CHANGES : ENDS */
        son_mif_oam_shutdown_req_handler,
        /* SMIF_EVENT_SHUTDOWN_REQ */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONNMM_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONAPPS_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONANR_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONES_SHUTDOWN_RESP */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONX2_SHUTDOWN_RESP */
#ifdef SON_MRO_FEATURE
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMRO_SHUTDOWN_RESP */
#endif
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONMLB_SHUTDOWN_RESP */
/* RACH OPT CHANGES : STARTS */
        son_mif_invalid_event_handler,
        /* SMIF_EVENT_SONRACH_OPT_SHUTDOWN_RESP */
/* RACH OPT CHANGES : ENDS */
/* Rel 3.0 Cell delete support changes Start */
        son_mif_sonanr_cell_delete_resp_handler,
            /* SMIF_EVENT_SONANR_CELL_DELETE_RES */
        son_mif_sones_cell_delete_resp_handler,
            /* SMIF_EVENT_SONES_CELL_DELETE_RES */
#ifdef SON_MRO_FEATURE
        son_mif_sonmro_cell_delete_resp_handler,
            /* SMIF_EVENT_SONMRO_CELL_DELETE_RES */
#endif
        son_mif_sonmlb_cell_delete_resp_handler,
            /* SMIF_EVENT_SONMLB_CELL_DELETE_RES */
        /* SPR-13251 Fix Starts */
        son_mif_sonrach_opt_cell_delete_resp_handler,
        /* SMIF_EVENT_SONRACH_OPT_CELL_DELETE_RES */
/* Rel 3.0 Cell delete support changes Stop */
        son_mif_sonnmm_get_log_level_resp_handler,
        /* SMIF_EVENT_SONNMM_GET_LOG_LEVEL_RES */
        son_mif_sonapps_get_log_level_resp_handler,
        /* SMIF_EVENT_SONAPPS_GET_LOG_LEVEL_RES */
        son_mif_sonanr_get_log_level_resp_handler,
        /* SMIF_EVENT_SONANR_GET_LOG_LEVEL_RES */
        son_mif_sones_get_log_level_resp_handler,
        /* SMIF_EVENT_SONES_GET_LOG_LEVEL_RES */
#ifdef SON_MRO_FEATURE
        son_mif_sonmro_get_log_level_resp_handler,
        /* SMIF_EVENT_SONMRO_GET_LOG_LEVEL_RES */
#endif
        son_mif_sonmlb_get_log_level_resp_handler,
        /* SMIF_EVENT_SONMLB_GET_LOG_LEVEL_RES */
        son_mif_sonrach_opt_get_log_level_resp_handler,
        /* SMIF_EVENT_SONRACH_OPT_GET_LOG_LEVEL_RES */
        /* SPR-13251 Fix Ends */
        /* SPR_19619 start */
        son_mif_sones_peer_cell_activation_resp_handler,
        /* SMIF_EVENT_SONES_PEER_CELL_ACTIVATION_RES */
        /* SPR_19619 stop */
    }
};

/*****************************************************************************
 * Function Name  : son_mif_fsm_process_event
 * Inputs         : Pointer to the SMIF global context
 *            Event that has occurred
 *                  Pointer to the msg of API received by SMIF
 * Outputs        : None
 * Returns        : None
 * Description    : This is entry point function for SON MgmtIfH FSM. This
 *                  function calls appropriate function handler based on state
 *                  and event combination.
 ****************************************************************************/
void
son_mif_fsm_process_event
(
    son_mif_gb_context_t *p_smif_gb_context,
    smif_event_et event,
    void                  *p_msg
)
{
    smif_state_et current_state = SMIF_GET_FSM_STATE(p_smif_gb_context);

    SON_UT_TRACE_ENTER();

    if ((current_state < SMIF_NUM_OF_STATES) && (event < SMIF_NUM_OF_EVENTS))
    {
        SMIF_FSM_EVENT_TRACE(smif_fsm_name,
            SMIF_FSM_EVENTS_NAMES[event],
            SMIF_FSM_STATES_NAMES[current_state]);

        /* Invoke appropriate event handler */
        (*smif_fsm[current_state][event])(p_smif_gb_context, p_msg);
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_INFO,
            "### %s: Processing Completed ###", smif_fsm_name);
    }
    else
    {
        SON_LOG(g_mif_log_on_off, p_son_mif_facility_name, SON_ERROR,
            "### %s: Unexpected state %u or event %u ###",
            smif_fsm_name, current_state, event);
    }

    SON_UT_TRACE_EXIT();
} /* son_mif_fsm_process_event */
