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
    void generate_click(float time);
    void Isplaying();
    float click_index = 1.0;
    float time = 0.0;
    bool is_click = false;
    bool is_playing = false;

  private:
    float a1;
    float b1;
    float a2;
    float b2;
    float a3;
    float b3;
};

#endif
