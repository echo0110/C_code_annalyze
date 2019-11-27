/***********************************************************************
**  FUNCTION:
**      Handle Statistics related APIs
************************************************************************
**
**  FILE NAME:
**      egtpu_stat.c
**
**  DESCRIPTION:
**      This file contains a function that gets statistics information
**      based on the type of stats
**
**  DATE            NAME            REFERENCE    REASON
**  ----            ----            ---------    ------
**  Oct, 2009       Vipul Aggarwal              Original
**
**   Copyright (C) 2009 Aricent Inc. All Rights Reserved
***********************************************************************/

#include <ylib.h>
#include <egtpu_glob.h>
#include <egtpu_apihdr.h>
#include <egtpu_error.h>

#include <egtpu_msgtypes.h>
#include <egtpu_ietypes.h>
#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_os_proto.h>
#include <egtpu_trace.h>
#include <egtpu_util.h>

#ifdef EGTPU_ENABLE_STATS

/***********************************************************************
**  DATA:
**      This structure holds information about start and end indices in
**      the statistics array.  The array that follow hold info per stats
**      type.
**
**  NOTE:
**      Array indices are tied to type values in egtpu_glob.h (careful)
**
**  USAGE:
**      local
***********************************************************************/
static const struct {
    U32bit first;
    U32bit last;
} __stats_arr_info__[] = {
    /* EGTPU_STATS Fix Start */
    {EGTPU_STAT_PMSG_BASE, EGTPU_STAT_LAST_PMSG},
    {EGTPU_STAT_IN_API_BASE, EGTPU_STAT_LAST_OUT_API},
    {EGTPU_STAT_ERROR_BASE, EGTPU_STAT_LAST_ERROR},
    /* EGTPU_STATS Fix End */
    /* SPR 7085 fix start */
    {EGTPU_STAT_INTERNAL_BASE, EGTPU_STAT_LAST_INTERNAL},
    {EGTPU_STAT_ERR_PMSG_BASE ,EGTPU_STAT_LAST_ERRONEOUS_PMSG_BASE}
    /* SPR 7085 fix end */
};

#endif

/****************************************************************************
 * Function Name  : __fill_stats__ 
 * Inputs         : p_global - pointer of structure egtpu_global_t  
 *                  type - statistics type requested 
 *                  init - initialize the stats after retrieving
 * Outputs        : p_buf - outgoing buffer 
 * Returns        : end of buffer 
 * Description    : This funtion Get GTP-U statistics based on type 
 *                  and optionally initialize the counters to zero. 
 *                  Then fill those numbers in the outgoing buffer
 ****************************************************************************/
static U8bit *__fill_stats__ (egtpu_global_t *p_global, U8bit *p_buf,
								U8bit type, egtpu_boolean_t init)
{
#if defined EGTPU_ENABLE_STATS
    if (type <= EGTPU_STATS_INTERNAL)
	{
        U32bit i;

        *p_buf++ = EGTPU_SUCCESS;
        *p_buf++ = type;

        EGTPU_SET_U16BIT (p_buf, (__stats_arr_info__[type].last -
                    __stats_arr_info__[type].first + 1));
        p_buf += EGTPU_SIZE_OF_LENGTH;

        for (i = __stats_arr_info__[type].first;
                i <= __stats_arr_info__[type].last; i++)
		{
            EGTPU_SET_U32BIT (p_buf, \
					p_global->egtpu_stats_arr[i - EGTPU_STAT_BASE]);
            p_buf += EGTPU_SIZE_OF_STAT_ENTRY;

            if (init)
                p_global->egtpu_stats_arr[i - EGTPU_STAT_BASE] = EGTPU_NULL;
        }

        return p_buf;
    }

    *p_buf++ = EGTPU_CERR_INV_STATS_TYPE;
    EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_CERR_INV_STATS_TYPE,
                egtpu_entity_name_arr[EGTPU_EI_GTPU], type));
#else
    *p_buf++ = EGTPU_CERR_STATS_DISABLED;
    EGTPU_ERROR_REPORTER (p_global,(EGTPU_TS_CERR_STATS_DISABLED,
                egtpu_entity_name_arr[EGTPU_EI_GTPU]));
#endif

    /* SPR 17747 FIX START */
    LOG_MSG( GTPU_ERROR_REPORTER, LOGWARNING, PR_GTPU_COMMON,
            relayGtpuCurrentTick_g, __LINE__, EGTPU_CERR_STATS_DISABLED,
            0,0,0,0,0, __func__, egtpu_entity_name_arr[EGTPU_EI_GTPU] );
    /* SPR 17747 FIX END */
    *p_buf++ = type;
    return p_buf;
}

/****************************************************************************
 * Function Name  : egtpu_get_stats_req 
 * Inputs         : p_global - pointer of structure egtpu_global_t  
 *                  p_egtpu_info 
 * Outputs        : None
 * Returns        : EGTPU_SUCCES/EGTPU_FAILURE
 * Description    : Calls __fill_stats__ to fill-in all stats of a particular type
 *                  and sends confirmation to LM
 ****************************************************************************/
egtpu_return_t egtpu_get_stats_req (egtpu_global_t *p_global,
									egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf, *p_end;
    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_GET_STATS_REQ);

    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_MAX_MSG_SIZE);

    /* Rel 5.3: Coverity 24397 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */
    p_end = __fill_stats__ (p_global,p_buf + EGTPU_API_HDR_LEN,
            *(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN), EGTPU_FALSE);

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_GET_STATS_CNF, (p_end - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_end - p_buf - 1));

    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_end - p_buf),\
							EGTPU_EI_LM, ret_val, NULL);

    /* free the buffer allocated for request API */
    /* During Covering it is found that in success scenario also it should be freed */
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);
    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_GET_STATS_CNF);
    return ret_val;
}

/****************************************************************************
 * Function Name  : egtpu_get_init_stats_req 
 * Inputs         : p_global - pointer of structure egtpu_global_t  
 *                  p_egtpu_info 
 * Outputs        : None
 * Returns        : EGTPU_SUCCES/EGTPU_FAILURE
 * Description    : Calls __fill_stats__ to fill-in all stats of a particular type
 *                  initialise it to 0 and sends confirmation to LM.
 ****************************************************************************/
egtpu_return_t egtpu_get_init_stats_req (egtpu_global_t *p_global,
										egtpu_info_t *p_egtpu_info)
{
    U8bit         *p_buf, *p_end;
    egtpu_return_t ret_val;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_GET_INIT_STATS_REQ);

    EGTPU_GET_MSG_BUF (p_global,U8bit, p_buf, EGTPU_MAX_MSG_SIZE);
    if (EGTPU_NULL == p_buf)
    {
        /* Free the allocated Memory */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
        return EGTPU_FAILURE;
    }

    p_end = __fill_stats__ (p_global,p_buf + EGTPU_API_HDR_LEN,
            *(p_egtpu_info->p_buff + EGTPU_API_HDR_LEN), EGTPU_TRUE);

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_LM,
            EGTPU_LM_GET_INIT_STATS_CNF, (p_end - p_buf),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, (p_end - p_buf - 1));

    EGTPU_SEND_FROM_GTPU (p_global,p_buf, (p_end - p_buf), EGTPU_EI_LM, \
												ret_val, NULL);

    /* free the buffer allocated for request API */
    /* During Covering it is found that in success scenario also it 
     * should be freed */
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_GET_INIT_STATS_CNF);
    return ret_val;
}
