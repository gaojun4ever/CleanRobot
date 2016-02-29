#include "imu.h"

IMU_Typedef imuStruct;

void imuInit(){

}

void imuUpdate(){
  readRawGyro(imuStruct.rawGyro.x, imuStruct.rawGyro.y, imuStruct.Gyro.z);
  readRawAccle(imuStruct.rawAccel.x, imuStruct.rawAccel.y, imuStruct.rawAccel.z);
}

void GyroCal(){
  imuStruct.Gyro.x=imuStruct.rawGyro.x/GYRO_LSB;
}

void AccelCal(){
  imuStruct.rawAccel.x=iirLPF(imuStruct.rawAccel.x);
  imuStruct.rawAccel.y=iirLPF(imuStruct.rawAccel.y);
  imuStruct.rawAccel.z=iirLPF(imuStruct.rawAccel.z);

  imuStruct.Accel.x=imuStruct.rawAccel.x/ACCEL_LSB;
  imuStruct.Accel.y=imuStruct.rawAccel.y/ACCEL_LSB;
  imuStruct.Accel.z=imuStruct.rawAccel.z/ACCEL_LSB;

}
