/**
 * 役割: 複数のオシレーターを組み合わせた音源の生成
 * 規約: {手法}Engine
 */
class AdditiveEngine {
  Summer mixer;
  Oscil wave1, wave2, wave3, tremoloOsc;
  
  AdditiveEngine(float freqHz, Waveform wf) {
    mixer = new Summer();
    
    wave1 = new Oscil(freqHz, SYNTH_OSC_AMP, wf);
    wave2 = new Oscil(freqHz * SYNTH_DETUNE_RATE, SYNTH_OSC_AMP, wf); 
    wave3 = new Oscil(freqHz, 0.0f, wf); 
    
    tremoloOsc = new Oscil(TREMOLO_FREQ, TREMOLO_AMP, Waves.SQUARE);
    tremoloOsc.offset.setLastValue(TREMOLO_AMP); 
    tremoloOsc.patch(wave3.amplitude);
    
    wave1.patch(mixer); 
    wave2.patch(mixer); 
    wave3.patch(mixer);
  }
  
  // エンジンで作った音を外（フィルター等）に繋ぐためのメソッド
  UGen getOutput() {
    return mixer;
  }
}
