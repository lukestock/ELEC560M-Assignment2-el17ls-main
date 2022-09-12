#include <stdbool.h>

#define TIMER_SUCCESS       0
#define TIMER_ERRORNOINIT  -1

signed int Timer_initialise( unsigned int base_address );

bool Timer_isInitialised( void );

signed int Timer_setLoadval( unsigned int timer_load_value );

signed int Timer_setControlvals ( unsigned int prescaler, bool I, bool A, bool E);

signed int Timer_readCurrentTimer ( void );

signed int Timer_readInterruptFlag( void );

signed int Timer_resetInterruptFlag( void );
