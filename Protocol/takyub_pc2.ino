#include<SoftwareSerial.h>

SoftwareSerial mySerial(10, 11);


void setup() {
  Serial.begin(115200);
  mySerial.begin(57600);
  //Set Client ID
  Serial.flush();
  Serial.println("============================");
}

bool canSend = true;
long timer = 0;
int dataSize = 5;

int frameNo = 0;
int ackNo = 0;

char flag = ':';
String bufferToSend = "";
String dataFrameSend = "";
String dataFrameRecieve = "";

String allData = "";

void loop() {
  //===================== Send Timer ===============//
  if (!canSend && millis() - timer >= 5000) {
    Serial.println(">>>>>>>>>>>> Sending Status : TimeOut");
    Serial.println(">>>>>>>>>>>> Sending again!");
    //Send dataFrame
    sendFrame(dataFrameSend);
  }

  //===================== Send =====================//
  if (Serial.available()) {
    //Get Input into bufferToSend
    String inputData = Serial.readStringUntil('\n');
    for (int i = 0; i < inputData.length(); i++) {
      bufferToSend += inputData[i];
    }
  }
  if (canSend && bufferToSend.length() != 0) {
    bool isAckFrame = false;
    //Slicing Data from Buffer
    String data = "";
    if (bufferToSend.length() < dataSize) {
      int num = bufferToSend.length();
      for (int i = 0; i < num; i++) {
        data += bufferToSend[0];
        bufferToSend.remove(0, 1);
      }
    }
    else {
      for (int i = 0; i < dataSize; i++) {
        data += bufferToSend[0];
        bufferToSend.remove(0, 1);
      }
    }

    String forSum = "I" + String(frameNo) + data  ;
    dataFrameSend = "I" + String(frameNo) + data + ";" + getSum(forSum) + "#" ;
    //Send dataFrame
    sendFrame(dataFrameSend);
    canSend = false;
  }

  receive();
  isSendEnd();

}

void receive() {
  if (mySerial.available()) {
    String frame = mySerial.readStringUntil("#");
  
    // I-Frame
    if (frame[0] != 'A') { // is I-Frame
      String data = "",sum = "";
 
      int i = 2;
      for (i = 2 ; i < 7 && frame[i] != ';' ; ++i) {
        data += String(frame[i]);
      }
 
      for(int j = 1 ; j <= 2  && frame[i + j] != '#'; j++){
        sum += String(frame[i+j]);
      }
     
      Serial.println("Received I-Frame");
      Serial.println("frame No. : " + String(frame[1]) );
      Serial.println("Data      : " + (data) );
      Serial.println("Sum       : " + (sum) );

      String forSum = "I" + String(frame[1]) + data ;

      if (getSum(forSum) == sum) {
        Serial.println("Frame is Correct");
        allData += data;
        Serial.println("[Sending Ack]");
        auto fn = String(frame[1]).toInt();

        if (ackNo == fn) { // if ack loss
          ackNo = !ackNo;
        }else  Serial.println("Frame Already Receive Reject!");
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
        canSend = true;
        frameNo = !frameNo;
      } else {
        Serial.println("Frame is Corrupted \nDiscarded");
      }
      // send next frame
    }
  }
}


void sendFrame(String frame) {
  Serial.println("Send frame : " + frame);
  Serial.println("Frame No : " + String(frameNo));
  Serial.println("============================");
  for (int i = 0; i < frame.length(); i++) {
    mySerial.write(frame[i]);
  }
  canSend = false;
  timer = millis();
}


void sendAck(String frame) {
  Serial.print("Send Ack : ");
  Serial.println(ackNo);
  Serial.println("============================");
  for (int i = 0; i < frame.length(); i++) {
    mySerial.write(frame[i]);
  }
}

void isSendEnd(){
  if(allData[ allData.length() - 1] == ':'){
    Serial.println("Send End With Data : " + allData);
    allData = "";
  }
}


String getSum(String string) {
  int sum = 0;
  for (int i = 0; i < string.length(); i++) {
    sum += string[i];
  }
  uint8_t a = sum & 0xFF , b = (sum >> 8) & 0xFF ;

  return String((char)a) + String((char)b);
}
