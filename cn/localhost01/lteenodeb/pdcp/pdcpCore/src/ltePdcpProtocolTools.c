/*****************************************************************************
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpProtocolTools.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : Functions that are used in PDCP protocol implementation 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpProtocolTools.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.7.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.7  2009/05/22 09:20:52  gur19836
 * ROHC Changes merged
 *
 * Revision 1.6  2009/05/22 06:04:24  gur20548
 * bugs fixed
 *
 * Revision 1.5  2009/05/21 07:10:46  gur19140
 * memory leak fixes
 *
 * Revision 1.4  2009/05/20 13:38:43  gur20548
 * header file name changed
 *
 * Revision 1.3  2009/05/15 06:18:46  gur19836
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
#include "ltePdcpLteFramework.h"
#include "ltePdcpProtocolTools.h"
#include "ltePdcpContext.h"
#include "ltePdcpErrors.h"
#include "logging.h"
#include "alarm.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* SPR 5112 changes start */
static void pdcpDeleteFrmStorageQ( LP_PdcpEnityContext ctx_p, UInt32 count ); 
static void pdcpDeleteFrmFwdingQ( LP_PdcpUeContext ueCtx_p, UInt8 lcId, 
    UInt32 count, UInt32 hfn, UInt16 nextSn, UInt8 snSize );
/* SPR 5112 changes end */
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 15909 fix start */
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/


/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : pdcpMakePDUHeaderDRB
 * Inputs         : UInt32 count, UInt16 snSize, void *data_p
 * Outputs        : None.
 * Returns        : returns allocated PDU DRB header
 * Description    : Make PDU header for DRB packet
 ****************************************************************************/
void* pdcpMakePDUHeaderDRB( UInt32 count, UInt16 snSize, void *data_p)
{
    UInt8 *buf=PNULL;
    UInt16 sn=0;
    /* SPR 1841 changes start */
    UInt32 byteSize = 0;
    /* SPR 1841 changes end */

    if ( (snSize != PDCP_TS_DRB_SN_SIZE_SMALL) && 
         (snSize != PDCP_TS_DRB_SN_SIZE_BIG) )
    {
        LOG_PDCP_MSG( PDCP_WRONG_SN_SIZE, LOGWARNING, (PDCP_TX|PDCP_RX), 
                 pdcpCurrentTime_g, __LINE__, snSize, 0, 0, count, 0,0, 
                 __func__, "Making DRB PDU header");
        return PNULL;
    }

    sn = PDCP_SN_FROM_COUNT( count, snSize );
    byteSize = GET_BYTESIZE_FROM_BITSIZE(snSize);

    buf = msgReserve(data_p, 0, byteSize);
    /* SPR 1841 changes end */

    if ( PNULL == buf ) {
        LOG_PDCP_MSG( PDCP_MSG_RESERVE_FAILED, LOGFATAL, (PDCP_TX|PDCP_RX),
                 pdcpCurrentTime_g, __LINE__, GET_BYTESIZE_FROM_BITSIZE(snSize),
                 0,0, count, 0,0, __func__, "Making DRB PDU header");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM); 
        ltePanic("msgReserve failed in %s", __FUNCTION__);
        return PNULL;
    }

    /*Adding SN to PDU*/
    if ( 1 == GET_BYTESIZE_FROM_BITSIZE(snSize) ) {
        buf[0] = (UInt8)(sn) & PDCP_TS_PDU_DRB_SN_SMALL_MASK;
    }
    else {
        buf[0] = (UInt8)(sn >> 8) & PDCP_TS_PDU_DRB_SN_BIG_MASK_HI; /*HI BYTE*/
        buf[1] = (UInt8)(sn); /*LOW BYTE*/
        /* SPR 3811 fix Start */
        /* Setting RESERVED bit to ZERO*/
        buf[0] = buf[0] & PDCP_TS_PDU_DL_DRB_RESV_BIT_MASK;
        /* SPR 3811 fix Start */
    }

    /* setting D/C bit */
    PDCP_TS_PDU_SET_D_C( buf[0], PDCP_TS_PDU_D_C_DATA_PDU ); /*DATA PDU*/

    return (void *)buf;
}

/****************************************************************************
 * Function Name  : pdcpMakePDUHeaderSRB
 * Inputs         : UInt32 count, UInt16 snSize, void *data_p
 * Outputs        : None.
 * Returns        : returns allocated PDU SRB header
 * Description    : Make PDU header for SRB packet
 ****************************************************************************/
void* pdcpMakePDUHeaderSRB( UInt32 count, UInt16 snSize, void *data_p)
{
    UInt8*  buf = PNULL;
    UInt16  sn = 0;

    if ( PDCP_TS_SRB_SN_SIZE != snSize )
    {
        LOG_PDCP_MSG( PDCP_WRONG_SN_SIZE, LOGWARNING, (PDCP_TX|PDCP_RX), 
                 pdcpCurrentTime_g, __LINE__, snSize, 0, 0, count, 0,0, __func__, 
                 "Making header of SRB PDU");
        return PNULL;
    }

    sn = PDCP_SN_FROM_COUNT( count, snSize );

    /*Adding SN to PDU*/
    buf = msgReserve( data_p, 0, 1 );
    if ( PNULL == buf )
    {
        LOG_PDCP_MSG( PDCP_MSG_RESERVE_FAILED, LOGFATAL, (PDCP_TX|PDCP_RX),
                 pdcpCurrentTime_g, __LINE__, 1, 0, 0, count, 0,0, __func__, 
                 "Making header of SRB PDU");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM); 
        ltePanic("msgReserve failed in %s",__FUNCTION__);
        msgFree( data_p );
        return PNULL;
    }
    buf[0] = (UInt8)(sn) & PDCP_TS_PDU_SRB_SN_MASK;
    return (void*)data_p;
}

/****************************************************************************
 * Function Name  : pdcpMakePDUHeaderControlPdu
 * Inputs         : ctrl_type, fms
 * Outputs        : None.
 * Returns        : returns allocated Control PDU header
 * Description    : Make PDU header for Control PDU
 ****************************************************************************/
/* SRP 3608 changes start */
void* pdcpMakePDUHeaderControlPdu(UInt8 ctrl_type, UInt16 fms)
{
    void *header_p = PNULL;
    UInt8 *buf = PNULL;
    header_p = msgAlloc( pdcpContextGetMsgPool(), 0, 0, 0 );
    if ( PNULL == header_p )
    {
        LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                __LINE__,0,0,0,0, 0,0, __func__,"Making control PDU header");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM); 
        ltePanic("msgAlloc failed in %s", __FUNCTION__);
        return PNULL;
    }

    if( !ctrl_type ) /* Check for PSR */
    {
        buf = msgReserve( header_p, 0, 2);
        if ( PNULL == buf )
        {
            LOG_PDCP_MSG( PDCP_MSG_RESERVE_FAILED, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__,1,0,0,0, 0,0,__func__,"Making control PDU header");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM); 
            /* Coverity_31454_Fix Start*/
            msgFree(header_p);
            /* Coverity_31454_Fix End */
            ltePanic("msgReserve failed in %s", __FUNCTION__);

            return PNULL;
        }
        buf[0] = (fms>>8) & 0x0F;
        buf[1] = fms & 0x00FF;
    }
    else
    {
        /*Adding SN to PDU*/
        buf = msgReserve( header_p, 0, 1);
        if ( PNULL == buf )
        {
            LOG_PDCP_MSG( PDCP_MSG_RESERVE_FAILED, LOGFATAL, PDCP_TX, pdcpCurrentTime_g,
                    __LINE__,1,0,0,0, 0,0,__func__,"Making control PDU header");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM); 
            /* Coverity_31454_Fix Start*/
            msgFree(header_p);
            /* Coverity_31454_Fix End */
            ltePanic("msgReserve failed in %s", __FUNCTION__);
            /*cov 11431*/
            return PNULL;
            /*cov 11431*/
        }
        buf[0] = 0x10; 
    }

    /* SPR 3608 changes end */
    /* setting D/C bit */
    PDCP_TS_PDU_SET_D_C( buf[0], PDCP_TS_PDU_D_C_CONTROL_PDU ); /*CONTROL PDU*/

    return header_p;
}

/****************************************************************************
 * Function Name  : pdcpStorageQFindNode
 * Inputs         : PDCP context, key
 * Outputs        : None.
 * Returns        :  PDCP_SUCCESS/ PDCP_FAIL
 * Description    : search storage for node and deletes the same with given key
 *                  and return LP_PdcpDataReq
 ****************************************************************************/
/* SPR 3608 changes start */
UInt32 pdcpStorageQFindNode( LP_PdcpEnityContext ctx_p, UInt16 key)
{
    LP_PdcpDataReq dataReq_p = PNULL;
    
    GET_FIRST_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
    while (PNULL != dataReq_p)
    {
        if (key == dataReq_p->commDataReq.sn)
        {
            DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
            pdcpFreeTxDataReq(dataReq_p);
            dataReq_p = PNULL;	
            return PDCP_SUCCESS;
        }
        else
        {
            GET_NEXT_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
        }
    }
    return PDCP_FAIL;
}

/****************************************************************************
 * Function Name  : pdcpFwdQFindNode
 * Inputs         : PDCP context, lcId, key
 * Outputs        : None.
 * Returns        :  PDCP_SUCCESS/ PDCP_FAIL
 * Description    : search storage for node and deletes the same with given key
 *                  and return LP_PdcpDataReq
 ****************************************************************************/
UInt32 pdcpFwdQFindNode(LP_PdcpUeContext ueCtx_p, UInt16 lcId, UInt32 key)
{
    LP_PdcpDataReq dataReq_p = PNULL;
    
    GET_FIRST_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dataReq_p);
    while(PNULL != dataReq_p) 
    {
        if((lcId == dataReq_p->commDataReq.lcId)&& (key == dataReq_p->commDataReq.sn))
        {        
            DELETE_NODE_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dataReq_p);
            pdcpFreeTxDataReq(dataReq_p);
            dataReq_p = PNULL;	
            return PDCP_SUCCESS;
        }
        else
        {
            GET_NEXT_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dataReq_p);
        }
    }

    return PDCP_FAIL;
}

/* SPR 3608 changes end */
/****************************************************************************
 * Function Name  : pdcpRxStorageQFindNode
 * Inputs         : PDCP context, key
 * Outputs        : None.
 * Returns        : pointer to found node in Q, PNULL if node not found
 * Description    : search storage for node and deletes the same with given key
 *                  and return LP_PdcpDataReq
 ****************************************************************************/
LP_PdcpDataInd pdcpRxStorageQFindNode(LP_PdcpEnityContext ctx_p, UInt32 key)
{
    LP_PdcpDataInd dataInd_p = PNULL;
    
    GET_FIRST_PDCP_RX_STORAGE_Q(ctx_p, dataInd_p);
    while (PNULL != dataInd_p)
    {
        if (key <= dataInd_p->count)
        {
            return dataInd_p;
        }
        else
        {
            GET_NEXT_PDCP_RX_STORAGE_Q(ctx_p, dataInd_p, dataInd_p);
        }
    }
    return PNULL;
}

/****************************************************************************
 * Function Name  : pdcpLogandIgnore
 * Inputs         : ueIndex, lcId, state, event, context_p
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS
 * Description    : Logs an error message and returns
 ****************************************************************************/
UInt32 pdcpLogandIgnore(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, void *context_p)
{
    /* +- SPR 17777 */
    LOG_PDCP_MSG( PDCP_INVALID_EVENT_RCVD, LOGDEBUG, (PDCP_TX|PDCP_RX), 
            pdcpCurrentTime_g, __LINE__, ueIndex, lcId, state, event,sizeof(context_p),0,
            __func__,"");
    return PDCP_SUCCESS;
}

/* SPR 5112 changes start */
/****************************************************************************
 * Function Name  : pdcpDeleteFrmStorageQ
 * Inputs         : ctx_p, count
 * Outputs        : None.
 * Returns        : None
 * Description    : Deletes the nodes from storage queue with count less than
 *                  count associated with FMS.
 ****************************************************************************/
void pdcpDeleteFrmStorageQ( LP_PdcpEnityContext ctx_p, UInt32 count ) 
{
    LP_PdcpDataReq dataReq_p = PNULL;
    LP_PdcpDataReq tempDataReq_p = PNULL;
    
    GET_FIRST_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
    while (PNULL != dataReq_p)
    {
        if ( dataReq_p->commDataReq.count < count ) 
        {
            tempDataReq_p = dataReq_p;
            GET_NEXT_PDCP_TX_STORAGE_Q(ctx_p, dataReq_p);
            DELETE_NODE_PDCP_TX_STORAGE_Q(ctx_p, tempDataReq_p );
            pdcpFreeTxDataReq( tempDataReq_p );
            tempDataReq_p = PNULL;
        }
        else
        {
            break;
        }
    }
}

/****************************************************************************
 * Function Name  : pdcpDeleteFrmFwdingQ
 * Inputs         : ueCtx_p, lcId, count, hfn, nextSn, snSize
 * Outputs        : None.
 * Returns        : None
 * Description    : Deletes the nodes from fwding queue with count less than
 *                  count associated with FMS.
 ****************************************************************************/
void pdcpDeleteFrmFwdingQ( LP_PdcpUeContext ueCtx_p, UInt8 lcId, 
    UInt32 count, UInt32 hfn, UInt16 nextSn, UInt8 snSize )
{
    LP_PdcpDataReq dataReq_p = PNULL;
    LP_PdcpDataReq tempDataReq_p = PNULL;
    
    GET_FIRST_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dataReq_p);
    while (PNULL != dataReq_p)
    {
        if ( lcId == dataReq_p->commDataReq.lcId && 
             PDCP_INVALID_SN != dataReq_p->commDataReq.sn )
        {
            if ( dataReq_p->commDataReq.sn > nextSn )
            {
                /* SN window roll-over case */
                dataReq_p->commDataReq.count = PDCP_MAKE_COUNT( dataReq_p->commDataReq.sn, 
                                                    hfn - 1, snSize );
            }
            else
            {
                dataReq_p->commDataReq.count = PDCP_MAKE_COUNT( dataReq_p->commDataReq.sn, 
                                                    hfn, snSize );
            }
            if ( dataReq_p->commDataReq.count < count ) 
            {
                tempDataReq_p = dataReq_p;
                GET_NEXT_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dataReq_p);
                DELETE_NODE_PDCP_DATA_FWDING_DL_Q(ueCtx_p, tempDataReq_p);
                pdcpFreeTxDataReq( tempDataReq_p );
                tempDataReq_p = PNULL;
            }
            else
            {
                break;
            }
        }
        else
        {
	        GET_NEXT_PDCP_DATA_FWDING_DL_Q(ueCtx_p, dataReq_p);
        }
    }
}
/* SPR 5112 changes end */

/*SPR 3608 changes start*/
/****************************************************************************
 * Function Name  : pdcpParsePSR
 * Inputs         : ueCtx_p, PDCP context, lcId, Pointer to data, length of data
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_FAIL
 * Description    : Process PSR recieved in control PDU.
 *****************************************************************************/
UInt32 pdcpParsePSR( LP_PdcpUeContext ueCtx_p, LP_PdcpEnityContext ctx_p, 
    UInt16 lcId, void *data_p, UInt32 len)
{
    UInt32 nextExpCount = 0;
    UInt32 fmsCount     = 0;
    UInt16 bitmapLen    = 0; 
    UInt16 byte_ix      = 0;
    UInt16 fms          = 0;
    UInt16 sn           = 0;
    UInt8  bit          = 0;
    UInt8  bit_ix       = 0;
    UInt8 *buf_p        = PNULL;
    UInt8 *bitmapBuf_p  = PNULL;

    buf_p = (UInt8 *)getMemFromPool(len, PNULL);
    if (PNULL == buf_p )
    {
        LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, len, 0, lcId, 0,
                0,0, __func__, 
                "Cannot allocate memory for PSR recieved");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemPool error in %s", __func__);
        /*cov 20403*/
        return PDCP_FAIL;
        /*cov 20403*/
    }

    /*531-klocwork warning Fix*/
    memSet(buf_p,0,len);
    /*531-klocwork warning Fix*/
    if ( 0 == msgExtract(data_p, 0, buf_p, len ) )
    {
        LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_TX,
                pdcpCurrentTime_g, __LINE__, len, 0, lcId, 0,
                0,0, __func__,
                "ZCB data cannot be extracted for PSR parsing");
        freeMemPool(buf_p);
        buf_p = PNULL;	
        ltePanic ("msgExtract error in %s", __func__);
        return PDCP_FAIL;
    }

    /* SPR 5112 changes start */
    /* Extract FMS */
    fms = ((((UInt16)buf_p[0])<<8) & 0x0F00) | buf_p[1];
    
    /* Find the associated count value corresponding to FMS field */
    fmsCount = PDCP_MAKE_COUNT( fms, ctx_p->hfn, ctx_p->snSize);
    nextExpCount = PDCP_MAKE_COUNT(ctx_p->next_SN, ctx_p->hfn, ctx_p->snSize);
    if( fmsCount > nextExpCount )
    {
        fmsCount = PDCP_MAKE_COUNT( fms, ctx_p->hfn - 1, ctx_p->snSize); 
    }
    
    /* Discard the packets from storage queue if count value in the queue node
     * is less than count value correponding to FMS field */
    pdcpDeleteFrmStorageQ( ctx_p, fmsCount );
    
    /* Discard the packets from forwarding queue if count value in the queue 
     * node is less than count value correponding to FMS field */
    pdcpDeleteFrmFwdingQ( ueCtx_p, lcId, fmsCount, ctx_p->hfn, 
                          ctx_p->next_SN, ctx_p->snSize );
    /* SPR 5112 changes end */
    
    /* Control Header is parsed so decrement length and advance data pointer 
     * by 2 bytes */
    bitmapLen = len-2;
    bitmapBuf_p = &buf_p[2];

    /*check for 1 set in the bitmap and delete those from the
      storage queue*/
    while( bitmapLen-- )
    {
        for( bit_ix = 0; bit_ix < 8; bit_ix++)
        {
            bit = (((*bitmapBuf_p) & (1<<(7-bit_ix))) >>(7-bit_ix));
            /* Check for 1 set in the bitmap and delete those from the
             * Storage queue (in case of Re-establihment) and Fwding queue 
             * (in case of HO) */
            if(bit == 1)
            {
                /* As per PDCP specs 36.323 Section 6.3.10 - 
                 * The MSB of the first octet of Bitmap indicates whether or 
                 * not the PDCP SDU with the SN (FMS + 1) modulo 4096 has been
                 * received and, optionally decompressed correctly. 
                 * The LSB of the first octet of Bitmap indicates whether or 
                 * not the PDCP SDU with the SN (FMS + 8) modulo 4096 has been
                 * received */
                sn = (fms + ( (byte_ix * 8) + bit_ix ) + 1 ) % 
                                       (PDCP_TS_MAX_SN_AM_MODE + 1) ; 
                /* delete node of SN from FWD queue in case of Target ENB */
                pdcpFwdQFindNode(ueCtx_p, lcId, sn);
                /*delete node of SN from storage queue*/
                pdcpStorageQFindNode(ctx_p, sn);
            }
        }
        byte_ix++;
        bitmapBuf_p++;
    }
    /* Free the memory */
    freeMemPool(buf_p);
    buf_p = PNULL;	

    return PDCP_SUCCESS;
}

/******************************************************************************
 * Function Name  : pdcpEncodePSR
 * Inputs         : ueIndex, lcId, ctx_p - UE + LC context
 * Outputs        :
 * Returns        : None
 * Description    : Encodes bitmap for UL packets received for a LC. This
 *                  function is called only when StatusReportRequired field set
 *                  true by RRC.
 *****************************************************************************/
void pdcpEncodePSR(UInt16 ueIndex, UInt8 lcId,  LP_PdcpEnityContext ctx_p)
{
    void  *data_p = PNULL;	
    
    data_p = msgAlloc( pdcpContextGetMsgPool(), 0, 0, 0 );
    if ( PNULL == data_p )
    {
        /* free memory */
        LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, 0, ueIndex, lcId, 0, 0,0, __func__, "pdcpEncodePSR");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic("msgAlloc failed in %s",__FUNCTION__);
        return;
    }

    if ( PNULL == msgInsert( data_p, 0, ctx_p->pdcpUlStatusInfo.bitmap, 
                             ctx_p->pdcpUlStatusInfo.noOfBytes) ) 
    {
        /* free memory */
        msgFree( data_p );
        data_p = PNULL;
        LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, 
                ctx_p->pdcpUlStatusInfo.noOfBytes, ueIndex, lcId, 0, 
                0,0, __func__, "PSR Bitmap cannot be inserted to ZCB");
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                  CRITICAL_ALARM);
        ltePanic("msgInsert failed in %s",__FUNCTION__);
    }


    if ( PNULL == pdcpStoreControlPduInd(
                ueIndex,
                lcId,
                PDCP_TX_CONTROL_PDU_IND,
                data_p,0) )
    {
	/*Coverity 55050 Fix Start*/
        msgFree( data_p );
        data_p = PNULL;
	/*Coverity 55050 Fix End*/
        LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, 0, ueIndex, lcId, 0, 0,0, __func__, "pdcpEncodePSR");
    }
}
/*SPR 3608 changes end*/

#ifndef UE_SIM_TESTING 
/******************************************************************************
 * Function Name  : pdcpCalculateAvgRlcSduSize
 * Inputs         : pktType,  RLC SDU size, ctx_p - UE + LC context
 * Outputs        :
 * Returns        : Average SDU size computed.
 * Description    : This function calculates Average RLC SDU size for UL/DL
 *                  packets and passes the same to MAC via BO in DL and queue
 *                  in UL
 *****************************************************************************/
UInt32 pdcpCalculateAvgRlcSduSize( UInt8 pktType, UInt32 size,
    LP_PdcpEnityContext ctx_p )
{
    UInt32 avgSduSize = 0;
    /* Compute Average SDU only if following conditions hold true:
       - isSpsLc is TRUE if SPS is enable 
       - if not a control PDU 
       - If not a Silence packet */
    if( LTE_TRUE == ctx_p->isSpsLc )
    {
        if( VOICE_PKT == pktType )
        {
            UInt8 nextIndex = (ctx_p->arrIndex + 1 ) % 
                                       NUM_RLC_SDU_FOR_RUNNING_AVG;
                
            /* Increment Valid SDUs until it has reached
             * NUM_RLC_SDU_FOR_RUNNING_AVG */
            if( NUM_RLC_SDU_FOR_RUNNING_AVG != ctx_p->numValidSdu )
            {
                (ctx_p->numValidSdu)++;
            }
            /* Array index at ctx_p->arrIndex stores the packet size
             * of all the NUM_RLC_SDU_FOR_RUNNING_AVG indices */
            ctx_p->sumSduSize = ctx_p->sumSduSize + 
                                (size - ctx_p->sduArray[nextIndex] );
            ctx_p->sduArray[nextIndex] = size;

            /* Update index pointer */
            ctx_p->arrIndex = nextIndex;
        }
        /* Average = total / (number of valid SDUs */
        if( 0 == ctx_p->numValidSdu )
        {
            avgSduSize = 0;
        }
        else
        {
            avgSduSize = ctx_p->sumSduSize / ctx_p->numValidSdu;
        }
    }
    return avgSduSize;
}

/****************************************************************************
 * Function Name  : pdcpSpsLogandIgnore
 * Inputs         : ueIndex, lcId, state, event, ctx_p, spsIntProf_p, intCellIdx
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS
 * Description    : Logs an error message and returns
 ****************************************************************************/
UInt32 pdcpSpsLogandIgnore(UInt16 ueIndex, UInt8 lcId, 
        UInt32 state, UInt32 event, LP_PdcpUeContext ueCtx_p,
        LP_PdcpSpsIntervalProfile spsIntProf_p, InternalCellIndex intCellIdx )
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(spsIntProf_p)
    /* +- SPR 17777 */
    LOG_PDCP_MSG( PDCP_INVALID_EVENT_RCVD, LOGDEBUG, (PDCP_TX|PDCP_RX), 
            pdcpCurrentTime_g, __LINE__, ueIndex, lcId, state, event,
            intCellIdx,ueCtx_p->newUeIndex,
            __func__,"");
    return PDCP_SUCCESS;
}
#endif
