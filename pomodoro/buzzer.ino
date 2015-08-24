
static  timing<>  buzzer_;
static  timing<>  buzzer_timeout_;

static  bool      alerting      = false;
static  bool      alert_status  = false;

static  uint32_t  alert_ms      = 0;
static  bool      alert_short   = false;

static  const uint16_t  alert_short_ms  = 500;

void buzzer_init() {
  pinMode(pin_buzzer, OUTPUT);
  digitalWrite(pin_buzzer, LOW);

  buzzer_.init();
  buzzer_.done();

  buzzer_timeout_.init();
  buzzer_timeout_.done();
}

void  buzzer_start(bool short_alert, uint32_t buzzer_ms) {
  alert_ms      = buzzer_ms;
  alert_short   = short_alert;

  if (config_.slient_mode) {
    return;
  }

  buzzer_.reset();
  buzzer_timeout_.reset();
  alerting  = true;
  alert_status  = true;
  digitalWrite(pin_buzzer, HIGH);
}

void  buzzer_stop() {
  if (alerting) {
    alerting  = false;
    digitalWrite(pin_buzzer, LOW);
    buzzer_.done();
    buzzer_timeout_.done();
  }
}

void  buzzer_update(uint32_t timestamp) {
  if (!alerting || config_.slient_mode) {
    return;
  }

  // check & stop alert
  if (alert_short) {
    if (buzzer_.check_periodic(timestamp, alert_short_ms)) {
      alert_status  = !alert_status;
      digitalWrite(pin_buzzer, alert_status ? HIGH : LOW);
    }
  }

  if (buzzer_timeout_.check_timeout(timestamp, alert_ms)) {
    buzzer_stop();
  }
}

