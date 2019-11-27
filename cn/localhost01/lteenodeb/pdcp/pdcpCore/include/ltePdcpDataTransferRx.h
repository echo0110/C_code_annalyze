/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpDataTransferRx.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Data transfer RX module definitions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpDataTransferRx.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.2  2009/07/13 10:23:27  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:17  gur20548
 * Changed header file name
 *
 * Revision 1.3  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_PDCP_DATA_TRANSFER_RX_DEF_
#define _LIB_PDCP_DATA_TRANSFER_RX_DEF_


/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "ltePdcpRxQueues.h"
#include "ltePdcpContext.h"
/* SPR 20899 Start */
#define MULTIPLY_BY_FOUR(x) ((x) << 2)
/* SPR 20899 Stop */

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
 * Function Name  : pdcpProcessSrbRx  
 * Inputs         : ueIndex, LcId
 *                  state - state of PDCP for RX entity,
 *                  event - event received to process packet.
 *                  context_p - PDCP Entity context,
 * Outputs        : None.
 * Returns        : None.
 * Description    : for RX processing of SRB
 ****************************************************************************/
UInt32 pdcpProcessSrbRx(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p);

/****************************************************************************
 * Function Name  : pdcpProcessDrbRx 
 * Inputs         : ueIndex, LcId
 *                  state - state of PDCP for RX entity,
 *                  event - event received to process packet.
 *                  context_p - PDCP Entity context,
 * Outputs        : None.
 * Returns        : None.
 * Description    : for RX processing of DRB
 ****************************************************************************/
UInt32 pdcpProcessDrbRx(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p);

/****************************************************************************
 * Function Name  : pdcpSrbFinishPduRx
 * Inputs         : LP_PdcpDataInd dataInd_p,LP_PdcpEnityContext ctx_p
 * Outputs        : None.
 * Returns        : None.
 * Description    : Finish Rx of SRB PDU
 ****************************************************************************/
void pdcpSrbFinishPduRx( LP_PdcpDataInd dataInd_p,
        LP_PdcpEnityContext ctx_p);

/****************************************************************************
 * Function Name  : pdcpDrbFinishPduRx
 * Inputs         : LP_PdcpDataInd dataInd_p,LP_PdcpEnityContext ctx_p
 * Outputs        : None.
 * Returns        : None.
 * Description    : Finish Rx of DRB PDU
 ****************************************************************************/
void pdcpDrbFinishPduRx( LP_PdcpDataInd dataInd_p,
        LP_PdcpEnityContext ctx_p);

#ifdef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : pdcpRxProcessRoHC
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Performs Decompression of deciphered PDUs received from 
 *                  security engine.
 ****************************************************************************/
void pdcpRxProcessRoHC( LP_PdcpDataInd dataInd_p );
#endif

typedef void (*pdcpFpDrbHandlerFuncT)( LP_PdcpEnityContext ctx_p, 
    LP_PdcpDataInd dataInd_p, UInt8* hdr_p);

void pdcpFpProcessDrbData(LP_PdcpEnityContext ctx_p,
    LP_PdcpDataInd dataInd_p, UInt8* hdr_p);
void pdcpFpProcessDrbControl(LP_PdcpEnityContext ctx_p,
    LP_PdcpDataInd dataInd_p, UInt8* hdr_p);

typedef UInt32 (*pdcpFpLcidHandlerFuncT)( LP_PdcpEnityContext ctx_p, 
    LP_PdcpDataInd dataInd_p);

UInt32 pdcpFpProcessSrbRx(LP_PdcpEnityContext ctx_p,
    LP_PdcpDataInd dataInd_p);
UInt32 pdcpFpProcessDrbRx(LP_PdcpEnityContext ctx_p,
        LP_PdcpDataInd dataInd_p);
#if defined(KPI_STATS) && !defined(UE_SIM_TESTING)
void lteMacUpdateKpiStatsIPThpUlOnDataArrival(
        UInt16 ueIndex , 
        UInt16 lcId,
        UInt32 size,
        tickType_t pktTimeStamp);
#endif

#endif /* _LIB_PDCP_DATA_TRANSFER_RX_DEF_ */
