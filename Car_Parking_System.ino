#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2);
#include <Servo.h>

Servo myservo1;

// setting up the arduino pins
int IR1 = 4;  
int IR2 = 2;  

// total number of parking slots 
int Slot = 4;  

int flag1 = 0;
int flag2 = 0;

void setup() {
	lcd.init();
	lcd.backlight();

	pinMode(IR1, INPUT);
	pinMode(IR2, INPUT);

	// determining the pin of the servo motor 
	myservo1.attach(3);
	myservo1.write(110);

	lcd.setCursor(0, 0);
	lcd.print("    ARDUINO    ");
	lcd.setCursor(0, 1);
	lcd.print(" PARKING SYSTEM ");
	delay(2000);
}

void loop() 
{

	// checking for car entry 
	if (digitalRead(IR1) == LOW && flag1 == 0) 
	{
		// checking whether there's empty slots left 
		if (Slot > 0) {
			flag1 = 1;
			// decrease the slot when a car enters 
			if (flag2 == 0) {
				myservo1.write(40);
				Slot = Slot - 1;
			}
		} 
		else 
		{
			lcd.setCursor(0, 0);
			lcd.print("    SORRY    ");
			lcd.setCursor(0, 1);
			lcd.print("parking is full!");
			delay(3000);
			lcd.clear();
		}
	}


	if (digitalRead(IR2) == LOW && flag2 == 0) 
	{
		flag2 = 1;
		// increase the slot when a car leaves 
		if (flag1 == 0) {
			myservo1.write(40);
			Slot = Slot + 1;
		}
	}

	// resetting and shutting the door when the entry/exit of the car finishes 
	if (flag1 == 1 && flag2 == 1) {
		delay(1000);
		myservo1.write(110);
		flag1 = 0, flag2 = 0;
	}

	// printing the number of slots available
	lcd.setCursor(0, 0);
	lcd.print("    WELCOME!    ");
	lcd.setCursor(0, 1);
	lcd.print("Slots Left:    ");
	lcd.print(Slot);
}