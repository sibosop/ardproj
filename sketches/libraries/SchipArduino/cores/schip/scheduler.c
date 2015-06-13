/****************** Task execution scheduler ***********************
 *  for Arduino impl, derived from schip's USBPIC code
 **/

// get the memset prototype
#include <string.h>

// our local stuff
#include "./scheduler.h"

// the list of task items to be executed
Sitem Tasks[USE_NumTasks];

/** initialize scheduler system
 **/
void initScheduler()
{
	uint8_t i;
	memset( Tasks, 0, sizeof(Tasks) );
	for( i=0; i<USE_NumTasks; ++i )
		Tasks[i].ticks = NOENTRY;

	return;
}

/** decrement tick settings as needed
 *  called from timer interrupt
 **/
void decrScheduler()
{
	uint8_t i;
	for( i=0; i<USE_NumTasks; ++i )
	{
		// runs in timer interrupt so doesn't need "atomic" block (?)
		if( (Tasks[i].ticks != NOENTRY) &&
		    (Tasks[i].ticks != RUNENTRY) &&
		    (Tasks[i].ticks != 0) )
		{
		    --Tasks[i].ticks;
		}
	}

	return;
}

/** run the scheduler,
 *    do one pass through the task list,
 *    exec anything with a 0 tick count
 *    (make sure you've posted something to do)
 *    runs in user loop() context
 **/
uint8_t _ti;	// current index in task list, for repost_task() use
void scheduler()
{
	PFV func;
	uint16_t arg;

	for( _ti = 0; _ti < USE_NumTasks; ++_ti )
	{
		// if it's time, do the wine
		//  note: do we need atomic around this test?
		if( Tasks[_ti].ticks == 0 )
		{
			// shut off interrupts to set ticks values
			uint8_t oldSREG = SREG;
			cli();
			Tasks[_ti].ticks = RUNENTRY;
			SREG = oldSREG;   

			func = Tasks[_ti].function;
			arg = Tasks[_ti].arg;

			// execute this task
			(*func)( arg );
			
			// not sure about atomic here...??
			// clear slot if task wasn't reposted
			oldSREG = SREG;
			cli();
			if( Tasks[_ti].ticks == RUNENTRY )
				Tasks[_ti].ticks = NOENTRY;
			SREG = oldSREG;   
		}
	}

	return;
}

/** Insert task into list
 *  Note1: fails silently if no space for task...
 *  Note2: enables interrupts
 **/
void postTask( uint16_t ticks,	// ticks until exec -- 0==now, -1==empty
			   PFV func,		// pointer to function to execute
			   uint16_t arg )	// function argument
{
	uint8_t i;

	for( i=0; i<USE_NumTasks; ++i )
	{
		// find an empty slot and set it
		// NOTE in "atomic" block with no interrupts
		cli();
		if( Tasks[i].ticks == NOENTRY )
		{
			Tasks[i].ticks = ticks;
			sei();
			Tasks[i].function = func;
			Tasks[i].arg = arg;
			return;		//break;
		}
		else
		{
			sei();  // I think ints get enabled automagically on return, but...
		}
	}

	return;
}

/** re-Insert current task into same place in list
 *  should only be called from task executed by scheduler loop
 *  Note1: fails silently and may fubark everything
 *  Note2: enables interrupts
 **/
void repostTask( uint16_t ticks,	// ticks until exec
				 uint16_t arg )		// new argument value
{
	// note: not sure about atomic here...??
    cli();
	Tasks[_ti].ticks = ticks;
    sei();
	Tasks[_ti].arg = arg;
}
