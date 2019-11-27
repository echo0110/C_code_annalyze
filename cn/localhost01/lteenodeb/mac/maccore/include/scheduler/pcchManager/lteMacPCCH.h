/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacPCCH.h,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains the declarations and macros for 
 *                     Paging functionality.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacPCCH.h,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.19  2009/07/26 08:11:40  gur15697
 * compilation warnings removed
 *
 * Revision 1.18  2009/07/13 11:50:32  gur15697
 * PCCH_MGR added
 *
 * Revision 1.17  2009/06/27 05:37:17  gur15697
 * recvPagingMessage() arguments modified
 *
 * Revision 1.16  2009/06/24 04:43:27  gur15697
 * recvPagingMessage() prototype changed
 *
 * Revision 1.15  2009/06/22 09:07:21  gur15697
 * review comments incorporated
 *
 * Revision 1.14  2009/06/17 08:45:40  gur15697
 * PAGING_MSG_ARR_SIZE  added
 *
 * Revision 1.13  2009/06/16 14:36:29  gur15697
 * UT bug fix
 *
 * Revision 1.12  2009/06/15 11:20:23  gur15697
 * UT bug fix
 *
 * Revision 1.11  2009/06/05 08:05:07  gur15697
 * init and cleanup functions added
 *
 * Revision 1.10  2009/06/03 11:56:57  gur15697
 * initPCCHMgr() added
 *
 * Revision 1.9  2009/05/29 10:19:37  gur18569
 * removed compilation error
 *
 * Revision 1.8  2009/05/27 09:53:24  gur15697
 * compilation errors removed
 *
 * Revision 1.7  2009/05/18 06:45:36  gur15697
 * Review comments incorporated
 *
 * Revision 1.6  2009/05/12 12:51:26  gur15697
 * Compilation errors removed
 *
 * Revision 1.5  2009/05/12 09:46:08  gur15697
 * macro moved to common file
 *
 * Revision 1.4  2009/05/08 14:30:04  gur15697
 * compilation errors removed
 *
 * Revision 1.2  2009/05/05 14:14:36  gur15697
 * functions updated
 *
 * Revision 1.1  2009/05/04 07:49:20  gur15697
 * Initial vertion
 *
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_PCCH_H
#define LTE_MAC_PCCH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacSFNMgr.h"
#include "lteMacTBSize.h"
#include "lteMacPDCCHMgr.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define PCCH_MGR "pcch_mgr"

#define PAGING_MSG_IN_ADVANCE_MIN 5   /*milliseconds*/
#define PAGING_MSG_IN_ADVANCE_MAX 32  /*Radio frames*/

/*As per spec Max TB size in DCI 1A is 57 bytes (456 bits)*/
/*DCI_1C_Comment Start */
/* SPR 16124 fix start */
#define MAX_PAGING_MSG_SIZE_FOR_DCI_FORMAT_1A 277
/* SPR 16124 fix end */
#define MAX_PAGING_MSG_SIZE_FOR_DCI_FORMAT_1C 217
/*DCI_1C_Comment End */

/* The paging SFN range */ 
#define MIN_PAGING_SFN_RANGE 0
#define MAX_PAGING_SFN_RANGE 8191
#define MAX_PAGING_CYCLE     7

/* The maximum number of Paging Ocassions in one SFN */
#define MAX_PAGING_SF_NUM    4

/****************************************************************************
 * Exported Types
 ****************************************************************************/
typedef struct PCCHInfoNodeT
{
    UInt32 dataLen;
    UInt32 numRepititions;
    UInt8 numRB;
    UInt8 mcsIndex;
    UInt8 *pagingData_p;
}PCCHInfoNode;

/* CA Changes start */
typedef struct PCCHGlobalsT
{
    PCCHInfoNode *pcchMsgArr[MAX_PAGING_SFN_RANGE+1][MAX_PAGING_SF_NUM];
    /* The below table converts the Paging Ocassion to paging Index */
    UInt8 pageSf2Idx[MAX_SUB_FRAME];
    /* First Paging Occasion in SFN.Used in case of SI Paging Message */
    UInt8 firstPagingOccasion;
}PCCHGlobals;
/* CA Changes end */


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* CA changes Start */
extern void initPCCHMgr(InternalCellIndex numOfCells);

extern void cleanupPCCHMgr(InternalCellIndex internalCellIndex);

void deInitPCCHMgr(InternalCellIndex internalCellIndex);

extern UInt32 recvPagingMessage(UInt32 dataLen,
                                UInt8  *data_p,
                                InternalCellIndex internalCellIndex
                                );

extern UInt32 schedulePCCHMsg(
        PdcchDLOutputInfo *pdcchDLUEOutputInfo_p,
        UInt32 *availableCommonCCE_p,
        SInt32 *availableDLRBCount_p,
        UInt32 subFrameNum,
        UInt32 sysFrameNum,
        InternalCellIndex internalCellIndex);

extern void calculatePagingOccasion(UInt8 Ns,InternalCellIndex internalCellIndex);

extern void freePagingMsg(UInt32 sysFrame, UInt32 subFrame,
        InternalCellIndex internalCellIndex);
/* CA changes End */
#endif
