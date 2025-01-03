/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: snow3g_ia.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 * File Description : Functions to calculate integrity using snow3g (EIA1) algo 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: snow3g_ia.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.2.2.1  2009/06/09 11:28:26  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/14 12:38:01  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _SNOW3G_IA_DEF_
#define _SNOW3G_IA_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "in_lteFramework.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

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

/****************************************************************************
 * Function Name  : snow3g_eia_ZCB
 * Inputs         : UInt8* key - 128 bit Integrity Key
 *					UInt16 bearer - same as lcId
 *					UInt32 count
 *					UInt16 dir - direction, 0(downlink) or 1(uplink)
 *					void *data_ZCB - ZCB with data to calc MACI for
 *					UInt32 byte_length - length of data in data_ZCB to calc MACI
 * Outputs        : UInt8 *res_MAC_I - calculated MACI will be placed here
 * Returns        : None
 * Description    : Calculates 32-bit MACI using snow3g (EIA1) algo
 ****************************************************************************/
void snow3g_eia_ZCB(UInt8* key, UInt16 bearer, UInt32 count, UInt16 dir,
         void *data_ZCB, UInt32 byte_length, UInt8 *res_MAC_I);

#endif
