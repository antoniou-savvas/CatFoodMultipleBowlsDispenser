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

  // Set RTC time if necessary (uncomment this block to set time initially)
  /*
  Ds1302::DateTime dt;
  dt.year = 24;           // Last two digits of the year (e.g., 2024)
  dt.month = Ds1302::MONTH_OCT;
  dt.day = 25;
  dt.hour = 14;
  dt.minute = 59;
  dt.second = 0;
  dt.dow = Ds1302::DOW_FRI;
  rtc.setDateTime(&dt);
  */

  // Display an initial message for 3 seconds
  lcd.setCursor(0, 0);
  lcd.print("Cat Feeder Ready");
  lcd.setCursor(0, 1);
  lcd.print("Waiting...");
  delay(3000);
  lcd.noBacklight();
}

void loop() {
  // Get the current time
  rtc.getDateTime(&currentTime);
  int hour = currentTime.hour;
  int minute = currentTime.minute;

  // Check if it's feeding time
  if ((hour == 12 && minute == 0) || (hour == 20 && minute == 0)) {
    // Turn the backlight on for feeding
    lcd.backlight();
    // Display the current time on the LCD for 3 seconds
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Time: ");
    lcd.print(formatTime(currentTime));
    lcd.setCursor(0, 1);
    lcd.print("Date: ");
    lcd.print(formatDate(currentTime));
    delay(3000);

    dispenseFood();
    // Toggle the direction for the next feeding
    dispenseLeftToRight = !dispenseLeftToRight;
    // Turn off the backlight after feeding
    lcd.noBacklight();
  }

  delay(60000); // Update every minute
}

void dispenseFood() {
  if (dispenseLeftToRight) {
    // Move from left to right
    moveServoToPosition('1', BOWL_1);
    moveServoToPosition('2', BOWL_2);
    moveServoToPosition('3', BOWL_3);
    moveServoToPosition('4', BOWL_4);
  } else {
    // Move from right to left
    moveServoToPosition('4', BOWL_4);
    moveServoToPosition('3', BOWL_3);
    moveServoToPosition('2', BOWL_2);
    moveServoToPosition('1', BOWL_1);
  }

  // Close the flap after dispensing
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
  char buffer[9]; // HH:MM:SS
  snprintf(buffer, sizeof(buffer), "%02d:%02d:%02d", dt.hour, dt.minute, dt.second);
  return String(buffer);
}

// Helper function to format date as a string
String formatDate(const Ds1302::DateTime& dt) {
  char buffer[11]; // YYYY-MM-DD
  snprintf(buffer, sizeof(buffer), "20%02d-%02d-%02d", dt.year, dt.month, dt.day);
  return String(buffer);
}
