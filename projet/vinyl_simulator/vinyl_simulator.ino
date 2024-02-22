#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "clicks.h"
#include "thumps.h"
#include "multimixer.h"



AudioPlaySdWav           playWav1;
AudioOutputI2S           i2s1;
AudioMixer4              mixer1; // mixer for the left channel
AudioMixer4              mixer2; // mixer for the right channel      
clicks                   clic;
thumps                   thump;      
multimixer               mul;  


// connect playWav1, lowpass filter, white noise and degradation.
AudioConnection          patchCord7(playWav1, 0, mul, 0);
AudioConnection          patchCord8(playWav1, 1, mul, 1);

// connect the result with clicks and thumps 
AudioConnection          patchCord1(mul, 0, mixer1, 0);
AudioConnection          patchCord2(clic, 0, mixer1, 1);  
AudioConnection          patchCord3(thump, 0, mixer1, 2);

AudioConnection          patchCord4(mul, 0 , mixer2, 0);
AudioConnection          patchCord5(clic, 0, mixer2, 1);
AudioConnection          patchCord6(thump, 0, mixer2, 2);

// output 
AudioConnection          patchCord9(mixer1, 0, i2s1, 0);
AudioConnection          patchCord10(mixer2, 0, i2s1, 1);

AudioControlSGTL5000     sgtl5000_1;

unsigned long musicPlaybackPosition = 0;

// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13  


// create the playlist and set the value of index
const char* songList[] = {"SDTEST1.WAV","TEST.WAV", "SDTEST2.WAV", "SDTEST3.WAV","SDTEST4.WAV"};
int currentSongIndex = 0; 
int songListSize = sizeof(songList) / sizeof(songList[0]);

//define a fonction to play next song
void playNextSong() {
  if (playWav1.isPlaying()) {
    playWav1.stop();  
  }

  currentSongIndex++;  // 
  if (currentSongIndex >= songListSize) {
    currentSongIndex = 0;  
  }

  playFile(songList[currentSongIndex]); 
}

// define a fonction to control the volume
void setGain(int gainLevel) {
  float gain = gainLevel / 125.0;  
  sgtl5000_1.volume(gain);  // 
}


void setup() {
  Serial.begin(9600);
  AudioMemory(20);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.4);
  
  // set the frequency of the low-pass filter
  mul.setParamValue("freq",3000);
  
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
     while (1) {
      Serial.println("Unable to access the SD card");
      delay(500);
    }
  }
  
  mixer1.gain(0, 0.6); // set the gain for the playWav1
  mixer1.gain(1, 50000.0); // set the gain for the click
  mixer1.gain(2, 100000.0); // set the gain for the thump
  
  mixer2.gain(0, 0.6); // set the gain for the playWav1
  mixer2.gain(1, 50000.0); // set the gain for the click
  mixer2.gain(2, 100000.0); // set the gain for the thump 
  
}

// define a fonction to play the file stocked in the sd card
void playFile(const char *filename){
 
  // reconnect the output
  patchCord9.connect();
  patchCord10.connect();
  
  Serial.print("Playing file: ");
  Serial.println(filename);

  playWav1.play(filename);

  // A brief delay for the library read WAV info
  delay(25);

  Serial.println(playWav1.isPlaying() ? "File is playing" : "File is not playing");
  
}

void loop() {
  // create the communication with the python code which makes the interface
  if (Serial.available() > 0) {
    char command = Serial.read();
    // play the song
    if (command == 'P' && playWav1.isPlaying() == false) {
      playFile(songList[currentSongIndex]);
      delay(5); // wait for library to parse WAV info
    }
    // stop the song
    else if (command == 'S') {
      if (playWav1.isPlaying()) {
        playWav1.stop();
      }
    }
    // control the volume
    else if (command == 'G') {
      // wait for receiving the value of gain
      while (Serial.available() < 1) {
        delay(1);
      }
      int gain = Serial.read();
      setGain(gain);
    }
    // control the value of the noise
    else if (command == 'N') {
      // wait for receiving the value of noise
      while (Serial.available() < 1) {
        delay(1);
      }
      int noiseread = Serial.read();
      float noise = noiseread / 100.0;
      mul.setParamValue("NoiseLevel", noise);
    }
    // play the next song
    else if (command == 'X') {
      playNextSong();
    }
    else if (command == 'M') {
      // wait for receiving the value of distorsion
      while (Serial.available() < 1) {
        delay(1);
      }
      int wowread = Serial.read();
      float wow = wowread/20.0;
      mul.setParamValue("Wow Frequency", wow);
    }
  };

  if (playWav1.isPlaying()) {
    musicPlaybackPosition = playWav1.positionMillis();
    
    // If it is the time to play the click, generate a click
    if (musicPlaybackPosition >= clic.click_index * 1000 && musicPlaybackPosition <= clic.click_index * 1000 + 100) {
      clic.Setplay();
      //Serial.println("Click started at " + String(musicPlaybackPosition));
    };

    // if it is the time to play the thump, generate a thump
    if (musicPlaybackPosition >= thump.gap * 1000 * thump.thump_num && musicPlaybackPosition <= thump.gap * 1000 * thump.thump_num + 10) {
      // we assume that this crack appears between third and 20th cicle in the disc
      if (thump.thump_num >=3 && thump.thump_num <=20){
        thump.Setplay();
        //Serial.println("Thump started at " + String(musicPlaybackPosition));
       };
       thump.thump_num++;
    }
  }
  
  // When the file is finished (playWav1.isPlaying() == false)
  else{
    patchCord9.disconnect();
    patchCord10.disconnect();
    Serial.println("File has finished playing");
    
    // Stop the click and thump
    thump.Setplay();
    thump.thump_num = 1;
    Serial.println("Thump stopped");
    clic.Setplay();
    clic.Resetindex();
    Serial.println("Click stopped");
  }
}
