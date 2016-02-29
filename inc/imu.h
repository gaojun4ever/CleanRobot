#include "config.h"
#include <mpu6050.h>
#include "filter.h"
#define GYRO_LSB 16.1
#define ACCEL_LSB 10
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
