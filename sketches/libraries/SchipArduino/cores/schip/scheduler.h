/** scheduler.h -- defines for interrupt driven Task execution scheduler
 * 					for Arduino impl.
 *   Derived from schip's USBPIC code...
 **/

#ifndef SCHEDULER_H
#define SCHEDULER_H

#include <inttypes.h>

#ifdef __cplusplus
extern "C"{
#endif

// default max number of tasks in list is USE_NumTasks not already defined
#ifndef USE_NumTasks
#define USE_NumTasks 8
#endif

// control structure for a scheduled item
typedef void(*PFV)(uint16_t);	// pointer to a function for task list
typedef struct
{
	// global interface values
	uint16_t ticks;		// ticks until exec -- 0==now, -1==empty
	PFV function;		// pointer to function to execute
	uint16_t arg;		// function argument
} Sitem;

#define NOENTRY		0xFFFF	// empty entry
#define RUNENTRY	0xFFFE	// currently running entry
#define MAXTIME		0xFFFD	// maximum count we can set

// in scheduler.c
extern Sitem Tasks[]; // list of items -- size will be set by USE_NumTasks
extern uint8_t _ti;	// current index of executing task
// internal entry points
extern void initScheduler(void);	// inititalize
extern void decrScheduler(void);	// for interrupt routine

// user access points:
//  main entry point to be called in loop()
extern void scheduler(void);
//  post a function to be executed after ticks time with given arg
extern void postTask( uint16_t ticks, PFV func, uint16_t arg );
//  re-Insert current task into same place in list with given time & arg
extern void repostTask( uint16_t ticks, uint16_t arg );

// and some additional function for wiring_analog.c if we want it
extern uint8_t analogReadAvg(uint8_t pin);
extern void ADCTask( uint16_t numADC );

#ifdef __cplusplus
} // extern "C"
#endif

#endif // SCHEDULER_H
