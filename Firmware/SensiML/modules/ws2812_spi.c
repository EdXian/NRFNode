#include "ws2812_spi.h"
#include "nrf_spi.h"
#include "nrf_spim.h"
#include "nrf_drv_spi.h"
#include "math.h"
//static const nrf_drv_spi_t spi = NRF_DRV_SPI_INSTANCE(2);
static const nrfx_spim_t spim = NRFX_SPIM_INSTANCE(2);
static count = 0;
uint8_t tx[2] = { 0x00 | 0x80, 0 };
uint8_t rx[2] = { 0 };
#define TX_RX_BUF_LENGTH 255
static uint8_t m_tx_data[PIXEL_NUM][24] = {0};
static uint8_t m_rx_data[TX_RX_BUF_LENGTH] = {0};

static void spi_master_event_handler(nrf_drv_spi_evt_t const * p_event, void * context)
{
  
}


/*
typedef void (* nrf_drv_spi_evt_handler_t)(nrf_drv_spi_evt_t const * p_event,
                                           void *                    p_context);
*/

void ws2812_spi_init(void)
{

  nrfx_spim_config_t spim_config =
    {
        .sck_pin = 32,
        .mosi_pin = 33,
        .miso_pin =NRF_SPI_PIN_NOT_CONNECTED,
        .ss_pin = NRF_SPI_PIN_NOT_CONNECTED,
        .ss_active_high = false,
        .irq_priority = NRFX_SPIM_DEFAULT_CONFIG_IRQ_PRIORITY,
        .orc = 0xFF,
        .frequency = NRF_SPIM_FREQ_8M,
        .mode = NRF_SPIM_MODE_1,
        .bit_order = NRF_SPIM_BIT_ORDER_MSB_FIRST,
        NRFX_SPIM_DEFAULT_EXTENDED_CONFIG        
    };

  //spi_config.irq_priority = APP_IRQ_PRIORITY_HIGH;
  nrfx_spim_init(&spim,&spim_config,NULL,NULL);
  //nrfx_spim_init()
  //APP_ERROR_CHECK(nrf_drv_spi_init(&spi, &spi_config, spi_master_event_handler, NULL));
}

uint32_t ws281x_color(uint8_t red, uint8_t green, uint8_t blue)
{
  return green << 16 | red << 8 | blue;
}

uint32_t ws281x_wheel(uint8_t wheelPos) {
  wheelPos = 255 - wheelPos;
  if(wheelPos < 85) {
    return ws281x_color(255 - wheelPos * 3, 0, wheelPos * 3);
  }
  if(wheelPos < 170) {
    wheelPos -= 85;
    return ws281x_color(0, wheelPos * 3, 255 - wheelPos * 3);
  }
  wheelPos -= 170;
  return ws281x_color(wheelPos * 3, 255 - wheelPos * 3, 0);
}



void ws2812_set_buffer(uint16_t n ,uint8_t red, uint8_t green, uint8_t blue){
    uint8_t i;
  
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < 24; ++i)
    {
      m_tx_data[n][i] = (((ws281x_color(red,green,blue) << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }

}

void ws2812_show(){

ws2812_spi_send(0,0,0);
}


void ws281x_setPixelRGB(uint16_t n ,uint8_t red, uint8_t green, uint8_t blue)
{
  uint8_t i;
  
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < 24; ++i)
    {
      m_tx_data[n][i] = (((ws281x_color(red,green,blue) << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
   ws2812_spi_send(0,0,0);
}



void ws281x_closeAll(void)
{
  uint16_t i;
  uint8_t j;
  
  for(i = 0; i < PIXEL_NUM; ++i)
  {
    for(j = 0; j < 24; ++j)
    {
      m_tx_data[i][j] = WS_LOW;
    }
  }
  ws2812_spi_send(0,0,0);
}


void ws281x_setPixelColor(uint16_t n ,uint32_t GRBcolor)
{
  uint8_t i;
  if(n < PIXEL_NUM)
  {
    for(i = 0; i < 24; ++i)
    {
      m_tx_data[n][i] = (((GRBcolor << i) & 0X800000) ? WS_HIGH : WS_LOW);
    }
  }
 ws2812_spi_send(0,0,0);
}



void ws2812_spi_send(uint8_t * const p_tx_data,uint8_t * const p_rx_data,const uint16_t len)           
{

    nrfx_spim_xfer_desc_t xfer =
    {
        .p_tx_buffer = m_tx_data,
        .tx_length = (PIXEL_NUM*24),
        .p_rx_buffer = m_rx_data,
        .rx_length= (PIXEL_NUM*24),
    };
    nrfx_spim_xfer(&spim, &xfer, 0);
   // nrf_drv_spi_transfer(&spi,m_tx_data ,255, m_rx_data,255);
 
}
