#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "clicks.h"
#include "thrumps.h"
#include "LOWPASS.h"


AudioPlaySdWav           playWav1;
AudioOutputI2S           i2s1;
AudioMixer4              mixer1; // mixer for the left channel
AudioMixer4              mixer2; // mixer for the right channel      
clicks                   clic;
thrumps                  thrump;      
LOWPASS                  lowpass;  

// connect playWav1, click and thrump to the mixer
AudioConnection          patchCord7(playWav1, 0, lowpass, 0);
AudioConnection          patchCord8(playWav1, 1, lowpass, 1);

AudioConnection          patchCord1(lowpass, 0, mixer1, 0);
AudioConnection          patchCord2(clic, 0, mixer1, 1);  
AudioConnection          patchCord3(thrump, 0, mixer1, 2);

AudioConnection          patchCord4(lowpass, 0 , mixer2, 0);
AudioConnection          patchCord5(clic, 0, mixer2, 1);
AudioConnection          patchCord6(thrump, 0, mixer2, 2);

AudioConnection          patchCord9(mixer1, 0, i2s1, 0);
AudioConnection          patchCord10(mixer2, 0, i2s1, 1);

AudioControlSGTL5000     sgtl5000_1;

unsigned long musicPlaybackPosition = 0;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13  


const char* songList[] = {"SDTEST1.WAV", "SDTEST2.WAV", "SDTEST3.WAV","SDTEST4.WAV"};
int currentSongIndex = 0; 
int songListSize = sizeof(songList) / sizeof(songList[0]);


void playNextSong() {
  if (playWav1.isPlaying()) {
    playWav1.stop();  // 如果当前有歌曲正在播放，先停止它
  }

  currentSongIndex++;  // 
  if (currentSongIndex >= songListSize) {
    currentSongIndex = 0;  
  }

  playFile(songList[currentSongIndex]); 
}

void setGain(int gainLevel) {
  float gain = gainLevel / 125.0;  
  sgtl5000_1.volume(gain);  // 
}

void setup() {
  Serial.begin(9600);
  AudioMemory(20);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.4);
  lowpass.setParamValue("freq",1000);
  
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
     while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  
  
  mixer1.gain(0, 0.2); // set the gain for the playWav1
  mixer1.gain(1, 20000.0); // set the gain for the click
  mixer1.gain(2, 8000.0); // set the gain for the thrump
  
  mixer2.gain(0, 0.2); // set the gain for the playWav1
  mixer2.gain(1, 20000.0); // set the gain for the click
  mixer2.gain(2, 8000.0); // set the gain for the thrump 
  
}

void playFile(const char *filename){
  patchCord9.connect();
  patchCord10.connect();
  
  Serial.print("Playing file: ");
  Serial.println(filename);

  // Start playing the file.  This sketch continues to
  // run while the file plays.
  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(25);

  Serial.println(playWav1.isPlaying() ? "File is playing" : "File is not playing");
  
}

void loop() {
  if (Serial.available() > 0) {
    char command = Serial.read();

    if (command == 'P' && playWav1.isPlaying() == false) {
      //Serial.println("Start playing");
      playFile(songList[currentSongIndex]);
      delay(5); // wait for library to parse WAV info
    }
    else if (command == 'S') {
      if (playWav1.isPlaying()) {
        playWav1.stop();
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
  };

  if (playWav1.isPlaying()) {
    musicPlaybackPosition = playWav1.positionMillis();
    // If it is the time to play the click or the thrump, set the play
    if (musicPlaybackPosition >= clic.click_index * 1000 && musicPlaybackPosition <= clic.click_index * 1000 + 100) {
      clic.Setplay();
      //Serial.println("Click started at " + String(musicPlaybackPosition));
    };
    if (musicPlaybackPosition >= thrump.gap * 1000 * thrump.thrump_num && musicPlaybackPosition <= thrump.gap * 1000 * thrump.thrump_num + 10) {
       if (thrump.thrump_num >=3 && thrump.thrump_num <=20){
        thrump.Setplay();
        //Serial.println("Thrump started at " + String(musicPlaybackPosition));
       };
       thrump.thrump_num++;
    }
  }
  else{
    patchCord9.disconnect();
    patchCord10.disconnect();
    // When the file is finished (playWav1.isPlaying() == false)
    Serial.println("File has finished playing");
    
    // Stop the click and thrump
    thrump.Setplay();
    thrump.thrump_num = 1;
    Serial.println("Thrump stopped");
    clic.Setplay();
    clic.Resetindex();
    Serial.println("Click stopped");
  }
}
