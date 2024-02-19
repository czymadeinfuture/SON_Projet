#include "clicks.h"
#include <cmath>
#include <cstdlib>
#include <random>
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

// Generate a random duration by using a Weibull distribution
float clicks::Duration(){
  std::default_random_engine generator;

  double shape = 10.69;
  double scale = 1.06;

  std::weibull_distribution<double> distribution(shape, scale);
  float dur = distribution(generator);
  return dur;
}

// Generate a random gap by using a Gamma distribution
float clicks::Gap(){
  std::default_random_engine generator;

  double shape = 0.2;
  double scale = 2433.8;

  std::gamma_distribution<double> distribution(shape, scale);
  float gap = distribution(generator);
  return gap;
}

float clicks::Amplitude(){
  float am = static_cast<float>(rand() - RAND_MAX / 2) / (RAND_MAX / 2);
  return am;
}

void clicks::Setplay(){
  is_click = not is_click;
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
          click_index = (clickGap <= 1.0) ? click_index + 1.0 : click_index + (round(clickGap));  
          transmit(block[channel], channel);
          release(block[channel]); 
          is_click = false;
          }
      }
    }
}


  

