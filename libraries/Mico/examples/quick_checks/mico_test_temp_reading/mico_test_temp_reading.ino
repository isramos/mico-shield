/*
MICO Shield for Arduino
by Igor Ramos
http://www.dossant.com/mico

based on code by:
Milan Malesevic and Zoran Stupic 2011

*/


#include <math.h>

enum {
  T_KELVIN=0,
  T_CELSIUS,
  T_FAHRENHEIT
};


#define NTC_100k 4400.0f,298.15f,100000.0f  

void setup() {
 Serial.begin(9600);
}

void loop() {
float tmp;
unsigned int unit,ten,pt;

 Serial.print("Temperature is: ");
 
 Serial.print(Temperature(2,T_CELSIUS,NTC_100k,100000.0f)); Serial.print("degC  ");
 
 Serial.print(Temperature(2,T_FAHRENHEIT,NTC_100k,100000.0f)); Serial.println("degF");

 delay(2000);
}




float Temperature(int AnalogInputNumber,int OutputUnit,float B,float T0,float R0,float R_Balance)
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

