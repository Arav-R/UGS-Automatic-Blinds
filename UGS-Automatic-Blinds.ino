#include <Arduino.h>
#include <WiFi.h>
#include <time.h>
#include <ESP32Servo.h>  // Include ESP32Servo library

const char* ntpServer = "pool.ntp.org";
const long gmtOffset_sec = -5 * 3600; // UTC-5
const int daylightOffset_sec = 0;

// WiFi credentials
const char* ssid = "utexas";
const char* username = "ar69444";
const char* identity = "ar69444";
const char* password = "Saumya2305##";

// Servo settings
Servo blindsServo;             // Servo object for ESP32
const int servoPin = 19;       // Specify the correct servo pin for your setup

// Schedule times in hours and minutes (24-hour format)
const int openHour = 16, openMinute = 46;
const int closeHour = 16, closeMinute = 47;

// Function to spin the servo for 5 seconds to open the blinds
void openBlinds() {
  blindsServo.write(180);  // Full open position (adjust if needed)
  Serial.println("Blinds opening...");
  delay(10000);             // Run the motor for 5 seconds
  blindsServo.write(95);   // Stop the servo (neutral position)
}

// Function to spin the servo for 5 seconds to close the blinds
void closeBlinds() {
  blindsServo.write(0);    // Full close position (adjust if needed)
  Serial.println("Blinds closing...");
  delay(10000);             // Run the motor for 5 seconds
  blindsServo.write(95);   // Stop the servo (neutral position)
}

void setup() {  
  Serial.begin(115200);
  blindsServo.attach(servoPin);  // Attach servo to pin with ESP32Servo library

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
  // Serial.print("Hour: ");
  // Serial.println(timeinfo.tm_hour);



  // Check if it's time to open or close the blinds
  if (timeinfo.tm_hour == openHour && timeinfo.tm_min == openMinute) {
    openBlinds();
  } else if (timeinfo.tm_hour == closeHour && timeinfo.tm_min == closeMinute) {
    closeBlinds();
  }

  // Check for manual command from Serial Monitor
  if (Serial.available() > 0) {
    String command = Serial.readStringUntil('\n');
    command.trim();  // Remove any whitespace or newline characters
    if (command.equalsIgnoreCase("open")) {
      openBlinds();
    } else if (command.equalsIgnoreCase("close")) {
      closeBlinds();
    } else {
      Serial.println("Invalid command. Type 'open' or 'close'.");
    }
  }
  
  delay(60000); // Check every half minute
}
