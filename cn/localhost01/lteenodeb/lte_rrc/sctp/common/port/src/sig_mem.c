/*****************************************************************************
 *    FILE NAME:
 *        sig_mem.c
 *
 *    DESCRIPTION:
 *       This file has porting function for Memory Related functions.
 *       This file has no GL related functions, this is tobe used when GL
 *       is not required to be Linked.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    14Sept02    Sandeep Mahajan     -            .Original Creation
 *    28Feb03     Siddharth Gupta     -            Adding function for memory
 *                                                 compare.
 *    15NOv2003   Vishavdeep sharma   -            Rel 5.0
 *
 *
 *     Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#include <sig_os.h>

/******************************************************************************
**  FUNCTION :    sigtran_malloc
**
**  DESCRIPTION:  This function Get memory by using Malloc
**
**  RETURN VALUE: sigtran_buffer_t
******************************************************************************/
sigtran_buffer_t sigtran_malloc(
    sigtran_U32bit         size )
{
#ifdef SIG_COMMON_WITH_CSPL
    /* Directly Call the Malloc functions */
    return ( sigtran_buffer_t )qvSysMalloc( size );
#else
    return ( sigtran_buffer_t )sigtran_os_malloc( size );
#endif

}

/******************************************************************************
**  FUNCTION :    sigtran_free
**
**  DESCRIPTION:  This function Free memory taken by Malloc
**
**  RETURN VALUE: None
******************************************************************************/
sigtran_void_t  sigtran_free(
    sigtran_buffer_t      p_buffer )
{
#ifdef SIG_COMMON_WITH_CSPL
    qvSysFree( p_buffer );
#else
    sigtran_os_free( ( sigtran_void_t * )p_buffer );
#endif
}



