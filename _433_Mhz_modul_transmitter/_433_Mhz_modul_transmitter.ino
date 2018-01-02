/*
simple transmit test for the 433 MHz modules for Arduino
Made by Sean Bailey
www.baileytec.net
*/
 
 
#include <VirtualWire.h>
 
void setup() {
  pinMode(13,OUTPUT);
  vw_set_tx_pin(12); //set our transmit pin to pin 12
  vw_setup(4000);// speed of data transfer in bps, can max out at 10000
}
 
void loop(){
  int sensorValue = analogRead(A0);
  String myString = String(sensorValue);

  int str_len = myString.length() + 1; 
  char char_array[str_len];
  myString.toCharArray(char_array, str_len);

  Serial.println(sensorValue);
  
//  char[] c = myString.toCharArray(char_array, str_len);

  
  char *senddata = char_array;
  digitalWrite(13,1); //this blinks the onboard LED so we know the system is indeed transmitting
  vw_send((uint8_t *)senddata, strlen(senddata));
  vw_wait_tx(); // Wait until the whole message is gone
  digitalWrite(13,0); //turns the LED off
  delay(1000); //wait a second between transfers
 
}
