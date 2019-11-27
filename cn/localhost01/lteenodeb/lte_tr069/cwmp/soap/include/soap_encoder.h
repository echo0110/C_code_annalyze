#include "soapH.h"

#define SOAP_ENCODE_DECODE "Soap_Encode_Decode"

int soap_decode(char *xml, int *type, void **resp,struct SOAP_ENV__Header **header );
char *soap_encode (void *msg, int methodType , struct SOAP_ENV__Header *header);

