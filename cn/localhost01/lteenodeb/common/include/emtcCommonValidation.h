/****************************************************************************
 *
 *  aricent -
 *
 *  copyright (c) aricent.
 *
 ****************************************************************************
 *
 *  $Id: emtcCommonValidation.h
 *
 ****************************************************************************
 *
 *  File Description : contains the function prototypes and data structures
 *  needed for validation of emtc comfiguration.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 ****************************************************************************/

#ifndef EMTC_COMMON_VALIDATION_H
#define EMTC_COMMON_VALIDATION_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteTypes.h"
#ifdef OAM_IPR
#include <cspl.h>
#endif
/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Macro Definitions
****************************************************************************/
#define COM_MEMSET                             memset
#define COM_PNULL                                  0
#define EMTC_SF_UPPER_LIMIT                    10240    
#define NUM_PRBS_PER_NARROWBAND                    6
#define CELL_EMTC                                  1 
#define CELL_MIXED                                 2  
#define COM_ZERO                                   0
#define COM_ONE                                    1
#define COM_FOUR                                   4
#define COM_TWO                                    2
#define COM_EIGHT                                  8
#define COM_SIXTEEN                               16
#define COM_THIRTY_TWO                            32
#define COM_SIXTY_FOUR                            64
#define COM_ONE_TWENTY_EIGHT                     128
#define COM_TWO_FIFTY_SIX                        256 
#define MAX_NUM_PRB                              100
/* Cov_122416_Fix_Start */
#define EMTC_PRB_BITMAP                          0xC0 
/* Cov_122416_Fix_End */
 
typedef enum ComReturnTypeT
{
    COM_FAILURE = 0,
    COM_SUCCESS = 1
}ComReturnType;

typedef enum UeTypeT
{
  UE_TYPE_LTE  = 0, 
  UE_TYPE_EMTC = 1
}UeType;

typedef enum EmtcCellBwT
{
    CELL_BW_1_4 = 0,
    CELL_BW_3   = 1,
    CELL_BW_5   = 2, 
    CELL_BW_10  = 3,
    CELL_BW_15  = 4,
    CELL_BW_20  = 5
}EmtcCellBw;

#define MPDCCH_START_SF_1DOT5                      1
#define MPDCCH_START_SF_2DOT5                      3
#define SI_PERIODICITY_16RF                       16
#define RF_TO_MS_FACTOR                           10
#define MS_TO_RF_FACTOR                           10
/* Number of UE for eMTC */
#define MAX_NUM_SUPPORTED_EMTC_UE                 64
#define MAX_NUM_SUPPORTED_EMTC_UE_PER_UECC  MAX_NUM_SUPPORTED_EMTC_UE
#define MIN_NUM_SUPPORTED_EMTC_UE                  1
/* SI TB size for eMTC SIs */
typedef enum EmtcSiTbBitsT
{
    EMTC_SI_TB_B152_BITS  = 152,
    EMTC_SI_TB_B208_BITS  = 208,
    EMTC_SI_TB_B256_BITS  = 256,
    EMTC_SI_TB_B328_BITS  = 328,
    EMTC_SI_TB_B408_BITS  = 408,
    EMTC_SI_TB_B504_BITS  = 504,
    EMTC_SI_TB_B600_BITS  = 600,
    EMTC_SI_TB_B712_BITS  = 712,
    EMTC_SI_TB_B808_BITS  = 808,
    EMTC_SI_TB_B936_BITS  = 936
}EmtcSiTbBits;

#define EMTC_SI_TB_INVALID                      0xFF
#define EMTC_INVALID_8_BIT_VAL                  0xFF
#define EMTC_BIT_TO_BYTE_CONV_FACTOR               8
/* Modification period */
#define EMTC_MIN_MOD_PERIOD                       64
#define EMTC_MAX_MOD_PERIOD                    32768
/* Paging for eMTC */
#define EMTC_MAC_PDSCH_PAGIN_REP_MAX               7
/* EPDCCH config in RRCConnReconf for eMTC */
#define EMTC_MAX_PUCCH_RESOURCE_START_OFFSET    2047
#define EMTC_MAX_DMRS_SCRAMBLING_SEQUENCE_INT    503
/*
** As per 3GPP spec 36.331, section 6.3.2 -EPDCCH-Config
** resourceBlockAssignment-r11 is a bit string of (SIZE(4..38))
** Below value is derived keeping 38 bits set starting from LSB
*/
#define EMTC_MAX_RB_ASSIGNMENT_SIZE              274877906943
#define EMTC_MIN_RB_ASSIGNMENT_SIZE               15
#define EMTC_MAX_NPUCCH_IDENTITY                 503
#define EMTC_MAX_EPDCCH_CONFIG_ID                  1
#define EMTC_MIN_EPDCCH_SET                        1
#define EMTC_MAX_EPDCCH_SET                        2
#define EMTC_MAX_RB_ASSIGNMENT_OCTET_SIZE          5 
/* SI for eMTC */
#define EMTC_MAX_SI_BUFFER_LEN                   277
#define EMTC_MAX_SI_WINDOW_MS                    200
#define EMTC_MIN_SI_WINDOW_MS                     20
#define EMTC_MAX_STARTING_SFN                   1023
#define EMTC_MAX_SIB_COUNT                         7
#define EMTC_MIN_SI_COUNT                          1
#define EMTC_MAX_SI_COUNT                          8
/* SIB1 BR for EMTC */
#define EMTC_MAX_SIB1_BR_INDEX                    18
#define EMTC_MAX_SIB1_BR_LENGTH                 1024
#define EMTC_MIN_START_SYMBOL                      1
#define EMTC_MAX_START_SYMBOL                      4
#define EMTC_MAX_SYSTEM_INFO_VALUE_TAG             3
#define EMTC_MAX_DL_SF_PATTERN_BYTES               5
#define EMTC_MIN_DL_SF_PATTERN_BYTES               2
/*
** As per 3GPP spec 36.331, section 6.2.2 -fdd-UplinkSubframeBitmapBR
** fdd-UplinkSubframeBitmapBR-r13 is a bit string of (SIZE (10)
** Below value is derived keeping 10 bits set starting from MSB
*/
#define EMTC_MAX_FDD_UPLINK_SF_BITMAP          65472
/* PRACH config for eMTC */
#define EMTC_MAX_PRACH_HOPPING_OFFSET             94
#define EMTC_MAX_PRACH_CONFIG_INDEX               63
#define EMTC_MAX_PRACH_FREQ_OFFSET                94
#define EMTC_MAX_RSRP_RANGE                       97
#define EMTC_MAX_RSRP_LIST_SIZE                    3
#define EMTC_MIN_CE_LEVEL                          1
#define EMTC_MAX_CE_LEVEL                          2
#define EMTC_MAX_INITIAL_CE_LEVEL                  1
#define EMTC_MAX_NARROWBAND_TO_MONITOR			   2
/* RACH  config for eMTC */
#define EMTC_MAX_PREAMBLE                         63
#define EMTC_MAX_N1_PUCCH_AN                    2047 
/* Narrowband config for eMTC */
#define EMTC_MIN_AVAILABLE_NARROWBAND              1
#define EMTC_MAX_AVAILABLE_NARROWBAND             16
#define EMTC_MAX_MCS                              15
#define EMTC_MAX_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_NUM_VALUES   8
#define EMTC_PDSCH_PAGING_REP_MIN                 1
#define EMTC_PDSCH_PAGING_REP_MAX                 8
#define EMTC_MPDCCH_NUM_REPETITION_PAGING_MAX     4
#define EMTC_MPDCCH_NUM_REPETITION_PAGING_MIN     1
#define EMTC_SFN_GAP_MAX                          256
#define EMTC_PAGING                               1

#define EMTC_MAX_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_NUM_VALUES   8
typedef enum EmtcNumRepPerPreambleAttemptT
{
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N1_VAL   = 1,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N2_VAL   = 2,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N4_VAL   = 4,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N8_VAL   = 8,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N16_VAL  = 16,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N32_VAL  = 32,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N64_VAL  = 64,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N128_VAL = 128
}EmtcNumRepPerPreambleAttempt;

#define EMTC_NUM_RA_PREAMBLES_LTE_NUM_VALUES        16
typedef enum EmtcNumRaPreamblesLteT
{
    EMTC_NUM_RA_PREAMBLES_LTE_N4_VAL  = 4,
    EMTC_NUM_RA_PREAMBLES_LTE_N8_VAL  = 8,
    EMTC_NUM_RA_PREAMBLES_LTE_N12_VAL = 12,
    EMTC_NUM_RA_PREAMBLES_LTE_N16_VAL = 16,
    EMTC_NUM_RA_PREAMBLES_LTE_N20_VAL = 20,
    EMTC_NUM_RA_PREAMBLES_LTE_N24_VAL = 24,
    EMTC_NUM_RA_PREAMBLES_LTE_N28_VAL = 28,
    EMTC_NUM_RA_PREAMBLES_LTE_N32_VAL = 32,
    EMTC_NUM_RA_PREAMBLES_LTE_N36_VAL = 36,
    EMTC_NUM_RA_PREAMBLES_LTE_N40_VAL = 40,
    EMTC_NUM_RA_PREAMBLES_LTE_N44_VAL = 44,
    EMTC_NUM_RA_PREAMBLES_LTE_N48_VAL = 48,
    EMTC_NUM_RA_PREAMBLES_LTE_N52_VAL = 52,
    EMTC_NUM_RA_PREAMBLES_LTE_N56_VAL = 56,
    EMTC_NUM_RA_PREAMBLES_LTE_N60_VAL = 60,
    EMTC_NUM_RA_PREAMBLES_LTE_N64_VAL = 64
}EmtcNumRaPreamblesLte;

#define EMTC_RA_RESP_WINDOW_SIZE_NUM_VALUES        8
typedef enum EmtcRaRespWindowSizeT
{
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF20  = 20,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF50  = 50,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF80  = 80,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF120 = 120,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF180 = 180,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF240 = 240,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF320 = 320,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF400 = 400
}EmtcRaRespWindowSize;

#define EMTC_SI_WINDOW_LEN_NUM_VALUES              7
typedef enum EmtcSiWindowLenT
{
    EMTC_SI_WINDOW_LEN_20MS  = 20,
    EMTC_SI_WINDOW_LEN_40MS  = 40,
    EMTC_SI_WINDOW_LEN_60MS  = 60,
    EMTC_SI_WINDOW_LEN_80MS  = 80,
    EMTC_SI_WINDOW_LEN_120MS = 120,
    EMTC_SI_WINDOW_LEN_160MS = 160,
    EMTC_SI_WINDOW_LEN_200MS = 200
}EmtcSiWindowLen;

#define EMTC_MPDCCH_NUM_REP_RA_NUM_VALUES          9
typedef enum EmtcMpdcchNumRepRaT
{
    EMTC_MPDCCH_NUM_REP_RA_R1_VAL   = 1,
    EMTC_MPDCCH_NUM_REP_RA_R2_VAL   = 2,
    EMTC_MPDCCH_NUM_REP_RA_R4_VAL   = 4,
    EMTC_MPDCCH_NUM_REP_RA_R8_VAL   = 8,
    EMTC_MPDCCH_NUM_REP_RA_R16_VAL  = 16,
    EMTC_MPDCCH_NUM_REP_RA_R32_VAL  = 32,
    EMTC_MPDCCH_NUM_REP_RA_R64_VAL  = 64,
    EMTC_MPDCCH_NUM_REP_RA_R128_VAL = 128,
    EMTC_MPDCCH_NUM_REP_RA_R256_VAL = 256
}EmtcMpdcchNumRepRa;

#define EMTC_MPDCCH_NUM_REP_UESS_NUM_VALUES        9
typedef enum EmtcMpdcchNumRepUessT
{
    EMTC_MPDCCH_NUM_REP_UESS_R1_VAL   = 1,
    EMTC_MPDCCH_NUM_REP_UESS_R2_VAL   = 2,
    EMTC_MPDCCH_NUM_REP_UESS_R4_VAL   = 4,
    EMTC_MPDCCH_NUM_REP_UESS_R8_VAL   = 8,
    EMTC_MPDCCH_NUM_REP_UESS_R16_VAL  = 16,
    EMTC_MPDCCH_NUM_REP_UESS_R32_VAL  = 32,
    EMTC_MPDCCH_NUM_REP_UESS_R64_VAL  = 64,
    EMTC_MPDCCH_NUM_REP_UESS_R128_VAL = 128,
    EMTC_MPDCCH_NUM_REP_UESS_R256_VAL = 256
}EmtcMpdcchNumRepUess;

#define EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_NUM_VALUES 3
typedef enum EmtcPdschMaxNumRepCeModeAT
{
    EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R8_VAL  = 8,
    EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R16_VAL = 16,
    EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R32_VAL = 32
}EmtcPdschMaxNumRepCeModeA;

#define EMTC_SI_REP_NUM_VALUES                    4
typedef enum EmtcSiRepValT
{
    EMTC_SI_REP_VAL_EVERY_RF        = 1,
    EMTC_SI_REP_VAL_EVERY_SECOND_RF = 2,
    EMTC_SI_REP_VAL_EVERY_FOURTH_RF = 4,
    EMTC_SI_REP_VAL_EVERY_EIGHTH_RF = 8
}EmtcSiRepVal;

#define EMTC_SI_PERIODICITY_NUM_VALUES            7
typedef enum EmtcSiPeriodicityValT
{
    EMTC_SI_PERIODICITY_VAL_8RF   = 8,
    EMTC_SI_PERIODICITY_VAL_16RF  = 16,
    EMTC_SI_PERIODICITY_VAL_32RF  = 32,
    EMTC_SI_PERIODICITY_VAL_64RF  = 64,
    EMTC_SI_PERIODICITY_VAL_128RF = 128,
    EMTC_SI_PERIODICITY_VAL_256RF = 256,
    EMTC_SI_PERIODICITY_VAL_512RF = 512
}EmtcSiPeriodicityVal;

#define EMTC_MPDCCH_NUM_REP_PAGING_NUM_VALUES     9
typedef enum EmtcMpdcchNumRepPagingT
{
    EMTC_MPDCCH_NUM_REP_PAGING_R1_VAL   = 1,
    EMTC_MPDCCH_NUM_REP_PAGING_R2_VAL   = 2,
    EMTC_MPDCCH_NUM_REP_PAGING_R4_VAL   = 4,
    EMTC_MPDCCH_NUM_REP_PAGING_R8_VAL   = 8,
    EMTC_MPDCCH_NUM_REP_PAGING_R16_VAL  = 16,
    EMTC_MPDCCH_NUM_REP_PAGING_R32_VAL  = 32,
    EMTC_MPDCCH_NUM_REP_PAGING_R64_VAL  = 64,
    EMTC_MPDCCH_NUM_REP_PAGING_R128_VAL = 128,
    EMTC_MPDCCH_NUM_REP_PAGING_R256_VAL = 256
}EmtcMpdcchNumRepPaging;

#define EMTC_DEFAULT_PAGING_CYCLE_NUM_VALUES      4
typedef enum EmtcDefaultPagingCycleT
{
    EMTC_DEFAULT_PAGING_CYCLE_RF32 = 32,
    EMTC_DEFAULT_PAGING_CYCLE_RF64 = 64,
    EMTC_DEFAULT_PAGING_CYCLE_RF128 = 128,
    EMTC_DEFAULT_PAGING_CYCLE_RF256 = 256
}EmtcDefaultPagingCycle;

#define EMTC_SIB1_BR_REP_NUM_VALUES               3
typedef enum EmtcSib1BrRepT
{
    EMTC_SIB1_BR_REP_VAL_N4 = 4,
    EMTC_SIB1_BR_REP_VAL_N8 = 8,
    EMTC_SIB1_BR_REP_VAL_N16 = 16
}EmtcSib1BrRep;

#define EMTC_BW_NUM_VALUES                        6
typedef enum EmtcBwRbT
{
    NRB_6  = 6,
    NRB_15 = 15,                                 
    NRB_25 = 25,                               
    NRB_50 = 50,                             
    NRB_75 = 75,                              
    NRB_100 = 100
}EmtcBwRb;

#define EMTC_MIN_RXLEVMIN                       -70
#define EMTC_MAX_RXLEVMIN                       -22
#define EMTC_MIN_Q_QUALMINRSRQ                  -34
#define EMTC_MAX_Q_QUALMINRSRQ                   -3

#define EMTC_MAX_START_SF_UESS_NUM_VALUES         8
#define EMTC_START_SF_UESS_VAL_1                  1
#define EMTC_START_SF_UESS_VAL_1DOT5              1.5
#define EMTC_START_SF_UESS_VAL_2                  2
#define EMTC_START_SF_UESS_VAL_2DOT5              2.5
#define EMTC_START_SF_UESS_VAL_4                  4
#define EMTC_START_SF_UESS_VAL_5                  5
#define EMTC_START_SF_UESS_VAL_8                  8
#define EMTC_START_SF_UESS_VAL_10                 10

#define EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX       8  
#define EMTC_MPDCCH_START_SF_CSS_RA_FDD_V1        1
#define EMTC_MPDCCH_START_SF_CSS_RA_FDD_V1DOT5    1.5
#define EMTC_MPDCCH_START_SF_CSS_RA_FDD_V2        2
#define EMTC_MPDCCH_START_SF_CSS_RA_FDD_V2DOT5    2.5
#define EMTC_MPDCCH_START_SF_CSS_RA_FDD_V4        4
#define EMTC_MPDCCH_START_SF_CSS_RA_FDD_V5        5
#define EMTC_MPDCCH_START_SF_CSS_RA_FDD_V8        8
#define EMTC_MPDCCH_START_SF_CSS_RA_FDD_V10       10


/* Prach Start sf for eMTC */
#define EMTC_PRACH_START_SF_NUM_VALUES            8
typedef enum EmtcPrachStartSfT
{
EMTC_PRACH_START_SF_SF2   = 2,
EMTC_PRACH_START_SF_SF4   = 4,
EMTC_PRACH_START_SF_SF8   = 8,
EMTC_PRACH_START_SF_SF16  = 16,
EMTC_PRACH_START_SF_SF32  = 32,
EMTC_PRACH_START_SF_SF64  = 64,
EMTC_PRACH_START_SF_SF128 = 128,
EMTC_PRACH_START_SF_SF256 = 256 
}EmtcPrachStartSf;

#define EMTC_NUM_PRB_PAIR_VAL_NUM_VALUES         3 
typedef enum
{
    EMTC_NUM_PRB_PAIR_VAL_N2 = 2,
    EMTC_NUM_PRB_PAIR_VAL_N4 = 4,
    EMTC_NUM_PRB_PAIR_VAL_N6 = 6,
}EmtcNumPrbPairVal_et;


/******************************************************************************
*  STRUCTURE 
******************************************************************************/
typedef struct _EmtcPreambleInfo_t
{
    UInt8 firstPreamble;
    UInt8 lastPreamble;
}EmtcPreambleInfo_t;

typedef struct _EmtcSchedulingInfoList_t
{
    UInt8                count;             
    UInt8                siPeriodicity[EMTC_MAX_SI_COUNT];
}EmtcSchedulingInfoList_t;

/* 
 ** This strcuture is used to derive the narrowband and
 ** rb index based on Prb Index
*/
typedef struct _EmtcPrbIndexToNbMap
{
    UInt8  nbIndex;
    UInt8  prbIndex;
}EmtcPrbIndexToNbMap_t;

/******************************************************************************
*  ENUMERATIONS 
******************************************************************************/
/*! \enum EmtcDuplexingMode_et 
 *  \brief an enum that specifies duplexing mode for emtc
 */ typedef enum
{
    EMTC_DUPLEXING_MODE_FD,  
    EMTC_DUPLEXING_MODE_HD,                                                               
    EMTC_DUPLEXING_MODE_MAX = EMTC_DUPLEXING_MODE_HD  
}EmtcDuplexingMode_et;
/*! \enum EmtcCeMode_et 
 *  \brief an enum that specifies type of ce mode
 */ 
typedef enum
{
  EMTC_CE_MODE_A,
  EMTC_CE_MODE_B,
  EMTC_CE_MODE_MAX = EMTC_CE_MODE_B
}EmtcCeMode_et;

/*! \enum EmtcSib1BrRepetitionNum_et 
 *  \brief an enum that specifies maximum number of SIB1 Br repetition
 *  \Reference: 3GPP 36.213, 
 */ 
typedef enum
{
    EMTC_SIB1_BR_REPETITION_N4,
    EMTC_SIB1_BR_REPETITION_N8,
    EMTC_SIB1_BR_REPETITION_N16,
    EMTC_SIB1_BR_REPETITION_NMAX =
       EMTC_SIB1_BR_REPETITION_N16
}EmtcSib1BrRepetitionNum_et;

/*! \enum EmtcCeParametersR13_et
 *  \brief an enum that specifies whether the UE supports operation in
 *         CE mode A and/or B
 *  \Reference: 3GPP 36.331 , CE-Parameters-r13
 */
typedef enum
{
    EMTC_CE_MODE_A_SUPPORTED,
    EMTC_CE_MODE_B_SUPPORTED,
    EMTC_CE_MODE_A_AND_B_SUPPORTED,
    EMTC_CE_MODE_MAX_SUPPORTED = EMTC_CE_MODE_A_AND_B_SUPPORTED
}EmtcCeParametersR13_et;

/*! \enum EmtcUeCategoryDl_et
 *  \brief an enum that specifies UE Category for Downlink transmission
 *         of release 13
 *  \Reference: 3GPP 36.331 , ue-CategoryDL-v1310
 */
typedef enum
{
    EMTC_DL_UE_CATEGORY_N17,
    EMTC_DL_UE_CATEGORY_M1,
    EMTC_DL_UE_CATEGORY_MAX = EMTC_DL_UE_CATEGORY_M1,
}EmtcUeCategoryDl_et;

/*! \enum EmtcUeCategoryUlV1310_et
 *  \brief an enum that specifies UE Category for Uplink transmission
 *         of release 13
 *  \Reference: 3GPP 36.331 , ue-CategoryUL-v1310
 */
typedef enum
{
    EMTC_UL_UE_CATEGORY_N14,
    EMTC_UL_UE_CATEGORY_M1,
    EMTC_UL_UE_CATEGORY_MAX = EMTC_UL_UE_CATEGORY_M1
}EmtcUeCategoryUlV1310_et;


/*! \enum EmtcOperationMode_et
 *  \brief an enum that specifies operation mode of cell
 *  \when EMTC is enabled
 */
typedef enum
{
    EMTC_OPERATION_MODE_LTE = 0,
    EMTC_OPERATION_MODE_STANDALONE = 1,
    EMTC_OPERATION_MODE_MIXED,
    EMTC_OPERATION_MODE_MAX = EMTC_OPERATION_MODE_MIXED
}EmtcOperationMode_et;

/*! \enum EmtcDirectIndication_et
 *  \brief an enum that specifies paging cause
 *  \when EMTC is enabled
 */
typedef enum
{
    EMTC_DIRECT_INDICATION_SI_MODIF = 1,
    EMTC_DIRECT_INDICATION_MAX = EMTC_DIRECT_INDICATION_SI_MODIF 
}EmtcDirectIndication_et;

/*! \enum EmtcCeLevel_et
 *  \brief an enum that specifies CE level of BL UEs and UEs in CE
 *  \Reference: 3GPP 36.331 , 6.3.2 rach-CE-LevelInfoList-r13 
 */
typedef enum
{
    EMTC_CE_LEVEL_0,
    EMTC_CE_LEVEL_1,
    EMTC_CE_LEVEL_MAX = EMTC_CE_LEVEL_1
}EmtcCeLevel_et;

/*! \enum EmtcModificationPeriodCoeff_et 
 *  \brief an enum that specifies modification period in terms of RF 
 *  \Reference: 3GPP 36.331,modificationPeriodCoeff & modificationPeriodCoeff-v1310
 */
typedef enum
{
  EMTC_MOD_PERIOD_COEFF_N2,
  EMTC_MOD_PERIOD_COEFF_N4,
  EMTC_MOD_PERIOD_COEFF_N8,
  EMTC_MOD_PERIOD_COEFF_N16,
  EMTC_MOD_PERIOD_COEFF_N64,
  EMTC_MOD_PERIOD_COEFF_MAX =
             EMTC_MOD_PERIOD_COEFF_N64
}EmtcModificationPeriodCoeff_et;

/*! \enum EmtcSibType_et 
 *  \brief an enum that specifies the EMTC sib type 
 */
typedef enum
{
    EMTC_SIB_TYPE_3,
    EMTC_SIB_TYPE_4,
    EMTC_SIB_TYPE_5,
    EMTC_SIB_TYPE_6,
    EMTC_SIB_TYPE_7,
    EMTC_SIB_TYPE_8,
    EMTC_SIB_TYPE_9,
    EMTC_SIB_TYPE_MAX = EMTC_SIB_TYPE_9
}EmtcSibType_et;


/*! \enum EmtcSiWindowLength_et
 *  \brief an enum that specifies SI Window length for EMTC
 *  \Reference: 3GPP 36.331 , si-WindowLength-BR-r13
 */
typedef enum
{
    EMTC_SI_WINDOW_LEN_20,
	EMTC_SI_WINDOW_LEN_40,
	EMTC_SI_WINDOW_LEN_60,
	EMTC_SI_WINDOW_LEN_80,
	EMTC_SI_WINDOW_LEN_120,
    EMTC_SI_WINDOW_LEN_160,
	EMTC_SI_WINDOW_LEN_200,
	EMTC_SI_WINDOW_LEN_MAX = EMTC_SI_WINDOW_LEN_200
}EmtcSiWindowLength_et;

/*! \enum EmtcSiRepetitionPattern_et
 *  \brief an enum that specifies EMTC SI Repetition Pattern for Release 13
 *  \Reference: 3GPP 36.331 , si-RepetitionPattern-r13  
 */
typedef enum
{
	EMTC_SI_REPETITION_EVERY_RF,
	EMTC_SI_REPETITION_EVERY_SECOND_RF,
	EMTC_SI_REPETITION_EVERY_FOURTH_RF,
	EMTC_SI_REPETITION_EVERY_EIGHTH_RF,
	EMTC_SI_REPETITION_MAX = EMTC_SI_REPETITION_EVERY_EIGHTH_RF
}EmtcSiRepetitionPattern_et;


/*! \enum EmtcSiHoppingConfigCommonR13_et
 *  \brief an enum that specifies EMTC SI Hopping Config Info for Release 13
 *  \Reference: 3GPP 36.331 , si-HoppingConfigCommon-r13
 */
typedef enum
{
    EMTC_SI_HOPPING_OFF,
	EMTC_SI_HOPPING_ON,
	EMTC_SI_HOPPING_MAX = EMTC_SI_HOPPING_ON,
}EmtcSiHoppingConfigCommon_et;

/*! \enum EmtcPrachStartingSubframe_et 
 *  \brief an enum that specifies PRACH starting subframe periodicity
 *  \Reference: 3GPP 36.331 , prach-StartingSubframe-r13
 */
typedef enum
{
    EMTC_PRACH_STARTING_SF2,
    EMTC_PRACH_STARTING_SF4,
    EMTC_PRACH_STARTING_SF8,
    EMTC_PRACH_STARTING_SF16,
    EMTC_PRACH_STARTING_SF32,
    EMTC_PRACH_STARTING_SF64,
    EMTC_PRACH_STARTING_SF128,
    EMTC_PRACH_STARTING_SF256,
    EMTC_PRACH_STARTING_SF_MAX = EMTC_PRACH_STARTING_SF256
}EmtcPrachStartingSubframe_et;


/*! \enum EmtcMaxNumPreambleAttemptCe_et 
 *  \brief an enum that specifies maximum number of preamble transmission 
     attempts per CE level
 *  \Reference: 3GPP 36.331 , maxNumPreambleAttemptCE-r13
 */
typedef enum
{
    EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_N3,
    EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_N4,
    EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_N5,
    EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_N6,
    EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_N7,
    EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_N8,
    EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_N10,
    EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_MAX = EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_N10
}EmtcMaxNumPreambleAttemptCe_et;

/*! \ enum EmtcNumRepetitionPerPreambleAttempt_et
 *  \brief an enum that specifies number of PRACH repetitions per attempt 
 *         for each CE level
 *  \Reference: 3GPP 36.331 , numRepetitionPerPreambleAttempt-r13
 */
typedef enum
{
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N1,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N2,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N4,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N8,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N16,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N32,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N64,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N128,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX =
                 EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N128
}EmtcNumRepetitionPerPreambleAttempt_et;

/*! \enum EmtcMpdcchNumRepetitionRa_et
 *  \brief an enum that specifies maximum number of repetitions for 
 *         MPDCCH common search space  for RAR, Msg3 and Msg4
 *  \Reference: 3GPP 36.331 , mpdcch-NumRepetition-RA-r13
 */
typedef enum
{
    EMTC_MPDCCH_NUM_REPETITION_RA_R1,
    EMTC_MPDCCH_NUM_REPETITION_RA_R2,
    EMTC_MPDCCH_NUM_REPETITION_RA_R4,
    EMTC_MPDCCH_NUM_REPETITION_RA_R8,
    EMTC_MPDCCH_NUM_REPETITION_RA_R16,
    EMTC_MPDCCH_NUM_REPETITION_RA_R32,
    EMTC_MPDCCH_NUM_REPETITION_RA_R64,
    EMTC_MPDCCH_NUM_REPETITION_RA_R128,
    EMTC_MPDCCH_NUM_REPETITION_RA_R256,
    EMTC_MPDCCH_NUM_REPETITION_RA_MAX = 
                   EMTC_MPDCCH_NUM_REPETITION_RA_R256
}EmtcMpdcchNumRepetitionRa_et;

/*! \ enum EmtcPuschMaxNumRepetitionCeModeA_et
 *  \brief an enum that specifies maximum value to indicate 
 *         the set of PUSCH repetition numbers for CE mode A
 *  \Reference: 3GPP 36.331 , pusch-maxNumRepetitionCEmodeA-r13
 */
typedef enum
{
    EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_R8,
    EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_R16,
    EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_R32,
    EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_MAX = 
               EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_R32
}EmtcPuschMaxNumRepetitionCeModeA_et;

/*! \enum EmtcPucchNumRepetitionCeMsg4Level_et
 *  \brief an enum that specifies number of repetitions
 *         for PUCCH carrying HARQ response to PDSCH containing 
 *         Msg4 for PRACH CE levels 0
 *  \Reference: 3GPP 36.331 , pucch-NumRepetitionCE-Msg4-Level0-r13,
 *                            pucch-NumRepetitionCE-Msg4-Level1-r13
 */
typedef enum
{
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_N1,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_N2,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_N4,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_N8,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_MAX = 
                  EMTC_PUCCH_NUM_REPETITION_CE_MSG4_N8
}EmtcPucchNumRepetitionCeMsg4Level_et;

/*! \enum EmtcPucchNumRepetitionCeMsg4Level0_et
 *  \brief an enum that specifies number of repetitions
 *         for PUCCH carrying HARQ response to PDSCH containing 
 *         Msg4 for PRACH CE levels 0
 *  \Reference: 3GPP 36.331 , pucch-NumRepetitionCE-Msg4-Level0-r13
 */
typedef enum
{
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_N1,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_N2,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_N4,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_N8,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_MAX = 
                  EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_N8
}EmtcPucchNumRepetitionCeMsg4Level0_et;

/*! \enum EmtcPucchNumRepetitionCeMsg4Level1_et
 *  \brief an enum that specifies number of repetitions for 
 *         PUCCH carrying HARQ response to PDSCH containing Msg4 
 *         for PRACH CE levels 1
 *  \Reference: 3GPP 36.331 , pucch-NumRepetitionCE-Msg4-Level1-r13 
 */
typedef enum
{
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L1_N1,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L1_N2,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L1_N4,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L1_N8,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L1_MAX = 
                   EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L1_N8
}EmtcPucchNumRepetitionCeMsg4Level1_et;


/*! \enum EmtcPucchNumRepetitionCeMsg4Level2_et 
 *  \brief An enum that specifies PUCCH number of repetitions
 *         for CE Level 2
 *  \Reference: 3GPP 36.331 , pucch-NumRepetitionCE-Msg4-Level2-r13
 */
typedef enum
{
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L2_N4,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L2_N8,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L2_N16,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L2_N32,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L2_MAX = 
                      EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L2_N32
}EmtcPucchNumRepetitionCeMsg4Level2_et;

/*! \enum EmtcPucchNumRepetitionCeMsg4Level3_et 
 *  \brief An enum that specifies PUCCH number of repetitions
 *         for CE Level 3
 *  \Reference: 3GPP 36.331 , pucch-NumRepetitionCE-Msg4-Level3-r13
 */
typedef enum
{
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L3_N4,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L3_N8,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L3_N16,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L3_N32,
    EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L3_MAX = 
                  EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L3_N32
}EmtcPucchNumRepetitionCeMsg4Level3_et;

/*! \enum EmtcPdschMaxNumRepetitionCeModeA_et
 *  \brief an enum that specifies maximum value to indicate 
 *         the set of PDSCH repetition numbers for CE mode A
 *  \Reference: 3GPP 36.331 , pdsch-maxNumRepetitionCEmodeA-r13 
 */
typedef enum
{
    EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R8,
    EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R16,
    EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R32,
    EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_MAX = 
                  EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R32
}EmtcPdschMaxNumRepetitionCeModeA_et;

/*! \enum EmtcMpdcchMaxNumRepetitionPaging_et
 *  \brief an enum that specifies maximum number of 
 *         repetitions for MPDCCH common search space (CSS) 
 *         for paging
 *  \Reference: 3GPP 36.331 , mpdcch-NumRepetition-Paging-r13
 */
typedef enum
{
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R1,
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R2,
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R4,
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R8,
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R16,
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R32,
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R64,
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R128,
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R256,
   EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_MAX = 
                EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_R256
}EmtcMpdcchMaxNumRepetitionPaging_et;

/*! \enum EmtcPreambleTransMaxCe_et 
 *  \brief An enum that specifies Max Preamble Transmission for Release 13
 *  \Reference: 3GPP 36.331 , PreambleTransMax
 */
typedef enum
{
    EMTC_PREAMBLE_TRANS_MAX_CE_N3,
    EMTC_PREAMBLE_TRANS_MAX_CE_N4,
    EMTC_PREAMBLE_TRANS_MAX_CE_N5,
    EMTC_PREAMBLE_TRANS_MAX_CE_N6,
    EMTC_PREAMBLE_TRANS_MAX_CE_N7,
    EMTC_PREAMBLE_TRANS_MAX_CE_N8,
    EMTC_PREAMBLE_TRANS_MAX_CE_N10,
    EMTC_PREAMBLE_TRANS_MAX_CE_N20,
    EMTC_PREAMBLE_TRANS_MAX_CE_N50,
    EMTC_PREAMBLE_TRANS_MAX_CE_N100,
    EMTC_PREAMBLE_TRANS_MAX_CE_N200,
    EMTC_PREAMBLE_TRANS_MAX_CE_MAX = 
                   EMTC_PREAMBLE_TRANS_MAX_CE_N200
}EmtcPreambleTransMaxCe_et;

/*! \enum EmtcRaResponseWindowSize_et 
 *  \brief an enum that specifies RRC RRM RA Response window size for Release13
 *  \Reference: 3GPP 36.331 , ra-ResponseWindowSize-r13
 */
typedef enum
{
    EMTC_RA_RESPONSE_WINDOW_SIZE_SF20,
    EMTC_RA_RESPONSE_WINDOW_SIZE_SF50,
    EMTC_RA_RESPONSE_WINDOW_SIZE_SF80,
    EMTC_RA_RESPONSE_WINDOW_SIZE_SF120,
    EMTC_RA_RESPONSE_WINDOW_SIZE_SF180,
    EMTC_RA_RESPONSE_WINDOW_SIZE_SF240,
    EMTC_RA_RESPONSE_WINDOW_SIZE_SF320,
    EMTC_RA_RESPONSE_WINDOW_SIZE_SF400,
    EMTC_RA_RESPONSE_WINDOW_SIZE_SF_MAX = 
                  EMTC_RA_RESPONSE_WINDOW_SIZE_SF400
}EmtcRaResponseWindowSize_et;

/*! \enum EmtcMacContentionResolutionTimer_et 
 *  \brief an enum that specifies RRC RRM Contention resolution timer for Release 13
 *  \Reference: 3GPP 36.331 , mac-ContentionResolutionTimer-r13
 */
typedef enum
{
    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_SF80,
    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_SF100,
    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_SF120,
    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_SF160,
    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_SF200,
    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_SF240,
    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_SF480,
    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_SF960,
    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_MAX = 
                    EMTC_MAC_CONTENTION_RESOLUTION_TIMER_SF960
}EmtcMacContentionResolutionTimer_et;

/*! \EmtcSiPeriodicity_et
 *  \brief an enum that specifies the si periodicity
 *         in radio frames
 *  \Reference: 3GPP 36.331, si-Periodicity
 */
typedef enum
{
    EMTC_SI_PERIODICITY_8,
    EMTC_SI_PERIODICITY_16,
    EMTC_SI_PERIODICITY_32,
    EMTC_SI_PERIODICITY_64,
    EMTC_SI_PERIODICITY_128,
    EMTC_SI_PERIODICITY_256,
    EMTC_SI_PERIODICITY_512,
    EMTC_SI_PERIODICITY_MAX = EMTC_SI_PERIODICITY_512
}EmtcSiPeriodicity_et;


/*! \enum EmtcNb_et
 *  \brief an enum that specifies the parameter to derive
 *         the Paging Frame and Paging Occasion
 *  \Reference: 3GPP 36.331 , nB-v1310 & nb
 */
typedef enum
{
    EMTC_NB_FOUR_T,
    EMTC_NB_TWO_T,
    EMTC_NB_ONE_T,
    EMTC_NB_HALF_T,
    EMTC_NB_QUARTER_T,
    EMTC_NB_ONE_EIGHTH_T,
    EMTC_NB_ONE_SIXTEENTH_T,
    EMTC_NB_ONE_THIRTY_SECOND_T,
    EMTC_NB_ONE_SIXTY_FOURTH_T,
    EMTC_NB_ONE_ONE_HUNDRED_TWENTY_EIGHTH_T,
    EMTC_NB_ONE_TWO_HUNDRED_FIFTY_SIXTH_T,
    EMTC_NB_MAX = EMTC_NB_ONE_TWO_HUNDRED_FIFTY_SIXTH_T
}EmtcNb_et;

/*! \enum EmtcSiTb_et 
 *  \brief an enum that indicates the transport block size 
 *         information used to
 *         broadcast the SI message towards UEs for Release 13
 *  \Reference: 3GPP 36.331 , si-TBS-r13
 */
typedef enum
{
    EMTC_SI_TB_B152,
    EMTC_SI_TB_B208,
    EMTC_SI_TB_B256,
    EMTC_SI_TB_B328,
    EMTC_SI_TB_B408,
    EMTC_SI_TB_B504,
    EMTC_SI_TB_B600,
    EMTC_SI_TB_B712,
    EMTC_SI_TB_B808,
    EMTC_SI_TB_B936,
    EMTC_SI_TB_MAX = EMTC_SI_TB_B936
}EmtcSiTb_et;

/*! \enum EmtcIntervalUlHoppingConfigCommonModeAFdd_et 
  *  \brief An enum that specifies the number of consecutive
  *         absolute subframes over which PUCCH or PUSCH for
  *         CE mode A stays at the same narrowband before
  *         hopping to another narrowband.
  *   \Reference: 3GPP 36.331, interval-FDD-r13
*/
typedef enum
{
    EMTC_INTERVAL_UL_HOPPING_MODE_A_FDD_INT1,
    EMTC_INTERVAL_UL_HOPPING_MODE_A_FDD_INT2,
    EMTC_INTERVAL_UL_HOPPING_MODE_A_FDD_INT4,
    EMTC_INTERVAL_UL_HOPPING_MODE_A_FDD_INT8,
    EMTC_INTERVAL_UL_HOPPING_MODE_A_MAX = 
                  EMTC_INTERVAL_UL_HOPPING_MODE_A_FDD_INT8
}EmtcIntervalUlHoppingConfigCommonModeAFdd_et;


/*! \enum EmtcPrachHoppingConfig_et 
 *  \brief An enum that specifies PRACH Hopping Config info 
 *         for Release 13
 *  \Reference: 3GPP 36.331 ,prach-HoppingConfig-r13
 */
typedef enum
{
	EMTC_PRACH_HOPPING_CONFIG_OFF,
	EMTC_PRACH_HOPPING_CONFIG_ON,
	EMTC_PRACH_HOPPING_CONFIG_MAX = 
              EMTC_PRACH_HOPPING_CONFIG_ON
}EmtcPrachHoppingConfig_et;

/*! \enum EmtcTransmissionType_et 
 *  \brief An enum that specifies EPDCCH transmission type
 *  \Reference: 3GPP 36.331 , transmissionType-r11
 */
typedef enum
{
    EMTC_TRANSMISSION_TYPE_LOCALISED,
    EMTC_TRANSMISSION_TYPE_DISTRIBUTED,
    EMTC_TRANSMISSION_TYPE_MAX = EMTC_TRANSMISSION_TYPE_DISTRIBUTED
}EmtcTransmissionType_et;

/*! \enum EmtcCsiNumRepetitionCe_et 
 *  \brief An enum that specifies no of subframes for CSI reference
 *         resource
 *  \Reference: 3GPP 36.331 , csi-NumRepetitionCE-r13
 */
typedef enum
{
    EMTC_CSI_NUM_REPETITION_SF1,
    EMTC_CSI_NUM_REPETITION_SF2,
    EMTC_CSI_NUM_REPETITION_SF4,
    EMTC_CSI_NUM_REPETITION_SF8,
    EMTC_CSI_NUM_REPETITION_SF16,
    EMTC_CSI_NUM_REPETITION_SF32,
    EMTC_CSI_NUM_REPETITION_MAX = EMTC_CSI_NUM_REPETITION_SF32
}EmtcCsiNumRepetitionCe_et;

/*! \enum EmtcMpdcchStartSfUessFdd_et 
 *  \brief An enum that specifies starting subframe configuration for 
 *        an MPDCCH UE specific search space of FDD
 *  \Reference: 3GPP 36.331 , mpdcch-StartSF-UESS-r13
 */
typedef enum
{
    EMTC_MPDCCH_START_SF_UESS_FDD_V1,
    EMTC_MPDCCH_START_SF_UESS_FDD_V1DOT5,
    EMTC_MPDCCH_START_SF_UESS_FDD_V2,
    EMTC_MPDCCH_START_SF_UESS_FDD_V2DOT5,
    EMTC_MPDCCH_START_SF_UESS_FDD_V4,
    EMTC_MPDCCH_START_SF_UESS_FDD_V5,
    EMTC_MPDCCH_START_SF_UESS_FDD_V8,
    EMTC_MPDCCH_START_SF_UESS_FDD_V10,
    EMTC_MPDCCH_START_SF_UESS_FDD_MAX = 
                    EMTC_MPDCCH_START_SF_UESS_FDD_V10
}EmtcMpdcchStartSfUessFdd_et;

/*! \enum EmtcUeType_et 
 *  \brief An enum that specifies UE type
 */
typedef enum
{
    EMTC_UE,
    LTE_UE,
    UE_MAX = LTE_UE
}EmtcUeType_et;

/*! \enum EmtcMpdcchNumRepetitionUess_et 
 *  \brief An enum that specifies maximum numbers of repetitions
 *        for UE search space for MPDCCH
 *  \Reference: 3GPP 36.331 , mpdcch-NumRepetition-r13 
 */
typedef enum
{
    EMTC_MPDCCH_NUM_REPETITION_UESS_R1,
    EMTC_MPDCCH_NUM_REPETITION_UESS_R2,
    EMTC_MPDCCH_NUM_REPETITION_UESS_R4,
    EMTC_MPDCCH_NUM_REPETITION_UESS_R8,
    EMTC_MPDCCH_NUM_REPETITION_UESS_R16,
    EMTC_MPDCCH_NUM_REPETITION_UESS_R32,
    EMTC_MPDCCH_NUM_REPETITION_UESS_R64,
    EMTC_MPDCCH_NUM_REPETITION_UESS_R128,
    EMTC_MPDCCH_NUM_REPETITION_UESS_R256,
    EMTC_MPDCCH_NUM_REPETITION_UESS_MAX = 
                  EMTC_MPDCCH_NUM_REPETITION_UESS_R256
}EmtcMpdcchNumRepetitionUess_et;

/*! \enum EmtcRepetitionFactor_et 
 *  \brief An enum that specifies repetition factor of PUCCH
 *         for Release 13
 *  \Reference: 3GPP 36.331 ,repetitionFactor-r13
 */
typedef enum
{
   EMTC_REPETITION_FACTOR_N2,
   EMTC_REPETITION_FACTOR_N4,
   EMTC_REPETITION_FACTOR_N6,
   EMTC_REPETITION_FACTOR_MAX = EMTC_REPETITION_FACTOR_N6
}EmtcRepetitionFactor_et;

/*! \enum EmtcMpdcchPdschHoppingConfig_et
 *  \brief An enum that specifies MPDCCH Hopping Config info 
 *         for Release 13
 *  \Reference: 3GPP 36.331 ,mpdcch-pdsch-HoppingConfig-r13
 */
typedef enum
{
	EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_OFF,
	EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_ON,
	EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX = 
                      EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_ON
}EmtcMpdcchPdschHoppingConfig_et;

/*! \enum EmtcNumRepetitionCeModeAFormat2_et 
 *  \brief An enum that specifies number of PUCCH repetitions 
 *         for Release 13 of format 2 Mode A
 *  \Reference: 3GPP 36.331 , pucch-NumRepetitionCE-format2-r13
 */
typedef enum
{
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_R1, 
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_R2, 
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_R4, 
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_R8, 
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_MAX = 
                     EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT2_R8 
}EmtcPucchNumRepetitionCeModeAFormat2_et;

/*! \enum EmtcNumPrbPairs_et 
 *  \brief An enum that specifies the number of physical RB pairs used
 *         for the EPDCCH set 
 *  \Reference: 3GPP 36.331 , numberPRB-Pairs-r11
 */
typedef enum
{
    EMTC_NUM_PRB_PAIR_N2,
    EMTC_NUM_PRB_PAIR_N4,
    EMTC_NUM_PRB_PAIR_N6,
    EMTC_NUM_PRB_PAIR_MAX = EMTC_NUM_PRB_PAIR_N6
}EmtcNumPrbPairs_et;

/*! \enum EmtcPucchNumRepetitionCeModeAFormat1_et 
 *  \brief An enum that specifies Number of PUCCH repetitions 
 *         for Release 13 of format 1 Mode A
 *  \Reference: 3GPP 36.331 , pucch-NumRepetitionCE-format1-r13
 */
typedef enum
{
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_R1, 
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_R2, 
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_R4, 
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_R8, 
   EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX = 
                  EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_R8 
}EmtcPucchNumRepetitionCeModeAFormat1_et;


/*! \enum EmtcRarHoppingConfig_et 
 *  \brief An enum that specifies ihpping enbale disable 
 *         for RAR
 *  Reference: 3GPP 36.331 , rar-HoppingConfig-r13 */
typedef enum 
{
    EMTC_RAR_HOPPPING_CONFIG_OFF,
    EMTC_RAR_HOPPPING_CONFIG_ON,
    EMTC_RAR_HOPPPING_CONFIG_MAX = 
        EMTC_RAR_HOPPPING_CONFIG_ON
}EmtcRarHoppingConfig_et;

/****************************************************************************
 * Prototypes
****************************************************************************/

/****************************************************************************
 * Function Name  : emtcValidatePucchNumRepetitionCeFormat1Range
 * Inputs         : pucchNumRepetitionCeFormat1
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for pucchNumRepetitionCeFormat1
 ****************************************************************************/
ComReturnType
emtcValidatePucchNumRepetitionCeFormat1Range
(
 UInt8 pucchNumRepetitionCeFormat1
);

/****************************************************************************
 * Function Name  : emtcValidateEmtcMcsRange 
 * Inputs         : mcs value
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for mcs
 ****************************************************************************/
ComReturnType
emtcValidateEmtcMcsRange
(
 UInt8 mcs 
);
/****************************************************************************
 * Function Name  : emtcValidateDuplexingModeRange 
 * Inputs         : duplexingMode
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for duplexingMode 
 ****************************************************************************/
ComReturnType
emtcValidateDuplexingModeRange
(
 UInt8  duplexingMode
);
/****************************************************************************
 * Function Name  : emtcValidateTransmissionTypeRange 
 * Inputs         : transmissionType
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for transmissionType.
 ****************************************************************************/
ComReturnType
emtcValidateTransmissionTypeRange
(
 UInt8  transmissionType
);

/****************************************************************************
 * Function Name  : emtcValidateMaxUeRange 
 * Inputs         : maxEmtcUe
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for maxEmtcUe.
 ****************************************************************************/
ComReturnType
emtcValidateMaxUeRange
(
 UInt8  maxEmtcUe
);

/****************************************************************************
 * Function Name  : emtcValidateModPeriodCoeffRange 
 * Inputs         : modificationPeriodCoeff
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for modificationPeriodCoeff.
 ****************************************************************************/
ComReturnType
emtcValidateModPeriodCoeffRange
(
 UInt8 modificationPeriodCoeff 
);

/****************************************************************************
 * Function Name  : emtcValidateNbRange 
 * Inputs         : Nb
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for Nb.
 ****************************************************************************/
ComReturnType
emtcValidateNbRange
(
  UInt8   nB
);

/****************************************************************************
 * Function Name  : emtcValidateSib1BrRepetitionRange 
 * Inputs         : sib1BrRepetition 
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for sib1BrRepetition.
 ****************************************************************************/
ComReturnType
emtcValidateSib1BrRepetitionRange
(
 UInt8  sib1BrRepetition
);

/****************************************************************************
 * Function Name  : emtcValidateQRxLevMinCeRange 
 * Inputs         : qRxLevMinCe
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for qRxLevMinCe.
 ****************************************************************************/
ComReturnType
emtcValidateQRxLevMinCeRange
(
 SInt8 qRxLevMinCe 
);

/****************************************************************************
 * Function Name  : emtcValidateQqualMinRsrqCeRange 
 * Inputs         : qQualMinRsrqCe
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for qQualMinRsrqCe.
 ****************************************************************************/
ComReturnType
emtcValidateQqualMinRsrqCeRange
(
 SInt8 qQualMinRsrqCe 
);

/****************************************************************************
 * Function Name  : emtcValidateSiWindowLenRange 
 * Inputs         : siWindowLen
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for siWindowLen.
 ****************************************************************************/
ComReturnType
emtcValidateSiWindowLenRange
(
 UInt8  siWindowLen
);

/****************************************************************************
 * Function Name  : emtcValidateSiRepPatternRange 
 * Inputs         : siRepPattern
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for siRepPattern.
 ****************************************************************************/
ComReturnType
emtcValidateSiRepPatternRange
(
 UInt8 siRepPattern 
);
/****************************************************************************
 * Function Name  : emtcValidateSiCountRange 
 * Inputs         : siCount
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for siCount.
 ****************************************************************************/
ComReturnType
emtcValidateSiCountRange
(
 UInt8  siCount 
);

/****************************************************************************
 * Function Name  : emtcValidateSiPeriodicityRange 
 * Inputs         : siPeriodicity
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for siPeriodicity.
 ****************************************************************************/
ComReturnType
emtcValidateSiPeriodicityRange
(
 UInt8  siPeriodicity
);

/****************************************************************************
 * Function Name  : emtcValidateSibTypeRange 
 * Inputs         : sibType
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for sibType.
 ****************************************************************************/
ComReturnType
emtcValidateSibTypeRange
(
 UInt8  sibType
);

/****************************************************************************
 * Function Name  : emtcValidateNumSibTypeRange 
 * Inputs         : numSibType
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for numSibType.
 ****************************************************************************/
ComReturnType
emtcValidateNumSibTypeRange
(
 UInt8 numSibType 
);

/****************************************************************************
 * Function Name  : emtcValidateStartSymbolRange
 * Inputs         : startSymbol
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for startSymbol.
 ****************************************************************************/
ComReturnType
emtcValidateStartSymbolRange
(
 UInt8 startSymbol 
);

/****************************************************************************
 * Function Name  : emtcValidateSiHoppingConfigRange
 * Inputs         : siHoppingConfig
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for siHoppingConfig.
 ****************************************************************************/
ComReturnType
emtcValidateSiHoppingConfigRange
(
 UInt8 siHoppingConfig
);

/****************************************************************************
 * Function Name  : emtcValidateSiValidityTimerRange
 * Inputs         : siValidityTimer
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for siValidityTimer.
 ****************************************************************************/
ComReturnType
emtcValidateSiValidityTimerRange
(
 UInt8 siValidityTimer 
);

/****************************************************************************
 * Function Name  : emtcValidateMpdcchStartSfUessRange
 * Inputs         : mpdcchStartSfUess
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for mpdcchStartSfUess.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchStartSfUessRange
(
 UInt8  mpdcchStartSfUess
);

/****************************************************************************
 * Function Name  : emtcValidateMpdcchNumRepUessRange
 * Inputs         : mpdcchNumRepUess
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for mpdcchNumRepUess.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchNumRepUessRange
(
 UInt8  mpdcchNumRepUess
);

/****************************************************************************
 * Function Name  : emtcValidateMpdcchPdschHoppingRange 
 * Inputs         : mpdcchPdschHopping
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for mpdcchPdschHopping.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchPdschHoppingRange
(
 UInt8 mpdcchPdschHopping
);

/****************************************************************************
 * Function Name  : emtcValidateNumCeLevelRange
 * Inputs         : numCeLevel
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for numCeLevel.
 ****************************************************************************/
ComReturnType
emtcValidateNumCeLevelRange
(
 UInt8  numCeLevel
);

/****************************************************************************
 * Function Name  : emtcValidatePreambleTransMaxRange 
 * Inputs         : preambleTransMax
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for preambleTransMax.
 ****************************************************************************/
ComReturnType
emtcValidatePreambleTransMaxRange
(
 UInt8 preambleTransMax
);

/****************************************************************************
 * Function Name  : emtcValidatePreambleRange 
 * Inputs         : emtcPreamble
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for emtcPreamble.
 ****************************************************************************/
ComReturnType
emtcValidatePreambleRange
(
 UInt8  emtcPreamble
);

/****************************************************************************
 * Function Name  : emtcValidateRaResponseWindowSizeRange 
 * Inputs         : raResponseWindowSize
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for raResponseWindowSize.
 ****************************************************************************/
ComReturnType
emtcValidateRaResponseWindowSizeRange
(
 UInt8  raResponseWindowSize
);

/****************************************************************************
 * Function Name  : emtcValidateMacContentionResTimerRange
 * Inputs         : macContentionResTimer
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for macContentionResTimer.
 ****************************************************************************/
ComReturnType
emtcValidateMacContentionResTimerRange
(
 UInt8 macContentionResTimer 
);

/****************************************************************************
 * Function Name  : emtcValidateRarHoppingConfigRange 
 * Inputs         : rarHoppingConfig
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for rarHoppingConfig.
 ****************************************************************************/
ComReturnType
emtcValidateRarHoppingConfigRange
(
 UInt8  rarHoppingConfig
);

/****************************************************************************
 * Function Name  : emtcValidateN1PucchAnRepRange
 * Inputs         : n1PucchAnRep
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for n1PucchAnRep.
 ****************************************************************************/
ComReturnType
emtcValidateN1PucchAnRepRange
(
 UInt16  n1PucchAnRep
);

/****************************************************************************
 * Function Name  : emtcValidatePdschMaxNumRepModeARange
 * Inputs         : pdschMaxNumRepModeA
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for pdschMaxNumRepModeA.
 ****************************************************************************/
ComReturnType
emtcValidatePdschMaxNumRepModeARange
(
 UInt8  pdschMaxNumRepModeA
);

/****************************************************************************
 * Function Name  : emtcValidateRsrpValRange
 * Inputs         : rsrpVal
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for rsrpVal.
 ****************************************************************************/
ComReturnType
emtcValidateRsrpValRange
(
 UInt8  rsrpVal
);

/****************************************************************************
 * Function Name  : emtcValidateMpdcchStartSfCssRange 
 * Inputs         : mpdcchStartSfCss
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for mpdcchStartSfCss.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchStartSfCssRange
(
 UInt8 mpdcchStartSfCss
);

/****************************************************************************
 * Function Name  : emtcValidatePrachConfigIdxRange
 * Inputs         : prachConfigIdx
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for prachConfigIdx.
 ****************************************************************************/
ComReturnType
emtcValidatePrachConfigIdxRange
(
 UInt8  prachConfigIdx
);

/****************************************************************************
 * Function Name  : emtcValidatePrachFreqOffsetRange 
 * Inputs         : prachFreqOffset
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for prachFreqOffset.
 ****************************************************************************/
ComReturnType
emtcValidatePrachFreqOffsetRange
(
 UInt8  prachFreqOffset
);

/****************************************************************************
 * Function Name  : emtcValidatePrachStartingSfRange
 * Inputs         : prachStartingSf
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for prachStartingSf.
 ****************************************************************************/
ComReturnType
emtcValidatePrachStartingSfRange
(
 UInt8  prachStartingSf
 );

/****************************************************************************
 * Function Name  : emtcValidateMaxNumPreambleAttemptRange
 * Inputs         : maxNumPreambleAttempt
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for maxNumPreambleAttempt.
 ****************************************************************************/
ComReturnType
emtcValidateMaxNumPreambleAttemptRange
(
 UInt8  maxNumPreambleAttempt
);

/****************************************************************************
 * Function Name  : emtcValidateNumRepPerPreambleAttemptRange 
 * Inputs         : numRepPerPreambleAttempt
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for numRepPerPreambleAttempt.
 ****************************************************************************/
ComReturnType
emtcValidateNumRepPerPreambleAttemptRange
(
 UInt8  numRepPerPreambleAttempt
);

/****************************************************************************
 * Function Name  : emtcValidateMpdcchNumRepCssRange
 * Inputs         : mpdcchNumRepCss
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for mpdcchNumRepCss.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchNumRepCssRange
(
 UInt8  mpdcchNumRepCss
);

/****************************************************************************
 * Function Name  : emtcValidatePrachHoppingConfigRange 
 * Inputs         : prachHoppingConfig 
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for .
 ****************************************************************************/
ComReturnType
emtcValidatePrachHoppingConfigRange
(
 UInt8  prachHoppingConfig
);

/****************************************************************************
 * Function Name  : emtcValidateNarrowbandIdxRange
 * Inputs         : narrowbandIdx
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for .
 ****************************************************************************/
ComReturnType
emtcValidateNarrowbandIdxRange
(
 UInt8  narrowbandIdx
);

/****************************************************************************
 * Function Name  : emtcValidatePuschMaxNumRepModeARange
 * Inputs         : puschMaxNumRepModeA
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for puschMaxNumRepModeA.
 ****************************************************************************/
ComReturnType
emtcValidatePuschMaxNumRepModeARange
(
 UInt8  puschMaxNumRepModeA
);

/****************************************************************************
 * Function Name  : emtcValidateMpdcchMaxNumRepPagingRange
 * Inputs         : mpdcchNumRepPaging
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for mpdcchNumRepPaging .
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchMaxNumRepPagingRange
(
 UInt8  mpdcchNumRepPaging
);
/****************************************************************************
 * Function Name  : emtcValidateMpdcchNumRepPagingRange
 * Inputs         : mpdcchNumRepPaging
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for mpdcchNumRepPaging .
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchNumRepPagingRange
(
 UInt8  mpdcchNumRepPaging
);
/****************************************************************************
 * Function Name  : emtcValidatePdschPagingRepRange 
 * Inputs         : pdschPagingRep
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for pdschPagingRep.
 ****************************************************************************/
ComReturnType
emtcValidatePdschPagingRepRange
(
 UInt8 pdschPagingRep
);
/****************************************************************************
 * Function Name  : emtcValidatePucchNumRepCeMsg4Range 
 * Inputs         : pucchNumRepCeMsg4
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the range for pucchNumRepCeMsg4
 ****************************************************************************/
ComReturnType
emtcValidatePucchNumRepCeMsg4Range
(
 UInt8  pucchNumRepCeMsg4
);

/****************************************************************************
 * Function Name  : emtcValidateSiWindowSiPeriodicityConfig 
 * Inputs         : siPeriodicity : Periodicity of an SI
 *                  siWindow      : Window lenghth for SI
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Converts the input enumerated values to actual values and
 *                  validates that configuration of Window lenghth for SI and 
 *                  Periodicity of an SI is correct.
 *                  - Check that configuration of SI window length and  
 *                    SI periodicity is such that SI is not periodically 
 *                    repeated within a window.
 ****************************************************************************/
ComReturnType 
emtcValidateSiWindowSiPeriodicityConfig
(
    UInt8 siWindow, 
    UInt8 siPeriodicity
);
/****************************************************************************
 * Function Name  : emtcValidateCellOperatingMode
 * Inputs         : bandwidth         : Cell bandwidth
 *                  cellOpMode        : Operation Mode
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates that Mode of operation is correct as per 
 *                  the cell bandwith.
 *                  - If cell bandwidth is 1.4MHz then Cell Operating 
 *                    mode should not be Mixed
 ****************************************************************************/
ComReturnType 
emtcValidateCellOperatingMode 
(
    UInt8 bandwidth,
    UInt8 cellOpMode
);

/****************************************************************************
 * Function Name  : emtcValidateCfi
 * Inputs         : cellOperatingMode : Mode of operation LTE/EMTC/MIXED
 *                  cfiVal            : CFI value
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates that CFI value is correct as per 
 *                  the Mode of operation.
 ****************************************************************************/
ComReturnType 
emtcValidateCfi
(
    UInt8 cfiVal,
    UInt8 cellOperatingMode
);

/****************************************************************************
 * Function Name  : emtcValidateSib1BrRepetition
 * Inputs         : Sib1BrRepNum  : SIB1 BR repetition number to be validated
 *                  bandwidth     : Cell bandwidth
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates that SIB1 BR repetition number correct as per 
 *                  the cell bandwith.
 *                  - Check if cell bandwidth BW is < 3 MHZ (DL_NRB <=15) then 
 *                      number of SIB1 BR repetition should be 4
 ****************************************************************************/
ComReturnType 
emtcValidateSib1BrRepetition 
(
    UInt8 sib1BrRepNum, 
    UInt8 bandwidth
);

/****************************************************************************
 * Function Name  : emtcValidateMpdcchStartSfCssRaMpdcchNumRepRaConfig
 * Inputs         : mpdcchStartSfCssRa : MPDCCH start sf CSS to be validated
 *                  mpdcchNumRepRa     : MPDCCH repettition number
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Converts the input enumerated values to actual values and 
 *                  then validates the configuration of mpdcchStartSfCssRa and
 *                  mpdcchNumRepRa as per spec 36.213
 *                  - Check that (mpdcchStartSfCssRa * mpdcchNumRepRa)
 *                    should be an integer value.
 ****************************************************************************/
ComReturnType 
emtcValidateMpdcchStartSfCssRaMpdcchNumRepRaConfig
(
    UInt8 mpdcchStartSfCssRa, 
    UInt8 mpdcchNumRepRa
);

/****************************************************************************
 * Function Name  : emtcValidatePrachFreqOffset
 * Inputs         : prachFreqOffset : Duration of the RA response window
 *                  bandwidth       : Cell bandwidth
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates that prachFreqOffset is in multiple if six.
 ****************************************************************************/
ComReturnType 
emtcValidatePrachFreqOffset
(
    UInt8 prachFreqOffset, 
    UInt8 bandwidth 
);

/****************************************************************************
 * Function Name  : emtcValidateNumRepetitionPerPreambleAttempt
 * Inputs         : numRepPerPreambleAttempt : Number of PRACH repetitions 
 *                                             per attempt per CE level
 *                  prachStartingSubframe    : PRACH starting subframe 
 *                                             periodicity.
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    :  Converts the input enumerated values to actual values and
 *                   validates that numRepPerPreambleAttempt and 
 *                   prachStartingSubframe configuration is correct as per 
 *                   3GPP 36.331.
 *                   - Check that value of numRepetitionPerPreambleAttempt
 *                     should be lesser than or equal to the value of
 *                     prachStartingSubframe.
 ****************************************************************************/
ComReturnType emtcValidateNumRepetitionPerPreambleAttempt 
(
    UInt8 numRepPerPreambleAttempt, 
    UInt8 prachStartingSubframe
);

/****************************************************************************
 * Function Name  : emtcValidateNbDefaultPagingCycleConfig 
 * Inputs         : defaultPagingCycle : Default paging cycle for SIB2
 *                  nbVal              : NB V1310 value for SIB2 
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    :  Converts the input enumerated values to actual values and
 *                   validates that defaultPagingCycle value is correct as per 
 *                   the configured nb value.
 *                   - Check that modulo of default paging cycle and nb is zero
 ****************************************************************************/
ComReturnType 
emtcValidateNbDefaultPagingCycleConfig
(
    UInt8 defaultPagingCycle, 
    UInt8 nb
);

/****************************************************************************
 * Function Name  : emtcValidateSiWindowSiPeriodicityConfig 
 * Inputs         : siPeriodicity : Periodicity of an SI
 *                  siWindow      : Window lenghth for SI
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Converts the input enumerated values to actual values and
 *                  validates that configuration of Window lenghth for SI and 
 *                  Periodicity of an SI is correct.
 *                  - Check that configuration of SI window length and  
 *                    SI periodicity is such that SI is not periodically 
 *                    repeated within a window.
 ****************************************************************************/
ComReturnType 
emtcValidateSiWindowSiPeriodicityConfig
(
    UInt8 siWindow, 
    UInt8 siPeriodicity
);

/****************************************************************************
 * Function Name  : emtcValidateSiWindowSiRepetitionConfig 
 * Inputs         : siRepetitionPattern : Repetition pattern for SI
 *                  siWindow            : Window lenghth for SI
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Converts the input enumerated values to actual values and
 *                  validates that configuration of Window lenghth for SI and 
 *                  Repetition pattern of an SI is correct.
 *                  - Check that configuration of SI window length and 
 *                    SI repetition such that SI window is sufficient enough to
 *                    accommodate at least one SI repetition.
 ****************************************************************************/
ComReturnType 
emtcValidateSiWindowSiRepetitionConfig
(
    UInt8 siWindow,
    UInt8 siRepetitionPattern
);

/****************************************************************************
 * Function Name  : emtcValidateDefPagingCycleMpdcchRepPagingConfig
 * Inputs         : defaultPagingCycle : Default paging cycle for SIB2
 *                  mpdcchRepPaging    : Maximum number of repetitions for 
 *                                       MPDCCH CSS for paging.
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    :  Converts the input enumerated values to actual values and
 *                   validates that defaultPagingCycle and mpdcchRepPaging  
 *                   configuration is correct.
 *                   - Check that modulo of default paging cycle and 
 *                     mpdcch-NumRepetition-Paging-r13 is zero.
 ****************************************************************************/
ComReturnType emtcValidateDefPagingCycleMpdcchRepPagingConfig
(
    UInt8 defaultPagingCycle, 
    UInt8 mpdcchRepPaging
);

/****************************************************************************
 * Function Name  : emtcValidateRsrpThresholdsPrachInfoList
 * Inputs         : pRrsrpThresholdsPrachInfoList : RSRP thresholds for BL UEs
 *                                                  and UEs in CE to select 
 *                                                  PRACH resource set.
 *                  numRsrpThresholdsCount        : Number of thresholds
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates the rsrp entries in the list should be 
 *                  in increasing order.
 ****************************************************************************/
ComReturnType emtcValidateRsrpThresholdsPrachInfoList 
(
    UInt8 *pRsrpThresholdsPrachInfoList, 
    UInt8  numRsrpThresholds
);

/****************************************************************************
 * Function Name  : emtcValidatefddDlSubframeBitmap
 * Inputs         : pFddDlSubframeBitmap : List of fdd ul/dl SFs
 *                  bitmapCount : Number of indexes to scan for bitmap
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates that pFddDlSubframeBitmap is non-zero.
 ****************************************************************************/
ComReturnType emtcValidatefddDlSubframeBitmap
(
    UInt8         *pFddDlSubframeBitmap,
    UInt8         bitmapCount
);

/****************************************************************************
 * Function Name  : emtcValidatefddUlSubframeBitmap
 * Inputs         : pFddUlSubframeBitmap : List of fdd ul SFs
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates that pFddUlSubframeBitmap is non-zero.
 ****************************************************************************/
ComReturnType emtcValidatefddUlSubframeBitmap
(
    UInt16         *pFddUlSubframeBitmap
);

/****************************************************************************
 * Function Name  : emtcValidateNarrowbandIdx
 * Inputs         : narrowbandNum : Narrowband Idx to be validated
 *                  bandwidth     : Cell bandwidth
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates that Narrowband Idx is correct as per the cell
 *                  bandwith.
 *                 - Check that each narrowband configured should be <=
 *                      (DL_NRB / 6)
 *                 - Check that each narrowband configured should be different
 *                      from the ones used for MIB.
 ****************************************************************************/
ComReturnType 
emtcValidateNarrowbandIdx
(
    UInt8 narrowbandNum, 
    UInt8 bandwidth
);

/****************************************************************************
 * Function Name  : emtcValidateRaResponseWindowSize
 * Inputs         : raResponseWindowSize : Duration of the RA response window
 *                  mpdcchRepetition     : number of repetitions for MPDCCH 
 *                                         common search space (CSS)
 *                  pdschRepetition      : Maximum value to indicate the set 
 *                                         of PDSCH repetition numbers 
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Converts the input enumerated values to actual values and
 *                  Validates the raResponseWindowSize is big enough to 
 *                  accomodate mpdcchRepetitions and pdschRepetitions.
 ****************************************************************************/
ComReturnType 
emtcValidateRaResponseWindowSize
(
    UInt8 raResponseWindowSize, 
    UInt8 mpdcchRepetition,
    UInt8 pdschRepetition
);

/****************************************************************************
 * Function Name  : emtcValidatePreambleInfo
 * Inputs         : cellOpMode: Cell Operating mode
 *                  *pPreambleInfo : List of preamble info for each 
 *                                   ce level              
 *                  preambleInfoCount : Number of PreambleInfo recieved
 *                  numRaPreamblesLte : Number of Preambles used for LTE
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Validates that pPreambleInfoe list should be non-overlapping
 *                  across the ce-levels and value configured for LTE also.
 ****************************************************************************/
ComReturnType emtcValidatePreambleInfo 
(
    UInt8             cellOpMode,
    EmtcPreambleInfo_t *pEmtcPreambleInfo, 
    UInt8             preambleInfoCount,
    UInt8             numRaPreamblesLte
);

/****************************************************************************
 * Function Name  : emtcValidateMpdcchStartSfUessMpdcchNumRepConfig
 * Inputs         : mpdcchStartSfUess : MPDCCH start sf UESS to be validated
 *                  mpdcchNumRep      : MPDCCH repettition number
 * Outputs        : None
 * Returns        : COM_ONE: If configuration is correct
 *                  COM_ZERO: If configuration is incorrect
 * Description    : Converts the input enumerated values to actual values and 
 *                  then validates the configuration of mpdcchStartSfCssRa and
 *                  mpdcchNumRepRa as per spec 36.213
 *                  - Check that (mpdcchStartSfUess * mpdcchNumRep)
 *                    should be an integer value.
 ****************************************************************************/
ComReturnType 
emtcValidateMpdcchStartSfUessMpdcchNumRepConfig
(
    UInt8 mpdcchStartSfUess, 
    UInt8 mpdcchNumRep
);

/******************************************************************************
*   FUNCTION NAME: emtcValidateSiScheduling
*   INPUT        : pSchedulingInf 
*                  siWindowLen 
*   OUTPUT       : none  
*   DESCRIPTION  : This function fully checks SIs scheduling
*                  Precondition: it should be already checked:
*                  si_WindowLength range,
*                  number of SIs, si_Periodicity range for each SI,
*                  number of SIBs in SI,SIBs are mapped only once to SIs.
*
*   RETURNS      : COM_ZERO : if SIs scheduling is not OK
*                  COM_ONE  : if SIs scheduling is OK
*
******************************************************************************/
ComReturnType 
emtcValidateSiScheduling
(
  EmtcSchedulingInfoList_t *pSchedulingInf,
  UInt8                     siWindowLen
);
/* SPR 22512 Fix Start */
/****************************************************************************
 * Function Name  : emtcValidateStartSymbol 
 * Inputs         : startSymbol
 *                  bandwidth         : Cell bandwidth
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates that startSymbol value is coorect as per system BW.
 *                   Refer Spec 3GPP 36.331 startSymbolBR-r13.
 ****************************************************************************/
ComReturnType 
emtcValidateStartSymbol
(
    UInt8 startSymbol, 
    UInt8 bandwidth 
);

void getPrbIndexToNbInfo(EmtcCellBw  bw,
                         UInt8       prbIndex,
                         UInt8*      derivedNbIndex,
                         UInt8*      derivedPrbIndex);
/* SPR 22512 Fix End */
#endif  /* EMTC_COMMON_VALIDATION_H */
