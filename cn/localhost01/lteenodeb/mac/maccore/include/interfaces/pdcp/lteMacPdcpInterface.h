
/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPdcpInterface.h,v  $
 *
 ****************************************************************************
 *
 *  File Description : The file contains the definitions for SPS scheduler.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef _LIB_MAC_PDCP_INTERFACE_H_
#define _LIB_MAC_PDCP_INTERFACE_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/* SPS state events to be sent to PDCP in case of SPS via RLC */
typedef enum PdcpSpsStateEventRxEnumT
{
    PDCP_MAC_SPS_RRC_CONFIG = 0,
    PDCP_MAC_SPS_CONFIGURE,   /*1*/
    PDCP_MAC_SPS_ACTIVATED,   /*2*/
    PDCP_MAC_SPS_DEACTIVATED, /*3*/

    PDCP_MAC_MAX_SPS_RX_EVENT,
    PDCP_MAC_INVALID_SPS_RX_EVENT
}PdcpSpsStateEventRxEnum;

/****************************************************************************
 * Function Name  : macUeSpsVoiceSilenceDetection
 * Inputs         : ueIndex, lcId, voiceSilenceDetect, spsState
 * Outputs        : None.
 * Returns        : void
 * Description    : The function sends indication to MAC to activate or 
 *                  deactivate based on voice or silence.
 ****************************************************************************/
void macUeSpsVoiceSilenceDetection( 
    UInt16 ueIndex,
    /* +- SPR 17777*/
    UInt8 voiceSilenceDetect, 
    UInt8 spsState,
    InternalCellIndex internalCellId );

/****************************************************************************
 * Function Name  : macUeSpsAverageSdu
 * Inputs         : ueIndex, lcId, averageSduSize
 * Outputs        : None.
 * Returns        : Success
 * Description    : The function sends average RLC SDU size  to MAC 
 ****************************************************************************/
/* +- SPR 17777*/
void macUeSpsAverageSdu(void); 
/* +- SPR 17777*/

#endif
