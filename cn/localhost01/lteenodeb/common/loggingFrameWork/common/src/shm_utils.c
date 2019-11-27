/****************************************************************************
 *
 *  ARICENT -
 *
 *  Copyright (c) 2011 Aricent.
 *
 ****************************************************************************
 * File Details
 * ------------
 *  $Id: logging.c,v  2011/01/27 06.00:00 gur19170 Exp $
 ****************************************************************************
 *
 *  File Description :
 *  This file contains implementation of logging framework
 *
 ****************************************************************************
 *
 * Revision Details
 * ----------------
 *
 * $Log: logging.c,v $
 * Initial version
 *
 ****************************************************************************/


/****************************************************************************
 * Standard Library Includes
 ****************************************************************************/
#include <sys/time.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h> 
#include <sys/timeb.h>
#include <signal.h>
#include <stdarg.h>
#include <stdio.h> 
#include <stdlib.h> 
#include <unistd.h>
#include <string.h>
#include <pthread.h>

/****************************************************************************
 * Project Includes
 ****************************************************************************/
#include "shm_utils.h"
/****************************************************************************
  Private Definitions
 ****************************************************************************/

/****************************************************************************
  Private Types
 ****************************************************************************/

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

/*****************************************************************************
 * Private Constants
 ****************************************************************************/

/****************************************************************************
 * Exported Variables
 ****************************************************************************/
unsigned int is_shm_init = 0;


/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/
               
/*****************************************************************************
 * Function Name  : shm_init
 * Inputs         : ModuleId 
 * Outputs        : 
 * Returns        : 0 - On success.
 *                  1 - On failure.
 * Description    : A thread which requires it's logs to be captured should 
 *                  register by calling this function.
 *****************************************************************************/

void* shm_init(char* file_name, unsigned file_size)
{
  int shm_fd;
  void *p_shmptr = NULL;
  mode_t omask;
  
  /*Creating shared memory object          ----shm_open()*/
  omask = umask(0);
  shm_fd = shm_open(file_name, O_CREAT | O_RDWR | O_EXCL, (S_IRWXU|S_IRWXG|S_IRWXO));
  umask(omask);
  if(shm_fd < 0)
  {
      if (errno == EEXIST)
      {
          omask = umask(0);
          shm_fd = shm_open(file_name, O_RDWR , (S_IRWXU|S_IRWXG|S_IRWXO));
          umask(omask);
          if(shm_fd < 0)
          {
              PERROR("In shm_open :");
              /* SPR 4010 Fix start */
              return NULL;
              /* SPR 4010 Fix End */
          }
          is_shm_init = 1;

      }
      else
      {
          PERROR("In shm_open :");
          /* SPR 4010 Fix start */
          return NULL;
	  /* SPR 4010 Fix End */
      }
  }

  /*
   * In case log file size is modified from last creation
   * from adjusting mapped file size
   */
  if(ftruncate(shm_fd,file_size) == -1)
  {
      shm_unlink(file_name);
      PERROR("In ftruncate()");
      /* SPR 4010 Fix start */
      return NULL;
      /* SPR 4010 Fix End */
  }

  /*Requesting the shared segment*/ 
  p_shmptr =  mmap(NULL, file_size, PROT_READ|PROT_WRITE,
          MAP_SHARED, shm_fd, 0);

  if(MAP_FAILED == p_shmptr)
  {
      shm_unlink(file_name);
      PERROR("In mmap():");
      /* SPR 4010 Fix start */
      return NULL;
      /* SPR 4010 Fix End */
  }

  return(p_shmptr);
} 
