import ddf.minim.*;
import ddf.minim.ugens.*;
import processing.serial.*;

Minim minim;
AudioOutput out;
Waveform synthWavetable;
Serial serialPort;

SerialManager serialManager;
WaveformView  waveformView;

void setup() {
  size(512, 200);
  minim = new Minim(this);
  out = minim.getLineOut(Minim.MONO, BUFFER_SIZE, SAMPLE_RATE); 

  synthWavetable = WavetableGenerator.gen10(4096, SYNTH_HARMONICS);

  waveformView = new WaveformView(out);
  serialManager = new SerialManager();

  printArray(Serial.list());
  serialPort = new Serial(this, "/dev/cu.usbmodem34B7DA61FB042", SERIAL_BAUD);
  serialPort.bufferUntil('\n');
}

void draw() {
  background(0);
  waveformView.display();
}

void serialEvent(Serial port) {
  String inString = port.readStringUntil('\n');
  if (inString != null) {
    serialManager.processInput(trim(inString));
  }
}
