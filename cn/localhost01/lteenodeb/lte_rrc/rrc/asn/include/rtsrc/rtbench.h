/* Benchmark utility functions */

/**
 * @file rtbench.h
 */
#ifndef _BENCH_H_
#define _BENCH_H_

#include <stdio.h>
#include <stdlib.h>
#ifndef _WIN32_WCE
#include <time.h>
#ifndef _NUCLEUS
#include <sys/stat.h>
#endif
#else
#include "rtxsrc/wceAddon.h"
#endif

#ifdef __cplusplus
extern "C" {
#endif

#define ITERCNT 100000

double rtBenchAverageMS (clock_t start, clock_t finish, double icnt);

#ifdef __cplusplus
}
#endif
#endif
