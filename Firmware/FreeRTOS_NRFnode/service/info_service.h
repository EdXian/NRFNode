#ifndef _INFO_SERVICE_H_
#define _INFO_SERVICE_H_

#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include "ble.h"
#include "ble_srv_common.h"
#include "boards.h"
#include "nrf_sdh_ble.h"

#ifndef BLE_INFO_SERVICE_OBSERVER_PROO
#define BLE_INFO_SERVICE_OBSERVER_PROO 2
#endif

#define INFO_SERVICE_UUID_BASE        {0x42, 0x00, 0x74, 0xa9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49,  0x35, 0x9B, 0x00, 0x40, 0x68, 0xEF}
#define INFO_SERVICE_UUID_SERVICE     0x0100
#define BLE_UUID_DEVICE_NAME_UUID        0x0101
#define INFO_SERVICE_ADV_UUID           0x0102

//#define INFO_SERVICE_UUID_LED_ENA     0x196A
//#define BLINK_UUID_LED_INT     0x196B

#define BLE_INFO_SERVICE_DEF(_name)                                  \
ble_bmwseat_service_t _name;                                    \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                              \
                     BLE_INFO_SERVICE_OBSERVER_PROO,                 \
                     ble_bmwseat_service_on_ble_evt, &_name)

//typedef void (*ble_bmwseat_evt_handler_t) (ble_bmwseat_service_t * p_bmwseat_service, ble_bmwseat_evt_t * p_evt);

//typedef struct {
//    ble_bmwseat_evt_handler_t evt_handler;  /**< Event handler to be called when a MIDI event occurs. */
//} ble_bmwseat_service_init_t;

//struct ble_bmwseat_service_s {
//    uint16_t                    service_handle;
//    ble_gatts_char_handles_t    control_handles;
//    ble_gatts_char_handles_t    bmw_pressure_handles;
//    ble_gatts_char_handles_t    bmw_status_handles;
//    ble_gatts_char_handles_t    motion_raw_handles;
//    ble_gatts_char_handles_t    motion_heading_handles;
//    ble_gatts_char_handles_t    motion_rotation_handles;
//    ble_gatts_char_handles_t    motion_euler_handles;
//    ble_gatts_char_handles_t    motion_gravity_handles;
//    ble_gatts_char_handles_t    dev_name_handles;
//    uint8_t                     uuid_type;
//    uint16_t                    conn_handle;
//   // ble_bmwseat_evt_handler_t  evt_handler;
//};

typedef enum {
    BLE_INFO_SERVICE_CONTROL,
    BLE_INFO_SERVICE_EVT_NOTIFICATION_ENABLED,
    BLE_INFO_SERVICE_EVT_NOTIFICATION_DISABLED,
} ble_info_service_evt_type_t;
 
/**@brief Biologue Service event. */
typedef struct {
  ble_info_service_evt_type_t evt_type;
} ble_info_service_evt_t;


struct ble_info_service_s {
        /* Service handle assigned by the BLE stack */
        uint16_t service_handle;
        ble_gatts_char_handles_t device_name_handle;
        ble_gatts_char_handles_t  adv_param_handle;



        uint8_t  service_type;        
};

typedef struct ble_info_service_s ble_info_service_t;


typedef void (*ble_info_service_evt_handler_t) (ble_info_service_t * p_bmwseat_service, ble_info_service_evt_t * p_evt);

typedef struct {
    ble_info_service_evt_handler_t evt_handler;  /**< Event handler to be called when a MIDI event occurs. */
} ble_info_service_init_t;


ret_code_t ble_info_service_init(ble_info_service_t *p_blink,const ble_info_service_init_t *p_info_service_init);


#endif
