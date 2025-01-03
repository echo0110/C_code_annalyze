/********************************************************************************      
*********************************************************************************
**      FILE NAME:
**              logger_print.c
**
**      DESCRIPTION:
**              This file contains the printing stuff for Logs generated by logger.
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

logger_api_traversal_info_t	api_traversal_info;
logger_trace_info_t		trace_info;
U32				line_count=0;

void lower_string(S8 *in)
{
	S8		*temp,*start;
	S16 	len;

	len = strlen(in);
	temp = (S8 *)malloc(len);
	strcpy(temp, in);
	start = temp;

	for(; *temp != '\0';*(in++) = tolower(*(temp++)));
	*in = '\0';

	free(start);
} 

void
logger_get_api_traversal_info
(
	U8	*p_buf
)
{

	api_traversal_info.api_id = p_buf[0];
	api_traversal_info.api_id = api_traversal_info.api_id << 8;
	api_traversal_info.api_id |= p_buf[1];
	
	api_traversal_info.src_id = p_buf[2];
	api_traversal_info.src_id = api_traversal_info.src_id << 8;
	api_traversal_info.src_id |= p_buf[3];

	api_traversal_info.dst_id = p_buf[4];
	api_traversal_info.dst_id = api_traversal_info.dst_id << 8;
	api_traversal_info.dst_id |= p_buf[5];

	printf("API %d travelling from SRC %d to DST %d\n",api_traversal_info.api_id, api_traversal_info.src_id, api_traversal_info.dst_id);

}

S16
logger_search
(
	S16 *search_mod_index,
	S16 *search_api_index,
	S16 *search_src_index,
	S16 *search_dst_index
)
{
	U16	mod_id,other_mod_id;
	S16	situation=1,*search_other_mod_index;
	S16 	i,other_mod_index=-1;
	logger_glob_data_t	*ptr=NULL;
	api_list_t		*ptr1=NULL;

	*search_mod_index = -1;
	*search_api_index = -1;
	*search_src_index = -1;
	*search_dst_index = -1;
	search_other_mod_index = &other_mod_index;

	if( sender == api_traversal_info.src_id)
	{
		mod_id = api_traversal_info.src_id;
		other_mod_id = api_traversal_info.dst_id;
	}
	else if( sender == api_traversal_info.dst_id)
	{
		mod_id = api_traversal_info.dst_id;
		other_mod_id = api_traversal_info.src_id;
	}
	else
		printf("Module for API Undefined\n");

	ptr = p_glob_data;
	i=0;
	while( i < cfg_params.num_modules)
	{
		if( ptr->mod_info.mod_id == mod_id)
		{
			*search_mod_index = i;
			situation *= LOGGER_SUCCESS;
			printf("Module record for this API FOUND in Logger's global data at Mod Index %d\n",i);
			break;
		}
		else
		{
			ptr++;
			i++;
		}
	}
	if(*search_mod_index == -1)
	{
		printf("Module record for this API CANNOT BE FOUND in Logger's global data\n");
		situation *= LOGGER_FAILURE;
		return situation;
	}

	i=0;
	if(ptr->api_info_present)
	{
		ptr1 = ptr->api_list;
		while( i < ptr->mod_info.num_apis )
		{
			if( ptr1->api_id == api_traversal_info.api_id)
			{
				printf("API Info FOUND at API Index %d of this Module Record\n",i);
				situation *= LOGGER_SUCCESS;
				*search_api_index = i;
				break;
			}
			else
			{
				ptr1++;
				i++;
			}
		}
		if(*search_api_index == -1)
		{
			printf("API Info CANNOT be found in Module Record\n");
			situation *= LOGGER_FAILURE;
			return situation;
		}

	}
	
	ptr = p_glob_data;
	i=0;
	while( i < cfg_params.num_modules )
	{
		if( ptr->mod_info.mod_id == other_mod_id)
		{
			printf("Other Module Record FOUND in Logger's global data at Mod Index %d\n",i);
			situation *= LOGGER_SUCCESS;
			*search_other_mod_index = i;
			break;
		}
		else
		{
			ptr++;
			i++;
		}
	}
	if(*search_other_mod_index == -1)
	{
		printf("Module record for this API CANNOT BE FOUND in Logger's global data\n");
		situation *= LOGGER_FAILURE;
		return situation;
	}

	if( mod_id == api_traversal_info.src_id)
	{
		*search_src_index = *search_mod_index;
		*search_dst_index = *search_other_mod_index;
	}
	else if( mod_id == api_traversal_info.dst_id)
	{
		*search_src_index = *search_other_mod_index;
		*search_dst_index = *search_mod_index;
	}
	else
		printf("SRC and DST Record Info Not Found with Logger's global data\n");
	
	return situation;
}

static  void 
l_walltime
(
	QTIME *now
)
{
        struct timeval  T;

        gettimeofday( &T, 0 );
        now->s = T.tv_sec;
        now->us = T.tv_usec;
}

void
logger_put_timestamp
(
	S8	*timestamp_string	
)
{
	QTIME		t;
	struct tm	*log_tm;
	time_t		time_in_sec;

	l_walltime(&t);
	time_in_sec = t.s;
	log_tm = localtime((const time_t *)&time_in_sec);
	sprintf(timestamp_string, "[%02d/%02d/%02d %02d:%02d:%02d]   ", 
						log_tm->tm_mday,(log_tm->tm_mon + 1),(log_tm->tm_year + 1900),
						log_tm->tm_hour,log_tm->tm_min,log_tm->tm_sec);
}

void
logger_put_ladder
(
	S8	*ladder_string	
)
{
	S32	i;
	
	strcpy(ladder_string, "|");
	for(i=1; i<cfg_params.num_modules; i++)
	{
		strcat(ladder_string,trace_info.space_str);
		strcat(ladder_string, "|");
	}
}

void
form_string
(
	S16 num_args,...
)
{
	S8	*s=NULL;
	S8	*tmp_str,*temp;
	va_list	vptr;
	S16	tot_len=0,count;

	va_start(vptr, num_args);
	tmp_str = va_arg(vptr, S8 *);
	tot_len += strlen(tmp_str);

	for(count=1;count<num_args;count++)
	{
		tmp_str = va_arg(vptr , S8 *);
		tot_len += strlen(tmp_str);
	}
	va_end(vptr);
	
	s = (S8 *)malloc(tot_len+10);

	va_start(vptr, num_args);
	temp = va_arg(vptr, S8 *);
	strcpy( s, temp);
	free(temp);

	for(count=1;count<num_args;count++)
	{
		temp = va_arg(vptr , S8 *);
		strcat( s, temp);
		free(temp);
	}

	puts(s);
	printf("strlen s : %d\n",strlen(s));
	
	fprintf(p_log_file,"%s\n",s);
	fflush(p_log_file);

	free(s);
}

void
retrieve_logger_trace_info
(
	S16 mi,
	S16 ai,
	S16 si,
	S16 di
)
{
	logger_glob_data_t	*ptr=NULL;
	api_list_t		*ptr1=NULL;
	U16			ladder_pos_src,ladder_pos_dst,last_mod_index;
	S32			xtra;

	ptr = p_glob_data;
	ptr1 = (ptr + mi)->api_list;
	ladder_pos_src = (ptr + si)->ladder_pos;
	ladder_pos_dst = (ptr + di)->ladder_pos;
	last_mod_index = cfg_params.num_modules - 1;
	
	trace_info.succ_diff_cur_mod = (ptr + 1)->cursor_x_pos - ptr->cursor_x_pos;
	strcpy(trace_info.api_name, (ptr1 + ai)->api_name);
	lower_string(trace_info.api_name);
	trace_info.x_cur_pos_src = (ptr + si)->cursor_x_pos;
	trace_info.x_cur_pos_dst = (ptr + di)->cursor_x_pos;
	trace_info.start_cur = ptr->cursor_x_pos;
	trace_info.end_cur = (ptr + last_mod_index)->cursor_x_pos;
	if ( ladder_pos_src < ladder_pos_dst )
	{
		trace_info.x_diff_src_dst = trace_info.x_cur_pos_dst - trace_info.x_cur_pos_src;
		trace_info.arrow = '>';
		if((trace_info.x_cur_pos_src + strlen(trace_info.api_name)) < trace_info.end_cur)
			trace_info.api_start_cur = (trace_info.x_cur_pos_src - trace_info.start_cur) + 1;
		else
		{
			xtra = trace_info.x_cur_pos_src + strlen(trace_info.api_name) - trace_info.end_cur;
			trace_info.api_start_cur = trace_info.x_cur_pos_src - xtra - trace_info.start_cur;
		}
	}
	else if(ladder_pos_dst < ladder_pos_src)
	{
		trace_info.x_diff_src_dst = trace_info.x_cur_pos_src - trace_info.x_cur_pos_dst;
		trace_info.arrow = '<';
		if((trace_info.x_cur_pos_dst + strlen(trace_info.api_name)) < trace_info.end_cur)
			trace_info.api_start_cur = (trace_info.x_cur_pos_dst - trace_info.start_cur) + 1;
		else
		{
			xtra = trace_info.x_cur_pos_dst + strlen(trace_info.api_name) - trace_info.end_cur;
			trace_info.api_start_cur = trace_info.x_cur_pos_dst - xtra - trace_info.start_cur;
		}
	}
	else
	{
		printf("Direction of API traverse Undefined\n");
	}
}

void
xstrcpy
(
	S8	*t,
	S8	*s,
	U16	pos
)
{
	U16	i=0,j = pos;
	
	while(s[i] != '\0')
	{
		t[j] = s[i];
		j++;
		i++;
	}
}

void
logger_put_title_trace
(
	void
)
{
	record_title_t		r;
	S32			i,title_length,num_args=0,last_mod_index;
	logger_glob_data_t	*ptr=NULL;
	
	r.timestamp_string = (S8 *)malloc(LOGGER_TIMESTAMP_LEN);
	logger_put_timestamp(r.timestamp_string);
	num_args++;

	ptr = p_glob_data;
	last_mod_index = cfg_params.num_modules - 1;
	
	title_length = (trace_info.end_cur - trace_info.start_cur) + strlen((ptr + last_mod_index)->mod_info.mod_name);	
	r.mod_names = (S8 *)malloc(title_length);
	num_args++;

	strcpy(r.mod_names, " ");
	for(i=1;i<cfg_params.num_modules;i++)
	{
		strcat(r.mod_names, trace_info.space_str);
		strcat(r.mod_names, " ");
	}

	i=0;
	while(i<cfg_params.num_modules)
	{
		xstrcpy(r.mod_names, (ptr+i)->mod_info.mod_name, (ptr+i)->cursor_x_pos - trace_info.start_cur);
		i++;
	}

	form_string(num_args, r.timestamp_string, r.mod_names);
}

void
logger_put_empty_trace
(
	void
)
{
	record_ladder_t		r;
	S16			num_args=0,ladder_len;

	r.timestamp_string = (S8 *)malloc(LOGGER_TIMESTAMP_LEN);
	logger_put_timestamp(r.timestamp_string);
	num_args++;

	ladder_len = (trace_info.end_cur - trace_info.start_cur) + 1;			
	r.ladder_str = (S8 *)malloc(ladder_len);
	logger_put_ladder(r.ladder_str);
	num_args++;

	form_string(num_args, r.timestamp_string, r.ladder_str);
}

void
logger_put_api_trace
(
	void
)
{
	record_ladder_t	r;
	S32		num_args=0,ladder_len;

	r.timestamp_string = (S8 *)malloc(LOGGER_TIMESTAMP_LEN);
	logger_put_timestamp(r.timestamp_string);
	num_args++;

	ladder_len = (trace_info.end_cur - trace_info.start_cur) + 1;			
	r.ladder_str = (S8 *)malloc(ladder_len);
	logger_put_ladder(r.ladder_str);
	xstrcpy(r.ladder_str, trace_info.api_name, trace_info.api_start_cur);
	num_args++;

	form_string(num_args, r.timestamp_string, r.ladder_str);
	line_count++;
}

void
logger_put_arrow_trace
(
	void
)
{
	record_ladder_t		r;
	S16			num_args=0,ladder_len,num_hyphen_blks,save_pos,i;

	r.timestamp_string = (S8 *)malloc(LOGGER_TIMESTAMP_LEN);
	logger_put_timestamp(r.timestamp_string);
	num_args++;

	ladder_len = (trace_info.end_cur - trace_info.start_cur) + 1;			
	r.ladder_str = (S8 *)malloc(ladder_len);
	logger_put_ladder(r.ladder_str);
	num_args++;

	num_hyphen_blks = trace_info.x_diff_src_dst/trace_info.succ_diff_cur_mod;

	switch(trace_info.arrow)
	{
		case '>':
			save_pos = trace_info.x_cur_pos_src - trace_info.start_cur;
			for(i=0;i<num_hyphen_blks;i++)
			{
				xstrcpy(r.ladder_str, trace_info.hyphen_str, save_pos + 1);
				if( i != (num_hyphen_blks - 1))
				{
					save_pos += trace_info.succ_diff_cur_mod;
					xstrcpy(r.ladder_str, "-", save_pos);
				}
			}
			xstrcpy(r.ladder_str, ">", trace_info.x_cur_pos_dst - trace_info.start_cur - 1);
			break;
		case '<':
			save_pos = trace_info.x_cur_pos_dst - trace_info.start_cur;
			for(i=0;i<num_hyphen_blks;i++)
			{
				xstrcpy(r.ladder_str, trace_info.hyphen_str, save_pos + 1);
				if( i != (num_hyphen_blks - 1))
				{
					save_pos += trace_info.succ_diff_cur_mod;
					xstrcpy(r.ladder_str, "-", save_pos);
				}
			}
			xstrcpy(r.ladder_str, "<", trace_info.x_cur_pos_dst - trace_info.start_cur + 1);
			break;
		default:
	}
	
	form_string(num_args, r.timestamp_string, r.ladder_str);
}

void 
logger_print_and_log_to_file
(
	S16 mod_index,
	S16 api_index,
	S16 src_index,
	S16 dst_index	
)
{
	retrieve_logger_trace_info(mod_index, api_index, src_index, dst_index);
	if ( ( (line_count%20) == 0) || (line_count == 0) )
			logger_put_title_trace();
	logger_put_empty_trace();
	logger_put_api_trace();
	logger_put_arrow_trace();
}
