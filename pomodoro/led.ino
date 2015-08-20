#include <_7seg-code.h>
#include <_74HC595.h>

#define  SEVEN_SEGMENT_BIT_A    B00000010
#define  SEVEN_SEGMENT_BIT_B    B00000001
#define  SEVEN_SEGMENT_BIT_C    B01000000
#define  SEVEN_SEGMENT_BIT_D    B00010000
#define  SEVEN_SEGMENT_BIT_E    B00001000
#define  SEVEN_SEGMENT_BIT_F    B00000100
#define  SEVEN_SEGMENT_BIT_G    B10000000
#define  SEVEN_SEGMENT_BIT_DOT  B00100000

static  const unsigned int led_number_code[]  = {
  CA7S_CODE(0),
  CA7S_CODE(1),
  CA7S_CODE(2),
  CA7S_CODE(3),
  CA7S_CODE(4),
  CA7S_CODE(5),
  CA7S_CODE(6),
  CA7S_CODE(7),
  CA7S_CODE(8),
  CA7S_CODE(9),
};

static  const int led_pins[]      = {16, 4, 5, 0};
static  const int led_xmod[]      = {6000, 600, 60, 10};
static  const int led_xdiv[]      = {600, 60, 10, 1};
static  const X_74HC595 led_ctrl  = {{13, 2, 15, MSBFIRST}};

static  bool  led_blink     = false;
static  unsigned  led_index = 0;

void  led_init() {
  for (int i = 0; i < sizeof(led_pins) / sizeof(led_pins[0]); ++i) {
    pinMode(led_pins[i], OUTPUT);
  }
  led_ctrl.init();
}

bool  led_isblink() {
  return  led_blink;
}

void  led_setblink(bool blink) {
  led_blink = blink;
}

void  led_update_code(unsigned  code) {
  unsigned  old_index  = (led_index) % (sizeof(led_pins) / sizeof(led_pins[0]));
  unsigned  index      = (++led_index) % (sizeof(led_pins) / sizeof(led_pins[0]));

  if (led_blink) {
    // hide 0.5 seconds in flash mode
    if ( (millis() / 500 % 2 == 0)) {
      digitalWrite(led_pins[index], LOW);
      return;
    }
  }

  // display clock code
  digitalWrite(led_pins[old_index], LOW);
  _74HC595_BEGIN(led_ctrl.data);
  _74HC595_WRITE(led_ctrl.data, code);
  _74HC595_END(led_ctrl.data);
  digitalWrite(led_pins[index], HIGH);
}

void  led_config_update() {
  unsigned  code    = CA7S_CODE_WITH_DOT(CA7S_CODE(MINUS));
  led_update_code(code);
}

void  led_update(unsigned value) {
  unsigned  index   = (led_index + 1) % (sizeof(led_pins) / sizeof(led_pins[0]));
  unsigned  number  = value % led_xmod[index] / led_xdiv[index];
  unsigned  code    = CA7S_CODE_WITH_DOT(led_number_code[number]);
  led_update_code(code);
}

