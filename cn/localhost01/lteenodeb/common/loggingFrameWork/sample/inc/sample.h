/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: sample.h,v  2011/01/27 06:00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This is sample header file contains log area and sample stack
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: sample.h,v $
 * Initial version
 *
 ****************************************************************************/
#ifndef SAMPLE_H 
#define SAMPLE_H

/*Sample Log area */
enum
{
    PDSCH_LA,
    PUSCH_LA
};



/* Sample Stack*/
enum
{
    PDCP_STACK,
    RLC_STACK,
    MAC_STACK

};

enum
{
    UE_ASSOC,
    UE_NONASSOC,
    S1AP_MAX_LOGAREA
};

enum
{
    S1AP_STACK,
    RRC_STACK,
    L3_MAX_STACK
};

enum
{
    OAMH = 0,
    CSC,
    LLIM,
    UECC,
    RRC_MAX_LOGAREA
};


#endif
