
/**************************************************************************
*
*  ARICENT -
*
*  Copyright (c) 2010 Aricent.
*
***************************************************************************/


#include "stacklayer.h"
#include "x2ap_db.h"
#include "x2ap_search_tree.h"
#include "x2ap_types.h"
#include "x2ap_tracing.h"
#include "x2ap_utils.h"
#include "rrc_x2apRrm_intf.h"
#include "rrc_common_utils.h"

/* These are used for compare function of a search tree*/
#define FIRST_IS_LESS_THAN_SECOND         -1
#define FIRST_IS_GREATER_THAN_SECOND       1
#define FIRST_IS_EQUAL_TO_SECOND           0
#define ERROR_IN_COMPARE                   0


U32 x2ap_rsu_get_meas_id (x2ap_gb_context_t *p_x2ap_gb_ctx);

void
x2ap_pack_U32
(
 void    *p_dest,
 void    *p_src
 );

void
x2ap_unpack_U32
(
 void    *p_dest,
 void    *p_src
 );
void rsu_st_init (
      /* Pointer to tree being initialised */
      x2ap_search_tree_t *p_tree,

      /* Pointer to compare function :
      it returns 0 if A==B, -1 if A<B and 1 if A>B */
      S32 (*compare)(const void *, const void *),

      /* Pointer to function returning Key */
      const void *(*keyof)(const x2ap_search_tree_node_t *));


void rsu_st_insert_node (
     x2ap_search_tree_t *p_tree,
     x2ap_search_tree_node_t *entry);

x2ap_return_et 
x2ap_send_x2ap_rrm_rsu_enb_start_resp(U32            transaction_id,
                                      x2ap_return_et response,
                                      U32            meas_id);

void 
x2ap_compose_asn_plmn_identity(OSOCTET            *p_x2ap_plmn_identity,
                                plmn_identity_t    *p_plmn_identity);


const void *
key_of_rsu_enb_meas_id_search_tree(
        const YTNODE * p_node);

x2ap_search_tree_node_t * rsu_st_find_tree_node (
     const x2ap_search_tree_t *p_tree,
     const void *item);

void x2ap_parse_asn_plmn_identity(
     plmn_identity_t    *p_plmn_identity,
     const OSOCTET      *p_s1ap_plmn_identity);


x2ap_return_et x2ap_validate_peer_cell_info_and_get_peer_enb_context(
        x2ap_gb_context_t* p_x2ap_gb_ctx,
        x2ap_rrm_rsu_rrm_start_req_t *p_x2ap_rsu_start_req,
        U8 *peer_enodeb_id);

S32
compare_rsu_enb_meas_id_search_tree(
        const void * p_elem1,
        const void * p_elem2);

