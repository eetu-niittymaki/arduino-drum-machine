#include <MozziGuts.h>
#include <Sample.h>
#include <EventDelay.h>
#include <ReverbTank.h>
#include "Utilities.h"

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

#define NUM_CELLS 4096 // Make sure this is the same as in .h wavetable files
#define SAMPLERATE 32768 //16384
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

#define auxPotA 12
#define auxPotB 13
#define auxPotC 14
#define auxPotD 15

// Digital
#define onSwitch 2
#define tempoButton 4
#define oledStateButton 29
#define auxStateButton 45
#define ledA 28
#define ledB 12
#define ledC 9
#define ledD 35
#define ledVol 47
#define ledPitch 49


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
EventDelay delayOled;
ReverbTank reverb;
Utilities utility;

uint8_t readOnSwitch;
bool tapState = false;
uint8_t oledState = 0;
uint8_t lastOledState = 0;
uint8_t auxState = 0;
uint8_t lastAuxState = 0;
unsigned short int tempoOrig;
unsigned short int oldTempo;
unsigned short int newTempo;

uint8_t volume;
uint8_t reverbSet;
const unsigned short int recorded_pitch = SAMPLERATE / NUM_CELLS;
uint8_t swingStep = 1;

uint8_t pointerA = 0;
uint8_t pointerB = 0;
uint8_t pointerC = 0;
uint8_t pointerD = 0;

uint8_t maxStepA = 16;
uint8_t maxStepB = 16;
uint8_t maxStepC = 16;
uint8_t maxStepD = 16;

uint8_t maxStepReadA;
uint8_t maxStepReadB;
uint8_t maxStepReadC;
uint8_t maxStepReadD;

uint8_t oldMaxStepA;
uint8_t oldMaxStepB;
uint8_t oldMaxStepC;
uint8_t oldMaxStepD;

unsigned short int printTempo;
uint8_t sampleIdA;
uint8_t sampleIdB;
uint8_t sampleIdC;
uint8_t sampleIdD;

uint8_t volA = 255;
uint8_t volB = 255;
uint8_t volC = 255;
uint8_t volD = 255;

uint8_t oldVolA = 0;
uint8_t oldVolB = 0;
uint8_t oldVolC = 0;
uint8_t oldVolD = 0;

float pitchA = recorded_pitch;
float pitchB = recorded_pitch;
float pitchC = recorded_pitch;
float pitchD = recorded_pitch;

float oldPitchA = 0;
float oldPitchB = 0;
float oldPitchC = 0;
float oldPitchD = 0;

void setup() {
  pinMode(onSwitch, INPUT_PULLUP);
  pinMode(tempoButton, INPUT_PULLUP);
  pinMode(oledStateButton, INPUT_PULLUP);
  pinMode(auxStateButton, INPUT_PULLUP);
  pinMode(41, INPUT_PULLUP); // A
  pinMode(43, INPUT_PULLUP);
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
  pinMode(ledVol, OUTPUT);
  pinMode(ledPitch, OUTPUT);

  startMozzi(CONTROL_RATE);

  Serial.begin(9600);

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
  delayOled.set(8000);
}

void playSound(uint8_t step, uint8_t beat, uint8_t pointer,
                Sample <NUM_CELLS, AUDIO_RATE> *sound, float pitch, uint8_t led) {
  if(utility.startPlayback(step, beat, pointer)) {
    (*sound).start();
    (*sound).setFreq(utility.setPitch(pitch, recorded_pitch));
    digitalWrite(led, HIGH);
    digitalWrite(led, LOW);
  }
}

void readSwitches(Sample <NUM_CELLS, AUDIO_RATE> **sound,
                  Sample <NUM_CELLS, AUDIO_RATE> *sample1,
                  Sample <NUM_CELLS, AUDIO_RATE> *sample2,
                  Sample <NUM_CELLS, AUDIO_RATE> *sample3,
                  uint8_t switch1, uint8_t switch2, uint8_t *sampleId) {
  if (digitalRead(switch1) == LOW) {
    *sound = sample1;
    *sampleId = 2;
  } else if (digitalRead(switch2) == LOW) {
    *sound = sample2;
    *sampleId = 1;
  } else {
    *sound = sample3;
    *sampleId = 0;
  }
}

void sendData() {
  if (delayTx.ready()) {
    if (oledState == 0) {
      Serial.print('[');
      Serial.print(oledState);
      Serial.print(",");
      delayTx.start();
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
      Serial.print(']');
      Serial.println();
    } else {
      Serial.print('[');
      Serial.print(oledState);
      Serial.print(",");
      delayTx.start();
      Serial.print(maxStepA);
      Serial.print(",");
      delayTx.start();
      Serial.print(maxStepB);
      Serial.print(",");
      delayTx.start();
      Serial.print(maxStepC);
      Serial.print(",");
      delayTx.start();
      Serial.print(maxStepD);
      Serial.print(']');
      Serial.println();
    }
  }
}

bool lastButtonReading = false;
unsigned long beatLength = 500;
unsigned long lastTapMs = 0;
int beatsUntilReset = 10;
const int maxTapValues = 8;
unsigned long tapLengths[maxTapValues];
int tapIndex = 0;
int tapCount = 0;
bool lastTapSkipped = false;
unsigned short int minTempo = 1000;
unsigned short int maxTempo = 214;

void tapTempoStart() {
  unsigned long timer = millis();
  bool buttonDown = digitalRead(tempoButton) == LOW;

  if (buttonDown && !lastButtonReading) { // Check if button has been pressed again
    calculateTaps(timer);
  }

  lastButtonReading = buttonDown;
}

void calculateTaps(unsigned long timer) {
  unsigned long duration = timer - lastTapMs;
  unsigned short int timeMin = lastTapMs + minTempo;
  unsigned short int timeNext = lastTapMs + (beatLength * beatsUntilReset);
  if (timeMin < timer && timeNext < timer) {
    resetTaps(timer);
  }

  lastTapMs = timer;
  tapCount++;
  if(tapCount == 1) {
    tapState = false;
    return;
  }

  if(tapCount > 2 && !lastTapSkipped
      && duration > beatLength * 1.75
      && duration < beatLength * 2.75) {
    duration = duration >> 1;
    lastTapSkipped = true;
  } else {
    lastTapSkipped = false;
  }

  tapLengths[tapIndex] = duration;
  tapIndex++;

  if(tapIndex == maxTapValues) {
    tapIndex = 0;
  }

  beatLength = getTapAverage();
  newTempo = utility.millisTo_BPM_ToMillis(beatLength) << 1;
}

unsigned short int getTapAverage() {
  int amount = tapCount - 1;
  if(amount > maxTapValues) {
    amount = maxTapValues;
  }

  unsigned short int tapTotalMs = 0;
  for(int i = 0; i < amount; i++) {
    tapTotalMs += tapLengths[i];
  }
  unsigned short int tapAverageMs = tapTotalMs / amount;
  if(tapAverageMs < maxTempo) {
    return maxTempo;
  }
  return tapAverageMs;
}

void resetTaps(unsigned long timer) {
  tapCount = 0;
  tapIndex = 0;
  for(int i = 0; i < maxTapValues; i++) {
    tapLengths[i] = 0;
  }
}

uint8_t oldStepA = 0;
uint8_t oldStepB = 0;
uint8_t oldStepC = 0;
uint8_t oldStepD = 0;

void checkPotChanges() {
  if(delayOled.ready()) {
    delayOled.start();
    if(abs(maxStepA - oldStepA) > 0  // If no changes to pots: display default screen
      || abs(maxStepB - oldStepB) > 0
      || abs(maxStepC - oldStepC) > 0
      || abs(maxStepD - oldStepD) > 0) {
      oldStepA = maxStepA;
      oldStepB = maxStepB;
      oldStepC = maxStepD;
      oldStepD = maxStepD;
      delayOled.start();
    } else {
      oledState = 0;
    }
  }
}
/*
int potValue = 0
float valueFiltered = 0;

void lowPassFilter() {
  potValue = mozziAnalogRead(volumePot);
  const float alpha = 0.1;
  valueFiltered = alpha * potValue + (1 — alpha) * valueFiltered;
}
*/
void updateControl() {
  uint8_t tapRead = digitalRead(tempoButton);
  uint8_t oledRead = digitalRead(oledStateButton) == LOW;
  uint8_t auxRead = digitalRead(auxStateButton) == LOW;
  if (oledRead && !lastOledState) {
    oledState = !(oledState);
    oldMaxStepA = maxStepReadA;
    oldMaxStepB = maxStepReadB;
    oldMaxStepC = maxStepReadC;
    oldMaxStepD = maxStepReadD;
    oldStepA = maxStepA;
    oldStepD = maxStepB;
    oldStepC = maxStepC;
    oldStepD = maxStepD;
  }
  lastOledState = oledRead;

  if (auxRead && !lastAuxState) {
    auxState = !(auxState);
    oldVolA = volA;
    oldVolB = volB;
    oldVolC = volC;
    oldVolD = volD;
    oldPitchA = pitchA;
    oldPitchB = pitchB;
    oldPitchC = pitchC;
    oldPitchD = pitchD;
  }
  lastAuxState = auxRead;

  /////////////////////////////
  // Potentiometer readings //
  ///////////////////////////
  tempoOrig = map(mozziAnalogRead(tempoPot), 0, 1023, 107, 500); // 500 - 107 milliseconds gives a range of 60 - 280 BPM at 1/4 notes
  float swing = utility.mapf(mozziAnalogRead(swingPot), 0, 1023, 1.00, 1.70); // 70 % upper range, same as in the Linn LM-1 drum machine
  reverbSet = map(mozziAnalogRead(reverbPot), 0, 1023, 6, 0);
  volume = map(mozziAnalogRead(volumePot), 0, 1023, 0, 10);

  unsigned int auxPotReadA = mozziAnalogRead(auxPotA);
  unsigned int auxPotReadB = mozziAnalogRead(auxPotB);
  unsigned int auxPotReadC = mozziAnalogRead(auxPotC);
  unsigned int auxPotReadD = mozziAnalogRead(auxPotD);

  // CDhange whether pots control individual channel volumes or pitch
  if (auxState == 0) {
    volA = auxPotReadA >> 2; // Bitshift from 0 - 1023 range to 0 - 255
    volB = auxPotReadB >> 2;
    volC = auxPotReadC >> 2;
    volD = auxPotReadD >> 2;
    digitalWrite(ledVol, HIGH);
    digitalWrite(ledPitch, LOW);
  } else {
    pitchA = auxPotReadA;
    pitchB = auxPotReadB;
    pitchC = auxPotReadC;
    pitchD = auxPotReadD;
    digitalWrite(ledPitch, HIGH);
    digitalWrite(ledVol, LOW);
  }

  unsigned int stepReadA = mozziAnalogRead(stepPotA);
  unsigned int stepReadB = mozziAnalogRead(stepPotB);
  unsigned int stepReadC = mozziAnalogRead(stepPotC);
  unsigned int stepReadD = mozziAnalogRead(stepPotD);

  maxStepReadA = (uint8_t) map(stepReadA, 0, 1023, 1, MAX_STEPS);
  maxStepReadB = (uint8_t) map(stepReadB, 0, 1023, 1, MAX_STEPS);
  maxStepReadC = (uint8_t) map(stepReadC, 0, 1023, 1, MAX_STEPS);
  maxStepReadD = (uint8_t) map(stepReadD, 0, 1023, 1, MAX_STEPS);

  uint8_t stepA;
  uint8_t stepB;
  uint8_t stepC;
  uint8_t stepD;

  // Changes channel max steps from 1 - 16 or num of steps from 1 - to max steps
  // Channels having different max steps enables you to create polyrhythms
  if (oledState == 0) {
    stepA = (uint8_t) map(stepReadA, 0, 1023, 1, maxStepA);
    stepB = (uint8_t) map(stepReadB, 0, 1023, 1, maxStepB);
    stepC = (uint8_t) map(stepReadC, 0, 1023, 1, maxStepD);
    stepD = (uint8_t) map(stepReadD, 0, 1023, 1, maxStepC);
  } else { // Only updates value if potentiometer is moved, and not immediately after state button is pressed
    if (maxStepReadA != oldMaxStepA) maxStepA = maxStepReadA;
    if (maxStepReadB != oldMaxStepB) maxStepB = maxStepReadB;
    if (maxStepReadC != oldMaxStepC) maxStepC = maxStepReadC;
    if (maxStepReadD != oldMaxStepD) maxStepD = maxStepReadD;
    checkPotChanges();
  }

  uint8_t beatA = (uint8_t) map(mozziAnalogRead(beatPotA), 0, 1023, 0, stepA);
  uint8_t beatB = (uint8_t) map(mozziAnalogRead(beatPotB), 0, 1023, 0, stepB);
  uint8_t beatC = (uint8_t) map(mozziAnalogRead(beatPotC), 0, 1023, 0, stepC);
  uint8_t beatD = (uint8_t) map(mozziAnalogRead(beatPotD), 0, 1023, 0, stepD);

  tempoOrig = utility.millisTo_BPM_ToMillis(tempoOrig);

  if (tapRead == LOW) { // if tap tempo button is pressed
    tapState = true;
    oldTempo = tempoOrig;
  }

  if (tapState == false) {
    newTempo = tempoOrig;
  } else {
    tapTempoStart();
  }

  uint8_t tapCalc = abs(tempoOrig - oldTempo); // If tempo pot change is more than 3 read that instead of tap tempo
  if (tapCalc > 3) {
    tapState = false;
  }

  printTempo = newTempo >> 1;

  if (swingStep > MAX_STEPS) swingStep = 1;

  if (swingStep % 2 == 0) {
    newTempo = newTempo / swing;
  } else {
    newTempo = newTempo * swing;
  }

  // Set what sample gets played on given channel according to switch readings,set sample index for oled_screen.ino
  readSwitches(&soundA, &aHatBongo, &aBongo, &aKick, 41, 43, &sampleIdA);
  readSwitches(&soundB, &aConga, &aRim, &aSnare, 5, 6, &sampleIdB);
  readSwitches(&soundC, &aCymbal, &aPercHat, &aHiHat, 7, 8, &sampleIdC);
  readSwitches(&soundD, &aTambourine, &aCowbell, &aClap, 40, 42, &sampleIdD);

  sendData();

  readOnSwitch = digitalRead(onSwitch);

   // If switch not at ON position stop playback, turn off LEDs
  if (readOnSwitch == HIGH) {
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, LOW);
    digitalWrite(ledC, LOW);
    digitalWrite(ledD, LOW);
    return;
  }

  const uint8_t arraySize = 4;
  uint8_t pointers[arraySize] = { pointerA, pointerB, pointerC, pointerD };
  uint8_t steps[arraySize] = { stepA, stepB, stepC, stepD };

  if(kTriggerDelay.ready()) {
    for (int i = 0; i < arraySize; i++) {
      if (pointers[i] >= steps[i])  {
        pointers[i] = 0;
      }
    }

    playSound(stepA, beatA, pointerA, soundA, pitchA, ledA);
    playSound(stepB, beatB, pointerB, soundB, pitchB, ledB);
    playSound(stepC, beatC, pointerC, soundC, pitchC, ledC);
    playSound(stepD, beatD, pointerD, soundD, pitchD, ledD);

    pointerA++;
    pointerB++;
    pointerC++;
    pointerD++;
    swingStep++;

    kTriggerDelay.start(utility.millisTo_BPM_ToMillis(newTempo));
  }
}

int updateAudio() {
  // Don't really know how all this works but after lots of  testing it just does, without too much distortion to audio output
  int gainA = (long)((*soundA).next() * volA) >> 2;
  int gainB = (long)((*soundB).next() * volB) >> 2;
  int gainC = (long)((*soundC).next() * volC) >> 2;
  int gainD = (long)((*soundD).next() * volD) >> 2;

  int gain = ((gainA + gainB + gainC + gainD) >> 5) * volume ; //8

  // Mozzi default output range is -244 to 243 in STANDARD mode or -8192 to 8191 in HIFI mode
  if (gain > 8191) {
    gain = 8191;
  } else if (gain < -8192) {
    gain = -8192;
  }

  int arev = reverb.next(gain);

  if (reverbSet == 6) {
    return gain;
  } else {
    return gain + (arev >> reverbSet);
  }
}

void loop() {
  audioHook();
}