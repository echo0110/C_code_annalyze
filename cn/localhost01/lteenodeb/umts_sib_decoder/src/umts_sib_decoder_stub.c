#include "umtsSibParsing.h"
#include "umts_sib_type_defines.h"
#include "umts_sib_decoder_intf.h"
#include "rtsrc/rtPrint.h"
#include "rtxsrc/rtxCommon.h"
int main(int argc, char *argv[])
{

    FILE            *fp=NULL;
    U16             noOfBytes=0,index=0; 
    S32             tempVal;
    OSOCTET         buffer[1000];
    return_code  ret_val  = SUCCESS;
    U32  msgType;

    fp=fopen(argv[1],"r");
    if ( fp == NULL)
    {
        printf("Error opening file");
        exit(0);
    }
    while(fscanf(fp,"%d",&tempVal)!=EOF)
    {
        buffer[noOfBytes++]=tempVal;
    }

    fclose(fp);
    while(index<noOfBytes)
    {
        printf("%d ",buffer[index++]);
    }
    printf("N==%d",noOfBytes);
    printf("CHOICE                MSGTYPE");
    printf("\n0                   BCCH_BCH_MSG");
    printf("\n1                   BCCH_DL_MSG");
    printf("\nENTER MSG CHOCE:\n");
    scanf("%d",&msgType);
    return 0;
}

