/*
433 MHz receiver test
Made by Sean Bailey
www.baileytec.net
*/
#include <VirtualWire.h>
void setup()
{
    Serial.begin(9600); //we wanna be able to read what we got
    vw_set_rx_pin(12);//connect the receiver data pin to pin 12
    vw_setup(4000);  // speed of data transfer in bps, maxes out at 10000
    pinMode(13, OUTPUT);
 
    vw_rx_start();       // Start the receiver PLL running
}
    void loop()
{
    uint8_t buf[VW_MAX_MESSAGE_LEN];
    uint8_t buflen = VW_MAX_MESSAGE_LEN;
 
    if (vw_get_message(buf, &buflen)) // if we get a message that we recognize on this buffer...
    {
 
      String out = "";
      digitalWrite(13,1); // we have data coming in, let's aknowledge somehow that we've received it
      for (int i = 0; i<buflen; i++)
      {
         out +=(char)buf[i]; // fill the string with the data received 
      }
      Serial.print(out);
      Serial.print(" "); // simple enoughSerial.print(out);
      digitalWrite(13,0); //transmission ended
    }
}
