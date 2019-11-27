
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc. All Rights Reserved.
 *
 **************************************************************************
 *
 * $Id: lteMacRRCInterface.h 
 *
 **************************************************************************
 *
 * File Description : This file contains the definitions of the APIs 
 *                    defined for MAC-RRC interface.
 *
 **************************************************************************/
#ifndef LTE_MAC_RRC_INTERFACE_H
#define LTE_MAC_RRC_INTERFACE_H
/**************************************************************************
 * Project Includes
 * *************************************************************************/
#define MAC_RRC_LOG_CH_EXISTS 0
#define MAC_RRC_LOG_CH_NOT_EXISTS 1

/* Error code returned in cell config */
/* When either MAC Init not done or PHY has not responded to PARAM Request */
#define MAC_RRC_ERROR_CELL_IN_INVALID_STATE   4
/* Error when the number of configured cells exceed maximum number */
#define MAC_RRC_ERROR_MAX_CELLS_CONFIGURED    5

/* Error code returned in UE config / reconfig */
#define MAC_RRC_ERROR_SCELL_NOT_EXISTS  6
#define MAC_RRC_ERROR_SCELL_NOT_RUNNING 7

/**************************************************************************
 * Private Definitions
  *************************************************************************/
/***************************************************************************
 * Enum Definitions For APIs
 ***************************************************************************/
#endif
