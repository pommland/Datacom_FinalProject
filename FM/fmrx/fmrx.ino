/* cbi this for increase analogRead Speed */
#include <TEA5767.h>
#include <Wire.h>
TEA5767 Radio;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
/*amplitude diff. for detecting rising or falling signal*/
unsigned long  r_slope = 50;
unsigned long  initial_signal = 25;
unsigned long  elapse_time = 48000;

void setup() {
  // put your setup code here, to run once:
  sbi(ADCSRA, ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  Serial.begin(115200);
  Serial.flush();
  Radio.init();
  Radio.set_frequency(90);
  pinMode(A1, INPUT);
  delay(500);

  Serial.println("Ready....-");
}

bool check = false;
auto timer = millis();
int  count = 0;
int  countBit = 0; 
int  prev = 0;
String res = "", all = "";
int max = 0;
auto lastCount = millis();


void checkBit() { // Check 8 bits
  if (check) {
    lastCount = millis();
    if (count >= 30) { // if count is more than 30 cycles
      res += "11";
    }
    else if (count >= 22) { // if count is more than 22 cycles
      res += "10";
    }
    else if (count >= 10) { // if count is more than 10 cycles
      res += "01" ;
    }
    else {
      res += "00";
    }

    if (check) {
      countBit ++;
      check = false;
      //      Serial.println(count);
      //      Serial.println(res);
      if (countBit == 4) {
        countBit = 0;
        Serial.println("------------------------\nResult =");
        int sum = 0;
        int t = 1;

        for (int i = 7 ; i >= 0 ; --i) {  // Binary to Decimal , index 7 is LSB  index 0 is MSB  so we recevied MSB first
          sum +=  (res[i] == '1' ? 1 : 0) * t;
          t <<= 1;
        }
        //        Serial.println((char)sum);
        all += (char)sum;
        Serial.println(all);
        Serial.println("------------------------");
        res = "";
      }
    }
  }
  return ;
}




void drop(int milli) { // if no data send for n millisec reset data
  if (countBit != 0 && millis() - lastCount >= milli) {
    count = 0;
    max   = 0;
    check = false;
    countBit = 0;
    res = "";
    all = "";
    Serial.println("Droped");
  } else   if (countBit == 0 && millis() - lastCount >= milli) {
    all = "";
  }
}


void split(String s[],int num, char value[], char sep[] = " " ) {
  char *token = strtok(value, sep);
  //  char *buf[10];
  int co =  0;
  while (token != NULL)
  {
    if (co >= num ) break; 
    s[co] = token;
    Serial.println(token);
    token = strtok(NULL, sep);
    co++;
  }
  //  Settings(buf[0], buf[1]);
}

 

void Settings(String mode , String value ) {

  unsigned long  val  =  value.toInt();
  int m  =  mode.toInt();

  switch (m) {
    case 0 :
      Serial.print("change r_slope to ");
      r_slope = val;
      Serial.println(r_slope);
      break;
    case 1 :
      Serial.print("change initial Signal to ");
      initial_signal = val;
      Serial.println(initial_signal);
      break;
    case 2 :
      Serial.print("change elapse Time to ");
      elapse_time = val;
      Serial.println(elapse_time);
      break;
    default:
      Serial.print("Invalid Input");
      break;
  }
}

void configs() {
  String res = "";
  if (Serial.available() > 0) {
    auto res = Serial.readStringUntil('~');
    char s[res.length()];
    for (int i = 0 ; i < res.length()  ; ++i) {
      s[i] = res[i];
    }

    if (strlen(s)) {
      String inp[3];
      split(inp, 3, s);
      Settings(inp[0], inp[1]);
      
    }
  }
}

bool initial() {
  int tmp = analogRead(A1) ;
  Serial.println(tmp);
  if (check == false && tmp >= initial_signal) { // signal at 25
    timer = micros();
    count = 0;
    prev  = 0;
    max   = 0;
    check = true;
  }
  return check;
}

void receiveData() {
  while (check && (micros() - timer) <=  elapse_time ) { // if signal receive begin loop for counting data
    int tmp = analogRead(A1)  ;
    if (max < tmp) max = tmp;
    if (tmp < 20 && max - tmp >  r_slope) {  // if peak down to base  it is 1 cycles
      //      Serial.println(String(max) + ":" + String(tmp));
      count ++;
      max = 0;
    }
  }
}

void FM() {
  configs(); // set var
  drop(100); // drops in 100ms
  if (initial()) {
    receiveData();
  }
//  checkBit();
}

void loop() {
  FM();
  // FLOW CONTROL

}
