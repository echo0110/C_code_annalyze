/***********************************************************************
**  FUNCTION:
**	Functions for Parsing the APIs
************************************************************************
**
**  FILE NAME:
**	egtpu_parse.h
**
**  DESCRIPTION:
**	Contains the functions for parsing the APIs
**
**  DATE	    NAME			REFERENCE	REASON
**  ----	    ----			---------	------
**  Oct 2009    Vipul Aggarwal        		Initial
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef __EGTPU_PARSE_DECL__
#define __EGTPU_PARSE_DECL__

egtpu_return_t egtpu_parse_msg (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t egtpu_setup_sap_req_parse (egtpu_global_t *, egtpu_info_t *);
egtpu_return_t egtpu_get_ctxt_info_req_parse (egtpu_global_t *,
												egtpu_info_t *);

extern egtpu_return_t egtpu_modify_reordering_ie_parse (egtpu_global_t *,
														egtpu_info_t *);
#endif
