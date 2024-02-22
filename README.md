# Simple Vinyl Simulator for Teensys 4.0

## Prerequisites
Before starting, ensure you have the following installed:

- Arduino IDE 1.8 (recommended) or later versions like 2.2，with "Audio", "SD", "SerialFlash" libraries.
- Python, with `pyserial` and `PyQt5` libraries.
  
## _Implementation steps_
1. Download the file projet.
2. In the file "vinyl_simulator", you can find "vinyl_simulator.ino".
3. Open it in Arduino IDE 1.8 (recommanded), but can be used in Arduino 2.2 ou plus.
4. Compile and upload it to your Teensys 4.0 ( Don't forget to choose the "Teensy 4.0" and corresponded port in the "tools").
5. After successful upload, open the `tourne_disque` directory.
6. Before running `control.py`, ensure you have `pyserial` and `PyQt5` installed in Python. If not, follow the steps below to install these libraries.
7. Enjoy yourself!

### Installing pyserial and PyQt5

Open your terminal or command prompt and execute the following commands:

- **For pyserial:**

  ```bash
  pip install pyserial
  
- **For pyQt5:**

  ```bash
  pip install pyqt5
  
### _Notices_
1. The Sampling Rate of original music should be 1411kbps.
2. Music should be stored in the SD card under the form ".wav".
3. The interface has 3 parameters adjustable    
   "degra" : level of degradation  
   "noise" : level of white noise  
   "gain"  : volume of the output  
   ( It's better to put the "degra" and "noise" in a low level, cause some songs are sensible with this distortion.)
   
