/*****************************************************************************
 *    FILE NAME:
 *      s_cogbl.h
 *
 *    DESCRIPTION:
 *          This file has    all the    SCTP global definitions
 *
 *    DATE        NAME       REFERENCE       REASON
 *    -----------------------------------------------------
 *    02June 2000     Sandeep Mahajan  -       .Original Creation
 *    19Sep03         Manish Rajpal            Rel 5.0
 *    07 Oct 2003     Vivek Bansal    Rel 5.0  Changes for CSPL
 *    08 Jul 2004     Nitin Khanna    Rel 5.0  SPR 6854
 *
 *    07 JUL 2008     Shikha Kathpal  Rel 6.2.0 RFC 4960 UPGRADE
 *    08 JUL 2008     Raza Mohamed    Rel 6.2.0 RFC 4960 UPGRADE
 *    Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ****************************************************************************/
#ifndef __FILE_sctp_global_SEEN__
#define __FILE_sctp_global_SEEN__

#if defined(SCTP_CMN_RECV_POLL_THREAD) &&  (defined(SCTP_CLOCK_THREAD) ||  defined(SCTP_NAME_RESOLV_THREAD) || \
             defined(SCTP_INDICATION_THREAD))
# error Invalid flag combination used with SCTP_CMN_RECV_POLL_THREAD 
#endif



#define SCTP_UNUSED_VARIABLE(a)  a=a

/* Changes FOR CSR#1-5037386 */
/* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND */
/* In case Congestion level remains non-zero and TSN value
 * of oldest node in transmit queue does not changes while
 * sending SCTP_NON_RESP_THRESH number of consecutive HBs
 * then possibaly association has reached a deadlock state
 * So stack ABORTs the association.*/
#define SCTP_NON_RESP_THRESH        16
/* CHANGES FOR CONGESTION DEADLOCK RECOVERY WORKAROUND ENDS*/
/* Changes FOR CSR#1-5037386 */

/* SCTP Compile Time Constants */
#define  SCTP_DELAYACK_TIME         200   /* delay ack timer */
#define  SCTP_MAX_DELAYACK_TIME     500   /* max delay ack timer */

#define  SCTP_SACK_FREQUENCY         2   /* sack frequency */
#define  SCTP_MAX_SACK_FREQUENCY     5   /* max sack frequency */

#define  SCTP_API_VERSION_NO        6
#define  SCTP_MINOR_VERSION_NO      2

#define  SCTP_MAX_NUM_PACKETS_RECV  10
#define  SCTP_NULL_ADAPTION_IND     0
/* This    may be moved to    timer module */
#define  SCTP_NULL_TIMER_ID         0
#define  SCTP_NULL                  ((void*) 0)
#define  SCTP_INVALID_ENDPOINT      0xffffffff
#define  SCTP_INVALID_ASSOC         0xffffffff

#define  SCTP_U32bit_MAX            0xffffffff
#define  SCTP_MAX_NOTF_Q_SIZE       500
#define SCTP_MAX_NUM_APIS           SCTP_MAX_NOTF_Q_SIZE
#define SCTP_SM_MAX_NUM_APIS        10

/*  sctp default config values */
#define SCTP_MAX_SERVER_CREATED_PORT    10

/* Units Multiply factor for HB intervel parameter feild for API
 * sctp_api_config_stack_params()  If HB taken in secs this constant
 * should be 1000 else if it is in millisec it should be 1.
 * ADDED FOR CSR#1-4982332*/
#define  SCTP_CONFIG_STACK_PARAMS_HB_UNIT   1000

/* default rto init 3 sec */
#define  SCTP_DEFAULT_RTO_INIT              3000
/* default rto max  60 sec */
#define  SCTP_DEFAULT_RTO_MAX               60000
#define  SCTP_MIN_RTO_MAX                   1000
#define  SCTP_MAX_RTO_MAX                   120000
/* default rto min  1 sec */
#define  SCTP_DEFAULT_RTO_MIN               1000
#define  SCTP_MIN_RTO_MIN                   10
#define  SCTP_MAX_RTO_MIN                   5000
/* default max burst  4 */
#define  SCTP_DEFAULT_MAX_BURST             4
/*Rel 6.2.0 change start*/
/*HB.Max.Burst*/
#define SCTP_HB_MAX_BURST                   1
/*Rel 6.2.0 change end */
/* default rto alpha percent 12 */
#define  SCTP_DEFAULT_RTO_ALPHA_PERCENT     12
/* default rto beta percent 25 */
#define  SCTP_DEFAULT_RTO_BETA_PERCENT      25
/* default valid cookie life 60 sec */
#define  SCTP_DEFAULT_VALID_COOKIE_LIFE     60000
#define  SCTP_MIN_VALID_COOKIE_LIFE         5000
#define  SCTP_MAX_VALID_COOKIE_LIFE         120000
/* default assoc max_retrans  8 attempts */
#define  SCTP_DEFAULT_ASSOC_MAX_RTX         8
/* default path max retrans   5 attempts */
#define  SCTP_DEFAULT_PATH_MAX_RTX          5
/* default init max retrans  8 attempts */
#define  SCTP_DEFAULT_MAX_INIT_RTX          8

/* max number of init trans per path */
#define  SCTP_DEFAULT_MAX_INIT_PATH_RTX     2

/* default init timeout value. The init procedure will be executed to max
 * this duration.
 */
#define SCTP_DEFAULT_INIT_TIMEOUT           0
/* default congestion level % */
#define  SCTP_DEFAULT_NO_CONG_LEVEL_ONSET         0
#define  SCTP_DEFAULT_NO_CONG_LEVEL_ABATE         0
#define  SCTP_DEFAULT_LOW_CONG_LEVEL_ONSET        40
#define  SCTP_DEFAULT_LOW_CONG_LEVEL_ABATE        35
#define  SCTP_DEFAULT_MED_CONG_LEVEL_ONSET        60
#define  SCTP_DEFAULT_MED_CONG_LEVEL_ABATE        55
#define  SCTP_DEFAULT_HIGH_CONG_LEVEL_ONSET       80
#define  SCTP_DEFAULT_HIGH_CONG_LEVEL_ABATE       75
/* default heart beat interval  30 sec */
#define  SCTP_DEFAULT_HB_INTERVAL           30000
#define  SCTP_MIN_HB_INTERVAL               1000
#define  SCTP_MAX_HB_INTERVAL               300000

#define  SCTP_MAX_IP_SCATTER_BUFFERS        SCTP_MAX_CHUNKS_PERDATAGRAM + 5
#ifdef SCTP_NAME_RESOLV_THREAD
/* Max number of TL buffers to have in list */
#define     SCTP_MAX_TL_BUFFER_NUM  1000
#else
/* Max number of TL buffers  need only 10 */
#define     SCTP_MAX_TL_BUFFER_NUM  1000
#endif

#define    SCTP_BUFFER_QUEUE_NUM     3
#define    SCTP_MAX_TRANSPORT_ADDR   3    /* Max Transport Addresses */
#define    SCTP_MAX_TOTAL_ASSOC      200  /* Max number of association*/
#define    SCTP_MAX_TOTAL_APP        10   /* Max number of application*/
#define    SCTP_MAX_BACKLOG          SCTP_MAX_LISTEN_BACKLOG
#define    SCTP_MAX_NUMSTREAM        32   /* Max streams SCTP can have    */
#define    SCTP_MAX_NUMSACKFRAGS     100  /* Max number of SACK fragments */
#define    SCTP_MAXDUPLICATE_TSNS    10   /* Max number of tsn    per SACK */
#define    SCTP_MAX_COOKIE_SIZE      2048 /* Max Cookie Size from peer    */

#define    SCTP_MAX_UDP_RESERVED_PORT 1023 /* Reserved UDP post ceiling */

#define    SCTP_MAX_CLIENT_SUPPORTED  SCTP_MAX_TOTAL_APP + 1
/**************************** PMTU Parameters ******************************/

/*
 * Following constant corrosponds to max MTU size SCTP can support,
 * officially max MTU size is 64k, change this if needed
 */

#define  SCTP_MAX_PMTU_NODES          100
#define  SCTP_PMTU_DISCOVER_TIME      100
#define  SCTP_LOWEST_PMTU             68


#define  SCTP_MAX_MTUSIZE          1500
#define  SCTP_MIN_MTUSIZE          508
#define  SCTP_MAX_SEND_BUFFERS         1000
#ifdef SCTP_RED_SUPPORT
#define    SCTP_MAX_UDPSIZE        65536 /*To support redundancy database*/
#define    SCTP_MAX_RED_MEM        65536
#else
#define    SCTP_MAX_UDPSIZE        1500  /*No need of 64K*/
#endif
/*
* This should be max mtu size minus ip header,udp header,
* sctp header,sctp data    chunk header (96 bytes max)
*/

#define  SCTP_MAX_ALLHEADERS_LEN     96

#define  SCTP_MIN_FRAGSIZE           100
#define  SCTP_MED_FRAGSIZE           500
#define  SCTP_MAX_FRAGSIZE           1500

#define SCTP_MIN_INIT_ARWND      1500

/*Thus the value of SCTP_MAX_FRAGSIZE is 8070 octets*/

/* This    is for case when pathmtu is disabled */
#define  SCTP_DEFAULT_MTUSIZE           1500

#define  SCTP_PMTU_REDISCOVER_TIME      1200000  /*    20 minutes */
#define  SCTP_PMTU_DISCOVER_FAIL_TIME   20000    /*    20 seconds */


/* MAC key update timer    value */
#define  SCTP_MAC_KEY_UPDATE_TIME       1200000    /* 20 minutes */


/***************** ECN related parameters *********************************/
/* rfc 4960 upgrade  fix start */
#define  SCTP_ECN_BIT                0X03 /* ECT and CE bit combined to ECN field */
/* rfc 4960 upgrade fix end */
#define  SCTP_ECT_BIT               0x02      /* ECNT bit */
#define  SCTP_CE_BIT                0x01      /* CE    bit */


#define  SCTP_ONE_SECOND_TIME       1000 /* 1 Second */
#define  SCTP_BUNDLING_TIME         100      /* 100 Milli Second */
#define  SCTP_MAX_LIFE_TIME         0XFFFFFF /* Invalid life time*/

#define  SCTP_BUNDLING_THRESHOLD    200

/******************* SCTP Common Header Offsets **************************/
#define       SCTP_OFFSET_SRCPORT           0
#define       SCTP_OFFSET_DSTPORT           2
#define       SCTP_OFFSET_VERTAG            4
#define       SCTP_OFFSET_CKSUM             8
#define       SCTP_OFFSET_BODY              12

#define       SCTP_OFFSET_FIRSTCHUNK      12

#define       SCTP_MIN_TLDATAGRAM_SIZE    16

/*********************** SCTP Chunk Offsets *********************************/
#define       SCTPCHUNKOFFSET_TYPE           0
#define       SCTPCHUNKOFFSET_FLAGS       1
#define       SCTPCHUNKOFFSET_LENGTH      2
#define       SCTPCHUNKOFFSET_BODY           4

/************************** SCTP Chunk Ids **************************/
#define       SCTPCHUNK_DATA              0x00
#define       SCTPCHUNK_INIT              0x01
#define       SCTPCHUNK_INITACK           0x02
#define       SCTPCHUNK_SACK              0x03
#define       SCTPCHUNK_HEARTBEAT         0x04
#define       SCTPCHUNK_HEARTBEATACK      0x05
#define       SCTPCHUNK_ABORT             0x06
#define       SCTPCHUNK_SHUTDOWN          0x07
#define       SCTPCHUNK_SHUTDOWNACK       0x08
#define       SCTPCHUNK_ERROR             0x09
#define       SCTPCHUNK_COOKIE            0x0A
#define       SCTPCHUNK_COOKIEACK         0x0B
#define       SCTPCHUNK_ECNE              0x0C
#define       SCTPCHUNK_CWR               0x0D
#define       SCTPCHUNK_SHUTDOWNCOMP      0x0E

/*
 * Follwoing id are not same as chunk type values specified in the draft
 * These are only used in the code to index the various arrays used
 */
#ifdef SCTP_DYNAMIC_IP_SUPPORT

#define       SCTPCHUNK_ASCONF            0x0F
#define       SCTPCHUNK_ASCONFACK         0x10

/*Values as used in the chunks*/
#define       SCTPCHUNK_TYPE_ASCONF       0xC1
#define       SCTPCHUNK_TYPE_ASCONFACK    0x80


#define       SCTP_MAX_CHUNKID               16

#else

#define       SCTP_MAX_CHUNKID               14

#endif

#define       SCTP_NUM_CHUNK_TYPES           SCTP_MAX_CHUNKID    + 1
#define       SCTP_MAX_CHUNKS_PERDATAGRAM 100

/**************** Chunk Length Constants *********************************/
#define       SCTP_MIN_CHUNK_LEN           4

#define       SCTP_MIN_INITCHUNK_LEN      20
#define       SCTP_MIN_INITACKCHUNK_LEN   20
#define       SCTP_MIN_SACKCHUNK_LEN      16
#define       SCTP_MIN_HBCHUNK_LEN         4
#define       SCTP_MIN_HBACKCHUNK_LEN      4
#define       SCTP_MIN_DATACHUNK_LEN      16

#define       SCTP_CHUNK_LEN_COOKIENACK   4
#define       SCTP_CHUNK_LEN_SHUTDOWN     8
#define       SCTP_CHUNK_LEN_SHUTDOWNACK  4
#define       SCTP_CHUNK_LEN_SHUTDOWNCOMP 4
#define       SCTP_CHUNK_LEN_CWR          8
#define       SCTP_CHUNK_LEN_ECNE         8

/******************* SCTP Optional Parameter Offset ********************/
#define       SCTPPARAMOFFSET_TYPE           0
#define       SCTPPARAMOFFSET_LENGTH      2
#define       SCTPPARAMOFFSET_BODY           4

/********************* Optional Parameter Codes *********************/
#define       SCTPVARPAR_IPV4ADDRESS      0x0005
#define       SCTPVARPAR_IPV6ADDRESS      0x0006
#define       SCTPVARPAR_COOKIE           0x0007
#define       SCTPVARPAR_UNKNOWNPAR       0x0008
#define       SCTPVARPAR_COOKIEPRESV      0x0009
#define       SCTPVARPAR_ECNCAPABLE       0x8000
#define       SCTPVARPAR_HOSTNAME         0x000b   /* Hostname  */
#define       SCTPVARPAR_SUPADDRTYPE      0x000c   /* Supported Address Types */

#define       SCTPVARPAR_ADAPTIONIND      0xC006

#define       SCTPVARPAR_HBINFO           1

/********************* Optional parameters length **************************/
#define       SCTP_MAX_HOSTNAME_SIZE      50
#define       SCTP_MAX_ADDR_TYPES         10
#define       SCTP_MAX_OPTIONAL_PARAMS    10

#define       SCTP_MAX_OPT_PARAMS_TOTAL_SIZE  1500
#define       SCTP_MAX_UNRECOGNISE_CHUNK_LEN  1500
#define       SCTP_MAX_UNKNOWN_CHUNK_LENGTH   1500

#define       SCTPPARAM_LEN_IPV4ADDRESS   8
#define       SCTPPARAM_LEN_IPV6ADDRESS   20
#define       SCTPPARAM_LEN_ECNCAPABLE    4
#define       SCTPPARAM_LEN_COOKIEPRESV   8
#define       SCTPPARAM_LEN_ADAPTIONIND   8

/************************ SCTP Error codes Offset **********************/
#define       SCTPERROROFFSET_ECODE       0
#define       SCTPERROROFFSET_LENGTH      2
#define       SCTPERROROFFSET_BODY        4

/********************* SCTP Error Cause Ids ***************************/
/*#define    SCTPERR_NOERROR         0*/
#define       SCTPERR_INVALIDSTREAM         1
#define       SCTPERR_MISSINGPARAM          2
#define       SCTPERR_STALECOOKIE           3
#define       SCTPERR_NORESOURCE            4
#define       SCTPERR_UNRESOLVEADDR         5
#define       SCTPERR_UNRECOCHUNK           6
#define       SCTPERR_INVMANDPARAM          7
#define       SCTPERR_UNRECOPARAMS          8
#define       SCTPERR_NOUSERDATA            9
#define       SCTPERR_COOKIERECVSHUTDOWN    10
#define       SCTPERR_RESTARTNEWADDRS       11
#define       SCTPERR_USER_INITIATED_ABORT  12
#define       SCTPERR_PROTOCOL_VIOLATION    13

#define       SCTPERR_LASTIPADDRESS         0x0100
#define       SCTPERR_OPREFRESOURCESHORT    0x0101
#define       SCTPERR_SRCIPADDRESS          0x0102
#define       SCTPERR_ILLEGALASCONFACK      0x0103
#define       SCTPERR_REFUSEDNOAUTH         0x0104
/****************** SCTP Error Cause Len *********************/
#define       SCTPERRORLEN_INVALIDSTREAM        8
#define       SCTPERRORLEN_STALECOOKIE          8
#define       SCTPERRORLEN_NORESOURCE           4
#define       SCTPERRORLEN_INVMANDPARAM         4
#define       SCTPERRORLEN_NOUSERDATA           8
#define       SCTPERRORLEN_COOKIERECVSHUTDOWN   4

#define       SCTP_MAX_ERROR_CODES 11

/********************** Error Cause fields length **************************/
/*These have been used in parsing of error chunk type    2*/
#define       SCTPERRORLEN_NUM_MISSING_PARAM   4
#define       SCTPERRORLEN_MISSING_PARAM_TYPE  2

/********************* Socket Modes ******************************/
#define       SCTPSOCKMODE_NONE            0
#define       SCTPSOCKMODE_SERVER          1
#define       SCTPSOCKMODE_CLIENT          2

/*********************** Numbers ********************/
#define       SCTP_MAX_PERCENTAGE_VALUE  100
#define       SCTP_KEYSIZE               16

#define       SCTP_ROLLOVERGUARD         32767

#define       SCTPDYNPORT_MIN            32767
#define       SCTPDYNPORT_MAX            65535

/******************** CHUNK parameters related constants ********************/
#define       SCTP_HBINFO_TYPE_TIME          0
#define       SCTP_HBINFO_LEN_TIME           12  /* sec and usec as U32bit */

#define       SCTP_HBINFO_TYPE_ADDRV4        1
#define       SCTP_HBINFO_LEN_ADDRV4         8

#define       SCTP_HBINFO_TYPE_ADDRV6        2
#define       SCTP_HBINFO_LEN_ADDRV6         20

#define       SCTP_HBINFO_TYPE_NONCE         3
#define       SCTP_HBINFO_LEN_NONCE          12

#define       SCTP_HBINFO_TYPE_PAD_DATA      3

#ifdef SCTP_INET6_SUPPORT

#define       SCTP_MIN_HBINFO_LEN             52

#else

#define       SCTP_MIN_HBINFO_LEN            40

#endif

#define       SCTPOFFSET_SACK_CUMTSN         4
#define       SCTPOFFSET_SACK_ARWND          8
#define       SCTPOFFSET_SACK_NUMFRAGMENTS   12
#define       SCTPOFFSET_SACK_DUPTSNS        14

/*****       INIT    Chunk parameters offset     */
#define       SCTPOFFSET_INIT_INITTAG    4
#define       SCTPOFFSET_INIT_ARWND      8
#define       SCTPOFFSET_INIT_NUMOS      12
#define       SCTPOFFSET_INIT_NUMIS      14
#define       SCTPOFFSET_INIT_INITTSN    16



/****  Retransmission flag types ***/
#define       SCTP_RETRANSFLAG_NONE           0
#define       SCTP_RETRANSFLAG_MARK           1
#define       SCTP_RETRANSFLAG_RETRANSMITTED  2

/****  Acknowledgement flag */
#define       SCTP_ACKFLAG_UNACKED          0
#define       SCTP_ACKFLAG_ACKBYGAPBLOCK 1


#define       SCTPCHUNKOFFSET_TSN          4
#define       SCTPCHUNKOFFSET_STREAM       8
#define       SCTPCHUNKOFFSET_SSN          10
#define       SCTPCHUNKOFFSET_PPI          12
#define       SCTPCHUNKOFFSET_PAYLOAD      16

#define       SCTPFLAGS_E              0x01
#define       SCTPFLAGS_B              0x02
#define       SCTPFLAGS_U              0x04

/********************** Assoc States **********************************/

#define       SCTPSTATE_CLOSED          0
#define       SCTPSTATE_COOKIEWAIT       1
#define       SCTPSTATE_COOKIESENT       2
#define       SCTPSTATE_SHUTDOWNPEND     3
#define       SCTPSTATE_SHUTDOWNRCVD     4
#define       SCTPSTATE_SHUTDOWNSENT     5
#define       SCTPSTATE_SHUTDOWNACKSENT  6
#define       SCTPSTATE_ESTABLISHED      7

#define       SCTP_STATE_NUM              8


/********************** Assoc events Events ******************************/
#define       SCTPEVENT_CMDABORT         0
#define       SCTPEVENT_CMDTERM          1
#define       SCTPEVENT_MSGINITACK       2
#define       SCTPEVENT_MSGCOOKIEACK     3
#define       SCTPEVENT_MSGABORT         4
#define       SCTPEVENT_MSGSHUT          5
#define       SCTPEVENT_MSGSHUTACK       6
#define       SCTPEVENT_TIMER            7
#define       SCTPEVENT_TIMERSHUTDN      8
#define       SCTPEVENT_MSGSACK          9
#define       SCTPEVENT_CMDUSERDATA      10
#define       SCTPEVENT_TIMERDATA        11
#define       SCTPEVENT_TIMERHB          12
#define       SCTPEVENT_TIMERINACTIVE    13
#define       SCTPEVENT_MSGDATA          14
#define       SCTPEVENT_MSGINIT          15
#define       SCTPEVENT_MSGHBACK         16
#define       SCTPEVENT_MSGCOOKIE        17
#define       SCTPEVENT_EVTNEWPMTU       18
#define       SCTPEVENT_TIMERPMTU        19
#define       SCTPEVENT_TIMERDELAYACK    20
#define       SCTPEVENT_TIMERLIFETIME    21
#define       SCTPEVENT_CMDCONNECT       22
#define       SCTPEVENT_MSGSTALECOOKIE   23
#define       SCTPEVENT_MSGECNE          24
#define       SCTPEVENT_MSGCWR           25
#define       SCTPEVENT_TIMERECN         26
#define       SCTPEVENT_MSGSHUTCOMPL     27
#define       SCTPEVENT_TIMERBUNDLE      28
#define       SCTPEVENT_TIMERLINGER      29 /* Same as guard timer */
#define       SCTPEVENT_TIMERAUTOCLOSE   30 /* Autoclose timer: only for UDP */

#define       SCTPEVENT_TIMERINIT        SCTPEVENT_TIMERSHUTDN
#ifdef SCTP_DYNAMIC_IP_SUPPORT

#define       SCTPEVENT_CMDASCONF        31
#define       SCTPEVENT_TIMERASCONF      32

#define       SCTPEVENT_MSGASCONF        33
#define       SCTPEVENT_MSGASCONFACK     34

/* SPR 6854 */
#define       SCTPEVENT_TIMER_LOCALHBEAT     35
/* SPR 6854 */

#define       SCTP_EVENT_NUM             36

#else

/* SPR 6854 */
#define       SCTPEVENT_TIMER_LOCALHBEAT     31
/* SPR 6854 */

#define       SCTP_EVENT_NUM             32

#endif

/* This event is used for mac key update */
#define       SCTPEVENT_TIMERMAC             100
#define       SCTPEVENT_TIMERPMU_DB_UPDATE   101

/* These events are used for handling the select system call */
#define       SCTPEVENT_TIMER_SELECT         200

#define       SCTP_MAX_SELECT_TIMEOUT        0xFFFFFFFF

/* Event defined for dequeuing the indication queue */
#define       SCTPEVENT_TIMER_DEQUE          300
#define       SCTPE_DEQUE_TIMEOUT            5000

#ifdef SCTP_CMN_RECV_POLL_THREAD
#define       SCTPEVENT_TIMER_DUMMY          400
#endif
/************************* SCTP FAST RETRANSMIT ************************/
/* rfc 4960 upgrade  fix start */

#define       SCTP_FAST_RETRANS_THRSHOLD          3 /* Threshold of miss indications before Fast retransmit */

/* rfc 4960 upgrade fix end */

/*********************** Socket Data Transmit States *************************/
#define       SCTP_TX_STATE_NULL          0
#define       SCTP_TX_STATE_AWTACK        1

#define       SCTP_TX_STATE_NUM           2

/******************** Socket Data Transmit Events ****************************/
#define       SCTP_TX_EVENT_SACK          0
#define       SCTP_TX_EVENT_USERDATA      1
#define       SCTP_TX_EVENT_TIMERDATA     2

#define       SCTP_TX_EVENT_NUM           3

/******************* Socket Data Receive States ******************************/
#define       SCTP_RX_STATE_NULL          0
#define       SCTP_RX_STATE_SACK_PENDING  1

#define       SCTP_RX_STATE_NUM           2

/****************** Socket Data Receive Events *****************************/
#define       SCTP_RX_EVENT_DATA          0
#define       SCTP_RX_EVENT_DELAYTIMEOUT  1
#define       SCTP_RX_EVENT_USERDATA      2

#define       SCTP_RX_EVENT_NUM           3


/***********************  For Socket Interface  *********************/
#ifdef SCTP_UI_SOCK_API
#define    SCTP_MAX_BHTABLE_SIZE      65535
#define    SCTP_MAX_LHTABLE_SIZE      65535
#else
#define    SCTP_MAX_BHTABLE_SIZE      32
#define    SCTP_MAX_LHTABLE_SIZE      32
#endif

#define    SCTP_MAX_VERIFICATION_NODE 32

#define    SCTP_BIND_HASH             0
#define    SCTP_LISTEN_HASH           1

#define    SCTP_MAX_LISTEN_BACKLOG    10

#define    SCTP_MAX_NTFY_PER_ASSOC    100

/* Max simultaneous select */
#define    SCTP_MAX_SIMUL_SELECT      10


/* Socket specific Flags */
#define    SCTP_IS_BOUND        0x00000001   /* Ep is bound */
#define    SCTP_IN_USE          0x00000002   /* Ep is used */
#define    SCTP_LISTENER        0x00000004   /* Listen is called on ep */
#define    SCTP_AUTOCLOSE       0x00000008   /* Autoclose option is true */
#define    SCTP_BLOCKING        0x00000010   /* Ep is Blocking */
#define    SCTP_NO_DELAY        0x00000020   /* No unnecessary delay in 
sending a packet */

#define    SCTP_CAN_READ        0x00000040   /* Data available for read or */
/* Conn there to be read */
#define    SCTP_CAN_WRITE       0x00000080   /* Ready for writing */
#define    SCTP_RD_BLOCKED      0x00000100   /* Read is blocked */
#define    SCTP_WR_BLOCKED      0x00000200   /* Write is blocked */
#define    SCTP_SK_ERROR        0x00000400   /* Write is blocked */

#define    SCTP_REUSEADDR       0x00000800   /* reuse of addr */
#define    SCTP_HASHED          0x00001000   /* socket is in hash table */

#define    SCTP_CLOSE_CALLED    0x00002000   /* whether close is called on ep */
/* or not   */

/* Socket Notification Enable Flags */
#define    SCTP_RECVDATAIOEVNT         0x00000100
#define    SCTP_RECVASSOCEVNT          0x00000200
#define    SCTP_RECVPADDREVNT          0x00000400
#define    SCTP_RECVPEERERR            0x00000800
#define    SCTP_RECVSENDFAILEVNT       0x00001000
#define    SCTP_RECVSHUTDOWNEVNT       0x00002000
#define    SCTP_RECVADAPTIONEVNT       0x00004000
#define    SCTP_RECVPARTDELIVERYEVNT   0x00008000

/* Macro to check the Socket Flags */
#define    SCTP_SOCK_IS_BOUND(a)        ((a&SCTP_IS_BOUND) ? 1 : 0)
#define    SCTP_SOCK_IS_IN_USE(a)       ((a&SCTP_IN_USE) ? 1 : 0)
#define    SCTP_SOCK_IS_LISTENER(a)     ((a&SCTP_LISTENER) ? 1 : 0)
#define    SCTP_SOCK_IS_AUTOCLOSE(a)    ((a&SCTP_AUTOCLOSE) ? 1 : 0)
#define    SCTP_SOCK_IS_BLOCKING(a)     ((a&SCTP_BLOCKING) ? 1 : 0)
#define    SCTP_SOCK_IS_NODELAY(a)      ((a&SCTP_NO_DELAY) ? 1 : 0)
#define    SCTP_SOCK_IS_READABLE(a)     ((a&SCTP_CAN_READ) ? 1: 0)
#define    SCTP_SOCK_IS_WRITEABLE(a)    ((a&SCTP_CAN_WRITE) ? 1: 0)
#define    SCTP_SOCK_IS_RD_BLOCKED(a)   ((a&SCTP_RD_BLOCKED) ? 1: 0)
#define    SCTP_SOCK_IS_WR_BLOCKED(a)   ((a&SCTP_WR_BLOCKED) ? 1: 0)
#define    SCTP_SOCK_IS_SK_ERROR(a)     ((a&SCTP_SK_ERROR) ? 1: 0)
#define    SCTP_SOCK_IS_REUSEADDR(a)    ((a&SCTP_REUSEADDR) ? 1: 0)
#define    SCTP_SOCK_IS_HASHED(a)       ((a&SCTP_HASHED) ? 1: 0)
#define    SCTP_SOCK_IS_CLOSE_CALLED(a) ((a&SCTP_CLOSE_CALLED) ? 1: 0)

/* Macro to check the Socket Notification Flags */

#define    SCTP_SOCK_IS_RECVDATAIO_ON(a)  ((a&SCTP_RECVDATAIOEVNT) ? 1 : 0)
#define    SCTP_SOCK_IS_RECVASSOC_ON(a)   ((a&SCTP_RECVASSOCEVNT) ? 1 : 0)
#define    SCTP_SOCK_IS_RECVPADDR_ON(a)   ((a&SCTP_RECVPADDREVNT) ? 1 : 0)
#define    SCTP_SOCK_IS_RECVPEERERR_ON(a) ((a&SCTP_RECVPEERERR) ? 1 : 0)
#define    SCTP_SOCK_IS_RECVSENDFAIL(a)   ((a&SCTP_RECVSENDFAILEVNT) ? 1 : 0)
#define    SCTP_SOCK_IS_RECVSHUTDOWN(a)   ((a&SCTP_RECVSHUTDOWNEVNT) ? 1 : 0)
#define    SCTP_SOCK_IS_RECVADAPTION(a)   ((a&SCTP_RECVADAPTIONEVNT) ? 1 : 0)
#define    SCTP_SOCK_IS_RECVPARTDELIVERY(a) ((a&SCTP_RECVPARTDELIVERYEVNT) ? 1 : 0)

/* Macro to set the Socket Flags */

#define    SCTP_SOCK_SET_BOUND(a, b)    \
    (a = (b? a|SCTP_IS_BOUND : a&(~SCTP_IS_BOUND)))

#define    SCTP_SOCK_SET_IN_USE(a,b)    \
    (a = (b? a|SCTP_IN_USE : a&(~SCTP_IN_USE)))

#define    SCTP_SOCK_SET_LISTENER(a,b)  \
    (a = (b? a|SCTP_LISTENER: a&(~SCTP_LISTENER)))

#define    SCTP_SOCK_SET_AUTOCLOSE(a,b) \
    (a = (b ? a|SCTP_AUTOCLOSE : a&(~SCTP_AUTOCLOSE)))

#define    SCTP_SOCK_SET_BLOCKING(a,b)  \
    (a = (b ? a|SCTP_BLOCKING : a&(~SCTP_BLOCKING)))

#define    SCTP_SOCK_SET_NODELAY(a,b)   \
    (a = (b ? a|SCTP_NO_DELAY : a&(~SCTP_NO_DELAY)))

#define    SCTP_SOCK_SET_READABLE(a,b)  \
    (a = (b ? a|SCTP_CAN_READ : a&(~SCTP_CAN_READ)))

#define    SCTP_SOCK_SET_WRITEABLE(a,b)    \
    (a = (b ? a|SCTP_CAN_WRITE : a&(~SCTP_CAN_WRITE)))

#define    SCTP_SOCK_SET_RD_BLOCKED(a,b)   \
    (a = (b ? a|SCTP_RD_BLOCKED: a&(~SCTP_RD_BLOCKED)))

#define    SCTP_SOCK_SET_WR_BLOCKED(a,b)   \
    (a = (b ? a|SCTP_WR_BLOCKED: a&(~SCTP_WR_BLOCKED)))

#define    SCTP_SOCK_SET_SK_ERROR(a,b)     \
    (a = (b ? a|SCTP_SK_ERROR: a&(~SCTP_SK_ERROR)))

#define    SCTP_SOCK_SET_REUSEADDR(a,b)     \
    (a = (b ? a|SCTP_REUSEADDR: a&(~SCTP_REUSEADDR)))

#define    SCTP_SOCK_SET_HASHED(a,b)     \
    (a = (b ? a|SCTP_HASHED: a&(~SCTP_HASHED)))

#define    SCTP_SOCK_SET_CLOSE_CALLED(a,b)     \
    (a = (b ? a|SCTP_CLOSE_CALLED: a&(~SCTP_CLOSE_CALLED)))


/*
 * Macro to set the Socket Notification Flags
 */

#define    SCTP_SOCK_SET_RECVDATAIO(a,b)   \
    (a = (b ? a|SCTP_RECVDATAIOEVNT : a&(~SCTP_RECVDATAIOEVNT)))

#define    SCTP_SOCK_SET_RECVASSOC(a,b)    \
    (a = (b ? a|SCTP_RECVASSOCEVNT : a&(~SCTP_RECVASSOCEVNT)))

#define    SCTP_SOCK_SET_RECVPADDR(a,b)    \
    (a = (b ? a|SCTP_RECVPADDREVNT : a&(~SCTP_RECVPADDREVNT)))

#define    SCTP_SOCK_SET_RECVPEERERR(a,b)  \
    (a = (b ? a|SCTP_RECVPEERERR : a&(~SCTP_RECVPEERERR)))

#define    SCTP_SOCK_SET_RECVSENDFAIL(a,b) \
    (a = (b ? a|SCTP_RECVSENDFAILEVNT : a&(~SCTP_RECVSENDFAILEVNT)))

#define    SCTP_SOCK_SET_RECVSHUTDOWN(a,b) \
    (a = (b ? a|SCTP_RECVSHUTDOWNEVNT : a&(~SCTP_RECVSHUTDOWNEVNT)))

#define    SCTP_SOCK_SET_RECVADAPTION(a,b) \
    (a = (b ? a|SCTP_RECVADAPTIONEVNT : a&(~SCTP_RECVADAPTIONEVNT)))

#define    SCTP_SOCK_SET_RECVPARTDELIVERY(a,b) \
    (a = (b ? a|SCTP_RECVPARTDELIVERYEVNT : a&(~SCTP_RECVPARTDELIVERYEVNT)))


/*
 * To set the endpoint as selected. This is a counter and not a boolean
 * value. And the Endpoint is selected if counter > 0.
 */
#define    SCTP_SOCK_IS_SELECTED(ep)     ((ep)->selected > 0)
#define    SCTP_SOCK_SET_SELECTED(ep, b) \
    ((b)? ((ep)->selected)++ : \
     ( !((ep)->selected) ? 0: ((ep)->selected)--))

/* The Possible Select Events */
#define    SCTP_SELECT_NEW          0 /* A new select call. */
#define    SCTP_SELECT_TIMEOUT      1 /* A timeout of the select call. */
#define    SCTP_SELECT_RWE_EVENT    2 /* A Read/Write/Error event. */



/*For supporting different flags for message based SU and SM*/
#if defined (SCTP_UI_SU_MESSAGEBASED) || defined (SCTP_UI_SM_MESSAGEBASED)

#ifndef SCTP_UI_MESSAGEBASED    /*avoid redefinition*/
#define SCTP_UI_MESSAGEBASED
#endif

#else

#undef  SCTP_UI_MESSAGEBASED

#endif


#define SCTP_MAX_PARAMS_PER_ASCONF          5
#define SCTP_MAX_ASCONF_MSG_SIZE  (SCTP_MAX_PARAMS_PER_ASCONF + 1)* 28 + 28
#endif /* __FILE_sctp_global_SEEN__ */


