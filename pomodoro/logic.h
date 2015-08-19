#ifndef _POMODORO_LOGIC_H_
#define _POMODORO_LOGIC_H_

#include  <procedure.h>
#include  <timestamp.h>

class logic_config;

class cancel_logic : public procedure_context, timestamp_timing<> {
  public:
    bool    confirming;

  public:
    void    reset();
    bool    update(unsigned long timestamp, bool action);
};

class pomodoro_logic : public procedure_context, timestamp_timing<> {
  public:
    void    init(const logic_config& cfg);
    void    update(unsigned long timestamp, bool action);

  public:
    bool      confirming;
    unsigned  remain_seconds;

  private:
    int       stage;
    int       round;
    void      do_update(unsigned long timestamp, bool action);

    cancel_logic  cancel;
};

#endif
