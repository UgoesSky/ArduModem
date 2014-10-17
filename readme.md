ArduModem

Audio modem based on arduino. Transmitter data input is over serial line, receiver data output is over serial line. 

Transmitter audio output is pin11 64khz pwm. This needs to be smoothed by an RC lowpass (22k and 1nf).

Receiver audio input is pin A3. DC bias voltage must be half of Vref pin. Use 2 22k resistors as divider and decouple with 1nf. Vref can be 5V, or 1.1V choosen with #define INTVREF. Audio swing max is 5V / 1V depending on #define INTVREF. 

Audio level and bias can be checked by using #define TESTRXLEVEL and then run DisplayArduModem_PC.pde (use processing) to see the arduino input. Example how it should look are in the DisplayArduModem_PC folder. Wrong bias will destroy the modem performance.

FSK is 1000 baud, using 1000hz and 2000hz tones. Minimum SNR is around 10db.

PSK is 250 baud, 1000hz center, Minimum SNR is around 1db

The internal serial buffer is 64 bytes. Do NOT send more data than the modem can transfer. Both modems are async, no checksum.





