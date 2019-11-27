/***********************************************************************
* ARICENT -                                                            
*                                                                      
* Copyright (C) 2009 Aricent Inc. All Rights Reserved.                 
*                                                                      
************************************************************************
* Filename: lteCommonQueueDefs.h                                                  
* Description: This file contains Macros for Queue handling of Queues used 
*              in Layer2.
***********************************************************************/

#ifndef LTE_COMMON_QUEUE_DEF_H
#define LTE_COMMON_QUEUE_DEF_H

/***********************************************************************
* QueueName              : ackNackCircQueue_g                               
* Structure of Queue Node: AckNackQueueNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define ACK_NACK_CIRC_TYPE    LTE_CIRCQUEUE

#define GET_MEM_NODE_ACK_NACK_CIRC_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_ACK_NACK_CIRC_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);
/*CA Changes start  */
#define CONSTRUCT_AND_INIT_ACK_NACK_CIRC_Q(internalCellIndex)   \
        createMemPool(sizeof(LTE_CIRCQUEUE), 1);\
        do{                                            \
              GET_MEM_FROM_POOL(LTE_CIRCQUEUE, ackNackCircQueue_gp[internalCellIndex], sizeof(LTE_CIRCQUEUE), PNULL); \
              circQInit(ackNackCircQueue_gp[internalCellIndex], MAX_NODES_SPLIT); \
          }while(0);

#define COUNT_ACK_NACK_CIRC_Q(internalCellIndex) \
        circQueueCount(ackNackCircQueue_gp[internalCellIndex])

#define ENQUEUE_ACK_NACK_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
        pushNodeCircQ(ackNackCircQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_ACK_NACK_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(ackNackCircQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_ACK_NACK_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(ackNackCircQueue_gp[internalCellIndex], (node_p))

#define QDEINIT_NODE_ACK_NACK_CIRC_Q(internalCellIndex) \
        circQDeInit(ackNackCircQueue_gp[internalCellIndex])

/*CA Changes end  */

/*********** End of ackNackCircQueue_g *******************/


/***********************************************************************
* QueueName              : demuxRachManagerInterfaceQueue_g                               
* Structure of Queue Node: RachDemuxQueueNode                                  
* Queue Type             : Circular Queue -  Intra-Core CIRC Q          
***********************************************************************/

#define DEMUX_RACH_MANAGER_INTERFACE_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_DEMUX_RACH_MANAGER_INTERFACE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DEMUX_RACH_MANAGER_INTERFACE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */

#define CONSTRUCT_AND_INIT_DEMUX_RACH_MANAGER_INTERFACE_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
         createMemPool(sizeof(DEMUX_RACH_MANAGER_INTERFACE_TYPE), numCells); \
         do{                                           \
               GET_MEM_FROM_POOL(DEMUX_RACH_MANAGER_INTERFACE_TYPE, demuxRachManagerInterfaceQueue_gp[numCells - 1], \
                      sizeof(DEMUX_RACH_MANAGER_INTERFACE_TYPE), PNULL); \
               circQInit(demuxRachManagerInterfaceQueue_gp[numCells - 1], MAX_NODES_SPLIT); \
           }while(--numCells);\
        }

#define COUNT_DEMUX_RACH_MANAGER_INTERFACE_Q(internalCellIndex) \
          circQueueCount(demuxRachManagerInterfaceQueue_gp[internalCellIndex])

#define ENQUEUE_DEMUX_RACH_MANAGER_INTERFACE_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(demuxRachManagerInterfaceQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_DEMUX_RACH_MANAGER_INTERFACE_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(demuxRachManagerInterfaceQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_DEMUX_RACH_MANAGER_INTERFACE_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(demuxRachManagerInterfaceQueue_gp[internalCellIndex], (node_p))
/*Coverity-5.3.0 CID-54598*/
#define QDEINIT_DEMUX_RACH_MANAGER_INTERFACE_Q(internalCellIndex)\
        circQDeInit(demuxRachManagerInterfaceQueue_gp[internalCellIndex])
/*CA Changes end  */
/*Coverity-5.3.0 CID-54598*/



/*********** End of demuxRachManagerInterfaceQueue_g *******************/


/***********************************************************************
* QueueName              : dlDRXOnOffTriggerQueue_g                               
* Structure of Queue Node: DRXOnOffTriggerNode                                  
* Queue Type             : Circular Queue -  Intra-Core CIRC Q          
***********************************************************************/

#define DL_DRX_ON_OFF_TRIGGER_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DL_DRX_ON_OFF_TRIGGER_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_DL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex) \
         circQInit(dlDRXOnOffTriggerQueue_gp[internalCellIndex], MAX_NODES_SPLIT); \

#define COUNT_DL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex) \
          circQueueCount(dlDRXOnOffTriggerQueue_gp[internalCellIndex])

#define ENQUEUE_DL_DRX_ON_OFF_TRIGGER_Q(nodeStruct, node_p,internalCellIndex) \
      pushNodeCircQ((dlDRXOnOffTriggerQueue_gp[internalCellIndex]), (node_p))

#define DEQUEUE_DL_DRX_ON_OFF_TRIGGER_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(dlDRXOnOffTriggerQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_DL_DRX_ON_OFF_TRIGGER_Q(nodeStruct, node_p,internalCellIndex) \
        getNodeCircQ(dlDRXOnOffTriggerQueue_gp[internalCellIndex], (node_p))


/*********** End of dlDRXOnOffTriggerQueue_g *******************/


/***********************************************************************
* QueueName              : dlMeasGapOnOffTriggerQueue_g                               
* Structure of Queue Node: MeasGapOnOffTriggerNode                                  
* Queue Type             : Circular Queue -  Intra-Core CIRC Q          
***********************************************************************/

#define DL_MEAS_GAP_ON_OFF_TRIGGER_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(DL_MEAS_GAP_ON_OFF_TRIGGER_TYPE), numCells);\
        do{                                          \
            GET_MEM_FROM_POOL(DL_MEAS_GAP_ON_OFF_TRIGGER_TYPE, dlMeasGapOnOffTriggerQueue_gp[numCells - 1], \
            sizeof(DL_MEAS_GAP_ON_OFF_TRIGGER_TYPE), PNULL); \
            circQInit(dlMeasGapOnOffTriggerQueue_gp[numCells - 1], MAX_NODES_SPLIT); \
          }while(--numCells);\
        } 

#define COUNT_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(internalCellIndex) \
          circQueueCount(dlMeasGapOnOffTriggerQueue_gp[internalCellIndex])

#define ENQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(nodeStruct, node_p,internalCellIndex) \
      pushNodeCircQ(dlMeasGapOnOffTriggerQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(dlMeasGapOnOffTriggerQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_DL_MEAS_GAP_ON_OFF_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(dlMeasGapOnOffTriggerQueue_gp[internalCellIndex], (node_p))


/*********** End of dlMeasGapOnOffTriggerQueue_g *******************/

/***********************************************************************
* QueueName              : dlUEReconfigDataQueue                               
* Structure of Queue Node: UELocationNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q(Inter-Thread)          
***********************************************************************/

#define DL_UE_RECONFIG_DATA_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_DL_UE_RECONFIG_DATA_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DL_UE_RECONFIG_DATA_Q(node_p) \
        do{\
              freeMemPool(node_p);\
             node_p=PNULL;\
          }while(0);

#define INIT_DL_UE_RECONFIG_DATA_Q(internalCellIndex) \
        circQInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        dlUEReconfigDataQueue),MAX_NODES_SPLIT)

#define COUNT_DL_UE_RECONFIG_DATA_Q(internalCellIndex) \
        circQueueCount(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        dlUEReconfigDataQueue))

#define ENQUEUE_DL_UE_RECONFIG_DATA_Q(nodeStruct, node_p, internalCellIndex) \
        pushNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        dlUEReconfigDataQueue), (node_p))

#define DEQUEUE_DL_UE_RECONFIG_DATA_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        dlUEReconfigDataQueue), (node_p))

#define GET_NODE_DL_UE_RECONFIG_DATA_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(irrmMacCellGlobal_gp[internalCellIndex]-> \
        dlUEReconfigDataQueue, (node_p))


/*********** End of dlUEReconfigDataQueue *******************/
/* EICIC +*/
/***********************************************************************
* QueueName              :  ueReconfigUserTypeQueue                               
* Structure of Queue Node: UEUserTypeNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q(Inter-Thread)          
***********************************************************************/

#define UE_RECONFIG_USER_TYPE   LTE_CIRCQUEUE

#define GET_MEM_UE_RECONFIG_USER_TYPE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UE_RECONFIG_USER_TYPE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
             node_p=PNULL;\
          }while(0);

#define INIT_UE_RECONFIG_USER_TYPE_Q(internalCellIndex) \
        circQInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigUserTypeQueue),MAX_NODES_SPLIT)

#define COUNT_UE_RECONFIG_USER_TYPE_Q(internalCellIndex) \
        circQueueCount(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigUserTypeQueue))

#define ENQUEUE_UE_RECONFIG_USER_TYPE_Q(nodeStruct, node_p, internalCellIndex) \
        pushNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigUserTypeQueue), (node_p))

#define DEQUEUE_UE_RECONFIG_USER_TYPE_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigUserTypeQueue), (node_p))

#define GET_NODE_UE_RECONFIG_USER_TYPE_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(irrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigUserTypeQueue, (node_p))
/* EICIC -*/

/***********************************************************************
* QueueName              : dlUEZeroNonZeroQueueLoadTriggerQueue                               
* Structure of Queue Node: ZeroNonZeroTriggerNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(internalCellIndex, macMaxNumOfUeSupported, macMaxNumOfLcSupported) \
          circQInit(dlUEZeroNonZeroQueueLoadTriggerQueue_gp[internalCellIndex],\
              (macMaxNumOfUeSupported * macMaxNumOfLcSupported))

#define COUNT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(internalCellIndex) \
          circQueueCount(dlUEZeroNonZeroQueueLoadTriggerQueue_gp[internalCellIndex])

#define ENQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(nodeStruct, node_p,internalCellIndex) \
      pushNodeCircQ(dlUEZeroNonZeroQueueLoadTriggerQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(dlUEZeroNonZeroQueueLoadTriggerQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(nodeStruct, node_p,internalCellIndex) \
        getNodeCircQ(&dlUEZeroNonZeroQueueLoadTriggerQueue_gp[internalCellIndex], (node_p))
/* SPR 8702 changes start */
#define QDEINIT_DL_UE_ZERO_NON_ZERO_QUEUE_LOAD_TRIGGER_Q(node_p) \
        circQDeInit(node_p);
/* SPR 8702 changes end */

/*********** End of dlUEZeroNonZeroQueueLoadTriggerQueue *******************/


/***********************************************************************
* QueueName              : drxDLSchedulerEventQueue_g                               
* Structure of Queue Node: DRXEventTriggerNode                                  
* Queue Type             : Circular Queue -  Intra-Core CIRC Q          
***********************************************************************/

#define DRX_DL_SCHEDULER_EVENT_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_DRX_DL_SCHEDULER_EVENT_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DRX_DL_SCHEDULER_EVENT_Q(node_p) \
        do{\
              freeMemPool(node_p);\
             node_p=PNULL;\
          }while(0);
#define CONSTRUCT_AND_INIT_DRX_DL_SCHEDULER_EVENT_Q(numOfCells) { \
       UInt8   numConfiguredCells = numOfCells;\
        createMemPool(sizeof(DRX_DL_SCHEDULER_EVENT_TYPE), numConfiguredCells);\
        do{                                          \
              GET_MEM_FROM_POOL(DRX_DL_SCHEDULER_EVENT_TYPE, drxDLSchedulerEventQueue_gp[numConfiguredCells - 1], sizeof(DRX_DL_SCHEDULER_EVENT_TYPE), PNULL); \
              /* Review comment fix start  LJ3 */\
              circQInit(drxDLSchedulerEventQueue_gp[numConfiguredCells - 1], cellSpecificParams_g.\
                        cellConfigAndInitParams_p[numConfiguredCells - 1]->initParams_p->macMaxNumOfUeSupported);\
              /* Review comment fix end  LJ3 */\
          }while(--numConfiguredCells);\
        }

#define COUNT_DRX_DL_SCHEDULER_EVENT_Q(internalCellIndex) \
          circQueueCount((drxDLSchedulerEventQueue_gp[internalCellIndex]))

#define ENQUEUE_DRX_DL_SCHEDULER_EVENT_Q(nodeStruct, node_p,internalCellIndex) \
      pushNodeCircQ((drxDLSchedulerEventQueue_gp[internalCellIndex]), (node_p))

#define DEQUEUE_DRX_DL_SCHEDULER_EVENT_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(drxDLSchedulerEventQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_DRX_DL_SCHEDULER_EVENT_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(drxDLSchedulerEventQueue_gp[internalCellIndex], (node_p))


/*********** End of drxDLSchedulerEventQueue_g *******************/


/***********************************************************************
* QueueName              : drxULSchedulerEventQueue_g                               
* Structure of Queue Node: DRXEventTriggerNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define DRX_UL_SCHEDULER_EVENT_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_DRX_UL_SCHEDULER_EVENT_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DRX_UL_SCHEDULER_EVENT_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);
#define CONSTRUCT_AND_INIT_DRX_UL_SCHEDULER_EVENT_Q(numOfCells) {\
        UInt8 numCells = numOfCells;\
        createMemPool(sizeof(DRX_UL_SCHEDULER_EVENT_TYPE), numCells);\
        do{                                          \
              GET_MEM_FROM_POOL(DRX_UL_SCHEDULER_EVENT_TYPE, drxULSchedulerEventQueue_gp[numCells - 1], sizeof(DRX_UL_SCHEDULER_EVENT_TYPE), PNULL); \
              /* Review comment fix start  LJ3 */\
              circQInit(drxULSchedulerEventQueue_gp[numCells - 1], cellSpecificParams_g.\
                        cellConfigAndInitParams_p[numCells - 1]->initParams_p->macMaxNumOfUeSupported);\
              /* Review comment fix end  LJ3 */\
          }while(--numCells);\
        }

#define COUNT_DRX_UL_SCHEDULER_EVENT_Q(internalCellIndex) \
          circQueueCount((drxULSchedulerEventQueue_gp[internalCellIndex]))

#define ENQUEUE_DRX_UL_SCHEDULER_EVENT_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ((drxULSchedulerEventQueue_gp[internalCellIndex]), (node_p))

#define DEQUEUE_DRX_UL_SCHEDULER_EVENT_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(drxULSchedulerEventQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_DRX_UL_SCHEDULER_EVENT_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(drxULSchedulerEventQueue_gp[internalCellIndex], (node_p))


/*********** End of drxULSchedulerEventQueue_g *******************/


/***********************************************************************
* QueueName              : encoderCircQueue                               
* Structure of Queue Node: EncoderQueueNode                                  
* Queue Type             : Circular Queue -  ICC Queue (Inter-Thread)          
***********************************************************************/

#define ENCODER_CIRC_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_ENCODER_CIRC_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_ENCODER_CIRC_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_ENCODER_CIRC_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_ENCODER_CIRC_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_ENCODER_CIRC_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_ENCODER_CIRC_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_ENCODER_CIRC_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))


/*********** End of encoderCircQueue *******************/


/***********************************************************************
* QueueName              : explicitDLPucchTpcScheduleQueue_g                               
* Structure of Queue Node: ExplicitDLDCIQueueNode                                  
* Queue Type             : Circular Queue -  Intra-Core CIRC Q          
***********************************************************************/

#define EXPLICIT_DL_PUCCH_TPC_SCHEDULE_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_EXPLICIT_DL_PUCCH_TPC_SCHEDULE_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))


/*********** End of explicitDLPucchTpcScheduleQueue_g *******************/


/***********************************************************************
* QueueName              : explicitULPuschTpcScheduleQueue_g                               
* Structure of Queue Node: ExplicitULDCINode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define EXPLICIT_UL_PUSCH_TPC_SCHEDULE_TYPE           LTE_CIRCQUEUE

#define GET_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))

/*CA Changes start  */
/*Coverity-5.3.0 CID-54633*/
#define QDEINIT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_Q(internalCellIndex) \
        circQDeInit(explicitULPuschTpcScheduleQueue_gp[internalCellIndex])
/*Coverity-5.3.0 CID-54633*/

/*Coverity-5.3.0 CID-54632*/
#define QDEINIT_EXPLICIT_UL_PUSCH_TPC_SCHEDULE_FAILURE_Q(internalCellIndex) \
        circQDeInit(explicitULPuschTpcScheduleFailureQueue_gp[internalCellIndex])
/*Coverity-5.3.0 CID-54632*/
/*CA Changes end  */

/********** *End of explicitULPuschTpcScheduleQueue_g *******************/

/*ca chngs start*/

/***********************************************************************
* QueueName              : dlCAEventQueue_g                               
* Structure of Queue Node: DLCAEventQueueNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/
/*DL_CA_EVENT_Q manages the events for the scell state machine for a particular state
the events can be triggered from interface , strategy ,ca timer expire and mux successfull scheduling
proper handling has been provided for each event and scell state pair*/

#define DL_CA_EVENT_QUEUE_TYPE           LTE_CIRCQUEUE

#define GET_MEM_NODE_DL_CA_EVENT_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DL_CA_EVENT_Q(node_p) \
         do{ \
             freeMemPool(node_p);\
             node_p=PNULL;\
         }while(0)

#define INIT_DL_CA_EVENT_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_DL_CA_EVENT_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_DL_CA_EVENT_Q(qName_p,node_p) \
      pushNodeCircQ(qName_p, node_p)

#define DEQUEUE_DL_CA_EVENT_Q(qName_p,node_p) \
        popNodeCircQ(qName_p, node_p)

#define GET_NODE_DL_CA_EVENT_Q(qName_p,node_p) \
        getNodeCircQ(qName_p, node_p)

#define QDEINIT_DL_CA_EVENT_Q(internalCellIndex) \
        circQDeInit(dlCAEventQueue_gp[internalCellIndex])
 
/***********************************************************************
* QueueName              : dlCAMacCEQueue_g                               
* Structure of Queue Node: DLCAMacCEQueueNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/
/*The MAC_CE_QUEUE is used to schedule a MAC CE to UE for activation/deactivation 
of the scell configured.each node consists of event ,ue id and scell id*/
#define DL_CA_MAC_CE_QUEUE_TYPE           LTE_CIRCQUEUE

#define GET_MEM_NODE_DL_CA_MAC_CE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL) 

#define FREE_MEM_NODE_DL_CA_MAC_CE_Q(node_p) \
         do \
         {\
            freeMemPool(node_p);\
            node_p=PNULL; \
         }while(0)

#define INIT_DL_CA_MAC_CE_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_DL_CA_MAC_CE_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_DL_CA_MAC_CE_Q(qName_p,node_p) \
      pushNodeCircQ(qName_p, node_p)

#define DEQUEUE_DL_CA_MAC_CE_Q(qName_p,node_p) \
        popNodeCircQ(qName_p, node_p)

#define GET_NODE_DL_CA_MAC_CE_Q(qName_p,node_p) \
        getNodeCircQ(qName_p, node_p)
/*Deint for MacCEQueue*/
#define QDEINIT_DL_CA_MAC_CE_Q(internalCellIndex) \
        circQDeInit(dlCAMacCEQueue_gp[internalCellIndex])

/*Deint for PendingMacCEQueue*/
#define QDEINIT_DL_CA_PEND_MAC_CE_Q(internalCellIndex) \
        circQDeInit(pendingDlCAMacCEQueue_gp[internalCellIndex])

/*Deint for pendingFailureDlCAMacCEQueue_gp*/
#define QDEINIT_DL_CA_PEND_FAIL_MAC_CE_Q(internalCellIndex) \
        circQDeInit(pendingFailureDlCAMacCEQueue_gp[internalCellIndex])

/*phase-3 changes*/
/***********************************************************************
* QueueName              : dlCAMacFreeScellHarqBufferQueue_g                               
* Structure of Queue Node: DLCAMacCEQueueNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/
/*The MAC_CE_QUEUE is used to schedule a MAC CE to UE for activation/deactivation 
of the scell configured.each node consists of event ,ue id and scell id*/

#define DL_CA_MAC_FREE_SCELL_HARQ_QUEUE_TYPE           LTE_CIRCQUEUE

#define GET_MEM_NODE_FREE_SCELL_HARQ_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL) 

#define FREE_MEM_NODE_FREE_SCELL_HARQ_Q(node_p) \
         do \
         {\
            freeMemPool(node_p);\
            node_p=PNULL; \
         }while(0)
/*Code Review Comments Changes Start*/
#define INIT_FREE_SCELL_HARQ_Q(qName_p) \
          circQInit(qName_p,MAX_UE_SUPPORTED)
/*Code Review Comments Changes End*/

#define COUNT_FREE_SCELL_HARQ_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_FREE_SCELL_HARQ_Q(qName_p,node_p) \
      pushNodeCircQ(qName_p, node_p)

#define DEQUEUE_FREE_SCELL_HARQ_Q(qName_p,node_p) \
        popNodeCircQ(qName_p, node_p)

#define GET_NODE_FREE_SCELL_HARQ_Q(qName_p,node_p) \
        getNodeCircQ(qName_p, node_p)

/*Deint for FreeScellHarqBufferQueue*/
#define QDEINIT_FREE_SCELL_HARQ_Q(internalCellIndex) \
        circQDeInit(dlCAMacFreeScellHarqBufferQueue_gp[internalCellIndex])
/*phase-3 changes*/
/*CA Changes end  */

/***********************************************************************
* QueueName              : macUeDataIndQ_g                               
* Structure of Queue Node: MacUeDataInd                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q (Inter-Thread)          
***********************************************************************/

#define MAC_UE_DATA_IND_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_MAC_UE_DATA_IND_Q(node_p, nodeStruct) \
        node_p = (nodeStruct *)getMemFromPool(sizeof(nodeStruct) ,PNULL)

#define FREE_MEM_NODE_MAC_UE_DATA_IND_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);


/** CA 2 Core Changes Start **/

#define INIT_MAC_UE_DATA_IND_Q(internalCellIndex) \
          circQInit(&(macUeDataIndQ_g[internalCellIndex]),MAX_NODES_SPLIT)

#define COUNT_MAC_UE_DATA_IND_Q(internalCellIndex) \
          circQueueCount(&(macUeDataIndQ_g[internalCellIndex]))

#define ENQUEUE_MAC_UE_DATA_IND_Q(nodeStruct, node_p,internalCellIndex) \
      pushNodeCircQ(&(macUeDataIndQ_g[internalCellIndex]), (node_p))

#define DEQUEUE_MAC_UE_DATA_IND_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(&(macUeDataIndQ_g[internalCellIndex]), (node_p))

#define GET_NODE_MAC_UE_DATA_IND_Q(nodeStruct, node_p,internalCellIndex) \
        getNodeCircQ(&(macUeDataIndQ_g[internalCellIndex]), (node_p))

/** CA 2 Core Changes End **/
/*+ SPR 21121*/
#define QINIT_RLC_LP_TO_RLC_UL_SCH_Q(rlcLPtoRlcUlSchedCircQ_p) \
    circQInit(rlcLPtoRlcUlSchedCircQ_p, MAX_UE_SUPPORTED)

#define ENQUEUE_RLC_LP_TO_RLC_UL_SCH_Q(nodeStruct, node_p) \
    pushNodeCircQ(&(rlcLPtoRlcUlSchedCircQ_g), (node_p))

#define COUNT_RLC_LP_TO_RLC_UL_SCH_Q() \
    circQueueCount(&(rlcLPtoRlcUlSchedCircQ_g))

#define DEQUEUE_RLC_LP_TO_RLC_UL_SCH_Q(nodeStruct, node_p) \
    popNodeCircQ(&rlcLPtoRlcUlSchedCircQ_g, (node_p))

#define QINIT_RLC_UL_SCH_TO_LP_Q(rlcUlSchedToLPCircQ_p) \
    circQInit(rlcUlSchedToLPCircQ_p, MAX_UE_SUPPORTED)

#define ENQUEUE_RLC_UL_SCH_TO_LP_Q(nodeStruct, node_p) \
    pushNodeCircQ(&(rlcUlSchedToLPCircQ_g), (node_p))

#define COUNT_RLC_UL_SCH_TO_LP_Q() \
    circQueueCount(&(rlcUlSchedToLPCircQ_g))

#define DEQUEUE_RLC_UL_SCH_TO_LP_Q(nodeStruct, node_p) \
    popNodeCircQ(&rlcUlSchedToLPCircQ_g, (node_p))
/*- SPR 21121*/


/*********** End of macUeDataIndQ_g *******************/


/***********************************************************************
* QueueName              : macUeOpporIndCircQ_g                               
* Structure of Queue Node: MacUeOpporInd                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define MAC_UE_OPPOR_IND_CIRC_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(node_p, nodeStruct) \
        node_p = (nodeStruct *)getMemFromPool(sizeof(nodeStruct) ,PNULL)

#define FREE_MEM_NODE_MAC_UE_OPPOR_IND_CIRC_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/* CA Changes start */
#define CONSTRUCT_AND_INIT_MAC_UE_OPPOR_IND_CIRC_Q(internalCellIndex) \
    createMemPool(sizeof(MAC_UE_OPPOR_IND_CIRC_TYPE) , 1);\
    do{\
         macUeOpporIndCircQ_gp[internalCellIndex] = (MAC_UE_OPPOR_IND_CIRC_TYPE*) getMemFromPool(sizeof(MAC_UE_OPPOR_IND_CIRC_TYPE), PNULL); \
        circQInit(macUeOpporIndCircQ_gp[internalCellIndex], MAX_DL_UE_SCHEDULED * 2); \
    }while(0);


#define COUNT_MAC_UE_OPPOR_IND_CIRC_Q(internalCellIndex) \
    circQueueCount(macUeOpporIndCircQ_gp[internalCellIndex])

#define ENQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
    pushNodeCircQ(macUeOpporIndCircQ_gp[internalCellIndex], (node_p))

#define DEQUEUE_MAC_UE_OPPOR_IND_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
    popNodeCircQ(macUeOpporIndCircQ_gp[internalCellIndex], (node_p))

#define GET_NODE_MAC_UE_OPPOR_IND_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
    getNodeCircQ(macUeOpporIndCircQ_gp[internalCellIndex], (node_p))
/* CA Changes end */

/*********** End of macUeOpporIndCircQ_g *******************/


/***********************************************************************
* QueueName              : nackCircQueue                               
* Structure of Queue Node: ContainerNackQueueNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define NACK_CIRC_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_NACK_CIRC_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_NACK_CIRC_Q(node_p) \
          do{\
               freeMemPool(node_p);\
               node_p=PNULL;\
            }while(0);
#define INIT_NACK_CIRC_Q(qName_p)  \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_NACK_CIRC_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_NACK_CIRC_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_NACK_CIRC_Q(qName_p, nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_NACK_CIRC_Q(qName_p,   nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))


/*********** End of nackCircQueue *******************/


/***********************************************************************
* QueueName              : nackCircQueueForTempUE                               
* Structure of Queue Node: ContainerNackQueueNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define NACK_CIRC_QUEUE_FOR_TEMP_UE_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(qName_p, nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(qName_p, nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_NACK_CIRC_QUEUE_FOR_TEMP_UE_Q(qName_p, nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))


/*********** End of nackCircQueueForTempUE *******************/


/***********************************************************************
* QueueName              : pdcchOrderQueue_g                               
* Structure of Queue Node: PDCCHOrderNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define PDCCH_ORDER_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_PDCCH_ORDER_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_PDCCH_ORDER_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_PDCCH_ORDER_Q(internalCellIndex) \
          circQInit(pdcchOrderQueue_gp[internalCellIndex],MAX_NODES_SPLIT)

#define COUNT_PDCCH_ORDER_Q(internalCellIndex) \
          circQueueCount((pdcchOrderQueue_gp[internalCellIndex]))

#define ENQUEUE_PDCCH_ORDER_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ((pdcchOrderQueue_gp[internalCellIndex]), (node_p))

#define DEQUEUE_PDCCH_ORDER_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(pdcchOrderQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_PDCCH_ORDER_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(pdcchOrderQueue_gp[internalCellIndex], (node_p))


/*********** End of pdcchOrderQueue_g *******************/


/***********************************************************************
* QueueName              : pdcchOrderTimerTriggerQueue_g                               
* Structure of Queue Node: PdcchOrderTimerTriggerQueueNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define PDCCH_ORDER_TIMER_TRIGGER_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_PDCCH_ORDER_TIMER_TRIGGER_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_PDCCH_ORDER_TIMER_TRIGGER_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_PDCCH_ORDER_TIMER_TRIGGER_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(PDCCH_ORDER_TIMER_TRIGGER_TYPE), numCells);\
        do{                                          \
            GET_MEM_FROM_POOL(PDCCH_ORDER_TIMER_TRIGGER_TYPE, pdcchOrderTimerTriggerQueue_gp[numCells - 1], sizeof(PDCCH_ORDER_TIMER_TRIGGER_TYPE), PNULL); \
                circQInit(pdcchOrderTimerTriggerQueue_gp[numCells - 1],MAX_NODES_SPLIT); \
          }while(--numCells);\
        } 

#define COUNT_PDCCH_ORDER_TIMER_TRIGGER_Q(internalCellIndex) \
          circQueueCount(pdcchOrderTimerTriggerQueue_gp[internalCellIndex])

#define ENQUEUE_PDCCH_ORDER_TIMER_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(pdcchOrderTimerTriggerQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_PDCCH_ORDER_TIMER_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(pdcchOrderTimerTriggerQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_PDCCH_ORDER_TIMER_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(pdcchOrderTimerTriggerQueue_gp[internalCellIndex], (node_p))

/*Coverity-5.3.0 CID-54600*/
#define QDEINIT_PDCCH_ORDER_TIMER_TRIGGER_Q(internalCellIndex) \
        circQDeInit(pdcchOrderTimerTriggerQueue_gp[internalCellIndex])
/*Coverity-5.3.0 CID-54600*/
/*CA Changes end  */

/*********** End of pdcchOrderTimerTriggerQueue_g *******************/

/***********************************************************************
* QueueName              : pdcchRachInterfaceQueue_g                               
* Structure of Queue Node: RachInterfaceQueueNode                                  
* Queue Type             : Circular Queue -  "Multiple Producers (different cores) - both intra/inter-core"          
***********************************************************************/

#define PDCCH_RACH_INTERFACE_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_PDCCH_RACH_INTERFACE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

/* Cyclomatic_complexity_changes_start */
#define FREE_MEM_NODE_PDCCH_RACH_INTERFACE_Q(node_p) \
          {\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }
/* Cyclomatic_complexity_changes_end */

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_PDCCH_RACH_INTERFACE_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(PDCCH_RACH_INTERFACE_TYPE), numCells);\
        do{                                          \
              GET_MEM_FROM_POOL(PDCCH_RACH_INTERFACE_TYPE, pdcchRachInterfaceQueue_gp[numCells - 1], sizeof(PDCCH_RACH_INTERFACE_TYPE), PNULL); \
              circQInit(pdcchRachInterfaceQueue_gp[numCells - 1],MAX_NODES_SPLIT); \
          }while(--numCells); \
        }

#define COUNT_PDCCH_RACH_INTERFACE_Q(internalCellIndex) \
          circQueueCount(pdcchRachInterfaceQueue_gp[internalCellIndex])

#define ENQUEUE_PDCCH_RACH_INTERFACE_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(pdcchRachInterfaceQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_PDCCH_RACH_INTERFACE_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(pdcchRachInterfaceQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_PDCCH_RACH_INTERFACE_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(pdcchRachInterfaceQueue_gp[internalCellIndex], (node_p))
/*Coverity-5.3.0 CID-54600*/
#define QDEINIT_PDCCH_RACH_INTERFACE_Q(internalCellIndex) \
        circQDeInit(pdcchRachInterfaceQueue_gp[internalCellIndex])
/*Coverity-5.3.0 CID-54600*/
/*CA Changes end  */


/*********** End of pdcchRachInterfaceQueue_g *******************/


/***********************************************************************
* QueueName              : pendingDLMemoryPoolFreeQ_g                               
* Structure of Queue Node: DLHARQTBInfo                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q (Inter-Thread)          
***********************************************************************/

#define PENDING_DL_MEMORY_POOL_FREE_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_PENDING_DL_MEMORY_POOL_FREE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_PENDING_DL_MEMORY_POOL_FREE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_PENDING_DL_MEMORY_POOL_FREE_Q(internalCellIndex) \
          createMemPool(sizeof(PENDING_DL_MEMORY_POOL_FREE_TYPE) , 1);\
          GET_MEM_FROM_POOL(LTE_CIRCQUEUE,pendingDLMemoryPoolFreeQ_gp[internalCellIndex] \
                  ,sizeof(LTE_CIRCQUEUE),PNULL) \
          circQInit(pendingDLMemoryPoolFreeQ_gp[internalCellIndex],MAX_NODES_SPLIT)

#define COUNT_PENDING_DL_MEMORY_POOL_FREE_Q(internalCellIndex) \
          circQueueCount(pendingDLMemoryPoolFreeQ_gp[internalCellIndex])

#define ENQUEUE_PENDING_DL_MEMORY_POOL_FREE_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(pendingDLMemoryPoolFreeQ_gp[internalCellIndex], (node_p))

#define DEQUEUE_PENDING_DL_MEMORY_POOL_FREE_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(pendingDLMemoryPoolFreeQ_gp[internalCellIndex], (node_p))

#define GET_NODE_PENDING_DL_MEMORY_POOL_FREE_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(pendingDLMemoryPoolFreeQ_gp[internalCellIndex], (node_p))


/*********** End of pendingDLMemoryPoolFreeQ_g *******************/


/***********************************************************************
* QueueName              : pendingDRXQueue                               
* Structure of Queue Node: DRXQueueNode                                  
* Queue Type             : Circular Queue -  Intra EL (Intra-Core CIRC Q)          
***********************************************************************/

#define DRX_Q_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_DRX_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

/* Cyclomatic_complexity_changes_start */
#define FREE_MEM_NODE_DRX_Q(node_p) \
          {\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }
/* Cyclomatic_complexity_changes_end */

#define INIT_DRX_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_DRX_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_DRX_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_DRX_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_DRX_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))
#define  QDEINIT_NODE_DRX_Q(qName_p)\
            circQDeInit(qName_p)



/*********** End of pendingDRXQueue *******************/


/***********************************************************************
* QueueName              : pendingTAQueue                               
* Structure of Queue Node: TimeAdvanceQueueNode                                  
* Queue Type             : Circular Queue -  Intra EL (Intra-Core CIRC Q)          
***********************************************************************/

#define TA_Q_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_TA_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_TA_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_TA_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_TA_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_TA_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_TA_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_TA_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))
#define  QDEINIT_NODE_TA_Q(qName_p)\
            circQDeInit(qName_p)

/*********** End of pendingTAQueue *******************/


/***********************************************************************
* QueueName              : puschSignalCircQueue_g                               
* Structure of Queue Node: PuschSignalNodeP                                  
* Queue Type             : Circular Queue -  ICC Queue (Inter-Thread)          
***********************************************************************/

#define PUSCH_SIGNAL_CIRC_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_PUSCH_SIGNAL_CIRC_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_PUSCH_SIGNAL_CIRC_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_PUSCH_SIGNAL_CIRC_Q(internalCellIndex) \
        createMemPool(sizeof(PUSCH_SIGNAL_CIRC_TYPE), 1);\
        do{                                          \
            GET_MEM_FROM_POOL(PUSCH_SIGNAL_CIRC_TYPE, puschSignalCircQueue_gp[internalCellIndex], sizeof(PUSCH_SIGNAL_CIRC_TYPE), PNULL); \
            circQInit(puschSignalCircQueue_gp[internalCellIndex], MAX_NODES_SPLIT); \
          }while(0);

#define COUNT_PUSCH_SIGNAL_CIRC_Q(internalCellIndex) \
          circQueueCount(puschSignalCircQueue_gp[internalCellIndex])

#define ENQUEUE_PUSCH_SIGNAL_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(puschSignalCircQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_PUSCH_SIGNAL_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(puschSignalCircQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_PUSCH_SIGNAL_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(puschSignalCircQueue_gp[internalCellIndex], (node_p))
/*CA Changes end  */


/*********** End of puschSignalCircQueue_g *******************/


/***********************************************************************
* QueueName              : rachDLStrategyInterfaceQueue_g                               
* Structure of Queue Node: RachDLStrategyQueueNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define RACH_DL_STRATEGY_INTERFACE_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_RACH_DL_STRATEGY_INTERFACE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_RACH_DL_STRATEGY_INTERFACE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_RACH_DL_STRATEGY_INTERFACE_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(RACH_DL_STRATEGY_INTERFACE_TYPE), numCells);\
        do{                                          \
            GET_MEM_FROM_POOL(RACH_DL_STRATEGY_INTERFACE_TYPE, \
            rachDLStrategyInterfaceQueue_gp[numCells - 1], sizeof(RACH_DL_STRATEGY_INTERFACE_TYPE), PNULL); \
            circQInit(rachDLStrategyInterfaceQueue_gp[numCells - 1],MAX_NODES_SPLIT); \
          }while(--numCells); \
        }

#define COUNT_RACH_DL_STRATEGY_INTERFACE_Q(internalCellIndex) \
          circQueueCount(rachDLStrategyInterfaceQueue_gp[internalCellIndex])

#define ENQUEUE_RACH_DL_STRATEGY_INTERFACE_Q(nodeStruct, node_p,internalCellIndex) \
      pushNodeCircQ(rachDLStrategyInterfaceQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_RACH_DL_STRATEGY_INTERFACE_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(rachDLStrategyInterfaceQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_RACH_DL_STRATEGY_INTERFACE_Q(nodeStruct, node_p,internalCellIndex) \
        getNodeCircQ(rachDLStrategyInterfaceQueue_gp[internalCellIndex], (node_p))
/*Coverity-5.3.0 CID-54613*/
#define QDEINIT_RACH_DL_STRATEGY_INTERFACE_Q(internalCellIndex) \
        circQDeInit(rachDLStrategyInterfaceQueue_gp[internalCellIndex])
/*Coverity-5.3.0 CID-54613*/
/*CA Changes end  */


/*********** End of rachDLStrategyInterfaceQueue_g *******************/


/***********************************************************************
* QueueName              : rcvdOAMKPIMsg_g                               
* Structure of Queue Node: MACGetKpiStatReq                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q (Inter-Thread)          
***********************************************************************/

#define RCVD_OAM_KPI_MSG_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_RCVD_OAM_KPI_MSG_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_RCVD_OAM_KPI_MSG_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/** CA-TDD Changes Start **/
#define INIT_RCVD_OAM_KPI_MSG_Q() {\
         UInt8 internalCellIndex = 0;\
         for( ;internalCellIndex < MAX_NUM_CELL;internalCellIndex++){\
          circQInit(&rcvdOAMKPIMsg_g[internalCellIndex],MAX_NODES_SPLIT);\
        }\
}
/** CA-TDD Changes Stop**/

/** CA-TDD Changes Start **/
#define COUNT_RCVD_OAM_KPI_MSG_Q(internalCellIndex) \
          circQueueCount(&(rcvdOAMKPIMsg_g[internalCellIndex]))
/** CA-TDD Changes Stop**/

/** CA-TDD Changes Start **/
#define ENQUEUE_RCVD_OAM_KPI_MSG_Q(nodeStruct, node_p,internalCellIndex) \
      pushNodeCircQ(&(rcvdOAMKPIMsg_g[internalCellIndex]), (node_p))
/** CA-TDD Changes Stop**/

/** CA-TDD Changes Start **/
#define DEQUEUE_RCVD_OAM_KPI_MSG_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(&rcvdOAMKPIMsg_g[internalCellIndex], (node_p))
/** CA-TDD Changes Stop**/

/** CA-TDD Changes Start **/
#define GET_NODE_RCVD_OAM_KPI_MSG_Q(nodeStruct, node_p,internalCellIndex) \
        getNodeCircQ(&rcvdOAMKPIMsg_g[internalCellIndex], (node_p))
/** CA-TDD Changes Stop**/


/*********** End of rcvdOAMKPIMsg_g *******************/


/***********************************************************************
* QueueName              : rrcOamHPtoLPThdHPrioQ_g                               
* Structure of Queue Node: RrcOamHiPrioQNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q (Inter-Thread)          
***********************************************************************/

#define RRC_OAM_HPTOLP_THDH_PRIO_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

/* Cyclomatic_complexity_changes_start */ 
#define FREE_MEM_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(node_p) \
        {\
              freeMemPool(node_p);\
              node_p=PNULL;\
        }
/* Cyclomatic_complexity_changes_end */
#define INIT_RRC_OAM_HPTOLP_THDH_PRIO_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_RRC_OAM_HPTOLP_THDH_PRIO_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_RRC_OAM_HPTOLP_THDH_PRIO_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_RRC_OAM_HPTOLP_THDH_PRIO_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))


/*********** End of rrcOamHPtoLPThdHPrioQ_g *******************/


/***********************************************************************
* QueueName              : rrcOamLPtoHPThdHPrioQ_g                               
* Structure of Queue Node: RrcOamHiPrioQNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q (Inter-Thread)          
***********************************************************************/
#define RRC_OAM_LPTOHP_THDH_PRIO_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_RRC_OAM_LPTOHP_THDH_PRIO_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_RRC_OAM_LPTOHP_THDH_PRIO_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_RRC_OAM_LPTOHP_THDH_PRIO_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_RRC_OAM_LPTOHP_THDH_PRIO_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_RRC_OAM_LPTOHP_THDH_PRIO_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_RRC_OAM_LPTOHP_THDH_PRIO_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_RRC_OAM_LPTOHP_THDH_PRIO_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))

/*********** End of rrcOamLPtoHPThdHPrioQ_g *******************/


/***********************************************************************
* QueueName              : sinrReportQueue_g                               
* Structure of Queue Node: SINRReportNode                                  
* Queue Type             : Circular Queue -  Intra-Core CIRC Q          
***********************************************************************/

#define SINR_REPORT_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_SINR_REPORT_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_SINR_REPORT_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
/* Construct and initialize the sinrReportQueue_g queues */
#ifdef DL_UL_SPLIT
#define CONSTRUCT_AND_INIT_SINR_REPORT_Q(numCells) \
    UInt8 cellCount = 0;\
    createMemPool(sizeof(SINR_REPORT_TYPE) * SINR_CONTAINER_SIZE, numCells);\
    for (cellCount = 0; cellCount < numCells; cellCount++)\
    {\
        GET_MEM_FROM_POOL(SINR_REPORT_TYPE, sinrReportQueue_gp[cellCount], sizeof(SINR_REPORT_TYPE) * SINR_CONTAINER_SIZE, PNULL);\
        for(containerCount = 0; containerCount < SINR_CONTAINER_SIZE; containerCount++)\
        {\
            circQInit(sinrReportQueue_gp[cellCount] + ((currentTTI - containerCount) % SINR_CONTAINER_SIZE), MAX_NODES_SPLIT);\
        }\
    }
#else
#define CONSTRUCT_AND_INIT_SINR_REPORT_Q(numCells) \
    UInt8 cellCount = 0;\
    createMemPool(sizeof(SINR_REPORT_TYPE), numCells);\
    for (cellCount = 0; cellCount < numCells; cellCount++)\
    {\
        GET_MEM_FROM_POOL(SINR_REPORT_TYPE, sinrReportQueue_gp[cellCount], sizeof(SINR_REPORT_TYPE), PNULL);\
        circQInit(sinrReportQueue_gp[cellCount], MAX_NODES_SPLIT);\
    }
#endif
/*CA Changes end  */

#define COUNT_SINR_REPORT_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_SINR_REPORT_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_SINR_REPORT_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_SINR_REPORT_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))

/*Coverity-5.3.0 CID-54613*/
#define QDEINIT_SINR_REPORT_Q(qName_p) \
        circQDeInit(qName_p)
/*Coverity-5.3.0 CID-54613*/

/*********** End of sinrReportQueue_g *******************/


/***********************************************************************
* QueueName              : tpcTriggerQueueForPusch_g                               
* Structure of Queue Node: TpcTriggerQueueNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define TPC_TRIGGER_QUEUE_FOR_PUSCH_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start */
#define INIT_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(internalCellIndex) \
        do{                                          \
            circQInit(tpcTriggerQueueForPusch_gp[internalCellIndex],MAX_TPC_TRIGGER_NODES_SPLIT);\
          }while(0);

#define COUNT_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(internalCellIndex) \
          circQueueCount(tpcTriggerQueueForPusch_gp[internalCellIndex])

#define ENQUEUE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(tpcTriggerQueueForPusch_gp[internalCellIndex], (node_p))

#define DEQUEUE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(tpcTriggerQueueForPusch_gp[internalCellIndex], (node_p))

#define GET_NODE_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(tpcTriggerQueueForPusch_gp[internalCellIndex], (node_p))
/*CA Changes end */
/* Rel 5.3.1: Memory related changes start*/
#define QDEINIT_TPC_TRIGGER_QUEUE_FOR_PUSCH_Q(internalCellIndex)\
        circQDeInit(tpcTriggerQueueForPusch_gp[internalCellIndex])
/* Rel 5.3.1: Memory related changes end*/

/*********** End of tpcTriggerQueueForPusch_g *******************/


/***********************************************************************
* QueueName              : ulDRXOnOffTriggerQueue_g                               
* Structure of Queue Node: DRXOnOffTriggerNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define UL_DRX_ON_OFF_TRIGGER_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UL_DRX_ON_OFF_TRIGGER_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);
/*CA Changes start  */
#define CONSTRUCT_AND_INIT_UL_DRX_ON_OFF_TRIGGER_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(UL_DRX_ON_OFF_TRIGGER_TYPE), numCells);\
        do{                                          \
            GET_MEM_FROM_POOL(UL_DRX_ON_OFF_TRIGGER_TYPE, ulDRXOnOffTriggerQueue_gp[numCells - 1], sizeof(UL_DRX_ON_OFF_TRIGGER_TYPE), PNULL); \
            circQInit(ulDRXOnOffTriggerQueue_gp[numCells - 1], MAX_NODES_SPLIT); \
          }while(--numCells);\
        }
#define COUNT_UL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex) \
          circQueueCount(ulDRXOnOffTriggerQueue_gp[internalCellIndex])

#define ENQUEUE_UL_DRX_ON_OFF_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(ulDRXOnOffTriggerQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_UL_DRX_ON_OFF_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(ulDRXOnOffTriggerQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_UL_DRX_ON_OFF_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(ulDRXOnOffTriggerQueue_gp[internalCellIndex], (node_p))

/*Coverity-5.3.0 CID-54634*/
#define QDEINIT_UL_DRX_ON_OFF_TRIGGER_Q(internalCellIndex) \
        circQDeInit(ulDRXOnOffTriggerQueue_gp[internalCellIndex])
/*CA Changes end  */
/*Coverity-5.3.0 CID-54634*/
/*********** End of ulDRXOnOffTriggerQueue_g *******************/


/***********************************************************************
* QueueName              : ulGrantAperiodicCqiQueue_g                               
* Structure of Queue Node: ULGrantRequestQueueNode                                  
* Queue Type             : Circular Queue -  "Multiple Producers (different cores) - both intra/inter-core"          
***********************************************************************/

#define UL_GRANT_APERIODIC_CQI_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UL_GRANT_APERIODIC_CQI_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define CONSTRUCT_AND_INIT_UL_GRANT_APERIODIC_CQI_Q(numOfCellConfigured){ \
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(UL_GRANT_APERIODIC_CQI_TYPE), numCells);\
        do{                                          \
            GET_MEM_FROM_POOL(UL_GRANT_APERIODIC_CQI_TYPE, ulGrantAperiodicCqiQueue_gp[numCells - 1], sizeof(UL_GRANT_APERIODIC_CQI_TYPE), PNULL); \
            circQInit(ulGrantAperiodicCqiQueue_gp[numCells - 1], MAX_NODES_SPLIT); \
          }while(--numCells);\
        }

#define COUNT_UL_GRANT_APERIODIC_CQI_Q(internalCellIndex) \
          circQueueCount(ulGrantAperiodicCqiQueue_gp[internalCellIndex])

#define ENQUEUE_UL_GRANT_APERIODIC_CQI_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(ulGrantAperiodicCqiQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_UL_GRANT_APERIODIC_CQI_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(ulGrantAperiodicCqiQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_UL_GRANT_APERIODIC_CQI_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(ulGrantAperiodicCqiQueue_gp[internalCellIndex], (node_p))

/*Coverity-5.3.0 CID-54635*/
#define QDEINIT_UL_GRANT_APERIODIC_CQI_Q(internalCellIndex) \
        circQDeInit(ulGrantAperiodicCqiQueue_gp[internalCellIndex])
/*Coverity-5.3.0 CID-54635*/
/*********** End of ulGrantAperiodicCqiQueue_g *******************/


/***********************************************************************
* QueueName              : ulMeasGapOnOffTriggerQueue_g                               
* Structure of Queue Node: MeasGapOnOffTriggerNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define UL_MEAS_GAPON_OFF_TRIGGER_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);
/*CA Changes start  */
#define CONSTRUCT_AND_INIT_UL_MEAS_GAPON_OFF_TRIGGER_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(UL_MEAS_GAPON_OFF_TRIGGER_TYPE), numCells);\
        do{                                         \
            GET_MEM_FROM_POOL(UL_MEAS_GAPON_OFF_TRIGGER_TYPE, ulMeasGapOnOffTriggerQueue_gp[numCells - 1], sizeof(UL_MEAS_GAPON_OFF_TRIGGER_TYPE), PNULL); \
            circQInit(ulMeasGapOnOffTriggerQueue_gp[numCells - 1], MAX_NODES_SPLIT); \
          }while(--numCells);\
        }

#define COUNT_UL_MEAS_GAPON_OFF_TRIGGER_Q(internalCellIndex) \
          circQueueCount(ulMeasGapOnOffTriggerQueue_gp[internalCellIndex])

#define ENQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(ulMeasGapOnOffTriggerQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_UL_MEAS_GAPON_OFF_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(ulMeasGapOnOffTriggerQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_UL_MEAS_GAPON_OFF_TRIGGER_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(ulMeasGapOnOffTriggerQueue_gp[internalCellIndex], (node_p))

/*Coverity-5.3.0 CID-54635*/
#define QDEINIT_UL_MEAS_GAPON_OFF_TRIGGER_Q(internalCellIndex) \
        circQDeInit(ulMeasGapOnOffTriggerQueue_gp[internalCellIndex])
/*CA Changes end  */
/*Coverity-5.3.0 CID-54635*/
/*********** End of ulMeasGapOnOffTriggerQueue_g *******************/


/***********************************************************************
* QueueName              : ulStrategyTxCircQueue_g                               
* Structure of Queue Node: ULStrategyTxNode                                  
* Queue Type             : Circular Queue -  "CIRC/ICC Q depending on the Compile time flag CIRC_QUEUE_IMPL"          
***********************************************************************/

#define UL_STRATEGY_TX_CIRC_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UL_STRATEGY_TX_CIRC_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_UL_STRATEGY_TX_CIRC_Q(internalCellIndex) \
        createMemPool(sizeof(UL_STRATEGY_TX_CIRC_TYPE), 1);\
        do{                                         \
            GET_MEM_FROM_POOL(UL_STRATEGY_TX_CIRC_TYPE, ulStrategyTxCircQueue_gp[internalCellIndex], sizeof(UL_STRATEGY_TX_CIRC_TYPE), PNULL); \
            circQInit(ulStrategyTxCircQueue_gp[internalCellIndex], MAX_NODES_SPLIT); \
          }while(0);

#define COUNT_UL_STRATEGY_TX_CIRC_Q(internalCellIndex) \
          circQueueCount(ulStrategyTxCircQueue_gp[internalCellIndex])

#define ENQUEUE_UL_STRATEGY_TX_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(ulStrategyTxCircQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_UL_STRATEGY_TX_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(ulStrategyTxCircQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_UL_STRATEGY_TX_CIRC_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(ulStrategyTxCircQueue_gp[internalCellIndex], (node_p))

/*Coverity-5.3.0 CID-54651*/
#define QDEINIT_UL_STRATEGY_TX_CIRC_Q(internalCellIndex) \
        circQDeInit(ulStrategyTxCircQueue_gp[internalCellIndex])
/*CA Changes end  */
/*Coverity-5.3.0 CID-54651*/

/*********** End of ulStrategyTxCircQueue_g *******************/


/***********************************************************************
* QueueName              : ulSyncLossTriggerQDL_g                               
* Structure of Queue Node: ULSyncLossQueueNode                                  
* Queue Type             : Circular Queue -  "Multiple Producers (different cores) - both intra/inter-core"          
***********************************************************************/

#define UL_SYNC_LOSS_TRIGGERQ_DL_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);
/*CA Changes start  */
#define CONSTRUCT_AND_INIT_UL_SYNC_LOSS_TRIGGERQ_DL_Q(internalCellIndex) \
        do{                                          \
            circQInit(ulSyncLossTriggerQDL_gp[internalCellIndex], MAX_NODES_SPLIT); \
          }while(0); 

#define COUNT_UL_SYNC_LOSS_TRIGGERQ_DL_Q(internalCellIndex) \
          circQueueCount(ulSyncLossTriggerQDL_gp[internalCellIndex])

#define ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(nodeStruct, node_p,internalCellIndex) \
      pushNodeCircQ(ulSyncLossTriggerQDL_gp[internalCellIndex], (node_p))

#define DEQUEUE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(ulSyncLossTriggerQDL_gp[internalCellIndex], (node_p))

#define GET_NODE_UL_SYNC_LOSS_TRIGGERQ_DL_Q(nodeStruct, node_p,internalCellIndex) \
        getNodeCircQ(ulSyncLossTriggerQDL_gp[internalCellIndex], (node_p))
/*CA Changes end  */


/*********** End of ulSyncLossTriggerQDL_g *******************/


/***********************************************************************
* QueueName              : ulSyncLossTriggerQUL_g                               
* Structure of Queue Node: ULSyncLossQueueNode                                  
* Queue Type             : Circular Queue -  "Multiple Producers (different cores) - both intra/inter-core"          
***********************************************************************/

#define UL_SYNC_LOSS_TRIGGERQ_UL_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_UL_SYNC_LOSS_TRIGGERQ_UL_Q(internalCellIndex) \
            circQInit(ulSyncLossTriggerQUL_gp[internalCellIndex], MAX_NODES_SPLIT);

#define COUNT_UL_SYNC_LOSS_TRIGGERQ_UL_Q(internalCellIndex) \
          circQueueCount(ulSyncLossTriggerQUL_gp[internalCellIndex])

#define ENQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(nodeStruct, node_p, internalCellIndex) \
      pushNodeCircQ(ulSyncLossTriggerQUL_gp[internalCellIndex], (node_p))

#define DEQUEUE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(ulSyncLossTriggerQUL_gp[internalCellIndex], (node_p))

#define GET_NODE_UL_SYNC_LOSS_TRIGGERQ_UL_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(ulSyncLossTriggerQUL_gp[internalCellIndex], (node_p))
/*CA Changes end  */


/*********** End of ulSyncLossTriggerQUL_g *******************/

/***********************************************************************
* QueueName              : ulUEReconfigDataQueue_g                               
* Structure of Queue Node: UELocationNode                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define UL_UE_RECONFIG_DATA_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_UL_UE_RECONFIG_DATA_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UL_UE_RECONFIG_DATA_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_UL_UE_RECONFIG_DATA_Q(internalCellIndex) \
        circQInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigDataQueue),MAX_NODES_SPLIT)

#define COUNT_UL_UE_RECONFIG_DATA_Q(internalCellIndex) \
        circQueueCount(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigDataQueue))

#define ENQUEUE_UL_UE_RECONFIG_DATA_Q(nodeStruct,node_p,internalCellIndex) \
        pushNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigDataQueue), (node_p))

#define DEQUEUE_UL_UE_RECONFIG_DATA_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigDataQueue), (node_p))

#define GET_NODE_UL_UE_RECONFIG_DATA_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigDataQueue), (node_p))

/*coverity changes CID-54604 */

/*********** End of ulUEReconfigDataQueue_g *******************/
/***********************************************************************
* QueueName              : ulUEReconfigMUMIMOQueue                               
* Structure of Queue Node: UEULMUMIMOInfoNode                                 
* Queue Type             : Circular Queue -  Inter-Core CIRC Q          
***********************************************************************/

#define GET_MEM_NODE_UL_UE_MUMIMO_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UL_UE_MUMIMO_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_UL_UE_MUMIMO_Q(internalCellIndex) \
        circQInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigMUMIMOQueue),MAX_NODES_SPLIT)

#define COUNT_UL_UE_MUMIMO_Q(internalCellIndex) \
        circQueueCount(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigMUMIMOQueue))

#define ENQUEUE_UL_UE_MUMIMO_Q(nodeStruct, node_p,internalCellIndex) \
        pushNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigMUMIMOQueue), (node_p))

#define DEQUEUE_UL_UE_MUMIMO_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigMUMIMOQueue), (node_p))

#define GET_NODE_UL_UE_MUMIMO_Q(nodeStruct, node_p,internalCellIndex) \
        getNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ulUEReconfigMUMIMOQueue), (node_p))


/*********** End of ulUEReconfigMUMIMOQueue *******************/
/***********************************************************************
* QueueName              : ueReconfigNumOfLayerQueue
* Structure of Queue Node: UENumOfLayerNode 
* Queue Type             : Circular Queue -  Inter-Core CIRC Q
***********************************************************************/

#define GET_MEM_NODE_UE_NUM_OF_LAYER_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UE_NUM_OF_LAYER_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_UE_NUM_OF_LAYER_Q(internalCellIndex) \
        circQInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigNumOfLayerQueue),MAX_NODES_SPLIT)

#define COUNT_UE_NUM_OF_LAYER_Q(internalCellIndex) \
        circQueueCount(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigNumOfLayerQueue))

#define ENQUEUE_UE_NUM_OF_LAYER_Q(nodeStruct, node_p) \
        pushNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigNumOfLayerQueue), (node_p))

#define DEQUEUE_UE_NUM_OF_LAYER_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigNumOfLayerQueue), (node_p))

#define GET_NODE_UE_NUM_OF_LAYER_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigNumOfLayerQueue), (node_p))


/*********** End of ueReconfigNumOfLayerQueue *******************/
/***********************************************************************
* QueueName              : ueReconfigBeamFormingAlgoQueue
* Structure of Queue Node: UEBeamformingAlgoNode
* Queue Type             : Circular Queue -  Inter-Core CIRC Q
***********************************************************************/

#define GET_MEM_NODE_UE_BEAM_FORMING_ALGO_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UE_BEAM_FORMING_ALGO_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_UE_BEAM_FORMING_ALGO_Q(internalCellIndex) \
        circQInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigBeamFormingAlgoQueue),MAX_NODES_SPLIT)

#define COUNT_UE_BEAM_FORMING_ALGO_Q(internalCellIndex) \
        circQueueCount(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigBeamFormingAlgoQueue))

#define ENQUEUE_UE_BEAM_FORMING_ALGO_Q(nodeStruct, node_p, internalCellIndex) \
        pushNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigBeamFormingAlgoQueue), (node_p))

#define DEQUEUE_UE_BEAM_FORMING_ALGO_Q(nodeStruct, node_p, internalCellIndex) \
        popNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigBeamFormingAlgoQueue), (node_p))

#define GET_NODE_UE_BEAM_FORMING_ALGO_Q(nodeStruct, node_p, internalCellIndex) \
        getNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigBeamFormingAlgoQueue), (node_p))


/*********** End of ueReconfigBeamFormingAlgoQueue *******************/
/*coverity changes CID-54604 */
/*ca changes start*/
/***********************************************************************
* QueueName              : ueReconfigScellSchedulingQueue
* Structure of Queue Node: UEScellSchedulingInfoNode
* Queue Type             : Circular Queue -  Inter-Core CIRC Q
***********************************************************************/

#define GET_MEM_NODE_UE_RECONFIG_SCELL_SCHEDULING_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UE_RECONFIG_SCELL_SCHEDULING_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_UE_RECONFIG_SCELL_SCHEDULING_Q(internalCellIndex) \
          circQInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
          ueReconfigScellSchedulingQueue),MAX_NODES_SPLIT)

#define COUNT_UE_RECONFIG_SCELL_SCHEDULING_Q(internalCellIndex) \
        circQueueCount(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigScellSchedulingQueue))

#define ENQUEUE_UE_RECONFIG_SCELL_SCHEDULING_Q(nodeStruct, node_p, \
                                                    internalCellIndex) \
      pushNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
      ueReconfigScellSchedulingQueue), (node_p))

#define DEQUEUE_UE_RECONFIG_SCELL_SCHEDULING_Q(nodeStruct, node_p, \
                                                    internalCellIndex) \
        popNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigScellSchedulingQueue), (node_p))

#define GET_NODE_UE_RECONFIG_SCELL_SCHEDULING_Q(nodeStruct, node_p, \
                                                    internalCellIndex) \
        getNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigScellSchedulingQueue), (node_p))
#define QDEINIT_NODE_UE_RECONFIG_SCELL_SCHEDULING_Q(internalCellIndex)\
        circQDeInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigScellSchedulingQueue))


/*********** End of ueReconfigScellSchedulingQueue *******************/

/***********************************************************************
* QueueName              : ueReconfigCqiValidityTimerQueue
* Structure of Queue Node: UECqiValidityTimerInfoNode
* Queue Type             : Circular Queue -  Inter-Core CIRC Q
***********************************************************************/

#define GET_MEM_NODE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(internalCellIndex) \
          circQInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
          ueReconfigCqiValidityTimerQueue), MAX_NODES_SPLIT)

#define COUNT_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(internalCellIndex) \
        circQueueCount(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigCqiValidityTimerQueue))

#define ENQUEUE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(nodeStruct, node_p, \
                                                    internalCellIndex) \
        pushNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigCqiValidityTimerQueue), (node_p))

#define DEQUEUE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(nodeStruct, node_p, \
                                                    internalCellIndex) \
        popNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigCqiValidityTimerQueue), (node_p))

#define GET_NODE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(nodeStruct, node_p, \
                                                    internalCellIndex) \
        getNodeCircQ(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigCqiValidityTimerQueue), (node_p))
#define QDEINIT_NODE_UE_RECONFIG_CQI_VALIDITY_TIMER_Q(internalCellIndex)\
        circQDeInit(&(rrmMacCellGlobal_gp[internalCellIndex]-> \
        ueReconfigCqiValidityTimerQueue))


/*********** End of ueReconfigCqiValidityTimerQueue*******************/
/*ca changes end*/
/***********************************************************************
* QueueName              : contentionResolutionMsgQ_g                               
* Structure of Queue Node: ContentionQueueNode                            
* Queue Type             : SQueue -  Inter-Core SQ          
***********************************************************************/

#define CONTENTION_RESOLUTION_MSG_Q   LTE_SQUEUE

#define GET_MEM_NODE_CONTENTION_RESOLUTION_SQ(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_CONTENTION_RESOLUTION_SQ(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_CONTENTION_RESOLUTION_SQ(numOfCellConfigured){ \
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(CONTENTION_RESOLUTION_MSG_Q), numCells);\
        do{                                          \
            GET_MEM_FROM_POOL(CONTENTION_RESOLUTION_MSG_Q, contentionResolutionMsgQ_gp[numCells - 1], sizeof(CONTENTION_RESOLUTION_MSG_Q), PNULL); \
            sQueueInit(contentionResolutionMsgQ_gp[numCells - 1]); \
          }while(--numCells); \
        }        
 
#define COUNT_CONTENTION_RESOLUTION_SQ(internalCellIndex) \
         sQueueCount(contentionResolutionMsgQ_gp[internalCellIndex]) 

#define ENQUEUE_CONTENTION_RESOLUTION_SQ(nodeStruct, node_p, internalCellIndex) \
        pushNode(contentionResolutionMsgQ_gp[internalCellIndex], node_p)

#define DEQUEUE_CONTENTION_RESOLUTION_SQ(nodeStruct, node_p, internalCellIndex) \
        node_p = (nodeStruct *)popNode(contentionResolutionMsgQ_gp[internalCellIndex])

#define GET_NODE_CONTENTION_RESOLUTION_SQ(nodeStruct, node_p, internalCellIndex) \
        node_p = (nodeStruct *) getFirstNode(contentionResolutionMsgQ_gp[internalCellIndex])
/*CA Changes end  */


/*********** End of contentionResolutionMsgQ_g *******************/

/***********************************************************************
* QueueName              : tpcTriggeredQueuePucch_g
* Structure of Queue Node: TpcTriggerQueueNode                       
* Queue Type             : SQueue -  Inter-Core SQ          
***********************************************************************/

#define TPC_TRIGGERED_QUEUE_PUCCH_Q   LTE_SQUEUE

#define GET_MEM_NODE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_TPC_TRIGGERED_QUEUE_PUCCH_SQ(internalCellIndex) \
       do{                                          \
            GET_MEM_FROM_POOL(TPC_TRIGGERED_QUEUE_PUCCH_Q, tpcTriggeredQueuePucch_gp[internalCellIndex], sizeof(TPC_TRIGGERED_QUEUE_PUCCH_Q), PNULL); \
            sQueueInit(tpcTriggeredQueuePucch_gp[internalCellIndex]);\
        }while(0);

#define COUNT_TPC_TRIGGERED_QUEUE_PUCCH_SQ(internalCellIndex) \
         sQueueCount(tpcTriggeredQueuePucch_gp[internalCellIndex]) 

#define ENQUEUE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(nodeStruct, node_p, internalCellIndex) \
        pushNode(tpcTriggeredQueuePucch_gp[internalCellIndex], node_p)

#define DEQUEUE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(nodeStruct, node_p, internalCellIndex) \
        node_p = (nodeStruct *)popNode(tpcTriggeredQueuePucch_gp[internalCellIndex])

#define GET_NODE_TPC_TRIGGERED_QUEUE_PUCCH_SQ(nodeStruct, node_p,internalCellIndex) \
        node_p = (nodeStruct *) getFirstNode(tpcTriggeredQueuePucch_gp[internalCellIndex])


/*********** End of tpcTriggeredQueuePucch_g *******************/

/***********************************************************************
* QueueName              : tpcSchedFailQueuePucch_g
* Structure of Queue Node: TpcTriggerQueueNode
* Queue Type             : SQueue -  Inter-Core SQ          
***********************************************************************/

#define TPC_SCHED_FAIL_QUEUE_PUCCH_Q   LTE_SQUEUE

#define GET_MEM_NODE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(internalCellIndex) \
          do{                                          \
               GET_MEM_FROM_POOL(TPC_SCHED_FAIL_QUEUE_PUCCH_Q, tpcSchedFailQueuePucch_gp[internalCellIndex], sizeof(TPC_SCHED_FAIL_QUEUE_PUCCH_Q), PNULL); \
               sQueueInit(tpcSchedFailQueuePucch_gp[internalCellIndex]);\
            }while(0);

#define COUNT_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(internalCellIndex) \
         sQueueCount(tpcSchedFailQueuePucch_gp[internalCellIndex]) 

#define ENQUEUE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(nodeStruct, node_p, internalCellIndex) \
        pushNode(tpcSchedFailQueuePucch_gp[internalCellIndex], node_p)

#define DEQUEUE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(nodeStruct, node_p, internalCellIndex) \
        node_p = (nodeStruct *)popNode(tpcSchedFailQueuePucch_gp[internalCellIndex])

#define GET_NODE_TPC_SCHED_FAIL_QUEUE_PUCCH_SQ(nodeStruct, node_p, internalCellIndex) \
        node_p = (nodeStruct *) getFirstNode(tpcSchedFailQueuePucch_gp[internalCellIndex])


/*********** End of tpcSchedFailQueuePucch_g  *******************/

/***********************************************************************
 * QueueName              : tpcSchedFailQueuePusch_g
 * Structure of Queue Node: TpcTriggerQueueNode
 * Queue Type             : Circular Queue -  Inter-Core CQ
 ***********************************************************************/
/*CA Changes start */
#ifdef DL_UL_SPLIT
#define TPC_SCHED_FAIL_QUEUE_PUSCH_TYPE LTE_CIRCQUEUE
#define INIT_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(internalCellIndex) \
          do{                                          \
              /* SPR 13088 Fix Start */\
              if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))\
              {\
               circQInit(tpcSchedFailQueuePusch_gp[internalCellIndex],MAX_NODES_SPLIT);\
              }\
              else\
              {\
               circQInitAfterCellSetup(tpcSchedFailQueuePusch_gp[internalCellIndex],MAX_NODES_SPLIT);\
              }\
              /* SPR 13088 Fix End */\
            }while(0);
#else
#define TPC_SCHED_FAIL_QUEUE_PUSCH_TYPE LTE_SQUEUE
#define INIT_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(internalCellIndex) \
          do{                                          \
               sQueueInit(tpcSchedFailQueuePusch_gp[internalCellIndex]);\
            }while(0);
#endif

#define GET_MEM_NODE_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(node_p, nodeStruct) \
    GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);


#define COUNT_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(internalCellIndex) \
    circQueueCount(tpcSchedFailQueuePusch_gp[internalCellIndex])

#define ENQUEUE_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(nodeStruct, node_p, internalCellIndex) \
    pushNodeCircQ(tpcSchedFailQueuePusch_gp[internalCellIndex], (node_p))

#define DEQUEUE_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(nodeStruct, node_p, internalCellIndex) \
    popNodeCircQ(tpcSchedFailQueuePusch_gp[internalCellIndex], (node_p))

#define GET_NODE_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(nodeStruct, node_p, internalCellIndex) \
    getNodeCircQ(tpcSchedFailQueuePusch_gp[internalCellIndex], (node_p))
/*CA Changes end */
/* Memory cleanup changes start*/
#define QDEINIT_TPC_SCHED_FAIL_QUEUE_PUSCH_Q(internalCellIndex)\
        circQDeInit(tpcSchedFailQueuePusch_gp[internalCellIndex])
/* Memory cleanup changes end*/


/*********** End of tpcSchedFailQueuePusch_g *******************/

/***********************************************************************
* QueueName              : rrcOamPendingQ_g                               
* Structure of Queue Node: RrcOamLPPendingNode                                  
* Queue Type             : Circular Queue 
***********************************************************************/
#define RRC_OAM_PENDING_QUEUE_TYPE   LTE_CIRCQUEUE

#define GET_MEM_NODE_RRC_OAM_LP_PENDING_NODE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_RRC_OAM_LP_PENDING_NODE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_RRC_OAM_LP_PENDING_NODE_Q(qName_p) \
          circQInit(qName_p,MAX_NODES_SPLIT)

#define COUNT_RRC_OAM_LP_PENDING_NODE_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_RRC_OAM_LP_PENDING_NODE_Q(qName_p, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_RRC_OAM_LP_PENDING_NODE_Q(qName_p, node_p) \
        popNodeCircQ(qName_p, (void *)&node_p )
/*********** End of rrcOamPendingQ_g *******************/
/***********************************************************************
 * QueueName              : dlSchedulerReportQueue_g
 * Structure of Queue Node: DlSchedulerReportQueueNode
 * Queue Type             : Circular Queue -  Inter-Core CQ
 ***********************************************************************/
#define DL_SCHEDULER_REPORT_QUEUE_TYPE LTE_CIRCQUEUE

#define GET_MEM_NODE_DL_SCHEDULER_REPORT_QUEUE_Q(node_p, nodeStruct) \
    GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DL_SCHEDULER_REPORT_QUEUE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

    /* SPR 99999 CQI with 255 UE Fix start */

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_DL_SCHEDULER_REPORT_QUEUE_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(LTE_CIRCQUEUE), numCells);   \
        do{                                     \
            GET_MEM_FROM_POOL(LTE_CIRCQUEUE, dlSchedulerReportQueue_gp[numCells - 1], sizeof(LTE_CIRCQUEUE), PNULL); \
            circQInit(dlSchedulerReportQueue_gp[numCells -1],MAX_CQI_NODES_SPLIT); \
          }while(--numCells);\
        } 

    /* SPR 99999 CQI with 255 UE Fix end */

#define COUNT_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex) \
    circQueueCount(dlSchedulerReportQueue_gp[internalCellIndex])

#define ENQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(nodeStruct, node_p, internalCellIndex) \
    pushNodeCircQ(dlSchedulerReportQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_DL_SCHEDULER_REPORT_QUEUE_Q(nodeStruct, node_p, internalCellIndex) \
    popNodeCircQ(dlSchedulerReportQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_DL_SCHEDULER_REPORT_QUEUE_Q(nodeStruct, node_p, internalCellIndex) \
    getNodeCircQ(dlSchedulerReportQueue_gp[internalCellIndex], (node_p))
#define  QDEINIT_NODE_DL_SCHEDULER_REPORT_QUEUE_Q(internalCellIndex)\
    circQDeInit(dlSchedulerReportQueue_gp[internalCellIndex])
/*CA Changes end  */

/*********** End of dlSchedulerReportQueue_g *******************/

/***********************************************************************
 * QueueName              : ulGrantPUSCHQueue_g
 * Structure of Queue Node: UlGrantPUSCHQueueNode
 * Queue Type             : Circular Queue -  Inter-Core CQ
 ***********************************************************************/
#define UL_GRANT_PUSCH_QUEUE_TYPE LTE_CIRCQUEUE

#define GET_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(node_p, nodeStruct) \
    GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_UL_GRANT_PUSCH_QUEUE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_UL_GRANT_PUSCH_PENDING_QUEUE_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(UL_GRANT_PUSCH_QUEUE_TYPE), numCells);\
        do{                                          \
              GET_MEM_FROM_POOL(UL_GRANT_PUSCH_QUEUE_TYPE, ulGrantPendingQueue_gp[numCells - 1], sizeof(UL_GRANT_PUSCH_QUEUE_TYPE), PNULL); \
              circQInit(ulGrantPendingQueue_gp[numCells - 1], MAX_NODES_SPLIT); \
         }while(--numCells);\
        }

#define CONSTRUCT_AND_INIT_UL_GRANT_PUSCH_FAILURE_QUEUE_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(UL_GRANT_PUSCH_QUEUE_TYPE), numCells);\
        do{                                          \
              GET_MEM_FROM_POOL(UL_GRANT_PUSCH_QUEUE_TYPE, ulGrantPUSCHFailureQueue_gp[numCells - 1], sizeof(UL_GRANT_PUSCH_QUEUE_TYPE), PNULL); \
              circQInit(ulGrantPUSCHFailureQueue_gp[numCells - 1], MAX_NODES_SPLIT); \
         }while(--numCells);\
       }
/*CA Changes end  */

#define COUNT_UL_GRANT_PUSCH_QUEUE_Q(qName_p) \
    circQueueCount(qName_p)

#define ENQUEUE_UL_GRANT_PUSCH_QUEUE_Q(qName_p,nodeStruct, node_p) \
    pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_UL_GRANT_PUSCH_QUEUE_Q(qName_p,nodeStruct, node_p) \
    popNodeCircQ(qName_p, (node_p))

#define GET_NODE_UL_GRANT_PUSCH_QUEUE_Q(qName_p,nodeStruct, node_p) \
    getNodeCircQ(qName_p, (node_p))

/*Coverity-5.3.0 CID-54636*/
#define QDEINIT_UL_GRANT_PUSCH_QUEUE_Q(qName_p) \
        circQDeInit(qName_p)
/*Coverity-5.3.0 CID-54636*/
/*********** End of ulGrantPUSCHFailureQueue_g *******************/

/***********************************************************************
 * QueueName              : dlRARspQueue_g
 * Structure of Queue Node: DlRARspQueueNode
 * Queue Type             : Circular Queue -  Inter-Core CQ
 ***********************************************************************/
#define DL_RA_RSP_QUEUE_TYPE LTE_CIRCQUEUE

#define GET_MEM_NODE_DL_RA_RSP_QUEUE_Q(node_p, nodeStruct) \
    GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_DL_RA_RSP_QUEUE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_DL_RA_RSP_QUEUE_Q(numOfCellConfigured){ \
        UInt8 numCells = numOfCellConfigured;\
    createMemPool(sizeof(DL_RA_RSP_QUEUE_TYPE), numCells);\
    do{                                          \
          GET_MEM_FROM_POOL(DL_RA_RSP_QUEUE_TYPE, dlRARspQueue_gp[numCells - 1], sizeof(DL_RA_RSP_QUEUE_TYPE), PNULL); \
          circQInit(dlRARspQueue_gp[numCells - 1],MAX_NODES_SPLIT); \
    }while(--numCells);\
   }

#define COUNT_DL_RA_RSP_QUEUE_Q(internalCellIndex) \
    circQueueCount(dlRARspQueue_gp[internalCellIndex])

#define ENQUEUE_DL_RA_RSP_QUEUE_Q(nodeStruct, node_p, internalCellIndex) \
    pushNodeCircQ(dlRARspQueue_gp[internalCellIndex], (node_p))

#define DEQUEUE_DL_RA_RSP_QUEUE_Q(nodeStruct, node_p, internalCellIndex) \
    popNodeCircQ(dlRARspQueue_gp[internalCellIndex], (node_p))

#define GET_NODE_DL_RA_RSP_QUEUE_Q(nodeStruct, node_p, internalCellIndex) \
    getNodeCircQ(dlRARspQueue_gp[internalCellIndex], (node_p))

/*Coverity-5.3.0 CID-54599*/
#define QDEINIT_DL_RA_RSP_QUEUE_Q(internalCellIndex) \
    circQDeInit(dlRARspQueue_gp[internalCellIndex])
/*Coverity-5.3.0 CID-54599*/
/*CA Changes end  */

/*********** End of dlRARspQueue_g *************************/
#ifdef FDD_CONFIG
/** TTIB_Code Start */
/***********************************************************************
 * QueueName              : nackQueueForTtiBundledUe
 * Structure of Queue Node: ContainerNackQueueNode
 * Queue Type             : Single Queue -  Inter-Core CQ
 ***********************************************************************/
#define NACK_QUEUE_FOR_TTIB_UE LTE_CIRCQUEUE

#define GET_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

#define INIT_NACK_QUEUE_FOR_TTIB_UE_Q(qName_p) \
          circQInit(qName_p,MAX_UL_UE_SCHEDULED)

#define COUNT_NACK_QUEUE_FOR_TTIB_UE_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_NACK_QUEUE_FOR_TTIB_UE_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_NACK_QUEUE_FOR_TTIB_UE_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_NACK_QUEUE_FOR_TTIB_UE_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))





/*********** End of nackQueueForTtiBundledUe *******************/

/* This node would be enqueued to the queue which will keep 
 *    record of the TTI Bundle ongoing */
/***********************************************************************
 * QueueName              : ttibScheduleQueue_g
 * Structure of Queue Node: TtiBScheduleNode
 * Queue Type             : Circular Queue
 ***********************************************************************/
#define TTIB_SCHEDULE_TYPE        LTE_CIRCQUEUE

//Coverity_36221 fixed
#define GET_MEM_NODE_TTIB_SCHEDULE_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)\
         if(PNULL == node_p)\
              ltePanic("getMemFromPool fails %s", __func__);

#define FREE_MEM_NODE_TTIB_SCHEDULE_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);
//CODE_COMMENT_L11_FIXED
#define CONSTRUCT_AND_INIT_TTIB_SCHEDULE_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        UInt8  qIdx = 0; \
        createMemPool(sizeof(TTIB_SCHEDULE_TYPE) * MAX_TTIB_SCHEDULE_QUEUE, numCells);\
        do{                                      \
            GET_MEM_FROM_POOL(TTIB_SCHEDULE_TYPE, ttibScheduleQueue_gp[numCells - 1], sizeof(TTIB_SCHEDULE_TYPE) * MAX_TTIB_SCHEDULE_QUEUE, PNULL); \
            for (qIdx = 0; qIdx < MAX_TTIB_SCHEDULE_QUEUE; qIdx++)  \
            {  \
                circQInit((ttibScheduleQueue_gp[numCells - 1] + qIdx), MAX_NODES_SPLIT); \
            } \
          }while(--numCells);\
       }

#define COUNT_TTIB_SCHEDULE_Q(qName_p) \
          circQueueCount(qName_p)

#define ENQUEUE_TTIB_SCHEDULE_Q(qName_p,nodeStruct, node_p) \
      pushNodeCircQ(qName_p, (node_p))

#define DEQUEUE_TTIB_SCHEDULE_Q(qName_p,nodeStruct, node_p) \
        popNodeCircQ(qName_p, (node_p))

#define GET_NODE_TTIB_SCHEDULE_Q(qName_p,nodeStruct, node_p) \
        getNodeCircQ(qName_p, (node_p))


/*********** End of ttibScheduleQueue_g *******************/


/** TTIB_Code End */

#endif
/* + E_CID_5.3 */
/***********************************************************************
* QueueName              : eCidReportsQueue_g                               
* Structure of Queue Node: EcidReportNode                             
* Queue Type             : Circular Queue -  Inter-Core CircQ          
***********************************************************************/
#define E_CID_REPORT_SEND_Q   LTE_CIRCQUEUE

#define GET_MEM_NODE_E_CID_REPORT_Q(node_p, nodeStruct) \
         GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_E_CID_REPORT_Q(node_p) \
        do{\
              freeMemPool(node_p);\
              node_p=PNULL;\
          }while(0);

/*CA Changes start  */
#define CONSTRUCT_AND_INIT_E_CID_REPORT_Q(numOfCellConfigured) {\
        UInt8 numCells = numOfCellConfigured;\
        createMemPool(sizeof(E_CID_REPORT_SEND_Q), numCells);\
        do{                                         \
            GET_MEM_FROM_POOL(E_CID_REPORT_SEND_Q, eCidReportsQueue_gp[numCells - 1], sizeof(E_CID_REPORT_SEND_Q), PNULL); \
            circQInit(eCidReportsQueue_gp[numCells - 1], MAX_NODES_SPLIT); \
          }while(--numCells);\
        } 
#define COUNT_E_CID_REPORT_Q(internalCellIndex) \
          circQueueCount(eCidReportsQueue_gp[internalCellIndex]) 

#define ENQUEUE_E_CID_REPORT_Q(nodeStruct, node_p, internalCellIndex) \
        pushNodeCircQ(eCidReportsQueue_gp[internalCellIndex], node_p)

#define DEQUEUE_E_CID_REPORT_Q(nodeStruct, node_p, internalCellIndex) \
         popNodeCircQ(eCidReportsQueue_gp[internalCellIndex], (node_p))
/*CA Changes end  */

/*********** End of eCidReportsQueue_g  *******************/

/* - E_CID_5.3 */

/* + SPR 9911 Fix */
#ifndef UE_SIM_TESTING 
/***********************************************************************
* QueueName              : rlcTriggerStatusReportQueue_g                               
* Structure of Queue Node: RlcTriggerStatusReportNode                             
* Queue Type             : Circular Queue -  Inter-Core CircQ          
***********************************************************************/

#define RLC_TRIGGER_STATUS_REPORT_Q   LTE_CIRCQUEUE

#define GET_MEM_NODE_RLC_TRIGGER_STATUS_REPORT_Q(node_p, nodeStruct) \
        node_p=(nodeStruct *)getMemFromPool(sizeof(nodeStruct),PNULL);\
        if(PNULL == node_p)\
        {\
            LOG_MAC_MSG( L2_GET_MEMFROMPOOL_FAIL_ID,LOGFATAL,L2_SYS_FAIL,\
                globalTTITickCount_g, __LINE__, sizeof(nodeStruct), 0,0,0,\
                0.0,0.0,__FILE__,__func__);\
            ALARM_MSG (MAC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID, CRITICAL_ALARM);\
         }\

#define FREE_MEM_NODE_RLC_TRIGGER_STATUS_REPORT_Q(node_p) \
         freeMemPool(node_p);\
         node_p=PNULL;

#define INIT_RLC_TRIGGER_STATUS_REPORT_Q(internalCellIndex) \
           circQInit(&(rlcTriggerStatusReportQueue_g[internalCellIndex]), MAX_NODES_SPLIT)

#define COUNT_RLC_TRIGGER_STATUS_REPORT_Q(internalCellIndex) \
          circQueueCount(&rlcTriggerStatusReportQueue_g[internalCellIndex]) 

#define ENQUEUE_RLC_TRIGGER_STATUS_REPORT_Q(internalCellIndex,node_p) \
        pushNodeCircQ(&rlcTriggerStatusReportQueue_g[internalCellIndex], node_p)

#define DEQUEUE_RLC_TRIGGER_STATUS_REPORT_Q(internalCellIndex,node_p) \
         popNodeCircQ(&rlcTriggerStatusReportQueue_g[internalCellIndex], (node_p))

#define QDEINIT_RLC_TRIGGER_STATUS_REPORT_Q(internalCellIndex){\
        RlcTriggerStatusReportNode *rlcTriggerStatusReportNode_p = PNULL;\
        DEQUEUE_RLC_TRIGGER_STATUS_REPORT_Q(internalCellIndex,(void **)&rlcTriggerStatusReportNode_p);\
        while(rlcTriggerStatusReportNode_p != RLC_NULL){\
           freeMemPool(rlcTriggerStatusReportNode_p);\
           rlcTriggerStatusReportNode_p = RLC_NULL;\
           DEQUEUE_RLC_TRIGGER_STATUS_REPORT_Q(internalCellIndex,(void **)&rlcTriggerStatusReportNode_p);}\
        circQDeInit(&rlcTriggerStatusReportQueue_g[internalCellIndex]);\
}
#endif
/*********** End of rlcTriggerStatusReportQueue_g *******************/

/*ca-tdd pucch code changes*/
/***********************************************************************
* QueueName              : dlSecondayCellScheulingQueue_g                               
* Structure of Queue Node: SCELL_SCHEDULING_CIRC_QUEUE                                  
* Queue Type             : Circular Queue -  "CIRC/ICC Q depending on the Compile time flag CIRC_QUEUE_IMPL"          
***********************************************************************/

#define SCELL_SCHEDULING_CIRC_Q   LTE_CIRCQUEUE

#define GET_NODE_SCELL_SCHEDULING_CIRC_Q(node_p, internalCellIndex) \
    getNodeCircQ(&(dlSecondayCellScheulingQueue_g[internalCellIndex]), (node_p))

#define GET_MEM_NODE_SCELL_SCHEDULING_CIRC_Q(node_p, nodeStruct) \
    GET_MEM_FROM_POOL(nodeStruct, node_p, sizeof(nodeStruct), PNULL)

#define FREE_MEM_NODE_SCELL_SCHEDULING_Q(node_p) \
    do{\
        freeMemPool(node_p);\
        node_p=PNULL;\
    }while(0);

/*CA Changes start  */
#if (defined (DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)) 
    #define SCELL_SCHEDULING_QUEUE_INIT(internalCellIndex) \
    /* SPR 13088 Fix Start */\
    if (!(cellSpecificParams_g.cellConfigAndInitParams_p[internalCellIndex]->isCellConfiguredOnce))\
    {\
        circQInit(&(dlSecondayCellScheulingQueue_g[internalCellIndex]), MAX_UE_SUPPORTED); \
    }\
    else\
    {\
        circQInitAfterCellSetup(&(dlSecondayCellScheulingQueue_g[internalCellIndex]), MAX_UE_SUPPORTED);\
    }\
    /* SPR 13088 Fix End */
#else 
    #define SCELL_SCHEDULING_QUEUE_INIT(internalCellIndex) \
        sQueueInit(&(dlSecondayCellScheulingQueue_g[internalCellIndex])); 
#endif

#if (defined (DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)) 
    #define SCELL_SCHEDULING_QUEUE_COUNT(internalCellIndex) \
        circQueueCount(&(dlSecondayCellScheulingQueue_g[internalCellIndex]))
#else 
    #define SCELL_SCHEDULING_QUEUE_COUNT(internalCellIndex) \
        sQueueCount(&(dlSecondayCellScheulingQueue_g[internalCellIndex]))
#endif

#if (defined (DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)) 
    #define ENQUEUE_SCELL_SCHEDULING_NODE(node_p,internalCellIndex) \
        pushNodeCircQ(&(dlSecondayCellScheulingQueue_g[internalCellIndex]), node_p)
#else 
    #define ENQUEUE_SCELL_SCHEDULING_NODE(node_p,internalCellIndex) \
        !pushNode(&(dlSecondayCellScheulingQueue_g[internalCellIndex]),&((node_p)->ackNackAnchor))
#endif

#if (defined (DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)) 
    #define DEQUEUE_SCELL_SCHEDULING_NODE(node_p, internalCellIndex) \
        popNodeCircQ(&(dlSecondayCellScheulingQueue_g[internalCellIndex]),(void **) &(node_p))
#else 
    #define DEQUEUE_SCELL_SCHEDULING_NODE(node_p, internalCellIndex) {\
        do{\
            LTE_SNODE *sN = popNode(&(dlSecondayCellScheulingQueue_g[internalCellIndex]));\
            node_p = MEMBEROF(SCellSchedulingQueueNode,ackNackAnchor,sN);\
        }while(0);\
    }
#endif

#if (defined (DL_UL_SPLIT) && defined(CIRC_QUEUE_IMPL)) 
    #define SCELL_SCHEDULING_QUEUE_DEINIT(internalCellIndex) \
        circQDeInit(&(dlSecondayCellScheulingQueue_g[internalCellIndex]));
#else 
    #define SCELL_SCHEDULING_QUEUE_DEINIT(internalCellIndex) {\
        do {\
            LTE_SNODE * sN = PNULL;\
            while((sN = popNode(&(dlSecondayCellScheulingQueue_g[internalCellIndex]))) != PNULL) {\
                freeMemPool(sN);\
            } \
        }while(0);\
    }
#endif

/* - SPR 9911 Fix */



#if defined(UE_SIM_TESTING) && defined(LTE_EMBMS_SUPPORTED)
/***********************************************************************
* QueueName              : macEmbmsDataIndQ_g                               
* Structure of Queue Node: MacUeDataInd                                  
* Queue Type             : Circular Queue -  Inter-Core CIRC Q (Inter-Thread)          
***********************************************************************/
#define INIT_MAC_EMBMS_DATA_IND_Q(internalCellIndex) \
          circQInit(&(macEmbmsDataIndQ_g[internalCellIndex]),MAX_NODES_SPLIT)

#define COUNT_MAC_EMBMS_DATA_IND_Q(internalCellIndex) \
          circQueueCount(&(macEmbmsDataIndQ_g[internalCellIndex]))

#define ENQUEUE_MAC_EMBMS_DATA_IND_Q(nodeStruct, node_p,internalCellIndex) \
      pushNodeCircQ(&(macEmbmsDataIndQ_g[internalCellIndex]), (node_p))

#define DEQUEUE_MAC_EMBMS_DATA_IND_Q(nodeStruct, node_p,internalCellIndex) \
        popNodeCircQ(&(macEmbmsDataIndQ_g[internalCellIndex]), (node_p))

#define GET_NODE_MAC_EMBMS_DATA_IND_Q(nodeStruct, node_p,internalCellIndex) \
        getNodeCircQ(&(macEmbmsDataIndQ_g[internalCellIndex]), (node_p))

#endif
#endif /* LTE_COMMON_QUEUE_DEF_H */
