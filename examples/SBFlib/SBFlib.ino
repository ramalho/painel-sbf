#include "ledlib.h"

Led led(13, HIGH);

void setup() {
  led.on();
  delay(5000);
  led.off();
  delay(1000);
}

void loop() {
  led.on();
  delay(500);
  led.off();
  delay(200);
}
