// TX Variables ///////////////////
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>
Adafruit_MCP4725 dac;
float delay0, delay1, delay2, delay3;
int   Cycles[4] = { 1 , 3 , 5 , 7};
const int size = 4 ;
uint16_t S_DAC[size];
///////////////////////////////////

// RX Variables ///////////////////
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

bool check = false;
auto timer = millis();
int  count = 0;
int  countBit = 0;
int  prev = 0;
String res = "", all = "";
int max = 0;
auto lastCount = millis();
///////////////////////////////////


void setup() {
  Serial.begin(115200);
  // TX ///////////////////
  initTx();
  /////////////////////////
  // RX ///////////////////
  initRx();
  /////////////////////////
}


void loop() {
  handleSerial();
  Rx();
  dac.setVoltage(0, false); // default
}

void split(String s[], int num, char value[], char sep[] = " " ) {
  char *token = strtok(value, sep);
  int co =  0;
  while (token != NULL)
  {
    if (co >= num ) break;
    s[co] = token;
    Serial.println(token);
    token = strtok(NULL, sep);
    co++;
  }
}

void handleSerial() {
  if (Serial.available() > 0) {
    String res = Serial.readStringUntil('~');

    // String to char[]
    char s[res.length()];
    for (int i = 0 ; i < res.length()  ; ++i) {
      s[i] = res[i];
    }

    // split
    String inp[3];
    split(inp, 3, s);

    if (inp[0] == "Send") {
      // Send data
      Tx(inp[1]);
    } else if (inp[0] == "Config") {
      // Config mode data
      Settings(inp[1], inp[2]);
    }
  }
}

/////////////////// TX ////////////////////////////////////////////////////////////////////////////
void Tx(String s) {
  if (s != "") {
    s = "~" + s + "~";  // data will be send  example  enter "xxx~" data will be "~xxx~"
    Serial.println(s);
    Send_FM_Data(s);
    Serial.flush();
  } else {
    dac.setVoltage(0, false); // default
  }
}


void initTx() {
  dac.begin(0x62);
  delay0 = 2300;
  delay1 = 667;
  delay2 = 337;
  delay3 = 196 ;
  getS_DAC();
  Serial.flush();
}

void getS_DAC() {
  int div = 360 / size;
  for ( int i = 0 ; i < size ; i++) {
    S_DAC[i] = map( sin(i * div * 3.1415926 / 180) * 100 , -100, 100 , 0 , 1023);
  }
}

void Send_FM_Data(String value) {
  for (int valueIndex = 0 ; valueIndex < value.length() ; ++valueIndex) {
    if (value[valueIndex] != '\n' || value[valueIndex] != '~') {
      char in  = value[valueIndex];
      int input[4] = { 0, 0, 0, 0 };
      int i = 0;
      while (in > 0) {   //  แปลงเป็น 2 บิต
        input[i] = in & 3 ;
        in >>= 2;
        i++;
      }

      for (int k = 3 ; k >= 0 ; k--) {
        float d;
        if (input[k] == 0) {      // ‘00’
          d = delay0;
        }
        else if (input[k] == 1) { // ‘01’
          d = delay1;
        }
        else if (input[k] == 2) { // ‘10’
          d = delay2;
        }
        else if (input[k] == 3) { // ‘11’
          d = delay3;
        }
        Serial.println(input[k]);
        for (int r = 0 ; r  <  5   ; ++r) // send with fixed baudrate
          for (int cl = 0; cl < Cycles[input[k]]; ++cl) {  // send with fixed cycles
            for (int s = 0 ; s < size ; ++s) { // send with fixed sampling
              dac.setVoltage(S_DAC[s], false);
              delayMicroseconds(d);
            }
          }
      }
    }
  }
}


////////////////////// Rx ////////////////////////////////////////////////////////////////////////////////////////////////

void initRx() {
  sbi(ADCSRA, ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  Radio.init();
  Radio.set_frequency(87);
  pinMode(A1, INPUT);
  delay(500);
}

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



bool initial() {
  int tmp = analogRead(A1) ;
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

void Rx() {
  drop(100); // drops in 100ms
  if (initial()) {
    receiveData();
  }
  checkBit();
}
