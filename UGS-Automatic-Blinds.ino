#include <Arduino.h>

#include "WiFi.h"
#include "time.h"

// NTP server to get time from
const char* ntpServer = "pool.ntp.org";
// Timezone offset in seconds (e.g., for UTC-5: 5 hours * 3600 seconds/hour)
const long gmtOffset_sec = -5 * 3600;
// Daylight saving time offset (in seconds)
const int daylightOffset_sec = 3600;

//Make into an AP to make configurations on
void setup() {  
  Serial.begin(115200);

  WiFi.begin("utexas", WPA2_AUTH_PEAP, "ar69444", "ar69444", "Saumya2305##");
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);  
    Serial.println("Connecting to WiFi..");
  }
  //return from func if alrady connected to WIFI otherwise try AP method.
  // if (WiFi.status() == WL_CONNECTED){
  //   return;
  // }

  Serial.println(" Connected!");


  // Initialize and get the time
  configTime(gmtOffset_sec, daylightOffset_sec, ntpServer);

  Serial.println("Time synchronized from NTP server...");
  
}

void printLocalTime() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S"); // Print the time in human-readable format
}

void loop() {
  // Print the current time every 5 seconds
  printLocalTime();
  delay(5000);
}