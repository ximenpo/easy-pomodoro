#include  "logic.h"
#include  "logic-config.h"

enum  LOGIC_STAGE_TYPE {
  STAGE_NONE,
  STAGE_CONFIRM,
  STAGE_WORK,
  STAGE_RELAX,
  STAGE_RELAX_LONG,
  STAGE_DONE,
  STAGE_TYPE_SUM,
};

static  unsigned long g_confirm_ms  = 5 * 1000;
static  unsigned      g_long_break_rounds = 4;
static  unsigned long g_stage_ms[STAGE_TYPE_SUM] = {0, 0, 0, 0, 0, 0,};

void  cancel_logic::reset() {
  procedure_context::reset();
  this->confirming  = false;
}
bool  cancel_logic::update(unsigned long timestamp, bool action) {
  this->current_timestamp = timestamp;

  PROCEDURE_BEGIN_RUNLOOP(this);

  this->confirming  = false;
  PROCEDURE_WAIT_(action, false);

  this->confirming  = true;
  PROCEDURE_WAIT_TIMEOUT_(action, this, g_confirm_ms, false);
  this->confirming  = false;
  if (!action) {
    PROCEDURE_STOP_(false);
  }

  PROCEDURE_END_(true);
}

void  pomodoro_logic::init(const logic_config& cfg) {
  g_stage_ms[STAGE_NONE]       = cfg.work_minites       * 60  * 1000;
  g_stage_ms[STAGE_CONFIRM]    = cfg.confirm_seconds          * 1000;
  g_stage_ms[STAGE_WORK]       = cfg.work_minites       * 60  * 1000;
  g_stage_ms[STAGE_RELAX]      = cfg.break_minites      * 60  * 1000;
  g_stage_ms[STAGE_RELAX_LONG] = cfg.long_break_minites * 60  * 1000;
  g_stage_ms[STAGE_DONE]       = cfg.work_minites       * 60  * 1000;

  g_confirm_ms        = cfg.confirm_seconds * 1000;
  g_long_break_rounds = cfg.long_break_work_times;

  this->stage = STAGE_NONE;
  this->round = 0;

  this->confirming  = false;
  this->remain_seconds  = 0;

  this->reset();
}

void  pomodoro_logic::update(unsigned long timestamp, bool action) {
  this->current_timestamp = timestamp;

  this->do_update(timestamp, action);

  switch (this->stage) {
    case STAGE_WORK:
    case STAGE_RELAX:
    case STAGE_RELAX_LONG:
      this->confirming  = cancel.confirming;
      this->remain_seconds  = (g_stage_ms[this->stage] - (timestamp - this->start_timestamp)) / 1000;
      break;
    case STAGE_CONFIRM:
      this->confirming  = true;
      break;
    default:
      this->confirming  = cancel.confirming;
      this->remain_seconds  = g_stage_ms[this->stage] / 1000;
  }
}

void  pomodoro_logic::do_update(unsigned long timestamp, bool action) {
  PROCEDURE_BEGIN_RUNLOOP(this);

  this->stage = STAGE_NONE; {
    this->round = 0;
    this->cancel.reset();
  }

  while (round < g_long_break_rounds) {
    this->stage = STAGE_NONE; {
      PROCEDURE_WAIT(action);
    }

    this->stage = STAGE_CONFIRM; {
      PROCEDURE_WAIT_TIMEOUT(action, this, g_stage_ms[this->stage]);
      if (this->check_expired(g_stage_ms[this->stage])) {
        continue;
      }
    }

    this->stage = STAGE_WORK; {
      cancel.reset();
      PROCEDURE_WAIT_TIMEOUT(cancel.update(timestamp, action), this, g_stage_ms[this->stage]);
      cancel.reset();
      if (!this->check_expired(g_stage_ms[this->stage])) {
        continue;
      }
    }

    this->stage = STAGE_CONFIRM; {
      PROCEDURE_WAIT(action);
    }

    if (round == g_long_break_rounds - 1) {
      break;
    }

    this->stage = STAGE_RELAX; {
      cancel.reset();
      PROCEDURE_WAIT_TIMEOUT(cancel.update(timestamp, action), this, g_stage_ms[this->stage]);
      cancel.reset();
    }

    ++round;
  }

  this->stage = STAGE_RELAX_LONG; {
    cancel.reset();
    PROCEDURE_WAIT_TIMEOUT(cancel.update(timestamp, action), this, g_stage_ms[this->stage]);
    cancel.reset();
  }

  this->stage = STAGE_DONE; {
    PROCEDURE_YIELD();
  }

  PROCEDURE_END();
}

