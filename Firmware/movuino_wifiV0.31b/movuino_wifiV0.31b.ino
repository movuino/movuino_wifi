//V0.3 added UDP
//0.31b testing write on memory until no battery or no space

#include "FS.h"
#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>
#include <WiFiUdp.h>
#include "Wire.h"


// I2Cdev and MPU6050 must be installed as libraries, or else the .cpp/.h files
// for both classes must be in the include path of your project
#include "I2Cdev.h"
#include "MPU6050.h"
int opMode = 0;
long int timer0;
long int timer1;
long int startTimer;
long int sampleNb;
int16_t ax, ay, az;
int16_t gx, gy, gz;
int16_t mx, my, mz;
MPU6050 accelgyro;
//Led and buttons
const int buttonPin = 13;
//variables for the debounce
int buttonState = 0;  // variable for reading the pushbutton status
long lastButtonTime = 0;
int debounceDelay = 400;
//wifi +UDP variables
ESP8266WiFiMulti WiFiMulti;
WiFiClient client;
const uint16_t port = 1000;
const char * host = "192.168.43.38"; // ip or dns
char packetBuffer[255]; //buffer to hold incoming packet
char  ReplyBuffer[] = "S: 1 2 3 4 1254 1245 1234";       // a string to send back
unsigned int localPort = 2390;      // local port to listen on
WiFiUDP Udp;

File fw;
void memory_info(){
    FSInfo fs_info;
    SPIFFS.info(fs_info);
    Serial.print("totalBytes :");
    Serial.println(fs_info.totalBytes);
    Serial.print("usedBytes :");
    Serial.println(fs_info.usedBytes);
}
void list_files(){
    Dir dir = SPIFFS.openDir("/data");
    while (dir.next()) {
       Serial.print(dir.fileName());
       File f = dir.openFile("r");
       Serial.println(f.size());
      }
}
void read_memory() {
  // nothing to do for now, this is just a simple test
  File f = SPIFFS.open("/data/1.txt", "r");
  // we could open the file
  while (f.available()) {
    //Lets read line by line from the file
    String line = f.readStringUntil('\n');
    Serial.println(line);
    delay(5);
   // yield();
  }
  f.close();
}
void Connect_Wifi() {
  //WiFiMulti.addAP("MotoG3", "z12345678");
  WiFi.persistent(false);
  WiFi.mode(WIFI_OFF);
  WiFi.mode(WIFI_STA);
  //WiFi.begin("Ke20 iPhone", "z12345678");
  // WiFiMulti.addAP("MotoG3", "z12345678");
  delay(100);
  while (WiFiMulti.run() != WL_CONNECTED) {
    Serial.println("connecting ..");
    //WiFi.begin("MotoG3", "z12345678");
    WiFiMulti.addAP("MotoG3", "z12345678");
    delay(500);
  }
  Serial.println("");
  Serial.println("WiFi connected");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
  delay(50);
  //WiFi.mode(WIFI_OFF);
}
void Send_Data() {
  Udp.begin(localPort);
  char msg[30];
  //accelgyro.getMotion9(&ax, &ay, &az, &gx, &gy, &gz, &mx, &my, &mz);
  Udp.beginPacket(host, 2390);
  sprintf(msg, "S: 1 2 3 a ");
  Udp.write(msg);
  Udp.endPacket();
  // if(packetNumber<255) packetNumber ++;
  // else packetNumber=0;
  //sprintf(msg, "S: 1 2 3 a %d %d %d",ax,ay,az);
  //sprintf(msg, "S: %d 2 3 a %d %d %d",packetNumber,ax,ay,az);
  //sprintf(msg, "S: %d 2 3 a %d 123 123",packetNumber,packetNumber);
  //sprintf(msg, "S: %d 2 3 a 123 123 123",packetNumber);
  // Serial.println(ax);
  /* Udp.write(msg);
    Udp.endPacket();
    time0=micros();*/
}
void Send_Data_From_File() {
  Udp.begin(localPort);
  char msg[45];
  char msg2[40];
  // nothing to do for now, this is just a simple test
  File f = SPIFFS.open("/data/1.txt", "r");
  int lineNb = 0;
  String toto = "S: 12345678lklkmlkmlkkmlkmlkmlk";
  // we could open the file
  while (f.available()) {
    //Lets read line by line from the file
    String line = f.readStringUntil('\n');
    Udp.beginPacket(host, 2390);
    //attention ceette ligne doit etre absolumrnt après udp bediginpacket
    line.toCharArray(msg, line.length());
    //sprintf(msg2, "S: 1 2 3 a ");
    // toto.toCharArray(msg2, 20);
    Udp.write(msg);
    Udp.endPacket();
    Serial.print(lineNb);
    Serial.print("length :");
    Serial.print(line.length());
    Serial.print("msg:");
    Serial.println(msg);
    delay(10);
    lineNb++;
  }
  f.close();
  Serial.println("finish sending");
}

void setup() {
  Serial.begin(115200);
  Wire.begin();
  // We start by connecting to a WiFi network
  WiFiMulti.addAP("MotoG3", "z12345678");
  //configure adc foor battery level monitoring
  pinMode(A0, INPUT);
  delay(2000);
  //blue built-in led
  pinMode(2, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  // always use this to "mount" the filesystem
  bool result = SPIFFS.begin();
  //turn off led
  digitalWrite(2, HIGH);
  Serial.println("SPIFFS opened: " + result);
  // this opens the file "f.txt" in read-mode
  //  File f = SPIFFS.open("/f.txt", "r");
  //if file does not exist
  /*  if (!f) {
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
    f.close();*/
  //init MPU
  Serial.println("Initializing I2C devices...");
  accelgyro.initialize();
}

void loop() {
  //Read button
  buttonState = digitalRead(buttonPin);
  if (!buttonState) {
    if (millis() - lastButtonTime > debounceDelay) {
      lastButtonTime = millis();
      //Serial.println("Recording...");
      //if we are recording stop recording
      if (opMode == 2) {
        //TODO fonction start_recording & stop_recording
        fw.close();
        Serial.println("Stop recording");
        digitalWrite(2, HIGH);
        opMode = 0;
      }
      else {
        Serial.println("Recording");
        fw = SPIFFS.open("/data/1.txt", "w+");
        opMode = 2;
        digitalWrite(2, LOW);
      }
    }
  }
  timer0 = millis();
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    //Serial.println(inByte);
    //Ping
    if (inByte == '?') {
      Serial.println("?");
    }
    //create dir + files
    else if (inByte == 'c') {
      Serial.println("Create file");

      File f = SPIFFS.open("/data/1.txt", "r");
      if (!f) {
        Serial.println("File doesn't exist yet. Creating it");
        File f = SPIFFS.open("/data/1.txt", "w");
        if (!f) {
          Serial.println("file creation failed");
          //put error code here
        }
        // now write two lines in key/value style with  end-of-line characters
        Serial.println("Writing in file");
        f.println("ssid=abc");
        f.println("password=123455secret");
        f.close();
      }
      else Serial.println("File exist!");
    }
    //dir
    else if (inByte == 'd') {
      Serial.println("Listing dir :");
      digitalWrite(2, LOW);
      Dir dir = SPIFFS.openDir("/data");
      while (dir.next()) {
        Serial.print(dir.fileName());
        File f = dir.openFile("r");
        Serial.print(" ");
        Serial.println(f.size());
      }
      Serial.println("End of listing");
      digitalWrite(2, HIGH);
    }
    //read
    else if (inByte == 'p') {
      Serial.println("Read");
      read_memory();
      Serial.println("End of Read");
    }
    //recording mode
    else if (inByte == 'r') {
      Serial.println("Recording");
      fw = SPIFFS.open("/data/1.txt", "w+");
      startTimer=millis(); 
      opMode = 2;
      //digitalWrite(2, LOW);
    }
    //stop recording mode
    else if (inByte == 'R') {
      fw.close();
      //reset sample nb
      sampleNb=0;
      Serial.println("Stop recording");
      digitalWrite(2, HIGH);
      opMode = 0;
      //read_memory();
    }
    //delete file
    else if (inByte == 'P') {
      Serial.println("Deleting file");
      SPIFFS.remove("/data/1.txt");
      Serial.println("deletion");
      opMode = 0;
      //read_memory();
    }
    //sending data via udp
    else if (inByte == 's') {
      Serial.println("Connecting to Wifi");
      Connect_Wifi();
     // Serial.println("Sending data ");
     // Send_Data();
     // Send_Data_From_File();
    }
    else if (inByte == 'S') {
      Serial.println("Turning off wifi");
      WiFi.mode(WIFI_OFF);
      WiFi.disconnect();
    }
    //live mode
    else if (inByte == 'l') {
      Serial.println("Start live");
      opMode = 1; //old 3
    }
    //quit live mode
    else if (inByte == 'L') {
      opMode = 0;
    }
    //scan networks
    else if (inByte == 'x') {
      Serial.println("Scanning networks");
      int n = WiFi.scanNetworks();
      Serial.println("scan done");
      if (n == 0)
        Serial.println("no networks found");
      else
      {
        Serial.print(n);
        Serial.println(" networks found");
        for (int i = 0; i < n; ++i)
        {
          // Print SSID and RSSI for each network found
          Serial.print(i + 1);
          Serial.print(": ");
          Serial.print(WiFi.SSID(i));
          Serial.print(" (");
          Serial.print(WiFi.RSSI(i));
          Serial.print(")");
          Serial.println((WiFi.encryptionType(i) == ENC_TYPE_NONE) ? " " : "*");
          delay(10);
        }
      }
      Serial.println("");
      // Wait a bit before scanning again
      delay(5000);
    }
    //send data live via udp 
    else if(inByte=='u'){
       Serial.println("Start UDP");
       Connect_Wifi(); 
       startTimer=millis(); 
       opMode = 3;
    }
    else if(inByte=='U'){
      Serial.println("Stop UDP");
      opMode = 0;
      sampleNb=0;
    }
    else if(inByte=='i'){
      Serial.println("Files info : ");
      list_files();
      memory_info();
    }
    else delay(2);
  }
  else delay(1);
  //print to serial
  if (opMode == 1) {
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int sensorValue = analogRead(A0);
    Serial.print(timer0);
    Serial.print(" ");
    Serial.print(ax);
    Serial.print(" ");
    Serial.print(ay);
    Serial.print(" ");
    Serial.print(az);
    Serial.print(" ");
    Serial.print(gx);
    Serial.print(" ");
    Serial.print(gy);
    Serial.print(" ");
    Serial.print(gz);
    Serial.print(" ");
    Serial.println(sensorValue);
    //delay(2);
  }
  //print in file
  else if (opMode == 2) {
   timer1=millis(); 
    if(timer1-startTimer>1000) {
    startTimer=timer1;  
    accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int sensorValue = analogRead(A0);
    //fw.println("toto");
   // fw.print("S: ");
       //stop recording if <3.3v or more than 1 hour
    if(sensorValue<600 /*|| sampleNb>9000*/) {
       fw.close();
       opMode=0;
       Serial.print("Stop recording");
       digitalWrite(2, HIGH);
      }
      else{
    fw.print(sampleNb);
    fw.print(" ");
    /*fw.print(ax);
    fw.print(" ");
    fw.print(ay);
    fw.print(" ");
    fw.print(az);
    fw.print(" ");
    fw.print(gx);
    fw.print(" ");
    fw.print(gy);
    fw.print(" ");
    fw.print(gz);
    fw.print(" ");*/
    fw.println(sensorValue);
    sampleNb++;
      }
    }
    else delay(1);
    //delay(2);
   
  }
  else if(opMode == 3){
    timer1=millis(); 
    if(timer1-startTimer>100) {
      sampleNb++;
      startTimer=timer1;
      //get data
      accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
      //int sensorValue = analogRead(A0);
      //send udp data
      Udp.begin(localPort);
      char msg[30];
      Udp.beginPacket(host, 2390);
     // sprintf(msg, "S: %d 2 3 a %d",ax,ay);
      sprintf(msg, "%d %d %d",ax,ay,az);
      Udp.write(msg);
      Udp.endPacket();   
      Serial.println(ax);
      Serial.println(ay);
      Serial.println(az);
      }
    else delay(2);
    /*  accelgyro.getMotion6(&ax, &ay, &az, &gx, &gy, &gz);
    int sensorValue = analogRead(A0);*/
  }
  else delay(2);
  //read_memory();
  //delay(100);
}


