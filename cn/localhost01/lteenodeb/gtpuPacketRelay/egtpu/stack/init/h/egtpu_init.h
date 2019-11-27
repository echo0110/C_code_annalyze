/***********************************************************************
************************************************************************
 ** FILE
 **     This is Header file for egtpu_init.c
 ***********************************************************************
 **
 ** DESCRIPTION:
 **     This file contains the various functions to initialize
 **     the GTP-U database
 **
 ** DATE            NAME            REFERENCE    REASON
 ** ----            ----            ---------    ------
 ** Oct, 2009      Vipul Aggarwal   			Original
 **
 **   Copyright (C) 2009 Aricent Inc . All Rights Reserved
 ***********************************************************************
 **********************************************************************/

#ifndef __EGTPU_INIT_DECL__
#define __EGTPU_INIT_DECL__

egtpu_return_t egtpu_initdb_req (egtpu_global_t *,egtpu_info_t *);
egtpu_return_t __initdb__ (egtpu_global_t *p_global);
egtpu_return_t egtpu_initdb (egtpu_global_t *p_global);
egtpu_void_t __cleardb__ (egtpu_global_t *p_global);

#endif
