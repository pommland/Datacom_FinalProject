#include<SoftwareSerial.h>
SoftwareSerial mySerial(10, 11);

String dataWord;
String wordEnter;
String package = "";
String data = "";
String bitData = "";
int cntack = 0  ;
int frameCount = 0;
int  ackreceive =  -1 ;
boolean firstsend = true ;
boolean enterWord ;
String tempPackage = "";
unsigned long clk ;

//// Variables for getACK
boolean resending  = false;
boolean failed = false;
boolean timeout =  false ;
boolean timerstart  = false ;

//// Variables for Receive
int num_rx = 0;
int count_rx = 0;
uint16_t tempChar = 0;
String Lstpackgage = "";
int i ;

//// Ack
boolean sendAck = false;
boolean ackStart = false;
boolean ackTimeout = false;
boolean resendingAck = false;
unsigned long clkAck = 0;
String tempAckPackage = "";

void resend() {
  if (timeout && timerstart) {
    clk = millis() ;
    if (!resending) {
      //      --i;
      resending = true;
      //      frameCount = !frameCount;
    }
    //    resending = true ;
    timeout = false ;
    for (int k = 0; k < tempPackage.length(); k++) {
      mySerial.write(tempPackage[k]);
    }

    mySerial.write('\n');
    Serial.println("[ReSend Frame]");
    Serial.println("Temppackage  = " + String(tempPackage));
    Serial.println("****************************************************");
    delay(500);
    //    i++;
    //    resending  = false ;
  }
}

void resendAck() {
    for (int k = 0; k < tempAckPackage.length(); k++) {
      mySerial.write(tempAckPackage[k]);
    }
    mySerial.write('\n');
    Serial.println("[ReSend Ack]");
    Serial.println("Temp Ack Package  = " + String(tempAckPackage));
    Serial.println("****************************************************");
    delay(500);
  }

void sendEnd() {
  if (i == wordEnter.length() && enterWord) {

    frameCount = 0 ;
    enterWord = false;
    firstsend = true ;
    ackreceive = -1;
    mySerial.write("~;~;~\n");
  }
}
void receiveframe() {
  if (mySerial.available()) {
    //// [type|frameNo|Data]
    String type = mySerial.readStringUntil(';');
    String frameNo = mySerial.readStringUntil(';');
    String receiveData = mySerial.readStringUntil('\n');
    //    Serial.println("type : " + type);
    if (type == "I" && frameNo.toInt() == cntack) {
      Serial.println(frameNo + " : " + cntack);
      //// Convert binary to String
      for (int i = 0; i < 4; i++) {
        uint16_t tempNum = 0;
        if (receiveData[i] == '0') {
          tempNum = 0; // รับข้อมูลมาเป็น int
        }
        else if (receiveData[i] == '1') {
          tempNum = 1;
        }
        else if (receiveData[i] == '2') {
          tempNum = 2;
        }
        else if (receiveData[i] == '3') {
          tempNum = 3;
        }

        tempNum = tempNum << (num_rx * 2);
        tempChar = tempChar | tempNum;
        num_rx += 1;
        if (num_rx == 4) {
          Serial.println("[Receive Frame]");
          Serial.println("Frame No. : " + String(frameNo));
          Serial.print("Word : ");
          Serial.println(char(tempChar));
          Serial.println("Data : " + receiveData);

          //// ACK Package
          cntack++;
          cntack %= 2;
          String ackPackage = "A;" + String(cntack) + ";This is ACK";
          tempAckPackage = ackPackage;
          for (int ll = 0; ll < ackPackage.length(); ll++) {
            mySerial.write(ackPackage[ll]);
          }
          sendAck = true;
          ackStart = true;
          clkAck = millis();
          Serial.println("Ack No."  + String(cntack) + " has been sent");
          Serial.println("****************************************************");
          //          cntack++;

          tempChar = 0;
          num_rx = 0;
        }
      }
    }
      //      delay(1500);

    else if (type == "A") {
      resending = false;
      timerstart = false;
      timeout = false;
      clk = 0 ;
      //// get ACK*
      //// ACK Receive
      //      double ackTimeOut = millis();
      //      boolean getAck = false;
      //      while(!getAck){
      //        if(millis() - ackTimeOut >= 3000){
      //          //temp = package;
      //          timeOut = true;
      //          getAck = true;
      //          break;
      //        }
      ackreceive = frameNo.toInt();
      Serial.println("[Receive ACK]");
      Serial.println("Header : " + type);
      Serial.println("ACK No. : " + frameNo);
      ackreceive = frameNo.toInt();
      resending  = false ;
      sendAck = false;
      ackStart = false;
      clkAck = 0;
      Serial.println("****************************************************");

      //      failed = true;
      //      getAck = true;
      //      timeOut = false;
      //      delay(1500);
      //        }
    }

    else if (type == "~" && frameNo == "~") {
      Serial.println("Reset Ack");
      cntack = 0 ;
      frameNo = "0";

    }
    else if(frameNo.toInt() != ackreceive){
      Serial.print(frameNo + " : " + cntack);
      resendAck();
    }
  }
}
void sendframe () {
  //  Serial.println( String(ackreceive) + ":" + String(frameCount));
  //  Serial.println(i);
  while (i < wordEnter.length() && (firstsend || ackreceive == frameCount )
         && !resending) { //&& !sendAck && !resendingAck
    firstsend = false  ;

    data = "";
    bitData = "";
    uint16_t temp = 0;
    char tempWord = wordEnter[i];
    Serial.println(String(i) + ":" + String(wordEnter[i]));
    for (int j = 0; j < 4; j++) {

      temp = tempWord & 3;

      if (temp == 0) {
        data += "0"; // ใช้ส่งจริง
        bitData += "00"; // ใช้ดู bit
      }
      else if (temp == 1) {
        data += "1";
        bitData += "01";
      }
      else if (temp == 2) {
        data += "2";
        bitData += "10";
      }
      else if (temp == 3) {
        data += "3";
        bitData += "11";
      }
      tempWord = tempWord >> 2;
      //      Serial.println("word Enter : " + String(wordEnter) );
    }
    //    Serial.println("temp : " + String(temp));

    //// Send Frame
    //// timeOut
    //        if(timeOut){
    //          for(int i=0;i<tempPackage.length();i++){
    //          mySerial.write(tempPackage[i]);
    //          }
    //        Serial.println("TimeOut Resend!!!");
    //        }
    //
    //        //// ส่ง Failed ต้องส่ง Frame เก่า
    //        else if(failed){
    //          for(int i=0;i<tempPackage.length();i++){
    //          mySerial.write(tempPackage[i]);
    //          }
    //        Serial.println("Send Failed Resend!!!");
    //        Serial.println("Send Frame : " + String(frameCount-1));
    //        }

    //// Not timeOut
    //        else{
    package = "I;" + String(frameCount) + ";" + data;
    tempPackage = package;

    for (int k = 0; k < package.length(); k++) {
      mySerial.write(package[k]);
    }

    mySerial.write('\n');

    //// Print data
    Serial.println("[Send Frame]");
    Serial.println("Send Frame : " + String(frameCount));
    Serial.println("Bit Data : " + bitData);
    Serial.println("Data : " + data);
    Serial.println("****************************************************");

    frameCount++;
    frameCount %= 2;
    //Serial.println(String(frameCount));
    //Serial.println("*****************************");
    i++;
    Serial.println("Timer Start");
    clk = millis();
    timerstart = true ;
    break;
    //      }
  }
}
void ENTERword () {
  if (Serial.available()) {
    enterWord = false;
    while (!enterWord) {
      if (Serial.available()) {
        wordEnter = Serial.readStringUntil('\n');
        dataWord = wordEnter;
        enterWord = true;
        i  = 0 ;
      }
    }
  }
}


void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  mySerial.begin(57600);
  Serial.flush();
  mySerial.flush();
}

void loop() {
  // Serial.print("Ack on loop :"+String(ackreceive));
  //// Enter word
  ENTERword();



  //// convert String to Binary and mySerial write

  //for (int i = 0; i < wordEnter.length(); i++) {
  //Serial.print( "Ack that Receive:" + String(ackreceive) + "Frame cnt:" + String(frameCount));
  sendframe();

  receiveframe();
  
  if (millis() - clk > 2000 && timerstart) {
    timeout = true;
    Serial.println("clock timeout: " + String(clk) + " : " + String(millis()));
  }
  else if (millis() - clk < 2000) {
    timeout = false ;
  }

//  //// Ack Timeout
//  if (millis() - clkAck > 2000 && ackStart) {
//    Serial.println("Ack lost");
//    ackTimeout = true;
//  }
//  else if (millis() - clkAck < 2000) {
//    ackTimeout = false;
//  }


  //// after send success
  if ((timeout || ackreceive != frameCount) && !firstsend) {
    resend() ;
  }
//  else if (ackTimeout){
//    Serial.println("need resend Ack");
//    ackTimeout = false;
////    resendAck();
//  }
  else {
    sendframe();
    sendEnd();
  }

  //// Receive

}
