#include "sbflib.h"

Led led(13, HIGH);

void setup() {
  for (int i=0; i<3; i ++) {
    delay(1000);
    led.on();
    delay(1000);
    led.off();
  }
  delay(1000);
  led.start_blinking(100);
}

void loop() {
  int count = led.update();
  if (count == 4) led.stop_blinking();
}
