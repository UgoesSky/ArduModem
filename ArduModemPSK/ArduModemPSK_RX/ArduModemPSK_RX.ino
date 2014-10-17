
#define RX  // TX or RX firmware
#define INTVREF  // use internal 1.1V ref for AD
#define LED 13
#define BAUDRATE 115200
//#define TESTRXLEVEL

// PWM out is pin 9 and 10
// AFSK out is pin 11
// AFSK in is pin A3

unsigned long time,lasttime;
uint16_t timediff;

void setup() 
{  
  pinMode (LED, OUTPUT);  
  SerialOpen(BAUDRATE);
  initPsk();
  digitalWrite(LED, 1);
  time = millis();
  lasttime = time;
}
  
uint16_t missedframes = 0;
uint16_t goodframes = 0;

void loop() 
{
  uint8_t c,i,d;
  
  time = millis();
  timediff = time - lasttime;
  if (timediff >= 100)
  {
    lasttime = time;
  }
        
  if (PskAvailable())
  {
    d = readPSK();
    SerialWrite(d);
  }
  
}



