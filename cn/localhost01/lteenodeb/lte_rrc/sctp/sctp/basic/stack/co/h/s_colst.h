/***************************************************************************
**  FUNCTION:
**     List manupulation related functinsyy
**
****************************************************************************
**
**  FILE NAME:
**  s_colst.h
**
**  DESCRIPTION:
**     List manupulation related functinsyy
**
**  DATE     NAME          REF#    REASON
**  -------  ----------------  -----   --------------------------------
**  23Mar00  sigtran team          Original
**  15OCt00  Harsh Bhondwe     VXWORKS porting for vxworks
**
**
**   Copyright (C) 2006 Aricent Inc . All Rights Reserved
***************************************************************************/
#ifndef __FILE_sctp_list_SEEN__     /* If this file has not been processed, */
#define __FILE_sctp_list_SEEN__     /* define it as seen--ref '#endif' below. */

#include <s_os.h>

#if defined (VXWORKS) || defined (VX_PNE)
/* These are OS library routines ..so redefine */
#define NODE SCTP_NODE
#define node sctp_node
#define lstInit sctp_lstInit
#define LIST SCTP_LIST
#define lstAdd sctp_lstAdd
#define lstConcat sctp_lstConcat
#define lstCount sctp_lstCount
#define lstDelete sctp_lstDelete
#define lstExtract sctp_lstExtract
#define lstFirst sctp_lstFirst
#define lstGet  sctp_lstGet
#define lstInsert sctp_lstInsert
#define lstLast sctp_lstLast
#define lstNext sctp_lstNext
#define lstNth  sctp_lstNth
#define lstPrevious sctp_lstPrevious
#define lstNStep sctp_lstNStep
#define lstFind sctp_lstFind
#define lstFree sctp_lstFree
#endif

typedef struct node
{
    struct node *next;    /* Points at the next node in the list */
    struct node *previous;    /* Points at the previous node in the list */
} NODE;


typedef struct         /* Header for a linked list. */
{
    NODE node;        /* Header list node */
    int count;        /* Number of nodes in list */
} LIST;

extern      void       lstInit( LIST *pList );
extern      void       lstAdd( LIST *pList, NODE *pNode );
extern      void       lstConcat( LIST *pDstList, LIST *pAddList );
extern      int        lstCount( LIST *pList );
extern      void       lstDelete( LIST *pList, NODE *pNode );
extern      void       lstExtract( LIST *pSrcList, NODE *pStartNode,
                                   NODE *pEndNode, LIST *pDstList );
extern      NODE      *lstFirst( LIST *pList );
extern      NODE      *lstGet( LIST *pList );
extern      void       lstInsert( LIST *pList, NODE *pPrev, NODE *pNode );
extern      NODE      *lstLast( LIST *pList );
extern      NODE      *lstNext( NODE *pNode );
extern      NODE      *lstNth( LIST *pList, int nodenum );
extern      NODE      *lstPrevious( NODE *pNode );
extern      NODE      *lstNStep( NODE *pNode, int nStep );
extern      int        lstFind( LIST *pList, NODE *pNode );
extern      void       lstFree( LIST *pList );

#endif /* __FILE_sctp_list_SEEN__   -- This MUST appear after all code! */

