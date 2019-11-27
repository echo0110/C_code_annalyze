/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpOamSap.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : Definitions for Interface between the PDCP layer and 
 *                     OAM stack entity
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpOamSap.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.3  2009/05/28 13:46:50  gur19140
 * *** empty log message ***
 *
 * Revision 1.2  2009/05/20 14:53:44  gur20548
 * header file name changed
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
 *
 * Revision 1.4  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_PDCP_OAM_SAP_H
#define LTE_PDCP_OAM_SAP_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "ltePdcpOamInterface.h"
#include "lteCommonStatsManager.h"
#include "ltePerfStats.h"
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*Minimum API REQ message sizes */
#define PDCP_OAM_INIT_LAYER_REQ_MIN_SIZE       ( sizeof(TlvHeader) + sizeof(PdcpOamInitLayerReq) )
#define PDCP_OAM_GET_STATUS_REQ_MIN_SIZE       ( sizeof(TlvHeader) + sizeof(PdcpOamGetStatusReq) )
#define PDCP_OAM_SET_LOG_REQ_MIN_SIZE          ( sizeof(TlvHeader) + sizeof(PdcpOamSetLogLevelReq) )


/*Maximum API CNF message size */
/*Coverity 10267, 10268 Fix Start*/
#define PDCP_OAM_CNF_MAX_SIZE_SMALL ( 256 )       
/*Coverity 10267, 10268 Fix End*/
#define PDCP_OAM_CNF_MAX_SIZE_LARGE ( 65535 ) 

#define PDCP_MAX_VAL_KPI_TIMER_T 60*60
#define PDCP_MIN_VAL_KPI_TIMER_T 60

#ifdef PERF_STATS
#define PDCP_MAX_VAL_PERF_TIMER_T 60*60
#define PDCP_MIN_VAL_PERF_TIMER_T 1
#define MAX_LC_IDENTITY 8
#define MAX_PDCP_LC_ID 11
#endif

#ifdef PDCP_ASYNC_INTERFACE
#define OAM_TRANS_ID 0xFFFF
#endif

/* +- SPR 15692 */
#ifndef UE_SIM_TESTING

/* + Layer2 NON CA changes */

#define PDCP_UPDATE_INTERNAL_CELL_INDEX(cellId)\
    layer2CommonUpdateCellIndexMapping(cellId)

#define PDCP_GET_INTERNAL_CELL_INDEX(cellId)\
    layer2CommonGetInternalCellIndex(cellId)

#define PDCP_GET_RRC_CELL_INDEX(cellIndex)\
    layer2CommonGetRRCCellIndex(cellIndex)

/* - Layer2 NON CA changes */

#define SPS_INTERVAL_LIST 16
/****************************************************************************
* Following structure defines SPS Interval profile 
****************************************************************************/
#endif
/* This macro is used for declaring array of variable length */
#define PDCP_OAM_VAR_SIZE(x) 1
/****************************************************************************
 * PDCP_INIT_LAYER_REQ API - Mandatory parameters structure definition 
 ****************************************************************************/
#define THRESHOLD_VALUE_PRESENT 0x01

/* +- SPR 15692 */
/*SPR 20899 Fix Start*/
#define MULTIPLY_BY_TWO(x) ((x) << 1)
/*SPR 20899 Fix Stop*/
/****************************************************************************
 * Function Name  : pdcpOamProcessMessage 
 * Inputs         : data_p  - pointer to TLV API buffer.
 *                  size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for
 *                  delivering OAM API to the PDCP layer. The OAM API message
 *                  message will be processed immediately.
 ****************************************************************************/
void    pdcpOamProcessMessage( UInt8 * data_p, UInt16 size );

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
        );

void modCmdConfigureKpiStatsReq( UInt8* data_p, UInt16 size, UInt8 moduleId );
void modCmdgetKpiStatsReq( UInt8* data_p, UInt16 size, UInt8 moduleId );

 /* + KPI_CA */
void pdcpResetKpiCounters(UInt32 statsIndex, InternalCellIndex internalCellIndex); 
 /* - KPI_CA */
#ifdef PDCP_ASYNC_INTERFACE
/****************************************************************************
 * Function Name  : oamCmdNotifyDeviceFailure
 * Inputs         : None
 * Outputs        : None.
 * Returns        : None.
 * Description    : It sends notification to OAM when device (security engine)
 *                  failure occurs in case of asynchronous interface.
 ****************************************************************************/
void oamCmdNotifyDeviceFailure( void );
#endif

/****************************************************************************
 * Function Name  : oamCmdPrepInitInd
 * Inputs         : responseBuf, length, intCellId 
 * Outputs        : None.
 * Returns        : None.
 * Description    : It prepares the PDCP Init Ind message for OAM.
 ****************************************************************************/
void oamCmdPrepInitInd( UInt8 *responseBuf, UInt16 *length,
        InternalCellIndex intCellId );

#ifdef PERF_STATS
void oamCmdConfigurePerfStatsReq( UInt8* data_p, UInt16 size );
void oamCmdGetPerfstatsReq( UInt8* data_p, UInt16 size );
/* + stats_review_comments_ipc */
UInt16 pdcpGetUePerfStats(void *cnfBuff,
		UInt32 uePerfStatsToGetBitMap,
		UInt16 xi_ueIndex,
		UInt32 msgType,
		InternalCellIndex internalCellIndex);
/* - stats_review_comments_ipc */
/* + PERF_CA */
void pdcpGetCellPerfStats(void *cnfBuff, 
		UInt32 msgType,
		UInt32 cellPerfStatsToGetBitMap,
		InternalCellIndex internalCellIndex);
/* - PERF_CA */
/* + SPR 17439 */
void updatePdcpPerfContainer(void);
/* - SPR 17439 */
void  pdcpResetCellPerfCounters(LteCellPerfStats *pLteCellPerfStats);
void  pdcpResetUePerfCounters(LteUePerfStats *pLteUePerfStats);
void resetPdcpPerfContainer(UInt8 resetId);
#endif

/* + SPR 10577 */
#if (defined KPI_STATS) && (!defined UE_SIM_TESTING)
/****************************************************************************
 * Function Name  : pdcpGetAndSendKpiThpStats
 * Inputs         : responseBuf - Pointer to confirmation buffer
 *                  transactionId - to be sent in API Header
 *                  kpiBitmap - Bitmap indicating KPI's to be pegged.
 *                  moduleTHPId - THP container Id
 *                  respMsgId - Response API ID to be filled in API Header
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will fill the Throughput stats in the 
 *                  confirmation buffer for Aperiodic KPI THP reports.
 ****************************************************************************/
/* SPR 22296 Fix Start */
void pdcpGetAndSendKpiThpStats( UInt8 *responseBuf, UInt16 transactionId,
        UInt8 kpiBitmap, UInt8 moduleThpId, UInt16 respMsgId, UInt8 internalCellIndex,UInt8 resetKpi);
/* SPR 22296 Fix End */
#endif
/* - SPR 10577 */
#endif /* _LIB_I_OAM_SAP_DEF_ */
