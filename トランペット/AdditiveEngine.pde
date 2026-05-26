class AdditiveEngine {
  Oscil wave;
  Oscil vibrato;

  AdditiveEngine(float freqHz, Waveform wf) {
    wave = new Oscil(freqHz, 1.0f, wf);
    
    vibrato = new Oscil(VIBRATO_FREQ, VIBRATO_AMP, Waves.SINE);
    vibrato.offset.setLastValue(freqHz);
    vibrato.patch(wave.frequency);
  }
  
  UGen getOutput() {
    return wave;
  }
}
