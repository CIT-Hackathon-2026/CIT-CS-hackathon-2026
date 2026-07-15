int written[400];
int a;

int wrote[400];
char situation;
const char* asciiTable[128] = {
  "NUL",  // 0: Null
  "SOH",  // 1: Start of Header
  "STX",  // 2: Start of Text
  "ETX",  // 3: End of Text
  "EOT",  // 4: End of Transmission
  "ENQ",  // 5: Enquiry
  "ACK",  // 6: Acknowledge
  "BEL",  // 7: Bell
  "BS",   // 8: Backspace
  "TAB",  // 9: Horizontal Tab
  "LF",   // 10: Line Feed
  "VT",   // 11: Vertical Tab
  "FF",   // 12: Form Feed
  "CR",   // 13: Carriage Return
  "SO",   // 14: Shift Out
  "SI",   // 15: Shift In
  "DLE",  // 16: Data Link Escape
  "DC1",  // 17: Device Control 1
  "DC2",  // 18: Device Control 2
  "DC3",  // 19: Device Control 3
  "DC4",  // 20: Device Control 4
  "NAK",  // 21: Negative Acknowledge
  "SYN",  // 22: Synchronous Idle
  "ETB",  // 23: End of Transmission Block
  "CAN",  // 24: Cancel
  "EM",   // 25: End of Medium
  "SUB",  // 26: Substitute
  "ESC",  // 27: Escape
  "FS",   // 28: File Separator
  "GS",   // 29: Group Separator
  "RS",   // 30: Record Separator
  "US",   // 31: Unit Separator
  " ",    // 32: Space
  "!", "\"", "#", "$", "%", "&", "'", "(", ")", "*", "+", ",", "-", ".", "/",  // 33–47
  "0", "1", "2", "3", "4", "5", "6", "7", "8", "9",                            // 48–57
  ":", ";", "<", "=", ">", "?", "@",                                         // 58–64
  "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O",  // 65–79
  "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z",                      // 80–90
  "[", "\\", "]", "^", "_", "`",                                             // 91–96
  "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o",  // 97–111
  "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z",                      // 112–122
  "{", "|", "}", "~", "DEL"                                                 // 123–127
};


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for(int i; i<8;i++){//ピン0~7を入力モードで起動
    pinMode(i,INPUT);
  }
}

void loop() {
  plot();
}

//開始の判定
void stx(int num){
  if (num == 0b010){
    situation = 's';
  }
}

//文字の判定と終了の判定
void txt(int num){
  unsigned long long answer=0;
  written[a] = num;
  if(a>=3){
    int A = (written[a-3]<<9) + (written[a-2]<<6) + (written[a-1]<<3) + written[a];

  
    if(A==0b000011000011){//終了信号を受け取ったら結果の出力に移行
      situation = 'e';
      for(int i=0; i<a-3;i++){//2進数の結合
        answer*=0b1000;
        answer += written[i];
      }
      cut(answer);//wrote[]に7bitずつ区切っていれる
      for(int i; i<100; i++){//受信した内容を出力
        Serial.print(asciiTable[wrote[i]]);
      }
      Serial.println("");
    }
  }
  a++;
}

//2進数の羅列を受け取る-->7桁づつ区切ってwrote[]にいれる
void cut(unsigned long long sign){
  int i = 0;
  while(sign>0){
    wrote[i] = sign % 0b10000000;

    sign/=0b10000000;
    i++;
  }
}

//値を常に受け取って，閾値以下になったら値を取得,どこが閾値以下になったかを返す
int sense(){
  int where;
  while(true){
    for(int i=0; i<8; i++){
      int score = digitalRead(i);
      if(score==0){
        where = i;
        delay(1000);
        return where;
      }
    }
  }
}

//本筋
void plot(){
  a=0;
  stx(sense());
  // situation='s';
  while(situation=='s'){
    txt(sense());
  }
}