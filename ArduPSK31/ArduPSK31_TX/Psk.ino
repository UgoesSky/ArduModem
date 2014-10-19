#include "Varicode.h"

// out pin 11
// in pin A3

#define PSK_BUFFER_SIZE 64
volatile uint8_t inBufPt;
volatile uint8_t outBufPt;
volatile uint16_t serialPSKBuffer[PSK_BUFFER_SIZE];

// 8khz sampling , 64khz / 4
// 8000 / 256 = 31,25 baud

//  8 samples/cycle, 8000/ 8 = 1000hz, 1000/31.25 = 32 cycles per bit
// 16 samples/cycle, 8000/16 =  500hz,  500/31.25 = 16 cycles per bit

#define mysinlen 16
#define myamplen 4*16
#define mysymblen 16*16

static const int8_t sin_table[mysinlen] =
{
  0, 47, 88, 115, 125, 115, 88, 47, 0, -47, -88, -115, -125, -115, -88, -47, 
};
static const int8_t amp_table[myamplen] = 
{
   125, 124, 124, 123, 122, 121, 119, 117, 115, 112, 110, 107, 103, 100, 96, 92, 
   88, 83, 79, 74, 69, 64, 58, 53, 47, 42, 36, 30, 24, 18, 12, 6, 
   0, -6, -12, -18, -24, -30, -36, -42, -47, -53, -58, -64, -69, -74, -79, -83, 
   -88, -92, -96, -100, -103, -107, -110, -112, -115, -117, -119, -121, -122, -123, -124, -124, 
};

void initPsk() 
{    
  // DA part
  pinMode(11,OUTPUT);    // 11 or 3 = output
  OCR2A = 128;  
  TCCR2B =  (1<<CS20);   //run at 64khz
  TCCR2A = _BV(COM2A1) | _BV(WGM20)| _BV(WGM21); // connect pin 11 to timer 2 channel A
  TIMSK2 |= (1<<TOIE2);  // enable tim2 interrupt
  sei();                   // set interrupt flag
}

// DA part
  
boolean is_change = true;
boolean is_invert = false;
uint8_t phase = 0;
uint8_t oddcycle = 0;
uint8_t bitct = 0;
volatile uint16_t data;

  
ISR(TIMER2_OVF_vect) // this runs at 64khz
{
  if (oddcycle < 7) oddcycle++; // runs 8khz
  else
  { 
    int8_t sinval = sin_table[phase%mysinlen]; 
    oddcycle = 0;    
    
    if (is_change) 
    {
      int16_t resval = amp_table[phase>>2]; 
      if (is_invert) OCR2A = 127 + ((sinval * resval) >> 7); 
      else           OCR2A = 127 - ((sinval * resval) >> 7);             
    }  
    else 
    {
      if (is_invert) OCR2A = 127 + sinval; 
      else           OCR2A = 127 - sinval; 
    }
    
    phase++;
    if (phase == 0) 
    //if (phase >= mysymblen) 
    {
      //phase = 0; 
      if (is_change) is_invert = !is_invert;
      if (bitct == 0)
      {
        if ((uint8_t)(inBufPt - outBufPt)%PSK_BUFFER_SIZE > 0) 
        {
          uint8_t i;
          outBufPt++; 
          if (outBufPt >= PSK_BUFFER_SIZE) outBufPt = 0;
          data = serialPSKBuffer[outBufPt];  
          for (i=0;i<16;i++) 
            if ((data & (1<<i)) != 0) bitct = i;
          bitct+=3;
        }
        else is_change = true;        
      }
      // bitcounter 1..16 data / 0 idle
      if (bitct > 0) // databit
      {
        bitct--;
        is_change = !(data & (1<<bitct));
      }
    }
  }
}

void writePSK(uint8_t ch) 
{
  uint8_t t = inBufPt;
  if (++t >= PSK_BUFFER_SIZE) t = 0;
  serialPSKBuffer[t] = ascii_to_psk[ch - 0x20];
  inBufPt = t;
}



