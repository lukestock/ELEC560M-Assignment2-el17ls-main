/*
 * StopWatch.h
 *
 *  Created on: 30 Mar 2022
 *      Author: lukes
 */

#ifndef STOPWATCH_H_
#define STOPWATCH_H_

void initialise_privateTimer( void );

// Define bass addresses
volatile unsigned int *key_ptr = (unsigned int *)0xFF20005C;  // base address for rising edge of key press

//Define new data type for function which takes an int parameter and returns updated int values.
typedef unsigned int (*TaskFunction)( unsigned int);

// Define task scheduler function to update hundredths
unsigned int hundredths ( unsigned int );

// Define task scheduler function to update seconds
unsigned int seconds ( unsigned int );

// Define task scheduler function to update minutes
unsigned int minutes ( unsigned int );

// Define task scheduler function to update hours
unsigned int hours ( unsigned int );

// Define function to display time on 7-segment display
void SevenSegDisplay_Time( unsigned int timeValues[] );

// Define reset timer values to zero and update lastTimerValue with current time values.
void ResetTimer( unsigned int timeValues[], unsigned int lastTimerValue[], unsigned int );

// Define reset key buttons pressed
void resetKeyInputs(void);

#endif /* STOPWATCH_H_ */
