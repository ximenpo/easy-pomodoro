#include  <timestamp.h>
#include  <debouncing.h>
#include  <timing.h>

#include  "logic.h"

static  timestamp<>     timer_;
static  debouncing<>    btn_;
static  pomodoro_logic  logic_;

static  timing<>        timing_confirm_;
static  timing<>        timing_alert_;

static  const int       pin_button  = 14;
static  const int       pin_alert   = 12;

void setup() {
  led_init();
  btn_.init(pin_button);
  logic_.init();
  timer_.reset(0);

  pinMode(pin_alert, OUTPUT);
  timing_confirm_.init();
  timing_alert_.init();

  timing_confirm_.done();
  timing_alert_.done();
}

void loop() {
  // update logic with tow input: time and button
  unsigned long timestamp = timer_.now();
  logic_.update(timestamp, btn_.check_click(timestamp, HIGH));

  // display using logic result
  if (logic_.confirming) {
    static  bool  alert_status  = false;
    if (!led_isblink()) {
      // begin alert
      if (logic_.remain_seconds == 0) {
        alert_status  = true;
        timing_alert_.reset();
      } else {
        timing_confirm_.reset();
      }
      digitalWrite(pin_alert, HIGH);
    }
    // check & stop alert
    if (timing_alert_.check_periodic(timestamp, 500, 10)) {
      alert_status  = !alert_status;
      //digitalWrite(pin_alert, alert_status ? HIGH : LOW);
    }
    if (timing_confirm_.check_timeout(timestamp, 1000)) {
      digitalWrite(pin_alert, LOW);
    }
  } else {
    digitalWrite(pin_alert, LOW);
  }
  led_setblink(logic_.confirming);
  led_update(logic_.remain_seconds);

  // next round
  if (logic_.is_done()) {
    logic_.reset();
    timer_.reset();
  }
}

