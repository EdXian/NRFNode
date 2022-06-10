#include "imu.h"


static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static volatile bool m_xfer_done = false;
static uint8_t ack_evt = false;
void twi_handler(nrf_drv_twi_evt_t const * p_event, void * p_context)
{
    switch (p_event->type)
    {
        case NRF_DRV_TWI_EVT_DONE:
            if (p_event->xfer_desc.type == NRF_DRV_TWI_XFER_RX)
            {
                //data_handler(m_sample);
                
            }
            m_xfer_done = true;
            ack_evt = true;
            break;


        case NRF_DRV_TWI_EVT_ADDRESS_NACK:
            asm("nop");

          break;
        

        case NRF_DRV_TWI_EVT_DATA_NACK:
            asm("nop");

          break;
        default:
            break;
    }
}


void wait_sync(){

  while (m_xfer_done == false){
  
  
  }

}

#define MAX_WRITE_LENGTH  200
static bool twi_write_reg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *write_data, uint16_t len)
{
    ret_code_t err_code;

    uint8_t data[MAX_WRITE_LENGTH] ;

    if(len> MAX_WRITE_LENGTH -1)
    {
        return false;
    }

    data[0] = reg_addr;
    memcpy(&data[1], write_data, len);

    m_xfer_done =false;
    err_code = nrf_drv_twi_tx(&m_twi, slave_addr, data, len+1, false);
    APP_ERROR_CHECK(err_code); 

    while (m_xfer_done == false);  

    return true;
}

static uint8_t twi_read_reg(uint8_t slave_addr, uint8_t reg_addr, uint8_t *read_data, uint16_t len)
{

    ret_code_t err_code;

    m_xfer_done = false;
    err_code = nrf_drv_twi_tx(&m_twi, slave_addr, &reg_addr,      1, true);
    APP_ERROR_CHECK(err_code);
    while (m_xfer_done == false);  



    m_xfer_done =false;
    err_code = nrf_drv_twi_rx(&m_twi, slave_addr , read_data, len);
    APP_ERROR_CHECK(err_code);    
    while (m_xfer_done == false);  

    return true;

}




void imu_flip(){

  nrf_gpio_pin_set(SAB);


  nrf_gpio_pin_set(CLK);
 
  nrf_gpio_pin_clear(SAB);
  nrf_gpio_pin_clear(CLK);

}

void imu_flop(){

  nrf_gpio_pin_clear(SAB);
 
  nrf_gpio_pin_set(CLK);

  nrf_gpio_pin_clear(CLK);
}

void imu_flush(){
  for(uint8_t i=0;i<255;i++){
  
    imu_flop();
  }

}


void select_imu(uint8_t i){
  imu_flush();
  imu_flip();
  for(uint8_t j=0;j<i-1;j++){
    imu_flop();
  }

}


//HGPIO01  P0.05
//GPIO19   p0.26

void imu_gpio_init(){
    nrf_gpio_pin_pull_t pull;
   
    nrf_gpio_cfg_output(SAB);
    nrf_gpio_cfg_output(CLK);
}


void imu_init(){

     imu_gpio_init();
     //imu_flush();
     //imu_flip();
      ret_code_t err_code;

    const nrf_drv_twi_config_t twi_lm75b_config = {
       .scl                = I2C_SCL,
       .sda                =  I2C_SDA,
       .frequency          = NRF_DRV_TWI_FREQ_100K,
       .interrupt_priority = APP_IRQ_PRIORITY_HIGH,
       .clear_bus_init     = false
    };

    err_code = nrf_drv_twi_init(&m_twi, &twi_lm75b_config, twi_handler, NULL);
    APP_ERROR_CHECK(err_code);

    nrf_drv_twi_enable(&m_twi);

   //bmp085_enable();
    //for(uint8_t i=1;i<SENSOR_NUMS;i++){
    
    //  select_imu(i);
    //  adxl345_enable();

    //}
}


void bmp085_enable(){
  uint8_t err_code;
  uint8_t data[1]={0xf4};
  err_code = twi_write_reg(0x77,0xf4 ,data, 1 );
  
  if(err_code!=true){
    while(1);
  }
}

static uint8_t adxl345_id;
static uint8_t BW_RATE;
static uint8_t PWR_CTRL;
static uint8_t imu_data;




void adxl345_enable(){

   uint8_t err_code;
  uint8_t data[1]={0x0};
  //err_code = twi_read_reg(0x53,0x00 ,&adxl345_id, 1 );  //adxl345
                                                      // lsm330

  err_code = twi_read_reg(IMU_ADDRESS,WHO_AM_I ,&adxl345_id, 1 );

  
  if(err_code!=true){ 
    while(1);
  }
  

   imu_data = 0x3c;
   twi_write_reg(IMU_ADDRESS,0x11 ,&imu_data, 1);
    if(err_code!=true){
      while(1);
    }


   imu_data = 0x38;
   twi_write_reg(IMU_ADDRESS,0x10 ,&imu_data, 1);
    if(err_code!=true){
      while(1);
    }

    //0x11   0x3c
    //0x10   0x38
  

  nrf_delay_ms(50);

 err_code = twi_read_reg(IMU_ADDRESS,0x10 ,&BW_RATE, 1 );
  if(err_code!=true){
    while(1);
  }
  
   err_code = twi_read_reg(IMU_ADDRESS,0x12 ,&BW_RATE, 1 );
  if(err_code!=true){
    while(1);
  }

}

uint8_t axis[6];   // 32 33 34 35 36 37
static adxl345_acc_t adxl345_acc;

adxl345_acc_t adxl345_get_axis(){

  uint8_t err_code;
  err_code = twi_read_reg(IMU_ADDRESS,0x28,axis,6 );
  //return ((int16_t)(axis[0])&0x00ff + (int16_t)(axis[1]<<8));

  adxl345_acc.x = ((int16_t)axis[0]) + (((int16_t)axis[1])<<8);
  adxl345_acc.y = ((int16_t)axis[2]) + (((int16_t)axis[3])<<8);
  adxl345_acc.z = ((int16_t)axis[4]) + (((int16_t)axis[5])<<8);

  return adxl345_acc;
}




static uint8_t bmp085_pressure[2];

int16_t bmp085_read(){
  //uint8_t data[2]={};
  twi_read_reg(0x77,0xf6,bmp085_pressure,2);
  return (bmp085_pressure[0] + bmp085_pressure[1]<<8);
}





void imu_probe(){
  uint8_t err_code;
  uint8_t data[1]={0xf4};
  err_code = twi_write_reg(0x77,0xf4 ,data, 1 );
  
  if(err_code!=true){
    while(1);
  }

}

