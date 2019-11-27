/***********************************************************************
**  FUNCTION:
**	eGTP-U Protocol Message and API headers container
************************************************************************
**
**  FILE NAME:
**	egtpu_apihdr.h
**
**  DESCRIPTION:
**	Contains get-set macros for headers of APIs and
**      GTPU protocol messages
**
**  NOTE:
**      It is required that buffers casted to (U8bit *) be
**      passed into these macros
**
**  DATE	    NAME			REFERENCE	REASON
**  ----	    ----			---------	------
**  Oct 2009    Vipul Aggarwal    			Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef _EGTPU_API_HDR_DECL_
#define _EGTPU_API_HDR_DECL_

/* get-set macros for API header fields */

/* Convention: To denote that the payload does not exist - set
   EGTPU_API_SET_START_PAYLOAD and EGTPU_API_SET_END_PAYLOAD to EGTPU_NO_PAYLOAD i.e. 0 */

#define EGTPU_NO_PAYLOAD         		0
#define EGTPU_API_HDR_LEN        		16
#define EGTPU_MIN_HDR_LEN        		8
#define EGTPU_HDR_LEN_WITH_SEQ_NUM      12
#define EGTPU_HDR_LEN_WITH_PDCP_NUM     16	/** 12 + 4 bytes of PDCP extn hdr **/
/* SPR 18822 Fix Start */
#define EGTPU_HDR_LEN_WITH_UDP_PORT_NUM 16	/** 12 + 4 bytes of UDP extn hdr **/
/* SPR 18822 Fix End */
#define EGTPU_API_VERSION_NUMBER 		1
#define EGTPU_DEF_PRIORITY       		0
#define EGTPU_SPARE_BYTE         		0

/*--------------------------------------------------------------------#
 #  GET Functions for buffer                                          #
 #--------------------------------------------------------------------*/

/* Get the API Version Number */
#define EGTPU_API_GET_VER_ID(p_buff) \
    (*(U8bit *)(p_buff))

/* Get the Priority */
#define EGTPU_API_GET_PRIORITY(p_buff) \
    (*(U8bit *)(p_buff + 1))

/* Get the Source Entity ID */
#define EGTPU_API_GET_SRC_ID(p_buff) \
    EGTPU_GET_U16BIT ((U8bit *)(p_buff + 2))

/* Get the Destination Entity ID */
#define EGTPU_API_GET_DST_ID(p_buff) \
    EGTPU_GET_U16BIT ((U8bit *)(p_buff + 4))

/* Get the API ID */
#define EGTPU_API_GET_API_ID(p_buff) \
    EGTPU_GET_U16BIT ((U8bit *)(p_buff+ 6))

/* Get the Message Length  */
#define EGTPU_API_GET_MSG_LEN(p_buff) \
    EGTPU_GET_U16BIT ((U8bit *)(p_buff + 8))

/* Get the End of the Payload (offset from the start of the API buffer) */
#define EGTPU_API_GET_END_PAYLOAD(p_buff) \
    EGTPU_GET_U16BIT ((U8bit *)(p_buff + 10))

/* Get the Start Payload (offset from the start of the API buffer) */
#define EGTPU_API_GET_START_PAYLOAD(p_buff) \
    EGTPU_GET_U16BIT ((U8bit *)(p_buff + 12))

/* Get the length of Payload (number of octets) */
#define EGTPU_API_GET_PAYLOAD_LEN(p_buff) \
    (EGTPU_API_GET_END_PAYLOAD (p_buff) - \
        EGTPU_API_GET_START_PAYLOAD (p_buff) + 1)

/* Get the Start of Payload Pointer */
#define EGTPU_API_GET_PAYLOAD_STRT_PTR(p_buff) \
    ((U8bit *)p_buff + EGTPU_API_GET_START_PAYLOAD (p_buff))

/* Get the End of Payload Pointer */
#define EGTPU_API_GET_PAYLOAD_END_PTR(p_buff) \
    ((U8bit *)p_buff + EGTPU_API_GET_END_PAYLOAD (p_buff))

/* Get the End of Parameter list (offset from start of API buffer) */
#define EGTPU_API_GET_END_PARAM_LIST(p_buff) \
    EGTPU_GET_U16BIT ((U8bit *)(p_buff+14))

/* Get the End of Parameter list Pointer */
#define EGTPU_API_GET_END_PARAM_LIST_PTR(p_buff) \
    ((U8bit *)p_buff + EGTPU_API_GET_END_PARAM_LIST (p_buff))
    

/**********************************************
    SET Functions for buffer
**********************************************/

/* Set the API Version Number */
#define EGTPU_API_SET_VER_ID(p_buff) \
    *(U8bit *)(p_buff) = EGTPU_API_VERSION_NUMBER

/* Set the Priority */
#define EGTPU_API_SET_PRIORITY(p_buff) \
    *(U8bit *)(p_buff+1) = EGTPU_DEF_PRIORITY

/* Set the Source Entity ID */
#define EGTPU_API_SET_SRC_ID(p_buff, src_id) \
    EGTPU_SET_U16BIT ((U8bit *)(p_buff + 2), src_id)

/* Set the Destination Entity ID */
#define EGTPU_API_SET_DST_ID(p_buff, dst_id) \
    EGTPU_SET_U16BIT ((U8bit *)(p_buff + 4), dst_id)

/* Set the API ID */
#define EGTPU_API_SET_API_ID(p_buff, api_id) \
    EGTPU_SET_U16BIT ((U8bit *)(p_buff + 6), api_id)

/* Set the Message Length */
#define EGTPU_API_SET_MSG_LEN(p_buff, msg_len) \
    EGTPU_SET_U16BIT ((U8bit *)(p_buff + 8), msg_len)

/* Set the End of Payload (offset from start of API buffer) */
#define EGTPU_API_SET_END_PAYLOAD(p_buff, end_pl) \
    EGTPU_SET_U16BIT ((U8bit *)(p_buff + 10), end_pl)

/* Set the Start of Payload (offset from start of API buffer) */
#define EGTPU_API_SET_START_PAYLOAD(p_buff, strt_pl) \
    EGTPU_SET_U16BIT ((U8bit *)(p_buff + 12), strt_pl)

/* Set the End of Parameter list (offset from start of API buffer) */
#define EGTPU_API_SET_END_PARAM_LIST(p_buff, end_param) \
    EGTPU_SET_U16BIT ((U8bit *)(p_buff + 14), end_param)

/* Set the API Header */
#define EGTPU_SET_API_HDR(p_buff, src_id, dst_id, api_id, msg_len,      \
                             pay_end, pay_start, param_end)            \
    do {                                                               \
        EGTPU_API_SET_VER_ID (p_buff);                                  \
        EGTPU_API_SET_PRIORITY (p_buff);                                \
        EGTPU_API_SET_SRC_ID (p_buff, src_id);                          \
        EGTPU_API_SET_DST_ID (p_buff, dst_id);                          \
        EGTPU_API_SET_API_ID (p_buff, api_id);                          \
        EGTPU_API_SET_MSG_LEN (p_buff, msg_len);                        \
        EGTPU_API_SET_END_PAYLOAD (p_buff, pay_end);                    \
        EGTPU_API_SET_START_PAYLOAD (p_buff, pay_start);                \
        EGTPU_API_SET_END_PARAM_LIST (p_buff, param_end);               \
    } while (0)

/**********************************************************************/
/**********************************************************************/

/* get-set macros for API header fields */

#define    GTP_VERSION              1
#define    EGTPU_PROTOCOL_TYPE       1
#define    GTP_MSG_HDR_MIN_LEN      8

/* +- SPR 18822 Fix */
#define		EGTPU_UNSET_EXT_HDR_FLAG		0
#define		EGTPU_SET_SEQ_NUM_FLAG			1
#define		EGTPU_UNSET_SEQ_NUM_FLAG		0
#define		EGTPU_SET_PN_FLAG				1
#define		EGTPU_UNSET_PN_FLAG				0

/**********************************************
    GET Functions for buffer
**********************************************/

/* get the GTPU Version Number */
#define EGTPU_GET_VER_NUM(p_buf)                                        \
    ((*((U8bit *)p_buf) & 0xE0) >> 5)

/* get the Protocol Type */
#define EGTPU_GET_PT_FLAG(p_buf)                                        \
    ((*((U8bit *)p_buf) & 0x10) >> 4)
        
/* get the Extension Header Flag */
#define EGTPU_GET_EXT_FLAG(p_buff)                                      \
    (*((U8bit *)p_buff) & 0x04) 

/* get the Sequnece Number Flag */
#define EGTPU_GET_SEQ_FLAG(p_buff)                                      \
    (*((U8bit *)p_buff) & 0x02) 

/* get the N-PDU Number Flag */
#define EGTPU_GET_PN_FLAG(p_buff)                                       \
    (*((U8bit *)p_buff) & 0x01)

/* get the Message Type */
#define EGTPU_GET_MSG_TYPE(p_buff)                                      \
    *((U8bit *)(p_buff) + 1)

/* get the LENGTH */
#define EGTPU_GET_MSG_LEN(p_buff)                                       \
    EGTPU_GET_U16BIT (((U8bit *)(p_buff) + 2))

/* get the teid */
#define EGTPU_GET_TEID(p_buff)                                          \
    EGTPU_GET_U32BIT (((U8bit *)(p_buff) + 4))

/* get the SEQENCE NUMBER */
#define EGTPU_GET_SEQ_NUM(p_buff)                                       \
    EGTPU_GET_U16BIT (((U8bit *)(p_buff) + 8))

/* get the N-PDU NUMBER */
#define EGTPU_GET_NPDU_NUM(p_buff)                                      \
    *((U8bit *)(p_buff) + 10)

/* get the Next Extension HDR type san */
#define EGTPU_GET_EXT_HDR_PTR(p_buff)                                   \
    (p_buff + 11)                    

/**********************************************
    SET Functions for buffer
**********************************************/

/* set the GTP Header for Signalling Messages */
#define EGTPU_SET_GTP_HDR(buff, ext_flg, seq_flg, pn_flg, msg_typ,      \
                             msg_len, teid_val, seqno, npduno,pdcp_udp_no) \
    do {                                                               \
        *(buff) = 0x30;                                                \
                                                                       \
        if (seq_flg) {                                                 \
            *(buff) |= 0x02;                                           \
            EGTPU_SET_U16BIT (buff + 8, seqno);                         \
        }                                                              \
                                                                       \
        if (pn_flg) {                                                  \
            *(buff) |= 0x01;                                           \
            *(buff + 10) = npduno;                                     \
        }  else {                                                      \
            /*  *(buff + 10) = 0;*/                                    \
		}	                                    						\
        /* Coverity 108613 Fix Start */                                 \
            if (ext_flg) {                                             \
                *(buff) |= 0x04;                                       \
                                                                       \
                /*+- SPR 18822 Fix */                                  \
                /* Either UDP Port or PDCP SN Extension
                 * will be Present */                                  \
                *(buff + 11) = ext_flg;                                \
                *(buff + 12) = 1;                                      \
                EGTPU_SET_U16BIT ((buff + 13), pdcp_udp_no);           \
                *(buff + 15) = 0;                                      \
            } else if (seq_flg || pn_flg) {                            \
                if (*(buff) & 0x03)                                    \
                    *(buff + 11) = 0;                                  \
        }                                                              \
        /* Coverity 108613 Fix End */                                  \
        *(buff + 1) = (msg_typ);                                       \
        EGTPU_SET_U16BIT (buff + 2, msg_len);                          \
        EGTPU_SET_U32BIT (buff + 4, teid_val);                         \
    } while (0)
#ifdef ENDC_ENABLED
/* Set the GTP Header containing RAN container */
#define EGTPU_SET_GTP_HDR_WITH_X2U_RAN_CONTAINER(buff, msg_typ, msg_len,  \
        teid_val)    \
\
do                                                                \
{                                                                 \
    /* Set version=1, PT=1 and E=1 */                             \
    *(buff) = 0x34;                                               \
    \
    /* Set message type */                                        \
    *(buff + 1) = (msg_typ);                                      \
    \
    /* Set message length */                                      \
    EGTPU_SET_U16BIT(buff + 2, msg_len);                          \
    \
    /* Set peer tunnel Id */                                      \
    EGTPU_SET_U32BIT(buff + 4, teid_val);                         \
    \
    /* Set sequence number and N-PDU as 0 */                      \
    EGTPU_SET_U24BIT(buff + 8, 0);                                \
    /* Set next extension header as RAN container */              \
    *(buff + 11) = 0x81;                                          \
    \
} while(0)

/* Set the GTP Header containing NR-U RAN container */
#define EGTPU_SET_GTP_HDR_WITH_NRU_RAN_CONTAINER(buff, msg_typ, msg_len,  \
		teid_val)    \
\
do                                                                \
{                                                                 \
	/* Set version=1, PT=1 and E=1 */                             \
	*(buff) = 0x34;                                               \
	\
	/* Set message type */                                        \
	*(buff + 1) = (msg_typ);                                      \
	\
	/* Set message length */                                      \
	EGTPU_SET_U16BIT(buff + 2, msg_len);                          \
	\
	/* Set peer tunnel Id */                                      \
	EGTPU_SET_U32BIT(buff + 4, teid_val);                         \
	\
	/* Set sequence number and N-PDU as 0 */                      \
	EGTPU_SET_U24BIT(buff + 8, 0);                                \
	\
	/* Set next extension header as NR-U RAN container */         \
	*(buff + 11) = 0x84;                                          \
	\
} while(0)

#endif
#endif
