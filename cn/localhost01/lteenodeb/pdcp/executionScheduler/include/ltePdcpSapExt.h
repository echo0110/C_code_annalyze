/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpSapExt.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Definitions for external parts of the PDCP layer 
 *                     interfaces pdcpSap, rrcSap, oamSam. These functions will
 *                     be called from inside of the PDCP layer.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpSapExt.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/06/09 11:27:21  gur19140
 * first wave of Integration
 *
 * Revision 1.1  2009/05/20 13:15:56  gur20548
 * Header files name changed
 *
 * Revision 1.2  2009/05/14 11:04:51  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _PDCP_SAP_EXT_DEF_
#define _PDCP_SAP_EXT_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

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
 * Function Name  : pdcpDataInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  data_p  - pointer to PDCP SDU data in Zero-copy buffering format.
 *                  size    - size of data.      
 * Outputs        : None.
 * Returns        : None.
 * Description    : The PDCP layer application has to implement this function.
                    It's used for passing SDU form the PDCP layer to the PDCP user (upper layers).
 ****************************************************************************/
void    pdcpDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p, UInt16 size);

/****************************************************************************
 * Function Name  : pdcpDataStatusInd 
 * Inputs         : ueIndex - UE Id.
 *                  lcId    - Logical channel Id.
 *                  transactionId - Transaction Id. It will be same as received in data request 
 *                                  for which delivery status need to be sent.
 *                  deliveryStatus - SDU delivery status
 * Outputs        : None.
 * Returns        : None.
 * Description    : The PDCP layer application has to implement this function.
                    It used for sending SDU delivery indictaion to the 
                    PDCP user (upper layers) in case of Discard_timer expires for SDU or 
                    PDU delivery is failed (for RLC AM).
 ****************************************************************************/
void    pdcpDataStatusInd( UInt16 ueIndex, UInt16 lcId, UInt32 transactionId, UInt16 deliveryStatus );

/****************************************************************************
 * Function Name  : pdcpOamSendMessage 
 * Inputs         : data_p  - pointer to TLV API buffer.
                    size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer for sending API message to 
                    the OAM stack entity.
                    The PDCP layer application has to implement this function because
                    it should be portable.
 ****************************************************************************/
SInt32 pdcpOamSendMessage(UInt8 *data_p, UInt16 size
        );

/****************************************************************************
 * Function Name  : pdcpRrcSendMessage 
 * Inputs         : data_p  - pointer to TLV API buffer.
                    size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer for sending API message to 
                    the RRC layer.
                    The PDCP layer application has to implement this function because
                    it should be portable..
 ****************************************************************************/
void pdcpRrcSendMessage(UInt8 *data_p, UInt16 size
        );



#endif /* _PDCP_SAP_EXT_DEF_ */
