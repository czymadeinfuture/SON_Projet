#include "clicks.h"
#include <cmath>
#include <cstdlib>
#define MULT_16 32767
#define SAMPLE_RATE_HZ 44100
#define AUDIO_OUTPUTS 1

clicks::clicks() :
AudioStream(AUDIO_OUTPUTS, new audio_block_t*[AUDIO_OUTPUTS])
{
  a1 = 10.7;
  b1 = 1.06;
  a2 = 0.2;
  b2 = 2433.8;
  a3 = -3.6;
  b3 = 0.74;
  is_click = false;
}

clicks::~clicks(){}


float clicks::Duration(){
 return  b1 * (pow(a1, -b1)) * (pow(time,b1-1)) * exp(-pow(time/a1,b1));
}

float clicks::Gap(){
  return  1/(pow(b2,a2) * tgamma(a2 + 1)) * pow(time,a2 - 1) * exp(time/b2);
}

float clicks::Amplitude(){
  float am = static_cast<float>(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
  return am;
}

void clicks::Setplay(unsigned long Time){
  is_click = not is_click;
  time = Time/1000.0;
}

void clicks::Resetindex(){
  click_index = 1.0;
}


void clicks::update(){
    audio_block_t *block[AUDIO_OUTPUTS];
    if (is_click){ 
      for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
        block[channel] = allocate();
        if (block[channel]) {    
          int output_index = 0;
              
          // Calculate the number of samples for each part of the click
          float clickDuration = Duration();
          int numSamples = round(clickDuration * SAMPLE_RATE_HZ);
          float clickGap = Gap();
          float amplitudeScale = MULT_16 * Amplitude();
              
          int i = 0;
          while (i < numSamples) {
            if (output_index < AUDIO_BLOCK_SAMPLES) {      
              float sampleValue = sin(2 * M_PI * i * 1000 / SAMPLE_RATE_HZ);
              sampleValue *= amplitudeScale;
              sampleValue = max(-1,min(1,sampleValue));
              int16_t sample = (int16_t)sampleValue;
                    
              block[channel] -> data[output_index] = sample;
              output_index++;
              i++;
            }
            else {
              transmit(block[channel], channel);
              release(block[channel]);
              output_index = 0;
              block[channel] = allocate(); 
            }
          }
          click_index = (clickGap <= 0.01) ? click_index + 0.01 : click_index + (round(clickGap * 100))/100 ;  
          transmit(block[channel], channel);
          release(block[channel]); 
          is_click = false;
          }
      }
    }
}


  

