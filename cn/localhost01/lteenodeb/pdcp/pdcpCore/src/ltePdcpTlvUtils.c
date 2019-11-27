/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpTlvUtils.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : TLV messages processing utilites definitions. 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpTlvUtils.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.4.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.4  2009/05/20 13:36:24  gur20548
 * header file name changed
 *
 * Revision 1.3  2009/05/15 07:45:45  gur19836
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

#include "ltePdcpErrors.h"
#include "ltePdcpTlvUtils.h"
#include "ltePdcpLteDefs.h"
#include "ltePdcpTlvModulesTable.h"
#include "lteL2RRMInterface.h"
#include "logging.h"
#include "ltePdcpRrcSapCommands.h"
/* + Layer2 NON CA Changes */
#include "ltePdcpContext.h"
/* - Layer2 NON CA Changes */


#define U16BIT_LEN 2
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
static UInt16 tlvFindIndexById(UInt16 id, LP_TlvId table_p,UInt16 elementSize);

void pdcpParseTlvTag (LP_TlvTag msg_tag_p, UInt8* data_p);
/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
/* SPR 15909 fix start */ 
extern tickType_t pdcpCurrentTime_g;
/* SPR 15909 fix end */


#ifndef UE_SIM_TESTING
/* + Layer2 NON CA Changes */
extern PdcpContext pdcpContext;
/* - Layer2 NON CA Changes */
#endif
/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/



/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : tlvProcessCommand 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size   
 * Outputs        : None.
 * Returns        : None.
 * Description    : It selects and runs TLV message handler.
 ****************************************************************************/
void tlvProcessCommand( UInt8* data_p, UInt16 size )
{
    LP_TlvHeader            messageHeader_p = PNULL;
    UInt16                  modulesTableIndex;
    UInt16                  messageHandlerIndex;
    tlvHandlerFuncT         messageHandler = PNULL;

    /* First, it finds command handlers table in TlvModulesTable_g table 
       using Source Module identifier from TLV message header. Then It 
       finds handler in corresponding command handlers table and runs it. 
       */

    /* Prepare message header */
    /* +COVERITY 83271 */
    if(sizeof(TlvHeader) <= size)
    {
        messageHeader_p = (LP_TlvHeader)data_p;
    }
    else
    {
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"Invalid size, too short msg" );
        return;
    }
    /* -COVERITY 83271 */

    /* Convert header fields from network to Host byte order */
    CONVERT_INT16_NETWORK_TO_HOST(messageHeader_p->transactionId);
    CONVERT_INT16_NETWORK_TO_HOST(messageHeader_p->sourceModuleId);
    CONVERT_INT16_NETWORK_TO_HOST(messageHeader_p->destinationModuleId);
    CONVERT_INT16_NETWORK_TO_HOST(messageHeader_p->apiId);
    CONVERT_INT16_NETWORK_TO_HOST(messageHeader_p->length);

#ifndef UE_SIM_TESTING
/*SPR 21743 Changes start*/
#if  defined (PDCP_GTPU_INTF)
    if( PACKET_RELAY_MODULE_ID != messageHeader_p->sourceModuleId )
/*SPR 21743 Changes end*/
    {
        RrcCellIndex  cellId = messageHeader_p->cellIndex;
        InternalCellIndex  cellIndex = PDCP_GET_INTERNAL_CELL_INDEX( cellId );

        if ( INVALID_CELL_INDEX == cellIndex )
        {
            LOG_PDCP_MSG( PDCP_RRC_CELL_INDEX_ERR, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, cellId, 
                    cellIndex , 0, 0, 0, 0, 
                    __func__, "" );
            return;
        }
    }
#endif
#endif

    /* Check message header */
    /* --- check destination module Id */ 
    if ( PDCP_MODULE_ID != messageHeader_p->destinationModuleId )
    {
        /* invalid destination module id -- drop it */
        LOG_PDCP_MSG (PDCP_DEST_ID_INCORRECT, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, 
                 messageHeader_p->destinationModuleId, 0,0,0,
                 0,0, __func__, "");
        return;
    }
    /* Find message handlers table */
    modulesTableIndex = tlvFindIndexById(
            messageHeader_p->sourceModuleId, 
            (LP_TlvId)TlvModulesTable_g, 
            sizeof(TlvModulesTable) );
    if ( TLV_LAST_ID == modulesTableIndex)
    {
        /* source module id is not supported -- drop it */
        LOG_PDCP_MSG (PDCP_SRC_ID_INCORRECT, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, 
                 messageHeader_p->sourceModuleId, 0,0,0, 0,0, __func__, "");
        return;
    }

    do 
    {
        /* Find message handler index */
        messageHandlerIndex = tlvFindIndexById(
               messageHeader_p->apiId, 
               (LP_TlvId)(TlvModulesTable_g[modulesTableIndex].commandTable_p), 
               sizeof(TlvCommandTable) );
        if ( TLV_LAST_ID == messageHandlerIndex)
        {
            /* API id is not supported */
            break;
        }

        /* Get and check msg handler */
        messageHandler = 
          TlvModulesTable_g[modulesTableIndex].commandTable_p[messageHandlerIndex].handler_f;
        if ( PNULL == messageHandler )
        {
            /* API id is not supported */
            break;
        }

        /* Run it */
        messageHandler( data_p, size );

    } while (0);

    if ( PNULL == messageHandler )
    {   /* run default TLV handler */
        CHECK_PNULL( TlvModulesTable_g[modulesTableIndex].defaultHandler_f );
        TlvModulesTable_g[modulesTableIndex].defaultHandler_f( data_p, size );
    }
}

/****************************************************************************
 * Function Name  : tlvCheckTags 
 * Inputs         : handlersTable_p
 *                  data_p
 *                  size
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks TLV tags (optional parameters).
 *                  It continuously alnalizes tags, selects and runs TLV tag
 *                  checker for each of them from handlersTable_p table.
 *                  It converts each tags header into Host byte order.
 *                  Each called TLV tag checker should convert mandatory tags
 *                  fields to Host byte order. It should be always called
 *                  before calling tlvProcessTags function.
 ****************************************************************************/
UInt16 tlvCheckTags(LP_TlvTagHandler handlersTable_p, UInt8* data_p, 
        UInt16 size)
{
    UInt16                     result   = PDCP_SUCCESS;
    LP_TlvTag                  msg_tag_p    = PNULL;
    UInt16                     dataIndex    = 0;
    UInt16                     tagCheackerIndex;
    TlvTagCheckerFuncT         tagChecker_f = PNULL;
    UInt16                     originalSize = size;
    TlvTag                     msg_tag_data;

    while ( 0 != size )
    {
        /* check Optional parameters length */
        if ( sizeof(TlvTagHeader) > size )
        {
            LOG_PDCP_MSG (PDCP_INVALID_OPTIONALPARAM_LEN, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, sizeof(TlvTagHeader),
                     size, 0,0, 0,0, __func__, "");
            result = PDCP_ERR_TLV_PARSING_INVALID_OPTIONAL_PARAMETERS_LENGTH;
            break; 
        }

       /* ALIGNMENT changes start */
       msg_tag_p = &msg_tag_data;
       READ_API_MSG(TlvTag, msg_tag_p, &data_p[dataIndex]); 
       /* ALIGNMENT changes end */

        /* check tag length */
        if ( ( sizeof(TlvTagHeader) > msg_tag_p->header.length ) || /* minimum tag length */
                ( msg_tag_p->header.length > size ) )     /* available data size */
        {
            LOG_PDCP_MSG (PDCP_INVALID_OPTIONALPARAM_LEN, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, msg_tag_p->header.tagId,
                     msg_tag_p->header.length, 0,0, 0,0, __func__, "");
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH; 
            break;
        }

        /* Find tag checker index */
        tagCheackerIndex = tlvFindIndexById(
                msg_tag_p->header.tagId, 
                (LP_TlvId)handlersTable_p, 
                sizeof(TlvTagHandler) );

        if ( TLV_LAST_ID == tagCheackerIndex)
        {
            /* Tag id is not supported -- drop it */
            LOG_PDCP_MSG (PDCP_INVALID_TAG_ID, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, msg_tag_p->header.tagId,
                     0,0,0, 0,0, __func__, "");
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_ID;
            break;
        }
        else 
        {
            /* Get and check msg checker */
            tagChecker_f  = handlersTable_p[tagCheackerIndex].checker_f;
            if ( PNULL == tagChecker_f )
            {
                /* API id is not need to be checked -- skip it */
                LOG_PDCP_MSG (PDCP_TAG_ID_NOT_CHKED, LOGWARNING, PDCP_RRCOAM, 
                         pdcpCurrentTime_g, __LINE__, msg_tag_p->header.tagId,
                         0,0,0, 0,0, __func__, "");
            }
            else 
            {
                /* Run it */
                /* ALIGNMENT changes start */
                result = tagChecker_f(
                        &data_p[dataIndex + sizeof(TlvTagHeader)],
                        msg_tag_p->header.length - sizeof(TlvTagHeader) 
                        );
                /* ALIGNMENT changes end */
                if ( PDCP_SUCCESS != result )
                {
                    /* Tag parsing is failed */
                    break;
                }
            }
        }

        /* switch to next tag */
        size = size - msg_tag_p->header.length;
        dataIndex = dataIndex + msg_tag_p->header.length;

        /* check dataIndex overflow */
        if ( (originalSize <= dataIndex) &&  (0 != size) )
        {
            PDCP_LOG(LOG_WARNING, 
                    "dataIndex overflow. originalSize[%u]dataIndex[%u]", 
                    originalSize, dataIndex );
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH; 
            break;
        }
    }

    return result;
}

/****************************************************************************
 * Function Name  : tlvProcessTags 
 * Inputs         : handlersTable_p,params,data_p,size,responseMessage    
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes TLV tags (optional parameters).
 *                  It continuously alnalizes tags, selects and runs TLV 
 *                  tags handler for them from handlersTable_p.
 *                  It should be always called after tlvCheckTags function.
 ****************************************************************************/
UInt16 tlvProcessTags(LP_TlvTagHandler handlersTable_p, void* params, 
        UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage )
{
    UInt16                      result   = PDCP_SUCCESS;
    /* ALIGNMENT changes start */
    LP_TlvTag                   msg_tag_p    = PNULL;
    /* ALIGNMENT changes end */
    UInt16                      dataIndex    = 0;
    UInt16                      tagHandlerIndex;
    TlvTagHandlerFuncT          tagHandler_f = PNULL;
    TlvTag                      msg_tag_data;

    while (size)
    {
        /* Optional parameters Length should be already 
           checked by tlvCheckTags() */
        CHECK_EXP( sizeof(TlvTagHeader) > size );

        /* ALIGNMENT changes start */
        msg_tag_p = &msg_tag_data;        
        READ_API_MSG(TlvTag, msg_tag_p, &data_p[dataIndex]);
        /* ALIGNMENT changes end */
        /* Tag header fields should be already converted to 
           Host byte order by tlvCheckTags() */

        /* Tag length should be already checked by tlvCheckTags() */

        /* Find tag handler index */
        tagHandlerIndex = tlvFindIndexById(
                msg_tag_p->header.tagId, 
                (LP_TlvId)handlersTable_p, 
                sizeof(TlvTagHandler) );

        /* TAG id should be already checked by tlvCheckTags() */
        CHECK_EXP( TLV_LAST_ID == tagHandlerIndex );

        /* Get and check msg handler */
        tagHandler_f  = handlersTable_p[tagHandlerIndex].handler_f;
        if ( PNULL == tagHandler_f )
        {
            /* API id is not supported -- skip it */
            LOG_PDCP_MSG (PDCP_INVALID_TAG_ID, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, msg_tag_p->header.tagId,
                     0,0,0, 0,0, __func__, "");
        }
        else
        {
            /* Run it */
            /* ALIGNMENT changes start */
            result = tagHandler_f( 
                    msg_tag_p->header.tagId,
                    params, 
                    &data_p[dataIndex + sizeof(TlvTagHeader)],
                    msg_tag_p->header.length - sizeof(TlvTagHeader), 
                    responseMessage 
                    );
            /* ALIGNMENT changes end */
            if ( PDCP_SUCCESS != result )
            {
                /* Tag handler should return error only in case when 
                   further tag processing isn't possible
                 */
                break;
            }
        }

        /* switch to next tag */
        size = size - msg_tag_p->header.length;
        dataIndex = dataIndex + msg_tag_p->header.length;
    }

    return result;
}

/****************************************************************************
 * Function Name  : tlvFindIndexById 
 * Inputs         : id - Table row key value to find
 *                  table_p - Pointer to table
 *                  elementSize - Size of table element
 * Outputs        : None.
 * Returns        : Table row index or TLV_LAST_ID if row with such id isn't
 *                  found.
 * Description    : TLV utilites function.
 *                  It finds the row in table which coresponds specified id.
 *                  Each table element should correspond to the following
 *                  conventions:
 *                       - each table element is structure;
 *                       - the first elemnt field has type TlvId.
 ****************************************************************************/
UInt16 tlvFindIndexById(UInt16 id, LP_TlvId table_p, UInt16 elementSize )
{
    UInt16 result = TLV_LAST_ID;
    UInt16 index = 0;
    while ( TLV_LAST_ID != table_p->id )
    {
        if ( table_p->id == id )
        {
            result = index;
            break;
        }
        index++;

        table_p= (LP_TlvId) ((UInt8 *)table_p + elementSize);
    }
    return result;
}

/****************************************************************************
 * Function Name  : tlvPrepareMessage 
 * Inputs         : msg_p - pointer on TLV buffer
 *                  transactionId - transaction id.
 *                  destinationModuleId - destination module id
 *                  apiId - API id
 *                  mandatoryPartSize - size of mandatory message part
 * Outputs        : None.
 * Returns        : None.
 * Description    : TLV utilites function.
 *                  Fills message header fields.
 *                  The length header fields is set to value sizeof(TlvHeader)
 ****************************************************************************/
void tlvPrepareMessage(LP_TlvMessage msg_p, UInt16 transactionId, 
        UInt16 destinationModuleId, UInt16 apiId, UInt16 mandatoryPartSize
        /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
        ,RrcCellIndex cellId  
        /* - Layer2 NON CA Changes */
#endif
        )
{
    if ( PNULL != msg_p )
    {
        if(destinationModuleId  == RRM_MODULE_ID)
        {
            UInt8 *cnfMsg_p = (UInt8 *)msg_p;
            UInt16 current = 0;
            LTE_SET_U16BIT(cnfMsg_p, transactionId);
            current += U16BIT_LEN;
            LTE_SET_U16BIT(cnfMsg_p + current, PDCP_MODULE_ID);
            current += U16BIT_LEN;
            LTE_SET_U16BIT(cnfMsg_p + current, destinationModuleId);
            current += U16BIT_LEN;
            LTE_SET_U16BIT(cnfMsg_p + current, apiId);
            current += U16BIT_LEN;
            LTE_SET_U16BIT(cnfMsg_p + current, (mandatoryPartSize + sizeof(TlvHeader)));
            current += U16BIT_LEN;
#ifndef UE_SIM_TESTING
            cnfMsg_p[current] = cellId;
#endif
        }
        else
        {
            /* Prepare message header */
            msg_p->header.transactionId = transactionId;
            msg_p->header.sourceModuleId = PDCP_MODULE_ID;
            msg_p->header.destinationModuleId = destinationModuleId;
            msg_p->header.apiId = apiId;
#ifndef UE_SIM_TESTING
            /* + Layer2 NON CA Changes */
            msg_p->header.cellIndex = cellId;
            /* - Layer2 NON CA Changes */
#endif
        /* + Layer2 NON CA Changes */

            msg_p->header.length = sizeof(TlvHeader) + mandatoryPartSize;
        }
        /* - Layer2 NON CA Changes */
    }
}

/****************************************************************************
 * Function Name  : tlvPrepareHeaderForSending
 * Inputs         : message - Pointer to TLV buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : TLV utilites function.
 *                  It converts all TLV header fields to network byte order.
 ****************************************************************************/
void tlvPrepareHeaderForSendingInt(LP_TlvMessage message_p)
{
    /* Convert header fields from Host to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK(message_p->header.transactionId);
    CONVERT_INT16_HOST_TO_NETWORK(message_p->header.sourceModuleId);
    CONVERT_INT16_HOST_TO_NETWORK(message_p->header.destinationModuleId);
    CONVERT_INT16_HOST_TO_NETWORK(message_p->header.apiId);
    CONVERT_INT16_HOST_TO_NETWORK(message_p->header.length);
}

/****************************************************************************
 * Function Name  : pdcpParseLP_TlvTag
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : msg_tag_p - pointer to LP_TlvTag
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the TlvTag from RRC.
 * ****************************************************************************/
void pdcpParseTlvTag (LP_TlvTag msg_tag_p, UInt8* data_p)
{
    UInt8 current =0;
    msg_tag_p->header.tagId         = LTE_GET_U16BIT(&data_p[current]);
    current +=2;
    msg_tag_p->header.length        = LTE_GET_U16BIT(&data_p[current]);
}
