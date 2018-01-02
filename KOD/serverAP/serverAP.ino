
#include <ESP8266WiFi.h>
#include <WiFiClient.h>
#include <ESP8266WebServer.h>
#include <SoftwareSerial.h>


//OneWire  ds(10);  // on pin 10 (a 4.7K resistor is necessary)
SoftwareSerial NodeSerial(D10, D11); // RX | TX
String sve = "";
String Vrednost = "";

const char *ssid = "Liofilizator";
const char *pass = "kodirano";
String command;
ESP8266WebServer server(80);

void respond(){
  server.send(200,"text/plain","Dobro Dosli");
}

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
NodeSerial.begin(9600);
pinMode(BUILTIN_LED,OUTPUT);
//Serial.println(" memulai konfigurasi aksees Point");
WiFi.softAP(ssid,pass);
IPAddress IPserver = WiFi.softAPIP();
Serial.println(" IP address server : ");
Serial.println(IPserver);
server.on("/",respond);
server.begin();
server.onNotFound([](){
  //command=" ini data dari klien : ";
  //command=server.uri();
  NodeSerial.print(0xA0);
  NodeSerial.print("\n");
  
  while (NodeSerial.available() > 0) {
    //Serial.println(NodeSerial.readString());
    sve = NodeSerial.readString();
    
    if(sve[0] == '>' && sve[1] == '>' && sve[2] == '>'){
      Vrednost = sve;
    }
    
    Serial.println(Vrednost);
  }
  delay(100);
  
  command = "";
 // command = command + sensorValue;
  server.send(200,"text/plain",Vrednost);
});


}

void loop() {
  // put your main code here, to run repeatedly:
  server.handleClient();
  if(command.equals("/off")){
    digitalWrite(BUILTIN_LED,HIGH);//turn off BUILTIN LED
    
  }
}
