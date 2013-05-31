

/*
 Basic MQTT example 
 
 - connects to an MQTT server
 - publishes "hello world" to the topic "outTopic"
 - subscribes to the topic "inTopic"
 */

#include <SPI.h>
#include <WiFi.h>
#include <PubSubClient.h>

char ssid[] = "";     //  your network SSID (name) 
char pass[] = "";  // your network password
int status = WL_IDLE_STATUS;     // the Wifi radio's status
int MIC_PIN = 0;
int LAST_MIC_VALUE = 0;
int CLIENT_ID_COUNT; 
char TOPIC[] = "audio";
char audioData[20];
int loopCount = 1;

// Update these with values suitable for your network.

byte mqtt_server[] = {
  192,168,1,78};
int mqtt_port = 1883;

void callback(char* topic, byte* payload, unsigned int length) {
  Serial.print("Callback Message from topic ");
  Serial.println(topic);
}

WiFiClient wifiClient;

PubSubClient client(mqtt_server,mqtt_port,callback,wifiClient);


void setup()
{
  Serial.begin(9600);
  delay(500);
  Serial.println("Booting ...");
  // check for the presence of the shield:
  if (WiFi.status() == WL_NO_SHIELD) {
    Serial.println("WiFi shield not present"); 
    // don't continue:
    while(true);  
  } 
  Serial.println("WiFi shield active - attempting to connect ...");

  // attempt to connect to Wifi network:
  while ( status != WL_CONNECTED) { 
    Serial.print("Attempting to connect to WPA SSID: ");
    Serial.println(ssid);
    // Connect to WPA/WPA2 network:    
    status = WiFi.begin(ssid, pass);

    // wait 10 seconds for connection:
    delay(10000);
  }
  Serial.print("WiFi shield connected to WiFi network " );
  Serial.println(ssid);
  printWifiData();
  printCurrentNet();


}

void printWifiData() {
  // print your WiFi shield's IP address:
  IPAddress ip = WiFi.localIP();
  Serial.print("IP Address: ");
  Serial.println(ip);
  Serial.println(ip);

  // print your MAC address:
  byte mac[6];  
  WiFi.macAddress(mac);
  Serial.print("MAC address: ");
  Serial.print(mac[5],HEX);
  Serial.print(":");
  Serial.print(mac[4],HEX);
  Serial.print(":");
  Serial.print(mac[3],HEX);
  Serial.print(":");
  Serial.print(mac[2],HEX);
  Serial.print(":");
  Serial.print(mac[1],HEX);
  Serial.print(":");
  Serial.println(mac[0],HEX);
}

void printCurrentNet() {
  // print the SSID of the network you're attached to:
  Serial.print("SSID: ");
  Serial.println(WiFi.SSID());

  // print the MAC address of the router you're attached to:
  byte bssid[6];
  WiFi.BSSID(bssid);    
  Serial.print("BSSID: ");
  Serial.print(bssid[5],HEX);
  Serial.print(":");
  Serial.print(bssid[4],HEX);
  Serial.print(":");
  Serial.print(bssid[3],HEX);
  Serial.print(":");
  Serial.print(bssid[2],HEX);
  Serial.print(":");
  Serial.print(bssid[1],HEX);
  Serial.print(":");
  Serial.println(bssid[0],HEX);

  // print the received signal strength:
  long rssi = WiFi.RSSI();
  Serial.print("signal strength (RSSI):");
  Serial.println(rssi);

  // print the encryption type:
  byte encryption = WiFi.encryptionType();
  Serial.print("Encryption Type:");
  Serial.println(encryption,HEX);
  Serial.println();
  mqttConnect();
}

void mqttConnect(){
  while (!client.connected()) {
    String str = "ArduinoMQTTDemo";
    String clientID = str + CLIENT_ID_COUNT;
    int strlen = clientID.length();
    char clientData[ strlen+1];
    clientID.toCharArray(clientData,strlen+1);
    if (client.connect(clientData,"admin","admin") == false) {
      Serial.println("MQTT Could not connect ... ");
      delay(1000);
    }
    else {
      client.subscribe("inTopic");
      Serial.println("MQTT Connected");
    }
    CLIENT_ID_COUNT++;
  }
}

int getMicValue(){
  int micValue = analogRead(MIC_PIN);
  micValue = constrain(abs(micValue - 512)-25,0,512);
  return micValue;
}

void loop()
{
  //Serial.println("looping ...");
  mqttConnect();
  //Serial.println("mqttConnect complete");
  int micValue = getMicValue();
  if (micValue != LAST_MIC_VALUE && micValue != 0){
    String str = "";
    String payload = str + micValue;
    //send to MQTT_BROKER

    int dataLength = payload.length() + 1;
    payload.toCharArray(audioData,dataLength);
    client.publish(TOPIC,(uint8_t*)audioData,dataLength);
  }
  LAST_MIC_VALUE = micValue;

  if (loopCount%100==0){
  if(!client.loop()){
    Serial.println("PubSubClient loop failed");
  }
  }
  loopCount++;
}




