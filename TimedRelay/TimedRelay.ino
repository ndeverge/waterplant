#include <Time.h>
#include <TimeAlarms.h>

#include <Wire.h>
#include <SeeedOLED.h>

#include "DHT.h"
#define DHTPIN 3       //D3 on breakboard

#define DHTTYPE DHT11   // DHT 11 

DHT dht(DHTPIN, DHTTYPE);

#define MOISTUREPIN A0  //A0 on breakboard
// Moisture values (http://wiki.seeedstudio.com/Grove-Moisture_Sensor/)
// Sensor in dry soil     Min: 0      Max:300
// Sensor in humid soil   Min: 300    Max:700
// Sensor in water 0      Min: 700    Max:950

#define BUTTON 2      //D2 on breakboard

#define RELAY 4  //D4 on breakboard

#define MINUTE 60

#define HOUR 60*MINUTE

#define DAY 24UL*HOUR

#define WATERING_DURATION 10*MINUTE

int buttonState = 0;         // variable for reading the pushbutton status
AlarmId id;
short watering = 0;

// the setup function runs once when you press reset or power the board
void setup() {

  Serial.begin(9600);
  Serial.println("Initializing");

  Wire.begin();

  dht.begin(); // initialize temp and humidity sensor
  
  SeeedOled.init();  //initialze SEEED OLED display

  SeeedOled.clearDisplay();          //clear the screen and set start position to top left corner
  SeeedOled.setNormalDisplay();      //Set display to normal mode (i.e non-inverse mode)
  SeeedOled.setPageMode();   

  pinMode(BUTTON, INPUT); // initialize the pushbutton pin as an input:
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(RELAY, OUTPUT);
  
  pinMode(MOISTUREPIN, INPUT);

  id = Alarm.timerRepeat(DAY, startWatering); // called once after XXX seconds 
}

// the loop function runs over and over again forever
void loop() {

  // read the state of the pushbutton value:
  buttonState = digitalRead(BUTTON);

  // check if the pushbutton is pressed.
  // if it is, the buttonState is HIGH:
  if (buttonState == HIGH) {
      startWatering();
  }

  if (watering == 0) {
    displayTimeLeftBeforeWatering();
  } else {
    displayTimeLeftDuringWatering();
  }
  
  displayTempAndHumidity();

  displayMoisture();

  Alarm.delay(500); // wait a bit
}

void startWatering() {
  if (watering == 0) {

    SeeedOled.clearDisplay();
  
    Serial.println("Starting watering");
    
    watering = 1;
    digitalWrite(RELAY, HIGH);   // turn the RELAY on (HIGH is the voltage level)
  
    Alarm.timerOnce(WATERING_DURATION, stopWatering);
  }
}

void stopWatering() {
  Serial.println("Stopping watering");
  
  digitalWrite(RELAY, LOW);   // turn off the RELAY
  watering = 0;
}

void displayTempAndHumidity() {
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
}

void displayMoisture() {
  int moisture = analogRead(MOISTUREPIN);

  // Convert it to a percentage
  moisture = map(moisture, 0, 1023, 0, 100);
  
  SeeedOled.setTextXY(7, 0);
  SeeedOled.putString("Moisture: "); 
  SeeedOled.putFloat(moisture);
  SeeedOled.putString("%"); 
}

void displayTimeLeftDuringWatering() {
  SeeedOled.setTextXY(0, 0);
  SeeedOled.putString("Arrosage pendant");

long timeLeft = WATERING_DURATION - now();
  Serial.println(timeLeft);
  
  SeeedOled.setTextXY(1, 1);
  printDigitsOled(minute(timeLeft));
  SeeedOled.putString(" minutes");
  SeeedOled.setTextXY(2, 1);
  printDigitsOled(second(timeLeft));
  SeeedOled.putString(" secondes");
}

void displayTimeLeftBeforeWatering() {

  time_t time = DAY - now();

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

void printDigitsOled(int digits) {
  if (digits < 10)
    SeeedOled.putNumber(0);
  SeeedOled.putNumber(digits);
}
