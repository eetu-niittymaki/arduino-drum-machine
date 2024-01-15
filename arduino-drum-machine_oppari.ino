#include <MozziGuts.h>
#include <Sample.h>
#include <EventDelay.h>

#include "sfx/bongo.h" 
#include "sfx/conga.h" 
#include "sfx/cymbal.h" 
#include "sfx/hihat.h"
#include "sfx/kick.h" 
#include "sfx/snare.h" 
#include "sfx/rim.h" 
#include "sfx/perc_hat.h" 
#include "sfx/hat_bongo.h" 

#define NUM_CELLS 2048 // Make sure this is the same as in .h wavetable files
#define SAMPLERATE 16384
#define CONTROL_RATE 256 // 128

#define MAX_TAP_TIMES 8

// Analog
#define tempoPot 0
#define volumePot 4 
#define beatPotA 1
#define beatPotB 2
#define beatPotC 3
//#define beatPotD
#define stepPotA 7
#define stepPotB 6
#define stepPotC 5
//#define stepPotD

// Digital
#define onSwitch 2
#define ledA 10
#define ledB 11
#define ledC 12
//#define tapSwitch

Sample <NUM_CELLS, AUDIO_RATE>aBongo(bongo_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aConga(conga_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aCymbal(cymbal_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aHatBongo(hat_bongo_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aRim(rim_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aPercHat(perc_hat_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aKick(kick_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aHiHat(hihat_DATA);
Sample <NUM_CELLS, AUDIO_RATE>aSnare(snare_DATA);

Sample <NUM_CELLS, AUDIO_RATE> *soundA = &aKick; // Default samples
Sample <NUM_CELLS, AUDIO_RATE> *soundB = &aSnare;
Sample <NUM_CELLS, AUDIO_RATE> *soundC = &aHiHat;
//Sample <NUM_CELLS, AUDIO_RATE> *soundD = &a;

// Schedules sampels to start
EventDelay kTriggerDelay;

const byte MAX_STEPS = 16;

int buttonState = 0;

byte pointerA = 0;
byte pointerB = 0;
byte pointerC = 0;
//byter pointerD = 0;
int swingStep = 1;

byte volume;
byte bpm;
int tempo = 120;

float swing;

float tapCountMa[MAX_TAP_TIMES];
byte nextTapIndex = 0;

const float recorded_pitch =  (float)SAMPLERATE / (float)NUM_CELLS;

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
  pinMode(9, OUTPUT); //9 for Nano, 11 for Mega
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledC, OUTPUT);

  startMozzi(CONTROL_RATE);

  aBongo.setFreq((float) bongo_SAMPLERATE / (float) NUM_CELLS); 
  aConga.setFreq((float) conga_SAMPLERATE / (float) NUM_CELLS);
  aCymbal.setFreq((float) cymbal_SAMPLERATE / (float) NUM_CELLS);
  aHatBongo.setFreq((float) hat_bongo_SAMPLERATE / (float) NUM_CELLS); 
  aRim.setFreq((float) rim_SAMPLERATE / (float) NUM_CELLS); 
  aPercHat.setFreq((float) perc_hat_SAMPLERATE / (float) NUM_CELLS); 
  aHiHat.setFreq((float) hihat_SAMPLERATE / (float) NUM_CELLS); 
  aKick.setFreq((float) kick_SAMPLERATE / (float) NUM_CELLS); 
  aSnare.setFreq((float) snare_SAMPLERATE / (float) NUM_CELLS); 

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
  
  swing = ((float)mozziAnalogRead(volumePot) / 1024) + 1;
  int tempoVal = mozziAnalogRead(tempoPot);
  byte printTempo = map(tempoVal, 0, 1024, 245, 80);

  //int volumeVal = mozziAnalogRead(volumePot);
  //volume =  map(volumeVal, 0, 1023, 0, 255);
  //int tempoVal = mozziAnalogRead(tempoPot);
  //tempo =  map(tempoVal, 0, 1023, 0, 255);

  if (swingStep % 2 == 0) {
    tempo = tempoVal / 4 + 70 * swing;
  } else {
    tempo = tempoVal / 4 + 70 * (2 - swing);
  }


  /*
  int pitchValA = mozziAnalogRead(pitchPotA);
  int pitchValB = mozziAnalogRead(pitchPotB);
  int pitchValC = mozziAnalogRead(pitchPotC);
  int pitchValD = mozziAnalogRead(pitchPotD);
  */
  
  // If switch not at ON position, stop playback, turn leds off
  if (digitalRead(onSwitch) == LOW) {
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, LOW);
    digitalWrite(ledC, LOW);
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
  /*
  if (digitalRead(9) == LOW) {
    soundD = &a;
  } else if (digitalRead(10) == LOW) {
    soundD = &a;
  } else {
    soundD = &a;
  } 
  */

  byte stepA = (byte) map(mozziAnalogRead(stepPotA), 0, 1023, 1, MAX_STEPS);
  byte stepB = (byte) map(mozziAnalogRead(stepPotB), 0, 1023, 1, MAX_STEPS);
  byte stepC = (byte) map(mozziAnalogRead(stepPotC), 0, 1023, 1, MAX_STEPS);
  //byte stepD = (byte) map(mozziAnalogRead(stepPotD), 0, 1023, 1, MAX_STEPS);

  byte beatA = (byte) map(mozziAnalogRead(beatPotA), 0, 1023, 0, stepA);
  byte beatB = (byte) map(mozziAnalogRead(beatPotB), 0, 1023, 0, stepB);
  byte beatC = (byte) map(mozziAnalogRead(beatPotC), 0, 1023, 0, stepC);
  //byte beatD = (byte) map(mozziAnalogRead(beatPotD), 0, 1023, 0, stepD);

  const int arrSize = 3;
  char pointers[arrSize] = { pointerA, pointerB, pointerC };
  char steps[arrSize] = { stepA, stepB, stepC };
  char beats[arrSize] = {beatA, beatB, beatC };
  int leds[arrSize] = {  ledA, ledB, ledC };
  //Sample<NUM_CELLS, AUDIO_RATE>[] *samples = { *soundA, *soundB, *soundC };

  if(kTriggerDelay.ready()) {
    for (int i = 0; i < arrSize; i++) {
      if (pointers[i] >= steps[i]) pointers[i] = 0;
    }

    if(startPlayback(stepA, beatA, pointerA)) {
      (*soundA).start();
      //(*soundA).setFreq(setPitch(pitchValA));
      digitalWrite(ledA, HIGH);
      digitalWrite(ledB, LOW);
      digitalWrite(ledC, LOW);
      digitalWrite(ledA, LOW);
    }
    if(startPlayback(stepB, beatB, pointerB)) {
      (*soundB).start();
      //(*soundB).setFreq(setPitch(pitchValB));
      digitalWrite(ledB, HIGH);
      digitalWrite(ledA, LOW);
      digitalWrite(ledC, LOW);
      digitalWrite(ledB, LOW);
    }
    if(startPlayback(stepC, beatC, pointerC)) {
      (*soundC).start();
      //(*soundC).setFreq(setPitch(pitchValC));
      digitalWrite(ledC, HIGH);
      digitalWrite(ledA, LOW);
      digitalWrite(ledB, LOW);
      digitalWrite(ledC, LOW);
    }
    /*
    if(startPlayback(stepD, beatD, pointerD)) {
      (*soundD).start();
      //(*soundD).setFreq(setPitch(pitchValD));
      digitalWrite(ledD, HIGH);
      digitalWrite(ledA, LOW);
      digitalWrite(ledB, LOW);
      digitalWrite(ledC, LOW);
      digitalWrite(ledD, LOW);
    }*/

    pointerA++;
    pointerB++;
    pointerC++;
    //pointerD++;
    swingStep++;

    kTriggerDelay.start(tempo);
  }
}

int updateAudio(){
  int gain = (int)
    ((long) (*soundA).next() * 255 +
            (*soundB).next() * 255 +
            (*soundC).next() * 255) >> 4;

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
