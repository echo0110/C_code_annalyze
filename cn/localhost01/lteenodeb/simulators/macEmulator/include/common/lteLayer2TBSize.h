/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2TBSize.h,v 1.1.1.1.16.1 2010/09/21 15:53:15 gur20491 Exp $
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
 * $Log: lteLayer2TBSize.h,v $
 * Revision 1.1.1.1.16.1  2010/09/21 15:53:15  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1  2009/03/30 10:20:16  gur19836
 * Initial Version
 *
 * Revision 1.4  2008/12/11 13:51:37  gur11912
 * Downlink Phase 2 changes
 *
 * Revision 1.3  2008/09/27 15:17:38  gur11912
 * UT Defect Fixed
 *
 * Revision 1.2  2008/09/19 07:10:22  gur11912
 * review comments incorporated
 *
 * Revision 1.1  2008/09/04 06:22:57  gur11912
 * Initial Version
 *
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_TBSIZE_H
#define INCLUDED_TBSIZE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2Types.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/


/****************************************************************************
 * Exported Constants
 ****************************************************************************/
#define MAX_MCS_INDEX  29
#define MAX_IBS_INDEX 27
#define MAX_RESOURCE_BLOCK 110
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
 
/****************************************************************************
 * Exported Functions
 ****************************************************************************/

UInt32 determineTBSize(SInt8 mcsIndex, UInt8 nbRB);

#endif  /* TBSIZE_H */