class SerialManager {
  TrumpetVoice currentVoice;   // 今鳴っている音（STOPで止めるために保持）

  SerialManager() {
    currentVoice = null;
  }

  void processInput(String dataString) {
    String cmd = trim(dataString);

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
    String[] data = split(cmd, ',');
    if (data.length == 3) {
      // float noteDur = float(data[0]);  // ★発音長には使わない（停止はSTOPで行う）
      float noteAmp    = float(data[1]);
      String pitchName = trim(data[2]);

      // 前の音がまだ鳴っていたら、先に止める（重なり防止）
      if (currentVoice != null) {
        currentVoice.noteOff();
        currentVoice = null;
      }

      // 休符（"R"）や音量0は鳴らさない
      // ※ Frequency.ofPitch("R") は無効な音名で例外を投げるため弾く
      if (pitchName.equals("R") || noteAmp <= 0.0f) {
        return;
      }

      try {
        float freqHz = Frequency.ofPitch(pitchName).asHz();
        // ★ playNote(長さ指定・自動停止) はやめて手動で noteOn。STOPが来るまで鳴り続ける
        currentVoice = new TrumpetVoice(freqHz, noteAmp, trumpetWavetable);
        currentVoice.noteOn(0.0f);
      } catch (Exception e) {
        println("音名を解釈できませんでした: " + pitchName);
      }
    }
  }
}
