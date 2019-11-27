
/*****************************************************************************
 *    FILE NAME:
 *        sig_buff.c
 *
 *    DESCRIPTION:
 *       This file has porting function for Memory Related functions.
 *       NOTE: By default this file is ported for Genlib (HSS Generic
 *             Library) Memory Manager. This needs to be changed if
 *             Genlib is not used and some other memory manager is being
 *             used.
 *
 *    DATE           NAME          REFERENCE        REASON
 *    ----------------------------------------------------
 *    14Sept02    Sandeep Mahajan     -            .Original Creation
 *    20NOv03     Vivek bansal        -           cspl porting in rel 5.0
 *    18Apr13     Pranav          SPR 21127       CSR 40731-GCC 4.7 warning removal
 *      Copyright (C) 2006 Aricent Inc . All Rights Reserved
 *****************************************************************************/

#include <sig_mem.h>
#include <sig_defs.h>
#include <sig_typs.h>
#include <sig_err.h>
#include <sig_util.h>


sigtran_boolean_t  sig_mem_state = SIGTRAN_FALSE;

/******************************************************************************
**  FUNCTION :    sigtran_destroy_all_cmn_pools
**
**  DESCRIPTION:  This function destroys all common pools and deinit gel lib
**                memory mgr. This is required by UA's using hash tables of
**                genlib as genlib is taking memory from common pools but at
**                the time of deinit hash tables , GL itself is not destroying
**                the common_pools
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
void  sigtran_destroy_all_cmn_pools( )
{
    /*
     * Don't know how to deallocate memory for all common pools in case
     * of CSPL.
     */
    sig_mem_state = SIGTRAN_FALSE;

    return;
}

/******************************************************************************
**  FUNCTION :    sigtran_mem_init
**
**  DESCRIPTION:  This function initialises the Memory Manager with the
**                Passed Common Pools and Task Specific Pools.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_mem_init(
    sigtran_U32bit              task_id,
    sigtran_U32bit              num_common_pools,
    sigtran_pool_info_st        *p_comm_pool_info,
    sigtran_pool_data_st        *p_common_pool_data, /*CMN Pool Info,OUT*/
    sigtran_U32bit              num_task_pools,
    sigtran_pool_info_st        *p_list_pool_info,
    sigtran_pool_id_t           *p_pool_id_list, /* List of Pool Ids,OUT*/
    sigtran_boolean_t           b_init_mesg_pool,
    sigtran_error_t             *p_ecode )
{
#ifdef SIG_COMMON_WITH_CSPL
    sigtran_U32bit   count;
    sig_q_rpooltuning_st      pool_tuning;
    sigtran_error_t           p_temp_ecode;

    task_id = task_id;

    if( p_ecode == NULL )
    {
        p_ecode = &p_temp_ecode;
    }

    sigtran_memset( &pool_tuning, 0, sizeof( sig_q_rpooltuning_st ) );

    /* Initialise the Generic Memory manager, if not done already */
    if( sig_mem_state != SIGTRAN_TRUE )
    {
        sig_mem_state = SIGTRAN_TRUE;
    }

    pool_tuning.npools = num_common_pools;

    /* Initialise the Task Common Pools */
    for( count  = 0; count < num_common_pools; count++ )
    {
        pool_tuning.qpt[count].bufsize  = p_comm_pool_info[count].buf_size + 4;
        pool_tuning.qpt[count].nbufs    = p_comm_pool_info[count].num_buf;
    }

    /*
     * Create Task Specific Common Pool.
     */

    if( num_common_pools > 0 )
    {
        if( SIGTRAN_FAILURE == b_init_mesg_pool )
        {
            /* Memory for the normal processing */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            *p_common_pool_data = qvPoolSetCreate( &pool_tuning, p_ecode );
#else
            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            *p_common_pool_data = qvPoolSetCreate( &pool_tuning, ( sigtran_S32bit * )p_ecode );
#endif
        }

        else
        {
            /* Memory for the messaging */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            *p_common_pool_data = qvMsgPoolSetCreate( &pool_tuning, p_ecode );
#else
            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            *p_common_pool_data = qvMsgPoolSetCreate( &pool_tuning, ( sigtran_S32bit * )p_ecode );
#endif
        }

        if( ( QVERROR_NONE != *p_ecode ) || ( NULL == *p_common_pool_data ) )
        {
            return SIGTRAN_FAILURE;
        }

    }

    for( count  = 0; count < num_task_pools; count++ )
    {
        /*
         * Create Task Specific Pools.
         */
        pool_tuning.qpt[0].bufsize  = p_list_pool_info[count].buf_size + 4;
        pool_tuning.qpt[0].nbufs    = p_list_pool_info[count].num_buf;
        pool_tuning.npools = 1;

        if( SIGTRAN_FAILURE == b_init_mesg_pool )
        {
            /* Memory for the normal processing */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            p_pool_id_list[count] = qvPoolSetCreate( &pool_tuning, p_ecode );
#else
            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            p_pool_id_list[count] = qvPoolSetCreate( &pool_tuning, ( sigtran_S32bit * )p_ecode );
#endif
        }

        else
        {
            /* Memory for the messaging */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            p_pool_id_list[count] = qvMsgPoolSetCreate( &pool_tuning, p_ecode );
#else
            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            p_pool_id_list[count] = qvMsgPoolSetCreate( &pool_tuning, ( sigtran_S32bit * )p_ecode );
#endif
        }

        if( ( QVERROR_NONE != *p_ecode ) || ( NULL == p_pool_id_list[count] ) )
        {
            return SIGTRAN_FAILURE;
        }


    }

    return SIGTRAN_SUCCESS;
#else
    task_id = task_id;
    num_common_pools = num_common_pools;
    p_comm_pool_info = p_comm_pool_info;
    p_common_pool_data = p_common_pool_data;
    num_task_pools = num_task_pools;
    p_list_pool_info = p_list_pool_info;
    p_pool_id_list = p_pool_id_list;
    b_init_mesg_pool = b_init_mesg_pool;
    p_ecode = p_ecode;
    return SIGTRAN_SUCCESS;
#endif
}

/******************************************************************************
**  FUNCTION :    sigtran_mem_deinit
**
**  DESCRIPTION:  This function de initialises the Memory Manager.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_mem_deinit(
    sigtran_U32bit              task_id,
    sigtran_U32bit              num_common_pools,
    sigtran_pool_info_st        *p_comm_pool_info,
    sigtran_pool_data_st        *p_common_pool_data,
    sigtran_U32bit              num_task_pools,
    sigtran_pool_info_st        *p_list_pool_info,
    sigtran_pool_id_t           *p_pool_id_list,
    sigtran_boolean_t           b_deinit_mesg_pool,
    sigtran_error_t             *p_ecode )
{
#ifdef SIG_COMMON_WITH_CSPL
    sigtran_U32bit   count = 0;
    sigtran_error_t  p_temp_ecode;

    task_id          = task_id;
    num_common_pools = num_common_pools;
    p_comm_pool_info = p_comm_pool_info;
    p_list_pool_info = p_list_pool_info;

    if( p_ecode == NULL )
    {
        p_ecode = &p_temp_ecode;
    }

    if( SIGTRAN_FALSE == b_deinit_mesg_pool )
    {
        if( SIGTRAN_NULL != p_common_pool_data )
        {
            if( SIGTRAN_NULL != *p_common_pool_data )
            {
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
                qvPoolSetDelete( *p_common_pool_data, p_ecode );
#else
                /* CSR#1-4825810 Changes done for warning removal activity
                 * for compilation with SUN cc */
                qvPoolSetDelete( *p_common_pool_data, ( sigtran_S32bit * )p_ecode );
#endif

                if( QVERROR_NONE != *p_ecode )
                {
                    return SIGTRAN_FAILURE;
                }
            }
        }

        for( count  = 0; count < num_task_pools; count++ )
        {
            /* Delete  Task specific pool */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            qvPoolSetDelete( p_pool_id_list[count], p_ecode );
#else
            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            qvPoolSetDelete( p_pool_id_list[count], ( sigtran_S32bit * )p_ecode );
#endif

            if( QVERROR_NONE != *p_ecode )
            {
                return SIGTRAN_FAILURE;
            }
        }
    }

    else
    {
        if( SIGTRAN_NULL != p_common_pool_data )
        {
            if( SIGTRAN_NULL != *p_common_pool_data )
            {
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
                qvMsgPoolSetDelete( *p_common_pool_data, p_ecode );
#else
                /* CSR#1-4825810 Changes done for warning removal activity
                 * for compilation with SUN cc */
                qvMsgPoolSetDelete( *p_common_pool_data, ( sigtran_S32bit * )p_ecode );
#endif
            }

            if( QVERROR_NONE != *p_ecode )
            {
                return SIGTRAN_FAILURE;
            }
        }

        for( count  = 0; count < num_task_pools; count++ )
        {
            /* Delete  Task specific pool */
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
            qvMsgPoolSetDelete( p_pool_id_list[count], p_ecode );
#else
            /* CSR#1-4825810 Changes done for warning removal activity
             * for compilation with SUN cc */
            qvMsgPoolSetDelete( p_pool_id_list[count], ( sigtran_S32bit * )p_ecode );
#endif

            if( QVERROR_NONE != *p_ecode )
            {
                return SIGTRAN_FAILURE;
            }
        }
    }

    return SIGTRAN_SUCCESS;
#else
    task_id             = task_id;
    num_common_pools    = num_common_pools;
    p_comm_pool_info    = p_comm_pool_info;
    p_common_pool_data  = p_common_pool_data;
    num_task_pools      = num_task_pools;
    p_list_pool_info    = p_list_pool_info;
    p_pool_id_list      = p_pool_id_list;
    b_deinit_mesg_pool  = b_deinit_mesg_pool;
    *p_ecode = ESIG_NO_ERROR;
    return SIGTRAN_SUCCESS;
#endif
}


/******************************************************************************
**  FUNCTION :    sigtran_get_buffer
**
**  DESCRIPTION:  This function get the buffer either from Task specific GL
**                Common Pool or from GL Task Pool.
**                Note: Parameter "p_pool_info" is a void pointer that needs
**                to be either pointer to sigtran_pool_data_st or
**                sigtran_pool_id_t
**
**  RETURN VALUE: sigtran_buffer_t
******************************************************************************/
sigtran_buffer_t sigtran_get_buffer(
    sigtran_U32bit              task_id,
    sigtran_boolean_t           is_cmn_pool,
    sigtran_void_t              *p_pool_info,
    sigtran_S32bit              buf_size )
{
    sigtran_buffer_t        p_buf;

#ifdef SIG_COMMON_WITH_CSPL
    sigtran_U32bit          *p_temp;
    sigtran_S32bit          act_buf_size;
    sigtran_error_t           e_code;
    sigtran_error_t           *p_ecode = &e_code;
    task_id = task_id;
    is_cmn_pool = is_cmn_pool;

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    /*SPR 20409 fix start*/
    p_buf = qvPoolSetAlloc( *( QMPOOLSET * )p_pool_info, buf_size + 4,
                            &act_buf_size,
                            p_ecode );
    /*SPR 20409 fix end*/
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_buf = qvPoolSetAlloc( *( QMPOOLSET * )p_pool_info, buf_size,
                            ( sigtran_U32bit * )&act_buf_size,
                            ( sigtran_S32bit * )p_ecode );
#endif

    if( QVERROR_NONE != *p_ecode )
    {
        return SIGTRAN_NULL;
    }

    if( p_buf != NULL )
    {
        /* First 4 Bytes for Reference Count. */
        p_temp = ( sigtran_U32bit * )p_buf;
        *p_temp = 1;
        /*        p_buf = (sigtran_buffer_t)(p_temp + 1);
        */
        p_buf = ( sigtran_buffer_t )( p_temp ) + sizeof( sigtran_U32bit * );
    }

#else
    task_id = task_id;
    is_cmn_pool = is_cmn_pool;
    p_pool_info     = p_pool_info;
    p_buf = sigtran_malloc( ( sigtran_U32bit )buf_size );
#endif

    return p_buf;
}

/******************************************************************************
**  FUNCTION :    sigtran_free_buffer
**
**  DESCRIPTION:  This function Frees the buffer passed either from GL Task
**                specific common pool or from GL Task Pool. This is only
**                done in case reference number is Zero.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_free_buffer(
    sigtran_U32bit              task_id,
    sigtran_boolean_t           is_cmn_pool,
    sigtran_pool_id_t           pool_id,
    sigtran_buffer_t           *pp_buffer )
{
#ifdef SIG_COMMON_WITH_CSPL
    sigtran_U32bit      *p_head;
#endif

    task_id     = task_id;
    is_cmn_pool = is_cmn_pool;
    pool_id     = pool_id;

#ifdef SIG_COMMON_WITH_CSPL
    p_head = ( sigtran_U32bit * )( *pp_buffer  - sizeof( sigtran_U32bit * ) );
    ( *p_head )--;

    if( *p_head > 0 )
    {
        /* return success without freeing because this buffer is with some ULP
         * which will free it
         */
        return SIGTRAN_SUCCESS;
    }

    *pp_buffer = ( sigtran_pvoid_t )p_head;
    qvPoolSetFree( ( sigtran_pvoid_t )*pp_buffer );
#else
    sigtran_free( *pp_buffer );
#endif
    return SIGTRAN_SUCCESS;
}

/******************************************************************************
**  FUNCTION :    sigtran_incr_mem_ref_count
**
**  DESCRIPTION:  This function increases the reference count by one.
**
**  RETURN VALUE: Void
************************************************************************/
sigtran_void_t  sigtran_incr_mem_ref_count(
    sigtran_buffer_t      p_buffer )
{
    sigtran_U32bit      *p_head;

    p_head = ( sigtran_U32bit * )( p_buffer - sizeof( sigtran_U32bit * ) );
    ( *p_head )++;

}


/******************************************************************************
**  FUNCTION :    sigtran_mesg_get_buffer
**
**  DESCRIPTION:  This function is used for allocating the memeory for
**                messaging for inter-module communication and for sending
**                the message on external interface.
**
**  RETURN VALUE: sigtran_buffer_t
************************************************************************/
sigtran_buffer_t  sigtran_mesg_get_buffer(
    sigtran_U32bit              task_id,
    sigtran_void_t              *p_pool_info,
    sigtran_S32bit              buf_size )
{
    sigtran_S32bit            act_buf_size;
    sigtran_buffer_t          p_buf;
#ifdef SIG_COMMON_WITH_CSPL
    sigtran_error_t           e_code;
    sigtran_error_t           *p_ecode = &e_code;
    task_id = task_id;
#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_buf = qvMsgPoolSetAlloc( *( QMPOOLSET * )p_pool_info, buf_size, &act_buf_size,
                               p_ecode );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_buf = qvMsgPoolSetAlloc( *( QMPOOLSET * )p_pool_info, buf_size, ( sigtran_U32bit * )&act_buf_size,
                               ( sigtran_S32bit * )p_ecode );
#endif

    if( QVERROR_NONE != *p_ecode )
    {
        return SIGTRAN_NULL;
    }

#else
    task_id = task_id;
    act_buf_size = 0;
    p_pool_info  = p_pool_info;
    p_buf = sigtran_malloc( buf_size );
#endif
    /*  SPR 21127 changes start */
    COMMON_UNUSED_VARIABLE( act_buf_size );
    /*  SPR 21127 changes end */

    return p_buf;
}

/******************************************************************************
**  FUNCTION :    sigtran_mesg_free_buffer
**
**  DESCRIPTION:  This function is used for frreing the message buffer.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
************************************************************************/
sigtran_return_t sigtran_mesg_free_buffer(
    sigtran_U32bit              task_id,
    sigtran_buffer_t           *pp_buffer )
{
    task_id = task_id;
#ifdef SIG_COMMON_WITH_CSPL
    qvMsgPoolSetFree( ( sigtran_pvoid_t )*pp_buffer );
#else
    sigtran_free( *pp_buffer );
#endif
    return SIGTRAN_SUCCESS;
}

#ifdef SIG_COMMON_WITH_CSPL
/******************************************************************************
**  Function Name       : sigtran_inst_ext_buff_in_mesg_buffer
**
**    Description       : This function is used for inserting the external
**                        buffer to the message buffer. This is used for zero
**                        copying.The entire io-vector can be clubbed into the
**                        single message and this can further be extracted
**                        using functions
**                        qvMsgSegCount and qvMsgSegNext.
**
**  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
**
******************************************************************************/
sigtran_return_t sigtran_inst_ext_buff_in_mesg_buffer(
    sigtran_U32bit              task_id,
    sigtran_buffer_t            *p_mesg_buffer,
    sigtran_S32bit              insert_position,
    sigtran_buffer_t            *p_ext_buffer,
    sig_free_func_ptr_t         free_fn,
    sigtran_pvoid_t             p_free_fn_args,
    sigtran_S32bit              buf_size,
    sigtran_error_t           *p_ecode )
{
    sigtran_buffer_t    *p_data;
    sigtran_error_t     p_temp_code;
    task_id = task_id;

    if( p_ecode == NULL )
    {
        p_ecode = &p_temp_code;
    }

#ifndef SIGTRAN_SOLARIS_SUN_CC_SUPPORT
    p_data = ( sigtran_buffer_t * )
             qvMsgInsertExternalEx( *p_mesg_buffer, insert_position,
                                    ( unsigned char * )*p_ext_buffer, buf_size, free_fn, p_free_fn_args,
                                    p_ecode );
#else
    /* CSR#1-4825810 Changes done for warning removal activity
     * for compilation with SUN cc */
    p_data = ( sigtran_buffer_t * )
             qvMsgInsertExternalEx( *p_mesg_buffer, insert_position,
                                    ( unsigned char * )*p_ext_buffer, buf_size, free_fn, p_free_fn_args,
                                    ( sigtran_S32bit * )p_ecode );
#endif

    /*  SPR 21127 changes start */
    COMMON_UNUSED_VARIABLE( p_data );
    /*  SPR 21127 changes end */

    if( QVERROR_NONE != *p_ecode )
    {
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

/******************************************************************************
 **  Function Name       : sigtran_extract_buffer_to_iovec
 **
 **    Description       : This function extracts the message into the iovector
 **                        form. In the sigtran_inst_ext_buff_in_mesg_buffer,
 **                        segments were added to the single buffer and now
 **                        using this function segments are extracted from the
 **                        buffer.
 **
 **  RETURN VALUE: SIGTRAN_SUCCESS/SIGTRAN_FAILURE
 **
 ******************************************************************************/
sigtran_return_t sigtran_extract_buffer_to_iovec(
    sigtran_U32bit              task_id,
    sigtran_buffer_t            *p_buffer,
    sigtran_iovec_st            *p_iovec,
    sigtran_U32bit              *p_num_iovec )
{
    void            *last;
    QSEGMENT        seg;
    sigtran_U32bit  i;
    sigtran_U32bit  nseg;

    task_id = task_id;

    nseg = qvMsgSegCount( p_buffer );

    for( i = 0, last = 0; i < nseg; i++ )
    {
        last = qvMsgSegNext( *p_buffer, last, &seg );

        p_iovec[i].iov_base = ( char * )seg.base;
        p_iovec[i].iov_len = seg.size;

    }

    *p_num_iovec = nseg + 1;

    return( SIGTRAN_SUCCESS );
}
#endif /* SIG_COMMON_WITH_CSPL */

