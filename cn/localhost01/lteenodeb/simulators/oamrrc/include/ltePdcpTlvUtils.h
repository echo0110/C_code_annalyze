/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: ltePdcpTlvUtils.h,v 1.1.4.1 2010/08/02 08:57:18 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : TLV messages processing utilites definitions.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: ltePdcpTlvUtils.h,v $
 * Revision 1.1.4.1  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:25  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.1  2009/06/16 10:26:13  gur19836
 * *** empty log message ***
 *
 * Revision 1.1.2.1  2009/06/09 11:33:04  gur19140
 * *** empty log message ***
 *
 * Revision 1.1  2009/05/20 13:22:18  gur20548
 * Changed header file name
 *
 * Revision 1.3  2009/05/15 12:22:41  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _LIB_PDCP_TLV_UTILS_DEF_
#define _LIB_PDCP_TLV_UTILS_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <lteTypes.h>
#include "ltePdcpLteDefs.h"

#ifndef WIN32
#include <netinet/in.h>
#endif

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define TLV_LAST_ID      0xFFFF

#ifdef SKIP_BYTE_ORDER_PROCESS

/*Do not process byte order in messages between modules in case when all modules run on platforms with same byte order.
It can improve performance.
Affects only on messages between modules, all other information (like Network packets in ROHC atc.) are processed
taking care of byte order*/
#define PDCP_NTOHS(data) (data)
#define PDCP_HTONS(data) (data)
#define PDCP_NTOHL(data) (data)
#define PDCP_HTONL(data) (data)

#define CONVERT_INT16_NETWORK_TO_HOST(data)
#define CONVERT_INT16_HOST_TO_NETWORK(data)

#define tlvPrepareHeaderForSending(message)

#else /*SKIP_BYTE_ORDER_PROCESS not defined*/

/*take care of byte order in messages between modules*/
#ifdef MSG_BYTE_ORDER_MSB
/*MSB (Network) BYTE ORDER in messages between modules*/
#define PDCP_NTOHS(data) ntohs(data)
#define PDCP_HTONS(data) htons(data)
#define PDCP_NTOHL(data) ntohl(data)
#define PDCP_HTONL(data) htonl(data)

#else /*MSG_BYTE_ORDER_MSB not defined*/
/*LSB (Intel) BYTE ORDER in messages between modules - default*/
#define PDCP_NTOHS(data) pdcpSwap16(ntohs(data))
#define PDCP_HTONS(data) pdcpSwap16(htons(data))
#define PDCP_NTOHL(data) pdcpSwap32(ntohl(data))
#define PDCP_HTONL(data) pdcpSwap32(htonl(data))
#endif /*MSG_BYTE_ORDER_MSB*/

#define CONVERT_INT16_NETWORK_TO_HOST(data) { (data) = PDCP_NTOHS(data); }
#define CONVERT_INT16_HOST_TO_NETWORK(data) { (data) = PDCP_HTONS(data); }
#define tlvPrepareHeaderForSending(message) tlvPrepareHeaderForSendingInt(message)

#endif /*SKIP_BYTE_ORDER_PROCESS*/

#define TLV_GET_WRITE_POS( msg )    (((LP_TlvMessage)(msg))->header.length - sizeof(TlvHeader))

#define TLV_GET_OPTIONAL_PART_SIZE( msg, mandatoryPartSize  )    (((LP_TlvMessage)(msg))->header.length - sizeof(TlvHeader) - (mandatoryPartSize))

/****************************************************************************
 * Exported Types
 ****************************************************************************/
/* TLV message handler */
typedef void (*tlvHandlerFuncT) (UInt8* data_p, UInt16 size);

/* TLV tag handler */
typedef UInt16 (*TlvTagHandlerFuncT) ( UInt16 tagId, void* params, UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage );

/* TLV tag checker */
typedef UInt16 (*TlvTagCheckerFuncT) ( UInt8* data_p, UInt16 size );

typedef const struct TlvIdT
{
    UInt16          id;
} TlvId, *LP_TlvId;

typedef const struct TlvCommandTableT
{
    TlvId				commandId;
    tlvHandlerFuncT		handler_f;
} TlvCommandTable, *LP_TlvCommandTable;

typedef const struct TlvModulesTableT
{
    TlvId                     moduleId;
    LP_TlvCommandTable        commandTable_p;
    tlvHandlerFuncT           defaultHandler_f;
} TlvModulesTable, *LP_TlvModulesTable;

typedef const struct TlvTagHandlerT
{
    TlvId                  commandId;
    TlvTagHandlerFuncT     handler_f;
    TlvTagCheckerFuncT     checker_f;
} TlvTagHandler, *LP_TlvTagHandler;

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
 * Function Name  : tlvProcessCommand 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It selects and runs TLV message handler.
 ****************************************************************************/
void tlvProcessCommand( UInt8* data_p, UInt16 size );

/****************************************************************************
 * Function Name  : tlvProcessTags 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes TLV tags (optional parameters).
                    It continuously alnalizes tags, selects and runs TLV tags handler for them 
                    from handlersTable_p.
 ****************************************************************************/
UInt16 tlvProcessTags( LP_TlvTagHandler handlersTable_p, void* params, 
                   UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage );

/****************************************************************************
 * Function Name  : tlvCheckTags 
 * Inputs         :     
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks TLV tags (optional parameters).
                    It continuously alnalizes tags, selects and runs TLV tag checker for each of them 
                    from handlersTable_p table.
                    It converts each tags header into Host byte order.
                    Each called TLV tag checker should convert mandatory tags fields to Host byte order.
                    It should be always called before calling tlvProcessTags function.
 ****************************************************************************/
UInt16 tlvCheckTags( LP_TlvTagHandler handlersTable_p, UInt8* data_p, UInt16 size );

/****************************************************************************
 * Function Name  : tlvPrepareMessage 
 * Inputs         : msg_p - pointer on TLV buffer
                    transactionId - transaction id.
                    destinationModuleId - destination module id
                    apiId - API id
                    mandatoryPartSize - size of mandatory message part
 * Outputs        : None.
 * Returns        : None.
 * Description    : TLV utilites function.
                    Fills message header fields.
                    The length header fields is set to value sizeof(TlvHeader)
 ****************************************************************************/
void tlvPrepareMessage(LP_TlvMessage msg_p, UInt16 transactionId, 
           UInt16 destinationModuleId, UInt16 apiId, UInt16 mandatoryPartSize
#ifndef UE_SIM_TESTING
            ,RrcCellIndex cellId
#endif
           );

/****************************************************************************
 * Function Name  : tlvPrepareHeaderForSending
 * Inputs         : message - Pointer to TLV buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : TLV utilites function.
                    It converts all TLV header fields to network byte order.
 ****************************************************************************/
void tlvPrepareHeaderForSendingInt(LP_TlvMessage message_p);

#endif /* _LIB_PDCP_TX_QUEUE_DEF_ */
