#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <SH1106.h>
#include <APDS9930.h>

#define DUMP_REGS

APDS9930 apds = APDS9930();
uint16_t proximity_data = 0;

#define OLED_CS    10
#define OLED_DC    9
#define OLED_RESET 8

Adafruit_SH1106 display(OLED_DC, OLED_RESET, OLED_CS); // hw spi

bool sensorFlag = true;

word SecondInterval = 500;
word secondPreviousMillis = 0;
word secondCurrentMillis;

char displayBuffer[8] = " ";

void setup(){
  
  display.begin(SH1106_SWITCHCAPVCC);
  display.display();
  delay(500);
  display.clearDisplay();
  
  // Initialize APDS-9930 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9930 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9930 init!"));
  }
  
  // Start running the APDS-9930 proximity sensor (no interrupts)
  if ( apds.enableProximitySensor(false) ) {
    Serial.println(F("Proximity sensor is now running"));
  } else {
    Serial.println(F("Something went wrong during sensor init!"));
  }
#ifdef DUMP_REGS
  /* Register dump */
  uint8_t reg;
  uint8_t val;

  for(reg = 0x00; reg <= 0x19; reg++) {
    if( (reg != 0x10) && \
        (reg != 0x11) )
    {
      apds.wireReadDataByte(reg, val);
      Serial.print(reg, HEX);
      Serial.print(": 0x");
      Serial.println(val, HEX);
    }
  }
  apds.wireReadDataByte(0x1E, val);
  Serial.print(0x1E, HEX);
  Serial.print(": 0x");
  Serial.println(val, HEX);
#endif

}

void loop(){
 if(sensorFlag == true){ 
    // Read the proximity value
    if ( !apds.readProximity(proximity_data) ) {
      Serial.println("Error reading proximity value");
    } else {
      Serial.print("Proximity: ");
      Serial.println(proximity_data);
    }
    sensorFlag = false;
 }
 secondCurrentMillis = millis();

 if(secondCurrentMillis - secondPreviousMillis >= SecondInterval){
    sensorFlag = sensorFlag ? false : true;
    secondPreviousMillis = secondCurrentMillis;
 }
 
  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);  
  display.setCursor(5, 20);
  display.print("APDS-9930 Proximity");
  
  display.setTextSize(2);  
  display.setCursor(40,40);
  sprintf(displayBuffer, "%04d", proximity_data );  
  display.print(displayBuffer);
    
  display.display();
  
}
