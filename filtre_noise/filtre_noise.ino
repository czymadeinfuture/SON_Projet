// Advanced Microcontroller-based Audio Workshop
//
// http://www.pjrc.com/store/audio_tutorial_kit.html
// https://hackaday.io/project/8292-microcontroller-audio-workshop-had-supercon-2015
// 
// Part 1-3: First "Hello World" program, play a music file
//
// WAV files for this and other Tutorials are here:
// http://www.pjrc.com/teensy/td_libs_AudioDataFiles.html


#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "LOWPASS.h"

LOWPASS lowpass;
AudioPlaySdWav           playSdWav1;
AudioOutputI2S           i2s1;
AudioConnection          patchCord1(playSdWav1, 0, lowpass,0); 
AudioConnection          patchCord2(playSdWav1, 1, lowpass, 1); 
AudioConnection          patchCord3(lowpass, 0, i2s1, 0); 
AudioConnection          patchCord4(lowpass, 1, i2s1, 1);
AudioControlSGTL5000     sgtl5000_1;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  7
#define SDCARD_SCK_PIN   14

// Use these with the Teensy 3.5 & 3.6 SD card
//#define SDCARD_CS_PIN    BUILTIN_SDCARD
//#define SDCARD_MOSI_PIN  11  // not actually used
//#define SDCARD_SCK_PIN   13  // not actually used

// Use these for the SD+Wiz820 or other adaptors
//#define SDCARD_CS_PIN    4
//#define SDCARD_MOSI_PIN  11
//#define SDCARD_SCK_PIN   13

const char* songList[] = {"SDTEST1.WAV", "SDTEST2.WAV", "SDTEST3.WAV","SDTEST4.WAV","TEST.WAV"};
int currentSongIndex = 0;  
int songListSize = sizeof(songList) / sizeof(songList[0]);

void playNextSong() {
  if (playSdWav1.isPlaying()) {
    playSdWav1.stop();  
  }

  currentSongIndex++; 
  if (currentSongIndex >= songListSize) {
    currentSongIndex = 0; 
  }

  playSdWav1.play(songList[currentSongIndex]);  
}

void setGain(int gainLevel) {
  float gain = gainLevel / 125.0;  
  sgtl5000_1.volume(gain);  // 
}

void setup() {
  Serial.begin(9600);
  AudioMemory(8);
  sgtl5000_1.enable();
  sgtl5000_1.volume(0.4);
  lowpass.setParamValue("freq",1000);
  lowpass.setParamValue("NoiseLevel",0.05);
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  delay(1000);
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'P' && playSdWav1.isPlaying() == false) {
      //Serial.println("Start playing");
      playSdWav1.play(songList[currentSongIndex]);
      delay(5); // wait for library to parse WAV info
    }
    else if (command == 'S') {
      if (playSdWav1.isPlaying()) {
        playSdWav1.stop();
        //Serial.println("Stopped");
      }
    }
    else if (command == 'G') {
      // wait for receiving the value of gain
      while (Serial.available() < 1) {
        delay(1);
      }
      int gain = Serial.read();
      setGain(gain);
      //Serial.println(gain);
    }
    else if (command == 'N') {
      // wait for receiving the value of gain
      while (Serial.available() < 1) {
        delay(1);
      }
      int noiseread = Serial.read();
      float noise = noiseread / 100.0;
      lowpass.setParamValue("NoiseLevel", noise);
    }
    else if (command == 'X') {
      playNextSong();
    }
  }
}
