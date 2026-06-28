class AdditiveEngine {
  Summer mixer;
  Oscil wave;
  Noise breath;
  MoogFilter lpf;
  Constant filterTarget;
  ADSR filterEnv; // フィルター専用ADSR（ノイズのカットオフを変調）

  AdditiveEngine(float freqHz, float maxAmp, Waveform wf) {
    mixer = new Summer();

    // 倍音はフィルターを通さず、そのまま mixer へ
    wave = new Oscil(freqHz, maxAmp, wf);
    wave.patch(mixer);

    // ノイズだけローパスに通す
    breath = new Noise(SYNTH_NOISE_AMP, Noise.Tint.WHITE);
    lpf = new MoogFilter(0.0f, 0.0f, MoogFilter.Type.LP);
    filterTarget = new Constant(SYNTH_FILTER_TARGET_FREQ);
    filterEnv = new ADSR(1.0f, SYNTH_FILTER_ATK_TIME, SYNTH_FILTER_DEC_TIME, SYNTH_FILTER_SUS_LVL, SYNTH_FILTER_REL_TIME);
    filterTarget.patch(filterEnv).patch(lpf.frequency);

    breath.patch(lpf); // ノイズ → ローパス
    lpf.patch(mixer);  // その後 mixer に合流
  }

  // エンジンで作った音を外（ADSR等）に繋ぐためのメソッド
  UGen getOutput() {
    return mixer;
  }
}
