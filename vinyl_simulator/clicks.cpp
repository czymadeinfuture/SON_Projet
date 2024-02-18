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
}

clicks::~clicks(){}

void clicks::generate_click(float Time){
  time = Time;
  is_click = true;
}

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

void clicks::Isplaying(){
  is_playing = not is_playing;
}


void clicks::update(){
    audio_block_t *block = allocate();
    if (!block) {
      return;
    }

    float clickDuration = Duration();
    int numSamples = round(clickDuration * SAMPLE_RATE_HZ);
    float clickGap = Gap();
    float amplitudeScale = MULT_16 * Amplitude();
    
    int output_index = 0;
    while (is_playing){  
      if (is_click){
        for (int i = 0; i < numSamples; i++) {
          if (output_index < AUDIO_BLOCK_SAMPLES) {      
              float sampleValue = sin(2 * M_PI * i * 1000 / SAMPLE_RATE_HZ);
              sampleValue *= amplitudeScale;
              sampleValue = max(-1,min(1,sampleValue));
              int16_t sample = (int16_t)sampleValue;
              
              block->data[output_index] = sample;
              output_index++;
              
              const float epsilon = 0.0001;
              click_index = (fabs(round(clickGap) - 0.0) < epsilon) ? click_index + 1.0 : click_index + round(clickGap);
          }
          else {
            transmit(block);
            release(block);
            output_index = 0;
            block = allocate();  
          }
        is_click = false;
        }
      }
      else {
        if (output_index < AUDIO_BLOCK_SAMPLES) {    
            block -> data[output_index] = 0;       
            output_index++;
        }
        else {
          transmit(block);
          release(block);
          output_index = 0;
          block = allocate();
        }
      }
    }
}

  

