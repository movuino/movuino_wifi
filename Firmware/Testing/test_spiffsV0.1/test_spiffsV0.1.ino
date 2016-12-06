#include "FS.h"

void setup() {
  Serial.begin(115200);
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
}

void loop() {
  if (Serial.available() > 0) {
    char inByte = Serial.read();
    Serial.println(inByte);
    if(inByte=='r') {
      read_memory();
    }
    else delay(2);
  }
  //read_memory();
  //delay(100);
}
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

