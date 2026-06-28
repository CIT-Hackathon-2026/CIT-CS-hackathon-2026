class SerialManager {
  SerialManager() {}

  void processInput(String data) {
    String[] d = split(data, ',');

    // この .ino は必ず4項目（Interval, Play, Flute, Drum）を送る
    if (d.length == 4) {
      float  dur      = float(d[0]); // STEP_INTERVALS を発音長に使う
      String drumType = trim(d[3]);  // 4番目＝ドラム種別

      if (drumType.equals("KICK")) {
        out.playNote(0.0, dur, new KickInstrument());
      } else if (drumType.equals("SNARE")) {
        out.playNote(0.0, dur, new SnareInstrument());
      } else if (drumType.equals("CYMBAL")) {
        out.playNote(0.0, dur, new CymbalInstrument());
      }
      // "NONE" は何も鳴らさない
    }
  }
}
