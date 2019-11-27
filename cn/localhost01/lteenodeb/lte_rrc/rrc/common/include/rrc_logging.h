/******************************************************************************
*
*   FILE NAME:
*       rrc_logging.h
*
*   DESCRIPTION:
*       This file contains macros and function prototypes which are used in
*       S1AP, X2AP and RRC as well.
*
*   DATE            AUTHOR             REFERENCE       REASON
*   16 Nov 2012     Praneet Gupta      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_LOGGING_H_
#define _RRC_LOGGING_H_

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "rrc_defines.h"
#include "x2ap_asn_enc_dec_3gpp.h"
#include "s1ap_asn_enc_dec_3gpp.h"
#include "rrc_asn_enc_dec.h"
#include "rrc_oam_intf.h"

/****************************************************************************
 * Macros
 ****************************************************************************/

#define RRC_EVENT_TRACE   RRC_TRACE
#define  RRC_ZERO                     0
#define  MAX_LOG_FILE_NUMBER          10
#define  MAX_CHUNK_COUNT              100
#define  RRC_SHM_LOG_READER_TIMER_VALUE  2000
#define  RRC_MKDIR                    mkdir


/****************************************************************************
 * Macro to log ASN hex dump
 ****************************************************************************/
#define RRC_LOG_ASN_HEX_DUMP(msgName, subName, msgBuf, msgLength) 


typedef enum
{
    S1AP_SUCCESS,
    S1AP_PROTOCOL_ERROR,
    S1AP_FAILURE,
    S1AP_TIMEOUT,
    GUMMEI_CONFLICT_PROTOCOL_ERROR,
    MAX_GUGI_EXCEEDED,
    SCTP_SHUTDOWN
}s1ap_setup_result_et;

typedef enum
{
    EVENT_VALUE_RRC_ASN1_DECODE_ERROR=0,
    EVENT_VALUE_UNKNOWN_MESSAGE_TYPE=1,
    EVENT_VALUE_UNKNOWN_IE_VALUE=2,
    EVENT_VALUE_MISSING_EXPECTED_CONDITIONAL_IE=3,
    EVENT_VALUE_RRC_LOGICAL_ERROR=4
}rrc_local_error_type_et;

typedef enum
{
    RRC_LOGGING_READ_TIMER
}rrc_logging_timer_id_et;

typedef struct 
{
    /* It identifies the timer type */
    rrc_logging_timer_id_et timer_type;
    /* If true timer buff is freed by timer handler */
    rrc_bool_et             auto_delete;
} rrc_logging_timer_buf_t;

typedef struct
{
    /* OAMH shared memory configuration */
    rrc_shared_mem_config_t  shared_mem_cfg;
    /* Log reading timer duration/health monitoring timer duration */
    rrc_timer_duration_t    int_log_read_duration;
} rrc_logging_config_t;

typedef struct
{
    /*Logging read timer */
    rrc_timer_t             init_log_read_timer;
    /*OAMH shared memory configuration*/
    rrc_logging_config_t    reader_config;
} rrc_logging_gl_ctx_t;

typedef enum
{
    RRC_LOGGER_FAILURE,
    RRC_LOGGER_SUCCESS
}rrc_logger_et;

/****************************************************************************
 * Public functions definitions
 ****************************************************************************/

/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_S1AP_PDU
*
*   DESCRIPTION:
*       This function prints ASN S1AP_PDUs using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/
void rrc_asn1PrtToStr_S1AP_PDU(
        U32              log_level,
        const char*      name,
        S1AP_PDU*        pvalue); /* Pointer to s1ap pdu */
        


/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_X2AP_PDU
*
*   DESCRIPTION:
*       This function prints ASN X2AP_PDUs using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/
void rrc_asn1PrtToStr_X2AP_PDU(
                            U32              log_level,
                            const char*      name,
                            x2ap_X2AP_PDU*   pvalue); /* Pointer to x2ap pdu */
                            

/******************************************************************************
  *   FUNCTION NAME: rrc_asn1PrtToStr_handover_prep
  *
  *   DESCRIPTION:
  *       This function prints ASN X2AP_PDUs using RRC logging mechanism.
  *
  *   RETURNS:
  *       void
  *
  *****************************************************************************/

void rrc_asn1PrtToStr_handover_preparation(
        U32                             log_level,
        const char*                     name,
        HandoverPreparationInformation* pvalue,
        const char*                     msgName,
        U8*                             p_buff,
        U16                             msgLen);

/******************************************************************************
  *   FUNCTION NAME: rrc_asn1PrtToStr_handover_command
  *
  *   DESCRIPTION:
  *       This function prints ASN X2AP_PDUs using RRC logging mechanism.
  *
  *   RETURNS:
  *       void
  *****************************************************************************/

void rrc_asn1PrtToStr_handover_command(
        U32                 log_level,
        const char*         name,
        HandoverCommand*    pvalue,
        const char*         msgName,
        U8*                 p_buff,
        U16                 msgLen);

/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_DL_CCCH_Message
*
*   DESCRIPTION:
*       This function prints ASN DL_CCCH_Message using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/

void rrc_asn1PrtToStr_DL_CCCH_Message(
       U32                 log_level,
       const char*         name,
       DL_CCCH_Message*    pvalue,
       const char*         msgName,
       U8*                 p_buff,
       U16                 msgLen);

/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_UL_CCCH_Message
*
*   DESCRIPTION:
*       This function prints ASN UL_CCCH_Message using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/

void rrc_asn1PrtToStr_UL_CCCH_Message( U32                 log_level,
                                       const char*         name,
                                       UL_CCCH_Message*    pvalue,
                                       const char*         msgName,
                                       U8*                 p_buff,
                                       U16                 msgLen);

/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_DL_DCCH_Message
*
*   DESCRIPTION:
*       This function prints ASN DL_DCCH_Message using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/

void rrc_asn1PrtToStr_DL_DCCH_Message( U32                 log_level,
                                       const char*         name,
                                       DL_DCCH_Message*    pvalue,
                                       const char*         msgName,
                                       U8*                 p_buff,
                                       U16                 msgLen);


/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_UL_DCCH_Message
*
*   DESCRIPTION:
*       This function prints ASN UL_DCCH_Message using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/
void rrc_asn1PrtToStr_UL_DCCH_Message( U32                 log_level,
                                       const char*         name,
                                       UL_DCCH_Message*    pvalue,
                                       const char*         msgName,
                                       U8*                 p_buff,
                                       U16                 msgLen);


/******************************************************************************
*   FUNCTION NAME: rrc_asn1PrtToStr_InitialUE_Identity_randomValue 
*
*   DESCRIPTION:
*       This function prints ASN Initial UE Identity using RRC logging mechanism.
*
*   RETURNS:
*       void
*
*****************************************************************************/
void rrc_asn1PrtToStr_InitialUE_Identity_randomValue(
        U32                                    log_level,
        const char*                            name,
        InitialUE_Identity_randomValue*        pvalue,
        const char*                            msgName,
        U8*                                    p_buff,
        U16                                    msgLen);


#endif /* _RRC_LOGGING_H_ */
