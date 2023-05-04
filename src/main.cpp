#include <Arduino.h>
#include <SSD1306Wire.h>
#include "WiFi.h"
#include <Wire.h>
#include "WLAN_connection.h"
#include "einlesen.h"
#include "display.h"

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define SEND_SIZE 250 // how big is the packet that we are sending to matlab
#define TIMER_INTERVAL_S   1000000 // 1sec == 1.000.000 us
#define TIMER_INTERVAL_ms   1000  // 1sec == 1.000 ms
#define Interruptfrequency   200  //Interrupt frequenz
#define Interrupt_TIME_micsec  (int) TIMER_INTERVAL_S*1.0/Interruptfrequency //Interrupt frequenz for Timer
#define Interrupt_TIME_micsec  (int) TIMER_INTERVAL_S*1.0/Interruptfrequency //Interrupt frequenz for Timer

using namespace std;

// global variables
const int ADC_PIN = 34;
volatile int i = 0;
volatile bool disp =false;
hw_timer_t *AnalogTimer = NULL;
string Password_WiFi = "56616967766283031728";
string Name_WiFi = "FRITZ!Box 7590 VL";
char incomingPacket[256];
unsigned long currentMillis = 0;
bool matlab_start;
int output;
int lastMillis;
bool sendSuccess1 = false;
bool sendSuccess2 = false;
bool sendSuccess3 = false;
bool sendSuccess4 = false;
bool sentPart1ofBufferAndFiltered = false;
bool sentPart2ofBufferAndFiltered = false;
bool sentPart3ofBufferAndFiltered = false;


void setup() {

  //fill_triangule_array(); // create test signal
  display.init();
  display.drawString(0, 0, "Start");

  display.display();
  delay(200);

  WLAN_connection();
  Serial.begin(115200); // baudrate 115200
  delay(2000);
  // Init OLED
  display.init();

  // Configure the interrupt
  AnalogTimer = timerBegin(0, 40, true);
  timerAttachInterrupt(AnalogTimer, &Einlesen, true);
  timerAlarmWrite(AnalogTimer, 8000, true); // 4 ms
  timerAlarmEnable(AnalogTimer);

  Serial.println("setup success: start matlab");
}

void loop() {

  int packetSize = Udp.parsePacket();

  if (packetSize) {
    // receive incoming UDP packets
    Serial.printf("Received %d bytes from %s, port %d\n", packetSize, Udp.remoteIP().toString().c_str(), Udp.remotePort());
    int len = Udp.read(incomingPacket, 255);

    if (len > 0) {
      incomingPacket[len] = 0;
      matlab_start = true;
      Serial.println("Matlab starting");
    }
    Serial.printf("UDP packet contents: %s\n", incomingPacket);

    if (matlab_start==true) {

      do {

        if(flag1){ // when the first part of raw buffer is full
          
          Serial.println("start sending Part 1 of buffer raw ");
          for (int i = 0; i<=14; i++) { //loop 0 ... loop 14, so 15 loops
            // Send first part of bufferFiltered
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); //initializes the packet of data 
            Udp.write((uint8_t*) &bufferRawAndFiltered[i*250], SEND_SIZE*2); //writes the message with data filtered
            sendSuccess1 = Udp.endPacket(); //send message

            if (sendSuccess1) {
              Serial.printf("     sending Packet %d \n", i);
            }
            if (!sendSuccess1) {
              Serial.printf("     ERROR sending Packet %d \n", i);
            }
            delay(100); // if there is no delay: [27031][E][WiFiUdp.cpp:185] endPacket(): could not send data: 12
          }

          Serial.println("end sending Part 1 of buffer raw");
          flag1 = false;
          Serial.println("flag 1 = false");
          sentPart1ofBufferAndFiltered = true;
          Serial.println("sentPart1ofBufferAndFiltered = true");          
        }

        if(flag2 && sentPart1ofBufferAndFiltered) { // when the second part of the raw buffer is full and the first part was already sent
        
          Serial.println("start sending Part 2 of buffer Raw");

          for (int i = 15; i<=29; i++) { // loop 15 ... loop 29, s0 15 loops
            // Send second part of buffer raw
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); //initializes the packet of data
            Udp.write((uint8_t*) &bufferRawAndFiltered[i*250], SEND_SIZE*2); //writes the message with data filtered
            sendSuccess2 = Udp.endPacket(); //send message

            if (sendSuccess2) {
              Serial.printf("     sending Packet %d \n", i);
            }
            if (!sendSuccess2) {
              Serial.printf("     ERROR sending Packet %d \n", i);
            }
            delay(100); // if there is no delay: [27031][E][WiFiUdp.cpp:185] endPacket(): could not send data: 12
          }

          Serial.println("end sending Part 2 of buffer raw");
          flag2 = false;
          Serial.println("flag 2 = false");
          sentPart2ofBufferAndFiltered = true;
          Serial.println("sentPart2ofBufferAndFiltered = true");
        }   
        if(flag3 && sentPart2ofBufferAndFiltered) { // when the third part of the raw buffer is full and the second part was already sent
        
          Serial.println("start sending Part 3 of buffer Raw and Filtered");

          for (int i = 30; i<=44; i++) { // loop 15 ... loop 29, s0 15 loops
            // Send second part of buffer raw
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); //initializes the packet of data
            Udp.write((uint8_t*) &bufferRawAndFiltered[i*250], SEND_SIZE*2); //writes the message with data filtered
            sendSuccess3 = Udp.endPacket(); //send message

            if (sendSuccess3) {
              Serial.printf("     sending Packet %d \n", i);
            }
            if (!sendSuccess3) {
              Serial.printf("     ERROR sending Packet %d \n", i);
            }
            delay(100); // if there is no delay: [27031][E][WiFiUdp.cpp:185] endPacket(): could not send data: 12
          }

          Serial.println("end sending Part 3 of buffer raw and filtered");
          flag3 = false;
          Serial.println("flag 3 = false");
          sentPart3ofBufferAndFiltered = true; 
          Serial.println("sentPart3ofBufferAndFiltered = true");
        }   
        if(flag4 && sentPart3ofBufferAndFiltered) { // when the forth half of the raw buffer is full and the third part was already sent
        
          Serial.println("start sending Part 4 of buffer Raw and Filtered");

          for (int i = 45; i<=59; i++) { // loop 45 ... loop 59, s0 15 loops
            // Send forth part of buffer raw
            Udp.beginPacket(Udp.remoteIP(), Udp.remotePort()); //initializes the packet of data
            Udp.write((uint8_t*) &bufferRawAndFiltered[i*250], SEND_SIZE*2); //writes the message with data filtered
            sendSuccess4 = Udp.endPacket(); //send message

            if (sendSuccess4) {
              Serial.printf("     sending Packet %d \n", i);
            }
            if (!sendSuccess4) {
              Serial.printf("     ERROR sending Packet %d \n", i);
            }
            delay(100); // if there is no delay: [27031][E][WiFiUdp.cpp:185] endPacket(): could not send data: 12
          }

          Serial.println("end sending Part 4 of buffer raw and filtered");
          flag4 = false;
          Serial.println("flag 4 = false");
        } 
      } while(!sentPart1ofBufferAndFiltered || !sentPart2ofBufferAndFiltered || !sentPart3ofBufferAndFiltered);
    }
  }
  //OLED Display update
  currentMillis = millis();
  delay(15); // in msec.
  update_OLED();  // output on display
  return;
}






