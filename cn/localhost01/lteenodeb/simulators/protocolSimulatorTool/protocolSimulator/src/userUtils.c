/************************************************************************************
*
*   FILE NAME  : userUtils.c
*
*   DESCRIPTION: Common service functions implementation.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <ctype.h>
#include <assert.h>
#ifdef WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/time.h>
#endif
#include <sys/timeb.h>

#include "simLogger.h"
#include "userUtils.h"
#include "callbacksVars.h"
#include "sock_tools.h"
#include "operations.h"

/****************************************************************************
 * Private Definitions
 ****************************************************************************/

/****************************************************************************
 * Functions implementation
 ****************************************************************************/

/************************************************************************************
 * FUNCTION NAME    :   userStrReplaceChar
 * DESCRIPTION      :   Replaces all char_old to char_new in given string
 * PARAMATERS       :   [IN] UChar8* str
 *                      [IN] UChar8 char_old
 *                      [IN] UChar8 char_new
 * RETURN VALUE     :   None
 ************************************************************************************/
void userStrReplaceChar(UChar8* str, UChar8 char_old, UChar8 char_new)
{
    while(0!=*str)
    {
        if(char_old==*str)
        {
            *str=char_new;
        }
        str++;
    }
}

/************************************************************************************
 * FUNCTION NAME    :   userPrintRawBuffer
 * DESCRIPTION      :   Debug print of buffer, which can contain NULL symbols
 * PARAMATERS       :   [IN] UInt8* buf - buffer with raw data
 *                      [IN] UInt32 len_buf - length of buffer
 *                      [IN] UInt32 fmt - define print leading '0x' symbols before
 *                      hex digits or not
 * RETURN VALUE     :   None
 ************************************************************************************/
void userPrintRawBuffer(const UChar8* strHeader, const UInt8* buf, UInt32 len_buf, UInt32 fmt)
{
	userPrintRawBufferMasked(strHeader,buf,len_buf,fmt,PNULL);
}

/************************************************************************************
 * FUNCTION NAME    :   userPrintRawBufferMasked
 * DESCRIPTION      :   Debug print of buffer, which can contain NULL symbols.
 *  					Masked digits are printed as "." .
 * PARAMATERS       :   [IN] UInt8* buf - buffer with raw data,
 *                      [IN] UInt32 len_buf - length of buffer,
 *                      [IN] UInt32 fmt - define print leading '0x' symbols before
 *                      hex digits or not
 *                      [IN] UInt8* mask - buffer with mask information
 * RETURN VALUE     :   None
 ************************************************************************************/
void userPrintRawBufferMasked(
	const UChar8* strHeader,
	const UInt8* buf,
	UInt32 len_buf,
	UInt32 fmt,
	const UInt8* mask
)
{
    UInt32 i;
    UChar8 logStr[LOG_MAX_STRING_LEN];
    UChar8 workStr[LOG_MAX_STRING_LEN];

    assert(buf);
    assert(strHeader);

    if ( SIM_LOG_DUMP > loggerGetLogLevel() )
    {
        /* Log disabled */
        return;
    }

    sprintf(logStr, "%s: ", strHeader);
    SIM_LOG(SIM_LOG_DUMP, "%s", logStr);
    logStr[0] = 0;

    for(i=1; i<=len_buf; i++)
    {
        if(fmt == 1)
        {
        	strcat(logStr, "0x");
        }

        if(PNULL!=mask)
        {
			if(0==(mask[i-1]&0xF0))
			{
				sprintf(workStr, ".");
			}
			else
			{
				sprintf(workStr, "%01x", (buf[i-1]>>4)&0xF);
			}
			strcat(logStr, workStr);

			if(0==(mask[i-1]&0xF))
			{
				sprintf(workStr, ".");
			}
			else
			{
				sprintf(workStr, "%01x ", buf[i-1]&0xF);
			}
        }
        else
        {
            sprintf(workStr, "%02x ", buf[i-1]);
        }
		strcat(logStr, workStr);

        if(i%4 == 0)
        {
            if(i%16 == 0)
            {
                sprintf(workStr, "\n\t\t  ");
            }
            else
            {
                sprintf(workStr, "\t");
            }
            strcat(logStr, workStr);

            if ( i%256 == 0 )
            {
                SIM_LOG(SIM_LOG_DUMP, "%s", logStr);
                logStr[0] = 0;
            }
        }
    }
    sprintf(workStr, "\n");
    strcat(logStr, workStr);

    SIM_LOG(SIM_LOG_DUMP, "%s", logStr);
}

/************************************************************************************
 * FUNCTION NAME    :   userIniKeyPresent
 * DESCRIPTION      :   Checks if string has KEY=VALUE format
 * PARAMATERS       :   UChar8 *keyName,UChar8 *string
 * RETURN VALUE     :   address of value (after '='), PNULL if no
 ************************************************************************************/
UChar8 *userIniKeyPresent(UChar8 *keyName,UChar8 *string)
{
    UInt32 keyLen=(UInt32)strlen(keyName);
    if ( 0 == strncmp(keyName, string, keyLen))
    {
        if ( '=' == string[keyLen] )
        {
            return string + keyLen + 1;
        }
    }
    return PNULL;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetListStringsFromCfgFile
 * DESCRIPTION      :   Reading configuration file and getting all strings with
 *                      configuration params from it, skipping comments,
 *                      started from symbol '#'
 * PARAMATERS       :   [IN] const UChar8* cfg_fname - name of configuration file
 * RETURN VALUE     :   [OUT] const UChar8** - buffer with strings
 ************************************************************************************/
UChar8** userGetListStringsFromCfgFile(const UChar8* cfg_file)
{
    UChar8*     str_buf = PNULL;
    UChar8**    str_arr = PNULL;

    assert(cfg_file);

    str_buf = userGetFileStringBuf(cfg_file);
    if(PNULL == str_buf)
    {
        return str_arr;
    }
    str_arr = userGetListStrings(str_buf, "\n", '#');
    free(str_buf);

    return str_arr;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetListStrings
 * DESCRIPTION      :
 * PARAMATERS       :   [IN] const UChar8* str_buf
 *                      [IN] const UChar8* str_delim
 *                      [IN] UChar8 comment_symbol
 * RETURN VALUE     :   [OUT] const UChar8** - buffer with strings
 ************************************************************************************/
UChar8** userGetListStrings(const UChar8* str_buf, const UChar8* str_delim, UChar8 comment_symbol)
{
    UInt32  i = 0, nlines  = 0;
    UChar8* token = PNULL, *loc_buf = PNULL;
    UChar8**        str_arr = PNULL;

    assert(str_buf);
    assert(str_delim);

    loc_buf = malloc(strlen(str_buf)+1);
    assert(loc_buf);
    strcpy(loc_buf, str_buf);

    nlines = userGetNumLinesInStrBuf(loc_buf, str_delim[0]);
    if(0 == nlines)
    {
        return str_arr;
    }

    str_arr = (UChar8**)malloc((nlines+1)*sizeof(UInt8*));
    assert(str_arr);

    token = strtok(loc_buf, str_delim);
    while( token != PNULL )
    {
        if(token[0] != comment_symbol)
        {
            /* we must reserve memory for nul symbol in the end of array*/
            /* otherwise this memory can be heap allocated for another  */
            /* needs and our nul will be destroyed                      */
#ifdef WIN32
            str_arr[i] = _strdup(token);
#else
            str_arr[i] = strdup(token);
#endif
            assert(str_arr[i]);
            i++;
        }
        token = strtok(PNULL, str_delim);
    }
    str_arr[i] = PNULL;
    free(loc_buf);
    return str_arr;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetOpParamsListStrings
 * DESCRIPTION      :
 * PARAMATERS       :   [IN] const UChar8* str_buf
 *                      [IN] UChar8 delim_chr
 * RETURN VALUE     :   [OUT] const UChar8** - buffer with strings
 ************************************************************************************/
UChar8** userGetOpParamsListStrings(const UChar8* str_buf,UChar8 delim_chr)
{
    UInt32  i = 0, nlines  = 0;
    UChar8**        str_arr = PNULL;
    UInt16 brk=0;
    UInt16 p=0;
    UInt16 p_start=0;
    UChar8 *loc_buf = PNULL;

    assert(str_buf);

    loc_buf = malloc(strlen(str_buf)+1);
    assert(loc_buf);
    strcpy(loc_buf, str_buf);

    if (0==strlen(loc_buf))
    {
        str_arr = (UChar8**)malloc(1*sizeof(UInt8*));
        assert(str_arr);
        str_arr[0]=PNULL;
        return str_arr;
    }

    nlines = userGetNumLinesInStrBuf(loc_buf, delim_chr);
    if(0 == nlines)
    {
        return str_arr;
    }

    str_arr = (UChar8**)malloc((nlines+1)*sizeof(UInt8*));
    assert(str_arr);

    for(p=0;0!=loc_buf[p];p++)
    {
        if(')'==loc_buf[p])
        {
            if(brk>0)
            {
                brk--;
            }
            else
            {
        		SIM_LOG( SIM_LOG_WARNING, "%s:extra ) in %s",__FUNCTION__,
        			str_buf);
                break;
            }
        }
        else
        if('('==loc_buf[p])
        {
            brk++;
        }
        else
        if((';'==loc_buf[p])&&(0==brk))
        {
            loc_buf[p]=0;
#ifdef WIN32
            str_arr[i] = _strdup(loc_buf+p_start);
#else
            str_arr[i] = strdup(loc_buf+p_start);
#endif
            assert(str_arr[i]);
            i++;
            p_start = p+1;
        }
    }

    if(brk>0)
    {
    	SIM_LOG( SIM_LOG_WARNING, "%s:no ) in %s",__FUNCTION__,str_buf);
    }

    if(0==loc_buf[p])
    {
#ifdef WIN32
        str_arr[i] = _strdup(loc_buf+p_start);
#else
        str_arr[i] = strdup(loc_buf+p_start);
#endif
        assert(str_arr[i]);
        i++;
    }

    str_arr[i] = PNULL;
    free(loc_buf);
    return str_arr;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetListStringsCount
 * DESCRIPTION      :
 * PARAMATERS       :   [IN] cconst UChar8** str_arr
 * RETURN VALUE     :   [OUT] UInt32 - number of strings in  array
 ************************************************************************************/
UInt32 userGetListStringsCount(const UChar8** str_arr)
{
    UInt32 i=0;
    assert(str_arr);
    for(i=0; str_arr[i]; i++);
    return i;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetNumLinesInStrBuf
 * DESCRIPTION      :   Calculalates number of lines in string buffer
 * PARAMATERS       :   [IN] const UChar8* str_buf
 * RETURN VALUE     :   [OUT] UInt32
 ************************************************************************************/
UInt32 userGetNumLinesInStrBuf(const UChar8* str_buf, UChar8 str_delim)
{
    UInt32 i,j;
    assert(str_buf);
    for(i=0,j=0; str_buf[i]; i++)
    {
        if(str_buf[i] == str_delim)
        {
            j++;
        }
    }
    if(str_buf[strlen(str_buf)-1] != str_delim)
    {
        j++;
    }
    return j;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetFileLen
 * DESCRIPTION      :   Calculalates length of file
 * PARAMATERS       :   [IN] const UChar8* file_name
 * RETURN VALUE     :   [OUT] UInt32
 ************************************************************************************/
UInt32 userGetFileLen(const UChar8* file_name)
{
    FILE*       fd      =   PNULL;
    UInt32      flen    = 0;

    assert(file_name);

    fd = fopen(file_name, "r");

    if(PNULL == fd)
    {
        SIM_LOG( SIM_LOG_WARNING, "Unable to open %s", file_name);
        return flen;
    }

    fseek(fd, 0, SEEK_END);
    flen = ftell(fd);
    fseek(fd, 0, SEEK_SET);

    fclose(fd);

    return flen;
}

/************************************************************************************
 * FUNCTION NAME    :   userGetFileStringBuf
 * DESCRIPTION      :   Returns string content of file
 * PARAMATERS       :   [IN] const UChar8* file_name
 * RETURN VALUE     :   [OUT] UChar8*
 ************************************************************************************/
UChar8* userGetFileStringBuf(const UChar8* file_name)
{
    FILE*       fd      = PNULL;
    UInt32      flen    = 0;
    UChar8*     buf     = PNULL;

    assert(file_name);

    flen = userGetFileLen(file_name);
    if(0 == flen)
    {
        SIM_LOG( SIM_LOG_WARNING, "Length of %s is 0", file_name);
        return buf;
    }

    fd = fopen(file_name, "r");

    if(PNULL == fd)
    {
        SIM_LOG( SIM_LOG_WARNING, "Unable to open %s", file_name);
        return buf;
    }

    buf = (UChar8*)malloc(flen+1);
    assert(buf);
    flen = (UInt32)fread(buf, 1, flen, fd);
    buf[flen] = 0;

    fclose(fd);

    return buf;
}

/************************************************************************************
 * FUNCTION NAME    :   userFreeListStrings
 * DESCRIPTION      :   Frees array of heap allocated strings
 * PARAMATERS       :   [IN] UChar8** strings - array with strings
 * RETURN VALUE     :   None
 ************************************************************************************/
void userFreeListStrings(UChar8** strings)
{
    UInt32 i;
    assert(strings);

    for(i=0; strings[i]; i++)
    {
        free(strings[i]);
    }
    free(strings);
}

/************************************************************************************
 * FUNCTION NAME    :   userSleep
 * DESCRIPTION      :   Sleep msec time
 * PARAMATERS       :   [IN] UInt32 msec - time to sleep
 * RETURN VALUE     :   None
 ************************************************************************************/
void userSleep(UInt32 msec)
{
    msec*=DELAY_SIMU;

#ifdef WIN32
    Sleep(msec);
#else
/*linux with usleep support*/
    usleep(msec*1000);
#endif
}

/************************************************************************************
 * FUNCTION NAME    :   userTolower
 * DESCRIPTION      :   Cast string to lowercase
 * PARAMATERS       :   [OUT] UChar8* str - string for cast
 * RETURN VALUE     :   None
 ************************************************************************************/
void userTolower(UChar8* str)
{
    assert(str);
    while(0!=(*str = tolower(*str)))
    {
    	str++;
    }
}

/************************************************************************************
 * FUNCTION NAME    :   userRTrim
 * DESCRIPTION      :   To trim \0x20\0x9\0xd\0xa from right
 * PARAMATERS       :   [OUT] UChar8* str - string for RTrim
 * RETURN VALUE     :   None
 ************************************************************************************/
void userRTrim(UChar8* str)
{
    UInt32 p=(UInt32)strlen(str);
    assert(str);
    while( (p>0) && (PNULL!=strchr("\x20\x9\xD\0xA",str[p])) )
    {
        str[p--]=0;
    }
}

/************************************************************************************
 * FUNCTION NAME    :   userAddMsgBit
 * DESCRIPTION      :   Add bit to buffer
 * PARAMATERS       :   [OUT] UInt8* msgBuffer_p - buffer to append data to
 *                      [IN/OUT] UInt16* pos - byte position in buffer (will be updated)
 *                      [IN/OUT] UInt8* bit_pos - bit position in buffer (will be updated)
 *                      [IN] UInt8 bit_val - BIT value to be added (0/1)
 * 						[OUT] UInt8* msgMask_p - buffer to append mask to
 *                      [IN] UInt8 bit_mask - BIT mask (0/1)
 * RETURN VALUE     :
 ************************************************************************************/
void userAddMsgBit(
	UInt8* msgBuffer_p,
	UInt16* pos,
	UInt8* bit_pos,
	UInt8 bit_val,
	UInt8* msgMask_p,
	UInt8 bit_mask
)
{
	assert(*bit_pos<8);
	msgBuffer_p[*pos]&=~(1<<(7-*bit_pos));
	msgBuffer_p[*pos]|=(bit_val&1)<<(7-*bit_pos);

	if(PNULL!=msgMask_p)
	{
		msgMask_p[*pos]&=~(1<<(7-*bit_pos));
		msgMask_p[*pos]|=(bit_mask&1)<<(7-*bit_pos);
	}

	(*bit_pos)++;
	if(*bit_pos>7)
	{
		*bit_pos=0;
		(*pos)++;
	}
}

/************************************************************************************
 * FUNCTION NAME    :   userAddMsgByte
 * DESCRIPTION      :   Add byte to buffer
 * PARAMATERS       :   [OUT] UInt8* msgBuffer_p - buffer to append data to
 *                      [IN/OUT] UInt16* pos - byte position in buffer (will be updated)
 *                      [IN/OUT] UInt8* bit_pos - bit position in buffer (will be updated)
 * 						[OUT] UInt8* msgMask_p - buffer to append mask to
 *                      [IN] UInt8 val - value to be added
 *                      [IN] UInt8 mask - mask to be added*
 * RETURN VALUE     :
 ************************************************************************************/
void userAddMsgByte(
	UInt8* msgBuffer_p,
	UInt16* pos,
	UInt8* bit_pos,
	UInt8 val,
	UInt8* msgMask_p,
	UInt8 mask
)
{
	UInt8 k=0;

    if(0==*bit_pos)
    {
    	if(PNULL!=msgMask_p)
    	{
    		msgMask_p[*pos]=mask;
    	}
        msgBuffer_p[(*pos)++]=val;
    }
    else
    {
    	for(k=0;k<8;k++)
    	{
    		userAddMsgBit(msgBuffer_p,pos,bit_pos,val>>(7-k),msgMask_p,mask);
    	}
    }
}

/************************************************************************************
 * FUNCTION NAME    :   userParseMsgStr
 * DESCRIPTION      :   Parses hex string (with variables and $operations)
 * PARAMATERS       :   [IN] const UChar8* str_p - hex string (with variables) to be parsed
 *                      [IN] UInt16 maxSize - max output buffer size
 *                      [OUT] UInt8* msgBuffer_p - pointer to output buffer
 *                      [OUT] UInt16* size - size of output message
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userParseMsgStr(
    const UChar8* str_p,
    UInt16 maxSize,
    UInt8* msgBuffer_p,
    UInt16* size,
    UInt8* msgMask_p
)
{
    const UChar8* space = "\xA\xD\x20\x9,";
    UChar8      str_buf[0xffff];
    UChar8      token_buf[0xffff];
    UChar8      op_name[MAX_OP_NAME_SIZE+1];
    UInt8       bit_pos=0;
    UInt16		p=0;
    UInt16		brk=0;

    assert(PNULL!=str_p);
    assert(maxSize>0);
    assert(PNULL!=msgBuffer_p);
    assert(PNULL!=size);

    if(SIM_TRUE!=varExpandVars(str_buf,str_p))
    {
        return SIM_FALSE;
    }

    *size=0;

    while(0!=str_buf[p])
    {
    	token_buf[0]=0;
    	if(0!=strchr(space,str_buf[p]))
    	{
    		p++;
    		continue;
    	}
        if(isxdigit((UInt8)str_buf[p]))
        {
        	strncat(token_buf,str_buf+p,1);
        	p++;
        	if(isxdigit((UInt8)str_buf[p]))
        	{
            	strncat(token_buf,str_buf+p,1);
            	p++;
        	}
        	else if(0!=str_buf[p])
        	{
                SIM_LOG( SIM_LOG_WARNING, "%s:Invalid character (0x%02x) '%s', after xdigit",
                	__FUNCTION__,str_buf[p],str_buf+p);
                return SIM_FALSE;
        	}

            userAddMsgByte(msgBuffer_p,size,&bit_pos,
                (UInt8)strtol(token_buf, PNULL, 16),
                msgMask_p,
                0xFF);
        }
        else if('$'==str_buf[p])
        {
        	p++;
        	brk=0;
        	op_name[0]=0;
        	while( (0!=str_buf[p]) && ('('!=str_buf[p]) &&
        		(strlen(op_name)<MAX_OP_NAME_SIZE) )
        	{
            	strncat(op_name,str_buf+p,1);
            	p++;
        	}
        	if('('!=str_buf[p])
        	{
        		SIM_LOG( SIM_LOG_WARNING, "%s:no ( after op_name '%s'",
        			__FUNCTION__,op_name);
        		return SIM_FALSE;
        	}
        	p++;
        	while(0!=str_buf[p])
        	{
        		if(')'==str_buf[p])
        		{
        			if(brk>0)
        			{
        				brk--;
        			}
        			else
        			{
        				break;
        			}
        		}
                else
        		if('('==str_buf[p])
        		{
        			brk++;
        		}
            	strncat(token_buf,str_buf+p,1);
            	p++;
        	}
        	if(')'!=str_buf[p])
        	{
        		SIM_LOG( SIM_LOG_WARNING, "%s:no ) after '$%s(%s'",__FUNCTION__,
        			op_name,token_buf);
        		return SIM_FALSE;
        	}
        	p++;
        	if(SIM_TRUE!=userParseOp(op_name,token_buf,maxSize,msgBuffer_p,size,&bit_pos,msgMask_p))
        	{
                SIM_LOG( SIM_LOG_WARNING, "Error when calling $%s(%s)",
                	op_name,token_buf);
                return SIM_FALSE;
        	}
        }
    	else if('#'==str_buf[p])
    	{
    		break;
    	}
        else
        {
            SIM_LOG( SIM_LOG_WARNING, "%s:Invalid character (0x%02x) '%s'",__FUNCTION__,
            	str_buf[p],str_buf+p);
            return SIM_FALSE;
        }
    }

    if(0!=bit_pos)
    {
        SIM_LOG( SIM_LOG_WARNING, "%s:Not whole byte, bit_pos=%i",__FUNCTION__,
        	bit_pos);
        return SIM_FALSE;
    }

    return SIM_TRUE;
}

/************************************************************************************
 * FUNCTION NAME    :   prepareTlv
 * DESCRIPTION      :   Parses confuguration file, and creates buffer with parameters
 *                      for callback
 * PARAMATERS       :   [IN] const UChar8* cfg_fname - name of configuration file for
 *                      callback
 *                      [OUT] UInt8* msgBuffer_p - pointer to message buffer
 *                      [OUT] UInt16* size - size of message
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] void* - pointer to buffer with parameters for
 *                      callback
 ************************************************************************************/
void* prepareTlv(
    const UChar8* cfg_fname,
    UInt8* msgBuffer_p,
    UInt16* size,
    UInt8* msgMask_p
)
{
    Int         i=0;
    /*UInt16      len = 0;*/
    UChar8**    str_arr = PNULL;
    Int         line_num = 0;
    UInt16      line_buf_size=0;

    assert(msgBuffer_p);

    if ( PNULL == cfg_fname )
    {
        return PNULL;
    }

    str_arr = userGetListStringsFromCfgFile(cfg_fname);
    if ( PNULL == str_arr )
    {
        return PNULL;
    }

    for(line_num=0;PNULL!=str_arr[line_num];line_num++)
    {
        if(i>=SOCKET_BUFFER_SIZE)
        {
            SIM_LOG(SIM_LOG_WARNING, "%s:too long cfg file. Max length=%i",__FUNCTION__,SOCKET_BUFFER_SIZE);
            break;
        }
        if(SIM_TRUE!=userParseMsgStr(str_arr[line_num],SOCKET_BUFFER_SIZE-i,msgBuffer_p+i,&line_buf_size,
        		(PNULL!=msgMask_p)?(msgMask_p+i):(PNULL)
        	))
        {
            SIM_LOG(SIM_LOG_WARNING, "%s:Error parsing cfg file '%s',line_num=%i",__FUNCTION__,cfg_fname,line_num);
            break;
        }

        i+=line_buf_size;
    }

    userFreeListStrings(str_arr);

    *size = i;

    return msgBuffer_p;
}

/****************************************************************************
* Function Name  : simTimeDiff
* Inputs         : const struct timeb* ptm_one - pointer to time structure
*                  const struct timeb* ptm_two - pointer to time structure
* Outputs        :
* Returns        : signed long - difference between two timestamps (milliseconds)
* Description    : This function calculates difference between two timestamps
****************************************************************************/
signed long simTimeDiff(const struct timeb* ptm_one, const struct timeb* ptm_two)
{
    return (signed long)(((ptm_one->time)%1000000)*1000+ptm_one->millitm) - (signed long)(((ptm_two->time)%1000000)*1000+ptm_two->millitm);
}

/************************************************************************************
 * FUNCTION NAME    :   exprIsTrue
 * DESCRIPTION      :   Checks if expression has TRUE value (non-zero byte)
 * PARAMATERS       :   UChar8 *expr
 * RETURN VALUE     :   SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt8 exprIsTrue(const UChar8 *expr)
{
    UInt8 buf_p[MAX_EXPRESSION_BUF_SIZE];
    UInt16 buf_size=0;
    UInt8 is_true=SIM_FALSE;
    UInt16 i=0;

    if(SIM_TRUE!=userParseMsgStr(expr,MAX_EXPRESSION_BUF_SIZE,buf_p,&buf_size,PNULL))
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:Error parsing expression '%s'",__FUNCTION__,expr);
        return SIM_FALSE;
    }

    if (0==buf_size)
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:expression hex length is 0",__FUNCTION__);
        return SIM_FALSE;
    }

    for (i=0;i<buf_size;i++)
    {
    	if (0!=buf_p[i])
    	{
    		is_true=SIM_TRUE;
    		break;
    	}
    }

    return is_true;
}
