/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcAmdRing.h,v 1.1.6.2.2.1 2010/09/21 15:49:27 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *                  This file declares the Ring operations of Am Entity
 *
 ****************************************************************************
 *  Revision Details
 * ------------------
 *
 *  $Log: lteRlcAmdRing.h,v $
 *  Revision 1.1.6.2.2.1  2010/09/21 15:49:27  gur20491
 *  FAPI changes
 *
 *  Revision 1.1.2.1  2010/08/10 09:37:45  gur19836
 *  Files added for RLC AM Release
 *
 *  Revision 1.1.6.2  2010/07/17 11:33:42  gur23054
 *  Review Comments fixed
 *
 *  Revision 1.1.6.1  2010/06/17 10:59:39  gur19836
 *  RLC AM Base Code Merged
 *
 *  Revision 1.1.2.3  2009/11/10 09:06:48  gur19140
 *  Minor changes
 *
 *  Revision 1.1.2.2  2009/10/28 09:01:42  gur19140
 *  Minor changes for file headers
 *
 *
 *
 ****************************************************************************/

#ifndef AMDRING_H
#define AMDRING_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteRlcRing.h"
#include "lteRlcAmdPdu.h"
#include "lteRlcSnList.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define RING_SIZE_AMD_PDU_RN(AmdPduRing_p) \
    GET_RING_SIZE((AmdPduRing_p)->pR)

#define GET_RING_SS_AMD_PDU_RN(AmdPduRing_p,ui32_ringNodeId) \
    GET_RING_SS((AmdPduRing_p)->pR,(ui32_ringNodeId))

#define SET_RING_SS_AMD_PDU_RN(AmdPduRing_p,ui32_ringNodeId,en_ringSS) \
    SET_RING_SS((AmdPduRing_p)->pR,(ui32_ringNodeId),(en_ringSS))

#define GET_RING_DATA_AMD_PDU_RN(AmdPduRing_p,ui32_ringNodeId) \
    (AmdPdu *)GET_RING_DATA((AmdPduRing_p)->pR,(ui32_ringNodeId))

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/* AmdPduRing is circular list of RingNodes */

typedef struct AmdPduRingT {
    Ring * pR ;
} AmdPduRing ;
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* Below is list of ring AmdPduR operations */
void rlcRingInitAmdPduR
(
    AmdPduRing *pR,
    RingSz sz
);
void rlcChangeSizeAmdPduR
(
    AmdPduRing *pR,
    RingSz sz
);
/* Cyclomatic Complexity changes - starts here */
 void rlcFreeAmdPduSegment(AmdPduSegment *amdPduSeg_p);
/* Cyclomatic Complexity changes - ends here */

#endif  /* AMDRING_H */
