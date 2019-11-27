
/**************************************************************************
 *
 * ARICENT -
 *
 * Copyright (C) 2014 Aricent Inc. All Rights Reserved.
 *
 *************************************************************************
 *
 * $Id: lteComOAMIntegration.c
 *
 *************************************************************************
 *
 * File Description : This file contains the functions and
 *                    structure that are used for OAM integration
 *************************************************************************/

/******************************************************************************
 * Project Includes
 *****************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <execinfo.h>
#include <fcntl.h>           /* For O_* constants */
#include <sys/stat.h>        /* For mode constants */
#include <mqueue.h>
#include "lteComOAMIntegration.h"
#include "logging.h"
#include "lteLog.h"
#include "lteMisc.h"
#include "lteThread.h"
#include "lteversion.h"

/*533-GCC-warning-fix*/
#ifdef CSPL_LEAK_DEBUG
#include "msgpool.h"
#endif
/*533-GCC-warning-fix*/
#include "lteLayer2CommanTypes.h"

/******************************************************************************
 Private Definitions
 *****************************************************************************/
#define MAX_CRITICAL_TIME 1000
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

LteLayer2CoreInfo lteLayer2CoreInfo_g;

/** CA2 Changes Start **/
extern UInt64 ccInMs_g;
/** CA2 Changes End **/
/* + SPR 17439 */
#ifndef MAC_AUT_TEST
extern void stopOtherThread(void);
#endif

extern void stopLogReader(void);
/* SPR 15909 fix start */
extern  tickType_t getCurrentTick(void);
/* SPR 15909 fix end */
/* - SPR 17439 */

#if !(defined UE_SIM_TESTING) && !(defined GTP_SPLIT)
extern SockAddr txOAM_addr_g;
#endif

TimeSpec g_deadTime;

/*****************************************************************************
 * Global variables
 ****************************************************************************/
#ifdef FLEXRAN
/* Global variable to enable/disable FAPI wireshark logging.
 * This can be enabled/disabled through command line argument.
 * */
SInt32 flexranFapiLoggingEnable_g = 1;
#endif
/*SPR 21653 changes start*/
/*SPR21717 Changes Start*/
UInt32 getCommandLineOptionsSplit(int argc, char *argv[])
{
	UInt32 numCPU = sysconf(_SC_NPROCESSORS_ONLN);

	if (argc < 5) {
		fprintf(stderr, "Usage:\n");
		fprintf(stderr, "./lte_gtpu <Core Binding> <GTPU numFile> <GTPU SHM size> <GTPU Log Level>\n");
		return 0;
	}
	if (atoi(argv[1]) > numCPU-1) {
		fprintf(stderr, "CPU Count can not be more than %d\n", numCPU);
		return 0;
	}
	/*SPR21708 Changes End*/

	if (!strcmp(argv[2], "0")) {
		snprintf(l2Input.numFile, MAX_LOGGER_TYPE_STR_LEN, "%d", MAX_LOG_FILES_DEFAULT);
	} else if (atoi(argv[2]) > MAX_LOG_FILES_ALLOWED) {
		fprintf(stderr, "GTPUnumFile is graeter than MAX_LOG_FILES_DEFAULT[%d]\n", MAX_LOG_FILES_DEFAULT);
		return 0;
	} else {
		snprintf(l2Input.numFile, MAX_LOGGER_TYPE_STR_LEN, "%s", argv[2]);
	}
	if (!strcmp(argv[3], "0")) {
		l2Input.shmSize = MAX_LOG_MEMORY_SIZE;
		snprintf(l2Input.logFileSize, MAX_LOGGER_TYPE_STR_LEN, "%d", MAX_LOG_MEMORY_SIZE);
	} else if (atoi(argv[3]) > MAX_LOG_MEMORY_SIZE) {
		fprintf(stderr, "GTPUlogFileSize is greater than MAX_LOG_MEMORY_SIZE[%d]\n", MAX_LOG_MEMORY_SIZE);
		return 0;
	} else {
		l2Input.shmSize = atoi(argv[3]);
		snprintf(l2Input.logFileSize, MAX_LOGGER_TYPE_STR_LEN, "%d", atoi(argv[3]));
	}

	if (atoi(argv[4]) > 6 || atoi(argv[4]) < 0) {
		fprintf(stderr, "Wrong log level Provided. Range for LogLevel 0-6\n");
		l2Input.loglevel = 3;
	} else {
		l2Input.loglevel = atoi(argv[4]);
	}

	strNCpy(l2Input.logger_option, "binary", MAX_LOGGER_TYPE_STR_LEN);
	l2Input.logger_option[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';
	strncpy(l2Input.gtpu_file_name, "GTPU_REGION", NAME_MAX);
	l2Input.gtpu_file_name[NAME_MAX - 1] = '\0';
	l2Input.l2gtpuflag = 1;
	return 1;
}
/* SPR21717 Changes End*/

UInt32 getCommandLineOptions(int argc, char *argv[])
/*SPR 21653 changes end*/
{
	SInt32 core_set_flag = 0;
#ifdef LOG_PRINT_ENABLED
	SInt32 log_level_set_flag = 0;
#endif
	/* SPR21717 Changes Start*/
	l2Input.l2gtpuflag = 0;
	/* SPR21717 Changes End*/
	SInt32 input_option;
	UInt32 option_index = 0;
	InternalCellIndex cellIndex = 0;
#ifndef GTP_SPLIT

	while (1) {
		static struct option long_options[] = {
			{"core", 1, NULL, 'c'},
			/** SPR 2911 start **/
			{"pdcpCore", 1, NULL, 'p'},
			/** SPR 2911 end **/
			{"ciphering_core", 1, NULL, 'e'},
			{"encoder_core", 1, NULL, 'g'},
			{"ulcore", 1, NULL, 'u'},
			{"mac_rlclogs", 1, NULL, 'f'},
			{"rlc_pdcplogs", 1, NULL, 'r'},
			{"loglevel", 1, NULL, 'l'},
			{"logger_option", 1, NULL, 'o'},
			{"log_shm_size", 1, NULL, 's'},
			{"log_file_size", 1, NULL, 'b'},
			{"log_num_files", 1, NULL, 'n'},
			{"help", 0, NULL, 'h'},
			{"version", 0, NULL, 'v'},
			{0, 0, 0, 0}
		};
		input_option = getopt_long(argc, argv, "", long_options, (int *)&option_index);
		if (input_option ==  -1)
			break;

		switch (input_option) {
		case 'c':
			/* CA Changes start */
			lteLayer2CoreInfo_g.lteLayer2DLCoreNum[cellIndex] = atoi(optarg);
			/* SPR 2991 changes start*/
			lteLayer2CoreInfo_g.lteLayer2PdcpCoreNum = atoi(optarg);
			/* SPR 2991 changes end*/
			core_set_flag = 1;
			break;

		case 'u':
			lteLayer2CoreInfo_g.lteLayer2ULCoreNum[cellIndex] = atoi(optarg);
			core_set_flag = 1;
			/* SPR 2991 changes start*/
		case 'p':
			lteLayer2CoreInfo_g.lteLayer2PdcpCoreNum = atoi(optarg);
			break;
			/* SPR 2991 changes end*/
		case 'e':
			lteLayer2CoreInfo_g.layer2ciphCoreNum = atoi(optarg);
			core_set_flag = 1;
			break;
		case 'g':
			lteLayer2CoreInfo_g.lteLayer2EncoderCoreNum[cellIndex] = atoi(optarg);
			core_set_flag = 1;
			break;
		case 'f':
			strNCpy(l2Input.mac_file_name, optarg, NAME_MAX);
			l2Input.mac_file_name[NAME_MAX - 1] = '\0';
			break;
		case 'r':
			strNCpy(l2Input.rlc_file_name, optarg, NAME_MAX);
			l2Input.rlc_file_name[NAME_MAX - 1] = '\0';
			break;
#ifdef LTE_EMBMS_SUPPORTED
		case 'm':
			strNCpy(l2Input.sync_file_name, optarg, NAME_MAX);
			l2Input.sync_file_name[NAME_MAX - 1] = '\0';
			break;
#endif
		case 'l':
			/* + Coverity 5.0 */
			//if(sscanf(optarg,"%x", &l2Input.loglevel) > 0)
			l2Input.loglevel = atoi(optarg);
			{
			/* SPR 2486 Start */
			if ((l2Input.loglevel != 0) && (l2Input.loglevel != 1)\
			&& (l2Input.loglevel != 2) && (l2Input.loglevel != 4)\
			&& (l2Input.loglevel != 4) && (l2Input.loglevel != 8)\
			&& (l2Input.loglevel != 16))
			/* SPR 2486 End*/
			{
				fprintf(stderr, "Invalid Log level");
			} else
			{
#ifdef LOG_PRINT_ENABLED
			log_level_set_flag = 1;
#endif
			}
			}
			break;
		case 'o':
			strNCpy(l2Input.logger_option, optarg, MAX_LOGGER_TYPE_STR_LEN);
			l2Input.logger_option[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';
			break;
		case 's':
			l2Input.shmSize = (atoi(optarg) * 1024 * 1024);
			break;
		case 'b':
			strNCpy(l2Input.logFileSize, optarg, MAX_LOGGER_TYPE_STR_LEN);
			l2Input.logFileSize[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';

			break;
		case 'n':
			strNCpy(l2Input.numFile, optarg, MAX_LOGGER_TYPE_STR_LEN);
			l2Input.numFile[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';

			break;
		case 'h':
			fprintf(stderr,
			"--core=<core ID>                         ->core Id to bind L2\n"
			"--pdcpCore=<core ID>                     ->core Id to bind PDCP thread\n"
			"--ciphering_core=<core ID>               ->core Id to bind ciphering thread\n"
			"--encoder_core=<core ID>                 ->core Id to bind MAC encoder thread\n"
			"--ulcore= <core ID>                      ->core Id to bind MAC UL thread\n --mac_rlclogs=<filename>                 ->filename of generated rlc_pdcplogs\n"
			"--rlc_pdcplogs=<filename>                ->filename of generated rlc_pdcplogs\n"
			"--loglevel=<loglevel>                    ->logging level:\n"
			"                                           DISABLE logs = 0\n"
			"                                           FATAL Logs   = 1\n"
			"                                           ERROR Logs   = 2\n"
			"                                           WARNING Logs = 4\n"
			"                                           INFO Logs    = 8\n"
			"                                           DEBUG Logs   = 10\n"
			"--logger_option=<console/text/binary>    ->logs format and location\n"
			"--log_shm_size=<in MB>                   ->size of shared memory fro logging\n"
			"--log_file_size=<in MB>                  ->size of log-binFiles\n"
			"-- log_num_files=<number of files>       ->no. of log-binFiles to be generated\n"
			"--version                                ->the last git commit id of the source code\n");
			break;
		case 'v':
			fprintf(stderr, "the last git commit id of the source code is %s\n", commit_id);
			exit(0);

		default:
			fprintf(stderr, "\n Usage is ./lteLayer2 --core=<core ID>--pdcpCore=<core ID> --ciphering_core=<core ID> --encoder_core=<core ID>--ulcore= <core ID> --mac_rlclogs=<filename> --rlc_pdcplogs=<filename> --loglevel=<loglevel>  --logger_option=<console/text/binary> --log_shm_size=<in MB> --log_file_size=<in MB>  --log_num_files=<number of files> --help <core id> <log level>");
			}
	}
#else
	while (1) {
		static struct option long_options[] = {
			{"core", 1, NULL, 'c'},
			{"gtplogs", 1, NULL, 'g'},
			{"loglevel", 1, NULL, 'l'},
			{"logger_option", 1, NULL, 'o'},
			{"log_shm_size", 1, NULL, 's'},
			{"log_file_size", 1, NULL, 'b'},
			{"log_num_files", 1, NULL, 'n'},
			{"help", 0, NULL, 'h'},
			{0, 0, 0, 0}
		};
		input_option = getopt_long(argc, argv, "", long_options, (int *)&option_index);
		if (input_option ==  -1)
			break;

		switch (input_option) {
		case 'c':
			lteLayer2CoreInfo_g.lteLayer2DLCoreNum[cellIndex] = atoi(optarg);
			core_set_flag = 1;
		break;

		case 'g':
			strNCpy(l2Input.mac_file_name, optarg, NAME_MAX);
			l2Input.mac_file_name[NAME_MAX - 1] = '\0';
		break;

		case 'l':
			l2Input.loglevel = atoi(optarg);
			{
			if ((l2Input.loglevel != 0) && (l2Input.loglevel != 1)\
			&& (l2Input.loglevel != 2) && (l2Input.loglevel != 4)\
			&& (l2Input.loglevel != 4) && (l2Input.loglevel != 8)\
			&& (l2Input.loglevel != 16))
			{
			fprintf(stderr, "Invalid Log level");
			} else
			{
#ifdef LOG_PRINT_ENABLED
			log_level_set_flag = 1;
#endif
			}
			}
			break;
		case 'o':
			strNCpy(l2Input.logger_option, optarg, MAX_LOGGER_TYPE_STR_LEN);
			l2Input.logger_option[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';
			break;
		case 's':
			l2Input.shmSize = (atoi(optarg) * 1024 * 1024);
			break;
		case 'b':
			strNCpy(l2Input.logFileSize, optarg, MAX_LOGGER_TYPE_STR_LEN);
			l2Input.logFileSize[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';

			break;
		case 'n':
			strNCpy(l2Input.numFile, optarg, MAX_LOGGER_TYPE_STR_LEN);
			l2Input.numFile[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';

			break;
		case 'h':
			fprintf(stderr,
			"--core=<core ID>                         ->core Id to bind L2\n"
			"--gtplogs=<filename>                     ->filename of generated gtplogs\n"
			"--loglevel=<loglevel>                    ->logging level:\n"
			"                                           DISABLE logs = 0\n"
			"                                           FATAL Logs   = 1\n"
			"                                           ERROR Logs   = 2\n"
			"                                           WARNING Logs = 4\n"
			"                                           INFO Logs    = 8\n"
			"                                           DEBUG Logs   = 10\n"
			"--logger_option=<console/text/binary>    ->logs format and location\n"
			"--log_shm_size=<in MB>                   ->size of shared memory fro logging\n"
			"--log_file_size=<in MB>                  ->size of log-binFiles\n"
			"-- log_num_files=<number of files>       ->no. of log-binFiles to be generated\n");
			break;
			default:
			fprintf(stderr, "\n Usage is ./lteLayer2 --core=<core ID> --gtplogs=<filename> --logger_option=<console/text/binary> --log_shm_size=<in MB> --log_file_size=<in MB>  --log_num_files=<number of files> --help <core id> <log level>");
		}
		l2Input.l2gtpuflag = 1;
	}
#endif /* GTP_SPLIT */
	if (atoi(l2Input.logFileSize) == 0)
		snprintf(l2Input.logFileSize, MAX_LOGGER_TYPE_STR_LEN, "%d", MAX_LOG_MEMORY_SIZE);

	/* SPR 4010 Fix Start */
	if (atoi(l2Input.numFile) == 0)
	/* SPR 4010 Fix End */
		snprintf(l2Input.numFile, MAX_LOGGER_TYPE_STR_LEN, "%d", MAX_LOG_FILES_DEFAULT);

	if (strlen_wrapper(l2Input.logger_option) == 0) {
		strNCpy(l2Input.logger_option, "binary", MAX_LOGGER_TYPE_STR_LEN);
		l2Input.logger_option[MAX_LOGGER_TYPE_STR_LEN - 1] = '\0';
	}
	if (l2Input.loglevel == 0)
		l2Input.loglevel = 4;

	if (strlen_wrapper(l2Input.mac_file_name) == 0) {
#ifndef GTP_SPLIT
		/*SPR 21653 changes start*/
		strNCpy(l2Input.mac_file_name, "MAC_RLC_REGION", NAME_MAX);
		/*SPR 21653 changes end*/
#else
		strNCpy(l2Input.mac_file_name, "GTP_REGION", NAME_MAX);
#endif
		l2Input.mac_file_name[NAME_MAX - 1] = '\0';
	}
#ifndef GTP_SPLIT
	if (strlen_wrapper(l2Input.rlc_file_name) == 0) {
		/*SPR 21653 changes start*/
		strNCpy(l2Input.rlc_file_name, "RLC_PDCP_REGION", NAME_MAX);
		/*SPR 21653 changes end*/
		l2Input.rlc_file_name[NAME_MAX - 1] = '\0';
	}
#endif
#ifdef LTE_EMBMS_SUPPORTED
	if (strlen_wrapper(l2Input.sync_file_name) == 0) {
		strNCpy(l2Input.sync_file_name, "SYNC_REGION", NAME_MAX);
		l2Input.sync_file_name[NAME_MAX - 1] = '\0';
	}
#endif
	/* SPR 5178 changes starts here */
	/* SPR 5178 changes ends here */
	if (l2Input.shmSize == 0)
		l2Input.shmSize = MAX_LOG_MEMORY_SIZE;

	if (core_set_flag == 0) {
		if (argc > optind) {
#ifndef GTP_SPLIT
			/* SPR 21804 Fix Start */
#ifdef LAYER2_DLTG_THREAD
			/* SPR 21804 Fix End */
			if (argc < 14) {
				fprintf(stderr, "Usage is ./lteLayer2 <MAC DL core num to bind><PDCP core num to bind><Ciphering Core Number><Encoder core num to bind><MAC UL Core Number><DL TG core num to bind> <packet size> <number of packets> <time in ms to pump> <throughput display time in ms><traffic-profile> <lastUE> <start_ueID>\n");
				return 0;
			}
#else
			if (argc < 6) {
				fprintf(stderr, "Usage is ./lteLayer2 <DL core num to bind> <PDCP core num to bind> <Ciphering Core Number>  <Encoder core num to bind><MAC UL Core Number>\n");
				/*SPR 21653 changes start*/
				/*code removed*/
				/*SPR 21653 changes end*/

				return 0;
			}
#endif

			/* In case DL/UL split this is DL core and
			*  w/o DL/UL it is MAC core */
#ifdef FLEXRAN
			lteLayer2CoreInfo_g.lteLayer2DLCoreNum[0] = atoi(argv[optind]);
			lteLayer2CoreInfo_g.lteLayer2DLCoreNum[1] = atoi(argv[optind+1]);
			lteLayer2CoreInfo_g.lteLayer2EncoderCoreNum[0] = atoi(argv[optind + 2]);
			lteLayer2CoreInfo_g.lteLayer2EncoderCoreNum[1] = atoi(argv[optind + 3]);
			lteLayer2CoreInfo_g.lteLayer2UlRlcCoreNum = atoi(argv[optind + 4]);
			lteLayer2CoreInfo_g.lteLayer2PdcpCoreNum = atoi(argv[optind+4]);
			flexranFapiLoggingEnable_g = atoi(argv[optind + 5]);
#else
			lteLayer2CoreInfo_g.lteLayer2DLCoreNum[cellIndex] = atoi(argv[optind]);
			lteLayer2CoreInfo_g.lteLayer2PdcpCoreNum = atoi(argv[optind+1]);
			lteLayer2CoreInfo_g.layer2ciphCoreNum = atoi(argv[optind + 2]);
			lteLayer2CoreInfo_g.lteLayer2EncoderCoreNum[cellIndex] = atoi(argv[optind + 3]);
			lteLayer2CoreInfo_g.lteLayer2ULCoreNum[cellIndex] = atoi(argv[optind + 4]);
#endif
#else
			if (argc < 2) {
				fprintf(stderr, "Usage is ./lte_gtpu <core num to bind>\n");
				return 0;
			}
			lteLayer2CoreInfo_g.lteLayer2DLCoreNum[cellIndex] = atoi(argv[optind]);
#endif /* GTP_SPLIT */
		} else {
#ifndef GTP_SPLIT
		fprintf(stderr, " Usage is ./lteLayer2 --DLcore=<core ID> --pdcpCore=<core ID> --ciphering_core=<core ID> --encoder_core=<core ID> --ULcore=<core ID> --mac_rlclogs=<filename> --rlc_pdcplogs=<filename> --loglevel=<loglevel> --logger_option=<console/text/binary> --log_shm_size=<in MB> --log_file_size=<in MB> --log_num_files=<number of files> --help <core id> <log level>");
#else
		fprintf(stderr, " Usage is ./lte_gtpu --core=<core ID> --gtplogs=<filename> --logger_option=<console/text/binary> --log_shm_size=<in MB> --log_file_size=<in MB> --log_num_files=<number of files> --help <core id> <log level>");
#endif
		}
	}


#ifdef LOG_PRINT_ENABLED
	if (argc > optind) {
		if (log_level_set_flag == 0) {
			if ((core_set_flag == 0) && (argc > optind+1))
				l2Input.loglevel = atoi(argv[optind + 1]);
			else if (core_set_flag == 1)
				l2Input.loglevel = atoi(argv[optind]);
			log_level_set_flag = 1;
		}
	}

	if (log_level_set_flag == 1)
		enable_log_level(l2Input.loglevel);

#endif

	return 1;
}

#ifdef OVERSHOOT_CORRECTION
volatile UInt64 ccIncrease_g = 0;
__thread UInt64 cc_start_count_g;
/****************************************************************************
 * Function Name  :CriticalTimerHandler()
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : This function is responsible to send init Indication
		    to OAM on L2 Initialization
 ****************************************************************************/
/* FDD_Warning_Fix */
void CriticalTimerHandler(SInt32 sig)
/* FDD_Warning_Fix */
{
	/* +- SPR 17777 */
	LTE_GCC_UNUSED_PARAM(sig)
	UInt64 cc_end_count = 0;

	cc_end_count = rdtsc();
	if (cc_start_count_g > cc_end_count)
		ccInMs_g  = ULLONG_MAX - cc_start_count_g + cc_end_count;
	else
		ccInMs_g  = cc_end_count - cc_start_count_g;
	ccIncrease_g++;
}


/****************************************************************************
 * Function Name  : getCcInMs()
 * Inputs         :
 * Outputs        :
 * Returns        :
 * Variables      :
 * Description    : This function is responsible to send init Indication
		    to OAM on L2 Initialization
 ****************************************************************************/
/* + SPR 17439 */
unsigned long long getCcInMs(void)
/* - SPR 17439 */
{
	struct itimerval CriticalTimer;
	int timer_ret = 1;
	struct sigaction sig;

	memSet(&sig, 0, sizeof(sig));
	sig.sa_handler = CriticalTimerHandler;

	//sighandler_t sig = SIG_ERR;
#ifdef CYCLE_COUNT_1MS_RDB
	ccInMs_g = 1000000;
	return ccInMs_g;
#endif
	CriticalTimer.it_interval.tv_sec = 0;
	CriticalTimer.it_interval.tv_usec = 0;
	CriticalTimer.it_value.tv_sec = 0;
	CriticalTimer.it_value.tv_usec = MAX_CRITICAL_TIME;
	if (-1 == sigaction(SIGALRM, &sig, 0)) {
		printf("signal/sigaction failed- errno = %d\n", errno);
		abort_wrapper();
	}
	cc_start_count_g = rdtsc();
	timer_ret = setitimer_wrapper(ITIMER_REAL, &CriticalTimer, NULL);
	if (timer_ret ==  -1)
		pError("timer error\n");

	while (1) {
		if ((ccIncrease_g  >= 1)) {
		    sig.sa_handler = SIG_DFL;
			if (-1 == sigaction(SIGALRM, &sig, 0)) {
				printf("signal/sigaction failed- errno = %d\n", errno);
				abort_wrapper();
			}
			return ccInMs_g;
		}
	}
}
#endif /*OVERSHOOT_CORRECTION*/

/*SPR 21653 changes start*/
/*code removed*/
/*SPR 21653 changes end*/

void initAndConfigLogFramework(
/*SPR 21653 changes start*/
/* + SPR 17439 */
void
/* - SPR 17439 */
/*SPR 21653 changes end*/
)
{
#ifdef LOG_PRINT_ENABLED
	/*SPR 16855 +-*/
	if (l2Input.l2gtpuflag) {
		/*SPR 21717 changes start*/
		logger_init(LTE_L2_LOG_REGION_FOUR_ID, LTE_L2_LOG_REGION_FIVE_NAME, l2Input.shmSize);
	} else {
#ifdef DL_UL_SPLIT
		/*SPR 21653 changes start*/
		logger_init(LTE_L2_LOG_REGION_ONE_ID, LTE_L2_LOG_REGION_ONE_NAME, l2Input.shmSize);
		/*SPR 21653 changes end*/

#else
		logger_init(LTE_L2_LOG_REGION_FOUR_ID, LTE_L2_LOG_REGION_FOUR_NAME, l2Input.shmSize);
		/* SPR 21930 Fix +- */
		/*SPR 21717 changes end*/

#endif/*DL_UL_SPLIT*/
	}
	/* SPR 21930 Fix +- */
	initlogFramework();
	startLogReader(&l2Input);
#endif

}

/* SPR 4422 Fix Start */
void handleStopEvent(int signum)
{
	/* +- SPR 17777 */
	LTE_GCC_UNUSED_PARAM(signum)
	/* +- SPR 17777 */
	sleep(500);
}

/* SPR 4422 Fix End */

/* SPR 1998 changes start */
/****************************************************************************
 * Function Name  : handleFatalSignal
 * Inputs         : None
 * Outputs        : handle fatal signal_wrapper and crash
 * Returns        : Success
 * Variables      :
 * Description    : This API handles fatal signal received.
 ****************************************************************************/
void handleFatalSignal(int signum)
{
	clock_gettime_real_time(&g_deadTime);

	signal_wrapper(signum, SIG_DFL);
#ifndef MAC_AUT_TEST
	stopOtherThread();
#endif
	stopLogReader();
	/*SPR 21653 changes start*/
	/*code removed*/
	/*SPR 21653 changes end*/
#ifdef CSPL_LEAK_DEBUG
	qvPrintAllocBuf();
	qvPrintMemAllocBuf();
#endif
#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
	qvPrintMemPoolAllocBuffers();
	qvPrintLeakedMsgBuf();
#endif
	logBacktrace;
	raise_wrapper(signum);
}

void handleSigInt(int signum)
{
	signal_wrapper(signum, SIG_DFL);
	stopLogReader();

	/*SPR 21653 changes start*/
	/*code removed*/
	/*SPR 21653 changes end*/
#ifdef CSPL_LEAK_DEBUG
	qvPrintAllocBuf();
	qvPrintMemAllocBuf();
#endif
#if (defined(CSPL_LEAK_DEBUG_LITE) && !defined(UE_SIM_TESTING))
	qvPrintMemPoolAllocBuffers();
	qvPrintLeakedMsgBuf();
#endif

#ifdef LINUX_PC_TEST
	fprintf(stderr,"handleSigInt----quit!\n");
#endif

	exit(0);
}

/****************************************************************************
 * Function Name  : setFatalSignalHandlers
 * Inputs         : None
 * Outputs        : handle fatal signal and crash
 * Returns        : Success
 * Variables      :
 * Description    : This API handles fatal signal received.
 ****************************************************************************/
/* + SPR 17439 */
 void setFatalSignalHandlers(void)
/* - SPR 17439 */
 {
	 /* Following signals will be caught and debug info shall be printed
  	- SIGQUIT
  	- SIGILL
  	- SIGABRT
  	- SIGFPE
  	- SIGSEGV
  	- SIGBUS
	  */
	/* SPR 4422 Fix Start */
	signal_wrapper(SIGRTMIN, handleStopEvent);
	/* SPR 4422 Fix End */
	signal_wrapper(SIGQUIT, handleFatalSignal);
	signal_wrapper(SIGILL, handleFatalSignal);
	signal_wrapper(SIGABRT, handleFatalSignal);
	signal_wrapper(SIGFPE, handleFatalSignal);
	signal_wrapper(SIGSEGV, handleFatalSignal);
	signal_wrapper(SIGBUS, handleFatalSignal);
	signal_wrapper(SIGINT, handleSigInt);
 }

/****************************************************************************
 * Function Name  : enableDebugTraceback
 * Inputs         : None
 * Outputs        : enable backtracing for debugging purpose
 * Returns        : Success
 * Variables      :
 * Description    : This API initialize the debugging for crash handling
 ****************************************************************************/
/* + SPR 17439 */
 UInt32 enableDebugTraceback(void)
/* - SPR 17439 */
 {
	UInt32 ret = 0;

	ret = setRlimitOfCore();
	if (ret) {
	  /* set signal handlers */
	  setFatalSignalHandlers();
	}
	return ret;
 }

void stopAllThread(UInt32 max_thread_id, ULong32 *threadId_arr)
{
	ULong32 thdSelfId = 0;

	thdSelfId = threadSelf();
	UInt8 thdId;

	for (thdId = 0; thdId < max_thread_id; thdId++) {
		if (!threadEqual(thdSelfId, threadId_arr[thdId])) {
		threadKill(threadId_arr[thdId], SIGRTMIN);
		} else {
			LOG_MAC_MSG(HSM_THREAD_FAILURE, LOGFATAL, L2_SYS_FAIL,
					0, thdId, 0, 0, 0, 0, 0, 0,
					__func__, "Health Status Thread Failure");

		}
	}
}

/*SPR 21653 changes start*/
/* + coverity 55796 */
/*code removed*/
/* - coverity 55796 */
/*SPR 21653 changes end*/
#if !(defined UE_SIM_TESTING) && !(defined GTP_SPLIT)
/****************************************************************************
 * Function Name  : sendL2InitIndToOAM
 * Inputs         : cnfBuff_p - Buffer to be sent
 *                  msgLen - Length of buffer
 *                  sendSockFD - Socket FD for sending the message
 * Returns        : Success
 * Variables      :
 * Description    : This API send indications to OAM
 ****************************************************************************/
SInt32 sendL2InitIndToOAM(UInt8 *cnfBuff_p, UInt16 msgLen, SInt32 sendSockFD)
{
	/* Fill msgLen Now */
	SInt32 retval = 0;

	LTE_SET_U16BIT(cnfBuff_p + API_HEADER_MSGLEN_OFFSET,  msgLen);
	retval = sendto_wrapper(sendSockFD, cnfBuff_p, msgLen, 0, (const SockAddr *)&txOAM_addr_g, sizeof(txOAM_addr_g));

	return retval;
}
#endif
