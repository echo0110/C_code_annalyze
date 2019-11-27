/***********************************************************************
************************************************************************
 **  FILE:
 **      Handles the Database initialization of GTP-U
 ***********************************************************************
 **
 **  DESCRIPTION:
 **      This file contains the various functions to initialize
 **      the GTP-U database.
 **      Sets the default echo timer values.
 **
 **  DATE           NAME            REFERENCE    REASON
 **  ----           ----            ---------    ------
 **  Oct, 2009      Vipul Aggarwal               Original
 **
 **   Copyright (C) 2009 Aricent Inc . All Rights Reserved
 ***********************************************************************
 **********************************************************************/

#include <egtpu_glob.h>
#include <egtpu_apihdr.h>

#include <egtpu_msgtypes.h>
#include <egtpu_ietypes.h>
#include <egtpu_error.h>
#include <egtpu_stat.h>
#include <egtpu_glob_struct.h>
#include <egtpu_cdb.h>
#include <egtpu_os_proto.h>
#include <egtpu_trace.h>
#include <egtpu_tmr.h>
#include <egtpu_util.h>

/****************************************************************************
 * Function Name  : __initdb__ 
 * Inputs         : p_global - pointer of structure egtpu_global_t  
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE 
 * Description    : This funtion Initializes common database (CDB)
 *                  Calls constructors of RAB-CTXT and PEER-CTXT tables
 *                  Does not care if any of them fails...
 ****************************************************************************/
egtpu_return_t __initdb__ (egtpu_global_t *p_global)
{	
    return egtpu_db_init_rab_ctxt_tbl (p_global) ||
        egtpu_db_init_peer_ctxt_tbl (p_global) || 
		egtpu_db_init_hash_ctxt_tbl (p_global) ||
		egtpu_db_init_hash_tbl (p_global) ||	
		egtpu_init_hash_table(p_global)  ||
		egtpu_db_init_timer_pool(p_global) ||
        /*+SPR #3456*/
        egtpu_db_init_peer_ctxt_addr_tbl (p_global);
        /*-SPR #3456*/
}
/****************************************************************************
 * Function Name  : egtpu_initdb 
 * Inputs         : p_global - pointer of structure egtpu_global_t  
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE 
 * Description    : This funtion Initializes common database (CDB)
 *                  Calls constructors of RAB-CTXT and PEER-CTXT tables
 *                  Does not care if any of them fails...
 ****************************************************************************/
 egtpu_return_t egtpu_initdb (egtpu_global_t *p_global)
{	
    return (__initdb__(p_global));
}

/****************************************************************************
 * Function Name  : __cleardb__ 
 * Inputs         : p_global - pointer of structure egtpu_global_t  
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE 
 * Description    : This funtion calls destructors of RAB-CTXT and PEER-CTXT tables
 *                  Cleans common database (CDB)
 ****************************************************************************/
egtpu_void_t __cleardb__ (egtpu_global_t *p_global)
{
    egtpu_db_destroy_rab_ctxt_tbl (p_global);
    egtpu_db_destroy_peer_ctxt_tbl (p_global);
	egtpu_db_destroy_hash_ctxt_tbl (p_global); 
	egtpu_db_destroy_hash_tbl (p_global);	
	egtpu_destroy_hash_table(p_global);
	egtpu_destroy_timer_pool(p_global);

}

/****************************************************************************
 * Function Name  : egtpu_initdb_req 
 * Inputs         : p_global - pointer of structure egtpu_global_t  
 *                  p_egtpu_info
 * Outputs        : None
 * Returns        : EGTP_SUCCESS/EGTP_FAILURE 
 * Description    : This funtion makes the stack operational
 *                 This function handles EGTPU_INITDB_REQ API
 *                 Calls egtpu_initdb to initialize CDB
 *                 Given dimensions of database
 *                 is put in global variable
 *                 Echo tmr defaults value are put in global variable
 ****************************************************************************/
egtpu_return_t egtpu_initdb_req (egtpu_global_t *p_global,
								egtpu_info_t *p_egtpu_info)
{

    U8bit      *p_buf, count, len, ip4_index = 0, ip6_index = 0,
               *p_param = p_egtpu_info->p_buff + EGTPU_API_HDR_LEN;
    egtpu_return_t ret_val, result=EGTPU_SUCCESS;

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_INITDB_REQ);

    /*
     *  Clear the initial database entries if any --->
     */

    if (p_global->stack_initdb_done)
        __cleardb__ (p_global);

    /*
     *  Store the initialization parameters given by LM into global
     */
    p_global->initdb_info.num_rab_rec.curr_size =
        EGTPU_GET_U32BIT (p_param);
	p_param = p_param + 4;

    p_global->initdb_info.num_rab_rec.step_size =
        EGTPU_GET_U32BIT (p_param);
	p_param = p_param + 4;

    p_global->initdb_info.num_rab_rec.max_size =
        EGTPU_GET_U32BIT (p_param);
	p_param = p_param + 4;

    p_global->initdb_info.num_peer_rec = EGTPU_GET_U16BIT(p_param);
	p_param = p_param + 2;

    p_global->initdb_info.num_my_addr4 = *p_param;
	p_param++;

    p_global->initdb_info.num_my_addr6 = *p_param;
	p_param++;

	if( (EGTPU_MAX_SRC_IP_ADDR < p_global->initdb_info.num_my_addr4 \
		 + p_global->initdb_info.num_my_addr6) || \
			(EGTPU_MIN_SRC_IP_ADDR > p_global->initdb_info.num_my_addr4 \
			                      + p_global->initdb_info.num_my_addr6) )
	{
		/** Return Failure,
		*** if num_my_addr is greater than maximum supported value
		*** or less than the minimum supported value of 1.
		**/
       /* SPR 2030 fix Start*/
       LOG_GTPU_MSG(GTPU_INITDB_REQ_FAIL_DUE_TO_NUM_SRC_IP, LOGWARNING, PR_GTPU_DL_DATA,
                relayGtpuCurrentTick_g, __LINE__, 
                EGTPU_MAX_SRC_IP_ADDR, EGTPU_MIN_SRC_IP_ADDR,
                ( p_global->initdb_info.num_my_addr4 + p_global->initdb_info.num_my_addr6),
                0, 0,0, __func__,"");
        /* SPR 2030 fix End*/
        p_global->stack_initdb_done = EGTPU_FALSE;
		result = EGTPU_FAILURE;
	}

  	else

	{
		for(count=0; count<p_global->initdb_info.num_my_addr4+p_global->initdb_info.num_my_addr6; count++)
		{
			len = EGTPU_GET_U16BIT(p_param); 
			if (len == EGTPU_SIZE_OF_IPV4_ADDR)
			{
				egtpu_memcpy(p_global->initdb_info.my_addr[ip4_index], p_param+EGTPU_SIZE_OF_LENGTH, len);
				p_param += (len + EGTPU_SIZE_OF_LENGTH);
				ip4_index++;
			}
			else if (len == EGTPU_SIZE_OF_IPV6_ADDR)
			{
				egtpu_memcpy(p_global->initdb_info.my_addr6[ip6_index], p_param+EGTPU_SIZE_OF_LENGTH, len);
				p_param += (len + EGTPU_SIZE_OF_LENGTH);
				ip6_index++;
			}
			else						
			{
				p_global->stack_initdb_done = EGTPU_FALSE;
               /* SPR 2030 fix Start*/
               LOG_GTPU_MSG(GTPU_INIT_FAIL_DUE_TO_IP_LEN, LOGWARNING, PR_GTPU_DL_DATA,
               relayGtpuCurrentTick_g, __LINE__, 
               EGTPU_SIZE_OF_IPV4_ADDR,EGTPU_SIZE_OF_IPV6_ADDR,len,0, 0,0,
               __func__,"");
               /* SPR 2030 fix End*/
			   	result = EGTPU_FAILURE;
				break;
			}
		}
  
  		if(result == EGTPU_SUCCESS)
		{
            /*+SPR #3456*/
            /* SPR 10597 Changes Start */
            p_global->tmr_info[0].tmr_dur =
                EGTPU_GET_U32BIT (p_param);
            p_param = p_param + 4;
            /* SPR 10597 Changes End */
    		p_global->tmr_info[0].max_retries = *p_param;
            p_param++;


    		p_global->ext_hdr_map = EGTPU_EXT_HDR_PDCP;

    	/*
     	*  Heart of Initialization. Initialize all the Data Structures.
     	*/
    		if ((result = egtpu_initdb (p_global)) == EGTPU_FAILURE)
			{
        		__cleardb__ (p_global);
        		p_global->stack_initdb_done = EGTPU_FALSE;
    		}
			else
        		p_global->stack_initdb_done = EGTPU_TRUE;
		}
	}

    /*
     *  Sending cnf (success/failure) to Layer Management Entity
     */

    EGTPU_GET_MSG_BUF (p_global, U8bit, p_buf, EGTPU_API_MSG_SIZE);

    /* Rel 5.3: Coverity 24418 Fix Start */
    if (EGTPU_NULL == p_buf)
    {
        /* free the initdb_req buffer */
        EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);

        /* Not able to allocate Memory */
        return EGTPU_FAILURE;
    }
    /* Rel 5.3: Coverity Fix End */

    *(p_buf + EGTPU_API_HDR_LEN) = result;

    EGTPU_SET_API_HDR (p_buf, EGTPU_EI_GTPU, EGTPU_EI_RELAY,
            EGTPU_LM_INITDB_CNF, (EGTPU_API_HDR_LEN + EGTPU_SIZE_OF_RESULT),
            EGTPU_NO_PAYLOAD, EGTPU_NO_PAYLOAD, EGTPU_API_HDR_LEN);

    EGTPU_SEND_FROM_GTPU (p_global,p_buf,
            (EGTPU_API_HDR_LEN + EGTPU_SIZE_OF_RESULT), EGTPU_EI_RELAY,
			ret_val, NULL);

    /* SPR 2030 fix Start*/
    LOG_GTPU_MSG(GTPU_INITDB_CNF_SENT_TO_RELAY, LOGDEBUG, PR_GTPU_DL_DATA,
             relayGtpuCurrentTick_g, __LINE__, 
             0,0,0,0, 0,0, __func__,"");
     /* SPR 2030 fix End*/
 
    /* free the initdb_req buffer */
    EGTPU_FREE_MSG_BUF (p_egtpu_info->p_buff);
    EGTPU_FREE_MSG_BUF (p_buf);

    EGTPU_UPDATE_STATS (p_global,EGTPU_STATS_API_LM_INITDB_CNF);
    /* + coverity 32081 */
    return ret_val;
    /* - coverity 32081 */
}
