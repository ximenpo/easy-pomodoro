
extern  void  led_init();
extern  void  led_setflash(bool flash);
extern  void  led_update(unsigned value);

extern  void      timer_init();
extern  void      timer_reset(unsigned seconds);
extern  unsigned  timer_stamp();

void setup() {
  led_init();
  timer_init();
}

void loop() {
  unsigned  stamp = timer_stamp();
  if (0 == stamp) {
    led_setflash(true);
  }
  led_update(stamp);
}

