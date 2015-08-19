#include  <stddef.h>
#include  <stdint.h>

#include  "config.h"

size_t  storage_load(uint8_t* begin, uint8_t* end);
void    storage_save(uint8_t* begin, uint8_t* end);

void Config::init() {
  this->work_minites;
  this->break_minites;
  this->long_break_minites;
  this->long_break_work_times;
}

void Config::load() {
  storage_load(&this->__storage_begin__, &this->__storage_end__);
}

void Config::save() {
  storage_save(&this->__storage_begin__, &this->__storage_end__);
}

