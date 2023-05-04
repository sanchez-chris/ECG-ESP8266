#include <WiFiUdp.h>
#include "WiFi.h"

#define TimeOUT 30

int UdpPort;
WiFiUDP Udp;
double IP;
unsigned int localUdpPort = 4210;

void WLAN_connection() {
  
  int i=0; // for the count of secons to connect
  Serial.begin(115200);  // Start the Serial communication to send messages to the computer
  delay(10);
  Serial.println('\n');
  WiFi.begin("FRITZ!Box 7590 VL", "56616967766283031728");   // Connect to the lab network
  // WiFi.begin("Henry&Co", "Henry4Life$$$"); // Connect to the Fernandos network
  // WiFi.begin("Redmi Note 9S", "11111111");
 

  while (WiFi.status() != WL_CONNECTED) { // Wait for the Wi-Fi to connect
  delay(1000);
  Serial.print(++i); 
  Serial.print(' ');
  if ( i >= TimeOUT) {   // Timeout, wenn Verbindung nicht hergestellt werden kann
    Serial.print("Connection to Wifi-Network failed for 30 seconds ");
    i = 0;
    }
  }

  char str[60] = {0};
  Serial.println('\n');
  Serial.println("Connection established!");  
  Serial.print("IP address:\t");
  Serial.println(WiFi.localIP());    // Send the IP address of the ESP8266 to the computer

  Udp.begin(localUdpPort);
  Serial.printf("Now listening at IP %s, UDP port %d\n", WiFi.localIP().toString().c_str(), localUdpPort);
  sprintf(str, "UDP port: %d" ,localUdpPort);
}
