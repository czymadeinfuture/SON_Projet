#include "thrumps.h"
#include <cmath>
#include <cstdlib>
#define MULT_16 32767
#define SAMPLE_RATE_HZ 44100
#define AUDIO_OUTPUTS 1
#define decayFactor 0.05

thrumps::thrumps() :
AudioStream(AUDIO_OUTPUTS, new audio_block_t*[AUDIO_OUTPUTS])
{
    discon_time = 0.02;
    tail_time = 1.0;
    gap = 60.0/33.0;
    is_thrump = false;
    thrump_num = 1;
}

thrumps::~thrumps(){}

void thrumps::Setplay(){
    is_thrump = not is_thrump;
}

void thrumps::update(){
    audio_block_t *block[AUDIO_OUTPUTS];
    // Calculate the number of samples for each part of the thrump
    float discon_Samples = round(discon_time * SAMPLE_RATE_HZ);
    float tail_Samples = round(tail_time * SAMPLE_RATE_HZ);
    if (is_thrump){
        for (int channel = 0; channel < AUDIO_OUTPUTS; channel++) {
            block[channel] = allocate();
            if (block[channel]) { 
                int index = 0;

                // Generate the discontinuity
                int i1 = 0;
                while (i1 < discon_Samples) {
                      if (index < AUDIO_BLOCK_SAMPLES) {
                          block[channel]->data[index] = 1;             
                          index++;
                          i1++;
                      }
                      else {    
                          transmit(block[channel], channel);
                          release(block[channel]);
                          block[channel] = allocate();
                          index = 0;
                      }
                };
                // Generate the tail
                int i2 = 0;
                while (i2 < tail_Samples) {
                      if (index < AUDIO_BLOCK_SAMPLES) {
                          float oscillation = cos(2 * M_PI * i2 * 10 / SAMPLE_RATE_HZ);
                          float decay = exp(-decayFactor * i2);
                          float value = oscillation * decay;
                          value = max(-1,min(1,value));    
                          block[channel]->data[index] = value * MULT_16;
                          index++;
                          i2++;
                      }
                      else {
                          transmit(block[channel], channel);
                          release(block[channel]);
                          block[channel] = allocate();
                          index = 0;
                      }
                };
                transmit(block[channel], channel);
                release(block[channel]);
                is_thrump = false;
            }
        }
    }
}
