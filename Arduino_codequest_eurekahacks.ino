#include <IRremote.h>
#include <LiquidCrystal.h>
#include <DHT_U.h>  
#include <DHT.h>
#include <dht.h>
#define DHTPIN A0
#define DHTTYPE DHT11
#define button 3
#define button2 4
DHT dht(DHTPIN, DHTTYPE);
int lives = 3;
const int sensor_pin = A1;
bool goBackToStart = false;
int sensor;
const int threshold = 250;
const int IR_RECEIVE_PIN = 2;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);
int timer = 30;
bool startProgram = false;
int startSunlightmission = 0;
int startTemperaturemission = 0;
void setup() { 
  Serial.begin(9600);
  pinMode(button, INPUT_PULLUP);
  pinMode(button2, INPUT_PULLUP);
  Serial.println("DHT11 Humidity & temperature Sensor\n\n");
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  lcd.begin(16, 2);
  lcd.clear();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Codequest");
  lcd.setCursor(0, 1);
  lcd.print("Enter 1 to start");
  dht.begin();
}

void loop() {

  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("IR Code: 0x");
    Serial.println(code, HEX);
    IrReceiver.printIRResultShort(&Serial);

    if (!startProgram && code == 0xF30CFF00) {  // Button "1"
      Serial.println("Start Program");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Choose a number");
      lcd.setCursor(0, 1);
      lcd.print("for an adventure");
      startProgram = true;
    }
    if(startProgram && goBackToStart)
    {
        lcd.setCursor(0, 0);
        lcd.print("Choose a number");
        lcd.setCursor(0, 1);
        lcd.print("for an adventure");
    }
    if (startProgram && code == 0xA15EFF00) { // Button "3"
      Serial.println("Start Adventure");
      startSunlightmission = 1;
      timer = 30; // Reset timer at start of mission
    }

    if (startProgram && code == 0xE718FF00) { // Button "2"
      Serial.println("Start Adventure");
      float h = dht.readHumidity();
      float t = dht.readTemperature();
      Serial.print(t);

      timer = 60; // Reset timer at start of mission
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Guess the temp");
      lcd.setCursor(0, 1);
      lcd.print(startTemperaturemission);
      if(IrReceiver.decode())
      {
        
        if (startProgram && code == 0xE619FF00){
          lcd.clear();
          lcd.setCursor(0, 0);
          lcd.print("Failed");
      }

      }

        if (digitalRead(button) == HIGH) {
          delay(200);
          startTemperaturemission = startTemperaturemission + 1;
          lcd.setCursor(0, 1);
          lcd.print(startTemperaturemission);
        }
        if (digitalRead(button2) == HIGH) {
          delay(200);
          if(t == startTemperaturemission)
          {
            lcd.clear();
            lcd.setCursor(0,0);
            lcd.print("success");
          }
          else{
            if (lives > 0){

            }
            else{
              lcd.clear();
              lcd.setCursor(0,0);
              lcd.print("You Failed");
            }
          }
        }
    }

    IrReceiver.resume();
  }

  // Run sunlight mission if it's started
  if (startSunlightmission == 1) {
    sensor = analogRead(sensor_pin);
    Serial.print("Sensor Value: ");
    Serial.println(sensor);

    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Go into sunlight");
    lcd.setCursor(0, 1);
    lcd.print("Time left: ");
    lcd.print(timer);
    delay(1000);

    if (sensor > threshold) {
      startSunlightmission = 2;
    } else {
      delay(1000);
      timer--;

      if (timer <= 0) {
        lcd.clear();
        lcd.setCursor(0, 0);
        lcd.print("Mission Failed");
        startSunlightmission = 0;
      }
    }
  }

  if (startSunlightmission == 2) {
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Mission Completed");
    delay(3000);
    startSunlightmission = 0;
    goBackToStart = true;
    delay(2000);
  }
}
