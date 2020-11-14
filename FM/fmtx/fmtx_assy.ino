#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>
Adafruit_MCP4725 dac;
#define defaultFreq 1700 // dac speed (Hz)
#define f0 500 // FSK f0
#define f1 750 // FSK f1
#define f2 1000 // FSK f2
#define f3 1250 // ASK f3
int delay0, delay1, delay2, delay3;

const int size = 36 ;

uint16_t S_DAC[size];

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  dac.begin(0x62);
  delay0 = (1000000 / f0 - 1000000 / defaultFreq) / size ;
  delay1 = (1000000 / f1 - 1000000 / defaultFreq) / size ;
  delay2 = (1000000 / f2 - 1000000 / defaultFreq) / size ;
  delay3 = (1000000 / f3 - 1000000 / defaultFreq) / size ;
  getS_DAC();
  Serial.flush();
}

void getS_DAC() {
  int div = 360 / size;
  for ( int i = 0 ; i < size ; i++) {
    S_DAC[i] = map( sin(i * div * 3.1415926 / 180) * 100 , -100, 100 , 0 , 1023);
    Serial.println(S_DAC[i]);
  }
}

void loop() {
  // put your main code here, to run repeatedly:
  if (Serial.available() > 0) { // for get input
    int in = Serial.parseInt(); // get Dec from Serial
    /*
      create an array store every set of 2 bits
      from each input byte
    */

    int input[4] = {-1,-1,-1,-1};
    int i = 0;
    while (in > 0) {
      input[i] = in & 3 ;
      in >>= 2;
      i++;
    }

    /*for each set of 2 bits in a data byte*/
    for (int k = 3 ; k >= 0 ; k--) {
      int d;
      if (input[k] > 3 || input[k] < 0) input[k] = 0;
      if (input[k] == 0) { // for input 00 -> 500 Hz
        //display bit value of input [k] (out ‘00’)
        //send S_DAC[4] N cycles (calculating for 500Hz)
        d = delay0;
      }
      else if (input[k] == 1) { // for input 01 - 750 Hz
        //display bit value of input [k] (out ‘01’)
        //send S_DAC[4] N cycles (calculating for 500Hz)
        d = delay1;
      }
      else if (input[k] == 2) { // for input 10 - 1000 Hz
        //display bit value of input [k] (out ‘10’)
        //send S_DAC[4] N cycles (calculating for 1000Hz)
        d = delay2;
      }
      else if (input[k] == 3) { // for input 11 - 1250 Hz
        //display bit value of input [k] (out ‘11’)
        //send S_DAC[4] N cycles (calculating for 1250Hz)
        d = delay3;
      }
      Serial.println(input[k]);
      auto start = millis();
      while (millis() - start <= 1000) {
        for (int s = 0 ; s < size ; ++s) {
          dac.setVoltage(S_DAC[s], false);
          delayMicroseconds(d);
        }
      }
    }
    dac.setVoltage(0, false); //
  }
}
