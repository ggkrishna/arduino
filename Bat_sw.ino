// Date and time functions using a DS3231 RTC connected via I2C and Wire lib
#include <Wire.h>
#include "RTClib.h"



// Clock variables
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {"Sunday", "Monday", "Tuesday", "Wednesday", "Thursday", "Friday", "Saturday"};


// Voltage Sensor variables

const int vsens = A0;       // voltage input for measurement
int swt = 13;                   // if the output of swt is LOW    - switch in off state
                                // if the output of swt is HIGH   - switch in on state



void setup () {

#ifndef ESP8266
  while (!Serial); // for Leonardo/Micro/Zero
#endif

  Serial.begin(9600);

  delay(3000); // wait for console opening

  if (! rtc.begin()) {
    Serial.println("Couldn't find RTC");
    while (1);
  }

  if (rtc.lostPower()) {
    Serial.println("RTC lost power, lets set the time!");
    // following line sets the RTC to the date & time this sketch was compiled
    rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));
    // This line sets the RTC with an explicit date & time, for example to set
    // January 21, 2014 at 3am you would call:
    //rtc.adjust(DateTime(2018, 06, 19, 15, 9, 00));
  }
}





void loop () {

    int in_vlt = analogRead(vsens);  // read the input from voltage sensor
    double voltage = map(in_vlt,0,1023, 0, 500); // map 0-1023 to 0-5V
    voltage/=100;   // input voltage in double precision
    
    
  
    DateTime now = rtc.now();
  
    Serial.print(now.year(), DEC);
    Serial.print('/');
    Serial.print(now.month(), DEC);
    Serial.print('/');
    Serial.print(now.day(), DEC);
    Serial.print(" (");
    Serial.print(daysOfTheWeek[now.dayOfTheWeek()]);
    Serial.print(") ");
    Serial.print(now.hour(), DEC);
    Serial.print(':');
    Serial.print(now.minute(), DEC);
    Serial.print(':');
    Serial.print(now.second(), DEC);
    Serial.println();
    Serial.println();
    
    if (now.hour()>=12 && now.hour()<22 && voltage>2.86)
    {
      Serial.println(" On Battery  ");
      Serial.print("Battery Voltage=");
      Serial.print(voltage);
      digitalWrite(swt, HIGH);  
    }

    else 
    {
      Serial.println(" On Grid  ");
      Serial.print("Battery Voltage=");
      Serial.print(voltage);
      digitalWrite(swt, LOW);  
    }

    
    Serial.println();
    Serial.println("-----------------------");
    delay(5000);
}
