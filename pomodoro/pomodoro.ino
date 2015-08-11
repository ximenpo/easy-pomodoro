#include  <timestamp.h>
#include  <debouncing.h>
#include  <timing.h>

#include  "logic.h"

static  timestamp<>     timer_;
static  debouncing<>    btn_;
static  pomodoro_logic  logic_;

static  const int       pin_button  = 14;

void setup() {
  led_init();
  btn_.init(pin_button);
  logic_.init();
  timer_.reset(0);
}

void loop() {
  // update logic with tow input: time and button
  logic_.update(timer_.now(), btn_.check_click(timer_.now(), HIGH));

  // display using logic result
  /*
  if (logic_.confirming) {
    if (!led_isblink()) {
      analogWriteFreq(262);
      analogWrite(12, 1023);
    }
  } else {
    analogWrite(12, 0);
  }*/
  led_setblink(logic_.confirming);
  led_update(logic_.remain_seconds);

  // next round
  if (logic_.is_finished()) {
    logic_.reset();
    timer_.reset();
  }
}

