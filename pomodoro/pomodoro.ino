#include  "timestamp.h"
#include  "logic.h"
#include  "debouncing.h"

static  logic       logic_;
static  timestamp   timer_;
static  debouncing  debouncing_;

static  const int   pin_button  = 14;

void setup() {
  led_init();
  logic_.init();
  timer_.reset(0);
  debouncing_.init(pin_button);
}

void loop() {
  bool  completed = logic_.update(timer_.now(), false);

  //led_setflash(logic_.waiting_for_confirmation);
  if (debouncing_.update(timer_.now())) {
    led_setflash(!led_isflash());
  }
  led_update(logic_.remain_seconds);

  if (completed) {
    logic_.reset();
    timer_.reset();
  }
}

