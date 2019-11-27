/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_dcpm_p6_r_done.h,v 1.0.0.0 2013/06/21 04:29:05 gur30693 Exp $
 *
 ****************************************************************************
 *
 *  File Description : This file contains function declaration for 
 *                     Decompressor CPM.
 *
 ****************************************************************************
 *
 *
 ****************************************************************************/


rohc_void_t rohcDP6IrIrCRNoContext(
         rohc_entity_t *p_entity ,
         rohc_dsc_tcp_t *p_dsc);

rohc_void_t rohcDP6IrIrCRStaticContext(
         rohc_entity_t *p_entity ,
         rohc_dsc_tcp_t *p_dsc);

rohc_void_t rohcDP6AllFullContext(
         rohc_entity_t *p_entity ,
         rohc_dsc_tcp_t *p_dsc);

rohc_void_t rohcDP6OthersNoContext(
            rohc_entity_t *p_entity ,
            rohc_dsc_tcp_t *p_dsc);

rohc_void_t rohcDP6IrdynCOStaticContext(
        rohc_entity_t *p_entity ,
        rohc_dsc_tcp_t *p_dsc);

rohc_void_t rohcDP6OthersStaticContext(
        rohc_entity_t *p_entity ,
        rohc_dsc_tcp_t *p_dsc);
