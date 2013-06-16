#include "MICO.h" //include the declaration for this class
 
char data_ready_status=0;
char _DTMF_RX_BUFFER;	

//<<constructor>> setup the LED, make pin 13 an OUTPUT
MICO::MICO(){
  
   // initialize the digital pin as an output.
    pinMode(_d0, INPUT);    
    pinMode(_d1, INPUT); 
    pinMode(_d2, INPUT); 
    pinMode(_d3, INPUT); 
    pinMode(_dr, INPUT);    
	
	  //setup port direction
	pinMode(_answerPin, OUTPUT);  
    digitalWrite(_answerPin, LOW);
	
}
 
//<<destructor>>
MICO::~MICO(){/*nothing to destruct*/}

//variables available to sketch:

float MICO::temp_degC ()
{
	return Temperature(_tempsnrPin,T_CELSIUS,NTC_100k,100000.0f);
}
float MICO::temp_degF (){
	return Temperature(_tempsnrPin,T_FAHRENHEIT,NTC_100k,100000.0f);
}
	
float MICO::mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
  
float MICO::Temperature(int AnalogInputNumber,int OutputUnit,float B,float T0,float R0,float R_Balance)
{
  float R,T;

  R=(1024.0f*R_Balance/float(analogRead(AnalogInputNumber)))-R_Balance;
  T=1.0f/(1.0f/T0+(1.0f/B)*log(R/R0));

  switch(OutputUnit) {
    case T_CELSIUS :
      T-=273.15f;
    break;
    case T_FAHRENHEIT :
      T=9.0f*(T-273.15f)/5.0f+32.0f;
    break;
    default:
    break;
  };

  return T;
}


		
 
// DTMF functions
//public functions
char MICO::dtmf_available(void)
{
	return data_ready_status;
}

char MICO::dtmf_read(void)
{
	data_ready_status = 0; 
	return _DTMF_RX_BUFFER;
} 

//private functions:
void MICO::dtmf_dr_interrupt_service_routine(void)  
{

      _DTMF_RX_BUFFER = dtmf_get_data(); //read dtmf ic 4 digital inputs
      _DTMF_RX_BUFFER = dtmf_ascii(_DTMF_RX_BUFFER);  //convert raw into ascii
	  
	  ring_det_cnt_reset(); //reset ring detector   
	  
	  data_ready_status = 1;  //set flag data ready  
	  
}

char MICO::dtmf_get_data ( void  )  //read data port and assemble the bits into a digit
  {
  // dtmf digit is decoded per Page 5 Table 1 of chip datasheet
  // http://www.zarlink.com/zarlink/mt8870d-datasheet-oct2006.pdf
    char data[4] = {0,0,0,0};
          
    data[0] = digitalRead(_d0);
    data[1] = digitalRead(_d1);
    data[2] = digitalRead(_d2);
    data[3] = digitalRead(_d3);
          
    return  ( 8* data[3] +  4* data[2] + 2* data[1] + data[0] ) ;
    
  }
  
  
char MICO::dtmf_ascii (char dtmf_raw)
{

    switch (dtmf_raw)
    {
    case 10:
      return '0';
    case 11:
        return '*';
    case 12:
        return '#';
    case 13:
        return 'A';
    case 14:
        return 'B';
    case 15:
        return 'C';
    case 0:
        return 'D';
    default:
        return (dtmf_raw+48);
    }
}
  
char MICO::dtmf_ascii2dec (char dtmf_raw)
{
  if ((dtmf_raw >= '0') && (dtmf_raw <= '9'))
    return (dtmf_raw-48);
  else
    return 0xff;
}  
  
float MICO::micBiasVoltage (void)
{
//values -7.35v and 10.0V are selected based on 
// voltage divider in MICO PCB
    return mapfloat((float)analogRead(_micbiasPin), 0,1023,-7.35F,10.00F);;
}  

float MICO::audioInVoltage (void)
{
    return mapfloat((float)analogRead(_audioinPin), 0,1023,0.0F,5.0F);;
}  
  
/* Ring detection support */
unsigned int _ring_cnt=0;
  
bool MICO::is_phone_ringing(void)
{
  long int time=millis();
  static long int rd_last_run=0,rd_last_sig_over_thr=0;

  
  audioIn_SH_run();    //read adc, and keep max and min value  
                       // the ring detect algorithm uses the max-min delta 
  
  if ( (time - rd_last_run) < RD_TASK_RATE)  //default rate = 100ms
    return 0;
    
    rd_last_run = time;
    
 
   if (audioIn_SH_Vpp() > RD_SIG_THR )  //is Sample & Hold value of great amplitude than threshold
   {                                  //i.e. is this a loud enough signal to be considered a phone ring?
    _ring_cnt++;
    rd_last_sig_over_thr = millis();      //elapsed time since last ring-like event
   }

   else if ((time - rd_last_sig_over_thr) > RD_CNT_TOUT)
   {
     _ring_cnt =0;    //reset counter if no loud enough signal within certain time. default=2000ms
   }
  
   audioIn_SH_reset();  //reset S&H
   
   if (_ring_cnt > RD_CNT_THR)
   {
     Serial.println("phone is ringing! ");

     _ring_cnt =0;  //reset counter
     
     return 1;    
   }

   return 0;     //not ringing 
 
}

unsigned int audioIn_ADCmax =0;
unsigned int audioIn_ADCmin=0xffff;

void MICO::audioIn_SH_run (void)
{

  
  unsigned char sensorValue = analogRead(_audioinPin);   // read the value from the sensor:
  
  if (sensorValue > audioIn_ADCmax)
    audioIn_ADCmax = sensorValue;

  else if (sensorValue < audioIn_ADCmin)
    audioIn_ADCmin = sensorValue;
    
  return;
} 
  


/* 
function: audioIn_Vpp_SH

  return the peak-to-peak voltage 
  the stored by Sample and Hold stored since last reset 
  
*/
float MICO::audioIn_SH_Vpp()
{
  unsigned int AudioIn_ADCVpp = audioIn_ADCmax - audioIn_ADCmin;
  
  /* convert raw ADC into Volts */
  return mapfloat((float)AudioIn_ADCVpp, 0,1023,0.0F,5.0F);;
}


void MICO::audioIn_SH_reset (void)
{
  
  audioIn_ADCmax =0;
 
  audioIn_ADCmin=0xffff;
  
  return;
}


void MICO::ring_det_cnt_reset(void)
{
  /* call this when a dtmf signal is decoded
if dtmf received we know phone is not ringing
*/
  _ring_cnt=0;
}

/*
this function is for displaying the ring counter in the main sketch
for debbugging purposes */
int MICO::ring_det_cnt_value(void)
{
  return _ring_cnt;
}

/* Answer/End Call button support */
void MICO::btn_double_press(void)
{
  pinMode(_answerPin, OUTPUT); 
  digitalWrite(_answerPin, HIGH);    //push ANSWER button 
  delay(400); //wait
  
  digitalWrite(_answerPin, LOW);  //release button
  delay(400);  //wait 
 
  digitalWrite(_answerPin, HIGH);  //push btn for 2nd time
  delay(400); //this number 
  
  digitalWrite(_answerPin, LOW); 

  return;
}
void MICO::btn_press(unsigned int psh_dur)
{
	 pinMode(_answerPin, OUTPUT); 
	 digitalWrite(_answerPin, HIGH); 
	 delay(psh_dur); //push answer button for 1000ms
	 digitalWrite(_answerPin, LOW); 
}


