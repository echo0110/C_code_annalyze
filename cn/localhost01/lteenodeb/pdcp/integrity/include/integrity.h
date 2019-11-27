/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 *  $Id: integrity.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ****************************************************************************
 *
 * File Description : Integrity functions for generating and checking of MACI 
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: integrity.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:28:26  gur19140
 * *** empty log message ***
 *
 * Revision 1.3  2009/05/20 14:02:14  gur20548
 * header file name changed
 *
 * Revision 1.2  2009/05/14 12:38:01  gur19836
 * CR Comments Incorporated
 *
 *
 *
 ****************************************************************************/

#ifndef _INTEGRITY_DEF_
#define _INTEGRITY_DEF_

/****************************************************************************
 * Project Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Includes
 ****************************************************************************/
#include "in_lteFramework.h"
#include "ltePdcpLteDefs.h"

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/****************************************************************************
 * Function Name  : integrityGenerateMACI
 * Inputs         : UInt16 AlgorithmId (ALGO_EIA1/ALGO_EIA2)
 *					UInt16 lcId
 *					UInt32 count
 *					UInt16 direction - 0(downlink) or 1(uplink)
 *					UInt8 *key_p - integrity key
 *					void *data_p - ZCB with data to calc MACI for
 * Outputs        : UInt8 *MACI_p - calculated MACI will be placed here
 * Returns        : None
 * Description    : Calculates MACI
 ****************************************************************************/
void integrityGenerateMACI(UInt16 AlgorithmId, UInt16 lcId, UInt32 count,
          UInt16 direction, UInt8 *key_p, 
	  /* + SPR 18103 */
	  void *data_p,
	  /* - SPR 18103 */
	  UInt8 *MACI_p);

/****************************************************************************
 * Function Name  : integrityCheckMACI
 * Inputs         : UInt16 AlgorithmId (ALGO_EIA1/ALGO_EIA2)
 *					UInt16 lcId
 *					UInt32 count
 *					UInt16 direction - 0(downlink) or 1(uplink)
 *					UInt8 *key_p - integrity key
 *					void *data_p - ZCB with data to calc MACI for
 *                  UInt8 * flag - to check for MACI against 0 in the SMC/SMF msg.
 * Outputs        : None
 * Returns        : PDCP_SUCCESS (MACI is valid) / PDCP_FAIL (MACI is invalid)
 * Description    : Checks if integrity MACI is valid
 ****************************************************************************/
/* SPR 8174 Changes Start */
UInt16 integrityCheckMACI(UInt16 AlgorithmId, UInt16 lcId, UInt32 count,
           UInt16 direction, UInt8 *key_p, 
    /* + SPR 18103 */
	  void *data_p
    /* - SPR 18103 */
#ifndef UE_SIM_TESTING
		   ,UInt8 *flag
#endif
		   );
/* SPR 8174 Changes End */
#endif
