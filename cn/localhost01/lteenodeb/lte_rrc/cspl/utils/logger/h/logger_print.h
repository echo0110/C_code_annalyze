/*********************************************************************************    
**********************************************************************************
**    FILE NAME:
**        logger_print.h
**
**    DESCRIPTION:	Contains all logger print function prototypes
**
**    DATE              AUTHOR              REF         REASON
**    ----              ------              ---         ------
**    06Feb03           svaid           ---         Original
**
**     Copyright (C) 2006 Aricent Inc . All Rights Reserved
*******************************************************************************/
#ifndef __LOGGER_PRINT_H__
#define __LOGGER_PRINT_H__

void logger_print_and_log_to_file( S16 mod_index, S16 api_index, S16 src_index, S16 dst_index	);
void logger_get_api_traversal_info( U8	*p_buf);
S16 logger_search( S16 *search_mod_index, S16 *search_api_index, S16 *search_src_index, S16 *search_dst_index);

#endif
