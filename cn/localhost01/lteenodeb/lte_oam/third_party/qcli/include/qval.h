/*******************************************************************************
* File Name   : qval.h
*
* Description : -- QCLI Toolkit
* Revision History: --
*
* Date        Author           Reference          Comments
* -----        ------           ---------          --------
*  2006        Avijit Ghosh       -              Initial Creation
*
* Copyright (c) 2006, Aricent Group.
*******************************************************************************/
#ifndef	__QVAL_H__
#define	__QVAL_H__

extern const char __qnam_INTEGER[];
extern char *__qval_INTEGER( const char *, const char * );

extern const char __qnam_FLOATINGPOINT[];
extern char *__qval_FLOATINGPOINT( const char *, const char * );

extern const char __qnam_IDENTIFIER[];
extern char *__qval_IDENTIFIER( const char *, const char * );

extern const char __qnam_CHOICE[];
extern char *__qval_CHOICE( const char *, const char * );

extern const char __qnam_DATE[];
extern char *__qval_DATE( const char *, const char * );

extern const char __qnam_TIME[];
extern char *__qval_TIME( const char *, const char * );

extern const char __qnam_STRING[];
extern char *__qval_STRING( const char *, const char * );

extern const char __qnam_IP4ADDRESS[];
extern char *__qval_IP4ADDRESS( const char *, const char * );

extern const char __qnam_IP4NETWORK[];
extern char *__qval_IP4NETWORK( const char *, const char * );

extern const char __qnam_IP6ADDRESS[];
extern char *__qval_IP6ADDRESS( const char *, const char * );

extern const char __qnam_MACADDRESS[];
extern char *__qval_MACADDRESS( const char *, const char * );

extern const char __qnam_DOMAINNAME[];
extern char *__qval_DOMAINNAME( const char *, const char * );

extern const char __qnam_OID[];
extern char *__qval_OID( const char *, const char * );

extern const char __qnam_HEXBUF[];
extern char *__qval_HEXBUF( const char *, const char * );

#define	qsvName(name) \
		(__qnam_ ## name)

#define	qsvFunction(name) \
		(__qval_ ## name)

#define	qsvValidate(type,value,range) \
		(__qval_ ## type)((value), (range))

#endif
