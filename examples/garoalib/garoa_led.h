#ifndef garoaled_h
#define garoaled_h

class Led {
  public:
    Led(int pin, int signal_on);
    bool lit();
    void on();
    void off();
    bool toggle();
    void start_blinking(unsigned long cycle_duration);
    int update();
    void stop_blinking();
  private:
    int _pin;
    bool _lit;
    int _signal_on;
    int _signal_off;
    bool _blinking;
    int _blink_count;
    unsigned long _blink_start_time;
    unsigned long _blink_duration;
};


#endif
