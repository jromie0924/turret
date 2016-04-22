// Arduino Libraries
#include <WiFi.h>
#include <WiFiUDP.h>
#include <Servo.h>

// C
#include <string.h>
#include <stdio.h>

#define LED_PIN 42
#define LASER 8
#define SERVO_PIN 9
#define PORT 2512

//-----------GLOBALS----------------
char ssid[] = "cuddles";
char pass[] = "subaruwrx";
char packetBuffer[256];
const int THRESH = 3;
int count = 1;
WiFiUDP udp;
int status = WL_IDLE_STATUS;
// Asserted IP address
IPAddress ip(10, 42, 0, 34);
//WiFiServer server(PORT);
bool isConnected = false;
char buff[255];
Servo servo;
//----------------------------------

void setup() {
  // initialize serial:
  Serial.begin(9600);

  servo.attach(SERVO_PIN);
  servo.write(90);

  pinMode(LASER, OUTPUT);
  digitalWrite(LASER, HIGH);
  
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  WiFi.config(ip);
  
  // attempt to connect using WPA2 encryption:
  Serial.println("Attempting to connect to WPA network...");
  status = WiFi.begin(ssid, pass);
  
  if ( status != WL_CONNECTED) { 
    Serial.println("Couldn't get a wifi connection");
    while(true);
  } else {
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
    //Serial.println(packetBuffer);
    char* coord;
    coord = strtok(packetBuffer, ",");
    int y = atoi(coord);
    coord = strtok(NULL, ",");
    int x = atoi(coord);
    if(x != 429496729) {
      servo.write(x);
    }
    delay(20);
  }
}






