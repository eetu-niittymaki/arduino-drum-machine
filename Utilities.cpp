#include "Arduino.h"
#include "Utilities.h"


float Utilities::setPitch(unsigned int oldPitch, unsigned short int recordedPitch) {
    return { (recordedPitch * (float) oldPitch / 512.f) + 0.1f };
}

unsigned int Utilities::millisTo_BPM_ToMillis(unsigned short int value) {
    return {  60000 / value };
}

float Utilities::mapf(float x, float in_min, float in_max, float out_min, float out_max) { // Copied and modified from Arduino map() reference page
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}