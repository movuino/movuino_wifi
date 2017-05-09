/*

  one_device.ino - example using i2c_adc_ads7828 library

  Library:: i2c_adc_ads7828
  Author:: Doc Walker <4-20ma@wvfans.net>

  Copyright:: 2009-2016 Doc Walker

  Licensed under the Apache License, Version 2.0 (the "License");
  you may not use this file except in compliance with the License.
  You may obtain a copy of the License at

      http://www.apache.org/licenses/LICENSE-2.0

  Unless required by applicable law or agreed to in writing, software
  distributed under the License is distributed on an "AS IS" BASIS,
  WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  See the License for the specific language governing permissions and
  limitations under the License.

*/


#include <i2c_adc_ads7828.h>


// device 0
// Address: A1=0, A0=0
// Command: SD=1, PD1=1, PD0=1
ADS7828 device(0, SINGLE_ENDED | REFERENCE_ON | ADC_ON/*, 0x0F*/); //no mask to do find how it works
ADS7828* adc = &device;
ADS7828Channel* Analog0= adc->channel(0);
ADS7828Channel* Analog1= adc->channel(1);
ADS7828Channel* Analog2= adc->channel(2);
ADS7828Channel* Analog3= adc->channel(3);
ADS7828Channel* Analog4= adc->channel(4);
ADS7828Channel* Analog5= adc->channel(5);
ADS7828Channel* Analog6= adc->channel(6);
ADS7828Channel* Analog7= adc->channel(7);
/*ADS7828Channel* ambientTemp = adc->channel(0);
ADS7828Channel* waterTemp = adc->channel(1);
ADS7828Channel* filterPressure = adc->channel(2);
ADS7828Channel* waterLevel = adc->channel(3);*/


void setup()
{
  // enable serial monitor
  Serial.begin(115200);

  // enable I2C communication
  ADS7828::begin();

  // adjust scaling on an individual channel basis 12 bits = 4096
  Analog0->minScale = 0;
  Analog0->maxScale = 4095;
  Analog1->minScale = 0;
  Analog1->maxScale = 4095;
  Analog2->minScale = 0;
  Analog2->maxScale = 4095;
  Analog3->minScale = 0;
  Analog3->maxScale = 4095;
  Analog4->minScale = 0;
  Analog4->maxScale = 4095;
  Analog5->minScale = 0;
  Analog5->maxScale = 4095;
  Analog6->minScale = 0;
  Analog6->maxScale = 4095;
  Analog7->minScale = 0;
  Analog7->maxScale = 4095;
}


void loop()
{
  // update all registered ADS7828 devices/unmasked channels
  ADS7828::updateAll();
  // output moving average values to console
  Serial.print("\n A0: ");
  Serial.print(Analog0->value(), DEC);
  // output moving average values to console
  Serial.print("\n A1: ");
  Serial.print(Analog1->value(), DEC);
   // output moving average values to console
  Serial.print("\n A2: ");
  Serial.print(Analog2->value(), DEC);
 // output moving average values to console
  Serial.print("\n A3: ");
  Serial.print(Analog3->value(), DEC);
   // output moving average values to console
  Serial.print("\n A4: ");
  Serial.print(Analog4->value(), DEC);
   // output moving average values to console
 Serial.print("\n A5: ");
  Serial.print(Analog5->value(), DEC);
   // output moving average values to console
  Serial.print("\n A6: ");
  Serial.print(Analog6->value(), DEC);
   // output moving average values to console
  Serial.print("\n A7: ");
  Serial.print(Analog7->value(), DEC);
  // delay
  delay(100);
   Serial.print("\n- - - - - - - - - - - - - - - - - - - - \n");
}
