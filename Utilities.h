#ifndef Utilities_h
#define Utilities_h

#include "Arduino.h"
#include <MozziHeadersOnly.h>

class Utilities {
    public:
        float setPitch(float oldPitch, unsigned short int recordedPitch);
        unsigned int millisTo_BPM_ToMillis(unsigned short int value);
        float mapFloat(float x, float in_min, float in_max, float out_min, float out_max);
        unsigned short int getAverage(unsigned short int value, unsigned short int *sum, uint8_t *counter, unsigned short int *array, uint8_t *max);
        bool startPlayback(uint8_t stepCount, uint8_t beatCount, uint8_t pointer);
    private:
        bool start = false;
};

#endif