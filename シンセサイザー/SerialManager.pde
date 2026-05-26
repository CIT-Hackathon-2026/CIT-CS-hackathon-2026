class SerialManager {
  
  SerialManager() {}
  
  void processInput(String dataString) {
    String[] data = split(dataString, ',');
    
    // 3つのデータ（長さ、振幅、音名）が揃っているか確認
    if (data.length == 3) {
      float noteDur = float(data[0]);
      float noteAmp = float(data[1]); // Arduinoから届いた強弱
      String pitchName = data[2];
      
      float freqHz = Frequency.ofPitch(pitchName).asHz();
      
      // 第2引数に DEFAULT_AMP の代わりに noteAmp を渡す
      out.playNote(0.0f, noteDur, new SynthVoice(freqHz, noteAmp, synthWavetable));
    }
  }
}
