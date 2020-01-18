# Movuino Wifi v0.99.3
Hardware and software files for Movuino wifi v0.99.3

Tested on ESP core v2.5.0 (please use the version of esp core so autoreset is working)

# Installation

 1- Install drivers for CP2104 http://www.silabs.com/products/development-tools/software/usb-to-uart-bridge-vcp-drivers <br>
 2- Install support for ESP8266 as explained here :  https://learn.adafruit.com/adafruit-huzzah-esp8266-breakout/using-arduino-ide  [Use version v2.5.0 of espcore]  
 3- In arduino software Choose the good board : Adafruit Feather huzzah esp8266  
 4- Copy and paste the content of the folder "libraries" into your arduino sketchbook library folder  
	- to know where is your arduino sketchbook folder Open Arduino software got to File -> Preferences -> Sketchbook location  
	- Libraries folder is inside the sketchbook folder  
 5- Install asukiaaa library (Open arduino software go to sketch -> library manager -> serch for mpu9250 and install it )  
 6- Install adafruit neopixel library using library manager  
 

# Unit tests (-> Firmware -> Unit tests)

### Basic example : Blink
-blink : blink the integrated LED (pin 2)

### I2C Scanner (used to check that all sensors are recognized)
-i2cscanner : scan all i2c adresses and display when a sensor is detected

### Neopixel  (pin 15)
-simpleNeopixel 

### MPU9250 9 axis IMU (I2C address 0x69)

**using I2cdev+MPU6050 library (compatible with mpu6050, mpu9150 and mpu9250)**<br>
- MPU6050_raw<br>
**using asukiaaa library**<br>
- MPU9250-asukiaaaGetData<br>

### Battery level (ADC reading with voltage divider 0.23)
- simpleBatteryMonitor-ADC 

# Main UDP examples  (-> Firmware -> UDP examples)

- udp.ino : send a ping to the udp server each time a motion > than threshold is detected (need the udp_recive.py opened on the server)<br>
- udp_receive.py simple python server to reeive data sent by movuinos on the same network<br>
- udp-send-broadcast.py ;: python code to send data to all movuinos connected on a network<br>