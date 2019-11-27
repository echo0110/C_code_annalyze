/*******************************************************************************
*
* FUNCTION : Pragma defines for the HSS Gateway.
*
********************************************************************************
*
* FILENAME : gw_pragma_defs.h
*
* DESCRIPTION : Contains global pragma defines for byte alignment in structures.
*               The idea is to include gw_pragma_defs.h file in the beginning of
*               a header file that contains structure declarations and include
*               gw_pragma_undefs.h at the end of it. This is to ensure that
*               these pragma defines are effective in only this header file.
*
*
* DATE       NAME           REFERENCE         REASON
* ---------- -------------- ----------------- ---------------------------------
*
* 19/1/2001  Sachin Mittal                    Initial
*
* 15/04/2002 Ravi Jindal        SPR-3928      To add Linux OS Port.
*
*
* Copyright 2001, Hughes Software Systems Ltd.
*
*******************************************************************************/

/* Because of the idea explained above the conditional inclusion starts in
gw_pragma_defs.h and ends in gw_pragma_undefs.h */

#ifndef _GW_PRAGMA_H_
#define _GW_PRAGMA_H_

#ifdef GW_WINNT
/* MSVC++ version 6.0 is the development environment */
/* 4103 is an informational warning coming due to use of pragma definintion
in mg defs. The warining is 4103 : used #pragma pack to change alignment */
#pragma warning (disable : 4103)
#pragma pack(push, gw_defs, 4)
#endif

#ifdef GW_VXWORKS
#pragma align 2
#endif

#if defined(GW_SOLARIS) || defined(GW_LINUX)
#pragma pack(4)
#endif

#endif  /* end of _GW_PRAGMA_H_ */
