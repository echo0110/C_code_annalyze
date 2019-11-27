/******************************************************************************
*
*   FILE NAME:
*       rrc_shared_stats.c
*
*   DESCRIPTION:
*       This file contains shared memory related definitions.
*
*   DATE            AUTHOR      REFERENCE       REASON
*   5 Sep 2012                  ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#include "rrc_shared_stats.h"
#include "rrc_common_utils.h"

/* GDB fix - duplicate symbols at link time */
rrc_stats_info_t g_rrc_stats;

/******************************************************************************
*   FUNCTION NAME: init_shared_mem_stats 
*   INPUT        : void
*   OUTPUT       : None
*   RETURNS:       RRC_FAILURE or RRC_SUCCESS
*   DESCRIPTION:   This function initializes the shared memory . 
******************************************************************************/   
rrc_return_et init_shared_mem_stats( void ) 
{
    /* SPR 7148 start*/
    S32 retVal = 0;
    /* SPR 7148 end */

    /* SHM_INIT Start */
    g_rrc_stats.rrc_shm_id = shmget_wrapper(RRC_STATS_SHM_KEY, 0, RRC_STATS_SHM_LOCATE_FLAGS);
    if (g_rrc_stats.rrc_shm_id >= 0)
    {
        retVal = shmctl_wrapper(g_rrc_stats.rrc_shm_id, IPC_RMID,PNULL);
        /*coverity 81411 fix start*/
        if ( -1 == retVal )
        {
            RRC_TRACE(RRC_ERROR, "SHMCTL failed %d", retVal);
        }
        /*coverity 81411 fix end*/
    }
    /* SHM_INIT Stop */
    /* Get the shared memory ID */
    g_rrc_stats.rrc_shm_id = shmget_wrapper(RRC_STATS_SHM_KEY, sizeof(rrc_stats_t), RRC_STATS_SHM_CREATE_FLAGS);
    if (0 > g_rrc_stats.rrc_shm_id)
    {
        RRC_TRACE(RRC_ERROR, "SHM get failed");
        return RRC_FAILURE;
    }
    /* Map Shared memory */
    /* Covertity fix 100023 start */
    g_rrc_stats.p_rrc_stats =(rrc_stats_t *) shmat_wrapper(g_rrc_stats.rrc_shm_id, RRC_NULL, 0);
    /* Covertity fix 100023 end */
    if ((void *)-1 == g_rrc_stats.p_rrc_stats)     
    {
        RRC_TRACE(RRC_ERROR, "SHM attach failed");
        return RRC_FAILURE;
    }
    /* SPR 7148 start*/
    retVal = shmctl_wrapper(g_rrc_stats.rrc_shm_id, IPC_RMID,PNULL);
    if ( -1 == retVal )
    {
        RRC_TRACE(RRC_ERROR, "SHMCTL failed %d", retVal);
    }
    /* SPR 7148 end */

    /* Initialize shared memory */
    memset_wrapper(g_rrc_stats.p_rrc_stats, RRC_NULL, sizeof(rrc_stats_t));
    return RRC_SUCCESS;
}
