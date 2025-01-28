#include "Arduino.h"
#include "garoa_led.h"

Led::Led(int pin, int signal_on) {
  _pin = pin;
  _signal_on = signal_on;
  _signal_off = signal_on == HIGH ? LOW : HIGH;
  _lit = false;
  _blinking = false;
  _blink_count = 0;
  _blink_start_time = 0;
  _blink_duration = 0;

  pinMode(_pin, OUTPUT);
  digitalWrite(_pin, _signal_off);
}

bool Led::lit() {
  return _lit;
}

void Led::on() {
  digitalWrite(_pin, _signal_on);
  _lit = true;
}
 
void Led::off() {
  digitalWrite(_pin, _signal_off);
  _lit = false;
}

bool Led::toggle() {
  if (_lit) off();
  else on();
  return _lit;
}

void Led::start_blinking(unsigned long blink_duration) {
  _blink_duration = blink_duration;
  _blinking = true;
  _blink_count = 0;
  _blink_start_time = millis();
}

int Led::update() {
  // update state of blinking led and
  // return blink_count
  if (!_blinking) return 0;
  if ((millis() - _blink_start_time) >= _blink_duration) {
    _blink_start_time = millis();
    toggle();
    if (_lit) _blink_count++;
  }
  return _blink_count;
}

void Led::stop_blinking() {
  _blinking = false;
  off();
}
