/* cbi this for increase analogRead Speed */
#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
/*amplitude diff. for detecting rising or falling signal*/
#define r_slope 220

void setup() {
  // put your setup code here, to run once:
  sbi(ADCSRA, ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  Serial.begin(115200);
  Serial.flush();
  pinMode(A0,INPUT);
}

bool check = false;
auto timer = millis();
long count = 0;
int  countBit = 0;
int  prev = 0;
String res = "";

void loop() {
  // put your main code here, to run repeatedly:
  int tmp = analogRead(A0) - 200;
  if (check == false && tmp > 100) {
     
    timer = millis();
    count = 0;
    prev  = 0;
    check = true;
  }
  while (millis() - timer <= 1010) {
    tmp = analogRead(A0);
    if (tmp >= 240 && tmp - prev > r_slope) {
      count ++;
    }
    prev = tmp;
    delayMicroseconds(100);
  }
  if (check) {
    if (count >= 320) {
      res += "11";
    }
    else if (count >= 250) {
      res += "10";
    }
    else if (count >= 10) {
      res += "01" ;
    }
    else {
      res += "00" ;
    }
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
      Serial.println(sum);
      Serial.println("------------------------");
      res = "";
    }
  }

}
