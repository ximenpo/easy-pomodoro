#include <procedure.h>

class  logic  : public procedure_context {
  public:
    void  init();
    bool  update(unsigned long timestamp, bool action);

  public:
    bool          waiting_for_confirmation;
    unsigned      remain_seconds;

  private:
    int           round;

    unsigned long  start_timestamp;
    unsigned long  current_timestamp;

    bool  check_expired(unsigned long delta)const {
      return  current_timestamp >= start_timestamp + delta;
    }
};

