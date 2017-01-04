/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  Most Arduinos have an on-board LED you can control. On the Uno and
  Leonardo, it is attached to digital pin 13. If you're unsure what
  pin the on-board LED is connected to on your Arduino model, check
  the documentation at http://arduino.cc

  This example code is in the public domain.

  modified 8 May 2014
  by Scott Fitzgerald
 */


// the setup function runs once when you press reset or power the board
void setup() {
  // initialize digital pin 13 as an output.
  pinMode(0, OUTPUT);
  Serial.begin(115200);
  pinMode(2, OUTPUT);
  pinMode(13, INPUT_PULLUP);
}

// the loop function runs over and over again forever
void loop() {
  /* if(digitalRead(13)) digitalWrite(0, HIGH);
   else digitalWrite(0, LOW);
   digitalWrite(2, HIGH); 
   delay(1000);              // wait for a second
   digitalWrite(2, LOW);
   delay(1000);              // wait for a second*/
   Serial.print(digitalRead(13));
   delay(100);
}
