#include  "timestamp.h"
#include  "logic.h"
#include  "debouncing.h"

static  timestamp<>     timer_;
static  pomodoro_logic  logic_;

static  debouncing      debouncing_;
static  const int       pin_button  = 14;

void setup() {
  led_init();
  logic_.init();
  timer_.reset(0);
  debouncing_.init(pin_button);
}

void loop() {
  logic_.update(timer_.now(), false);

  //led_setflash(logic_.confirming);
  if (debouncing_.update(timer_.now())) {
    led_setblink(!led_isblink());
  }
  led_update(logic_.remain_seconds);

  if (logic_.is_finished()) {
    logic_.reset();
    timer_.reset();
  }
}

