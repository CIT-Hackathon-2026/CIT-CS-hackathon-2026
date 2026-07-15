class SerialManager {
  FluteVoice currentVoice;   // 今鳴っている音（STOPで止めるために保持）

  SerialManager() {
    currentVoice = null;
  }

  void processInput(String data) {
    String cmd = trim(data);

    // ===== 「止めて」の合図 =====
    // Arduino が発音時間ぶん（PLAY_DURATIONS×TICKS_PER_BEAT）の光を数え終わると
    // "STOP" を送ってくる。それを受けて今鳴っている音を止める。
    if (cmd.equals("STOP")) {
      if (currentVoice != null) {
        currentVoice.noteOff();
        currentVoice = null;
      }
      return;
    }

    // ===== 音符データ（長さ, 振幅, 音名）=====
    String[] d = split(cmd, ',');
    if (d.length == 3) {
      // float dur = float(d[0]);  // ★発音長には使わない（実際の停止はSTOPで行う）
      float amp    = float(d[1]);
      String pitch = trim(d[2]);

      // 前の音がまだ鳴っていたら、先に止める（重なり防止）
      if (currentVoice != null) {
        currentVoice.noteOff();
        currentVoice = null;
      }

      // 休符（"R"）や音量0は鳴らさない。
      // ※ Frequency.ofPitch("R") は無効な音名で例外を投げるため、ここで弾く。
      if (pitch.equals("R") || amp <= 0.0f) {
        return;
      }

      // 念のため、想定外の音名でも落ちないよう保護
      try {
        float freq = Frequency.ofPitch(pitch).asHz();
        // ★ playNote(長さ指定・自動停止) はやめて、手動で noteOn する。
        //    STOP が来るまで鳴り続ける。
        currentVoice = new FluteVoice(freq, amp, fluteWavetable);
        currentVoice.noteOn(0.0f);
      } catch (Exception e) {
        println("音名を解釈できませんでした: " + pitch);
      }
    }
  }
}
