#include  <procedure.h>
#include  "hw-define.h"

static  bool  is_config_mode  = false;
static  void  (*func_setup)() = 0;
static  void  (*func_loop)() = 0;

void setup() {
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

  func_setup();
}

void loop() {
  func_loop();
}

