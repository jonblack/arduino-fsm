/*
 initBugTest -- sketch to demonstrate startup behaviour of a classic event
                driven FSM and also a timed-transition FSM.

                Issue with first invokation of the trigger() function.
                Issue with first invokation of the check_timer() function.

                Expect to see the "on_enterX" function called followed by
                the "on_exitX" function.

                Only one state machine is needed to demonstrate the problem.
                However the change I made to fix the problem requires a fresh
                copy of the FSM to excercise the two code paths affected.

 John Mauzey 20160212
*/
#include <Fsm.h>

#define BAUD 115200 //for console log of status messages

//define a standard FSM that prints a text message on both enter and exit
#define TRANS_TEST 1  //for testing classic transitions (not timed)
void on_enter1(void)
{
  Serial.print(millis());
  Serial.println(" fsm1: on_enter called");
}

void on_exit1(void)
{
  Serial.print(millis());
  Serial.println(" fsm1: on_exit  called ");
}

State state_enter_exit1(&on_enter1,&on_exit1);
Fsm fsm1(&state_enter_exit1);

//define a periodic FSM that prints a text message on both enter and exit
void on_enter2(void)
{
  Serial.print(millis());
  Serial.println(" fsm2: on_enter called");
}

void on_exit2(void)
{
  Serial.print(millis());
  Serial.println(" fsm2: on_exit  called ");
}

State state_enter_exit2(&on_enter2,&on_exit2);

Fsm fsm2(&state_enter_exit2);

void setup()
{
  Serial.begin(BAUD);
  fsm1.add_transition(&state_enter_exit1, &state_enter_exit1, TRANS_TEST, NULL);
  fsm2.add_timed_transition(&state_enter_exit2, &state_enter_exit2, 3000, NULL);
  
  Serial.println("Testing classic state machine -- Sending first trigger");
  fsm1.trigger(TRANS_TEST);

  Serial.println("Testing classic state machine -- Sending second trigger");
  fsm1.trigger(TRANS_TEST);
  Serial.println("");
  Serial.println("Testing timed state machine -- 3 second periodic timer");
}

void loop(void)
{
  fsm2.check_timer();
}

