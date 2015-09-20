#ifndef INSTRUMENT_H
#define INSTRUMENT_H

typedef enum waveforms_e {
            square,
            triangle,
            saw,
            sinus,
            noise
        } wafeforms_t ;

class Instrument {

public:
        // instrument definition
        waveforms_e _waveform=square;
        float _duty_cycle=0.5;
        int _envelope_a=255;
        int _envelope_d=0;
        int _envelope_s=255;
        int _envelope_r=255;

  public:

        waveforms_e getWaveForm();
        void setWaveForm(waveforms_e waveform);
  
};


#endif
