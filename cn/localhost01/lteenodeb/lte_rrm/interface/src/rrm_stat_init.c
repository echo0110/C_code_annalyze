/******************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc . All Rights Reserved.
 *
 *******************************************************************************
 *
 * $$Id: rrm_stat_init.c $
 *
 *******************************************************************************
 *
 * File Description: Stat init functions definations.
 *
 *******************************************************************************
 * Revision Details
 * ----------------
 *
 ******************************************************************************/

#include "rrm_stats.h"
#include "rrm_utils.h"
#include "rrm_defines.h"
#include "rrm_mif_fsm.h"
#include <sys/ipc.h>
#include <sys/shm.h>

rrmStats_t*                  gp_rrmStats;

rrmStats_t* rrm_stat_shm_init
(
 unsigned short                      access_mode
 )
{

    S32  shmid;
    rrmStats_t *p_rrmStats;

    if(access_mode == W_MODE)
    {
        /*
         * Create the segment.
         */
        if ((shmid = RRM_SHMGET(RRM_STATS_SHARED_MEM_KEY, RRM_STATS_SHARED_MEM_SIZE,IPC_CREAT | 0666)) < 0)
        {
            RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,"error in creating shared Memory");
            RRM_PRINTF("error in creating shared Memory\n");
            return RRM_PNULL;
        }
    }
    else
    {
        /*
         * Locate the segment
         */
        if ((shmid = RRM_SHMGET(RRM_STATS_SHARED_MEM_KEY, RRM_STATS_SHARED_MEM_SIZE, 0666)) < 0)
        {
            RRM_PRINTF("error in getting shared Memory\n");
            return RRM_PNULL;
        }
    }

    /*
     * Now we attach the segment to our data space.
     */
    p_rrmStats = (rrmStats_t*)shmat(shmid, NULL, 0);
    if ((rrm_void_t *)p_rrmStats == (S8 *) -1)
    {
        RRM_PRINTF("Eror in Attaching Shared memory to process\n");
        return RRM_NULL;
    }
    return p_rrmStats;
}



