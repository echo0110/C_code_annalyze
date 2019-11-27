/***********************************************************************
**  FUNCTION:
**      Function prototypes of OS related functions in the port.
************************************************************************
**
**  FILE NAME:
**      egtpu_os_proto.h
**
**  DATE           NAME               REFERENCE        REASON
**  ----           ----               ---------        ------
**  Oct, 2009     Vipul Aggarwal                       Original
**
**   Copyright (C) 2009 Aricent Inc . All Rights Reserved
***********************************************************************/
#ifndef _EGTPU_OS_PROTO_
#define _EGTPU_OS_PROTO_

/* OS Porting function declerations */
extern egtpu_void_t   *egtpu_memget(egtpu_global_t *, U32bit, U8bit);

extern egtpu_void_t   egtpu_memfree(U8bit, egtpu_void_t *);
extern egtpu_void_t   egtpu_bzero(egtpu_void_t *, U32bit);

extern egtpu_void_t   egtpu_memcpy(egtpu_void_t *,
                                  const egtpu_void_t *, U32bit);

extern S32bit        egtpu_memcmp(const egtpu_void_t *,
                                  const egtpu_void_t *, U32bit);

extern S32bit        egtpu_strcmp(const S8bit *, const S8bit *);

extern S8bit*        egtpu_strcpy(S8bit *, const S8bit *);

extern egtpu_return_t egtpu_start_timer(egtpu_global_t *,egtpu_entity_id_t, U32bit,
                                       egtpu_boolean_t, U8bit *,
                                       U16bit, U32bit *);

/* + SPR 21577 */
extern egtpu_void_t   egtpu_stop_timer(U32bit *);
/* - SPR 21577 */

extern egtpu_return_t egtpu_send_from_egtpu(egtpu_global_t *, U8bit *, U16bit,
                                          egtpu_boolean_t);

extern egtpu_void_t *egtpu_pool_alloc(egtpu_pool_t);

extern U16bit egtpu_pool_sequence(egtpu_void_t *);

extern S32bit egtpu_pool_extend(egtpu_pool_t, U32bit);

extern egtpu_void_t egtpu_pool_free(egtpu_void_t *);

extern U32bit egtpu_pool_index(egtpu_pool_t, egtpu_void_t *);

extern egtpu_void_t *egtpu_pool_locate(egtpu_pool_t, U32bit);

extern egtpu_void_t egtpu_sysfree(egtpu_void_t *);

extern egtpu_void_t egtpu_msgfree(egtpu_void_t *);

extern egtpu_void_t *egtpu_malloc(U32bit);

extern egtpu_void_t *egtpu_msg_alloc_from_pool(egtpu_pool_t);

extern egtpu_pool_t egtpu_pool_create(U32bit, U32bit);

extern egtpu_void_t egtpu_pool_delete(egtpu_pool_t);

extern egtpu_pool_t egtpu_msg_pool_create(U32bit, U32bit);
extern egtpu_void_t *egtpu_pool_alloc_specific(egtpu_pool_t , U32bit ,U16bit );
/*reordering*/
extern S32bit egtpu_timer_running(U8bit *);
extern egtpu_return_t egtpu_start_reorder_timer(egtpu_global_t * ,egtpu_rab_ctxt_t *);
extern egtpu_void_t egtpu_stop_reorder_timer(U8bit *);
/*hash related functions*/

extern egtpu_return_t egtpu_find_in_hash_table(YHASH *, U16bit,YHNODE **);

extern egtpu_void_t egtpu_insert_in_hash_table(egtpu_hash_tbl *,YHASH *);

extern egtpu_void_t egtpu_delete_from_hash_table(YHASH *, YHNODE *);

extern YHNODE *egtpu_first_pdu_in_hash_table(YHASH *);

extern YHNODE *egtpu_next_pdu_in_hash_table(YHASH *, YHNODE *N);  

extern U32bit egtpu_count_pdu_in_hash_table(YHASH *);

extern const egtpu_void_t * egtpu_search_list( const YHNODE *);

extern S32bit egtpu_Compare(const egtpu_void_t *, const egtpu_void_t *);

extern unsigned long egtpu_hash(const egtpu_void_t *);

extern void egtpu_init(void);

extern void lteWarning( const Char8 *format, ... );

extern void ltePanic( const Char8 *format, ... );

extern void *memSet( void *s, SInt32 c, size_t n );

extern void *memCpy( void *dest, const void *src, size_t n );

extern S32bit memCmp( const void *a, const void *b, size_t n );

extern void * msgPoolCreate(UInt32 blockSize, UInt32 nBuf);

extern void msgFree(void *buffer);

extern void *msgAlloc(void* pool, U32bit headroom, U32bit tailroom, U32bit blockSize);

extern void qvPoolFree (void *);

/*extern S32bit lteLog( UInt32 level, ModuleLogDetail *det, const Char8 *format, ...);*/

#endif
