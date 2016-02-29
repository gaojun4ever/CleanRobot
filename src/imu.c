#include "imu.h"
#include "MPU6050.h"
IMU_Typedef imuStruct;

void imuInit(){

}

void GyroCal(){
  imuStruct.Gyro.x=imuStruct.rawGyro.x/GYRO_LSB;
}

void AccelCal(){
  imuStruct.rawAccel.x=iirLPF(imuStruct.rawAccel.x);
  imuStruct.rawAccel.y=iirLPF(imuStruct.rawAccel.y);
  imuStruct.rawAccel.z=iirLPF(imuStruct.rawAccel.z);
  printf("%d,%d,%d\n",imuStruct.rawAccel.x,imuStruct.rawAccel.y,imuStruct.rawAccel.z );
  imuStruct.Accel.x=imuStruct.rawAccel.x/ACCEL_LSB;
  imuStruct.Accel.y=imuStruct.rawAccel.y/ACCEL_LSB;
  imuStruct.Accel.z=imuStruct.rawAccel.z/ACCEL_LSB;
}

void imuUpdate(){
  readRawGyro(&imuStruct.rawGyro.x, &imuStruct.rawGyro.y, &imuStruct.rawGyro.z);
  readRawAccle(&imuStruct.rawAccel.x, &imuStruct.rawAccel.y, &imuStruct.rawAccel.z);
  GyroCal();
  AccelCal();
}
