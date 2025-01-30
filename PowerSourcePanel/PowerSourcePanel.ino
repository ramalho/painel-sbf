
#include <Array.h>
#include "garoa_digital_io.h"
#include "garoa_button.h"

enum class Mode {
    StandBy,           // initial mode: source LEDs blinking, all relays off;
    ExternalSelected,  // only external_led and external_relay on;
    DigitalSelected,   // only digital_led and digital_relay on;
    AnalogSelected,    // only analog_led and analog_relay on;
    WifiSelected,      // wifi_led and analog_led on, only wifi_relay on;
    TrackActive,       // track_led on, as well as led and relay of source selection;
    TrackProtected,    // track_led blinking, led of source selection on, all relays off; 
    Unset              // used only as initial value for previous_mode;
};

const unsigned long SEC = 1000;  // 1s in milliseconds
const unsigned long HOLD_DURATION = 5 * SEC;  // duration to hold button to confirm source selection

// Jumper from ground to a pin configured as INPUT_PULLUP.
// Disconnect this jumper to disable panel for maintenance.
// See disable_panel() function, called from loop().
const int MAIN_JUMPER = A5;

Button track_button(12, LOW);
OutputOnOff track_led(11, LOW);
OutputOnOff track_relay(10, LOW);

Button digital_button(9, LOW);
OutputOnOff digital_led(8, LOW);
OutputOnOff digital_relay(7, LOW);

Button analog_button(6, LOW);
OutputOnOff analog_led(5, LOW);
OutputOnOff analog_relay(4, LOW);

Button external_button(3, LOW);
OutputOnOff external_led(2, LOW);
OutputOnOff external_relay(A0, LOW);


const unsigned long STANDBY_BLINK_DELAY = SEC / 2;

const int LEN_SOURCES = 3;
static Array<OutputOnOff*,LEN_SOURCES> source_leds;
static Array<Button*,LEN_SOURCES> source_buttons;

void setup() {
  pinMode(MAIN_JUMPER, INPUT_PULLUP);

  source_leds.push_back(&external_led);
  source_leds.push_back(&analog_led);
  source_leds.push_back(&digital_led);

  source_buttons.push_back(&external_button);
  source_buttons.push_back(&analog_button);
  source_buttons.push_back(&digital_button);
}

void configure_standby() {
  for (OutputOnOff *led : source_leds) led->start_cycling(STANDBY_BLINK_DELAY);
}

void configure_external_selected() {
  for (OutputOnOff *led : source_leds) led->stop_cycling();
  external_led.turn_on();
  track_led.turn_on();
}

void update_controls() {
  for (OutputOnOff *led : source_leds) led->update();
  for (Button *button : source_buttons) button->update();
}

Mode current_mode = Mode::StandBy;
Mode previous_mode = Mode::Unset;

void loop() {
  update_controls();
  // if (external_button.just_pressed()) {
  //   external_led.toggle();
  // }

  switch (current_mode) {
    case Mode::StandBy: {
      if (previous_mode != Mode::StandBy) {
        configure_standby();
        previous_mode = Mode::StandBy;
      }
      if (external_button.is_held(5 * SEC)) {
        current_mode = Mode::ExternalSelected;
      } 
      break;
    }
    case Mode::ExternalSelected: {
      configure_external_selected();
      break;
    }  
  }

  delay(10);  // "Add a small debouncing delay" --@ladyada
}

