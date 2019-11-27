/******************************************************************************
*
*   FILE NAME:
*       rrc_entity.c
*
*   DESCRIPTION:
*       In this file RRC is composed from RRC CSPL modules
*
*   DATE            AUTHOR      REFERENCE       REASON
*   03 April 2009   VasylS      ---------       Initial
*
*   Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
******************************************************************************/
#define _GNU_SOURCE

#ifdef RRC_PROFILER
#include <signal.h>
extern void __gcov_flush(void);
#endif
#include <lteMisc.h>
#include <cspl.h>
#include <stacklayer.h>

#include "simulation_environment.h"

#include "rrc_common_utils.h"
#include "rrc_handlers.h"
#include "s1ap_handlers.h"
#include "rrc_msg_mgmt.h"
#include <sched.h>
#include "pthread.h"
#include "rrc_check.h"
#include "rrc_validation.h"
#include "rrc_logging.h"
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
#include <lteLogger.h>
#include <advlogging.h>
#endif

#include "rtxsrc/rtxMemory.h"

#include "x2ap_init.h"  /* L3-X2AP Integration Activity */ 

#ifdef LTE_EMBMS_SUPPORTED
#include "m2ap_md.h"
#include "m2ap_init.h"
#endif

#ifdef RRC_UNIT_TEST_FRAMEWORK
#include "rrc_ut_stub_db.h"
#endif


#ifdef LINUX_PC_TEST
#include "TestFrameApi.h"
short adapter_hdl_msg_callback(char *buf, long len);
short adapter_init_socket(void);
void adapter_register_send_data_to_queue(pfun_adapter_send_data_to_queue pfun);
extern U8 for_adapter_send_data_to_queue(U16 ulSrcModId, U16 ulDstModId, U16 ulMsgId, char *msg, U32 ulMsgLen);
#endif

/* Changes for logging enhancement */
pthread_key_t cell_and_ue_index_key;

void set_cell_index(U8 cell_index)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info == NULL)
    {
        info = (cell_and_ue_index*) malloc_wrapper(sizeof(cell_and_ue_index));
        /* Coverity Fix 96724 Start */
        if (NULL == info)
        {
            return;
        }
         /* Coverity Fix 96724 End */
    }
    
    info->cell_index = cell_index;
    pthread_setspecific(cell_and_ue_index_key, info);
}

void set_ue_index(U16 ue_index)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info == NULL)
    {
        info = (cell_and_ue_index*) malloc_wrapper(sizeof(cell_and_ue_index));
        /* Coverity Fix 96756 Start */
        if (NULL == info)
        {
            return;
        }
         /* Coverity Fix 96756 End */
    }

    info->ue_index = ue_index;
    pthread_setspecific(cell_and_ue_index_key, info);
}

void set_cell_and_ue_index(U8 cell_index, U16 ue_index)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info == NULL)
    {
        info = (cell_and_ue_index*) malloc_wrapper(sizeof(cell_and_ue_index));
        /* Coverity Fix 96714 Start */
        if (NULL == info)
        {
            return;
        }
        /* Coverity Fix 96714 End */
    }

    info->cell_index = cell_index;
    info->ue_index = ue_index;
    pthread_setspecific(cell_and_ue_index_key, info);
}

U8 get_cell_index(void)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info)
    {
        return info->cell_index;
    }
    return 0xff;
}

U16 get_ue_index(void)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info != NULL)
    {
        return info->ue_index;
    }
    return 0xffff;
}

// destructor
void free_cell_and_ue_index(void* info)
{
    free_wrapper((cell_and_ue_index*)info);
}

/* Utility function to split addr to ip and port */
#ifdef ALM_FRWK
static void rrc_get_addr(char* addr,
                         char* ip_addr, 
                         unsigned int* port);
#endif

QDRIVER my_driver[RRC_MAX_THREADS];
thread_arg_t thd_arg[RRC_MAX_THREADS];
U8 num_active_threads=0;
U8 g_uecc_lthd_id=0;
U8 g_main_lthd_id=0;
void    *listener = PNULL;
    

/* Compose stack */
static void *Alloc(QMODULE module, U32 attrsize)
{
    
/*SPR 20636 Changes Start*/
    RRC_TRACE(RRC_DETAILEDALL ,"Unused variable :%x",(U32_64_Bit)module);     
/*SPR 20636 Changes End*/
    return qvMsgAllocBySize( attrsize );
}

static void *Pack(QMODULE module, void *buffer, U32 *size)
{

/*SPR 20636 Changes Start*/
    RRC_TRACE(RRC_DETAILEDALL ,"Unused variable :%x",(U32_64_Bit)module);     
/*SPR 20636 Changes End*/
    *size = rrc_get_api_buf_size((U8 *)buffer);
    return buffer;
}



/* RRC CSPL modules */
static const unsigned long RrcLlimDestinationList[] =
{
    RRC_OAMH_MODULE_ID,
    RRC_CSC_MODULE_ID,
    RRC_UECCMD_MODULE_ID,
    RRC_UECC_1_MODULE_ID,
    RRC_UECC_2_MODULE_ID,
    RRC_UECC_3_MODULE_ID,
    RRC_UECC_4_MODULE_ID,
    RRC_UECC_5_MODULE_ID,
    RRC_UECC_6_MODULE_ID,
    RRC_UECC_7_MODULE_ID,
    RRC_UECC_8_MODULE_ID,
    RRC_PDCP_1_MODULE_ID,
    RRC_PDCP_2_MODULE_ID,
    RRC_PDCP_3_MODULE_ID,
    RRC_PDCP_4_MODULE_ID,
    RRC_PDCP_5_MODULE_ID,
    RRC_PDCP_6_MODULE_ID,
    RRC_PDCP_7_MODULE_ID,
    RRC_PDCP_8_MODULE_ID,
    RRC_RLC_1_MODULE_ID,
    RRC_RLC_2_MODULE_ID,
    RRC_RLC_3_MODULE_ID,
    RRC_RLC_4_MODULE_ID,
    RRC_RLC_5_MODULE_ID,
    RRC_RLC_6_MODULE_ID,
    RRC_RLC_7_MODULE_ID,
    RRC_RLC_8_MODULE_ID,
    RRC_MAC_1_MODULE_ID,
    RRC_MAC_2_MODULE_ID,
    RRC_MAC_3_MODULE_ID,
    RRC_MAC_4_MODULE_ID,
    RRC_MAC_5_MODULE_ID,
    RRC_MAC_6_MODULE_ID,
    RRC_MAC_7_MODULE_ID,
    RRC_MAC_8_MODULE_ID,
    RRC_PHY_1_MODULE_ID,
    RRC_PHY_2_MODULE_ID,
    RRC_PHY_3_MODULE_ID,
    RRC_PHY_4_MODULE_ID,
    RRC_PHY_5_MODULE_ID,
    RRC_PHY_6_MODULE_ID,
    RRC_PHY_7_MODULE_ID,
    RRC_PHY_8_MODULE_ID,
    RRC_S1U_1_MODULE_ID,
    RRC_S1U_2_MODULE_ID,
    RRC_S1U_3_MODULE_ID,
    RRC_S1U_4_MODULE_ID,
    RRC_S1U_5_MODULE_ID,
    RRC_S1U_6_MODULE_ID,
    RRC_S1U_7_MODULE_ID,
    RRC_S1U_8_MODULE_ID,
    0
};

const   QMANIFEST RrcLlimEntity =
{
    "RRC_LLIM_MODULE_ID",
    RRC_LLIM_MODULE_ID,

    { 0, rrc_llim_init },
    { rrc_llmd_process_msg, rrc_llmd_process_timer_msg },
    { Pack, 0, Alloc },

    RrcLlimDestinationList
};

static const unsigned long RrcCscDestinationList[] =
{
    RRC_OAMH_MODULE_ID,
    RRC_UECCMD_MODULE_ID,
    RRC_LLIM_MODULE_ID,
    RRC_S1AP_MODULE_ID,
    RRC_RRM_MODULE_ID,
    /*SPR_17448_START*/
    RRC_X2AP_MODULE_ID,
    /*SPR_17448_END*/   
    0
};

const   QMANIFEST RrcCscEntity =
{
    "RRC_CSC_MODULE_ID",
    RRC_CSC_MODULE_ID,

    { 0, rrc_csc_init },
    { rrc_csc_process_msg, rrc_csc_process_timer_msg },
    { Pack, 0, Alloc },

    RrcCscDestinationList
};

static const unsigned long RrcOamhDestinationList[] =
{
    RRC_UECCMD_MODULE_ID,
    RRC_UECC_1_MODULE_ID,
    RRC_UECC_2_MODULE_ID,
    RRC_UECC_3_MODULE_ID,
    RRC_UECC_4_MODULE_ID,
    RRC_UECC_5_MODULE_ID,
    RRC_UECC_6_MODULE_ID,
    RRC_UECC_7_MODULE_ID,
    RRC_UECC_8_MODULE_ID,
    RRC_CSC_MODULE_ID,
    RRC_LLIM_MODULE_ID,
    RRC_OAM_MODULE_ID,
    0
};

const   QMANIFEST RrcOamhEntity =
{
    "RRC_OAMH_MODULE_ID",
    RRC_OAMH_MODULE_ID,

    { 0, rrc_oamh_init },
    { rrc_oamh_process_msg, rrc_oamh_process_timer_msg },
    { Pack, 0, Alloc },

    RrcOamhDestinationList
};

static const unsigned long RrcUeccDestinationList[] =
{
    RRC_OAMH_MODULE_ID,
    RRC_CSC_MODULE_ID,
    RRC_LLIM_MODULE_ID,
    RRC_S1AP_MODULE_ID,
    RRC_UECCMD_MODULE_ID,
    /*SPR_17448_START*/
    RRC_X2AP_MODULE_ID,
    /*SPR_17448_END*/   
    0
};

const   QMANIFEST RrcUeccEntity1 =
{
    "RRC_UECC_1_MODULE_ID",
    RRC_UECC_1_MODULE_ID,

    { 0, rrc_uecc_init },
    { rrc_uecc_process_msg, rrc_uecc_process_timer_msg },
    { Pack, 0, Alloc },

    RrcUeccDestinationList
};

const   QMANIFEST RrcUeccEntity2 =
{
    "RRC_UECC_2_MODULE_ID",
    RRC_UECC_2_MODULE_ID,

    { 0, rrc_uecc_init },
    { rrc_uecc_process_msg, rrc_uecc_process_timer_msg },
    { Pack, 0, Alloc },

    RrcUeccDestinationList
};

const   QMANIFEST RrcUeccEntity3 =
{
    "RRC_UECC_3_MODULE_ID",
    RRC_UECC_3_MODULE_ID,

    { 0, rrc_uecc_init },
    { rrc_uecc_process_msg, rrc_uecc_process_timer_msg },
    { Pack, 0, Alloc },

    RrcUeccDestinationList
};

const   QMANIFEST RrcUeccEntity4 =
{
    "RRC_UECC_4_MODULE_ID",
    RRC_UECC_4_MODULE_ID,

    { 0, rrc_uecc_init },
    { rrc_uecc_process_msg, rrc_uecc_process_timer_msg },
    { Pack, 0, Alloc },

    RrcUeccDestinationList
};

const   QMANIFEST RrcUeccEntity5 =
{
    "RRC_UECC_5_MODULE_ID",
    RRC_UECC_5_MODULE_ID,

    { 0, rrc_uecc_init },
    { rrc_uecc_process_msg, rrc_uecc_process_timer_msg },
    { Pack, 0, Alloc },

    RrcUeccDestinationList
};

const   QMANIFEST RrcUeccEntity6 =
{
    "RRC_UECC_6_MODULE_ID",
    RRC_UECC_6_MODULE_ID,

    { 0, rrc_uecc_init },
    { rrc_uecc_process_msg, rrc_uecc_process_timer_msg },
    { Pack, 0, Alloc },

    RrcUeccDestinationList
};

const   QMANIFEST RrcUeccEntity7 =
{
    "RRC_UECC_7_MODULE_ID",
    RRC_UECC_7_MODULE_ID,

    { 0, rrc_uecc_init },
    { rrc_uecc_process_msg, rrc_uecc_process_timer_msg },
    { Pack, 0, Alloc },

    RrcUeccDestinationList
};

const   QMANIFEST RrcUeccEntity8 =
{
    "RRC_UECC_8_MODULE_ID",
    RRC_UECC_8_MODULE_ID,

    { 0, rrc_uecc_init },
    { rrc_uecc_process_msg, rrc_uecc_process_timer_msg },
    { Pack, 0, Alloc },

    RrcUeccDestinationList
};

static const unsigned long S1apDestinationList[] =
{
    RRC_OAM_MODULE_ID,
    RRC_CSC_MODULE_ID,
    RRC_UECCMD_MODULE_ID,
    RRC_UECC_1_MODULE_ID,
    RRC_UECC_2_MODULE_ID,
    RRC_UECC_3_MODULE_ID,
    RRC_UECC_4_MODULE_ID,
    RRC_UECC_5_MODULE_ID,
    RRC_UECC_6_MODULE_ID,
    RRC_UECC_7_MODULE_ID,
    RRC_UECC_8_MODULE_ID,
    RRC_MME_MODULE_ID,
    RRC_RRM_MODULE_ID,
    0
};

const   QMANIFEST S1apEntity =
{
    "RRC_S1AP_MODULE_ID",
    RRC_S1AP_MODULE_ID,

    { 0, s1ap_init },
    { s1ap_process_msg, s1ap_process_timer_msg },
    { Pack, 0, Alloc },

    S1apDestinationList
};



/* L3-X2AP Integration Activity - Start */

static const unsigned long X2apDestinationList[] =
{
     RRC_OAM_MODULE_ID,
     RRC_UECCMD_MODULE_ID,
     RRC_UECC_1_MODULE_ID,
     RRC_UECC_2_MODULE_ID,
     RRC_UECC_3_MODULE_ID,
     RRC_UECC_4_MODULE_ID,
     RRC_UECC_5_MODULE_ID,
     RRC_UECC_6_MODULE_ID,
     RRC_UECC_7_MODULE_ID,
     RRC_UECC_8_MODULE_ID,
     RRC_CSC_MODULE_ID,
     RRC_X2AP_PEER_eNODEB_MODULE_ID,
     RRC_SON_MODULE_ID,
     RRC_S1AP_MODULE_ID,
     0
};

const   QMANIFEST X2apEntity =
{
    "RRC_X2AP_MODULE_ID", /* Module name*/
    RRC_X2AP_MODULE_ID,   /* Module ID*/
    { 0, x2ap_init },      /* Early Init Function, Late Init Function */
    { x2ap_process_msg, x2ap_process_timer_msg }, /* Message Handler, Timer Handler */
    { Pack, 0, Alloc },   /* Pack, Unpack and alloc functions*/

    X2apDestinationList   /* List of external destinations*/
};
/* L3-X2AP Integration Activity - End*/

#ifdef LTE_EMBMS_SUPPORTED
static const unsigned long M2apDestinationList[] =
{
     RRC_OAM_MODULE_ID,
     RRC_MCE_MODULE_ID,
     RRC_RRM_MODULE_ID,
     RRC_MAC_1_MODULE_ID,
     RRC_MAC_2_MODULE_ID,
     RRC_MAC_3_MODULE_ID,
     RRC_MAC_4_MODULE_ID,
     RRC_MAC_5_MODULE_ID,
     RRC_MAC_6_MODULE_ID,
     RRC_MAC_7_MODULE_ID,
     RRC_MAC_8_MODULE_ID,
     RRC_RLC_1_MODULE_ID,
     RRC_RLC_2_MODULE_ID,
     RRC_RLC_3_MODULE_ID,
     RRC_RLC_4_MODULE_ID,
     RRC_RLC_5_MODULE_ID,
     RRC_RLC_6_MODULE_ID,
     RRC_RLC_7_MODULE_ID,
     RRC_RLC_8_MODULE_ID,
     RRC_S1U_1_MODULE_ID,
     RRC_MODULE_ID,
     RRC_X2AP_MODULE_ID,
     RRC_LLIM_MODULE_ID,
     0
};

const   QMANIFEST M2apEntity =
{
    "RRC_M2AP_MODULE_ID", /* Module name*/
    RRC_M2AP_MODULE_ID,   /* Module ID*/
    { 0, m2ap_init },      /* Early Init Function, Late Init Function */
    { m2ap_process_msg, m2ap_process_timer_msg }, /* Message Handler, Timer Handler */
    { Pack, 0, Alloc },   /* Pack, Unpack and alloc functions*/

    M2apDestinationList   /* List of external destinations*/
};
#endif
#ifdef RRC_UT_FRAMEWORK
U8 suitId = 0xFF;
#endif

static const unsigned long RrcUeccMdDestinationList[] =
{
    RRC_OAMH_MODULE_ID,
    RRC_UECC_1_MODULE_ID,
    RRC_UECC_2_MODULE_ID,
    RRC_UECC_3_MODULE_ID,
    RRC_UECC_4_MODULE_ID,
    RRC_UECC_5_MODULE_ID,
    RRC_UECC_6_MODULE_ID,
    RRC_UECC_7_MODULE_ID,
    RRC_UECC_8_MODULE_ID,
    RRC_LLIM_MODULE_ID,
    RRC_CSC_MODULE_ID,
    RRC_S1AP_MODULE_ID,
    RRC_X2AP_MODULE_ID,
    RRC_RRM_MODULE_ID,
    0
};

const   QMANIFEST RrcUeccMdEntity =
{
    "RRC_UECCMD_MODULE_ID",
    RRC_UECCMD_MODULE_ID,

    { 0, rrc_ueccmd_init },
    { rrc_ueccmd_process_msg, rrc_ueccmd_process_timer_msg },
    { Pack, 0, Alloc },

    RrcUeccMdDestinationList
};

static const unsigned long LoggerDestinationList[] =
{
    0
};

const   QMANIFEST LoggerEntity =
{
    "RRC_LOGGER_MODULE_ID", /* Module name*/
    RRC_LOGGER_MODULE_ID,   /* Module ID*/
    { 0, l3_logger_init },      /* Early Init Function, Late Init Function */
    { l3_logger_process_msg, l3_logger_process_timer_msg }, /* Message Handler, Timer Handler */
    { Pack, 0, Alloc },   /* Pack, Unpack and alloc functions*/

    LoggerDestinationList   /* List of external destinations*/
};

extern const QSYSOP* os;
extern const QWAIT* waitstruct;
extern const QSHELL* operations;
void * thread_func(void * thread_arg);

void * thread_func(void * thread_arg)
{
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);

    thread_arg_t *p_thread_arg = (thread_arg_t *)thread_arg;
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    lgr_logger_register_thread(p_thread_arg->logical_id);
#endif

    qvDriverLoop(p_thread_arg->driver_id,p_thread_arg->p_qwait);
    return 0;
}

/*****************************************************************************
 * Function Name  : user_get_file_len
 * Inputs         : p_file_name - Pointer to configuration file.
 * Outputs        : None.
 * Returns        : Number of lines present in the file.
 * Description    : This function counts the length of file.
 ****************************************************************************/
static U32 user_get_file_len(const S8* p_file_name)
{
        FILE*       fd      =   PNULL;
        S32      flen    = 0;

        RRC_ASSERT(PNULL != p_file_name);

        fd = fopen_wrapper(p_file_name, (const S8*)"r");

        if(PNULL == fd)
        {
                RRC_TRACE( RRC_WARNING, "Unable to open %s", p_file_name);
                return flen;
        }

        rrc_fseek(fd, 0, SEEK_END);
        flen = rrc_ftell(fd);
        rrc_fseek(fd, 0, SEEK_SET);

        rrc_fclose(fd);
        if (flen <= 0)
           flen = 0;

        return ((U32)flen);
}
/******************************************************************************
 * Function Name  : get_string_buff
 * Inputs         : p_file_name - Pointer to configuration file.
 * Outputs        : None.
 * Returns        : Pointer to string buffer.
 * Description    : This function reads the content of the file.
 ******************************************************************************/
static S8* get_string_buff(const S8* p_file_name)
{
    FILE*       fd      = PNULL;
    U32         flen    = 0;
    S8*       buf     = PNULL;


    flen = user_get_file_len(p_file_name);
    if(0 == flen)
    {
        return buf;
    }
    fd = fopen_wrapper(p_file_name, (const S8*)"r");
                            
    if(PNULL == fd)
    {
        RRC_TRACE(RRC_WARNING, "Unable to open %s", p_file_name);
        return buf;
    }

    buf = (S8*)rrc_mem_get(flen+1);
    if(PNULL == buf)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        rrc_fclose(fd);
        return PNULL;
    }
    flen = (U32)rrc_fread(buf, 1, flen, fd);
    buf[flen] = 0;

    rrc_fclose(fd);
    return buf;
}
                        
/******************************************************************************
 * Function Name  : user_get_num_lines_in_str_buf
 * Inputs         : p_str_buf - Pointer to target string.
 *                  str_delim - string delimiter.
 * Outputs        : None.
 * Returns        : Number of lines in string buffer.
 * Description    : This function calculates number of lines based on delimiter.
 ******************************************************************************/

static U32 user_get_num_lines_in_str_buf(const S8* p_str_buf, S8 str_delim)
{
    U32 i,j;
    RRC_ASSERT(PNULL != p_str_buf);
    for(i=0,j=0; p_str_buf[i]; i++)
    {
        if(p_str_buf[i] == str_delim)
        {
            j++;
        }
    }
    if(p_str_buf[strlen_wrapper((const Char8 *)p_str_buf)-1] != str_delim)
    {
        j++;
    }
    return j;
}

/*****************************************************************************
 * Function Name  : get_string
 * Inputs         : p_str_buf - Pointer to target string.
 *                  p_str_delim - Pointer to string delimiter.
 *                  comment_symbol - represents the comment symbol used for 
 *                  comments.
 * Outputs        : None.
 * Returns        : Pointer to pointer to the string buffer.
 * Description    : This function parse the buffer read from the configuration
 *                  file and tokenizes it to get key values. It ignores the
 *                  starting with '#'.
 ****************************************************************************/

static S8** get_string(const S8* str_buf, const S8* str_delim, S8 comment_symbol)
{
    U32  i = 0, nlines  = 0;
    S8* token = PNULL;
    S8* loc_buf = PNULL;
    S8**        str_arr = PNULL;

    RRC_ASSERT(PNULL!= str_buf);
    RRC_ASSERT(PNULL != str_delim);
    loc_buf = rrc_mem_get(strlen_wrapper((const Char8 *)(str_buf))+1);
    if(PNULL == loc_buf)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        return PNULL;
    }
    /*valgrind_fix start*/
    memset_wrapper(loc_buf,0,strlen_wrapper((const Char8 *)(str_buf))+1);
    /*valgrind_fix stop*/
 
   /* Coverity Fix 20191 Start */
   l3_strncpy_wrapper((S8*)loc_buf, (const S8*)str_buf, strlen_wrapper((const Char8 *)str_buf));
   /* Coverity Fix 20191 End */
    nlines = user_get_num_lines_in_str_buf(loc_buf, str_delim[0]);
    if(0 == nlines)
    {
        /* coverity_fix_63308_start */
        rrc_mem_free(loc_buf);
        /* coverity_fix_63308_stop */
        return str_arr;
    }
    str_arr = (S8**)rrc_mem_get((nlines+1)*sizeof(U8*));
    if(PNULL == str_arr)
    {
        RRC_TRACE(RRC_WARNING,"%s: Memory allocation failed",__FUNCTION__);
        /* coverity_fix_63308_start */
        rrc_mem_free(loc_buf);
        /* coverity_fix_63308_stop */
        return PNULL;
    }

    token = (S8*)l3_strtok_wrapper(loc_buf, str_delim);
    while( token != PNULL )
    {
        if(token[0] != comment_symbol)
        {
         /* we must reserve memory for nul symbol in the end of array*/
         /* otherwise this memory can be heap allocated for another  */
         /* needs and our nul will be destroyed                      */
            
        str_arr[i] = (S8*)strdup_wrapper((const S8*)token);
            
        RRC_ASSERT(PNULL != str_arr[i]);
            i++;
         }
         token = (S8*)l3_strtok_wrapper(PNULL, str_delim);
    }
    str_arr[i] = PNULL;
    rrc_mem_free(loc_buf);
    return str_arr;
}




/*****************************************************************************
 * Function Name  : read_config_from_file
 * Inputs         : p_config_file_name - Pointer to configuration file.
 * Outputs        : None.
 * Returns        : Pointer to pointer to string buffer.
 * Description    : This function reads the content of the file and returns 
 *                  the pointer to pointer to the strings buffer after 
 *                  removing commented lines.
 ****************************************************************************/

static S8 ** read_config_from_file(S8 *p_config_file_name)
{
    S8*     str_buf = PNULL;
    S8**    str_arr = PNULL;

    str_buf = get_string_buff(p_config_file_name);
    if(PNULL == str_buf)
    {
         return str_arr;
    }
    str_arr = get_string(str_buf, (const S8*)"\n", '#');
    rrc_mem_free(str_buf);
    return str_arr;
}

/*****************************************************************************
 * Function Name  : is_key_present
 * Inputs         : p_KeyName - Pointer to key value to be searched.
 *                  p_string - Pointer to target string.
 * Outputs        : None.
 * Returns        : Pointer to the key value.
 * Description    : This function searches the key value from the target string
 ****************************************************************************/
static S8 *is_key_present(S8 *p_keyName,S8 *p_string)
{
        U32 keyLen=(U32)strlen_wrapper((const Char8 *)p_keyName);
    /*if ( 0 == strncmp_wrapper((const char*)p_keyName, (const char*)p_string,
     * keyLen))*/ /*sudhansu*/
    if ( 0 == strncmp_wrapper((const S8 *)p_keyName, (const S8 *)p_string, keyLen))
    {
        if ( '=' == p_string[keyLen] )
        {
               /* Returning position for Key Value */  
            return p_string + keyLen + 1;
        }
    }
    return PNULL;
}

/*****************************************************************************
 * Function Name  : free_all_strings
 * Inputs         : p_p_strings - Pointer to pointer to strings. 
 * Outputs        : None
 * Returns        : None
 * Description    : This function frees all the memory taken for reading 
 *                  configuration.
 ****************************************************************************/
static void free_all_strings(S8** p_p_strings)
{
    U32 counter;
    RRC_ASSERT(PNULL != p_p_strings);

    for(counter = 0; p_p_strings[counter]; counter++)
    {
        rrc_free(p_p_strings[counter]);
    }
    rrc_mem_free(p_p_strings);
}


/*****************************************************************************
 * Function Name  : read_config
 * Inputs         : p_filename - Pointer to cofiguration file. 
 *                  p_oam_stack_address - Pointer to oam stack address
 *                  p_rrc_stack_address - Pointer to rrc stack address
 * Outputs        : None
 * Returns        : None
 * Description    : This function reads the OAM and RRC configuration from the file.
 ****************************************************************************/

static void read_config( S8 *p_filename, S8* p_oam_stack_address, S8* p_rrc_stack_address)
{
    S32 i=0;
    S8 **p_p_strings = read_config_from_file(p_filename);
    S8 *p_val = PNULL;
    
    if(PNULL!=p_p_strings)
    {
        RRC_TRACE(RRC_INFO,"Reading OAM and RRC configuration from '%s'",p_filename);
        for(i=0; p_p_strings[i]; i++)
        {
            p_val=is_key_present((S8*)"ENB_OAM_IP_ADDRESS",p_p_strings[i]);
            if(PNULL != p_val)
            {
                l3_memcpy_wrapper(p_oam_stack_address, p_val, strlen_wrapper((const Char8 *)p_val));
                p_oam_stack_address[strlen_wrapper((const Char8 *)p_val)] = 0;
            }

            p_val = is_key_present((S8*)"ENB_OAM_RX_PORT",p_p_strings[i]);
            if(PNULL != p_val)
            {
            /* Coverity Fix 11365 Start */
                l3_strncat_wrapper((S8*)p_oam_stack_address, ((const S8 *)":"), 1);
                l3_strncat_wrapper((S8*)p_oam_stack_address, (const S8 *)p_val,
                        strlen_wrapper((const Char8 *)p_val));
            /* Coverity Fix 11365 End */

            }
            p_val=is_key_present((S8*)"ENB_L3_IP_ADDRESS",p_p_strings[i]);
            if(PNULL != p_val)
            {
                l3_memcpy_wrapper(p_rrc_stack_address, p_val, strlen_wrapper((const Char8*)p_val));
                p_rrc_stack_address[strlen_wrapper((const Char8*)p_val)] = 0;
            }

            p_val = is_key_present((S8*)"ENB_L3_PORT",p_p_strings[i]);
            if(PNULL != p_val)
            {
            /* Coverity Fix 11365 Start */
                l3_strncat_wrapper((S8*)p_rrc_stack_address,((const S8 *)":"), 1);
                l3_strncat_wrapper((S8*)p_rrc_stack_address, (const S8 *)p_val,
                        strlen_wrapper((const Char8 *)p_val));
            /* Coverity Fix 11365 End */

            }
        }
        RRC_TRACE(RRC_INFO,"Read the configuration");
        free_all_strings(p_p_strings);
    }
}

#ifdef RRC_PROFILER
/*****************************************************************************
 * Function Name  : sigusr1_handler 
 * Inputs         : S32 signal
 * Outputs        : None
 * Returns        : void
 * Description    : Signal Handler function for doing gcov_flush operation
 ********************************************************************************/
void sigusr1_handler(int sig)
{
    printf("Signal received\n");
    __gcov_flush();
    rrc_signal_handler(SIGUSR1,sigusr1_handler);
}
#endif


/*****************************************************************************
 * Function Name  : main 
 * Inputs         : argc
 *                : argv
 * Outputs        : None
 * Returns        : integer
 * Description    : This is the main entry function.
 ****************************************************************************/
int main(int argc, char** argv)
{
    pthread_key_create(&cell_and_ue_index_key, free_cell_and_ue_index);
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);

#ifdef LICENSE_FLAG
    S32 verify              = RRC_NULL;
    S32 check_return        = RRC_NULL;
    FILE *fptr_limitsfile  = PNULL;
    FILE *fptr_licensefile = PNULL;
#endif
    S8 oam_stack_address[MAX_LEN_COMM_INFO] = OAM_STACK_ADDRESS;
    S8 rrc_stack_address[MAX_LEN_COMM_INFO] = RRC_STACK_ADDRESS;
    U8 thread_index = 0;
    /* SPR 21806 Start */
    struct rlimit limit;
    /* SPR 21806 End */
    struct sched_param schedParam;
    pthread_attr_t attr;
    int s = 0;

/*SPR 22554 Fix Start*/
     U8 threadName[MAX_THREAD_NAME_LEN] = "rrc_main";
/*SPR 22554 Fix End*/
    /* SPR 1416 Start */
#ifdef L3_CORE_BINDING_ENABLE
    U32 coreNum = 0;
#endif

#ifdef RRC_PROFILER
    rrc_signal_handler(SIGUSR1,sigusr1_handler);
#endif
    /* SPR 16120 Start */
    rrc_signal_handler();
    /* SPR 16120 Stop */

    cpu_set_t cpuSetSch;
    pid_t p = 0;
/*SPR 21653 changes start*/    
    comm_info_t comm_info_int =
    {{ OAM_STACK_ADDRESS, RRM_STACK_ADDRESS, RRC_STACK_ADDRESS, SON_STACK_ADDRESS },
     { 8, {{0,  {S1U_STACK_ADDRESS, PDCP_STACK_ADDRESS, RLC_STACK_ADDRESS, MAC_STACK_ADDRESS, PHY_STACK_ADDRESS }},
           {1, { S1U_STACK_ADDRESS, PDCP_STACK_ADDRESS, RLC_STACK_ADDRESS, MAC_STACK_ADDRESS, PHY_STACK_ADDRESS }},
           {2, { S1U_STACK_ADDRESS, PDCP_STACK_ADDRESS, RLC_STACK_ADDRESS, MAC_STACK_ADDRESS, PHY_STACK_ADDRESS }},
           {3, { S1U_STACK_ADDRESS, PDCP_STACK_ADDRESS, RLC_STACK_ADDRESS, MAC_STACK_ADDRESS, PHY_STACK_ADDRESS }},
           {4, { S1U_STACK_ADDRESS, PDCP_STACK_ADDRESS, RLC_STACK_ADDRESS, MAC_STACK_ADDRESS, PHY_STACK_ADDRESS }},
           {5, { S1U_STACK_ADDRESS, PDCP_STACK_ADDRESS, RLC_STACK_ADDRESS, MAC_STACK_ADDRESS, PHY_STACK_ADDRESS }},
           {6, { S1U_STACK_ADDRESS, PDCP_STACK_ADDRESS, RLC_STACK_ADDRESS, MAC_STACK_ADDRESS, PHY_STACK_ADDRESS }},
           {7, { S1U_STACK_ADDRESS, PDCP_STACK_ADDRESS, RLC_STACK_ADDRESS, MAC_STACK_ADDRESS, PHY_STACK_ADDRESS }}}}};
/*SPR 21653 changes end*/    
    
    comm_info_init_param_t initial_param;
    U8   uecc1_module_id = RRC_NULL;
    U8 uecc2_module_id = RRC_NULL, uecc3_module_id = RRC_NULL, uecc4_module_id = RRC_NULL, 
       uecc5_module_id = RRC_NULL, uecc6_module_id = RRC_NULL, uecc7_module_id = RRC_NULL, uecc8_module_id = RRC_NULL;


    

#ifdef LICENSE_FLAG
    /* Checking limitsfile existence */
    fptr_limitsfile = fopen_wrapper("limitsfile","r");

    /* Checking License File existence */
    fptr_licensefile = fopen_wrapper("licensefile","r");

    /* Check the validation of license file*/
    check_return = check("limitsfile","licensefile");
#endif

#ifdef LINUX_PC_TEST
	adapter_init_socket();
	TestInitFrame("192.168.201.31", 54403);
	TestFrameRegisterMsgHandler(adapter_hdl_msg_callback);
	adapter_register_send_data_to_queue(for_adapter_send_data_to_queue);
#endif

    memset_wrapper(&thd_arg, 0, sizeof(thread_arg_t)*RRC_MAX_THREADS);
    num_active_threads=1; 


    memset_wrapper(&initial_param, 0, sizeof(comm_info_init_param_t));
    l3_memcpy_wrapper(&initial_param.comm_info, &comm_info_int, sizeof(comm_info_t));
   
    qvInit(0,os);
   /* assert check for two #defines does not make any sense */
   /* RRC_ASSERT(OAM_MODULE_ID_RRM + 1 == MAX_EXT_MODULES);*/

#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    if (init_log(LogTarget_Console, NULL, NULL) != LogSuccess)
    {
        qvPanic("Cannot init advanced logger");
        return -1;
    }

    if (init_log(LogTarget_Console, NULL, &g_advl_ctxt_s1ap) != LogSuccess)
    {
        qvPanic("Cannot init advanced logger");
        return -1;
    }

    if (init_log(LogTarget_Console, NULL, &g_advl_ctxt_x2ap) != LogSuccess)
    {
        qvPanic("Cannot init advanced logger");
        return -1;
    }

/* SPR 22248 MBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED 
    if (init_log(LogTarget_Console, NULL, &g_advl_ctxt_m2ap) != LogSuccess)
    {
        qvPanic("Cannot init advanced logger");
        return -1;
    }
#endif
/* SPR 22248 MBMS Changes Stop */

    set_loglevel_limit(g_advl_ctxt_rrc, LogBrief);
    /* SPR_17842_FIX_START - */
    set_loglevel_limit(g_advl_ctxt_x2ap, LogBrief);
    set_loglevel_limit(g_advl_ctxt_s1ap, LogBrief);
    /* SPR_17842_FIX_STOP - */
#elif (LOGGER_TYPE == RRC_INTERNAL_LOGGER)
    rrc_set_loglevel(RRC_BRIEF);
#endif /* (LOGGER_TYPE == RRC_ADV_LOGGER) */

    RRC_TRACE(RRC_INFO,"Number of arguments:%d",argc);



#ifdef RRC_UNIT_TEST_FRAMEWORK
   stub_init_suit(); 
#endif
#ifdef RRC_UT_FRAMEWORK
    if (argc < 2) {
        RRC_TRACE(RRC_INFO,"L3 compiled with L3_CORE_BINDING_ENABLE flag so Core No can't be absent.");
        RRC_TRACE(RRC_INFO,"Usage is : ./rrc_entity <core num to bind> <optional- configuration file path> \n "
                " Now exiting.....");
        return 0;
    }
	suitId = atoi(argv[1]);
    test_cunit(0xFFFF);
#else

#ifdef L3_CORE_BINDING_ENABLE
    
    if (argc < 2) {
        RRC_TRACE(RRC_INFO,"L3 compiled with L3_CORE_BINDING_ENABLE flag so Core No can't be absent.");
        RRC_TRACE(RRC_INFO,"Usage is : ./rrc_entity <core num to bind> <optional- configuration file path> \n "
                " Now exiting.....");
        return 0;
    }

    coreNum = atoi_wrapper(argv[1]);
    CPU_ZERO(&cpuSetSch);
    CPU_SET(coreNum ,&cpuSetSch);
    if (0 != sched_setaffinity(p, sizeof(cpu_set_t), &cpuSetSch))
    {
        RRC_TRACE(RRC_INFO, " sched_setaffinity Returned Error in L3 RRC");
    }
    else
    {
        CPU_ZERO(&cpuSetSch);
        sched_getaffinity(p, sizeof(cpu_set_t), &cpuSetSch);
        RRC_TRACE(RRC_INFO," Core Binding is successful. Core Affinity mask returned by kernel is  %u",
                cpuSetSch);
    }
#else
    CPU_ZERO(&cpuSetSch);
    sched_getaffinity(p, sizeof(cpu_set_t), &cpuSetSch);
    /* SPR_17377_Warning_fix + */
    RRC_TRACE(RRC_INFO," Core Binding is Disable. Default Core Affinity mask returned by kernel");
    /* SPR_17377_Warning_fix - */
#endif

#endif
    /* SPR 1416 Stop */
    /* SPR 21806 Start */
    /* Increased the open files limit for the process. It will impact on the cell trace feature. When we increase no of 
     * UEs then no of files will also get increased as per the ues. */
    limit.rlim_cur = MAX_UE_SUPPORTED + RRC_HUNDRED;
    limit.rlim_max = MAX_UE_SUPPORTED + RRC_HUNDRED;
    if (RRC_ZERO > setrlimit_wrapper(RLIMIT_NOFILE, &limit))
    {
	    	RRC_TRACE(RRC_WARNING,"Unable to increase open file limit. It will impact over the Cell Trace feature. error code: %d ", errno);
	    
	    if(RRC_ZERO == getrlimit_wrapper(RLIMIT_NOFILE, &limit))
	    {
	    	RRC_TRACE(RRC_WARNING,"Total: %d files can be opened.", limit.rlim_cur);
	    }
	    else
	    {
	    	RRC_TRACE(RRC_WARNING,"unable to get total no of files that can be opened. error code: %d ", errno);
	    }
    }
    /* SPR 21806 End */
#ifdef LICENSE_FLAG
    /*Checking limits file*/
    if(!fptr_limitsfile)
    {
        RRC_TRACE(RRC_WARNING,"Unable to open limitsfile,"
        " Make sure you have in current dir \n");
        if(fptr_licensefile)
        {
            rrc_fclose(fptr_licensefile);
        }
        exit_wrapper(RRC_FAILURE);
    }
    if(!fptr_licensefile)
    {
        RRC_TRACE(RRC_WARNING,"Unable to open licensefile,"
        " Make sure you have in current dir \n");
        if(fptr_limitsfile)
        {
            rrc_fclose(fptr_limitsfile);
        }
        exit_wrapper(RRC_FAILURE);
    }
    /*Checking the return value of check*/
    if(!check_return)
    {
        RRC_TRACE(RRC_WARNING,"INVALID LICENSE ");
        exit_wrapper(RRC_FAILURE);
    }
    
    /* RRC Executable Validation for MAC address and time stamp */
    verify = rrc_authentication(fptr_limitsfile, &(initial_param.expiry_duration));
    if (RRC_FAILURE == verify)
    {
        RRC_TRACE(RRC_INFO, "************** LICENSE IS EXPIRED ****************");
        rrc_fclose(fptr_limitsfile);
        rrc_fclose(fptr_licensefile);
        exit_wrapper(RRC_FAILURE);
    }
    rrc_fclose(fptr_limitsfile);
    rrc_fclose(fptr_licensefile);
#endif

    RRC_TRACE(RRC_INFO, "rrc_entry started");

/*SPR_18125_START*/
    rtxMemSetAllocFuncs(rrc_asn_mem_get, NULL, rrc_asn_mem_free);
/*SPR_18125_END*/
#ifdef RRC_UT_FRAMEWORK
    if (PNULL != argv[2]) /* SPR 1416 Start */
	{
        read_config((S8 *)argv[2], oam_stack_address, rrc_stack_address); /* SPR 1416 Stop */
        RRC_TRACE(RRC_INFO,"OAM STACK ADDRESS : [ %s ]",oam_stack_address);
        RRC_TRACE(RRC_INFO,"RRC STACK ADDRESS : [ %s ]",rrc_stack_address);
    }
#else

#ifdef L3_CORE_BINDING_ENABLE
    /* Read OAM and RRC Configuration from file*/
    if (PNULL != argv[2]) /* SPR 1416 Start */
    {
        read_config((S8 *)argv[2], oam_stack_address, rrc_stack_address); /* SPR 1416 Stop */
        read_config((S8*)argv[1], oam_stack_address, rrc_stack_address);
        RRC_TRACE(RRC_INFO,"OAM STACK ADDRESS : [ %s ]",oam_stack_address);
        RRC_TRACE(RRC_INFO,"RRC STACK ADDRESS : [ %s ]",rrc_stack_address);
    }
    else
    {
        RRC_TRACE(RRC_INFO,"Loading Default Configuration for OAM..");
    }
#else
    /* Read OAM and RRC Configuration from file*/
    if (PNULL != argv[1]) /* SPR 1416 Start */
    {
        read_config((S8 *)argv[1], oam_stack_address, rrc_stack_address); /* SPR 1416 Stop */
        RRC_TRACE(RRC_INFO,"OAM STACK ADDRESS : [ %s ]",oam_stack_address);
        RRC_TRACE(RRC_INFO,"RRC STACK ADDRESS : [ %s ]",rrc_stack_address);
    }
    else
    {
        RRC_TRACE(RRC_INFO,"Loading Default Configuration for OAM..");
    }
#endif
#endif

    listener = qvOpen(operations, rrc_stack_address);
    
    if (listener == PNULL)
    {
        /* Couldn't initialize connection to external entities */
        return -1;
    }

    for (thread_index = 0; thread_index < RRC_MAX_THREADS; thread_index++)
    {
        /* Creating a new driver */
        my_driver[thread_index] = qvNewDriver();
    }

    qvRegister(my_driver[0], 0, 0, &RrcOamhEntity, &initial_param);
    qvRegister(my_driver[0], 0, 0, &RrcUeccMdEntity, 0);
    qvRegister(my_driver[0], 0, 0, &RrcLlimEntity, 0);
    qvRegister(my_driver[0], 0, 0, &RrcCscEntity, 0);
    qvRegister(my_driver[0], 0, 0, &S1apEntity, 0);
    qvRegister(my_driver[0], 0, 0, &X2apEntity, &initial_param);
#ifdef LTE_EMBMS_SUPPORTED
    qvRegister(my_driver[0], 0, 0, &M2apEntity, 0);
#endif


    uecc1_module_id =  RRC_UECC_1_MODULE_ID;
/*SPR 21653 changes start*/
    qvRegister(my_driver[1], 0, 0, &RrcUeccEntity1, &uecc1_module_id);
/*SPR 21653 changes end*/
    uecc2_module_id =  RRC_UECC_2_MODULE_ID;
    qvRegister(my_driver[2], 0, 0, &RrcUeccEntity2, &uecc2_module_id);
    uecc3_module_id =  RRC_UECC_3_MODULE_ID;
    qvRegister(my_driver[3], 0, 0, &RrcUeccEntity3, &uecc3_module_id);
    uecc4_module_id =  RRC_UECC_4_MODULE_ID;
    qvRegister(my_driver[4], 0, 0, &RrcUeccEntity4, &uecc4_module_id);
    uecc5_module_id =  RRC_UECC_5_MODULE_ID;
    qvRegister(my_driver[5], 0, 0, &RrcUeccEntity5, &uecc5_module_id);
    uecc6_module_id =  RRC_UECC_6_MODULE_ID;
    qvRegister(my_driver[6], 0, 0, &RrcUeccEntity6, &uecc6_module_id);
    uecc7_module_id =  RRC_UECC_7_MODULE_ID;
    qvRegister(my_driver[7], 0, 0, &RrcUeccEntity7, &uecc7_module_id);
    uecc8_module_id =  RRC_UECC_8_MODULE_ID;
    qvRegister(my_driver[8], 0, 0, &RrcUeccEntity8, &uecc8_module_id);
    qvSetDestination(RRC_OAM_MODULE_ID, operations, listener,
        oam_stack_address);
        
    qvSetDestination(RRC_RRM_MODULE_ID, operations, listener,
        &(initial_param.comm_info.
        comm_info_upper_layer[RRC_RRM_MODULE_ID - RRC_MIN_EXT_MODULE_ID][0]));
    qvSetDestination(RRC_PDCP_1_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[0].
            comm_info_lower_layer[RRC_PDCP_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    
/*SPR 21653 changes start*/
    qvSetDestination(RRC_PDCP_2_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[1].
            comm_info_lower_layer[RRC_PDCP_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PDCP_3_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[2].
            comm_info_lower_layer[RRC_PDCP_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PDCP_4_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[3].
            comm_info_lower_layer[RRC_PDCP_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PDCP_5_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[4].
            comm_info_lower_layer[RRC_PDCP_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PDCP_6_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[5].
            comm_info_lower_layer[RRC_PDCP_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PDCP_7_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[6].
            comm_info_lower_layer[RRC_PDCP_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PDCP_8_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[7].
            comm_info_lower_layer[RRC_PDCP_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
/*SPR 21653 changes end*/

    qvSetDestination(RRC_RLC_1_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[0].
            comm_info_lower_layer[RRC_RLC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
/*SPR 21653 changes start*/
    qvSetDestination(RRC_RLC_2_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[1].
            comm_info_lower_layer[RRC_RLC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_RLC_3_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[2].
            comm_info_lower_layer[RRC_RLC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_RLC_4_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[3].
            comm_info_lower_layer[RRC_RLC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_RLC_5_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[4].
            comm_info_lower_layer[RRC_RLC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_RLC_6_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[5].
            comm_info_lower_layer[RRC_RLC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_RLC_7_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[6].
            comm_info_lower_layer[RRC_RLC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_RLC_8_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[7].
            comm_info_lower_layer[RRC_RLC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
/*SPR 21653 changes end*/

    qvSetDestination(RRC_MAC_1_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[0].
            comm_info_lower_layer[RRC_MAC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
/*SPR 21653 changes start*/
    qvSetDestination(RRC_MAC_2_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[1].
            comm_info_lower_layer[RRC_MAC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_MAC_3_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[2].
            comm_info_lower_layer[RRC_MAC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_MAC_4_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[3].
            comm_info_lower_layer[RRC_MAC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_MAC_5_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[4].
            comm_info_lower_layer[RRC_MAC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_MAC_6_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[5].
            comm_info_lower_layer[RRC_MAC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_MAC_7_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[6].
            comm_info_lower_layer[RRC_MAC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_MAC_8_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[7].
            comm_info_lower_layer[RRC_MAC_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
/*SPR 21653 changes end*/
    qvSetDestination(RRC_PHY_1_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[0].
            comm_info_lower_layer[RRC_PHY_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
/*SPR 21653 changes start*/
    qvSetDestination(RRC_PHY_2_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[1].
            comm_info_lower_layer[RRC_PHY_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PHY_3_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[2].
            comm_info_lower_layer[RRC_PHY_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PHY_4_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[3].
            comm_info_lower_layer[RRC_PHY_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PHY_5_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[4].
            comm_info_lower_layer[RRC_PHY_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PHY_6_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[5].
            comm_info_lower_layer[RRC_PHY_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PHY_7_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[6].
            comm_info_lower_layer[RRC_PHY_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_PHY_8_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[7].
            comm_info_lower_layer[RRC_PHY_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
/*SPR 21653 changes end*/

    qvSetDestination(RRC_S1U_1_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[0].
            comm_info_lower_layer[RRC_S1U_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
/*SPR 21653 changes start*/
    qvSetDestination(RRC_S1U_2_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[1].
            comm_info_lower_layer[RRC_S1U_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_S1U_3_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[2].
            comm_info_lower_layer[RRC_S1U_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_S1U_4_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[3].
            comm_info_lower_layer[RRC_S1U_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_S1U_5_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[4].
            comm_info_lower_layer[RRC_S1U_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_S1U_6_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[5].
            comm_info_lower_layer[RRC_S1U_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_S1U_7_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[6].
            comm_info_lower_layer[RRC_S1U_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
    qvSetDestination(RRC_S1U_8_MODULE_ID, operations, listener,
        &((initial_param.comm_info.sector_comm_info_list.sector_comm_info[7].
            comm_info_lower_layer[RRC_S1U_MODULE_ID + RRC_MIN_EXT_MODULE_ID - RRC_OAM_MAX_EXT_LOWER_LAYER_MODULES][0])));
/*SPR 21653 changes end*/
    qvSetDestination(RRC_SON_MODULE_ID, operations, listener,
        &(initial_param.comm_info.comm_info_upper_layer[3][0]));

#ifdef ALM_FRWK
    /* Initializing alarm framework */
    char oam_dst_ip[IP_ADDR_MAX_SIZE] = "127.0.0.1";
    unsigned int oam_dst_port = 6666;
    rrc_get_addr((char *)oam_stack_address, oam_dst_ip, &oam_dst_port);
    RRC_TRACE(RRC_INFO,"OAM STACK ADDRESS used by Alarm FRWK: [ %s ] [ %u ]",
            oam_dst_ip, oam_dst_port);
    /* +- SPR 17777 */
    alarm_init(RRC_OAM_MODULE_ID,oam_dst_ip,oam_dst_port);
    /* +- SPR 17777 */
#endif    

    thd_arg[num_active_threads].driver_id = my_driver[0];
    thd_arg[num_active_threads].p_qwait = (QWAIT*)waitstruct;
    /* creating Driver loop for RRC-S1AP Stack */
    thd_arg[num_active_threads].logical_id = num_active_threads;
    pthread_create(&thd_arg[num_active_threads].tid, 0, thread_func, (void *)&thd_arg[num_active_threads]);
/*SPR 22554 Fix Start*/
    /* Set name of the main thread */
    if (0 != pthread_setname_np_wrapper(&thd_arg[num_active_threads].tid, threadName))
    {
        RRC_TRACE( RRC_WARNING, "Failed to set name of the %d main thread", num_active_threads);
    }

/*SPR 22554 Fix End*/
    g_uecc_lthd_id = num_active_threads;

    thd_arg[num_active_threads-1].logical_id = g_main_lthd_id;
    thd_arg[num_active_threads-1].tid = pthread_self();
#if (LOGGER_TYPE == RRC_ADV_LOGGER)
    lgr_logger_register_thread(g_main_lthd_id);
#endif

    num_active_threads++;
#ifndef LINUX_PC_TEST
    /*Create the high priority thread which is only responsible for getting CDMA
     * System Time and Timestamp from the platform, SFN, SF & L2 timestamp from
     * cell_ctx and calliberating the CDMA system time's first segment to be
     * broadcasted in SIB8. It will only generate CDMA System Time for the first
     * segment and give the control back to the thread running CSC module. CSC
     * module will be responsible for generating the CDMA System time for
     * remaining MAX_SIB8_INSTANCES-1 SIB-8 segments.*/

    /*Assigning it real-time priority. The range of thread priorities are from
     * 0-99. From 86 to 99 are the real time priorities*/
    schedParam.sched_priority = 86; 

    /* create a thread attributes object. */
    s = pthread_attr_init_wrapper(&attr);
    if(s!=0)
        HANDLE_ERROR_EN(s, "pthread_attr_init");

    /*set inherit scheduler attribute in thread attributes object*/
    s = pthread_attr_setinheritsched_wrapper(&attr, PTHREAD_EXPLICIT_SCHED);
    if(s!=0)
        HANDLE_ERROR_EN(s, "pthread_attr_setinheritsched");

    /* set scheduling policy attribute in thread attributes object to FIFO*/
    s = pthread_attr_setschedpolicy_wrapper(&attr, SCHED_FIFO);
    if(s!=0)
        HANDLE_ERROR_EN(s, "pthread_attr_setschedpolicy");

    /* set scheduling parameter attributes in thread attributes object */
    s = pthread_attr_setschedparam_wrapper(&attr, &schedParam);
    if(s!=0)
        HANDLE_ERROR_EN(s, "pthread_attr_setschedparam");

    /* create High Priority thread for CDMA system time fetching & calibrating */
    thd_arg[num_active_threads].logical_id = num_active_threads;
    s = pthread_create(&thd_arg[num_active_threads].tid, &attr, cdma_sys_time_caliberate_func, NULL);
    if (s != 0)
        HANDLE_ERROR_EN(s, "pthread_create");

    /* destroy thread attributes object */
    s = pthread_attr_destroy_wrapper(&attr);
    if (s != 0)
        HANDLE_ERROR_EN(s, "pthread_attr_destroy");
    num_active_threads++;
#endif

    g_uecc_lthd_id = num_active_threads;

#ifdef RRC_UNIT_TEST_FRAMEWORK
    qvSetDestination(RRC_MODULE_ID, 
                    operations, 
                    listener, 
                    0);
    /* Start threads for all the stub */
    thd_arg[num_active_threads].driver_id = qvNewDriver();
    thd_arg[num_active_threads].p_qwait = (QWAIT*)waitstruct;
    thd_arg[num_active_threads].logical_id = num_active_threads;
    stub_register(&thd_arg[num_active_threads]);
    num_active_threads++;
#endif
    qvInputLoop(operations,listener);

    return 0;
}

#ifdef ALM_FRWK
/* Function to split addr into ip and port */
/*****************************************************************************
 * Function Name  : rrc_get_addr 
 * Inputs         : addr 
 *                : ip_addr 
 *                : port
 * Outputs        : None.
 * Returns        : None.
 * Description    : This function splits  the address in to IP and port 
 ****************************************************************************/
static void rrc_get_addr(char* addr,
                         char* ip_addr, 
                         unsigned int* port)
{
    const char token[] = ":";
    char pStr[IP_ADDR_MAX_SIZE] = {0};
    char* pSubStr = NULL;
    
    l3_strncpy_wrapper((S8*)pStr,(const S8*)addr,IP_ADDR_MAX_SIZE-1);

    pSubStr = (char *)l3_strtok_wrapper((S8 *)pStr,(const S8*)token);
    if(pSubStr != NULL)
    {
        l3_strncpy_wrapper((S8*)ip_addr, (const S8*)pSubStr,IP_ADDR_MAX_SIZE-1);
    }
    
    pSubStr = (char *)l3_strtok_wrapper(NULL,(const S8*)token);
    if(pSubStr != NULL)
    {
        *port = atoi_wrapper((const U8*)pSubStr);
    }
}
#endif    
