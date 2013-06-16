#ifndef MICO_H
#define MICO_H
 
#include <Arduino.h> //using Arduino 1.0 OR GREATER 

const int _dr = 2; // dig 2
const int _d0 = A5; // analog 5
const int _d1 = A4; // analog 4
const int _d2 = 5; // dig 5
const int _d3 = 7; // dig 7

const int _audioinPin = A0;    
const int _micbiasPin = A1;   
const int _tempsnrPin = A2;

const int _answerPin = 8;

/* settings for the Ring Detection Algorithm */
#define RD_SIG_THR     4.0F // signals above this threshold will increment counter
#define RD_TASK_RATE   100  // run function every x ms
#define RD_CNT_THR     50
#define RD_CNT_TOUT    2000	// cnt will reset after this time(ms) 



// Support for thermistor
enum {
  T_KELVIN=0,
  T_CELSIUS,
  T_FAHRENHEIT
};

#define NTC_100k 4400.0f,298.15f,100000.0f  

class MICO {
public:
		MICO();
		~MICO();

/* thermistor support */
       	float Temperature(int AnalogInputNumber,int OutputUnit,float B,float T0,float R0,float R_Balance);
		float temp_degC (void);
		float temp_degF (void);

/* DTMF support */
		char dtmf_available(void);				
		char dtmf_read(void);	
        char dtmf_ascii2dec (char dtmf_raw);	//in progress
				
		//private:
		void dtmf_dr_interrupt_service_routine(void);
		char dtmf_get_data (void);
        char dtmf_ascii (char dtmf_raw); 
		
/* misc */		
		float mapfloat(float x, float in_min, float in_max, float out_min, float out_max);

/* Audio In & Mic Bias support */		
		float micBiasVoltage (void);
		float audioInVoltage (void);
		
/* Ring detection support */
		bool is_phone_ringing(void);
		void audioIn_SH_run (void);
		float audioIn_SH_Vpp(void)	;
		void audioIn_SH_reset (void);
		void ring_det_cnt_reset(void);	
		int ring_det_cnt_value(void);
/* Answer/End Call button support */
		void btn_double_press(void);
		void btn_press(unsigned int psh_dur);

};
 
#endif