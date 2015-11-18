#include  <stddef.h>
#include  <stdint.h>
#include  <string.h>

#include  "logic-config.h"

size_t  storage_load(uint8_t* begin, uint8_t* end);
void    storage_save(uint8_t* begin, uint8_t* end);

void logic_config::init() {
  const char DEFAULT_SSID[]        = "pomodoro";
  const char DEFAULT_PASSWORDD[]   = "88888888";

  memcpy(this->ap_ssid, DEFAULT_SSID, sizeof(DEFAULT_SSID));
  memcpy(this->ap_password, DEFAULT_PASSWORDD, sizeof(DEFAULT_PASSWORDD));

  memset(this->wifi_ssid, 0, sizeof(this->wifi_ssid));
  memset(this->wifi_password, 0, sizeof(this->wifi_password));

  memset(this->app_id, 0, sizeof(this->app_id));
  memset(this->app_key, 0, sizeof(this->app_key));

  this->work_minites          = 25;
  this->break_minites         = 5;
  this->long_break_minites    = 15;
  this->long_break_work_times = 4;

  this->confirm_seconds       = 5;
  this->slient_mode           = 0;
}

void logic_config::load() {
  if (!storage_load(&this->__storage_begin__, &this->__storage_end__)) {
    this->init();
  }
}

void logic_config::save() {
  storage_save(&this->__storage_begin__, &this->__storage_end__);
}
