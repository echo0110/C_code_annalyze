/******************************************************************************
*
*   FILE NAME:
*       rrc_msg_mgmt.h
*
*   DESCRIPTION:
*       This header file contains RRC messages management functions
*
*   DATE            AUTHOR      REFERENCE       REASON
*   02 Apr 2009     VasylN      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/

#ifndef _RRC_MSG_MGMT_H_
#define _RRC_MSG_MGMT_H_

#include "rrc_defines.h"

#define RRC_API_HEADER_SIZE             16

#define RRC_API_INTERFACE_HEADER_CELL_INDEX 10

#define RRC_INTERFACE_API_HEADER_SIZE   16


#define RRC_FULL_INTERFACE_HEADERS_SIZE \
    (RRC_API_HEADER_SIZE + RRC_INTERFACE_API_HEADER_SIZE)

U8
rrc_get_version
(
    void *p_api
);

rrc_module_id_t
rrc_get_src_module_id
(
    void *p_api
);

rrc_module_id_t
rrc_get_dst_module_id
(
    U8 *p_api
);

U16
rrc_get_api_id
(
    U8 *p_api
);

U16
rrc_get_api_buf_size
(
    U8 *p_api
);

void rrc_set_api_buf_size
(
    U8      *p_header,      /* RRC header */
    U32     api_buf_size    /* API message size */
);

void
rrc_construct_api_header
(
    U8                  *p_header,
    U8                  version_id,
    rrc_module_id_t     src_module_id,
    rrc_module_id_t     dst_module_id,
    U16                 api_id,
    U32                 api_buf_size
);


U16
rrc_get_transaction_id
(
    void *p_api
);

void
rrc_construct_interface_api_header
(
    U8                  *p_header,
    U16                 transaction_id,
    rrc_module_id_t     src_module_id,
    rrc_module_id_t     dst_module_id,
    U16                 api_id,
    U16                 api_buf_size,
    U8                  cell_index
);

void rrc_set_api_dest_id
(
    U8 *msg, 
    rrc_module_id_t dst_module_id
);
#endif /* _RRC_MSG_MGMT_H_ */
