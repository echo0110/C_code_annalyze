/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpSapExt.c,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:06 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Implementation for external parts of the PDCP layer 
 *                     interfaces pdcpSap, rrcSap, oamSam. These functions 
 *                     will be called from inside of the PDCP layer.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpSapExt.c,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:06  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:11:41  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5.2.4  2009/07/30 08:28:56  gur19140
 * performance changes
 *
 * Revision 1.5.2.3  2009/07/07 11:37:15  gur19140
 * *** empty log message ***
 *
 * Revision 1.5.2.2  2009/07/03 14:06:55  gur19479
 * updated for pdcp adapter
 *
 * Revision 1.5.2.1  2009/06/09 11:27:28  gur19140
 * first wave of Integration
 *
 * Revision 1.5  2009/05/20 13:15:13  gur20548
 * Changes header files name
 *
 * Revision 1.4  2009/05/15 09:05:23  gur19140
 * bug fixes
 *
 * Revision 1.3  2009/05/14 11:04:58  gur19836
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
#include "ltePdcpEsLteFramework.h"
#include <ltePdcpLteDefs.h>
#include "ltePdcpSapExt.h"
#include "ltePdcpSockTools.h"
#include "lteLayer2CommanTypes.h"


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
extern UInt32 pdcpCurrentTime_g;

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/



/****************************************************************************
 * Function Name  : pdcpOamSendMessage 
 * Inputs         : data_p  - pointer to TLV API buffer.
 *                  size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer for sending API
 *                  message to the OAM stack entity.
 *                  The PDCP layer application has to implement this function
 *                  because it should be portable.
 ****************************************************************************/
SInt32 pdcpOamSendMessage(UInt8 *data_p,UInt16 size
        )
{
    WriteToSocket(data_p,size,OAM_MODULE_ID);
    
    return 0;
}

/****************************************************************************
 * Function Name  : pdcpRrcSendMessage 
 * Inputs         : data_p  - pointer to TLV API buffer.
 *                  size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer for sending API
 *                  message to the RRC layer.
 *                  The PDCP layer application has to implement this function
 *                  because it should be portable.
 ****************************************************************************/
void pdcpRrcSendMessage(UInt8 *data_p,UInt16 size
        )
{
    WriteToSocket( data_p, size, RRC_MODULE_ID );
}
