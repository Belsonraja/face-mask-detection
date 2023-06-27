/* 
Developed by BELSONRAJA T
ME, Applied Electronics
*/

#include <Servo.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <Adafruit_MLX90614.h>

LiquidCrystal_I2C lcd(0x3F,16,2);  // set the LCD address to 0x27 for a 16 chars and 2 line display
Adafruit_MLX90614 mlx = Adafruit_MLX90614();

Servo servo;  // create servo object to control a servo [twelve servo objects can be created on most boards]

int pos = 0;    // variable to store the servo position
int buzzPin= 12; // I/O-pin from buzzer connects here 
int data = 0;

void setup() {
  servo.attach(9);  // attaches the servo on pin 9 to the servo object
  pinMode(buzzPin,OUTPUT); // Set buzzer-pin as output

  lcd.begin();
  lcd.backlight();
  lcd.clear();

  mlx.begin();
  
  Serial.begin(9600);  // set serial monitor to same speed
}

void loop() {
  bool tempFlag = false;
  displayTemprature(mlx.readObjectTempC(), mlx.readObjectTempF());
  
  if (Serial.available()>0 && tempFlag == false) {  // check if any data received
    data = Serial.read(); // yes, so read it from incoming buffer
    if (data == '0' || mlx.readObjectTempC() > 37){
      //accessDenied();
      tone(buzzPin, 1000); // Send 1KHz sound signal...
      servo.write(180);
      displayAccessMessage("Access Denied!");
    } else if(data == '1'){
      //accessGranted();
      noTone(buzzPin);     // Stop sound...
      servo.write(0);
      displayAccessMessage("Access Granted!");
    }
  }
}

void accessGranted(){
  noTone(buzzPin);     // Stop sound...
  servo.write(0);
  displayAccessMessage("Access Granted!");
}

void accessDenied(){
  tone(buzzPin, 1000); // Send 1KHz sound signal...
  servo.write(180);
  displayAccessMessage("Access Denied!");
}

void displayAccessMessage(String message){
  lcd.setCursor(0,0);
  lcd.print(message);
}

void displayTemprature(int tempC, int tempF){
  lcd.setCursor(0,1);
  lcd.print("Temp:");
  lcd.print(tempC);
  lcd.print(char(223));
  lcd.print("C");
  lcd.print(" | ");
  lcd.print(tempF);
  lcd.print(char(223));
  lcd.print("F");
}
