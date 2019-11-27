/******************************************************************************
*
* FILE NAME    : arAdler.c
*
* DESCRIPTION  : Contains the Adler Checksum routines
*
* Revision History :
*
*  DATE            NAME                 REFERENCE       REASON
* ------         ------------           ---------     ----------
* 11-Feb-2009    Rajnesh Tyagi          None          Initial
*
* Copyright 2009, Aricent.
*
*******************************************************************************/

#include "arAdler_api.h"

/****************************************************************************
 * API Name: arAdler_update_chcksum
 *
 * Description: This API is invoked to update the checksum
 *
 * Return : It will return unsigned int containing the Adler checksum.
 ******************************************************************************/

static UINT4 arAdler_update_chcksum(
    UINT1 *p_dataBuf,
    UINT4 len )
{
    UINT4 sum1 = 1;
    UINT4 sum2 = 0;
    UINT4 k;

    while( len > 0 )
    {
        k = len < ARADLER_NMAX ? len : ARADLER_NMAX;
        len -= k;

        while( k >= 32 )
        {
            ARADLER_SUM_BYTE32( p_dataBuf );
            p_dataBuf += 32;
            k -= 32;
        }

        if( k != 0 ) do
            {
                ARADLER_SUM_BYTE1( p_dataBuf, 0 );
                p_dataBuf ++;
            }
            while( --k );

        sum1 %= ARADLER_BASE;
        sum2 %= ARADLER_BASE;
    }

    return ( sum2 << 16 ) | sum1;
}


/****************************************************************************
 * API Name: arAdler_compute_chcksum
 *
 * Description: This API is invoked to calculate the checksum
 *
 * Return : It will return unsigned int containing the Adler checksum.
 ******************************************************************************/

UINT4 arAdler_compute_chcksum(
    UINT1 *p_dataBuf,
    UINT4 len,
    UINT4 *p_error )
{
    if( p_error == ARADLER_NULL )
    {
#ifdef  ARADLER_DEBUG_LOG
        ARADLER_TRACE( "ADLER: ERROR: p_error NULL received\n" );
#endif
        return ARADLER_INVALID;
    }

    *p_error = E_NO_ERROR;

    if( p_dataBuf == ARADLER_NULL )
    {
        *p_error = E_NULL_BUFFER_RECEIVED;
#ifdef  ARADLER_DEBUG_LOG
        ARADLER_TRACE( "ADLER: ERROR: Data Buffer NULL received\n" );
#endif

        return ARADLER_INVALID;
    }

    return arAdler_update_chcksum( p_dataBuf, len );
}

/****************************************************************************
 * API Name: arAdler_update_chcksum_iovec
 *
 * Description: This API is invoked to update the checksum
 *
 * Return : It will return unsigned int containing the Adler checksum.
 ******************************************************************************/

static UINT4 arAdler_update_chcksum_iovec(
    IOVEC *p_iovec,
    UINT4 iovec_len )
{
    UINT4 sum1 = 1;
    UINT4 sum2 = 0;
    UINT1 *p_dataBuf;
    UINT4 len;
    UINT4 k, i;

    for( i = 0; i < iovec_len; i++ )
    {
        p_dataBuf = ( UINT1 * )p_iovec[i].iov_base;
        len = p_iovec[i].iov_len;

        while( len > 0 )
        {
            k = len < ARADLER_NMAX ? len : ARADLER_NMAX;
            len -= k;

            while( k >= 32 )
            {
                ARADLER_SUM_BYTE32( p_dataBuf );
                p_dataBuf += 32;
                k -= 32;
            }

            if( k != 0 ) do
                {
                    ARADLER_SUM_BYTE1( p_dataBuf, 0 );
                    p_dataBuf ++;
                }
                while( --k );

            sum1 %= ARADLER_BASE;
            sum2 %= ARADLER_BASE;
        }
    }

    return ( sum2 << 16 ) | sum1;
}
/****************************************************************************
 * API Name: arAdler_compute_chcksum_iovec
 *
 * Description: This API is invoked to calculate the checksum
 *
 * Return : It will return unsigned int containing the Adler checksum.
 ******************************************************************************/

UINT4 arAdler_compute_chcksum_iovec(
    IOVEC *p_iovec,
    UINT4 iovec_len,
    UINT4 *p_error )
{
    UINT4 i;

    if( p_error == ARADLER_NULL )
    {
#ifdef  ARADLER_DEBUG_LOG
        ARADLER_TRACE( "ADLER: ERROR: p_error NULL received\n" );
#endif
        return ARADLER_INVALID;
    }

    *p_error = E_NO_ERROR;

    if( ARADLER_NULL == p_iovec )
    {
        *p_error = E_NULL_BUFFER_RECEIVED;
#ifdef  ARADLER_DEBUG_LOG
        ARADLER_TRACE( "ADLER: ERROR: p_iovec NULL received\n" );
#endif

        return ARADLER_INVALID;
    }

    for( i = 0; i < iovec_len; i++ )
    {
        if( ARADLER_NULL == ( UINT1 * )p_iovec[i].iov_base )
        {
            *p_error = E_NULL_BUFFER_RECEIVED;
#ifdef  ARADLER_DEBUG_LOG
            ARADLER_TRACE( "ADLER: ERROR: p_iovec %dth iov_base NULL received\n", i );
#endif

            return ARADLER_INVALID;
        }
    }

    return arAdler_update_chcksum_iovec( p_iovec, iovec_len );
}
