// Go-Back-N [TX]
////////////////////////////////// Window Size ///////////////////////////////////
//window_size = 2^m - 1 : m = 3
int s_window = 7;
int s_first = 0;
int s_now = 0;

///////////////////////////////////// FRAME //////////////////////////////////////
int inFrame = 0;
int outFrame = 0;

////////////////////////////////////// ACK ///////////////////////////////////////
bool ACK_received = false;
int ACK_NO = 0, Frame_NO = 0;

///////////////////////////////////// Timer //////////////////////////////////////
bool timeOut = false;
int timer = 0;
int timeOver = 5000;

////////////////////////////// for Store and Purge ///////////////////////////////
int f_storage[10] 
memset(f_storage,-1,sizeof(f_storage)
       
#define IFrame 0
#define SFrame 2
#define UFrame 3

void setup() {
  // put your setup code here, to run once:

}

void loop() {
//////////////////////////////// wait for request ///////////////////////////////
  if received_request()
  {
    if(s_next - s_first >= s_window)
    {
      sleep();
    }
    getdata();
    makeFrame(s_now);
    storeFrame(s_now); 
    sendframe(s_now);
    s_now = s_now + 1;
    if(timer == 0)
    {
      startTimer(); // millis run
    }
  }

////////////////////////////// ACK Arrive ////////////////////////////////
  if(ACK_received)
  {
    Receive(ACK_NO);
    if(corruptd(ACK))
    {
      sleep();
    }
    if((ACK_NO > s_first) && (ACK_NO <= s_now))
    {
      while(s_first <= ACK_NO)
      {
        purgeFrame(s_first); //sliding window
        s_first = s_first + 1;
      }
      stopTimer(); // millis = 0
    }
  }

/////////////////////////////// Time Out ////////////////////////////////
  if(timeOut) // after time > timeOver
  {
    startTimer(); // millis run
    temp = s_first;
    while(temp < s_now)
    {
      sendFrame(temp);
      temp = temp + 1;
    }
  }
}

////////////////////////////// make Frame ////////////////////////////////
void makeFrame() //สร้างเฟรมที่จะส่ง
{
  outFrame = 0; //Reset
  switch (type)
  {
    case 'I': //I-Frame [00|001|0101010101010101] = [ประเภทเฟรม|หมายเลข Frame|ข้อมูล]

      outFrame = IFrame;
      outFrame <<= 3;
      outFrame |= Frame_NO;
      outFrame <<= 10;
      outFrame |= Data;
      break;
    case 'S': //S-Frame [10|10000000000000000|1] = [ประเภทเฟรม|ประเภทการทำงาน|หมายเลข ACK]
      //Serial.println("F number ");
      //Serial.println(ackNo);
      outFrame = SFrame;
      outFrame <<= 10;
      outFrame |= Data;
      outFrame <<= 1;
      outFrame |= ackNo;
      break;
    default: //U-Frame [11|10101010101010101] = [ประเภทเฟรม|ประเภทการทำงาน]
      outFrame = UFrame;
      outFrame <<= 11;
      outFrame |= Data;
      break;
  }
  //Serial.println("outF");
  //Serial.println(outFrame,BIN);
}

////////////////////////////// check Frame ////////////////////////////////
void checkFrame()
{
  if (checkError(inFrame)) //Check Error ของเฟรมที่รับเข้ามา ด้วย Check sum
  {
    if (inFrame >> 10 == IFrame) //ข้อมูลที่รับเข้ามาเป็น I-Frame
    {
      //Serial.println("--------------------------------------------");
      //Serial.println("SK");
      int temp = (inFrame >> 9) & B111 ;
      if (temp == ACK_NO)
      {
        if (ackNo > 7) ACK_NO = 0;
        else ACK_NO++;
        receivedACK = true;
      }
      else
      {
        receivedACK = false;
      }
      type = 'S'; //S-Frame สำหรับตอบ ACK
      Data = 0x200;
      delay(500);
      sendFrame(true); //ตอบ ACK แบบไม่จับเวลา
//            Serial.println(outFrame, BIN); 
    }
    else if (inFrame >> 10 == UFrame) //ข้อมูลที่รับเข้ามาเป็น U-Frame
    {
      int temp = (inFrame >> 5)&B11111;
      //Serial.println("...Accepting Request...");
      Data = 0x180;
      type = 'U';
      sendFrame(true); //ตอบ ACK แบบไม่จับเวลา
      if (temp == B11100) //Code สำหรับเริ่มการทำงาน
      {
        Serial.print("S");
        //Serial.println("--------------------------------------------");
      }
      else if (temp == B00100) //Code สำหรับจบการทำงาน
      {
        Serial.print("E");
        //Serial.println("--------------------------------------------");
      }
    }
    else if (inFrame >> 10 == SFrame) //ข้อมูลที่รับเข้ามาเป็น S-Frame
    {
      //Serial.println("--------------------------------------------");
      //Serial.println("RK");
      startTimer = false;
      Frame_NO = (inFrame >> 5) & 1; //เปลี่ยนหมายเลข Frame ให้ตรงกับ หมายเลข ACK ที่รับเข้ามา
    }
  }

////////////////////////////// Store data ////////////////////////////////
void storeFrame(int framenum,int data){
  if(f_storage[framenum]==-1 ){
      f_storage[framenum] = data
  }
  else {
    Serial.print("Storage Has already Data Storage Number >> ")
    Serial.println(framenum)
  }
}

////////////////////////////// Purge Frame ////////////////////////////////
void purgeFrame (int framenum){
    f_storage[framenum] = -1
       //Serial.print("Clear Frame No.")
       //Serial.print(framenum)
       
}
