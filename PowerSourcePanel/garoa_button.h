#include "Adafruit_Debounce.h"

#ifndef garoa_button_h
#define garoa_button_h

class Button {
  public:
    Button(uint8_t pin, uint8_t mode);
    void update();
    bool is_pressed();
    bool is_released();
    bool just_pressed();
    bool just_released();
    bool is_held(unsigned long int duration);
  private:
    Adafruit_Debounce _debouncer;
    bool _already_pressed;
    unsigned long int _press_started;
};

#endif

