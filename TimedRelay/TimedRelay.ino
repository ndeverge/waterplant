#include <Time.h>
#include <TimeAlarms.h>

#include <Wire.h>
#include <SeeedOLED.h>

#include "DHT.h"
#define DHTPIN 3       //D3 on breakboard

#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

#define BUTTON 2      //D2 on breakboard

#define RELAY 4  //D4 on breakboard

#define MINUTE 60

#define HOUR 60*MINUTE

#define DAY 24UL*HOUR

int buttonState = 0;         // variable for reading the pushbutton status
AlarmId id;

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(9600);

  Wire.begin();

  dht.begin(); // initialize temp and humidity sensor
  
  SeeedOled.init();  //initialze SEEED OLED display

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();   

  pinMode(BUTTON, INPUT); // initialize the pushbutton pin as an input:
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(RELAY, OUTPUT);

  id = Alarm.timerRepeat(DAY, watering); // called once after XXX seconds 
}

// the loop function runs over and over again forever
void loop() {

  // read the state of the pushbutton value:
  buttonState = digitalRead(BUTTON);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
      watering();
  }

  displayTimeLeftBeforeWatering();

  float temp_hum_val[2] = {0};

  if(!dht.readTempAndHumidity(temp_hum_val)){

    SeeedOled.setTextXY(5, 0);         //Set the cursor to Xth Page, Yth Column
    SeeedOled.putString("Temp: "); 
    SeeedOled.putFloat(temp_hum_val[1]);
    SeeedOled.putString("C");
  
    SeeedOled.setTextXY(6, 0);
    SeeedOled.putString("Humidite: "); 
    SeeedOled.putFloat(temp_hum_val[0]);
    SeeedOled.putString("%");
  } else {
     Serial.println("Failed to get temprature and humidity value.");
  }

  Alarm.delay(500); // wait a bit
}

void watering() {

  // 10mn
  const unsigned long duration = 10UL * (60UL * 1000UL);//ms
  
  digitalWrite(RELAY, HIGH);   // turn the RELAY on (HIGH is the voltage level)

  Alarm.delay(duration);
  digitalWrite(RELAY, LOW);   // turn off the RELAY
}

time_t time;
void displayTimeLeftBeforeWatering() {
  // digital clock display of the time

  time = DAY - now();
  
  printOnSerialPort();

  printOnOled();

}

void printOnSerialPort() {
  Serial.print(hour(time));
  printDigitsSerial(minute(time));
  printDigitsSerial(second(time));
  Serial.println();
}

void printOnOled() {
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("Demarrage dans");
  SeeedOled.setTextXY(1, 1);
  printDigitsOled(hour(time));
  SeeedOled.putString(" heures");
  SeeedOled.setTextXY(2, 1);
  printDigitsOled(minute(time));
  SeeedOled.putString(" minutes");
  SeeedOled.setTextXY(3, 1);
  printDigitsOled(second(time));
  SeeedOled.putString(" secondes");
}

void printDigitsSerial(int digits) {
  Serial.print(":");
  if (digits < 10)
    Serial.print('0');
  Serial.print(digits);
}

void printDigitsOled(int digits) {
  if (digits < 10)
    SeeedOled.putNumber(0);
  SeeedOled.putNumber(digits);
}
