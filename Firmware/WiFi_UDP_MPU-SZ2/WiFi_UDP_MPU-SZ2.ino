/*
 *  This sketch sends a message to a TCP server
 *
 */

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include "Wire.h"

// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"

ESP8266WiFiMulti WiFiMulti;
int connected2 =0;
WiFiClient client;
const uint16_t port = 1000;
const char * host = "192.168.43.168"; // ip or dns
char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "S: 1 2 3 4 1254 1245 1234";       // a string to send back
unsigned int localPort = 2390;      // local port to listen on
int packetNumber=0;
MPU6050 accelgyro;
unsigned int time0=micros();
unsigned int time1=micros();
int samplingDelay = 50000; //20ms =50 hz
int counter =0;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

WiFiUDP Udp;

void setup() {
  pinMode(0,OUTPUT);
   Wire.begin();
    Serial.begin(115200);
    delay(10);
// initialize device
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    // We start by connecting to a WiFi network
     WiFiMulti.addAP("MotoG3", "z12345678");

    Serial.println();
    Serial.println();
    Serial.print("Wait for WiFi... ");

    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(50);
    Udp.begin(localPort);
    delay(50);
}


void loop() {
    if(counter>=10) 
     {
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    counter=0;
    }
    else counter++;
    //Udp.beginPacket(host,2390);
    //char msg[30];
    // Serial.print(ax);
    time1=micros();
    unsigned int time2=time1-time0;
    if(time2>=1)
   // if(time2>=(samplingDelay-28000))
         {
          // if(counter>=10) 
     //{
    accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
    //counter=0;
    //}
    //else counter++;
         char msg[30];
         //accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
         Udp.beginPacket(host,2390);
         digitalWrite(0,LOW);
         if(packetNumber<255) packetNumber ++;
         else packetNumber=0;
         //sprintf(msg, "S: 1 2 3 a %d %d %d",ax,ay,az);
         sprintf(msg, "S: %d 2 3 a %d %d %d",packetNumber,ax,ay,az);
         //sprintf(msg, "S: %d 2 3 a %d 123 123",packetNumber,packetNumber);
         //sprintf(msg, "S: %d 2 3 a 123 123 123",packetNumber);
         Serial.println(ax);
           Udp.write(msg);
           Udp.endPacket();
         time0=micros();
         delay(5);
         }
   // else  digitalWrite(0,HIGH);
}

