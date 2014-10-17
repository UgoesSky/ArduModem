

// out pin 11
// in pin A3

#define PSK_BUFFER_SIZE 32
volatile uint8_t inBufPt,outBufPt;
volatile uint8_t serialPSKBuffer[PSK_BUFFER_SIZE];

uint8_t chksum;

// 32khz sampling , 64khz / 2
// 128 samples per cycle 
// 32000 / 128 = 250 baud
// 4 cycles per bit, 250 * 4 = 1000hz

#define mydelay 128
static const uint8_t sin_table_same[mydelay] =
{
  151, 174, 195, 214, 230, 241, 248, 251, 248, 241, 230, 214, 195, 174, 151, 127, 
  103, 80, 59, 40, 24, 13, 6, 3, 6, 13, 24, 40, 59, 80, 103, 127, 
  151, 174, 195, 214, 230, 241, 248, 251, 248, 241, 230, 214, 195, 174, 151, 127, 
  103, 80, 59, 40, 24, 13, 6, 3, 6, 13, 24, 40, 59, 80, 103, 127, 
  151, 174, 195, 214, 230, 241, 248, 251, 248, 241, 230, 214, 195, 174, 151, 127, 
  103, 80, 59, 40, 24, 13, 6, 3, 6, 13, 24, 40, 59, 80, 103, 127, 
  151, 174, 195, 214, 230, 241, 248, 251, 248, 241, 230, 214, 195, 174, 151, 127, 
  103, 80, 59, 40, 24, 13, 6, 3, 6, 13, 24, 40, 59, 80, 103, 127,
};
static const uint8_t sin_table_change[mydelay] = 
{
  151, 174, 195, 214, 229, 240, 246, 248, 245, 238, 226, 210, 192, 171, 149, 127, 
  105, 85, 66, 50, 38, 29, 25, 24, 28, 35, 46, 60, 75, 92, 110, 127, 
  143, 158, 171, 182, 190, 195, 197, 195, 192, 185, 177, 168, 157, 147, 136, 127, 
  119, 112, 106, 102, 100, 100, 101, 103, 107, 111, 115, 119, 122, 125, 127, 127, 
  127, 125, 122, 119, 115, 111, 107, 103, 101, 100, 100, 102, 106, 112, 119, 127, 
  136, 147, 157, 168, 177, 185, 192, 195, 197, 195, 190, 182, 171, 158, 143, 127, 
  110, 92, 75, 60, 46, 35, 28, 24, 25, 29, 38, 50, 66, 85, 105, 127, 
  149, 171, 192, 210, 226, 238, 245, 248, 246, 240, 229, 214, 195, 174, 151, 127,
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
  
byte phase = 0;
boolean is_change = true;
boolean is_invert = false;
byte oddcycle = 0;
byte bitct = 0;
byte data;

  
ISR(TIMER2_OVF_vect) // this runs at 64khz
{
  if (oddcycle < 1) oddcycle++;
  else
  {  
    oddcycle = 0;    
    // this runs at 32 khz
    if (is_change) 
    {
      byte sinval = sin_table_change[phase];
      if (is_invert) OCR2A = sinval ^ 0xFF;
      else           OCR2A = sinval; 
    }  
    else 
    {
      byte sinval = sin_table_same[phase];
      if (is_invert) OCR2A = sinval ^ 0xFF;
      else           OCR2A = sinval; 
    }
    phase++;
    if (phase >= mydelay) 
    {
      if (is_change) is_invert = !is_invert;
      phase = 0; 
      
      // bitcounter 0..7 data / 8 stopbit / 9 idle, else startbit
      if (bitct <= 7) // databit
      {
        // get next bit 
        is_change = (data & (1<<bitct));
        bitct++;
      }
      else if (bitct == 8)
      {
        //stopbit 1000 hz
        is_change = true;
        bitct++;
      }
      else if (bitct == 9)
      {
        if (outBufPt < inBufPt) 
        {
          data = serialPSKBuffer[outBufPt];  
          outBufPt++;
          // startbit no_change
          bitct = 0;
          is_change = false;
        }
        else is_change = true;        
      }
      //if (bitct >= 1) bitct = 0;
      //else bitct++;
      //is_change = (bitct == 1);
     }
   }
 }
; 
  
void writePSK(uint8_t a) 
{
  serialPSKBuffer[inBufPt] = a;
  inBufPt++;
  chksum ^= a;
}
void writePSK16(int32_t a) 
{
  writePSK(a & 0xFF);
  writePSK((a>>8) & 0xFF);
}
void writePSK32(int32_t a) 
{
  writePSK(a & 0xFF);
  writePSK((a>>8) & 0xFF);
  writePSK((a>>16) & 0xFF);
  writePSK((a>>24) & 0xFF);
}
  
void SendPSK()
{
  outBufPt = 0;
}
  
void StartPSK()
{
  byte i;
  inBufPt = 0;
  writePSK(0x00);
} 



