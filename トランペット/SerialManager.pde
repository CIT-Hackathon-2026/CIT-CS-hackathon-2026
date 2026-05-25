/**
 * 役割: 通信データの解析と発音指示
 * 規約: {役割}Manager
 */
class SerialManager {
  
  SerialManager() {
    // 初期化処理（今回は特になし）
  }
  
  void processInput(String dataString) {
    String[] data = split(dataString, ',');
    if (data.length == 2) {
      float noteDur = float(data[0]);
      String pitchName = data[1];
      
      float freqHz = Frequency.ofPitch(pitchName).asHz();
      
      // SynthVoice を生成して鳴らす
      out.playNote(0.0f, noteDur, new SynthVoice(freqHz, DEFAULT_AMP, synthWavetable));
    }
  }
}
