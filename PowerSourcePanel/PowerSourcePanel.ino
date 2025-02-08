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

enum class PowerSource {
  None,
  External,
  Digital,
  Analog,
  Wifi
};

const unsigned long SEC = 1000;  // 1s in milliseconds
const unsigned long SELECTION_DELAY = 5 * SEC;  // duration to hold button to confirm source selection
const unsigned long STANDBY_BLINK_DELAY = SEC / 2;
const unsigned long SHORT_CIRCUIT_BLINK_DELAY = SEC / 5;

// Jumper from ground to a pin configured as INPUT_PULLUP.
// Disconnect this jumper to disable panel for maintenance.
Jumper main_jumper(A5, INPUT_PULLUP);

// Signal from power source: LOW means short circuit detected.
Jumper short_circuit(A4, INPUT_PULLUP);

Button track_button(12, LOW);
OutputOnOff track_led(11, LOW);
OutputOnOff track_relay(10, LOW);

Button digital_button(9, LOW);
OutputOnOff digital_led(8, LOW);
OutputOnOff digital_relay(7, LOW);

Button analog_button(6, LOW);
OutputOnOff analog_led(1, LOW);
OutputOnOff analog_relay(4, LOW);

Button external_button(3, LOW);
OutputOnOff external_led(0, LOW);
OutputOnOff external_relay(A0, LOW);

// wifi_button is also used as reverse direction button when AnalogSelected
Button wifi_button(A2, LOW);  
OutputOnOff wifi_led(2, LOW);
OutputOnOff wifi_relay(A3, LOW);

const int LEN_SOURCES = 3;
OutputOnOff* source_leds[LEN_SOURCES] = { &external_led, &analog_led, &digital_led };
Button* source_buttons[LEN_SOURCES] = { &external_button, &analog_button, &digital_button };

const int LEN_ALL_RELAYS = 5;
OutputOnOff* all_relays[LEN_ALL_RELAYS] = { &track_relay, &external_relay, &analog_relay, &wifi_relay, &digital_relay };

Mode current_mode = Mode::StandBy;
Mode previous_mode = Mode::Unset;
PowerSource selected_source = PowerSource::None;

void turn_off_all_relays() {
  for (int i = 0; i < LEN_ALL_RELAYS; i++) {
    all_relays[i]->turn_off();
  }
}

void setup() {
  turn_off_all_relays();
}

void configure_standby() {
  selected_source = PowerSource::None;
  turn_off_all_relays();
  for (int i = 0; i < LEN_SOURCES; i++) {
    source_leds[i]->turn_on();
    source_leds[i]->start_cycling(STANDBY_BLINK_DELAY);
  }
  wifi_led.turn_off();
}

void configure_external_selected() {
  selected_source = PowerSource::External;
  turn_off_all_relays();
  for (int i = 0; i < LEN_SOURCES; i++) {
    source_leds[i]->stop_cycling();
  }
  external_led.turn_on();
  external_relay.turn_on();
}

void configure_analog_selected() {
  selected_source = PowerSource::Analog;
  turn_off_all_relays();
  for (int i = 0; i < LEN_SOURCES; i++) {
    source_leds[i]->stop_cycling();
  }
  analog_led.turn_on();
  analog_relay.turn_on();
}

void configure_wifi_selected() {
  selected_source = PowerSource::Wifi;
  turn_off_all_relays();
  for (int i = 0; i < LEN_SOURCES; i++) {
    source_leds[i]->stop_cycling();
  }
  analog_led.turn_on();
  wifi_led.turn_on();
  wifi_relay.turn_on();
}

void configure_digital_selected() {
  selected_source = PowerSource::Digital;
  turn_off_all_relays();
  for (int i = 0; i < LEN_SOURCES; i++) {
    source_leds[i]->stop_cycling();
  }
  digital_led.turn_on();
  digital_relay.turn_on();
}

void activate_track() {
  track_led.turn_on();
  track_relay.turn_on();
}

void deactivate_track() {
  track_led.turn_off();
  track_relay.turn_off();
}

void update_controls() {
  for (int i = 0; i < LEN_SOURCES; i++) {
    source_leds[i]->update();
  }
  track_led.update();
  for (int i = 0; i < LEN_SOURCES; i++) {
    source_buttons[i]->update();
  }
  wifi_button.update();
  track_button.update();
  for (int i = 0; i < LEN_ALL_RELAYS; i++) {
    all_relays[i]->update();
  }
}

bool panel_enabled() {
  if (main_jumper.is_closed()) return true;
  else {
    turn_off_all_relays();
    for (int i = 0; i < LEN_SOURCES; i++) {
      source_leds[i]->turn_off();
    }
    track_led.turn_off();
    wifi_led.turn_off();
  }
  return false;
}

Mode restore_previous_mode(PowerSource selected_source) {
  switch (selected_source) {
    case PowerSource::External:
      return Mode::ExternalSelected;
    case PowerSource::Analog:
      return Mode::AnalogSelected;
    case PowerSource::Wifi:
      return Mode::WifiSelected;
    case PowerSource::Digital:
      return Mode::DigitalSelected;
  }
}

void loop() {
  if (! panel_enabled()) return;

  update_controls();
  switch (current_mode) {
    case Mode::StandBy: {
      if (previous_mode != Mode::StandBy) {
        configure_standby();
        previous_mode = Mode::StandBy;
      }
      if (external_button.is_held(SELECTION_DELAY)) current_mode = Mode::ExternalSelected;
      else if (analog_button.is_held(SELECTION_DELAY)) current_mode = Mode::AnalogSelected;
      else if (wifi_button.is_held(SELECTION_DELAY)) current_mode = Mode::WifiSelected;  
      else if (digital_button.is_held(SELECTION_DELAY)) current_mode = Mode::DigitalSelected;  
      break;
    }
    case Mode::ExternalSelected: {
      if (previous_mode != Mode::ExternalSelected) {
        configure_external_selected();
        previous_mode = Mode::ExternalSelected;
      }
      if (external_button.just_pressed()) current_mode = Mode::StandBy;
      else if (track_button.just_pressed()) current_mode = Mode::TrackActive; 
      break;
    }  
    case Mode::AnalogSelected: {
      if (previous_mode != Mode::AnalogSelected) {
        configure_analog_selected();
        previous_mode = Mode::AnalogSelected;
      }
      if (analog_button.just_pressed()) current_mode = Mode::StandBy;
      else if (track_button.just_pressed()) current_mode = Mode::TrackActive; 
      break;
    }  
    case Mode::WifiSelected: {
      if (previous_mode != Mode::WifiSelected) {
        configure_wifi_selected();
        previous_mode = Mode::WifiSelected;
      }
      if (analog_button.just_pressed()) current_mode = Mode::StandBy;
      else if (track_button.just_pressed()) current_mode = Mode::TrackActive; 
      break;
    }  
    case Mode::DigitalSelected: {
      if (previous_mode != Mode::DigitalSelected) {
        configure_digital_selected();
        previous_mode = Mode::DigitalSelected;
      }
      if (digital_button.just_pressed()) current_mode = Mode::StandBy;
      else if (track_button.just_pressed()) current_mode = Mode::TrackActive; 
      break;
    }  
    case Mode::TrackActive: {
      if (previous_mode != Mode::TrackActive) {
        activate_track();
        previous_mode = Mode::TrackActive;
      }
      if (track_button.just_pressed()) {
        deactivate_track();
        current_mode = restore_previous_mode(selected_source);
      } else if (short_circuit.is_closed()) current_mode = Mode::TrackProtected;
      break;
    }
    case Mode::TrackProtected: {
      if (previous_mode != Mode::TrackProtected) {
        turn_off_all_relays();
        track_led.start_cycling(SHORT_CIRCUIT_BLINK_DELAY);
        previous_mode = Mode::TrackProtected;
      };
      if (! short_circuit.is_closed()) {
        track_led.stop_cycling();
        current_mode = restore_previous_mode(selected_source);
      }
      break;
    }
  }  // switch (current_mode)

  delay(10);  // "Add a small debouncing delay" --@ladyada
}