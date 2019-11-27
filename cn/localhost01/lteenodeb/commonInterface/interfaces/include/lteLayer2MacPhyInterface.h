/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2008 Aricent Inc . All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: lteLayer2MacPhyInterface.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 ****************************************************************************/

#ifndef MACPHYINTERFACE_H
#define MACPHYINTERFACE_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
/****************************************************************************
 * Exported Types
 ****************************************************************************/
/*typedefs for data transfer between MAC and RLC */

/****************************************************************************
 * Exported Functions
 ****************************************************************************/
/* PHY Layer has to implement these indications */
typedef enum PHYTagIndentifierT
{

    DCI_0,        
    DCI_1,                
    DCI_1A,               
    DCI_1B,              
    DCI_1C,               
    DCI_1D,               
    DCI_2,                 
    DCI_2A,
    DCI_3,                 
    DCI_3A,
    MAC_DCI_PDU_INFO,
    MAC_HI_PDU_INFO,              
    MAC_TB_INFO,                  
    MAC_BCH_PDU_INFO,             
    MAC_DLSCH_PDU_INFO,           
    MAC_PCH_PDU_INFO,             
    SR_HARQ_PDU_INFO,             
    CRC_HARQ_PDU_INFO,            
    CRC_PDU_INFO,                 
    SR_PDU_INFO,                  
    HARQ_PDU_INFO,                
    HARQ_TB_PDU_INFO,             
    RACH_PDU_INFO,                
    ULSCH_PDU,                    
    ULSCH_CQI_RI_PDU,             
    UCI_CQI_PDU,                      
    TIMING_ADVANCE_PDU_INFO,      
    DL_ERROR_INFO,                
    UL_ERROR_INFO,
    APERIODIC_CQI_MODE_2_0,
    APERIODIC_CQI_MODE_3_0,
    APERIODIC_CQI_MODE_3_1,
    APERIODIC_CQI_MODE_1_2,
    APERIODIC_CQI_MODE_2_2,
    PERIODIC_CQI_MODE_1_0_TYPE_3,
    PERIODIC_CQI_MODE_1_0_TYPE_4,
    PERIODIC_CQI_MODE_1_1_TYPE_2,
    PERIODIC_CQI_MODE_1_1_TYPE_3,
    PERIODIC_CQI_MODE_2_0_TYPE_1,
    PERIODIC_CQI_MODE_2_0_TYPE_3,
    PERIODIC_CQI_MODE_2_0_TYPE_4,
    PERIODIC_CQI_MODE_2_1_TYPE_1,
    PERIODIC_CQI_MODE_2_1_TYPE_2,
    PERIODIC_CQI_MODE_2_1_TYPE_3

}PHYTagIndentifier;
typedef struct ControlNodeInfoT {
               UInt8  sendSF;
               UInt8  qIndex;
               UInt8  twoTbFlag;
               UInt8  tb1harqInfo;
               UInt8  tb2harqInfo;
               UInt8  crc;
               UInt16 sendSFN;
               UInt16 pduChoice;
               UInt16 crnti;
}ControlNodeInfo;
void prepareSRPdu(UInt16 crnti );
/* SPR 15909 fix start */
/* + SPR 17439 */    
tickType_t getCurrentglobaltick(void);
/* - SPR 17439 */    
/* SPR 15909 fix end */
void prepareDataPduNode(UInt8 *pdu_p, UInt16 pduLen, UInt16 crnti, UInt8 sendSF, UInt16 sendSFN, UInt8 qIndex,UInt16 pduChoice, UInt8 cqiMode);
UInt8 readAperiodicCqiInfoFromFile(const UChar8 *fileName);
UInt8 readPeriodicCqiInfoFromFile(const UChar8 *fileName);
void initAllResourceAllocationPolicyInfo(InternalCellIndex internalCellIndex );

/* MAC Layer has to implement these requests */



#endif  /* INCLUDED_MACINTERFACE_H */
