#include "HPS_PrivateTimer.h"

//Driver Base Addresses
volatile unsigned int   *timer_base_ptr     = 0x0;  //0xFFFEC600

//Driver Initialised
bool timer_initialised = false;

//Register Offsets
#define TIMER_LOAD      (0x00/sizeof(unsigned int))
#define TIMER_VALUE     (0x04/sizeof(unsigned int))
#define TIMER_CONTROL   (0x08/sizeof(unsigned int))
#define TIMER_INTERRUPT (0x0C/sizeof(unsigned int))

//Function to initialise the Timer
signed int Timer_initialise( unsigned int base_address ){

    //Initialise base address pointers
    timer_base_ptr = (unsigned int *)  base_address;

    //Ensure timer initialises to disabled
    timer_base_ptr[TIMER_CONTROL] = 0;

    //Timer now initialised
    timer_initialised = true;

    return TIMER_SUCCESS;

}

//Check if driver initialised
bool Timer_isInitialised() {

    return timer_initialised;

}

//Function to initialise the Timer Load Value
signed int Timer_setLoadval( unsigned int timer_load_value ) {

	// If timer is NOT initialised return error
	if (!timer_initialised) return TIMER_ERRORNOINIT;

    timer_base_ptr[TIMER_LOAD] = timer_load_value;

    return TIMER_SUCCESS;
}

//Function to initialise the Timer Control Values
signed int Timer_setControlvals( unsigned int prescaler, bool I, bool A, bool E) {

		// If timer is NOT initialised return error
		if (!timer_initialised) return TIMER_ERRORNOINIT;

	    timer_base_ptr[TIMER_CONTROL] = (prescaler << 8 | I << 2 | A << 1 | E << 0 );

	    return TIMER_SUCCESS;

}

//Function to read Current Timer Value
signed int Timer_readCurrentTimer() {

	return timer_base_ptr[TIMER_VALUE];

}


//Function to read the Interrupt flag
signed int Timer_readInterruptFlag() {

	return timer_base_ptr[TIMER_INTERRUPT];

}

//Function to reset the Interrupt flag
signed int Timer_resetInterruptFlag() {

	// If timer is NOT initialised return error
	if (!timer_initialised) return TIMER_ERRORNOINIT;

	timer_base_ptr[TIMER_INTERRUPT] = 0x1;

	return TIMER_SUCCESS;

}
