

// out pin 11
// in pin A3

#define PSK_BUFFER_SIZE 64
volatile uint8_t inBufPt;
volatile uint8_t outBufPt;
volatile uint8_t serialPSKBuffer[PSK_BUFFER_SIZE];

uint8_t chksum;

void initPsk() 
{    
  #if defined (RX)
  // AD part
    OCR2A = 128; // pin 11
    TCCR2B =  (1<<CS21);   // run at 8khz
    TCCR2A =  0;           // normal mode
    TIMSK2 |= (1<<TOIE2);  // enable tim2 interrupt
    
    pinMode(12, OUTPUT);   // pin = output  
    pinMode(11, OUTPUT);   // pin = output  
    DIDR0 = 0x3F;          // digital inputs disabled
    #if defined (INTVREF)
      ADMUX = 0xE3;          // measuring on ADC3, use 1.1V reference, left adjust
    #else
      ADMUX = 0x63;          // measuring on ADC3, use VCC reference, left adjust
    #endif
    ADCSRA = 0x87;         // AD-converter on, interrupt disabled, prescaler = 128
    ADCSRB = 0x00;         // AD channels MUX on
  #endif

  sei();                   // set interrupt flag
}

#if defined (RX)
  
  #define OFFS (128)
  #define SINLEN 16
  #define SAMPINSYMB 256
  
  static const int8_t sin_table[SINLEN] =
  {
    0, 47, 88, 115, 125, 115, 88, 47, 0, -47, -88, -115, -125, -115, -88, -47, 
  };
  static const int8_t cos_table[SINLEN] =
  {
    125, 115, 88, 47, 0, -47, -88, -115, -125, -115, -88, -47, 0, 47, 88, 115, 
  };

  int16_t sum = 0;
  
  byte history = 0;
  byte bitct = 0;
  byte bitphase = 0;
  byte data;
  byte ct = 0;

  uint8_t phase;
  uint8_t decimation;
  int16_t IpartSum,QpartSum;
  int8_t ang0,ang1,ang2,ang3,ang4;
  
  ISR(TIMER2_OVF_vect) // this runs at 8khz
  {
    uint8_t ad_byte;
    int8_t currentSample;
    int16_t Ipart,Qpart;
    int8_t ang,angdiff;;
    int8_t I,Q;
    boolean is_space;
    
    ad_byte = ADCH;   
    
    #if defined(TESTRXLEVEL)
      SerialWrite(ad_byte);
      ADCSRA = 0xC7; // start AD conversion
    #else
    
    currentSample = ad_byte - OFFS;
    ADCSRA = 0xC7; // start AD conversion

    Ipart = currentSample * sin_table[phase];
    Qpart = currentSample * cos_table[phase];
    
    phase++;
    phase %= SINLEN;

    IpartSum += Ipart>>4;
    QpartSum += Qpart>>4;
    
    // symbol length = 256 , 31.25 baud
    // sinlen = 16, decimation to 16 samples / symbol
    if (phase == 0) // first decimation to 500hz rate = 1/16 symbol
    {
      decimation++;
      decimation %= 16;
      
      if ((decimation%4) ==  0)
      {
        I = IpartSum >> 8;
        Q = QpartSum >> 8;
        ang = _atan2(I,Q);
        IpartSum = QpartSum = 0;
        
        
        //now compare ang to ang of previous symbol
        ang4=ang3; ang3=ang2; ang2=ang1; ang1=ang0; ang0=ang;
        angdiff = ang0-ang4;
        if (angdiff <0) angdiff+=8;
        is_space = ((angdiff<2) | (angdiff>6));
        history <<= 1;  
        if (is_space) history |= 0x01;
            
        if ((history == 0xF0) || (history == 0x0F)) 
        {
          bitphase = (decimation + 8)%16;
        }
      }
      //if (decimation==0) SerialWrite(13);
       
      if (decimation == bitphase) 
      {
        data <<= 1;
        if (is_space) data |= 0x01;
        bitct++;
      
        if (bitct >= 8)
        {
          uint8_t t = inBufPt;
          if (++t >= PSK_BUFFER_SIZE) t = 0;
          serialPSKBuffer[t] = data;
          inBufPt = t;
          bitct = 0;  
        }      
      }
      //if (is_space) digitalWrite(12, HIGH); // marker low
      //else digitalWrite(12, LOW); // marker high
    }
    
    #endif //TESTRXLEVEL    
  }; 

  uint8_t readPSK()
  {
    uint8_t t = outBufPt;
    if (inBufPt != t) 
    {
      uint8_t r;
      if (++t >= PSK_BUFFER_SIZE) t = 0;
      outBufPt = t;
      r = serialPSKBuffer[t];
      chksum ^= r;
      return r;
    }
    return 0;
  }

  uint16_t readPSK16() {
    uint16_t t = readPSK();
    t+= (uint16_t)readPSK()<<8;
    return t;
  }
  uint32_t readPSK32() {
    uint32_t t = readPSK16();
    t+= (uint32_t)readPSK16()<<16;
    return t;
  }

  uint8_t PskAvailable() 
  {
    return ((uint8_t)(inBufPt - outBufPt))%PSK_BUFFER_SIZE;
  }
#endif


