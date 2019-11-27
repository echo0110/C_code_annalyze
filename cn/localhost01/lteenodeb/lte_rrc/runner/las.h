/***********************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2009 Aricent.
 *
 **********************************************************************/
#ifndef LAS_H
#define LAS_H

extern char *md5digestfile( const char *filename );
extern char *rsa( const char *sX, const char *sE, const char *sM );
extern char *readkey( const char *keyfile );
extern int authorize( const char *file, const char *keyfile, const char *key );
extern char *license( const char *file, const char *key );

#endif
