/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpMacSap.h,v $
 *
 ****************************************************************************
 *
 *  File Description : Interface definition for interaction beetwen the PDCP
 *                     layer and MAC layer (MAC SAP).
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef _LIB_MAC_SAP_DEF_
#define _LIB_MAC_SAP_DEF_

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
 * Function Name  : macUeSpsVoiceSilenceDetection
 * Inputs         : ueIndex, voiceSilenceDetect, spsState, intCellIdx
 * Outputs        : None.
 * Returns        : void
 * Description    : The function sends indication to MAC to activate or 
 *                  deactivate based on voice or silence.
 ****************************************************************************/
void macUeSpsVoiceSilenceDetection( 
    UInt16 ueIndex, 
    /* +- SPR 17777*/
    UInt8  voiceSilenceDetect,
    UInt8  spsState, 
    InternalCellIndex intCellIdx );

/****************************************************************************
 * Function Name  : macUeSpsAverageSdu
 * Inputs         : ueIndex, lcId, averageSduSize
 * Outputs        : None.
 * Returns        : Success
 * Description    : The function sends average RLC SDU size to MAC in UL. 
 ****************************************************************************/
/* +- SPR 17777*/
void macUeSpsAverageSdu(void);
/* +- SPR 17777*/

#endif /* _LIB_MAC_SAP_DEF_ */