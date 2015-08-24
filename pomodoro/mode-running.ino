
#include  <debouncing.h>

#include  "logic.h"

static  debouncing<>    btn_;
static  pomodoro_logic  logic_;

void  running_setup() {
  WiFi.mode(WIFI_STA);

  buzzer_init();

  led_init();
  btn_.init(pin_button);
  logic_.init(config_);

  timeout_.init();
  timeout_.done();

}

void  running_loop() {
  // update logic with tow input: time and button
  uint32_t timestamp = timer_.now();
  logic_.update(timestamp, btn_.check_click(timestamp, HIGH));

  // buzz
  if (!logic_.confirming) {
    buzzer_stop();
  } else if (led_isblink()) {
    buzzer_update(timestamp);
  } else {
    // begin alert
    (logic_.remain_seconds == 0) ? buzzer_start(true, 5000) : buzzer_start(false, 800);
  }

  // display using logic result
  led_setblink(logic_.confirming);
  led_update(logic_.remain_seconds);

  // next round
  if (logic_.is_done()) {
    logic_.reset();
    timer_.reset();
  }
}

