/*
MICO Shield for Arduino
by Igor Ramos
http://www.dossant.com/mico

reads the temp of the thermocouple
in degC and degF


6-16-2013

*/

#include <Mico.h>

MICO mico;

void setup() 
{
 Serial.begin(9600);
}

void loop() {

 Serial.print("Temperature is: ");
 
 Serial.print( mico.temp_degC() ); 
 Serial.print("degC  ");
 
 Serial.print( mico.temp_degF() ); 
 Serial.println("degF");

 delay(1000);
}



