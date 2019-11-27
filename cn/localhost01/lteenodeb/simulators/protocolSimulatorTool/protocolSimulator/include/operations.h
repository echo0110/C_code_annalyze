/************************************************************************************
*
*   FILE NAME  : operations.h
*
*   DESCRIPTION: Script operations support definitions.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

#ifndef __OPERATIONS_H__
#define __OPERATIONS_H__

/************************************************************************************
 * Includes
 ************************************************************************************/

#include "simTypes.h"

/****************************************************************************
 * Exported Functions
 ****************************************************************************/

/************************************************************************************
 * FUNCTION NAME    :   userRunInttohexOperation
 * DESCRIPTION      :   Run $inttohex operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunInttohexOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);
/************************************************************************************
 * FUNCTION NAME    :   userRunVarSizeOperation
 * DESCRIPTION      :   Run $VarSize operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunVarSizeOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userRunBitsOperation
 * DESCRIPTION      :   Run $bits operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunBitsOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userRunDigOperation
 * DESCRIPTION      :   Run $dig operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunDigOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userRunSkipOperation
 * DESCRIPTION      :   Run $skip operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunSkipOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userRunNtohsOperation
 * DESCRIPTION      :   Run $ntoh operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunNtohOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userRunCmpOperation
 * DESCRIPTION      :   Run $cmp operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunCmpOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userRunIncOperation
 * DESCRIPTION      :   Run $inc operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunIncOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userRunReadFileOperation
 * DESCRIPTION      :   Run $readfile operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunReadFileOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userRunReadSockOperation
 * DESCRIPTION      :   Run $readsock operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunReadSockOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userRunAndOperation
 * DESCRIPTION      :   Run $and operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunAndOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userOrAndOperation
 * DESCRIPTION      :   Run $or operation function
 * PARAMATERS       :   [IN] UInt32 argc - operation arguments counter
 *                      [IN] const UChar8** argv - operation arguments
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   [OUT] status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userRunOrOperation(
	UInt32 argc,
	const UChar8** argv,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

/************************************************************************************
 * FUNCTION NAME    :   userParseOp
 * DESCRIPTION      :   Run script operation function
 * PARAMATERS       :   [IN] UChar8 *op_name - operation name
 *                      [IN] UChar8 *params - parameters string
 * 						[IN] UInt16 maxSize - max output buffer size
 * 						[OUT] UInt8* msgBuffer_p - pointer to output buffer
 * 						[IN/OUT] UInt16* size - size of output message
 * 						[IN/OUT] UInt8* bit_pos - bit position in buffer
 *                      [OUT] UInt8* msgMask_p - pointer to output mask buffer
 * RETURN VALUE     :   status - SIM_TRUE / SIM_FALSE
 ************************************************************************************/
UInt32 userParseOp(
	UChar8 *op_name,
	const UChar8 *params,
	UInt16 maxSize,
	UInt8* msgBuffer_p,
	UInt16* size,
	UInt8* bit_pos,
	UInt8* msgMask_p
);

#endif /*__OPERATIONS_H__*/
