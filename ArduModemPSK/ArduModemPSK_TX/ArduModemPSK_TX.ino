
// AFSK out is pin 11

#define TX  // TX or RX firmware
#define LED 13
#define BAUDRATE 115200

unsigned long time,lasttime;

void setup() 
{
  pinMode (LED, OUTPUT);  
  SerialOpen(BAUDRATE); // GPS baudrate
  initPsk();
  digitalWrite(LED, 1);
  time = millis();
  lasttime = time;
}

uint8_t c;

void loop() 
{
  time = millis(); 
  if (time - lasttime >= 1000) // 1000 baud = 100 char /sec
  {
    // send data over modem
    lasttime = time;
    StartPSK();
    writePSK('$');              //  1 0x24
    writePSK('T');              //  2 0x54 
    writePSK('G');              //  3 0x47
    writePSK(c++);              
    writePSK(1);              
    writePSK(2);              
    writePSK(3);              
    writePSK(4);              
    writePSK(5);              
    writePSK(6);              
    writePSK(7);              
    writePSK(8);              
    writePSK(9);              
    writePSK(10);              
    writePSK(13);              
    SendPSK();
  }        
}



