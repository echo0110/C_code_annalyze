/*********************************************************************************    
**********************************************************************************
**    FILE NAME:
**        logger_init.h
**
**    DESCRIPTION:	Contains all logger initialisation function 
**			prototypes 
**
**    DATE              AUTHOR              REF         REASON
**    ----              ------              ---         ------
**    06Feb03           svaid           ---         Original
**
**     Copyright (C) 2006 Aricent Inc . All Rights Reserved
*******************************************************************************/
#ifndef __LOGGER_INIT_H__
#define __LOGGER_INIT_H__

void get_mod_info(conf_section_t	*p_sec);
logger_return_t get_api_info_for_mod(logger_glob_data_t 	*ptr, S16 index);
void logger_set_space_and_hyphen_blks(void);
logger_return_t logger_init_glob_data( void);


#endif
