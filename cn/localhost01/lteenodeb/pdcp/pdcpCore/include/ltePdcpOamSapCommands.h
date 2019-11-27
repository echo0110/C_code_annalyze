/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpOamSapCommands.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : OAM API message handlers definitions
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpOamSapCommands.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
 *
 * Revision 1.3  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_I_OAM_SAP_COMMANDS_DEF_
#define _LIB_I_OAM_SAP_COMMANDS_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include "ltePdcpContext.h"
#include "ltePdcpOamInterface.h"

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
extern TlvCommandTable oamCmdTable_g[];

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : oamCmdDefaultHandler
 * Inputs         : data_p - Pointer to TLV API message buffer
                    size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes unknown OAM API.
 ****************************************************************************/
void oamCmdDefaultHandler( UInt8* data_p, UInt16 size );

#ifdef KPI_STATS
void updatePdcpKpiContainer
(
    UInt32 passiveIndexForKpiStats,
    UInt32 moduleId, /* The container for respective modules */
    InternalCellIndex internalCellIndex
);

/* SPR 15692 fix start */
void updatePdcpKpiThpContainer(UInt32 passiveIndexForKpiThpStats, 
        UInt32 moduleId,InternalCellIndex internalCellIndex);
/* SPR 15692 fix end */

void pdcpResetKpiCounters(UInt32 passiveIndexForPdcpStats,InternalCellIndex internalCellIndex);

void modCmdConfigureKpiStatsReq( UInt8* data_p, UInt16 size, UInt8 moduleId );

void modCmdGetKpiStatsReq( UInt8* data_p, UInt16 size, UInt8 moduleId);

void sendPdcpGetKpiThpStatsCnf(UInt8* data_p, UInt8 moduleThpId);
/*+- 10577*/
#endif

#endif /* _LIB_I_OAM_SAP_COMMANDS_DEF_ */
