import ddf.minim.*;
import ddf.minim.ugens.*;
import processing.serial.*;

Minim minim;
AudioOutput out;
Serial serialPort;

SerialManager serialManager;
WaveformView waveformView;

void setup() {
  size(600, 300);
  minim = new Minim(this);
  out = minim.getLineOut();

  waveformView = new WaveformView(out);
  serialManager = new SerialManager();

  printArray(Serial.list());
  
  // ⚠️ お使いのMac/PCとArduinoの接続ポート名に合わせて変更してください
  serialPort = new Serial(this, "/dev/cu.usbmodem4827E2E078242", 115200);
  serialPort.bufferUntil('\n');
}

void draw() {
  background(30);
  waveformView.display(); 
}

void serialEvent(Serial port) {
  String s = port.readStringUntil('\n');
  if (s != null) serialManager.processInput(trim(s));
}

void stop() {
  out.close();
  minim.stop();
  super.stop();
}
