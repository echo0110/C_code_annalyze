/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright 2009 (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTrafficGenMain.c,v 1.3 2010/03/11 13:40:03 gur19479 Exp $
 *
 ****************************************************************************
 *
 *  File Description :
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: lteTrafficGenMain.c,v $
 * Revision 1.3  2010/03/11 13:40:03  gur19479
 * core binding chnages.
 *
 * Revision 1.2  2010/02/18 07:31:11  gur19479
 * updated for endianess
 *
 * Revision 1.1.2.4.2.1  2009/09/24 21:40:30  gur19479
 * updated for video and audio processing
 *
 * Revision 1.1.2.4  2009/08/12 08:23:55  gur19836
 * Graceful shutdown of traffic generator code added
 *
 * Revision 1.1.2.3  2009/07/28 15:36:58  gur19836
 * Downlink Issues Resolved, Latency Measurement code added
 *
 * Revision 1.1.2.2  2009/07/14 14:08:28  gur19836
 * CR Comments Disposed
 *
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "lteTrafficGenExtInterface.h"
#include "lteTrafficGenEntities.h"
#include "lteTrafficGenRrcInterface.h"
#include "lteRrcInterface.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/


/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern UInt8 initializeTrafficGen_g;
UInt32 diffInMiliSec_g = 5000;
UInt32 coreNum1 = 0, coreNum2 =0;
/****************************************************************************
 * Function Name  : initRrcFuncPtr
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function initializes the function pointers for 
 *                  traffic generator's RRC Interface.
 ****************************************************************************/

void initRrcFuncPtr()
{
    processIncomingMessage[UE_CREATE_ENTITY_REQ] = processCreateUeReq;
    processIncomingMessage[UE_RECONFIG_ENTITY_REQ] = processReconfigUeReq;
    processIncomingMessage[UE_DELETE_ENTITY_REQ] = processDeleteUeReq;
    processIncomingMessage[TRAFFIC_GEN_START_STOP_REQ] = processTrafficGenStartStopReq;
    processIncomingMessage[TRAFFIC_GEN_CLEANUP_REQ] = processTrafficGenCleanupReq;
    
    sendConfirmationMessage[UE_CREATE_ENTITY_REQ] = processCreateUeCnf;      
    sendConfirmationMessage[UE_RECONFIG_ENTITY_REQ] = processReconfigUeCnf; 
    sendConfirmationMessage[UE_DELETE_ENTITY_REQ] = processDeleteUeCnf;      
    sendConfirmationMessage[TRAFFIC_GEN_START_STOP_REQ] = processTrafficGenStartStopCnf;      

}

/****************************************************************************
 * Function Name  : main
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Variables      :
 * Description    : This function initializes traffic generator and spawns 
 *                  all traffic generator's threads.
 ****************************************************************************/
int main(UInt32 argc, UInt8 *argv[])
{
    /* Initialise the memory pool required by traffic generator */
    fprintf(stderr,"Traffic Generator Starting......");
    if(argc < 4)
    {    
       fprintf(stderr,"Usage is: ./ulTrafficGen <core Num for > <core Num for Timer Thread> <throughput display (time in ms)>");
       return 0;
    }
    coreNum1 = atoi(argv[1]);
    coreNum2 = atoi(argv[2]);
    diffInMiliSec_g = atoi(argv[3]);
    initMemPool();
    initializeTrafficGen_g = 1;    
    /* initialise the traffic generator module */
    initTrafficGen();

    /* initialise the database maintained by traffic generator */
    trafficGenInitDbReq();

    /* thread for receiving the data from adapter */
    trafficGenRxData();
    return 0; 
}
