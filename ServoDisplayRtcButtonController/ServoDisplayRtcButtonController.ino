#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Ds1302.h>

// Set the LCD address to 0x27 (default) for a 16x2 display
LiquidCrystal_I2C lcd(0x27, 16, 2);

Servo myServo;

// Define the servo positions for the 6 states
const int LEFT_CLOSED = 0;
const int BOWL_1 = 45;
const int BOWL_2 = 90;
const int BOWL_3 = 135;
const int BOWL_4 = 180;
const int RIGHT_CLOSED = 225;

// State to track the current direction
bool dispenseLeftToRight = true; // true = left to right, false = right to left

// RTC pins
const int CLK_PIN = 6;
const int DAT_PIN = 7;
const int RST_PIN = 8;

// Button configuration
const int BUTTON_PIN = 2; // Digital pin for the manual feeding button

// Timing variables
unsigned long previousMillis = 0; // Stores the last time feeding was checked
const long feedingInterval = 60000; // 60 seconds (1 minute) interval

// Button debounce variables
unsigned long lastDebounceTime = 0;
const long debounceDelay = 50; // Debounce time of 50 ms
bool lastButtonState = HIGH;
bool buttonPressed = false;

// Create an RTC instance
Ds1302 rtc(RST_PIN, CLK_PIN, DAT_PIN);

// Create a DateTime structure
Ds1302::DateTime currentTime;

void setup() {
  myServo.attach(9); // Attach the servo to pin 9
  myServo.write(LEFT_CLOSED); // Start with the flap closed

  // Initialize the LCD
  lcd.init();
  lcd.backlight();

  // Initialize the RTC
  rtc.init();

  // Configure button pin as input with internal pull-up
  pinMode(BUTTON_PIN, INPUT_PULLUP);

  // Display an initial message for 3 seconds
  lcd.setCursor(0, 0);
  lcd.print("Cat Feeder Ready");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
  delay(3000);
  lcd.noBacklight();
}

void loop() {
  // Check if it's time to update the feeding logic
  unsigned long currentMillis = millis();
  if (currentMillis - previousMillis >= feedingInterval) {
    previousMillis = currentMillis;
    // Get the current time
    rtc.getDateTime(&currentTime);
    int hour = currentTime.hour;
    int minute = currentTime.minute;

    // Check if it's feeding time
    if (hour == 12 && minute == 0 || hour == 20 && minute == 0) {
      lcd.backlight();
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Time: ");
      lcd.print(formatTime(currentTime));
      lcd.setCursor(0, 1);
      lcd.print("Date: ");
      lcd.print(formatDate(currentTime));
      delay(3000);

      dispenseFood();
      dispenseLeftToRight = !dispenseLeftToRight;
      lcd.noBacklight();
    }
  }

  // Check if the button is pressed
  if (isButtonPressed()) {
    // Manual feeding triggered by the button
    lcd.backlight();
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Manual Feeding");
    delay(3000);
    dispenseFood();
    lcd.noBacklight();
  }
}

void dispenseFood() {
  if (dispenseLeftToRight) {
    moveServoToPosition('1', BOWL_1);
    moveServoToPosition('2', BOWL_2);
    moveServoToPosition('3', BOWL_3);
    moveServoToPosition('4', BOWL_4);
  } else {
    moveServoToPosition('4', BOWL_4);
    moveServoToPosition('3', BOWL_3);
    moveServoToPosition('2', BOWL_2);
    moveServoToPosition('1', BOWL_1);
  }

  myServo.write(dispenseLeftToRight ? RIGHT_CLOSED : LEFT_CLOSED);
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Feeding Done");
  delay(3000);
}

void moveServoToPosition(char bowl, int position) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing in ");
  lcd.setCursor(0, 1);
  lcd.print("bowl ");
  lcd.print(bowl);

  myServo.write(position);
  delay(2000); // Open duration at position
}

// Helper function to format time as a string
String formatTime(const Ds1302::DateTime& dt) {
  char buffer[9];
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", dt.hour, dt.minute, dt.second);
  return String(buffer);
}

// Helper function to format date as a string
String formatDate(const Ds1302::DateTime& dt) {
  char buffer[11];
  snprintf(buffer, sizeof(buffer), "20%02d-%02d-%02d", dt.year, dt.month, dt.day);
  return String(buffer);
}

// Check if the button is pressed with debounce
bool isButtonPressed() {
  bool reading = digitalRead(BUTTON_PIN);
  
  // If the button state has changed
  if (reading != lastButtonState) {
    lastDebounceTime = millis(); // Reset the debounce timer
  }
  
  // Check if enough time has passed to consider the press valid
  if ((millis() - lastDebounceTime) > debounceDelay) {
    // If the state has changed
    if (reading == LOW) {
      buttonPressed = true; // Button pressed
    } else {
      buttonPressed = false; // Button released
    }
  }
  
  lastButtonState = reading; // Update last button state
  return buttonPressed;
}
