/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcArithmetic.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:29 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *                  This file contains macro defintions to help in arithmetic
 *                  operations.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 *
 *
 ****************************************************************************/
#ifndef ERROR_CODE_H
#define ERROR_CODE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcGlobalData.h"

/****************************************************************************
 * Definitions
 ****************************************************************************/

 #define    INVALID_LENGTH 264
 #define    TRUE            1  
 #define    FALSE           0
 #define    ZERO            0


typedef enum RlcErrorCodeT
{
    INVALID_ERR_CODE = 1,

    RLC_SOCKET_RX_MEM_POOL_ERR,
    RLC_SOCKET_ERROR,
    RLC_SOCKET_RX_INVALID_MSG_LEN,
    RLC_SOCKET_INVALID_DEST_ID,
    RLC_SOCKET_LAYER_NOT_INITIALIZED,
    RLC_SOCKET_INVALID_SRC_MODULE_ID,
    RLC_OAM_RX_MEM_POOL_ERR,
    RLC_OAM_RX_INVALID_MSG_ID,
    RLC_OAM_LAYER_ALREADY_INITIALIZED, /*10*/
    RLC_OAM_INVALID_OAM_INIT_REQ_LEN,
    RLC_OAM_INVALID_RNTI_RANGE,
    RLC_OAM_INVALID_SUPPORTED_USER,
    RLC_OAM_INVALID_CLEANUP_LAYER_REQ_LEN,
    RLC_OAM_FILE_NOT_EXIST,   
    RLC_OAM_FILE_INCORRECT,
    RLC_RESET_INVALID_STATS_REQ_LEN,
    RLC_OAM_INVALID_GET_STATS_REQ_LEN,
   /* RLC RRC Interface */
    RLC_RRC_RX_INVALID_MSG_LEN,
    RLC_RRC_INVALID_RRC_MSG_ID,/*20*/
    RLC_TXRX_ENTITY_NOT_EXIST,
    RLC_TX_ENTITY_NOT_EXIST,
    RRC_MSG_LEN_INVALID ,
    INVALID_RRC_MSG_ID ,
    INVALID_CMM_MSG_ID,
    RLC_INVALID_LC_ID,
    MSG_POOL_ERR ,
    INVALID_MSG_LEN,
    DEST_MODULE_ID_INVALID,
    INVALID_ENTITY_TYPE   ,/*30*/
    RLC_STATUS_MEM_ALLOC_FAIL,
    INVALID_RNTI_RANGE,
    SUPPORTED_USER_INALID, 
    INVALID_RLC_OAM_INIT_REQ_LEN,
    OUT_OF_WINDOW,
    STATUS_PDU_SEQ,
    LTE_RLC_ERR_INVALID_LCID,
    LTE_RLC_ERR_INVALID_ENTITY,
    LTE_RLC_ERR_INVALID_MAC_OPP,
    LTE_RLC_ERR_INVALID_TX_ENTITY, /*40*/
    LTE_RLC_ERR_INVALID_RX_ENTITY,
    LTE_RLC_UE_OPP_IND_NULL,
    LTE_RLC_PUSH_DUMMY_NODE,
    RLC_RX_DISCARD_DUPLICATE_PDU,
    RLC_RX_DISCARD_OUT_OF_WINDOW,
    RLC_RX_INVALID_PDU,
    RLC_RX_WITH_IN_RE_ORDERING_WINDOW,
    RLC_RX_OUTSIDE_RE_ORDERING_WINDOW,
    RLC_INSUFF_SIZE_FOR_ENCODING,
    RLC_MSG_ALLOC_FAIL, /*50*/
    RLC_MSG_SPLIT_FAIL,
    RLC_TIMER_SP_NODE_ALREADY_EXIST,
    RLC_TIMER_SP_IDX_ALREADY_EXIST,
    RLC_TIMER_RO_NODE_ALREADY_EXIST,
    RLC_TIMER_RO_IDX_ALREADY_EXIST,
    RLC_TIMER_ERR_INVALID_SP_TIMER_IDX,
    RLC_TIMER_ERR_TIMER_NODE_NOT_EXIST,
    LTE_RLC_ERR_DB_NOTFOUND,
    RLC_TIMER_ERR_RX_AM_ENTITY_NOT_EXIST,
    RLC_NULL_PTR_RECVD, /*60*/
    RLC_UE_CTX_NOT_EXIST,
    RLC_ENTITY_NOT_EXIST,
    RLC_AM_ENTITY_NOT_EXIST,
    RLC_UM_ENTITY_NOT_EXIST,
    RLC_DECODE_INVALID_SEG_LEN,
    RLC_DECODE_MEM_ALLOC_FAIL,
    RLC_DECODE_MSG_REMOVE_ERR,
    RLC_TIMER_EXPIRY_INVALID,
    RLC_RX_INVALID_NULL_PDU,
    RLC_RX_UM_NOT_FOUND_SDU,/*70*/
    RLC_RX_ZERO_SIZE_PDU,
    RLC_RX_NULL_PDU,
    RLC_RX_MSZ_SIZE_NOT_EQUAL,
    RLC_RX_INVALID_LCID,
    RLC_RX_ENTITY_NOT_EXIST,
    RLC_RX_ENTITY_NOT_IN_ACTIVE_MODE,
    RLC_RX_UM_ENTITY_NOT_FOUND,
    RLC_RX_AM_ENTITY_NOT_FOUND,
    RLC_TX_CTX_NOT_FOUND,
    RLC_ENTITY_NOT_ACTIVE_MODE, /*80*/
    LTE_RLC_UE_CTX_NOT_FOUND,
    LTE_RLC_INSUFF_SIZE_FOR_ENCODING,
    LTE_RLC_MSG_SPLIT_FAIL,
    LTE_RLC_NO_DATA_TO_ENCODE,
    LTE_RLC_MSG_RESERVE_FAIL,
    LTE_RLC_MEM_ALLOC_FAIL,
    LTE_RLC_MSG_ALLOC_FAILURE,
    LTE_RLC_ENCODING_SDU_SEG_FAIL,
    LTE_RLC_UM_MAX_SDU_REVD,
    LTE_RLC_INALID_SPS_PARAM,
    /* Start 128 UE: 25% NACK Changes */
    LTE_RLC_VALIDATION_FAILED_SN_OUTSIDE_TX_WINDOW,
    /* End 128 UE: 25% NACK Changes */
    /* SPR 7587 Changes Start */
    LTE_RLC_ENCODING_FAIL,
    /* SPR 7587 Changes End */
    /* + Layer2 NON CA Changes */
    LTE_RLC_INVALID_CELL_COUNT_RANGE,
    LTE_RLC_INVALID_CELL_INDEX,
    LTE_RLC_INVALID_SCELL_PARAM
    /* - Layer2 NON CA Changes */
    
}RlcErrorCode;
/****************************************************************************
 * Exported Functions
 ****************************************************************************/


#endif
