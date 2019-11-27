/******************************************************************************
*
* FILE NAME    : arAdler.h
*
* DESCRIPTION  : Contains the Macro's for the Adler Algo implementation
*
* Revision History :
*
*  DATE            NAME                 REFERENCE       REASON
* ------         ------------           ---------     ----------
* 11-Feb-2009    Rajnesh Tyagi          None          Initial
*
* Copyright 2008, Aricent.
*
*******************************************************************************/
#ifndef __ARADLER_H__
#define __ARADLER_H__
#define    ARADLER_BASE    65521 /* largest prime smaller than 65536 2^16*/

#define ARADLER_NMAX 5552

#define ARADLER_SUM_BYTE1(dataBuf,i)  {sum1 += dataBuf[i]; sum2 += sum1;}
#define ARADLER_SUM_BYTE2(dataBuf,i)  ARADLER_SUM_BYTE1(dataBuf,i); ARADLER_SUM_BYTE1(dataBuf,i+1);
#define ARADLER_SUM_BYTE4(dataBuf,i)  ARADLER_SUM_BYTE2(dataBuf,i); ARADLER_SUM_BYTE2(dataBuf,i+2);
#define ARADLER_SUM_BYTE8(dataBuf,i)  ARADLER_SUM_BYTE4(dataBuf,i); ARADLER_SUM_BYTE4(dataBuf,i+4);
#define ARADLER_SUM_BYTE16(dataBuf)   ARADLER_SUM_BYTE8(dataBuf,0); ARADLER_SUM_BYTE8(dataBuf,8);

#define ARADLER_SUM_BYTE16_NEW(dataBuf, i)   ARADLER_SUM_BYTE8(dataBuf,i); ARADLER_SUM_BYTE8(dataBuf,i+8);
#define ARADLER_SUM_BYTE32(dataBuf)          ARADLER_SUM_BYTE16_NEW(dataBuf,0); ARADLER_SUM_BYTE16_NEW(dataBuf,16);
#endif
