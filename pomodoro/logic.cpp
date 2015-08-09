#include "logic.h"

// TODO:
const unsigned  x_confirm_ms    = 5 * 1000;
const unsigned  x_work_ms       = 30 * 1000;
const unsigned  x_relax_ms      = 10 * 1000;
const unsigned  x_relax_long_ms = 15 * 1000;
const unsigned  x_work_rounds   = 4;

enum  LOGIC_STAGE_TYPE {
  STAGE_NONE,
  STAGE_CONFIRM,
  STAGE_WORK,
  STAGE_RELAX,
  STAGE_RELAX_LONG,
  STAGE_DONE,
  STAGE_TYPE_SUM,
};

static  const unsigned long x_stage_seconds[STAGE_TYPE_SUM] = {
  x_work_ms,
  0,
  x_work_ms,
  x_relax_ms,
  x_relax_long_ms,
  0,
};

bool  cancel_logic::update(unsigned long timestamp, bool action) {
  PROCEDURE_BEGIN(this);

  for (;;) {
    this->confirming  = false;
    PROCEDURE_WAIT_(action, false);

    this->confirming  = true;
    PROCEDURE_WAIT_TIMEOUT_(action, this, x_confirm_ms, false);
    if (action) {
      break;
    }
  }

  PROCEDURE_END_(true);
}

void  pomodoro_logic::init() {
  this->stage = STAGE_NONE;
  this->round = 0;

  this->confirming  = false;
  this->remain_seconds  = 0;

  this->reset();
}

void  pomodoro_logic::update(unsigned long timestamp, bool action) {
  this->current_timestamp = timestamp;

  this->do_update(timestamp, action);

  this->confirming  = (this->stage == STAGE_CONFIRM) || cancel.confirming;
  switch (this->stage) {
    case STAGE_WORK:
    case STAGE_RELAX:
    case STAGE_RELAX_LONG:
      this->remain_seconds  = (x_stage_seconds[this->stage] - (timestamp - this->start_timestamp)) / 1000;
  }
}

void  pomodoro_logic::do_update(unsigned long timestamp, bool action) {
  PROCEDURE_BEGIN_RUNLOOP(this);

  this->stage = STAGE_NONE; {
    this->round = 0;
  }

  do {
    this->stage = STAGE_NONE; {
      PROCEDURE_WAIT(action);
    }

    this->stage = STAGE_CONFIRM; {
      PROCEDURE_WAIT_TIMEOUT(action, this, x_stage_seconds[this->stage]);
      if (this->check_expired(x_stage_seconds[this->stage])) {
        continue;
      }
    }

    this->stage = STAGE_WORK; {
      cancel.reset();
      PROCEDURE_WAIT_TIMEOUT(cancel.update(timestamp, action), this, x_stage_seconds[this->stage]);
      if (!this->check_expired(x_stage_seconds[this->stage])) {
        continue;
      }
    }

    this->stage = STAGE_CONFIRM; {
      PROCEDURE_WAIT(action);
    }

    if (round == x_work_rounds - 1) {
      break;
    }

    this->stage = STAGE_RELAX; {
      cancel.reset();
      PROCEDURE_WAIT_TIMEOUT(cancel.update(timestamp, action), this, x_stage_seconds[this->stage]);
    }
  } while (++round < x_work_rounds);

  this->stage = STAGE_RELAX_LONG; {
    cancel.reset();
    PROCEDURE_WAIT_TIMEOUT(cancel.update(timestamp, action), this, x_stage_seconds[this->stage]);
  }

  this->stage = STAGE_DONE; {
  }

  PROCEDURE_END();
}

