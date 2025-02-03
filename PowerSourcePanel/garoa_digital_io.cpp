#include "Arduino.h"
#include "garoa_digital_io.h"

/************************************************************* Jumper class */

Jumper::Jumper(uint8_t pin, uint8_t pin_mode) {
  _pin = pin;
  _closed_state = pin_mode == INPUT_PULLUP ? LOW : HIGH;
  pinMode(pin, pin_mode);
}

bool Jumper::is_closed() {
  return digitalRead(_pin) == _closed_state;
}

/******************************************************** OutputOnOff class */

OutputOnOff::OutputOnOff(int pin, int signal_on) {
  _pin = pin;
  _signal_on = signal_on;
  _signal_off = signal_on == HIGH ? LOW : HIGH;
  _on = false;
  _cycling = false;
  _cycle_count = 0;
  _cycle_start_time = 0;
  _cycle_duration = 0;

  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, _signal_off);
}

bool OutputOnOff::is_on() {
  return _on;
}

void OutputOnOff::turn_on() {
  digitalWrite(_pin, _signal_on);
  _on = true;
}
 
void OutputOnOff::turn_off() {
  digitalWrite(_pin, _signal_off);
  _on = false;
}

bool OutputOnOff::toggle() {
  if (_on) turn_off();
  else turn_on();
  return _on;
}

void OutputOnOff::start_cycling(unsigned long cycle_duration) {
  _cycle_duration = cycle_duration;
  _cycling = true;
  _cycle_count = 0;
  _cycle_start_time = millis();
}

int OutputOnOff::update() {
  // update state of device and
  // return cycle_count
  if (!_cycling) return 0;
  unsigned long half_cycle = _cycle_duration / 2;
  if ((millis() - _cycle_start_time) >= half_cycle) {
    _cycle_start_time = millis();
    toggle();
    if (_on) _cycle_count++;
  }
  return _cycle_count;
}

void OutputOnOff::stop_cycling() {
  _cycling = false;
  turn_off();
}


