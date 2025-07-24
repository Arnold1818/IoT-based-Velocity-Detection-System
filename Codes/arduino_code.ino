#include <Wire.h>
#include <LiquidCrystal_I2C.h>

LiquidCrystal_I2C lcd(0x27, 16, 2); // Set the I2C address, and LCD size (columns x rows)
int timer1;
int timer2;
float Time;
float distance = 0.15; // distance between the ir sensors
float speed;
int ir_s1 = 2; // Adjust the pin numbers to match your IR sensor connections
int ir_s2 = 3;

void setup() {
  pinMode(ir_s1, INPUT);
  pinMode(ir_s2, INPUT);
  Wire.begin();
  lcd.init();
  lcd.backlight();
  lcd.setCursor(0, 0);
  lcd.print("Speed: ");
  lcd.setCursor(0, 1);
  lcd.print("Direction: ");
  Serial.begin(9600);
}

void loop() {
  if (digitalRead(ir_s1) == LOW && digitalRead(ir_s2) == HIGH) {
    timer1 = millis();
    while (digitalRead(ir_s2) == HIGH) {
      // Do nothing, just wait for the train to pass
    }
    timer2 = millis();
    Time = (timer2 - timer1) / 1000.0; // Convert milliseconds to seconds

    // Calculate speed (v = d / t)
    speed = distance / Time;
    // Convert speed to Km/Hr
    speed = speed * 3600.0 / 1000.0;

    // Display direction and speed on LCD if speed is within the range (1 to 250 Km/Hr)
    if (speed >= 1 && speed <= 250) {
      lcd.setCursor(7, 0);
      lcd.print(speed, 1); // Display one decimal place
      lcd.print(" Km/Hr");
      lcd.setCursor(11, 1);
      lcd.print("-->");
      Serial.println("Direction: -->");
      Serial.print("Speed: ");
      Serial.print(speed, 1); // Display one decimal place
      Serial.println(" Km/Hr");
    }

    // Reset values
    timer1 = 0;
    timer2 = 0;
    Time = 0;
    speed = 0;

    delay(1000);
  }

  if (digitalRead(ir_s1) == HIGH && digitalRead(ir_s2) == LOW) {
    timer2 = millis();
    while (digitalRead(ir_s1) == HIGH) {
      // Do nothing, just wait for the train to pass
    }
    timer1 = millis();
    Time = (timer1 - timer2) / 1000.0; // Convert milliseconds to seconds

    // Calculate speed (v = d / t)
    speed = distance / Time;
    // Convert speed to Km/Hr
    speed = speed * 3600.0 / 1000.0;

    // Display direction and speed on LCD if speed is within the range (1 to 250 Km/Hr)
    if (speed >= 1 && speed <= 250) {
      lcd.setCursor(7, 0);
      lcd.print(speed, 1); // Display one decimal place
      lcd.print(" Km/Hr");
      lcd.setCursor(11, 1);
      lcd.print("<--");
      Serial.println("Direction: <--");
      Serial.print("Speed: ");
      Serial.print(speed, 1); // Display one decimal place
      Serial.println(" Km/Hr");
    }

    // Reset values
    timer1 = 0;
    timer2 = 0;
    Time = 0;
    speed = 0;

    delay(1000);
  }
}
