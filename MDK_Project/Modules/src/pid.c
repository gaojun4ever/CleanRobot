#include "pid.h"


void pidInit(PID_Typedef *pid, const float desired, const float kp,
             const float ki, const float kd, const float dt)
{
  pid->error     = 0;
  pid->prevError = 0;
  pid->integ     = 0;
  pid->deriv     = 0;
  pid->desired = desired;
  pid->kp = kp;
  pid->ki = ki;
  pid->kd = kd;
  pid->iLimit    = DEFAULT_PID_INTEGRATION_LIMIT;
  pid->iLimitLow = -DEFAULT_PID_INTEGRATION_LIMIT;
  pid->dt        = dt;
}

float pidCalculate(PID_Typedef *pid,float measured){
    float output;

    pid->error = pid->desired - measured;

    pid->integ += pid->error * pid->dt;
    if (pid->integ > pid->iLimit)
    {
        pid->integ = pid->iLimit;
    }
    else if (pid->integ < pid->iLimitLow)
    {
        pid->integ = pid->iLimitLow;
    }

    pid->deriv = (pid->error - pid->prevError) / pid->dt;

    pid->outP = pid->kp * pid->error;
    pid->outI = pid->ki * pid->integ;
    pid->outD = pid->kd * pid->deriv;

    output = pid->outP + pid->outI + pid->outD;

    pid->prevError = pid->error;

    return output;
}

void pidSetIntegralLimit(PID_Typedef* pid, const float limit) {
    pid->iLimit = limit;
}


void pidSetIntegralLimitLow(PID_Typedef* pid, const float limitLow) {
    pid->iLimitLow = limitLow;
}

void pidReset(PID_Typedef* pid)
{
  pid->error     = 0;
  pid->prevError = 0;
  pid->integ     = 0;
  pid->deriv     = 0;
}

void pidSetError(PID_Typedef* pid, const float error)
{
  pid->error = error;
}

void pidSetDesired(PID_Typedef* pid, const float desired)
{
  pid->desired = desired;
}

float pidGetDesired(PID_Typedef* pid)
{
  return pid->desired;
}

bool pidIsActive(PID_Typedef* pid)
{
  bool isActive = TRUE;

  if (pid->kp < 0.0001 && pid->ki < 0.0001 && pid->kd < 0.0001)
  {
    isActive = FALSE;
  }

  return isActive;
}

void pidSetKp(PID_Typedef* pid, const float kp)
{
  pid->kp = kp;
}

void pidSetKi(PID_Typedef* pid, const float ki)
{
  pid->ki = ki;
}

void pidSetKd(PID_Typedef* pid, const float kd)
{
  pid->kd = kd;
}
void pidSetDt(PID_Typedef* pid, const float dt) {
    pid->dt = dt;
}
