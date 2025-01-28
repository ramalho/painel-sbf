/*

Blink, Non Blocking

Turns on the red LED for one second, turns off for one second,
repeating forever.
When the button is pressed, the red LED is immediately turned off,
and the green LED is turned on.

Coding conventions:

- UPPER_CASE for constants;
- camelCase for variables or functions defined by Arduino;
- snake_case for variables or functions defined by us;
- CamelCase for names of classes defined by us;

*/

const int RED_LED = 12;
const int GREEN_LED = 11;
const int BUTTON = 7;
const unsigned long ONE_SECOND = 1000;


bool read_button() {
  return (digitalRead(BUTTON) == HIGH) ? false : true;
}

void set_green(bool state) {
  digitalWrite(GREEN_LED, state ? LOW : HIGH);
}

bool _red_state = true;

void set_red(bool state) {
  _red_state = state;
  digitalWrite(RED_LED, state ? LOW : HIGH);
}

bool toggle_red() {
  _red_state = !_red_state;
  set_red(_red_state);
  return _red_state;
}   

void setup() {
  // configure pins
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);

  // set initial states
  set_red(_red_state);
  set_green(false);
}

unsigned long _previous_toggle_time = 0;

bool time_to_toggle(int interval) {
  if ((millis() - _previous_toggle_time) >= interval) {
    _previous_toggle_time = millis();  // restart timer
    return true;
  } 
  else {
    return false;
  }
 }

bool red_blinking = true;

void loop() {
  if (red_blinking && time_to_toggle(ONE_SECOND)) {
    toggle_red();
  }
  if (read_button()) {
    set_red(false);
    set_green(true);
    red_blinking = false;
  } else {
    set_green(false);
    red_blinking = true;
  }
}
