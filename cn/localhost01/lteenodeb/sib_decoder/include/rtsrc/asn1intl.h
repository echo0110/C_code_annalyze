/*
 * Copyright (c) 1997-2017 Objective Systems, Inc.
 *
 * This software is furnished under a license and may be used and copied
 * only in accordance with the terms of such license and with the
 * inclusion of the above copyright notice. This software or any other
 * copies thereof may not be provided or otherwise made available to any
 * other person. No title to and ownership of the software is hereby
 * transferred.
 *
 * The information in this software is subject to change without notice
 * and should not be construed as a commitment by Objective Systems, Inc.
 *
 * PROPRIETARY NOTICE
 *
 * This software is an unpublished work subject to a confidentiality agreement
 * and is protected by copyright and trade secret law.  Unauthorized copying,
 * redistribution or other use of this work is prohibited.
 *
 * The above notice of copyright on this source code product does not indicate
 * any actual or intended publication of such source code.
 *
 *****************************************************************************/
/*
 * asn1intl.h - ASN.1 runtime internal definitions
 */
/**
 * @file asn1intl.h
 */
#ifndef _ASN1INTL_H_
#define _ASN1INTL_H_

/**
 * The ASN1C runtime version number.  This should match the ASN1C
 * compiler version.
 */
#define ASN1RT_VERSION  670

#ifndef _COMPACT

/* For ARM compilers. */
#ifndef __MODULE__
/**
 * Logs an ASN.1 error.
 *
 * @param   ctxt     An OSCTXT structure (not a pointer).
 * @param   stat     The error status to be logged.
 */
#define LOG_ASN1ERR(ctxt,stat) \
      rtxErrSetData(&ctxt->errInfo,stat,__FILE__,__LINE__)
#else
/**
 * Logs an ASN.1 error; this is used for ARM compilers that use the
 * __MODULE__ preprocessor definition instead of the __FILE__ one to
 * save memory.
 *
 * @param   ctxt     An OSCTXT structure (not a pointer).
 * @param   stat     The error status to be logged.
 */
#define LOG_ASN1ERR(ctxt,stat) \
      rtErrSetData(&ctxt->errInfo,stat,__MODULE__,__LINE__)
#endif /* ifndef __MODULE__ */

/**
 * Prints an ASN.1 octet string.  N.b., this makes a system call to printf.
 *
 * @param   os    The octet string to print.
 */
#define ASN1_PRINT_OCTSTR(os) \
printf ("%-*.*s", os.numocts, os.numocts, os.data)

#else /* if defined _COMPACT */
#undef _TRACE
/**
 * A no-op when compiling a compact library.
 *
 * @param   os    The octet string.
 */
#define ASN1_PRINT_OCTSTR(os)
#endif

/**
 * This logs an error to a global context and frees the local context used for
 * encoding or decoding.
 *
 * @param   ctxt_p      A pointer to the main context data structure.
 * @param   stat        The error status.
 * @param   lctxt       A pointer to the local encoding/decoding context.
 *
 * @return  The result of logging the error to the global context.
 */
#define LOG_RTERR_AND_FREE(ctxt_p,stat,lctxt) \
rtFreeContext ((lctxt)), LOG_RTERR(ctxt_p, stat)

/*
 * Memory allocations macros.  These are for use in generated ASN1C code only.
 */
#define ALLOC_ASN1ELEMDNODE_MEM(pctxt,type) \
(type*) (((char*)rtxMemHeapAlloc (pctxt, sizeof(type) + \
sizeof(OSRTDListNode))) + sizeof(OSRTDListNode))

#define ALLOC_ASN1ELEMDNODE(pctxt,type) \
(type*) (((char*)rtxMemHeapAllocZ (pctxt, sizeof(type) + \
sizeof(OSRTDListNode))) + sizeof(OSRTDListNode))

#define ALLOC_ASN1ELEMSNODE_MEM(pctxt,type) \
(type*) (((char*)rtxMemHeapAlloc (pctxt, sizeof(type) + \
sizeof(OSRTSListNode))) + sizeof(OSRTSListNode))

#define ALLOC_ASN1ELEMSNODE(pctxt,type) \
(type*) (((char*)rtxMemHeapAllocZ (pctxt, sizeof(type) + \
sizeof(OSRTSListNode))) + sizeof(OSRTSListNode))

#endif /* _ASN1INTL_H_ */
