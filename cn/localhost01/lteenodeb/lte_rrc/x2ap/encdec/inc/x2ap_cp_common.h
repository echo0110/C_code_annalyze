
/**************************************************************************
*
*  ARICENT -
*
*  Copyright (c) 2010 Aricent.
*
***************************************************************************/
#ifndef _X2AP_CP_COMMON_H_
#define _X2AP_CP_COMMON_H_

x2ap_return_et x2ap_cp_pack_U8(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_pack_U16(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_pack_U64(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_pack_U32(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_pack_S8(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_pack_S16(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_pack_S32(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_unpack_U8(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_unpack_U16(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_unpack_U64(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_unpack_U32(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_unpack_S8(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_unpack_S16(
    void    *p_dest,
    void    *p_src,
    char    *varname);

x2ap_return_et x2ap_cp_unpack_S32(
    void    *p_dest,
    void    *p_src,
    char    *varname);

/* Prototypes for packing */
x2ap_length_t x2ap_il_get_x2ap_oam_provision_req_st_len(
        x2ap_oam_provision_req_t *p_x2ap_oam_provision_req_t);

x2ap_return_et x2ap_il_compose_x2ap_oam_provision_req_t(
        U8  **pp_buffer,
        x2ap_oam_provision_req_t *p_x2ap_oam_provision_req_t);

x2ap_return_et x2ap__il_send_x2ap_oam_provision_req_t(
        x2ap_oam_provision_req_t  *p_x2ap_oam_provision_req_t,  /* Pointer to API specific information. */
        U16                 src_module_id,  /* Source module identifier */
        U16                 transaction_id);  /* Interface transaction identifier */

x2ap_return_et x2ap_il_compose_x2ap_oam_provision_resp_t(
        U8  **pp_buffer,
        x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp_t);

x2ap_length_t x2ap_il_get_x2ap_oam_provision_resp_st_len(
        x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp_t);

x2ap_return_et x2ap__il_send_x2ap_oam_provision_resp_t(
        x2ap_oam_provision_resp_t  *p_x2ap_oam_provision_resp_t, /* Pointer to API specific information. */
        U16                 src_module_id,  /* Source module identifier */
        U16                 transaction_id);  /* Interface transaction identifier */

x2ap_length_t x2ap_il_get_x2ap_oam_x2_link_down_st_len(
        x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down_t);

x2ap_return_et x2ap_il_compose_x2ap_oam_x2_link_down_t(
        U8  **pp_buffer,
        x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down_t);

x2ap_return_et x2ap__il_send_x2ap_oam_x2_link_down_t(
        x2ap_oam_x2_link_down_t  *p_x2ap_oam_x2_link_down_t,           /* Pointer to API specific information. */
        U16                 src_module_id,  /* Source module identifier */
        U16                 transaction_id);  /* Interface transaction identifier */

x2ap_length_t x2ap_il_get_x2ap_oam_x2_link_up_st_len(
        x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up_t);

x2ap_return_et x2ap_il_compose_x2ap_oam_x2_link_up_t(
        U8  **pp_buffer,
        x2ap_oam_x2_link_up_t *p_x2ap_oam_x2_link_up_t );

x2ap_return_et x2ap__il_send_x2ap_oam_x2_link_up_t(
        x2ap_oam_x2_link_up_t  *p_x2ap_oam_x2_link_up_t, /* Pointer to API specific information. */
        U16                 src_module_id,  /* Source module identifier */
        U16                 transaction_id);  /* Interface transaction identifier */

x2ap_length_t x2ap_il_get_x2ap_oam_cleanup_resp_st_len(
        x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp_t);

x2ap_return_et x2ap_il_compose_x2ap_oam_cleanup_resp_t(
        U8  **pp_buffer,
        x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp_t);

x2ap_return_et x2ap__il_send_x2ap_oam_cleanup_resp_t(
        x2ap_oam_cleanup_resp_t  *p_x2ap_oam_cleanup_resp_t,/* Pointer to API specific information. */
        U16                 src_module_id,  /* Source module identifier */
        U16                 transaction_id);  /* Interface transaction identifier */


/* Prototypes for unpacking */
x2ap_return_et x2ap_il_parse_x2ap_oam_provision_req_t(
        x2ap_oam_provision_req_t *p_x2ap_oam_provision_req_t,
        U8  *p_src,
        S32 length_left,
        S32 *p_length_read);

x2ap_return_et x2ap_il_parse_x2ap_oam_provision_resp_t(
        x2ap_oam_provision_resp_t *p_x2ap_oam_provision_resp_t,
        U8  *p_src,
        S32 length_left,
        S32 *p_length_read);

x2ap_return_et x2ap_il_parse_x2ap_oam_x2_link_down_t(
        x2ap_oam_x2_link_down_t *p_x2ap_oam_x2_link_down_t,
        U8  *p_src,
        S32 length_left,
        S32 *p_length_read);

x2ap_return_et x2ap_il_parse_x2ap_oam_cleanup_resp_t(
        x2ap_oam_cleanup_resp_t *p_x2ap_oam_cleanup_resp_t,
        U8  *p_src,
        S32 length_left,
        S32 *p_length_read);


x2ap_length_t x2ap_il_get_x2ap_oam_x2_link_setup_req_st_len(
        x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req_t);

x2ap_return_et x2ap_il_compose_x2ap_oam_x2_link_setup_req_t(
        U8  **pp_buffer,
        x2ap_oam_x2_link_setup_req_t *p_x2ap_oam_x2_link_setup_req_t);

x2ap_return_et x2ap__il_send_x2ap_oam_x2_link_setup_req_t(
        x2ap_oam_x2_link_setup_req_t  *p_x2ap_oam_x2_link_setup_req_t,      /* Pointer to API specific information. */
        U16                 src_module_id,  /* Source module identifier */
        U16                 transaction_id);  /* Interface transaction identifier */

x2ap_length_t x2ap_il_get_x2ap_oam_x2_link_setup_resp_st_len(
        x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp_t);

x2ap_return_et x2ap_il_compose_x2ap_oam_x2_link_setup_resp_t(
        U8  **pp_buffer,
        x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp_t);

x2ap_return_et x2ap__il_send_x2ap_oam_x2_link_setup_resp_t
(
 x2ap_oam_x2_link_setup_resp_t  *p_x2ap_oam_x2_link_setup_resp_t,   /* Pointer to API specific information. */
 U16                 src_module_id,  /* Source module identifier */
 U16                 transaction_id);  /* Interface transaction identifier */

x2ap_return_et x2ap_il_parse_x2ap_oam_x2_link_setup_resp_t(
        x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp_t,
        U8  *p_src,
        S32 length_left,
        S32 *p_length_read);

x2ap_return_et x2ap_il_parse_x2ap_oam_x2_link_setup_resp_t(
        x2ap_oam_x2_link_setup_resp_t *p_x2ap_oam_x2_link_setup_resp_t,
        U8  *p_src,
        S32 length_left,
        S32 *p_length_read);


#endif /* _X2AP_CP_COMMON_H_ */
