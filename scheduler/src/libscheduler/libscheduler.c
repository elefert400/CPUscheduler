/** @file libscheduler.c
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "libscheduler.h"
#include "../libpriqueue/libpriqueue.h"


/**
  Stores information making up a job to be scheduled including any statistics.

  You may need to define some global variables or a struct to store your job queue elements.
*/
priqueue_t q;
int num_cores;
float waiting_time;
float turnaround_time;
float response_time;
int num_process;
int current_time;
scheme_t s;

//array of cores
job_t cores_arr;
typedef struct _job_t
{
	int id;
	int priority;
	int arrival_time;
	int start_time;
	int running_time;
	int remaining_time;
	int end_time;
} job_t;

struct core_t
{
	bool free;
	job_t *job;
}core_t;

int comparer(const void* a, const void b)
{
	job_t job_a = (job_t*) a;
	job_t job_b = (job_t*) b;

	//compare differences
	int priority_difference = job_a->priority - job_b ->priority;
	int arrival_difference = job_a->arrival_time - job_b->arrival_time;
	int run_differnce = job_a ->running_time - job_b->running_time;
	int remaining_difference = job_a ->remaining_time - job_b->remaining_time;

	//fcfs compare
	if(s == FCFS)
	{
		return arrival_difference;
	}
	//ppri compare
	if(s == PPRI)
	{
		if(priority_difference == 0)
		{
			return arrival_difference;
		}
		return priority_difference;
	}
	//pri compare
	if(s == PRI)
	{
		if(priority_difference == 0)
		{
			return arrival_difference;
		}
		return priority_difference;
	}
	//psjf compare
	if(s == PSJF)
	{
		if(remaining_time == 0)
		{
			return arrival_difference;
		}
		return remaining_difference;
	}
	//RR, nothing to compare
	if(s == RR)
	{
		return 0;
	}
	//sjf compare
	if(s == SJF)
	{
		if(run_differnce == 0)
		{
			return arrival_difference;
		}
		return run_differnce;
	}
	//default
	return 0;
}
/**
  Initalizes the scheduler.

  Assumptions:
    - You may assume this will be the first scheduler function called.
    - You may assume this function will be called once once.
    - You may assume that cores is a positive, non-zero number.
    - You may assume that scheme is a valid scheduling scheme.

  @param cores the number of cores that is available by the scheduler. These cores will be known as core(id=0), core(id=1), ..., core(id=cores-1).
  @param scheme  the scheduling scheme that should be used. This value will be one of the six enum values of scheme_t
*/
void scheduler_start_up(int cores, scheme_t scheme)
{
	num_cores = cores;
	cores_arr = malloc(sizeof(core_t) * num_cores);
	waiting_time = 0.0;
	turnaround_time = 0.0;
	response_time = 0.0;
	num_process = 0;
	current_time = 0;
	s = scheme;

	for(int i = 0; i < cores; i++)
	{
		cores_arr[i] = NULL;
	}
	q = (priqueue_t*)malloc(sizeof(priqueue_t));
	priqueue_init(q, &comparer);
}


/**
  Called when a new job arrives.

  If multiple cores are idle, the job should be assigned to the core with the
  lowest id.
  If the job arriving should be scheduled to run during the next
  time cycle, return the zero-based index of the core the job should be
  scheduled on. If another job is already running on the core specified,
  this will preempt the currently running job.
  Assumptions:
    - You may assume that every job wil have a unique arrival time.

  @param job_number a globally unique identification number of the job arriving.
  @param time the current time of the simulator.
  @param running_time the total number of time units this job will run before it will be finished.
  @param priority the priority of the job. (The lower the value, the higher the priority.)
  @return index of core job should be scheduled on
  @return -1 if no scheduling changes should be made.

 */
int scheduler_new_job(int job_number, int time, int running_time, int priority)
{
	return -1;
}


/**
  Called when a job has completed execution.
  The core_id, job_number and time parameters are provided for convenience. You may be able to calculate the values with your own data structure.
  If any job should be scheduled to run on the core free'd up by the
  finished job, return the job_number of the job that should be scheduled to
  run on core core_id.

  @param core_id the zero-based index of the core where the job was located.
  @param job_number a globally unique identification number of the job.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled to run on core core_id
  @return -1 if core should remain idle.
 */
int scheduler_job_finished(int core_id, int job_number, int time)
{
	return -1;
}


/**
  When the scheme is set to RR, called when the quantum timer has expired
  on a core.

  If any job should be scheduled to run on the core free'd up by
  the quantum expiration, return the job_number of the job that should be
  scheduled to run on core core_id.

  @param core_id the zero-based index of the core where the quantum has expired.
  @param time the current time of the simulator.
  @return job_number of the job that should be scheduled on core cord_id
  @return -1 if core should remain idle
 */
int scheduler_quantum_expired(int core_id, int time)
{
	return -1;
}


/**
  Returns the average waiting time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average waiting time of all jobs scheduled.
 */
float scheduler_average_waiting_time()
{
	if(num_jobs > 0)
	{
		return waiting_time/num_jobs;
	}
	return 0.0;
}


/**
  Returns the average turnaround time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average turnaround time of all jobs scheduled.
 */
float scheduler_average_turnaround_time()
{
	if(num_jobs > 0)
	{
		return turnaround_time / num_jobs;
	}
	return 0.0;
}


/**
  Returns the average response time of all jobs scheduled by your scheduler.

  Assumptions:
    - This function will only be called after all scheduling is complete (all jobs that have arrived will have finished and no new jobs will arrive).
  @return the average response time of all jobs scheduled.
 */
float scheduler_average_response_time()
{
	return 0.0;
}


/**
  Free any memory associated with your scheduler.

  Assumptions:
    - This function will be the last function called in your library.
*/
void scheduler_clean_up()
{
	priqueue_destroy(q);
}


/**
  This function may print out any debugging information you choose. This
  function will be called by the simulator after every call the simulator
  makes to your scheduler.
  In our provided output, we have implemented this function to list the jobs in the order they are to be scheduled. Furthermore, we have also listed the current state of the job (either running on a given core or idle). For example, if we have a non-preemptive algorithm and job(id=4) has began running, job(id=2) arrives with a higher priority, and job(id=1) arrives with a lower priority, the output in our sample output will be:

    2(-1) 4(0) 1(-1)

  This function is not required and will not be graded. You may leave it
  blank if you do not find it useful.
 */
void scheduler_show_queue()
{
	//Fill in if you want I wont be
}
