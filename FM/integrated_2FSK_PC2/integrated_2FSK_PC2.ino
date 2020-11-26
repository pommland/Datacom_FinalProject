// TX Variables ///////////////////
#include <Wire.h>
#include <Adafruit_MCP4725.h>
#include <Adafruit_ADS1015.h>
#define radio_freq 87.5
#define dac_Address 0x61
Adafruit_MCP4725 dac;
float delay0,delay3; // delay0 for bit '0' , delay3 for bit '1'
int   Cycles[2] = { 1 , 3 }; // cycle/baud 
const int size = 4 ; // sampling size
uint16_t S_DAC[size]; // pwmDuty
///////////////////////////////////

// RX Variables ///////////////////
#include <TEA5767.h>  // FM rx library
TEA5767 Radio;

#ifndef cbi
#define cbi(sfr, bit) (_SFR_BYTE(sfr) &= ~_BV(bit))
#endif
#ifndef sbi
#define sbi(sfr, bit) (_SFR_BYTE(sfr) |= _BV(bit))
#endif
/*amplitude diff. for detecting rising or falling signal*/
//unsigned long  r_slope = 150;
//unsigned long  initial_signal = 50;
//unsigned long  elapse_time = 44000;
//unsigned long  mode = 0;
unsigned long  r_slope = 30;                 // Vpeak
unsigned long  initial_signal = 10;          // Vstart
unsigned long  elapse_time = 43500;          // 1 baud width
unsigned long  mode = 0;                     // calculation mode


bool check = false;        // isOnCounting
auto timerFM = millis();   // timer for FM Counting
int  count = 0;            // Cycles Count
int  countBit = 0;         // Bit count
String res = "", all = ""; // res 8bit char , all store data in 1 frame 
int max = 0;               // store peak amplitude
auto lastCount = millis(); // lastest time as occur
///////////////////////////////////

//// Flow Control /////
bool canSend = true;       // can send data
long timer = 0;            // time out
int dataSize = 5;

int frameNo = 0;
int ackNo = 0;

char flag = ':';
String bufferToSend = "";
String dataFrameSend = "";

String allData = "";
//////////////////////


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
  timeOut();
  handleSerial();
  readySend();
  receive();
  isSendEnd();
  //  dac.setVoltage(0, false); // default
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

//////////////////////// Flow Control /////////////////////////////////////

void timeOut() {  // check is Timeout and then Resend data
  if (!canSend && millis() - timer >= 10000) {
    Serial.println("Status : TimeOut");
    Serial.println("Resend!");
    //Send dataFrame
    sendFrame(dataFrameSend);
  }
}

void readySend() { // make frame and send data if can send
  if (canSend && bufferToSend.length() != 0) {
    bool isAckFrame = false;
    //Slicing Data from Buffer
    String data = "";
    if (bufferToSend.length() < dataSize) {  // if data is less than 5 bytes
      int num = bufferToSend.length();
      for (int i = 0; i < num; i++) { 
        data += bufferToSend[0];  
        bufferToSend.remove(0, 1); 
      }
    }
    else {
      for (int i = 0; i < dataSize; i++) { // if data is more than 5 bytes
        data += bufferToSend[0];
        bufferToSend.remove(0, 1);
      }
    }
    // tmp package  =  [I][frameNo][data]
    String forSum = "I" + String(frameNo) + data  ; // for Checksum
    // real package =  [I][frameNo][data][;][sum][#]
    dataFrameSend = "I" + String(frameNo) + data + ";" + getSum(forSum) + "#" ;
    //Send dataFrame
    sendFrame(dataFrameSend);
    canSend = false;
  }
}

void sendFrame(String frame) {
  Serial.println("Send frame : " + frame);
  Serial.println("Frame No : " + String(frameNo));
  Serial.println("============================");
  Tx(frame); // let Tx send frame
  canSend = false;
  timer = millis();
}

void sendAck(String frame) {
  Serial.print("Send Ack : ");
  Serial.println(ackNo);
  Serial.println("============================");
  Tx(frame);
}
void isSendEnd() {
  if (allData[ allData.length() - 1] == ':') {  // if data end with ':' 
    Serial.println("Send End With Data : " + allData);
    ackNo = 0;
    allData = "";
  }
}

void receive() {
  String res = Rx();
  if (res != "" && res[res.length() - 1] == '#') { // if data is detect and not the end flag
    String frame = res;

    // I-Frame
    if (frame[0] != 'A') { // is I-Frame
      String data = "", sum = "";

      int i = 2;
      for (i = 2 ; i < 7 && frame[i] != ';' ; ++i) {
        data += String(frame[i]);
      }

      //      for (int j = 1 ; j <= 2  && frame[i + j] != '#'; j++) {
      //        sum += String(frame[i + j]);
      //      }
      sum = String(frame[frame.length() - 2]);

      Serial.println("Received I-Frame");
      Serial.println("frame No. : " + String(frame[1]) );
      Serial.println("Data      : " + (data) );
      Serial.println("Sum       : " + (sum) );

      String forSum = "I" + String(frame[1]) + data ;

      if (getSum(forSum) == sum) {
        Serial.println("Frame is Correct");
        //        allData += data;
        Serial.println("[Sending Ack]");
        auto fn = String(frame[1]).toInt();

        if (ackNo == fn) { // if ack loss
          ackNo = !ackNo;
          allData += data;
        } else  Serial.println("Frame Already Receive Reject!");
        String ack = "A" + String(ackNo);
        ack = "A" + String(ackNo) + getSum(ack) + "#";
        sendAck(ack);
        // send ack
      } else {
        Serial.println(getSum(forSum) + ":" + sum);
        Serial.println("Frame is Corrupted \nDiscarded");
        // discarding
      }

    } else {
      Serial.println("Received ACK-Frame");
      String forSum = "A" + String(frame[1]);
      String sum = String(frame[2]);
      if (getSum(forSum) == sum) {
        Serial.println("Frame is Correct");
        long aN = String(frame[1]).toInt();
        if (aN == !frameNo) { // send Next Frame
          canSend = true;
          frameNo = !frameNo;
        } else { // resend
          if (bufferToSend.length() != 0)
            sendFrame(dataFrameSend);
          else {
            Serial.println("Out of Data");
            frameNo = 0;
          }
        }
      } else {
        Serial.println("Frame is Corrupted \nDiscarded");
      }

    }
    Serial.println("Alldata : " + allData);
  }
}


String getSum(String string) {
  int sum = 0;
  for (int i = 0; i < string.length(); i++) {
    sum += string[i];
  }
  //  uint8_t a = sum & 0xFF , b = (sum >> 8) & 0xFF ;

  return String(sum % 10);
}
/////////////////////////////////////////////////////////////////////



void handleSerial() {
  if (Serial.available() > 0) {
    String res = Serial.readStringUntil('~'); // inpuit format  "Command data1 data2 endtag  ex. Send 1234 ~

    // String to char[]
    char s[res.length()];
    for (int i = 0 ; i < res.length()  ; ++i) {
      s[i] = res[i];
    }

    // split
    String inp[4];
    split(inp, 4, s);

    if (inp[0] == "Send") {
      bufferToSend = "";
      for (int i = 0 ; i < inp[1].length(); ++i) {
        bufferToSend += String(inp[1][i]);
      }
    } else if (inp[0] == "Config") {
      // Config mode data
      Settings(inp[1], inp[2]);
    }
  }
}

/////////////////// TX ////////////////////////////////////////////////////////////////////////////
void Tx(String s) {
  if (s != "") {
    // data will be send  example  enter "xxx~" data will be "~xxx~"
    Serial.println(s);
    Send_FM_Data(s);
    Serial.flush();
  } else {
    //    dac.setVoltage(0, false); // default
  }
}


void initTx() {
  dac.begin(dac_Address); // 0x62
  delay0 = 5325;
  delay3 = 1667;
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
      int input[8] = { 0, 0, 0, 0, 0, 0, 0, 0 };
      int i = 0;

      while (in > 0) {   //  แปลงเป็น 2 บิต
        input[i] = in & 1 ;
        in >>= 1;
        i++;
      }
      for (int k = 7 ; k >= 0 ; k--) {
        float d;
        if (input[k] == 0) {      // ‘0’
          d = delay0;
        }
        else if (input[k] == 1) { // ‘1’
          d = delay3;
        }
        Serial.print(input[k]);
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


////////////////////// Rx ////////////////////////////////////////////////////////////////////////////////////////////////

void initRx() {
  sbi(ADCSRA, ADPS2) ; // this for increase analogRead Speed
  cbi(ADCSRA, ADPS1) ;
  cbi(ADCSRA, ADPS0) ;
  Radio.init();
  Radio.set_frequency(radio_freq);
  pinMode(A1, INPUT);
  //  delay(500);
  Serial.println("Ready...");
}

void checkBit() { // Check 8 bits
  if (check) {
    lastCount = millis();
    if (count > 4 ) { // if count is more than 5 cycles
      res += "1";
    }
    else {
      res += "0";
    }
    if (check) {
      countBit ++;
      check = false;
      if (countBit == 8) {
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
        //        Serial.println("All Data : " + allData);
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


void Settings(String moe , String value ) {

  unsigned long  val  =  value.toInt();
  int m  =  moe.toInt();

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
    case 3 :
      Serial.print("change Detection mode to ");
      mode = val;
      Serial.println(mode);
      break;
    default:
      Serial.print("Invalid Input");
      break;
  }
}



bool initial() {
  int tmp =  analogRead(A1) ;
  if (!mode) {
    if (check == false && analogRead(A1) >= initial_signal) { // signal at 25
      timerFM = micros();
      count = 0;
      prev  = 0;
      max   = 0;
      check = true;
    }
  }
  else if (check == false && analogRead(A1) >= initial_signal) { // signal at 25
    timerFM = micros();
    count = 0;
    prev  = 0;
    max   = 5000;
    check = true;
  }
  return check;
}

void receiveData() {
  while (check && (micros() - timerFM) <=  elapse_time ) { // if signal receive begin loop for counting data
    int tmp = analogRead(A1);

    if (!mode) {
      if (max < tmp) max = tmp;
      if (tmp < 20 && max - tmp >  r_slope) {  // if peak down to base  it is 1 cycles
        count ++;
        max = 0;
      }
    } else {
      if (max > tmp) max = tmp;
      if ( tmp - max > r_slope) {
        count ++;
        max = 5000;
      }
    }
  }
}

String Rx() {
  drop(100); // drops in 100ms
  //  Serial.println(analogRead(A1));
  if (initial()) {
    receiveData();
  }
  checkBit();
  if (all.length() != 0 && all[all.length() - 1] == '#') {
    return all;
  } else {
    return "";
  }
}
