/****************************************************************************
 *
 *  File Description : This file contains the common validation functionality
 *                     for eMTC configuration.
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *$Log: emtcCommonValidation.c,v $
 *Revision 0.1 06/07/2017 gur26515
 *Validation functionality for eMTC Configuration stats related changes
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/
#ifdef OAM_IPR
enum
{
    QVLOG_WARNING = 7
};
#endif
/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteMisc.h"
#include "lteTypes.h"
#include "emtcCommonValidation.h"
#include "lteLog.h"
/****************************************************************************
 Private Definitions
 ****************************************************************************/
/*  3GPP 36.213, section 9.1.5
**  Table 9.1.5-3: Determination of repetition levels
*/
UInt16 mpdcchRepUeLevel[EMTC_MPDCCH_NUM_REP_UESS_NUM_VALUES][EMTC_MAX_CE_LEVEL] =
{
    {1,0},
    {1,2},
    {1,2},
    {1,2},
    {2,4},
    {4,8},
    {8,16},
    {16,32},
    {32,64}
};

/*  3GPP 36.213, section 7.1.11,
** Table 7.1.11-1: PDSCH repetition levels (DCI Format 6-1A) 
*/
UInt8 pdschRepUeLevel[EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_NUM_VALUES][EMTC_MAX_CE_LEVEL] =
{
    {1,2},
    {1,4},
    {1,4}
};

/*
** Mapiing of NumPrbPairs enum to actual values
*/
EmtcNumPrbPairVal_et 
numRbPairMpdcch[EMTC_NUM_PRB_PAIR_VAL_NUM_VALUES]=
{
    EMTC_NUM_PRB_PAIR_VAL_N2,
    EMTC_NUM_PRB_PAIR_VAL_N4,
    EMTC_NUM_PRB_PAIR_VAL_N6
};

/*
* Mapping of bandwith and  number of RBs
*/
EmtcBwRb
EmtcRbCount[EMTC_BW_NUM_VALUES] = 
{
  NRB_6,
  NRB_15,
  NRB_25,
  NRB_50,
  NRB_75,
  NRB_100
};


/*
* This map contains the PRB index to narrow band and prb index mapping

*
*/
EmtcPrbIndexToNbMap_t emtcPrbIndexToNbMap [EMTC_BW_NUM_VALUES] [100] =
{   /// Nb Index, Prb Index Pair
    /// Bandwidth NRB 6
    { {0, 0}, {0, 1}, {0, 2}, {0,3}, {0,4}, {0,5}, {255, 0}, {255, 0}, 
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}
    },

    /// Bandwidth NRB 15
    { {255, 0}, {0, 0}, {0, 1}, {0,2}, {0,3}, {0,4}, {0, 5}, {255, 0}, 
      {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}
    },

    /// Bandwidth NRB 25
    { {0, 0}, {0, 1}, {0, 2}, {0,3}, {0,4}, {0,5}, {1, 0}, {1, 1}, 
      {1, 2}, {1, 3}, {1, 4}, {1, 5}, {255, 0}, {2, 0}, {2, 1}, {2, 2},
      {2, 3}, {2, 4}, {2, 5}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4},
      {3, 5}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}
    },

    /// Bandwidth NRB 50
    { {255, 0}, {0, 0}, {0, 1}, {0,2}, {0,3}, {0,4}, {0, 5}, {1, 0}, 
      {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 0}, {2, 1}, {2, 2},
      {2, 3}, {2, 4}, {2, 5}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4},
      {3, 5}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {5, 0},
      {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {6, 0}, {6, 1}, {6, 2},
      {6, 3}, {6, 4}, {6, 5}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4},
      {7, 5}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}
    },

    /// Bandwidth NRB 75
    { {255, 0}, {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, {1, 0}, 
      {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, {2, 0}, {2, 1}, {2, 2},
      {2, 3}, {2, 4}, {2, 5}, {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4},
      {3, 5}, {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, {5, 0},
      {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, {255, 0}, {6, 0}, {6, 1}, 
      {6, 2}, {6, 3}, {6, 4}, {6, 5}, {7, 0}, {7, 1}, {7, 2}, {7, 3}, 
      {7, 4}, {7, 5}, {8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5},
      {9, 0}, {9, 1}, {9, 2}, {9, 3}, {9, 4}, {9, 5}, {10, 0}, {10, 1}, 
      {10, 2}, {10, 3}, {10, 4}, {10, 5}, {11, 0}, {11, 1}, {11, 2}, {11, 3},
      {11, 4}, {11, 5}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, 
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, 
      {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0}, {255, 0},
      {255, 0}, {255, 0}, {255, 0}, {255, 0}
    },

    /// Bandwidth NRB 100
    { {255, 0}, {255, 0}, 
      {0, 0}, {0, 1}, {0, 2}, {0, 3}, {0, 4}, {0, 5}, 
      {1, 0}, {1, 1}, {1, 2}, {1, 3}, {1, 4}, {1, 5}, 
      {2, 0}, {2, 1}, {2, 2}, {2, 3}, {2, 4}, {2, 5},
      {3, 0}, {3, 1}, {3, 2}, {3, 3}, {3, 4}, {3, 5}, 
      {4, 0}, {4, 1}, {4, 2}, {4, 3}, {4, 4}, {4, 5}, 
      {5, 0}, {5, 1}, {5, 2}, {5, 3}, {5, 4}, {5, 5}, 
      {6, 0}, {6, 1}, {6, 2}, {6, 3}, {6, 4}, {6, 5}, 
      {7, 0}, {7, 1}, {7, 2}, {7, 3}, {7, 4}, {7, 5}, 
      {8, 0}, {8, 1}, {8, 2}, {8, 3}, {8, 4}, {8, 5},
      {9, 0}, {9, 1}, {9, 2}, {9, 3}, {9, 4}, {9, 5},
      {10, 0}, {10, 1},{10, 2}, {10, 3}, {10, 4}, {10, 5},
      {11, 0}, {11, 1},{11, 2}, {11, 3},{11, 4}, {11, 5},
      {12, 0}, {12, 1},{12, 2}, {12, 3},{12, 4}, {12, 5},
      {13, 0}, {13, 1},{13, 2}, {13, 3},{13, 4}, {13, 5},
      {14, 0}, {14, 1},{14, 2}, {14, 3},{14, 4}, {14, 5},
      {15, 0}, {15, 1}, {15, 2}, {15, 3},{15, 4}, {15, 5}, 
      {255, 0}, {255, 0}
    }
};


void getPrbIndexToNbInfo(EmtcCellBw  bw,
                         UInt8       prbIndex, 
                         UInt8*      derivedNbIndex,
                         UInt8*      derivedPrbIndex)
{
   *derivedNbIndex  = emtcPrbIndexToNbMap[bw][prbIndex].nbIndex;    
   *derivedPrbIndex = emtcPrbIndexToNbMap[bw][prbIndex].prbIndex;    
}

/*
** Mapping of Sib1BrRepetition enumerated value 
** and actual value 
*/
EmtcSib1BrRep
EmtcSib1BrRepetitionVal[EMTC_SIB1_BR_REP_NUM_VALUES] = 
{
 EMTC_SIB1_BR_REP_VAL_N4,
 EMTC_SIB1_BR_REP_VAL_N8,
 EMTC_SIB1_BR_REP_VAL_N16
};

/*
** Mapping of Default Paging Cycle enumerated value 
** and actual value 
*/
EmtcDefaultPagingCycle
EmtcDefPagingCycleVal[EMTC_DEFAULT_PAGING_CYCLE_NUM_VALUES] =
{
  EMTC_DEFAULT_PAGING_CYCLE_RF32,
  EMTC_DEFAULT_PAGING_CYCLE_RF64,
  EMTC_DEFAULT_PAGING_CYCLE_RF128,
  EMTC_DEFAULT_PAGING_CYCLE_RF256
};

/*
** Mapping of MpdcchRepPaging enumerated value 
** and actual value 
*/
EmtcMpdcchNumRepPaging
EmtcMpdcchRepPagingVal[EMTC_MPDCCH_NUM_REP_PAGING_NUM_VALUES] = 
{
   EMTC_MPDCCH_NUM_REP_PAGING_R1_VAL,
   EMTC_MPDCCH_NUM_REP_PAGING_R2_VAL,
   EMTC_MPDCCH_NUM_REP_PAGING_R4_VAL,
   EMTC_MPDCCH_NUM_REP_PAGING_R8_VAL,
   EMTC_MPDCCH_NUM_REP_PAGING_R16_VAL,
   EMTC_MPDCCH_NUM_REP_PAGING_R32_VAL,
   EMTC_MPDCCH_NUM_REP_PAGING_R64_VAL,
   EMTC_MPDCCH_NUM_REP_PAGING_R128_VAL,
   EMTC_MPDCCH_NUM_REP_PAGING_R256_VAL
};

/*
** Mapping of SiPeriodicity enumerated value 
** and actual value 
*/
EmtcSiPeriodicityVal
EmtcSiPeriodicityRfVal[EMTC_SI_PERIODICITY_NUM_VALUES] =
{
    EMTC_SI_PERIODICITY_VAL_8RF,
    EMTC_SI_PERIODICITY_VAL_16RF, 
    EMTC_SI_PERIODICITY_VAL_32RF,
    EMTC_SI_PERIODICITY_VAL_64RF,
    EMTC_SI_PERIODICITY_VAL_128RF,
    EMTC_SI_PERIODICITY_VAL_256RF,
    EMTC_SI_PERIODICITY_VAL_512RF
};


/*
** Mapping of SiRepetitionPattern enumerated value 
** and actual value 
*/
EmtcSiRepVal
EmtcSiRepetitionPatternVal[EMTC_SI_REP_NUM_VALUES] = 
{
    EMTC_SI_REP_VAL_EVERY_RF, 
    EMTC_SI_REP_VAL_EVERY_SECOND_RF,
    EMTC_SI_REP_VAL_EVERY_FOURTH_RF,
    EMTC_SI_REP_VAL_EVERY_EIGHTH_RF
};


/*
** Mapping of PdschMaxNumRepCeModeA enumerated value 
** and actual value 
*/
EmtcPdschMaxNumRepCeModeA
EmtcPdschMaxNumRepCeModeAVal[EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_NUM_VALUES] = 
{
  EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R8_VAL,
  EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R16_VAL,
  EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_R32_VAL
};

/*
** Mapping of MpdcchNumRepetitionUess enumerated value 
** and actual value 
*/
EmtcMpdcchNumRepUess
EmtcMpdcchNumRepetitionUessVal[EMTC_MPDCCH_NUM_REP_UESS_NUM_VALUES] = 
{
 EMTC_MPDCCH_NUM_REP_UESS_R1_VAL,
 EMTC_MPDCCH_NUM_REP_UESS_R2_VAL,
 EMTC_MPDCCH_NUM_REP_UESS_R4_VAL,
 EMTC_MPDCCH_NUM_REP_UESS_R8_VAL,
 EMTC_MPDCCH_NUM_REP_UESS_R16_VAL,
 EMTC_MPDCCH_NUM_REP_UESS_R32_VAL,
 EMTC_MPDCCH_NUM_REP_UESS_R64_VAL,
 EMTC_MPDCCH_NUM_REP_UESS_R128_VAL,
 EMTC_MPDCCH_NUM_REP_UESS_R256_VAL 
};

/*
** Mapping of MpdcchNumRepetitionRa enumerated value 
** and actual value 
*/
EmtcMpdcchNumRepRa
EmtcMpdcchNumRepetitionRaVal[EMTC_MPDCCH_NUM_REP_RA_NUM_VALUES] = 
{
 EMTC_MPDCCH_NUM_REP_RA_R1_VAL,
 EMTC_MPDCCH_NUM_REP_RA_R2_VAL,
 EMTC_MPDCCH_NUM_REP_RA_R4_VAL,
 EMTC_MPDCCH_NUM_REP_RA_R8_VAL,
 EMTC_MPDCCH_NUM_REP_RA_R16_VAL,
 EMTC_MPDCCH_NUM_REP_RA_R32_VAL,
 EMTC_MPDCCH_NUM_REP_RA_R64_VAL,
 EMTC_MPDCCH_NUM_REP_RA_R128_VAL,
 EMTC_MPDCCH_NUM_REP_RA_R256_VAL 
};

/*
** Mapping of SiWindowLen enumerated value 
** and actual value 
*/
EmtcSiWindowLen
EmtcSiWindowLenMsVal[EMTC_SI_WINDOW_LEN_NUM_VALUES] = 
{
    EMTC_SI_WINDOW_LEN_20MS, 
    EMTC_SI_WINDOW_LEN_40MS ,
    EMTC_SI_WINDOW_LEN_60MS ,
    EMTC_SI_WINDOW_LEN_80MS ,
    EMTC_SI_WINDOW_LEN_120MS,
    EMTC_SI_WINDOW_LEN_160MS,
    EMTC_SI_WINDOW_LEN_200MS
};

/*
** Mapping of RaResponseWindowSize enumerated value 
** and actual value 
*/
EmtcRaRespWindowSize
EmtcRaResponseWindowSizeVal[EMTC_RA_RESP_WINDOW_SIZE_NUM_VALUES] = 
{
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF20, 
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF50 ,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF80 ,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF120,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF180,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF240,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF320,
    EMTC_RA_RESP_WINDOW_SIZE_VAL_SF400
};

/*
** Mapping of prachStartingSubframe enumerated value 
** and actual value 
*/
EmtcPrachStartSf
EmtcPrachStartingSubframeVal[EMTC_PRACH_START_SF_NUM_VALUES] =
{
    EMTC_PRACH_START_SF_SF2,
    EMTC_PRACH_START_SF_SF4,
    EMTC_PRACH_START_SF_SF8,
    EMTC_PRACH_START_SF_SF16,
    EMTC_PRACH_START_SF_SF32,
    EMTC_PRACH_START_SF_SF64,
    EMTC_PRACH_START_SF_SF128,
    EMTC_PRACH_START_SF_SF256
};

/* 
** Mapping of MPDCCH start SF UESS enumerated value 
** and actual value
*/
UDouble32
EmtcFddR13OfMpdcchStartSfUessR13[EMTC_MAX_START_SF_UESS_NUM_VALUES] =
{
    EMTC_START_SF_UESS_VAL_1,
    EMTC_START_SF_UESS_VAL_1DOT5,
    EMTC_START_SF_UESS_VAL_2,
    EMTC_START_SF_UESS_VAL_2DOT5,
    EMTC_START_SF_UESS_VAL_4,
    EMTC_START_SF_UESS_VAL_5,
    EMTC_START_SF_UESS_VAL_8,
    EMTC_START_SF_UESS_VAL_10

};

/*
** Mapping of NumRepetitionPerPreambleAttemptVal enumerated value 
** and actual value 
*/
EmtcNumRepPerPreambleAttempt
EmtcNumRepetitionPerPreambleAttemptVal[EMTC_MAX_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_NUM_VALUES] =
{   EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N1_VAL,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N2_VAL,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N4_VAL,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N8_VAL,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N16_VAL,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N32_VAL,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N64_VAL,
    EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_N128_VAL
};

/*
** Mapping of numRaPreamblesLte enumerated value 
** and actual value 
*/
EmtcNumRaPreamblesLte
EmtcNumRaPreamblesLteVal[EMTC_NUM_RA_PREAMBLES_LTE_NUM_VALUES] = 
{
 EMTC_NUM_RA_PREAMBLES_LTE_N4_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N8_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N12_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N16_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N20_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N24_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N28_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N32_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N36_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N40_VAL, 
 EMTC_NUM_RA_PREAMBLES_LTE_N44_VAL,
 EMTC_NUM_RA_PREAMBLES_LTE_N48_VAL, 
 EMTC_NUM_RA_PREAMBLES_LTE_N52_VAL, 
 EMTC_NUM_RA_PREAMBLES_LTE_N56_VAL, 
 EMTC_NUM_RA_PREAMBLES_LTE_N60_VAL, 
 EMTC_NUM_RA_PREAMBLES_LTE_N64_VAL 
};
/* 
** Mapping of MPDCCH start SF CSS enumerated value 
** and actual value
*/
UDouble32
EmtcFddR13OfMpdcchStartSfCssR13[EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX ] =
{
    EMTC_MPDCCH_START_SF_CSS_RA_FDD_V1,
    EMTC_MPDCCH_START_SF_CSS_RA_FDD_V1DOT5,
    EMTC_MPDCCH_START_SF_CSS_RA_FDD_V2,
    EMTC_MPDCCH_START_SF_CSS_RA_FDD_V2DOT5,
    EMTC_MPDCCH_START_SF_CSS_RA_FDD_V4,
    EMTC_MPDCCH_START_SF_CSS_RA_FDD_V5,
    EMTC_MPDCCH_START_SF_CSS_RA_FDD_V8,
    EMTC_MPDCCH_START_SF_CSS_RA_FDD_V10
};

/****************************************************************************
 * Function Name  : emtcGetNbVal
 * Inputs         : defaultPagingCycle : Enumerated value of Default paging 
 *                                       cycle for SIB2.
 *                  nb                 : Enumerated value of NB V1310
 *                                       value for SIB2
 * Outputs        : *pNbVal            : Actual value of nb  in 
 *                                       terms of defaultPagingCycle
 * Returns        : COM_SUCCESS : If able to derive the actual value
 *                  COM_FAILURE : If unable to derive the actual value
 * Description    : Converts the enumerated value to actual value.
 ****************************************************************************/
ComReturnType 
emtcGetNbVal
(
    UInt8 defaultPagingCycle,
    UInt8 nb,
    /* SPR 22948 fix start */
    UInt16 *pNbVal
    /* SPR 22948 fix start */
)
{
    ComReturnType retVal = COM_SUCCESS;
    /* coverity 120841 start */
    EmtcDefaultPagingCycle defaultPagingCycleVal = EMTC_DEFAULT_PAGING_CYCLE_RF256;
    /* coverity 120841 stop */

    if(defaultPagingCycle >= EMTC_DEFAULT_PAGING_CYCLE_NUM_VALUES)
    {
        LTE_LOG(LOG_WARNING,"Invalid defaultPagingCycle[%d]",defaultPagingCycle);
        retVal = COM_FAILURE;
    }

    if(retVal)
    {
        defaultPagingCycleVal = EmtcDefPagingCycleVal[defaultPagingCycle];
        switch (nb)
        {
            case EMTC_NB_FOUR_T:
            {
                *pNbVal = COM_FOUR * defaultPagingCycleVal;
                break;
            }
            case EMTC_NB_TWO_T:
            {
                *pNbVal = COM_TWO * defaultPagingCycleVal;
                break;
            }
            case EMTC_NB_ONE_T:
            {
                *pNbVal = defaultPagingCycleVal;
                break;
            }
            case EMTC_NB_HALF_T:
            {
                *pNbVal = defaultPagingCycleVal / COM_TWO;
                break;
            }
            case EMTC_NB_QUARTER_T:
            {
                *pNbVal = defaultPagingCycleVal / COM_FOUR;
                break;
            }
            case EMTC_NB_ONE_EIGHTH_T:
            {
                *pNbVal = defaultPagingCycleVal / COM_EIGHT;
                break;
            }
            case EMTC_NB_ONE_SIXTEENTH_T:
            {
                *pNbVal = defaultPagingCycleVal / COM_SIXTEEN;
                break;
            }
            case EMTC_NB_ONE_THIRTY_SECOND_T:
            {
                *pNbVal = defaultPagingCycleVal / COM_THIRTY_TWO;
                break;
            }
            case EMTC_NB_ONE_SIXTY_FOURTH_T:
            {
                *pNbVal = defaultPagingCycleVal / COM_SIXTY_FOUR;
                break;
            }
            case EMTC_NB_ONE_ONE_HUNDRED_TWENTY_EIGHTH_T:
            {
                *pNbVal = defaultPagingCycleVal / COM_ONE_TWENTY_EIGHT;
                break;
            }
            case EMTC_NB_ONE_TWO_HUNDRED_FIFTY_SIXTH_T:
            {
                *pNbVal = defaultPagingCycleVal / COM_TWO_FIFTY_SIX;
                break;
            }
            default:
            {
                LTE_LOG(LOG_WARNING,"Invalid nb %d",nb);
                retVal = COM_FAILURE;
                break;
            }
        } 

        if(COM_ZERO == *pNbVal)
            retVal = COM_FAILURE;
    }
    return retVal;
}

/****************************************************************************
 * Function Name  : emtcGetCenterRbs 
 * Inputs         : rbCount        : Number of RBs in cell bandwith
 * Outputs        : pCenterRbStart : Number of RB at which central 
 *                                   region starts
 *                  pCenterRbEnd   : Number of RB at which central
 *                                   region ends.
 * Returns        : COM_SUCCESS : If able to derive central bandwith region
 *                  COM_FAILURE : If unable to derive central bandwith region
 * Description    : Derives central bandwith region for given bandwidth.
 ****************************************************************************/
ComReturnType 
emtcGetCenterRbs
(
    EmtcBwRb rbCount,
    UInt8 *pCenterRbStart,  
    UInt8 *pCenterRbEnd
)
{   
    ComReturnType retVal = COM_SUCCESS;
    switch(rbCount)
    {
        case NRB_6:
        {
            *pCenterRbStart =  0;
            *pCenterRbEnd   =  5;
            break;
        }
        case NRB_15:
        {
            *pCenterRbStart = 4; 
            *pCenterRbEnd   = 10; 
            break;
        }
        case NRB_25:
        {
            *pCenterRbStart = 9; 
            *pCenterRbEnd   = 15;
            break;
        }
        case NRB_50:
        {
            *pCenterRbStart = 22; 
            *pCenterRbEnd   = 27;  
            break;
        }

        case NRB_75:
        {
            *pCenterRbStart = 34 ;
            *pCenterRbEnd =  40;
            break;
        }
        case NRB_100:
        {
            *pCenterRbStart = 47;
            *pCenterRbEnd = 52;
            break;
        }
        default:
        {
            LTE_LOG(LOG_WARNING,"Invalid Cell Bandwidth RBs%d",rbCount);
            retVal = COM_FAILURE;
            break;
        }
    }
   return retVal;
}


/****************************************************************************
 * Function Name  : emtcValidateNarrowbandIdx
 * Inputs         : narrowbandNum : Narrowband Idx to be validated
 *                  bandwidth     : Cell bandwidth
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    /*Coverity ID 120851 Fix Start*/
    EmtcBwRb rbCount        = (EmtcBwRb)COM_ZERO;
    /*Coverity ID 120851 Fix End*/
    ComReturnType    result         = COM_FAILURE;
    UInt8    centerRbStart  = COM_ZERO;
    UInt8    centerRbEnd    = COM_ZERO;

    do
    { 
        if( bandwidth >= EMTC_BW_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid bandwidth[%d]",bandwidth);
            break;
        }
      
        rbCount = EmtcRbCount[bandwidth];

        if( narrowbandNum > (rbCount/NUM_PRBS_PER_NARROWBAND))
        {
            LTE_LOG(LOG_WARNING,"narrowband [%d] is not valid for RBs[%d]",
                                   narrowbandNum,rbCount);
            break;
        }
        if( COM_FAILURE == emtcGetCenterRbs(rbCount,&centerRbStart,
                               &centerRbEnd))
        {
            LTE_LOG(LOG_WARNING,"lteGetCenterRbs is unSuccessful");
            break;
        }

        if( (narrowbandNum == 
                ((centerRbStart/NUM_PRBS_PER_NARROWBAND) + COM_ONE)) ||
            (narrowbandNum == 
                ((centerRbEnd/NUM_PRBS_PER_NARROWBAND) + COM_ONE))
          )
        {
            fprintf(stderr,"narrowband [%d] is same as MIB Narrowband",narrowbandNum);
            break;
        }

        LTE_LOG(LOG_WARNING,"emtcValidateNarrowband is Successful");
        result  = COM_SUCCESS;

    }while(COM_ZERO);

    return result;
}

/****************************************************************************
 * Function Name  : emtcValidateSib1BrRepetition
 * Inputs         : Sib1BrRepNum  : SIB1 BR repetition number to be validated
 *                  bandwidth     : Cell bandwidth
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    /*Coverity ID 120810 Fix Start*/
    EmtcBwRb      rbCount      = (EmtcBwRb)COM_ZERO;
    /*Coverity ID 120810 Fix End*/
    /*Coverity Id 120785 Fix Start*/
    EmtcSib1BrRep sib1BrRepVal = EMTC_SIB1_BR_REP_VAL_N4;
    /*Coverity Id 120785 Fix Stop*/
    ComReturnType         result       = COM_FAILURE;

    do
    {
        if( bandwidth >= EMTC_BW_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid bandwidth[%d]",bandwidth);
            break;
        }
        rbCount = EmtcRbCount[bandwidth];

        if( sib1BrRepNum >= EMTC_SIB1_BR_REP_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid sib1BrRepNum[%d]",sib1BrRepNum);
            break;
        }
        sib1BrRepVal  = EmtcSib1BrRepetitionVal[sib1BrRepNum];

        if((rbCount <= NRB_15 ) && (sib1BrRepVal > EMTC_SIB1_BR_REP_VAL_N4) )
        {
            LTE_LOG(LOG_WARNING,"sib1BrRepVal[%d] is not valid for RBs[%d]",
                    sib1BrRepVal,rbCount);
            break;
        }

        LTE_LOG(LOG_INFO,"emtcValidateSib1BrRepetition is Successful");
        result = COM_SUCCESS;

    }while(COM_ZERO);

    return result;
}

/****************************************************************************
 * Function Name  : emtcValidateMpdcchStartSfCssRaMpdcchNumRepRaConfig
 * Inputs         : mpdcchStartSfCssRa : MPDCCH start sf CSS to be validated
 *                  mpdcchNumRepRa     : MPDCCH repettition number
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    /*Coverity Id 120784 Fix Start*/
    EmtcMpdcchNumRepRa mpdcchNumRepRaVal = EMTC_MPDCCH_NUM_REP_RA_R1_VAL;
    /*Coverity Id 120784 Fix Stop*/
    ComReturnType  result            = COM_FAILURE;

    do
    {
        if( mpdcchNumRepRa >= EMTC_MPDCCH_NUM_REP_RA_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid mpdcchNumRepRa[%d]",mpdcchNumRepRa);
            break;
        }
        mpdcchNumRepRaVal = EmtcMpdcchNumRepetitionRaVal[mpdcchNumRepRa];

        if( ( COM_ZERO != ( mpdcchNumRepRaVal % COM_TWO ) ) &&
                ( ( mpdcchStartSfCssRa == MPDCCH_START_SF_1DOT5) ||
                  ( mpdcchStartSfCssRa == MPDCCH_START_SF_2DOT5)) 
          )
        {
            LTE_LOG(LOG_WARNING,"Multiplication of mpdcchStartSfCssRa[%d(enum)]"
                    "and mpdcchNumRepRaVal [%d] is non integer",
                    mpdcchStartSfCssRa,mpdcchNumRepRaVal);
            break;
        }

        LTE_LOG(LOG_INFO,"emtcValidateMpdcchStartSfCssRaMpdcchNumRepRaConfig is"
                " Successful");
        result = COM_SUCCESS;

    }while(COM_ZERO);

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMpdcchStartSfUessMpdcchNumRepConfig
 * Inputs         : mpdcchStartSfUess : MPDCCH start sf UESS to be validated
 *                  mpdcchNumRep      : MPDCCH repettition number
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    /*Coverity ID 120812 Fix Start*/
    EmtcMpdcchNumRepUess mpdcchNumRepVal = (EmtcMpdcchNumRepUess)COM_ZERO;
    /*Coverity ID 120812 Fix Start*/
    ComReturnType result  = COM_SUCCESS;

    mpdcchNumRepVal = EmtcMpdcchNumRepetitionUessVal[mpdcchNumRep]; 

    if( ( COM_ZERO != ( mpdcchNumRepVal % COM_TWO ) ) &&
            ( ( MPDCCH_START_SF_1DOT5 == mpdcchStartSfUess ) || 
              ( MPDCCH_START_SF_2DOT5 == mpdcchStartSfUess)) 
      )
    {
        LTE_LOG(LOG_WARNING,"Multiplication of mpdcchStartSfUess[%d(enum)]"
                "and mpdcchNumRepRaVal [%d] is non integer",
                mpdcchStartSfUess,mpdcchNumRep);
        result = COM_FAILURE;
    }
    else
    {
        LTE_LOG(LOG_INFO,"emtcValidateMpdcchStartSfUessMpdcchNumRepConfig is"
                " Successful");
    }
    return result;
}


/****************************************************************************
 * Function Name  : emtcValidateCellOperatingMode
 * Inputs         : bandwidth         : Cell bandwidth
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    ComReturnType result  = COM_FAILURE;

    do
    {
        if( bandwidth >= EMTC_BW_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid bandwidth[%d]",bandwidth);
            break;
        }
        if( bandwidth < CELL_BW_5)
        {
            LTE_LOG(LOG_WARNING,"EMTC is not supported for BW[%d]",
                    bandwidth);
            break;
        }
        if((bandwidth < CELL_BW_10) && (EMTC_OPERATION_MODE_MIXED == cellOpMode))
        {
            LTE_LOG(LOG_WARNING,"EMTC in Mixed is not supported for BW[%d]",
                    bandwidth);
            break;
        }

        LTE_LOG(LOG_INFO,"emtcValidateCellOperatingMode is"
                " Successful");
        result = COM_SUCCESS;

    }while(COM_ZERO);

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateCfi
 * Inputs         : cellOperatingMode : Mode of operation LTE/EMTC/MIXED
 *                  cfiVal            : CFI value
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates that CFI value is correct as per 
 *                  the Mode of operation.
 ****************************************************************************/
ComReturnType 
emtcValidateCfi
(
    UInt8 cfiVal,
    UInt8 cellOperatingMode
)
{
    ComReturnType result  = COM_SUCCESS;

    if((cellOperatingMode == CELL_MIXED ) && ( cfiVal > COM_TWO ))
    {

        LTE_LOG(LOG_WARNING,"cfiVal[%d] is not valid for cellOperatingMode[%d]",
                cfiVal,cellOperatingMode);
        result = COM_FAILURE;
    }
    else
    {
        LTE_LOG(LOG_INFO,"emtcValidatetCfi is"
                " Successful");
    }

    return result;
}

/****************************************************************************
 * Function Name  : emtcValidateNbDefaultPagingCycleConfig 
 * Inputs         : defaultPagingCycle : Default paging cycle for SIB2
 *                  nbVal              : NB V1310 value for SIB2 
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    :  Converts the input enumerated values to actual values and
 *                   validates that defaultPagingCycle value is correct as per 
 *                   the configured nb value.
 *                   - Check that default paging cycle should be greater than or
 *                     equal to nb.
 ****************************************************************************/
ComReturnType 
emtcValidateNbDefaultPagingCycleConfig
(
    UInt8 defaultPagingCycle, 
    UInt8 nb
)
{
    ComReturnType result  = COM_SUCCESS;
    /* SPR 22948 fix start */
    UInt16 nbVal   = COM_ZERO;
    /* SPR 22948 fix stop */
    
    if(COM_FAILURE == emtcGetNbVal(defaultPagingCycle,nb,&nbVal))
    {
        LTE_LOG(LOG_WARNING,"defaultPagingCycle[%d] and nb[%d] are not valid"
                           defaultPagingCycle,nb);
        result = COM_FAILURE;
    }
    else
    {
        LTE_LOG(LOG_INFO,"emtcValidateNbDefaultPagingCycleConfig is"
                " Successful");
    }


    return result;
}

/****************************************************************************
 * Function Name  : emtcValidateSiWindowSiPeriodicityConfig 
 * Inputs         : siPeriodicity : Periodicity of an SI
 *                  siWindow      : Window lenghth for SI
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    ComReturnType result            = COM_FAILURE;
    /*Coverity ID 120857 Fix Start*/
    EmtcSiPeriodicityVal siPeridicityRf   = (EmtcSiPeriodicityVal)COM_ZERO ;
    /*Coverity ID 120857 Fix End*/
    /*Coverity ID 120831 Fix Start*/
    EmtcSiWindowLen siWindowMs       = (EmtcSiWindowLen)COM_ZERO;
    /*Coverity ID 120831 Fix End*/

    do
    {
        if( siPeriodicity >= EMTC_SI_PERIODICITY_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid siPeriodicity[%d]",siPeriodicity);
            break;
        }
        siPeridicityRf = EmtcSiPeriodicityRfVal[siPeriodicity];

        if(siPeridicityRf <= EMTC_SI_PERIODICITY_VAL_16RF) 
        {
            if( siWindow >= EMTC_SI_WINDOW_LEN_NUM_VALUES)
            {
                LTE_LOG(LOG_WARNING,"Invalid siWindow [%d]",siWindow);
                break;
            }
            siWindowMs = EmtcSiWindowLenMsVal[siWindow];

            if( siWindowMs > (siPeridicityRf * RF_TO_MS_FACTOR))
            {
                LTE_LOG(LOG_INFO,"Si window[%d] and Si periodicity[%d] Invalid",
                        siWindow,siPeriodicity);
                break;
            }
        }

        LTE_LOG(LOG_INFO,"emtcValidateSiWindowSiPeriodicityConfig is"
                " Successful");
        result = COM_SUCCESS;

    }while(COM_ZERO);

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateSiWindowSiRepetitionConfig 
 * Inputs         : siRepetitionPattern : Repetition pattern for SI
 *                  siWindow            : Window lenghth for SI
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    ComReturnType result                 = COM_FAILURE;
    /*Coverity ID 120809 Fix Start*/
    EmtcSiWindowLen siWindowMs             = (EmtcSiWindowLen)COM_ZERO;
    /*Coverity ID 120809 Fix End*/
    /*Coverity ID 120792 Fix Start*/
    EmtcSiRepVal siRepetitionPatternVal = (EmtcSiRepVal)COM_ZERO;
    /*Coverity ID 120792 Fix End*/

    do
    {
        if(siRepetitionPattern >= EMTC_SI_REP_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid siRepetitionPattern[%d]",siRepetitionPattern);
            break;
        }
        siRepetitionPatternVal = EmtcSiRepetitionPatternVal[siRepetitionPattern];

        if( siWindow >= EMTC_SI_WINDOW_LEN_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid siWindow [%d]",siWindow);
            break;
        }
        siWindowMs = EmtcSiWindowLenMsVal[siWindow];

        if( siRepetitionPatternVal >= EMTC_SI_REP_VAL_EVERY_FOURTH_RF )
        {
            if( siWindowMs < (siRepetitionPatternVal * RF_TO_MS_FACTOR) )
            {     
                LTE_LOG(LOG_WARNING,"siWindowMs [%d] is not valid for "
                        "siRepetitionPatternVal[%d]",
                        siWindowMs,siRepetitionPatternVal);
                break;
            }
        }
        LTE_LOG(LOG_INFO,"emtcValidateSiWindowSiRepetitionConfig is"
                "Successful \n");
        result = COM_SUCCESS;

    }while(COM_ZERO);

    return result;
}

/****************************************************************************
 * Function Name  : emtcValidateRaResponseWindowSize
 * Inputs         : raResponseWindowSize : Duration of the RA response window
 *                  mpdcchRepetition     : number of repetitions for MPDCCH 
 *                                         common search space (CSS)
 *                  pdschRepetition      : Maximum value to indicate the set 
 *                                         of PDSCH repetition numbers 
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    ComReturnType  result                  = COM_FAILURE;
    UInt8 pdschRepetitionVal      = COM_ZERO;
    UInt16 mpdcchRepetitionVal     = COM_ZERO;
    UInt16 raResponseWindowSizeVal = COM_ZERO;

    do
    {
        if( mpdcchRepetition >= EMTC_MPDCCH_NUM_REP_RA_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid mpdcchRepetition[%d]",mpdcchRepetition);
            break;
        }
        mpdcchRepetitionVal = (UInt16)EmtcMpdcchNumRepetitionRaVal[mpdcchRepetition];

        if( pdschRepetition >= EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_NUM_VALUES)
        { 
            LTE_LOG(LOG_WARNING,"Invalid pdschRepetition[%d]",pdschRepetition);
            break;
        }
        pdschRepetitionVal = (UInt8)EmtcPdschMaxNumRepCeModeAVal[pdschRepetition];

        if( raResponseWindowSize >= EMTC_RA_RESP_WINDOW_SIZE_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid raResponseWindowSize[%d]",raResponseWindowSize);
            break;
        }
        raResponseWindowSizeVal = (UInt16)EmtcRaResponseWindowSizeVal[raResponseWindowSize];

        /* SPR 22540 Fix Start */
        if( ((raResponseWindowSizeVal) < mpdcchRepetitionVal )  
                ||
                ((raResponseWindowSizeVal) < pdschRepetitionVal ))
        /* SPR 22540 Fix End */
        {
            LTE_LOG(LOG_WARNING,"raResponseWindowSizeVal[%d] is not big enough"
                    "to take care mpdcchRepetitionVal[%d] and"
                    "pdschRepetitionVal[%d]",
                    raResponseWindowSizeVal,mpdcchRepetitionVal,
                    pdschRepetitionVal);
            break;
        }

        LTE_LOG(LOG_INFO,"emtcValidateRaResponseWindowSize is Successful");
        result = COM_SUCCESS;

    }while(COM_ZERO);

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePrachFreqOffset
 * Inputs         : prachFreqOffset : Duration of the RA response window
 *                  bandwidth       : Cell bandwidth
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates that prachFreqOffset as per 3GPP 36.211 5.2.4.
 ****************************************************************************/
ComReturnType 
emtcValidatePrachFreqOffset
(
    UInt8 prachFreqOffset, 
    UInt8 bandwidth 
)
{
    ComReturnType    result  = COM_FAILURE;
    /* SPR 22510 Fix Start */
    EmtcPrbIndexToNbMap_t emtcPrbIndexToNbMapVal;
        
    COM_MEMSET(&emtcPrbIndexToNbMapVal, COM_ZERO, sizeof(EmtcPrbIndexToNbMap_t));
    /* SPR 22510 Fix End */

    do
    {
        if( bandwidth >= EMTC_BW_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid bandwidth[%d]",bandwidth);
            break;
        }

        if( prachFreqOffset >= MAX_NUM_PRB )
        {
            LTE_LOG(LOG_WARNING,"Invalid prachFreqOffset[%d]",prachFreqOffset);
            break;
        }

        /* SPR 22510 Fix Start */
        emtcPrbIndexToNbMapVal = emtcPrbIndexToNbMap[bandwidth][prachFreqOffset];
 
        if( (emtcPrbIndexToNbMapVal.nbIndex != 255) &&
            (emtcPrbIndexToNbMapVal.prbIndex == 0))
        {
            LTE_LOG(LOG_INFO,"emtcValidatePrachFreqOffset is Successful"
                    "prachFreqOffset[%d] is start of Narrowband [%d]",
                    prachFreqOffset,emtcPrbIndexToNbMapVal.nbIndex);
            result  = COM_SUCCESS;
        }
        /* SPR 22510 Fix End */

    }while(COM_ZERO);

    return result;
}


/****************************************************************************
 * Function Name  : emtcValidateDefPagingCycleMpdcchRepPagingConfig
 * Inputs         : defaultPagingCycle : Default paging cycle for SIB2
 *                  mpdcchRepPaging    : Maximum number of repetitions for 
 *                                       MPDCCH CSS for paging.
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    ComReturnType result                 = COM_FAILURE;
    /*Coverity ID 120841 Fix Start*/
    EmtcDefaultPagingCycle defaultPagingCycleVal = (EmtcDefaultPagingCycle)COM_ZERO;
    /*Coverity ID 120841 Fix End*/
    /*Coverity ID 120793 Fix Start*/
    EmtcMpdcchNumRepPaging mpdcchRepPagingVal    = (EmtcMpdcchNumRepPaging)COM_ZERO;
    /*Coverity ID 120793 Fix End*/

    do
    {
        if(defaultPagingCycle >= EMTC_DEFAULT_PAGING_CYCLE_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid defaultPagingCycle[%d]",defaultPagingCycle);
            break;
        }

        defaultPagingCycleVal = EmtcDefPagingCycleVal[defaultPagingCycle];

        if( mpdcchRepPaging >= EMTC_MPDCCH_NUM_REP_PAGING_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid mpdcchRepPaging [%d]",mpdcchRepPaging);
            break;
        }

        mpdcchRepPagingVal = EmtcMpdcchRepPagingVal[mpdcchRepPaging]; 

        if( COM_ZERO != (defaultPagingCycleVal % mpdcchRepPagingVal ))

        {
            LTE_LOG(LOG_WARNING,"defaultPagingCycleVal[%d] is not big enough"
                    "to take care mpdcchRepPagingVal[%d]",
                    defaultPagingCycleVal,mpdcchRepPagingVal);
            break;
        }
        LTE_LOG(LOG_INFO,"emtcValidateDefPagingCycleMpdcchRepPagingConfig "
                "is Successful");
        result = COM_SUCCESS;
    }while(COM_ZERO);

    return result;
}

/****************************************************************************
 * Function Name  : emtcValidateNumRepetitionPerPreambleAttempt
 * Inputs         : numRepPerPreambleAttempt : Number of PRACH repetitions 
 *                                             per attempt per CE level
 *                  prachStartingSubframe    : PRACH starting subframe 
 *                                             periodicity.
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
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
)
{
    ComReturnType  result  = COM_FAILURE;
    /* Cov_120855_Fix_Start */
    UInt16    numRepPerPreambleAttemptVal = COM_ZERO;
    UInt16    prachStartingSubframeVal   = COM_ZERO;
    /* Cov_120855_Fix_End */

    do
    {
        if( numRepPerPreambleAttempt >= EMTC_MAX_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid numRepPerPreambleAttempt[%d]",numRepPerPreambleAttempt);
            break;
        }
        /* Cov_120855_Fix_Start */
        numRepPerPreambleAttemptVal =
            (UInt16)EmtcNumRepetitionPerPreambleAttemptVal[numRepPerPreambleAttempt];
        /* Cov_120855_Fix_End */

        if( prachStartingSubframe >= EMTC_PRACH_START_SF_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid prachStartingSubframe[%d]",prachStartingSubframe);
            break;
        }
        /* Cov_120855_Fix_Start */
        prachStartingSubframeVal = 
            (UInt16)EmtcPrachStartingSubframeVal[prachStartingSubframe];
        /* Cov_120855_Fix_End */
        if( (UInt16)numRepPerPreambleAttemptVal > (UInt16)prachStartingSubframeVal)
        {
            LTE_LOG(LOG_WARNING,"numRepPerPreambleAttemptVal[%d] and"
                    "prachStartingSubframe[%d] is not valid",
                    numRepPerPreambleAttemptVal,
                    prachStartingSubframe);
            break;
        }

        LTE_LOG(LOG_INFO,"emtcValidateNumRepetitionPerPreambleAttempt"
                "is Successful");
        result = COM_SUCCESS;

    }while(COM_ZERO);

    return result;
}


/****************************************************************************
 * Function Name  : emtcValidateRsrpThresholdsPrachInfoList
 * Inputs         : pRrsrpThresholdsPrachInfoList : RSRP thresholds for BL UEs
 *                                                  and UEs in CE to select 
 *                                                  PRACH resource set.
 *                  numRsrpThresholdsCount        : Number of thresholds
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the rsrp entries in the list should be 
 *                  in increasing order.
 ****************************************************************************/
ComReturnType emtcValidateRsrpThresholdsPrachInfoList 
(
    UInt8 *pRsrpThresholdsPrachInfoList, 
    UInt8  numRsrpThresholds
)
{
    ComReturnType  result  = COM_SUCCESS;
    UInt8  rsrpIdx = COM_ZERO;

    for(rsrpIdx = COM_ZERO ; rsrpIdx < (numRsrpThresholds - COM_ONE)  ; rsrpIdx++)
    {
        if ( pRsrpThresholdsPrachInfoList != PNULL )
        {
        if( pRsrpThresholdsPrachInfoList[rsrpIdx +COM_ONE] < 
                pRsrpThresholdsPrachInfoList[rsrpIdx])
        {
            LTE_LOG(LOG_WARNING,"pRsrpThresholdsPrachInfoList is"
                                "unSuccessful");
            result = COM_FAILURE;
            break;
        }
    }
    }
    return result;
}

/****************************************************************************
 * Function Name  : emtcValidatePreambleInfo
 * Inputs         : cellOpMode: Cell Operating mode
 *                  *pPreambleInfo : List of preamble info for each 
 *                                   ce level              
 *                  preambleInfoCount : Number of PreambleInfo recieved
 *                  numRaPreamblesLte : Number of Preambles used for LTE
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates that pPreambleInfoe list should be non-overlapping
 *                  across the ce-levels and value configured for LTE also.
 ****************************************************************************/
ComReturnType emtcValidatePreambleInfo 
(
    UInt8               cellOpMode,
    EmtcPreambleInfo_t *pEmtcPreambleInfo, 
    UInt8             preambleInfoCount,
    UInt8             numRaPreamblesLte
)
{
    UInt8 preambleCount = COM_ZERO;
    UInt8 preambleStartIdx = COM_ZERO;
    ComReturnType  result  = COM_SUCCESS;
    /*Coverity ID 120823 Fix Start*/
    EmtcNumRaPreamblesLte numRaPreamblesLteVal = (EmtcNumRaPreamblesLte)COM_ZERO;
    /*Coverity ID 120823 Fix End*/

    numRaPreamblesLteVal = EmtcNumRaPreamblesLteVal[numRaPreamblesLte];
 
    for( preambleCount = preambleStartIdx ; 
            preambleCount < (preambleInfoCount - COM_ONE);
            preambleCount++,preambleStartIdx++)
    {
        /* For each CE list, last preamble >= firstpreamble */
        if( pEmtcPreambleInfo[preambleCount].firstPreamble >
                pEmtcPreambleInfo[preambleCount].lastPreamble )
        {
            LTE_LOG(LOG_WARNING,"FirstPreamble[%d] LastPreamble[%d] is"
                    "incorrect",pEmtcPreambleInfo[preambleCount].firstPreamble,
                    pEmtcPreambleInfo[preambleCount].lastPreamble);
            result = COM_FAILURE;
            break;
        }

        if(( EMTC_OPERATION_MODE_STANDALONE != cellOpMode ) && 
           (pEmtcPreambleInfo[preambleCount].firstPreamble < numRaPreamblesLteVal ))
        {
                LTE_LOG(LOG_WARNING,"FirstPreamble[%d] is overlapping with "
                        "LTE PReamble 0-[%d]",
                        pEmtcPreambleInfo[preambleCount].firstPreamble,
                        numRaPreamblesLteVal);
                result = COM_FAILURE;
                break;
        }
    }
    return result;
}

/****************************************************************************
 * Function Name  : emtcValidatefddDlSubframeBitmap
 * Inputs         : pFddDlSubframeBitmap : List of fdd ul/dl SFs
 *                  bitmapCount : Number of indexes to scan for bitmap
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates that pFddDlSubframeBitmap is non-zero.
 ****************************************************************************/
ComReturnType emtcValidatefddDlSubframeBitmap
(
    UInt8         *pFddDlSubframeBitmap,
    UInt8         bitmapCount
)
{
    ComReturnType  result    = COM_FAILURE;
    UInt8  bitmapIdx = COM_ZERO;

    if( COM_PNULL != pFddDlSubframeBitmap)
    {
        for(bitmapIdx = COM_ZERO ; bitmapIdx < bitmapCount ; bitmapIdx++)
        {
            if( COM_ZERO != pFddDlSubframeBitmap[bitmapIdx] )
            {
                result = COM_SUCCESS;
                break;
            }
        }
    }
    else
    {
        LTE_LOG(LOG_WARNING,"pFddDlSubframeBitmap is NULL");

    }

    if(bitmapIdx < bitmapCount)
        LTE_LOG(LOG_WARNING,"datefddDlSubframeBitmap is successful");

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatefddUlSubframeBitmap
 * Inputs         : pFddUlSubframeBitmap : List of fdd ul SFs
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates that pFddUlSubframeBitmap is non-zero.
 ****************************************************************************/
ComReturnType emtcValidatefddUlSubframeBitmap
(
    UInt16         *pFddUlSubframeBitmap
)
{
    ComReturnType  result    = COM_FAILURE;

    if (COM_ZERO != *pFddUlSubframeBitmap)
    {
        result = COM_SUCCESS;
    }
 
    return result;
}

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
)
{
    ComReturnType    result  = COM_FAILURE;
    /*Coverity Id 120776 Fix Start*/
    EmtcBwRb rbCount = NRB_6;
    /*Coverity Id 120776 Fix Stop*/
    do
    {
        if( bandwidth >= EMTC_BW_NUM_VALUES)
        {
            LTE_LOG(LOG_WARNING,"Invalid bandwidth[%d]",bandwidth);
            break;
        }
        rbCount = EmtcRbCount[bandwidth];

        if((rbCount > NRB_6) && ( startSymbol >= 4))
        {
            LTE_LOG(LOG_WARNING,"startSymbol is [%d] and rbCount is [%d]"
                   "Invalid Configuration",startSymbol,rbCount);
            break;
        }
        if((rbCount == NRB_6) && (( startSymbol == 1) || (startSymbol > 4))) 
        {
            LTE_LOG(LOG_WARNING,"startSymbol is [%d] and rbCount is [%d]"
                   "Invalid Configuration",startSymbol,rbCount);
            break;
        }


        LTE_LOG(LOG_INFO,"emtcValidateStartSymbol is Successful");
        result = COM_SUCCESS;

    }while(COM_ZERO);

    return result;
}
/* SPR 22512 Fix End */
/****************************************************************************
 * Function Name  : emtcValidateTransmissionTypeRange 
 * Inputs         : transmissionType
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for transmissionType.
 ****************************************************************************/
ComReturnType
emtcValidateTransmissionTypeRange
(
 UInt8  transmissionType
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(transmissionType > EMTC_TRANSMISSION_TYPE_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of transmissionType"
                "Max allowed is %d",transmissionType,
                EMTC_TRANSMISSION_TYPE_MAX);
        result = COM_FAILURE;
    }

    return result;
}

/****************************************************************************
 * Function Name  : emtcValidateMaxUeRange 
 * Inputs         : maxEmtcUe
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for maxEmtcUe.
 ****************************************************************************/
ComReturnType
emtcValidateMaxUeRange
(
 UInt8  maxEmtcUe
)
{
    ComReturnType  result    = COM_SUCCESS;

    if((maxEmtcUe < MIN_NUM_SUPPORTED_EMTC_UE) || (maxEmtcUe > MAX_NUM_SUPPORTED_EMTC_UE))
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of Num EMTC UE"
                "(allowed %d to %d)",maxEmtcUe,MIN_NUM_SUPPORTED_EMTC_UE,MAX_NUM_SUPPORTED_EMTC_UE);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateModPeriodCoeffRange 
 * Inputs         : modificationPeriodCoeff
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for modificationPeriodCoeff.
 ****************************************************************************/
ComReturnType
emtcValidateModPeriodCoeffRange
(
 UInt8 modificationPeriodCoeff 
)
{
    ComReturnType  result    = COM_SUCCESS;
    if ( modificationPeriodCoeff > EMTC_MOD_PERIOD_COEFF_MAX)
    {
        LTE_LOG(LOG_WARNING,"Invalid value [%d] configured for mod period coeff"
                "Max allowed is %d",modificationPeriodCoeff,
                  EMTC_MOD_PERIOD_COEFF_MAX);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateNbRange 
 * Inputs         : Nb
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for Nb.
 ****************************************************************************/
ComReturnType
emtcValidateNbRange
(
  UInt8   nB
)
{
    ComReturnType  result    = COM_SUCCESS;

    if ( nB > EMTC_NB_MAX)
    {
        LTE_LOG(LOG_WARNING,"Invalid value [%d] configured for Emtc Nb",
                  "Max allowed is %d",nB,EMTC_NB_MAX);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateSib1BrRepetitionRange 
 * Inputs         : sib1BrRepetition 
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for sib1BrRepetition.
 ****************************************************************************/
ComReturnType
emtcValidateSib1BrRepetitionRange
(
 UInt8  sib1BrRepetition
)
{
    ComReturnType  result    = COM_SUCCESS;
    if ( sib1BrRepetition > EMTC_SIB1_BR_REPETITION_NMAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of sib1BrRepettion."
                "Max allowed is %d",sib1BrRepetition ,
                  EMTC_SIB1_BR_REPETITION_NMAX);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateQRxLevMinCeRange 
 * Inputs         : qRxLevMinCe
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for qRxLevMinCe.
 ****************************************************************************/
ComReturnType
emtcValidateQRxLevMinCeRange
(
 SInt8 qRxLevMinCe 
)
{
    ComReturnType  result    = COM_SUCCESS;
    if((qRxLevMinCe < EMTC_MIN_RXLEVMIN ) ||
            (qRxLevMinCe > EMTC_MAX_RXLEVMIN) )
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of qRxLevMinCeR13"
                "(allowed %d to %d)",qRxLevMinCe,
                   EMTC_MIN_RXLEVMIN,EMTC_MAX_RXLEVMIN);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateQqualMinRsrqCeRange 
 * Inputs         : qQualMinRsrqCe
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for qQualMinRsrqCe.
 ****************************************************************************/
ComReturnType
emtcValidateQqualMinRsrqCeRange
(
 SInt8 qQualMinRsrqCe 
)
{
    ComReturnType  result    = COM_SUCCESS;
    if((qQualMinRsrqCe < EMTC_MIN_Q_QUALMINRSRQ) || 
            (qQualMinRsrqCe > EMTC_MAX_Q_QUALMINRSRQ ))
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of qQualMinRsrqCeR13"
                "(allowed %d to %d)",qQualMinRsrqCe,
                 EMTC_MIN_Q_QUALMINRSRQ,EMTC_MAX_Q_QUALMINRSRQ);

        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateSiWindowLenRange 
 * Inputs         : siWindowLen
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for siWindowLen.
 ****************************************************************************/
ComReturnType
emtcValidateSiWindowLenRange
(
 UInt8  siWindowLen
)
{
    ComReturnType  result    = COM_SUCCESS;
    if (siWindowLen > EMTC_SI_WINDOW_LEN_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of siWindowLengthBRR13"
                "Max allowed is %d ",siWindowLen,EMTC_SI_WINDOW_LEN_MAX); 
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateSiRepPatternRange 
 * Inputs         : siRepPattern
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for siRepPattern.
 ****************************************************************************/
ComReturnType
emtcValidateSiRepPatternRange
(
 UInt8 siRepPattern 
)
{
    ComReturnType  result    = COM_SUCCESS;

    if (siRepPattern > EMTC_SI_REPETITION_MAX ) 
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of "
                "siRepetitionPatternR13.Max allowed is %d",siRepPattern,
                EMTC_SI_REPETITION_MAX);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateSiCountRange 
 * Inputs         : siCount
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for siCount.
 ****************************************************************************/
ComReturnType
emtcValidateSiCountRange
(
 UInt8  siCount 
)
{
    ComReturnType  result    = COM_SUCCESS;

    if((siCount < EMTC_MIN_SI_COUNT ) || 
            (siCount > EMTC_MAX_SI_COUNT))
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of br_si_count"
                "(allowed %d to %d)",siCount,
                EMTC_MIN_SI_COUNT, EMTC_MAX_SI_COUNT);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateSiPeriodicityRange 
 * Inputs         : siPeriodicity
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for siPeriodicity.
 ****************************************************************************/
ComReturnType
emtcValidateSiPeriodicityRange
(
 UInt8  siPeriodicity
)
{
    ComReturnType  result    = COM_SUCCESS;
    if (siPeriodicity > EMTC_SI_PERIODICITY_MAX)
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of brSiPeriodicity"
                "Max allowed is %d",siPeriodicity,
                EMTC_SI_PERIODICITY_MAX);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateSibTypeRange 
 * Inputs         : sibType
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for sibType.
 ****************************************************************************/
ComReturnType
emtcValidateSibTypeRange
(
 UInt8  sibType
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(sibType > EMTC_SIB_TYPE_MAX)
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of parameter "
                "SibType.Max allowed is %d",sibType,
                EMTC_SIB_TYPE_MAX);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateNumSibTypeRange 
 * Inputs         : numSibType
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for numSibType.
 ****************************************************************************/
ComReturnType
emtcValidateNumSibTypeRange
(
 UInt8 numSibType 
)
{
    ComReturnType  result    = COM_SUCCESS;

    if( numSibType > EMTC_MAX_SIB_COUNT)
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of numSibType"
                "Max allowed is %d",numSibType,EMTC_MAX_SIB_COUNT);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateStartSymbolRange
 * Inputs         : startSymbol
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for startSymbol.
 ****************************************************************************/
ComReturnType
emtcValidateStartSymbolRange
(
 UInt8 startSymbol 
)
{
    ComReturnType  result    = COM_SUCCESS;

    if((startSymbol < EMTC_MIN_START_SYMBOL) ||
            (startSymbol > EMTC_MAX_START_SYMBOL))
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of startSymbolBrR13"
                "(allowed %d to %d)",startSymbol,
                EMTC_MIN_START_SYMBOL,
                EMTC_MAX_START_SYMBOL);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateSiHoppingConfigRange
 * Inputs         : siHoppingConfig
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for siHoppingConfig.
 ****************************************************************************/
ComReturnType
emtcValidateSiHoppingConfigRange
(
 UInt8 siHoppingConfig
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(siHoppingConfig > EMTC_SI_HOPPING_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of si hopping config"
                "Max allowed is %d",siHoppingConfig,EMTC_SI_HOPPING_MAX);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateSiValidityTimerRange
 * Inputs         : siValidityTimer
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for siValidityTimer.
 ****************************************************************************/
ComReturnType
emtcValidateSiValidityTimerRange
(
 UInt8 siValidityTimer 
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(siValidityTimer > COM_ONE)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of si validity timer"
                "Max allowed is %d ",siValidityTimer,COM_ONE);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMpdcchStartSfUessRange
 * Inputs         : mpdcchStartSfUess
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for mpdcchStartSfUess.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchStartSfUessRange
(
 UInt8  mpdcchStartSfUess
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(mpdcchStartSfUess > EMTC_MPDCCH_START_SF_UESS_FDD_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of mpdcchStartSfUESSRaR13"
                "Max allowed is %d ",mpdcchStartSfUess,
                 EMTC_MPDCCH_START_SF_UESS_FDD_MAX);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMpdcchNumRepUessRange
 * Inputs         : mpdcchNumRepUess
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for mpdcchNumRepUess.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchNumRepUessRange
(
 UInt8  mpdcchNumRepUess
)
{
    ComReturnType  result    = COM_SUCCESS;
    if (mpdcchNumRepUess > EMTC_MPDCCH_NUM_REPETITION_UESS_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of "
                "mpdcch_NumRepetition_r13.Max allowed is %d ",
                mpdcchNumRepUess,EMTC_MPDCCH_NUM_REPETITION_UESS_MAX);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMpdcchPdschHoppingRange 
 * Inputs         : mpdcchPdschHopping
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for mpdcchPdschHopping.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchPdschHoppingRange
(
 UInt8 mpdcchPdschHopping
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(mpdcchPdschHopping > EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of mpdcch pdsch hopping config"
                "Max allowed is %d ",mpdcchPdschHopping,
                EMTC_MPDCCH_PDSCH_HOPPING_CONFIG_MAX);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateNumCeLevelRange
 * Inputs         : numCeLevel
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for numCeLevel.
 ****************************************************************************/
ComReturnType
emtcValidateNumCeLevelRange
(
 UInt8  numCeLevel
)
{
    ComReturnType  result    = COM_SUCCESS;

    if((numCeLevel < EMTC_MIN_CE_LEVEL) ||
            (numCeLevel > EMTC_MAX_CE_LEVEL))
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of maxCeLevelSupported"
                "(allowed %d to %d)",numCeLevel,
                EMTC_MIN_CE_LEVEL,EMTC_MAX_CE_LEVEL);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePreambleTransMaxRange 
 * Inputs         : preambleTransMax
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for preambleTransMax.
 ****************************************************************************/
ComReturnType
emtcValidatePreambleTransMaxRange
(
 UInt8 preambleTransMax
)
{
    ComReturnType  result    = COM_SUCCESS;

    if (preambleTransMax > EMTC_PREAMBLE_TRANS_MAX_CE_MAX)
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of PreambleTransMax"
                "Max allowed is %d",preambleTransMax,
                EMTC_PREAMBLE_TRANS_MAX_CE_MAX);
        result =  COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePreambleRange 
 * Inputs         : emtcPreamble
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for emtcPreamble.
 ****************************************************************************/
ComReturnType
emtcValidatePreambleRange
(
 UInt8  emtcPreamble
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(emtcPreamble > EMTC_MAX_PREAMBLE)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of parameter "
                "lastPreamble.Max allowed is %d ",emtcPreamble,
                EMTC_MAX_PREAMBLE);
        result =  COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateRaResponseWindowSizeRange 
 * Inputs         : raResponseWindowSize
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for raResponseWindowSize.
 ****************************************************************************/
ComReturnType
emtcValidateRaResponseWindowSizeRange
(
 UInt8  raResponseWindowSize
)
{
    ComReturnType  result    = COM_SUCCESS;

    if( raResponseWindowSize > EMTC_RA_RESPONSE_WINDOW_SIZE_SF_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of parameter"
                "RAResponseWindowSize.Max allowed is %d",raResponseWindowSize,
                    EMTC_RA_RESPONSE_WINDOW_SIZE_SF_MAX);
        result =  COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMacContentionResTimerRange
 * Inputs         : macContentionResTimer
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for macContentionResTimer.
 ****************************************************************************/
ComReturnType
emtcValidateMacContentionResTimerRange
(
 UInt8 macContentionResTimer 
)
{
    ComReturnType  result    = COM_SUCCESS;

    if (macContentionResTimer > EMTC_MAC_CONTENTION_RESOLUTION_TIMER_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of parameter"
                "MACContentionResolutionTimer.Max allowed is %d",
                macContentionResTimer,EMTC_MAC_CONTENTION_RESOLUTION_TIMER_MAX);
        result =  COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateRarHoppingConfigRange 
 * Inputs         : rarHoppingConfig
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for rarHoppingConfig.
 ****************************************************************************/
ComReturnType
emtcValidateRarHoppingConfigRange
(
 UInt8  rarHoppingConfig
)
{
    ComReturnType  result    = COM_SUCCESS;

    if( rarHoppingConfig > EMTC_RAR_HOPPPING_CONFIG_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of parameter"
                "rarHoppingConfig.Max allowed is %d ",
                rarHoppingConfig,EMTC_RAR_HOPPPING_CONFIG_MAX);
        result =  COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateiN1PucchAnRepRange
 * Inputs         : n1PucchAnRep
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for n1PucchAnRep.
 ****************************************************************************/
ComReturnType
emtcValidateN1PucchAnRepRange
(
 UInt16  n1PucchAnRep
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(n1PucchAnRep > EMTC_MAX_N1_PUCCH_AN)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of parameter"
                "n1PucchAnRepR13.Max allowed is %d ",
                n1PucchAnRep,EMTC_MAX_N1_PUCCH_AN);
        result =  COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePdschMaxNumRepModeARange
 * Inputs         : pdschMaxNumRepModeA
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for pdschMaxNumRepModeA.
 ****************************************************************************/
ComReturnType
emtcValidatePdschMaxNumRepModeARange
(
 UInt8  pdschMaxNumRepModeA
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(pdschMaxNumRepModeA > EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of parameter"
                "pdschMaxNumRepetitionCeModeAR13.Max allowed is %d",
                pdschMaxNumRepModeA,EMTC_PDSCH_MAX_NUM_REPETITION_CE_MODE_A_MAX);
        result    = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateRsrpValRange
 * Inputs         : rsrpVal
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for rsrpVal.
 ****************************************************************************/
ComReturnType
emtcValidateRsrpValRange
(
 UInt8  rsrpVal
)
{
    ComReturnType  result    = COM_SUCCESS;
    if(rsrpVal > EMTC_MAX_RSRP_RANGE )
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of RsrpVal"
                "Max allowed is %d",rsrpVal,EMTC_MAX_RSRP_RANGE);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMpdcchStartSfCssRange 
 * Inputs         : mpdcchStartSfCss
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for mpdcchStartSfCss.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchStartSfCssRange
(
 UInt8 mpdcchStartSfCss
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(mpdcchStartSfCss > EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of mpdcchStartSfCssRaR13"
                "Max allowed is %d ",mpdcchStartSfCss,
                  EMTC_MPDCCH_START_SF_CSS_RA_FDD_MAX);
        result    = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePrachConfigIdxRange
 * Inputs         : prachConfigIdx
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for prachConfigIdx.
 ****************************************************************************/
ComReturnType
emtcValidatePrachConfigIdxRange
(
 UInt8  prachConfigIdx
)
{
    ComReturnType  result    = COM_SUCCESS;

    if (prachConfigIdx > EMTC_MAX_PRACH_CONFIG_INDEX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of prachConfigIndexR13"
                "Max allowed is %d",prachConfigIdx,
                EMTC_MAX_PRACH_CONFIG_INDEX);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePrachFreqOffsetRange 
 * Inputs         : prachFreqOffset
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for prachFreqOffset.
 ****************************************************************************/
ComReturnType
emtcValidatePrachFreqOffsetRange
(
 UInt8  prachFreqOffset
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(prachFreqOffset > EMTC_MAX_PRACH_FREQ_OFFSET)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of prachFreqOffsetR13"
                "Max allowed is %d ",prachFreqOffset,
                EMTC_MAX_PRACH_FREQ_OFFSET);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePrachStartingSfRange
 * Inputs         : prachStartingSf
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for prachStartingSf.
 ****************************************************************************/
ComReturnType
emtcValidatePrachStartingSfRange
(
 UInt8  prachStartingSf
 )
{
    ComReturnType  result    = COM_SUCCESS;

    if(prachStartingSf > EMTC_PRACH_STARTING_SF_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of prachStartingSubframeR13"
                "Max allowed is %d",prachStartingSf,
                EMTC_PRACH_STARTING_SF_MAX);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMaxNumPreambleAttemptRange
 * Inputs         : maxNumPreambleAttempt
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for maxNumPreambleAttempt.
 ****************************************************************************/
ComReturnType
emtcValidateMaxNumPreambleAttemptRange
(
 UInt8  maxNumPreambleAttempt
)
{
    ComReturnType  result    = COM_SUCCESS;

    if (maxNumPreambleAttempt > EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_MAX)
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of maxNumPreambleAttemptCeR13"
                "Max allowed is %d ",maxNumPreambleAttempt,
                EMTC_MAX_NUM_PREAMBLE_ATTEMPT_CE_MAX);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateNumRepPerPreambleAttemptRange 
 * Inputs         : numRepPerPreambleAttempt
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for numRepPerPreambleAttempt.
 ****************************************************************************/
ComReturnType
emtcValidateNumRepPerPreambleAttemptRange
(
 UInt8  numRepPerPreambleAttempt
)
{
    ComReturnType  result    = COM_SUCCESS;
    if (numRepPerPreambleAttempt > EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX )
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of "
                "numRepetitionPerPreambleAttemptR13.Max allowed is %d",
                numRepPerPreambleAttempt,EMTC_NUM_REPETITION_PER_PREAMBLE_ATTEMPT_MAX);
        result = COM_FAILURE;
    }


    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMpdcchNumRepCssRange
 * Inputs         : mpdcchNumRepCss
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for mpdcchNumRepCss.
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchNumRepCssRange
(
 UInt8  mpdcchNumRepCss
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(mpdcchNumRepCss > EMTC_MPDCCH_NUM_REPETITION_RA_MAX )
    {
        LTE_LOG(LOG_WARNING, "Invalid value %d of mpdcchNumRepetitionRaR13"
                "Max allowed is %d",mpdcchNumRepCss,
                EMTC_MPDCCH_NUM_REPETITION_RA_MAX);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePrachHoppingConfigRange 
 * Inputs         : prachHoppingConfig 
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for .
 ****************************************************************************/
ComReturnType
emtcValidatePrachHoppingConfigRange
(
    UInt8 prachHoppingConfig
)
{
    ComReturnType  result    = COM_SUCCESS;
    
    if(prachHoppingConfig > EMTC_PRACH_HOPPING_CONFIG_MAX )
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of prachHoppingConfigR13"
                "Max allowed is %d ",prachHoppingConfig,
                EMTC_PRACH_HOPPING_CONFIG_MAX);
        result = COM_FAILURE;
    }

    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateNarrowbandIdxRange
 * Inputs         : narrowbandIdx
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for .
 ****************************************************************************/
ComReturnType
emtcValidateNarrowbandIdxRange
(
 UInt8  narrowbandIdx
)
{
    ComReturnType  result    = COM_SUCCESS;

    if ((narrowbandIdx < EMTC_MIN_AVAILABLE_NARROWBAND) ||
            (narrowbandIdx >  EMTC_MAX_AVAILABLE_NARROWBAND))
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of narrowbandIdx"
                "(allowed %d to %d)",narrowbandIdx,
                EMTC_MIN_AVAILABLE_NARROWBAND,EMTC_MAX_AVAILABLE_NARROWBAND);
        result = COM_FAILURE;
    }

    return result;
}

/****************************************************************************
 * Function Name  : emtcValidatePucchNumRepetitionCeFormat1Range
 * Inputs         : pucchNumRepetitionCeFormat1
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for .
 ****************************************************************************/
ComReturnType
emtcValidatePucchNumRepetitionCeFormat1Range
(
 UInt8  pucchNumRepetitionCeFormat1
)
{
    ComReturnType  result    = COM_SUCCESS;

   /* Cov_fix 147582 */ 
    if ((pucchNumRepetitionCeFormat1 >  EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX))
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of pucchNumRepetitionCeFormat1"
                "(allowed %d to %d)",pucchNumRepetitionCeFormat1,
                EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_R1,
                EMTC_PUCCH_NUM_REPETITION_CE_MODE_A_FORMAT1_MAX);
        result = COM_FAILURE;
    }

    return result;
}

/****************************************************************************
 * Function Name  : emtcValidatePuschMaxNumRepModeARange
 * Inputs         : puschMaxNumRepModeA
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for puschMaxNumRepModeA.
 ****************************************************************************/
ComReturnType
emtcValidatePuschMaxNumRepModeARange
(
 UInt8  puschMaxNumRepModeA
)
{
    ComReturnType  result    = COM_SUCCESS;
    
    if(puschMaxNumRepModeA > EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invlid value %d of puschMaxNumRepetitionCeModeAR13"
                "Max allowed is %d",puschMaxNumRepModeA,
                 EMTC_PUSCH_MAX_NUM_REPTITION_CE_MODE_A_MAX);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMpdcchMaxNumRepPagingRange
 * Inputs         : mpdcchNumRepPaging
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for mpdcchNumRepPaging .
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchMaxNumRepPagingRange
(
 UInt8  mpdcchNumRepPaging
)
{
    ComReturnType  result    = COM_SUCCESS;
    
    if (mpdcchNumRepPaging > EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_MAX)
    {
        LTE_LOG(LOG_WARNING, "Invalid Value %d of mpdcchNumRepetitionPagingR13"
                "Max allowed is %d",mpdcchNumRepPaging,
                 EMTC_MPDCCH_MAX_NUM_REPETITION_PAGING_MAX);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidateMpdcchNumRepPagingRange
 * Inputs         : mpdcchNumRepPaging
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for mpdcchNumRepPaging .
 ****************************************************************************/
ComReturnType
emtcValidateMpdcchNumRepPagingRange
(
 UInt8  mpdcchNumRepPaging
)
{
    ComReturnType  result    = COM_SUCCESS;
    
    if ((mpdcchNumRepPaging < EMTC_MPDCCH_NUM_REPETITION_PAGING_MIN) ||
         (mpdcchNumRepPaging > EMTC_MPDCCH_NUM_REPETITION_PAGING_MAX))
    {
        LTE_LOG(LOG_WARNING, "Invalid Value %d of mpdcchNumRepetitionPaging"
                "Allowed range is %d to %d",mpdcchNumRepPaging,
                 EMTC_MPDCCH_NUM_REPETITION_PAGING_MIN,EMTC_MPDCCH_NUM_REPETITION_PAGING_MAX);
        result = COM_FAILURE;
    }
    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePdschPagingRepRange 
 * Inputs         : pdschPagingRep
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for pdschPagingRep.
 ****************************************************************************/
ComReturnType
emtcValidatePdschPagingRepRange
(
 UInt8 pdschPagingRep
)
{
    ComReturnType  result    = COM_SUCCESS;
   
    if((pdschPagingRep < EMTC_PDSCH_PAGING_REP_MIN) || 
       (pdschPagingRep > EMTC_PDSCH_PAGING_REP_MAX))
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of pdschPagingRepLevel1"
                "Allowed range is %d to %d",pdschPagingRep,
                 EMTC_PDSCH_PAGING_REP_MIN,EMTC_PDSCH_PAGING_REP_MAX);
        result = COM_FAILURE;
    }


    return result;
}
/****************************************************************************
 * Function Name  : emtcValidatePucchNumRepCeMsg4Range 
 * Inputs         : pucchNumRepCeMsg4
 * Outputs        : None
 * Returns        : COM_SUCCESS: If configuration is correct
 *                  COM_FAILURE: If configuration is incorrect
 * Description    : Validates the range for pucchNumRepCeMsg4
 ****************************************************************************/
ComReturnType
emtcValidatePucchNumRepCeMsg4Range
(
 UInt8  pucchNumRepCeMsg4
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(pucchNumRepCeMsg4 > EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_MAX)
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of pucchNumRepCeMsg4"
                "Max allowed is %d ",pucchNumRepCeMsg4,
                EMTC_PUCCH_NUM_REPETITION_CE_MSG4_L0_MAX);
        result = COM_FAILURE;
    }
    return result;
}
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
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(duplexingMode > EMTC_DUPLEXING_MODE_MAX)
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of duplexingMode"
                "Max allowed is %d ",duplexingMode,
                EMTC_DUPLEXING_MODE_MAX);
        result = COM_FAILURE;
    }
    return result;
}
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
)
{
    ComReturnType  result    = COM_SUCCESS;

    if(mcs > EMTC_MAX_MCS)
    {
        LTE_LOG(LOG_WARNING,"Invalid value %d of mcs"
                "Max allowed is %d ",mcs,
                EMTC_MAX_MCS);
        result = COM_FAILURE;
    }
    return result;
}
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
*   RETURNS      : COM_FAILURE : if SIs scheduling is not OK
*                  COM_SUCCESS  : if SIs scheduling is OK
*
******************************************************************************/
ComReturnType 
emtcValidateSiScheduling
(
  EmtcSchedulingInfoList_t *pSchedulingInf,
  UInt8                     siWindowLen
)
{
    UInt8                      siIdx               = COM_ZERO;
    UInt16                     startSf             = COM_ZERO;
    UInt16                     sf                  = COM_ZERO;
    /*Coverity ID 120843 Fix Start*/
    EmtcSiPeriodicityVal       siPeriodicitySf     = (EmtcSiPeriodicityVal)COM_ZERO;
    /*Coverity ID 120843 Fix End*/
    /*Coverity ID 120813 Fix Start*/
    EmtcSiWindowLen            siWindowLengthSf    = (EmtcSiWindowLen)COM_ZERO;
    /*Coverity ID 120813 Fix End*/
    ComReturnType                      result              = COM_SUCCESS;
    static UInt8               timeLine[EMTC_SF_UPPER_LIMIT];

    if( siWindowLen < EMTC_SI_WINDOW_LEN_NUM_VALUES)
    {
        siWindowLengthSf = EmtcSiWindowLenMsVal[siWindowLen];

        /* timeLine - initially free. 0 - free, 1 busy */
        COM_MEMSET(timeLine, COM_ZERO, sizeof(timeLine));

        if(pSchedulingInf->count < EMTC_MIN_SI_COUNT)
        {
            LTE_LOG(LOG_WARNING, "Scheduling information count is less than 1");
            return COM_FAILURE;
        }
        for (siIdx = COM_ZERO; (siIdx < pSchedulingInf->count) && (COM_SUCCESS == result); siIdx++)
        {
            /* The first sf, where this SI is scheduled */
            startSf = (UInt16)(siIdx * siWindowLengthSf);
            /* Convert si_periodicity to sf */
            siPeriodicitySf = EmtcSiPeriodicityRfVal[pSchedulingInf->siPeriodicity[siIdx]];
            /* Check possibility of: the SI-window starts ... in the radio frame for
             *  which SFN mod T = FLOOR(startSf/10), where T is the si-Periodicity */
            if (startSf >= siPeriodicitySf)
            {
                fprintf(stderr, "startSf [%d] is greater than Si periodicty [%d]",
                        startSf,siPeriodicitySf);
                result = COM_FAILURE;
                break;
            }

            /* Check that: SI-windows of different SI messages do not overlap. */
            /* And mark all sf where this SI is scheduled as busy */
            for (; (startSf < EMTC_SF_UPPER_LIMIT) && (COM_SUCCESS == result); startSf = (UInt16)(startSf +
                        siPeriodicitySf))
            {
                for (sf = startSf; (sf < startSf + siWindowLengthSf) && (sf <
                            EMTC_SF_UPPER_LIMIT); ++sf)
                {
                    if (timeLine[sf])
                    {
                        /* Already busy */
                        LTE_LOG(LOG_WARNING, "Collision at Sf[%d] for SI[%d]",
                                sf,siIdx);
                        result = COM_FAILURE;
                        break;
                    }
                    else
                    {
                        /* Mark sf as busy */
                        timeLine[sf] = COM_ONE;
                    }
                }
                COM_MEMSET(timeLine, COM_ZERO, sizeof(timeLine));
            }
        }
    }
    else
    {
        LTE_LOG(LOG_WARNING,"Invalid siWindow [%d]",siWindowLen);
        result = COM_FAILURE;
    }
   
    return result;
}
