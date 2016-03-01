/*
 fsmUnitTest2 -- sketch to test Fsm using pointers tothe state machine.

 John Mauzey 20160227
*/
#include <Fsm.h>

#define BAUD 115200

//define a periodic fsm that prints a text message on both enter and exit
void on_enter(void)
{
  Serial.print(millis());
  Serial.println(" fsm1: on_enter called");
}

void on_exit(void)
{
  Serial.print(millis());
  Serial.println(" fsm1: on_exit  called ");
}


Fsm *fsm1;
State *state_enter_exit;

void setup()
{
  Serial.begin(BAUD);
  state_enter_exit = new State(&on_enter,&on_exit);
  fsm1 = new Fsm(state_enter_exit);
  fsm1->add_timed_transition(state_enter_exit, state_enter_exit, 3000, NULL);
}

uint32_t lastTime = 0;
void loop(void)
{
  if (millis() - lastTime > 2000) {
    lastTime = millis();
    Serial.println("heartbeat:");
  }
  fsm1->check_timer();
}

