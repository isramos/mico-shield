#ifndef MICO_H
#define MICO_H
 
#include <Arduino.h> //using Arduino 1.0 OR GREATER 

class MICO {
public:
		MICO();
		~MICO();

        float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);
		float Temperature(int AnalogInputNumber,int OutputUnit,float B,float T0,float R0,float R_Balance);

/* DTMF support */
        char dtmf_ascii (char dtmf_raw);
        char dtmf_ascii2dec (char dtmf_raw);
		char DTMF_available(void);
		char DTMF_read(void);
		void data_ready_int(void);
		char dtmf_get_data (void);
/* thermistor support */

		
};
 
#endif