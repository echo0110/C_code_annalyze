/**************************************************************************************
*
*  FILE NAME  : aes_ia.h
*
*  DESCRIPTION: functions to calculate integrity using AES CMAC (EIA2) algo
*
*
*	DATE	NAME	REFERENCE	REASON
*
*  Copyright 2009, Aricent Inc.
*
*************************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/

#include "aes_ia.h"
#include "cmac.h"
#include "integrity.h"

/****************************************************************************
 * Functions implementation 
 ****************************************************************************/

/****************************************************************************
 * Function Name  : aes_eia_ZCB
 * Inputs         : UInt8* key - 128 bit Integrity Key
 *					UInt16 bearer - same as lcId
 *					UInt32 count
 *					UInt16 dir - direction, 0(downlink) or 1(uplink)
 *					void *data_ZCB - ZCB with data to calc MACI for
 *					UInt32 byte_length - length of data in data_ZCB to calc MACI
 * Outputs        : UInt8 *res_MAC_I - calculated MACI will be placed here
 * Returns        : None
 * Description    : Calculates 32-bit MACI using AES-CMAC (EIA2) algo
 ****************************************************************************/
void aes_eia_ZCB(UInt8* key,UInt16 bearer,UInt32 count,UInt16 dir,void *data_ZCB,UInt32 byte_length,UInt8 *res_MAC_I)
{
	cmac_state ctx;
	ZCB_state ZCB_st;

	UInt8 buf[8];
	dir&=1;
	
	buf[0]=(UInt8)(count>>24);
	buf[1]=(UInt8)(count>>16);
	buf[2]=(UInt8)(count>>8);
	buf[3]=(UInt8)count;
	/* SPR 1975 changes start */
    buf[4]=(UInt8)(((bearer&0x1F)<<3) | ((dir&0x01)<<2));
	/* SPR 1975 changes end */
	buf[5]=buf[6]=buf[7]=0;
    /* +- SPR 17777 */
	cmac_init(key,&ctx);
    /* +- SPR 17777 */
	cmac_data(buf,8,&ctx);

	Init_ZCB_state(data_ZCB,&ZCB_st);
	while(byte_length>0)
	{
		if(PNULL==ZCB_st.last)
		{
			/*PDCP_LOG(LOG_DUMP,"ZCB_st.last==0 in aes_eia_ZCB");*/
			break;
		}
		if(ZCB_st.segment.size>0)
		{
			if(ZCB_st.segment.size<byte_length)
			{
				cmac_data(ZCB_st.segment.base,ZCB_st.segment.size,&ctx);
				byte_length-=ZCB_st.segment.size;
			}
			else
			{
				cmac_data(ZCB_st.segment.base,byte_length,&ctx);
				break;
			}
			if(PNULL==ZCB_next_segment(&ZCB_st))
			{
				/*PDCP_LOG(LOG_DUMP,"End of ZCB buffer in aes_eia_ZCB");*/
				break;
			}
		}
                /* Coverity 5.0.1 ID:24963 fix start*/
                else 
                {
                   break;
                }
                /* Coverity 5.0.1 ID:24963 fix end*/
	}

	cmac_end(res_MAC_I,MACI_LEN,&ctx);
}
