#include <Ticker.h>

static  Ticker  ticker;

static  unsigned  timer_remain;
static  unsigned  long  timer_begin;

static  void  timer_update() {
  unsigned long stamp = millis();
  if(curr_ms
  timer_remain  = ;
}

void  timer_init() {
  timer_remain  = 0;
  timer_begin = millis();

  ticker.attach(0.2, );
}

void  timer_reset(unsigned seconds) {
  timer_remain  = seconds;
  timer_begin   = millis();
}

unsigned  timer_stamp() {
  return  timer_remain;
}

