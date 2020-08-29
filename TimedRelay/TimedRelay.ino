#include <Time.h>
#include <TimeAlarms.h>


#define BUTTON 2      //D2 on breakboard

#define RELAY 4  //D4 on breakboard

#define MINUTE 60

#define HOUR 60*MINUTE

int buttonState = 0;         // variable for reading the pushbutton status

// the setup function runs once when you press reset or power the board
void setup() {

  pinMode(BUTTON, INPUT); // initialize the pushbutton pin as an input:
  
  // initialize digital pin LED_BUILTIN as an output.
  pinMode(RELAY, OUTPUT);

  Alarm.timerRepeat(24UL*HOUR, watering); // called once after XXX seconds 
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

  Alarm.delay(100); // wait a bit
}

void watering() {

  // 10mn
  const unsigned long duration = 10UL * (60UL * 1000UL);//ms
  
  digitalWrite(RELAY, HIGH);   // turn the RELAY on (HIGH is the voltage level)

  Alarm.delay(duration);
  digitalWrite(RELAY, LOW);   // turn off the RELAY
}
