import ddf.minim.*;
import ddf.minim.ugens.*;
import ddf.minim.effects.*;  
import processing.serial.*;


Minim minim;
AudioOutput out;
Waveform trumpetWavetable; 
Serial serialPort;

SerialManager serialManager;
WaveformView  waveformView;
WavRecordManager recordManager;

void setup() {
  size(512, 200);
  minim = new Minim(this);
  out = minim.getLineOut(Minim.MONO, BUFFER_SIZE);

  trumpetWavetable = WavetableGenerator.gen10(4096, TRUMPET_HARMONICS);
  
  waveformView = new WaveformView(out);
  serialManager = new SerialManager();
  recordManager = new WavRecordManager(minim, out, "trumpet_make_A4.wav");

  printArray(Serial.list());
  serialPort = new Serial(this, "/dev/cu.usbmodem34B7DA643ABC2", SERIAL_BAUD);
  serialPort.bufferUntil('\n');
}

void draw() {
  background(0);
  waveformView.display();
  recordManager.displayStatus();
}

void serialEvent(Serial port) {
  String inString = port.readStringUntil('\n');
  if (inString != null) {
    serialManager.processInput(trim(inString));
  }
}

void keyPressed() {
  recordManager.handleKeyPress(key);
}
