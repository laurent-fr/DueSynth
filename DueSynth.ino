#include <MIDI.h>


#include "Channel.h"
#include "Instrument.h"
#include "Oscillator.h"


MIDI_CREATE_INSTANCE(HardwareSerial, Serial1, MIDI);

Channel channels[NB_CHANNELS];
Instrument instruments[NB_INSTRUMENTS];

Oscillator testosc;
Instrument testinstr;

// -----------------------------------------------------------------------------

void dac_setup ()
{
 analogWriteResolution(12);  
  pmc_enable_periph_clk(DACC_INTERFACE_ID);
  dacc_reset(DACC_INTERFACE);
  dacc_set_transfer_mode(DACC_INTERFACE, 0);
  dacc_set_power_save(DACC_INTERFACE, 0, 0);
  dacc_set_timing(DACC_INTERFACE, 0x0, 1, 0x0);

  dacc_set_analog_control(DACC_INTERFACE, DACC_ACR_IBCTLCH0(0x02) |
                   DACC_ACR_IBCTLCH1(0x02) |
                   DACC_ACR_IBCTLDACCORE(0x01));
  dacc_disable_trigger(DACC_INTERFACE);
  dacc_set_channel_selection(DACC_INTERFACE, 0);
  dacc_enable_channel(DACC_INTERFACE, 0);
}


void TimerStart(Tc *tc, uint32_t channel, IRQn_Type irq, uint32_t freq)
{
   pmc_set_writeprotect(false);
   pmc_enable_periph_clk(irq);
   TC_Configure(tc, channel, TC_CMR_WAVE | TC_CMR_WAVSEL_UP_RC |
                             TC_CMR_TCCLKS_TIMER_CLOCK3 
                             );
   uint32_t rc = VARIANT_MCK / 32 / freq;
   TC_SetRA(tc, channel, rc >> 1); // 50% duty cycle square wave
   TC_SetRC(tc, channel, rc);
   TC_Start(tc, channel);
   tc->TC_CHANNEL[channel].TC_IER=  TC_IER_CPCS;
   tc->TC_CHANNEL[channel].TC_IDR= ~ TC_IER_CPCS;
   NVIC_EnableIRQ(irq);
}

// -----------------------------------------------------------------------------


//TC1 ch 0
void TC3_Handler()
{

  TC_GetStatus(TC1, 0);

  int out=0;

 for(int c=0;c<NB_CHANNELS;c ++) {
    /*Oscillator &osc = channels[c].firstOscillator();
    while(channels[c].hasNextOscillator() ) {
      out+=osc.play();
     osc = channels[c].nextOscillator();
   }*/

  if (channels[c].isUsed()==false) continue;
   
  for(int o=0;o<NB_OSCILLATORS;o ++) {
  
    Oscillator &osc = channels[c].getOscillator(o);
    if (osc.isUsed() == true) out+=osc.play();
    
   }
 }


//Oscillator &osc = channels[1].firstOscillator();
//out=osc.play();
 // out=testosc.play();

  out = out/32+2048;
  if (out<0) out=0;
  if (out>4095) out=4095;

  dacc_write_conversion_data(DACC_INTERFACE, out);
  
}



void handleNoteOn(byte inChannel, byte inNote, byte inVelocity)
{

 // Serial.print("ON :");
 // Serial.print(inChannel);
 // Serial.print(",");
 // Serial.println(inNote);
  channels[inChannel].setUsed(true);
  if (inVelocity>0)
    channels[inChannel].startNote(inNote,inVelocity);
  else
    channels[inChannel].endNote(inNote);
  
}

void handleNoteOff(byte inChannel, byte inNote, byte inVelocity)
{
 // Serial.print("OFF :");
 // Serial.println(inNote);
  channels[inChannel].endNote(inNote);
}

void handleProgramChange( byte inChannel, byte inNumber) {
  Serial.print("Program Change : ");
  Serial.print(inChannel);
  Serial.print(",");
  Serial.println(inNumber);
  
}

// -----------------------------------------------------------------------------

void setup()
{
  Serial.begin(115200);

  pinMode(13, OUTPUT);

  dac_setup();
  TimerStart(TC1, 0, TC3_IRQn, 22050);

  instruments[3].setWaveForm(sinus);
  instruments[4].setWaveForm(noise);

  for(int i=0;i<NB_CHANNELS;i ++) {
    channels[i].useInstrument(instruments[i]);
  }
    
  MIDI.setHandleNoteOn(handleNoteOn);
  MIDI.setHandleNoteOff(handleNoteOff);
  MIDI.setHandleProgramChange(handleProgramChange);
  MIDI.begin(MIDI_CHANNEL_OMNI);

  //testosc.startNote(testinstr,69,100);
 
}

void loop()
{
    MIDI.read();
}
