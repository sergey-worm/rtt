/***************************************************************************
 tag: Sergey Worm  Tue Sep 11 14:53:50 MSK 2018  fosi.h

                       fosi.h -  description
                          -------------------
   begin                : Tue Sep 11 2018
   copyright            : (C) 2018 Sergey Worm
   email                : sergey.worm@gmail.com

***************************************************************************
*                                                                         *
*   This program is free software; you can redistribute it and/or modify  *
*   it under the terms of the GNU General Public License as published by  *
*   the Free Software Foundation; either version 2 of the License, or     *
*   (at your option) any later version.                                   *
*                                                                         *
***************************************************************************/


/**
 * This file translates the FOSI (Framework Operating System Interface) from
 * orocos calls to native RTOS calls
 */

#ifndef __FOSI_H
#define __FOSI_H

#define HAVE_FOSI_API

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <limits.h>
#include <float.h>
#include <assert.h>
#include <sys/time.h>
#include <time.h>
#include <unistd.h>
#include "../oro_limits.h"
#include "wlibc_panic.h"
#include "wrm_mtx.h"
#include "wrm_log.h"

unsigned long long l4_system_clock();

typedef long long NANO_TIME;
typedef long long TICK_TIME;
typedef struct timespec TIME_SPEC;

static const TICK_TIME InfiniteTicks = LLONG_MAX;
static const NANO_TIME InfiniteNSecs = LLONG_MAX;
static const double    InfiniteSeconds = DBL_MAX;

#define ORO_WAIT_ABS 0 /** rtos_task_wait_period may wait less than the duration required to pad the period to
                           catch-up with overrun timesteps (wait according to an absolute timeline) */
#define ORO_WAIT_REL 1 /** rtos_task_wait_period will always pad the current timestep to the desired period,
                           regardless of previous overruns (wait according to a relative timeline) */

typedef struct
{
	L4_thrid_t thr;        //
	TIME_SPEC periodMark;  // moment for next wakeup
	NANO_TIME period;      //
	char name[16];         //
	int priority;          //
	int wait_policy;       //
} RTOS_TASK;


#define ORO_SCHED_RT    0  /** not used for Wrm.OS */
#define ORO_SCHED_OTHER 0  /** not used for Wrm.OS */

static inline NANO_TIME nano_abs2rel(NANO_TIME nano_abs)
{
	NANO_TIME now = l4_system_clock() * 1000;
	if (now >= nano_abs)
		return 0;
	return nano_abs - now;
}

static inline NANO_TIME rtos_get_time_ns(void)
{
	return l4_system_clock() * 1000;
}

/**
 * This function should return ticks,
 * but we use ticks == nsecs in userspace
 */
static inline NANO_TIME rtos_get_time_ticks()
{
	//printf("%s:  hi.\n", __func__);
	return l4_system_clock() / 10000;
}

static inline int rtos_nanosleep(const TIME_SPEC* rqtp, TIME_SPEC* rmtp)
{
	panic("%s:  implement me.\n", __func__);
	return nanosleep(rqtp, rmtp);
}

/**
 * No conversions are needed in userspace.
 * The HBGenerator needs this for accurate timekeeping,
 * which is an anachronism in userspace.
 */
static inline long long nano2ticks(long long nano)
{
	//wrm_logd("%s:  hi - nano=%llu.\n", __func__, nano);
	return nano / (10*1000*1000);
}

static inline long long ticks2nano(long long count)
{
	//printf("%s:  hi.\n", __func__);
	return count * 10*1000*1000;
}

// high-resolution time to timespec
// hrt is in ticks
static inline TIME_SPEC ticks2timespec(TICK_TIME hrt)
{
	//wrm_logd("%s:  hi - hrt=%llu.\n", __func__, hrt);
	NANO_TIME nano = ticks2nano(hrt);
	TIME_SPEC timevl;
	timevl.tv_sec  = nano / 1000000000LL;
	timevl.tv_nsec = nano % 1000000000LL;
	return timevl;
}

typedef Wrm_sem_t rt_sem_t;

static inline int rtos_sem_init(rt_sem_t* m, int value)
{
	//printf("%s:  hi - sem=%p, val=%d.\n", __func__, m, value);
	return wrm_sem_init(m, 0, value);
}

static inline int rtos_sem_destroy(rt_sem_t* m)
{
	//printf("%s:  hi - sem=%p.\n", __func__, m);
	return wrm_sem_destroy(m);
}

static inline int rtos_sem_signal(rt_sem_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_sem_post(m);
}

static inline int rtos_sem_wait(rt_sem_t* m)
{
	//printf("%s:  hi - sem=%p.\n", __func__, m);
	return wrm_sem_wait(m, -1);
}

static inline int rtos_sem_trywait(rt_sem_t* m)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

static inline int rtos_sem_wait_timed(rt_sem_t* m, NANO_TIME delay)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

static inline int rtos_sem_wait_until(rt_sem_t* m, NANO_TIME abs_time)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

static inline int rtos_sem_value(rt_sem_t* m)
{
	//printf("%s:  hi - sem=%p.\n", __func__, m);
	unsigned val = 0;
	int rc = wrm_sem_getvalue(m, &val);
	//printf("%s:  hi - sem=%p, rc=%d, val=%u.\n", __func__, m, rc, val);
	return rc ? -1 : val;
}

// Mutex functions

typedef Wrm_mtx_t rt_mutex_t;
typedef Wrm_mtx_t rt_rec_mutex_t;

static inline int rtos_mutex_init(rt_mutex_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_init(m);
}

static inline int rtos_mutex_destroy(rt_mutex_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_destroy(m);
}

static inline int rtos_mutex_rec_init(rt_mutex_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_init_recursive(m);
}

static inline int rtos_mutex_rec_destroy(rt_mutex_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_destroy(m);
}

static inline int rtos_mutex_lock(rt_mutex_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_lock(m, -1);
}

static inline int rtos_mutex_rec_lock(rt_mutex_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_unlock(m);
}

static inline int rtos_mutex_lock_until( rt_mutex_t* m, NANO_TIME abs_time)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

static inline int rtos_mutex_rec_lock_until(rt_mutex_t* m, NANO_TIME abs_time)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_lock(m, nano_abs2rel(abs_time) / 1000);
}

static inline int rtos_mutex_trylock(rt_mutex_t* m)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

static inline int rtos_mutex_rec_trylock(rt_mutex_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_lock(m, 0);
}

static inline int rtos_mutex_unlock(rt_mutex_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_unlock(m);
}

static inline int rtos_mutex_rec_unlock(rt_mutex_t* m)
{
	//printf("%s:  hi.\n", __func__);
	return wrm_mtx_unlock(m);
}

static inline void rtos_enable_rt_warning()
{
	panic("%s:  implement me.\n", __func__);
}

static inline void rtos_disable_rt_warning()
{
	panic("%s:  implement me.\n", __func__);
}

typedef pthread_cond_t rt_cond_t;

static inline int rtos_cond_init(rt_cond_t* cond)
{
	//printf("%s:  implement me.\n", __func__);
	return 0;
	//return pthread_cond_init(cond, NULL);
}

static inline int rtos_cond_destroy(rt_cond_t* cond)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
	//return pthread_cond_destroy(cond);
}

static inline int rtos_cond_wait(rt_cond_t* cond, rt_mutex_t* mutex)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
	//return pthread_cond_wait(cond, mutex);
}

static inline int rtos_cond_timedwait(rt_cond_t* cond, rt_mutex_t* mutex, NANO_TIME abs_time)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
	//TIME_SPEC arg_time = ticks2timespec( abs_time );
	//return pthread_cond_timedwait(cond, mutex, &arg_time);
}

static inline int rtos_cond_broadcast(rt_cond_t* cond)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
	//return pthread_cond_broadcast(cond);
}

#define rtos_printf printf

#ifdef __cplusplus
}
#endif

#endif // __FOSI_H
