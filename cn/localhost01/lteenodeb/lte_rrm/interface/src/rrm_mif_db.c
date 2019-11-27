/****************************************************************************
 *
 *  ARICENT GROUP
 *
 *  Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: rrm_mif_db.c v0.1 2012/02/02 10:50:45 $
 ****************************************************************************
 *
 *  File Description :
 *      This file contains contains the context creation and maintainance 
 *      functions for the MIF modules
 *
 *   DATE            AUTHOR      REFERENCE       REASON
 *   13 June ,2014     gur29831    SPR-11838     Plmn id fix.
 *   17 July 2014    gur31824    SPR-12598       Check for correct values of 
 *                                               initial_ri_periodicity
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: rrm_mif_db.c  $
 * Revision 0.2  2012/01/31 09:50:42 gur29990
 * Initial code
 *
 *
 * Revision 0.3  2012/05/30 06:38:42 gur29220
 * 
 * Revision 0.4  2012/08/09 15:40:00 gur27278
 * Removed cross compilation warnings
 * 06 June 2014 gur29990    11317    OLPC redesigned. 
 * 18 June 2014 gur26515    11922    Validation check for CQI and SR periodicity 
 *
 * Revision 0.5  2014/05/01 15:30:38 gur32417
 * PUCCH Format 1b_cs-v1020 & Format 3-v1020 
 * handling for Carrier Aggrigation
 * 08 March 2016	RAVI P  Bug-4397/18936 - q_qual_min_r9 should not be included if not configured from OAM 
 ****************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/
#include "rrm_event_defines.h"
#include "rrm_mif_db.h"
#include "rrm_mem_utils.h"
#include "rrm_utils.h"

/*SPR 15993 Fix Start*/
U16 g_transaction_id = RRM_ZERO;
/*SPR 15993 Fix End*/
/****************************************************************************
 * Function Name  : rrm_alloc_mif_context_from_pool
 * Inputs         : U8     p_mif_ctxt_id: Context identifier for MIF
 * Outputs        : rrm_mif_gb_context_t     p_gl_ctxt : global Context
 * Returns        : None
 * Description    : Initialises the mif context parameters
 ****************************************************************************/
static void 
*rrm_alloc_mif_context_from_pool (U8                   *p_mif_ctxt_id, 
		rrm_mif_gb_context_t *p_gl_ctxt);

/****************************************************************************
 * Function Name  : rrm_find_mif_context_from_pool
 * Inputs         : U8     p_mif_ctxt_id: Context identifier for MIF
 * Outputs        : rrm_mif_gb_context_t     p_gl_ctxt : global Context
 * Returns        : None
 * Description    : brief Finds MIF Context from pool
 ****************************************************************************/
static void 
*rrm_find_mif_context_from_pool (U8                    mif_ctxt_id, 
		rrm_mif_gb_context_t *p_gl_ctxt);

/****************************************************************************
 * Function Name  : rrm_dealloc_mif_context_from_pool
 * Inputs         : U16     dst_module_id: Destination module identifier
 *                : U16     api_id  : API Id
 * Outputs        : None
 * Returns        : RRM_SUCCESS/RRM_FAILURE
 * Description    : Initialises the cell context cell_resource and other
 *                  parameters
 ****************************************************************************/
static void rrm_dealloc_mif_context_from_pool (rrm_mif_context *p_mif_context);


/****************************************************************************
 * Function Name  :rrm_num_pool_block 
 * Inputs         :
 *      1.p_gl_ctxt is not null pointer.
 * Outputs        : 
 * Returns        : Number of used buffer in the MIF context Pool.
 * Description    : This function returns the Number of Context buffer used 
 *                  with in the pool.
 ****************************************************************************/
U32 rrm_num_pool_block (rrm_mif_gb_context_t *p_gl_ctxt)
{
	unsigned int      num_used = RRM_ZERO;

	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
	qvPoolSize(p_gl_ctxt->mif_ctxt_pool.pool, &num_used);
	RRM_UT_TRACE_EXIT();
	return num_used;
}

/****************************************************************************
 * Function Name  : rrm_alloc_mif_context
 * Inputs         : 
 *      1.p_gl_ctxt is not null pointer.
 * Outputs        : 
 * Returns        : MIF Context Pointer.
 * Description    : This function allocates the MIF Context.
 ****************************************************************************/
rrm_mif_context *rrm_alloc_mif_context (rrm_mif_gb_context_t *p_gl_ctxt)
{
	rrm_mif_context         *p_mif_context = RRM_PNULL;
	U8                     mif_ctxt_id      = RRM_ZERO;

	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != p_gl_ctxt);  

	p_mif_context = rrm_alloc_mif_context_from_pool(&mif_ctxt_id, p_gl_ctxt);
	RRM_ASSERT (RRM_PNULL != p_mif_context);
	/* Coverity 94797 Fix Start */
	if (RRM_PNULL == p_mif_context)
	{
	      RRM_UT_TRACE_EXIT();
	      return p_mif_context;
	}
	/* Coverity 94797 Fix End */
	RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Allocated MIF Context [%d]", mif_ctxt_id);
	RRM_MEMSET(p_mif_context, RRM_ZERO, sizeof(rrm_mif_context));

	p_mif_context->mif_ctxt_id = mif_ctxt_id;

	RRM_UT_TRACE_EXIT();

	return p_mif_context;
}


/****************************************************************************
 * Function Name  : rrm_alloc_mif_context_from_pool
 * Inputs         : 
 *      1.p_gl_ctxt is not null pointer.
 * Outputs        : 
 *      1.Mif Ctxt Id          : Returns MIF Ctxt ID.
 * Returns        : void pointer.
 * Description    : This function allocates the MIF Context From Pool.
 ****************************************************************************/
static void *rrm_alloc_mif_context_from_pool (U8               *p_mif_ctxt_id, 
		rrm_mif_gb_context_t *p_gl_ctxt)
{
	void *p_buf = RRM_PNULL;

	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != p_mif_ctxt_id);
	RRM_ASSERT (RRM_PNULL != p_gl_ctxt);

	p_buf = cvContextAlloc(&(p_gl_ctxt->mif_ctxt_pool));
	RRM_ASSERT (RRM_PNULL != p_buf);
	*p_mif_ctxt_id = cvContextIndex(&(p_gl_ctxt->mif_ctxt_pool), p_buf);

	RRM_UT_TRACE_EXIT();
	return p_buf; 
}


/****************************************************************************
 * Function Name  : rrm_mif_create_pool
 * Inputs         :
 *      1.p_gl_ctxt is not null pointer.
 * Outputs        :
 * Returns        : RRM_SUCCESS / RRM_FAILURE
 * Description    : This function Creates the MIF Pool.
 ****************************************************************************/
rrm_return_et rrm_mif_create_pool (rrm_mif_gb_context_t *p_gl_ctxt)
{
	rrm_return_et    ret_val = RRM_SUCCESS;

	RRM_UT_TRACE_ENTER();
	RRM_ASSERT(RRM_PNULL != p_gl_ctxt);

	if(RRM_FALSE == p_gl_ctxt->is_mif_ctxt_pool_created)
	{
		/* 
		   mif_ctxt_pool will be created once only when the 
		   MIF moved to ACTIVE state. 
		 */
		ret_val = rrm_cv_context_pool_setup( &(p_gl_ctxt->mif_ctxt_pool),
				sizeof(rrm_mif_context),
				RRM_MIN_MIF_CONTEXT,
				RRM_MAX_MIF_CONTEXT,
				RRM_STEP_SIZE);
		if (ret_val == RRM_FAILURE)
		{
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "rrm_mif_create_pool: Memory Pool Allocation"
					"Failed");
		}
		else
		{
			p_gl_ctxt->is_mif_ctxt_pool_created = RRM_TRUE;
		}
	}
	RRM_UT_TRACE_EXIT();
	return ret_val;
}

/****************************************************************************
 * Function Name  : rrm_cv_context_pool_setup
 * Inputs         : cpool - it's not null pointer.
 * Outputs        :
 * Returns        :
 * Description    : This is the porting function for creating POOL for
 *                  RRM MIF module.
 ****************************************************************************/
rrm_return_et rrm_cv_context_pool_setup(STACKCPOOL *cpool, U32 bufsize, U32 min,
		U32 max, U32 step)
{
	rrm_return_et    ret_val = RRM_FAILURE;

	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != cpool);
	/* coverity : CID 31022 */
	ret_val = (rrm_return_et) cvContextPoolSetup(cpool, bufsize, min, max, step);
	if (ret_val == RRM_SUCCESS)
	{
		ret_val = RRM_SUCCESS;
	}
	RRM_UT_TRACE_EXIT();
	return ret_val;
}


/****************************************************************************
 * Function Name  : rrm_find_mif_context
 * Inputs         : 
 *      1.p_gl_ctxt : is not null pointer
 * Outputs        : 
 * Returns        : MIF Context Pointer.
 * Description    : This function finds the MIF Context Block based on mif_ctxt_id.
 ****************************************************************************/
rrm_mif_context *rrm_find_mif_context (rrm_mif_gb_context_t *p_gl_ctxt, 
		U8                    mif_ctxt_id)
{
	rrm_mif_context         *p_mif_context = RRM_PNULL;
	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
	p_mif_context = rrm_find_mif_context_from_pool(mif_ctxt_id, p_gl_ctxt);

	RRM_UT_TRACE_EXIT();
	return (p_mif_context);
}


/****************************************************************************
 * Function Name  : rrm_find_mif_context_from_pool
 * Inputs         : 
 *      1.mif_ctxt_id          : MIF Identifier.
 *      1.p_gl_ctxt is not null pointer.
 * Outputs        : 
 * Returns        : MIF Context Pointer
 * Description    : This function find the MIF Context Buffer in the pool with
 *                  corresponding to MIF Ctxt Id.
 ****************************************************************************/
static void *rrm_find_mif_context_from_pool (U8                    mif_ctxt_id, 
		rrm_mif_gb_context_t *p_gl_ctxt)
{
	void    *p_buf = RRM_PNULL;
	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
	p_buf = cvContextLocate(&(p_gl_ctxt->mif_ctxt_pool), mif_ctxt_id);

	RRM_UT_TRACE_EXIT();
	return (p_buf);
}

/****************************************************************************
 * Function Name  : mif_start_timer
 * Inputs         : 
 *      1.timerType     : Timer Type that to be started.
 *      2.p_data        : Pointer to the data to be saved with timer and
 *                        it's not null pointer.
 *      3.size          : size of data.
 *      4.p_gl_ctxt : Pointer to global context & it's not null pointer.
 * Outputs        : 
 * Returns        : 
 * Description    : This function starts the timer.
 ****************************************************************************/
QTIMER mif_start_timer(mif_timer_type timerType, void *p_data, U16 size,
		rrm_mif_gb_context_t *p_gl_ctxt)
{
	QTIME   time;
	void    *p_timer_buf = RRM_PNULL;
	void    *p_timerId = RRM_PNULL;
	U32     duration = RRM_ZERO;

	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != p_data);
	RRM_ASSERT (RRM_PNULL != p_gl_ctxt);

	p_timer_buf  = rrm_mem_get(size);

	/*raising error event - TIMER_START_FAIL*/
	if (RRM_PNULL == p_timer_buf)
	{
		RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_BRIEF,
				"Memory allocation to p_timer_buf failed" );
		rrm_raise_error_event_notification(RRM_EE_TIMER_START_FAIL);
		return RRM_ZERO;
	}
	else
	{

		RRM_MEMCPY(p_timer_buf, p_data, size);
		RRM_ASSERT (timerType < MIF_MAX_NUM_TIMER_TYPE);  
		duration = p_gl_ctxt->timer_val[timerType];
		/* SPR 16765 START */
		if (duration <= RRM_ZERO)
		{
			RRM_TRACE(g_cellm_log_on_off, p_rrm_cellm_facility_name, RRM_WARNING,
					"Invalid value[%d] for timer received",duration );
			return(p_timerId);
		}
		/* SPR 16765 END */

		RRM_ASSERT(duration != RRM_ZERO);

		time.s  = duration;
		time.us = RRM_ZERO;
		p_timerId   = qvTimerStart(&time, p_timer_buf, RRM_ZERO);

		/*raising error event - TIMER_START_FAIL*/
		if (RRM_PNULL == p_timerId)
		{
			rrm_raise_error_event_notification(RRM_EE_TIMER_START_FAIL);  
			/*Cov_fix_63799*/
			if(PNULL != p_timer_buf)
				RRM_MEM_FREE(p_timer_buf);
			/*Cov_fix_63799*/
		}
		else
		{

			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "%s: Started Timer: Timer Type [%d]: "
					"Timer Id [%p]: "
					"Duration: [%d]sec", "MIF:", 
					timerType, p_timerId, duration);
		}
	}

	RRM_UT_TRACE_EXIT();
	return(p_timerId);
}

/****************************************************************************
 * Function Name  : mif_stop_timer
 * Inputs         : 
 *      1.p_timerId : Pointer to Timer Identifier.
 * Outputs        : 
 * Returns        : 
 * Description    : This function stops the running timer.p_timerId is expected 
 *                  to be NULL as well as NON NULL, so no NULL check is done 
 *                  on it.
 ****************************************************************************/
void mif_stop_timer (QTIMER    p_timerId)
{
	void    *p_timerData = RRM_PNULL;

	RRM_UT_TRACE_ENTER();

	if (RRM_PNULL != p_timerId)
	{
		p_timerData = mif_get_timer_data(p_timerId);

		if(qvTimerRunning(p_timerId))
		{
			if(p_timerData != RRM_PNULL)
			{
				RRM_MEM_FREE(p_timerData);
			}
			qvTimerStop(p_timerId, RRM_PNULL);
		}
		/*raising error event - TIMER_STOP_FAIL*/
		else
		{
			rrm_raise_error_event_notification(RRM_EE_TIMER_STOP_FAIL);
		}
	}
	/*raising error event - TIMER_STOP_FAIL*/
	else
	{
		rrm_raise_error_event_notification(RRM_EE_TIMER_STOP_FAIL);  
	} 

	RRM_UT_TRACE_EXIT();
}

/****************************************************************************
 * Function Name  : mif_get_timer_data
 * Inputs         :
 *      1.  p_timerId : pointer to timer identifier.
 * Outputs        :
 * Returns        :
 * Description    : This function is to get the store data associated with
 *                  the timer.
 ****************************************************************************/
void* mif_get_timer_data(QTIMER p_timerId)
{
	void    *p_timerData = RRM_PNULL;

	RRM_UT_TRACE_ENTER();

	p_timerData = qvTimerData(p_timerId);

	/*raising error event - TIMER_STOP_FAIL*/
	if (RRM_PNULL == p_timerData)
	{
		rrm_raise_error_event_notification(RRM_EE_TIMER_STOP_FAIL);
	}
	RRM_ASSERT (RRM_PNULL != p_timerData);

	RRM_UT_TRACE_EXIT();

	return(p_timerData);
}

void rrm_dealloc_all_mif_context (rrm_mif_gb_context_t *p_gl_ctxt)
{
	U8 valid_ctxt = RRM_ZERO;
	rrm_mif_context *p_mif_context = RRM_PNULL;

	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != p_gl_ctxt);
	for(valid_ctxt = RRM_ZERO;valid_ctxt < MAX_NUM_MIF_CTXT;valid_ctxt++)
	{
		if(RRM_TRUE == p_gl_ctxt->map_info[valid_ctxt].is_info_valid)
		{
			p_mif_context = rrm_find_mif_context(p_gl_ctxt, p_gl_ctxt->map_info[valid_ctxt].mif_ctxt_id);
			if (p_mif_context != RRM_PNULL)
			{
				RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "DeAllocated the MIF Context [%d]", 
						p_mif_context->mif_ctxt_id);

				rrm_dealloc_mif_context(p_mif_context, p_gl_ctxt);
			}
		}
	}
	RRM_UT_TRACE_EXIT();
}     
/****************************************************************************
 * Function Name  : rrm_dealloc_mif_context
 * Inputs         : 
 *      1.p_mif_context : is not null pointer
 *      2.p_gl_ctxt : is not null pointer
 * Outputs        : 
 * Returns        : 
 * Description    : This function deallocates the MIF Context
 ****************************************************************************/
void rrm_dealloc_mif_context (rrm_mif_context *p_mif_context,
		rrm_mif_gb_context_t *p_gl_ctxt)
{
	U8 mifCtxtCount = RRM_ZERO;
	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != p_mif_context);
	RRM_ASSERT (RRM_PNULL != p_gl_ctxt);

	/*Free the memory allocated  for cell cfg node*/
	for(mifCtxtCount = RRM_ZERO; mifCtxtCount < MAX_NUM_MIF_CTXT; mifCtxtCount++)
	{
		if ((RRM_TRUE == p_gl_ctxt->map_info[mifCtxtCount].is_info_valid) &&
				(p_gl_ctxt->map_info[mifCtxtCount].mif_ctxt_id == p_mif_context->mif_ctxt_id))
		{
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "DeAllocated the MIF Context Id [%d], Idx[%d]", 
					p_mif_context->mif_ctxt_id, mifCtxtCount);
            /* SPR 20653 Fix Start */
            /* Code Removed */
            /* SPR 20653 Fix End */
			p_gl_ctxt->map_info[mifCtxtCount].is_info_valid = RRM_FALSE;
		}
	}

	rrm_dealloc_mif_context_from_pool(p_mif_context);

	RRM_UT_TRACE_EXIT();
}     


/****************************************************************************
 * Function Name  :rrm_dealloc_mif_context_from_pool 
 * Inputs         : 
 *      1.p_mif_context is not null pointer.
 * Outputs        : 
 * Returns        : void
 * Description    : This function deallocates the MIF Context from the pool.
 ****************************************************************************/
void rrm_dealloc_mif_context_from_pool(rrm_mif_context *p_mif_context)
{
	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (RRM_PNULL != p_mif_context);
	cvContextFree (p_mif_context);

	RRM_UT_TRACE_ENTER();
}

/****************************************************************************
 * Function Name  : compare_global_cell_id 
 * Inputs         :
 *                : rrm_oam_eutran_global_cell_id_t  p_recvd
 *                : rrm_oam_eutran_global_cell_id_t  p_stored
 * Outputs        :
 * Returns        : SUCCESS/FAILURE
 * Description    : This function compares the received and stored global cell
 *                  id
 ****************************************************************************/
rrm_return_et compare_global_cell_id(
		rrm_oam_eutran_global_cell_id_t     *p_rcvd,
		rrm_oam_eutran_global_cell_id_t     *p_strd)
{
	rrm_return_et ret_val = RRM_SUCCESS;
	U8 index = RRM_ZERO;

	RRM_UT_TRACE_ENTER();

	RRM_ASSERT(RRM_PNULL != p_rcvd);
	RRM_ASSERT(RRM_PNULL != p_strd);

	/* Comparing MCC Id */
	for (index=RRM_ZERO; index< MAX_MCC_DIGITS; index++)
	{
		if (p_rcvd->primary_plmn_id.mcc[index] != 
				p_strd->primary_plmn_id.mcc[index])
		{
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "stored MCC[%d]  Recvd MCC[%d] "
					"at INDEX[%d]",
					p_strd->primary_plmn_id.mcc[index], 
					p_rcvd->primary_plmn_id.mcc[index], 
					index);
			ret_val = RRM_FAILURE;
			break;
		}
	} 

	if (ret_val != RRM_FAILURE)
	{ 
		/**Comparing MNC Id **/ 
		if (p_rcvd->primary_plmn_id.num_mnc_digit == 
				p_strd->primary_plmn_id.num_mnc_digit)
		{ 
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "No of MNC matches");
			for (index=RRM_ZERO; index< p_rcvd->primary_plmn_id.num_mnc_digit && index < MAX_MNC_DIGITS; index++)
			{
				if (p_rcvd->primary_plmn_id.mnc[index] != 
						p_strd->primary_plmn_id.mnc[index])
				{
					RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "stored MNC[%d] "
							"Recvd MNC[%d]  at INDEX [%d]", 
							p_strd->primary_plmn_id.mnc[index], 
							p_rcvd->primary_plmn_id.mnc[index], index);
					ret_val = RRM_FAILURE;
					break;
				}
			}/*for loop ends*/
		}
		else
		{
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "stored num_mnc_digit[%d] "
					"Recvd num_mnc_digit[%d]", 
					p_strd->primary_plmn_id.num_mnc_digit, 
					p_rcvd->primary_plmn_id.num_mnc_digit); 
			ret_val = RRM_FAILURE; 
		}
	}


	if (ret_val != RRM_FAILURE)
	{

		/* Comparing Cell Identity */
		RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Comparing Cell Ident");
		for (index=RRM_ZERO; index < MAX_CELL_IDENTITY_OCTETS; index++)
		{
			if (p_strd->cell_identity[index] != p_rcvd->cell_identity[index])
			{
				RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, "stored CELL_ID[%d] "
						"Recvd CELL_ID[%d] at INDEX[%d]", 
						p_strd->cell_identity[index],  
						p_rcvd->cell_identity[index], index);
				ret_val = RRM_FAILURE;
				break;
			}
		}/*for loop ends*/
	}

	RRM_UT_TRACE_EXIT();
	return ret_val;
}

/****************************************************************************
 * Function Name  : allocate_and_store_tcb_info_in_mif_context 
 * Inputs         :
 *                : rrm_mif_context  p_mif_context
 *                : U16  trans id
 *                : U16  src id
 * Outputs        :
 * Returns        : SUCCESS/FAILURE
 * Description    : This function allocates the transaction control block 
 ****************************************************************************/

rrm_return_et allocate_and_store_tcb_info_in_mif_context(
		rrm_mif_context    *p_mif_context,
        /* SPR_19279_START */
        void               *ptr, 
        U16                size,
        /* SPR_19279_END */
		U16                 rcvd_trans_id,
		U16                 rcvd_src_id)
{

	rrm_return_et     ret_val = RRM_SUCCESS;
	mif_tcb           *p_current = RRM_PNULL;
	mif_tcb           *p_temp = RRM_PNULL;
	/* SPR_19279_START */
        /* SPR 19620: CID 109456 Fix Start */
        rrm_void_t  *ptr_data = RRM_PNULL;
        /* SPR 19620: CID 109456 Fix End */
	/* SPR_19279_END */

	RRM_UT_TRACE_ENTER();

	RRM_ASSERT (p_mif_context != RRM_PNULL);

	if (RRM_PNULL == p_mif_context->p_tcb)
	{
		p_mif_context->p_tcb = rrm_mem_get(sizeof(mif_tcb));
		if (RRM_PNULL == p_mif_context->p_tcb)
		{
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
					"Memory allocation failed for TCB");
	        /* SPR_19279_START */
	        return RRM_FAILURE;
	        /* SPR_19279_END */
		}
		else
		{
			RRM_MEMSET(p_mif_context->p_tcb, RRM_ZERO, sizeof(mif_tcb)); /* Coverity: CID 30921 */
			p_mif_context->p_tcb->trans_id = rcvd_trans_id;
			p_mif_context->p_tcb->api_first_source = rcvd_src_id;
          /*SPR 15993 Fix Start*/
          p_mif_context->p_tcb->egress_trans_id  = g_transaction_id++ ;
          /*SPR 15993 Fix Start*/
          p_mif_context->currently_processed_api_trans_d= p_mif_context->p_tcb->egress_trans_id  ;
          /*SPR 15993 Fix End*/
          /*SPR 15993 Fix End*/
			p_mif_context->p_tcb->p_next = RRM_PNULL;
          /*SPR 15993 Fix Start*/
          /*SPR 15993 Fix Start*/
          RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, 
                  "TCB Allocated: Recv TransId at MIF (%d), Source(%d) OutgoingTransId from MIF (%d)",
          /*SPR 15993 Fix Start*/
                  rcvd_trans_id, rcvd_src_id,p_mif_context->currently_processed_api_trans_d);
          /*SPR 15993 Fix End*/
          /*SPR 15993 Fix End*/
          /* SPR 21251 +- */
			/* SPR_19279_START */
			if( (RRM_NULL != ptr) && (size) )
			{    
                ptr_data = rrm_mem_get(size);
                if (RRM_PNULL == ptr_data)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                            "Memory allocation failed for mif_tcb->p_data");
                    RRM_MEM_FREE(p_mif_context->p_tcb);
                    return RRM_FAILURE;
                }

        			/* SPR 19620: CID 109456 Fix Start */
				RRM_MEMCPY(ptr_data, ptr, size);
				p_mif_context->p_tcb->p_data = ptr_data;
        			/* SPR 19620: CID 109456 Fix End */
			} 
            /* SPR_19279_END */
          /*SPR 15993 Fix Start*/
        /* Code Deleted */
          /*SPR 15993 Fix End*/
		}
	}
	else
	{
		p_temp = p_current = p_mif_context->p_tcb;
		while(RRM_PNULL != p_temp)
		{
			p_current = p_temp;
			p_temp = p_temp->p_next;
		}
		p_current->p_next = rrm_mem_get(sizeof(mif_tcb));
		if (RRM_PNULL == p_current->p_next)
		{
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
					"Memory allocation failed for New TCB");
	        /* SPR_19279_START */
			return RRM_FAILURE;
	        /* SPR_19279_END */
		}
		else
		{
			RRM_MEMSET(p_current->p_next, RRM_ZERO, sizeof(mif_tcb)); /* Coverity: CID 30921 */
			p_current->p_next->trans_id = rcvd_trans_id;
			p_current->p_next->api_first_source = rcvd_src_id;
          /*SPR 15993 Fix Start*/
          p_current->p_next->egress_trans_id = g_transaction_id++;
          /*SPR 15993 Fix Start*/
          p_mif_context->currently_processed_api_trans_d = p_current->p_next->egress_trans_id;
          /*SPR 15993 Fix End*/
          /*SPR 15993 Fix End*/
			p_current->p_next->p_next = RRM_PNULL;
          /*SPR 15993 Fix Start*/
          RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_BRIEF, 
                  "TCB(Next): Recv TransId at MIF(%d), Source(%d) OutGoing TransId from MIF (%d)",
                  rcvd_trans_id, rcvd_src_id,p_current->p_next->egress_trans_id);
          /*SPR 15993 Fix End*/
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
					"TCB(Next): TransId(%d), Source(%d)", rcvd_trans_id, rcvd_src_id);
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
					"DEBUG: Memory allocated success for TCB(Next) = %p",
					p_current->p_next);
			/* SPR_19279_START */
			if( (RRM_NULL != ptr) && (size) )
			{    
                /* SPR 19620: CID 109456 Fix Start */
                ptr_data = rrm_mem_get(size);
                /* SPR 19620: CID 109456 Fix End */
                if (RRM_PNULL == ptr_data)
                {
                    RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
                            "Memory allocation failed for mif_tcb->p_data");
                    RRM_MEM_FREE(p_current->p_next);
                    return RRM_FAILURE;
                }

        			/* SPR 19620: CID 109456 Fix Start */
				RRM_MEMCPY(ptr_data, ptr, size);
				p_current->p_next->p_data = ptr_data;   
        			/* SPR 19620: CID 109456 Fix End */
			} 
            /* SPR_19279_END */
		}
          /*SPR 15993 Fix Start*/
        /* Code Deleted */
          /*SPR 15993 Fix End*/
	}
	RRM_UT_TRACE_EXIT();
	return ret_val;
}

/****************************************************************************
 * Function Name  : validate_trans_and_src_id_rcvd_in_resp
 * Inputs         :
 *                : rrm_mif_context  *p_mif_context
 *                : U16              rcvd_trans_id 
 * Outputs        :
 * Returns        : SUCCESS/FAILURE
 * Description    : This function validates the incoming trans Id
 ****************************************************************************/
rrm_return_et validate_trans_id_rcvd_in_resp (
		rrm_mif_context    *p_mif_context,
		U16                 rcvd_trans_id)
{
	rrm_return_et      ret_val = RRM_SUCCESS;
	mif_tcb           *p_temp = RRM_PNULL;

	RRM_UT_TRACE_ENTER();

	RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Validating TransId(%d) Rcvd in TCB", rcvd_trans_id);
	if (RRM_PNULL == p_mif_context->p_tcb)
	{
		RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Incorrect Data TransId(%d) Rcvd", rcvd_trans_id);
		/* BUG_FIX_12597_START */
		ret_val = RRM_SUCCESS;
		/* BUG_FIX_12597_END */
	}
	else
	{
		p_temp = p_mif_context->p_tcb;
		while(RRM_PNULL != p_temp)
		{
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Rcvd TransId(%d), Stored TransId(%d) in TCB", 
                    /*SPR 15993 Fix Start*/
                    rcvd_trans_id, p_temp->egress_trans_id);
            if (p_temp->egress_trans_id == rcvd_trans_id)
                /*SPR 15993 Fix End*/
			{
				ret_val = RRM_SUCCESS;
				RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Matched DataTransId(%d) Rcvd", rcvd_trans_id);
				break;
			}
			p_temp = p_temp->p_next;
		}
	}

	RRM_UT_TRACE_EXIT();
	return ret_val;
}
U16 fetch_fail_cause(
		rrm_mif_gb_context_t *p_gl_ctxt,
		U16                 rcvd_trans_id)
{
	U16 fail_cause = RRM_ZERO; 
	mif_tcb           *p_temp = RRM_PNULL;
	rrm_mif_context *p_mif_context = RRM_PNULL;
	U8 valid_ctxt = RRM_ZERO;
	RRM_UT_TRACE_ENTER();

	RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Validating TransId(%d) Rcvd in TCB", rcvd_trans_id);
	RRM_ASSERT(p_gl_ctxt != RRM_PNULL);
	for(valid_ctxt = RRM_ZERO;valid_ctxt < MAX_NUM_MIF_CTXT;valid_ctxt++)
	{
		if(RRM_TRUE == p_gl_ctxt->map_info[valid_ctxt].is_info_valid)
		{
			p_mif_context = rrm_find_mif_context(p_gl_ctxt, p_gl_ctxt->map_info[valid_ctxt].mif_ctxt_id);
			if (p_mif_context != RRM_PNULL)
			{
				p_temp = p_mif_context->p_tcb;
				while(RRM_PNULL != p_temp)
				{
					RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
							"Rcvd TransId(%d), Stored TransId(%d) in TCB", 
/*SPR 15993 Fix Start*/
                            rcvd_trans_id, p_temp->egress_trans_id);
                    if (p_temp->egress_trans_id == rcvd_trans_id)
/*SPR 15993 Fix End*/
					{
						fail_cause = p_temp->fail_cause;
						RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
								"Fail cause(%d)", fail_cause);
						if(fail_cause != RRM_NO_ERROR)
						{
							break;
						}
					}
					p_temp = p_temp->p_next;
				}
			}
		}
	}

	RRM_UT_TRACE_EXIT();
	return fail_cause;

}
rrm_return_et store_fail_cause_req_send_rcvd (
		U16 fail_cause,
		rrm_mif_context    *p_mif_context,
		U16                 rcvd_trans_id)
{
	rrm_return_et      ret_val = RRM_FAILURE;
	mif_tcb           *p_temp = RRM_PNULL;

	RRM_UT_TRACE_ENTER();

	RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Validating TransId(%d) Rcvd in TCB", rcvd_trans_id);
	if (RRM_PNULL == p_mif_context->p_tcb)
	{
		RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, "Incorrect Data TransId(%d) Rcvd", rcvd_trans_id);
	}
	else
	{
		p_temp = p_mif_context->p_tcb;
		while(RRM_PNULL != p_temp)
		{
/*SPR 15993 Fix Start*/
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Rcvd TransId(%d), Stored TransId(%d) in TCB", 
                    rcvd_trans_id, p_temp->egress_trans_id);
            if (p_temp->egress_trans_id == rcvd_trans_id)
/*SPR 15993 Fix End*/
			{
				p_temp->fail_cause = fail_cause;
				RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Stored fail cause :%d", 
						p_temp->fail_cause);
				/* Coverity ID 74370 Fix Start */
				ret_val = RRM_SUCCESS;
				/* Coverity ID 74370 Fix End */
				break;
			}
			p_temp = p_temp->p_next;
		}
		if (RRM_SUCCESS == ret_val)
		{
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Matched DataTransId(%d) Rcvd", rcvd_trans_id);
		}
	}

	RRM_UT_TRACE_EXIT();
	return ret_val;
}
rrm_return_et de_allocate_all_tcb_from_mif_ctxt(rrm_mif_gb_context_t *p_gl_ctxt,
		U16 recv_trans_id)
{
	U8 valid_ctxt = RRM_ZERO;
	rrm_return_et ret_val = RRM_SUCCESS;
	rrm_mif_context *p_mif_context = RRM_PNULL;

	RRM_UT_TRACE_ENTER();

	for(valid_ctxt = RRM_ZERO;valid_ctxt<MAX_NUM_MIF_CTXT;valid_ctxt++)
	{
		if (RRM_TRUE == p_gl_ctxt->map_info[valid_ctxt].is_info_valid)
		{
			p_mif_context = rrm_find_mif_context(p_gl_ctxt, p_gl_ctxt->map_info[valid_ctxt].mif_ctxt_id);
			if (p_mif_context != RRM_PNULL)
			{
/*SPR 15993 Fix Start*/
					ret_val = de_allocate_tcb_from_mif_ctxt(p_mif_context,recv_trans_id); 
/*SPR 15993 Fix End*/
			}
		}

	}
	RRM_UT_TRACE_EXIT();
	return ret_val;
}

/****************************************************************************
 * Function Name  : de_allocate_tcb_from_mif_ctxt
 * Inputs         :
 *                  -  p_mif_context
 *                  -  trans  Id
 * Outputs        :
 *
 * Returns        :  SUCCESS/FAILURE
 *
 * Variables      :
 * Description    : This function de allocated the TCB 
 ****************************************************************************/
rrm_return_et de_allocate_tcb_from_mif_ctxt(
		rrm_mif_context    *p_mif_context,
		U16                 rcvd_trans_id)
{
	mif_tcb           *p_temp = RRM_PNULL;
	mif_tcb           *p_current = RRM_PNULL;
	rrm_return_et      ret_val = RRM_FAILURE;

	RRM_UT_TRACE_ENTER();

	RRM_ASSERT(p_mif_context != RRM_PNULL); 

	if (RRM_PNULL == p_mif_context->p_tcb)
	{
		RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
				"Incorrect Data TransId(%d) Rcvd", rcvd_trans_id);
		ret_val = RRM_FAILURE;
	}
	else
	{
		p_current = p_temp = p_mif_context->p_tcb;
/*SPR 15993 Fix Start*/
/*SPR 15993 Fix Start*/
      if ( p_current->egress_trans_id == rcvd_trans_id )
/*SPR 15993 Fix End*/
/*SPR 15993 Fix End*/
		{
/*SPR 15993 Fix Start*/
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                 "TCB De-allocated for  TransId (%d) at MIF (OutgoingTransId from MIF was %d) at MIF from Source(%d)",
                  p_current->trans_id, p_current->egress_trans_id,p_current->api_first_source);
/*SPR 15993 Fix End*/
			p_mif_context->p_tcb = p_current->p_next;
			/* SPR 21251 +- */
            RRM_MEM_FREE(p_current);
			RRM_UT_TRACE_EXIT();
			return RRM_SUCCESS;
		}
		else
		{
			while(RRM_PNULL != p_temp)
			{
/*SPR 15993 Fix Start*/
/*SPR 15993 Fix Start*/
		      if (p_temp->egress_trans_id  == rcvd_trans_id)
/*SPR 15993 Fix End*/
/*SPR 15993 Fix End*/
				{
					ret_val = RRM_SUCCESS;
					break;
				}
				p_current = p_temp;
				p_temp = p_temp->p_next;
			}
		}
		if (RRM_SUCCESS == ret_val)
		{
			p_current->p_next = p_temp->p_next;
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
					"Matched Data With Rcvd TransId(%d) & Stored Trans Id(%d)", 
					rcvd_trans_id, p_temp->trans_id);
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
					"DEBUG: Memory de-allocated success for TCB(Next) = %p",
					p_current->p_next);
			RRM_MEM_FREE(p_temp);
		}
	}

	RRM_UT_TRACE_EXIT();
	return ret_val;
}
/****************************************************************************
 * Function Name  : return_src_id_from_tcb
 * Inputs         :
 *                : rrm_mif_context  p_mif_context
 *                : U16  rcvd_trans_id
 * Outputs        :
 * Returns        : Source Id
 * Description    : This function return the source Id
 ****************************************************************************/
U16 return_src_id_from_tcb(
		rrm_mif_context    *p_mif_context,
		U16                 rcvd_trans_id)
{
	U16                orig_src_id = RRM_ZERO;
	mif_tcb           *p_temp = RRM_PNULL;
	mif_tcb           *p_current = RRM_PNULL;
	rrm_return_et      ret_val = RRM_FAILURE;

	RRM_UT_TRACE_ENTER();

	if (RRM_PNULL == p_mif_context->p_tcb)
	{
		RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_ERROR, 
				"Incorrect Data TransId(%d) Rcvd", rcvd_trans_id);
	}
	else
	{
		p_current = p_temp = p_mif_context->p_tcb;
        /*SPR 15993 Fix Start*/
        if (p_current->egress_trans_id == rcvd_trans_id)
            /*SPR 15993 Fix End*/
		{
			orig_src_id = p_current->api_first_source;

			/* CSR_00070511 fix start*/
            /*SPR 15993 Fix Start*/
            p_mif_context->currently_processed_api_trans_d = p_current->trans_id;
            /*SPR 15993 Fix Start*/
            /* Code Deleted */
            /*SPR 15993 Fix End*/

			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                    "Matched Data: TransId Rcvd at MIF(%d) & Src Id(%d) currently_processed_api_trans_d (%d)", 
					rcvd_trans_id, orig_src_id, p_mif_context->currently_processed_api_trans_d );
            /*SPR 15993 Fix End*/
			/* CSR_00070511 fix end */

			RRM_UT_TRACE_EXIT();
			return orig_src_id;
		}
		else
		{
			while(RRM_PNULL != p_temp)
			{
                /*SPR 15993 Fix Start*/
				RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED,
						"Matching Stored TransId(%d) & Rcvd Src Id(%d)", 
                        p_temp->egress_trans_id, rcvd_trans_id);
                if (p_temp->egress_trans_id == rcvd_trans_id)
                    /*SPR 15993 Fix End*/
				{
					/* CSR_00070511 fix start*/
                    /*SPR 15993 Fix Start*/
                    p_mif_context->currently_processed_api_trans_d = p_temp->trans_id;
                    /*SPR 15993 Fix Start*/
                    /* Code Deleted */
                    /*SPR 15993 Fix End*/
                    /*SPR 15993 Fix End*/
					/* CSR_00070511 fix end */
					orig_src_id = p_temp->api_first_source;
					ret_val = RRM_SUCCESS;
					break;
				}
                /* Coverity CID 75517 Fix Start */
                /* Code Deleted */
                /* Coverity CID 75517 Fix End */
				p_temp = p_temp->p_next;
			}
		}
		if (RRM_SUCCESS == ret_val)
		{
            /*SPR 15993 Fix Start*/
			/* CSR_00070511 fix start*/
			RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, 
                    "Matched Data TransId Rcvd at MIF (%d) & Src Id(%d) currently_processed_api_trans_d =%d ", 
					rcvd_trans_id, orig_src_id,p_mif_context->currently_processed_api_trans_d );
            /*SPR 15993 Fix End*/
			/* CSR_00070511 fix end */
		}
	}

	RRM_UT_TRACE_EXIT();
	return orig_src_id;
}


/****************************************************************************
 * Function Name  :  fetch_mif_ctxt_id_from_global_ctxt
 * Inputs         :
 *                  -  p_gl_ctxt
 *                  -  p_global_cell_id
 *                  -  p_mif_ctxt_cnt
 *                  -  p_mif_ctxt 
 * Outputs        :
 * Returns        : SUCCESS/FAILURE
 * Description    : This function fetches the MIF Context Id
 ****************************************************************************/
rrm_return_et fetch_mif_ctxt_id_from_global_ctxt
(
 rrm_mif_gb_context_t     *p_gl_ctxt,
 rrm_oam_eutran_global_cell_id_t *p_global_cell_id,
 U8                       *p_mif_ctxt_cnt,
 U8                       *p_mif_ctxt
 )
{
	rrm_return_et      ret_val = RRM_FAILURE;
	U8                 mif_ctxt_cnt = RRM_ZERO;


	RRM_UT_TRACE_ENTER();

	for (mif_ctxt_cnt = RRM_ZERO; mif_ctxt_cnt < MAX_NUM_MIF_CTXT; mif_ctxt_cnt++)
	{
		if (RRM_TRUE == p_gl_ctxt->map_info[mif_ctxt_cnt].is_info_valid)
		{
			ret_val = compare_global_cell_id(p_global_cell_id,
                &(p_gl_ctxt->map_info[mif_ctxt_cnt].
						global_cell_id));
			if (RRM_SUCCESS == ret_val)
			{
				*p_mif_ctxt = p_gl_ctxt->map_info[mif_ctxt_cnt].mif_ctxt_id;
				*p_mif_ctxt_cnt = mif_ctxt_cnt;
				RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Mif Context %d matched with stored Info",
						*p_mif_ctxt);
				ret_val = RRM_SUCCESS;
				break;
			}
		}
	}

	RRM_UT_TRACE_EXIT();
	return ret_val;
}


/****************************************************************************
 * Function Name  : fetch_mif_ctxt_id_based_on_cell_id 
 * Inputs         :
 *                  -  p_gl_ctxt
 *                  -  p_global_cell_id
 *                  -  p_mif_ctxt_cnt
 *                  -  p_mif_ctxt 
 * Outputs        :
 *
 * Returns        :  SUCCESS/FAILURE
 *
 * Variables      :
 * Description    : This function fetches the MIF Context Id
 ****************************************************************************/
rrm_return_et fetch_mif_ctxt_id_based_on_cell_id
(
 rrm_mif_gb_context_t *p_gl_ctxt,
 U8                    cell_index,
 U8                   *p_mif_ctxt_cnt,
 U8                   *p_mif_ctxt
 )
{
	rrm_return_et      ret_val = RRM_FAILURE;
	U8                 mif_ctxt_cnt = RRM_ZERO;


	RRM_UT_TRACE_ENTER();

	for (mif_ctxt_cnt = RRM_ZERO; mif_ctxt_cnt < MAX_NUM_MIF_CTXT; mif_ctxt_cnt++)
	{
		if (RRM_TRUE == p_gl_ctxt->map_info[mif_ctxt_cnt].is_info_valid)
		{
			if (p_gl_ctxt->map_info[mif_ctxt_cnt].cell_index == cell_index)
			{
				*p_mif_ctxt = p_gl_ctxt->map_info[mif_ctxt_cnt].mif_ctxt_id;
				*p_mif_ctxt_cnt = mif_ctxt_cnt;
				RRM_TRACE(g_mif_log_on_off, p_rrm_mif_facility_name, RRM_DETAILED, "Mif Context %d matched with stored Info", 
						*p_mif_ctxt);
				ret_val = RRM_SUCCESS;
				break;
			}
		}
	}

	RRM_UT_TRACE_EXIT();
	return ret_val;
}

/****************************************************************************
 * Function Name  : copy_global_cell_id_from_ctxt 
 * Inputs         :
 *                  -  p_gl_ctxt
 *                  -  p_global_cell_id
 *                  -  p_mif_ctxt_cnt
 *                  -  p_mif_ctxt 
 * Outputs        :
 *
 * Returns        :  SUCCESS/FAILURE
 *
 * Variables      :
 * Description    : This function fetches the MIF Context Id
 ****************************************************************************/
rrm_return_et copy_global_cell_id_from_ctxt(rrm_mif_gb_context_t *p_gl_ctxt,
		rrm_oam_eutran_global_cell_id_t
		*p_global_cell_id,
		U8        cell_index)
{
	rrm_return_et      ret_val = RRM_FAILURE;
	U8                 mif_ctxt_cnt = RRM_ZERO;


	RRM_UT_TRACE_ENTER();

	for (mif_ctxt_cnt = RRM_ZERO; mif_ctxt_cnt < MAX_NUM_MIF_CTXT; mif_ctxt_cnt++)
	{
		if (RRM_TRUE == p_gl_ctxt->map_info[mif_ctxt_cnt].is_info_valid)
		{
			if (p_gl_ctxt->map_info[mif_ctxt_cnt].cell_index == cell_index)
			{
				//RRM_MEMCPY(p_global_cell_id,
				//          &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id),
				//           sizeof(rrm_oam_eutran_global_cell_id_t));
				rrm_memcpy_rrm_oam_eutran_global_cell_id(p_global_cell_id, &(p_gl_ctxt->map_info[mif_ctxt_cnt].global_cell_id));
				ret_val = RRM_SUCCESS;
				break;
			}
		}
	}

	RRM_UT_TRACE_EXIT();
	return ret_val;
}
