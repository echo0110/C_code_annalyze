/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2013 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: lteL2RRMInterface.h
 *
 *************************************************************************
 *
 * File Description : This file contains the common structure/ functions
 *                    used by Layer2 and RRM module.   
 *************************************************************************/
/**************************************************************************
 *  Project Includes
 **************************************************************************/
#ifndef LTE_L2_RRM_INTERFACE_H
#define LTE_L2_RRM_INTERFACE_H

#include "lteTypes.h"

#define L2_RRM_IND_TRANS_ID 0xFFFF

typedef struct L2RrmApiHeaderT
{
    UInt16      transactionId;
    UInt16      srcModuleId;
    UInt16      destModuleId;
    UInt16      apiId;
    UInt16      length;
/* CA changes start */
    UInt8       cellIndex;
    UInt8       reserved1;
/*SPR 21653 changes start*/
    UInt32      reserved2;
/*SPR 21653 changes end*/
/* CA changes end */
}L2RrmApiHeader;

typedef struct L2RrmMsgT
{
    L2RrmApiHeader  header;  /* header of the RRM/PDCP requested/sent message */
    UInt8           data[];  /* buffer data of the RRM/PDCP requested/sent message */
}L2RrmMsg;
/* CA changes start */
#define L2_RRM_API_HEADER_LEN  sizeof(L2RrmApiHeader) 
/* CA changes end */
#endif
