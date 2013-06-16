/*
MICO Shield for Arduino
by Igor Ramos
http://www.dossant.com/mico

Test receiving DTMF tones using interrupts
Test the mico Library

6-15-2013

 */
#include <Mico.h>

MICO mico;

void setup()
{
    
  Serial.begin(9600);

  Serial.print("Waiting for DTMF tone...");

  attachInterrupt(0, data_ready_int , RISING); //TODO: move to mico lib
}


void loop(void) {   
char DTMF_RX_BUFFER=0; //store data from dtmf rx buffer

  if ( mico.dtmf_available() > 0 ) 
  {  
        
      DTMF_RX_BUFFER = mico.dtmf_read(); //grab buffer & clears flag
      
      Serial.print("DTMF digit received: ");
      Serial.println(DTMF_RX_BUFFER); 
     
  }
  
}

void data_ready_int()    //TODO: move to mico lib
{
      mico.dtmf_dr_interrupt_service_routine();
}
 
