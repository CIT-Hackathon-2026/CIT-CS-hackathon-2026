class AdditiveEngine {
  Oscil wave;
  Oscil vibrato;

  Noise breath;
  Summer mix;

  AdditiveEngine(float freqHz, Waveform wf) {
    wave = new Oscil(freqHz, 1.0f, wf);

    vibrato = new Oscil(VIBRATO_FREQ, VIBRATO_AMP, Waves.SINE);
    vibrato.offset.setLastValue(freqHz);
    vibrato.patch(wave.frequency);

    // ノイズとミキサーを生成して、波形と合流させる
    breath = new Noise(0.030f, Noise.Tint.WHITE);   // ★ 0.005f → 0.030f
    mix = new Summer();
    wave.patch(mix);
    breath.patch(mix);
  }

  UGen getOutput() {
    return mix;
  }
}
