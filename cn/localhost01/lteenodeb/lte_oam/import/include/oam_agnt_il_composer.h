/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2017 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_agnt_composer.h $
 *
 *******************************************************************************
 *
 * File Description: OAM AGENT composer header file.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _OAM_AGNT__COMPOSER_H_
#define _OAM_AGNT__COMPOSER_H_


oam_length_t
oam_agnt_get_agent_oam_init_ind_len
(
    agent_oam_init_ind_t *p_agent_oam_init_ind
);

oam_return_et
oam_agnt_compose_agent_oam_init_ind
(
    UInt8  **pp_buffer,
    agent_oam_init_ind_t *p_agent_oam_init_ind
);

oam_length_t
oam_agnt_get_oam_agent_spawn_process_req_len
(
    oam_agent_spawn_process_req_t *p_oam_agent_spawn_process_req
);

oam_return_et
oam_agnt_compose_oam_agent_spawn_process_req
(
    UInt8  **pp_buffer,
    oam_agent_spawn_process_req_t *p_oam_agent_spawn_process_req
);

oam_length_t
oam_agnt_get_agent_oam_spawn_process_resp_len
(
    agent_oam_spawn_process_resp_t *p_agent_oam_spawn_process_resp
);

oam_return_et
oam_agnt_compose_agent_oam_spawn_process_resp
(
    UInt8  **pp_buffer,
    agent_oam_spawn_process_resp_t *p_agent_oam_spawn_process_resp
);

oam_length_t
oam_agnt_get_oam_agent_kill_process_req_len
(
    oam_agent_kill_process_req_t *p_oam_agent_kill_process_req
);

oam_return_et
oam_agnt_compose_oam_agent_kill_process_req
(
    UInt8  **pp_buffer,
    oam_agent_kill_process_req_t *p_oam_agent_kill_process_req
);

oam_length_t
oam_agnt_get_agent_oam_kill_process_resp_len
(
    agent_oam_kill_process_resp_t *p_agent_oam_kill_process_resp
);

oam_return_et
oam_agnt_compose_agent_oam_kill_process_resp
(
    UInt8  **pp_buffer,
    agent_oam_kill_process_resp_t *p_agent_oam_kill_process_resp
);

oam_length_t
oam_agnt_get_agent_oam_get_logs_resp_len
(
    agent_oam_get_logs_resp_t *p_agent_oam_get_logs_resp
);

oam_return_et
oam_agnt_compose_agent_oam_get_logs_resp
(
    UInt8  **pp_buffer,
    agent_oam_get_logs_resp_t *p_agent_oam_get_logs_resp
);

oam_length_t
oam_agnt_get_oam_agent_get_logs_cnf_len
(
    oam_agent_get_logs_cnf_t *p_oam_agent_get_logs_cnf
);

oam_return_et
oam_agnt_compose_oam_agent_get_logs_cnf
(
    UInt8  **pp_buffer,
    oam_agent_get_logs_cnf_t *p_oam_agent_get_logs_cnf
);

oam_length_t
oam_agnt_get_agent_oam_im_ok_ind_len
(
    agent_oam_im_ok_ind_t *p_agent_oam_im_ok_ind
);

oam_return_et
oam_agnt_compose_agent_oam_im_ok_ind
(
    UInt8  **pp_buffer,
    agent_oam_im_ok_ind_t *p_agent_oam_im_ok_ind
);

oam_return_et
oam_agnt_send_agent_oam_init_ind
(
    agent_oam_init_ind_t  *p_agent_oam_init_ind,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  instance_id
);

oam_return_et
oam_agnt_send_oam_agent_spawn_process_req
(
    oam_agent_spawn_process_req_t  *p_oam_agent_spawn_process_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  instance_id
);

oam_return_et
oam_agnt_send_agent_oam_spawn_process_resp
(
    agent_oam_spawn_process_resp_t  *p_agent_oam_spawn_process_resp,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  instance_id
);

oam_return_et
oam_agnt_send_oam_agent_kill_process_req
(
    oam_agent_kill_process_req_t  *p_oam_agent_kill_process_req,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  instance_id
);

oam_return_et
oam_agnt_send_agent_oam_kill_process_resp
(
    agent_oam_kill_process_resp_t  *p_agent_oam_kill_process_resp,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  instance_id
);

oam_return_et
oam_agnt_send_agent_oam_get_logs_resp
(
    agent_oam_get_logs_resp_t  *p_agent_oam_get_logs_resp,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  instance_id
);

oam_return_et
oam_agnt_send_oam_agent_get_logs_cnf
(
    oam_agent_get_logs_cnf_t  *p_oam_agent_get_logs_cnf,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  instance_id
);

oam_return_et
oam_agnt_send_agent_oam_im_ok_ind
(
    agent_oam_im_ok_ind_t  *p_agent_oam_im_ok_ind,
    UInt16                 src_module_id,
    UInt16                 dest_module_id,
    UInt16                 transaction_id,
    UInt8                  instance_id
);

#endif 
