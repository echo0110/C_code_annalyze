/****************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * $Id: son_nmm.c $
 *
 ****************************************************************************
 *
 * File Description: contains initialization and message processing function for
 *                   NMM module.
 *
 ****************************************************************************
 * Revision Details
 * ----------------
 *   DATE            AUTHOR      REFERENCE       REASON
 *   July, 2012      Sandip Samal                Initial code
 *   Aug, 2014       Shilpi                      SPR 13251 Fix
 ****************************************************************************/

/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_nmm.h>

/****************************************************************************
 *  Private Definitions
 ****************************************************************************/

/****************************************************************************
 *                              global variables
 *****************************************************************************/

/****************************************************************************
 * Exported variables
 ****************************************************************************/

/***************************************************************************
 * Private Types
 ****************************************************************************/

/****************************************************************************
 * Macro definitions
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/****************************************************************************
 *                      Functions forward declarations
 *****************************************************************************/

static void *
son_nmm_init
(
 void *p_son_nmm_init_data
 );

static S32
son_nmm_process_msg
(
 void *p_api_buf,
 void *p_gl_ctx
 );

static void
son_nmm_process_timer_msg
(
 son_timer_t timer_id,
 void        *p_timer_buf,
 void        *p_gl_ctx
 );

static void
son_nmm_init_gl_ctx
(
 nmm_gl_ctx_t    *p_nmm_gl_ctx
 );

static void
nmmctl_init_ctx
(
 nmm_gl_ctx_t    *p_nmm_gl_ctx
 );

/****************************************************************************
 *                      Variable initialisation
 ****************************************************************************/
U8 g_nmm_log_enable = SON_NULL;
/* SPR-13251 Fix Starts */
U8 g_nmm_log_level = SON_OAM_LOG_LEVEL_BRIEF;
/* SPR-13251 Fix Ends */
const son_8    *p_son_nmm_facility_name = "SON_NMM";

static const son_ul32 SonNmmDestinationList[] =
{
	SON_MIF_MODULE_ID,
	SON_ANR_MODULE_ID,
	SON_APPS_MODULE_ID,
	SON_NMM_MODULE_ID,
	SON_RRM_MODULE_ID,
	SON_PHY_MODULE_ID
};

const QMANIFEST son_manifest_nmm =
{
	"SON_NMM_MODULE_ID",
	SON_NMM_MODULE_ID,

	{ 0, son_nmm_init },
	{ son_nmm_process_msg, son_nmm_process_timer_msg },
	{0, 0, 0 },

	SonNmmDestinationList
};

/****************************************************************************
 *                      Functions implementations
 *****************************************************************************/

/*****************************************************************************
 * Function Name  : son_nmm_init
 * Inputs         : p_son_nmm_init_data - Pointer to NMM stack initialization
 *                  data. Unused
 * Outputs        : None
 * Returns        : p_nmm_gl_ctx - Pointer to nmm global context
 *                  This will be passed in son_nmm_process_msg and in
 *                  son_nmm_process_timer_msg callbacks.
 * Description    : This function is CSPL callback. It initializes NMM entity.
 ****************************************************************************/
static void *
son_nmm_init
(
 /* Pointer to NMM stack initialization data. Unused */
 void *p_son_nmm_init_data
 )
{
    /*SPR 17777 +-*/
    (void)p_son_nmm_init_data;
	nmm_gl_ctx_t *p_nmm_gl_ctx = SON_PNULL;

	SON_UT_TRACE_ENTER();

	/* Init advance logger with default log file */
	SON_INIT_LOG(SON_PNULL);

	/*
	 ** Set the NMM module loging to SON_OAM_LOG_ON and log level to BRIEF till
	 ** the time logging and log level is not configured by OAM
	 */
	g_nmm_log_enable = SON_OAM_LOG_ON;
	SET_MODULE_LOG_LEVEL(SON_OAM_LOG_LEVEL_BRIEF);

	SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
			"Initializing NMM module");

	/* Allocate Memory for NMM global context data structure */
	p_nmm_gl_ctx = (nmm_gl_ctx_t *) son_mem_get(sizeof(nmm_gl_ctx_t));

	if (SON_PNULL == p_nmm_gl_ctx)
	{
		SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
				"son_nmm_init: unable to Allocate Memory for NMM global context!");
	}
	else
        {
            /* Initialize the memory allocated for global context data structure */
            son_nmm_init_gl_ctx(p_nmm_gl_ctx);

            /*
             ** Send INIT indication to the Management interface:
             ** 1) Fill the CSPL header for INIT indication.
             ** 3) Send the message.
             */


            /*
             ** Call the prepare and send message handler
             ** to send the init indication to SMIF.
             */
            son_create_send_buffer((U8 *)SON_PNULL,
                    SON_NMM_MODULE_ID,
                    SON_MIF_MODULE_ID,
                    SMIF_INIT_IND, 0);
        }

	SON_UT_TRACE_EXIT();

	return p_nmm_gl_ctx;
} /* son_nmm_init */

/*****************************************************************************
 * Function Name  : son_nmm_process_msg
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : RELEASE_SON_BUFFER - buffer released by the encapsulating shell
 * Description    : This is the main entry point of SON NMM module - CSPL callback.
 *                  It passes all the internal/external APIs received from internal
 *                  (SON modules i.e. SONAPPS/ANR)/external(RRM/PHY) modules to appropriate
 *                  message handlers based on incoming module ID and API ID.
 ****************************************************************************/
static S32
son_nmm_process_msg
(
 void *p_api_buf,    /* Pointer to input API buffer */
 void *p_gl_ctx       /* Pointer to the NMM global context data */
 )
{
	nmm_gl_ctx_t          *p_nmm_gl_ctx = SON_PNULL;
	son_procedure_code_et api_id        = SON_PROCEDURE_UNDEFINED;
	STACKAPIHDR cspl_hdr = {0};

	SON_UT_TRACE_ENTER();
	son_parse_cspl_header((U8 *)p_api_buf, &cspl_hdr);

	/* Check general message integrity */
	if ((SON_PNULL == p_api_buf) || (SON_PNULL == p_gl_ctx))
	{
		SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
				"son_nmm_process_msg: somebody call us in wrong context %p %p",
				p_api_buf, p_gl_ctx);
	}
	else
	{
		SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
				"API ID %u received from module %u",
				cspl_hdr.api, cspl_hdr.from);

		/* Convert generic global context data to the NMM global context type */
		p_nmm_gl_ctx = (nmm_gl_ctx_t *)p_gl_ctx;

		/* Get the source module of the received data buffer */

		/* Call the appropriate message handler as per the source module ID */
		switch (cspl_hdr.from)
		{
			/*
			 ** NMM controller is the destination for data received from
			 ** SON Management interface, SON Apps and RRM modules.
			 ** Call the NMM controller FSM process message handler.
			 */
			case SON_MIF_MODULE_ID:
			case SON_ANR_MODULE_ID:
			case SON_APPS_MODULE_ID:
			case SON_RRM_MODULE_ID:
				{
					SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
							"Call event handler for NMM Controller");
					son_nmmctl_fsm_process_msg(p_api_buf, p_nmm_gl_ctx);
					break;
				}

				/*
				 ** NMM (interacting with PHY(L1)) and NMM controller are the two sub
				 ** parts of NMM module and their module ID(SON_NMM_MODULE_ID) are same.
				 ** So the source module ID and destination module ID will be same for
				 ** all the message exchanged between NMM and NMMCTL.
				 ** To call the appropriate message handler, we need the API ID.
				 */
			case SON_NMM_MODULE_ID:
				{
					/* Get the API ID from the received message buffer */
					api_id = (son_procedure_code_et)cspl_hdr.api;

					if ((api_id == SONNMM_START_SCAN_RES) ||
							(api_id == SONNMM_STOP_SCAN_RES))
					{
						SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
								"son_nmm_process_msg: Call NMM Controller event handler");
						son_nmmctl_fsm_process_msg(p_api_buf, p_nmm_gl_ctx);
					}
					else
					{
						SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
								"son_nmm_process_msg: Call NMM event handler");
                        /*SPR 17777 +-*/
						son_nmm_fsm_process_msg(p_api_buf);
					}

					break;
				}

				/* NMM is the destination for all the data received from L1 */
			case SON_PHY_MODULE_ID:
				{
					SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
							"son_nmm_process_msg: Call NMM event handler");
                    /*SPR 17777 +-*/
					son_nmm_fsm_process_msg(p_api_buf);
					break;
				}

			default:
				{
					SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
							"son_nmm_process_msg: Unexpected API %u received from module %u",
							cspl_hdr.api, cspl_hdr.from);
					break;
				}
		} /* switch */
	}

	SON_UT_TRACE_EXIT();

	/* Encapsulating shell should release the buffer */
	return RELEASE_SON_BUFFER;
} /* son_nmm_process_msg */

/*****************************************************************************
 * Function Name  : son_nmm_process_timer_msg
 * Inputs         : p_api_buf    - Pointer to the received message buffer
 *                  p_nmm_gl_ctx - Pointer to NMM global context
 * Outputs        : None
 * Returns        : None
 * Description    : This is the main entry point of SON NMM module - CSPL callback.
 *                  It handles all the timer expiry events.
 *                  The timer buffer is always allocated in the son_start_timer() function.
 *                  This buffer should always start with nmm_timer_buf_t
 *                  Timer buffer can be freed depending on information in nmm_timer_buf_t.
 ****************************************************************************/
static void
son_nmm_process_timer_msg
(
 son_timer_t timer_id,     /* Identity of the expired timer */
 void         *p_timer_buf,  /* Pointer to data associated with the timer */
 void         *p_gl_ctx      /* Pointer to the NMM global context data */
 )
{
	nmm_gl_ctx_t    *p_nmm_gl_ctx     = SON_PNULL;
	nmm_timer_buf_t *p_nmm_timer_buf = SON_PNULL;

	SON_UT_TRACE_ENTER();

	/* Check general message integrity */
	if ((SON_PNULL == p_timer_buf) || (SON_PNULL == p_gl_ctx))
	{
		SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
				"son_nmm_process_timer_msg: somebody call us in wrong context %p %p",
				p_timer_buf, p_gl_ctx);
	}
	else
	{
		p_nmm_gl_ctx    = (nmm_gl_ctx_t *)p_gl_ctx;
		p_nmm_timer_buf = (nmm_timer_buf_t *)p_timer_buf;

		/*
		 ** Call the appropriate timeout message handle depending upon
		 ** the submodule ID, provided during the timer creation
		 */
		switch (p_nmm_timer_buf->target_submodule)
		{
			case SONNMM_SUBMODULE_NMM:
				{
					SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
							"son_nmm_process_timer_msg: Call NMM timer handler");
                    /*SPR 17777 +-*/
					son_nmm_time_out_handler(timer_id);
					break;
				}

			case SONNMM_SUBMODULE_NMMCTL:
				{
					SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
							"son_nmm_process_timer_msg: Call NMM Controller timer handler");
                      /*SPR 17777 +-*/
					son_nmmctl_time_out_handler(timer_id,
							p_nmm_gl_ctx);
					break;
				}

			default:
				{
					SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
							"son_nmm_process_timer_msg: timer event for "
							"unexpected submodule %d is received", p_nmm_timer_buf->target_submodule);
					break;
				}
		} /* switch */

		/* Free the timer if auto delete parameter is set during timer creation */
		if (SON_TRUE == p_nmm_timer_buf->auto_delete)
		{
			if (SON_PNULL != p_timer_buf)
			{
				son_mem_free(p_timer_buf);
			}
		}
	}

	SON_UT_TRACE_EXIT();

	return;
} /* son_nmm_process_timer_msg */

/*****************************************************************************
 * Function Name  : son_nmm_init_gl_ctx
 * Inputs         : p_nmm_gl_ctx - Pointer to the NMM global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function init NMM global context with default values.
 ****************************************************************************/
static void
son_nmm_init_gl_ctx
(
 nmm_gl_ctx_t    *p_nmm_gl_ctx
 )
{
	SON_UT_TRACE_ENTER();

	SON_MEMSET(p_nmm_gl_ctx, 0, sizeof(nmm_gl_ctx_t));

	/* Call the NMM controller specific intialization function */
	nmmctl_init_ctx(p_nmm_gl_ctx);

	SON_UT_TRACE_EXIT();

	return;
}

/*****************************************************************************
 * Function Name  : nmmctl_init_ctx
 * Inputs         : p_nmm_gl_ctx - Pointer to the NMM global context data
 * Outputs        : None
 * Returns        : None
 * Description    : This function init NMM controller specific global context.
 ****************************************************************************/
static void
nmmctl_init_ctx
(
 nmm_gl_ctx_t    *p_nmm_gl_ctx   /* Pointer to the NMM global context data */
 )
{
	nmmctl_ctx_t *p_nmmctl_ctx = SON_PNULL;

	SON_UT_TRACE_ENTER();

	/* Allocate Memory for NMM controller specific global data structure */
	p_nmmctl_ctx = (nmmctl_ctx_t *) son_mem_get(sizeof(nmmctl_ctx_t));

	if (SON_PNULL == p_nmmctl_ctx)
	{
		SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
				"nmmctl_init_ctx: Unable to Allocate Memory for NMM controller global context");
	}
	else
	{
		SON_MEMSET(p_nmmctl_ctx, 0, sizeof(nmmctl_ctx_t));

		/* Set NMM controller state */
		NMMCTL_FSM_SET_STATE(p_nmmctl_ctx, NMMCTL_STATE_INIT);
		SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_BRIEF,
				"NMM controller is in NMMCTL_STATE_INIT state");

		/* Initialize the scan configuration parameters */
		p_nmmctl_ctx->nmm_scan_config_param.force_scan         = SON_FALSE;
		p_nmmctl_ctx->nmm_scan_config_param.scan_time_interval = SCAN_TIME_INTERVAL;

		/* Initialize the NMM controller periodic scan timer data */
		p_nmmctl_ctx->nmmctl_periodic_scan_timer_buf.auto_delete      = SON_FALSE;
		p_nmmctl_ctx->nmmctl_periodic_scan_timer_buf.target_submodule = SONNMM_SUBMODULE_NMMCTL;

		/* Initialize the NMM controller scan response timer data */
		p_nmmctl_ctx->nmmctl_scan_resp_timer_buf.auto_delete      = SON_TRUE;
		p_nmmctl_ctx->nmmctl_scan_resp_timer_buf.target_submodule = SONNMM_SUBMODULE_NMMCTL;

		/* Initialize the scan queue list */
		SCANING_Q_INIT(&p_nmmctl_ctx->scaning_queue);

		/* Save the pointer of the NMM controller specific global context data */
		p_nmm_gl_ctx->p_nmmctl_ctx = p_nmmctl_ctx;
	}

	SON_UT_TRACE_EXIT();

	return;
} /* nmmctl_init_ctx */

/*****************************************************************************
 * Function Name  : son_nmm_fill_time_stamp
 * Inputs         : p_time_stamp - Pointer to the time stamp.
 * Outputs        : None
 * Returns        : None
 * Description    : This function fills the time stamp.
 ****************************************************************************/
void
son_nmm_fill_time_stamp
(
 son_nmm_time_stamp_t *p_time_stamp
 )
{
	SON_UT_TRACE_ENTER();

	time_t currentTimestamp;
	struct tm  *currentTime;

	son_time(&currentTimestamp);

	/*localtime function will fill the structure currentTime*/
	currentTime = son_localtime(&currentTimestamp);
	if (SON_PNULL != currentTime)
	{
		p_time_stamp->year  = (1900 + currentTime->tm_year);
		p_time_stamp->month = ++currentTime->tm_mon;
		p_time_stamp->day   = currentTime->tm_mday;
		p_time_stamp->hour  = currentTime->tm_hour;
		p_time_stamp->min   = currentTime->tm_min;
		p_time_stamp->sec   = currentTime->tm_sec;
	}
	else
	{
		SON_LOG(g_nmm_log_enable, p_son_nmm_facility_name, SON_ERROR,
				"son_nmm_fill_time_stamp: localtime function not working properly ");
	}

	SON_UT_TRACE_EXIT();
} /* son_nmm_fill_time_stamp */
