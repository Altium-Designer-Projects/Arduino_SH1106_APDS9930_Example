#include <Wire.h>
#include <APDS9930.h>

#define DUMP_REGS

APDS9930 apds = APDS9930();
uint16_t proximity_data = 0;

bool tempFlag = true;

word SecondInterval = 1000;
word secondPreviousMillis = 0;
word secondCurrentMillis;

void setup() {
  //analogReference(EXTERNAL);

  // Initialize Serial port
  Serial.begin(9600);
  Serial.println();
  Serial.println(F("---------------------------"));
  Serial.println(F("APDS-9930 - ProximitySensor"));
  Serial.println(F("---------------------------"));
  
  // Initialize APDS-9930 (configure I2C and initial values)
  if ( apds.init() ) {
    Serial.println(F("APDS-9930 initialization complete"));
  } else {
    Serial.println(F("Something went wrong during APDS-9930 init!"));
  }
  
  // // Adjust the Proximity sensor gain
  // if ( !apds.setProximityGain(PGAIN_2X) ) {
  //   Serial.println(F("Something went wrong trying to set PGAIN"));
  // }
  
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

void loop() {
 if(tempFlag == true){ 
  if(!apds.readProximity(proximity_data)){
     Serial.println("Error reading proximity value");
  }else{
     Serial.print("Proximity: ");
     Serial.println(proximity_data);
  }
  tempFlag = false;
 }
 
 secondCurrentMillis = millis();

 if(secondCurrentMillis - secondPreviousMillis >= SecondInterval){
    tempFlag = tempFlag ? false : true;
    secondPreviousMillis = secondCurrentMillis;
 }

}
