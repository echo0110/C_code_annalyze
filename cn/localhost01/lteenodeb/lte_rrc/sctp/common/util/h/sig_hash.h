/****************************************************************************
** FUNCTION      :
**                File for hash table related functions.
******************************************************************************
** FILE          :
**                sig_hash.h
**
** DESCRIPTION   :
**                File contains the functions for Hash table implementation
**                in Sigtran Stack.
**
*****************************************************************************/
#ifndef __FILE_SIG_HASH_H_SEEN__    /* If this file has not been processed, */
#define __FILE_SIG_HASH_H_SEEN__    /* define it as seen--ref '#endif' below.*/
#ifdef __cplusplus
extern "C"
{
#endif
    sigtran_return_t   sigtran_htm_init( sigtran_error_t      *p_ecode );


    sigtran_return_t   sigtran_htm_deinit( sigtran_error_t      *p_ecode );


    sigtran_return_t sigtran_create_hash_table( sigtran_S8bit         task_id,
                                                sigtran_U16bit        *hash_table_id,
                                                sigtran_U32bit        no_of_keys,
                                                sigtran_U32bit        no_of_bucket,
                                                sigtran_error_t       *p_ecode );

    sigtran_return_t  sigtran_delete_hash_table( sigtran_S8bit   task_id,
                                                 sigtran_U16bit  hash_table_id,
                                                 sigtran_error_t *p_ecode );


    sigtran_return_t  sigtran_insert_node( sigtran_S8bit   task_id,
                                           sigtran_U16bit  hash_table_id,
                                           sigtran_pvoid_t key,/*based on key_type*/
                                           sigtran_pvoid_t p_elem,
                                           sigtran_error_t *p_ecode );

    sigtran_return_t  sigtran_get_node( sigtran_S8bit    task_id,
                                        sigtran_U16bit    hash_table_id,
                                        sigtran_pvoid_t  key,
                                        sigtran_pvoid_t  *p_elem,
                                        sigtran_error_t  *p_ecode );

    sigtran_return_t  sigtran_delete_node( sigtran_S8bit   task_id,
                                           sigtran_U16bit  hash_table_id,
                                           sigtran_pvoid_t key,
                                           sigtran_error_t *p_ecode );



#ifdef __cplusplus
}
#endif

#endif
