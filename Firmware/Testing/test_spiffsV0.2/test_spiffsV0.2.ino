#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include "Wire.h"


// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
int opMode=0;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
MPU6050 accelgyro;
File fw;
void read_memory(){
  // nothing to do for now, this is just a simple test
  File f = SPIFFS.open("/f.txt", "r");
  // we could open the file
  while(f.available()) {
      //Lets read line by line from the file
      String line = f.readStringUntil('n');
      Serial.println(line);
    }
   f.close();
}
void write_data(){
   fw = SPIFFS.open("/f.txt", "w+");
}
void setup() {
  Serial.begin(115200);
  Wire.begin();
  delay(2000);
  // always use this to "mount" the filesystem
  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);
  // this opens the file "f.txt" in read-mode
  File f = SPIFFS.open("/f.txt", "r");
 //if file does not exist
  if (!f) {
    Serial.println("File doesn't exist yet. Creating it");
    // open the file in write mode
    File f = SPIFFS.open("/f.txt", "w");
    if (!f) {
      Serial.println("file creation failed");
    }
    // now write two lines in key/value style with  end-of-line characters
    Serial.println("Writing in file");
    f.println("ssid=abc");
    f.println("password=123455secret");
  } 
  //if file exist
  else {
    // we could open the file
    while(f.available()) {
      //Lets read line by line from the file
      String line = f.readStringUntil('n');
      Serial.println(line);
    }
  }
f.close();
//init MPU
Serial.println("Initializing I2C devices...");
accelgyro.initialize();
}

void loop() {
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    Serial.println(inByte);
    if(inByte=='p') {
      read_memory();
    }
    //recording mode
    else if(inByte=='r') {
       fw = SPIFFS.open("/f.txt", "w+");
       opMode=2;
      //read_memory();
    }
     //stop recording mode
    else if(inByte=='R') {
       fw.close();
       opMode=0;
      //read_memory();
    }
    //live mode
    else if(inByte=='l'){
      opMode=1;
    }
    //quit live mode
    else if(inByte=='L'){
      opMode=0;
    }
    else delay(2);
  }
  else delay(1);
  if(opMode==1){
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Serial.println(ax);
  }
  else if(opMode==2){
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    Serial.println("recording");
    fw.println(ax);
  }
  else delay(2);
  //read_memory();
  //delay(100);
}


