#include <Ethernet.h>
#include <EthernetClient.h>
#include <EthernetServer.h>
#include <EthernetUdp.h>
#include <SPI.h>
#include <dht.h>
//-------------------------------------------------------------
#define dht_dpin A0 //Define the signal receiving by Analog pin: A0   
dht DHT; 
// Define the MAC Address on your Ethernet Shield => Syntax: Ethernet.begin(mac,IP)
//You can define any MAC address on you 
byte mac[] = {0x90, 0xA2, 0xDA, 0x00, 0x23, 0x36}; 
byte ip[] = {200,200,200,200}; //This is a static IP address
byte dnserver[] = {200,200,200,100}; //This is a static DNS address
byte subnet[] = {255,255,0,0}; //This is a static Sub-network Mask
byte gateway[] = {200,200,254,254}; //This is a static gateway address
char server[] = "api.pushingbox.com";
EthernetClient client; //define 'client' as object
String data;
float temp; //溫度
float humidity; //濕度
boolean signalflag = false; //true or false
//-----------------------------------------------

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  if(Ethernet.begin(mac) == 0){
    Serial.println("Using DHCP,Please use Static IP...");
    Ethernet.begin(mac,ip,dnserver,gateway,subnet);
    }
    delay(1000);
}
//------------------------------------------------
void loop() {
  // put your main code here, to run repeatedly:
    DHT.read11(dht_dpin);
    temp = DHT.temperature;
    humidity = DHT.humidity;
    sendToGoogle();
    Serial.println("Connecting1...");
    if(client.connect(server,80)){
      sendData();
      signalflag = true;
    }else{
        Serial.println("Fail to Connect...");
    }
    while(signalflag){
        if(client.available()){
            char c = client.read();
            Serial.print(c);  
        }
        if(!client.connected()){
            Serial.println();
            Serial.println("Not Linking...");
            Serial.println("Temperature Sent:");
            Serial.println(temp); 
            Serial.println("Humidity Sent:");
            Serial.println(humidity); 
            client.stop();
            signalflag = false;
            data = "";         //reset data
        }
      }
      delay(5000);
}
void sendToGoogle(){
  data += "";
  //Devid got from your pushingbox
  //data += "GET //pushingbox?devid=your_dev_id&cascade_item="; 
  data += "GET //pushingbox?devid=v2AFC2C86E004F0A&tempData="; 
  data += temp; 
  data += "&humData=";
  data += humidity;
  data += " HTTP/1.1"; 
}
void sendData(){
  Serial.println("Connecting2...");
  client.println(data);
  client.println("Host: api.pushingbox.com");
  client.println("Connection: close");
  client.println();  
}
