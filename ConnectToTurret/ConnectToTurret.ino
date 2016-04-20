#include <WiFi.h>
#include <WiFiUDP.h>
#include <string.h>

#define LED_PIN 42
#define PORT 2512

char ssid[] = "cuddles";
char pass[] = "subaruwrx";

int status = WL_IDLE_STATUS;
char packetBuffer[256];

WiFiUDP udp;

const int THRESH = 3;

int count = 1;

// Asserted IP address
IPAddress ip(10, 42, 0, 34);
//WiFiServer server(PORT);
bool isConnected = false;
char buff[255];

void setup() {
  // initialize serial:
  Serial.begin(9600);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.config(ip);
  
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(ssid, pass);

  // if you're not connected, stop here:
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } 
  // if you are connected, print out info about the connection:
  else {
    Serial.println("Connected to network");
    digitalWrite(42, HIGH);
   int a = WiFi.getSocket();
   Serial.print("Socket: ");
   Serial.println(a);
   Serial.print("IP Address: ");
   Serial.println(WiFi.localIP());
   Serial.print("Wifi Signal Stringth: ");
   Serial.print(WiFi.RSSI());
   Serial.println(" dBm");
  }
  
  Serial.println("\nStarting connection to server");
  udp.begin(PORT);
}

void loop() {
  int packetSize = udp.parsePacket();
  if(packetSize) {
    // read the packet into packetBuffer
    int len = udp.read(packetBuffer, 256);
    if(len > 0) {
      packetBuffer[len] = 0;
    }
    Serial.println(packetBuffer);
  }
}






