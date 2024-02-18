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
    discon_time = 0.05;
    tail_time = 0.5;
    gap = 60.0/33.0;
}

thrumps::~thrumps(){}

void thrumps::Isplaying(){
    is_thrump = not is_thrump;
}

void thrumps::update(){
    audio_block_t *block = allocate();
    if (!block){
        return;
    }    
    float discon_Samples = round(discon_time * SAMPLE_RATE_HZ);
    float tail_Samples = round(tail_time * SAMPLE_RATE_HZ);
    float gap_Samples = round(gap * SAMPLE_RATE_HZ);
    
    int index = 0;
    while (is_thrump){
        for (int i = 0; i < discon_Samples; i++) {
            if (index < AUDIO_BLOCK_SAMPLES) {
                block->data[index] = 1;                
                index++;
            }
            else {    
                transmit(block);
                release(block);
                block = allocate();
                index = 0;
            }
        };
        for (int i = 0; i < tail_Samples; i++) {
            if (index < AUDIO_BLOCK_SAMPLES) {
                float oscillation = sin(2 * M_PI * i * 1000 / SAMPLE_RATE_HZ);
                float decay = exp(-decayFactor * i);
                float value = oscillation * decay;
                value = max(-1,min(1,value));    
                block->data[index] = value * MULT_16;
                index++;
            }
            else {
                transmit(block);
                release(block);
                block = allocate();
                index = 0;
            }
        };
        for (int i = 0; i < (gap_Samples - discon_Samples - tail_Samples); i++) {
            if (index < AUDIO_BLOCK_SAMPLES){
                block->data[index] = 0;
                index++;
            }
            else {
                transmit(block);
                release(block);
                block = allocate();
                index = 0;
            }
        }
    }
    transmit(block);
    release(block);
}