/******************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (C) 2009 Aricent Inc. All Rights Reserved.
 *
 ******************************************************************************
 *
 *  $Id: rohc_crc_basic.c,v 1.1.1.1 2010/02/11 04:51:26 cm_intel Exp $
 *
 ******************************************************************************
 *
 *  File Description : This file contains the functions for Basic CRC Computation.
 *
 ******************************************************************************
 *
 * Revision Details
 * ----------------
 * $Log: rohc_crc_basic.c,v $
 * Revision 1.1.1.1  2010/02/11 04:51:26  cm_intel
 * eNB framework for intel
 *
 * Revision 1.3.2.1  2009/06/09 11:34:56  gur19140
 * first wave of Integration
 *
 * Revision 1.3  2009/05/27 13:50:15  gur19836
 * File Header and Function Headers updated
 *
 *
 *****************************************************************************/


/******************************************************************************
 * Standard Library Includes
 *****************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/
#include "rohc_crc_basic.h"

/******************************************************************************
  Private Definitions
 *****************************************************************************/

/******************************************************************************
  Private Types
 *****************************************************************************/

/*****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/******************************************************************************
 * Private Constants
 *****************************************************************************/

/******************************************************************************
 * Exported Variables
 *****************************************************************************/

/*****************************************************************************
 * Private Variables (Must be declared static)
 *****************************************************************************/
static rohc_U8bit_t rohc_crc_table_8[256];
static rohc_U8bit_t rohc_crc_table_7[256];
static rohc_U8bit_t rohc_crc_table_3[256];





/*****************************************************************************
 * Function Name  : crc_get_polynom
 * Inputs         : type - CRC type
 * Outputs        : None
 * Returns        : the polynom for the requested type
 * Description    : Get the polynom for the crc type
 *****************************************************************************/
rohc_U8bit_t crc_get_polynom(rohc_U8bit_t type)
{
	if(type == ROHC_CRC_TYPE_3) return 0x6;
	if(type == ROHC_CRC_TYPE_7) return 0x79;
	if(type == ROHC_CRC_TYPE_8) return 0xe0;
	return 0;
}

/*****************************************************************************
 * Function Name  : crc_get_polynom
 * Inputs         : table - 256 bytes table
 *                  poly - polynom to use
 * Outputs        : None
 * Returns        : None
 * Description    : Initialize a table given a 256 bytes table and the 
 *                  polynom to use
 *****************************************************************************/
void rohc_crc_init_table(rohc_U8bit_t *table, rohc_U8bit_t poly) {
	rohc_U8bit_t crc;
	rohc_U32bit_t i,j;

	for (i=0; i<256; i++) {
		crc = i;
		for (j=0; j<8; j++) {
			if (crc & 1)
				crc = (crc>>1) ^ poly;
			else
				crc = (crc>>1);
		}
		table[i] = crc;
	}
}

/*****************************************************************************
 * Function Name  : rohc_crc_calc_8
 * Inputs         : buf - Buffer
 *                  size 
 *                  crc_init
 * Outputs        : None
 * Returns        : Optimized CRC-8
 * Description    : Optimized CRC-8 calculation using the table
 *****************************************************************************/
rohc_U8bit_t rohc_crc_calc_8(rohc_U8bit_t *buf, rohc_U32bit_t size, rohc_U32bit_t crc_init) {
	rohc_U32bit_t i;
	rohc_U8bit_t crc = 0xff & crc_init;
	for (i=0; i<size; i++) {
		crc = rohc_crc_table_8[buf[i]^crc];

	}
	return crc;
}

/*****************************************************************************
 * Function Name  : rohc_crc_calc_7
 * Inputs         : buf - Buffer
 *                  size 
 *                  crc_init
 * Outputs        : None
 * Returns        : Optimized CRC-7
 * Description    : Optimized CRC-7 calculation using the table
 *****************************************************************************/
rohc_U8bit_t rohc_crc_calc_7(rohc_U8bit_t *buf, rohc_U32bit_t size, rohc_U32bit_t crc_init) {
	rohc_U32bit_t i;
/* SPR5816 Fix Start */
	rohc_U8bit_t crc = crc_init;
/* SPR5816 Fix End */
	for (i=0; i<size; i++) {

		crc = rohc_crc_table_7[buf[i]^(crc&127)];

	}
	return crc;
}

/*****************************************************************************
 * Function Name  : rohc_crc_calc_3
 * Inputs         : buf - Buffer
 *                  size 
 *                  crc_init
 * Outputs        : None
 * Returns        : Optimized CRC-3
 * Description    : Optimized CRC-3 calculation using the table
 *****************************************************************************/
rohc_U8bit_t rohc_crc_calc_3(rohc_U8bit_t *buf, rohc_U32bit_t size, rohc_U32bit_t crc_init) {
	rohc_U32bit_t i;
/* SPR5816 Fix Start */
	rohc_U8bit_t crc = crc_init;
/* SPR5816 Fix End */
	for (i=0; i<size; i++) {
		crc = rohc_crc_table_3[buf[i]^(crc&7)];
	}
	return crc;
}

/*****************************************************************************
 * Function Name  : rohc_compute_crc
 * Inputs         : type - the CRC type (CRC_TYPE_3, CRC_TYPE_7 or CRC_TYPE_8)
 *                  data - data to calculate the checksum on
 *                  length - Length of data
 *                  crc_init - 
 * Outputs        : None
 * Returns        : the checksum
 * Description    : Calculates the checksym for the given data
 *****************************************************************************/
rohc_U8bit_t rohc_compute_crc ROHC_PARAMS((type,data,length,crc_init),
		rohc_U8bit_t type _AND_ 
		rohc_U8bit_t *data _AND_ 
		rohc_U32bit_t length _AND_ 
		rohc_U8bit_t crc_init) 
{
	switch (type) {
		case ROHC_CRC_TYPE_8: return rohc_crc_calc_8(data, length, crc_init);
		case ROHC_CRC_TYPE_7: return rohc_crc_calc_7(data, length, crc_init);
		case ROHC_CRC_TYPE_3: return rohc_crc_calc_3(data, length, crc_init);
	}
	return 0;
}
/*****************************************************************************
 * Function Name  : init_crc
 * Inputs         : None
 * Outputs        : None
 * Returns        : None
 * Description    : Initializes CRC tables
 *****************************************************************************/
void init_crc()
{
	rohc_crc_init_table(rohc_crc_table_3, crc_get_polynom(ROHC_CRC_TYPE_3));
	rohc_crc_init_table(rohc_crc_table_7, crc_get_polynom(ROHC_CRC_TYPE_7));
	rohc_crc_init_table(rohc_crc_table_8, crc_get_polynom(ROHC_CRC_TYPE_8));
}        
