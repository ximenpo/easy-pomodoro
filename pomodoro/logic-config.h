#ifndef _POMODORO_CONFIG_H_
#define _POMODORO_CONFIG_H_

#include  <cstdint>

class  logic_config {
  public:
    uint8_t   __storage_begin__;

    char      ap_ssid[32];          // max 31 bytes
    char      ap_password[64];      // max 63 bytes

    char      wifi_ssid[32];        // max 31 bytes
    char      wifi_password[64];    // max 63 bytes
    
    char      app_id[32];           // max 31 bytes
    char      app_key[64];          // max 63 bytes

    uint8_t   work_minites;
    uint8_t   break_minites;
    uint8_t   long_break_minites;
    uint8_t   long_break_work_times;

    uint8_t   confirm_seconds;

    uint8_t   slient_mode;          // slient mode, no buzz, blink only

    uint8_t   __storage_end__;

  public:
    void      init();
    void      load();
    void      save();
} __attribute__((__packed__));

#endif

