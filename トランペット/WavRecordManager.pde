class WavRecordManager {
  AudioRecorder recorder;
  boolean isEnabled = true;

  WavRecordManager(Minim minim, AudioOutput out, String fileName) {
    if (isEnabled) {
      recorder = minim.createRecorder(out, fileName);
    }
  }

  void handleKeyPress(char k) {
    if (!isEnabled) return;
    
    if (k == 'r' || k == 'R') {
      if (!recorder.isRecording()) {
        recorder.beginRecord();
        println("【録音スタート】演奏をWAVに書き出します...");
      }
    }
    
    // Sキーで録音終了＆保存
    if (k == 's' || k == 'S') {
      if (recorder.isRecording()) {
        recorder.endRecord();
        recorder.save();
        println("【保存完了】trumpet_synth.wav を書き出しました！");
      }
    }
  }

  void displayStatus() {
    if (!isEnabled) return;
    
    fill(255);
    textSize(14);
    text("Press 'R' : Start Recording", 20, 150);
    text("Press 'S' : Save to WAV", 20, 170);
    
    // 録音中なら赤い文字で警告を出す
    if (recorder != null && recorder.isRecording()) {
      fill(255, 50, 50);
      text("● RECORDING NOW...", 20, 190);
    }
  }
}
