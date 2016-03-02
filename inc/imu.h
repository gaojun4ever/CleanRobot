#include "config.h"

typedef struct{
  int16_t x;
  int16_t y;
  int16_t z;
}Axis3_Int_Typedef;
typedef struct{
  float x;
  float y;
  float z;
}Axis3_Float_Typedef;

typedef struct{
  float q0;
  float q1;
  float q2;
  float q3;
}Quaternion_Typedef;

typedef struct{
  Axis3_Int_Typedef rawGyro;
  Axis3_Int_Typedef rawAccel;
  Axis3_Float_Typedef Gyro;
  Axis3_Float_Typedef Accel;
  Axis3_Float_Typedef eulerAngle;
  Quaternion_Typedef quaternion;

}IMU_Typedef;

extern IMU_Typedef imuStruct;

void imuUpdate(void);
void filterUpdate(float gx, float gy, float gz, float ax, float ay, float az, float dt);
void GetEulerAngle(void);
