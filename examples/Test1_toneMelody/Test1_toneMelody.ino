/*
MICO

sketch purpose: play melody on MICO SPK jack, loop indefinetely
6/5/2013
by Igor Ramos - 
http://dossant.com

code based on Tom Igoe's toneMelody example

With assistance from Ben Tesser.
This example code is in the public domain.
 
 */
 
 #include "pitches.h"
int WHOLE = 1,
    HALF = 2,
    QUARTER = 4,
    EIGHTH = 8,
    SIXTEENTH = 16,
    THIRTY_SECOND = 32,
    SIXTY_FOURTH = 64;
int pauseDurations[];

// CONFIGURE BELOW 
// notes in the melody:
int melody[] = {
  NOTE_C4, NOTE_G3,NOTE_G3, NOTE_A3, NOTE_G3,0, NOTE_B3, NOTE_C4};
// The tempo, in beats-per-minute, to play the notes
int bpm = 120;
// note durations: 4 = quarter note, 8 = eighth note, etc.:
int noteDurations[] = { 4, 8, 8, 4,4,4,4,4 };
// The pause between each playing of the melody
int pauseBetweenSongs = 500;

// One of the following 3 options is necessary:
// 1.  Pause Duration - Optional, the amount to pause after each note.  
// pauseDurations[] = { 8, 16, 16, 8, 8, 8, 8, 8 };
// 2.  Pause Percent - deduct this percent of time from each note 
//     to be turned into a pause
int pausePercent = 25;
// 3.  Pause Length - The length of the pause in milliseconds.
//     This will be deducted from each note.  This can also use
//     the getNoteDuration function
// int pauseLength = 2;
// int pauseLength = getNoteDuration(QUARTER);

// Once you have chosen which pause type you want, enter the number
int pauseType = 2;
void setup() {
  // Set the pause and note length for each note
  switch(pauseType){
    case 3: // Set pause length in ms
      for(int i=0; i<noteDurations.length; i++){
        pauseDurations.push(pauseLength);
        noteDurations[i] = getNoteDuration(noteDurations[i]) - pauseLength;
      }
      break;
    case 2: // Set pause length by percent
      int tempNoteDuration;
      for(int i=0; i<noteDurations.length; i++){
        tempNoteDuration = getNoteDuration(noteDurations[i]);
        pauseDurations.push(pausePercent/100 * tempNoteDuration);
        noteDurations[i] = tempNoteDuration * ((100-pausePercent)/100);
      }
      break;
    case 1:
    default: // Use preconfigured pause lengths
      for(int i=0; i<noteDurations.length; i++){
        noteDurations[i] = getNoteDuration(noteDurations[i]);
      }
  }
}
/**
 * @param {integer} noteType The note type (8 for eigth note, 4 for quarter note)
 * @return {integer} The length of the note in ms 
 */
int getNoteDuration(noteType){
  // The multiplier is the difference the note length must be depending on beats per minute.
  int multiplier = bpm/60.00;
  // to calculate the note duration, take one second 
  // divided by the note type.
  //e.g. quarter note = 1000 / 4, eighth note = 1000/8, etc.
  return 1000/(multiplier*noteType);

}
void loop() {
  while(1){
  // iterate over the notes of the melody:
    for (int thisNote = 0; thisNote < 8; thisNote++) {
      // Play the note
      tone(9, melody[thisNote],noteDurations[thisNote]);
      // Delay between notes
      delay(pauseDurations[thisNote]);
      // stop the tone playing:
      noTone(9);
    }
    delay(pauseBetweenSongs);
  }
}
