#include "config.h"

#define PID_ROLL_RATE_KP  70.0
#define PID_ROLL_RATE_KI  0.0
#define PID_ROLL_RATE_KD  0.0
#define PID_ROLL_RATE_INTEGRATION_LIMIT    100.0

#define PID_PITCH_RATE_KP  70.0
#define PID_PITCH_RATE_KI  0.0
#define PID_PITCH_RATE_KD  0.0
#define PID_PITCH_RATE_INTEGRATION_LIMIT   100.0

#define PID_YAW_RATE_KP  50.0
#define PID_YAW_RATE_KI  25.0
#define PID_YAW_RATE_KD  0.0
#define PID_YAW_RATE_INTEGRATION_LIMIT     500.0

#define PID_ROLL_KP  3.5
#define PID_ROLL_KI  2.0
#define PID_ROLL_KD  0.0
#define PID_ROLL_INTEGRATION_LIMIT    20.0

#define PID_PITCH_KP  3.5
#define PID_PITCH_KI  2.0
#define PID_PITCH_KD  0.0
#define PID_PITCH_INTEGRATION_LIMIT   20.0

#define PID_YAW_KP  0.0
#define PID_YAW_KI  0.0
#define PID_YAW_KD  0.0
#define PID_YAW_INTEGRATION_LIMIT     360.0


#define DEFAULT_PID_INTEGRATION_LIMIT  5000.0


typedef struct{
  float desired;     //< set point
  float error;        //< error
  float prevError;    //< previous error
  float integ;        //< integral
  float deriv;        //< derivative
  float kp;           //< proportional gain
  float ki;           //< integral gain
  float kd;           //< derivative gain
  float outP;         //< proportional output (debugging)
  float outI;         //< integral output (debugging)
  float outD;         //< derivative output (debugging)
  float iLimit;       //< integral limit
  float iLimitLow;    //< integral limit
  float dt;           //< delta-time dt
}PID_Typedef;

void pidInit(PID_Typedef *pid, const float desired, const float kp,const float ki, const float kd, const float dt);
float pidCalculate(PID_Typedef *pid,float measured);
void pidSetIntegralLimit(PID_Typedef* pid, const float limit);
void pidSetIntegralLimitLow(PID_Typedef* pid, const float limitLow);
void pidReset(PID_Typedef* pid);
void pidSetError(PID_Typedef* pid, const float error);
void pidSetDesired(PID_Typedef* pid, const float desired);
float pidGetDesired(PID_Typedef* pid);
bool pidIsActive(PID_Typedef* pid);
void pidSetKp(PID_Typedef* pid, const float kp);
void pidSetKi(PID_Typedef* pid, const float ki);
void pidSetKd(PID_Typedef* pid, const float kd);
void pidSetDt(PID_Typedef* pid, const float dt);
