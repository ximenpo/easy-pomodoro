#include <procedure.h>

class  debouncing  : public procedure_context {
  public:
    void  init(int pin);
    bool  update(unsigned long timestamp);

  private:
    int   pin;

    unsigned long  start_timestamp;
    unsigned long  current_timestamp;

    bool  check_expired(unsigned long delta)const {
      return  current_timestamp >= start_timestamp + delta;
    }
};

