/*! \file son_rach_opt_cell_fsm.h
 *  \brief This file contains function prototype and enums related to RACH OPT cell fsm.
 *  \date  Aug, 2013
 *  \author Shilpi
 */

/*!
 *  \copyright {Copyright (c) 2013, Aricent Inc. All Rights Reserved}
 */
#ifndef _SON_RACH_OPT_CELL_FSM_H_
#define _SON_RACH_OPT_CELL_FSM_H_

/*! \headerfile son_utils.h <>
 */
#include <son_utils.h>

/*! \headerfile son_rrm_intf.h <>
 */
#include <son_rrm_intf.h>

/*! \headerfile son_rach_opt_intf.h <>
 */
#include <son_rach_opt_intf.h>

/*! \headerfile son_rach_opt_ctxt_mgr.h <>
 */
#include <son_rach_opt_ctxt_mgr.h>


/* SPR-19168 START */
#define SON_MIN_NUM_OF_ROOT_SEQS                1

#define SON_MAX_NUM_OF_PCI_FOR_INTERFERENCE     3
#define SON_MAX_NUM_OF_ROOT_SEQS                838
#define SON_MAX_NUM_OF_PRACH_FREQ_OFFSETS       95
#define SON_MAX_NUM_OF_PRACH_CONFIG_IDX          64

/* SPR-18735 START */
#define SON_ROOT_SEQ_USED                       0x01
#define SON_ROOT_SEQ_NOT_USED                   0xFF
/* SPR-18735 END */

/* SPR-19168 START */
#define SON_PRACH_FREQ_OFFSET_USED              0x01
#define SON_PRACH_FREQ_OFFSET_NOT_USED          0xFF
/* SPR-19168 END */


/* SPR-19168 START */
#define SON_PRACH_CONFIG_IDX_USED               0x01
#define SON_PRACH_CONFIG_IDX_NOT_USED           0xFF
/* SPR-19168 END */


/* SPR-19168 START */
/* code removed */

#define SON_MAX_PRB_OFFSET_VALUE_6RB_CELL                  6 
#define SON_MAX_PRB_OFFSET_VALUE_15RB_CELL                 15
#define SON_MAX_PRB_OFFSET_VALUE_25RB_CELL                 25
#define SON_MAX_PRB_OFFSET_VALUE_50RB_CELL                 50
#define SON_MAX_PRB_OFFSET_VALUE_75RB_CELL                 75
#define SON_MAX_PRB_OFFSET_VALUE_100RB_CELL                100
/* SPR-19168 END */


#define SON_NUM_OF_CONSECUTIVE_PRB                         6

#define SON_TRANSMISSION_BW_6_RBS       0
#define SON_TRANSMISSION_BW_15_RBS      1
#define SON_TRANSMISSION_BW_25_RBS      2
#define SON_TRANSMISSION_BW_50_RBS      3
#define SON_TRANSMISSION_BW_75_RBS      4
#define SON_TRANSMISSION_BW_100_RBS     5
#define SON_MAX_NUM_OF_TRANSMISSION_BW  6


#define     NA_PREAMBLE_FORMAT                  0xFF
#define     NA_DENSITY_PER_10_MS_DRA            0xFF
#define     NA_VERSION_rRA                      0xFF
#define     TOTAL_NUM_OF_PREAMBLE_FORMAT        64


#define MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_0         19
#define MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_1         29
#define MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_2         39
#define MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_3         47
#define MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_4         57
#define MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_PREAMBLE_OFFSET_NA        63
#define MAXIMUM_PRACH_CONFIG_INDEX_VALUE_WITH_INVALID_PREAMBLE_FORMAT   0xFF



#define SON_RACH_MIN_PERCENTAGE_OF_CENTRE_N_EDGE_SAMPLES 40

/* SPR-19168 END */

/* SPR-19827 START */
#define SON_INVALID_TXID                     0xFFFFFFFF
/* SPR-19827 END */


/*! \def son_rach_opt_cell_index_et
 *  \brief An enum that specifies the index value in RACH OPT CELL FSM
 */
typedef enum son_rach_opt_cell_index_et
{
    RACH_OPT_CELL_IDX_SONRACH_OPT_BASE = 0,
    RACH_OPT_CELL_IDX_EVENT_ENABLE_REQ = RACH_OPT_CELL_IDX_SONRACH_OPT_BASE,
    RACH_OPT_CELL_IDX_EVENT_DISABLE_REQ,
    RACH_OPT_CELL_IDX_EVENT_MODIFY_CONFIG_PARAM_REQ,
    RACH_OPT_CELL_IDX_RRM_EVENT_RACH_CONFIG_RESP,
    RACH_OPT_CELL_IDX_RRM_EVENT_UE_INFO_IND,
    RACH_OPT_CELL_IDX_RRM_EVENT_L2_IND,
    RACH_OPT_CELL_IDX_RRM_EVENT_CELL_STATE_CHANGE_IND,
    RACH_OPT_CELL_IDX_SONANR_CELL_ANR_MODE_IND,
    RACH_OPT_CELL_NUM_OF_EVENTS,
    RACH_OPT_CELL_IDX_NOT_AVAILABLE  = 0xFF
}son_rach_opt_cell_index_et;


/*! \def son_rach_opt_inteference_val_et
 *  \brief An enum that specifies value getting tuned for iterference
 */
typedef enum son_rach_opt_inteference_val_et
{
    RACH_OPT_INTERFERENCE_UPDATE_ROOT_SEQ = 0,
    RACH_OPT_INTERFERENCE_UPDATE_PRACH_FREQ_OFFSET,
    RACH_OPT_INTERFERENCE_UPDATE_PRACH_CONFIG_IDX,
    RACH_OPT_INTERFERENCE_NUM_OF_UPDATES,
    RACH_OPT_INTERFERENCE_UPDATE_NOT_AVAILABLE = 0xFF
}son_rach_opt_inteference_val_et;

/* SPR-18735 START */

/*! \fn son_u8 son_rach_opt_get_number_of_root_seq_index( 
 * son_duplex_mode_et serving_cell_duplex_mode,
 * son_prach_configuration_t prach_configuration)
 * \brief This function shall find the number of root sequence indexes
 * derived using prach_configuration_index,
 * zero_correlation_zone_config and high_speed_flag.
 * \param serving_cell_duplex_mode   Duplex Mode of Serving Cell
 * \param prach_configuration        prach_configuration 
 */
son_u8      
son_rach_opt_get_number_of_root_seq_index
(           
 son_duplex_mode_et                   serving_cell_duplex_mode,
 son_prach_configuration_t            prach_configuration 
);   

/*! \fn son_u8 son_rach_opt_compare_root_seq_indx( 
 * son_u16 serv_root_seq_index_start, son_u16 nbr_root_seq_index_start,
 * son_u8 num_serv_root_seq_indexes, son_u8 num_nbr_root_seq_indexes
 * \brief This function shall compare the serving cell's root_sequence_index with
 * that of neighbor's root_sequence_index for equality.
 * \param serv_root_seq_index   Starting root_sequence_index of serving cell
 * \param neigh_root_seq_index  Starting root_sequence_index of neighbor cell
 * \param num_serv_root_seq_indexes Number of root sequence indexes for serving cell 
 * \param num_nbr_root_seq_indexes  Number of root sequence indexes for neighboring cell
 */
son_u8
son_rach_opt_compare_root_seq_indx
(
 son_u16 serv_root_seq_index_start,
 son_u16 nbr_root_seq_index_start,
 son_u8 num_serv_root_seq_indexes,
 son_u8 num_nbr_root_seq_indexes
);

/*! \  struct rach_opt_ncs_mapping_format_0_3
 *  \brief Structure for storing Ncs values in preamble generation for formats 0-3 
 *  \param zczc_val             zeroCorrelationZoneConfig
 *  \param ncs_val_unrestricted Ncs Values for unrestricted set
 *  \param ncs_val_restricted   Ncs Values for restricted set
 */
typedef struct _rach_opt_ncs_mapping_format_0_3
{       
    son_u8               zczc_val;
    son_u16              ncs_val_unrestricted;
    son_u16              ncs_val_restricted;
}rach_opt_ncs_mapping_format_0_3;

/*! \  struct rach_opt_ncs_mapping_format_0_4
 *  \brief Structure for storing Ncs values in preamble generation for formats 4
 *  \param zczc_val             zeroCorrelationZoneConfig
 *  \param ncs_val              Ncs Values for both restricted and unrestricted sets
 */
typedef struct _rach_opt_ncs_mapping_format_4
{       
    son_u8              zczc_val;
    son_u16              ncs_val;
}rach_opt_ncs_mapping_format_4;
/* SPR-18735 END */


/*! \fn son_void_t rach_opt_cell_process_msg(STACKAPIHDR *p_cspl_hdr,
 * rach_opt_cell_context_t *p_cell_ctxt, son_u8 *p_rach_opt_msg)
 * \brief This is the entry point to the RACH OPT Cell FSM. This function takes
 *         the API message received, CSPL header, cell context and calls
 *         the required event handler function.
 * \param p_cspl_hdr        Pointer to cspl header
 * \param p_cell_ctxt       Pointer to cell context
 * \param p_rach_opt_msg    Pointer to RACH OPT specific message
 */
son_void_t
rach_opt_cell_process_msg
(
    STACKAPIHDR             *p_cspl_hdr,
    rach_opt_cell_context_t *p_cell_ctxt,
    son_u8                  *p_rach_opt_msg
);

#endif /* ifndef _SON_RACH_OPT_CELL_FSM_H_ */
