#include <ESP8266WiFi.h>
#include <WiFiUdp.h>
#include "Wire.h"
#include "I2Cdev.h"
#include "MPU6050.h"
#include <Adafruit_NeoPixel.h>
//#include <NTPClient.h>

//Pixel definitions
#define PIXEL_PIN 15
#define NUMPIXELS 1

//device ID
int DEVICE_ID =0;

//SSID Put here your Wifi Network informations
const char* ssid = "Air DJ";  //SSID name HERE
const char* password = ""; //password HERE empty if no passwword used

//UDP parameters
const char * host = "10.0.0.186"; // Put HERE the IP adresse of the server
const uint16_t port = 2390;
//variables for acceleroscope/gyro/magneto
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;

MPU6050 accelgyro(0x69);
int lastAx=11000;
int lastAy=11000;
int lastAz=11000;
int az_threshold = 20000;
//counter for the timestamp
int timestamp;
WiFiUDP EthernetUdp;
unsigned int localPort = 2390;
unsigned int receivePort = 2389;

char packetBuffer[UDP_TX_PACKET_MAX_SIZE];

//sample rate in Hz
int sampleRateHz = 10;
//sample rate in ms
int sampleRateMs = 1000/sampleRateHz;
int delay_between_2_shakes = 500; //cannot shake more than 1 time evry 500ms

long int timer2;
long int timer1;
long int startTimer;
long int startingTime;
int fistCounter=0;
bool gameStarted=false;
bool verboseDebug=true;

Adafruit_NeoPixel pixels = Adafruit_NeoPixel(NUMPIXELS, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

void connect_wifi(){
  // WiFi.disconnect();
    if (WiFi.status() != WL_CONNECTED) { // FIX FOR USING 2.3.0 CORE (only .begin if not connected)
             WiFi.forceSleepWake();  
             WiFi.persistent(false);
             WiFi.setAutoConnect ( false );                                             // Autoconnect to last known Wifi on startup
             WiFi.setAutoReconnect ( false ) ; 
             //WiFi.config(ip, gateway, subnet);
             WiFi.mode(WIFI_STA);
             WiFi.begin(ssid, password); // connect to the network
             }
    while (WiFi.status() != WL_CONNECTED) {
        //pixels.setPixelColor(0, pixels.Color(125,0,0));
        //pixels.show();
        delay(500);
        Serial.print(".");
      //  pixels.setPixelColor(0, pixels.Color(0,0,0));
        //pixels.show();
        //delay(100);
       // Serial.print(WiFi.status());
       // WiFi.disconnect();
        //reconnect_trials++;
        //if(reconnect_trials>=10) reconnect_wifi();
        }
    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    timestamp=0;
}


void disconnect_wifi(){
   Serial.println("Closing Wifi");
   WiFi.mode(WIFI_OFF);
   WiFi.disconnect(); 
   WiFi.forceSleepBegin();
   delay(1);
   }

void Send_Data() {
     EthernetUdp.begin(localPort);
     char msg[30];
     //accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
     accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
     int test=sq(ay);//sq(ax)+sq(az)+
     int test2=sqrt(test);
     EthernetUdp.beginPacket(host, 2390);
     //sprintf(msg, "S: 1 2 3 a ");
     sprintf(msg, "S: %d %d %d %d",ax,ay,az,test2);
     //sprintf(msg, "S: %d %d %d %d",ax,ay,az,test2);
     EthernetUdp.write(msg);
     EthernetUdp.endPacket(); 
     }
void Send_Data(int thr) {
     EthernetUdp.begin(localPort);
     char msg[30];
     EthernetUdp.beginPacket(host, 2390);
     //sprintf(msg, "S: 1 2 3 a ");
     sprintf(msg, "S: %d ",thr);
     //sprintf(msg, "S: %d %d %d %d",ax,ay,az,test2);
     EthernetUdp.write(msg);
     EthernetUdp.endPacket();
     }
void Send_Data(int thr , int count) {
     EthernetUdp.begin(localPort);
     char msg[30];
     EthernetUdp.beginPacket(host, 2390);
     //sprintf(msg, "S: 1 2 3 a ");
     sprintf(msg, "%d %d ",thr,count);
     //sprintf(msg, "S: %d %d %d %d",ax,ay,az,test2);
     EthernetUdp.write(msg);
     EthernetUdp.endPacket();
     }
void Send_Data(int thr1 , int thr2,int thr3) {
     EthernetUdp.begin(localPort);
     char msg[30];
     EthernetUdp.beginPacket(host, 2390);
     //sprintf(msg, "S: 1 2 3 a ");
     sprintf(msg, "S: %d %d %d",thr1,thr2, thr3);
     //sprintf(msg, "S: %d %d %d %d",ax,ay,az,test2);
     EthernetUdp.write(msg);
     EthernetUdp.endPacket();
     }
void Send_Data(int thr1 , int thr2,int thr3,int thr4) {
     EthernetUdp.begin(localPort);
     char msg[30];
     EthernetUdp.beginPacket(host, 2390);
     //sprintf(msg, "S: 1 2 3 a ");
     sprintf(msg, "S: %d %d %d %d",thr1,thr2, thr3,thr4);
     //sprintf(msg, "S: %d %d %d %d",ax,ay,az,test2);
     EthernetUdp.write(msg);
     EthernetUdp.endPacket();
     }
  
void udpListener()
    {
    int packetSize = EthernetUdp.parsePacket();
    if(packetSize){
    // read the packet into packetBufffer
    EthernetUdp.read(packetBuffer, UDP_TX_PACKET_MAX_SIZE);
    /*Serial.println("Contents:");
    Serial.println(packetBuffer[0]);*/
    if(packetBuffer[0]=='S')
        {
        gameStarted=true; 
        startingTime=millis();
        pixels.setPixelColor(0, pixels.Color(0,0,125));
        pixels.show();
        Serial.println("received data");
        }
     else if(packetBuffer[0]=='X')
        {
        gameStarted=false; 
        pixels.setPixelColor(0, pixels.Color(0,125,0));
        pixels.show();
        }   
     else Serial.println("error receiving UDP");
     }
    }

void setup() {
    Serial.begin(115200);
    pixels.begin(); // This initializes the NeoPixel library.
    pixels.setPixelColor(0, pixels.Color(125,0,0));
    pixels.show();
    delay(10);
    WiFi.mode(WIFI_OFF); 
    WiFi.persistent(false);
    WiFi.setAutoConnect ( false );  
    WiFi.setAutoReconnect ( false ); 
    Wire.begin();
    //configure adc foor battery level monitoring
    pinMode(A0, INPUT);
    delay(200);
    //blue built-in led
    pinMode(2, OUTPUT);
    // initialize the pushbutton pin as an input:
    // pinMode(buttonPin, INPUT_PULLUP);
    // always use this to "mount" the filesystem
    //init accelerometer & gyro
    Serial.println("Initializing I2C devices...");
    accelgyro.initialize();
    //signal that everything is ok by blinking the led
    delay(500);
    connect_wifi();
    pixels.setPixelColor(0, pixels.Color(0,125,0));
    pixels.show();
    //digitalWrite(2,LOW);
    delay(100);
    startTimer=millis();
    timer1=millis(); 
    timer2=millis();
    //digitalWrite(2,HIGH);
    //Starting UDP listener
    EthernetUdp.begin(receivePort);
    }

void loop() {
  udpListener();
  timer1=millis(); 
  //for sampling rate 
  if(timer1-startTimer>(1000/sampleRateHz))
      {
      startTimer=timer1; 
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      if(az>az_threshold )
        {
        if(millis()-timer2>delay_between_2_shakes)
          {
          if(verboseDebug) Serial.println("Sending data !");
          Send_Data(DEVICE_ID, millis());
          timer2=millis();
          }
        }
       } 
  }
