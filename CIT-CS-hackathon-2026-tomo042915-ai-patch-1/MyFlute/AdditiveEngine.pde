class AdditiveEngine {
  Oscil wave;
  Oscil vibrato;

  AdditiveEngine(float freq, Waveform wf) {
    wave = new Oscil(freq, 1.0f, wf);

    vibrato = new Oscil(VIBRATO_FREQ, VIBRATO_AMP, Waves.SINE);
    vibrato.offset.setLastValue(freq);
    vibrato.patch(wave.frequency);
  }

  UGen getOutput() {
    return wave;
  }
}
