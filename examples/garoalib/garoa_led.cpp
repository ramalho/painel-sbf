#include "Arduino.h"
#include "garoa_led.h"

Led::Led(int pin, int signal_on) {
  this->pin = pin;
  this->signal_on = signal_on;
  signal_off = signal_on == HIGH ? LOW : HIGH;
  lit = false;
  blinking = false;
  blink_count = 0;
  blink_start_time = 0;
  blink_duration = 0;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, signal_off);
}

void Led::on() {
  digitalWrite(pin, signal_on);
  lit = true;
}
 
void Led::off() {
  digitalWrite(pin, signal_off);
  lit = false;
}

bool Led::toggle() {
  if (lit) off();
  else on();
  return lit;
}

void Led::start_blinking(unsigned long blink_duration) {
  this->blink_duration = blink_duration;
  blinking = true;
  blink_count = 0;
  blink_start_time = millis();
}

int Led::update() {
  // update state of blinking led and
  // return blink_count
  if (!blinking) return 0;
  if ((millis() - blink_start_time) >= blink_duration) {
    blink_start_time = millis();
    toggle();
    if (lit) blink_count++;
  }
  return blink_count;
}

void Led::stop_blinking() {
  blinking = false;
  off();
}
