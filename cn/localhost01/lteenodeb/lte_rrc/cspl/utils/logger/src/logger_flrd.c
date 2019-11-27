/*****************************************************************************
*
* FILENAME      : logger_flrd.c
*
* DESCRIPTION   : This file contains the functions to read the provising data
*                 from the file and fills that data into its own structure. 
*
*
*
* DATE              NAME            REFERENCE           REASON
* 30 Sep, 2001      kumarp           original
* 6 Feb, 2003       svaid            modified
*
*
*
*  Copyright (C) 2006 Aricent Inc . All Rights Reserved
*****************************************************************************/
#include "logger.h"
#include "logger_init.h"

/*****************************************************************************
* GLOBAL VARIABLE
*
*****************************************************************************/
conf_file_t      			conf_file;
conf_section_t   			conf_section;


/*****************************************************************************
*
* FUNCTION NAME : get_conf_info
*
* DESCRIPTION   : This Function reads the cfg file and Get the 
*                 module names to be configured from the main section 
*                 MODULES.
*				  This function tries to read an environment varaible 
*				  CFG_FILE_NAME to get the name of the file that it tries to 
*				  open for reading.
*				  Then it uses the function conf_get_conf_file for reading the
*				  complete file into the global variable conf_file.
*				  If this is successful then it usus function conf_get_section
*				  to extrace the data in between the section named after each module 
*				  read in main section.
*				  This data is stored in the global data structure named
*				  conf_section.
*
*                 NOTE - This function affects state of global variables.
* 
* PARAMETERS    : void
*
* RETURNS       :
* 					void
* 					If environment variable CFG_FILE_NAME is not set the 
* 					function exits with an error trace.
* 					If config file can not be opened for reading then function
* 					exits with an error trace.
* 					If function conf_get_conf_file returns LOGGER_FAILURE, 
* 					function get_conf_info exits with an error trace. 
*
* 					If function conf_get_section returns LOGGER_FAILURE, 
* 					function get_conf_info exits with an error trace. 
* 					Trace is based on the error code returned by 
* 					conf_get_section.
*
*****************************************************************************/
void get_conf_info (void)
{
    FILE*                   fp 		= NULL;
    logger_return_t          err 	= 0;
	S8*			fname;


   	/*** Conf File name is obtained from Variable CONFIG_FILE ***/

	fname = (S8 *)getenv("CFG_FILE_NAME");
    if (NULL == fname)
    {
    	LOGGER_TRACE(("Set the environment variable for conf directory \n"));
        exit(1);
    }
	else
	{
  		fp = fopen(fname, "r" );
	};

  	if (NULL == fp)
  	{
		LOGGER_TRACE(("CONFIG FILE OPEN ERROR\n"));
    	exit(1);
  	};

   	/*** Read the config data into conf_file_t structure. ***/

  	if (LOGGER_FAILURE == conf_get_conf_file(fp, &conf_file))
	{
		LOGGER_TRACE(("ERROR IN FILLING THE STRUCTURE FROM THE FILE \n"));
		fclose(fp);
		exit(1);
	}
  	fclose(fp);
#ifdef ENABLE_TRACE
  	conf_print_conf(&conf_file);
#endif

   	/*** Get the module names to be configured from the main section MODULES*/

	if (LOGGER_FAILURE == conf_get_section(&conf_file,(S8 *)"MODULES",&conf_section,\
                                          &err))
 	{
		switch (err)
		{
			case CONF_ERR_BAD_FILE:
					LOGGER_TRACE(("BAD CONFIG FILE NAME \n"));
					break;
			case CONF_ERR_ZERO_SECTION:
					LOGGER_TRACE(("SECTION DOES NOT CONTAIN ANY DATA\n"));
					break;
			case CONF_ERR_SECTION_NOT_FOUND:
					LOGGER_TRACE(("SECTION NOT FOUND IN CONFIG FILE\n"));
					break;
			case CONF_ERR_SECTION_TOO_LONG:
					LOGGER_TRACE(("SECTION LENGTH IS TOO LONG\n"));
					break;
			default:
					LOGGER_TRACE(("ERROR!!: Unrecognized error code returned to \
								get_conf_info function from conf_get_section \
								function \n"));
					break;
		}
    	exit(1);
  	}
	
	get_mod_info(&conf_section);

  	return ;
}


/****************************************************************************
**    FUNCTION :
**              conf_print_conf
**
**
****************************************************************************
**    DESCRIPTION:
**              Print the conf file
****************************************************************************/
void conf_print_conf(conf_file_t* p_file)
{
	U16 	count;


	LOGGER_TRACE(("Configuration file\n"));
	LOGGER_TRACE(("------------------------------------------\n"));
	LOGGER_TRACE(("Number of lines = %d\n", p_file->count));

	if (0 == p_file->count) 
	{
		LOGGER_TRACE(("No line exist in file\n"));
	}

	for (count = 0; count < p_file->count; count++)
	{
		LOGGER_TRACE(("%s\n",p_file->line[count].data));
		LOGGER_TRACE(("\n"));
	}
}

/*****************************************************************************
*
* FUNCTION NAME : str_trim
*
* DESCRIPTION   : 
*              	  This function removes whitespaces from the 
*              	  beginning and end of the string provided as
*              	  input through parameter p_str.   
* 
* PARAMETERS    :
* 				  p_str 	- Pointer to the string to be trimmed.
*
* RETURNS       :
* 					void
*
****************************************************************************/

void str_trim(S8* p_str)
{
  	S32 			starts_from = 0;
	S32			ends_at = 0;
  	S8 			strtemp[CONF_MAX_LINE_SIZE];
	S32			count	= 0;

	
	if (NULL == p_str || '\0' == p_str[0])
	{
		return ;
	}

  	while (p_str[count] == ' ' || p_str[count] == '\t')
	{
		count++;
	}
  	starts_from = count;

  	ends_at = strlen(p_str);

  	while (p_str[ends_at] == '\n' || p_str[ends_at] == '\0' ||
		   p_str[ends_at] == '\t' )
	{
		ends_at--;
		if (-1 == ends_at)
		{
			break;
		};
	}

  	p_str[ends_at + 1] = '\0';

  	strcpy(strtemp, p_str + starts_from);
  	strcpy(p_str, strtemp);
}


/*****************************************************************************
*
* FUNCTION NAME : conf_get_conf_file
*
* DESCRIPTION   : 
*                 This function is used to read the conf file typically 
*                 logger.cfg
*
* PARAMETERS    :
* 				  FILE*	fp	-	Pointer to a open file which is to be read.
* 				  p_file	-	Pointer to the structure in which data read
* 				  from the file would be stored.
*
* RETURNS       :
* 					LOGGER_SUCCESS
*
* 					This function reads data from the file into the array of
* 					structures pointed to by p_file.
*
* 					Leading and trailing spaces are removed from lines.
* 					Blank and commented lines in the file are ignored.
*
* 					A line is considered to be commented if first non blank
* 					character in line is CONF_COMMENT_CHAR (a #define variable).
*
* 					Lines read from the file are stored in
* 					p_file->line[line_num], where line_num is the number
* 					of line read from file, without counting commented lines.
*
* 					If the data from file can be read successfully, then 
* 					p_file->count contains number of lines read into the array
* 					pointed to by p_file (counting starts from 1). 
*
*
* 					LOGGER_FAILURE
*
* 					If some error occurs in reading the file, then 
* 					conf_get_conf_file returns LOGGER_FAILURE with an error 
* 					trace. p_file->count contains the number of lines 
* 					successfully read till the point of failure.
*
*
*****************************************************************************/
logger_return_t conf_get_conf_file(FILE* fp, conf_file_t* p_file)
{

  	p_file->count = 0;

  	for (p_file->count = 0 ; p_file->count < CONF_MAX_LINES; p_file->count++)
  	{
    	if (NULL == fgets(p_file->line[p_file->count].data,\
                             CONF_MAX_LINE_SIZE, fp))
    	{
       		if (feof(fp)) 
			{
				return LOGGER_SUCCESS;
			}

			if (ferror(fp))
			{
				return LOGGER_FAILURE;
			}
    	}

    	str_trim(p_file->line[p_file->count].data);

    	if (p_file->line[p_file->count].data[0] == CONF_COMMENT_CHAR ||
            p_file->line[p_file->count].data[0] == '\n')
      	{
      		p_file->count--;
    	}

  	}
  	return LOGGER_SUCCESS;
}


/****************************************************************************
**    FUNCTION :
**              conf_print_section
**
**
****************************************************************************
**    DESCRIPTION:
**              Print the section conf file
****************************************************************************/
void conf_print_section(conf_section_t* p_sec)
{
	U16 			count;


	LOGGER_TRACE(("Section : %s\n", p_sec->section_name ));
	LOGGER_TRACE(("Number of lines = %d\n", p_sec->count));
	LOGGER_TRACE(("------------------------------------------\n"));
	if (0 == p_sec->count)
	{
		LOGGER_TRACE(("No line in section to print\n"));
	}
	for (count = 0; count < p_sec->count; count++)
	{
		LOGGER_TRACE(("%d,%s",count,p_sec->line[count].data));
		LOGGER_TRACE(("\n"));
	}

}

/*****************************************************************************
*
* FUNCTION NAME : conf_get_section
*
* DESCRIPTION   : 
* 				  Gets a section of the conf file. Section start and end is 
* 				  detected based on input parameter modulename.
* 				  In case of multiple section starts being present, only 
* 				  the first section start
* 				  is recognized and all other starts are ignored. They are
* 				  not even considered part of the valid data and are not 
* 				  reported in output.
* 				  If there are multiple section ends then the first 
* 				  section end is recognized as the end.
* 				  
* 
* PARAMETERS    :
* 				  p_file 		- Pointer to input data from which a section 
* 				  				  of the conf file has to be read.
* 				  sectionname	- Name of the section whose data has to be 
* 				  				  extracted.
* 				  p_section 	- Pointer to the output, i.e. the place where
* 				  				  extracted section data should be ouputted.
* 				  				  p_section->count reports number lines read
* 				  				  with counting starting from 1.
* 				  p_err			- Pointer to location where error code should
* 				  				  be stored in case conf_get_section returns
* 				  				  LOGGER_FAILURE.
*
*
* RETURNS       :
* 				  LOGGER_SUCCESS
*
* 				  Line containing section name is not included in the output.
* 				  psection->count counts number of lines in a section starting
* 				  from 0.
*
*
* 				  LOGGER_FAILURE
*
* 				  If no section start or section end is detected then 
* 				  LOGGER_FAILURE is returned with error code as 
* 				  CONF_ERR_SECTION_NOT_FOUND.
*
* 				  If the input data has no line that can be read then 
* 				  LOGGER_FAILURE is returned and error code is set as 
* 				  CONF_ERR_BAD_FILE.
*
* 				  If section start is present and section end is also present 
* 				  but there is no data in that section then LOGGER_FAILURE 
* 				  is returned with error code set to CONF_ERR_ZERO_SECTION.
* 				  
* 				  If no section stop is detected before 
* 				  CONF_MAX_LINES_PER_SECTION number of lines are read between 
* 				  start of a section and end of the section, then LOGGER_FAILURE
* 				  is returned with error code set as CONF_ERR_SECTION_TOO_LONG.
* 				  In this case p_file->count, p_file->data contain values
* 				  for the data read till the line count exceeded maximum 
* 				  permitted.
*
*
*****************************************************************************/

logger_return_t conf_get_section(conf_file_t* p_file, S8* sectionname, \
				           conf_section_t* p_section, logger_return_t* p_err)
{
  	U8 			section_start 	= 0;
  	U8 			sec_name_start[CONF_MAX_LINE_SIZE + 2];
  	U8 			sec_name_stop[CONF_MAX_LINE_SIZE + 2];
  	U16 			count 			= 0;


  	p_section->count = -1;
  	if ((p_file == NULL) || p_file->count <= 0)
  	{
     	*p_err = CONF_ERR_BAD_FILE;
     	return LOGGER_FAILURE;
  	}
	
  	strcpy(sec_name_start,"[");
  	strcat(sec_name_start, sectionname);
  	strcat(sec_name_start,"]");

  	strcpy(sec_name_stop,"[/");
  	strcat(sec_name_stop, sectionname);
  	strcat(sec_name_stop,"]");

  	for (count = 0; count < p_file->count; count++)
  	{
    	if (0 == strcmp(sec_name_start, p_file->line[count].data))
    	{
      		section_start = LOGGER_TRUE;
      		strcpy(p_section->section_name, sectionname);
      		count++;          /* to skip the current line which contains just 
                                 section name */
    	}

    	if (0 == strcmp(sec_name_stop, p_file->line[count].data))
    	{
    		if (-1 == (S8)p_section->count)
      		{
         		*p_err = CONF_ERR_ZERO_SECTION;
         		return LOGGER_FAILURE;
      		}
      		p_section->count++; /* final count */

      		return LOGGER_SUCCESS;
    	}

    	if (LOGGER_TRUE == section_start)
    	{
      		p_section->count++;

      		if (CONF_MAX_LINES_PER_SECTION == p_section->count)
      		{
         		*p_err = CONF_ERR_SECTION_TOO_LONG;
         		return LOGGER_FAILURE;
      		}
      		strcpy(p_section->line[p_section->count].data, \
                                                    p_file->line[count].data );
    	}
  	}

  	*p_err = CONF_ERR_SECTION_NOT_FOUND;

  	return LOGGER_FAILURE;
}

/*****************************************************************************
*
* FUNCTION NAME : tok_fun
*
* DESCRIPTION   :
* 				  This function tokenise the single line and stores tokens in
* 				  array p_dest and count of tokens in p_token_count.
*
* 				  Assumption: No of tokens should not exceed NO_OF_TOKEN and no
* 				  token should be longer than LEN_OF_TOKEN.
* 				  Leading whitespaces are removed and a token is supposed to end
* 				  when trailing white space is encountered.
*
* PARAMETERS    :
* 				  p_src : Pointer to the line that is to be tokenise.
* 				  p_dest : Pointer to two dimension array which stores
* 				           pointers to tokens.
* 				  p_token_count : No of tokens extracted counting from 1
* 				                  onwards.
*
* RETURNS       :
* 				  void
*
*****************************************************************************/
void tok_func(U8 *p_src, U8 (*p_dest)[LEN_OF_TOKEN], U8 *p_token_count)
{
    U16 				j 			= 0;
    U16 				i 			= 0;
    U16 				k 			= 0;
    U16 				flag 		= 0;
	

    for (i = 0; i < strlen(p_src); )
    {
        if ((*(p_src + i) == ' ') || (*(p_src + i) == '\t') || 
			                                            (*(p_src + i) == '\n'))
        {
			i++;
        }
        else
        {
            flag = 1;
            while (flag == 1)
            {
                p_dest[j][k] = *(p_src + i);
                k++;
                i++;
                if ((*(p_src + i) == ' ')  || (*(p_src + i) == '\t') || \
					(*(p_src + i) == '\0') || (*(p_src + i) == '\n'))
                {
                    flag = 0;
                }
            }
            p_dest[j][k] = '\0';
            j++;
            k = 0;
        }
    }
	
	*p_token_count = j + 1;
	
}

/*****************************************************************************
*
* FUNCTION NAME : tokenise
*
* DESCRIPTION   :
* 				  Function to extract tokens from all lines of a section.
* 				  It calls the tok_fun() to tokenise the line.  
*
* 				  Assumption : p_section->count should not exceed 
* 				  CONF_MAX_LINES_PER_SECTION. 
*
* PARAMETERS    :
* 				  p_section : Pointer to the section that is the input to 
* 				  the function
*
* RETURNS       :
* 				  void
*
*****************************************************************************/
void tokenise(conf_section_t *p_section)
{
	U8 			count = 0;
	U8 			token_count = 0;

	if (NULL == p_section || 0 >= p_section->count)
	{
		return ;
	}
	for (count = 0; count < p_section->count; count++)
    {
		tok_func(p_section->line[count].data, p_section->line[count].token,\
                                                            &token_count );
		p_section->line[count].token_count = token_count;
	}
}
