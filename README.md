An arduino library for implementing a finite state machine.

# Documentation

Other than the examples included in the library, the following pages might be
useful to you:

* [Arduino finite state machine library][1]
* [Arduino multitasking using a finite state machine][2]

[1]: https://jonblack.me/arduino-finite-state-machine-library/
[2]: https://jonblack.me/arduino-multitasking-using-finite-state-machines/

# Contribution

If you'd like to contribute to `arduino-fsm` please submit a pull-request on a
feature branch.

# Donate

* Bitcoin: 1HnqohdK1d6gwDc7bT6LPPkmUFAXczEJKp

# Changelog

**Unreleased **

* Added `id` member to `State` struct
* Updated [fsm_graph.ino](https://github.com/LennartHennigs/arduino-fsm/blob/master/examples/fsm_graph/fsm_graph.ino) example to explain naming
* Updated `make_transition()` as suggested in [#22(https://github.com/jonblack/arduino-fsm/issues/22)] and [#35](https://github.com/jonblack/arduino-fsm/issues/35)

**2.3.0 - 01/04/2018 (L. Hennigs)**

* Added `get_dot_definition()` function to FSM to get the FSM graph in [.dot syntax](http://graphs.grevian.org/example)
* Added `get_current_state()` function to FSM
* Added `on_transition()` function to FSM
* States now need to be named, transitions can be named - names are needed for graph generation
* Added webserver example to show a autogenerated FSM graph
* Added `CallbackFunction` struct
* Added `ESP8266` as a possible target architecture in `library.properties`
* Fixed `light_switch` example
* Fixed `timed_switchoff` example
* Updated `keywords.txt`


**2.2.0 - 25/10/2017**

* Add `on_state()` handler to states
* New `run_machine()` method to invoke machine execution (includes a `check_timed_transitions()` call)
* New `timed_switchoff.ino` example sketch to ilustrate new `on_state()` and `run_machine()` funcionality
* Corrections:
 - `make_transition()` correctly initialices timed transitions start milliseconds (`make_transition()` is now a fsm method)
 - Initial state `on_enter()` handler is now correctly executed on fsm first run
 - Removed `Serial.println(now);` trace in _Fsm.cpp_
 - Correct initialization of `m_num_timed_transitions`
 

**2.1.0 - 21/11/2015**

* Add timed transitions

**2.0.0 - 03/09/2015**

* Remove AUTHORS files: too much hassle to maintain
* Add library.properties
* Add keywords.txt
* Remove name attribute from state
* Use int for transition event instead of string

**1.0.0 - 24/12/2013**

* Initial release.
