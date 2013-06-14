/*
 Stereo doorbell example, created 23 Jan 2013 by Lutz Lisseck
 Need SD card properly connected with files DING.AFS and DONG.AFS in root folder.
 Connect button between pin 2 and GND.
 Audio out on 9/10 @ Normal-Arduinos, pin 44/45 @ Mega-Arduinos 
 */
#include <SimpleSDAudio.h>
#define BUTTON  2
void setup()
{ 
  // SdPlay.setSDCSPin(10); // Enable if your SD card CS-Pin is not at Pin 4... 
  SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_STEREO | SSDA_MODE_AUTOWORKER);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop(void) {
  // Wait until button is pressed 
  while(digitalRead(BUTTON) == HIGH) {}
  SdPlay.setFile("DING.AFS"); 
  SdPlay.play();
  delay(200);  // wait a little for minimum Ding sound
  
  // Wait until button is released
  while(digitalRead(BUTTON) == LOW) {}
  SdPlay.setFile("DONG.AFS"); 
  SdPlay.play();  
}
