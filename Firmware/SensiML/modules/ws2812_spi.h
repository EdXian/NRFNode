#ifndef _WS2812_SPI_H_
#define _WS2812_SPI_H_
#include "stdio.h"
#include "nrf_drv_spi.h"

#define SPI_NUM 4
#define SPI_BIT_RATE 1  //1MHZ
#define WLED_NUM 3
#define WS2812_BUFFER_SIZE  (9*WLED_NUM) // 9*N
#define PIXEL_NUM 4
#define WS_HIGH 0XF8
#define WS_LOW  0XE0


#define MOSI_PIN   33
#define MISO_PIN   34
#define SCK_PIN    32


void ws2812_spi_init(void);
void ws2812_spi_num_register(void);
void ws2812_spi_send(uint8_t * const p_tx_data,uint8_t * const p_rx_data, const uint16_t len);
                   
                  
#endif
