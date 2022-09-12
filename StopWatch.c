/*
 * StopWatch.c
 *
 *  Created on: 30 Mar 2022
 *      Author: Luke Stock
 */

#include "StopWatch.h"
#include "HPS_Watchdog/HPS_Watchdog.h"
#include "HPS_PrivateTimer/HPS_PrivateTimer.h"
#include "DE1SoC_SevenSeg/DE1SoC_SevenSeg.h"


void initialise_privateTimer( void ) {

    // Configure the ARM Private Timer using functions
    Timer_initialise(0xFFFEC600);      // Timer base address
    Timer_setLoadval(0xFFFFFFFF);      // Set to maximum value
    Timer_setControlvals(224, 0, 1, 1);  // (prescaler, I, E, A): Set clock frequency - prescaler (prescaler = 225-1, as clock rate is 225MHz), disable ISR - 0,  enable automatic overflow - A, enable timer - E

}

// If called by task scheduler, function increments the hundredths value by 1 and resets value to zero beyond 100 calls
unsigned int hundredths (unsigned int hundredths) {

	if (hundredths < 99) {
		hundredths = hundredths + 1;
	}
	else {
		hundredths = 0;
	}

	return hundredths;
}

// If called by task scheduler, function increments the seconds value by 1 and resets value to zero beyond 60 calls
unsigned int seconds (unsigned int seconds) {

	if (seconds < 59) {
		seconds = seconds + 1;
	}
	else {
		seconds = 0;
	}

	return seconds;
}

// If called by task scheduler, function increments the minutes value by 1 and resets value to zero beyond 60 calls
unsigned int minutes (unsigned int minutes) {

	if (minutes < 59) {
		minutes = minutes + 1;
	}
	else {
		minutes = 0;
	}

	return minutes;
}

// If called by task scheduler, function increments the hours value by 1 and resets value to zero beyond 24 calls
unsigned int hours (unsigned int hours) {

	if (hours < 23) {
		hours = hours + 1;
	}
	else {
		hours = 0;
	}

	return hours;

}

// Displays the time on the 7-segment displays
void SevenSegDisplay_Time(unsigned int timeValues[]) {

	unsigned int hours = timeValues[3];

	// If under an hour display MM:SS:FF
	if (hours <= 0) {

		DE1SoC_SevenSeg_SetDoubleDec(0, timeValues[0]);  // hundredths in lowest pair of 7-segment displays
		DE1SoC_SevenSeg_SetDoubleDec(2, timeValues[1]);  // seconds in middle pair of 7-segment displays
		DE1SoC_SevenSeg_SetDoubleDec(4, timeValues[2]);  // minutes in highest pair of 7-segment displays

	}
	// If above an hour display HH:MM:SS
	else {

		DE1SoC_SevenSeg_SetDoubleDec(0, timeValues[1]);  // seconds in lowest pair of 7-segment displays
		DE1SoC_SevenSeg_SetDoubleDec(2, timeValues[2]);  // minutes in middle pair of 7-segment displays
		DE1SoC_SevenSeg_SetDoubleDec(4, timeValues[3]);  // hours in highest pair of 7-segment displays

	}

}

// Reset Timer Values. Set all values for hundredths, seconds, minutes, hours to zero
// Set the lastTimerValue for all the tasks to the current time value
void ResetTimer( unsigned int timeValues[], unsigned int lastTimerValue[], unsigned int taskCount ) {

	unsigned int counter;

	for (counter = 0; counter < taskCount; counter++){
		timeValues[counter] = 0;                             // All time values are eual to zero
		lastTimerValue[counter] = Timer_readCurrentTimer();  // All tasks begin from current time value
	}
}

// Resets key buttons pressed by writing 1 to all bits to clear them
void resetKeyInputs_() {

	*key_ptr = 0xF;

}

 stopwatch() {

	int startup_flag = 1;
	int i;
    const unsigned int taskCount = 4;                                                                // Define number of task counts (hundredths, seconds, minutes, hours)
	unsigned int timeValues[taskCount] = {0};                                                        // Declare stop watch time values to be passed to display
	unsigned int lastTimerValue[taskCount] = {0};                                                    // Last timer values are initialised to zero (hundredths, seconds, minutes, hours)
	const unsigned int period = 1000000;                                                             // Period for 1 sec
	const unsigned int incrementedPeriod[taskCount] = {period/100, period, period*60, period*3600};  // Increment timer periods (hundredths, seconds, minutes, hours)
	TaskFunction taskFunctions[taskCount] = {&hundredths, &seconds, &minutes, &hours};               // Define task Functions to call time increments (hundredths, seconds, minutes, hours)


	while (1) {

        // Check if it is time to run each task
        for (i = 0; i < taskCount; i++) {
            if ((lastTimerValue[i] - Timer_readCurrentTimer()) >=  incrementedPeriod[i]) {
                // When the task is ready to be run, call function, i is param
            	// Current time values are updated to corresponding addresses
            	timeValues[i] = taskFunctions[i](timeValues[i]);
                // To avoid accumulation errors, we make sure to mark the last time
                // the task was run as when we expected to run it. Counter is going
                // down, so subtract the interval from the last time.
                lastTimerValue[i] = lastTimerValue[i] - incrementedPeriod[i];
            }
        }

        // Upon start up timer is not running until a button is pressed to start stop watch
        if (startup_flag == 1) {

        	ResetTimer(timeValues, lastTimerValue, taskCount);  // Reset timer values and push current timer into last increment time
        	Timer_setControlvals(224, 0, 1, 0);                 // Timer is paused at startup, begins once key 0 is pressed
        	startup_flag = 0;                                   // Clears startup_flag to avoid returning to function
        }

        // Start timer on screen
        if (*key_ptr & 0x1) {

           	Timer_setControlvals(224, 0, 1, 1);
        	SevenSegDisplay_Time(timeValues);

        }

        // Stop timer on screen
        if (*key_ptr & 0x2) {

        	SevenSegDisplay_Time(timeValues);                   // Display paused time on 7-segment displays
        	resetKeyInputs();                                   // Reset key buttons pressed
           	Timer_setControlvals(224, 0, 1, 0);                 // Stop the timer values incrementing

        }

        // Reset timer - pauses timer on screen (press KEY 0 to restart timer from 00:00:0) OR press KEY 1 to set time value to 00:00:00)
        if (*key_ptr & 0x4) {

            Timer_setControlvals(224, 0, 1, 0);                 // Timer is paused at startup, begins once key 0 is pressed
        	ResetTimer(timeValues, lastTimerValue, taskCount);  // Reset timer values and push current timer into last increment time
        	resetKeyInputs();                                   // Reset key buttons pressed

        }


        // Make sure we clear the private timer interrupt flag if it is set
        if (Timer_readInterruptFlag() & 0x1) {
            // If the timer interrupt flag is set, clear the flag
        	Timer_resetInterruptFlag();
        }

        // Reset Watchdog timer
        HPS_ResetWatchdog();

	}


}


