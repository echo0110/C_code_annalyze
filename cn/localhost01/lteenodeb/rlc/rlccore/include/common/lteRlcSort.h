/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcSort.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:38 gur20491 Exp $
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
 * $Log: lteRlcSort.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:38  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.2.28.1  2010/07/17 11:35:26  gur23054
 * Review Comments fixed
 *
 * Revision 1.2  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef RLCSORT_H
#define RLCSORT_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteRlcTypes.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
void rlcSwap(UInt32 *aL,UInt32 *aR);

UInt32 rlcPartition( UInt32 *a, UInt32 low, UInt32 high );

void rlcQuicksort( UInt32 *a, UInt32 low, UInt32 high ) ;

void rlcFnSortHeap(UInt32 *arr, UInt32 arr_ubound) ;

void rlcHeapsort(UInt32 *arr,UInt32 arr_num_items) ;

UInt32 rlcCompare( void *arg, void *data) ; 

#endif  /* INCLUDED_FILENAME_H */
