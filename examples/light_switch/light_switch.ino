#include "Fsm.h"

// State machine variables
#define FLIP_LIGHT_SWITCH 1

// set to true for built-in LED on ESP8266, false for Arduino/Genuino
#define LED_PULLUP   true

State state_light_on(on_light_on_enter, NULL, &on_light_on_exit);
State state_light_off(on_light_off_enter, NULL, &on_light_off_exit);
Fsm fsm(&state_light_off);

// Transition callback functions
void on_light_on_enter()
{
  Serial.println("Entering LIGHT_ON");
  digitalWrite(LED_BUILTIN, LED_PULLUP ? LOW : HIGH);

}

void on_light_on_exit()
{
  Serial.println("Exiting LIGHT_ON");
}

void on_light_off_enter()
{
  Serial.println("Entering LIGHT_OFF");
  digitalWrite(LED_BUILTIN, LED_PULLUP ? HIGH : LOW);
}

void on_light_off_exit()
{
  Serial.println("Exiting LIGHT_OFF");
}

void on_trans_light_on_light_off()
{
  Serial.println("Transitioning from LIGHT_ON to LIGHT_OFF");
}

void on_trans_light_off_light_on()
{
  Serial.println("Transitioning from LIGHT_OFF to LIGHT_ON");
}

// standard arduino functions
void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT);

  fsm.add_transition(&state_light_on, &state_light_off,
                     FLIP_LIGHT_SWITCH,
                     &on_trans_light_on_light_off);
  fsm.add_transition(&state_light_off, &state_light_on,
                     FLIP_LIGHT_SWITCH,
                     &on_trans_light_off_light_on);
}

void loop()
{
  fsm.run_machine();
  delay(2000);
  fsm.trigger(FLIP_LIGHT_SWITCH);
}
