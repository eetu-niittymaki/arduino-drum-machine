#include "Arduino.h"
#include "Utilities.h"


float Utilities::setPitch(float oldPitch, unsigned short int recordedPitch) {
    return { (recordedPitch * oldPitch / 512.f) + 0.1f };
}

unsigned int Utilities::millisTo_BPM_ToMillis(unsigned short int value) {
    return {  60000 / value };
}

float Utilities::mapFloat(float x, float in_min, float in_max, float out_min, float out_max) { // Copied and modified from Arduino map() reference page
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

// Potentiometer readings fluctuate, so count average of 5 values for better accuracy
unsigned short int Utilities::getAverage(unsigned short int value, unsigned short int *sum, uint8_t *counter) {
  const uint8_t max = 5;
  unsigned short int tempValue = 0;
  *sum += value;
  *counter += 1;

  if (*counter >= max) {
    tempValue = *sum / max;
    *counter = 0;
    *sum = 0;
    return tempValue;
  }
}

bool Utilities::startPlayback(uint8_t stepCount, uint8_t beatCount, uint8_t pointer) {
  float divider = ((float)beatCount / (float)stepCount);
  uint8_t count = pointer * divider;
  uint8_t prevCount = (pointer - 1) * divider;

  return { (pointer == 0) ? start = (beatCount != 0) : start = (count > prevCount) };
}