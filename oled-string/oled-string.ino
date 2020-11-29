#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <SH1106.h>

#define OLED_CS    10
#define OLED_DC    9
#define OLED_RESET 8

Adafruit_SH1106 display(OLED_DC, OLED_RESET, OLED_CS); // hw spi

void setup(){
  
  display.begin(SH1106_SWITCHCAPVCC);
  display.display();
  delay(500);
  display.clearDisplay();
  
}

void loop(){

  display.setTextColor(WHITE, BLACK);
  display.setTextSize(1);  
  display.setCursor(5, 20);
  display.print("APDS-9930 Proximity");
  
  display.setTextSize(2);  
  display.setCursor(40,40);
  display.print("1023");
    
  display.display();
  
}
