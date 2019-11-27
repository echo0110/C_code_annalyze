/****************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2012 Aricent Inc. All Rights Reserved.
 *
 ****************************************************************************
 *
 * SPR-612 Coverity Fix Starts
 * $Id: son_main.c $
 * SPR-612 Coverity Fix Ends
 *
 ****************************************************************************
 *
 * File Description: This file contains the main function for the SON which
 *                   is composed from Apps, ES, ANR, NMM and SMGMTIFH modules
 *                   and uses CSPL framework.
 *
 ****************************************************************************
 * Revision Details
 * ----------------
 *
 * SPR-612 Coverity Fix Starts
 * $Log: son_main.c $
 * SPR-612 Coverity Fix Ends
 * Revision 0.1  2012/02/18 09:30:00  Shilpi Jain
 * Initial code
 *
 ****************************************************************************/

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include <son_utils.h>
#include <son_types.h>
#ifdef SON_MRO_FEATURE
#include <son_mro_ctxt_mgr.h>
#endif
#include <son_rach_opt_ctxt_mgr.h>
#include <sched.h>
#include <pthread.h>

#ifdef LINUX_PC_TEST
#include "TestFrameApi.h"
short adapter_hdl_msg_callback(char *buf, long len);
short adapter_init_socket(void);
#endif


/****************************************************************************
 * MACRO Definitions
 ****************************************************************************/
const son_8 *CONFIG_FILE =  "../cfg/eNodeB_Configuration.cfg";

/****************************************************************************
 * Global Variables
 ****************************************************************************/
const son_8 *son_log_facility_name = "LTE_SON";
son_s8 son_log_file[LOG_FILENAME_LEN] = {0};
son_void_t *p_listener = SON_PNULL;
son_global_config_params_t g_config_params;
/* SPR-612 Coverity Fix Starts */
son_u8 *p_file_name = SON_PNULL; 
/* SPR-612 Coverity Fix Ends */

#if (SON_ADV_LOGGER == LOGGER_TYPE)

AdvL_ContextIndex g_main_advl_ctxt;
#endif

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
extern const QSYSOP son_os;
extern const QSHELL son_shell;
extern const QWAIT son_waitstruct;
extern son_s8 son_oam_addr[];
extern son_s8 son_rrm_addr[];
extern son_s8 son_rrc_addr[];
extern son_s8 son_phy_addr[];

/* Changes for logging enhancement */
pthread_key_t cell_and_ue_index_key;

void set_cell_index(U8 cell_index)
{
    cell_and_ue_index* info = (cell_and_ue_index*) pthread_getspecific(cell_and_ue_index_key);
    if (info == NULL)
    {
        info = (cell_and_ue_index*) malloc(sizeof(cell_and_ue_index));
        /* Coverity Fix 96724 Start */
        if(SON_PNULL == info)
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
        info = (cell_and_ue_index*) malloc(sizeof(cell_and_ue_index));
        /* Coverity Fix 96756 Start */
        if(NULL == info)
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
        info = (cell_and_ue_index*) malloc(sizeof(cell_and_ue_index));
        /* Coverity Fix 96714 Start */
        if(SON_PNULL == info)
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
    free((cell_and_ue_index*)info);
}

/******************************************************************************
 * Function Name  : num_lines_in_str_buf
 * Inputs         : p_str_buf - Pointer to target string.
 *                  str_delim - string delimiter.
 * Outputs        : None.
 * Returns        : Number of lines in string buffer.
 * Description    : This function calculates number of lines based on delimiter.
 ******************************************************************************/
static son_u32
num_lines_in_str_buf
(
 const son_s8 *p_str_buf,
 son_s8 str_delim
 )
{
    son_u32 i = 0, j = 0;

    if (SON_PNULL == p_str_buf)
    {
        fprintf(stderr, "Pointer to the config file name is NULL.\n");
    }
    else
    {
        for (i = 0, j = 0; p_str_buf[i]; i++)
        {
            if (p_str_buf[i] == str_delim)
            {
                j++;
            }
        }

        if (p_str_buf[SON_STRLEN((const son_s8 *)p_str_buf) - 1] != str_delim)
        {
            j++;
        }
    }

    return j;
} /* num_lines_in_str_buf */

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
static son_s8 * *
get_string
(
 const son_s8 *p_str_buf,
 const son_s8 *p_str_delim,
 son_s8 comment_symbol
 )
{
    son_u32 i = 0, nlines  = 0;
    son_s8   *p_token    = SON_PNULL;
    son_s8   *p_loc_buf  = SON_PNULL;
    son_s8   * *p_str_arr = SON_PNULL;

    SON_ASSERT(SON_PNULL != p_str_buf);
    SON_ASSERT(SON_PNULL != p_str_delim);

    /* Coverity CID 109028 Fix Start */
    if(p_str_buf != NULL)
    {
	    p_loc_buf = son_mem_get(SON_STRLEN((const son_s8 *)p_str_buf) + 1);
    }
    /* Coverity CID 109028 Fix End */
    /*Coverity Fix CID 66929 Start*/
    if (SON_PNULL == p_loc_buf)
    {
        fprintf(stderr, "get_string : Memory allocation failure,exiting SON application \n"); 
        son_abort();
    }
    else
    {
        SON_STRNCPY((son_s8 *)p_loc_buf, (const son_s8 *)p_str_buf,
                SON_STRLEN((const son_s8 *)p_str_buf) + 1);

        nlines = num_lines_in_str_buf(p_loc_buf, p_str_delim[0]);
        if (0 != nlines)
        {
            p_str_arr = (son_s8 * *)son_mem_get((nlines + 1) * sizeof(son_u8 *));
            /* Coverity ID 66929 Starts */
            if (SON_PNULL == p_str_arr)
            {
                fprintf(stderr, "get_string : Memory allocation failure,exiting SON application \n"); 
                son_abort();
            }
            /* Coverity ID 66929 Ends */
            p_token = (son_s8 *)SON_STRTOK(p_loc_buf, p_str_delim);
            while (p_token != SON_PNULL)
            {
                if (p_token[0] != comment_symbol)
                {
                    /* we must reserve memory for null symbol in the end of array
                     * otherwise this memory can be heap allocated for another
                     * needs and our null will be destroyed                      */

                    p_str_arr[i] = (son_s8 *)SON_STRDUP(p_token);

                    SON_ASSERT(SON_PNULL != p_str_arr[i]);
                    i++;
                }

                p_token = (son_s8 *)SON_STRTOK(SON_PNULL, p_str_delim);
            }

            p_str_arr[i] = SON_PNULL;

        }
        /* SPR 15935 Fix Start */
        if (SON_PNULL != p_loc_buf)
        {
            son_mem_free(p_loc_buf);
            p_loc_buf = SON_PNULL;
        }
        /* SPR 15935 Fix Stop */
    }
    /*Coverity Fix CID 66929 End*/

    return p_str_arr;
} /* get_string */

/*****************************************************************************
 * Function Name  : user_get_file_len
 * SPR-612 Coverity Fix Starts
 * Inputs         : None.
 * SPR-612 Coverity Fix Ends
 * Outputs        : None.
 * Returns        : Number of bytes present in the file
 * Description    : This function finds the size of file.
 ****************************************************************************/
static son_u32
user_get_file_len
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
 /* SPR-612 Coverity Fix Starts */
)
/* SPR-612 Coverity Fix Ends */
{
    FILE     *p_fd   = SON_PNULL;
    son_u32 flen    = 0;
    son_s32 ret = 0;

    if (SON_PNULL != p_file_name)
    {
        p_fd = SON_FOPEN(p_file_name, (const son_s8 *)"r");

        if (SON_PNULL == p_fd)
        {
            /* SPR-442 Fix Starts */
            fprintf(stderr, "Unable to open configuration file %s\n", (son_8 *)p_file_name); /*Klocwork Fix Start-End*/
            son_abort();
            /* SPR-442 Fix Ends */
        }
        else
        {
            /* Coverity CID 100109 Fix Start */
            if(0!= SON_FSEEK(p_fd, 0, SEEK_END))
            {   
                fprintf(stderr, "Error in fseek %s\n",
                        p_file_name);
                exit(0);
            }
            /* Coverity CID 100109 Fix End */

            ret = SON_FTELL(p_fd);
            if (0 >= ret)
            {
                /* Either file length is 0 or stream is invalid */
                fprintf(stderr, "Unable to find length of file %s\n",
                        p_file_name);
            }
            else
            {
                flen = (son_u32)ret;
            }
         /* Coverity CID 100109 Fix Start */ 
           if(0!= SON_FSEEK(p_fd, 0, SEEK_SET))
            {
                fprintf(stderr, "Error in fseek %s\n",
                        p_file_name);
                exit(0);
            }
         /* Coverity CID 100109 Fix End */ 
            SON_FCLOSE(p_fd);
        }
    }

    return flen;
} /* user_get_file_len */

/******************************************************************************
 * Function Name  : get_string_buff
 * SPR-612 Coverity Fix Starts
 * Inputs         : None.
 * SPR-612 Coverity Fix Ends
 * Outputs        : None.
 * Returns        : Pointer to string buffer.
 * Description    : This function reads the content of the file.
 ******************************************************************************/
static son_s8 *
get_string_buff
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
 /* SPR-612 Coverity Fix Starts */
)
/* SPR-612 Coverity Fix Ends */
{
    FILE *p_fd      = SON_PNULL;
    son_u32 flen      = 0;
    son_s8 *p_buf     = SON_PNULL;

    if (SON_PNULL != p_file_name)
    {
        /* SPR-612 Coverity Fix Starts */
        flen = user_get_file_len();
        /* SPR-612 Coverity Fix Ends */
        if (0 == flen)
        {
            fprintf(stderr, "Configuration file length is zero.\n");
            son_abort();
        }
        else
        {
            p_fd = SON_FOPEN(p_file_name, (const son_s8 *)"r");

            if (SON_PNULL == p_fd)
            {
                /* SPR-442 Fix Starts */
                fprintf(stderr, "Unable to open configuration file %s.\n", (son_8 *)p_file_name); /*Klocwork Fix Start-End*/
                son_abort();
                /* SPR-442 Fix Ends */
            }
            else
            {
                p_buf = (son_s8 *)son_mem_get(flen + 1);
                if (SON_PNULL != p_buf)
                {
                    flen = (son_u32)SON_FREAD(p_buf, 1, flen, p_fd);
                    p_buf[flen] = 0;
                }

                SON_FCLOSE(p_fd);
            }
        }
    }

    return p_buf;
} /* get_string_buff */

/*****************************************************************************
 * Function Name  : read_config_from_file
 * SPR-612 Coverity Fix Starts
 * Inputs         : None.
 * SPR-612 Coverity Fix Ends
 * Outputs        : None.
 * Returns        : Pointer to pointer to string buffer.
 * Description    : This function reads the content of the file and returns
 *                : the pointer to pointer to the strings buffer after
 *                : removing commented lines.
 ****************************************************************************/
static son_s8 * *
read_config_from_file
(
/* + SPR 17439 */
    void
/* - SPR 17439 */
 /* SPR-612 Coverity Fix Starts */
 /* Parameter is deleted */
 /* SPR-612 Coverity Fix Ends */
)
{
    son_s8 *p_str_buf  = SON_PNULL;
    son_s8 * *p_str_arr = SON_PNULL;

    /* SPR-612 Coverity Fix Starts */
    p_str_buf = get_string_buff();
    /* SPR-612 Coverity Fix Ends */
    if (SON_PNULL != p_str_buf)
    {
        p_str_arr = get_string(p_str_buf, (const son_s8 *)"\n", '#');
        son_mem_free(p_str_buf);
    }

    return p_str_arr;
}

/*****************************************************************************
 * Function Name  : is_key_present
 * Inputs         : p_keyName - Pointer to key value to be searched.
 *                  p_string  - Pointer to target string.
 * Outputs        : None.
 * Returns        : Pointer to the key value.
 * Description    : This function searches the key value from the target string
 ****************************************************************************/
static son_s8 *
is_key_present
(
 son_s8 *p_keyName,
 son_s8 *p_string
 )
{
    if ((SON_PNULL != p_keyName) && (SON_PNULL != p_string))
    {
        son_u32 keyLen = (son_u32)SON_STRLEN((const son_s8 *)p_keyName);
        if (0 == SON_STRNCMP((son_s8 *)p_keyName, (son_s8 *)p_string, keyLen))
        {
            if ('=' == p_string[keyLen])
            {
                /* Returning position for Key Value */
                return p_string + keyLen + 1;
            }
        }
    }

    return SON_PNULL;
}

/*****************************************************************************
 * Function Name  : free_all_strings
 * Inputs         : p_p_strings - Pointer to pointer to strings.
 * Outputs        : None
 * Returns        : None
 * Description    : This function frees all the memory taken for reading
 *                  configuration.
 ****************************************************************************/
static son_void_t
free_all_strings
(
 son_s8 * *p_p_strings
 )
{
    son_u32 counter = 0;

    if (SON_PNULL != p_p_strings)
    {
        if (SON_PNULL != *p_p_strings)
        {
            for (counter = 0; p_p_strings[counter]; counter++)
            {
                SON_FREE(p_p_strings[counter]);
            }
        }

        son_mem_free(p_p_strings);
    }
} /* free_all_strings */

/*****************************************************************************
 * Function Name  : read_config
 * SPR-612 Coverity Fix Starts
 * Inputs         : p_oam_son_stack_address - Pointer to oam stack address
 * SPR-612 Coverity Fix Ends
 *                : p_rrm_son_stack_address - Pointer to rrm_son stack address
 *                : p_rrc_son_stack_address - Pointer to rrc_son stack address
 *                : p_phy_son_stack_address - Pointer to phy_son stack address
 *                : p_son_oam_stack_address - Pointer to son_oam stack address
 *                : p_son_rrm_stack_address- Pointer to son_rrm stack address
 *                : p_son_rrc_stack_address- Pointer to son_rrc stack address
 *                : p_son_phy_stack_address- Pointer to son_phy stack address
 *                : p_son_log_filename- Pointer to son log file
 * Outputs        : None
 * Returns        : None
 * Description    : This function reads the OAM, RRM and PHY configuration
 *                : from the file.
 ****************************************************************************/
static son_void_t
read_config
(
 /* SPR-612 Coverity Fix Starts */
 /* Parameter is deleted */
 /* SPR-612 Coverity Fix Ends */
 son_s8 *p_oam_son_stack_address,
 son_s8 *p_rrm_son_stack_address,
 son_s8 *p_rrc_son_stack_address,
 son_s8 *p_phy_son_stack_address,
 son_s8 *p_son_oam_stack_address,
 son_s8 *p_son_rrm_stack_address,
 son_s8 *p_son_rrc_stack_address,
 son_s8 *p_son_phy_stack_address
   /*SPR 21881 +-*/
 )
{
    son_s32 i = 0;
    /* SPR-612 Coverity Fix Starts */
    son_s8 **p_p_strings = read_config_from_file();
    /* SPR-612 Coverity Fix Ends */
    son_s8 *p_val = SON_PNULL;
    son_s8 seperator[] = ": ";
    son_size_t seperator_len = SON_STRLEN(seperator);

    if (SON_PNULL != p_p_strings)
    {
        for (i = 0; p_p_strings[i]; i++)
        {
            /* Reading OAM-SON IP ADDRESS and PORT */
            p_val = is_key_present((son_s8 *)"ENB_OAM_IP_ADDRESS", p_p_strings[i]);
            if (SON_PNULL != p_val)
            {
                SON_MEMCPY(p_oam_son_stack_address, p_val,
                        SON_STRLEN((const son_s8 *)p_val));
                p_oam_son_stack_address[SON_STRLEN((const son_s8 *)p_val)] = 0;
            }

            p_val = is_key_present((son_s8 *)"ENB_OAM_RX_PORT", p_p_strings[i]);
            if (SON_PNULL != p_val)
            {
                SON_STRNCAT((son_s8 *)p_oam_son_stack_address, seperator,
                        seperator_len);
                SON_STRNCAT((son_s8 *)p_oam_son_stack_address, p_val,
                        SON_STRLEN(p_val));
            }

            /* Reading RRM-SON IP ADDRESS and PORT */
            p_val = is_key_present((son_s8 *)"ENB_RRM_IP_ADDRESS", p_p_strings[i]);
            if (SON_PNULL != p_val)
            {
                SON_MEMCPY(p_rrm_son_stack_address, p_val,
                        SON_STRLEN((const son_s8 *)p_val));
                p_rrm_son_stack_address[SON_STRLEN((const son_s8 *)p_val)] = 0;
            }

            p_val = is_key_present((son_s8 *)"ENB_RRM_RX_PORT", p_p_strings[i]);
            if (SON_PNULL != p_val)
            {
                SON_STRNCAT((son_s8 *)p_rrm_son_stack_address, seperator,
                        seperator_len);
                SON_STRNCAT((son_s8 *)p_rrm_son_stack_address, p_val,
                        SON_STRLEN(p_val));
            }

            /* Reading RRC-SON IP ADDRESS and PORT */
            p_val = is_key_present((son_s8 *)"ENB_L3_IP_ADDRESS", p_p_strings[i]);
            if (SON_PNULL != p_val)
            {
                SON_MEMCPY(p_rrc_son_stack_address, p_val,
                        SON_STRLEN((const son_s8 *)p_val));
                p_rrc_son_stack_address[SON_STRLEN((const son_s8 *)p_val)] = 0;
            }

            p_val = is_key_present((son_s8 *)"ENB_L3_PORT", p_p_strings[i]);
            if (SON_PNULL != p_val)
            {
                SON_STRNCAT((son_s8 *)p_rrc_son_stack_address, seperator,
                        seperator_len);
                SON_STRNCAT((son_s8 *)p_rrc_son_stack_address, p_val,
                        SON_STRLEN(p_val));
            }

            /* Reading PHY-SON IP ADDRESS and PORT */
            p_val = is_key_present((son_s8 *)"L1_IP_ADDRESS_00", p_p_strings[i]);
            if (SON_PNULL != p_val)
            {
                SON_MEMCPY(p_phy_son_stack_address, p_val,
                        SON_STRLEN((const son_s8 *)p_val));
                p_phy_son_stack_address[SON_STRLEN((const son_s8 *)p_val)] = 0;
            }

            p_val = is_key_present((son_s8 *)"SON_L1_CONTROL_RX_PORT",
                    p_p_strings[i]);
            if (SON_PNULL != p_val)
            {
                SON_STRNCAT((son_s8 *)p_phy_son_stack_address, seperator,
                        seperator_len);
                SON_STRNCAT((son_s8 *)p_phy_son_stack_address, p_val,
                        SON_STRLEN(p_val));
            }

            /* Reading SON-OAM IP ADDRESS and PORT */
            /*SPR 21881 Start*/
            p_val = is_key_present((son_s8 *)"ENB_SON_IP_ADDRESS", p_p_strings[i]);
            /*SPR 21881 End*/
            if (SON_PNULL != p_val)
            {
                SON_MEMCPY(p_son_oam_stack_address, p_val,
                        SON_STRLEN((const son_s8 *)p_val));
                p_son_oam_stack_address[SON_STRLEN((const son_s8 *)p_val)] = 0;
            }

            /*SPR 21881 Start*/
            p_val = is_key_present((son_s8 *)"ENB_SON_OAM_RX_PORT", p_p_strings[i]);
            /*SPR 21881 End*/
            if (SON_PNULL != p_val)
            {
                SON_STRNCAT((son_s8 *)p_son_oam_stack_address, seperator,
                        seperator_len);
                SON_STRNCAT((son_s8 *)p_son_oam_stack_address, p_val,
                        SON_STRLEN(p_val));
            }

            /* Reading SON-RRM IP ADDRESS and PORT */
            /*SPR 21881 Start*/
            p_val = is_key_present((son_s8 *)"ENB_SON_IP_ADDRESS", p_p_strings[i]);
            /*SPR 21881 End*/
            if (SON_PNULL != p_val)
            {
                SON_MEMCPY(p_son_rrm_stack_address, p_val,
                        SON_STRLEN((const son_s8 *)p_val));
                p_son_rrm_stack_address[SON_STRLEN((const son_s8 *)p_val)] = 0;
            }

            /*SPR 21881 Start*/
            p_val = is_key_present((son_s8 *)"ENB_SON_RRM_RX_PORT", p_p_strings[i]);
            /*SPR 21881 End*/
            if (SON_PNULL != p_val)
            {
                SON_STRNCAT((son_s8 *)p_son_rrm_stack_address, seperator,
                        seperator_len);
                SON_STRNCAT((son_s8 *)p_son_rrm_stack_address, p_val,
                        SON_STRLEN(p_val));
            }

            /* Reading SON-RRC IP ADDRESS and PORT */
            /*SPR 21881 Start*/
            p_val = is_key_present((son_s8 *)"ENB_SON_IP_ADDRESS", p_p_strings[i]);
            /*SPR 21881 End*/
            if (SON_PNULL != p_val)
            {
                SON_MEMCPY(p_son_rrc_stack_address, p_val,
                        SON_STRLEN((const son_s8 *)p_val));
                p_son_rrc_stack_address[SON_STRLEN((const son_s8 *)p_val)] = 0;
            }

            /*SPR 21881 Start*/
            p_val = is_key_present((son_s8 *)"ENB_SON_RRC_RX_PORT", p_p_strings[i]);
            /*SPR 21881 End*/
            if (SON_PNULL != p_val)
            {
                SON_STRNCAT((son_s8 *)p_son_rrc_stack_address, seperator,
                        seperator_len);
                SON_STRNCAT((son_s8 *)p_son_rrc_stack_address, p_val,
                        SON_STRLEN(p_val));
            }

            /* Reading SON-PHY IP ADDRESS and PORT */
            /*SPR 21881 Start*/
            p_val = is_key_present((son_s8 *)"ENB_SON_IP_ADDRESS", p_p_strings[i]);
            /*SPR 21881 End*/
            if (SON_PNULL != p_val)
            {
                SON_MEMCPY(p_son_phy_stack_address, p_val,
                        SON_STRLEN((const son_s8 *)p_val));
                p_son_phy_stack_address[SON_STRLEN((const son_s8 *)p_val)] = 0;
            }

            p_val = is_key_present((son_s8 *)"SON_L1_CONTROL_TX_PORT",
                    p_p_strings[i]);
            if (SON_PNULL != p_val)
            {
                SON_STRNCAT((son_s8 *)p_son_phy_stack_address, seperator,
                        seperator_len);
                SON_STRNCAT((son_s8 *)p_son_phy_stack_address, p_val,
                        SON_STRLEN(p_val));
            }

           /*SPR 21881 +-*/
        }

        free_all_strings(p_p_strings);
    }
    else
    {
        fprintf(stderr, "Either configuration file is not available"
                "or the file is empty\n");
        son_abort();
    }
} /* read_config */

#ifdef SON_MRO_FEATURE

/*****************************************************************************
 * Function Name  : son_mro_fill_mro_param_default_values
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function initializes the MRO global context
 *                  configuration parameters with default values.
 ******************************************************************************/
static son_void_t
son_fill_mro_param_default_values
(
 son_mro_file_config_param_t *p_mro_config_param
 )
{
    SON_UT_TRACE_ENTER();

    p_mro_config_param->mro_mode = SON_MODE_DISTRIBUTED;
    p_mro_config_param->mro_param = SON_MRO_CIO;
    p_mro_config_param->mro_ho_failure_accumulation_duration =
        SON_MRO_DEFAULT_HO_FAILURE_ACCUMULATION_DURATION;
    p_mro_config_param->mro_ho_attempts_threshold =
        SON_MRO_DEFAULT_HO_ATTEMPTS_THRESHOLD;
    p_mro_config_param->ttt = SON_TTT_MS_512;
    p_mro_config_param->delta_cio = SON_MRO_DEFAULT_DELTA_CIO;
    p_mro_config_param->delta_ttt = SON_MRO_DEFAULT_DELTA_TTT;
    p_mro_config_param->min_ttt = SON_TTT_MS_0;
    p_mro_config_param->max_ttt = SON_TTT_MS_5120;
    p_mro_config_param->min_cio = SON_Q_OFFSET_RANGE_DB_24;
    p_mro_config_param->max_cio = SON_Q_OFFSET_RANGE_DB24;
    p_mro_config_param->mro_ho_optimization_start_threshold =
        SON_MRO_DEFAULT_HO_OPTIMIZATION_START_THRESHOLD * MRO_SCALE_FACTOR;
    p_mro_config_param->mro_ho_optimization_stop_threshold =
        SON_MRO_DEFAULT_HO_OPTIMIZATION_STOP_THRESHOLD * MRO_SCALE_FACTOR;
    p_mro_config_param->mro_ho_fail_rate_for_optimization =
        SON_MRO_DEFAULT_HO_FAIL_RATE_FOR_OPTIMIZATION * MRO_SCALE_FACTOR;
    p_mro_config_param->mro_expected_ho_improvement_for_single_step =
        SON_MRO_DEFAULT_EXPECTED_HO_IMPROVEMENT_FOR_SINGLE_STEP * MRO_SCALE_FACTOR;

    SON_UT_TRACE_EXIT();
} /* son_fill_mro_param_default_values */

#endif /* ifdef SON_MRO_FEATURE */

#ifdef SON_IN_MEM_LOGGER
/*****************************************************************************
 * Function Name  : son_fill_logger_cfg_param_values
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function assigns default values to the Logger 
 *                  configuration params
 ******************************************************************************/
static son_void_t
son_fill_logger_cfg_param_values
(
 son_logger_cfg_t *p_logger_config_param
 )
{
    SON_UT_TRACE_ENTER();

    p_logger_config_param->log_wait_interval = 2;
    p_logger_config_param->log_path[0] = '.';
    p_logger_config_param->log_shm_size = 5;
    p_logger_config_param->log_file_sz = 10;
    p_logger_config_param->log_chunk_cnt = 10;
    p_logger_config_param->log_cpu_bitmap = 1;
    p_logger_config_param->log_file_cnt = 10;

    SON_UT_TRACE_EXIT();
} /* son_fill_logger_cfg_param_values */

#endif /* ifdef SON_IN_MEM_LOGGER */
/*SRVCC-Start*/

/*****************************************************************************
 * Function Name  : son_fill_anr_default_param_values
 * Inputs         : None
 * Outputs        : None
 * Returns        : son_void_t
 * Description    : This function assigns default values to the ANR
 *                  configuration params
 ******************************************************************************/
static son_void_t
son_fill_anr_default_param_values
(
 son_anr_config_param_t *p_anr_config_param
 )
{
    SON_UT_TRACE_ENTER();

    p_anr_config_param->anr_ps_ho_supported = SON_TRUE;
    p_anr_config_param->anr_voip_capable = SON_TRUE;
    p_anr_config_param->anr_max_num_cgi_request = 3; /*Added for SPR-9759*/
    /* Rel 3.0 Multi Sector code changes Start */
    p_anr_config_param->anr_auto_maintain_local_cell_as_nbr = 
        SON_TRUE;
    /* Rel 3.0 Multi Sector code changes Stop */

    SON_UT_TRACE_EXIT();
} /* son_fill_anr_default_param_values */


/*SRVCC-End*/
/*****************************************************************************
 * Function Name  : main
 * Inputs         : None
 * Outputs        : None
 * Returns        : SON_FAILURE - Indicates failure
 *                  SON_SUCCESS - Indicates success
 * Description    : main function of the SON binary. It initializes the SON
 *                  binary with the CSPL.
 ****************************************************************************/
int
main
(
 int argc,
 char * *argv
 )
{
    pthread_key_create(&cell_and_ue_index_key, free_cell_and_ue_index);
    SET_CELL_AND_UE_INDEX(0xff, 0xffff);


	
#ifdef LINUX_PC_TEST
	adapter_init_socket();
	TestInitFrame("192.168.1.120", 45499);
	TestFrameRegisterMsgHandler(adapter_hdl_msg_callback);
#endif

#ifdef SON_IN_MEM_LOGGER
    son_thrd_arg_t      thd_arg;
#endif
    son_s8 oam_son_stack_address[IP_PORT_STR_LEN] = {0};
    son_s8 rrm_son_stack_address[IP_PORT_STR_LEN] = {0};
    son_s8 rrc_son_stack_address[IP_PORT_STR_LEN] = {0};
    son_s8 phy_son_stack_address[IP_PORT_STR_LEN] = {0};
    son_s8 son_oam_stack_address[IP_PORT_STR_LEN] = {0};
    son_s8 son_rrm_stack_address[IP_PORT_STR_LEN] = {0};
    son_s8 son_rrc_stack_address[IP_PORT_STR_LEN] = {0};
    son_s8 son_phy_stack_address[IP_PORT_STR_LEN] = {0};
    /*Initialising CSPL*/
    qvSimpleInit(&son_os);

    /* PR 7715 Fix Start */
    /* Read IP and Port configuration from file */
    /* SPR-442 Fix Starts */
    /* SPR-612 Coverity Fix Starts */
    if ((argc > 2) && SON_PNULL != argv[2])
    {
        p_file_name = (son_u8 *)argv[2];
    }
    else
    {
        /* SPR-442 Fix Starts */
        fprintf(stderr, "Configuration file name is not given as the "
                "command line argument as expected (./lte_son <core num "
                "to bind> <configuration file path>) so using default "
                "configuraiton file %s and port.\n", CONFIG_FILE);
        /* SPR-442 Fix Ends */
        p_file_name = (son_u8 *)CONFIG_FILE;
    }

    read_config(oam_son_stack_address,
            rrm_son_stack_address, rrc_son_stack_address,
            phy_son_stack_address, son_oam_stack_address,
            son_rrm_stack_address, son_rrc_stack_address,
   /*SPR 21881 Start*/
            son_phy_stack_address);
   /*SPR 21881 End*/
    /* SPR-612 Coverity Fix Ends */
    /* SPR-442 Fix Ends */

#if (SON_ADV_LOGGER == LOGGER_TYPE)

    /* Initialize the advance logger and set its level to SON_ERROR for
     * any log for common code (not specific to any module). */
/* coverity_89312 start */
   /*SPR 21881 Start*/
   if(LogSuccess != init_log(SON_LOG_CTXT_FILE,SON_DEFAULT_LOG_FILE, &g_main_advl_ctxt))
   /*SPR 21881 End*/
    {
	    return 1;
    }
/* coverity_89312 stop */
    set_loglevel_limit(g_main_advl_ctxt, SON_ERROR);

#elif (SON_INTERNAL_LOGGER == LOGGER_TYPE)

    /* Set global log level to SON_LOG_LEVEL_NONE so that CSPL does not
     * print any log for common code (not specific to any module). This
     * code report any error using SON_ENCAPS_WARNING and  SON_PANIC*/
    qvLogLevel(SON_LOG_LEVEL_NONE);
#endif
    /* PR 7715 Fix End */
    /*KLOCKWORK:Fix Start*/
    /* SPR-442 Fix Starts */
    if ((argc > 1) && (SON_PNULL != argv[1]))
    {
        son_u32 core_id = son_atoi(argv[1]);
        cpu_set_t   cpuSetSch;

        if (core_id >= (SON_EIGHT * sizeof(cpuSetSch)))
        {
            SON_PANIC("Invalid core number[%d] is passed.", core_id);
            return 1;/*not using the son failure and success defined for rest*/
        }
        SON_CPU_ZERO(&cpuSetSch);
        SON_CPU_SET(core_id ,&cpuSetSch);
        pid_t p = 0;
        if (0 != son_sched_setaffinity(p, sizeof(cpu_set_t), &cpuSetSch))
        {
            SON_PANIC("sched_setaffinity Returned Error in SON. %s Core not Binded", argv[1]);
            return 1;/*not using the son failure and success defined for rest*/
        }
        /*KLOCKWORK:Fix end*/
    }
    else
    {
        SON_ENCAPS_WARNING("lte_son binary will bind to default core since the "
                "command line argument is not given as expected (./lte_son "
                "<core num to bind> <configuration file path>)");
    }
    /* SPR-442 Fix Ends */

    /* Copy SON IP address and Port number towards OAM,RRM and PHY*/
    SON_MEMCPY(son_oam_addr, son_oam_stack_address, IP_PORT_STR_LEN);
    SON_MEMCPY(son_rrm_addr, son_rrm_stack_address, IP_PORT_STR_LEN);
    SON_MEMCPY(son_rrc_addr, son_rrc_stack_address, IP_PORT_STR_LEN);
    SON_MEMCPY(son_phy_addr, son_phy_stack_address, IP_PORT_STR_LEN);

    /* Creating Listener */
    p_listener = qvOpen(&son_shell, SON_PNULL);
    if (SON_PNULL == p_listener)
    {
        /* Couldn't initialize connection to external entities */
        SON_PANIC("qvOpen failed");
        return 1; /*not using the son failure and success defined for rest*/
    }

    /*Specifying external modules with which this process can communicate*/
    qvSetDestination(SON_RRM_MODULE_ID, &son_shell, p_listener,
            rrm_son_stack_address);
    qvSetDestination(SON_RRC_MODULE_ID, &son_shell, p_listener,
            rrc_son_stack_address);
    qvSetDestination(SON_PHY_MODULE_ID, &son_shell, p_listener,
            phy_son_stack_address);
    qvSetDestination(SON_OAM_MODULE_ID, &son_shell, p_listener,
            oam_son_stack_address);

#ifdef SON_MRO_FEATURE
    son_fill_mro_param_default_values(&g_config_params.mro_config_params);
#endif

#ifdef SON_IN_MEM_LOGGER
    son_fill_logger_cfg_param_values(&g_config_params.son_logger_cfg);
#endif

    /* Set default values for the config params which should be used in case 
       the parameters are not found in the config file (son_oam.cfg) */ 
    /*SRVCC-Start*/
    son_fill_anr_default_param_values(&g_config_params.anr_config_params);
    /*SRVCC-End*/
    g_config_params.rach_config_params.read_only_config_params.
        rachopt_strong_cell_count = 0xFF;
    g_config_params.rach_config_params.read_only_config_params.
        rachopt_no_of_windows_to_skip_for_optimization = 0xFF;
    g_config_params.rach_config_params.read_only_config_params.
        rachopt_grpAConfig_vs_grpAobsrvd_diff_threshold = 0xFF;
    g_config_params.rach_config_params.read_only_config_params.
        rachopt_cfra_unused_threshold = 0xFF;
    g_config_params.rach_config_params.rachopt_mode = RACH_OPT_DEFAULT_MODE;

    if (SON_SUCCESS != parse_and_store_son_oam_cfg_params())
    {
        SON_ENCAPS_WARNING("main: Error in Filling params from config file");
    }

    /*Register CSPL modules with this process*/
#ifdef SON_IN_MEM_LOGGER
    thd_arg.driver_id = qvNewDriver();
    thd_arg.p_qwait = &son_waitstruct;
    thd_arg.logical_thid = SON_ONE;
/*SPR 17777 +-*/ 

    son_register_modules(&thd_arg);
    lgr_logger_register_thread(SON_NULL);
#else
    son_register_modules();
/*SPR 17777 +-*/ 
#endif

    /*Start CSPL scheduler*/
    qvRun(&son_shell, p_listener, &son_waitstruct, 0);

    /* Close all the sockets and free the listener structure memory
     *  before terminating */
    qvClose(&son_shell, p_listener);

    return 0;
} /* main */
