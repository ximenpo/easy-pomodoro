#include  <timestamp.h>
#include  <debouncing.h>

#include  "logic.h"

static  timestamp<>     timer_;
static  debouncing<>    button_;
static  pomodoro_logic  logic_;

static  const int       pin_button  = 14;

void setup() {
  led_init();
  button_.init(pin_button, HIGH);
  logic_.init();
  timer_.reset(0);
}

void loop() {
  // update logic with tow input: time and button
  logic_.update(timer_.now(), button_.update(timer_.now()));

  // display using logic result
  led_setblink(logic_.confirming);
  led_update(logic_.remain_seconds);

  // next round
  if (logic_.is_finished()) {
    logic_.reset();
    timer_.reset();
  }
}

