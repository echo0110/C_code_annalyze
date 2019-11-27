/*! \file     rrm_mif_db.h
 *  \brief    This file contains contains the context creation and  
 *             maintainance functions for the MIF modules
 *  \date     January 24, 2012
 *  \author   Hargovind Sharma
*/
/*!
 *  \copyright {Copyright (c) 2009, Aricent Inc. All Rights Reserved}
*/
/****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *  DATE         AUTHOR          REFERENCE          REASON
 * 01/05/2014    gur32417        CA STG 3         Carrier Aggregation Stage 3
 *                                                code changes
 * 17 July 2014  gur31824        SPR-12598       Check for correct values of 
 *                                               initial_ri_periodicity
 * 19 July 2014  gur31824        SPR 12473       RRM is admitting GBR rab 
 *                                               even resource are not available.
****************************************************************************/


#ifndef _RRM_MIF_DB_H_
#define _RRM_MIF_DB_H_
/* Header File Includes */


/*! \headerfile rrm_defines.h <>
   */
#include "rrm_defines.h"

/*! \headerfile rrm_utils.h <>
   */
#include "rrm_utils.h"

/*! \headerfile rrm_cell_rmim_intf.h <>
   */
#include "rrm_cell_rrmim_intf.h"

/*! \headerfile rrm_oam_intf.h <>
   */
#include "rrm_oam_intf.h"

/*! \headerfile rrm_uem_cellm_intf.h <>
     */
#include "rrm_uem_cellm_intf.h"

/*! \headerfile rrm_uem_rrmim_intf.h <>
     */
#include "rrm_uem_rrmim_intf.h"

/*! \headerfile rrm_oam_composer.h <>
     */
#include "rrm_oam_composer.h"

/*! \headerfile rrm_oam_parser.h <>
     */
#include "rrm_oam_parser.h"

/*! \headerfile rrm_mif_fsm.h <>
 */
#include "rrm_mif_fsm.h"

/*
 * Enumeration Definitions
 */


/*! \enum   rmif_super_state_et
 *  \brief  An enum for Timer Types
*/
typedef enum mif_timer_type_et
{
   MIF_TIME_TO_SHUTDOWN_TIMER = 0,
   MIF_INIT_INDICATION_TIMER,
   MIF_BLOCK_CELL_TIMER,
   MIF_MAX_NUM_TIMER_TYPE
}mif_timer_type;


/* SPR 20653 Fix Start */
/* Code Removed */
/* SPR 20653 Fix End */

#define RRM_MAX_NUM_TAC_OCTETS 2

/*! \def    MIF_DEFAULT_TIMER_VAL
 *  \brief  Default timer value
 */
#define MIF_DEFAULT_TIMER_VAL 5

/*! \def    RRM_MIN_MIF_CONTEXT
 *  \brief  Minimum value for MIF context 
 */
#define RRM_MIN_MIF_CONTEXT   1

/*! \def    RRM_MAX_MIF_CONTEXT
 *  \brief  Max value for MIF context
 */
#define RRM_MAX_MIF_CONTEXT   RRM_MAX_NUM_CELLS

/*! \def    RRM_STEP_SIZE
 *  \brief  Step Size 
 */
#define RRM_STEP_SIZE         1

/*! \def    MAX_NUM_MIF_CTXT
 *  \brief Max number of MIF Contexts 
 */
#define MAX_NUM_MIF_CTXT      RRM_MAX_MIF_CONTEXT

/*! \def    MAX_CFG_PARAM_NAME_LEN
 *  \brief Max length of a parameter name in the config file 
 */
#define MAX_CFG_PARAM_NAME_LEN   60

/*! \def    RRM_SIB_3
 *  \brief Indicates Sib-3
 */
#define RRM_SIB_3               3

/*! \def    RRM_SIB_10
 *  \brief Indicates Sib-10
 */
#define RRM_SIB_10              10

/*! \def   RRM_RLC_AM 
 *  \brief Indicates AM is present
 */
#define RRM_RLC_AM              1

/*! \def    RRM_RLC_UM
 *  \brief Indicates UM is present
 */
#define RRM_RLC_UM              2

/*! \def    RRM_RLC_UM
 *  \brief Indicates UM is present
 */
#define MAX_NUM_GBR_DRBS_PER_UE 4

/*! \def    RRM_RLC_UM
 *  \brief Indicates UM is present
 */

/* changes for TDD feature */
/*! \def    MAX_NUM_NON_GBR_DRBS_PER_UE
 *  \brief Indicates maximum no of non GBR DRBS per UE
 */
#define MAX_NUM_NON_GBR_DRBS_PER_UE 5
/*! \def    RB_COUNT_6
 *  \brief Indicates RB count is 6
 */ 
#define RB_COUNT_6    6
/*! \def    RB_COUNT_15
 *  \brief Indicates RB count is 15
 */  
#define RB_COUNT_15   15  
/*! \def    RB_COUNT_25
 *  \brief Indicates RB count is 25
 */
#define RB_COUNT_25   25
/*! \def    RB_COUNT_50
 *  \brief Indicates RB count is 50
 */
#define RB_COUNT_50   50
/*! \def    RB_COUNT_75
 *  \brief Indicates RB count is 75
 */  
#define RB_COUNT_75   75 
/*! \def    RB_COUNT_100
 *  \brief Indicates RB count is 100
 */ 
#define RB_COUNT_100  100 

/*
** As per specs 36.211 
*/

/*
** Each radio frame of length = 307200 = 10ms  
*/
#define ONE_RF_LENGTH    307200
#define ONE_SF_LENFTH    30720
#define ONE_SLOT_LENGTH  15360

#define DL_SUBFRMS_COUNT_2 2
#define DL_SUBFRMS_COUNT_3 3
#define DL_SUBFRMS_COUNT_4 4
#define DL_SUBFRMS_COUNT_6 6
#define DL_SUBFRMS_COUNT_7 7
#define DL_SUBFRMS_COUNT_8 8
#define SPCL_SUBFRMS_COUNT_2 2
#define SPCL_SUBFRMS_COUNT_1 1
#define TOTAL_SF_IN_ONE_RF 10
#define SPCL_ONE_SYMBOL_UPPTS_LENGTH 2192
#define SPCL_TWO_SYMBOL_UPPTS_LENGTH 4384

#define SPCL_DWPTS_LENGTH_0 6592
#define SPCL_DWPTS_LENGTH_1 19760
#define SPCL_DWPTS_LENGTH_2 21952
#define SPCL_DWPTS_LENGTH_3 24144
#define SPCL_DWPTS_LENGTH_4 26336

#define RRM_MAX_LOG_PATH_STR_LEN    256

/* changes for TDD feature */
/*! \  rrm_mif_gb_context_t
 *  \brief  This Struc is to store the global context of RRM MgmtIfH
 *  \param  global_cell_id      store the global cell id
 *  \param  mif_ctxt_id         store the MIF Context Id
 *  \param  cell_indexd         store the cell id
 *  \param  is_info_valid       store the bool for validity of info
 */
typedef struct _rrm_mif_globalCell_mifCtxt_mapping
{
    rrm_oam_eutran_global_cell_id_t  global_cell_id;   /* Global Cell Id for mapping */
    U8                        mif_ctxt_id;   /* MIF Context Id */
    rrm_cell_index_t          cell_index;   /* Cell Index */
    rrm_bool_et             is_info_valid; /* Flag to determine validity of Info */
}rrm_mif_globalCell_mifCtxt_mapping;

/*! \   rrm_mif_gb_context_t
 *  \brief  This Struct is to store the global context of RRM MgmtIfH
 *  \param init_ind                store the receipt status of init ind messages from RRMCM and RRMUEM
 *  \param init_config_ind         store the receipt status of config resp messages from RRMCM and RRMUEM
 *  \param current_req_for_all_modules flag to be set if current request is to be processed by all modules
 *  \param module_resp  collective response from all modules 
 *  \param current_state           stores give the current state
 *  \param log_level                stores the value from rrm_oam_log_level_et
 *  \param log_enabled              stores the current state of logger(on/off)
 *  \param fsm_process_func         stores the MIF FSM function
 *  \param timer_id[MIF_MAX_NUM_TIMER_TYPE] Timer ids for running timers 
 *  \param timer_val                 stores the timer values 
 *  \param mif_ctxt_pool              stores the MIF Context Pool Info 
 *  \param is_mif_ctxt_pool_created     stores the info to check if pool is created or not
 *  \param map_info                  stores the mapping info
 *  \param bs_cellmap  Bit-set to be used for tracking all cell req/resp
 *  \param rac_response_sent_to_oam Required for RAC enable-disable handling for multiple cells
 *  \param data  data
 *  \param tx_power_params_from_oam Required for dl tx power params received from oam
 *  \param cell_stop_flag flag for cell stop
 *  \param cell_resp_flag flag for cell response
 *  \param dl_tx_alpha downlink tx alpha
 *  \param dl_tx_beta downlink tx beta
 *  \param ul_tx_alpha uplink tx alpha
 *  \param ul_offset_K uplink offset
 *  \param son_available check if son is available or not
 *  \param sps_crnti_range_flag sps crnti range present or not
 *  \param health_timer Denotes the time on which each thread needs to update its running status
 */
typedef struct _rrm_mif_gb_context_t
{
    U8               init_ind;                  /* Bit-0 for RRMCM, Bit-1 for RRMUEM */
    U8               init_config_ind;                /* Bit-0 for RRMCM, Bit-1 for RRMUEM */
    rrm_bool_et      current_req_for_all_modules;  /* flag to be set if current request is to be processed
                                                      by all modules */
    U8               module_resp;               /* collective response from all modules */
    U8               current_state;             /* This will give the current state. */
    rrm_oam_log_level_et               log_level;                 /* value from rrm_oam_log_level_et */
    rrm_oam_log_on_off_et         log_enabled;               /* current state of logger(on/off) */
    void            *fsm_process_func[MAX_FSM_TYPE]; /*Process event for MIF FSM*/
    QTIMER           timer_id[MIF_MAX_NUM_TIMER_TYPE]; /* Timer ids for running timers */
    U32              timer_val[MIF_MAX_NUM_TIMER_TYPE];/*Value of timer corresponding to particular timer type*/
    STACKCPOOL       mif_ctxt_pool;               /*Pool of MIF Contexts*/
    rrm_bool_et      is_mif_ctxt_pool_created;      /* Flag to check if pool is created or not */
    rrm_mif_globalCell_mifCtxt_mapping
                     map_info[MAX_NUM_MIF_CTXT];  /* Mapping Inof Of Global Cell id and MIF Ctxt Id*/
    U8               bs_cellmap;  /* Bit-set to be used for tracking all cell req/resp. */
    rrm_bool_et      rac_response_sent_to_oam;   /* Required for RAC enable-disable handling for multiple cells */
    /* SPR 20653 Fix Start */
    /* Code removed */
    /* SPR 20653 Fix End */
    rrm_bool_et      cell_resp_flag;
    U16                             health_timer;/* for health monitoring timer value */
    /*SPR_15766 FIX START*/
    rrm_cell_state_et cell_state;
    /*SPR_15766 FIX END*/               
} rrm_mif_gb_context_t;

/*! \  _cfg_read_data_t 
 *  \brief  This Struct is used to store the cfg data 
 *  \param  transaction_id Transaction id 
 *  \param  source Source
 */
/*vishal code change */
/* SPR 20653 Fix Start */
/* Code Removed */
/* SPR 20653 Fix End */

/*! \  mif_timer_buf 
 *  \brief  This Struct is used to store Timer Buffer
 *  \param  timer_type Type of timer
 *  \param  mif_ctxt_id  Mif Context Id
 *  \param  fsm_index Type of FSM
 *  \param  transaction_id Transaction id 
 *  \param  source Source
 */
typedef struct mif_timer_buf_t
{
    mif_timer_type timer_type;   /*Type of timer*/
    U32            mif_ctxt_id;   /*Mif Context Id*/
    mif_fsm_type     fsm_index;    /*Type of FSM*/
    U16              transaction_id;  /* Transaction id */
    U16              source;          /* Source */
}mif_timer_buf;


/*! \fn rrm_mif_fsm_process_event(rrm_mif_gb_context_t *p_gl_ctxt,rrm_mif_context *p_mif_context,U16 event_id,void *p_msg,mif_fsm_type src_fsm) 
 *  \brief  This function is used for processing event 
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param p_mif_context rrm_mif_context
 *  \param event_id U16 type
 *  \param p_msg void type pointing to msg
 *  \param src_fsm mif_fsm_type
 */
void rrm_mif_fsm_process_event(
            rrm_mif_gb_context_t     *p_gl_ctxt, 
            rrm_mif_context          *p_mif_context,
            U16                       event_id,
            void                     *p_msg,
            mif_fsm_type              src_fsm
            );

/*! \fn mif_fsm_action_routine 
 *  \brief This function is used to  Process Action Routine 
 *  \param p_gl_ctxt Pointer to global context
 *  \param p_mif_context Pointer to mif context
 *  \param p_msg Message
 */
typedef void (*mif_fsm_action_routine)
                (
                    rrm_mif_gb_context_t     *p_gl_ctxt,
                    rrm_mif_context          *p_mif_context,
                    void                     *p_msg
                );

/*! \fn mif_process_main_func 
 *  \brief This function is to Process Action Routine 
 *  \param p_gl_ctxt pointer to global context
 *  \param p_mif_context pointer to mif context
 *  \param event_id event identifier
 *  \param msg_p pointer to msg
 *  \param srcFSm source fsm
 */
typedef void    (*mif_process_main_func)
                (
                    rrm_mif_gb_context_t  *p_gl_ctxt,
                    rrm_mif_context       *p_mif_context,
                    U16                    event_id,
                    void                  *msg_p,
                    mif_fsm_type           srcFSm
                );

/*! \  interface_api_header 
 *  \brief  This Struct is to store interface api header
 *  \param  transaction_id transaction identifier
 *  \param  source source identifier
 *  \param  destination destination identifier
 *  \param  api_id api identifier
 *  \param  msg_size size of message
 */
typedef struct
{
      U16        transaction_id;
      U16        source;
      U16        destination;
      U16        api_id;
      U16        msg_size;
}interface_api_header;

/*! \fn U32 rrm_num_pool_block (rrm_mif_gb_context_t *p_gl_ctxt)
 *  \brief This function Finds number of pool block
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 */
U32 
rrm_num_pool_block (rrm_mif_gb_context_t *p_gl_ctxt);


/*! \fn rrm_mif_context* rrm_alloc_mif_context (rrm_mif_gb_context_t *p_gl_ctxt)
 *  \brief This function allocates MIF Context
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 */
rrm_mif_context* 
rrm_alloc_mif_context (rrm_mif_gb_context_t *p_gl_ctxt);


/*! \fn rrm_dealloc_mif_context (rrm_mif_context *p_mif_context,rrm_mif_gb_context_t *p_gl_ctxt)
 *  \brief this function allocates MIF Context
 *  \param p_mif_context rrm_mif_context
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 */
void rrm_dealloc_mif_context (rrm_mif_context *p_mif_context,
                              rrm_mif_gb_context_t *p_gl_ctxt);

void rrm_dealloc_all_mif_context (rrm_mif_gb_context_t *p_gl_ctxt);

/*! \fn rrm_mif_context* rrm_find_mif_context(rrm_mif_gb_context_t *p_gl_ctxt,U8 mif_ctxt_id)
 *  \brief This function finds MIF Context
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 *  \param mif_ctxt_id U8
 */
rrm_mif_context* 
rrm_find_mif_context (rrm_mif_gb_context_t *p_gl_ctxt,
                       U8                   mif_ctxt_id);

 

/*! \fn rrm_return_et rrm_mif_create_pool (rrm_mif_gb_context_t *p_gl_ctxt)
 *  \brief This function creates CSPL Pool
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 */
rrm_return_et 
rrm_mif_create_pool (rrm_mif_gb_context_t *p_gl_ctxt);

/*! \fn rrm_return_et  rrm_cv_context_pool_setup(STACKCPOOL *cpool,U32 bufsize,U32 min,U32 max,U32 step)
 *  \brief Sets up Context Pool
 *  \param cpool STACKCPOOL
 *  \param bufsize buffersize
 *  \param min minimum value
 *  \param max maximum value
 *  \param step step size
 */
rrm_return_et 
rrm_cv_context_pool_setup(STACKCPOOL *cpool, 
                          U32        bufsize, 
                          U32        min,
                          U32        max, 
                          U32        step);


/*! \fn QTIMER mif_start_timer (mif_timer_type timerType,void *p_data,U16 size,rrm_mif_gb_context_t *p_gl_ctxt)
 *  \brief this function Starts Timer
 *  \param timerType mif_timer_type
 *  \param p_data   Pointer to data 
 *  \param size      U16
 *  \param p_gl_ctxt Pointer to global context
 */
QTIMER 
mif_start_timer(
                mif_timer_type        timerType, 
                void                  *p_data, 
                U16                   size,
                rrm_mif_gb_context_t  *p_gl_ctxt
);

/*! \fn mif_stop_timer (QTIMER    p_timerId) 
 *  \brief this function stops Timer
 *  \param p_timerId QTIMER
 */
void 
mif_stop_timer (QTIMER p_timerId);

/*! \fn mif_get_timer_data(QTIMER p_timerId)
 *  \brief This function id usde to fetch Timer Data
 *  \param p_timerId timer identifier
 */
void* 
mif_get_timer_data(QTIMER  p_timerId);

/*! \fn rrm_return_et compare_global_cell_id(rrm_oam_eutran_global_cell_id_t *p_rcvd,rrm_oam_eutran_global_cell_id_t *p_strd)
 *  \brief  Function to compare global cell id  
 *  \param  p_rcvd Pointer to  global Ctxt received
 *  \param  p_strd store the global Ctxt
 */
rrm_return_et compare_global_cell_id(
      rrm_oam_eutran_global_cell_id_t     *p_rcvd,
      rrm_oam_eutran_global_cell_id_t     *p_strd);

/*! \fn rrm_return_et allocate_and_store_tcb_info_in_mif_context(rrm_mif_context *p_mif_context,U16 rcvd_trans_id,U16 rcvd_src_id) 
 *  \brief  this Function is to allocate TCB  
 *  \param  p_mif_context  store the Mif Ctxt
 *  \param  rcvd_trans_id   store the trans id
 *  \param  rcvd_src_id   store the source id
 */
rrm_return_et allocate_and_store_tcb_info_in_mif_context(
                                            rrm_mif_context    *p_mif_context,
                                            /* SPR_19279_START */
                                            void               *ptr,
                                            U16                size,
                                            /* SPR_19279_END */
                                            U16                rcvd_trans_id,
                                            U16                rcvd_src_id);
rrm_return_et store_fail_cause_req_send_rcvd (
        U16 fail_cause,
        rrm_mif_context    *p_mif_context,
        U16                 rcvd_trans_id);

rrm_return_et de_allocate_all_tcb_from_mif_ctxt(rrm_mif_gb_context_t *p_gl_ctxt,
        U16 recv_trans_id);


/*! \fn free_cell_config_cfg_node(rrm_mif_gb_context_t *p_gl_ctxt,rrm_cell_index_t  cell_index)
 *  \brief This function allocates MIF Context
 *  \param p_gl_ctxt rrm_mif_gb_context_t
 */
void free_cell_config_cfg_node(rrm_mif_gb_context_t *p_gl_ctxt,rrm_cell_index_t  cell_index);

U16 fetch_fail_cause(
        rrm_mif_gb_context_t *p_gl_ctxt,
        U16                 rcvd_trans_id);

/*! \fn rrm_return_et validate_trans_id_rcvd_in_resp(rrm_mif_context *p_mif_context,U16 rcvd_trans_id)
 *  \brief  This Function is to validate transaction id  
 *  \param  p_mif_context  store the Mif Ctxt
 *  \param  rcvd_trans_id   store the trans id
 */
rrm_return_et validate_trans_id_rcvd_in_resp (
                                            rrm_mif_context    *p_mif_context,
                                            U16                 rcvd_trans_id);

/*! \fn U16 return_src_id_from_tcb(rrm_mif_context *p_mif_context,U16 rcvd_trans_id) 
 *  \brief  This Function is to get the source id  
 *  \param  p_mif_context  store the Mif Ctxt
 *  \param  rcvd_trans_id   store the trans id
 */
U16 return_src_id_from_tcb(
                           rrm_mif_context    *p_mif_context,
                           U16                 rcvd_trans_id);


/*! \fn rrm_return_et fetch_mif_ctxt_id_from_global_ctxt (rrm_mif_gb_context_t *p_gl_ctxt,rrm_oam_eutran_global_cell_id_t *p_global_cell_id,U8 *p_mif_ctxt_cnt,U8 *p_mif_ctxt)
 *  \brief  This function is to get the Mif Ctxt Id  
 *  \param  p_gl_ctxt  store the Global Ctxt
 *  \param  p_global_cell_id store the Global Cell Id
 *  \param  p_mif_ctxt_cnt   store the MIF Ctxt Cnt
 *  \param  p_mif_ctxt       store the MIF Ctxt
 */
rrm_return_et fetch_mif_ctxt_id_from_global_ctxt(rrm_mif_gb_context_t *p_gl_ctxt,
                                                 rrm_oam_eutran_global_cell_id_t
                                                                      *p_global_cell_id,
                                                 U8                   *p_mif_ctxt_cnt,
                                                 U8                   *p_mif_ctxt);



//CFG START
/***********ENUMS************/
/*! \enum rrm_rrc_modification_period_coeff_cfg_et
 *  \brief An enum for modification period coefficient cfg 
 */
typedef enum
{
  RRM_RRC_MOD_PERIOD_COFF_CFG
}rrm_rrc_modification_period_coeff_cfg_et;
/*! \enum rrm_rrc_default_paging_cycle_cfg_et
 *  \brief An enum for default paging cycle config
 */
typedef enum
{
  RRM_RRC_DEF_PAG_CYCLE_RF32_CFG,
  RRM_RRC_DEF_PAG_CYCLE_RF64_CFG,
  RRM_RRC_DEF_PAG_CYCLE_RF128_CFG,
  RRM_RRC_DEF_PAG_CYCLE_RF256_CFG
}rrm_rrc_default_paging_cycle_cfg_et;
/*! \enum rrm_rrc_nB_cfg_et
 *  \brief An enum for nodeB configuration info
 */
typedef enum
{
  RRM_RRC_NB_FOUR_T_CFG,
  RRM_RRC_NB_TWO_T_CFG,
  RRM_RRC_NB_ONE_T_CFG,
  RRM_RRC_NB_HALF_T_CFG,
  RRM_RRC_NB_QUARTER_T_CFG,
  RRM_RRC_NB_ONE_EIGHTH_T_CFG,
  RRM_RRC_NB_ONE_SIXTEENTH_T_CFG,
  RRM_RRC_NB_ONE_THIRTY_SECOND_T_CFG
}rrm_rrc_nB_cfg_et;
/*! \enum rrm_rrc_ul_cyclic_prefix_length_cfg_et
 *  \brief An enum for uplink cyclic prefix length configuration
 */
typedef enum
{
  RRM_RRC_UL_CYC_PREFIX_LEN_1_CFG,
  RRM_RRC_UL_CYC_PREFIX_LEN_2_CFG
}rrm_rrc_ul_cyclic_prefix_length_cfg_et;
/*! \enum rrm_rrc_antenna_ports_count_cfg_et
 *  \brief An enum for antenna ports count config
 */
typedef enum
{
  RRM_RRC_ANTENNA_PORTS_COUNT_AN1_CFG =1,
  RRM_RRC_ANTENNA_PORTS_COUNT_AN2_CFG =2,
  RRM_RRC_ANTENNA_PORTS_COUNT_AN4_CFH =4,
  RRM_RRC_ANTENNA_PORTS_COUNT_SPARE1_CFG
}rrm_rrc_antenna_ports_count_cfg_et;
/*! \enum rrm_rrc_delta_f_pucch_format_1b_cfg_et
 *  \brief An enum for delta pucch format 1b config
 */
typedef enum
{
        RRM_RRC_DELTA_F_PUCCH_FORMAT_1B_DELTAF1_CFG,
        RRM_RRC_DELTA_F_PUCCH_FORMAT_1B_DELTAF3_CFG,
        RRM_RRC_DELTA_F_PUCCH_FORMAT_1B_DELTAF5_CFG
}rrm_rrc_delta_f_pucch_format_1b_cfg_et;

/* CA_Stage3_PUCCH_Format_Enh: Start */
/*! \enum rrm_rrc_delta_f_pucch_format_1b_cs_r10_cfg_et
 *  \brief An enum for delta pucch format 1b_cs-v1020 config
 */
typedef enum
{
    RRM_RRC_DELTA_F_PUCCH_FORMAT_1B_CS_R10_DELTAF1_CFG,
    RRM_RRC_DELTA_F_PUCCH_FORMAT_1B_CS_R10_DELTAF2_CFG
}
rrm_rrc_delta_f_pucch_format_1b_cs_r10_cfg_et;

/*! \enum rrm_rrc_delta_f_pucch_format_3_r10_et
 *  \brief An enum for delta pucch format 3-v1020 config
 */
typedef enum
{
    RRM_RRC_DELTA_F_PUCCH_FORMAT_3_R10_DELTAF_1_CFG,
    RRM_RRC_DELTA_F_PUCCH_FORMAT_3_R10_DELTAF0_CFG,
    RRM_RRC_DELTA_F_PUCCH_FORMAT_3_R10_DELTAF1_CFG,
    RRM_RRC_DELTA_F_PUCCH_FORMAT_3_R10_DELTAF2_CFG,
    RRM_RRC_DELTA_F_PUCCH_FORMAT_3_R10_DELTAF3_CFG,
    RRM_RRC_DELTA_F_PUCCH_FORMAT_3_R10_DELTAF4_CFG,
    RRM_RRC_DELTA_F_PUCCH_FORMAT_3_R10_DELTAF5_CFG,
    RRM_RRC_DELTA_F_PUCCH_FORMAT_3_R10_DELTAF6_CFG
}rrm_rrc_delta_f_pucch_format_3_r10_et;
/* CA_Stage3_PUCCH_Format_Enh: End */

/*! \enum rrm_rrc_delta_f_pucch_format_2_cfg_et
 *  \brief An enum for delta pucch format 2 config
 */
typedef enum
{
        RRM_RRC_DELTA_F_PUCCH_FORMAT_2_CFG_DELTAF_2_CFG,
        RRM_RRC_DELTA_F_PUCCH_FORMAT_2_CFG_DELTAF0_CFG,
        RRM_RRC_DELTA_F_PUCCH_FORMAT_2_CFG_DELTAF1_CFG,
        RRM_RRC_DELTA_F_PUCCH_FORMAT_2_CFG_DELTAF2_CFG
}rrm_rrc_delta_f_pucch_format_2_cfg_et;
/*! \enum rrm_rrc_delta_f_pucch_format_cfg_et
 *  \brief An enum for  delta f pucch format 2 config
 */
typedef enum
{
        RRM_RRC_DELTA_F_PUCCH_FORMAT_CFG_DELTAF_2_CFG,
        RRM_RRC_DELTA_F_PUCCH_FORMAT_CFG_DELTAF0_CFG,
        RRM_RRC_DELTA_F_PUCCH_FORMAT_CFG_DELTAF2_CFG
}rrm_rrc_delta_f_pucch_format_cfg_et;

/******* STRUCTURES *********/
/* SPR 20653 Fix Start */
/* Code Removed */
/* SPR 20653 Fix End */

/*! \fn rrm_return_et fetch_mif_ctxt_id_based_on_cell_id(rrm_mif_gb_context_t *p_gl_ctxt,rrm_cell_index_t cell_index,U8 *p_mif_ctxt_cnt,U8 *p_mif_ctxt)
 *  \brief This function is used to fetch mif context id based on cell ID
 *  \param p_gl_ctxt Pointer to global context
 *  \param cell_index cell index
 *  \param p_mif_ctxt_cnt mif context count
 *  \param p_mif_ctxt mif context
 */
rrm_return_et fetch_mif_ctxt_id_based_on_cell_id(rrm_mif_gb_context_t *p_gl_ctxt,
                                                 rrm_cell_index_t      cell_index,
                                                 U8                   *p_mif_ctxt_cnt,
                                                 U8                   *p_mif_ctxt);

/*! \fn rrm_return_et copy_global_cell_id_from_ctxt(rrm_mif_gb_context_t *p_gl_ctxt,rrm_oam_eutran_global_cell_id_t *p_global_cell_id,rrm_cell_index_t cell_index)
 *  \brief This function is used to copy global cell id from context
 *  \param p_gl_ctxt pointer to global context
 *  \param p_global_cell_id global cell ID
 *  \param cell_index cell index
 */
rrm_return_et copy_global_cell_id_from_ctxt(rrm_mif_gb_context_t *p_gl_ctxt,
                                            rrm_oam_eutran_global_cell_id_t
                                                            *p_global_cell_id,
                                            rrm_cell_index_t      cell_index);
/*! \fn  rrm_return_et de_allocate_tcb_from_mif_ctxt( rrm_mif_context *p_mif_context, U16 rcvd_trans_id)
 *  \brief This function is used to deallocate tcb from mif context 
 *  \param p_mif_context Pointer to MIF context
 *  \param rcvd_trans_id Received transaction ID
 */
rrm_return_et de_allocate_tcb_from_mif_ctxt(
                           rrm_mif_context    *p_mif_context,
                           U16                 rcvd_trans_id);

/* SPR 20653 Fix Start */
/* Code Removed */
/* SPR 20653 Fix End */

/*SPR_11838_FIX_START */

/* Hash Defines for Boundary Conditions */
#define RRM_CFG_DB_BETA_OFFSET_ACK_INDEX_LOWER_BOUND			0
#define RRM_CFG_DB_BETA_OFFSET_ACK_INDEX_UPPER_BOUND			15
#define RRM_CFG_DB_BETA_OFFSET_RI_INDEX_LOWER_BOUND			0
#define RRM_CFG_DB_BETA_OFFSET_RI_INDEX_UPPER_BOUND			15
#define RRM_CFG_DB_BETA_OFFSET_CQI_INDEX_LOWER_BOUND			0
#define RRM_CFG_DB_BETA_OFFSET_CQI_INDEX_UPPER_BOUND			15

#define RRM_CFG_DB_P0_UE_PUSCH_LOWER_BOUND		                -8	
#define RRM_CFG_DB_P0_UE_PUSCH_UPPER_BOUND			         7
/* SPR_14230_start */
/* code removed */
/* SPR_14230_stop */
#define RRM_CFG_DB_P0_UE_PUCCH_LOWER_BOUND		                -8	
#define RRM_CFG_DB_P0_UE_PUCCH_UPPER_BOUND			         7
#define RRM_CFG_DB_P_SRS_OFFSET_LOWER_BOUND		                 0	
#define RRM_CFG_DB_P_SRS_OFFSET_UPPER_BOUND			         15
#define RRM_CFG_DB_FILTER_COEFFICIENT_LOWER_BOUND		         0	
#define RRM_CFG_DB_FILTER_COEFFICIENT_UPPER_BOUND			 14
#define RRM_CFG_DB_NOM_PDSCH_RS_EPRE_OFFSET_LOWER_BOUND                  -1
#define RRM_CFG_DB_NOM_PDSCH_RS_EPRE_OFFSET_UPPER_BOUND                  6
#define RRM_CFG_DB_INITIAL_SR_PERIODICITY_LOWER_BOUND                    0
/* SPR 18871 Fix Start */
#define RRM_CFG_DB_INITIAL_SR_PERIODICITY_UPPER_BOUND                    6
#define RRM_CFG_DB_FINAL_SR_PERIODICITY_LOWER_BOUND                      0
#define RRM_CFG_DB_FINAL_SR_PERIODICITY_UPPER_BOUND                      6
/* SPR 18871 Fix End */
#define RRM_CFG_DB_INITIAL_CQI_PERIODICITY_LOWER_BOUND                   0
#define RRM_CFG_DB_INITIAL_CQI_PERIODICITY_UPPER_BOUND                   6
#define RRM_CFG_DB_FINAL_CQI_PERIODICITY_LOWER_BOUND                     0
#define RRM_CFG_DB_FINAL_CQI_PERIODICITY_UPPER_BOUND                     6
#define RRM_CFG_DB_SRS_PERIODICITY_LOWER_BOUND                           0 
#define RRM_CFG_DB_SRS_PERIODICITY_UPPER_BOUND                           7
#define RRM_CFG_DB_CONTENTION_FREE_RACH_TIMER_LOWER_BOUND                0
#define RRM_CFG_DB_CONTENTION_FREE_RACH_TIMER_UPPER_BOUND                7 
#define RRM_CFG_DB_INITIAL_UE_LOCATION_LOWER_BOUND                       0
#define RRM_CFG_DB_INITIAL_UE_LOCATION_UPPER_BOUND                       1 
#define RRM_CFG_DB_WAIT_TIME_LOWER_BOUND                                 1
#define RRM_CFG_DB_WAIT_TIME_UPPER_BOUND                                 16
#define RRM_CFG_DB_EXTENDED_WAIT_TIME_LOWER_BOUND                        1
#define RRM_CFG_DB_EXTENDED_WAIT_TIME_UPPER_BOUND                        1800
#define RRM_CFG_DB_MAX_UL_HARQ_TX_LOWER_BOUND                            0
#define RRM_CFG_DB_MAX_UL_HARQ_TX_UPPER_BOUND                            13 
#define RRM_CFG_DB_SAMPLING_NORMAL_TIME_LOWER_BOUND                      1 
#define RRM_CFG_DB_SAMPLING_NORMAL_TIME_UPPER_BOUND                      255 
#define RRM_CFG_DB_TNL_LOAD_SUPPORTED_FLAG_LOWER_BOUND                   0
#define RRM_CFG_DB_TNL_LOAD_SUPPORTED_FLAG_UPPER_BOUND                   1 
#define RRM_CFG_DB_HW_LOAD_SUPPORTED_FLAG_LOWER_BOUND                    0
#define RRM_CFG_DB_HW_LOAD_SUPPORTED_FLAG_UPPER_BOUND                    1 
#define RRM_CFG_DB_RB_SIZE_LOWER_BOUND                                   0
#define RRM_CFG_DB_RB_SIZE_UPPER_BOUND                                   1 
#define RRM_CFG_DB_PERIODIC_REPORT_REQUIRED_LOWER_BOUND                  0
#define RRM_CFG_DB_PERIODIC_REPORT_REQUIRED_UPPER_BOUND                  1 
#define RRM_CFG_DB_PERIODIC_REPORT_PERIODICITY_LOWER_BOUND               100
#define RRM_CFG_DB_PERIODIC_REPORT_UE_BATCH_SIZE_LOWER_BOUND             1 
#define RRM_CFG_DB_PERIODIC_REPORT_UE_BATCH_SIZE_UPPER_BOUND             25 
#define RRM_CFG_DB_CELL_STOP_FLAG_LOWER_BOUND                            0
#define RRM_CFG_DB_CELL_STOP_FLAG_UPPER_BOUND                            1 
#define RRM_CFG_DB_UL_PATHLOSS_THRESHOLD_LOWER_BOUND                     0
#define RRM_CFG_DB_UL_PATHLOSS_THRESHOLD_UPPER_BOUND                     255 
#define RRM_CFG_DB_DL_SINR_QUALITY_THRESHOLD_LOWER_BOUND                 0
#define RRM_CFG_DB_DL_SINR_QUALITY_THRESHOLD_UPPER_BOUND                 255 
#define RRM_CFG_DB_UPLINK_SINR_THRESHOLD_LOWER_BOUND                     0
#define RRM_CFG_DB_UPLINK_SINR_THRESHOLD_UPPER_BOUND                     255 
#define RRM_CFG_DB_DB_UPLINK_PRB_BUDGET_THRESHOLD_LOWER_BOUND            1 
#define RRM_CFG_DB_DB_UPLINK_PRB_BUDGET_THRESHOLD_UPPER_BOUND            100
#define RRM_CFG_DB_UPLINK_PRB_BUDGET_DOWN_THRESHOLD_LOWER_BOUND          1
#define RRM_CFG_DB_UPLINK_PRB_BUDGET_DOWN_THRESHOLD_UPPER_BOUND          100
/* SPR_12473_FIX_start */
#define RRM_CFG_DB_UL_PRB_PER_BPS_LOWER_BOUND                            1
#define RRM_CFG_DB_UL_PRB_PER_BPS_UPPER_BOUND                            300 
#define RRM_CFG_DB_DL_PRB_PER_BPS_LOWER_BOUND                            1
#define RRM_CFG_DB_DL_PRB_PER_BPS_UPPER_BOUND                            300 
/* SPR_12473_FIX_end */
#define RRM_CFG_DB_SRB_BIT_RATE_LOWER_BOUND                              0
#define RRM_CFG_DB_NGBR_THRESHOLD_LOWER_BOUND                            0
/* SPR_15537_Fix_Start */
#define RRM_CFG_DB_NUM_CONTROL_PRBS_LOWER_BOUND                          0
/* SPR_15537_Fix_End */
/* BUG_11815_FIX_START */
#define RRM_CFG_DB_NGBR_THRESHOLD_UPPER_BOUND                            1 
#define RRM_CFG_DB_NON_DEFAULT_BEARER_MULTIPLIER_LOWER_BOUND             0
#define RRM_CFG_DB_NON_DEFAULT_BEARER_MULTIPLIER_UPPER_BOUND             1 
/* BUG_11815_FIX_END */
#define RRM_CFG_DB_DL_SRB_BIT_RATE_LOWER_BOUND                           0
#define RRM_CFG_DB_DL_SRB_BIT_RATE_UPPER_BOUND                           0 
#define RRM_CFG_DB_UL_SRB_BIT_RATE_LOWER_BOUND                           0
#define RRM_CFG_DB_UL_SRB_BIT_RATE_UPPER_BOUND                           0 
#define RRM_CFG_DB_UE_SINR_THRESHOLD_LOWER_BOUND                         0
#define RRM_CFG_DB_UE_SINR_THRESHOLD_UPPER_BOUND                         255 
#define RRM_CFG_DB_RLC_TX_BUFFER_SIZE_FACTOR_NGBR_LOWER_BOUND            1
#define RRM_CFG_DB_RLC_TX_BUFFER_SIZE_FACTOR_NGBR_UPPER_BOUND            4 
#define RRM_CFG_DB_MINIMUM_BITRATE_LOWER_BOUND                           0
#define RRM_CFG_DB_DELAYED_A3_ENABLED_LOWER_BOUND                        0
#define RRM_CFG_DB_DELAYED_A3_ENABLED_UPPER_BOUND                        1 
#define RRM_CFG_DB_A4_MEAS_CFG_ENABLED_LOWER_BOUND                       0
#define RRM_CFG_DB_A4_MEAS_CFG_ENABLED_UPPER_BOUND                       1 
#define RRM_CFG_DB_CSFB_BLIND_TIMER_DURATION_LOWER_BOUND                 0
#define RRM_CFG_DB_CSFB_BLIND_TIMER_DURATION_UPPER_BOUND                 65535 
/*SPR 21365 fix start*/
/*Lines Deleted*/
/*SPR 21365 fix stop*/
#define RRM_CFG_DB_B1_EVENT_TRIGGER_QUANTITY_LOWER_BOUND                 0
#define RRM_CFG_DB_B1_EVENT_TRIGGER_QUANTITY_UPPER_BOUND                 1 
#define RRM_CFG_DB_B2_EVENT_UTRAN_TRIGGER_QUANTITY_LOWER_BOUND           0
#define RRM_CFG_DB_B2_EVENT_UTRAN_TRIGGER_QUANTITY_UPPER_BOUND           1 
#define RRM_CFG_DB_IRAT_OFFSET_LOWER_BOUND                               -15
#define RRM_CFG_DB_IRAT_OFFSET_UPPER_BOUND                               15 
#define RRM_CFG_DB_SPID_FOR_CDMA_LOWER_BOUND                             1 
#define RRM_CFG_DB_SPID_FOR_CDMA_UPPER_BOUND                             255
#define RRM_CFG_DB_S_MEASURE_LOWER_BOUND                                 0
#define RRM_CFG_DB_S_MEASURE_UPPER_BOUND                                 97
#define RRM_CFG_DB_TRIGGER_QUANTITY_FOR_BLOCK_LOWER_BOUND                0
#define RRM_CFG_DB_TRIGGER_QUANTITY_FOR_BLOCK_UPPER_BOUND                97 
#define RRM_CFG_DB_A4_THRESHOLD_RSRP_FOR_BLOCK_LOWER_BOUND               0
#define RRM_CFG_DB_A4_THRESHOLD_RSRP_FOR_BLOCK_UPPER_BOUND               97 
#define RRM_CFG_DB_A4_THRESHOLD_RSRQ_FOR_BLOCK_LOWER_BOUND               0
#define RRM_CFG_DB_A4_THRESHOLD_RSRQ_FOR_BLOCK_UPPER_BOUND               34 


#define RRM_CFG_DB_TPC_FOR_MSG3_LOWER_BOUND						0
#define RRM_CFG_DB_TPC_FOR_MSG3_UPPER_BOUND						7
#define RRM_CFG_DB_SRS_SUB_BAND_GROUP_SIZE_LOWER_BOUND					1
#define RRM_CFG_DB_SRS_SUB_BAND_GROUP_SIZE_UPPER_BOUND					8
#define RRM_CFG_DB_TPC_TRIGGER_WAIT_TIME_FOR_PUSCHDCI0_LOWER_BOUND			0
#define RRM_CFG_DB_TPC_TRIGGER_WAIT_TIME_FOR_PUCCHDCI1x2x_LOWER_BOUND			0
#define RRM_CFG_DB_TPC_NUM_UE_THRESHOLD_FOR_PUSCHDCI3A_LOWER_BOUND			0
#define RRM_CFG_DB_TPC_NUM_UE_THRESHOLD_FOR_PUSCHDCI3A_UPPER_BOUND			100
#define RRM_CFG_DB_TPC_NUM_UE_THRESHOLD_FOR_PUCCHDCI3A_LOWER_BOUND			0
#define RRM_CFG_DB_TPC_NUM_UE_THRESHOLD_FOR_PUCCHDCI3A_UPPER_BOUND			100
#define RRM_CFG_DB_COOL_OFF_FOR_BLER_TRIGGERED_PUCCH_TPC_UPPER_BOUND			0
#define RRM_CFG_DB_COOL_OFF_FOR_PUCCH_TPC_LOWER_BOUND					0
#define RRM_CFG_DB_COOL_OFF_FOR_PUSCH_TPC_LOWER_BOUND					0

#define RRM_CFG_DB_DEFAULT_PL_LOWER_BOUND			0
#define RRM_CFG_DB_DEFAULT_PL_UPPER_BOUND			 460

#define RRM_CFG_DB_ALPHA_BASED_PL_TO_TARGET_SINR_COUNT_LOWER_BOUND			1
#define RRM_CFG_DB_ALPHA_BASED_PL_TO_TARGET_SINR_COUNT_UPPER_BOUND			10
#define RRM_CFG_DB_ALPHA_BASED_PL_TO_TARGET_SINR_START_PL_LOWER_BOUND			-230
#define RRM_CFG_DB_ALPHA_BASED_PL_TO_TARGET_SINR_START_PL_UPPER_BOUND			460
#define RRM_CFG_DB_ALPHA_BASED_PL_TO_TARGET_SINR_END_PL_LOWER_BOUND			-230
#define RRM_CFG_DB_ALPHA_BASED_PL_TO_TARGET_SINR_END_PL_UPPER_BOUND			460
#define RRM_CFG_DB_ALPHA_BASED_PL_TO_TARGET_SINR_LOWER_BOUND			0
#define RRM_CFG_DB_ALPHA_BASED_PL_TO_TARGET_SINR_UPPER_BOUND			255

#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_COUNT_LOWER_BOUND			1
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_COUNT_UPPER_BOUND			4
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_START_DELTA_SINR_LOWER_BOUND			-255
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_START_DELTA_SINR_UPPER_BOUND			255
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_END_DELTA_SINR_LOWER_BOUND			-255
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_END_DELTA_SINR_UPPER_BOUND			255
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_TPC_LOWER_BOUND			-4
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUSCH_TPC_UPPER_BOUND			4


#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_BLER_TARGET_LOWER_BOUND			0
#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_BLER_TARGET_UPPER_BOUND			100
#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_COUNT_LOWER_BOUND			1
#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_COUNT_UPPER_BOUND			4
#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_START_BLER_LOWER_BOUND			-255
#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_START_BLER_UPPER_BOUND			255
#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_END_BLER_LOWER_BOUND			-255
#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_END_BLER_UPPER_BOUND			255
#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_TPC_LOWER_BOUND			-1
#define RRM_CFG_DB_DELTA_BLER_TO_TPC_MAP_FOR_PUCCH_TPC_UPPER_BOUND			3



#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_SINR_TARGET_LOWER_BOUND			0
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_SINR_TARGET_UPPER_BOUND			255
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_COUNT_LOWER_BOUND			1
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_COUNT_UPPER_BOUND			4
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_START_SINR_LOWER_BOUND			-255
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_START_SINR_UPPER_BOUND			255
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_END_SINR_LOWER_BOUND			-255
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_END_SINR_UPPER_BOUND			255
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_TPC_LOWER_BOUND			-1
#define RRM_CFG_DB_DELTA_SINR_TO_TPC_MAP_FOR_PUCCH_TPC_UPPER_BOUND			3
#define RRM_CFG_DB_CQI_TO_PHICH_POWER_INFO_LOWER_BOUND			0
#define RRM_CFG_DB_CQI_TO_PHICH_POWER_INFO_UPPER_BOUND			10000
#define RRM_CFG_DB_MIN_RB_FOR_PL_PHR_CALC_LOWER_BOUND			0
#define RRM_CFG_DB_MIN_RB_FOR_PL_PHR_CALC_UPPER_BOUND			100

#define RRM_CFG_DB_TIME_TO_STAY_THRESHOLD_LOWER_BOUND			0
#define RRM_CFG_DB_TIME_TO_STAY_THRESHOLD_UPPER_BOUND			255
#define RRM_CFG_DB_MIN_NUM_OF_OSCILLATION_LOOP_LOWER_BOUND			0
#define RRM_CFG_DB_MIN_NUM_OF_OSCILLATION_LOOP_UPPER_BOUND			255
#define RRM_CFG_DB_MAX_LAST_VISITED_CELLS_SEARCHED_LOWER_BOUND			0
#define RRM_CFG_DB_MAX_LAST_VISITED_CELLS_SEARCHED_UPPER_BOUND			16
#define RRM_CFG_DB_HO_OSCILLATION_CIO_OFFSET_LOWER_BOUND			0
#define RRM_CFG_DB_HO_OSCILLATION_CIO_OFFSET_UPPER_BOUND			30

#define RRM_CFG_DB_UE_HAND_OVER_COUNT_LOWER_BOUND			1
#define RRM_CFG_DB_UE_HAND_OVER_COUNT_UPPER_BOUND			255
#define RRM_CFG_DB_MIN_UE_POWER_LOWER_BOUND			-40
#define RRM_CFG_DB_MIN_UE_POWER_UPPER_BOUND			23


#define RRM_CFG_DB_REF_SIGNAL_POWER_RAMPING_UP_LOWER_BOUND			0
#define RRM_CFG_DB_REF_SIGNAL_POWER_RAMPING_UP_UPPER_BOUND			90
#define RRM_CFG_DB_PA_RAMPING_STEP_LOWER_BOUND			0
#define RRM_CFG_DB_PA_RAMPING_STEP_UPPER_BOUND			7
#define RRM_CFG_DB_PDCCH_INTERFERENCE_IMPACT_LOWER_BOUND			0
#define RRM_CFG_DB_PDCCH_INTERFERENCE_IMPACT_UPPER_BOUND			4
#define RRM_CFG_DB_L1_MEAS_REPORT_REQUIRED_LOWER_BOUND			0
#define RRM_CFG_DB_L1_MEAS_REPORT_REQUIRED_UPPER_BOUND			1
#define RRM_CFG_DB_L1_MEAS_REPORT_AVG_ENABLED_LOWER_BOUND			0
#define RRM_CFG_DB_L1_MEAS_REPORT_AVG_ENABLED_UPPER_BOUND			1
#define RRM_CFG_DB_L1_MEAS_REPORT_PERIODICITY_LOWER_BOUND			20
#define RRM_CFG_DB_CELL_EDGE_PRB_USAGE_REQUIRED_LOWER_BOUND			0
#define RRM_CFG_DB_CELL_EDGE_PRB_USAGE_REQUIRED_UPPER_BOUND			1
#define RRM_CFG_DB_L2_ICIC_REPORT_PERIODICITY_LOWER_BOUND			20

#define RRM_CFG_DB_PDSCH_EPRE_TO_UE_RS_RATIO_LOWER_BOUND			-6
#define RRM_CFG_DB_PDSCH_EPRE_TO_UE_RS_RATIO_UPPER_BOUND			3
#define RRM_CFG_DB_ENABLE_PLI_RI_REPORTT_R9_LOWER_BOUND			0
#define RRM_CFG_DB_ENABLE_PLI_RI_REPORTT_R9_UPPER_BOUND			1
#define RRM_CFG_DB_BEAM_FORMING_ALGO_LOWER_BOUND			0
#define RRM_CFG_DB_BEAM_FORMING_ALGO_UPPER_BOUND			1


#define RRM_CFG_DB_BEAM_FORMING_VECTOR_PRESENT_LOWER_BOUND			0
#define RRM_CFG_DB_BEAM_FORMING_VECTOR_PRESENT_UPPER_BOUND			1
#define RRM_CFG_DB_ATOA_BEAM_FORMING_VECTOR_LOWER_BOUND			0
#define RRM_CFG_DB_ATOA_BEAM_FORMING_VECTOR_UPPER_BOUND			65535
#define RRM_CFG_DB_SINR_TO_BEAM_FORMING_VECTOR_LOWER_BOUND			0
#define RRM_CFG_DB_SINR_TO_BEAM_FORMING_VECTOR_UPPER_BOUND			65535

#define RRM_CFG_DB_HIGH_PRIORITY_BUCKET_1_LOWER_BOUND			0
#define RRM_CFG_DB_HIGH_PRIORITY_BUCKET_1_UPPER_BOUND			999
#define RRM_CFG_DB_HIGH_PRIORITY_BUCKET_2_LOWER_BOUND			0
#define RRM_CFG_DB_HIGH_PRIORITY_BUCKET_2_UPPER_BOUND			999
#define RRM_CFG_DB_HIGH_PRIORITY_BUCKET_3_LOWER_BOUND			0
#define RRM_CFG_DB_HIGH_PRIORITY_BUCKET_3_UPPER_BOUND			999
#define RRM_CFG_DB_MEDIUM_PRIORITY_BUCKET_1_LOWER_BOUND			0
#define RRM_CFG_DB_MEDIUM_PRIORITY_BUCKET_1_UPPER_BOUND			999
#define RRM_CFG_DB_MEDIUM_PRIORITY_BUCKET_2_LOWER_BOUND			0
#define RRM_CFG_DB_MEDIUM_PRIORITY_BUCKET_2_UPPER_BOUND			999
#define RRM_CFG_DB_LOW_PRIORITY_BUCKET_1_LOWER_BOUND			0
#define RRM_CFG_DB_LOW_PRIORITY_BUCKET_1_UPPER_BOUND			999
#define RRM_CFG_DB_LOW_PRIORITY_BUCKET_2_LOWER_BOUND			0
#define RRM_CFG_DB_LOW_PRIORITY_BUCKET_2_UPPER_BOUND			999
#define RRM_CFG_DB_LOW_PRIORITY_BUCKET_3_LOWER_BOUND			0
#define RRM_CFG_DB_LOW_PRIORITY_BUCKET_3_UPPER_BOUND			999

#define RRM_CFG_DB_MAC_ECID_MEAS_START_SFN_LOWER_BOUND			0
#define RRM_CFG_DB_MAC_ECID_MEAS_START_SFN_UPPER_BOUND			255
#define RRM_CFG_DB_ACTIVE_UE_SUPPORT_TIMER_LOWER_BOUND			1
#define RRM_CFG_DB_ACTIVE_UE_SUPPORT_TIMER_UPPER_BOUND			255

/* BUG_11317_FIX_START */
#define RRM_CFG_DB_PUSCH_BLER_THRESHOLD_LOWER_BOUND             1
/* SPR 18641_18843 fix start */
#define RRM_CFG_DB_PUSCH_BLER_THRESHOLD_UPPER_BOUND             200
/* SPR 18641_18843 fix end */
#define RRM_CFG_DB_OLPC_PUSCH_SUPPORT_CATEGORY_ACTION_TRIGGER_LOWER_BOUND			0
#define RRM_CFG_DB_OLPC_PUSCH_SUPPORT_CATEGORY_ACTION_TRIGGER_UPPER_BOUND			2
#define RRM_CFG_DB_OLPC_PUSCH_SNR_THREHOLD_LOWER_BOUND			0
#define RRM_CFG_DB_OLPC_PUSCH_SNR_THREHOLD_UPPER_BOUND			255
#define RRM_CFG_DB_OLPC_PUSCH_RANGE_START_LOWER_BOUND			-8
#define RRM_CFG_DB_OLPC_PUSCH_RANGE_START_UPPER_BOUND			7
#define RRM_CFG_DB_OLPC_PUSCH_RANGE_END_LOWER_BOUND			-8
#define RRM_CFG_DB_OLPC_PUSCH_RANGE_END_UPPER_BOUND			7

#define RRM_CFG_DB_OLPC_PUCCH_SUPPORT_CATEGORY_ACTION_TRIGGER_LOWER_BOUND			0
#define RRM_CFG_DB_OLPC_PUCCH_SUPPORT_CATEGORY_ACTION_TRIGGER_UPPER_BOUND			2
#define RRM_CFG_DB_OLPC_PUCCH_SNR_THREHOLD_LOWER_BOUND			0
#define RRM_CFG_DB_OLPC_PUCCH_SNR_THREHOLD_UPPER_BOUND			255
#define RRM_CFG_DB_OLPC_PUCCH_RANGE_START_LOWER_BOUND			-8
#define RRM_CFG_DB_OLPC_PUCCH_RANGE_START_UPPER_BOUND			7
#define RRM_CFG_DB_OLPC_PUCCH_RANGE_END_LOWER_BOUND			-8
#define RRM_CFG_DB_OLPC_PUCCH_RANGE_END_UPPER_BOUND			7
/* BUG_11317_FIX_END */

/* Fix for CSR: 00059124 start */
#define RRM_CFG_CDRX_ON_DURATION_TIMER                   RRM_OAM_ON_DURATION_TIMER_PSF_2
#define RRM_CFG_CDRX_DRX_INACTIVITY_TIMER                RRM_OAM_DRX_INACTIVITY_TIMER_PSF_2
#define RRM_CFG_CDRX_DRX_RETRANSMISSION_TIMER            RRM_OAM_DRX_RETRANS_TIMER_PSF_2
#define RRM_CFG_CDRX_LONG_DRX_CYCLE                      RRM_OAM_SF20
/* coverity_<280099>_fix_start */
#ifdef ENDC_ENABLED
#define RRM_CFG_CDRX_LONG_DRX_CYCLE_R15                  RRM_OAM_R15_SF20
#endif
/* coverity_<280099>_fix_end */
#define RRM_CFG_CDRX_DRX_START_OFFSET                    5
#define RRM_CFG_CDRX_SHORT_DRX_CYCLE                     RRM_OAM_SHORT_DRX_CYCLE_SF_5
#define RRM_CFG_CDRX_DRX_SHORT_CYCLE_TIMER               4

#define RRM_UE_THROUGHPUT_ANR_THRESHOLD_LOWER_BOUND      0
#define RRM_UE_THROUGHPUT_ANR_THRESHOLD_UPPER_BOUND      255
/* Fix for CSR: 00059124 end */
/* SPR_12598_FIX_start */
#define RRM_CFG_DB_INITIAL_RI_PERIDIOCITY_1                 1   
#define RRM_CFG_DB_INITIAL_RI_PERIDIOCITY_2                 2
#define RRM_CFG_DB_INITIAL_RI_PERIDIOCITY_4                 4
#define RRM_CFG_DB_INITIAL_RI_PERIDIOCITY_8                 8
#define RRM_CFG_DB_INITIAL_RI_PERIDIOCITY_16                16
#define RRM_CFG_DB_INITIAL_RI_PERIDIOCITY_32                32
/* SPR_12598_FIX_end */

#endif /* _RRM_MIF_DB_H_ */
