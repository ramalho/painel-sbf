#include "garoa_led.h"
#include "garoa_button.h"

Led board_led(13, HIGH);
Led line_led(11, LOW);
Button b1(12, INPUT_PULLUP);

void setup() {
  for (int i=0; i<3; i ++) {
    delay(1000);
    board_led.on();
    delay(1000);
    board_led.off();
  }
  delay(1000);
  board_led.start_blinking(1000);
}

void loop() {
  int count = board_led.update();
  if (count == 6) board_led.stop_blinking();
  if (b1.is_pressed()) line_led.on();
  else line_led.off();
}
