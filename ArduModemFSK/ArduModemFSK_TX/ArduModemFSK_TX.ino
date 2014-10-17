
// AFSK out is pin 11

#define TX  // TX or RX firmware
#define LED 13
#define BAUDRATE 115200

#define HELLOMSG

void setup() 
{
  pinMode (LED, OUTPUT);  
  SerialOpen(BAUDRATE); // GPS baudrate
  initFsk();
  digitalWrite(LED, 1);
}

void loop() 
{
  uint8_t c,d;
  
  #if defined(HELLOMSG)
    delay(200);
    writeFSK('H');
    writeFSK('E');
    writeFSK('L');
    writeFSK('L');
    writeFSK('O');
    writeFSK(13);
  #endif
    
  c = SerialAvailable();
  while (c--) 
  {
    digitalWrite(LED, HIGH);
    d = SerialRead();
    writeFSK(d);
    digitalWrite(LED, LOW);
  }
}



