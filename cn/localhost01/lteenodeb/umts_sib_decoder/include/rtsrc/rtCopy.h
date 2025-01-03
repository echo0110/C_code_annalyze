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

/**
 * @file rtsrc/rtCopy.h  Functions for copying values of primitive
 * ASN.1 types.
 */
#ifndef _RTCOPY_H_
#define _RTCOPY_H_
#include "rtsrc/asn1type.h"

#ifdef __cplusplus
extern "C" {
#endif
/**
 * @defgroup copy Copy Functions
 * @ingroup cruntime
 * @{
 * This group of functions allows copying values of primitive ASN.1 types.
 *
 * These functions are used in copy routines that are generated by the ASN.1
 * complier when <i>-gencopy</i> option is used. Some primitive types that are
 * mapped onto C standard primitive types (such as BOOLEAN, INTEGER REAL) do
 * not need copy functions. The standard assingment operator can be used to
 * copy these types.
 */

/**
 * The rtCopyBitStr64 function copies one ASN.1 BIT STRING value to another.
 * This function is safe fo use in 64-bit environments.
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * @param srcNumbits   The number of bits in the source value to copy.
 * @param pSrcData     The pointer to data of the source value to copy.
 * @param pDstNumbits  The pointer to destination number of bits. The
 *                       srcNumbits argument will be copied into it.
 * @param pDstData     The pointer to the destination buffer to receive the
 *                       copied data. The buffer is assumed to be already
 *                       allocated or static and should be enough to receive
 *                       the copying data.
 * @param dstDataSize  Size of destination data static buffer.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyBitStr64
(OSSIZE srcNumbits, const OSOCTET* pSrcData,
 OSSIZE* pDstNumbits, OSOCTET* pDstData, OSSIZE dstDataSize);

/**
 * The rtCopyBitStr function copies one ASN.1 BIT STRING value to another.
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * This function is deprecated and considered to be unsafe because the
 * size of the destination buffer is not specified.  It exists for backward
 * compatibility with previous versions only.
 *
 * @param srcNumbits   The number of bits in the source value to copy.
 * @param pSrcData     The pointer to data of the source value to copy.
 * @param pDstNumbits  The pointer to destination number of bits. The
 *                       srcNumbits argument will be copied into it.
 * @param pDstData     The pointer to the destination buffer to receive the
 *                       copied data. The buffer is assumed to be already
 *                       allocated or static and should be enough to receive
 *                       the copying data.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyBitStr (OSUINT32 srcNumbits,
                              const OSOCTET* pSrcData,
                              OSUINT32* pDstNumbits, OSOCTET* pDstData);

/**
 * The rtCopyDynBitStr64 function is similar to the rtCopyBitStr64, but it
 * copies a dynamic ASN.1 BIT STRING value.
 *
 * The return vale is one of the TRUE (copied successfully) or FALSE (error has
 * occurred).
 *
 * This function is safe for use in 64-bit environments.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pSrcData     The pointer to data of the source value to copy.
 * @param pDstData     The pointer to the destination dynamic bit string
 *                       structure to receive the copied data. The memory will
 *                       be allocated dynamically via call to rtxMemAlloc
 *                       function.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyDynBitStr64
   (OSCTXT* pctxt, const ASN1DynBitStr64* pSrcData, ASN1DynBitStr64* pDstData);

/**
 * The rtCopyDynBitStr function is similar to the rtCopyBitStr, but it
 * copies a dynamic ASN.1 BIT STRING value.
 *
 * The return vale is one of the TRUE (copied successfully) or FALSE (error has
 * occurred).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pSrcData     The pointer to data of the source value to copy.
 * @param pDstData     The pointer to the destination dynamic bit string
 *                       structure to receive the copied data. The memory will
 *                       be allocated dynamically via call to rtxMemAlloc
 *                       function.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyDynBitStr
   (OSCTXT* pctxt, const ASN1DynBitStr* pSrcData, ASN1DynBitStr* pDstData);

/**
 * The rtCopyOctStr function copies one ASN.1 OCTET STRING value to another.
 * This function is safe fo use in 64-bit environments.
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * @param srcNumocts   The number of octets in the source value to copy.
 * @param pSrcData     The pointer to data of the source value to copy.
 * @param pDstNumocts  The pointer to the destination number of octets. The
 *                       srcNumocts argument will be copied into it.
 * @param pDstData     The pointer to the destination buffer to receive the
 *                       copied data. The buffer is assumed to be already
 *                       allocated or static and should be enough to receive
 *                       the copying data.
 * @param dstDataSize  Size of destination data static buffer.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyOctStr64
   (OSSIZE srcNumocts, const OSOCTET* pSrcData,
    OSSIZE* pDstNumocts, OSOCTET* pDstData, OSSIZE dstDataSize);

/**
 * The rtCopyOctStr function copies one ASN.1 OCTET STRING value to another.
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * This function is deprecated and considered to be unsafe because the
 * size of the destination buffer is not specified.  It exists for backward
 * compatibility with previous versions only.
 *
 * @param srcNumocts   The number of octets in the source value to copy.
 * @param pSrcData     The pointer to data of the source value to copy.
 * @param pDstNumocts  The pointer to the destination number of octets. The
 *                       srcNumocts argument will be copied into it.
 * @param pDstData     The pointer to the destination buffer to receive the
 *                       copied data. The buffer is assumed to be already
 *                       allocated or static and should be enough to receive
 *                       the copying data.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyOctStr
   (OSUINT32 srcNumocts, const OSOCTET* pSrcData,
    OSUINT32* pDstNumocts, OSOCTET* pDstData);

/**
 * The rtCopyDynOctStr funtion is similar to rtCopyOctStr, but it copies a
 * dynamic ASN.1 OCTET STRING value.
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * This function is safe for use in 64-bit environments.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pSrcData     The pointer to the source dynamic octet string structure
 *                       to copy.
 * @param pDstData     The point to destination dynamic octet string structure
 *                       to receive the copied data. The memory will be
 *                       allocated dynamically via a call to rtxMemAlloc
 *                       function.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyDynOctStr64
   (OSCTXT* pctxt, const OSDynOctStr64* pSrcData, OSDynOctStr64* pDstData);

/**
 * The rtCopyDynOctStr funtion is similar to rtCopyOctStr, but it copies a
 * dynamic ASN.1 OCTET STRING value.
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param pSrcData     The pointer to the source dynamic octet string structure
 *                       to copy.
 * @param pDstData     The point to destination dynamic octet string structure
 *                       to receive the copied data. The memory will be
 *                       allocated dynamically via a call to rtxMemAlloc
 *                       function.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyDynOctStr
   (OSCTXT* pctxt, const ASN1DynOctStr* pSrcData, ASN1DynOctStr* pDstData);

/**
 * The rtCopyCharStr function copies one ASN.1 8-bit character string value to
 * another (including IA5String, VisibleString, PrintableString, NumericString,
 * etc).
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param srcStr       The pointer to the source standard null-terminated
 *                       string to copy.
 * @param dstStr       The pointer to pointer destination string to receive the
 *                       copied string. The memory will be allocated
 *                       dynamically via a call to rtxMemAlloc function.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyCharStr
   (OSCTXT* pctxt, const char* srcStr, char** dstStr);

/**
 * The rtCopy16BitCharStr function copies one ASN.1 16-bit character string
 * value to another (generally BMPString).
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param srcStr       The pointer to the source 16-bit character string
 *                       structure to copy.
 * @param dstStr       The pointer to destination 16-bit character string
 *                       structure to receive the copied string. The memory
 *                       will be allocated dynamically via call to rtxMemAlloc
 *                       function.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopy16BitCharStr
   (OSCTXT* pctxt, const Asn116BitCharString* srcStr, Asn116BitCharString* dstStr);

/**
 * The rtCopy32BitCharStr function copies one ASN.1 32-bit character string
 * value to another (generally UniversalString).
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param srcStr       The pointer to the source 32-bit character string
 *                       structure to copy.
 * @param dstStr       The pointer to destination 32-bit character string
 *                       structure to receive the copied string. The memory
 *                       will be allocated dynamically via call to rtxMemAlloc
 *                       function.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopy32BitCharStr
   (OSCTXT* pctxt, const Asn132BitCharString* srcStr, Asn132BitCharString* dstStr);

/**
 * The rtCopyIOD function copies one ASN.1 OBJECT IDENTIFIER or RELATED-IOD
 * value to another.
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * @param srcOID       The pointer to the source object identifier structure to
 *                       copy.
 * @param dstOID       The pointer to destination structure t receive the
 *                       copied string.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyOID
   (const ASN1OBJID* srcOID, ASN1OBJID* dstOID);

/**
 * The rtCopyOID64 function copies one 64-bit ASN.1 OBJECT IDENTIFIER or
 * RELATIVE-OID value to another.
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * @param srcOID       The pointer to the source object identifier structure to
 *                       copy.
 * @param dstOID       The pointer to destination structure t receive the
 *                       copied string.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyOID64
   (const ASN1OID64* srcOID, ASN1OID64* dstOID);

/**
 * The rtCopyOpenType copies ASN.1 value of the old (pre- 1994) ASN.1 ANY type
 * or other elements defined in the later standards to be Open Types (for
 * example, a variable type declaration in a CLASS construct as defined in
 * X.681).
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred).
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param srcOT        The pointer to the source Open Type structure to copy.
 * @param dstOT        The pointer to the destination Open Type structure to
 *                       receive the copied data. The memory will be allocated
 *                       dynamically via call to the rtxMemAlloc function.
 * @return             The copying result. TRUE, if success, otherwise FALSE.
 */
EXTERNRT OSBOOL rtCopyOpenType
   (OSCTXT* pctxt, const ASN1OpenType* srcOT, ASN1OpenType* dstOT);

/**
 * The rtCopyOpenTypeExt function copies an ASN.1 open type extension value.
 *
 * The return value is one of the TRUE (copied successfully) or FALSE (error
 * has occurred). An open type extension is defined as extensibility marker on
 * a constructed type without any extension elements defined (for example,
 * SEQUENCE { a INTEGER, ... }). The difference is that this is an implicit
 * field that can span more elements whereas the standard Open Type is assumed
 * to be a single tagged field.
 *
 * @param pctxt        Pointer to a context structure. This provides a storage
 *                       area for the function to store all working variables
 *                       that must be maintained between function calls.
 * @param srcList      The pointer to the source linked list structure to copy.
 *                       The list should consist of ASN1OpenType elements.
 * @param dstList      The pointer to destination linked list structure to
 *                       receive the copied data. The memory for list nodes and
 *                       data will be allocated dynamically via call to the
 *                       rtxMemAlloc function. The list nodes will contain the
 *                       data of ASN1OpenType type.
 */
EXTERNRT OSBOOL rtCopyOpenTypeExt
   (OSCTXT* pctxt, const OSRTDList* srcList, OSRTDList* dstList);
/**
 * @}
 */

/* Macros */

/**
 * This macro copies the source string to the destination string by calling
 * the common runtime function ::rtCopyCharStr.  This function allocates
 * memory on the managed heap to store the string that will be released when
 * ::rtxMemFree or ::rtFreeContext are called.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   src      The source string.
 * @param   dst      The destination string.
 */
#define RTCOPYCHARSTR(pctxt,src,dst) \
do { char* ptr; rtCopyCharStr (pctxt, src, &ptr); *dst = ptr; } while(0)

/**
 * This macro copies the source string to the destination string by calling
 * the common runtime function ::rtCopyCharStr.  This function allocates
 * memory on the managed heap to store the string that will be released when
 * ::rtxMemFree or ::rtFreeContext are called. This macro contains an error
 * status check and will return a failure if needed.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   src      The source string.
 * @param   dst      The destination string.
 * @param   log      Whether to log the error or just return the code.
 */
#define RTCHKCOPYCHARSTR(pctxt,src,dst,log) \
do { \
   char* ptr; \
   if (! rtCopyCharStr (pctxt, src, &ptr)) { \
      if (log) return LOG_RTERR (pctxt, RTERR_COPYFAIL); \
      else return RTERR_COPYFAIL; \
   } \
   *dst = ptr; \
} while(0)

#ifdef __cplusplus
}
#endif

#endif /* _RTCOPY_H_ */
