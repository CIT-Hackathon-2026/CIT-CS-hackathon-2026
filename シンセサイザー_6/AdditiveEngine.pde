class AdditiveEngine {
  Summer mixer;
  Oscil wave;
  Noise breath;
  Noise breath2;
  MoogFilter lpf;
  MoogFilter lpf2;   // breath2専用の固定ローパス
  Constant filterTarget;
  ADSR filterEnv;    // ノイズ1のカットオフを変調

  AdditiveEngine(float freqHz, float maxAmp, Waveform wf) {
    mixer = new Summer();

    // 倍音はフィルターを通さず、そのまま mixer へ
    wave = new Oscil(freqHz, maxAmp, wf);
    wave.patch(mixer);

    // ノイズ1：ローパス＋ADSRでカットオフを変調
    breath = new Noise(SYNTH_NOISE_AMP, Noise.Tint.WHITE);
    lpf = new MoogFilter(0.0f, 0.0f, MoogFilter.Type.LP);
    filterTarget = new Constant(SYNTH_FILTER_TARGET_FREQ);
    filterEnv = new ADSR(1.0f, SYNTH_FILTER_ATK_TIME, SYNTH_FILTER_DEC_TIME, SYNTH_FILTER_SUS_LVL, SYNTH_FILTER_REL_TIME);
    filterTarget.patch(filterEnv).patch(lpf.frequency);
    breath.patch(lpf);
    lpf.patch(mixer);

    // ノイズ2：固定ローパス（13500Hz）
    breath2 = new Noise(SYNTH_NOISE2_AMP, Noise.Tint.WHITE);
    lpf2 = new MoogFilter(SYNTH_NOISE2_CUTOFF, 0.0f, MoogFilter.Type.LP);
    breath2.patch(lpf2);
    lpf2.patch(mixer);
  }

  // エンジンで作った音を外（ADSR等）に繋ぐためのメソッド
  UGen getOutput() {
    return mixer;
  }
}
