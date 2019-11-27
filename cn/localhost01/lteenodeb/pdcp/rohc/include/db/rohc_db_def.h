/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: rohc_db_def.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 *  File Description : 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rohc_db_def.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/28 05:02:21  gur19836
 * File Header updated
 *
 *
 *
 ****************************************************************************/


#ifndef _ROHC_DB_DEF_H_
#define _ROHC_DB_DEF_H_

#define	ROHC_POOL_EXTEND_STEP	(10U)
#define IP4_KEY_POOL_SIZE		(50U)
#define IP6_KEY_POOL_SIZE		(100U)
#define ROHC_INVALID_CONTEXT_ID	(0x4000)
#define ROHC_MAX_PROTOCOL_NODE_POOL	(50U)
#define ROHC_MAX_STREAM_CONTEXTS	(100U)

#define PoolTunningNeeded(pool_name) \
	LTE_LOG(LOG_DUMP,PNULL, \
        "DB :: URGENT POOL TUNNING NEEDED for %s\n", \
            pool_name)


#endif /* _ROHC_DB_DEF_H_ */
