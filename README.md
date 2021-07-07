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
