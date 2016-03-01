/*
 fsmUnitTest1 -- sketch to test basic functionality of the Fsm class.
                 Test runs the simplest timed transition by creating
                 a periodic timer that fires every three seconds.

 John Mauzey 20160226
*/
#include <Fsm.h>

#define BAUD 115200
#define PERIOD 3000

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


State state_enter_exit(&on_enter, &on_exit);
Fsm fsm1(&state_enter_exit);

void setup()
{
  Serial.begin(BAUD);
  //Serial.println("JFM -- Testing periodic timer");
  fsm1.add_timed_transition(&state_enter_exit, &state_enter_exit, PERIOD, NULL);
  //Serial.println("Entering main loop");
}

void loop(void)
{
  fsm1.check_timer();
}

