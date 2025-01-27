#include "Arduino.h"
#include "ledlib.h"

Led::Led(int pin, int signal_on) {
  this->pin = pin;
  this->lit = false;
  this->signal_on = signal_on;
  this->signal_off = signal_on == HIGH ? LOW : HIGH;
  this->blink_count = 0;
  this->blinking = false;
  this->previous_toggle = 0;

  pinMode(pin, OUTPUT);
  digitalWrite(pin, signal_off);
}

void Led::on() {
  digitalWrite(this->pin, this->signal_on);
}
 
void Led::off() {
  digitalWrite(this->pin, this->signal_off);
}

bool Led::toggle() {
  this->lit = !this->lit;
  digitalWrite(this->pin, this->lit ? this->signal_on : this->signal_off);
  return this->lit;
}

void start_blinking(int times, unsigned long cycle_duration) {
  this->blinking = true;
  this->blink_count = 0;
  this->blink_duration = cycle_duration;
  this->blink_start_time = millis();
}

void update() {
  if (!this->blinking) return;
  if (this.toggle()) {
    this->blink_count++;
  }
  this.
}

