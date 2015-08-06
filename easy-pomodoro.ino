
extern  void  led_init();
extern  void  led_setflash(bool flash);
extern  void  led_update(unsigned int value);

void setup() {
  led_init();
}

void loop() {
  led_update(millis() / 1000);
}

