/*******************************************************************************
*
*  FILE NAME   : x2ap_mem_mang.h
*
*  DESCRIPTION : This file contains the structure and function declaration 
*                required for x2ap memory managment.
*
*  DATE 24-June-2010 NAME Ashutosh Gupta REFERENCE 36.423_880(3GPP Spec) REASON
*
*  Copyright 2007, Aricent Inc.
*
*******************************************************************************/
#ifndef _X2AP_MEM_MNG_H_
#define _X2AP_MEM_MNG_H_

#include "x2ap_types.h"

/* X2AP Memory Management Function Prototypes */

void* x2ap_mem_get ( x2ap_size_t size );
void x2ap_mem_free ( void *p_buffer );
void* x2ap_msg_mem_get ( x2ap_size_t size );
void x2ap_msg_mem_free ( void *p_buffer );
void* x2ap_sys_mem_get ( x2ap_size_t size );
void x2ap_sys_mem_free( void *p_buffer );
#endif
