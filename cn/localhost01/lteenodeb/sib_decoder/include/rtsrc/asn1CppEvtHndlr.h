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

// rtsrc/asn1CppEvtHndlr.h - this .h file defines the object that is registered
// for the C++ version of the event handler feature.

/**
 * @file rtsrc/asn1CppEvtHndlr.h
 * Named event handler base class.  The Asn1Named Event Handler class is
 * an abstract base class from which user-defined event handlers are derived.
 * This class contains pure virtual function definitions for all of the
 * methods that must be implemented to create a customized event handler
 * class.
 */
/**
 * @defgroup Asn1NamedEventHandler Named Event Handlers
 * Named Event Handler Classes include base classes from which user-defined
 * error handler and event handler classes are derived.
 * @ingroup cppruntime
 * @{
 */
#ifndef _ASN1CPPEVTHNDLR_H_
#define _ASN1CPPEVTHNDLR_H_

#include "rtsrc/asn1type.h"

#ifndef OS_UNUSED_ARG
/**
 * This macro is used to prevent warnings of unused arguments passed to
 * generated functions and methods.
 *
 * @param   arg      The argument to be passed.
 */
#define OS_UNUSED_ARG(arg) (void)arg
#endif

// The following pure abstract class defines the event handler interface
// callback functions that are invoked as a message is parsed..
/**
 * Named event handler base class.  This is the base class from which
 * user-defined event handler classes are derived.  These classes can be
 * used to handle events during the parsing of an ASN.1 message.  The
 * event callback methods that can be implemented are startElement,
 * endElement, and contents methods.
 */
class EXTRTCLASS Asn1NamedEventHandler {
 public:
   Asn1NamedEventHandler() {}
   virtual ~Asn1NamedEventHandler() {}

   // Start element callback function.  This is invoked upon entry into
   // an element in a constructed type (SEQUENCE, SET, SEQUENCE OF,
   // SET OF, or CHOICE).  A single char* argument (name) is passed.
   /**
    * This method is invoked from within a decode function when an element of a
    * SEQUENCE, SET, SEQUENCE OF, SET OF, or CHOICE construct is parsed.
    *
    * @param name         For SEQUENCE, SET, or CHOICE, this is the name of the
    *                       element as defined in the ASN.1 defination. For
    *                       SEQUENCE OF or SET OF, this is set to the name
    *                       "element".
    * @param index        For SEQUENCE, SET, or CHOICE, this is not used and is
    *                       set to the value
    *                       -1. For SEQUENCE OF or SET OF, this contains the
    *                       zero-based index of the element in the conceptual
    *                       array associated with the construct.
    * @return             - none
    */
   virtual void startElement (const char* name, int index) = 0;

   // End element callback function.  This is invoked upon exit from
   // a constructed type.  A single char* argument (name) is passed.
   /**
    * This method is invoked from within a decode function when parsing is
    * complete on an element of a SEQUENCE, SET, SEQUENCE OF, SET OF, or CHOICE
    * construct.
    *
    * @param name         For SEQUENCE, SET, or CHOICE, this is the name of the
    *                       element as defined in the ASN.1 defination. For
    *                       SEQUENCE OF or SET OF, this is set to the name
    *                       "element".
    * @param index        For SEQUENCE, SET, or CHOICE, this is not used and is
    *                       set to the value
    *                       -1. For SEQUENCE OF or SET OF, this contains the
    *                       zero-based index of the element in the conceptual
    *                       array associated with the construct.
    * @return             - none
    */
   virtual void endElement (const char* name, int index) = 0;

   // Data callback functions.  These are invoked when data contents
   // are parsed from a field.  A separate callback is invoked for
   // each of the ASN.1 primitive data types..

   /**
    * This method is invoked from within a decode function when a value of the
    * BOOLEAN ASN.1 type is parsed.
    *
    * @param value        Parsed value.
    * @return             - none
    */
   virtual void boolValue (OSBOOL value) { OS_UNUSED_ARG(value); }

   /**
    * This method is invoked from within a decode function when a value of the
    * INTEGER ASN.1 type is parsed.
    *
    * @param value        Parsed value.
    * @return             - none
    */
   virtual void intValue (OSINT32 value) { OS_UNUSED_ARG(value); }

   /**
    * This method is invoked from within a decode function when a value of the
    * INTEGER ASN.1 type is parsed.
    *
    * In this case, constraints on the integer value forced the use of unsigned
    * integer C type to represent the value.
    *
    * @param value        Parsed value.
    * @return             - none
    */
   virtual void uIntValue (OSUINT32 value) { OS_UNUSED_ARG(value); }

   /**
    * This method is invoked from within a decode function when a value of the
    * 64-bit INTEGER ASN.1 type is parsed.
    *
    * @param value        Parsed value.
    * @return             - none
    */
   virtual void int64Value (OSINT64 value) {
      intValue ((OSINT32)value);
   }

   /**
    * This method is invoked from within a decode function when a value of the
    * 64-bit INTEGER ASN.1 type is parsed.
    *
    * @param value        Parsed value.
    * @return             - none
    */
   virtual void uInt64Value (OSUINT64 value) {
      uIntValue ((OSUINT32)value);
   }

   /**
    * This method is invoked from within a decode function when a value of the
    * BIT STRING ASN.1 type is parsed.
    *
    * @param numbits      - Number of bits in the parsed value.
    * @param data         - Pointer to a byte array that contains the bit
    *                         string data.
    * @return             - none
    */
   virtual void bitStrValue (OSUINT32 numbits, const OSOCTET* data) {
      OS_UNUSED_ARG(numbits);  OS_UNUSED_ARG(data);
   }

   /**
    * This method is invoked from within a decode function when a value of one
    * of the OCTET STRING ASN.1 type is parsed.
    *
    * @param numocts      Number of octets in the parsed value.
    * @param data         Pointer to byte array containing the octet string
    *                       data.
    * @return             - none
    */
   virtual void octStrValue (OSUINT32 numocts, const OSOCTET* data) {
      OS_UNUSED_ARG(numocts);  OS_UNUSED_ARG(data);
   }

   /**
    * This method is invoked from within a decode function when a value of one
    * of the 8-bit ASN.1 character string types is parsed.
    *
    * @param value        Null terminated character string value.
    * @return             - none
    */
   virtual void charStrValue (const char* value) { OS_UNUSED_ARG(value); }

   /**
    * This method is invoked from within a decode function when a value of a
    * UTF-8 character string type is parsed.
    *
    * @param nchars     Number of characters in the parsed value.
    * @param value      A UTF-8 character string.
    * @return           - none
    */
   virtual void charStrValue (OSUINT32 nchars, const OSUTF8CHAR* value) {
      OS_UNUSED_ARG(nchars);  OS_UNUSED_ARG(value);
   }

   /**
    * This method is invoked from within a decode function when a value of one
    * of the 16-bit ASN.1 character string types is parsed.
    *
    * This is used for the ASN.1 BmpString type.
    *
    * @param nchars       Number of characters in the parsed value.
    * @param data         Pointer to an array containing 16-bit values.
    *                       These are represented using unsigned short integer
    *                       values.
    * @return             - none
    */
   virtual void charStrValue (OSUINT32 nchars, OSUNICHAR* data) {
      OS_UNUSED_ARG(nchars);  OS_UNUSED_ARG(data);
   }

   /**
    * This method is invoked from within a decode function when a value of one
    * of the 32-bit ASN.1 characer string types is parsed.
    *
    * This is used for the ASN.1 UniversalString type.
    *
    * @param nchars       Number of characters in the parsed value.
    * @param data         Pointer to an array containing 32-bit values.
    *                       Each 32-bit integer value is a universal character.
    * @return             - none
    */
   virtual void charStrValue (OSUINT32 nchars, OS32BITCHAR* data) {
      OS_UNUSED_ARG(nchars);  OS_UNUSED_ARG(data);
   }

   /**
    * This method is invoked from within a decode function when a value of the
    * NULL ASN.1 type is parsed.
    *
    * @param             - none
    * @return             - none
    */
   virtual void nullValue () {}

   /**
    * This method is invoked from within a decode function whn a value the
    * OBJECT IDENTIFIER ASN.1 type is parsed.
    *
    * @param numSubIds    Number of subidentifiers in the object identifier.
    * @param pSubIds      Pointer to array containing the subidentifier values.
    * @return             -none
    */
   virtual void oidValue (OSUINT32 numSubIds, OSUINT32* pSubIds) {
      OS_UNUSED_ARG(numSubIds);  OS_UNUSED_ARG(pSubIds);
   }

   /**
    * This method is invoked from within a decode function when a value the
    * REAL ASN.1 type is parsed.
    *
    * @param value        Parsed value.
    * @return             - none
    */
   virtual void realValue (double value) { OS_UNUSED_ARG(value); }

   /**
    * This method is invoked from within a decode function when a value of the
    * ENUMERATED ASN.1 type is parsed.
    *
    * @param value        - Parsed enumerated value
    * @param text         - Textual value of enumerated identifier
    * @return             - none
    */
   virtual void enumValue (OSUINT32 value, const OSUTF8CHAR* text) {
      OS_UNUSED_ARG(value);  OS_UNUSED_ARG(text);
   }

   /**
    * This value is invoked from within a decode function when an ASN.1 open
    * type is parsed.
    *
    * @param numocts      Number of octets in the parsed value.
    * @param data         Pointer to byet array contain in tencoded ASN.1
    *                       value.
    * @return             - none
    */
   virtual void openTypeValue (OSUINT32 numocts, const OSOCTET* data) {
      OS_UNUSED_ARG(numocts);  OS_UNUSED_ARG(data);
   }

   // The following static function is called to add a new event handler
   // to the context event handler list..

   /**
    * This method is called to add a new event handler to the context event
    * handler list.  It is a static method.
    *
    * @param   pCtxt    A pointer-to an ::OSCTXT data structure.
    * @param   pHandler A pointer to an ::Asn1NamedEventHandler.
    */
   EXTRTMETHOD static void addEventHandler
      (OSCTXT* pCtxt, Asn1NamedEventHandler* pHandler);

   // The following static function is called to remove an event handler
   // from the context event handler list.  Note that it does not delete
   // the event handler object.

   /**
    * This method removes the given event handler from the context event
    * handler list.  This method does not delete the handler, so memory
    * allocated for it will need to be released elsewhere.
    *
    * @param   pCtxt    A pointer-to an ::OSCTXT data structure.
    * @param   pHandler A pointer to an ::Asn1NamedEventHandler.
    */
   EXTRTMETHOD static void removeEventHandler
      (OSCTXT* pCtxt, Asn1NamedEventHandler* pHandler);

   // The following static methods are invoked from within the generated
   // code to call the various user-defined event handler methods ..

   /**
    * This method is called by generated code to invoke the event handlers'
    * start element methods.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   name        The name of the element.
    * @param   index       The index of the element, if it is in a SEQUENCE
    *                         or SET OF type.
    */
   EXTRTMETHOD static void invokeStartElement
      (OSCTXT* pCtxt, const char* name, int index);

   /**
    * This method is called by generated code to invoke the event handlers'
    * end element methods.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   name        The name of the element.
    * @param   index       The index of the element, if it is in a SEQUENCE
    *                         or SET OF type.
    */
   EXTRTMETHOD static void invokeEndElement
      (OSCTXT* pCtxt, const char* name, int index);

   /**
    * This method is called by generated code to invoke the event handlers'
    * boolean method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   value       A decoded boolean value.
    */
   EXTRTMETHOD static void invokeBoolValue (OSCTXT* pCtxt, OSBOOL value);

   /**
    * This method is called by generated code to invoke the event handlers'
    * integer method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   value       A decoded 32-bit integer value.
    */
   EXTRTMETHOD static void invokeIntValue (OSCTXT* pCtxt, OSINT32 value);

   /**
    * This method is called by generated code to invoke the event handlers'
    * unsigned integer method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   value       A decoded unsigned 32-bit integer value.
    */
   EXTRTMETHOD static void invokeUIntValue (OSCTXT* pCtxt, OSUINT32 value);

   /**
    * This method is called by generated code to invoke the event handlers'
    * 64-bit integer method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   value       A decoded 64-bit integer value.
    */
   EXTRTMETHOD static void invokeInt64Value (OSCTXT* pCtxt, OSINT64 value);

   /**
    * This method is called by generated code to invoke the event handlers'
    * unsigned 64-bit integer method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   value       A decoded 64-bit integer value.
    */
   EXTRTMETHOD static void invokeUInt64Value (OSCTXT* pCtxt, OSUINT64 value);

   /**
    * This method is called by generated code to invoke the event handlers'
    * bit string method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   numbits     The number of bits in the decoded bit string.
    * @param   data        The decoded bit string data.
    */
   EXTRTMETHOD static void invokeBitStrValue
      (OSCTXT* pCtxt, OSUINT32 numbits, const OSOCTET* data);

   /**
    * This method is called by generated code to invoke the event handlers'
    * octet string method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   numocts     The number of octets in the decoded octet string.
    * @param   data        The decoded octet string data.
    */
   EXTRTMETHOD static void invokeOctStrValue
      (OSCTXT* pCtxt, OSUINT32 numocts, const OSOCTET* data);

   /**
    * This method is called by generated code to invoke the event handlers'
    * character string method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   value       The decoded character string.
    */
   EXTRTMETHOD static void invokeCharStrValue
      (OSCTXT* pCtxt, const char* value);

   /**
    * This method is called by generated code to invoke the event handlers'
    * 16-bit character string method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   nchars      The number of characters in the string.
    * @param   value       The decoded 16-bit character string.
    */
   EXTRTMETHOD static void invokeCharStrValue
      (OSCTXT* pCtxt, OSUINT32 nchars, OSUNICHAR* data);

   /**
    * This method is called by generated code to invoke the event handlers'
    * 32-bit character string method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   nchars      The number of characters in the string.
    * @param   value       The decoded character string.
    */
   EXTRTMETHOD static void invokeCharStrValue
      (OSCTXT* pCtxt, OSUINT32 nchars, OS32BITCHAR* data);

   /**
    * This method is called by generated code to invoke the event handlers'
    * UTF-8 character string method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   nchars      The number of characters in the string.
    * @param   value       The decoded character string.
    */
   EXTRTMETHOD static void invokeCharStrValue
      (OSCTXT* pCtxt, OSUINT32 nchars, const OSUTF8CHAR* data);

   /**
    * This method is called by generated code to invoke the event handlers'
    * null method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    */
   EXTRTMETHOD static void invokeNullValue (OSCTXT* pCtxt);

   /**
    * This method is called by generated code to invoke the event handlers'
    * object identifier method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   numSubIds   The number of OID subids.
    * @param   value       The decoded OID ids.
    */
   EXTRTMETHOD static void invokeOidValue
      (OSCTXT* pCtxt, OSUINT32 numSubIds, OSUINT32* pSubIds);

   /**
    * This method is called by generated code to invoke the event handlers'
    * real method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   value       A decoded real value.
    */
   EXTRTMETHOD static void invokeRealValue (OSCTXT* pCtxt, double value);

   /**
    * This method is called by generated code to invoke the event handlers'
    * enumerated method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   value       A decoded 64-bit integer value.
    * @param   text        The character string representation of the value.
    */
   EXTRTMETHOD static void invokeEnumValue
      (OSCTXT* pCtxt, OSUINT32 value, const OSUTF8CHAR* text);

   /**
    * This method is called by generated code to invoke the event handlers'
    * open type method.  It is static.
    *
    * @param   pCtxt       A pointer to an ::OSCTXT data structure.
    * @param   numocts     The number of octets in the open type.
    * @param   data        The data octets that comprise the open type value.
    */
   EXTRTMETHOD static void invokeOpenTypeValue
      (OSCTXT* pCtxt, OSUINT32 numocts, const OSOCTET* data);

} ;

// Null event handler class.  This class contains an empty implementation
// of all user methods..

/**
 * The Asn1NullEventHandler contains a completely empty implementation of all
 * user methods.
 */
class EXTRTCLASS Asn1NullEventHandler : public Asn1NamedEventHandler {
 public:
   /** This startElement method does nothing. */
   virtual void startElement (const char* , int ) {}
   /** This endElement method does nothing. */
   virtual void endElement (const char* , int ) {}
} ;

// The following pure abstract class defines the error handler interface
// callback functions that are invoked when a parsing error occurs..

/** This definition is a forward reference to the ASN1MessageBuffer class. */
class EXTRTCLASS ASN1MessageBuffer ;

/**
 * Error handler base class.  This is the base class from which
 * user-defined error classes are derived.  These classes can be
 * used to provide fault-tolerance when parsing a message.  The normal
 * decoder behavior is to stop decoding when it encounters an error.
 * An error handler can be used to ignore or take corrective action
 * that will allow the decoding process to continue.
 */
class EXTRTCLASS Asn1ErrorHandler {
 public:
   Asn1ErrorHandler() {}
   virtual ~Asn1ErrorHandler() {}

   /**
    * The error handler callback method.  This is the method that the user
    * must override to provide customized error handling.
    *
    * @param pCtxt - Pointer to a context block structure.
    * @param pCCB - Pointer to a context control block structure.
    * @param stat - The error status that caused the handler to be invoked.
    * @return - Corrected status.  Set to 0 to cause decoding to
    *   continue or to a negative status code (most likely \c stat) to
    *   cause decoding to terminate.
    */
   virtual int error (OSCTXT* pCtxt, ASN1CCB* pCCB, int stat) = 0;

   // The following static method is called from within the generated
   // code to call the virtual error callback method..

   EXTRTMETHOD static int invoke (OSCTXT* pCtxt, ASN1CCB* pCCB, int stat);

   // The following static method is called from within the generated
   // code to call the virtual error callback method..

   EXTRTMETHOD static int invoke
      (OSCTXT* pCtxt, OSOCTET* ptr, int len, int stat);

   /**
    * This static method is called to set the error handler
    * within the context structure.  Note that unlike event handlers, only
    * a single error handling object can be specified.  This must be
    * called by the user to specify the error handling object prior to
    * execution of the main decode function..
    *
    * @param pCtxt - Pointer to a context block structure.
    * @param pHandler - Pointer to error handler object to register.
    */
   EXTRTMETHOD static void setErrorHandler
      (OSCTXT* pCtxt, Asn1ErrorHandler* pHandler);

} ;
/**
 * @}Asn1NamedEventHandler
 */
#endif
