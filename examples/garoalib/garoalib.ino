#include "garoa_led.h"
#include "garoa_button.h"

const unsigned long SECOND = 1000;  // one second in milliseconds

Led board_led(13, HIGH);
Led track_led(11, LOW);
Button track_btn(12, LOW);
Led dcc_led(8, LOW);
Button dcc_btn(9, LOW);

void setup() {
  delay(1000);
  board_led.start_blinking(500);
}

void loop() {
  //onboard LED
  int count = board_led.update();

  track_btn.update();
  track_led.update();
  dcc_btn.update();
  dcc_led.update();
  if (track_btn.is_pressed()) track_led.on();
  else track_led.off();

  if (dcc_led.lit()) {
    if (dcc_btn.just_pressed()) dcc_led.off();
  }
  else {    
    if (dcc_btn.is_held(5*SECOND)) dcc_led.on();
  }
  delay(10); // ""
}
