
#include "Channel.h"

void Channel::useInstrument(Instrument &instrument) {
  _instrument = &instrument;
}

void Channel::setUsed(bool used) {
    _used=used;
}

bool Channel::isUsed() {
  return _used;
}


Oscillator &Channel::firstOscillator() {
  _current_osc=0;
  return _oscillators[0];
}

Oscillator &Channel::getOscillator(int number) {
  return _oscillators[number];
}

Oscillator &Channel::nextOscillator() {

  _current_osc++;
  while(_current_osc<NB_OSCILLATORS) {
    if ( _oscillators[_current_osc].isUsed() ) return _oscillators[_current_osc];
    _current_osc++;
  }

  return _oscillators[0];
}
  
bool Channel::hasNextOscillator() {
  if (_current_osc<NB_OSCILLATORS) return true;
  return false;
}

Oscillator& Channel::allocateOscillator(uint8_t note) {
  int o=0;
  while(o<NB_OSCILLATORS) {
    if (_oscillators[o].isUsed() == false) return _oscillators[o];
    o++;
  }

  return _oscillators[0];
}


Oscillator& Channel::findOscillator(uint8_t note) {
  int o=0;
   while(o<NB_OSCILLATORS) {
    //Serial.print(o);
    if ((_oscillators[o].isUsed() == true) && (_oscillators[o].getNote()==note)) return _oscillators[o];
    o++;
  }

  return  _oscillators[0];
}


void Channel::startNote(uint8_t note,uint8_t velocity) {
  Oscillator &osc = allocateOscillator(note);
  osc.startNote(*_instrument,note,velocity);
}

void Channel::endNote(uint8_t note) {
  Oscillator &osc = findOscillator(note);
  osc.endNote();
}

