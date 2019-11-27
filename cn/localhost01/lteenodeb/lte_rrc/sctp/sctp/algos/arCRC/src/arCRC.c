/******************************************************************************
*
* FILE NAME    : arCRC.c
*
* DESCRIPTION  : Contains the CRC Checksum routines
*
* Revision History :
*
*  DATE            NAME                 REFERENCE       REASON
* ------         ------------           ---------     ----------
* 24-Feb-2009    Shivi Singh Verma          None          Initial
*
* Copyright 2009, Aricent.
*
*******************************************************************************/


#include "arCRC_api.h"

UINT4 g_crc[ARCRC_TABLE_SIZE];
UINT4 g_is_crc_init = ARCRC_FALSE;


/****************************************************************************
 * API Name:  arCRC_reflect_32
 *
 * Description: This API is invoked to reflect the 32 bit value
 *
 * Return : It will return unsigned int containing the reflected value.
 ******************************************************************************/
static UINT8 arCRC_reflect_32(
    UINT8 b )
{
    UINT4 i;
    UINT8 rw = 0L;

    for( i = 0; i < 32; i++ )
    {
        if( b & 1 ) /* CSR 1-8128593 fix starts*/
        {
            rw |= ( UINT8 )( 1 << ( 31 - i ) );
        }

        /* CSR 1-8128593 fix ends*/
        b >>= 1;
    }

    return ( rw );
}

/****************************************************************************
 * API Name:  arCRC_build_table
 *
 * Description: This API is invoked to build the CRC lookup table
 *
 * Return : It will return unsigned long long containing the CRC lookup table
 * index.
 ******************************************************************************/
static UINT8 arCRC_build_table(
    INT4 index )
{
    UINT4 i;
    UINT8 rb;

    rb = arCRC_reflect_32( index );

    for( i = 0; i < 8; i++ )
    {
        if( rb & 0x80000000L )
        {
            rb = ( rb << 1 ) ^ ARCRC_POLY;
        }

        else
        {
            rb <<= 1;
        }
    }

    return ( arCRC_reflect_32( rb ) );
}

/****************************************************************************
 * API Name:  arCRC_init
 *
 * Description: This API is invoked to initialize the CRC lookup table
 *
 * Return : None
 ******************************************************************************/
static VOID arCRC_init( )
{
    UINT4 i;

    for( i = 0; i < ARCRC_TABLE_SIZE; i++ )
    {
        /*windows fix*/
#if defined (SIGTRAN_PORT_WINXP) || defined( SIGTRAN_PORT_WINNT)
        g_crc[i] = ( UINT4 )arCRC_build_table( i );
#else
        g_crc[i] = arCRC_build_table( i );
#endif
        /*windows fix*/
    }
}

/****************************************************************************
 * API Name:  arCRC_compute_chcksum
 *
 * Description: This API is invoked to compute the CRC checksum of the given
 * continous data stream
 *
 * Return : It will return unsigned int containing the CRC checksum value.
 ******************************************************************************/
UINT4 arCRC_compute_chcksum(
    UINT1 *p_dataBuf,
    UINT4 len,
    UINT4 *p_error )
{
    UINT4 i, result;
    UINT4 index = 0;
    UINT4 crc = ( UINT4 )~0L;
    UINT1 byte0, byte1, byte2, byte3;

    if( p_error == ARCRC_NULL )
    {
#ifdef  ARCRC_DEBUG_LOG
        ARCRC_TRACE( "CRC: ERROR: p_error NULL received\n" );
#endif
        return ARCRC_INVALID;
    }

    *p_error = E_NO_ERROR;
    /*Initialize the error code with the E_NO_ERROR */

    if( p_dataBuf == ARCRC_NULL )
    {
        /*Set the error code with the E_NULL_BUFFER_RECEIVED as NULL data buffer is received */
        *p_error = E_NULL_BUFFER_RECEIVED;
#ifdef  ARCRC_DEBUG_LOG
        ARCRC_TRACE( "CRC: ERROR: Data Buffer NULL received\n" );
#endif
        return ARCRC_INVALID;
    }

    if( ARCRC_FALSE ==  g_is_crc_init )
    {
        arCRC_init();
        g_is_crc_init = ARCRC_TRUE;
    }

    /*
     * For 4 bytes messages multiple, calculate CRC.
     */
    for( i = 0; i < ( len / ARCRC_CIRCULAR_BASE ); i++ )
    {
        index = ARCRC_CIRCULAR_BASE * i;
        ARCRC_32C( crc, p_dataBuf[index] );
        ARCRC_32C( crc, p_dataBuf[index + 1] );
        ARCRC_32C( crc, p_dataBuf[index + 2] );
        ARCRC_32C( crc, p_dataBuf[index + 3] );
    }

    /*
     * For remaining bytes, calculate CRC.
     */
    index = ARCRC_CIRCULAR_BASE * ( len / ARCRC_CIRCULAR_BASE );

    for( i = 0; i < ( len % ARCRC_CIRCULAR_BASE ) ; i++ )
    {
        ARCRC_32C( crc, p_dataBuf[index] );
        index++;
    }

    result = ~crc;

    byte0 = result & 0xff;
    byte1 = ( result >> 8 ) & 0xff;
    byte2 = ( result >> 16 ) & 0xff;
    byte3 = ( result >> 24 ) & 0xff;

    crc = ( ( byte0 << 24 ) |
            ( byte1 << 16 ) |
            ( byte2 << 8 )  |
            byte3 );
    return ( crc );
}
/****************************************************************************
 * API Name:  arCRC_compute_chcksum_iovec
 *
 * Description: This API is invoked to compute the CRC checksum of the given
 * scattered data stream
 *
 * Return : It will return unsigned int containing the CRC checksum value.
 ******************************************************************************/
UINT4 arCRC_compute_chcksum_iovec( IOVEC *p_iovec, UINT4 iovec_len, UINT4 *p_error )
{
    UINT4 i, j, result;
    UINT1 *p_dataBuf;
    UINT4 len;
    UINT4 index = 0;

    UINT4 crc = ( UINT4 )~0L;
    UINT1 byte0, byte1, byte2, byte3;

    if( p_error == ARCRC_NULL )
    {
#ifdef  ARCRC_DEBUG_LOG
        ARCRC_TRACE( "CRC: ERROR: p_error NULL received\n" );
#endif
        return ARCRC_INVALID;
    }

    *p_error = E_NO_ERROR;
    /*Initialize the error code with the E_NO_ERROR */

    if( p_iovec == ARCRC_NULL )
    {
        /*Set the error code with the E_NULL_BUFFER_RECEIVED as NULL data buffer is received */
        *p_error = E_NULL_BUFFER_RECEIVED;
#ifdef  ARCRC_DEBUG_LOG
        ARCRC_TRACE( "CRC: ERROR: p_iovec Buffer NULL received\n" );
#endif
        return ARCRC_INVALID;
    }

    for( i = 0; i < iovec_len; i++ )
    {
        if( ARCRC_NULL == ( UINT1 * )p_iovec[i].iov_base )
        {
            *p_error = E_NULL_BUFFER_RECEIVED;
#ifdef  ARCRC_DEBUG_LOG
            ARCRC_TRACE( "CRC: ERROR: p_iovec %dth iov_base NULL received\n", i );
#endif

            return ARCRC_INVALID;
        }
    }

    if( ARCRC_FALSE ==  g_is_crc_init )
    {
        arCRC_init();
        g_is_crc_init = ARCRC_TRUE;
    }

    for( i = 0; i < iovec_len; i++ )
    {
        p_dataBuf = ( UINT1 * )p_iovec[i].iov_base;
        len = p_iovec[i].iov_len;

        /*
         * For 4 bytes messages multiple, calculate CRC.
         */
        for( j = 0; j < ( len / ARCRC_CIRCULAR_BASE ); j++ )
        {
            index = ARCRC_CIRCULAR_BASE * j;
            ARCRC_32C( crc, p_dataBuf[index] );
            ARCRC_32C( crc, p_dataBuf[index + 1] );
            ARCRC_32C( crc, p_dataBuf[index + 2] );
            ARCRC_32C( crc, p_dataBuf[index + 3] );
        }

        /*
         * For remaining bytes, calculate CRC.
         */
        index = ARCRC_CIRCULAR_BASE * ( len / ARCRC_CIRCULAR_BASE );

        for( j = 0; j < ( len % ARCRC_CIRCULAR_BASE ) ; j++ )
        {
            ARCRC_32C( crc, p_dataBuf[index] );
            index++;
        }
    }


    result = ~crc;

    byte0 = result & 0xff;
    byte1 = ( result >> 8 ) & 0xff;
    byte2 = ( result >> 16 ) & 0xff;
    byte3 = ( result >> 24 ) & 0xff;

    crc = ( ( byte0 << 24 ) |
            ( byte1 << 16 ) |
            ( byte2 << 8 )  |
            byte3 );

    return ( crc );
}

