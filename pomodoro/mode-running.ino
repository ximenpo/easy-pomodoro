
#include  <debouncing.h>

#include  "hw-define.h"
#include  "logic.h"

static  debouncing<>    btn_;
static  pomodoro_logic  logic_;

static  timing<>        buzz_;

static  bool      alerting    = false;

void  running_setup() {
  WiFi.mode(WIFI_STA);

  pinMode(pin_buzzer, OUTPUT);
  digitalWrite(pin_buzzer, LOW);

  led_init();
  btn_.init(pin_button);
  logic_.init(config_);

  timeout_.init();
  timeout_.done();
  
  buzz_.init();
  buzz_.done();
}

void  running_loop() {
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
        buzz_.reset();
      } else {
        timeout_.reset();
      }
      alerting  = true;
      digitalWrite(pin_buzzer, HIGH);
    }
    // check & stop alert
    if (buzz_.check_periodic(timestamp, 500, 11)) {
      alert_status  = !alert_status;
      digitalWrite(pin_buzzer, alert_status ? HIGH : LOW);
    }
    if (timeout_.check_timeout(timestamp, 1000)) {
      alerting  = false;
      digitalWrite(pin_buzzer, LOW);
    }
  } else if (alerting) {
    digitalWrite(pin_buzzer, LOW);
    alerting  = false;
    timeout_.done();
    buzz_.done();
  }

  led_setblink(logic_.confirming);
  led_update(logic_.remain_seconds);

  // next round
  if (logic_.is_done()) {
    logic_.reset();
    timer_.reset();
  }
}

