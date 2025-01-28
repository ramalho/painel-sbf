#include "Arduino.h"
#include "garoa_button.h"

/*
class Button {
  public:
    Button(int pin, int mode);
    bool is_pressed();
  private:
    int pin;
    int mode;
    int state;
};
*/

Button::Button(int pin, int mode) {
  this->pin = pin;
  this->mode = mode;

  pinMode(pin, mode);
  state = is_pressed();
}

bool Button::is_pressed() {
  bool reading = digitalRead(pin) == HIGH;
  state = (mode == INPUT_PULLUP) ? !reading : reading;
  return state;
}
 


