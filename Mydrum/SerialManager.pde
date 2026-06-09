class SerialManager {
  SerialManager() {}

  void processInput(String data) {
    String[] d = split(data, ',');
    
    // データが4つ(Interval, Play, Flute, Drum)届いているかチェック
    if (d.length == 4) {
      float dur = float(d[0]); // 基準の長さ
      String drumType = d[3];  // 4番目の「ドラムの種類」だけを抽出！

      // 送られてきた文字に合わせてお気に入りのドラム音源をトリガー
      if (drumType.equals("KICK")) {
        out.playNote(0.0, dur, new KickInstrument());
      } 
      else if (drumType.equals("SNARE")) {
        out.playNote(0.0, dur, new SnareInstrument());
      } 
      else if (drumType.equals("CYMBAL")) {
        out.playNote(0.0, dur, new CymbalInstrument());
      }
      // "NONE" のときは何も鳴らさずスルーします
    }
  }
}
