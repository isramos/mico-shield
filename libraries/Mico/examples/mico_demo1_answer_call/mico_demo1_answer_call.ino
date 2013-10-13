/*
MICO Shield for Arduino
by Igor Ramos
http://www.HelloMICO.com

This example monitors the AudioIn and determines if
phone is rining. Then it answers the call and play
welcome message to caller.

Caller uses phone dtmf to remotelly control arduino

This is the sketch used in the kickstarter pitch video
Optionally connect an LED to A3

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
      Serial.print("Mic Bias: ");
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

  if ( mico.is_phone_ringing() )
    {

      answer_call(); //answer and present user with voice prompt
      
    }
    
/* when DTMF signal is available */

  if ( mico.dtmf_available() > 0 ) 
  {
      mico.ring_det_cnt_reset(); //TODO: may make sense to move this to lib    
      
      char inByte = mico.dtmf_read(); //grab buffer & clears flag
   
      Serial.print("DTMF digit received: ");
      Serial.println(inByte); 
  
       action(inByte); //do something with the dtmf code rxd
  
    }
      
/* the section provide system status
  and can help in troubleshooting 
 */

  if (time - task1 > 100)
  {
    task1 = time;
    
    if (time - timestamp_dtmf_rx >1000) //if last dtmf was rx more than 1000ms ago..
    {         
      
     float var = mico.audioIn_SH_Vpp();  //note: S&H only hold peak of last 100ms. If you want to see
                               // value in terminal, make sure you print it fast enough
      if (var >0.05F) // do not show any low amplitude 'noise'
      {
        Serial.print("Audio In Vpp=");
        Serial.print(var);  //show audio voltage peak to peak
   
        if (var > 4.9F)    //lower the volume on the cell phone
            Serial.print("\n\rAudio saturating (>4.9V) "); 
          
      Serial.print(", Ring count=");
      Serial.println( mico.ring_det_cnt_value() );
      }
    }
  }
}

/* what to do when a dtmf code is rxd */
void action (char inByte)
{
  
           if (inByte == '1')
          {
            read_temp();
            say_word (w_deg_f);
          }
          else if (inByte == '2')
          {
            say_word (w_ON);
            digitalWrite(A3, HIGH); //led on
          }
          else if (inByte == '3')
          {
            digitalWrite(A3, LOW); //led off
            say_word (w_OFF);
            
          }
          else if (inByte == '0')
             say_word (w_help);
          else  
            say_word(inByte);
            
}

//TODO: move to library ?
void answer_call(void)
{
  
  mico.btn_press(1000); //push button for 1sec
 
 Serial.println("answering phone");
 delay(1750); //wait a bit before playing welcome message

  say_word (w_welcome);
  say_word (w_for_help);
    
 Serial.println("welcome message played");
  
}

void read_temp(void)
{
float tmp;
char dec_digit;
unsigned int unit,ten,pt;

 Serial.println("Temperature is:");
 Serial.println( mico.temp_degF() );
 

tmp = mico.temp_degF();

 ten = (unsigned int)(tmp/10);   
 unit = ((unsigned int)tmp) - ten*10;
 pt = (unsigned int)(10 * ((tmp -unit) -(ten*10) )  );

 say_word(ten);
 say_word(unit);
 say_word(w_point);
 say_word(pt);
   
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
 
