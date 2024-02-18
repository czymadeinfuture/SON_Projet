#include <Audio.h>
#include <Wire.h>
#include <SPI.h>
#include <SD.h>
#include <SerialFlash.h>
#include "clicks.h"
#include "thrumps.h"


AudioPlaySdWav           playWav1;
AudioOutputI2S           i2s1;
AudioMixer4              mixer1; 
AudioMixer4              mixer2;       
clicks                   click;
thrumps                  thrump;        

AudioConnection          patchCord1(playWav1, 0, mixer1, 0);
AudioConnection          patchCord2(click, 0, mixer1, 1);
AudioConnection          patchCord3(thrump, 0, mixer1, 2);

AudioConnection          patchCord4(playWav1, 1 , mixer2, 0);
AudioConnection          patchCord5(click, 0, mixer2, 1);
AudioConnection          patchCord6(thrump, 0, mixer2, 2);

AudioConnection          patchCord7(mixer1, 0, i2s1, 0);
AudioConnection          patchCord8(mixer2, 0, i2s1, 1);

AudioControlSGTL5000     sgtl5000_1;



// Use these with the Teensy Audio Shield
#define SDCARD_CS_PIN    10
#define SDCARD_MOSI_PIN  11
#define SDCARD_SCK_PIN   13  

unsigned long musicPlaybackPosition = 0;

void setup() {
  Serial.begin(9600);

  AudioMemory(8);

  sgtl5000_1.enable();
  sgtl5000_1.volume(0.5);
  
  SPI.setMOSI(SDCARD_MOSI_PIN);
  SPI.setSCK(SDCARD_SCK_PIN);
  if (!(SD.begin(SDCARD_CS_PIN))) {
    Serial.println("Unable to access the SD card");
    return;
  }

  File file = SD.open("SDTEST1.WAV");
  if (!file) {
    Serial.println("File not found");
    return;
  }
  
  mixer1.gain(0, 0.5);
  mixer1.gain(1, 0.1);
  mixer1.gain(2, 0.1);  
  
  click.Isplaying();
  thrump.Isplaying();  
  
  playFile("TEST.WAV");
  delay(1000);
}

void playFile(const char *filename)
{
  Serial.print("Playing file: ");
  Serial.println(filename);

  playWav1.play(filename);
  delay(50);
}



void loop() {
  musicPlaybackPosition = playWav1.positionMillis();
  if (musicPlaybackPosition >= click.click_index * 1000 && musicPlaybackPosition <= click.click_index * 1000 + 100) {
    click.generate_click(musicPlaybackPosition);
  };
  
  mixer1.update(); 
  
  if (!playWav1.isPlaying()) {
    patchCord2.disconnect();
    patchCord3.disconnect();
    Serial.println("File has finished playing");
    thrump.Isplaying();
    click.Isplaying();
  }  
   
}

