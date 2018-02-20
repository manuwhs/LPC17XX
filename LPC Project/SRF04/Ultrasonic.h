
#include "LPC17xx.h"

#define PULSE_WIDTH     10000		      ///< Pulse width in microseconds
#define PULSE_DELAY   	10000		 ///< Initial pulse delay in microseconds
#define PERIOD_SRF04	100000       ///	< Time between measures of distance (if 0 we only do it once) (50ms)
#define PRESCALER_T0  	24	  	 /// < Frecuency dividor 

void configSRF04(void);
void initPulse(void);
void get_distance(void);

extern uint32_t return_time;
extern uint32_t distance_ready; 
extern uint32_t distance_cm;	/// < The distance in cm
extern uint32_t frecuency;
extern uint32_t speed_sound;
extern uint32_t temperature;
extern uint32_t times_measured; 
