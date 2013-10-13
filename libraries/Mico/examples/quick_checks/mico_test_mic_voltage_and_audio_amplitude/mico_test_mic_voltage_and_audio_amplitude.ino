/*
MICO Shield for Arduino
by Igor Ramos
http://www.HelloMICO.com

This sketch is to assist setting you MICO. It help you in getting the MIC polarity right 
and setting the phone volume.
Sketch will show:
- Peak to peak voltage
- Max voltage
- Min Voltage
- Mic bias voltage: signal measurement range -5 to +5. This is the voltage your phone is applying to the mic.
It should read +0.5v to +1.2v depending on the phone. If it reads a negative value, 
change the MIC POLARITY jumpers in the MICO Shield. Refer to MICO's documentation on how to do that.

Jun 6 2013

*/

int audioinPin = A0;    // select the input pin for the pot or analog snsr
int micbiasPin = A1;    // select the input pin for the pot or analog snsr

unsigned int sensorValue = 0;  // variable to store the value coming from the sensor

void setup()
{
  
  //setup port direction
//  pinMode(micbiasPin, INPUT);  
     
 // Open serial communications and wait for port to open:
  Serial.begin(9600);

  Serial.print(F("\nInitializing..."));  

}

unsigned int audiopeak =0;
unsigned int audiovalley=0xffff;
unsigned int AudioInVpp=0;


void loop(void) {
static long int time, task1=0, timestamp_dtmf_rx=0;
float tmp_var,vpp=0,vrms=0,res; 
int inByte = 0;         // incoming serial byte 
char tmp,data_ready_status=0;
 
  time = millis();
  
//hold max and min audio input voltages
  sensorValue = analogRead(audioinPin);   // read the value from the sensor:
  if (sensorValue > audiopeak)        
    audiopeak = sensorValue;
  else if (sensorValue < audiovalley)
    audiovalley = sensorValue;
    
  AudioInVpp = audiopeak - audiovalley;
  
  if (time - task1 > 1000)
  {
    task1 = time;
        
    Serial.print("\n\rAudio Input: Vpp=");
    vpp = mapfloat((float)AudioInVpp, 0,1023,0.0F,5.0F);
    Serial.print(vpp);  //show audio voltage peak to peak
   // Serial.print(", Vrms=");
   // vrms = vpp * 0.3535; //Vrms = 0.3535 * Vpp
   //  Serial.print(vrms);  //show audio voltage peak to peak
   
   tmp_var = mapfloat((float)audiopeak, 0,1023,0.0F,5.0F);
    Serial.print("V, Vmax:"); Serial.print(tmp_var);  
    
    tmp_var = mapfloat((float)audiovalley, 0,1023,0.0F,5.0F);
    Serial.print("V, Vmin:"); Serial.print (tmp_var); 
    Serial.print("V,   Mic Bias Voltage:");
    sensorValue = analogRead(micbiasPin);   // read the value from the sensor:
    res = mapfloat((float)sensorValue, 0,1023,-7.35F,10.00F);
    Serial.print(res); 
    Serial.print("V");
 
    audiopeak =0; //reset peak
    audiovalley = 0xffff; //reset peak
    
    
  }
 


  }


  
  
  float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
 
