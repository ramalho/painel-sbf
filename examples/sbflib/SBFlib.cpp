#include "Arduino.h"
#include "sbflib.h"

Led::Led(int pin, int signal_on) {
  this->pin = pin;
  this->lit = false;
  this->signal_on = signal_on;
  this->signal_off = signal_on == HIGH ? LOW : HIGH;
  this->blinking = false;
  this->blink_count = 0;
  this->blink_start_time = 0;
  this->blink_duration = 0;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, signal_off);
}

void Led::on() {
  digitalWrite(this->pin, this->signal_on);
  this->lit = true;
}
 
void Led::off() {
  digitalWrite(this->pin, this->signal_off);
  this->lit = false;
}

bool Led::toggle() {
  if (this->lit) this->off();
  else this->on();
  return this->lit;
}

void Led::start_blinking(unsigned long blink_duration) {
  this->blink_duration = blink_duration;
  this->blinking = true;
  this->blink_count = 0;
  this->blink_start_time = millis();
}

int Led::update() {
  // update state of blinking led and
  // return blink_count
  if (!this->blinking) return 0;
  if (millis() > (this->blink_start_time + this->blink_duration)) {
    this->toggle();
    this->blink_start_time = millis();
    if (this->lit) this->blink_count++;
  }
  return this->blink_count;
}

void Led::stop_blinking() {
  this->blinking = false;
  this->off();
}

