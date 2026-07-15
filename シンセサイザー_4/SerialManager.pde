class SerialManager {
  
  SerialManager() {}
  
  void processInput(String dataString) {
    String[] data = split(dataString, ',');
    
    if (data.length == 3) {
      float noteDur = float(data[0]);
      float noteAmp = float(data[1]); 
      String pitchName = data[2];
      
      float freqHz = Frequency.ofPitch(pitchName).asHz();
      
      out.playNote(0.0f, noteDur, new SynthVoice(freqHz, noteAmp, synthWavetable));
    }
  }
}
