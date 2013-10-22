/*
MICO Shield for Arduino
by Igor Ramos
http://www.HelloMICO.com

---
This sketch is a end-of-line testing for MICO.
---

This sketch uses work from:
Lutz Lisseck - SimpleSDAudio library
Milan Malesevic and Zoran Stupic - thermistor

10-14-2013

TO DO 1: add time out for TEST 2 and TEST 3. 10-15sec
TO DO 2: review the prompts to look better
TO DO 3: add considering a temp sensor in range check e.g. 0C to 70C
 
 
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
#define dtmf0       108
#define dtmf1       109
#define dtmf2       110
#define dtmf3       111
#define dtmf4       112
#define dtmf5       113
#define dtmf6       114
#define dtmf7       115
#define dtmf8       116
#define dtmf9       117
#define dtmf-str    118
#define dtmf-pnd    119

unsigned char PASS_CNT=0;
unsigned char FAIL_CNT=0;

void setup()
{
  
  Serial.begin(9600);
  
  attachInterrupt(0, data_ready_int , RISING);  //enable dtmd rx int

  // Using F("...") to avoid wasting RAM  
  Serial.print(F("\n---------------------------------------------------------------")); 
  Serial.print(F("\n--- MICO Shield for Arduino - end-of-line test sketch       ---"));  
  Serial.print(F("\n--- Date:Oct/12/2013 - version 1                            ---"));  
  Serial.print(F("\n---------------------------------------------------------------"));  
  Serial.print("\n\r\n\r");
  
     
}

static long int time, task1=0, timestamp_dtmf_rx=0;
unsigned char dtmf_flag=0;

void loop(void) {
  
  time = millis();
  
    card_init();  //initializa card

    test_dtmf();  //play dtmf till rxd  
  
    test_set3(); //test analog temp, mic bias, analog in, button
  
    end_of_test();
  //watch for dtmf

  while( mico.dtmf_available() > 0 )  //empty buffer
  {
          char inByte = mico.dtmf_read(); //grab buffer & clears flag
  }
  Serial.print("\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r\n\r");
  
  
 // ---- push button test -----
//  if  (Serial.available() >0 )
//  {  
//    char inByte = Serial.read();
//    Serial.println("serial in");
//    if (inByte =='1')
//    {
//       Serial.println("...pressing button. Watch your phone...");
//       mico.btn_press(500); 
//    }
//    else if (inByte =='2')
//    { 
//       Serial.println("...double press button. Watch your phone...");
//         mico.btn_double_press(); 
//    }
//  }
 //-------end of push button test ----
 

}

/* what to do when a dtmf code is rxd */
void action (char inByte)
{
  
            
}

//TODO: move to library ?
void answer_call(void)
{
  
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
            else if (say_word == dtmf0) err=SdPlay.setFile("DTMF-0.RAW");
            else if (say_word == dtmf1) err=SdPlay.setFile("DTMF-1.RAW");
			else if (say_word == dtmf2) err=SdPlay.setFile("DTMF-2.RAW");

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
            Serial.print(F("."));
       
}

void data_ready_int()    //TODO: move to mico lib
{
      mico.dtmf_dr_interrupt_service_routine();
}
 
 
void card_init()
{
  Serial.println(F("\n\rTEST 1.")); 
  Serial.print(F("\nInitializing MicroSD card..."));  
  
  // If your SD card CS-Pin is not at Pin 4, enable and adapt the following line:
  // SdPlay.setSDCSPin(10);
  
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO)) {
    Serial.println(F("initialization failed."));
    Serial.print(F("Error code: "));
    Serial.println(SdPlay.getLastError());
    
    Serial.println(F("\n\r >ACTION: Things to check:"));
    Serial.println(F("* is a card is inserted? ")); //error1
    Serial.println(F("* is a card damaged/incompatible/not-formatted?"));  //error8 , error 49

    show_result(0);
    end_of_test();
 //   while(1);
  } else {
   Serial.println(F("Card is present.")); 
  }

  Serial.print(F("Looking for file DTMF-0.raw... "));
  if(!SdPlay.setFile("DTMF-0.raw")) {
    Serial.println(F(" not found on card!"));
    Serial.print(F("Error code: "));
    Serial.println(SdPlay.getLastError());
    Serial.println(F("\n\r >ACTION: Create a MICO test card by copying required files."));
    show_result(0);
    end_of_test();
  //  while(1);

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

  show_result(1);

}

void show_result(unsigned char testres)
{
  if (testres ==1)
  {
    Serial.println(F("\n\r............................................PASSED"));
    PASS_CNT++;
  }
  else
  {
    Serial.println(F("\n\r............................................FAILED"));
    FAIL_CNT++;
  }
}

void test_dtmf()
{
  unsigned char rxdtmf; 
   attachInterrupt(0, data_ready_int , RISING);  //enable dtmd rx int
    
   Serial.println(F("\n\rTEST 2."));

   Serial.println(F("\n\r >ACTION: Add jumper wire between JP5 pin2 to JP10 pin4"));
  
    while( dtmf_flag != 0b00000111  )
    {
      if ( (dtmf_flag&1)!=1 )
      {
        say_word (dtmf0);

      }
      else if ((dtmf_flag&2)!=2 )
      {
        say_word (dtmf1);
      }
      else if((dtmf_flag&4)!=4)
      {
        say_word (dtmf2);
      }
      rxdtmf = check_for_dtmf_in();
      if (rxdtmf == '0') 
        dtmf_flag |= 1 ;
      else if (rxdtmf == '1') 
        dtmf_flag |= 2 ;
      else if (rxdtmf == '2') 
        dtmf_flag |= 4 ;
        
      delay(50);

      
    }
  //play sound
    dtmf_flag =0;
    
    show_result(1);
    Serial.println(F(" >ACTION: Remove jumper wire"));
  
}

void test_set3()
{  //test analog temp, mic bias, analog in, button
unsigned char seq=0,pass_test=0;  
unsigned int an0,an1,an2;
unsigned int an0b,an1b,an2b;

pinMode(8, OUTPUT);

  Serial.println(F("\n\rTEST 3."));
 
   an2 = analogRead(A2);
   
   if (an2 > 100) //wire not present 
   {
     Serial.println(F("\n\r >ACTION: Add jumper wire between JP9 & User Header"));
     seq=1;
   }
   
  while(pass_test==0)
  {
    //sample sensor with & without pushbutton
    an0 = analogRead(A0);       an1 = analogRead(A1);      an2 = analogRead(A2);  
    digitalWrite(8, HIGH);    delay(200);   
    an0b = analogRead(A0);       an1b = analogRead(A1);      an2b = analogRead(A2);     
    digitalWrite(8, LOW); 
    
      if ((an2 > an2b)  && (an2 < 100) )// if after 'push'result read lower
      {
        if ( ((an2 - an2b) > 5) &&  ((an2 - an2b) < 100) ) //difference at least 5 counts
        {
          if ((an1 - an1b) > 2)
          {
           pass_test=1;
           Serial.print("  TempSnsr = " );  Serial.print(an2);    
           Serial.print(" - " );  Serial.print(an2b);  
           Serial.print(" = "); 
           Serial.println(an2 - an2b);   
          }
        }
      }
    Serial.print("."); 
    delay(200); 
  
  }
    if (seq==0)
    {
      Serial.println(F("\n\r >ACTION: REMOVE jumper between JP9 & User Header"));
      while (analogRead(A2) < 100)
      {
        Serial.print("."); 
        delay(200); 
      }
    }
   
  show_result(1);

}

void end_of_test()
{
  
  Serial.print("\n\rTest time:"); Serial.print(millis()- time);   Serial.print("ms");
  Serial.print("\n\r----------------==== END OF TEST ====---------- FINAL RESULT:");
  
  pinMode(2, OUTPUT);  //make DTR an output forcing LED on
    
  if (PASS_CNT ==3)
  {
      Serial.println(F("PASS"));
      while(1)
      {
        digitalWrite(2, HIGH); 
        delay(250); 
        digitalWrite(2, LOW); 
        delay(250); 
      }
  }
  else 
  {
      Serial.println(F("FAIL"));
      digitalWrite(2, HIGH);
 
  }
    
     while(1);

  
  Serial.print("\n\r \n\rrestarting in...");
   
    Serial.print("3...");
    delay(1000);
    Serial.print("2...");
    delay(1000);
    Serial.print("1...");
    delay(1000);
}

unsigned char check_for_dtmf_in()
{
  char inByte = 0xFF;
  if ( mico.dtmf_available() > 0 ) 
  {
      mico.ring_det_cnt_reset(); //TODO: may make sense to move this to lib    
      
      inByte = mico.dtmf_read(); //grab buffer & clears flag
   
      Serial.print("DTMF digit received: ");
      Serial.println(inByte); 
  
      // action(inByte); //do something with the dtmf code rxd

    }
    return(inByte);
}


void push_to_start()
{
  Serial.println("\n\rpush button to restart");
  
  pinMode(8, INPUT); 
  
  while(1){
    if ( digitalRead(8) == 1)
    {
      delay(200);
      if ( digitalRead(8) == 1 )
      {
         Serial.print( "btn pressed" );
        break;
      }
    }
  }  
  
  pinMode(8, OUTPUT);
  digitalWrite(8, LOW); 
 }
