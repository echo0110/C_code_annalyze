/****************************************************************************
** FUNCTION      :
**                File for hash table related functions.
******************************************************************************
** FILE          :
**                sig_hash.c
**
** DESCRIPTION   :
**                File contains the functions for Hash table implementation
**                in Sigtran Stack.
**
** DATE       NAME                REFERENCE           REASON
** 07NOV2002  Kameshwar Prasad    -----           Create Original
**
**  Copyright (C) 2006 Aricent Inc . All Rights Reserved
*****************************************************************************/
#include "sig_os.h"
#include "gl_htm_interfaces.h"
#include "gl_mm_interfaces.h"
#include "sig_hash.h"


static sigtran_boolean_t    sig_hash_init = SIGTRAN_FALSE;

gl_return_t sigtran_remove_hash_elem
( gw_U32bit        user_id,
  gl_hash_key_ut        *p_key,
  gw_U8bit         key_type,
  gl_pvoid_t       p_elem_buf_ptr,
  gl_pvoid_t       p_hash_arg,
  gl_error_t       *p_ecode );


/************************************************************************
** FUNCTION    :
**               sigtran_htm_init
**
** DESCRIPTION :
**              Function to initialise the Gen Lib hash table manager module
** Return Type : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
*************************************************************************/

sigtran_return_t   sigtran_htm_init( sigtran_error_t      *p_ecode )
{

    /* Initialise the Generic Memory manager */
    if( SIGTRAN_FALSE == sig_hash_init )
    {
        gl_htm_init();
        sig_hash_init = SIGTRAN_TRUE;
    }

    return SIGTRAN_SUCCESS;
}

/************************************************************************
** FUNCTION    :
**               sigtran_remove_hash_elem
**
** DESCRIPTION :
**              Function to delete an element stored in hash
**
** Return Type : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
*************************************************************************/
gl_return_t sigtran_remove_hash_elem
( gw_U32bit        user_id,
  gl_hash_key_ut        *p_key,
  gw_U8bit         key_type,
  gl_pvoid_t       p_elem_buf_ptr,
  gl_pvoid_t       p_hash_arg,
  gl_error_t       *p_ecode )
{
    return GL_SUCCESS;
}


/************************************************************************
** FUNCTION    :
**               sigtran_htm_deinit
**
** DESCRIPTION :
**              Function to deinitialise the Gen Lib hash table manager module
**
** Return Type : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
*************************************************************************/

sigtran_return_t   sigtran_htm_deinit( sigtran_error_t      *p_ecode )
{
    gl_error_t   ecode;
    gl_return_t  ret_val;

    ret_val = gl_htm_destroy_all_hash_tables( GL_TRUE , &ecode );

    if( ret_val == GL_SUCCESS )
    {
        sig_hash_init = SIGTRAN_FALSE;
    }

    sigtran_destroy_all_cmn_pools( );

    return ( sigtran_return_t )ret_val;
}


/************************************************************************
** FUNCTION    :
**              sigtran_create_hash_table
**
** DESCRIPTION :
**              create a hash table
**
** Return Type : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
*************************************************************************/

sigtran_return_t sigtran_create_hash_table( sigtran_S8bit         task_id,
                                            sigtran_U16bit       *hash_table_id,
                                            sigtran_U32bit        no_of_keys,
                                            sigtran_U32bit        no_of_bucket,
                                            sigtran_error_t       *p_ecode )
{
    gw_error_t   error_code;

    if( GL_FAILURE == gl_htm_create_hash_table( task_id,
                                                hash_table_id,
                                                no_of_keys,
                                                no_of_bucket,
                                                GL_HTM_NUMERIC_KEY_TYPE,
                                                GL_HTM_HASH_FUNC_FOR_NUMERIC_KEYS,
                                                GL_NULL,
                                                ( gl_htm_free_key_elem_fn_t )sigtran_remove_hash_elem,
                                                GL_NULL,
                                                GL_NULL,
                                                &error_code ) )
    {
        SIGTRAN_SET_ECODE( p_ecode , error_code );
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

/************************************************************************
** FUNCTION    :
**             sigtran_delete_hash_table
**
** DESCRIPTION :
**              delete a hash table
** Return Type : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
*************************************************************************/
sigtran_return_t  sigtran_delete_hash_table( sigtran_S8bit   task_id,
                                             sigtran_U16bit  hash_table_id,
                                             sigtran_error_t *p_ecode )
{
    gw_error_t   error_code;

    if( GL_FAILURE == gl_htm_delete_hash_table( task_id,
                                                hash_table_id,
                                                GL_NULL,
                                                &error_code ) )
    {
        SIGTRAN_SET_ECODE( p_ecode , error_code );
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

/************************************************************************
** FUNCTION    :
**              sigtran_insert_node
**
** DESCRIPTION :
**              Insert a key into the hash table
**
** Return Type : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
*************************************************************************/
sigtran_return_t  sigtran_insert_node( sigtran_S8bit   task_id,
                                       sigtran_U16bit  hash_table_id,
                                       sigtran_pvoid_t key,/*based on key_type*/
                                       sigtran_pvoid_t p_elem,
                                       sigtran_error_t *p_ecode )
{
    gw_error_t      error_code;
    gl_hash_key_ut  hash_key;
    gw_U32bit       element;

    hash_key.numeric_id = *( sigtran_U32bit * )key;

    if( GL_FAILURE == gl_htm_add_key_to_hash_table( task_id,
                                                    ( gl_hash_table_t )hash_table_id,
                                                    &hash_key,
                                                    p_elem,
                                                    ( gl_pvoid_t * )&element,
                                                    &error_code ) )
    {
        SIGTRAN_SET_ECODE( p_ecode , error_code );
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}


/************************************************************************
** FUNCTION    :
**              sigtran_get_node
**
** DESCRIPTION :
**              Get a key from the hash table
**
** Return Type : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
*************************************************************************/
sigtran_return_t  sigtran_get_node( sigtran_S8bit    task_id,
                                    sigtran_U16bit   hash_table_id,
                                    sigtran_pvoid_t  key,
                                    sigtran_pvoid_t  *p_elem,
                                    sigtran_error_t  *p_ecode )
{
    gw_error_t      error_code;
    gl_hash_key_ut  hash_key;

    hash_key.numeric_id = *( sigtran_U32bit * )key;

    if( GL_FAILURE == gl_htm_get_key_ref_from_hash_table( task_id,
                                                          ( gl_hash_table_t )hash_table_id,
                                                          &hash_key,
                                                          p_elem,
                                                          &error_code ) )
    {
        SIGTRAN_SET_ECODE( p_ecode , error_code );
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

/************************************************************************
** FUNCTION    :
**              sigtran_delete_node
**
** DESCRIPTION :
**              Remove a key from the hash table
**
** Return Type : SIGTRAN_SUCCESS/SIGTRAN_FAILURE
*************************************************************************/
sigtran_return_t  sigtran_delete_node( sigtran_S8bit   task_id,
                                       sigtran_U16bit  hash_table_id,
                                       sigtran_pvoid_t key,
                                       sigtran_error_t *p_ecode )
{
    gw_error_t      error_code;
    gl_hash_key_ut  hash_key;
    gl_pvoid_t      p_elem;
    gl_pvoid_t      p_key_buf;


    hash_key.numeric_id = *( sigtran_U32bit * )key;

    if( GL_FAILURE == gl_htm_remove_key_from_hash_table( task_id,
                                                         ( gl_hash_table_t )hash_table_id,
                                                         &hash_key,
                                                         &p_key_buf,
                                                         &p_elem,
                                                         &error_code ) )
    {
        SIGTRAN_SET_ECODE( p_ecode , error_code );
        return SIGTRAN_FAILURE;
    }

    return SIGTRAN_SUCCESS;
}

