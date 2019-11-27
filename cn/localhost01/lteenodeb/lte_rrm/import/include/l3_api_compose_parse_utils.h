/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrm_cp_common.h,v 1.1.2.1.8.1 2010/10/15 13:42:33 gur20491 Exp $
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrm_cp_common.h,v $
 * Revision 1.1.2.1.8.1  2010/10/15 13:42:33  gur20491
 * Changes for RRC 2.0.1 Compliance
 *
 * Revision 1.1.2.1  2010/08/02 12:57:03  gur20491
 * FrameworkSetupWith_RRC1.0_MAC1.2.4_NEW_OAM_RRM
 *
 * 2012/08/09 15:36:00  gur27278   Removed cross-compilation warnings
 *
 *
 *
 ****************************************************************************/
/*! \file l3_api_compose_parse_utils.h
 *  \brief This file contains the layer 3 api composer parser utils 
 *  \date January 02, 2012
 *  \author Komal Thareja
 */
/*!
 *  \copyright {Copyright (c) 2012, Aricent Inc. All Rights Reserved}
 */

#ifndef _L3_API_COMPOSE_PARSE_H_
#define _L3_API_COMPOSE_PARSE_H_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
/*! \headerfile stacklayer.h <>
 */
#include <stacklayer.h>
/*! \headerfile rrm_defines.h <>
 */
#include "rrm_defines.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/


/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/*! \def   p_rrc_intf_facility_name
 *  \brief  Pointer variable storing the SMIF name for logging
 */
extern S8 *p_rrc_intf_facility_name;

/*rrm_return_et*/
rrm_return_et
rrm_cp_pack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_pack_S16
(
    void    *p_dest,
    void    *p_src,
    S8    *varname
);

rrm_return_et
rrm_cp_pack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_U8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_U16
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_U64
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_U32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_S8
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

rrm_return_et
rrm_cp_unpack_S16
(
    void    *p_dest,
    void    *p_src,
    S8    *varname
);

rrm_return_et
rrm_cp_unpack_S32
(
    void    *p_dest,
    void    *p_src,
    char    *varname
);

void*
rrm_msg_mem_get
(
        size_t size /* Size of buffer which will be allocated */
);

void
rrm_msg_mem_free
(
     void *p_buffer /* Pointer to buffer which will be freed */
);

void rrm_construct_interface_api_header
(
      U8                  *p_header,      /* RRC interface header */
      U16                 transaction_id, /* Interface transaction identifier */
      U16                 src_module_id,  /* Source module identifier */
      U16                 dst_module_id,  /* Destination module identifier */
      U16                 api_id,         /* API identifier */
      U16                 api_buf_size,    /* API buffer size */
      rrm_cell_index_t    cell_index      /* Cell Index */
     
);

#endif /* _RRM_CP_COMMON_H_ */

