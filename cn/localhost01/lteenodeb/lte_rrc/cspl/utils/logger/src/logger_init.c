/********************************************************************************      
*********************************************************************************
**      FILE NAME:
**              logger_init.c
**
**      DESCRIPTION:
**              This file contains the initialisation fns for logger glob data.
**
**
**      DATE                    AUTHOR                          REF
        REASON
**      ----                    ------                          ---
        ------
**      22Jan2003               svaid                           ---
**
**       Copyright (C) 2006 Aricent Inc . All Rights Reserved
******************************************************************************/

#include "logger.h"
#include "logger_init.h"

logger_glob_data_t		*p_glob_data;
U8				gb_already_read=0;
extern 			logger_trace_info_t		trace_info;

#define CONFIG_DATA_READING_FAILURE(ecode)        {\
	switch (ecode)                            \
        {                                     \
            case CONF_ERR_BAD_FILE:           \
                    printf("BAD CONFIG FILE NAME \n");\
                    break;                                 \
            case CONF_ERR_ZERO_SECTION:                    \
                    printf("SECTION DOES NOT CONTAIN ANY DATA\n"); \
                    break;                                 \
            case CONF_ERR_SECTION_NOT_FOUND:               \
                    printf("SECTION NOT FOUND IN CONFIG FILE\n");\
                    break;                                 \
            case CONF_ERR_SECTION_TOO_LONG:                \
                    printf("SECTION LENGTH IS TOO LONG\n");\
                    break;                                 \
            default:                                       \
                    printf("ERROR!!: Unrecognized error code returned to \
                                get_conf_info function from conf_get_section \
                                function \n");            \
                    break;                                 \
        }                                                  \
}

void upper_string(S8 *in)
{
	S8	*temp,*start;
	S16	len;

	len = strlen(in);
	temp = (S8 *)malloc(len);
	strcpy(temp, in);
	start = temp;

	for(; *temp != '\0';*(in++) = toupper(*(temp++)));
	*in = '\0';

	free(start);
} 

U16
get_index_after_adjusting_space
(
	S16	index
)
{
	U16	x_start,x_pos;
	
	x_start = LOGGER_TIMESTAMP_LEN + 1;
	if ( index == 0)
	{
		return x_start;
	}
	else
	{
		switch(cfg_params.num_modules)
		{
			case 2:	
				x_pos = (x_start + MAX_API_NAME) * index;	
				break;
			case 3:		
				x_pos = x_start + (MAX_MOD_NAME * cfg_params.num_modules * index);	
				break;
			case 4:	
				x_pos = x_start + (MAX_MOD_NAME * 2 * index);	
				break;
			case 5:
			case 6:
				x_pos = x_start + (MAX_MOD_NAME + DELTASPACE) * index;	
				break;
			default:	
				x_pos = x_start + MAX_MOD_NAME * index;	
		}
	}
	return x_pos;			
}

void
logger_set_space_and_hyphen_blks
(
	void
)
{
	S16	i,max_iter;
	
	switch( cfg_params.num_modules )
	{
		case 2:	max_iter = MAX_API_NAME - 1;
			break;
		case 3: max_iter = (MAX_MOD_NAME * 3) - 1;
			break;
		case 4: max_iter = (MAX_MOD_NAME * 2) - 1;
			break;
		case 5:
		case 6: max_iter = (MAX_MOD_NAME + DELTASPACE) -1;
			break;
		default:
			max_iter = MAX_MOD_NAME - 1;
						
	}
	for(i=0;i<max_iter;i++)
	{
		trace_info.space_str[i] = ' ';
		trace_info.hyphen_str[i] = '-';
	}
	trace_info.space_str[i] = '\0';
	trace_info.hyphen_str[i] = '\0';
}


void 
get_mod_info
(
	conf_section_t	*p_sec
)
{
	
	logger_glob_data_t	*ptr=NULL;
	S16			i;

	tokenise(p_sec);
	
	cfg_params.num_modules = p_sec->count;
	printf("Number of Modules %d\n",cfg_params.num_modules);

	if((cfg_params.num_modules < LOGGER_MIN_MODULES) || (cfg_params.num_modules > LOGGER_MAX_MODULES))
	{
		printf("Number of Modules to be logged are %d out of range\n",cfg_params.num_modules);
		printf("Logger Exiting\n");
		exit(0);
	}
		
	p_glob_data = (logger_glob_data_t *) malloc( sizeof(logger_glob_data_t) * cfg_params.num_modules );
	ptr = p_glob_data;

	for(i=0;i<cfg_params.num_modules;i++)
	{
		ptr->mod_info.mod_id = string_to_u16((U8 *) p_sec->line[i].token[0]);
		strcpy( ptr->mod_info.mod_name , (U8 *) p_sec->line[i].token[1] );
		upper_string(ptr->mod_info.mod_name);
		ptr++;
	}
}

logger_return_t
get_api_info_for_mod
(
	logger_glob_data_t	*ptr,
	S16			index
)
{
	conf_section_t	conf_section_1;
	U16		api_count,j;
	api_list_t		*ptr1=NULL;
	logger_return_t      	ecode= 0;
	
	if (LOGGER_FAILURE == conf_get_section(&conf_file, ptr->mod_info.mod_name, \
                                                      &conf_section_1, &ecode))
	{
		CONFIG_DATA_READING_FAILURE(ecode);
		return LOGGER_FAILURE;
	}

	tokenise(&conf_section_1);
	ptr->mod_info.num_apis = api_count = conf_section_1.count;
	printf("Mod id:%d Mod Name:%s Num APIs:%d\n",ptr->mod_info.mod_id, ptr->mod_info.mod_name, ptr->mod_info.num_apis);
	if( api_count != 0)
		ptr->api_info_present = 1;
	else
		ptr->api_info_present = 0;
	ptr->ladder_pos = index;
	ptr->cursor_x_pos = get_index_after_adjusting_space(index); 
	if(ptr->api_info_present)
	{
		ptr->api_list = (api_list_t *) malloc( sizeof(api_list_t) * api_count);
		ptr1 = ptr->api_list;
		for(j=0;j<api_count;j++)
		{
				ptr1->api_id = string_to_u16((U8 *) conf_section_1.line[j].token[0]);
				strcpy( ptr1->api_name, (U8 *) conf_section_1.line[j].token[1]);
				printf("API id:%d API Name:%s\n",ptr1->api_id, ptr1->api_name);
				ptr1++;
		}
	}
	else
	{
		ptr->api_list = NULL;
	}
	return LOGGER_SUCCESS;
}

logger_return_t
logger_init_glob_data
(
	void
)
{
	S16	i;
	logger_glob_data_t	*ptr=NULL;
	logger_return_t		ret=0;

	if (LOGGER_TRUE != gb_already_read)
	{
		get_conf_info();
		gb_already_read = LOGGER_TRUE;
	}

	ptr = p_glob_data;
	for(i=0;i<cfg_params.num_modules;i++)
	{
		ret = get_api_info_for_mod(ptr,i);
		if (ret == 0)
			break;
		ptr++;
	}
	logger_set_space_and_hyphen_blks();
	return ret;
}

