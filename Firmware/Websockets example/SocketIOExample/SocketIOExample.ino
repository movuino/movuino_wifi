#include <Arduino.h>

#include <ESP8266WiFi.h>
#include <ESP8266WiFiMulti.h>

#include <SocketIoClient.h>

#define USE_SERIAL Serial

ESP8266WiFiMulti WiFiMulti;
//char server[] = "echo.websocket.org";
SocketIoClient webSocket;

void event(const char * payload, size_t length) {
  USE_SERIAL.printf("got message: %s\n", payload);
}
void handler(const char * payload, size_t length) {
  USE_SERIAL.printf("connected");
}

void setup() {
    USE_SERIAL.begin(115200);

   /* USE_SERIAL.setDebugOutput(true);

    USE_SERIAL.println();
    USE_SERIAL.println();
    USE_SERIAL.println();

     for(uint8_t t = 4; t > 0; t--) {
          USE_SERIAL.printf("[SETUP] BOOT WAIT %d...\n", t);
          USE_SERIAL.flush();
          delay(1000);
      }*/

    WiFiMulti.addAP("Air DJ", "");

 
    while(WiFiMulti.run() != WL_CONNECTED) {
        Serial.print(".");
        delay(500);
    }

    Serial.println("");
    Serial.println("WiFi connected");
    Serial.println("IP address: ");
    Serial.println(WiFi.localIP());
    delay(50);

    webSocket.on("event", event);
    webSocket.on("connect", handler);
    webSocket.begin("10.0.0.186",3000);
    //webSocket.begin("my.socket-io.server",3000,"172.20.10.2");
    // use HTTP Basic Authorization this is optional remove if not needed
    //webSocket.setAuthorization("username", "password");
}

void loop() {
    webSocket.loop();
    delay(100);
    webSocket.emit("sample", "\"100\"");
}
