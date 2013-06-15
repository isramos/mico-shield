// working just fine - Igor 3-26-2013

/*
based on code by:
Milan Malesevic and Zoran Stupic 2011

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



