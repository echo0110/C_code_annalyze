/******************************************************************************
*
*   FILE NAME:
*       simulation_environment.h
*
*   DESCRIPTION:
*       This header file contains default socket configurations
*
*   DATE            AUTHOR      REFERENCE       REASON
*   03 April 2009   VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/


#ifndef _SIMULATION_ENVIRONMENT_H
#define _SIMULATION_ENVIRONMENT_H

/* Listening socket of RRC */
#define RRC_STACK_ADDRESS   "54404"

/* Simulators of internal RRC CSPL modules */
#define OAMH_STACK_ADDRESS  "127.0.0.1:3519"
#define UECC_STACK_ADDRESS  "127.0.0.1:3519"
#define CSC_STACK_ADDRESS   "127.0.0.1:3519"
#define LLIM_STACK_ADDRESS  "127.0.0.1:3519"
#define S1AP_STACK_ADDRESS  "127.0.0.1:3519"

/* External entities */
#define OAM_STACK_ADDRESS   "127.0.0.1:6566"
#define RRM_STACK_ADDRESS   "127.0.0.1:3519"
#define SON_STACK_ADDRESS   "127.0.0.1:3519"
#define PDCP_STACK_ADDRESS  "127.0.0.1:3519"
#define RLC_STACK_ADDRESS   "127.0.0.1:3519"
#define MAC_STACK_ADDRESS   "127.0.0.1:3519"
#define PHY_STACK_ADDRESS   "127.0.0.1:3519"
#define S1U_STACK_ADDRESS   "127.0.0.1:3519"
#define MME_STACK_ADDRESS   "127.0.0.1:3529"



#endif /* _SIMULATION_ENVIRONMENT_H */

