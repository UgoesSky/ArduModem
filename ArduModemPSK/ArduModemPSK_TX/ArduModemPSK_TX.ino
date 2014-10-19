
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
  if (time - lasttime >= 1500) // 1000 baud = 100 char /sec
  {
    // send data over modem
    lasttime = time;
    writePSK('$');              //  1 0x24
    writePSK('T');              //  2 0x54 
    writePSK('G');              //  3 0x47
    writePSK(c++);              
    writePSK(1);              
    writePSK(2);              
    writePSK(3);              
    writePSK(13);              
  }        
}



