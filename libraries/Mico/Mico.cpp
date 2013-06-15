#include "MICO.h" //include the declaration for this class
 
//<<constructor>> setup the LED, make pin 13 an OUTPUT
MICO::MICO(){
  
}

 
//<<destructor>>
MICO::~MICO(){/*nothing to destruct*/}

//variables available to sketch:
char DTMF_RX_BUFFER;


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

char MICO::DTMF_available(void)
{
	return data_ready_status;

}

char MICO::DTMF_read(void)
{
	data_ready_status = 0; 
	return DTMF_RX_BUFFER;
}
void MICO::data_ready_int(void)  //interrupt service routine (no public access needed)
{
      data_ready_status = 1; 
      DTMF_RX_BUFFER = dtmf_get_data(); //read 4 dig ins and put into byte. Need to do now before it vanishes
}
 
char MICO::dtmf_get_data ( void  )  //read data port and assemble the bits into a digit
  {
  // dtmf digit is decoded per Page 5 Table 1 of chip datasheet
  // http://www.zarlink.com/zarlink/mt8870d-datasheet-oct2006.pdf
    char data[4] = {0,0,0,0};
          
    data[0] = digitalRead(d0);
    data[1] = digitalRead(d1);
    data[2] = digitalRead(d2);
    data[3] = digitalRead(d3);
          
    return  ( 8* data[3] +  4* data[2] + 2* data[1] + data[0] ) ;
    
  }
  

