#include <IRremote.h>
#include <LiquidCrystal.h>

const int sensor_pin = A0; 
int sensor; 
const int threshold= 500; 
//aaaahghghgghga
const int IR_RECEIVE_PIN = 2;
LiquidCrystal lcd(7, 8, 9, 10, 11, 12);  

int a = 1;
int b = 0;
bool startAnimation = false;
bool startProgram = false;

void setup() {
  pinMode (led, OUTPUT);  
  Serial.begin(9600);
  IrReceiver.begin(IR_RECEIVE_PIN, ENABLE_LED_FEEDBACK);
  lcd.begin(16, 2);
  lcd.clear();
  delay(1000);
  lcd.setCursor(0, 0);
  lcd.print("Codequest");
  lcd.setCursor(0, 1);
  lcd.print("Enter 1 to start");
}

void loop() {
  if (IrReceiver.decode()) {
    unsigned long code = IrReceiver.decodedIRData.decodedRawData;
    Serial.print("IR Code: 0x");
    Serial.println(code, HEX);
    IrReceiver.printIRResultShort(&Serial);

    if (!startProgram && code == 0xF30CFF00) {  
      Serial.println("Start Program");
      lcd.clear();
      lcd.setCursor(0, 0);
      lcd.print("Choose a number");
      lcd.setCursor(0,1);
      lcd.print("for an adventure");
      startProgram = true;
    }

    if (startProgram && code == 0xAD52FF00) { 
      Serial.println("Start Animation");
      startAnimation = true;
    }

    if (startProgram && code == 0xA15EFF00) //button 3
    { 
      Serial.println("Start Adverture");
      startSunlightmission = true;
    }

    IrReceiver.resume();  
  }

  if(startSunlightmission)
  {
    sensor = analogRead(sensor_pin); 
    Serial.println(sensor);
    
    if (sensor > threshold) { // if sensor reading more less than threshold 

      } 
    else{ 

      }
  }

  if (startAnimation) {
    lcd.setCursor(0, 0);
    lcd.print("                ");
    lcd.setCursor(0, 1);
    lcd.print("                ");
    lcd.setCursor(b, 0);
    lcd.print("Before Luck");
    b++;

    if (b > 16) {
      String messages[] = {
        "k", "ck", "uck", "luck", "e luck",
        "re luck", "ore luck", "fore luck", "efore luck"
      };

      for (int i = 0; i < 9; i++) {
        lcd.setCursor(0, 0);
        lcd.print("                ");
        lcd.setCursor(0, 0);
        lcd.print(messages[i]);
        lcd.setCursor(0, 1);
        lcd.print(a++);
        delay(1000);
      }

      b = 0;
      startAnimation = false;
    }

    lcd.setCursor(0, 1);
    lcd.print(a++);
    delay(1000);
  }
}
