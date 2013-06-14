You need SoX (http://sox.sourceforge.net) to convert audio files into raw format. 

Windows users:
- New: SoX is already included in this library
- Drag and drop your .WAV-files to the appropriate .bat files for conversion
  (in "Arduino with xx MHz" folder)
- Converted files will go to folder "converted"

Linux/Mac users:
- Install SoX by compiling it from source or using your favourite
  package manager
- use the following line for conversion:

sox inputfile.wav --norm=-1 -e unsigned-integer -b 8 -r 31250 -c 1 -t raw outputfile.raw

- For stereo change -c 1 to -c 2
- For full rate use -r 62500 @ 16MHz, -r 31250 @ 8 MHz
- For half rate use -r 31250 @ 16MHz, -r 15625 @ 8 MHz
- For 16-Bit playback (need two PWM outputs for every channel) change -b 8 to -b 16

