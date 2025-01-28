#ifndef garoa_button_h
#define garoa_button_h

class Button {
  public:
    Button(int pin, int mode);
    bool is_pressed();
  private:
    int pin;
    int mode;
    int state;
};

#endif

