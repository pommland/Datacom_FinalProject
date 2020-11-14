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
int ACK_NO = 0;

///////////////////////////////////// Timer //////////////////////////////////////
bool timeOut = false;
int timer = 0;
int timeOver = 5000;

void setup() {
  // put your setup code here, to run once:

}

void loop() {
  
////////////////////////// wait for request /////////////////////////////
  if received_request()
  {
    if(s_next - s_first >= s_window)
    {
      sleep();
    }
    getdata();
    makeframe(s_now);
    storeframe(s_now); 
    sendframe(s_now);
    s_now = s_now + 1;
    if(timer == 0)
    {
      startTimer(); // millis run
    }
  }

///////////////////////////// ACK Arrive ////////////////////////////////
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

////////////////////////////// Time Out ////////////////////////////////
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
