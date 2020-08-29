// Example testing sketch for various DHT humidity/temperature sensors
// Written by ladyada, public domain

#include "DHT.h"
#include <Wire.h>
#include <SeeedOLED.h>

#include "Arduino.h"
#include "SI114X.h"

#define DHTPIN 3       //D3 on breakboard

#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

#if defined(ARDUINO_ARCH_AVR)
#define debug  Serial

#elif defined(ARDUINO_ARCH_SAMD) ||  defined(ARDUINO_ARCH_SAM)
#define debug  SerialUSB
#else
#define debug  Serial
#endif

#define RELAYPIN 4   //D4 on breakboard

#define MOISTUREPIN A0  //A0 on breakboard
// Moisture values (http://wiki.seeedstudio.com/Grove-Moisture_Sensor/)
// Sensor in dry soil     Min: 0      Max:300
// Sensor in humid soil   Min: 300    Max:700
// Sensor in water 0      Min: 700    Max:950

SI114X SI1145 = SI114X(); 
// Sunlight Sensor values (http://wiki.seeedstudio.com/Grove-Sunlight_Sensor/)
// For Vis
// Dark night: 0.001—0.02
// Moonlight night: 0.02—0.3
// Cloudy day indoor: 5—50
// Cloudy day outdoor: 50—500
// Sunny day indoor: 100—1000
// Suitable for reading: 500—600
// Home video camera: 1400


void setup() 
{

    Serial.begin(9600); 
    Serial.println("DHTxx test!");
    Wire.begin();

    /*if using WIO link,must pull up the power pin.*/
    // pinMode(PIN_GROVE_POWER, OUTPUT);
    // digitalWrite(PIN_GROVE_POWER, 1);

    dht.begin();

    pinMode(RELAYPIN, OUTPUT);
    pinMode(MOISTUREPIN, INPUT);

    while (!SI1145.Begin()) {
      Serial.println("Si1145 is not ready!");
      delay(1000);
    }
    Serial.println("Si1145 is ready!");

    SeeedOled.init();  //initialze SEEED OLED display
    SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
    SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
    SeeedOled.setPageMode();           //Set addressing mode to Page Mode
}

void loop() 
{
    float temp_hum_val[2] = {0};
    int moisture = 0;
    // Reading temperature or humidity takes about 250 milliseconds!
    // Sensor readings may also be up to 2 seconds 'old' (its a very slow sensor)
    
    if(!dht.readTempAndHumidity(temp_hum_val)){

        moisture = analogRead(MOISTUREPIN);

        Serial.print("//-----------------------------------------------------------//\r\n");
        Serial.print("Humidity: "); 
        Serial.print(temp_hum_val[0]);
        Serial.print(" %\t");
        Serial.print("Temperature: "); 
        Serial.print(temp_hum_val[1]);
        Serial.print(" °C\t");
        Serial.print("Moisture: "); 
        Serial.println(moisture);

        Serial.print("Vis: "); Serial.print(SI1145.ReadVisible());
        Serial.print("\t\tIR: "); Serial.print(SI1145.ReadIR());
        //the real UV value must be div 100 from the reg value , datasheet for more information.
        Serial.print("\t\t\tUV: ");  Serial.println((float)SI1145.ReadUV()/100);

        SeeedOled.setTextXY(0, 0);         //Set the cursor to Xth Page, Yth Column
        SeeedOled.putString("Temp: "); //Print the String
        SeeedOled.putFloat(temp_hum_val[1]);

        SeeedOled.setTextXY(1, 0);
        SeeedOled.putString("Humidite: "); 
        SeeedOled.putFloat(temp_hum_val[0]);

        SeeedOled.setTextXY(2, 0);
        SeeedOled.putString("Moisture: "); 
        SeeedOled.putFloat(moisture);

        SeeedOled.setTextXY(3, 0);
        SeeedOled.putString("Vis: "); 
        SeeedOled.putFloat(SI1145.ReadVisible());

        SeeedOled.setTextXY(4, 0);
        SeeedOled.putString("IR: "); 
        SeeedOled.putFloat(SI1145.ReadIR());

        SeeedOled.setTextXY(5, 0);
        SeeedOled.putString("UV: "); 
        SeeedOled.putFloat((float)SI1145.ReadUV()/100);

        if (moisture < 500) {
          digitalWrite(RELAYPIN, HIGH);
        } else {
          digitalWrite(RELAYPIN, LOW);
        }
        
    }
    else{
       Serial.println("Failed to get temprature and humidity value.");
    }

   delay(1500);
}
