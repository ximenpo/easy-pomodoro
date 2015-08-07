
#include "logic.h"

void  logic::init() {
  this->start_timestamp   = 0;
  this->current_timestamp = 0;
}

// TODO:
const unsigned  x_work_ms       = 10 * 1000;
const unsigned  x_relay_ms      = 5 * 1000;
const unsigned  x_long_relay_ms = 8 * 1000;

bool  logic::update(unsigned long timestamp, bool action) {
  this->current_timestamp = timestamp;
  PROCEDURE_BEGIN_RUNLOOP(this);


  this->round = 0;
  this->waiting_for_confirmation  = false;
  this->remain_seconds  = x_work_ms / 1000;

  for (; round < 4; ++round) {
    this->start_timestamp = this->current_timestamp;
    this->remain_seconds  = x_work_ms / 1000;
    while (!this->check_expired(x_work_ms)) {
      this->remain_seconds  = (x_work_ms - (this->current_timestamp - this->start_timestamp)) / 1000;
      PROCEDURE_YIELD_(false);
    }

    if (round == 4 - 1) {
      break;
    }

    this->start_timestamp = this->current_timestamp;
    this->remain_seconds  = x_relay_ms / 1000;
    while (!this->check_expired(x_relay_ms)) {
      this->remain_seconds  = (x_relay_ms - (this->current_timestamp - this->start_timestamp)) / 1000;
      PROCEDURE_YIELD_(false);
    }
  }

  this->start_timestamp = this->current_timestamp;
  this->remain_seconds  = x_long_relay_ms / 1000;
  while (!this->check_expired(x_long_relay_ms)) {
    this->remain_seconds  = (x_long_relay_ms - (this->current_timestamp - this->start_timestamp)) / 1000;
    PROCEDURE_YIELD_(false);
  }

  PROCEDURE_END_(true);
}

