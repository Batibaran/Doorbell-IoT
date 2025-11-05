#include <ESP8266WiFi.h>
#include <WiFiUdp.h>

const char* ssid     = "********";
const char* password = "********";

WiFiUDP Udp;
unsigned int localUdpPort = 12345;
char incomingPacket[255];

const int SW3_CTRL = D1;
const int LED_PIN = LED_BUILTIN;
const unsigned long PRESS_TIME = 800;
const unsigned long HEARTBEAT_INTERVAL = 20000;

unsigned long lastHeartbeat = 0;

void setup() {
  Serial.begin(115200);
  delay(100);

  pinMode(SW3_CTRL, OUTPUT);
  digitalWrite(SW3_CTRL, LOW);

  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, HIGH);

  Serial.println();
  Serial.println("Connecting to WiFi...");
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);

  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println();
  Serial.print("Connected! IP address: ");
  Serial.println(WiFi.localIP());

  Udp.begin(localUdpPort);
  Serial.print("Listening for UDP broadcast on port ");
  Serial.println(localUdpPort);
}

void loop() {
  unsigned long currentMillis = millis();
  
  if (currentMillis - lastHeartbeat >= HEARTBEAT_INTERVAL) {
    heartbeatFlash();
    lastHeartbeat = currentMillis;
  }
  
  int packetSize = Udp.parsePacket();
  if (packetSize) {
    int len = Udp.read(incomingPacket, sizeof(incomingPacket) - 1);
    if (len > 0) incomingPacket[len] = 0;

    Serial.print("Received packet: ");
    Serial.println(incomingPacket);

    if (strcasecmp(incomingPacket, "unlock") == 0) {
      pressDoorbell();
    }
  }

  delay(10);
}

void pressDoorbell() {
  Serial.println("Unlock command received → Pressing door 2 times...");
  
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED_PIN, LOW);
    digitalWrite(SW3_CTRL, HIGH);
    delay(PRESS_TIME);
    digitalWrite(SW3_CTRL, LOW);
    digitalWrite(LED_PIN, HIGH);
    
    Serial.print("Press ");
    Serial.print(i + 1);
    Serial.println(" complete.");
    
    if (i < 1) {
      delay(3000);
    }
  }
  
  Serial.println("All presses complete.");
}

void heartbeatFlash() {
  digitalWrite(LED_PIN, LOW);
  delay(50);
  digitalWrite(LED_PIN, HIGH);
}

