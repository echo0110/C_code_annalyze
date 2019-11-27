/***************************************************************************
**  FUNCTION:
**    Association control resides here
**
****************************************************************************
**
**  FILE NAME:
**    s_comd5.h
**
**  DESCRIPTION:
**    contains:
**
**  DATE     NAME           REF#   REASON
**  -------  ----------------  -----  -------------------------------------
**  22Mar00  Sigtran Team      -----  Original
**
**   Copyright (C) 2006 Aricent Inc . All Rights Reserved
***************************************************************************/
#ifndef _S_CO_MD5_H_SEEN_
#define _S_CO_MD5_H_SEEN_

#ifdef __cplusplus
extern "C" {
#endif

    /***************************************************************************
     *    FUNCTION :    md5digest
     *
     *    DESCRIPTION:
     *      md5digest is an external porting function required to be defined
     *  along with customer application code.
     *  This Function takes a Buffer pointer ("buffer") of size "length")
     *  as input and outputs a 16 bytes Digest key for the buffer in
     *  "result". The digest key should such that whenever same buffer is
     *  passed as input the resultant digest key remains same.
     *
     *  You may follow below mentioned guidelines to define this function.
     *  1)  Download rfc 1321 from following link:
     *      http://www.ietf.org/rfc/rfc1321.txt
     *  2)  In this rfc [APPENDIX A - Reference Implementation] you
     *      can find the source code for a Reference Implemenetation
     *      of MD5 digest generation algorithm. Please create source-files
     *      global.h,md5.h and md5c.c.
     *  3)  Please add following function definition at the end of
     *      md5c.c.
     *      ========================
     *      void md5digest( unsigned char *buffer,
     *                      unsigned long length,
     *                      unsigned char *result );
     *      void md5digest( unsigned char *buffer,
     *                      unsigned long length,
     *                      unsigned char *result )
     *      {
     *          MD5_CTX    context;
     *          MD5Init( &context );
     *          MD5Update( &context, buffer, length );
     *          MD5Final( result, &context );
     *      }
     *      ========================
     *  4)  Instead of the above mentioned steps you can also use the attached
     *           package MD5_PACKAGE_FROM_RFC_1321.tar.gz  This package includes the sample
     *           source-files global.h, md5.h and md5c.c created through above mentioned
     *           steps.
     *      5)       Prepare the object-file for md5c.c. This can be done using
     *         following command:
     *      gcc -Wall -I[path of header-files global.h and md5.h] -c md5c.c
     *      This command would create an object-file md5c.o. Now you
     *      need to link it with the stack application along with sctp.o.
     *
     *    RETURN VALUE:
     *    None.
     *
     **************************************************************************/
    extern void
    md5digest(
        unsigned char *buffer,
        unsigned long length,
        unsigned char *result );

#ifdef __cplusplus
}
#endif

#endif /* _S_CO_MD5_H_SEEN_ */


