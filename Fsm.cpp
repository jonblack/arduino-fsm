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


State::State(void (*on_enter)(), void (*on_exit)())
: on_enter(on_enter),
  on_exit(on_exit)
{
}


Fsm::Fsm(State* initial_state)
: m_current_state(initial_state),
  m_transitions(NULL),
  m_num_transitions(0)
{
}


Fsm::~Fsm()
{
  free(m_transitions);
  m_transitions = NULL;
}


void Fsm::add_transition(State* state_from, State* state_to, String event,
                         void (*on_transition)())
{
  if (state_from == NULL || state_to == NULL)
    return;

  Transition transition;
  transition.state_from = state_from;
  transition.state_to = state_to;
  transition.event = event;
  transition.on_transition = on_transition;

  m_transitions = (Transition*) realloc(m_transitions, (m_num_transitions + 1)
                                                       * sizeof(Transition));
  m_transitions[m_num_transitions] = transition;
  m_num_transitions++;
}


void Fsm::trigger(String event)
{
  // Find the transition with the current state and given event.
  Transition transition;
  bool found = false;
  for (int i = 0; i < m_num_transitions; ++i)
  {
    transition = m_transitions[i];
    if (transition.state_from == m_current_state &&
        transition.event == event)
    {
      found = true;
      break;
    }
  }

  // Execute the handlers in the correct order.
  if (found)
  {
    if (transition.state_from->on_exit != NULL)
      transition.state_from->on_exit();

    if (transition.on_transition != NULL)
      transition.on_transition();

    m_current_state = transition.state_to;

    if (transition.state_to->on_enter != NULL)
      transition.state_to->on_enter();
  }
}
