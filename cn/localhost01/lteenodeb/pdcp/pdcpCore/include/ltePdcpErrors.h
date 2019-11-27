/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpErrors.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : PDCP layer error codes definitions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpErrors.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:17  gur20548
 * Changed header file name
 *
 * Revision 1.3  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_PDCP_ERRORS_DEF_
#define _LIB_PDCP_ERRORS_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* General response codes */ 
#define PDCP_FAIL               0
#define PDCP_SUCCESS            1
#define PDCP_PARTIAL_SUCCESS    2


/* PDCP errors groups */
#define PDCP_ERR_SYSTEM_GROUP		0x0100
#define PDCP_ERR_CONTEXT_GROUP		0x0200
#define PDCP_ERR_TLV_PARSING_GROUP	0x0300
#define PDCP_ERR_DATA_TRANSFER      0x0400

#ifdef MAC_AUT_TEST
#define MEM_ALLOC_AUT(bufferLen)\
    (UInt8 *)getMemFromPool(bufferLen,PNULL);

#define MEM_COPY_AUT(dest_p,src_p,size)\
    memCpy(dest_p,src_p,size);
#endif

/* PDCP general systaem errors */
enum PdcpSystemErrorsT
{
    PDCP_ERR_SYSTEM_MEMORY_ALLOCATION = PDCP_ERR_SYSTEM_GROUP,
    PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER, 
    PDCP_ERR_FILE_IO,
    
    PDCP_ERR_SYSTEM_LAST
}; 

/* PDCP Context management errors */
enum PdcpContextErrorsT
{
    PDCP_ERR_CONTEXT_NOT_INITIALIZED    = PDCP_ERR_CONTEXT_GROUP,
    PDCP_ERR_CONTEXT_ALREADY_INITIALIZED,
    PDCP_ERR_UE_CONTEXT_ALREADY_CREATED,
    PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED,
    PDCP_ERR_ENTITY_ALREADY_CREATED,
	PDCP_ERR_ENTITY_WRONG_DIRECTION,
	PDCP_ERR_ENTITY_WRONG_TYPE,
    PDCP_ERR_ENTITY_NOT_FOUND,
    PDCP_ERR_ENTITY_SET_SN_SIZE,
    PDCP_ERR_ENTITY_SET_STATUS_REPORT_REQUIRED,

    PDCP_ERR_UE_DELETE_DATA_FWDING,
    /* Lossy HO changes start */
    PDCP_ERR_UE_LOSSY_FOR_AM_RECVD,
    /* Lossy HO changes stop */

    PDCP_ERR_CONTEXT_LAST
}; 

/* PDCP TLV parsing errors */
enum PdcpTlvParsingErrorsT
{
    PDCP_ERR_TLV_PARSING_INVALID_LENGTH   = PDCP_ERR_TLV_PARSING_GROUP,
    PDCP_ERR_TLV_PARSING_INVALID_UE_ID,
    PDCP_ERR_TLV_PARSING_INVALID_OPTIONAL_PARAMETERS_LENGTH,
    PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH,

    PDCP_ERR_TLV_PARSING_INVALID_LC_ID,
    PDCP_ERR_TLV_PARSING_INVALID_RB_ID,
    PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE,

    PDCP_ERR_TLV_PARSING_INVALID_CRNTI,
    PDCP_ERR_TLV_PARSING_INVALID_TAG_ID,

    PDCP_ERR_TLV_PARSING_INVALID_RNTI_RANGE,
    PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM,
    PDCP_ERR_TLV_PARSING_INVALID_API_ID,
    /* CA Changes Start */
    PDCP_ERR_TLV_PARSING_INVALID_CELL_INDEX,
    /* CA Changes Start */
    /* + SPR 19066 */
    PDCP_ERR_TLV_PARSING_INVALID_INACT_TIME_VAL,
    /* - SPR 19066 */
    PDCP_ERR_TLV_PARSING_LAST
}; 

/* PDCP data transfer fails */
enum PdcpDataTransferErrorsT
{
    PDCP_ERR_DT_INTEGRITY_FAIL = PDCP_ERR_DATA_TRANSFER,
	PDCP_ERR_DT_ROHC_COMPRESS_FAIL,
    PDCP_ERR_DT_RLC_NACK,
    PDCP_ERR_DT_DISCARD_TIMER_EXPIRED,
    /* SPR 3538 changes start */
    PDCP_ERR_INVALID_DATA_SIZE,
    /* SPR 3538 changes end */
    
    PDCP_ERR_DT_LAST
}; 

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif /* _LIB_PDCP_ERRORS_DEF_ */
