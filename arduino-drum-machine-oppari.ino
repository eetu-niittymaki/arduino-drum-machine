#include <MozziGuts.h>
#include <Sample.h>
#include <EventDelay.h>
#include <ReverbTank.h>

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

#define pitchPotA 12
#define pitchPotB 13
#define pitchPotC 14 
#define pitchPotD 15

// Digital
#define onSwitch 2
#define tempoButton 4
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

uint8_t readOnSwitch;
bool tapState = false;
unsigned short int tempo;
unsigned short int oldTempo;
unsigned short int newTempo;

uint8_t volume;
uint8_t reverbSet;
const int recorded_pitch = SAMPLERATE / NUM_CELLS;
uint8_t swingStep = 1;

uint8_t pointerA = 0;
uint8_t pointerB = 0;
uint8_t pointerC = 0;
uint8_t pointerD = 0;

unsigned short int printTempo;
uint8_t sampleIdA; 
uint8_t sampleIdB;
uint8_t sampleIdC; 
uint8_t sampleIdD;

void setup() {
  pinMode(onSwitch, INPUT_PULLUP);
  pinMode(tempoButton, INPUT_PULLUP);
  //pinMode(tapSwitch, INPUT);
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
}

bool startPlayback(uint8_t stepCount, uint8_t beatCount, uint8_t pointer) {
  float divider = ((float)beatCount / (float)stepCount);
  bool start;
  uint8_t count = pointer * divider;
  uint8_t prevCount = (pointer - 1) * divider;

  return { (pointer == 0) ? start = (beatCount != 0) : start = (count > prevCount) };
}

void playSound(uint8_t step, uint8_t beat, uint8_t pointer, 
                Sample <NUM_CELLS, AUDIO_RATE> *sound, unsigned int pitch, uint8_t led) {
  if(startPlayback(step, beat, pointer)) {
      (*sound).start();
      (*sound).setFreq(setPitch(pitch));
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

float setPitch(unsigned int oldPitch) {
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

unsigned int millisTo_BPM_ToMillis(unsigned short int value) {
  return {  60000 / value };
}

uint8_t buttonState;    
uint8_t lastButtonState = 1;
int tapBeat;

unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 50; 

unsigned long last_debounce_millis = 0;
const uint8_t taps_len = 24;
unsigned long taps_millis[taps_len];
uint8_t next_tap = 0;
uint8_t total_taps = 0;

void tapTempo() {
  unsigned long total = 0;
  uint8_t reading = digitalRead(tempoButton);
  
  if (reading != lastButtonState) {
    lastDebounceTime = millis();
  }

  if ((millis() - lastDebounceTime) > debounceDelay) {
    if (reading != buttonState) {
      buttonState = reading;
      if((millis() - last_debounce_millis) > 2000) {
        reset_taps();
      }

      taps_millis[next_tap] = millis();
      if (buttonState == LOW) {
        tapBeat = calculateTaps();
        next_tap = ++next_tap % taps_len;
        total_taps++;
        last_debounce_millis = millis();
      }
    }
  }
  
  lastButtonState = reading;
  newTempo = tapBeat;
}

int calculateTaps() {
  if(total_taps < 4) {
    return 0;
  }
  
  unsigned long total = 0;
  for(int i = 1; i <= 4; i++) {
    int tap = next_tap - i;
    
    if(tap < 0) {
      tap = taps_len + tap;
    }
    if(i > 1) {
      total += (taps_millis[tap + 1 % taps_len] - taps_millis[tap]);
    }
  }
  return 60000/(total / 3);
}

void reset_taps() {
  for(int i = 0; i < taps_len;i++) {
    taps_millis[i] = 0;
  }
  next_tap = 0;
  total_taps = 0;
}

void updateControl() {
  /////////////////////////////
  // Potentiometer readings //
  ///////////////////////////
  unsigned int volumeRead = mozziAnalogRead(volumePot);
  unsigned int tempoRead = mozziAnalogRead(tempoPot);
  unsigned int swingRead = mozziAnalogRead(swingPot);
  unsigned int reverbRead = mozziAnalogRead(reverbPot);
  tempo = map(tempoRead, 0, 1023, 214, 1000); // 214 - 1000 milliseconds gives a range of 60 - 280 BPM at 1/4 notes
  uint8_t swing = map(swingRead, 0, 1023, 0, 84);
  reverbSet = map(reverbRead, 0, 1023, 3, 0);
  volume =  map(volumeRead, 0, 1023, 0, 255);

  unsigned int pitchReadA = mozziAnalogRead(pitchPotA);
  unsigned int pitchReadB = mozziAnalogRead(pitchPotB);
  unsigned int pitchReadC = mozziAnalogRead(pitchPotC);
  unsigned int pitchReadD = mozziAnalogRead(pitchPotD); 

  uint8_t stepA = (uint8_t) map(mozziAnalogRead(stepPotA), 0, 1023, 1, MAX_STEPS);
  uint8_t stepB = (uint8_t) map(mozziAnalogRead(stepPotB), 0, 1023, 1, MAX_STEPS);
  uint8_t stepC = (uint8_t) map(mozziAnalogRead(stepPotC), 0, 1023, 1, MAX_STEPS);
  uint8_t stepD = (uint8_t) map(mozziAnalogRead(stepPotD), 0, 1023, 1, MAX_STEPS);

  uint8_t beatA = (uint8_t) map(mozziAnalogRead(beatPotA), 0, 1023, 0, stepA);
  uint8_t beatB = (uint8_t) map(mozziAnalogRead(beatPotB), 0, 1023, 0, stepB);
  uint8_t beatC = (uint8_t) map(mozziAnalogRead(beatPotC), 0, 1023, 0, stepC);
  uint8_t beatD = (uint8_t) map(mozziAnalogRead(beatPotD), 0, 1023, 0, stepD);

  uint8_t tapRead = digitalRead(tempoButton);
  tempo = millisTo_BPM_ToMillis(tempo);

  if (tapRead == LOW) { // if tap tempo button is pressed
    tapState = true;
    oldTempo = tempo;
  }

  if (tapState == false) {
    newTempo = tempo;
  } else {
    tapTempo();
  }

  uint8_t tapCalc = abs(tempo - oldTempo); // If tempo pot change is more than 3 read that instead of tap tempo
  if (tapCalc > 3) {
    tapState = false;
  }

  printTempo = newTempo;

  if (swingStep > MAX_STEPS) swingStep = 1;
  
  if (swingStep % 2 == 0) {
    newTempo = newTempo - swing;
  } else {
    newTempo = newTempo + swing;
  }

  // Set what sample gets played on given channel according to switch readings, set sample index for oled_screen.ino
  readSwitches(&soundA, &aHatBongo, &aBongo, &aKick, 41, 43, &sampleIdA);
  readSwitches(&soundB, &aConga, &aRim, &aSnare, 5, 6, &sampleIdB);
  readSwitches(&soundC, &aCymbal, &aPercHat, &aHiHat, 7, 8, &sampleIdC);
  readSwitches(&soundD, &aTambourine, &aCowbell, &aClap, 40, 42, &sampleIdD);

  sendData();

  readOnSwitch = digitalRead(onSwitch);

   // If switch not at ON position stop playback
  if (readOnSwitch == HIGH) {
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

    playSound(stepA, beatA, pointerA, soundA, pitchReadA, ledA);
    playSound(stepB, beatB, pointerB, soundB, pitchReadB, ledB);
    playSound(stepC, beatC, pointerC, soundC, pitchReadC, ledC);
    playSound(stepD, beatD, pointerD, soundD, pitchReadD, ledD);

    pointerA++;
    pointerB++;
    pointerC++;
    pointerD++;
    swingStep++;

    kTriggerDelay.start(millisTo_BPM_ToMillis(newTempo));
  }
}

AudioOutput_t updateAudio() {
  int gain = (int)
    ( (long)((*soundA).next() * volume) +
            ((*soundB).next() * volume) +
            ((*soundC).next() * volume) +
            ((*soundD).next() * volume) ) >> 8;

  // Mozzi default output range is -244 to 243
  if (gain > 243) {
    gain = 243;
  } else if (gain < -244) {
    gain = -244; 
  }

  int arev = reverb.next(gain);

  if (reverbSet == 3) {
    return MonoOutput::from8Bit(gain);
  } else {
    return MonoOutput::from8Bit(gain + (arev >> reverbSet));
  }
}

void loop() {
  audioHook();
}