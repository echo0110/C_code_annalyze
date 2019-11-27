/****************************************************************************/
/**  Copyright (C) 2012 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Common Stack Porting Library - Interface Definition                    **/
/**                                                                        **/
/****************************************************************************/

#ifndef __HSS_SHMPOOL_H__
#define __HSS_SHMPOOL_H__


#include <stdint.h>

intptr_t qvGetRelativeAddr(void *shmAddr, void *buffer);

void *qvGetPointer(void *shmAddr, intptr_t offset);

int qvShmPoolGetSize(unsigned int bufSize, unsigned int nbuf);

int qvShmCreatePool(void *shmAddr, unsigned int shmSize,
	 unsigned int poolOffset, unsigned int size,
	  unsigned int nbuf);

void *qvShmPoolAlloc(void *shmAddr, void *pool);

void qvShmPoolFree(void *shmAddr, void *buffer);

#endif /* __HSS_MSGPOOL_H__ */
