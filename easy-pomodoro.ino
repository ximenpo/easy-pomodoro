
extern  void  led_init();
extern  void  led_setflash(bool flash);
extern  void  led_update(unsigned int value);

void setup() {
  // put your setup code here, to run once:
  led_init();
}

void loop() {
  // put your main code here, to run repeatedly:
  led_update(millis() / 1000);
}

