/*
 fsmUnitTest1 -- sketch to test added functionality to allow setting of state
                 at runtime.

 John Mauzey 20160226
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


Fsm fsm1;
State state_enter_exit;

void setup()
{
  Serial.begin(BAUD);
  state_enter_exit.init(&on_enter,&on_exit);
  fsm1.init(&state_enter_exit);
  fsm1.add_timed_transition(&state_enter_exit, &state_enter_exit, 3000, NULL);
}

void loop(void)
{
  fsm1.check_timer();
}

