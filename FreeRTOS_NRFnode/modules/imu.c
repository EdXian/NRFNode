#include "imu.h"


static const nrf_drv_twi_t m_twi = NRF_DRV_TWI_INSTANCE(TWI_INSTANCE_ID);
static volatile bool m_xfer_done = false;

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




//HGPIO01  P0.05
//GPIO19   p0.26
void imu_init(){

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

    imu_probe();

}


void imu_probe(){
  uint8_t err_code;
  uint8_t data[2]={0xf4,0xf4};
  err_code = twi_write_reg(&m_twi, 0x77,data, 2 );
  if(err_code!=0){
    while(1);
  }

}

