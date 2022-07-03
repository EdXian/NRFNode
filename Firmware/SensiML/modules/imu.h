#ifndef _IMU_H_
#define _IMU_H_
#include "stdio.h"
#include "string.h"
#include "nrf_drv_twi.h"
#include "nrf_twi.h"
#include "nrf_twim.h"
#include "nrf_twi_mngr.h"
#include "nrf_drv_twi.h"

#include "nrf.h"
#include "nrf_gpio.h"

#include "nrf_delay.h"

#define I2C_SCL   5//GPIO1
#define I2C_SDA   26//GPOI19
#define SAB       4//HGPIO0 P0.04
//#define SAB_        6 //HGPIO2 p0.06
#define CLK       7// HGPIO3 p0.07



//#define  CLK 12             //HGPIO8 CLK
//#define   SAB 41              //HGPIO6 SAB
//#define  I2C_SCL   11            //HGPIO7 SCL
//#define   I2C_SDA 8            //HGPIO4 SDA
#define IMU_ADDRESS 0x6a
#define WHO_AM_I  0x0F
#define SENSOR_NUMS 11
#define TWI_INSTANCE_ID          1
#define BMP085_ADDR   (0xEE>>1)
#pragma pack(push) 
#pragma   pack(4) 

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

#pragma pack(pop) 

typedef acc_t adxl345_acc_t;  

adxl345_acc_t adxl345_get_axis();


void imu_init();


void imu_flush();
void imu_flop();
void imu_flip();


#endif