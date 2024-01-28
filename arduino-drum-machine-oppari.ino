#include <MozziGuts.h>
#include <Sample.h>
#include <EventDelay.h>
#include <ReverbTank.h>
#include <Oscil.h>

#include <tables/cos8192_int8.h>
#include <tables/envelop2048_uint8.h>

#include "samples/bongo.h" 
#include "samples/conga.h" 
#include "samples/cymbal.h" 
#include "samples/hihat.h"
#include "samples/kick.h" 
#include "samples/snare.h" 
#include "samples/rim.h" 
#include "samples/perc_hat.h" 
#include "samples/hat_bongo.h" 
#include "samples/clap.h" 
#include "samples/cowbell.h" 
#include "samples/tambourine.h" 

#define NUM_CELLS 2048 // Make sure this is the same as in .h wavetable files
#define SAMPLERATE 16384
#define CONTROL_RATE 256

#define MAX_STEPS 16

// Analog
#define reverbPot 0
#define tempoPot 9
#define volumePot 10
#define swingPot 11

#define beatPotA 1
#define beatPotB 2
#define beatPotC 3
#define beatPotD 4

#define stepPotA 5
#define stepPotB 6
#define stepPotC 7
#define stepPotD 8

#define pitchPotA 12
#define pitchPotB 13
#define pitchPotC 14 
#define pitchPotD 15

// Digital
#define onSwitch 2
#define ledA 28
#define ledB 12
#define ledC 9
#define ledD 35

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

EventDelay kTriggerDelay; // Schedules sampels to start
EventDelay delayTx; // So serial receiver device doesn't get flooded with data
ReverbTank reverb;

uint8_t readOnSwitch = 0;

byte pointerA = 0;
byte pointerB = 0;
byte pointerC = 0;
byte pointerD = 0;

byte volume;
byte swing;
byte reverbSet;
const float recorded_pitch =  (float)SAMPLERATE / (float)NUM_CELLS;
unsigned short int tempo;
uint8_t swingStep = 1;

unsigned short int printTempo;
byte sampleIdA; 
byte sampleIdB;
byte sampleIdC; 
byte sampleIdD;

void setup() {
  Serial.begin(9600);
  pinMode(onSwitch, INPUT);
  //pinMode(tapSwitch, INPUT);
  pinMode(3, INPUT_PULLUP); // A
  pinMode(4, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP); // B
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP); // C
  pinMode(8, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP); // D
  pinMode(42, INPUT_PULLUP);
  //pinMode(10, OUTPUT); 
  //pinMode(11, OUTPUT); //9 for Nano, 11 for Mega
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

  kTriggerDelay.set(10);
  delayTx.set(350); 
}

bool startPlayback(byte stepCount, byte beatCount, byte pointer) {
  float divider = ((float)beatCount / (float)stepCount);
  bool start;
  byte count = pointer * divider;
  byte prevCount = (pointer - 1) * divider;

  return { (pointer == 0) ? start = (beatCount != 0) : start = (count > prevCount) };
}

void playSample(byte step, byte beat, byte pointer, 
                Sample <NUM_CELLS, AUDIO_RATE> *sample, int pitch, int led) {
  if(startPlayback(step, beat, pointer)) {
      (*sample).start();
      (*sample).setFreq(setPitch(pitch));
      digitalWrite(led, HIGH);
      digitalWrite(led, LOW);
  }
}
/*
void readSwitches(Sample <NUM_CELLS, AUDIO_RATE> *sound, 
                  Sample <NUM_CELLS, AUDIO_RATE> &sample1,
                  Sample <NUM_CELLS, AUDIO_RATE> &sample2,
                  Sample <NUM_CELLS, AUDIO_RATE> &sample3,
                  int switch1, int switch2, int sampleId) {
    if (digitalRead(switch1) == LOW) {
      sound = &sample1;
      sampleId = 2;
    } else if (digitalRead(switch2) == LOW) {
      sound = &sample2;
      sampleId = 1;
    } else {
      sound = &sample3;
      sampleId = 0;
    }
}
*/
float setPitch(int oldPitch) {
  return { (recorded_pitch * (float) oldPitch / 512.f) + 0.1f };
}

void sendData() { 
  if (delayTx.ready()) {
    Serial.print('[');
    Serial.print(readOnSwitch);
    Serial.print(",");
    delayTx.start();
    Serial.print(printTempo);
    Serial.print(",");
    delayTx.start();
    Serial.print(sampleIdA);
    Serial.print(",");
    delayTx.start();
    Serial.print(sampleIdB);
    Serial.print(",");
    delayTx.start();
    Serial.print(sampleIdC);
    Serial.print(",");
    delayTx.start();
    Serial.print(sampleIdD); 
    delayTx.start();
    Serial.print(']');
    Serial.println();
  }
}

unsigned int millisTo_BPM_ToMillis(unsigned int value) {
  unsigned int calc = 60000 / value;
  return calc;
}

void updateControl() {
  readOnSwitch = digitalRead(onSwitch);

  if (swingStep > MAX_STEPS) swingStep = 1;

  /////////////////////////////
  // Potentiometer readings //
  ///////////////////////////
  unsigned int volumeRead = mozziAnalogRead(volumePot);
  unsigned int tempoRead = mozziAnalogRead(tempoPot);
  unsigned int swingRead = mozziAnalogRead(swingPot);
  unsigned int reverbRead = mozziAnalogRead(reverbPot);
  volume =  map(volumeRead, 0, 1023, 0, 255);
  tempo = map(tempoRead, 0, 1023, 214, 1000); // 214 - 1000 milliseconds gives a range of 50 - 280 BPM at 1/4 notes
  swing = map(swingRead, 0, 1023, 0, 75);
  reverbSet = map(reverbRead, 0, 1023, 3, 0);

  unsigned int pitchReadA = mozziAnalogRead(pitchPotA);
  unsigned int pitchReadB = mozziAnalogRead(pitchPotB);
  unsigned int pitchReadC = mozziAnalogRead(pitchPotC);
  unsigned int pitchReadD = mozziAnalogRead(pitchPotD); 

  byte stepA = (byte) map(mozziAnalogRead(stepPotA), 0, 1023, 1, MAX_STEPS);
  byte stepB = (byte) map(mozziAnalogRead(stepPotB), 0, 1023, 1, MAX_STEPS);
  byte stepC = (byte) map(mozziAnalogRead(stepPotC), 0, 1023, 1, MAX_STEPS);
  byte stepD = (byte) map(mozziAnalogRead(stepPotD), 0, 1023, 1, MAX_STEPS);

  byte beatA = (byte) map(mozziAnalogRead(beatPotA), 0, 1023, 0, stepA);
  byte beatB = (byte) map(mozziAnalogRead(beatPotB), 0, 1023, 0, stepB);
  byte beatC = (byte) map(mozziAnalogRead(beatPotC), 0, 1023, 0, stepC);
  byte beatD = (byte) map(mozziAnalogRead(beatPotD), 0, 1023, 0, stepD);

  tempo = millisTo_BPM_ToMillis(tempo);
  printTempo = tempo;
  
  if (swingStep % 2 == 0) {
    tempo = tempo - swing;
  } else {
    tempo = tempo + swing;
  }
  if (digitalRead(3) == LOW) {
    soundA = &aHatBongo;
    sampleIdA = 2;
  } else if (digitalRead(4) == LOW) {
    soundA = &aBongo;
    sampleIdA = 1;
  } else {
    soundA = &aKick;
    sampleIdA = 0;
  }

  if (digitalRead(5) == LOW) {
    soundB = &aConga;
    sampleIdB = 2;
  } else if (digitalRead(6) == LOW) {
    soundB = &aRim;
    sampleIdB = 1;
  } else {
    soundB = &aSnare;
    sampleIdB = 0;
  }

  if (digitalRead(7) == LOW) {
    soundC = &aCymbal;
    sampleIdC = 2;
  } else if (digitalRead(8) == LOW) {
    soundC = &aPercHat;
    sampleIdC = 1;
  } else {
    soundC = &aHiHat;
    sampleIdC = 0;
  } 

  if (digitalRead(40) == LOW) {
    soundD = &aTambourine;
    sampleIdD = 2;
  } else if (digitalRead(42) == LOW) {
    soundD = &aCowbell;
    sampleIdD = 1;
  } else {
    soundD = &aClap;
    sampleIdD = 0;
  }
  /*
  readSwitches(soundA, aHatBongo, aBongo, aKick, 3, 4, sampleIdA);
  readSwitches(soundB, aConga, aRim, aSnare, 5, 6, sampleIdB);
  readSwitches(soundC, aCymbal, aPercHat, aHiHat, 7, 8, sampleIdC);
  readSwitches(soundD, aTambourine, aCowbell, aClap, 40, 42, sampleIdD);
  */

  sendData();

   // If switch not at ON position, stop playback, turn leds off
  if (readOnSwitch == 0) {
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, LOW);
    digitalWrite(ledC, LOW);
    digitalWrite(ledD, LOW);
    return;
  }

  const int arraySize = 4;
  char pointers[arraySize] = { pointerA, pointerB, pointerC, pointerD };
  char steps[arraySize] = { stepA, stepB, stepC, stepD };

  if(kTriggerDelay.ready()) {
    for (int i = 0; i < arraySize; i++) {
      if (pointers[i] >= steps[i])  {
        pointers[i] = 0;
      }
    }

    playSample(stepA, beatA, pointerA, soundA, pitchReadA, ledA);
    playSample(stepB, beatB, pointerB, soundB, pitchReadB, ledB);
    playSample(stepC, beatC, pointerC, soundC, pitchReadC, ledC);
    playSample(stepD, beatD, pointerD, soundD, pitchReadD, ledD);

    pointerA++;
    pointerB++;
    pointerC++;
    pointerD++;
    swingStep++;

    kTriggerDelay.start(millisTo_BPM_ToMillis(tempo));
  }
}

int updateAudio() {
  int gain = (int)
    ( (long)((*soundA).next() * volume) +
            ((*soundB).next() * volume) +
            ((*soundC).next() * volume) +
            ((*soundD).next() * volume) ) >> 8; // try >> 8

  int arev = reverb.next(gain);

  // Mozzi default output range is -244 to 243
  if (gain > 243) {
    gain = 243;
  } else if (gain < -244) {
    gain = -244; 
  }
  return gain + (arev >> reverbSet);
}

void loop() {
  audioHook();
}