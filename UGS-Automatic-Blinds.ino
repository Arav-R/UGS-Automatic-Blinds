#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <Servo.h>

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -5 * 3600; // UTC-5
const int daylightOffset_sec = 3600;

// WiFi credentials
const char* ssid = "utexas";
const char* username = "ar69444";
const char* identity = "ar69444";
const char* password = "Saumya2305##";

// Servo settings
Servo blindsServo;
const int servoPin = 9; // Specify the correct servo pin for your setup

// Schedule times in hours and minutes (24-hour format)
const int openHour = 8, openMinute = 0;
const int closeHour = 20, closeMinute = 0;

// Function to spin the servo for 5 seconds to open the blinds
void openBlinds() {
  blindsServo.write(180); // Full speed in one direction (adjust if needed)
  Serial.println("Blinds opening...");
  delay(5000);            // Run the motor for 5 seconds
  blindsServo.write(90);   // Stop the servo
}

// Function to spin the servo for 5 seconds to close the blinds
void closeBlinds() {
  blindsServo.write(0);    // Full speed in the opposite direction
  Serial.println("Blinds closing...");
  delay(5000);             // Run the motor for 5 seconds
  blindsServo.write(90);   // Stop the servo
}

void setup() {  
  Serial.begin(115200);
  blindsServo.attach(servoPin);

  // Connect to WiFi
  WiFi.begin(ssid, WPA2_AUTH_PEAP, username, identity, password);
  while(WiFi.status() != WL_CONNECTED) {
    delay(1000);  
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi!");

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
  Serial.println(&timeinfo, "%A, %B %d %Y %H:%M:%S");
}

void loop() {
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo)) {
    Serial.println("Failed to obtain time");
    return;
  }
  
  // Print the current time
  printLocalTime();

  // Check if it's time to open or close the blinds
  if (timeinfo.tm_hour == openHour && timeinfo.tm_min == openMinute) {
    openBlinds();
  } else if (timeinfo.tm_hour == closeHour && timeinfo.tm_min == closeMinute) {
    closeBlinds();
  }
  
  delay(60000); // Check every minute
}
