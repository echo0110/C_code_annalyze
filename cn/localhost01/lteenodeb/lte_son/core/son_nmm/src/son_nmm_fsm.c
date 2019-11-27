/****************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * $Id: son_nmm_fsm.c  $
 *
 ****************************************************************************
 *
 * File Description: Contains function for nmm fsm 
 *
 ****************************************************************************
 * Revision Details
 * ----------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *  Jan, 2012        gur26210    Initial         Initial Code
 *  May, 2014        Shilpi                      Coverity Fixes  
 *   July, 2014      Shilpi                      SPR 12708 Fix 
 ****************************************************************************/

/****************************************************************************
 *                 Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 *                  Project Includes
 ****************************************************************************/
#include    <son_nmm_fsm.h>
#include    <son_nmm.h>

/****************************************************************************
 *                  Private Definitions
 ****************************************************************************/
static U16  g_earfcn_count = SON_NULL;        /*S_global variable used in FSM*/
static U16  g_loop_count = SON_NULL;          /*S_global variable used in FSM*/
static U16  g_count = SON_NULL;               /*S_global variable used in FSM*/
static U16  g_son_flag = SON_NULL;            /*S_global variable used in FSM*/
static nmm_db_t *p_nmm_db = SON_PNULL;        /*NMM database structure*/
static U8   g_meas_bandwidth = SON_NULL;      /*S_global variable used in FSM*/
static U16  g_sibx_presence_bitmask = SON_NULL;
static U16  g_max_num_sib_read = SON_NULL;
static U8  g_scan_type = SON_NULL;
static nmm_state_et g_current_state = NMM_W_FOR_NULL; /*NMM_FSM current state*/
static nmm_utran_state_et g_utran_current_state = NMM_UTRAN_W_FOR_SCAN_REQ;
son_nmm_errno_et g_son_nmm_errno = SON_NMM_SUCCESS; /*error type for nmm*/

/*+ SPR 18399*/
/*- SPR 18399*/
/*****************************************************************************
 *                   Global variables                                        
 *****************************************************************************/
nmm_timer_buf_t g_son_nmm_timer_buf = {SON_FALSE, SONNMM_SUBMODULE_NMM};
/* Start: CSR 00070093 */
son_timer_t g_son_nmm_timer_id = SON_PNULL;
/*End: CSR 00070093 */

/****************************************************************************
 *                  Exported variables
 ****************************************************************************/

/***************************************************************************
 *                  Private Types
 ***************************************************************************/
/***************           START            ************
 *************** SET and GET nmm fsm state **************
 *Set current NMM FSM state*/
static void
son_nmm_set_current_state
(
 nmm_state_et state                                    /*NMM_FSM state*/
 );

/*get current state of NMM_FSM*/
static nmm_state_et
son_nmm_get_current_state
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*Set son_nmm_errno NMM_FSM state*/
void
son_nmm_set_errno
(
 son_nmm_errno_et son_nmm_errno                         /*NMM_FSM errno*/
 );

/*get son_nmm_errno*/
static son_nmm_errno_et
son_nmm_get_errno
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*************** SET and GET nmm fsm state ***************/

/************ NMM FSM MODULE & DB INITIALIZATION *******
 *This function initialize nmm module data*/
static void
son_nmm_module_data_initialization
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*This function initialise nmm_db_t structures object*/
static son_return_et 
son_nmm_db_initialisation
(
 start_scan_req_t *p_start_scan_req   /*staer scan req data*/
 );

/************ NMM FSM MODULE & DB INITIALIZATION ********/

/*********** NMM FSM MODULE & DB cleanup function ******
 *Clean NMM dateabase*/
static void
son_nmm_db_free
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*clean earfcn from database who don't have their rssi value*/
static son_return_et 
son_nmm_cleanup_earfcn
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*clean earfcn_pci pair*/
static son_return_et 
son_nmm_cleanup_earfcn_pci
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*clean pci from database who don't have their rsrp value*/
static son_return_et 
son_nmm_cleanup_pci
(
 U16 earfcn_index             /*earfcn index*/
 );

/*********** NMM FSM MODULE & DB cleanup function *******/

/*********** NMM FSM common utility functions ***********/

/******* num_earfcn or num_pci not equal to ZERO *******
 *search earfcn_rssi or pci_rsrp pair and fill in database*/
static void 
son_nmm_search_and_fill
(
 U8 data,                                    /*rssi or rsrp*/
 U16 target                                  /*earfcn or pci*/
 );

/*binary search for earfcn*/
static S16 
son_nmm_binary_search_earfcn
( 
 freq_channel_t  *p_arr,                 /*Array to be search*/
 S16             start,                  /*Start of array*/
 S16             end,                    /*end of array*/
 U16             target,                 /*target to be searched*/
 U8              *p_flag                 /*flag to show presence of target*/           
 );

/*Binary search for pci*/
static S16 
son_nmm_binary_search_pci
(
 cell_t *p_arr,                        /*pointer of array*/
 S16 start,                              /*start of array*/
 S16 end,                                /*end of array*/
 U16 target,                             /*target to be search*/
 U8 *p_flag                       /*flag to show presence of target*/
 );

/*****************************************/

/******* num_earfcn or num_pci is equal to ZERO ********
 *fill earfcn_rssi or pci_rsrp pair in nmm_db when num is zero*/
static void 
son_nmm_fill_data
(
 U8 data1,                               /*rssi or rsrp*/
 U16 data2                               /*earfcn or pci*/
 );

/*search existence of earfcn in database*/
static S32 
son_nmm_earfcn_present
(
 U16 earfcn                              /*earfcn value to be searched*/
 );

/*search existence of pci in database*/
static S16 
son_nmm_pci_present
(
 U16 pci                                 /*pci value to be searched*/
 );

/*filling earfcn_rssi value in database at specified place*/
static void 
son_nmm_fill_earfcn_rssi_at_index
(
 U16 index,                              /*index where data have to store*/
 U8 rssi,                                /*rssi value to be filled*/
 U16 earfcn                              /*earfcn value to be filled*/
 );

/*filling pci_rsrp value in database at specified place*/
static void 
son_nmm_fill_pci_rsrp_at_index
(
 U16 index,                              /*index where data have to store*/
 U8 rsrp,                                /*rsrp value to be filled*/
 U16 pci                                 /*pci value to be filled*/
 );

/*****************************************/

/** Short earfcn or pci received from external module **
 *quick short*/
static void
son_nmm_q_sort
(
 U16 *p_data,                            /*Array to be shorted*/
 S16 left,                               /*leftmost index*/
 S16 right                               /*right most index*/
 );

/*wraper on quick short*/
static void
son_nmm_quick_sort
(
 U16 *p_data,                            /*Array to be shorted*/
 S16 array_size                          /*Size of array*/
 );
/*SPR No:10540  changes start */    
static void
son_nmm_quick_sort_earfcn
(
 earfcn_pci_info_t *p_data,                            /*Array to be shorted*/
 S16 array_size                          /*Size of array*/
 );
/*SPR No:10540  changes end */    

/*payload length*/
static U32
son_nmm_get_payload_len
(
 void *p_msg
 );

/*****************************************/

/****** Default action when unwanted event occur *******
 * This function perform default action*/
son_return_et 
son_nmm_default_action
(
 /*unused*/ void *p_msg
 );

/****************************************
 *jump to next pci for pbch req*/
static son_return_et
son_nmm_move_to_next_cell
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/******* NMM to PHY req initiator *********
 *intiate son_nmm_rssi_meas_req req to phy*/
static son_return_et
son_nmm_rssi_meas_req_start
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*intiate son_nmm_cell_search_req req to phy*/
static son_return_et 
son_nmm_cell_search_req_start
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*intiate son_nmm_pbch_config_req req to phy*/
static son_return_et
son_nmm_pbch_config_req_start
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*intiate son_nmm_sib1_config_req req to phy*/
static son_return_et
son_nmm_sib1_config_req_start
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*intiate son_nmm_bcch_config_req req to phy*/
static son_return_et
son_nmm_bcch_config_req_start
(
 /* + SPR 17439 */
 sib_type_1_Info_t *p_sib_info_type1
 /* - SPR 17439 */
 );

/*******************************************
 ****stoping various type of req****
 *This function Stop nmm module*/

static void
son_nmm_clean_module
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*stop rssi_meas_req */
static void
son_nmm_stop_rssi_meas_req
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/* stop cell_search req*/
static void
son_nmm_stop_cell_search
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );
/*stop pbch_req */
static void
son_nmm_stop_pbch_req
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*stop bcch_req*/
static void
son_nmm_stop_bcch_req
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*********** NMM FSM common utility functions ***********/

/********* NMM FSM EXTERNAL module event handler ********/

/*This function process nmm_rssi_meas_req_event \
 * received from nmm_controler*/
son_return_et
son_nmm_rssi_meas_req_handler
(
 void *p_msg            /*data received from external module*/
 );

/*This function handle nmm_rssi_meas_ind event received from PHY*/
static son_return_et
son_nmm_rssi_meas_ind_handler
(
 void *p_msg            /*data received from external module*/
 );

/*This function handle nmm_rssi_meas_cnf event received from PHY*/
static son_return_et
son_nmm_rssi_meas_cnf_handler
(
 /*unused*/ void *p_msg
 );

/*This function handle nmm_cell_search_ind event received from PHY*/
static son_return_et
son_nmm_cell_search_ind_handler
(
 void *p_msg            /*data received from external module*/
 );

/*This function handle nmm_cell_search_cnf event received from PHY*/
static son_return_et
son_nmm_cell_search_cnf_handler
(
 /*unused*/ void *p_msg
 );

/*This function handle nmm_pbch_config_cnf event received from PHY*/
static son_return_et
son_nmm_pbch_config_cnf_handler
(
 void *p_msg              /*data received from external module*/
 );

/*This function handle son_nmm_pbch_data_ind_handler event received from PHY*/
static son_return_et
son_nmm_pbch_data_ind_handler
(
 void *p_msg              /*data received from external module*/
 );

/*This function handle son_nmm_sib1_config_cnf_handler event received from PHY*/
static son_return_et
son_nmm_sib1_config_cnf_handler
(
 /*unused*/ void *p_msg
 );

/*This function handle nmm_bcch_data_ind_pre_req event received from PHY*/
static son_return_et
son_nmm_bcch_data_ind_pre_req_handler
(
 void *p_msg              /*data received from external module*/
 );

/*This function handle nmm_bcch_config_cnf event received from PHY*/
static son_return_et
son_nmm_bcch_config_cnf_handler
(
 /*unused*/ void *p_msg
 );

/* This function handle nmm_bcch_data_ind event received from PHY */
static son_return_et
son_nmm_bcch_data_ind_handler
(
 void *p_msg              /*data received from external module*/
 );

/*This function process son_nmm_stop_scan_req_handler event \
 * received from nmm_controler*/
son_return_et
son_nmm_stop_scan_req_handler
(
 void *p_msg              /*data received from external module*/
 );

static son_return_et
son_nmm_utran_si_msg_req_handler
(
 void *p_msg              /*data received from external module*/
 );

/********* NMM FSM EXTERNAL module event handler ********/

/************        NMM FSM RESP MSG       ************
 * This function prepare responce msg and send to upper layer*/
static void
son_nmm_prepare_send_resp
(
 son_nmm_errno_et son_nmm_resp            /*resp status*/
 );

/*This function create son_resp msg*/
static void
son_nmm_create_resp_msg
(
 nmm_scan_res_list_t *p_scan_res_list     /*resp data head node*/
 );

/************        NMM FSM RESP MSG       ************
 * Function prototype for UMTS, will be modify later */

/*This function handles the UMTS default cases */
static son_return_et
son_nmm_utran_default_handler
(
 void *p_msg
 );

/*This function handles the UMTS scan request */
static son_return_et
son_nmm_utran_si_msg_req_handler
(
 void *p_msg
 );

/*This function handles the UMTS SI messages */
static son_return_et
son_nmm_utran_si_msg_ind_handler
(
 void *p_msg
 );

/*This function creates and sends the scan response */
static son_return_et
son_nmm_umts_create_send_message
(
 nmm_scan_res_list_t *p_scan_res_list,
 start_scan_res_utran_data_t *p_start_scan_res_utran_data
 );

/*This function gets the UMTS present state */
static nmm_utran_state_et
son_nmm_utran_get_current_state
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 );

/*This function sets the UMTS FSM state */
static void
son_nmm_utran_set_current_state
(
 nmm_utran_state_et state
 );

/*This function Frees the Sibs memory */
void
son_nmm_free_mem_sibx
(
 U16 earfcn_index,
 U16 pci_index
 );

#define SON_PHY_UTRAN_SI_MSG_IND 10000

/***************           END            ***************/

/****************************************************************************
 *                  Macro definitions
 ****************************************************************************/

/* SPR-12708 Fix Starts */
/* Default timer for NMM. The value is in seconds. */
#define DEFAULT_DURATION 100000
/* SPR-12708 Fix Ends */
/*SPR_15949 Fix start*/
#define MIN_RSSI_THRESHHOLD 50  
/*SPR_15949 Fix end*/

/****************************************************************************
 *                  Private Function Prototypes
 ****************************************************************************/        

/****************************************************************************
 *                  Variable initialisation
 ****************************************************************************/
/*function pointer initialization of state machine*/
nmm_fsm_func nmm_fsm_func_name[MAX_NUM_STATE][MAX_NUM_EVENT + 1] =
{
    /*NMM_W_FOR_NULL*/
    {
        son_nmm_rssi_meas_req_handler, 
        son_nmm_stop_scan_req_handler, 
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,  
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action, 
    },
    /*NMM_W_FOR_RSSI_MEAS_IND*/
    {
        son_nmm_default_action, 
        son_nmm_stop_scan_req_handler, 
        son_nmm_rssi_meas_ind_handler,
        son_nmm_rssi_meas_cnf_handler, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action,
    }, 
    /* NMM_W_FOR_CELL_SEARCH_IND */
    {
        son_nmm_default_action, 
        son_nmm_stop_scan_req_handler, 
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_cell_search_ind_handler, 
        son_nmm_cell_search_cnf_handler, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action,
    }, 
    /*NMM_W_FOR_PBCH_CONFIG_CNF*/
    {
        son_nmm_default_action, 
        son_nmm_stop_scan_req_handler, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_pbch_config_cnf_handler,
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action,
    }, 
    /*NMM_W_FOR_PBCH_DATA_IND*/
    {
        son_nmm_default_action, 
        son_nmm_stop_scan_req_handler, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action,
        son_nmm_pbch_data_ind_handler, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action,
    },
    /*NMM_W_FOR_SIB1_CONFIG_CNF*/
    {
        son_nmm_default_action, 
        son_nmm_stop_scan_req_handler, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_sib1_config_cnf_handler, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action,
    },
    /*NMM_W_FOR_BCCH_DATA_IND_PRE_REQ*/
    {
        son_nmm_default_action, 
        son_nmm_stop_scan_req_handler, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_bcch_data_ind_pre_req_handler, 
        son_nmm_default_action,
    }, 
    /*NMM_W_FOR_BCCH_CONFIG_CNF*/
    {
        son_nmm_default_action, 
        son_nmm_stop_scan_req_handler, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_bcch_config_cnf_handler, 
        son_nmm_default_action, 
        son_nmm_default_action, 
    }, 
    /*NMM_W_FOR_BCCH_DATA_IND*/
    {
        son_nmm_default_action, 
        son_nmm_stop_scan_req_handler, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_default_action, 
        son_nmm_default_action,
        son_nmm_default_action, 
        son_nmm_bcch_data_ind_handler,
        son_nmm_default_action, 
    } 
};

/* Function pointer initialization for UMTS state machine */

nmm_fsm_func nmm_utran_fsm[MAX_NUM_UTRAN_STATES][MAX_NUM_UTRAN_EV] =
{
    /* NMM_UTRAN_W_FOR_SCAN_REQ*/
    {
        son_nmm_utran_si_msg_req_handler,      /*NMM_START_UTRAN_SCAN_EV */                                 
        son_nmm_utran_default_handler,         /*SON_PHY_UTRAN_SI_MSG_IND_EV */  
    },
    /*NMM_UTRAN_W_FOR_SI_MSG_IND*/
    {
        son_nmm_utran_default_handler,         /*NMM_START_UTRAN_SCAN_EV */
        son_nmm_utran_si_msg_ind_handler,      /*SON_PHY_UTRAN_SI_MSG_IND_EV */
    }
};    

/*****************************************************************************
 *                    Functions forward declarations                         *
 *****************************************************************************/

        /*****************************************************************************
         *                    Functions implementations           
         *****************************************************************************/

        /*****************************************************************************
         * Function Name  : son_nmm_set_current_state
         * Inputs         : state
         * Outputs        : None
         * Returns        : None
         * Description    : This function configure nmm_fsm current state
         ****************************************************************************/
    static void
    son_nmm_set_current_state
(
 nmm_state_et state               /*nmm_fsm current state*/
 )
{
    SON_UT_TRACE_ENTER();
    /*set current state of fsm*/
    g_current_state = state;
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_nmm_get_current_state
 * Inputs         : None
 * Outputs        : None
 * Returns        : Enum nmm_state_e
 * Description    : This function provide current state of nmm_fsm
 ****************************************************************************/
    static nmm_state_et
    son_nmm_get_current_state
    (
     /* + SPR 17439 */
     void
     /* - SPR 17439 */
    )
{
    return g_current_state;
}

/*****************************************************************************
 * Function Name  : son_nmm_get_payload_len 
 * Inputs         : p_msg
 * Outputs        : None
 * Returns        : cspl_hdr.paramlen - external header size
 * Description    : This function provide payload len 
 ****************************************************************************/
static U32
    son_nmm_get_payload_len
(
 void *p_msg
 )
{
    STACKAPIHDR cspl_hdr = {0};
    U32 ret_val = SON_NULL;


    son_parse_cspl_header((U8 *)p_msg, &cspl_hdr);

#ifdef __SON_FAPI__
    ret_val = cspl_hdr.paramlen - SON_FAPI_MSG_API_HDR_SIZE;
#endif

    return ret_val;
}

/*****************************************************************************
 * Function Name  : son_nmm_set_errno 
 * Inputs         : son_nmm_errno
 * Outputs        : None
 * Returns        : None
 * Description    : This function configure errno for nmm_fsm 
 ****************************************************************************/
        /*Set son_nmm_errno NMM_FSM state*/
        void
    son_nmm_set_errno
(
 son_nmm_errno_et son_nmm_errno                         /*NMM_FSM errno*/
 )
{
    g_son_nmm_errno = son_nmm_errno;    
}

/*****************************************************************************
 * Function Name  : son_nmm_get_errno
 * Inputs         : None
 * Outputs        : None
 * Returns        : Enum son_nmm_errno_et
 * Description    : This function provide current errno
 ****************************************************************************/
        /*get son_nmm_errno*/
        static son_nmm_errno_et
        son_nmm_get_errno
        (
         /* + SPR 17439 */
         void
         /* - SPR 17439 */
        )
{
    return g_son_nmm_errno;
}

/*****************************************************************************
 * Function Name  : son_nmm_cleanup_pci
 * Inputs         : earfcn_index - frequency index for the PCI.                   
 * Outputs        : None
 * Returns        : SON_FAILURE     - No member to free
 *                  SON_SUCCESS     - database clean successfully
 * Description    : This function clean database regarding PCI.
 ****************************************************************************/
        static son_return_et
    son_nmm_cleanup_pci
(
 U16 earfcn_index
 )
{
    U16 temp_index = SON_NULL;               /*index num for array*/
    U8 pci_index = SON_NULL;                 /*Pci index*/
    son_return_et ret = SON_FAILURE;


    SON_UT_TRACE_ENTER();

    /*SPR_15671 Fix start*/
    /*check number of pci in earfcn before processing*/
    if(SON_NULL == (GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci)))
    {
        GET_FREQ_CHANNEL_PARM(earfcn_index, rssi ) = SON_ONE; 
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_DETAILED, 
                "Num of pci [%d] in earfcn [%d] no need to check rsrp", 
                GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci), 
                GET_FREQ_CHANNEL_PARM(earfcn_index,earfcn));
        ret = SON_SUCCESS;
        SON_UT_TRACE_EXIT();
        return ret;
    }
    /*SPR_15671 Fix end*/


    /*Loop controller variable: Number of PCI in a earfcn*/
    for (pci_index = SON_NULL;
            /* Klockwork 2.1.3 */
            (pci_index < GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci)) &&
            (pci_index < SON_MAX_NO_PCI); 
            pci_index++)
    {
        /*check on rsrp*/
        if (GET_CELL_PARM(earfcn_index, pci_index, rsrp) != SON_NULL)
        {
            continue;
        }
        else
        {
            temp_index = pci_index;
            pci_index++;

            /*Loop controller variable: Number of PCI in a earfcn*/
            for (;
                    /* Klockwork 2.1.3 */
                    (pci_index < GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci)) &&
                    (pci_index < SON_MAX_NO_PCI); 
                    pci_index++)
            {
                /*check on rsrp, cell validety and current state*/
                if (GET_CELL_PARM(earfcn_index, pci_index, rsrp) == SON_NULL)
                {
                    continue;
                }
                else
                {
                    GET_CELL_PARM(earfcn_index, temp_index, rsrp ) = 
                        GET_CELL_PARM(earfcn_index, pci_index, rsrp);
                    GET_CELL_PARM(earfcn_index, temp_index, cell_id ) = 
                        GET_CELL_PARM(earfcn_index, pci_index, cell_id );
                    GET_CELL_PARM(earfcn_index, pci_index, cell_id ) = 
                        SON_NULL;
                    GET_CELL_PARM(earfcn_index, pci_index, rsrp ) = 
                        SON_NULL;
                    pci_index = temp_index;
                    break;
                }
            }
        }
    }

    /*is rsrp at index 0 equals to Zero*/
    if (GET_CELL_PARM(earfcn_index, SON_NULL, rsrp ) == SON_NULL)
    {
        GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci ) = SON_NULL;
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                "PCI in DB is already free");
        ret = SON_FAILURE;
    }
    /*rsrp not equal to Zero
     *temp_index not equal to Zero*/
    else
        /* Start: CSR 00070093 */
    {
        if (temp_index != SON_NULL)
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                    " NUM of pci before cleanup in earfcn \
                    %d is %d: ", GET_FREQ_CHANNEL_PARM(earfcn_index, earfcn ), \
                    GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci ));

            GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci ) = temp_index;
        }

        ret = SON_SUCCESS;

        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NUM of pci after cleanup in earfcn \
                %d is %d: ", GET_FREQ_CHANNEL_PARM(earfcn_index, earfcn ), \
                GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci ));
    }
    /*End: CSR 00070093 */

    SON_UT_TRACE_EXIT();
    return ret;
} /* son_nmm_cleanup_pci */

/*****************************************************************************
 * Function Name  : son_nmm_cleanup_earfcn_pci 
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failure
 *                  SON_SUCCESS     - Indicates success
 * Description    : This function clean database regarding EARFCN_PCI pair
 ****************************************************************************/
            static son_return_et
            son_nmm_cleanup_earfcn_pci
            (
             /* + SPR 17439 */
             void
             /* - SPR 17439 */
            )
{
    U16 temp_index = SON_NULL;               /*array index*/
    U16 earfcn_index = SON_NULL;             /*earfcn index*/
    U16 pci_index = SON_NULL;                /*pci index*/
    son_return_et ret = SON_FAILURE;


    SON_UT_TRACE_ENTER();
    /* Start: CSR 00070093 */
    for (earfcn_index = SON_NULL;
            (earfcn_index < GET_NMM_DB_PARM(num_earfcn )) &&
            (earfcn_index < SON_MAX_NO_EARFCN) ; 
            earfcn_index++)
        /*End: CSR 00070093 */
    {
        /*Check for son_nmm_cleanup_pci return value*/
        if (SON_SUCCESS == son_nmm_cleanup_pci( earfcn_index ))
        {
            continue;
        }
        else
        {
            temp_index = earfcn_index;

            GET_FREQ_CHANNEL_PARM(earfcn_index, rssi ) = SON_NULL;
            earfcn_index++;
            for (;
                    /* Klockwork 2.1.3 */
                    (earfcn_index < GET_NMM_DB_PARM(num_earfcn )) && 
                    (earfcn_index < SON_MAX_NO_EARFCN); 
                    earfcn_index++)
            {
                /*Check for son_nmm_cleanup_pci return value*/
                if (son_nmm_cleanup_pci(earfcn_index) ==  SON_FAILURE)
                {
                    GET_FREQ_CHANNEL_PARM(earfcn_index, rssi ) = SON_NULL;		    
                    continue;
                }
                else
                {
                    GET_FREQ_CHANNEL_PARM(temp_index, rssi ) = \
                                                               GET_FREQ_CHANNEL_PARM(earfcn_index, rssi );

                    GET_FREQ_CHANNEL_PARM(temp_index, earfcn ) = \
                                                                 GET_FREQ_CHANNEL_PARM(earfcn_index, earfcn );

                    GET_FREQ_CHANNEL_PARM(temp_index, meas_bandwidth) = \
                                                                        GET_FREQ_CHANNEL_PARM(earfcn_index, meas_bandwidth );

                    GET_FREQ_CHANNEL_PARM(temp_index, num_pci ) = \
                                                                  GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci );
                    /* Start: CSR 00070093 */                
                    for (pci_index = SON_NULL;
                            (pci_index < GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci )) &&
                            (pci_index < SON_MAX_NO_PCI);
                            pci_index++)
                        /*End: CSR 00070093 */
                    {
                        GET_CELL_PARM(temp_index, pci_index, rsrp ) = 
                            GET_CELL_PARM(earfcn_index, pci_index, rsrp);

                        GET_CELL_PARM(temp_index, pci_index, cell_id ) = 
                            GET_CELL_PARM(earfcn_index, pci_index, cell_id );

                        GET_CELL_PARM(earfcn_index, pci_index, cell_id ) =
                            SON_NULL;

                        GET_CELL_PARM(earfcn_index, pci_index, rsrp ) =
                            SON_NULL;
                    }

                    GET_FREQ_CHANNEL_PARM(earfcn_index, rssi ) = \
                                                                 SON_NULL;
                    GET_FREQ_CHANNEL_PARM(earfcn_index, earfcn ) = \
                                                                   SON_NULL;
                    GET_FREQ_CHANNEL_PARM(earfcn_index, meas_bandwidth) = \
                                                                          SON_NULL;
                    GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci ) = \
                                                                    SON_NULL;
                    earfcn_index = temp_index;
                    break;
                }
            }
        }
    }

    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
            " NUM of earfcn before cleanup in band \
            %d is %d: "                                                 , GET_NMM_DB_PARM(freq_band_id), \
            GET_NMM_DB_PARM(num_earfcn));
    /*Check for rssi value at index Zero*/
    if (GET_FREQ_CHANNEL_PARM(SON_NULL, rssi ) == SON_NULL)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                SON_ERROR, "SON_PHY_NULL_DATA");
        ret = SON_FAILURE;
    }
    /*Check for rssi value*/
    else
    {
        if (SON_NULL != temp_index)
        {
            GET_NMM_DB_PARM(num_earfcn ) = temp_index;
        }

        ret = SON_SUCCESS;

        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NUM of earfcn after cleanup in band %d is %d: ", \
                GET_NMM_DB_PARM(freq_band_id), GET_NMM_DB_PARM(num_earfcn));
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* son_nmm_cleanup_earfcn_pci */

/*****************************************************************************
 * Function Name  : son_nmm_cleanup_earfcn 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function clean database regarding EARFCN
 ****************************************************************************/
static son_return_et
son_nmm_cleanup_earfcn
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 )
{
    U16 temp_index = SON_NULL;               /*array index*/
    U16 earfcn_index = SON_NULL;             /*earfcn index*/
    son_return_et ret = SON_FAILURE;


    SON_UT_TRACE_ENTER();
    for (;
            /* Klockwork 2.1.3 */
            (earfcn_index < GET_NMM_DB_PARM(num_earfcn)) &&
            (earfcn_index < SON_MAX_NO_EARFCN); 
            earfcn_index++)
    {
        /*Check for rssi value*/
        if (GET_FREQ_CHANNEL_PARM(earfcn_index, rssi ) != SON_NULL)
        {
            continue;
        }
        else
        {
            temp_index = earfcn_index;
            earfcn_index++;
            for (;
                    /* Klockwork 2.1.3 */
                    (earfcn_index < GET_NMM_DB_PARM(num_earfcn )) &&
                    (earfcn_index < SON_MAX_NO_EARFCN); 
                    earfcn_index++)
            {
                /*Check for rssi value*/
                if (GET_FREQ_CHANNEL_PARM(earfcn_index, rssi ) == \
                        SON_NULL)
                {
                    continue;
                }
                else
                {
                    GET_FREQ_CHANNEL_PARM(temp_index, rssi ) = \
                                                               GET_FREQ_CHANNEL_PARM(earfcn_index, rssi );

                    GET_FREQ_CHANNEL_PARM(temp_index, earfcn ) = \
                                                                 GET_FREQ_CHANNEL_PARM(earfcn_index, earfcn );

                    GET_FREQ_CHANNEL_PARM(temp_index, meas_bandwidth ) = \
                                                                         GET_FREQ_CHANNEL_PARM(earfcn_index, meas_bandwidth );

                    GET_FREQ_CHANNEL_PARM(earfcn_index, rssi ) = \
                                                                 SON_NULL;

                    GET_FREQ_CHANNEL_PARM(earfcn_index, earfcn ) = \
                                                                   SON_NULL;

                    GET_FREQ_CHANNEL_PARM(earfcn_index, meas_bandwidth ) = \
                                                                           SON_NULL;
                    earfcn_index = temp_index;
                    break;
                }
            }
        }
    }

    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
            " NUM of earfcn before cleanup in band \
            %d is %d: "                                                 , GET_NMM_DB_PARM(freq_band_id), \
            GET_NMM_DB_PARM(num_earfcn));

    /*Check for rssi value*/
    if (GET_FREQ_CHANNEL_PARM(SON_NULL, rssi ) == SON_NULL)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                SON_ERROR, "SON_NMM_UNKNOWN_FAILURE");
        ret = SON_FAILURE;
    }
    /*Check for rssi value*/
    else
    {
        if (SON_NULL != temp_index)
        {
            GET_NMM_DB_PARM(num_earfcn ) = temp_index;
        }

        ret = SON_SUCCESS;
    }

    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
            " NUM of earfcn after cleanup in band %d is %d: ", \
            GET_NMM_DB_PARM(freq_band_id), GET_NMM_DB_PARM(num_earfcn));

    SON_UT_TRACE_EXIT();
    return ret;
} /* son_nmm_cleanup_earfcn */

/*****************************************************************************
 * Function Name  : son_nmm_db_free 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None 
 * Description    : This function clean nmm_database
 ****************************************************************************/
            static void
            son_nmm_db_free
            (
             /* + SPR 17439 */
             void
             /* - SPR 17439 */
            )
{
    /* Coverity CID-55061 Fix Starts */
    U16     earfcn_index = SON_NULL;         /* local earfcn index */
    U8      pci_index = SON_NULL;            /* local pci index */


    SON_UT_TRACE_ENTER();

    /*free all allocated memory
     *check for p_nmm_db*/
    if (p_nmm_db != SON_PNULL)
    {   
        for (earfcn_index = SON_NULL; \
                earfcn_index < SON_MAX_NO_EARFCN; \
                earfcn_index++)
        {
            pci_index = SON_NULL;
            for (; \
                    pci_index < SON_MAX_NO_PCI; \
                    pci_index++)
            {
                /*check for p_mib*/
                if (GET_CELL_PARM(earfcn_index, pci_index, p_mib ) != \
                        SON_PNULL)
                {
                    son_mem_free( GET_CELL_PARM(earfcn_index, pci_index, \
                                p_mib ));  
                    GET_CELL_PARM(earfcn_index, pci_index, p_mib ) = \
                                                                     SON_PNULL;
                }

                /*check for p_sib_type_1_Info*/
                if (GET_CELL_PARM(earfcn_index, pci_index, p_sib_type_1_Info) \
                        != SON_PNULL)
                {
                    son_mem_free(GET_CELL_PARM(earfcn_index, pci_index, \
                                p_sib_type_1_Info));
                    GET_CELL_PARM(earfcn_index, pci_index, \
                            p_sib_type_1_Info) = SON_PNULL;
                }

                son_nmm_free_mem_sibx(earfcn_index, pci_index);
            }
        }

        son_mem_free( p_nmm_db );
        p_nmm_db = SON_PNULL;
    }
    else
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                "wrong call son_nmm_db_free");
    }

    g_earfcn_count = SON_NULL;
    g_loop_count = SON_NULL;
    g_count = SON_NULL;        
    g_son_flag = SON_FALSE;
    g_sibx_presence_bitmask = SON_NULL;

    SON_UT_TRACE_EXIT();
    /* Coverity CID-55061 Fix Ends */
} /* son_nmm_db_free */
/*SPR No:10540  changes start */    
/*****************************************************************************
 * Function Name  : son_nmm_q_sort_earfcn 
 * Inputs         : p_data, left, right
 * Outputs        : None
 * Returns        : None 
 * Description    : This function sorts given data based on earfcn.
 ****************************************************************************/
            static void
    son_nmm_q_sort_earfcn
(
 earfcn_pci_info_t *p_data,                       /*Array to be shorted*/
 S16 left,                          /*leftmost index*/
 S16 right                          /*right most index*/
 )
{
    /* SPR 21438 Fix Start */
    U32 pivot = SON_NULL;             /*value on which quick short work*/
    /* SPR 21438 Fix End */
    S16 l_hold = SON_NULL;            /*left index holder*/
    S16 r_hold = SON_NULL;            /*right index holder*/
    earfcn_pci_info_t *temp = SON_PNULL;
    SON_UT_TRACE_ENTER();
    l_hold = left;
    r_hold = right;
    /* SPR 21438 Fix Start */
    pivot = p_data[left].earfcn;
    /* SPR 21438 Fix End */
    if (SON_PNULL == (temp= (earfcn_pci_info_t * )son_mem_get(sizeof(earfcn_pci_info_t))))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                "Mem allocation fail");
        son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);
        return;
    }


    /*execute loop till left less then right*/
    while (left < right)
    {
        SON_MEMSET(temp, SON_NULL, sizeof(earfcn_pci_info_t));

        /*execute loop till p_data[right] >= pivot
         *execute loop till left less then right*/
        while ((p_data[right].earfcn >= pivot) && (left < right))
        {
            right--;
        }

        if (left != right)
        {
            SON_MEMCPY(temp,&p_data[left],sizeof(earfcn_pci_info_t));
            SON_MEMCPY(&p_data[left],&p_data[right],sizeof(earfcn_pci_info_t));
            SON_MEMCPY(&p_data[right],temp,sizeof(earfcn_pci_info_t));

        }

        /*execute loop till p_data[left] <= pivot
         *execute loop till left less then right*/
        while ((p_data[left].earfcn <= pivot) && (left < right))
        {
            left++;
        }

        if (left != right)
        {
            SON_MEMCPY(temp,&p_data[right],sizeof(earfcn_pci_info_t));
            SON_MEMCPY(&p_data[right],&p_data[left],sizeof(earfcn_pci_info_t));
            SON_MEMCPY(&p_data[left],temp,sizeof(earfcn_pci_info_t));

        }

    }
    son_mem_free(temp);

    p_data[left].earfcn = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;
    /*check if left < pivot*/
    if (left < pivot)
    {
        son_nmm_q_sort_earfcn(p_data, left, pivot - SON_ONE);
    }

    /*check if right > pivot*/
    if (right > pivot)
    {
        son_nmm_q_sort_earfcn(p_data, pivot + SON_ONE, right);
    }
    SON_UT_TRACE_EXIT();
} /* son_nmm_q_sort_earfcn */


/*SPR No:10540  changes end */    

/*****************************************************************************
 * Function Name  : son_nmm_q_sort 
 * Inputs         : p_data, left, right
 * Outputs        : None
 * Returns        : None 
 * Description    : This function sort given data
 ****************************************************************************/
static void 
    son_nmm_q_sort
(
 U16 *p_data,                       /*Array to be shorted*/               
 S16 left,                          /*leftmost index*/
 S16 right                          /*right most index*/
 )
{
    S16 pivot = SON_NULL;             /*value on which quick short work*/
    S16 l_hold = SON_NULL;            /*left index holder*/
    S16 r_hold = SON_NULL;            /*right index holder*/

    SON_UT_TRACE_ENTER();

    l_hold = left;
    r_hold = right;
    pivot = p_data[left];

    /*execute loop till left less then right*/
    while (left < right)
    {
        /*execute loop till p_data[right] >= pivot
         *execute loop till left less then right*/
        while ((p_data[right] >= pivot) && (left < right))
        {
            right--;
        }

        if (left != right)
        {
            p_data[left] = p_data[right];
            left++;
        }

        /*execute loop till p_data[left] <= pivot
         *execute loop till left less then right*/
        while ((p_data[left] <= pivot) && (left < right))
        {
            left++;
        }

        if (left != right)
        {
            p_data[right] = p_data[left];
            right--;
        }
    }

    p_data[left] = pivot;
    pivot = left;
    left = l_hold;
    right = r_hold;
    /*check if left < pivot*/
    if (left < pivot)
    {
        son_nmm_q_sort(p_data, left, pivot - SON_ONE);
    }

    /*check if right > pivot*/
    if (right > pivot)
    {
        son_nmm_q_sort(p_data, pivot + SON_ONE, right);
    }

    SON_UT_TRACE_EXIT();
} /* son_nmm_q_sort */

/*SPR No:10540  changes start */    

/*****************************************************************************
 * Function Name  : son_nmm_quick_sort_earfcn 
 * Inputs         : p_data, array_size
 * Outputs        : None
 * Returns        : None 
 * Description    : This function calls the function to sort data based on earfcn.
 ****************************************************************************/
 static void 
    son_nmm_quick_sort_earfcn
(
 earfcn_pci_info_t *p_data, 
 S16 array_size
 )
{
    SON_UT_TRACE_ENTER();

    /*calling quick short*/
    son_nmm_q_sort_earfcn(p_data, SON_NULL, array_size - SON_ONE);

    SON_UT_TRACE_EXIT();
}
/*SPR No:10540  changes end */    


/*****************************************************************************
 * Function Name  : son_nmm_quick_sort 
 * Inputs         : p_data, array_size
 * Outputs        : None
 * Returns        : None 
 * Description    : This function short data
 ****************************************************************************/
    static void 
    son_nmm_quick_sort
(
 U16 *p_data, 
 S16 array_size
 )
{
    SON_UT_TRACE_ENTER();

    /*calling quick short*/
    son_nmm_q_sort(p_data, SON_NULL, array_size - SON_ONE);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_nmm_binary_search_earfcn 
 * Inputs         : *p_arr
 *                  start
 *                  end
 *                  target
 *                  *p_flag                   
 * Outputs        : None
 * Returns        : Index number
 * Description    : do binary_search on give value
 ****************************************************************************/
    static S16
    son_nmm_binary_search_earfcn
(
 freq_channel_t *p_arr,
 S16 start,
 S16 end,
 U16 target,
 U8 *p_flag
 )
{
    S32 mid = SON_NULL;                      /* middle index if array */


    SON_UT_TRACE_ENTER();

    /*execute loop till start<=end */ 
    while (start <= end)
    {
        mid = (start + end) / 2;
        /*check for target*/
        if (target < p_arr[mid].earfcn)
        {
            end = mid - SON_ONE;
            continue;
        }
        /*check for target*/
        else if (target > p_arr[mid].earfcn)
        {
            start = mid + SON_ONE;
            continue;
        }
        /*if target == p_arr[mid].earfcn*/
        else
        {
            *p_flag = SON_TRUE;
            break;
        }
    }

    SON_UT_TRACE_EXIT();
    return mid;
} /* son_nmm_binary_search_earfcn */

/*****************************************************************************
 * Function Name  : son_nmm_binary_search_pci 
 * Inputs         : *p_arr
 *                  start
 *                  end
 *                  target
 *                  *p_flag                
 * Outputs        : None
 * Returns        : Index number
 * Description    : do binary_search on give value
 ****************************************************************************/
    static S16 
    son_nmm_binary_search_pci
(
 cell_t *p_arr,
 S16 start, 
 S16 end, 
 U16 target, 
 U8 *p_flag
 )
{
    S16 mid = SON_NULL;                      /* middle index if array */


    SON_UT_TRACE_ENTER();

    /*execute loop till start<=end*/
    while (start <= end)
    {
        mid = (start + end) / 2;

        /*check for target*/
        if (target < p_arr[mid].cell_id)
        {
            end = mid - SON_ONE;
            continue;
        }
        /*check for target*/
        else if (target > p_arr[mid].cell_id)
        {
            start = mid + SON_ONE;
            continue;
        }

        /*target == p_arr[mid].cell_id*/
        else
        {
            *p_flag = SON_TRUE;
            break;
        }
    }

    SON_UT_TRACE_EXIT();
    return mid;
} /* son_nmm_binary_search_pci */

/*****************************************************************************
 * Function Name  : son_nmm_search_and_fill 
 * Inputs         : data
 *                  target 
 * Outputs        : None
 * Returns        : None
 * Description    : This function search given value in database and fill at 
 *                  proper position
 ****************************************************************************/
static void 
    son_nmm_search_and_fill
(
 U8 data, 
 U16 target
 )
{
    U16 start = SON_NULL;                    /*start of array */
    U16 end = SON_NULL;                      /* end of array */ 
    S16 index = SON_NULL;                    /*array index*/
    U8  flag = SON_NULL;                     /*true or false*/ 


    SON_UT_TRACE_ENTER();

    /*check for current state of fsm*/
    if (son_nmm_get_current_state() == NMM_W_FOR_RSSI_MEAS_IND)
    {
        end = GET_NMM_DB_PARM(num_earfcn );

        /*check existence of target in data base*/
        index = son_nmm_binary_search_earfcn(
                GET_NMM_DB_PARM(freq_channel ), start, end, target, &flag);
        /*check for flag value*/
        if (flag == SON_TRUE)
        {
            GET_FREQ_CHANNEL_PARM(index, rssi) = data;
        }
    }
    /*check for current state of fsm*/
    else if (son_nmm_get_current_state() == NMM_W_FOR_CELL_SEARCH_IND)
    {
        end = GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci );

        /*check existence of target in data base*/
        index = son_nmm_binary_search_pci(GET_FREQ_CHANNEL_PARM( \
                    g_earfcn_count, cell ), start, end, target, &flag);

        /*check for flag value*/
        if (flag == SON_TRUE)
        {
            GET_CELL_PARM(g_earfcn_count, index, rsrp) = data;
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                    "data store in DB PCI: %d rsrp: %d \n", target, data);
        }
    }
    /*current sate have some wrong value*/
    else
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                "wrong state[%d]\n", son_nmm_get_current_state());
    }

    SON_UT_TRACE_EXIT();
} /* son_nmm_search_and_fill */

/*****************************************************************************
 * Function Name  : son_nmm_earfcn_present 
 * Inputs         : earfcn
 * Outputs        : None
 * Returns        : S32 
 * Description    : search existence of earfcn in database
 ****************************************************************************/
static S32 
    son_nmm_earfcn_present
(
 U16 earfcn
 )
{
    U8      flag = SON_NULL;                     /* true or false */
    U16     index = SON_NULL;                    /*array index*/
    U16     start = SON_NULL;                    /* start of array */
    U16     end = g_count;                       /*end of array */
    S32     ret = SON_INDEX_ERR;                 /*value return by func*/


    SON_UT_TRACE_ENTER();

    /*check existence of earfcn in data base*/
    index = son_nmm_binary_search_earfcn( GET_NMM_DB_PARM(freq_channel ), \
            start, end, earfcn, &flag);
    /*check for flag value*/
    if (flag == SON_FALSE)
    {
        /*check on earfcn index*/
        if (index < SON_MAX_NO_EARFCN)
        {
            ret = index;
        }
        /*index having wrong value*/
        else
        {
            ret = SON_INDEX_ERR;
        }
    }
    /*index having wrong value*/
    else 
    {
        if (SON_NULL == earfcn)
        {
            ret = SON_NULL;
        }
        else
        {
            ret =  SON_INDEX_ERR;
        }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* son_nmm_earfcn_present */

/*****************************************************************************
 * Function Name  : son_nmm_pci_present 
 * Inputs         : pci
 * Outputs        : None
 * Returns        : Index number
 * Description    : search existence of pci in database
 ****************************************************************************/
    static S16 
    son_nmm_pci_present
(
 U16 pci
 )
{
    U8      flag  = SON_NULL;                    /*true or false*/
    U16     index = SON_NULL;                    /*index of array*/
    U16     start = SON_NULL;                    /*start of array*/
    U16     end   = g_count;                     /*end of array*/
    S16     ret   = SON_INDEX_ERR;               /*value return by func*/


    SON_UT_TRACE_ENTER();

    /*check existence of pci in data base*/
    index = son_nmm_binary_search_pci(GET_FREQ_CHANNEL_PARM(g_earfcn_count, \
                cell ), start, end, pci, &flag);

    /*check for flag value*/
    if (flag == SON_FALSE)
    {
        /*check on pci index*/
        if (index < SON_MAX_NO_PCI)
        {
            ret = index;
        }
        /*index having wrong value*/
        else
        {
            ret = SON_INDEX_ERR;
        }
    }
    /*index having wrong value*/
    else 
    {           
        if (SON_NULL == pci)
        {
            ret = SON_NULL;
        }
        else
        {
            ret = SON_INDEX_ERR;
        }
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* son_nmm_pci_present */

/*****************************************************************************
 * Function Name  : son_nmm_fill_earfcn_rssi_at_index
 * Inputs         : index
 *                  rssi
 *                  EARFCN
 * Outputs        : None
 * Returns        : NONE
 * Description    : filling earfcn_rssi value in database at specified place.
 ****************************************************************************/
static void 
    son_nmm_fill_earfcn_rssi_at_index
(
 U16 index,
 U8 rssi, 
 U16 earfcn
 )
{
    /* Coverity CID-55060 Fix Starts */
    U16     earfcn_index = SON_NULL; 


    SON_UT_TRACE_ENTER();

    /*check for
     *index having valid value*/
    /* Start: CSR 00070093 */
    if (index == g_count)
        /*End: CSR 00070093 */
    {
        if (earfcn < GET_FREQ_CHANNEL_PARM(index, earfcn ))
        {
            GET_FREQ_CHANNEL_PARM(index + SON_ONE, rssi ) = GET_FREQ_CHANNEL_PARM(index, rssi );
            GET_FREQ_CHANNEL_PARM(index + SON_ONE, earfcn ) = GET_FREQ_CHANNEL_PARM(index, earfcn );
        }

        GET_FREQ_CHANNEL_PARM(index, rssi ) =  rssi;
        GET_FREQ_CHANNEL_PARM(index, earfcn ) =  earfcn ;

    }
    /*SPR NO:10540  changes start*/

    /*index having valid value*/
    else if (SON_MAX_NO_EARFCN > earfcn_index) 
    {
        if (earfcn > GET_FREQ_CHANNEL_PARM(index, earfcn ))
        {
            index++;
        }

        earfcn_index = g_count;
        for (; \
                (earfcn_index > index)&&(earfcn_index < SON_MAX_NO_EARFCN); \
                earfcn_index --)
        {
            GET_FREQ_CHANNEL_PARM(earfcn_index, rssi ) = GET_FREQ_CHANNEL_PARM(earfcn_index - SON_ONE, rssi );
            GET_FREQ_CHANNEL_PARM(earfcn_index, earfcn ) = GET_FREQ_CHANNEL_PARM(earfcn_index - SON_ONE, earfcn );
        }

        GET_FREQ_CHANNEL_PARM(index, rssi ) = rssi;
        /*coverity fix 2.1.3*/
        if (earfcn_index < SON_MAX_NO_EARFCN)
        {
            GET_FREQ_CHANNEL_PARM(earfcn_index, earfcn ) = earfcn;

        }
        else
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                    "earfcn_index value is:%d\n", earfcn_index);
        }
    }

    /*Klocwork Fix Start*/
    /*Else part removed as it was a piece of dead code*/
    /*Klocwork Fix End*/

    /*SPR NO:10540  changes end*/

    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
            "data store in DB earfcn: %d \
            rssi: %d \n"                                       , earfcn, rssi);
    SON_UT_TRACE_EXIT();
    /* Coverity CID-55060 Fix Ends */



}   /* son_nmm_fill_earfcn_rssi_at_index */

/*****************************************************************************
 * Function Name  : son_nmm_fill_pci_rsrp_at_index
 * Inputs         : index
 *                  rsrp
 *                  pci
 * Outputs        : None
 * Returns        : None
 * Description    : filling pci_rsrp value in database at specified place
 ****************************************************************************/
static void 
    son_nmm_fill_pci_rsrp_at_index
(
 U16 index,
 U8 rsrp, 
 U16 pci
 )
{
    U16     pci_index = SON_NULL;


    SON_UT_TRACE_ENTER();

    /*check on pci 
     * index having valid value*/
    /* Start: CSR 00070093 */
    if (index == g_count)
        /*End: CSR 00070093 */
    {
        if (pci < GET_CELL_PARM(g_earfcn_count, index, cell_id ))
        {
            GET_CELL_PARM(g_earfcn_count, index + SON_ONE, rsrp ) = GET_CELL_PARM(g_earfcn_count, index, rsrp );
            GET_CELL_PARM(g_earfcn_count, index + SON_ONE, cell_id ) = GET_CELL_PARM(g_earfcn_count, index, cell_id );
            GET_CELL_PARM(g_earfcn_count, index, rsrp ) = rsrp;
            GET_CELL_PARM(g_earfcn_count, index, cell_id ) = pci;
        }
        else
        {
            GET_CELL_PARM(g_earfcn_count, index, rsrp ) = rsrp;
            GET_CELL_PARM(g_earfcn_count, index, cell_id ) = pci;
        }
    }
    /*index having valid value*/
    else if (SON_MAX_NO_PCI > pci_index)
    {
        if (pci > GET_CELL_PARM(g_earfcn_count, index, cell_id ))
        {
            index++;
        }

        pci_index = g_count;
        /* Klockwork 2.1.3 */
        for (; (pci_index > index) && (pci_index < SON_MAX_NO_PCI); pci_index--)
        {
            GET_CELL_PARM(g_earfcn_count, pci_index,
                    rsrp ) =   GET_CELL_PARM(g_earfcn_count, pci_index - SON_ONE, rsrp );
            GET_CELL_PARM(g_earfcn_count, pci_index, cell_id ) = GET_CELL_PARM(g_earfcn_count,
                    pci_index - SON_ONE,
                    cell_id );
        }
        GET_CELL_PARM(g_earfcn_count, index, rsrp ) = rsrp;
        /*coverity fix 2.1.3*/
        if (pci_index < SON_MAX_NO_PCI)
        {
            GET_CELL_PARM(g_earfcn_count, pci_index, cell_id ) = pci;
        }
        else
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                    "pci_index value is:%d\n", pci_index);
        }
    }  

    /*Klocwork Fix Start*/
    /*Else part removed as it was a piece of dead code*/
    /*Klocwork Fix End*/


    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
            "data store in DB pci: %d rsrp: %d \n", pci, rsrp);
    SON_UT_TRACE_EXIT();
} /* son_nmm_fill_pci_rsrp_at_index */

/*****************************************************************************
 * Function Name  : son_nmm_fill_data
 * Inputs         : data1
 *                  data2
 * Outputs        : None
 * Returns        : None
 * Description    : fill earfcn_rssi or pci_rsrp pair when num is zero.
 ****************************************************************************/
    static void 
    son_nmm_fill_data
(
 U8 data1, 
 U16 data2
 )
{
    S32     index = SON_NULL;                    /* array index */


    SON_UT_TRACE_ENTER();

    /*check for current state of fsm and num of earfcn in db*/
    if ((son_nmm_get_current_state() == NMM_W_FOR_RSSI_MEAS_IND) && \
            (g_count < SON_MAX_NO_EARFCN))
    {
        /*check on  existence of earfcn in database*/
        if ((index = son_nmm_earfcn_present(data2)) != SON_INDEX_ERR)
        {
            /*filling earfcn_rrsi at given index*/
            son_nmm_fill_earfcn_rssi_at_index(index, data1, data2);
            g_count++;
        }
    }
    /*check for current state of fsm and num of pci in db*/
    else if ((son_nmm_get_current_state() == NMM_W_FOR_CELL_SEARCH_IND) && \
            (g_count < SON_MAX_NO_PCI))
    {
        if ((index = son_nmm_pci_present(data2)) != SON_INDEX_ERR)
        {
            /*filling pci_rsrp at given index*/
            son_nmm_fill_pci_rsrp_at_index(index, data1, data2);
            g_count++;
        }
    }
    /*wrong value of index*/
    else if ((g_count >= SON_MAX_NO_PCI) || (g_count >= SON_MAX_NO_EARFCN))
    {
        /*check for current state of fsm*/
        if (son_nmm_get_current_state() == NMM_W_FOR_RSSI_MEAS_IND)
        {
            son_nmm_stop_rssi_meas_req();
        }
        else 
        {
            son_nmm_stop_cell_search();
        }
    }

    SON_UT_TRACE_EXIT();
} /* son_nmm_fill_data */

/***************************************************************************
 * Function Name  : son_nmm_clean_module
 * Inputs         : None
 * Outputs        : None 
 * Returns        : None 
 * Description    : This function clean nmm module 
 ****************************************************************************/
            static void
            son_nmm_clean_module
            (
             /* + SPR 17439 */
             void
             /* - SPR 17439 */
            )
{
    SON_UT_TRACE_ENTER();
    if (SON_NULL != g_son_nmm_timer_id)
    {
        son_stop_timer(g_son_nmm_timer_id);
        /* Start: CSR 00070093 */
        g_son_nmm_timer_id = SON_PNULL;
        /*End: CSR 00070093 */
    }

    /*cleaning data base and setting module at NULL state*/
    son_nmm_db_free();

    /*check for current state of fsm*/
    switch (son_nmm_get_current_state())
    {
        case NMM_W_FOR_NULL:
        {
            break;
        }

        case NMM_W_FOR_RSSI_MEAS_IND:
        {
            son_nmm_stop_rssi_meas_req();
            break;
        }

        case NMM_W_FOR_CELL_SEARCH_IND:
        {
            son_nmm_stop_cell_search();
            break;
        }

        case NMM_W_FOR_PBCH_CONFIG_CNF:
        case NMM_W_FOR_PBCH_DATA_IND:
        {
            son_nmm_stop_pbch_req();
            break;
        }

        case NMM_W_FOR_SIB1_CONFIG_CNF:
        case NMM_W_FOR_BCCH_DATA_IND_PRE_REQ:
        {
            break;
        }

        case NMM_W_FOR_BCCH_CONFIG_CNF:
        case NMM_W_FOR_BCCH_DATA_IND:
        {
            son_nmm_stop_bcch_req();
            break;
        }

        default:
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                "wrong state");
    } /* switch */

    /*cleaning L1*/
    son_nmm_clean_phy();
    son_nmm_set_current_state(NMM_W_FOR_NULL);
    son_nmm_set_errno(SON_NMM_SUCCESS);

    SON_UT_TRACE_EXIT();
    return;
} /* son_nmm_clean_module */

/***************************************************************************
 * Function Name  : son_nmm_module_data_initialization
 * Inputs         : None
 * Outputs        : None 
 * Returns        : None 
 * Description    : This function initialize nmm module data
 ****************************************************************************/
        static void
        son_nmm_module_data_initialization
        (
         /* + SPR 17439 */
         void
         /* - SPR 17439 */
        )
{
    SON_UT_TRACE_ENTER();
    g_earfcn_count = SON_NULL;        /*S_global variable used in FSM*/
    g_loop_count = SON_NULL;          /*S_global variable used in FSM*/
    g_count = SON_NULL;               /*S_global variable used in FSM*/
    g_son_flag = SON_NULL;            /*S_global variable used in FSM*/
    g_meas_bandwidth = SON_NULL;      /*S_global variable used in FSM*/
    g_sibx_presence_bitmask = SON_NULL;

    /*setting all member of g_son_nmm_timer_buf to Zero*/
    g_son_nmm_timer_id = SON_NULL;

    g_son_nmm_timer_buf.auto_delete = SON_FALSE;
    g_son_nmm_timer_buf.target_submodule = SONNMM_SUBMODULE_NMM;
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_nmm_rssi_meas_req_start
 * Inputs         : None 
 * Outputs        : None
 * Returns        : son_return_et 
 * Description    : This function handle internal nmm_rssi_meas_req.
 ****************************************************************************/
        static son_return_et
        son_nmm_rssi_meas_req_start
        (
         /* + SPR 17439 */
         void
         /* - SPR 17439 */
        )
{
    /* Coverity CID-55059 Fix Starts */
    /*obj og son_phy_rssi_meas_req_t*/
    son_phy_rssi_meas_req_t son_phy_rssi_meas_req = {SON_NULL};  
    U16     index = SON_NULL;                    /* index number */
    son_return_et ret = SON_FAILURE;             /*value return by func*/


    SON_UT_TRACE_ENTER();

    son_phy_rssi_meas_req.freq_band_indicator = \
                                                GET_NMM_DB_PARM(freq_band_id);
    son_phy_rssi_meas_req.meas_period = GET_NMM_DB_PARM(meas_period);

    /*check for g_loop_count
     *filling of son_phy_rssi_meas_req req data */
    if (g_loop_count == GET_NMM_DB_PARM(num_earfcn) / MAX_CARRIER_LIST)
    {
        for (index = SON_NULL, g_earfcn_count = \
                g_loop_count * MAX_CARRIER_LIST; \
                g_earfcn_count < GET_NMM_DB_PARM(num_earfcn); \
                g_earfcn_count++, index++)
        {
            son_phy_rssi_meas_req.earfcn[index] = \
                                                  GET_FREQ_CHANNEL_PARM(g_earfcn_count, earfcn );
        }

        son_phy_rssi_meas_req.num_earfcn = \
                                           GET_NMM_DB_PARM(num_earfcn) - g_loop_count * MAX_CARRIER_LIST;
    }
    else
    {
        for (index = SON_NULL, g_earfcn_count = \
                g_loop_count * MAX_CARRIER_LIST; index < MAX_CARRIER_LIST; \
                index++, g_earfcn_count++)
        {
            son_phy_rssi_meas_req.earfcn[index] = \
                                                  GET_FREQ_CHANNEL_PARM(g_earfcn_count, earfcn );
        }

        son_phy_rssi_meas_req.num_earfcn = MAX_CARRIER_LIST;
    }
    /* SPR 22925 Fix + */
    son_phy_rssi_meas_req.num_vendor_specific_list = SON_NULL;

    /* SPR 22925 Fix - */
    /*sending req to L1*/
    if (SON_FAILURE == \
            son_phy_send_son_phy_rssi_meas_req(&son_phy_rssi_meas_req))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                SON_ERROR, "son_phy_rssi_meas_req failed");
        son_nmm_set_errno(SON_NMM_MSG_SEND_FAILED);
        return SON_FAILURE;
    }

    /*start timer*/
    g_son_nmm_timer_id = son_start_timer(DEFAULT_DURATION, \
            &g_son_nmm_timer_buf, \
            sizeof(g_son_nmm_timer_buf), \
            SON_TRUE);

    /*setting current sate of fsm*/
    son_nmm_set_current_state(NMM_W_FOR_RSSI_MEAS_IND);
    ret = SON_SUCCESS;

    SON_UT_TRACE_EXIT();
    return ret;
    /* Coverity CID-55059 Fix Ends */
} /* son_nmm_rssi_meas_req_start */

/*****************************************************************************
 * Function Name  : son_nmm_db_initialisation
 * Inputs         : *p_start_scan_req
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed to initilise p_nmm_db
 *                  SON_SUCCESS     - Indicates successful initialisation of p_nmm_db
 * Description    : This function initialise nmm_db_t structures object
 ****************************************************************************/
static son_return_et 
    son_nmm_db_initialisation
(
 start_scan_req_t *p_start_scan_req
 )
{
    U16     earfcn_index = SON_NULL;            /*earfcn index */
    U16     pci_index = SON_NULL;               /*pci index */
    son_return_et ret = SON_FAILURE;


    SON_UT_TRACE_ENTER();
    /* Assumption:-start_scan_req_t structure always have valid database */
    do
    {
        if (((p_start_scan_req->is_meas_bandwidth_valid != SON_NULL) && \
                    (p_start_scan_req->meas_bandwidth == SON_NULL)) || \
                ((p_start_scan_req->is_meas_bandwidth_valid == SON_NULL) && \
                 (p_start_scan_req->num_earfcn == SON_NULL)) || \
                (p_start_scan_req->retry_count == SON_NULL) || \
                (p_start_scan_req->meas_period == SON_NULL) || \
                (p_start_scan_req->freq_band_id == SON_NULL) || \
                (p_start_scan_req->num_earfcn > SON_MAX_NO_EARFCN)) 
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                    "Wrong msg");
            ret = SON_FAILURE;
            break;
        }

        /*num earfcn == SON_NULL*/
        if (p_start_scan_req->num_earfcn == SON_NULL)
        {
            g_meas_bandwidth = p_start_scan_req->meas_bandwidth;
        }

        GET_NMM_DB_PARM(retry_count) = p_start_scan_req->retry_count;
        GET_NMM_DB_PARM(meas_period) = p_start_scan_req->meas_period;
        GET_NMM_DB_PARM(freq_band_id ) = p_start_scan_req->freq_band_id;
        GET_NMM_DB_PARM(num_earfcn ) =  p_start_scan_req->num_earfcn;

        /*sorting earfcn*/
        /*SPR No:10540  changes start */    
        son_nmm_quick_sort_earfcn(p_start_scan_req->earfcn_pci_info, \
                p_start_scan_req->num_earfcn);
        /*SPR No:10540  changes end */    


        for (; \
                earfcn_index < p_start_scan_req->num_earfcn; \
                earfcn_index++)
        {
            /* Start: CSR 00053250 */
            son_nmm_quick_sort(p_start_scan_req->earfcn_pci_info[earfcn_index].pci, 
                    p_start_scan_req->earfcn_pci_info[earfcn_index].num_pci);
            GET_FREQ_CHANNEL_PARM(earfcn_index, earfcn ) = \
                                                           p_start_scan_req->earfcn_pci_info[earfcn_index].earfcn;

            GET_FREQ_CHANNEL_PARM(earfcn_index, rssi) = SON_NULL;

            GET_FREQ_CHANNEL_PARM(earfcn_index, meas_bandwidth) = \
                                                                  p_start_scan_req->earfcn_pci_info[earfcn_index].meas_bandwidth_per_earfcn;

            GET_FREQ_CHANNEL_PARM(earfcn_index, num_pci) = \
                                                           p_start_scan_req->earfcn_pci_info[earfcn_index].num_pci;

            for (pci_index = SON_NULL; \
                    pci_index < p_start_scan_req->earfcn_pci_info[earfcn_index].num_pci; \
                    pci_index++)
            {
                GET_CELL_PARM(earfcn_index, pci_index, cell_id ) = \
                                                                   p_start_scan_req->earfcn_pci_info[earfcn_index].pci[pci_index];
                GET_CELL_PARM(earfcn_index, pci_index, rsrp ) = SON_NULL;
                /*End: CSR 00053250 end */
            }
        }

        ret = SON_SUCCESS;
    } while (SON_NULL); /*execute loop only one time*/

    SON_UT_TRACE_EXIT();

    return ret;
} /* son_nmm_db_initialisation */

/*+ SPR 18399*/
/*- SPR 18399*/
/***************************************************************************
 * Function Name  : son_nmm_rssi_meas_req_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed to process 
 *                                   nmm_rssi_meas_req
 *                  SON_SUCCESS     - Indicates successful process of 
 *                                   nmm_rssi_meas_req
 * Description    : This function process nmm_rssi_meas_req_event received 
 *                 from nmm_controler
 ****************************************************************************/
        /*+ SPR 18399*/
        son_return_et 
    son_nmm_rssi_meas_req_handler          
(
 void *p_msg
 )
{
    /*+ SPR 18399*/
    U8 initialise_nmm_db_flag = TRUE;
    U8 send_rssi_meas_req_flag = TRUE;
    son_phy_rssi_meas_req_t son_phy_rssi_meas_req = {SON_NULL};

    start_scan_req_t *p_start_scan_req = {SON_NULL};
    son_return_et ret = SON_FAILURE;           /*value return by func*/
    SON_UT_TRACE_ENTER();
    if (TRUE == initialise_nmm_db_flag )
    {
        p_start_scan_req = \
                           ((nmm_start_scan_data_t *)(p_msg + SON_API_HEADER_SIZE))->p_start_scan_req;

        if (SON_PNULL == (p_nmm_db = ( nmm_db_t *)son_mem_get(sizeof(nmm_db_t))))
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                    "Mem allocation fail");
            son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);
            return SON_FAILURE;
        }

        SON_MEMSET(p_nmm_db, '\0', sizeof(nmm_db_t));
    }
    do
    {
        /*initialise nmm_db */

        if (TRUE == initialise_nmm_db_flag )
        {
            son_nmm_module_data_initialization();

            if (son_nmm_db_initialisation(p_start_scan_req) == SON_FAILURE)
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                        SON_ERROR, "SON_NMM DB initialization failed");
                /* send reply msg as wrong msg formate */
                son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
                send_rssi_meas_req_flag = FALSE;
                /* set NMM_NULL state */
                ret = SON_FAILURE;
            }
        }
        if(TRUE == send_rssi_meas_req_flag)
        {
            /*- SPR 18399*/
            /*prepare NMM_RSSI_MEAS_REQ */
            g_earfcn_count = SON_NULL;
            g_loop_count = SON_NULL;
            g_count = SON_NULL;

            if (GET_NMM_DB_PARM(num_earfcn) != SON_NULL)
            {
                /*sending req to L1*/
                if (SON_FAILURE != son_nmm_rssi_meas_req_start())
                {
                    ret = SON_SUCCESS;
                }
                else
                {
                    ret = SON_FAILURE;
                }
            }
            else
            {
                son_phy_rssi_meas_req.freq_band_indicator = \
                                                            GET_NMM_DB_PARM(freq_band_id);
                son_phy_rssi_meas_req.meas_period = \
                                                    GET_NMM_DB_PARM(meas_period);
                /* SPR 22925 Fix + */
                /* SPR 22925 Fix - */

                if (SON_FAILURE == \
                        son_phy_send_son_phy_rssi_meas_req(&son_phy_rssi_meas_req))
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                            SON_ERROR, "son_phy_rssi_meas_req failed");
                    son_nmm_set_errno(SON_NMM_MSG_SEND_FAILED);
                    ret = SON_FAILURE;
                }

                /*start timer*/
                g_son_nmm_timer_id = son_start_timer(DEFAULT_DURATION, \
                        &g_son_nmm_timer_buf, \
                        sizeof(g_son_nmm_timer_buf), \
                        SON_TRUE);

                /*set current state of fsm*/
                son_nmm_set_current_state(NMM_W_FOR_RSSI_MEAS_IND);
                ret = SON_SUCCESS;
            }
        }
    } while (SON_NULL);
    SON_UT_TRACE_EXIT();
    return ret;
} /* son_nmm_rssi_meas_req_handler */

/*****************************************************************************
 * Function Name  : son_nmm_utran_si_msg_req_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_SUCCESS   - Indicates successfultate changed
 * Description    : This function handles NMM_START_UTRAN_SCAN_EV event
 ****************************************************************************/
                static son_return_et
    son_nmm_utran_si_msg_req_handler
(
 void *p_msg
 )
{
    /* Message will be sent to PHY */
    /*SPR 17777 +-*/
    (void)p_msg;
    son_nmm_utran_set_current_state(NMM_UTRAN_W_FOR_SI_MSG_IND);
    return SON_SUCCESS;
}

/*****************************************************************************
 * Function Name  : son_nmm_utran_si_msg_ind_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_SUCCESS   - Indicates successful receiving of SI 
 *                  SON_FAILURE   - Indicates creation/sending resp failed
 * Description    : This function handles SON_PHY_UTRAN_SI_MSG_IND_EV event
 ****************************************************************************/
                static son_return_et
    son_nmm_utran_si_msg_ind_handler
(
 void *p_msg
 )
{
    U16 index = SON_NULL;
    umts_sib_message_t *p_umts_sib_message = (umts_sib_message_t *)son_mem_get(sizeof(umts_sib_message_t));

    if (SON_PNULL == p_umts_sib_message)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        return SON_FAILURE;
    }
    /* Coverity CID 72755 Fix Start */
    p_umts_sib_message->presence_bitmask = SON_NULL;
    /* Coverity CID 72755 Fix End */
    start_scan_res_utran_data_t *p_start_scan_res_utran_data  =
        (start_scan_res_utran_data_t *)son_mem_get(sizeof(start_scan_res_utran_data_t));
    if (SON_PNULL == p_start_scan_res_utran_data)
    {
        /*SPR 15935 Fix Start*/
        if(SON_PNULL != p_umts_sib_message)
        {
            son_mem_free(p_umts_sib_message);
        }
        /*SPR 15935 Fix Stop*/
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        return SON_FAILURE;
    }

    /*SPR 15935 Fix Start*/
    son_bool_et          is_mem_free_reqd = SON_FALSE;
    /*SPR 15935 Fix Stop*/
    nmm_scan_res_list_t *p_scan_res_list = SON_PNULL;
    son_return_et ret = SON_FAILURE;
    p_scan_res_list = (nmm_scan_res_list_t *)son_mem_get( \
            sizeof(nmm_scan_res_list_t));
    if (SON_PNULL == p_scan_res_list)
    {
        son_mem_free(p_start_scan_res_utran_data);
        /*SPR 15935 Fix Start*/
        if(SON_PNULL != p_umts_sib_message)
        {
            son_mem_free(p_umts_sib_message);
        }
        /*SPR 15935 Fix Stop*/
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        return SON_FAILURE;
    }

    S32 payload_len = son_nmm_get_payload_len(p_msg);
    if (umts_sib_decoder(p_msg + SON_API_HEADER_SIZE + SON_PHY_INTERFACE_API_HEADER_SIZE, \
                p_umts_sib_message, 
                payload_len ))
    {
        if (p_umts_sib_message->presence_bitmask & UMTS_MIB_INFO)
        {
            p_start_scan_res_utran_data->scan_res_data.cell[0].csg_indication
                = p_umts_sib_message->umts_mib_info.mib_v690_nce.mib_v690_nce_v6b0_nce.mib_v690_nce_v6b0_nce_v860_nce.
                mib_v860ext_ies.csg_indicator;

            p_start_scan_res_utran_data->scan_res_data.cell[0].num_plmn_id 
                = p_umts_sib_message->umts_mib_info.mib_v690_nce.mib_v690ext.multiple_plmn_list_r6.
                multiple_plmn_list_r6_multiple_plmns.count;

            /* Coverity CID-63019 Fix Starts */
            for (index = SON_NULL; index < MAX_PLMN_ID_LIST_SIZE; index++)
                /* Coverity CID-63019 Fix Ends */
            {
                SON_MEMCPY(&p_start_scan_res_utran_data->scan_res_data.cell[0].plmn_id[index],
                        &p_umts_sib_message->umts_mib_info.mib_v690_nce.mib_v690ext.multiple_plmn_list_r6.multiple_plmn_list_r6_multiple_plmns.plmn_identity_with_optional_mcc_r6[index],
                        sizeof(plmn_id_t));
            }
        }   

        if ((p_umts_sib_message->presence_bitmask & UMTS_SIB_MSG_3) && 
                (p_umts_sib_message->presence_bitmask & UMTS_MIB_INFO))
        {
            for (index = SON_NULL; index < SON_CELL_ID_OCTET_SIZE; index++)
            {
                p_start_scan_res_utran_data->scan_res_data.cell[0].cell_Id[index] 
                    = p_umts_sib_message->umts_sib_type_3_info.cell_identity.data[index];
            }

            p_start_scan_res_utran_data->scan_res_data.cell[0].cell_barred  = 0;

            for (index = SON_NULL; index < SON_CSG_ID_OCTET_SIZE; index++)
            {
                p_start_scan_res_utran_data->scan_res_data.cell[0].csg_identity[index] = 0;
            }

            if (!son_nmm_umts_create_send_message(p_scan_res_list, p_start_scan_res_utran_data ))
            { 
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                        "UMTS response sending failed ");
                ret = SON_FAILURE;
                /*SPR 15935 Fix Start*/
                is_mem_free_reqd = SON_TRUE;
                /*SPR 15935 Fix Stop*/
            }
        }
        /*SPR 15935 Fix Start*/
        else
        {
            is_mem_free_reqd = SON_TRUE;
        }
        /*SPR 15935 Fix Stop*/
    }  
    /*SPR 15935 Fix Start*/
    else
    {
        is_mem_free_reqd = SON_TRUE;
    }

    if(SON_TRUE == is_mem_free_reqd)
    {
        if(SON_PNULL != p_start_scan_res_utran_data)
        {
            son_mem_free(p_start_scan_res_utran_data);
            p_start_scan_res_utran_data = SON_PNULL;
        }

        if(SON_PNULL != p_scan_res_list)
        {
            son_mem_free(p_scan_res_list);
            p_scan_res_list = SON_PNULL;
        }
    }
    /*SPR 15935 Fix Stop*/

    /*SPR 15935 Fix Start*/
    if(SON_PNULL != p_umts_sib_message)
    {
        son_mem_free(p_umts_sib_message);
    }
    /*SPR 15935 Fix Stop*/
    ret =  SON_SUCCESS;
    return ret;
} /* son_nmm_utran_si_msg_ind_handler */

/*****************************************************************************
 * Function Name  : son_nmm_rssi_meas_ind_handler 
 * Inputs         : *p_msg           
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle nmm_rssi_meas_ind event received 
 *                 from nmm_controler
 ****************************************************************************/
        son_return_et 
    son_nmm_rssi_meas_ind_handler
(
 void *p_msg
 )
{
    /* SPR 22069 Fix Start */
/*SPR 22925 + */
    son_phy_rssi_meas_ind_t *p_son_phy_rssi_meas_ind = SON_PNULL; 
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);    
    p_son_phy_rssi_meas_ind = (son_phy_rssi_meas_ind_t *)son_mem_get(sizeof(son_phy_rssi_meas_ind_t));
    if (SON_PNULL == p_son_phy_rssi_meas_ind)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation failed for p_son_phy_rssi_meas_ind");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    /*parse data received from external module*/
    /* SPR 22310 CID 115887 Fix Start */
    if (SON_FAILURE == \
        son_parse_son_phy_rssi_meas_ind(p_son_phy_rssi_meas_ind, p_msg + \
            SON_API_HEADER_SIZE + \
            CV_HDRSIZE, \
            payload_len,
            &length_read))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
             "Invalid msg received[ msg parse failed ]");
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
        son_mem_free(p_son_phy_rssi_meas_ind);
        /* SPR 22069 Fix End */
        return SON_FAILURE;
    }
    /* SPR 22310 CID 115887 Fix End */

    SON_UT_TRACE_ENTER();
    /* SPR 22069 Fix Start */

    if (GET_NMM_DB_PARM(num_earfcn ) != SON_NULL)
    {
        /*fill earfcn rssi pair in data base
         *incremet of 1 in rssi to make rssi 0 as invalid*/
        son_nmm_search_and_fill(
			    p_son_phy_rssi_meas_ind->rssi + SON_ONE,
                p_son_phy_rssi_meas_ind->earfcn);
    }
    else
    {
        /*fill earfcn rssi pair in data base
         *incremet of 1 in rssi to make rssi 0 as invalid*/
        son_nmm_fill_data(
                p_son_phy_rssi_meas_ind->rssi + SON_ONE,  
                p_son_phy_rssi_meas_ind->earfcn);
    }
    son_mem_free(p_son_phy_rssi_meas_ind);
    /* SPR 22069 Fix End */
    son_nmm_set_current_state(NMM_W_FOR_RSSI_MEAS_IND);
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
/*SPR 22925 - */
} /* son_nmm_rssi_meas_ind_handler */

/*****************************************************************************
 * Function Name  : son_nmm_stop_scan_req_handler 
 * Inputs         : *p_msg 
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle stop_scan event received from 
 *                 nmm_controler
 ****************************************************************************/
                    son_return_et
    son_nmm_stop_scan_req_handler
(
 /*unused*/ void *p_msg
 )
{
    SON_UT_TRACE_ENTER();
    /*SPR 17777 +-*/
    (void)p_msg;
    /*prepare reply massage for nmm_agent
     *send response to  nmm_agent */
    if (SON_FAILURE == son_nmm_cleanup_earfcn_pci())
    {
        son_nmm_clean_module();
        son_nmm_prepare_send_resp(SON_NMM_STOP_MSG_REC);
    }
    else
    {
        son_nmm_prepare_send_resp(SON_NMM_STOP_MSG_REC);
    }

    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_nmm_stop_scan_req_handler */

/*****************************************************************************
 * Function Name  : son_nmm_time_out_handler
 * Inputs         : timer_id
 * Outputs        : None
 * Returns        : None 
 * Description    : This function handle time_out event received from 
 *                 nmm_controler
 ****************************************************************************/
void 
son_nmm_time_out_handler
(
 son_timer_t         timer_id
 /*SPR 17777 +-*/
 )
{
    SON_UT_TRACE_ENTER();
    g_son_nmm_timer_id = timer_id;
    /*prepare reply massage for nmm_agent
     *send FATAL FAILURE to nmm_agent */
    son_nmm_prepare_send_resp(SON_NMM_TIMER_EXPIRE);
    /*clean nnm_module*/
    son_nmm_clean_module();
    SON_UT_TRACE_EXIT();
    return;
}

/*****************************************************************************
 * Function Name  : son_nmm_move_to_next_cell
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : jump to next pci for pbch req 
 ****************************************************************************/
        static son_return_et
        son_nmm_move_to_next_cell
        (
         /* + SPR 17439 */
         void
         /* - SPR 17439 */
        )
{
    son_return_et ret = SON_FAILURE;

    /*SPR_15671 Fix start*/
    /*check state before sending stop_bcch_req to phy*/
    if (NMM_W_FOR_BCCH_DATA_IND == son_nmm_get_current_state())
    {
        son_nmm_stop_bcch_req();
    }
    /*SPR_15671 Fix end*/

    g_count++;
    if (g_count < GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ))
    {
        /*sending req to L1*/
        ret = son_nmm_pbch_config_req_start();
    }
    else
    {
        /*SPR_15671 Fix start*/
        /*Move to next cell for pbch_config_req_start with valid pci*/
        do{
            g_earfcn_count++;
            if(g_earfcn_count < GET_NMM_DB_PARM(num_earfcn))
            {   
                if(SON_NULL == GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ))
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                            SON_DETAILED, "no Cell for earfcn [%d] moving to next cell", 
                            GET_FREQ_CHANNEL_PARM(g_earfcn_count, earfcn ));
                    continue;
                }
                else
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                            SON_DETAILED, "Num of Cell [%d] for earfcn [%d] moving to next cell",
                            GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ),
                            GET_FREQ_CHANNEL_PARM(g_earfcn_count, earfcn ));
                    break;
                }
            }
        }while(g_earfcn_count < GET_NMM_DB_PARM(num_earfcn)); 
        /*SPR_15671 Fix end*/

        if(g_earfcn_count < GET_NMM_DB_PARM(num_earfcn ))
        {
            g_count = SON_NULL;
            /*sending req to L1*/
            ret = son_nmm_pbch_config_req_start();
        }
        else
        {
            if (SON_FAILURE == son_nmm_cleanup_earfcn_pci())
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                        SON_ERROR, "no data for sending");
                son_nmm_set_errno(SON_PHY_NULL_DATA);
                ret = SON_FAILURE;
            }
            else
            {
                /*+ SPR 18399*/
                /*- SPR 18399*/
                /*prepare reply massage for nmm_agent */
                son_nmm_prepare_send_resp(SON_NMM_SUCCESS);
                son_nmm_set_current_state(NMM_W_FOR_NULL);
                /*clean nmm module*/
                son_nmm_clean_module();
                ret = SON_SUCCESS;
            }
        }
    }

    return ret;
} /* son_nmm_move_to_next_cell */

/*****************************************************************************
 * Function Name  : son_nmm_free_mem_sibx 
 * Inputs         : earfcn_index, pci_index 
 * Outputs        : None
 * Returns        : None
 * Description    : This function deallocate memory assing to sibx
 ****************************************************************************/
            void
            son_nmm_free_mem_sibx
            ( 
             U16 earfcn_index, 
             U16 pci_index 
            )
{
    if (GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_2_Info) != \
            SON_PNULL)
    {
        son_mem_free( \
                GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_2_Info));
        GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_2_Info) = \
                                                                    SON_PNULL;
    }

    if (GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_3_Info) != \
            SON_PNULL)
    {
        son_mem_free( \
                GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_3_Info));
        GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_3_Info) = \
                                                                    SON_PNULL;
    }

    if (GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_4_Info) != \
            SON_PNULL)
    {
        son_mem_free( \
                GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_4_Info));
        GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_4_Info) = \
                                                                    SON_PNULL;
    }

    if (GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_5_Info) != \
            SON_PNULL)
    {
        son_mem_free( \
                GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_5_Info));
        GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_5_Info) = \
                                                                    SON_PNULL;
    } 

    if (GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_6_Info) != \
            SON_PNULL)
    {
        son_mem_free( \
                GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_6_Info));
        GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_6_Info) = \
                                                                    SON_PNULL;
    } 

    if (GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_9_Info) != \
            SON_PNULL)
    { 
        son_mem_free( \
                GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_9_Info));
        GET_SIBX_PARM(earfcn_index, pci_index, p_sib_type_9_Info) = \
                                                                    SON_PNULL;
    }
} /* son_nmm_free_mem_sibx */

/* LTE_SON_KLOCWORK_WARN_13JULY_2012_START */

/*****************************************************************************
 * Function Name  : son_nmm_get_sib_memory 
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failure in memory allocation
 *                  SON_SUCCESS     - Indicates success in memory allocation
 * Description    : This function is used to allocate memory for various SIBs
 ****************************************************************************/
            static son_return_et
            son_nmm_get_sib_memory
            (
             /* + SPR 17439 */
             void
             /* - SPR 17439 */
            )
{
    son_return_et is_success = SON_SUCCESS;


    SON_UT_TRACE_ENTER();

    /* do-while is used here to avoid multiple return statements */
    do 
    {
        if (g_max_num_sib_read & SIB_TYPE_2_PRESENCE_FLAG)
        {
            GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_2_Info) = \
                                                                        (sib_type_2_Info_t *)son_mem_get(sizeof(sib_type_2_Info_t));

            if (SON_PNULL == GET_SIBX_PARM(g_earfcn_count, g_count, \
                        p_sib_type_2_Info))
            {
                is_success = SON_FAILURE;
                break;
            }

            SON_MEMSET(GET_SIBX_PARM(g_earfcn_count, g_count, 
                        p_sib_type_2_Info), SON_NULL, sizeof(sib_type_2_Info_t));
        }

        if (g_max_num_sib_read & SIB_TYPE_3_PRESENCE_FLAG)
        {
            GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_3_Info) = \
                                                                        (sib_type_3_Info_t *)son_mem_get(sizeof(sib_type_3_Info_t));

            if (SON_PNULL == \
                    GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_3_Info))
            {
                is_success = SON_FAILURE;
                break;
            }

            SON_MEMSET(GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_3_Info), \
                    SON_NULL, sizeof(sib_type_3_Info_t));
        }

        if (g_max_num_sib_read & SIB_TYPE_4_PRESENCE_FLAG)
        {
            GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_4_Info) = \
                                                                        (sib_type_4_Info_t *)son_mem_get(sizeof(sib_type_4_Info_t));

            if (SON_PNULL == \
                    GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_4_Info))
            {
                is_success = SON_FAILURE;
                break;
            }

            SON_MEMSET(GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_4_Info), \
                    SON_NULL, sizeof(sib_type_4_Info_t));
        }

        if (g_max_num_sib_read & SIB_TYPE_5_PRESENCE_FLAG)
        {
            GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_5_Info) = \
                                                                        (sib_type_5_Info_t *)son_mem_get(sizeof(sib_type_5_Info_t));

            if (SON_PNULL == \
                    GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_5_Info))
            {
                is_success = SON_FAILURE;
                break;
            }

            SON_MEMSET(GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_5_Info), \
                    SON_NULL, sizeof(sib_type_5_Info_t));
        }

        if (g_max_num_sib_read & SIB_TYPE_6_PRESENCE_FLAG)
        {
            GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_6_Info) = \
                                                                        (sib_type_6_Info_t *)son_mem_get(sizeof(sib_type_6_Info_t));

            if (SON_PNULL == \
                    GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_6_Info))
            {
                is_success = SON_FAILURE;
                break;
            }

            SON_MEMSET(GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_6_Info), \
                    SON_NULL, sizeof(sib_type_6_Info_t));
        }

        if (g_max_num_sib_read & SIB_TYPE_9_PRESENCE_FLAG)
        {
            GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_9_Info) = \
                                                                        (sib_type_9_Info_t *)son_mem_get(sizeof(sib_type_9_Info_t));

            if (SON_PNULL == \
                    GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_9_Info))
            {
                is_success = SON_FAILURE;
                break;
            }

            SON_MEMSET(GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_9_Info), \
                    SON_NULL, sizeof(sib_type_9_Info_t));
        }
    } while (0);

    SON_UT_TRACE_EXIT();
    return is_success;
} /* son_nmm_get_sib_memory */

/*****************************************************************************
 *  Function Name  : son_nmm_fill_sibs_db 
 *  Inputs         : p_sib_messages
 *  Returns        : None 
 *  Description    : This function stores the SIBs information in Database.
 * ****************************************************************************/
void
    son_nmm_fill_sibs_db
(
 sib_messages_t *p_sib_messages
 )
{
    SON_UT_TRACE_ENTER();
    if (p_sib_messages == SON_PNULL)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NULL pointer recevied  son_nmm_fill_sibs_db p_sib_messages %p \n ", p_sib_messages);
        return;
    }

    if ((p_sib_messages->presence_bitmask & SIB_TYPE_2_PRESENCE_FLAG) && \
            (g_max_num_sib_read & SIB_TYPE_2_PRESENCE_FLAG))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF, \
                "SIB_TYPE_2_RECEVIED");
        /*store the value of SIB2 in database*/
        SON_MEMCPY( \
                GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_2_Info), \
                &(p_sib_messages->sib_type_2_Info), sizeof(sib_type_2_Info_t));
        g_sibx_presence_bitmask |= SIB_TYPE_2_PRESENCE_FLAG;
        SON_MEMSET(&(p_sib_messages->sib_type_2_Info), SON_NULL, sizeof(sib_type_2_Info_t)); 
    }

    if ((p_sib_messages->presence_bitmask & SIB_TYPE_3_PRESENCE_FLAG) && \
            (g_max_num_sib_read & SIB_TYPE_3_PRESENCE_FLAG))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF, \
                "SIB_TYPE_3_RECEVIED");
        /*store the value of SIB3 in database*/
        SON_MEMCPY( \
                GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_3_Info), \
                &(p_sib_messages->sib_type_3_Info), sizeof(sib_type_3_Info_t));
        g_sibx_presence_bitmask |= SIB_TYPE_3_PRESENCE_FLAG;
        SON_MEMSET(&(p_sib_messages->sib_type_3_Info), SON_NULL, sizeof(sib_type_3_Info_t)); 
    }

    if ((p_sib_messages->presence_bitmask & SIB_TYPE_4_PRESENCE_FLAG) && \
            (g_max_num_sib_read & SIB_TYPE_4_PRESENCE_FLAG))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF, \
                "SIB_TYPE_4_RECEVIED");
        /*store the value of SIB4 in database*/
        SON_MEMCPY( \
                GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_4_Info), \
                &(p_sib_messages->sib_type_4_Info), sizeof(sib_type_4_Info_t));

        g_sibx_presence_bitmask |= SIB_TYPE_4_PRESENCE_FLAG;
        SON_MEMSET(&(p_sib_messages->sib_type_4_Info), SON_NULL, sizeof(sib_type_4_Info_t)); 
    }

    if ((p_sib_messages->presence_bitmask & SIB_TYPE_5_PRESENCE_FLAG) && \
            (g_max_num_sib_read & SIB_TYPE_5_PRESENCE_FLAG))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF, \
                "SIB_TYPE_5_RECEVIED");
        /*store the value of SIB6 in database*/
        SON_MEMCPY( \
                GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_5_Info), \
                &(p_sib_messages->sib_type_5_Info), sizeof(sib_type_5_Info_t));

        g_sibx_presence_bitmask |= SIB_TYPE_5_PRESENCE_FLAG;
        SON_MEMSET(&(p_sib_messages->sib_type_5_Info), SON_NULL, sizeof(sib_type_5_Info_t)); 
    }

    if ((p_sib_messages->presence_bitmask & SIB_TYPE_6_PRESENCE_FLAG) && \
            (g_max_num_sib_read & SIB_TYPE_6_PRESENCE_FLAG))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF, \
                "SIB_TYPE_6_RECEVIED");
        /*store the value of SIB6 in database*/
        SON_MEMCPY( \
                GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_6_Info), \
                &(p_sib_messages->sib_type_6_Info), sizeof(sib_type_6_Info_t));

        g_sibx_presence_bitmask |= SIB_TYPE_6_PRESENCE_FLAG;
        SON_MEMSET(&(p_sib_messages->sib_type_6_Info), SON_NULL, sizeof(sib_type_6_Info_t)); 
    }

    if ((p_sib_messages->presence_bitmask & SIB_TYPE_9_PRESENCE_FLAG) && \
            (g_max_num_sib_read & SIB_TYPE_9_PRESENCE_FLAG))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF, \
                "SIB_TYPE_9_RECEVIED");
        /*store the value of SIB9 in database*/
        SON_MEMCPY( \
                GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_9_Info), \
                &(p_sib_messages->sib_type_9_Info), sizeof(sib_type_9_Info_t));

        g_sibx_presence_bitmask |= SIB_TYPE_9_PRESENCE_FLAG;
        SON_MEMSET(&(p_sib_messages->sib_type_9_Info), SON_NULL, sizeof(sib_type_9_Info_t)); 
    }

    SON_UT_TRACE_EXIT();
} /* son_nmm_fill_sibs_db */

/*****************************************************************************
 * Function Name  : son_nmm_process_bcch_data 
 * Inputs         : *p_son_phy_bcch_data_ind Pointer to the bcch data received 
 *                  from PHY
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failure in data processing
 *                  SON_SUCCESS     - Indicates success in data processing
 * Description    : This function processes the bcch data received from PHY and 
 *                  stores the SIBs information in Database.
 ****************************************************************************/
        son_return_et
    son_nmm_process_bcch_data
(
 son_phy_bcch_data_ind_t *p_son_phy_bcch_data_ind
 )
{
    U8 index = SON_NULL;
    sib_messages_t *p_sib_messages = SON_PNULL;


    SON_UT_TRACE_ENTER();

    p_sib_messages = (sib_messages_t *)son_mem_get(sizeof(sib_messages_t));

    if (SON_PNULL == p_sib_messages)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");

        son_nmm_free_mem_sibx(g_earfcn_count, g_count);

        son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    for (index = SON_NULL; \
            index < p_son_phy_bcch_data_ind->num_bcch_data_list; \
            index++)
    {
        SON_MEMSET(p_sib_messages, SON_NULL, sizeof(sib_messages_t));
        /*calling sib decoder*/
        if (sib_and_si_decoder(p_son_phy_bcch_data_ind->sib_info[index].data, \
                    p_sib_messages, p_son_phy_bcch_data_ind->sib_info[index].len))
        {
            son_nmm_fill_sibs_db(p_sib_messages);
        }/*+ SPR 18399*/
        else
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED, \
                    "SIB Decoding Failed g_sibx_presence_bitmask = %04X g_max_num_sib_read = %04X\n", 
                    g_sibx_presence_bitmask, g_max_num_sib_read);
            /* SPR 18542 FIX START */
            GET_CELL_PARM(g_earfcn_count, g_count, cell_valid ) = SON_NULL;
            break;
            /* SPR 18542 FIX END */
        }
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED, \
                "SIB MASK g_sibx_presence_bitmask = %04X g_max_num_sib_read = %04X\n", 
                g_sibx_presence_bitmask, g_max_num_sib_read);
        if (!(g_sibx_presence_bitmask ^ g_max_num_sib_read))
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED, \
                    "all SIBs MASK g_sibx_presence_bitmask = %0X g_max_num_sib_read = %0X\n", 
                    g_sibx_presence_bitmask, g_max_num_sib_read);
            /*- SPR 18399*/
            break;
        }
    }

    if (index != p_son_phy_bcch_data_ind->num_bcch_data_list)
    {
        son_stop_timer(g_son_nmm_timer_id);
        /* Start: CSR 00070093 */
        g_son_nmm_timer_id = SON_PNULL;
        /*End: CSR 00070093 */
        /* Klockwork 2.1.3 */
        sib_and_si_mem_free(p_sib_messages);
        GET_CELL_PARM(g_earfcn_count, g_count, sibx_presence_flag ) = g_max_num_sib_read;
        son_nmm_move_to_next_cell();
        g_son_flag = SON_FALSE;
        g_sibx_presence_bitmask = SON_NULL; 
        g_max_num_sib_read = SON_NULL;
    }
    else
    {
        /* Klockwork 2.1.3 */
        sib_and_si_mem_free(p_sib_messages);
        g_son_flag = SON_TRUE;
        son_nmm_set_current_state(NMM_W_FOR_BCCH_DATA_IND);
    }

    /* Coverity ID 72466 Starts */
    son_mem_free(p_sib_messages);
    /* Coverity ID 72466 Ends */
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_nmm_process_bcch_data */

/*****************************************************************************
 * Function Name  : son_nmm_bcch_data_ind_handler 
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle son_nmm_bcch_data_ind_handler event 
 *                 received from PHY
 ****************************************************************************/
son_return_et
    son_nmm_bcch_data_ind_handler
(
 void *p_msg
 )
{
    /* SPR 22925 Fix + */
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);    
    son_return_et ret_val = SON_SUCCESS;


    SON_UT_TRACE_ENTER();

    son_phy_bcch_data_ind_t *p_son_phy_bcch_data_ind = \
                                                       (son_phy_bcch_data_ind_t *)son_mem_get(sizeof(son_phy_bcch_data_ind_t));
    if (SON_PNULL == p_son_phy_bcch_data_ind)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);
        return SON_FAILURE;
    }
    if (SON_FAILURE == \
            son_parse_son_phy_bcch_data_ind(p_son_phy_bcch_data_ind, p_msg + \
                SON_API_HEADER_SIZE + \
                CV_HDRSIZE, \
                payload_len, &length_read))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Invalid msg rec[ msg parse failed]");
        son_mem_free(p_son_phy_bcch_data_ind);
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
        return SON_FAILURE;
    }


    if ((g_son_flag == SON_FALSE) && (SON_FAILURE == son_nmm_get_sib_memory()))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        son_nmm_free_mem_sibx(g_earfcn_count, g_count);
        son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);
        /*SPR 15935 Fix Start*/
        son_mem_free(p_son_phy_bcch_data_ind);
        p_son_phy_bcch_data_ind = SON_PNULL;
        /*SPR 15935 Fix Stop*/
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    ret_val = son_nmm_process_bcch_data(p_son_phy_bcch_data_ind);
    son_mem_free(p_son_phy_bcch_data_ind);
    SON_UT_TRACE_EXIT();
    return ret_val;
/* SPR 22925 Fix - */
} /* son_nmm_bcch_data_ind_handler */

/* LTE_SON_KLOCWORK_WARN_13JULY_2012_END */

/*****************************************************************************
 * Function Name  : son_nmm_bcch_config_cnf_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle son_nmm_bcch_config_cnf_handler event 
 *                 received from PHY
 ****************************************************************************/
son_return_et
    son_nmm_bcch_config_cnf_handler
(
 void *p_msg
 )
{
    /* SPR 22925 Fix + */
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);


    SON_UT_TRACE_ENTER();
    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED, \
            "son_nmm_bcch_config_cnf received");

    /* SPR 22069 Fix Start */
    son_phy_bcch_config_cnf_t *p_son_phy_bcch_config_cnf = SON_PNULL; 
    p_son_phy_bcch_config_cnf = (son_phy_bcch_config_cnf_t *)son_mem_get(sizeof(son_phy_bcch_config_cnf_t));
    if (SON_PNULL == p_son_phy_bcch_config_cnf)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation failed for p_son_phy_bcch_config_cnf");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    /*parsing data recevied from external module*/

    /* SPR 22310 CID 111467 Fix Start */
    if (SON_FAILURE == \
            son_parse_son_phy_bcch_config_cnf(p_son_phy_bcch_config_cnf, p_msg + \
                SON_API_HEADER_SIZE + \
                CV_HDRSIZE, \
                payload_len, &length_read))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
               "Invalid msg rec [ msg parse failed ]");
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
        son_mem_free(p_son_phy_bcch_config_cnf);
        return SON_FAILURE;
    }
    /* SPR 22310 CID 111467 Fix End */

    son_nmm_set_current_state(NMM_W_FOR_BCCH_DATA_IND);
    son_mem_free(p_son_phy_bcch_config_cnf);
    /* SPR 22069 Fix End */
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
/* SPR 22925 Fix - */
} /* son_nmm_bcch_config_cnf_handler */

/*****************************************************************************
 * Function Name  : son_nmm_bcch_config_req_start
 * Inputs         : *p_sib_info_type1
 * Outputs        : None
 * Returns        : None 
 * Description    : This function handle son_nmm_bcch_config_req_start event 
 *                 received from internal function.
 ****************************************************************************/
static son_return_et
    son_nmm_bcch_config_req_start
(
 sib_type_1_Info_t *p_sib_info_type1
 )
{
    U8 index = SON_NULL;
    U16 si_periodicity[] = {80, 160, 320, 640, 1280, 2560, 5120};
    U8 si_window_length[] = {1, 2, 5, 10, 15, 20, 40};

    son_phy_bcch_config_req_t  son_phy_bcch_config_req = { SON_NULL }; 


    SON_UT_TRACE_ENTER();
    /*filling req for L1*/
    for (; 
            index < p_sib_info_type1->scheduling_info_list.count;
            index++)
    {
        if (p_sib_info_type1->si_window_length <= 6)
        {
            son_phy_bcch_config_req.si_periodicity_list[index] = \
                                                                 si_periodicity[p_sib_info_type1->scheduling_info_list.sheduling_info[index].si_periodicity];
        }
    }

    son_phy_bcch_config_req.num_si_periodicity_list = \
                                                      p_sib_info_type1->scheduling_info_list.count;

    if (p_sib_info_type1->si_window_length <= 6)
    {
        son_phy_bcch_config_req.si_window = si_window_length[p_sib_info_type1->si_window_length];
    }

    son_phy_bcch_config_req.retry_count = GET_NMM_DB_PARM(retry_count);
    /*+SPR 18854 FIX*/
    /*SPR 18854 FIX*/
    /* SPR 22925 Fix + */
#ifdef NMM_TDD_ENABLED
    son_phy_bcch_config_req.tdd_uldl_config       = p_sib_info_type1->tdd_config.sub_frame_assignment;
    son_phy_bcch_config_req.tdd_ssubframe_config  = p_sib_info_type1->tdd_config.special_sub_frame_pattern;
#endif
    /*SPR 18854 FIX*/
/* SPR 22925 Fix - */

    /*sending req to L1*/
    if (SON_FAILURE == \
            son_phy_send_son_phy_bcch_config_req(&son_phy_bcch_config_req))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Invalid msg rec [ msg parse failed ]");
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
        return SON_FAILURE;
    }

    son_nmm_set_current_state(NMM_W_FOR_BCCH_CONFIG_CNF);
    g_loop_count = SON_NULL;
    /*start timer*/
    g_son_nmm_timer_id = son_start_timer(DEFAULT_DURATION, \
            &g_son_nmm_timer_buf, \
            sizeof(g_son_nmm_timer_buf), \
            SON_TRUE);
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_nmm_bcch_config_req_start */

/*let zero represent sib3 and 1 represent sib4 and so on...*/

/*****************************************************************************
 * Function Name  :son_nmm_set_max_num_sib_read 
 * Inputs         : *p_sib_type_1_info
 * Outputs        : None
 * Returns        : None 
 * Description    : This function set the max number of sib, that has to be read. 
 ****************************************************************************/
        void
    son_nmm_set_max_num_sib_read
(
 sib_type_1_Info_t *sib_type_1_Info
 )
{
    U8 index_1 = SON_NULL;
    U8 index_2 = SON_NULL;


    g_max_num_sib_read = SON_NULL;
    g_max_num_sib_read |= SIB_TYPE_2_PRESENCE_FLAG;

    for (index_1 = SON_NULL; \
            index_1 < sib_type_1_Info->scheduling_info_list.count;
            index_1++)
    {
        for (index_2 = SON_NULL; \
                index_2 < sib_type_1_Info->scheduling_info_list.sheduling_info[index_1].sib_mapping_info.count;
                index_2++)
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED, \
                    "SIB MASK g_sibx_presence_bitmask = %04X g_max_num_sib_read = %04X sib_TYPE %d \n", 
                    g_sibx_presence_bitmask, g_max_num_sib_read,
                    sib_type_1_Info->scheduling_info_list.sheduling_info[index_1].sib_mapping_info.sib_type[index_2]+3);
            if (SIB_TYPE_3 ==
                    sib_type_1_Info->scheduling_info_list.sheduling_info[index_1].sib_mapping_info.sib_type[index_2])
            {
                g_max_num_sib_read |= SIB_TYPE_3_PRESENCE_FLAG;
            }

            if (SIB_TYPE_4 ==
                    sib_type_1_Info->scheduling_info_list.sheduling_info[index_1].sib_mapping_info.sib_type[index_2])
            {
                g_max_num_sib_read |= SIB_TYPE_4_PRESENCE_FLAG;
            }

            if (SIB_TYPE_5 ==
                    sib_type_1_Info->scheduling_info_list.sheduling_info[index_1].sib_mapping_info.sib_type[index_2])
            {
                g_max_num_sib_read |= SIB_TYPE_5_PRESENCE_FLAG;
            }

            if (SIB_TYPE_6 ==
                    sib_type_1_Info->scheduling_info_list.sheduling_info[index_1].sib_mapping_info.sib_type[index_2])
            {
                g_max_num_sib_read |= SIB_TYPE_6_PRESENCE_FLAG;
            }

            if (SIB_TYPE_9 ==
                    sib_type_1_Info->scheduling_info_list.sheduling_info[index_1].sib_mapping_info.sib_type[index_2])
            {
                g_max_num_sib_read |= SIB_TYPE_9_PRESENCE_FLAG;
            }

            if (!(g_max_num_sib_read ^ (
                            SIB_TYPE_2_PRESENCE_FLAG | \
                            SIB_TYPE_3_PRESENCE_FLAG | \
                            SIB_TYPE_4_PRESENCE_FLAG | \
                            SIB_TYPE_5_PRESENCE_FLAG | \
                            SIB_TYPE_6_PRESENCE_FLAG | \
                            SIB_TYPE_9_PRESENCE_FLAG)))
            {
                break;
            }
        }

        if (!(g_max_num_sib_read ^ (
                        SIB_TYPE_2_PRESENCE_FLAG | \
                        SIB_TYPE_3_PRESENCE_FLAG | \
                        SIB_TYPE_4_PRESENCE_FLAG | \
                        SIB_TYPE_5_PRESENCE_FLAG | \
                        SIB_TYPE_6_PRESENCE_FLAG | \
                        SIB_TYPE_9_PRESENCE_FLAG)))
        {
            break;
        }
    }
} /* son_nmm_set_max_num_sib_read */

/*****************************************************************************
 * Function Name  : son_nmm_bcch_data_ind_pre_req_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle son_nmm_bcch_data_ind_pre_req_handler 
 *                 event received from PHY.
 ****************************************************************************/
        son_return_et
    son_nmm_bcch_data_ind_pre_req_handler
(
 void *p_msg
 )
{
    U8 index = SON_NULL;
    son_return_et ret = SON_FAILURE;
    sib_type_1_Info_t *p_sib_type_1_Info = SON_PNULL;
    sib_messages_t *p_sib_messages = SON_PNULL;

    /* SPR 22925 Fix + */
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);    

    son_phy_bcch_data_ind_t *p_son_phy_bcch_data_ind = \
                                                       (son_phy_bcch_data_ind_t *)son_mem_get(sizeof(son_phy_bcch_data_ind_t));


    if (p_son_phy_bcch_data_ind == SON_PNULL)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);
        return SON_FAILURE;
    }

    /*parsing data recevied from external module*/
    if (SON_FAILURE == \
            son_parse_son_phy_bcch_data_ind(p_son_phy_bcch_data_ind, p_msg + \
                SON_API_HEADER_SIZE + \
                CV_HDRSIZE, \
                payload_len, &length_read))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
            "Invalid msg rec [msg parse failed]");
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
        son_mem_free(p_son_phy_bcch_data_ind);
        return SON_FAILURE;
    }





    {

    p_sib_type_1_Info =  (sib_type_1_Info_t *)son_mem_get(sizeof(sib_type_1_Info_t));
    if (SON_PNULL == p_sib_type_1_Info)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);
        son_mem_free(p_son_phy_bcch_data_ind);
        return SON_FAILURE;
    }

    SON_UT_TRACE_ENTER();

    p_sib_messages =  (sib_messages_t *)son_mem_get(sizeof(sib_messages_t));

    if (SON_PNULL == p_sib_messages)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        son_mem_free(p_sib_type_1_Info);
        son_mem_free(p_son_phy_bcch_data_ind);
        son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);
        return SON_FAILURE;
    }

    for (index = SON_NULL; \
            index < p_son_phy_bcch_data_ind->num_bcch_data_list;
            index++)
    {
        /*calling sib decoder*/
        if (sib_and_si_decoder(p_son_phy_bcch_data_ind->sib_info[index].data, \
                    p_sib_messages, p_son_phy_bcch_data_ind->sib_info[index].len))
        {
            if (p_sib_messages->presence_bitmask & SIB_TYPE_1_PRESENCE_FLAG)
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF, \
                        "SIB_TYPE_1_RECEVIED");

                son_stop_timer(g_son_nmm_timer_id);
                /* Start: CSR 00070093 */
                g_son_nmm_timer_id = SON_PNULL;
                /*End: CSR 00070093 */
                /*filling sib1 in database*/

                SON_MEMCPY( \
                        p_sib_type_1_Info, &(p_sib_messages->sib_type_1_Info), \
                        sizeof(sib_type_1_Info_t));

                GET_SIBX_PARM(g_earfcn_count, g_count, p_sib_type_1_Info) = \
                                                                            p_sib_type_1_Info;
                son_nmm_set_max_num_sib_read(p_sib_type_1_Info);
                ret = son_nmm_bcch_config_req_start(p_sib_type_1_Info);
                /* Coverity ID 72469 Starts */
                p_sib_type_1_Info = SON_PNULL;
                /* Coverity ID 72469 Starts */
                break;
            }
        }
    }

    }
    if (index == p_son_phy_bcch_data_ind->num_bcch_data_list)
    {
        son_stop_timer(g_son_nmm_timer_id);
        /* Start: CSR 00070093 */
        g_son_nmm_timer_id = SON_PNULL;            
        /*End: CSR 00070093 */
        GET_CELL_PARM(g_earfcn_count, g_count, cell_valid ) = SON_NULL;
        g_count++;
        if (g_count < GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ))
        {
            /*calling son_nmm_pbch_config_req_start to initiate new req*/
            ret = son_nmm_pbch_config_req_start();
        }
        else
        {
            g_earfcn_count++;
            if (g_earfcn_count < GET_NMM_DB_PARM(num_earfcn ))
            {
                g_count = SON_NULL;
                /*calling son_nmm_pbch_config_req_start to initiate new req*/
                ret = son_nmm_pbch_config_req_start();
            }
            else
            {
                if (SON_FAILURE == son_nmm_cleanup_earfcn_pci())
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                            SON_ERROR, "no data for sending");
                    son_nmm_set_errno(SON_PHY_NULL_DATA);
                    ret = SON_FAILURE;
                }
                else
                {
                        /*prepare reply massage for nmm_agent */ 
                    son_nmm_prepare_send_resp(SON_NMM_SUCCESS);
                        /*cleaning nmm module*/
                    son_nmm_clean_module();
                        /* Coverity ID 72470, 72469, 72468 Starts */
                    ret = SON_SUCCESS;
                        /* Coverity ID 72470, 72469, 72468 Ends */
                }
            }
        }
    }
 if(SON_PNULL != p_son_phy_bcch_data_ind)
    {
        son_mem_free(p_son_phy_bcch_data_ind);
    }

    /* Coverity ID 72470, 72469, 72468 Starts */
    if(SON_PNULL != p_sib_messages)
    {
        son_mem_free(p_sib_messages);
    }
    if(SON_PNULL != p_sib_type_1_Info)
    {
        son_mem_free(p_sib_type_1_Info);
    }
    /* Coverity ID 72470, 72469, 72468 Ends */
    SON_UT_TRACE_EXIT();
    return ret;
    /* SPR 22925 Fix - */
}

/*****************************************************************************
 * Function Name  : son_nmm_sib1_config_cnf_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle son_nmm_sib1_config_cnf_handler event 
 *                 received from PHY
 ****************************************************************************/
    son_return_et
    son_nmm_sib1_config_cnf_handler
(
 void *p_msg
 )
{
    /* SPR 22925 Fix + */
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);

    SON_UT_TRACE_ENTER();
    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED, \
            "son_nmm_sib1_config_cnf received");

    /* SPR 22069 Fix Start */
	son_phy_sib1_config_cnf_t *p_son_phy_sib1_config_cnf = SON_PNULL; 

    p_son_phy_sib1_config_cnf = (son_phy_sib1_config_cnf_t *)son_mem_get(sizeof(son_phy_sib1_config_cnf_t));
    if (SON_PNULL == p_son_phy_sib1_config_cnf)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation failed for p_son_phy_sib1_config_cnf");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }
    /*parsing data recevied from external module*/
    /* SPR 22310 CID 115890 Fix Start */
    if (SON_FAILURE == \
    		son_parse_son_phy_sib1_config_cnf(p_son_phy_sib1_config_cnf, p_msg + \
                SON_API_HEADER_SIZE + \
                CV_HDRSIZE, \
                payload_len, &length_read))

    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Invalid msg rec [msg parse failed]");
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
        son_mem_free(p_son_phy_sib1_config_cnf);
        return SON_FAILURE;
    }
    /* SPR 22310 CID 115890 Fix End */

    son_nmm_set_current_state(NMM_W_FOR_BCCH_DATA_IND_PRE_REQ);
    son_mem_free(p_son_phy_sib1_config_cnf);
    /* SPR 22069 Fix End */
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
/* SPR 22925 Fix - */
} /* son_nmm_sib1_config_cnf_handler */

/*****************************************************************************
 * Function Name  : son_nmm_sib1_config_req_start
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function handle son_nmm_sib1_config_req_start request
 *                  received from internal function
 ****************************************************************************/
static son_return_et
son_nmm_sib1_config_req_start
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 )
{
    son_phy_sib1_config_req_t son_phy_sib1_config_req = { SON_NULL };

    son_phy_sib1_config_req.retry_count = GET_NMM_DB_PARM(retry_count);
    SON_UT_TRACE_ENTER();
    /* SPR 22925 Fix + */
    /* SPR 22925 Fix - */

    /*sending req to L1*/
    if (SON_FAILURE == \
            son_phy_send_son_phy_sib1_config_req(&son_phy_sib1_config_req))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Invalid msg rec [msg parse failed]");
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
        return SON_FAILURE;
    }

    son_nmm_set_current_state(NMM_W_FOR_SIB1_CONFIG_CNF);

    /*start timer*/
    g_son_nmm_timer_id = son_start_timer(DEFAULT_DURATION, \
            &g_son_nmm_timer_buf, \
            sizeof(g_son_nmm_timer_buf), \
            SON_TRUE);
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_nmm_sib1_config_req_start */

/*****************************************************************************
 * Function Name  : son_nmm_pbch_config_req_start
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : This function handle nmm_pbch_config_req request received 
 *                  from internal function.
 ****************************************************************************/
    static son_return_et
    son_nmm_pbch_config_req_start
    (
     /* + SPR 17439 */
     void
     /* - SPR 17439 */
    )
{
    son_return_et ret = SON_FAILURE;

    son_phy_pbch_config_req_t son_phy_pbch_config_req = { SON_NULL };


    SON_UT_TRACE_ENTER();

    son_phy_pbch_config_req.pci = \
                                  GET_CELL_PARM(g_earfcn_count, g_count, cell_id );

    son_phy_pbch_config_req.earfcn = \
                                     GET_FREQ_CHANNEL_PARM(g_earfcn_count, earfcn );
    /* SPR 22925 Fix + */
    /* SPR 22925 Fix - */
    /*sending req to L1*/

    if (SON_FAILURE == \
            son_phy_send_son_phy_pbch_config_req(&son_phy_pbch_config_req))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                SON_ERROR, "son_phy_pbch_config_req send failed");
        son_nmm_set_errno(SON_NMM_MSG_SEND_FAILED);
        ret = SON_FAILURE;
    }
    else
    {
        /*start timer*/
        g_son_nmm_timer_id = son_start_timer(DEFAULT_DURATION, \
                &g_son_nmm_timer_buf, \
                sizeof(g_son_nmm_timer_buf), \
                SON_TRUE);
        son_nmm_set_current_state(NMM_W_FOR_PBCH_CONFIG_CNF);
        ret = SON_SUCCESS;
    }

    SON_UT_TRACE_EXIT();
    return ret;
} /* son_nmm_pbch_config_req_start */

/*****************************************************************************
 * Function Name  : son_nmm_pbch_data_ind_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle son_nmm_pbch_data_ind_handler event 
 *                  received from nmm_controller.
 ****************************************************************************/
son_return_et
    son_nmm_pbch_data_ind_handler
(
 void *p_msg
 )
{
    /* SPR 22925 Fix + */
    son_return_et ret = SON_FAILURE;
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);    

    /* SPR 22069 Fix Start */
    son_phy_pbch_data_ind_t *p_son_phy_pbch_data_ind = SON_PNULL;
    p_son_phy_pbch_data_ind = (son_phy_pbch_data_ind_t *)son_mem_get(sizeof(son_phy_pbch_data_ind_t));
    if (SON_PNULL == p_son_phy_pbch_data_ind)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation failed for p_son_phy_pbch_data_ind");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }


    /*SPR_14770 Fix start*/
    /* SPR 22310 CID 115902 Fix Start */
    if ((SON_FAILURE == \
                son_parse_son_phy_pbch_data_ind(p_son_phy_pbch_data_ind, p_msg + \
                SON_API_HEADER_SIZE + \
                CV_HDRSIZE, \
                payload_len, &length_read)) && (p_son_phy_pbch_data_ind->cause == MIB_FAILED))
            /*SPR_14770 Fix end*/

    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Invalid msg rec [msg parse failed]");
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
        son_mem_free(p_son_phy_pbch_data_ind);
        return SON_FAILURE;
    }
    /* SPR 22310 CID 115902 Fix End */


    SON_UT_TRACE_ENTER();

    mib_t *p_mib = SON_PNULL;
    p_mib = (mib_t *)son_mem_get(sizeof(mib_t));

    if (SON_PNULL == p_mib)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);

        son_mem_free(p_son_phy_pbch_data_ind);
        return SON_FAILURE;
    }


/*SPR_14770 Fix start*/
    if (p_son_phy_pbch_data_ind->cause == MIB_OK) 
/*SPR_14770 Fix end*/
    {
        p_mib->sfn = p_son_phy_pbch_data_ind->sfn;
        p_mib->phich_duration = p_son_phy_pbch_data_ind->phich_duration;
        p_mib->phich_resource = p_son_phy_pbch_data_ind->phich_resource;
        p_mib->dl_sys_bandwidth = p_son_phy_pbch_data_ind->dl_sys_bandwidth;
        GET_CELL_PARM(g_earfcn_count, g_count, num_antenna ) = \
                                                               p_son_phy_pbch_data_ind->num_antenna;

        GET_CELL_PARM(g_earfcn_count, g_count, p_mib ) = p_mib;
        GET_CELL_PARM(g_earfcn_count, g_count, cell_valid ) = SON_SUCCESS;

        /* coverity fix 79420 start */
        /*+ SPR 18397*/
        /* code removed*/
        /*+ SPR 18397*/
        /* coverity fix 79420 end */
        p_mib = SON_PNULL;

        son_nmm_stop_pbch_req();
        son_stop_timer(g_son_nmm_timer_id);
        /* Start: CSR 00070093 */
        g_son_nmm_timer_id = SON_PNULL;
        /*End: CSR 00070093 */
        /*start new sib1 req to phy*/
        ret = son_nmm_sib1_config_req_start();
    }
    else 
    {
        son_mem_free(p_mib);
        g_loop_count = SON_NULL;
        son_stop_timer(g_son_nmm_timer_id);
        /* Start: CSR 00070093 */
        g_son_nmm_timer_id = SON_PNULL;
        /*End: CSR 00070093 */
        GET_CELL_PARM(g_earfcn_count, g_count, cell_valid ) = \
                                                              SON_NULL;

        son_nmm_stop_pbch_req();
        g_count++;
        if (g_count < GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ))
        {
            /*start pbch req*/
            ret = son_nmm_pbch_config_req_start();
        }
        else
        {
            g_earfcn_count++;
            if (g_earfcn_count < GET_NMM_DB_PARM(num_earfcn ))
            {
                /*start pbch req*/
                g_count = SON_NULL;
                /*+ SPR 18396*/
                if(SON_NULL == GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ))
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                            SON_DETAILED, "There is no cell in earfcn [%d] so  move to next cell",
                            GET_FREQ_CHANNEL_PARM(g_earfcn_count, earfcn));

                    ret = son_nmm_move_to_next_cell();
                }
                else
                {
                    ret = son_nmm_pbch_config_req_start();
                }
                /*- SPR 18396*/
            }
            else
            {
                g_count = SON_NULL;
                g_earfcn_count = SON_NULL;
                if (SON_FAILURE == son_nmm_cleanup_earfcn_pci())
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                            SON_ERROR, "no data for sending");
                    son_nmm_set_errno(SON_PHY_NULL_DATA);
                    ret = SON_FAILURE;
                }
                else
                {
                    /*prepare reply massage for nmm_agent */
                    son_nmm_prepare_send_resp(SON_NMM_SUCCESS);
                    son_nmm_set_current_state(NMM_W_FOR_NULL);
                    son_nmm_clean_module();
                    ret = SON_SUCCESS;
                }
            }
        }
    }
    son_mem_free(p_son_phy_pbch_data_ind);
    /* SPR 22069 Fix End */
    SON_UT_TRACE_EXIT();
    return ret;
    /* SPR 22925 Fix - */
} /* son_nmm_pbch_data_ind_handler */

/*****************************************************************************
 * Function Name  : son_nmm_pbch_config_cnf_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle son_nmm_pbch_config_cnf_handler 
 *                 event received from PHY.
 ****************************************************************************/
        son_return_et
    son_nmm_pbch_config_cnf_handler
(
 void *p_msg
 )
{
    /* SPR 22925 Fix + */
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);

    SON_UT_TRACE_ENTER();
    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED, \
            "son_nmm_pbch_config_cnf recevied");

    /* SPR 22069 Fix Start */
    son_phy_pbch_config_cnf_t *p_son_phy_pbch_config_cnf = SON_PNULL; 
    p_son_phy_pbch_config_cnf = (son_phy_pbch_config_cnf_t *)son_mem_get(sizeof(son_phy_pbch_config_cnf_t));
    if (SON_PNULL == p_son_phy_pbch_config_cnf)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation failed for p_son_phy_pbch_config_cnf");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    /*parsing data recevied from external module*/
    /* SPR 22310 CID 115896 Fix Start */
    if (SON_FAILURE == \
            son_parse_son_phy_pbch_config_cnf(p_son_phy_pbch_config_cnf, p_msg + \
                SON_API_HEADER_SIZE + \
                CV_HDRSIZE, \
                payload_len, &length_read))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Invalid msg rec [msg parse failed]");
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
	son_mem_free(p_son_phy_pbch_config_cnf);
        return SON_FAILURE;
    }
    /* SPR 22310 CID 115896 Fix End */


    son_nmm_set_current_state(NMM_W_FOR_PBCH_DATA_IND);
    son_mem_free(p_son_phy_pbch_config_cnf);
/* SPR 22925 Fix - */
    /* SPR 22069 Fix End */
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_nmm_pbch_config_cnf_handler */

/*****************************************************************************
 * Function Name  : son_nmm_cell_search_cnf_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle son_nmm_cell_search_cnf_handler event 
 *                  received from PHY.
 ****************************************************************************/
son_return_et 
    son_nmm_cell_search_cnf_handler
(
 void *p_msg
 )
{
    son_return_et ret = SON_FAILURE; 
    /* SPR 22925 Fix + */
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);

    SON_UT_TRACE_ENTER();
    son_stop_timer(g_son_nmm_timer_id);
    /* Start: CSR 00070093 */
    g_son_nmm_timer_id = SON_PNULL;
    /*End: CSR 00070093 */

    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED, \
            "son_nmm_cell_search_cnf recevied");

    /* SPR 22069 Fix Start */
    son_phy_cell_search_cnf_t *p_son_phy_cell_search_cnf = SON_PNULL;
    p_son_phy_cell_search_cnf = (son_phy_cell_search_cnf_t *)son_mem_get(sizeof(son_phy_cell_search_cnf_t));
    if (SON_PNULL == p_son_phy_cell_search_cnf)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation failed for p_son_phy_cell_search_cnf");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }
    /*parsing data recevied from external module*/
    /* SPR 22310 CID 115894 Fix Start */
    if (SON_FAILURE == \
            son_parse_son_phy_cell_search_cnf(p_son_phy_cell_search_cnf, p_msg + \
                SON_API_HEADER_SIZE + \
                CV_HDRSIZE, \
                payload_len, &length_read))
    {
         SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Invalid msg rec [msg parse failed]");
         son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
	 son_mem_free(p_son_phy_cell_search_cnf);
         return SON_FAILURE;
    }
    /* SPR 22310 CID 115894 Fix End */


    if (GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ) == SON_NULL)
    {
        GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci) = g_count;
    }

    g_earfcn_count++;
    /*check for g_earfcn_count*/
    if (g_earfcn_count < GET_NMM_DB_PARM(num_earfcn ))
    {
        g_count = SON_NULL;
        ret = son_nmm_cell_search_req_start();
    } 
    else
    {
        g_earfcn_count = SON_NULL;
        g_count = SON_NULL;
        if (SON_FAILURE == son_nmm_cleanup_earfcn_pci())
        {
            GET_NMM_DB_PARM(num_earfcn ) = SON_NULL;
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                    SON_ERROR, "no data for sending");
            son_nmm_set_errno(SON_PHY_NULL_DATA);
            ret = SON_FAILURE;
        }
        else
        {
            /*SPR_15671 Fix start*/
            /* check if num of pci in given earfcn is 0 then 
             * further processing of that earfcn is not required instead move to next cell */
            if(SON_NULL == GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ))
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                        SON_DETAILED, "There is no cell in earfcn [%d] so  move to next cell",
                        GET_FREQ_CHANNEL_PARM(g_earfcn_count, earfcn));

                ret = son_nmm_move_to_next_cell();
            }
            else
            {
                ret = son_nmm_pbch_config_req_start();
            }
            /*SPR_15671 Fix end*/
        }
    }
    son_mem_free(p_son_phy_cell_search_cnf);
/* SPR 22925 Fix + */
    /* SPR 22069 Fix End */
    SON_UT_TRACE_EXIT();
    return ret;
} /* son_nmm_cell_search_cnf_handler */

/*****************************************************************************
 * Function Name  : son_nmm_cell_search_ind_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle son_nmm_cell_search_ind_handler event 
 *                  received from PHY 
 ****************************************************************************/
son_return_et 
    son_nmm_cell_search_ind_handler
(
 void *p_msg
 )
{
    /* SPR 22925 Fix + */
    U8      index = SON_NULL;                     /*array index*/
    /* SPR 22069 Fix Start */
    son_phy_cell_search_ind_t *p_son_phy_cell_search_ind = SON_PNULL;
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);    
    p_son_phy_cell_search_ind = (son_phy_cell_search_ind_t *)son_mem_get(sizeof(son_phy_cell_search_ind_t));
    if (SON_PNULL == p_son_phy_cell_search_ind)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation failed for p_son_phy_cell_search_ind");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }


if (SON_FAILURE == \
            son_parse_son_phy_cell_search_ind(p_son_phy_cell_search_ind, p_msg + \
                    SON_API_HEADER_SIZE + \
                    CV_HDRSIZE, \
                    payload_len, &length_read))
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                    "Invalid msg rec [msg parse failed]");
            son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
	    son_mem_free(p_son_phy_cell_search_ind);
            return SON_FAILURE;
        }

    SON_UT_TRACE_ENTER();

    /*storing pci_rssp pair in database*/

    if (GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ) != SON_NULL)
    {
        for (; 
                index < p_son_phy_cell_search_ind->num_pci; 
                index++)
        {
            /*incremet of 1 in rsrp to make rsrp 0 as invalid*/
            son_nmm_search_and_fill( \
                    p_son_phy_cell_search_ind->cell_found_list[index].rsrp + SON_ONE, 
                    p_son_phy_cell_search_ind->cell_found_list[index].pci);
        }
    }
    else
    {
        for (; index < p_son_phy_cell_search_ind->num_pci; index++)
        {
            /*incremet of 1 in rsrp to make rsrp 0 as invalid*/
            son_nmm_fill_data( \
                    p_son_phy_cell_search_ind->cell_found_list[index].rsrp + SON_ONE, 
                    p_son_phy_cell_search_ind->cell_found_list[index].pci);
        }
    }
    son_nmm_set_current_state(NMM_W_FOR_CELL_SEARCH_IND);
    son_mem_free(p_son_phy_cell_search_ind);
/* SPR 22925 Fix - */
    /* SPR 22069 Fix End */
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_nmm_cell_search_ind_handler */

/*****************************************************************************
 * Function Name  : son_nmm_cell_search_req_start
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_return_et
 * Description    : This function process son_nmm_cell_search_req_start req
 ****************************************************************************/
static son_return_et
son_nmm_cell_search_req_start
(
 /* + SPR 17439 */
 void
 /* - SPR 17439 */
 )
{
    son_return_et ret = SON_SUCCESS;

    son_phy_cell_search_req_t son_phy_cell_search_req = { SON_NULL };

    U8 index = SON_NULL;


    SON_UT_TRACE_ENTER();

    /*is num pci == 0*/
    if (GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ) != SON_NULL)
    {
        /*making req for L1*/
        for (index = SON_NULL; 
                index < GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ); \
                index++)
        {
            son_phy_cell_search_req.pci_list[index] = \
                                                      GET_CELL_PARM(g_earfcn_count, index, cell_id );
        }
    }                                        /*end of num pci == 0*/

    if (SON_NULL == GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci ))
    {
        son_phy_cell_search_req.exhaustive_search = 1;    
    }

    son_phy_cell_search_req.earfcn = \
                                     GET_FREQ_CHANNEL_PARM(g_earfcn_count, earfcn );
    son_phy_cell_search_req.num_pci_list = \
                                           GET_FREQ_CHANNEL_PARM(g_earfcn_count, num_pci );
    son_phy_cell_search_req.meas_bandwidth = \
                                             GET_FREQ_CHANNEL_PARM(g_earfcn_count, meas_bandwidth);
    son_phy_cell_search_req.meas_period = \
                                          GET_NMM_DB_PARM(meas_period);
    /* SPR 22925 Fix + */
    son_phy_cell_search_req.num_vendor_specific_list = 0;
    /* SPR 22925 Fix - */
    /*sending req to L1*/ 
    if (SON_FAILURE == \
            son_phy_send_son_phy_cell_search_req(&son_phy_cell_search_req))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                SON_ERROR, "son_phy_cell_search_req failed");
        son_nmm_set_errno(SON_NMM_MSG_SEND_FAILED);
        ret = SON_FAILURE;
    }

    g_son_nmm_timer_id = son_start_timer(DEFAULT_DURATION, \
            &g_son_nmm_timer_buf, \
            sizeof(g_son_nmm_timer_buf), \
            SON_TRUE);

    son_nmm_set_current_state(NMM_W_FOR_CELL_SEARCH_IND);
    SON_UT_TRACE_EXIT();
    return ret;
} /* son_nmm_cell_search_req_start */

/*****************************************************************************
 * Function Name  : son_nmm_rssi_meas_cnf_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function handle nmm_rssi_meas_cnf event received 
 *                 from PHY
 ****************************************************************************/
    son_return_et 
    son_nmm_rssi_meas_cnf_handler
(
 void *p_msg
 )
{
    /* SPR 22925 Fix + */
    U16 index = SON_NULL;                    /*array index*/ 
    son_return_et  ret = SON_FAILURE; 
    S32 length_read = SON_NULL;
    S32 payload_len = son_nmm_get_payload_len(p_msg);

    /*for testing only gautam*/

    SON_UT_TRACE_ENTER();
    /*stop timer*/
    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED, \
            "son_nmm_rssi_meas_cnf recevied");

    /* SPR 22069 Fix Start */
    son_phy_rssi_meas_cnf_t *p_son_phy_rssi_meas_cnf = SON_PNULL;
    p_son_phy_rssi_meas_cnf = (son_phy_rssi_meas_cnf_t *)son_mem_get(sizeof(son_phy_rssi_meas_cnf_t));
    if (SON_PNULL == p_son_phy_rssi_meas_cnf)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation failed for p_son_phy_rssi_meas_cnf");
        SON_UT_TRACE_EXIT();
        return SON_FAILURE;
    }

    /*parsing data recevied from external module*/

    /* SPR 22310 CID 115886 Fix Start */
    if (SON_FAILURE == \
            son_parse_son_phy_rssi_meas_cnf(p_son_phy_rssi_meas_cnf, p_msg + \
                SON_API_HEADER_SIZE + \
                CV_HDRSIZE, \
                payload_len, &length_read))

    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Invalid msg rec [msg parse failed]");
        son_nmm_set_errno(SON_NMM_INVALID_MSG_REC);
        son_mem_free(p_son_phy_rssi_meas_cnf);
        return SON_FAILURE;
    }
    /* SPR 22310 CID 115886 Fix End */

    /*storing pci_rssp pair in database*/

    son_stop_timer(g_son_nmm_timer_id);
    /* Start: CSR 00070093 */
    g_son_nmm_timer_id = SON_PNULL;
    /*End: CSR 00070093 */
    /*check  num_earfcn == 0*/
    if (GET_NMM_DB_PARM(num_earfcn ) != SON_NULL)
    {
        g_loop_count++;
        /*to find rssi for all earfcn at MAX 750*/
        if (((GET_NMM_DB_PARM(num_earfcn ) % MAX_CARRIER_LIST !=  SON_NULL) && \
                    (GET_NMM_DB_PARM(num_earfcn ) / MAX_CARRIER_LIST == g_loop_count)) || \
                g_loop_count < GET_NMM_DB_PARM(num_earfcn ) / MAX_CARRIER_LIST)
        {
            ret = son_nmm_rssi_meas_req_start();
        }
        else
        {
            g_earfcn_count = SON_NULL;
            g_loop_count = SON_NULL;
            g_count = SON_NULL;

            if (SON_FAILURE == son_nmm_cleanup_earfcn())
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                        SON_ERROR, "no data for sending");
                son_nmm_set_errno(SON_PHY_NULL_DATA);
                ret = SON_FAILURE;
            }
            else
            {
                /*start nmm cell search req*/
                ret = son_nmm_cell_search_req_start();
            }
        }
    }                       /* end of check num_earfcn == 0*/
    else
    {
        GET_NMM_DB_PARM(num_earfcn ) = g_count;
        g_earfcn_count = SON_NULL;
        g_loop_count = SON_NULL;
        g_count = SON_NULL;

        for (index = SON_NULL; \
                index < GET_NMM_DB_PARM(num_earfcn ); \
                index++)
        {
            GET_FREQ_CHANNEL_PARM(index, meas_bandwidth) = g_meas_bandwidth;
        }

        if (SON_FAILURE == son_nmm_cleanup_earfcn())
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, \
                    SON_ERROR, "no data for sending");
            son_nmm_set_errno(SON_PHY_NULL_DATA);
            ret = SON_FAILURE;
        }
        else
        {
            /*start nmm cell search req*/
            ret = son_nmm_cell_search_req_start();
        }
    }            
    son_mem_free(p_son_phy_rssi_meas_cnf);
    /* SPR 22069 Fix End */
    SON_UT_TRACE_EXIT();
    return ret;
    /* SPR 22925 Fix - */
} /* son_nmm_rssi_meas_cnf_handler */

/*****************************************************************************
 * Function Name  : son_nmm_fsm_process_msg 
 * Inputs         : *p_msg
 *                  p_nmm_gl_ctx
 * Outputs        : None
 * Returns        : None 
 * Description    : This function handle all nmm_fsm msg  received from PHY
 ****************************************************************************/
    void
    son_nmm_fsm_process_msg
    (
     void *p_msg 
     /*SPR 17777 +-*/
    )
{
    son_return_et ret = SON_FAILURE;
    STACKAPIHDR cspl_hdr = {0};


    son_parse_cspl_header((U8 *)p_msg, &cspl_hdr);
    U16 nmm_fsm_msg_id = cspl_hdr.api;
    U16 nmm_utran_event_id = SON_NULL;
    SON_UT_TRACE_ENTER();

    /*+ SPR 18399*/
    /*- SPR 18399*/
    if (SONNMM_START_SCAN_REQ == (son_procedure_code_et)nmm_fsm_msg_id)
    {
        start_scan_req_t *p_start_scan_req = \
                                             ((nmm_start_scan_data_t *)(p_msg + SON_API_HEADER_SIZE))->p_start_scan_req;
        g_scan_type = p_start_scan_req->rat_type;
    }    

    if (SON_EUTRAN_CELL == g_scan_type)
    {
                    if (((phy_nmm_msg_id_e)nmm_fsm_msg_id >= SON_PHY_RSSI_MEAS_IND) &&
                            ((phy_nmm_msg_id_e)nmm_fsm_msg_id <= SON_PHY_STOP_BCCH_REQ))
                    {
                        /*getting index regarding api id received*/
                        son_phy_get_api_id(&nmm_fsm_msg_id);
                        if ((nmm_fsm_event_e)nmm_fsm_msg_id > MAX_NUM_EVENT)
                        {
                            nmm_fsm_msg_id = (U16)MAX_NUM_EVENT;
                        }
                    }

                    else if (((son_procedure_code_et)nmm_fsm_msg_id >= SONNMM_START_SCAN_REQ) && \
                            ((son_procedure_code_et)nmm_fsm_msg_id <= SONNMM_STOP_SCAN_REQ))
                    {
                        nmm_fsm_msg_id = nmm_fsm_msg_id - (U16)SONNMM_START_SCAN_REQ;
                    }
                    else
                    {
                        /*SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,\
                         *      "wrong msg");*/

                        nmm_fsm_msg_id = (U16)MAX_NUM_EVENT; 
                    }  
        /*calling fsm handler functions*/ 
        if (nmm_fsm_msg_id != (U16)(MAX_NUM_EVENT + 1))
        {
            nmm_state_et nmm_current_state = son_nmm_get_current_state(); 
            ret = nmm_fsm_func_name[nmm_current_state] \
                  [nmm_fsm_msg_id](p_msg);
        }
    }
    else if (SON_UTRAN_CELL == g_scan_type)
    {
        switch (nmm_fsm_msg_id)
        {
            case (U16)SONNMM_START_SCAN_REQ:
            {
                nmm_utran_event_id = NMM_START_UTRAN_SCAN_EV;
                break;
            }

            case (U16)SON_PHY_UTRAN_SI_MSG_IND:
            {
                nmm_utran_event_id = SON_PHY_UTRAN_SI_MSG_IND_EV;
                break;
            }

            default:
            {
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_WARNING, \
                        "Unexpected API received");
                break;
            }         
        } /* switch */

        ret = nmm_utran_fsm[son_nmm_utran_get_current_state()] \
              [nmm_utran_event_id](p_msg);
    }          
    else
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_WARNING, \
                "Unexpected API received for network g_scan_type = %d", g_scan_type);
        SON_UT_TRACE_EXIT();
        return;
    }

    if ((ret == SON_FAILURE) && \
            ((son_nmm_get_errno() == SON_NMM_MEM_ALLOC_FAILURE) || \
             (son_nmm_get_errno() == SON_NMM_MSG_SEND_FAILED) || \
             (son_nmm_get_errno() == SON_NMM_INVALID_MSG_REC) || \
             ((SON_NMM_PHY_NOT_PARAM <= son_nmm_get_errno()) && \
              (SON_NMM_PHY_HEAP_ALLOC_ERROR >= son_nmm_get_errno())) || \
             ((SON_NMM_PHY_NOT_ENABLED <= son_nmm_get_errno()) && \
              (SON_NMM_PHY_LTE_IN_USE >= son_nmm_get_errno())) || \
             (son_nmm_get_errno() == SON_PHY_NULL_DATA)))
    {
        son_nmm_prepare_send_resp(son_nmm_get_errno());
        son_nmm_clean_module();
    }
    else if (ret == SON_FAILURE)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "wrong pair of return errno set");
    }

    SON_UT_TRACE_EXIT();
    return;
} /* son_nmm_fsm_process_msg */

/*****************************************************************************
 * Function Name  : son_nmm_default_action
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE     - Indicates failed message processing
 *                  SON_SUCCESS     - Indicates successful message processing
 * Description    : This function performs default action for nmm. 
 ****************************************************************************/
    son_return_et
    son_nmm_default_action
    ( 
     void *p_msg
    )
{
    U32 son_nmm_errno = SON_NULL;
    STACKAPIHDR cspl_hdr = {0};


    son_parse_cspl_header((U8 *)p_msg, &cspl_hdr);
    U16 nmm_fsm_msg_id = cspl_hdr.api;
    nmm_state_et nmm_state = son_nmm_get_current_state();

    SON_UT_TRACE_ENTER();

    if (((son_module_id_et)cspl_hdr.from) == SON_PHY_MODULE_ID)
    {
    }
    else
    {
        SON_LOG(g_nmm_log_enable,
                p_son_nmm_facility_name,
                SON_ERROR,
                " son_nmm_default_action cspl_hdr.from != SON_PHY_MODULE_ID wrong msg");
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Error received from PHY %u current state %u , Api Id %u \n", son_nmm_errno, nmm_state, nmm_fsm_msg_id );
    }

    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_nmm_default_action */

/*****************************************************************************
 * Function Name  : son_nmm_stop_rssi_meas_req
 * Inputs         : None
 * Outputs        : None
 * Returns        : None 
 * Description    : This function send a stop req to L1
 ****************************************************************************/
                     static void
                     son_nmm_stop_rssi_meas_req
                     (
                      /* + SPR 17439 */
                      void
                      /* - SPR 17439 */
                     )
{
    /* SPR 22925 Fix + */
    /* SPR 21438 Fix Start */
    son_phy_stop_rssi_meas_req_t son_phy_stop_rssi_meas_req ;
    SON_MEMSET(&son_phy_stop_rssi_meas_req, SON_NULL, sizeof(son_phy_stop_rssi_meas_req_t));
    /* SPR 21438 Fix end */


    SON_UT_TRACE_ENTER();
    /*sending stop req to L1*/
    son_phy_stop_rssi_meas_req.num_vendor_specific_list = 0;
    son_phy_send_son_phy_stop_rssi_meas_req(&son_phy_stop_rssi_meas_req);

    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_WARNING, \
                  "son_nmm_stop_rssi_meas_req successfully sended");
    SON_UT_TRACE_EXIT();
    /* SPR 22925 Fix - */
}

/*****************************************************************************
 * Function Name  : son_nmm_stop_cell_search
 * Inputs         : None
 * Outputs        : None
 * Returns        : None 
 * Description    : This function send a stop req to L1
 ****************************************************************************/
            static  void
            son_nmm_stop_cell_search
            (
             /* + SPR 17439 */
             void
             /* - SPR 17439 */
            )
{
    son_phy_stop_cell_search_req_t son_phy_stop_cell_search_req = { SON_NULL };


    SON_UT_TRACE_ENTER();
    /*sending stop req to L1*/
    /* SPR 22925 Fix + */
    son_phy_stop_cell_search_req.num_vendor_specific_list = 0;
    /* SPR 22925 Fix - */
    son_phy_send_son_phy_stop_cell_search_req(&son_phy_stop_cell_search_req);
    SON_UT_TRACE_EXIT();
}
/*****************************************************************************
 * Function Name  : son_nmm_stop_pbch_req
 * Inputs         : None
 * Outputs        : None
 * Returns        : None 
 * Description    : This function send a stop req to L1
 ****************************************************************************/
    static void
    son_nmm_stop_pbch_req
    (
     /* + SPR 17439 */
     void
     /* - SPR 17439 */
    )
{
    son_phy_stop_pbch_req_t  son_phy_stop_pbch_req = { SON_NULL }; 


    SON_UT_TRACE_ENTER();
    /*sending stop req to L1*/
    /* SPR 22925 Fix + */
    son_phy_stop_pbch_req.num_vendor_specific_list = 0;
    /* SPR 22925 Fix - */
    son_phy_send_son_phy_stop_pbch_req(&son_phy_stop_pbch_req);
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_nmm_stop_bcch_req 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None 
 * Description    : This function send a stop req to L1
 ****************************************************************************/
            static void
            son_nmm_stop_bcch_req
            (
             /* + SPR 17439 */
             void
             /* - SPR 17439 */
            )
{
    son_phy_stop_bcch_req_t  son_phy_stop_bcch_req = { SON_NULL };


    SON_UT_TRACE_ENTER();
    /*sending stop req to L1*/
    /* SPR 22925 Fix + */
    son_phy_stop_bcch_req.num_vendor_specific_list = 0;
    /* SPR 22925 Fix - */
    son_phy_send_son_phy_stop_bcch_req(&son_phy_stop_bcch_req);

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_nmm_fill_sib1_resp
 * Inputs         : p_eutran_cell
 *                 p_sib_type_1_Info
 * Outputs        : None
 * Returns        : None 
 * Description    : This function fills the sib1 response info.
 ****************************************************************************/
void
    son_nmm_fill_sib1_resp
(
 eutran_cell_num_t *p_eutran_cell,
 sib_type_1_Info_t *p_sib_type_1_Info
 )
{
    SON_UT_TRACE_ENTER();
    if ((SON_PNULL == p_eutran_cell) || (SON_PNULL == p_sib_type_1_Info))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NULL pointer recevied p_eutran_cell %p, p_sib_type_1_Info %p \n ", p_eutran_cell, p_sib_type_1_Info);
        return;
    }

    SON_MEMCPY( \
            p_eutran_cell->cell_Id, \
            p_sib_type_1_Info->cell_access_related_info.cell_Id, \
            CELL_ID_OCTET_SIZE);

    p_eutran_cell->cell_barred = \
                                 p_sib_type_1_Info->cell_access_related_info.cell_barred;

    p_eutran_cell->csg_indication = \
                                    p_sib_type_1_Info->cell_access_related_info.csg_indication;
    if (p_sib_type_1_Info->cell_access_related_info.presence_bitmask & \
            CELL_ACCESS_INFO_CSG_ID_PRESENCE_FLAG)
    {
        SON_MEMCPY( \
                p_eutran_cell->csg_identity, \
                p_sib_type_1_Info->cell_access_related_info.csg_identity, \
                CSG_ID_OCTET_SIZE);
        p_eutran_cell->presence_bitmask |= CELL_ACCESS_INFO_CSG_ID_PRES_FLAG;
    }

    p_eutran_cell->num_plmn_id = \
                                 p_sib_type_1_Info->cell_access_related_info.plmn_Id_info_list.count;

    SON_MEMCPY( \
            p_eutran_cell->tac, \
            p_sib_type_1_Info->cell_access_related_info.tac, \
            TAC_OCTET_SIZE);
    SON_UT_TRACE_EXIT();
} /* son_nmm_fill_sib1_resp */

/*****************************************************************************
 * Function Name  : son_nmm_fill_sib2_resp
 * Inputs         : p_eutran_cell
 *                 p_sib_type_2_Info
 * Outputs        : None
 * Returns        : None 
 * Description    : This function fills the sib2 response info.
 ****************************************************************************/
void
    son_nmm_fill_sib2_resp
(
 eutran_cell_num_t *p_eutran_cell,
 sib_type_2_Info_t *p_sib_type_2_Info
 )
{
    /*SPR_14772 Fix start*/
    son_u16 ul_sys_bw_array[SON_MAX_RBS]={6,15,25,50,75,100};
    /*SPR_14772 Fix end*/

    SON_UT_TRACE_ENTER();
    if ((SON_PNULL == p_eutran_cell) || (SON_PNULL == p_sib_type_2_Info))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NULL pointer recevied p_eutran_cell %p, p_sib_type_2_Info %p \n ", p_eutran_cell, p_sib_type_2_Info);
        return;
    }

    p_eutran_cell->reference_signal_power = \
                                            p_sib_type_2_Info->radio_resource_config_common_sib.pdsch_config_common.ref_signal_power;

    if (FREQ_INFO_UL_BW_PRESENCE_FLAG & p_sib_type_2_Info->freq_info.presence_bitmask)
    {
        p_eutran_cell->presence_bitmask |= FREQ_INFO_UL_BW_PRES_FLAG;

        /*SPR_14772 Fix start*/
        /*Change sib2info ul band widh is the index for the Bandwith*/
        p_eutran_cell->ul_sys_bw = \
                                   ul_sys_bw_array[p_sib_type_2_Info->freq_info.ul_bandwidth];
        /*SPR_14772 Fix end*/

    }

    SON_UT_TRACE_EXIT();
} /* son_nmm_fill_sib2_resp */

/*****************************************************************************
 * Function Name  : son_nmm_fill_sib3_resp
 * Inputs         : p_eutran_cell
 *                 p_sib_type_3_Info
 * Outputs        : None
 * Returns        : None 
 * Description    : This function fills the sib3 response info.
 ****************************************************************************/
    void
    son_nmm_fill_sib3_resp
(
 eutran_cell_num_t *p_eutran_cell,
 sib_type_3_Info_t *p_sib_type_3_Info
 )
{
    SON_UT_TRACE_ENTER();
    if ((SON_PNULL == p_eutran_cell) || (SON_PNULL == p_sib_type_3_Info))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NULL pointer recevied p_eutran_cell %p, p_sib_type_3_Info %p \n ", p_eutran_cell, p_sib_type_3_Info);
        return;
    }

    if (INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRESENCE_FLAG &
            p_sib_type_3_Info->intra_freq_cell_reselect_info.presence_bitmask)
    {
        p_eutran_cell->presence_bitmask |= INTRA_FREQ_CELL_RESELECT_MEAS_BW_PRES_FLAG;
        p_eutran_cell->allowed_meas_bandwidth = \
                                                /* SPR 9781 Start */
                                                p_sib_type_3_Info->intra_freq_cell_reselect_info.measure_bw;
        /* SPR 9781 End */
    }

    p_eutran_cell->presence_antenna_port = \
                                           (son_bool_et)p_sib_type_3_Info->intra_freq_cell_reselect_info.presence_antenna_port_1; 
    SON_UT_TRACE_EXIT();
} /* son_nmm_fill_sib3_resp */

/*****************************************************************************
 * Function Name  : son_nmm_fill_sib4_resp
 * Inputs         : p_eutran_cell
 *                 p_sib_type_4_Info
 * Outputs        : None
 * Returns        : None 
 * Description    : This function fills the sib4 response info.
 ****************************************************************************/
    void
    son_nmm_fill_sib4_resp
(
 eutran_cell_num_t *p_eutran_cell,
 sib_type_4_Info_t *p_sib_type_4_Info
 )
{
    SON_UT_TRACE_ENTER();
    U8 num_intra_freq_neighbor = SON_NULL; /*nuber of intra fre neig*/
    if ((SON_PNULL == p_eutran_cell) || (SON_PNULL == p_sib_type_4_Info))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NULL pointer recevied p_eutran_cell %p, p_sib_type_4_Info %p \n ", p_eutran_cell, p_sib_type_4_Info);
        return;
    }

    if (p_sib_type_4_Info->presence_bitmask & \
            SIB_4_NEIGH_CELL_LIST_PRESENCE_FLAG)
    {
        p_eutran_cell->num_intra_freq_neighbor = \
                                                 p_sib_type_4_Info->intra_freq_neigh_cell_list.count;

        for (num_intra_freq_neighbor = 0;
                num_intra_freq_neighbor < p_eutran_cell->num_intra_freq_neighbor;
                num_intra_freq_neighbor++)
        {
            p_eutran_cell->intra_freq_neighbor_list[num_intra_freq_neighbor].pci = \
                                                                                   p_sib_type_4_Info->intra_freq_neigh_cell_list.neigh_cell[num_intra_freq_neighbor].phys_cell_id;

            p_eutran_cell->intra_freq_neighbor_list[num_intra_freq_neighbor].cell_specific_offset = \
                                                                                                    (son_q_offset_range_et) p_sib_type_4_Info->intra_freq_neigh_cell_list.neigh_cell[num_intra_freq_neighbor
                                                                                                    ].q_offset_cell;
        }
    }
    else
    {
        p_eutran_cell->intra_freq_neighbor_list[num_intra_freq_neighbor].pci = SON_NULL;
    }

    SON_UT_TRACE_EXIT();
} /* son_nmm_fill_sib4_resp */

/*****************************************************************************
 * Function Name  : son_nmm_fill_sib5_resp
 * Inputs         : p_eutran_cell
 *                 p_sib_type_5_Info
 * Outputs        : None
 * Returns        : None 
 * Description    : This function fills the sib5 response info.
 ****************************************************************************/
    void
    son_nmm_fill_sib5_resp
(
 eutran_cell_num_t *p_eutran_cell,
 sib_type_5_Info_t *p_sib_type_5_Info
 )
{
    SON_UT_TRACE_ENTER();
    if ((SON_PNULL == p_eutran_cell) || (SON_PNULL == p_sib_type_5_Info))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NULL pointer recevied p_eutran_cell %p, p_sib_type_5_Info %p \n ", p_eutran_cell, p_sib_type_5_Info);
        return;
    }

    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_nmm_fill_sib6_resp
 * Inputs         : p_eutran_cell
 *                 p_sib_type_6_Info
 * Outputs        : None
 * Returns        : None 
 * Description    : This function fills the sib6 response info.
 ****************************************************************************/
void
    son_nmm_fill_sib6_resp
(
 eutran_cell_num_t *p_eutran_cell,
 sib_type_6_Info_t *p_sib_type_6_Info
 )
{ 
    SON_UT_TRACE_ENTER();
    U8  arr_index = SON_NULL;
    if ((SON_PNULL == p_eutran_cell) || (SON_PNULL == p_sib_type_6_Info))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NULL pointer recevied p_eutran_cell %p, p_sib_type_6_Info %p \n ", p_eutran_cell, p_sib_type_6_Info);
        return;
    }

    if (p_sib_type_6_Info->presence_bitmask & \
            CARRIER_FREQ_LIST_UTRA_FDD)
    {
        p_eutran_cell->num_utran_fdd_carrier_freq = p_sib_type_6_Info->carrier_freq_list_UTRA_FDD.count;

        for (arr_index = SON_NULL; \
                arr_index < p_eutran_cell->num_utran_fdd_carrier_freq; \
                arr_index++)
        {
            SON_MEMCPY(p_eutran_cell->carrier_freq_utran_fdd_data + arr_index,
                    p_sib_type_6_Info->carrier_freq_list_UTRA_FDD.carrier_freq_UTRA_FDD_data + arr_index,
                    sizeof(carrier_freq_UTRA_TDD_t));
        }    
    }
    else
    {
        p_eutran_cell->num_utran_fdd_carrier_freq = SON_NULL;
    }

    SON_UT_TRACE_EXIT();
} /* son_nmm_fill_sib6_resp */

/*****************************************************************************
 * Function Name  : son_nmm_fill_sib9_resp
 * Inputs         : p_eutran_cell
 *                 p_sib_type_9_Info
 * Outputs        : None
 * Returns        : None 
 * Description    : This function fills the sib9 response info.
 ****************************************************************************/
    void
    son_nmm_fill_sib9_resp
(
 eutran_cell_num_t *p_eutran_cell,
 sib_type_9_Info_t *p_sib_type_9_Info
 )
{
    SON_UT_TRACE_ENTER();
    if ((SON_PNULL == p_eutran_cell) || (SON_PNULL == p_sib_type_9_Info))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NULL pointer recevied p_eutran_cell %p, p_sib_type_9_Info %p \n ", p_eutran_cell, p_sib_type_9_Info);
        return;
    }

    p_eutran_cell->size_hnb_Name_str = p_sib_type_9_Info->count;

    SON_MEMCPY(p_eutran_cell->hnb_Name, p_sib_type_9_Info->hnb_Name, p_eutran_cell->size_hnb_Name_str);
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_nmm_fill_sibs_resp
 * Inputs         : p_eutran_cell
 *                 p_cell
 * Outputs        : None
 * Returns        : None 
 * Description    : This function fills the all sibs response info.
 ****************************************************************************/
 void
    son_nmm_fill_sibs_resp
(
 eutran_cell_num_t *p_eutran_cell,
 cell_t *p_cell
 )
{
    SON_UT_TRACE_ENTER();
    U8 num_intra_freq_neighbor = SON_NULL; /*nuber of intra fre neig*/
    if ((SON_PNULL == p_eutran_cell) || (SON_PNULL == p_cell))
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_BRIEF, \
                " NULL pointer recevied p_eutran_cell %p, p_cell %p \n ", p_eutran_cell, p_cell);
        return;
    }

    if (p_cell->sibx_presence_flag & SIB_TYPE_3_PRESENCE_FLAG)
    {
        p_eutran_cell->presence_bitmask |= SIB_TYPE_3_PRES_FLAG;
        son_nmm_fill_sib3_resp(p_eutran_cell, p_cell->p_sib_type_3_Info);
    }

    if (p_cell->sibx_presence_flag & SIB_TYPE_4_PRESENCE_FLAG)
    {
        p_eutran_cell->presence_bitmask |= SIB_TYPE_4_PRES_FLAG;
        son_nmm_fill_sib4_resp(p_eutran_cell, p_cell->p_sib_type_4_Info);
    }
    else
    {
        p_eutran_cell->intra_freq_neighbor_list[num_intra_freq_neighbor].pci = SON_NULL;
    }

    if (p_cell->sibx_presence_flag & SIB_TYPE_5_PRESENCE_FLAG)
    {
        p_eutran_cell->presence_bitmask |= SIB_TYPE_5_PRES_FLAG;
        son_nmm_fill_sib5_resp(p_eutran_cell, p_cell->p_sib_type_5_Info);
    }

    if (p_cell->sibx_presence_flag & SIB_TYPE_6_PRESENCE_FLAG)
    {
        p_eutran_cell->presence_bitmask |= SIB_TYPE_6_PRES_FLAG;

        son_nmm_fill_sib6_resp(p_eutran_cell, p_cell->p_sib_type_6_Info);
    }
    else
    {
        p_eutran_cell->num_utran_fdd_carrier_freq = SON_NULL;
    }

    if (p_cell->sibx_presence_flag & SIB_TYPE_9_PRESENCE_FLAG)
    {
        p_eutran_cell->presence_bitmask |= SIB_TYPE_9_PRES_FLAG;
        son_nmm_fill_sib9_resp(p_eutran_cell, p_cell->p_sib_type_9_Info);
    }
    else
    { 
        p_eutran_cell->size_hnb_Name_str = SON_NULL;
    }

    SON_UT_TRACE_EXIT();
} /* son_nmm_fill_sibs_resp */

/*****************************************************************************
 * Function Name  : son_nmm_create_resp_msg 
 * Inputs         : p_scan_res_list 
 * Outputs        : None
 * Returns        : None
 * Description    : This function create son_resp 
 ****************************************************************************/
    static void
    son_nmm_create_resp_msg
(
 nmm_scan_res_list_t *p_scan_res_list
 )
{
    U16 index_earfcn = SON_NULL;             /*earfcn_index*/
    U8  index_pci = SON_NULL;                /*pci index*/
    U8  index_plmn = SON_NULL;               /*plmn index*/
    U8  valid_plmn_index = SON_NULL; 
    start_scan_res_eutran_data_t  *p_start_scan_res_data = SON_PNULL;


    SON_UT_TRACE_ENTER();

    for (index_earfcn = SON_NULL;
            index_earfcn < GET_NMM_DB_PARM(num_earfcn);
            index_earfcn++)
    {
        p_start_scan_res_data = SON_PNULL;
        p_start_scan_res_data = (start_scan_res_eutran_data_t *)son_mem_get(sizeof(start_scan_res_eutran_data_t));

        if (SON_PNULL == p_start_scan_res_data)
        {
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                    "Mem allocation fail");
            son_nmm_set_errno(SON_NMM_MEM_ALLOC_FAILURE);
            return;
        }

        SON_MEMSET(p_start_scan_res_data, SON_NULL, sizeof(start_scan_res_eutran_data_t));

        p_start_scan_res_data->scan_res_data.earfcn = \
                                                      GET_FREQ_CHANNEL_PARM(index_earfcn, earfcn);
        /*to make vale of rssi in rang*/
        p_start_scan_res_data->scan_res_data.rssi = \
                                                    GET_FREQ_CHANNEL_PARM(index_earfcn, rssi) - 1;
        p_start_scan_res_data->scan_res_data.num_of_cells = \
                                                            GET_FREQ_CHANNEL_PARM(index_earfcn, num_pci);
        /*+ SPR 18396*/
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED,
                "earfcn  [%d] num_pci %d ", p_start_scan_res_data->scan_res_data.earfcn,  
                p_start_scan_res_data->scan_res_data.num_of_cells);

        U8 temp_pci_idx =SON_NULL;

        for (index_pci = SON_NULL;
                index_pci < GET_FREQ_CHANNEL_PARM(index_earfcn, num_pci);
                index_pci++
            )
        {
            /*to make value of rsrp in range*/
            p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].rsrp = \
                                                                           p_nmm_db->freq_channel[index_earfcn].cell[index_pci].rsrp - 1;

            p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].allowed_meas_bandwidth =
                p_nmm_db->freq_channel[index_earfcn].meas_bandwidth;

            p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].pci_id = \
                                                                             GET_CELL_PARM(index_earfcn, index_pci, cell_id);
            /* SPR 21438 Fix Start */
            SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED,
                    "Earfcn [%d] num_pci [%d] Cell [%d] PCI [%d] RSRP [%d] Meas_BW [%d]",
                    p_start_scan_res_data->scan_res_data.earfcn,  
                    p_start_scan_res_data->scan_res_data.num_of_cells,
                    temp_pci_idx,
                    p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].pci_id,
                    p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].rsrp,
                    p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].allowed_meas_bandwidth
                   );
            /* SPR 21438 Fix end */


            if (GET_CELL_PARM(index_earfcn, index_pci, cell_valid) == SON_TRUE)
            {
                p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].num_of_antenna = \
                                                                                         p_nmm_db->freq_channel[index_earfcn].cell[index_pci].num_antenna;

                p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].dl_sys_bw = \
                                                                                    p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_mib->dl_sys_bandwidth;

                p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].presence_bitmask |= SIB_TYPE_1_PRES_FLAG;
                p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].presence_bitmask |= SIB_TYPE_2_PRES_FLAG;

                for (index_plmn = SON_NULL; \
                        index_plmn <
                        p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_sib_type_1_Info->cell_access_related_info.
                        plmn_Id_info_list.count; \
                        index_plmn++)

                {
                    if (p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_sib_type_1_Info->cell_access_related_info
                            .plmn_Id_info_list.plmn_identity_info[index_plmn].plmn_identity.presence_bitmask & \
                            PLMN_IDENTITY_MCC_PRESENCE_FLAG)
                    {
                        p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].plmn_identity_info[index_plmn].
                            cell_res_for_operator_use =
                            p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_sib_type_1_Info->
                            cell_access_related_info
                            .plmn_Id_info_list.plmn_identity_info[index_plmn].cell_res_for_operator_use;
                        SON_MEMCPY( \
                                p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].plmn_identity_info[index_plmn].plmn_id.mcc, \
                                p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_sib_type_1_Info->cell_access_related_info.plmn_Id_info_list.plmn_identity_info[
                                index_plmn].plmn_identity.mcc, \
                                MCC_OCTET_SIZE);
                        valid_plmn_index = index_plmn;
                    }
                    else if (index_plmn == SON_NULL)
                    {
                        GET_CELL_PARM(index_earfcn, index_pci, cell_valid) = SON_FALSE;
                        p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].presence_bitmask = \
                                                                                                   SON_NULL;
                        break;
                    }
                    else
                    {
                        SON_MEMCPY( \
                                p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].plmn_identity_info[index_plmn].plmn_id.mcc, \
                                p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_sib_type_1_Info->cell_access_related_info.plmn_Id_info_list.plmn_identity_info[
                                valid_plmn_index].plmn_identity.mcc, \
                                MCC_OCTET_SIZE);
                    }

                    p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].plmn_identity_info[index_plmn].plmn_id.
                        num_mnc_digits = \
                        p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_sib_type_1_Info->cell_access_related_info
                        .plmn_Id_info_list.plmn_identity_info[index_plmn].plmn_identity.mnc.count;

                    SON_MEMCPY(
                            p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].plmn_identity_info[index_plmn].plmn_id.mnc, \
                            p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_sib_type_1_Info->cell_access_related_info.plmn_Id_info_list.plmn_identity_info[
                            index_plmn].plmn_identity.mnc.mnc, \
                            SON_MAX_MNC_OCTET_SIZE);
                }

                valid_plmn_index = SON_NULL;

                if (GET_CELL_PARM(index_earfcn, index_pci, cell_valid) == SON_TRUE)
                {
                    son_nmm_fill_sib1_resp(&(p_start_scan_res_data->scan_res_data.cell[temp_pci_idx]),
                            p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_sib_type_1_Info);

                    son_nmm_fill_sib2_resp(&(p_start_scan_res_data->scan_res_data.cell[temp_pci_idx]),
                            p_nmm_db->freq_channel[index_earfcn].cell[index_pci].p_sib_type_2_Info);

                    son_nmm_fill_sibs_resp(&(p_start_scan_res_data->scan_res_data.cell[temp_pci_idx]),
                            &(p_nmm_db->freq_channel[index_earfcn].cell[index_pci]));
                    temp_pci_idx++;
                }
                else
                {
                    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED,
                            "Cell [%d] not valid",p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].pci_id);
                    SON_MEMSET(&(p_start_scan_res_data->scan_res_data.cell[temp_pci_idx]),SON_NULL,sizeof(eutran_cell_num_t));
                    p_start_scan_res_data->scan_res_data.num_of_cells -= SON_ONE;    
                }
            }
            else
            {
                /*reduce number of cell as this cell is not valid*/
                SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_DETAILED,
                        "Cell [%d] not valid",p_start_scan_res_data->scan_res_data.cell[temp_pci_idx].pci_id);
                SON_MEMSET(&(p_start_scan_res_data->scan_res_data.cell[temp_pci_idx]),SON_NULL,sizeof(eutran_cell_num_t));
                p_start_scan_res_data->scan_res_data.num_of_cells -= SON_ONE;    
            }
        }

        /*- SPR 18396*/
        /*adding data in list*/
        SCAN_RESP_LIST_ENQUEUE(p_scan_res_list, p_start_scan_res_data);
    }

    SON_UT_TRACE_EXIT();
} /* son_nmm_create_resp_msg */

/*****************************************************************************
 * Function Name  : son_nmm_prepare_send_resp
 * Inputs         : son_nmm_resp
 * Outputs        : None
 * Returns        : None 
 * Description    : This function create and send son_resp msg to NMMCTRL 
 ****************************************************************************/
static void
    son_nmm_prepare_send_resp
(
 son_nmm_errno_et son_nmm_resp
 )
{
    nmm_start_scan_res_t *p_nmm_start_scan_res = SON_PNULL;
    U16 msg_api_length = SON_NULL;                  /*api length*/
    U8 *p_msg = SON_PNULL;                          /*p_msg buffer*/
    U8 *p_nmm_msg = SON_PNULL;                      /*p_nmm_msg buffer*/
    son_return_et son_return = SON_FAILURE;            /*value return by func*/ 
    son_error_et  son_error_code = SON_NO_ERROR;       /*error code*/ 
    nmm_scan_res_list_t *p_scan_res_list = SON_PNULL; /*NMM_FSM scan_resp_list*/
    U32 temp_son_nmm_resp = son_nmm_resp;


    SON_UT_TRACE_ENTER();
    /*+ SPR 18399*/
    /*+ SPR 18399*/
    p_scan_res_list = (nmm_scan_res_list_t *)son_mem_get( \
            sizeof(nmm_scan_res_list_t));

    if (SON_PNULL == p_scan_res_list)
    {
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        return;
    }

    /*SPR 15935 Fix Start*/
    /* Get API length */
    msg_api_length = sizeof(nmm_start_scan_res_t);

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length + SON_API_HEADER_SIZE);

    if (p_msg == SON_PNULL)
    {
        /* Not enough memory */
        son_mem_free(p_scan_res_list);
        p_scan_res_list = SON_PNULL;
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        return;
    }
    /*SPR 15935 Fix Stop*/

    /*initialise p_scan_res_list*/ 
    SCAN_RESP_LIST_INIT(p_scan_res_list);

    switch (son_nmm_resp)
    {
        case SON_NMM_PHY_NOT_PARAM:
        case SON_NMM_PHY_UNKNOWN_API:
        case SON_NMM_PHY_UNKNOWN_TYPE:
        case SON_NMM_PHY_UNKNOWN_PARAM:
        case SON_NMM_PHY_INCORRECT_PARAM:
        case SON_NMM_PHY_NO_VALID_DATA_ERROR:
        case SON_NMM_PHY_NOT_ENABLED:
        case SON_NMM_PHY_UNEXPECTED_CMD:
        case SON_NMM_PHY_INVALID_FREQ_BAND:
        case SON_NMM_PHY_LTE_IN_USE:
        {
            son_return = SON_FAILURE;
            son_error_code = SON_ERR_NMM_UNKNOWN_FAILURE; 
            break;
        }

        case SON_NMM_PHY_CTX_ALLOC_ERROR:
        /*case SON_NMM_PHY_PART_ALLOC_ERROR:*/
        case SON_NMM_PHY_CONTEX_ALLOC_ERROR:
        case SON_NMM_PHY_HEAP_ALLOC_ERROR:
        case SON_NMM_PHY_LTE_OUT_OF_MEMORY:
        {
            son_return = SON_FAILURE;
            son_error_code = SON_ERR_SYS_MEM_ALLOC_FAILURE; 
            break;
        }

        case SON_NMM_PHY_INVALID_EARFCN:
        {
            son_return = SON_FAILURE;
            son_error_code = SON_ERR_NMM_INVALID_EARFCN;
            break;
        }

        case SON_NMM_PHY_FREQ_BAND_NOT_SUPPORTED_BY_HW:
        {
            son_return = SON_FAILURE;
            son_error_code = SON_ERR_NMM_FREQ_BAND_NOT_SUPPORTED_BY_HW;
            break;
        }

        case SON_NMM_PHY_INVALID_NR_CARRIER_LIST:
        {
            son_return = SON_FAILURE;
            son_error_code = SON_ERR_NMM_INVALID_NR_CARRIER_LIST;
            break;
        }

        case SON_NMM_PHY_INVALID_CARRIER_LIST:
        {
            son_return = SON_FAILURE;
            son_error_code = SON_ERR_NMM_INVALID_CARRIER_LIST;
            break;
        }

        case SON_NMM_TIMER_EXPIRE:
        case SON_NMM_MEM_ALLOC_FAILURE:
        case SON_NMM_MSG_SEND_FAILED:
        case SON_PHY_NULL_DATA:
        case SON_NMM_INVALID_MSG_REC:
        {
            son_return = SON_FAILURE; 
            son_error_code = (son_error_et)temp_son_nmm_resp;
            break;
        }

        case SON_NMM_SUCCESS:
        case SON_NMM_STOP_MSG_REC:
        {
            if ((SON_PNULL != p_nmm_db) && \
                    (SON_NULL != GET_NMM_DB_PARM(num_earfcn)))
            {
                son_nmm_create_resp_msg(p_scan_res_list);
                son_return = SON_SUCCESS;
                son_error_code = SON_NO_ERROR;
            }
            else
            {
                son_return = SON_FAILURE;
                son_error_code = (son_error_et)SON_NMM_STOP_MSG_REC; 
            }

            break;
        }

        default:
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name,  SON_ERROR, \
                "SON_NMM_UNKNOWN_FAILURE");
        son_return = SON_FAILURE;
        son_error_code = (son_error_et) SON_NMM_UNKNOWN_FAILURE; 
    } /* switch */

    /*SPR 15935 Fix Start*/
    /*Lines Deleted*/
    /*SPR 15935 Fix Stop*/

    SON_MEMSET(p_msg, SON_NULL, msg_api_length);
    p_nmm_msg = p_msg;

    /* Fill CSPL header */
    if (son_nmm_resp == SON_NMM_STOP_MSG_REC)
    {
        son_construct_cspl_header(p_nmm_msg, SON_VERSION_ID, SON_NMM_MODULE_ID,
                SON_NMM_MODULE_ID, SONNMM_STOP_SCAN_RES, SON_NULL, msg_api_length);
    }
    else
    {
        son_construct_cspl_header(p_nmm_msg, SON_VERSION_ID, SON_NMM_MODULE_ID,
                SON_NMM_MODULE_ID, SONNMM_START_SCAN_RES, SON_NULL, msg_api_length);
    }

    /* Fill SON_PHY_RSSI_MEAS_IND message */

    p_nmm_start_scan_res = \
                           (nmm_start_scan_res_t *)(p_nmm_msg + SON_API_HEADER_SIZE);

    p_nmm_start_scan_res->start_scan_res.son_resp = son_return;

    p_nmm_start_scan_res->start_scan_res.son_error = son_error_code;

    p_nmm_start_scan_res->start_scan_res.p_eutran_scan_res_list = p_scan_res_list;

    /* Send message to */
    son_send_msg(p_msg, SON_NMM_MODULE_ID);

    SON_UT_TRACE_EXIT();
    return;
} /* son_nmm_prepare_send_resp */

/*****************************************************************************
 * Function Name  : son_nmm_umts_create_send_message 
 * Inputs         : p_scan_res_list
 *                   p_start_scan_res_utran_data
 * Outputs        : None
 * Returns        : SON_FAILURE     - Not enough memory
 *                  SON_SUCCESS     - created and sent successfully
 * Description    : This function creates ans sends UMTS scan response
 ****************************************************************************/
        static son_return_et    
    son_nmm_umts_create_send_message
(
 nmm_scan_res_list_t *p_scan_res_list,    
 start_scan_res_utran_data_t *p_start_scan_res_utran_data  
 )
{
    nmm_start_scan_res_t *p_nmm_start_scan_res = SON_PNULL;
    U16 msg_api_length = SON_NULL;                  /*api length*/
    U8 *p_msg = SON_PNULL;                          /*p_msg buffer*/
    U8 *p_nmm_msg = SON_PNULL;                      /*p_nmm_msg buffer*/

    SCAN_RESP_LIST_INIT(p_scan_res_list);
    SCAN_RESP_LIST_ENQUEUE(p_scan_res_list, p_start_scan_res_utran_data);

    msg_api_length = sizeof(son_nmm_start_scan_res_t);

    /* Allocate buffer */
    p_msg = son_msg_mem_get(msg_api_length + SON_API_HEADER_SIZE);

    if (p_msg == SON_PNULL)
    {
        /* Not enough memory */
        SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR, \
                "Mem allocation fail");
        return SON_FAILURE;
    }

    SON_MEMSET(p_msg, SON_NULL, msg_api_length);
    p_nmm_msg = p_msg;

    son_construct_cspl_header(p_nmm_msg, SON_VERSION_ID, SON_NMM_MODULE_ID,
            SON_NMM_MODULE_ID, SONNMM_START_SCAN_RES, SON_NULL, msg_api_length);

    p_nmm_start_scan_res = \
                           (nmm_start_scan_res_t *)(p_nmm_msg + SON_API_HEADER_SIZE);

    p_nmm_start_scan_res->start_scan_res.son_resp = SON_SUCCESS;

    p_nmm_start_scan_res->start_scan_res.son_error = (son_error_et) SON_NMM_SUCCESS; 

    p_nmm_start_scan_res->start_scan_res.p_utran_scan_res_list = p_scan_res_list;

    /* Send message to */
    son_send_msg(p_msg, SON_NMM_MODULE_ID);

    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
} /* son_nmm_umts_create_send_message */

/*****************************************************************************
 * Function Name  : son_nmm_utran_get_current_state
 * Inputs         : None
 * Outputs        : None
 * Returns        : Enum nmm_utran_state_et
 * Description    : This function provide current state of UMTS nmm_fsm
 ****************************************************************************/
    static nmm_utran_state_et
    son_nmm_utran_get_current_state
    (
     /* + SPR 17439 */
     void
     /* - SPR 17439 */
    )
{
    return g_utran_current_state;
}

/*****************************************************************************
 * Function Name  : son_nmm_utran_set_current_state
 * Inputs         : state
 * Outputs        : None
 * Returns        : None
 * Description    : This function configure UMTS nmm_fsm current state
 ****************************************************************************/
 static void
    son_nmm_utran_set_current_state
(
 nmm_utran_state_et state
 )
{
    SON_UT_TRACE_ENTER();
    /*set current state of fsm*/
    g_utran_current_state = state;
    SON_UT_TRACE_EXIT();
}

/*****************************************************************************
 * Function Name  : son_nmm_utran_default_handler
 * Inputs         : *p_msg
 * Outputs        : None
 * Returns        : SON_FAILURE   - Indicates creation/sending resp failed
 * Description    : This function handles the invalid events for UMTS fsm  
 ****************************************************************************/
    static son_return_et
    son_nmm_utran_default_handler
(
 void *p_msg
 )
{
    SON_UT_TRACE_ENTER();
    SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
            /*SPR 17777 +-*/
            "Invalid Event %p",p_msg);    
    SON_UT_TRACE_EXIT();
    return SON_SUCCESS;
}


