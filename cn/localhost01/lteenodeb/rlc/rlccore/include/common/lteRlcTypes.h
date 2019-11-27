/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteRlcTypes.h,v 1.1.1.1.12.2.2.1 2010/09/21 15:49:43 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *  This file describes types for RLC stack . 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteRlcTypes.h,v $
 * Revision 1.1.1.1.12.2.2.1  2010/09/21 15:49:43  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.10.1  2010/08/10 12:14:04  gur19836
 * RLC Rel 2.0 Code Merged
 *
 * Revision 1.9.2.1  2010/06/17 10:59:42  gur19836
 * RLC AM Base Code Merged
 *
 * Revision 1.9  2010/06/14 11:57:59  gur21148
 * RLC_UM_MERGE
 *
 * Revision 1.8  2009/04/02 14:27:51  gur19140
 * changes for intg and peer to peer testing
 *
 * Revision 1.7  2009/03/23 08:37:17  gur19140
 * Integration changes first waves
 *
 * Revision 1.6  2009/03/18 05:08:35  gur19140
 * Review Comments Incorporation.
 *
 * Revision 1.5  2009/02/13 06:54:59  gur20294
 * added function rlcParseReEstablishMsg to lteRlcRrcParseUtil.c file
 *
 * Revision 1.4  2009/02/09 11:47:11  gur19836
 * RlcId type changed to UInt8
 *
 * Revision 1.3  2009/02/06 10:58:57  gur19836
 * CR comments incorporated
 *
 * Revision 1.2  2009/01/29 05:35:22  gur20294
 * after code review comments incorporated
 *
 * Revision 1.1  2008/12/30 06:24:49  gur20294
 * RLC Layer added. INITIAL DRAFT
 *
 *
 *
 ****************************************************************************/

#ifndef RLCTYPES_H
#define RLCTYPES_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteRlcBuffer.h"
#include "lteLayer2CommanTypes.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/* Upper bounds */
#define MAX_UE_ID           MAX_UE_SUPPORTED 

#define MAX_LC_ID           MAX_LOGICAL_CHANNEL  
#define MAX_COMMON_CHANNEL  3 
#define MIN_DRB_LC_ID       3

#define TM_LC_CCCH         0
#define TM_LC_BCCH         1
#define TM_LC_PCCH         2

/****************************************************************************
 * Exported Types
 ****************************************************************************/
extern ModuleLogDetail *rlcModuleLogDetail_p;

typedef UInt32 RlcResult ;
typedef UInt16 RlcCount ;
typedef UInt8  RlcLcId ;
typedef UInt8  RlcExt ;
typedef UInt16 RlcUeIndex ;
typedef UInt16 RlcUeId ;
typedef UInt16 RlcVr ;
typedef UInt16 RlcSn ;
typedef UInt16 RlcVt ;
typedef UInt32 RlcQSz ;
typedef UInt32 RlcPduSz ;

typedef enum RlcFiT {
    /*First byte of the Data field corresponds to the first byte of a RLC SDU.
      Last byte of the Data field corresponds to the last byte of a RLC SDU.
     */
    RLC_DF_FB_LB_MATCH = 0 ,    
    /* First byte of the Data field corresponds to the first byte of a RLC SDU.
       Last byte of the Data field does not correspond to the last byte of a RLC SDU.
     */
    RLC_DF_FB_NLB_MATCH = 1 ,    
    /*First byte of the Data field does not correspond to the first byte of a RLC SDU.
      Last byte of the Data field corresponds to the last byte of a RLC SDU.
     */
    RLC_DF_NFB_LB_MATCH = 2 ,    
    /*First byte of the Data field does not correspond to the first byte of a RLC SDU.
      Last byte of the Data field does not correspond to the last byte of a RLC SDU.
     */
    RLC_DF_NFB_NLB_MATCH = 3     
} RlcFi;

typedef enum RlcAutFlagValuesT { 
 RLC_ZERO = 0,
 RLC_ONE = 1,
 RLC_TWO = 2,
 RLC_THREE = 3,
 RLC_FOUR = 4,
 RLC_FIVE = 5,
 RLC_SIX = 6,
 RLC_SEVEN = 7,
 RLC_EIGHT = 8,
 RLC_NINE = 9,
 RLC_TEN = 10,
 RLC_ELEVEN = 11
} RlcAutFlagValues ;

typedef enum RlcBoolT { 
 RLC_FALSE = 0,
 RLC_TRUE = 1
} RlcBool ;

typedef enum RlcUEContextState {
    RLC_UECONTEXT_IDLE = 0 ,
    RLC_UECONTEXT_ACTIVE ,
    RLC_UECONTEXT_INACTIVE ,
    RLC_UECONTEXT_PENDING_DELETION
} RlcUEState ;
            
typedef enum RlcEntityModeT {
    RLC_ENTITY_NONE = 0,
    RLC_ENTITY_TM , 
    RLC_ENTITY_UM ,
    RLC_ENTITY_AM 
} RlcEntityMode ;

typedef enum RlcEntityStateT{
    RLC_ENTITY_IDLE = 0,
    RLC_ENTITY_ACTIVE,
    RLC_ENTITY_INACTIVE,
    RLC_ENTITY_PENDING_DELETION
} RlcEntityState ;

typedef enum RlcEntityTypeT {
    RLC_ENTITY_TYPE_UNKNOWN = 0,
    RLC_TX_ENTITY ,
    RLC_RX_ENTITY ,
    RLC_TX_RX_ENTITY 
} RlcEntityType ;

typedef enum RlcEntityOpCodeT {
    RLC_ADD_ENTITY = 0,
    RLC_DELETE_ENTITY,
    RLC_RECONFIG_ENTITY,
	RLC_REESTAB_ENTITY
} RlcEntityOpCode ;


typedef enum SnFieldLengthT { 
    /* Rel 5.3: Coverity 32378 and 32374 Fix Start */
    invalidSnSize = 0,
    /* Rel 5.3: Coverity 32378 and 32374 Fix End */
    size5 = 5,
    size10 = 10
} SnFieldLength ;

typedef enum TPollRetransmitT {
    prms5 = 5, prms10 = 10, prms15 = 15, prms20 = 20, prms25 = 25, prms30 = 30, prms35 = 35,
    prms40 = 40, prms45 = 45, prms50 = 50, prms55 = 55, prms60 = 60, prms65 = 65, prms70 = 70,
    prms75 = 75, prms80 = 80, prms85 = 85, prms90 = 90, prms95 = 95, prms100 = 100, prms105 = 105,
    prms110 = 110, prms115 = 115, prms120 = 120, prms125 = 125, prms130 = 130, prms135 = 135,
    prms140 = 140, prms145 = 145, prms150 = 150, prms155 = 155, prms160 = 160, prms165 = 165,
    prms170 = 170, prms175 = 175, prms180 = 180, prms185 = 185, prms190 = 190, prms195 = 195,
    prms200 = 200, prms205 = 205, prms210 = 210, prms215 = 215, prms220 = 220, prms225 = 225,
    prms230 = 230, prms235 = 235, prms240 = 240, prms245 = 245, prms250 = 250, prms300 = 300,
    prms350 = 350, prms400 = 400, prms450 = 450, prms500 = 500
} TPollRetransmit ;

/*SPR 8407 Fix Start*/
typedef enum PollPDUT {
    p0 = 0, p4 = 4, p8 = 8, p16 = 16 , p32 = 32, p64 = 64, p128 = 128 , p256 = 256, pInfinity   
}PollPDU ;
/*SPR 8407 Fix End*/

typedef enum PollByteT {
    /* Rel 5.3: Coverity 32376 Fix Start */
    kb0 = 0, kb25 = 25, kb50 = 50, kb75 = 75, kb100 = 100, kb125 = 125, kb250 = 250, kb375 = 375,
    /* Rel 5.3: Coverity 32376 Fix End */
    kb500 = 500, kb750 = 750, kb1000 = 1000, kb1250 = 1250, kb1500 = 1500, kb2000 = 2000,
    kb3000 = 3000, kbinfinity
} PollByte ;

typedef enum TReorderingT {
    rms0 = 0, rms5 = 5, rms10 = 10, rms15 = 15, rms20 = 20, rms25 = 25, rms30 = 30, rms35 = 35,
    rms40 = 40, rms45 = 45, rms50 = 50, rms55 = 55, rms60 = 60, rms65 = 65, rms70 = 70,
    rms75 = 75, rms80 = 80, rms85 = 85, rms90 = 90, rms95 = 95, rms100 = 100, rms110 = 110,
    rms120 = 120, rms130 = 130, rms140 = 140, rms150 = 150, rms160 = 160, rms170 = 170,
    rms180 = 180, rms190 = 190, rms200 = 200
} TReordering ;

typedef enum TStatusProhibitT {
    spms0 = 0, spms5 = 5, spms10 = 10 , spms15 = 15, spms20 = 20, spms25 = 25, spms30 = 30, spms35 = 35, 
    spms40 = 40, spms45 = 45, spms50 = 50, spms55 = 55, spms60 = 60, spms65 = 65, spms70 = 70,
    spms75 = 75, spms80 = 80, spms85 = 85, spms90 = 90, spms95 = 95, spms100 = 100, spms105 = 105,
    spms110 = 110, spms115 = 115, spms120 = 120, spms125 = 125, spms130 = 130, spms135 = 135,
    spms140 = 140, spms145 = 145, spms150 = 150, spms155 = 155, spms160 = 160, spms165 = 165,
    spms170 = 170, spms175 = 175, spms180 = 180, spms185 = 185, spms190 = 190, spms195 = 195,
    spms200 = 200, spms205 = 205, spms210 = 210, spms215 = 215, spms220 = 220, spms225 = 225,
    spms230 = 230, spms235 = 235, spms240 = 240, spms245 = 245, spms250 = 250, spms300 = 300,
    spms350 = 350, spms400 = 400, spms450 = 450, spms500 = 500
} TStatusProhibit ;

    /* Rel 5.3: Coverity 32375 Fix Start */
typedef enum MaxRetxThresholdT {
    t0 = 0, t1 = 1, t2 = 2 , t3 = 3, t4 = 4, t6 = 6, t8 = 8, t16 = 16, t32 = 32
} MaxRetxThreshold ;
    /* Rel 5.3: Coverity 32375 Fix End */

/*RLC Rel 2.0 Changes Start*/
typedef enum RlcSendPduTypesT {
    RLC_NOTHING_TO_SEND = 0,
    RLC_CONTROL_PDU ,
    RLC_NEW_DATA_PDU,
    RLC_RETX_DATA_PDU
}RlcSendPduTypes;
/*RLC Rel 2.0 Changes End*/

#ifdef ENDC_ENABLED
#define X2U_RLC_DL_DATA_REQ 12
#define RLC_X2U_DL_DATA_DELIVERY_STATUS 13
#define RLC_X2U_UL_DATA_IND 14
#define X2U_RLC_DATA_DISCARD_REQ 15
#endif


#endif  /* RLCTYPES_H */
