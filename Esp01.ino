#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <EEPROM.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 32

// Use GPIO0 as SDA, GPIO2 as SCL for ESP-01 (software I2C)
#define SDA_PIN 0
#define SCL_PIN 2

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

unsigned char sec = 0, minu = 0;
unsigned int hours = 0  , flag ; 
unsigned long temp = 0;

void setup() {
  Serial.begin(9600);

  // Start I2C on ESP-01
  Wire.begin(SDA_PIN, SCL_PIN);

  // OLED Init
  if (!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) {
    Serial.println(F("SSD1306 allocation failed"));
    while (1);
  }

  display.clearDisplay();
  display.setTextSize(4);
  display.setTextColor(SSD1306_WHITE);

  // EEPROM Init
  EEPROM.begin(512); // Allocate 512 bytes (can be 512 or 4096 depending on flash size)

  // Load hours from EEPROM
  EEPROM.get(0, flag );
  if ( flag == 0 ) 
  {
      EEPROM.get(1, hours  );

  }
  else 
  {
    EEPROM.put(0, 0);
    EEPROM.commit();       // Necessary for ESP to write to flash
   
  }
  display.setCursor(0, 0);
  display.println(hours);
  display.display();
  Serial.println(hours);
}

void loop() {
  unsigned char prevSec = sec;

  if ((millis() - temp) >= 1000) {
    temp = millis();
    sec++;
  }

  if (sec == 60) {
    sec = 0;
    minu++;
  }

  if (minu == 60) {
    minu = 0;
    hours++;

    EEPROM.put(1, hours);  // Save updated hours
    EEPROM.commit();       // Necessary for ESP to write to flash
  }

  if (sec != prevSec) {
    display.clearDisplay();
    display.setCursor(0, 0);
    display.println(hours);
    display.display();
    Serial.println(hours);
  }
}
