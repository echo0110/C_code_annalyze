/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2MacRlcPdcpInterface.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *  This file contains the implementation of interface APIs for MAC.
 *
 ****************************************************************************/

#include "lteLayer2MacRlcInterface.h"
#include "lteLayer2RlcPdcpInterface.h"
UInt16 rlcNumOfSupportedAmLC_g = LTE_NUM_OF_SRB + 1; /* 2 SRBs and 1 DRB */
UInt16 rlcNumOfSupportedUmLC_g = 0;
UInt16 pdcpNumOfSupportedLC_g    = 1 + LTE_NUM_OF_SRB; /* 2 SRBs and 1 DRB */

/*SPR23252 Changes Start */
extern int getMsgPoolStatsInShm(unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf );
extern int getMemPoolStatsInShm( unsigned int poolIndex, unsigned int *allocBuf, unsigned int *peakAllocBuf );
extern unsigned int getQpctlSize();
extern unsigned int getQMessageSize();
extern int getQMsgPoolStatsInShm( unsigned int *allocBuf, unsigned int *peakAllocBuf );
extern int getQpctlPoolStatsInShm( unsigned int *allocBuf, unsigned int *peakAllocBuf );
/*SPR23252 Changes End */

/*SPR 4558 Fix Start */
/* SPR 4864 fix start */
UInt16 ueIdxToRrcUeIdx[MAX_UE_SUPPORTED];
UInt16 ueIdxValidOnLayer[MAX_UE_SUPPORTED];
/* SPR 4864 fix end */
UInt16 rrcUeIdxToUeIdx[INVALID_RRC_UE_INDEX + 1];

/* CA changes start */
/*HSM_CHG_START*/
HealthMonitor l2ThreadHealthStoreTick_g;
/*HSM_CHG_END*/

/* Mapping for RRC to Internal Cell index */

InternalCellIndex rrcCellIndexToInternalMap_g[MAX_RRC_CELL_INDEX+1] =
                                            {INVALID_CELL_INDEX, 
                                             INVALID_CELL_INDEX,
                                             INVALID_CELL_INDEX,
                                             INVALID_CELL_INDEX,
                                             INVALID_CELL_INDEX,
                                             INVALID_CELL_INDEX,
                                             INVALID_CELL_INDEX,
                                             INVALID_CELL_INDEX};


/* Mapping for Internal Cell index to RRC Cell index */

RrcCellIndex internalCellIndexToRRCMap_g[MAX_NUM_CELL]; 


/* This global update the cell index which will be used to update the 
 * tick count at RLC/PDCP and relay used for timer and logging */
InternalCellIndex l2CommonActiveTick_g = INVALID_CELL_INDEX;
/*SPR 11244 Changes Start */
#ifndef GTP_SPLIT
extern UInt32 macIsCellRunning(InternalCellIndex cellIndex);
#endif
/*SPR 11244 Changes End */
/***************************************************************************
 * Function Name  : layer2CommonUpdateCellIndexMapping
 * Inputs         : rrcCellIndex - RRC cell index
 * Outputs        : None
 * Returns        : Updated internalCell index or INVALID_CELL_INDEX.
 * Variables      :
 * Description    : This function will update cell index mapping.
 ****************************************************************************/
InternalCellIndex layer2CommonUpdateCellIndexMapping(RrcCellIndex cellIndex)
{
    InternalCellIndex intCellIndex = INVALID_CELL_INDEX;
     if (MAX_RRC_CELL_INDEX < cellIndex)
    {
          return intCellIndex;
    }     
    /* get the Internal cell Index, to check if it already exists */
    intCellIndex = layer2CommonGetInternalCellIndex(cellIndex);

    /* Update only in case when value at cellIndex is invalid,
     * if not invalid then mapping has already been done before.
     */
    if (INVALID_CELL_INDEX == intCellIndex)
    {
        for (intCellIndex = 0; intCellIndex < MAX_NUM_CELL; intCellIndex++)
        {
            /* If the entry is free break from loop */
            if (INVALID_CELL_INDEX == internalCellIndexToRRCMap_g[intCellIndex])
            {
                break;
            }
        }
        /* check if the free entry found for cell Index */
        if (MAX_NUM_CELL == intCellIndex)
        {
            intCellIndex = INVALID_CELL_INDEX;
        }
        /* Update Internal to external maps and vice versa */
        else
        {
            rrcCellIndexToInternalMap_g[cellIndex] = intCellIndex;
            internalCellIndexToRRCMap_g[intCellIndex] = cellIndex;
        }
    }

    return intCellIndex;
}

/***************************************************************************
 * Function Name  : layer2CommonLookupIfValidCellIndex
 * Inputs         : rrcCellIndex - RRC cell index
 * Outputs        : None
 * Returns        : Returns True/false
 * Variables      :
 * Description    : This function validate the rrc cell id and return TRUE
 *                  if cell index exists otw return false.
*****************************************************************************/
UInt8 layer2CommonLookupIfValidCellIndex(RrcCellIndex cellIndex)
{
    InternalCellIndex internalCellIndex = INVALID_CELL_INDEX;
    UInt8 ret = LTE_FALSE;

    /* if cell index is not in range */
    if (MAX_RRC_CELL_INDEX >= cellIndex)
    {
        internalCellIndex = rrcCellIndexToInternalMap_g[cellIndex];
    }
    /* If internal cell index is in range */
    if (MAX_INTERNAL_CELL_INDEX >= internalCellIndex)
    {
        ret = LTE_TRUE;
    }
    return ret;
}

/***************************************************************************
 * Function Name  : layer2CommonGetInternalCellIndex
 * Inputs         : rrcCellIndex - RRC cell index
 * Outputs        : None
 * Returns        : internal index corresponding to higher layer assigned
 *                  cell index or INVALID_CELL_INDEX if wrong rrc index received.
 * Variables      :
 * Description    : This function will return internal cell index corresponding
 *                  to rrc cell index.
****************************************************************************/
InternalCellIndex layer2CommonGetInternalCellIndex(RrcCellIndex rrcCellIndex)
{
    InternalCellIndex internalCellIndex = INVALID_CELL_INDEX;

    if (MAX_RRC_CELL_INDEX >= rrcCellIndex)
    {
        internalCellIndex = rrcCellIndexToInternalMap_g[rrcCellIndex];
        if (MAX_INTERNAL_CELL_INDEX < internalCellIndex)
        {
            internalCellIndex =  INVALID_CELL_INDEX;
        }
    }
    return internalCellIndex;
}

/*************************************************************************
 * Function Name  : layer2CommonGetRRCCellIndex
 * Inputs         : internalCellIndex - internal cell id
 * Outputs        : None
 * Returns        : Higher layer given cell Index based on assigned internal
 *                  index or INVALID_CELL_INDEX if wrong internal index
 *                  received.
 * Variables      :
 * Description    : This function will return return rrc cell index
 *                  corresponding to internal cell index.
****************************************************************************/
RrcCellIndex layer2CommonGetRRCCellIndex(InternalCellIndex internalCellIndex)
{
    RrcCellIndex cellIndex = INVALID_CELL_INDEX;

    if (MAX_INTERNAL_CELL_INDEX >= internalCellIndex)
    {
        cellIndex = internalCellIndexToRRCMap_g[internalCellIndex];
        if (MAX_RRC_CELL_INDEX < cellIndex)
        {
            cellIndex =  INVALID_CELL_INDEX;
        }
    }
    return cellIndex;
}

/****************************************************************************
 * Function Name  : layer2CommonDeleteCellIndexMapping
 * Inputs         : cellIndex - RRC cell index
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function will delete the rrc to internal cell index
 *                  mapping and vice versa.
****************************************************************************/
void layer2CommonDeleteCellIndexMapping(RrcCellIndex cellIndex)
{
    InternalCellIndex internalIndex = INVALID_CELL_INDEX;

    /* update only in case when value at cellIndex is not invalid, if invalid then
     * mapping has already been reeset */
    if (MAX_RRC_CELL_INDEX >= cellIndex)
    {
        internalIndex = layer2CommonGetInternalCellIndex(cellIndex);
        if (MAX_INTERNAL_CELL_INDEX >= internalIndex)
        {
            rrcCellIndexToInternalMap_g[cellIndex] = INVALID_CELL_INDEX;
            internalCellIndexToRRCMap_g[internalIndex] = INVALID_CELL_INDEX;
        }
    }
    return;
}

/***************************************************************************
 * Function Name  : layer2CommonRevertCellIndexMapping
 * Inputs         : internalCellIndex - internal cell id
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function will revert the updated cell index mapping.
****************************************************************************/
void layer2CommonRevertCellIndexMapping(InternalCellIndex internalIndex)
{
    RrcCellIndex cellIndex = INVALID_CELL_INDEX;

    /* update only in case when value at internalIndex is valid,
     * if invalid then mapping has already been reverted*/

    if (MAX_INTERNAL_CELL_INDEX >= internalIndex)
    {
        cellIndex = layer2CommonGetRRCCellIndex(internalIndex);
        if (MAX_RRC_CELL_INDEX >= cellIndex)
        {
            rrcCellIndexToInternalMap_g[cellIndex] = INVALID_CELL_INDEX;
            internalCellIndexToRRCMap_g[internalIndex] = INVALID_CELL_INDEX;
        }
    }
    return;
}
/***************************************************************************
 * Function Name  : layer2CommonInitCellIndexMapping
 * Inputs         : None 
 * Outputs        : None
 * Returns        : None.
 * Variables      :
 * Description    : This function will initialises cell index mapping
 *                  tables with INVALID_CELL_INDEX.
 ****************************************************************************/
void layer2CommonInitCellIndexMapping(void)
{
    InternalCellIndex cellIndex=0;

    for(cellIndex =0; cellIndex < MAX_NUM_CELL;cellIndex++)
    {
        internalCellIndexToRRCMap_g[cellIndex] = INVALID_CELL_INDEX;
    }
}

/***************************************************************************
 * Function Name  : layer2CommonSetActiveCellTick
 * Inputs         : Internal cell Index
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function will return internal cell index update
 *                  to update the tick
****************************************************************************/
inline void layer2CommonSetActiveCellTick(InternalCellIndex internalCellIndex)
{
    l2CommonActiveTick_g = internalCellIndex;
}

/***************************************************************************
 * Function Name  : layer2CommonGetActiveCellTick
 * Inputs         : None
 * Outputs        : None
 * Returns        : internal index corresponding which is used to update the ticks
 * Variables      :
 * Description    : This function will return internal cell index update
 *                  to update the tick
****************************************************************************/
inline InternalCellIndex layer2CommonGetActiveCellTick()
{
    return l2CommonActiveTick_g;
}

/***************************************************************************
 * Function Name  : layer2CommonGetRlcNumOfSupportedAmLC
 * Inputs         : None
 * Outputs        : None 
 * Returns        : Num of supporrted AM LC for RLC
 * Variables      :
 * Description    : This function will give num of supported AM LC for RLC.
****************************************************************************/
UInt16 layer2CommonGetRlcNumOfSupportedAmLC()
{
    return rlcNumOfSupportedAmLC_g;
}
/***************************************************************************
 * Function Name  : layer2CommonGetRlcNumOfSupportedUmLC
 * Inputs         : None
 * Outputs        : None 
 * Returns        : Num of supporrted UM LC for RLC
 * Variables      :
 * Description    : This function will give num of supported UM LC for RLC.
****************************************************************************/
UInt16 layer2CommonGetRlcNumOfSupportedUmLC()
{
    return rlcNumOfSupportedUmLC_g;
}
/***************************************************************************
 * Function Name  : layer2CommonGetPdcpNumOfSupportedLC
 * Inputs         : None
 * Outputs        : None 
 * Returns        : Num of supporrted LC for PDCP
 * Variables      :
 * Description    : This function will give num of supported AM LC for RLC.
****************************************************************************/
UInt16 layer2CommonGetPdcpNumOfSupportedLC()
{
    return pdcpNumOfSupportedLC_g;
}

/* SPR 4558 Fix Start */
/* This code is moved from lteCommonStatsManager.c file */
/***************************************************************************
 * Function Name  : initUeIdxMap
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function initializes the data structures used for
 *                  UE index mappings between RRC and L2
****************************************************************************/
void initUeIdxMap()
{
    UInt32 i = 0;

    for (i = 0; i < MAX_UE_SUPPORTED; i++)
    {
        ueIdxToRrcUeIdx[i] = INVALID_RRC_UE_INDEX;
    }
    for (i = 0; i < INVALID_RRC_UE_INDEX+1; i++)
    {
        rrcUeIdxToUeIdx[i] = INVALID_UE_ID;
    }
    return;
}

/***************************************************************************
 * Function Name  : allocateUeIdxFromRrcUeIdx
 * Inputs         : rrcUeIdx - UE index generated by RRC layer
 * Outputs        : None
 * Returns        : L2 internal UE Index
 * Variables      :
 * Description    : This function returns the internal UE Index
****************************************************************************/
UInt16 allocateUeIdxFromRrcUeIdx(UInt16 rrcUeIdx)
{
    UInt16 i = 0;
    /* + SPR 19169 */
    /* First attempt to allocate the same UE index which was generated by L3 */
    if ((MAX_UE_SUPPORTED > rrcUeIdx) &&
            (INVALID_RRC_UE_INDEX == ueIdxToRrcUeIdx[rrcUeIdx])&&
            (INVALID_UE_ID == rrcUeIdxToUeIdx[rrcUeIdx]))
    {
        rrcUeIdxToUeIdx[rrcUeIdx] = rrcUeIdx;
        ueIdxToRrcUeIdx[rrcUeIdx] = rrcUeIdx;
        return rrcUeIdx;
    }
    /* - SPR 19169 */

    for (i = 0; i < MAX_UE_SUPPORTED; i++)
    {
        if (ueIdxToRrcUeIdx[i] == INVALID_RRC_UE_INDEX)
        {
            break;
        }
    }
    if (i == MAX_UE_SUPPORTED)
    {
        return INVALID_UE_ID;
    }
    if (rrcUeIdxToUeIdx[rrcUeIdx] == INVALID_UE_ID)
    {
        rrcUeIdxToUeIdx[rrcUeIdx] = i;
        ueIdxToRrcUeIdx[i] = rrcUeIdx;
        return i;
    }
    else
    {
        return INVALID_UE_ID;
    }
}

/* SPR 4864 fix start */
/***************************************************************************
 * Function Name  : setUeIdxValid
 * Inputs         : ueIdx - UE index generated by L2 layer
 *                : layer -  PDCP/MAC/RLC layer
 * Outputs        : None
 * Returns        : Status of ue Index on requested layer
 * Variables      :
 * Description    : This function set the staus of UE Index on corresponding 
 *                  Layer
****************************************************************************/
UInt16 setUeIdxValid(UInt16 ueIdx, UInt16 layer)
{
    if (ueIdx >= MAX_UE_SUPPORTED)
    {
        return 0;
    }

    ueIdxValidOnLayer[ueIdx] |= layer;
    return ueIdxValidOnLayer[ueIdx];
}

/***************************************************************************
 * Function Name  : setUeIdxInvalid
 * Inputs         : ueIdx - UE index generated by L2 layer
 *                : layer -  PDCP/MAC/RLC layer
 * Outputs        : None
 * Returns        : Status of ue Index on requested layer
 * Variables      :
 * Description    : This function set the staus of UE Index on corresponding 
 *                  Layer
****************************************************************************/
UInt16 setUeIdxInvalid(UInt16 ueIdx, UInt16 layer)
{
    if (ueIdx >= MAX_UE_SUPPORTED)
    {
        return 0;
    }
    ueIdxValidOnLayer[ueIdx] &= ~layer;
    return ueIdxValidOnLayer[ueIdx];
}

/* SPR 4864 fix end */
/***************************************************************************
 * Function Name  : freeUeIdx
 * Inputs         : ueIdx - UE index generated by L2 layer
 * Outputs        : Set the corresponding contexts to Invalid
 * Returns        : None
 * Variables      :
 * Description    : This function free the RRC/internal UE Index
****************************************************************************/
void freeUeIdx(UInt16 ueIdx)
{
    UInt16 rrcUeIdx = ueIdxToRrcUeIdx[ueIdx];
    rrcUeIdxToUeIdx[rrcUeIdx] = INVALID_UE_ID;
    ueIdxToRrcUeIdx[ueIdx] = INVALID_RRC_UE_INDEX;
}

/***************************************************************************
 * Function Name  : getRrcUeIdxFromUeIdx
 * Inputs         : ueIdx - UE index generated by L2 layer
 * Outputs        : None
 * Returns        : RRC UE Index
 * Variables      :
 * Description    : This function returns the RRC UE Index
****************************************************************************/
UInt16 getRrcUeIdxFromUeIdx(UInt16 ueIdx)
{
    	return ueIdxToRrcUeIdx[ueIdx];
}

/***************************************************************************
 * Function Name  : getUeIdxFromRrcUeIdx
 * Inputs         : rrcUeIdx - UE index generated by RRC layer
 * Outputs        : None
 * Returns        : L2 internal UE Index
 * Variables      :
 * Description    : This function returns the internal UE Index
****************************************************************************/
UInt16 getUeIdxFromRrcUeIdx(UInt16 rrcUeIdx)
{
    return rrcUeIdxToUeIdx[rrcUeIdx];
}
/* SPR 4558 Fix End */

/*SPR 11244 Changes Start */
#if !defined GTP_SPLIT
/***************************************************************************
 * Function Name  : layer2CommonIsCellRunning
 * Inputs         : None
 * Outputs        : None
 * Returns        : internal index corresponding which is used to update the ticks
 * Variables      :
 * Description    : This function will return SUCCESS or FAILURE for 
 *                  cell running or not
****************************************************************************/
inline UInt32 layer2CommonIsCellRunning(InternalCellIndex internalCellIndex)
{
    return macIsCellRunning(internalCellIndex);
}
#endif
/*SPR 11244 Changes End */
/* CA Changes end */

/* +- SPR 17777 */
UInt32 macComChDataReq(void)
{
    return 1;
}
/*coverity 25519 Fix*/

/* + SPR 17439 */
UInt32 macComChUpdateBufferOccupancyReq(void)
/* - SPR 17439 */
{
    return 1;
}
/*coverity 25519 Fix*/

/* +- SPR 17777 */
UInt32 rlcGetUeLogicalChannelBO( void)
{
    return 1;
}

UInt32 rlcSendARQIndication (void)
    /* +- SPR 17777 */
{
    return 1;
}

/*spr 22474 start*/
/****************************************************************************
 *  * Function Name  : L2PrepareMemPoolStats
 *  * Inputs         : pointer to buffer
 *  * Outputs        : prepares the mempool stats for OAM
 *  * Returns        : None
 *  * Description    : This function updates the stats of mempool
 *  ****************************************************************************/
void L2PrepareMemPoolStats
(
    L2MemPoolStats *csplMemPoolStats_p
)
{
    UInt32 allocBufMemPool = 0, nbufMemPool = 0, peakBufMemPool = 0;
    UInt32 poolIndex = 0, bufSize = 0, validPool = 0;
    
    /* Get the mempool stats */
    for( poolIndex = 0,bufSize = MINBUF; poolIndex < NVARMEMPOOL; poolIndex++, bufSize *= 2)
    {
        allocBufMemPool = nbufMemPool = peakBufMemPool = 0;
        nbufMemPool = getMemPoolStatsInShm(poolIndex, &allocBufMemPool, &peakBufMemPool );
        /* Send only pools which has non zero total buffers */
        if (nbufMemPool)
        {
            csplMemPoolStats_p->stats[validPool].bufSize = bufSize;
            csplMemPoolStats_p->stats[validPool].numOfBuf = nbufMemPool;
            csplMemPoolStats_p->stats[validPool].numOfAllocBuf = allocBufMemPool;
            csplMemPoolStats_p->stats[validPool].numOfPeakBuf = peakBufMemPool;
            validPool++;
        }
    }
    /* Set the end marker as bufSize to 0 so that OAM is aware where the status ends in array */
    if (validPool < NVARMEMPOOL)
    {
        csplMemPoolStats_p->stats[validPool].bufSize = 0;
    }
    return;
}

/****************************************************************************
 *  * Function Name  : L2PrepareMsgPoolStats
 *  * Inputs         : pointer to buffer
 *  * Outputs        : prepares the mempool stats for OAM
 *  * Returns        : None
 *  * Description    : This function updates the stats of msgpool
 *  ****************************************************************************/
void L2PrepareMsgPoolStats
(
    L2MsgPoolStats *csplMsgPoolStats_p
)
{
    UInt32 allocBufMsgPool = 0, nbufMsgPool = 0, peakBufMsgPool = 0;
    UInt32 nbufQpctlPool = 0, allocBufQpctlPool = 0, peakBufQpctlPool = 0; 
    UInt32 nbufQMsgPool = 0, allocBufQmsgPool = 0, peakBufQmsgPool = 0;

    UInt32 poolIndex=0, bufSize = 0, validPool = 0;

    /* Get the msgpool stats */
    for( poolIndex = 0,bufSize = MINBUF; poolIndex < NVARMSGPOOL; poolIndex++, bufSize *= 2)
    {
        allocBufMsgPool = nbufMsgPool = peakBufMsgPool = 0;
        nbufMsgPool = getMsgPoolStatsInShm(poolIndex, &allocBufMsgPool, &peakBufMsgPool);
        /* Send only pools which has non zero total buffers */
        if (nbufMsgPool)
        {
            csplMsgPoolStats_p->stats[validPool].bufSize = bufSize;
            csplMsgPoolStats_p->stats[validPool].numOfBuf = nbufMsgPool;
            csplMsgPoolStats_p->stats[validPool].numOfAllocBuf = allocBufMsgPool;
            csplMsgPoolStats_p->stats[validPool].numOfPeakBuf = peakBufMsgPool;
            validPool++;
        }
    }

    /* Set the end marker as bufSize to 0 so that OAM is aware where the status ends in array */
    if (validPool < NVARMSGPOOL)
    {
        csplMsgPoolStats_p->stats[validPool].bufSize = 0;
    }

    nbufQpctlPool = getQpctlPoolStatsInShm(&allocBufQpctlPool, &peakBufQpctlPool);
    csplMsgPoolStats_p->qpctlStats.bufSize = getQpctlSize();
    csplMsgPoolStats_p->qpctlStats.numOfBuf = nbufQpctlPool;
    csplMsgPoolStats_p->qpctlStats.numOfAllocBuf = allocBufQpctlPool;
    csplMsgPoolStats_p->qpctlStats.numOfPeakBuf = peakBufQpctlPool;
    
    nbufQMsgPool = getQMsgPoolStatsInShm(&allocBufQmsgPool, &peakBufQmsgPool);
    csplMsgPoolStats_p->qmsgStats.bufSize = getQMessageSize();
    csplMsgPoolStats_p->qmsgStats.numOfBuf = nbufQMsgPool;
    csplMsgPoolStats_p->qmsgStats.numOfAllocBuf = allocBufQmsgPool;
    csplMsgPoolStats_p->qmsgStats.numOfPeakBuf = peakBufQmsgPool;

    return;
}
/*spr 22474 end*/

