#ifndef clicks_teensy_h_
#define clicks_teensy_h_

#include "Arduino.h"
#include "AudioStream.h"
#include "Audio.h"


class clicks : public AudioStream
{
  public:
    clicks();
    ~clicks();
    
    virtual void update();
    float Duration();
    float Gap();
    float Amplitude();
    void Setplay();
    void Resetindex();
    float click_index = 1.0;

  private:

    bool is_click;
};

#endif
