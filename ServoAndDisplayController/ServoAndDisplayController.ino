#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

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

void setup() {
  myServo.attach(9); // Attach the servo to pin 9
  Serial.begin(9600);
  myServo.write(LEFT_CLOSED); // Start with the flap closed

  // Initialize the LCD
  lcd.init();
}

void loop() {
  // Turn the backlight on for feeding
  lcd.backlight();
  // Display an initial message
  lcd.setCursor(0, 0); // Set cursor to first row, first column
  lcd.print("Cat Feeder Ready");
  lcd.setCursor(0, 1); // Set cursor to second row, first column
  lcd.print("Waiting...");
  delay(1000);
  
  // Dispense food
  dispenseFood();
  
  // Toggle the direction for the next feeding
  dispenseLeftToRight = !dispenseLeftToRight;

  // Turn off the backlight after feeding
  delay(10000);
  lcd.noBacklight();

  // Wait for the next feeding cycle (e.g., 12 hours)
  delay(60000); // Replace with 12 hours (12 * 60 * 60 * 1000)
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
}

void moveServoToPosition(char bowl, int position) {
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Dispensing in ");
  lcd.setCursor(0, 1); // Set cursor to second row, first column
  lcd.print("bowl ");
  lcd.print(bowl);

  myServo.write(position);
  delay(2000); // Adjust the delay to control how long food is dispensed
  Serial.print("Moved to position: ");
  Serial.println(position);
}
