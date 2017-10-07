/*
TEsting detection of different click patterns (short_long)
*/

// constants won't change. They're used here to set pin numbers:
const int buttonPin = 13;     // the number of the pushbutton pin
const int ledPin =  2;      // the number of the LED pin

// variables will change:
int buttonState = 1;         // variable for reading the pushbutton status           // the current reading from the input pin
int lastButtonState = 1;   // the previous reading from the input pin
int currentState=1;
// the following variables are unsigned longs because the time, measured in
// milliseconds, will quickly become a bigger number than can be stored in an int.
unsigned long lastDebounceTime = 0;  // the last time the output pin was toggled
unsigned long debounceDelay = 60;  
int count=0;
void setup() {
  // initialize the LED pin as an output:
  pinMode(ledPin, OUTPUT);
  // initialize the pushbutton pin as an input:
  pinMode(buttonPin, INPUT_PULLUP);
  Serial.begin(115200);
  digitalWrite(ledPin, HIGH);
}

void loop() {
  // read the state of the pushbutton value:
  buttonState = digitalRead(buttonPin);
  //click
  if(!buttonState){
    //lastDebounceTime= millis();
   if(millis()-lastDebounceTime>debounceDelay /*&& lastButtonState==0*/){
    //  Serial.println(count);
      count=count+1;
      lastDebounceTime= millis();
      digitalWrite(ledPin, LOW);
     // currentState=!currentState; 
      //lastButtonState=1;
      }
  }
  //if button is not pressed or released
  else {
    digitalWrite(ledPin, HIGH);
    //lastButtonState=0;
    if(count>0){
      Serial.println(count);
      count=0;
    }
  }
}

