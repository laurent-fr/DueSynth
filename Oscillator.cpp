#include "Oscillator.h"


bool Oscillator::isUsed() {
  return _used;
}

uint8_t Oscillator::getNote() {
  return _note;
}

void Oscillator::startNote(Instrument &instrument,uint8_t note,uint8_t velocity) {
  _instrument = &instrument;
  _note = note;
  _velocity = velocity;
  _frequency = freq_table[note];
  _adsr_mode = mode_a;
  _used = true;
   _envelope_volume=16384; //TEMP


/*Serial.print("Oscillator::starNote(");
  Serial.print(note);
  Serial.print(",");
  Serial.print(velocity);
  Serial.println(")");*/
}

void Oscillator::endNote() {
  _used = false;
}

int Oscillator::play() {

  if (!_used) return 0;

  int output=0;

  
  
  _phase+=_frequency*256/22050;

  while (_phase>=65536) _phase-=65536;
  while (_phase<0) _phase+=65536;

 switch(_instrument->getWaveForm() ) {
    case square:   output = playSquare(); break;
    case sinus: output = playSin(); break;
    case triangle: output = playTriangle(); break;
    case saw: output=playSaw(); break;
    case noise: output=playNoise(); break;
  }


 // TODO : ADSR
  output = output*_envelope_volume/16384;

  // TEMP
  if ((_instrument->getWaveForm()==noise)&&(_envelope_volume>0)) {
    _envelope_volume-=4;
  }
  
  
  //Velocity
  output = output*_velocity/128;

  return output;
  
}




int Oscillator::playSin() {
  return (int)sin_table[_phase/64];
}

int Oscillator::playSaw() {
  return 0; //_phase*2.-1.;
}

int Oscillator::playTriangle() {
  if (_phase<32768) return _phase*2-32768;
  return 98304-_phase*2;
}

int Oscillator::playSquare() {
    if (_phase<32768) return 32768;
    return -32767;
}

int Oscillator::playNoise() {
  return (rand()%65536)-32768;
}

/*
float Oscillator::getEnvelope() {

    //if (_duration_left<=0) _adsr_mode=mode_r;

    switch(_adsr_mode) {

        case mode_a:
            _envelope_volume+=(float)_envelope_a*100./SAMPLE_RATE;
            if (_envelope_volume>=255) _adsr_mode=mode_d;
        break;

        case mode_d:
            _envelope_volume-=(float)_envelope_d*100./SAMPLE_RATE;
            if (_envelope_volume<=_envelope_s) _adsr_mode=mode_s;
        break;

        case mode_s:

        break;

        case mode_r:
            _envelope_volume-=(float)_envelope_r*100./SAMPLE_RATE;
            if (_envelope_volume<=0) _envelope_volume=0;
        break;


    }

    return _envelope_volume;
    
}
*/

