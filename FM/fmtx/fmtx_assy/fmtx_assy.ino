#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>
Adafruit_MCP4725 dac;
#define defaultFreq 6039 // dac speed (Hz)
#define f0 500  // FSK f0
#define f1 750  // FSK f1
#define f2 1000 // FSK f2
#define f3 1500 // FSK f3
float delay0, delay1, delay2, delay3;
//int   Cycles[4] = { 1 , 3 , 5 , 7};
//int   Cycles[4] = { 2 , 3 , 4 , 5};
int Cycles[2] = { 1, 3 };

const int size = 4 ;

uint16_t S_DAC[size];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dac.begin(0x62);

  delay0 = 5325; // microSecs at 1200Hz
  //  delay1 = 667;
  //  delay2 = 337;
  delay3 = 1667 ;//

  Serial.println( String(delay0) + " " + String(delay1) + " " + String(delay2) + " " + String(delay3));
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
      int input[8] = { 0, 0, 0, 0 };
      int i = 0;
      //      while (in > 0) {   //  แปลงเป็น 2 บิต
      //        input[i] = in & 3 ;
      //        in >>= 2;
      //        i++;
      //      }

      while (in > 0) {   //  แปลงเป็น 2 บิต
        input[i] = in & 1 ;
        in >>= 1;
        i++;
      }

      //      for (int k = 3 ; k >= 0 ; k--) {
      //        float d;
      //        if (input[k] == 0) {      // ‘00’
      //          d = delay0;
      //        }
      //        else if (input[k] == 1) { // ‘01’
      //          d = delay1;
      //        }
      //        else if (input[k] == 2) { // ‘10’
      //          d = delay2;
      //        }
      //        else if (input[k] == 3) { // ‘11’
      //          d = delay3;
      //        }
      //        Serial.println(input[k]);
      //        for (int r = 0 ; r  <  5 ; ++r) // send with fixed baudrate
      //          for (int cl = 0; cl < Cycles[input[k]]; ++cl) {  // send with fixed cycles
      //            for (int s = 0 ; s < size ; ++s) { // send with fixed sampling
      //              dac.setVoltage(S_DAC[s], false);
      //              delayMicroseconds(d);
      //            }
      //          }
      //      }

      for (int k = 7 ; k >= 0 ; k--) {
        float d;
        if (input[k] == 0) {      // ‘0’
          d = delay0;
        }
        else if (input[k] == 1) { // ‘1’
          d = delay3;
        }
        Serial.print(input[k]);
        //        for (int r = 0 ; r  <  5 ; ++r) // send with fixed baudrate
        for (int cl = 0; cl < Cycles[input[k]] * 2; ++cl) {  // send with fixed cycles
          for (int s = 0 ; s < size ; ++s) { // send with fixed sampling
            dac.setVoltage(S_DAC[s], false);
            delayMicroseconds(d);
          }
        }
      }
      Serial.println();
    }
  }

}

String getSerial() { // get Data methods
  String res = "";
  if (Serial.available() > 0) {
    while (1) { // read untill '~'  example to enter at the terminal   "xxx~"
      char c = Serial.read();
      if (c != 0xFFFFFFFF) {
        res += c;
      }
      if (c == '~') break;
    }
  }
  return res;
}

void loop() {
  String s  = getSerial();
  if (s != "") {
    s = "~" + s;  // data will be send  example  enter "xxx~" data will be "~xxx~"
    Serial.println(s);
    Send_FM_Data(s);
    Serial.flush();
  } else {
    dac.setVoltage(0, false); // default
  }
}
