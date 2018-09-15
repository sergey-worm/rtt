/***************************************************************************
  tag: Sergey Worm  Tue Sep 11 14:53:49 MSK 2018  fosi_internal.hpp

                        fosi_internal.hpp -  description
                           -------------------
    begin                : Tue Sep  11 2018
    copyright            : (C) 2018 Sergey Worm
    email                : sergey.worm@gmail.com

    ***************************************************************************
    *   This library is free software; you can redistribute it and/or         *
    *   modify it under the terms of the GNU General Public                   *
    *   License as published by the Free Software Foundation;                 *
    *   version 2 of the License.                                             *
    *                                                                         *
    *   As a special exception, you may use this file as part of a free       *
    *   software library without restriction.  Specifically, if other files   *
    *   instantiate templates or use macros or inline functions from this     *
    *   file, or you compile this file and link it with other files to        *
    *   produce an executable, this file does not by itself cause the         *
    *   resulting executable to be covered by the GNU General Public          *
    *   License.  This exception does not however invalidate any other        *
    *   reasons why the executable file might be covered by the GNU General   *
    *   Public License.                                                       *
    *                                                                         *
    *   This library is distributed in the hope that it will be useful,       *
    *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
    *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU     *
    *   Lesser General Public License for more details.                       *
    *                                                                         *
    *   You should have received a copy of the GNU General Public             *
    *   License along with this library; if not, write to the Free Software   *
    *   Foundation, Inc., 59 Temple Place,                                    *
    *   Suite 330, Boston, MA  02111-1307  USA                                *
    *                                                                         *
    ***************************************************************************/

#include "../ThreadInterface.hpp"
#include "../fosi_internal_interface.hpp"
#include "fosi.h"
#include "../../Logger.hpp"
#include <cassert>
#include <sys/time.h>
#include <sys/resource.h>
#include <iostream>
#include <cstdlib>
#include <sys/types.h>
#include <unistd.h>
#include <sys/syscall.h>

#include "wrm_mpool.h"
#include "wrm_thr.h"
#include "wrm_log.h"

using namespace std;

#define INTERNAL_QUAL

namespace RTT
{
namespace os
{
INTERNAL_QUAL int rtos_task_create_main(RTOS_TASK* main_task)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

INTERNAL_QUAL int rtos_task_delete_main(RTOS_TASK* main_task)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

INTERNAL_QUAL void* rtos_posix_thread_wrapper(void* cookie)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

INTERNAL_QUAL int rtos_task_create(RTOS_TASK* task,
                                   int priority,
                                   unsigned cpu_affinity,
                                   const char * name,
                                   int sched_type,
                                   size_t stack_size,
                                   void* (*start_routine)(void*),
                                   ThreadInterface* obj)
{
	//printf("%s:  hi - prio=%d, cpu=%d, name=%s, sched=%d, stack_sz=%zu, routine=0x%p.\n",
	//	__func__, priority, cpu_affinity, name, sched_type, stack_size, start_routine);

	task->wait_policy = ORO_WAIT_REL; // ORO_WAIT_ABS;
	task->priority = priority;

	if (!strlen(name))
		name = "Thread";
	strncpy(task->name, name, sizeof(task->name));
	task->name[sizeof(task->name)-1] = '\0';

	stack_size = round_pg_up(stack_size);
	if (!stack_size)
		stack_size = Cfg_page_sz;
	L4_fpage_t stack_fp = wrm_pgpool_alloc(stack_size);
	L4_fpage_t utcb_fp = wrm_pgpool_alloc(Cfg_page_sz);
	assert(!stack_fp.is_nil());
	assert(!utcb_fp.is_nil());
	int rc = wrm_thr_create(utcb_fp, (L4_thread_func_t)start_routine, (long)obj, stack_fp.addr(),
	                        stack_fp.size(), 255, task->name, Wrm_thr_flag_fpu, &task->thr);
	//wrm_logi("create_thread:  rc=%d, id=%u.\n", rc, task->thr.number());
	assert(!rc && "failed to create driver thread");

	if (cpu_affinity != (unsigned)~0)
	{
		log(Debug) << "Setting CPU affinity to " << cpu_affinity << endlog();
		int result = rtos_task_set_cpu_affinity(task, cpu_affinity);
		if (result != 0)
			log(Error) << "Failed to set CPU affinity to " << cpu_affinity << " for "
				<< task->name << ": " << strerror(result) << endlog();
	}
	return rc;
}

INTERNAL_QUAL void rtos_task_yield(RTOS_TASK* t)
{
	panic("%s:  implement me.\n", __func__);
}

INTERNAL_QUAL unsigned int rtos_task_get_pid(const RTOS_TASK* task)
{
	//printf("%s:  hi - task=%s.\n", __func__, task->name);
	if (task)
		return task->thr.number();
	return 0;
}

INTERNAL_QUAL int rtos_task_is_self(const RTOS_TASK* task)
{
	panic("%s:  implement me.\n", __func__);
    return 1;
}

INTERNAL_QUAL int rtos_task_set_scheduler(RTOS_TASK* task, int sched_type)
{
	//printf("%s:  hi - task=%s.\n", __func__, task->name);
	return 0;
}

INTERNAL_QUAL int rtos_task_get_scheduler(const RTOS_TASK* task)
{
	//printf("%s:  hi - task=%s.\n", __func__, task->name);
	return 0;
}

INTERNAL_QUAL void rtos_task_make_periodic(RTOS_TASK* mytask, NANO_TIME nanosecs)
{
	//printf("%s:  hi - task=%s, nsec=%llu.\n", __func__, mytask->name, nanosecs);
	// set period
	mytask->period = nanosecs;
	// set next wake-up time.
	mytask->periodMark = ticks2timespec(nano2ticks(rtos_get_time_ns() + nanosecs));
}

INTERNAL_QUAL void rtos_task_set_period(RTOS_TASK* mytask, NANO_TIME nanosecs )
{
	rtos_task_make_periodic(mytask, nanosecs);
}

INTERNAL_QUAL void rtos_task_set_wait_period_policy(RTOS_TASK* task, int policy)
{
	panic("%s:  implement me.\n", __func__);
	task->wait_policy = policy;
}

INTERNAL_QUAL int rtos_task_wait_period(RTOS_TASK* task)
{
	//wrm_logd("%s:  hi - task=%s, period=%llu ms, wakeup=%llu ms.\n", __func__, task->name,
	//	task->period/1000000, (task->periodMark.tv_sec * 1000000000ll + task->periodMark.tv_nsec)/1000000);

	if (!task->period)
		return 0;

	// record this to detect overrun
	NANO_TIME now  = rtos_get_time_ns();
	NANO_TIME wake = task->periodMark.tv_sec * 1000000000LL + task->periodMark.tv_nsec;

	usleep(nano_abs2rel(wake) / 1000);

	if (task->wait_policy == ORO_WAIT_ABS)
	{
		// in the case of overrun by more than 4 periods,
		// skip all the updates before now, with the next update aligned to period
		int maxDelayInPeriods = 4;
		NANO_TIME period = task->period;
		if (now - wake > maxDelayInPeriods*period) {
			period = period * ((now - wake) / period);
		}
		// program next period:
		// 1. convert period to timespec
		TIME_SPEC ts = ticks2timespec(nano2ticks(nano2ticks(period)));
		// 2. Add ts to periodMark (danger: tn guards for overflows!)
		NANO_TIME tn = (task->periodMark.tv_nsec + ts.tv_nsec);
		task->periodMark.tv_nsec = tn % 1000000000LL;
		task->periodMark.tv_sec += ts.tv_sec + tn / 1000000000LL;
	}
	else
	{
		TIME_SPEC ts = ticks2timespec( nano2ticks( task->period) );
		//printf("REL:  rtos_get_time_ns=%llu.\n", rtos_get_time_ns());
		//printf("REL:  period:    timespec={%u; %u}.\n", ts.tv_sec, ts.tv_nsec);
		TIME_SPEC now = ticks2timespec(nano2ticks(rtos_get_time_ns()));
		//printf("REL:  now:       timespec={%u; u}.\n", now.tv_sec, now.tv_nsec);
		NANO_TIME tn = (now.tv_nsec + ts.tv_nsec);
		//printf("REL:  new nano:  %llu.\n", tn);
		task->periodMark.tv_nsec = tn % 1000000000LL;
		//printf("REL:  new_nsec:  %u.\n", task->periodMark.tv_nsec);
		task->periodMark.tv_sec = ts.tv_sec + now.tv_sec + tn / 1000000000LL;
		//printf("REL:  new_sec:   %u.\n", task->periodMark.tv_sec);
	}

	//wrm_logd("%s:  hi - task=%s, next wakeup=%llu ms.\n", __func__, task->name,
	//	(task->periodMark.tv_sec * 1000000000ll + task->periodMark.tv_nsec)/1000000);

	return now > wake ? -1 : 0;
}

INTERNAL_QUAL void rtos_task_delete(RTOS_TASK* mytask)
{
	//printf("%s:  hi - task=%s, todo me.\n", __func__, mytask->name);

	// delete thread
	long term_code = 0;
	int term_state;
	L4_fpage_t utcb;
	addr_t stack;
	size_t stack_sz;
	int rc = wrm_thr_delete(mytask->thr, &term_code, &term_state, &utcb, &stack, &stack_sz);
	//wrm_logi("%s:  wrm_thr_delete() - rc=%d, id=%u, stack=0x%lx/0x%zx, utcb=0x%lx, tcode=%ld, tstate=%d.\n",
	//	__func__, rc, mytask->thr.number(), stack, stack_sz, utcb.addr(), term_code, term_state);
	if (rc)
		panic("%s:  wrm_thr_delete() - rc=%d, id=%u, stack=0x%lx/0x%zx, utcb=0x%lx, tcode=%ld, tstate=%d.\n",
			__func__, rc, mytask->thr.number(), stack, stack_sz, utcb.addr(), term_code, term_state);
	assert(stack);
	assert(is_pg_aligned(stack_sz));
	assert(!utcb.is_nil());

	// free memory
	wrm_pgpool_add(utcb);
	wrm_pgpool_add(L4_fpage_t::create(stack, stack_sz, Acc_rw));
}

INTERNAL_QUAL int rtos_task_check_scheduler(int* scheduler)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

INTERNAL_QUAL int rtos_task_check_priority(int* scheduler, int* priority)
{
	panic("%s:  implement me.\n", __func__);
	return 0;
}

INTERNAL_QUAL int rtos_task_set_priority(RTOS_TASK* task, int priority)
{
	panic("%s:  implement me.\n", __func__);
	return -1;
}

INTERNAL_QUAL int rtos_task_get_priority(const RTOS_TASK* task)
{
	//printf("%s:  hi - task=%s.\n", __func__, task->name);
	return task->priority;
}

INTERNAL_QUAL int rtos_task_set_cpu_affinity(RTOS_TASK* task, unsigned cpu_affinity)
{
	//printf("%s:  hi, todo me.\n", __func__);
	return 0;
}

INTERNAL_QUAL unsigned rtos_task_get_cpu_affinity(const RTOS_TASK* task)
{
	//printf("%s:  hi - task=%s.\n", __func__, task->name);
	return 0;
}

INTERNAL_QUAL const char* rtos_task_get_name(const RTOS_TASK* task)
{
	return task->name;
}

} // namespace os
} // namespace RTT
#undef INTERNAL_QUAL
