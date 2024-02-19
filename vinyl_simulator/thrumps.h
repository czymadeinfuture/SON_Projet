#ifndef thrumps_teensy_h_
#define thrumps_teensy_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "Audio.h"

class thrumps : public AudioStream
{
  public:
    thrumps();
    ~thrumps();
    
    virtual void update();
    bool is_thrump = false;
    void Isplaying();
  private:
    float discon_time;
    float tail_time;
    float gap; 
};

#endif