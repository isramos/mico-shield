/*
MICO Shield for Arduino
by Igor Ramos
http://www.dossant.com/mico

This sketch will place a call to the last dialed number and 
report temp in Fahrenheit.

on your phone press 0 to change temperature to celcius
press 1 to change back to Fahrenheit


This sketch uses work from:
Lutz Lisseck - SimpleSDAudio library
Milan Malesevic and Zoran Stupic - thermistor


6-15-2013
 
 
*/
#include <SimpleSDAudio.h>
#include <math.h>
#include <Mico.h>

MICO mico;

/* these are the words mico can say */
#define w_ON        100
#define w_OFF       101
#define w_for_help  102
#define w_welcome   103
#define w_deg_c     104 
#define w_deg_f     105
#define w_help      106
#define w_point     107


const int ledPin = A3;   //this is an optional connection for this demo sketch

char tmp_req_units = 1; //store requested units for temp 1=F 0=Celcius

void setup()
{
  
  pinMode(ledPin, OUTPUT);
  digitalWrite(ledPin, LOW); 
  
  Serial.begin(9600);

  // Using F("...") to avoid wasting RAM  
  Serial.print(F("\nInitializing SD card..."));  
  
  // If your SD card CS-Pin is not at Pin 4, enable and adapt the following line:
  // SdPlay.setSDCSPin(10);
  
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO)) {
    Serial.println(F("initialization failed. Things to check:"));
    Serial.println(F("* is a card is inserted?"));
    Serial.println(F("* Is your wiring correct?"));
    Serial.println(F("* maybe you need to change the chipSelect pin to match your shield or module?"));
    Serial.print(F("Error code: "));
    Serial.println(SdPlay.getLastError());
    while(1);
  } else {
   Serial.println(F("Wiring is correct and a card is present.")); 
  }

  Serial.print(F("Looking for welcome.raw... "));
  if(!SdPlay.setFile("welcome.raw")) {
    Serial.println(F(" not found on card! Error code: "));
    Serial.println(SdPlay.getLastError());
    while(1);
  } else {
   Serial.println(F("found.")); 
  }    

  Serial.print(F("Playing... ")); 
  SdPlay.play();
  while(!SdPlay.isStopped()) {
    SdPlay.worker();
  }
  Serial.println(F("done."));
//  SdPlay.deInit();


/* help diagnose mic */
      Serial.print("\n\rMic Bias: ");
      float var = mico.micBiasVoltage();
 
      if (var >0.25F)
         Serial.print("OK");
      else if (var < -0.25F)
         Serial.print("Reversed");    //set JP9 on mico board
      else
         Serial.print("no mic found"); //check connection, 4-pole cable, jumper
      Serial.print('(');
      Serial.print(var);  
      Serial.println("V)");

    attachInterrupt(0, data_ready_int , RISING);  //required for dtmf rx interrupt
}


void loop(void) {
static long int time, task1=0, timestamp_dtmf_rx=0;
 
  time = millis();

  //wait for serial
  Serial.println("\n\rthis code will redial last dialed number");
  Serial.println("Press any key to continue...");

  while (  Serial.available() ==0 );
  
  Serial.println("ok...will double press button. Watch your phone...");
  
  mico.btn_double_press(); 
  
  delay(2500); //wait till call is completed 
   
  while(1)
  {
    time = millis();
    // if ringing answer call
    if (time - task1 > 1500)
    {
      task1 = time;
      
      read_temp(tmp_req_units); //1==F else C 
    }
    
/* when DTMF signal is available */

  if ( mico.dtmf_available() > 0 ) 
  {      
      char inByte = mico.dtmf_read(); //grab buffer & clears flag
   
      Serial.print("DTMF digit received: ");
      Serial.println(inByte); 
      
      if (inByte == '1')
        tmp_req_units=1;
      else if (inByte == '0')
        tmp_req_units=0;

    }
    
   }
  
}  

void read_temp(char tmp_unit)
{
float tmp;
char dec_digit;
unsigned int unit,ten,pt;

  if (tmp_unit == 1)
    tmp = mico.temp_degF();
  else 
    tmp = mico.temp_degC();

  Serial.println("Temperature is:"); 
  Serial.println( tmp );
 
 ten = (unsigned int)(tmp/10);   
 unit = ((unsigned int)tmp) - ten*10;
 pt = (unsigned int)(10 * ((tmp -unit) -(ten*10) )  );

 say_word(ten);
 say_word(unit);
 say_word(w_point);
 say_word(pt);
 
  if (tmp_unit==1)
   say_word (w_deg_f);
 else
   say_word (w_deg_c);
}



void say_word(char say_word)
{
char err;
            if      (say_word == 0) err=SdPlay.setFile("0.RAW");
            else if (say_word == 1) err=SdPlay.setFile("1.RAW");
            else if (say_word == 2) err=SdPlay.setFile("2.RAW");
            else if (say_word == 3) err=SdPlay.setFile("3.RAW");
            else if (say_word == 4) err=SdPlay.setFile("4.RAW");
            else if (say_word == 5) err=SdPlay.setFile("5.RAW");
            else if (say_word == 6) err=SdPlay.setFile("6.RAW");
            else if (say_word == 7) err=SdPlay.setFile("7.RAW");
            else if (say_word == 8) err=SdPlay.setFile("8.RAW");
            else if (say_word == 9) err=SdPlay.setFile("9.RAW");
            else if (say_word == w_point )err=SdPlay.setFile("p.RAW");
            else if (say_word == w_ON) err=SdPlay.setFile("on.RAW");
            else if (say_word == w_OFF) err=SdPlay.setFile("off.RAW");
            else if (say_word == w_welcome) err=SdPlay.setFile("welcome.RAW");
            else if (say_word == w_for_help) err=SdPlay.setFile("for_help.RAW");
            else if (say_word == w_deg_f) err=SdPlay.setFile("deg_f.RAW");
            else if (say_word == w_deg_c) err=SdPlay.setFile("deg_c.RAW");
            else if (say_word == w_help) err=SdPlay.setFile("help.RAW");
            
            else
            {  
              Serial.println(F("Error: word not in list"));
              return;
            }
            
           if (!err){
               Serial.println(SdPlay.getLastError());
               return;  
           }
         
            SdPlay.play();
            
            while(!SdPlay.isStopped()) {
              SdPlay.worker();
            }
            Serial.println(F("done."));
       
}

void data_ready_int()    //TODO: move to mico lib
{
      mico.dtmf_dr_interrupt_service_routine();
}
 
