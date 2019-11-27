#include "lteFapiMacPhyInterface.h"
#include "lteMacPhyInterface.h"

#define MAX_INTERFERENCE_PER_PRB 100 
#define LOG_INVALID_MSG_ID 0xffff

const char * fapiL1ApiMsgStr[] = {
    "PHY_PARAM_REQUEST", // 0
    "PHY_PARAM_RESPONSE", // 1
    "PHY_CELL_CONFIG_REQUEST", // 2
    "PHY_CELL_CONFIG_RESPONSE", // 3
    "PHY_START_REQUEST", // 4
    "PHY_STOP_REQUEST", // 5
    "PHY_STOP_INDICATION", // 6
    "PHY_UE_CONFIG_REQUEST", // 7
    "PHY_UE_CONFIG_RESPONSE", // 8
    "PHY_ERROR_INDICATION", // 9
    "PHY_UE_RELEASE_REQ", // 10
    "PHY_UE_RELEASE_RESP", // 11
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "PHY_DL_CONFIG_REQUEST", // 128
    "PHY_UL_CONFIG_REQUEST", // 129
    "PHY_UL_SUBFRAME_INDICATION", // 130
    "PHY_DL_HI_DCI0_REQUEST", // 131
    "PHY_DL_TX_REQUEST", // 132
    "PHY_UL_HARQ_INDICATION", // 133
    "PHY_UL_CRC_INDICATION", // 134
    "PHY_UL_RX_ULSCH_INDICATION", // 135
    "PHY_UL_RACH_INDICATION", // 136
    "PHY_UL_SRS_INDICATION", // 137
    "PHY_UL_RX_SR_INDICATION", // 138
    "PHY_UL_RX_CQI_INDICATION", // 139 
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "INVALID_MSG_ID",
    "PHY_MEASUREMENT_REPORT_INDICATION" // 150
};

const char * fapiL1ErrorCodes[] = {
    "FAPI_MSG_OK",
    "FAPI_MSG_INVALID_STATE",
    "FAPI_MSG_INVALID_CONFIG",
    "FAPI_SFN_OUT_OF_SYNC",
    "FAPI_MSG_SUBFRAME_ERR",
    "FAPI_MSG_BCH_MISSING",
    "FAPI_MSG_INVALID_SFN",
    "FAPI_MSG_HI_ERR",
    "FAPI_MSG_TX_ERR"
};

const char * fapiL1DlDciFormats[] = {
    "FAPI_DL_DCI_FORMAT_1", // 0
    "FAPI_DL_DCI_FORMAT_1A", // 1
    "FAPI_DL_DCI_FORMAT_1B", // 2
    "FAPI_DL_DCI_FORMAT_1C", // 3
    "FAPI_DL_DCI_FORMAT_1D", // 4
    "FAPI_DL_DCI_FORMAT_2", // 5
    "FAPI_DL_DCI_FORMAT_2A", // 6
    "FAPI_DL_DCI_FORMAT_2B", // 7
    "FAPI_DL_DCI_FORMAT_2C" // 8
};

const char * fapiL1DlPduTypes[] = {
    "FAPI_DCI_DL_PDU", // 0
    "FAPI_BCH_PDU", // 1
    "FAPI_MCH_PDU", // 2
    "FAPI_DLSCH_PDU", // 3
    "FAPI_PCH_PDU", // 4
    "FAPI_PRS_PDU", // 5
    "FAPI_CSI_RS_PDU" // 6
};

const char * fapiL1UlPduTypes[] = {
    "FAPI_ULSCH", // 0
    "FAPI_ULSCH_CQI_RI", // 1
    "FAPI_ULSCH_HARQ", // 2
    "FAPI_ULSCH_CQI_HARQ_RI", // 3
    "FAPI_UCI_CQI", // 4
    "FAPI_UCI_SR", // 5
    "FAPI_UCI_HARQ", // 6
    "FAPI_UCI_SR_HARQ", // 7
    "FAPI_UCI_CQI_HARQ", // 8
    "FAPI_UCI_CQI_SR", // 9
    "FAPI_UCI_CQI_SR_HARQ", // 10
    "FAPI_SRS", // 11
    "FAPI_HARQ_BUFFER", // 12
    "FAPI_ULSCH_UCI_CSI", // 13
    "FAPI_ULSCH_UCI_HARQ", // 14
    "FAPI_ULSCH_CSI_UCI_HARQ" // 15
};

#ifndef FAPI_4_0_COMPLIANCE
const char * fapiL1Tags[] = {
    "INVALID TAG", //0
    "FAPI_DUPLEXING_MODE", //1 
    "FAPI_PCFICH_POWER_OFFSET", //2
    "FAPI_P_B", //3
    "FAPI_DL_CYCLIC_PREFIX_TYPE", //4
    "FAPI_UL_CYCLIC_PREFIX_TYPE", //5
    "FAPI_DL_CHANNEL_BANDWIDTH", //6   
    "FAPI_UL_CHANNEL_BANDWIDTH", //7
    "FAPI_REFERENCE_SIGNAL_POWER", //8
    "FAPI_TX_ANTENNA_PORTS", //9
    "FAPI_RX_ANTENNA_PORTS", //10
    "FAPI_PHICH_RESOURCE", //11
    "FAPI_PHICH_DURATION", //12
    "FAPI_PHICH_POWER_OFFSET", //13
    "FAPI_PRIMARY_SYNC_SIGNAL", //14
    "FAPI_SECONDARY_SYNC_SIGNAL", //15
    "FAPI_PHYSICAL_CELL_ID", //16
    "FAPI_CONFIGURATION_INDEX", //17
    "FAPI_ROOT_SEQUENCE_INDEX", //18
    "FAPI_ZERO_CORRELATION_ZONE_CONFIGURATION", //19
    "FAPI_HIGH_SPEED_FLAG", //20
    "FAPI_FREQUENCY_OFFSET", //21
    "FAPI_HOPPING_MODE", //22
    "FAPI_HOPPIG_OFFSET", //23
    "FAPI_NUM_OF_SUB_BANDS", //24
    "FAPI_DELTA_PUCCH_SHIFT", //25
    "FAPI_N_CQI_RB", //26
    "FAPI_N_AN_CS", //27
    "FAPI_N_1_PUCCH_AN", //28
    "FAPI_BANDWIDTH_CONFIGURATION", //29
    "FAPI_MAX_UP_PTS", //30
    "FAPI_SRS_SUB_FRAME_CONFIGURATION", //31
    "FAPI_SRS_ACK_NACK_SRS_SIMULTANEOUS_TRANSMISSION", //32
    "FAPI_UPLINK_RS_HOPPING", //33
    "FAPI_GROUP_ASSIGNMENT", //34
    "FAPI_CYCLIC_SHIFT_1_FOR_DMRS", //35
    "FAPI_SUB_FRAME_ASSIGNMENT", //36
    "FAPI_SPECIAL_SUB_FRAME_PATTERNS", //37
    "FAPI_PRS_BANDWIDTH", //38
    "FAPI_PRS_CYCLIC_PREFIX", //39
    "FAPI_DL_BANDWIDTH_SUPPORT", //40 
    "FAPI_UL_BANDWIDTH_SUPPORT", //41
    "FAPI_DL_MODULATION_SUPPORT", //42
    "FAPI_UL_MODULATION_SUPPORT", //43
    "FAPI_PHY_ANTENNA_CAPABILITY", //44
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_DATA_REPORT_MODE", //50 
    "FAPI_SFN_SF", //51
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_PHY_STATE", //60
};
#else
const char * fapiL1Tags[] = {
    "INVALID TAG",
    "FAPI_DUPLEXING_MODE", //1 
    "FAPI_PCFICH_POWER_OFFSET", //2
    "FAPI_P_B", //3
    "FAPI_DL_CYCLIC_PREFIX_TYPE", //4
    "FAPI_UL_CYCLIC_PREFIX_TYPE", //5
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_DL_CHANNEL_BANDWIDTH", //10   
    "FAPI_UL_CHANNEL_BANDWIDTH", //11
    "FAPI_REFERENCE_SIGNAL_POWER", //12
    "FAPI_TX_ANTENNA_PORTS", //13
    "FAPI_RX_ANTENNA_PORTS", //14
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_PHICH_RESOURCE", //20
    "FAPI_PHICH_DURATION", //21
    "FAPI_PHICH_POWER_OFFSET", //22
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_PRIMARY_SYNC_SIGNAL", //30
    "FAPI_SECONDARY_SYNC_SIGNAL", //31
    "FAPI_PHYSICAL_CELL_ID", //32
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_CONFIGURATION_INDEX", //40
    "FAPI_ROOT_SEQUENCE_INDEX", //41
    "FAPI_ZERO_CORRELATION_ZONE_CONFIGURATION", //42
    "FAPI_HIGH_SPEED_FLAG", //43
    "FAPI_FREQUENCY_OFFSET", //44
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_HOPPING_MODE", //50
    "FAPI_HOPPIG_OFFSET", //51
    "FAPI_NUM_OF_SUB_BANDS", //52
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_DELTA_PUCCH_SHIFT", //60
    "FAPI_N_CQI_RB", //61
    "FAPI_N_AN_CS", //62
    "FAPI_N_1_PUCCH_AN", //63
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_BANDWIDTH_CONFIGURATION", //70
    "FAPI_MAX_UP_PTS", //71
    "FAPI_SRS_SUB_FRAME_CONFIGURATION", //72
    "FAPI_SRS_ACK_NACK_SRS_SIMULTANEOUS_TRANSMISSION", //73
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_UPLINK_RS_HOPPING", //80
    "FAPI_GROUP_ASSIGNMENT", //81
    "FAPI_CYCLIC_SHIFT_1_FOR_DMRS", //82
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_SUB_FRAME_ASSIGNMENT", //90
    "FAPI_SPECIAL_SUB_FRAME_PATTERNS", //91
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_MBSFN_AREA_ID", //100
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_PRS_BANDWIDTH", //110
    "FAPI_PRS_CYCLIC_PREFIX", //111
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_DL_BANDWIDTH_SUPPORT", //200
    "FAPI_UL_BANDWIDTH_SUPPORT", //201
    "FAPI_DL_MODULATION_SUPPORT", //202
    "FAPI_UL_MODULATION_SUPPORT", //203
    "FAPI_PHY_ANTENNA_CAPABILITY", //204
    "FAPI_PHY_RELEASE_CAPABILITY", //205
    "FAPI_PHY_MBSFN_CAPABILITY", //206 
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_DATA_REPORT_MODE", //240
    "FAPI_SFN_SF", //241
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "INVALID TAG",
    "FAPI_PHY_STATE" //250
};
#endif

const char * fapiL1HarqMode[] = {
    "BUNDLING", // 0
    "MULTIPLEXING", // 1
    "SPECIAL_BUNDLING" // 2
};


typedef struct
{
    FAPI_ulSCHPduInfo_st           ulSCHPduInfo;
    FAPI_cqiPduInfo_st             cqiInfo;
} Temp_FAPI_ULSCH_UCI_CSI_Struct;

typedef struct
{
    FAPI_ulSCHPduInfo_st           ulSCHPduInfo;
#ifdef TDD_CONFIG
    FAPI_tddHarqPduInfo_st         harqInfo;
#endif
#ifdef FDD_CONFIG
    FAPI_fddHarqPduInfo_st         harqInfo;
#endif
} Temp_FAPI_ULSCH_UCI_HARQ_Struct;

typedef struct
{
    FAPI_ulSCHPduInfo_st           ulSCHPduInfo;
    FAPI_cqiPduInfo_st             cqiInfo;
#ifdef TDD_CONFIG
    FAPI_tddHarqPduInfo_st         harqInfo;
#endif
#ifdef FDD_CONFIG
    FAPI_fddHarqPduInfo_st         harqInfo;
#endif
} Temp_FAPI_ULSCH_CSI_UCI_HARQ_Struct;

void printSfnSf(FILE *outFilePtr, UInt16 sfnsf)
{
    fprintf(outFilePtr, "SF %d\n", (UInt8)(sfnsf & 0x0F));
    fprintf(outFilePtr, "SFN %d\n", (UInt16)((sfnsf & 0xFFF0) >> 4));
}

void decode_fapi_message(char *msg_buffer, FILE *outFilePtr)
{
#ifndef FLEXRAN
    FAPI_l1ApiMsg_st    *l1ApiMsg_p= PNULL;
    UInt16               msgId  = 0;
    /* +- SPR 17777 */
    UInt32               value  = 0;
    UInt16               index_msgId = LOG_INVALID_MSG_ID;
    /* SPR 17884 FIX START */
    UInt16		 i = 0;
    UInt16               j = 0;
    UInt16               k = 0;
    /* SPR 17884 FIX END */
    l1ApiMsg_p = (FAPI_l1ApiMsg_st *)msg_buffer;

	if (PNULL == msg_buffer) {
		fprintf(outFilePtr, "msg_buffer is NULL\n");
		return;
	}

    msgId  = l1ApiMsg_p->msgId;
    /* +- SPR 17777 */
    // FAPI_VendorSpecific_st *vendorSpecificData_p = PNULL;
    //index_msgId = (msgId >= PHY_PARAM_REQUEST && msgId <= PHY_UE_RELEASE_RESP)? 
    //                      msgId:((msgId >= PHY_DL_CONFIG_REQUEST && msgId <= PHY_UL_RX_CQI_INDICATION)?
    //                      (msgId - PHY_DL_CONFIG_REQUEST + PHY_UE_RELEASE_RESP + 1):LOG_INVALID_MSG_ID);
    index_msgId = msgId;
    /* +- SPR 17777 */
    if ((UInt16)LOG_INVALID_MSG_ID == index_msgId || (index_msgId + 1) > (UInt16)(sizeof(fapiL1ApiMsgStr)/sizeof(fapiL1ApiMsgStr[0])))
    {
        fprintf(outFilePtr, "Invalid MsgId[%d] received at L1/L2 FAPI Interface. Message decoding failed\n", msgId);
        return;
    }
    switch (msgId)
    {
        case PHY_PARAM_REQUEST:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                break;
            }

        case PHY_PARAM_RESPONSE:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_paramResponse_st *msg_body = (FAPI_paramResponse_st *)&l1ApiMsg_p->msgBody[0];
                fprintf(outFilePtr, "Error Code %s (%d)\n",
                        fapiL1ErrorCodes[msg_body->errCode], msg_body->errCode);
                fprintf(outFilePtr, "Number of TLVs %d\n", msg_body->numOfTlv);
                if(msg_body->errCode == FAPI_MSG_OK)
                {
    /* SPR 17884 FIX START */
                    for(i = 0; i < msg_body->numOfTlv; i++)
    /* SPR 17884 FIX END */
                    {
                        FAPI_paramResponseTLV_st *tlv = (FAPI_paramResponseTLV_st *)&msg_body->tlvs[i];
                        fprintf(outFilePtr, "\tTag=%s(%d), ",
                                (tlv->tag >=
                                 sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                                "Invalid Tag" : fapiL1Tags[tlv->tag],
                                tlv->tag);

                        UInt8 *paramValue = (UInt8 *)&tlv->configParam;

                        switch(tlv->tagLen)
                        {
                            //p_value = &tlv->configParam;
                            case 1:
                                {
                                    //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                    value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                    break;
                                }
                            case 2:
                                {
                                    //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                    value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                    break;
                                }
                            case 4:
                                {
                                    //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                    value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                    break;
                                }
                            default :
                                {
                                    break;
                                }
                        }

                        fprintf(outFilePtr, "Value=%d\n", value);
                    }
                }
                break;
            }

        case PHY_CELL_CONFIG_REQUEST:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_phyCellConfigRequest_st *msg_body =
                    (FAPI_phyCellConfigRequest_st *)&l1ApiMsg_p->msgBody[0];
                fprintf(outFilePtr, "Number of TLVs %d\n", msg_body->numOfTlv);
    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfTlv; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cellConfig_st *tlv = (FAPI_cellConfig_st *)&msg_body->configtlvs[i];
                    fprintf(outFilePtr, "\tTag=%s(%d), ",
                            (tlv->tag >=
                             sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                            "Invalid Tag" : fapiL1Tags[tlv->tag],
                            tlv->tag);

                    UInt8 *paramValue = (UInt8 *)&tlv->configParam;

                    fprintf(outFilePtr, "Length (in bytes)=%d, ", tlv->tagLen);
                    switch(tlv->tagLen)
                    {
                        //p_value = &tlv->configParam;
                        case 1:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                break;
                            }
                        case 2:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                break;
                            }
                        case 4:
                            {
                                //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                break;
                            }
                        default :
                            {
                                break;
                            }
                    }

                    fprintf(outFilePtr, "Value=%d\n", value);
                }
                break;
            }

        case PHY_CELL_CONFIG_RESPONSE:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_phyCellConfigResp_st *msg_body = (FAPI_phyCellConfigResp_st *)&l1ApiMsg_p->msgBody[0];
                fprintf(outFilePtr, "Error Code %s (%d)\n",
                        fapiL1ErrorCodes[msg_body->errorCode], msg_body->errorCode);
                fprintf(outFilePtr, "Number of Invalid or Unsupported TLVs %d\n", msg_body->numOfInvalidOrUnsupportedTLV);
                fprintf(outFilePtr, "Number of Missing TLVs %d\n", msg_body->numOfMissingTLV);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfInvalidOrUnsupportedTLV; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cellConfig_st *tlv = (FAPI_cellConfig_st *)&msg_body->listOfTLV[i];
                    fprintf(outFilePtr, "\tTag=%s(%d), ",
                            (tlv->tag >=
                             sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                            "Invalid Tag" : fapiL1Tags[tlv->tag],
                            tlv->tag);

                    fprintf(outFilePtr, "Length (in bytes)=%d, ", tlv->tagLen);
                    UInt8 *paramValue = (UInt8 *)&tlv->configParam;

                    switch(tlv->tagLen)
                    {
                        //p_value = &tlv->configParam;
                        case 1:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                break;
                            }
                        case 2:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                break;
                            }
                        case 4:
                            {
                                //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                break;
                            }
                        default :
                            {
                                break;
                            }
                    }

                    fprintf(outFilePtr, "Value=%d\n", value);
                }

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfMissingTLV; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cellConfig_st *tlv = (FAPI_cellConfig_st *)&msg_body->listOfMissingTlv[i];
                    fprintf(outFilePtr, "\tTag=%s(%d), ",
                            (tlv->tag >=
                             sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                            "Invalid Tag" : fapiL1Tags[tlv->tag],
                            tlv->tag);

                    fprintf(outFilePtr, "Length (in bytes)=%d, ", tlv->tagLen);
                    UInt8 *paramValue = (UInt8 *)&tlv->configParam;

                    switch(tlv->tagLen)
                    {
                        //p_value = &tlv->configParam;
                        case 1:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                break;
                            }
                        case 2:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                break;
                            }
                        case 4:
                            {
                                //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                break;
                            }
                        default :
                            {
                                break;
                            }
                    }

                    fprintf(outFilePtr, "Value=%d\n", value);
                }
                break;
            }

        case PHY_START_REQUEST:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                break;
            }

        case PHY_STOP_REQUEST:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                break;
            }

        case PHY_STOP_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                break;
            }

        case PHY_UE_CONFIG_REQUEST:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_ueConfigRequest_st *msg_body =
                    (FAPI_ueConfigRequest_st *)&l1ApiMsg_p->msgBody[0];
                fprintf(outFilePtr, "Number of TLVs %d\n", msg_body->numOfTlv);
    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfTlv; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cellConfig_st *tlv = (FAPI_cellConfig_st *)&msg_body->tlvs[i];
                    fprintf(outFilePtr, "\tTag=%s(%d), ",
                            (tlv->tag >=
                             sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                            "Invalid Tag" : fapiL1Tags[tlv->tag],
                            tlv->tag);

                    fprintf(outFilePtr, "Length (in bytes)=%d, ", tlv->tagLen);
                    UInt8 *paramValue = (UInt8 *)&tlv->configParam;

                    switch(tlv->tagLen)
                    {
                        //p_value = &tlv->configParam;
                        case 1:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                break;
                            }
                        case 2:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                break;
                            }
                        case 4:
                            {
                                //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                break;
                            }
                        default :
                            {
                                break;
                            }
                    }

                    fprintf(outFilePtr, "Value=%d\n", value);
                }
                break;
            }

        case PHY_UE_CONFIG_RESPONSE:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_phyUeConfigResp_st *msg_body = (FAPI_phyUeConfigResp_st *)&l1ApiMsg_p->msgBody[0];
                fprintf(outFilePtr, "Error Code %s (%d)\n",
                        fapiL1ErrorCodes[msg_body->errorCode], msg_body->errorCode);
    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfInvalidOrUnsupportedTLV; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cellConfig_st *tlv = (FAPI_cellConfig_st *)&msg_body->listOfTLV[i];
                    fprintf(outFilePtr, "\tTag=%s(%d), ",
                            (tlv->tag >=
                             sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                            "Invalid Tag" : fapiL1Tags[tlv->tag],
                            tlv->tag);

                    fprintf(outFilePtr, "Length (in bytes)=%d, ", tlv->tagLen);
                    UInt8 *paramValue = (UInt8 *)&tlv->configParam;

                    switch(tlv->tagLen)
                    {
                        //p_value = &tlv->configParam;
                        case 1:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                break;
                            }
                        case 2:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                break;
                            }
                        case 4:
                            {
                                //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                break;
                            }
                        default :
                            {
                                break;
                            }
                    }

                    fprintf(outFilePtr, "Value=%d\n", value);
                }

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfMissingTLV; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cellConfig_st *tlv = (FAPI_cellConfig_st *)&msg_body->listOfMissingTlv[i];
                    fprintf(outFilePtr, "\tTag=%s(%d), ",
                            (tlv->tag >=
                             sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                            "Invalid Tag" : fapiL1Tags[tlv->tag],
                            tlv->tag);

                    fprintf(outFilePtr, "Length (in bytes)=%d, ", tlv->tagLen);
                    UInt8 *paramValue = (UInt8 *)&tlv->configParam;

                    switch(tlv->tagLen)
                    {
                        //p_value = &tlv->configParam;
                        case 1:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                break;
                            }
                        case 2:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                break;
                            }
                        case 4:
                            {
                                //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                break;
                            }
                        default :
                            {
                                break;
                            }
                    }

                    fprintf(outFilePtr, "Value=%d\n", value);
                }
                break;
            }

        case PHY_ERROR_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_phyErrorIndication_st *msg_body = (FAPI_phyErrorIndication_st *)&l1ApiMsg_p->msgBody[0];
                fprintf(outFilePtr, "Error Code %s (%d)\n",
                        fapiL1ErrorCodes[msg_body->errorCode], msg_body->errorCode);
                switch(msg_body->errorCode)
                {
                    case FAPI_SFN_OUT_OF_SYNC:
                    case FAPI_MSG_INVALID_SFN:
                        {
                            FAPI_errMsgBody1_st errorMsgBody =
                                msg_body->errMsgBody.msgBody1;
                            fprintf(outFilePtr, "Received SFN/SF - \n");
                            printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(errorMsgBody.recvdSfnSf));
                            fprintf(outFilePtr, "Expected SFN/SF - \n");
                            printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(errorMsgBody.expectedSfnSf));
                            break;
                        }

                    case FAPI_MSG_SUBFRAME_ERR:
                    case FAPI_MSG_INVALID_CONFIG:
                        {
                            FAPI_errMsgBody2_st errorMsgBody =
                                msg_body->errMsgBody.msgBody2;
                            fprintf(outFilePtr, "Sub Error Code %d\n", errorMsgBody.subErrCode);
                            fprintf(outFilePtr, "Direction %d\n", errorMsgBody.direction);
                            fprintf(outFilePtr, "RNTI %d\n", MAC_PHY_CONVERT_16(errorMsgBody.rnti));
                            fprintf(outFilePtr, "PDU Type %d\n", errorMsgBody.pduType);
                            break;
                        }

                    case FAPI_MSG_HI_ERR:
                        {
                            FAPI_errMsgBody3_st errorMsgBody =
                                msg_body->errMsgBody.msgBody3;
                            fprintf(outFilePtr, "Sub Error Code %d\n", errorMsgBody.subErrCode);
                            fprintf(outFilePtr, "PHICH Lowest UL RB Index %d\n", errorMsgBody.phichLowestulRbIndex);
                            break;
                        }

                    case FAPI_MSG_TX_ERR:
                        {
                            FAPI_errMsgBody4_st errorMsgBody =
                                msg_body->errMsgBody.msgBody4;
                            fprintf(outFilePtr, "Sub Error Code %d\n", errorMsgBody.subErrCode);
                            fprintf(outFilePtr, "PDU Index %d\n", MAC_PHY_CONVERT_16(errorMsgBody.pduIndex));
                            break;
                        }
                    case FAPI_MSG_INVALID_STATE:
                    case FAPI_MSG_BCH_MISSING:
                    case FAPI_MSG_OK:
                        break;
                }

                break;
            }

        case PHY_UE_RELEASE_REQ:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
#ifdef FAPI_4_0_COMPLIANCE
                FAPI_ueReleaseRequest_st *msg_body =
                    (FAPI_ueReleaseRequest_st *)&l1ApiMsg_p->msgBody[0];
                fprintf(outFilePtr, "Number of TLVs %d\n", msg_body->numOfTlv);
    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfTlv; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cellConfig_st *tlv = (FAPI_cellConfig_st *)&msg_body->tlvs[i];
                    fprintf(outFilePtr, "\tTag=%s(%d), ",
                            (tlv->tag >=
                             sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                            "Invalid Tag" : fapiL1Tags[tlv->tag],
                            tlv->tag);

                    UInt8 *paramValue = (UInt8 *)&tlv->configParam;

                    switch(tlv->tagLen)
                    {
                        //p_value = &tlv->configParam;
                        case 1:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                break;
                            }
                        case 2:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                break;
                            }
                        case 4:
                            {
                                //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                break;
                            }
                        default :
                            {
                                break;
                            }
                    }

                    fprintf(outFilePtr, "Value=%d\n", value);
                }
#endif          
                break;
            }

        case PHY_UE_RELEASE_RESP:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);

#ifdef FAPI_4_0_COMPLIANCE
                FAPI_phyUeReleaseResp_st *msg_body = (FAPI_phyUeReleaseResp_st *)&l1ApiMsg_p->msgBody[0];
                fprintf(outFilePtr, "Error Code %s (%d)\n",
                        fapiL1ErrorCodes[msg_body->errorCode], msg_body->errorCode);
    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfInvalidOrUnsupportedTLV; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cellConfig_st *tlv = (FAPI_cellConfig_st *)&msg_body->listOfTLV[i];
                    fprintf(outFilePtr, "\tTag=%s(%d), ",
                            (tlv->tag >=
                             sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                            "Invalid Tag" : fapiL1Tags[tlv->tag],
                            tlv->tag);

                    UInt8 *paramValue = (UInt8 *)&tlv->configParam;
                    fprintf(outFilePtr, "Length (in bytes)=%d, ", tlv->tagLen);

                    switch(tlv->tagLen)
                    {
                        //p_value = &tlv->configParam;
                        case 1:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                break;
                            }
                        case 2:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                break;
                            }
                        case 4:
                            {
                                //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                break;
                            }
                        default :
                            {
                                break;
                            }
                    }

                    fprintf(outFilePtr, "Value=%d\n", value);
                }

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfMissingTLV; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cellConfig_st *tlv = (FAPI_cellConfig_st *)&msg_body->listOfMissingTlv[i];
                    fprintf(outFilePtr, "\tTag=%s(%d), ",
                            (tlv->tag >=
                             sizeof(fapiL1Tags)/sizeof(fapiL1Tags[0])) ?
                            "Invalid Tag" : fapiL1Tags[tlv->tag],
                            tlv->tag);

                    UInt8 *paramValue = (UInt8 *)&tlv->configParam;

                    fprintf(outFilePtr, "Length (in bytes)=%d, ", tlv->tagLen);
                    switch(tlv->tagLen)
                    {
                        //p_value = &tlv->configParam;
                        case 1:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt8)(*(UInt8 *)paramValue));
                                break;
                            }
                        case 2:
                            {
                                //value = MAC_PHY_CONVERT_16((UInt16)(*((UInt16 *)p_value)));
                                value = MAC_PHY_CONVERT_16((UInt16)(*(UInt16 *)paramValue));
                                break;
                            }
                        case 4:
                            {
                                //value = //MAC_PHY_CONVERT_32((UInt32)(*((UInt32 *)p_value)));
                                value = MAC_PHY_CONVERT_32((UInt32)(*(UInt32 *)paramValue));
                                break;
                            }
                        default :
                            {
                                break;
                            }
                    }

                    fprintf(outFilePtr, "Value=%d\n", value);
                }
#endif          
                break;
            }

        case PHY_DL_CONFIG_REQUEST:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_dlConfigRequest_st *msg_body =
                    (FAPI_dlConfigRequest_st *)&l1ApiMsg_p->msgBody[0];

                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Length %d\n", MAC_PHY_CONVERT_16(msg_body->length));
                //fprintf(outFilePtr, "Number of PDCCH  %d\n", msg_body->cfi);
                fprintf(outFilePtr, "Number of PDCCH OFDM symbols %d\n", msg_body->cfi);
                fprintf(outFilePtr, "Number of DCIs %d\n", msg_body->numDCI);
                fprintf(outFilePtr, "Number of  PDUs %d\n", msg_body->numOfPDU);
                fprintf(outFilePtr, "Number of PDSCH RNTIs %d\n", msg_body->numOfPDSCHRNTI);
                fprintf(outFilePtr, "Transmission power for PCFICH %d\n", msg_body->txPowerForPCFICH);
                FAPI_dlConfigPDUInfo_st *pduInfo = (FAPI_dlConfigPDUInfo_st *)msg_body->dlConfigpduInfo;
    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfPDU; i++)
    /* SPR 17884 FIX END */
                {
                    fprintf(outFilePtr, "{\n\tPDU Type %s (%d)\n",
                            (pduInfo->pduType >=
                             sizeof(fapiL1DlPduTypes)/sizeof(fapiL1DlPduTypes[0])) ?
                            "Invalid PDU" : fapiL1DlPduTypes[pduInfo->pduType],
                            pduInfo->pduType);

                    fprintf(outFilePtr, "\tPDU Size %d\n", pduInfo->pduSize);

                    switch(pduInfo->pduType)
                    {
                        case FAPI_DCI_DL_PDU:
                            {
                                FAPI_dciDLPduInfo_st *pdu =
                                    (FAPI_dciDLPduInfo_st *)&pduInfo->dlConfigpduInfo.DCIPdu;
                                fprintf(outFilePtr, "\tDCI Format %d\n", pdu->dciFormat);
                                fprintf(outFilePtr, "\tCCE Index %d\n", pdu->cceIndex);
                                fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                                switch(pdu->dciFormat)
                                {
                                    case FAPI_DL_DCI_FORMAT_1:
                                        {
                                            FAPI_dciFormat1_st *pdu1 =
                                                (FAPI_dciFormat1_st *)&pdu->dciPdu[0];
                                            fprintf(outFilePtr, "\t\tAggregation Level %d\n",
                                                    pdu1->aggregationLevel);
                                            fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                                    pdu1->resAllocationType);
                                            fprintf(outFilePtr, "\t\tMCS_1 %d\n",
                                                    pdu1->mcs_1);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_1 %d\n",
                                                    pdu1->redundancyVersion_1);
                                            fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                                    pdu1->rbCoding);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_1 %d\n",
                                                    pdu1->newDataIndicator_1);
                                            fprintf(outFilePtr, "\t\tHARQ Process %d\n",
                                                    pdu1->harqProcessNum);
                                            fprintf(outFilePtr, "\t\tTPC %d\n",
                                                    pdu1->tpc);
                                            fprintf(outFilePtr, "\t\tDownlink Assignment Index %d\n",
                                                    pdu1->dlAssignmentIndex);
                                            fprintf(outFilePtr, "\t\tTransmission power %d\n",
                                                    MAC_PHY_CONVERT_16(pdu1->txPower));
                                            fprintf(outFilePtr, "\t\tRNTI type %d\n",
                                                    pdu1->rntiType);
#ifdef FAPI_4_0_COMPLIANCE
                                            fprintf(outFilePtr, "\t\tRelease 9 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tMCCH flag %d\n",
                                                    pdu1->release9Param.mcchFlag);
                                            fprintf(outFilePtr, "\t\t\tMCCH Change Notification %d\n",
                                                    pdu1->release9Param.mcchChangeNotification);
                                            fprintf(outFilePtr, "\t\t\tScrambling Identity %d\n",
                                                    pdu1->release9Param.scramblingIndentity);
                                            fprintf(outFilePtr, "\t\tRelease 10 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tCross carrier scheduling flag %d\n",
                                                    pdu1->release10Param.crossCarrierSchedulingFlag);
                                            fprintf(outFilePtr, "\t\t\tCarrier Indicator %d\n", pdu1->release10Param.carrierIndicator);
                                            fprintf(outFilePtr, "\t\t\tSRS flag %d\n",
                                                    pdu1->release10Param.srsFlag);
                                            fprintf(outFilePtr, "\t\t\tSRS request %d\n",
                                                    pdu1->release10Param.srsRequest);
                                            fprintf(outFilePtr, "\t\t\tAntenna ports, Scrambling and Layers %d\n",
                                                    pdu1->release10Param.antennaPortScramblingAndLayers);
#endif
                                            break;
                                        }

                                    case FAPI_DL_DCI_FORMAT_1A:
                                        {
                                            FAPI_dciFormat1A_st *pdu1 =
                                                (FAPI_dciFormat1A_st *)&pdu->dciPdu[0];
                                            fprintf(outFilePtr, "\t\tAggregation Level %d\n",
                                                    pdu1->aggregationLevel);
                                            fprintf(outFilePtr, "\t\tVirtual resource block assignment flag %d\n",
                                                    pdu1->vRBAssignmentFlag);
                                            fprintf(outFilePtr, "\t\tMCS_1 %d\n",
                                                    pdu1->mcs_1);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_1 %d\n",
                                                    pdu1->redundancyVersion_1);
                                            fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                                    pdu1->rbCoding);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_1 %d\n",
                                                    pdu1->newDataIndicator_1);
                                            fprintf(outFilePtr, "\t\tHARQ Process %d\n",
                                                    pdu1->harqProcessNum);
                                            fprintf(outFilePtr, "\t\tTPC %d\n",
                                                    pdu1->tpc);
                                            fprintf(outFilePtr, "\t\tDownlink Assignment Index %d\n",
                                                    pdu1->dlAssignmentIndex);
                                            fprintf(outFilePtr, "\t\tAllocate PRACH flag %d\n",
                                                    pdu1->allocatePRACHFlag);
                                            fprintf(outFilePtr, "\t\tPreamble Index %d\n",
                                                    pdu1->preambleIndex);
                                            fprintf(outFilePtr, "\t\tTransmission power %d\n",
                                                    MAC_PHY_CONVERT_16(pdu1->txPower));
                                            fprintf(outFilePtr, "\t\tPRACH Mask Index %d\n",
                                                    pdu1->pRACHMaskIndex);
                                            fprintf(outFilePtr, "\t\tRNTI type %d\n",
                                                    pdu1->rntiType);
#ifdef FAPI_4_0_COMPLIANCE
                                            fprintf(outFilePtr, "\t\tRelease 9 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tMCCH flag %d\n",
                                                    pdu1->release9Param.mcchFlag);
                                            fprintf(outFilePtr, "\t\t\tMCCH Change Notification %d\n",
                                                    pdu1->release9Param.mcchChangeNotification);
                                            fprintf(outFilePtr, "\t\t\tScrambling Identity %d\n",
                                                    pdu1->release9Param.scramblingIndentity);
                                            fprintf(outFilePtr, "\t\tRelease 10 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tCross carrier scheduling flag %d\n",
                                                    pdu1->release10Param.crossCarrierSchedulingFlag);
                                            fprintf(outFilePtr, "\t\t\tCarrier Indicator %d\n", pdu1->release10Param.carrierIndicator);
                                            fprintf(outFilePtr, "\t\t\tSRS flag %d\n",
                                                    pdu1->release10Param.srsFlag);
                                            fprintf(outFilePtr, "\t\t\tSRS request %d\n",
                                                    pdu1->release10Param.srsRequest);
                                            fprintf(outFilePtr, "\t\t\tAntenna ports, Scrambling and Layers %d\n",
                                                    pdu1->release10Param.antennaPortScramblingAndLayers);
#endif
                                            break;
                                        }

                                    case FAPI_DL_DCI_FORMAT_1B:
                                        {
                                            FAPI_dciFormat1B_st *pdu1 =
                                                (FAPI_dciFormat1B_st *)&pdu->dciPdu[0];
                                            fprintf(outFilePtr, "\t\tAggregation Level %d\n",
                                                    pdu1->aggregationLevel);
                                            fprintf(outFilePtr, "\t\tVirtual resource block assignment flag %d\n",
                                                    pdu1->vRBAssignmentFlag);
                                            fprintf(outFilePtr, "\t\tMCS_1 %d\n",
                                                    pdu1->mcs_1);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_1 %d\n",
                                                    pdu1->redundancyVersion_1);
                                            fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                                    pdu1->rbCoding);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_1 %d\n",
                                                    pdu1->newDataIndicator_1);
                                            fprintf(outFilePtr, "\t\tHARQ Process %d\n",
                                                    pdu1->harqProcessNum);
                                            fprintf(outFilePtr, "\t\tTPMI %d\n",
                                                    pdu1->tPMI);
                                            fprintf(outFilePtr, "\t\tPMI %d\n",
                                                    pdu1->pmi);
                                            fprintf(outFilePtr, "\t\tTPC %d\n",
                                                    pdu1->tpc);
                                            fprintf(outFilePtr, "\t\tDownlink Assignment Index %d\n",
                                                    pdu1->dlAssignmentIndex);
                                            fprintf(outFilePtr, "\t\tTransmission power %d\n",
                                                    MAC_PHY_CONVERT_16(pdu1->txPower));
                                            fprintf(outFilePtr, "\t\tNGAP %d\n",
                                                    pdu1->nGAP);
#ifdef FAPI_4_0_COMPLIANCE
                                            fprintf(outFilePtr, "\t\tRelease 9 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tMCCH flag %d\n",
                                                    pdu1->release9Param.mcchFlag);
                                            fprintf(outFilePtr, "\t\t\tMCCH Change Notification %d\n",
                                                    pdu1->release9Param.mcchChangeNotification);
                                            fprintf(outFilePtr, "\t\t\tScrambling Identity %d\n",
                                                    pdu1->release9Param.scramblingIndentity);
                                            fprintf(outFilePtr, "\t\tRelease 10 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tCross carrier scheduling flag %d\n",
                                                    pdu1->release10Param.crossCarrierSchedulingFlag);
                                            fprintf(outFilePtr, "\t\t\tCarrier Indicator %d\n", pdu1->release10Param.carrierIndicator);
                                            fprintf(outFilePtr, "\t\t\tSRS flag %d\n",
                                                    pdu1->release10Param.srsFlag);
                                            fprintf(outFilePtr, "\t\t\tSRS request %d\n",
                                                    pdu1->release10Param.srsRequest);
                                            fprintf(outFilePtr, "\t\t\tAntenna ports, Scrambling and Layers %d\n",
                                                    pdu1->release10Param.antennaPortScramblingAndLayers);
#endif
                                            break;
                                        }

                                    case FAPI_DL_DCI_FORMAT_1C:
                                        {
                                            FAPI_dciFormat1C_st *pdu1 =
                                                (FAPI_dciFormat1C_st *)&pdu->dciPdu[0];
                                            fprintf(outFilePtr, "\t\tAggregation Level %d\n",
                                                    pdu1->aggregationLevel);
                                            fprintf(outFilePtr, "\t\tMCS_1 %d\n",
                                                    pdu1->mcs_1);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_1 %d\n",
                                                    pdu1->redundancyVersion_1);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_1 %d\n",
                                                    pdu1->newDataIndicator_1);
                                            fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                                    pdu1->rbCoding);
                                            fprintf(outFilePtr, "\t\tNGAP %d\n",
                                                    pdu1->nGAP);
                                            fprintf(outFilePtr, "\t\tTransport block size index %d\n",
                                                    pdu1->tbSizeIndex);
                                            fprintf(outFilePtr, "\t\tTransmission power %d\n",
                                                    MAC_PHY_CONVERT_16(pdu1->txPower));
#ifdef FAPI_4_0_COMPLIANCE
                                            fprintf(outFilePtr, "\t\tRelease 9 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tMCCH flag %d\n",
                                                    pdu1->release9Param.mcchFlag);
                                            fprintf(outFilePtr, "\t\t\tMCCH Change Notification %d\n",
                                                    pdu1->release9Param.mcchChangeNotification);
                                            fprintf(outFilePtr, "\t\t\tScrambling Identity %d\n",
                                                    pdu1->release9Param.scramblingIndentity);
                                            fprintf(outFilePtr, "\t\tRelease 10 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tCross carrier scheduling flag %d\n",
                                                    pdu1->release10Param.crossCarrierSchedulingFlag);
                                            fprintf(outFilePtr, "\t\t\tCarrier Indicator %d\n", pdu1->release10Param.carrierIndicator);
                                            fprintf(outFilePtr, "\t\t\tSRS flag %d\n",
                                                    pdu1->release10Param.srsFlag);
                                            fprintf(outFilePtr, "\t\t\tSRS request %d\n",
                                                    pdu1->release10Param.srsRequest);
                                            fprintf(outFilePtr, "\t\t\tAntenna ports, Scrambling and Layers %d\n",
                                                    pdu1->release10Param.antennaPortScramblingAndLayers);
#endif
                                            break;
                                        }

                                    case FAPI_DL_DCI_FORMAT_1D:
                                        {
                                            FAPI_dciFormat1D_st *pdu1 =
                                                (FAPI_dciFormat1D_st *)&pdu->dciPdu[0];
                                            fprintf(outFilePtr, "\t\tAggregation Level %d\n",
                                                    pdu1->aggregationLevel);
                                            fprintf(outFilePtr, "\t\tVirtual resource block assignment flag %d\n",
                                                    pdu1->vRBAssignmentFlag);
                                            fprintf(outFilePtr, "\t\tMCS_1 %d\n",
                                                    pdu1->mcs_1);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_1 %d\n",
                                                    pdu1->redundancyVersion_1);
                                            fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                                    pdu1->rbCoding);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_1 %d\n",
                                                    pdu1->newDataIndicator_1);
                                            fprintf(outFilePtr, "\t\tHARQ Process %d\n",
                                                    pdu1->harqProcessNum);
                                            fprintf(outFilePtr, "\t\tTPMI %d\n",
                                                    pdu1->tPMI);
                                            fprintf(outFilePtr, "\t\tTPC %d\n",
                                                    pdu1->tpc);
                                            fprintf(outFilePtr, "\t\tDownlink Assignment Index %d\n",
                                                    pdu1->dlAssignmentIndex);
                                            fprintf(outFilePtr, "\t\tNGAP %d\n",
                                                    pdu1->nGAP);
                                            fprintf(outFilePtr, "\t\tTransmission power %d\n",
                                                    MAC_PHY_CONVERT_16(pdu1->txPower));
                                            fprintf(outFilePtr, "\t\tDownlink power offset %d\n",
                                                    pdu1->dlPowerOffset);
#ifdef FAPI_4_0_COMPLIANCE
                                            fprintf(outFilePtr, "\t\tRelease 9 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tMCCH flag %d\n",
                                                    pdu1->release9Param.mcchFlag);
                                            fprintf(outFilePtr, "\t\t\tMCCH Change Notification %d\n",
                                                    pdu1->release9Param.mcchChangeNotification);
                                            fprintf(outFilePtr, "\t\t\tScrambling Identity %d\n",
                                                    pdu1->release9Param.scramblingIndentity);
                                            fprintf(outFilePtr, "\t\tRelease 10 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tCross carrier scheduling flag %d\n",
                                                    pdu1->release10Param.crossCarrierSchedulingFlag);
                                            fprintf(outFilePtr, "\t\t\tCarrier Indicator %d\n", pdu1->release10Param.carrierIndicator);
                                            fprintf(outFilePtr, "\t\t\tSRS flag %d\n",
                                                    pdu1->release10Param.srsFlag);
                                            fprintf(outFilePtr, "\t\t\tSRS request %d\n",
                                                    pdu1->release10Param.srsRequest);
                                            fprintf(outFilePtr, "\t\t\tAntenna ports, Scrambling and Layers %d\n",
                                                    pdu1->release10Param.antennaPortScramblingAndLayers);
#endif
                                            break;
                                        }

                                    case FAPI_DL_DCI_FORMAT_2:
                                        {
                                            FAPI_dciFormat2_st *pdu1 =
                                                (FAPI_dciFormat2_st *)&pdu->dciPdu[0];
                                            fprintf(outFilePtr, "\t\tAggregation Level %d\n",
                                                    pdu1->aggregationLevel);
                                            fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                                    pdu1->resAllocationType);
                                            fprintf(outFilePtr, "\t\tMCS_1 %d\n",
                                                    pdu1->mcs_1);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_1 %d\n",
                                                    pdu1->redundancyVersion_1);
                                            fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                                    pdu1->rbCoding);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_1 %d\n",
                                                    pdu1->newDataIndicator_1);
                                            fprintf(outFilePtr, "\t\tTransport block to codeword swap flag%d\n",
                                                    pdu1->tbToCodeWordSwapFlag);
                                            fprintf(outFilePtr, "\t\tMCS_2 %d\n",
                                                    pdu1->mcs_2);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_2 %d\n",
                                                    pdu1->redundancyVersion_2);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_2 %d\n",
                                                    pdu1->newDataIndicator_2);
                                            fprintf(outFilePtr, "\t\tHARQ Process %d\n",
                                                    pdu1->harqProcessNum);
                                            fprintf(outFilePtr, "\t\tPrecoding Information %d\n",
                                                    pdu1->preCodingInfo);
                                            fprintf(outFilePtr, "\t\tTPC %d\n",
                                                    pdu1->tpc);
                                            fprintf(outFilePtr, "\t\tTransmission power %d\n",
                                                    MAC_PHY_CONVERT_16(pdu1->txPower));
                                            fprintf(outFilePtr, "\t\tDownlink Assignment Index %d\n",
                                                    pdu1->dlAssignmentIndex);
                                            fprintf(outFilePtr, "\t\tRNTI type %d\n",
                                                    pdu1->rntiType);
#ifdef FAPI_4_0_COMPLIANCE
                                            fprintf(outFilePtr, "\t\tRelease 9 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tMCCH flag %d\n",
                                                    pdu1->release9Param.mcchFlag);
                                            fprintf(outFilePtr, "\t\t\tMCCH Change Notification %d\n",
                                                    pdu1->release9Param.mcchChangeNotification);
                                            fprintf(outFilePtr, "\t\t\tScrambling Identity %d\n",
                                                    pdu1->release9Param.scramblingIndentity);
                                            fprintf(outFilePtr, "\t\tRelease 10 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tCross carrier scheduling flag %d\n",
                                                    pdu1->release10Param.crossCarrierSchedulingFlag);
                                            fprintf(outFilePtr, "\t\t\tCarrier Indicator %d\n", pdu1->release10Param.carrierIndicator);
                                            fprintf(outFilePtr, "\t\t\tSRS flag %d\n",
                                                    pdu1->release10Param.srsFlag);
                                            fprintf(outFilePtr, "\t\t\tSRS request %d\n",
                                                    pdu1->release10Param.srsRequest);
                                            fprintf(outFilePtr, "\t\t\tAntenna ports, Scrambling and Layers %d\n",
                                                    pdu1->release10Param.antennaPortScramblingAndLayers);
#endif
                                            break;
                                        }

                                    case FAPI_DL_DCI_FORMAT_2A:
                                        {
                                            FAPI_dciFormat2A_st *pdu1 =
                                                (FAPI_dciFormat2A_st *)&pdu->dciPdu[0];
                                            fprintf(outFilePtr, "\t\tAggregation Level %d\n",
                                                    pdu1->aggregationLevel);
                                            fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                                    pdu1->resAllocationType);
                                            fprintf(outFilePtr, "\t\tMCS_1 %d\n",
                                                    pdu1->mcs_1);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_1 %d\n",
                                                    pdu1->redundancyVersion_1);
                                            fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                                    pdu1->rbCoding);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_1 %d\n",
                                                    pdu1->newDataIndicator_1);
                                            fprintf(outFilePtr, "\t\tTransport block to codeword swap flag%d\n",
                                                    pdu1->tbToCodeWordSwapFlag);
                                            fprintf(outFilePtr, "\t\tMCS_2 %d\n",
                                                    pdu1->mcs_2);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_2 %d\n",
                                                    pdu1->redundancyVersion_2);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_2 %d\n",
                                                    pdu1->newDataIndicator_2);
                                            fprintf(outFilePtr, "\t\tHARQ Process %d\n",
                                                    pdu1->harqProcessNum);
                                            fprintf(outFilePtr, "\t\tPrecoding Information %d\n",
                                                    pdu1->preCodingInfo);
                                            fprintf(outFilePtr, "\t\tTPC %d\n",
                                                    pdu1->tpc);
                                            fprintf(outFilePtr, "\t\tTransmission power %d\n",
                                                    MAC_PHY_CONVERT_16(pdu1->txPower));
                                            fprintf(outFilePtr, "\t\tDownlink Assignment Index %d\n",
                                                    pdu1->dlAssignmentIndex);
                                            fprintf(outFilePtr, "\t\tRNTI type %d\n",
                                                    pdu1->rntiType);
#ifdef FAPI_4_0_COMPLIANCE
                                            fprintf(outFilePtr, "\t\tRelease 9 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tMCCH flag %d\n",
                                                    pdu1->release9Param.mcchFlag);
                                            fprintf(outFilePtr, "\t\t\tMCCH Change Notification %d\n",
                                                    pdu1->release9Param.mcchChangeNotification);
                                            fprintf(outFilePtr, "\t\t\tScrambling Identity %d\n",
                                                    pdu1->release9Param.scramblingIndentity);
                                            fprintf(outFilePtr, "\t\tRelease 10 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tCross carrier scheduling flag %d\n",
                                                    pdu1->release10Param.crossCarrierSchedulingFlag);
                                            fprintf(outFilePtr, "\t\t\tCarrier Indicator %d\n", pdu1->release10Param.carrierIndicator);
                                            fprintf(outFilePtr, "\t\t\tSRS flag %d\n",
                                                    pdu1->release10Param.srsFlag);
                                            fprintf(outFilePtr, "\t\t\tSRS request %d\n",
                                                    pdu1->release10Param.srsRequest);
                                            fprintf(outFilePtr, "\t\t\tAntenna ports, Scrambling and Layers %d\n",
                                                    pdu1->release10Param.antennaPortScramblingAndLayers);
#endif
                                            break;
                                        }

                                    case FAPI_DL_DCI_FORMAT_2B:
                                        {
                                            FAPI_dciFormat2B_st *pdu1 =
                                                (FAPI_dciFormat2B_st *)&pdu->dciPdu[0];
                                            fprintf(outFilePtr, "\t\tAggregation Level %d\n",
                                                    pdu1->aggregationLevel);
                                            fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                                    pdu1->resAllocationType);
                                            fprintf(outFilePtr, "\t\tMCS_1 %d\n",
                                                    pdu1->mcs_1);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_1 %d\n",
                                                    pdu1->redundancyVersion_1);
                                            fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                                    pdu1->rbCoding);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_1 %d\n",
                                                    pdu1->newDataIndicator_1);
                                            fprintf(outFilePtr, "\t\tMCS_2 %d\n",
                                                    pdu1->mcs_2);
                                            fprintf(outFilePtr, "\t\tRedundancy Version_2 %d\n",
                                                    pdu1->redundancyVersion_2);
                                            fprintf(outFilePtr, "\t\tNew Data Indicator_2 %d\n",
                                                    pdu1->newDataIndicator_2);
                                            fprintf(outFilePtr, "\t\tHARQ Process %d\n",
                                                    pdu1->harqProcessNum);
                                            fprintf(outFilePtr, "\t\tScrambling Identity %d\n",
                                                    pdu1->scramblingId);
                                            fprintf(outFilePtr, "\t\tTPC %d\n",
                                                    pdu1->tpc);
                                            fprintf(outFilePtr, "\t\tDownlink Assignment Index %d\n",
                                                    pdu1->dlAssignmentIndex);
                                            fprintf(outFilePtr, "\t\tTransmission power %d\n",
                                                    MAC_PHY_CONVERT_16(pdu1->txPower));
                                            fprintf(outFilePtr, "\t\tRNTI type %d\n",
                                                    pdu1->rntiType);
#ifdef FAPI_4_0_COMPLIANCE
                                            fprintf(outFilePtr, "\t\tRelease 9 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tMCCH flag %d\n",
                                                    pdu1->release9Param.mcchFlag);
                                            fprintf(outFilePtr, "\t\t\tMCCH Change Notification %d\n",
                                                    pdu1->release9Param.mcchChangeNotification);
                                            fprintf(outFilePtr, "\t\t\tScrambling Identity %d\n",
                                                    pdu1->release9Param.scramblingIndentity);
                                            fprintf(outFilePtr, "\t\tRelease 10 Parameters - \n");
                                            fprintf(outFilePtr, "\t\t\tCross carrier scheduling flag %d\n",
                                                    pdu1->release10Param.crossCarrierSchedulingFlag);
                                            fprintf(outFilePtr, "\t\t\tCarrier Indicator %d\n", pdu1->release10Param.carrierIndicator);
                                            fprintf(outFilePtr, "\t\t\tSRS flag %d\n",
                                                    pdu1->release10Param.srsFlag);
                                            fprintf(outFilePtr, "\t\t\tSRS request %d\n",
                                                    pdu1->release10Param.srsRequest);
                                            fprintf(outFilePtr, "\t\t\tAntenna ports, Scrambling and Layers %d\n",
                                                    pdu1->release10Param.antennaPortScramblingAndLayers);
#endif
                                            break;
                                        }
                                }
                                break;
                            }

                        case FAPI_BCH_PDU:
                            {
                                FAPI_bchConfigPDUInfo_st *pdu =
                                    (FAPI_bchConfigPDUInfo_st *)&pduInfo->dlConfigpduInfo.BCHPdu;
                                fprintf(outFilePtr, "\tLength %d\n", pdu->bchPduLen);
                                fprintf(outFilePtr, "\tPDU index %d\n", MAC_PHY_CONVERT_16(pdu->pduIndex));
                                fprintf(outFilePtr, "\tTransmission power %d\n", MAC_PHY_CONVERT_16(pdu->txPower));
                                break;
                            }

                        case FAPI_MCH_PDU:
                            {
                                FAPI_mchConfigPDUInfo_st *pdu =
                                    (FAPI_mchConfigPDUInfo_st *)&pduInfo->dlConfigpduInfo.MCHPdu;
                                fprintf(outFilePtr, "\tLength %d\n", pdu->mchPduLen);
                                fprintf(outFilePtr, "\tPDU index %d\n", MAC_PHY_CONVERT_16(pdu->pduIndex));
                                fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                                fprintf(outFilePtr, "\tResource Allocation Type %d\n", pdu->resAllocationType);
                                fprintf(outFilePtr, "\tModulation %d\n", pdu->modulationType);
                                fprintf(outFilePtr, "\tResource Block Coding %d\n", pdu->rbCoding);
                                fprintf(outFilePtr, "\tTransmission power %d\n", MAC_PHY_CONVERT_16(pdu->txPower));
                                break;
                            }

                        case FAPI_DLSCH_PDU:
                            {
                                FAPI_dlSCHConfigPDUInfo_st *pdu =
                                    (FAPI_dlSCHConfigPDUInfo_st *)&pduInfo->dlConfigpduInfo.DlSCHPdu;
                                fprintf(outFilePtr, "\tLength %d\n", pdu->dlschPduLen);
                                fprintf(outFilePtr, "\tPDU index %d\n", MAC_PHY_CONVERT_16(pdu->pduIndex));
                                fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                                fprintf(outFilePtr, "\tResource Allocation Type %d\n", pdu->resAllocationType);
                                fprintf(outFilePtr, "\tVirtual resource block assignment flag %d\n",
                                        pdu->vRBAssignmentFlag);
                                fprintf(outFilePtr, "\tResource Block Coding %d\n", pdu->rbCoding);
                                fprintf(outFilePtr, "\tModulation %d\n", pdu->mcs);
                                fprintf(outFilePtr, "\tRedundancy Version %d\n",
                                        pdu->redundancyVersion);
                                fprintf(outFilePtr, "\tTransport Blocks %d\n",
                                        pdu->transportBlocks);
                                fprintf(outFilePtr, "\tTransport block to codeword swap flag %d\n",
                                        pdu->tbToCodeWordSwapFlag);
                                fprintf(outFilePtr, "\tTransmission Scheme %d\n",
                                        pdu->transmissionScheme);
                                fprintf(outFilePtr, "\tNumber Of Layers %d\n", pdu->numOfLayers);
                                fprintf(outFilePtr, "\tNumber of subbands %d\n",
                                        pdu->numOfSubBand);
                                fprintf(outFilePtr, "\tUE Category Capacity %d\n", pdu->ueCatagoryCapacity);
                                fprintf(outFilePtr, "\tP-A %d\n", pdu->pA);
                                fprintf(outFilePtr, "\tDelta power offset index %d\n",
                                        pdu->deltaPowerOffsetAIndex);
                                fprintf(outFilePtr, "\tNGAP %d\n", pdu->nGap);
                                fprintf(outFilePtr, "\tNPRB %d\n", pdu->nPRB);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\tTransmission Mode %d\n", pdu->transmissionMode);
                                fprintf(outFilePtr, "\tnumBfPRBperSubband %d\n", pdu->numRbPerSubBand);
                                fprintf(outFilePtr, "\tnumBfVector %d\n", pdu->numbfVector);
#else
                                fprintf(outFilePtr, "\tnumBfPRBperSubband %d\n", pdu->numRbPerSubBand);
                                fprintf(outFilePtr, "\tnumBfVector %d\n", pdu->numbfVector);
#endif
#ifdef FAPI_DEVIATIONS
#ifndef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\tNSCID %d\n", pdu->nScId);
#endif
#endif
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\tRelease 9 Parameters - \n");
                                fprintf(outFilePtr, "\t\tNSCID %d\n", pdu->release9Param.nSCID);
                                fprintf(outFilePtr, "\tRelease 10 Parameters - \n");
                                fprintf(outFilePtr, "\t\tCSI-RS flag %d\n", pdu->release10Param.csi_rs_flag);
                                fprintf(outFilePtr, "\t\tCSI-RS Resource Config R10 %d\n",
                                        pdu->release10Param.csi_rs_ResourceConfigR10);
                                fprintf(outFilePtr, "\t\tCSI-RS Zero Tx Power Resource Config Bitmap R10 %d\n",
                                        pdu->release10Param.csi_rs_ZeroTxPowerResConfigBitmapR10);
#endif
    /* SPR 17884 FIX START */
                                for(j = 0; j <pdu->numbfVector; j++)
    /* SPR 17884 FIX END */
                                {
                                    FAPI_beamFormingVectorInfo_st *beam =
                                        (FAPI_beamFormingVectorInfo_st *)&pdu->bfVector[j];
                                    fprintf(outFilePtr, "\t\tsubbandIndex %d\n", beam->subBandIndex);
                                    fprintf(outFilePtr, "\t\tnumAntennas %d\n", beam->numOfAntenna);
    /* SPR 17884 FIX START */
                                    for(k = 0; k <beam->numOfAntenna; k++)
    /* SPR 17884 FIX END */
                                    {
                                        fprintf(outFilePtr, "\t\tBeam forming value %d\n",
                                                (UInt16)beam->bfValue_per_antenna[k]);
                                    }
                                }
    /* SPR 17884 FIX START */
                                for(k = 0; k <pdu->numOfSubBand; k++)
    /* SPR 17884 FIX END */
                                {
                                    fprintf(outFilePtr, "\t\tBeam forming value %d\n",
                                            (UInt8)pdu->subBandInfo[k]);
                                }

                                break;
                            }

                        case FAPI_PCH_PDU:
                            {
                                FAPI_pchPduConfigInfo_st *pdu =
                                    (FAPI_pchPduConfigInfo_st *)&pduInfo->dlConfigpduInfo.PCHPdu;
                                fprintf(outFilePtr, "\tLength %d\n", pdu->pchPduLen);
                                fprintf(outFilePtr, "\tPDU index %d\n", MAC_PHY_CONVERT_16(pdu->pduIndex));
                                fprintf(outFilePtr, "\tP-RNTI %d\n", pdu->pRNTI);
                                fprintf(outFilePtr, "\tResource Allocation Type %d\n", pdu->resAllocationType);
                                fprintf(outFilePtr, "\tVirtual resource block assignment flag %d\n",
                                        pdu->vRBAssignmentFlag);
                                fprintf(outFilePtr, "\tResource Block Coding %d\n", pdu->rbCoding);
                                fprintf(outFilePtr, "\tMCS %d\n", pdu->mcs);
                                fprintf(outFilePtr, "\tRedundancy Version %d\n",
                                        pdu->redundancyVersion);
                                fprintf(outFilePtr, "\tNumber Of Transport Blocks %d\n",
                                        pdu->numOftransportBlocks);
                                fprintf(outFilePtr, "\tTransport block to codeword swap flag %d\n",
                                        pdu->tbToCodeWordSwapFlag);
                                fprintf(outFilePtr, "\tTransmission Scheme %d\n",
                                        pdu->transmissionScheme);
                                fprintf(outFilePtr, "\tNumber Of Layers %d\n", pdu->numOfLayers);
                                fprintf(outFilePtr, "\tCodebook Index %d\n", pdu->codeBookIndex);
                                fprintf(outFilePtr, "\tUE Category Capacity %d\n", pdu->ueCatagoryCapacity);
                                fprintf(outFilePtr, "\tP-A %d\n", pdu->pA);
                                fprintf(outFilePtr, "\tNPRB %d\n", pdu->nPRB);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\tNGAP %d\n", pdu->nGap);
#endif
                                break;
                            }

                        case FAPI_PRS_PDU:
                            {
/* SPR 19288 change start */
                                FAPI_prsPduConfigInfo_st *pdu =
                                    (FAPI_prsPduConfigInfo_st *)&pduInfo->dlConfigpduInfo.PRSPdu;
                                fprintf(outFilePtr, "\tTransmission power %d\n",
                                        pdu->transmissionPower);
/* SPR 19288 change end */
                                break;
                            }

#ifdef FAPI_4_0_COMPLIANCE
                        case FAPI_CSI_RS_PDU:
                            {
                                FAPI_csirs_PduConfigInfo_st *pdu =
                                    (FAPI_csirs_PduConfigInfo_st *)&pduInfo->dlConfigpduInfo.CSIRSPdu;
                                fprintf(outFilePtr, "\tCSI-RS Antenna Port Count R10 %d\n",
                                        pdu->csirs_ResourceConfigR10);
                                fprintf(outFilePtr, "\tCSI-RS Resource Config R10 %d\n", pdu->csirs_antennaPortCountR10);
                                fprintf(outFilePtr, "\tTransmission power %d\n", pdu->transmissionPower);
                                fprintf(outFilePtr, "\tCSI-RS Zero Tx Power Resource Config Bitmap R10 %d\n",
                                        pdu->CsiRsZeroTxPowerBitmapR10);
                                break;
                            }
#endif
                    }
                    fprintf(outFilePtr, "}\n");

                    pduInfo = (FAPI_dlConfigPDUInfo_st *)(((UInt8 *)pduInfo) + pduInfo->pduSize);
                }

                break;
            }

        case PHY_UL_CONFIG_REQUEST:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_ulConfigRequest_st *msg_body =
                    (FAPI_ulConfigRequest_st *)&l1ApiMsg_p->msgBody[0];

                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Length %d\n", msg_body->ulConfigLen);
                fprintf(outFilePtr, "Number of PDUs %d\n", msg_body->numOfPdu);
                fprintf(outFilePtr, "RACH/PRACH Frequency Resources %d\n", msg_body->rachFreqResources);
                fprintf(outFilePtr, "SRS present %d\n", msg_body->srsPresent);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfPdu; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_ulPDUConfigInfo_st *pduInfo =
                        (FAPI_ulPDUConfigInfo_st *)&msg_body->ulPduConfigInfo[i];

                    fprintf(outFilePtr, "{\n\tPDU Type %s (%d)\n",
                            (pduInfo->ulConfigPduType >= sizeof(fapiL1UlPduTypes)/sizeof(fapiL1UlPduTypes[0])) ?
                            "Invalid PDU Type" : fapiL1UlPduTypes[pduInfo->ulConfigPduType],
                            pduInfo->ulConfigPduType);
                    fprintf(outFilePtr, "\tPDU Size %d\n", pduInfo->ulConfigPduSize);

                    switch(pduInfo->ulConfigPduType)
                    {
                        case FAPI_ULSCH :
                            {
                                FAPI_ulSCHPduInfo_st *pdu =
                                    (FAPI_ulSCHPduInfo_st *)&pduInfo->ulPduConfigInfo[0];
                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                                fprintf(outFilePtr, "\t\tSize %d\n", pdu->size);
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                                fprintf(outFilePtr, "\t\tResource Block Start %d\n", pdu->rbStart);
                                fprintf(outFilePtr, "\t\tNumber of Resource Blocks %d\n",
                                        pdu->numOfRB);
                                fprintf(outFilePtr, "\t\tModulation Type %d\n", pdu->modulationType);
                                fprintf(outFilePtr, "\t\tCyclic Shift 2 for DMRS %d\n",
                                        pdu->cyclicShift2forDMRS);
                                fprintf(outFilePtr, "\t\tFrequency hopping enabled flag %d\n",
                                        pdu->freqHoppingenabledFlag);
                                fprintf(outFilePtr, "\t\tFrequency hopping bits %d\n",
                                        pdu->freqHoppingBits);
                                fprintf(outFilePtr, "\t\tNew Data Indication %d\n",
                                        pdu->newDataIndication);
                                fprintf(outFilePtr, "\t\tRedundancy Version %d\n",
                                        pdu->redundancyVersion);
                                fprintf(outFilePtr, "\t\tHARQ Process Number %d\n",
                                        pdu->harqProcessNumber);
                                fprintf(outFilePtr, "\t\tUL Tx Mode %d\n", pdu->ulTxMode);
                                fprintf(outFilePtr, "\t\tCurrent TX NB %d\n", pdu->currentTxNB);
                                fprintf(outFilePtr, "\t\tN srs %d\n", pdu->nSRS);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                        pdu->release10Param.resourceAllocationType);
                                fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                        MAC_PHY_CONVERT_32(pdu->release10Param.resourceBlockCoding));
                                fprintf(outFilePtr, "\t\tTransport Blocks %d\n",
                                        pdu->release10Param.transportBlocks);
                                fprintf(outFilePtr, "\t\tTransmission Scheme %d\n",
                                        pdu->release10Param.transmissionScheme);
                                fprintf(outFilePtr, "\t\tNumber Of Layers %d\n",
                                        pdu->release10Param.numOfLayers);
                                fprintf(outFilePtr, "\t\tCodebook Index %d\n", pdu->release10Param.codebookIndex);
                                fprintf(outFilePtr, "\t\tDisable Sequence Hopping Flag %d\n",
                                        pdu->release10Param.disableSequenceHopping);
#endif
                                break;
                            }

                        case FAPI_ULSCH_CQI_RI :
                            {
                                FAPI_ulSCHCqiRiPduInfo_st *pdu =
                                    (FAPI_ulSCHCqiRiPduInfo_st *)&pduInfo->ulPduConfigInfo[0];
                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.handle));
                                fprintf(outFilePtr, "\t\tSize %d\n", pdu->ulSCHPduInfo.size);
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->ulSCHPduInfo.rnti));
                                fprintf(outFilePtr, "\t\tResource Block Start %d\n",
                                        pdu->ulSCHPduInfo.rbStart);
                                fprintf(outFilePtr, "\t\tNumber of Resource Blocks %d\n",
                                        pdu->ulSCHPduInfo.numOfRB);
                                fprintf(outFilePtr, "\t\tModulation Type %d\n", pdu->ulSCHPduInfo.modulationType);
                                fprintf(outFilePtr, "\t\tCyclic Shift 2 for DMRS %d\n",
                                        pdu->ulSCHPduInfo.cyclicShift2forDMRS);
                                fprintf(outFilePtr, "\t\tFrequency hopping enabled flag %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingenabledFlag);
                                fprintf(outFilePtr, "\t\tFrequency hopping bits %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingBits);
                                fprintf(outFilePtr, "\t\tNew Data Indication %d\n",
                                        pdu->ulSCHPduInfo.newDataIndication);
                                fprintf(outFilePtr, "\t\tRedundancy Version %d\n",
                                        pdu->ulSCHPduInfo.redundancyVersion);
                                fprintf(outFilePtr, "\t\tHARQ Process Number %d\n",
                                        pdu->ulSCHPduInfo.harqProcessNumber);
                                fprintf(outFilePtr, "\t\tUL Tx Mode %d\n", pdu->ulSCHPduInfo.ulTxMode);
                                fprintf(outFilePtr, "\t\tCurrent TX NB %d\n", pdu->ulSCHPduInfo.currentTxNB);
                                fprintf(outFilePtr, "\t\tN srs %d\n", pdu->ulSCHPduInfo.nSRS);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                        pdu->ulSCHPduInfo.release10Param.resourceAllocationType);
                                fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                        MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.release10Param.resourceBlockCoding));
                                fprintf(outFilePtr, "\t\tTransport Blocks %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transportBlocks);
                                fprintf(outFilePtr, "\t\tTransmission Scheme %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transmissionScheme);
                                fprintf(outFilePtr, "\t\tNumber Of Layers %d\n",
                                        pdu->ulSCHPduInfo.release10Param.numOfLayers);
                                fprintf(outFilePtr, "\t\tCodebook Index %d\n", pdu->ulSCHPduInfo.release10Param.codebookIndex);
                                fprintf(outFilePtr, "\t\tDisable Sequence Hopping Flag %d\n",
                                        pdu->ulSCHPduInfo.release10Param.disableSequenceHopping);
#endif

                                // FAPI_cqiRiPduInfo_st
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size Rank = 1 %d\n",
                                        pdu->cqiRiInfo.dlCqiPmiSizeRank_1);
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size Rank>1 %d\n",
                                        pdu->cqiRiInfo.dlCqiPmiSizeRankGT_1);
                                fprintf(outFilePtr, "\t\tRI Size %d\n", pdu->cqiRiInfo.riSize);
                                fprintf(outFilePtr, "\t\tDelta Offset CQI %d\n",
                                        pdu->cqiRiInfo.deltaOffsetCQI);
                                fprintf(outFilePtr, "\t\tDelta Offset RI %d\n",
                                        pdu->cqiRiInfo.deltaOffsetRI);

                                // FAPI_initialTxParam_st
                                fprintf(outFilePtr, "\t\tN srs initial %d\n",
                                        pdu->initialTxParamInfo.nSRSInitial);
                                fprintf(outFilePtr, "\t\tInitial number of resource blocks %d\n",
                                        pdu->initialTxParamInfo.initialNumOfRB);
                                break;
                            }

                        case FAPI_ULSCH_HARQ :
                            {
                                FAPI_ulSCHHarqPduInfo_st *pdu =
                                    (FAPI_ulSCHHarqPduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.handle));
                                fprintf(outFilePtr, "\t\tSize %d\n", pdu->ulSCHPduInfo.size);
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->ulSCHPduInfo.rnti));
                                fprintf(outFilePtr, "\t\tResource Block Start %d\n",
                                        pdu->ulSCHPduInfo.rbStart);
                                fprintf(outFilePtr, "\t\tNumber of Resource Blocks %d\n",
                                        pdu->ulSCHPduInfo.numOfRB);
                                fprintf(outFilePtr, "\t\tModulation Type %d\n", pdu->ulSCHPduInfo.modulationType);
                                fprintf(outFilePtr, "\t\tCyclic Shift 2 for DMRS %d\n",
                                        pdu->ulSCHPduInfo.cyclicShift2forDMRS);
                                fprintf(outFilePtr, "\t\tFrequency hopping enabled flag %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingenabledFlag);
                                fprintf(outFilePtr, "\t\tFrequency hopping bits %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingBits);
                                fprintf(outFilePtr, "\t\tNew Data Indication %d\n",
                                        pdu->ulSCHPduInfo.newDataIndication);
                                fprintf(outFilePtr, "\t\tRedundancy Version %d\n",
                                        pdu->ulSCHPduInfo.redundancyVersion);
                                fprintf(outFilePtr, "\t\tHARQ Process Number %d\n",
                                        pdu->ulSCHPduInfo.harqProcessNumber);
                                fprintf(outFilePtr, "\t\tUL Tx Mode %d\n", pdu->ulSCHPduInfo.ulTxMode);
                                fprintf(outFilePtr, "\t\tCurrent TX NB %d\n", pdu->ulSCHPduInfo.currentTxNB);
                                fprintf(outFilePtr, "\t\tN srs %d\n", pdu->ulSCHPduInfo.nSRS);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                        pdu->ulSCHPduInfo.release10Param.resourceAllocationType);
                                fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                        MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.release10Param.resourceBlockCoding));
                                fprintf(outFilePtr, "\t\tTransport Blocks %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transportBlocks);
                                fprintf(outFilePtr, "\t\tTransmission Scheme %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transmissionScheme);
                                fprintf(outFilePtr, "\t\tNumber Of Layers %d\n",
                                        pdu->ulSCHPduInfo.release10Param.numOfLayers);
                                fprintf(outFilePtr, "\t\tCodebook Index %d\n", pdu->ulSCHPduInfo.release10Param.codebookIndex);
                                fprintf(outFilePtr, "\t\tDisable Sequence Hopping Flag %d\n",
                                        pdu->ulSCHPduInfo.release10Param.disableSequenceHopping);
#endif
                                // FAPI_ulSCHHarqInfo_st
                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
                                fprintf(outFilePtr, "\t\tDelta Offset HARQ %d\n",
                                        pdu->harqInfo.deltaOffsetHarq);
                                fprintf(outFilePtr, "\t\tACK_NACK mode %d\n",
                                        pdu->harqInfo.ackNackMode);

                                // FAPI_initialTxParam_st
                                fprintf(outFilePtr, "\t\tN srs initial %d\n",
                                        pdu->initialTxParamInfo.nSRSInitial);
                                fprintf(outFilePtr, "\t\tInitial number of resource blocks %d\n",
                                        pdu->initialTxParamInfo.initialNumOfRB);
                                break;
                            }

                        case FAPI_ULSCH_CQI_HARQ_RI :
                            {
                                FAPI_ulSCHCqiHarqRIPduInfo_st *pdu =
                                    (FAPI_ulSCHCqiHarqRIPduInfo_st *)&pduInfo->ulPduConfigInfo[0];
                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.handle));
                                fprintf(outFilePtr, "\t\tSize %d\n", pdu->ulSCHPduInfo.size);
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->ulSCHPduInfo.rnti));
                                fprintf(outFilePtr, "\t\tResource Block Start %d\n",
                                        pdu->ulSCHPduInfo.rbStart);
                                fprintf(outFilePtr, "\t\tNumber of Resource Blocks %d\n",
                                        pdu->ulSCHPduInfo.numOfRB);
                                fprintf(outFilePtr, "\t\tModulation Type %d\n", pdu->ulSCHPduInfo.modulationType);
                                fprintf(outFilePtr, "\t\tCyclic Shift 2 for DMRS %d\n",
                                        pdu->ulSCHPduInfo.cyclicShift2forDMRS);
                                fprintf(outFilePtr, "\t\tFrequency hopping enabled flag %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingenabledFlag);
                                fprintf(outFilePtr, "\t\tFrequency hopping bits %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingBits);
                                fprintf(outFilePtr, "\t\tNew Data Indication %d\n",
                                        pdu->ulSCHPduInfo.newDataIndication);
                                fprintf(outFilePtr, "\t\tRedundancy Version %d\n",
                                        pdu->ulSCHPduInfo.redundancyVersion);
                                fprintf(outFilePtr, "\t\tHARQ Process Number %d\n",
                                        pdu->ulSCHPduInfo.harqProcessNumber);
                                fprintf(outFilePtr, "\t\tUL Tx Mode %d\n", pdu->ulSCHPduInfo.ulTxMode);
                                fprintf(outFilePtr, "\t\tCurrent TX NB %d\n", pdu->ulSCHPduInfo.currentTxNB);
                                fprintf(outFilePtr, "\t\tN srs %d\n", pdu->ulSCHPduInfo.nSRS);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                        pdu->ulSCHPduInfo.release10Param.resourceAllocationType);
                                fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                        MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.release10Param.resourceBlockCoding));
                                fprintf(outFilePtr, "\t\tTransport Blocks %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transportBlocks);
                                fprintf(outFilePtr, "\t\tTransmission Scheme %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transmissionScheme);
                                fprintf(outFilePtr, "\t\tNumber Of Layers %d\n",
                                        pdu->ulSCHPduInfo.release10Param.numOfLayers);
                                fprintf(outFilePtr, "\t\tCodebook Index %d\n", pdu->ulSCHPduInfo.release10Param.codebookIndex);
                                fprintf(outFilePtr, "\t\tDisable Sequence Hopping Flag %d\n",
                                        pdu->ulSCHPduInfo.release10Param.disableSequenceHopping);
#endif

                                // FAPI_cqiRiPduInfo_st
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size Rank = 1 %d\n",
                                        pdu->cqiRiInfo.dlCqiPmiSizeRank_1);
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size Rank>1 %d\n",
                                        pdu->cqiRiInfo.dlCqiPmiSizeRankGT_1);
                                fprintf(outFilePtr, "\t\tRI Size %d\n", pdu->cqiRiInfo.riSize);
                                fprintf(outFilePtr, "\t\tDelta Offset CQI %d\n",
                                        pdu->cqiRiInfo.deltaOffsetCQI);
                                fprintf(outFilePtr, "\t\tDelta Offset RI %d\n",
                                        pdu->cqiRiInfo.deltaOffsetRI);

                                // FAPI_ulSCHHarqInfo_st
                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
                                fprintf(outFilePtr, "\t\tDelta Offset HARQ %d\n",
                                        pdu->harqInfo.deltaOffsetHarq);
                                fprintf(outFilePtr, "\t\tACK_NACK mode %d\n",
                                        pdu->harqInfo.ackNackMode);

                                // FAPI_initialTxParam_st
                                fprintf(outFilePtr, "\t\tN srs initial %d\n",
                                        pdu->initialTxParamInfo.nSRSInitial);
                                fprintf(outFilePtr, "\t\tInitial number of resource blocks %d\n",
                                        pdu->initialTxParamInfo.initialNumOfRB);

                                break;
                            }

                        case FAPI_UCI_CQI :
                            {
                                FAPI_uciCqiPduInfo_st *pdu =
                                    (FAPI_uciCqiPduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));

                                // FAPI_cqiPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->cqiInfo.pucchIndex));
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size %d\n",
                                        pdu->cqiInfo.dlCqiPmiSize);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->cqiInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->cqiInfo.release10Param.pucchIndexP1);
#endif
                                break;
                            }

                        case FAPI_UCI_SR :
                            {
                                FAPI_uciSrPduInfo_st *pdu =
                                    (FAPI_uciSrPduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));

                                // FAPI_srPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->srInfo.pucchIndex));
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->srInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->srInfo.release10Param.pucchIndexP1);
#endif
                                break;
                            }

                        case FAPI_UCI_HARQ :
                            {
                                FAPI_uciHarqPduInfo_st *pdu =
                                    (FAPI_uciHarqPduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));

                                // FAPI_tddHarqPduInfo_st
#ifdef TDD_CONFIG
                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
                                fprintf(outFilePtr, "\t\tACK_NACK mode %d\n",
                                        pdu->harqInfo.acknackMode);
                                fprintf(outFilePtr, "\t\tNumber of PUCCH Resources %d\n",
                                        pdu->harqInfo.numOfPUCCHResource);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_0);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_1);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_2 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_2);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_3 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_3);
#endif

                                // FAPI_fddHarqPduInfo_st
#ifdef FDD_CONFIG
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        MAC_PHY_CONVERT_16(pdu->harqInfo.pucchIndex));

#if ((!defined(FAPI_1_1_COMPLIANCE)) &&  (!defined(FAPI_4_0_COMPLIANCE)))
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.pucchIndex1);
#endif

                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
#endif
                                break;
                            }

                        case FAPI_UCI_SR_HARQ :
                            {
                                FAPI_uciSrHarqPduInfo_st *pdu =
                                    (FAPI_uciSrHarqPduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));

                                // FAPI_srPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->srInfo.pucchIndex));
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->srInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->srInfo.release10Param.pucchIndexP1);
#endif

                                // FAPI_tddHarqPduInfo_st
#ifdef TDD_CONFIG
                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
                                fprintf(outFilePtr, "\t\tACK_NACK mode %d\n",
                                        pdu->harqInfo.acknackMode);
                                fprintf(outFilePtr, "\t\tNumber of PUCCH Resources %d\n",
                                        pdu->harqInfo.numOfPUCCHResource);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_0);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_1);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_2 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_2);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_3 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_3);
#endif

                                // FAPI_fddHarqPduInfo_st
#ifdef FDD_CONFIG
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        MAC_PHY_CONVERT_16(pdu->harqInfo.pucchIndex));

#if ((!defined(FAPI_1_1_COMPLIANCE)) &&  (!defined(FAPI_4_0_COMPLIANCE)))
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.pucchIndex1);
#endif

                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
#endif
                                break;
                            }

                        case FAPI_UCI_CQI_HARQ :
                            {
                                FAPI_uciCqiHarqPduInfo_st *pdu =
                                    (FAPI_uciCqiHarqPduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));

                                // FAPI_cqiPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->cqiInfo.pucchIndex));
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size %d\n",
                                        pdu->cqiInfo.dlCqiPmiSize);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->cqiInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->cqiInfo.release10Param.pucchIndexP1);
#endif
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));

                                // FAPI_tddHarqPduInfo_st
#ifdef TDD_CONFIG
                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
                                fprintf(outFilePtr, "\t\tACK_NACK mode %d\n",
                                        pdu->harqInfo.acknackMode);
                                fprintf(outFilePtr, "\t\tNumber of PUCCH Resources %d\n",
                                        pdu->harqInfo.numOfPUCCHResource);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_0);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_1);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_2 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_2);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_3 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_3);
#endif

                                // FAPI_fddHarqPduInfo_st
#ifdef FDD_CONFIG
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        MAC_PHY_CONVERT_16(pdu->harqInfo.pucchIndex));

#if ((!defined(FAPI_1_1_COMPLIANCE)) &&  (!defined(FAPI_4_0_COMPLIANCE)))
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.pucchIndex1);
#endif

                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
#endif
                                break;
                            }

                        case FAPI_UCI_CQI_SR :
                            {
                                FAPI_uciCqiSrPduInfo_st *pdu =
                                    (FAPI_uciCqiSrPduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));

                                // FAPI_cqiPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->cqiInfo.pucchIndex));
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size %d\n",
                                        pdu->cqiInfo.dlCqiPmiSize);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->cqiInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->cqiInfo.release10Param.pucchIndexP1);
#endif
                                // FAPI_srPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->srInfo.pucchIndex));
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->srInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->srInfo.release10Param.pucchIndexP1);
#endif
                                break;
                            }

                        case FAPI_UCI_CQI_SR_HARQ :
                            {
                                FAPI_uciCqiSrHarqPduInfo_st *pdu =
                                    (FAPI_uciCqiSrHarqPduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));

                                // FAPI_srPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->srInfo.pucchIndex));
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->srInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->srInfo.release10Param.pucchIndexP1);
#endif

                                // FAPI_cqiPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->cqiInfo.pucchIndex));
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size %d\n",
                                        pdu->cqiInfo.dlCqiPmiSize);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->cqiInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->cqiInfo.release10Param.pucchIndexP1);
#endif

                                // FAPI_tddHarqPduInfo_st
#ifdef TDD_CONFIG
                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
                                fprintf(outFilePtr, "\t\tACK_NACK mode %d\n",
                                        pdu->harqInfo.acknackMode);
                                fprintf(outFilePtr, "\t\tNumber of PUCCH Resources %d\n",
                                        pdu->harqInfo.numOfPUCCHResource);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_0);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_1);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_2 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_2);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_3 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_3);
#endif

                                // FAPI_fddHarqPduInfo_st
#ifdef FDD_CONFIG
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        MAC_PHY_CONVERT_16(pdu->harqInfo.pucchIndex));

#if ((!defined(FAPI_1_1_COMPLIANCE)) &&  (!defined(FAPI_4_0_COMPLIANCE)))
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.pucchIndex1);
#endif

                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
#endif
                                break;
                            }

                        case FAPI_SRS :
                            {
                                FAPI_srsPduInfo_st *pdu =
                                    (FAPI_srsPduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                                fprintf(outFilePtr, "\t\tSize %d\n", pdu->size);
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                                fprintf(outFilePtr, "\t\tSRS Bandwidth  %d\n", pdu->srsBandWidth);
                                fprintf(outFilePtr, "\t\tFrequency Domain Position %d\n",
                                        pdu->freqDomainPosition);
                                fprintf(outFilePtr, "\t\tSRS Hopping Bandwidth %d\n",
                                        pdu->srsHoppingBandWidth);
                                fprintf(outFilePtr, "\t\tTransmission Comb %d\n",
                                        pdu->transmissionComb);
                                fprintf(outFilePtr, "\t\tISRS / SRS-ConfigIndex %d\n",
                                        MAC_PHY_CONVERT_16(pdu->isrsSRSConfigIndex));
                                fprintf(outFilePtr, "\t\tSounding Reference Cyclic Shift %d\n",
                                        pdu->soundingRefCyclicShift);

#ifdef FAPI_4_0_COMPLIANCE
                                // FAPI_srsPduInfoRel10_st
                                fprintf(outFilePtr, "\t\tAntenna Port %d\n",
                                        pdu->release10Param.antennaPort);
#endif
                                break;
                            }

                        case FAPI_HARQ_BUFFER :
                            {
#if  (defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE))
                                FAPI_ulSCHHarqBufferReleasePduInfo_st *pdu =
                                    (FAPI_ulSCHHarqBufferReleasePduInfo_st *)&pduInfo->ulPduConfigInfo[0];

                                fprintf(outFilePtr, "\t\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                                fprintf(outFilePtr, "\t\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
#endif
                                break;
                            }

                        case FAPI_ULSCH_UCI_CSI :
                            {
                                Temp_FAPI_ULSCH_UCI_CSI_Struct *pdu =
                                    (Temp_FAPI_ULSCH_UCI_CSI_Struct *)&pduInfo->ulPduConfigInfo[0];

                                // FAPI_ulSCHPduInfo_st
                                fprintf(outFilePtr, "\t\tHandle %d\n",
                                        MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.handle));
                                fprintf(outFilePtr, "\t\tSize %d\n",
                                        pdu->ulSCHPduInfo.size);
                                fprintf(outFilePtr, "\t\tRNTI %d\n",
                                        MAC_PHY_CONVERT_16(pdu->ulSCHPduInfo.rnti));
                                fprintf(outFilePtr, "\t\tResource Block Start %d\n",
                                        pdu->ulSCHPduInfo.rbStart);
                                fprintf(outFilePtr, "\t\tNumber of Resource Blocks %d\n",
                                        pdu->ulSCHPduInfo.numOfRB);
                                fprintf(outFilePtr, "\t\tModulation Type %d\n",
                                        pdu->ulSCHPduInfo.modulationType);
                                fprintf(outFilePtr, "\t\tCyclic Shift 2 for DMRS %d\n",
                                        pdu->ulSCHPduInfo.cyclicShift2forDMRS);
                                fprintf(outFilePtr, "\t\tFrequency hopping enabled flag %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingenabledFlag);
                                fprintf(outFilePtr, "\t\tFrequency hopping bits %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingBits);
                                fprintf(outFilePtr, "\t\tNew Data Indication %d\n",
                                        pdu->ulSCHPduInfo.newDataIndication);
                                fprintf(outFilePtr, "\t\tRedundancy Version %d\n",
                                        pdu->ulSCHPduInfo.redundancyVersion);
                                fprintf(outFilePtr, "\t\tHARQ Process Number %d\n",
                                        pdu->ulSCHPduInfo.harqProcessNumber);
                                fprintf(outFilePtr, "\t\tUL Tx Mode %d\n",
                                        pdu->ulSCHPduInfo.ulTxMode);
                                fprintf(outFilePtr, "\t\tCurrent TX NB %d\n",
                                        pdu->ulSCHPduInfo.currentTxNB);
                                fprintf(outFilePtr, "\t\tN srs %d\n",
                                        pdu->ulSCHPduInfo.nSRS);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                        pdu->ulSCHPduInfo.release10Param.resourceAllocationType);
                                fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                        MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.release10Param.resourceBlockCoding));
                                fprintf(outFilePtr, "\t\tTransport Blocks %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transportBlocks);
                                fprintf(outFilePtr, "\t\tTransmission Scheme %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transmissionScheme);
                                fprintf(outFilePtr, "\t\tNumber Of Layers %d\n",
                                        pdu->ulSCHPduInfo.release10Param.numOfLayers);
                                fprintf(outFilePtr, "\t\tCodebook Index %d\n",
                                        pdu->ulSCHPduInfo.release10Param.codebookIndex);
                                fprintf(outFilePtr, "\t\tDisable Sequence Hopping Flag %d\n",
                                        pdu->ulSCHPduInfo.release10Param.disableSequenceHopping);
#endif

                                // FAPI_cqiPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->cqiInfo.pucchIndex));
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size %d\n",
                                        pdu->cqiInfo.dlCqiPmiSize);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->cqiInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->cqiInfo.release10Param.pucchIndexP1);
#endif
                                break;
                            }

                        case FAPI_ULSCH_UCI_HARQ :
                            {
                                Temp_FAPI_ULSCH_UCI_HARQ_Struct *pdu =
                                    (Temp_FAPI_ULSCH_UCI_HARQ_Struct *)&pduInfo->ulPduConfigInfo[0];

                                // FAPI_ulSCHPduInfo_st
                                fprintf(outFilePtr, "\t\tHandle %d\n",
                                        MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.handle));
                                fprintf(outFilePtr, "\t\tSize %d\n",
                                        pdu->ulSCHPduInfo.size);
                                fprintf(outFilePtr, "\t\tRNTI %d\n",
                                        MAC_PHY_CONVERT_16(pdu->ulSCHPduInfo.rnti));
                                fprintf(outFilePtr, "\t\tResource Block Start %d\n",
                                        pdu->ulSCHPduInfo.rbStart);
                                fprintf(outFilePtr, "\t\tNumber of Resource Blocks %d\n",
                                        pdu->ulSCHPduInfo.numOfRB);
                                fprintf(outFilePtr, "\t\tModulation Type %d\n",
                                        pdu->ulSCHPduInfo.modulationType);
                                fprintf(outFilePtr, "\t\tCyclic Shift 2 for DMRS %d\n",
                                        pdu->ulSCHPduInfo.cyclicShift2forDMRS);
                                fprintf(outFilePtr, "\t\tFrequency hopping enabled flag %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingenabledFlag);
                                fprintf(outFilePtr, "\t\tFrequency hopping bits %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingBits);
                                fprintf(outFilePtr, "\t\tNew Data Indication %d\n",
                                        pdu->ulSCHPduInfo.newDataIndication);
                                fprintf(outFilePtr, "\t\tRedundancy Version %d\n",
                                        pdu->ulSCHPduInfo.redundancyVersion);
                                fprintf(outFilePtr, "\t\tHARQ Process Number %d\n",
                                        pdu->ulSCHPduInfo.harqProcessNumber);
                                fprintf(outFilePtr, "\t\tUL Tx Mode %d\n",
                                        pdu->ulSCHPduInfo.ulTxMode);
                                fprintf(outFilePtr, "\t\tCurrent TX NB %d\n",
                                        pdu->ulSCHPduInfo.currentTxNB);
                                fprintf(outFilePtr, "\t\tN srs %d\n",
                                        pdu->ulSCHPduInfo.nSRS);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                        pdu->ulSCHPduInfo.release10Param.resourceAllocationType);
                                fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                        MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.release10Param.resourceBlockCoding));
                                fprintf(outFilePtr, "\t\tTransport Blocks %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transportBlocks);
                                fprintf(outFilePtr, "\t\tTransmission Scheme %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transmissionScheme);
                                fprintf(outFilePtr, "\t\tNumber Of Layers %d\n",
                                        pdu->ulSCHPduInfo.release10Param.numOfLayers);
                                fprintf(outFilePtr, "\t\tCodebook Index %d\n",
                                        pdu->ulSCHPduInfo.release10Param.codebookIndex);
                                fprintf(outFilePtr, "\t\tDisable Sequence Hopping Flag %d\n",
                                        pdu->ulSCHPduInfo.release10Param.disableSequenceHopping);
#endif

                                // FAPI_tddHarqPduInfo_st
#ifdef TDD_CONFIG
                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
                                fprintf(outFilePtr, "\t\tACK_NACK mode %d\n",
                                        pdu->harqInfo.acknackMode);
                                fprintf(outFilePtr, "\t\tNumber of PUCCH Resources %d\n",
                                        pdu->harqInfo.numOfPUCCHResource);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_0);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_1);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_2 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_2);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_3 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_3);
#endif

                                // FAPI_fddHarqPduInfo_st
#ifdef FDD_CONFIG
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        MAC_PHY_CONVERT_16(pdu->harqInfo.pucchIndex));

#if ((!defined(FAPI_1_1_COMPLIANCE)) &&  (!defined(FAPI_4_0_COMPLIANCE)))
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.pucchIndex1);
#endif

                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
#endif
                                break;
                            }

                        case FAPI_ULSCH_CSI_UCI_HARQ :
                            {
                                Temp_FAPI_ULSCH_CSI_UCI_HARQ_Struct *pdu =
                                    (Temp_FAPI_ULSCH_CSI_UCI_HARQ_Struct *)&pduInfo->ulPduConfigInfo[0];

                                // FAPI_ulSCHPduInfo_st
                                fprintf(outFilePtr, "\t\tHandle %d\n",
                                        MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.handle));
                                fprintf(outFilePtr, "\t\tSize %d\n",
                                        pdu->ulSCHPduInfo.size);
                                fprintf(outFilePtr, "\t\tRNTI %d\n",
                                        MAC_PHY_CONVERT_16(pdu->ulSCHPduInfo.rnti));
                                fprintf(outFilePtr, "\t\tResource Block Start %d\n",
                                        pdu->ulSCHPduInfo.rbStart);
                                fprintf(outFilePtr, "\t\tNumber of Resource Blocks %d\n",
                                        pdu->ulSCHPduInfo.numOfRB);
                                fprintf(outFilePtr, "\t\tModulation Type %d\n",
                                        pdu->ulSCHPduInfo.modulationType);
                                fprintf(outFilePtr, "\t\tCyclic Shift 2 for DMRS %d\n",
                                        pdu->ulSCHPduInfo.cyclicShift2forDMRS);
                                fprintf(outFilePtr, "\t\tFrequency hopping enabled flag %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingenabledFlag);
                                fprintf(outFilePtr, "\t\tFrequency hopping bits %d\n",
                                        pdu->ulSCHPduInfo.freqHoppingBits);
                                fprintf(outFilePtr, "\t\tNew Data Indication %d\n",
                                        pdu->ulSCHPduInfo.newDataIndication);
                                fprintf(outFilePtr, "\t\tRedundancy Version %d\n",
                                        pdu->ulSCHPduInfo.redundancyVersion);
                                fprintf(outFilePtr, "\t\tHARQ Process Number %d\n",
                                        pdu->ulSCHPduInfo.harqProcessNumber);
                                fprintf(outFilePtr, "\t\tUL Tx Mode %d\n",
                                        pdu->ulSCHPduInfo.ulTxMode);
                                fprintf(outFilePtr, "\t\tCurrent TX NB %d\n",
                                        pdu->ulSCHPduInfo.currentTxNB);
                                fprintf(outFilePtr, "\t\tN srs %d\n",
                                        pdu->ulSCHPduInfo.nSRS);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tResource Allocation Type %d\n",
                                        pdu->ulSCHPduInfo.release10Param.resourceAllocationType);
                                fprintf(outFilePtr, "\t\tResource Block Coding %d\n",
                                        MAC_PHY_CONVERT_32(pdu->ulSCHPduInfo.release10Param.resourceBlockCoding));
                                fprintf(outFilePtr, "\t\tTransport Blocks %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transportBlocks);
                                fprintf(outFilePtr, "\t\tTransmission Scheme %d\n",
                                        pdu->ulSCHPduInfo.release10Param.transmissionScheme);
                                fprintf(outFilePtr, "\t\tNumber Of Layers %d\n",
                                        pdu->ulSCHPduInfo.release10Param.numOfLayers);
                                fprintf(outFilePtr, "\t\tCodebook Index %d\n",
                                        pdu->ulSCHPduInfo.release10Param.codebookIndex);
                                fprintf(outFilePtr, "\t\tDisable Sequence Hopping Flag %d\n",
                                        pdu->ulSCHPduInfo.release10Param.disableSequenceHopping);
#endif

                                // FAPI_cqiPduInfo_st
                                fprintf(outFilePtr, "\t\tPUCCH Index %d\n", MAC_PHY_CONVERT_16(pdu->cqiInfo.pucchIndex));
                                fprintf(outFilePtr, "\t\tDL CQI/PMI Size %d\n",
                                        pdu->cqiInfo.dlCqiPmiSize);
#ifdef FAPI_4_0_COMPLIANCE
                                fprintf(outFilePtr, "\t\tRelease 10 parameters - \n");
                                fprintf(outFilePtr, "\t\t\tNumber of PUCCH Resources %d\n",
                                        pdu->cqiInfo.release10Param.numPucchRes);
                                fprintf(outFilePtr, "\t\t\tPUCCH Index P1 %d\n",
                                        pdu->cqiInfo.release10Param.pucchIndexP1);
#endif

                                // FAPI_tddHarqPduInfo_st
#ifdef TDD_CONFIG
                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
                                fprintf(outFilePtr, "\t\tACK_NACK mode %d\n",
                                        pdu->harqInfo.acknackMode);
                                fprintf(outFilePtr, "\t\tNumber of PUCCH Resources %d\n",
                                        pdu->harqInfo.numOfPUCCHResource);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_0);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_1);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_2 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_2);
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_3 %d\n",
                                        pdu->harqInfo.n_PUCCH_1_3);
#endif

                                // FAPI_fddHarqPduInfo_st
#ifdef FDD_CONFIG
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_0 %d\n",
                                        MAC_PHY_CONVERT_16(pdu->harqInfo.pucchIndex));

#if ((!defined(FAPI_1_1_COMPLIANCE)) &&  (!defined(FAPI_4_0_COMPLIANCE)))
                                fprintf(outFilePtr, "\t\tn_PUCCH_1_1 %d\n",
                                        pdu->harqInfo.pucchIndex1);
#endif

                                fprintf(outFilePtr, "\t\tHARQ Size %d\n", pdu->harqInfo.harqSize);
#endif
                                break;
                            }
                    }
                    fprintf(outFilePtr, "}\n");
                }

                break;
            }

        case PHY_UL_SUBFRAME_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_subFrameIndication_st *msg_body =
                    (FAPI_subFrameIndication_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                break;
            }

        case PHY_DL_HI_DCI0_REQUEST:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_dlHiDCIPduInfo_st *msg_body =
                    (FAPI_dlHiDCIPduInfo_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of DCI %d\n", msg_body->numOfDCI);
                fprintf(outFilePtr, "Number of  HI %d\n", msg_body->numOfHI);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfHI; i++)
    /* SPR 17884 FIX END */
                {
                    // FAPI_dlHiPduInfo_st
                    FAPI_dlHiPduInfo_st *pdu =
                        (FAPI_dlHiPduInfo_st *)&msg_body->hidciPduInfo[i];
                    fprintf(outFilePtr, "{\n\tPDU Type %d\n", pdu->pduType);
                    fprintf(outFilePtr, "\tPDU Size %d\n", pdu->hipduSize);
                    fprintf(outFilePtr, "\tResource Block Start %d\n", pdu->rbStart);
                    fprintf(outFilePtr, "\tCyclic Shift 2 for DMRS %d\n", pdu->cyclicShift2_forDMRS);
                    fprintf(outFilePtr, "\tHI Value %d\n", pdu->hiValue);
                    fprintf(outFilePtr, "\tI_PHICH %d\n", pdu->iPHICH);
                    fprintf(outFilePtr, "\tTransmission power %d\n", MAC_PHY_CONVERT_16(pdu->txPower));
#ifdef FAPI_4_0_COMPLIANCE
                    // FAPI_dlHiPduInfoRel10_st
                    fprintf(outFilePtr, "\tFlag TB2 %d\n", pdu->release10Param.flag_TB2);
                    fprintf(outFilePtr, "\tHI Value 2 %d\n", pdu->release10Param.hiValue2);
#endif
                    fprintf(outFilePtr, "}\n");
                }

                // FAPI_dlDCIPduInfo_st
    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfDCI; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_dlDCIPduInfo_st *pdu =
                        (FAPI_dlDCIPduInfo_st *)&msg_body->hidciPduInfo[i + msg_body->numOfHI];
                    fprintf(outFilePtr, "{\n\tPDU Type %d\n", pdu->pduType);
                    fprintf(outFilePtr, "\tPDU Size %d\n", pdu->ulDCIPDUSize);
                    fprintf(outFilePtr, "\tDCI Format %d\n", pdu->ulDCIFormat);
                    fprintf(outFilePtr, "\tCCE Index %d\n", pdu->cceIndex);
                    fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                    fprintf(outFilePtr, "\tAggregation Level %d\n", pdu->aggLevel);
                    fprintf(outFilePtr, "\tResource Block Start %d\n", pdu->rbStart);
                    fprintf(outFilePtr, "\tNumber of Resource Blocks %d\n", pdu->numOfRB);
                    fprintf(outFilePtr, "\tMCS_1 %d\n", pdu->mcs);
                    fprintf(outFilePtr, "\tCyclic Shift 2 for DMRS %d\n", pdu->cyclicShift2_forDMRS);
                    fprintf(outFilePtr, "\tFrequency hopping enabled flag %d\n", pdu->freqEnabledFlag);
                    fprintf(outFilePtr, "\tFrequency hopping bits %d\n", pdu->freqHoppingBits);
                    fprintf(outFilePtr, "\tNew Data Indication_1 %d\n", pdu->newDataIndication);
                    fprintf(outFilePtr, "\tUE TX antenna selection %d\n", pdu->ueTxAntennaSelection);
                    fprintf(outFilePtr, "\tTPC %d\n", pdu->tpc);
                    fprintf(outFilePtr, "\tCQI/CSI request %d\n", pdu->cqiRequest);
                    fprintf(outFilePtr, "\tUL index %d\n", pdu->ulIndex);
                    fprintf(outFilePtr, "\tDL assignment index %d\n", pdu->dlAssignmentIndex);
#if  defined( FAPI_DEVIATIONS) || defined(FAPI_4_0_COMPLIANCE)
                    fprintf(outFilePtr, "\tTransmission power %d\n", MAC_PHY_CONVERT_16(pdu->txPower));
#endif
                    fprintf(outFilePtr, "\tTPC bitmap %d\n", pdu->tpcBitMap);
#ifdef FAPI_4_0_COMPLIANCE
                    // FAPI_dlDCIPduInfoRel10_st
                    fprintf(outFilePtr, "\tCross carrier scheduling flag %d\n",
                            pdu->release10Param.crossCarrierScheduling);
                    fprintf(outFilePtr, "\tCarrier Indicator %d\n",
                            pdu->release10Param.carrierIndicator);
                    fprintf(outFilePtr, "\tSize of CQI/CSI field %d\n",
                            pdu->release10Param.sizeOfCqiCsiField);
                    fprintf(outFilePtr, "\tSRS flag %d\n",
                            pdu->release10Param.srsFlag);
                    fprintf(outFilePtr, "\tSRS request %d\n",
                            pdu->release10Param.srsRequest);
                    fprintf(outFilePtr, "\tResource Allocation flag %d\n",
                            pdu->release10Param.resourceAllocationFlag);
                    fprintf(outFilePtr, "\tResource Allocation Type %d\n",
                            pdu->release10Param.resourceAllocationType);
                    fprintf(outFilePtr, "\tResource Block Coding %d\n",
                            pdu->release10Param.rbCoding);
                    fprintf(outFilePtr, "\tMCS_2 %d\n",
                            pdu->release10Param.mcs_2);
                    fprintf(outFilePtr, "\tNew Data Indication_2 %d\n",
                            pdu->release10Param.newDataIndicator_2);
                    fprintf(outFilePtr, "\tNumber of Antenna Ports %d\n",
                            pdu->release10Param.numAntennaPorts);
                    fprintf(outFilePtr, "\tTPMI %d\n",
                            pdu->release10Param.tpmi);
#endif
                    fprintf(outFilePtr, "}\n");
                }
                break;
            }

        case PHY_DL_TX_REQUEST:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_dlDataTxRequest_st *msg_body =
                    (FAPI_dlDataTxRequest_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of PDUs %d\n", msg_body->numOfPDU);

                /* TX.request contains the MAC PDU data for transmission. NOT PARSING IT */
                fprintf(outFilePtr, "\t == MAC PDU DATA SKIPPED ==\n");
                /*
                for(i = 0; i < msg_body->numOfPDU; i++)
                {
                    FAPI_dlPduInfo_st *pduInfo = (FAPI_dlPduInfo_st *)&msg_body->dlPduInfo[i];
                    fprintf(outFilePtr, "\tPDU Length %d\n", MAC_PHY_CONVERT_16(pduInfo->pduLen));
                    fprintf(outFilePtr, "\tPDU index %d\n", MAC_PHY_CONVERT_16(pduInfo->pduIndex));
                    fprintf(outFilePtr, "\tnumTLV %d\n", MAC_PHY_CONVERT_32(pduInfo->numOfTLV));
                    for(j = 0; j < pduInfo->numOfTLV; j++)
                    {
                        FAPI_dlTLVInfo_st *tlv = (FAPI_dlTLVInfo_st *)&pduInfo->dlTLVInfo[j];
                        fprintf(outFilePtr, "\t\ttag %d, ", MAC_PHY_CONVERT_16(tlv->tag));
                        fprintf(outFilePtr, "\t\tlength %d, ", MAC_PHY_CONVERT_16(tlv->tagLen));
                        fprintf(outFilePtr, "\t\tvalue %d\n", (UInt32)tlv->value);
                    }
                }
                */
                break;
            }

        case PHY_UL_HARQ_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_harqIndication_st *msg_body =
                    (FAPI_harqIndication_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of HARQs %d\n", msg_body->numOfHarq);

                // FAPI_tddHarqPduIndication_st
#ifdef TDD_CONFIG
    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfHarq; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_tddHarqPduIndication_st *pduInfo =
                        (FAPI_tddHarqPduIndication_st *)&msg_body->harqPduInfo[i];
                    fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(pduInfo->handle));
                    fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pduInfo->rnti));
                    fprintf(outFilePtr, "\tMode %d\n", pduInfo->mode);
                    fprintf(outFilePtr, "\tnumOfAckNack %d\n", pduInfo->numOfAckNack);
                    switch(pduInfo->mode)
                    {
                        case BUNDLING :
                            {
                                FAPI_tddBundlingHarqInfo_st *harqInfo =
                                    (FAPI_tddBundlingHarqInfo_st *)&pduInfo->harqBuffer;
                                fprintf(outFilePtr, "\t\tValue[0]=%d\n",
                                        harqInfo->value0);
                                fprintf(outFilePtr, "\t\tValue [1]=%d\n",
                                        harqInfo->value1);
                                break;
                            }

                        case MULTIPLEXING :
                            {
                                FAPI_tddMultiplexingHarqInfo_st *harqInfo =
                                    (FAPI_tddMultiplexingHarqInfo_st *)&pduInfo->harqBuffer;
                                fprintf(outFilePtr, "\t\tValue[0]=%d\n",
                                        harqInfo->value0);
                                fprintf(outFilePtr, "\t\tValue[1]=%d\n",
                                        harqInfo->value1);
                                fprintf(outFilePtr, "\t\tValue[2]=%d\n",
                                        harqInfo->value2);
                                fprintf(outFilePtr, "\t\tValue[3]=%d\n",
                                        harqInfo->value3);
                                break;
                            }

                        case SPECIAL_BUNDLING :
                            {
                                FAPI_tddSpecialBundlingHarqInfo_st *harqInfo =
                                    (FAPI_tddSpecialBundlingHarqInfo_st *)&pduInfo->harqBuffer;
                                fprintf(outFilePtr, "\t\tValue[0]=%d\n",
                                        harqInfo->value0);
                                break;
                            }
                    }
                    fprintf(outFilePtr, "}\n");
                }
#endif
                // FAPI_fddHarqPduIndication_st
#ifdef FDD_CONFIG
    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfHarq; i++)
    /* SPR 17884 FIX END */
                {
#ifdef FAPI_RELEASE8 
                       FAPI_fddHarqPduIndicationR8_st *pduInfo =
                           (FAPI_fddHarqPduIndicationR8_st *)&msg_body->harqPduInfo[i];
                       fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(pduInfo->handle));
                       fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pduInfo->rnti));
                       fprintf(outFilePtr, "\tHARQ TB1 %d\n", pduInfo->release8Param.harqTB1);
                       fprintf(outFilePtr, "\tHARQ TB2 %d\n", pduInfo->release8Param.harqTB2);
                       fprintf(outFilePtr, "}\n");

#elif (defined(FAPI_RELEASE9) || defined(FAPI_RELEASE10)) 
                       FAPI_fddHarqPduIndicationR9_st *pduInfo =
                           (FAPI_fddHarqPduIndicationR9_st *)&msg_body->harqPduInfo[i];
                       fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(pduInfo->handle));
                       fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pduInfo->rnti));
                       fprintf(outFilePtr, "\tMode %d\n", pduInfo->release9Param.mode);
                       fprintf(outFilePtr, "\tNumber of ACK/NACK %d\n", pduInfo->release9Param.numOfAckNack);
    /* SPR 17884 FIX START */
                       for(j=0; j < pduInfo->release9Param.numOfAckNack; j++)
    /* SPR 17884 FIX END */
                       {
                           fprintf(outFilePtr, "\t\tHARQ TB_N %d\n", pduInfo->release9Param.HARQTB[j]);
                       }
                       fprintf(outFilePtr, "}\n");

#else
                       FAPI_fddHarqPduIndication_st *pduInfo =
                           (FAPI_fddHarqPduIndication_st *)&msg_body->harqPduInfo[i];
#if  (defined(FAPI_1_1_COMPLIANCE) || defined(FAPI_4_0_COMPLIANCE))
                            fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(pduInfo->handle));
#endif
                            fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pduInfo->rnti));
                            fprintf(outFilePtr, "\tHARQ TB1 %d\n", pduInfo->harqTB1);
                            fprintf(outFilePtr, "\tHARQ TB2 %d\n", pduInfo->harqTB2);

/* SPR 21001 Changes end */

                            fprintf(outFilePtr, "}\n");
#endif                            
                }
#endif
                break;
            }

        case PHY_UL_CRC_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_crcIndication_st *msg_body =
                    (FAPI_crcIndication_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of CRCs %d\n", msg_body->numOfCrc);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfCrc; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_crcPduIndication_st *crc =
                        (FAPI_crcPduIndication_st *)&msg_body->crcPduInfo[i];
                    fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(crc->handle));
                    fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(crc->rnti));
                    fprintf(outFilePtr, "\tCRC Flag %d\n", crc->crcFlag);
                    fprintf(outFilePtr, "}\n");
                }
                break;
            }

        case PHY_UL_RX_ULSCH_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
#ifdef FAPI_4_0_COMPLIANCE                    
                FAPI_rxULSCHIndicationR9_st *msg_body =
                    (FAPI_rxULSCHIndicationR9_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of PDUs %d\n", msg_body->numOfPdu);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfPdu; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_ulDataPduIndicationR9_st *pdu =
                        (FAPI_ulDataPduIndicationR9_st *)&msg_body->ulDataPduInfo[i];
                    fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                    fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                    fprintf(outFilePtr, "\tLength %d\n", MAC_PHY_CONVERT_16(pdu->length));
                    fprintf(outFilePtr, "\tData Offset %d\n", pdu->dataOffset);
                    fprintf(outFilePtr, "\tUL_CQI %d\n", pdu->ulCqi);
                    fprintf(outFilePtr, "\tTiming Advance %d\n", pdu->timingAdvance);
                    fprintf(outFilePtr, "\tTiming Advance R9 %d\n", MAC_PHY_CONVERT_16(pdu->release9Param.timingAdvanceR9));
                    fprintf(outFilePtr, "}\n");
                }
#else                    
                FAPI_rxULSCHIndication_st *msg_body =
                    (FAPI_rxULSCHIndication_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of PDUs %d\n", msg_body->numOfPdu);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfPdu; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_ulDataPduIndication_st *pdu =
                        (FAPI_ulDataPduIndication_st *)&msg_body->ulDataPduInfo[i];
                    fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                    fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                    fprintf(outFilePtr, "\tLength %d\n", MAC_PHY_CONVERT_16(pdu->length));
                    fprintf(outFilePtr, "\tData Offset %d\n", pdu->dataOffset);
                    fprintf(outFilePtr, "\tUL_CQI %d\n", pdu->ulCqi);
                    fprintf(outFilePtr, "\tTiming Advance %d\n", pdu->timingAdvance);
                    fprintf(outFilePtr, "}\n");
                }
#endif                    
                break;
            }

        case PHY_UL_RACH_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
#ifdef FAPI_4_0_COMPLIANCE                
                FAPI_rachIndicationR9_st *msg_body =
                    (FAPI_rachIndicationR9_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of Preambles %d\n", msg_body->numOfPreamble);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfPreamble; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_rachPduIndicationR9_st *pdu =
                        (FAPI_rachPduIndicationR9_st *)&msg_body->rachPduInfo[i];
                    fprintf(outFilePtr, "{\n\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                    fprintf(outFilePtr, "{\n\tPreamble %d\n", MAC_PHY_CONVERT_16(pdu->preamble));
                    fprintf(outFilePtr, "{\n\tTiming Advance %d\n", MAC_PHY_CONVERT_16(pdu->timingAdvance));
                    fprintf(outFilePtr, "{\n\tTiming Advance R9 %d\n", MAC_PHY_CONVERT_16(pdu->release9Param.timingAdvanceR9));
                }
#else                    
                FAPI_rachIndication_st *msg_body =
                    (FAPI_rachIndication_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of Preambles %d\n", msg_body->numOfPreamble);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfPreamble; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_rachPduIndication_st *pdu =
                        (FAPI_rachPduIndication_st *)&msg_body->rachPduInfo[i];
                    fprintf(outFilePtr, "{\n\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                    fprintf(outFilePtr, "\tTiming Advance %d\n", pdu->timingAdvance);
                    fprintf(outFilePtr, "\tPreamble %d\n", pdu->preamble);
                    fprintf(outFilePtr, "}\n");
                }
#endif                    
                break;
            }

        case PHY_UL_SRS_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_srsIndication_st *msg_body =
                    (FAPI_srsIndication_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of UEs %d\n", msg_body->numOfUe);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfUe; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_srsPduIndication_st *pdu =
                        (FAPI_srsPduIndication_st *)&msg_body->srsPduInfo[i];
                    fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(pdu->handle));
                    fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(pdu->rnti));
                    fprintf(outFilePtr, "\tDoppler estimation  %d\n", pdu->dopplerEstimation);
                    fprintf(outFilePtr, "\tTiming Advance %d\n", pdu->timingAdvance);
                    fprintf(outFilePtr, "\tNumber of resource blocks %d\n", pdu->numOfRB);
                    fprintf(outFilePtr, "\tRB start %d\n", pdu->rbStart);
    /* SPR 17884 FIX START */
                    for(i = 0; i < pdu->numOfRB; i++)
    /* SPR 17884 FIX END */
                    {
                        fprintf(outFilePtr, "\t\tSNR %d\n", pdu->snr[i]);
                    }
                    fprintf(outFilePtr, "}\n");
                }
                break;
            }

        case PHY_UL_RX_SR_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_rxSRIndication_st *msg_body =
                    (FAPI_rxSRIndication_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of SRs %d\n", msg_body->numOfSr);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfSr; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_srPduIndication_st *sr =
                        (FAPI_srPduIndication_st *)&msg_body->srPduInfo[i];
                    fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(sr->handle));
                    fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(sr->rnti));
                    fprintf(outFilePtr, "}\n");
                }
                break;
            }

        case PHY_UL_RX_CQI_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
#ifdef FAPI_4_0_COMPLIANCE
                FAPI_rxCqiIndicationR9_st *msg_body =
                    (FAPI_rxCqiIndicationR9_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "Number of PDUs %d\n", msg_body->numOfCqi);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfCqi; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cqiPduIndicationR9_st *sr =
                        (FAPI_cqiPduIndicationR9_st *)&msg_body->cqiPduInfo[i];
                    fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(sr->handle));
                    fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(sr->rnti));
                    fprintf(outFilePtr, "\tLength %d\n", MAC_PHY_CONVERT_16(sr->length));
                    fprintf(outFilePtr, "\tData Offset %d\n", sr->dataOffset);
                    fprintf(outFilePtr, "\tUL_CQI %d\n", sr->cqiRiInformation.ulCqi);
                    fprintf(outFilePtr, "\tNumber of CC reported %d\n", sr->cqiRiInformation.numberOfCCReported);
    /* SPR 17884 FIX START */
                    for(j = 0; j < sr->cqiRiInformation.numberOfCCReported; j++)
    /* SPR 17884 FIX END */
                    {
                        fprintf(outFilePtr, "\tRI %d\n", sr->cqiRiInformation.ri[j]);
                    }
                    fprintf(outFilePtr, "\tTiming Advance %d\n", sr->cqiRiInformation.timingAdvance);
                    fprintf(outFilePtr, "\tTiming Advance R9 %d\n", sr->cqiRiInformation.timingAdvance);
                    fprintf(outFilePtr, "}\n");
                }
#else
                FAPI_rxCqiIndication_st *msg_body =
                    (FAPI_rxCqiIndication_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "\tNumber of PDUs %d\n", msg_body->numOfCqi);

    /* SPR 17884 FIX START */
                for(i = 0; i < msg_body->numOfCqi; i++)
    /* SPR 17884 FIX END */
                {
                    FAPI_cqiPduIndication_st *sr =
                        (FAPI_cqiPduIndication_st *)&msg_body->cqiPduInfo[i];
                    fprintf(outFilePtr, "{\n\tHandle %d\n", MAC_PHY_CONVERT_32(sr->handle));
                    fprintf(outFilePtr, "\tRNTI %d\n", MAC_PHY_CONVERT_16(sr->rnti));
                    fprintf(outFilePtr, "\tLength %d\n", MAC_PHY_CONVERT_16(sr->length));
                    fprintf(outFilePtr, "\tData Offset %d\n", sr->dataOffset);
                    fprintf(outFilePtr, "\tUL_CQI %d\n", sr->ulCqi);
#if defined(FLEXRAN)
/* SPR 21001 Changes start */
                    fprintf(outFilePtr, "\tRI %d\n", sr->ri[0]);
#else
                    fprintf(outFilePtr, "\tRI %d\n", sr->ri);

/* SPR 21001 Changes end */
#endif
                    fprintf(outFilePtr, "\tTiming Advance %d\n", sr->timingAdvance);
                    fprintf(outFilePtr, "}\n");
                }
#endif
                break;
            }

        case PHY_MEASUREMENT_REPORT_INDICATION:
            {
                fprintf(outFilePtr, "[ Message %s (%d) ]\n", fapiL1ApiMsgStr[index_msgId], msgId);
                FAPI_measReportInfo_st *msg_body =
                    (FAPI_measReportInfo_st *)&l1ApiMsg_p->msgBody[0];
                printSfnSf(outFilePtr, MAC_PHY_CONVERT_16(msg_body->sfnsf));
                fprintf(outFilePtr, "bitmask %d\n", msg_body->measReportInfo.bitmask);
    /* SPR 17884 FIX START */
                for(i = 0; i < MAX_INTERFERENCE_PER_PRB; i++)
    /* SPR 17884 FIX END */
                {
                    fprintf(outFilePtr, "\tinterferencePerPRB %d\n",
                            msg_body->measReportInfo.interferencePerPRB[i]);
                }
                fprintf(outFilePtr, "\tthermalNoise %d\n", msg_body->measReportInfo.thermalNoise);
                fprintf(outFilePtr, "\tdlRsTxPower %d\n", msg_body->measReportInfo.dlRsTxPower);

                break;
            }

        default:
            {
                break;
            }
    }
#endif
}
