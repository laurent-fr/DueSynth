#ifndef CHANNEL_H
#define CHANNEL_H

#include "config.h"

#include "Instrument.h"
#include "Oscillator.h"

class Channel {

  Oscillator _oscillators[NB_OSCILLATORS];
  Instrument *_instrument;
  int _current_osc = 0;
  bool _used=false;

  public:
  
  void useInstrument(Instrument &instrument);

  Oscillator &findOscillator(uint8_t note);
  Oscillator &allocateOscillator(uint8_t note);
  void removeOscillator(uint8_t note);
  void addOscillator(uint8_t note, uint8_t velocity);

  void setUsed(bool used);
  bool isUsed();

  Oscillator &firstOscillator();
  Oscillator &nextOscillator();
  Oscillator &getOscillator(int number);
  bool hasNextOscillator();

  void startNote(uint8_t note,uint8_t velocity);
  void endNote(uint8_t note);
  
};

#endif
