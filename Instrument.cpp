
#include "Instrument.h"

waveforms_e Instrument::getWaveForm() {
  return _waveform;
}


void Instrument::setWaveForm(waveforms_e waveform) {
 _waveform = waveform;
}

