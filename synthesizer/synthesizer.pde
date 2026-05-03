import ddf.minim.*;
import ddf.minim.ugens.*;

Minim minim;
AudioOutput out;
Waveform currentWaveform;

String[] melody = {
  "A4", "A#4", "B4", "C5", "C#5", "D5", "D#5", "E5", "F5", "F#5", "G5", "G#5", "A5"
};

float[] duration = {
  2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f, 2.0f
};

float[] startTime = {
  0.0f, 2.5f, 5.0f, 7.5f, 10.0f, 12.5f, 15.0f, 17.5f, 20.0f, 22.5f, 25.0f, 27.5f, 30.0f
};

// 🎹 「トトトト」の間隔を少し長く（遅く）調整したクラス 🎹
class HackInstrument implements Instrument {
  Summer sum;
  Oscil wave1;      
  Oscil wave2;      
  Oscil wave3;      
  Oscil tremolo;    
  MoogFilter filter;
  ADSR adsr;

  HackInstrument(float frequency, float maxAmp, Waveform wf) {
    sum = new Summer();
    
    // 1. メインの音（まっすぐ太く）
    wave1 = new Oscil(frequency, 0.35f, wf);
    wave2 = new Oscil(frequency * 1.008f, 0.35f, wf); 
    
    // 2. 後ろで鳴る「ト・ト・ト・ト」の音
    wave3 = new Oscil(frequency, 0.0f, wf); 
    
    // ★ ここを修正しました！ ★
    // 12.0f（1秒間に12回）から 8.0f（1秒間に8回）に変更し、間隔を少し長くしました。
    // もしこれでも「まだ速い」場合は 6.0f に、「少し遅すぎる」場合は 10.0f など、
    // お好みに合わせてここの数字を微調整してみてください！
    tremolo = new Oscil(8.0f, 0.15f, Waves.SQUARE);
    tremolo.offset.setLastValue(0.15f); 
    
    tremolo.patch(wave3.amplitude);
    
    // 3. すべての波を合体させる
    wave1.patch(sum);
    wave2.patch(sum);
    wave3.patch(sum);
    
    filter = new MoogFilter(frequency * 3.0f, 0.5f);
    adsr = new ADSR(maxAmp, 0.01f, 0.1f, 0.8f, 0.3f);
    
    sum.patch(filter).patch(adsr);
  }

  void noteOn(float duration) {
    adsr.patch(out);
    adsr.noteOn();
  }

  void noteOff() {
    adsr.noteOff();
    adsr.unpatchAfterRelease(out);
  }
}

void setup() {
  size(512, 200);
  minim = new Minim(this);
  out = minim.getLineOut();
  out.setTempo(120);

  currentWaveform = Waves.SQUARE; 
}

void playSong() {
  out.pauseNotes(); 
  for (int i = 0; i < melody.length; i++) {
    out.playNote(startTime[i], duration[i],
      new HackInstrument(Frequency.ofPitch(melody[i]).asHz(), 0.5f, currentWaveform));
  }
  out.resumeNotes();
}

void draw() {
  background(0);
  stroke(255);
  for(int i = 0; i < out.bufferSize() - 1; i++) {
    line(i, 50 + out.left.get(i)*50, i+1, 50 + out.left.get(i+1)*50);
    line(i, 150 + out.right.get(i)*50, i+1, 150 + out.right.get(i+1)*50);
  }
}

void keyPressed() {
  switch (key) {
    case '1': currentWaveform = Waves.SINE; break;
    case '2': currentWaveform = Waves.TRIANGLE; break;
    case '3': currentWaveform = Waves.SAW; break;
    case '4': currentWaveform = Waves.SQUARE; break;
    case '5': currentWaveform = Waves.randomNOddHarms(16); break;
    case '6':
      currentWaveform = WavetableGenerator.gen10(
        4096, 
        new float[] { 1.0f, 0.0f, 0.333f, 0.0f, 0.2f, 0.0f, 0.143f, 0.0f, 0.111f, 0.0f } 
      );
      break;
    case 'p': 
      playSong(); 
      break;
    default: break;
  }
}
