/****************************************************************************/
/**  Copyright (C) 2006 Aricent Inc . All Rights Reserved */
/****************************************************************************/
/**                                                                        **/
/** Architecture dependent atomic operations interfaces                    **/
/**                                                                        **/
/****************************************************************************/

#ifndef __ATOMIC_OPS_H__
#define __ATOMIC_OPS_H__

#ifdef __PPC__

/* Start of change for VARIBLE Declaration */
enum
{
    ATOMIC_FAILURE,
    ATOMIC_SUCCESS
} AtmoicResult;
/*End  of change for VARIBLE Declaration */

#define lwarx(dest, src) \
({__asm__ volatile("lwarx %0,%y1" : "=r" (dest):"Z" (src));})

#define stwcx(dest, newVal) \
({__asm__ volatile ("stwcx. %1,%y0\n" \
                   :  "=Z" (*dest) \
                   :  "r" (newVal) \
                   : "cr0");})
#endif
/* SPR 4283 Fix start */
#ifdef __arm__
#define ldrex(dest, src) \
({__asm__ volatile("ldrex %0,[%1]" : "=&r" (dest):"r" (src):"cc", "memory");})

#define strex(dest, newVal, error) \
( \
{ __asm__ volatile ("strex %0,%1,[%2]\n" \
                   :  "=&r" (error) \
                   :  "r" (newVal), "r" (dest) \
                   : "cc", "memory");})
#endif
/* SPR 4283 Fix End */

int atomicCompareAndSwapInt(unsigned int *ptr, unsigned int origVal,
                         unsigned int newVal);
int atomicCompareAndSwapPtr(void **ptr, void *origVal,
                         void *newVal);
void atomicIncrement(unsigned int *ptr);
void atomicDecrement(unsigned int *ptr);
void atomicAddInt(unsigned int *orig, unsigned int addedVal);
void atomicSubtractInt(unsigned int *orig, unsigned int subtractedVal);

#endif /* __ATOMIC_OPS_H__ */

