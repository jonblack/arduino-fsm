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

#include "Fsm.h"

State::State(String name, CallbackFunction on_enter, CallbackFunction on_state, CallbackFunction on_exit) :
    name(name),
    on_enter(on_enter),
    on_state(on_state),
    on_exit(on_exit) {
}

Fsm::Fsm(State* initial_state) :
    m_current_state(initial_state),
    m_transitions(NULL),
    m_num_transitions(0),
    m_num_timed_transitions(0),
    m_dot_definition(create_dot_inital_state(initial_state->name)),
    m_initialized(false),
    m_on_transition(NULL) {
}

Fsm::~Fsm() {
    free(m_transitions);
    free(m_timed_transitions);
    m_transitions = NULL;
    m_timed_transitions = NULL;
}

void Fsm::on_transition(CallbackFunction f) {
    m_on_transition = f;
}

void Fsm::add_transition(State* state_from, State* state_to, int event, CallbackFunction on_transition, String name) {
    if (state_from == NULL || state_to == NULL) {
        return;
    }
    Transition transition = Fsm::create_transition(state_from, state_to, event, on_transition);
    m_transitions = (Transition*) realloc(m_transitions, (m_num_transitions + 1) * sizeof(Transition));
    m_transitions[m_num_transitions] = transition;
    m_num_transitions++;

    m_dot_definition =
        m_dot_definition +
        create_dot_transition(state_from->name, state_to->name, name);
}

void Fsm::add_timed_transition(State* state_from, State* state_to, unsigned long interval, CallbackFunction on_transition, String name) {
    if (state_from == NULL || state_to == NULL) {
        return;
    }
    Transition transition = Fsm::create_transition(state_from, state_to, 0, on_transition);
    TimedTransition timed_transition;
    timed_transition.transition = transition;
    timed_transition.start = 0;
    timed_transition.interval = interval;

    m_timed_transitions = (TimedTransition*) realloc(m_timed_transitions, (m_num_timed_transitions + 1) * sizeof(TimedTransition));
    m_timed_transitions[m_num_timed_transitions] = timed_transition;
    m_num_timed_transitions++;

    m_dot_definition =
        m_dot_definition +
        create_dot_transition(state_from->name, state_to->name, name + " (" + String(interval) + ")");
}

Fsm::Transition Fsm::create_transition(State* state_from, State* state_to, int event, CallbackFunction on_transition) {
    Transition t;
    t.state_from = state_from;
    t.state_to = state_to;
    t.event = event;
    t.on_transition = on_transition;
    return t;
}

void Fsm::trigger(int event) {
    if (m_initialized) {
    // Find the transition with the current state and given event.
    for (int i = 0; i < m_num_transitions; i++) {
      if (m_transitions[i].state_from == m_current_state && m_transitions[i].event == event) {
        Fsm::make_transition(&(m_transitions[i]));
        return;
      }
    }
  }
}

void Fsm::check_timed_transitions() {
    for (int i = 0; i < m_num_timed_transitions; i++) {
        TimedTransition* transition = &m_timed_transitions[i];
        if (transition->transition.state_from == m_current_state) {
            if (transition->start == 0) {
                transition->start = millis();
            } else {
                unsigned long now = millis();
                if (now - transition->start >= transition->interval) {
                    Fsm::make_transition(&(transition->transition));
                    transition->start = 0;
                }
            }
        }
    }
}

State* Fsm::get_current_state() {
    return m_current_state;
}

void Fsm::run_machine() {
    // first run must exec first state "on_enter"
    if (!m_initialized) {
        m_initialized = true;
        if (m_current_state->on_enter != NULL) {
            m_current_state->on_enter();
        }
    }
    if (m_current_state->on_state != NULL) {
        m_current_state->on_state();
    }

    Fsm::check_timed_transitions();
}

void Fsm::make_transition(Transition* transition) {
    // Execute the handlers in the correct order.
    if (transition->state_from->on_exit != NULL) {
        transition->state_from->on_exit();
    }
    if (transition->on_transition != NULL) {
      transition->on_transition();
    }
    if (transition->state_to->on_enter != NULL) {
        transition->state_to->on_enter();
    }
    m_current_state = transition->state_to;

    // Initialice all timed transitions from m_current_state
    unsigned long now = millis();
    for (int i = 0; i < m_num_timed_transitions; i++) {
        TimedTransition* ttransition = &m_timed_transitions[i];
        if (ttransition->transition.state_from == m_current_state) {
            ttransition->start = now;
        }
    }
    if (m_on_transition != NULL) {
        m_on_transition();
    }
}

String Fsm::get_dot_definition() {
    return
        "digraph G {\n" +
        create_dot_header() +
        m_dot_definition +
        create_dot_active_node() +
        "}\n";
}

String Fsm::create_dot_transition(String from, String to, String label /* = "" */) {
    return "\t\"" +
        from + "\" -> \"" + to + "\"" +
        ((label != "") ? String(" [label=\"" + label + "\"]") : "") +
        ";\n";
}

String Fsm::create_dot_inital_state(String name) {
    return "\t\"" + name + "\" [shape=doublecircle];\n\n";
}


String Fsm::create_dot_active_node() {
    return "\t\"" + m_current_state->name + "\" [style=filled fillcolor=black fontcolor=white];\n";
}

String Fsm::create_dot_header() {
    return "\trankdir=LR;\n\tnode [shape=circle];\n";
}
