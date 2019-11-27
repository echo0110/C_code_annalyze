/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteMacBCCH.h,v 1.1.1.1.6.1.4.2.2.2 2010/10/15 10:37:52 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : It contains macro declarations, function declarations 
 *                     sturucture declarations required by BCCH manager
 *             
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteMacBCCH.h,v $
 * Revision 1.1.1.1.6.1.4.2.2.2  2010/10/15 10:37:52  gur20491
 * Modified function signature for RRC_2_0_1 Compliance.
 *
 * Revision 1.1.1.1.6.1.4.2.2.1  2010/09/21 15:45:56  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.24.34.1  2010/07/21 09:08:13  gur24420
 * Aligned changes with respect to eNode Integration activity(based on RMI-NetLogic Code).
 * Overall Changes Cover:-
 * 1. MAX_MIB_MSG_LEN macro changed to 3 to avoid the Integration problems in MIB message coming from RRC.
 *
 * Revision 1.24  2009/07/26 08:10:22  gur15697
 * compilation warnings removed
 *
 * Revision 1.23  2009/07/13 11:58:25  gur15697
 * BCCH_MGR added
 *
 * Revision 1.22  2009/07/10 06:50:50  gur15697
 * MIB_LENGTH_FIELD_SIZE  added
 *
 * Revision 1.21  2009/06/25 07:13:33  gur15697
 * MAX_BCCH_MSG_WINDOW added
 *
 * Revision 1.20  2009/06/22 08:48:25  gur15697
 * review comments incorporated
 *
 * Revision 1.19  2009/06/20 12:36:46  gur15697
 * review comments incorporated
 *
 * Revision 1.18  2009/06/20 11:24:46  gur15697
 * review comments incorporated
 *
 * Revision 1.17  2009/06/19 06:18:10  gur11083
 * updated for cell config
 *
 * Revision 1.16  2009/06/19 05:27:10  gur15697
 * review comments incorporated
 *
 * Revision 1.15  2009/06/18 10:17:31  gur15697
 * Review comments incorporated
 *
 * Revision 1.14  2009/06/17 06:02:34  gur15697
 * allocateMIBResourcesInAdvance() removed
 *
 * Revision 1.13  2009/06/15 11:31:22  gur15697
 * UT bug fix
 *
 * Revision 1.12  2009/06/11 13:51:02  gur11083
 * compilation warning remived
 *
 * Revision 1.11  2009/06/11 11:40:49  gur15697
 * UT bug fixes
 *
 * Revision 1.10  2009/06/05 08:06:06  gur15697
 * init and cleanup functions added
 *
 * Revision 1.9  2009/06/03 12:00:44  gur15697
 * initBCCHMgr() added
 *
 * Revision 1.8  2009/05/29 10:19:57  gur18569
 * removed compilation error
 *
 * Revision 1.7  2009/05/26 10:21:21  gur15697
 * Function name receiveAndStoreBroadcastMsg() changed to parseAndStoreBroadcastMsg()
 *
 * Revision 1.6  2009/05/22 04:44:00  gur15697
 * lteMacParseUtil.h included
 *
 * Revision 1.5  2009/05/20 05:12:05  gur15697
 * comments incorporated
 *
 * Revision 1.4  2009/05/18 08:31:52  gur15697
 * Review comments incorporated
 *
 * Revision 1.3  2009/05/13 11:16:52  gur15697
 * Enum for validating SI-Window length and SI-Periodicity added
 *
 * Revision 1.2  2009/05/12 10:42:42  gur15697
 * Code added for BCCH
 *
 * Revision 1.1  2009/04/30 05:50:59  gur15697
 * Initial vertion
 *
 *
 *
 ****************************************************************************/

#ifndef LTE_MAC_BCCH_H
#define LTE_MAC_BCCH_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteMacTypes.h"
#include "lteMacPDCCHMgr.h"
#include "lteMacDLResourceAllocator.h"
#include "lteMacPCCH.h"
#include "lteMacParseUtil.h"
/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define BCCH_MGR "bcch_mgr" 

#define MAX_SIMAP_TABLE        2

 /* SIB8_Code Start */
#define MAX_NO_SI8_MSG_INDX 2 
#define INDEX_STATUS_FREE 0 
#define INDEX_STATUS_FILLED 1 
#define INDEX_STATUS_SCHEDULING 2 
#define SCHEDULING_REQD_FOR_SIB8_TAG_LEN 5
#define NUM_SI_REPETITIONS_TAG_LEN 6
/* SIB8_Code Start */

/*512 is the maximum periodicity of the SI messages*/
#define MAX_SIMAP_TABLE_NODES  512

/*SFN is of 10 bits (max SFN value is 1024). Every 40ms (for every 4 SFN) a 
 *new MIB should be scheduled. So there is a provision that RRC may send
 *1024/4 = 256 MIBs at a time. 
 */

#define MAX_MIB_MSG_LEN     3 
#define MAX_SI_REPETITIONS  1
 /* SPR 3067 Chg */
#define MIN_SI_REPETITIONS  1
 /* SPR 3067 Chg Ends */


/*Max TB size in bit for DCI 1A is 456 (57 bytes)*/
#define MAX_SIB1_MSG_SIZE_FOR_DCI_1A 57
/* SPR 8879 Fix */
#define MAX_SIB1_MSG_SIZE_FOR_DCI_1C 57
/* SPR 13250 fix start */
/*Max TB size in bit for DCI 1A is 2216 (277 bytes)*/
#define MAX_SI_MSG_SIZE_FOR_DCI_1A  277
/* SPR 13250 fix end */
/*Max TB size in bit for DCI 1C is 1736 (217 bytes)*/
#define MAX_SI_MSG_SIZE_FOR_DCI_1C  217 

/*As per the MAC API Reference Manual*/
#define MIN_SIBTYPE1_MSG_INFO_LEN 7
#define MIN_SI_MSG_INFO_LEN 15
#define MIN_SI_MSG_LEN 8
/* CMAS_MSG_COUNT will be assigned to CMAS message counter when second CMAS messages was processing */
#define CMAS_MSG_COUNT 2
#define RB_FOR_MIB 6
#define BCCH_TESTING 0

/*In API size field length in MAC_BCH_PDU_INFO TLV*/
#define MIB_LENGTH_FIELD_SIZE 2
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*Valid SI window length values*/
typedef enum SIWindowLenT
{
    SI_WINDOW_LEN_1  = 1,
    SI_WINDOW_LEN_2  = 2,
    SI_WINDOW_LEN_5  = 5,
    SI_WINDOW_LEN_10 = 10,
    SI_WINDOW_LEN_15 = 15,
    SI_WINDOW_LEN_20 = 20,
    SI_WINDOW_LEN_40 = 40
}SIWindowLen;

/* SIB8_Code_phase2 Start */
typedef enum SIB8InfoT
{
   NO_SIB8 = 0,
   SIB8_NEW_BUFFER = 1,
   SIB8_FLUSH_OLD_BUFFER_APPLY_NEW_BUFFER = 2,
   SIB8_NO_CHANGE = 3,
   SIB8_MAX_TAG = 4
}SIB8Info;
/* SIB8_Code_phase2 End */
/*Valid SI Periodicity values*/
typedef enum SIPeriodicityT
{
    SI_PERIODICITY_8   = 8,
    SI_PERIODICITY_16  = 16,
    SI_PERIODICITY_32  = 32,
    SI_PERIODICITY_64  = 64,
    SI_PERIODICITY_128 = 128,
    SI_PERIODICITY_256 = 256,
    SI_PERIODICITY_512 = 512,
    
    /*Add new entry above MAX_SI_PERIODICITY and 
     *set MAX_SI_PERIODICITY to max value of the 
     *above filelds
     */    
    MAX_SI_PERIODICITY = SI_PERIODICITY_512
}SIPeriodicity;

/* SIB8_Code Start */
typedef struct SIMsg8T
{
    DCIFormat dciFormat;
    UInt16  siMsgReptitions;  
    UInt16  msgLen[MAX_NO_SI_MSG_SGMNTS];
    UInt8  totalSiMsg8Sgmnts;
    UInt8  siMsg8SgmntToBeScheduled;
    UInt8  *msg_p[MAX_NO_SI_MSG_SGMNTS];    
    UInt8  numRB[MAX_NO_SI_MSG_SGMNTS];
    UInt8  mcsIndex[MAX_NO_SI_MSG_SGMNTS];
    UInt8  tpc[MAX_NO_SI_MSG_SGMNTS];
    /* siWindowSize used to store Window size of each SI Msg*/
    UInt8 siWindowSize;
    /* windowSizeStatus is used to store, at which current window SI Schedule*/
    UInt8 remaingWindowSize;
    /* Counter maintained for SI Transmissions */
    UInt8 transmissionCount;
    /* Counter maintained for Current SI Transmissions */
    UInt8 currentTransmissionCount;
    /* offsetValue used to store offsets at which SI to schedule */
    UInt8 offsetValue[8];
    UInt8 indexStatus;
}SIMsg8;
/* SIB8_Code End */

typedef struct SIMsgT
{
    UInt8  siMsgSgmnts;
    UInt8  splSIMsgflag;
    UInt8  schedulingCompleted;

    /* CMAS_CODE START */

    UInt8  nextCMASMessageToBeScheduled;

    /* Counter maintained for SI messages scheduling continously */
    UInt16  siMsgReptitions;  

    /* CMAS_CODE END */

    /* SPR_3898 changes start*/
    UInt8  siMsgSgmntToBeScheduled;
    /* SPR_3898 changes end*/

    /* This indicates type of format used for scheduling of SI mesage */
    DCIFormat dciFormat;
    UInt8  *msg_p[MAX_NO_SI_MSG_SGMNTS];    
    UInt16  msgLen[MAX_NO_SI_MSG_SGMNTS];
    UInt8  numRB[MAX_NO_SI_MSG_SGMNTS];
    /*added for UT defect*/   
    UInt8  mcsIndex[MAX_NO_SI_MSG_SGMNTS];
    UInt8  tpc[MAX_NO_SI_MSG_SGMNTS];
    /* SPR 3067 Chg */
    /* siWindowSize used to store Window size of each SI Msg*/
    UInt8 siWindowSize;
    /* windowSizeStatus is used to store, at which current window SI Schedule*/
    UInt8 remaingWindowSize;
    /* Counter maintained for SI Transmissions */
    UInt8 transmissionCount;
    /* Counter maintained for Current SI Transmissions */
    UInt8 currentTransmissionCount;
    /* offsetValue used to store offsets at which SI to schedule */
    /* offsetValue used to store offsets at which SI to schedule */
    UInt8 offsetValue[8];
    /* SPR 3067 Chg ends */

    /* SIB8_Code Start */
    UInt8  si8Flag;
    /* SIB8_Code End */

}SIMsg;

typedef struct SIMsgInfoT
{
    /*Contains the SI message information*/
    SIMsg siMsgBuff[MAX_SI_MSG/*32*/];

    /*Nummber of SI messages in siMsgBuff[] array*/
    UInt8 numSImsg;
}SIMsgInfo;

typedef struct SubFrameMapT
{
    /*If this flag is set schedule the SI message*/
    UInt8 sendSIMsgFlag;

    /* CMAS_CODE START */

    UInt8 lastCMASMessage;
    /* CMAS_CODE END */

    /*Type of SI message to be scheduled*/
    SIMsgType siMsgType;

}SubFrameMap;

typedef struct SIMapTableT
{
    /*This contains SI message scheduling info for 10 subframes*/
    SubFrameMap subFrame[MAX_SUB_FRAME];
}SIMapTable;

typedef struct SIInfoT
{
    /*siMsg[] array current index inuse*/
    UInt8       siMsgActiveIndex;

    /*siMapTable[] array current index inuse*/
    UInt8       siMapActiveIndex;

    /*siInfoUpdateFlag is set by RRC Interface when a new set of 
     *SI messagese received from RRC and new Map table created
     */
    UInt8       siInfoUpdateFlag;

    /*SFN from which new SI messages received is to be applied*/
    UInt16      sfn;
    
    SIMsgInfo   siMsg[2];
    SIMapTable  siMapTable[MAX_SIMAP_TABLE][MAX_SIMAP_TABLE_NODES];
}SIInfo;

/*This structure is used to temprarly store the decoded 
 *MIB messages when received from RRC
 */
typedef struct MIBMsgFromRRCT
{
    UInt32 sysFrameNum;
    UInt32 dataLen;
    /* MIB msg buffer ptr*/
    UInt8  *msg_p;
}MIBMsgFromRRC;
/* emtc changes -*/
/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/****************************************************************************
 * Exported Functions
 ****************************************************************************/
extern void initBCCHMgr(UInt8 numCells);

extern void cleanupBCCHMgr(InternalCellIndex internalCellIndex);

extern UInt32 scheduleBroadcastMsgs(PdcchDLOutputInfo  *pdcchDLOutputInfo_p,
                                    DciCCEContainerInfo *dciCCEContainerInfo_p,
                                    UInt32 *availableCommonCCE_p,
                                    UInt32 sysFrameNum,
                                    UInt32 subFrameNum,
                                    SInt32 *availableDLRBCount_p,
                                    InternalCellIndex internalCellIndex);

extern void scheduleMIB(UInt32 sysFrameNum,
                        /* +- SPR 17777 */
                        DciCCEContainerInfo *dciCCEContainerInfo_p
                        , SInt32 *availableDLRBCount_p
                        ,InternalCellIndex internalCellIndex
                        );

extern UInt32 scheduleSIBType1(PdcchDLOutputInfo  *pdcchDLOutputInfo_p,
                               UInt32 *availableCommonCCE_p,
                               UInt32 sysFrameNum,
                               /* +- SPR 17777 */
                               SInt32 *availableDLRBCount_p,
                               InternalCellIndex internalCellIndex);

extern UInt32 scheduleSI(PdcchDLOutputInfo  *pdcchDLOutputInfo_p,
                          UInt32 *availableCommonCCE_p,
                          UInt32 sysFrameNum,
                          UInt32 subFrameNum,
                          SInt32 *availableDLRBCount_p,
                          InternalCellIndex internalCellIndex);

/* _RED_VER_ */
extern RedundancyVerVal redundancyVerCalc(UInt32 inputValK);

extern UInt32 parseAndStoreBroadcastMsg(UInt8 **data_p,
/* SPR 609 changes start */   
                                       UInt16 *remLen,
/* SPR 609 changes end */   
                                       UInt32 cellConfigFlag,
                                       UInt32 sysFrameNum 
                                       ,InternalCellIndex internalCellIndex);

extern UInt32 decodeSIMsg(UInt8  **msg_p, 
                          UInt32 *remLen_p,
                          UInt32 cellCfgFlag,
                          UInt32 sysFrameNum,
                          InternalCellIndex internalCellIndex);

extern MacRetType validateStartSFNAndSF(SIMsgInfoFromRRC *siMsgInfoReceived_p,
                                        UInt8 siWindowSize,
                                        UInt8 orderOfSIMsg);

extern MacRetType createSIMapTable(SIMsgInfoFromRRC *siMsgInfoReceived_p,
                             UInt32 numSIMsg,
                             UInt32 siWindowSize,
                             UInt32 cellCfgFlagi,
                             InternalCellIndex internalCellIndex);

extern void cleanSIMsg(SIInfo *siMsgInfo_p, 
                       UInt32 siMsgActiveIndex,
                       UInt32 siMapActiveIndex);

/* SIB8_Code Start */
void cleanS8buffer(SIMsg8 *siMsg8buff_p);
/* SIB8_Code Start */
void hexDump(UInt8 *buff_p, UInt32 msgLen);
#endif  
