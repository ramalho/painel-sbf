#ifndef ledlib_h
#define ledlib_h

class Led {
  public:
    Led(int pin, int signal_on);
    void on();
    void off();
    bool toggle();
    void start_blinking(unsigned long cycle_duration);
    int update();
    void stop_blinking();
  private:
    int pin;
    bool lit;
    int signal_on;
    int signal_off;
    bool blinking;
    int blink_count;
    unsigned long blink_start_time;
    unsigned long blink_duration;
};

#endif
