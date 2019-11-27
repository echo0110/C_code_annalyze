/* Based on Public domain C from Philip J. Erdelsky */
/* www.efgh.com/software/rijndael.htm */

#ifndef _RIJNDAEL_H
#define _RIJNDAEL_H

#include "in_lteFramework.h"

/* for AES-128 */
#define RIJNDAEL_KEYBITS 128

#define RIJNDAEL_KEYLENGTH(keybits) ((keybits)/8)
#define RIJNDAEL_RKLENGTH(keybits)  ((keybits)/8+28)
#define RIJNDAEL_NROUNDS(keybits)   ((keybits)/32+6)

typedef struct
{
    UInt32	rk[RIJNDAEL_RKLENGTH(RIJNDAEL_KEYBITS)];
} rijndael_state;

int rijndaelSetupEncrypt(rijndael_state *rk, UInt8 *key, Int keybits);
int rijndaelSetupDecrypt(rijndael_state *rk, UInt8 *key, Int keybits);
void rijndaelEncrypt(rijndael_state *rk, Int nrounds, UInt8 *plaintext, UInt8 *ciphertext);
void rijndaelDecrypt(rijndael_state *rk, Int nrounds, UInt8 *ciphertext, UInt8 *plaintext);

#endif
