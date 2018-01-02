#include <SoftwareSerial.h>
#include <ESP8266WiFi.h>

//Code By ArduinoALL
SoftwareSerial NodeSerial(D10, D11); // RX | TX
String sve = "";
void setup() {

Serial.begin(115200);
NodeSerial.begin(9600);
}
void loop() {
NodeSerial.print(0xA0);
NodeSerial.print("\n");

while (NodeSerial.available() > 0) {
  //Serial.println(NodeSerial.readString());
  sve = NodeSerial.readString();
  Serial.println(sve);
  }
  delay(100);
}
