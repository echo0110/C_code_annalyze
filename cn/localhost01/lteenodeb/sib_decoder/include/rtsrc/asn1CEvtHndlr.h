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
 * @file asn1CEvtHndlr.h
 * C event handler structure. The ASN1CEventHandler type is a structured type
 * that can be used to define event handlers by the user.
 */
/**
 * @defgroup Asn1CEventHandler Asn1CEventHandler
 * Asn1CEventHandler is a structure typed used for user-defined event handlers.
 * @ingroup cppruntime
 * @{
 */
#ifndef _ASN1CEVTHNDLR_H_
#define _ASN1CEVTHNDLR_H_

#include <stdio.h>
#include "rtsrc/asn1type.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * This is a function pointer for a callback function which is invoked
 * from within a decode function when an element of a SEQUENCE, SET,
 * SEQUENCE OF, SET OF, or CHOICE construct is parsed.
 *
 * @param name         For SEQUENCE, SET, or CHOICE, this is the name of the
 *                       element as defined in the ASN.1 defination. For
 *                       SEQUENCE OF or SET OF, this is set to the name
 *                       "element".
 * @param idx          For SEQUENCE, SET, or CHOICE, this is not used and is
 *                       set to the value
 *                       -1. For SEQUENCE OF or SET OF, this contains the
 *                       zero-based index of the element in the conceptual
 *                       array associated with the construct.
 * @return             - none
 */
typedef void (*rtxStartElement) (const char* name, OSSIZE idx) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when parsing is complete on an element of a
 * SEQUENCE, SET, SEQUENCE OF, SET OF, or CHOICE construct.
 *
 * @param name         For SEQUENCE, SET, or CHOICE, this is the name of the
 *                       element as defined in the ASN.1 defination. For
 *                       SEQUENCE OF or SET OF, this is set to the name
 *                       "element".
 * @param idx          For SEQUENCE, SET, or CHOICE, this is not used and is
 *                       set to the value
 *                       -1. For SEQUENCE OF or SET OF, this contains the
 *                       zero-based index of the element in the conceptual
 *                       array associated with the construct.
 * @return             - none
 */
typedef void (*rtxEndElement) (const char* name, OSSIZE idx) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the BOOLEAN ASN.1 type is parsed.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxBoolValue) (OSBOOL value);

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the INTERGER ASN.1 type is parsed.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxIntValue) (OSINT32 value);

/**
 * This is a function pointer for a callback function which is invoked
 * from within a decode function when a value of the INTEGER ASN.1 type
 * is parsed. In this case, constraints on the integer value forced the
 * use of unsigned integer C type to represent the value.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxUIntValue) (OSUINT32 value);

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the 64-bit INTERGER ASN.1 type
 * is parsed.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxInt64Value) (OSINT64 value);

/**
 * This is a function pointer for a callback function which is invoked
 * from within a decode function when a value of the 64-bit INTEGER ASN.1 type
 * is parsed.
 *
 * @param value        Parsed value.
 * @return             - none
 */
typedef void (*rtxUInt64Value) (OSUINT64 value);

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the BIT STRING ASN.1 type is
 * parsed.
 *
 * @param numbits      - Number of bits in the parsed value.
 * @param data         - Pointer to a byte array that contains the bit
 *                         string data.
 * @return             - none
 */
typedef void (*rtxBitStrValue) (OSSIZE numbits, const OSOCTET* data);

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of one of the OCTET STRING ASN.1 type
 * is parsed.
 *
 * @param numocts      Number of octets in the parsed value.
 * @param data         Pointer to byte array containing the octet string
 *                       data.
 * @return             - none
 */
typedef void (*rtxOctStrValue) (OSSIZE numocts, const OSOCTET* data) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of one of the 8-bit ASN.1 character
 * string types is parsed.
 *
 * @param value        Null terminated character string value.
 * @return             - none
 */
typedef void (*rtxCharStrValue) (const char* value) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of one of the 16-bit ASN.1 character
 * string types is parsed.
 *
 * This is used for the ASN.1 BmpString type.
 *
 * @param nchars       Number of characters in the parsed value.
 * @param data         Pointer to an array containing 16-bit values.
 *                       These are represented using unsigned short integer
 *                       values.
 * @return             - none
 */
typedef void (*rtxCharStrValue16Bit) (OSSIZE nchars, OSUNICHAR* data) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of one of the 32-bit ASN.1 characer
 * string types is parsed.
 *
 * This is used for the ASN.1 UniversalString type.
 *
 * @param nchars       Number of characters in the parsed value.
 * @param data         Pointer to an array containing 32-bit values.
 *                       Each 32-bit integer value is a universal character.
 * @return             - none
 */
typedef void (*rtxCharStrValue32Bit) (OSSIZE nchars, OS32BITCHAR* data) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the NULL ASN.1 type is parsed.
 *
 * @param             - none
 * @return             - none
 */
typedef void (*rtxNullValue) (void) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function whn a value the OBJECT IDENTIFIER ASN.1 type is
 * parsed.
 *
 * @param numSubIds    Number of subidentifiers in the object identifier.
 * @param pSubIds      Pointer to array containing the subidentifier values.
 * @return             -none
 */
typedef void (*rtxOidValue) (OSUINT32 numSubIds, OSUINT32* pSubIds) ;

 /**
  * This is a function pointer for a callback function which is invoked from
  * within a decode function when a value the REAL ASN.1 type is parsed.
  *
  * @param value        Parsed value.
  * @return             - none
  */
typedef void (*rtxRealValue) (double value) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when a value of the ENUMERATED ASN.1 type is
 * parsed.
 *
 * @param value        - Parsed enumerated value
 * @param text         - Textual value of the enumerated identifier
 * @return             - none
 */
typedef void (*rtxEnumValue) (OSUINT32 value, const OSUTF8CHAR* text);

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when an ASN.1 open type is parsed.
 *
 * @param numocts      Number of octets in the parsed value.
 * @param data         Pointer to byet array contain in tencoded ASN.1
 *                       value.
 * @return             - none
 */
typedef void (*rtxOpenTypeValue) (OSSIZE numocts, const OSOCTET* data) ;

/**
 * This is a function pointer for a callback function which is invoked from
 * within a decode function when an error occurs.  This allows users to
 * intercept errors in decoding and recover from them.
 *
 * @param pctxt        Pointer to an ::OSCTXT data structure.
 * @param pCCB         Pointer to an ::ASN1CCB data structure; this is the
 *                      context control block that contains information
 *                      pertinent to parsing data in an ASN.1 message.
 * @param stat         The error status.
 *
 * @return             The new error status.
 */
typedef int (*rtErrorHandler) (OSCTXT *pctxt, ASN1CCB *pCCB, int stat);

/**
 * This is a basic C based event handler structure, which can be used
 * to define user-defined event handlers.
 */
typedef struct Asn1NamedCEventHandler {
   rtxStartElement      startElement;
   rtxEndElement        endElement;
   rtxBoolValue         boolValue;
   rtxIntValue          intValue;
   rtxUIntValue         uIntValue;
   rtxInt64Value        int64Value;
   rtxUInt64Value       uInt64Value;
   rtxBitStrValue       bitStrValue;
   rtxOctStrValue       octStrValue;
   rtxCharStrValue      charStrValue;
   rtxCharStrValue16Bit charStrValue16Bit;
   rtxCharStrValue32Bit charStrValue32Bit;
   rtxNullValue         nullValue;
   rtxOidValue          oidValue;
   rtxRealValue         realValue;
   rtxEnumValue         enumValue;
   rtxOpenTypeValue     openTypeValue;
} Asn1NamedCEventHandler;

/**
 * This function is called to add a new event handler to the context event
 * handler list.
 *
 * @param pCtxt       Context to which event handler has to be added.
 * @param pHandler    Pointer to the event handler structure.
 * @return            none
 */

EXTERNRT void rtAddEventHandler
(OSCTXT* pCtxt, Asn1NamedCEventHandler* pHandler);

/**
 * This function is called to remove an event handler from the context event
 * handler list.  Note that it does not delete the event handler object.
 *
 * @param pCtxt       Context from which event handler has to be removed.
 * @param pHandler    Pointer to event handler structure.
 * @return            none
 */
EXTERNRT void rtRemoveEventHandler
(OSCTXT* pCtxt, Asn1NamedCEventHandler* pHandler);

/**
 * This function is called to set the error handler in an ::OSCTXT data
 * structure.  The error handler is called within generated code if an
 * error occurs during message parsing.  Only one error handler may be used
 * at a time.
 *
 * @param pctxt      A pointer to an ::OSCTXT structure used for decoding.
 * @param pErrCB     A pointer to the ::rtErrorHandler callback function.
 *
 * @return           None.
 */
EXTERNRT void rtSetErrorHandler (OSCTXT *pctxt, rtErrorHandler pErrCB);

/**
 * This function removes the error handler from the given context.
 *
 * @param pctxt      A pointer to an ::OSCTXT data structure.
 *
 * @return           None.
 */
EXTERNRT void rtRemoveErrorHandler (OSCTXT *pctxt);

/**
 * This function invokes the error handler callback defined inside the
 * context.
 *
 * @param   pctxt    A pointer to an ::OSCTXT data structure.
 * @param   pCCB     A pointer to an ::ASN1CCB data structure.
 * @param   stat     The error status encountered.
 *
 * @return  None.
 *
 * @see  ::rtSetErrorHandler
 * @see  ::rtErrorHandler
 */
EXTERNRT int rtInvokeErrorHandler (OSCTXT *pctxt, ASN1CCB *pCCB, int stat);

/*
 * The following functions are invoked from within the generated
 * code to call the various user-defined event handler methods ..
 */

/**
 * This function is called by generated code to invoke the event handler's
 * StartElement function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   name     The name of the element on which the function is called.
 * @param   idx      The index of the element (for use in SEQUENCE OF items).
 */
EXTERNRT void rtInvokeStartElement
(OSCTXT* pCtxt, const char* name, OSSIZE idx);

/**
 * This function is called by generated code to invoke the event handler's
 * end element function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   name     The name of the element on which the function is called.
 * @param   idx      The index of the element (for use in SEQUENCE OF items).
 */
EXTERNRT void rtInvokeEndElement
(OSCTXT* pCtxt, const char* name, OSSIZE idx);

/**
 * This function is called by generated code to invoke the event handler's
 * boolean value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   value    The decoded boolean value passed to the event handler.
 */
EXTERNRT void rtInvokeBoolValue (OSCTXT* pCtxt, OSBOOL value);

/**
 * This function is called by generated code to invoke the event handler's
 * signed integer value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   value    The decoded signed integer value passed to the event
 *                      handler.
 */
EXTERNRT void rtInvokeIntValue (OSCTXT* pCtxt, OSINT32 value);

/**
 * This function is called by generated code to invoke the event handler's
 * unsigned integer value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   value    The decoded unsigned integer value passed to the event
 *                      handler.
 */
EXTERNRT void rtInvokeUIntValue (OSCTXT* pCtxt, OSUINT32 value);

/**
 * This function is called by generated code to invoke the event handler's
 * signed 64-bit integer value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   value    The decoded signed 64-bit integer value passed to the
 *                      event handler.
 */
EXTERNRT void rtInvokeInt64Value (OSCTXT* pCtxt, OSINT64 value);

/**
 * This function is called by generated code to invoke the event handler's
 * unsigned 64-bit integer value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   value    The decoded unsigned 64-bit integer value passed to the
 *                      event handler.
 */
EXTERNRT void rtInvokeUInt64Value (OSCTXT* pCtxt, OSUINT64 value);

/**
 * This function is called by generated code to invoke the event handler's
 * bit string value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   numbits  The number of bits contained in the decoded bit string.
 * @param   data     A pointer to the decoded octets containing the data.
 */
EXTERNRT void rtInvokeBitStrValue
(OSCTXT* pCtxt, OSSIZE numbits, const OSOCTET* data);

/**
 * This function is called by generated code to invoke the event handler's
 * octet string value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   numocts  The number of octets contained in the decoded octet string.
 * @param   data     A pointer to the decoded octets containing the data.
 */
EXTERNRT void rtInvokeOctStrValue
(OSCTXT* pCtxt, OSSIZE numocts, const OSOCTET* data);

/**
 * This function is called by generated code to invoke the event handler's
 * character string value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   value    The decoded character string.
 */
EXTERNRT void rtInvokeCharStrValue (OSCTXT* pCtxt, const char* value);

/**
 * This function is called by generated code to invoke the event handler's
 * 16-bit character string value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   nchars   The number of characters in the string.
 * @param   value    The decoded 16-bit character string.
 */
EXTERNRT void rtInvokeCharStr16BitValue
(OSCTXT* pCtxt, OSSIZE nchars, OSUNICHAR* data);

/**
 * This function is called by generated code to invoke the event handler's
 * 32-bit character string value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   nchars   The number of characters in the string.
 * @param   value    The decoded 32-bit character string.
 */
EXTERNRT void rtInvokeCharStr32BitValue
(OSCTXT* pCtxt, OSSIZE nchars, OS32BITCHAR* data);

/**
 * This function is called by generated code to invoke the event handler's
 * null value function.  No value is required for NULL types.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 */
EXTERNRT void rtInvokeNullValue (OSCTXT* pCtxt);

/**
 * This function is called by generated code to invoke the event handler's
 * object identifier value function.
 *
 * @param   pctxt       A pointer to an OSCTXT data structure.
 * @param   numSubIds   The number of sub ids in the OID value.
 * @param   pSubIds     The sub ids that comprise the OID value.
 */
EXTERNRT void rtInvokeOidValue
(OSCTXT* pCtxt, OSUINT32 numSubIds, OSUINT32* pSubIds);

/**
 * This function is called by generated code to invoke the event handler's
 * real value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   value    The decoded real value.
 */
EXTERNRT void rtInvokeRealValue (OSCTXT* pCtxt, double value);

/**
 * This function is called by generated code to invoke the event handler's
 * enumerated value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   value    The integer value corresponding to the enumerated value.
 * @param   text     The character string text that represents the enumerated
 *                      value.
 */
EXTERNRT void rtInvokeEnumValue
(OSCTXT* pCtxt, OSUINT32 value, const OSUTF8CHAR* text);

/**
 * This function is called by generated code to invoke the event handler's
 * open type value function.
 *
 * @param   pctxt    A pointer to an OSCTXT data structure.
 * @param   numocts  The number of octets the decoded open type contains.
 * @param   data     The content of the decoded open type.
 */
EXTERNRT void rtInvokeOpenTypeValue
(OSCTXT* pCtxt, OSSIZE numocts, const OSOCTET* data);


#ifdef __cplusplus
}
#endif

#endif

/**
 * @}
 */
