/*
  AnalogReadSerial
  Reads an analog input on pin 0, prints the result to the serial monitor.
  Attach the center pin of a potentiometer to pin A0, and the outside pins to +5V and ground.

 This example code is in the public domain.
 */

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin(115200);
  pinMode(A0, INPUT);
}

// the loop routine runs over and over again forever:
void loop() {
  // read the input on analog pin 0:
  int sensorValue = analogRead(A0);
  // print out the value you read:
  // Serial.println(sensorValue);
  /*0.23 voltage divider*/
  /*1023 = 1v*/
  float voltValue=sensorValue*0.000977;
  //Serial.println(voltValue);
  /*real value = volt value * 4.3 */
  float battValue=voltValue*4.3;
  Serial.println(battValue);
  delay(10);        // delay in between reads for stability
}
