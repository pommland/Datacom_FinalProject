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
#define r_slope 50
#define peak    40
#define elapse_time 49000

void setup() {
  // put your setup code here, to run once:
  sbi(ADCSRA, ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  Serial.begin(115200);
  Serial.flush();
  Radio.init();
  Radio.set_frequency(102.5);
  pinMode(A0, INPUT);
  delay(500);
  //  calibrate();
  Serial.println("Ready....-");
}

bool check = false;
auto timer = millis();
long count = 0;
int  countBit = 0;
int  prev = 0;
String res = "", all = "";
int max = 0;
auto lastCount = millis();
int jitter = 0;

void checkBit() {
  if (check) {
    lastCount = millis();
    if (count >= 30) {
      res += "11";
    }
    else if (count >= 22) {
      res += "10";
    }
    else if (count >= 10) {
      res += "01" ;
    }
    else {
      res += "00";
    }

    if (check) {
      countBit ++;
      check = false;
      Serial.println(count);
      Serial.println(res);
      if (countBit == 4) {
        countBit = 0;
        Serial.println("------------------------\nResult =");
        int sum = 0;
        int t = 1;
        for (int i = 7 ; i >= 0 ; --i) {
          sum +=  (res[i] == '1' ? 1 : 0) * t;
          t <<= 1;
        }
        Serial.println((char)sum);
        Serial.println("------------------------");
        res = "";
      }
    }
  }
  return ;
}

void calibrate() {
  auto t = millis();
  Serial.println("Calibrating..");
  while (millis() - t <= 100) {
    jitter = analogRead(A0);
  }
  Serial.println("Done.");
}

void drop(int milli) {
  if (countBit != 0 && millis() - lastCount >= milli) {
    count = 0;
    max   = 0;
    check = false;
    countBit = 0;
    res = "";
    Serial.println("Droped");
    //    calibrate();
  }
}

void loop() {

  int tmp = analogRead(A0) - jitter;
  //  Serial.println(tmp);

  drop(100);

  if (check == false && tmp >= 25) {
    timer = micros();
    count = 0;
    prev  = 0;
    max   = 0;
    check = true;
  }
  while (check && (micros() - timer) <=  elapse_time ) {
    tmp = analogRead(A0) - jitter;
    if (max < tmp) max = tmp;
    if (tmp < 3 && max - tmp > r_slope) {
      //      Serial.println(String(max) + ":" + String(tmp));
      count ++;
      max = 0;
    }
  }
  checkBit();

}
