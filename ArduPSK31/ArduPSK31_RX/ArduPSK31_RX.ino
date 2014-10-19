
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

#include "Varicode.h"

void setup() 
{  
  pinMode (LED, OUTPUT);  
  SerialOpen(BAUDRATE);
  initPsk();
  digitalWrite(LED, 1);
  time = millis();
  lasttime = time;
}
  
void loop() 
{
  uint8_t c,i,d;
  uint16_t h;
  
  while (PskAvailable())
  {
    d = readPSK();
    //SerialWrite(d);  
      
    for (i=0;i<8;i++)
    {
      checkbit((d&0x80) == 0x80);
      d <<= 1;
    }
    
  }  
}

uint8_t zeros;
uint16_t symb;

void checkbit(boolean b)
{
  if (b)
  {
    if (zeros >=2) symb = 0x01;
    else { symb <<= 1; symb |= 0x01; }
    zeros = 0;
  }
  else
  {    
    if      (zeros == 0) { symb <<= 1; zeros++; }
    else if (zeros == 1) { gotchar(symb>>1); zeros++; } 
  }
}

void gotchar(uint16_t s)
{
  uint8_t i;
  uint16_t v;
  for (i=0;i<96;i++)
  {
    v = ascii_to_psk[i];
    if (s == v)
    {
      SerialWrite(i+0x20);
      continue;
    }
  }
}
