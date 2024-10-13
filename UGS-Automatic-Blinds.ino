#include <Wire.h>
#include <RTClib.h>
#include <Servo.h>

// Create RTC and Servo objects
RTC_DS3231 rtc;
Servo myServo;

// Set the pin for the servo
int servoPin = 9;

// Define the time when you want the servo to move
int openHour = 8;   // 8 AM
int openMinute = 0; // 8:00 AM
int closeHour = 18; // 6 PM
int closeMinute = 0; // 6:00 PM

void setup() {
  // Start the serial communication
  Serial.begin(9600);
  
  // Initialize the RTC
  if (!rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, setting the time...");
    // Set the RTC time, e.g., to the date and time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
  }

  // Attach the servo to the pin
  myServo.attach(servoPin);
  
  // Set initial servo position (blinds closed)
  myServo.write(0); // Adjust this based on your blinds' closed position
}

void loop() {
  // Get the current time from the RTC
  DateTime now = rtc.now();
  
  // Display the current time in the serial monitor (for debugging)
  Serial.print(now.hour());
  Serial.print(':');
  Serial.print(now.minute());
  Serial.print(':');
  Serial.println(now.second());
  
  // Check if it's time to open the blinds
  if (now.hour() == openHour && now.minute() == openMinute) {
    myServo.write(90); // Adjust this to the desired open position
    Serial.println("Blinds opened.");
  }
  
  // Check if it's time to close the blinds
  if (now.hour() == closeHour && now.minute() == closeMinute) {
    myServo.write(0); // Adjust this to the desired closed position
    Serial.println("Blinds closed.");
  }

  // Small delay to avoid spamming the serial monitor
  delay(1000);
}
