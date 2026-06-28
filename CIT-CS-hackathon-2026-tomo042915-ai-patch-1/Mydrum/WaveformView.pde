class WaveformView {
  AudioOutput targetOut;

  WaveformView(AudioOutput targetOut) {
    this.targetOut = targetOut;
  }

  void display() {
    stroke(0, 255, 255); // ドラムらしくサイバーな水色の線にしました
    strokeWeight(2);
    for (int i = 0; i < targetOut.bufferSize() - 1; i++) {
      line(i,
           height/2 + targetOut.left.get(i) * 100,
           i + 1,
           height/2 + targetOut.left.get(i + 1) * 100);
    }
  }
}
