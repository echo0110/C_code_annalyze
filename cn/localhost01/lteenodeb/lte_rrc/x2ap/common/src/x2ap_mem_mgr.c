/*******************************************************************************
*
*  FILE NAME   : x2ap_mem_mang.c
*
*  DESCRIPTION : This file contains definition of functions used to allocate 
*                memory, deallocate memory and other memory related operations.
*
*      DATE             NAME               REFERENCE                REASON
*  24-JUNE-2010         Vaibhav Singh      36.423_880(3GPP Spec)    Initial
*
*  Copyright (c) 2009, Aricent Inc. All Rights Reserved
*
*******************************************************************************/

#include "x2ap_mem_mgr.h"
#include "x2ap_utils.h"

/******************************************************************************
*   FUNCTION NAME: x2ap_mem_get
 * INPUTS        : size - Size of buffer which will be allocated
 * OUTPUTS       : None
 * RETURNS       : Pointer to memory buffer or NULL in case of failure
 * DESCRIPTION   : This function returns memory buffer from memory pool.
*       Function is used for general memory management purposes.
******************************************************************************/
void* x2ap_mem_get(x2ap_size_t size)/* Size of buffer which will be allocated */
{
    void *p_buf = X2AP_NULL;
    int err = 0;
/*SPR_18125_START*/
#ifdef CSPL_LEAK_DEBUG_LITE
    p_buf = qvAllocEX(size, PNULL,&err,__func__,__LINE__);
#else
    p_buf = qvAllocEx(size, X2AP_NULL,&err);
#endif
/*SPR_18125_END*/

    if(QVERROR_MEM_ALLOC_ILLEGAL_SIZE == err)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_mem_get: memory allocation failed invalid buff size %u error %d",size,err);
        
        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_INVALID_PARAM_ERR_CAUSE_ID);
    }

    if((QVERROR_SYS_ALLOC_FAIL == err) || (X2AP_NULL == p_buf))
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_mem_get: memory allocation failed no buff available error = %d",err);
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_NO_BUFF_AVAILABLE_ERR_CAUSE_ID);
    }

    return p_buf;
}

/******************************************************************************
*   FUNCTION NAME: x2ap_mem_free
 * INPUTS        : p_buffer - Pointer to buffer which will be freed
 * OUTPUTS       : p_buffer
 * RETURNS       : None
 * DESCRIPTION   : This function frees memory buffer allocated in pool.
*       Function is used for general memory management purposes.
******************************************************************************/
void x2ap_mem_free(void *p_buffer) /* Pointer to buffer which will be freed */
{
    int err = 0;
    if (X2AP_NULL != p_buffer)
    {
        qvFreeEx(p_buffer,&err);
        if(QVERROR_MEM_UNALLOCATED_BUFFER == err)
        {
            X2AP_TRACE(X2AP_FATAL, "x2ap_mem_free: Memory already freed error %d",err);
            ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_ALREADY_FREE_ERR_CAUSE_ID);
        }
    }
    else
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_mem_free: null pointer passed");
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_NULL_POINTER_PASSED_ERR_CAUSE_ID);
    }
}

/******************************************************************************
*   FUNCTION NAME: x2ap_msg_mem_get
 * INPUTS        : size - Size of buffer which will be allocated
 * OUTPUTS       : None
 * RETURNS       : Pointer to memory buffer or NULL in case of failure
 * DESCRIPTION   : This function returns memory buffer from memory pool.
*       Function is used for messages management purposes.
******************************************************************************/
void* x2ap_msg_mem_get(x2ap_size_t size)/*Size of buffer which will be allocated*/
{
    void *p_buf = X2AP_NULL;
    int err = 0;

    p_buf = qvMsgAllocEx(PNULL, 0, 0, size,&err);

    if(QVERROR_MEM_ALLOC_ILLEGAL_SIZE == err)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_msg_mem_get: memory allocation failed invalid size %d error %d",size,err);

        /* alarm framework integration */
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_INVALID_PARAM_ERR_CAUSE_ID);
    }

    if ((QVERROR_SYS_ALLOC_FAIL == err) || (X2AP_NULL == p_buf))
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_msg_mem_get: memory allocation failed no buff available error %d",err);
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_NO_BUFF_AVAILABLE_ERR_CAUSE_ID);
    }
    
    /* Coverity Fix 64012 start */
    if (PNULL != p_buf )
    {
         memset_wrapper(p_buf,0,size);
    }
    /* Coverity Fix 64012 stop */
    return (p_buf);
}

/******************************************************************************
*   FUNCTION NAME: x2ap_msg_mem_free
 * INPUTS        : p_buffer - Pointer to buffer which will be freed
 * OUTPUTS       : p_buffer
 * RETURNS       : None
 * DESCRIPTION   : This function frees memory buffer allocated in pool.
*       Function is used for messages management purposes.
******************************************************************************/
void x2ap_msg_mem_free(void *p_buffer)/*Pointer to buffer which will be freed */
{
    int err = 0;
    if (X2AP_NULL != p_buffer)
    {
        qvMsgFreeEx(p_buffer,&err);
        if(QVERROR_MEM_UNALLOCATED_BUFFER == err)
        {
            X2AP_TRACE(X2AP_FATAL, "x2ap_msg_mem_free: Memory already freed error %d",err);
            ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_ALREADY_FREE_ERR_CAUSE_ID);
        }
    }
    else
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_msg_mem_free: null pointer passed");
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_NULL_POINTER_PASSED_ERR_CAUSE_ID);
    }
}


/******************************************************************************
*   FUNCTION NAME: x2ap_sys_mem_get
 * INPUTS        : size - Size of buffer which will be allocated
 * OUTPUTS       : None
 * RETURNS       : Pointer to memory buffer or NULL in case of failure
 * DESCRIPTION   : This function returns memory buffer using direct OS call 
 *                 allocation routine.Function is used for managing static buffers
 *                 like x2ap_gb_ctx.
******************************************************************************/
void* x2ap_sys_mem_get(x2ap_size_t size)/*Size of buffer which will be allocated*/
{
    void *p_buf = X2AP_NULL;

    p_buf = (void *)qvSysMalloc(size);

    if (X2AP_NULL == p_buf)
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_sys_mem_get: memory allocation failed");
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_ALLOC_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_ALLOC_NO_BUFF_AVAILABLE_ERR_CAUSE_ID);
    }

    return(p_buf);
}

/******************************************************************************
*   FUNCTION NAME: x2ap_sys_mem_free
 * INPUTS        : p_buffer -  Pointer to buffer which will be freed
 * OUTPUTS       : p_buffer
 * RETURNS       : None
 * DESCRIPTION   : This function frees memory buffer using direct OS call free
 *                 routine. Function is used for managing static buffers like
 *                 x2ap_gb_ctx.
******************************************************************************/
void x2ap_sys_mem_free(void *p_buffer) /* Pointer to buffer which will be freed */
{
    if (X2AP_NULL != p_buffer)
    {
        qvSysFree(p_buffer);
    }
    else
    {
        X2AP_TRACE(X2AP_FATAL, "x2ap_msg_mem_free: null pointer passed");
        ALARM_MSG_L3(RRC_MODULE_ID, MEM_FREE_FAILED_ALARM_ID,CRITICAL_ALARM,MEM_FREE_NULL_POINTER_PASSED_ERR_CAUSE_ID);
    }
}


