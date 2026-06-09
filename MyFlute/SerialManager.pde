class SerialManager {
  SerialManager() {}

  void processInput(String data) {
    String[] d = split(data, ',');
    if (d.length == 3) {
      float dur = float(d[0]);
      float amp = float(d[1]);
      String pitch = d[2];

      float freq = Frequency.ofPitch(pitch).asHz();

      // FluteVoiceに音のデータを渡して再生
      out.playNote(0.0f, dur, new FluteVoice(freq, amp, fluteWavetable));
    }
  }
}
