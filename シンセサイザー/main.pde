import ddf.minim.*;
import ddf.minim.ugens.*;
import processing.serial.*;

Minim minim;
AudioOutput out;
Waveform synthWavetable;
Serial serialPort;

// 新しく作った管理・描画クラスのインスタンス
SerialManager serialManager;
WaveformView  waveformView;

void setup() {
  size(512, 200);
  minim = new Minim(this);
  out = minim.getLineOut(Minim.MONO, BUFFER_SIZE);
  
  synthWavetable = WavetableGenerator.gen10(4096, SYNTH_HARMONICS);
  
  // 描画ビューと通信マネージャーの準備
  waveformView = new WaveformView(out);
  serialManager = new SerialManager();

  printArray(Serial.list());
  serialPort = new Serial(this, "/dev/cu.usbmodem34B7DA61FB042", SERIAL_BAUD);
  serialPort.bufferUntil('\n');
}

void draw() {
  background(0);
  // 波形の描画をViewクラスに任せる
  waveformView.display();
}

void serialEvent(Serial port) {
  String inString = port.readStringUntil('\n');
  if (inString != null) {
    // データの処理をManagerクラスに任せる
    serialManager.processInput(trim(inString));
  }
}
