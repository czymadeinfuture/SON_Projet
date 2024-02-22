#include "clicks.h"
#include <cmath>
#include <cstdlib>
#define MULT_16 32767
#define SAMPLE_RATE_HZ 44100
#define AUDIO_OUTPUTS 1

clicks::clicks() :
AudioStream(AUDIO_OUTPUTS, new audio_block_t*[AUDIO_OUTPUTS])
{
  is_click = false;
}

clicks::~clicks(){}

// Generate a random duration by using a Weibull distribution
/*float clicks::Duration(){
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
}*/

// Gereration of duration,gap and amplitude by using the same model
float clicks::Duration() {
    float uniformRandom = random(1000) / 1000.0; 
    float dur = pow((-log(1 - uniformRandom)), 0.5) * 0.4;
    return dur;
}

float clicks::Gap() {
    float uniformRandom = random(1000) / 1000.0; 
    float gap = pow((-log(1 - uniformRandom)), 0.4) * 0.9;
    return gap;
}

float clicks::Amplitude(){
    float uniformRandom = random(1000) / 1000.0; 
    float am = pow((-log(1 - uniformRandom)), 0.3) * 1.0;
  return am;
}

// control of the generation of click
void clicks::Setplay(){
  is_click = not is_click;
}

// reset the index of click to the begining
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
          float amplitude = Amplitude();
              
          int i = 0;
          while (i < numSamples) {
            if (output_index < AUDIO_BLOCK_SAMPLES) {      
              amplitude = max(-1,min(1,amplitude)) ;
              int16_t sample = (int16_t)amplitude;
                    
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
          click_index = click_index + clickGap;  
          transmit(block[channel], channel);
          release(block[channel]); 
          is_click = false;
          }
      }
    }
}


  
