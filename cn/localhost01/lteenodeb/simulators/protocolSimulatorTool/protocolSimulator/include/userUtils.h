/************************************************************************************
*
*   FILE NAME  : userUtils.h
*
*   DESCRIPTION: Common service functions definitions.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef __USERUTILS_H_
#define __USERUTILS_H_

/************************************************************************************
 * Includes
 ************************************************************************************/
#include "simTypes.h"

/************************************************************************************
 * Defines
 ************************************************************************************/

#define MAX_EXPRESSION_BUF_SIZE /*1024~~~*/0xFFFE
#define MAX_FMT_BUF_SIZE        16
#define MAX_CALLBACK_NAME_SIZE  40
#define MAX_LABEL_NAME_SIZE     40
#define MAX_OP_NAME_SIZE  		20
#define MAX_PARAMS_SIZE         256
#define MAX_FILE_NAME_SIZE      256
#define END_INPUT   '\n'
#define LAST_INT    (UInt32)(-1)

#define LOG_MAX_STRING_LEN  1024

#define DELAY_SIMU 1
#define TIMEOUT_SIMU 1

/************************************************************************************
 * FUNCTION NAME    :   userStrReplaceChar
 * DESCRIPTION      :   Replaces all char_old to char_new in given string
 * PARAMATERS       :   [IN] UChar8* str
 *                      [IN] UChar8 char_old
 *                      [IN] UChar8 char_new
 * RETURN VALUE     :   None
 ************************************************************************************/
void userStrReplaceChar(UChar8* str, UChar8 char_old, UChar8 char_new);

/************************************************************************************
 * FUNCTION NAME    :   userPrintRawBuffer
 * DESCRIPTION      :   Debug print of buffer, which can contain NULL symbols
 * PARAMATERS       :   [IN] UInt8* buf - buffer with raw data
 *                      [IN] UInt32 len_buf - length of buffer
 *                      [IN] UInt32 fmt - define print leading '0x' symbols before
 *                      hex digits or not
 * RETURN VALUE     :   None
 ************************************************************************************/
void userPrintRawBuffer(const UChar8* strHeader, const UInt8* buf, UInt32 len_packet, UInt32 fmt);

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
);

/************************************************************************************
 * FUNCTION NAME    :   userPrintZCBBuffer
 * DESCRIPTION      :   Debug print of buffer, which can contain NULL symbols
 * PARAMATERS       :   [IN] void* zcb_p - ZCB buffer
 *                      [IN] UInt32 fmt - define print leading '0x' symbols before
 *                      hex digits or not
 * RETURN VALUE     :   None
 ************************************************************************************/
void userPrintZCBBuffer(const UChar8* strHeader, void* zcb_p, UInt32 fmt);

/************************************************************************************
 * FUNCTION NAME    :   userIniKeyPresent
 * DESCRIPTION      :   Checks if string has KEY=VALUE format
 * PARAMATERS       :   UChar8 *keyName,UChar8 *string
 * RETURN VALUE     :   address of value (after '='), PNULL if no
 ************************************************************************************/
UChar8 *userIniKeyPresent(UChar8 *keyName,UChar8 *string);

/************************************************************************************
 * FUNCTION NAME    :   userGetListStringsFromCfgFile
 * DESCRIPTION      :   Reading configuration file and getting all strings with
 *                      configuration params from it, skipping comments,
 *                      started from symbol '#'
 * PARAMATERS       :   [IN] const UChar8* cfg_fname - name of configuration file
 * RETURN VALUE     :   [OUT] const UChar8** - buffer with strings
 ************************************************************************************/
UChar8** userGetListStringsFromCfgFile(const UChar8* cfg_file);

/************************************************************************************
 * FUNCTION NAME    :   userGetListStrings
 * DESCRIPTION      :   Reading configuration file and getting all strings with
 *                      configuration params from it, skipping comments,
 *                      started from symbol '#'
 * PARAMATERS       :   [IN] const UChar8* str_buf
 *                      [IN] const UChar8* str_delim
 *                      [IN] UChar8 comment_symbol
 * RETURN VALUE     :   [OUT] const UChar8** - buffer with strings
 ************************************************************************************/
UChar8** userGetListStrings(const UChar8* str_buf, const UChar8* str_delim, UChar8 comment_symbol);

/************************************************************************************
 * FUNCTION NAME    :   userGetOpParamsListStrings
 * DESCRIPTION      :
 * PARAMATERS       :   [IN] const UChar8* str_buf
 *                      [IN] UChar8 delim_chr
 * RETURN VALUE     :   [OUT] const UChar8** - buffer with strings
 ************************************************************************************/
UChar8** userGetOpParamsListStrings(const UChar8* str_buf,UChar8 delim_chr);

/************************************************************************************
 * FUNCTION NAME    :   userGetListStringsCount
 * DESCRIPTION      :
 * PARAMATERS       :   [IN] cconst UChar8** str_arr
 * RETURN VALUE     :   [OUT] UInt32 - number of strings in  array
 ************************************************************************************/
UInt32 userGetListStringsCount(const UChar8** str_arr);

/************************************************************************************
 * FUNCTION NAME    :   userFreeListStrings
 * DESCRIPTION      :   Frees array of heap allocated strings
 * PARAMATERS       :   [IN] UChar8** strings - array with strings
 * RETURN VALUE     :   None
 ************************************************************************************/
void userFreeListStrings(UChar8** strings);

/************************************************************************************
 * FUNCTION NAME    :   userGetNumLinesInStrBuf
 * DESCRIPTION      :   Calculalates number of lines in string buffer
 * PARAMATERS       :   [IN] const UInt8* str_buf
 * RETURN VALUE     :   [OUT] UInt32
 ************************************************************************************/
UInt32 userGetNumLinesInStrBuf(const UChar8* str_buf, UChar8 str_delim);

/************************************************************************************
 * FUNCTION NAME    :   userGetFileLen
 * DESCRIPTION      :   Calculalates length of file
 * PARAMATERS       :   [IN] const UChar8* file_name
 * RETURN VALUE     :   [OUT] UInt32
 ************************************************************************************/
UInt32 userGetFileLen(const UChar8* file_name);

/************************************************************************************
 * FUNCTION NAME    :   userGetFileStringBuf
 * DESCRIPTION      :   Returns string content of file
 * PARAMATERS       :   [IN] const UChar8* file_name
 * RETURN VALUE     :   [OUT] UChar8*
 ************************************************************************************/
UChar8* userGetFileStringBuf(const UChar8* file_name);

/************************************************************************************
 * FUNCTION NAME    :   userSleep
 * DESCRIPTION      :   Sleep msec time
 * PARAMATERS       :   [IN] UInt32 msec - time to sleep
 * RETURN VALUE     :   None
 ************************************************************************************/
void userSleep(UInt32 msec);

/************************************************************************************
 * FUNCTION NAME    :   userTolower
 * DESCRIPTION      :   Cast string to lowercase
 * PARAMATERS       :   [OUT] UInt8* str - string for cast
 * RETURN VALUE     :   None
 ************************************************************************************/
void userTolower(UChar8* str);

/************************************************************************************
 * FUNCTION NAME    :   userRTrim
 * DESCRIPTION      :   To trim \0x20\0x9\0xd\0xa from right
 * PARAMATERS       :   [OUT] UChar8* str - string for RTrim
 * RETURN VALUE     :   None
 ************************************************************************************/
void userRTrim(UChar8* str);

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
);

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
);

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
);

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
);

/****************************************************************************
* Function Name  : simTimeDiff
* Inputs         : const struct timeb* ptm_one - pointer to time structure
*                  const struct timeb* ptm_two - pointer to time structure
* Outputs        :
* Returns        : signed long - difference between two timestamps (milliseconds)
* Description    : This function calculates difference between two timestamps
****************************************************************************/
signed long simTimeDiff(const struct timeb* ptm_one, const struct timeb* ptm_two);

/************************************************************************************
 * FUNCTION NAME    :   exprIsTrue
 * DESCRIPTION      :   Checks if expression has TRUE value (non-zero byte)
 * PARAMATERS       :   UChar8 *expr
 * RETURN VALUE     :   SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt8 exprIsTrue(const UChar8 *expr);

#endif /* __USERUTILS_H_ */
