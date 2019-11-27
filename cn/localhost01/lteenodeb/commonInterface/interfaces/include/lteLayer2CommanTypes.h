/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2CommanTypes.h,v 1.3.6.1.4.2.2.2 2010/10/25 08:42:15 gur21010 Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 ****************************************************************************/
#ifndef INCLUDED_LTELAYER2_COMMMON_TYPES_H
#define INCLUDED_LTELAYER2_COMMMON_TYPES_H
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteThread.h"

/****************************************************************************
 * Exported Includes
 ****************************************************************************/


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/
#define TICK_IN_MS 1
#define MAX_THREAD_PRIORITY 99
#define TIMER_THREAD_PRIORITY          MAX_THREAD_PRIORITY-4
#define ULDISPATCHER_THREAD_PRIORITY   MAX_THREAD_PRIORITY-5
#define RLCPDCP_THREAD_PRIORITY        MAX_THREAD_PRIORITY-6
#define ULRECEIVER_THREAD_PRIORITY     MAX_THREAD_PRIORITY-7

#define EXECSCHEDULER_THREAD_PRIORITY	ULDISPATCHER_THREAD_PRIORITY

#define INVALID_UE_INDEX       MAX_UE_SUPPORTED

/* SPR 4864 fix start */
#define UE_IDX_MAPPING_FOR_MAC  0x01
#define UE_IDX_MAPPING_FOR_RLC  0x02
#define UE_IDX_MAPPING_FOR_PDCP 0x04
#define UE_IDX_MAPPING_FOR_S1U  0x08
#define INVALID_RRC_UE_INDEX    65535
#define INVALID_UE_ID           MAX_UE_SUPPORTED
/* SPR 4864 fix end */

#define MAX_THREAD_ID 2

#define MAX_LCS                11
#define MAXIMUM_LC_ID          10
#define INTERFACE_SUCCESS   1
#define INTERFACE_FAILURE   0

/* MAC RX PORTS */
#define MAC_RX_OAM_RRC_PORT    12345    
#define MAC_RX_PHY_PORT        5557
/* MAC TX PORTS */
#define MAC_TX_OAM_PORT        13457
#define MAC_TX_RRC_PORT        13456
#define MAC_TX_PHY_PORT        5556
#define MAC_TX_PHY_TTI_PORT    6544
/*SPR 20863 FIX*/
#define MAC_TX_RRM_PORT        3213
/*SPR 20863 FIX*/
/* RLC TX PORTS */
#define RLC_TX_OAM_PORT        13457 
#define RLC_TX_RRC_PORT        13456
/* RLC RX PORTS */
#define RLC_RX_OAM_RRC_PORT    12340

#if defined(ENDC_ENABLED) &&  defined (PDCP_GTPU_INTF)
/*Dual Connectivity SeNB changes +*/
#define RLC_RX_X2U_PORT        54322
#define GTPU_RX_RLC_PORT        54323
#define GTPU_MODULE_HOST_NAME "127.0.0.1"
/*Dual Connectivity SeNB changes -*/
#endif
/* PDCP TX PORTS */
#define PDCP_TX_OAM_PORT   13457 
#define PDCP_TX_RRC_PORT   13456 
/* PDCP RX PORTS */
#define PDCP_RX_OAM_RRC_PORT   34324 

/* Default hostnames */
#define OAM_MODULE_HOST_NAME "127.0.0.1"
#define RRC_MODULE_HOST_NAME "127.0.0.1"
#define L2_MODULE_HOST_NAME "127.0.0.1"
/* SPR 19288 change start */
#define HOSTNAME_MAX_LEN 64
/* SPR 19288 change end */

/* LTE stack Modules identifiers */
#ifndef OAM_IPR

#define OAM_MODULE_ID            1
#define RRM_MODULE_ID            2
#define RRC_MODULE_ID            3
#define PACKET_RELAY_MODULE_ID   4
#define PDCP_MODULE_ID           5
#define RLC_MODULE_ID            6
#define MAC_MODULE_ID            7
#define SON_MODULE_ID            10

#endif

#define TRAFFIC_GEN_MODULE_ID    100

/* SPR 21412 Fix Start */
/* SPR 21738 Fix Start */
/* Code Deleted */
#define CELL_DELETE             1
#define CELL_CONFIG             2
#define CELL_START              3
#define CELL_STOP               4
/* Code Deleted */
/* SPR 21738 Fix End */
/* SPR 21412 Fix End */

#define SIZEOF_UINT16 2
#define SIZEOF_UINT32 4
#define SIZEOF_UINT64 8

/*Changes DROP1*/
/*because maxNodes in CIRCQ are always 2^n*/
#ifdef DL_UL_SPLIT
#define MAX_CIRCQ_NODES 128
#endif
/*Changes DROP1*/


#define LTE_ROOT_ENV    getenv("LTE_ROOT")
#define PORT_FILE_NAME  "../cfg/eNodeB_Configuration.cfg"

#define GET_LAYER2_PORT_CONFIG_FILE(filename) {\
    strNCpy(filename,PORT_FILE_NAME,sizeof(PORT_FILE_NAME));\
}

#define UE_SIM_PORT_FILE_NAME  "../cfg/UESIM_Configuration.cfg"

#define GET_UE_SIM_PORT_CONFIG_FILE(filename) {\
    strNCpy(filename,UE_SIM_PORT_FILE_NAME,sizeof(UE_SIM_PORT_FILE_NAME));\
}


#define LTE_GET_U16BIT(p_buff)                                        \
    ((UInt16)(*(p_buff) << 8) |                                        \
        (UInt16)(*(p_buff + 1)))

/* to read a 24 bit value starting at the location p_buff */
#define LTE_GET_U24BIT(p_buff)                                        \
    ((UInt32)(*(p_buff) << 16) |                                       \
        (UInt32)(*(p_buff + 1) << 8) |                                 \
        (UInt32)(*(p_buff + 2)))

/* to read a 32 bit value starting at the location p_buff */
#define LTE_GET_U32BIT(p_buff)                                        \
    ((UInt32)(*(p_buff) << 24) |                                       \
        (UInt32)(*(p_buff + 1) << 16) |                                \
        (UInt32)(*(p_buff + 2) << 8) |                                 \
        (UInt32)(*(p_buff + 3)))
/* to read a 40 bit value starting at the location p_buff */
#define LTE_GET_U40BIT(p_buff)                                        \
        (((UInt64)(*(p_buff)) << 32) |                                       \
        (UInt64)(*(p_buff + 1) << 24) |                                \
        (UInt64)(*(p_buff + 2) << 16) |                                 \
        (UInt64)(*(p_buff + 3) << 8) |                                \
        (UInt64)(*(p_buff + 4)))

/* to read a 64 bit value starting at the location p_buff */

#define LTE_GET_U64BIT(p_buff)                                        \
        ((UInt64)(*(p_buff) << 56) |                                       \
                 (UInt64)(*(p_buff + 1) << 48) |                                \
                 (UInt64)(*(p_buff + 2) << 40) |                                 \
                 (UInt64)(*(p_buff + 3) << 32) |                                \
                 (UInt64)(*(p_buff + 4) << 24) |                                \
                 (UInt64)(*(p_buff + 5) << 16) |                                \
                 (UInt64)(*(p_buff + 6) << 8) |                                \
                 (UInt64)(*(p_buff + 7)))

/* macros to set U16bit, U24bit, and U32bit values onto buffer */
/* p_buff must be typecast to a (U8bit *) before using these macros */

/* to write a 16 bit value starting from the location p_buff */
#define LTE_SET_U16BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) >> 8) ;                              \
        *(p_buff + 1) = (UInt8)(val);                                  \
    } while (0)

/* to write a 24 bit value starting from the location p_buff */
#define LTE_SET_U24BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) >> 16);                              \
        *(p_buff + 1) = (UInt8)((val) >> 8);                           \
        *(p_buff + 2) = (UInt8)(val);                                  \
    } while (0)

/* to write a 32 bit value starting from the location p_buff */
#define LTE_SET_U32BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) >> 24);                              \
        *(p_buff + 1) = (UInt8)((val) >> 16);                          \
        *(p_buff + 2) = (UInt8)((val) >> 8);                           \
        *(p_buff + 3) = (UInt8)(val);                                  \
    } while (0)

/* to write a 64 bit value starting from the location p_buff */
#define LTE_SET_U64BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) >> 56);                              \
        *(p_buff + 1) = (UInt8)((val) >> 48);                          \
        *(p_buff + 2) = (UInt8)((val) >> 40);                           \
        *(p_buff + 3) = (UInt8)((val) >> 32);                           \
        *(p_buff + 4) = (UInt8)((val) >> 24);                           \
        *(p_buff + 5) = (UInt8)((val) >> 16);                           \
        *(p_buff + 6) = (UInt8)((val) >> 8);                           \
        *(p_buff + 7) = (UInt8)(val);                                  \
    } while (0)



#ifdef MAC_PHY_INT_LITTLE_ENDIAN
#define LTE_PHY_INT_GET_U16BIT(p_buff)                                        \
    ((UInt16)(*(p_buff) ) |                                        \
        (UInt16)(*(p_buff + 1)<< 8))

/* to read a 32 bit value starting at the location p_buff */
#define LTE_PHY_INT_GET_U32BIT(p_buff)                                        \
    ((UInt32)(*(p_buff) ) |                                       \
        (UInt32)(*(p_buff + 1) << 8) |                                \
        (UInt32)(*(p_buff + 2) << 16) |                                 \
        (UInt32)(*(p_buff + 3)<< 24))

/* macros to set U16bit, U24bit, and U32bit values onto buffer */
/* p_buff must be typecast to a (U8bit *) before using these macros */

/* to write a 16 bit value starting from the location p_buff */
#define LTE_PHY_INT_SET_U16BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val) ) ;                              \
        *(p_buff + 1) = (UInt8)((val)>> 8);                                  \
    } while (0)

/* to write a 32 bit value starting from the location p_buff */
#define LTE_PHY_INT_SET_U32BIT(p_buff, val)                                   \
    do {                                                               \
        *(p_buff) = (UInt8)((val));                              \
        *(p_buff + 1) = (UInt8)((val) >> 8);                          \
        *(p_buff + 2) = (UInt8)((val) >> 16);                           \
        *(p_buff + 3) = (UInt8)((val) >> 24);                                  \
    } while (0)
#else /*These Will Work when the host platform is Big Endian Itself*/

#define LTE_PHY_INT_GET_U16BIT LTE_GET_U16BIT
#define LTE_PHY_INT_GET_U32BIT LTE_GET_U32BIT
#define LTE_PHY_INT_SET_U16BIT LTE_SET_U16BIT
#define LTE_PHY_INT_SET_U32BIT LTE_SET_U32BIT


#endif

#define MAX_RLC_SDU_IN_PDU_FOR_KPI 20

#define MAC_EMULATOR_LOG(param1,...)

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/** SPR 1753 Start **/
extern UInt8 RLC_MULTI_BUFFER_FLAG; 
/** SPR 1753 End **/

typedef enum VoiceSilenceTypeT
{
    VOICE_PKT        = 0,
    SILENCE_PKT      = 1,
    PDCP_CONTROL_PKT = 2
}VoiceSilenceType;

#define S8  signed char
#define U32 unsigned int
#define U64 unsigned long long 
#define S32 signed int
#define U16 UInt16
#define U8  UInt8
#define PDCP_INTERFACE_API_HEADER_SIZE 12
#define PDCP_API_HEADER_SIZE           16

#ifdef LOG_PRINT_ENABLED
extern UInt32 loglevelEnabled_g;
extern UInt32 macloglevelEnabled_g;
extern UInt32 pdcploglevelEnabled_g;
extern UInt32 rlcloglevelEnabled_g;
extern UInt32 gtpuloglevelEnabled_g;
extern UInt64 logCategoryEnabled_g;
extern UInt64 maclogCategoryEnabled_g;
extern UInt64 pdcplogCategoryEnabled_g;
extern UInt64 rlclogCategoryEnabled_g;
extern UInt64 gtpulogCategoryEnabled_g;
#endif
extern void l2_map_ext_loglevel_to_intern_loglevel(UInt8 *p_log_level);

typedef enum healthMonitorLayerIdT
{
    MAC_LAYER_ID = 0,
    RLC_LAYER_ID,
    PDCP_LAYER_ID,
    GTPU_LAYER_ID,
    MAX_L2_LAYER
} healthMonitorLayerId;

typedef enum hsmMacThreadTypeT
{
    MAC_MAIN_THREAD = 0,
    MAC_DL_THREAD,
#ifdef DL_UL_SPLIT
    MAC_UL_THREAD,
#endif 
    MAC_MAX_THREAD_ID
} hsmMacThreadType;

typedef enum hsmRlcThreadTypeT
{
    RLC_UL_THREAD = 0,
    RLC_DL_THREAD,
    RLC_MAX_THREAD_ID
} hsmRlcThreadType;

typedef enum hsmPdcpThreadTypeT
{
    PDCP_THREAD = 0,
    PDCP_MAX_THREAD_ID
} hsmPdcpThreadType;

typedef enum hsmGtpuThreadTypeT
{
    GTPU_THREAD = 0,
    GTPU_MAX_THREAD_ID
} hsmGtpuThreadType;

/* CA changes start */
typedef struct MacRlcHealthMonitorT
{
    /* MAC thread is per cell */
    /* MAC control thread is updated for both cell with same tick */
    UInt32 macThread[MAC_MAX_THREAD_ID];
    /* DLRLC thread is per cell, ULRLC thread is updated for both cell with same tick */
    UInt32 rlcThread[RLC_MAX_THREAD_ID];
} MacRlcHealthMonitor;

typedef struct HealthMonitorT
{
    /* MAC/RLC thread is per cell */
    MacRlcHealthMonitor rlcMacThread[MAX_NUM_CELL];
    /* PDCP thread is one thread for all cells */
    UInt32 pdcpThread[PDCP_MAX_THREAD_ID];
    /* Packet Relay/GTPU thread is one thread for all cells */
    UInt32 egtpuThread[GTPU_MAX_THREAD_ID];
}HealthMonitor;

extern HealthMonitor l2ThreadHealthStoreTick_g;

typedef struct PhyPortInfoT
{
    UInt16 txPortPHY;
    UInt16 rxPortPHY;
    UInt16  phyControlTxPort;
    UInt16  phyControlRxPort;
    Char8 phyIP [64];
}PhyPortInfo;

extern PhyPortInfo phyPortInfo_g[MAX_NUM_CELL];

typedef struct LteLayerCoreInfoT
{
    UInt8 lteLayer2PdcpCoreNum;

#ifdef LTE_EMBMS_SUPPORTED
    UInt8 lteLayer2SyncCoreNum;
#endif
    UInt8 layer2ciphCoreNum;
    UInt8 lteLayer2ULRLCCoreNum;
    /* In case DL/UL split this is DL core and
     * w/o DL/UL it is MAC core */
    UInt8 lteLayer2DLCoreNum[MAX_NUM_CELL];
    UInt8 lteLayer2ULCoreNum[MAX_NUM_CELL];
    UInt8 lteLayer2EncoderCoreNum[MAX_NUM_CELL];
#ifdef FLEXRAN
    UInt8 lteLayer2UlRlcCoreNum;
#endif
}LteLayer2CoreInfo;

extern  LteLayer2CoreInfo lteLayer2CoreInfo_g;

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
void layer2CommonInitCellIndexMapping(void);
InternalCellIndex layer2CommonUpdateCellIndexMapping(RrcCellIndex cellIndex);
UInt8 layer2CommonLookupIfValidCellIndex(RrcCellIndex cellIndex);
InternalCellIndex layer2CommonGetInternalCellIndex(RrcCellIndex rrcCellIndex);
RrcCellIndex layer2CommonGetRRCCellIndex(InternalCellIndex internalCellIndex);
void layer2CommonDeleteCellIndexMapping(RrcCellIndex cellIndex);
void layer2CommonRevertCellIndexMapping(InternalCellIndex internalIndex);
void layer2CommonSetActiveCellTick(InternalCellIndex internalIndex);
InternalCellIndex layer2CommonGetActiveCellTick(void);
UInt16 layer2CommonGetRlcNumOfSupportedAmLC(void);
UInt16 layer2CommonGetRlcNumOfSupportedUmLC(void);
UInt16 layer2CommonGetPdcpNumOfSupportedLC(void);
void initUeIdxMap(void);
UInt16 allocateUeIdxFromRrcUeIdx(UInt16 rrcUeIdx);
UInt16 setUeIdxValid(UInt16 ueIdx, UInt16 layer);
UInt16 setUeIdxInvalid(UInt16 ueIdx, UInt16 layer);
void freeUeIdx(UInt16 ueIdx);
UInt16 getRrcUeIdxFromUeIdx(UInt16 ueIdx);
UInt16 getUeIdxFromRrcUeIdx(UInt16 rrcUeIdx);
UInt32 layer2CommonIsCellRunning(InternalCellIndex internalCellIndex);
/* CA Changes end */
#endif  
