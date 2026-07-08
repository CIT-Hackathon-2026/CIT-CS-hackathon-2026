import ddf.minim.*;
import ddf.minim.ugens.*;
import processing.serial.*;

Minim minim;
AudioOutput out;
Waveform fluteWavetable; 
Serial serialPort;

SerialManager serialManager;
WaveformView waveformView;

void setup() {
  size(512, 200); 
  minim = new Minim(this);
  out = minim.getLineOut(Minim.MONO, 512);

  // 1〜6次倍音の手動合成
  int tableSize = 1024;
  float[] waveBuffer = new float[tableSize];
  for (int i = 0; i < tableSize; i++) {
    float totalAmp = 0;
    float fraction = (float)i / tableSize; 
    for (int h = 0; h < 6; h++) { 
      int harmonicOrder = h + 1; 
      float amplitude = FLUTE_HARMONICS[h]; 
      totalAmp += amplitude * sin(fraction * TWO_PI * harmonicOrder);
    }
    waveBuffer[i] = totalAmp;
  }
  fluteWavetable = new Wavetable(waveBuffer);

  waveformView = new WaveformView(out);
  serialManager = new SerialManager();

  printArray(Serial.list());
  
  // シリアルポートを開く
  serialPort = new Serial(this, "/dev/cu.usbmodem34B7DA643ABC2", 115200);
  serialPort.bufferUntil('\n');

  // 【追加】Arduinoのバッファに溜まったゴミデータを一度クリアする
  serialPort.clear();
  
  // 【追加】Arduinoに「演奏開始（Start）」の合図を送る
  // これにより、曲の最初からきれいに1回だけ再生されます
  serialPort.write('S');
}

void draw() {
  background(0);
  waveformView.display(); 
}

void serialEvent(Serial port) {
  String s = port.readStringUntil('\n');
  if (s != null) {
    serialManager.processInput(trim(s));
  }
}

void stop() {
  out.close();
  minim.stop();
  super.stop();
}
