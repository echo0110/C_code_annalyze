/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: ltePdcpCmnTools.c,v 1.1.1.1.16.1 2010/10/25 08:04:11 gur21010 Exp $
 *
 ******************************************************************************
 *
 *  File Description : Functions to work with ZCB and other PDCP common parts 
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: ltePdcpCmnTools.c,v $
 * Revision 1.1.1.1.16.1  2010/10/25 08:04:11  gur21010
 * warning removed
 *
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.5.2.2  2009/07/28 15:36:33  gur19836
 * Downlink Issues Resolved, Latency Measurement code added
 *
 * Revision 1.5.2.1  2009/06/09 11:30:25  gur19140
 * first wave of Integration
 *
 * Revision 1.5  2009/05/20 13:45:41  gur20548
 * header file name changed
 *
 * Revision 1.4  2009/05/14 13:09:20  gur19836
 * CR Comments Incorporated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "ltePdcpCmnTools.h"
/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
ModuleLogDetail *pdcpModuleLogDetail_p = PNULL;

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : initPdcpLog
 * Inputs         : void *fn - filename for log, PNULL if use LTE_CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : Inits pdcp log context 
 ****************************************************************************/
void initPdcpLog(void *fn)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(fn)
    /* +- SPR 17777 */
}

/****************************************************************************
 * Function Name  : redirectPdcpLog
 * Inputs         : void *fn - filename for log, PNULL if use LTE_CONSOLE_DEV
 * Outputs        : None.
 * Returns        : None.
 * Description    : redirect pdcp log output
 ****************************************************************************/
void redirectPdcpLog(void *fn)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(fn)
    /* +- SPR 17777 */
}

/****************************************************************************
 * Function Name  : pdcpLogLevel
 * Inputs         : UInt32 level
 * Outputs        : None.
 * Returns        : None.
 * Description    : Sets pdcp log level
 ****************************************************************************/
void pdcpLogLevel(UInt32 level)
{
    if(PNULL != pdcpModuleLogDetail_p)
    {
        pdcpModuleLogDetail_p->level = level;
    }
}

/****************************************************************************
 * Function Name  : pdcpGetLogLevel
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : Returns pdcp log level
 * Description    : Returns pdcp log level
 ****************************************************************************/
UInt32 pdcpGetLogLevel(void)
{
    if(PNULL != pdcpModuleLogDetail_p)
    {
        return pdcpModuleLogDetail_p->level;
    }
    else
    {
        return LOG_NONE;
    }
}

/****************************************************************************
 * Function Name  : cleanPdcpLog
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Cleans pdcp log context 
 ****************************************************************************/
void cleanPdcpLog(void)
{
    /*do not clean pdcp context to preserve filename*/
}

/****************************************************************************
 * Function Name  : pdcpSwap16
 * Inputs         : UInt16 value
 * Outputs        : None.
 * Returns        : value with swapped bytes
 * Description    : calculates value with swapped bytes
 ****************************************************************************/
UInt16 pdcpSwap16(UInt16 value)
{
    return ((value&0xFF)<<8)|((value>>8)&0xFF);
}

/****************************************************************************
 * Function Name  : pdcpSwap32
 * Inputs         : UInt32 value
 * Outputs        : None.
 * Returns        : value with swapped bytes
 * Description    : calculates value with swapped bytes
 ****************************************************************************/
UInt32 pdcpSwap32(UInt32 value)
{
    return ((value&0xFF)<<24) |
        (((value>>8)&0xFF)<<16) |
        (((value>>16)&0xFF)<<8) |
        ((value>>24)&0xFF);
}

/****************************************************************************
 * Function Name  : Init_ZCB_state
 * Inputs         : buffer - ZCB,
 *                  st - ZCB state pointer
 * Outputs        : None.
 * Returns        : None
 * Description    : Initialize ZCB iterator
 ****************************************************************************/
void Init_ZCB_state(void *buffer,ZCB_state_PTR st)
{
    /*	PDCP_LOG(LOG_DUMP,"Number of segments=%i",msgSegCount(buffer));*/
    if(PNULL == buffer)
    {
        PDCP_LOG(LOG_DUMP,"Error in Init_ZCB_state: buffer==0");
    }
    st->buffer = buffer;
    st->last = msgSegNext(buffer, 0, &st->segment);
    if(PNULL == st->last)
    {
        st->isEnd = LTE_TRUE;
        PDCP_LOG(LOG_DUMP,"Error in Init_ZCB_state: st->last==0");
    }
    else
    {
        st->isEnd = LTE_FALSE;
    }
    /*PDCP_LOG(LOG_DUMP,"segment.size=%i",st->segment.size);*/
    st->cur_pos = 0;
}

/****************************************************************************
 * Function Name  : GetNextUInt8_ZCB
 * Inputs         : st - ZCB state pointer
 * Outputs        : None
 * Returns        : Next byte from ZCB 
 * Description    : Returns next BYTE from ZCB. When reading after end of 
 *                  buffer, returns 0.
 ****************************************************************************/
UInt8 GetNextUInt8_ZCB(ZCB_state_PTR st)
{
    if(LTE_FALSE!=st->isEnd)
    {
        return 0;
    }
    while(st->cur_pos>=st->segment.size)
    {
        st->last = msgSegNext(st->buffer, st->last, &st->segment);
        if(PNULL==st->last)
        {
            st->isEnd=LTE_TRUE;
            return 0;
        }
        st->cur_pos=0;
    }
    return st->segment.base[st->cur_pos++];
}

/****************************************************************************
 * Function Name  : GetNextZCB
 * Inputs         : dst_buf - Destination buffer pointer
 *                  size - Number of bytes to be copied
 *                  st - ZCB state pointer
 * Outputs        : None.
 * Returns        : Number of copied bytes 
 * Description    : Copies next size BYTEs from ZCB to dst_buf.Returns number 
 *                  of copied bytes.
 ****************************************************************************/
UInt32 GetNextZCB(UInt8 *dst_buf,SInt32 size,ZCB_state_PTR st)
{
    SInt32 i=0;
    SInt32 part_size=0;
    if(LTE_FALSE!=st->isEnd)
    {
        return 0;
    }
    while(size>0)
    {
        part_size = st->segment.size-st->cur_pos;
        while(part_size==0)
        {
            st->last = msgSegNext(st->buffer, st->last, &st->segment);
            if(PNULL==st->last)
            {
                st->isEnd=LTE_TRUE;
                return i;
            }
            st->cur_pos=0;
            part_size = st->segment.size;
        }
        if(part_size>size)
        {
            part_size=size;
        }
        memCpy(dst_buf+i,st->segment.base+st->cur_pos,part_size);
        i+=part_size;
        st->cur_pos+=part_size;
        size-=part_size;
    }
    return i;
}

/****************************************************************************
 * Function Name  : SeekZCB
 * Inputs         : position - Specified position in ZCB
 *                  st - ZCB state pointer
 * Outputs        : None.
 * Returns        : New position offset 
 * Description    : Seek to specified position in ZCB. Returns new position 
 *                  offset in ZCB.
 ****************************************************************************/
UInt32 SeekZCB(SInt32 position,ZCB_state_PTR st)
{
    SInt32 i=0;
    SInt32 part_size=0;
    st->last = msgSegNext(st->buffer, 0, &st->segment);
    if(PNULL==st->last)
    {
        st->isEnd=LTE_TRUE;
        return 0;
    }
    else
    {
        st->isEnd=LTE_FALSE;
    }
    st->cur_pos=0;

    while(i<position)
    {
        part_size = st->segment.size-st->cur_pos;
        while(part_size==0)
        {
            st->last = msgSegNext(st->buffer, st->last, &st->segment);
            if(PNULL==st->last)
            {
                st->isEnd=LTE_TRUE;
                return i;
            }
            st->cur_pos=0;
            part_size = st->segment.size;
        }
        if(part_size>(position-i))
        {
            part_size=position-i;
        }
        i+=part_size;
        st->cur_pos+=part_size;
    }
    return i;
}

/****************************************************************************
 * Function Name  : GetNextUInt32_ZCB
 * Inputs         : st - ZCB state pointer
 * Outputs        : None.
 * Returns        : Next DWORD from ZCB 
 * Description    : It returns the next DWORD from ZCB. 
 ****************************************************************************/
UInt32 GetNextUInt32_ZCB(ZCB_state_PTR st)
{
    UInt32 res;
    res=(UInt32)GetNextUInt8_ZCB(st)<<24;
    res|=(UInt32)GetNextUInt8_ZCB(st)<<16;
    res|=(UInt32)GetNextUInt8_ZCB(st)<<8;
    res|=(UInt32)GetNextUInt8_ZCB(st);
    return res;
}

/****************************************************************************
 * Function Name  : GetNextUInt16_ZCB
 * Inputs         : st - ZCB state pointer
 * Outputs        : None.
 * Returns        : Next WORD from ZCB 
 * Description    : It returns the next WORD from ZCB. 
 ****************************************************************************/
UInt16 GetNextUInt16_ZCB(ZCB_state_PTR st)
{
    UInt16 res;
    res=(UInt32)GetNextUInt8_ZCB(st)<<8;
    res|=(UInt32)GetNextUInt8_ZCB(st);
    return res;
}

/****************************************************************************
 * Function Name  : MakeUInt32
 * Inputs         : x - Pointer to a 4 byte array
 * Outputs        : None.
 * Returns        : DWORD made from byte[4] array 
 * Description    : It Make DWORD from byte[4] array. 
 ****************************************************************************/
/*Make DWORD from byte[4] array*/
UInt32 MakeUInt32(UInt8 *x)
{
    return (x[0]<<24)|(x[1]<<16)|(x[2]<<8)|x[3];
}

/****************************************************************************
 * Function Name  : ZCB_next_segment
 * Inputs         : st - ZCB state pointer
 * Outputs        : None.
 * Returns        : Next segment of ZCB  
 * Description    : It Move to next ZCB. Return st->last, 
 *                  0 if no next segment. 
 ****************************************************************************/
void *ZCB_next_segment(ZCB_state_PTR st)
{
    st->last = msgSegNext(st->buffer, st->last, &st->segment);
    if(PNULL==st->last)
    {
        st->buffer=PNULL;
    }
    st->cur_pos=0;
    return st->last;
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
#define LOG_MAX_STRING_LEN  1024
void userPrintRawBuffer(const Char8* strHeader, const UInt8* buf, UInt32 len_buf, UInt32 fmt)
{
    /* +- SPR 17777 */
  LTE_GCC_UNUSED_PARAM(strHeader)
  LTE_GCC_UNUSED_PARAM(buf)
  LTE_GCC_UNUSED_PARAM(len_buf)
  LTE_GCC_UNUSED_PARAM(fmt)
    /* +- SPR 17777 */
}

#define PDCP_MAX_PRINT_BUFF_LEN 200
char g_pdcp_print_buff[PDCP_MAX_PRINT_BUFF_LEN];
char* pdcp_change_data_to_str(UInt16 len, UInt8* data)
{
	UInt16 cnt, curlen = 0;
	if (len * 2 + 1 > PDCP_MAX_PRINT_BUFF_LEN)
	{
		len = (PDCP_MAX_PRINT_BUFF_LEN>>1) - 1;
	}

	for (cnt = 0; cnt < len; cnt++)
	{
		sprintf(g_pdcp_print_buff + curlen, "%02X", data[cnt]);
		curlen += 2;
	}
	g_pdcp_print_buff[curlen] = 0;
	return g_pdcp_print_buff;
}

