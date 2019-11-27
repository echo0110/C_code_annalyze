/**************************************************************************************
*
*  FILE NAME  : simuScheduler.c
*
*  DESCRIPTION: External scheduler implementation.
*
*
*   DATE    NAME    REFERENCE   REASON
*
*  Copyright 2009, Aricent Inc. Ukraine
*
*************************************************************************************/

/****************************************************************************
 * Includes
 ****************************************************************************/

#include <pthread.h>
#include <semaphore.h>
#include "simLogger.h"

#include "simuScheduler.h"
#include "sock_tools.h"
#include "userUtils.h"
#include "callbacksSockets.h"

/****************************************************************************
 * Private Function Prototypes
 ****************************************************************************/

#ifdef RX_TASK
/* Execution Sheduler task for RX. It do the follwoing:
    - call to simuElTxProcessSocket execution leg;
 */
static void* esRxTask(void* lpParam);

/* Execution leg for processing incoming UDP messages
 */
static void simuElTxProcessSocket(void);
#endif

/****************************************************************************
 * Private Variables (Must be declared static)
 ****************************************************************************/

static pthread_t es_tRxId; /* Sheduler RX task Id */
static sem_t     es_sem_stop; /* Stop Sheduler task semaphore   */
static sem_t     es_sem_notify; /* Sheduler notification semaphore   */

/****************************************************************************
 * Function Name  : esStart
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Runs RLC layer simulator.
 ****************************************************************************/
void    esStart()
{
    pthread_attr_t thread_attr;

    SIM_LOG(SIM_LOG_DUMP, "Start %s", __FUNCTION__);

    sem_init(&es_sem_stop, 0, 0);
    sem_init(&es_sem_notify, 0, 1);

    /* first create a thread attribute init object. */
    pthread_attr_init(&thread_attr);

#ifdef RX_TASK
    /* create RX task */
    pthread_create(&es_tRxId, &thread_attr, &esRxTask, PNULL);
#endif
}

/****************************************************************************
 * Function Name  : esStop
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Stops RLC layer simulator.
 ****************************************************************************/
void    esStop()
{
    void *status;

    sem_post(&es_sem_stop);
    esUnlock();
    pthread_join( es_tRxId, &status );
}

/****************************************************************************
 * Function Name  : esLock
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Lock External scheduler.
 ****************************************************************************/
void    esLock()
{
    sem_wait(&es_sem_notify);
}
/****************************************************************************
 * Function Name  : esUnlock
 * Inputs         : .
 * Outputs        : None.
 * Returns        : None.
 * Description    : Unlock External scheduler.
 ****************************************************************************/
void    esUnlock()
{
    sem_post(&es_sem_notify);
}


#ifdef RX_TASK
/****************************************************************************
 * Function Name  : simuElTxProcessSocket
 * Inputs         : None.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Process incoming UDP messages
 ****************************************************************************/
void simuElTxProcessSocket(void)
{
    UInt8 buf[SOCKET_BUFFER_SIZE];
    UInt32 length;
    length = ReadFromSocket( SIM_SOCKET_TYPE_SCTP, buf ,0);
    if(length>0) /*packet length is correct according to header*/
    {
        SIM_LOG(SIM_LOG_DUMP,"RECV\t: udpLen[%i]", length);
        userPrintRawBuffer("RECV\t", buf, length, 0);
    }
}

/****************************************************************************
 * Function Name  : esRxTask
 * Inputs         : lpParam - not used.
 * Outputs        : None.
 * Returns        : None.
 * Description    : Execution Sheduler task for RX Path. It do the follwoing:
    - call to simuRxProcessMessage execution leg;
 ****************************************************************************/
void* esRxTask(void* lpParam)
{
    SInt32  sem_state;

    SIM_LOG(SIM_LOG_DUMP, "Start %s", __FUNCTION__);

    do {
        if(SIM_TRUE==isReadSocketTaskEnabled)
        {
            esLock();

            /* process incoming UDP packets */
            simuElTxProcessSocket();

            esUnlock();
        }

        userSleep(0);
        /* get and check stop task condition */
        sem_state = sem_trywait(&es_sem_stop);
    } while (sem_state);

    sem_post(&es_sem_stop);
    SIM_LOG(SIM_LOG_DUMP, "Stop %s", __FUNCTION__);
    return PNULL;
}
#endif
