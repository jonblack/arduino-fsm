#include "Fsm.h"

/*
 * FSM Library sample with user and timed 
 * transitions.
 * Uses a button and Arduino builtin led, 
 * button can be replaced just grounding 
 * pin.
 */

// Used pins
#define LED_PIN             15
#define LED_PULLUP          false
#define BUTTON_PIN          16

//Events
#define BUTTON_EVENT  0

int buttonState = 0;

/* state 1:  led off
 * state 2:  led on
 * transition from s1 to s2 on button press
 * transition back from s2 to s1 after 3 seconds or button press
 */
State state_led_off(&led_off, &check_button);
State state_led_on(&led_on, &check_button);

Fsm fsm(&state_led_off);

// Transition functions
void led_off() {
    Serial.println("led_off");
    digitalWrite(LED_PIN, LED_PULLUP ? true : false);
}

void led_on() {
    Serial.println("led_on");
    digitalWrite(LED_PIN, LED_PULLUP ? false : true);
}

void check_button() {
    int buttonState = digitalRead(BUTTON_PIN);
    if (buttonState == LOW) {
        Serial.println("button_pressed");
        fsm.trigger(BUTTON_EVENT);
    }
}

// standard arduino functions
void setup() {
    Serial.begin(9600);
  
    pinMode(LED_PIN, OUTPUT);
    pinMode(BUTTON_PIN, INPUT_PULLUP);

    fsm.add_transition          (&state_led_off, &state_led_on, BUTTON_EVENT);
    fsm.add_transition          (&state_led_on, &state_led_off, BUTTON_EVENT);
    fsm.add_timed_transition    (&state_led_on, &state_led_off, 3000);
    Serial.println("Setup END");
}

void loop() {
  // Call fsm run
  fsm.run_machine();
  delay(100);
}
