/*
MICO Shield for Arduino
by Igor Ramos
http://www.dossant.com/mico

This sketch is to diagnose the MIC polarity right.
It will tell you the mic bias voltage reading and tell y


- Mic bias voltage: signal measurement range -5 to +5. 
It should read +0.5v to +2.0v depending on the phone. 
If it reads a negative value, you must change the MIC POLARITY jumpers (JP9) in the MICO Shield. 
Refer to MICO's docs for details



I'm using the smoothing function that is found at:
  http://arduino.cc/en/Tutorial/Smoothing

Jun 6 2013

*/

int micbiasPin = A1;    // select the input pin for the pot or analog snsr

const int numReadings = 10;

int readings[numReadings];      // the readings from the analog input
int index = 0;                  // the index of the current reading
int total = 0;                  // the running total
int average = 0;                // the average

unsigned int sensorValue = 0;  // variable to store the value coming from the sensor

#define DEADBAND 0.2F // ignore voltages smaller that +/-200mV
#define OFFSET 0.02F  // (optional setting) voltage when unpluuged here to zero out readings

void setup()
{

 // Open serial communications and wait for port to open:
  Serial.begin(9600);

  // initialize all the readings to 0: 
  for (int thisReading = 0; thisReading < numReadings; thisReading++)
    readings[thisReading] = 0; 
    
  Serial.println(F("\nInitializing..."));  

}

unsigned int audiopeak =0;
unsigned int audiovalley=0xffff;
unsigned int AudioInVpp=0;


void loop(void) {
static long int time, task1=0;
float res; 
 
  time = millis();
  

    // subtract the last reading:
  total= total - readings[index];         
  // read from the sensor:  
  readings[index] = analogRead(micbiasPin); 
  // add the reading to the total:
  total= total + readings[index];       
  // advance to the next position in the array:  
  index = index + 1;                    

  // if we're at the end of the array...
  if (index >= numReadings)              
    // ...wrap around to the beginning: 
    index = 0;                           

  // calculate the average:
  average = total / numReadings;         

//  Serial.println(average);   
  delay(1);        // delay in between reads for stability 
  
  
  if (time - task1 > 1000)
  {
    task1 = time;
        
    Serial.print("Mic Bias Voltage:");
 //   sensorValue = analogRead(micbiasPin);   // read the value from the sensor:
    res = mapfloat((float)average, 0,1023,-7.35F+OFFSET,10.00F+OFFSET);
    Serial.print(res); 
    Serial.print("V");
    
    Serial.print("   Diagnostic:");
     
   if (res > DEADBAND)
     Serial.println("mic bias detected. working fine!");
   else if (res < (-1.0F * DEADBAND) )
    Serial.println("mic bias REVERSE polarity. Fix your MICO jumpers.");
   else
    Serial.println("no mic detected. Is cable plugged? Check cable connection & type, jumper setting (JP9)...)");
    
  }
 


  }


  
  
  float mapfloat(float x, float in_min, float in_max, float out_min, float out_max)
  {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
  }
  
  
  



