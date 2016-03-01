/*
 fsmUnitTest2 -- sketch to test operator new for Fsm class.

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


Fsm *fsm1;
State *state_enter_exit;

void setup()
{
  Serial.begin(BAUD);
  /* NOTE: the following println affects demostrating the defect. */
  //Serial.println("JFM -- Testing operator new");
  state_enter_exit = new State(&on_enter,&on_exit);
  fsm1 = new Fsm(state_enter_exit);
  fsm1->add_timed_transition(state_enter_exit, state_enter_exit, 3000, NULL);
  //Serial.println("Entering main loop");
}

void loop(void)
{
  fsm1->check_timer();
}

