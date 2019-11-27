/******************************************************************************
*
*   FILE NAME:
*       rrc_msg_mgmt.c
*
*   DESCRIPTION:
*       This source file contains RRC messages management functions
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_msg_mgmt.h"
#include "rrc_common_utils.h"

/******************************************************************************
*   The format of the RRC API Header structure is following:
*           (In case of U16, HI is transmitted first)
*
*   U8  - version
*   U8  - from (HI)
*   U8  - from (LOW)
*   U8  - to (HI)
*   U8  - to (LOW)
*   U8  - api id (HI)
*   U8  - api id (LOW)
*   U8  - api size (HI)
*   U8  - api size (LOW)
*   10-16 bytes - spare
******************************************************************************/

/******************************************************************************
*   FUNCTION NAME: rrc_get_byte_from_header
*   INPUT        : U8 *p_header
*   OUTPUT       : None
*   RETURNS      : Byte
*   DESCRIPTION  : Function retreives byte from RRC header
******************************************************************************/
static U8
rrc_get_byte_from_header
(
    U8 *p_header /* RRC header */
)
{
    RRC_ASSERT(p_header != PNULL);

    return *p_header;
}

/******************************************************************************
*   FUNCTION NAME: rrc_get_word_from_header
*   INPUT        : U8 *p_header
*   OUTPUT       : None
*   RETURNS      : Word
*   DESCRIPTION  : Function retreives word from RRC header
******************************************************************************/
static U16
rrc_get_word_from_header
(
    U8 *p_header /* RRC header */
)
{
    U16 value = RRC_NULL;

    RRC_ASSERT(p_header != PNULL);

    value = (U16)(p_header[0] << 8 | p_header[1]);

    return value;
}

/******************************************************************************
*   FUNCTION NAME: rrc_get_version
*   INPUT        : void *p_header
*   OUTPUT       : None
*   RETURNS      : Header version
*   DESCRIPTION  : Function retreives version from RRC header
******************************************************************************/
U8
rrc_get_version
(
    void *p_header /* RRC header */
)
{
    RRC_ASSERT(p_header != PNULL);

    return(rrc_get_byte_from_header(&(((U8 *)p_header)[0])));
}

/******************************************************************************
*   FUNCTION NAME: rrc_get_src_module_id
*   INPUT        : void *p_header
*   OUTPUT       : None
*   RETURNS      : Source module Id
*   DESCRIPTION  : Function retreives source module Id from RRC header
******************************************************************************/
rrc_module_id_t
rrc_get_src_module_id
(
    void *p_header /* RRC header */
)
{
    RRC_ASSERT(p_header != PNULL);

    return((rrc_module_id_t)(rrc_get_word_from_header(&(((U8 *)p_header)[1]))));
}


/******************************************************************************
*   FUNCTION NAME: rrc_get_api_id
*   INPUT        : U8 *p_header
*   OUTPUT       : None
*   RETURNS      : API Id
*   DESCRIPTION  : Function retreives API Id from RRC header
******************************************************************************/
U16
rrc_get_api_id
(
    U8 *p_header /* RRC header */
)
{
    return(rrc_get_word_from_header(&(((U8 *)p_header)[5])));
}

/******************************************************************************
*   FUNCTION NAME: rrc_get_api_buf_size
*   INPUT        : U8 *p_header
*   OUTPUT       : None
*   RETURNS      : API buffer size
*   DESCRIPTION  : Function retreives API buffer size from RRC header
******************************************************************************/
U16
rrc_get_api_buf_size
(
    U8 *p_header /* RRC header */
)
{
    return(rrc_get_word_from_header(&(((U8 *)p_header)[7])));
}

/******************************************************************************
*   FUNCTION NAME: rrc_set_api_buf_size
*   INPUT        : U8 *p_header
*                  U16     api_buf_size
*   OUTPUT       : None
*   RETURNS      : API buffer size
*   DESCRIPTION  : Function set API buffer size in RRC header
******************************************************************************/
void rrc_set_api_buf_size
(
    U8      *p_header,      /* RRC header */
    U32     api_buf_size    /* API message size */
)
{
    if (api_buf_size <= 65400)	//reserved for potential header length
    {
        /* api size (HI) */
        p_header[7] = (U8)((api_buf_size & 0xFF00) >> 8);

        /* api size (LOW) */
        p_header[8] = (U8)(api_buf_size & 0x00FF);
    }
    else
    {
        p_header[9] = (U8)((api_buf_size & 0xFF000000) >> 24);
        p_header[10] = (U8)((api_buf_size & 0x00FF0000) >> 16);
        p_header[11] = (U8)((api_buf_size & 0x0000FF00) >> 8);
        p_header[12] = (U8)(api_buf_size & 0x000000FF);
    }
}

/******************************************************************************
*   FUNCTION NAME: rrc_construct_api_header
*   INPUT        : U8 *p_header
*                  U8                  version_id
*                  rrc_module_id_t     src_module_id
*                  rrc_module_id_t     dst_module_id
*                  U16                 api_id
*                  U16                 api_buf_size
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : Function constructs RRC header from given parameters
******************************************************************************/
void
rrc_construct_api_header
(
    U8                  *p_header,      /* RRC header */
    U8                  version_id,     /* API version Id */
    rrc_module_id_t     src_module_id,  /* Source module Id */
    rrc_module_id_t     dst_module_id,  /* Destination module Id */
    U16                 api_id,         /* API Id */
    U32                 api_buf_size    /* API buffer size */
)
{
    /* version */
    p_header[0] = version_id;

    /* from (HI) */
    p_header[1] = (U8)((src_module_id & 0xFF00) >> 8);

    /* from (LOW) */
    p_header[2] = (U8)(src_module_id & 0x00FF);

    /* to (HI) */
    p_header[3] = (U8)((dst_module_id & 0xFF00) >> 8);

    /* to (LOW) */
    p_header[4] = (U8)(dst_module_id & 0x00FF);

    /* api id (HI) */
    p_header[5] = (U8)((api_id & 0xFF00) >> 8);

    /* api id (LOW) */
    p_header[6] = (U8)(api_id & 0x00FF);

#ifdef LINUX_PC_TEST
    if (api_buf_size <= 65400)	//reserved for potential header length
    {
#endif
        /* api size (HI) */
        p_header[7] = (U8)((api_buf_size & 0xFF00) >> 8);

        /* api size (LOW) */
        p_header[8] = (U8)(api_buf_size & 0x00FF);

        p_header[9]    = 0x00;
        p_header[10]    = 0x00;
        p_header[11]    = 0x00;
        p_header[12]    = 0x00;
#ifdef LINUX_PC_TEST
    }
    else
    {
        p_header[7]    = 0x00;
        p_header[8]    = 0x00;
        p_header[9] = (U8)((api_buf_size & 0xFF000000) >> 24);
        p_header[10] = (U8)((api_buf_size & 0x00FF0000) >> 16);
        p_header[11] = (U8)((api_buf_size & 0x0000FF00) >> 8);
        p_header[12] = (U8)(api_buf_size & 0x000000FF);
    }
#endif
    /* spare bytes */;
    p_header[13]    = 0x00;
    p_header[14]    = 0x00;
    p_header[15]    = 0x00;
}

/******************************************************************************
*   FUNCTION NAME: rrc_get_transaction_id
*   INPUT        : void *p_api
*   OUTPUT       : None
*   RETURNS      : API buffer size
*   DESCRIPTION  : Function retreives transaction_id from external API message
******************************************************************************/
U16
rrc_get_transaction_id
(
    void *p_api /* external API message */
)
{
    return(rrc_get_word_from_header((U8 *)p_api + RRC_API_HEADER_SIZE));
}

/******************************************************************************
*   FUNCTION NAME: rrc_construct_interface_api_header
*   INPUT        : U8                  *p_header
*                  U16                 transaction_id
*                  rrc_module_id_t     src_module_id
*                  rrc_module_id_t     dst_module_id
*                  U16                 api_id
*                  U16                 api_buf_size
*                  U8                  cell_index
*   OUTPUT       : None
*   RETURNS      : None
*   DESCRIPTION  : Function constructs interface api header from given parameters
******************************************************************************/
void
rrc_construct_interface_api_header
(
    U8                  *p_header,      /* RRC interface header */
    U16                 transaction_id, /* Interface transaction identifier */
    rrc_module_id_t     src_module_id,  /* Source module identifier */
    rrc_module_id_t     dst_module_id,  /* Destination module identifier */
    U16                 api_id,         /* API identifier */
    U16                 api_buf_size,   /* API buffer size */
    U8                  cell_index      /* cell index */
)
{
    /* transaction identifier (HI) */
    p_header[0] = (U8)((transaction_id & 0xFF00) >> 8);

    /* transaction identifier (LOW) */
    p_header[1] = (U8)(transaction_id & 0x00FF);

    /* from (HI) */
    p_header[2] = (U8)((src_module_id & 0xFF00) >> 8);

    /* from (LOW) */
    p_header[3] = (U8)(src_module_id & 0x00FF);

    /* to (HI) */
    p_header[4] = (U8)((dst_module_id & 0xFF00) >> 8);

    /* to (LOW) */
    p_header[5] = (U8)(dst_module_id & 0x00FF);

    /* api id (HI) */
    p_header[6] = (U8)((api_id & 0xFF00) >> 8);

    /* api id (LOW) */
    p_header[7] = (U8)(api_id & 0x00FF);

    /*size includes length of header*/
    api_buf_size = (U16)(api_buf_size + RRC_INTERFACE_API_HEADER_SIZE);

    /* api size (HI) */
    p_header[8] = (U8)((api_buf_size & 0xFF00) >> 8);

    /* api size (LOW) */
    p_header[9] = (U8)(api_buf_size & 0x00FF);

    /* cell index */
    p_header[10] = (U8)(cell_index);
    RRC_TRACE(RRC_DETAILED, "Value of cell index = %d", (U8)p_header[10]);

    /* spare bytes */
    p_header[11] = 0x00;
    p_header[12] = 0x00;
    p_header[13] = 0x00;
    p_header[14] = 0x00;
    p_header[15] = 0x00;

}

#ifdef LTE_EMBMS_SUPPORTED 
/******************************************************************************
*   FUNCTION NAME: rrc_set_api_dest_id
*   INPUT        : void *p_api
*   OUTPUT       : None
*   RETURNS      : API dest module id. 
*   DESCRIPTION  : Function sets dest.module id in message
******************************************************************************/
void rrc_set_api_dest_id
(
    U8 *msg, 
    rrc_module_id_t dst_module_id)
{
    /* to (HI) */
    msg[3] = (U8)((dst_module_id & 0xFF00) >> 8);

    /* to (LOW) */
    msg[4] = (U8)(dst_module_id & 0x00FF);
}
#endif
