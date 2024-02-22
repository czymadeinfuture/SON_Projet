#ifndef thumps_teensy_h_
#define thumps_teensy_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "Audio.h"

class thumps : public AudioStream
{
  public:
    thumps();
    ~thumps();
    
    virtual void update();
    void Setplay();
    float discon_time;
    float tail_time;
    float gap; 
    bool is_thump;
    int thump_num;
    
};

#endif
