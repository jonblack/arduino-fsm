// This file is part of arduino-fsm.
//
// arduino-fsm is free software: you can redistribute it and/or modify it under
// the terms of the GNU Lesser General Public License as published by the Free
// Software Foundation, either version 3 of the License, or (at your option)
// any later version.
//
// arduino-fsm is distributed in the hope that it will be useful, but WITHOUT
// ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or
// FITNESS FOR A PARTICULAR PURPOSE.  See the GNU Lesser General Public License
// for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with arduino-fsm.  If not, see <http://www.gnu.org/licenses/>.

#ifndef FSM_H
#define FSM_H


#if defined(ARDUINO) && ARDUINO >= 100
  #include <Arduino.h>
#else
  #include <WProgram.h>
#endif


struct State
{
  State(void (*on_enter)(), void (*on_exit)());

  void (*on_enter)();
  void (*on_exit)();
};


class Fsm
{
public:
  Fsm(State* initial_state);
  ~Fsm();

  void add_transition(State* state_from, State* state_to, int event,
                      void (*on_transition)());

  void add_timed_transition(State* state_from, State* state_to,
                            unsigned long interval, void (*on_transition)());

  void trigger(int event);
  void check_timer();

private:
  struct Transition
  {
    State* state_from;
    State* state_to;
    int event;
    void (*on_transition)();

    State* make_transition();
  };
  struct TimedTransition
  {
    Transition transition;
    unsigned long start;
    unsigned long interval;
  };

  static Transition create_transition(State* state_from, State* state_to,
                                      int event, void (*on_transition)());

private:
  State* m_current_state;
  Transition* m_transitions;
  int m_num_transitions;

  TimedTransition* m_timed_transitions;
  int m_num_timed_transitions;
};


#endif
