#include  <stddef.h>
#include  <stdint.h>

#include  "logic-config.h"

size_t  storage_load(uint8_t* begin, uint8_t* end);
void    storage_save(uint8_t* begin, uint8_t* end);

void logic_config::init() {
  this->confirm_seconds       = 5;
  this->work_minites          = 25;
  this->break_minites         = 5;
  this->long_break_minites    = 15;
  this->long_break_work_times = 4;
}

void logic_config::load() {
  storage_load(&this->__storage_begin__, &this->__storage_end__);
}

void logic_config::save() {
  storage_save(&this->__storage_begin__, &this->__storage_end__);
}

