#include <Arduino.h>
#include "debouncing.h"

void debouncing::init(int pin) {
  this->pin = pin;
  this->start_timestamp   = 0;
  this->current_timestamp = 0;

  pinMode(this->pin, INPUT);
}

bool  debouncing::update(unsigned long timestamp) {
  this->current_timestamp = timestamp;
  PROCEDURE_BEGIN_RUNLOOP(this);

  //  first trigger
  while (digitalRead(this->pin) != HIGH) {
    PROCEDURE_YIELD_(false);
  }

  //  second check
  PROCEDURE_SLEEP_(this, 50, false);
  if (digitalRead(this->pin) != HIGH) {
    PROCEDURE_STOP_(false);
  }

  //  waiting for end.
  while (digitalRead(this->pin) == HIGH) {
    PROCEDURE_YIELD_(false);
  }

  PROCEDURE_END_(true);
}

