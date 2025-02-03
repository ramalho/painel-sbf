#include "Arduino.h"
#include "garoa_button.h"


Button::Button(uint8_t pin, uint8_t polarity)
  : _debouncer(pin, polarity)
{
  _already_pressed = false;
  _press_started = 0;
  _debouncer.begin();
}

void Button::update() {
  _debouncer.update();
  if (is_pressed()) {
    if (!_already_pressed) {
      _already_pressed = true;
      _press_started = millis();
    }
  } else {
    _already_pressed = false;
    _press_started = 0;
  }
}

bool Button::is_pressed() {return _debouncer.isPressed();}
bool Button::is_released() {return _debouncer.isReleased();}

bool Button::just_pressed() {return _debouncer.justPressed();} 
bool Button::just_released() {return _debouncer.justReleased();}

bool Button::is_held(unsigned long int duration) {
  if (is_pressed() && ((millis() - _press_started) >= duration)) {
    return true;
  }
  return false;
}
