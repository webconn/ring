#ifndef TASKS_H
#define TASKS_H

#include <stdint.h>

/**
 * Main functions
 */

volatile uint8_t flags;

#ifdef DEBUG_NT
volatile uint8_t num_tasks;
#endif

void task_init(void); // init timer and flags
void task_manager(void); // function to run in main program cycle

void task_add(void * func, uint16_t delay); // add task to the queue

void sys_reset(void);

#endif
