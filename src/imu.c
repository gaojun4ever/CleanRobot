#include "imu.h"
#include "mpu6050.h"
#include "filter.h"
#define GYRO_DEG_PER_LSB MPU6050_DEG_PER_LSB_2000
#define ACCEL_G_PER_LSB MPU6050_G_PER_LSB_8
IMU_Typedef imuStruct;

void imuInit(){

}

void GyroCal(){
  imuStruct.Gyro.x = imuStruct.rawGyro.x * GYRO_DEG_PER_LSB;
  imuStruct.Gyro.y = imuStruct.rawGyro.y * GYRO_DEG_PER_LSB;
  imuStruct.Gyro.z = imuStruct.rawGyro.z * GYRO_DEG_PER_LSB;
  printf("%f,%f,%f\n",imuStruct.Gyro.x,imuStruct.Gyro.y,imuStruct.Gyro.z );
}

void GyroCal(){
  imuStruct.rawAccel.x = iirLPF(imuStruct.rawAccel.x);
  imuStruct.rawAccel.y = iirLPF(imuStruct.rawAccel.y);
  imuStruct.rawAccel.z = iirLPF(imuStruct.rawAccel.z);
  //printf("%d,%d,%d\n",imuStruct.rawAccel.x,imuStruct.rawAccel.y,imuStruct.rawAccel.z );
  imuStruct.Accel.x = imuStruct.rawAccel.x * ACCEL_G_PER_LSB;
  imuStruct.Accel.y = imuStruct.rawAccel.y * ACCEL_G_PER_LSB;
  imuStruct.Accel.z = imuStruct.rawAccel.z * ACCEL_G_PER_LSB;
  //printf("%f,%f,%f\n",imuStruct.Accel.x,imuStruct.Accel.y,imuStruct.Accel.z );
}

void imuUpdate(){
  readRawGyro(&imuStruct.rawGyro.x, &imuStruct.rawGyro.y, &imuStruct.rawGyro.z);
  readRawAccle(&imuStruct.rawAccel.x, &imuStruct.rawAccel.y, &imuStruct.rawAccel.z);
  GyroCal();
  AccelCal();
}
