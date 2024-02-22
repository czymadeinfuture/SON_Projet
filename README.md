# Simple Vinyl Simulator for Teensys 4.0
## _Implementation steps_
1. Download the file projet.
2. In the file "vinyl_simulator", you can find "vinyl_simulator.ino".
3. Open it in Arduino IDE 1.8(recommanded), but can be used in Arduino 2.2 ou plus.
4. Compile and upload it to your Teensys 4.0 ( Don't forget to choose the teensy 4.0 and corresponded port in the "tools").
5. If it goes well, open the file "tourne_disque".
6. Run control.py ( If you meet the error, check if you have downloaded pyserial, pyQt5 in python).
7. Enjoy yourself!

### _Notices_
1. The Sampling Rate of original music should be 1411kbps.
2. Music should be stored in the SD card under the form ".wav".
3. The interface has 3 parameters adjustable
   "degra" : level of degradation  
   "noise" : level of white noise  
   "gain"  : volume of the output  
   ( It's better to put the "degra" and "noise" in a low level, cause some songs are sensible with this distortion.)
   
