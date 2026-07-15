class SerialManager {
  SerialManager() {}

  void processInput(String data) {
    String[] d = split(data, ',');
    if (d.length == 3) {
      float dur = float(d[0]);
      float amp = float(d[1]);
      String pitch = trim(d[2]);

      // 休符（"R"）や音量0は鳴らさない。
      // ※ Frequency.ofPitch("R") は無効な音名で例外を投げるため、ここで弾く。
      if (pitch.equals("R") || amp <= 0.0f) {
        return;
      }

      // 念のため、想定外の音名でも落ちないよう保護
      try {
        float freq = Frequency.ofPitch(pitch).asHz();
        out.playNote(0.0f, dur, new FluteVoice(freq, amp, fluteWavetable));
      } catch (Exception e) {
        println("音名を解釈できませんでした: " + pitch);
      }
    }
  }
}
