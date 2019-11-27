/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUmdRing.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:50:00 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcUmdRing.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:50:00  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:16:37  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.3.28.1  2010/07/17 11:41:05  gur23054
 * Review Comments fixed
 *
 * Revision 1.3  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef UMDRING_H
#define UMDRING_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcRing.h"
#include "lteRlcUmdPdu.h"
#include "lteRlcSnList.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define RING_SIZE_UMD_PDU_RN(UmdPduRing_p) \
    GET_RING_SIZE((UmdPduRing_p)->pR)

/* SPR 3547 - 3546 Fix Start */
#define GET_RING_SS_UMD_PDU_RN(UmdPduRing_p,ui32_ringNodeId,modValForWrapAround) \
    GET_RING_SS((UmdPduRing_p)->pR,(ui32_ringNodeId & modValForWrapAround))
/* SPR 3547 -3546 Fix End */

#define SET_RING_SS_UMD_PDU_RN(UmdPduRing_p,ui32_ringNodeId,en_ringSS) \
    SET_RING_SS((UmdPduRing_p)->pR,(ui32_ringNodeId),(en_ringSS))

#define GET_RING_DATA_UMD_PDU_RN(UmdPduRing_p,ui32_ringNodeId) \
    (UmdPdu *)GET_RING_DATA((UmdPduRing_p)->pR,(ui32_ringNodeId))

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* UmdPduRing is circular list of RingNodes */

typedef struct UmdPduRingT {
    Ring * pR ;
} UmdPduRing ;
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/* Below is list of ring UmdPduR operations */
/* SPR 5599_129119 Fix Start */
int rlcRingInitUmdPduR(UmdPduRing *pR,RingSz sz) ;
void rlcChangeSizeUmdPduR(UmdPduRing *pR,RingSz sz);
/* SPR 5599_129119 Fix End */


#endif  /* UMDRING_H */
