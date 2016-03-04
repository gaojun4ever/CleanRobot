#include "imu.h"
#include "mpu9250.h"
#include "filter.h"
#include "protocolUtils.h"
#include "math.h"

#define BETA_DEF     0.01f    // 2 * proportional gain
float beta = BETA_DEF;     // 2 * proportional gain (Kp)

float q0 = 1.0f, q1 = 0.0f, q2 = 0.0f, q3 = 0.0f; // estimated orientation quaternion elements with initial conditions

#define GYRO_DEG_PER_LSB MPU6050_DEG_PER_LSB_2000
#define ACCEL_G_PER_LSB MPU6050_G_PER_LSB_8
IMU_Typedef imuStruct;

int16_t gyroData[3];
int16_t accelData[3];

void imuInit(){

}

void GyroCal(){
  imuStruct.Gyro.x = imuStruct.rawGyro.x * GYRO_DEG_PER_LSB;
  imuStruct.Gyro.y = imuStruct.rawGyro.y * GYRO_DEG_PER_LSB;
  imuStruct.Gyro.z = imuStruct.rawGyro.z * GYRO_DEG_PER_LSB;
  //printf("%f,%f,%f\n",imuStruct.Gyro.x,imuStruct.Gyro.y,imuStruct.Gyro.z );
}

void AccelCal(){
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

  MPU6050GyroRead(gyroData);
  MPU6050AccRead(accelData);
  MPU9250_Get3AxisAccelRawData(accelData);
  MPU9250_Get3AxisGyroRawData(gyroData);
  imuStruct.rawAccel.x = accelData[0];
  imuStruct.rawAccel.y = accelData[1];
  imuStruct.rawAccel.z = accelData[2];

  imuStruct.rawGyro.x = gyroData[0];
  imuStruct.rawGyro.y = gyroData[1];
  imuStruct.rawGyro.z = gyroData[2];


  GyroCal();
  AccelCal();

  filterUpdate(imuStruct.Gyro.x, imuStruct.Gyro.y, imuStruct.Gyro.z, imuStruct.Accel.x, imuStruct.Accel.y, imuStruct.Accel.z,0.005);
  GetEulerAngle();
}

//---------------------------------------------------------------------------------------------------
// Fast inverse square-root
// See: http://en.wikipedia.org/wiki/Fast_inverse_square_root
float invSqrt(float x)
{
  float halfx = 0.5f * x;
  float y = x;
  long i = *(long*)&y;
  i = 0x5f3759df - (i>>1);
  y = *(float*)&i;
  y = y * (1.5f - (halfx * y * y));
  return y;
}


void filterUpdate(float gx, float gy, float gz, float ax, float ay, float az, float dt)
{
  float recipNorm;
  float s0, s1, s2, s3;
  float qDot1, qDot2, qDot3, qDot4;
  float _2q0, _2q1, _2q2, _2q3, _4q0, _4q1, _4q2 ,_8q1, _8q2, q0q0, q1q1, q2q2, q3q3;

  // Rate of change of quaternion from gyroscope
  qDot1 = 0.5f * (-q1 * gx - q2 * gy - q3 * gz);
  qDot2 = 0.5f * (q0 * gx + q2 * gz - q3 * gy);
  qDot3 = 0.5f * (q0 * gy - q1 * gz + q3 * gx);
  qDot4 = 0.5f * (q0 * gz + q1 * gy - q2 * gx);

  // Compute feedback only if accelerometer measurement valid (avoids NaN in accelerometer normalisation)
  if(!((ax == 0.0f) && (ay == 0.0f) && (az == 0.0f)))
  {
   // Normalise accelerometer measurement
   recipNorm = invSqrt(ax * ax + ay * ay + az * az);
   ax *= recipNorm;
   ay *= recipNorm;
   az *= recipNorm;

   // Auxiliary variables to avoid repeated arithmetic
   _2q0 = 2.0f * q0;
   _2q1 = 2.0f * q1;
   _2q2 = 2.0f * q2;
   _2q3 = 2.0f * q3;
   _4q0 = 4.0f * q0;
   _4q1 = 4.0f * q1;
   _4q2 = 4.0f * q2;
   _8q1 = 8.0f * q1;
   _8q2 = 8.0f * q2;
   q0q0 = q0 * q0;
   q1q1 = q1 * q1;
   q2q2 = q2 * q2;
   q3q3 = q3 * q3;

   // Gradient decent algorithm corrective step
   s0 = _4q0 * q2q2 + _2q2 * ax + _4q0 * q1q1 - _2q1 * ay;
   s1 = _4q1 * q3q3 - _2q3 * ax + 4.0f * q0q0 * q1 - _2q0 * ay - _4q1 + _8q1 * q1q1 + _8q1 * q2q2 + _4q1 * az;
   s2 = 4.0f * q0q0 * q2 + _2q0 * ax + _4q2 * q3q3 - _2q3 * ay - _4q2 + _8q2 * q1q1 + _8q2 * q2q2 + _4q2 * az;
   s3 = 4.0f * q1q1 * q3 - _2q1 * ax + 4.0f * q2q2 * q3 - _2q2 * ay;
   recipNorm = invSqrt(s0 * s0 + s1 * s1 + s2 * s2 + s3 * s3); // normalise step magnitude
   s0 *= recipNorm;
   s1 *= recipNorm;
   s2 *= recipNorm;
   s3 *= recipNorm;

   // Apply feedback step
   qDot1 -= beta * s0;
   qDot2 -= beta * s1;
   qDot3 -= beta * s2;
   qDot4 -= beta * s3;
  }

  // Integrate rate of change of quaternion to yield quaternion
  q0 += qDot1 * dt;
  q1 += qDot2 * dt;
  q2 += qDot3 * dt;
  q3 += qDot4 * dt;

  // Normalise quaternion
  recipNorm = invSqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
  q0 *= recipNorm;
  q1 *= recipNorm;
  q2 *= recipNorm;
  q3 *= recipNorm;
}

void GetEulerAngle(void)
{
  imuStruct.eulerAngle.x=atan2(2*(q0*q1+q2*q3),(1-2*(q1*q1+q2*q2)))*57.3;
  imuStruct.eulerAngle.y=asin(2*(q0*q2-q1*q3))*57.3;
  imuStruct.eulerAngle.z=atan2(2*(q0*q3+q1*q2),(1-2*(q3*q3+q2*q2)))*57.3;
  printf("%f,%f,%f\n",imuStruct.eulerAngle.x,imuStruct.eulerAngle.y,imuStruct.eulerAngle.z );
  //sendDataProtocol(&imuStruct.eulerAngle);
}
