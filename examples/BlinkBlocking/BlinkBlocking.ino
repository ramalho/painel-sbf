/*

Blink, Blocking

Turns on the red LED for one second, turns off for one second,
repeating forever.
When the button is pressed, the red LED completes its current cycle,
and then the green LED is turned on.

Coding conventions:

- UPPER_CASE for constants;
- camelCase for variables or functions defined by Arduino;
- snake_case for variables or functions defined by us;

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

void set_red(bool state) {
  digitalWrite(RED_LED, state ? LOW : HIGH);
}

void setup() {
  // configure pins
  pinMode(RED_LED, OUTPUT);
  pinMode(GREEN_LED, OUTPUT);
  pinMode(BUTTON, INPUT_PULLUP);
}

void loop() {
  if (read_button()) {
    set_green(true);
  } else {
    set_green(false);
    set_red(true);
    delay(ONE_SECOND);
    set_red(false);
    delay(ONE_SECOND);
  }
}
