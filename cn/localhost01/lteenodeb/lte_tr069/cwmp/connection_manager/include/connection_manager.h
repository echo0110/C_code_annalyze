
#include <stdio.h>
#include <stdlib.h>
//#include <common_def.h>


#define CPE_FILE_DIR_NAME  "/cpe_files/"
#define DEFAULT_INP_SIZE 255 

/*SPR 17635 FIxed Start*/
typedef enum {
HTTP=0 ,
FTP,
SFTP,
HTTPS
}eProtocolType;
/*SPR 17635 FIxed End*/

struct FtpFile {
  const char *filename;
  FILE *stream;
};


