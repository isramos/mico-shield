/*
 SimpleSDAudio absolute minimum example, plays file EXAMPLE.AFM from root folder of SD card (CS on pin 4)
 through speaker/amplifier at pin 9 for ATmega328 / pin 44 for ATmega1280/2560.
 */
#include <SimpleSDAudio.h>
void setup()
{ 
  // SdPlay.setSDCSPin(10); // Enable if your SD card CS-Pin is not at Pin 4... 
  SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO | SSDA_MODE_AUTOWORKER);
  SdPlay.setFile("EXAMPLE.AFM"); 
  SdPlay.play();
}

void loop(void) {
}
