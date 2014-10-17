
// AFSK in is pin A3

#define RX  // RX firmware
#define INTVREF  // use internal 1.1V ref for AD
#define LED 13
#define BAUDRATE 115200

//#define TESTRXLEVEL 

void setup() 
{  
  pinMode (LED, OUTPUT);  
  SerialOpen(BAUDRATE);
  initFsk();
  digitalWrite(LED, 1);
}

void loop() 
{
  uint8_t i,n,d;
  
  delay(10);
  n = FskAvailable();
  for (i=0;i<n;i++)
  {
    d = readFSK();
    SerialWrite(d);
  }
  
}



