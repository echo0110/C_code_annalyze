/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: lteMacUtilityAndSharedStructs.c,v 1.1.1.1.6.1 2010/08/02 08:13:42 gur20491 Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file defines the different functionalities i.e logint
                       calculating logint etc.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: lteMacUtilityAndSharedStructs.c,v $
 * Revision 1.1.1.1.6.1  2010/08/02 08:13:42  gur20491
 * FrameworkSetupWith1.0_MAC1.2.4
 *
 * Revision 1.3  2009/08/03 09:41:00  gur20052
 * Fix for UT_LOGS
 *
 * Revision 1.2  2009/07/20 08:41:27  gur20052
 * changed the signature of logint
 *
 * Revision 1.1  2009/06/02 08:13:15  gur20052
 * added the initial version
 *
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/
#include "lteMacUtilityAndSharedStructs.h" 


/******************************************************************************
 * Project Includes
 *****************************************************************************/

/******************************************************************************
 Private Definitions
 *****************************************************************************/

/******************************************************************************
 Private Types
 *****************************************************************************/
/* + Critical section changes */
CellELSyncTrackerState cellElSyncTracker[MAX_NUM_CELL];
/* - Critical section changes */
/* SPR 15909 fix start */
/* + SPR 17439 */
 extern tickType_t getCurrentTick(void);
 /* - SPR 17439 */
/* SPR 15909 fix end */


// log table for 1-1000 multiplied by factor of 1024
static const Int logtbl[] = {
    0,  709, 1124, 1419, 1648, 1834, 1992, 2129, 2249, 2357,
    2455, 2544, 2626, 2702, 2773, 2839, 2901, 2959, 3015, 3067,
    3117, 3165, 3210, 3254, 3296, 3336, 3374, 3412, 3448, 3482,
    3516, 3548, 3580, 3610, 3640, 3669, 3697, 3724, 3751, 3777,
    3802, 3827, 3851, 3875, 3898, 3920, 3942, 3964, 3985, 4005,
    4026, 4046, 4065, 4084, 4103, 4121, 4140, 4157, 4175, 4192,
    4209, 4226, 4242, 4258, 4274, 4290, 4305, 4320, 4335, 4350,
    4364, 4379, 4393, 4407, 4421, 4434, 4448, 4461, 4474, 4487,
    4499, 4512, 4524, 4537, 4549, 4561, 4573, 4584, 4596, 4607,
    4619, 4630, 4641, 4652, 4663, 4673, 4684, 4695, 4705, 4715,
    4725, 4735, 4745, 4755, 4765, 4775, 4784, 4794, 4803, 4813,
    4822, 4831, 4840, 4849, 4858, 4867, 4876, 4885, 4893, 4902,
    4910, 4919, 4927, 4935, 4944, 4952, 4960, 4968, 4976, 4984,
    4992, 4999, 5007, 5015, 5023, 5030, 5038, 5045, 5052, 5060,
    5067, 5074, 5081, 5089, 5096, 5103, 5110, 5117, 5124, 5130,
    5137, 5144, 5151, 5157, 5164, 5171, 5177, 5184, 5190, 5196,
    5203, 5209, 5216, 5222, 5228, 5234, 5240, 5246, 5253, 5259,
    5265, 5271, 5276, 5282, 5288, 5294, 5300, 5306, 5311, 5317,
    5323, 5328, 5334, 5340, 5345, 5351, 5356, 5362, 5367, 5372,
    5378, 5383, 5388, 5394, 5399, 5404, 5410, 5415, 5420, 5425,
    5430, 5435, 5440, 5445, 5450, 5455, 5460, 5465, 5470, 5475,
    5480, 5485, 5489, 5494, 5499, 5504, 5509, 5513, 5518, 5523,
    5527, 5532, 5536, 5541, 5546, 5550, 5555, 5559, 5564, 5568,
    5573, 5577, 5581, 5586, 5590, 5594, 5599, 5603, 5607, 5612,
    5616, 5620, 5624, 5629, 5633, 5637, 5641, 5645, 5649, 5653,
    5658, 5662, 5666, 5670, 5674, 5678, 5682, 5686, 5690, 5694,
    5698, 5701, 5705, 5709, 5713, 5717, 5721, 5725, 5728, 5732,
    5736, 5740, 5744, 5747, 5751, 5755, 5758, 5762, 5766, 5770,
    5773, 5777, 5780, 5784, 5788, 5791, 5795, 5798, 5802, 5805,
    5809, 5812, 5816, 5819, 5823, 5826, 5830, 5833, 5837, 5840,
    5844, 5847, 5850, 5854, 5857, 5860, 5864, 5867, 5870, 5874,
    5877, 5880, 5884, 5887, 5890, 5893, 5897, 5900, 5903, 5906,
    5909, 5913, 5916, 5919, 5922, 5925, 5928, 5932, 5935, 5938,
    5941, 5944, 5947, 5950, 5953, 5956, 5959, 5962, 5965, 5968,
    5971, 5974, 5977, 5980, 5983, 5986, 5989, 5992, 5995, 5998,
    6001, 6004, 6007, 6010, 6013, 6015, 6018, 6021, 6024, 6027,
    6030, 6033, 6035, 6038, 6041, 6044, 6047, 6049, 6052, 6055,
    6058, 6060, 6063, 6066, 6069, 6071, 6074, 6077, 6080, 6082,
    6085, 6088, 6090, 6093, 6096, 6098, 6101, 6104, 6106, 6109,
    6111, 6114, 6117, 6119, 6122, 6124, 6127, 6130, 6132, 6135,
    6137, 6140, 6142, 6145, 6147, 6150, 6153, 6155, 6158, 6160,
    6163, 6165, 6168, 6170, 6172, 6175, 6177, 6180, 6182, 6185,
    6187, 6190, 6192, 6194, 6197, 6199, 6202, 6204, 6206, 6209,
    6211, 6214, 6216, 6218, 6221, 6223, 6225, 6228, 6230, 6232,
    6235, 6237, 6239, 6242, 6244, 6246, 6249, 6251, 6253, 6255,
    6258, 6260, 6262, 6264, 6267, 6269, 6271, 6273, 6276, 6278,
    6280, 6282, 6285, 6287, 6289, 6291, 6293, 6296, 6298, 6300,
    6302, 6304, 6306, 6309, 6311, 6313, 6315, 6317, 6319, 6321,
    6324, 6326, 6328, 6330, 6332, 6334, 6336, 6338, 6340, 6343,
    6345, 6347, 6349, 6351, 6353, 6355, 6357, 6359, 6361, 6363,
    6365, 6367, 6369, 6371, 6373, 6375, 6377, 6380, 6382, 6384,
    6386, 6388, 6390, 6392, 6394, 6396, 6397, 6399, 6401, 6403,
    6405, 6407, 6409, 6411, 6413, 6415, 6417, 6419, 6421, 6423,
    6425, 6427, 6429, 6431, 6433, 6434, 6436, 6438, 6440, 6442,
    6444, 6446, 6448, 6450, 6452, 6453, 6455, 6457, 6459, 6461,
    6463, 6465, 6466, 6468, 6470, 6472, 6474, 6476, 6477, 6479,
    6481, 6483, 6485, 6487, 6488, 6490, 6492, 6494, 6496, 6497,
    6499, 6501, 6503, 6505, 6506, 6508, 6510, 6512, 6513, 6515,
    6517, 6519, 6521, 6522, 6524, 6526, 6528, 6529, 6531, 6533,
    6534, 6536, 6538, 6540, 6541, 6543, 6545, 6547, 6548, 6550,
    6552, 6553, 6555, 6557, 6558, 6560, 6562, 6564, 6565, 6567,
    6569, 6570, 6572, 6574, 6575, 6577, 6579, 6580, 6582, 6584,
    6585, 6587, 6588, 6590, 6592, 6593, 6595, 6597, 6598, 6600,
    6602, 6603, 6605, 6606, 6608, 6610, 6611, 6613, 6614, 6616,
    6618, 6619, 6621, 6622, 6624, 6626, 6627, 6629, 6630, 6632,
    6633, 6635, 6637, 6638, 6640, 6641, 6643, 6644, 6646, 6648,
    6649, 6651, 6652, 6654, 6655, 6657, 6658, 6660, 6661, 6663,
    6664, 6666, 6668, 6669, 6671, 6672, 6674, 6675, 6677, 6678,
    6680, 6681, 6683, 6684, 6686, 6687, 6689, 6690, 6692, 6693,
    6695, 6696, 6698, 6699, 6700, 6702, 6703, 6705, 6706, 6708,
    6709, 6711, 6712, 6714, 6715, 6717, 6718, 6719, 6721, 6722,
    6724, 6725, 6727, 6728, 6730, 6731, 6732, 6734, 6735, 6737,
    6738, 6739, 6741, 6742, 6744, 6745, 6747, 6748, 6749, 6751,
    6752, 6754, 6755, 6756, 6758, 6759, 6761, 6762, 6763, 6765,
    6766, 6767, 6769, 6770, 6772, 6773, 6774, 6776, 6777, 6778,
    6780, 6781, 6783, 6784, 6785, 6787, 6788, 6789, 6791, 6792,
    6793, 6795, 6796, 6797, 6799, 6800, 6801, 6803, 6804, 6805,
    6807, 6808, 6809, 6811, 6812, 6813, 6815, 6816, 6817, 6819,
    6820, 6821, 6823, 6824, 6825, 6826, 6828, 6829, 6830, 6832,
    6833, 6834, 6836, 6837, 6838, 6839, 6841, 6842, 6843, 6845,
    6846, 6847, 6848, 6850, 6851, 6852, 6853, 6855, 6856, 6857,
    6859, 6860, 6861, 6862, 6864, 6865, 6866, 6867, 6869, 6870,
    6871, 6872, 6874, 6875, 6876, 6877, 6879, 6880, 6881, 6882,
    6883, 6885, 6886, 6887, 6888, 6890, 6891, 6892, 6893, 6895,
    6896, 6897, 6898, 6899, 6901, 6902, 6903, 6904, 6905, 6907,
    6908, 6909, 6910, 6911, 6913, 6914, 6915, 6916, 6917, 6919,
    6920, 6921, 6922, 6923, 6925, 6926, 6927, 6928, 6929, 6930,
    6932, 6933, 6934, 6935, 6936, 6937, 6939, 6940, 6941, 6942,
    6943, 6944, 6946, 6947, 6948, 6949, 6950, 6951, 6953, 6954,
    6955, 6956, 6957, 6958, 6959, 6961, 6962, 6963, 6964, 6965,
    6966, 6967, 6969, 6970, 6971, 6972, 6973, 6974, 6975, 6976,
    6978, 6979, 6980, 6981, 6982, 6983, 6984, 6985, 6987, 6988,
    6989, 6990, 6991, 6992, 6993, 6994, 6995, 6997, 6998, 6999,
    7000, 7001, 7002, 7003, 7004, 7005, 7006, 7007, 7009, 7010,
    7011, 7012, 7013, 7014, 7015, 7016, 7017, 7018, 7019, 7021,
    7022, 7023, 7024, 7025, 7026, 7027, 7028, 7029, 7030, 7031,
    7032, 7033, 7034, 7035, 7037, 7038, 7039, 7040, 7041, 7042,
    7043, 7044, 7045, 7046, 7047, 7048, 7049, 7050, 7051, 7052,
    7053, 7054, 7055, 7057, 7058, 7059, 7060, 7061, 7062, 7063,
    7064, 7065, 7066, 7067, 7068, 7069, 7070, 7071, 7072, 7073,
};



/*****************************************************************************
 * Function Name  : logint
 * Inputs         : val - Value for which the Log need to find out
 * Outputs        : logtbl - Gives the Log value of the input 
 * Returns        : logtbl - This is the Log value of the input
 * Description    : This Function is used to find out the log using the lookup 
 *                  tables.
 *****************************************************************************/
 UInt32 logint(UInt32 val)
{
    if (val <= 0 || val > 1000) 
    {
        LTE_MAC_UT_LOG(LOG_DETAIL,SHARED_STRUCTS,
                "[%s]:error in input val = %d\n", __func__, val);
        return 1024;
    }
    return logtbl[val-1];
}


#ifdef OVERSHOOT_CORRECTION
#ifdef DL_UL_SPLIT
UInt64 ccFirstSF_g = 0;                //cycleCount when first tick is recvd
UInt64 ccDelayInSF_g = 0;              // Delay in receiveing the current SFI
UInt64 ccInMs_g  = 0;              //cycle count in 1 Ms obtained at init time.
UInt64 ccAtTickUpdate_g = 0;          //cycle count on recving tick from phy
UInt8 ueSchdIndxInCurrTick_g = 2;    // dlSchdConfig_g.maxUEsToBeScheduledDL/2; 
__thread UInt64 phyTickSend_g = 0;    //to capture tick send time from phy
__thread UInt64 phyMsgRecv_g = 0; //to capture time at which msg is recv at phy
__thread UInt8 noTimeToProcessflag_g = 0;// Optional leg node procesing flag
OverLoadLevel currTickOverLoadDL_g = 0 ; // over load level at current tick
OverLoadLevel currTickOverLoadUL_g = 0 ; // over load level at current tick
/** CA-TDD Changes Start **/
OverLoadLevel macOverLoadLevel_g[MAX_NUM_CELL] = {OVERLOAD_LEVEL_NORMAL};    // mac overload level
/** CA-TDD Changes Stop **/
OverLoadLevel currTickOverLoad_g[MAX_NUM_CELL] = {OVERLOAD_LEVEL_NORMAL};  /*  over load level at current tick*/

/*
 {PROCESS_DL_HARQ_ACK_NACK_QUEUE,0,0,0}, //0
 {PROCESS_DL_HARQ_TIMER_EXPIRY,1,0,0},   //1
 {PROCESS_DL_SCH_STRATEGY,0,0,0},        //2
 {PROCESS_PDCCH_PDSCH_WITH_MUX,0,0,0},   //3
 {PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA,0,0,0}, //4
 {TX_DL_CONTROL_MSG,0,0,0},              //5
 {TX_DL_DATA_MSG,0,0,0},                 //6
 {PROCESS_DRX_FOR_ALL_UE,0,0,0},         //7
 {PROCESS_MEAS_GAP_FOR_ALL_UE,0,0,0},    //8
 {PROCESS_CQI_REPORTS,0,0,0},            //9
 {PROCESS_DL_OAM_AND_RRC_MSG,0,0,0},     //10
 {PROCESS_DL_PERFORMANCE_STATS_UPDATE,DL_OVERSHOOT_STATS,0,0} //11 
*/
UInt32 reqElProcessingTimeDL[MAX_UES_SCHEDULED_INDX][
    /*SPR 16855 Fix Start*/
    15 /* execution leg count */
    /*SPR 16855 Fix End*/
] = /*{{0}};*/

{   /*0,  1,  2,  3,  4,  5,   6,  7,  8,  9,10,11,12*/
    /*[0UE]*/  { 18, 2, 12, 12, 20,  0, 80, 100, 15, 15, 15, 15 ,5,
    /*SPR 16855 +-*/
        50, //11
    /*SPR 16855 +-*/
        10  //11 or 12
    },
    /*[1UE]*/  { 18, 2, 12, 12, 48, 50, 80, 180, 15, 15, 15, 15 ,5,
    /*SPR 16855 +-*/
        50, //11
    /*SPR 16855 +-*/
        10  //11 or 12
    },
    /*[2UE]*/  { 19, 2, 15, 15, 56, 50, 80, 190, 18, 15, 15, 15, 5,
    /*SPR 16855 +-*/
        50,  //11
    /*SPR 16855 +-*/
        10  //11 or 12
    },
    /*[3UE]*/  { 21, 2, 18, 18, 57, 50,100, 200, 19, 15, 15, 15, 5,
    /*SPR 16855 +-*/
        50,  //11 
    /*SPR 16855 +-*/
        10   //11 or 12
    },
    /*[4UE]*/  { 24, 2, 21, 21 , 63, 60,110, 230, 23, 15, 15, 15, 5,
    /*SPR 16855 +-*/
        50,  //11
    /*SPR 16855 +-*/
        10   //11 or 12
    }    
};

UInt64 reqElProcessingCyclesDL[MAX_UES_SCHEDULED_INDX][
    /*SPR 16855 Fix Start*/
    15 /* execution leg count */
    /*SPR 16855 Fix End*/
] = {{0}};  
/*
   {PROCESS_RX_UNTIL_SUBFRAME_IND,0,0,0}, //0
   {PROCESS_UL_HARQ_TIMER_EXPIRY,1,0,0},  //1
   {PROCESS_UL_SCH_STRATEGY,0,0,0},       //2
   {PROCESS_PUSCH_FOR_DELAY_ONE,0,0,0},   //3
   {PROCESS_SRS_REPORTS,0,0,0},           //4
   {PROCESS_RLC_HANDLE_TX_OPPORTUNITY,0,0,0}, //5
   {PROCESS_DEMUX_QUEUE,0,0,0},           //6
   {PROCESS_UL_CSI_SINR_REPORTS,0,0,0},   //7
   {PROCESS_UL_RA,0,0,0},                 //8
   {PROCESS_RLC_HANDLE_HARQ_FAILURE_IND,0,0,0}, //9
   {PROCESS_RLC_DL_CHECK_TIMER_EXPIRY,0,0,0},   //10
   {PROCESS_RLC_DL_HDLR_REESTABLISH_Q,0,0,0},   //11
   {PROCESS_RLC_DL_HDLR_PEN_DEL_Q,0,0,0},       //12
   {PROCESS_UL_OAM_AND_RRC_MSG,0,0,0},          //13
   {PROCESS_UL_PENDING_DELETE_UE,0,0,0},        //14
   {PROCESS_PENDING_RESET_UE,0,0,0},            //15
   {PROCESS_MISC_UPDATES,0,0,0},                //16
   {SEND_AUDIT_REPORT_FOR_INACTIVE_UE,0,0,0},   //17
   {PROCESS_UL_PERFORMANCE_STATS_UPDATE,UL_OVERSHOOT_STATS,0,0}  //17 or 18
*/ 
UInt32 reqElProcessingTimeUL[MAX_UES_SCHEDULED_INDX][
    /*SPR 16855 Fix Start*/
    20 /* execution leg count */
    /*SPR 16855 Fix End*/
]  = /*{{0}};*/
{      /* 0,  1, 2,  3,  4, 5,  6,  7,  8, 9,10,11,12,13,14,15,16,17,18,19*/
    /*[0UE]*/{ 0, 14,10, 10, 12, 38, 1, 45, 25, 25, 9, 5, 5, 5, 5, 5, 5,
    /*SPR 16855 +-*/
        5, 5, 5},
    /*[1UE]*/{ 0, 14,10, 10, 12, 38, 1, 45, 25,25,  9, 5, 5, 5, 5, 5, 5,
    /*SPR 16855 +-*/
        5, 5, 5},
    /*[2UE]*/{ 0, 14,10,10, 13, 43, 1, 51, 32,32, 12, 5, 5, 5, 5, 5, 5,
    /*SPR 16855 +-*/
        5, 5, 5},
    /*[3UE]*/{ 0, 14,10,10, 15, 45, 1, 58, 37,37, 14, 5, 5, 5, 5, 5, 5,
    /*SPR 16855 +-*/
        5, 5, 5},
    /*[4UE]*/{ 0, 14,10,10, 15, 46, 1, 64, 42,42, 15, 5, 5, 5, 5, 5, 5,
    /*SPR 16855 +-*/
        5, 5, 5}
};
UInt64 reqElProcessingCyclesUL[MAX_UES_SCHEDULED_INDX][
    /*SPR 16855 Fix Start*/
    20 /* execution leg count */
    /*SPR 16855 Fix End*/
] = {{0}};  
#else /*DL_UL_SPLIT*/
__thread UInt64 ccFirstSF_g = 0; //cycleCount when first tick is recvd
__thread UInt64 ccDelayInSF_g = 0; // Delay in receiveing the current SFI
/** CA2 Changes Start **/
UInt64 ccInMs_g  = 0; //cycle count in 1 Ms obtained at init time.
/** CA2 Changes End **/
__thread UInt64 ccAtTickUpdate_g = 0; //cycle count on recving tick from phy
/*+ Dyn UEs To Be SchPerTTI +*/
/** CA-TDD Changes Start **/
__thread UInt8 dlUeSchdIndxInCurrTick_g = 2;
/** CA-TDD Changes Stop **/
__thread UInt8 ulUeSchdIndxInCurrTick_g = 2;
__thread UInt8 reqIndexForUeSchdInDlUl_g = 0;
/*- Dyn UEs To Be SchPerTTI -*/
__thread UInt64 phyTickSend_g = 0; //to capture tick send time from phy
__thread UInt64 phyMsgRecv_g = 0;//to capture time at which msg is recv at phy
__thread UInt8 noTimeToProcessflag_g = 0;//Optional leg node procesing falg
/*+COVERITY 5.2.2 - 32668,32669*/
OverLoadLevel currTickOverLoad_g[MAX_NUM_CELL] = {OVERLOAD_LEVEL_NORMAL};  /*  over load level at current tick*/
/** CA-TDD Changes Start **/
OverLoadLevel macOverLoadLevel_g[MAX_NUM_CELL] = {OVERLOAD_LEVEL_NORMAL}; /*mac overload level */
/** CA-TDD Changes Stop **/
/*-COVERITY 5.2.2 - 32668,32669*/

UInt32 reqElProcessingTime[MAX_UES_SCHEDULED_INDX][MAX_CRITICAL_LEGS]=
/* 
 * INVALID,
 * MAC_DL_HARQ_ACK_NACK_QUEUE   = 1,
 MAC_HARQ_TIMER_EXPIRY,         //2
 MAC_Dl_SPS_STRATEGY,           //3
 MAC_DL_SCH_STRATEGY,           //4
 MAC_Ul_SPS_STRATEGY,           //5
 MAC_UL_SCH_STRATEGY,           //6
 MAC_PRS_TRANSMISSION,          //7
 MAC_PDCCH_PDSCH_WITH_MUX,      //8
 MAC_PUSCH_FOR_DELAY_ONE,       //9
 MAC_SRS_REPORTS,               //10
 MAC_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA,      //11
 MAC_DL_TX_CONTROL_MSG,         //12
 MAC_DL_TX_DATA_MSG,            //13
 MAC_DRX_FOR_ALL_UE,            //14
 MAC_MEAS_GAP_FOR_ALL_UE,       //15
 MAC_DEMUX_QUEUE,               //16
 MAC_UL_RA,                     //17
 MAC_CQI_REPORTS,               //18
 MAC_SPS_PDSCH,                 //19
 MAC_UL_CSI_SINR_REPORTS,       //20
 MAC_SPS_PUSCH,                 //21
 MAC_E_CID_REPORTS,             //22
 MAC_PENDING_DELETE_UE,         //23
 MAC_PENDING_RESET_UE,          //24
 MAC_AUDIT_REPORT_FOR_INACTIVE_UE,  //25
 MAC_MISC_UPDATES,              //26
 RLC_TX_OPP,                    //27
 RLC_TIMER_EXPIRY,              //28
 RLC_HANDLE_RESTAB_Q,           //29
 MAX_CRITICAL_LEGS              //30
 */

/*
   MAC_DL_HARQ_ACK_NACK_QUEUE = 1,
   MAC_HARQ_TIMER_EXPIRY, =2
   MAC_DL_CA_TIMER_EXPIRY, =3   
   MAC_Dl_SPS_STRATEGY, 4
   MAC_SET_STRATEGY_FD, 5    
   MAC_CHECK_STRATEGY_FD, 6  
   MAC_DL_SCH_STRATEGY, 7
   MAC_SET_PDCCH_PDSCH_FD, 8
   MAC_UL_HARQ_TIMER_EXPIRY, 9
   MAC_Ul_SPS_STRATEGY, 10
   MAC_UL_SCH_STRATEGY, 11
   MAC_PRS_TRANSMISSION, 12
   MAC_CHECK_PDCCH_PDSCH_FD,  13
   MAC_PDCCH_PDSCH_WITH_MUX,  14
   MAC_SET_MUX_FD,  15
   MAC_CHECK_MUX_FD,  16
   MAC_MUX_QUEUE,  17
   RLC_TX_OPP,  18
   MAC_PUSCH_FOR_DELAY_ONE, 19
   MAC_SRS_REPORTS, 20
   MAC_SET_ENCODER_FD, 21
   MAC_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA, 22
   MAC_CHECK_ENCODER_FD, 23
   MAC_ENCODER_QUEUE_OF_OTHER_CELLS, 24
   MAC_DL_TX_CONTROL_MSG, 25
   MAC_DL_TX_DATA_MSG, 26
   MAC_DRX_FOR_ALL_UE, 27
   MAC_MEAS_GAP_FOR_ALL_UE, 28
   MAC_DEMUX_QUEUE, 29
   MAC_UL_RA, 30
   MAC_CQI_REPORTS, 31
   MAC_SPS_PDSCH, 32
   MAC_UL_CSI_SINR_REPORTS, 33
   MAC_SPS_PUSCH, 34
   MAC_E_CID_REPORTS, 35
   MAC_UPDATE_DL_TOP_SCH_LIST_SCELL_STATE, 36
   MAC_UL_OAM_AND_RRC_MSG, 37
   MAC_DL_OAM_AND_RRC_MSG, 38
   MAC_UL_PENDING_DELETE_UE, 39
   MAC_PENDING_DELETE_UE, 40
   MAC_PENDING_RESET_UE, 41
   MAC_AUDIT_REPORT_FOR_INACTIVE_UE, 42
   MAC_MISC_UPDATES, 43
   MAX_CRITICAL_LEGS 44
*/


{
//case 0
{14, 8, 2, 5, 12, 5, 5, 12, 5, 5, 12, 12, 7, 5, 48, 5, 5, 5, 40, 30, 1, 5, 11, 5, 5, 18, 81, 12, 5, 10, 5, 10, 6,  5,  2,  1,  5, 1, 1, 1, 1, 2, 1, 5},
//case 1 
{14, 8, 4, 5, 12, 5, 5, 32, 5, 5, 12, 41, 7, 5, 59, 5, 5, 5, 40, 34, 1, 5, 11, 5, 5, 18, 81, 12, 5, 10, 5, 10, 6,  5,  2,  1,  5, 1, 1, 1, 1, 2, 1, 5},
//case 2
{14, 8, 5, 5, 18, 5, 5, 33, 5, 5, 15, 42, 7, 5, 70, 5, 5, 5, 49, 37, 1, 5, 18, 5, 5, 25, 88, 12, 2, 25, 5, 10, 6,  5, 2, 1, 5, 1, 1, 1, 1, 2, 1, 5},
//case 3
{14, 8, 6, 5, 26, 5, 5, 39, 5, 5, 18, 42, 7, 5, 88, 5, 5, 5, 56, 40, 1, 5, 27, 5, 5, 30, 110, 13, 2, 41, 5, 10, 6,  5, 2, 1, 5, 1, 1, 1, 1, 2, 1, 5},
//case 4 
{14, 8, 6, 5, 26, 5, 5, 39, 5, 5, 18, 42, 7, 5, 88, 5, 5, 5, 56, 40, 1, 5, 27, 5, 5, 30,110,  13, 2, 41, 5, 10, 6,  5, 2, 1, 5, 1, 1, 1, 1, 2, 1, 5},
//case 5 
{14, 8, 7, 5, 37, 5, 5, 47, 5, 5, 22, 42, 7, 5, 102, 5, 5, 5, 60, 45, 1, 5, 34, 5, 5, 34,118,  14, 2, 55, 5, 10, 6,  5, 2, 1, 5, 1, 1, 1, 1, 2, 1, 5},
//case 6 
{14, 8, 7, 5, 37, 5, 5, 47, 5, 5, 22, 42, 7, 5, 102, 5, 5, 5, 60, 47, 1, 5, 34, 5, 5, 34,118,  14, 2, 55, 5, 10, 6,  5, 2, 1, 5, 1, 1, 1, 1, 2, 1, 5},
//case 7 
{14, 8, 7, 5, 37, 5, 5, 47, 5, 5, 22, 42, 7, 5, 102, 5, 5, 5, 60, 55, 1, 5, 34, 5, 5, 34,118,  14, 2, 55, 5, 10, 6,  5, 2, 1, 5, 1, 1, 1, 1, 2, 1, 5},
//case 8 
{14, 8, 7, 5, 37, 5, 5, 47, 5, 5, 22, 42, 7, 5, 102, 5, 5, 5, 60, 60, 1, 5, 34, 5, 5, 34,118,  14, 2, 55, 5, 10, 6,  5, 2, 1, 5, 1, 1, 1, 1, 2, 1, 5
}
};
/*- Dyn UEs To Be SchPerTTI -*/
/* SPR_5948_CHG_END */
/** CA2 Changes Start **/
UInt64 reqElProcessingCycles[MAX_NUM_CELL][MAX_UES_SCHEDULED_INDX][MAX_CRITICAL_LEGS]= {{{0}}}; 
/** CA2 Changes End **/

#ifdef OVERSHOOT_CORRECTION
UInt8 reqElProcessingTimeIndex[MAX_DL_UE_SCHEDULED][MAX_UL_UE_SCHEDULED] = 
      {/*0  1  2  3  4  5  6  7  8  9 10 11 12 13 14 15 <- UL UEs*/
        {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*0 <- DL UEs*/
        {0, 1, 0, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 8}, /*1 <- DL UEs*/
        {0, 0, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 8, 0}, /*2 <- DL UEs*/
        {0, 2, 0, 0, 0, 4, 0, 0, 0, 0, 0, 0, 0, 8, 0, 0}, /*3 <- DL UEs*/
        {0, 0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0}, /*4 <- DL UEs*/
        {0, 0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0}, /*5 <- DL UEs*/
        {0, 0, 3, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0}, /*6 <- DL UEs*/
        {0, 3, 0, 0, 0, 0, 0, 0, 0, 7, 0, 0, 0, 0, 0, 0}, /*7 <- DL UEs*/
        {0, 0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0}, /*8 <- DL UEs*/
        {0, 0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0}, /*9 <- DL UEs*/
        {0, 0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*10 <- DL UEs*/
        {0, 0, 0, 0, 0, 6, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*11 <- DL UEs*/
        {0, 0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*12 <- DL UEs*/
        {0, 0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*13 <- DL UEs*/
        {0, 0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}, /*14 <- DL UEs*/
        {0, 5, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0} /*15 <- DL UEs*/
};
#endif
 
/*- Dyn UEs To Be SchPerTTI -*/
#endif /*DL_UL_SPLIT*/

#define STABLE_LOAD_TIME 5
#define OAM_REPORTING_TIME 20
/*****************************************************************************
 * Function Name  : setOverLoadLevel
 * Inputs         : tickOverLoadLevel, internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : Process the over load level of MAC and inform to OAM. 
 *****************************************************************************/
void setOverLoadLevel(OverLoadLevel tickOverLoadLevel, InternalCellIndex internalCellIndex)
{
    /** CA-TDD Changes Start **/
   /* SPR 15909 fix start */
    static tickType_t tickAtOvrLoad[MAX_NUM_CELL] = {0};
   /* SPR 15909 fix end */
    static UInt32 lastReportedLevel_g[MAX_NUM_CELL] = {0};
    /** CA-TDD Changes Stop **/

    if (macOverLoadLevel_g[internalCellIndex] == OVERLOAD_LEVEL_NORMAL && tickOverLoadLevel == OVERLOAD_LEVEL_NORMAL)
    {
        return;
    }

    if( macOverLoadLevel_g[internalCellIndex] <= tickOverLoadLevel )
    {
#ifndef DL_UL_SPLIT
        /* +- SPR 17777 */
        LOG_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                L2_SYS_FAIL,
                getCurrentTick(),
                __LINE__,
                tickOverLoadLevel,
                /*+ Dyn UEs To Be SchPerTTI +*/
                /** CA-TDD Changes Start **/
                macOverLoadLevel_g[internalCellIndex],
                /** CA-TDD Changes Stop **/
                /*+ Dyn UEs To Be SchPerTTI +*/
                dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                /*- Dyn UEs To Be SchPerTTI -*/
                "Overload level changed.");
#endif
        /** CA-TDD Changes Start **/
        macOverLoadLevel_g[internalCellIndex] = tickOverLoadLevel;
        /** CA-TDD Changes Stop **/
        /** CA-TDD Changes Start **/
        tickAtOvrLoad[internalCellIndex] = getCurrentTick();
        /** CA-TDD Changes Stop **/
    }
    else
    {
        /** CA-TDD Changes Start **/
        if((getCurrentTick() - tickAtOvrLoad[internalCellIndex]) >= STABLE_LOAD_TIME)
        /** CA-TDD Changes Stop **/
        {/** CA-TDD Changes Start **/
            if(macOverLoadLevel_g[internalCellIndex])
                /** CA-TDD Changes Stop **/
            {/** CA-TDD Changes Start **/
                macOverLoadLevel_g[internalCellIndex] --;
                /** CA-TDD Changes Stop **/
                /** CA-TDD Changes Start **/
                tickAtOvrLoad[internalCellIndex] = getCurrentTick();
                /** CA-TDD Changes Stop **/
            }
        }
    } /** CA-TDD Changes Start **/
    if( macOverLoadLevel_g[internalCellIndex] != lastReportedLevel_g[internalCellIndex]  ) 
    {
        lastReportedLevel_g[internalCellIndex] = macOverLoadLevel_g[internalCellIndex]; 
        /** CA-TDD Changes Stop **/
#ifndef DL_UL_SPLIT
        /* +- SPR 17777 */
        LOG_MSG(MAC_TICK_LD_CHANGE,LOGWARNING,
                L2_SYS_FAIL,
                getCurrentTick(),
                __LINE__,
                tickOverLoadLevel,
                /** CA-TDD Changes Start **/
                macOverLoadLevel_g[internalCellIndex],
                /** CA-TDD Changes Stop **/
                /*+ Dyn UEs To Be SchPerTTI +*/
                dlUeSchdIndxInCurrTick_g, ulUeSchdIndxInCurrTick_g, 0, 0 ,__func__,
                /*- Dyn UEs To Be SchPerTTI -*/
                "Overload level changed.");
#endif
        /*  report overload level to OAM */
	}
}
#ifdef OVERSHOOT_CORRECTION
/*+ Dyn UEs To Be SchPerTTI +*/
/*****************************************************************************
 * Function Name  : getIndexForReqElProcessingCycles
 * Inputs         : internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Description    : The thread executes execution legs passed to it through
 *                  arguments.
 *****************************************************************************/
/* +- SPR 17777 */
void getIndexForReqElProcessingCycles(void)
/* +- SPR 17777 */
{
    /** CA-TDD Changes Start **/
#ifndef DL_UL_SPLIT
    UInt8 ueToBeScheduledInDl = dlUeSchdIndxInCurrTick_g;
    UInt8 ueToBeScheduledInUl = ulUeSchdIndxInCurrTick_g;
    /** CA-TDD Changes Stop **/
    if(reqElProcessingTimeIndex[ueToBeScheduledInDl][ueToBeScheduledInUl] == 0)
    {
        /* Coverity CID: 55811 Fix Start */
        if (ueToBeScheduledInDl >= ueToBeScheduledInUl)
        {
            while((reqElProcessingTimeIndex[ueToBeScheduledInDl][ueToBeScheduledInUl] == 0) &&
                      (ueToBeScheduledInUl < (MAX_UL_UE_SCHEDULED - 1)))
            {
                ueToBeScheduledInUl++;
            }   
        }
        else
        {
            while((reqElProcessingTimeIndex[ueToBeScheduledInDl][ueToBeScheduledInUl] == 0) &&
                      (ueToBeScheduledInDl < (MAX_DL_UE_SCHEDULED - 1)))
            {
                ueToBeScheduledInDl++;
            }
        }
        /* Coverity CID: 55811 Fix End */
    }
    reqIndexForUeSchdInDlUl_g = reqElProcessingTimeIndex[ueToBeScheduledInDl][ueToBeScheduledInUl];    
#endif
}
/*+ Dyn UEs To Be SchPerTTI +*/
#endif

/*TDD Tick overshoot changes start*/
#ifdef TDD_CONFIG
/* It will contain sum of processing time for All ELs after MUX and before
 * UL_RA*/
/* To be calculated*/
/** CA-TDD Changes Start **/
UInt32
reqdProcessingTimeAtMuxTDD_g[MAX_NUM_CELL][MAX_NUMBER_OF_SUBFRAME_IN_SYSTEMFRAME][MAX_UES_SCHEDULED_INDX]
= {{{0}}};
/** CA-TDD Changes Stop **/

/* To be calculated*/
/** CA-TDD Changes Start **/
UInt64
reqdProcessingCyclesAtMuxTDD_g[MAX_NUM_CELL][MAX_NUMBER_OF_SUBFRAME_IN_SYSTEMFRAME][MAX_UES_SCHEDULED_INDX]
= {{{0}}};
/** CA-TDD Changes Stop **/

/* To be filled*/

/*+ Dyn UEs To Be SchPerTTI +*/
UInt32
reqdProcessingTimeELTdd_g[MAX_EXECUTION_LEGS][MAX_UES_SCHEDULED_INDX]
= {
   /* CASE0,   CASE1,   CASE2, CASE3,   CASE4, CASE5,  CASE6,  CASE7,  CASE8*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*UNKNOWN*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*WAIT_FOR_SUBFRAME_IND*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_HARQ_TIMER_EXPIRY_IF_TIME_LEFT*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_CQI_REPORTS_IF_TIME_LEFT*/
     {7,	7,	7,	7,	7,	7,	7,	7,	7},	 /*PROCESS_DEMUX_QUEUE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_DEMUX_QUEUE_IF_TIME_LEFT*/
     {50,	50,	60,	60,	70,	70,	100,	100,	100},	 /*PROCESS_MUX_QUEUE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_MUX_QUEUE_UNTIL_LAST_NODE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_N_RRC_OAM_MSG_IF_TIME_LEFT*/
     {59,	59,	70,	88,	88,	100,	100,	100,	100},	 /*PROCESS_PDCCH_PDSCH_WITH_MUX*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_PDCCH_PDSCH_WITHOUT_MUX*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_PDCCH_PDSCH_WITH_MUX_AND_TRIGGER_PUSCH*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_PDCCH_PDSCH_WITHOUT_MUX_AND_TRIGGER_PUSCH*/
     {40,	40,	49,	56,	56,	60,	60,	60,	60},	 /*PROCESS_PUSCH_FOR_DELAY_ONE*/
     {81,	81,	88,	110,	110,	125,	125,	125,	125},	 /*TX_DL_DATA_MSG*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_TX_QUEUE_UNTIL_LAST_NODE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_TX_QUEUE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*TX_COMMON_CHANNEL_DATA*/
     {38,	38,	39,	42,	42,	44,	44,	44,	44},	 /*TX_DL_CONTROL_MSG*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*TX_DL_DATA_MSG_IF_DATA_READY*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*TX_DL_CONTROL_MSG_AND_TRIGGER*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_ENCODER_QUEUE_WITH_TX_UNTIL_LAST_DATA*/
     {15,	15,	27,	49,	49,	60,	60,	60,	60},	 /*PROCESS_DL_HARQ_ACK_NACK_QUEUE*/
     /*SPR 16855 +-*/
     {1,	1,	1,	1,	1,	1,	1,	1,	1},	 /*PROCESS_DL_OAM_AND_RRC_MSG*/
     {1,	1,	1,	1,	1,	1,	1,	1,	1},	 /*PROCESS_UL_OAM_AND_RRC_MSG*/
     {1,	1,	1,	1,	1,	1,	1,	1,	1},	 /*PROCESS_UL_PENDING_DELETE_UE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_START_PHY*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_OAM_AND_RRC_MSG*/
     /*SPR 16855 +-*/
     {1,	1,	1,	1,	1,	1,	1,	1,	1},	 /*PROCESS_PENDING_DELETE_UE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_PENDING_RESET_UE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*SEND_AUDIT_REPORT_FOR_INACTIVE_UE*/
     {3,	3,	4,	4,	4,	4,	4,	4,	4},	 /*PROCESS_UL_RA*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_DL_SPS_STRATEGY*/
     {32,	32,	33,	39,	39,	45,	45,	45,	45},	 /*PROCESS_DL_SCH_STRATEGY*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_UL_SPS_STRATEGY*/
     {41,	41,	42,	42,	42,	44,	44,	44,	44},	 /*PROCESS_UL_SCH_STRATEGY*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_RX_CONTROL_DATA_WITH_DEMUX*/
     {3,	3,	3,	3,	3,	3,	3,	3,	3},	 /*PROCESS_HARQ_TIMER_EXPIRY*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_DL_HARQ_TIMER_EXPIRY*/
     {3,	3,	3,	3,	3,	3,	3,	3,	3},	 /*PROCESS_UL_HARQ_TIMER_EXPIRY*/
     {53,	53,	71,	103,	103,	107,	107,	107,	107},	 /*RLC_HANDLE_TX_OPPORTUNITY*/
     {1,	1,	1,	1,	1,	1,	1,	1,	1},	 /*PROCESS_CQI_REPORTS*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_N_CQI_REPORTS*/
     {1,	1,	1,	1,	1,	1,	1,	1,	1},	 /*PROCESS_N_RRC_OAM_MSG*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*REGISTER_WITH_PHY_FOR_SUBFRAME_IND*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*REGISTER_WITH_PHY_FOR_SUBFRAME_IND_AFTER_INIT_MAC*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*WAIT_FOR_CELL_CONFIG*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*WAIT_FOR_PDCCH_READY_TRIGGER*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_N_DEMUX_QUEUE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*WAIT_FOR_CONTROL_MSG_SENT_TRIGGER*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_RX_UNTIL_SUBFRAME_IND*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_RX_CONTROL_DATA_WITHOUT_DEMUX*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_ENCODER_QUEUE_WITH_TX*/
     {11,	11,	18,	27,	27,	31,	31,	31,	31},	 /*PROCESS_ENCODER_QUEUE_WITHOUT_TX*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_ENCODER_QUEUE_WITHOUT_TX_UNTIL_LAST_DATA*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_PUSCH_FOR_DELAY_TWO*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_PUSCH_FOR_DELAY_THREE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_UL_SCH_STRATEGY_IF_NOT_ALREADY_DONE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*SEND_CONTROL_TRIGGER*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*RX_CONTROL_TRIGGER*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*DEFAULT_STARTUP_EXEC_LEG*/
     {7,	7,	8,	9,	9,	13,	13,	13,	13},	 /*PROCESS_UL_CONFIG_MSG*/
     {10,	10,	10,	11,	11,	12,	12,	12,	12},	 /*TX_UL_CONFIG_MSG*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_SPS_PDSCH*/
     {5,	5,	5,	5,	5,	5,	5,	5,	5},	 /*PROCESS_UL_CSI_SINR_REPORTS*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_SPS_PUSCH*/
     /* + E_CID_PRS_TO_5.3*/
     {6,	6,	6,	6,	6,	6,	6,	6,	6},	 /*PROCESS_E_CID_REPORTS*/
     {7,	7,	7,	7,	7,	7,	7,	7,	7},	 /*PROCESS_PRS_TRANSMISSION*/
     /* - E_CID_PRS_TO_5.3*/
     {720,	720,	723,	638,	638,	650,	650,	650,	650},	 /*WAIT_FOR_TDD_SUBFRAME_IND*/
     {8,	8,	8,	8,	8,	8,	8,	8,	8},	 /*PROCESS_SRS_FOR_S_SUBFRAME*/
     {8,    8,  8,  8,  8,  8,  8,  8,  8},  /*PROCESS_PRACH_FOR_S_SUBFRAME*/
     {3,	3,	3,	3,	3,	3,	3,	3,	3},	 /*PROCESS_SRS_FOR_U_SUBFRAME*/
     /* TDD Config 0 changes start */
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_MERGE_UL_STARTEGY_NODES*/
     /* TDD Config 0 changes end */
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_SRS_REPORTS*/
     {12,	12,	12,	13,	13,	15,	15,	15,	15},	 /*PROCESS_DRX_FOR_ALL_UE*/
     {2,	2,	2,	2,	2,	2,	2,	2,	2},	 /*PROCESS_MEAS_GAP_FOR_ALL_UE*/
     {10,	10,	10,	10,	10,	20,	20,	20,	20},	 /*PROCESS_DELETE_SCHEDULE_LC_LIST*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PDCP_TX_PROCESS_MESSAGE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*RLC_HANDLE_CTRL_PDU_RLC_AM*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*RLC_HANDLE_HARQ_FAIL_IND*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*RLC_PDCP_HANDLE_DATA_FRM_MAC*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*RLC_TIMER_CHK_FOR_EXPIRY*/
     {3,	3,	3,	3,	3,	3,	3,	3,	3},	 /*RLC_TIMER_CHK_FOR_TX_EXPIRY*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PDCP_TX_PROCESS_DELIVERY_IND*/
     {3,	3,	3,	3,	3,	3,	3,	3,	3},	 /*RLC_HANDLE_COMMON_CHANNEL_AND_PENDING_QUEUE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*RLC_HANDLE_PENDING_DELETE_UE*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*PROCESS_RLC_PDCP_MSG*/
     {3,	3,	3,	3,	3,	3,	3,	3,	3}	/*PROCESS_MISC_UPDATES*/
#ifdef UTFWK_SIMULATION
     ,{0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*UPDATE_TICK_RX_MSQ*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0},	 /*RLC_UPDATE_QLOAD*/
     {0,	0,	0,	0,	0,	0,	0,	0,	0}	 /*RLC_HANDLE_TX_OPPORTUNITY_QOS*/
#endif
#ifdef PERF_STATS
     ,{0,	0,	0,	0,	0,	0,	0,	0,	0}	/*PROCESS_MAC_PERF_STATS*/
#endif
     ,{0,	0,	0,	0,	0,	0,	0,	0,	0}	/*PROCESS_MEM_CLEANUP*/
     ,{20,	20,	20,	20,	30,	30,	30,	30,	30}	/*UPDATE_DL_TOP_SCH_LIST_SCELL_STATE*/
     ,{20,	20,	20,	20,	20,	30,	30,	30,	30}	/*PROCESS_DL_CA_TIMER_EXPIRY*/
     ,{20,	20,	20,	20,	30,	30,	30,	30,	30}	/*PROCESS_PENDING_EVENT_FD*/
#ifdef DL_DATA_SEPARATION
     ,{0,	0,	0,	0,	0,	0,	0,	0,	0},	 /* PROCESS_RLC_HANDLE_TX_OPPORTUNITY_WITH_ENCODER_PER_UE*/
     {15,	15,	15,	15,	15,	15,	15,	15,	15},	 /* RLC_UPDATE_BUFF_OCC_TO_MAC*/
     {40,	40,	40,	40,	40,	40,	40,	40,	40}	 /* PROCESS_DL_SYNC_TX_DATA*/
#endif
     ,{30,	30,	30,	30,	50,	50,	50,	50,	50}	/*PROCESS_ENCODER_QUEUE_OF_OTHER_CELLS*/
     ,{10,	10,	10,	10,	10,	10,	10,	10,	10}	/*PROCESS_SET_STRATEGY_FD*/
     ,{10,	10,	10,	10,	10,	10,	10,	10,	10}	/*PROCESS_CHECK_STRATEGY_FD*/
     ,{10,	10,	10,	10,	10,	10,	10,	10,	10}	/*PROCESS_SET_PDCCH_PDSCH_FD*/
     ,{10,	10,	10,	10,	10,	10,	10,	10,	10}	/*PROCESS_CHECK_PDCCH_PDSCH_FD*/
     ,{10,	10,	10,	10,	10,	10,	10,	10,	10}	/*PROCESS_SET_MUX_FD*/
     ,{10,	10,	10,	10,	10,	10,	10,	10,	10}	/*PROCESS_CHECK_MUX_FD*/
     ,{10,	10,	10,	10,	10,	10,	10,	10,	10}	/*PROCESS_SET_ENCODER_FD*/
     ,{10,	10,	10,	10,	10,	10,	10,	10,	10}	/*PROCESS_CHECK_ENCODER_FD*/
}; /*End of reqdProcessingTimeELTdd_g */
/*- Dyn UEs To Be SchPerTTI -*/

UInt32 reqOptionalElProcessingTimeTdd_g[MAX_OPTIONAL_LEGS][MAX_UES_SCHEDULED_INDX] = 
{
    { 3,    3,      3,      4,      4,      4,      4,      4,      4}, /*PROCESS_UL_RA*/
    { 2,   10,     11,     25,     31,     35,     41,     41,     41}, /*PROCESS_DEMUX_QUEUE*/
    { 1,    1,      1,      1,      1,      1,      1,      1,      1}, /*PROCESS_CQI_REPORTS*/
    { 5,    5,      5,      5,      5,      5,      5,      5,      5}, /*PROCESS_UL_CSI_SINR_REPORTS*/
    /* + E_CID_PRS_TO_5.3*/
    { 6,    6,      6,      6,      6,      6,      6,      6,      6}, /*PROCESS_E_CID_REPORTS*/
    /* + E_CID_PRS_TO_5.3*/
    /* + SPR 13854 Changes */
    {0,     0,      0,      0,      0,      0,      0,      0,      0}, /*UPDATE_DL_TOP_SCH_LIST_SCELL_STATE*/
    /* - SPR 13854 Changes */
     /*SPR 16855 +-*/
    { 3,    3,      3,      3,      3,      3,      3,      3,      3}, /*RLC_TIMER_CHK_FOR_TX_EXPIRY*/
    { 1,    1,      1,      1,      1,      1,      1,      1,      1}, /*PROCESS_PENDING_DELETE_UE*/
    { 1,    1,      1,      1,      1,      1,      1,      1,      1}, /*SEND_AUDIT_REPORT_FOR_INACTIVE_UE*/
    { 3,    3,      3,      3,      3,      3,      3,      3,      3}, /*PROCESS_MISC_UPDATES*/
     /*SPR 16855 +-*/
    { 1,    1,      1,      1,      1,      1,      1,      1,      1}, /*PROCESS_DL_OAM_AND_RRC_MSG*/
    { 1,    1,      1,      1,      1,      1,      1,      1,      1}, /*PROCESS_UL_OAM_AND_RRC_MSG*/
    { 1,    1,      1,      1,      1,      1,      1,      1,      1}, /*PROCESS_UL_PENDING_DELETE_UE*/
     /*SPR 16855 +-*/
    { 3,    3,      3,      3,      3,      3,      3,      3,      3}, /*RLC_HANDLE_COMMON_CHANNEL_AND_PENDING_QUEUE*/
    /* + SPR 13854 Changes */
    { 3,    3,      3,      3,      3,      3,      3,      3,      3} /*RLC_HANDLE_PENDING_DELETE_UE*/
    /* - SPR 13854 Changes */
};

/*To be calculated*/
UInt64 reqOptionalElProcessingCyclesTdd_g[MAX_OPTIONAL_LEGS][MAX_UES_SCHEDULED_INDX] = {{0}};


/* SPR 13768 Changes Start */
/****************************************************************************
 * Function Name  : updateElcycleCountTDD
 * Inputs         :  internalCellIndex
 * Outputs        : None
 * Returns        : None
 * Variables      : None
 * Description    : This function calculates cycle count required to process
                    mandatory legs at MUX.
 ***************************************************************************/
    /* +- SPR 17777 */
void updateElCycleCountTDD(InternalCellIndex internalCellIndex)
    /* +- SPR 17777 */
{ 
    /* SPR 13768 Changes End */
    UInt8 maxMandatoryLegs = 0;
    UInt8 mandatoryExecutionLegCount = 0;
    UInt8 maxOptLegs = 0;
    UInt8 maxUeSch = 0;
    UInt8 subFrame = 0;
    UInt8 afterMuxFlag = 0;
    UInt8 mandatoryLegAfterMux = 0;
    ExecutionLeg executionLeg;

    TDDSubFrame *subFrameInf_p = PNULL;
    /* SPR 13768 Changes Start */
    ExecutionFlowArguments *execFlowThreadArgs_p =
        &(executionFlowCreationParamArr_g[internalCellIndex]->threadArguments);   
    /* SPR 13768 Changes End */
    subFrameInf_p= &(execFlowThreadArgs_p->subFrameInfo[0]);

    /* + SPR 14364 Changes */
    execFlowThreadArgs_p->internalCellIndex = internalCellIndex;
    /* - SPR 14364 Changes */
    

    for (subFrame = 0; subFrame < NUMBER_OF_SUBFRAME_IN_SYSTEM_FRAME;++subFrame)
    {
        afterMuxFlag = 0;
        mandatoryExecutionLegCount = subFrameInf_p[subFrame].mandatoryExecutionLegCount;   
        for(maxMandatoryLegs = 0; maxMandatoryLegs <
                mandatoryExecutionLegCount ; ++maxMandatoryLegs)
        {

            if( subFrameInf_p[subFrame].executionLegArr[maxMandatoryLegs].executionLeg == PROCESS_PDCCH_PDSCH_WITH_MUX)
            {
                afterMuxFlag = 1;
                mandatoryLegAfterMux = maxMandatoryLegs + 1;
                break;
            }
        }
        if(afterMuxFlag)
        {
            for(maxUeSch =0; maxUeSch < MAX_UES_SCHEDULED_INDX ; ++maxUeSch)
            {

                for(maxMandatoryLegs = mandatoryLegAfterMux ; maxMandatoryLegs <
                        mandatoryExecutionLegCount ; ++maxMandatoryLegs)
                {
                    executionLeg = subFrameInf_p[subFrame].executionLegArr[maxMandatoryLegs].executionLeg;

                    /** CA-TDD Changes Start **/
                    reqdProcessingTimeAtMuxTDD_g[execFlowThreadArgs_p->internalCellIndex][subFrame][maxUeSch] +=
                        reqdProcessingTimeELTdd_g[executionLeg][maxUeSch];
                    /** CA-TDD Changes Stop **/
                }
                /** CA-TDD Changes Start **/
                reqdProcessingCyclesAtMuxTDD_g[execFlowThreadArgs_p->internalCellIndex][subFrame][maxUeSch] =
                    (reqdProcessingTimeAtMuxTDD_g[execFlowThreadArgs_p->internalCellIndex][subFrame][maxUeSch] *
                     ccInMs_g)/1000;
                /** CA-TDD Changes Stop **/
            }
        }
    }
    


    for(maxUeSch =0; maxUeSch < MAX_UES_SCHEDULED_INDX ; maxUeSch++)
    {
        for(maxOptLegs =0; maxOptLegs < MAX_OPTIONAL_LEGS; maxOptLegs++)
        {
            reqOptionalElProcessingCyclesTdd_g[maxOptLegs][maxUeSch] =  (reqOptionalElProcessingTimeTdd_g[maxOptLegs][maxUeSch] *ccInMs_g)/1000;
        }
    }



}

#endif
#endif

/*TDD Tick overshoot changes end*/


/* cyclomatic changes */
/*****************************************************************************
 * Function Name  : mac_reset_cce
 * Inputs         : aggrLvl -Aggregation Level,
 *                  dciInfo_p - pointer to DciCCEInfo
 *                  internalCellIndex    
 * Outputs        : None
 * Returns        : None
 * Description    : This Function is used to reset the given CCE.
 *****************************************************************************/
#ifdef FDD_CONFIG
void mac_reset_cce(
     SInt8 aggrLvl, 
     DciCCEInfo *dciInfo_p,
     InternalCellIndex internalCellIndex)
{
    UInt8 cceNumber = dciInfo_p->dciPduInfo.cceIndex;
    /*DYN_PDCCH*/
    UInt32 maxCCEAvailable = 0;
    maxCCEAvailable = maxCCEForCFI_g[internalCellIndex][currentCFIValue_g[internalCellIndex]-1];
    /*DYN_PDCCH*/
    if (dciInfo_p->msgType != SPS_OCCASION) 
    {
        if ((aggrLvl == 0) || (dciInfo_p->isCCEFree))
        {
            ltePanic("Passed AGGR LVL is [%d] \n", aggrLvl);
        }
        /*DYN_PDCCH*/
        if ((UInt32)(cceNumber + aggrLvl ) <= maxCCEAvailable)
        /*DYN_PDCCH*/
        {
            while (aggrLvl > 0)
            {
                dciInfo_p->isCCEFree = CCE_FREE;
                dciInfo_p++;
                aggrLvl--;
            }
        }
        else
        {
            /*DYN_PDCCH*/
            SInt32 toResetFrmEnd = maxCCEAvailable - cceNumber;
            /*DYN_PDCCH*/
            UInt8 toResetFrmStart = aggrLvl - (toResetFrmEnd );
            DciCCEInfo *dciCCEInfoStart_p = dciInfo_p - cceNumber;
            while (toResetFrmEnd > 0)
            {
                dciInfo_p->isCCEFree = CCE_FREE;
                dciInfo_p++;
                toResetFrmEnd--;
            }
            while (toResetFrmStart--)
            {
            dciCCEInfoStart_p[toResetFrmStart].isCCEFree = CCE_FREE;
            }
        }
     }
     else
     {
          dciInfo_p->isCCEFree = CCE_FREE;
     }
}
#else
void mac_reset_cce(
     SInt8 aggrLvl, 
     DciCCEInfo *dciInfo_p,
     InternalCellIndex internalCellIndex)
{
    UInt8 cceNumber = dciInfo_p->dciPduInfo.cceIndex;
    /*DYN_PDCCH*/
    UInt32 maxCCEAvailable = 0;
    /*cov 85808 +-*/
    /*DYN_PDCCH*/
    LTE_GCC_UNUSED_PARAM(internalCellIndex)
    /* DL DATA Sep Changes*/
    maxCCEAvailable = 100;
    if (dciInfo_p->msgType != SPS_OCCASION) 
    {
        if ((aggrLvl == 0) || (dciInfo_p->isCCEFree))
        {
            /* DL DATA Sep Changes*/
            /*ltePanic("Passed AGGR LVL is [%d] \n", aggrLvl);*/
        }
        /*DYN_PDCCH*/
        /* + SPR 17439 */
        if ((UInt32)(cceNumber + aggrLvl ) <= maxCCEAvailable)
        /* - SPR 17439 */    
        /*DYN_PDCCH*/
        {
            while (aggrLvl > 0)
            {
                dciInfo_p->isCCEFree = CCE_FREE;
                /* + SPS_TDD_Changes */
                dciInfo_p->msgType = BCCH_MSG;
                /* - SPS_TDD_Changes */
                dciInfo_p++;
                aggrLvl--;
            }
        }
        else
        {
            /*DYN_PDCCH*/
            SInt32 toResetFrmEnd = maxCCEAvailable - cceNumber;
            /*DYN_PDCCH*/
            UInt8 toResetFrmStart = aggrLvl - (toResetFrmEnd );
            DciCCEInfo *dciCCEInfoStart_p = dciInfo_p - cceNumber;
            while (toResetFrmEnd > 0)
            {
                dciInfo_p->isCCEFree = CCE_FREE;
                /* + SPS_TDD_Changes */
                dciInfo_p->msgType = BCCH_MSG;
                /* - SPS_TDD_Changes */
                dciInfo_p++;
                toResetFrmEnd--;
            }
            while (toResetFrmStart--)
            {
                dciCCEInfoStart_p[toResetFrmStart].isCCEFree = CCE_FREE;
                /* + SPS_TDD_Changes */
                dciCCEInfoStart_p[toResetFrmStart].msgType = BCCH_MSG;
                /* - SPS_TDD_Changes */
            }
        }
     }
     else
     {
         /* + SPS_TDD_Changes */
         dciInfo_p->msgType = BCCH_MSG;
         /* - SPS_TDD_Changes */
          dciInfo_p->isCCEFree = CCE_FREE;
          dciInfo_p->dciPduInfo.dciFormat=MAX_NUM_DCI_FORMAT;
     }
}
#endif
/******************************************************************************
 * Function Name   : mac_reset_extra_cce
 * Inputs          : dciCCEContainerInfo_p-
 *                   dciCCEInfo_p-
 *                   dlUEContext_p-
 *                   internalCellIndex-
 * Outputs         : None
 * Returns         : None
 * Description     : This func empties any strategy nodes which 
 *                   couldnt be processed by pdcch and frees the harq processes
 *                   in case of new transmissions and QoS based scheduling
 *******************************************************************************/
void mac_reset_extra_cce( DciCCEContainerInfo *dciCCEContainerInfo_p  , DciCCEInfo *dciCCEInfo_p 
          ,DLUEContext *dlUEContext_p,InternalCellIndex internalCellIndex)
{
   DciCCEInfo *dciExtraCCEInfo_p=PNULL;
   /*pdcch for scell changes start*/
    if(IS_CA_ENABLED()  && (dciCCEInfo_p->dciPduInfo.aggregationLvl
         == ONE) && ALLOCATE_EXTRA_CCE(dlUEContext_p,internalCellIndex) )
    {
            dciExtraCCEInfo_p=&(dciCCEContainerInfo_p->dciCCEInfo[dciCCEInfo_p->dciPduInfo.cceIndex + ONE]);
            mac_reset_cce(dciExtraCCEInfo_p->dciPduInfo.aggregationLvl, dciExtraCCEInfo_p,
                    internalCellIndex);
    }
    /*pdcch for scell changes end*/
}
/*****************************************************************************
 * Function Name  : getdlCQIInfo
 * Inputs         : dlUEContext_p - pointer to Dl UE Context
 *                  internalCellIndex - Cell-Index at MAC
 * Outputs        : It will set the  pointer to DL CQI info.
 * Returns        : Pointer to DLCQIInfo
 * Description    : This funtion will be used to find out whether cqi info
 * is to be updated in primary or secondary DL UE context
 *****************************************************************************/
DLCQIInfo *getdlCQIInfo( DLUEContext *dlUEContext_p,
		InternalCellIndex internalCellIndex)
{

	DLCQIInfo *dlCQIInfo_p = PNULL;
	if(internalCellIndex == dlUEContext_p->internalCellIndex)
	{
		dlCQIInfo_p = &dlUEContext_p->dlCQIInfo;
	}
	else 
	{
		if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX])
		{
			if(dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->internalCellIndex==internalCellIndex)
			{

				dlCQIInfo_p = &dlUEContext_p->dlUeScellContext_p[START_SCELL_INDEX]->ScelldlCQIInfo;
			}
		}

	}
	return dlCQIInfo_p;

}

/* cyclomatic changes */
/* Compilation fix 10-Jun-2014 start */ 
#if (!defined(DL_UL_SPLIT) && !defined(DL_UL_SPLIT_TDD)) 
/* Compilation fix 10-Jun-2014 end */ 
/****************************************************************************
 * Function Name  : lteMacInterfacePostAllSyncFds 
 * Inputs         : internalCellIndex - Internal cell Index
 *                  isOtherCellStateRunning : Other cell state  
 * Outputs        : None
 * Returns        : None
 * Description    : This function post all the syn fds. 
 ****************************************************************************/
/* + Critical section changes */
void lteMacInterfacePostAllSyncFds(InternalCellIndex internalCellIndex,
  				   UInt8 isOtherCellStateRunning )
{
    if (isOtherCellStateRunning) 
    {
        /* SPR 13132 Changes Start */
        /* SPR 15909 fix start */
        tickType_t getCurrentTick = getCurrentTickForCell(!internalCellIndex);
        /* SPR 15909 fix end */
#ifdef TDD_CONFIG
          UInt8 nexTickFlag = 0;
          if((getCurrentTickForCell(internalCellIndex)) == (getCurrentTickForCell(!internalCellIndex)+1))
          {
              nexTickFlag = TRUE;
          }
#endif
        switch(cellElSyncTracker[!internalCellIndex])
        {
/******** By Default EVENT FDs are used for sync threads. Mutex & semaphores are 
  implemented but not fully tested. **********/
            case STRATEGY_SYNC:
#ifdef SEM_SYNC
		/* SPR 16617 Changes start */
        /* SPR 18122 Changes Start*/
                semPostDefault (&ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPostDefault (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPostDefault (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPostDefault (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
		/* SPR 16617 Changes end */
#elif MUTEX_SYNC
                semPost (&ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPost (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPost (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPost (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#else
                eventfd_write_wrapper(ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
                eventfd_write_wrapper(ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
                eventfd_write_wrapper(ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
                eventfd_write_wrapper(ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
#endif
         /* SPR 18122 Changes End*/
                break;

            case PDSCH_SYNC:
#ifdef SEM_SYNC
         /* SPR 18122 Changes Start*/
                semPostDefault (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPostDefault (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPostDefault (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#elif MUTEX_SYNC
                semPost (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPost (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPost (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#else
                eventfd_write_wrapper(ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
                eventfd_write_wrapper(ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
                eventfd_write_wrapper(ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
#endif
       /* SPR 18122 Changes End*/
                break;

            case MUX_SYNC:
#ifdef SEM_SYNC
      /* SPR 18122 Changes Start*/
                semPostDefault (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPostDefault (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#elif MUTEX_SYNC
                semPost (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                semPost (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#else
                eventfd_write_wrapper(ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
                eventfd_write_wrapper(ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
#endif
      /* SPR 18122 Changes End*/
                break;

            case ENCODER_SYNC:
#ifdef SEM_SYNC
      /* SPR 18122 Changes Start*/
                semPostDefault (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#elif MUTEX_SYNC
                semPost (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#else
                eventfd_write_wrapper(ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)],1);
#endif
      /* SPR 18122 Changes End*/
                break;

            case FALSE_SYNC:
                /* Post events for other cell only if this cell is one tick ahead */
                if( (getCurrentTickForCell(internalCellIndex)) == (getCurrentTickForCell(!internalCellIndex) + 1))
                {
#ifdef SEM_SYNC
      /* SPR 18122 Changes Start*/
                    semPostDefault (&ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                    semPostDefault (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                    semPostDefault (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                    semPostDefault (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#elif MUTEX_SYNC
                    semPost (&ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                    semPost (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                    semPost (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
                    semPost (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#else
                    eventfd_write_wrapper(ev_fd_strategy_g[internalCellIndex][!(MODULO(getCurrentTick,2))],1);
                    eventfd_write_wrapper(ev_fd_pdsch_g[internalCellIndex][!(MODULO(getCurrentTick,2))],1);
                    eventfd_write_wrapper(ev_fd_mux_g[internalCellIndex][!(MODULO(getCurrentTick,2))],1);
                    eventfd_write_wrapper(ev_fd_encoder_g[internalCellIndex][!(MODULO(getCurrentTick,2))],1);
#endif
     /* SPR 18122 Changes End*/
                }
                break;
                /* SPR 13132 Changes End */

            default:
                break;
        }
#ifdef TDD_CONFIG
        if(nexTickFlag)
        {
#ifdef SEM_SYNC
     /* SPR 18122 Changes Start*/
		/* SPR 16617 Changes start */
		semPostDefault (&ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTick,2)]);
		semPostDefault (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)]);
		semPostDefault (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
		semPostDefault (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
		/* SPR 16617 Changes stop*/
#elif MUTEX_SYNC
		semPost (&ev_fd_strategy_g[internalCellIndex][MODULO(getCurrentTick,2)]);
		semPost (&ev_fd_pdsch_g[internalCellIndex][MODULO(getCurrentTick,2)]);
		semPost (&ev_fd_mux_g[internalCellIndex][MODULO(getCurrentTick,2)]);
		semPost (&ev_fd_encoder_g[internalCellIndex][MODULO(getCurrentTick,2)]);
#else
		eventfd_write_wrapper(ev_fd_strategy_g[internalCellIndex][!(MODULO(getCurrentTick,2))],1);
		eventfd_write_wrapper(ev_fd_pdsch_g[internalCellIndex][!(MODULO(getCurrentTick,2))],1);
		eventfd_write_wrapper(ev_fd_mux_g[internalCellIndex][!(MODULO(getCurrentTick,2))],1);
		eventfd_write_wrapper(ev_fd_encoder_g[internalCellIndex][!(MODULO(getCurrentTick,2))],1);
#endif
     /* SPR 18122 Changes End*/
        }
#endif
    }
    else
    {
        macParams_g.lowestRunningCellIndex = internalCellIndex;
    } 
}

/* - Critical section changes */
#endif
