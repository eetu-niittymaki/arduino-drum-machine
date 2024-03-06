#include <EEPROM.h>
#include <MozziGuts.h>
#include <Sample.h>
#include <EventDelay.h>
#include <Oscil.h>
#include <IntMap.h>
#include <tables/cos2048_int8.h>
#include <StateVariable.h>
#include <mozzi_rand.h>
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

#define NUM_CELLS 4096    // Make sure this is the same as in .h wavetable files
#define SAMPLERATE 32768  //16384
#define CONTROL_RATE 256

#define MAX_STEPS 16

// Analog
#define filterPotRes 0
#define filterPotFreq 10
#define tempoPot 9
#define swingPot 11

#define pitchPotA 1
#define pitchPotB 2
#define pitchPotC 3
#define pitchPotD 4

#define stepPotA 5
#define stepPotB 6
#define stepPotC 7
#define stepPotD 8

#define volPotA 12
#define volPotB 13
#define volPotC 14
#define volPotD 15

// Digital
#define onSwitch 2
#define tempoButton 4
#define oledStateButton 29
#define filterStateButton 31
#define ledA 28
#define ledB 12
#define ledC 9
#define ledD 35
#define ledFilter 27

//A
Sample<NUM_CELLS, AUDIO_RATE> aBongo(bongo_DATA);
Sample<NUM_CELLS, AUDIO_RATE> aHatBongo(hat_bongo_DATA);
Sample<NUM_CELLS, AUDIO_RATE> aKick(kick_DATA);

//B
Sample<NUM_CELLS, AUDIO_RATE> aConga(conga_DATA);
Sample<NUM_CELLS, AUDIO_RATE> aRim(rim_DATA);
Sample<NUM_CELLS, AUDIO_RATE> aSnare(snare_DATA);

//C
Sample<NUM_CELLS, AUDIO_RATE> aCymbal(cymbal_DATA);
Sample<NUM_CELLS, AUDIO_RATE> aPercHat(perc_hat_DATA);
Sample<NUM_CELLS, AUDIO_RATE> aHiHat(hihat_DATA);

//D
Sample<NUM_CELLS, AUDIO_RATE> aClap(clap_DATA);
Sample<NUM_CELLS, AUDIO_RATE> aTambourine(tambourine_DATA);
Sample<NUM_CELLS, AUDIO_RATE> aCowbell(cowbell_DATA);

// Default sample values
Sample<NUM_CELLS, AUDIO_RATE> *soundA = &aKick;
Sample<NUM_CELLS, AUDIO_RATE> *soundB = &aSnare;
Sample<NUM_CELLS, AUDIO_RATE> *soundC = &aHiHat;
Sample<NUM_CELLS, AUDIO_RATE> *soundD = &aClap;

Oscil<COS2048_NUM_CELLS, CONTROL_RATE> kFilterMod(COS2048_DATA);

StateVariable<HIGHPASS> hpf;
StateVariable<BANDPASS> bpf;
StateVariable<LOWPASS> lpf;

EventDelay playbackDelay;  // Schedules sampels to start
EventDelay delayTx;        // So serial receiver device doesn't get flooded with data
EventDelay delayOled;
Utilities utility;

bool tapState = false;
bool oledState = false;
bool lastOledState = false;
bool filterState = false;
bool lastFilterState = false;
uint8_t filterIndex;
unsigned short int tempoOriginal;
unsigned short int oldTempo;
unsigned short int newTempo;
unsigned short int printTempo;
float sendTempo1, sendTempo2;

const unsigned short int recorded_pitch = SAMPLERATE / NUM_CELLS;
uint8_t swingStep = 1;

uint8_t pointerA = 0;
uint8_t pointerB = 0;
uint8_t pointerC = 0;
uint8_t pointerD = 0;

int addressA = 1;
int addressB = 2;
int addressC = 3;
int addressD = 4;

uint8_t maxStepA = EEPROM.read(addressA);
uint8_t maxStepB = EEPROM.read(addressB);
uint8_t maxStepC = EEPROM.read(addressC);
uint8_t maxStepD = EEPROM.read(addressD);

uint8_t maxStepMappedA, maxStepMappedB, maxStepMappedC, maxStepMappedD;
uint8_t oldMaxStepA, oldMaxStepB, oldMaxStepC, oldMaxStepD;

uint8_t sampleIdA, sampleIdB, sampleIdC, sampleIdD;

uint8_t volA;
uint8_t volB;
uint8_t volC;
uint8_t volD;

// Avrg values
unsigned short int sumTempo, sumA, sumB, sumC, sumD;

uint8_t counterTempo, counterA, counterB, counterC, counterD;

void setup() {
  pinMode(onSwitch, INPUT_PULLUP);
  pinMode(tempoButton, INPUT_PULLUP);
  pinMode(oledStateButton, INPUT_PULLUP);
  pinMode(filterStateButton, INPUT_PULLUP);
  pinMode(41, INPUT_PULLUP); // A
  pinMode(43, INPUT_PULLUP);
  pinMode(5, INPUT_PULLUP);  // B
  pinMode(6, INPUT_PULLUP);
  pinMode(7, INPUT_PULLUP);  // C
  pinMode(8, INPUT_PULLUP);
  pinMode(40, INPUT_PULLUP); // D
  pinMode(42, INPUT_PULLUP);
  pinMode(38, INPUT_PULLUP);  // Filter
  pinMode(39, INPUT_PULLUP);
  //pinMode(10, OUTPUT);
  //pinMode(11, OUTPUT); //9 for Nano, 11 for Mega
  pinMode(ledA, OUTPUT);
  pinMode(ledB, OUTPUT);
  pinMode(ledC, OUTPUT);
  pinMode(ledD, OUTPUT);
  pinMode(ledFilter, OUTPUT);

  startMozzi(CONTROL_RATE);

  Serial.begin(9600);

  setFrequencies();

  playbackDelay.set(10);
  delayTx.set(400);
  delayOled.set(8000);
}

void playSound(uint8_t step, uint8_t beat, uint8_t pointer,
               Sample<NUM_CELLS, AUDIO_RATE> *sound, float pitch, uint8_t led) {
  if (utility.startPlayback(step, beat, pointer)) {
    (*sound).start();
    (*sound).setFreq(utility.setPitch(pitch, recorded_pitch));
    digitalWrite(led, HIGH);
    digitalWrite(led, LOW);
  }
}

void readSwitches(Sample<NUM_CELLS, AUDIO_RATE> **sound,
                  Sample<NUM_CELLS, AUDIO_RATE> *sample1,
                  Sample<NUM_CELLS, AUDIO_RATE> *sample2,
                  Sample<NUM_CELLS, AUDIO_RATE> *sample3,
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

void setSwing(float swing) {
  if (swingStep > MAX_STEPS) swingStep = 1;
  if (swingStep % 2 == 0) {
    newTempo /= swing;
  } else {
    newTempo *= swing;
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

  if (buttonDown && !lastButtonReading) {  // Check if button has been pressed again
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
  if (tapCount == 1) {
    tapState = false;
    return;
  }

  if (tapCount > 2 && !lastTapSkipped
      && duration > beatLength * 1.75
      && duration < beatLength * 2.75) {
    duration = duration >> 1;
    lastTapSkipped = true;
  } else {
    lastTapSkipped = false;
  }

  tapLengths[tapIndex] = duration;
  tapIndex++;

  if (tapIndex == maxTapValues) {
    tapIndex = 0;
  }

  beatLength = getTapAverage();
  newTempo = utility.millisTo_BPM_ToMillis(beatLength) << 1;
}

unsigned short int getTapAverage() {
  int amount = tapCount - 1;
  if (amount > maxTapValues) {
    amount = maxTapValues;
  }

  unsigned short int tapTotalMs = 0;
  for (int i = 0; i < amount; i++) {
    tapTotalMs += tapLengths[i];
  }
  unsigned short int tapAverageMs = tapTotalMs / amount;
  if (tapAverageMs < maxTempo) {
    return maxTempo;
  }
  return tapAverageMs;
}

void resetTaps(unsigned long timer) {
  tapCount = 0;
  tapIndex = 0;
  for (int i = 0; i < maxTapValues; i++) {
    tapLengths[i] = 0;
  }
}

void setFilterValues(unsigned int resonance, uint8_t frequency) {
  if (rand(CONTROL_RATE / 2) == 0) {
    kFilterMod.setFreq(frequency / 64);
  }

  int cutoffFreq = 2200 + kFilterMod.next() * 12;

  switch (filterIndex) {
    case 2:
      hpf.setResonance(resonance);
      hpf.setCentreFreq(frequency);
      break;
    case 1:
      lpf.setResonance(resonance);
      lpf.setCentreFreq(frequency);
      break;
    default:
      bpf.setResonance(resonance);
      bpf.setCentreFreq(frequency);
  }
}

uint8_t checkButtonPressed(uint8_t read, uint8_t state, uint8_t lastState) {
  if (read && !(lastState)) {
    return !(state);
  }
}

void updateControl() {
  const uint8_t arraySize = 4;
  uint8_t readOnSwitch = digitalRead(onSwitch);
  uint8_t tapRead = digitalRead(tempoButton);
  uint8_t oledRead = digitalRead(oledStateButton) == LOW;
  uint8_t filterRead = digitalRead(filterStateButton) == LOW;

  filterState = checkButtonPressed(filterRead, filterState, lastFilterState);
  oledState = checkButtonPressed(oledRead, oledState, lastOledState);
  if (oledRead == false) {
    oldMaxStepA = maxStepMappedA;
    oldMaxStepB = maxStepMappedB;
    oldMaxStepC = maxStepMappedC;
    oldMaxStepD = maxStepMappedD;
    if (oledState == 0 && lastOledState == 1) {
      //EEPROM.update(addressA, maxStepMappedA);
      //EEPROM.update(addressB, maxStepMappedB);
      //EEPROM.update(addressC, maxStepMappedC);
      //EEPROM.update(addressD, maxStepMappedD);
    }
  }

  lastOledState = oledRead;
  lastFilterState = filterRead;

  (filterState == false) ? digitalWrite(ledFilter, LOW) : digitalWrite(ledFilter, HIGH);

  /////////////////////////////
  // Potentiometer readings //
  ///////////////////////////
  tempoOriginal = map(mozziAnalogRead(tempoPot), 0, 1023, 107, 500); // 500 - 118 milliseconds gives a range of 60 - 255 BPM at 1/4 notes
  float swing = utility.mapFloat(mozziAnalogRead(swingPot), 0, 1023, 1.00, 1.70);  // 70 % upper range, same as in the Linn LM-1 drum machine
  unsigned int filterRes = map(mozziAnalogRead(filterPotRes), 0, 1023, 2, 212);
  uint8_t filterFreq = mozziAnalogRead(filterPotFreq) >> 2;

  unsigned int stepReadA = utility.getAverage(mozziAnalogRead(stepPotA), &sumA, &counterA);
  unsigned int stepReadB = utility.getAverage(mozziAnalogRead(stepPotB), &sumB, &counterB);
  unsigned int stepReadC = utility.getAverage(mozziAnalogRead(stepPotC), &sumC, &counterC);
  unsigned int stepReadD = utility.getAverage(mozziAnalogRead(stepPotD), &sumD, &counterD);
  maxStepMappedA = map(stepReadA, 0, 1023, 1, MAX_STEPS);
  maxStepMappedB = map(stepReadB, 0, 1023, 1, MAX_STEPS);
  maxStepMappedC = map(stepReadC, 0, 1023, 1, MAX_STEPS);
  maxStepMappedD = map(stepReadD, 0, 1023, 1, MAX_STEPS);

  uint8_t beatA, beatB, beatC, beatD;

  if (oledState == false) {  // Change how densely beats are scattered in relation to max steps
    beatA = (uint8_t) map(stepReadA, 0, 1023, 0, maxStepA);
    beatB = (uint8_t) map(stepReadB, 0, 1023, 0, maxStepB);
    beatC = (uint8_t) map(stepReadC, 0, 1023, 0, maxStepC);
    beatD = (uint8_t) map(stepReadD, 0, 1023, 0, maxStepD);
  } else {                                                         // Changes channel max steps from 1 - 16. Channels having different max steps enables you to create polyrhythms
    if (maxStepMappedA != oldMaxStepA) maxStepA = maxStepMappedA;  // Only updates value if potentiometer is moved, and not immediately
    if (maxStepMappedB != oldMaxStepB) maxStepB = maxStepMappedB;  // when state is changed
    if (maxStepMappedC != oldMaxStepC) maxStepC = maxStepMappedC;
    if (maxStepMappedD != oldMaxStepD) maxStepD = maxStepMappedD;
  }

  volA = mozziAnalogRead(volPotA) >> 2;  // Shift from 0 - 1023 range to 0 - 255
  volB = mozziAnalogRead(volPotB) >> 2;
  volC = mozziAnalogRead(volPotC) >> 2;
  volD = mozziAnalogRead(volPotD) >> 2;

  float pitchA = mozziAnalogRead(pitchPotA);
  float pitchB = mozziAnalogRead(pitchPotB);
  float pitchC = mozziAnalogRead(pitchPotC);
  float pitchD = mozziAnalogRead(pitchPotD);

  tempoOriginal = utility.millisTo_BPM_ToMillis(tempoOriginal);

  if (tapRead == LOW) {  // if tap tempo button is pressed
    tapState = true;
    oldTempo = tempoOriginal;
  }

  if (tapState == false) {
    newTempo = tempoOriginal;
  } else {
    tapTempoStart();
  }

  uint8_t tapCalc = abs(tempoOriginal - oldTempo);  // If tempo pot change is more than 3 read that instead of tap tempo
  if (tapCalc > 3) {
    tapState = false;
  }

  printTempo = utility.getAverage(newTempo >> 1, &sumTempo, &counterTempo);
  sendTempo1 = printTempo / 2; // Sending data over Serial is limited to one byte, so to send a value higher than 255 (tempo is 60 - 280)
  sendTempo2 = printTempo / 2; // I decided to split the value in two and then add them together on the receiver

  setSwing(swing);
  setFilterValues(filterFreq, filterRes);

  if (digitalRead(39) == LOW) {
    filterIndex = 2;
  } else if (digitalRead(38) == LOW) {
    filterIndex = 1;
  } else {
    filterIndex = 0;
  }

  // Set what sample gets played on given channel according to switch readings,set sample index for oled_screen.ino
  readSwitches(&soundA, &aHatBongo, &aBongo, &aKick, 41, 43, &sampleIdA);
  readSwitches(&soundB, &aConga, &aRim, &aSnare, 5, 6, &sampleIdB);
  readSwitches(&soundC, &aCymbal, &aPercHat, &aHiHat, 7, 8, &sampleIdC);
  readSwitches(&soundD, &aTambourine, &aCowbell, &aClap, 40, 42, &sampleIdD);

  sendData(readOnSwitch);

  // If switch not at ON position stop playback, turn off LEDs
  if (readOnSwitch == HIGH) {
    digitalWrite(ledA, LOW);
    digitalWrite(ledB, LOW);
    digitalWrite(ledC, LOW);
    digitalWrite(ledD, LOW);
    return;
  }

  uint8_t pointers[arraySize] = { pointerA, pointerB, pointerC, pointerD };
  uint8_t steps[arraySize] = { maxStepA, maxStepB, maxStepC, maxStepD };

  if (playbackDelay.ready()) {
    for (int i = 0; i < arraySize; i++) {
      if (pointers[i] >= steps[i]) {
        pointers[i] = 0;
      }
    }

    playSound(maxStepA, beatA, pointerA, soundA, pitchA, ledA);
    playSound(maxStepB, beatB, pointerB, soundB, pitchB, ledB);
    playSound(maxStepC, beatC, pointerC, soundC, pitchC, ledC);
    playSound(maxStepD, beatD, pointerD, soundD, pitchD, ledD);

    pointerA++;
    pointerB++;
    pointerC++;
    pointerD++;
    swingStep++;

    playbackDelay.start(utility.millisTo_BPM_ToMillis(newTempo));
  }
}

int updateAudio() {
  int gainA = (long)(*soundA).next() * volA >> 2;
  int gainB = (long)(*soundB).next() * volB >> 2;
  int gainC = (long)(*soundC).next() * volC >> 2;
  int gainD = (long)(*soundD).next() * volD >> 2;

  int gain = (gainA + gainB + gainC + gainD);

  // Mozzi default output range is -244 to 243 in STANDARD mode or -8192 to 8191 in HIFI mode
  if (gain > 8191) {
    gain = 8191;
  } else if (gain < -8192) {
    gain = -8192;
  }

  switch (filterState) {
    case false:
      return gain;
    case true:
      switch (filterIndex) { // Highpass, bandpass or lowpass filter
        case 2:
          return hpf.next(gain);
        case 1:
          return lpf.next(gain);
        default:
          return bpf.next(gain);
      }
  }
}

void setFrequencies() {
  aBongo.setFreq((float)bongo_SAMPLERATE / (float)NUM_CELLS);
  aHatBongo.setFreq((float)hat_bongo_SAMPLERATE / (float)NUM_CELLS);
  aKick.setFreq((float)kick_SAMPLERATE / (float)NUM_CELLS);

  aConga.setFreq((float)conga_SAMPLERATE / (float)NUM_CELLS);
  aRim.setFreq((float)rim_SAMPLERATE / (float)NUM_CELLS);
  aSnare.setFreq((float)snare_SAMPLERATE / (float)NUM_CELLS);

  aCymbal.setFreq((float)cymbal_SAMPLERATE / (float)NUM_CELLS);
  aPercHat.setFreq((float)perc_hat_SAMPLERATE / (float)NUM_CELLS);
  aHiHat.setFreq((float)hihat_SAMPLERATE / (float)NUM_CELLS);

  aClap.setFreq((float)clap_SAMPLERATE / (float)NUM_CELLS);
  aTambourine.setFreq((float)tambourine_SAMPLERATE / (float)NUM_CELLS);
  aCowbell.setFreq((float)cowbell_SAMPLERATE / (float)NUM_CELLS);

  kFilterMod.setFreq(1.3f);
}

void sendData(uint8_t readOnSwitch) {
  if (delayTx.ready()) {
    if (oledState == 0) {
      Serial.print('[');
      Serial.print(oledState);
      Serial.print(",");
      delayTx.start();
      Serial.print(readOnSwitch);
      Serial.print(",");
      delayTx.start();
      Serial.print(sendTempo1);
      Serial.print(",");
      delayTx.start();
      Serial.print(sendTempo2);
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

void loop() {
  audioHook();
}