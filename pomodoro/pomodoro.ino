#include  <ESP8266WiFi.h>
#include  <ESP8266WebServer.h>

#include  <procedure.h>
#include  <timestamp.h>
#include  <timing.h>

#include  "hw-define.h"
#include  "logic-config.h"

static  logic_config    config_;
static  timing<>        timeout_;
static  timestamp<>     timer_;

static  bool  is_config_mode  = false;
static  void  (*func_setup)() = 0;
static  void  (*func_loop)() = 0;

void setup() {
  config_.init();
  config_.load();

  unsigned  long  start = millis();
  do {
    if (digitalRead(pin_button) == HIGH) {
      is_config_mode = true;
      break;
    }
  } while (millis() - start < 500);

  {
    func_setup  = is_config_mode  ? config_setup  : running_setup;
    func_loop   = is_config_mode  ? config_loop   : running_loop;
  }

  timer_.reset(0);
  func_setup();
}

void loop() {
  func_loop();
}

