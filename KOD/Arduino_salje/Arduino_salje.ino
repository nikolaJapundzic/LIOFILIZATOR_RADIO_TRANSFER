#include <SoftwareSerial.h>
#include "HX711.h"
#include <OneWire.h>


OneWire  ds(3);  // on pin 10 (a 4.7K resistor is necessary)
String temperature = "";
String Vrednost = "";
float masa = 0;
HX711 scale(A1, A0); // DOUT, SCK

//Code By ArduinoALL
SoftwareSerial ArduinoSerial(11, 10); // RX, TX
void setup() {
Serial.begin(115200);
ArduinoSerial.begin(9600);

scale.set_scale(2025.f);
  //2280 ovo je standard
  scale.tare();
}
void loop() {
while (ArduinoSerial.available() > 0) {
  float val = ArduinoSerial.parseFloat();
  if (ArduinoSerial.read() == '\n') {
    if (val == 0xA0) {
      boolean flag = true;
      while(flag){
        //TEMPERATURA
      byte i;
      byte present = 0;
      byte type_s;
      byte data[12];
      byte addr[8];
      float celsius, fahrenheit;
      
      if ( !ds.search(addr)) {
        masa = scale.get_units();
        temperature = temperature + masa;

        //-----------------------------------
        int duzinaNiza = temperature.length() +1;
        char niz [duzinaNiza];
        temperature.toCharArray(niz, duzinaNiza);
        //-----------------------------------

        if(temperature.length()<5){
          ArduinoSerial.print(Vrednost);
          ArduinoSerial.print("\n");
        }else{
          Vrednost = temperature;
          ArduinoSerial.print(niz);
          ArduinoSerial.print("\n");
          Serial.println(niz); 
        }   
        temperature = "";
        flag = false;
        ds.reset_search();
        delay(250);
        return;
      }
    
      if (OneWire::crc8(addr, 7) != addr[7]) {
          return;
      }
     
      switch (addr[0]) {
        case 0x10:
          type_s = 1;
          break;
        case 0x28:
          type_s = 0;
          break;
        case 0x22:
          type_s = 0;
          break;
        default:
          return;
      } 
    
      ds.reset();
      ds.select(addr);
      ds.write(0x44, 1);        // start conversion, with parasite power on at the end
      
      delay(1000);     // maybe 750ms is enough, maybe not
      // we might do a ds.depower() here, but the reset will take care of it.
      
      present = ds.reset();
      ds.select(addr);    
      ds.write(0xBE);         // Read Scratchpad
    
      for ( i = 0; i < 9; i++) {           // we need 9 bytes
        data[i] = ds.read();
      }
      int16_t raw = (data[1] << 8) | data[0];
      if (type_s) {
        raw = raw << 3; // 9 bit resolution default
        if (data[7] == 0x10) {
          // "count remain" gives full 12 bit resolution
          raw = (raw & 0xFFF0) + 12 - data[6];
        }
      } else {
        byte cfg = (data[4] & 0x60);
        // at lower res, the low bits are undefined, so let's zero them
        if (cfg == 0x00) raw = raw & ~7;  // 9 bit resolution, 93.75 ms
        else if (cfg == 0x20) raw = raw & ~3; // 10 bit res, 187.5 ms
        else if (cfg == 0x40) raw = raw & ~1; // 11 bit res, 375 ms
        //// default is 12 bit resolution, 750 ms conversion time
      }
      celsius = (float)raw / 16.0;
      temperature += celsius;
      temperature += " ";
      //TEMPERATURA
      }
      
    }
  }
}
delay(1000);
}
