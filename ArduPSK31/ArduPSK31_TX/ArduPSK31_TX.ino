
// AFSK out is pin 11

#define TX  // TX or RX firmware
#define LED 13
#define BAUDRATE 115200

unsigned long time,lasttime;

void setup() 
{
  pinMode (LED, OUTPUT);  
  SerialOpen(BAUDRATE); 
  initPsk();
  digitalWrite(LED, 1);
  time = millis();
  lasttime = time;
}

uint8_t c;

void loop() 
{
  time = millis(); 
  if (time - lasttime >= 3000) // 31 baud max
  {
    // send data over modem
    lasttime = time;
    writePSK(' ');               
    writePSK('0');               
    writePSK('1');               
    writePSK('2');  
    writePSK('a');               
    writePSK('b');               
    writePSK('c');  
  }        
}



