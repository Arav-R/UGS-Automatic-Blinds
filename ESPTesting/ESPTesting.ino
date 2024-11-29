#include <WiFi.h>

const char* ssid = "utguest";
const char* password = "12345678";

void setup(){
    Serial.begin(115200);
    delay(1000);

    WiFi.mode(WIFI_STA); //Optional
    WiFi.begin(ssid, password);
    Serial.println("\nConnecting");
    Serial.println(ssid);
    Serial.println(password);

    // check if not connected
    while(WiFi.status() != WL_CONNECTED){

        Serial.print(".");
        delay(100);
    }

    Serial.println("\nConnected to the WiFi network");
    Serial.print("Local ESP32 IP: ");
    Serial.println(WiFi.localIP());
}

void loop(){}