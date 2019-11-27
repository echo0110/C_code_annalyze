/***************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2InitMac.c,v 1.3.6.1.4.2.2.1 2010/09/21 15:53:32 gur20491 Exp $
 *
 ****************************************************************************
 *
 * File Description : This file provide APIs to initialize and cleanup MAC  
 *                    Layer.
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteLayer2InitMac.c,v $
 * Revision 1.3.6.1.4.2.2.1  2010/09/21 15:53:32  gur20491
 * FAPI changes
 *
 * Revision 1.3.6.1  2010/08/02 08:57:18  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.1.1.1  2010/06/01 07:49:19  cm_netlogic
 * code import
 *
 * Revision 1.3  2010/03/02 12:19:34  gur15697
 * thread priority changes
 *
 * Revision 1.2  2010/02/16 07:34:41  gur15697
 * ul Dataprocess thread removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:22  cm_intel
 * eNB framework for intel
 *
 * Revision 1.22  2009/11/14 11:11:43  gur11912
 * removed warnings generated on O2
 *
 * Revision 1.21  2009/11/12 13:33:17  gur19413
 * added UT_TESTING Flag
 *
 * Revision 1.20  2009/10/26 10:17:32  gur19413
 * modified socket functionality
 *
 * Revision 1.19  2009/09/03 14:25:33  gur20439
 * ltePanic put in macro LTE_MAC_UT_LOG.
 *
 * Revision 1.18  2009/08/21 07:17:00  gur20439
 * SIT bug fix.
 *
 * Revision 1.17  2009/07/31 06:32:51  gur15697
 * cleanup Bugfix
 *
 * Revision 1.16  2009/07/29 15:18:28  gur20439
 * cleanup and look and feel to simulator fix.
 *
 * Revision 1.15  2009/07/29 10:27:38  gur15697
 * cleanup bug fix
 *
 * Revision 1.14  2009/07/28 15:31:16  gur15697
 * segfault during cleanup fix
 *
 * Revision 1.13  2009/07/28 15:05:54  gur20439
 * cleanup bug fix.
 *
 * Revision 1.12  2009/07/27 08:50:35  gur20439
 * periodic cqi enhancement.
 *
 * Revision 1.11  2009/07/24 15:15:00  gur20439
 * compilation error resolved.
 *
 * Revision 1.10  2009/07/24 15:13:32  gur20439
 * cleanup functionality added.
 *
 * Revision 1.9  2009/07/24 14:40:27  gur20439
 * support added for non adaptive retransmission for UPLINK.
 *
 * Revision 1.8  2009/07/14 14:48:54  gur20439
 * rrc interface code change.
 *
 * Revision 1.7  2009/06/16 14:43:59  gur20439
 * modified to get new rx port for MAC Emulator.
 *
 * Revision 1.6  2009/06/15 13:53:43  gur20439
 * modifed for release 1.2.
 *
 * Revision 1.5  2009/06/08 13:57:13  gur20439
 * bug fixing in unit testing.
 *
 * Revision 1.4  2009/06/01 10:53:31  gur20439
 * include/interfaces/phySimulatorApiHandler.h
 *
 * Revision 1.3  2009/05/26 07:30:05  gur20439
 * modified for mac release 1.2
 *
 * Revision 1.2  2009/03/30 10:32:05  gur19836
 * LOG_DETAIL structure initialized
 *
 * Revision 1.1  2009/03/30 10:20:41  gur19836
 * Initial Version
 *
 * Revision 1.17  2009/03/23 09:20:39  gur19140
 * Integration changes first waves
 *
 * Revision 1.16  2009/03/13 08:00:08  gur19836
 * Modified for OAM and RLC Interface Changes
 *
 * Revision 1.15  2009/03/02 11:01:22  gur18569
 * removed funcNamp_p and replaced it with __func__
 *
 * Revision 1.14  2009/02/20 14:25:02  gur18569
 * error handling in case of reinit and re cleanup
 *
 * Revision 1.13  2009/02/17 06:49:00  gur18569
 * statsManager thread created when first invoked
 *
 * Revision 1.12  2009/02/06 06:12:53  gur18569
 * added sem for ulTxNode_p
 *
 * Revision 1.11  2009/02/03 05:57:58  gur18569
 * removed unnecessary sem ackNackQueueSem_g
 *
 * Revision 1.10  2009/01/28 12:19:05  gur03939
 * Updated comment for bind failed
 *
 * Revision 1.9  2009/01/27 08:49:24  gur19836
 * Removed startup logs and added code for setting lteLogLevel from file
 *
 * Revision 1.8  2009/01/25 20:22:29  gur19413
 * comment the shared Memory(for Timer)
 *
 * Revision 1.7  2009/01/24 10:17:24  gur11912
 * Regression bugs fixed
 *
 * Revision 1.6  2009/01/23 12:55:57  gur18569
 * changes regarding leak
 *
 * Revision 1.5  2009/01/22 11:20:14  gur19836
 * Prototype for resetStatsInfo API changed
 *
 * Revision 1.4  2009/01/21 17:43:45  gur18569
 * fixed error
 *
 * Revision 1.3  2009/01/21 17:27:25  gur18569
 * initializing globals by memset
 *
 * Revision 1.2  2009/01/21 13:58:49  gur19836
 * New global variable cellConfigured_g added
 *
 * Revision 1.1  2009/01/20 07:41:02  gur19836
 * Initial Version
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/



/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "lteLayer2InitMac.h"
#include "lteLayer2RRCInterface.h"
#include "lteLayer2OAM.h"
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
#define LEN 30
#define PHYSIP "127.0.0.1"
/****************************************************************************
 * Exported Variables
 ****************************************************************************/
/* Socket FD for receiving messages */ 
SInt32 rxSockFD_g;
/* Socket FD for receiving messages */ 
SInt32 txOAMSockFD_g;
SockAddr tx_addr_g;
SockAddr txOAM_addr_g;

/* Socket FD for eNB RRC Tx  messages */ 
SInt32 txRRCSockFD_g;
/* Range configured for CRNTI, lower boundary & upper boundary */
extern FAPI_dciConfig_st dlDciConfigSPS_g[MAX_SPS_UE];//UeSim-Sps
RNTIInfo *rntiToIdxTbl_g[MAX_NUM_CELLS];
UInt32 stoppedProcessing_g[MAX_NUM_CELLS]={1,1};
/* SPR 609 changes start */
Char8 * macLogFileName_g =  "LTE_MAC_EMULATOR";
/* SPR 609 changes end */
ModuleLogDetail MACModuleLogDetail_g;



//APeriodicCQI *apcqiInfo_p = PNULL;
extern ueContextForUplink* uplinkContextInfoMap_g[MAX_UE_SUPPORTED];
extern dynamicUEContext *dynUEContextInfoMap_g[MAX_UE_SUPPORTED];
extern dynamicUEContext *dynUEContextInfoMap_g[MAX_UE_SUPPORTED];
extern DLUEContext * ueDLContextArr_g[MAX_UE_SUPPORTED];
extern ULHarqTimerEntityList ulHarqTimerEntityList_g[10];

/* +CA */ 
extern UInt8 cellConfigured_g[MAX_NUM_CELLS];
extern CellConfig cellConfigUeSim_g[MAX_NUM_CELLS];
/* -CA */ 
extern void cleanupMACUplink(void);

extern UInt8 numCells_g;
extern UInt16  startRARNTI_g;
/*LTE_REG_FIX*/
extern UInt16  endRARNTI_g[MAX_NUM_CELLS];
/*LTE_REG_FIX*/

extern UInt8 macRegFlag_g[MAX_NUM_CELLS];
/*CLPC_CHG*/
TpcInfo tpcRntiInfoMap[TPC_RNTI_COUNT];
/*CLPC_CHG*/

extern HarqIndQueueNodeInfo *harqIndQNode_gp[MAX_NUM_CELLS]; 

//UChar8 rrcIPAddress_g[40];
UChar8 enodebRRCIPAddress_g[40];
UChar8 oamIPAddress_g[40];
UChar8 macIPAddress_g[40];
/* SPR 5313 changes start */
extern void init( UInt16 rxPort, UInt16 txPort, UInt16 rxPortForHO);

void fapiInit(UInt16 *, UInt16 *, UInt16 *, UInt8
#ifdef DL_UL_SPLIT_TDD
        ,UInt16 *
#endif
        );
/* SPR 5313 changes end */


#ifdef LTE_MAC_UT_LOG
/****************************************************************************
 * Function Name  : createMACLogdetail
 * Inputs         : none
 * Outputs        :
 * Returns        : none
 * Variables      :
 * Description    :This API is used to create MAC module log detail. It will
 fill the MACModuleLogDetail_g structure used for MAC traces.
 ****************************************************************************/
void createMACEmulatorLogdetail( void )
{
    UChar8 fullName[300];
    semInit(&(MACModuleLogDetail_g.logLock),1);
    strcpy(MACModuleLogDetail_g.filename,macLogFileName_g);
    strcpy(fullName,macLogFileName_g);
    createLogFile(fullName);
    MACModuleLogDetail_g.fp = fopen(fullName,"w");
    MACModuleLogDetail_g.level = LOG_MINOR;
    setLogLevel(7); 
    if (MACModuleLogDetail_g.fp == NULL) {
        lteWarning("Unable to open file. %s\n", fullName);
    }
}
#endif

/****************************************************************************
 * Function Name  : createPools
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      : 
 * Description    : This API is invoked during MAC Layer initialization, to 
 *                  create memory pools for uplink and downlink MAC usage. 
 ****************************************************************************/

void createPools(void)
{
    /* Memory Pool for UEContext and UEContextHashNode structures,statFile also */
    createMemPool(sizeof (DLUEContext),MAX_UE_SUPPORTED); 
    createMemPool(sizeof (ueContextForUplink ),MAX_UE_SUPPORTED);
    createMemPool(sizeof (dynamicUEContext ),MAX_UE_SUPPORTED);
}

/****************************************************************************
 * Function Name  : initMACLayer
 * Inputs         : None
 * Outputs        : None
 * Returns        : MacRetType [MAC_FAILURE OR MAC_SUCCESS]
 * Variables      : 
 * Description    : This API is invoked from the application layer, using 
 *                  MAC layer as library. 
 *                  
 *                  - It is used to initialize the MAC layer which creates, 
 *                    initializes the downlink scheduler thread, dispatcher 
 *                    thread, receiver thread, timer thread and worker 
 *                    threads.(TickThread - for testing purposes).
 *                  - It also creates and initializes memory pool along with 
 *                    configuring other important parameters like downlink 
 *                    scheduling configuration parameters. 
 ****************************************************************************/
/* +CA */
MacRetType initMACLayer(UInt8 cellIndex)
{

    enableLog_g = 1;
    LTE_LOG(LOG_DETAIL, &MACModuleLogDetail_g, "[%s]entry\n",__func__);
    
    memSet(ueDLContextArr_g,0,MAX_UE_SUPPORTED* sizeof(DLUEContext*));
    //memSet(dlUECtxInfoArr_g,0,TBL_SIZE * sizeof(DLUEContext*));
    memSet(uplinkContextInfoMap_g,0,MAX_UE_SUPPORTED * sizeof(ueContextForUplink*));
    memSet(dynUEContextInfoMap_g,0,MAX_UE_SUPPORTED* sizeof(dynamicUEContext*));
    globalTTITickCount_g = 0;
    cellConfigured_g[cellIndex] = TRUE;
    UInt8 idx = 0;
    UInt8 ueCount = 0;
    for(idx = 0; idx < TPC_RNTI_COUNT; idx++)
    {
        TpcInfo tpcInfo = tpcRntiInfoMap[idx];
        for(ueCount = 0; ueCount < 16; ueCount++)
        {
            tpcInfo.isValid[ueCount] = FALSE;
        }
    }
    return MAC_SUCCESS;
    
}
/* -CA */
/*!
 * \fn getDlDciConfigSpsInfoFromMap
 * */

/* CA */ 
/****************************************************************************
* Function Name  : getDlDciConfigSpsInfoFromMap 
* Inputs         : rnti 
                   servingCellIndex
* Outputs        : 
* Returns        : pointer to rntiToIdxTbl_g
* Variables      :
* Description    : 
                   
****************************************************************************/
FAPI_dciConfig_st * getDlDciConfigSpsInfoFromMap(UInt16 rnti,UInt8 rxCellIndex)
{
        if
        (( rnti < cellConfigUeSim_g[rxCellIndex].startSpsCrnti || rnti > cellConfigUeSim_g[rxCellIndex].endSpsCrnti)||
          (rnti >= startRARNTI_g && rnti <= endRARNTI_g[rxCellIndex]))
        {
                return PNULL;
        }
        else
        {
            return(&dlDciConfigSPS_g [(rnti - cellConfigUeSim_g[rxCellIndex].startSpsCrnti)]);
        }
}

/****************************************************************************
* Function Name  : getRntiInfofromRNTIMap 
* Inputs         : rnti 
                   servingCellIndex
* Outputs        : 
* Returns        : pointer to rntiToIdxTbl_g
* Variables      :
* Description    : This function checks for RNTI range and return pointer
                   to rntiToIdxTbl_g for rnti passed
****************************************************************************/
 RNTIInfo * getRntiInfofromRNTIMap(UInt16 rnti, UInt8 servingCellIndex )
{

	UInt8 cellIndex = 0;
    UInt8 txCellIndex = 0;
	UInt32 diff = 0;
	diff = endRARNTI_g[servingCellIndex] - startRARNTI_g;
	if ( stoppedProcessing_g[servingCellIndex] == TRUE )
	{
		return PNULL;
	}
	else if (cellConfigured_g[servingCellIndex] == FALSE )
    {
       return PNULL;
    }
    /* Sps Ue */
	if (rnti >= cellConfigUeSim_g[servingCellIndex].startSpsCrnti && 
			rnti <= cellConfigUeSim_g[servingCellIndex].endSpsCrnti)
	{
		return(&rntiToIdxTbl_g[servingCellIndex][(rnti - 
					cellConfigUeSim_g[servingCellIndex].startSpsCrnti + 
					cellConfigUeSim_g[servingCellIndex].crntiRange)]);
	}

	else
	{
        GET_RNTI_TO_CELL_INDEX( rnti, txCellIndex);
		/* Check for RA-RNTI */
		if( ( rnti < cellConfigUeSim_g[txCellIndex].lowerCRNTIBound ) || 
				(rnti > cellConfigUeSim_g[txCellIndex].upperCRNTIBound ) || 
				( rnti >= startRARNTI_g && rnti <= endRARNTI_g[servingCellIndex] ) )
		{
			return PNULL;
		}
		else
		{
			for (cellIndex = 0; cellIndex < numCells_g; cellIndex++)
			{
				
				/* Check for RNTI */
				if( ( rnti >= cellConfigUeSim_g[cellIndex].lowerCRNTIBound ) && 
						(rnti <= cellConfigUeSim_g[cellIndex].upperCRNTIBound ) )
				{
					break;
				}
			}
		}
		if (cellConfigUeSim_g[cellIndex].lowerCRNTIBound == startRARNTI_g)
		{
			diff = endRARNTI_g[servingCellIndex] - startRARNTI_g;
			return(&rntiToIdxTbl_g[cellIndex][(rnti - (diff + 1) - 
						cellConfigUeSim_g[cellIndex].lowerCRNTIBound )]);
		}
		else
		{
			return(&rntiToIdxTbl_g[cellIndex][(rnti - 
						cellConfigUeSim_g[cellIndex].lowerCRNTIBound )]);
		}

	}
}
/****************************************************************************
* Function Name  : macLayerCleanup 
* Inputs         : None
* Outputs        : None
* Returns        : None
* Variables      :
* Description    : This API cleanup the MAC Layer and closes the sockets 
*                  on RRC Layer side.
****************************************************************************/

/* +CA  */
UInt8 macLayerCleanup(UInt8 cellIndex)
{
    UInt32 i = 0,rnti = 0,diff = 0;
    stoppedProcessing_g[cellIndex] = 1;
    
    LTE_LOG(LOG_DETAIL,&MACModuleLogDetail_g, "MAC Layer cleanedup");

    if (TRUE == cellConfigured_g[cellIndex])
    {
        for ( rnti = cellConfigUeSim_g[cellIndex].lowerCRNTIBound; 
                rnti <= cellConfigUeSim_g[cellIndex].upperCRNTIBound ; rnti++ )
        {
            if ((rnti >= startRARNTI_g) &&
                (rnti <= endRARNTI_g))
            {
                continue;
            }

            diff = endRARNTI_g - startRARNTI_g;
            i = rnti - (diff + 1) - cellConfigUeSim_g[cellIndex].lowerCRNTIBound;

            if ( rntiToIdxTbl_g[cellIndex][i].aperiodiccqiInfo )
            {
                freeMemPool(rntiToIdxTbl_g[cellIndex][i].aperiodiccqiInfo);
                rntiToIdxTbl_g[cellIndex][i].aperiodiccqiInfo = PNULL;
            }
            if ( rntiToIdxTbl_g[cellIndex][i].periodiccqiInfo )
            {
                freeMemPool(rntiToIdxTbl_g[cellIndex][i].periodiccqiInfo);
                rntiToIdxTbl_g[cellIndex][i].periodiccqiInfo = PNULL;
            }
        }

        freeMemPool(rntiToIdxTbl_g[cellIndex]);
        rntiToIdxTbl_g[cellIndex] = PNULL;
    }

for (i = 0; i < MAX_UE_SUPPORTED; i++)
{
    if (dlUECtxInfoArr_g[i].dlUEContext_p)
    {
      freeMemPool(dlUECtxInfoArr_g[i].dlUEContext_p);
    }
    dlUECtxInfoArr_g[i].dlUEContext_p = PNULL;
    dlUECtxInfoArr_g[i].pendingDeleteFlag = 1;
}

for (i = 0; i< MAX_UE_SUPPORTED; i++)
{
    if (uplinkContextInfoMap_g[i])
    {
        freeMemPool(uplinkContextInfoMap_g[i]);
    }
    uplinkContextInfoMap_g[i] = PNULL;
}


for (i = 0; i < MAX_UE_SUPPORTED; i++)
{
    if (dynUEContextInfoMap_g[i])
    {
        freeMemPool(dynUEContextInfoMap_g[i]);
    }
    uplinkContextInfoMap_g[i] = PNULL;
}


    macRegFlag_g[cellIndex] = FALSE;
    cellConfigured_g[cellIndex] = FALSE;

    return MAC_SUCCESS;
}
/* -CA  */

/* CA */ 
/****************************************************************************
* Function Name  : initIdxTbl
* Inputs         : None
* Outputs        : None
* Returns        : None
* Variables      :
* Description    : This API will initialize the idxTbl, temp CRNTI Pool,
*                  free Indexes Pool and freeIdxQueue.
****************************************************************************/
/* + CA */
void initIdxTbl(UInt8 cellIndex)
{
    UInt16 i = 0;
    /* SPR 11962 changes start */
    UInt16 spscrntiRange = cellConfigUeSim_g[cellIndex].spscrntiRange;
    /* SPR 11962 changes end */
    UInt32 rnti = 0;
    UInt32 diff = 0;
    
    /* Initialize rnti to Idx mapping table */
    rntiToIdxTbl_g[cellIndex] =  ( RNTIInfo *) getMemFromPool( 
            /* SPR 11962 fix start */
            sizeof(RNTIInfo) * (cellConfigUeSim_g[cellIndex].crntiRange + spscrntiRange), PNULL);
            /* SPR 11962 fix end */
    if(rntiToIdxTbl_g[cellIndex] == PNULL) {
         ltePanic("initIdxTbl: Memory Allocation failed for rntiToIdxTbl_g[%d]\n", cellIndex);
    }

    for ( rnti = cellConfigUeSim_g[cellIndex].lowerCRNTIBound; rnti <= cellConfigUeSim_g[cellIndex].upperCRNTIBound ; rnti++ )
    {
        if ((rnti >= startRARNTI_g) &&
            (rnti <= endRARNTI_g[cellIndex]))
        {
            continue;
        }
		if (cellConfigUeSim_g[cellIndex].lowerCRNTIBound != START_RARNTI)
		{
			diff = 0;
		}
		else
		{
			diff = endRARNTI_g[cellIndex] - startRARNTI_g + 1 ; 
		}
		i = rnti - diff  - cellConfigUeSim_g[cellIndex].lowerCRNTIBound;
        rntiToIdxTbl_g[cellIndex][i].ueIndex = MAX_UE_SUPPORTED;
        rntiToIdxTbl_g[cellIndex][i].state = UE_INACTIVE;
        rntiToIdxTbl_g[cellIndex][i].rntiType = NONE;
        rntiToIdxTbl_g[cellIndex][i].aperiodiccqiInfo = 
			(APeriodicCQI *) getMemFromPool(sizeof(APeriodicCQI), PNULL );
        if ( rntiToIdxTbl_g[cellIndex][i].aperiodiccqiInfo == PNULL )
        {
             ltePanic("initIdxTbl: Memory Allocation failed for APeriodicCQI\n");
        }
        rntiToIdxTbl_g[cellIndex][i].periodiccqiInfo = (PeriodicCQI *) getMemFromPool(
                sizeof(PeriodicCQI), PNULL );
        if ( rntiToIdxTbl_g[cellIndex][i].periodiccqiInfo == PNULL )
        {
             ltePanic("initIdxTbl: Memory Allocation failed for PeriodicCQI\n");
        }
    }
    for ( i = cellConfigUeSim_g[cellIndex].crntiRange; i < cellConfigUeSim_g[cellIndex].crntiRange + 
            spscrntiRange ; i++ )
    {
        rntiToIdxTbl_g[cellIndex][i].ueIndex = MAX_UE_SUPPORTED;
        rntiToIdxTbl_g[cellIndex][i].state = UE_INACTIVE;
        rntiToIdxTbl_g[cellIndex][i].rntiType = NONE;
    }
    

     harqIndQNode_gp[cellIndex] =  ( HarqIndQueueNodeInfo *) getMemFromPool( 
             sizeof(HarqIndQueueNodeInfo) * 
             ( cellConfigUeSim_g[cellIndex].crntiRange ), PNULL);
     if(harqIndQNode_gp[cellIndex] == PNULL) 
     {
         ltePanic(" Memory Allocation failed for harqIndQNode_gp[%d]\n", cellIndex);
     }
     UInt32 rntiCntr = 0;
     for (rntiCntr = 0; rntiCntr < cellConfigUeSim_g[cellIndex].crntiRange; rntiCntr++)
     {
        harqIndQNode_gp[cellIndex][rntiCntr].state = FALSE;
/* + SPR 14111 Changes */
#ifdef FDD_CONFIG        
        harqIndQNode_gp[cellIndex][rntiCntr].qIndex = MAX_SUBFRAME;
#elif TDD_CONFIG        
        harqIndQNode_gp[cellIndex][rntiCntr].qIndex = MAX_UL_ARRAY;
#endif        
/* - SPR 14111 Changes */
     } 
}
/* - CA */

/****************************************************************************
* Function Name  : createSockets
* Inputs         : None
* Outputs        : None
* Returns        : None
* Variables      :
* Description    : This API will create sockets for receiving/transmitting data
*                  to/from RRC.
****************************************************************************/

void createSockets(UInt16 rxMacEmuPort, UInt16 txOAMPort, UInt16 txRRCPort)
{
    /* Socket creation for Receiving data from RRC Layer */
    SockAddr rx_addr;
  
/*
**
**  The following socket will listen to any data from UE
**
*/

    if ((rxSockFD_g = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic("Error while opening the receiver socket.\n");
        exit(0);
    }

    rx_addr.sin_family = AF_INET;
    rx_addr.sin_port = htons (rxMacEmuPort);
   
/* SPR 609 changes start */
    if (inet_aton((const char *)l2ModuleIP,&rx_addr.sin_addr) == 0)
/* SPR 609 changes end */
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }
                            
    if ((bind(rxSockFD_g, (struct sockaddr *)&rx_addr, sizeof(rx_addr))) == -1)
    {
        ltePanic ("Unable to bind the receiver socket with RRC.\n");
        exit(0);
    }

/*
**
**  The following socket will be used to transmit data to UE RRC
**
*/
    if ((txRRCSockFD_g = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic(" Socket creation failed.\n");
        exit(0);
    }

    tx_addr_g.sin_family = AF_INET;
    tx_addr_g.sin_port = htons(txRRCPort);
/* SPR 609 changes start */
    if (inet_aton((const char *)rrcModuleIP_g,&tx_addr_g.sin_addr) == 0)
/* SPR 609 changes end */
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }
    
/*
**
**  The following socket will be used to transmit data to UE OAM
**
*/
    if ((txOAMSockFD_g = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP)) == -1)
    {
        ltePanic(" Socket creation failed for Tx to OAM.\n");
        exit(0);
    }

    txOAM_addr_g.sin_family = AF_INET;
    txOAM_addr_g.sin_port = htons(txOAMPort);
/* SPR 609 changes start */
    if (inet_aton((const char *)oamModuleIP_g, &txOAM_addr_g.sin_addr) == 0)
/* SPR 609 changes end */
    {
        ltePanic ("Error while converting the address.");
        exit(0);
    }    
}


/****************************************************************************
* Function Name  : initializeMAC
* Inputs         : None
* Outputs        : Initialize MAC Layer threads
* Returns        : Success if MAC Layer initialized. 
* Variables      :
* Description    : This API initialize the MAC Layer 
****************************************************************************/
/* + CA */
UInt8 initializeMAC( UInt8 cellIndex)
{
    /* Initialize index table with all indexes in free indexes queue */
    initIdxTbl(cellIndex);

    /* Initialize MAC Layer */
    if(MAC_FAILURE == initMACLayer(cellIndex))
    {
#ifdef LTE_MAC_UT_LOG
        lteWarning("MAC Initialization fails\n");
#endif
        return MAC_FAILURE;
    }
    return MAC_SUCCESS;
}
/* - CA */


#ifdef FAPI_UL_HARQ_ENABLE 
inline void initULHarqTimerArray()
{
    UInt8 i = 0;
    UInt8 j = 0;
    ULHarqTimerEntityList* ulHarqEntityTimerList_p = PNULL;
    ULHarqTimerNode* ulharqTimerNode_p = PNULL;



    for (i = 10; i; i-- )
    {
        ulHarqEntityTimerList_p = &ulHarqTimerEntityList_g[i-1];
        for (j=100; j; j--)
        {
            ulharqTimerNode_p = &ulHarqEntityTimerList_p->ulHarqTimerNodeArr[j-1];
            ulharqTimerNode_p->harqProcessId = 255;
            ulharqTimerNode_p->isValid = 0;
            ulharqTimerNode_p->ueIndex = INVALID_UE_INDEX;
            ulharqTimerNode_p->qIndex = 255;
            ulharqTimerNode_p->crnti = INVALID_UE_INDEX;
            ulharqTimerNode_p->sfn = 0;
            ulharqTimerNode_p->sf = 0;            
        }
        ulHarqEntityTimerList_p->count = 0;
    }

}
#endif




/****************************************************************************
* Function Name  : initMAC
* Inputs         : None
* Outputs        : Initialize registered Layer2 modules
* Returns        : Success if Layer2 initialized. 
* Variables      :
* Description    : This API initialize the Layer2 modules which are regsitered
*                  to it.
****************************************************************************/
UInt8 initMAC( UInt16 *rxenbMacPort, 
               UInt16 *txMacPort,
               /* SPR 10868 Changes Start */
               UInt16 *rxPorteNBMacForHO, 
               /* SPR 10868 Changes End */
               UInt8 cellIndex
#ifdef DL_UL_SPLIT_TDD
               ,UInt16 *txMacPortULThd
#endif
	     )
               /* SPR 5313 changes end */
{
    UInt8 idx = 0;

#ifdef FAPI_UL_HARQ_ENABLE 
    initULHarqTimerArray();
#endif

#ifdef LTE_MAC_UT_LOG
    createMACEmulatorLogdetail();
#endif
    /* SPR 5313 changes start */
    fapiInit(rxenbMacPort, txMacPort, rxPorteNBMacForHO, cellIndex
#ifdef DL_UL_SPLIT_TDD
            , txMacPortULThd
#endif
	    );
    /* SPR 5313 changes end */

#ifdef FDD_CONFIG
    for( idx = 0; idx < MAX_SUBFRAME; idx++ ) 
#elif TDD_CONFIG
    for( idx = 0; idx < MAX_UL_ARRAY; idx++ ) 
#endif
    {
        INIT_PHY_ERR_IND_Q( idx , cellIndex);
    }

    /*
       The following thread will create the Tick 
       and post the semaphore.
       */
    // Moved to main for thread creation
#if 0
    ret = threadCreate(&PhyTimerThread_g,
            THREAD_CREATE_DETACHED,
            /*THREAD_SCHED_FIFO,*/
            SCHED_FIFO,
            /*0,*/
            TIMER_THREAD_PRIORITY, 
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_DEFAULT,
            phyTimerThread,
            (void *) NULL );

    if ( ret != 0)
    {
        ltePanic("Error in creating Phy timer thread");
    }

    /*
       The following thread will be used to send UL control
       data Messages to the EnodeB MAC.
       */
    ret = threadCreate(&UlDispatcherThread_g,
            THREAD_CREATE_DETACHED,
            /*THREAD_SCHED_FIFO,*/
            SCHED_FIFO,
            /*0,*/
            ULDISPATCHER_THREAD_PRIORITY,
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_DEFAULT,
            ulDispatcherThread,
            (void *) NULL );
    if ( ret != 0)
    {
        ltePanic("Error in creating ULDispatcher thread");
    }

    /*
       The following thread will be responsible for
       reading the Messages (Both Control and Data)
       from the EnodeB MAC in the DL Path.
       */

    ret = threadCreate(&ulReceiverThread_g,
            THREAD_CREATE_DETACHED,
            /*THREAD_SCHED_FIFO,*/
            SCHED_FIFO,
            /*0,*/
            ULRECEIVER_THREAD_PRIORITY,
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_DEFAULT,
            ulReceiverThread,
            (void *) NULL );
    if ( ret != 0)
    {
        ltePanic("Error in creating DL Receiver thread");
    }

#endif

    /*
       The following thread is resposible for envoking 
       proper functionality of the MAC EMULATOR (UE MAC)
       and updating the UL Control and DataQueue to be
       used by the UL Dispatcher Thead.
       */ 
#if 0
    ret = threadCreate(&ulDataProcessThread_g,
            THREAD_CREATE_DETACHED,
            THREAD_SCHED_FIFO,
            0,
            THREAD_EXPLICIT_SCHED,
            THREAD_SCOPE_DEFAULT,
            ulDataProcessThread,
            (void *) NULL );
    if ( ret != 0)
    { 
        ltePanic("Error in creating Data Process thread");
    }
#endif
    return MAC_SUCCESS;
}
