/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpTlvModulesTable.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : TLV API modules message handlers table implementation 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpTlvModulesTable.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.4.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.4  2009/05/20 13:36:24  gur20548
 * header file name changed
 *
 * Revision 1.3  2009/05/15 07:49:01  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpLteFramework.h"

#include "ltePdcpLteDefs.h"
#include "ltePdcpTlvUtils.h"
#include "ltePdcpOamSapCommands.h"
#include "ltePdcpRrcSapCommands.h"
#if  defined(PDCP_GTPU_INTF)
#include "ltePdcpPdcpUserSap.h"
#endif

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
TlvModulesTable TlvModulesTable_g[] =
{
    { { OAM_MODULE_ID       },      oamCmdTable_g,  oamCmdDefaultHandler },
    { { RRC_MODULE_ID       },      rrcCmdTable_g,  rrcCmdDefaultHandler },
    /*SPR 21188 +*/
#if  defined(PDCP_GTPU_INTF)
    { { PACKET_RELAY_MODULE_ID },   prCmdTable_g,    prCmdDefaultHandler },
#endif
    /*SPR 21188 -*/
    { { TLV_LAST_ID         },      PNULL,          PNULL },
};

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


