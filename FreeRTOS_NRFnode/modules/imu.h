#ifndef _IMU_H_
#define _IMU_H_
#include "stdio.h"
#include "string.h"
#include "nrf_drv_twi.h"
#include "nrf_twi.h"
#include "nrf_twim.h"
#include "nrf_twi_mngr.h"
#include "nrf_drv_twi.h"

#define I2C_SCL   5//GPIO1
#define I2C_SDA   26//GPOI19

#define TWI_INSTANCE_ID          1
#define BMP085_ADDR   (0xEE>>1)


typedef struct acc{

  int16_t x;
  int16_t y;
  int16_t z;

}acc_t;


typedef struct gyro{

  int16_t x;
  int16_t y;
  int16_t z;

}gyro_t;


typedef acc_t adxl345_acc_t;  

adxl345_acc_t adxl345_get_axis();


void imu_init();





#endif