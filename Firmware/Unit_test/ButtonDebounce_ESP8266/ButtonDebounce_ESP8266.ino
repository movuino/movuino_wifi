#include "FS.h"
File f;

// constants won't change. They're used here to
// set pin numbers:
const int buttonPin = 13;     // the number of the pushbutton pin
const int ledPin =  2;      // the number of the LED pin

// variables will change:
int buttonState = 0;         // variable for reading the pushbutton status
long lastButtonTime =0;
int debounceDelay=100;
int mode=0; 
/*modes : 
0: nothing 
1: read from flash
2: write to flash
*/

void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);
  delay(1000);
  //start filesystem
  bool result = SPIFFS.begin();
  Serial.println("SPIFFS opened: " + result);
  File f = SPIFFS.open("/f.txt", "w");
  if (!f) {
      Serial.println("file creation failed");
    }
  else Serial.println("file opened");
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  //Serial.println(buttonState);
  if(!buttonState){
   if(millis()-lastButtonTime>debounceDelay){
     lastButtonTime=millis();
     Serial.println(0);
     read_memory();
     //Serial.println(buttonState);
     }
  }
}
void read_memory()
    {
    // we could open the file
    while(f.available()) {
      //Lets read line by line from the file
      String line = f.readStringUntil('n');
      Serial.println(line);
      }
    }
