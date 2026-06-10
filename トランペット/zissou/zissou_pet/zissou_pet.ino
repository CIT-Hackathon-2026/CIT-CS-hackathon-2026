const int SERIAL_BAUD_RATE = 9600;
const int BUTTON_PIN = 2;  // スイッチを接続するデジタルピン番号
const int NUM_NOTES = 13;

const String MELODY[] = {"C5", "D5", "E5", "F5", "E5", "D5", "C5", "E5", "F5", "G5", "A5", "G5", "F5", "E5", "C5", "C5", "C5", "C5",
  "C5", "C5", "D5", "D5", "E5", "E5", "F5", "F5", "E5", "D5", "C5"};
const float DURATIONS[] = {1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0f, 1.0, 1.0f, 1.0f, 1.0f, 1.0,
  0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 0.5f, 1.0f, 1.0f, 1.0f};
const float AMPLITUDES[] = {
  0.8, 0.4, 0.8, 0.5, 0.8, 0.9, 1.0, 0.8, 0.4, 0.8, 0.5, 0.8, 0.9
};

int noteIndex = 0; 
int lastButtonState = HIGH;

void setup() {
  Serial.begin(SERIAL_BAUD_RATE);
  
  pinMode(BUTTON_PIN, INPUT_PULLUP);
}

void loop() {
  int buttonState = digitalRead(BUTTON_PIN);

  if (lastButtonState == HIGH && buttonState == LOW) {
    
    Serial.print(DURATIONS[noteIndex]);
    Serial.print(",");
    Serial.print(AMPLITUDES[noteIndex]);
    Serial.print(",");
    Serial.println(MELODY[noteIndex]);
    
    noteIndex++;
    if (noteIndex >= NUM_NOTES) {
      noteIndex = 0; 
    }
    
    delay(50); 
  }
  
  lastButtonState = buttonState;
}