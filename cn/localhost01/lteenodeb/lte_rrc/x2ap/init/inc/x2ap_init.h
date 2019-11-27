/*******************************************************************************
*
*  FILE NAME   : x2ap_init.h
*
*  DESCRIPTION : This file contains the structure and function declaration
*                required to initailize x2ap module.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2AP_INIT_H_
#define _X2AP_INIT_H_

#include "commoninc.h"
#include "x2ap_utils.h"
#include "x2ap_db.h"
#include "x2ap_md.h"


#define X2AP_FIRST_IS_EQUAL_TO_SECOND 0
#define X2AP_FIRST_IS_LESS_THAN_SECOND 1
#define X2AP_FIRST_IS_GREATER_THAN_SECOND -1

#define X2AP_ERROR_IN_COMPARE 0

    
void * x2ap_init (void * p_x2ap_init_data);
const void * x2ap_key_of_enb_id_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 );

S32 x2ap_compare_enb_id_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 );

const void * x2ap_key_of_ecgi_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 );

S32 x2ap_compare_ecgi_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 );

const void * x2ap_key_of_sd_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 );

S32 x2ap_compare_sd_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 );

const void * x2ap_key_of_ip_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 );

/*spr8341 IPv6 fix start*/
const void * x2ap_key_of_ipv6_search_tree
(
 /* Pointer to Tree node */
 const YTNODE * p_node
 );
/*spr8341 IPv6 fix end*/

S32 x2ap_compare_ip_search_tree
(
 /* Pointer to first element being compared */
 const void * p_elem1 ,

 /* Pointer to second element being compared */
 const void * p_elem2
 );

 void x2ap_init_int 
 (
   /* Pointer to Global Context */
   x2ap_gb_context_t* p_x2ap_gl_ctx
 );

#ifdef ENDC_ENABLED
/* MENB CHNAGES - START */
const void * x2ap_key_of_gnb_id_search_tree
(
    const YTNODE * p_node
);

S32
x2ap_compare_gnb_id_search_tree
(
    const void * p_elem1,
    const void * p_elem2
);
#endif
#endif
