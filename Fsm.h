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
  State(void (*on_enter)(), void (*on_state)(), void (*on_exit)());
  void (*on_enter)();
  void (*on_state)();
  void (*on_exit)();
};


class Fsm
{
public:
  Fsm(State* initial_state);
  ~Fsm();

  /**
   * add_transition
   *
   * Adds a transition from `state_from` to `state_to` when `event` is
   * triggered. `on_transition` is called when the transition occurs.
   */
  void add_transition(State* state_from, State* state_to, int event,
                      void (*on_transition)());

  /**
   * add_timed_transition
   *
   * Adds a timed transition from `state_from` to `state_to` when `interval`
   * has elapsed. `on_transition` is called when the transition occurs.
   */
  void add_timed_transition(State* state_from, State* state_to,
                            unsigned long interval, void (*on_transition)());

  /**
   * init
   *
   * Initializes the machine, transitioning into the initial state by calling
   * `on_enter` and `on_state`.
   *
   * This method should be called once, probably somewhere in `setup()`.
   */
  void init();

  /**
   * process
   *
   * Processes changes to the machine. Calls the `on_state` callback for the
   * current state before checking if any timed transitions should be fired.
   *
   * This method should be called repeatedly, probably somewhere in `loop()`.
   */
  void process();

  /**
   * trigger
   *
   * Triggers a state change. The current state plus the given event determine
   * which state the machine should transition into.
   */
  void trigger(int event);

private:
  struct Transition
  {
    State* state_from;
    State* state_to;
    int event;
    void (*on_transition)();

  };
  struct TimedTransition
  {
    Transition transition;
    unsigned long start;
    unsigned long interval;
  };

  void check_timed_transitions();

  static Transition create_transition(State* state_from, State* state_to,
                                      int event, void (*on_transition)());

  void make_transition(Transition* transition);

private:
  State* m_current_state;
  Transition* m_transitions;
  int m_num_transitions;

  TimedTransition* m_timed_transitions;
  int m_num_timed_transitions;
  bool m_initialized;
};


#endif
