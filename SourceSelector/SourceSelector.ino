
/*
SourceSelector.ino

Code for a power source selector panel for the SBF model railroad layout

Adopted coding conventions:

- UPPER_CASE for constants;
- camelCase for variables or functions defined by Arduino;
- snake_case for variables or functions defined by us;

*/

const unsigned long ONE_SECOND = 1000;

// controls for the track
const int TRACK_BUTTON = 12;
const int TRACK_LED = 11;
const int TRACK_RELAY = 10;

// controls for DCC power source
const int DCC_BUTTON = 9;
const int DCC_LED = 8;
const int DCC_RELAY = 7;

// controls for analog power source
const int ANALOG_BUTTON = 6;
const int ANALOG_LED = 5;
const int ANALOG_RELAY = 4;

// controls for external power source
const int EXTERNAL_BUTTON = 3;
const int EXTERNAL_LED = 2;
const int EXTERNAL_RELAY = A0;  // pins 0, 1 are used as RX, TX

void setup() {
  // track controls
  pinMode(TRACK_LED, OUTPUT);
  pinMode(TRACK_BUTTON, INPUT_PULLUP);
  pinMode(TRACK_RELAY, OUTPUT);
  digitalWrite(TRACK_RELAY, LOW);

  // DCC source controls
  pinMode(DCC_LED, OUTPUT);
  pinMode(DCC_BUTTON, INPUT_PULLUP);
  pinMode(DCC_RELAY, OUTPUT);
  digitalWrite(DCC_RELAY, LOW);

  // analog source controls
  pinMode(ANALOG_LED, OUTPUT);
  pinMode(ANALOG_BUTTON, INPUT_PULLUP);
  pinMode(ANALOG_RELAY, OUTPUT);
  digitalWrite(ANALOG_RELAY, LOW);

  // external source controls
  pinMode(EXTERNAL_LED, OUTPUT);
  pinMode(EXTERNAL_BUTTON, INPUT_PULLUP);
  pinMode(EXTERNAL_RELAY, OUTPUT);
  digitalWrite(EXTERNAL_RELAY, LOW);
}

const int LEDS[] = {TRACK_LED, DCC_LED, ANALOG_LED, EXTERNAL_LED};
const int LEN_LEDS = sizeof(LEDS) / sizeof(LEDS[0]);
const int RELAYS[] = {TRACK_RELAY, DCC_RELAY, ANALOG_RELAY, EXTERNAL_RELAY};
const int LEN_RELAYS = sizeof(RELAYS) / sizeof(RELAYS[0]);

void test_leds() {
  int i;
  for (i = 0; i < LEN_LEDS; i++) {
    digitalWrite(LEDS[i], LOW); 
    delay(ONE_SECOND / 10);
    digitalWrite(LEDS[i], HIGH); 
    delay(ONE_SECOND / 10);
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


void loop() {
  test_leds();
  //test_relays();
}
