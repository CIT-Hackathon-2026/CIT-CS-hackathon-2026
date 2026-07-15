// ピタゴラ装置に必要な宣言
#include <Servo.h>;
#define DELAY_TIME 1200

//光装置に必要な宣言
#define THRESHOLD 935
#define bitduration 5000
#define start 160000
#define end_count 10




int wrote[400];//cut関数のリターン用配列

//光通信に必要な変数
const int pin = A0;
int n = 0;
int m = 0;
int sign[490];
int binary[70][7];
int end[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 1};

char situation= 'u';

//サーボモータの関数の宣言
Servo StandardServo2;
Servo StandardServo3;
Servo StandardServo4;
Servo StandardServo5;
Servo StandardServo6;
Servo StandardServo7; 
Servo StandardServo8;
Servo StandardServo9;

// ASCIIコード0〜127に対応した文字列配列
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
  pinMode(pin,INPUT);
  
  //シリアルモニタの宣言
  Serial.begin(9600);
  Serial.println("shoot start!");

// サーボモータとArduinoのピンの対応
  StandardServo2.attach(2);
  StandardServo3.attach(3);
  StandardServo4.attach(4);
  StandardServo5.attach(5);
  StandardServo6.attach(6);
  StandardServo7.attach(7);
  StandardServo8.attach(8);
  StandardServo9.attach(9);

// サーボモータのリセット
  reset_servo();
  delay(1000);
  for (int i = 0; i < 400; i++) {//不要な送信を行わないよう，配列の初期値を1000にして，ループで1000が検出されたらbreak;するようにするための下準備
    wrote[i] = 1000;
  }
  for(int i=0; i<490; i++){
    sign[i] = 1000;
  }

}

void sendSequence(const int* seq, int length) {
  reset_servo();
  delay(1000);

  for (int i = 0; i < length; i++) {
    Serial.print(asciiTable[seq[i]]);
    Serial.print(" ");
    shoot(seq[i]);
  }

  Serial.println("Transmission complete!");
}

void loop() {
  int sendData[] = {6,1,7,2,1,5,1,6,1,3,0,1,0,3,0,3};
  int len = sizeof(sendData) / sizeof(sendData[0]);

  sendSequence(sendData, len);

  while(true) {
    // 送信完了後は止める
  }
}



// void loop() {
//   int start_time =0;//処理時間計測のための変数
//   Serial.println("flash");
//     for(int i=0; i<490; i++){//不要な送信を行わないよう，配列の初期値を1000にして，ループで1000が検出されたらbreak;するようにするための下準備
//     sign[i] = 1000;
//   }
//   while(true){//plot()関数に一度入ると処理がwhile(true){}に入ってループしないため，situation変数によるループで処理が繰り返されるように変更
  
//     if(detectlight(start)){
//       Serial.println("開始信号を検知！");
//       start_time = millis();//処理開始時間の保存
//       situation='s';//繰り返しを抜けるためにsituation変数に's'を代入
//       flash();//光通信の主要関数？
//       if(situation == 'e'){//situation変数が'e'のときは繰り返しを抜ける
//         break;
//       }
//     }
//   }
//   Serial.println("plot");
//     for (int i = 0; i < 400; i++) {//不要な送信を行わないよう，配列の初期値を1000にして，ループで1000が検出されたらbreak;するようにするための下準備
//     wrote[i] = 1000;
//   }

//   reset_servo();//サーボモータをリセット

//   plot();//ピタゴラ装置の主要関数
//   Serial.print("処理時間=");
//   Serial.print(millis()- start_time);//処理時間の出力
//   Serial.println("(ms)");
// }

//number番のマイクロサーボをリセット(角度=0)にする
void reset_servo(){
  for(int i=0; i<8;i++){
  select_servo(i, 0);
  }
}

//番号と角度を入れたとき、対応するマイクロサーボがその角度ぶん動く
//関数たちをまとめる関数
void select_servo(int number,int position){

  if(number == 0){
    StandardServo2.write(position);
  }
  else if (number == 1){
    StandardServo3.write(position);
  }
  else if (number ==2){
    StandardServo4.write(position);
  }
  else if (number ==3){
    StandardServo5.write(position);
  }
  else if (number ==4){
    StandardServo6.write(position);
  }
  else if (number ==5){
    StandardServo7.write(position);
  }
  else if (number ==6){
    StandardServo8.write(position);
  }
  else if (number == 7){
    StandardServo9.write(position);
  }
  else {
    Serial.println("command select_servo() is error.");
  }
}

//ルート番号を入れたら通路を作る
void route(int route_number){

  if (route_number>=4){//4,5,6,7
    select_servo(1,60);
    if(route_number>5){//6,7
      select_servo(3,120);
      if(route_number==7){//7
        select_servo(7,60);
      }
    }else{//4,5
      if(route_number==5){//5
        select_servo(6,50);
      }
    }
  }else{//0,1,2,3
    if(route_number>1){//2,3
      select_servo(2,90);
      if(route_number==3){//3
        select_servo(5,45);
      }
    }else{//0,1
      if(route_number==0){//0
        select_servo(4,0);
      }
      else{
        select_servo(4,80);//1
      }
    }
  }
}

//玉を転がす
void shoot(int number){
  delay(1000);
  route(number);
  select_servo(0,90);
  delay(DELAY_TIME);
  reset_servo();
}

void plot(){//ピタゴラ装置の主要関数
//STXの送信
  shoot(0b010);
  combine();//sign[]配列からピタゴラで送信する配列を作成
  
  for(int i=0; i<1000;i++){//メッセージ部の送信
    if(wrote[i]>=1000){break;}
    Serial.print(asciiTable[wrote[i]]);
    shoot(wrote[i]);
  }

  //etxの送信
  Serial.println("");
  shoot(0b000);
  shoot(0b011);
  shoot(0b000);
  shoot(0b011);
  Serial.println("already shot!");
}

//開始信号を識別するための関数？
bool detectlight(int duration){
  unsigned long starttime = micros();
  while(true){
    if(analogRead(pin) > THRESHOLD){
      if(micros() - starttime >= duration){
        return true;
      }
    } else {
      starttime = micros();
    }
  }
}


//光通信の主要関数？
void flash() {
  n = 0;
  unsigned long pretime = micros();
  int count = 0;

  while(true){
    unsigned long currenttime = micros();
    int vol = analogRead(pin);

    if(currenttime - pretime >= bitduration){
      pretime = currenttime;
      
      if(vol >= THRESHOLD){//analogRead(pin)が閾値を超えた場合，sign[n]に１を，超えなかった場合，0を格納する？
        sign[n] = 1;
        count = 0;
      } else {
        sign[n] = 0;
        count++;
      }

      if(endcheck()){//終了信号を受け取ったときの処理？
        if(n >= 10){
          n -= 10;
        }
        groupbits();
        situation = 'e';
        break;
      }
      n++;

    }
  }
}

void groupbits(){//sign[]配列を7bitずつの２次元配列binary[]配列に再格納？
  m = 0;
  for(int i = 0; i + 7 <= n; i += 7){
    for(int j = 0; j < 7; j++){
      binary[m][j] = sign[i + j];
    }
    m++;
  }
  Serial.println("7ビットごとのビット列");
  for(int i = 0; i < m; i++){
    for (int j = 0; j < 7; j++){
      Serial.print(binary[i][j]);
    }
    Serial.println("");
  }
}

void combine(){//sign[]配列から７桁ずつ取り出してwrote[]配列の中に組み込んでいく関数．以前のcut()関数とtranslate()関数の仕事を一度に行う．これによってint型の定義域を外れない
  int bin=0;
  Serial.println("combine");
  for(int i=0;i<490;){
    i++;
    for(int j=0; j<7;j++){
      if(i>=490){break;}
      if(sign[i]==1000){break;}
      if(wrote[bin]==1000){wrote[bin]=0;}
      // int A = sign[bin-7]*0b1000000+sign[bin-6]*0b100000+sign[bin-5]*0b10000+sign[bin-4]*0b1000+sign[bin-3]*0b100+sign[bin-2]*0b100+sign[bin-1]*0b10+sign[bin];
      // if(A==0b11111100000000){break;}
        wrote[bin] *= 0b10;
        wrote[bin] +=sign[i];
        i++;
    }

    Serial.println(wrote[bin]);
    if(i>=490){break;}
    if(sign[i]==1000){break;}

    bin ++;
  }
}

bool endcheck(){//光信号の終了信号の識別?
  if(n < 10) return false;
  for(int i = 0; i < 10; i++){
    if(sign[n - 10 + i] == end[i]) return false;
  }
  return true;
}
