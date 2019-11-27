/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2017 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: oam_agnt_parser.h $
 *
 *******************************************************************************
 *
 * File Description: File oam_agnt_parser.h added for OAM AGENT module code
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#ifndef _OAM_AGNT_PARSER_H_
#define _OAM_AGNT_PARSER_H_

oam_agnt_return_et
oam_agnt_parse_agent_oam_init_ind
(
    agent_oam_init_ind_t *p_agent_oam_init_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_agnt_return_et
oam_agnt_parse_oam_agent_spawn_process_req
(
    oam_agent_spawn_process_req_t *p_oam_agent_spawn_process_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_agnt_return_et
oam_agnt_parse_agent_oam_spawn_process_resp
(
    agent_oam_spawn_process_resp_t *p_agent_oam_spawn_process_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_agnt_return_et
oam_agnt_parse_oam_agent_kill_process_req
(
    oam_agent_kill_process_req_t *p_oam_agent_kill_process_req,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_agnt_return_et
oam_agnt_parse_agent_oam_kill_process_resp
(
    agent_oam_kill_process_resp_t *p_agent_oam_kill_process_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_agnt_return_et
oam_agnt_parse_agent_oam_get_logs_resp
(
    agent_oam_get_logs_resp_t *p_agent_oam_get_logs_resp,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_agnt_return_et
oam_agnt_parse_oam_agent_get_logs_cnf
(
    oam_agent_get_logs_cnf_t *p_oam_agent_get_logs_cnf,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

oam_agnt_return_et
oam_agnt_parse_agent_oam_im_ok_ind
(
    agent_oam_im_ok_ind_t *p_agent_oam_im_ok_ind,
    UInt8  *p_src,
    SInt32 length_left,
    SInt32 *p_length_read
);

#endif 
