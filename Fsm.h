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
  State();
  State(void (*on_enter)(void * ctx), void (*on_state)(void * ctx) = nullptr, void (*on_exit)(void * ctx) = nullptr);
  void (*on_enter)(void * ctx);
  void (*on_state)(void * ctx);
  void (*on_exit)(void * ctx);
};


class Fsm
{
public:
  Fsm(State* initial_state);
  ~Fsm();

  void add_transition(State* state_from, State* state_to, int event,
                      void (*on_transition)(void* ctx) = nullptr);

  void add_timed_transition(State* state_from, State* state_to,
                            unsigned long interval, void (*on_transition)(void* ctx) = nullptr);

  /**
   * checks the timed transitions for the current state and if timeout occurred
   * trigger appropriate transition. Timed transitions are checked and triggered in the same order as added
   * @param ctx pointer to context to be passed onto state functions. Default is nullptr to retain backwards compatability.
   */
  void check_timed_transitions(void* ctx = nullptr);

  /**
   * looks for the current state's timed transitions to the target state and reset the timer 
   * @param state_to target state to reset the timed transition for. If nullptr reset all current state timers
   * @param ctx pointer to context to be passed onto state functions. Default is nullptr to retain backwards compatability.
   */
  void reset_timed_transition(State* state_to);

  /**
   * trigger transition with the event 
   * @param event enum that defines the trigger
   * @param ctx pointer to context to be passed onto state functions. Default is nullptr to retain backwards compatability.
   */
  void trigger(int event, void* ctx = nullptr);
  
  /**
   * iterate one run-cycle of the state machine
   * @param ctx pointer to context to be passed onto state functions. Default is nullptr to retain backwards compatability.
   */
  void run_machine(void* ctx = nullptr);

  /**
   * returns current state (helpful if the same handler is used to drive many similar states)
   * @return current state
   */
  State* get_current_state();

private:
  struct Transition
  {
    State* state_from;
    State* state_to;
    int event;
    void (*on_transition)(void* ctx = nullptr);

  };
  struct TimedTransition
  {
    Transition transition;
    unsigned long start;
    unsigned long interval;
  };

  static Transition create_transition(State* state_from, State* state_to,
                                      int event, void (*on_transition)(void* ctx));

  void make_transition(Transition* transition, void* ctx = nullptr);

private:
  State* m_current_state;
  Transition* m_transitions;
  int m_num_transitions;

  TimedTransition* m_timed_transitions;
  int m_num_timed_transitions;
  bool m_initialized;
};


#endif
