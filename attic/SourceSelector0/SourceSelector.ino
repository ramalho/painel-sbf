
/*
SourceSelector.ino

Code for a power source selector panel for the SBF model railroad layout

# Notes

## 8 relay board

             outputs      
input   │led│ N.O. │ N.C.
────────┼───┼──────┼──────
default │off│ open │closed 
LOW     │on │closed│ open
HIGH    |off│ open │closed

"When input is at low level, the relay is off, and the indicator light is on"
https://mm.digikey.com/Volume0/opasdata/d220001/medias/docus/2522/TS0012%20DATASHEET.pdf


# Adopted coding conventions

- UPPER_CASE for constants;
- camelCase for variables or functions defined by Arduino;
- snake_case for variables or functions defined by us;

*/

const unsigned long ONE_SECOND = 1000;
const unsigned long HOLD_DURATION = ONE_SECOND * 5;  // duration to hold button to register selection

// Jumper from ground to a pin configured as INPUT_PULLUP.
// Disconnect this jumper to disable panel for maintenance.
// See disable_panel() function, called from loop().
const int MAIN_JUMPER = A5;

// pins for track control
const int TRACK_BUTTON = 12;
const int TRACK_LED = 11;
const int TRACK_RELAY = 10;

// pins for DCC power source
const int DCC_BUTTON = 9;
const int DCC_LED = 8;
const int DCC_RELAY = 7;

// pins for analog power source
const int ANALOG_BUTTON = 6;
const int ANALOG_LED = 5;
const int ANALOG_RELAY = 4;

// pins for external power source
const int EXTERNAL_BUTTON = 3;
const int EXTERNAL_LED = 2;
const int EXTERNAL_RELAY = A0;  // pins 0, 1 are used as RX, TX

void set_led(int pin, bool state);
void set_relay(int pin, bool state);

void setup() {
  pinMode(MAIN_JUMPER, INPUT_PULLUP);

  // track controls
  pinMode(TRACK_BUTTON, INPUT_PULLUP);
  pinMode(TRACK_LED, OUTPUT);
  pinMode(TRACK_RELAY, OUTPUT);
  set_led(TRACK_LED, false);
  set_relay(TRACK_RELAY, false);

  // DCC source controls
  pinMode(DCC_BUTTON, INPUT_PULLUP);
  pinMode(DCC_LED, OUTPUT);
  pinMode(DCC_RELAY, OUTPUT);
  set_led(DCC_LED, true);
  set_relay(DCC_RELAY, false);

  // analog source controls
  pinMode(ANALOG_BUTTON, INPUT_PULLUP);
  pinMode(ANALOG_LED, OUTPUT);
  pinMode(ANALOG_RELAY, OUTPUT);
  set_led(ANALOG_LED, true);
  set_relay(ANALOG_RELAY, false);

  // external source controls
  pinMode(EXTERNAL_BUTTON, INPUT_PULLUP);
  pinMode(EXTERNAL_LED, OUTPUT);
  pinMode(EXTERNAL_RELAY, OUTPUT);
  set_led(EXTERNAL_LED, true);
  set_relay(EXTERNAL_RELAY, false);
}

void set_led(int pin, bool state) {
  // For leds with cathode connected to pin
  // state = true -> led ON
  // state = false -> led OFF
  digitalWrite(pin, state ? LOW : HIGH);
}

void set_relay(int pin, bool state) {
  // state = true -> N.O. output closed, led OFF
  // state = false -> N.O. output closed, led ON
  digitalWrite(pin, state ? LOW : HIGH);
}

bool switch_closed(int pin) {
  // For switches connected to pins in INPUT_PULLUP mode
  // switch closed -> return true;
  // switch open -> return false;
  return (digitalRead(pin) == HIGH) ? false : true;
}

// ATTENTION: these arrays must have the same number of items
// and the corresponding items must be in the same order!
const int LEDS[] = {TRACK_LED, EXTERNAL_LED, ANALOG_LED, DCC_LED};
const int LEN_LEDS = sizeof(LEDS) / sizeof(LEDS[0]);
const int RELAYS[] = {TRACK_RELAY, EXTERNAL_RELAY, ANALOG_RELAY, DCC_RELAY};
const int LEN_RELAYS = sizeof(RELAYS) / sizeof(RELAYS[0]);
const int BUTTONS[] = {TRACK_BUTTON, EXTERNAL_BUTTON, ANALOG_BUTTON, DCC_BUTTON};
const int LEN_BUTTONS = sizeof(BUTTONS) / sizeof(BUTTONS[0]);


enum Source {
  NO_SOURCE = -1,
  EXTERNAL_SOURCE = 1,
  ANALOG_SOURCE = 2,
  DCC_SOURCE = 3
};

void disable_panel() {
  // turn off all leds and relays
  int i;
  for (i = 0; i < LEN_RELAYS; i++) {
    digitalWrite(RELAYS[i], HIGH); 
  }
  for (i = 0; i < LEN_LEDS; i++) {
    set_led(LEDS[i], false);
  }
}

void exclusive_on(int source) {
  // turns on source led and relay
  // and turns off other sources and relays
  int i;
  for (i = 0; i < LEN_LEDS; i++) {
    if (i == source) {
      set_led(LEDS[i], true);
      set_relay(RELAYS[i], true);
    } else {
      set_led(LEDS[i], false);
      set_relay(RELAYS[i], false);
    }
  }  
}

void set_track(bool state) {
  set_led(TRACK_LED, state);
  set_relay(TRACK_RELAY, state);
}

int previous_held_button = -1;
unsigned long hold_started = 0;

int source_button() {
  // Returns pin of button held for more than HOLD_DURATION.
  // If more than one button is held, only first pin is reported.
  // Returns -1 if no button was held long enough
  int i;
  int current_held_button = -1;
  for (i = 0; i < LEN_BUTTONS; i++) {
    int candidate_button = BUTTONS[i];
    if (switch_closed(candidate_button)) {
      current_held_button = candidate_button;
      if (current_held_button == previous_held_button) {
        if ((millis() - hold_started) > HOLD_DURATION) {
          previous_held_button = -1;
          hold_started = 0;
          return current_held_button;        
        } else {
          return -1;  // same button, but not held long enough yet
        }
      } else {  //different button
        hold_started = millis();
        previous_held_button = current_held_button;
      }
      break;  // ignore further buttons after one is pressed
    } 
  }
  if (current_held_button == -1) {
    hold_started = millis();
  }
  return -1;
}


const int SOURCE_LEDS[] = {EXTERNAL_LED, ANALOG_LED, DCC_LED};
const int LEN_SOURCE_LEDS = sizeof(LEDS) / sizeof(LEDS[0]);
bool _source_leds_state = false;

bool toggle_source_leds() {
  _source_leds_state = !_source_leds_state;
  int i;
  for (i = 0; i < LEN_SOURCE_LEDS; i++) {
    set_led(SOURCE_LEDS[i], _source_leds_state);
  }
  return _source_leds_state;
}   

unsigned long _previous_toggle_time = 0;

void blink_source_leds(unsigned long interval) {

  if ((millis() - _previous_toggle_time) >= interval) {
    _previous_toggle_time = millis();  // restart timer
    toggle_source_leds();
  }

}

enum Source selected_source = NO_SOURCE;

void loop() {
  //test_leds();
  //test_relays();
  if (switch_closed(MAIN_JUMPER)) {
    switch (selected_source) {
      case NO_SOURCE:
        blink_source_leds(ONE_SECOND);
        break;
      case EXTERNAL_SOURCE:
        exclusive_on(EXTERNAL_SOURCE);
        set_led(TRACK_LED, true);
        break;
      case ANALOG_SOURCE:
        exclusive_on(ANALOG_SOURCE);
        set_led(TRACK_LED, true);
        break;
      case DCC_SOURCE:
        exclusive_on(DCC_SOURCE);
        set_led(TRACK_LED, true);
        break;
    }
  } else {
    disable_panel();
  }
}

// functions for manual testing

void test_source_button() {
  int selected = source_button();
  if (selected == -1) return;
  switch (selected) {
    case EXTERNAL_BUTTON:
      exclusive_on(EXTERNAL_SOURCE);
      break;
    case ANALOG_BUTTON:
      exclusive_on(ANALOG_SOURCE);
      break;
    case DCC_BUTTON:
      exclusive_on(DCC_SOURCE);
      break;
  }

}

void test_leds() {
  int i;
  for (i = 0; i < LEN_LEDS; i++) {
    set_led(LEDS[i], true); 
    delay(ONE_SECOND * 2);
    set_led(LEDS[i], false); 
  }
}

void test_relays() {
  int i;
  for (i = 0; i < LEN_RELAYS; i++) {
    digitalWrite(RELAYS[i], LOW); 
    delay(ONE_SECOND / 10);
    digitalWrite(RELAYS[i], HIGH); 
    delay(ONE_SECOND / 10);
  }
}

void test_button() {
  bool pressed = switch_closed(TRACK_BUTTON);
  set_led(TRACK_LED, pressed);
  set_relay(TRACK_RELAY, pressed);
}
