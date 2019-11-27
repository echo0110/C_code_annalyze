/***********************************************************
**  FILE NAME:
**       egtpu_list.h
**       
**       
*************************************************************
**       
**  DESCRIPTION:
**      This file contains data structures and function
**      prototypes for E-GTPU stack's List Data structure.
**       
**      DATE       NAME          REFERENCE    REASON
**      ----       ----          ---------    ------
**
**
**  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
************************************************************/

#ifndef __EGTPU_LIST_DECL__
#define __EGTPU_LIST_DECL__

typedef struct __xlist__ XLIST;

struct __xlist__ {
    XLIST *next, *prev;
};

egtpu_void_t    xlInit (XLIST *);
egtpu_boolean_t xlEmpty (XLIST *);
egtpu_void_t    xlInsert (XLIST *, XLIST *, XLIST *);
egtpu_void_t    xlDelete (XLIST *);
egtpu_void_t    xlPushHead (XLIST *, XLIST *);
egtpu_void_t    xlPushTail (XLIST *, XLIST *);
XLIST          *xlPopHead (XLIST *);
XLIST          *xlPopTail (XLIST *);

#endif
