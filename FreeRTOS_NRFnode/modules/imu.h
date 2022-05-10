#ifndef _IMU_H_
#define _IMU_H_
#include "stdio.h"
#include "string.h"
#include "nrf_drv_twi.h"
#include "nrf_twi.h"
#include "nrf_twim.h"
#include "nrf_twi_mngr.h"
#include "nrf_drv_twi.h"

#define TWI_INSTANCE_ID                 1
#define BMP085_ADDR   (0xEE>>1)
#define I2C_SCL   5//GPIO1
#define I2C_SDA   26//GPOI19

void imu_init();





#endif