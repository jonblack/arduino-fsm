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

typedef void (*CallbackFunction) ();


struct State {
    State(String name, CallbackFunction on_enter, CallbackFunction on_state = NULL, CallbackFunction on_exit = NULL);  
    String name;
    CallbackFunction on_enter;
    CallbackFunction on_state;
    CallbackFunction on_exit; 
      int id;
    private:
      static int _next_id;
};


class Fsm {
  public:
    Fsm(State* initial_state);
    ~Fsm();

    void add_transition(State* state_from, State* state_to, int event, CallbackFunction on_transition = NULL, String name = "");
    void add_timed_transition(State* state_from, State* state_to, unsigned long interval, CallbackFunction on_transition = NULL, String name = "");
    void check_timed_transitions();
    void trigger(int event);
    void run_machine();
    String get_dot_definition();
    State* get_current_state();

    void on_transition(CallbackFunction f);

  private:
    struct Transition {
      State* state_from;
      State* state_to;
      int event;
      CallbackFunction on_transition;
    };

    struct TimedTransition {
      Transition transition;
      unsigned long start;
      unsigned long interval;
    };

    static Transition create_transition(State* state_from, State* state_to, int event, CallbackFunction on_transition);
    void make_transition(Transition* transition);

    State* m_current_state;
    Transition* m_transitions;
    int m_num_transitions;
    CallbackFunction m_on_transition;

    TimedTransition* m_timed_transitions;
    int m_num_timed_transitions;
    bool m_initialized;

    String create_dot_transition(String from, String to, String label = "");
    String create_dot_active_node();
    String create_dot_header();
    String create_dot_inital_state(String name);
    String m_dot_definition;
};

#endif
