/*
 * Copyright (c) 2003-2017 Objective Systems, Inc.
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
/**
 * @file rtContext.h
 * ASN.1 run-time context definitions.
 */
#ifndef _RTCONTEXT_H_
#define _RTCONTEXT_H_

#include "rtxsrc/rtxContext.h"
#include "rtxsrc/rtxSList.h"
#include "rtxsrc/rtxStack.h"
#include "rtsrc/asn1CEvtHndlr.h"

/**
 * @addtogroup rtxCtxt
 * @{
 */
/* ASN.1 size constraint structure */

/**
 * Defines a basic size value range, with unsigned 32-bit upper and lower
 * bounds.
 */
typedef struct _Asn1SizeValueRange {
   /** The lower bound. */
   OSSIZE lower;

   /** The upper bound. */
   OSSIZE upper;
} Asn1SizeValueRange;

/**
 * A basic size constraint structure that contains a root size constraint
 * and the extension size constraint, if applicable.
 */
typedef struct _Asn1SizeCnst {
   /** The root size constraint. */
   Asn1SizeValueRange root;

   /** The extension size constraint. */
   Asn1SizeValueRange ext;
} Asn1SizeCnst;

/**
 * Returns true if a size constraint contains an extended constraint.
 *
 * @param   pSizeCnst     A pointer to an _Asn1SizeCnst structure.
 *
 * @return  TRUE if the constraint's extension has an upper bound > 0.
 */
#define ASN1EXTSIZE(pSizeCnst) \
(OSBOOL)((0 != pSizeCnst) ? (pSizeCnst->ext.upper > 0) : FALSE)

/**
 * Returns true if a size constraint is fixed.
 *
 * @param   pSizeCnst      A pointer to an _Asn1SizeCnst structure.
 *
 * @return  TRUE if (1) there is no extension to the constraint and (2) the
 *             upper and lower bounds on the root are the same.
 */
#define ASN1FIXEDSIZE(pSizeCnst) \
(OSBOOL)((0 != pSizeCnst && 0 == pSizeCnst->ext.upper) ? \
(pSizeCnst->root.lower == pSizeCnst->root.upper) : FALSE)

/* Flag mask constant values */

/** This flag is set if context is dynamic */
#define ASN1DYNCTXT     0x8000
/* set if message is indefinite length */
#define ASN1INDEFLEN    0x4000
/** This flag is set if PER bit tracing is enabled. */
#define ASN1TRACE       0x2000
/** This flag indicates the last parsed item was an EOC. */
#define ASN1LASTEOC     0x1000
/** This flag turns on the "fast copy" mode. */
#define ASN1FASTCOPY    0x0800
/** This is the form of the last parsed tag. */
#define ASN1CONSTAG     0x0400
/** This flag turns on canonical XER mode.
   Note that ASN1CANON uses the same bit. */
#define ASN1CANXER      0x0200

/** This flag turns on canonical OER mode.
   It could be used, in the future, for other
   encoding rules as well. */
#define ASN1CANON       0x0200

/** This flag indicates the item is an open type field. */
#define ASN1OPENTYPE    0x0080
/** This flag indicates that BER's boolean TRUE is 0x01 instead of 0xFF. */
#define ASN1BOOLTRUE1   0x0040
/** This flag indicates that null-terminators will be escaped in character
 * strings. */
#define ASN1ESCZTERM    0x0020

/* ASN.1 encode/decode context block structure */

/**
 * This structure contains elements used when decoding and encoding in ASN.1
 * applications.  The structure fills in the pAsn1Info element of the
 * ::OSCTXT data structure.
 */
typedef struct {
   /** A pointer to the context free function callback. */
   OSFreeCtxtAppInfoPtr  pFreeFunc;

   /** A pointer to the context reset function callback. */
   OSResetCtxtAppInfoPtr pResetFunc;

   /** The PER field list. */
   OSRTSList    fieldList;

   /** The PER effective size constraint. */
   Asn1SizeCnst sizeConstraint;

   /** The string of permitted characters. */
   const char*  pCharSet;

   /** The element name stack, used in tracing and debugging. */
   OSRTStack    nameStack;

   /** The event handler object list. */
   OSRTSList    evtHndlrList;

   /**
    * The error handler callback function. This is maintained as a
    * void pointer because it is stored in different forms in C and C++.
    * In C, it holds a pointer to an error handling callback function of
    * type rtErrorHandler.  For C++, it holds the address of an
    * Asn1ErrorHandler structure which contains an error virtual method
    * which is overridden by the user to handle the error.
    */
   void*        errHndlrCB;
} OSASN1CtxtInfo;


/**
 * A macro used to provide easy access to the ASN.1 info in a context.
 *
 * @param   pctxt    A pointer to the OSCTXT data structure.
 * @return  The properly-casted pASN1Info member of the context.
 */
#define ACINFO(pctxt)           ((OSASN1CtxtInfo*)(pctxt)->pASN1Info)

/**
 * Returns the current byte in the context buffer.
 *
 * @param   cp    A pointer to an OSCTXT data structure.
 * @return  The byte at the current byte index in the context data buffer.
 */
#define ASN1BUFCUR(cp)          (cp)->buffer.data[(cp)->buffer.byteIndex]

/**
 * Returns a pointer to the current byte in the context buffer.
 *
 * @param   cp    A pointer to an OSCTXT data structure.
 * @return  A pointer to the current byte index in the context data buffer.
 */
#define ASN1BUFPTR(cp)          &(cp)->buffer.data[(cp)->buffer.byteIndex]

/**
 * Returns true if the context's buffer is flagged indefinite length.
 *
 * @param   cp    A pointer to an OSCTXT data structure.
 * @return  TRUE if the ASN1INDEFLEN flag is set in the context.
 */
#define ASN1BUF_INDEFLEN(cp)    (((cp)->flags&ASN1INDEFLEN)!=0)

/**
 * Another handle for ASN1BUFPTR.
 * @param   cp    A pointer to an OSCTXT data structure.
 * @return  A pointer to the current byte index in the context data buffer.
 *
 * @see  ASN1BUFPTR
 */
#define ASN1BUF_PTR(cp)         ASN1BUFPTR(cp)

#ifdef __cplusplus
extern "C" {
#endif
/**
 * This function initializes a context using a run-time key.  This form
 * is required for evaluation and limited distribution software.  The
 * compiler will generate a macro for rtInitContext in the rtkey.h
 * file that will invoke this function with the generated run-time key.
 *
 * @param pctxt        The pointer to the context structure variable to be
 *                       initialized.
 * @param key          Key data generated by ASN1C compiler.
 * @param keylen       Key data field length.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
EXTERNRT int rtInitContextUsingKey
(OSCTXT* pctxt, const OSOCTET* key, size_t keylen);

/**
 * This function initializes an OSCTXT block. It makes sure that if the block
 * was not previously initialized, that all key working parameters are set to
 * their correct initial state values (i.e. declared within a function as a
 * normal working variable).  A user must call this function or
 * rtNewContext before making any other run-time library calls.
 *
 * @param pctxt  Pointer to context structure variable to be initialized.
 * @return             Completion status of operation:
 *                       - 0 (ASN_OK) = success,
 *                       - negative return value is error.
 */
#ifndef rtInitContext
EXTERNRT int rtInitContext (OSCTXT* pctxt);
#endif

/**
 * This function initializes an OSCTXT block using an existing context
 * as a basis.  This provides improved performance over initializing a
 * context from scratch because the overhead of creating a memory heap
 * and doing license checking is eliminated.  The new context shares
 * the memory heap of the existing context.
 *
 * @param pctxt  Pointer to context structure variable to be initialized.
 * @param psrc   Source context to use fo initialization.
 * @return       Completion status of operation:
 *                 - 0 (ASN_OK) = success,
 *                 - negative return value is error.
 */
EXTERNRT int rtInitSubContext (OSCTXT* pctxt, OSCTXT* psrc);

/**
 * This function allocates a new OSCTXT block and initializes it. Although
 * the block is allocated from the standard heap, it should not be freed using
 * free. The rtFreeContext function should be used because this frees items
 * allocated within the block before freeing the block itself.
 *
 * This is the preferred way of setting up a new encode or decode context
 * because it ensures the block is properly initialized before using it. If a
 * context variable is declared on the stack, the user must first remember to
 * invoke the rtInitContext function on it. This can be a source of errors.
 * This function can be called directly when setting up BER context or it will
 * be invoked from within the pu_newContext call for PER.
 *
 * @return             Newly allocated context if successful,
 *                       NULL if failure.
 */
#ifndef rtNewContext
EXTERNRT OSCTXT* rtNewContext (OSVOIDARG);
#endif

/**
 * This function allocates a new OSCTXT block and initializes it. Although
 * the block is allocated from the standard heap, it should not be freed using
 * free. The rtFreeContext function should be used because this frees items
 * allocated within the block before freeing the block itself.
 *
 * This is the preferred way of setting up a new encode or decode context
 * because it ensures the block is properly initialized before using it. If a
 * context variable is declared on the stack, the user must first remember to
 * invoke the rtInitContext function on it. This can be a source of errors.
 * This function can be called directly when setting up BER context or it will
 * be invoked from within the pu_newContext call for PER.
 *
 * @param key          Key data generated by ASN1C compiler.
 * @param keylen       Key data field length.
 * @return             Newly allocated context if successful,
 *                       NULL if failure.
 */
EXTERNRT OSCTXT* rtNewContextUsingKey (const OSOCTET* key, size_t keylen);

/**
 * This function frees all dynamic memory associated with a context. This
 * includes all memory inside the block (in particular, the list of memory
 * blocks used by the rtMem functions) as well as the block itself if allocated
 * with the rtNewContext function
 *
 * @param pctxt        A pointer to a context structure.
 */
EXTERNRT void rtFreeContext (OSCTXT* pctxt);

/**
 * This function is deprecated, use ::rtSetFastCopy.
 */
EXTERNRT void rtSetCopyValues (OSCTXT* pctxt, OSBOOL value);

/**
 * This function sets the ASN1FASTCOPY flag in the specified context. This flag
 * has effect only if decoding is being performed. If this flag is set then
 * some decoded data (BIT STRINGs, OCTET STRINGs or OPEN TYPEs) will not
 * be copied.  Instead, a pointer to the location of the data in the decode
 * buffer will be stored in the generated structure.  This may improve
 * decoding performance, but should be used carefully. If the decode
 * buffer is destroyed, all uncopied data will be lost. Use only if you
 * are sure the decode buffer will be available when data in the decoded
 * structure is processed. By default this flag is not set.
 *
 * @param pctxt        Pointer to context block structure.
 * @param value        Boolean value of the flag to set.
 */
EXTERNRT void rtSetFastCopy (OSCTXT* pctxt, OSBOOL value);

/**
 * @} rtxCtxt
 */

/* Internal function defs */
/*
 * This function initializes the ASN.1 error table.
 */
void rtErrASN1Init (OSVOIDARG);

/*
 * This function initializes an ASN.1 context.  Users should call the
 * rtInitContext function instead of this one as this is for internal
 * use only by the run-time.
 */
#ifndef rtInitASN1Context
EXTERNRT int rtInitASN1Context (OSCTXT* pctxt);
#endif

/*
 * This function initializes the ASN.1 information block stored in an OSCTXT
 * data structure.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 *
 * @return  0 on success; less than 0 otherwise.
 */
EXTERNRT int rtCtxtInitASN1Info (OSCTXT* pctxt);
#ifdef __cplusplus
}
#endif

#endif
