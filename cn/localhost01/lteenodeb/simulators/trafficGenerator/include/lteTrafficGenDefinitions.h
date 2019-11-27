/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) Aricent.
 *
 ****************************************************************************
 *
 *  $Id: lteTrafficGenDefinitions.h,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
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
 * $Log: lteTrafficGenDefinitions.h,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.1.2.2.2.1  2009/09/24 21:45:29  gur19479
 * updated for video and audio processing
 *
 * Revision 1.1.2.2  2009/07/14 14:08:22  gur19836
 * CR Comments Disposed
 *
 * Revision 1.1.2.1  2009/07/12 09:37:31  gur19479
 * updated for traffic generator
 *
 *
 ****************************************************************************/

#ifndef INCLUDED_LTE_TRAFFIC_GEN_DEFINITIONS_H
#define INCLUDED_LTE_TRAFFIC_GEN_DEFINITIONS_H

/****************************************************************************
 * Project Includes
 ****************************************************************************/

#include <stdio.h>
#include <stdlib.h>

/****************************************************************************
 * Exported Includes
 ****************************************************************************/

/****************************************************************************
 * Exported Types
 ****************************************************************************/

/****************************************************************************
 * Exported Definitions
 ****************************************************************************/

/* SPR 20526 Fix Start */
#define   MAX_UE                   MAX_UE_SUPPORTED
/* SPR 20526 Fix End */
#define   MEM_POOL_SIZE_UE_DATA    sizeof(UeData)
#define   MAX_LC                   11
#define   TRUE                     1 
#define   FALSE                    0 
#define   SYS_CALL_FAIL            -1

#define   SIZE_OF_PDU              9422 //3712
#define   SIZE_OF_PDU_DATA         9422 //3710
#define   SIZE_OF_PDU_HEADER       2
#define   MAX_PDCP_DATA            (MAX_UE * MAX_LC)

#define   TIME_IN_SECOND           1000//4000//100000
#define   ONE_TICK_TIME            250//1000//100  

#define   MAX_TIME_INTERVAL_BUCKETS         1000  
#define   DEFAULT_GRANULARITY                   5
typedef enum profiles
{
    TRAFFIC_GEN_DATA_PROFILE =1,
    TRAFFIC_GEN_AUDIO_PROFILE,
    TRAFFIC_GEN_VIDEO_PROFILE,
}profiles_e;



/****************************************************************************
 * Exported Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

#endif  /* INCLUDED_LTE_RLC_SIM_DEFINITIONS_H */
