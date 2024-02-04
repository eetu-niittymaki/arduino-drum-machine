#ifndef Utilities_h
#define Utilities_h

#include "Arduino.h"

class Utilities {
    public:
        float setPitch(unsigned intoldPitch, unsigned short int recordedPitch);
        unsigned int millisTo_BPM_ToMillis(unsigned short int value);
        float mapf(float x, float in_min, float in_max, float out_min, float out_max);
        bool startPlayback(uint8_t stepCount, uint8_t beatCount, uint8_t pointer);
    private:
        bool start = false;
};

#endif