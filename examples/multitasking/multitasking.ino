// This example shows how two finite state machines can be used to simulate
// multitasking on an arduino. Two LED's are turned on and off at irregular
// intervals; the finite state machines take care of the transitions.

#include "Fsm.h"

#define LED1_PIN 15
#define LED2_PIN 13

void on_led1_on_enter() {
  Serial.println("on_led1_on_enter");
  digitalWrite(LED1_PIN, HIGH);
}

void on_led1_off_enter() {
  Serial.println("on_led1_off_enter");
  digitalWrite(LED1_PIN, LOW);
}

void on_led2_on_enter() {
  Serial.println("on_led2_on_enter");
  digitalWrite(LED2_PIN, HIGH);
}

void on_led2_off_enter() {
  Serial.println("on_led2_off_enter");
  digitalWrite(LED2_PIN, LOW);
}

State state_led1_on("LED1 on", on_led1_on_enter);
State state_led1_off("LED1 off", on_led1_off_enter);

State state_led2_on("LED2 on", on_led2_on_enter);
State state_led2_off("LED2 off", on_led2_off_enter);

Fsm fsm_led1(&state_led1_off);
Fsm fsm_led2(&state_led2_off);

void setup() {
  Serial.begin(9600);
  while (!Serial) {
    delay(50);
  }
  Serial.println("\nRUNNING: multitasking");

  pinMode(LED1_PIN, OUTPUT);
  pinMode(LED2_PIN, OUTPUT);

  fsm_led1.add_timed_transition(&state_led1_off,  &state_led1_on,     1000);
  fsm_led1.add_timed_transition(&state_led1_on,   &state_led1_off,    3000);
  fsm_led2.add_timed_transition(&state_led2_off,  &state_led2_on,     1000);
  fsm_led2.add_timed_transition(&state_led2_on,   &state_led2_off,    2000);

  Serial.println(fsm_led1.get_dot_definition());
  Serial.println(fsm_led2.get_dot_definition());
  Serial.println("Setup END");
}


void loop() {
  fsm_led1.run_machine();
  fsm_led2.run_machine();
  delay(200);
}
