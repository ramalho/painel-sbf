#ifndef ledlib_h
#define ledlib_h

class Led {
  public:
    Led(int pin, int signal_on);
    void on();
    void off();
    bool toggle();
    void start_blinking(unsigned long cycle_duration);
    void update();
    void stop_blinking();
  private:
    int pin;
    bool lit;
    int signal_on;
    int signal_off;
    int blink_count;
    bool blinking;
    unsigned long blink_start_time;
    unsigned long blink_duration;
};

#endif