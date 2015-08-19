#ifndef _POMODORO_CONFIG_H_
#define _POMODORO_CONFIG_H_

#include  <cstdint>

struct  Config {
  public:
    uint8_t   __storage_begin__;
    uint8_t   work_minites;
    uint8_t   break_minites;
    uint8_t   long_break_minites;
    uint8_t   long_break_work_times;
    uint8_t   __storage_end__;

  public:
    void      init();
    void      load();
    void      save();
} __attribute__((__packed__));

#endif

