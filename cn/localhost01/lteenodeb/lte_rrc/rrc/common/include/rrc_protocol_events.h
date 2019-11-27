/******************************************************************************
 *
 *   FILE NAME:
 *       rrc_protocol_events.h
 *
 *   DESCRIPTION:
 *       This is the header file for the LOCAL EVENTS implementation.
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   07 Jan 2013     L3 Team     ---------       Initial
 *
 *   Copyright (c) 2013, Aricent Inc. All Rights Reserved
 *
 * ******************************************************************************/
#ifndef _RRC_PROTOCOL_EVENTS_H_
#define _RRC_PROTOCOL_EVENTS_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "rrc_s1ap_uecc_intf.h" 


/****************************************************************************
 * Exported Definitions
 ****************************************************************************/
#define LOCAL_RRC_ERROR 37890
#define LOCAL_RRC_CONNECTION_SETUP 32768
#define LOCAL_RRC_CONNECTION_REJECT 32769
#define LOCAL_RRC_CONNECTION_REQUEST 32770
#define LOCAL_RRC_CONNECTION_RE_ESTABLISHMENT_REQUEST 32771
#define LOCAL_RRC_CONNECTION_RE_ESTABLISHMENT_REJECT 32772
#define LOCAL_RRC_CONNECTION_RELEASE 32773
#define LOCAL_RRC_DL_INFORMATION_TRANSFER 32774
#define LOCAL_RRC_CONNECTION_RECONFIGURATION 32776
#define LOCAL_RRC_SECURITY_MODE_COMMAND 32777
#define LOCAL_RRC_UE_CAPABILITY_ENQUIRY 32778
#define LOCAL_RRC_MEASUREMENT_REPORT 32779
#define LOCAL_RRC_CONNECTION_SETUP_COMPLETE 32780
#define LOCAL_RRC_CONNECTION_RECONFIGURATION_COMPLETE 32781
#define LOCAL_RRC_UL_INFORMATION_TRANSFER 32782
#define LOCAL_RRC_SECURITY_MODE_COMPLETE 32783
#define LOCAL_RRC_SECURITY_MODE_FAILURE 32784
#define LOCAL_RRC_UE_CAPABILITY_INFORMATION 32785
#define LOCAL_RRC_PAGING 32786
#define LOCAL_RRC_MASTER_INFORMATION_BLOCK 32787
#define LOCAL_RRC_SYSTEM_INFORMATION 32788
#define LOCAL_RRC_SYSTEM_INFORMATION_BLOCK_TYPE_1 32789
#define LOCAL_RRC_CONN_SETUP_ATTEMPT 37891
#define LOCAL_EVENT_NO_RESET_ACK_FROM_MME 37893
#define LOCAL_DOWNLINK_NAS_TRANSPORT 33793
#define LOCAL_ENB_STATUS_TRANSFER 33794
#define LOCAL_S1_ERROR_INDICATION 33795
#define LOCAL_EVENT_S1_ERROR_INDICATION 38102
#define LOCAL_S1_HANDOVER_CANCEL 33796
#define LOCAL_S1_HANDOVER_CANCEL_ACKNOWLEDGE 33797
#define LOCAL_X2_HANDOVER_CANCEL 34816
#define LOCAL_S1_HANDOVER_COMMAND 33798
#define LOCAL_S1_HANDOVER_FAILURE 33799
#define LOCAL_S1_HANDOVER_NOTIFY 33800
#define LOCAL_S1_HANDOVER_PREPARATION_FAILURE 33801
#define LOCAL_X2_HANDOVER_PREPARATION_FAILURE 34817
#define LOCAL_S1_HANDOVER_REQUEST 33802
#define LOCAL_S1_HANDOVER_REQUEST_ACKNOWLEDGE 33803
#define LOCAL_X2_HANDOVER_REQUEST 34818
#define LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE 34819
#define LOCAL_HANDOVER_REQUIRED 33804
#define LOCAL_S1_INITIAL_CONTEXT_SETUP_FAILURE 33805
#define LOCAL_S1_INITIAL_CONTEXT_SETUP_REQUEST 33806
#define LOCAL_S1_INITIAL_CONTEXT_SETUP_RESPONSE 33807
#define LOCAL_S1_INITIAL_UE_MESSAGE 33808
#define LOCAL_MME_STATUS_TRANSFER 33809
#define LOCAL_SN_STATUS_TRANSFER 34820
#define LOCAL_NAS_NON_DELIVERY_INDICATION 33810
#define LOCAL_EVENT_S1_NAS_NON_DELIVERY_INDICATION 38103
#define LOCAL_S1_PATH_SWITCH_REQUEST 33812
#define LOCAL_S1_PATH_SWITCH_REQUEST_ACKNOWLEDGE 33813
#define LOCAL_S1_PATH_SWITCH_REQUEST_FAILURE 33814
#define LOCAL_S1_RESET 33815
#define LOCAL_S1_RESET_ACKNOWLEDGE 33816
#define LOCAL_S1_ERAB_MODIFY_REQUEST 33817
#define LOCAL_S1_ERAB_MODIFY_RESPONSE 33818
#define LOCAL_S1_ERAB_RELEASE_COMMAND 33819
#define LOCAL_S1_ERAB_RELEASE_RESPONSE 33820
#define LOCAL_S1_ERAB_RELEASE_INDICATION 38221
#define LOCAL_EVENT_S1_ERAB_RELEASE_RESPONSE 38114
#define LOCAL_S1_ERAB_RELEASE_REQUEST 33821
#define LOCAL_S1_ERAB_SETUP_REQUEST 33822
#define LOCAL_S1_ERAB_SETUP_RESPONSE 33823
#define LOCAL_EVENT_S1_ERAB_SETUP_RESPONSE 38115
#define LOCAL_S1_SETUP_FAILURE 33824
#define LOCAL_S1_SETUP_REQUEST 33825
#define LOCAL_S1_SETUP_RESPONSE 33826
#define LOCAL_UE_CONTEXT_MODIFICATION_FAILURE 33828
#define LOCAL_UE_CONTEXT_MODIFICATION_REQUEST 33829
#define LOCAL_UE_CONTEXT_MODIFICATION_RESPONSE 33830
#define LOCAL_S1_UE_CONTEXT_RELEASE_COMMAND 33831
#define LOCAL_S1_UE_CONTEXT_RELEASE_COMPLETE 33832
#define LOCAL_S1_UE_CONTEXT_RELEASE_REQUEST 33833
#define LOCAL_UE_CONTEXT_RELEASE 34821
#define LOCAL_UE_CTXT_RELEASE_READY 37898
#define LOCAL_UPLINK_NAS_TRANSPORT 33835
#define LOCAL_UNEXPECTED_RRC_MSG 37903
#define LOCAL_INTEGRITY_VER_FAIL_RRC_MSG 37913
#define LOCAL_EVENT_X2_CONN_RELEASE 37915
#define LOCAL_RESET_REQUEST 34822
#define LOCAL_RESET_RESPONSE 34823
#define LOCAL_X2_SETUP_REQUEST 34824
#define LOCAL_X2_SETUP_RESPONSE 34825
#define LOCAL_EVENT_X2_RESET_REQUEST 38105
#define LOCAL_EVENT_X2_RESET_TO 38106
#define LOCAL_X2_ERROR_INDICATION 34826
#define LOCAL_EVENT_X2_ERROR_INDICATION 38107
#define LOCAL_X2_ENB_CONFIGURATION_UPDATE 34847
#define LOCAL_X2_ENB_CONFIGURATION_UPDATE_ACKNOWLEDGE 34848
#define LOCAL_X2_ENB_CONFIGURATION_UPDATE_FAILURE 34849
#define LOCAL_X2_SETUP_FAILURE 34850
#define LOCAL_S1_ENB_CONFIGURATION_UPDATE 33836
#define LOCAL_S1_ENB_CONFIGURATION_UPDATE_ACKNOWLEDGE 33837
#define LOCAL_S1_ENB_CONFIGURATION_UPDATE_FAILURE 33838
#define LOCAL_S1_MME_CONFIGURATION_UPDATE 33839
#define LOCAL_S1_MME_CONFIGURATION_UPDATE_ACKNOWLEDGE 33840
#define LOCAL_S1_MME_CONFIGURATION_UPDATE_FAILURE 33841
#define LOCAL_SCTP_SETUP_ATTEMPT 38000
#define LOCAL_SCTP_SETUP_RESULT 38001
#define LOCAL_S1_SETUP_ATTEMPT 38002
#define LOCAL_S1_SETUP_RESULT 38003
#define LOCAL_MME_CONFIGURATION_UPDATE_ERROR 38005
#define LOCAL_UE_CTXT_RELEASE_ATTEMPT_ENB 38006
#define LOCAL_EVENT_RRC_PROTOCOL_ERROR 38119
#define LOCAL_EVENT_X2_PRIVATE_MESSAGE 38076
#define LOCAL_RRC_CONNECTION_RE_ESTABLISHMENT 38081
#define LOCAL_RRC_CONNECTION_RE_ESTABLISHMENT_COMPLETE 38082
#define LOCAL_RRC_CONN_SETUP_REATT 38118
#define LOCAL_X2_RLF_INDICATION 38123
#define LOCAL_X2_HANDOVER_REPORT 38124
#define LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH 38140
#define LOCAL_UE_CTXT_MOD_FAIL 38216

#define RRC_MESSAGE_DISCARDED_SIZE 152

typedef struct  {
    /* Total length of the message */
    U32 length; 

    U64 timeStamp;  //Not Supported
    U16 core_id;    //Not Supported
    U16 event_id; 
    U32 EVENT_PARAM_LROP_IN_MSECS; //Not Supported
    /* Identity of the cell */
    U32 EVENT_PARAM_CELL_ID;
    U32 EVENT_PARAM_MMES1APID;

    /* The UE identity */
    U32 EVENT_PARAM_RAC_UE_REF;

    /* MOM Id */
    U32 EVENT_PARAM_EUTRANCELL_FROID;

} EVENT_UE_HEADER;

typedef struct  {
    /* Total length of the message */
    U32 length; 

    U64 timeStamp;  //Not Supported
    U16 core_id;    //Not Supported
    U16 event_id; 
    U32 EVENT_PARAM_LROP_IN_MSECS; //Not Supported
    /* Identity of the cell */
    U32 EVENT_PARAM_CELL_ID;

    /* MOM Id */
    U32 EVENT_PARAM_EUTRANCELL_FROID;

} EVENT_Cell_HEADER;

typedef struct  {
    /* Total length of the message */
    U32 length; 

    U64 timeStamp;  //Not Supported
    U16 core_id;    //Not Supported
    U16 event_id; 
    U32 EVENT_PARAM_LROP_IN_MSECS; //Not Supported
} EVENT_RBS_HEADER;

typedef struct  {
    /* Total length of the message */
    U32 length; 

    U64 timeStamp;  //Not Supported
    U16 core_id;    //Not Supported
    U16 event_id; 
    U32 EVENT_PARAM_LROP_IN_MSECS; //Not Supported

    /* Identity of the cell */
    U32 EVENT_PARAM_CELL_ID;
    U32 EVENT_PARAM_MMES1APID;

    /* MOM Id */
    U32 EVENT_PARAM_EUTRANCELL_FROID;


    /* The UE identity */
    U32 EVENT_PARAM_RAC_UE_REF;

    /* Direction of message
                  EVENT_VALUE_RECEIVED=0
                  EVENT_VALUE_SENT=1 */
    U32 EVENT_PARAM_MESSAGE_DIRECTION;

    /* Number of bytes in ASN.1 coded message */
    U32 EVENT_PARAM_L3MESSAGE_LENGTH;

    /* L3 message if following */
    U8 message[0];  //Containing ASN encoded hex dump
} EVENT_EXTERNAL_HEADER;

// An RRC message decoding problem has been detected
typedef struct  {
    EVENT_UE_HEADER header;

    /* Error type
        EVENT_VALUE_RRC_ASN1_DECODE_ERROR=0
        EVENT_VALUE_UNKNOWN_MESSAGE_TYPE=1
        EVENT_VALUE_UNKNOWN_IE_VALUE=2
        EVENT_VALUE_MISSING_EXPECTED_CONDITIONAL_IE=3
        EVENT_VALUE_RRC_LOGICAL_ERROR=4
         */
    U32 EVENT_PARAM_RRC_ERROR_TYPE;

    /* Number of bytes in ASN.1 coded message.
    Unit: byte */
    U32 EVENT_PARAM_L3MESSAGE_LENGTH;

    /* The message content has variable length. This parameter has a limit of 4 Kbytes according to 3GPP. The actual size used is variable as the recording functions only use the same amount of space as is required by the message so no ’wasted’ space will occur - i.e.even if the size is 4 Kbytes, this will only be fully used if the message contents are actually 4 Kbytes long. */
    // EVENT_PARAM_L3MESSAGE_CONTENTS variable size message
}LOCAL_RRC_ERROR_STRUCT;

// RRC CONNECTION REJECT sent. See 3GPP 36.331
typedef struct  {
    EVENT_UE_HEADER header;

    /* Reason for sending RRC Connection Reject. Currently only set when reject due to load.
        EVENT_VALUE_REJECT_DUE_TO_HIGH_LOAD=0
        EVENT_VALUE_OTHER_REASON=1
         */
    U32 EVENT_PARAM_RRC_REJECT_CAUSE;

    /* The establishment cause received in RRC Connection request
        EVENT_VALUE_EMERGENCY=0
        EVENT_VALUE_HIGH_PRIORITY_ACCESS=1
        EVENT_VALUE_MT_ACCESS=2
        EVENT_VALUE_MO_SIGNALLING=3
        EVENT_VALUE_MO_DATA=4
        EVENT_VALUE_DELAYTOLERANTACCESS_V1020=5
         */
    U32 EVENT_PARAM_RRC_ESTABL_CAUSE;

    /* Direction of message
                  EVENT_VALUE_RECEIVED=0
                  EVENT_VALUE_SENT=1 */
    U32 EVENT_PARAM_MESSAGE_DIRECTION;

    /* Number of bytes in ASN.1 coded message */
    U32 EVENT_PARAM_L3MESSAGE_LENGTH;

    /* L3 message if following */
    U8 message[0];  //Containing ASN encoded hex dump
}LOCAL_RRC_CONNECTION_REJECT_STRUCT;

// The RRC connection setup attempt is started
typedef struct  {
    EVENT_UE_HEADER header;

    /* Type of identifier used by UE in RRC Connection Request
        EVENT_VALUE_S_TMSI=0
        EVENT_VALUE_RANDOM_VALUE=1
         */
    U32 EVENT_PARAM_INITIAL_UE_IDENTITY_TYPE;

    /* The identity of the UE received in RRC Connection Request  */
    U8 EVENT_PARAM_INITIAL_UE_IDENTITY[5];

    /* The establishment cause received in RRC Connection request
        EVENT_VALUE_EMERGENCY=0
        EVENT_VALUE_HIGH_PRIORITY_ACCESS=1
        EVENT_VALUE_MT_ACCESS=2
        EVENT_VALUE_MO_SIGNALLING=3
        EVENT_VALUE_MO_DATA=4
        EVENT_VALUE_DELAYTOLERANTACCESS_V1020=5
         */
    U32 EVENT_PARAM_RRC_ESTABL_CAUSE;
}LOCAL_RRC_CONN_SETUP_ATTEMPT_STRUCT;


// RESET ACKNOWLEDGE has not been received from the remote node (MME), despite re-transmissions of RESET.
typedef struct  {
    EVENT_RBS_HEADER header;

    /* The GUMMEI of the released MME */
    //U8 EVENT_PARAM_GUMMEI[8];   //Not Supported
    /* GUMMEI format should be : <PLMN-id, MME Group-id, MME Code> */
    s1ap_gummei_t EVENT_PARAM_GUMMEI;

    /* The IP address (v4 or v6) of the released S1 at MME end */
    /* bug_8096 */
    U8 EVENT_PARAM_MME_IP_ADDRESS[16];
}LOCAL_EVENT_NO_RESET_ACK_FROM_MME_STRUCT;

// S1_ERROR_INDICATION received or sent over a common channel/dedicatd channel/stream. See 3GPP 36.413 section 9.2.1.3. Note that this local event is NOT mapped to an external pmEvent. Used to extract CAUSE values.
typedef struct  {
    EVENT_RBS_HEADER header;

    /* Direction of message (received or sent) 
        EVENT_VALUE_RECEIVED=0
        EVENT_VALUE_SENT=1
         */
    U32 EVENT_PARAM_MESSAGE_DIRECTION;

    /* This parameter holds all Cause-Group values that are described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.     Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE
        EVENT_VALUE_RADIO_NETWORK_LAYER_GROUP=0
        EVENT_VALUE_TRANSPORT_LAYER_GROUP=1
        EVENT_VALUE_NAS_GROUP=2
        EVENT_VALUE_PROTOCOL_GROUP=3
        EVENT_VALUE_MISCELLANEOUS_GROUP=4
         */
    U32 EVENT_PARAM_3GPP_CAUSE_GROUP;

    /* This parameter holds any possible Cause value described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.  Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE_GROUP */
    U32 EVENT_PARAM_3GPP_CAUSE;
}LOCAL_EVENT_S1_ERROR_INDICATION_STRUCT;

// S1_HANDOVER_COMMAND received. See 3GPP 36.413
typedef struct  {
    EVENT_UE_HEADER header;

    /* Bitmap pointing out UE ERAB Ids that failed to be prepared in target cell. 0..15. (LSB) Bit position 0=ERAB Id 0, position 1=ERAB Id 1 etc. See X2 3GPP 36.423 */
    U32 EVENT_PARAM_HO_OUT_PREP_ERAB_FAIL_BITMAP;

    /* Direction of message
                  EVENT_VALUE_RECEIVED=0
                  EVENT_VALUE_SENT=1 */
    U32 EVENT_PARAM_MESSAGE_DIRECTION;

    /* Number of bytes in ASN.1 coded message */
    U32 EVENT_PARAM_L3MESSAGE_LENGTH;

    /* L3 message if following */
    U8 message[0];  //Containing ASN encoded hex dump
}LOCAL_S1_HANDOVER_COMMAND_STRUCT;

// X2_HANDOVER_REQUEST_ACKNOWLEDGE received or sent. See 3GPP 36.423
typedef struct  {
    EVENT_UE_HEADER header;

    /* Bitmap pointing out UE ERAB Ids that failed to be prepared in target cell. 0..15. (LSB) Bit position 0=ERAB Id 0, position 1=ERAB Id 1 etc. See X2 3GPP 36.423 */
    U32 EVENT_PARAM_HO_OUT_PREP_ERAB_FAIL_BITMAP;

    /* Direction of message
                  EVENT_VALUE_RECEIVED=0
                  EVENT_VALUE_SENT=1 */
    U32 EVENT_PARAM_MESSAGE_DIRECTION;

    /* Number of bytes in ASN.1 coded message */
    U32 EVENT_PARAM_L3MESSAGE_LENGTH;

    /* L3 message if following */
    U8 message[0];  //Containing ASN encoded hex dump
}LOCAL_X2_HANDOVER_REQUEST_ACKNOWLEDGE_STRUCT;

// NAS_NON_DELIVERY_INDICATION sent. See 3GPP 36.413 section 9.2.1.3 Note that this local event is NOT mapped to an external pmEvent. Used to extract CAUSE values.
typedef struct  {
    EVENT_UE_HEADER header;

    /* This parameter holds all Cause-Group values that are described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.     Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE
        EVENT_VALUE_RADIO_NETWORK_LAYER_GROUP=0
        EVENT_VALUE_TRANSPORT_LAYER_GROUP=1
        EVENT_VALUE_NAS_GROUP=2
        EVENT_VALUE_PROTOCOL_GROUP=3
        EVENT_VALUE_MISCELLANEOUS_GROUP=4
         */
    U32 EVENT_PARAM_3GPP_CAUSE_GROUP;

    /* This parameter holds any possible Cause value described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.  Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE_GROUP */
    U32 EVENT_PARAM_3GPP_CAUSE;
}LOCAL_EVENT_S1_NAS_NON_DELIVERY_INDICATION_STRUCT;

// S1_ERAB_RELEASE_RESPONSE sent. See 3GPP 36.413. Used to extract CAUSE values.
typedef struct  {
    EVENT_UE_HEADER header;

    /* Array with 3GPP S1 Cause Group values for each ERAB in a failed release, element position indicates E-RAB Id and element value indicates which Failure Cause Group value was applicable for that E-RAB Id.  Needs to be used jointly with EVENT_ARRAY_ERAB_RELEASE_FAILURE_3GPP_CAUSE */
    U32 EVENT_ARRAY_ERAB_RELEASE_FAILURE_3GPP_CAUSE_GROUP[16];

    /* Array with 3GPP S1 Cause values for each ERAB in a failed release, element position indicates E-RAB Id and element value indicates which Failure Cause value was applicable for that E-RAB Id.  Needs to be used jointly with EVENT_ARRAY_ERAB_RELEASE_FAILURE_3GPP_CAUSE_GROUP */
    U32 EVENT_ARRAY_ERAB_RELEASE_FAILURE_3GPP_CAUSE[16];
}LOCAL_EVENT_S1_ERAB_RELEASE_RESPONSE_STRUCT;

// S1_ERAB_SETUP_RESPONSE sent. See 3GPP 36.413. Used to extract CAUSE values.
typedef struct  {
    EVENT_UE_HEADER header;

    /* Array with 3GPP S1 Cause Group values for each ERAB in a failed setup, element position indicates E-RAB Id and element value indicates which failure Cause Group value was applicable for that E-RAB.   Needs to be used jointly with EVENT_ARRAY_ERAB_SETUP_FAILURE_3GPP_CAUSE */
    U32 EVENT_ARRAY_ERAB_SETUP_FAILURE_3GPP_CAUSE_GROUP[16];

    /* Array with 3GPP S1 Cause values for each ERAB in a failed setup, element position indicates E-RAB Id and element value indicates which failure Cause value was applicable for that E-RAB.       Needs to be used jointly with EVENT_ARRAY_ERAB_SETUP_FAILURE_3GPP_CAUSE_GROUP */
    U32 EVENT_ARRAY_ERAB_SETUP_FAILURE_3GPP_CAUSE[16];
}LOCAL_EVENT_S1_ERAB_SETUP_RESPONSE_STRUCT;

// S1_UE_CONTEXT_RELEASE_COMMAND received. See 3GPP 36.413
typedef struct  {
    EVENT_UE_HEADER header;

    /* This parameter holds all Cause-Group values that are described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.     Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE
        EVENT_VALUE_RADIO_NETWORK_LAYER_GROUP=0
        EVENT_VALUE_TRANSPORT_LAYER_GROUP=1
        EVENT_VALUE_NAS_GROUP=2
        EVENT_VALUE_PROTOCOL_GROUP=3
        EVENT_VALUE_MISCELLANEOUS_GROUP=4
         */
    U32 EVENT_PARAM_3GPP_CAUSE_GROUP;

    /* This parameter holds any possible Cause value described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.  Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE_GROUP */
    U32 EVENT_PARAM_3GPP_CAUSE;

    /* Direction of message
                  EVENT_VALUE_RECEIVED=0
                  EVENT_VALUE_SENT=1 */
    U32 EVENT_PARAM_MESSAGE_DIRECTION;

    /* Number of bytes in ASN.1 coded message */
    U32 EVENT_PARAM_L3MESSAGE_LENGTH;

    /* L3 message if following */
    U8 message[0];  //Containing ASN encoded hex dump
}LOCAL_S1_UE_CONTEXT_RELEASE_COMMAND_STRUCT;

// The Initial Context Release attempt is completed
typedef struct  {
    EVENT_UE_HEADER header;
}LOCAL_UE_CTXT_RELEASE_READY_STRUCT;


// An uplink RRC message was discarded because it was received in an unexpected state. That is,  no active process could accept the RRC message
typedef struct  {
    EVENT_UE_HEADER header;

    /* Describes which RRC message it is
        EVENT_VALUE_MEASUREMENT_REPORT=0
        EVENT_VALUE_RRC_CONN_RECONFIG_COMPL=1
        EVENT_VALUE_RRC_CONN_RECONFIG_FAIL=2
        EVENT_VALUE_RRC_CONN_REESTABL_COMPL=3
        EVENT_VALUE_RRC_CONN_SETUP_COMPL=4
        EVENT_VALUE_RRC_STATUS=5
        EVENT_VALUE_SEC_MODE_COMPL=6
        EVENT_VALUE_SEC_MODE_FAIL=7
        EVENT_VALUE_UE_CAP_INFO=8
        EVENT_VALUE_UL_INFO_TRANSFER=9
        EVENT_VALUE_RRC_CONN_REESTABL_REQ=10
        EVENT_VALUE_RRC_CONN_REQ=11
        EVENT_VALUE_INVALID=12
         */
    U32 EVENT_PARAM_RRC_MESSAGE_ID;
}LOCAL_UNEXPECTED_RRC_MSG_STRUCT;

// The X2 connection to another eNodeB has ended, either released by the local or remote node, or simply lost.   
typedef struct  {
    EVENT_RBS_HEADER header;

    /* The Global eNB Id. Globally unique Id for an eNB. */
    U8 EVENT_PARAM_GENBID[4];

    /* The IP address (v4 or v6) of the neighbor/remote eNB */
    U8 EVENT_PARAM_ENB_IP_ADDRESS[16];

    /* The cause of the release
        EVENT_VALUE_NO_X2_CONN_EXISTED=0
        EVENT_VALUE_X2_CONN_LOST=1
         */
    U32 EVENT_PARAM_X2_RELEASE_CAUSE;
}LOCAL_EVENT_X2_CONN_RELEASE_STRUCT;

// RESET_REQUEST received or sent. See 3GPP 36.423 section 9.2.6
typedef struct  {
    EVENT_RBS_HEADER header;

    /* Direction of message (received or sent) 
        EVENT_VALUE_RECEIVED=0
        EVENT_VALUE_SENT=1
         */
    U32 EVENT_PARAM_MESSAGE_DIRECTION;

    /* This parameter holds all Cause-Group values that are described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.     Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE
        EVENT_VALUE_RADIO_NETWORK_LAYER_GROUP=0
        EVENT_VALUE_TRANSPORT_LAYER_GROUP=1
        EVENT_VALUE_NAS_GROUP=2
        EVENT_VALUE_PROTOCOL_GROUP=3
        EVENT_VALUE_MISCELLANEOUS_GROUP=4
         */
    U32 EVENT_PARAM_3GPP_CAUSE_GROUP;

    /* This parameter holds any possible Cause value described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.  Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE_GROUP */
    U32 EVENT_PARAM_3GPP_CAUSE;

    /* The IP address (v4 or v6) of the neighbor/remote eNB */
    U8 EVENT_PARAM_ENB_IP_ADDRESS[16];
}LOCAL_EVENT_X2_RESET_REQUEST_STRUCT;

// RESET_RESPONSE received or sent. See 3GPP 36.423
typedef struct  {
    EVENT_UE_HEADER header;

    /* The IP address (v4 or v6) of the neighbor/remote eNB */
    U8 EVENT_PARAM_ENB_IP_ADDRESS[16];

    /* Direction of message
                  EVENT_VALUE_RECEIVED=0
                  EVENT_VALUE_SENT=1 */
    U32 EVENT_PARAM_MESSAGE_DIRECTION;

    /* Number of bytes in ASN.1 coded message */
    U32 EVENT_PARAM_L3MESSAGE_LENGTH;

    /* L3 message if following */
    U8 message[0];  //Containing ASN encoded hex dump
}LOCAL_RESET_RESPONSE_STRUCT;

// At expiry of the acknowledgement timer, indicating no RESET RESPONSE message was received from the peer eNB.
typedef struct  {
    EVENT_RBS_HEADER header;

    /* The IP address (v4 or v6) of the neighbor/remote eNB */
    U8 EVENT_PARAM_ENB_IP_ADDRESS[16];
}LOCAL_EVENT_X2_RESET_TO_STRUCT;

// X2_ERROR_INDICATION received or sent over a common channel/dedicatd channel/stream. See 3GPP 36.423 section 9.2.6. Note that this local event is not mapped to an external pmEvent. Used to extract CAUSE values.
typedef struct  {
    EVENT_RBS_HEADER header;

    /* Direction of message (received or sent) 
        EVENT_VALUE_RECEIVED=0
        EVENT_VALUE_SENT=1
         */
    U32 EVENT_PARAM_MESSAGE_DIRECTION;

    /* This parameter holds all Cause-Group values that are described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.     Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE
        EVENT_VALUE_RADIO_NETWORK_LAYER_GROUP=0
        EVENT_VALUE_TRANSPORT_LAYER_GROUP=1
        EVENT_VALUE_NAS_GROUP=2
        EVENT_VALUE_PROTOCOL_GROUP=3
        EVENT_VALUE_MISCELLANEOUS_GROUP=4
         */
    U32 EVENT_PARAM_3GPP_CAUSE_GROUP;

    /* This parameter holds any possible Cause value described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.  Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE_GROUP */
    U32 EVENT_PARAM_3GPP_CAUSE;
}LOCAL_EVENT_X2_ERROR_INDICATION_STRUCT;


// At failed MAC-I verification of a RRC message
typedef struct  {
    EVENT_UE_HEADER header;
}LOCAL_INTEGRITY_VER_FAIL_RRC_MSG_STRUCT;

// When SCTP Setup is attempted
typedef struct  {
    EVENT_RBS_HEADER header;

    /* The IP address (v4 or v6) used in DNS lookup */
    U8 EVENT_PARAM_IP_ADDRESS[16];
}LOCAL_SCTP_SETUP_ATTEMPT_STRUCT;

// When SCTP setup succeeded or failed
typedef struct  {
    EVENT_RBS_HEADER header;

    /* The result of the SCTP setup
        EVENT_VALUE_SUCCESS=0
        EVENT_VALUE_FAILURE=1
        EVENT_VALUE_INVALID_FAMILY=2
         */
    U32 EVENT_PARAM_SCTP_SETUP_RESULT;

    /* The type of remote endpoint for the SCTP connection 
        EVENT_VALUE_MME=0
        EVENT_VALUE_EXTERNAL_ENB=1
         */
    U32 EVENT_PARAM_SCTP_ENDPOINT_TYPE;

    /* The IP address (v4 or v6) used in DNS lookup */
    U8 EVENT_PARAM_IP_ADDRESS[16];

    /* The node that initiates the SCTP setup
        EVENT_VALUE_LOCAL=0
        EVENT_VALUE_REMOTE=1
         */
    U32 EVENT_PARAM_SCTP_CAUSE;
}LOCAL_SCTP_SETUP_RESULT_STRUCT;

// When S1AP setup Request is sent
typedef struct  {
    EVENT_RBS_HEADER header;

    /* The IP address (v4 or v6) of the released S1 at MME end */
    /* bug_8096 */
    U8 EVENT_PARAM_MME_IP_ADDRESS[16];
}LOCAL_S1_SETUP_ATTEMPT_STRUCT;

// When S1AP Setup Response is received
typedef struct  {
    EVENT_RBS_HEADER header;

    /* The result of the S1AP Setup procedure
        EVENT_VALUE_SUCCESS=0
        EVENT_VALUE_S1AP_PROTOCOL_ERROR=1
        EVENT_VALUE_S1AP_FAILURE=2
        EVENT_VALUE_S1AP_TIMEOUT=3
        EVENT_VALUE_GUMMEI_CONFLICT_PROTOCOL_ERROR=4
        EVENT_VALUE_MAX_GUGI_EXCEEDED=5
        EVENT_VALUE_SCTP_SHUTDOWN=6
         */
    U32 EVENT_PARAM_S1_SETUP_RESULT;

    /* The IP address (v4 or v6) of the released S1 at MME end */
    /* bug_8096 */
    U8 EVENT_PARAM_MME_IP_ADDRESS[16];

    /* The GUMMEI of the released MME */
    //U8 EVENT_PARAM_GUMMEI[8];   //Not Supported
    /* GUMMEI format should be : <PLMN-id, MME Group-id, MME Code> */
    s1ap_gummei_t EVENT_PARAM_GUMMEI;

    /* MME name  */
    U8 EVENT_PARAM_MME_NAME[150];

    /* This parameter holds all Cause-Group values that are described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.     Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE
        EVENT_VALUE_RADIO_NETWORK_LAYER_GROUP=0
        EVENT_VALUE_TRANSPORT_LAYER_GROUP=1
        EVENT_VALUE_NAS_GROUP=2
        EVENT_VALUE_PROTOCOL_GROUP=3
        EVENT_VALUE_MISCELLANEOUS_GROUP=4
         */
    U32 EVENT_PARAM_3GPP_CAUSE_GROUP;

    /* This parameter holds any possible Cause value described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.  Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE_GROUP */
    U32 EVENT_PARAM_3GPP_CAUSE;
}LOCAL_S1_SETUP_RESULT_STRUCT;

// When a MME CONFIGURATION UPDATE Failure is sent due to violation of  network configuration supported by the eNodeB
typedef struct  {
    EVENT_RBS_HEADER header;

    /* The IP address (v4 or v6) of the released S1 at MME end */
    /* bug_8096 */
    U8 EVENT_PARAM_MME_IP_ADDRESS[16];

    /* The GUMMEI of the released MME */
    //U8 EVENT_PARAM_GUMMEI[8];  //Not Supported
    /* GUMMEI format should be : <PLMN-id, MME Group-id, MME Code> */
    s1ap_gummei_t EVENT_PARAM_GUMMEI;
}LOCAL_MME_CONFIGURATION_UPDATE_ERROR_STRUCT;

// The Initial Context Release attempt is started
typedef struct  {
    EVENT_UE_HEADER header;

    /* ARP Priority Level for each ERAB in the request */
    U32 EVENT_ARRAY_ERAB_SETUP_REQ_ARP[16];

    /* ARP PCI  for each E-RAB in the request. */
    U32 EVENT_ARRAY_ERAB_SETUP_REQ_PCI[16];

    /* ARP PVI  for each E-RAB in the request. */
    U32 EVENT_ARRAY_ERAB_SETUP_REQ_PVI[16];

    /* This parameter holds all Cause-Group values that are described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.     Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE
        EVENT_VALUE_RADIO_NETWORK_LAYER_GROUP=0
        EVENT_VALUE_TRANSPORT_LAYER_GROUP=1
        EVENT_VALUE_NAS_GROUP=2
        EVENT_VALUE_PROTOCOL_GROUP=3
        EVENT_VALUE_MISCELLANEOUS_GROUP=4
         */
    U32 EVENT_PARAM_3GPP_CAUSE_GROUP;

    /* This parameter holds any possible Cause value described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.  Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE_GROUP */
    U32 EVENT_PARAM_3GPP_CAUSE;
}LOCAL_UE_CTXT_RELEASE_ATTEMPT_ENB_STRUCT;


// A faulty RRC message was received. Possible fault causes are: protocol error, ASN.1 violation, encoding error, IE type non-exists or not implemented.
typedef struct  {
    EVENT_RBS_HEADER header;

    /* Name of the message being discarded due to unknown IE in RRC. */
    U8 EVENT_PARAM_RRC_MESSAGE_DISCARDED[RRC_MESSAGE_DISCARDED_SIZE];
}LOCAL_EVENT_RRC_PROTOCOL_ERROR_STRUCT;

// The event is generated when there is a mismatch regarding what ciphering and integrity algorithms that the UE supports and what the eNB supports (i.e. none of the algorithms supported by the UE are supported by the eNB). This check is performed during Initial Context Setup and incoming handover.
typedef struct  {
    EVENT_UE_HEADER header;
}LOCAL_EVENT_CIPHERING_INTEGRITY_ALG_MISMATCH_STRUCT;


// Local event generated at transmission of S1 AP UE CONTEXT MODIFICATION FAILURE
typedef struct  {
    EVENT_UE_HEADER header;

    /* This parameter holds all Cause-Group values that are described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.     Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE
        EVENT_VALUE_RADIO_NETWORK_LAYER_GROUP=0
        EVENT_VALUE_TRANSPORT_LAYER_GROUP=1
        EVENT_VALUE_NAS_GROUP=2
        EVENT_VALUE_PROTOCOL_GROUP=3
        EVENT_VALUE_MISCELLANEOUS_GROUP=4
         */
    U32 EVENT_PARAM_3GPP_CAUSE_GROUP;

    /* This parameter holds any possible Cause value described in the S1AP 3GPP specification, Section 9.2.1.3 in TS 36.413 or in the X2AP 3GPP specification, Section 9.2.6 in TS 36.423.  Needs to be used jointly with EVENT_PARAM_3GPP_CAUSE_GROUP */
    U32 EVENT_PARAM_3GPP_CAUSE;
}LOCAL_UE_CTXT_MOD_FAIL_STRUCT;

#endif /* _RRC_PROTOCOL_EVENTS_H_ */
