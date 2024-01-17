#include <Wire.h>
#include <MozziGuts.h>
#include <Sample.h>
#include <EventDelay.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#include "sfx/bongo.h" 
#include "sfx/conga.h" 
#include "sfx/cymbal.h" 
#include "sfx/hihat.h"
#include "sfx/kick.h" 
#include "sfx/snare.h" 
#include "sfx/rim.h" 
#include "sfx/perc_hat.h" 
#include "sfx/hat_bongo.h" 
#include "sfx/clap.h" 
#include "sfx/cowbell.h" 
#include "sfx/tambourine.h" 

#define NUM_CELLS 4096 // Make sure this is the same as in .h wavetable files
#define SAMPLERATE 16384
#define CONTROL_RATE 256

#define MAX_TAP_TIMES 8
#define MAX_STEPS 16

// Analog
#define tempoPot 0
#define volumePot 10
#define swingPot 11
#define beatPotA 1
#define beatPotB 2
#define beatPotC 3
#define beatPotD 4
#define stepPotA 7
#define stepPotB 6
#define stepPotC 5
#define stepPotD 4
#define pitchPotA 12
#define pitchPotB 13
#define pitchPotC 14 
#define pitchPotD 15
// use for 8?

// Digital
#define onSwitch 2
#define ledA 10
#define ledB 11
#define ledC 12
#define ledD 13
//#define tapSwitch

//A
Sample <NUM_CELLS, AUDIO_RATE>aBongo(bongo_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aHatBongo(hat_bongo_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aKick(kick_DATA);

//B
Sample <NUM_CELLS, AUDIO_RATE>aConga(conga_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aRim(rim_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aSnare(snare_DATA);

//C
Sample <NUM_CELLS, AUDIO_RATE>aCymbal(cymbal_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aPercHat(perc_hat_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aHiHat(hihat_DATA);

//D
Sample <NUM_CELLS, AUDIO_RATE>aClap(clap_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aTambourine(tambourine_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aCowbell(cowbell_DATA);

// Default sample values
Sample <NUM_CELLS, AUDIO_RATE> *soundA = &aKick;
Sample <NUM_CELLS, AUDIO_RATE> *soundB = &aSnare;
Sample <NUM_CELLS, AUDIO_RATE> *soundC = &aHiHat;
Sample <NUM_CELLS, AUDIO_RATE> *soundD = &aClap;

// Schedules sampels to start
EventDelay kTriggerDelay;

int buttonState = 0;

byte pointerA = 0;
byte pointerB = 0;
byte pointerC = 0;
byte pointerD = 0;

byte volume;
byte bpm;
float swing;
const float recorded_pitch =  (float)SAMPLERATE / (float)NUM_CELLS;

int tempo = 120;
int swingStep = 1;

float tapCountMa[MAX_TAP_TIMES];
byte nextTapIndex = 0;

void setup() {
  Serial.begin(115200);
  pinMode(onSwitch, INPUT);
  //pinMode(tapSwitch, INPUT);
  pinMode(3, INPUT_PULLUP);
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);
  pinMode(8, INPUT_PULLUP);
  pinMode(9, INPUT_PULLUP);
  pinMode(10, INPUT_PULLUP);
  pinMode(11, OUTPUT); //9 for Nano, 11 for Mega
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledC, OUTPUT);
  pinMode(ledD, OUTPUT);

  startMozzi(CONTROL_RATE);

  aBongo.setFreq((float) bongo_SAMPLERATE / (float) NUM_CELLS); 
  aHatBongo.setFreq((float) hat_bongo_SAMPLERATE / (float) NUM_CELLS); 
  aKick.setFreq((float) kick_SAMPLERATE / (float) NUM_CELLS); 
  
  aConga.setFreq((float) conga_SAMPLERATE / (float) NUM_CELLS);
  aRim.setFreq((float) rim_SAMPLERATE / (float) NUM_CELLS); 
  aSnare.setFreq((float) snare_SAMPLERATE / (float) NUM_CELLS); 

  aCymbal.setFreq((float) cymbal_SAMPLERATE / (float) NUM_CELLS);
  aPercHat.setFreq((float) perc_hat_SAMPLERATE / (float) NUM_CELLS); 
  aHiHat.setFreq((float) hihat_SAMPLERATE / (float) NUM_CELLS); 

  aClap.setFreq((float) clap_SAMPLERATE / (float) NUM_CELLS); 
  aTambourine.setFreq((float) tambourine_SAMPLERATE / (float) NUM_CELLS); 
  aCowbell.setFreq((float) cowbell_SAMPLERATE / (float) NUM_CELLS); 


  //kTriggerDelay.set(10);
}

bool startPlayback(byte stepCount, byte beatCount, byte i) {
  float divider = ((float)beatCount / (float)stepCount);
  bool start;
  byte count = i * divider;
  byte prevCount = (i - 1) * divider;

  return { (i == 0) ? start = (beatCount != 0) : start = (count > prevCount) };
}

float setPitch(int oldPitch) {
  return { (recorded_pitch * (float) oldPitch / 512.f) + 0.1f };
}

void tapTempo() {
  /*
  byte buttonsPressed = 0;
  byte buttonGroup = 0;
  byte lastButtonsPressed = 0;
  byte menuState = 0;

  if (tapSWITCH)

  */
}

void updateControl() {
  buttonState = digitalRead(onSwitch);

  if (swingStep > MAX_STEPS) swingStep = 1;

  ////////////////////////////
  // Potentiometer readings //
  ///////////////////////////

  swing = ((float)mozziAnalogRead(volumePot) / 1024) + 1;
  int tempoVal = mozziAnalogRead(tempoPot);
  byte printTempo = map(tempoVal, 0, 1024, 245, 80);

  int volumeVal = mozziAnalogRead(volumePot);
  volume =  map(volumeVal, 0, 1023, 0, 255);

  byte stepA = (byte) map(mozziAnalogRead(stepPotA), 0, 1023, 1, MAX_STEPS);
  byte stepB = (byte) map(mozziAnalogRead(stepPotB), 0, 1023, 1, MAX_STEPS);
  byte stepC = (byte) map(mozziAnalogRead(stepPotC), 0, 1023, 1, MAX_STEPS);
  byte stepD = (byte) map(mozziAnalogRead(stepPotD), 0, 1023, 1, MAX_STEPS);

  byte beatA = (byte) map(mozziAnalogRead(beatPotA), 0, 1023, 0, stepA);
  byte beatB = (byte) map(mozziAnalogRead(beatPotB), 0, 1023, 0, stepB);
  byte beatC = (byte) map(mozziAnalogRead(beatPotC), 0, 1023, 0, stepC);
  byte beatD = (byte) map(mozziAnalogRead(beatPotD), 0, 1023, 0, stepD);
  
  int pitchValA = mozziAnalogRead(pitchPotA);
  int pitchValB = mozziAnalogRead(pitchPotB);
  int pitchValC = mozziAnalogRead(pitchPotC);
  int pitchValD = mozziAnalogRead(pitchPotD);
  
  if (swingStep % 2 == 0) {
    tempo = tempoVal / 4 + 70 * swing;
  } else {
    tempo = tempoVal / 4 + 70 * (2 - swing);
  }

  /*
  int val = mozziAnalogRead(SIG_pin);

  //return the value
  float voltage = (val * 5.0) / 1024.0;
  */

  //////////////////////
  // Switch readings //
  ////////////////////

  // If switch not at ON position, stop playback, turn leds off
  if (digitalRead(onSwitch) == LOW) {
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, LOW);
    digitalWrite(ledC, LOW);
    digitalWrite(ledD, LOW);
    return;
  }
 
  if (digitalRead(7) == LOW) {
    soundA = &aHatBongo;
  } else if (digitalRead(8) == LOW) {
    soundA = &aBongo;
  } else {
    soundA = &aKick;
  }

  if (digitalRead(5) == LOW) {
    soundB = &aConga;
  } else if (digitalRead(6) == LOW) {
    soundB = &aRim;
  } else {
    soundB = &aSnare;
  }

  if (digitalRead(3) == LOW) {
    soundC = &aCymbal;
  } else if (digitalRead(4) == LOW) {
    soundC = &aPercHat;
  } else {
    soundC = &aHiHat;
  } 
  
  if (digitalRead(9) == LOW) {
    soundD = &aTambourine;
  } else if (digitalRead(10) == LOW) {
    soundD = &aCowbell;
  } else {
    soundD = &aClap;
  }

  const int arraySize = 4;
  char pointers[arraySize] = { pointerA, pointerB, pointerC, pointerD };
  char steps[arraySize] = { stepA, stepB, stepC, stepD };
  char beats[arraySize] = { beatA, beatB, beatC, beatD };
  int leds[arraySize] = { ledA, ledB, ledC, ledD };
  int pitches[arraySize] = { pitchValA, pitchValB, pitchValC, pitchValD };
  Sample <NUM_CELLS, AUDIO_RATE> *sounds[arraySize] = { soundA, soundB, soundC, soundD };

  if(kTriggerDelay.ready()) {
    for (int i = 0; i < arraySize; i++) {
      if (pointers[i] >= steps[i])  {
        pointers[i] = 0;
      }

      if (startPlayback(steps[i], beats[i], pointers[i])) {
        (*sounds[i]).start();
        (*sounds[i]).setFreq(setPitch(pitches[i]));
        digitalWrite(leds[i], HIGH);
        //digitalWrite(!(leds[i]), LOW);
        digitalWrite(leds[i], LOW);
      }
    }

    pointerA++;
    pointerB++;
    pointerC++;
    pointerD++;
    swingStep++;

    kTriggerDelay.start(tempo);
  }
}

int updateAudio(){
  int gain = (int)
    ((long) (*soundA).next() * 255 +
            (*soundB).next() * 255 +
            (*soundC).next() * 255 +
            (*soundD).next() * 255) >> 4;

  // Mozzi default output range is -244 to 243
  if (gain > 243) {
    gain = 243;
  } else if (gain < -244) {
    gain = -244; 
  }
  return gain;
}

void loop() {
  audioHook();
}
