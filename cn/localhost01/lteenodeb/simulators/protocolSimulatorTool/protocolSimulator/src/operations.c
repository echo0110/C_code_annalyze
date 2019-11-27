/************************************************************************************
*
*   FILE NAME  : operations.h
*
*   DESCRIPTION: Script operations support implementation.
*
*   DATE   NAME    REFERENCE   REASON
*
*   Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/************************************************************************************
 * Includes
 ************************************************************************************/

#include <stdlib.h>
#include <assert.h>
#include <string.h>

#include "simLogger.h"
#include "userUtils.h"
#include "simu_table_func.h"
#include "callbacksVars.h"

/*for socket-related operations*/
#include "sock_tools.h"

/*for hton operations */
#ifndef WIN32
	#include <arpa/inet.h>
#else
    #include <winsock.h>
#endif

/****************************************************************************
 * Functions implementation
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
)
{
	UInt32 value=0;
	UInt8 hex_len=0;
	UInt16 k=0;

    if( 2 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$inttohex(<int value>;<result length in bytes>)'");
        return SIM_FALSE;
    }

    assert(argv);

    value=atoi(argv[0]);
    hex_len=atoi(argv[1]);

    if( (hex_len<1) || (hex_len>4) )
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:length value=%i,should be in range 1..4",__FUNCTION__,hex_len);
        return SIM_FALSE;
    }

    for(k=0;k<hex_len;k++)
    {
    	if((*size)<maxSize)
    	{
    		userAddMsgByte(msgBuffer_p,size,bit_pos,(value>>(8*(hex_len-k-1)))&0xFF,msgMask_p,0xFF);
    	}
    	else
    	{
            SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
            return SIM_FALSE;
    	}
    }

	return SIM_TRUE;
}

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
)
{
	UInt32 value=0;
	Int var_idx=0;
	UInt16 k=0;
	UInt8 hex_len=2;

    if( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$varsize(<variable name>)'");
        return SIM_FALSE;
    }

    assert(argv);

    var_idx=varFindIdx(argv[0]);

    if(INVALID_VAR_IDX!=var_idx)
    {
    	value=varGetSize(var_idx);
    }

    for(k=0;k<hex_len;k++)
    {
    	if((*size)<maxSize)
    	{
    		userAddMsgByte(msgBuffer_p,size,bit_pos,(value>>(8*(hex_len-k-1)))&0xFF,msgMask_p,0xFF);
    	}
    	else
    	{
            SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
            return SIM_FALSE;
    	}
    }

	return SIM_TRUE;
}

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
)
{
	UInt16 bits_len=0;
	UInt16 start_bit=0;

	UInt8 exp1_buf[MAX_EXPRESSION_BUF_SIZE];
	UInt16 exp1_buf_size=0;
	UInt16 k=0;
	UInt8 bit_val=0;

    if( 3 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$bits(<hex bytes>;<bits length>;<start bit offset (0=leftmost)>)'");
        return SIM_FALSE;
    }

    assert(argv);

    if(SIM_TRUE!=userParseMsgStr(argv[0],MAX_EXPRESSION_BUF_SIZE,exp1_buf,&exp1_buf_size,PNULL))
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:Error parsing expression '%s'",__FUNCTION__,argv[0]);
        return SIM_FALSE;
    }

    bits_len=atoi(argv[1]);
    start_bit=atoi(argv[2]);

    if( (bits_len+start_bit) > (exp1_buf_size*8) )
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:(bits_len+start_bit) > (exp1_buf_size*8)",__FUNCTION__);
        return SIM_FALSE;
    }

    for(k=0;k<bits_len;k++)
    {
    	if((*size)<maxSize)
    	{
    		bit_val=(exp1_buf[(start_bit+k)/8]>>(7-((start_bit+k)%8)))&1;
    		userAddMsgBit(msgBuffer_p,size,bit_pos,bit_val,msgMask_p,1);
    	}
    	else
    	{
            SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
            return SIM_FALSE;
    	}
    }

	return SIM_TRUE;
}

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
)
{
	UInt16 dig_val=0;
	UInt16 k=0;
	UInt8 bit_val=0;

    if( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$dig(<hex_digit_val>)'");
        return SIM_FALSE;
    }

    assert(argv);

    dig_val=(UInt16)strtol(argv[0], PNULL, 16);

    if(dig_val>0xF)
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:dig_val %x > 0xF",__FUNCTION__,dig_val);
        return SIM_FALSE;
    }

    for(k=0;k<4;k++)
    {
    	if((*size)<maxSize)
    	{
    		bit_val=(dig_val>>(3-k))&1;
    		userAddMsgBit(msgBuffer_p,size,bit_pos,bit_val,msgMask_p,1);
    	}
    	else
    	{
            SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
            return SIM_FALSE;
    	}
    }

	return SIM_TRUE;
}

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
)
{
	UInt16 num_of_bits=0;
	UInt16 k=0;

    if( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$skip(<num_of_bits_to_skip>)'");
        return SIM_FALSE;
    }

    assert(argv);

    num_of_bits=atoi(argv[0]);

    if(0==num_of_bits)
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:num_of_bits == 0",__FUNCTION__);
        return SIM_FALSE;
    }

    if(PNULL==msgMask_p)
    {
        SIM_LOG(SIM_LOG_WARNING, "$skip() should be used only in .res files",__FUNCTION__);
        return SIM_FALSE;
    }

    for(k=0;k<num_of_bits;k++)
    {
    	if((*size)<maxSize)
    	{
    		userAddMsgBit(msgBuffer_p,size,bit_pos,0,msgMask_p,0);
    	}
    	else
    	{
            SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
            return SIM_FALSE;
    	}
    }

	return SIM_TRUE;
}

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
)
{
	UInt8 exp1_buf[MAX_EXPRESSION_BUF_SIZE];
	UInt16 exp1_buf_size=0;
	UInt16 k=0;

    if( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$ntoh(<hex bytes>)'");
        return SIM_FALSE;
    }

    assert(argv);

    if(SIM_TRUE!=userParseMsgStr(argv[0],MAX_EXPRESSION_BUF_SIZE,exp1_buf,&exp1_buf_size,PNULL))
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:Error parsing expression '%s'",__FUNCTION__,argv[0]);
        return SIM_FALSE;
    }

    switch(exp1_buf_size)
    {
    case 2:
    	*(UInt16*)exp1_buf=htons(*(UInt16*)exp1_buf);
    	break;
    case 4:
    	*(UInt32*)exp1_buf=htonl(*(UInt32*)exp1_buf);
    	break;
    default:
        SIM_LOG(SIM_LOG_WARNING, "%s:parameter length is %i. Should be 2 or 4",__FUNCTION__,exp1_buf_size);
        return SIM_FALSE;
    }

    for(k=0;k<exp1_buf_size;k++)
    {
    	if((*size)<maxSize)
    	{
    		userAddMsgByte(msgBuffer_p,size,bit_pos,exp1_buf[k],msgMask_p,0xFF);
    	}
    	else
    	{
            SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
            return SIM_FALSE;
    	}
    }

	return SIM_TRUE;
}

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
)
{
	UInt8 exp1_buf[MAX_EXPRESSION_BUF_SIZE];
	UInt8 exp1_mask[MAX_EXPRESSION_BUF_SIZE];
	UInt16 exp1_buf_size=0;
	UInt8 exp2_buf[MAX_EXPRESSION_BUF_SIZE];
	UInt8 exp2_mask[MAX_EXPRESSION_BUF_SIZE];
	UInt16 exp2_buf_size=0;
	UInt16 k=0;
    UInt8 cmp_res=0;
    UInt32 val1=0;
    UInt32 val2=0;

    if( 3 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$cmp(<hex value 1 NBO>;<cmp_type>;<hex value 2 NBO>)'");
        return SIM_FALSE;
    }

    assert(argv);

    if(SIM_TRUE!=userParseMsgStr(argv[0],MAX_EXPRESSION_BUF_SIZE,exp1_buf,&exp1_buf_size,exp1_mask))
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:Error parsing expression '%s'",__FUNCTION__,argv[0]);
        return SIM_FALSE;
    }

    if(SIM_TRUE!=userParseMsgStr(argv[2],MAX_EXPRESSION_BUF_SIZE,exp2_buf,&exp2_buf_size,exp2_mask))
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:Error parsing expression '%s'",__FUNCTION__,argv[2]);
        return SIM_FALSE;
    }

    if( exp1_buf_size!=exp2_buf_size )
    {
        /*SIM_LOG(SIM_LOG_WARNING, "%s:exp1_buf_size(%i)!=exp2_buf_size(%i)",__FUNCTION__,exp1_buf_size,exp2_buf_size);
        return SIM_FALSE;*/
        cmp_res=0;
    }
    else
    {
        if( exp1_buf_size<1 )
        {
            SIM_LOG(SIM_LOG_WARNING, "%s:invalid exp1_buf_size (%i)",__FUNCTION__,exp1_buf_size);
            return SIM_FALSE;
        }

        if( (0==strcmp(argv[1],"=")) || (0==strcmp(argv[1],"==")) )
        {
            cmp_res=1;
            for(k=0;k<exp1_buf_size;k++)
            {
                if( (exp1_buf[k]&exp1_mask[k]&exp2_mask[k])!=(exp2_buf[k]&exp1_mask[k]&exp2_mask[k]) )
                {
                    cmp_res=0;
                    break;
                }
            }
        }
        else
        if( (0==strcmp(argv[1],"!=")) || (0==strcmp(argv[1],"<>")) )
        {
            cmp_res=0;
            for(k=0;k<exp1_buf_size;k++)
            {
                if( (exp1_buf[k]&exp1_mask[k]&exp2_mask[k])!=(exp2_buf[k]&exp1_mask[k]&exp2_mask[k]) )
                {
                    cmp_res=1;
                    break;
                }
            }
        }
        else
        if( (0==strcmp(argv[1],"<")) || (0==strcmp(argv[1],">")) ||
            (0==strcmp(argv[1],"<=")) || (0==strcmp(argv[1],"=<")) ||
            (0==strcmp(argv[1],">=")) || (0==strcmp(argv[1],"=>")) || (0==strcmp(argv[1],"%"))
        )
        {
            switch(exp1_buf_size)
            {
            case 1:
                val1=exp1_buf[0];
                val2=exp2_buf[0];
                break;
            case 2:
                val1=((exp1_buf[0])<<8)|(exp1_buf[1]);
                val2=((exp2_buf[0])<<8)|(exp2_buf[1]);
                break;
            case 4:
                val1=((exp1_buf[0])<<24)|((exp1_buf[1])<<16)|((exp1_buf[2])<<8)|(exp1_buf[3]);
                val2=((exp2_buf[0])<<24)|((exp2_buf[1])<<16)|((exp2_buf[2])<<8)|(exp2_buf[3]);
                break;
            default:
                SIM_LOG(SIM_LOG_WARNING, "%s:parameter length is %i. Should be 1, 2 or 4",__FUNCTION__,exp1_buf_size);
                return SIM_FALSE;
            }

            if(0==strcmp(argv[1],"%"))
            {
              if (val1 % val2)
              {
                cmp_res = 1;
              }
              else
              {
                cmp_res = 0;
              }
            }
            else
            if(0==strcmp(argv[1],"<"))
            {
                cmp_res = (val1<val2);
            }
            else
            if(0==strcmp(argv[1],">"))
            {
                cmp_res = (val1>val2);
            }
            else
            if( (0==strcmp(argv[1],"<=")) || (0==strcmp(argv[1],"=<")) )
            {
                cmp_res = (val1<=val2);
            }
            else
            if( (0==strcmp(argv[1],">=")) || (0==strcmp(argv[1],"=>")) )
            {
                cmp_res = (val1>=val2);
            }
            else
            {
                assert(0);
            }
        }
        else
        {
            SIM_LOG(SIM_LOG_WARNING, "Invalid cmp_type(%s). Should be one of:\n"
                "= or ==\n"
                "!= or <>\n"
                "< or >\n"
                "<= or =<\n"
                ">= or =>",
                __FUNCTION__,argv[1]);
        }
    }

    if((*size)<maxSize)
	{
		userAddMsgByte(msgBuffer_p,size,bit_pos,cmp_res,msgMask_p,0xFF);
	}
	else
	{
		SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
		return SIM_FALSE;
	}

	return SIM_TRUE;
}

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
)
{
	UInt8 exp1_buf[MAX_EXPRESSION_BUF_SIZE];
	UInt16 exp1_buf_size=0;
	UInt16 k=0;
    UInt32 inc_val=1;

    if( (1 != argc) && (2 != argc) )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$inc(<hex value NBO>;[<decimal value to add|1 by default>])'");
        return SIM_FALSE;
    }

    assert(argv);

    if(SIM_TRUE!=userParseMsgStr(argv[0],MAX_EXPRESSION_BUF_SIZE,exp1_buf,&exp1_buf_size,PNULL))
    {
        SIM_LOG(SIM_LOG_WARNING, "%s:Error parsing expression '%s'",__FUNCTION__,argv[0]);
        return SIM_FALSE;
    }

    if(2==argc)
    {
        inc_val=atoi(argv[1]);
        if(0==inc_val)
        {
            SIM_LOG(SIM_LOG_WARNING, "%s:Value to add is 0",__FUNCTION__);
        }
    }

    switch(exp1_buf_size)
    {
    case 1:
        exp1_buf[0]+=inc_val;
        break;
    case 2:
    	*(UInt16*)exp1_buf=htons(ntohs(*(UInt16*)exp1_buf)+inc_val);
    	break;
    case 4:
    	*(UInt32*)exp1_buf=htonl(ntohl(*(UInt32*)exp1_buf)+inc_val);
    	break;
    default:
        SIM_LOG(SIM_LOG_WARNING, "%s:parameter length is %i. Should be 2 or 4",__FUNCTION__,exp1_buf_size);
        return SIM_FALSE;
    }

    for(k=0;k<exp1_buf_size;k++)
    {
    	if((*size)<maxSize)
    	{
    		userAddMsgByte(msgBuffer_p,size,bit_pos,exp1_buf[k],msgMask_p,0xFF);
    	}
    	else
    	{
            SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
            return SIM_FALSE;
    	}
    }

	return SIM_TRUE;
}

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
)
{
	UInt8 exp1_buf[MAX_EXPRESSION_BUF_SIZE];
	UInt8 exp1_mask[MAX_EXPRESSION_BUF_SIZE];
	UInt16 exp1_buf_size=0;
	UChar8 cfg_fname[MAX_FILE_NAME_SIZE];
	UInt32 fnlen=0;
	UInt16 k=0;

    if( 1 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$readfile(<filename>)'");
        return SIM_FALSE;
    }

    assert(argv);

    SIM_LOG( SIM_LOG_INFO,"readile(%s)",argv[0]);

    fnlen=strlen(argv[0]);
    if((fnlen<1)||(fnlen>=MAX_FILE_NAME_SIZE))
	{
        SIM_LOG( SIM_LOG_WARNING, "Invalid filename length:%d", fnlen);
        return SIM_FALSE;
	}

    if('"'==argv[0][0])
    {
    	strcpy(cfg_fname,argv[0]+1);
    	fnlen=strlen(cfg_fname);
    	if((fnlen<1)||('"'!=cfg_fname[fnlen-1]))
    	{
            SIM_LOG( SIM_LOG_WARNING, "Invalid filename");
            return SIM_FALSE;
    	}
    	cfg_fname[fnlen-1]=0;
    }
    else
    {
    	strcpy(cfg_fname,argv[0]);
    }

	if(PNULL == prepareTlv(cfg_fname, exp1_buf, &exp1_buf_size, exp1_mask))
	{
		SIM_LOG( SIM_LOG_WARNING, "Error reading message from file '%s'", cfg_fname);
		return SIM_FALSE;
	}

    for(k=0;k<exp1_buf_size;k++)
    {
    	if((*size)<maxSize)
    	{
    		userAddMsgByte(msgBuffer_p,size,bit_pos,exp1_buf[k],msgMask_p,exp1_mask[k]);
    	}
    	else
    	{
            SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
            return SIM_FALSE;
    	}
    }

	return SIM_TRUE;
}

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
)
{
	UInt8 exp1_buf[MAX_EXPRESSION_BUF_SIZE];
	UInt16 exp1_buf_size=0;
	UInt16 k=0;
	sim_socket_type_et sock_type=SIM_SOCKET_TYPE_UDP;
    SInt32 timeout=0;

    if( (1 != argc) && (2 != argc) )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$readsock(udp|sctp[;<timeout (ms)>)]'");
        return SIM_FALSE;
    }

    assert(argv);
    userTolower((UChar8 *)argv[0]);
    if (0==strcmp("sctp",argv[0]))
    {
        sock_type=SIM_SOCKET_TYPE_SCTP;
    }
    else
    if (0==strcmp("udp",argv[0]))
    {
        sock_type=SIM_SOCKET_TYPE_UDP;
    }
    else
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$readsock(udp|sctp[;<timeout (ms)>])'");
        return SIM_FALSE;
    }

    if( 2 == argc )
    {
        timeout=atoi(argv[1]);
    }
    else
    {
        timeout=60000; /*default is 1 minute*/
    }

    if (SIM_SOCKET_TYPE_UDP==sock_type)
    {
    	SIM_LOG( SIM_LOG_INFO, "$readsock(UDP),timeout=%i",timeout);
    }
    else
    {
    	SIM_LOG( SIM_LOG_INFO, "$readsock(SCTP),timeout=%i",timeout);
    }

    esLock();

    /*check if we have msg on socket port*/
    exp1_buf_size = ReadFromSocket( sock_type, exp1_buf ,timeout);

    esUnlock();

    if(1<=exp1_buf_size)
    {
        userPrintRawBuffer("Received", exp1_buf, exp1_buf_size, 0);
    }
    else
    {
        SIM_LOG( SIM_LOG_INFO, "Timeout");

        /*returning AA buffer*/
        exp1_buf_size=1;
        exp1_buf[0]=0xAA;
    }

    for(k=0;k<exp1_buf_size;k++)
    {
    	if((*size)<maxSize)
    	{
    		userAddMsgByte(msgBuffer_p,size,bit_pos,exp1_buf[k],msgMask_p,0xFF);
    	}
    	else
    	{
            SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
            return SIM_FALSE;
    	}
    }

	return SIM_TRUE;
}

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
)
{
    UInt8 op_res=0;

    if( 2 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$and(<expression1>;<expression2>)'");
        return SIM_FALSE;
    }

    assert(argv);

    op_res = (SIM_FALSE!=exprIsTrue(argv[0])) && (SIM_FALSE!=exprIsTrue(argv[1]));

    if((*size)<maxSize)
	{
		userAddMsgByte(msgBuffer_p,size,bit_pos,op_res,msgMask_p,0xFF);
	}
	else
	{
		SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
		return SIM_FALSE;
	}

	return SIM_TRUE;
}

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
)
{
    UInt8 op_res=0;

    if( 2 != argc )
    {
        SIM_LOG( SIM_LOG_WARNING, "Wrong parameters. Should be '$or(<expression1>;<expression2>)'");
        return SIM_FALSE;
    }

    assert(argv);

    op_res = (SIM_FALSE!=exprIsTrue(argv[0])) || (SIM_FALSE!=exprIsTrue(argv[1]));

    if((*size)<maxSize)
	{
		userAddMsgByte(msgBuffer_p,size,bit_pos,op_res,msgMask_p,0xFF);
	}
	else
	{
		SIM_LOG(SIM_LOG_WARNING, "%s:Message size too big",__FUNCTION__);
		return SIM_FALSE;
	}

	return SIM_TRUE;
}

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
)
{
    UInt32      argc = 0;
    UChar8**    argv = PNULL;
    UInt32 		i=0;
    UInt32      result = SIM_FALSE;
    UInt8		op_found=SIM_FALSE;

    argv = userGetOpParamsListStrings(params, ';');
    argc = userGetListStringsCount((const UChar8**)argv);

    /* find and run operation handler */
    for(i = 0; table_operation_func[i].str_id; i++)
    {
        userTolower(op_name);
        if ( 0 == strcmp(op_name, table_operation_func[i].str_id) )
        {
        	op_found = SIM_TRUE;
            result = (*table_operation_func[i].pfunc)(argc,
            	(const UChar8**)argv,
            	maxSize,
            	msgBuffer_p,
            	size,
            	bit_pos,
            	msgMask_p);
            break;
        }
    }

    if(SIM_TRUE!=op_found)
    {
    	SIM_LOG( SIM_LOG_WARNING, "%s:Unknown operation $%s",__FUNCTION__,op_name);
    }


    if (PNULL != argv)
    {
        userFreeListStrings(argv);
    }

	return result;
}
