/*****************************************************************************
 * File Name   : oam_config_tcb.h
 *
 * Description : This file contains includes required for OAM Hash
 *
 * Revision History:
 *
 * Date         Author             Reference            Comments
 * -----        ------             ---------            --------
 * May,2012     Rajesh Kumar    OAM Design Document   Initial Version
 * July,2012    Rajesh Kumar
 *
 * Copyright (c) 2012, Aricent Inc. All Rights Reserved
 ****************************************************************************/

#ifndef _OAM_HASH_
#define _OAM_HASH_

/****************************************************************************
 ** INCLUDES
 ****************************************************************************/
/*! ylib.h is header file of cspl of hash implementation */
#include    <ylib.h>
#include    <oam_types.h>
#include    <oam_defines.h>
#include    <oam_proto.h>

/****************************************************************************
 ** MACROS / TYPEDEFS
 ****************************************************************************/
/*! \typedef OAM_HASH from YHASH */
typedef YHASH OAM_HASH;

/*! \typedef OAM_HASH_NODE from YHNODE */
typedef YHNODE OAM_HASH_NODE;


#define	  OAM_PNULL           ((void *)0)
#define   OAM_HASH_SIZE               100

#define   TCB_TRUE                      0
#define   TCB_FALSE                    -1

#define  TCB_BITMASK_SON             0x01
#define  TCB_BITMASK_RRM             0x02
#define  TCB_BITMASK_MAC             0x04
#define  TCB_BITMASK_RLC             0x08
#define  TCB_BITMASK_PDCP            0x10
#define  TCB_BITMASK_EGTPU           0x20
#define  TCB_BITMASK_RRC             0x40
#define  TCB_BITMASK_S1AP            0x80
#define  TCB_BITMASK_X2AP           0x100
#define  TCB_BITMASK_OAM            0x200
#define  TCB_BITMASK_L2ALL          0x400
#define  TCB_BITMASK_L3ALL          0x800
#define  TCB_BITMASK_ALL           0x1000
#define  TCB_BITMASK_FM            0x2000
#define  TCB_BITMASK_PM            0x4000
#define  TCB_BITMASK_SM            0x8000
#ifdef OAM_SON_ENABLE
#define  TCB_BITMASK_SYS            TCB_BITMASK_SON|TCB_BITMASK_RRM|TCB_BITMASK_RRC
#else
#define  TCB_BITMASK_SYS            TCB_BITMASK_RRM|TCB_BITMASK_RRC
#endif
#ifdef OAM_RADIO_HEAD_SUPPORT
    #define  TCB_BITMASK_RRH       0x10000
#endif

/* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
#define  TCB_BITMASK_M2AP           0x20000
#endif
/* EMBMS Changes End */

/* CSR No 00055803 Start */
    /*SPR_10579 Start*/ 
#define  TCB_BITMASK_RRC_MAC_PDCP_RRM  (TCB_BITMASK_RRC) | (TCB_BITMASK_MAC) | (TCB_BITMASK_PDCP) | (TCB_BITMASK_RRM) 
    /*SPR_10579 End*/ 
/* CSR No 00055803 End */

/****************************************************************************
 ** ENUMERATIONS / STRUCTURES 
 ****************************************************************************/
typedef enum {
    OAM_TRANS_SON = 0,
    OAM_TRANS_RRM,
    OAM_TRANS_MAC,
    OAM_TRANS_RLC,
    OAM_TRANS_PDCP,
    OAM_TRANS_EGTPU,
    OAM_TRANS_RRC,
    OAM_TRANS_S1AP,
    OAM_TRANS_X2AP,
    OAM_TRANS_L2ALL,
    OAM_TRANS_L3ALL,
#ifdef OAM_RADIO_HEAD_SUPPORT   
    OAM_TRANS_RRH,
#endif
    OAM_TRANS_ALL,
    OAM_TRANS_OAM,
    OAM_TRANS_FM,
    OAM_TRANS_PM,
    OAM_TRANS_SM,
    OAM_TRANS_SYSTEM_LOG,
/* CSR No 00055803 Start */
    /*SPR_10579 Start*/ 
    OAM_TRANS_RRC_MAC_PDCP_RRM,
    /*SPR_10579 End*/ 
/* CSR No 00055803 End */
/* LWA changes start*/
/* LWA changes end*/
    /* EMBMS Changes Start */
#ifdef LTE_EMBMS_SUPPORTED
    OAM_TRANS_M2AP
#endif
        /* EMBMS Changes End */

} oam_trans_type_et;


typedef struct {
    UInt32    transaction_id;
    QTIMER    timer_id;
    UInt32    bitmask;
    void *oam_tcb_context;
    /* SPR_20467_FIX_START */
    /* used to store whether the sent msg is old/new */
    UInt8     old_msg_sent_flag;
    void     *oam_procedure_specific_context;
    /* SPR_20467_FIX_END */
} oam_struct_t;

typedef struct{
    OAM_HASH_NODE     anchor;
    oam_struct_t      oam_struct;
} oam_node_t;


/****************************************************************************
 ** FUNCTION PROTOTYPES
 ****************************************************************************/

/*! \fn oam_return_et oam_hash_init(ULong32 (*hash)(const void*),
 *     				              SInt32 (*compare)(const void *,
 *						                           const void *),
 *     				 const void *(*keyof)(const OAM_HASH_NODE *))
 *  \brief Intialises the HASH table.
 *  \param hash             hash function
 *  \param compare          compare function
 *  \param keyof            keyoff function
 */
oam_return_et
oam_hash_init
(
    ULong32     (*hash)(const void*),
    SInt32      (*compare)(const void *,
                           const void *),
    const void *(*keyof)(const OAM_HASH_NODE *)
);

/*! \fn oam_return_et oam_hash_insert(OAM_HASH_NODE *p_node)
 *  \brief Inserts in the hash table.
 *  \param p_node           Pointer to the node to be inserted
 */
oam_return_et
oam_hash_insert
(
    OAM_HASH_NODE *p_node
);

/*! \fn oam_return_et oam_hash_delete(OAM_HASH_NODE *p_node)
 *  \brief Deletes from the hash table
 *  \param p_node           Pointer to the node to be deleted
 */
oam_return_et
oam_hash_delete
(
    OAM_HASH_NODE *p_node
);

/*! \fn UInt32  oam_hash_count(void)
 *  \brief Counts the number of nodes in the hash table
 *  \param void
 */
UInt32
oam_hash_count
(
    void
);

/*! \fn OAM_HASH_NODE *oam_hash_find(const OAM_HASH *p_hash, const void *p_item)
 *  \brief Finds an item in the hash table
 *  \param p_item           Pointer to the item that will be compared to
 *                          identify node
 */
OAM_HASH_NODE *
oam_hash_find
(
    const void *p_item
);

/*! \fn OAM_HASH_NODE *oam_hash_get_first(void)
 *  \brief Returns the first element in the hash table on a sequential search
 *  \param void
 */
OAM_HASH_NODE *
oam_hash_get_first
(
   void
);

/*! \fn OAM_HASH_NODE *oam_hash_get_next(OAM_HASH_NODE *p_node)
 *  \brief Returns the next element in the hash table
 *  \param p_node           Pointer to the current node
 */
OAM_HASH_NODE *
oam_hash_get_next
(
    OAM_HASH_NODE *p_node
);

/*! \fn ULong32 oam_hash(const void *key_p)
 *  \brief Returns key after apply hash function
 *  \param key_p 	    Pointer to key
 */
ULong32
oam_hash
(
    const void *key_p
);

/*! \fn oam_return_et oam_init_hash_table(void)
 *  \brief initialize the hash table
 */
oam_return_et
oam_init_hash_table
(
    void
);

/*! \fn SInt32 oam_compare(const void *hashEntry1,
 *  \					   const void *hashEntry2)
 *  \brief This function compares two keys
 *  \param hashEntry1           Pointer to hash key1
 *  \param hashEntry2           Pointer to hash key2
 */
SInt32
oam_compare
(
    const void *hashEntry1,
    const void *hashEntry2
);

/*! \fn const void *oam_keyOf(const OAM_HASH_NODE  *p_hash_node)
 *  \brief returns key of hash node
 *  \param p_hash_node          Pointer to hash node
 */
const void *
oam_keyOf
(
    const OAM_HASH_NODE  *p_hash_node
);

/*! \fn oam_return_et oam_tcb_insert(void *p_msg)
 *  \brief insert message in TCB
 */
oam_return_et
oam_tcb_insert
(
    UInt32            trans_id,
    QTIMER            timer_id,
    oam_trans_type_et trans_type,
    UInt16            msg_len,
    void              *p_msg
);

/*! \fn oam_return_et oam_tcb_delete(void *item)
 *  \brief delete item from TCB
 *  \param item          Pointer to item to be deleted
 */
oam_return_et
oam_tcb_delete
(
    void    *item
);

/*! \fn oam_struct_t *oam_tcb_get_detail(void *item)
 *  \brief returns details of particular key
 *  \param item          Pointer to item
 */
oam_struct_t *
oam_tcb_get_detail
(
    void    *item
);

/*! \fn oam_return_et oam_tcb_update_bitmask(UInt16 trans_id,
 *                                           oam_trans_type_et trans_type,
 *                                           UInt16 *bitmask;)
 *  \brief update bitmask
 */
oam_return_et
oam_tcb_update_bitmask
(
   UInt16            trans_id,
   oam_trans_type_et trans_type,
   UInt32 *          bitmask
);
#endif  /* _OAM_HASH_ */
