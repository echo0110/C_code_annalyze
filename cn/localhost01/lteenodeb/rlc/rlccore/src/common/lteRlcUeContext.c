/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcUeContext.c,v 1.1.1.1.12.2.2.2 2010/10/07 13:35:50 gur23054 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *  it contains implementation of database operations.                   
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteRlcUeContext.c,v $
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include "lteMemPool.h"
#include "lteRlcUeContext.h"
#include "logging.h"
#include "alarm.h"
#include "lteRlcErrorCode.h"
/* + Layer2 NON CA Changes */
#include "lteRlcOamInterface.h"
/* - Layer2 NON CA Changes */
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
UEContext* rlcUEContextList_g[MAX_UE_ID] = {0};
ComChContext rlcComChContext_g ;
/* SPR 15909 fix start */
extern tickType_t globalTTITickCount_g;
/* SPR 15909 fix end */
static inline void rlcDbDeInitUEContext(UEContext* );

/****************************************************************************
 * Function Name  : rlcDbInitDB 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None 
 * Description    : it initialise the UE Context DB.
 ****************************************************************************/
/* + SPR 17439 */
void rlcDbInitDB(void)
/* - SPR 17439 */    
{
    UInt32 idx = 0 ;
    for( ;(idx < MAX_UE_ID) ; idx++)
    {
        rlcUEContextList_g[idx] = RLC_NULL ;
    }
}

/****************************************************************************
 * Function Name  : rlcDbDeInitDB 
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : it deinitialise the UE Context DB.
 ****************************************************************************/
/* + SPR 17439 */
void rlcDbDeInitDB(void)
/* - SPR 17439 */    
{
    UInt32 idx = 0 ;
    for( ;(idx < MAX_UE_ID) ; idx++)
    {
        rlcDbDeInitUEContext(rlcUEContextList_g[idx]);
        rlcUEContextList_g[idx] = RLC_NULL ;
    }
}

/****************************************************************************
 * Function Name  : rlcDbInitUEContext 
 * Inputs         : ueContext_p - pointer to type UEContext
 * Outputs        : None
 * Returns        : None
 * Description    : it initialise the UE Context DB.
 ****************************************************************************/
void rlcDbInitUEContext(UEContext* ueContext_p)
{
    UInt32 idx = 0 ;
    ueContext_p->rlcUEState = RLC_UECONTEXT_INACTIVE; 
    for(; (idx < MAX_LC_ID) ; idx++)
    {
        ueContext_p->rlcEntityInfoList[idx] = RLC_NULL ;
    }
}
/****************************************************************************
 * Function Name  : rlcDbDeInitUEContext 
 * Inputs         : ueContext_p - pointer to type UEContext
 * Outputs        : None
 * Returns        : None
 * Description    : it deinitialise the UE Context DB.
 ****************************************************************************/
static inline void rlcDbDeInitUEContext(UEContext* ueContext_p)
{
    UInt32 idx = 0;
    
    if (RLC_NULL != ueContext_p)
    {
        for( ;(idx < MAX_LC_ID); idx++)
        {
            if(ueContext_p->rlcEntityInfoList[idx] != RLC_NULL)
            {
                /* SPR 4071 Fix start */
                switch(ueContext_p->rlcEntityInfoList[idx]->entityType)
                {
                    case RLC_TX_RX_ENTITY:
                    /** SPR 3805 Chanegs Start **/
                        rlcFreeRLCEntityInfo(ueContext_p->rlcEntityInfoList[idx],
                        RLC_TX_RX_ENTITY);
                    /** SPR 3805 Chanegs End **/
                        ueContext_p->rlcEntityInfoList[idx]=  RLC_NULL;
                    break;
                    case RLC_TX_ENTITY:
                        rlcFreeRLCEntityInfo(ueContext_p->rlcEntityInfoList[idx],
                        RLC_TX_ENTITY);
                        ueContext_p->rlcEntityInfoList[idx]=  RLC_NULL;
                    break;
                    case RLC_RX_ENTITY:
                        rlcFreeRLCEntityInfo(ueContext_p->rlcEntityInfoList[idx],
                        RLC_RX_ENTITY);
                        ueContext_p->rlcEntityInfoList[idx]=  RLC_NULL;
                    break;
                    default:
                    break;
                }
                /* SPR 4071 Fix End */
            }
        }
    }
}

/****************************************************************************
 * Function Name  : rlcDbAllocateUEContext
 * Inputs         : ueIndex - Index of UE
 * Outputs        : ueContext_p - it return pointer to newly created UEContext
 * Returns        : it return RLC_SUCCESS in case of successful allocation of
 *                  UEContext and set pointer of newly created UEContext to 
 *                  @ueContext_p. In case of failure, it returns error code
 * Description    : it creates new UEContext.
 ****************************************************************************/
UEContext* rlcDbAllocateUEContext(UInt32 ueIndex) 
{
    UEContext* ueContext_p = RLC_NULL;
    ueContext_p = (UEContext *)getMemFromPool(sizeof(UEContext) ,RLC_NULL);  
    if(ueContext_p == RLC_NULL )
    {
        return RLC_NULL;
    }    
    rlcDbInitUEContext(ueContext_p);
    /* initially setting state of UE Context to ACTIVE */
    ueContext_p->rlcUEState    = RLC_UECONTEXT_ACTIVE;
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    ueContext_p->delProcessCnt = 0;
#endif
    ueContext_p->ueId          = 0;
   
#ifndef UE_SIM_TESTING
    /* Initialize SPS variables */
    ueContext_p->dlSpsReqType = 0;
    ueContext_p->semiPersistentSchedIntervalDl = 0;
    ueContext_p->ulSpsReqType = 0;
    ueContext_p->semiPersistentSchedIntervalUl = 0;
    /* + Layer2 NON CA Changes */
    ueContext_p->cellIndex = INVALID_CELL_INDEX;
    /* - Layer2 NON CA Changes */

#endif

    rlcUEContextList_g[ueIndex] = ueContext_p;

    return ueContext_p ;
}
/****************************************************************************
 * Function Name  : rlcDbDeleteUEContext
 * Inputs         : ueIndex - Index of UE
 * Outputs        : None
 * Returns        : RLC_SUCCESS for successful deletion else error code
 * Description    : it deletes UEContext.
 ****************************************************************************/

inline UInt32 rlcDbDeleteUEContext(UInt32 ueIndex) 
{
    UInt32 ret              = RLC_SUCCESS ;
    UEContext * ueContext_p = RLC_NULL ;
    
    ueContext_p = rlcUEContextList_g[ueIndex];
    
    if(ueContext_p != RLC_NULL)
    {
        /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
        ueContext_p->cellIndex = INVALID_CELL_INDEX;
#endif
        /* - Layer2 NON CA Changes */
        rlcDbDeInitUEContext(ueContext_p);
        freeMemPool(ueContext_p);
        rlcUEContextList_g[ueIndex] = RLC_NULL ;
    }
    else
    {
        ret = RLC_ERR_DB_NOTFOUND ;
        LOG_RLC_MSG( LTE_RLC_INVALID_UEINDEX_UECONTEXT_MATCH, LOGWARNING,
        (RLC_RX), globalTTITickCount_g, __LINE__, ueIndex, 0,
        0,0, 0,0, __func__, "");

    }
    return ret ;
}
/****************************************************************************
 * Function Name  : rlcDbAllocateRLCEntityInfo
 * Inputs         : ueIndex - Index of UE,
 *                  lcId - Logical channel ID,
 *                  eM - Entity Mode,
 *                  eT - Entity Type
 * Outputs        : rlcEntityInfo_p - it return pointer to newly created 
 *                  RLCEntityInfo.
 * Returns        : it return RLC_SUCCESS in case of successful allocation of
 *                  RLCEntityInfo. and set pointer of newly created 
 *                  RLCEntityInf to @rlcEntityInfo_p. In case of failure, 
 *                  it return error code.
 * Description    : it creates new RLCEntityInfo in existing
 *                  (or creates new context if not exist) UEContext.
 ****************************************************************************/
RLCEntityInfo* rlcDbAllocateRLCEntityInfo( UInt32 ueIndex,
                                           RlcLcId lcId,
                                           RlcEntityMode eM,
                                           RlcEntityType eT
                                         ) 
{
    UEContext *ueContext_p         = RLC_NULL ;
    RLCEntityInfo* rlcEntityInfo_p = RLC_NULL;
    /* validating entity mode and entity type */
    if(( eM == RLC_ENTITY_NONE )||( eT == RLC_ENTITY_TYPE_UNKNOWN ))
    {
        return RLC_NULL;        
    }
    ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex);
    /* if UE Context not exist at given ueIndex then allocate new*/
    if(ueContext_p == RLC_NULL)
    {
        LOG_RLC_MSG(LTE_RLC_INVALID_UECONTEXT, LOGFATAL,
                (RLC_RX), globalTTITickCount_g, __LINE__,
                ueIndex, lcId,0 , 0, 0,0, __func__, "");
        ueContext_p = rlcDbAllocateUEContext(ueIndex); 
        if(ueContext_p == RLC_NULL)
            return RLC_NULL;
    }
    rlcEntityInfo_p = rlcCreateRLCEntityInfo(eM,eT) ;
    if (rlcEntityInfo_p == RLC_FAILURE)
        return RLC_NULL;
    ueContext_p->rlcEntityInfoList[lcId] = rlcEntityInfo_p ;
/*SPR 16855 +-*/
#ifdef UE_SIM_TESTING
    rlcEntityInfo_p->rlcEntityState = RLC_ENTITY_INACTIVE ;
#endif    

    return rlcEntityInfo_p ;
}
/****************************************************************************
 * Function Name  : rlcDbDeleteRLCEntityInfo
 * Inputs         : ueIndex - Index of UE,
 *                  lcId - Logical channel ID,
 *                  entityType - RLC Entity Type
 * Outputs        : None
 * Returns        : RLC_SUCCESS for successful deletion else error code
 * Description    : it deletes RLCEntityInfo from existing UEContext.
 ****************************************************************************/
UInt32 rlcDbDeleteRLCEntityInfo ( UInt32 ueIndex, 
                                  RlcLcId lcId, 
                                  RlcEntityType entityType
                                )
{
    UInt32 ret = RLC_SUCCESS ;
    RLCEntityInfo* rlcEntityInfo_p = RLC_NULL ;
    UEContext *ueContext_p = RLC_NULL ;
    ueContext_p = DB_FETCH_UE_CONTEXT(ueIndex);
    /* ++ SPR 19601 */
/*** Coverity 110082 ++ ***/
#ifndef UE_SIM_TESTING
    UInt8 dlSpsReqType = 0;
    UInt8 ulSpsReqType = 0;
#endif    
    /* -- SPR 19601 */
    if (ueContext_p != RLC_NULL)
    {
        rlcEntityInfo_p = ueContext_p->rlcEntityInfoList[lcId];
    /* ++ SPR 19601 */
#ifndef UE_SIM_TESTING
    	dlSpsReqType = ueContext_p->dlSpsReqType;
    	ulSpsReqType = ueContext_p->ulSpsReqType;
/*** Coverity 110082 -- ***/
        if(rlcEntityInfo_p->rlcEntity.umEntity_p->txUMEntity_p->isSpsLc )
        {
            dlSpsReqType = 0; /*RELEASE*/
        }
        if(rlcEntityInfo_p->rlcEntity.umEntity_p->rxUMEntity_p->isSpsLc )
        {
            ulSpsReqType = 0; /*RELEASE*/
        }
#endif
        /* -- SPR 19601 */
        if(RLC_SUCCESS == rlcFreeRLCEntityInfo(rlcEntityInfo_p, entityType)) 
        {
            ueContext_p->rlcEntityInfoList[lcId] = RLC_NULL ;
        /* ++ SPR 19601 */
#ifndef UE_SIM_TESTING
            ueContext_p->dlSpsReqType = dlSpsReqType;
            ueContext_p->ulSpsReqType = ulSpsReqType;
#endif            
        }
        else
        {
            ret = RLC_FAILURE;
        }
        /* -- SPR 19601 */
    }
    else
    {
        LOG_RLC_MSG(LTE_RLC_DB_DELETE_RLC_ENTITY, LOGERROR, 
       (RLC_RX), globalTTITickCount_g, __LINE__,
                ueIndex, lcId,0 , 0, 0,0, __func__,
                "UE Ctx does not exist ");

        ret = LTE_RLC_ERR_DB_NOTFOUND ;
    }
    return ret ;
}

/****************************************************************************
 * Function Name  : rlcGetRlcEntityID 
 * Inputs         : eInfo_p - pointer to RLCEntityInfo
 *                  ueIndex - Index of UE,
 *                  lcId_p - pointer to Logical channel ID
 * Outputs        : None
 * Returns        : ueIndex_p and lcId_p associated with Rlc Entity
 * Description    : This API is used to return ueIndex_p and lcId_p.
 ****************************************************************************/

void rlcGetRlcEntityID ( RLCEntityInfo* eInfo_p, 
                         UInt32 *ueIndex_p,
                         RlcLcId *lcId_p
                       )
{
    RlcEntityMode eM = eInfo_p->rlcEntityMode;
    switch(eM)
    {
        case RLC_ENTITY_NONE:
            {
            }
            break;
        case RLC_ENTITY_UM:
            {
                if( eInfo_p->rlcEntity.umEntity_p->txUMEntity_p != RLC_NULL ) 
                {
                    *ueIndex_p 
                        = eInfo_p->rlcEntity.umEntity_p->txUMEntity_p->ueIndex;
                    *lcId_p 
                        = eInfo_p->rlcEntity.umEntity_p->txUMEntity_p->lcId;
                }
                else
                {
                    *ueIndex_p 
                        = eInfo_p->rlcEntity.umEntity_p->rxUMEntity_p->ueIndex;
                    *lcId_p 
                        = eInfo_p->rlcEntity.umEntity_p->rxUMEntity_p->lcId;
                }
            }
            break;

        case RLC_ENTITY_AM:
            {
            }
            break;
        default:
            LOG_RLC_MSG(LTE_RLC_DB_DELETE_RLC_ENTITY_1, LOGERROR, 
                    (RLC_RX), globalTTITickCount_g, __LINE__,
                    *ueIndex_p, *lcId_p, 0, 0, 0, 0, __func__,
                    "Invalid ENITY MODE");
            break;
    }
    return;
}   


