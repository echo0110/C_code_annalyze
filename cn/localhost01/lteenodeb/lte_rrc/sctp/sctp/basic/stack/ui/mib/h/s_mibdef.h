/*********************************************************************
 *
 *  FILE NAME:   s_mibdef.h
 *
 *  DESCRIPTION:  contain all API prototype.
 *
 *    DATE         NAME           REF#               REASON
 *    ---------------------------------------------------------
 *    08May2001    sigtran team    HSS:40600244      Original
 *    28Sep2001    Yogesh Gahlaut      -             Rel 3.0 changes
 *
 *   Copyright (C) 2006 Aricent Inc . All Rights Reserved
 ********************************************************************/

/* this API is to get all state controlled statistics */
sctp_return_t
sctpmib_get_statecontrol_stat(
    sctpmib_state_stat_params_st      *p_stats,
    sctp_error_t                      *p_ecode );

/* API for getting all common state parameters */
sctp_return_t
sctpmib_get_common_stat_parameter(
    sctpmib_other_stat_params_st      *p_stats,
    sctp_error_t                      *p_ecode );

/* API for getting global config parameters */
sctp_return_t
sctpmib_get_config_parameter(
    sctpmib_config_params_st     *p_params,
    sctp_error_t             *p_ecode );

/* this API is to get association table variables */
sctp_return_t
sctpmib_get_assoc_table_var(
    sctp_U32bit                   assoc_id,
    sctp_sockaddr_st              *dest_add,
    sctp_sockaddr_st          *p_local_add,
    sctpmib_assoc_table_st        *p_assoc_tab,
    sctp_error_t                  *p_ecode );


/* this API is to set association state variable */
sctp_return_t
sctpmib_set_assoc_state(
    sctp_U32bit                   assoc_id,
    sctpmib_assoc_state_et        assoc_state,
    sctp_error_t                  *p_ecode );

