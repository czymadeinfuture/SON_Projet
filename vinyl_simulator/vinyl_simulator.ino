#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "clicks.h"
#include "thrumps.h"


AudioPlaySdWav           playWav1;
AudioOutputI2S           i2s1;
AudioMixer4              mixer1; // mixer for the left channel
AudioMixer4              mixer2; // mixer for the right channel      
clicks                   click;
thrumps                  thrump;        

// connect playWav1, click and thrump to the mixer

AudioConnection          patchCord1(playWav1, 0, mixer1, 0);
AudioConnection          patchCord2(click, 0, mixer1, 1);  
AudioConnection          patchCord3(thrump, 0, mixer1, 2);

AudioConnection          patchCord4(playWav1, 1 , mixer2, 0);
AudioConnection          patchCord5(click, 0, mixer2, 1);
AudioConnection          patchCord6(thrump, 0, mixer2, 2);

AudioConnection          patchCord7(mixer1, 0, i2s1, 0);
AudioConnection          patchCord8(mixer2, 0, i2s1, 1);

AudioControlSGTL5000     sgtl5000_1;

unsigned long musicPlaybackPosition = 0;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13  


void setup() {
  Serial.begin(9600);
  AudioMemory(20);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
     while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  
  
  mixer1.gain(0, 0.0); // set the gain for the playWav1
  mixer1.gain(1, 2000.0); // set the gain for the click
  mixer1.gain(2, 3000.0); // set the gain for the thrump
  
  mixer2.gain(0, 0.0); // set the gain for the playWav1
  mixer2.gain(1, 2000.0); // set the gain for the click
  mixer2.gain(2, 3000.0); // set the gain for the thrump 
  
}

void playFile(const char *filename){
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(25);

  Serial.println(playWav1.isPlaying() ? "File is playing" : "File is not playing");
  
  int thrump_num = 1;
  while (playWav1.isPlaying()) {
    musicPlaybackPosition = playWav1.positionMillis();
    // If it is the time to play the click or the thrump, set the play
    if (musicPlaybackPosition >= click.click_index * 1000 && musicPlaybackPosition <= click.click_index * 1000 + 5) {
      click.Setplay();
      Serial.println("Click started at " + String(musicPlaybackPosition));
    };
    if (musicPlaybackPosition >= thrump.gap * 1000 * thrump_num && musicPlaybackPosition <= thrump.gap * 1000 * thrump_num + 10) {
      thrump.Setplay();
      Serial.println("Thrump started at " + String(musicPlaybackPosition));
      thrump_num++;
    };
  }
  
  // When the file is finished (playWav1.isPlaying() == false)
  Serial.println("File has finished playing");
  
  // Stop the click and thrump
  thrump.Setplay();
  Serial.println("Thrump stopped");
  click.Setplay();
  click.Resetindex();
  Serial.println("Click stopped");
  
}

void setGain(int gainLevel) {
  float gain = gainLevel / 125.0;  
  sgtl5000_1.volume(gain);   
}

void loop() {
  playFile("TEST.WAV");
  delay(1000);
  /*
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'P' && playWav1.isPlaying() == false) {
      //Serial.println("Start playing");
      playFile("SDTEST4.WAV");
      delay(5); // wait for library to parse WAV info
    }
    else if (command == 'S') {
      if (playWav1.isPlaying()) {
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
    
    }
  }
  */
}

