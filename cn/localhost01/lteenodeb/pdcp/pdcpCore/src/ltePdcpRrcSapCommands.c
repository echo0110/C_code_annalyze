/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpRrcSapCommands.c,v 1.1.1.1.6.2.4.2.2.6 2010/10/20 15:15:05 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : RRC API message handlers implementation 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpRrcSapCommands.c,v $
 * Revision 1.1.1.1.6.2.4.2.2.6  2010/10/20 15:15:05  gur20491
 * removing some unwanted logs
 *
 * Revision 1.1.1.1.6.2.4.2.2.5  2010/10/20 05:31:59  gur20491
 * RRC 2.0.1 Compliance changes
 *
 * Revision 1.1.1.1.6.2.4.2.2.4  2010/10/14 02:51:30  gur19479
 * updated to fix SPR#671
 *
 * Revision 1.1.1.1.6.2.4.2.2.3  2010/10/13 10:28:41  gur23054
 * Names removed from code
 *
 * Revision 1.1.1.1.6.2.4.2.2.2  2010/10/07 13:23:19  gur23054
 * Fix for SPR 713
 *
 * Revision 1.1.1.1.6.2.4.2.2.1  2010/09/21 15:49:17  gur20491
 * FAPI changes
 *
 * Revision 1.1.1.1.6.2.2.2  2010/08/31 05:23:31  gur19836
 * Added to configure DRB Entity in AM Mode with snSize = 12
 *
 * Revision 1.1.1.1.6.2.2.1  2010/08/16 04:03:14  gur19836
 * Integration Issues Fixed
 *
 * Revision 1.1.1.1.6.2  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.7.2.3  2009/07/20 08:21:56  gur19479
 * Changes done for SRB handling
 *
 * Revision 1.7.2.2  2009/07/13 11:04:33  gur20435
 *
 * PDCP Optimizations
 *
 * Revision 1.7.2.1  2009/06/09 11:33:51  gur19140
 * *** empty log message ***
 *
 * Revision 1.6  2009/05/20 13:38:43  gur20548
 * header file name changed
 *
 * Revision 1.5  2009/05/15 18:43:24  gur19140
 * fixes
 *
 * Revision 1.4  2009/05/15 09:07:42  gur19836
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
#include "ltePdcpRrcSap.h"
#include "ltePdcpTlvUtils.h"
#include "ltePdcpLteDefs.h"
#include "ltePdcpTSDefs.h"
#include "ltePdcpErrors.h"
#include "ltePdcpContext.h"
#include "ltePdcpDataTransferTx.h"
#include "ltePdcpDataTransferRx.h"
#include "logging.h"
#include "alarm.h"
#include "integrity.h"
#include "ltePdcpStatistic.h"
#include "ltePdcpRrcSapCommands.h"
/* SPR 20591 Fix Start */
/* SPR 20591 Fix End */



/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/
/* Start of change for VARIBLE Declaration */
enum RrcSpsIntervalListT
{ 
    sf10 = 0, 
    sf20, 
    sf32,
    sf40, 
    sf64, 
    sf80,
    sf128, 
    sf160, 
    sf320, 
    sf640,

    intProfileLast
} RrcSpsIntervalList;
/* This structure will be passed to the nested tag handlers */
typedef struct RrcTagParamsT
{
    UInt16      ueIndex;        /* UE identifier */
    UInt16      rbDirection;    /* Radio bearer link type (TX, RX, Both) */
    UInt8       lcId;           /* logical channel identifier */
#ifndef UE_SIM_TESTING
    UInt8       qci;            /* Quality class Identifier */
#endif    
/* SPR 1757 changes start */
} __attribute__ ((packed)) RrcTagParams, *LP_RrcTagParams;
/* SPR 1757 changes end */

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/
/* RRC API command handlers definitions */
STATIC void rrcCmdCreateUeEntity( UInt8* data_p, UInt16 size );
STATIC void rrcCmdReconfigUeEntity( UInt8* data_p, UInt16 size );
STATIC void rrcCmdDeleteUeEntity( UInt8* data_p, UInt16 size );
STATIC void rrcCmdSrbDataReq( UInt8* data_p, UInt16 size );
STATIC void rrcCmdSuspendUeEntity( UInt8* data_p, UInt16 size );
STATIC void rrcCmdSnHfnStatusReq( UInt8* data_p, UInt16 size );
STATIC void rrcCmdSnHfnStatusInd( UInt8* data_p, UInt16 size );
STATIC void rrcCmdDataBufferStop( UInt8* data_p, UInt16 size );
STATIC void rrcCmdMacIReq( UInt8* data_p, UInt16 size );
STATIC void rrcCmdReestablishUeEntity( UInt8* data_p, UInt16 size );
STATIC void rrcCmdResumeUeEntity( UInt8* data_p, UInt16 size );
STATIC void rrcCmdChangeCrnti( UInt8* data_p, UInt16 size );
/*COUNTER_CHK_START*/
STATIC void rrcCmdDrbCountMsbReq( UInt8* data_p, UInt16 size );
/*COUNTER_CHK_END*/
/* + SPR 19066 */
STATIC void rrcCmdHoPrepInfoReq( UInt8* data_p, UInt16 size );
/* - SPR 19066 */

/* RRC API tags checkers definitions */
STATIC UInt16 rrcTagCheckCreateSrbEntity( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckCreateDrbEntity( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckDeleteEntity( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckReconfigSrbEntity( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckReconfigDrbEntity( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckConfigureIntegrity( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckDiscardTimer( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckSnSize( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckStatusReportRequired( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckConfigureCiphering( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckConfigureRohc( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckHoTrigger( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckMessageMacI( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckRbEntity( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckSnHfnStatus( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckUlRecvSn( UInt8* data_p, UInt16 size );
/* SPR 3608 changes start */
STATIC UInt16 rrcTagCheckConfigureUePsr( UInt8* data_p, UInt16 size );
/* SPR 3608 changes end */
/* SPR 3607 fix start */
STATIC UInt16 rrcTagCheckSecurityOffset( UInt8* data_p, UInt16 size );
/* SPR 3607 fix end */
/*COUNTER_CHK_START*/
STATIC UInt16 rrcTagCheckDrbCountMsbList( UInt8* data_p, UInt16 size );
/*COUNTER_CHK_END*/
#ifndef UE_SIM_TESTING
STATIC UInt16 rrcTagCheckSpsDlInfo( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckSpsUlInfo( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckSpsDlSetupInfo( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagCheckSpsUlSetupInfo( UInt8* data_p, UInt16 size );
#endif
/* Lossy HO changes start */
STATIC UInt16 rrcTagCheckLossyHoRequired (UInt8* data_p, UInt16 size );
/* Lossy HO changes stop */

/* RRC API tags handlers definitions */
STATIC UInt16 rrcTagCreateSrbEntity( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagCreateDrbEntity( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size,  LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagDeleteEntity( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size,  LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagReconfigSrbEntity( UInt16 tagId, void* params, 
        UInt8* data_p, UInt16 size,  LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagReconfigDrbEntity( UInt16 tagId, void* params, 
        UInt8* data_p, UInt16 size,  LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagConfigureIntegrity( UInt16 tagId, void* params, 
        UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagConfigureDiscardTimer( UInt16 tagId, void* params, 
        UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagConfigureSnSize( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagConfigureStatusReportRequired( UInt16 tagId, void* params, 
        UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage);
/* SPR_1235_FIX_START*/
STATIC UInt16 rrcTagConfigureSrbCiphering( UInt16 tagId, void* params, 
        UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagConfigureDrbCiphering( UInt16 tagId, void* params, 
        UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage);
/* SPR_1235_FIX_END*/
STATIC UInt16 rrcTagConfigureRohc( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagReConfigureRohc( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagHoTrigger( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagMessageMacI( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage_p);
STATIC UInt16 rrcTagSnHfnStatus( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagUlRecvSn( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagReEstablishRbEntity( UInt16 tagId, void* params, 
        UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagResumeRbEntity( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
/* SPR 3608 changes start */
STATIC UInt16 rrcTagConfigureUePsr( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
/* SPR 3608 changes end */
/* SPR 3607 Fix start */
STATIC UInt16 rrcTagSecurityOffset( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
/* SPR 3607 Fix end */
STATIC UInt16 rrcTagDrbCountMsbList( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagLossyHoConfigure (UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
#ifndef UE_SIM_TESTING
STATIC UInt16 rrcTagSpsConfig( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagSpsDlInfo( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagSpsUlInfo( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagSpsDlSetupInfo( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
STATIC UInt16 rrcTagSpsUlSetupInfo( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
#endif

/* SPR 17157 Fix start */
STATIC UInt16 rrcTagCheckMaciReq( UInt8* data_p, UInt16 size );

STATIC UInt16 rrcTagMaciReq( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);

STATIC void rrcMakeTagResponseMaciResp(UInt8* macI, LP_TlvMessage responseMessage);
/* SPR 17157 Fix end */

/* + SPR 19066 */
STATIC UInt16 rrcTagCheckInactivityTimer( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagInactivityTimer( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage );
STATIC void rrcMakeTagInactiveUe( UInt16 tagId, UInt16 ueIndex, 
    LP_TlvMessage responseMsg_p );
STATIC UInt16 rrcTagCheckUeInactTimeReq( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagUeInactTimeReq( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage );
STATIC void rrcMakeTagResponseInactiveTime( UInt16 tagId, tickType_t inactTimeVal,
        LP_TlvMessage responseMsg_p );
STATIC UInt16 rrcTagCheckInactiveTimeLapse( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagInactTimeLapse( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage );
STATIC UInt16 rrcTagCheckHoMaciReq( UInt8* data_p, UInt16 size );
STATIC UInt16 rrcTagHoMaciReq( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage);
/* - SPR 19066 */

void rrcMakeTagResponseDrbCount(UInt16 tagId, UInt16 subTag1, UInt32 count1,
        UInt16 subTag2, UInt32 count2, UInt8 lcId, LP_TlvMessage responseMessage);
/* RRC Sap tools functions */
void rrcMakeTagResponseMacI(UInt16 tagId, LP_RrcMessageMacI cellIdentity, 
        UInt8* macI, UInt16 result, LP_TlvMessage responseMessage);
void rrcMakeTagError(UInt16 tagId, UInt8 lcId, UInt16 result, 
        LP_TlvMessage responseMessage);


/** ALIGNMENT changes start  ***/
void pdcpParseRrcTagCreateSrbEntity (LP_RrcTagCreateSrbEntity tagCreateSrb_p,
     UInt8* data_p);
void pdcpParseRrcTagCreateDrbEntity (LP_RrcTagCreateDrbEntity  tagCreateDrb_p,
      UInt8* data_p);
void pdcpParseRrcTagConfigureIntegrity (LP_RrcTagConfigureIntegrity
      tagConfigureIntegrity_p, UInt8* data_p);
void pdcpParseRrcTagConfigureCiphering (LP_RrcTagConfigureCiphering
      tagConfigureCiphering_p, UInt8* data);
void pdcpParseRrcTagReconfigSrbEntity (LP_RrcTagReconfigSrbEntity
      tagRecofigSrb_p, UInt8* data_p);
void pdcpParseRrcTagReconfigDrbEntity (LP_RrcTagReconfigDrbEntity
      tagRecofigDrb_p, UInt8* data_p);
void pdcpParseRrcTagConfigureRohc (LP_RrcTagConfigureRohc
      tagConfigureRohc_p, UInt8* data_p);
void pdcpParseRrcTagConfigureDiscardTimer (LP_RrcTagConfigureDiscardTimer
      tagConfigureDiscardTimer_p, UInt8* data_p);
void pdcpParseRrcTagSnHfnStatus (LP_RrcTagSnHfnStatus tagSnHfnStatus_p,
      UInt8* data_p);
void pdcpParseRrcTagDrbCountMsbList (LP_RrcTagDrbCountMsbList tagDrbCountMsb_p,
       UInt8* data_p);
void pdcpParseRrcTagHoTriggered (LP_RrcTagHoTriggered tagHoTriggered_p,
      UInt8* data_p);
void pdcpParseRrcTagDeleteEntity (LP_RrcTagDeleteEntity tagDelete_p,
     UInt8* data_p);
void pdcpParseRrcTagSecurityOffset (LP_RrcTagSecurityOffset tagSecurityOffset_p,
     UInt8* data_p);
void pdcpParseRrcTagConfigureSnSize (LP_RrcTagConfigureSnSize tagConfigureSnSize_p,
     UInt8* data_p);
void pdcpParseRrcTagConfigureStatusReportRequired (LP_RrcTagConfigureStatusReportRequired
     tagConfigureStatusReportRequired_p, UInt8* data_p);
void pdcpParseRrcTagConfigureUePsr (LP_RrcTagConfigureUePsr
     tagConfigUePsr_p, UInt8* data_p);
void pdcpParseRrcTagUlRecvSn (LP_RrcTagUlRecvSn tagUlRecvSn_p, UInt8* data_p);
void pdcpParseRrcTagRbEntity (LP_RrcTagRbEntity tagRbEntity_p, UInt8* data_p);
void pdcpParseRrcTagSpsDlInfo (LP_RrcTagSpsDlInfo tagSpsDlInfo_p, UInt8* data_p);
void pdcpParseRrcTagSpsUlInfo (LP_RrcTagSpsUlInfo tagSpsUlInfo_p, UInt8* data_p);
void pdcpParseRrcTagSpsDlSetupInfo (LP_RrcTagSpsDlSetupInfo tagSpsDlSetupInfo_p,
      UInt8* data_p);
void pdcpParseRrcTagSpsUlSetupInfo (LP_RrcTagSpsUlSetupInfo tagSpsUlSetupInfo_p,
     UInt8* data_p);
/* SPR 17157 Fix start */
void pdcpParseRrcTagMaciReq(LP_RrcTagMaciReq tagMaciReq_p, UInt8* data_p);
/* SPR 17157 Fix end */
/* + SPR 19066 */
void pdcpParseRrcTagInactivityTimer( LP_RrcTagInactivityTimer tagInactTimer_p, 
        UInt8* data_p );
void pdcpParseRrcTagInactTimeReqd( LP_RrcTagInactTimeReqd tagInactTimeReqd_p,
       UInt8 *data_p );
/* - SPR 19066 */
#ifdef ENDC_ENABLED
STATIC void rrcCmdUpdateTrafficInd( UInt8* data_p, UInt16 size );
#endif


/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/
extern PdcpContext pdcpContext;
extern PdcpStats   pdcpStats ;
/* SPR 15909 fix start */
extern tickType_t      pdcpCurrentTime_g;
/* SPR 15909 fix start */
#ifndef UE_SIM_TESTING
extern UInt16 setUeIdxValid(UInt16, UInt16);
extern UInt16 getRrcUeIdxFromUeIdx(UInt16);
extern UInt16 getUeIdxFromRrcUeIdx(UInt16);
#endif
#ifdef L2_FAST_PATH
UInt8             pdcpIsSrbDataReceivedInTxQ_g = 0;
#endif
/* SPR 20591 Fix Start */
/* SPR 20591 Fix End */
/* RRC API command handlers table */
TlvCommandTable rrcCmdTable_g[] =
{
    { { PDCP_CREATE_UE_ENTITY_REQ       },     rrcCmdCreateUeEntity,      },
    { { PDCP_RECONFIG_UE_ENTITY_REQ     },     rrcCmdReconfigUeEntity,    },
    { { PDCP_DELETE_UE_ENTITY_REQ       },     rrcCmdDeleteUeEntity,      },
    { { PDCP_SRB_DATA_REQ               },     rrcCmdSrbDataReq,          },
    { { PDCP_SUSPEND_UE_ENTITY_REQ      },     rrcCmdSuspendUeEntity,     },
    { { PDCP_SN_HFN_STATUS_REQ          },     rrcCmdSnHfnStatusReq,      },
    { { PDCP_SN_HFN_STATUS_IND          },     rrcCmdSnHfnStatusInd,      },
    { { PDCP_DATA_BUFFER_STOP_IND       },     rrcCmdDataBufferStop,      },
    { { PDCP_MAC_I_REQ                  },     rrcCmdMacIReq,             },
    { { PDCP_RE_ESTABLISH_UE_ENTITY_REQ },     rrcCmdReestablishUeEntity, },
    { { PDCP_RESUME_UE_ENTITY_REQ       },     rrcCmdResumeUeEntity,      },
    { { PDCP_CHANGE_CRNTI_REQ           },     rrcCmdChangeCrnti,         },
    /*COUNTER_CHK_START*/
    { { PDCP_DRB_COUNT_MSB_REQ          },     rrcCmdDrbCountMsbReq,      },
    /*COUNTER_CHK_END*/
#if defined(ENDC_ENABLED)
    { { PDCP_UPDATE_TRAFFIC_IND      },    rrcCmdUpdateTrafficInd, },
#endif
    /* + SPR 19066 */
    { { PDCP_HO_PREP_INFO_REQ           },     rrcCmdHoPrepInfoReq,      },
    /* - SPR 19066 */
    { { TLV_LAST_ID                     },     PNULL,                     },
};

/* Tags handlers table for PDCP_CREATE_UE_ENTITY_REQ RRC API command */
TlvTagHandler rrcCreateUeEntityTagsTable[] =
{
    { { PDCP_CREATE_SRB_ENTITY                },   rrcTagCreateSrbEntity,             rrcTagCheckCreateSrbEntity, },
    { { PDCP_CREATE_DRB_ENTITY                },   rrcTagCreateDrbEntity,             rrcTagCheckCreateDrbEntity, },
    { { PDCP_CONFIGURE_INTEGRITY_PROTECTION   },   rrcTagConfigureIntegrity,          rrcTagCheckConfigureIntegrity, },
    /* SPR_1235_FIX_START*/
    { { PDCP_CONFIGURE_SRB_CIPHERING          },   rrcTagConfigureSrbCiphering,       rrcTagCheckConfigureCiphering, },
    { { PDCP_CONFIGURE_DRB_CIPHERING          },   rrcTagConfigureDrbCiphering,       rrcTagCheckConfigureCiphering, },
    /* SPR_1235_FIX_END*/
    { { PDCP_HO_TRIGGERED                     },   rrcTagHoTrigger,                   rrcTagCheckHoTrigger, },
#ifndef UE_SIM_TESTING 
    { { PDCP_SPS_CONFIG                       },  rrcTagSpsConfig,                    PNULL  },
#endif
    /* SPR 17157 Fix start */
    { { RRC_PDCP_MACI_REQ_TAG                 },  rrcTagMaciReq,                      rrcTagCheckMaciReq,  },
    /* SPR 17157 Fix end */
    /* + SPR 19066 */
    { { RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_TAG },  rrcTagInactivityTimer,            rrcTagCheckInactivityTimer,  },
    /* - SPR 19066 */
    { { TLV_LAST_ID                           },   PNULL,                             PNULL, },
};

/* Tags handlers table for PDCP_RECONFIG_UE_ENTITY_REQ RRC API command */
TlvTagHandler rrcReconfigUeEntityTagsTable[] =
{
    { { PDCP_CREATE_SRB_ENTITY                },   rrcTagCreateSrbEntity,              rrcTagCheckCreateSrbEntity, },
    { { PDCP_CREATE_DRB_ENTITY                },   rrcTagCreateDrbEntity,              rrcTagCheckCreateDrbEntity, },
    { { PDCP_CONFIGURE_INTEGRITY_PROTECTION   },   rrcTagConfigureIntegrity,           rrcTagCheckConfigureIntegrity, },
    /* SPR_1235_FIX_START*/
    { { PDCP_CONFIGURE_SRB_CIPHERING          },   rrcTagConfigureSrbCiphering,        rrcTagCheckConfigureCiphering, },
    { { PDCP_CONFIGURE_DRB_CIPHERING          },   rrcTagConfigureDrbCiphering,        rrcTagCheckConfigureCiphering, },
    /* SPR_1235_FIX_END*/ 
    { { PDCP_DELETE_SRB_ENTITY                },   rrcTagDeleteEntity,                 rrcTagCheckDeleteEntity, },
    { { PDCP_DELETE_DRB_ENTITY                },   rrcTagDeleteEntity,                 rrcTagCheckDeleteEntity, },
    { { PDCP_RECONFIG_SRB_ENTITY              },   rrcTagReconfigSrbEntity,            rrcTagCheckReconfigSrbEntity, },
    { { PDCP_RECONFIG_DRB_ENTITY              },   rrcTagReconfigDrbEntity,            rrcTagCheckReconfigDrbEntity, },
    /* SPR 3607 Fix Start */
    { { PDCP_SECURITY_OFFSET                  },   rrcTagSecurityOffset,               rrcTagCheckSecurityOffset, },
    /* SPR 3607 Fix End */
#ifndef UE_SIM_TESTING
    { { PDCP_SPS_CONFIG                       },  rrcTagSpsConfig,                     PNULL  },
#endif
    /* SPR 17157 Fix start */
    { { RRC_PDCP_MACI_REQ_TAG                 },  rrcTagMaciReq,                       rrcTagCheckMaciReq,  },
    /* SPR 17157 Fix end */
    /* + SPR 19066 */
    { { RRC_PDCP_UE_INACTIVITY_TIMER_CONFIG_TAG },  rrcTagInactivityTimer,            rrcTagCheckInactivityTimer,  },
    /* - SPR 19066 */
    { { TLV_LAST_ID                           },   PNULL,                              PNULL, },
};

/* Nested tags handlers table for PDCP_CREATE_DRB_ENTITY tag */
TlvTagHandler rrcCreateDrbEntityTagsTable[] =
{
    { { PDCP_CONFIGURE_ROHC                   },   rrcTagConfigureRohc,                 rrcTagCheckConfigureRohc, },
    { { PDCP_CONFIGURE_DISCARD_TIMER          },   rrcTagConfigureDiscardTimer,         rrcTagCheckDiscardTimer, },
    { { PDCP_CONFIGURE_SN_SIZE                },   rrcTagConfigureSnSize,               rrcTagCheckSnSize, },
    { { PDCP_CONFIGURE_STATUS_REPORT_REQUIRED },   rrcTagConfigureStatusReportRequired, rrcTagCheckStatusReportRequired, },
    /* SPR 3608 changes start */
    { { PDCP_CONFIGURE_UE_PSR  		      },   rrcTagConfigureUePsr,   	   	rrcTagCheckConfigureUePsr,},
    /* SPR 3608 changes end */
    { { TLV_LAST_ID                           },   PNULL,                               PNULL, },
};

/* Nested tags handlers table for PDCP_RECONFIG_DRB_ENTITY tag */
TlvTagHandler rrcReconfigDrbEntityTagsTable[] =
{
    { { PDCP_CONFIGURE_ROHC                   },   rrcTagReConfigureRohc,               rrcTagCheckConfigureRohc, },
    { { PDCP_CONFIGURE_STATUS_REPORT_REQUIRED },   rrcTagConfigureStatusReportRequired, rrcTagCheckStatusReportRequired, },
    /* SPR 3608 changes start */
    { { PDCP_CONFIGURE_UE_PSR   	      },   rrcTagConfigureUePsr,                rrcTagCheckConfigureUePsr,},
    /* SPR 3608 changes end */
    { { TLV_LAST_ID                           },   PNULL,                               PNULL, },
};

/* Tags handlers table for PDCP_CREATE_UE_ENTITY_REQ RRC API command */
TlvTagHandler rrcComputeMacITagsTable[] =
{
    { { PDCP_MESSAGE          },   rrcTagMessageMacI,    rrcTagCheckMessageMacI, },
    { { TLV_LAST_ID           },   PNULL,                PNULL,                  },
};

/* Nested tags handlers table for PDCP_SN_HFN_STATUS_IND RRC API Command */
TlvTagHandler rrcSnHfnStatusTagsTable[] =
{
    { { PDCP_LC_SN_HFN_STATUS },   rrcTagSnHfnStatus,    rrcTagCheckSnHfnStatus, },
    { { TLV_LAST_ID           },   PNULL,                PNULL,                  },
};


TlvTagHandler rrcLcSnHfnTagsTable[] =
{
    { { PDCP_UL_RECV_SN       },   rrcTagUlRecvSn,       rrcTagCheckUlRecvSn,    },     
    { { TLV_LAST_ID           },   PNULL,                PNULL,                  },
};

/* Nested tags handlers table for PDCP_RE_ESTABLISH_UE_ENTITY_REQ RRC API */
TlvTagHandler rrcReEstablishUeEntityTagsTable[] =
{
    { { PDCP_RB_ENTITY  },  rrcTagReEstablishRbEntity,  rrcTagCheckRbEntity, },
    { { TLV_LAST_ID     },  PNULL,                      PNULL,               },
};

/* Nested tags handlers table for PDCP_RESUME_UE_ENTITY_REQ RRC API */
TlvTagHandler rrcResumeUeEntityTagsTable[] =
{
    { { PDCP_RB_ENTITY  },   rrcTagResumeRbEntity,   rrcTagCheckRbEntity,   },
    { { TLV_LAST_ID     },   PNULL,                  PNULL,                 },
};

/*COUNTER_CHK_START*/
TlvTagHandler rrcDrbCountMsbListTagsTable[] =
{
    { { PDCP_DRB_COUNT_MSB_LIST  },   rrcTagDrbCountMsbList,   rrcTagCheckDrbCountMsbList,},
    { { TLV_LAST_ID              },   PNULL,                   PNULL,                 },
};
/*COUNTER_CHK_END*/

#ifndef UE_SIM_TESTING
/* Nested tags handlers table for PDCP_SPS_DL_INFO RRC Tag */
TlvTagHandler rrcSpsConfigTagsTable[] =
{
    { { PDCP_SPS_DL_INFO    },  rrcTagSpsDlInfo,   rrcTagCheckSpsDlInfo  },
    { { PDCP_SPS_UL_INFO    },  rrcTagSpsUlInfo,   rrcTagCheckSpsUlInfo  },
    { { TLV_LAST_ID         },  PNULL,             PNULL,                },
};

/* Nested tags handlers table for PDCP_SPS_DL_INFO RRC Tag */
TlvTagHandler rrcSpsDlInfoTagsTable[] =
{
    { { PDCP_SPS_DL_SETUP_INFO },  rrcTagSpsDlSetupInfo, rrcTagCheckSpsDlSetupInfo },
    { { TLV_LAST_ID            },  PNULL,                PNULL,                    },
};

/* Nested tags handlers table for PDCP_SPS_UL_INFO RRC Tag */
TlvTagHandler rrcSpsUlInfoTagsTable[] =
{
    { { PDCP_SPS_UL_SETUP_INFO },  rrcTagSpsUlSetupInfo, rrcTagCheckSpsUlSetupInfo },
    { { TLV_LAST_ID            },  PNULL,                PNULL,                    },
};
#endif

/* Lossy HO changes start */
/* Nested tags handlers table for PDCP_LOSSY_HO_REQUIRED RRC API */
TlvTagHandler rrcLossyHoTagsTable[] =
{
    { { PDCP_LOSSY_HO_REQUIRED }, rrcTagLossyHoConfigure, rrcTagCheckLossyHoRequired, },
    { { TLV_LAST_ID     },        PNULL,                  PNULL,                 },
};

/* Lossy HO changes stop */
/* + SPR 19066 */
/* Tags handlers table for PDCP_HO_PREP_INFO_REQ RRC API command */
TlvTagHandler rrcHoPrepInfoTagsTable[] =
{
    { { RRC_PDCP_MACI_HO_REQ_TAG          },  rrcTagHoMaciReq,       rrcTagCheckHoMaciReq,       },
    { { RRC_PDCP_UE_INACTIVE_TIME_REQ_TAG },  rrcTagUeInactTimeReq,  rrcTagCheckUeInactTimeReq,  },
    { { TLV_LAST_ID                       },  PNULL,                 PNULL,                      },
};

/* Tags handlers table for PDCP_HO_TRIGGERED Tag */
TlvTagHandler rrcHoTriggerTagsTable[] =
{
    { { RRC_PDCP_UE_INACTIVE_TIME_LAPSED_TAG },  rrcTagInactTimeLapse,  rrcTagCheckInactiveTimeLapse, },
    { { TLV_LAST_ID                          },  PNULL,                 PNULL,                        },
};
/* - SPR 19066 */

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : rrcCmdCreateUeEntity 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes RRC PDCP_CREATE_UE_ENTITY_REQ API.
 *                  It creates set of PDCP Entities for the UE. As response
 *                  It sends PDCP_CREATE_UE_ENTITY_CNF message to RRC.
 ****************************************************************************/
void rrcCmdCreateUeEntity( UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }
    /* SPR 11109 Fix start */ 
    memSet(responseBuf, 0, PDCP_RRC_CNF_MAX_SIZE);
    /* SPR 11109 Fix end */ 

    /* Coverity 55830 */
    LP_TlvMessage  message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage  responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcCreateUeEntityReq     reqCreateUeEntity_p = PNULL;
    LP_RrcCreateUeCnf           createResponse_p = PNULL;
    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    InternalCellIndex  cellIndex = INVALID_CELL_INDEX;
#endif 
    /* - Layer2 NON CA Changes */

    /*Check API id */
    CHECK_EXP( PDCP_CREATE_UE_ENTITY_REQ != message_p->header.apiId );
    /* + Layer2 NON CA Changes */

    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            RRC_MODULE_ID,
            PDCP_CREATE_UE_ENTITY_CNF,
            sizeof(RrcCreateUeCnf)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif 
                );
    /* - Layer2 NON CA Changes */

    /* Prepare pointer to response message data */
    createResponse_p = (LP_RrcCreateUeCnf)responseMsg_p->data;

    /* Set default values in response message */
    createResponse_p->header.ueIndex = 0;
    createResponse_p->responseCode = PDCP_SUCCESS;

    do {
	    /* SES-526 Fix Start */
	    /* Prepare pointer to RRC header  */
	    reqCreateUeEntity_p = (LP_RrcCreateUeEntityReq)message_p->data;

	    /* Convert data fields from network to Host byte order */
	    CONVERT_INT16_NETWORK_TO_HOST( reqCreateUeEntity_p->header.ueIndex );
	    CONVERT_INT16_NETWORK_TO_HOST( reqCreateUeEntity_p->crnti );

	    /* save ueIndex in response message */
	    createResponse_p->header.ueIndex = reqCreateUeEntity_p->header.ueIndex;
	    /* SES-526 Fix End */
	    
            /* --- check message length */
	    if ( message_p->header.length != size )
	    {   /* invalid length -- reject it */
		    LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
				    pdcpCurrentTime_g, __LINE__, message_p->header.length, 
				    size, 0,0, 0,0, __func__,"PDCP_CREATE_UE_ENTITY_REQ");
		    createResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
		    break;
	    }

	    /* Check data size */
	    if ( message_p->header.length < PDCP_RRC_CREATE_UE_ENTITY_REQ_MIN_SIZE )
	    {
		    LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
				    pdcpCurrentTime_g, __LINE__, message_p->header.length, 
				    PDCP_RRC_CREATE_UE_ENTITY_REQ_MIN_SIZE, 0,0, 
				    0,0, __func__,"PDCP_CREATE_UE_ENTITY_REQ");
		    createResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
		    break;
	    }

#ifndef UE_SIM_TESTING
	    /* SPR 4558 Fix Start */
	    reqCreateUeEntity_p->header.ueIndex = getUeIdxFromRrcUeIdx(reqCreateUeEntity_p->header.ueIndex);
	    /* SPR 4558 Fix End */
#endif
	    /* check it hear for making response with valid ueIndex */
	    if ( !pdcpContextIsInitialized() )
	    {   /* PDCP isn't initialized */
		    LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
				    pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"" );
		    createResponse_p->responseCode = PDCP_ERR_CONTEXT_NOT_INITIALIZED;
		    break;
	    }

	    /* parse and check message optional parameters */
	    createResponse_p->responseCode = tlvCheckTags(
			    rrcCreateUeEntityTagsTable,
			    &message_p->data[sizeof(RrcCreateUeEntityReq)], /* pointer to the optional parameters */
			    TLV_GET_OPTIONAL_PART_SIZE( message_p, sizeof(RrcCreateUeEntityReq) ) /* size of optional parameters */
			    );
	    if ( PDCP_SUCCESS != createResponse_p->responseCode )
	    {
		    /* message parsing error */
		    break;
	    }

	    /***** Create UE context */
	    /* Check crnti, ueIndex RRC header fields shell be checked inside pdcpUeContextCreate */
	    createResponse_p->responseCode = pdcpUeContextCreate( 
			    reqCreateUeEntity_p->header.ueIndex, 
			    reqCreateUeEntity_p->crnti );
	    if ( PDCP_SUCCESS != createResponse_p->responseCode )
	    {
		    break;
	    }
#ifndef UE_SIM_TESTING

	    pdcpContext.ueContexts[reqCreateUeEntity_p->header.ueIndex]->rrcUeIndex  = createResponse_p->header.ueIndex;
	    /* + Layer2 NON CA Changes */
	    cellIndex = PDCP_GET_INTERNAL_CELL_INDEX( message_p->header.cellIndex );

	    pdcpContext.ueContexts[reqCreateUeEntity_p->header.ueIndex]->cellIndex =
		    cellIndex;

	    /* - Layer2 NON CA Changes */
	    /* SPR 4864 fix start */
	    setUeIdxValid(reqCreateUeEntity_p->header.ueIndex, UE_IDX_MAPPING_FOR_PDCP);
	    /* SPR 4864 fix end */

#endif
	    /***** Process optional parameters */
	    createResponse_p->responseCode = tlvProcessTags(
			    rrcCreateUeEntityTagsTable,
			    &reqCreateUeEntity_p->header,
			    &message_p->data[sizeof(RrcCreateUeEntityReq)], /* pointer to the optional parameters */
			    TLV_GET_OPTIONAL_PART_SIZE( message_p, sizeof(RrcCreateUeEntityReq) ), /* size of optional parameters */
			    responseMsg_p);

    } while (0);

    /* check if PDCP_PARTIAL_SUCCESS response code should be sent */
    if ( ( PDCP_SUCCESS == createResponse_p->responseCode ) &&
/* SPR 17157 Fix start */
            (( sizeof(TlvHeader) + sizeof(RrcCreateUeCnf) )  != responseMsg_p->header.length ) &&
            (( sizeof(TlvHeader) + sizeof(RrcCreateUeCnf) + sizeof(TlvTagHeader) + MACI_LEN) != responseMsg_p->header.length))
/* SPR 17157 Fix end */
    {
        createResponse_p->responseCode = PDCP_PARTIAL_SUCCESS;
    }

    if ( (PDCP_SUCCESS != createResponse_p->responseCode) && 
            (PDCP_PARTIAL_SUCCESS != createResponse_p->responseCode) )
    {
        LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__,createResponse_p->responseCode, 
                 createResponse_p->header.ueIndex,0,0, 0,0,__func__,
                 "PDCP_CREATE_UE_ENTITY_REQ" );
        createResponse_p->responseCode = PDCP_FAIL;
    }

    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( createResponse_p->header.ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( createResponse_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
    );
    
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}


/****************************************************************************
 * Function Name  : rrcTagCheckCreateSrbEntity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_CREATE_SRB_ENTITY RRC tag and converts 
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckCreateSrbEntity( UInt8* data_p, UInt16 size )
{
    UInt16                          result = PDCP_SUCCESS;

    /* SPR_1024_FIX_START */
    do {
    /* SPR_1024_FIX_END */
        /* check length */
        if ( sizeof(RrcTagCreateSrbEntity) > size )
        {
            /* TLV parsing error stop processing */
            /* +- SPR 17777 */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, size, 
                     sizeof(RrcTagCreateSrbEntity),sizeof(data_p),0, 0,0, __func__,
                     "PDCP_CREATE_SRB_ENTITY");
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;            
            /* SPR_1024_FIX_START */
            break;
            /* SPR_1024_FIX_END */
        }

   /* SPR_1024_FIX_START */    
    } while (0);
    /* SPR_1024_FIX_END */
    /* SPR_1024_FIX_END */
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCreateSrbEntity 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcHeader structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CREATE_SRB_ENTITY RRC tag.
 *                  It creates PDCP Entities for the selected logical channel
 *                  within UE context. As response It creates
 *                  PDCP_CREATE_ENTITY_ERROR tag in response message buffer.
 ****************************************************************************/
UInt16 rrcTagCreateSrbEntity( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size,  LP_TlvMessage responseMessage)
{   
    UInt16                          result      = PDCP_SUCCESS;
    RrcTagParams                    tagPrams;
    LP_RrcTagCreateSrbEntity        tagCreateSrb_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagCreateSrbEntity           tagCreateSrb_data = {0};
    /* ALIGNMENT changes end */

    tagPrams.ueIndex = ((LP_RrcHeader)params)->ueIndex;
    tagPrams.lcId    = 0;

    /* SPR_1024_FIX_START */
    do {
        /* SPR_1024_FIX_END */
        /* Tag length is checked by rrcTagCheckCreateSrbEntity()*/

    
        /* ALIGNMENT changes start */
        tagCreateSrb_p = &tagCreateSrb_data;
        READ_API_MSG(RrcTagCreateSrbEntity, tagCreateSrb_p, data_p); 
        /* ALIGNMENT changes end */
        /* Tag data fields are converted to Host byte order by rrcTagCheckCreateSrbEntity() */

        /* check all tag fields */
        /* SPR 3276 changes start */
        if ( (LTE_SRB_LC_ID_LOW_BOUND > tagCreateSrb_p->srbId) ||
                (LTE_SRB_LC_ID_HIGH_BOUND < tagCreateSrb_p->srbId) )
            /* SPR 3276 changes end */
        {
            LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,tagCreateSrb_p->srbId, tagPrams.ueIndex, 0,0, 
                    0,0,__func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
            /* SPR_1024_FIX_START */
            break;
            /* SPR_1024_FIX_END */
        }

        if ( PDCP_ENTITY_RLC_AM < tagCreateSrb_p->rlcMode )
        {
            LOG_PDCP_MSG (PDCP_INVALID_RLC_MODE, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, tagCreateSrb_p->rlcMode,
                    tagPrams.ueIndex, tagCreateSrb_p->srbId, 0, 0,0, 
                    __func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
            /* SPR_1024_FIX_START */
            break;
            /* SPR_1024_FIX_END */
        }

        if ( PDCP_ENTITY_RB_BOTH < tagCreateSrb_p->rbDirection )
        {
            LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, tagCreateSrb_p->rbDirection, tagPrams.ueIndex,
                    tagCreateSrb_p->srbId, 0, 0,0, __func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
            /* SPR_1024_FIX_START */
            break;
            /* SPR_1024_FIX_END */
        }

        /* store channel id and direction*/
        tagPrams.lcId = tagCreateSrb_p->srbId;
        tagPrams.rbDirection = tagCreateSrb_p->rbDirection;

        LOG_PDCP_MSG (PDCP_CONTEXT_PARAMS, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, tagPrams.ueIndex, tagPrams.lcId,
                tagCreateSrb_p->rbDirection, tagCreateSrb_p->rlcMode,  
                0,0,__func__,"" );
        /* Create entity */
        result = pdcpEntityCreate( 
                tagPrams.ueIndex, 
                tagPrams.lcId, 
                PDCP_ENTITY_SRB, 
                tagCreateSrb_p->rlcMode, 
                tagCreateSrb_p->rbDirection,
                /* SPR 3276 changes start */
                tagCreateSrb_p->srbId);
        /* SPR 3276 changes end */
        /* SPR_1024_FIX_START */
        if ( PDCP_SUCCESS != result )
        {
            break;
        } 
        /* SPR_1024_FIX_END */

        /* SPR_1235_FIX_START*/
        pdcpEntitySetDirectionFlag(
                tagPrams.ueIndex, 
                tagPrams.lcId,
                tagCreateSrb_p->rbDirection,
                PDCP_ENTITY_SRB );
        /* SPR_1235_FIX_END*/
        /* SPR_1024_FIX_START */

#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        LteUePdcpRadioBearerInfoStats *ueRadioBearerInfoStats_p =
            &(gPerfStats.perfStats[0].lteUePerfStats[tagPrams.ueIndex].
                    lteUePdcpRadioBearerInfoStats[tagPrams.lcId]);
        LteUePdcpRadioBearerInfoStats *ueRadioBearerInfoStatsAtFirstIndex_p =
            &(gPerfStats.perfStats[1].lteUePerfStats[tagPrams.ueIndex].
                    lteUePdcpRadioBearerInfoStats[tagPrams.lcId]); 

        ueRadioBearerInfoStats_p->bearerId =  tagCreateSrb_p->srbId;
        ueRadioBearerInfoStatsAtFirstIndex_p->bearerId =  tagCreateSrb_p->srbId;
        ueRadioBearerInfoStats_p->bearerType = EVENT_VALUE_SRB;
        ueRadioBearerInfoStatsAtFirstIndex_p->bearerType = EVENT_VALUE_SRB;
        if (PDCP_ENTITY_RLC_UM ==  tagCreateSrb_p->rlcMode)
        {
            ueRadioBearerInfoStats_p->rlcMode = EVENT_VALUE_UNACK_MODE;
            ueRadioBearerInfoStatsAtFirstIndex_p->rlcMode = EVENT_VALUE_UNACK_MODE;
        }
        else
        {
            ueRadioBearerInfoStats_p->rlcMode = EVENT_VALUE_ACK_MODE;
            ueRadioBearerInfoStatsAtFirstIndex_p->rlcMode = EVENT_VALUE_ACK_MODE;
        } 
#endif

    } while (0);
    /* SPR_1024_FIX_END */

    if ( PDCP_SUCCESS != result )
    {
        /* COVERITY 10051 : code removed */
        /* make createSrbEntityError tag and update response message */
        rrcMakeTagError( PDCP_CREATE_SRB_ENTITY_ERROR, tagCreateSrb_p->srbId,
                result, responseMessage ); 

        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,result, tagPrams.ueIndex,size,tagId,
                responseMessage->header.transactionId,0,
                __func__,"PDCP_CREATE_SRB_ENTITY" );
    }

    /* Don't interrupt tag processing */
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagCheckConfigureIntegrity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_CONFIGURE_INTEGRITY_PROTECTION RRC tag and
 *                  converts mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckConfigureIntegrity( UInt8* data_p, UInt16 size )
{
    UInt16                        result      = PDCP_SUCCESS;
    /* check length */
    if ( sizeof(RrcTagConfigureIntegrity) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, 
                 sizeof(RrcTagConfigureIntegrity),sizeof(data_p),0, 0,0, __func__,
                 "PDCP_CONFIGURE_INTEGRITY_PROTECTION");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagConfigureIntegrity 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CONFIGURE_INTEGRITY_PROTECTION RRC tag.
 ****************************************************************************/
UInt16 rrcTagConfigureIntegrity( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                        result      = PDCP_SUCCESS;
    LP_RrcTagConfigureIntegrity   tagConfigureIntegrity_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagConfigureIntegrity      tagConfigureIntegrity_data;
    /* ALIGNMENT changes end */
    LP_RrcTagParams               tagPrams_p = PNULL;
    UInt16                        lcCtr = 0;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Tag length is checked by rrcTagCheckConfigureIntegrity()*/
    /* Tag data fields are converted to Host byte order by
       rrcTagCheckConfigureIntegrity() */

    /* Prepare pointer to tag data */
    
    /* ALIGNMENT changes start */
    tagConfigureIntegrity_p = &tagConfigureIntegrity_data;
    READ_API_MSG(RrcTagConfigureIntegrity, tagConfigureIntegrity_p, data_p);
    /* ALIGNMENT changes end */

    /* check all tag fields */
    if ( ALGO_EIA2 < tagConfigureIntegrity_p->AlgorithmId )
    {
        LOG_PDCP_MSG (PDCP_UNKNOWN_ALGO_ID, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, tagConfigureIntegrity_p->AlgorithmId, 
                 0, tagPrams_p->ueIndex,0, 0,0, __func__,
                 "PDCP_CONFIGURE_INTEGRITY_PROTECTION");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
    }
    else 
    {
        LOG_PDCP_MSG (PDCP_INTEGRITY_PARAMS, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, tagPrams_p->ueIndex, tagConfigureIntegrity_p->AlgorithmId,
                 0,0, 0,0,__func__, pdcp_change_data_to_str(16, tagConfigureIntegrity_p->Key));
        /* SPR_1235_FIX_START*/
        for ( lcCtr = LTE_SRB_LC_ID_LOW_BOUND;
              lcCtr <= LTE_SRB_LC_ID_HIGH_BOUND; lcCtr++)
        {
            /* configure entity */
            pdcpEntityConfigureIntegrity(
                    tagPrams_p->ueIndex,
                    lcCtr, 
                    tagConfigureIntegrity_p->AlgorithmId,
                    tagConfigureIntegrity_p->Key,
                    /*SPR 10446 fix start */
                    PDCP_SECURITY_OFFSET_ENABLED
                    /*SPR 10446 fix end */ );
            /* No optional parameters. */
        }
        /* SPR_1235_FIX_END*/
    }

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                 "PDCP_CONFIGURE_INTEGRITY_PROTECTION" );
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcCmdDeleteUeEntity 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes RRC PDCP_DELETE_UE_ENTITY_REQ API.
 *                  It deletes all of PDCP Entities for the UE.
 *                  As response It sends PDCP_DELETE_UE_ENTITY_CNF message to RRC.
 ****************************************************************************/
void rrcCmdDeleteUeEntity( UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }
    /* SPR 11109 Fix start */ 
    memSet(responseBuf, 0,PDCP_RRC_CNF_MAX_SIZE);
    /* SPR 11109 Fix end */ 

    /* Coverity 55830 */
    LP_TlvMessage              message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcDeleteUeEntityReq    reqDeleteUeEntity_p = PNULL;
    LP_RrcDeleteUeCnf          deleteResponse_p = PNULL;

    /*Check API id */
    CHECK_EXP( PDCP_DELETE_UE_ENTITY_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            RRC_MODULE_ID,
            PDCP_DELETE_UE_ENTITY_CNF,
            sizeof(RrcDeleteUeCnf)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif
            );
    /* - Layer2 NON CA Changes */

    /* Prepare pointer to response message data */
    deleteResponse_p = (LP_RrcDeleteUeCnf)responseMsg_p->data;

    /* Set default values in response message */
    deleteResponse_p->header.ueIndex = 0;
    deleteResponse_p->responseCode = PDCP_SUCCESS;

    do {
        /* SES-526 Fix Start */ 
        /* Prepare pointer to RRC header  */
        reqDeleteUeEntity_p = (LP_RrcDeleteUeEntityReq)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( reqDeleteUeEntity_p->header.ueIndex );

        /* save ueIndex in response message */
        deleteResponse_p->header.ueIndex = reqDeleteUeEntity_p->header.ueIndex;
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, message_p->header.length, 
                     PDCP_RRC_DELETE_UE_ENTITY_REQ_MIN_SIZE, 0,0, 
                     0,0, __func__,"PDCP_DELETE_UE_ENTITY_REQ");
            deleteResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /* Check data size */
        if ( message_p->header.length < PDCP_RRC_DELETE_UE_ENTITY_REQ_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, message_p->header.length, 
                     PDCP_RRC_DELETE_UE_ENTITY_REQ_MIN_SIZE, 0,0, 
                     0,0, __func__,"PDCP_DELETE_UE_ENTITY_REQ");
            deleteResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

#ifndef UE_SIM_TESTING
         /* SPR 4558 Fix Start */
        reqDeleteUeEntity_p->header.ueIndex = getUeIdxFromRrcUeIdx(reqDeleteUeEntity_p->header.ueIndex); 
        /* SPR 4558 Fix End */
#endif

        /* check it hear for making response with valid ueIndex */
        if ( !pdcpContextIsInitialized() )
        {   /* PDCP isn't initialized */
            LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"" );
            deleteResponse_p->responseCode = PDCP_ERR_CONTEXT_NOT_INITIALIZED;
            break;
        }

        /***** Delete UE context */
        /* Check ueIndex RRC header field shell be checked inside pdcpUeContextDelete */
        deleteResponse_p->responseCode 
            = pdcpUeContextDelete(reqDeleteUeEntity_p->header.ueIndex);

    } while (0);

    if ( PDCP_SUCCESS != deleteResponse_p->responseCode )
    {
	    /* +Coverity 243564 */
		    LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
				    __LINE__, deleteResponse_p->responseCode, 
				    deleteResponse_p->header.ueIndex, 0,0, 0,0,__func__,
				    "PDCP_DELETE_UE_ENTITY_REQ" );
	    /* -Coverity 243564 */
	    deleteResponse_p->responseCode = PDCP_FAIL;
    }

    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( deleteResponse_p->header.ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( deleteResponse_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage(responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
    );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : rrcCmdSrbDataReq 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes RRC PDCP_SRB_DATA_REQ API.
 *                  It processes Data request from RRC for the UE SRB.
 *                  It don't send CNF message to RRC.
 ****************************************************************************/
void rrcCmdSrbDataReq( UInt8* data_p, UInt16 size )
{
    /* +Coverity 83271 */
    LP_TlvMessage        message_p = PNULL;
    /* -Coverity 83271 */
    LP_RrcSrbDataReq     reqHeader_p = PNULL;
    LP_PdcpSrbDataReq    txDataReq_p = PNULL;
    UInt16               response = PDCP_SUCCESS;
    UInt8                lcId = 0;
    UInt16               ueIndex = 0;
    UInt32               transactionId = 0;
    UInt8                serviceReqested = LTE_TRUE;
    UInt16               responseCode = PDCP_SUCCESS;
    LP_PdcpEnityContext  ctx_p = PNULL;        
    /* SPR 3538 changes start */
    UInt32               dataSize = 0;
    /* SPR 3538 changes end */

    /* +Coverity 83271 */
    if(sizeof(TlvMessage) <= size)
    {
        message_p = (LP_TlvMessage)data_p;
    }
    else
    {
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"Invalid size, too short msg" );
        return;
    }
    /* -Coverity 83271 */

    /* Check API id */
    CHECK_EXP( PDCP_SRB_DATA_REQ != message_p->header.apiId );

    do {
        /* SES-526 Fix Start */
        /* Prepare pointer to RRC header  */
        reqHeader_p = (LP_RrcSrbDataReq)message_p->data;

        /* Convert data fields from network to Host byte order */
        transactionId = PDCP_NTOHL( reqHeader_p->transactionId );
        ueIndex = PDCP_NTOHS( reqHeader_p->ueIndex );
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, message_p->header.length, 
                    PDCP_RRC_PDCP_SRB_DATA_REQ_MIN_SIZE, 0,0, 
                    0,0, __func__,"PDCP_SRB_DATA_REQ");
            response = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /* Check data size */
        if ( message_p->header.length < PDCP_RRC_PDCP_SRB_DATA_REQ_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, message_p->header.length, 
                    PDCP_RRC_PDCP_SRB_DATA_REQ_MIN_SIZE, 0,0, 
                    0,0, __func__,"PDCP_SRB_DATA_REQ");
            response = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

#ifndef UE_SIM_TESTING
        /* SPR 4558 Fix Start */
        ueIndex = getUeIdxFromRrcUeIdx(ueIndex);
        /* SPR 4558 Fix End */
#endif
        lcId = reqHeader_p->lcId;
        serviceReqested = reqHeader_p->serviceReqested; 

        if ( !pdcpContextIsInitialized() )
        {   /* PDCP isn't initialized */
            LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"" );
            response = PDCP_ERR_CONTEXT_NOT_INITIALIZED;
            break;
        }

        /* Check RRC header fields */
        if ( pdcpContextGetMaxUeNumber() <= ueIndex )
        {
            LOG_PDCP_MSG (PDCP_INVALID_UE, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, ueIndex, 
                    pdcpContext.isInitialized, 0,0, 0,0,__func__,"" );
            response = PDCP_ERR_TLV_PARSING_INVALID_UE_ID;
            break;
        }

        if ( (LTE_SRB_LC_ID_LOW_BOUND > lcId) ||
                (LTE_SRB_LC_ID_HIGH_BOUND < lcId) )
        {
            LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,lcId, ueIndex, 0,0, 0,0,__func__,"" );
            response = PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
            break;
        }

        /* SPR 3538 changes start */
        dataSize = TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                sizeof(RrcSrbDataReq) );
        if ( (0 == dataSize) || (PDCP_TS_SDU_MAX_SIZE < dataSize) )
        {
            LOG_PDCP_MSG( PDCP_INVALID_DATA_SIZE, LOGWARNING, PDCP_TX, 
                    pdcpCurrentTime_g, __LINE__,  dataSize, 0, ueIndex, lcId,
                    0,0, __func__,"");
            response = PDCP_ERR_INVALID_DATA_SIZE;
            break;
        }
        /* SPR 3538 changes end */

        /***** Process SRB Data request */
        /* create TX request */
        txDataReq_p = pdcpAllocateSrbTxDataReq( ueIndex, lcId, transactionId,
                PDCP_INVALID_SN, dataSize, serviceReqested );
        if ( PNULL == txDataReq_p )
        {
            response = PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
            LOG_PDCP_MSG( PDCP_GET_MEM_FROM_POOL_FAILED, LOGFATAL, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__,  sizeof(PdcpSrbDataReq), ueIndex,
                    lcId, 0, 0,0, __func__, "txDataReq_p is NULL");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic("MemAlloc fails for PdcpSrbDataReq\n");
            break;
        }   

        /* create ZCB for message data */
        txDataReq_p->data_p = msgAlloc( pdcpContextGetMsgPool(), 0, 0, 0 );
        if ( PNULL == txDataReq_p->data_p )
        {
            /* free memory */
            LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_RRCOAM, 
                    pdcpCurrentTime_g,__LINE__, 0, ueIndex, lcId, 0, 0,0,
                    __func__, "PdcpSrbDataReq");
            ALARM_MSG( PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            pdcpFreeSrbTxDataReq( txDataReq_p );
            /*SPR 2498 changes start*/
            txDataReq_p = PNULL;
            /*SPR 2498 changes end*/                   
            response = PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
            ltePanic("msgAlloc failed in %s",__FUNCTION__);
            break;
        }
        if ( PNULL == msgInsert( 
                    txDataReq_p->data_p, 0, 
                    /* get pointer to data */
                    &message_p->data[sizeof(RrcSrbDataReq)],  
                    /* get data size */
                    dataSize) )
        {
            /* free memory */
            pdcpFreeSrbTxDataReq( txDataReq_p );
            /*SPR 2498 changes start*/
            txDataReq_p = PNULL;
            /*SPR 2498 changes end*/                   
            response = PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
            LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, dataSize, ueIndex, lcId, 0, 
                    0,0, __func__, "PdcpSrbDataReq cannot be inserted to ZCB");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, 
                    CRITICAL_ALARM);
            ltePanic("msgInsert failed in %s",__FUNCTION__);
            break;
        }
        /* Get PDCP Context */
        ctx_p = pdcpGetEntityContext(ueIndex, lcId, PDCP_ENTITY_RB_TX);
        if(PNULL == ctx_p) {
            /* free memory */
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 0,0, 0,0, 
                    __func__,"");
            response = PDCP_ERR_ENTITY_NOT_FOUND;
            pdcpFreeSrbTxDataReq(txDataReq_p);
            /*SPR 2498 changes start*/
            txDataReq_p = PNULL;
            /*SPR 2498 changes end*/                   
            break;
        }

        /* Associate discard timer for freshly arrived SDU, if configured */
        if ( 0 != ctx_p->discardTimer )
        {
            txDataReq_p->tickAssociated = pdcpCurrentTime_g;
            LOG_PDCP_MSG( PDCP_DISCARD_TIMER_ASSOCIATED, LOGDEBUG, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
                    txDataReq_p->tickAssociated
 			        , ctx_p->discardTimer, 0,0,
                    __func__,"");

        }
        else
        {
            LOG_PDCP_MSG( PDCP_NO_DISCARD_TIMER_ASSOCIATED, LOGDEBUG, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
                    ctx_p->discardTimer, 0,0,0, __func__, "");
        }

        /*Update UE stats when data received from RRC*/
        pdcpStats.statsUEs[ueIndex].srbs[lcId - LTE_SRB_LC_ID_LOW_BOUND].
            numberOfDlSrbSduReceived += 1;

        LOG_PDCP_MSG( PDCP_SDU_ENQUEUED_FROM_RRC, LOGINFO, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, ueIndex, lcId, 
                txDataReq_p->srbServiceReqested , 0,0,0, __func__, "");
        /* Handle Srb Data Request */
        ENQUEUE_PDCP_TX_DATA_REQ_Q(ctx_p, txDataReq_p);
#ifdef L2_FAST_PATH
        pdcpIsSrbDataReceivedInTxQ_g = LTE_TRUE;
#endif
    } while (0);

    /* response generation */
    if ( PDCP_SUCCESS != response )
    {
        LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, response, ueIndex, 0,0, 0,0,__func__, "PDCP_SRB_DATA_REQ" );
        responseCode = PDCP_FAIL;

        LOG_PDCP_MSG (PDCP_STATUS_IND_RRC, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, ueIndex, lcId, serviceReqested, response, 
                0,0, __func__, "");
        if ( LTE_TRUE == serviceReqested )
        {
            /* SPR 1825 changes start */
            /* SPR 4772 changes start */
            pdcpRrcSrbDataStatusInd( message_p->header.transactionId, 
                    ueIndex, lcId, transactionId, responseCode, response,
                    &message_p->data[sizeof(RrcSrbDataReq)], LTE_FALSE,  
                    TLV_GET_OPTIONAL_PART_SIZE( message_p, sizeof(RrcSrbDataReq)) );
            /* SPR 4772 changes end */
            /* SPR 1825 changes end */
        }
    }
}

/****************************************************************************
 * Function Name  : rrcTagCheckCreateDrbEntity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_CREATE_DRB_ENTITY RRC tag and converts
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckCreateDrbEntity( UInt8* data_p, UInt16 size )
{
    UInt16                      result = PDCP_SUCCESS;

    do {
        /* check length */
        if ( sizeof(RrcTagCreateDrbEntity) > size )
        {
            /* TLV parsing error stop processing */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, size, 
                     sizeof(RrcTagCreateDrbEntity), 0,0, 0,0, __func__,
                     "PDCP_CREATE_DRB_ENTITY");
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
            break;
        }

        /* parse and check tag optional parameters */
        result = tlvCheckTags( 
                rrcCreateDrbEntityTagsTable,
                &data_p[sizeof(RrcTagCreateDrbEntity)], /* pointer to the optional parameters */
                (size - sizeof(RrcTagCreateDrbEntity))  /* size of optional parameters */
                );
        if ( PDCP_SUCCESS != result )
        {
            /* message parsing error */
            break;
        }    

    } while (0);

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCreateDrbEntity 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcHeader structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CREATE_DRB_ENTITY RRC tag.
 *                  It creates PDCP Entities for the selected logical channel
 *                  within UE context. As response It creates 
 *                  PDCP_CREATE_ENTITY_ERROR tag in response message buffer.
 ****************************************************************************/
UInt16 rrcTagCreateDrbEntity( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size,  LP_TlvMessage responseMessage)
{   
    UInt16                          result      = PDCP_SUCCESS;
    RrcTagParams                    tagPrams;
    LP_RrcTagCreateDrbEntity        tagCreateDrb_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagCreateDrbEntity           tagCreateDrb_data = {0};
    /* ALIGNMENT changes end */
    UInt16                          deleteEntity = LTE_FALSE;

    tagPrams.ueIndex = ((LP_RrcHeader)params)->ueIndex;
    tagPrams.lcId    = 0;

    do {
        /* Tag length is checked by rrcTagCheckCreateDrbEntity()*/

        /* Prepare pointer to tag data */
        /* ALIGNMENT changes start */
        tagCreateDrb_p = &tagCreateDrb_data;
        READ_API_MSG(RrcTagCreateDrbEntity, tagCreateDrb_p, data_p);
        /* ALIGNMENT changes end */

        /* Tag data fields are converted to Host byte order by rrcTagCheckCreateDrbEntity() */

        /* check all tag fields */
        if ( (LTE_DRB_LC_ID_LOW_BOUND > tagCreateDrb_p->lcId) ||
                (LTE_DRB_LC_ID_HIGH_BOUND < tagCreateDrb_p->lcId) )
        {
            LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,tagCreateDrb_p->lcId, tagPrams.ueIndex ,0,0, 0,0,__func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
            break;
        }
        if ( PDCP_ENTITY_RLC_AM < tagCreateDrb_p->rlcMode )
        {
            LOG_PDCP_MSG (PDCP_INVALID_RLC_MODE, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, tagCreateDrb_p->rlcMode,
                    tagPrams.ueIndex, tagCreateDrb_p->lcId, 0, 0,0, __func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
            break;
        }
        if ( PDCP_ENTITY_RB_BOTH < tagCreateDrb_p->rbDirection )
        {
            LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, tagCreateDrb_p->rbDirection, tagPrams.ueIndex, 
                    tagCreateDrb_p->lcId, 0, 0,0, __func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
            break;
        }

#ifndef UE_SIM_TESTING
        if  (tagCreateDrb_p->qci <= 0 ||  tagCreateDrb_p->qci > MAX_QCI) 
        {
            LOG_PDCP_MSG (PDCP_INVALID_QCI, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, tagCreateDrb_p->qci, tagPrams.ueIndex, 
                    tagCreateDrb_p->lcId, 0, 0,0, __func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
            break;
        }
#endif        

        /* SPR 3276 changes start */
        if ( (LTE_MIN_DRB_ID_RANGE > tagCreateDrb_p->radioBearer) ||
                (LTE_MAX_DRB_ID_RANGE < tagCreateDrb_p->radioBearer) )
        {
            LOG_PDCP_MSG( PDCP_INVALID_RB, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, tagCreateDrb_p->radioBearer, 
                    tagPrams.ueIndex ,0,0, 0,0,__func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_RB_ID;
            break;
        }
        /* SPR 3276 changes end */

        /* store channel id and direction*/
        tagPrams.lcId = tagCreateDrb_p->lcId;
        tagPrams.rbDirection = tagCreateDrb_p->rbDirection;
#ifndef UE_SIM_TESTING
        tagPrams.qci = tagCreateDrb_p->qci;
#endif        

        LOG_PDCP_MSG (PDCP_CONTEXT_PARAMS, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, tagPrams.ueIndex, tagPrams.lcId,
                tagCreateDrb_p->rbDirection, tagCreateDrb_p->rlcMode,  
                0,0,__func__,"" );
        /* Create entity */
        result = pdcpEntityCreate( 
                tagPrams.ueIndex, 
                tagPrams.lcId, 
                PDCP_ENTITY_DRB, 
                tagCreateDrb_p->rlcMode, 
                tagCreateDrb_p->rbDirection,
                /* SPR 3276 changes start */
                tagCreateDrb_p->radioBearer);
        /* SPR 3276 changes end */

        if ( PDCP_SUCCESS != result )
        {
            break;
        }
        deleteEntity = LTE_TRUE;
#ifndef UE_SIM_TESTING
        /* SPR 3569 changes start */
        result = pdcpEntityQci(
                tagPrams.ueIndex,
                tagPrams.lcId,
                tagPrams.rbDirection,
                tagPrams.qci);
        if ( PDCP_SUCCESS != result )
        {
            break;
        }
        /* SPR 3569 changes end */
#endif
        /* SPR_1235_FIX_START*/
        pdcpEntitySetDirectionFlag(
                tagPrams.ueIndex, 
                tagPrams.lcId,
                tagCreateDrb_p->rbDirection,
                PDCP_ENTITY_DRB );
        /* SPR_1235_FIX_END*/

        /***** Process tag optional parameters (nested tags)*/
        result = tlvProcessTags( 
                rrcCreateDrbEntityTagsTable,
                &tagPrams,
                &data_p[sizeof(RrcTagCreateDrbEntity)], /* pointer to the optional parameters */
                (size - sizeof(RrcTagCreateDrbEntity)), /* size of optional parameters */
                responseMessage
                );
        if ( PDCP_SUCCESS != result )
        {
            break;
        }

#if  defined(PERF_STATS) && !defined(UE_SIM_TESTING)
        LteUePdcpRadioBearerInfoStats *ueRadioBearerInfoStats_p =
            &(gPerfStats.perfStats[0].lteUePerfStats[tagPrams.ueIndex].
                    lteUePdcpRadioBearerInfoStats[tagPrams.lcId]);

        LteUePdcpRadioBearerInfoStats *ueRadioBearerInfoStatsAtFirstIndex_p =
            &(gPerfStats.perfStats[1].lteUePerfStats[tagPrams.ueIndex].
                    lteUePdcpRadioBearerInfoStats[tagPrams.lcId]);

        ueRadioBearerInfoStats_p->bearerId =  tagCreateDrb_p->radioBearer;
        ueRadioBearerInfoStatsAtFirstIndex_p->bearerId =  tagCreateDrb_p->radioBearer;
        ueRadioBearerInfoStats_p->bearerType = EVENT_VALUE_DRB;
        ueRadioBearerInfoStatsAtFirstIndex_p->bearerType = EVENT_VALUE_DRB;
        ueRadioBearerInfoStats_p->qci = tagPrams.qci;
        ueRadioBearerInfoStatsAtFirstIndex_p->qci = tagPrams.qci;
        if (PDCP_ENTITY_RLC_UM ==  tagCreateDrb_p->rlcMode)
        {
            ueRadioBearerInfoStats_p->rlcMode = EVENT_VALUE_UNACK_MODE;
            ueRadioBearerInfoStatsAtFirstIndex_p->rlcMode = EVENT_VALUE_UNACK_MODE;
        }
        else
        {
            ueRadioBearerInfoStats_p->rlcMode = EVENT_VALUE_ACK_MODE;
            ueRadioBearerInfoStatsAtFirstIndex_p->rlcMode = EVENT_VALUE_ACK_MODE;
        } 
#endif    

    } while (0);

    if ( PDCP_SUCCESS != result )
    {
        /* We need to delete created Entities here */
        if ( LTE_TRUE == deleteEntity )
        {
            /*coverity-530-25462*/
            if( PDCP_SUCCESS != pdcpDrbEntityFree( tagPrams.ueIndex, tagCreateDrb_p->lcId ))
            {
                LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__, result, tagPrams.ueIndex, 0,0, 0,0, __func__,
                    "PDCP_DELETE_ENTITY" );
            }
            /*coverity-530-25462*/
        }

        /* make createDrbEntityError tag and update response message */
        rrcMakeTagError( PDCP_CREATE_DRB_ENTITY_ERROR,
                tagCreateDrb_p->lcId, result, responseMessage );

        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, result, tagPrams.ueIndex,tagId,0, 0,0,__func__,
                "PDCP_CREATE_DRB_ENTITY" );
    }

    /* Don't interrupt tag processing */
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagCheckDiscardTimer 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_CONFIGURE_DISCARD_TIMER RRC tag and 
 *                  converts mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckDiscardTimer( UInt8* data_p, UInt16 size )
{
    UInt16                           result      = PDCP_SUCCESS;
    /* check length */
    if ( sizeof(RrcTagConfigureDiscardTimer) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, 
                 sizeof(RrcTagConfigureDiscardTimer),sizeof(data_p),0, 0,0, __func__,
                 "PDCP_CONFIGURE_DISCARD_TIMER");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    else 
    {
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagConfigureDiscardTimer 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CONFIGURE_DISCARD_TIMER RRC tag.
 ****************************************************************************/
UInt16 rrcTagConfigureDiscardTimer( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                           result      = PDCP_SUCCESS;
    LP_RrcTagConfigureDiscardTimer   tagConfigureDiscardTimer_p = PNULL;
    RrcTagConfigureDiscardTimer      tagConfigureDiscardTimer_data;
    LP_RrcTagParams                  tagPrams_p = PNULL;

    static const UInt16 allowedDiscardTimerValues[] = {
        Infinity, ms50, ms100, ms150, ms300, ms500, ms750, ms1500 };
    UInt16  i;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Tag length is checked by rrcTagCheckDiscardTimer()*/
    /* Tag data fields are converted to Host byte order by rrcTagCheckDiscardTimer() */

    /* Prepare pointer to tag data */
    tagConfigureDiscardTimer_p = &tagConfigureDiscardTimer_data;
    READ_API_MSG(RrcTagConfigureDiscardTimer, tagConfigureDiscardTimer_p, data_p);

    /* check all tag fields */
    result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
    /*As per SPR 713, change start*/
    /* SPR 18122 Changes Start*/
    for ( i = 0; i < ((DIVIDE_BY_TWO(sizeof(allowedDiscardTimerValues)))); i++ )
        /* SPR 18122 Changes End*/
    /*As per SPR 713, change end*/
    {
        if ( allowedDiscardTimerValues[i] == tagConfigureDiscardTimer_p->discardTimer )
        {
            result = PDCP_SUCCESS;
            break;
        }
    }

    if ( PDCP_SUCCESS != result )
    {
        LOG_PDCP_MSG (PDCP_INVALID_DISCARD_TIMER_VALUE, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, tagPrams_p->ueIndex, 
                 tagPrams_p->lcId, tagConfigureDiscardTimer_p->discardTimer, 0,
                 0,0,__func__,"PDCP_CONFIGURE_DISCARD_TIMER" );
    }
    else 
    {
        LOG_PDCP_MSG (PDCP_CONF_DIS_TIMER, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, tagPrams_p->ueIndex, tagPrams_p->lcId,
                 tagPrams_p->rbDirection, tagConfigureDiscardTimer_p->discardTimer,  
                 0,0,__func__,"" );
        /* configure entity */
        result = pdcpEntityConfigureDiscardTimer(
                tagPrams_p->ueIndex,
                tagPrams_p->lcId,
                tagPrams_p->rbDirection,
                tagConfigureDiscardTimer_p->discardTimer);

        /* No optional parameters. */
    }

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                 "PDCP_CONFIGURE_DISCARD_TIMER" );
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckSnSize 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_CONFIGURE_SN_SIZE RRC tag and converts 
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckSnSize( UInt8* data_p, UInt16 size )
{
    UInt16                     result      = PDCP_SUCCESS;

    /* check length */
    if ( sizeof(RrcTagConfigureSnSize) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, 
                 sizeof(RrcTagConfigureSnSize),sizeof(data_p),0, 0,0, __func__,
                 "PDCP_CONFIGURE_SN_SIZE");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    else 
    {
        /* Tag fields values will be checked when it will be processed after checking */
        /* No optional parameters. */
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagConfigureSnSize 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_CONFIGURE_SN_SIZE RRC tag.
 ****************************************************************************/
UInt16 rrcTagConfigureSnSize( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                     result      = PDCP_SUCCESS;
    LP_RrcTagConfigureSnSize   tagConfigureSnSize_p = PNULL;
    RrcTagConfigureSnSize      tagConfigureSnSize_data;
    LP_RrcTagParams            tagPrams_p = PNULL;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Tag length is checked by rrcTagCheckSnSize()*/
    /* Tag data fields are converted to Host byte order by rrcTagCheckSnSize() */

    /* Prepare pointer to tag data */
    tagConfigureSnSize_p = &tagConfigureSnSize_data;
    READ_API_MSG(RrcTagConfigureSnSize, tagConfigureSnSize_p, data_p);

    /* check all tag fields */
    if ( ( PDCP_TS_DRB_SN_SIZE_SMALL != tagConfigureSnSize_p->snSize ) &&
            ( PDCP_TS_DRB_SN_SIZE_BIG != tagConfigureSnSize_p->snSize ) ) 
    {
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
        LOG_PDCP_MSG (PDCP_WRONG_SN_SIZE, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, tagConfigureSnSize_p->snSize, 
                 tagPrams_p->ueIndex, tagPrams_p->lcId,0, 0,0,__func__, 
                 "PDCP_CONFIGURE_SN_SIZE" );
    }
    else 
    {
        LOG_PDCP_MSG (PDCP_CONF_SN_SIZE, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, tagPrams_p->ueIndex, tagPrams_p->lcId,
                 tagPrams_p->rbDirection, tagConfigureSnSize_p->snSize,  
                 0,0,__func__,"" );
        /* configure entity */
        result = pdcpEntityConfigureSnSize(
                tagPrams_p->ueIndex,
                tagPrams_p->lcId,
                tagPrams_p->rbDirection,
                tagConfigureSnSize_p->snSize);

        /* No optional parameters. */
    }

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                 "PDCP_CONFIGURE_SN_SIZE" );
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckStatusReportRequired 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_CONFIGURE_STATUS_REPORT_REQUIRED RRC tag 
 *                  and converts mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckStatusReportRequired( UInt8* data_p, UInt16 size )
{
    UInt16                         result      = PDCP_SUCCESS;

    /* check length */
    if ( sizeof(RrcTagConfigureStatusReportRequired) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, 
                 sizeof(RrcTagConfigureStatusReportRequired),sizeof(data_p),0, 0,0, 
                 __func__,"PDCP_CONFIGURE_STATUS_REPORT_REQUIRED");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    else 
    {
        /* Tag fields values will be checked when it will be processed after checking */
        /* No optional parameters. */
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagConfigureStatusReportRequired 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CONFIGURE_STATUS_REPORT_REQUIRED RRC tag.
 ****************************************************************************/
UInt16 rrcTagConfigureStatusReportRequired( UInt16 tagId, void* params, 
        UInt8* data_p, UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                                   result = PDCP_SUCCESS;
    LP_RrcTagConfigureStatusReportRequired   tagConfigureStatusReportRequired_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagConfigureStatusReportRequired      tagConfigureStatusReportRequired_data = {0};
    /* ALIGNMENT changes end */
    LP_RrcTagParams                          tagPrams_p = PNULL;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Tag length is checked by rrcTagCheckStatusReportRequired()*/
    /* Tag data fields are converted to Host byte order by rrcTagCheckStatusReportRequired() */

    /* Prepare pointer to tag data */
    
    /* ALIGNMENT changes start */
    tagConfigureStatusReportRequired_p = &tagConfigureStatusReportRequired_data; 
    READ_API_MSG(RrcTagConfigureStatusReportRequired, tagConfigureStatusReportRequired_p, data_p);
    /* ALIGNMENT changes end */
    /* check all tag fields */
    if ( LTE_TRUE < tagConfigureStatusReportRequired_p->statusReportRequired ) 
    {
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
        LOG_PDCP_MSG (PDCP_INVALID_STATUS_REPORT_VALUE, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, tagPrams_p->ueIndex, 
                 tagPrams_p->lcId, 
                 tagConfigureStatusReportRequired_p->statusReportRequired,  
                 0, 0,0,__func__,"PDCP_CONFIGURE_STATUS_REPORT_REQUIRED" );
    }
    else 
    {
        LOG_PDCP_MSG (PDCP_CONF_STATUS_REPRT, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, tagPrams_p->ueIndex, tagPrams_p->lcId,
                 tagPrams_p->rbDirection, 
                 tagConfigureStatusReportRequired_p->statusReportRequired,  
                 0,0,__func__,"" );
        /* configure entity */
        result = pdcpEntityConfigureStatusReportRequired(
                tagPrams_p->ueIndex,
                tagPrams_p->lcId,
                tagPrams_p->rbDirection,
                tagConfigureStatusReportRequired_p->statusReportRequired);

        /* No optional parameters. */
    }

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                 "PDCP_CONFIGURE_STATUS_REPORT_REQUIRED" );
    }

    return result;
}


/****************************************************************************
 * Function Name  : rrcCmdReconfigUeEntity 
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes RRC PDCP_RECONFIG_UE_ENTITY_REQ API.
 *                  It reconfigures set of PDCP Entities for the UE. As 
 *                  response It sends PDCP_RECONFIG_UE_ENTITY_CNF message to RRC.
 ****************************************************************************/
void rrcCmdReconfigUeEntity( UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL );
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }
    /* SPR 11109 Fix start */ 
    memSet(responseBuf, 0,PDCP_RRC_CNF_MAX_SIZE);
    /* SPR 11109 Fix end */ 

    /* Coverity 55830 */
    LP_TlvMessage  message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage  responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcReconfigUeEntityReq reqReconfigUeEntity_p = PNULL;
    LP_RrcReconfigUeCnf       reconfigResponse_p = PNULL;

    /*Check API id */
    CHECK_EXP( PDCP_RECONFIG_UE_ENTITY_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            RRC_MODULE_ID,
            PDCP_RECONFIG_UE_ENTITY_CNF,
            sizeof(RrcReconfigUeCnf)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif 
            );
    /* - Layer2 NON CA Changes */

    /* Prepare pointer to response message data */
    reconfigResponse_p = (LP_RrcReconfigUeCnf)responseMsg_p->data;

    /* Set default values in response message */
    reconfigResponse_p->header.ueIndex = 0;
    reconfigResponse_p->responseCode = PDCP_SUCCESS;

    do {
        /* Coverity TI Fix Start */
        /* Prepare pointer to RRC header  */
        reqReconfigUeEntity_p = (LP_RrcReconfigUeEntityReq)message_p->data;
        if(PNULL == reqReconfigUeEntity_p)
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, message_p->header.length, 
                     PDCP_RRC_RECONFIG_UE_ENTITY_REQ_MIN_SIZE, 0,0, 
                     0,0, __func__,"PDCP_RECONFIG_UE_ENTITY_REQ");
            reconfigResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }
        /* Coverity TI Fix End */
        /* SES-526 Fix Start */
        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( reqReconfigUeEntity_p->header.ueIndex );

        /* save ueIndex in response message */
        reconfigResponse_p->header.ueIndex = reqReconfigUeEntity_p->header.ueIndex;
        /* SES-526 Fix End */

        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, message_p->header.length, 
                     PDCP_RRC_RECONFIG_UE_ENTITY_REQ_MIN_SIZE, 0,0, 
                     0,0, __func__,"PDCP_RECONFIG_UE_ENTITY_REQ");
            reconfigResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /* Check data size */
        if ( message_p->header.length < PDCP_RRC_RECONFIG_UE_ENTITY_REQ_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, message_p->header.length, 
                     PDCP_RRC_RECONFIG_UE_ENTITY_REQ_MIN_SIZE, 0,0, 
                     0,0, __func__,"PDCP_RECONFIG_UE_ENTITY_REQ");
            reconfigResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }
#ifndef UE_SIM_TESTING
        /* SPR 4558 Fix Start */
        reqReconfigUeEntity_p->header.ueIndex =  getUeIdxFromRrcUeIdx(reqReconfigUeEntity_p->header.ueIndex);
       /* SPR 4558 Fix End */
#endif
        /* check it hear for making response with valid ueIndex */
        if ( !pdcpContextIsInitialized() )
        {   /* PDCP isn't initialized */
            LOG_PDCP_MSG (PDCP_CONTEXT_NOT_INITIALIZED, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__,0,0,0,0, 0,0,__func__,"" );
            reconfigResponse_p->responseCode = PDCP_ERR_CONTEXT_NOT_INITIALIZED;
            break;
        }

        /* Check RRC header fields */
        if ( pdcpContextGetMaxUeNumber() <= reqReconfigUeEntity_p->header.ueIndex )
        {
            LOG_PDCP_MSG (PDCP_INVALID_UE, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, 
                 reqReconfigUeEntity_p->header.ueIndex, pdcpContext.isInitialized,
                 0,0, 0,0,__func__,"" );
            reconfigResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_UE_ID;
            break;
        }

        /* parse and check message optional parameters */
        reconfigResponse_p->responseCode = tlvCheckTags(
                rrcReconfigUeEntityTagsTable,
                &message_p->data[sizeof(RrcReconfigUeEntityReq)], /* pointer to the optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, sizeof(RrcReconfigUeEntityReq) ) /* size of optional parameters */
                );
        if ( PDCP_SUCCESS != reconfigResponse_p->responseCode )
        {
            /* message parsing error */
            break;
        }

        /***** Process optional parameters */
        reconfigResponse_p->responseCode = tlvProcessTags(
                rrcReconfigUeEntityTagsTable,
                &reqReconfigUeEntity_p->header,
                &message_p->data[sizeof(RrcReconfigUeEntityReq)], /* pointer to the optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, sizeof(RrcReconfigUeEntityReq) ), /* size of optional parameters */
                responseMsg_p);

    } while (0);

    /* + Layer2 NON CA Changes */
    if ( ( PDCP_SUCCESS == reconfigResponse_p->responseCode ) &&
/* SPR 17157 Fix start */
            ((( sizeof(TlvHeader) + sizeof(RrcReconfigUeCnf) ) != responseMsg_p->header.length) &&
            (( sizeof(TlvHeader) + sizeof(RrcReconfigUeCnf) + sizeof(TlvTagHeader) + MACI_LEN)  != responseMsg_p->header.length)))
/* SPR 17157 Fix end */
    {
        reconfigResponse_p->responseCode = PDCP_PARTIAL_SUCCESS;
    }

    if ( (PDCP_SUCCESS != reconfigResponse_p->responseCode) && 
            (PDCP_PARTIAL_SUCCESS != reconfigResponse_p->responseCode) )
    {
        LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__,reconfigResponse_p->responseCode, 
                 reconfigResponse_p->header.ueIndex, 0,0, 0,0, __func__,
                 "PDCP_RECONFIG_UE_ENTITY_REQ" );
        reconfigResponse_p->responseCode = PDCP_FAIL;
    }

    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( reconfigResponse_p->header.ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( reconfigResponse_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
    );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : rrcTagCheckDeleteEntity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks DCP_DELETE_SRB_ENTITY and PDCP_DELETE_DRB_ENTITY  
 *                  RRC tags and converts mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckDeleteEntity( UInt8* data_p, UInt16 size )
{
    UInt16                          result = PDCP_SUCCESS;

    do {
        /* check length */
        if ( sizeof(RrcTagDeleteEntity) > size )
        {
            /* TLV parsing error stop processing */
            /* +- SPR 17777 */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, size, 
                     sizeof(RrcTagDeleteEntity),sizeof(data_p),0, 0,0, __func__,"PDCP_DELETE_ENTITY");
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
            break;
        }

        /*Prepare pointer to tag data */
        /* Convert data fields from network to Host byte order */
        /* Tag fields values will be checked when it will be processed after checking */
    } while (0);

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagDeleteEntity 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcHeader structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes DCP_DELETE_SRB_ENTITY and PDCP_DELETE_DRB_ENTITY
 *                  RRC tags. It deletes PDCP Entities for the selected logical
 *                  channel within UE context. As response It creates
 *                  PDCP_DELETE_SRB_ENTITY_ERROR tag in response message buffer.
 ****************************************************************************/
UInt16 rrcTagDeleteEntity( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size,  LP_TlvMessage responseMessage )
{   
    UInt16                           result      = PDCP_SUCCESS;
    RrcTagParams                     tagPrams;
    LP_RrcTagDeleteEntity            tagDelete_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagDeleteEntity               tagDelete_data = {0};
    /* ALIGNMENT changes end */

    tagPrams.ueIndex = ((LP_RrcHeader)params)->ueIndex;
    tagPrams.lcId    = 0;

    do {
        /* Tag length is checked by rrcTagCheckDeleteEntity()*/
    
        /* ALIGNMENT changes start */
        tagDelete_p = &tagDelete_data;
        READ_API_MSG(RrcTagDeleteEntity, tagDelete_p, data_p);
       /* ALIGNMENT changes end */

        /* Tag data fields are converted to Host byte order by rrcTagCheckDeleteSrbEntity() */

        /* check all tag fields */
        if ( (LTE_SRB_LC_ID_LOW_BOUND > tagDelete_p->lcId) ||
                (LTE_DRB_LC_ID_HIGH_BOUND < tagDelete_p->lcId) )
        {
            LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__,tagDelete_p->lcId, tagPrams.ueIndex, 0,0, 0,0,
                     __func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
            break;
        }

        /* Delete entity */
        if ( LTE_SRB_LC_ID_HIGH_BOUND >= tagDelete_p->lcId )
        {
            result = pdcpSrbEntityFree( tagPrams.ueIndex, tagDelete_p->lcId );
        }
        else
        {
            result = pdcpDrbEntityFree( tagPrams.ueIndex, tagDelete_p->lcId );
        }
        if ( PDCP_SUCCESS != result )
        {
            break;
        }

    } while (0);

    if ( PDCP_SUCCESS != result )
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, result, tagPrams.ueIndex,size,0, 0,0, __func__,
                 "PDCP_DELETE_ENTITY" );

        /* make deleteEntityError tag and update response message */
        if ( PDCP_DELETE_SRB_ENTITY == tagId )
        {
            rrcMakeTagError( PDCP_DELETE_SRB_ENTITY_ERROR, 
                    tagDelete_p->lcId, result, responseMessage ); 
        }
        else
        {
            rrcMakeTagError( PDCP_DELETE_DRB_ENTITY_ERROR, 
                    tagDelete_p->lcId, result, responseMessage ); 
        }
    }

    /* Don't interrupt tag processing */
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagCheckConfigureCiphering 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_CONFIGURE_CIPHERING RRC tag and converts
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckConfigureCiphering( UInt8* data_p, UInt16 size )
{
    UInt16                        result      = PDCP_SUCCESS;

    /* check length */
    if ( sizeof(RrcTagConfigureCiphering) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, 
                 sizeof(RrcTagConfigureCiphering),sizeof(data_p),0, 0,0, __func__,
                 "PDCP_CONFIGURE_CIPHERING");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagConfigureSrbCiphering 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CONFIGURE_CIPHERING RRC tag.
 ****************************************************************************/
UInt16 rrcTagConfigureSrbCiphering( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                        result      = PDCP_SUCCESS;
    LP_RrcTagConfigureCiphering   tagConfigureCiphering_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagConfigureCiphering      tagConfigureCiphering_data;
    /* ALIGNMENT changes end */
    LP_RrcTagParams               tagPrams_p = PNULL;
    UInt16                        lcCtr = 0;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Tag length is checked by rrcTagCheckConfigureCiphering()*/
    /* Tag data fields are converted to Host byte order by rrcTagCheckConfigureCiphering() */

    /* Prepare pointer to tag data */
    
    /* ALIGNMENT changes start */
    tagConfigureCiphering_p = &tagConfigureCiphering_data;
    READ_API_MSG(RrcTagConfigureCiphering, tagConfigureCiphering_p, data_p);
    /* ALIGNMENT changes end */

    /* check all tag fields */
    if ( ALGO_EEA2 < tagConfigureCiphering_p->AlgorithmId )
    {
        LOG_PDCP_MSG (PDCP_UNKNOWN_ALGO_ID, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, tagConfigureCiphering_p->AlgorithmId, 
                 0, tagPrams_p->ueIndex,0, 0,0, __func__,
                 "PDCP_CONFIGURE_SRB_CIPHERING");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
    }
    else 
    {
        LOG_PDCP_MSG (PDCP_SRB_CIPHERING_PARAMS, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, tagPrams_p->ueIndex, tagConfigureCiphering_p->AlgorithmId,
                 0,0, 0,0,__func__, pdcp_change_data_to_str(16, tagConfigureCiphering_p->Key));
        /* SPR_1235_FIX_START*/
        for ( lcCtr = LTE_SRB_LC_ID_LOW_BOUND;
              lcCtr <= LTE_SRB_LC_ID_HIGH_BOUND; lcCtr++)
        {
            /* configure entity */
            pdcpEntityConfigureSrbCiphering(
                         tagPrams_p->ueIndex,
                         lcCtr,
                         tagConfigureCiphering_p->AlgorithmId,
                         tagConfigureCiphering_p->Key,
                         /*SPR 10446 fix start */
                         PDCP_SECURITY_OFFSET_ENABLED
                         /*SPR 10446 fix end */ );
            /* No optional parameters. */
        }
        /* SPR_1235_FIX_END*/
    }

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                 "PDCP_CONFIGURE_SRB_CIPHERING" );
    }

    return result;
}

/* SPR_1235_FIX_START*/
/****************************************************************************
 * Function Name  : rrcTagConfigureDrbCiphering 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CONFIGURE_CIPHERING RRC tag.
 ****************************************************************************/
UInt16 rrcTagConfigureDrbCiphering( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                        result      = PDCP_SUCCESS;
    LP_RrcTagConfigureCiphering   tagConfigureCiphering_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagConfigureCiphering      tagConfigureCiphering_data;
    /* ALIGNMENT changes end */
    LP_RrcTagParams               tagPrams_p = PNULL;
    UInt16                        lcCtr = 0;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Tag length is checked by rrcTagCheckConfigureCiphering()*/
    /* Tag data fields are converted to Host byte order by rrcTagCheckConfigureCiphering() */

    /* Prepare pointer to tag data */
    
    /* ALIGNMENT changes start */
    tagConfigureCiphering_p = &tagConfigureCiphering_data;
    READ_API_MSG(RrcTagConfigureCiphering, tagConfigureCiphering_p, data_p);
    /* ALIGNMENT changes end */
    
    /* check all tag fields */
    if ( ALGO_EEA2 < tagConfigureCiphering_p->AlgorithmId )
    {
        LOG_PDCP_MSG (PDCP_UNKNOWN_ALGO_ID, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, tagConfigureCiphering_p->AlgorithmId, 
                 0, tagPrams_p->ueIndex,0, 0,0, __func__,
                 "PDCP_CONFIGURE_DRB_CIPHERING");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
    }
    else 
    {
        LOG_PDCP_MSG (PDCP_DRB_CIPHERING_PARAMS, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, tagPrams_p->ueIndex, tagConfigureCiphering_p->AlgorithmId,
                 0,0, 0,0,__func__, pdcp_change_data_to_str(16, tagConfigureCiphering_p->Key));
        for ( lcCtr = LTE_DRB_LC_ID_LOW_BOUND;
              lcCtr <= LTE_DRB_LC_ID_HIGH_BOUND; lcCtr++)
        {
            pdcpEntityConfigureDrbCiphering(
                           tagPrams_p->ueIndex,
                           lcCtr,
                           tagConfigureCiphering_p->AlgorithmId,
                           tagConfigureCiphering_p->Key);
            /* No optional parameters. */
        }
    }

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                 "PDCP_CONFIGURE_DRB_CIPHERING" );
    }

    return result;
}
/* SPR_1235_FIX_END*/

/****************************************************************************
 * Function Name  : rrcTagCheckHoTrigger 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_HO_TRIGGERED RRC tag and converts 
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckHoTrigger( UInt8* data_p, UInt16 size )
{
    /* + SPR 19066 */
    UInt16 result = PDCP_SUCCESS;
    do{
        /* check length */
        if ( sizeof(RrcTagHoTriggered) > size )
        {
            /* TLV parsing error stop processing */
            /* +- SPR 17777 */
            LOG_PDCP_WARNING (PDCP_RRCOAM, 
                    "PDCP_HO_TRIGGERED[%u]",sizeof(RrcTagHoTriggered));
            result =  PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;    
            break;
        }

        /* parse and check tag optional parameters */
        result = tlvCheckTags( 
                rrcHoTriggerTagsTable,
                /* pointer to the optional parameters */
                &data_p[sizeof(RrcTagHoTriggered)], 
                /* size of optional parameters */
                (size - sizeof(RrcTagHoTriggered)) 
                );
        if ( PDCP_SUCCESS != result )
        {
            /* message parsing error */
            break;
        }    
    } while( 0 );
    return result;
    /* - SPR 19066 */
}

/****************************************************************************
 * Function Name  : rrcTagHoTrigger 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_HO_TRIGGERED RRC tag.
 ****************************************************************************/
UInt16 rrcTagHoTrigger( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    LP_RrcTagParams       tagPrams_p = PNULL;
    LP_PdcpUeContext      ueCtx_p = PNULL;
    LP_RrcTagHoTriggered  tagHoTriggered_p = PNULL; 
    RrcTagHoTriggered     tagHoTriggered = {0}; 
#ifdef LOG_PRINT_ENABLED 
    UInt32                state = 0; 
#endif
    /* + SPR 19066 */
    UInt16                result = PDCP_SUCCESS;
    /* - SPR 19066 */
    
    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;
    tagHoTriggered_p = &tagHoTriggered;
    READ_API_MSG(RrcTagHoTriggered, tagHoTriggered_p, data_p);
    
    if ( LTE_TRUE == (tagHoTriggered_p->hoTriggered))
    {
        ueCtx_p = PDCP_GET_UE_CONTEXT(tagPrams_p->ueIndex); 
        if ( PNULL != ueCtx_p)
        {
#ifdef LOG_PRINT_ENABLED 
            state = ueCtx_p->pdcpTxSrb1State;
#endif
            /*Call Function to set PDCP Entity State in HO*/
            (void)pdcpRrcTxStateMachine[ueCtx_p->pdcpTxSrb1State][PDCP_HO](
                    tagPrams_p->ueIndex, LTE_SRB_LC_ID_LOW_BOUND, 
                    ueCtx_p->pdcpTxSrb1State, PDCP_HO, ueCtx_p );

            LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                          __LINE__, tagPrams_p->ueIndex, state, PDCP_HO,
                          ueCtx_p->pdcpTxSrb1State, LTE_SRB_LC_ID_LOW_BOUND, 0, __func__,
                          "PdcpRrcTxStateMachine");

#ifdef LOG_PRINT_ENABLED 
            state = ueCtx_p->pdcpTxSrb2DrbState;
#endif
            ueCtx_p->event = PDCP_HO;
            (void)pdcpRrcTxStateMachine[ueCtx_p->pdcpTxSrb2DrbState]
                [ueCtx_p->event]( tagPrams_p->ueIndex, 
                        LTE_SRB_LC_ID_HIGH_BOUND,
                        ueCtx_p->pdcpTxSrb2DrbState, 
                        ueCtx_p->event, ueCtx_p );

            LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                          __LINE__, tagPrams_p->ueIndex, state, ueCtx_p->event,
                          ueCtx_p->pdcpTxSrb2DrbState, LTE_SRB_LC_ID_HIGH_BOUND, 0, __func__,
                          "PdcpRrcTxStateMachine");

            ueCtx_p->pdcpRxState = PDCP_RX_STATE_HO;
            /* SPR 6758 changes start */
            /* Set hoTriggerRecvd to TRUE */
            ueCtx_p->hoTriggerRecvd = LTE_TRUE;
            /* SPR 6758 changes end */
            LOG_PDCP_MSG (PDCP_UE_CONTEXT_STATE_RX, LOGINFO, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, tagPrams_p->ueIndex, 
                    ueCtx_p->pdcpRxState, 0, 0, 0,0, __func__,"");

            /* Initialize data forwarding queues (only at target side) */
            queueInit( &(ueCtx_p->pdcpDataForwardingDlQ) );
            /* + SPR 19066 */
            /***** Process tag optional parameters (nested tags)*/
            result = tlvProcessTags( 
                    rrcHoTriggerTagsTable,
                    tagPrams_p,
                    /* pointer to the optional parameters */
                    &data_p[sizeof(RrcTagHoTriggered)],
                    /* size of optional parameters */
                    (size - sizeof(RrcTagHoTriggered)),
                    responseMessage
                    );
        }
        else
        {
            /* +- SPR 17777 */
            LOG_PDCP_WARNING (PDCP_RRCOAM,
                    "PDCP_UE_CONTEXT NOT INITIALIZED[%u]",tagPrams_p->ueIndex);
            result = PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
        }
    }
    return result;
    /* - SPR 19066 */
}

/****************************************************************************
 * Function Name  : rrcTagCheckConfigureRohc
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_CONFIGURE_ROHC RRC tag and converts 
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckConfigureRohc( UInt8* data_p, UInt16 size )
{
    UInt16                   result      = PDCP_SUCCESS;

    /* check length */
    if ( sizeof(RrcTagConfigureRohc) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, 
                 sizeof(RrcTagConfigureRohc),sizeof(data_p),0, 0,0, 
                 __func__, "PDCP_CONFIGURE_ROHC");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagConfigureRohc 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CONFIGURE_ROHC RRC tag.
 ****************************************************************************/
UInt16 rrcTagConfigureRohc( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                   result      = PDCP_SUCCESS;
    LP_RrcTagConfigureRohc   tagConfigureRohc_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagConfigureRohc      tagConfigureRohc_data = {0};
    /* ALIGNMENT changes end */
    LP_RrcTagParams          tagPrams_p = PNULL;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Tag length is checked by rrcTagCheckConfigureRohc()*/
    /* Tag data fields are converted to Host byte order by rrcTagCheckConfigureRohc() */

    /* Prepare pointer to tag data */
    
    /* ALIGNMENT changes start */
    tagConfigureRohc_p = &tagConfigureRohc_data;
    READ_API_MSG(RrcTagConfigureRohc, tagConfigureRohc_p, data_p);
    /* ALIGNMENT changes end */

    /* check all tag fields */
    if ( 0 != ((0xFFFF << profileLast) & tagConfigureRohc_p->profileIDsMask) )
    {
        LOG_PDCP_MSG (PDCP_INVALID_ROHC_PROFILE_VALUE, LOGWARNING, PDCP_RRCOAM,
                 pdcpCurrentTime_g, __LINE__, tagPrams_p->ueIndex, 
                 tagPrams_p->lcId, tagConfigureRohc_p->profileIDsMask,0, 0,0, 
                 __func__, "PDCP_CONFIGURE_ROHC");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
    }
    else
    {
        LOG_PDCP_MSG (PDCP_CONF_ROHC, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g, 
                 __LINE__, tagPrams_p->ueIndex, tagPrams_p->lcId, 
                 tagConfigureRohc_p->profileIDsMask, tagConfigureRohc_p->maxCid,
                 0, 0, __func__, "");
        /* configure entity */
        result = pdcpEntityConfigureRohc(
                tagPrams_p->ueIndex,
                tagPrams_p->lcId,
                tagPrams_p->rbDirection,
                tagConfigureRohc_p->profileIDsMask,
                tagConfigureRohc_p->maxCid);
        /* No optional parameters. */
    }

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,tagId,size,
                 responseMessage->header.transactionId, 0,0,
                 __func__, "PDCP_CONFIGURE_ROHC" );
    }

    return result;
}


/****************************************************************************
 * Function Name  : rrcTagReConfigureRohc 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RRC_TAG_PARAMS structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CONFIGURE_ROHC RRC tag.
 ****************************************************************************/
UInt16 rrcTagReConfigureRohc( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                   result      = PDCP_SUCCESS;
    LP_RrcTagConfigureRohc   tagConfigureRohc_p = PNULL;
    RrcTagConfigureRohc      tagConfigureRohc_data_p;
    LP_RrcTagParams          tagPrams_p = PNULL;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Tag length is checked by rrcTagCheckConfigureRohc()*/
    /* Tag data fields are converted to Host byte order by rrcTagCheckConfigureRohc() */

    /* Prepare pointer to tag data */
    tagConfigureRohc_p = &tagConfigureRohc_data_p;
    READ_API_MSG(RrcTagConfigureRohc, tagConfigureRohc_p, data_p);

    /* check all tag fields */
    if ( 0 != ((0xFFFF << profileLast) & tagConfigureRohc_p->profileIDsMask) )
    {
        LOG_PDCP_MSG (PDCP_INVALID_ROHC_PROFILE_VALUE, LOGWARNING, PDCP_RRCOAM,
                 pdcpCurrentTime_g, __LINE__, tagPrams_p->ueIndex, 
                 tagPrams_p->lcId, tagConfigureRohc_p->profileIDsMask,0, 0,0, 
                 __func__, "PDCP_CONFIGURE_ROHC");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
    }
    else
    {     
        LOG_PDCP_MSG (PDCP_CONF_ROHC, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g, 
                 __LINE__, tagPrams_p->ueIndex, tagPrams_p->lcId, 
                 tagConfigureRohc_p->profileIDsMask, tagConfigureRohc_p->maxCid,
                 0, 0, __func__, "");
        /* configure entity */
        result = pdcpEntityReConfigureRohc(
                tagPrams_p->ueIndex,
                tagPrams_p->lcId,
                tagPrams_p->rbDirection,
                tagConfigureRohc_p->profileIDsMask,
                tagConfigureRohc_p->maxCid);
        /* No optional parameters. */
    }

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,responseMessage->header.transactionId,tagId,0,0,
                 __func__, "PDCP_CONFIGURE_ROHC" );
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckReconfigSrbEntity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_RECONFIG_SRB_ENTITY and 
 *                  PDCP_DELETE_DRB_ENTITY RRC tags and converts mandatory
 *                  tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckReconfigSrbEntity( UInt8* data_p, UInt16 size )
{
    UInt16                          result = PDCP_SUCCESS;
    /* SPR_1024_FIX_START */
    do {
    /* SPR_1024_FIX_END */
        /* check length */
        if ( sizeof(RrcTagReconfigSrbEntity) > size )
        {
            /* TLV parsing error stop processing */
            /* +- SPR 17777 */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, size, 
                     sizeof(RrcTagReconfigSrbEntity),sizeof(data_p),0, 0,0, __func__,
                     "PDCP_RECONFIG_SRB_ENTITY");
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
            /* SPR_1024_FIX_START */
            break;
            /* SPR_1024_FIX_END */
        }

   /* SPR_1024_FIX_START */
    } while (0);
    /* SPR_1024_FIX_END */
    /* SPR_1024_FIX_END */
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagReconfigSrbEntity 
 * Inputs         : tagId -
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcHeader structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_RECONFIG_SRB_ENTITY RRC tag.
 *                  It reconfigures PDCP Entities for the selected logical
 *                  channel within UE context.  As response It creates 
 *                  PDCP_RECONFIG_SRB_ENTITY_ERROR tag in response message buffer.
 ****************************************************************************/
UInt16 rrcTagReconfigSrbEntity( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size,  LP_TlvMessage responseMessage)
{   
    UInt16                          result      = PDCP_SUCCESS;
    RrcTagParams                    tagPrams;
    LP_RrcTagReconfigSrbEntity      tagRecofigSrb_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagReconfigSrbEntity         tagRecofigSrb_data = {0};
    /* ALIGNMENT changes end */

    tagPrams.ueIndex = ((LP_RrcHeader)params)->ueIndex;
    tagPrams.lcId    = 0;

    /* SPR_1024_FIX_START */
    do {
    /* SPR_1024_FIX_END */
        /* Tag length is checked by rrcTagCheckReconfigSrbEntity()*/

        /* Prepare pointer to tag data */
    
        /* ALIGNMENT changes start */
        tagRecofigSrb_p = &tagRecofigSrb_data;
        READ_API_MSG(RrcTagReconfigSrbEntity, tagRecofigSrb_p, data_p);
       /* ALIGNMENT changes end */

        /* Tag data fields are converted to Host byte order by rrcTagCheckReconfigSrbEntity() */

        /* check all tag fields */
        if ( (LTE_SRB_LC_ID_LOW_BOUND > tagRecofigSrb_p->lcId) ||
                (LTE_SRB_LC_ID_HIGH_BOUND < tagRecofigSrb_p->lcId) )
        {
            LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__,tagRecofigSrb_p->lcId, tagPrams.ueIndex,0,0, 
                     0,0,__func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
            /* SPR_1024_FIX_START */
            break;
            /* SPR_1024_FIX_END */
        }

        if ( PDCP_ENTITY_RB_BOTH < tagRecofigSrb_p->rbDirection )
        {
            LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__, tagRecofigSrb_p->rbDirection, tagPrams.ueIndex, 
                     tagRecofigSrb_p->lcId, 0, 0,0, __func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
            /* SPR_1024_FIX_START */
            break;
            /* SPR_1024_FIX_END */
        }

        /* store channel id and direction*/
        tagPrams.lcId = tagRecofigSrb_p->lcId;
        tagPrams.rbDirection = tagRecofigSrb_p->rbDirection;
        /* SPR_1235_FIX_START*/
        
        LOG_PDCP_MSG (PDCP_CONTEXT_PARAMS, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, tagPrams.ueIndex, tagPrams.lcId,
                 tagRecofigSrb_p->rbDirection, 0,0,0,__func__,"" );
        pdcpEntitySetDirectionFlag(
                     tagPrams.ueIndex, 
                     tagPrams.lcId,
                     tagRecofigSrb_p->rbDirection,
                     PDCP_ENTITY_SRB );
        /* SPR_1235_FIX_END*/

    /* SPR_1024_FIX_START */
    } while (0);
    /* SPR_1024_FIX_END */

    if ( PDCP_SUCCESS != result )
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,size,tagId,0, 0,0,__func__,
                 "PDCP_RECONFIG_SRB_ENTITY" );
        /* We need to restore Entities state here */

        /* make reconfigSrbEntityError tag and update response message */
        rrcMakeTagError( PDCP_RECONFIG_SRB_ENTITY_ERROR, tagRecofigSrb_p->lcId, result, responseMessage ); 
    }

    /* Don't interrupt tag processing */
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagCheckReconfigDrbEntity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_RECONFIG_DRB_ENTITY and 
 *                  PDCP_DELETE_DRB_ENTITY RRC tags and converts mandatory 
 *                  tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckReconfigDrbEntity( UInt8* data_p, UInt16 size )
{
    UInt16                          result = PDCP_SUCCESS;
    do {
        /* check length */
        if ( sizeof(RrcTagReconfigDrbEntity) > size )
        {
            /* TLV parsing error stop processing */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                     pdcpCurrentTime_g, __LINE__, size, 
                     sizeof(RrcTagReconfigDrbEntity), 0,0, 0,0, __func__,
                     "PDCP_RECONFIG_DRB_ENTITY");
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
            break;
        }
        result = tlvCheckTags( 
                rrcReconfigDrbEntityTagsTable,
                &data_p[sizeof(RrcTagReconfigDrbEntity)], /* pointer to the optional parameters */
                (size - sizeof(RrcTagReconfigDrbEntity))  /* size of optional parameters */
                );
        if ( PDCP_SUCCESS != result )
        {
            /* message parsing error */
            break;
        }  
    } while (0);

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagReconfigDrbEntity 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcHeader structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_RECONFIG_DRB_ENTITY RRC tag.
 *                  It reconfigures PDCP Entities for the selected logical 
 *                  channel within UE context. As response It creates
 *                  PDCP_RECONFIG_DRB_ENTITY_ERROR tag in response message buffer.
 ****************************************************************************/
UInt16 rrcTagReconfigDrbEntity( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size,  LP_TlvMessage responseMessage)
{   
    UInt16                          result      = PDCP_SUCCESS;
    RrcTagParams                    tagPrams;
    LP_RrcTagReconfigDrbEntity      tagRecofigDrb_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagReconfigDrbEntity         tagRecofigDrb_data = {0};
    /* ALIGNMENT changes end */

    tagPrams.ueIndex = ((LP_RrcHeader)params)->ueIndex;
    tagPrams.lcId    = 0;

    do {
        /* Tag length is checked by rrcTagCheckReconfigSrbEntity()*/

        /* Prepare pointer to tag data */
    
        /* ALIGNMENT changes start */
        tagRecofigDrb_p = &tagRecofigDrb_data;
        READ_API_MSG(RrcTagReconfigDrbEntity, tagRecofigDrb_p, data_p);
       /* ALIGNMENT changes end */

        /* Tag data fields are converted to Host byte order by rrcTagCheckReconfigSrbEntity() */

        /* check all tag fields */
        if ( (LTE_DRB_LC_ID_LOW_BOUND > tagRecofigDrb_p->lcId) ||
                (LTE_DRB_LC_ID_HIGH_BOUND < tagRecofigDrb_p->lcId) )
        {
            LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__, tagRecofigDrb_p->lcId, tagPrams.ueIndex, 0,0, 
                     0,0,__func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
            break;
        }

        if ( PDCP_ENTITY_RB_BOTH < tagRecofigDrb_p->rbDirection )
        {
            LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__, tagRecofigDrb_p->rbDirection, tagPrams.ueIndex, 
                     tagRecofigDrb_p->lcId, 0, 0,0, __func__,"" );
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
            break;
        }

        /* save entities parameters */
        /* store channel id and direction*/
        tagPrams.lcId = tagRecofigDrb_p->lcId;
        tagPrams.rbDirection = tagRecofigDrb_p->rbDirection;

        LOG_PDCP_MSG (PDCP_CONTEXT_PARAMS, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, tagPrams.ueIndex, tagPrams.lcId,
                 tagRecofigDrb_p->rbDirection, 0,0,0,__func__,"" );
      
        /* SPR_1235_FIX_START*/
        pdcpEntitySetDirectionFlag(
                     tagPrams.ueIndex, 
                     tagPrams.lcId,
                     tagRecofigDrb_p->rbDirection,
                     PDCP_ENTITY_DRB );
        /* SPR_1235_FIX_END*/

        /***** Process tag optional parameters (nested tags)*/
        result = tlvProcessTags( 
                rrcReconfigDrbEntityTagsTable,
                &tagPrams,
                &data_p[sizeof(RrcTagReconfigDrbEntity)], /* pointer to the optional parameters */
                (size - sizeof(RrcTagReconfigDrbEntity)), /* size of optional parameters */
                responseMessage
                );

    } while (0);

    if ( PDCP_SUCCESS != result )
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,tagId,0,0, 0,0,
                 __func__, "PDCP_RECONFIG_DRB_ENTITY" );
        /* We need to restore Entities state here */

        /* make reconfigSrbEntityError tag and update response message */
        rrcMakeTagError( PDCP_RECONFIG_DRB_ENTITY_ERROR, tagRecofigDrb_p->lcId, result, responseMessage ); 
    }

    /* Don't interrupt tag processing */
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcMakeTagError 
 * Inputs         : tagId - Tag id
 *                  lcId - logical channel identifier
 *                  result - Response field value
 *                  responseMessage - pointer to response message buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : It inserts RrcTagEntityError tag into response message.
 ****************************************************************************/
void rrcMakeTagError(UInt16 tagId, UInt8 lcId, UInt16 result, LP_TlvMessage responseMessage)
{
    LP_TlvTagHeader         tagHeader_p = PNULL;
    LP_RrcTagEntityError    tagError_p = PNULL;

    /* add tag header */
    tagHeader_p = (LP_TlvTagHeader)&responseMessage->data[ TLV_GET_WRITE_POS(responseMessage) ];
    tagHeader_p->tagId = tagId;
    tagHeader_p->length = sizeof(TlvTagHeader) + sizeof(RrcTagEntityError);

    /* update massage length */
    responseMessage->header.length += sizeof(TlvTagHeader);

    /* add Error tag data */
    tagError_p = (LP_RrcTagEntityError)&responseMessage->data[ TLV_GET_WRITE_POS(responseMessage) ];
    tagError_p->lcId = lcId;
    tagError_p->Response = result;

    /*Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->tagId);
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->length);

    CONVERT_INT16_HOST_TO_NETWORK(tagError_p->Response);

    /* update massage length */
    responseMessage->header.length += sizeof(RrcTagEntityError);
}

/****************************************************************************
 * Function Name  : rrcCmdDefaultHandler
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size       
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes unknown RRC API.
 ****************************************************************************/
void rrcCmdDefaultHandler( UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL );
    if (responseBuf == PNULL)
    {
        /* +- SPR 17777 */
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 size,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }

    /* Coverity 55830 */
    LP_TlvMessage   message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage   responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    UInt16*         response_p;

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            RRC_MODULE_ID,
            message_p->header.apiId,
            sizeof(UInt16)
#ifndef UE_SIM_TESTING
           ,message_p->header.cellIndex
#endif 
            );
    /* - Layer2 NON CA Changes */

    /*Prepare pointer to response message data */
    response_p = (UInt16*)responseMsg_p->data;

    /* Set default values in response message and response fields to network byte order */
    /* SPR  3444 changes start */
    *response_p = PDCP_HTONS((UInt16)PDCP_ERR_TLV_PARSING_INVALID_API_ID);
    /* SPR  3444 changes end */
    LOG_PDCP_MSG (PDCP_RRC_API_NS, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
             __LINE__, message_p->header.apiId, 0,0,0, 0,0,__func__,"" );

    /*Convert response to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /*send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : pdcpRrcProcessMessage 
 * Inputs         : data_p  - pointer to TLV API buffer.
 *                  size  -  TLV API buffer size.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for 
 *                  delivering RRC API to the PDCP layer. The RRC API
 *                  message will be processed immediately.
 ****************************************************************************/
void    pdcpRrcProcessMessage( UInt8 * data_p, UInt16 size )
{
    /* process message */
    tlvProcessCommand( data_p, size );
}

/****************************************************************************
 * Function Name  : pdcpRrcSrbDataStatusInd 
 * Inputs         : tlvTransactionId - Transaction Id from TLV header.
 *                  ueIndex - UE Id.
 *                  lcId - logical channel id.
 *                  transactionId - Transaction Id.
 *                  responseCode - PDCP_SUCCESS/PDCP_FAIL
 *                  response - SRB SDU/PDU processing failing code.
 *                  data_p -
 *                  dataInZCB -
 *                  dataSize -
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for 
 *                  delivering to RRC layer the Integrity protection fail
 *                  indication according to TS 36323-840 5.1.2.2 (RX).
 *                  On Tx side, it can be response of PDCP_SRB_DATA_REQ API
 *                  for SRB SDU processing status indication. It will be sent
 *                  only if serviceRequested filed is set to true
 ****************************************************************************/
void    pdcpRrcSrbDataStatusInd( 
        UInt16 tlvTransactionId, 
        UInt16 ueIndex, 
        UInt16 lcId, 
        UInt32 transactionId, 
        UInt16 responseCode, 
        UInt16 response,
        void* data_p,
        UInt8 dataInZCB,
        UInt32 dataSize )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL );
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }
    /* SPR 11109 Fix start */ 
    memSet(responseBuf, 0,PDCP_RRC_CNF_MAX_SIZE);
    /* SPR 11109 Fix end */ 

    /** SPR 11078 Changes Start **/
    if ( PNULL == pdcpContext.ueContexts[ueIndex]) {
        freeMemPool(responseBuf);
        responseBuf = PNULL;	
        LOG_PDCP_MSG ( PDCP_INVALID_UE_CONTEXT, LOGWARNING,
                (PDCP_RRCOAM|PDCP_TX|PDCP_RX), pdcpCurrentTime_g, 
                __LINE__, ueIndex, pdcpContext.isInitialized, 0,0, 
                0,0, __func__,"");
        return ;
    }
    /** SPR 11078 Changes End **/

    /* Coverity 55830 */
    LP_TlvMessage               responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcSrbDataStatusStatusInd  indSrbDataStatus_p = PNULL;
    LP_TlvTagHeader             tagHeader_p = PNULL;
    LP_RrcTagSrbDataStatusError tagError_p = PNULL;
    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    RrcCellIndex cellId = PDCP_GET_RRC_CELL_INDEX( pdcpContext.\
                               ueContexts[ueIndex]->cellIndex );
#endif
    /* Prepare response buffer */
    tlvPrepareMessage(  responseMsg_p,
            tlvTransactionId,
            RRC_MODULE_ID,
            PDCP_SRB_DATA_STATUS_IND,
            sizeof(RrcSrbDataStatusStatusInd)
#ifndef UE_SIM_TESTING
            ,cellId
#endif 
            );
    /* - Layer2 NON CA Changes */

    /* fill message */
    indSrbDataStatus_p = (LP_RrcSrbDataStatusStatusInd)responseMsg_p->data;

    /* Fill and convert response fields to network byte order */
    indSrbDataStatus_p->transactionId = PDCP_HTONL(transactionId);
    /* SPR 4772 Fix start */
#ifndef UE_SIM_TESTING
    indSrbDataStatus_p->ueIndex = PDCP_HTONS(getRrcUeIdxFromUeIdx(ueIndex));
#else
    indSrbDataStatus_p->ueIndex = PDCP_HTONS(ueIndex);
#endif
    /* SPR 4772 Fix end */
    indSrbDataStatus_p->lcId = (UInt8)lcId;
    indSrbDataStatus_p->responseCode = PDCP_HTONS(responseCode);

    if ( PDCP_SUCCESS != responseCode )
    {   /* add error tag */
        /* add tag header */
        tagHeader_p = (LP_TlvTagHeader)&responseMsg_p->data[ TLV_GET_WRITE_POS(responseMsg_p) ];
	/* SPR  3444 changes start */
        tagHeader_p->tagId  = PDCP_NTOHS((UInt16)PDCP_SRB_DATA_STATUS_ERROR);
        tagHeader_p->length = PDCP_NTOHS((UInt16)(sizeof(TlvTagHeader) + \
                                         sizeof(RrcTagSrbDataStatusError) + \
                                         dataSize));
	/* SPR  3444 changes End */
        /* update massage length */
        responseMsg_p->header.length += sizeof(TlvTagHeader);

        /* add Error tag data */
        tagError_p = (LP_RrcTagSrbDataStatusError)&responseMsg_p->data[ TLV_GET_WRITE_POS(responseMsg_p) ];

        /* SPR 1825 changes start */
        tagError_p->Response = PDCP_HTONS(response);

        /* update massage length */
        responseMsg_p->header.length += sizeof(RrcTagSrbDataStatusError);

        if ( LTE_TRUE == dataInZCB )
        {
            /* extract data from ZCB msg to TLV msg */
            if ( (SInt64)dataSize != msgExtract( 
                        data_p, 
                        0, 
                        &responseMsg_p->data[ TLV_GET_WRITE_POS(responseMsg_p) ],
                        dataSize ) )
            {
                /* Coverity 55830 */
                freeMemPool(responseBuf);
                /* Coverity 55830 */
                LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RRCOAM, 
                        pdcpCurrentTime_g, __LINE__, dataSize, ueIndex, lcId, 
                        0, 0,0, __func__,"SRB_DATA_STATUS_IND");
                return;
            }
        }
        else
        {
            memCpy ( &responseMsg_p->data[ TLV_GET_WRITE_POS(responseMsg_p) ],
                     data_p,
                     dataSize ); 
        }

        /* update massage length */
        responseMsg_p->header.length += dataSize; 
        /* SPR 1825 changes end */
    }

    /*Convert response fields to network byte order */
    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : pdcpRrcProcessMessage 
 * Inputs         : ueIndex - UE Id.
 *                  lcId - logical channel id.
 *                  data_p - pointer ZCB message.
 * Outputs        : None.
 * Returns        : None.
 * Description    : This API will be used by the PDCP layer application for 
 *                  delivering SRB SDU to RRC layer. It creates 
 *                  PDCP_SRB_DATA_IND TLV message and sends it to RRC.
 ****************************************************************************/
void    pdcpRrcDataInd( UInt16 ueIndex, UInt16 lcId, void* data_p )
{
    /* + Coverity_Stack Fix */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool( 
                sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE, PNULL);
    /* - Coverity_Stack Fix */
    /* + Coverity_30692 */
    if (responseBuf == PNULL)
    {
        LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
                pdcpCurrentTime_g, __LINE__, 
                (sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                0,0,0,0.0,0.0,__FILE__,__func__);
        return;
    }
    /* SPR 11109 Fix start */ 
    memSet(responseBuf, 0, (
                sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE));
    /* SPR 11109 Fix end */ 

    /* - Coverity_30692 */
    LP_TlvMessage responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcSrbDataInd     indSrbData_p = PNULL;
    UInt16               size;
    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    RrcCellIndex cellId = INVALID_CELL_INDEX;
#endif 
    /* - Layer2 NON CA Changes */

    /* check message */
    if ( PNULL == data_p )
    {
        ltePanic("Invalid pointer");
        /* + Coverity_Stack Fix */
        freeMemPool(responseBuf);
        /* - Coverity_Stack Fix */
        return;
    }

    /* Get msg size */
    size = msgSize( data_p, PNULL );
    if ( (0 == size) || ( PDCP_TS_SDU_MAX_SIZE < size ) )
    {
        /* + Coverity_Stack Fix */
        freeMemPool(responseBuf);
        /* - Coverity_Stack Fix */
        LOG_PDCP_MSG( PDCP_INVALID_DATA_SIZE, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, size, 0, ueIndex, lcId, 
                0,0,__func__,"SRB_DATA_IND");
        return;
    }

    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    cellId= PDCP_GET_RRC_CELL_INDEX( pdcpContext.\
            ueContexts[ueIndex]->cellIndex );
#endif
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            0,
            RRC_MODULE_ID,
            PDCP_SRB_DATA_IND,
            sizeof(RrcSrbDataInd)
#ifndef UE_SIM_TESTING
            ,cellId
#endif 
            );
    /* - Layer2 NON CA Changes */

    /* fill message */
    indSrbData_p = (LP_RrcSrbDataInd)responseMsg_p->data;

#ifndef UE_SIM_TESTING
    indSrbData_p->header.ueIndex = pdcpContext.ueContexts[ueIndex]->rrcUeIndex;
#else
    indSrbData_p->header.ueIndex = ueIndex;
#endif
    indSrbData_p->lcId = (UInt8)lcId;

    /* extract data from ZCB msg to TLV msg */
    if ( size != msgExtract( 
                data_p, 
                0, 
                &responseMsg_p->data[ TLV_GET_WRITE_POS(responseMsg_p) ], 
                size ) )
    {
        /* + Coverity_Stack Fix */
        freeMemPool(responseBuf);
        /* - Coverity_Stack Fix */
        LOG_PDCP_MSG( PDCP_MSG_EXTRACT_FAILED, LOGFATAL, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, size, indSrbData_p->header.ueIndex, indSrbData_p->lcId, 0, 
                0,0, __func__,"PDCP_SRB_DATA_IND");
        return;
    }

    responseMsg_p->header.length += size;

    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( indSrbData_p->header.ueIndex );

    tlvPrepareHeaderForSending(responseMsg_p);
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* + Coverity_Stack Fix */
    freeMemPool(responseBuf);
    /* - Coverity_Stack Fix */
}

/****************************************************************************
 * Function Name  : rrcTagCheckMessageMacI
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_MESSAGE RRC tag and converts mandatory
 *                  tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckMessageMacI( UInt8* data_p, UInt16 size )
{
    /* check length */
    if ( sizeof(RrcMessageMacI) > size )
    {
       /* TLV parsing error stop processing */
        /* +- SPR 17777 */
       LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, size, sizeof(RrcMessageMacI),
                sizeof(data_p),0, 0,0, __func__, "PDCP_MESSAGE");
       return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcMakeTagResponseMacI
 * Inputs         : tagId - Tag of the message
 *                  cellIdentity, macI - 32 bit value by integrity engine
 *                  macI -
 *                  result - SUCCESS/FAILURE
 *                  responseMessage - response array to be sent to RRC      
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_MSG_AUTHENTICATION_CODE RRC tag and 
 *                  converts mandatory tag fields to Network byte order.
 ****************************************************************************/
void rrcMakeTagResponseMacI(UInt16 tagId, LP_RrcMessageMacI cellIdentity, 
        UInt8* macI, UInt16 result, LP_TlvMessage responseMessage)
{
    LP_TlvTagHeader       tagHeader_p = PNULL;
    LP_RrcMacIResponse    tagResponseMacI_p = PNULL;

    /* add tag header */
    tagHeader_p = (LP_TlvTagHeader)&responseMessage->
                          data[ TLV_GET_WRITE_POS(responseMessage) ];
    tagHeader_p->tagId = tagId;
    tagHeader_p->length = sizeof(TlvTagHeader) + sizeof(RrcMacIResponse);

    /* update massage length */
    responseMessage->header.length += sizeof(TlvTagHeader);

    /* add Error tag data */
    tagResponseMacI_p = (LP_RrcMacIResponse)&responseMessage->
                                 data[ TLV_GET_WRITE_POS(responseMessage) ];
    memCpy (tagResponseMacI_p->cellIdentity, cellIdentity, MACI_LEN);
    tagResponseMacI_p->Response = result;

    /* update message length */
    responseMessage->header.length += sizeof(RrcMacIResponse);
    
    if (PDCP_SUCCESS == result)
    {
        tagHeader_p->length += MACI_LEN;
        memCpy (&responseMessage->data[ TLV_GET_WRITE_POS(responseMessage) ],
                macI, MACI_LEN);
        /* update message length */
        responseMessage->header.length += MACI_LEN;
    }

    /*Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->tagId);
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->length);

    CONVERT_INT16_HOST_TO_NETWORK(tagResponseMacI_p->Response);
}

/****************************************************************************
 * Function Name  : rrcTagMessageMacI
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcHeader structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_MESSAGE RRC tag.
 *                  It computes MAC-I for PDCP_MAC_I_REQ API.
 ****************************************************************************/
UInt16 rrcTagMessageMacI( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size,  LP_TlvMessage responseMessage_p)
{   
    UInt16                 result      = PDCP_SUCCESS;
    /* SPR 3569 changes start */
    LP_RrcMacIReq          tagParams_p  = PNULL;
    /* SPR 3569 changes end */
    LP_RrcMessageMacI	   tagMessageMacI_p = PNULL;
    LP_PdcpEnityContext    ctx_p = PNULL;
    UInt8                  MACI[MACI_LEN] =  {0};
    void                   *integrityData_p = PNULL;
    
    /* SPR 3569 changes start */
    tagParams_p = (LP_RrcMacIReq)params;
    /* SPR 3569 changes end */

    do {
        /* Tag length is checked by rrcTagCheckMessageMacI()*/
        /* Prepare pointer to tag data */
        tagMessageMacI_p = (LP_RrcMessageMacI)data_p;

        ctx_p = pdcpGetEntityContext( tagParams_p->header.ueIndex, 
                     LTE_SRB_LC_ID_LOW_BOUND, PDCP_ENTITY_RB_TX);
        if (PNULL == ctx_p) 
        {
            LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
                     pdcpCurrentTime_g, __LINE__, tagParams_p->header.ueIndex,
                     tagParams_p->lcId,0,0,0,0,__func__,"");
            result = PDCP_ERR_ENTITY_NOT_FOUND;
            break;
        }
        integrityData_p = msgAlloc( pdcpContextGetMsgPool(), 0, 0, 0 );
        if ( PNULL == integrityData_p )
        {
            /* free memory */
            LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, 0, tagParams_p->header.ueIndex,
                    tagParams_p->lcId, 0, 0,0, __func__, "rrcTagMessageMacI");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            result = PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
            ltePanic("msgAlloc failed in %s",__FUNCTION__);
            break;
        }

        if ( PNULL == msgInsert( integrityData_p, 0, 
                    &data_p[sizeof(RrcMessageMacI)], /* get pointer to data */
                    /* get data size */
                    ( size - sizeof(RrcMessageMacI) ) ) ) 
        {
            /* free memory */
            msgFree( integrityData_p );
            integrityData_p = PNULL;	
            result = PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
            LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__, TLV_GET_OPTIONAL_PART_SIZE( data_p, 
                     sizeof(RrcMessageMacI) ), tagParams_p->header.ueIndex,
                     tagParams_p->lcId, 0,0,0, __func__,
                     "Data for MAC-I calculation cannot be inserted to ZCB");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            ltePanic("msgInsert failed in %s",__FUNCTION__);
            break;
        }
        /* +- SPR 17777 */
        LOG_PDCP_MSG( PDCP_DATA_TO_INTEGRITY_ENGINE, LOGDEBUG, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, tagParams_p->header.ueIndex,
                 tagParams_p->lcId, MAX_COUNT,tagId,0,0,__func__,"");
        /* Compute MAC-I */
        integrityGenerateMACI
                   ( ctx_p->integrity.algorithmId,
                    /* SPR 3466 changes start */
                    tagParams_p->lcId,
                    /* SPR 3466 changes end */
                    MAX_COUNT,
                    tagParams_p->rbDirection,
                    ctx_p->integrity.key,
                    integrityData_p,
                    MACI); 
        msgFree( integrityData_p );
        integrityData_p = PNULL;
    } while (0);

    rrcMakeTagResponseMacI(PDCP_MSG_AUTHENTICATION_CODE, tagMessageMacI_p, MACI, 
        result, responseMessage_p);
    if ( PDCP_SUCCESS != result )
    {
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result, 0,0,0, 0,0,__func__,
                 "MESSAGE in MAC-I Request" );
    }

    /* Don't interrupt tag processing */
    return PDCP_SUCCESS;
}

/****************************************************************************
* Function Name  : rrcCmdMacIReq
* Inputs         : data_p - Pointer to TLV API message buffer
*                  size - message size
* Outputs        : None.
* Returns        : None.
* Description    : It processes PDCP_MAC_I_REQ API.
*                  It sends PDCP_MAC_I_RESP message to RRC.
****************************************************************************/
void rrcCmdMacIReq( UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL );
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }

    /* Coverity 55830 */
    LP_TlvMessage    message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcMacIReq    rrcMacIReq_p = PNULL;
    LP_RrcMacIResp   rrcMacIResponse_p = PNULL;
    
    /*Check API id */
    CHECK_EXP( PDCP_MAC_I_REQ != message_p->header.apiId );
    
    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
              message_p->header.transactionId,
              RRC_MODULE_ID,
              PDCP_MAC_I_RESP,
              sizeof(RrcMacIResp)
#ifndef UE_SIM_TESTING
              ,message_p->header.cellIndex
#endif 
              );
    /* - Layer2 NON CA Changes */
    /* Prepare pointer to response message data */
    rrcMacIResponse_p = (LP_RrcMacIResp)responseMsg_p->data;
    
    /* Set default values in response message */
    rrcMacIResponse_p->header.ueIndex = 0;
    rrcMacIResponse_p->responseCode = PDCP_SUCCESS;
    do
    {   
        /* SES-526 Fix Start */
        /*Prepare pointer to message data */
        rrcMacIReq_p = (LP_RrcMacIReq)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( rrcMacIReq_p->header.ueIndex );
        CONVERT_INT16_NETWORK_TO_HOST( rrcMacIReq_p->rbDirection );

        /* save ueIndex in response message */
        rrcMacIResponse_p->header.ueIndex = rrcMacIReq_p->header.ueIndex;
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, message_p->header.length,
                     size, 0,0, 0,0, __func__,"PDCP_MAC_I_REQ");
            rrcMacIResponse_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }
    
        /* Check data size */
        if ( message_p->header.length < PDCP_RRC_PDCP_MAC_I_REQ_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, message_p->header.length,
                     PDCP_RRC_PDCP_MAC_I_REQ_MIN_SIZE, 0,0,
                     0,0, __func__,"PDCP_MAC_I_REQ");
            rrcMacIResponse_p->responseCode = 
                     PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }
    
#ifndef UE_SIM_TESTING
         /* SPR 4558 Fix Start */
         rrcMacIReq_p->header.ueIndex = getUeIdxFromRrcUeIdx(rrcMacIReq_p->header.ueIndex);
          /* SPR 4558 Fix End */
#endif

        /* check it hear for making response with valid ueIndex */        
        if ( rrcMacIReq_p->header.ueIndex >= pdcpContext.maxUeNumber )
        {
            LOG_PDCP_MSG (PDCP_INVALID_UE, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, rrcMacIReq_p->header.ueIndex,
                    pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
            rrcMacIResponse_p->responseCode = 
                                     PDCP_ERR_TLV_PARSING_INVALID_UE_ID;
            break;
        }

        /* parse and check message optional parameters */
        rrcMacIResponse_p->responseCode = tlvCheckTags(
                rrcComputeMacITagsTable,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcMacIReq)], 
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, sizeof(RrcMacIReq) )
                );
        if ( PDCP_SUCCESS != rrcMacIResponse_p->responseCode )
        {
            /* message parsing error */
            break;
        }

        /***** Process optional parameters */
        rrcMacIResponse_p->responseCode = tlvProcessTags(
                rrcComputeMacITagsTable,
                &rrcMacIReq_p->header,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcMacIReq)], 
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, sizeof(RrcMacIReq) ), 
                responseMsg_p);
    }while (0);
    
    /* check if PDCP_PARTIAL_SUCCESS response code should be sent */
    if ( PDCP_SUCCESS != rrcMacIResponse_p->responseCode )
    {
    	/*+ Coverity 243567*/
		LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
				__LINE__,rrcMacIResponse_p->responseCode, 
				rrcMacIResponse_p->header.ueIndex,0,0, 0,0,__func__, /*MS Change */
				"PDCP_MAC_I_REQ" );
    	/*- Coverity 243567*/
	    rrcMacIResponse_p->responseCode = PDCP_FAIL;
    }

    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( rrcMacIResponse_p->header.ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( rrcMacIResponse_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
* Function Name  : rrcCmdSnHfnStatusReq
* Inputs         : data_p - Pointer to TLV API message buffer
*                  size - message size
* Outputs        : None.
* Returns        : None.
* Description    : It processes RRC PDCP_SN_HFN_STATUS_REQ API.
*                  It sends PDCP_SN_HFN_STATUS_RESP message to RRC.
****************************************************************************/
void rrcCmdSnHfnStatusReq( UInt8* data_p, UInt16 size )
{
    UInt8 lcId = 0;
    /* + Coverity_Stack Fix */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool( (
                PDCP_RRC_CNF_MAX_SIZE + (BITMAP_SIZE * PDCP_MAX_DRB)), PNULL);
#ifdef LOG_PRINT_ENABLED 
    UInt32 state = 0;
#endif
    /* - Coverity_Stack Fix */
    /* + Coverity_30693 */
    if (responseBuf == PNULL)
    {
        LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
                pdcpCurrentTime_g, __LINE__, 
                (PDCP_RRC_CNF_MAX_SIZE + (BITMAP_SIZE * PDCP_MAX_DRB)), 0,0,0,
                0.0,0.0,__FILE__,__func__);
        return;
    }
    /* - Coverity_30693 */
    LP_TlvMessage         message_p        = (LP_TlvMessage)data_p;
    LP_TlvMessage         responseMsg_p    = (LP_TlvMessage)&responseBuf[0];
    LP_RrcSnHfnStatusReq  rrcSnHfnStatus_p = PNULL;
    LP_RrcSnHfnStatusResp snHfnStatusResponse_p = PNULL;
    LP_TlvTagHeader       tagHeader_p      = PNULL;
    LP_RrcTagSnHfnStatus  tagSnHfnStatus_p = PNULL;
    LP_RrcTagUlRecvSn     tagUlRecvSn_p    = PNULL;
    LP_PdcpUeContext      pdcpUeCtx_p      = PNULL;
    LP_PdcpEnityContext   pdcpEntityContext_p = PNULL;
    /* SPR 3895 changes start */
    LP_TlvTagHeader       tagHeaderUlBitmap_p = PNULL;
    /* SPR 3895 changes end */

    /*Check API id */
    CHECK_EXP( PDCP_SN_HFN_STATUS_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            RRC_MODULE_ID,
            PDCP_SN_HFN_STATUS_RESP,
            sizeof(RrcSnHfnStatusResp)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif 
            );
    /* - Layer2 NON CA Changes */

    /* Prepare pointer to response message data */
    snHfnStatusResponse_p = (LP_RrcSnHfnStatusResp)responseMsg_p->data;

    /* Set default values in response message */
    snHfnStatusResponse_p->ueIndex = 0;
    snHfnStatusResponse_p->responseCode = PDCP_SUCCESS;
    do
    {   
        /* SES-526 Fix Start */
        /*Prepare pointer to message data */
        rrcSnHfnStatus_p = (LP_RrcSnHfnStatusReq)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( rrcSnHfnStatus_p->header.ueIndex );

        /* save ueIndex in response message */
        snHfnStatusResponse_p->ueIndex = rrcSnHfnStatus_p->header.ueIndex;
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    size, 0,0, 0,0, __func__,"PDCP_SN_HFN_STATUS_REQ");
            snHfnStatusResponse_p->responseCode = 
                PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /* Check data size */
        if ( message_p->header.length < 
                PDCP_RRC_PDCP_SN_HFN_STATUS_REQ_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    PDCP_RRC_PDCP_SN_HFN_STATUS_REQ_MIN_SIZE, 0,0,
                    0,0, __func__,"PDCP_SN_HFN_STATUS_REQ");
            snHfnStatusResponse_p->responseCode = 
                PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

#ifndef UE_SIM_TESTING
        /* SPR 4558 Fix Start */
        rrcSnHfnStatus_p->header.ueIndex = getUeIdxFromRrcUeIdx(rrcSnHfnStatus_p->header.ueIndex);
        /* SPR 4558 Fix End */
#endif

        /* check it hear for making response with valid ueIndex */
        /* if (rrcSnHfnStatus_p->sn_hfn_required)*/
        {
            LOG_PDCP_MSG( PDCP_SN_HFN_STATUS_REQ_RECVD, LOGINFO, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, rrcSnHfnStatus_p->header.ueIndex, 
                    rrcSnHfnStatus_p->sn_hfn_required, 0,0, 0,0,__func__,"" );

            pdcpUeCtx_p = PDCP_GET_UE_CONTEXT( rrcSnHfnStatus_p->header.ueIndex );
            if ( PNULL == pdcpUeCtx_p )            
            {
                LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                        pdcpCurrentTime_g, __LINE__, rrcSnHfnStatus_p->header.ueIndex,
                        pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
                snHfnStatusResponse_p->responseCode = 
                    PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
                break;
            }

            /* Lossy HO changes start */
            /* Rarse and check message optional parameters */
            snHfnStatusResponse_p->responseCode = tlvCheckTags(
                    rrcLossyHoTagsTable,
                    /* pointer to the optional parameters */
                    &message_p->data[sizeof(RrcSnHfnStatusReq)], 
                    /* size of optional parameters */
                    TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                        sizeof(RrcSnHfnStatusReq) )
                    );
            if ( PDCP_SUCCESS !=  snHfnStatusResponse_p->responseCode)
            {
                /* message parsing error */
                break;
            }

            /***** Process optional parameters */
            snHfnStatusResponse_p->responseCode = tlvProcessTags(
                    rrcLossyHoTagsTable,
                    &rrcSnHfnStatus_p->header,
                    /* pointer to the optional parameters */
                    &message_p->data[sizeof(RrcSnHfnStatusReq)], 
                    /* size of optional parameters */
                    TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                        sizeof(RrcSnHfnStatusReq) ), 
                    responseMsg_p);
            if ( PDCP_SUCCESS !=  snHfnStatusResponse_p->responseCode)
            {
                /* message parsing error */
                break;
            }
            /* Lossy HO changes stop */
            for(; lcId < pdcpUeCtx_p->numOfActiveDrbLc; lcId++)
            {
                pdcpEntityContext_p = pdcpGetEntityContext(
                        rrcSnHfnStatus_p->header.ueIndex, 
                        pdcpUeCtx_p->activeLcInfo[lcId], 
                        PDCP_ENTITY_RB_TX);
                /* It is assumed that the PdcpEntityContext will be present for
                   both TX and RX */
                if ( (PNULL != pdcpEntityContext_p) && 
                        (pdcpEntityContext_p->rlcMode == PDCP_ENTITY_RLC_AM) )
                {
                    /* add tag header */
                    tagHeader_p = (LP_TlvTagHeader)&responseMsg_p->
                        data[ TLV_GET_WRITE_POS(responseMsg_p) ];
                    /* SPR  3444 changes start */
                    tagHeader_p->tagId  = PDCP_HTONS((UInt16)PDCP_LC_SN_HFN_STATUS);
                    tagHeader_p->length = PDCP_HTONS((UInt16)(sizeof(TlvTagHeader) + \
                                sizeof(RrcTagSnHfnStatus)));
                    /* SPR  3444 changes End */
                    /* update massage length */
                    responseMsg_p->header.length += sizeof(TlvTagHeader);

                    tagSnHfnStatus_p = (LP_RrcTagSnHfnStatus)&responseMsg_p->
                        data[ TLV_GET_WRITE_POS(responseMsg_p) ];

                    tagSnHfnStatus_p->lcId     = pdcpUeCtx_p->activeLcInfo[lcId];
                    tagSnHfnStatus_p->dlSnCnt  = PDCP_HTONS(pdcpEntityContext_p->next_SN);
                    tagSnHfnStatus_p->dlHfnCnt = PDCP_HTONL(pdcpEntityContext_p->hfn);

                    /* update massage length */
                    responseMsg_p->header.length += sizeof(RrcTagSnHfnStatus);
                }
                else
                {
                    LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, 
                            PDCP_RRCOAM, pdcpCurrentTime_g, __LINE__, 
                            rrcSnHfnStatus_p->header.ueIndex,
                            pdcpUeCtx_p->activeLcInfo[lcId], 0,0, 
                            0,0,__func__,"");
                }
                pdcpEntityContext_p = pdcpGetEntityContext(
                        rrcSnHfnStatus_p->header.ueIndex, 
                        pdcpUeCtx_p->activeLcInfo[lcId], 
                        PDCP_ENTITY_RB_RX);
                if ( (PNULL != pdcpEntityContext_p) && 
                        (pdcpEntityContext_p->rlcMode == PDCP_ENTITY_RLC_AM) &&
                        (PNULL != tagSnHfnStatus_p) )
                {
                    /* Reset isPsrRecvd flag */
                    pdcpEntityContext_p->isPsrRecvd = LTE_FALSE;
                    tagSnHfnStatus_p->ulSnCnt = PDCP_HTONS(
                            /* SPR 3589 changes start */
                            /* SPR 3822 changes start */
                            (pdcpEntityContext_p->\
                             last_Submitted_SN  + 1) %\
                            (PDCP_TS_MAX_SN_AM_MODE + 1));
                    /* SPR 3822 changes end */
                    /* SPR 3589 changes end */
                    /* SPR 8703 fix start */
                    if(pdcpEntityContext_p->next_SN > 
                            pdcpEntityContext_p->last_Submitted_SN || 
                            /* SPR 9008 changes start */
                            PDCP_TS_MAX_SN_AM_MODE == 
                            pdcpEntityContext_p->last_Submitted_SN )
                        /* SPR 9008 changes end */
                    {
                        tagSnHfnStatus_p->ulHfnCnt = PDCP_HTONL( 
                                pdcpEntityContext_p->hfn);
                    }
                    else
                    {
                        tagSnHfnStatus_p->ulHfnCnt = PDCP_HTONL(
                                pdcpEntityContext_p->hfn - 1);
                    }
                    /* SPR 8703 fix end */
                    if (LTE_TRUE == pdcpEntityContext_p->statusReportRequired)
                    {
                        /*SPR 4735 fix +*/
                        /* SPR 5034 FIX START */
                        pdcpEntityContext_p->fms = (pdcpEntityContext_p->\
                                last_Submitted_SN  + 1) %\
                                                   (PDCP_TS_MAX_SN_AM_MODE + 1); 
                        /* SPR 5034 FIX END */

                        /*SPR 4735 fix -*/
                        pdcpEncodeUlBitmap(rrcSnHfnStatus_p->header.ueIndex, 
                                pdcpUeCtx_p->activeLcInfo[lcId], 
                                pdcpEntityContext_p);
                        /* SPR 3895 changes start */
                        tagHeaderUlBitmap_p = (LP_TlvTagHeader)&responseMsg_p->
                            data[ TLV_GET_WRITE_POS(responseMsg_p) ];
                        /* SPR  3444 changes start */
                        tagHeaderUlBitmap_p->tagId  = PDCP_HTONS( 
                                (UInt16) PDCP_UL_RECV_SN );
                        tagHeaderUlBitmap_p->length = PDCP_HTONS( 
                                (UInt16)(sizeof(TlvTagHeader) + \
                                    sizeof(RrcTagUlRecvSn) ));
                        /* SPR  3444 changes End */
                        tagHeader_p->length = PDCP_HTONS(
                                (UInt16) (PDCP_NTOHS (
                                        (UInt16)(tagHeader_p->length)) + \
                                    (UInt16)PDCP_NTOHS(
                                        tagHeaderUlBitmap_p->length) ));
                        /* SPR 3895 changes end */
                        responseMsg_p->header.length += sizeof(TlvTagHeader);

                        tagUlRecvSn_p = (LP_RrcTagUlRecvSn)&responseMsg_p->
                            data[ TLV_GET_WRITE_POS(responseMsg_p) ];
                        memCpy(tagUlRecvSn_p->ulRecvSn, 
                                pdcpEntityContext_p->pdcpUlStatusInfo.bitmap,
                                BITMAP_SIZE);
                        responseMsg_p->header.length += BITMAP_SIZE;
                    }
                    else
                    {
                        /* No encoding reqd as status report reqd flag is not
                         * set for this particular DRB */
                        LOG_PDCP_MSG( PDCP_NO_UL_BITMAP_SENT, LOGINFO, PDCP_RRCOAM,
                                pdcpCurrentTime_g, __LINE__, 
                                rrcSnHfnStatus_p->header.ueIndex, 
                                pdcpUeCtx_p->activeLcInfo[lcId], 
                                pdcpEntityContext_p->statusReportRequired,0, 
                                0,0,__func__,"");
                    }
                }
                else
                {
                    LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, 
                            PDCP_RRCOAM, pdcpCurrentTime_g, __LINE__, 
                            rrcSnHfnStatus_p->header.ueIndex, /*MS Change*/
                            pdcpUeCtx_p->activeLcInfo[lcId], 0,0, 
                            0,0,__func__,"");
                }
            }
#ifdef LOG_PRINT_ENABLED 
            state = pdcpUeCtx_p->pdcpTxSrb1State;
#endif
            /* Change the TX and RX state to HO from CONNECTED */
            (void)pdcpRrcTxStateMachine[pdcpUeCtx_p->pdcpTxSrb1State]
                [PDCP_HO]( rrcSnHfnStatus_p->header.ueIndex, /*MS Change*/
                        LTE_SRB_LC_ID_LOW_BOUND, 
                        pdcpUeCtx_p->pdcpTxSrb1State, PDCP_HO, 
                        pdcpUeCtx_p );
            LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, rrcSnHfnStatus_p->header.ueIndex, state, PDCP_HO,
                    pdcpUeCtx_p->pdcpTxSrb1State, LTE_SRB_LC_ID_LOW_BOUND, 0, __func__,
                    "PdcpRrcTxStateMachine");

#ifdef LOG_PRINT_ENABLED 
            state = pdcpUeCtx_p->pdcpTxSrb2DrbState;
#endif
            pdcpUeCtx_p->event = PDCP_HO;
            (void)pdcpRrcTxStateMachine[pdcpUeCtx_p->pdcpTxSrb2DrbState]
                [pdcpUeCtx_p->event]( rrcSnHfnStatus_p->header.ueIndex, /*MS Change*/
                        LTE_SRB_LC_ID_HIGH_BOUND, 
                        pdcpUeCtx_p->pdcpTxSrb2DrbState, 
                        pdcpUeCtx_p->event, pdcpUeCtx_p );
            LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, rrcSnHfnStatus_p->header.ueIndex, state, pdcpUeCtx_p->event,
                    pdcpUeCtx_p->pdcpTxSrb2DrbState, LTE_SRB_LC_ID_HIGH_BOUND, 0, __func__,
                    "PdcpRrcTxStateMachine");

            pdcpUeCtx_p->pdcpRxState = PDCP_RX_STATE_HO;
            LOG_PDCP_MSG (PDCP_UE_CONTEXT_STATE_RX, LOGINFO, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, 
                    rrcSnHfnStatus_p->header.ueIndex, pdcpUeCtx_p->pdcpRxState, /*MS Change*/
                    0, 0, 0,0, __func__,"");
        }
    }while (0);

    if ( PDCP_SUCCESS != snHfnStatusResponse_p->responseCode )
    {
        /* +Coverity 243565*/
            LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__,
                    snHfnStatusResponse_p->responseCode, 
                    snHfnStatusResponse_p->ueIndex,0,0, 0,0,__func__, /*MS Change*/
                    "PDCP_SN_HFN_STATUS_REQ" );

        /* -Coverity 243565*/

        snHfnStatusResponse_p->responseCode = PDCP_FAIL;
    }

    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( snHfnStatusResponse_p->ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( snHfnStatusResponse_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* + Coverity_Stack Fix */
    freeMemPool(responseBuf);
    /* - Coverity_Stack Fix */
}

/****************************************************************************
 * Function Name  : rrcCmdSuspendUeEntity
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_SUSPEND_UE_ENTITY_REQ API.
 *                  It sends PDCP_SUSPEND_UE_ENTITY_CNF message to RRC.
 ****************************************************************************/
void rrcCmdSuspendUeEntity( UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }

    /* Coverity 55830 */
    LP_TlvMessage               message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage               responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcSuspendUeEntityReq    rrcSuspendUeReq_p = PNULL;
    LP_RrcSuspendUeEntityCnf    rrcSuspendUeResp_p = PNULL;
    /*Check API id */
    CHECK_EXP( PDCP_SUSPEND_UE_ENTITY_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
        message_p->header.transactionId,
        RRC_MODULE_ID,
        PDCP_SUSPEND_UE_ENTITY_CNF,
        sizeof(RrcSuspendUeEntityCnf)
#ifndef UE_SIM_TESTING
        ,message_p->header.cellIndex
#endif 
        );
    /* - Layer2 NON CA Changes */
        
        /* Prepare pointer to response message data */
    rrcSuspendUeResp_p = (LP_RrcSuspendUeEntityCnf)responseMsg_p->data;
    
    /* Set default values in response message */
    rrcSuspendUeResp_p->header.ueIndex = 0;
    rrcSuspendUeResp_p->responseCode = PDCP_SUCCESS;
    do
    {
        /* SES-526 Fix Start */
        /*Prepare pointer to message data */
        rrcSuspendUeReq_p = (LP_RrcSuspendUeEntityReq)message_p->data;
        
        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( rrcSuspendUeReq_p->header.ueIndex );

        /* check it hear for making response with valid ueIndex */
        rrcSuspendUeResp_p->header.ueIndex = rrcSuspendUeReq_p->header.ueIndex;
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
            pdcpCurrentTime_g, __LINE__, message_p->header.length,
            size, 0,0, 0,0, __func__,"PDCP_SUSPEND_UE_ENTITY_REQ");
            rrcSuspendUeResp_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }
        
        /* Check data size */
        if ( message_p->header.length < PDCP_SUSPEND_UE_ENTITY_REQ_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
            pdcpCurrentTime_g, __LINE__, message_p->header.length,
            PDCP_SUSPEND_UE_ENTITY_REQ_MIN_SIZE, 0,0,
            0,0, __func__,"PDCP_SUSPEND_UE_ENTITY_REQ");
            rrcSuspendUeResp_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }
        
#ifndef UE_SIM_TESTING
          /* SPR 4558 Fix Start */
        rrcSuspendUeReq_p->header.ueIndex = getUeIdxFromRrcUeIdx(rrcSuspendUeReq_p->header.ueIndex);
         /* SPR 4558 Fix End */
#endif
        rrcSuspendUeResp_p->responseCode = pdcpSuspendAllRbEntities(
                                            rrcSuspendUeReq_p->header.ueIndex);
        if ( PDCP_SUCCESS != rrcSuspendUeResp_p->responseCode)
        {
            break;
        }
    } while(0);
    
    /* check if PDCP_PARTIAL_SUCCESS response code should be sent */
    if ( ( PDCP_SUCCESS == rrcSuspendUeResp_p->responseCode ) &&
                /* SPR 4850 Fix Start */
    /* + Layer2 NON CA Changes */
       ( sizeof(TlvHeader) + sizeof(RrcSuspendUeEntityCnf)) <
    /* - Layer2 NON CA Changes */
                /* SPR 4850 Fix End */
         responseMsg_p->header.length )
    {
        rrcSuspendUeResp_p->responseCode = PDCP_PARTIAL_SUCCESS;
    }
       
    if ( (PDCP_SUCCESS != rrcSuspendUeResp_p->responseCode) &&
         (PDCP_PARTIAL_SUCCESS != rrcSuspendUeResp_p->responseCode) )
    {
        LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__,rrcSuspendUeResp_p->responseCode,
                 rrcSuspendUeResp_p->header.ueIndex,0,0, 0,0,__func__,
                 "PDCP_SUSPEND_UE_ENTITY_REQ" );
        rrcSuspendUeResp_p->responseCode = PDCP_FAIL;
    }
        
    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( rrcSuspendUeResp_p->header.ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( rrcSuspendUeResp_p->responseCode );
    
    tlvPrepareHeaderForSending(responseMsg_p);
    
    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : rrcTagCheckRbEntity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_RB_ENTITY RRC tag and converts
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckRbEntity( UInt8* data_p, UInt16 size )
{
    /* check length */
    if ( sizeof(RrcTagRbEntity) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, sizeof(RrcTagRbEntity), 
                 sizeof(data_p),0, 0,0, __func__,"PDCP_RB_ENTITY");
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagResumeRbEntity 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagRbEntity structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_RB_ENTITY RRC tag.
 ****************************************************************************/
UInt16 rrcTagResumeRbEntity( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                        result    = PDCP_SUCCESS;
    LP_RrcTagRbEntity             tagRbEntity_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagRbEntity                tagRbEntity_data = {0};
    /* ALIGNMENT changes end */
    /* SPR 3411 changes start */
    LP_RrcResumeUeEntityReq       tagPrams_p = PNULL;
    /* SPR 3411 changes end */


    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    do 
    {
        /* SPR 3411 changes start */
        tagPrams_p = (LP_RrcResumeUeEntityReq)params;
        /* SPR 3411 changes end */

        /* Prepare pointer to tag data */
        /* ALIGNMENT changes start */
        tagRbEntity_p = &tagRbEntity_data;
        READ_API_MSG(RrcTagRbEntity, tagRbEntity_p, data_p);
        /* ALIGNMENT changes end */
        /* check all tag fields */
        if ( 0 >= tagRbEntity_p->lcId || LTE_MAX_LC_ID < tagRbEntity_p->lcId)
        {
            result = PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
            LOG_PDCP_MSG( PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, tagRbEntity_p->lcId,
                    /* SPR 3411 changes start */
                    tagPrams_p->header.ueIndex, 0,0, 0,0,__func__,"" ); 
            /* SPR 3411 changes end */
            break;
        }
        LOG_PDCP_MSG( PDCP_RESUME_REQ, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                /* SPR 3411 changes start */
                __LINE__, tagPrams_p->header.ueIndex, tagRbEntity_p->lcId,
                tagPrams_p->resumeDirection, 0, 0,0,__func__,"" ); 
        /* SPR 3411 changes end */

        result = pdcpResumeRbEntity(tagPrams_p->header.ueIndex,
                          tagRbEntity_p->lcId,
                          /* SPR 3569 changes start */
                          tagPrams_p->resumeDirection);
                          /* SPR 3569 changes end */
    }while (0);

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, result,size,tagId,0, 0,0,__func__,
                "PDCP_RB_ENTITY" );
        rrcMakeTagError( PDCP_RB_ENTITY_ERROR, tagRbEntity_p->lcId, result,
                responseMessage );
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcCmdResumeUeEntity
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_RESUME_UE_ENTITY_REQ API.
 *                  It sends PDCP_RESUME_UE_ENTITY_CNF message to RRC.
 ****************************************************************************/
void rrcCmdResumeUeEntity(UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }

    /* Coverity 55830 */
    LP_TlvMessage               message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage               responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcResumeUeEntityReq     rrcResumeUeReq_p = PNULL;
    LP_RrcResumeUeEntityCnf     rrcResumeUeResp_p = PNULL;
    /*Check API id */
    CHECK_EXP( PDCP_RESUME_UE_ENTITY_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            RRC_MODULE_ID,
            PDCP_RESUME_UE_ENTITY_CNF,
            sizeof(RrcResumeUeEntityCnf)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif 
            );
    /* - Layer2 NON CA Changes */

    /* Prepare pointer to response message data */
    rrcResumeUeResp_p = (LP_RrcResumeUeEntityCnf)responseMsg_p->data;

    /* Set default values in response message */
    rrcResumeUeResp_p->header.ueIndex = 0;
    rrcResumeUeResp_p->responseCode = PDCP_SUCCESS;
    do
    {   
        /* SES-526 Fix Start */
        /*Prepare pointer to message data */
        rrcResumeUeReq_p = (LP_RrcResumeUeEntityReq)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( rrcResumeUeReq_p->header.ueIndex );
        rrcResumeUeResp_p->header.ueIndex = rrcResumeUeReq_p->header.ueIndex;
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    size, 0,0, 0,0, __func__,"PDCP_RESUME_UE_ENTITY_REQ");
            rrcResumeUeResp_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /* Check data size */
        if ( message_p->header.length < PDCP_RESUME_UE_ENTITY_REQ_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    PDCP_RESUME_UE_ENTITY_REQ_MIN_SIZE, 0,0,
                    0,0, __func__,"PDCP_RESUME_UE_ENTITY_REQ");
            rrcResumeUeResp_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }
        CONVERT_INT16_NETWORK_TO_HOST( rrcResumeUeReq_p->resumeDirection);

        /* SPR 3411 changes start */
        if ( PDCP_ENTITY_RB_BOTH < rrcResumeUeReq_p->resumeDirection  )
        {
            LOG_PDCP_MSG (PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__, rrcResumeUeReq_p->resumeDirection, rrcResumeUeReq_p->header.ueIndex,
                    0, 0, 0,0, __func__,"" );
            rrcResumeUeResp_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
            break;
        }
        /* SPR 3411 changes end */
#ifndef UE_SIM_TESTING
        /* SPR 4558 Fix Start */
        rrcResumeUeReq_p->header.ueIndex = getUeIdxFromRrcUeIdx(rrcResumeUeReq_p->header.ueIndex);
        /* SPR 4558 Fix End */
#endif

        /* parse and check message optional parameters */
        rrcResumeUeResp_p->responseCode = tlvCheckTags(
                rrcResumeUeEntityTagsTable,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcResumeUeEntityReq)], 
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                    sizeof(RrcResumeUeEntityReq) ) );
        if ( PDCP_SUCCESS != rrcResumeUeResp_p->responseCode )
        {
            /* message parsing error */
            break;
        }

        /***** Process optional parameters */
        rrcResumeUeResp_p->responseCode = tlvProcessTags(
                rrcResumeUeEntityTagsTable,
                &rrcResumeUeReq_p->header,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcResumeUeEntityReq)], 
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                    sizeof(RrcResumeUeEntityReq) ), 
                responseMsg_p);

    } while(0);

    /* check if PDCP_PARTIAL_SUCCESS response code should be sent */
    if ( ( PDCP_SUCCESS == rrcResumeUeResp_p->responseCode ) &&
            /* SPR 4850 Fix Start */
    /* + Layer2 NON CA Changes */
         ( sizeof(TlvHeader) + sizeof(RrcResumeUeEntityCnf)) <
    /* - Layer2 NON CA Changes */
            /* SPR 4850 Fix End */
            responseMsg_p->header.length )
    {
        rrcResumeUeResp_p->responseCode = PDCP_PARTIAL_SUCCESS;
    }

    if ( (PDCP_SUCCESS != rrcResumeUeResp_p->responseCode) &&
            (PDCP_PARTIAL_SUCCESS != rrcResumeUeResp_p->responseCode) )
    {
        /* +Coverity 146547*/
            LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                    __LINE__,rrcResumeUeResp_p->responseCode,
                    rrcResumeUeResp_p->header.ueIndex,0,0, 0,0,__func__, /*MS Change */
                    "PDCP_RESUME_UE_ENTITY_REQ" );
        /* -Coverity 146547*/
        rrcResumeUeResp_p->responseCode = PDCP_FAIL;
    }
    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( rrcResumeUeResp_p->header.ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( rrcResumeUeResp_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : rrcTagReEstablishRbEntity 
 * Inputs         : tagId -
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagRbEntity structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_RB_ENTITY RRC tag.
 ****************************************************************************/
UInt16 rrcTagReEstablishRbEntity( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                        result      = PDCP_SUCCESS;
    LP_RrcTagRbEntity             tagRbEntity_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagRbEntity                tagRbEntity_data = {0};
    /* ALIGNMENT changes end */
    LP_RrcTagParams               tagPrams_p = PNULL;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    do 
    {
        tagPrams_p = (LP_RrcTagParams)params;

        /* Prepare pointer to tag data */
    
        /* ALIGNMENT changes start */
        tagRbEntity_p = &tagRbEntity_data;
        READ_API_MSG(RrcTagRbEntity, tagRbEntity_p, data_p);
        /* ALIGNMENT changes end */

        /* check all tag fields */
        if ( 0 >= tagRbEntity_p->lcId || LTE_MAX_LC_ID < tagRbEntity_p->lcId)
        {
            result = PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
            LOG_PDCP_MSG( PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, tagRbEntity_p->lcId,
                    tagPrams_p->ueIndex, 0,0, 0,0,__func__,"" ); 
            break;
        }
        LOG_PDCP_MSG( PDCP_RE_ESTABLISH_REQ, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, tagPrams_p->ueIndex, tagRbEntity_p->lcId, 0,0, 0,0,
                __func__, "" );
        /* Re-establish DL for UE and LC Id */
        result = pdcpReEstablishDl (tagPrams_p->ueIndex, tagRbEntity_p->lcId);
        if ( PDCP_SUCCESS != result )
        {
            break;
        }
        /* Re-establish UL for UE and LC Id */
        result = pdcpReEstablishUl (tagPrams_p->ueIndex, tagRbEntity_p->lcId);
        if ( PDCP_SUCCESS != result )
        {
            break;
        }
    }while (0);
    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, result,size,tagId,0, 0,0,__func__,
                "PDCP_RB_ENTITY" );
        rrcMakeTagError( PDCP_RB_ENTITY_ERROR, tagRbEntity_p->lcId, result,
                responseMessage );
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcCmdReestablishUeEntity
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_RE_ESTABLISH_UE_ENTITY_REQ API.
 *                  It sends PDCP_RE_ESTABLISH_UE_ENTITY_CNF message to RRC.
 ****************************************************************************/
void rrcCmdReestablishUeEntity(UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }

    /* Coverity 55830 */
    LP_TlvMessage   message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage   responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcReestablishUeEntityReq     rrcReestablishUeReq_p = PNULL;
    LP_RrcReestablishUeEntityCnf     rrcReestablishUeResp_p = PNULL;
    /*Check API id */
    CHECK_EXP( PDCP_RE_ESTABLISH_UE_ENTITY_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            RRC_MODULE_ID,
            PDCP_RE_ESTABLISH_UE_ENTITY_CNF,
            sizeof(RrcReestablishUeEntityCnf)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif 
            );
    /* - Layer2 NON CA Changes */

    /* Prepare pointer to response message data */
    rrcReestablishUeResp_p = (LP_RrcReestablishUeEntityCnf)responseMsg_p->data;

    /* Set default values in response message */
    rrcReestablishUeResp_p->header.ueIndex = 0;
    rrcReestablishUeResp_p->responseCode = PDCP_SUCCESS;
    do
    {   
        /* SES-526 Fix Start */
        /*Prepare pointer to message data */
        rrcReestablishUeReq_p = (LP_RrcReestablishUeEntityReq)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( rrcReestablishUeReq_p->header.ueIndex );

        rrcReestablishUeResp_p->header.ueIndex = 
            rrcReestablishUeReq_p->header.ueIndex;
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    size, 0,0, 0,0, __func__,"PDCP_RE_ESTABLISH_UE_ENTITY_REQ");
            rrcReestablishUeResp_p->responseCode = 
                PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /* Check data size */
        if ( message_p->header.length < 
                PDCP_RE_ESTABLISH_UE_ENTITY_REQ_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    PDCP_RE_ESTABLISH_UE_ENTITY_REQ_MIN_SIZE, 0,0,
                    0,0, __func__,"PDCP_RE_ESTABLISH_UE_ENTITY_REQ");
            rrcReestablishUeResp_p->responseCode = 
                PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

#ifndef UE_SIM_TESTING
        /* SPR 4558 Fix Start */
        rrcReestablishUeReq_p->header.ueIndex = getUeIdxFromRrcUeIdx(rrcReestablishUeReq_p->header.ueIndex);
        /* SPR 4558 Fix End */
#endif    
        /* parse and check message optional parameters */
        rrcReestablishUeResp_p->responseCode = tlvCheckTags(
                rrcReEstablishUeEntityTagsTable,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcReestablishUeEntityReq)],
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                    sizeof(RrcReestablishUeEntityReq) ) );
        if ( PDCP_SUCCESS != rrcReestablishUeResp_p->responseCode )
        {
            /* message parsing error */
            break;
        }

        /***** Process optional parameters */
        rrcReestablishUeResp_p->responseCode = tlvProcessTags(
                rrcReEstablishUeEntityTagsTable,
                &rrcReestablishUeReq_p->header,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcReestablishUeEntityReq)], 
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                    sizeof(RrcReestablishUeEntityReq)), 
                responseMsg_p);

    } while(0);

    /* check if PDCP_PARTIAL_SUCCESS response code should be sent */
    if ( ( PDCP_SUCCESS == rrcReestablishUeResp_p->responseCode ) &&
            /* SPR 4850 Fix Start */
    /* + Layer2 NON CA Changes */
            ( sizeof(TlvHeader) + sizeof(RrcReestablishUeEntityCnf) ) <
    /* - Layer2 NON CA Changes */
            /* SPR 4850 Fix End */
            responseMsg_p->header.length )
    {
        rrcReestablishUeResp_p->responseCode = PDCP_PARTIAL_SUCCESS;
    }

    if ( (PDCP_SUCCESS != rrcReestablishUeResp_p->responseCode) &&
            (PDCP_PARTIAL_SUCCESS != rrcReestablishUeResp_p->responseCode) )
    {
        /* +Coverity 243566 */
        LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__,rrcReestablishUeResp_p->responseCode,
                rrcReestablishUeResp_p->header.ueIndex,0,0, 0,0,__func__, /*MS Change */
                "PDCP_RE_ESTABLISH_UE_ENTITY_REQ" );
        /* -Coverity 243566 */
        rrcReestablishUeResp_p->responseCode = PDCP_FAIL;
    }
    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( rrcReestablishUeResp_p->header.ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( rrcReestablishUeResp_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : rrcCmdChangeCrnti
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_CHANGE_CRNTI_REQ API.
 *                  It sends PDCP_CHANGE_CRNTI_CNF message to RRC.
 ****************************************************************************/
void rrcCmdChangeCrnti( UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }

    /* Coverity 55830 */
    LP_TlvMessage               message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage               responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcChangeCrntiReq        rrcChangeCrntiReq_p = PNULL;
    LP_RrcChangeCrntiCnf        rrcChangeCrntiResp_p = PNULL;
    LP_PdcpUeContext            pdcpUeContext_p = PNULL;
    /*Check API id */
    CHECK_EXP( PDCP_CHANGE_CRNTI_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            RRC_MODULE_ID,
            PDCP_CHANGE_CRNTI_CNF,
            sizeof(RrcChangeCrntiCnf)
#ifndef UE_SIM_TESTING
            ,message_p->header.cellIndex
#endif 
            );
    /* - Layer2 NON CA Changes */

    /* Prepare pointer to response message data */
    rrcChangeCrntiResp_p = (LP_RrcChangeCrntiCnf)responseMsg_p->data;

    /* Set default values in response message */
    rrcChangeCrntiResp_p->header.ueIndex = 0;
    rrcChangeCrntiResp_p->responseCode = PDCP_SUCCESS;
    do
    {
        /* SES-526 Fix Start */
        /*Prepare pointer to message data */
        rrcChangeCrntiReq_p = (LP_RrcChangeCrntiReq)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( rrcChangeCrntiReq_p->header.ueIndex );
        rrcChangeCrntiResp_p->header.ueIndex = 
            rrcChangeCrntiReq_p->header.ueIndex;
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    size, 0,0, 0,0, __func__,"PDCP_CHANGE_CRNTI_REQ");
            rrcChangeCrntiResp_p->responseCode = 
                PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }
        /* Check data size */
        if ( message_p->header.length < PDCP_RESUME_UE_ENTITY_REQ_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    PDCP_RESUME_UE_ENTITY_REQ_MIN_SIZE, 0,0,
                    0,0, __func__,"PDCP_CHANGE_CRNTI_REQ");
            rrcChangeCrntiResp_p->responseCode = 
                PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        CONVERT_INT16_NETWORK_TO_HOST( rrcChangeCrntiReq_p->oldCrnti );
        CONVERT_INT16_NETWORK_TO_HOST( rrcChangeCrntiReq_p->newCrnti );

#ifndef UE_SIM_TESTING
        /* SPR 4558 Fix Start */
        rrcChangeCrntiReq_p->header.ueIndex =  getUeIdxFromRrcUeIdx(rrcChangeCrntiReq_p->header.ueIndex);
        /* SPR 4558 Fix End */
#endif

        LOG_PDCP_MSG( PDCP_CHANGE_CRNTI_REQ_RECVD, LOGINFO, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, rrcChangeCrntiReq_p->header.ueIndex,
                rrcChangeCrntiReq_p->oldCrnti, rrcChangeCrntiReq_p->newCrnti, 
                0, 0,0, __func__, "" );

        /* Check UeIndex and if its context exist */                
        pdcpUeContext_p = PDCP_GET_UE_CONTEXT( 
                rrcChangeCrntiReq_p->header.ueIndex );
        if (PNULL == pdcpUeContext_p)
        {
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, rrcChangeCrntiReq_p->
                    header.ueIndex, pdcpContext.isInitialized, 0,0, 0,0,
                    __func__,"");
            rrcChangeCrntiResp_p->responseCode = 
                PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            break;
        }

        /* Check State */
        if ( (pdcpUeContext_p->pdcpTxSrb1State != PDCP_TX_STATE_CONNECTED) || 
                (pdcpUeContext_p->pdcpTxSrb2DrbState != PDCP_TX_STATE_CONNECTED) )
        {
            LOG_PDCP_MSG (PDCP_INVALID_STATE, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, rrcChangeCrntiReq_p->header.ueIndex, 
                    0, pdcpUeContext_p->pdcpTxSrb1State,
                    pdcpUeContext_p->pdcpTxSrb2DrbState, 0, 0, 0, __func__, "");
        }

        if(pdcpUeContext_p->crnti != rrcChangeCrntiReq_p->oldCrnti)
        {
            LOG_PDCP_MSG (PDCP_INVALID_OLD_CRNTI, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, rrcChangeCrntiReq_p->
                    header.ueIndex, rrcChangeCrntiReq_p->oldCrnti, 
                    pdcpUeContext_p->crnti, 0, 0,0, __func__,"");
        }
        pdcpUeContext_p->crnti = rrcChangeCrntiReq_p->newCrnti;

    } while(0);

    /* check if PDCP_PARTIAL_SUCCESS response code should be sent */
    if ( ( PDCP_SUCCESS == rrcChangeCrntiResp_p->responseCode ) &&
            /* SPR 4850 Fix Start */
    /* + Layer2 NON CA Changes */
            ( sizeof(TlvHeader) + sizeof(RrcChangeCrntiCnf) ) <
    /* - Layer2 NON CA Changes */
            /* SPR 4850 Fix End */
            responseMsg_p->header.length )
    {
        rrcChangeCrntiResp_p->responseCode = PDCP_PARTIAL_SUCCESS;
    }

    if ( (PDCP_SUCCESS != rrcChangeCrntiResp_p->responseCode) &&
            (PDCP_PARTIAL_SUCCESS != rrcChangeCrntiResp_p->responseCode) )
    {
        /* +Coverity 243563 */
        LOG_PDCP_MSG( PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, rrcChangeCrntiResp_p->responseCode,
                rrcChangeCrntiResp_p->header.ueIndex, 0,0, 0,0, __func__, /* MS Change */
                "PDCP_CHANGE_CRNTI_REQ" );
        /* -Coverity 243563 */
        rrcChangeCrntiResp_p->responseCode = PDCP_FAIL;
    }
    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( rrcChangeCrntiResp_p->header.ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( rrcChangeCrntiResp_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : rrcTagCheckUlRecvSn 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_UL_RECV_SN RRC tag and converts
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckUlRecvSn( UInt8* data_p, UInt16 size )
{
    UInt16                        result        = PDCP_SUCCESS;

    /* check length */
    if ( sizeof(RrcTagUlRecvSn) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, size, 
                sizeof(RrcTagUlRecvSn),sizeof(data_p),0, 0,0, __func__,
                "PDCP_UL_RECV_SN");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    else 
    {
        /* Tag fields values will be checked when it will be processed after checking */
        /* No optional parameters. */
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagUlRecvSn 
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                       It should be pointer to RrcTagSnHfnStatus structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_UL_RECV_SN RRC tag.
 ****************************************************************************/
UInt16 rrcTagUlRecvSn( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                   result        = PDCP_SUCCESS;
    LP_RrcTagUlRecvSn        tagUlRecvSn_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagUlRecvSn           tagUlRecvSn_data;
    /* ALIGNMENT changes end */
    RrcTagParams             tagPrams      = {0};
    LP_PdcpEnityContext      pdcpEntityContext_p = PNULL;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams.ueIndex = ((LP_RrcTagParams)params)->ueIndex;
    tagPrams.lcId    = ((LP_RrcTagParams)params)->lcId;

    /* Prepare pointer to tag data */
    /* ALIGNMENT changes start */
    tagUlRecvSn_p = &tagUlRecvSn_data;
    READ_API_MSG(RrcTagUlRecvSn, tagUlRecvSn_p, data_p);
    /* ALIGNMENT changes end */
    pdcpEntityContext_p = pdcpGetEntityContext( 
            tagPrams.ueIndex, tagPrams.lcId, 
            PDCP_ENTITY_RB_RX);
    if ( (PNULL != pdcpEntityContext_p) && 
            (pdcpEntityContext_p->rlcMode == PDCP_ENTITY_RLC_AM))
    {
        memCpy( pdcpEntityContext_p->pdcpUlStatusInfo.bitmap, 
                tagUlRecvSn_p->ulRecvSn, BITMAP_SIZE);
    }
    else
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex, tagPrams.lcId,
                0, 0, 0,0, __func__,"");
        result = PDCP_ERR_ENTITY_NOT_FOUND; 
    }
    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                "PDCP_RB_ENTITY" );
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckSnHfnStatus 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_LC_SN_HFN_STATUS RRC tag and converts
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckSnHfnStatus( UInt8* data_p, UInt16 size )
{
    UInt16                        result      = PDCP_SUCCESS;
    /* check length */
    if ( sizeof(RrcTagSnHfnStatus) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, size, 
                sizeof(RrcTagSnHfnStatus),sizeof(data_p),0, 0,0, __func__,
                "PDCP_LC_SN_HFN_STATUS");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagSnHfnStatus 
 * Inputs         : tagID - 
 *                  params - pointer to tag processing parameters.
 *                  It should be pointer to RrcTagSnHfnStatus structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_LC_SN_HFN_STATUS RRC tag.
 ****************************************************************************/
UInt16 rrcTagSnHfnStatus( UInt16 tagId, void* params, UInt8* data_p, 
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                   result      = PDCP_SUCCESS;
    LP_RrcTagSnHfnStatus     tagSnHfnStatus_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagSnHfnStatus        tagSnHfnStatus_data = {0};
    /* ALIGNMENT changes end */
    RrcTagParams             tagPrams = {0};
    LP_PdcpEnityContext      pdcpEntityContext_p = PNULL;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    /* Prepare pointer to tag data */
    /* ALIGNMENT changes start */
    tagSnHfnStatus_p = &tagSnHfnStatus_data;
    READ_API_MSG(RrcTagSnHfnStatus, tagSnHfnStatus_p, data_p);
    /* ALIGNMENT changes end */

    tagPrams.ueIndex = ((LP_RrcHeader)params)->ueIndex;
    tagPrams.lcId    = tagSnHfnStatus_p->lcId;
    /* check all tag fields */
    pdcpEntityContext_p = pdcpGetEntityContext(
            tagPrams.ueIndex, tagSnHfnStatus_p->lcId,
            PDCP_ENTITY_RB_TX);
    /* Set state variables at TX side */
    if ( PNULL != pdcpEntityContext_p ) 
    {
        if ( PDCP_ENTITY_RLC_AM == pdcpEntityContext_p->rlcMode )
        {
            pdcpEntityContext_p->next_SN = tagSnHfnStatus_p->dlSnCnt;
            pdcpEntityContext_p->hfn = tagSnHfnStatus_p->dlHfnCnt;

            LOG_PDCP_MSG( PDCP_STATE_VARS_RESET_TX, LOGDEBUG, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex, 
                    tagSnHfnStatus_p->lcId, pdcpEntityContext_p->next_SN,
                    pdcpEntityContext_p->hfn, 0, 0, __func__, "" );
        }
    }
    else
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex, 
                tagSnHfnStatus_p->lcId, 0,0, 0,0,__func__,"");
    }

    /* Set state variables at RX side */
    pdcpEntityContext_p = pdcpGetEntityContext(
            tagPrams.ueIndex, tagSnHfnStatus_p->lcId,
            PDCP_ENTITY_RB_RX);
    if ( PNULL != pdcpEntityContext_p ) 
    {
        if ( pdcpEntityContext_p->rlcMode == PDCP_ENTITY_RLC_AM )
        {            
            /* SPR 3589 changes start */
            if(0 == tagSnHfnStatus_p->ulSnCnt)
            {
                pdcpEntityContext_p->last_Submitted_SN = 
                    PDCP_TS_MAX_SN_AM_MODE;
            }
            else
            {
                pdcpEntityContext_p->last_Submitted_SN = 
                    tagSnHfnStatus_p->ulSnCnt - 1;
            }
            /* SPR 3589 changes end */
            pdcpEntityContext_p->hfn = tagSnHfnStatus_p->ulHfnCnt;
            /* SPR 3589 changes start */
            pdcpEntityContext_p->next_SN =
                tagSnHfnStatus_p->ulSnCnt;
            /* SPR 3589 changes end */
            pdcpEntityContext_p->fms = 
                (pdcpEntityContext_p->last_Submitted_SN + 1) %
                (PDCP_TS_MAX_SN_AM_MODE + 1);

            LOG_PDCP_MSG( PDCP_STATE_VARS_RESET_RX, LOGDEBUG, PDCP_RRCOAM,
                    pdcpCurrentTime_g, tagPrams.ueIndex, 
                    tagSnHfnStatus_p->lcId, pdcpEntityContext_p->next_SN,
                    pdcpEntityContext_p->hfn, 
                    pdcpEntityContext_p->last_Submitted_SN, 
                    0,0, __func__, "AM Mode" );
        }
    }
    else
    {
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_RX, LOGWARNING, PDCP_RRCOAM,
                 pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex, 
                 tagSnHfnStatus_p->lcId, 0,0, 0,0,__func__,"");
    }
    
    result = tlvCheckTags(rrcLcSnHfnTagsTable,
                 /* pointer to the optional parameters */
                 &data_p[sizeof(RrcTagSnHfnStatus)],
                 /* size of optional parameters */
                 (size - sizeof(RrcTagSnHfnStatus)) );
    if ( PDCP_SUCCESS == result )
    {    
        /***** Process parameters */
        result = tlvProcessTags(rrcLcSnHfnTagsTable,
                     &tagPrams,
                     /* pointer to the optional parameters */
                     &data_p[sizeof(RrcTagSnHfnStatus)], 
                     /* size of optional parameters */
                     (size - sizeof(RrcTagSnHfnStatus)), 
                     responseMessage);
    }       
    if ( PDCP_SUCCESS != result )          
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,tagId,0,0, 0,0,__func__,
                 "PDCP_RB_ENTITY" );
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcCmdSnHfnStatusInd
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_SN_HFN_STATUS_IND API.
 ****************************************************************************/
void rrcCmdSnHfnStatusInd( UInt8* data_p, UInt16 size )
{
    UInt16 result = 0;
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }

    /* Coverity 55830 */
    LP_TlvMessage        message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage        responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcSnHfnStatusInd rrcSnHfnStatusInd_p = PNULL;
    /*Check API id */
    CHECK_EXP( PDCP_SN_HFN_STATUS_IND != message_p->header.apiId );

    do
    {
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, message_p->header.length,
                     size, 0,0, 0,0, __func__,"PDCP_SN_HFN_STATUS_IND");
            break;
        }

        /* Check data size */
        /* SPR 6711 fix start */
        if ( message_p->header.length < PDCP_RRC_PDCP_SN_HFN_STATUS_IND_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, message_p->header.length,
                     PDCP_RRC_PDCP_SN_HFN_STATUS_IND_MIN_SIZE, 0,0,
                     0,0, __func__,"PDCP_SN_HFN_STATUS_IND");
            break;
        }
        /* SPR 6711 fix end */
        /*Prepare pointer to message data */
        rrcSnHfnStatusInd_p = (LP_RrcSnHfnStatusInd)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( rrcSnHfnStatusInd_p->header.ueIndex );

#ifndef UE_SIM_TESTING 
/* SPR 4558 Fix Start */
        rrcSnHfnStatusInd_p->header.ueIndex = getUeIdxFromRrcUeIdx(rrcSnHfnStatusInd_p->header.ueIndex);
/* SPR 4558 Fix End */
#endif

        /* parse and check message optional parameters */
        result = tlvCheckTags(rrcSnHfnStatusTagsTable,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcSnHfnStatusInd)],
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                    sizeof(RrcSnHfnStatusInd) ) 
                );
        if (PDCP_SUCCESS != result )
        {
            /* Message parsing fails */
            break;
        }
        /***** Process optional parameters */
        result = tlvProcessTags(rrcSnHfnStatusTagsTable,
                        &rrcSnHfnStatusInd_p->header,
                        /* pointer to the optional parameters */
                        &message_p->data[sizeof(RrcSnHfnStatusInd)], 
                        /* size of optional parameters */
                        TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                            sizeof(RrcSnHfnStatusInd)), 
                        responseMsg_p);

    } while(0);
    if ( PDCP_SUCCESS != result )          
    {
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result, 0,0,0, 0,0,__func__,
                 "PDCP_RB_ENTITY" );
    }
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : rrcCmdDataBufferStop
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_DATA_BUFFER_STOP_IND API.
 ****************************************************************************/
void rrcCmdDataBufferStop( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage               message_p = (LP_TlvMessage)data_p;
    LP_RrcDataBufferStopInd     rrcDataBufferStopInd_p = PNULL;
    LP_PdcpUeContext            ueCtx_p = PNULL;
    /* SRP 3608 changes start */
    UInt8               drbLcId = 0;
    LP_PdcpEnityContext ctx_p = PNULL;
    /* Lossy HO changes start */
    LP_PdcpEnityContext tx_ctx_p = PNULL;
    /* Lossy HO changes stop */
    /* SPR 3608 changes end */
    /*Check API id */
    CHECK_EXP( PDCP_DATA_BUFFER_STOP_IND != message_p->header.apiId );

    do
    {
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    size, 0,0, 0,0, __func__,"PDCP_DATA_BUFFER_STOP_IND");
            break;
        }

        /* Check data size */
        if ( message_p->header.length < PDCP_DATA_BUFFER_STOP_IND_MIN_SIZE )
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, message_p->header.length,
                    PDCP_DATA_BUFFER_STOP_IND_MIN_SIZE, 0,0,
                    0,0, __func__,"PDCP_DATA_BUFFER_STOP_IND");
            break;
        }
        /*Prepare pointer to message data */
        rrcDataBufferStopInd_p = (LP_RrcDataBufferStopInd)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( rrcDataBufferStopInd_p->header.ueIndex );
#ifndef UE_SIM_TESTING
        /* SPR 4558 Fix Start */
        rrcDataBufferStopInd_p->header.ueIndex = getUeIdxFromRrcUeIdx(
                                         rrcDataBufferStopInd_p->header.ueIndex);
        /* SPR 4558 Fix End */
#endif
        /* check it hear for making response with valid ueIndex */        
        ueCtx_p = PDCP_GET_UE_CONTEXT(rrcDataBufferStopInd_p->header.ueIndex );
        if (PNULL == ueCtx_p)
        {
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__, rrcDataBufferStopInd_p->
                    header.ueIndex, pdcpContext.isInitialized, 0,0, 
                    0,0, __func__,"");
            break;
        }

        /* + SPR  20549 */
        if( PDCP_TX_STATE_HO != ueCtx_p->pdcpTxSrb2DrbState )
        {
            LOG_PDCP_ERROR( PDCP_RRCOAM, "Unable to handle "
                    "PDCP_DATA_BUFFER_STOP_IND as it is received in "
                    "incorrect state[%d] for UE[%d]", 
                    ueCtx_p->pdcpTxSrb2DrbState, 
                    rrcDataBufferStopInd_p->header.ueIndex );
            break;
        }
        /* - SPR  20549 */

        /* SPR 16973 fix start */
        /* Set to Connected state  for SRB1 */
        (void)pdcpRrcTxStateMachine[ueCtx_p->pdcpTxSrb1State]
            [PDCP_CONNECTED] ( rrcDataBufferStopInd_p->header.ueIndex,
                    LTE_SRB_LC_ID_LOW_BOUND, ueCtx_p->pdcpTxSrb1State, 
                    PDCP_CONNECTED, ueCtx_p );

         LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                          __LINE__, rrcDataBufferStopInd_p->header.ueIndex,0, PDCP_CONNECTED,
                          ueCtx_p->pdcpTxSrb1State, LTE_SRB_LC_ID_LOW_BOUND, 0, __func__,
                          "PdcpRrcTxStateMachine");
        /* SRP 3608 changes start */
        /* Initialize event by connected for no status report to be exchanged
         *  between UE and eNB */
        ueCtx_p->event = PDCP_CONNECTED;

        /*if UE PSR required is set*/
        for( drbLcId = 0; drbLcId < ueCtx_p->numOfActiveDrbLc; drbLcId++)
        {
            /* SPR 5485 changes start */
            ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p,
                      ueCtx_p->activeLcInfo[drbLcId], PDCP_ENTITY_RB_RX );
            /* Lossy HO changes start */
            tx_ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT( ueCtx_p, 
                    ueCtx_p->activeLcInfo[drbLcId], PDCP_ENTITY_RB_TX );
            /* SPR 5485 changes end */
            tx_ctx_p->lossyHoRab = LTE_FALSE;
            /* Lossy HO changes stop */
            if( LTE_TRUE ==  ueCtx_p->hoTriggerRecvd )
            {
                /* This is target eNB */
#ifdef UE_SIM_TESTING                    
            if ( LTE_TRUE == ctx_p->ueStatusReportReq ) 
#else                            
            if ( LTE_TRUE == ctx_p->statusReportRequired ) 
#endif
            {
                /* Encode PSR and put it in CtrlPDU Q to be sent to RLC */
                pdcpEncodePSR( rrcDataBufferStopInd_p->header.ueIndex,
                        ueCtx_p->activeLcInfo[drbLcId], ctx_p );
                ueCtx_p->event = PDCP_AWAITED;
                (ueCtx_p->totNumOfPSRtoSend)++;
            }
#ifndef UE_SIM_TESTING 
            if( LTE_TRUE == ctx_p->ueStatusReportReq ) 
#else
            if( LTE_TRUE == ctx_p->statusReportRequired )
#endif
            {
                /* PSR is expected from peer so start the PSR expiry timer and
                 * set the state to PDCP_AWAITED */
                ueCtx_p->pdcpPSRTimer = pdcpCurrentTime_g;
                ueCtx_p->event = PDCP_AWAITED;
                (ueCtx_p->totNumPsrRecvd)++;
            } 
        }
            else
            {
                /* This is source eNB for HO cancel scenario */
                /* Reset HO (forwarding) related flags */
                tx_ctx_p->isFwdingEnabled = LTE_FALSE;
                tx_ctx_p->isEndMarkerIndRecvd = LTE_FALSE;
                tx_ctx_p->isEMIFwded = LTE_FALSE;
                tx_ctx_p->numberOfNodesFwded = LTE_FALSE;
                tx_ctx_p->dataNode = PNULL;
            }
        }
        /* Reset hoTriggerRecvd */
        ueCtx_p->hoTriggerRecvd = LTE_FALSE;
        ueCtx_p->isFwdingApiRecvd = LTE_FALSE;
        /* SPR 16973 fix end */
        (void)pdcpRrcTxStateMachine[ueCtx_p->pdcpTxSrb2DrbState]
                                      [ueCtx_p->event] ( 
                          rrcDataBufferStopInd_p->header.ueIndex,
                          LTE_SRB_LC_ID_HIGH_BOUND, 
                          ueCtx_p->pdcpTxSrb2DrbState, ueCtx_p->event, 
                          ueCtx_p );

        LOG_PDCP_MSG( PDCP_UE_CONTEXT_STATE_TX, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                          __LINE__, rrcDataBufferStopInd_p->header.ueIndex,0, ueCtx_p->event,
                          ueCtx_p->pdcpTxSrb2DrbState, LTE_SRB_LC_ID_HIGH_BOUND, 0, __func__,
                          "PdcpRrcTxStateMachine");
        /* If event is AWAITED, State is changed to RESUME after sending PSR.
         * In RESUME state, it will process already processed nodes (applicable
         * in case of Intra-eNB HO). Once it checks for already processed 
         * nodes, the state changes CONNECTED to process other packets. 
         * 
         * If event is CONNECTED, State is changed to CONNECTED to process new 
         * packets, since no PSR needs to be sent/received. */
        
        ueCtx_p->pdcpRxState = PDCP_RX_STATE_CONNECTED;
        LOG_PDCP_MSG (PDCP_UE_CONTEXT_STATE_RX, LOGINFO, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, rrcDataBufferStopInd_p->header.
                ueIndex, ueCtx_p->pdcpRxState, 0, 0, 0,0, __func__,"");

    } while(0);
}

/*SPR 3608 changes start*/
/****************************************************************************
 * Function Name  : rrcTagConfigureUePsr
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_CONFIGURE_UE_PSR RRC tag.
 ****************************************************************************/
UInt16 rrcTagConfigureUePsr( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    LP_RrcTagParams             tagPrams_p = PNULL;
    LP_RrcTagConfigureUePsr     tagConfigUePsr_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagConfigureUePsr        tagConfigUePsr_data = {0};
    /* ALIGNMENT changes end */
    UInt16                      result  = 0;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Prepare pointer to tag data */
    
    /* ALIGNMENT changes start */
    tagConfigUePsr_p = &tagConfigUePsr_data;
    READ_API_MSG(RrcTagConfigureUePsr, tagConfigUePsr_p, data_p);
    /* ALIGNMENT changes end */
    if ( LTE_TRUE < tagConfigUePsr_p->configureUePsr)
    {
	result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
        LOG_PDCP_MSG (PDCP_INVALID_STATUS_REPORT_VALUE, LOGWARNING, PDCP_RRCOAM,
                 pdcpCurrentTime_g, __LINE__, tagPrams_p->ueIndex,
                 tagPrams_p->lcId,
                 tagConfigUePsr_p->configureUePsr,
                 0, 0,0,__func__,"PDCP_CONFIGURE_UE_PSR" );
    }
    else
    {
        LOG_PDCP_MSG (PDCP_CONF_STATUS_REPRT, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__, tagPrams_p->ueIndex, tagPrams_p->lcId,
                 tagPrams_p->rbDirection,
                 tagConfigUePsr_p->configureUePsr,
                 0,0,__func__,"" );
        /* configure entity */
        result = pdcpEntityConfigureUeStatusReportReq(
                tagPrams_p->ueIndex,
                tagPrams_p->lcId,
                tagPrams_p->rbDirection,
                tagConfigUePsr_p->configureUePsr);

        /* No optional parameters. */
    }
     if ( PDCP_SUCCESS != result )
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM,
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                 "PDCP_CONFIGURE_UE_PSR" );
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckConfigureUePsr
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It checks PDCP_CONFIGURE_UE_PSR RRC tag and converts
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckConfigureUePsr( UInt8* data_p, UInt16 size )
{
    /* check length */
    if ( sizeof(RrcTagConfigureUePsr) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                 pdcpCurrentTime_g, __LINE__, size, sizeof(RrcTagConfigureUePsr),
                 sizeof(data_p),0, 0,0, __func__, "PDCP_CONFIGURE_UE_PSR");
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }

    return PDCP_SUCCESS;
}
/*SPR 3608 changes end*/

/* SPR 3607 changes Start */
/****************************************************************************
 * * Function Name  : pdcpSendCountWraparoundInd
 * * Inputs         : ueIndex - On which Count Wraparound has been done.
 * * Outputs        : None.
 * * Returns        : None.
 * * Description    : It sends PDCP_COUNT_WRAPAROUND_IND message to RRC.
 * ****************************************************************************/
void pdcpSendCountWraparoundInd( UInt16 ueIndex )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
        LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
                pdcpCurrentTime_g, __LINE__, 
                PDCP_RRC_CNF_MAX_SIZE,
                0,0,0,0.0,0.0,__FILE__,__func__);
        return;
    }

    /* Coverity 55830 */
    LP_TlvMessage    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcCountWraparoundInd   rrcCountWraparoundInd_p = PNULL;

    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    RrcCellIndex cellId = PDCP_GET_RRC_CELL_INDEX( pdcpContext.\
            ueContexts[ueIndex]->cellIndex );
#endif
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            1000,
            RRC_MODULE_ID,
            PDCP_COUNT_WRAPAROUND_IND,
            sizeof(RrcCountWraparoundInd)
#ifndef UE_SIM_TESTING
            ,cellId 
#endif 
            );
    /* - Layer2 NON CA Changes */
    /* Prepare pointer to response message data */
    rrcCountWraparoundInd_p = (LP_RrcCountWraparoundInd)responseMsg_p->data;

    /* Set default values in response message */
#ifndef UE_SIM_TESTING
    rrcCountWraparoundInd_p->header.ueIndex = pdcpContext.ueContexts[ueIndex]->rrcUeIndex;
#else
    rrcCountWraparoundInd_p->header.ueIndex = ueIndex;
#endif

    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( rrcCountWraparoundInd_p->header.ueIndex );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}
/* SPR 3607 changes End */

/*SPR 3607 Fix start*/
/****************************************************************************
 * Function Name  : rrcTagSecurityOffset
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS or Error code
 * Description    : It processes PDCP_SECURITY_OFFSET RRC tag.
 ****************************************************************************/
UInt16 rrcTagSecurityOffset( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    LP_RrcTagParams             tagPrams_p = PNULL;
    LP_RrcTagSecurityOffset     tagSecurityOffset_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagSecurityOffset        tagSecurityOffset = {0};
    /* ALIGNMENT changes end */
    UInt16                      result  = 0;
    UInt16                      lcCtr  = 0;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams_p = (LP_RrcTagParams)params;

    /* Prepare pointer to tag data */
    
    /* ALIGNMENT changes start */
    tagSecurityOffset_p = &tagSecurityOffset;
    READ_API_MSG(RrcTagSecurityOffset, tagSecurityOffset_p, data_p);
    /* ALIGNMENT changes end */

    if ( LTE_TRUE != tagSecurityOffset_p->securityOffset)
    {
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
        LOG_PDCP_MSG (PDCP_INVALID_STATUS_REPORT_VALUE, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, tagPrams_p->ueIndex,
                tagSecurityOffset_p->securityOffset, 0,
                0,0,0,__func__,"PDCP_SECURITY_OFFSET" );
    }
    else
    {
        LOG_PDCP_MSG (PDCP_CONF_STATUS_REPRT, LOGINFO, PDCP_RRCOAM, pdcpCurrentTime_g,
                __LINE__, tagPrams_p->ueIndex, 
                tagSecurityOffset_p->securityOffset,
                0,0,0,0,__func__,"" );
        /* configure entity */
        for ( lcCtr = LTE_SRB_LC_ID_LOW_BOUND;
                lcCtr <= LTE_SRB_LC_ID_HIGH_BOUND; lcCtr++)
        {

            result = pdcpEntityConfigureSecurityOffset(
                    tagPrams_p->ueIndex, lcCtr);
        }
        /* No optional parameters. */
    }
    if ( PDCP_SUCCESS != result )
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, result,
                size,tagId,responseMessage->header.transactionId,0,0, __func__,
                "PDCP_SECURITY_OFFSET" );
    }

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckSecurityOffset
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH
 * Description    : It checks PDCP_CONFIGURE_UE_PSR RRC tag and converts
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckSecurityOffset( UInt8* data_p, UInt16 size )
{
    /* check length */
    if ( sizeof(RrcTagSecurityOffset) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                 pdcpCurrentTime_g, __LINE__, size, sizeof(RrcTagSecurityOffset),
                 sizeof(data_p),0, 0,0, __func__, "PDCP_CONFIGURE_UE_PSR");
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }

    return PDCP_SUCCESS;
}
/*SPR 3607 Fix end*/

/* SPR 3175 changes Start */
/****************************************************************************
 * * Function Name  : pdcpSendNotifyIntegrityFailureInd
 * * Inputs         : ueIndex - On which Count Wraparound has been done.
 * *                  lcId -          
 * * Outputs        : None.
 * * Returns        : None.
 * * Description    : It sends PDCP_COUNT_WRAPAROUND_IND message to RRC.
 * ****************************************************************************/
void pdcpSendNotifyIntegrityFailureInd( UInt16 ueIndex, UInt16 lcId )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }

    /* Coverity 55830 */
    LP_TlvMessage    responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcNotifyIntegrityFailureInd   rrcNotifyIntegrityFailureInd_p = PNULL;

    /* + Layer2 NON CA Changes */
#ifndef UE_SIM_TESTING
    RrcCellIndex cellId = PDCP_GET_RRC_CELL_INDEX( pdcpContext.\
                                            ueContexts[ueIndex]->cellIndex );
#endif
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            1000,
            RRC_MODULE_ID,
            PDCP_NOTIFY_INTEGRITY_FAILURE,
            sizeof(RrcNotifyIntegrityFailureInd)
#ifndef UE_SIM_TESTING
            ,cellId
#endif 
            );
    /* - Layer2 NON CA Changes */
    /* Prepare pointer to response message data */
    rrcNotifyIntegrityFailureInd_p = (LP_RrcNotifyIntegrityFailureInd)responseMsg_p->data;

    /* Set default values in response message */
    rrcNotifyIntegrityFailureInd_p->header.ueIndex = ueIndex;
    rrcNotifyIntegrityFailureInd_p->lcId = lcId;

    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( rrcNotifyIntegrityFailureInd_p->header.ueIndex );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/* SPR 3175 changes End */

#ifndef UE_SIM_TESTING
/****************************************************************************
 * Function Name  : rrcTagCheckSpsDlInfo
 * Inputs         : tagId - 
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 *                  responseMessage -
 * Outputs        : None.
 * Returns        : UInt16
 * Description    : It checks PDCP_SPS_CONFIG RRC tag and converts 
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagSpsConfig( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16              result         = PDCP_SUCCESS;
    RrcTagParams        tagPrams       = {0};

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagPrams.ueIndex = ((LP_RrcTagParams)params)->ueIndex;
    do
    {
        result = tlvCheckTags(rrcSpsConfigTagsTable,
                     /* pointer to the optional parameters */
                     data_p,
                     /* size of optional parameters */
                     size  );
        if ( PDCP_SUCCESS != result )    
        {
            break;
        }
        /***** Process parameters */
        result = tlvProcessTags(rrcSpsConfigTagsTable,
                     &tagPrams,
                     /* pointer to the optional parameters */
                     data_p, 
                     /* size of optional parameters */
                     size, 
                     responseMessage);
    } while( 0 );
    
    if ( PDCP_SUCCESS != result )          
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,tagId,0,0, 0,0,__func__,
                 "PDCP_SPS_CONFIG" );
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckSpsDlInfo
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH
 * Description    : It checks PDCP_SPS_DL_INFO RRC tag and converts 
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckSpsDlInfo( UInt8* data_p, UInt16 size )
{
    /* check length */
    if( sizeof(RrcTagSpsDlInfo) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, sizeof(RrcTagSpsDlInfo),
                 sizeof(data_p),0, 0,0, __func__, "PDCP_SPS_DL_INFO");
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;            
    }

    return PDCP_SUCCESS;  
}

/****************************************************************************
 * Function Name  : rrcTagSpsDlInfo
 * Inputs         : tagId -
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : UInt16
 * Description    : It processes PDCP_SPS_DL_INFO RRC tag.
 ****************************************************************************/
UInt16 rrcTagSpsDlInfo( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16              result         = PDCP_SUCCESS;
    LP_RrcTagSpsDlInfo  tagSpsDlInfo_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagSpsDlInfo     tagSpsDlInfo_data = {0};
    /* ALIGNMENT changes end */
    RrcTagParams        tagPrams       = {0};
    LP_PdcpUeContext    ueCtx_p        = PNULL;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    /* Prepare pointer to tag data */
    /* ALIGNMENT changes start */
    tagSpsDlInfo_p = &tagSpsDlInfo_data;
    READ_API_MSG(RrcTagSpsDlInfo, tagSpsDlInfo_p, data_p);
    /* ALIGNMENT changes end */
    tagPrams.ueIndex = ((LP_RrcTagParams)params)->ueIndex;

    do
    {
        /* Check SPS crnti */
        if ( LTE_TRUE < tagSpsDlInfo_p->requestType )
        {
            LOG_PDCP_MSG( PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__,
                    PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM, tagPrams.ueIndex,
                    0,0, 0,0, __func__,"");
            result = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            break;
        }

        /* Get UE context */
        ueCtx_p = PDCP_GET_UE_CONTEXT(tagPrams.ueIndex); 
        if ( PNULL == ueCtx_p)
        {
            /* Invalid UE Context */
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex,
                     pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
            result = PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            break;
        }
        if( LTE_TRUE == tagSpsDlInfo_p->requestType && 
            LTE_TRUE == ueCtx_p->dlSpsReqType )
        {
            LOG_PDCP_MSG( PDCP_MULTIPLE_SPS_SETUP_NOT_VALID, LOGWARNING, 
                    PDCP_RRCOAM, pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex,
                    tagSpsDlInfo_p->requestType, ueCtx_p->dlSpsReqType, 0,
                    0,0, __func__,"");
            /* Already SPS Configured hence Ignore It */
            break;
        }
        ueCtx_p->dlSpsReqType = tagSpsDlInfo_p->requestType;
      
        pdcpEntitySetsSpsLc( tagPrams.ueIndex, ueCtx_p, PDCP_ENTITY_RB_TX );

        result = tlvCheckTags(rrcSpsDlInfoTagsTable,
                     /* pointer to the optional parameters */
                     &data_p[sizeof(RrcTagSpsDlInfo)],
                     /* size of optional parameters */
                    (size - sizeof(RrcTagSpsDlInfo)) );
        if ( PDCP_SUCCESS != result )    
        {
            break;
        }
        /***** Process parameters */
        result = tlvProcessTags(rrcSpsDlInfoTagsTable,
                     &tagPrams,
                     /* pointer to the optional parameters */
                     &data_p[sizeof(RrcTagSpsDlInfo)], 
                     /* size of optional parameters */
                     (size - sizeof(RrcTagSpsDlInfo)), 
                     responseMessage);
    } while (0);
    if ( PDCP_SUCCESS != result )          
    {
        /* Reset as DL Setup info has failed */
        if( PNULL != ueCtx_p )
        {
            ueCtx_p->dlSpsReqType = LTE_FALSE;
            pdcpEntitySetsSpsLc( tagPrams.ueIndex, ueCtx_p, 
                                 PDCP_ENTITY_RB_TX );
        }
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,tagId,0,0, 0,0,__func__,
                 "PDCP_SPS_DL_INFO" );
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckSpsUlInfo
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH
 * Description    : It checks PDCP_SPS_UL_INFO RRC tag and converts 
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckSpsUlInfo( UInt8* data_p, UInt16 size )
{
    /* check length */
    if( sizeof(RrcTagSpsUlInfo) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, sizeof(RrcTagSpsUlInfo),
                 sizeof(data_p),0, 0,0, __func__, "PDCP_SPS_UL_INFO");
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;            
    }

    return PDCP_SUCCESS;  
}

/****************************************************************************
 * Function Name  : rrcTagSpsUlInfo
 * Inputs         : tagId -
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : UInt16
 * Description    : It processes PDCP_SPS_UL_INFO RRC tag.
 ****************************************************************************/
UInt16 rrcTagSpsUlInfo( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{   
    UInt16              result         = PDCP_SUCCESS;
    LP_RrcTagSpsUlInfo  tagSpsUlInfo_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagSpsUlInfo     tagSpsUlInfo_data = {0};
    /* ALIGNMENT changes end */
    RrcTagParams        tagPrams       = {0};
    LP_PdcpUeContext    ueCtx_p        = PNULL;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    /* Prepare pointer to tag data */
    /* ALIGNMENT changes start */
    tagSpsUlInfo_p = &tagSpsUlInfo_data;
    READ_API_MSG(RrcTagSpsUlInfo, tagSpsUlInfo_p, data_p);
    /* ALIGNMENT changes end */
    tagPrams.ueIndex = ((LP_RrcTagParams)params)->ueIndex;

    do
    {
        /* Check Request Type */
        if ( LTE_TRUE < tagSpsUlInfo_p->requestType )
        {
            LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__,
                    PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM, tagPrams.ueIndex,
                    0,0, 0,0, __func__,"");
            result = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            break;
        }

        /* Get UE context */
        ueCtx_p = PDCP_GET_UE_CONTEXT(tagPrams.ueIndex); 
        if ( PNULL == ueCtx_p)
        {
            /* Invalid UE Context */
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex,
                     pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
            result = PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            break;
        }
        if( LTE_TRUE == tagSpsUlInfo_p->requestType && 
            LTE_TRUE == ueCtx_p->ulSpsReqType )
        {
            LOG_PDCP_MSG( PDCP_MULTIPLE_SPS_SETUP_NOT_VALID, LOGWARNING, 
                    PDCP_RRCOAM, pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex,
                    tagSpsUlInfo_p->requestType, ueCtx_p->ulSpsReqType, 0,
                    0,0, __func__,"");
            /* Already SPS Configured hence Ignore It */
            break;
        }
        ueCtx_p->ulSpsReqType = tagSpsUlInfo_p->requestType;
        
        pdcpEntitySetsSpsLc( tagPrams.ueIndex, ueCtx_p, PDCP_ENTITY_RB_RX );
        
        result = tlvCheckTags(rrcSpsUlInfoTagsTable,
                     /* pointer to the optional parameters */
                     &data_p[sizeof(RrcTagSpsUlInfo)],
                     /* size of optional parameters */
                    (size - sizeof(RrcTagSpsUlInfo)) );
        if ( PDCP_SUCCESS != result )    
        {
            break;
        }
        /***** Process parameters */
        result = tlvProcessTags(rrcSpsUlInfoTagsTable,
                     &tagPrams,
                     /* pointer to the optional parameters */
                     &data_p[sizeof(RrcTagSpsUlInfo)], 
                     /* size of optional parameters */
                     (size - sizeof(RrcTagSpsUlInfo)), 
                     responseMessage);
    } while (0);
    if ( PDCP_SUCCESS != result )          
    {
        /* Reset as UL Setup info has failed */
        if( PNULL != ueCtx_p )
        { 
            ueCtx_p->ulSpsReqType = LTE_FALSE;
            pdcpEntitySetsSpsLc( tagPrams.ueIndex, ueCtx_p, 
                                 PDCP_ENTITY_RB_RX );
        }
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,tagId,0,0, 0,0,__func__,
                 "PDCP_SPS_UL_INFO" );
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckSpsDlSetupInfo
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH
 * Description    : It checks PDCP_SPS_DL_SETUP_INFO RRC tag and converts 
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckSpsDlSetupInfo( UInt8* data_p, UInt16 size )
{
    /* check length */
    if( sizeof(RrcTagSpsDlSetupInfo) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, 
                 sizeof(RrcTagSpsDlSetupInfo),sizeof(data_p),0, 0,0, __func__, 
                 "PDCP_SPS_DL_SETUP_INFO");
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;            
    }

    return PDCP_SUCCESS; 
}

/****************************************************************************
 * Function Name  : rrcTagSpsDlSetupInfo
 * Inputs         : tagId -
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : UInt16
 * Description    : It processes PDCP_SPS_DL_SETUP_INFO RRC tag.
 ****************************************************************************/
UInt16 rrcTagSpsDlSetupInfo( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                  result              = PDCP_SUCCESS;
    LP_RrcTagSpsDlSetupInfo tagSpsDlSetupInfo_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagSpsDlSetupInfo    tagSpsDlSetupInfo_data = {0};
    /* ALIGNMENT changes end */
    RrcTagParams            tagPrams            = {0};
    LP_PdcpUeContext        ueCtx_p             = PNULL;
    /* + Layer2 NON CA Changes */
    InternalCellIndex cellIndex = INVALID_CELL_INDEX;
    /* - Layer2 NON CA Changes */

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    /* Prepare pointer to tag data */
    tagSpsDlSetupInfo_p = &tagSpsDlSetupInfo_data;
    READ_API_MSG(RrcTagSpsDlSetupInfo, tagSpsDlSetupInfo_p, data_p);
    tagPrams.ueIndex = ((LP_RrcTagParams)params)->ueIndex;

    do
    {     
        /* Check for interval received */
        if ( intProfileLast < 
                tagSpsDlSetupInfo_p->semiPersistentSchedIntervalDl )
        {
            LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__,
                    PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM, tagPrams.ueIndex,
                    0,0, 0,0, __func__,"");
            result = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            break;
        }

    /* + Layer2 NON CA Changes */
        /* Get UE context */
        ueCtx_p = PDCP_GET_UE_CONTEXT(tagPrams.ueIndex); 

        if ( PNULL == ueCtx_p)
        {
            /* Invalid UE Context */
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex,
                     pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
            result = PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            break;
        }

        cellIndex = ueCtx_p->cellIndex;

        if( LTE_FALSE == pdcpContext.sCellInfo[cellIndex].\
                    supportedSpsDlIntervalList[tagSpsDlSetupInfo_p->\
                                        semiPersistentSchedIntervalDl] )
        {
            LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__,
                    PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM, tagPrams.ueIndex,
                    0,0, 0,0, __func__,"");
            result = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            break;
        }

    /* - Layer2 NON CA Changes */
        ueCtx_p->semiPersistentSchedIntervalDL =  
                tagSpsDlSetupInfo_p->semiPersistentSchedIntervalDl;

    } while( 0 );
    
    if ( PDCP_SUCCESS != result )          
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                 "PDCP_SPS_DL_SETUP_INFO" );
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckSpsUlSetupInfo
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH
 * Description    : It checks PDCP_SPS_UL_SETUP_INFO RRC tag and converts 
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckSpsUlSetupInfo( UInt8* data_p, UInt16 size )
{   
    /* check length */
    if( sizeof(RrcTagSpsUlSetupInfo) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, 
                 sizeof(RrcTagSpsUlSetupInfo),sizeof(data_p),0, 0,0, __func__, 
                 "PDCP_SPS_UL_SETUP_INFO");
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;            
    }

    return PDCP_SUCCESS; 
}

/****************************************************************************
 * Function Name  : rrcTagSpsUlSetupInfo
 * Inputs         : tagId - 
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcTagParams structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : UInt16
 * Description    : It processes PDCP_SPS_UL_SETUP_INFO RRC tag.
 ****************************************************************************/
UInt16 rrcTagSpsUlSetupInfo( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{ 
    UInt16                  result              = PDCP_SUCCESS;
    LP_RrcTagSpsUlSetupInfo tagSpsUlSetupInfo_p = PNULL;
    /* ALIGNMENT changes start */
    RrcTagSpsUlSetupInfo    tagSpsUlSetupInfo_data = {0};
    /* ALIGNMENT changes end */
    RrcTagParams            tagPrams            = {0};
    LP_PdcpUeContext        ueCtx_p             = PNULL;

    /* + Layer2 NON CA Changes */
    InternalCellIndex  cellIndex  =  INVALID_CELL_INDEX;
    /* - Layer2 NON CA Changes */

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    /* Prepare pointer to tag data */
    tagSpsUlSetupInfo_p = &tagSpsUlSetupInfo_data; 
    READ_API_MSG(RrcTagSpsUlSetupInfo, tagSpsUlSetupInfo_p, data_p);
    tagPrams.ueIndex = ((LP_RrcTagParams)params)->ueIndex;

    do
    {     
        /* Check for interval received */
        if ( intProfileLast < 
                tagSpsUlSetupInfo_p->semiPersistentSchedIntervalUl )
        {
            LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__,
                    PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM, tagPrams.ueIndex,
                    0,0, 0,0, __func__,"");
            result = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            break;
        }

        /* + Layer2 NON CA Changes */
        /* Get UE context */
        ueCtx_p = PDCP_GET_UE_CONTEXT(tagPrams.ueIndex); 
        if ( PNULL == ueCtx_p)
        {
            /* Invalid UE Context */
            LOG_PDCP_MSG (PDCP_INVALID_UE_CONTEXT, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex,
                     pdcpContext.isInitialized, 0,0, 0,0, __func__,"");
            result = PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            break;
        }

        cellIndex = ueCtx_p->cellIndex;
        
        if( LTE_FALSE == pdcpContext.sCellInfo[cellIndex].\
                    supportedSpsUlIntervalList[tagSpsUlSetupInfo_p->\
                                        semiPersistentSchedIntervalUl] )
        {
            LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM,
                    pdcpCurrentTime_g, __LINE__,
                    PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM, tagPrams.ueIndex,
                    0,0, 0,0, __func__,"");
            result = PDCP_ERR_TLV_PARSING_INVALID_SPS_PARAM;
            break;
        }
        /* - Layer2 NON CA Changes */
        ueCtx_p->semiPersistentSchedIntervalUL = 
                     tagSpsUlSetupInfo_p->semiPersistentSchedIntervalUl;

    } while( 0 );
    
    if ( PDCP_SUCCESS != result )          
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,
                 size,tagId,responseMessage->header.transactionId,0,0, __func__,
                 "PDCP_SPS_UL_SETUP_INFO" );
    }
    return result;
}
#endif

/****************************************************************************
 * Function Name  : rrcTagCheckDrbCountMsbList 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : UInt16
 * Description    : It checks PDCP_DRB_COUNT_MSB_LIST RRC tag and converts
 *                  mandatory tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckDrbCountMsbList( UInt8* data_p, UInt16 size )
{
    UInt16                        result      = PDCP_SUCCESS;
    /* check length */
    if ( sizeof(RrcTagDrbCountMsbList) > size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, 
                 sizeof(RrcTagDrbCountMsbList),sizeof(data_p),0, 0,0, __func__,
                 "PDCP_DRB_COUNT_MSB_LIST");
        result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
   return result;
}

/****************************************************************************
 * Function Name  : rrcMakeTagResponseDrbCount 
 * Inputs         : tagId - Tag of the message
 *                  subTag1 - Sub Tag PDCP_DRB_COUNT_MSB_UPLINK 
 *                  count1 - COUNT value for subTag1 
 *                  subTag2 - Sub Tag PDCP_DRB_COUNT_MSB_DOWNLINK
 *                  count2 - COUNT value for subTag2 
 *                  lcId - Logical Channel ID
 *                  responseMessage - response to be sent to RRC      
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_DRB_COUNT_MSB_INFO RRC tag and 
 *                  converts mandatory tag fields to Network byte order.
 ****************************************************************************/
void rrcMakeTagResponseDrbCount(UInt16 tagId, UInt16 subTag1, UInt32 count1,
        UInt16 subTag2, UInt32 count2, UInt8 lcId, LP_TlvMessage responseMessage)
{
    LP_TlvTagHeader       tagHeader_p = PNULL;
    LP_TlvTagHeader       subTagHeader1_p = PNULL;
    LP_TlvTagHeader       subTagHeader2_p = PNULL;
    LP_RrcTagDrbCountMsbInfo tagResponseDrbInfo_p = PNULL;
    LP_RrcTagDrbCountMsbUpLink tagResponseDrbUp_p = PNULL;
    LP_RrcTagDrbCountMsbDownLink tagResponseDrbDown_p = PNULL;

    /* add tag header */
    tagHeader_p = (LP_TlvTagHeader)&responseMessage->
                          data[ TLV_GET_WRITE_POS(responseMessage) ];
    tagHeader_p->tagId = tagId;
    tagHeader_p->length = sizeof(TlvTagHeader) + sizeof(RrcTagDrbCountMsbInfo);

    /* update massage length */
    responseMessage->header.length += sizeof(TlvTagHeader);

    /* add tag data */
    tagResponseDrbInfo_p = (LP_RrcTagDrbCountMsbInfo)&responseMessage->
                                 data[ TLV_GET_WRITE_POS(responseMessage) ];
    tagResponseDrbInfo_p->lcId = lcId;

    /* update message length */
    responseMessage->header.length += sizeof(RrcTagDrbCountMsbInfo);
    
    if (PDCP_DRB_COUNT_MSB_UPLINK == subTag1)
    {
        /* add sub tag header for UP Link */
        subTagHeader1_p = (LP_TlvTagHeader)&responseMessage->
                                 data[ TLV_GET_WRITE_POS(responseMessage) ];
	subTagHeader1_p->tagId = subTag1;
	subTagHeader1_p->length = sizeof(TlvTagHeader) + sizeof(RrcTagDrbCountMsbUpLink);

        /* update massage length */
        tagHeader_p->length += sizeof(TlvTagHeader) + sizeof(RrcTagDrbCountMsbUpLink);
        responseMessage->header.length += sizeof(TlvTagHeader);

	/* add tag data */
        tagResponseDrbUp_p = (LP_RrcTagDrbCountMsbUpLink)&responseMessage->
                               data[ TLV_GET_WRITE_POS(responseMessage) ];
        tagResponseDrbUp_p->countMsb = count1;
	CONVERT_INT32_HOST_TO_NETWORK(tagResponseDrbUp_p->countMsb);

        /* update message length */
        responseMessage->header.length += sizeof(RrcTagDrbCountMsbUpLink);
	CONVERT_INT16_HOST_TO_NETWORK(subTagHeader1_p->tagId);
	CONVERT_INT16_HOST_TO_NETWORK(subTagHeader1_p->length);
    }

    if (PDCP_DRB_COUNT_MSB_DOWNLINK == subTag2)
    {
        /* add sub tag header for UP Link */
        subTagHeader2_p = (LP_TlvTagHeader)&responseMessage->
                                 data[ TLV_GET_WRITE_POS(responseMessage) ];
	subTagHeader2_p->tagId = subTag2;
	subTagHeader2_p->length = sizeof(TlvTagHeader) + sizeof(RrcTagDrbCountMsbDownLink);

        /* update massage length */
        tagHeader_p->length += sizeof(TlvTagHeader) + sizeof(RrcTagDrbCountMsbDownLink);
        responseMessage->header.length += sizeof(TlvTagHeader);

	/* add tag data */
        tagResponseDrbDown_p = (LP_RrcTagDrbCountMsbDownLink)&responseMessage->
                               data[ TLV_GET_WRITE_POS(responseMessage) ];
        tagResponseDrbDown_p->countMsb = count2;
	CONVERT_INT32_HOST_TO_NETWORK(tagResponseDrbDown_p->countMsb);

        /* update message length */
        responseMessage->header.length += sizeof(RrcTagDrbCountMsbDownLink);
	CONVERT_INT16_HOST_TO_NETWORK(subTagHeader2_p->tagId);
	CONVERT_INT16_HOST_TO_NETWORK(subTagHeader2_p->length);
    }

    /*Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->tagId);
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->length);
}

/****************************************************************************
 * Function Name  : rrcTagDrbCountMsbList 
 * Inputs         : tagId -
 *                  params - pointer to tag processing parameters.
 *                           It should be pointer to RrcDrbCountMsbReq structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 *                  responseMessag - pointer to response message buffer
 * Outputs        : None.
 * Returns        : UInt16
 * Description    : It processes PDCP_DRB_COUNT_MSB_LIST RRC tag.
 ****************************************************************************/
UInt16 rrcTagDrbCountMsbList( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    /* ALIGNMENT changes start */
    RrcTagDrbCountMsbList         tagDrbCountMsb_data = {0};
    /* ALIGNMENT changes end */
    UInt16                        result    = PDCP_SUCCESS;
    LP_RrcTagDrbCountMsbList      tagDrbCountMsb_p = PNULL;
    LP_RrcDrbCountMsbReq          tagPrams_p = PNULL;
    UInt32 countMsb1 = 0, countMsb2 = 0;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }
    
    do 
    {
        tagPrams_p = (LP_RrcDrbCountMsbReq)params;

        /* Prepare pointer to tag data */
        /* ALIGNMENT changes start */
        tagDrbCountMsb_p = &tagDrbCountMsb_data;
        READ_API_MSG(RrcTagDrbCountMsbList, tagDrbCountMsb_p, data_p);
        /* ALIGNMENT changes end */

        /* check all tag fields */
        if ( (LTE_DRB_LC_ID_LOW_BOUND > tagDrbCountMsb_p->lcId) ||
                (LTE_DRB_LC_ID_HIGH_BOUND < tagDrbCountMsb_p->lcId) )
        {
            result = PDCP_ERR_TLV_PARSING_INVALID_LC_ID;
            LOG_PDCP_MSG (PDCP_INVALID_LC, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__,tagDrbCountMsb_p->lcId, tagPrams_p->ueIndex, 0,0, 0,0,__func__,"" );
            break;
        }
        if ( PDCP_ENTITY_RB_BOTH < tagDrbCountMsb_p->rbDirection)
        {
            result = PDCP_ERR_ENTITY_WRONG_DIRECTION;
            LOG_PDCP_MSG( PDCP_INVALID_DIR, LOGWARNING, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, tagDrbCountMsb_p->rbDirection,
                    tagPrams_p->ueIndex, 0,0, 0,0,__func__,"" ); 
            break;
        }

	if ( PDCP_ENTITY_RB_BOTH == tagDrbCountMsb_p->rbDirection )
	{
	    /* direction BOTH - First get COUNT for TX */
	    result = pdcpGetDrbCountMsb(tagPrams_p->ueIndex, tagDrbCountMsb_p->lcId, 
                         PDCP_ENTITY_RB_RX, &countMsb1);
            if (PDCP_SUCCESS == result) 
            {
	        /* Get COUNT for RX */
	        result = pdcpGetDrbCountMsb(tagPrams_p->ueIndex, tagDrbCountMsb_p->lcId, 
                             PDCP_ENTITY_RB_TX, &countMsb2);
                if (PDCP_SUCCESS == result) 
                {
		    rrcMakeTagResponseDrbCount(PDCP_DRB_COUNT_MSB_INFO, PDCP_DRB_COUNT_MSB_UPLINK, countMsb1,
			PDCP_DRB_COUNT_MSB_DOWNLINK, countMsb2, tagDrbCountMsb_p->lcId, responseMessage);
		}
            }
	}
	else
	{
	    /* direction is one */
	    result = pdcpGetDrbCountMsb(tagPrams_p->ueIndex, tagDrbCountMsb_p->lcId, 
                         tagDrbCountMsb_p->rbDirection, &countMsb1);
            if (PDCP_SUCCESS == result) 
            {
                /* For single direction update COUNT in response according to the rbDirection*/
                if (PDCP_ENTITY_RB_RX == tagDrbCountMsb_p->rbDirection) 
                {
		    rrcMakeTagResponseDrbCount(PDCP_DRB_COUNT_MSB_INFO, PDCP_DRB_COUNT_MSB_UPLINK, countMsb1,
			PDCP_CMD_TAG_LAST, countMsb2, tagDrbCountMsb_p->lcId, responseMessage);
		}
                else 
                {
		    rrcMakeTagResponseDrbCount(PDCP_DRB_COUNT_MSB_INFO, PDCP_CMD_TAG_LAST, countMsb2,
			PDCP_DRB_COUNT_MSB_DOWNLINK, countMsb1, tagDrbCountMsb_p->lcId, responseMessage);
		}
            }
	}
    }while (0);

    if ( PDCP_SUCCESS != result )
    { 
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_OPTIONAL_TAG_FAILS, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, result,size,tagId,0, 0,0,__func__,
                 "PDCP_DRB_COUNT_MSB_LIST" );
        /* No need to add error tag, just return response with result 
        rrcMakeTagError( PDCP_DRB_COUNT_MSB_ERROR, tagDrbCountMsb_p->lcId, result,
                responseMessage ); */
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcCmdDrbCountMsbReq 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_DRB_COUNT_MSB_REQ API.
 *                  It sends PDCP_DRB_COUNT_MSB_RESP message to RRC.
 ****************************************************************************/
void rrcCmdDrbCountMsbReq(UInt8* data_p, UInt16 size )
{
    /* Coverity 55830 */
    UInt8  *responseBuf = PNULL;
    responseBuf = (UInt8 *)getMemFromPool(PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if (responseBuf == PNULL)
    {
	    LOG_PDCP_MSG(L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,
		 pdcpCurrentTime_g, __LINE__, 
		(sizeof(TlvHeader) + sizeof(RrcSrbDataInd) + PDCP_TS_SDU_MAX_SIZE),
                 0,0,0,0.0,0.0,__FILE__,__func__);
	    return;
    }

    /* Coverity 55830 */
    LP_TlvMessage               message_p = (LP_TlvMessage)data_p;
    LP_TlvMessage   responseMsg_p = (LP_TlvMessage)&responseBuf[0];
    LP_RrcDrbCountMsbReq	rrcDrbCountMsbReq_p = PNULL;
    LP_RrcDrbCountMsbResp	rrcDrbCountMsbResp_p = PNULL;
    /*Check API id */
    CHECK_EXP( PDCP_DRB_COUNT_MSB_REQ != message_p->header.apiId );

    /* + Layer2 NON CA Changes */
    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
         message_p->header.transactionId,
         RRC_MODULE_ID,
         PDCP_DRB_COUNT_MSB_RESP,
         sizeof(RrcDrbCountMsbResp)
#ifndef UE_SIM_TESTING
         ,message_p->header.cellIndex
#endif 
         );
    /* - Layer2 NON CA Changes */

    /* Prepare pointer to response message data */
    rrcDrbCountMsbResp_p = (LP_RrcDrbCountMsbResp)responseMsg_p->data;

    /* Set default values in response message */
    rrcDrbCountMsbResp_p->ueIndex = 0;
    rrcDrbCountMsbResp_p->responseCode = PDCP_SUCCESS;
    do
    {
        /* SES-526 Fix Start */
        /*Prepare pointer to message data */
        rrcDrbCountMsbReq_p = (LP_RrcDrbCountMsbReq)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( rrcDrbCountMsbReq_p->ueIndex );
        rrcDrbCountMsbResp_p->ueIndex = rrcDrbCountMsbReq_p->ueIndex;
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, message_p->header.length,
                     size, 0,0, 0,0, __func__,"PDCP_DRB_COUNT_MSB_REQ");
            rrcDrbCountMsbResp_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /* Check data size */
        if ( message_p->header.length < PDCP_RRC_PDCP_DRB_COUNT_MSB_REQ_MIN_SIZE)
        {
            LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM,
                     pdcpCurrentTime_g, __LINE__, message_p->header.length,
                     PDCP_RRC_PDCP_DRB_COUNT_MSB_REQ_MIN_SIZE, 0,0,
                     0,0, __func__,"PDCP_DRB_COUNT_MSB_REQ");
            rrcDrbCountMsbResp_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }
        
        /* parse and check message optional parameters */
        rrcDrbCountMsbResp_p->responseCode = tlvCheckTags(
                rrcDrbCountMsbListTagsTable,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcDrbCountMsbReq)], 
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                                 sizeof(RrcDrbCountMsbReq) ) );
        if ( PDCP_SUCCESS != rrcDrbCountMsbResp_p->responseCode )
        {
            /* message parsing error */
            break;
        }

        /***** Process optional parameters */
        rrcDrbCountMsbResp_p->responseCode = tlvProcessTags(
                rrcDrbCountMsbListTagsTable,
                rrcDrbCountMsbReq_p,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcDrbCountMsbReq)], 
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                                 sizeof(RrcDrbCountMsbReq) ), 
                responseMsg_p);

    } while(0);

    if (PDCP_SUCCESS != rrcDrbCountMsbResp_p->responseCode)
    {
        LOG_PDCP_MSG (PDCP_RRC_MESSAGE_FAIL, LOGWARNING, PDCP_RRCOAM, pdcpCurrentTime_g,
                 __LINE__,rrcDrbCountMsbResp_p->responseCode,
                 rrcDrbCountMsbResp_p->ueIndex,0,0, 0,0,__func__,
                 "PDCP_DRB_COUNT_MSB_REQ" );
        rrcDrbCountMsbResp_p->responseCode = PDCP_FAIL;
    }
    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( rrcDrbCountMsbResp_p->ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( rrcDrbCountMsbResp_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( responseBuf, PDCP_NTOHS(responseMsg_p->header.length)
            );
    /* Coverity 55830 */
    freeMemPool(responseBuf);
    /* Coverity 55830 */
}

/****************************************************************************
 * Function Name  : rrcTagCheckLossyHoRequired
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size       
 * Outputs        : None.
 * Returns        : PDCP_SUCCESS/PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH
 * Description    : It checks PDCP_MESSAGE RRC tag and converts mandatory
 *                  tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagCheckLossyHoRequired (UInt8* data_p, UInt16 size )
{
    /* check length */
    if ( sizeof(RrcTagLossyHoRequired) > size )
    {
       /* TLV parsing error stop processing */
        /* +- SPR 17777 */
       LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                pdcpCurrentTime_g, __LINE__, size, sizeof(RrcTagLossyHoRequired),
                sizeof(data_p),0, 0,0, __func__, "PDCP_MESSAGE");
       return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagLossyHoConfigure
 * Inputs         :  tagId -
 *                   params - pointer to tag processing parameters.
 *                     It should be pointer to RrcTagSnHfnStatus structure.
 *                   data_p - pointer to tag data in TLV API message buffer
 *                   size - tag size
 *                   responseMessag - pointer to response message buffer
 *                         
 * Outputs        : None.
 * Returns        : UInt16
 * Description    : It checks PDCP_MESSAGE RRC tag and converts mandatory
 *                  tag fields to Host byte order.
 ****************************************************************************/
UInt16 rrcTagLossyHoConfigure (UInt16 tagId, void* params, UInt8* data_p,
              UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16   result      = PDCP_SUCCESS;
    LP_RrcTagLossyHoRequired tagLossyHoRequired_p   = PNULL;
    LP_PdcpEnityContext         pdcpEntityContext_p = PNULL;
    RrcTagParams                tagPrams = {0};

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }
    tagLossyHoRequired_p = (LP_RrcTagLossyHoRequired)data_p;

    tagPrams.ueIndex = ((LP_RrcHeader)params)->ueIndex;
    tagPrams.lcId    = tagLossyHoRequired_p->lcId;

    pdcpEntityContext_p = pdcpGetEntityContext( tagPrams.ueIndex, 
            tagLossyHoRequired_p->lcId, PDCP_ENTITY_RB_TX);
    if ( PNULL != pdcpEntityContext_p )
    {
        if ( pdcpEntityContext_p->rlcMode == PDCP_ENTITY_RLC_AM )
        {
            result = PDCP_ERR_UE_LOSSY_FOR_AM_RECVD;
        }  
        else
        {
            /* Set Lossy into the PDCP context */
            pdcpEntityContext_p->lossyHoRab = LTE_TRUE;
        }
    }
    else
    {
        /* +- SPR 17777 */
        LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_RRCOAM,
                pdcpCurrentTime_g, __LINE__, tagPrams.ueIndex, tagPrams.lcId,
                size,tagId,responseMessage->header.transactionId,0, __func__,"");
        result = PDCP_ERR_CONTEXT_NOT_INITIALIZED; 
    }
    return result;
}

/* SPR 17157 Fix start */
/****************************************************************************
 * Function Name  : rrcTagCheckMaciReq 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks RRC_PDCP_MACI_REQ_TAG tag
 ****************************************************************************/
STATIC UInt16 rrcTagCheckMaciReq( UInt8* data_p, UInt16 size )
{
    /* check length, atleast one extra byte should be for encoded string */
    if ( sizeof(RrcTagMaciReq) >= size )
    {
        /* TLV parsing error stop processing */
        /* +- SPR 17777 */
        LOG_PDCP_MSG (PDCP_INVALID_TLV_MESSAGE_LENGTH, LOGWARNING, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, size, sizeof(RrcTagMaciReq),
                 sizeof(data_p),0, 0,0, __func__, "RRC_PDCP_MACI_REQ_TAG");
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }

    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagMaciReq
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks RRC_PDCP_MACI_REQ_TAG tag and converts mandatory
 *                   tag fields to Host byte order. Also prepares the MACI response Tag
 ****************************************************************************/
STATIC UInt16 rrcTagMaciReq( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    LP_RrcTagParams       tagParams_p = PNULL;
    LP_RrcTagMaciReq      tagMaciReq_p = PNULL;
    RrcTagMaciReq         tagMaciReq = {0};
    UInt8                 MACI[MACI_LEN] =  {0};
    void                  *integrityData_p = PNULL;
    LP_PdcpEnityContext   ctx_p = PNULL;
	UInt16                result = PDCP_SUCCESS;

    if ( PNULL == params )
    {
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagParams_p = (LP_RrcTagParams)params;
    tagMaciReq_p = &tagMaciReq;
    READ_API_MSG(RrcTagMaciReq, tagMaciReq_p, data_p);
/* SPR 17157 Fix start */
    tagParams_p->lcId = tagMaciReq_p->lcId;
    tagParams_p->rbDirection = tagMaciReq_p->rbDirection;
/* SPR 17157 Fix end */
    

    do {   
	   ctx_p = pdcpGetEntityContext( tagParams_p->ueIndex, 
					LTE_SRB_LC_ID_LOW_BOUND, PDCP_ENTITY_RB_TX);
	   if (PNULL == ctx_p) 
	   {
        /* +- SPR 17777 */
		   LOG_PDCP_MSG( PDCP_UNINITIALIZED_CONTEXT_TX, LOGWARNING, PDCP_TX,
					pdcpCurrentTime_g, __LINE__, tagParams_p->ueIndex,
					tagParams_p->lcId,tagId,0,0,0,__func__,"");
		   result = PDCP_ERR_ENTITY_NOT_FOUND;
		   break;
	   }

       integrityData_p = msgAlloc( pdcpContextGetMsgPool(), 0, 0, 0 );
        if ( PNULL == integrityData_p )
        {
            /* free memory */
            /* +- SPR 17777 */
            LOG_PDCP_MSG( PDCP_MSG_ALLOC_FAILED, LOGFATAL, PDCP_RRCOAM, 
                    pdcpCurrentTime_g, __LINE__, 0, tagParams_p->ueIndex,
                    tagParams_p->lcId,tagId, 0,0, __func__, "rrcTagMaciReq");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            result = PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
            break;
        }

        if ( PNULL == msgInsert( integrityData_p, 0, 
                    &data_p[sizeof(RrcTagMaciReq)], /* get pointer to data */
                    /* get data size */
                    ( size - sizeof(RrcTagMaciReq) ) ) )
        {
            /* free memory */
            msgFree( integrityData_p );
            integrityData_p = PNULL;
            LOG_PDCP_MSG( PDCP_MSG_INSERT_FAILED, LOGFATAL, PDCP_RRCOAM, pdcpCurrentTime_g,
                     __LINE__, TLV_GET_OPTIONAL_PART_SIZE( data_p, 
                     sizeof(RrcMessageMacI) ), tagParams_p->ueIndex,
                     tagParams_p->lcId, 0,0,0, __func__,
                     "Data for MAC-I TAG calculation cannot be inserted to ZCB");
            ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
            result = PDCP_ERR_SYSTEM_MEMORY_ALLOCATION;
            break;
        }
        LOG_PDCP_MSG( PDCP_DATA_TO_INTEGRITY_ENGINE, LOGDEBUG, PDCP_RRCOAM, 
                 pdcpCurrentTime_g, __LINE__, tagParams_p->ueIndex,
                 tagParams_p->lcId, MAX_COUNT, 0,0,0,__func__,"");

        /* Compute MAC-I */
        integrityGenerateMACI
                   ( ctx_p->integrity.algorithmId,
                    tagParams_p->lcId,
                    MAX_COUNT,
                    tagParams_p->rbDirection,
                    ctx_p->integrity.key,
                    integrityData_p,
                    MACI); 
        msgFree( integrityData_p );
        integrityData_p = PNULL;
         /* make MACI tag and update response message */
        rrcMakeTagResponseMaciResp(MACI, responseMessage);
    }while (0);

    return result;
}

/****************************************************************************
 * Function Name  : rrcMakeTagResponseMaciResp
 * Inputs         : macI - 32 bit value by integrity engine 
 *                  responseMessage - response array to be sent to RRC      
 * Outputs        : None.
 * Returns        : None.
 * Description    : It preapares the RRC_PDCP_MACI_RESP_TAG response for RRC and 
 *                  converts mandatory tag fields to Network byte order.
 ****************************************************************************/
STATIC void rrcMakeTagResponseMaciResp(UInt8* macI, LP_TlvMessage responseMessage)
{
    LP_TlvTagHeader       tagHeader_p = PNULL;

    /* add tag header (TAG and LENGTH) */
    tagHeader_p = (LP_TlvTagHeader)&responseMessage->
                          data[ TLV_GET_WRITE_POS(responseMessage) ];
    tagHeader_p->tagId = RRC_PDCP_MACI_RESP_TAG;
    tagHeader_p->length = sizeof(TlvTagHeader) + MACI_LEN;

    /* update massage length */
    responseMessage->header.length += sizeof(TlvTagHeader);
    /* Copy the calculated MACI for RRC */
    memCpy (&responseMessage->data[TLV_GET_WRITE_POS(responseMessage)],
               macI, MACI_LEN);
    /* update message length */
    responseMessage->header.length += MACI_LEN;

    /*Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->tagId);
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->length);
	return;
}
/* SPR 17157 Fix end */

/****************************************************************************
 * Function Name  : pdcpParseRrcTagCreateSrbEntity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagCreateSrb_p - pointer to LP_RrcTagCreateSrbEntity 
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Create Srb Create Message from RRC.
 *
 ****************************************************************************/
void pdcpParseRrcTagCreateSrbEntity (LP_RrcTagCreateSrbEntity 
     tagCreateSrb_p, UInt8* data_p)
{	
    UInt8 current = 0;
    tagCreateSrb_p->srbId       = data_p[current++];
    tagCreateSrb_p->rlcMode     = LTE_GET_U16BIT(&data_p[current]);
    current += 2;
    tagCreateSrb_p->rbDirection = LTE_GET_U16BIT(&data_p[current]);

}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagCreateDrbEntity  
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagCreateDrb_p - pointer to LP_RrcTagCreateDrbEntity 
 *                
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Create Drb Entity Message from RRC.
 *
 ****************************************************************************/
void pdcpParseRrcTagCreateDrbEntity (LP_RrcTagCreateDrbEntity  
     tagCreateDrb_p, UInt8* data_p)
{
    UInt8 current = 0;
    tagCreateDrb_p->lcId        = data_p[current++];
    tagCreateDrb_p->radioBearer = data_p[current++];
    tagCreateDrb_p->rlcMode     = LTE_GET_U16BIT(&data_p[current]);
    current += 2;
    tagCreateDrb_p->rbDirection = LTE_GET_U16BIT(&data_p[current]);
    current += 2;
#ifndef UE_SIM_TESTING
    tagCreateDrb_p->qci         = data_p[current];    
#endif
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagConfigureIntegrity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagConfigureIntegrity_p - pointer to 
 *                  LP_RrcTagConfigureIntegrity
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Integrity configurations from RRC.
 ****************************************************************************/
void pdcpParseRrcTagConfigureIntegrity (LP_RrcTagConfigureIntegrity  
     tagConfigureIntegrity_p, UInt8* data_p)
{
    UInt8 current =0;
    UInt8 Index =0;
    tagConfigureIntegrity_p->AlgorithmId = LTE_GET_U16BIT(&data_p[current]);
    current +=2;
    for(Index =0; Index < LTE_SECURITY_KEY_LENGTH; Index++)
    {
        tagConfigureIntegrity_p->Key[Index] =  data_p[current++];
    }
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagConfigureCiphering 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagConfigureCiphering_p - pointer to 
 *                  LP_RrcTagConfigureCiphering
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Ciphering configurations from RRC.
 ****************************************************************************/
void pdcpParseRrcTagConfigureCiphering (LP_RrcTagConfigureCiphering   
     tagConfigureCiphering_p, UInt8* data_p)
{
    UInt8 current =0;
    UInt8 Index =0;
    tagConfigureCiphering_p->AlgorithmId = LTE_GET_U16BIT(&data_p[current]);
    current +=2;
    for(Index =0; Index < LTE_SECURITY_KEY_LENGTH; Index++)
    {
        tagConfigureCiphering_p->Key[Index] =  data_p[current++];
    }
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagReconfigSrbEntity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagRecofigSrb_p - pointer to LP_RrcTagReconfigSrbEntity
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Reconfig Srb Entity Req from RRC
 ****************************************************************************/
void pdcpParseRrcTagReconfigSrbEntity (LP_RrcTagReconfigSrbEntity  
     tagRecofigSrb_p, UInt8* data_p)
{
    UInt8 current =0;
    tagRecofigSrb_p->lcId        = data_p[current++];
    tagRecofigSrb_p->rbDirection = LTE_GET_U16BIT(&data_p[current]);
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagReconfigDrbEntity 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagRecofigDrb_p - pointer to LP_RrcTagReconfigDrbEntity
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Reconfig Drb Entity Req from RRC
 ****************************************************************************/
void pdcpParseRrcTagReconfigDrbEntity (LP_RrcTagReconfigDrbEntity  
     tagRecofigDrb_p, UInt8* data_p)
{
    UInt8 current =0;
    tagRecofigDrb_p->lcId  =  data_p[current++];
    tagRecofigDrb_p->rbDirection  = LTE_GET_U16BIT(&data_p[current]);
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagConfigureRohc 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagConfigureRohc_p - pointer to LP_RrcTagConfigureRohc
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the ROHC configuration from RRC.
 ****************************************************************************/
void pdcpParseRrcTagConfigureRohc (LP_RrcTagConfigureRohc   
     tagConfigureRohc_p, UInt8* data_p)
{
    UInt8 current = 0;
    /* SPR 6551 Fix Start */
    tagConfigureRohc_p->profileIDsMask = LTE_GET_U16BIT(&data_p[current]);
    current += 2;
    tagConfigureRohc_p->maxCid  = LTE_GET_U16BIT(&data_p[current]);
    /* SPR 6551 Fix End */
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagConfigureDiscardTimer 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagConfigureDiscardTimer_p - pointer to 
 *                                          LP_RrcTagConfigureDiscardTimer
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Discard Timer configuration from RRC.
 ****************************************************************************/
void pdcpParseRrcTagConfigureDiscardTimer (LP_RrcTagConfigureDiscardTimer   
     tagConfigureDiscardTimer_p, UInt8* data_p)
{
    UInt8 current =0;
    tagConfigureDiscardTimer_p->discardTimer = LTE_GET_U16BIT(&data_p[current]);
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagSnHfnStatus 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagSnHfnStatus_p - pointer to LP_RrcTagSnHfnStatus 
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the SN HFN Status from RRC.
 ****************************************************************************/
void pdcpParseRrcTagSnHfnStatus (LP_RrcTagSnHfnStatus 
     tagSnHfnStatus_p, UInt8* data_p)
{
    UInt8 current = 0;
    tagSnHfnStatus_p->lcId  =    data_p[current++];
    tagSnHfnStatus_p->dlSnCnt  = LTE_GET_U16BIT(&data_p[current]);
    current += 2;
    /* SPR 6711 fix start */
    tagSnHfnStatus_p->dlHfnCnt = LTE_GET_U32BIT(&data_p[current]);
    current += 4;
    /* SPR 6711 fix end */
    tagSnHfnStatus_p->ulSnCnt  = LTE_GET_U16BIT(&data_p[current]);
    current += 2;
    /* SPR 6711 fix start */
    tagSnHfnStatus_p->ulHfnCnt = LTE_GET_U32BIT(&data_p[current]);
    /* SPR 6711 fix end */

}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagDrbCountMsbList 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagDrbCountMsb_p - pointer to LP_RrcTagDrbCountMsbList
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Drb Count from RRC.
 ****************************************************************************/
void pdcpParseRrcTagDrbCountMsbList (LP_RrcTagDrbCountMsbList 
     tagDrbCountMsb_p, UInt8* data_p)
{
    UInt8 current =0;
    tagDrbCountMsb_p->lcId         =     data_p[current++];
    tagDrbCountMsb_p->rbDirection  = LTE_GET_U16BIT(&data_p[current]);
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagHoTriggered
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagHoTriggered_p - pointer to LP_RrcTagHoTriggered
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Ho Triggered Command from RRC.
 ****************************************************************************/
void pdcpParseRrcTagHoTriggered (LP_RrcTagHoTriggered tagHoTriggered_p,
     UInt8* data_p)
{
    UInt8 current =0;
    tagHoTriggered_p->hoTriggered  = data_p[current];
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagDeleteEntity
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagDelete_p - pointer to LP_RrcTagDeleteEntity
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Delete Entity Req from RRC.
 ****************************************************************************/
void pdcpParseRrcTagDeleteEntity (LP_RrcTagDeleteEntity tagDelete_p,
     UInt8* data_p)
{
    UInt8 current =0;
    tagDelete_p->lcId  = data_p[current];
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagSecurityOffset
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagSecurityOffset_p - pointer to LP_RrcTagSecurityOffset 
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Security Offset from RRC.
 ****************************************************************************/
void pdcpParseRrcTagSecurityOffset (LP_RrcTagSecurityOffset tagSecurityOffset_p,
     UInt8* data_p)
{
    UInt8 current =0;
    tagSecurityOffset_p->securityOffset  = data_p[current];
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagConfigureSnSize
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagConfigureSnSize_p - pointer to LP_RrcTagConfigureSnSize
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the SN configuration from RRC.
 ****************************************************************************/
void pdcpParseRrcTagConfigureSnSize (LP_RrcTagConfigureSnSize tagConfigureSnSize_p,
     UInt8* data_p)
{
    UInt8 current =0;
    tagConfigureSnSize_p->snSize  = data_p[current];
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagConfigureSnSize
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 * Outputs        : tagConfigureStatusReportRequired_p
 * Returns        : None.
 * Description    : It parse the SN configuration from RRC.
 ****************************************************************************/
void pdcpParseRrcTagConfigureStatusReportRequired (LP_RrcTagConfigureStatusReportRequired
     tagConfigureStatusReportRequired_p, UInt8* data_p)
{
    UInt8 current =0;
    tagConfigureStatusReportRequired_p->statusReportRequired = data_p[current];
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagConfigureUePsr
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagConfigUePsr_p - pointer to LP_RrcTagConfigureUePsr
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the UE PSR configuration from RRC.
 ****************************************************************************/
void pdcpParseRrcTagConfigureUePsr (LP_RrcTagConfigureUePsr
     tagConfigUePsr_p, UInt8* data_p)
{
    UInt8 current =0;
    tagConfigUePsr_p->configureUePsr = data_p[current];
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagUlRecvSn
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagUlRecvSn_p - pointer to LP_RrcTagUlRecvSn
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the UL recv SN from RRC.
 ****************************************************************************/
void pdcpParseRrcTagUlRecvSn (LP_RrcTagUlRecvSn tagUlRecvSn_p, UInt8* data_p)
{
    UInt16 Index = 0;
    for(Index =0; Index < BITMAP_SIZE; Index++)
    {
        tagUlRecvSn_p->ulRecvSn[Index] = data_p[Index];
    }
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagRbEntity
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagRbEntity_p - pointer to LP_RrcTagRbEntity
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Rb Entity configuration from RRC.
 ****************************************************************************/
void pdcpParseRrcTagRbEntity (LP_RrcTagRbEntity tagRbEntity_p, UInt8* data_p)
{
    UInt8 current =0;
    tagRbEntity_p->lcId = data_p[current]; 
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagSpsDlInfo
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagSpsDlInfo_p - pointer to LP_RrcTagSpsDlInfo
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Sps DL Information from RRC.
 ****************************************************************************/
void pdcpParseRrcTagSpsDlInfo (LP_RrcTagSpsDlInfo tagSpsDlInfo_p, UInt8* data_p)
{
    UInt8 current =0;
    tagSpsDlInfo_p->requestType = data_p[current]; 
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagSpsUlInfo
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagSpsUlInfo_p - pointer to LP_RrcTagSpsUlInfo
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Sps UL  Information from RRC.
 ****************************************************************************/
void pdcpParseRrcTagSpsUlInfo (LP_RrcTagSpsUlInfo tagSpsUlInfo_p, UInt8* data_p)
{
    UInt8 current =0;
    tagSpsUlInfo_p->requestType = data_p[current]; 
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagSpsDlSetupInfo
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagSpsDlSetupInfo_p - pointer to LP_RrcTagSpsDlSetupInfo
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Sps DL setup  Info from RRC.
 ****************************************************************************/
void pdcpParseRrcTagSpsDlSetupInfo (LP_RrcTagSpsDlSetupInfo tagSpsDlSetupInfo_p, 
     UInt8* data_p)
{
    UInt8 current =0;
    tagSpsDlSetupInfo_p->semiPersistentSchedIntervalDl  = data_p[current]; 
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagSpsUlSetupInfo
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagSpsUlSetupInfo_p - pointer to LP_RrcTagSpsUlSetupInfo
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the Sps UL setup Info from RRC.
 ****************************************************************************/
void pdcpParseRrcTagSpsUlSetupInfo (LP_RrcTagSpsUlSetupInfo tagSpsUlSetupInfo_p, 
     UInt8* data_p)
{
    UInt8 current =0;
    tagSpsUlSetupInfo_p->semiPersistentSchedIntervalUl  = data_p[current]; 
}
/* SPR 17157 Fix start */
/****************************************************************************
 * Function Name  : pdcpParseRrcTagMaciReq
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagMaciReq_p - pointer to LP_RrcTagMaciReq
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the MaciReq Info from RRC.
 ****************************************************************************/
void pdcpParseRrcTagMaciReq(LP_RrcTagMaciReq tagMaciReq_p, UInt8* data_p)
{
    UInt8 current = 0;
    tagMaciReq_p->lcId = data_p[current];
    current++;
    tagMaciReq_p->rbDirection = LTE_GET_U16BIT(&data_p[current]); 
}
/* SPR 17157 Fix end */

#ifdef ENDC_ENABLED
/****************************************************************************
 * Function Name  : rrcCmdUpdateTrafficInd
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_UPDATE_TRAFFIC_IND API.
 ****************************************************************************/
void rrcCmdUpdateTrafficInd( UInt8* data_p, UInt16 size )
{
    LP_TlvMessage               message_p                = (LP_TlvMessage)data_p;
    LP_RrcUpdateTrafficInd      rrcUpdateTrafficInd_p = PNULL;
    LP_PdcpUeContext            ueCtx_p                  = PNULL;
    LP_PdcpEnityContext         ctx_p                    = PNULL;

    CHECK_EXP( PDCP_UPDATE_TRAFFIC_IND != message_p->header.apiId );

    do
    {
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_WARNING(PDCP_RRCOAM,
                    "PDCP_UPDATE_TRAFFIC_IND invalid MSG length length %u size %u \n",
                     message_p->header.length,size);
            break;
        }

        /* Check data size */
        if ( message_p->header.length < PDCP_UPDATE_TRAFFIC_IND_MIN_SIZE )
        {
            LOG_PDCP_WARNING(PDCP_RRCOAM,
                    "PDCP_UPDATE_TRAFFIC_IND invalid MSG length length %u size %u \n",
                     message_p->header.length,size);
            break;
        }

        /* Prepare pointer to message data */
        rrcUpdateTrafficInd_p = (LP_RrcUpdateTrafficInd)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST(rrcUpdateTrafficInd_p->header.ueIndex);

#ifndef UE_SIM_TESTING
        rrcUpdateTrafficInd_p->header.ueIndex 
             = getUeIdxFromRrcUeIdx(rrcUpdateTrafficInd_p->header.ueIndex);
#endif

        /* Ensure that UE context is valid */
        ueCtx_p = PDCP_GET_UE_CONTEXT(rrcUpdateTrafficInd_p->header.ueIndex);
        if (PNULL == ueCtx_p)
        {
            LOG_PDCP_WARNING(PDCP_RRCOAM,
                    "PDCP_INVALID_UE_CONTEXT ueIndex %d isInitialized %d \n",
                    rrcUpdateTrafficInd_p->header.ueIndex, pdcpContext.isInitialized);
            break;
        }

        /* Fetch RB Entity TX context */
        ctx_p = PDCP_GET_VALID_ENTITY_CONTEXT(ueCtx_p,
                                              rrcUpdateTrafficInd_p->lcId, 
                                              PDCP_ENTITY_RB_TX);
        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST(rrcUpdateTrafficInd_p->traffic_ind);

        if (rrcUpdateTrafficInd_p->traffic_ind == START_DATA_TRAFFIC)
        {
            ctx_p->isTrafficAllowed  = LTE_TRUE;
        }
        else if (rrcUpdateTrafficInd_p->traffic_ind == STOP_DATA_TRAFFIC)
        {
            ctx_p->isTrafficAllowed  = LTE_FALSE;
        }
        else
        {
            /*Need to print log for failure.*/
        }

    } while(0);
}
#endif


/* + SPR 19066 */
/****************************************************************************
 * Function Name  : pdcpParseRrcTagUeInactivityTimer
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                : tagInactTimer_p - pointer to LP_RrcTagInactivityTimer
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the InactivityTimer Info from RRC.
 ****************************************************************************/
void pdcpParseRrcTagInactivityTimer( LP_RrcTagInactivityTimer tagInactTimer_p, 
        UInt8* data_p )
{
    UInt8 current = 0;
    tagInactTimer_p->inactivityTimerVal = LTE_GET_U32BIT( &data_p[current] );
}

/****************************************************************************
 * Function Name  : pdcpParseRrcTagInactTimeReqd
 * Inputs         : tagInactTimeReqd_p - pointer to LP_RrcTagInactTimeReqd
 *                : data_p - pointer to tag data in TLV API message buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : It parse the InactivityTimeReqd Tag from RRC.
 ****************************************************************************/
void pdcpParseRrcTagInactTimeReqd( LP_RrcTagInactTimeReqd tagInactTimeReqd_p,
        UInt8 *data_p )
{
    UInt8 current = 0;
    tagInactTimeReqd_p->inactTimeValReqd = data_p[current]; 
}

/****************************************************************************
 * Function Name  : rrcCmdHoPrepInfoReq
 * Inputs         : data_p - Pointer to TLV API message buffer
 *                  size - message size
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes RRC PDCP_HO_PREP_INFO_REQ API.
 *                  It sends PDCP_HO_PREP_INFO_RESP message to RRC.
 ****************************************************************************/
void rrcCmdHoPrepInfoReq( UInt8* data_p, UInt16 size )
{
    UInt8  *responseBuf = PNULL;
    LP_RrcHoPrepInfoReq  hoPrepReq_p   = PNULL;
    LP_RrcHoPrepInfoResp hoPrepResp_p  = PNULL;
    LP_PdcpUeContext     ueCtx_p       = PNULL;
    LP_TlvMessage        responseMsg_p = PNULL;
    LP_TlvMessage        message_p     = (LP_TlvMessage)data_p;

    responseBuf = (UInt8 *)getMemFromPool( 
            PDCP_RRC_CNF_MAX_SIZE, PNULL);
    if( PNULL == responseBuf )
    {
        LOG_PDCP_FATAL(PDCP_RRCOAM,
                "PDCP_HO_PREP_INFO_RESP[%u]",PDCP_RRC_CNF_MAX_SIZE);
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemFromPool error in %s", __func__);
        return;
    }
    responseMsg_p = (LP_TlvMessage)&responseBuf[0];

    /*Check API id */
    CHECK_EXP( PDCP_HO_PREP_INFO_REQ != message_p->header.apiId );

    /* Prepare response buffer */
    tlvPrepareMessage( responseMsg_p,
            message_p->header.transactionId,
            RRC_MODULE_ID,
            PDCP_HO_PREP_INFO_RESP,
            sizeof(RrcHoPrepInfoResp)
#ifndef UE_SIM_TESTING
         ,message_p->header.cellIndex
#endif
         );
                

    /* Prepare pointer to response message data */
    hoPrepResp_p = (LP_RrcHoPrepInfoResp)responseMsg_p->data;

    /* Set default values in response message */
    hoPrepResp_p->header.ueIndex = 0;
    hoPrepResp_p->responseCode = PDCP_SUCCESS;

    do {
        /* SES-526 Fix Start */
        /* Prepare pointer to RRC header  */
        hoPrepReq_p = (LP_RrcHoPrepInfoReq)message_p->data;

        /* Convert data fields from network to Host byte order */
        CONVERT_INT16_NETWORK_TO_HOST( hoPrepReq_p->header.ueIndex );

        /* save ueIndex in response message */
        hoPrepResp_p->header.ueIndex = hoPrepReq_p->header.ueIndex;
        /* SES-526 Fix End */
        /* --- check message length */
        if ( message_p->header.length != size )
        {   /* invalid length -- reject it */
            LOG_PDCP_WARNING (PDCP_RRCOAM, 
                    "PDCP_HO_PREP_INFO_REQ[%u]",message_p->header.length);
            hoPrepResp_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

        /* Check data size */
        if ( message_p->header.length < PDCP_RRC_HO_PREP_INFO_REQ_MIN_SIZE )
        {
            LOG_PDCP_WARNING (PDCP_RRCOAM, 
                    "PDCP_HO_PREP_INFO_RESPi[%u]",message_p->header.length);
            hoPrepResp_p->responseCode = PDCP_ERR_TLV_PARSING_INVALID_LENGTH;
            break;
        }

#ifndef UE_SIM_TESTING
        hoPrepReq_p->header.ueIndex = getUeIdxFromRrcUeIdx(hoPrepReq_p->header.ueIndex);
#endif
        /* check it hear for making response with valid ueIndex */
        if ( !pdcpContextIsInitialized() )
        {   /* PDCP isn't initialized */
            LOG_PDCP_WARNING (PDCP_RRCOAM, 
                    "PDCP_ERR_CONTEXT_NOT_INITIALIZED[%u]",hoPrepResp_p->header.ueIndex );
            hoPrepResp_p->responseCode = PDCP_ERR_CONTEXT_NOT_INITIALIZED;
            break;
        }

        /* Check RRC header fields */
        ueCtx_p = PDCP_GET_UE_CONTEXT( hoPrepReq_p->header.ueIndex );
        if ( PNULL == ueCtx_p )            
        {
            LOG_PDCP_WARNING (PDCP_RRCOAM,
                    "PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED[%u]",hoPrepResp_p->header.ueIndex);
            hoPrepResp_p->responseCode = 
                PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            break;
        }

        /* parse and check message optional parameters */
        hoPrepResp_p->responseCode = tlvCheckTags(
                rrcHoPrepInfoTagsTable,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcHoPrepInfoReq)],
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                    sizeof(RrcHoPrepInfoReq) ) );
        if ( PDCP_SUCCESS != hoPrepResp_p->responseCode )
        {
            /* message parsing error */
            break;
        }

        /***** Process optional parameters */
        hoPrepResp_p->responseCode = tlvProcessTags(
                rrcHoPrepInfoTagsTable,
                &hoPrepReq_p->header,
                /* pointer to the optional parameters */
                &message_p->data[sizeof(RrcHoPrepInfoReq)],
                /* size of optional parameters */
                TLV_GET_OPTIONAL_PART_SIZE( message_p, 
                    sizeof(RrcHoPrepInfoReq) ), 
                responseMsg_p);
    } while( 0 );

    if( PDCP_SUCCESS != hoPrepResp_p->responseCode )
    {
        LOG_PDCP_WARNING( PDCP_RRCOAM,
                "PDCP_HO_PREP_INFO_RESP[%u]",hoPrepResp_p->header.ueIndex ); //CID:122018
        hoPrepResp_p->responseCode = PDCP_FAIL;
    }
    /* Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK( hoPrepResp_p->header.ueIndex );
    CONVERT_INT16_HOST_TO_NETWORK( hoPrepResp_p->responseCode );

    tlvPrepareHeaderForSending(responseMsg_p);

    /* send response */
    pdcpRrcSendMessage( (UInt8*)responseMsg_p, 
            PDCP_NTOHS(responseMsg_p->header.length)
            );

    /* Free the memory */
    freeMemPool( responseBuf );
}

/****************************************************************************
 * Function Name  : rrcTagCheckInactivityTimer 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks PDCP_UE_INACTIVITY_TIMER_CONFIG tag
 ****************************************************************************/
UInt16 rrcTagCheckInactivityTimer( UInt8* data_p, UInt16 size )
{
    /* check length, atleast one extra byte should be for encoded string */
    if ( sizeof(RrcTagInactivityTimer) > size )
    {
        /* TLV parsing error stop processing */
        LOG_PDCP_WARNING (PDCP_RRCOAM,
                "PDCP_UE_INACTIVITY_TIMER_CONFIG[%u]",sizeof(RrcTagInactivityTimer));
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagInactivityTimer
 * Inputs         :  params - pointer to tag processing parameters.
 *                   It should be pointer to RrcTagInactivityTimer structure.
 *                   data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 *                   responseMessage - pointer to response message buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks PDCP_UE_INACTIVITY_TIMER_CONFIG tag and 
 *                  converts mandatory tag fields to Host byte order. 
 *                  Also, stores the inactivity timer value.
 ****************************************************************************/
UInt16 rrcTagInactivityTimer( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage )
{
    LP_RrcTagParams          tagParams_p     = PNULL;
    LP_RrcTagInactivityTimer tagInactTimer_p = PNULL;
    RrcTagInactivityTimer    tagInactTimer   = {0};
    LP_PdcpUeContext         ueCtx_p         = PNULL;
    UInt16                   result          = PDCP_SUCCESS;

    if ( PNULL == params )
    {
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagParams_p = (LP_RrcTagParams)params;
    tagInactTimer_p = &tagInactTimer;
    READ_API_MSG( RrcTagInactivityTimer, tagInactTimer_p, data_p);

    do
    {
        /* Get UE context */
        ueCtx_p = PDCP_GET_UE_CONTEXT( tagParams_p->ueIndex );
        if( PNULL == ueCtx_p )
        {
            LOG_PDCP_WARNING (PDCP_RRCOAM,
                    "PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED[%u]",tagParams_p->ueIndex);
            result = PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            break;
        }

        /* Store the inactivity timer value in Ue Context */
        ueCtx_p->inactivityTimerVal = tagInactTimer_p->inactivityTimerVal;
    }while( 0 );

    if ( PDCP_SUCCESS != result )
    {
        LOG_PDCP_WARNING (PDCP_RRCOAM,
                "PDCP_UE_INACTIVITY_TIMER_CONFIG[%u]",result );
    }
    return result;
}

/****************************************************************************
 * Function Name  : pdcpPrepareSendReport
 * Inputs         : numInactiveUe - total number of inactive UEs
 *                  listInactiveUe_p - Contains the list of inactive UEs
 * Outputs        : None.
 * Returns        : None.
 * Description    : It constructs the message for L3 for all the inactive UEs
 *                  and send the same.
 ****************************************************************************/
void pdcpPrepareSendReport( UInt16 numInactiveUe, UInt16 *listInactiveUe_p 
#ifndef UE_SIM_TESTING
        ,InternalCellIndex  cellIndex
#endif
        )
{
    UInt8 *responseBuf = PNULL;
    LP_TlvMessage responseMsg_p = PNULL;
    LP_RrcInactiveUesInd inactiveUesInd_p = PNULL;
    UInt16 numUeReprtSent = 0;
    UInt16 idx = 0;

    responseBuf = (UInt8 *)getMemFromPool(
            PDCP_RRC_CNF_MAX_SIZE , PNULL);
    if( PNULL == responseBuf )
    {
        LOG_PDCP_FATAL( PDCP_RRCOAM,
                "PDCP_INACTIVE_UES_IND[%u]",PDCP_RRC_CNF_MAX_SIZE);
        ALARM_MSG (PDCP_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);
        ltePanic ("getMemFromPool error in %s", __func__);
        return;
    }
#ifndef UE_SIM_TESTING
    /*Get the RRC Cell index from the local InternalCellIndex*/
     RrcCellIndex cellId = PDCP_GET_RRC_CELL_INDEX(cellIndex);
#endif
    do {
        responseMsg_p = (LP_TlvMessage)&responseBuf[0];

        /* Prepare response buffer */
        tlvPrepareMessage( responseMsg_p, 0xFF,
                RRC_MODULE_ID, PDCP_INACTIVE_UES_IND,
                sizeof(RrcInactiveUesInd)
#ifndef UE_SIM_TESTING
                ,cellId
#endif
                );


        /* Prepare pointer to response message data */
        inactiveUesInd_p = (LP_RrcInactiveUesInd)responseMsg_p->data;

        for( idx = 0; 
                (idx < PDCP_MAX_UE_AUDIT_REPORT && idx < numInactiveUe);
                idx++ )
        {
            /* Prepare the TLV for the inactive UE */
            rrcMakeTagInactiveUe( RRC_PDCP_INACTIVE_UE_INFO_TAG, 
                    listInactiveUe_p[numUeReprtSent], responseMsg_p );
            numUeReprtSent++;
        }
        /* Fill the UE count for which tag is prepared */
        inactiveUesInd_p->numOfInactiveUe = idx;

        /*Convert response fields to network byte order */
        tlvPrepareHeaderForSending(responseMsg_p);

        /* send response */
        pdcpRrcSendMessage( (UInt8*)responseMsg_p, 
                PDCP_NTOHS(responseMsg_p->header.length)
                );
    } while(0);

    /* Free the memory */
    freeMemPool(responseBuf);
}

/****************************************************************************
 * Function Name  : rrcMakeTagInactiveUe
 * Inputs         : tagId - Tag of the message
 *                  ueIndex - UE identity
 *                  responseMsg_p - response array to be sent to RRC      
 * Outputs        : None.
 * Returns        : None.
 * Description    : It processes PDCP_INACTIVE_UE_INFO, RRC tag and 
 *                  converts mandatory tag fields to Network byte order.
 ****************************************************************************/
void rrcMakeTagInactiveUe( UInt16 tagId, UInt16 ueIndex, 
        LP_TlvMessage responseMsg_p )
{
    LP_TlvTagHeader       tagHeader_p = PNULL;
    LP_RrcInactiveUeInfo  tagInactUeInfo_p = PNULL;

    /* add tag header */
    tagHeader_p = (LP_TlvTagHeader)&responseMsg_p->
        data[ TLV_GET_WRITE_POS(responseMsg_p) ];
    tagHeader_p->tagId = tagId;
    tagHeader_p->length = sizeof(TlvTagHeader) + sizeof(RrcInactiveUeInfo);

    /* update massage length */
    responseMsg_p->header.length += sizeof(TlvTagHeader);

    /* add inactive UE info response tag */
    tagInactUeInfo_p = (LP_RrcInactiveUeInfo)&responseMsg_p->
        data[ TLV_GET_WRITE_POS(responseMsg_p) ];
#ifndef UE_SIM_TESTING
    tagInactUeInfo_p->ueIndex = getRrcUeIdxFromUeIdx(ueIndex);
#else
    tagInactUeInfo_p->ueIndex = ueIndex;
#endif
    /* update message length */
    responseMsg_p->header.length += sizeof(RrcInactiveUeInfo);

    /*Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->tagId);
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->length);

    CONVERT_INT16_HOST_TO_NETWORK(tagInactUeInfo_p->ueIndex);
}

/****************************************************************************
 * Function Name  : rrcTagCheckUeInactTimeReq 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks PDCP_UE_INACTIVE_TIME_REQ tag
 ****************************************************************************/
UInt16 rrcTagCheckUeInactTimeReq( UInt8* data_p, UInt16 size )
{
    /* check length, atleast one extra byte should be for encoded string */
    if ( sizeof(RrcTagInactTimeReqd) > size )
    {
        /* TLV parsing error stop processing */
        LOG_PDCP_WARNING (PDCP_RRCOAM, 
                "PDCP_UE_INACTIVE_TIME_REQ[%u]",sizeof(RrcTagInactTimeReqd));
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagUeInactTimeReq
 * Inputs         : params - pointer to tag processing parameters.
 *                  It should be pointer to RrcTagInactTimeReqd structure.
 *                  data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size
 *                  responseMessage - pointer to response message buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks PDCP_UE_INACTIVE_TIME_REQ tag and 
 *                  converts mandatory tag fields to Host byte order. 
 *                  Also, prepares the Inactive response for L3.
 ****************************************************************************/
UInt16 rrcTagUeInactTimeReq( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage )
{
    LP_RrcTagParams        tagParams_p        = PNULL;
    LP_RrcTagInactTimeReqd tagInactTimeReqd_p = PNULL;
    RrcTagInactTimeReqd    tagInactTimeReqd   = {0};
    tickType_t             inactTimeVal       = 0;
    UInt16                 result             = PDCP_SUCCESS;

    if ( PNULL == params )
    {   /* CRITICAL! */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagParams_p = (LP_RrcTagParams)params;
    /* Prepare pointer to tag data */
    tagInactTimeReqd_p = &tagInactTimeReqd;
    READ_API_MSG( RrcTagInactTimeReqd, tagInactTimeReqd_p, data_p );

    do
    {
        if ( LTE_TRUE != tagInactTimeReqd_p->inactTimeValReqd )
        {
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_PARAMETER_VALUE;
            break;
        }
        /* Get the inactive time value */
        result = pdcpGetInactiveTimeVal( tagParams_p->ueIndex, 
                &inactTimeVal );
        if( PDCP_SUCCESS != result )
        {
            break;
        }
        /* Make the response tag for L3 */
        rrcMakeTagResponseInactiveTime( RRC_PDCP_UE_INACTIVE_TIME_RESP_TAG,
                inactTimeVal, responseMessage );
    } while( 0 );

    if ( PDCP_SUCCESS != result )
    {
        LOG_PDCP_WARNING (PDCP_RRCOAM,
                "PDCP_UE_INACTIVE_TIME_REQ[%u]",result );
    }
    return result;
}

/****************************************************************************
 * Function Name  : rrcMakeTagResponseInactiveTime
 * Inputs         : inactTimeVal - inactivity time when UE not scheduled 
 *                  responseMsg_p - response array to be sent to RRC      
 * Outputs        : None.
 * Returns        : None.
 * Description    : It prepares the PDCP_UE_INACTIVE_TIME_RESP response for
 *                  RRC and converts mandatory tag fields to Network byte 
 *                  order.
 ****************************************************************************/
void rrcMakeTagResponseInactiveTime( UInt16 tagId, tickType_t inactTimeVal,
        LP_TlvMessage responseMsg_p )
{
    LP_TlvTagHeader          tagHeader_p    = PNULL;
    LP_RrcTagInactivityTimer tagInactTime_p = PNULL;

    /* add tag header */
    tagHeader_p = (LP_TlvTagHeader)&responseMsg_p->
        data[ TLV_GET_WRITE_POS(responseMsg_p) ];
    tagHeader_p->tagId = tagId;
    tagHeader_p->length = sizeof(TlvTagHeader) + sizeof(RrcTagInactivityTimer);

    /* update massage length */
    responseMsg_p->header.length += sizeof(TlvTagHeader);

    /* add inactivity Time for UE response tag */
    tagInactTime_p = (LP_RrcTagInactivityTimer)&responseMsg_p->
        data[ TLV_GET_WRITE_POS(responseMsg_p) ];
    tagInactTime_p->inactivityTimerVal = inactTimeVal;

    /* update message length */
    responseMsg_p->header.length += sizeof(RrcTagInactivityTimer);

    /*Convert response fields to network byte order */
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->tagId);
    CONVERT_INT16_HOST_TO_NETWORK(tagHeader_p->length);

    CONVERT_INT32_HOST_TO_NETWORK( tagInactTime_p->inactivityTimerVal );
}

/****************************************************************************
 * Function Name  : rrcTagCheckInactiveTimeLapse 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks PDCP_UE_INACTIVE_TIME_LAPSED tag
 ****************************************************************************/
UInt16 rrcTagCheckInactiveTimeLapse( UInt8* data_p, UInt16 size )
{
    /* check length, atleast one extra byte should be for encoded string */
    if ( sizeof(RrcTagInactivityTimer) > size )
    {
        /* TLV parsing error stop processing */
        LOG_PDCP_WARNING (PDCP_RRCOAM, 
                "PDCP_UE_INACTIVITY_TIMER_CONFIG[%u]",sizeof(RrcTagInactivityTimer));
        return PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
    }
    return PDCP_SUCCESS;
}

/****************************************************************************
 * Function Name  : rrcTagInactTimeLapse
 * Inputs         :  params - pointer to tag processing parameters.
 *                   It should be pointer to RrcTagInactivityTimer structure.
 *                   data_p - pointer to tag data in TLV API message buffer
 *                   size - tag size
 *                   responseMessage - pointer to response message buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks PDCP_UE_INACTIVE_TIME_LAPSED tag and 
 *                  converts mandatory tag fields to Host byte order. 
 *                  Also, stores the time already lapsed. This tag is only 
 *                  applicable at target.
 ****************************************************************************/
UInt16 rrcTagInactTimeLapse( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage )
{
    LP_RrcTagParams          tagParams_p     = PNULL;
    LP_RrcTagInactivityTimer tagInactTimer_p = PNULL;
    RrcTagInactivityTimer    tagInactTimer   = {0};
    LP_PdcpUeContext         ueCtx_p         = PNULL;
    UInt16                   result          = PDCP_SUCCESS;

    if ( PNULL == params )
    {   /* !Critical */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagParams_p = (LP_RrcTagParams)params;
    tagInactTimer_p = &tagInactTimer;
    READ_API_MSG( RrcTagInactivityTimer, tagInactTimer_p, data_p);

    do
    {
        if( MAX_UE_INACTIVITY_TIME <= tagInactTimer_p->inactivityTimerVal )
        {
            result =  PDCP_ERR_TLV_PARSING_INVALID_INACT_TIME_VAL;
            break;
        }

        /* Get UE context */
        ueCtx_p = PDCP_GET_UE_CONTEXT( tagParams_p->ueIndex );
        if( PNULL == ueCtx_p )
        {
            LOG_PDCP_WARNING (PDCP_RRCOAM,
                    "UE context is null [%u]",tagParams_p->ueIndex);
            result = PDCP_ERR_UE_CONTEXT_NOT_INITIALIZED;
            break;
        }
        /* Convert the time received into ms and store into UE context */
        ueCtx_p->lapsedInactiveTime = pdcpConvertTimetoMs( 
                tagInactTimer_p->inactivityTimerVal ); 
    }while(0);

    return result;
}

/****************************************************************************
 * Function Name  : rrcTagCheckHoMaciReq 
 * Inputs         : data_p - pointer to tag data in TLV API message buffer
 *                  size - tag size                           
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks RRC_PDCP_MACI_HO_REQ_TAG tag
 ****************************************************************************/
UInt16 rrcTagCheckHoMaciReq( UInt8* data_p, UInt16 size )
{
    UInt16 result = PDCP_SUCCESS;

    do {
        /* check length, atleast one extra byte should be for encoded string */
        if ( sizeof(RrcTagMaciReq) > size )
        {
            /* TLV parsing error stop processing */
            LOG_PDCP_WARNING (PDCP_RRCOAM, 
                    "PDCP_MACI_HO_REQ_TAG[%u]",sizeof(RrcTagInactivityTimer));
            result = PDCP_ERR_TLV_PARSING_INVALID_TAG_LENGTH;
            break;
        }

        /* parse and check tag optional parameters */
        result = tlvCheckTags( 
                rrcComputeMacITagsTable,
                /* pointer to the optional parameters */
                &data_p[sizeof(RrcTagMaciReq)], 
                /* size of optional parameters */
                (size - sizeof(RrcTagMaciReq)) 
                );
        if ( PDCP_SUCCESS != result )
        {
            /* message parsing error */
            break;
        }    
    } while(0);
    return result;
}

/****************************************************************************
 * Function Name  : rrcTagHoMaciReq 
 * Inputs         :  params - pointer to tag processing parameters.
 *                   It should be pointer to RrcTagInactivityTimer structure.
 *                   data_p - pointer to tag data in TLV API message buffer
 *                   size - tag size
 *                   responseMessage - pointer to response message buffer
 * Outputs        : None.
 * Returns        : None.
 * Description    : It checks RRC_PDCP_MACI_HO_REQ_TAG tag and composes MAC-I
 *                  response for RRC.
 ****************************************************************************/
UInt16 rrcTagHoMaciReq( UInt16 tagId, void* params, UInt8* data_p,
        UInt16 size, LP_TlvMessage responseMessage)
{
    UInt16                result       = PDCP_SUCCESS;
    RrcMacIReq            maciReq      = {{0}};
    LP_RrcTagParams       tagParams_p  = PNULL;
    LP_RrcTagMaciReq      tagMaciReq_p = PNULL;
    RrcTagMaciReq         tagMaciReq   = {0};

    if ( PNULL == params )
    { /* !CRITICAL */
        ltePanic("Invalid function parameter params == PNULL");
        return PDCP_ERR_SYSTEM_INVALID_FUNCTION_PARAMETER;
    }

    tagParams_p = (LP_RrcTagParams)params;
    tagMaciReq_p = &tagMaciReq;
    READ_API_MSG(RrcTagMaciReq, tagMaciReq_p, data_p);
    
    /* Fill the LP_RrcMacIReq structure for consistancy */
    maciReq.header.ueIndex = tagParams_p->ueIndex; 
    maciReq.lcId = tagMaciReq_p->lcId; 
    maciReq.rbDirection = tagMaciReq_p->rbDirection;

    do {
        /***** Process optional parameters */
        result = tlvProcessTags(
                rrcComputeMacITagsTable,
                &maciReq.header,
                /* pointer to the optional parameters */
                &data_p[sizeof(RrcTagMaciReq)], 
                /* size of optional parameters */
                ( size - (sizeof(RrcTagMaciReq) )), 
                responseMessage );
        if( PDCP_SUCCESS != result )
        {
            break;
        }
    }while (0);

    if ( PDCP_SUCCESS != result )
    {
        LOG_PDCP_WARNING (PDCP_RRCOAM, 
                 "PDCP_MACI_HO_REQ Fails![%u]",result );
    }

    return result;
}
/* - SPR 19066 */
