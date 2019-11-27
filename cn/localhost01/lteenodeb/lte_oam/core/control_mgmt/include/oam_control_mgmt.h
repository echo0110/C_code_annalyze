/********************************************************************************
* File Name        : oam_control_mgmt.h
* Description      : This file contains the enums, struct, constants definitions
*                    for Control Module(CoM)
* Revision History :
* Date        Author       Reference       Comments
* 03/11/2012  Rohit Kumar
*
* copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
*********************************************************************************/
#ifndef _OAM_CONTROL_MGMT_H_
#define _OAM_CONTROL_MGMT_H__


/****************************************************************************
 **  INCLUDES
 ****************************************************************************/
#include <oam_types.h>
#include <oam_proto.h>
#include <oam_defines.h>

/****************************************************************************
 **  MACROS
 ****************************************************************************/
#define OAM_ALL_MODULES_ID 0

/****************************************************************************
 **  FUNCTION PROTOTYPES
 ****************************************************************************/
void oam_CoM_init_modules(void);
void oam_CoM_de_init_modules(UInt16 module_id);

#endif /*_OAM_CONTROL_MGMT_H_*/
