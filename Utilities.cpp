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

// Potentiometer readings fluctuate, so count average for better accuracy
unsigned short int Utilities::getAverage(unsigned short int value, unsigned short int *sum,
                                        uint8_t *counter, unsigned short int *array, uint8_t *max) {
  *sum -= array[*counter];
  array[*counter] = value;
  *sum += array[*counter];
  *counter += 1;

  if (*counter >= *max) {
    *counter = 0;
  }

  return *sum / *max;
}

// stepCount = number of steps in sequence, beatCount = number of beats, pointer = position in sequence
bool Utilities::startPlayback(uint8_t stepCount, uint8_t beatCount, uint8_t pointer) {
  // Check for potential division by zero
  if (stepCount == 0 || beatCount == 0) {
      return false; // No steps or beats to process, no playback
  }

  // Calculate the divider once
  float divider = static_cast<float>(beatCount) / static_cast<float>(stepCount);

  // Calculate the current count and previous count
  uint8_t count = pointer * divider;       // Calculate count for the current pointer
  uint8_t prevCount = count - divider;     // Derive previous count from current

  // Determine playback based on pointer
  if (pointer == 0) {
      return beatCount != 0;  // Playback if there are any beats
  } else {
      return count > prevCount; // Continue playback if the current step leads to a new beat
  }
}