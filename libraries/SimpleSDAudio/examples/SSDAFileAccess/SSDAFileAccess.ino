/*
 Simple SD Audio example, prints content of TEST.TXT from SD card.
 
 This example shows how to use the SimpleSDAudio library embedded low-level file access.
 You need: 
 - An Arduino with ATmega368 or better
 - An SD-Card connected to Arduinos SPI port (many shields will do)
   -> copy TEST.TXT on freshly formated SD card into root folder
 
 See SimpleSDAudio.h or our website for more information:
 http://www.hackerspace-ffm.de/wiki/index.php?title=SimpleSDAudio
 
 created  19 Jan 2013 by Lutz Lisseck
*/
#include <SimpleSDAudio.h>
#include <sd_l2.h>
#include <sd_l1.h>

// Create static buffer (shared for all file access, because mega328 are quite low on RAM!)
#define BIGBUFSIZE (2*512)      // bigger than 2*512 is often only possible on Arduino megas!
uint8_t bigbuf[BIGBUFSIZE];

SD_L2_File_t TxtFileInfo;  

// helper function to determine free ram at runtime
int freeRam () {
  extern int __heap_start, *__brkval; 
  int v; 
  return (int) &v - (__brkval == 0 ? (int) &__heap_start : (int) __brkval); 
}

void setup()
{
  uint8_t retval;
  
  // Open serial communications and wait for port to open:
  Serial.begin(9600); while (!Serial) {}

  // Setting the buffer manually for more flexibility
  SdPlay.setWorkBuffer(bigbuf, BIGBUFSIZE); 

  // Using F("...") to avoid wasting RAM  
  Serial.print(F("\nInitializing SD card..."));  
  
  // If your SD card CS-Pin is not at Pin 4, enable and adapt the following line:
  // SdPlay.setSDCSPin(10);
  
  if (!SdPlay.init(SSDA_MODE_FULLRATE | SSDA_MODE_MONO)) {
    Serial.println(F("Initialization failed. Error code:"));
    Serial.println(SdPlay.getLastError());
    while(1);
  } 

  Serial.print(F("Free Ram: "));
  Serial.println(freeRam());

  Serial.print(F("Looking for TEST.TXT... "));
  // Search for file TEST.TXT in Rootdir (=cluster 0), search shortname files only (0x00,0x18)
  retval = SD_L2_SearchFile((uint8_t *)"TEST.TXT", 0UL, 0x00, 0x18, &TxtFileInfo);
  if(retval) {
    Serial.println(F(" not found on card! Error code: "));
    Serial.println(retval);
    while(1);
  } else {
   Serial.println(F("found.")); 
   Serial.print(F("First sector: "));
   Serial.print(TxtFileInfo.ActSector);
   Serial.print(F(" Size: "));
   Serial.println(TxtFileInfo.Size);
  }    

  while(TxtFileInfo.ActBytePos < TxtFileInfo.Size) {
    // read one sector
    retval = SD_L1_ReadBlock(TxtFileInfo.ActSector++, &bigbuf[0]);
    if(!retval) {
      uint16_t cnt = 0;
      // print sector content until 512 bytes or file length is reached
      while((cnt < 512) && (TxtFileInfo.ActBytePos < TxtFileInfo.Size)) {
        cnt++;
        TxtFileInfo.ActBytePos++;
        Serial.write(bigbuf[cnt]);
      }
    } else {
      Serial.println(F("Err read sector. Error code: "));
      Serial.println(retval);              
      while(1);
    }
  }
  Serial.println("done.");
}

void loop(void) {
}
