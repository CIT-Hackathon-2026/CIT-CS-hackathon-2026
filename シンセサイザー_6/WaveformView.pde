class WaveformView {
  AudioOutput targetOut;
  
  WaveformView(AudioOutput targetOut) {
    this.targetOut = targetOut;
  }
  
  void display() {
    stroke(255);
    for(int i = 0; i < targetOut.bufferSize() - 1; i++) {
      line(i, 50 + targetOut.left.get(i)*50, i+1, 50 + targetOut.left.get(i+1)*50);
    }
  }
}
