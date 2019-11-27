/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2ParseUtil.c,v 1.1.1.1.6.2.4.2.2.1 2010/09/21 15:53:33 gur20491 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file provide APIs to receive request from RRC Layer, 
 *                     configure MAC and send a response to RRC Layer.
 *                     Presently communication with RRC is socket based.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2ParseUtil.c,v $
 * Revision 1.1.1.1.6.2.4.2.2.1  2010/09/21 15:53:33  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.21  2009/11/12 13:34:04  gur19413
 * added UT_TESTING Flag
 *
 * Revision 1.20  2009/11/11 08:14:40  gur19413
 * added printf statment for initialize the mac emulator
 *
 * Revision 1.19  2009/10/21 07:03:11  gur11912
 * removed warnings
 *
 * Revision 1.18  2009/10/21 05:47:59  gur19413
 * bug fixed and optimized for UE addition
 *
 * Revision 1.17  2009/09/02 13:33:42  gur20439
 * printf put inside UT_TESTING flag
 *
 * Revision 1.16  2009/08/31 15:16:40  gur12140
 * Support Added For Rbs OverLap
 *
 * Revision 1.15  2009/08/21 07:17:12  gur20439
 * SIT bug fix.
 *
 * Revision 1.14  2009/08/20 07:32:48  gur19413
 * changed CQI handling
 *
 * Revision 1.13  2009/08/19 11:14:30  gur20470
 * modified validation of start & end RARnti range
 *
 * Revision 1.12  2009/07/29 15:18:52  gur20439
 * look and feel to simulator fix.
 *
 * Revision 1.11  2009/07/28 15:18:33  gur20439
 * delete ue from rrc simulator support.
 *
 * Revision 1.10  2009/07/27 14:58:03  gur20439
 * delete UE from rrc interface support.
 *
 * Revision 1.9  2009/07/27 08:50:48  gur20439
 * periodic cqi enhancement.
 *
 * Revision 1.8  2009/07/24 14:40:54  gur20439
 * support for aperiodic file read after cell config.
 *
 * Revision 1.7  2009/07/17 06:26:11  gur20439
 * changes related to rrc interface modification incorporated.
 *
 * Revision 1.6  2009/07/14 14:48:54  gur20439
 * rrc interface code change.
 *
 * Revision 1.5  2009/07/06 10:02:40  gur20439
 * global variable corrution fix.
 *
 * Revision 1.4  2009/06/19 12:54:39  gur18569
 * set the starting rnti range to 1
 *
 * Revision 1.3  2009/06/15 13:54:20  gur20439
 * modified for release 1.2.
 *
 * Revision 1.2  2009/04/03 13:41:38  gur20439
 * handling of moudule level log request from OAM.
 *
 * Revision 1.1  2009/03/30 10:20:41  gur19836
 * Initial Version
 *
 * Revision 1.7  2009/03/13 08:38:21  gur19836
 * Warnings removed
 *
 * Revision 1.6  2009/03/13 08:00:08  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.5  2009/02/25 11:04:07  gur10445
 * memset removed from readRRC Msg
 *
 * Revision 1.4  2009/01/27 08:50:09  gur19836
 * Change the prepareLcErrorResponse() for incorporating review comments
 *
 * Revision 1.3  2009/01/25 11:51:20  gur20052
 * Added during Regression Testing
 *
 * Revision 1.2  2009/01/21 13:58:58  gur19836
 * New global variable cellConfigured_g added
 *
 * Revision 1.1  2009/01/20 07:41:27  gur19836
 * Initial Version
 *
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2ParseUtil.h"
#include "lteLayer2OAM.h"
#include "lteLayer2MacPhyInterface.h"
#include "lteLayer2CommanTypes.h"
#include "phySimulatorApiHandler.h"
/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/
UInt8 cellConfigured_g[MAX_NUM_CELLS];
CellConfig cellConfigUeSim_g[MAX_NUM_CELLS] = {{0},{0}};
/* SPR 21660 changes start_Sahil */
/* SPR 21660 changes end_Sahil */
UInt16  startRARNTI_g;
/*LTE_REG_FIX*/
UInt16  endRARNTI_g[MAX_NUM_CELLS];
/*LTE_REG_FIX*/
UInt8 numCells_g;
/* SPR 12298 Fix Start */
/* SPR 14223 Fix : Start */
//UInt8 ueProfileIndex_g[MAX_NUM_CELLS] = {0,0};
/* SPR 14223 Fix : End */
/* SPR 12298 Fix Start */
/* + CA_PHASE2 */
UInt8 validScellDeactivationTimerValues_g[SCELL_DEACTIVATION_TIMER_VALUE_COUNT]=
{
	2,
	4,
	8,
	16,
	32,
	64,
	128
};
UInt8 validnumOfLayerValues_g[NUM_OF_LAYERS_COUNT]=
{
	1,
	2,
	4
};
/* - CA_PHASE2 */

#ifdef LTE_EMBMS_SUPPORTED
/* eMBMS Globals */
UInt8 eMBMSEnable_g[MAX_NUM_CELLS]={0,0};
#endif

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
void getLAndNValues(UInt16 ueIndex, UInt8 cell_index);
void parseMsg(UInt16 msgId, UInt8 *msg_p, UInt16 msgLen, UInt8 cellIndex);
void prepareBufferHeader(UInt8 *msgBuf, UInt16 msgId, UInt16 destModuleId, UInt8 cell_index);
/* + CA_AB */
UInt16 parseMACInitCellReq(UInt8 *msgBuf_p, UInt16 *msgLen_p, UInt8 cell_index);
UInt16 parseMACReconfigUEInfo(UInt8 *msg_p,
        ueContextForDownlink *dwlkUEInfo_p,
        ueContextForUplink *uplkUEInfo_p, 
        UInt16 msgLen, UInt16 *current_p,
		UInt8 cellIndex);

UInt16 parseMACCreateUEInfo( UInt8 *msg_p,
        ueContextForDownlink *dwlkUEInfo_p,
        ueContextForUplink *uplkUEInfo_p, UInt16 *current_p,
        UInt16 msgLen, UInt8 cellIndex);
/* - CA_AB */

/* new code added for cell config support */
MacRetType validateHarqParams(UInt8 maxHarqRetrans);
MacRetType  validatenumOfTxAntennas( UInt8 numOfTxAntennas);
MacRetType  validateStartRARntiRange( UInt8 startRARntiRange,UInt8 endRARntiRange );
MacRetType  validateEndRARntiRange( UInt8 endRARntiRange, UInt8 startRARntiRange );
MacRetType  validatePhichDuration( UInt8 phichDuration );
MacRetType  validatePhichResource( UInt8 phichResource );
static inline MacRetType validateCqiMode(UInt8 cqimode);
static inline MacRetType validateDlMaxRB(UInt8 MaxRB);
static inline MacRetType validateUlMaxRB(UInt8 MaxRB);
static inline MacRetType validateTransmissionMode(UInt8 transmissionMode);
static inline MacRetType validateNumOfLayer(UInt8 numOfLayer);
static inline MacRetType validatePrecodingIndex(UInt8 precodingIndex);
static inline MacRetType validateSRRequestType(UInt8 requestType);
static inline MacRetType validateSrPucchResourceIndex(UInt16 srPucchResourceIndex);
static inline MacRetType validateSrConfigurationIndex(UInt8 srConfigurationIndex);
void processCellConfig( ConfigCellReq *configCellReq_p);
UInt16 setConfigParams(InitReqParams *initReqParams_p);

static inline UInt16 parseReconfigUEInfo( ueContextForDownlink *dlUEInfo_p,
                                          UInt8 *msg_p,
                                          UInt16 tagLen,
                                          ueContextForUplink *ulUEInfo_p,
                                          UInt8 cellIndex
                                        );
/* + CL-MIMO RSM */
static inline MacRetType validateCBSR( codebookSubsetRestrictionInfo *cbsrInfo,
                                       UInt8 transmissionMode,
                                       UInt8 rankIndicator, UInt8 cellIndex);
/* - CL-MIMO RSM */
/* + DRX_CHG */
static inline MacRetType parseDRXConfigInfo( UInt8 **message_p,
                                       UInt16 tagLength,
                                       ueContextForDownlink *dlCreateReq_p
                                          ); 
/* - DRX_CHG */
/* + MEAS_GAP_CHG */
static inline MacRetType parseMeasGapConfigInfo( UInt8 **message_p,
                                                 UInt16 tagLength,
                                     ueContextForDownlink *dlCreateRequest_p
                                               );
/* - MEAS_GAP_CHG*/

/*CLPC_CHG*/
static MacRetType parseTpcPdcchConfig(UInt8 *message_p,
        UInt16 paramLength,
        UInt16 current,
        TpcPdcchConfig* tcpPdcchConf,
        UInt8 isTpcForPusch); /*0-Pucch 1-Pusch*/
static MacRetType parseDedicatedULPowerControlInfo(
        UInt8 *message_p,
        UInt16 paramLength,
        UInt16 current,
        DedicatedULPowerControlInfo* ulPowCtrlInfo);

/*CLPC_CHG*/

/* HO CHG */

UInt16 macEmuChangeCrntiReq(ChangeCrntiInfo *changedCrntiInfo_p, UInt8 oldCellIndex, UInt8 newCellIndex );
UInt16 validateUEIndex(UInt16 ueIndex);

extern UInt16 *crntiToIdxTbl_g;
extern UInt16 crntiRange_g;
extern ueContextForUplink* uplinkContextInfoMap_g[];

extern UInt8 validateCodingRate(UInt8 modScheme, UInt32 codingRate);

extern UInt8 rlfDebugFlag;
extern HarqIndQueueNodeInfo *harqIndQNode_gp[MAX_NUM_CELLS];
/* ULA_CHG */
UInt8 ueProfileNum[MAX_NUM_CELLS];
UInt8 srsProfileNum;
UInt8 srsSinrProfileNum;
UInt8 ulschProfileNum;
UInt8 ulschSinrProfileNum;
/* CLPC CHG */
UInt8 cqiProfileNum;
UInt8 cqiSinrProfileNum;
UInt8 phrProfileNum[MAX_NUM_CELLS] = {0,0};
/* CLPC_CHG */
UInt8 srsSinrValues_g[MAX_SRS_SINR_PROFILES][MAX_PRB] = {0, 0};
UInt8 ulschSinrValues_g[MAX_ULSCH_SINR_PROFILES] = {0};
/* CLPC_CHG */
UInt8 cqiSinrValues_g[MAX_CQI_SINR_PROFILES] = {0};
UInt8 profileValues_g[MAX_COUNT_VALUE] = {0};
extern TpcInfo tpcRntiInfoMap[TPC_RNTI_COUNT];
/* CLPC_CHG */
ueProfile ueProfileInfo_g[MAX_NUM_CELLS][MAX_UE_PROFILES];
SRSProfile srsProfileInfo_g[MAX_SRS_PROFILES];
ULSCHProfile ulschProfileInfo_g[MAX_ULSCH_PROFILES];
/* CLPC_CHG */
PHRProfile phrProfileInfo_g[MAX_NUM_CELLS][MAX_PHR_PROFILES];
CQIProfile cqiProfileInfo_g[MAX_CQI_PROFILES];
/* CLPC_CHG */
/* + SPR 11213 Changes */
STATIC inline updateUeCAUplinkContext(ueContextForUplink *ulUEInfo);
/* - SPR 11213 Changes */
/* ULA_CHG */
static inline MacRetType parseSpsConfigInfo(
                             UInt8 *message_p,
                             UInt16 tagLength,       
                             ueContextForDownlink *dlCreateRequest_p,
                             ueContextForUplink *ulCreateRequest_p);

//extern UInt32 stoppedProcessing_g;
/* + CA_AB */
/* Phase 2 Periodic CQi Changes Start*/
extern cqiPmiRiReportMap cqiPmiRiScellReportMap_g[MAX_NUM_SCELL][MAX_SFN_VALUE];
extern cqiPmiRiReportMap cqiPmiRiScellReportMap2_g[MAX_NUM_SCELL][MAX_SFN_VALUE];
/* Phase 2 Periodic CQi Changes End*/
extern void initializeMAC(UInt8 cell_index);
extern void macLayerCleanup(void);
static void setCellConfigValues(UInt8 cell_index);
extern void setActiveCell(UInt8 cellIndex);
/* - CA_AB */

//extern DLUEContext * ueDLContextArr_g[TBL_SIZE];


//Shashank
extern LTE_SQUEUE rrcMsgQ_g;
extern LTE_SQUEUE rlcRrcOamMsgQ_g;
extern LTE_SQUEUE pdcpRrcOamMsgQ_g;
extern LTE_SQUEUE macPhyDataQ_g[MAX_NUM_CELLS];
extern LTE_SQUEUE enodebRrcPhyMsgQ_g[MAX_NUM_CELLS];
extern SockAddr phyLayerAddr_g[MAX_NUM_CELLS];
extern UInt16 rxPorteNBMac_g[MAX_NUM_CELLS];
extern SInt32 sockfd_g[MAX_NUM_CELLS];
extern SInt32 txRRCSockFD_g;

extern volatile UInt16 cellId_g[MAX_NUM_CELLS];

/* + CQI_4.1 */
cqiUEProfileValues cqiUEProfileValues_g[MAX_CQI_PROFILE];
/* Aperiodic CQI Changes Start */
cqiPeriodicReportMode10 cqiPeriodicReportMode10_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiPeriodicReportMode11 cqiPeriodicReportMode11_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiPeriodicReportMode20 cqiPeriodicReportMode20_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiPeriodicReportMode21 cqiPeriodicReportMode21_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode12 cqiAperiodicReportMode12_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode20 cqiAperiodicReportMode20_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode22 cqiAperiodicReportMode22_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode30 cqiAperiodicReportMode30_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode31 cqiAperiodicReportMode31_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
posOfMSelSBForAperiodicMode20 posOfMSelSBForAperiodicMode20_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
posOfMSelSBForAperiodicMode22RIOne posOfMSelSBForAperiodicMode22RIOne_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
posOfMSelSBForAperiodicMode22RITwo posOfMSelSBForAperiodicMode22RITwo_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
/* Aperiodic CQI Changes End */

/* EICIC +*/
cqiPeriodicReportMode10 cqiPeriodicReportMode10_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiPeriodicReportMode11 cqiPeriodicReportMode11_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiPeriodicReportMode20 cqiPeriodicReportMode20_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiPeriodicReportMode21 cqiPeriodicReportMode21_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode12 cqiAperiodicReportMode12_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode20 cqiAperiodicReportMode20_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode22 cqiAperiodicReportMode22_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode30 cqiAperiodicReportMode30_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
cqiAperiodicReportMode31 cqiAperiodicReportMode31_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
posOfMSelSBForAperiodicMode20 posOfMSelSBForAperiodicMode20_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
posOfMSelSBForAperiodicMode22RIOne posOfMSelSBForAperiodicMode22RIOne_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
posOfMSelSBForAperiodicMode22RITwo posOfMSelSBForAperiodicMode22RITwo_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
/* EICIC -*/

UInt32 nCm_g[MAX_UE_SELECTED_N][MAX_UE_SELECTED_SUBBAND] = {{0},{0}};

/* Refer 36213-860 Table 7.2.2-1A
   Mapping of cqi-pmi-ConfigIndex to NP and NOFFSET,CQI for FDD.
   cqi-pmi-ConfigIndex = 317  Reserved
   cqi-pmi-ConfigIndex = 542- 1023   Reserved */
#ifdef FDD_CONFIG
static CqiPmiConfig cqiPmiConfigTable_g [] =
{  /* CQI PMI Index   Value of Np    Value of Noffset cqi
      Start  End Index  Periodicity      Offset CQI   */
    {  0  ,    1  ,       2  ,              0   },
    {  2  ,    6  ,       5  ,             -2   },
    {  7  ,    16 ,       10 ,             -7   },
    {  17 ,    36 ,       20 ,             -17  },
    {  37 ,    76 ,       40 ,             -37  },
    {  77 ,    156,       80 ,             -77  },
    {  157,    316,       160,             -157 },
    {  318,    349,       32 ,             -318 },
    {  350,    413,       64 ,             -350 },
    {  414,    541,       128,             -414 }
};

/*TDD SB_CQI*/
#else

/* Refer 36213-860 Table 7.2.2-1C
   Mapping of cqi-pmi-ConfigIndex to NP and NOFFSET,CQI for TDD.
   cqi-pmi-ConfigIndex = 317  Reserved 
   cqi-pmi-ConfigIndex = 542- 1023   Reserved */
static CqiPmiConfig cqiPmiConfigTable_g [] =
{  /* CQI PMI Index   Value of Np    Value of Noffset cqi
      Start  End Index  Periodicity      Offset CQI   */
    {  0 ,   0 ,         1,               0   },
    {  1 ,   5 ,         5,              -1   },
    {  6,    15,        10,              -6   },
    {  16 ,  35,        20,              -16  },
    {  36,   75,        40,              -36  },          
    {  76 ,  155,       80,              -76  },
    {  156 , 315,       160,             -156 }
};

#endif
/*TDD SB_CQI*/
/* Refer specs 36214-860 Table 7.2.2-1B
   Mapping of ri-ConfigIndex to MRI and NOFFSET,RI.
   ri-ConfigIndex = 966 - 1023   Reserved  */
static RiConfig riConfigTable_g [] =
{
  /*  ri-ConfigIndex   Value of MRI    Value of NOFFSET,RI
      start   end                                            */
    { 0  ,    160,        1 ,               0   },
    { 161,    321,        2 ,              -161 },
    { 322,    482,        4 ,              -322 },
    { 483,    643,        8 ,              -483 },
    { 644,    804,        16,              -644 },
    { 805,    965,        32,              -805 }
};
#define MAX_UE_SELECTED_SUBBAND 7
#define MAX_UE_SELECTED_N 26
/* - CQI_4.1 */

/* + CQI_5.0 */
/* + SPR 13126 Changes */
UInt8 RIValues_g[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE];
/* - SPR 13126 Changes */
/* - CQI_5.0 */

/* EICIC +*/
UInt8 RIValues_g_2[MAX_NUM_CELLS][MAX_CQI_VALUES_PER_PROFILE]; /* EICIC +-*/
/* EICIC -*/

/* + E_CID_5.3 */
Rel9TAValues rel9TaValues_g[MAX_NUM_CELLS];
/* - E_CID_5.3 */

/* + TM7_8 Changes Start */
SInt32  angleOfArrivalValues_g[MAX_NUM_CELLS][MAX_NUM_ANTENNA][MAX_SF_VALUE]= {0};
/* - TM7_8 Changes End */

/* SPR 5313 changes start */
#define BUFFERSIZE 9500
extern UInt8 recvDataBuffer_g[BUFFERSIZE];
/*START: HO Fix*/
extern SockAddr phyLayerAddrHO_g[MAX_NUM_CELLS];
extern SInt32 sockfdHO_g[MAX_NUM_CELLS];
/*END: HO Fix*/
/* SPR 10868 Changes Start */
extern UInt16 rxPorteNBMacForHO_g[MAX_NUM_CELLS];
/* SPR 10868 Changes End */

/****************************************************************************
 * Function Name  : setSocketBlockingEnabled
 * Inputs         : socketfiledecriptor, blocking (0 - Non Blocking, 1-Blocking)
 * Outputs        : NONE
 * Returns        : status (0-Failure,1-Successful)
 * Variables      :
 * Description    : to set the specific socket blocking or non blocking
 ****************************************************************************/
static UInt8 setSocketBlockingEnabled(UInt8 sockfd, UInt8 blocking)
{
    SInt32 fileflags;
    fileflags = fcntl(sockfd, F_GETFL,0);
    if(fileflags == -1)
    {
        ltePanic("fcntl F_GETFL");
        return 0;
    }
    fileflags = blocking ? (fileflags & ~FNDELAY) : (fileflags |FNDELAY);
    return (fcntl(sockfd, F_SETFL, fileflags)== 0) ? 1 : 0 ;
}

/****************************************************************************
 * Function Name  : initSocketAddress
 * Inputs         : sockaddr_in, socketFileDescriptor,Port Number
 * Outputs        :
 * Returns        : status (0-Failure,1-Successful)
 * Variables      :
 * Description    : to intialize the Socket address with specific socket file
 *                  descriptor and specific port
 ****************************************************************************/
static UInt8 initSocketAddress(struct sockaddr_in *s,SInt32 *sockfd,UInt16 port)
{
    struct hostent *host_p;
    host_p= (struct hostent *) gethostbyname((char *)l2ModuleIP);
    if((*sockfd = socket(AF_INET, SOCK_DGRAM, 0)) == -1)
    {
        LTE_LOG(LOG_DETAIL,PNULL,"creating socket error\n");
        return 0;
    }
    (*s).sin_family = AF_INET;
    (*s).sin_port = htons(port);
    (*s).sin_addr = *((struct in_addr *)host_p->h_addr);
    bzero(&((*s).sin_zero),8);
    return 1;
}
/* SPR 5313 changes end */

/****************************************************************************
 * Function Name  : parseMACInitLayerReq
 * Inputs         : msgBuf_p - Pointer to msg read from socket
 *                  msgLen_p - Pointer tot he msg length of msg recv
 * Outputs        : None
 * Returns        : MAC_SUCCESS or FAILURE
 * Variables      :
 * Description    : This API parse the init MAC Layer request and initalizes
 *                  MAC Layer after validating the request.
 ********************* ********************************************************/
/* + CA_AB */
UInt16 parseMACInitLayerReq(UInt8 *msgBuf_p, UInt16 *msgLen_p, UInt8 cell_index)
{
    if (0 == stoppedProcessing_g[cell_index] )
    {
        MAC_EMULATOR_LOG(LOG_INFO,"MAC is already initialized !!\n");
        return MAC_LAYER_ALREADY_INITIALIZED;
    
    }
    if(*msgLen_p != (MAC_API_HEADER_LEN + MAC_INIT_REQ_MANDATORY_LENGTH))
    {
        return MAC_SYNTAX_ERROR;
    }

    InitReqParams initReqParams = {0};
    UInt16 current = MAC_API_HEADER_LEN;
    UInt16 numOfUe = 0, rntiStartRange = 0, rntiEndRange = 0;
    UInt16 resp = MAC_SUCCESS;
    UInt8 index = 0;

    numOfUe = LTE_GET_U16BIT(msgBuf_p+current);
    current += 2;

    rntiStartRange = LTE_GET_U16BIT(msgBuf_p+current);
    current += 2;
    
    rntiEndRange = LTE_GET_U16BIT(msgBuf_p+current);
    current += 2;
    
    if((rntiStartRange < 0x01) || (rntiStartRange > 0xFFF2) ||
       (rntiEndRange < 0x01) || (rntiEndRange > 0xFFF2) ||
       (rntiEndRange <= rntiStartRange)) {
        return MAC_SYNTAX_ERROR;
    }    

    initReqParams.dlUeQueueLoadWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlLcQueueLoadWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlUeNackQueueLoadWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlMsWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlMs64QAMFactorWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlMs16QAMFactorWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlMsQPSKFactorWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlUePriorityWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlLcPriorityWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlUeDelayWt = *(msgBuf_p + current);
    current++;
    initReqParams.dlLcDelayWt = *(msgBuf_p + current);
    current++;
    initReqParams.frequencySelectiveSchedulingEnabled = *(msgBuf_p + current); 
    current++;
    initReqParams.ulMsWt = *(msgBuf_p + current);
    current++;
    initReqParams.ulMs64QAMFactorWt = *(msgBuf_p + current);
    current++;
    initReqParams.ulMs16QAMFactorWt = *(msgBuf_p + current);
    current++;
    initReqParams.ulMsQPSKFactorWt = *(msgBuf_p + current);
    current++;
    initReqParams.ulUePriorityWt = *(msgBuf_p + current);
    current++;
    initReqParams.ulUeDelayWt = *(msgBuf_p + current);
    current++;
    initReqParams.ulUeBsrLoadWt = *(msgBuf_p + current);
    current++;
    initReqParams.ulLcgPriorityWt = *(msgBuf_p + current);
    current++;
    initReqParams.logLevel = *(msgBuf_p + current);
    current++;
    initReqParams.dlSchedulerStrategy = *(msgBuf_p + current);
    current++;
    initReqParams.ulSchedulerStrategy = *(msgBuf_p + current);
    current++;
    initReqParams.maxUEsToBeScheduledDL = *(msgBuf_p + current);
    current++;
    initReqParams.maxUEsToBeScheduledUL = *(msgBuf_p + current);
    current++;
    initReqParams.startSpsCrnti= LTE_GET_U16BIT(msgBuf_p+current);
    current += 2;
    initReqParams.endSpsCrnti= LTE_GET_U16BIT(msgBuf_p+current);
    current += 2;

    for(index = 0; index < MAX_QCI; index++)
    {
        initReqParams.qciSpslist[index] = *(msgBuf_p + current);
        current++;
    }

	/* UE_TESTING need to uncomment*/
    initReqParams.isCASupported = *(msgBuf_p + current);
    current++;
    initReqParams.maxNumCells = *(msgBuf_p + current);
    /*LTE_REG_FIX*/
    current++;
    cellType_g[cell_index].cellType = *(msgBuf_p + current);   
    /*LTE_REG_FIX*/
    numCells_g = initReqParams.maxNumCells ;

#ifdef LTE_EMBMS_SUPPORTED
    current++;
    initReqParams.eMBMSEnable = *(msgBuf_p + current);
    eMBMSEnable_g[cell_index]=initReqParams.eMBMSEnable;
#endif
		    
			  cellConfigUeSim_g[cell_index].lowerCRNTIBound = rntiStartRange ;
			  cellConfigUeSim_g[cell_index].upperCRNTIBound = rntiEndRange ;
#ifdef UT_TESTING
    MAC_EMULATOR_LOG(LOG_INFO,"rntiStartRange = %d rntiEndRange = %d CellIndex %d\n",rntiStartRange, rntiEndRange, cell_index);
  #endif
    /* Enabling Logs by default */
    enableLog_g = 1;
    /* set the cell config structure with default value */
    cellConfigUeSim_g[cell_index].startSpsCrnti = initReqParams.startSpsCrnti;
    cellConfigUeSim_g[cell_index].endSpsCrnti = initReqParams.endSpsCrnti;
    for(index = 0; index < MAX_QCI; index++)
    {
         cellConfigUeSim_g[cell_index].ueSpsSysWideParam.qciSpslist[index] = 
												initReqParams.qciSpslist[index];
    }

    setCellConfigValues(cell_index); 
   
    initializeMAC(cell_index);
    
    stoppedProcessing_g[cell_index] = 0;
    
    

    MAC_EMULATOR_LOG(LOG_INFO,"\n########## Initialized MAC Emulator Successfully ###########\n\n"); 
    return resp;
}    
/* - CA_AB */
/****************************************************************************
 * Function Name  : sendMsgToOAM
 * Inputs         : cnfBuff_p - Pointer to buffer to be send to OAM
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API sends the confirmation message to OAM. 
 ****************************************************************************/

void sendMsgToOAM(UInt8 *cnfBuff_p, UInt16 msgLen)
{
    /* Fill msgLen Now */
    /* + CA_AB */
	/* why 2nd time length is filled */
	/* need to uncomment */
    //LTE_SET_U16BIT(cnfBuff_p + MAC_API_HEADER_LEN -2 -CELL_INDEX_LENGTH, msgLen);
	/* UE_TESTING */
    LTE_SET_U16BIT(cnfBuff_p + MAC_API_HEADER_LEN -2, msgLen);
    /* - CA_AB */

    sendto(txOAMSockFD_g, cnfBuff_p, msgLen, 0,
            (const SockAddr *)&txOAM_addr_g, sizeof (txOAM_addr_g));
    LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g,
          "Function %s Exit\n", __func__);
}


/****************************************************************************
 * Function Name  : parseOAMMsg
 * Inputs         : msgBuf_p - Pointer to the buffer read from socket.
 *                  msgId - Message Id received in the buffer header.
 *                  msgLen_p - Pointer to the message length received.
 *                  transactionId - transactionId of the request received.
 * Outputs        : None
 * Returns        : SUCCESS or FAILURE
 * Variables      :
 * Description    : This API parse the OAm message received on socket and send 
 *                  confirmation if required.
 ****************************************************************************/

/* + CA_AB */
UInt8 parseOAMMsg(UInt8 *msgBuf_p, UInt16 msgId, UInt16 *msgLen_p, 
                  UInt16 transactionId, UInt8 cellIndex)
{
    UInt16 resp = MAC_SUCCESS;
    UInt8 cnfBuff[MAX_DATA_SIZE] = {0};
    UInt16 current = 0;
    //UInt8 logLevel = 0, enableLogging = 0, statsEnable = 0, macLogLevel = 0;

    switch(msgId) {
        case MAC_INIT_LAYER_REQ:
            if(1 == stoppedProcessing_g[cellIndex]) 
			{
				resp = parseMACInitLayerReq(msgBuf_p, msgLen_p , cellIndex);
			} else {
				resp = MAC_LAYER_ALREADY_INITIALIZED;
			}    
			/*Prepare a response and send it to OAM*/
			/*set the transaction id */
            LTE_SET_U16BIT(cnfBuff, transactionId);
            prepareBufferHeader(cnfBuff, MAC_INIT_LAYER_CNF, OAM_MODULE_ID, cellIndex); 
            current += (MAC_API_HEADER_LEN - CELL_INDEX_LENGTH -2); 
            cnfBuff[current] = cellIndex ;
            current++; 
            LTE_SET_U16BIT(cnfBuff + current, resp);
            current += 2;
            /* Fill Msg Len in the API header */
            LTE_SET_U16BIT(cnfBuff + MAC_API_HEADER_LEN -7 - CELL_INDEX_LENGTH, current);

            sendMsgToOAM(cnfBuff, current);
            break;
        case MAC_CLEANUP_LAYER_REQ:
            if(0 == stoppedProcessing_g[cellIndex]) {
                macLayerCleanup();
                //cleanupAllMemoryToPool();
            } else {
                lteWarning("MAC Layer not initialized yet\n");
                resp = MAC_LAYER_NOT_INITIALIZED;
            }    
            /*Prepare a response and send it to OAM*/
            /*set the transaction id */
            LTE_SET_U16BIT(cnfBuff, transactionId);
            prepareBufferHeader(cnfBuff, MAC_CLEANUP_LAYER_CNF, OAM_MODULE_ID, cellIndex); 
            current += MAC_API_HEADER_LEN;
            LTE_SET_U16BIT(cnfBuff + current, resp);
            current += 2;
            /* Fill Msg Len in the API header */
            LTE_SET_U16BIT(cnfBuff + MAC_API_HEADER_LEN - 2, resp);
            /*Send confirmation to OAM */
            sendMsgToOAM(cnfBuff, current);
            break;
	   case MAC_INIT_CELL_REQ:
            if((1 == stoppedProcessing_g[cellIndex]) && numCells_g > 1) 
            {
                resp = parseMACInitCellReq(msgBuf_p, msgLen_p , cellIndex);
            }
            else 
            {
                resp = MAC_LAYER_ALREADY_INITIALIZED;
            }    

            /*Prepare a response and send it to OAM*/
            /*set the transaction id */
			LTE_SET_U16BIT(cnfBuff, transactionId);
			prepareBufferHeader(cnfBuff, MAC_INIT_CELL_CNF, OAM_MODULE_ID, cellIndex); 
          current += (MAC_API_HEADER_LEN - CELL_INDEX_LENGTH -2); 
			 cnfBuff[current] = cellIndex ;
			current++; 
            LTE_SET_U16BIT(cnfBuff + current, resp);
            current += 2;
            /* Fill Msg Len in the API header */
             LTE_SET_U16BIT(cnfBuff + MAC_API_HEADER_LEN -7 - CELL_INDEX_LENGTH, current);

            sendMsgToOAM(cnfBuff, current);
            break;
          default :
              MAC_EMULATOR_LOG(LOG_INFO,"unsupported api %d received from OAM \n", msgId);
              break;
    }
    return MAC_SUCCESS;
}
/* - CA_AB */
/****************************************************************************
 * Function Name  : readRRCMsgFromSocket
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API receives data on socket, parse the msgId and call
 *                  the corresponding API to deal with the message.
 ****************************************************************************/

void readRRCMsgFromSocket(void)
{
  //UInt8 msgBuf[MAX_DATA_SIZE];
  UInt8 *msgBuf = PNULL;
  UInt16 msgId = 0;
  UInt16 resp = MAC_SUCCESS;
  SockAddr rrc_addr;
  //SInt32 bytesRead = 0;
  //UInt32 addr_len = sizeof(rrc_addr);
  memSet(&rrc_addr, 0x0, sizeof(rrc_addr));
  UInt16 srcModuleId = 0;
  UInt16 destModuleId = 0;
  UInt16 msgLen = 0;
  /* + CA_AB */
  UInt8 cellIndex = 0;
  /* - CA_AB */
  UInt16 current = 0;
  UInt16 transactionId = 0;
  /* SPR 13294 Fix Start */
  UInt32 numPdu      = 0;
  /* SPR 13294 Fix End */
  rrcOamMsgNode *rrcOamMsg_p = NULL;
  ChangeCrntiRespInfo changeCrntiResp = {0};
  ChangeCrntiInfo   changeCrntiInfo = {0};
  numPdu = sQueueCount(&rrcMsgQ_g);
  while (numPdu--)
  {
	  rrcOamMsg_p = (rrcOamMsgNode*) popNode (&rrcMsgQ_g);
	  //	  if(bytesRead >= MAC_API_HEADER_LEN) 
          if (NULL != rrcOamMsg_p)
	  {
		  /* Parse the buffer header */
		  if (NULL == (msgBuf = rrcOamMsg_p->msg))
		  {
			  freeMemPool (rrcOamMsg_p);
			  rrcOamMsg_p = NULL;
			  continue;
		  }
    transactionId = LTE_GET_U16BIT(msgBuf);
    current += 2;

    srcModuleId = LTE_GET_U16BIT(msgBuf+current);
    current += 2;

    destModuleId = LTE_GET_U16BIT(msgBuf+current);
    current += 2;
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"SRC MOD ID = %d, DEST MOD ID = %d\n", 
        srcModuleId, destModuleId);
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"transactionId received in request = %d\n",
        transactionId);

    msgId = LTE_GET_U16BIT(msgBuf+current);
    current += 2;

    msgLen  = LTE_GET_U16BIT(msgBuf+current);
    current += 7;



    /* + CA_AB */
    cellIndex = *(msgBuf+current);
    current++;;
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g,"msgId = %d, msgLen = %d, cellIndex = %d\n", msgId, msgLen, cellIndex);
    /* - CA_AB */
    /* Parse Message received from OAM */
    if(OAM_MODULE_ID == srcModuleId) {
      /* + CA_AB */
      parseOAMMsg(msgBuf, msgId, &msgLen, transactionId, cellIndex);
      /* - CA_AB */
      // Free memory
		  if(msgBuf)
		  {
			  freeMemPool (msgBuf);
                          msgBuf = NULL;

		  }
		  if(rrcOamMsg_p)
		  {
			  freeMemPool(rrcOamMsg_p);
			  rrcOamMsg_p = NULL;
		  }

      return;
    }
	msgLen -= MAC_API_HEADER_LEN;
    if(1 == stoppedProcessing_g[cellIndex]) {
      lteWarning("MAc Layer not initialized yet\n");
      // Free memory
		  if(msgBuf)
		  {
			  freeMemPool (msgBuf);
                          msgBuf = NULL;

		  }
		  if(rrcOamMsg_p)
		  {
			  freeMemPool(rrcOamMsg_p);
			  rrcOamMsg_p = NULL;
		  }

      return;
    }
    switch(msgId) {
      case MAC_CREATE_UE_ENTITY_REQ:
      case MAC_DELETE_UE_ENTITY_REQ:
      case MAC_RECONFIGURE_UE_ENTITY_REQ:
         /*SPR 21660 changes end_Kishore*/    
        parseMsg(msgId, msgBuf, msgLen, cellIndex);
        break;
        /* RRC_CONNECTION_REQ */
      case RRC_CONNECTION_REQ:
        MAC_EMULATOR_LOG(LOG_INFO,"RRC_CONNECTION_REQ received........\n");
		/* + CA_AB */
		/* UE_TESTING */
		/* need to change */
        /*parseRRCConnectionReqMsg(msgBuf + MAC_API_HEADER_LEN +2 -1, msgLen-2,cellIndex);*/
        parseRRCConnectionReqMsg(msgBuf + MAC_API_HEADER_LEN, msgLen,cellIndex);
		/* UE_TESTING */
		/* - CA_AB */
        break;
      /* HO Code changes */
      case MAC_EMU_RESET_REQ:
        MAC_EMULATOR_LOG(LOG_INFO,"MAC_EMU_RESET_REQ received........\n");
        handleResetReq(msgBuf + MAC_API_HEADER_LEN,transactionId, cellIndex);
        break;
      case MAC_EMU_CHANGE_CRNTI_REQ:
        fprintf(stderr,"MAC_EMU_CHANGE_CRNTI_REQ received........\n");
	if (MAC_EMU_CHANGE_CRNTI_REQ_LEN == msgLen)
    {
        /*Start: Fix for SPR 1158*/ 
        UInt8 tempCurrent = current - 2;
        UInt8 oldCellIndex =  *((UInt8*)(msgBuf + tempCurrent));
        tempCurrent++; 
        UInt8 newCellIndex =  *((UInt8*)(msgBuf + tempCurrent));
        /*End: Fix for SPR 1158*/ 
   
        changeCrntiResp.ueIndex = LTE_GET_U16BIT(msgBuf + current);
        current += 2;
        if (MAC_SUCCESS == validateUEIndex(changeCrntiResp.ueIndex))
        {
            changeCrntiInfo.oldCrnti = LTE_GET_U16BIT(msgBuf + current);
            current += 2;
            changeCrntiInfo.newCrnti = LTE_GET_U16BIT(msgBuf + current);
            current += 2;
            changeCrntiInfo.ueIndex = changeCrntiResp.ueIndex;
            /*Start: Fix for SPR 1158*/ 
            fprintf(stderr," ueIndex = %d oldcnrti = %d newcrnti = %d old cell index =%d, new cell Index= %d\n",changeCrntiResp.ueIndex, changeCrntiInfo.oldCrnti, changeCrntiInfo.newCrnti,oldCellIndex, newCellIndex);
            resp = macEmuChangeCrntiReq(&changeCrntiInfo, oldCellIndex, newCellIndex); 
            /*End: Fix for SPR 1158*/ 
        }   
        else
        {
            resp = MAC_INVALID_UE_ID;
        }
    }       
        else
        {
            resp = MAC_SYNTAX_ERROR;
        }
        changeCrntiResp.resp = resp;
        changeCrntiResp.transactionId = transactionId;
        /* Send Msg to RRC */
        sendMsgToRRC(MAC_EMU_CHANGE_CRNTI_CNF, &changeCrntiResp, PNULL, PNULL, cellIndex);
        break;
      case MAC_EMU_HO_BASED_RACH_REQ:
        fprintf(stderr,"MAC_EMU_HO_BASED_RACH_REQ received from CellIndex [%d]........\n", cellIndex);
        parseAndtriggerHoRachReq(msgBuf + MAC_API_HEADER_LEN,transactionId, cellIndex);
        break;
      /* HO code support */
      default:
        MAC_EMULATOR_LOG(LOG_INFO,"wrong API received from cellIndex[%d]........\n", cellIndex);
        lteWarning("Incorrect msgId received, dropping message Msg Id [%d] from cellIndex [%d]\n", msgId, cellIndex);
        break;
    }
		  // Free memory
#if 0    
		  if(msgBuf)
		  {
			  freeMemPool (msgBuf);
                          msgBuf = NULL;

		  }
#endif          

          /* msgBuf is getting corrupted, free rrcOamMsg_p->msg directly */
		  if(rrcOamMsg_p->msg)
		  {
			  freeMemPool (rrcOamMsg_p->msg);
              rrcOamMsg_p->msg = NULL;

		  }
		  if(rrcOamMsg_p)
		  {
			  freeMemPool(rrcOamMsg_p);
			  rrcOamMsg_p = NULL;
		  }
	  }
  }
}
/****************************************************************************
 * Function Name  : validateRBs
 * Inputs         : configCellReq_p->maxHarqRetrans -value that specifies
 *		    maximim harq retransmissions to be done.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This function validates resource blocks and allocation type 
 *                  and return response of validation to the calling function.
 ****************************************************************************/
 
MacRetType validateRBs(ConfigCellReq *configCellReq_p)
{
    if((configCellReq_p->dlResBlocks < MAC_MIN_RESOURCE_BLOCKS) ||
       (configCellReq_p->dlResBlocks > MAC_MAX_RESOURCE_BLOCKS) )
       /*(configCellReq_p->dlStartRange < MAC_MIN_RANGE_VALUE) ||
       (configCellReq_p->dlStartRange > MAC_MAX_RANGE_VALUE) ||
       (configCellReq_p->dlEndRange > MAC_MAX_RANGE_VALUE) ||
       (configCellReq_p->dlEndRange < MAC_MIN_RANGE_VALUE) ||
       (configCellReq_p->dlEndRange < configCellReq_p->dlStartRange) ||
       ((configCellReq_p->dlEndRange - configCellReq_p->dlStartRange + 1)
                                          != configCellReq_p->dlResBlocks)*/
    {
        lteWarning("INVALID CONFIG DL RBS Parameters ");
        return MAC_SYNTAX_ERROR;
    }
    else if((configCellReq_p->ulResBlocks < MAC_MIN_RESOURCE_BLOCKS) ||
       (configCellReq_p->ulResBlocks > MAC_MAX_RESOURCE_BLOCKS) )
   /*
       (configCellReq_p->ulStartRange < MAC_MIN_RANGE_VALUE) ||
       (configCellReq_p->ulStartRange > MAC_MAX_RANGE_VALUE) ||
       (configCellReq_p->ulEndRange > MAC_MAX_RANGE_VALUE) ||
       (configCellReq_p->ulEndRange < MAC_MIN_RANGE_VALUE) ||
       (configCellReq_p->ulEndRange < configCellReq_p->ulStartRange) ||
       ((configCellReq_p->ulEndRange - configCellReq_p->ulStartRange + 1)
                                              != configCellReq_p->ulResBlocks)*/
    {
        lteWarning("INVALID CONFIG UL RBS Parameters ");
        return MAC_SYNTAX_ERROR;
    }

    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validateHarqParams
 * Inputs         : maxHarqRetrans - Max HARQ Retransmissions
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This function validates harq profile parameters and return the 
 *                  result of validation to the calling function.
 ****************************************************************************/
 
MacRetType validateHarqParams(UInt8 maxHarqRetrans)
{
    if((maxHarqRetrans < MAC_MIN_HARQ_RETRANSMISSIONS)
        ||(maxHarqRetrans > MAC_MAX_HARQ_RETRANSMISSIONS))
    {
        return MAC_FAILURE;
    }    

    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  :validatenumOfTxAntennas 
 * Inputs         : numOfTxAntennas - Number of Transmission Antennas 
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This function validates Number of Transmission Antennas
 *		    parameters and return the result of validation to the 
 *		    calling function.
 ****************************************************************************/


MacRetType  validatenumOfTxAntennas( UInt8 numOfTxAntennas)
{
	if(numOfTxAntennas != 1 && numOfTxAntennas != 2){
		 return MAC_FAILURE;
	}
	return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validateStartRARntiRange
 * Inputs         : startRARntiRange_g - Start RARNTI range
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This function validates start RARNTI range
 *                  parameters and return the result of validation to the
 *                  calling function.
 ****************************************************************************/


MacRetType  validateStartRARntiRange( UInt8 startRARntiRange_g,UInt8 endRARntiRange_g )
{
    if((startRARntiRange_g < cellConfigUeSim_g[MAIN_CELL].lowerCRNTIBound ) 
            || (startRARntiRange_g >=
                cellConfigUeSim_g[numCells_g - 1].upperCRNTIBound) 
            || (startRARntiRange_g >= endRARntiRange_g))
    {
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validatePhichDuration 
 * Inputs         : phichDuration - extended  or normal cyclic prefix.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This function validates phichDuration
 *                  parameters and return the result of validation to the
 *                  calling function.
 ****************************************************************************/


MacRetType  validatePhichDuration( UInt8 phichDuration )
{
    if(phichDuration != 0 && phichDuration != 1){
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validatePhichResource
 * Inputs         : phichResource - Ng value
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This function validates phichResource Ng value
 *                  parameters and return the result of validation to the
 *                  calling function.
 ****************************************************************************/


MacRetType  validatePhichResource( UInt8 phichResource )
{
    if(phichResource != PHICH_RESOURCE_0 && phichResource != PHICH_RESOURCE_1 && 
            phichResource != PHICH_RESOURCE_2 && phichResource !=
            PHICH_RESOURCE_3){
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateEndRARntiRange
 * Inputs         : endRARntiRange_g - End RARNTI range
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This function validates end RARNTI range
 *                  parameters and return the result of validation to the
 *                  calling function.
 ****************************************************************************/


MacRetType  validateEndRARntiRange( UInt8 endRARntiRange_g, UInt8 startRARntiRange_g )
{
    if((endRARntiRange_g > cellConfigUeSim_g[numCells_g - 1].upperCRNTIBound) || 
            (endRARntiRange_g <=
             cellConfigUeSim_g[MAIN_CELL].lowerCRNTIBound) || 
            (endRARntiRange_g <= startRARntiRange_g))
    {
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateUEId
 * Inputs         : ueId  - UE Identifier.
 *                  cellIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Variables      :
 * Description    : This API takes ueId to validate and returns the result of
 *                  validation to the calling function.
 ****************************************************************************/
 
UInt16 validateUEId(UInt16 ueId, UInt8 cellIndex)
{
    if((ueId < cellConfigUeSim_g[cellIndex].lowerCRNTIBound) ||
        (ueId >cellConfigUeSim_g[cellIndex].upperCRNTIBound))
    {
        lteWarning("Invalid UE Id = %d\n", ueId);
        return MAC_INVALID_UE_ID;
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateLchId
 * Inputs         : lchId - Logical channel id.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes lchId  to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/
static inline MacRetType validateLchId(UInt8 lchId)
{
    if (lchId > MAC_MAX_LCHID)
    {
        lteWarning("Invalid range for lchId .\n");
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validateUEIndex
 * Inputs         : ueIndex  - UE Index which uniquely identifies UE.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Variables      :
 * Description    : This API takes ueIndex to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/
 
UInt16 validateUEIndex(UInt16 ueIndex)
{
    if(ueIndex >= MAX_UE_SUPPORTED)
    {
        lteWarning("Invalid UE Index = %d\n", ueIndex);
        return MAC_INVALID_UE_ID;
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validateUEPriority
 * Inputs         : uePriority - UE Priority to be validated.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Variables      :
 * Description    : This API takes uePriority to validate and returns the
 *                  result of validation to the calling function.
 ****************************************************************************/
 
UInt16 validateUEPriority(UInt8 uePriority)
{
    if(uePriority > MAX_UE_PRIORITY_RANGE)
    {
        lteWarning("Invalid UE Priority = %d\n", uePriority);
        return MAC_SYNTAX_ERROR;
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateModScheme
 * Inputs         : modulationScheme - modulation Scheme to be validated.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Variables      :
 * Description    : This API validates the modulation scheme for a UE.
 *                  Valid modulation schemes are QPSK, 16QAM, 64QAM.
 ****************************************************************************/
 
UInt16 validateModScheme(UInt8 modulationScheme)
{
    if((QPSK == modulationScheme) ||
       (SIXTEENQAM == modulationScheme) ||
       (SIXFOURQAM == modulationScheme)) 
    {
        return MAC_SUCCESS;
    }
    lteWarning("Invalid modulation scheme = %d\n", modulationScheme);
    return MAC_SYNTAX_ERROR;
}

/****************************************************************************
 * Function Name  : validatelchId
 * Inputs         : lchId  - ID of a logical Channel.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Variables      :
 * Description    : This API validates the lchId. Valid range is 1-10.
 ****************************************************************************/
 
UInt16 validatelchId(UInt8 lchId)
{
    if( lchId > MAX_NUMBER_OF_LOGICAL_CHANNEL)
    {
        lteWarning("Invalid lchId = %d\n", lchId);
        return MAC_SYNTAX_ERROR;
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : prepareLCErrorResponse
 * Inputs         : dwlkUEInfo_p - Ptr to a DL UE Info structure
 *                  uplkUEInfo_p - Ptr to a UL UE Info structure
 *                  msgBuff - Ptr to a buffer to be send to RRC
 *                  current_p - Ptr to current idx
 *                  uplkLogicalChCount_p - Ptr to uplk Logical Channel Count
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API takes pointer to the msg to be send and prepare
 *                  LC Error TLV in case of MAC_PARTIAL_SUCCESS.
 ****************************************************************************/

void prepareLCErrorResponse(ueContextForDownlink *dwlkUEInfo_p,
      ueContextForUplink *uplkUEInfo_p, UInt8 *msgBuff, UInt16 *current_p)
{
    UInt16 current = *current_p;
    LCConfigResp lcResp[MAX_NUMBER_OF_LOGICAL_CHANNEL];
    UInt16 lenIdx = 0, tag = 0, tagLen = 0;
    UInt16 len = 0;
    UInt8 i = 0, lcId = 0;
    
    for(i=1; i<MAX_NUMBER_OF_LOGICAL_CHANNEL; i++)
    {
        lcResp[i].status = 0;
    }    
    /* Get the DL LCs for which failure response to be send */
    for(i = 0; i < dwlkUEInfo_p->dwlkLogicalChCount; i++)
    {
        if(MAC_SUCCESS != dwlkUEInfo_p->logicalChConfig[i].result)
        {
            lcId = dwlkUEInfo_p->logicalChConfig[i].lchId;
            lcResp[lcId].opCode =
                dwlkUEInfo_p->logicalChConfig[i].operationType;
            lcResp[lcId].status |= DL_LC_INFO;
            lcResp[lcId].result = 
                   dwlkUEInfo_p->logicalChConfig[i].result;
        }    
    }
    /* Get the UL LCs for which failure response to be send */
    for(i = 0; i < uplkUEInfo_p->numLogicalChannels; i++)
    {
        if(MAC_SUCCESS != uplkUEInfo_p->lcInfo[i].result)
        {
            lcId = uplkUEInfo_p->lcInfo[i].lchId;
            lcResp[lcId].opCode =
                uplkUEInfo_p->lcInfo[i].operationType;
            lcResp[lcId].status |= UL_LC_INFO;
            lcResp[lcId].result =
                 uplkUEInfo_p->lcInfo[i].result;
        }    
    }

    /* Insert Error codes for failed LCs */
    for(i=1; i<MAX_NUMBER_OF_LOGICAL_CHANNEL; i++)
    {
        if(lcResp[i].status > 0)
        {
            if(ADD == lcResp[i].opCode)
            {
                tag = CREATE_LC_ERROR;
            }
            else if(RECONFIGURE == lcResp[i].opCode)
            {
                tag = RECONFIGURE_LC_ERROR;
            }
            else if(DELETE == lcResp[i].opCode)
            {
                tag = DELETE_LC_ERROR;
            }
            else 
                continue;
                
               
            LTE_SET_U16BIT(msgBuff + current,tag);
            current += TAG_SIZE;

            lenIdx = current;
            current += LENGTH_SIZE;
            /* Fill lchId for which request fail */
            msgBuff[current] = i;
            current++;
            /* This will give the reason why this LC config request fails */
            LTE_SET_U16BIT(msgBuff + current,lcResp[i].result);
            current += 2;

            len += (LENGTH_SIZE + TAG_SIZE + 3);      
            if(DELETE_LC_ERROR == tag)
            {
                msgBuff[current] = lcResp[i].status;
                current++;
                len++;
            }
            else
            {
                if((UL_DL_LC_INFO == lcResp[i].status) ||
                  (UL_LC_INFO == lcResp[i].status)) 
                {
                    tag = UL_LC_CONFIG_RESP;
                    LTE_SET_U16BIT(msgBuff + current,tag);
                    current += TAG_SIZE;

                    tagLen = UL_LC_RESP_LENGTH;
                    LTE_SET_U16BIT(msgBuff + current,tagLen);
                    current += LENGTH_SIZE;
                    len += (LENGTH_SIZE + TAG_SIZE);
                }    
                if((UL_DL_LC_INFO == lcResp[i].status) ||
                   (DL_LC_INFO == lcResp[i].status))
                {
                    tag = DL_LC_CONFIG_RESP;
                    LTE_SET_U16BIT(msgBuff + current,tag);
                    current += TAG_SIZE;

                    tagLen = DL_LC_RESP_LENGTH;
                    LTE_SET_U16BIT(msgBuff + current,tagLen);
                    current += LENGTH_SIZE;
                    len += (TAG_SIZE + LENGTH_SIZE);
                }  
            }    
            LTE_SET_U16BIT(msgBuff + lenIdx,len);
        }
    }

    *current_p = current;
}

/****************************************************************************
 * Function Name  : parseMsg
 * Inputs         : msg_p - Pointer to the data received from RRC.
 *                  msgId - Message Type of the message
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API takes pointer to the msg received as input, parse
 *                  the message and call createUEEntity() to configure UE at
 *                  Layer 2.
 ****************************************************************************/

/* + CA_AB */
void parseMsg(UInt16 msgId, UInt8 *msg_p, UInt16 msgLen, UInt8 cell_index)
{
/*UE_TESTING */
   // UInt16 current = MAC_API_HEADER_LEN -1 ;  /* Buffer Header is of 8 bytes */
    /*following needs to be uncommented*/
    UInt16 current = MAC_API_HEADER_LEN;  /* Buffer Header is of 8 bytes */
    ueContextForDownlink dlUEInfo;
    ueContextForUplink ulUEInfo;
    UInt8 i_loop =0;

/* Phase 2 Periodic CQi Changes Start*/
    UInt8 scellCount = 0;
/* Phase 2 Periodic CQi Changes End*/

    memset(&dlUEInfo,0, sizeof(ueContextForDownlink));
    memset(&ulUEInfo,0, sizeof(ueContextForUplink));
    
    UEEntityCreateRes createRes = {0};
    UEEntityDeleteRes deleteResp = {0};
    UEEntityReconfigRes reconfigUEEntityRes = {0};
    UInt8 resp = ACK;
    //UEEntityReconfigRes reconfigResp = {0};
    UInt16 transactionId = 0, idx = 0;
    UInt16 retVal = MAC_SUCCESS;

    transactionId = LTE_GET_U16BIT(msg_p);

    switch(msgId) {
        case MAC_RECONFIGURE_UE_ENTITY_REQ:
        #if 0
            fprintf(stderr, "\n Inside Case: MAC_RECONFIGURE_UE_ENTITY_REQ\n");
            #endif

//rajni - don't remove this code right now.. 
// will remove later once CA-Config changes done
#if 0
    if(reconfigCount == 1)
    {
        
        fprintf(stderr, " Rajni ---- reconfig ue for ca \n");
        uplinkContextInfoMap_g[0]->carrierAggrConfig.scellCount = 1;
         uplinkContextInfoMap_g[0]->carrierAggrConfig.scellInfo[0].cellIndex = 0;
         uplinkContextInfoMap_g[0]->carrierAggrConfig.scellInfo[0].sCellIndex= 1;
         uplinkContextInfoMap_g[0]->carrierAggrConfig.scellInfo[0].transmissionMode= 1;
         uplinkContextInfoMap_g[0]->carrierAggrConfig.scellInfo[0].dlNumHarqProcess= 8;

#if 1
         harqIndQNode_gp[cell_index] =  ( HarqIndQueueNodeInfo *) getMemFromPool( 
                 sizeof(HarqIndQueueNodeInfo) * (cellConfigUeSim_g[cell_index].crntiRange), PNULL);
         if(harqIndQNode_gp[cell_index] == PNULL) 
         {
             ltePanic(" Memory Allocation failed for harqIndQNode_gp[%d]\n", cell_index);
         }
         UInt32 rntiCntr = 0;
#if 0
		if (cellConfigUeSim_g[cell_index].lowerCRNTIBound == startRARNTI_g)
		{
			diff = endRARNTI_g - startRARNTI_g + 1 + cellConfigUeSim_g[cell_index].lowerCRNTIBound;
			//return(&rntiToIdxTbl_g[cell_index][(rnti - (diff + 1 + cellConfigUeSim_g[cell_index].lowerCRNTIBound ))]);
		}
		else
		{
            diff = cellConfigUeSim_g[cell_index].lowerCRNTIBound;
#if 0
			return(&rntiToIdxTbl_g[cell_index][(rnti - 
						cellConfigUeSim_g[cell_index].lowerCRNTIBound )]);
#endif
        }
#endif
         for (rntiCntr = 0; rntiCntr < cellConfigUeSim_g[cell_index].crntiRange; rntiCntr++)
         {
            harqIndQNode_gp[cell_index][rntiCntr].state = FALSE;
            harqIndQNode_gp[cell_index][rntiCntr].qIndex = MAX_SUBFRAME;
         } 
#endif
         return 1;

    }
    reconfigCount++;
#endif
            retVal = parseMACReconfigUEInfo(msg_p, &dlUEInfo,
                           &ulUEInfo, msgLen, &current, cell_index );
            /* + SPR_12249 */
            if (retVal == MAC_SUCCESS && (uplinkContextInfoMap_g[ulUEInfo.ueIdx]))
            /* - SPR_12249 */
            {
                /* + SPR 11213 Changes */
                if(ulUEInfo.carrierAggrConfig.scellCount == 1)
                {
				    updateUeCAUplinkContext(&ulUEInfo);
                }
                /* + CA_TDD_HARQ_CHANGES */
               /* SPR 19679: TDD HARQ Multiplexing Changes Start */
               //Code Removed
               /* SPR 19679: TDD HARQ Multiplexing Changes End */
                /* - CA_TDD_HARQ_CHANGES */
                /* + SPR 11213 Changes */
                resp =
                    reconfigureMACUEEntity( &dlUEInfo,
                            &ulUEInfo,
                            ulUEInfo.numLogicalChannels,
                            cell_index);
                /* + CQI_4.1 */
                getLAndNValues(dlUEInfo.ueIndex, cell_index); 
                if ( (0 != dlUEInfo.cqiInfoFlag) &&
                     (0 != ulUEInfo.periodicModeType) )
                {    
                    /* Calling the function to create the CQI PMI and RI preodicity map */
                    createCqiPmiRiPeriodicityMap(
                            /* SPR 10980 Fix Start */
							cqiPmiRiReportMap_g,
                            /* SPR 10980 Fix End */
                            ulUEInfo.cqiPMIConfigIndex,
                            ulUEInfo.riConfigIndex,
                            ulUEInfo.cqiformatIndicatorPeriodic_subband_k,
                            ulUEInfo.periodicModeType,
                            dlUEInfo.ueIndex,
                            0, FALSE);

                    
			if (SETUP == ulUEInfo.cqiConfigRequestType) /* EICIC +-*/
			{
                        /* Calling the function to create the CQI PMI and RI preodicity map for ABS */
                    createCqiPmiRiPeriodicityMap(
                            /* SPR 10980 Fix Start */
							cqiPmiRiReportMap2_g,
                            /* SPR 10980 Fix End */
                            ulUEInfo.cqiPMIConfigIndex_2,
                            ulUEInfo.riConfigIndex_2,
                            ulUEInfo.cqiformatIndicatorPeriodic_subband_k,
                            ulUEInfo.periodicModeType,
                            dlUEInfo.ueIndex,
                            0,TRUE);
			}

                }
                /* - CQI_4.1 */
/* Phase 2 Periodic CQi Changes Start */
                if(ulUEInfo.carrierAggrConfig.scellCount > 0)
                {
                    /* SPR 12465 FIX Start */
                    
                    getLAndNValuesScell(dlUEInfo.ueIndex,dlUEInfo.carrierAggrConfig.
                            scellInfo[0].sCellIndex);
                    /* SPR 12465 FIX End */
                    for (scellCount = 0 ; scellCount < dlUEInfo.carrierAggrConfig.\
                            scellCount; scellCount++)
                    {
                        createScellCqiPmiRiMap(&ulUEInfo, &dlUEInfo, scellCount);
                    }
                }
/* Phase 2 Periodic CQi Changes End*/
            }        
            else
            {
                resp = retVal;
            }
            if (MAC_LOG_CH_ERROR == resp)
            {
                reconfigUEEntityRes.response = MAC_PARTIAL_SUCCESS;
            }
            else
            {
                reconfigUEEntityRes.response= resp;
            }
            reconfigUEEntityRes.transactionId = transactionId;
            /* function that sends the confirmation message to RRC
             * */
#ifdef UT_TESTING
            MAC_EMULATOR_LOG(LOG_INFO,"\n********** response = %d\n", reconfigUEEntityRes.response);
#endif
            sendMsgToRRC(MAC_RECONFIGURE_UE_ENTITY_CNF,&reconfigUEEntityRes,
                             &dlUEInfo,&ulUEInfo, cell_index);
            break;

        case MAC_CREATE_UE_ENTITY_REQ:
            if(TRUE == cellConfigured_g[cell_index])
            {
                retVal = parseMACCreateUEInfo(msg_p, &dlUEInfo,
                             &ulUEInfo, &current, msgLen, cell_index);
                if(retVal == MAC_SUCCESS)
                {
                    if(cellConfigUeSim_g[cell_index].totalActiveUE >= MAX_NUM_UE)
                    {
                        resp = MAC_MAX_ACTIVE_UE_REACHED_ERR;
                    }
                    else 
                    {
                        /* Create UE Entity */
                        resp = createMACUEEntity(&dlUEInfo, &ulUEInfo,
                             ulUEInfo.numLogicalChannels, 
							 transactionId, cell_index);
                             // Nitin tweak
#ifdef TDD_CONFIG                        
							resp = MAC_SUCCESS;
#endif
                    }
                    /* + CQI_4.1 */
                    getLAndNValues(dlUEInfo.ueIndex, cell_index); 
                    if (0 != ulUEInfo.periodicModeType)
                    {    
                        /* Calling the function to create the CQI PMI and RI preodicity map */
                        createCqiPmiRiPeriodicityMap(
                                /* SPR 10980 Fix Start */
								cqiPmiRiReportMap_g,
                                /* SPR 10980 Fix End */
                                ulUEInfo.cqiPMIConfigIndex,
                                ulUEInfo.riConfigIndex,
                                ulUEInfo.cqiformatIndicatorPeriodic_subband_k,
                                ulUEInfo.periodicModeType,
                                dlUEInfo.ueIndex,
                                0,FALSE);

				/* EICIC +*/
	                      if (SETUP == ulUEInfo.cqiConfigRequestType) /* EICIC +-*/
				{
	                        /* Calling the function to create the CQI PMI and RI preodicity map for ABS */
	                    		createCqiPmiRiPeriodicityMap(
	                            /* SPR 10980 Fix Start */
								cqiPmiRiReportMap2_g,
	                            /* SPR 10980 Fix End */
	                            ulUEInfo.cqiPMIConfigIndex_2,
	                            ulUEInfo.riConfigIndex_2,
	                            ulUEInfo.cqiformatIndicatorPeriodic_subband_k,
	                            ulUEInfo.periodicModeType,
	                            dlUEInfo.ueIndex,
	                            0,TRUE);
				}
	                     /* EICIC -*/


                        
                    }
                    /* - CQI_4.1 */
                }
                else {
                    resp = retVal;
                }
            }
            else
            {
                resp = MAC_CELL_NOT_CONFIGURED;
            }    
            if(MAC_LOG_CH_ERROR == resp)
            {
                createRes.response = MAC_PARTIAL_SUCCESS;
            }    
            else {
                createRes.response = resp;
            }    
            if ( createRes.response == MAC_SUCCESS )
                   cellConfigUeSim_g[cell_index].totalActiveUE++; 
            createRes.transactionId = transactionId;
#ifdef UT_TESTING
            MAC_EMULATOR_LOG(LOG_INFO,"********** response = %d\n", createRes.response);
#endif
            sendMsgToRRC(MAC_CREATE_UE_ENTITY_CNF, &createRes, &dlUEInfo, &ulUEInfo, cell_index);
            break;

        case MAC_DELETE_UE_ENTITY_REQ:
             /* SPR 8748 FIX START*/
             if(msgLen == 2)
             {
                idx = LTE_GET_U16BIT(msg_p + current);
                current += 2;
                resp =  validateUEIndex (idx);
                if (resp != MAC_SUCCESS)
                {
                    lteWarning("Validation of ueIndex failed\n");
                    resp = MAC_INVALID_UE_ID;	
                }
                else
                    resp = deleteMACUEEntity(idx, transactionId,cell_index);

             }
             else 
             {
                resp = MAC_SYNTAX_ERROR;
             }
             /* SPR 8748 FIX END*/

            if((resp == MAC_SUCCESS) && (cellConfigUeSim_g[cell_index].totalActiveUE != 0 ))
            {
                cellConfigUeSim_g[cell_index].totalActiveUE--;
            }
            /* Prepare a response and send it to RRC Layer */
            deleteResp.response = resp;
            deleteResp.idx = idx;
            deleteResp.transactionId = transactionId;
            sendMsgToRRC(MAC_DELETE_UE_ENTITY_CNF, &deleteResp, PNULL,PNULL, cell_index);

            break;
        default:
            break;
    }
}
/* - CA_AB */


/****************************************************************************
 * Function Name  : prepareBufferHeader
 * Inputs         : msgId - Msg Type to be send to RRC
 *                : msgBuf - Buffer to be filled
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API takes the msgId & buffer to be filled by msg 
 *                  header as input. 
 ****************************************************************************/
/* + CA_AB */
void prepareBufferHeader(UInt8 *msgBuf, UInt16 msgId, UInt16 destModuleId, UInt8 cell_index )
{
    /* transaction Id to be filled later */
    UInt16 current = 2;

    LTE_SET_U16BIT(msgBuf + current, MAC_MODULE_ID);
    current += 2;
    LTE_SET_U16BIT(msgBuf + current, destModuleId);
    current += 2;
    LTE_SET_U16BIT(msgBuf + current, msgId);
    current += 2;
//	msgBuf[current] = cell_index ;
 //   current++;
    return;
}
/* - CA_AB */


/****************************************************************************
 * Function Name  : sendMsgToRRC
 * Inputs         : msgId - Msg Type to be send to RRC
 *                : data_p - pointer to the Data to be send
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API takes the msgId & data to be send as input,
 *                  prepares the message and send it to RRC
 ****************************************************************************/
void sendMsgToRRC(UInt16 msgId, void *data_p, ueContextForDownlink *dlUEInfo, ueContextForUplink *ulUEInfo, UInt8 cellIndex)
{
    LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g,
          "Function %s Entry %d\n", __func__, msgId);
    UInt16 current = 0;
    UInt8 msgBuf[MAX_DATA_SIZE] = {0};
    UInt8 wrongMsgId = FALSE;
    UInt16 ueIdx = MAX_UE_SUPPORTED;
    UInt16 transactionId = 0xFFFF;
    rrcDataInfo *rrcInfo_p = PNULL;
    ChangeCrntiRespInfo changeCrntiResp = {0};

    prepareBufferHeader(msgBuf, msgId, RRC_MODULE_ID, cellIndex);
	/* UE_TESTING */
//    current = MAC_API_HEADER_LEN -1; 
	/* UE_TESTING */
	/* need to uncomment */
    current = MAC_API_HEADER_LEN; 
	/* need to uncomment */

    switch(msgId) {
        case MAC_CREATE_UE_ENTITY_CNF:
            /* Insert transactionId received for create request */
            LTE_SET_U16BIT(msgBuf,((UEEntityCreateRes *)data_p)->transactionId); 
            /* Insert ueIndex and response Code */  
            ueIdx = dlUEInfo->ueIndex;
            LTE_SET_U16BIT(msgBuf+current,ueIdx);
            current += 2;
            LTE_SET_U16BIT(msgBuf+current,((UEEntityCreateRes *)data_p)->response);   
            current += 2;
            /* If responseCode is MAC_PARTIAL_SUCCESS, fill the LCs which
               failed in config */
            if(MAC_PARTIAL_SUCCESS == ((UEEntityCreateRes *)data_p)->response)
            {
                prepareLCErrorResponse(dlUEInfo,ulUEInfo, msgBuf,&current);
            }
            break;

        case MAC_DELETE_UE_ENTITY_CNF:
            /* fill transaction Id for delete resonse */
            LTE_SET_U16BIT(msgBuf,((UEEntityDeleteRes *)data_p)->transactionId);
            /* Fill ueIndex and responseCode */  
            LTE_SET_U16BIT(msgBuf+current,((UEEntityDeleteRes *)data_p)->idx);
            current += 2;
            /* Fill responseCode */
            LTE_SET_U16BIT(msgBuf+current,((UEEntityDeleteRes *)data_p)->response);
            current += 2;
            break;

        case MAC_RECONFIGURE_UE_ENTITY_CNF:
            /* fill transaction Id for delete resonse */
            LTE_SET_U16BIT(msgBuf,((UEEntityReconfigRes *)data_p)->transactionId);

            ueIdx = dlUEInfo->ueIndex;
            LTE_SET_U16BIT(msgBuf+current,ueIdx);
            current += 2;
            LTE_SET_U16BIT(msgBuf+current,((UEEntityReconfigRes *)data_p)->response);
            current += 2;
            break;
        case RRC_CONNECTION_SETUP:
        case RRC_CONNECTION_SETUP_NB:  
            LTE_SET_U16BIT(msgBuf,transactionId);
      
            rrcInfo_p = (rrcDataInfo *)data_p;

            LTE_SET_U16BIT(&msgBuf[current], rrcInfo_p->rnti);
            current +=2;
            
            memcpy(&msgBuf[current], rrcInfo_p->buffer_p, rrcInfo_p->dataLen);
            current += rrcInfo_p->dataLen;
            break;

        case MAC_PCCH_MSG_IND:
            transactionId = 0;
            LTE_SET_U16BIT(msgBuf,transactionId);

            rrcInfo_p = (rrcDataInfo *)data_p;

            /* SPR 2722 Changes Start */
            LTE_SET_U16BIT(&msgBuf[current], rrcInfo_p->rnti);
            current += 2;
            /* SPR 2722 Changes End */
	    if( (rrcInfo_p->buffer_p) && (rrcInfo_p->rnti == 65534))
            { 
		    memcpy(&msgBuf[current], rrcInfo_p->buffer_p, rrcInfo_p->dataLen);
		    current += rrcInfo_p->dataLen;

	    }
	    else
	    {
		    wrongMsgId = TRUE;
	    }

                        
            break;

/* HO code changes */
        case MAC_EMU_RESET_CNF:
            LTE_SET_U16BIT(msgBuf,((UEEntityDeleteRes *)data_p)->transactionId);
            LTE_SET_U16BIT(msgBuf+current,((UEEntityDeleteRes *)data_p)->idx);
            current += 2;
            /* Fill responseCode */
            LTE_SET_U16BIT(msgBuf+current,((UEEntityDeleteRes *)data_p)->response);
            current += 2;
            break;
        case MAC_EMU_CHANGE_CRNTI_CNF:

            LTE_SET_U16BIT(msgBuf, (((ChangeCrntiRespInfo *)data_p)->transactionId));
            LTE_SET_U16BIT(msgBuf + current,(((ChangeCrntiRespInfo *)data_p)->ueIndex));
            current += 2;
            LTE_SET_U16BIT(msgBuf + current,(((ChangeCrntiRespInfo *)data_p)->resp));
            current += 2;
            break;
           /*SPR 21660 changes start_Kishore*/
            /*SPR 21660 changes end_Kishore*/
/**/
        default:
            wrongMsgId = TRUE;
            lteWarning("Incorrect msgId provided\n");
            break;
    }

    /* Fill msgLen Now */
	/* UE_TESTING*/
//    LTE_SET_U16BIT(msgBuf + MAC_API_HEADER_LEN - 2 -1,current);
	/* Need to uncomment */
   LTE_SET_U16BIT(msgBuf + MAC_API_HEADER_LEN - 2,current);
	/* Need to uncomment */

    /* Send msg to RRC Layer */
    if(FALSE == wrongMsgId)
    {
#ifdef UT_TESTING	    
        UInt8 i = 0;
        MAC_EMULATOR_LOG(LOG_INFO, "sending data to RRC Lenth [%d]\n", current);
        while (i < current)
        {
           MAC_EMULATOR_LOG(LOG_INFO, "[%x]\t", msgBuf[i++]);
        }
#endif
        sendto(txRRCSockFD_g, (UInt8 *)msgBuf, current, 0,
                (const SockAddr *)&tx_addr_g, sizeof (tx_addr_g));
    }
    LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g,
          "Function %s Exit\n", __func__);
}

/****************************************************************************
 * Function Name  : parseDeleteLcReq
 * Inputs         : uplkUEInfo_p - Ptr to UL UE Info structure
 *                  dwlkUEInfo_p - Ptr to DL UE Info structure
 *                  len - Length of the TLV
 *                  current_p - current Idx of the message received.
 *                  msg_p - buffer received from RRC Layer.
 *                  uplkLCCount_p - UL LC count to be configured.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_SYNTAX_ERROR.
 * Variables      :
 * Description    : This API parse the deleteLc Request and validate it. If
 *                  validation fails it returns MAC_SYNTAX_ERROR.
 ****************************************************************************/

UInt16 parseDeleteLcReq(ueContextForUplink *ulLCDeleteReq_p,
        ueContextForDownlink * dlLCDeleteReq_p,
        UInt16 deleteLcReqTagLen, 
        UInt8 *msg_p)
{
    UInt16 resp = MAC_SUCCESS;
    UInt32 lchId  = 0;
    UInt32 lcType = 0;
    UInt32 lcIdx  = 0;
    /* Atleast DL or UL tag should be present */
    if ( deleteLcReqTagLen != DELETE_LC_REQ_LEN)
    {
        lteWarning("Atleast DL or UL tag should be present .\n");
        return MAC_SYNTAX_ERROR;
    }

    lchId = *msg_p++;
    /* validation of logical channel id */
    resp = validateLchId(lchId );
    if (resp != MAC_SUCCESS)
    {
        return resp;
    }
    /* LC Types represent the operation for  
                1 - Only Downlink - DL_LC_INFO
                2 - Only uplink   - UL_LC_INFO
                3 - Both          - UL_DL_LC_INFO
    */
    lcType = *msg_p++;
    
    if ((lcType == 0) || (lcType > MAX_LC_TYPE))
    {
        lteWarning("Invalid Lc Type [%d]", lcType);
        return MAC_SYNTAX_ERROR;
    }
    if ((UL_DL_LC_INFO == lcType) || (UL_LC_INFO == lcType))
    {
        lcIdx = ulLCDeleteReq_p->numLogicalChannels;
        if (lcIdx < MAC_MAX_LCHID)
        {
            ulLCDeleteReq_p->lcInfo[lcIdx].lchId = lchId;
            ulLCDeleteReq_p->lcInfo[lcIdx].operationType = DELETE;
            ulLCDeleteReq_p->numLogicalChannels++;    
        }
    }    
    if ((UL_DL_LC_INFO == lcType) || (DL_LC_INFO == lcType))
    {
        lcIdx = dlLCDeleteReq_p->dwlkLogicalChCount;
        if (lcIdx < MAC_MAX_LCHID)
        {
            dlLCDeleteReq_p->logicalChConfig[lcIdx].lchId = lchId;
            dlLCDeleteReq_p->logicalChConfig[lcIdx].operationType = DELETE;
            dlLCDeleteReq_p->dwlkLogicalChCount++;
        }
    }
    
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseReconfigLcReq
 * Inputs         : uplkUEInfo_p - Ptr to UL UE Info structure
 *                  dwlkUEInfo_p - Ptr to DL UE Info structure
 *                  len - Length of the TLV
 *                  current_p - current Idx of the message received.
 *                  msg_p - buffer received from RRC Layer.
 *                  uplkLCCount_p - UL LC count to be configured.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_SYNTAX_ERROR.
 * Variables      :
 * Description    : This API buffer to be parsed for RECONFIG LC REQ,
 *                  and returns MAC_SUCCESS or MAC_SYNTAX_ERROR on failure.
 ****************************************************************************/

MacRetType parseReconfigLcReq(ueContextForUplink *ulLCReconfigReq_p,
        ueContextForDownlink *dlLCReconfigReq_p,
        UInt16 reconfigLcReqTagLen, 
        UInt8 *msg_p, UInt8 cell_index)
{
    UInt16 resp = MAC_SUCCESS;
    UInt8 lchId = 0;
    UInt8 lcIdx = 0;
    UInt16 tagLen = 0;
    UInt16 tag = 0;
    
    lchId = *msg_p++;
    /* validation of logical channel id */
    resp = validateLchId(lchId);
    if (resp != MAC_SUCCESS)
    {
        return resp;
    }

    reconfigLcReqTagLen -= RECONFIG_LC_REQ_MIN_LEN;
    /* Atleast DL or UL tag should be present */
    if ( reconfigLcReqTagLen < TAG_LEN)
    {
        lteWarning("Atleast DL or UL tag should be present .\n");
        return MAC_SYNTAX_ERROR;
    }

    while ( reconfigLcReqTagLen > 0)
    {
        tag = LTE_GET_U16BIT(msg_p);
        msg_p += 2;

        tagLen  = LTE_GET_U16BIT(msg_p);
        msg_p += 2;

        reconfigLcReqTagLen -= tagLen;
        switch(tag)
        {
            case UL_LC_RECONFIGURE_REQ:
                //lcIdx = ulUEInfo_p->numLogicalChannels;
                if (tagLen != UL_LC_RECONFIG_REQ_LEN)
                {
                    lteWarning("Mandatory fields missing in UL_LC_RECONFIG_REQ_LEN.\n");
                    return MAC_SYNTAX_ERROR;
                }
                lcIdx = ulLCReconfigReq_p->numLogicalChannels;
                if (lcIdx >= MAC_MAX_LCHID)
                {
                    lteWarning("Maximum UL LC reached.\n");
                    return MAC_FAILURE;
                }
                ulLCReconfigReq_p->lcInfo[lcIdx].lchId = lchId;
                ulLCReconfigReq_p->lcInfo[lcIdx].operationType = RECONFIGURE;
#if 1
                ulLCReconfigReq_p->lcInfo[lcIdx].lcGId = *msg_p++;
#endif
                ulLCReconfigReq_p->numLogicalChannels++;

                break;

            case DL_LC_RECONFIGURE_REQ:
                if (tagLen != DL_LC_RECONFIG_REQ_LEN)
                {
                    lteWarning("Mandatory fields missing in DL_LC_RECONFIG_REQ_LEN.\n");
                    return MAC_SYNTAX_ERROR;
                }
                lcIdx = dlLCReconfigReq_p->dwlkLogicalChCount;
                if (lcIdx >= MAC_MAX_LCHID)
                {
                    lteWarning("Maximum UL LC reached.\n");
                    return MAC_FAILURE;

                }
                dlLCReconfigReq_p->logicalChConfig[lcIdx].lchId = lchId;
                dlLCReconfigReq_p->logicalChConfig[lcIdx].operationType = RECONFIGURE;

                dlLCReconfigReq_p->dwlkLogicalChCount++;
                break;

            default:
                lteWarning("Invalid tag received for RECONFIG_LC_REQ\n");
                return MAC_SYNTAX_ERROR;
        }
    }

    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : parseCreateLcReq
 * Inputs         : uplkUEInfo_p - Ptr to UL UE Info structure
 *                  dwlkUEInfo_p - Ptr to DL UE Info structure
 *                  len - Length of the TLV
 *                  current_p - current Idx of the message received.
 *                  msg_p - buffer received from RRC Layer.
 *                  uplkLCCount_p - UL LC count to be configured.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_SYNTAX_ERROR.
 * Variables      :
 * Description    : This API buffer to be parsed for CREATE LC REQ,
 *                  and returns MAC_SUCCESS or MAC_SYNTAX_ERROR on failure.
 ****************************************************************************/

/* + CA_AB */
UInt16 parseCreateLcReq(ueContextForUplink *ulUEInfo_p,
        ueContextForDownlink * dlUEInfo_p,
        UInt16 len, UInt16 *current_p,
        UInt8 *msg_p, UInt8 cell_index)
{
    UInt16 tag = 0;
    UInt16 tagLen = 0;
    UInt16 current = *current_p;
    UInt8 lchId = 0, lcIdx = 0;
    UInt8 rlcMode = 0;
/* SPR 1172 Fix Start */    
    UInt8 rlcSNFieldLength = 0;
/* SPR 1172 Fix End */    
    UInt8 ulLcIdx = 0;
    UInt8 bearerType = 0;
    UInt8 qci = 0;
    //len = len - 4;

    lchId = *(msg_p + current);
    if(MAC_SYNTAX_ERROR == validatelchId(lchId))
    {
        current += len;
        *current_p = current;
        return MAC_SYNTAX_ERROR;
    }
    current++;
    rlcMode = *(msg_p + current);
    current++;
    len -= CREATE_LC_REQ_MIN_LEN;

    if(len < TAG_LEN)
    {
        lteWarning("Atleast DL or UL tag should be present .\n");
        *current_p = current;
        return MAC_SYNTAX_ERROR;
    }

    while(len > 0)
    {
        tag = LTE_GET_U16BIT(msg_p + current);
        current += 2;

        tagLen = LTE_GET_U16BIT(msg_p + current);
        current += 2; 
        len -= tagLen;

        switch(tag)
        {
            case UL_LC_CREATE_REQ:
                if (tagLen < UL_LC_CREATE_REQ_LEN)
                {
                    *current_p = current;
                    lteWarning("Mandatory fields missing in UL_LC_CREATE_REQ_LEN.\n");
                    return MAC_SYNTAX_ERROR;
                }

                lcIdx = ulUEInfo_p->numLogicalChannels;
                if(lcIdx >= MAX_NUMBER_OF_LOGICAL_CHANNEL)
                {
                    current += (tagLen - 4);
                    len -= (tagLen - 4);
                    break;
                }
            
                ulUEInfo_p->lcInfo[lcIdx].lchId = lchId;
                ulUEInfo_p->lcInfo[lcIdx].rlcMode = rlcMode;
                ulUEInfo_p->lcInfo[lcIdx].operationType = ADD;
                ulUEInfo_p->lcInfo[lcIdx].lcGId =
                    *(msg_p + current);
                current++;
                ulUEInfo_p->rlcMode = rlcMode;
#if 0
                MAC_EMULATOR_LOG(LOG_INFO," UL =>RLC MOde is %d and sn Field is %d and ulUEInfo_p->rlcMode is %d\n",ulUEInfo_p->lcInfo[lcIdx].rlcMode,ulUEInfo_p->lcInfo[lcIdx].rlcSNFieldLength,ulUEInfo_p->rlcMode);
                #endif
#if 0
                /*Rohit :  tweak start*/
                MAC_EMULATOR_LOG(LOG_INFO,"UL_LC_CREATE_REQ: Tweak added for configuring DRB in Am mode lcIdx = %d\n", lcIdx);
                ulUEInfo_p->rlcMode = 2;
                ulUEInfo_p->lcInfo[lcIdx].rlcMode = 2;
                ulUEInfo_p->lcInfo[lcIdx].rlcSNFieldLength = 10;
                /*Rohit :  tweak end*/
#endif                

                ulUEInfo_p->numLogicalChannels++;
#ifdef UPDATED_RRC_QOS
                /**NOTE: ENABLE THIS FLAG ONLY WHEN THE RRC SUPPORTS SENDING
                 * OF NEWLY ADDED PARAMETER: Priority**/
                //current = current + 1;
#endif
                break;

            case DL_LC_CREATE_REQ:
                if (tagLen < DL_LC_CREATE_REQ_LEN)
                {
                    lteWarning("Mandatory fields missing in DL_LC_CREATE_REQ_LEN.\n");
                    return MAC_SYNTAX_ERROR;
                }
                lcIdx = dlUEInfo_p->dwlkLogicalChCount;
                if(lcIdx >= MAX_NUMBER_OF_LOGICAL_CHANNEL)
                {
                    current += (tagLen -4);
                    len -= (tagLen -4);
                    break;
                }           
                dlUEInfo_p->logicalChConfig[lcIdx].lchId = lchId;
                dlUEInfo_p->logicalChConfig[lcIdx].rlcMode = rlcMode;
                dlUEInfo_p->logicalChConfig[lcIdx].operationType = ADD;

                dlUEInfo_p->logicalChConfig[lcIdx].lchPriority =
                    *(msg_p + current);
                current++;
                dlUEInfo_p->rlcMode = rlcMode;

/* SPR 1172 Fix Start */    
              rlcSNFieldLength = *(msg_p + current);
              current++;
              if (rlcSNFieldLength != 5 && rlcSNFieldLength != 10)
              {
                  lteWarning("Invalid rlcSNFieldLength set");
                  return MAC_FAILURE;
              }
              dlUEInfo_p->logicalChConfig[lcIdx].rlcSNFieldLength = rlcSNFieldLength;
/* SPR 1172 Fix End */    

#if 0
                MAC_EMULATOR_LOG(LOG_INFO," DL =>RLC MOde is %d and sn Field is %d and dlUEInfo_p->rlcMode is %d\n",dlUEInfo_p->logicalChConfig[lcIdx].rlcMode,dlUEInfo_p->logicalChConfig[lcIdx].rlcSNFieldLength,dlUEInfo_p->rlcMode);
#endif
#if 0
                /*Rohit :  tweak start*/
                MAC_EMULATOR_LOG(LOG_INFO,"DL_LC_CREATE_REQ: Tweak added for configuring DRB in Am mode, lcIdx = %d\n", lcIdx);
                dlUEInfo_p->rlcMode = 2;
                dlUEInfo_p->logicalChConfig[lcIdx].rlcMode = 2;
                dlUEInfo_p->logicalChConfig[lcIdx].rlcSNFieldLength = 10;
                /*Rohit :  tweak start*/
#endif
                dlUEInfo_p->dwlkLogicalChCount++;
                break;
            case QOS_INFO:
                {
                    if(tagLen < QOS_INFO_MIN_LEN ||
                            !( dlUEInfo_p->dwlkLogicalChCount ||
                                ulUEInfo_p->numLogicalChannels) )
                    {
                        lteWarning("Mac syntax length error .\n");
                        return MAC_SYNTAX_ERROR;
                    }

                    ulLcIdx = ulUEInfo_p->numLogicalChannels - 1;

                    bearerType =*(msg_p + current);
                    current++;

                    if (bearerType > 1)
                    {
                        lteWarning("Mac syntax length error .\n");
                    }

                    qci =*(msg_p + current);
                    current++;
                    if (qci > 8)
                    {
                        lteWarning("Mac syntax length error .\n");
                    }

                    ulUEInfo_p->lcInfo[ulLcIdx].bearerType = bearerType;
                    ulUEInfo_p->lcInfo[ulLcIdx].qci = qci;

                    /* setting isSpsLc as per qci mapping */
                    if (TRUE == cellConfigUeSim_g[cell_index].ueSpsSysWideParam.qciSpslist[qci - 1])
                    {
                        ulUEInfo_p->lcInfo[ulLcIdx].isSpsLc = TRUE;
                    }
                    else
                    {
                        ulUEInfo_p->lcInfo[ulLcIdx].isSpsLc = FALSE;
                    }
                }
            break;

            default:
                lteWarning("Invalid tag received for CREATE_LC_REQ\n");
                current += (tagLen - 4);
                len -= (tagLen - 4);
                *current_p = current;
                //return MAC_SYNTAX_ERROR;
        }
    }
    *current_p = current;
    return MAC_SUCCESS;
}

/* EICIC +*/
/******************************************************************************
 * Function Name  : parseCreateUECqiConfigIndexV10Info 
 * Inputs         : msgBuf - pointer to the input buffer
 *                  ulCreateReq_p - pointer to Uplink create UE structure
 *                  dlCreateReq_p - pointer to Downlink create UE structure 
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function parse the csiconfigindexv10 information in 
 *                  Create UE Entity Request and fills in the structure needed  
 *                  by MAC to create a UE context.
 ******************************************************************************/
STATIC inline MacRetType parseCreateUECqiConfigIndexV10Info( UInt8 **msgBuf,
        ueContextForUplink *ulCreateReq_p, ueContextForDownlink *dlCreateReq_p 
        /* CA changes Start */
        ,UInt8 internalCellIndex
        /* CA Changes end */
        )
{
    UInt16 tag             = 0;
    UInt8 *msg_p           = PNULL;
    UInt16 cqiconfigindexTagLenV10   = 0;
    UInt16 cqiconfigindex2TagLenV10   = 0;
    UInt16 riconfigindexTagLenV10   = 0;
    msg_p = *msgBuf;

    msg_p += 2;

    /* cqi report v920 tag length*/
    cqiconfigindexTagLenV10 = LTE_GET_U16BIT(msg_p);
    msg_p += 2;

    if (CSI_CONFIG_INDEX_VR10_TAG_LENGTH > cqiconfigindexTagLenV10)
    {      
         lteWarning("Invalid cqiconfigindexTagLenV10 ");
        MAC_EMULATOR_LOG(LOG_INFO,"Invalid cqiconfigindexTagLenV10 -> %d\n",cqiconfigindexTagLenV10 ); 
        return MAC_FAILURE;
    }

        ulCreateReq_p->cqiConfigRequestType = *(msg_p);
        msg_p++;
        /* + SPR 5953 Fix */
        dlCreateReq_p->cqiConfigRequestType = ulCreateReq_p->cqiConfigRequestType;
        /* - SPR 5953 Fix */
        /*L2_FIX_8 */
        if ( ulCreateReq_p->cqiConfigRequestType > SETUP)
        {
	         lteWarning("Invalid cqiConfigRequestType ");
	        MAC_EMULATOR_LOG(LOG_INFO,"Invalid cqiConfigRequestType -> %d\n", ulCreateReq_p->cqiConfigRequestType ); 
            /* Review comment fix end RJ13 */
             return MAC_FAILURE;
        }
        if ( SETUP == ulCreateReq_p->cqiConfigRequestType )
        {
                 tag = LTE_GET_U16BIT(msg_p);
                 if (tag == CQI_PMI_CONFIGINDEX2_VR10)
                 {
                 msg_p += 2;
		   cqiconfigindex2TagLenV10 = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (CSI_CONFIG_INDEX_VR10_TAG_LENGTH > cqiconfigindex2TagLenV10)
                 {      
	         lteWarning("Invalid cqiconfigindex2TagLenV10 ");
	        MAC_EMULATOR_LOG(LOG_INFO,"Invalid cqiconfigindex2TagLenV10 -> %d\n", cqiconfigindex2TagLenV10 ); 

        return MAC_FAILURE;
                 }
		  ulCreateReq_p->cqiPMIConfigIndex_2 = LTE_GET_U16BIT(msg_p);
		  msg_p += 2;

		   tag = LTE_GET_U16BIT(msg_p);
                 }
                 if (tag == RI_CONFIG_INDEX_INFO)
                 {
                 msg_p += 2;
		   riconfigindexTagLenV10 = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (RI_CONFIGINDEX2_VR10_TAG_LENGTH > riconfigindexTagLenV10)
                 {      
		         lteWarning("Invalid riconfigindexTagLenV10 ");
		        MAC_EMULATOR_LOG(LOG_INFO,"Invalid riconfigindexTagLenV10 -> %d\n", riconfigindexTagLenV10 ); 
                    return MAC_FAILURE;
                 }
		  ulCreateReq_p->riConfigIndex_2 = LTE_GET_U16BIT(msg_p);
		  msg_p += 2;
                 }
				 
        }
    *msgBuf = msg_p;
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name  : parseCreateUECsiSubframePatternV10Info 
 * Inputs         : msgBuf - pointer to the input buffer
 *                  ulCreateReq_p - pointer to Uplink create UE structure
 *                  dlCreateReq_p - pointer to Downlink create UE structure 
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function parse the csisubframepatternconfigv10 information in 
 *                  Create UE Entity Request and fills in the structure needed  
 *                  by MAC to create a UE context.
 ******************************************************************************/
STATIC inline MacRetType parseCreateUECsiSubframePatternV10Info( UInt8 **msgBuf,
         ueContextForUplink *ulCreateReq_p, ueContextForDownlink *dlCreateReq_p 
        /* CA changes Start */
        ,UInt8 internalCellIndex
        /* CA Changes end */
        )
{
    UInt16 tag             = 0;
    UInt8 *msg_p           = PNULL;
    UInt16 csisubframepatternTagLen   = 0;
    UInt16 csimeasurementsubsetTagLen   = 0;
    UInt8 subsetIndexNum = 0;
    msg_p = *msgBuf;
    UInt8 ConfigRequestType = 0;
    CsiChoice choice = 0;
    UInt8 subsetPatternLength = 0;
    UInt8 csiMeasurementSubset1[15] = { 0 };
    UInt8 csiMeasurementSubset2[15] = { 0 };

    msg_p += 2;

    /* cqi report v920 tag length*/
    csisubframepatternTagLen = LTE_GET_U16BIT(msg_p);
    msg_p += 2;

    if (CSI_SUBFRAME_PATTERN_CONFIG_VR10_TAG_LENGTH  > csisubframepatternTagLen)
    {      
		   lteWarning("Invalid csisubframepatternTagLen\n");
		   MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid csisubframepatternTagLen %d\n", csisubframepatternTagLen);     

        return MAC_FAILURE;
    }

     ConfigRequestType = *(msg_p);
      ulCreateReq_p->cqiSubsetConfigRequestType = ConfigRequestType;
      dlCreateReq_p->cqiSubsetConfigRequestType = ConfigRequestType;
      
        msg_p++;
      
       
        if ( ConfigRequestType > SETUP)
        {
		            lteWarning("Invalid ConfigRequestType\n");
		            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid ConfigRequestType %d\n", ConfigRequestType);     

            /* Review comment fix end RJ13 */
            return MAC_FAILURE;
        }
        if ( ConfigRequestType )
        {
                 tag = LTE_GET_U16BIT(msg_p);
                 if(tag == CSI_MEASUREMENT_SUBSET_VR10)
                 {
                 msg_p += 2;
		   csimeasurementsubsetTagLen = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (CSI_MEASUREMENT_SUBSET_VR10_TAG_LENGTH  > csimeasurementsubsetTagLen)
                 {      
		            lteWarning("Invalid csimeasurementsubsetTagLen\n");
		            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid csimeasurementsubsetTagLen %d\n", csimeasurementsubsetTagLen);     
					 
                     return MAC_FAILURE;
                 }
			  
                  choice = (UInt8)*msg_p;
		    msg_p += 1;
#ifdef FDD_CONFIG

		    if(FDD_CONFIG_TYPE== choice)
		    {
		        subsetPatternLength = 5;
		    }
		    else
		    {
		            lteWarning("Invalid FDD choice\n");
		            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid choice  %d\n", choice);     

            /* Review comment fix end RJ13 */
                     return MAC_FAILURE;
		    }
#else
		    if(TDD_CONFIG1_5 == choice)
		    {
		        subsetPatternLength = 3;
		    }	
                  else if(TDD_CONFIG0 == choice)
		    {
		        subsetPatternLength = 9;
		    }	
		    else if(TDD_CONFIG6 == choice)
		    {
		        subsetPatternLength = 8;
		    }	
		    else
		    {
		            lteWarning("Invalid TDD choice\n");
		            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid choice  %d\n", choice);     
			 
            /* Review comment fix end RJ13 */
                     return MAC_FAILURE;
		    }
				
#endif
	           for(subsetIndexNum = 0;subsetIndexNum< subsetPatternLength; subsetIndexNum++)
	           {
		      csiMeasurementSubset1[subsetIndexNum] = *(msg_p);
		      msg_p++;
	      	   }

	           updateCsiMeasurementSubset(ulCreateReq_p->csiMeasurementSubset1,csiMeasurementSubset1,internalCellIndex);
		 }
	      tag = LTE_GET_U16BIT(msg_p);
               

                if(tag == CSI_MEASUREMENT_SUBSET2_VR10)
                {
                 msg_p += 2;
		   csimeasurementsubsetTagLen = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (CSI_MEASUREMENT_SUBSET_VR10_TAG_LENGTH  > csimeasurementsubsetTagLen)
                 {      
		            lteWarning("Invalid csimeasurementsubsetTagLen\n");
		            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid csimeasurementsubsetTagLen  %d\n", csimeasurementsubsetTagLen);     
					 
                     return MAC_FAILURE;
                 }
			  
                  choice = (UInt8)*msg_p;
		    msg_p += 1;
#ifdef FDD_CONFIG

		    if(FDD_CONFIG_TYPE== choice)
		    {
		        subsetPatternLength = 5;
		    }
		    else
		    {
		            lteWarning("Invalid choice\n");
		            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid Choice %d\n", choice);        

            /* Review comment fix end RJ13 */
                     return MAC_FAILURE;
		    }
#else
		    if(TDD_CONFIG1_5 == choice)
		    {
		        subsetPatternLength = 3;
		    }	
                  else if(TDD_CONFIG0 == choice)
		    {
		        subsetPatternLength = 9;
		    }	
		    else if(TDD_CONFIG6 == choice)
		    {
		        subsetPatternLength = 8;
		    }	
		    else
		    {
		            lteWarning("Invalid Choice\n");
		            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Choice \n");        
			 
            /* Review comment fix end RJ13 */
                     return MAC_FAILURE;
		    }
				
#endif
	           for(subsetIndexNum = 0;subsetIndexNum< subsetPatternLength; subsetIndexNum++)
	           {
		          csiMeasurementSubset2[subsetIndexNum] = *(msg_p);
		      msg_p++;
	      	   }
	           updateCsiMeasurementSubset(ulCreateReq_p->csiMeasurementSubset2,csiMeasurementSubset2,internalCellIndex);
	           
	      tag = LTE_GET_U16BIT(msg_p);
               }
		}		 
    *msgBuf = msg_p;
    return MAC_SUCCESS;

}

#if 0

/******************************************************************************
 * Function Name  : parseCreateUECsiSubframePatternV10Info 
 * Inputs         : msgBuf - pointer to the input buffer
 *                  ulCreateReq_p - pointer to Uplink create UE structure
 *                  dlCreateReq_p - pointer to Downlink create UE structure 
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function parse the csisubframepatternconfigv10 information in 
 *                  Create UE Entity Request and fills in the structure needed  
 *                  by MAC to create a UE context.
 ******************************************************************************/
STATIC inline MacRetType parseCreateUECsiSubframePatternV10Info( UInt8 **msgBuf,
         ueContextForUplink *ulCreateReq_p, ueContextForDownlink *dlCreateReq_p 
        /* CA changes Start */
        ,UInt8 internalCellIndex
        /* CA Changes end */
        )
{
    UInt16 tag             = 0;
    UInt8 *msg_p           = PNULL;
    UInt16 csisubframepatternTagLen   = 0;
    UInt16 csimeasurementsubset1TagLen   = 0;
    UInt16 csimeasurementsubset2TagLen   = 0;
    UInt8 subsetIndexNum = 0;
    msg_p = *msgBuf;
    UInt8 ConfigRequestType = 0;
    UInt8 csiMeasurementSubset1[5] = { 0 };
    UInt8 csiMeasurementSubset2[5] = { 0 } ;

    msg_p += 2;

    /* cqi report v920 tag length*/
    csisubframepatternTagLen = LTE_GET_U16BIT(msg_p);
    msg_p += 2;

    if (CSI_SUBFRAME_PATTERN_CONFIG_VR10_TAG_LENGTH  > csisubframepatternTagLen)
    {      
         lteWarning("Invalid csisubframepatternTagLen ");
        MAC_EMULATOR_LOG(LOG_INFO,"CQI_REPORT_CONFIG_V920_INVALID_TAGLEN -> %d\n",csisubframepatternTagLen );               
        return MAC_FAILURE;
    }

     ConfigRequestType = *(msg_p);
      ulCreateReq_p->cqiSubsetConfigRequestType = ConfigRequestType;
     
        msg_p++;  
       
        if ( ConfigRequestType > SETUP)
        {
            lteWarning("Invalid CQI Request Type");
            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid CQI Request Type -> %d\n", ConfigRequestType);               
            /* Review comment fix end RJ13 */
            return MAC_FAILURE;
        }
        if ( ConfigRequestType )
        {
                 tag = LTE_GET_U16BIT(msg_p);
                 if(tag == CSI_MEASUREMENTSUBSET1_VR10 )
                 {
                 msg_p += 2;
		   csimeasurementsubset1TagLen = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (CSI_MEASUREMENTSUBSET1_VR10_TAG_LENGTH  > csimeasurementsubset1TagLen)
                 {      
	            lteWarning("Invalid csimeasurementsubset1TagLen");
	            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid csimeasurementsubset1TagLen -> %d\n", csimeasurementsubset1TagLen);        
	            return MAC_FAILURE;
                 }
         		tag = LTE_GET_U16BIT(msg_p);
                 if(tag == CSI_MEASUREMENT_SUBSET1_FDD )
                 {
                   msg_p += 2;
		   csimeasurementsubset1TagLen = LTE_GET_U16BIT(msg_p);
                   msg_p += 2;
				 
	             if (CSI_MEASUREMENT_SUBSET1_FDD_TAG_LENGTH  > csimeasurementsubset1TagLen)
                     {      
	            lteWarning("Invalid csimeasurementsubset1TagLen");
	            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid csimeasurementsubset1TagLen -> %d\n", csimeasurementsubset1TagLen);        

                      return MAC_FAILURE;
                    }
	           for(subsetIndexNum = 0;subsetIndexNum<5; subsetIndexNum++)
	           {
	           	csiMeasurementSubset1[subsetIndexNum]  = *(msg_p);
		      msg_p++;
	      	   }

	            updateCsiMeasurementSubset(ulCreateReq_p->csiMeasurementSubset1,csiMeasurementSubset1,internalCellIndex);

		 }
	      tag = LTE_GET_U16BIT(msg_p);
               }

		 if(tag == CSI_MEASUREMENTSUBSET2_VR10 )
                 {
                   msg_p += 2;
		   csimeasurementsubset2TagLen = LTE_GET_U16BIT(msg_p);
                   msg_p += 2;
				 
	          if (CSI_MEASUREMENTSUBSET2_VR10_TAG_LENGTH  > csimeasurementsubset2TagLen)
                 {      
	            lteWarning("Invalid csimeasurementsubset2TagLen");
	            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid csimeasurementsubset2TagLen -> %d\n", csimeasurementsubset2TagLen);        
                    return MAC_FAILURE;
                 }
         	tag = LTE_GET_U16BIT(msg_p);
                 if(tag == CSI_MEASUREMENT_SUBSET2_FDD )
                 {
                   msg_p += 2;
		   csimeasurementsubset1TagLen = LTE_GET_U16BIT(msg_p);
                   msg_p += 2;
				 
	             if (CSI_MEASUREMENT_SUBSET2_FDD_TAG_LENGTH  > csimeasurementsubset1TagLen)
                     {      
		            lteWarning("Invalid csimeasurementsubset1TagLen");
		            MAC_EMULATOR_LOG(LOG_INFO,"MAC_PARSING_ERROR:Invalid csimeasurementsubset1TagLen -> %d\n", csimeasurementsubset1TagLen);        

                      return MAC_FAILURE;
                    }
	          
                    for(subsetIndexNum = 0;subsetIndexNum<5; subsetIndexNum++)
	            {
	             csiMeasurementSubset2[subsetIndexNum] = *(msg_p);
			msg_p++;
	      	    }
                    
                  updateCsiMeasurementSubset(ulCreateReq_p->csiMeasurementSubset2,csiMeasurementSubset2,internalCellIndex);
    
	        }
	    }
				 
        }
    *msgBuf = msg_p;
    return MAC_SUCCESS;
}
#endif
/* EICIC -*/

/****************************************************************************
 * Function Name  : parseMACCreateUEInfo
 * Inputs         : msg_p - Pointer to the data received from RRC
 *                  dlUEInfo_p - Ptr to a structure to be filled for DL
 *                  ulUEInfo_p - Ptr to a structure to be filled for UL
 *                  current_p - Ptr to current idx
 *                  uplkLogicalChCount_p - Ptr to uplk Logical Channel Count
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API takes pointer to the msg received as input, parse
 *                  the message and prepare DL & UL structures for UE context
 *                  creation at MAC Layer.
 ****************************************************************************/

UInt16 parseMACCreateUEInfo(UInt8 *msg_p,
        ueContextForDownlink *dlUEInfo_p,
        ueContextForUplink *ulUEInfo_p, UInt16 *current_p,
        UInt16 msgLen, UInt8 cellIndex)
{
    UInt16 current = *current_p;
    UInt16 tag = 0, tagLen = 0, len = 0;
    UInt16 resp   = MAC_SUCCESS;
    UInt32 codingRate = 0;
    UInt8 i;
    UInt16 timeAlignTimer = 0;
    /*CLPC_CHG*/
    TpcPdcchConfig* tpcPdcchConf_p = PNULL;
    DedicatedULPowerControlInfo* dedicatedULPowControlInfo_p = PNULL;
    /*CLPC_CHG*/
    /* + MEAS_GAP_CHG */
    dlUEInfo_p->measurementGapRequestType = FALSE ;
    /* - MEAS_GAP_CHG */
    /* + CQI_4.1 */
    /*Setting Invalid value */
    ulUEInfo_p->cqiformatIndicatorPeriodic = INVALID_CQI_REPORT_MODE;
    dlUEInfo_p->cqiInfoFlag = FALSE;
    /* - CQI_4.1 */
    /* + TM7_8 Changes Start */
    dlUEInfo_p->pmiRiReportV920 = INVALID_PMI_RI_CONF_V920;
    ulUEInfo_p->pmiRiReportV920 = INVALID_PMI_RI_CONF_V920;
    /* - TM7_8 Changes End */
    /* SPS_CHG */

    /* EICIC +*/

     ulUEInfo_p->cqiPMIConfigIndex_2 = MAC_MAX_CQI_PMI_CONFIG_INDEX + 1;
     ulUEInfo_p->riConfigIndex_2 = MAC_RI_CONFIG_INDEX_INVALID;
     ulUEInfo_p->csiMeasurementSubset1[0] = 0;
     ulUEInfo_p->csiMeasurementSubset1[1] = 0;
     ulUEInfo_p->csiMeasurementSubset1[2] = 0;
     ulUEInfo_p->csiMeasurementSubset1[3] = 0;
     ulUEInfo_p->csiMeasurementSubset1[4] = 0;

     ulUEInfo_p->csiMeasurementSubset2[0] = 0;
     ulUEInfo_p->csiMeasurementSubset2[1] = 0;
     ulUEInfo_p->csiMeasurementSubset2[2] = 0;
     ulUEInfo_p->csiMeasurementSubset2[3] = 0;
     ulUEInfo_p->csiMeasurementSubset2[4] = 0;
     ulUEInfo_p->cqiConfigRequestType = INVALID_PERIODIC_CQI_REQ_TYPE;
     ulUEInfo_p->cqiSubsetConfigRequestType = INVALID_PERIODIC_CQI_REQ_TYPE;
     dlUEInfo_p->cqiConfigRequestType = ulUEInfo_p->cqiRequestType;
     dlUEInfo_p->cqiSubsetConfigRequestType = ulUEInfo_p->cqiSubsetConfigRequestType;
     
    /* EICIC -*/
    
    /* Initialize optional parameter with default values */
      dlUEInfo_p->isSpsDlInfoPresent = FALSE;
      dlUEInfo_p->spsDownlinkInfo.requestType = SPS_RELEASE;
      dlUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
          N1PucchAnPersistentListVal.numOfN1PucchAnPersistent = 0;
      dlUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
          isMaxSpsHarqRetxPresent = FALSE;
      dlUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
          isExplicitReleaseAfterPresent = FALSE;
      ulUEInfo_p->isSpsUlInfoPresent = FALSE;
      ulUEInfo_p->spsUplinkInfo.requestType = SPS_RELEASE;
     
      /** For TTIB_Code */
      ulUEInfo_p->ttiBundling = TTIB_DISABLED;

#if 0
      ulUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
           isP0TagPresent = FALSE;
#endif
      /* + SPS_TDD_Changes */
      ulUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
          isTwoIntervalConfigPresent = FALSE;
      /* - SPS_TDD_Changes */

#ifdef UESIM_ENHANCEMENTS
    for(i = 0; i < cellConfigUeSim_g[cellIndex].dlAvailableRBs; i++)
    {
        dlRBInfo_g[cellIndex][i] = RB_CAN_BE_ALLOCATED__FREE;
    }

    for(i = cellConfigUeSim_g[cellIndex].dlAvailableRBs; i < MAX_RBs; i++)
    {
        dlRBInfo_g[cellIndex][i] = RB_OUT_OF_SYSYEM_BANDWIDTH;
    }

    for(i = 0; i < cellConfigUeSim_g[cellIndex].ulAvailableRBs; i++)
    {
        ulRBInfo_g[cellIndex][i] = RB_CAN_BE_ALLOCATED__FREE;
    }

    for(i = cellConfigUeSim_g[cellIndex].ulAvailableRBs; i < MAX_RBs; i++)
    {
        ulRBInfo_g[cellIndex][i] = RB_OUT_OF_SYSYEM_BANDWIDTH;
    }
#endif    

    /* SPS_CHG */ 
    if ( msgLen < (ADD_UE_MANDATORY_LENGTH + 2))
    {
        lteWarning("Mandatory fields missing in CREATE_UE_ENTITY.\n");
        return MAC_SYNTAX_ERROR;
    }

    /* Parse UE Index Allocated for the UE and validate it */
    dlUEInfo_p->ueIndex = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g,"ueIndex parsed for create req = %d\n\n",
            dlUEInfo_p->ueIndex);
    resp = validateUEIndex(dlUEInfo_p->ueIndex);
    if(MAC_SUCCESS != resp)
    {
        lteWarning("Index already allocated for a UE\n");
        return resp;
    }    
    ulUEInfo_p->ueIdx = dlUEInfo_p->ueIndex;
    dlUEInfo_p->dwlkLogicalChCount = 0;
    ulUEInfo_p->numLogicalChannels = 0;
    /* Parse tag, it should be ADD_UE_INFO, else return failure */
    tag = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    if(ADD_UE_INFO != tag)
    {
        return MAC_FAILURE;
    }

    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    len = tagLen;

    if(len < ADD_UE_MANDATORY_LENGTH)
    {
        return MAC_SYNTAX_ERROR;
    }    
    /* Parse CRNTI and validate it */
    dlUEInfo_p->cRnti = LTE_GET_U16BIT(msg_p + current);
    current += 2;
    LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g,"ueId parsed for create req = %d\n\n",
            dlUEInfo_p->cRnti);

    resp = validateUEId(dlUEInfo_p->cRnti, cellIndex);
    if(MAC_SUCCESS != resp)
    {
        lteWarning("UE ID validation fails for create UE request.\n");
        return resp;
    }

    ulUEInfo_p->crnti = dlUEInfo_p->cRnti;
    /* Parse Ue Priority and validate it */ 
    dlUEInfo_p->uePriority = *(msg_p + current);
    current++;
    resp = validateUEPriority(dlUEInfo_p->uePriority);
    if(MAC_SUCCESS != resp)
    {
        return resp;
    }

    /* Parse Num of harq processes and validate it */ 
    dlUEInfo_p->numHarqProcess = *(msg_p + current);
    current++;
#if 0
    dlUEInfo_p->modulationScheme = *(msg_p + current);
#else
    dlUEInfo_p->mcsIndex= *(msg_p + current);
#endif
    current++;
#if 0
    resp = validateModScheme(dlUEInfo_p->modulationScheme);
#else
    //resp = validateModScheme(dlUEInfo_p->mcsIndex);
#endif
    if(MAC_SUCCESS != resp)
    {
        return resp;
    }    
#if 0
    codingRate = LTE_GET_U32BIT(msg_p + current);
    current += 4;
    dlUEInfo_p->codingRate = codingRate;
#endif
    /*DL UE Context Info */
    dlUEInfo_p->maxRB = *(msg_p + current);
    current++;

    /* Uplink UE Context Info */
    ulUEInfo_p->uePriority = dlUEInfo_p->uePriority;

    /* max harq tx changes */
    //ulUEInfo_p->maxRB = *(msg_p + current);
#if 0
    ulUEInfo_p->maxUlHarqTx = *(msg_p + current);
    /* max harq tx changes */

    current++;
    ulUEInfo_p->modulationScheme = *(msg_p + current);
    current++;

    ulUEInfo_p->codingrate = LTE_GET_U32BIT(msg_p + current);
    current += 4;
#else
    ulUEInfo_p->mcsIndex = *(msg_p + current);
    current++;
#endif
    dlUEInfo_p->transmissionMode = *(msg_p + current);
    current++;
    /* + CQI_5.0 */
    dlUEInfo_p->numOfLayer = *(msg_p + current);

    if (MAC_SUCCESS != 
         validateNumOfLayer(dlUEInfo_p->numOfLayer))
    {
        return MAC_SYNTAX_ERROR;
    }
    /* + CQI_5.0 */
    current++;
    dlUEInfo_p->precodingIndex = *(msg_p + current);
    current++;
    //--Anuj-- Added below two lines
#if 1
    ulUEInfo_p->maxUlHarqTx = *(msg_p + current);
    current++;
    
    timeAlignTimer = LTE_GET_U16BIT(msg_p + current);
    current +=2;
    //Anuj End
#endif    
#if 0
    ulUEInfo_p->simultaneousACKNACKAndCQI = *(msg_p + current);
    current++;
#endif

#ifndef CLPC_STUB 
     ulUEInfo_p->pcMax = *(msg_p + current);
     current++;
     if((MIN_PC_MAX > ulUEInfo_p->pcMax)||
        (MAX_PC_MAX < ulUEInfo_p->pcMax))
     {
         lteWarning("Invalid value for pcMax\n");
         return MAC_FAILURE;
     }

     dlUEInfo_p->pA = *(msg_p + current);
     current++;

     if ((dlUEInfo_p->pA < MIN_PA_VALUE) ||
        (dlUEInfo_p->pA > MAX_PA_VALUE))
     {
         lteWarning("Invalid value for p-a\n");
         return MAC_FAILURE;
     }

#endif

    msgLen -= ADD_UE_MANDATORY_LENGTH+2;
    len -= ADD_UE_MANDATORY_LENGTH;
#if 0
    if(len < TAG_LEN )
    {
        lteWarning("mac syntax error.\n");
        return MAC_SYNTAX_ERROR;
    }
#endif
    dlUEInfo_p->tpcPdcchConfigPucch.release = TRUE;
    ulUEInfo_p->tpcPdcchConfigPusch.release = TRUE;

    while (len > 0)
    {
        tag = LTE_GET_U16BIT(msg_p + current);
        current += 2;
        tagLen = LTE_GET_U16BIT(msg_p + current);
        current += 2;
        len -= tagLen;
        msgLen -= tagLen;
        switch (tag)
        {
            case SR_CONFIG_INFO:
                if (tagLen < SR_CONFIG_LEN)
                {
                    return MAC_FAILURE;
                }
                ulUEInfo_p->requestType = *(msg_p+current);
                current++; // request Type parsmeter
                if (  ulUEInfo_p->requestType == 0 )
                {
                    current += SR_SETUP_INFO_LEN;
                    continue;
                }
                tagLen -= SR_CONFIG_LEN;
                ulUEInfo_p->srconfigflag = TRUE;
                if(tagLen != SR_SETUP_INFO_LEN)
                {
                    lteWarning("mac syntax length error .\n");
                    return MAC_SYNTAX_ERROR;
                }
                tag = LTE_GET_U16BIT(msg_p + current);
                current += 2;
                if (tag != SR_SETUP_INFO)
                {
                    lteWarning("wrong tag received.\n");
                    return MAC_TAG_NOT_EXISTS;
                }
                tagLen = LTE_GET_U16BIT(msg_p + current);
                current += 2;
                if(tagLen != SR_SETUP_INFO_LEN)
                {
                    lteWarning("mac syntax length error .\n");
                    return MAC_FAILURE;
                }
                ulUEInfo_p->srPucchResourceIndex = LTE_GET_U16BIT(msg_p + current);
                current += 2;
                ulUEInfo_p->srConfigurationIndex = *(msg_p + current);
                current++;
                ulUEInfo_p->srSetupflag = TRUE;

                break;
            case CQI_INFO:

                /* + CQI_4.1 */
                /* to check if optional fields CQI tag are present */
                /* + SPR 7615 Fix */
                if ( tagLen > 93 || tagLen < 9 )  /* EICIC +-*/
                {
                /* - SPR 7615 Fix */
                    lteWarning("CQI INFO TAG Len Invalid [%d]", tagLen);
                    return MAC_FAILURE;
                }

                tag = LTE_GET_U16BIT(msg_p + current);
                /* + CQI_4.1 */
                dlUEInfo_p->cqiInfoFlag = TRUE;
                /* - CQI_4.1 */

                if ( CQI_APERIODIC_INFO == tag )
                {
                    /*skip the length of the CQI Aperiodic Info tag*/
                    current += 2;
                     
                    if (8 > dlUEInfo_p->maxRB)
                    {
                        lteWarning("Aperiodic mode configured with DL "
                                    "RBs are less than 8\n");
                        return MAC_FAILURE;
                    }    
                    /*Skip the length of aperiodic tag */
                    current += 2 ;
                    ulUEInfo_p->cqiMode = *(msg_p + current);
                    current++; 
                    dlUEInfo_p->cqiMode = ulUEInfo_p->cqiMode;
                    if ((dlUEInfo_p->cqiMode != CQI_MODE_1_2)&&
                            (dlUEInfo_p->cqiMode != CQI_MODE_2_0)&&
                            (dlUEInfo_p->cqiMode != CQI_MODE_2_2)&&
                            (dlUEInfo_p->cqiMode != CQI_MODE_3_0)&&
                            (dlUEInfo_p->cqiMode != CQI_MODE_3_1))
                    {
                        lteWarning(" Invalid Aperiodic CQI Mode");
                        return MAC_FAILURE;
                    }

                    /* Periodic CQI Info is present */
                    if ( tagLen == 14 || tagLen <= 35 )
                    {
                        tag = LTE_GET_U16BIT(msg_p + current);
                    }
                }
                if ( CQI_PERIODIC_INFO == tag )
                {
                    UInt32 cqiPeriodicTagLen = 0;
                    UInt32 cqiRequestType = 0;
                    current += 2;

                    /* cqi periodic tag length*/
                    cqiPeriodicTagLen = LTE_GET_U16BIT(msg_p + current);
                    current += 2;

                    cqiRequestType = *(msg_p + current);
                    current++;

                    if ( cqiRequestType > 1)
                    {
                        lteWarning("Invalid CQI Request Type");
                        return MAC_FAILURE;
                    }    
                    if ( cqiRequestType )
                    {
                        if ( cqiPeriodicTagLen < PERIODIC_CQI_SETUP_LEN )
                        {
                            lteWarning("cqiPeriodicTagLen < PERIODIC_CQI_SETUP_LEN");
                            return resp;
                        }
                        tag = LTE_GET_U16BIT(msg_p + current);
                        current += 2;

                        if ( CQI_PMI_CONFIG_INDEX_INFO == tag)
                        {
                            cqiPeriodicTagLen = LTE_GET_U16BIT(msg_p + current);
                            current += 2;

                            if ( cqiPeriodicTagLen < PERIODIC_CQI_LEN )  
                            {
                                lteWarning("cqiPeriodicTagLen < PERIODIC_CQI_LEN");
                                return MAC_FAILURE;
                            }
                            ulUEInfo_p->cqiPMIConfigIndex =  LTE_GET_U16BIT(msg_p + current);
                            current += 2;
                            /* + FDD Specific validation */
                            if ( ( ulUEInfo_p->cqiPMIConfigIndex > 
                                    MAC_MAX_CQI_PMI_CONFIG_INDEX ) ||
                               ( ulUEInfo_p->cqiPMIConfigIndex == 
                                    MAC_MAX_CQI_PMI_CONFIG_INDEX_3_1_7 ) )  
                            {
                                lteWarning("Invalid range for cqiPMIConfigIndex \n");
                                return MAC_FAILURE;
                            }        
                            /* - FDD Specific validation */

                            ulUEInfo_p->cqiPucchResourceIndex = LTE_GET_U16BIT(msg_p + current);
                            current += 2;
                            if (ulUEInfo_p->cqiPucchResourceIndex > 
                                    MAC_MAX_CQI_PUCCH_RESOURCE_INDEX)
                            {
                                lteWarning("Invalid range for cqiPucchResourceIndex \n");
                                return MAC_FAILURE;
                            }     

                            ulUEInfo_p->simultaneousACKNACKAndCQI = *(msg_p + current);
                            if ( ( ulUEInfo_p->simultaneousACKNACKAndCQI != 
                                        SIMULTANEOUS_ACK_NACK_CQI_0) &&
                                 ( ulUEInfo_p->simultaneousACKNACKAndCQI != 
                                   SIMULTANEOUS_ACK_NACK_CQI_1) )
                            {
                                lteWarning("Invalid range for simultaneousACKNACKAndCQI \n");
                                return MAC_FAILURE;
                            }  
                            current++;
                            ulUEInfo_p->cqiformatIndicatorPeriodic = *(msg_p + current);
                            current++;
                            tag = LTE_GET_U16BIT(msg_p + current);
                            /* + Bandwidth Fix */
                            if(ulUEInfo_p->cqiformatIndicatorPeriodic)
                            {
                                if (8 > cellConfigUeSim_g[cellIndex].dlAvailableRBs)
                                {
                                    lteWarning("Periodic Subband mode configured with DL "
                                            "RBs are less than 8, dlUEInfo_p->maxRB = %d\n",cellConfigUeSim_g[cellIndex].dlAvailableRBs);
                                    return MAC_FAILURE;
                                }
                            }
                            /* - Bandwidth Fix */
                            /*Check if Report FORMAT is SUBBAND*/
                            if(CQI_FORMAT_PERIODIC_SUBBAND_K == tag)     
                            {    
                                current += 2;
                                tagLen = LTE_GET_U16BIT(msg_p + current);
                                current += 2;
                                ulUEInfo_p->cqiformatIndicatorPeriodic_subband_k = *(msg_p + current);
                                current += 1;
                            }
                            else
                            {
                                if(ulUEInfo_p->cqiformatIndicatorPeriodic)
                                {
                                    return MAC_FAILURE;
                                }    
                            }    

                            tag = LTE_GET_U16BIT(msg_p + current);
                            if (RI_CONFIG_INDEX_INFO == tag) 
                            { 
                                current += 2;
                                tagLen = LTE_GET_U16BIT(msg_p + current);
                                current += 2;
                                ulUEInfo_p->riConfigIndex = LTE_GET_U16BIT(msg_p + current);
                                current += 2;
                                if (ulUEInfo_p->riConfigIndex > MAC_MAX_RI_CONFIG_INDEX)
                                {
                                    lteWarning("Invalid range for riConfigIndex \n");
                                    return MAC_FAILURE;
                                }
                                /* + CQI_4.1 */
                                else if( (3 != dlUEInfo_p->transmissionMode &&
                                          4 != dlUEInfo_p->transmissionMode) && 
                                /* - CQI_4.1 */
                                /* + TM7_8 Changes Start */
                                        (8 != dlUEInfo_p->transmissionMode) )
                                /* - TM7_8 Changes End */
                                {
                                    ulUEInfo_p->riConfigIndex = MAC_RI_CONFIG_INDEX_INVALID;
                                    lteWarning("riConfigIndex is configured in TX mode "
                                            "other than 3 and 4.\n");
                                }
                            }    
                        }   
                        else if (CQI_PMI_CONFIG_INDEX_INFO_v1020 == tag) /* EICIC +*/
                        {
                            cqiPeriodicTagLen = LTE_GET_U16BIT(msg_p + current);
                            current += 2;

                            if ( cqiPeriodicTagLen < PERIODIC_CQI_LEN )  
                            {
                                lteWarning("cqiPeriodicTagLen < PERIODIC_CQI_LEN");
                                return MAC_FAILURE;
                            }
                            ulUEInfo_p->cqiPMIConfigIndex =  LTE_GET_U16BIT(msg_p + current);
                            current += 2;
                            /* + FDD Specific validation */
                            if ( ( ulUEInfo_p->cqiPMIConfigIndex > 
                                    MAC_MAX_CQI_PMI_CONFIG_INDEX ) ||
                               ( ulUEInfo_p->cqiPMIConfigIndex == 
                                    MAC_MAX_CQI_PMI_CONFIG_INDEX_3_1_7 ) )  
                            {
                                lteWarning("Invalid range for cqiPMIConfigIndex \n");
                                return MAC_FAILURE;
                            }        
                            /* - FDD Specific validation */

                            ulUEInfo_p->cqiPucchResourceIndex = LTE_GET_U16BIT(msg_p + current);
                            current += 2;
                            if (ulUEInfo_p->cqiPucchResourceIndex > 
                                    MAC_MAX_CQI_PUCCH_RESOURCE_INDEX)
                            {
                                lteWarning("Invalid range for cqiPucchResourceIndex \n");
                                return MAC_FAILURE;
                            }     

                            ulUEInfo_p->simultaneousACKNACKAndCQI = *(msg_p + current);
                            if ( ( ulUEInfo_p->simultaneousACKNACKAndCQI != 
                                        SIMULTANEOUS_ACK_NACK_CQI_0) &&
                                 ( ulUEInfo_p->simultaneousACKNACKAndCQI != 
                                   SIMULTANEOUS_ACK_NACK_CQI_1) )
                            {
                                lteWarning("Invalid range for simultaneousACKNACKAndCQI \n");
                                return MAC_FAILURE;
                            }  
                            current++;
                            ulUEInfo_p->cqiformatIndicatorPeriodic = *(msg_p + current);
                            current++;
                            tag = LTE_GET_U16BIT(msg_p + current);
                            /* + Bandwidth Fix */
                            if(ulUEInfo_p->cqiformatIndicatorPeriodic)
                            {
                                if (8 > cellConfigUeSim_g[cellIndex].dlAvailableRBs)
                                {
                                    lteWarning("Periodic Subband mode configured with DL "
                                            "RBs are less than 8, dlUEInfo_p->maxRB = %d\n",cellConfigUeSim_g[cellIndex].dlAvailableRBs);
                                    return MAC_FAILURE;
                                }
                            }
                            /* - Bandwidth Fix */
                            /*Check if Report FORMAT is SUBBAND*/
                            if(CQI_FORMAT_PERIODIC_SUBBAND_K == tag)     
                            {    
                                current += 2;
                                tagLen = LTE_GET_U16BIT(msg_p + current);
                                current += 2;
                                ulUEInfo_p->cqiformatIndicatorPeriodic_subband_k = *(msg_p + current);
                                current += 1;
                            }
                            else
                            {
                                if(ulUEInfo_p->cqiformatIndicatorPeriodic)
                                {
                                    return MAC_FAILURE;
                                }    
                            }    

                            tag = LTE_GET_U16BIT(msg_p + current);
                            if (RI_CONFIG_INDEX_INFO == tag) 
                            { 
                                current += 2;
                                tagLen = LTE_GET_U16BIT(msg_p + current);
                                current += 2;
                                ulUEInfo_p->riConfigIndex = LTE_GET_U16BIT(msg_p + current);
                                current += 2;
                                if (ulUEInfo_p->riConfigIndex > MAC_MAX_RI_CONFIG_INDEX)
                                {
                                    lteWarning("Invalid range for riConfigIndex \n");
                                    return MAC_FAILURE;
                                }
                                /* + CQI_4.1 */
                                else if( (3 != dlUEInfo_p->transmissionMode &&
                                          4 != dlUEInfo_p->transmissionMode) && 
                                /* - CQI_4.1 */
                                /* + TM7_8 Changes Start */
                                        (8 != dlUEInfo_p->transmissionMode) )
                                /* - TM7_8 Changes End */
                                {
                                    ulUEInfo_p->riConfigIndex = MAC_RI_CONFIG_INDEX_INVALID;
                                    lteWarning("riConfigIndex is configured in TX mode "
                                            "other than 3 and 4.\n");
                                }
                                tag = LTE_GET_U16BIT(msg_p + current);
                            } 
                            if (CSI_CONFIG_INDEX_VR10 == tag) 
				{
					if (MAC_SUCCESS != parseCreateUECqiConfigIndexV10Info (&msg_p, 
								ulUEInfo_p, dlUEInfo_p 
								/* CA changes Start */
								, cellIndex
								/* CA Changes end */
								))
					{
						return MAC_FAILURE;
					}
		                   tag = LTE_GET_U16BIT(msg_p + current);
				}
                          } /* EICIC -*/
                        }
                    }

             
                /* + SPR 7615 Fix */
                tag = LTE_GET_U16BIT(msg_p + current);
                /* - SPR 7615 Fix */
                /* - CQI_4.1 */
                /* + TM7_8 Changes Start */
                if (CQI_REPORT_CONFIG_V920 == tag) 
                {
                    UInt16 cqiTagLenV920 = 0;
                    current += 2;

                    /* cqi report v920 tag length*/
                    cqiTagLenV920 = LTE_GET_U16BIT(msg_p + current);
                    current += 2;
                    
                    if ( CQI_REPORT_V920_LEN > cqiTagLenV920)
                    {
                        return MAC_FAILURE;
                    }
                    tag = LTE_GET_U16BIT(msg_p + current);
                    current += 2;

                    if (CQI_MASK_V920 == tag)
                    {
                       tagLen = LTE_GET_U16BIT(msg_p + current);
                       current += 2;
                       ulUEInfo_p->cqiMaskV920 = *(msg_p + current);
                       if (ulUEInfo_p->cqiMaskV920 != TRUE && 
                               ulUEInfo_p->cqiMaskV920 != FALSE )
                       {
                           return MAC_FAILURE;
                       }
                       tag = LTE_GET_U16BIT(msg_p + current);
                    }
                    
                    if (PMI_RI_REPORT_V920 == tag)
                    {
                        current += 2;
                        tagLen = LTE_GET_U16BIT(msg_p + current);
                        current += 2;
                        ulUEInfo_p->pmiRiReportV920 = *(msg_p + current);
                        if (ulUEInfo_p->pmiRiReportV920 != TRUE && 
                                ulUEInfo_p->pmiRiReportV920 != FALSE )
                        {
                            return MAC_FAILURE;
                        }
                        dlUEInfo_p->pmiRiReportV920 = ulUEInfo_p->pmiRiReportV920;
                    }

                   tag = LTE_GET_U16BIT(msg_p + current);
                }
                /* Rel9_upgrade_CR396 */
                /* - TM7_8 Changes End */

		/* EICIC +*/
		if (CSI_SUBFRAME_PATTERN_CONFIG_VR10 == tag) 
		{
			if (MAC_SUCCESS != parseCreateUECsiSubframePatternV10Info (&msg_p, 
						ulUEInfo_p, dlUEInfo_p 
						, cellIndex
						))
			{
				return MAC_FAILURE;
			}
		}

		/* EICIC -*/
		break;

#ifdef TDD_CONFIG
            case TDD_ACK_NACK_FEEDBACK_MODE_INFO:
                if(tagLen != TDD_ACK_NACK_MODE_TLV_LEN)
                {
                    lteWarning("mac syntax length error .\n");
                    return MAC_FAILURE;
                }

                ulUEInfo_p->dlAckNackMode = *(msg_p + current);
                current++;

                if((0 != ulUEInfo_p->dlAckNackMode) &&
                        (1 != ulUEInfo_p->dlAckNackMode))
                {
                    lteWarning("Invalid TddAckNack Mode received\n");
                    return MAC_FAILURE;
                }
                break;
#endif
            case UE_CATEGORY:
                {
                    if ( tagLen != 5 )
                    {
                        lteWarning("Invalid Tag Length %d for UE_CATEGORY  Info\n"
                                ,tagLen);
                        fprintf(stderr,"Invalid Tag Length %d for UE_CATEGORY  Info\n"
                                ,tagLen);
                        return MAC_FAILURE;
                    }
                    if (*msg_p > 5 || *msg_p < 1)
                    {
                        fprintf(stderr,"**** UE_CATEGORY[%d] Returning Failure*****\n",*msg_p);
                        lteWarning("Invalid range for UeCategory .\n");
                        return MAC_FAILURE;
                    }
                    dlUEInfo_p->ueCategory = *msg_p++;
                    ulUEInfo_p->ueCategory = dlUEInfo_p->ueCategory;
                    break;
                }

            /* + CL-MIMO RSM*/
            case CODEBOOK_SUBSET_RESTRICTION:
                {
                    //MAC_EMULATOR_LOG(LOG_INFO,"\n Codebook subset restriction [%d]\n",tagLen);
                    if ( tagLen != 13 )
                    {
                        lteWarning("Invalid Tag Length for codebookSubsetRestriction in MAC_CREATE_UE_ENTITY_REQ\n");
                        return MAC_FAILURE;
                    }
                    /* Codebook subset restriction is supported for
                     *                  *  transmission modes 3, 4, 5 and 6
                     *                                   */
                    if((3 == dlUEInfo_p->transmissionMode)||
                            (4 == dlUEInfo_p->transmissionMode)||
                            (5 == dlUEInfo_p->transmissionMode)||
                            (6 == dlUEInfo_p->transmissionMode)
                      )
                    {
                        ulUEInfo_p->codeBookSubsetfromRRC.cbsrType = *(msg_p + current);
                        current++;
                        for (i = 0; i < MAX_CBSR_SIZE; i++)
                        {
                            ulUEInfo_p->codeBookSubsetfromRRC.cbsrValue[i] = *(msg_p + current);
                            current++;
                        }
                    }
                    else
                    {
                        lteWarning("codebookSubsetRestriction received for \
                                transmission mode other than 3,4,5,6 in \
                                MAC_CREATE_UE_ENTITY_REQ\n");
                        current = current +MAX_CBSR_SIZE +1;
                    }
                    if ((resp = validateCBSR(&ulUEInfo_p->codeBookSubsetfromRRC
									,dlUEInfo_p->transmissionMode,
                                    /*4*4 MIMO CHG START*/
                                    dlUEInfo_p->rankIndicator,
                                    /*4*4 MIMO CHG START*/
                                    cellIndex) != MAC_SUCCESS))
                    {
                        lteWarning("Atleast one bit should be 1 in cbsrValue in MAC_CREATE_UE_ENTITY_REQ\n");
                        return resp;
                    }
                }
                break;
            /* - CL-MIMO RSM*/
            
             /* + DRX_CHG */
             case  DRX_CONFIG_INFO:
             {
                  resp =  parseDRXConfigInfo(&msg_p,
                                             tagLen,
                                             dlUEInfo_p);

                  if( MAC_FAILURE == resp )
                  {
                      return  MAC_FAILURE;
                  }
             }
             break;
            /* - DRX_CHG */
            
            /* + MEAS_GAP_CHG */
            case  MEAS_GAP_CONFIG_INFO:
            {
                  resp =  parseMeasGapConfigInfo(&msg_p,
                                                 tagLen,
                                                 dlUEInfo_p);
                
                  if( MAC_FAILURE == resp )
                  {
                      return  MAC_FAILURE;
                  }
                  dlUEInfo_p->measurementGapRequestType = TRUE;
            }
            break;

          case CQI_MASK_INFO:
          {
                               
          }      
        /* -  MEAS_GAP_CHG */
#if 1
            // Anuj Start : Missing cases as per API ver3.8
            case PUSCH_CONFIG_INFO:
            case SRS_DEDICATED_CONFIG:
            {
                //msg_p = msg_p + tagLen;
                current = current + (tagLen - TAG_LEN);
                continue;

            }
            break;
#endif
            /*CLPC_CHG*/
            case TPC_PDCCH_CONFIG_PUCCH:
            if((tagLen - TAG_LEN) < TPC_PDCCH_CONFIG_PARAM_LENGTH)
            {
                lteWarning("Mandatory Parameters missing\n");
                return MAC_FAILURE;
            }
            tpcPdcchConf_p = &(dlUEInfo_p->tpcPdcchConfigPucch);
            tpcPdcchConf_p->release = *(msg_p + current);
            current ++;
            if((FALSE != tpcPdcchConf_p->release) &&
                (TRUE != tpcPdcchConf_p->release))
            {
                return MAC_FAILURE;
            }
            if (!tpcPdcchConf_p->release)
            {
                resp = parseTpcPdcchConfig(msg_p,
                        tagLen,
                        current,
                        tpcPdcchConf_p,
                        FALSE);
                if(resp == MAC_FAILURE)
                {
                    return MAC_FAILURE;
                }
            }
            current = current + (tagLen - TAG_LEN - 1);
            if(tpcRntiInfoMap[(dlUEInfo_p->tpcPdcchConfigPucch.tpcRnti) - START_TPC_RNTI]
                .isValid[dlUEInfo_p->tpcPdcchConfigPucch.tpcBitMapIndex] == 
                    FALSE)
            {
                tpcRntiInfoMap[(dlUEInfo_p->tpcPdcchConfigPucch.tpcRnti) - START_TPC_RNTI]
                    .ueIndex[dlUEInfo_p->tpcPdcchConfigPucch.tpcBitMapIndex] = 
                    dlUEInfo_p->ueIndex;
                tpcRntiInfoMap[(dlUEInfo_p->tpcPdcchConfigPucch.tpcRnti) - START_TPC_RNTI]
                    .isValid[dlUEInfo_p->tpcPdcchConfigPucch.tpcBitMapIndex] = 
                    TRUE;
                tpcRntiInfoMap[(dlUEInfo_p->tpcPdcchConfigPucch.tpcRnti) - START_TPC_RNTI]
                    .configuredCount++;
            }
            break;

            case TPC_PDCCH_CONFIG_PUSCH:
            if((tagLen - TAG_LEN) < TPC_PDCCH_CONFIG_PARAM_LENGTH)
            {
                lteWarning("Mandatory Parameters missing\n");
                return MAC_FAILURE;
            }
            tpcPdcchConf_p = &(ulUEInfo_p->tpcPdcchConfigPusch);
            tpcPdcchConf_p->release = *(msg_p + current);
            current ++;
            if((FALSE != tpcPdcchConf_p->release) &&
                (TRUE != tpcPdcchConf_p->release))
            {
                return MAC_FAILURE;
            }
            if (!tpcPdcchConf_p->release)
            {
                resp = parseTpcPdcchConfig(msg_p,
                        tagLen,
                        current,
                        tpcPdcchConf_p,
                        TRUE);
                if(MAC_FAILURE == resp)
                {
                    return MAC_FAILURE;
                }
            }
            current = current + (tagLen - TAG_LEN - 1);
            if(tpcRntiInfoMap[ulUEInfo_p->tpcPdcchConfigPusch.tpcRnti - START_TPC_RNTI].
                isValid[ulUEInfo_p->tpcPdcchConfigPusch.tpcBitMapIndex] == 
                    FALSE)
            {
                tpcRntiInfoMap[ulUEInfo_p->tpcPdcchConfigPusch.tpcRnti - START_TPC_RNTI].
                    ueIndex[ulUEInfo_p->tpcPdcchConfigPusch.tpcBitMapIndex] = 
                    ulUEInfo_p->ueIdx;
                tpcRntiInfoMap[ulUEInfo_p->tpcPdcchConfigPusch.tpcRnti - START_TPC_RNTI].
                    isValid[ulUEInfo_p->tpcPdcchConfigPusch.tpcBitMapIndex] = 
                    TRUE;
                tpcRntiInfoMap[ulUEInfo_p->tpcPdcchConfigPusch.tpcRnti - START_TPC_RNTI].
                    configuredCount++;
            }
            break;

            case UPLINK_POWER_CONTROL_DEDICATED:
            if((tagLen - TAG_LEN) < ULP_CONTROL_DEDICATED_PARAM_LEN)
            {
                lteWarning("Mandatory Parameters missing\n");
                return MAC_FAILURE;
            }
            dedicatedULPowControlInfo_p = &(ulUEInfo_p->dedicatedULPowerControlInfo);
            resp =  parseDedicatedULPowerControlInfo(msg_p,
                    tagLen,
                    current,
                    dedicatedULPowControlInfo_p);
            if(MAC_FAILURE == resp)
            {
                return MAC_FAILURE;
            }
            current = current + ULP_CONTROL_DEDICATED_PARAM_LEN;
            break;
            /* HD FDD Changes Start */
            case RF_PARAMS:
            {
              /* Checking if tagLen is minimum of RF_PARAMS_LENGTH */
              if(tagLen < RF_PARAMS_LENGTH)
              {
                  fprintf(stderr,"Invalid length for RF_PARAMS");
                  return MAC_FAILURE;
              }
              //msg_p++;
#ifdef HD_FDD_CONFIG
              if((FALSE != ulUEInfo_p->isHDFddFlag) &&
                 (TRUE != ulUEInfo_p->isHDFddFlag))
              {
                  fprintf(stderr,"Invalid Duplexing mode. Value = %d\n",
                          ulUEInfo_p->isHDFddFlag);
                  return MAC_FAILURE;
              }
              else if (TRUE == ulUEInfo_p->isHDFddFlag)
              {
                  fprintf(stderr,"ue index of HD-FDD UE is = %d",ulUEInfo_p->ueIdx );
              }
              //ulUEInfo_p->isHDFddFlag = *msg_p;
              //dlUEInfo_p->isHDFddFlag = *msg_p;
              ulUEInfo_p->isHDFddFlag = *(msg_p + current);
              dlUEInfo_p->isHDFddFlag = *(msg_p + current);
#endif
              current++;
            }
            break;
            /* HD FDD Changes End*/
                /* SPS_CHG */
                case  SPS_CONFIG:
                {
                    UInt8 *message_p = PNULL;
                    message_p = msg_p + current;
                    resp =  parseSpsConfigInfo(message_p, 
                                           tagLen,
                                           dlUEInfo_p,
                                           ulUEInfo_p);
                    if( MAC_FAILURE == resp )
                    {
                        return MAC_FAILURE;
                    }
                    current += (tagLen - TAG_LEN);
                    break;
                }  


                case TTI_BUNDLING_INFO :
                {
                    if (TTI_BUNDLING_INFO_TAG_LEN != tagLen )
                    {
                        lteWarning("Invalid Tag Length %d for TTI_BUNDLING_Info\n"
                                    ,tagLen);
                        return MAC_FAILURE;
                    }
    

#ifdef FDD_CONFIG
                   /** For TTIB_Code */
                    ulUEInfo_p->ttiBundling = *(msg_p+current);
                    if ((ulUEInfo_p->ttiBundling != TTIB_DISABLED) &&
                            (ulUEInfo_p->ttiBundling != TTIB_ENABLED))
                    {
                        lteWarning("Invalid Value for ttiBundling Info :%d \n",
                        ulUEInfo_p->ttiBundling);
                        return MAC_FAILURE;
                    }
#endif
                    current++;
    
                    break;
                }
				/*+ CA_PHASE2 */
            case UE_CATEGORY_V1020:
				{
					if (tagLen != UE_CATEGORY_TAG_LEN)
					{
						lteWarning("Mandatory fields missing in UE_CATEGORY_V1020_LEN.\n");
						return MAC_SYNTAX_ERROR;
					}
					dlUEInfo_p->ueCategoryV1020 = *msg_p++;
					ulUEInfo_p->ueCategoryV1020 = dlUEInfo_p->ueCategoryV1020 ;
					dlUEInfo_p->ueCategoryReqType = TRUE ;
					ulUEInfo_p->ueCategoryReqType = TRUE ;
					resp = macValidateUeCategoryV1020(dlUEInfo_p->ueCategoryV1020);
					if (resp != MAC_SUCCESS)
					{
						return resp;
					}
					break;
				}
            case PUCCH_FORMAT_V1020:
				{
					resp = parseReconfigPUCCHFormatV1020Info(ulUEInfo_p, dlUEInfo_p, 
							tagLen, &msg_p, cellIndex);
					if (resp != MAC_SUCCESS)
					{
						return resp;
					}
					break;
				}		
				
            case CA_CONFIG:
				{
					resp = parseReconfigCAConfigInfo(ulUEInfo_p, dlUEInfo_p, tagLen, &msg_p, cellIndex);
					if (resp != MAC_SUCCESS)
					{
						return resp;
					}
					break;
				}

				/*- CA_PHASE2 */

                /* SPS_CHG */
            default :
            /*If a TLV is not found, just skip that TLV and move to 
             * parsing of rest of TLVs*/
            /*CLPC_CHG*/
            current = current + (tagLen - TAG_LEN);
            lteWarning("Invalid tag received while parsing UE create Info\n");
        } 
    }
   /* + TM7_8 Changes Start */


    if((8 == dlUEInfo_p->transmissionMode) &&
        (TRUE != dlUEInfo_p->pmiRiReportV920))
        {
            ulUEInfo_p->riConfigIndex = 1000;
            lteWarning("PmiRiReport is not configured in TX mode 8. \n");
        }
    
    /* - TM7_8 Changes End */


    /* SPR 1372 FIX */
    if (TRUE != ulUEInfo_p->srconfigflag || TRUE != ulUEInfo_p->srSetupflag)
    {
        lteWarning("Either sr config not passed or not setup, attach will not work, hence returning failure\n");
        return MAC_FAILURE;
    }
    /* SPR 1372 FIX */

    if (msgLen == 0)
    {
        return MAC_SUCCESS;
    }
    /* to check if optional fields CREATE_LC_REQ tag are present */
    while ( msgLen > 0 )
    {
        if (msgLen < TAG_LEN)
        {
            lteWarning("mac syntax error.\n");
            return MAC_SYNTAX_ERROR;
        }
    tag = LTE_GET_U16BIT(msg_p + current);
    current += 2;
        if (tag != CREATE_LC_REQ)
        {
            lteWarning("wrong tag received.\n");
            return MAC_TAG_NOT_EXISTS;
        }
    tagLen = LTE_GET_U16BIT(msg_p + current);
    current += 2;
        msgLen -= tagLen;

        if(tagLen < CREATE_LC_REQ_MIN_LEN)
        {
            lteWarning("mac syntax length error .\n");
            return MAC_SYNTAX_ERROR;
        }
        resp = parseCreateLcReq(ulUEInfo_p, dlUEInfo_p,
                tagLen, &current, msg_p, cellIndex);
        if (resp != MAC_SUCCESS)
        {
            lteWarning("parsing create LC req failed.\n");
            return MAC_FAILURE;
        }

    }  
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseMACReconfigUEInfo
 * Inputs         : msg_p - Pointer to the data received from RRC
 *                  dlUEInfo_p - Ptr to a structure to be filled for DL
 *                  ulUEInfo_p - Ptr to a structure to be filled for UL
 *                  current_p - Ptr to current idx
 *                  uplkLogicalChCount_p - Ptr to uplk Logical Channel Count
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This API takes pointer to the msg received as input, parse
 *                  the message and prepare DL & UL structures for UE context
 *                  reconfiguration at MAC Layer (inc UE Info & LCs).
 ****************************************************************************/
/* + CA_AB */
UInt16 parseMACReconfigUEInfo(UInt8 *msg_p,
        ueContextForDownlink *dlUEInfo_p,
        ueContextForUplink *ulUEInfo_p, 
        UInt16 totalMsgLen, UInt16 *current_p, UInt8 cell_index)
{
    UInt16 tag = 0, tagLen = 0;
    UInt16 resp = MAC_SUCCESS;
    UInt16 remLen = totalMsgLen;;
    UInt16 current = *current_p;
    UInt8 *msgBuf_p = msg_p;
    /* SPS_CHG */
    /* Initialize optional parameter with default values */
      dlUEInfo_p->isSpsDlInfoPresent = FALSE;
      dlUEInfo_p->spsDownlinkInfo.requestType = SPS_RELEASE;
      dlUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
          N1PucchAnPersistentListVal.numOfN1PucchAnPersistent = 0;
      dlUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
          isMaxSpsHarqRetxPresent = FALSE;
      dlUEInfo_p->spsDownlinkInfo.spsDownlinkSetupInfo.\
          isExplicitReleaseAfterPresent = FALSE;
      ulUEInfo_p->isSpsUlInfoPresent = FALSE;
      ulUEInfo_p->spsUplinkInfo.requestType = SPS_RELEASE;
#if 0
      ulUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
           isP0TagPresent = FALSE;
#endif
      /* + SPS_TDD_Changes */
      ulUEInfo_p->spsUplinkInfo.spsUplinkSetupInfo.\
          isTwoIntervalConfigPresent = FALSE;
      /* - SPS_TDD_Changes */
    /* SPS_CHG */ 

    msg_p += current;

    if (totalMsgLen < RECONFIG_UE_ENTITY_MIN_LEN)
    {
        return MAC_SYNTAX_ERROR;
    }

    /* Parse ueIndex and validate it */
    //dlUEInfo_p->ueIndex = (*msg_p | (*(msg_p + 1) << SHIFT_EIGHT_BITS));
    dlUEInfo_p->ueIndex = LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    current += 2;

    ulUEInfo_p->ueIdx = dlUEInfo_p->ueIndex;

    remLen -= RECONFIG_UE_ENTITY_MIN_LEN;

    resp = validateUEIndex(dlUEInfo_p->ueIndex);
    if (MAC_SUCCESS != resp) 
    {
        return MAC_FAILURE;
    }    

    dlUEInfo_p->dwlkLogicalChCount = 0;
    ulUEInfo_p->numLogicalChannels = 0;

    if (remLen < TAG_LEN)
    {
        lteWarning(
                "Atleast one optional tag should be present for RECONFIG_UE_ENTITY Req .\n");
        return MAC_FAILURE;
    }

    while (remLen > 0)
    {
        //tag = (*msg_p | (*(msg_p + 1) << SHIFT_EIGHT_BITS));
        tag = LTE_GET_U16BIT(msg_p);
        msg_p += 2;
        current += 2;
        //tagLen = (*msg_p | (*(msg_p + 1) << SHIFT_EIGHT_BITS));
        tagLen = LTE_GET_U16BIT(msg_p);
        msg_p += 2;
        current += 2;
        
        remLen -= tagLen;
        switch ( tag )
        {
            case CREATE_LC_REQ:
                resp =
                    parseCreateLcReq( ulUEInfo_p, dlUEInfo_p, tagLen, &current, msgBuf_p, cell_index );
                    msg_p += tagLen - 4;
                break;

            case RECONFIGURE_LC_REQ:
                resp =
                    parseReconfigLcReq( ulUEInfo_p,dlUEInfo_p, tagLen, msg_p, cell_index );
                    msg_p += tagLen - 4;
                    current += tagLen - 4;
                break;

            case DELETE_LC_REQ:
                resp = parseDeleteLcReq( ulUEInfo_p, dlUEInfo_p, tagLen, msg_p );
                msg_p += tagLen - 4;
                current += tagLen - 4;
                break;             

            case RECONFIG_UE_INFO:
                resp = parseReconfigUEInfo( dlUEInfo_p, msg_p, tagLen,
                        ulUEInfo_p, cell_index);
                msg_p += tagLen -4;
                current += tagLen - 4;
                break;

            default:
                lteWarning("Invalid Tag received from RRC Layer\n");
                break;
        }
        if (resp != MAC_SUCCESS)
        {
            return MAC_SYNTAX_ERROR;
        }    
    }       

    return MAC_SUCCESS;
}
/* - CA_AB */

/* EICIC +*/


/******************************************************************************
 * Function Name  : parseReconfigUECsiSubframePatternV10Info 
 * Inputs         : msgBuf - pointer to the input buffer
 *                  ulCreateReq_p - pointer to Uplink create UE structure
 *                  dlCreateReq_p - pointer to Downlink create UE structure 
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function parse the csisubframepatternconfigv10 information in 
 *                  Create UE Entity Request and fills in the structure needed  
 *                  by MAC to create a UE context.
 ******************************************************************************/
STATIC inline MacRetType parseReconfigUECsiSubframePatternV10Info( UInt8 **msgBuf,
        ueContextForUplink *ulCreateReq_p, ueContextForDownlink *dlCreateReq_p 
        /* CA changes Start */
        ,InternalCellIndex internalCellIndex
        /* CA Changes end */
        )
{
    UInt16 tag             = 0;
    UInt8 *msg_p           = PNULL;
    UInt16 csisubframepatternTagLen   = 0;
    UInt16 csimeasurementsubsetTagLen   = 0;
    UInt8 subsetIndexNum = 0;
    msg_p = *msgBuf;
    UInt8 ConfigRequestType = 0;
    CsiChoice choice = 0;
    UInt8 subsetPatternLength = 0;
     UInt8 csiMeasurementSubset1[15] = { 0 };
    UInt8 csiMeasurementSubset2[15] = { 0 } ;

    msg_p += 2;

    /* cqi report v920 tag length*/
    csisubframepatternTagLen = LTE_GET_U16BIT(msg_p);
    msg_p += 2;

    if (CSI_SUBFRAME_PATTERN_CONFIG_VR10_TAG_LENGTH  > csisubframepatternTagLen)
    {      
        return MAC_FAILURE;
    }

     ConfigRequestType = *(msg_p);
      ulCreateReq_p->cqiSubsetConfigRequestType = ConfigRequestType;
      dlCreateReq_p->cqiSubsetConfigRequestType = ConfigRequestType;
      
        msg_p++;
      
       
        if ( ConfigRequestType > SETUP)
        {

           return MAC_FAILURE;
        }
        if ( ConfigRequestType )
        {
                 tag = LTE_GET_U16BIT(msg_p);
                 if(tag == CSI_MEASUREMENT_SUBSET_VR10)
                 {
                 msg_p += 2;
		   csimeasurementsubsetTagLen = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (CSI_MEASUREMENT_SUBSET_VR10_TAG_LENGTH  > csimeasurementsubsetTagLen)
                 {      
				 
                     return MAC_FAILURE;
                 }
			  
                  choice = (UInt8)*msg_p;
		    msg_p += 1;
#ifdef FDD_CONFIG

		    if(FDD_CONFIG_TYPE== choice)
		    {
		        subsetPatternLength = 5;
		    }
		    else
		    {

			 
            /* Review comment fix end RJ13 */
                     return MAC_FAILURE;
		    }
#else
		    if(TDD_CONFIG1_5 == choice)
		    {
		        subsetPatternLength = 3;
		    }	
                  else if(TDD_CONFIG0 == choice)
		    {
		        subsetPatternLength = 9;
		    }	
		    else if(TDD_CONFIG6 == choice)
		    {
		        subsetPatternLength = 8;
		    }	
		    else
		    {

			 
            /* Review comment fix end RJ13 */
                     return MAC_FAILURE;
		    }
				
#endif
	           for(subsetIndexNum = 0;subsetIndexNum< subsetPatternLength; subsetIndexNum++)
	           {
		      csiMeasurementSubset1[subsetIndexNum] = *(msg_p);
		      msg_p++;
	      	   }

	      updateCsiMeasurementSubset(ulCreateReq_p->csiMeasurementSubset1,csiMeasurementSubset2,internalCellIndex);

		 }
	      tag = LTE_GET_U16BIT(msg_p);
               

                if(tag == CSI_MEASUREMENT_SUBSET2_VR10)
                {
                 msg_p += 2;
		   csimeasurementsubsetTagLen = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (CSI_MEASUREMENT_SUBSET_VR10_TAG_LENGTH  > csimeasurementsubsetTagLen)
                 {      
				 
                     return MAC_FAILURE;
                 }
			  
                  choice = (UInt8)*msg_p;
		    msg_p += 1;
#ifdef FDD_CONFIG

		    if(FDD_CONFIG_TYPE== choice)
		    {
		        subsetPatternLength = 5;
		    }
		    else
		    {
		 
            /* Review comment fix end RJ13 */
                     return MAC_FAILURE;
		    }
#else
		    if(TDD_CONFIG1_5 == choice)
		    {
		        subsetPatternLength = 3;
		    }	
                  else if(TDD_CONFIG0 == choice)
		    {
		        subsetPatternLength = 9;
		    }	
		    else if(TDD_CONFIG6 == choice)
		    {
		        subsetPatternLength = 8;
		    }	
		    else
		    {
		    	 
            /* Review comment fix end RJ13 */
                     return MAC_FAILURE;
		    }
				
#endif
	           for(subsetIndexNum = 0;subsetIndexNum< subsetPatternLength; subsetIndexNum++)
	           {
		      csiMeasurementSubset2[subsetIndexNum] = *(msg_p);
		      msg_p++;
	      	   }

	         updateCsiMeasurementSubset(ulCreateReq_p->csiMeasurementSubset2,csiMeasurementSubset2,internalCellIndex);

	      tag = LTE_GET_U16BIT(msg_p);
               }
		}		 
    *msgBuf = msg_p;
    return MAC_SUCCESS;
}

/******************************************************************************
 * Function Name  : parseReconfigUECqiConfigIndexV10Info 
 * Inputs         : msgBuf - pointer to the input buffer
 *                  ulCreateReq_p - pointer to Uplink create UE structure
 *                  dlCreateReq_p - pointer to Downlink create UE structure 
 *                  internalCellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function parse the csiconfigindexv10 information in 
 *                  Create UE Entity Request and fills in the structure needed  
 *                  by MAC to create a UE context.
 ******************************************************************************/
STATIC inline MacRetType parseReconfigUECqiConfigIndexV10Info( UInt8 **msgBuf,
        ueContextForUplink *ulCreateReq_p, ueContextForDownlink *dlCreateReq_p 
        /* CA changes Start */
        ,UInt8 internalCellIndex
        /* CA Changes end */
        )
{
    UInt16 tag             = 0;
    UInt8 *msg_p           = PNULL;
    UInt16 cqiconfigindexTagLenV10   = 0;
    UInt16 cqiconfigindex2TagLenV10   = 0;
    UInt16 riconfigindexTagLenV10   = 0;
    msg_p = *msgBuf;

    msg_p += 2;

    /* cqi report v920 tag length*/
    cqiconfigindexTagLenV10 = LTE_GET_U16BIT(msg_p);
    msg_p += 2;

    if (CSI_CONFIG_INDEX_VR10_TAG_LENGTH > cqiconfigindexTagLenV10)
    {      
         lteWarning("Invalid cqiconfigindexTagLenV10 ");
        MAC_EMULATOR_LOG(LOG_INFO,"cqiconfigindexTagLenV10 -> %d\n",cqiconfigindexTagLenV10 );               

        return MAC_FAILURE;
    }

     ulCreateReq_p->cqiConfigRequestType = *(msg_p);
        msg_p++;
        /* + SPR 5953 Fix */
     dlCreateReq_p->cqiConfigRequestType = ulCreateReq_p->cqiRequestType;
        /* - SPR 5953 Fix */
        /*L2_FIX_8 */
        if ( ulCreateReq_p->cqiConfigRequestType > SETUP)
        {
         lteWarning("Invalid cqiconfigindexTagLenV10 ");
        MAC_EMULATOR_LOG(LOG_INFO,"cqiconfigindexTagLenV10 -> %d\n",cqiconfigindexTagLenV10 );    
            /* Review comment fix end RJ13 */
            return MAC_FAILURE;
        }
        if ( ulCreateReq_p->cqiConfigRequestType )
        {
                 tag = LTE_GET_U16BIT(msg_p);
                 if (tag == CQI_PMI_CONFIGINDEX2_VR10 )
                 {
                 msg_p += 2;
		   cqiconfigindex2TagLenV10 = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (CSI_CONFIG_INDEX_VR10_TAG_LENGTH > cqiconfigindex2TagLenV10)
                 {      
		         lteWarning("Invalid cqiconfigindex2TagLenV10 ");
	       	 MAC_EMULATOR_LOG(LOG_INFO,"cqiconfigindex2TagLenV10 -> %d\n",cqiconfigindex2TagLenV10 );  
        		return MAC_FAILURE;
                 }
		  ulCreateReq_p->cqiPMIConfigIndex_2 = LTE_GET_U16BIT(msg_p);
		  msg_p += 2;

		   tag = LTE_GET_U16BIT(msg_p);
                 }
                  if (tag == RI_CONFIG_INDEX_INFO)
                 {
                 msg_p += 2;
		   riconfigindexTagLenV10 = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (RI_CONFIGINDEX2_VR10_TAG_LENGTH > riconfigindexTagLenV10)
                 {      
		    lteWarning("Invalid riconfigindexTagLenV10 ");
		        MAC_EMULATOR_LOG(LOG_INFO,"riconfigindexTagLenV10 -> %d\n",riconfigindexTagLenV10 );  
	        	return MAC_FAILURE;
                 }
		  ulCreateReq_p->riConfigIndex_2 = LTE_GET_U16BIT(msg_p);
		  msg_p += 2;
                 }
				 
        }
    *msgBuf = msg_p;
    return MAC_SUCCESS;
}

/* EICIC -*/

/****************************************************************************
 * Function Name  : parseReconfigUEInfo
 * Inputs         : tagLen - length of the API message received.
 *                  dlUEInfo_p - Ptr to a structure to be filled for DL
 *                  ulUEInfo_p - Ptr to a structure to be filled for UL
 *                  current_p_p - Ptr to current_p idx
 * Outputs        : None
 * Returns        : None
 * Description    : This API takes pointer to the msg received as input, parse
 *                  the message and prepare DL & UL structures for UE context
 *                  reconfiguration at MAC Layer (inc UE Info & LCs).
 *****************************************************************************/

static inline UInt16 parseReconfigUEInfo( ueContextForDownlink *dlUEInfo_p,
                                          UInt8 *msg_p,
                                          UInt16 tagLen,
                                          ueContextForUplink *ulUEInfo_p,
                                          UInt8 cellIndex
                                        )
{
    UInt16 tag = 0;
    UInt16 len = 0;
    UInt8 i;
    UInt16 resp = MAC_SUCCESS;
    
    /* max harq tx changes */
    //ulUEInfo_p->maxRB            = 0;
    /* max harq tx changes */
    
    dlUEInfo_p->maxRB            = 0;
    dlUEInfo_p->transmissionMode = 0; 
    dlUEInfo_p->numOfLayer       = 0;
    dlUEInfo_p->precodingIndex   = 0;
    /* + MEAS_GAP_CHG */
    dlUEInfo_p->measurementGapRequestType = FALSE ;
    /* - MEAS_GAP_CHG */
    /*CLPC_CHG*/
    TpcPdcchConfig* tpcPdcchConf_p = PNULL;
    /*CLPC_CHG*/
    /* + CQI_4.1 */
    /*Setting Invalid value */
    ulUEInfo_p->cqiformatIndicatorPeriodic = INVALID_CQI_REPORT_MODE;
    dlUEInfo_p->cqiInfoFlag = FALSE;
    /* + SPR_8414 */    
    ulUEInfo_p->cqiRequestType = INVALID_REQUEST_TYPE;
    /* - SPR_8414 */    
    //ulUEInfo_p->ttiBundling = TTIB_DISABLED;

    
    /* - CQI_4.1 */
    /* + TM7_8 Changes Start */
    dlUEInfo_p->pmiRiReportV920 = INVALID_PMI_RI_CONF_V920;
    ulUEInfo_p->pmiRiReportV920 = INVALID_PMI_RI_CONF_V920;
    /* - TM7_8 Changes End */

	/* EICIC +*/
     ulUEInfo_p->cqiPMIConfigIndex_2 = MAC_MAX_CQI_PMI_CONFIG_INDEX + 1;
     ulUEInfo_p->riConfigIndex_2 = MAC_RI_CONFIG_INDEX_INVALID;
     ulUEInfo_p->csiMeasurementSubset1[0] = 0;
     ulUEInfo_p->csiMeasurementSubset1[1] = 0;
     ulUEInfo_p->csiMeasurementSubset1[2] = 0;
     ulUEInfo_p->csiMeasurementSubset1[3] = 0;
     ulUEInfo_p->csiMeasurementSubset1[4] = 0;

     ulUEInfo_p->csiMeasurementSubset2[0] = 0;
     ulUEInfo_p->csiMeasurementSubset2[1] = 0;
     ulUEInfo_p->csiMeasurementSubset2[2] = 0;
     ulUEInfo_p->csiMeasurementSubset2[3] = 0;
     ulUEInfo_p->csiMeasurementSubset2[4] = 0;
     ulUEInfo_p->cqiConfigRequestType = INVALID_PERIODIC_CQI_REQ_TYPE;
     ulUEInfo_p->cqiSubsetConfigRequestType = INVALID_PERIODIC_CQI_REQ_TYPE;
     dlUEInfo_p->cqiConfigRequestType = ulUEInfo_p->cqiRequestType;
     dlUEInfo_p->cqiSubsetConfigRequestType = ulUEInfo_p->cqiSubsetConfigRequestType;
    /* EICIC -*/
	
    if (tagLen < RECONFIG_UE_INFO_MIN_LEN)
    {
        return MAC_SYNTAX_ERROR;
    }    
    tagLen -= RECONFIG_UE_INFO_MIN_LEN;

    while ( tagLen > 0 )
    {
        if (tagLen < TAG_LEN)
        {
            lteWarning("No optional tag present in ReconfigUEInfo.\n");
            return MAC_FAILURE;
        }
        /* Parse Tag and validate it */
        //tag = (*msg_p | (*(msg_p + 1) << SHIFT_EIGHT_BITS));
        tag = LTE_GET_U16BIT(msg_p);
        msg_p += 2;
        
        //len = (*msg_p | (*(msg_p + 1) << SHIFT_EIGHT_BITS));
        len = LTE_GET_U16BIT(msg_p);
        msg_p += 2;
        
        /*Rel 5.3: ueSim Fix */
        if (tagLen >= len)
        {
            tagLen -= len; 
        }
        else
        {
            fprintf(stderr, "Invalid Length received len[%d] tag[%d] tagLen[%d]\n", len, tag, tagLen);
            return MAC_FAILURE;
        }

        switch ( tag )
        {
            case CQI_INFO:

                /* + CQI_4.1 */
                /* Tweak added as length is coming as 4 
                   which is invalid and seg fault occurs, fix to be provided by
                   L3 team */
                if ( 4 == len)
                {
                    break;
                }
                /* + SPR 7615 Fix */
                if ( len > 93 || len < 9 ) /* EICIC +-*/
                {
                /* - SPR 7615 Fix */
                    lteWarning("CQI INFO TAG Len Invalid [%d]", len);
                    return MAC_FAILURE;
                }
                tag = LTE_GET_U16BIT(msg_p);
                msg_p += 2;
                /* + CQI_4.1 */
                dlUEInfo_p->cqiInfoFlag = TRUE;
                /* - CQI_4.1 */

                if ( CQI_APERIODIC_INFO == tag )
                {
                    if (8 > cellConfigUeSim_g[cellIndex].dlAvailableRBs)
                    {
                        lteWarning("Aperiodic mode configured with DL "
                                "RBs are less than 8\n");
                        return MAC_FAILURE;
                    }
                    /*skip the length of the CQI Aperiodic Info tag*/
                    msg_p += 2;
                    ulUEInfo_p->cqiMode = *msg_p++;
                    resp = validateCqiMode ( ulUEInfo_p->cqiMode );
                    if ( resp != MAC_SUCCESS )
                    {
                       lteWarning(" Invalid Aperiodic CQI Mode");
                       return resp;
                    }
                    
                    dlUEInfo_p->cqiMode = ulUEInfo_p->cqiMode;

                    /* Periodic CQI Info is present */
                    if ( len >= 14 && len <= 35 )         
                    {
                        tag = LTE_GET_U16BIT(msg_p);
                        msg_p += 2;
                    }
                }
                if ( CQI_PERIODIC_INFO == tag )
                {
                    UInt32 cqiPeriodicTagLen = 0;
                    UInt32 cqiRequestType = 0;

                    /* cqi periodic tag length*/
                    cqiPeriodicTagLen = LTE_GET_U16BIT(msg_p);
                    msg_p += 2;

                    cqiRequestType = *(msg_p); 
                    msg_p++;

                    if ( cqiRequestType > 1)
                    {
                        lteWarning("Invalid CQI Request Type");
                        return MAC_FAILURE;
                    }
                    /* + SPR_8414 */    
                    ulUEInfo_p->cqiRequestType = cqiRequestType;
                    /* - SPR_8414 */    
                    if ( cqiRequestType )
                    {
                        if ( cqiPeriodicTagLen < PERIODIC_CQI_SETUP_LEN )
                        {
                            return MAC_FAILURE;
                        }
                        tag = LTE_GET_U16BIT(msg_p);
                        msg_p += 2;

                        if ( CQI_PMI_CONFIG_INDEX_INFO == tag)
                        {
                            cqiPeriodicTagLen = LTE_GET_U16BIT(msg_p);
                            msg_p += 2;

                            if ( cqiPeriodicTagLen <  PERIODIC_CQI_LEN )
                            {
                                return MAC_FAILURE;
                            }

                            ulUEInfo_p->cqiPMIConfigIndex = LTE_GET_U16BIT(msg_p);
                            msg_p += 2;
                            /* + FDD Specific validation */
                            if ( ( ulUEInfo_p->cqiPMIConfigIndex > 
                                    MAC_MAX_CQI_PMI_CONFIG_INDEX ) ||
                               ( ulUEInfo_p->cqiPMIConfigIndex == 
                                    MAC_MAX_CQI_PMI_CONFIG_INDEX_3_1_7 ) )  
                            {
                                lteWarning("Invalid range for cqiPMIConfigIndex \n");
                                return MAC_FAILURE;
                            }        
                            /* - FDD Specific validation */

                            ulUEInfo_p->cqiPucchResourceIndex = LTE_GET_U16BIT(msg_p);
                            msg_p += 2;
                            if (ulUEInfo_p->cqiPucchResourceIndex > 
                                    MAC_MAX_CQI_PUCCH_RESOURCE_INDEX)
                            {
                                lteWarning("Invalid range for cqiPucchResourceIndex \n");
                                return MAC_FAILURE;
                            }     

                            ulUEInfo_p->simultaneousACKNACKAndCQI = *msg_p;
                            msg_p++;
                            if ( ( ulUEInfo_p->simultaneousACKNACKAndCQI != 
                                        SIMULTANEOUS_ACK_NACK_CQI_0) &&
                                 ( ulUEInfo_p->simultaneousACKNACKAndCQI != 
                                   SIMULTANEOUS_ACK_NACK_CQI_1) )
                            {
                                lteWarning("Invalid range for simultaneousACKNACKAndCQI \n");
                                return MAC_FAILURE;
                            }  

                            ulUEInfo_p->cqiformatIndicatorPeriodic = *msg_p;
                            msg_p += 1;
                            tag = LTE_GET_U16BIT(msg_p);
                            /* + Bandwidth Fix */
                            if(ulUEInfo_p->cqiformatIndicatorPeriodic)
                            {
                                if (8 > cellConfigUeSim_g[cellIndex].dlAvailableRBs)
                                {
                                    lteWarning("Periodic Subband mode configured with DL "
                                            "RBs are less than 8\n");
                                    return MAC_FAILURE;
                                }
                            }
                            /* - Bandwidth Fix */
                            /*Check if Report FORMAT is SUBBAND*/
                            if(CQI_FORMAT_PERIODIC_SUBBAND_K == tag)     
                            {    
                                msg_p += 2;
                                len = LTE_GET_U16BIT(msg_p);
                                msg_p += 2;
                                ulUEInfo_p->cqiformatIndicatorPeriodic_subband_k = *msg_p;
                                msg_p += 1;
                            }
                            else
                            {
                                if(1 == ulUEInfo_p->cqiformatIndicatorPeriodic)
                                {
                                    return MAC_FAILURE;
                                }
                            }

                            tag = LTE_GET_U16BIT(msg_p);
                            if (RI_CONFIG_INDEX_INFO == tag) 
                            { 
                                msg_p += 2;
                                len = LTE_GET_U16BIT(msg_p);
                                msg_p += 2;
                                ulUEInfo_p->riConfigIndex = LTE_GET_U16BIT(msg_p);
                                msg_p += 2;
                                if (ulUEInfo_p->riConfigIndex > MAC_MAX_RI_CONFIG_INDEX)
                                {
                                    lteWarning("Invalid range for riConfigIndex \n");
                                    return MAC_FAILURE;
                                }
                            }    

                        }
                        else if (CQI_PMI_CONFIG_INDEX_INFO_v1020 == tag) /*EICIC +*/
	                 {
	                 
	                           cqiPeriodicTagLen = LTE_GET_U16BIT(msg_p);
	                            msg_p += 2;

	                            if ( cqiPeriodicTagLen <  PERIODIC_CQI_LEN )
	                            {
	                                return MAC_FAILURE;
	                            }

	                            ulUEInfo_p->cqiPMIConfigIndex = LTE_GET_U16BIT(msg_p);
	                            msg_p += 2;
	                            /* + FDD Specific validation */
	                            if ( ( ulUEInfo_p->cqiPMIConfigIndex > 
	                                    MAC_MAX_CQI_PMI_CONFIG_INDEX ) ||
	                               ( ulUEInfo_p->cqiPMIConfigIndex == 
	                                    MAC_MAX_CQI_PMI_CONFIG_INDEX_3_1_7 ) )  
	                            {
	                                lteWarning("Invalid range for cqiPMIConfigIndex \n");
	                                return MAC_FAILURE;
	                            }        
	                            /* - FDD Specific validation */

	                            ulUEInfo_p->cqiPucchResourceIndex = LTE_GET_U16BIT(msg_p);
	                            msg_p += 2;
	                            if (ulUEInfo_p->cqiPucchResourceIndex > 
	                                    MAC_MAX_CQI_PUCCH_RESOURCE_INDEX)
	                            {
	                                lteWarning("Invalid range for cqiPucchResourceIndex \n");
	                                return MAC_FAILURE;
	                            }     

	                            ulUEInfo_p->simultaneousACKNACKAndCQI = *msg_p;
	                            msg_p++;
	                            if ( ( ulUEInfo_p->simultaneousACKNACKAndCQI != 
	                                        SIMULTANEOUS_ACK_NACK_CQI_0) &&
	                                 ( ulUEInfo_p->simultaneousACKNACKAndCQI != 
	                                   SIMULTANEOUS_ACK_NACK_CQI_1) )
	                            {
	                                lteWarning("Invalid range for simultaneousACKNACKAndCQI \n");
	                                return MAC_FAILURE;
	                            }  

	                            ulUEInfo_p->cqiformatIndicatorPeriodic = *msg_p;
	                            msg_p += 1;
	                            tag = LTE_GET_U16BIT(msg_p);
	                            /* + Bandwidth Fix */
	                            if(ulUEInfo_p->cqiformatIndicatorPeriodic)
	                            {
	                                if (8 > cellConfigUeSim_g[cellIndex].dlAvailableRBs)
	                                {
	                                    lteWarning("Periodic Subband mode configured with DL "
	                                            "RBs are less than 8\n");
	                                    return MAC_FAILURE;
	                                }
	                            }
	                            /* - Bandwidth Fix */
	                            /*Check if Report FORMAT is SUBBAND*/
	                            if(CQI_FORMAT_PERIODIC_SUBBAND_K == tag)     
	                            {    
	                                msg_p += 2;
	                                len = LTE_GET_U16BIT(msg_p);
	                                msg_p += 2;
	                                ulUEInfo_p->cqiformatIndicatorPeriodic_subband_k = *msg_p;
	                                msg_p += 1;
	                            }
	                            else
	                            {
	                                if(1 == ulUEInfo_p->cqiformatIndicatorPeriodic)
	                                {
	                                    return MAC_FAILURE;
	                                }
	                            }

	                            tag = LTE_GET_U16BIT(msg_p);
	                            if (RI_CONFIG_INDEX_INFO == tag) 
	                            { 
	                                msg_p += 2;
	                                len = LTE_GET_U16BIT(msg_p);
	                                msg_p += 2;
	                                ulUEInfo_p->riConfigIndex = LTE_GET_U16BIT(msg_p);
	                                msg_p += 2;
	                                if (ulUEInfo_p->riConfigIndex > MAC_MAX_RI_CONFIG_INDEX)
	                                {
	                                    lteWarning("Invalid range for riConfigIndex \n");
	                                    return MAC_FAILURE;
	                                }
	                                tag = LTE_GET_U16BIT(msg_p);
	                            }    
	                            
					if (CSI_CONFIG_INDEX_VR10 == tag) 
					{

					      
						if (MAC_SUCCESS != parseReconfigUECqiConfigIndexV10Info (&msg_p, 
									ulUEInfo_p, dlUEInfo_p 
									/* CA changes Start */
									, cellIndex
									/* CA Changes end */
									))
						{
							return MAC_FAILURE;
						}
						
						tag = LTE_GET_U16BIT(msg_p);
					}

	                        } /* EICIC -*/
                        }
                    }
                
                /* + SPR 7615 Fix */
                tag = LTE_GET_U16BIT(msg_p);
                /* - SPR 7615 Fix */
                /* - CQI_4.1 */
                /* + TM7_8 Changes Start */
                if (CQI_REPORT_CONFIG_V920 == tag) 
                {
                    UInt16 cqiTagLenV920 = 0;
                    /* skip CQI_REPORT_CONFIG_V920 tag */
                    msg_p += 2; 

                    /* cqi report v920 tag length*/
                    cqiTagLenV920 = LTE_GET_U16BIT(msg_p);
                    msg_p += 2;

                    if ( CQI_REPORT_V920_LEN > cqiTagLenV920 )
                    {
                        return MAC_FAILURE;
                    }
                    tag = LTE_GET_U16BIT(msg_p);

                    if (CQI_MASK_V920 == tag)
                    {
                        /* SPR 3203 3226 Start */
                        /* skip CQI_MASK_V920 tag */
                        msg_p += 2;

                        //tagLen = LTE_GET_U16BIT(msg_p);
                        //Skip the tag length
                        /* SPR 3203 3226 End */
                        msg_p += 2;
                        ulUEInfo_p->cqiMaskV920 = *msg_p++;
                        if (ulUEInfo_p->cqiMaskV920 != TRUE && 
                                ulUEInfo_p->cqiMaskV920 != FALSE )
                        {
                            return MAC_FAILURE;
                        }
                        tag = LTE_GET_U16BIT(msg_p);
                    }

                    if (PMI_RI_REPORT_V920 == tag)
                    {
                        /* SPR 3203 3226 Start */
                        /* skip PMI_RI_REPORT_V920 tag */
                        msg_p += 2;
                        //tagLen = LTE_GET_U16BIT(msg_p);
                        //Skip the tag length
                        /* SPR 3203 3226 End */
                        msg_p += 2;
                        ulUEInfo_p->pmiRiReportV920 = *msg_p++;
                        if (ulUEInfo_p->pmiRiReportV920 != TRUE && 
                                ulUEInfo_p->pmiRiReportV920 != FALSE )
                        {
                            return MAC_FAILURE;
                        }
                        dlUEInfo_p->pmiRiReportV920 = ulUEInfo_p->pmiRiReportV920;
                    }
                  tag = LTE_GET_U16BIT(msg_p);
                }
                /* Rel9_upgrade_CR396 */
                /* - TM7_8 Changes End */
 
                /* EICIC +*/
                if (CSI_SUBFRAME_PATTERN_CONFIG_VR10 == tag) 
		 {
				if (MAC_SUCCESS != parseReconfigUECsiSubframePatternV10Info (&msg_p, 
									ulUEInfo_p, dlUEInfo_p 
									, cellIndex))
				{
							return MAC_FAILURE;
				}
		}
                /* EICIC -*/
                
                break;

            case DL_MAX_RB_INFO:
                if (len != DL_MAX_RB_INFO_LEN)
                {
                    lteWarning(
                    "Minimum length require not present for DL_MAX_RB_INFO_LEN\n");
                    return MAC_SYNTAX_ERROR;
                }    
                dlUEInfo_p->maxRB = *msg_p++;
                if (MAC_SUCCESS != 
                        validateDlMaxRB(dlUEInfo_p->maxRB))
                {
                    return MAC_SYNTAX_ERROR;
                }    
                break; 

            case UL_MAX_RB_INFO:
    /* max harq tx changes */
#if 0
                if (len != UL_MAX_RB_INFO_LEN)
                {
                    lteWarning(
                    "Minimum length require not present for UL_MAX_RB_INFO_LEN\n");
                    return MAC_SYNTAX_ERROR;
                }    
                ulUEInfo_p->maxRB = *msg_p++;
                if (MAC_SUCCESS != 
                        validateUlMaxRB(ulUEInfo_p->maxRB))
                {
                    return MAC_SYNTAX_ERROR;
                }
#endif
    /* max harq tx changes */
                break;

            case TX_MODE_INFO:
                if (len != TX_MODE_INFO_LEN)
                {
                    lteWarning(
                    "Minimum length require not present for TX_MODE_INFO_LEN\n");
                    return MAC_SYNTAX_ERROR;
                }    
                dlUEInfo_p->transmissionMode = *msg_p++;
                if (MAC_SUCCESS != 
                        validateTransmissionMode(dlUEInfo_p->transmissionMode))
                {
                    return MAC_SYNTAX_ERROR;
                }
                break;

            case NUM_OF_LAYER_INFO:
                if (len != NUM_OF_LAYER_INFO_LEN)
                {
                    lteWarning(
                    "Minimum length require not present for NUM_OF_LAYER_INFO\n");
                    return MAC_SYNTAX_ERROR;
                }    
                
                dlUEInfo_p->numOfLayer = *msg_p++;
                
                if (MAC_SUCCESS != 
                        validateNumOfLayer(dlUEInfo_p->numOfLayer))
                {
                    return MAC_SYNTAX_ERROR;
                }
                break;

            case CODEBOOK_INDEX_INFO:
                if (len != CODEBOOK_INDEX_INFO_LEN)
                {
                    lteWarning(
                    "Minimum length require not present for CODEBOOK_INDEX_INFO\n");
                    return MAC_SYNTAX_ERROR;
                }    
                dlUEInfo_p->precodingIndex = *msg_p++;
                if (MAC_SUCCESS != 
                        validatePrecodingIndex(dlUEInfo_p->precodingIndex))
                {
                    return MAC_SYNTAX_ERROR;
                }
                break;


            case SR_CONFIG_INFO:
                LTE_LOG (LOG_DETAIL, macModuleLogDetail_p,
                        "SR_CONFIG_INFO ");

                if ( len != 5 && len != 12 )
                {
                    lteWarning("Invalid Tag Length for SR Info");
                    return MAC_FAILURE;
                }

                ulUEInfo_p->requestType = *msg_p++;

                resp = validateSRRequestType( ulUEInfo_p->requestType);
                if ( resp != MAC_SUCCESS )
                {
                    return resp;
                }
                len -= 5;
                if ( ulUEInfo_p->requestType == 0 )
                {
                    continue;
                }
                if ( len != 7)
                {
                    lteWarning("Mac syntax error.\n");
                    return MAC_FAILURE;
                }
                //tag = (*msg_p | (*(msg_p + 1) << SHIFT_EIGHT_BITS));
                tag = LTE_GET_U16BIT(msg_p);
                msg_p += 2;
                if ( tag != SR_SETUP_INFO)
                {
                    lteWarning("Invalid SR_SETUP_INFO Tag.\n");
                    return MAC_TAG_NOT_EXISTS;
                }
                //len = (*msg_p | (*(msg_p + 1) << SHIFT_EIGHT_BITS));
                len = LTE_GET_U16BIT(msg_p);
                msg_p += 2;
                if ( len != SR_SETUP_INFO_LEN)
                {
                    lteWarning("Mac syntax length error .\n");
                    return MAC_FAILURE;
                }
                //ulUEInfo_p->srPucchResourceIndex =
                  //  (*msg_p | (*(msg_p + 1) << SHIFT_EIGHT_BITS));
                ulUEInfo_p->srPucchResourceIndex = LTE_GET_U16BIT(msg_p);
                msg_p += 2;
                resp = validateSrPucchResourceIndex(
                        ulUEInfo_p->srPucchResourceIndex);
                if (resp != MAC_SUCCESS)
                {
                    return resp;
                }

                ulUEInfo_p->srConfigurationIndex =  *msg_p++;

                resp = validateSrConfigurationIndex(
                        ulUEInfo_p->srConfigurationIndex );
                if (resp != MAC_SUCCESS)
                {
                    return resp;
                }
                break;
            /* + CL-MIMO RSM*/
            case CODEBOOK_SUBSET_RESTRICTION:
                {
                    fprintf(stderr,"codebookSubsetRestriction in MAC_RECONFIG \n");
                    if ( len != 13 )
                    {
                        lteWarning("Invalid Tag Length for codebookSubsetRestriction in MAC_CREATE_UE_ENTITY_REQ\n");
                        return MAC_FAILURE;
                    }
                    /* Codebook subset restriction is supported for
                     * transmission modes 3, 4, 5 and 6
                     */
                    if((3 == dlUEInfo_p->transmissionMode)||
                            (4 == dlUEInfo_p->transmissionMode)||
                            (5 == dlUEInfo_p->transmissionMode)||
                            (6 == dlUEInfo_p->transmissionMode)
                      )
                    {
                        ulUEInfo_p->codeBookSubsetfromRRC.cbsrType = *msg_p++;
                        for (i = 0; i < MAX_CBSR_SIZE; i++)
                        {
                            ulUEInfo_p->codeBookSubsetfromRRC.cbsrValue[i] = *msg_p++;
                        }
                    }
                    else
                    {
                        lteWarning("codebookSubsetRestriction received for \
                                transmission mode other than 3,4,5,6 in \
                                MAC_CREATE_UE_ENTITY_REQ\n");
                    }
                    /* SPR 8422 Fix Start */
                    if ((resp = validateCBSR( &ulUEInfo_p->codeBookSubsetfromRRC,
                                              dlUEInfo_p->transmissionMode ,
                                              /*4x4 DL MIMO CHG START*/
                                              dlUEInfo_p->rankIndicator,
                                              /*4x4 DL MIMO CHG END*/ 
                                              cellIndex) != MAC_SUCCESS))
                                                                                                                                                                {     
                        lteWarning("Atleast one bit should be 1 in cbsrValue in MAC_CREATE_UE_ENTITY_REQ\n");
                        return resp; 
                    }
                    /* SPR 8422 Fix End */
                }
                break;
                /* - CL-MIMO RSM*/
                /* SPR 7963 Fix Start */
                case UE_CATEGORY:
                {
                    if ( len != 5 )
                    {
                        lteWarning("Invalid Tag Length %d for UE_CATEGORY  Info\n"
                                ,len);
                        fprintf(stderr,"Invalid Tag Length %d for UE_CATEGORY  Info\n"
                                ,len);
                        return MAC_FAILURE;
                    }
                    if (*msg_p > 5 || *msg_p < 1)
                    {
                        fprintf(stderr,"**** UE_CATEGORY[%d] Returning Failure*****\n",*msg_p);
                        lteWarning("Invalid range for UeCategory .\n");
                        return MAC_FAILURE;
                    }
                    fprintf(stderr,"**** UE_CATEGORY[%d] IN MAC RECONFIG *****\n",*msg_p);
                    dlUEInfo_p->ueCategory = *msg_p++;
                    ulUEInfo_p->ueCategory = dlUEInfo_p->ueCategory;
                    break;
                }
                /* SPR 7963 Fix End */ 
                /* +  DRX_CHG */
                case  DRX_CONFIG_INFO:
                {
                     resp =  parseDRXConfigInfo(&msg_p,
                                                 len,
                                                 dlUEInfo_p);
                     if( MAC_FAILURE == resp )
                     {
                          return  MAC_FAILURE;
                     }
                }
                break;
                /* -  DRX_CHG */
 
               /* + MEAS_GAP_CHG */
               case  MEAS_GAP_CONFIG_INFO:
               {
                    resp =  parseMeasGapConfigInfo(&msg_p,
                                                   len,
                                                   dlUEInfo_p);
                    if( MAC_FAILURE == resp )
                    {
                        return  MAC_FAILURE;
                    }
                    dlUEInfo_p->measurementGapRequestType = TRUE ;
               }
               break;

               case CQI_MASK_INFO:
               {
               }
              /* -  MEAS_GAP_CHG */
               case TIME_ALIGNMENT_TIMER_INFO:
               {
                   msg_p+=2;
               }
            /*CLPC_CHG_BEGIN*/
            case PDSCH_CONFIG_INFO:
            {
                if(len != (PDSCH_CONFIG_INFO_PARAM_LENGTH + TAG_LEN))
                {
                    return MAC_FAILURE;
                }
                dlUEInfo_p->isPAPresent = TRUE;
                dlUEInfo_p->pA = *msg_p++;
                if((MIN_PA_VALUE > dlUEInfo_p->pA) ||
                        (MAX_PA_VALUE < dlUEInfo_p->pA))
                {
                    return MAC_FAILURE;
                }
                break;
            }

            case P0_UE_PUSCH:
            {
                if(len != P0_UE_PUSCH_LEN)
                {
                    return MAC_FAILURE;
                }
                ulUEInfo_p->isDedicatedULPowerControlInfoPresent = TRUE;
                ulUEInfo_p->dedicatedULPowerControlInfo.p0UePusch = *msg_p++;
                if((MIN_P0_UE_PUSCH_VALUE > ulUEInfo_p->dedicatedULPowerControlInfo.p0UePusch) ||
                        (MAX_P0_UE_PUSCH_VALUE < ulUEInfo_p->dedicatedULPowerControlInfo.p0UePusch))
                {
                    return MAC_FAILURE;
                }
                break;
            }

            case TPC_PDCCH_CONFIG_PUCCH:
            {
                /*Checking if tagLen is minimum len of TPC_PDCCH_CONFIG_PUCCH*/
                if(len < TPC_PDCCH_CONFIG_LENGTH)
                {
                    return MAC_FAILURE;
                }
                dlUEInfo_p->isTpcPdcchConfigPucchPresent = TRUE;
                tpcPdcchConf_p = &(dlUEInfo_p->tpcPdcchConfigPucch);

                /* CLPC_REWORK CHG */
                tpcPdcchConf_p->release = *msg_p++;
                if((FALSE != tpcPdcchConf_p->release) &&
                        (TRUE != tpcPdcchConf_p->release))
                {
                    lteWarning("Invalid release. Value = %d\n",
                            tpcPdcchConf_p->release);
                    return MAC_FAILURE;
                }
                if (!tpcPdcchConf_p->release)
                {
                    resp = parseTpcPdcchConfig(msg_p,
                            len,
                            0, /*Current Not Required*/
                            tpcPdcchConf_p,
                            FALSE);
                    if(resp == MAC_FAILURE)
                    {
                        return MAC_FAILURE;
                    }
                }
                msg_p += (len - TAG_LEN - 1); 
                /* CLPC_REWORK_CHG END */
                break;
            }

            case TPC_PDCCH_CONFIG_PUSCH:
            {
                /*Checking if tagLen is minimum len of TPC_PDCCH_CONFIG_PUCCH*/
                if(len < TPC_PDCCH_CONFIG_LENGTH)
                {
                    return MAC_FAILURE;
                }
                ulUEInfo_p->isTpcPdcchConfigPuschPresent = TRUE;
                tpcPdcchConf_p = &(ulUEInfo_p->tpcPdcchConfigPusch);

                /* CLPC_REWORK CHG */
                tpcPdcchConf_p->release = *msg_p++;
                if((FALSE != tpcPdcchConf_p->release) &&
                        (TRUE != tpcPdcchConf_p->release))
                {
                    lteWarning("Invalid release. Value = %d\n",
                            tpcPdcchConf_p->release);
                    return MAC_FAILURE;
                }

                if (!tpcPdcchConf_p->release)
                {
                    resp = parseTpcPdcchConfig(msg_p,
                            len,
                            0,/*Current Not Required*/
                            tpcPdcchConf_p,
                            TRUE);
                }
                /* CLPC_REWORK_CHG END */
                break;
            }
                /* SPS_CHG */
                case  SPS_CONFIG:
                {
                    resp =  parseSpsConfigInfo(msg_p,
                            /* SPR 8551 Fix Starts*/
                                           len,
                            /* SPR 8551 Fix Ends*/
                                           dlUEInfo_p,
                                           ulUEInfo_p);
                    if( MAC_FAILURE == resp )
                    {
                        return MAC_FAILURE;
                    }
                    /* SPR 8551 Fix Starts*/
                    msg_p += (len - TAG_LEN);
                    /* SPR 8551 Fix Ends*/
                    break;
                }  
                /* SPS_CHG */
                /*CLPC_CHG_END*/
            /* HD FDD Changes Start */
            case RF_PARAMS:
            {
              /* Checking if tagLen is minimum of RF_PARAMS_LENGTH */
              if(len < RF_PARAMS_LENGTH)
              {
                  fprintf(stderr,"Invalid length for RF_PARAMS");
                  return MAC_FAILURE;
              }
              //msg_p++;
              //KAR:UT
#ifdef HD_FDD_CONFIG
              if((FALSE != ulUEInfo_p->isHDFddFlag) &&
                 (TRUE != ulUEInfo_p->isHDFddFlag))
              {
                  fprintf(stderr,"Invalid Duplexing mode. Value = %d\n",
                          ulUEInfo_p->isHDFddFlag);
                  return MAC_FAILURE;
              }
              else if (TRUE == ulUEInfo_p->isHDFddFlag)
              {
                  fprintf(stderr,"ue index of HD-FDD UE is = %d",ulUEInfo_p->ueIdx );
              }
              ulUEInfo_p->isHDFddFlag = *msg_p;
              dlUEInfo_p->isHDFddFlag = *msg_p;
              fprintf(stderr,"[%s] RF Params [%d]\n",__func__,ulUEInfo_p->isHDFddFlag);
#endif
              msg_p++;
            }
            break;


                case TTI_BUNDLING_INFO :
                {
                    if (TTI_BUNDLING_INFO_TAG_LEN != len )
                    {
                        lteWarning("Invalid Tag Length %d for TTI_BUNDLING_Info\n"
                                    ,len);
                        return MAC_FAILURE;
                    }
    
#ifdef FDD_CONFIG
                   /** For TTIB_Code */

                    ulUEInfo_p->ttiBundling = *msg_p;

                    if ((ulUEInfo_p->ttiBundling != TTIB_DISABLED) &&
                            (ulUEInfo_p->ttiBundling != TTIB_ENABLED))
                    {
                        lteWarning("Invalid Value for ttiBundling Info :%d \n",
                        ulUEInfo_p->ttiBundling);
                        return MAC_FAILURE;
                    }
#endif
                    msg_p++;
                    break;
                }

            /* HD FDD Changes End*/
				/* + CA_PHASE2 */
            case UE_CATEGORY_V1020:
				{
                    /* + SPR 11213 Changes */
					if (len != UE_CATEGORY_TAG_LEN)
                    /* + SPR 11213 Changes */
					{
						lteWarning("Mandatory fields missing in UE_CATEGORY_V1020_LEN.\n");
						return MAC_SYNTAX_ERROR;
					}
					dlUEInfo_p->ueCategoryV1020 = *msg_p++;
					ulUEInfo_p->ueCategoryV1020 = dlUEInfo_p->ueCategoryV1020 ;
					dlUEInfo_p->ueCategoryReqType = TRUE ;
					ulUEInfo_p->ueCategoryReqType = TRUE ;
					resp = macValidateUeCategoryV1020(dlUEInfo_p->ueCategoryV1020);
					if (resp != MAC_SUCCESS)
					{
						return resp;
					}
					break;
				}
            case PUCCH_FORMAT_V1020:
				{
                    /* + SPR 11213 Changes */
					resp = parseReconfigPUCCHFormatV1020Info(ulUEInfo_p, dlUEInfo_p, 
							len, &msg_p, cellIndex);
                    /* - SPR 11213 Changes */
					if (resp != MAC_SUCCESS)
					{
						return resp;
					}
					break;
				}		
				
            case CA_CONFIG:
				{
                    /* + SPR 11213 Changes */
					resp = parseReconfigCAConfigInfo(ulUEInfo_p, dlUEInfo_p, len, &msg_p, cellIndex);
                    /* + SPR 11213 Changes */
					if (resp != MAC_SUCCESS)
					{
						return resp;
					}
					break;
				}

				/* - CA_PHASE2 */
            default:
                lteWarning("Incorrect Tag received in RECONFIGURE_UE_INFO\n");

                /*CLPC_CHG_BEGIN*/
                /*SPR 7770 Changes Start */
                /* Rel 5.3: ueSim Fix */
                msg_p += (len - 4);
                /*SPR 7770 Changes End */
                //return MAC_SYNTAX_ERROR;
                /*CLPC_CHG_END*/


            }/* end of switch */        
        }

      /* + CQI_4.1 */
      if ( (3 != dlUEInfo_p->transmissionMode &&
            4 != dlUEInfo_p->transmissionMode) &&
             /* + TM7_8 Changes Start */
           (!((8 == dlUEInfo_p->transmissionMode) &&
             (TRUE == dlUEInfo_p->pmiRiReportV920))))
             /* - TM7_8 Changes End */
      {
          ulUEInfo_p->riConfigIndex = MAC_RI_CONFIG_INDEX_INVALID;
          lteWarning("riConfigIndex is configured in TX mode "
                  "other than 3 and 4.\n");
      }
      /* - CQI_4.1 */

    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validatecqimode
 * Inputs         : cqiIndication - 0 - Aperiodic 
 *                                  1 - Periodic
 *
 *                  cqimode - In case cqiIndication is 0, 
 *                            any value among 12, 20, 22, 30, 31 
 *                            In case cqiIndication is 1, 
 *                            any value among 10, 11, 20, 21
 *
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes cqimode  to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

static inline MacRetType validateCqiMode(UInt8 cqimode)
{
    if ((cqimode != CQI_MODE_1_2)&&
            (cqimode != CQI_MODE_2_0)&& 
            (cqimode != CQI_MODE_2_2)&& 
            (cqimode != CQI_MODE_3_0)&& 
            (cqimode != CQI_MODE_3_1))
    {
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validateDlMaxRB
 * Inputs         : MaxRB - tells the maximum no of RBS a UE can have.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes MaxRB to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

static inline MacRetType validateDlMaxRB(UInt8 MaxRB)
{
    LTE_LOG (LOG_DETAIL, macModuleLogDetail_p,"[%s] Entry ", __func__ );
//Blr support 21-Dec
/*
    if ( MaxRB > cellConfigUeSim_g.dlAvailableRBs ) 
    {
        lteWarning("MAX RB [%d ] of UE is more than the DL AvailableRBs [%d ]"
                ",hence setting the max RB = %d [ DL AvailableRBs] \n",
                MaxRB,cellConfigUeSim_g.dlAvailableRBs,cellConfigUeSim_g.dlAvailableRBs );
    }
*/
    LTE_LOG (LOG_DETAIL, macModuleLogDetail_p,"[%s] Exit ", __func__ );
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateUlMaxRB
 * Inputs         : MaxRB - tells the maximum no of RBS a UE can have.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes MaxRB to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

static inline MacRetType validateUlMaxRB(UInt8 MaxRB)
{
    LTE_LOG (LOG_DETAIL, macModuleLogDetail_p,"[%s] Entry ", __func__ );
//Blr support 21-Dec
/*
    if (MaxRB > cellConfigUeSim_g.ulAvailableRBs) 
    {
        lteWarning(" MAX RB [%d ] of UE is more than the UL AvailableRBs [%d ]"
                ",hence setting the max RB = %d [ UL AvailableRBs] \n",
                MaxRB,cellConfigUeSim_g.ulAvailableRBs,cellConfigUeSim_g.ulAvailableRBs );
        return MAC_FAILURE;
    }
*/
    LTE_LOG (LOG_DETAIL, macModuleLogDetail_p,"[%s] Exit ", __func__ );
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validateTransmissionMode
 * Inputs         : transmissionMode - tells the mode of transmission .
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes transmissionMode to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

static inline MacRetType validateTransmissionMode(UInt8 transmissionMode)
{
    LTE_LOG (LOG_DETAIL, macModuleLogDetail_p,"[%s] Entry ", __func__ );
    if ((transmissionMode < MAC_MIN_TRANSMISSION_MODE) ||
            (transmissionMode > MAC_MAX_TRANSMISSION_MODE))
    {
        lteWarning("Invalid range for transmission mode.\n");
        return MAC_FAILURE;
    }
    LTE_LOG (LOG_DETAIL, macModuleLogDetail_p,"[%s] Exit ", __func__ );
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validatenumOfLayer
 * Inputs         : numOfLayer - Number of layer.Its value can be 1-4
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes numOfLayer  to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/
static inline MacRetType validateNumOfLayer(UInt8 numOfLayer)
{
    if (numOfLayer > MAX_NUM_OF_LAYER ||
            numOfLayer < MIN_NUM_OF_LAYER )
    {
        lteWarning("Invalid range for numOfLayer .\n");
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validatePrecodingIndex
 * Inputs         : precodingIndex - It specifies  precoding matrix index
                                     to be used
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes precodingIndex  to validate and
 *                  returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

static inline MacRetType validatePrecodingIndex(UInt8 precodingIndex)
{
    if (precodingIndex > MAX_PRE_CODING_INDEX )
    {
        lteWarning("Invalid range for precodingIndex .\n");
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validaterequestType
 * Inputs         : requestType -
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes requestType  to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

static inline MacRetType validateSRRequestType(UInt8 requestType)
{
    LTE_LOG (LOG_DETAIL, macModuleLogDetail_p,"[%s] Entry ", __func__ );
    if ((requestType != REQUEST_TYPE_0) &&
            (requestType != REQUEST_TYPE_1))
    {
        lteWarning("Invalid range for requestType .\n");
        return MAC_FAILURE;
    }
    LTE_LOG (LOG_DETAIL, macModuleLogDetail_p,"[%s] Exit ", __func__ );
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validatesrPucchResourceIndex
 * Inputs         : srPucchResourceIndex
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes srPucchResourceIndex  to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

static inline MacRetType validateSrPucchResourceIndex(UInt16 srPucchResourceIndex)
{
    if (srPucchResourceIndex > MAC_MAX_SRPUCCHRESOURCEINDEX)
    {
        lteWarning("Invalid range for srPucchResourceIndex .\n");
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 * Function Name  : validatesrConfigurationIndex
 * Inputs         : srConfigurationIndex -
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes srConfigurationIndex  to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

static inline MacRetType validateSrConfigurationIndex(UInt8 srConfigurationIndex)
{
    if (srConfigurationIndex > MAC_MAX_SRCONFIGURATIONINDEX)
    {
        lteWarning("Invalid range for srConfigurationIndex .\n");
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/* + CA_AB */
static void setCellConfigValues(UInt8 cellIndex)
{
     cellConfigUeSim_g[cellIndex].totalActiveUE = 0;    
     startRARNTI_g   = START_RARNTI; 
     {
     endRARNTI_g[cellIndex]     = END_RARNTI;
     }
     /*LTE_REG_FIX*/
	 /* +CA_NA */
	if (cellConfigUeSim_g[cellIndex].lowerCRNTIBound == START_RARNTI)
	{
		cellConfigUeSim_g[cellIndex].crntiRange = 
			(cellConfigUeSim_g[cellIndex].upperCRNTIBound - 
			 cellConfigUeSim_g[cellIndex].lowerCRNTIBound + 1) -
			(endRARNTI_g[cellIndex] - startRARNTI_g + 1);
	}
	else
	{
		cellConfigUeSim_g[cellIndex].crntiRange = 
			(cellConfigUeSim_g[cellIndex].upperCRNTIBound - 
			 cellConfigUeSim_g[cellIndex].lowerCRNTIBound + 1) ;
	}
	 /* -CA_NA */
     cellConfigUeSim_g[cellIndex].spscrntiRange = cellConfigUeSim_g[cellIndex].endSpsCrnti - 
         cellConfigUeSim_g[cellIndex].startSpsCrnti + 1;

#ifdef UT_TESTING
    MAC_EMULATOR_LOG(LOG_INFO,"*********** For CellIndex[%d] crntiRange = %d\n", cellIndex, cellConfigUeSim_g[cellIndex].crntiRange);
#endif
     
}
/* - CA_AB */
/****************************************************************************
 * Function Name  : parseRRCConnectionReqMsg 
 * Inputs         :
 *
 *
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : This function will prepare the RACH node.
 *
 ****************************************************************************/
void parseRRCConnectionReqMsg(UInt8 *msg_p, UInt16 msgLen, UInt8 cellIndex)
{
    RARequest raReq;
    UInt8 i = 0;
   
    while(i < MAX_NUM_RA_REQUEST)
    {
        raReq = raRntiArr_g[cellIndex][i];
        if ( INVALID_RNTI == raReq.RaRnti )
        {
             raReq.RaRnti = i+START_RARNTI;
             raReq.preamble = getNextPreambleIndex();
            
             memcpy(raReq.rrcConnectionReq, msg_p, msgLen);
             raReq.rrcConReqLen = msgLen;
             raReq.rachTypeFlag = CONTENTION_BASED_RACH;
             raRntiArr_g[cellIndex][raReq.RaRnti - 1] = raReq;
            break;
        }
        i++;
    }
    fapiPrepareRAInfo( raReq.RaRnti - 1, cellIndex);
}


/* HO related changes */

/****************************************************************************
 *    Function Name : macEmuChangeCrntiReq 
 *    Inputs        : from RRC change rnti from exisiting old rnti 
 *    Outputs       : 
 *    Returns       : 
 *    Description   : This function will change the new rnti with old crnti in 
 *                    UE Context & other data structures.
 ******************************************************************************/
UInt16 macEmuChangeCrntiReq(ChangeCrntiInfo *changedCrntiInfo_p, UInt8 oldCellIndex, UInt8 newCellIndex)
{
   MacRetType resp = MAC_SUCCESS;
   RNTIInfo *oldRntiInfo_p = PNULL, *newRntiInfo_p = PNULL;
   ueContextForUplink *ulUEContext_p = PNULL;

   ulUEContext_p = uplinkContextInfoMap_g[changedCrntiInfo_p->ueIndex];
   if ( PNULL == ulUEContext_p )
   {
       fprintf(stderr,"function [%s]->ue context is NULL for ueIndex = %d\n",__func__,
                     changedCrntiInfo_p->ueIndex );
       resp = MAC_INVALID_UE_ID;
       return resp;
   }
   /*Start: Fix for SPR 1158*/ 
   oldRntiInfo_p = getRntiInfofromRNTIMap(changedCrntiInfo_p->oldCrnti, oldCellIndex);
   if ( PNULL == oldRntiInfo_p )
   {
       fprintf(stderr,"function [%s]-> old rnti Info is NULL for rnti = %d, cellIndex=%d\n",__func__,
                     changedCrntiInfo_p->oldCrnti,oldCellIndex);
       resp = MAC_INVALID_UE_ID;
       return resp;
   }
   newRntiInfo_p = getRntiInfofromRNTIMap(changedCrntiInfo_p->newCrnti, newCellIndex);
   if ( PNULL == newRntiInfo_p )
   {
       fprintf(stderr,"function [%s]-> new rnti Info is NULL for rnti = %d, cellIndex=%d\n",__func__,
                     changedCrntiInfo_p->newCrnti,newCellIndex);
       resp = MAC_INVALID_UE_ID;
       return resp;
   }  
   /*End: Fix for SPR 1158*/ 
   if ( ulUEContext_p->crnti  == changedCrntiInfo_p->newCrnti )
   {
       return resp;
   }
   else if ( ulUEContext_p->crnti == changedCrntiInfo_p->oldCrnti )
   {
       ulUEContext_p->crnti = changedCrntiInfo_p->newCrnti;
   }
   newRntiInfo_p->state = oldRntiInfo_p->state;
   newRntiInfo_p->ueIndex = oldRntiInfo_p->ueIndex;
   newRntiInfo_p->rntiType = oldRntiInfo_p->rntiType;
   oldRntiInfo_p->ueIndex = MAX_UE_SUPPORTED;
   oldRntiInfo_p->state = UE_INACTIVE;
   oldRntiInfo_p->rntiType = NONE;
   /* SPR 11123 Changes Start */
   if(ulUEContext_p->resetFlag)
   {
     fprintf(stderr, "Reset Flag set to FALSE\n");
     ulUEContext_p->resetFlag = FALSE;
   }
   /* SPR 11123 Changes End */

   return resp;
   
}
/****************************************************************************
 *    Function Name : handleResetReq 
 *    Inputs        : from RRC reset trigger for rnti 
 *    Outputs       : 
 *    Returns       : 
 *    Description   : This function will trigger the MAC RESET procedure 
 *                    in hand over scenario.
 ******************************************************************************/
void handleResetReq( UInt8 *msg_p,UInt16 transactionId, UInt8 cellIndex )
{
   ResetRes resetRes = {0};
   ueContextForUplink *ulUEContext_p = PNULL;
   UInt16 ueIndex = 0;
   UInt16 resp = MAC_SUCCESS;
   ueIndex = LTE_GET_U16BIT(msg_p); 
   msg_p += 2;
  
   ulUEContext_p = uplinkContextInfoMap_g[ueIndex];
   if ( PNULL == ulUEContext_p )
   {
       fprintf(stderr,"function [%s]->ue context is NULL for ueIndex = %d\n",__func__,ueIndex );
       resp = MAC_INVALID_UE_ID;
   }
   /*SPR 5455 Fix start*/
   else
   {
       ulUEContext_p->resetFlag = TRUE;
       resetUeContext(ulUEContext_p);
   }
   /*SPR 5455 Fix end*/
   
   resetRes.idx = ueIndex;
   resetRes.response = resp;
   resetRes.transactionId = transactionId;
   sendMsgToRRC( MAC_EMU_RESET_CNF, &resetRes, PNULL, PNULL, cellIndex ); 
}
/****************************************************************************
 *    Function Name : triggerHOBasedRach 
 *    Inputs        : rnti, preamble, rachTypeFlag 
 *    Outputs       : 
 *    Returns       : 
 *    Description   : This function will trigger the rach procedure in hand over
 *                   scenario.
 ******************************************************************************/
void triggerHOBasedRach(UInt16 rnti, UInt8 preamble, UInt8 rachTypeFlag, 
                         UInt16 cellId, UInt16 ueCount, UInt8 cellIndex)
{
    RARequest raReq;
    UInt8 i = 0, raReqIndex = 0;
 
   /* check for preamble range based upon rach type triggered from UE L3 */ 
    if ( CONTENTION_BASED_RACH == rachTypeFlag 
         ||  CONTENTION_BASED_HO_RACH == rachTypeFlag
         /* SPR 5313 changes start */
         ||  CONTENTION_BASED_INTRA_ENB_HO_RACH == rachTypeFlag )
         /* SPR 5313 changes end */
    {
        if ( preamble > preambleRangeEnd_g )
        {
            fprintf(stderr,"preamble[%d] passed from UE L3 is not valid for CBR \
                     going out of range [%d]\n",preamble, preambleRangeEnd_g);
            return;
        }
        /* SPR 5313 changes start */ 
        rachTypeFlag = CONTENTION_BASED_HO_RACH;
        /* SPR 5313 changes end */
    }
    /* SPR 5313 changes start */ 
    else if ( CONTENTION_FREE_HO_RACH == rachTypeFlag || 
              CONTENTION_FREE_INTRA_ENB_HO_RACH == rachTypeFlag )  
    /* SPR 5313 changes end */ 
    {
        if ( preamble <= preambleRangeEnd_g )
        {
            fprintf(stderr,"preamble[%d] passed from UE L3 is not valid for CFR \
                     going out of range [%d]\n",preamble, preambleRangeEnd_g);
            return;
        }
        /* SPR 5313 changes start */ 
        rachTypeFlag = CONTENTION_FREE_HO_RACH ;
        /* SPR 5313 changes end */ 
    }
    else
    {
        fprintf(stderr,"rachType flag [%d] is not correct \n", rachTypeFlag);
        return;
    }
    while(i < MAX_NUM_RA_REQUEST)
    {
        raReq = raRntiArr_g[cellIndex][i];
        if ( INVALID_RNTI == raReq.RaRnti )
        {
            raReq.RaRnti = i + START_RARNTI;

            if(preamble != 0)
            { 
                raReq.preamble = preamble;
            }
            else
            {
                raReq.preamble = getNextPreambleIndex(cellIndex);
            }
            
            memset(raReq.rrcConnectionReq, 0, 100);
            raReq.rrcConReqLen = 0;
            raReq.rnti = rnti;
            raReq.rachTypeFlag = rachTypeFlag;
            raRntiArr_g[cellIndex][raReq.RaRnti - 1] = raReq;
            raReqIndex = i;
            break;
        }
        i++;
    }
    ueHoCount_g[cellIndex]++;
    /* check for cellId received in all the messages and if find any mismatch
       return immediatly */
    if (ueHoCount_g[cellIndex] == 1 )
    {
	    hoInfo_g[cellIndex][ueHoCount_g[cellIndex] - 1 ].raReqIndex = raReqIndex;
	    hoInfo_g[cellIndex][ueHoCount_g[cellIndex] - 1 ].cellId = cellId;
    }
    else
    {
	    hoInfo_g[cellIndex][ueHoCount_g[cellIndex]- 1 ].raReqIndex = raReqIndex;
	    hoInfo_g[cellIndex][ueHoCount_g[cellIndex] -1 ].cellId = cellId;
	    if ( hoInfo_g[cellIndex][ueHoCount_g[cellIndex]- 1].cellId != cellId )
	    {
		    fprintf(stderr,"cellId passed [%d] is not matching with the preveious passed cellId[%d]\n",
				    cellId,hoInfo_g[cellIndex][ueHoCount_g[cellIndex]- 1].cellId);
		    fprintf(stderr,"HO is aborintg cause of same\n");
		    return;
	    }
    }
    if ( ueHoCount_g[cellIndex] > HO_MAX_NUM_UE )
    {
        fprintf(stderr,"ueCount = %d received is more than supported for handover now\n",ueCount);
        return;
    }
    /* start RACH Procedure when ueCount match with ueHoCount_g[cellIndex] */
    if ( ueCount == ueHoCount_g[cellIndex] )
    {
        hoRachTriggerFlag[cellIndex] = TRUE;
        UInt8 ctr = 0;
        /* SPR 5313 changes start */
        if ( cellId_g[cellIndex] == hoInfo_g[cellIndex][ueHoCount_g[cellIndex] - 1 ].cellId )
        {
            return;
        }
        /* SPR 12823 fix start */
        for( ctr = 0; ctr < numCells_g; ctr++ )
        {
            cellId_g[ctr] = hoInfo_g[cellIndex][ueHoCount_g[cellIndex] - 1 ].cellId;
            if(cellId_g[ctr] == 0)
            {
        		/* SPR 18819 fix start */
                close(sockfd_g[ctr]);
                initSocketAddress(&phyLayerAddr_g[ctr],&sockfd_g[ctr], rxPorteNBMac_g[ctr]);
                /*set sockets Blocking or Non Blocking 0- NonBlocking/1-Blocking*/
                setSocketBlockingEnabled(sockfd_g[ctr],1);
                if ( bind(sockfd_g[ctr], (struct sockaddr*)&phyLayerAddr_g[ctr],
                            sizeof(SockAddr)) == -1)
                {
                    ltePanic("Bind Failed PHY RX PORT = %d", rxPorteNBMac_g[ctr]);
                }
		        /* SPR 18819 fix end */
            }
            else
            {
                /*START: HO Fix*/
                close(sockfdHO_g[ctr]);
                /* SPR 10868 Changes Start */

                initSocketAddress(&phyLayerAddrHO_g[ctr],&sockfdHO_g[ctr], rxPorteNBMacForHO_g[ctr]);
                /* SPR 10868 Changes End */
                /*set sockets Blocking or Non Blocking 0- NonBlocking/1-Blocking*/
                setSocketBlockingEnabled(sockfdHO_g[ctr],1);
                if ( bind(sockfdHO_g[ctr], (struct sockaddr*)&phyLayerAddrHO_g[ctr],
                            sizeof(SockAddr)) == -1)
                {
                    /*END: HO Fix*/
                    /* SPR 10868 Changes Start */
                    ltePanic("Bind Failed PHY RX PORT = %d", rxPorteNBMacForHO_g[ctr]);
                    /* SPR 10868 Changes End */
                }
                /* SPR 12823 fix end */
            }
        }
        /* SPR 5313 changes end */
    }
}
/****************************************************************************
 *    Function Name : parseAndtriggerHoRachReq 
 *    Inputs        :  from RRC
 *                  : Contention Free/Baced rach for Hand over.
 *    Outputs       : 
 *    Returns       : 
 *    Description   : This function parse the message get the preamble,
 *                   triggering rach flag and trigger rach procedure.
 ******************************************************************************/
void parseAndtriggerHoRachReq(UInt8 *msg_p,UInt16 transactionId,UInt8 cellIndex)
{
    UInt16 rnti = 0;
    UInt8 preamble = 0;
    /*SPR 19572 Fix Start*/
    UInt8 rachTypeFlag = 255;
    /*SPR 19572 Fix End */
    UInt16 cellId = 0,ueCount = 0;
    rnti = LTE_GET_U16BIT(msg_p);
    ueContextForUplink *ueContext_p = getUEContext(rnti, cellIndex);
    if ( PNULL == ueContext_p )
    {
        fprintf(stderr,"[%s] No ue context found for rnti = %d\n",__func__,rnti);
        return;
    }
    msg_p += 2;
    preamble = *msg_p;
/* incrementing for prach mack index */
    msg_p += 2;
    cellId = LTE_GET_U16BIT(msg_p);
    msg_p += 2;
    rachTypeFlag = *msg_p++;
    ueCount = LTE_GET_U16BIT(msg_p);
    msg_p += 2;

    fprintf(stderr,"reset flag = %d queueLoad = %d cellId_g = %d cellId = %d rachTypeFlag = %d\n",ueContext_p->resetFlag,ueContext_p->queueLoad, cellId_g[cellIndex], cellId, rachTypeFlag);

    if ( RLF_TRIGGER_NO_RACH == rachTypeFlag )
    {
	rlfDebugFlag = 1;
        return;	
    }
    if ( ULSYNC_START_RACH == rachTypeFlag )
    {
	rlfDebugFlag = 0;
        return;	
    }
    /* For reestablishment code changes */
    if ( REESTABLISHMENT_RACH == rachTypeFlag )
    {
        ueContext_p->resetFlag = FALSE;
        cellId_g[cellIndex] = cellId;
        return;
    } 
    /* SPR 5313 changes start */ 
    if( ( CONTENTION_FREE_INTRA_ENB_HO_RACH == rachTypeFlag || 
          CONTENTION_BASED_INTRA_ENB_HO_RACH == rachTypeFlag ) && 
         cellId_g[cellIndex] != cellId )
    {
        fprintf(stderr, "Cannot trigger RACH in INTRA_ENB_HO rachTypeFlag[%d] " 
                "cellId_g[cellIndex] = %d recvd cellId = %d\n", rachTypeFlag, cellId_g[cellIndex], cellId );
        ueContext_p->resetFlag = FALSE;
        return;
    }
 
    if (ueContext_p->resetFlag == TRUE && ueContext_p->queueLoad != 0 && 
        ( cellId_g[cellIndex] != cellId || 
          CONTENTION_FREE_INTRA_ENB_HO_RACH == rachTypeFlag || 
           CONTENTION_BASED_INTRA_ENB_HO_RACH == rachTypeFlag ) )
    /* SPR 5313 changes end */ 
    {
        fprintf(stderr,"HO RACH triggered rachTypeFlag = %d preamble = %d "
                "cellId = %d rnti = %d ueCount = %d cellId_g[cellIndex] = %d\n",
	            rachTypeFlag,preamble,cellId,rnti,ueCount,cellId_g[cellIndex]);
        triggerHOBasedRach(rnti, preamble, rachTypeFlag, cellId, ueCount,
                cellIndex);
    }
}

/* + CL-MIMO RSM */
#define NUM_OF_TX_ANTENNAS_2 2
#define NUM_OF_TX_ANTENNAS_4 4
/*4x4 DL MIMO CHG START*/
#define TX_MODE_3 3
#define TX_MODE_4 4
#define TX_MODE_6 6
#define RI_VALUE_2 2
#define RI_VALUE_4 4
/*4x4 DL MIMO CHG END*/
/****************************************************************************
 *    Function Name  : validateCBSR
 *    Inputs         : cbsrInfo received from RRC
 *                     : Transmission mode configured
 *    Outputs       : Whether codebookSubsetRestriction is correctly applied or not
 *    Returns       : Succes or failure
 *    Description  : This function checks whether at least one of the relevant bits of cbsrValue is
 *    one or not. For TM3, MSB two bits are checked. For TM4, MSB 6 bits are checked.
 *    Reference 36.331.
 ******************************************************************************/

static inline MacRetType validateCBSR( 
        codebookSubsetRestrictionInfo *cbsrInfo, 
        UInt8 transmissionMode, UInt8 rankIndicator,
        UInt8 cellIndex)
{
    printf("***INSIDE CBSR Validation TM [%d] TxAnt[%d]***\n",
           transmissionMode, cellConfigUeSim_g[cellIndex].numOfTxAnteenas);
    /* According to Table 7.2-1b of 36.213 */
    if (TX_MODE_3 == transmissionMode)
    {   /*4x4 DL MIMO CHG START*/
        if((rankIndicator == RI_VALUE_4) &&
           (0 == (cbsrInfo->cbsrValue[0]&0xF0)))
        {
           return MAC_FAILURE;
        }
        /*4x4 DL MIMO CHG END*/
        else
        {
            if( (cellConfigUeSim_g[cellIndex].numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2 )&& \
            (0 == (cbsrInfo->cbsrValue[0]&0xC0)) )
            {
                /*At least 1 of MSB two bits is not 1, so returning failure*/
                return MAC_FAILURE;
            }
            /* the first 2 bit of CBSR denoting rank 3/4 should be set to 0 for 4x2 MIMO */
            /* The next 2 bit denotes rank 1/2, atleast one of those 2 bit should be set */
            else if ( (cellConfigUeSim_g[cellIndex].numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)
                    && (rankIndicator == RI_VALUE_2) &&
                    (0x00 == (cbsrInfo->cbsrValue[0]&0x30)) )
            {
                return MAC_FAILURE;
            }
        }
    }
    else if((TX_MODE_4 == transmissionMode))
    {    
        /*4x4 DL MIMO CHG START*/
        if((rankIndicator == RI_VALUE_4)&&
           ((0 == cbsrInfo->cbsrValue[0])&&(0 == cbsrInfo->cbsrValue[1])&&
            (0 == cbsrInfo->cbsrValue[2])&&(0 == cbsrInfo->cbsrValue[3])&&
            (0 == cbsrInfo->cbsrValue[4])&&(0 == cbsrInfo->cbsrValue[5])&&
            (0 == cbsrInfo->cbsrValue[6])&&(0 == cbsrInfo->cbsrValue[7])))
        {
        /*At least 1 of last six bits is not 1, so returning failure*/
            return MAC_FAILURE;
        } 
        /*4x4 DL MIMO CHG END*/
        else
        {
            if((cellConfigUeSim_g[cellIndex].numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2 )&& 
                (0 == (cbsrInfo->cbsrValue[0]&0xFC)))
            {
               /*At least 1 of last six bits is not 1, so returning failure*/
               return MAC_FAILURE;
            }
            else if((cellConfigUeSim_g[cellIndex].numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4) 
                    && (rankIndicator == RI_VALUE_2))
            {
                /* the first 4 bytes in CBSR for 4x2 MIMO should always set to 0 */
                /* the last 4 bytes denotes the rank 1 and 2 related PMI support, so 
                 * atleast one bit indicating PMI support should be set */
                if (((0 != cbsrInfo->cbsrValue[0])|| (0 != cbsrInfo->cbsrValue[1])||
                 (0 != cbsrInfo->cbsrValue[2])|| (0 != cbsrInfo->cbsrValue[3])) ||
                ((0 == (cbsrInfo->cbsrValue[4]&0xFF))&&(0 == (cbsrInfo->cbsrValue[5]&0xFF))&&
                 (0 == (cbsrInfo->cbsrValue[6]&0xFF))&&(0 == (cbsrInfo->cbsrValue[7]&0xFF))) )
             {
                  return MAC_FAILURE;
             }
        }
        }
    }
    /* + TM6_5.2 */
    else if(TX_MODE_6 == transmissionMode)
    {
        if((cellConfigUeSim_g[cellIndex].numOfTxAnteenas == NUM_OF_TX_ANTENNAS_2 )&& \
            (0 == (cbsrInfo->cbsrValue[0]&0xF0)))
    {
        return MAC_FAILURE;
        }
        /* As TM6 mode supports single layer transmission, all 16 bits in CBSR can be set */
        else if( (cellConfigUeSim_g[cellIndex].numOfTxAnteenas == NUM_OF_TX_ANTENNAS_4)&&
                 ((0x00 == (cbsrInfo->cbsrValue[0]&0xFF)) &&
                  (0x00 == (cbsrInfo->cbsrValue[1]&0xFF))) )
        {
            return MAC_FAILURE;
        }
    }
    /* - TM6_5.2 */
    printf("Returnning form the validate CBSR [%d]\n",MAC_SUCCESS);
    return MAC_SUCCESS;
}
/* - CL-MIMO RSM */

/* ULA_CHG */
#define ULA_MAX_LINE_SIZE 600
/****************************************************************************
 * Function Name  : stringTokenArray
 * Inputs         : Char pointer to comma separated values string
 *                  Pointer to array where comma separated values are stored
 *                  numRows - number of rows in the array
 *                  numCol - number of columns in the array
 * Outputs        :
 * Returns        : void
 * Description    : converts input string containing comma separated values 
 *                  into an array of input numRows and numCol.
 *****************************************************************************/
UInt8  stringTokenArray(char *str, 
                       UInt8 *arr, 
                       UInt8 numRows, 
                       UInt8 numCol)
{
    UInt8 idx = 0;
    char *str1;
    UInt8 count = 0;

    /* extract first string from string sequence */
    str1 = strtok(str, ",");
    count++;

    *(arr+ numRows*numCol + idx) = atoi(str1);
    idx++;

    /* loop until finished */
    while (1)
    {
        /* extract string from string sequence */
        str1 = strtok(NULL, ",");

        /* check if there is nothing else to extract */
        if (str1 == NULL)
        {
            return count;
        }
        *(arr+ numRows*numCol + idx) = atoi(str1);
        count++;
        idx++;
    }
    return count;
}
/* +DYNAMIC_ICIC*/
/****************************************************************************
 * Function Name  : stringTokenSArray
 * Inputs         : Char pointer to comma separated values string
 *                  Pointer to array where comma separated values are stored
 *                  numRows - number of rows in the array
 *                  numCol - number of columns in the array
 * Outputs        :
 * Returns        : void
 * Description    : converts input string containing comma separated values
 *                  into an array of Signed input numRows and numCol.
 ******************************************************************************/
UInt8  stringTokenSArray(char *str,
                       SInt32 *arr,
                       UInt8 numRows,
                       UInt8 numCol)
{
        UInt8 idx = 0;
        char *str1;
        UInt8 count = 0;

        /* extract first string from string sequence */
        str1 = strtok(str, ",");
        count++;

        *(arr+ numRows*numCol + idx) = atoi(str1);
        idx++;

        /* loop until finished */
        while (1)
        {
            /* extract string from string sequence */
            str1 = strtok(NULL, ",");

            /* check if there is nothing else to extract */
            if (str1 == NULL)
            {
                 return count;
            }
            *(arr+ numRows*numCol + idx) = atoi(str1);
            count++;
            idx++;
        }
        return count;
}
/* -DYNAMIC_ICIC*/

/* +DYNAMIC_ICIC*/
/****************************************************************************
 * Function Name  : stringTokenS1DArray
 * Inputs         : Char pointer to comma separated values string
 *                  Pointer to array where comma separated values are stored
 *                  in the array 
 * Outputs        :
 * Returns        : UInt8-number of values
 * Description    : converts input string containing comma separated values
 *                  into a single dimension  array of Signed inputs. 
 ******************************************************************************/
UInt8  stringTokenS1DArray(char *str,
                       SInt32 *arr,
                       UInt8  num)
{
        UInt8 idx = 0;
        char *str1;
        UInt8 count = 0;

        /* extract first string from string sequence */
        str1 = strtok(str, ",");
        count++;

        *(arr+idx) = atoi(str1);
        idx++;

        /* loop until finished */
        while (idx<num)
        {
            /* extract string from string sequence */
            str1 = strtok(NULL, ",");

            /* check if there is nothing else to extract */
            if (str1 == NULL)
            {
                 return count;
            }
            *(arr+idx) = atoi(str1);
            count++;
            idx++;
        }
        return count;
}
/* -DYNAMIC_ICIC*/

/* +DYNAMIC_ICIC*/
/****************************************************************************
 * Function Name  : stringTokenU1DArray
 * Inputs         : Char pointer to comma separated values string
 *                  Pointer to array where comma separated values are stored
 *                  in the array 
 * Outputs        :
 * Returns        : UInt8-number of values
 * Description    : converts input string containing comma separated values
 *                  into a single dimension  array of Unsigned inputs. 
 ******************************************************************************/
UInt8  stringTokenU1DArray(char *str,
                       UInt8 *arr,
                       UInt8  num)
{
        UInt8 idx = 0;
        char *str1;
        UInt8 count = 0;

        /* extract first string from string sequence */
        str1 = strtok(str, ",");
        count++;

        *(arr+idx) = atoi(str1);
        idx++;

        /* loop until finished */
        while (idx<num)
        {
            /* extract string from string sequence */
            str1 = strtok(NULL, ",");

            /* check if there is nothing else to extract */
            if (str1 == NULL)
            {
                 return count;
            }
            *(arr+idx) = atoi(str1);
            count++;
            idx++;
        }
        return count;
}

/* -DYNAMIC_ICIC*/


/****************************************************************************
 * Function Name  : trimSpaces
 * Inputs         : char pointer to str.
 * Outputs        : void.
 * Returns        : char pointer to str.
 * Description    : Trims spaces from input string. 
 *****************************************************************************/
void trimSpaces(char *str) /* ULA_UTP */
{
    char ptr[strlen(str)+1];
    int i,j=0;
    for(i=0;str[i]!='\0';i++)
    {
        if (str[i] != ' ' && str[i] != '\t')
            ptr[j++]=str[i];
    }
    ptr[j]='\0';
    strncpy(str,ptr,strlen(ptr)); /* ULA_UTP */
    str[j] = '\0'; /* ULA_UTP */
    //return str;
}
/* + E_CID_5.3 */
/****************************************************************************
 * Function Name  : parseUERel9TAValues
 * Inputs         : 
 *
 *
 * Outputs        :
 * Returns        : void
 * Description    : Parse eCIDConfig.cfg file to configure different Rel9 TA
 *                  values
 *****************************************************************************/
void  parseUERel9TAValues()
{
	SInt8 cellIndex = -1;
		fprintf(stderr, "\n [%s]", __func__);
	FILE    *fp;
	UInt16  length                    = 0;
	Char8   buf[E_CID_MAX_LINE_SIZE]   = {0};
	Char8   temp[E_CID_MAX_LINE_SIZE]   = {0};

	fp = fopen(UE_E_CID_FILE_PATH,"r");
	if (fp == NULL)
	{
		fprintf(stderr,"Unable to open file - eCIDConfig.cfg PATH\n");
	}
	while( (fgets(buf,E_CID_MAX_LINE_SIZE,fp) != NULL) && cellIndex < numCells_g )
	{
		if((strncmp(buf,"#",1) == 0) || (strncmp(buf,"\n",1) == 0))
		{
			continue;
		}

		trimSpaces(buf);
		length = strlen(buf);
		if(strncmp(buf,"[CELL]",6)==0)
		{
			fprintf(stderr, "\n 1. [%s] cellIndex = %d", __func__,cellIndex);
			strncpy(temp,buf+6,length-6);
			cellIndex++;
		}
		if (cellIndex >= numCells_g)
		break;

		if (strncmp(buf,"TA_REL9_VALUE_RACH",18) == 0)
		{
		fprintf(stderr,"\nTA_REL9_VALUE_RACH, cellIndex = %d ", cellIndex);
			strncpy(temp,buf+19,length-19);
			rel9TaValues_g[cellIndex].rel9TARach[rel9TaValues_g
				[cellIndex].numTaRach] = atoi(temp);
			rel9TaValues_g[cellIndex].numTaRach++;
			fprintf(stderr, "\n NUM RACH = %d", rel9TaValues_g[cellIndex].numTaRach);
		}

		else if(strncmp(buf,"TA_REL9_VALUE_ULSCH",19) == 0)
		{
			strncpy(temp,buf+20,length-20);
			rel9TaValues_g[cellIndex].rel9TAUlsch[rel9TaValues_g
				[cellIndex].numTaUlSch] = atoi(temp);
			rel9TaValues_g[cellIndex].numTaUlSch++;
		}

		else if(strncmp(buf,"TA_REL9_VALUE_CQI",17) == 0)
		{
			strncpy(temp, buf+18,length-18);
			rel9TaValues_g[cellIndex].rel9TACqi[rel9TaValues_g
				[cellIndex].numTaCqi] = atoi(temp);
			rel9TaValues_g[cellIndex].numTaCqi++;
		}

		else if(strncmp(buf,"TA_REL9_VALUE_SRS",17) == 0) 
		{
			strncpy(temp, buf +18,length-18);
			rel9TaValues_g[cellIndex].rel9TASrs[rel9TaValues_g
				[cellIndex].numTaSrs] = atoi(temp);
			rel9TaValues_g[cellIndex].numTaSrs++; 
		} 
	}
	cellIndex = cellIndex -1;
	fprintf(stderr,"\n *** 2 *** cellIndex = %d, numTaRach = %d, numTaUlSch = %d, numTaCqi = %d, numTaSrs = %d", cellIndex, rel9TaValues_g[cellIndex].numTaRach, rel9TaValues_g[cellIndex].numTaUlSch, rel9TaValues_g[cellIndex].numTaCqi, rel9TaValues_g[cellIndex].numTaSrs);
	return;

}

/* - E_CID_5.3 */


/****************************************************************************
 * Function Name  : parseUeProfileConfig
 * Inputs         : pointer to struct ueProfile
 *
 *
 * Outputs        :
 * Returns        : void
 * Description    : Parse ueProfileConfig.cfg file to configure different ue
 *                  profiles
 *****************************************************************************/
void parseUeProfileConfig(ueProfile ueProfileInfo_p[][MAX_UE_PROFILES])
{
	SInt8 cellIndex = -1;
	FILE *fp;
	UInt8  length;
	UInt8 line[MAX_LINE_SIZE] = {0};
	UInt8 temp[MAX_LINE_SIZE] = {0};
	UInt8 idx = 0;

	fp = fopen(UE_PROFILE_FILE_PATH,"r");
	if (fp == NULL)
	{
		ltePanic("Unable to open file.\n");
	}
	while( (fgets(line,MAX_LINE_SIZE,fp) != NULL) && cellIndex < numCells_g )
	{
		if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
		{
			continue;
		}

		trimSpaces(line);
			if(strncmp(line,"[CELL]",6)==0)
			{
				length = strlen(line);
				strncpy(temp,line+6,length-6);
				cellIndex++;
			}
			if (cellIndex >= numCells_g)
				break;
			if(strncmp(line,"[UE]",4)==0)
			{
				for(idx = 0 ;idx < NUM_PROFILE_ID; idx++)
				{
					if (fgets(line,MAX_LINE_SIZE,fp) != NULL)
					{
						if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
						{
							idx--;
							continue;
						}

						trimSpaces(line);

						memset(temp,0,MAX_LINE_SIZE);
						length = strlen(line);
						if (strncmp(line,"srsProfileId",12) == 0)
						{
							strncpy(temp,line+13,length-13);
							ueProfileInfo_p[cellIndex][ueProfileNum[cellIndex]].\
								srsProfileId = atoi(temp);
						}
						else if (strncmp(line,"ulschProfileId",14) == 0)
						{
							strncpy(temp,line+15,length-15);
							ueProfileInfo_p[cellIndex][ueProfileNum[cellIndex]].\
								ulschProfileId = atoi(temp);
						}
						/*CLPC_CHG*/
						/*Parsing the PHR Profile Id*/
						else if(strncmp(line,"phrProfileId",12) == 0)
						{
							strncpy(temp,line+13,length-13);
							ueProfileInfo_p[cellIndex][ueProfileNum[cellIndex]].\
								phrProfileId = atoi(temp);
#ifdef PHR_TESTING
							fprintf(stderr,"PhrId parsed Value = %d\n",
									ueProfileInfo_p[cellIndex][ueProfileNum[cellIndex]].\
									phrProfileId);
#endif
						}
						else if (strncmp(line,"cqiProfileId",12) == 0)
						{
							strncpy(temp,line+13,length-13);
							ueProfileInfo_p[cellIndex][ueProfileNum[cellIndex]].\
								cqiProfileId = atoi(temp);
						}
						/* + CQI_4.1 */
						else if (strncmp((const char*)line,"dlCqiProfileId",14) == 0)
						{
							strncpy(temp,line+15,length-15);
							ueProfileInfo_p[cellIndex][ueProfileNum[cellIndex]].\
								dlCqiProfileId = atoi(temp);
						}
						/* - CQI_4.1 */
					}
				}
				ueProfileNum[cellIndex]++;
			}
		}
	fclose(fp);
}

/****************************************************************************
 * Function Name  : createUeProfileConfig
 * Inputs         : pointer to local structure ueProfile
 *                  containing values parsed from ueProfileconfig.cfg.
 *
 * Outputs        : 
 * Returns        : void
 * Description    : create ue profile 
 *****************************************************************************/
void createUeProfileConfig(ueProfile ueProfileInfo_p[][MAX_UE_PROFILES])
{
	UInt8 idx       = 0;
	UInt8 retVal    = 0;
	UInt8 cellIndex = 0;
	for (cellIndex = 0; cellIndex < numCells_g; cellIndex++)
	{
		for(idx=0;idx<ueProfileNum[cellIndex];idx++)
		{
			retVal = validateSrsProfile(ueProfileInfo_p[cellIndex][idx].\
					srsProfileId);
			if (retVal == MAC_SUCCESS)
			{
				ueProfileInfo_g[cellIndex][idx].srsProfileId =
					ueProfileInfo_p[cellIndex][idx].srsProfileId;
			}
			else
			{
				ueProfileInfo_g[cellIndex][idx].srsProfileId = 0;
			}
			retVal = validateUlschProfile(ueProfileInfo_p[cellIndex][idx].\
					ulschProfileId);
			if (retVal == MAC_SUCCESS)
			{
				ueProfileInfo_g[cellIndex][idx].ulschProfileId =
					ueProfileInfo_p[cellIndex][idx].ulschProfileId;
			}
			else
			{
				ueProfileInfo_g[cellIndex][idx].ulschProfileId = 0;
			}
			/*CLPC_CHG*/
			/*Validate Phr Profile Id*/
			retVal = validatePHRId(ueProfileInfo_p[cellIndex][idx].\
					phrProfileId);
			if (retVal == MAC_SUCCESS)
			{
				ueProfileInfo_g[cellIndex][idx].phrProfileId =
					ueProfileInfo_p[cellIndex][idx].phrProfileId;
#ifdef PHR_TESTING
				fprintf(stderr,"PhrId is validated.Value = %d\n",
						ueProfileInfo_p[cellIndex][idx].phrProfileId);
#endif
			}
			else
			{
				/*Setting it to invalid profile id*/
				ueProfileInfo_g[cellIndex][idx].phrProfileId = PHR_INACTIVE;
#ifdef PHR_TESTING
				fprintf(stderr,"PhrId is not validated. Putting Inactive Value = %d\n",
						ueProfileInfo_g[cellIndex][idx].phrProfileId);
#endif
			}
			retVal = validateCqiProfile(ueProfileInfo_p[cellIndex][idx].\
					cqiProfileId);
			if (retVal == MAC_SUCCESS)
			{
				ueProfileInfo_g[cellIndex][idx].cqiProfileId =
					ueProfileInfo_p[cellIndex][idx].cqiProfileId;
			}
			else
			{
				ueProfileInfo_g[cellIndex][idx].cqiProfileId = 0;
			}
			/* + CQI_4.1 */
			retVal = validateDlCqiProfile(ueProfileInfo_p[cellIndex][idx].\
					dlCqiProfileId);
			if (retVal == MAC_SUCCESS)
			{
				ueProfileInfo_g[cellIndex][idx].dlCqiProfileId =
					ueProfileInfo_p[cellIndex][idx].dlCqiProfileId;
			}
			else
			{
				ueProfileInfo_g[cellIndex][idx].dlCqiProfileId = 0;
			}
			/* - CQI_4.1 */
		}
	}
}

/****************************************************************************
 * Function Name  : validateSrsProfile
 * Inputs         : srsProfileId value 
 *
 *
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate srsProfileId profile values
 *****************************************************************************/
MacRetType validateSrsProfile(UInt8 srsProfileId)
{
    if(srsProfileId >= 0 && srsProfileId<= MAX_SRS_PROFILES)
    {
        return MAC_SUCCESS;
    }
    return MAC_FAILURE;
}

/****************************************************************************
 * Function Name  : validateUlschProfile
 * Inputs         : ulschProfileId value 
 *
 *
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate ulsch profile values
 *****************************************************************************/
MacRetType validateUlschProfile(UInt8 ulschProfileId)
{
    if(ulschProfileId >= 0 && ulschProfileId <= MAX_ULSCH_PROFILES)
    {
        return MAC_SUCCESS;
    }
    return MAC_FAILURE;
}

/* + CQI_4.1 */
/****************************************************************************
 * Function Name  : validateDlCqiProfile
 * Inputs         : dlCqiProfileId value 
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate cqi profile values
 *****************************************************************************/
MacRetType validateDlCqiProfile(UInt8 dlCqiProfileId)
{
    if(dlCqiProfileId >= 0 && dlCqiProfileId <= MAX_CQI_PROFILE)
    {
        return MAC_SUCCESS;
    }
    return MAC_FAILURE;
}
/* + CQI_4.1 */

/* CLPC_CHG*/
/****************************************************************************
 * Function Name  : validateCqiProfile
 * Inputs         : ulschProfileId value 
 *
 *
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate cqi profile values
 *****************************************************************************/
MacRetType validateCqiProfile(UInt8 cqiProfileId)
{
    if(cqiProfileId >= 0 && cqiProfileId <= MAX_ULSCH_PROFILES)
    {
        return MAC_SUCCESS;
    }
    return MAC_FAILURE;
}
/* CLPC_CHG*/
/*Added For Power Control*/
/****************************************************************************
 * Function Name  : validatePHRId
 * Inputs         : phrProfileId value 
 *
 *
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate phr profile values
 *****************************************************************************/
MacRetType validatePHRId(UInt8 phrProfileId)
{
    if(phrProfileId >= 0 && phrProfileId < MAX_PHR_PROFILES)
    {
        return MAC_SUCCESS;
    }
    return MAC_FAILURE;
}

#define PHR_MAX_LINE_SIZE 600
/****************************************************************************
 * Function Name  : parseUePHRConfig
 * Inputs         : pointer to struct PHRProfile
 * Outputs        : None
 * Returns        : void
 * Description    : Parse uePHRConfig.cfg to configure various PHR profiles
 *****************************************************************************/
void parseUePHRConfig(PHRProfile phrProfileInfo_p[][MAX_PHR_PROFILES],
                    UInt8 phrValues[])
{
	FILE *fp;
	UInt16  length                   = 0;
	UInt8 line[PHR_MAX_LINE_SIZE]   = {0};
	UInt8 temp[PHR_MAX_LINE_SIZE]   = {0};
	UInt8 index                     = 0;
	UInt8 idx = 0;
	UInt8 count = 0;
	UInt8 tempCount = 0;
	SInt8 cellIndex = -1;

	fp = fopen(UE_PHR_FILE_PATH,"r");
	if (fp == NULL)
	{
		printf("Unable to open file.\n");
	}

	while((fgets(line,PHR_MAX_LINE_SIZE,fp) != NULL) && cellIndex < numCells_g)
	{
		if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
		{
			continue;
		}

		trimSpaces(line);
		length = strlen(line);

		if(strncmp(line,"[CELL]",6)==0)
		{
			strncpy(temp,line+6,length-6);
			cellIndex++;
		}
		if (cellIndex >= numCells_g)
			break;
		if(strncmp(line,"[PHR]",5)==0)
		{
			for(index = 0 ;index < NUM_PHR_PARAMETER; index++)
			{
				if(fgets(line,PHR_MAX_LINE_SIZE,fp) != NULL)
				{
					if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
					{
						index--;
						continue;
					}

					trimSpaces(line);
					memset(temp,0,PHR_MAX_LINE_SIZE);
					length=strlen(line);

					if (strncmp(line,"phrCount",8) == 0)
					{
						strncpy(temp,line+9,length-9);
						phrProfileInfo_p[cellIndex][phrProfileNum[cellIndex]].
							phrCount = atoi(temp);
						tempCount = phrProfileInfo_p[cellIndex]
							[phrProfileNum[cellIndex]].phrCount;
#ifdef PHR_TESTING
						fprintf(stderr,"CellIndex [%d] phrCount=%d\n",
								cellIndex,phrProfileInfo_p[cellndex]
								[phrProfileNum[cellIndex]].phrCount);
#endif
					}
					else if(strncmp(line,"phrValues",9) == 0)
					{
						strncpy(temp,line+10,length-10);
						count = stringTokenArray(temp,(unsigned char *)phrValues,
								0,MAX_COUNT_VALUE);

						if(count != tempCount)
						{
							phrProfileInfo_p[cellIndex][phrProfileNum
								[cellIndex]].phrCount = 1;
							phrProfileInfo_p[cellIndex][phrProfileNum
								[cellIndex]].phrValues[0] = 0;
#ifdef PHR_TESTING
							fprintf(stderr,"counts do not match,phrCount=%d,phrValue=%d\n",
									phrProfileInfo_p[cellIndex]
									[phrProfileNum[cellIndex]].phrCount,
									phrProfileInfo_p[phrProfileNum].phrValues[0]);
#endif
						}
						else
						{
							for(idx = 0; idx < tempCount; idx++)
							{
								phrProfileInfo_p[cellIndex][
									phrProfileNum[cellIndex]].
									phrValues[idx] = phrValues[idx];
#ifdef PHR_TESTING
								fprintf(stderr,"CellIndex [%d] phrValues=%d\n",
										phrProfileInfo_p[cellIndex]
										[phrProfileNum[cellIndex]].
										phrValues[idx]);
#endif
							}
						}
					}
					else if(strncmp(line,"phrTimer",8) == 0)
					{
						strncpy(temp,line+9,length-9);
						phrProfileInfo_p[cellIndex][phrProfileNum
							[cellIndex]].phrTimer = atoi(temp);
#ifdef PHR_TESTING
						fprintf(stderr,"CellIndex [%d] phrTimer=%d\n",
								phrProfileInfo_p[cellIndex]
								[phrProfileNum[cellIndex]].phrTimer);
#endif
					}
				}
			}
			phrProfileNum[cellIndex]++;
		}
	}
	fclose(fp);
}
/* +DYNAMIC_ICIC*/
/****************************************************************************
 * Function Name  : parseL1MeasReport
 * Inputs         :
 * Outputs        :
 * Returns        : void
 * Description    : Parse l1MeasurementConfig.cfg to configure 
 *                  various Interference profiles as global variable
 *****************************************************************************/
//#define UE_INTER_FILE_PATH "../../../../../cfg/l1MeasurementConfig.cfg"//diclaration should be included in header file
UInt8  interferenceEnb_g[MAX_NUM_CELLS];
SInt32 interValues_g [MAX_NUM_CELLS][MAX_INTER_PROFILES][MAX_PRB];
SInt32 interProfileNum_g[MAX_NUM_CELLS];
UInt8  dlRsTxPowerEnb_g[MAX_NUM_CELLS];
UInt8  dlRsTxPowerNum_g[MAX_NUM_CELLS];
SInt32 dlRsTxValues_g[MAX_NUM_CELLS][100];
UInt8  thermalNoiseEnb_g[MAX_NUM_CELLS];
UInt8  thermalNoiseNum_g[MAX_NUM_CELLS];
SInt32 thermalNoiseValues_g[MAX_NUM_CELLS][100];
UInt8  measEnabled_g[MAX_NUM_CELLS];
UInt8  interSequence_g[MAX_NUM_CELLS][MAX_INTER_PROFILES];
UInt32 measReportPeriodicity_g[MAX_NUM_CELLS];
void parseL1MeasReport()
{
	FILE *fp;
	UInt16  length                   = 0;/* ULA_UTP *///**
	UInt8 line[ULA_MAX_LINE_SIZE]   = {0};//**
	UInt8 temp[ULA_MAX_LINE_SIZE]   = {0};//**
	UInt8 idx                   = 0;//**
	UInt8 count = 0;
	SInt8 cellIndex = -1;

	fp = fopen(UE_MEAS_REPORT_FILE_PATH,"r");



	if (fp == NULL)
	{
		printf("Unable to open file.\n");
	}

	while((fgets(line,ULA_MAX_LINE_SIZE,fp) != NULL)&& cellIndex < numCells_g)
	{
		if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
		{
			continue;
		}

		trimSpaces(line);
		length = strlen(line);

		if (strncmp(line,"[CELL]",6) == 0)
		{
			strncpy(temp,line+6,length-6);
			cellIndex++;
		}
		if (cellIndex >= numCells_g)
			break;
		if (strncmp(line,"ENABLED",7) == 0)
		{
			strncpy(temp,line+8,length-8);
			measEnabled_g[cellIndex]=atoi(temp);
		}
		if (strncmp(line,"PERIODICITY",11) == 0)
		{
			strncpy(temp,line+12,length-12);
			measReportPeriodicity_g[cellIndex]=atoi(temp);
		}

		else if (strncmp(line,"INTER_PROF1",11) == 0)
		{
			strncpy(temp,line+12,length-12);
			count = stringTokenSArray(temp,(signed int *)
					interValues_g[cellIndex],
					interProfileNum_g[cellIndex],MAX_PRB);
			interProfileNum_g[cellIndex]++;
		}
		else if (strncmp(line,"INTER_PROF2",11) == 0)
		{
			strncpy(temp,line+12,length-12);

			count = stringTokenSArray(temp,(signed int *)
					interValues_g[cellIndex],
					interProfileNum_g[cellIndex],MAX_PRB);
			interProfileNum_g[cellIndex]++;
		}
		else if (strncmp(line,"INTER_PROF3",11) == 0)
		{
			strncpy(temp,line+12,length-12);
			count = stringTokenSArray(temp,(signed int *)
					interValues_g[cellIndex],
					interProfileNum_g[cellIndex],MAX_PRB);
			interProfileNum_g[cellIndex]++;
		}
		else if (strncmp(line,"INTER_PROF4",11) == 0)
		{
			strncpy(temp,line+12,length-12);
			count = stringTokenSArray(temp,(signed int *)
					interValues_g[cellIndex],
					interProfileNum_g[cellIndex],MAX_PRB);
			interProfileNum_g[cellIndex]++;
		}
		else if (strncmp(line,"INTER_PROF5",11) == 0)
		{
			strncpy(temp,line+12,length-12);
			count = stringTokenSArray(temp,(signed int *)
					interValues_g[cellIndex],
					interProfileNum_g[cellIndex],MAX_PRB);
			interProfileNum_g[cellIndex]++;
		}
		else if (strncmp(line,"INTERFERENCE_REPORT_SEQUENCE",28) == 0)
		{
			strncpy(temp,line+29,length-29);
			count = stringTokenU1DArray(temp,(unsigned char *)
					interSequence_g[cellIndex],MAX_INTER_PROFILES);
		}
		else if (strncmp(line,"DLRSTXPOWER_NUM",15) == 0)
		{
			strncpy(temp,line+16,length-16);
			dlRsTxPowerNum_g[cellIndex]=atoi(temp);
		}
		else if (strncmp(line,"DLRSTXPOWER_VAL",15) == 0)
		{
			strncpy(temp,line+16,length-16);
			stringTokenS1DArray(temp,(signed int *)dlRsTxValues_g[cellIndex],
					dlRsTxPowerNum_g[cellIndex]);
		}

		else if (strncmp(line,"THERMAL_NOISE_NUM",17) == 0)
		{
			strncpy(temp,line+18,length-18);
			thermalNoiseNum_g[cellIndex]=atoi(temp);
		}
		else if (strncmp(line,"THERMAL_NOISE_VAL",17) == 0)
		{
			strncpy(temp,line+18,length-18);
			stringTokenS1DArray(temp,(signed int *)thermalNoiseValues_g[cellIndex],
					thermalNoiseNum_g[cellIndex]);
		}
		else if (strncmp(line,"INTER_PROF_ENABLED",18) == 0)
		{
			strncpy(temp,line+19,length-19);
			interferenceEnb_g[cellIndex]=atoi(temp);
		}
		else if (strncmp(line,"DLRSTXPOWER_ENABLED",19) == 0)
		{
			strncpy(temp,line+20,length-20);
			dlRsTxPowerEnb_g[cellIndex]=atoi(temp);
		}
		else if (strncmp(line,"THERMAL_NOISE_ENABLED",21) == 0)
		{
			strncpy(temp,line+22,length-22);
			thermalNoiseEnb_g[cellIndex]=atoi(temp);
		}



	}
		close(fp);
}
/* -DYNAMIC_ICIC*/

/****************************************************************************
 * Function Name  : parseUeSINRConfig
 * Inputs         : pointer to struct SRSProfile
 *                  pointer to struct ULSCHProfile
 *                  srsSinrValues array
 *                  ulschSinrValues array
 * Outputs        : 
 * Returns        : void
 * Description    : Parse ueSINRConfig.cfg to configure various SRS profiles
 *****************************************************************************/
void parseUeSINRConfig(SRSProfile *srsProfileInfo_p,
                            ULSCHProfile *ulschProfileInfo_p,
                            CQIProfile *cqiProfileInfo_p,
                            UInt8 srsSinrValues [][MAX_PRB], 
                            UInt8 ulschSinrValues [],
                            UInt8 cqiSinrValues [])
{
    FILE *fp;
    UInt16  length                   = 0;/* ULA_UTP */
    UInt8 line[ULA_MAX_LINE_SIZE]   = {0};
    UInt8 temp[ULA_MAX_LINE_SIZE]   = {0};
    UInt8 idx                   = 0;
    UInt8 count = 0;


    fp = fopen(UE_SINR_FILE_PATH,"r");
    if (fp == NULL)
    {
        printf("Unable to open file.\n");
    }

    while(fgets(line,ULA_MAX_LINE_SIZE,fp) != NULL)
    {
        if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
        {
            continue;
        }

        trimSpaces(line);
        length = strlen(line);

        if (strncmp(line,"SRS_SINR_PROF1",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            count = stringTokenArray(temp,(unsigned char *)srsSinrValues,
                    srsSinrProfileNum,MAX_PRB);
            srsSinrProfileNum++;
        }
        else if (strncmp(line,"SRS_SINR_PROF2",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            
            count = stringTokenArray(temp,(unsigned char *)srsSinrValues,
                    srsSinrProfileNum,MAX_PRB);
            srsSinrProfileNum++;
        }
        else if (strncmp(line,"SRS_SINR_PROF3",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            count = stringTokenArray(temp,(unsigned char *)srsSinrValues,
                    srsSinrProfileNum,MAX_PRB);
            srsSinrProfileNum++;
        }
        else if (strncmp(line,"SRS_SINR_PROF4",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            count = stringTokenArray(temp,(unsigned char *)srsSinrValues,
                    srsSinrProfileNum,MAX_PRB);
            srsSinrProfileNum++;
        }
        else if (strncmp(line,"SRS_SINR_PROF5",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            count = stringTokenArray(temp,(unsigned char *)srsSinrValues,
                    srsSinrProfileNum,MAX_PRB);
            srsSinrProfileNum++;
        }
        else if (strncmp(line,"ULSCH_SINR_PROF1",16) == 0)
        {
            strncpy(temp,line+17,length-17);
            ulschSinrValues[ulschSinrProfileNum]=atoi(temp);
            ulschSinrProfileNum++;
        }
        else if (strncmp(line,"ULSCH_SINR_PROF2",16) == 0)
        {
            strncpy(temp,line+17,length-17);
            ulschSinrValues[ulschSinrProfileNum]=atoi(temp);
            ulschSinrProfileNum++;
        }
        else if (strncmp(line,"ULSCH_SINR_PROF3",16) == 0)
        {
            strncpy(temp,line+17,length-17);
            ulschSinrValues[ulschSinrProfileNum]=atoi(temp);
            ulschSinrProfileNum++;
        }
        else if (strncmp(line,"ULSCH_SINR_PROF4",16) == 0)
        {
            strncpy(temp,line+17,length-17);
            ulschSinrValues[ulschSinrProfileNum]=atoi(temp);
            ulschSinrProfileNum++;
        }
        else if (strncmp(line,"ULSCH_SINR_PROF5",16) == 0)
        {
            strncpy(temp,line+17,length-17);
            ulschSinrValues[ulschSinrProfileNum]=atoi(temp);
            ulschSinrProfileNum++;
        }
        /* CLPC_CHG */
        else if (strncmp(line,"CQI_SINR_PROF1",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            cqiSinrValues[cqiSinrProfileNum]=atoi(temp);
            cqiSinrProfileNum++;
        }
        else if (strncmp(line,"CQI_SINR_PROF2",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            cqiSinrValues[cqiSinrProfileNum]=atoi(temp);
            cqiSinrProfileNum++;
        }
        else if (strncmp(line,"CQI_SINR_PROF3",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            cqiSinrValues[cqiSinrProfileNum]=atoi(temp);
            cqiSinrProfileNum++;
        }
        else if (strncmp(line,"CQI_SINR_PROF4",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            cqiSinrValues[cqiSinrProfileNum]=atoi(temp);
            cqiSinrProfileNum++;
        }
        else if (strncmp(line,"CQI_SINR_PROF5",14) == 0)
        {
            strncpy(temp,line+15,length-15);
            cqiSinrValues[cqiSinrProfileNum]=atoi(temp);
            cqiSinrProfileNum++;
        }
        /* CLPC_CHG */
        else if(strncmp(line,"[SRS]",5)==0)
        {
            for(idx = 0 ;idx < NUM_SRS_PARAMETER; idx++)
            {
                if(fgets(line,ULA_MAX_LINE_SIZE,fp) != NULL)
                {
                    if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
                    {
                        idx--;
                        continue;
                    }

                    trimSpaces(line);

                    memset(temp,0,ULA_MAX_LINE_SIZE);
                    length=strlen(line);
                    if (strncmp(line,"startPosition",13) == 0)
                    {
                        strncpy(temp,line+14,length-14);
                        srsProfileInfo_p[srsProfileNum].startPosition = atoi(temp);
                    }
                    else if (strncmp(line,"srsBandwidth",12) == 0)
                    {
                        strncpy(temp,line+13,length-13);
                        srsProfileInfo_p[srsProfileNum].srsBandwidth = atoi(temp);
#if 1
                        /*Upscaling the srsBandwith to a multiple of 4 in case it
                         * is not a multiple of 4                                                                                                    */
                        if((srsProfileInfo_p[srsProfileNum].srsBandwidth% 4))
                        {
                            srsProfileInfo_p[srsProfileNum].srsBandwidth += \
                                 (4-(srsProfileInfo_p[srsProfileNum].srsBandwidth % 4));
                        }
#endif
                    }
                    else if (strncmp(line,"hoppingEnabled",14) == 0)
                    {
                        strncpy(temp,line+15,length-15);
                        srsProfileInfo_p[srsProfileNum].hoppingEnable = atoi(temp);
                    }
                    else if (strncmp(line,"sinrProfileSequence",19) == 0)
                    {
                        strncpy(temp,line+20,length-20);
                        count = stringTokenArray(temp,(unsigned char *)srsProfileInfo_p
                                [srsProfileNum].sinrSeq,0,srsSinrProfileNum);
                    }
                }
            }
            srsProfileNum++;
        }
        else if(strncmp(line,"[ULSCH]",7)==0)
        {
            for(idx = 0 ;idx < NUM_ULSCH_PARAMETER; idx++)
            {
                if(fgets(line,ULA_MAX_LINE_SIZE,fp) != NULL)
                {
                    if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
                    {
                        idx--;
                        continue;
                    }
                    trimSpaces(line);
                    length=strlen(line);
                    if (strncmp(line,"sinrProfileSequence",19) == 0)
                    {
                        strncpy(temp,line+20,length-20);
                        count = stringTokenArray(temp,(unsigned char *)ulschProfileInfo_p
                                [ulschProfileNum].sinrSeq,0,ulschSinrProfileNum);
                    }
                }
            }
            ulschProfileNum++;
        }
        else if(strncmp(line,"[CQI]",5)==0)
        {
           for(idx = 0 ;idx < NUM_CQI_PARAMETER; idx++)
           {
               if(fgets(line,ULA_MAX_LINE_SIZE,fp) != NULL)
               {
                   if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
                   {
                      idx--;
                      continue;
                   }
                   trimSpaces(line);
                   length=strlen(line);
                   if (strncmp(line,"sinrProfileSequence",19) == 0)
                   {
                        strncpy(temp,line+20,length-20);
                        stringTokenArray(temp,(unsigned char *)cqiProfileInfo_p
                                [cqiProfileNum].sinrSeq,0,cqiSinrProfileNum);
                   }
               }
           }
                cqiProfileNum++;
        }
    }
    fclose(fp);
}
/*CLPC_CHG*/
/****************************************************************************
 * Function Name  : createUePHRConfig
 * Inputs         : pointer to struct PHRProfile
 *                  profileValues array
 * Outputs        : none
 * Returns        : void
 * Description    : validate and create PHR profiles(from parsed profiles 
 *                  of uePHRConfig.cfg)
 *****************************************************************************/
void createUePHRConfig(PHRProfile phrProfileInfo_p[][MAX_UE_PROFILES],
        UInt8 phrValues[])
{
	UInt8 idx = 0;
	UInt8 retVal = MAC_SUCCESS; 
	UInt8 phrProfileIdx = 0;
	UInt8 tempCount = 0;
	UInt8 profValIdx = 0;
	UInt8 profCount = 0;
	UInt8 profValIndex = 0;
	UInt8 Idx = 0;
	UInt8 cellIndex = 0;
	for (cellIndex = 0; cellIndex < numCells_g ; cellIndex++)
	{
#ifdef PHR_TESTING
		fprintf(stderr,"CellIndex [%d] phrProfileNum=%d\n",cellIndex, phrProfileNum);
#endif
		for(phrProfileIdx=0; phrProfileIdx < phrProfileNum[cellIndex]; phrProfileIdx++)
		{
			/*Validate the value of count*/
			retVal = validatePHRCount(phrProfileInfo_p[cellIndex]\
					[phrProfileIdx].phrCount);
			if(retVal == MAC_SUCCESS)
			{
				phrProfileInfo_g[cellIndex][phrProfileIdx].phrCount =
					phrProfileInfo_p[cellIndex][phrProfileIdx].phrCount;
				tempCount = phrProfileInfo_p[cellIndex][phrProfileIdx].phrCount;
#ifdef PHR_TESTING
				fprintf(stderr,"Valid Count Read=%d\n",
						phrProfileInfo_g[cellIndex][phrProfileIdx].phrCount);
#endif
			}
			else
			{
#ifdef PHR_TESTING            
				fprintf(stderr,"CellIndex [%d] Invalid Count read from \
						uePHRconfig.cfg=%d,using default count(%d) for PHR\n",\
						phrProfileInfo_p[cellIndex][phrProfileIdx].phrCount,1);
#endif
				/*Setting the count to the default value*/
				phrProfileInfo_g[cellIndex][phrProfileIdx].phrCount = 1;
			}

			/*Validate each value of profile Values*/
			for(profValIdx = 0; profValIdx < tempCount; profValIdx++)
			{
				retVal = validatePHRValues(phrProfileInfo_p[cellIndex]
						[phrProfileIdx].phrValues[profValIdx]);
				if(retVal == MAC_SUCCESS)
				{
					phrProfileInfo_g[cellIndex][phrProfileIdx].phrValues[profValIdx]
						= phrProfileInfo_p[cellIndex][phrProfileIdx].
						phrValues[profValIdx];
#ifdef PHR_TESTING
					fprintf(stderr,"CEllIndex [%d] Valid phrValues Read=%d\n",
							phrProfileInfo_g[cellIndex][phrProfileIdx].
							phrValues[profValIdx]);
#endif
				}
				else
				{
#ifdef PHR_TESTING
					fprintf(stderr,"Invalid Profile Values read from uePHRconfig.cfg\
							=%d,using default=1\n",phrProfileInfo_p[cellIndex]
							[phrProfileIdx].phrValues[profValIdx]);
#endif
					/*Setting the profile Values to the default Value*/
					phrProfileInfo_g[cellIndex][phrProfileIdx].
						phrValues[profValIdx] = 0;
				}
			}

			/*Validate the timer value*/
			retVal = validatePHRTimer(phrProfileInfo_p[cellIndex]
					[phrProfileIdx].phrTimer);
			if(retVal == MAC_SUCCESS)
			{
				phrProfileInfo_g[cellIndex][phrProfileIdx].phrTimer =
					phrProfileInfo_p[cellIndex][phrProfileIdx].phrTimer;
#ifdef PHR_TESTING
				fprintf(stderr,"Valid timer Read=%d\n",
						phrProfileInfo_g[cellIndex][phrProfileIdx].phrTimer);
#endif
			}
			else
			{
#ifdef PHR_TESTING
				fprintf(stderr,"CellIndex [%d] Invalid Timer read from \
						uePHRconfig.cfg=%d,using default Timer(%d) for PHR\n",\
						phrProfileInfo_p[phrProfileIdx].phrTimer,100);
#endif
				/*Setting the timer value to the default value*/
				phrProfileInfo_g[cellIndex][phrProfileIdx].phrTimer = 100;

			}
		}
	}
}

/****************************************************************************
 * Function Name  : createUeSINRConfig
 * Inputs         : pointer to struct SRSProfile
 *                  pointer to struct ULSCHProfile
 *                  srsSinrValues array
 *                  ulschSinrValues array
 * Outputs        : 
 * Returns        : void
 * Description    : validate and create SINR profiles(from parsed profiles 
 *                  of ueSINRConfig.cfg)
 *****************************************************************************/
void createUeSINRConfig(SRSProfile *srsProfileInfo_p,
                        ULSCHProfile *ulschProfileInfo_p,
                        CQIProfile *cqiProfileInfo_p,
                        UInt8 srsSinrValues [][MAX_PRB], 
                        UInt8 ulschSinrValues [],
                        UInt8 cqiSinrValues [])
{
	UInt8 ueProfileIdx          = 0;
	UInt8 ueProfileIndex        = 0;
	UInt8 srsSinrProfileIdx     = 0;
	UInt8 ulschSinrProfileIdx   = 0;
	/* CLPC_CHG */
	UInt8 cqiSinrProfileIdx     = 0;
	/* CLPC_CHG */
	UInt8 idx                   = 0;
	UInt8 retVal                = 0;
	UInt8 cellIndex = 0;
	//    UInt8 defaultSinrSeqused[]  = {0,1,2,3,4};

	for ( srsSinrProfileIdx = 0; srsSinrProfileIdx < MAX_SRS_SINR_PROFILES ;
			srsSinrProfileIdx++)
	{
		for( idx = 0; idx < MAX_PRB ; idx++)
		{
			retVal = validateSinrValues(srsSinrValues[srsSinrProfileIdx][idx]);
			if (retVal == MAC_SUCCESS)
			{
				srsSinrValues_g[srsSinrProfileIdx][idx] =
					srsSinrValues[srsSinrProfileIdx][idx];
			}
			else
			{
				fprintf(stderr,"Invalid SINR input from ueSINRconfig.cfg,using" \ 
						"default(max value) %d for SRS SINR \n",MAX_SINR_VAL);  
					srsSinrValues_g[srsSinrProfileIdx][idx] = MAX_SINR_VAL;
			}
		}
	}

	for( ulschSinrProfileIdx = 0; ulschSinrProfileIdx < MAX_ULSCH_SINR_PROFILES;
			ulschSinrProfileIdx++)
	{
		retVal = validateSinrValues(ulschSinrValues[ulschSinrProfileIdx]);
		if (retVal == MAC_SUCCESS)
		{
			ulschSinrValues_g[ulschSinrProfileIdx] =
				ulschSinrValues[ulschSinrProfileIdx];
		}
		else
		{
			fprintf(stderr,"Invalid SINR input from ueSINRconfig.cfg,using \
					default(max value) %d for ULSCH SINR \n",MAX_SINR_VAL);  
				ulschSinrValues_g[ulschSinrProfileIdx] = MAX_SINR_VAL;
		}
	}
	/* CLPC_CHG */
	for ( cqiSinrProfileIdx = 0; cqiSinrProfileIdx < MAX_CQI_SINR_PROFILES ;
			cqiSinrProfileIdx++)
	{
		retVal = validateSinrValues(cqiSinrValues[cqiSinrProfileIdx]);
		if (retVal == MAC_SUCCESS)
		{
			cqiSinrValues_g[cqiSinrProfileIdx] =
				cqiSinrValues[cqiSinrProfileIdx];
		}
		else
		{
			fprintf(stderr,"Invalid SINR input from ueSINRconfig.cfg,using \
					default(max value) %d for CQI SINR \n",MAX_SINR_VAL);        
				cqiSinrValues_g[cqiSinrProfileIdx] = MAX_SINR_VAL;
		}
	}
	/* CLPC_CHG */
	for(ueProfileIdx=0;ueProfileIdx<ueProfileNum[cellIndex];ueProfileIdx++)
	{  
		retVal = validateStartPosition(srsProfileInfo_p[ueProfileInfo_g\
				[cellIndex][ueProfileIdx].
				srsProfileId].startPosition);
		if (retVal == MAC_SUCCESS)
		{
			srsProfileInfo_g[ueProfileInfo_g[cellIndex][ueProfileIdx].\
				srsProfileId].startPosition =
				srsProfileInfo_p[ueProfileInfo_g[cellIndex][ueProfileIdx].\
				srsProfileId].startPosition;
		}
		else
		{
			fprintf(stderr,"Invalid start position input from ueSINRconfig.cfg,using \
					default start position(%d) for ULSCH SINR\n",0);
			srsProfileInfo_g[ueProfileInfo_g[cellIndex][ueProfileIdx].\
				srsProfileId].startPosition = 0;
		}

		retVal = validateSrsBandwidth(srsProfileInfo_p[ueProfileInfo_g\
				[cellIndex][ueProfileIdx].srsProfileId].
				srsBandwidth);
		if (retVal == MAC_SUCCESS)
		{
			srsProfileInfo_g[ueProfileInfo_g[cellIndex][ueProfileIdx].\
				srsProfileId].srsBandwidth =
				srsProfileInfo_p[ueProfileInfo_g[cellIndex][ueProfileIdx].\
				srsProfileId].srsBandwidth;
		}
		else
		{
			fprintf(stderr,"Invalid srs bandwidth input from ueSINRconfig.cfg,using \
					default bandwidth(%d) for SRS\n",MAX_PRB);
			srsProfileInfo_g[ueProfileInfo_g[cellIndex][ueProfileIdx].\
				srsProfileId].srsBandwidth = 
				cellConfigUeSim_g[cellIndex].ulAvailableRBs;
		}

		retVal = validateHoppingEnable(srsProfileInfo_p[ueProfileInfo_g\
				[cellIndex][ueProfileIdx].srsProfileId].hoppingEnable);
		if (retVal == MAC_SUCCESS)
		{
			srsProfileInfo_g[ueProfileInfo_g[cellIndex][ueProfileIdx].\
				srsProfileId].hoppingEnable =
				srsProfileInfo_p[ueProfileInfo_g[cellIndex][ueProfileIdx].\
				srsProfileId].hoppingEnable;
		}
		else
		{
			fprintf(stderr,"Invalid Hopping input from ueSINRconfig.cfg,using \
					default value for hopping(%d - disable) for SRS\n",0);
			srsProfileInfo_g[ueProfileInfo_g[cellIndex][ueProfileIdx].\
				srsProfileId].hoppingEnable = 0;
		}
		for(srsSinrProfileIdx=0;srsSinrProfileIdx<srsSinrProfileNum;
				srsSinrProfileIdx++)
		{
			retVal = validateSinrSeq(srsProfileInfo_p[ueProfileInfo_g\
					[cellIndex][ueProfileIdx].
					srsProfileId].sinrSeq[srsSinrProfileIdx],srsSinrProfileNum);
			if (retVal == MAC_SUCCESS)
			{
				srsProfileInfo_g[ueProfileInfo_g[cellIndex][ueProfileIdx].\
					srsProfileId].sinrSeq[srsSinrProfileIdx] =
					srsProfileInfo_p[ueProfileInfo_g[cellIndex][ueProfileIdx].\
					srsProfileId].sinrSeq[srsSinrProfileIdx];
			}
			else
			{
				fprintf(stderr,"Invalid sinr sequence input from ueSINRconfig.cfg,using \
						default sequence value (%d) for SRS\n",0);
				srsProfileInfo_g[ueProfileInfo_g[cellIndex][ueProfileIdx].\
					srsProfileId].sinrSeq[srsSinrProfileIdx] = 0;
			}
		}

		for(ueProfileIndex=0;ueProfileIndex<ueProfileNum[cellIndex];ueProfileIndex++)
		{
			for(idx=0;idx < ulschSinrProfileNum; idx++)
			{
				retVal = validateSinrSeq(ulschProfileInfo_p[ueProfileInfo_g\
						[cellIndex][ueProfileIndex].ulschProfileId].sinrSeq[idx],ulschSinrProfileNum);
				if (retVal == MAC_SUCCESS)
				{
					ulschProfileInfo_g[ueProfileInfo_g[cellIndex]\
						[ueProfileIndex].ulschProfileId].sinrSeq[idx] =
						ulschProfileInfo_p[ueProfileInfo_g[cellIndex]\
						[ueProfileIndex].ulschProfileId].sinrSeq[idx];
				}
				else
				{
					fprintf(stderr,"Invalid sinr sequence input from ueSINRconfig.cfg,using \
							default sequence value (%d) for SRS\n",0);
					ulschProfileInfo_g[ueProfileInfo_g[cellIndex]\
						[ueProfileIndex].ulschProfileId].sinrSeq[idx] = 0;                    
				}
			}
			/* CLPC_CHG */ 
			for(idx=0;idx < cqiSinrProfileNum; idx++)
			{
				retVal = validateSinrSeq(cqiProfileInfo_p[ueProfileInfo_g\
						[cellIndex][ueProfileIndex].cqiProfileId].sinrSeq[idx],
						cqiSinrProfileNum);
				if (retVal == MAC_SUCCESS)
				{
					cqiProfileInfo_g[ueProfileInfo_g[cellIndex]\
						[ueProfileIndex].cqiProfileId].sinrSeq[idx] =
						cqiProfileInfo_p[ueProfileInfo_g[cellIndex]\
						[ueProfileIndex].cqiProfileId].sinrSeq[idx];
				}
				else
				{
					fprintf(stderr,"Invalid sinr sequence input from ueSINRconfig.cfg,using \
							default sequence value (%d) for SRS\n",0);
					cqiProfileInfo_g[ueProfileInfo_g[cellIndex]\
						[ueProfileIndex].cqiProfileId].sinrSeq[idx] = 0;                    
				}
			}
			/* CLPC_CHG */ 
		}
	}
}

/****************************************************************************
 * Function Name  : validateSinrValues
 * Inputs         : sinrVal value 
 *
 *
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate sinr values 
 *                  Should be in the range of 0 -255
 *****************************************************************************/
MacRetType validateSinrValues (UInt16 sinrVal)
{
    if ( sinrVal < 0 || sinrVal > MAX_SINR_VAL )
    {
        return MAC_FAILURE;
    }
    else
    {
        return MAC_SUCCESS;
    }
}

/****************************************************************************
 * Function Name  : validateStartPosition
 * Inputs         : startPos value 
 *
 *
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate start position value.
 *****************************************************************************/
MacRetType validateStartPosition(UInt8 startPos)
{
    if(startPos < 0 || startPos > MAX_PRB)
    {
        return MAC_FAILURE;
    }
    else
    {
        return MAC_SUCCESS;
    }
}

/****************************************************************************
 * Function Name  : validateSrsBandwidth
 * Inputs         : srsBandwidth value 
 *
 *
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate srs bandwidth
 *****************************************************************************/
MacRetType validateSrsBandwidth(UInt8 srsBandwidth)
{ 
    if(srsBandwidth < 0 || srsBandwidth > MAX_PRB)
    {
        return MAC_FAILURE;
    }
    else
    {
        return MAC_SUCCESS;
    }
}

/****************************************************************************
 * Function Name  : validateHoppingEnable
 * Inputs         : hoppingEnable value 
 *
 *
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate hoppingEnable value 
 *                  It should be either 0(disabled) or 1(enabled)
 *****************************************************************************/
MacRetType validateHoppingEnable(UInt8 hoppingEnable)
{ 
    if(hoppingEnable < 0 || hoppingEnable > 1)
    {
        return MAC_FAILURE;
    }
    else
    {
        return MAC_SUCCESS;
    }
}

/****************************************************************************
 * Function Name  : validateSinrSeq
 * Inputs         : sinrSeqVal value 
 *                  sinrProfileNum - maximum number of SINR profiles parsed 
 *
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate sinr sequence used values
 *****************************************************************************/
MacRetType validateSinrSeq(UInt8 sinrSeqVal,
                           UInt8 sinrProfileNum)
{
    if (sinrSeqVal < 0 || sinrSeqVal > sinrProfileNum)
    {
        return MAC_FAILURE;   
    }
    else
    {
        return MAC_SUCCESS;
    }
}
/* ULA_CHG */

/*CLPC_CHG*/
/****************************************************************************
 * Function Name  : validatePHRCount
 * Inputs         : Count value 
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate count value
 *****************************************************************************/
MacRetType validatePHRCount(UInt8 phrCount)
{
    if (phrCount < 1 || phrCount > MAX_COUNT_VALUE)
    {
        return MAC_FAILURE;   
    }
    else
    {
        return MAC_SUCCESS;
    }
}

/****************************************************************************
 * Function Name  : validatePHRTimer
 * Inputs         : Timer value 
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate timer value
 *****************************************************************************/
MacRetType validatePHRTimer(UInt16 phrTimer)
{
    if ((phrTimer == 10)||
            (phrTimer == 20) ||
                (phrTimer == 50) ||
                    (phrTimer == 100) ||
                        (phrTimer == 200)||
                            (phrTimer == 500)||
                                (phrTimer == 1000))
    {
        return MAC_SUCCESS;   
    }
    else
    {
        return MAC_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : validatePHRValues
 * Inputs         : Profile value
 * Outputs        :
 * Returns        : MAC_SUCCESS - If parsed values are valid
 *                  MAC_FAILURE - If parsed values are not valid
 * Description    : validate profile values
 *****************************************************************************/
MacRetType validatePHRValues(UInt8 phrValue)
{
    if (phrValue < 0 || phrValue > MAX_PROFILE_VALUE)
    {
        return MAC_FAILURE;   
    }
    else
    {
        return MAC_SUCCESS;
    }
}
/* + DRX_CHG */
/****************************************************************************
 *  Function Name  : parseDRXConfigInfo
 *  Inputs         : *msg_p - Pointer to the data received from RRC
 *                 : tagLen - tag length of the DRX_CONFIG_INFO
 *                 : *dlCreateReq_p - Pointer to UE context for downlink 
 *                                    where information is stored after buffer 
 *                                    parsing
 * 
 *   Outputs       : Parsed parameters get stored
 *   Returns       : MacRetType
 *   Description   : This function check for the parameters from the RRC and
 *                    stored in the UeContextForDownlink structure
 *
 *******************************************************************************/

static inline MacRetType parseDRXConfigInfo(UInt8 **message_p,
                                            UInt16 tagLength,
                                       ueContextForDownlink *dlCreateRequest_p)
{

    UInt16 tag    =  0;
    UInt8 *msg_p  =  *message_p;
    UInt16 tagLen =  tagLength;
    UInt8 drxOnDurationTimer = 0;
    UInt8 drxInactivityTimer = 0;
    UInt8 drxRetransmissionTimer = 0;
    UInt8 longDRXCycle = 0;
    UInt8 shortDRXCycle = 0;
    UInt8 drxShortCycleTimer = 0;
    UInt16 drxStartOffset = 0;
    if ( 5 > tagLen || 21 < tagLen )
    {
        lteWarning("Invalid DRX config TagLen. Value = %d\n", tagLen);
        return MAC_FAILURE;
    }

    dlCreateRequest_p->drxConfigType = *msg_p;
    if ( SETUP != dlCreateRequest_p->drxConfigType &&
          RELEASE != dlCreateRequest_p->drxConfigType )
    {
        lteWarning("Invalid DRX config drxConfigType value");
        return MAC_FAILURE;
    }
    msg_p++;

    /* If DRXConfigType is SETUP then only DRX will be configured */
    if ( SETUP == dlCreateRequest_p->drxConfigType )
    {
        tag = LTE_GET_U16BIT(msg_p);
        msg_p += 2;
        if ( DRX_SETUP_CONFIG_INFO == tag )
        {
            tagLen = LTE_GET_U16BIT(msg_p);
            msg_p += 2;
            if ( 10 > tagLen || 16 < tagLen )
            {
                lteWarning("Invalid DRX_SETUP_CONFIG_INFO  TagLen."
                        "Value = %d\n", tagLen);
                return MAC_FAILURE;
            }

            drxOnDurationTimer = *msg_p;
            msg_p ++;

        /*  if ( MAX_ON_DURATION_TIMER_VALUE < drxOnDurationTimer )
         *  {
         *       lteWarning("Invalid value of drxOnDurationTimer. Value = %d\n",
         *                     drxOnDurationTimer);
         *       return MAC_FAILURE;
         *
         *  }
         */
            dlCreateRequest_p->drxContext.drxOnDurationTimer = drxOnDurationTimer;
            
            drxInactivityTimer = *msg_p;
            msg_p++;
 
       /*  if ( MAX_INACTIVITY_TIMER_VALUE < drxInactivityTimer )
	*    {
	*        lteWarning("Invalid value of drxInactivityTimer."
	*                  "Value = %d\n",drxInactivityTimer);
	*        return MAC_FAILURE;
	*    }
        */
            dlCreateRequest_p->drxContext.drxInactivityTimer = 
                                                     drxInactivityTimer;
            drxRetransmissionTimer = *msg_p;
            msg_p++;

        /*   if ( MAX_RETRANSMISSION_TIMER_VALUE < drxRetransmissionTimer )
         *   {
         *       lteWarning("Invalid value of drxRetransmissionTimer." 
         *                   "Value = %d\n",drxRetransmissionTimer );
         *       return MAC_FAILURE;
         *   }
         */
            dlCreateRequest_p->drxContext.drxRetransmissionTimer = 
                                                        drxRetransmissionTimer;
            longDRXCycle = *msg_p;
             msg_p++;

          /*  if ( MAX_LONG_CYCLE_TIMER_VALUE < longDRXCycle )
           *  {
           *      lteWarning("Invalid value of longDRXCycle . Value = %d\n",
           *                    longDRXCycle);
           *      return MAC_FAILURE;
           *  }
           */   
           dlCreateRequest_p->drxContext.longDRXCycle = longDRXCycle;

           drxStartOffset = LTE_GET_U16BIT(msg_p);
           msg_p += 2;
         
          /*   if ( ( (0 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *            dlCreateRequest_p->drxContext.drxStartOffset > 9) ||
           *             ((1 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 19) ||
           *             ((2 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 31) ||
           *             ((3 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 39) ||
           *             ((4 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 63) ||
           *             ((5 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 79) ||
           *             ((6 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 127) ||
           *             ((7 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 159) ||
           *             ((8 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 255 ) ||
           *             ((9 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 319) ||
           *             ((10 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 511 ) ||
           *             ((11 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset >  639) ||
           *             ((12 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 1023 ) ||
           *             ((13 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 1279) ||
           *            ((14 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 2047) ||
           *             ((15 == dlCreateRequest_p->drxContext.longDRXCycle) &&
           *              dlCreateRequest_p->drxContext.drxStartOffset > 2559) )
           *    {
           *        lteWarning("longDRXCycle=[%d] & Invalid value of "
           *                     "drxStartOffset =[%d]\n",longDRXCycle,
           *                      drxStartOffset);
           *        return MAC_FAILURE;
           *    }
           */
             dlCreateRequest_p->drxContext.drxStartOffset = drxStartOffset; 
               tag = LTE_GET_U16BIT(msg_p);
           
               if (tag == SHORT_DRX_CONFIG)
               {
                    msg_p += 2;
                    tagLen = LTE_GET_U16BIT(msg_p);
                    msg_p += 2;

                    if ( 6 != tagLen )
                    {
                        lteWarning("Invalid SHORT_DRX_CONFIG TagLen."
                                     "Value = %d\n", tagLen);
                        return MAC_FAILURE;
                    }

                    dlCreateRequest_p->drxContext.isDRXShortCycleConfigured = 1;
                    shortDRXCycle = *msg_p;
                    msg_p++;

                 /*   if ( MAX_SHORT_CYCLE_TIMER_VALUE < shortDRXCycle )
                  *  {
                  *      lteWarning("Invalid value of shortDRXCycle ."
                  *                " Value = %d\n",shortDRXCycle);
                  *      return MAC_FAILURE;
                  *  }
                  */
                  dlCreateRequest_p->drxContext.shortDRXCycle = shortDRXCycle;
                                     
                  drxShortCycleTimer = *msg_p;
                  msg_p++;
                 /* if ( (1 > drxContext.drxShortCycleTimer) || 
                  *                         (16 < drxShortCycleTimer) )
                  *  {
                  *
                  *      lteWarning("Invalid value of drxShortCycleTimer."
                  *                 " Value = %d\n",drxShortCycleTimer);
                  *      return MAC_FAILURE;
                  *
                  *  }
                  */
                   dlCreateRequest_p->drxContext.drxShortCycleTimer = 
                                                           drxShortCycleTimer;

                }

               /* if ( (drxOnDurationTimer > longDRXCycle)||
                *        (drxOnDurationTimer > shortDRXCycle))
                * {
                *    lteWarning("drxOnduration Timer value less then Short Cycle "
                *                "and Long Cycle timer .Value = %d\n",
                *                shortDRXCycle,longDRXCycle);
                *    return MAC_FAILURE;
                *
                *  }
                */
            }

        }


    *message_p = msg_p;
    return MAC_SUCCESS;

}
/* - DRX_CHG */




 /* + MEAS_GAP_CHG*/
/****************************************************************************
 *  Function Name : parseMeasGapConfigInfo
 *  Inputs        : *msg_p - Pointer to the data received from RRC
 *                   :tagLen - tag length of the MEAS_GAP_CONFIG_INFO
 *                   :*dlCreateReq_p - Pointer to UE context for downlink where
 *                         information is stored after buffer parsing
 *  Outputs       : Parsed parameters get stored
 *  Returns       : MacRetType
 *  Description   : This function check for the parameters from the RRC and
 *                          stored in the UeContextForDownlink structure
 *        
 *****************************************************************************/
static inline MacRetType parseMeasGapConfigInfo(UInt8 **message_p,
                                                    UInt16 tagLength,
                                       ueContextForDownlink *dlCreateRequest_p)
{
        UInt16 tag    =  0;
        UInt8 *msg_p  =  *message_p;
        UInt16 tagLen =  tagLength;
        UInt8 gapPatternId  =  0;
        UInt8 gapOffset = 0;
        
        if ( 5 > tagLen || 11 < tagLen )
        {
            lteWarning("Invalid Meas_Gap config TagLen. Value = %d\n", tagLen);
            return MAC_FAILURE;
        }
      
        dlCreateRequest_p->measGapConfigType = *msg_p;
        msg_p++;

        if ( SETUP != dlCreateRequest_p->measGapConfigType &&
                RELEASE != dlCreateRequest_p->measGapConfigType )
        {
            lteWarning("Invalid Meas_Gap measGapConfigType  value");
            return MAC_FAILURE;
        }

        if  ( SETUP == dlCreateRequest_p->measGapConfigType )
        {
            tag = LTE_GET_U16BIT(msg_p);
           if( MEAS_GAP_SETUP_CONFIG_INFO == tag )
	   {
                msg_p += 2;
                tagLen = LTE_GET_U16BIT(msg_p);
	        msg_p += 2;

                if ( 6 != tagLen )
	        {
	            lteWarning("Invalid MEAS_GAP_SETUP_CONFIG_INFO TagLen."
		               "Value = %d\n", tagLen);
		    return MAC_FAILURE;
	        }

	        gapPatternId = *msg_p;
                msg_p++;
	    
                if ( (GP0 != gapPatternId) && (GP1 != gapPatternId) 
                	/*Meas_Gap_Changes_Start*/
                	&& GP11_R15 < gapPatternId)
                	/*Meas_Gap_Changes_End*/
	        {
	            lteWarning("Invalid value of gapPatternId",gapPatternId);
		    return MAC_FAILURE;
                }
                dlCreateRequest_p->measGapConfig.gapPatternId  =  gapPatternId;

	       gapOffset = *msg_p;
	       msg_p++;

	       if((GP0 == gapPatternId  && gapOffset >
				   (MEASUREMENT_GAP_PERIODICITY_40 - 1)) ||
	          (GP1 == gapPatternId  && gapOffset >
	           		    (MEASUREMENT_GAP_PERIODICITY_80 - 1)))
	       {
	            lteWarning("Invalid value of  meas gapOffset =[%d]\n",
					   gapOffset);
		    return MAC_FAILURE;
	       }
	       /*Meas_Gap_Changes_Start*/
		   else if((GP4_R15 <= gapPatternId && GP11_R15 >= gapPatternId) 
				&& (gapOffset < (MEASUREMENT_GAP_PERIODICITY_20-1) ||
				gapOffset > (MEASUREMENT_GAP_PERIODICITY_160 -1)))
		   {
 			   lteWarning("Invalid value of  meas gapOffset =[%d]\n",
					   gapOffset);

		   }
	       /*Meas_Gap_Changes_End*/

	       dlCreateRequest_p->measGapConfig.gapOffset = gapOffset;
	   }
           else
           {
              return MAC_FAILURE;
           }
              
        }
        *message_p = msg_p;
        return MAC_SUCCESS;

}
 /* - MEAS_GAP_CHG */

/*CLPC_CHG*/                                                                                                                          
/****************************************************************************
 * Function Name  : parseTpcPdcchConfig
 * Inputs         : message_p      : Pointer to the buffer containing the
 *                                   values of the TPC_PDCCH_CONFIG API
 *                  paramLength    : It is the tag length of the
 *                                   TPC_PDCCH_CONFIG API obtained through the
 *                                   buffer
 *                  configCellReq_p: Pointer to the ConfigCellReq structure
 * Output         : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This method will parse the passed buffer that is received
 *                  at the time of creation of UE Context and parse the
 *                  TpcPdcchConfig. It then populates UeContextForUplink's
 *                  tpcPdcchConfigPucch/ tpcPdcchConfigPusch.
 ******************************************************************************/

MacRetType parseTpcPdcchConfig(UInt8 *message_p,
        UInt16 paramLength,
        UInt16 current,
        TpcPdcchConfig *tpcPdcchConf,
        UInt8 isTpcForPusch) /*0-Pucch 1-Pusch*/
{
    MacRetType retVal = MAC_SUCCESS;
    UInt16 tagLen = paramLength;
    UInt16 tagLength = 0;
    UInt16 tag = 0;
    UInt8 *msg_p = message_p;
    UInt8 remLen = paramLength;

    remLen = remLen - TAG_LEN;
    /*Subtracting the length of parameter release*/
    remLen = remLen - 1;
    //If Release is False, then parse TPC_PDCCH_INFO TLV

    tag = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    tagLength = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    if(tag != TPC_RNTI_INFO)
    {
        lteWarning("Invalid tag TPC_PDCCH_INFO\n");
        retVal = MAC_FAILURE;
    }

    remLen = remLen - TAG_LEN;
    if(remLen < TPC_RNTI_INFO_PARAM_LENGTH)
    {
        lteWarning("Mandatory Parameters are missing in TPC_PDCCH_INFO\n");
        retVal = MAC_FAILURE;
    }

    tpcPdcchConf->tpcRnti = LTE_GET_U16BIT(msg_p + current);
    current += 2;

    if(tpcPdcchConf->tpcRnti < START_TPC_RNTI ||
        tpcPdcchConf->tpcRnti > END_TPC_RNTI)
    {
        lteWarning("Tpc Out of Range\n");
        return MAC_FAILURE;
    }
    tpcPdcchConf->tpcBitMapIndex = *(msg_p + current);
    current++;
    fprintf(stderr,"BITMAP=%d\n",tpcPdcchConf->tpcBitMapIndex);
    fprintf(stderr,"BITMAP MACRO=%d\n",MIN_TPC_INDEX_VALUE_DCI3);
    fprintf(stderr,"BITMAP MACRO=%d\n",MAX_TPC_INDEX_VALUE_DCI3);

    tpcPdcchConf->isConfiguredForDci3Or3A = *(msg_p + current);
    current++;

    if (tpcPdcchConf->isConfiguredForDci3Or3A == DCI_FORMAT_3)
    {
        if ((tpcPdcchConf->tpcBitMapIndex < MIN_TPC_INDEX_VALUE_DCI3) ||
                (tpcPdcchConf->tpcBitMapIndex > MAX_TPC_INDEX_VALUE_DCI3))
        {
            lteWarning("Invalid tpcBitMapIndex. Value = %d\n",
                    tpcPdcchConf->tpcBitMapIndex);
            return MAC_FAILURE;
        }
    }
    else if (tpcPdcchConf->isConfiguredForDci3Or3A == DCI_FORMAT_3A)
    {
        if ((tpcPdcchConf->tpcBitMapIndex < MIN_TPC_INDEX_VALUE_DCI3A) ||
                (tpcPdcchConf->tpcBitMapIndex > MAX_TPC_INDEX_VALUE_DCI3A))
        {
            lteWarning("Invalid tpcBitMapIndex. Value = %d\n",
                    tpcPdcchConf->tpcBitMapIndex);
            return MAC_FAILURE;
        }
    }
    else
    {
        lteWarning("Invalid DCI Format. Neither DCI3 nor DCI3A received \n");
        return MAC_FAILURE;
    }


    return retVal;
}


/****************************************************************************
 * Function Name  : parseDedicatedULPowerControlInfo
 * Inputs         : message_p      : Pointer to the buffer containing the
 *                                   values of the UPLINK_POWER_CONTROL_DEDICATED API
 *                  paramLength    : It is the tag length of the
 *                                   UPLINK_POWER_CONTROL_DEDICATED API
 *                                   obtained through the buffer
 *                  configCellReq_p: Pointer to the ConfigCellReq structure
 * Output         : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This method will parse the passed buffer that is received
 *                  at the time of creation of UE Context and parse the
 *                  DedicatedULPowerControlInfo. It then populates
 *                  UeContextForUplink's DedicatedULPowerControlInfo.
 ******************************************************************************/
MacRetType parseDedicatedULPowerControlInfo(
        UInt8 *message_p,
        UInt16 paramLength,
        UInt16 current,
        DedicatedULPowerControlInfo* ulPowCtrlInfo)
{
    UInt16 tagLen = paramLength;
    UInt8 *msg_p = message_p;

    ulPowCtrlInfo->p0UePusch = *(msg_p + current);
    current++;

    /*Checking the range of the p0UePusch.*/
    if((MIN_P0_UE_PUSCH_VALUE > ulPowCtrlInfo->p0UePusch) ||
            (MAX_P0_UE_PUSCH_VALUE < ulPowCtrlInfo->p0UePusch))
    {
        lteWarning("Invalid p0UePusch. Value = %d\n",
                ulPowCtrlInfo->p0UePusch);
        return MAC_FAILURE;
    }

    ulPowCtrlInfo->deltaMcsEnabled =*(msg_p + current);
    current++;

    /*Checking the range of the deltaMcsEnabled.*/
    if((FALSE != ulPowCtrlInfo->deltaMcsEnabled) &&
            (TRUE != ulPowCtrlInfo->deltaMcsEnabled))
    {
        lteWarning("Invalid deltaMcsEnabled. Value = %d\n",
                ulPowCtrlInfo->deltaMcsEnabled);
        return MAC_FAILURE;
    }

    ulPowCtrlInfo->accumulationEnabled = *(msg_p + current);
    current++;

    /*Checking the range of the accumulationEnabled.*/
    if((FALSE != ulPowCtrlInfo->accumulationEnabled) &&
            (TRUE != ulPowCtrlInfo->accumulationEnabled))
    {
        lteWarning("Invalid accumulationEnabled. Value = %d\n",
                ulPowCtrlInfo->accumulationEnabled);
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;

}
/*CLPC_CHG*/

/* + CQI_4.1 */
/****************************************************************************
 * Function Name  : initUESelectedBinomalValues
 * Inputs         : N - Number of Subbands
 *                  M - preferred Subbands 
 * Outputs        : initialize binomial values.  
 * Returns        : None
 * Description    : This function initialize nCm_g with binomal coefficient
 *                  (nCr) from N and M.
 ****************************************************************************/
void initUESelectedBinomalValues(UInt16 ueIndex)
{
    UInt8 numSubbands = 0 , preferredSubbands = 0;
    DLUEContext *newDLUEContext_p = PNULL;
    newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    UInt8 N = newDLUEContext_p->dlCQIInfo.ueSelectedN;
    UInt8 M =  newDLUEContext_p->dlCQIInfo.preferredSubbands;    

    /* nCm is the binomial coefficient of N and M 
     *if N >= M then nCm_g will contain nCm ( n = N , m = M )
     *if N< M then nCm_g will contain 0
     */

    for (preferredSubbands = 1; preferredSubbands <= M; preferredSubbands++) 
    {
        nCm_g[0][preferredSubbands] = 0;
    }

    for (numSubbands  = 0; numSubbands <= N; numSubbands++) 
    {
        nCm_g[numSubbands][0] = 1;
    }
    
    for (numSubbands  = 1; numSubbands <= N; numSubbands++)
    {
        for(preferredSubbands = 1; preferredSubbands <= M; preferredSubbands++)
        {
             nCm_g[numSubbands][preferredSubbands] = 
              nCm_g[numSubbands - 1][preferredSubbands - 1] + 
                           nCm_g[numSubbands - 1][preferredSubbands];
        }
    }
    return;

}

/****************************************************************************
 * Function Name  : getBinomialCofficent
 * Inputs         : N - Number of Subbands
 *                  M - preferred Subbands 
 * Outputs        : binomial coefficient  
 * Returns        : binomial coefficient
 * Description    : This function access nCm_g to get binomal coefficient
 *                  (nCr) of N and M.
 ****************************************************************************/
UInt32  getBinomialCofficent(UInt8 N , UInt8 M)
{
        return  nCm_g[N][M];
}

/*****************************************************************************
 * Function Name  : getposOfMSelSBForAperiodicMode20
 * Inputs         : count
 *                  ueIndex
 * Outputs        : 
 * Returns        : posOfMSelSubbandValue
 ******************************************************************************/
/* Aperiodic CQI Changes Start */
UInt32 getposOfMSelSBForAperiodicMode20(UInt8 count,UInt16 ueIndex, UInt8 cellIndex)
{
    UInt8 subbandNum = 1;
    UInt8  counter = 0;
    UInt32 posOfMSelSubbandValue = 0;
    DLUEContext *newDLUEContext_p = PNULL;
    newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    UInt8 N = newDLUEContext_p->dlCQIInfo.ueSelectedN;
    UInt8 M =  newDLUEContext_p->dlCQIInfo.preferredSubbands;    
    UInt8 *posOfMSelSBArray_p = &(posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_1);
    for (counter = 0; counter < M; counter++)
    {
        subbandNum = *posOfMSelSBArray_p;
        posOfMSelSubbandValue += getBinomialCofficent((N - subbandNum ),(M - counter));
        posOfMSelSBArray_p++;
    }
    return posOfMSelSubbandValue;
}
/* Aperiodic CQI Changes End */

/*****************************************************************************
 * Function Name  : getposOfMSelSBForAperiodicMode20RIOne
 * Inputs         : count
 *                  ueIndex
 * Outputs        : 
 * Returns        : posOfMSelSubbandValue
 ******************************************************************************/
/* Aperiodic CQI Changes Start */
UInt32 getposOfMSelSBForAperiodicMode22RIOne(UInt8 count,UInt16 ueIndex, UInt8 cellIndex)
{
    UInt8 subbandNum = 1;
    UInt8  counter = 0;
    UInt32 posOfMSelSubbandValue = 0;
    DLUEContext *newDLUEContext_p = PNULL;
    newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    UInt8 N = newDLUEContext_p->dlCQIInfo.ueSelectedN;
    UInt8 M =  newDLUEContext_p->dlCQIInfo.preferredSubbands;    
    UInt8 *posOfMSelSBArray_p = &(posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_1);
    for (counter = 0; counter < M; counter++)
    {
        subbandNum = *posOfMSelSBArray_p;
        posOfMSelSubbandValue += getBinomialCofficent((N - subbandNum ),(M - counter));
        posOfMSelSBArray_p++;
    }
    return posOfMSelSubbandValue;
}
/* Aperiodic CQI Changes End */

/*****************************************************************************
 * Function Name  : getposOfMSelSBForAperiodicMode20RITwo
 * Inputs         : count
 *                  ueIndex
 * Outputs        : 
 * Returns        : posOfMSelSubbandValue
 ******************************************************************************/
/* Aperiodic CQI Changes Start */
UInt32 getposOfMSelSBForAperiodicMode22RITwo(UInt8 count,UInt16 ueIndex, UInt8 cellIndex)
{
    UInt8 subbandNum = 1;
    UInt8  counter = 0;
    UInt32 posOfMSelSubbandValue = 0;
    DLUEContext *newDLUEContext_p = PNULL;
    newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    UInt8 N = newDLUEContext_p->dlCQIInfo.ueSelectedN;
    UInt8 M =  newDLUEContext_p->dlCQIInfo.preferredSubbands;    
    UInt8 *posOfMSelSBArray_p = &(posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_1);
    for (counter = 0; counter < M; counter++)
    {
        subbandNum = *posOfMSelSBArray_p;
        posOfMSelSubbandValue += getBinomialCofficent((N - subbandNum ),(M - counter));
        posOfMSelSBArray_p++;
    }
    return posOfMSelSubbandValue;
}
/* Aperiodic CQI Changes End */

/* EICIC +*/
/*****************************************************************************
 * Function Name  : getposOfMSelSBForAperiodicMode20
 * Inputs         : count
 *                  ueIndex
 * Outputs        : 
 * Returns        : posOfMSelSubbandValue
 ******************************************************************************/
/* Aperiodic CQI Changes Start */
UInt32 getposOfMSelSBForAperiodicMode20Abs(UInt8 count,UInt16 ueIndex, UInt8 cellIndex)
{
    UInt8 subbandNum = 1;
    UInt8  counter = 0;
    UInt32 posOfMSelSubbandValue = 0;
    DLUEContext *newDLUEContext_p = PNULL;
    newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    UInt8 N = newDLUEContext_p->dlCQIInfo.ueSelectedN;
    UInt8 M =  newDLUEContext_p->dlCQIInfo.preferredSubbands;    
    UInt8 *posOfMSelSBArray_p = &(posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_1);
    for (counter = 0; counter < M; counter++)
    {
        subbandNum = *posOfMSelSBArray_p;
        posOfMSelSubbandValue += getBinomialCofficent((N - subbandNum ),(M - counter));
        posOfMSelSBArray_p++;
    }
    return posOfMSelSubbandValue;
}
/* Aperiodic CQI Changes End */

/*****************************************************************************
 * Function Name  : getposOfMSelSBForAperiodicMode20RIOne
 * Inputs         : count
 *                  ueIndex
 * Outputs        : 
 * Returns        : posOfMSelSubbandValue
 ******************************************************************************/
/* Aperiodic CQI Changes Start */
UInt32 getposOfMSelSBForAperiodicMode22RIOneAbs(UInt8 count,UInt16 ueIndex, UInt8 cellIndex)
{
    UInt8 subbandNum = 1;
    UInt8  counter = 0;
    UInt32 posOfMSelSubbandValue = 0;
    DLUEContext *newDLUEContext_p = PNULL;
    newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    UInt8 N = newDLUEContext_p->dlCQIInfo.ueSelectedN;
    UInt8 M =  newDLUEContext_p->dlCQIInfo.preferredSubbands;    
    UInt8 *posOfMSelSBArray_p = &(posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_1);
    for (counter = 0; counter < M; counter++)
    {
        subbandNum = *posOfMSelSBArray_p;
        posOfMSelSubbandValue += getBinomialCofficent((N - subbandNum ),(M - counter));
        posOfMSelSBArray_p++;
    }
    return posOfMSelSubbandValue;
}
/* Aperiodic CQI Changes End */

/*****************************************************************************
 * Function Name  : getposOfMSelSBForAperiodicMode20RITwo
 * Inputs         : count
 *                  ueIndex
 * Outputs        : 
 * Returns        : posOfMSelSubbandValue
 ******************************************************************************/
/* Aperiodic CQI Changes Start */
UInt32 getposOfMSelSBForAperiodicMode22RITwoAbs(UInt8 count,UInt16 ueIndex, UInt8 cellIndex)
{
    UInt8 subbandNum = 1;
    UInt8  counter = 0;
    UInt32 posOfMSelSubbandValue = 0;
    DLUEContext *newDLUEContext_p = PNULL;
    newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    UInt8 N = newDLUEContext_p->dlCQIInfo.ueSelectedN;
    UInt8 M =  newDLUEContext_p->dlCQIInfo.preferredSubbands;    
    UInt8 *posOfMSelSBArray_p = &(posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_1);
    for (counter = 0; counter < M; counter++)
    {
        subbandNum = *posOfMSelSBArray_p;
        posOfMSelSubbandValue += getBinomialCofficent((N - subbandNum ),(M - counter));
        posOfMSelSBArray_p++;
    }
    return posOfMSelSubbandValue;
}
/* EICIC -*/

/****************************************************************************
 * Function Name  : parseUeCQIProfile
 * Inputs         : 
 * Outputs        :
 * Returns        : void
 * Description    : Parse ueCQIProfile.cfg to configure various CQI profiles
 *****************************************************************************/
void parseUeCQIProfile()
                       
{
    FILE    *fp;
    UInt16  length                    = 0;/* ULA_UTP */
    Char8   line[CQI_MAX_LINE_SIZE]   = {0};
    Char8   temp[CQI_MAX_LINE_SIZE]   = {0};
    UInt8   count                     = 0;
    Char8   *str1;
    /*EICIC Enhancement +*/
    UInt32  indexTracker = 0;
    /*EICIC Enhancement -*/
    fp = fopen(UE_CQI_PROFILE_FILE_PATH,"r");
    if (fp == NULL)
    {
        fprintf(stderr,"Unable to open file - UE_CQI_FILE_PATH\n");
    }
    while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
    {
        if ((strncmp((const char*)line,"#",1) == 0) || (strncmp((const char*)line,"\n",1) == 0))
        {
            continue;
        }

        trimSpaces(line);
        length = strlen(line);

        if (strncmp((const char*)line,"[UE_PROFILE_1]",14) == 0)
        {   
            while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
            {
                trimSpaces(line);
                length = strlen(line);
                if (strncmp((const char*)line,"cqiProfileSequence",18) == 0)
                {
                    strncpy((char *)temp,(const char*)line+19,length-19);
                    /* extract first string from string sequence */
                    str1 = strtok((char *)temp, ",");
                    cqiUEProfileValues_g[0].ueProfile[count] =  atoi(str1);
                    count++;
                    /* loop until finished */
                    while (1)
                    {
                        /* extract string from string sequence */
                        str1 = strtok(NULL, ",");
                        /* check if there is nothing else to extract */
                        if (str1 == NULL)
                        {
                            break;
                        }
                        cqiUEProfileValues_g[0].ueProfile[count] =  atoi(str1);
                        cqiUEProfileValues_g[0].indexCount++;
                        count++;
                    }
                    count = 0;
                }
	/*EICIC Enhancement +*/	 
                else if (strncmp((const char*)line,"IndexTracker",12) == 0)
		{
                    strncpy((char *)temp,(const char*)line+13,length-13);
                    /* extract the string from string sequence */
                    str1 = strtok((char *)temp, " ");
                    cqiUEProfileValues_g[0].indexTracker =  atoi(str1);
		}
	/*EICIC Enhancement -*/
			
                else if(strncmp((const char*)line,"[/UE_PROFILE_1]",15) == 0)
                {
                    break;
                }    
            }
        }
        else if (strncmp((const char*)line,"[UE_PROFILE_2]",14) == 0)
        {   
            while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
            {
                trimSpaces(line);
                length = strlen(line);
                if (strncmp((const char*)line,"cqiProfileSequence",18) == 0)
                {
                    strncpy((char *)temp,(const char*)line+19,length-19);
                    /* extract first string from string sequence */
                    str1 = strtok((char *)temp, ",");
                    cqiUEProfileValues_g[1].ueProfile[count] =  atoi(str1);
                    count++;
                    /* loop until finished */
                    while (1)
                    {
                        /* extract string from string sequence */
                        str1 = strtok(NULL, ",");
                        /* check if there is nothing else to extract */
                        if (str1 == NULL)
                        {
                            break;
                        }
                        cqiUEProfileValues_g[1].ueProfile[count] =  atoi(str1);
                        cqiUEProfileValues_g[1].indexCount++;
                        count++;
                    }
                    count = 0;
                }
	/*EICIC Enhancement +*/	 
                else if (strncmp((const char*)line,"IndexTracker",12) == 0)
		{
                    strncpy((char *)temp,(const char*)line+13,length-13);
                    /* extract the string from string sequence */
                    str1 = strtok((char *)temp, " ");
                    cqiUEProfileValues_g[1].indexTracker =  atoi(str1);
		}
	/*EICIC Enhancement -*/
                else if(strncmp((const char*)line,"[/UE_PROFILE_2]",15) == 0)
                {
                    break;
                }    
            }
        }
        else if (strncmp((const char*)line,"[UE_PROFILE_3]",14) == 0)
        {   
            while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
            {
                trimSpaces(line);
                length = strlen(line);
                if (strncmp((const char*)line,"cqiProfileSequence",18) == 0)
                {
                    strncpy((char *)temp,(const char*)line+19,length-19);
                    /* extract first string from string sequence */
                    str1 = strtok((char *)temp, ",");
                    cqiUEProfileValues_g[2].ueProfile[count] =  atoi(str1);
                    count++;
                    /* loop until finished */
                    while (1)
                    {
                        /* extract string from string sequence */
                        str1 = strtok(NULL, ",");
                        /* check if there is nothing else to extract */
                        if (str1 == NULL)
                        {
                            break;
                        }
                        cqiUEProfileValues_g[2].ueProfile[count] =  atoi(str1);
                        cqiUEProfileValues_g[2].indexCount++;
                        count++;
                    }
                    count = 0;
                }
	/*EICIC Enhancement +*/	 
                else if (strncmp((const char*)line,"IndexTracker",12) == 0)
		{
                    strncpy((char *)temp,(const char*)line+13,length-13);
                    /* extract the string from string sequence */
                    str1 = strtok((char *)temp, " ");
                    cqiUEProfileValues_g[2].indexTracker =  atoi(str1);
		}
	/*EICIC Enhancement -*/
                else if(strncmp((const char*)line,"[/UE_PROFILE_3]",15) == 0)
                {
                    break;
                }    
            }
        }
        else if (strncmp((const char*)line,"[UE_PROFILE_4]",14) == 0)
        {   
            while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
            {
                trimSpaces(line);
                length = strlen(line);
                if (strncmp((const char*)line,"cqiProfileSequence",18) == 0)
                {
                    strncpy((char *)temp,(const char*)line+19,length-19);
                    /* extract first string from string sequence */
                    str1 = strtok((char *)temp, ",");
                    cqiUEProfileValues_g[3].ueProfile[count] =  atoi(str1);
                    count++;
                    /* loop until finished */
                    while (1)
                    {
                        /* extract string from string sequence */
                        str1 = strtok(NULL, ",");
                        /* check if there is nothing else to extract */
                        if (str1 == NULL)
                        {
                            break;
                        }
                        cqiUEProfileValues_g[3].ueProfile[count] =  atoi(str1);
                        cqiUEProfileValues_g[3].indexCount++;
                        count++;
                    }
                    count = 0;
                }
	/*EICIC Enhancement +*/	 
                else if (strncmp((const char*)line,"IndexTracker",12) == 0)
		{
                    strncpy((char *)temp,(const char*)line+13,length-13);
                    /* extract the string from string sequence */
                    str1 = strtok((char *)temp, " ");
                    cqiUEProfileValues_g[3].indexTracker =  atoi(str1);
		}
	/*EICIC Enhancement -*/
                else if(strncmp((const char*)line,"[/UE_PROFILE_4]",15) == 0)
                {
                    break;
                }    
            }
        }
        else if (strncmp((const char*)line,"[UE_PROFILE_5]",14) == 0)
        {   
            while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
            {
                trimSpaces(line);
                length = strlen(line);
                if (strncmp((const char*)line,"cqiProfileSequence",18) == 0)
                {
                    strncpy((char *)temp,(const char*)line+19,length-19);
                    /* extract first string from string sequence */
                    str1 = strtok((char *)temp, ",");
                    cqiUEProfileValues_g[4].ueProfile[count] =  atoi(str1);
                    count++;
                    /* loop until finished */
                    while (1)
                    {
                        /* extract string from string sequence */
                        str1 = strtok(NULL, ",");
                        /* check if there is nothing else to extract */
                        if (str1 == NULL)
                        {
                            break;
                        }
                        cqiUEProfileValues_g[4].ueProfile[count] =  atoi(str1);
                        cqiUEProfileValues_g[4].indexCount++;
                        count++;
                    }
                    count = 0;
                }
	/*EICIC Enhancement +*/	 
                else if (strncmp((const char*)line,"IndexTracker",12) == 0)
		{
                    strncpy((char *)temp,(const char*)line+13,length-13);
                    /* extract the string from string sequence */
                    str1 = strtok((char *)temp, " ");
                    cqiUEProfileValues_g[4].indexTracker =  atoi(str1);
		}
	/*EICIC Enhancement -*/
                else if(strncmp((const char*)line,"[/UE_PROFILE_5]",15) == 0)
                {
                    break;
                }    
            }
        }
        else if (strncmp((const char*)line,"[UE_PROFILE_6]",14) == 0)
        {   
            while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
            {
                trimSpaces(line);
                length = strlen(line);
                if (strncmp((const char*)line,"cqiProfileSequence",18) == 0)
                {
                    strncpy((char *)temp,(const char*)line+19,length-19);
                    /* extract first string from string sequence */
                    str1 = strtok((char *)temp, ",");
                    cqiUEProfileValues_g[5].ueProfile[count] =  atoi(str1);
                    count++;
                    /* loop until finished */
                    while (1)
                    {
                        /* extract string from string sequence */
                        str1 = strtok(NULL, ",");
                        /* check if there is nothing else to extract */
                        if (str1 == NULL)
                        {
                            break;
                        }
                        cqiUEProfileValues_g[5].ueProfile[count] =  atoi(str1);
                        cqiUEProfileValues_g[5].indexCount++;
                        count++;
                    }
                    count = 0;
                }
	/*EICIC Enhancement +*/	 
                else if (strncmp((const char*)line,"IndexTracker",12) == 0)
		{
                    strncpy((char *)temp,(const char*)line+13,length-13);
                    /* extract the string from string sequence */
                    str1 = strtok((char *)temp, ";");
                    cqiUEProfileValues_g[5].indexTracker =  atoi(str1);
		}
	/*EICIC Enhancement -*/
                else if(strncmp((const char*)line,"[/UE_PROFILE_6]",15) == 0)
                {
                    break;
                }    
            }
        }
        else if (strncmp((const char*)line,"[UE_PROFILE_7]",14) == 0)
        {   
            while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
            {
                trimSpaces(line);
                length = strlen(line);
                if (strncmp((const char*)line,"cqiProfileSequence",18) == 0)
                {
                    strncpy((char *)temp,(const char*)line+19,length-19);
                    /* extract first string from string sequence */
                    str1 = strtok((char *)temp, ",");
                    cqiUEProfileValues_g[6].ueProfile[count] =  atoi(str1);
                    count++;
                    /* loop until finished */
                    while (1)
                    {
                        /* extract string from string sequence */
                        str1 = strtok(NULL, ",");
                        /* check if there is nothing else to extract */
                        if (str1 == NULL)
                        {
                            break;
                        }
                        cqiUEProfileValues_g[6].ueProfile[count] =  atoi(str1);
                        cqiUEProfileValues_g[6].indexCount++;
                        count++;
                    }
                    count = 0;
                }
	/*EICIC Enhancement +*/	 
                else if (strncmp((const char*)line,"IndexTracker",12) == 0)
		{
                    strncpy((char *)temp,(const char*)line+13,length-13);
                    /* extract the string from string sequence */
                    str1 = strtok((char *)temp, " ");
                    cqiUEProfileValues_g[6].indexTracker =  atoi(str1);
		}
	/*EICIC Enhancement -*/
                else if(strncmp((const char*)line,"[/UE_PROFILE_7]",15) == 0)
                {
                    break;
                }    
            }
        }
        else if (strncmp((const char*)line,"[UE_PROFILE_8]",14) == 0)
        {   
            while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
            {
                trimSpaces(line);
                length = strlen(line);
                if (strncmp((const char*)line,"cqiProfileSequence",18) == 0)
                {
                    strncpy((char *)temp,(const char*)line+19,length-19);
                    /* extract first string from string sequence */
                    str1 = strtok((char *)temp, ",");
                    cqiUEProfileValues_g[7].ueProfile[count] =  atoi(str1);
                    count++;
                    /* loop until finished */
                    while (1)
                    {
                        /* extract string from string sequence */
                        str1 = strtok(NULL, ",");
                        /* check if there is nothing else to extract */
                        if (str1 == NULL)
                        {
                            break;
                        }
                        cqiUEProfileValues_g[7].ueProfile[count] =  atoi(str1);
                        cqiUEProfileValues_g[7].indexCount++;
                        count++;
                    }
                    count = 0;
               }
	/*EICIC Enhancement +*/ 
                else if (strncmp((const char*)line,"IndexTracker",12) == 0)
		{
                    strncpy((char *)temp,(const char*)line+13,length-13);
                    /* extract the string from string sequence */
                    str1 = strtok((char *)temp, " ");
                    cqiUEProfileValues_g[7].indexTracker =  atoi(str1);
		}
	/*EICIC Enhancement -*/
                else if(strncmp((const char*)line,"[/UE_PROFILE_8]",15) == 0)
                {
                    break;
                }    
            }
        }
    }
}

/****************************************************************************
 * Function Name  : parseUeCQIConfig
 * Inputs         : 
 * Outputs        :
 * Returns        : void
 * Description    : Parse ueCQIConfig.cfg to configure various CQI profiles
 *****************************************************************************/
/* Aperiodic CQI Changes Start */
void parseUeCQIConfig()
{
	FILE    *fp;
	UInt16  length                    = 0;/* ULA_UTP */
	Char8   line[CQI_MAX_LINE_SIZE]   = {0};
	Char8   temp[CQI_MAX_LINE_SIZE]   = {0};
	UInt8   count                     = 0;
	Char8   *str1;
	SInt8 cellIndex = -1;
	fp = fopen(UE_CQI_FILE_PATH,"r");
	if (fp == NULL)
	{
		fprintf(stderr,"Unable to open file - UE_CQI_FILE_PATH\n");
	}
	while((fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL) && cellIndex < numCells_g)
	{
		if ((strncmp((const char*)line,"#",1) == 0) || (strncmp((const char*)line,"\n",1) == 0))
		{
			continue;
		}
		trimSpaces(line);
		length = strlen(line);

        if (strncmp((const char*)line,"[CELL]",6) == 0)
        {
            strncpy((char *)temp,(const char*)line+6,length-6);
                    cellIndex++;
        }
        if (cellIndex >= numCells_g)
                break;
		/* 4x2 MIMO E */
		if (strncmp((const char*)line,"[num_tx_Antenna]",16) == 0)
		{
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"numberOfTxAnteena",17) == 0)
				{
					strncpy((char *)temp,(const char*)line+18,length-18);
					str1 = strtok((char *)temp, ",");
                    cellConfigUeSim_g[cellIndex].numOfTxAnteenas = atoi(str1);
                    fprintf(stderr,"CellIndex [%d] value for Tx Anteena\
                            [%d] \n",cellIndex, cellConfigUeSim_g[cellIndex].numOfTxAnteenas);
                } 
                else if(strncmp((const char*)line,"[/num_tx_Antenna]",17) == 0)
                {
                    break;
                }
            }
        }
		/* 4x2 MIMO E */
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_1_0]",23) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCqi",11) == 0)
				{
					strncpy((char *)temp,(const char*)line+12,length-12);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode10_g[cellIndex][count].wideBandCQI = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode10_g[cellIndex][count].wideBandCQI = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_1_0]",24) == 0)
				{
					break;
				}    
			}
		}
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_1_1_RI_1]",28) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCqi",11) == 0)
				{
					strncpy((char *)temp,(const char*)line+12,length-12);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g[cellIndex][count].wideBandCQI_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g[cellIndex][count].wideBandCQI_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g[cellIndex][count].PMI_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g[cellIndex][count].PMI_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_1_1_RI_1]",29) == 0)
				{
					break;
				}    
			}
		}
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_1_1_RI_2]",28) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCqi",11) == 0)
				{
					strncpy((char *)temp,(const char*)line+12,length-12);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g[cellIndex][count].wideBandCQI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g[cellIndex][count].wideBandCQI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"spatialDifferentialCQI",22) == 0)
				{
					strncpy((char *)temp,(const char*)line+23,length-23);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g[cellIndex][count].spatialDiffCQI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g[cellIndex][count].spatialDiffCQI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g[cellIndex][count].PMI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g[cellIndex][count].PMI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_1_1_RI_2]",29) == 0)
				{
					break;
				}    
			}
		}
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_2_0]",23) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if(strncmp((const char*)line,"subBandCqi",10) == 0)
				{
					strncpy((char *)temp,(const char*)line+11,length-11);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode20_g[cellIndex][count].subBandCQI = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode20_g[cellIndex][count].subBandCQI = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"subBandLabel",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode20_g[cellIndex][count].subBandLabel = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode20_g[cellIndex][count].subBandLabel = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_2_0]",24) == 0)
				{
					break;
				}    
			}
		}
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_2_1_RI_1]",28) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"subBandCqi",10) == 0)
				{
					strncpy((char *)temp,(const char*)line+11,length-11);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g[cellIndex][count].subBandCQI_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g[cellIndex][count].subBandCQI_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				/* SPR 8067 Fix Start */ 
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g[cellIndex][count].PMI_RI_1 = atoi(str1);
					fprintf(stderr,"cqiPeriodicReportMode21[%d].PMI_RI_1 = %d\n",
							count,cqiPeriodicReportMode21_g[cellIndex][count].PMI_RI_1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g[cellIndex][count].PMI_RI_1 = atoi(str1);
						fprintf(stderr,"cqiPeriodicReportMode21[%d].PMI_RI_1 = %d\n",
								count,cqiPeriodicReportMode21_g[cellIndex][count].PMI_RI_1);
						count++;
					}
					count = 0;
				}
				/* SPR 8067 Fix End */ 
				else if (strncmp((const char*)line,"subBandLabel",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g[cellIndex][count].subBandLabel_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g[cellIndex][count].subBandLabel_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_2_1_RI_1]",29) == 0)
				{
					break;
				}   
			}
		} 
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_2_1_RI_2]",28) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"subBandCqi",10) == 0)
				{
					strncpy((char *)temp,(const char*)line+11,length-11);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g[cellIndex][count].subBandCQI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g[cellIndex][count].subBandCQI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"spatialDifferentialCQI",22) == 0)
				{
					strncpy((char *)temp,(const char*)line+23,length-23);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g[cellIndex][count].spatialDiffCQI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g[cellIndex][count].spatialDiffCQI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				/* SPR 8067 Fix Start */ 
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g[cellIndex][count].PMI_RI_2 = atoi(str1);
					fprintf(stderr,"cqiPeriodicReportMode21[%d].PMI_RI_2 = %d\n",
							count,cqiPeriodicReportMode21_g[cellIndex][count].PMI_RI_2);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g[cellIndex][count].PMI_RI_2 = atoi(str1);
						fprintf(stderr,"cqiPeriodicReportMode21[%d].PMI_RI_2 = %d\n",
								count,cqiPeriodicReportMode21_g[cellIndex][count].PMI_RI_2);
						count++;
					}
					count = 0;
				}
				/* SPR 8067 Fix End */ 
				else if (strncmp((const char*)line,"subBandLabel",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g[cellIndex][count].subBandLabel_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g[cellIndex][count].subBandLabel_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_2_1_RI_2]",29) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_1_2_RI_1]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor1SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor2SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor3SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor4SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor5SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor6SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor7SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor8SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor9SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor10SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor11SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor12SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor13SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_1[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_1_2_RI_1]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_1_2_RI_2]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"wideBandCQICodeword1",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor1SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor2SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor3SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor4SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor5SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor6SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor7SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor8SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor9SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor10SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor11SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor12SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor13SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g[cellIndex][count].pmi_RI_2[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_1_2_RI_2]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_2_0]",24) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword",19) == 0)
				{
					strncpy((char *)temp,(const char*)line+20,length-20);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode20_g[cellIndex][count].wideBandCQICW = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode20_g[cellIndex][count].wideBandCQICW = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQI",22) == 0)
				{
					strncpy((char *)temp,(const char*)line+23,length-23);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode20_g[cellIndex][count].subBandDiffCQI = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode20_g[cellIndex][count].subBandDiffCQI = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_1",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_2",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_3",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_3 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_3 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_4",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_4 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_4 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_5",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_5 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_5 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_6",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_6 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g[cellIndex][count].subbandNum_6 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_2_0]",25) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_2_2_RI_1]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g[cellIndex][count].subBandDiffCQICW0_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g[cellIndex][count].subBandDiffCQICW0_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_1",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_2",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_3",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_3 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_3 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_4",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_4 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_4 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_5",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_5 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_5 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_6",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_6 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g[cellIndex][count].subbandNum_6 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g[cellIndex][count].pmi_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g[cellIndex][count].pmi_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_2_2_RI_1]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_2_2_RI_2]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g[cellIndex][count].subBandDiffCQICW0_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g[cellIndex][count].subBandDiffCQICW0_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"wideBandCQICodeword1",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g[cellIndex][count].subBandDiffCQICW1_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g[cellIndex][count].subBandDiffCQICW1_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_1",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_2",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_3",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_3 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_3 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_4",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_4 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_4 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_5",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_5 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_5 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_6",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_6 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g[cellIndex][count].subbandNum_6 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g[cellIndex][count].pmi_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g[cellIndex][count].pmi_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_2_2_RI_2]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_3_0]",24) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword",19) == 0)
				{
					strncpy((char *)temp,(const char*)line+20,length-20);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].wideBandCQICW = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].wideBandCQICW = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor1SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor2SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor3SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor4SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor5SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor6SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor7SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor8SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor9SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor10SB",29) == 0)
				{
					strncpy((char *)temp,(const char*)line+30,length-30);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor11SB",29) == 0)
				{
					strncpy((char *)temp,(const char*)line+30,length-30);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor12SB",29) == 0)
				{
					strncpy((char *)temp,(const char*)line+30,length-30);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor13SB",29) == 0)
				{
					strncpy((char *)temp,(const char*)line+30,length-30);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g[cellIndex][count].subBandDiffCQI[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_3_0]",25) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_3_1_RI_1]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For1SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For2SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For3SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For4SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For5SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For6SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For7SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For8SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For9SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For10SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For11SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For12SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For13SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_1[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].pmi_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].pmi_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_3_1_RI_1]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_3_1_RI_2]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For1SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For2SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For3SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For4SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For5SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For6SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For7SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For8SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For9SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For10SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For11SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For12SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For13SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW0_RI_2[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"wideBandCQICodeword1",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For1SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For2SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For3SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For4SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For5SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For6SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For7SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For8SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For9SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For10SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For11SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For12SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For13SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].subBandDiffCQICW1_RI_2[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g[cellIndex][count].pmi_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g[cellIndex][count].pmi_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_3_1_RI_2]",30) == 0)
				{
					break;
				}   
			}
		}
		/* + CQI_5.0 */
		else if (strncmp((const char*)line,"[RI_VALUES]",11) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"RI",2) == 0)
				{
					strncpy((char *)temp,(const char*)line+3,length-3);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
                    /* + SPR 13126 Changes */
					RIValues_g[cellIndex][count] = atoi(str1);
                    /* - SPR 13126 Changes */
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
                        /* + SPR 13126 Changes */
						RIValues_g[cellIndex][count] = atoi(str1); 
                        /* - SPR 13126 Changes */
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/RI_VALUES]",12) == 0)
				{
					break;
				}  
			}
		}
		/* - CQI_5.0 */
	}
}
/* Aperiodic CQI Changes End */
/* EICIC +*/
/****************************************************************************
 * Function Name  : parseUeCQIConfig_ABS
 * Inputs         : 
 * Outputs        :
 * Returns        : void
 * Description    : Parse ueCQIConfig.cfg to configure various CQI profiles
 *****************************************************************************/
void parseUeCQIConfig_ABS()
{
	FILE    *fp;
	UInt16  length                    = 0;/* ULA_UTP */
	Char8   line[CQI_MAX_LINE_SIZE]   = {0};
	Char8   temp[CQI_MAX_LINE_SIZE]   = {0};
	UInt8   count                     = 0;
	Char8   *str1;
    /*SPR 19931 +*/
	SInt8 cellIndex = 0;
    /*SPR 19931 -*/
	fp = fopen(UE_CQI_FILE_PATH_ABS,"r");
	if (fp == NULL)
	{
		fprintf(stderr,"Unable to open file - UE_CQI_FILE_PATH_ABS\n");
	}
	while((fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL) && cellIndex < numCells_g)
	{
		if ((strncmp((const char*)line,"#",1) == 0) || (strncmp((const char*)line,"\n",1) == 0))
		{
			continue;
		}
		trimSpaces(line);
		length = strlen(line);

        if (strncmp((const char*)line,"[CELL]",6) == 0)
        {
            strncpy((char *)temp,(const char*)line+6,length-6);
                    cellIndex++;
        }
        if (cellIndex >= numCells_g)
                break;
		/* 4x2 MIMO E */
		if (strncmp((const char*)line,"[num_tx_Antenna]",16) == 0)
		{
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"numberOfTxAnteena",17) == 0)
				{
					strncpy((char *)temp,(const char*)line+18,length-18);
					str1 = strtok((char *)temp, ",");
                    cellConfigUeSim_g[cellIndex].numOfTxAnteenas = atoi(str1);
                    fprintf(stderr,"CellIndex [%d] value for Tx Anteena\
                            [%d] \n",cellIndex, cellConfigUeSim_g[cellIndex].numOfTxAnteenas);
                } 
                else if(strncmp((const char*)line,"[/num_tx_Antenna]",17) == 0)
                {
                    break;
                }
            }
        }
		/* 4x2 MIMO E */
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_1_0]",23) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCqi",11) == 0)
				{
					strncpy((char *)temp,(const char*)line+12,length-12);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode10_g_2[cellIndex][count].wideBandCQI = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode10_g_2[cellIndex][count].wideBandCQI = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_1_0]",24) == 0)
				{
					break;
				}    
			}
		}
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_1_1_RI_1]",28) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCqi",11) == 0)
				{
					strncpy((char *)temp,(const char*)line+12,length-12);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g_2[cellIndex][count].wideBandCQI_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g_2[cellIndex][count].wideBandCQI_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g_2[cellIndex][count].PMI_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g_2[cellIndex][count].PMI_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_1_1_RI_1]",29) == 0)
				{
					break;
				}    
			}
		}
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_1_1_RI_2]",28) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCqi",11) == 0)
				{
					strncpy((char *)temp,(const char*)line+12,length-12);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g_2[cellIndex][count].wideBandCQI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g_2[cellIndex][count].wideBandCQI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"spatialDifferentialCQI",22) == 0)
				{
					strncpy((char *)temp,(const char*)line+23,length-23);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g_2[cellIndex][count].spatialDiffCQI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g_2[cellIndex][count].spatialDiffCQI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode11_g_2[cellIndex][count].PMI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode11_g_2[cellIndex][count].PMI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_1_1_RI_2]",29) == 0)
				{
					break;
				}    
			}
		}
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_2_0]",23) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if(strncmp((const char*)line,"subBandCqi",10) == 0)
				{
					strncpy((char *)temp,(const char*)line+11,length-11);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode20_g_2[cellIndex][count].subBandCQI = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode20_g_2[cellIndex][count].subBandCQI = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"subBandLabel",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode20_g_2[cellIndex][count].subBandLabel = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode20_g_2[cellIndex][count].subBandLabel = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_2_0]",24) == 0)
				{
					break;
				}    
			}
		}
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_2_1_RI_1]",28) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"subBandCqi",10) == 0)
				{
					strncpy((char *)temp,(const char*)line+11,length-11);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g_2[cellIndex][count].subBandCQI_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g_2[cellIndex][count].subBandCQI_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				/* SPR 8067 Fix Start */ 
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g_2[cellIndex][count].PMI_RI_1 = atoi(str1);
					fprintf(stderr,"cqiPeriodicReportMode21[%d].PMI_RI_1 = %d\n",
							count,cqiPeriodicReportMode21_g_2[cellIndex][count].PMI_RI_1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g_2[cellIndex][count].PMI_RI_1 = atoi(str1);
						fprintf(stderr,"cqiPeriodicReportMode21[%d].PMI_RI_1 = %d\n",
								count,cqiPeriodicReportMode21_g_2[cellIndex][count].PMI_RI_1);
						count++;
					}
					count = 0;
				}
				/* SPR 8067 Fix End */ 
				else if (strncmp((const char*)line,"subBandLabel",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g_2[cellIndex][count].subBandLabel_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g_2[cellIndex][count].subBandLabel_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_2_1_RI_1]",29) == 0)
				{
					break;
				}   
			}
		} 
		else if (strncmp((const char*)line,"[cqi_Periodic_Mode_2_1_RI_2]",28) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"subBandCqi",10) == 0)
				{
					strncpy((char *)temp,(const char*)line+11,length-11);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g_2[cellIndex][count].subBandCQI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g_2[cellIndex][count].subBandCQI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"spatialDifferentialCQI",22) == 0)
				{
					strncpy((char *)temp,(const char*)line+23,length-23);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g_2[cellIndex][count].spatialDiffCQI_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g_2[cellIndex][count].spatialDiffCQI_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				/* SPR 8067 Fix Start */ 
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g_2[cellIndex][count].PMI_RI_2 = atoi(str1);
					fprintf(stderr,"cqiPeriodicReportMode21[%d].PMI_RI_2 = %d\n",
							count,cqiPeriodicReportMode21_g_2[cellIndex][count].PMI_RI_2);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g_2[cellIndex][count].PMI_RI_2 = atoi(str1);
						fprintf(stderr,"cqiPeriodicReportMode21[%d].PMI_RI_2 = %d\n",
								count,cqiPeriodicReportMode21_g_2[cellIndex][count].PMI_RI_2);
						count++;
					}
					count = 0;
				}
				/* SPR 8067 Fix End */ 
				else if (strncmp((const char*)line,"subBandLabel",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiPeriodicReportMode21_g_2[cellIndex][count].subBandLabel_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiPeriodicReportMode21_g_2[cellIndex][count].subBandLabel_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Periodic_Mode_2_1_RI_2]",29) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_1_2_RI_1]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor1SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor2SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor3SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor4SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor5SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor6SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor7SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor8SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor9SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor10SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor11SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor12SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor13SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_1[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_1_2_RI_1]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_1_2_RI_2]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"wideBandCQICodeword1",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor1SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor2SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor3SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor4SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor5SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor6SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor7SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor8SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor9SB",30) == 0)
				{
					strncpy((char *)temp,(const char*)line+31,length-31);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor10SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor11SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor12SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicatorFor13SB",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode12_g_2[cellIndex][count].pmi_RI_2[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_1_2_RI_2]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_2_0]",24) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword",19) == 0)
				{
					strncpy((char *)temp,(const char*)line+20,length-20);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode20_g_2[cellIndex][count].wideBandCQICW = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode20_g_2[cellIndex][count].wideBandCQICW = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQI",22) == 0)
				{
					strncpy((char *)temp,(const char*)line+23,length-23);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode20_g_2[cellIndex][count].subBandDiffCQI = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode20_g_2[cellIndex][count].subBandDiffCQI = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_1",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_2",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_3",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_3 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_3 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_4",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_4 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_4 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_5",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_5 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_5 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_6",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_6 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode20_g_2[cellIndex][count].subbandNum_6 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_2_0]",25) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_2_2_RI_1]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g_2[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g_2[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g_2[cellIndex][count].subBandDiffCQICW0_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g_2[cellIndex][count].subBandDiffCQICW0_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_1",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_2",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_3",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_3 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_3 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_4",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_4 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_4 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_5",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_5 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_5 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_6",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_6 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RIOne_g_2[cellIndex][count].subbandNum_6 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g_2[cellIndex][count].pmi_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g_2[cellIndex][count].pmi_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_2_2_RI_1]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_2_2_RI_2]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g_2[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g_2[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g_2[cellIndex][count].subBandDiffCQICW0_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g_2[cellIndex][count].subBandDiffCQICW0_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"wideBandCQICodeword1",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g_2[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g_2[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1",31) == 0)
				{
					strncpy((char *)temp,(const char*)line+32,length-32);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g_2[cellIndex][count].subBandDiffCQICW1_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g_2[cellIndex][count].subBandDiffCQICW1_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_1",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_2",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_3",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_3 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_3 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_4",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_4 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_4 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_5",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_5 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_5 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subbandNum_6",12) == 0)
				{
					strncpy((char *)temp,(const char*)line+13,length-13);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_6 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						posOfMSelSBForAperiodicMode22RITwo_g_2[cellIndex][count].subbandNum_6 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode22_g_2[cellIndex][count].pmi_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode22_g_2[cellIndex][count].pmi_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_2_2_RI_2]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_3_0]",24) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword",19) == 0)
				{
					strncpy((char *)temp,(const char*)line+20,length-20);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].wideBandCQICW = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].wideBandCQICW = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor1SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor2SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor3SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor4SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor5SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor6SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor7SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor8SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor9SB",28) == 0)
				{
					strncpy((char *)temp,(const char*)line+29,length-29);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor10SB",29) == 0)
				{
					strncpy((char *)temp,(const char*)line+30,length-30);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor11SB",29) == 0)
				{
					strncpy((char *)temp,(const char*)line+30,length-30);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor12SB",29) == 0)
				{
					strncpy((char *)temp,(const char*)line+30,length-30);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQIFor13SB",29) == 0)
				{
					strncpy((char *)temp,(const char*)line+30,length-30);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode30_g_2[cellIndex][count].subBandDiffCQI[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_3_0]",25) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_3_1_RI_1]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].wideBandCQICW0_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For1SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For2SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For3SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For4SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For5SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For6SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For7SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For8SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For9SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For10SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For11SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For12SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For13SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_1[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].pmi_RI_1 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].pmi_RI_1 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_3_1_RI_1]",30) == 0)
				{
					break;
				}   
			}
		}
		else if (strncmp((const char*)line,"[cqi_Aperiodic_Mode_3_1_RI_2]",29) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"wideBandCQICodeword0",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].wideBandCQICW0_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For1SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For2SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For3SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For4SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For5SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For6SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For7SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For8SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For9SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For10SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For11SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For12SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword0For13SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW0_RI_2[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"wideBandCQICodeword1",20) == 0)
				{
					strncpy((char *)temp,(const char*)line+21,length-21);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].wideBandCQICW1_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For1SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[0] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[0] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For2SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[1] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[1] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For3SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[2] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[2] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For4SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[3] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[3] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For5SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[4] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[4] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For6SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[5] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[5] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For7SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[6] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[6] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For8SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[7] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[7] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For9SB",37) == 0)
				{
					strncpy((char *)temp,(const char*)line+38,length-38);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[8] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[8] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For10SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[9] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[9] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For11SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[10] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[10] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For12SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[11] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[11] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"subBandDifferentialCQICodeword1For13SB",38) == 0)
				{
					strncpy((char *)temp,(const char*)line+39,length-39);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[12] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].subBandDiffCQICW1_RI_2[12] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if (strncmp((const char*)line,"precodingMatrixIndicator",24) == 0)
				{
					strncpy((char *)temp,(const char*)line+25,length-25);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					cqiAperiodicReportMode31_g_2[cellIndex][count].pmi_RI_2 = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						cqiAperiodicReportMode31_g_2[cellIndex][count].pmi_RI_2 = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/cqi_Aperiodic_Mode_3_1_RI_2]",30) == 0)
				{
					break;
				}   
			}
		}
		/* + CQI_5.0 */
		else if (strncmp((const char*)line,"[RI_VALUES]",11) == 0)
		{   
			while(fgets((char *)line,CQI_MAX_LINE_SIZE,fp) != NULL)
			{
				trimSpaces(line);
				length = strlen(line);
				if (strncmp((const char*)line,"RI",2) == 0)
				{
					strncpy((char *)temp,(const char*)line+3,length-3);
					/* extract first string from string sequence */
					str1 = strtok((char *)temp, ",");
					RIValues_g_2[cellIndex][count] = atoi(str1);
					count++;
					/* loop until finished */
					while (1)
					{
						/* extract string from string sequence */
						str1 = strtok(NULL, ",");
						/* check if there is nothing else to extract */
						if (str1 == NULL)
						{
							break;
						}
						RIValues_g_2[cellIndex][count] = atoi(str1);
						count++;
					}
					count = 0;
				}
				else if(strncmp((const char*)line,"[/RI_VALUES]",12) == 0)
				{
					break;
				}  
			}
		}
		/* - CQI_5.0 */
	}
}
/* EICIC -*/

/*****************************************************************************
 * Function Name  : getCqiRiPeriodicityAndOffset
 * Inputs         : cqiMode - Its represent the Periodic Mode -10,11,20,21
 *                  cqiPmiConfigIndex - Its represent the CQI configuration
 *                                      index lies between 0 -1023.
 *                  riConfigIndex - Its indicates the RI config Index and lies
 *                                   between 0 - 1023.
 *                  cqiPeriodicity , nOffsetCQI , riPeriodicity, nOffsetRI
 *                  These are the pointer and fill by this function
 *
 * Outputs        : Fills the cqiPeriodicity,,nOffsetCQI,riPeriodicity,nOffsetRI
 * Returns        : None
 * Description    : This function fills the value of cqiPeriodicity,nOffsetCQI
 *                  and riPeriodicity, nOffsetRI
 *****************************************************************************/
UInt32 getCqiRiPeriodicityAndOffset( UInt16 cqiPmiConfigIndex,
                                     UInt16 riConfigIndex,
                                     UInt8  H,
                                     UInt16 *cqiPeriodicity_p,
                                     SInt16 *nOffsetCQI_p,
                                     UInt16 *riPeriodicity_p,
                                     SInt16 *nOffsetRI_p )
{
    UInt8  idx  = 0;
    SInt16 cqiOffset = 0;
    SInt16 riOffset  = 0;
    UInt16 cqiPeriodicity = 0;
    UInt16 riPeriodicity  = 0;


    for ( idx = 0 ; idx < TOTAL_NUM_ROW_IN_CQI_CONFIG; idx++)
    {
        /* check for cqiPeriodicity, cqiOffset */
        if ((cqiPmiConfigIndex >= cqiPmiConfigTable_g[idx].cqiPmiConfigStartIndex &&
               cqiPmiConfigIndex <=
                        cqiPmiConfigTable_g[idx].cqiPmiConfigEndIndex) )
        {
            cqiPeriodicity = cqiPmiConfigTable_g[idx].cqiPeriodicity;
            cqiOffset = cqiPmiConfigIndex;
            cqiOffset = cqiPmiConfigTable_g[idx].nOffsetCQI + cqiOffset;
            break;
        }
    }
    *cqiPeriodicity_p = cqiPeriodicity;
    *nOffsetCQI_p     = cqiOffset;


    if( MAC_RI_CONFIG_INDEX_INVALID != riConfigIndex )
    {    
        for ( idx = 0 ; idx < TOTAL_NUM_ROW_IN_RI_CONFIG; idx++)
        {
            /* check for riPeriodicity, riOffset */
            if  ((riConfigIndex  >= riConfigTable_g[idx].riConfigStartIndex &&
                        riConfigIndex <= riConfigTable_g[idx].riConfigEndIndex))
            {
                riPeriodicity = riConfigTable_g[idx].riPeriodicity;
                riOffset = riConfigIndex;
                riOffset = riConfigTable_g[idx].nOffsetRI + riOffset;
                break;
            }
        }
        *nOffsetRI_p = riOffset;
        *riPeriodicity_p = riPeriodicity;
    }


    return 1;
}

/* Phase 2 Periodic CQi Changes Start*/


/*****************************************************************************
 * Function Name  : getPeriodicReportingMode 
 * Inputs         : transMode  - trasmission mode as received from RRC
 *                  periodicFeedBackType -  wideband/ subband 
 * Outputs        : None
 * Returns        : transmission mode
 * Description    : This function returns the periodic transmission mode
 *                  Refer table 7.2.2-1 of 36.213  
 *****************************************************************************/
cqiPeriodicModeType getPeriodicReportingMode
    (UInt8 transMode, PeriodicFeedbackType periodicFeedBackType,
    /* + TM7_8 Changes Start */
    UInt8 pmiRiReportV920)
    /* - TM7_8 Changes End */
{
#ifdef UT_TESTING
    MAC_EMULATOR_LOG(LOG_INFO,"Calculating periodic mode for "\
            " Transmission mode = %d, report type = %d\n", transMode, 
            periodicFeedBackType);
#endif
/* SPR 545 Fix Starts */    
    if(TX_MODE_1 == transMode || TX_MODE_2 == transMode || 
          TX_MODE_3 == transMode || TX_MODE_7 == transMode || TX_MODE_ATTACH_NOT_COMPLETE == transMode ||
          /* + TM7_8 Changes Start */
          ((TX_MODE_8 == transMode) && (TRUE != pmiRiReportV920)))
         /* - TM7_8 Changes End */
/* SPR 545 Fix Ends */
    {
        if(PERIODIC_FEEDBACK_WIDEBAND == periodicFeedBackType)
        {
#ifdef UT_TESTING
            LTE_MAC_UT_LOG(LOG_DETAIL,UE_CONF_MGR,"Periodic Mode calulated as "
                    "MODE_1_0");
#endif
            return CQI_PERIODIC_TYPE_10; 
        }
        else
        {
#ifdef UT_TESTING
            MAC_EMULATOR_LOG(LOG_INFO,"Periodic Mode calulated as "\
                    "MODE_2_0");
#endif
            return CQI_PERIODIC_TYPE_20;
        }
    }
    else if (TX_MODE_4 == transMode || TX_MODE_5 == 
            transMode || TX_MODE_6 == transMode || 
            /* + TM7_8 Changes Start */
            ((TX_MODE_8 == transMode)&&(TRUE == pmiRiReportV920)))
            /* - TM7_8 Changes End */
    {
        if(PERIODIC_FEEDBACK_WIDEBAND == periodicFeedBackType)
        {
#ifdef UT_TESTING
            MAC_EMULATOR_LOG(LOG_INFO,"Periodic Mode calulated as "\
                    "MODE_1_1");
#endif
            return CQI_PERIODIC_TYPE_11;
        }
        else
        {
#ifdef UT_TESTING
            MAC_EMULATOR_LOG(LOG_INFO,"Periodic Mode calulated as "\
                    "MODE_2_1");
#endif
            return CQI_PERIODIC_TYPE_21;
        }
    }
    else
    {
#ifdef UT_TESTING
        MAC_EMULATOR_LOG(LOG_INFO,"Invalid transmission mode %u",
                transMode);
#endif
    }
    /*+COVERITY 5.3.0 - 32259*/
    return PERIODIC_MODE_INVALID;
    /*-COVERITY 5.3.0 - 32259*/
}


/*****************************************************************************
 * Function Name  : createScellCqiPmiRiMap 
 * Inputs         : ueContextForDownlink
 *                  ueContextForUplink
 * Outputs        : None
 * Returns        : 
 * Description    : This function call function to get periodic report
 *                  type for scell
 *****************************************************************************/
void createScellCqiPmiRiMap(ueContextForUplink *ulUEInfo,
                            ueContextForDownlink *dlUEInfo,
                            UInt8 scellIndex)
{
    UInt32 scellPeriodicModeType = 0;
    ScellInfo *scellInfo  = PNULL;
    CqiInfo *scellCqiInfo = PNULL;

    scellInfo = &(ulUEInfo->carrierAggrConfig.scellInfo[scellIndex]);
    scellCqiInfo = &(ulUEInfo->carrierAggrConfig.scellInfo[scellIndex].cqiInfo);

 /*   scellPeriodicModeType = getPeriodicReportingMode(scellInfo->transmissionMode,
            scellCqiInfo->cqiformatIndicatorPeriodic,
            scellCqiInfo->pmiRiReportV920);
 */
    UInt16 ueIndex = 0;
    ueIndex = dlUEInfo->ueIndex;
    scellPeriodicModeType = uplinkContextInfoMap_g[ueIndex]->carrierAggrConfig.scellInfo[0].dlCQIInfo.periodicModeType;

//dlUEInfo->cqiInfoFlag = 1;
    //if ( (0 != dlUEInfo->cqiInfoFlag) &&
    if ( (0 != ulUEInfo->cqiinfoflag) &&
            (0 != scellPeriodicModeType) )
    {   

        /* Calling the function to create the CQI PMI and RI preodicity map */
        createCqiPmiRiPeriodicityMap(
                cqiPmiRiScellReportMap_g[scellIndex],
                scellCqiInfo->cqiPMIConfigIndex,
                scellCqiInfo->riConfigIndex,
                scellCqiInfo->cqiformatIndicatorPeriodic_subband_k,
                scellPeriodicModeType,
                dlUEInfo->ueIndex,
                1,FALSE);

        	 if (SETUP == ulUEInfo->cqiConfigRequestType) /* EICIC +*/
		{
	                        /* Calling the function to create the CQI PMI and RI preodicity map for ABS */
	           createCqiPmiRiPeriodicityMap(
	                            /* SPR 10980 Fix Start */
                cqiPmiRiScellReportMap2_g[scellIndex],
                scellCqiInfo->cqiPMIConfigIndex_2,
                scellCqiInfo->riConfigIndex_2,
                scellCqiInfo->cqiformatIndicatorPeriodic_subband_k,
                scellPeriodicModeType,
                dlUEInfo->ueIndex,
	                 0,TRUE); /* EICIC -*/
		}
    }
}
/* Phase 2 Periodic CQi Changes End*/

/*****************************************************************************
 * Function Name  : createCqiPmiRiPeriodicityMap
 * Inputs         : cqiPmiRiReportMap
 *                : cqiPmiConfigIndex
 *                : riConfigIndex
 *                : K
 * Outputs        : None
 * Returns        : cqiPmiRiReportMap
 * Description    : This function create the Mapping of the CQI, PMI and RI
 *                : subFrame collision Mapping
 *****************************************************************************/
void createCqiPmiRiPeriodicityMap(cqiPmiRiReportMap *cqiPmiRiReportMap_p,
                                  UInt16             cqiPmiConfigIndex,
                                  UInt16             riConfigIndex,
                                  UInt8              K,
                                  UInt8              wide_suband,
                                  UInt16             ueIndex,
                                  UInt8              isScell,
                                  UInt8			isAbs)
{
    UInt16            sfnCount        = 0;
    UInt8             sfCount         = 0;
    UInt8             H               = 0;
    UInt16            cqiPeriodicity  = 0;
    SInt16            nOffsetCQI      = 0;
    UInt16            riPeriodicity   = 0;
    SInt16            nOffsetRI       = 0;
    DLUEContext *newDLUEContext_p = PNULL;
    newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
   /* SPR 12465 Fix Start */
    DLCQIInfo *dlCQIInfo_p = PNULL;
    
    /*SPR 12146 Fix start*/
    if(PNULL == newDLUEContext_p)
    {
        fprintf(stderr,"\n [%s] UE context is NULL for ueIndex=[%d]\n",__func__, ueIndex);
    }
   /* SA_FIX  start */
    if(isScell)
    {

    dlCQIInfo_p = & (uplinkContextInfoMap_g[ueIndex]->carrierAggrConfig.scellInfo[0].dlCQIInfo);
    }
    else
    {
        dlCQIInfo_p =  &newDLUEContext_p->dlCQIInfo;
    }
    /*SPR 12146 Fix End*/
    /* calculate the value of H */
    H = dlCQIInfo_p->bandwidthParts * K + 1;
   

    /* Get the value of periodicity and offset of RI and CQI/PMI */
    if(getCqiRiPeriodicityAndOffset(cqiPmiConfigIndex,
                                    riConfigIndex,
                                    H,
                                    &cqiPeriodicity,
                                    &nOffsetCQI,
                                    &riPeriodicity,
                                    &nOffsetRI))
    {
        /* + CQI_4.1 */
	/* EICIC +*/
        if(isAbs)
        {
        	 dlCQIInfo_p->cqiPeriodicity_2 = cqiPeriodicity;
        	 dlCQIInfo_p->cqiOffset_2 = nOffsetCQI;
       	 dlCQIInfo_p->widebandPeriodicity_2  = (H * dlCQIInfo_p->cqiPeriodicity_2);
        }
        else
        {
        	 dlCQIInfo_p->cqiPeriodicity = cqiPeriodicity;
        	 dlCQIInfo_p->cqiOffset = nOffsetCQI;
       	 dlCQIInfo_p->widebandPeriodicity  = (H * dlCQIInfo_p->cqiPeriodicity);
        }
	/* EICIC -*/
        
        /* - CQI_4.1 */
        if ( CQI_PERIODIC_TYPE_10 == wide_suband )
        {
            /* calculation for the periodic mode 1-0 */
            for(sfnCount = 0; sfnCount < MAX_SFN_VALUE; sfnCount++)
            {
                for(sfCount = 0; sfCount < MAX_SF_VALUE; sfCount++)
                {
                    /* WideBand CQI/PMI Report instances */
                    if(((10*sfnCount + sfCount - nOffsetCQI)%
                                                          cqiPeriodicity) == 0)
                    {
                        cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] = 
                                                          WIDE_BAND_CQI_TYPE_4;
                    }
                    if( 0 != riPeriodicity )
                    {    
                        /* RI Report instances */
                        if (((10*sfnCount + sfCount - nOffsetCQI + nOffsetRI)%
                                    (cqiPeriodicity*riPeriodicity))== 0)
                        {
                            cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] = 
                                RI_TYPE_3;
                        }
                    }
                }
            }   
        }
        else if ( CQI_PERIODIC_TYPE_20 == wide_suband )
        {
            /* calculation for the periodic mode 2-0 */
            for(sfnCount = 0; sfnCount < MAX_SFN_VALUE; sfnCount++)
            {
                for(sfCount = 0; sfCount < MAX_SF_VALUE; sfCount++)
                {
                    /* Sub Band CQI Report instances */
                    if(((10*sfnCount + sfCount - nOffsetCQI)%
                                                          cqiPeriodicity) == 0)
                    {
                        cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] =
                                                               SUB_BAND_TYPE_1;
                    }
                    /* Wide Band CQI/PMI Report instances */
                    if (((10*sfnCount + sfCount - nOffsetCQI)%
                                                      (H*cqiPeriodicity)) == 0)
                    {
                        cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] =
                                                          WIDE_BAND_CQI_TYPE_4;
                    }
                    if( 0 != riPeriodicity )
                    {    
                        /* RI Report instances */
                        if (((10*sfnCount + sfCount - nOffsetCQI + nOffsetRI)%
                                    (H*cqiPeriodicity*riPeriodicity))== 0)
                        {
                            cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] =
                                RI_TYPE_3;
                        }
                    }
                }
            }
        }
        else if ( CQI_PERIODIC_TYPE_11 == wide_suband )
        {
            /* calculation for the periodic mode 1-1 */
            for(sfnCount = 0; sfnCount < MAX_SFN_VALUE; sfnCount++)
            {
                for(sfCount = 0; sfCount < MAX_SF_VALUE; sfCount++)
                {
                    /* WideBand CQI/PMI Report instances */
                    if(((10*sfnCount + sfCount - nOffsetCQI)%
                                                          cqiPeriodicity) == 0)
                    {
                        cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] =
                                                      WIDE_BAND_CQI_PMI_TYPE_2;
                    }
                    if( 0 != riPeriodicity )
                    {    
                        /* RI Report instances */
                        if (((10*sfnCount + sfCount - nOffsetCQI + nOffsetRI)%
                                    (cqiPeriodicity * riPeriodicity))== 0)
                        {
                            cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] =
                                RI_TYPE_3;
                        }
                    }
                }
            }

        }
        else if ( CQI_PERIODIC_TYPE_21 == wide_suband )
        {
            /* calculation for the periodic mode 2-1 */
            for(sfnCount = 0; sfnCount < MAX_SFN_VALUE; sfnCount++)
            {
                for(sfCount = 0; sfCount < MAX_SF_VALUE; sfCount++)
                {
                    /* Sub Band CQI Report instances */
                    if(((10*sfnCount + sfCount - nOffsetCQI)%
                                                          cqiPeriodicity) == 0)
                    {
                        cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] =
                                                               SUB_BAND_TYPE_1;
                    }
                    /* Wide Band CQI/PMI Report instances */
                    if (((10*sfnCount + sfCount - nOffsetCQI)%
                                                    (H * cqiPeriodicity)) == 0)
                    {
                        cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] =
                                                      WIDE_BAND_CQI_PMI_TYPE_2;
                    }
                    if( 0 != riPeriodicity )
                    {    
                        /* RI Report instances */
                        if (((10*sfnCount + sfCount - nOffsetCQI + nOffsetRI)%
                                    (H*cqiPeriodicity*riPeriodicity))== 0)
                        {
                            cqiPmiRiReportMap_p[sfnCount].reportType[sfCount][ueIndex] =
                                RI_TYPE_3;
                        }
                    }
                }
            }
        }
    }
}

/****************************************************************************
 * Function Name  : getLAndNValues
 * Inputs         : ueIndex 
 * Outputs        : None  
 * Returns        : None 
 * Description    : This function calculates and fills the N, ueSelectedN, 
 *                  posOfMSelSBAperiodic and LBitLabelForPeriodic values.
 ****************************************************************************/
void getLAndNValues(UInt16 ueIndex, UInt8 cellIndex)
{
    UInt8 count = 0;
    DLUEContext *newDLUEContext_p = PNULL;
    newDLUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    /*SPR 12146 Fix start*/
    if(PNULL == newDLUEContext_p)
    {
        fprintf(stderr,"\n [%s] UE context is NULL for ueIndex=[%d]\n",__func__, ueIndex);
        /* SPR 12539 fix start */
        return;
        /* SPR 12539 fix end */
    }
    /*SPR 12146 Fix End*/
    if ( (CQI_MODE_2_0 == newDLUEContext_p->dlCQIInfo.aperiodicModeType) ||
            (CQI_MODE_2_2 == newDLUEContext_p->dlCQIInfo.aperiodicModeType) )
    {
        if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 8 && 
                cellConfigUeSim_g[cellIndex].dlAvailableRBs <= 10)
        {
            newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize = 2;
            newDLUEContext_p->dlCQIInfo.preferredSubbands = 1;
            newDLUEContext_p->dlCQIInfo.ueSelectedN = 
                ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
                        newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize);
        }
        else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 11 && \
                cellConfigUeSim_g[cellIndex].dlAvailableRBs <=26)
        {
            newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize = 2;
            newDLUEContext_p->dlCQIInfo.preferredSubbands = 3;
            newDLUEContext_p->dlCQIInfo.ueSelectedN = 
                ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
                        newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize);
        }
        else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 27 &&\
                cellConfigUeSim_g[cellIndex].dlAvailableRBs <=63)
        {
            newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize = 3;
            newDLUEContext_p->dlCQIInfo.preferredSubbands = 5;
            newDLUEContext_p->dlCQIInfo.ueSelectedN = 
                ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
                        newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize);
        }
        else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 64 &&\
                cellConfigUeSim_g[cellIndex].dlAvailableRBs <=110)
        { 
            newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize = 4;
            newDLUEContext_p->dlCQIInfo.preferredSubbands = 6;
            newDLUEContext_p->dlCQIInfo.ueSelectedN = 
                ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
                        newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize);
        }

        /* This will contain Binomial cofficient of ue selected N and 
         * M preferred subband  from nCm_g */
        UInt32 nCm = 0;
        initUESelectedBinomalValues(ueIndex);
        nCm = getBinomialCofficent(newDLUEContext_p->dlCQIInfo.ueSelectedN,
                newDLUEContext_p->dlCQIInfo.preferredSubbands);
        newDLUEContext_p->dlCQIInfo.posOfMSelSBAperiodic = 
            ceil(log((UDouble32)nCm)/log(2)); 

        for(count=0; count<MAX_CQI_VALUES_PER_PROFILE; count++)
        {
            /* Aperiodic CQI Changes Start */
            cqiAperiodicReportMode20_g[cellIndex][count].posOfMSelSubband = 
                getposOfMSelSBForAperiodicMode20(count,ueIndex,cellIndex);
            cqiAperiodicReportMode22_g[cellIndex][count].posOfMSelSubband_RI_1 = 
                getposOfMSelSBForAperiodicMode22RIOne(count,ueIndex, cellIndex);
            cqiAperiodicReportMode22_g[cellIndex][count].posOfMSelSubband_RI_2 = 
                getposOfMSelSBForAperiodicMode22RITwo(count,ueIndex, cellIndex);
            /* Aperiodic CQI Changes End */
            /* EICIC +*/
            cqiAperiodicReportMode20_g_2[cellIndex][count].posOfMSelSubband = 
                getposOfMSelSBForAperiodicMode20Abs(count,ueIndex,cellIndex);
            cqiAperiodicReportMode22_g_2[cellIndex][count].posOfMSelSubband_RI_1 = 
                getposOfMSelSBForAperiodicMode22RIOneAbs(count,ueIndex, cellIndex);
            cqiAperiodicReportMode22_g_2[cellIndex][count].posOfMSelSubband_RI_2 = 
                getposOfMSelSBForAperiodicMode22RITwoAbs(count,ueIndex, cellIndex);
            /* EICIC -*/

        }
    }

    if ( (CQI_MODE_1_2 == newDLUEContext_p->dlCQIInfo.aperiodicModeType) ||
            (CQI_MODE_3_0 == newDLUEContext_p->dlCQIInfo.aperiodicModeType) ||
            (CQI_MODE_3_1 == newDLUEContext_p->dlCQIInfo.aperiodicModeType) ||
            (CQI_PERIODIC_TYPE_10 == newDLUEContext_p->dlCQIInfo.periodicModeType) ||
            (CQI_PERIODIC_TYPE_11 == newDLUEContext_p->dlCQIInfo.periodicModeType) ||
            (CQI_PERIODIC_TYPE_20 == newDLUEContext_p->dlCQIInfo.periodicModeType) ||
            (CQI_PERIODIC_TYPE_21 == newDLUEContext_p->dlCQIInfo.periodicModeType) )
    {
        if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 8 && \
                cellConfigUeSim_g[cellIndex].dlAvailableRBs <= 10)
        {
            newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize = 4;
            newDLUEContext_p->dlCQIInfo.bandwidthParts = 1;
            newDLUEContext_p->dlCQIInfo.N = 
                ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
                        newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize);
        }
        else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 11 && \
                cellConfigUeSim_g[cellIndex].dlAvailableRBs <=26)
        {
            newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize = 4;
            newDLUEContext_p->dlCQIInfo.bandwidthParts = 2;
            newDLUEContext_p->dlCQIInfo.N = 
                ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
                        newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize);
        }
        else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 27 &&\
                cellConfigUeSim_g[cellIndex].dlAvailableRBs <=63)
        {
            newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize = 6;
            newDLUEContext_p->dlCQIInfo.bandwidthParts = 3;
            newDLUEContext_p->dlCQIInfo.N = 
                ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
                        newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize);
        }
        else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 64 && \
                cellConfigUeSim_g[cellIndex].dlAvailableRBs <=110)
        { 
            newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize = 8;
            newDLUEContext_p->dlCQIInfo.bandwidthParts = 4;
            newDLUEContext_p->dlCQIInfo.N = 
                ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
                        newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize);
        }
        newDLUEContext_p->dlCQIInfo.LBitLabelForPeriodic =  
            ceil((UDouble32)(log(ceil((UDouble32)\
                                cellConfigUeSim_g[cellIndex].dlAvailableRBs/
                                newDLUEContext_p->dlCQIInfo.aperiodicSubBandSize/
                                newDLUEContext_p->dlCQIInfo.bandwidthParts))/log(2)));
    }

#ifdef UESIM_ENHANCEMENTS
    if(cellConfigUeSim_g[cellIndex].dlAvailableRBs <= 10)
    {
        cellConfigUeSim_g[cellIndex].rat0RbgSize = 1;  
    }
    else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 11 &&\
            cellConfigUeSim_g[cellIndex].dlAvailableRBs <=26)
    {
        cellConfigUeSim_g[cellIndex].rat0RbgSize = 2;  
        if(15 == cellConfigUeSim_g[cellIndex].dlAvailableRBs)
        {
            cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x40;  
            cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated = 0x3F;  
            cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x180;  
        }
        else if(25 == cellConfigUeSim_g[cellIndex].dlAvailableRBs)
        {
            cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x800;  
            cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated = 0x7FF;  
            cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x3000;  
        }

    }
    else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 27 && \
            cellConfigUeSim_g[cellIndex].dlAvailableRBs <=63)
    {
        cellConfigUeSim_g[cellIndex].rat0RbgSize = 3;  
        cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x4000;  
        cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated = 0x3FFF;  
        cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x18000;  
    }
    else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 64 && \
            cellConfigUeSim_g[cellIndex].dlAvailableRBs <=110)
    { 
        cellConfigUeSim_g[cellIndex].rat0RbgSize = 4;  
        if(75 == cellConfigUeSim_g[cellIndex].dlAvailableRBs)
        {
            cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x10000;  
            cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated  = 0xFFFF;  
            cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x60000;  
        }
        else if(100 == cellConfigUeSim_g[cellIndex].dlAvailableRBs)
        {
            cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x400000;  
            cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated = 0x3FFFFF;  
            cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x1800000;  
        }

    }
#endif    
}
/* - CQI_4.1 */
/****************************************************************************/
 /*!
  * \fn           static inline MacRetType parseSpsConfigInfo(
  *                                 UInt8 **message_pp, 
  *                                 UInt16 tagLength,
  *                                 UeContextForDownlink *dlCreateRequest_p) 
  * \param [in]   *message_p - Pointer to the data received from RRC
  * \param [in]   tagLen - tag length of the SPS_CONFIG
  * \param [Out]  *dlCreateRequest_p - Pointer to UE context for downlink
  * \return       MacRetType
  * \brief
  *  Description   : This function check for the parameters from the RRC and 
  *                     stored in the UeContextForDownlink structure
  */
 /******************************************************************************/
static inline MacRetType parseSpsConfigInfo(
                             UInt8 *message_p,
                             UInt16 tagLength,       
                             ueContextForDownlink *dlCreateRequest_p,
                             ueContextForUplink *ulCreateRequest_p)
{ 
    UInt8  index     = 0;
    UInt8 *msg_p     = message_p;
    UInt16 tag       = 0;
    UInt16 tagLen    = tagLength;
    UInt16 count     = TAG_LEN;

    spsDlInfo *spsDownlinkInfo_p = \
              &(dlCreateRequest_p->spsDownlinkInfo);
    spsUlInfo *spsUplinkInfo_p = \
              &(ulCreateRequest_p->spsUplinkInfo);
    /* Currnently this param is not required for Ue Sim */
#if 0
    spsDownlinkInfo_p->spsDownlinkSetupInfo.\
              N1PucchAnPersistentListVal.numOfN1PucchAnPersistent = 0;
#endif

 
    if ( SPS_CONFIG_TAG_MIN_VALUE  > tagLen || 
                                 SPS_CONFIG_TAG_MAX_VALUE < tagLen )
    {        
        return MAC_FAILURE;
    }
    if(tagLength > SPS_CONFIG_TAG_MIN_VALUE)
    {
        while(count < tagLength)
        {
            tag =  LTE_GET_U16BIT(msg_p);
            msg_p += 2;
            count += 2;
            tagLen = LTE_GET_U16BIT(msg_p);
            msg_p += 2;
            count += 2;

            switch(tag)
            {
                case SPS_DL_INFO :
                {
                    if (SPS_DL_INFO_TAG_MIN_VALUE  > tagLen || 
                        SPS_DL_INFO_TAG_MAX_VALUE < tagLen )
                    {         
                        return MAC_FAILURE;
                    }
                    spsDownlinkInfo_p->requestType = *msg_p++;
                    count++;
                    if (spsDownlinkInfo_p->requestType > SPS_SETUP)
                    {
                        return MAC_FAILURE;
                    }
                    /* setting the isSpsDlInfoPresent falg true */
                    dlCreateRequest_p->isSpsDlInfoPresent = TRUE;
                }
                break;
                case SPS_DL_SETUP_INFO :
                {
                    if (SPS_DL_SETUP_INFO_TAG_MIN_VALUE  > tagLen || 
                        SPS_DL_SETUP_INFO_TAG_MAX_VALUE < tagLen )
                    {         
                        return MAC_FAILURE;
                    }
                    spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        semiPersistentSchedIntervalDL = *msg_p++;
                    count++;
                    if (spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        semiPersistentSchedIntervalDL > SPS_ITTERVAL_640)
                    {
                        return MAC_FAILURE;
                    }
                    spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        numberOfConfSpsProcesses = *msg_p++;
                    count++;
                    if ((spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        numberOfConfSpsProcesses < MIN_NUMBER_OF_CONF_SPS_PROCESS) || \
                        (spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        numberOfConfSpsProcesses > MAX_NUMBER_OF_CONF_SPS_PROCESS))
                    {
                        return MAC_FAILURE;
                    }

                }
                break;
                case SPS_N1_PUCCH_AN_PERSIST_LIST:
                {
                    if (SPS_N1_PUCCH_AN_PERSIST_LIST_TAG_VALUE != tagLen)
                    {         
                        return MAC_FAILURE;
                    }
                    index = spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        N1PucchAnPersistentListVal.numOfN1PucchAnPersistent++;
                    spsDownlinkInfo_p->spsDownlinkSetupInfo.N1PucchAnPersistentListVal.\
                    N1PucchAnPersistent[index]= LTE_GET_U16BIT(msg_p);
                    msg_p += 2;
                    count += 2;
                    if ((spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        N1PucchAnPersistentListVal.N1PucchAnPersistent[index] > \
                        N1_PUCCH_AN_PERSISTENT_MAX_VALUE) || \
                        (spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                         N1PucchAnPersistentListVal.numOfN1PucchAnPersistent < \
                        MIN_NUM_OF_PUCCH_AN_PERSISTENT) || \
                        (spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        N1PucchAnPersistentListVal.numOfN1PucchAnPersistent > \
                        MAX_NUM_OF_PUCCH_AN_PERSISTENT))
                    {
                        return MAC_FAILURE;
                    }
                }
                break;
                case SPS_DL_MAX_HARQ_RETX :
                {
                    if (SPS_DL_MAX_HARQ_RETX_TAG_LEN != tagLen)
                    {         
                        return MAC_FAILURE;
                    }
                    spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        isMaxSpsHarqRetxPresent = TRUE;
                    spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        maxSpsHarqRetx = *msg_p++;
                    count++;
                    if ((spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                         maxSpsHarqRetx < MAX_SPS_HARQ_RETX_MIN_VALUE )|| \
                        (spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        maxSpsHarqRetx > MAX_SPS_HARQ_RETX_MAX_VALUE))
                    {
                        return MAC_FAILURE;
                    }
                }
                break;
                case SPS_DL_EXPLICIT_RELEASE_AFTER :
                {
                    if (SPS_DL_EXPLICIT_RELEASE_AFTER_TAG_LEN != tagLen)
                    {         
                        return MAC_FAILURE;
                    }
                    spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        isExplicitReleaseAfterPresent = TRUE;
                    spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        explicitReleaseAfter = *msg_p++;
                    count++;
                    if ((spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                         explicitReleaseAfter < EXPLICIT_RELEASE_AFTER_MIN_VALUE) ||\
                        (spsDownlinkInfo_p->spsDownlinkSetupInfo.\
                        explicitReleaseAfter > EXPLICIT_RELEASE_AFTER_MAX_VALUE))
                    {
                        return MAC_FAILURE;
                    }
                }
                break;
                case SPS_UL_INFO :
                {
                    if (SPS_UL_INFO_TAG_MIN_VALUE  > tagLen || 
                        SPS_UL_INFO_TAG_MAX_VALUE < tagLen )
                    {         
                        return MAC_FAILURE;
                    }
                    spsUplinkInfo_p->requestType = *msg_p++;
                    count++;
                    if (spsUplinkInfo_p->requestType > SPS_SETUP)
                    {
                        return MAC_FAILURE;
                    }
                    /* setting the isSpsUlInfoPresent flag true */
                    ulCreateRequest_p->isSpsUlInfoPresent = TRUE;
                }
                break;
                case SPS_UL_SETUP_INFO :
                {
                    if (SPS_UL_SETUP_INFO_TAG_MIN_VALUE  > tagLen || 
                        SPS_UL_SETUP_INFO_TAG_MAX_VALUE < tagLen )
                    {         
                        return MAC_FAILURE;
                    }
                    spsUplinkInfo_p->spsUplinkSetupInfo.\
                        semiPersistentSchedIntervalUL = *msg_p++;
                    count++;
                    if (spsUplinkInfo_p->spsUplinkSetupInfo.\
                        semiPersistentSchedIntervalUL > SPS_ITTERVAL_640)
                    {
                        return MAC_FAILURE;
                    }
                    spsUplinkInfo_p->spsUplinkSetupInfo.\
                        implicitReleaseAfter = *msg_p++;
                    count++;
                    if (spsUplinkInfo_p->spsUplinkSetupInfo.\
                         implicitReleaseAfter >= INVALID_IMPLICIT_RELEASE)
                    {
                        return MAC_FAILURE;
                    }
                }
                break;
                case P_ZERO_PERSISTENT :
                {
                    if (SPS_UL_P_ZERO_PERSISTENT_TAG_LEN_VALUE  != tagLen) 
                    {         
                        return MAC_FAILURE;
                    }
    /* Currnently this param is not required for Ue Sim */
                    *msg_p++;
                    count++;
                    *msg_p++;
                    count++;
    /* Currnently this param is not required for Ue Sim */
#if 0
                    spsUplinkInfo_p->spsUplinkSetupInfo.\
                        isP0TagPresent = TRUE;
                    spsUplinkInfo_p->spsUplinkSetupInfo.\
                        p0PersistentInfo.p0NominalPuschPersistent = *msg_p++;
                    count++;
                    if ((spsUplinkInfo_p->spsUplinkSetupInfo.\
                        p0PersistentInfo.p0NominalPuschPersistent < \
                        P0_NOMINAL_PUSCH_PERSISTENT_MIN_VALUE )|| \
                        (spsUplinkInfo_p->spsUplinkSetupInfo.\
                        p0PersistentInfo.p0NominalPuschPersistent > \
                        P0_NOMINAL_PUSCH_PERSISTENT_MAX_VALUE))
                    {
                        return MAC_FAILURE;
                    }
                    spsUplinkInfo_p->spsUplinkSetupInfo.\
                        p0PersistentInfo.p0UePuschPersistent = *msg_p++;
                    count++;
                    if ((spsUplinkInfo_p->spsUplinkSetupInfo.\
                        p0PersistentInfo.p0UePuschPersistent < \
                        P0_UE_PUSCH_PERSISTENT_MIN_VALUE) || \
                        (spsUplinkInfo_p->spsUplinkSetupInfo.\
                        p0PersistentInfo.p0UePuschPersistent > \
                        P0_UE_PUSCH_PERSISTENT_MAX_VALUE))
                    {
                        return MAC_FAILURE;
                    }
#endif
                }
                break;
                case TWO_INTERVALS_CONFIG :
                {
                    if (SPS_UL_TWO_INTERVALS_CONFIG_TAG_LEN_VALUE  != tagLen)
                    {         
                        return MAC_FAILURE;
                    }
                    /* + SPS_TDD_Changes */
                    spsUplinkInfo_p->spsUplinkSetupInfo.\
                        isTwoIntervalConfigPresent = TRUE;
                    spsUplinkInfo_p->spsUplinkSetupInfo.\
                        twoIntervalsConfig = *msg_p++;
                    count++;
                    if (spsUplinkInfo_p->spsUplinkSetupInfo.\
                            isTwoIntervalConfigPresent > LTE_TRUE)
                    {
                        return MAC_FAILURE;
                    }
                    /* - SPS_TDD_Changes */
                }
                break;
            };
            
        };
    }
    if (count != tagLength)
    {        
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

/* + TM7_8 Changes Start */
void parseAngleOfArrivalValues()
{
	FILE *fp;
	UInt16  length                   = 0;
	UInt8 line[ULA_MAX_LINE_SIZE]    = {0};
	UInt8 temp[ULA_MAX_LINE_SIZE]    = {0};
	UInt8 count                      = 0;
	UInt8 numberOfAntenna            = 0;
	SInt8 cellIndex = -1;
#if 0
	char AOA_VALUES_FOR_ANTENNA_1_str[MAX_STR_LENGTH];
	char AOA_VALUES_FOR_ANTENNA_2_str[MAX_STR_LENGTH];
	char AOA_VALUES_FOR_ANTENNA_3_str[MAX_STR_LENGTH];
	char AOA_VALUES_FOR_ANTENNA_4_str[MAX_STR_LENGTH];
#endif

	fp = fopen(ANGLE_OF_ARRIVAL_VALUE_FILE_PATH,"r");

	if (fp == NULL)
	{
		printf("Unable to open file.\n");
	}

	while((fgets(line,ULA_MAX_LINE_SIZE,fp) != NULL) && cellIndex < numCells_g)
	{
		if ((strncmp(line,"#",1) == 0) || (strncmp(line,"\n",1) == 0))
		{
			continue;
		}

		trimSpaces(line);
		length = strlen(line);
		/*createTokenStringForAngleOfArrival(AOA_VALUES_FOR_ANTENNA_1_str,AOA_VALUES_FOR_ANTENNA_2_str,
		  AOA_VALUES_FOR_ANTENNA_3_str, AOA_VALUES_FOR_ANTENNA_4_str);
		  */
		if (strncmp(line,"[CELL]",6) == 0)
		{
			fprintf(stderr, "\n 1. [%s] cellIndex = %d", __func__,cellIndex);
			strncpy(temp,line+6,length-6);
			cellIndex++;
            /* SPR 10055 Fix Start */
            numberOfAntenna = 0;
            /* SPR 10055 Fix End */
		}
		if (cellIndex >= numCells_g)
			break;
		if (strncmp(line,"AOA_VALUES_FOR_ANTENNA_1",24) == 0)
		{
			strncpy(temp,line+25,length-25);
			count = stringTokenSArray(temp,(unsigned int *)angleOfArrivalValues_g[cellIndex],
					numberOfAntenna,MAX_SF_VALUE);
            numberOfAntenna++;
		}
		else if (strncmp(line,"AOA_VALUES_FOR_ANTENNA_2",24) == 0)
		{
			strncpy(temp,line+25,length-25);
			count = stringTokenSArray(temp,(unsigned int *)angleOfArrivalValues_g[cellIndex],
					numberOfAntenna,MAX_SF_VALUE);
			numberOfAntenna++;
		}
		else if (strncmp(line,"AOA_VALUES_FOR_ANTENNA_3",24) == 0)
		{
			strncpy(temp,line+25,length-25);
			count = stringTokenSArray(temp,(unsigned int *)angleOfArrivalValues_g[cellIndex],
					numberOfAntenna,MAX_SF_VALUE);
			numberOfAntenna++;
		}
		else if (strncmp(line,"AOA_VALUES_FOR_ANTENNA_4",24) == 0)
		{
			strncpy(temp,line+25,length-25);
			count = stringTokenSArray(temp,(unsigned int *)angleOfArrivalValues_g[cellIndex],
					numberOfAntenna,MAX_SF_VALUE);
			numberOfAntenna++;
		}
	}
	close(fp);
}
/* - TM7_8 Changes End */
/* + CA_AB */
/****************************************************************************
 * Function Name  : parseMACInitCellReq
 * Inputs         : msgBuf_p - Pointer to msg read from socket
 *                  msgLen_p - Pointer tot he msg length of msg recv
 * Outputs        : None
 * Returns        : MAC_SUCCESS or FAILURE
 * Variables      :
 * Description    : This API parse the init MAC Layer request and initalizes
 *                  MAC Layer after validating the request.
 ********************* ********************************************************/
/* + CA_AB */
UInt16 parseMACInitCellReq(UInt8 *msgBuf_p, UInt16 *msgLen_p, UInt8 cell_index)
{
	if(*msgLen_p != (MAC_API_HEADER_LEN + MAC_INIT_CELL_REQ_MANDATORY_LENGTH))
	{
		return MAC_SYNTAX_ERROR;
	}

	InitCellReqParams initCellReqParams = {0};
	UInt16 current = MAC_API_HEADER_LEN;
	UInt16 numOfUe = 0, rntiStartRange = 0, rntiEndRange = 0;
	UInt16 resp = MAC_SUCCESS;
	UInt8 index = 0;

	numOfUe = LTE_GET_U16BIT(msgBuf_p+current);
	current += 2;

	rntiStartRange = LTE_GET_U16BIT(msgBuf_p+current);
	current += 2;

	rntiEndRange = LTE_GET_U16BIT(msgBuf_p+current);
	current += 2;
	if((rntiStartRange < 0x01) || (rntiStartRange > 0xFFF2) ||
			(rntiEndRange < 0x01) || (rntiEndRange > 0xFFF2) ||
			(rntiEndRange <= rntiStartRange)) {
		return MAC_SYNTAX_ERROR;
	}    
	initCellReqParams.fieldBitMask = LTE_GET_U32BIT(msgBuf_p+current); 
	current += 4;

	initCellReqParams.maxUEsToBeScheduledDl = *(msgBuf_p + current);
	current++;

	initCellReqParams.maxUEsToBeScheduledUl = *(msgBuf_p + current);
	current++;

	initCellReqParams.cqiThreshold = *(msgBuf_p + current);
	current++;

	initCellReqParams.dlRbRestrictionFactor = *(msgBuf_p + current);
	current++;

	initCellReqParams.ulRbRestrictionFactor = *(msgBuf_p + current);
	current++;

	initCellReqParams.ambrTokenLimitFactor = *(msgBuf_p + current);
	current++;

	initCellReqParams.gbrTokenLimitFactor = *(msgBuf_p + current);
	current++;

    initCellReqParams.commChWithMibAllowed = *(msgBuf_p + current);
    current++;

    initCellReqParams.dlSchWithMibAllowed = *(msgBuf_p + current);
    current++;

    initCellReqParams.maxMcs16Qam = *(msgBuf_p + current);
    current++;

    initCellReqParams.avgNumAmLcPerUe = *(msgBuf_p + current);
    current++;

    initCellReqParams.avgNumUmLcPerUE = *(msgBuf_p + current);
    current++;

    initCellReqParams.IsLastAttempt = *(msgBuf_p + current);
    current++;
    
    cellConfigUeSim_g[cell_index].startSpsCrnti = LTE_GET_U16BIT(msgBuf_p+current);
    current +=2;

    cellConfigUeSim_g[cell_index].endSpsCrnti = LTE_GET_U16BIT(msgBuf_p+current);
    current +=2;

    for(index = 0; index < MAX_QCI; index++)
    {
        cellConfigUeSim_g[cell_index].ueSpsSysWideParam.qciSpslist[index] = *(msgBuf_p + current);
        current++;
    }

#ifdef LTE_EMBMS_SUPPORTED
    eMBMSEnable_g[cell_index]=*(msgBuf_p + current);
#endif
/* Need to uncomment */
	 cellConfigUeSim_g[cell_index].lowerCRNTIBound = rntiStartRange ;
    cellConfigUeSim_g[cell_index].upperCRNTIBound = rntiEndRange ;
	/*UE_TESTING */
    

	/*UE_TESTING */
#ifdef UT_TESTING
    MAC_EMULATOR_LOG(LOG_INFO,"rntiStartRange = %d rntiEndRange = %d CellIndex %d\n",rntiStartRange, rntiEndRange, cell_index);
  #endif
   
    setCellConfigValues(cell_index); 
   
    initializeMAC(cell_index);
    
    stoppedProcessing_g[cell_index] = 0;

    MAC_EMULATOR_LOG(LOG_INFO,"\n########## Initialized MAC Emulator Successfully ###########\n\n"); 
    return resp;
}    
/* - CA_AB */
/* + CA_PHASE2 */
/****************************************************************************
 * Function Name  : parseExtendedUeInfo
 * Inputs         : uplkUEInfo_p - Ptr to UL UE Info structure
 *                  dwlkUEInfo_p - Ptr to DL UE Info structure
 *                  len - Length of the TLV
 *                  current_p - current Idx of the message received.
 *                  msg_p - buffer received from RRC Layer.
 *                  uplkLCCount_p - UL LC count to be configured.
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_SYNTAX_ERROR.
 * Variables      :
 * Description    : This API buffer to be parsed for EXTENDED_UE_INFO,
 *                  and returns MAC_SUCCESS or MAC_SYNTAX_ERROR on failure.
 ****************************************************************************/
#if 0
MacRetType parseExtendedUeInfo(ueContextForUplink *ulUEInfo_p,
        ueContextForDownlink *dlUEInfo_p,
        UInt16 extendeUeInfoTagLen, 
        UInt8 *msg_p, UInt8 cellIndex)
{
    UInt16 resp = MAC_SUCCESS;
    UInt16 tagLen = 0;
    UInt16 tag = 0;
    
    while ( extendeUeInfoTagLen > 0)
    {
        tag = LTE_GET_U16BIT(msg_p);
        msg_p += 2;

        tagLen  = LTE_GET_U16BIT(msg_p);
        msg_p += 2;

        extendeUeInfoTagLen -= tagLen;
        switch(tag)
        {
            case UE_CATEGORY_V1020:
                if (tagLen != UE_CATEGORY_V1020_LEN)
                {
                    lteWarning("Mandatory fields missing in UE_CATEGORY_V1020_LEN.\n");
                    return MAC_SYNTAX_ERROR;
                }
                dlUEInfo_p->ueCategoryV1020 = *msg_p++;
				ulUEInfo_p->ueCategoryV1020 = dlUEInfo_p->ueCategoryV1020 ;
				dlUEInfo_p->ueCategoryReqType = TRUE ;
				ulUEInfo_p->ueCategoryReqType = TRUE ;
				resp = macValidateUeCategoryV1020(dlUEInfo_p->ueCategoryV1020);
				    if (resp != MAC_SUCCESS)
					    {
						    return resp;
						}
									
                break;

            case PUCCH_FORMAT_V1020:
                if (tagLen != PUCCH_FORMAT_V1020_LEN)
                {
                    lteWarning("Mandatory fields missing in PUCCH_FORMAT_V1020_LEN.\n");
                    return MAC_SYNTAX_ERROR;
                }
					resp = parseReconfigPUCCHFormatV1020Info(ulUEInfo_p, dlUEInfo_p, 
															tagLen, &msg_p, cellIndex);
				    	if (resp != MAC_SUCCESS)
					    {
						    return resp;
						}
                	break;

            case CA_CONFIG:
				resp = parseReconfigCAConfigInfo(ulUEInfo_p, dlUEInfo_p, tagLen, &msg_p, cellIndex);
					if (resp != MAC_SUCCESS)
				 	{
					    return resp;
					}
                break;

            default:
                lteWarning("Invalid tag received for RECONFIG_LC_REQ\n");
                return MAC_SYNTAX_ERROR;
        }
    }

    return MAC_SUCCESS;
}
#endif
/****************************************************************************
 ** Function Name  : macValidateUeCategoryV1020
 ** Inputs         : ueCategoryV1020
 ** Outputs        : None
 ** Returns        : MAC_SUCCESS or MAC_FAILURE
 ** Description    : This API takes ueCategory for CA to validate and returns the
 **                  result of validation to the calling function.
 ******************************************************************************/
STATIC inline MacRetType macValidateUeCategoryV1020(UInt8 ueCategoryV1020)
{
	if ( (ueCategoryV1020 > MAC_MAX_UE_CATEGORY_V1020) ||
			(ueCategoryV1020 < MAC_MIN_UE_CATEGORY_V1020) )
	{
		lteWarning("Invalid range for UeCategory .\n");
		return MAC_FAILURE;
	}
	return MAC_SUCCESS;
}
/******************************************************************************
 ** Function Name  : parseReconfigPUCCHFormatV1020Info
 ** Inputs         : msgBuf_pp - pointer to the input buffer
 **                  tagLen - Length of the tag
 **                  dlUEInfo_p - pointer to Downklink config UE structure
 **                  ulUEInfo_p - pointer to Uplink config UE structure
 **                  cellIndex - cell index used at MAC
 ** Outputs        : None
 ** Returns        : MAC_SUCCESS or MAC_FAILURE
 ** Description    : This function parse the PUCCHFormatV1020 information for CA in
 **                  Create/Reconfig UE Entity Request and fills in the structure needed
 **                  by UE-MAC to create a UE context.
 *******************************************************************************/
STATIC inline MacRetType parseReconfigPUCCHFormatV1020Info( 
		ueContextForUplink *ulUEInfo_p,
		ueContextForDownlink *dlUEInfo_p,
		UInt16 tagLen,
		UInt8 **msgBuf_pp,
		UInt8 cellIndex
		)
{
    UInt8 *msg_p    = PNULL;
	MacRetType retval = MAC_SUCCESS;
	UInt16 tag=0;

    msg_p = *msgBuf_pp;
	UInt8 pucchAnCsListElement=0;
	UInt16 pucchFormatTagLength=0;
	UInt8 requestType=0;
    /* + SPR 11213 Changes */
	if ( tagLen < 4 ) 
    /* + SPR 11213 Changes */
	{
		lteWarning("PUCCH_FORMAT_V1020_INFO_INVALID_TAGLEN .\n");
		return MAC_FAILURE;
	}
	tag = LTE_GET_U16BIT(msg_p);
	//if (PUCCH_CHANNEL_SELECTION_CONFIG_V1020 != tag )
	if (PUCCH_CHANNEL_SELECTION_CONFIG_V1020 == tag )
	{
		pucchFormatTagLength = tagLen - TAG_LEN;
		msg_p += U16BIT_LEN;
		tagLen = LTE_GET_U16BIT(msg_p);
		if ( tagLen < PUCCH_CHANNEL_SELECTION_CONFIG_V1020_MIN_LEN )
		{
			lteWarning("PUCCH_CHANNEL_SELECTION_CONFIG_V1020_INFO_INVALID_TAGLEN .\n");
			return MAC_SYNTAX_ERROR;
		}
		msg_p += U16BIT_LEN;
		requestType = *msg_p++;
		dlUEInfo_p->pucchFormatR10.requestType = requestType;
		if(PUCCH_CHANNEL_SELECTION_CONFIG_V1020_SETUP == requestType)
		{
			pucchFormatTagLength = pucchFormatTagLength -
				PUCCH_CHANNEL_SELECTION_CONFIG_V1020_MIN_LEN;
			while ( pucchFormatTagLength > 0)
			{
				tag = LTE_GET_U16BIT(msg_p);
				if (PUCCH_AN_CS_LIST_V1020 == tag )
				{
				//	tag = LTE_GET_U16BIT(msg_p); 
					msg_p += U16BIT_LEN;
					tagLen = LTE_GET_U16BIT(msg_p);
					msg_p += U16BIT_LEN;
					pucchFormatTagLength = pucchFormatTagLength - tagLen;
					if ( tagLen < PUCCH_AN_CS_LIST_V1020_MIN_LEN )
					{
						lteWarning("PUCCH_AN_CS_LIST_V1020_INFO_INVALID_TAGLEN.\n");
						return MAC_SYNTAX_ERROR;
					}
					tagLen = tagLen - PUCCH_AN_CS_LIST_V1020_MIN_LEN;
					retval = parsePucchAnCsV1020(&msg_p,tagLen,
							pucchAnCsListElement,dlUEInfo_p,
							ulUEInfo_p
							,cellIndex);
					if ( retval != MAC_SUCCESS )
					{
						lteWarning("Parsing PUCCH_FORMAT_V1020 failed.\n");
						return MAC_FAILURE;
					}
					pucchAnCsListElement++;
				}
				else
				{
					retval = MAC_SYNTAX_ERROR;
					return retval;
				}
			}
		}
    /* + SPR 11213 Changes */
		else
		{
            if(PUCCH_CHANNEL_SELECTION_CONFIG_V1020_RELEASE != requestType)
            {
                lteWarning("INVALID_PARAMETER_RECEIVED PUCCH_AN_CS_LIST_V1020 INVALID_REQUEST_TYPE");
                return MAC_FAILURE;
            }

		}
    /* - SPR 11213 Changes */
	}

    *msgBuf_pp = msg_p;
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parsePucchAnCsV1020
 * Inputs         : msgBuf_pp -  pointer to the input buffer
 *                  remainingTagLen - length of remaining of the API message 
 *                  received 
 *                  pucchAnCsListElement- position of  pucchAnCs element in
 *                  pucchAnCsList
 *                  ulUEInfo_p - pointer to Uplink create UE structure
 *                  dlUEInfo_p - pointer to Downlink create UE structure
 *                  cellIndex - the cell index used at UE-MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function parses the input buffer and fills in the
 *                  structure needed by MAC to create a UE context.
 ******************************************************************************/
STATIC inline MacRetType parsePucchAnCsV1020 ( UInt8 **msgBuf_pp,
        UInt16 remainingTagLen,
        UInt8 pucchAnCsListElement,
        ueContextForDownlink *dlUEInfo_p,
        ueContextForUplink   *ulUEInfo_p,
        UInt8 cellIndex
        )
{
    UInt8 *msg_p    = PNULL;
    MacRetType resp = MAC_SUCCESS;
    UInt8 pucchIndex = 0;
    UInt16 tagLen = 0;
    UInt16 tag = 0;
    UInt16 n1Pucch=0;
    msg_p = *msgBuf_pp;
    while (remainingTagLen > 0 )
    {
        tag = LTE_GET_U16BIT(msg_p);
        msg_p += U16BIT_LEN;
        tagLen = LTE_GET_U16BIT(msg_p);
        msg_p += U16BIT_LEN;
        remainingTagLen = remainingTagLen - tagLen;
        if (PUCCH_AN_CS_V1020 == tag )
        {
            if ( PUCCH_AN_CS_V1020_LEN != tagLen )
            {
					lteWarning("PUCCH_AN_CS_V1020_INFO_INVALID_TAGLEN.\n");
					return MAC_FAILURE;
            }
            n1Pucch = LTE_GET_U16BIT(msg_p);
            msg_p += U16BIT_LEN;
            resp = validateN1Pucch(n1Pucch);
            if (resp != MAC_SUCCESS)
            {
                lteWarning("Invalid value of n1Pucch= [%u],not in range \
                          [0,2047].\n",n1Pucch);
                return resp;
            }
            dlUEInfo_p->pucchFormatR10.n1Pucch
                [pucchAnCsListElement][pucchIndex] = n1Pucch;
            ulUEInfo_p->pucchFormatR10.n1Pucch
                [pucchAnCsListElement][pucchIndex] = n1Pucch;
            pucchIndex++;
        }
        else
        {
            lteWarning("Invalid tag in parsePucchAnCsV1020\n");
            resp = MAC_SYNTAX_ERROR;
        }
    }
    *msgBuf_pp = msg_p;
    return resp;
}
/****************************************************************************
 *  Function Name  : validateN1Pucch
 *  Inputs         : n1Pucch
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE.
 *  Description    : This API takes n1Pucch to validate and returns the result
 *                   of validation to the calling function.
 * ****************************************************************************/
STATIC inline MacRetType validateN1Pucch(UInt16 n1Pucch)
{
    if ( n1Pucch > MAX_N1PUCCH_RESOURCE_VAL )
    {
        lteWarning("Invalid value for n1Pucch [%u], valid range [%u,%u] .\n",
                n1Pucch,MIN_N1PUCCH_RESOURCE_VAL,MAX_N1PUCCH_RESOURCE_VAL);
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}


/******************************************************************************
 * Function Name  : parseReconfigCAConfigInfo 
 * Inputs         : msgBuf_pp - pointer to the input buffer
 *                  tagLen - Length of the tag
 *                  ulUEInfo_p - pointer to Uplink create UE structure
 *                  dlUEInfo_p - pointer to Downlink create UE structure
 *                  cellIndex - the cell index used at UE-MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function parses the CA config information for CA in 
 *                  Create UE Entity Request and fills in the structure needed  
 *                  by MAC to create a UE context.
 ******************************************************************************/
STATIC MacRetType parseReconfigCAConfigInfo( 
    ueContextForUplink *ulUEInfo_p,
    ueContextForDownlink *dlUEInfo_p,
    UInt16 tagLen, 
	UInt8 **msgBuf_pp, 
    UInt8 cellIndex)
{
    UInt8 *msg_p    = PNULL;
    MacRetType resp = MAC_SUCCESS;
    UInt16 tag=0;

    msg_p = *msgBuf_pp;

    UInt16 caConfigTagLength=0;
    if ( tagLen < MIN_CA_CONFIG_TAG_LEN )
    {
        lteWarning("Invalid Tag Length %d for  CA_CONFIG  Info\n"
                ,tagLen);
        return MAC_FAILURE;
    }
    caConfigTagLength = tagLen;
    caConfigTagLength = caConfigTagLength - MIN_CA_CONFIG_TAG_LEN;
    while (caConfigTagLength > 0)
    {
        tag = LTE_GET_U16BIT(msg_p);
        msg_p += U16BIT_LEN;
        tagLen = LTE_GET_U16BIT(msg_p);
        msg_p += U16BIT_LEN;
        caConfigTagLength = caConfigTagLength - tagLen;
        switch( tag )
        {
            case SCELL_DEACTIVATION_TIMER_INFO :
            {
                resp = parseScellDeactivationTimer(ulUEInfo_p,dlUEInfo_p,
														tagLen,&msg_p,cellIndex);
                if ( resp != MAC_SUCCESS )
                {
                    return MAC_FAILURE;
                }
                break;
            }
            case SCELL_INFO :
            {
                resp = parseAndValidateScellInfo(ulUEInfo_p,dlUEInfo_p,
														tagLen,&msg_p,cellIndex);
                if ( resp != MAC_SUCCESS )
                {
                    return MAC_FAILURE;
                }
                break;
            }
            case CA_LC_BANDWIDTH_DIST_INFO:
            {
                resp = parseCALCBandwidthDistanceInfo(tagLen,&msg_p,
                    dlUEInfo_p);
                if ( resp != MAC_SUCCESS )
                {
                    return MAC_FAILURE;
                }
                break;
            }
            default: 
            {
                lteWarning("Invalid Tag [%d] Identifer Recieved in CA CONFIG",tag);
                return MAC_FAILURE;
            }
        }
    }

    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseScellDeactivationTimer
 * Inputs         : msgBuf_pp - pointer to the input buffer
 *                  tagLen - Length of the tag
 *                  ulUEInfo_p - pointer to Uplink create UE structure
 *                  dlUEInfo_p - pointer to Downlink create UE structure
 *                  cellIndex - the cell index used at UE-MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for MAC_FAILURE
 * Description    : This function is called on receiving SCELL_DEACTIVATION_TIMER_INFO
 *                  and returns the result of parsing to the calling function.
 * ****************************************************************************/                                        
STATIC inline MacRetType parseScellDeactivationTimer(
    ueContextForUplink *ulUEInfo_p,
    ueContextForDownlink *dlUEInfo_p,
	UInt16 tagLen,
    UInt8 **msgBuf_pp,
    UInt8 cellIndex
    )
{
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    UInt8 scellDeactivationTimer=0;

    if ( tagLen != SCELL_DEACTIVATION_TIMER_INFO_TAG_LEN )
    {
        lteWarning("Invalid Tag Length %d for SCELL_DEACTIVATION_TIMER_INFO_TAG_LEN  Info\n"
                ,tagLen);
        return MAC_FAILURE;
    }
    scellDeactivationTimer = *msg_p++;
    if(MAC_SUCCESS!=(validateScellDeactivationTimer(scellDeactivationTimer)))
    {
        lteWarning("Parsing scellDeactivationTimer failed.\n");
        return MAC_FAILURE;
    }
    dlUEInfo_p->carrierAggrConfig.scellDeactivationTimer = 
        scellDeactivationTimer;
    ulUEInfo_p->carrierAggrConfig.scellDeactivationTimer = 
        scellDeactivationTimer;
    /*phase-3 changes start*/
    ulUEInfo_p->carrierAggrConfig.scellInfo[1].scellDeactivationExpirytti = scellDeactivationTimer + getCurrentglobaltick();
    /*phase-3 changes end*/
    
	*msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateScellDeactivationTimer
 * Inputs         : scellDeactivationTimer
 * Outputs        : None
 *  Returns       : MAC_SUCCESS or reason for failure.
 *                  (possible values are rf2 = 20ms, rf4=40ms, rf8=80ms, 
 *                  rf16=160ms, rf32=320ms, rf64=640ms, rf128=1280ms)
 * Description    : This API takes scellDeactivationTimer to validate and
 *                  returns the result of validation to the calling function.
 * ****************************************************************************/                                        
STATIC inline MacRetType validateScellDeactivationTimer(
    UInt8 scellDeactivationTimer)
{
    UInt8 count=0;
    for (count =0;count < SCELL_DEACTIVATION_TIMER_VALUE_COUNT;count++)
    {
        if(scellDeactivationTimer == validScellDeactivationTimerValues_g[count])
            break;
    }
    if(count < SCELL_DEACTIVATION_TIMER_VALUE_COUNT )
    {
        return MAC_SUCCESS;
    }
    else
    {
        lteWarning("Invalid value of  scellDeactivationTimer [%u].\n",
                scellDeactivationTimer);
        return MAC_FAILURE;
    }
}

/****************************************************************************
 * Function Name  : parseAndValidateScellInfo
 * Inputs         : msgBuf_pp - pointer to the input buffer
 *                  tagLen - Length of the tag
 *                  ulUEInfo_p - pointer to Uplink create UE structure
 *                  dlUEInfo_p - pointer to Downlink create UE structure
 *                  cellIndex - the cell index used at UE-MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for MAC_FAILURE
 * Description    : This function is called on receiving SCELL_INFO and
 *                  returns the result of parsing of Scell Info to the 
 *                  calling function.
 * ****************************************************************************/
STATIC inline MacRetType parseAndValidateScellInfo(
    ueContextForUplink *ulUEInfo_p,
    ueContextForDownlink *dlUEInfo_p,
    UInt16 tagLen,
    UInt8 **msgBuf_pp,
    UInt8 cellIndex
    )
{
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    
	if ( tagLen < MIN_SCELL_INFO_TAG_LEN )
    {
        lteWarning("Invalid Tag Length %d for SCELL_INFO_TAG_LEN  Info\n"
                ,tagLen);
        return MAC_FAILURE;
    }
    if (MAC_SUCCESS !=  parseScellInfo(ulUEInfo_p,dlUEInfo_p,tagLen,&msg_p,
							  dlUEInfo_p->carrierAggrConfig.scellCount,cellIndex))
    {
        lteWarning("Parsing SCELL_INFO failed.\n");
        return MAC_FAILURE;
    }
    dlUEInfo_p->carrierAggrConfig.scellCount++;
    //rajni - temp fix
    ulUEInfo_p->carrierAggrConfig.scellCount++;

    *msgBuf_pp = msg_p;

    return MAC_SUCCESS;
}
/****************************************************************************
 *  Function Name  : parseScellInfo
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                 : tagLen - tag length  of the SCELL_INFO API message received
 *                   scellCount - SCELL index for which data is going to be parsed
 *                   dlUEInfo_p - pointer to Downlink create UE structure
 *                   ulUEInfo_p - pointer to Uplink create UE structure
 *                   cellIndex - the cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for SCELL_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellInfo(
        ueContextForUplink   *ulUEInfo_p,
        ueContextForDownlink *dlUEInfo_p,
        UInt16 tagLen,
		UInt8 **msgBuf_pp,
        UInt8 scellCount,
        UInt8 cellIndex
        ) 
{
    UInt8 cellId=0,scellIndex=0,operationType=0;
    UInt16 tag, remainingTagLen = 0;
    MacRetType resp = MAC_SUCCESS;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    /* SPR 14223 Fix : Start */
    DLUEContext *dlUEContext_p = PNULL;
    DLCQIInfo *scellUlCQIInfo = PNULL;
    DLCQIInfo *scellDlCQIInfo = PNULL;
    /* SPR 14223 Fix : End */

    /* + SPR 11213 Changes */
    remainingTagLen = tagLen;
    /* - SPR 11213 Changes */
//AZIM check below condition
/* Mandeep */
   ulUEInfo_p->cqiinfoflag = FALSE;
    if(scellCount > 0)
    {
        lteWarning("Currently one Scell is supported.\n");
        return MAC_FAILURE;
    }
    ScellInfo *dlScellInfo_p = &dlUEInfo_p->carrierAggrConfig.
                                scellInfo[scellCount];
    ScellInfo *ulScellInfo_p = &ulUEInfo_p->carrierAggrConfig.
                                scellInfo[scellCount];
     ulScellInfo_p->cqiInfo.riConfigIndex = MAC_RI_CONFIG_INDEX_INVALID;
    cellId = *msg_p++;
    if ( cellId != cellIndex)
    {
        lteWarning("SCELL_NOT_EXISTS\nInvalid value for cellIndex [%u], valid range [%u,%u] .\n",
                cellId,MIN_SCELL_ID,MAX_SCELL_ID);
//        return MAC_FAILURE;
    }
	/*
    if(!(CELL_PHY_RUNNING == 
        cellSpecificParams_g.cellConfigAndInitParams_p
        [internalCellIndex]->cellState)) // TODO CELL_PHY_RUNNING_CONFIG_RECVD
    {
        lteWarning("SCELL_NOT_RUNNING\nInvalid [cellIndex][cell state] [%u],[%u] .\n",
                cellIndex,cellSpecificParams_g.cellConfigAndInitParams_p
                [internalCellIndex]->cellState);
        return MAC_FAILURE;
    }
   */
    dlScellInfo_p->cellIndex = cellIndex;
    ulScellInfo_p->cellIndex = cellIndex;
    scellIndex = *msg_p++;
     /*SPR 12127 fix start*/ 
    scellIndex = cellId;

    if(/*scellIndex < MIN_SCELL_INDEX ||*/ scellIndex > MAX_SCELL_INDEX)
    {
        lteWarning("Invalid value for scellIndex [%u], valid range [%u,%u] .\n",
                scellIndex,MIN_SCELL_INDEX,MAX_SCELL_INDEX);
        return MAC_FAILURE;

    }
     /*SPR 12127 fix end*/ 
    dlScellInfo_p->sCellIndex = scellIndex;
    ulScellInfo_p->sCellIndex = scellIndex;
    operationType= *msg_p++;
    if(OPERATION_TYPE_ADD != operationType)
    {
        lteWarning("Invalid value for operationType [%u], valid value is  [%u] .\n",
                operationType,
                OPERATION_TYPE_ADD);
        return MAC_FAILURE;

    }
    dlScellInfo_p->operationType = operationType;
    ulScellInfo_p->operationType = operationType;
    remainingTagLen = remainingTagLen - MIN_SCELL_INFO_TAG_LEN; 
    while ( remainingTagLen > 0)
    {
        tag = LTE_GET_U16BIT(msg_p);
        msg_p += U16BIT_LEN;
        tagLen = LTE_GET_U16BIT(msg_p);
        msg_p += U16BIT_LEN;
        remainingTagLen = remainingTagLen - tagLen;
        switch( tag )
        {
            case MIMO_CAPABILITY_DL_V1020 :
            {
                resp = parseScellMimoCapabilityV1020(&msg_p,tagLen,
                            dlScellInfo_p);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
				}
                 /* + SPR_14458 */
                ulScellInfo_p->rankIndicator = dlScellInfo_p->rankIndicator;
                 /* - SPR_14458 */
                break;
            }
            case TX_MODE_INFO :
            {
                resp = parseScellTxModeInfo(&msg_p,tagLen,
                            dlScellInfo_p, ulScellInfo_p, cellIndex);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            case CODEBOOK_SUBSET_RESTRICTION_V1020 :
            {
                resp = parseScellCodeBookSubsetRestV1020(&msg_p,tagLen,
                            dlScellInfo_p, ulScellInfo_p);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            case PDSCH_CONFIG_INFO :
            {
                resp = parseScellPDSCHConfigInfo(&msg_p,tagLen,
                            dlScellInfo_p, ulScellInfo_p, cellIndex);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            case CQI_INFO :
            {
                resp = parseCAReconfigUECqiInfo(&msg_p,tagLen,scellCount,
                        dlUEInfo_p, ulUEInfo_p,cellIndex);
                if (resp != MAC_SUCCESS)
                {
                    lteWarning("Parsing CQI_INFO failed.\n");
                    return resp;
                }

                break;
            }
            case DL_NUM_HARQ_PROC :
            {
                resp = parseScellDLHarqProc(&msg_p,tagLen,
                            dlScellInfo_p);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            case DL_INIT_MCS :
            {
                resp = parseScellDLInitMCS(&msg_p,tagLen,
                            dlScellInfo_p);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            case NUM_OF_LAYER_INFO :
            {
                resp = parseScellNumOfLayerInfo(&msg_p,tagLen,
                        dlScellInfo_p, ulScellInfo_p,
                        dlUEInfo_p->ueCategoryV1020,
                        cellIndex);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            case CODEBOOK_INDEX_INFO :
            {
                resp = parseScellCodebookIndexInfo(&msg_p,tagLen,
                            dlScellInfo_p, ulScellInfo_p, cellIndex);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            case USER_LOCATION_TYPE :
            {
                resp = parseScellUserLocationType(&msg_p,tagLen,
                            dlScellInfo_p, ulScellInfo_p);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            case RF_PARAMS :
            {
                resp = parseScellRFParams(&msg_p,tagLen,
                            dlScellInfo_p, ulScellInfo_p, cellIndex,
                           dlUEInfo_p->ueIndex);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            case SCELL_ACTIVATE_STATUS :
            {
                resp = parseScellActivationStatus(&msg_p,tagLen,
                            dlScellInfo_p, ulScellInfo_p, cellIndex);
                if (MAC_SUCCESS !=  resp)
                {
                    return resp;
                }
                break;
            }
            default: 
            {
                lteWarning("Invalid Tag [%d] Identifer Recieved in SCELL_INFO",tag);
                return MAC_FAILURE;
            }
        }
    }

    /* SPR 14223 Fix : Start */
    dlUEContext_p = dlUECtxInfoArr_g[dlUEInfo_p->ueIndex].dlUEContext_p;
    if (PNULL == dlUEContext_p)
    {
        lteWarning("UE does not exist corresponding to this IDx [%d]\n", dlUEInfo_p->ueIndex);
        return MAC_FAILURE;
    }
    if ( dlUECtxInfoArr_g[dlUEInfo_p->ueIndex].pendingDeleteFlag )
    {
        lteWarning("Pending Delete Flag is set for this UEIndex. %d\n",dlUEInfo_p->ueIndex);
        return MAC_FAILURE;
    }
    ulUEInfo_p = uplinkContextInfoMap_g[dlUEInfo_p->ueIndex]; 
    scellUlCQIInfo = &ulUEInfo_p->carrierAggrConfig.scellInfo[scellCount].dlCQIInfo;
    scellDlCQIInfo = &ulUEInfo_p->carrierAggrConfig.scellInfo[scellCount].dlCQIInfo;
    scellDlCQIInfo->ueCqiProfileNum = dlUEContext_p->dlCQIInfo.ueCqiProfileNum;
    scellUlCQIInfo->ueCqiProfileNum = dlUEContext_p->dlCQIInfo.ueCqiProfileNum;
    /* SPR 14223 Fix : End */

    /* SPR 12465 FIX start */
    if ( (3 != dlScellInfo_p->transmissionMode &&
                4 != dlScellInfo_p->transmissionMode) &&
            /* + TM7_8 Changes Start */
            (!((8 == dlScellInfo_p->transmissionMode) &&
               (TRUE == ulUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.pmiRiReportV920))))
        /* - TM7_8 Changes End */
    {
        ulUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo.riConfigIndex = MAC_RI_CONFIG_INDEX_INVALID;
        lteWarning("riConfigIndex is configured in TX mode "
                "other than 3 and 4.\n");
    }
    /* SPR 12465 FIX end */


    *msgBuf_pp = msg_p;
    return resp;
}
/****************************************************************************
 *  Function Name  : parseScellMimoCapabilityV1020
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for MIMO_CAPABILITY_DL_V1020
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellMimoCapabilityV1020(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p
        ) 
{
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    UInt8 numOfLayer = 0, loopCount = 0;
    if ( tagLen != MIMO_CAPABILITY_DL_V1020_TAG_LEN )
    {
        lteWarning("Invalid Tag Length %u for MIMO_CAPABILITY_DL_V1020  Info\n"
                ,tagLen);
        return MAC_SYNTAX_ERROR;
    }
    numOfLayer = *msg_p++;
    for (loopCount =0 ;loopCount < NUM_OF_LAYERS_COUNT; loopCount++)
    {
        if(validnumOfLayerValues_g[loopCount] == numOfLayer)
            break;
    }
    if(loopCount == NUM_OF_LAYERS_COUNT)
    {
        lteWarning("Invalid value for numOfLayer [%u] .\n",
                numOfLayer);
        return MAC_FAILURE;
    }
    dlScellInfo_p->numOfLayer = numOfLayer;
    /* + SPR_14458 */
    dlScellInfo_p->rankIndicator = numOfLayer;
    /* - SPR_14458 */
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseScellTxModeInfo
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *                   ulScellInfo_p - pointer to uplink Scell info
 *                   cellIndex - the cell index used at UE-MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellTxModeInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 cellIndex
        ) 
{
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    UInt8 transmissionMode = INVALID_TX_MODE;
    if (tagLen != TX_MODE_INFO_LEN)
    {
        lteWarning("Invalid Tag Length %u for TX_MODE_INFO   Info\n"
                ,tagLen);
        return MAC_SYNTAX_ERROR;
    }    
    transmissionMode = *msg_p++;
    if (MAC_SUCCESS != 
            validateTransmissionMode(transmissionMode))
    {
        lteWarning("Invalid value for transmissionMode[%u] .\n",
                transmissionMode);
        return MAC_SYNTAX_ERROR;
    }
    dlScellInfo_p->transmissionMode = transmissionMode; 
    ulScellInfo_p->transmissionMode = transmissionMode; 
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}
#if 0
/****************************************************************************
 * Function Name  : validateTransmissionMode
 * Inputs         : transmissionMode - tells the mode of transmission .
 *                  cellIndex - cell index used at UE-MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes transmissionMode to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

STATIC inline MacRetType validateTransmissionMode(UInt8 transmissionMode)
   
{
    if ((transmissionMode < MAC_MIN_TRANSMISSION_MODE) || 
            (transmissionMode > MAC_MAX_TRANSMISSION_MODE))
    {
        lteWarning("Invalid range for transmission mode.\n");
        return MAC_FAILURE;
    }

    return MAC_SUCCESS;
}
#endif
/****************************************************************************
 *  Function Name  : parseScellCodeBookSubsetRestV1020
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *                   ulScellInfo_p - pointer to uplink Scell info
 *                   cellIndex - the cell index used at UE-MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CODEBOOK_SUBSET_RESTRICTION_V1020
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellCodeBookSubsetRestV1020(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p
        )
{
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    UInt8 loopCount = 0;
    codebookSubsetRestrictionType  CbsrType = CBSR_INVALID;
    if (tagLen != CODEBOOK_SUBSET_RESTRICTION_V1020_TAG_LEN)
    {
        lteWarning("Invalid Tag Length %u for CODEBOOK_SUBSET_RESTRICTION_V1020 \
						Info\n", tagLen);
        return MAC_SYNTAX_ERROR;
    }    
    CbsrType =  (codebookSubsetRestrictionType)*msg_p++;
    if(CBSR_TYPE_MIN_VAL < CbsrType || CbsrType > CBSR_TYPE_MAX_VAL)
    {
        lteWarning("Invalid value for CbsrType[%u] .\n",
                CbsrType);
        return MAC_SYNTAX_ERROR;

    }
    dlScellInfo_p->codebookSubsetRestrictionv1020.cbsrType = CbsrType;
    ulScellInfo_p->codebookSubsetRestrictionv1020.cbsrType = CbsrType;
    for(loopCount =0;loopCount < MAX_CBSR_VALUE_COUNT;loopCount++)
    {
        dlScellInfo_p->codebookSubsetRestrictionv1020.cbsrValue[loopCount] = 
            *msg_p++;
        ulScellInfo_p->codebookSubsetRestrictionv1020.cbsrValue[loopCount] = 
    /* + SPR 11213 Changes */
		dlScellInfo_p->codebookSubsetRestrictionv1020.cbsrValue[loopCount] ;
    /* + SPR 11213 Changes */
    }
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseScellPDSCHConfigInfo
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *                   ulScellInfo_p - pointer to uplink Scell info
 *                   cellIndex - the cell index used at UE-MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellPDSCHConfigInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 cellIndex
        ) 
{
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    UInt8 pA = 0;
    if(tagLen != (PDSCH_CONFIG_INFO_PARAM_LENGTH + TAG_LEN))
    {
        lteWarning("Invalid Tag Length %u for PDSCH_CONFIG_INFO info\n"
                ,tagLen);
        return MAC_SYNTAX_ERROR;
    }
    pA = *msg_p++;
    if(MAX_PA_VALUE < pA)
    {
        lteWarning("Invalid Value of pA = %d received\n", pA);
        return MAC_FAILURE;
    }
    dlScellInfo_p->pA = pA;
    ulScellInfo_p->pA = pA;
    
    *msgBuf_pp = msg_p;
	return MAC_SUCCESS;
}



/* Scell Periodic Cqi Changes Start */
/****************************************************************************
 *  Function Name  : configureCAReconfigUECqiPeriodicInfo
 *                   dlUEInfo_p - pointer to Downlink Reconfig Cqi structure
 *                   ulUEInfo_p - pointer to Uplink Reconfig Cqi structure
 *  Outputs        : None
 *  Returns        : None
 *  Description    : This function parses the input buffer for CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/

void configureCAReconfigUECqiPeriodicInfo(
        ueContextForDownlink *dlUEInfo_p,
        ueContextForUplink   *ulUEContext_p,
        UInt8 scellCount
        )
{
    DLCQIInfo *scellUlCQIInfo = PNULL;
    CqiInfo *cqiInfoUl_p=NULL;
    ScellInfo *dlScellInfo_p = PNULL;
    DLUEContext *dlUEContext_p = PNULL;
    UInt32 ueIndex = dlUEInfo_p->ueIndex;
    dlUEContext_p = dlUECtxInfoArr_g[ueIndex].dlUEContext_p;
    UInt8 cellIndex = 0;
    ueContextForUplink   *ulUEInfo_p = PNULL;
    ulUEInfo_p = uplinkContextInfoMap_g[ueIndex]; 
    cellIndex      = ulUEContext_p->carrierAggrConfig.scellInfo[scellCount].sCellIndex;
    scellUlCQIInfo = &ulUEInfo_p->carrierAggrConfig.scellInfo[scellCount].dlCQIInfo;
    dlScellInfo_p  = &dlUEInfo_p->carrierAggrConfig.scellInfo[scellCount];
    /* SPR 12465 Fix  start */
    //cqiInfoUl_p    = &ulUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo;
    cqiInfoUl_p = &ulUEContext_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo;
    /* SPR 12465 Fix  end */

    if ( (cqiInfoUl_p->cqiformatIndicatorPeriodic == 0) && 
            ((1 == dlScellInfo_p->transmissionMode) ||
             (2 == dlScellInfo_p->transmissionMode) ||
             (3 == dlScellInfo_p->transmissionMode) ||      
             /* + TM7_8 Changes Start */
             (7 == dlScellInfo_p->transmissionMode) ||
             ((8 == dlScellInfo_p->transmissionMode) &&
              (TRUE != cqiInfoUl_p->pmiRiReportV920))))      
        /* - TM7_8 Changes End */
    {
        scellUlCQIInfo->periodicModeType = CQI_PERIODIC_TYPE_10;
        scellUlCQIInfo->periodicCQIMode10_p = &cqiPeriodicReportMode10_g[cellIndex][0];
        scellUlCQIInfo->periodicCQIMode10Abs_p = &cqiPeriodicReportMode10_g_2[cellIndex][0];
        scellUlCQIInfo->periodicModeType = CQI_PERIODIC_TYPE_10;
    }
    else if ( (cqiInfoUl_p->cqiformatIndicatorPeriodic == 0) &&
            ((4 == dlScellInfo_p->transmissionMode) ||
             /* + TM6_5.2 */
             (6 == dlScellInfo_p->transmissionMode)||         
             /* - TM6_5.2 */
             /* + TM7_8 Changes Start */
             ((8 == dlScellInfo_p->transmissionMode) &&         
              (TRUE == cqiInfoUl_p->pmiRiReportV920))))
        /* - TM7_8 Changes End */
    {
        scellUlCQIInfo->periodicModeType = CQI_PERIODIC_TYPE_11;
        scellUlCQIInfo->periodicCQIMode11_p = &cqiPeriodicReportMode11_g[cellIndex][0];
       scellUlCQIInfo->periodicCQIMode11Abs_p = &cqiPeriodicReportMode11_g[cellIndex][0];
        scellUlCQIInfo->periodicModeType = CQI_PERIODIC_TYPE_11;
    }
    else if ( (cqiInfoUl_p->cqiformatIndicatorPeriodic == 1) && 
            ((1 == dlScellInfo_p->transmissionMode) ||
             (2 == dlScellInfo_p->transmissionMode) ||
             (3 == dlScellInfo_p->transmissionMode) ||        
             /* + TM7_8 Changes Start */
             (7 == dlScellInfo_p->transmissionMode) ||      
             ((8 == dlScellInfo_p->transmissionMode) &&
              (TRUE != dlUEContext_p->isPmiRiConfigured_V920))))      
        /* - TM7_8 Changes End */
    {
        scellUlCQIInfo->periodicModeType = CQI_PERIODIC_TYPE_20;
        scellUlCQIInfo->periodicCQIMode20_p = &cqiPeriodicReportMode20_g[cellIndex][0];
        scellUlCQIInfo->periodicCQIMode20Abs_p = &cqiPeriodicReportMode20_g[cellIndex][0];
        /* Bandwidth Fix */
        scellUlCQIInfo->calculateBandwidthPartFlag = TRUE;
        /* Bandwidth Fix */
        scellUlCQIInfo->periodicModeType = CQI_PERIODIC_TYPE_20;
    }   
    else if ( (cqiInfoUl_p->cqiformatIndicatorPeriodic == 1) &&
            ((4 == dlScellInfo_p->transmissionMode) ||
             /* + TM6_5.2 */
             (6 == dlScellInfo_p->transmissionMode)|| 
             /* - TM6_5.2 */
             /* + TM7_8 Changes Start */
             ((8 == dlScellInfo_p->transmissionMode)&&
              (TRUE == dlUEContext_p->isPmiRiConfigured_V920))))
        /* - TM7_8 Changes End */
    {
        scellUlCQIInfo->periodicModeType = CQI_PERIODIC_TYPE_21;
        scellUlCQIInfo->periodicCQIMode21_p = &cqiPeriodicReportMode21_g[cellIndex][0];
        scellUlCQIInfo->periodicCQIMode21Abs_p = &cqiPeriodicReportMode21_g[cellIndex][0];
        /* Bandwidth Fix */
        scellUlCQIInfo->calculateBandwidthPartFlag = TRUE;
        /* Bandwidth Fix */
        scellUlCQIInfo->periodicModeType = CQI_PERIODIC_TYPE_21;
    }
    if(0 != dlScellInfo_p->cqiInfo.cqiMode)
    {    
        scellUlCQIInfo->aperiodicModeType = dlScellInfo_p->cqiInfo.cqiMode; 
    }

    if (CQI_MODE_1_2 == scellUlCQIInfo->aperiodicModeType)  
    {
        scellUlCQIInfo->aperiodicCQIMode12_p = &cqiAperiodicReportMode12_g[cellIndex][0];
        scellUlCQIInfo->aperiodicCQIMode12Abs_p = &cqiAperiodicReportMode12_g[cellIndex][0];
    }    
    else if (CQI_MODE_2_0 == scellUlCQIInfo->aperiodicModeType) 
    {
        scellUlCQIInfo->aperiodicCQIMode20_p = &cqiAperiodicReportMode20_g[cellIndex][0];
        scellUlCQIInfo->aperiodicCQIMode20Abs_p = &cqiAperiodicReportMode20_g[cellIndex][0];
    }    
    else if (CQI_MODE_2_2 == scellUlCQIInfo->aperiodicModeType)  
    {
        scellUlCQIInfo->aperiodicCQIMode22_p = &cqiAperiodicReportMode22_g[cellIndex][0];
        scellUlCQIInfo->aperiodicCQIMode22Abs_p = &cqiAperiodicReportMode22_g[cellIndex][0];
    }    
    else if (CQI_MODE_3_0 == scellUlCQIInfo->aperiodicModeType)  
    {
        scellUlCQIInfo->aperiodicCQIMode30_p = &cqiAperiodicReportMode30_g[cellIndex][0];
        scellUlCQIInfo->aperiodicCQIMode30Abs_p = &cqiAperiodicReportMode30_g[cellIndex][0];
    }    
    else if (CQI_MODE_3_1 == scellUlCQIInfo->aperiodicModeType)  
    {
        scellUlCQIInfo->aperiodicCQIMode31_p = &cqiAperiodicReportMode31_g[cellIndex][0];
        scellUlCQIInfo->aperiodicCQIMode31Abs_p = &cqiAperiodicReportMode31_g[cellIndex][0];
    }    
    /* SPR 14223 Fix : Start */
    //scellUlCQIInfo->ueCqiProfileNum = \
                                      ueProfileInfo_g[cellIndex][(ueIndex%MAX_UE_PROFILES)].dlCqiProfileId;
    /* SPR 14223 Fix : End */
    /* - CQI_4.1 */

    /* + CQI_5.0 */
    /* + SPR 13126 Changes */
    scellUlCQIInfo->riValue_p = &RIValues_g[cellIndex][0];
    scellUlCQIInfo->riValueAbs_p = &RIValues_g_2[cellIndex][0]; /* EICIC +-*/
    /* - SPR 13126 Changes */
    scellUlCQIInfo->isULSCHPDUType[0] = 0;
    /* SPR 12465 fix start */
    scellUlCQIInfo->ueCqiIndexNum = 0;
    scellUlCQIInfo->ueRIIndexNum = 0;
    /* SPR 12465 fix end */
    /* + SPR_11077 */
    if(( scellUlCQIInfo->periodicModeType == CQI_PERIODIC_TYPE_10) ||  
            ( scellUlCQIInfo->periodicModeType == CQI_PERIODIC_TYPE_11) || 
            ( scellUlCQIInfo->periodicModeType == CQI_PERIODIC_TYPE_20) || 
            ( scellUlCQIInfo->periodicModeType == CQI_PERIODIC_TYPE_21) ) 
        /* - SPR_11077 */
    {   
        scellUlCQIInfo->RI = dlScellInfo_p->numOfLayer;
        scellUlCQIInfo->RI = 2;
    } 
    /* - CQI_5.0 */
    /* + TM6_5.2 */
    if((6 == dlScellInfo_p->transmissionMode ) &&
            (FALSE == dlUEContext_p->dlCQIInfo.cqiInfoFlagInCreate))
    {
        scellUlCQIInfo->RI = 1;
    }
    /* - TM6_5.2 */

}
/* Scell Periodic Cqi Changes End */


/****************************************************************************
 *  Function Name  : parseCAReconfigUECqiInfo
 *  Inputs         : msgBuf_pp - pointer to input buffer
 *                   tagLen - pointer to the length remaining of the API message received
 *                   scellCount - SCELL number for which data is going to be parsed
 *                   dlUEInfo_p - pointer to Downlink Reconfig Cqi structure
 *                   ulUEInfo_p - pointer to Uplink Reconfig Cqi structure
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseCAReconfigUECqiInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        UInt8  scellCount,
        ueContextForDownlink *dlUEInfo_p,
        ueContextForUplink   *ulUEInfo_p,
        UInt8 cellIndex
        ) 
{
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    UInt8 aperiodicCQIEnabled=0,periodicCQIEnabled=0;
    UInt8 tag = 0;

    MacRetType resp = MAC_SUCCESS;
    CqiInfo *cqiInfoUl_p=NULL,*cqiInfoDl_p=NULL;
    cqiInfoUl_p = &ulUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo;	
    cqiInfoDl_p = &dlUEInfo_p->carrierAggrConfig.scellInfo[scellCount].cqiInfo;	
    /* - SPR 11213 Changes */
    if ( tagLen < 4 ) 
    /* - SPR 11213 Changes */
    {
        lteWarning("Invalid Tag Length %u for CQI_INFO received\n", tagLen);
        return MAC_FAILURE;
    }
    /*tagLen +4*/
    tag = LTE_GET_U16BIT(msg_p);
    ulUEInfo_p->cqiinfoflag = TRUE;
   // dlUEInfo_p->cqiInfoFlag = TRUE;
    
	if ( CQI_APERIODIC_INFO == tag )
    {
        resp = parseCAReconfigUECqiAperiodicInfo(
                    &msg_p,cqiInfoDl_p,cqiInfoUl_p,
                    dlUEInfo_p->carrierAggrConfig. \
                    scellInfo[scellCount].transmissionMode,
/* Aperiodic CQI Changes Start */
                    &aperiodicCQIEnabled,cellIndex, 
                    scellCount,
                    dlUEInfo_p
/* Aperiodic CQI Changes End */
        );
        if ( resp != MAC_SUCCESS )
        {
            return resp;
        }
        tag = LTE_GET_U16BIT(msg_p);
        /* if Periodic CQI Info is present */
    }/* if ( CQI_APERIODIC_INFO == tag )*/
    if ( CQI_PERIODIC_INFO == tag )
    {
        resp = parseCAReconfigUECqiPeriodicInfo(
                &msg_p,cqiInfoDl_p,cqiInfoUl_p,
                dlUEInfo_p->carrierAggrConfig. \
                scellInfo[scellCount].transmissionMode,
                &periodicCQIEnabled,cellIndex
                );
        if ( resp != MAC_SUCCESS )
        {
            return resp;
        }
        else
        {
            configureCAReconfigUECqiPeriodicInfo(dlUEInfo_p, ulUEInfo_p,scellCount);
        }
/* Scell Periodic Cqi Changes End */
        tag = LTE_GET_U16BIT(msg_p);
    }/*if ( CQI_PERIODIC_INFO == tag )*/

    if ( (periodicCQIEnabled == TRUE) && (aperiodicCQIEnabled == TRUE) )
    {
        cqiInfoDl_p->cqiIndication = PERIODIC_APERIODIC_MODE;
    }
    else if (periodicCQIEnabled == TRUE)
    {
        cqiInfoDl_p->cqiIndication = PERIODIC_CQI_REPORTING_MODE;
    }
    else if  (aperiodicCQIEnabled == TRUE)
    {
        cqiInfoDl_p->cqiIndication = APERIODIC_CQI_REPORTING_MODE;
    }
    /* - CQI_4.1 */
    if (CQI_REPORT_CONFIG_V920 == tag)
    {
        UInt16 cqiTagLenV920 = 0;
        /* skip CQI_REPORT_CONFIG_V920 tag */
        msg_p += U16BIT_LEN;

        /* cqi report v920 tag length*/
        cqiTagLenV920 = LTE_GET_U16BIT(msg_p);
        msg_p += U16BIT_LEN;

        if ( CQI_REPORT_V920_LEN > cqiTagLenV920 )
        {
            lteWarning("Invalid CQI_REPORT_CONFIG_V920_INVALID_TAGLEN recieved\n");
            return MAC_FAILURE;
        }
        /*tagLen +4  */
        tag = LTE_GET_U16BIT(msg_p);
        if (CQI_MASK_V920 == tag)
        {
            /* skip CQI_MASK_V920 tag */
            msg_p += U16BIT_LEN;
            //Skip the tag length
            msg_p += U16BIT_LEN;
            cqiInfoUl_p->cqiMaskV920 = *msg_p++;
            if (cqiInfoUl_p->cqiMaskV920 != TRUE &&
                    cqiInfoUl_p->cqiMaskV920 != FALSE )
            {
                lteWarning("Invalid CQI_MASK_V920_INVALID_VALUE received\n");
                return MAC_FAILURE;
            }
            tag = LTE_GET_U16BIT(msg_p);
            /*tagLen +5  */
        }
        if (PMI_RI_REPORT_V920 == tag)
        {
            /* skip PMI_RI_REPORT_V920 tag */
            msg_p += U16BIT_LEN;
            //Skip the tag length
            msg_p += U16BIT_LEN;
            cqiInfoUl_p->pmiRiReportV920 = *msg_p++;
            if (cqiInfoUl_p->pmiRiReportV920 != TRUE &&
                    cqiInfoUl_p->pmiRiReportV920 != FALSE )
            {
                lteWarning("Invalid Value PMI_RI_REPORT_V920_INVALID_VALUE recived\n");
                return MAC_FAILURE;
            }
            cqiInfoDl_p->pmiRiReportV920 = 
                cqiInfoUl_p->pmiRiReportV920;
            /*tagLen +5 */
        }


    }
    *msgBuf_pp = msg_p;
    return resp;
}

/****************************************************************************
 *  Function Name  : parseCAReconfigUECqiAperiodicInfo
 *  Inputs         : msgBuf_pp - input message buffer
 *                   cqiInfoDl_p - pointer to downlink CQI info structure
 *                   cqiInfoUl_p - pointer to uplink CQI info structure
 *                   transmission mode - CQI transmission mode
 *                   cellIndex - the cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseCAReconfigUECqiAperiodicInfo(
        UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoDl_p,
        CqiInfo *cqiInfoUl_p, 
        UInt8 transmissionMode,
        UInt8 *aperiodicCQIEnabled,
/* Aperiodic CQI Changes Start */
        UInt8 cellIndex,
        UInt8 scellCount,
        ueContextForDownlink *dlUEInfo_p
        )
{
    DLCQIInfo *scellDlCQIInfo = PNULL;
/* Aperiodic CQI Changes End */
/*
    CellConfigParams *cellParams_p = cellSpecificParams_g.\
        cellConfigAndInitParams_p[internalCellIndex]->cellParams_p;
		*/
    UInt8 tag = 0; 
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    MacRetType resp = MAC_SUCCESS;
/* SPR 12135 Fix Start */
    UInt8 scellIndex = 255;
     scellIndex = dlUEInfo_p->carrierAggrConfig.scellInfo[scellCount].sCellIndex; 
/* SPR 12135 Fix End */
    /* Bandwidth Fix */
 /*   if (DL_NUM_RB > cellParams_p->dlAvailableRBs)
    {
        lteWarning("Aperiodic mode configured with DL "
                "RBs are less than 8\n");
    } */
    *aperiodicCQIEnabled = TRUE;
    /* Bandwidth Fix */
    /* skip CQI_APERIODIC_INFO tag */
    msg_p += U16BIT_LEN;
    /*skip the length of the CQI Aperiodic Info tag*/
    msg_p += U16BIT_LEN;

    cqiInfoUl_p->cqiMode = (CQIMode)*msg_p++;
    cqiInfoDl_p->cqiMode = cqiInfoUl_p->cqiMode;

    /* This function validates the CQI mode for corresponding TM modes */
    if (MAC_FAILURE == 
            (validateAperiodicModeForTXMode(cqiInfoDl_p->cqiMode,
                                            transmissionMode)))

    {
        lteWarning("TX mode [%d] is not compatible with CQI aperiodic mode [%d]\n",
                transmissionMode,cqiInfoDl_p->cqiMode);
        return MAC_FAILURE;
    }
    /* SPR 12135 Fix Start */
    ueContextForUplink   *ulUEInfo_p = PNULL;
    ulUEInfo_p = uplinkContextInfoMap_g[dlUEInfo_p->ueIndex];



/* Aperiodic CQI Changes Start */
    scellDlCQIInfo = &ulUEInfo_p->carrierAggrConfig.scellInfo[scellCount].dlCQIInfo;
  
  

    if(0 != cqiInfoDl_p->cqiMode)
    {    
        scellDlCQIInfo->aperiodicModeType = cqiInfoDl_p->cqiMode; 
    }

    if (CQI_MODE_1_2 == scellDlCQIInfo->aperiodicModeType)  
    {
        scellDlCQIInfo->aperiodicCQIMode12_p = &cqiAperiodicReportMode12_g[scellIndex][0];
        scellDlCQIInfo->aperiodicCQIMode12Abs_p = &cqiAperiodicReportMode12_g[scellIndex][0];
    }    
    else if (CQI_MODE_2_0 == scellDlCQIInfo->aperiodicModeType) 
    {
        scellDlCQIInfo->aperiodicCQIMode20_p = &cqiAperiodicReportMode20_g[scellIndex][0];
        scellDlCQIInfo->aperiodicCQIMode20Abs_p = &cqiAperiodicReportMode20_g[scellIndex][0];
    }    
    else if (CQI_MODE_2_2 == scellDlCQIInfo->aperiodicModeType)  
    {
        scellDlCQIInfo->aperiodicCQIMode22_p = &cqiAperiodicReportMode22_g[scellIndex][0];
          scellDlCQIInfo->aperiodicCQIMode22Abs_p = &cqiAperiodicReportMode22_g[scellIndex][0];
    }    
    else if (CQI_MODE_3_0 == scellDlCQIInfo->aperiodicModeType)  
    {
        scellDlCQIInfo->aperiodicCQIMode30_p = &cqiAperiodicReportMode30_g[scellIndex][0];
        scellDlCQIInfo->aperiodicCQIMode30Abs_p = &cqiAperiodicReportMode30_g[scellIndex][0];
    }    
    else if (CQI_MODE_3_1 == scellDlCQIInfo->aperiodicModeType)  
    {
        scellDlCQIInfo->aperiodicCQIMode31_p = &cqiAperiodicReportMode31_g[scellIndex][0];
         scellDlCQIInfo->aperiodicCQIMode31Abs_p = &cqiAperiodicReportMode31_g[scellIndex][0];
    }
    /* + SPR 13126 Changes */
    scellDlCQIInfo->riValue_p = &RIValues_g[scellIndex][0];
    scellDlCQIInfo->riValueAbs_p = &RIValues_g_2[scellIndex][0];
    /* - SPR 13126 Changes */
    getLAndNValuesScell(dlUEInfo_p->ueIndex, dlUEInfo_p->carrierAggrConfig.
                                          scellInfo[scellCount].sCellIndex); 
    
    /* SPR 14223 Fix : Start */
    // scellDlCQIInfo->ueCqiProfileNum =\
		//ueProfileInfo_g[cellIndex][(dlUEInfo_p->ueIndex%MAX_UE_PROFILES)].dlCqiProfileId;
        /* SPR 12298 Fix Start */
    //   ueProfileInfo_g[scellIndex][ueProfileIndex_g[scellIndex]%MAX_UE_PROFILES].dlCqiProfileId;
    //ueProfileIndex_g[scellIndex]++;
    /* SPR 14223 Fix : End */
        /* SPR 12298 Fix End */
   scellDlCQIInfo->isULSCHPDUType[0] = 0;
    scellDlCQIInfo->ueRIIndexNum = 0;

    scellDlCQIInfo->ueCqiIndexNum = 0;
 /* SPR 12135 Fix End */
    
/* Aperiodic CQI Changes End */
    /*tagLen +5*/

    tag = LTE_GET_U16BIT(msg_p);
    if (APERIODIC_CSI_TRIGGER_R10 == tag)
    {
        lteWarning("APERIODIC_CSI_TRIGGER_R10 tag is not suppoerted by scell\n");
        return MAC_FAILURE;
    }

    *msgBuf_pp = msg_p;

    return resp;
}


/****************************************************************************
 * Function Name  : validateAperiodicModeForTXMode 
 * Inputs         : aperiodicCqiMode - configured Aperiodic Mode
 *                  transmissionMode - configured transmission mode
 *                    
 * Output         : None
 * Returns        : MAC_SUCCESS or MAC_FAILURE
 * Description    : This function will validate the aperiodic CQI mode against 
 *                  transmission mode. If transmission mode is Closed loop MIMO 
 *                  and  aperiodic mode 20/ aperiodic 30 is configured then
 *                  return failure or if transmission mode is SISO/ transmit
 *                  diversity/Open loop MIMO and aperiodic 12/aperiodic 22/
 *                  aperiodic 31 is configured then return failure.
 *****************************************************************************/
MacRetType validateAperiodicModeForTXMode(UInt8 aperiodicCQIMode,
                                          UInt8 transmissionMode)
{

    switch (transmissionMode)
    {
        case TX_MODE_1:
        case TX_MODE_2:
        case TX_MODE_3:
        /* + TM7_8 Changes Start */
        case TX_MODE_7:     
        /* - TM7_8 Changes End */
             if( (CQI_MODE_1_2 ==  aperiodicCQIMode) || 
                 (CQI_MODE_2_2 == aperiodicCQIMode) ||
                 (CQI_MODE_3_1 == aperiodicCQIMode) )
             {
                return MAC_FAILURE;
             }    
        break;

        case TX_MODE_4:
        /* + TM6_5.2 */
        case TX_MODE_6:
        /* - TM6_5.2 */
             if( (CQI_MODE_2_0 == aperiodicCQIMode) || (CQI_MODE_3_0 == aperiodicCQIMode) )
             {
                return MAC_FAILURE;
             }    
        break;     
        /* + TM7_8 Changes Start */
        case TX_MODE_8:
        /* CQI Mode for TM8  depends upon PmiRiConfig,
         * so its validation will take place at createUE */
        break;
        /* - TM7_8 Changes End */

        default:
           lteWarning("Unsupported/Invalid transmissionMode:%d\n", transmissionMode);
           return MAC_FAILURE;
        break;
    }

    return MAC_SUCCESS;

}    
/****************************************************************************
 *  Function Name  : parseCAReconfigUECqiPeriodicInfo
 *  Inputs         : msgBuf_pp - pointer to input buffer
 *                   cqiInfoDl_p - pointer to downlink CQI info structure
 *                   cqiInfoUl_p - pointer to uplink CQI info structure
 *                   transmission mode - CQI transmission Mode
 *                   periodicCQIEnabled_p - check to know validity of periodicCQI
 *                   cellIndex - the cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for periodic CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseCAReconfigUECqiPeriodicInfo(
        UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoDl_p,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 *periodicCQIEnabled_p,
        UInt8 cellIndex
        )
{
    UInt8 tag = 0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    MacRetType resp = MAC_SUCCESS;
    UInt32 cqiPeriodicTagLen = 0;
    *periodicCQIEnabled_p = TRUE;
    /* skip the CQI_PERIODIC_INFO tag */
    msg_p += U16BIT_LEN;

    /* cqi periodic tag length*/
    cqiPeriodicTagLen = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;

    cqiInfoUl_p->cqiRequestType =  *(msg_p);
    msg_p++;
    /* + SPR 5953 Fix */
    cqiInfoDl_p->cqiRequestType =  
        cqiInfoUl_p->cqiRequestType;
    /* - SPR 5953 Fix */
    if (  cqiInfoUl_p->cqiRequestType > SETUP)
    {
        lteWarning("Invalid CQI Request Type");
        return MAC_FAILURE;
    }
    if ( cqiInfoUl_p->cqiRequestType )
    {
        if ( cqiPeriodicTagLen < PERIODIC_CQI_SETUP_LEN)
        {
            lteWarning("cqiPeriodicTagLen < PERIODIC_CQI_SETUP_LEN");
            return MAC_FAILURE;
        }
        tag = LTE_GET_U16BIT(msg_p);

        if ( CQI_PMI_CONFIG_INDEX_INFO == tag)
        {
    /* + SPR 11213 Changes */
            msg_p += U16BIT_LEN; 
    /* - SPR 11213 Changes */
            resp = parseCAReconfigUECqiPMIConfigIndex(
                        &msg_p,cqiInfoUl_p,transmissionMode,
                        cellIndex
                        );
            if(MAC_SUCCESS != resp)
            {
                return resp;
            }
        }/*if ( CQI_PMI_CONFIG_INDEX_INFO == tag)*/
    /* + SPR 11213 Changes */
            tag = LTE_GET_U16BIT(msg_p);
    /* - SPR 11213 Changes */
        if ( CQI_PMI_CONFIG_INDEX_INFO_v1020 == tag)
        {
    /* + SPR 11213 Changes */
            msg_p += U16BIT_LEN; 
    /* - SPR 11213 Changes */
            resp = parseCAReconfigUECqiPMIIndexV1020(
                        &msg_p,cqiInfoUl_p,transmissionMode,
                        cellIndex
            );
            if(MAC_SUCCESS != resp)
            {
                return resp;
            }
        }/*if ( CQI_PMI_CONFIG_INDEX_INFO_v1020 == tag)*/

    }/* if (  ulLCCreateReq_p->scellInfo[scellIndex].cqiRequestType )*/
    *msgBuf_pp = msg_p;
    return resp;
}


/****************************************************************************
 *  Function Name  : parseCAReconfigUECqiPMIConfigIndex
 *  Inputs         : msgBuf_pp - pointer to input buffer
 *                   cqiInfoUl_p - pointer to uplink CQI info structure
 *                   transmission Mode
 *                   internalCellIndex - cell index used at MAC
 *                   dlReconfigUeReq_p - pointer to Downlink reconfig UE structure
 *                   ulReconfigUeReq_p - pointer to Uplink reconfig UE structure
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for PMI Config Index
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseCAReconfigUECqiPMIConfigIndex(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 cellIndex
)
{
    UInt8 tag=0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    MacRetType resp = MAC_SUCCESS;
    UInt32 cqiPeriodicTagLen = 0;
    CellConfig *cellParams_p = &cellConfigUeSim_g[cellIndex];
                                     
									 
    /* skip the CQI_PMI_CONFIG_INDEX_INFO tag */
    cqiPeriodicTagLen = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;
    if ( cqiPeriodicTagLen <  PERIODIC_CQI_LEN )
    {
        lteWarning("cqiPeriodicTagLen < PERIODIC_CQI_LEN");
        return MAC_FAILURE;
    }
    cqiInfoUl_p->cqiPMIConfigIndex = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;

#ifdef FDD_CONFIG
    if (cqiInfoUl_p->cqiPMIConfigIndex > MAC_MAX_CQI_PMI_CONFIG_INDEX ||
            cqiInfoUl_p->cqiPMIConfigIndex == MAC_MAX_CQI_PMI_CONFIG_INDEX_3_1_7  )
   {
        lteWarning("Invalid range for cqiPMIConfigIndex .\n");
        return MAC_FAILURE;
    }
#endif
    cqiInfoUl_p->cqiPucchResourceIndex = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;
    if (cqiInfoUl_p->cqiPucchResourceIndex > 
            MAC_MAX_CQI_PUCCH_RESOURCE_INDEX)
    {
        lteWarning("Invalid range for cqiPucchResourceIndex .\n");
        return MAC_FAILURE;
    }
    cqiInfoUl_p->simultaneousACKNACKAndCQI = (*msg_p);
    if ((cqiInfoUl_p->simultaneousACKNACKAndCQI != SIMULTANEOUS_ACK_NACK_CQI_0)
            && (cqiInfoUl_p->simultaneousACKNACKAndCQI != 
             SIMULTANEOUS_ACK_NACK_CQI_1))
    {
        lteWarning("Invalid range for simultaneousACKNACKAndCQI .\n");
        return MAC_FAILURE;
    }
    msg_p++;
    cqiInfoUl_p->cqiformatIndicatorPeriodic = (PeriodicFeedbackType)*msg_p;
    msg_p++;
    if(MAC_SUCCESS != validateCqiformatIndicatorPeriodic(
                cqiInfoUl_p->cqiformatIndicatorPeriodic))
    {
        lteWarning("Invalid range for cqiformatIndicatorPeriodic:%d\n",
                cqiInfoUl_p->cqiformatIndicatorPeriodic);

        return MAC_FAILURE;
    }

    tag = LTE_GET_U16BIT(msg_p);
    /*tagLen + 5  */
    /*tagLen + 10  */
    if(CQI_FORMAT_PERIODIC_SUBBAND_K == tag)
    {
            resp = parseCAUECqiFormatPeriodicSubbandK(
                        &msg_p,cqiInfoUl_p,
                        cellParams_p,
                        cellIndex
                        );
            if(MAC_SUCCESS != resp)
            {
                return resp;
            }
    }
    else
    {
        if(cqiInfoUl_p->cqiformatIndicatorPeriodic)
        {
            return MAC_FAILURE;
        }
    }
    tag = LTE_GET_U16BIT(msg_p);
    /*tagLen +5  */
    if (RI_CONFIG_INDEX_INFO == tag)
    {
        resp = parseCAUECqiRIConfigIndex(
                &msg_p,cqiInfoUl_p,transmissionMode,
                cellIndex
                );
        if(MAC_SUCCESS != resp)
        {
            return resp;
        }
    }
    tag = LTE_GET_U16BIT(msg_p);
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseCAUECqiFormatPeriodicSubbandK
 *  Inputs         : msgBuf_pp - pointer to input buffer
 *                   cqiInfoUl_p - pointer to uplink CQI info structure
 *                   cellIndex - the cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for Periodic Sub bandK
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseCAUECqiFormatPeriodicSubbandK(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        CellConfig *cellParams_p,
        UInt8 cellIndex
        )
{
    UInt8 tagLen =0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    if (DL_NUM_RB > cellParams_p->dlAvailableRBs)
    {
        lteWarning("Periodic Subband mode configured with DL" \ 
                "RBs are less than 8\n");
        return MAC_FAILURE;
    }
    /* - CQI_4.1 */
    msg_p += U16BIT_LEN;
    tagLen = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;
    if (tagLen < CQI_FORMAT_PERIODIC_SUBBAND_K_LEN)
    {
        lteWarning("Invalid tagLen for CQI_FORMAT_PERIODIC_SUBBAND_K %u received \
						\n",tagLen); 
        return MAC_FAILURE;
    }
    cqiInfoUl_p->cqiformatIndicatorPeriodic_subband_k = *msg_p;
    msg_p++;
    if(MAC_SUCCESS != validateCqiformatIndicatorPeriodicSubbandK(
                cqiInfoUl_p->cqiformatIndicatorPeriodic_subband_k))
    {
        lteWarning("Invalid range for" \ 
                "cqiformatIndicatorPeriodic_subband_k:%d\n", \
                cqiInfoUl_p->cqiformatIndicatorPeriodic_subband_k);

        return MAC_FAILURE;
    }
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}
/****************************************************************************
 *  * Function Name  : validateCqiformatIndicatorPeriodicSubbandK
 *  * Inputs         : cqiPeriodicSubbandK: cqi format Indicator for Periodic subband k
 *  * Outputs        : None
 *  * Returns        : MAC_SUCCESS or reason for failure.
 *  * Description    : This API takes cqiformatIndicatorPeriodic_subband_k  to validate and returns the result
 *  *                  of validation to the calling function.
 *  ***************************************************************************/
STATIC inline MacRetType validateCqiformatIndicatorPeriodicSubbandK (UInt8 cqiPeriodicSubbandK)
{

    if ((cqiPeriodicSubbandK > MAC_MAX_SUBBAND_CQI_K) ||
        (cqiPeriodicSubbandK < MAC_MIN_SUBBAND_CQI_K ))
    {
        lteWarning("Invalid range for CQI format Indicator Periodic subband k:%d\n", \
                 cqiPeriodicSubbandK);
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/****************************************************************************
 *  Function Name  : parseCAUECqiRIConfigIndex
 *  Inputs         : msgBuf_pp - pointer to input buffer
 *                   cqiInfoUl_p - pointer to uplink CQI info structure
 *                   transmission Mode
 *                   cellIndex - the cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for RI Config Index 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseCAUECqiRIConfigIndex(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 internalCellIndex
        )
{
    UInt8 len =0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;

    msg_p += U16BIT_LEN;
    len = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;

    if(len != RI_CONFIG_INDEX_INFO_LEN)
    {
        lteWarning("Invalid Length for riConfigIndex .\n");
        return MAC_FAILURE;
    }
    cqiInfoUl_p->riConfigIndex = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;
    /*tagLen +6 */
    if ((cqiInfoUl_p->riConfigIndex > MAC_MAX_RI_CONFIG_INDEX))
    {
        lteWarning("Invalid range for riConfigIndex .\n");
        return MAC_FAILURE;
    }

    else if( TX_MODE_3 != transmissionMode &&
            TX_MODE_4 != transmissionMode &&                         
            TX_MODE_5 != transmissionMode &&                        
            TX_MODE_6 != transmissionMode && 
            TX_MODE_8 != transmissionMode )                          
    {
        cqiInfoUl_p->riConfigIndex = MAC_RI_CONFIG_INDEX_INVALID;
        lteWarning("riConfigIndex is configured in TX mode other" \ 
                "than 3 and 4.\n");
    }
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}
/****************************************************************************
 *  Function Name  : parseCAReconfigUECqiPMIIndexV1020
 *  Inputs         : msgBuf_pp - pointer to input buffer
 *                   cqiInfoUl_p - pointer to uplink CQI info structure
 *                   transmission Mode
 *                   internalCellIndex - cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for PMI IndexV1020
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseCAReconfigUECqiPMIIndexV1020(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 cellIndex
        )
{
    UInt16 tag=0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    MacRetType resp = MAC_SUCCESS;
    UInt32 cqiPeriodicTagLen = 0;
    CellConfig *cellParams_p = &cellConfigUeSim_g[cellIndex];
    /* EICIC +*/
    UInt16 cqiconfigindexTagLenV10   = 0;
    UInt16 cqiconfigindex2TagLenV10   = 0;
    UInt16 riconfigindexTagLenV10   = 0;
    /* EICIC -*/
    
	/* skip the CQI_PMI_CONFIG_INDEX_INFO_v1020 tag */
     /*PARSING CORRECTION*/
    cqiPeriodicTagLen = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;

    if ( cqiPeriodicTagLen <  PERIODIC_CQI_LEN )
    {
        lteWarning("cqiPeriodicTagLen < PERIODIC_CQI_LEN");
        return MAC_FAILURE;
    }
    cqiInfoUl_p->cqiPMIConfigIndexV1020 = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;

    resp = validateCqiPMIConfigIndex(cqiInfoUl_p->cqiPMIConfigIndexV1020, 
        cellIndex);
    if (resp != MAC_SUCCESS)
    {
        lteWarning("Validation failed for cqiPMIConfigIndexV1020. Invalid value %d Received", \
					cqiInfoUl_p->cqiPMIConfigIndexV1020);
        return resp;

    }
    cqiInfoUl_p->cqiPucchResourceIndexV1020 = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;

    resp = validateCqiPucchResourceIndex(cqiInfoUl_p->cqiPucchResourceIndexV1020, cellIndex);
    if (resp != MAC_SUCCESS)
    {
        lteWarning("Invalid range for cqiPucchResourceIndexV1020i %d Recieved\n", \
							cqiInfoUl_p->cqiPucchResourceIndexV1020);
        return resp;
    }
    cqiInfoUl_p->simultaneousACKNACKAndCQIV1020 = *msg_p;
    msg_p++;
    resp = validateSimultaneousACKNACKAndCQI(
            cqiInfoUl_p->simultaneousACKNACKAndCQIV1020
            ,cellIndex
            );
    if ( resp != MAC_SUCCESS )
    {
        lteWarning("Invalid range for simultaneousACKNACKAndCQIV1020");
        return resp;
    }
    cqiInfoUl_p->cqiformatIndicatorPeriodicV1020 = (PeriodicFeedbackType)*msg_p;
    msg_p++;
    if(MAC_SUCCESS != validateCqiformatIndicatorPeriodic(cqiInfoUl_p->cqiformatIndicatorPeriodicV1020))
    {
        lteWarning("Invalid range for cqiformatIndicatorPeriodic:%d\n", \
                cqiInfoUl_p->cqiformatIndicatorPeriodicV1020);

        return MAC_FAILURE;
    }

    tag = LTE_GET_U16BIT(msg_p);
    /*tagLen + 10 */
    if(CQI_FORMAT_PERIODIC_SUBBAND_K == tag)
    {
            resp = parseCAUECqiFormatPeriodicSubbandKV1020(
                        &msg_p,cqiInfoUl_p,
                        cellParams_p,
                        cellIndex
                        );
            if(MAC_SUCCESS != resp)
            {
                return resp;
            }
    }
    else
    {
        if(cqiInfoUl_p->cqiformatIndicatorPeriodicV1020)
        {
            return MAC_FAILURE;
        }
    }
    tag = LTE_GET_U16BIT(msg_p);

    /*tagLen +5  effective 25*/
    if (RI_CONFIG_INDEX_INFO == tag)
    {
        resp = parseCAUECqiRIConfigIndexV1020(
                &msg_p,cqiInfoUl_p,transmissionMode,
                cellIndex
                );
        if(MAC_SUCCESS != resp)
        {
            return resp;
        }
            tag = LTE_GET_U16BIT(msg_p);
    }

  /* EICIC +*/
  
    msg_p += U16BIT_LEN;

    /* cqi report v920 tag length*/
    cqiconfigindexTagLenV10 = LTE_GET_U16BIT(msg_p);
    
    msg_p += 2;

    if (CSI_CONFIG_INDEX_VR10_TAG_LENGTH > cqiconfigindexTagLenV10)
    {      
         lteWarning("Invalid cqiconfigindexTagLenV10 ");
        MAC_EMULATOR_LOG(LOG_INFO,"cqiconfigindexTagLenV10 -> %d\n",cqiconfigindexTagLenV10 );               

        return MAC_FAILURE;
    }

     cqiInfoUl_p->cqiConfigRequestType = *(msg_p);
        msg_p++;
        /* + SPR 5953 Fix */
     cqiInfoUl_p->cqiConfigRequestType = cqiInfoUl_p->cqiRequestType;
        /* - SPR 5953 Fix */
        /*L2_FIX_8 */
        if ( cqiInfoUl_p->cqiConfigRequestType > SETUP)
        {
         lteWarning("Invalid cqiconfigindexTagLenV10 ");
        MAC_EMULATOR_LOG(LOG_INFO,"cqiconfigindexTagLenV10 -> %d\n",cqiconfigindexTagLenV10 );    
            /* Review comment fix end RJ13 */
            return MAC_FAILURE;
        }
        if ( cqiInfoUl_p->cqiConfigRequestType )
        {
                 tag = LTE_GET_U16BIT(msg_p);
                 if (tag == CQI_PMI_CONFIGINDEX2_VR10 )
                 {
                 msg_p += 2;
		   cqiconfigindex2TagLenV10 = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (CSI_CONFIG_INDEX_VR10_TAG_LENGTH > cqiconfigindex2TagLenV10)
                 {      
		         lteWarning("Invalid cqiconfigindex2TagLenV10 ");
	       	 MAC_EMULATOR_LOG(LOG_INFO,"cqiconfigindex2TagLenV10 -> %d\n",cqiconfigindex2TagLenV10 );  
        		return MAC_FAILURE;
                 }
		  cqiInfoUl_p->cqiPMIConfigIndex_2 = *(msg_p);
		  msg_p++;

		   tag = LTE_GET_U16BIT(msg_p);
                 }
                  if (tag == RI_CONFIG_INDEX_INFO)
                 {
                 msg_p += 2;
		   riconfigindexTagLenV10 = LTE_GET_U16BIT(msg_p);
                 msg_p += 2;
				 
	          if (RI_CONFIGINDEX2_VR10_TAG_LENGTH > riconfigindexTagLenV10)
                 {      
		    lteWarning("Invalid riconfigindexTagLenV10 ");
		        MAC_EMULATOR_LOG(LOG_INFO,"riconfigindexTagLenV10 -> %d\n",riconfigindexTagLenV10 );  
	        	return MAC_FAILURE;
                 }
		  cqiInfoUl_p->riConfigIndex_2 = *(msg_p);
		  msg_p++;
                 }
				 
        }
        
        /* EICIC -*/

    *msgBuf_pp = msg_p;
    return resp;
}

/****************************************************************************
 * Function Name  : validatecqiPMIConfigIndex
 * Inputs         : cqiPMIConfigIndex - Specifies the CQI/PMI periodicity 
                                        and configuration index. 
 *                  cellIndex - cell index used at MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes cqiPMIConfigIndex  to validate and returns 
 *                  the result of validation to the calling function.
 ****************************************************************************/

STATIC inline MacRetType validateCqiPMIConfigIndex(UInt32 cqiPMIConfigIndex
    /* CA changes Start */
    ,UInt8 cellIndex
    /* CA Changes end */
)
{
    /* SPR 1583 Start */
    #ifdef FDD_CONFIG
    if (cqiPMIConfigIndex > MAC_MAX_CQI_PMI_CONFIG_INDEX ||
        cqiPMIConfigIndex == MAC_MAX_CQI_PMI_CONFIG_INDEX_3_1_7  )
    {
        lteWarning("Invalid range for cqiPMIConfigIndex .\n");
        return MAC_FAILURE;
    }
    #elif TDD_CONFIG
    if (cqiPMIConfigIndex > MAC_MAX_CQI_PMI_CONFIG_INDEX_TDD )
    {
        lteWarning("Invalid range for cqiPMIConfigIndex .\n");
        return MAC_FAILURE;
    }
    #endif
    /* SPR 1583 End*/

    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validatecqiPucchResourceIndex
 * Inputs         : cqiPucchResourceIndex - Specifies the pucch resources for 
                    periodic CQI on uplink.
 *                  cellIndex - cell index used at UE-MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes cqiPucchResourceIndex  to validate and 
                    returns the result  of validation to the calling function.
 ****************************************************************************/

STATIC inline MacRetType validateCqiPucchResourceIndex(UInt32 cqiPucchResourceIndex
    /* CA changes Start */
    ,UInt8 cellIndex
    /* CA Changes end */
)
{
	if (cqiPucchResourceIndex > MAC_MAX_CQI_PUCCH_RESOURCE_INDEX)
	{
		lteWarning("Invalid range for cqiPucchResourceIndex .\n");
		return MAC_FAILURE;
	}
	return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validatesimultaneousACKNACKAndCQI  
 * Inputs         : simultaneousACKNACKAndCQI - value of ACK/NACK with CQI 
 *                  cellIndex - cell index used at UE-MAC
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes simultaneousACKNACKAndCQI  to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

STATIC inline MacRetType validateSimultaneousACKNACKAndCQI(UInt8 simultaneousACKNACKAndCQI
        /* CA changes Start */
        ,UInt8 cellIndex
        /* CA Changes end */
        )
{ 
    if ((simultaneousACKNACKAndCQI != SIMULTANEOUS_ACK_NACK_CQI_0) && 
            (simultaneousACKNACKAndCQI != SIMULTANEOUS_ACK_NACK_CQI_1))
    {
        lteWarning("Invalid range for simultaneousACKNACKAndCQI .\n");
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : validateCqiformatIndicatorPeriodic
 * Inputs         : cqiformatIndicatorPeriodic - cqi format Indicator for Periodic 
 *                  subband/wideband
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for failure.
 * Description    : This API takes cqiformatIndicatorPeriodic  to validate and returns the result
 *                  of validation to the calling function.
 ****************************************************************************/

STATIC inline MacRetType validateCqiformatIndicatorPeriodic (UInt8 cqiformatIndicatorPeriodic)
{

    if ((cqiformatIndicatorPeriodic != PERIODIC_WIDEBAND) &&
        (cqiformatIndicatorPeriodic != PERIODIC_SUBBAND))
    {
        lteWarning("Invalid range for cqiformatIndicatorPeriodic:%d\n", cqiformatIndicatorPeriodic);
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseCAUECqiFormatPeriodicSubbandKV1020
 *  Inputs         : msgBuf_pp - pointer to input buffer
 *                   cqiInfoUl_p - pointer to uplink CQI info structure
 *                   cellParams_p -  pointer to cell parameters
 *                   cellIndex - the cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for Periodic Sub bandKV1020
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseCAUECqiFormatPeriodicSubbandKV1020(
        UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        CellConfig *cellParams_p,
        UInt8 internalCellIndex
        )
{
    UInt8 tagLen = 0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    if (DL_NUM_RB > cellParams_p->dlAvailableRBs)
    {
        lteWarning("Periodic Subband mode configured with DL" \ 
                "RBs are less than 8\n");
        return MAC_FAILURE;
    }
    /* - CQI_4.1 */
    msg_p += U16BIT_LEN;
    tagLen = LTE_GET_U16BIT(msg_p);
    msg_p += U16BIT_LEN;
    if (tagLen < CQI_FORMAT_PERIODIC_SUBBAND_K_LEN)
    {
        lteWarning(" Invalid length %u received for CQI_FORMAT_PERIODIC_SUBBAND_K\n",tagLen);
        return MAC_FAILURE;
    }
    cqiInfoUl_p->cqiformatIndicatorPeriodic_subband_kV1020 = *msg_p;
    msg_p++;
    if(MAC_SUCCESS != validateCqiformatIndicatorPeriodicSubbandK(cqiInfoUl_p->
        cqiformatIndicatorPeriodic_subband_kV1020))
    {
        lteWarning("Invalid range for cqiformatIndicatorPeriodic_subband_kV1020:%d\n", \
                cqiInfoUl_p->cqiformatIndicatorPeriodic_subband_kV1020);
        return MAC_FAILURE;
    }
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseCAUECqiRIConfigIndexV1020
 *  Inputs         : msgBuf_pp - pointer to input buffer
 *                   cqiInfoUl_p - pointer to uplink CQI info structure
 *                   transmissionMode
 *                   cellIndex - the cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for RI Config IndexV1020
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseCAUECqiRIConfigIndexV1020(UInt8 **msgBuf_pp,
        CqiInfo *cqiInfoUl_p,
        UInt8 transmissionMode,
        UInt8 cellIndex
        )
{
    UInt8 len =0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    msg_p +=U16BIT_LEN;
    len = LTE_GET_U16BIT(msg_p);
    msg_p +=U16BIT_LEN;

    if(len != RI_CONFIG_INDEX_INFO_LEN)
    {
        lteWarning("Invalid Length for riConfigIndex .\n");
        return MAC_FAILURE;
    }

    cqiInfoUl_p->riConfigIndexV1020 = LTE_GET_U16BIT(msg_p);
    msg_p +=U16BIT_LEN;
    /*tagLen +6  */
    if (cqiInfoUl_p->riConfigIndexV1020 > MAC_MAX_RI_CONFIG_INDEX)
    {
        lteWarning("Invalid range for riConfigIndexV1020.\n");
        return MAC_FAILURE;
    }

    else if( TX_MODE_3 != transmissionMode &&
            TX_MODE_4 != transmissionMode &&                         
            TX_MODE_5 != transmissionMode &&                        
            TX_MODE_6 != transmissionMode && 
            TX_MODE_8 != transmissionMode )                          
    {
        cqiInfoUl_p->riConfigIndexV1020 = MAC_RI_CONFIG_INDEX_INVALID;
        lteWarning("riConfigIndexV1020 is configured in TX mode other than 3 and 4.\n");
    }
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 * Function Name  : parseCALCBandwidthDistanceInfo
 * Inputs         : msgBuf_pp - pointer to the input buffer
 *                  tagLen - Length of the tag
 *                  dlCreateReq_p - pointer to Downlink create UE structure
 * Outputs        : None
 * Returns        : MAC_SUCCESS or reason for MAC_FAILURE
 * Description    : This function is called on receiving CA_LC_BANDWIDTH_DIST_INFO
 *                  and returns the result of parsing of Scell Info to the 
 *                  calling function.
 * ****************************************************************************/                                        
STATIC inline MacRetType parseCALCBandwidthDistanceInfo(UInt16 tagLen,
    UInt8 **msgBuf_pp,
    ueContextForDownlink *dlUEInfo_p
    )
{
    UInt8 *msg_p    = PNULL;
    MacRetType resp = MAC_SUCCESS;
    msg_p = *msgBuf_pp;
    UInt8 numBandwidthDist = 0, bandWidthDistribution[MAX_CELLS_PER_UE] = {0};
    if ( tagLen != CA_LC_BANDWIDTH_DIST_INFO_TAG_LEN )
    {
        lteWarning("Invalid Tag Length %d for"
                "CA_LC_BANDWIDTH_DIST"
                "_INFO_TAG_LEN  Info\n"
                ,tagLen);
        return MAC_FAILURE;
    }
    for (numBandwidthDist = 0; numBandwidthDist < MAX_CELLS_PER_UE; 
            numBandwidthDist++)
    {
        bandWidthDistribution[numBandwidthDist] = *msg_p++;
        resp = validateCALCBandwidthDistribution
                (bandWidthDistribution[numBandwidthDist]);
        if (resp != MAC_SUCCESS)
        {
            lteWarning("Parsing CA_LC_BANDWIDTH_DIST_INFO failed.\n");
            return resp;
        }
        dlUEInfo_p->carrierAggrConfig.bandWidthDistribution[numBandwidthDist] = 
            bandWidthDistribution[numBandwidthDist];
    }
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : validateCALCBandwidthDistribution
 *  Inputs         : bandWidthDistribution
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or reason for failure.
 *  Description    : This API takes n1Pucch to validate and returns the result
 *                   of validation to the calling function.
 * ****************************************************************************/
STATIC inline MacRetType validateCALCBandwidthDistribution(
    UInt8 bandWidthDistribution)
{
    if (bandWidthDistribution > MAX_CA_LC_BANDWIDTH_DIST_VAL)
    {
        lteWarning("Invalid value for bandWidthDistribution [%u], valid range [%u,%u] .\n",
                bandWidthDistribution,
                MIN_CA_LC_BANDWIDTH_DIST_VAL,MAX_CA_LC_BANDWIDTH_DIST_VAL);
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseScellDLHarqProc
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellDLHarqProc(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p
        ) 
{
    UInt8 dlNumHarqProcess =0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    if (tagLen != DL_NUM_HARQ_PROC_TAG_LEN)
    {
        lteWarning("Invalid Tag Length %u for"
                "DL_NUM_HARQ_PROC   Info\n"
                ,tagLen);
        return MAC_SYNTAX_ERROR;
    }    
    dlNumHarqProcess = *msg_p++;
    if ( (dlNumHarqProcess < MIN_DL_NUM_HARQ_PROC_VAL)
            || (dlNumHarqProcess > MAX_DL_NUM_HARQ_PROC_VAL) )
    {
        lteWarning("Invalid value for dlNumHarqProcess[%u] .\n",
                dlNumHarqProcess);
        return MAC_SYNTAX_ERROR;
    }
    dlScellInfo_p->dlNumHarqProcess = dlNumHarqProcess;
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseScellDLInitMCS
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellDLInitMCS(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p
        ) 
{
    UInt8 dlInitMCS = 0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    if (tagLen != DL_INIT_MCS_TAG_LEN)
    {
        lteWarning("Invalid Tag Length %u for"
                "DL_INIT_MCS   Info\n"
                ,tagLen);
        return MAC_SYNTAX_ERROR;
    }    
    dlInitMCS = *msg_p++;
    if (dlInitMCS > MAX_DL_INIT_MCS_VAL)
    {
        lteWarning("Invalid value for dlInitMCS[%u] .\n",
                dlInitMCS);
        return MAC_SYNTAX_ERROR;
    }
    dlScellInfo_p->dlInitMCS = dlInitMCS;
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseScellNumOfLayerInfo
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *                   ueCategory -
 *                   cellIndex -
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for NUM_OF_LAYER_INFO
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellNumOfLayerInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 ueCategoryV1020,
        UInt8 cellIndex
        ) 
{
    UInt8 rankIndicator = 0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    if (tagLen != NUM_OF_LAYER_INFO_LEN)
    {
        lteWarning(
                "Invalid Tag Length %u for "
                "NUM_OF_LAYER_INFO\n",tagLen);
        return MAC_SYNTAX_ERROR;
    }    

    rankIndicator = *msg_p++;
/* AZIM Need to Fix parameters old function is called with one parameter */
    if (MAC_SUCCESS != 
            //validateNumOfLayer(rankIndicator,ueCategoryV1020,cellIndex))
            validateNumOfLayer(rankIndicator))
    {
        return MAC_SYNTAX_ERROR;
    }
    dlScellInfo_p->rankIndicator = rankIndicator;
    ulScellInfo_p->rankIndicator = rankIndicator;
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseScellCodebookIndexInfo
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *                   ulScellInfo_p - pointer to uplink Scell info
 *                   cellIndex - cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CODEBOOK_INDEX_INFO
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellCodebookIndexInfo(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p
        ,UInt8 cellIndex
        ) 
{
    UInt8 precodingIndex = 0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    if (tagLen != CODEBOOK_INDEX_INFO_LEN)
    {
        lteWarning(
                "Invalid tag length %u "
                "for CODEBOOK_INDEX_INFO\n",tagLen);
        return MAC_SYNTAX_ERROR;
    }    
    precodingIndex = *msg_p++;
    if (MAC_SUCCESS != 
            validatePrecodingIndex(precodingIndex))
    {
        return MAC_SYNTAX_ERROR;
    }
    dlScellInfo_p->precodingIndex = precodingIndex;
    ulScellInfo_p->precodingIndex = precodingIndex;
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseScellUserLocationType
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *                   ulScellInfo_p - pointer to uplink Scell info
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for USER_LOCATION_TYPE
 *                   and fills in the structure needed by MAC to create a 
 *                   UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellUserLocationType(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p
        ) 
{
    UInt8 userLocationType = 0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    if((tagLen - TAG_LEN) < USER_LOCATION_TYPE_LEN)
    {
        lteWarning(
                "Invalid tag length %u "
                "for USER_LOCATION_TYPE\n",tagLen);
        return MAC_SYNTAX_ERROR;
    }
    userLocationType = *msg_p++;
    if(userLocationType >= MAX_USER_LOCATION)
    {
        return MAC_FAILURE;
    }
    dlScellInfo_p->userLocationType = userLocationType;
    ulScellInfo_p->userLocationType = userLocationType;
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseScellRFParams
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *                   ulScellInfo_p - pointer to uplink Scell info
 *                   cellIndex - cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellRFParams(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 cellIndex,
        UInt16 ueIndex
        ) 
{
    UInt8 hdFddFlag = 0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    if(tagLen != RF_PARAMS_LEN)
    {
        lteWarning("Invaild Length %u recived for RF_PARAMS_INVALID_LENGTH\n"
			   				,tagLen);
        return MAC_FAILURE;
    }
#ifdef HD_FDD_CONFIG
    hdFddFlag = *msg_p++;
    /* The flag should be either TRUE or FALSE */
    if((hdFddFlag!=TRUE) && (hdFddFlag!= FALSE))
    {
                lteWarning("Invalid hdFddFlag received in Create UE");
        return MAC_FAILURE;
    }
    dlScellInfo_p->isHDFddFlag = hdFddFlag;
    ulScellInfo_p->isHDFddFlag = hdFddFlag;
#else
    msg_p++;
#endif
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}

/****************************************************************************
 *  Function Name  : parseScellActivationStatus
 *  Inputs         : msgBuf_pp - pointer to the input buffer
 *                   tagLen - length of tag
 *                   dlScellInfo_p - pointer to downlink Scell info
 *                   ulScellInfo_p - pointer to uplink Scell info
 *                   cellIndex - cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function parses the input buffer for CQI_INFO 
 *                   and fills in the structure needed by MAC to create a UE context
 * ****************************************************************************/
STATIC inline MacRetType parseScellActivationStatus(UInt8 **msgBuf_pp,
        UInt16 tagLen,
        ScellInfo *dlScellInfo_p,
        ScellInfo *ulScellInfo_p,
        UInt8 cellIndex
        ) 
{
    UInt8 isScellActivateStatus = 0;
    UInt8 *msg_p    = PNULL;
    msg_p = *msgBuf_pp;
    if(tagLen != SCELL_ACTIVATE_STATUS_LEN)
    {
        lteWarning("Invalid length %u recieved in SCELL_ACTIVATE_STATUS_INVALID_LENGTH\n"
						,tagLen);
        return MAC_FAILURE;
    }
    isScellActivateStatus = *msg_p++;
    /* The flag should be either TRUE or FALSE */
    if(!( (isScellActivateStatus==TRUE) || 
                (isScellActivateStatus == FALSE)) )
    {
        lteWarning("Invalid value %d for Flag isScellActivateStatus received in Create UE\n"
								,isScellActivateStatus);
        return MAC_FAILURE;
    }
    dlScellInfo_p->isScellActivateStatus = isScellActivateStatus;
    ulScellInfo_p->isScellActivateStatus = isScellActivateStatus;
    /* + SPR 11213 Changes */
    /* Code Removed */
    /* - SPR 11213 Changes */
    *msgBuf_pp = msg_p;
    return MAC_SUCCESS;
}


/****************************************************************************
 *  Function Name  : updateUeCAUplinkContext
 *                   &ulUEInfo - pointer to uplink Scell info
 *                   cellIndex - cell index used at MAC
 *  Outputs        : None
 *  Returns        : MAC_SUCCESS or MAC_FAILURE
 *  Description    : This function updates params in ueContext received in Ca Config 
 *                   in MAC_RECONFIGURE_UE_REQ
 * ****************************************************************************/
STATIC inline updateUeCAUplinkContext(ueContextForUplink *ulUEInfo)
{
    ueContextForUplink* ulUEContext_p = PNULL;
    /* SPR 11595 Fix Start */
    ulUEContext_p = uplinkContextInfoMap_g[ulUEInfo->ueIdx];
    /* SPR 11595 Fix End */
    if ( PNULL == ulUEContext_p )
    {
        fprintf(stderr,"function [%s]->ue context is NULL for ueIndex = %d\n",__func__,ulUEInfo->ueIdx );
        //resp = MAC_INVALID_UE_ID;
    }
    ulUEContext_p->carrierAggrConfig.scellCount = ulUEInfo->carrierAggrConfig.scellCount;
    ulUEContext_p->carrierAggrConfig.scellInfo[0].cellIndex = ulUEInfo->carrierAggrConfig.scellInfo[0].cellIndex;
    ulUEContext_p->carrierAggrConfig.scellInfo[0].sCellIndex= ulUEInfo->carrierAggrConfig.scellInfo[0].sCellIndex;
    ulUEContext_p->carrierAggrConfig.scellInfo[0].transmissionMode= ulUEInfo->carrierAggrConfig.scellInfo[0].transmissionMode;
    ulUEContext_p->carrierAggrConfig.scellInfo[0].dlNumHarqProcess= ulUEInfo->carrierAggrConfig.scellInfo[0].dlNumHarqProcess;
    /* SPR 12135 Fix Start */
    ulUEContext_p->carrierAggrConfig.scellInfo[0].rankIndicator = ulUEInfo->carrierAggrConfig.scellInfo[0].
        rankIndicator;
 /* SPR 12135 Fix End */
    /* + CA_TDD_HARQ_CHANGES */
#ifdef TDD_CONFIG
    ulUEContext_p->ackNackMode = CHANNEL_SELECTION;
#endif
    /* - CA_TDD_HARQ_CHANGES */
}
/* - CA_PHASE2 */
/* SPR 12135 Fix Start */
/****************************************************************************
 * Function Name  : getLAndNValuesScell
 * Inputs         : ueIndex 
 * Outputs        : None  
 * Returns        : None 
 * Description    : This function calculates and fills the N, ueSelectedN, 
 *                  posOfMSelSBAperiodic and LBitLabelForPeriodic values.
 ****************************************************************************/
void getLAndNValuesScell(UInt16 ueIndex, UInt8 cellIndex)
{
    DLCQIInfo *scellDlCQIInfo = PNULL;
    ueContextForUplink   *ulUEInfo_p = PNULL;
    ulUEInfo_p = uplinkContextInfoMap_g[ueIndex];

/* Aperiodic CQI Changes Start */
    scellDlCQIInfo = &ulUEInfo_p->carrierAggrConfig.scellInfo[0].dlCQIInfo;

	UInt8 count = 0;

	if ( (CQI_MODE_2_0 == scellDlCQIInfo->aperiodicModeType) ||
			(CQI_MODE_2_2 == scellDlCQIInfo->aperiodicModeType) )
	{
		if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 8 && 
				cellConfigUeSim_g[cellIndex].dlAvailableRBs <= 10)
		{
			scellDlCQIInfo->aperiodicSubBandSize = 2;
			scellDlCQIInfo->preferredSubbands = 1;
			scellDlCQIInfo->ueSelectedN = 
				ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
						scellDlCQIInfo->aperiodicSubBandSize);
		}
		else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 11 && \
				cellConfigUeSim_g[cellIndex].dlAvailableRBs <=26)
		{
			scellDlCQIInfo->aperiodicSubBandSize = 2;
			scellDlCQIInfo->preferredSubbands = 3;
			scellDlCQIInfo->ueSelectedN = 
				ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
						scellDlCQIInfo->aperiodicSubBandSize);
		}
		else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 27 &&\
				cellConfigUeSim_g[cellIndex].dlAvailableRBs <=63)
		{
			scellDlCQIInfo->aperiodicSubBandSize = 3;
			scellDlCQIInfo->preferredSubbands = 5;
			scellDlCQIInfo->ueSelectedN = 
				ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
						scellDlCQIInfo->aperiodicSubBandSize);
		}
		else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 64 &&\
				cellConfigUeSim_g[cellIndex].dlAvailableRBs <=110)
		{ 
			scellDlCQIInfo->aperiodicSubBandSize = 4;
			scellDlCQIInfo->preferredSubbands = 6;
			scellDlCQIInfo->ueSelectedN = 
				ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
						scellDlCQIInfo->aperiodicSubBandSize);
		}

		/* This will contain Binomial cofficient of ue selected N and 
		 * M preferred subband  from nCm_g */
		UInt32 nCm = 0;
		initUESelectedBinomalValues(ueIndex);
		nCm = getBinomialCofficent(scellDlCQIInfo->ueSelectedN,
				scellDlCQIInfo->preferredSubbands);
		scellDlCQIInfo->posOfMSelSBAperiodic = 
			ceil(log((UDouble32)nCm)/log(2)); 

		for(count=0; count<MAX_CQI_VALUES_PER_PROFILE; count++)
		{
/* Aperiodic CQI Changes Start */
			cqiAperiodicReportMode20_g[cellIndex][count].posOfMSelSubband = 
				getposOfMSelSBForAperiodicMode20(count,ueIndex,cellIndex);
			cqiAperiodicReportMode22_g[cellIndex][count].posOfMSelSubband_RI_1 = 
				getposOfMSelSBForAperiodicMode22RIOne(count,ueIndex, cellIndex);
			cqiAperiodicReportMode22_g[cellIndex][count].posOfMSelSubband_RI_2 = 
				getposOfMSelSBForAperiodicMode22RITwo(count,ueIndex, cellIndex);

			/* EICIC +*/
				cqiAperiodicReportMode20_g_2[cellIndex][count].posOfMSelSubband = 
				getposOfMSelSBForAperiodicMode20Abs(count,ueIndex,cellIndex);
			cqiAperiodicReportMode22_g_2[cellIndex][count].posOfMSelSubband_RI_1 = 
				getposOfMSelSBForAperiodicMode22RIOneAbs(count,ueIndex, cellIndex);
			cqiAperiodicReportMode22_g_2[cellIndex][count].posOfMSelSubband_RI_2 = 
				getposOfMSelSBForAperiodicMode22RITwoAbs(count,ueIndex, cellIndex);
			/* EICIC -*/
			
/* Aperiodic CQI Changes End */

		}
	}

	if ( (CQI_MODE_1_2 == scellDlCQIInfo->aperiodicModeType) ||
			(CQI_MODE_3_0 == scellDlCQIInfo->aperiodicModeType) ||
			(CQI_MODE_3_1 == scellDlCQIInfo->aperiodicModeType) ||
			(CQI_PERIODIC_TYPE_10 == scellDlCQIInfo->periodicModeType) ||
			(CQI_PERIODIC_TYPE_11 == scellDlCQIInfo->periodicModeType) ||
			(CQI_PERIODIC_TYPE_20 == scellDlCQIInfo->periodicModeType) ||
			(CQI_PERIODIC_TYPE_21 == scellDlCQIInfo->periodicModeType) )
	{
		if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 8 && \
				cellConfigUeSim_g[cellIndex].dlAvailableRBs <= 10)
		{
			scellDlCQIInfo->aperiodicSubBandSize = 4;
			scellDlCQIInfo->bandwidthParts = 1;
			scellDlCQIInfo->N = 
				ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
						scellDlCQIInfo->aperiodicSubBandSize);
		}
		else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 11 && \
				cellConfigUeSim_g[cellIndex].dlAvailableRBs <=26)
		{
			scellDlCQIInfo->aperiodicSubBandSize = 4;
			scellDlCQIInfo->bandwidthParts = 2;
			scellDlCQIInfo->N = 
				ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
						scellDlCQIInfo->aperiodicSubBandSize);
		}
		else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 27 &&\
				cellConfigUeSim_g[cellIndex].dlAvailableRBs <=63)
		{
			scellDlCQIInfo->aperiodicSubBandSize = 6;
			scellDlCQIInfo->bandwidthParts = 3;
			scellDlCQIInfo->N = 
				ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
						scellDlCQIInfo->aperiodicSubBandSize);
		}
		else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 64 && \
				cellConfigUeSim_g[cellIndex].dlAvailableRBs <=110)
		{ 
			scellDlCQIInfo->aperiodicSubBandSize = 8;
			scellDlCQIInfo->bandwidthParts = 4;
			scellDlCQIInfo->N = 
				ceil((UDouble32)cellConfigUeSim_g[cellIndex].dlAvailableRBs / 
						scellDlCQIInfo->aperiodicSubBandSize);
		}
		scellDlCQIInfo->LBitLabelForPeriodic =  
			ceil((UDouble32)(log(ceil((UDouble32)\
								cellConfigUeSim_g[cellIndex].dlAvailableRBs/
								scellDlCQIInfo->aperiodicSubBandSize/
								scellDlCQIInfo->bandwidthParts))/log(2)));
	}

#ifdef UESIM_ENHANCEMENTS
	if(cellConfigUeSim_g[cellIndex].dlAvailableRBs <= 10)
	{
		cellConfigUeSim_g[cellIndex].rat0RbgSize = 1;  
	}
	else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 11 &&\
			cellConfigUeSim_g[cellIndex].dlAvailableRBs <=26)
	{
		cellConfigUeSim_g[cellIndex].rat0RbgSize = 2;  
		if(15 == cellConfigUeSim_g[cellIndex].dlAvailableRBs)
		{
			cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x40;  
			cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated = 0x3F;  
			cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x180;  
		}
		else if(25 == cellConfigUeSim_g[cellIndex].dlAvailableRBs)
		{
			cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x800;  
			cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated = 0x7FF;  
			cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x3000;  
		}

	}
	else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 27 && \
			cellConfigUeSim_g[cellIndex].dlAvailableRBs <=63)
	{
		cellConfigUeSim_g[cellIndex].rat0RbgSize = 3;  
		cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x4000;  
		cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated = 0x3FFF;  
		cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x18000;  
	}
	else if(cellConfigUeSim_g[cellIndex].dlAvailableRBs >= 64 && \
			cellConfigUeSim_g[cellIndex].dlAvailableRBs <=110)
	{ 
		cellConfigUeSim_g[cellIndex].rat0RbgSize = 4;  
		if(75 == cellConfigUeSim_g[cellIndex].dlAvailableRBs)
		{
			cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x10000;  
			cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated  = 0xFFFF;  
			cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x60000;  
		}
		else if(100 == cellConfigUeSim_g[cellIndex].dlAvailableRBs)
		{
			cellConfigUeSim_g[cellIndex].bitsMaskForSpan = 0x400000;  
			cellConfigUeSim_g[cellIndex].rat1MaxRBsAllocated = 0x3FFFFF;  
			cellConfigUeSim_g[cellIndex].bitsMaskForSubsetCount = 0x1800000;  
		}

	}
#endif    
}
/* SPR 12135 Fix End */
/* - CA_PHASE2 */
