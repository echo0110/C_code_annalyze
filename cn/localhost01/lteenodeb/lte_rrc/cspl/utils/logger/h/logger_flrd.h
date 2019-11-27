/*****************************************************************************
*
* FILENAME      : logger_flrd.h
*
* DESCRIPTION   : This function contains the function prototype
*                 used in file reading module.
*
*
*
* DATE              NAME            REFERENCE           REASON
*		kumarp			original	
* 6th Feb2k3	svaid			Modified
*
*
*  Copyright (C) 2006 Aricent Inc . All Rights Reserved
*****************************************************************************/

#ifndef _LOGGER_FLRD_H_
#define _LOGGER_FLRD_H_

#define CONF_MAX_LINE_SIZE            200
#define CONF_MAX_LINES                200000
#define CONF_COMMENT_CHAR             '#'
#define CONF_MAX_LINES_PER_SECTION    1000
#define LEN_OF_TOKEN                  50
#define NO_OF_TOKEN                   10

 /*** CONF Errors ***/

#define CONF_ERR                   100

#define CONF_ERR_SECTION_NOT_FOUND (CONF_ERR+0x01)
#define CONF_ERR_BAD_FILE          (CONF_ERR+0x02)
#define CONF_ERR_SECTION_TOO_LONG  (CONF_ERR+0x03)
#define CONF_ERR_ZERO_SECTION      (CONF_ERR+0x04)

   
typedef struct
{
  U8 data[CONF_MAX_LINE_SIZE];
  U8 token[NO_OF_TOKEN][LEN_OF_TOKEN];
  U8 token_count;
} conf_line_t;

typedef struct
{
  conf_line_t  line[CONF_MAX_LINES];
  U32       count;
} conf_file_t;

typedef struct
{
  conf_line_t line[CONF_MAX_LINES_PER_SECTION];
  U32      count;
  U8       section_name[CONF_MAX_LINE_SIZE];
}conf_section_t;

void get_conf_info(void);
logger_return_t conf_get_conf_file(FILE* fp, conf_file_t* p_file);
void conf_print_conf(conf_file_t* p_file);
logger_return_t conf_get_section(conf_file_t* p_file, S8* sectionname,\
                                conf_section_t* p_section, \
                                logger_return_t*  p_err);
void conf_print_section(conf_section_t* p_sec);
void tokenise(conf_section_t *p_section);
extern S32 errno;
void tok_func(U8 *src, U8 (*dest)[LEN_OF_TOKEN], U8 *token_count);
void str_trim(S8* str);

#endif
