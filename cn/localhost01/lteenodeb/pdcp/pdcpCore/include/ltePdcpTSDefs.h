/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpTSDefs.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : PDCP definitions according to TS 3GPP TS 36.323 V8.4.0
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpTSDefs.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
 *
 * Revision 1.4  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef __LIB_PDCP_TS_DEF__
#define __LIB_PDCP_TS_DEF__

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/*SN size in bits*/
#define PDCP_TS_SRB_SN_SIZE			5
#define PDCP_TS_DRB_SN_SIZE_SMALL	7
#define PDCP_TS_DRB_SN_SIZE_BIG		12

/*PDU SN masks*/
#define PDCP_TS_PDU_SRB_SN_MASK			0x1F
#define PDCP_TS_PDU_DRB_SN_SMALL_MASK	0x7F
#define PDCP_TS_PDU_DRB_SN_BIG_MASK_HI	0xF

/* SPR 3811 Changes Start */
/*PDU Reserved Bit masks*/
#define PDCP_TS_PDU_DL_DRB_RESV_BIT_MASK	0x8F
/* SPR 3811 Changes End */

/*d/c DRB PDU Header bit values*/
#define PDCP_TS_PDU_D_C_DATA_PDU	    1
#define PDCP_TS_PDU_D_C_CONTROL_PDU	    0

#define PDCP_TS_PDU_SET_D_C(data, d_c)	( data |= d_c << 7 )
#define PDCP_TS_PDU_GET_D_C(data)	    ( (data >> 7) & 1 )

/* allowed values for pduType header field*/
#define PDCP_TS_PDU_TYPE_STATUS_REPORT    0
#define PDCP_TS_PDU_TYPE_ROHC_FEEDBACK    1
#define PDCP_TS_PDU_TYPE_RESERVED1        2
#define PDCP_TS_PDU_TYPE_RESERVED2        3
#define PDCP_TS_PDU_TYPE_RESERVED3        4
#define PDCP_TS_PDU_TYPE_RESERVED4        5
#define PDCP_TS_PDU_TYPE_RESERVED5        6
#define PDCP_TS_PDU_TYPE_RESERVED6        7

/* Reordering_Window constant*/
#define PDCP_TS_REORDERING_WINDOW     2048

/* The maximum supported size of a PDCP SDU is 8188 octets. 3GPP TS 36.323 V8.5.0 (2009-03)*/
#define PDCP_TS_SDU_MAX_SIZE            8188 

/* Maximum SN value in AM mode */
#define PDCP_TS_MAX_SN_AM_MODE     4095

/* AM mode Masking For SN */
#define PDCP_SN_MASK_BIT 0xFFF

#define PDCP_ROHC_SIZE_ENTITY_POOL 100
#define PDCP_ROHC_SIZE_CSC_POOL  100
#define PDCP_ROHC_SIZE_DSC_POOL  100


/* Discard Timer values (discardTimer field) */
enum PdcpDiscardTimerValueT
{
    Infinity 	= 0,
    ms50 	= 50,
    ms100 	= 100,
    ms150 	= 150,
    ms300 	= 300,
    ms500 	= 500,
    ms750 	= 750,
    ms1500 	= 1500,
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

#endif  /*__LIB_PDCP_TS_DEF__*/
