/**
* @file ble_bmw_service.c
* @author Danny Deng
* @Company Biologue Co.,Ltd
* @Date 2021/07/13
* 
* @brief BLE service for BLE sensor collector, this file defines the bmwseat service and its characteristic.
*/

#ifndef BLE_BMW_SERVICE_H
#define BLE_BMW_SERVICE_H

#include <stdint.h>
#include <string.h>
#include "ble.h"
#include "ble_srv_common.h"
//#include "ble_biologue_ota.h"

#define BMW_SEAT_VERSION       (0x12)     // high 4 bit x, low 4 bit y => verser = x.y, example 0x10 is version 1.0
#define BLE_INFO_ADDRESS       (0x60000)
#define BLE_VERSION_ADDRESS    (0x60000)
#define BLE_NAME_SIZE_ADDRESS  (0x60001)
#define BLE_NAME_ADDRESS       (0x60002)

#ifndef BLE_BMWSEART_OBSERVER_PROO
#define BLE_BMWSEART_OBSERVER_PROO 2
#endif

// Define the Biologue service
#define BLE_BMWSEART_DEF(_name)                                  \
ble_bmwseat_service_t _name;                                    \
NRF_SDH_BLE_OBSERVER(_name ## _obs,                              \
                     BLE_BMWSEART_OBSERVER_PROO,                 \
                     ble_bmwseat_service_on_ble_evt, &_name)

// Biologue Service UUID: 0x1234E101-FFFF-1234-FFFF-111122223333
// Biologue Control Characteristic UUID: 0x1234E102-FFFF-1234-FFFF-111122223333
// Biologue Response Characteristic UUID: 0x1234E103-FFFF-1234-FFFF-111122223333
// Biologue Hardness Characteristic UUID: 0x1234E104-FFFF-1234-FFFF-111122223333
// The bytes need to be in reverse order to match the UUID from the spec
// Note: They are stored in little endian, meaning that the Least Significant Byte
// is stored first.

//	EF68xxxx-9B35-4933-9B10-52FFA9740042    
//#define BLE_UUID_BMWSEART_SERVICE_BASE_UUID {0x42, 0x00, 0x74, 0xa9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49,  0x35, 0x9B, 0x00, 0x40, 0x68, 0xEF}
//#define BLE_UUID_BMWSEART_SERVICE_UUID        0x0400
    

#define BLE_UUID_BMWSEART_SERVICE_BASE_UUID   {0x42, 0x00, 0x74, 0xa9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49,  0x35, 0x9B, 0x00, 0x40, 0x68, 0xEF}
#define BLE_UUID_BMWSEART_SERVICE_UUID        0x0400

#define BLE_UUID_BMWSEART_CONTROL_BASE_UUID  {0x42, 0x00, 0x74, 0xa9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49,  0x35, 0x9B, 0x00, 0x40, 0x68, 0xEF}
#define BLE_UUID_BMWSEART_CONTROL_UUID        0x0401

#define BLE_UUID_BMWSEART_INFO_BASE_UUID     {0x42, 0x00, 0x74, 0xa9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49,  0x35, 0x9B, 0x00, 0x40, 0x68, 0xEF}
#define BLE_UUID_BMWSEART_INFO_UUID           0x0402

#define BLE_UUID_BMWSEART_STATUS_BASE_UUID   {0x42, 0x00, 0x74, 0xa9, 0xFF, 0x52, 0x10, 0x9B, 0x33, 0x49,  0x35, 0x9B, 0x00, 0x40, 0x68, 0xEF}
#define BLE_UUID_BMWSEART_STATUS_UUID         0x0403

/* Define the value size */
#define VALUE_LEN_MAX                        (NRF_SDH_BLE_GATT_MAX_MTU_SIZE-3)
#define VALUE_LEN_CONTROL                    (230)
#define VALUE_LEN_BMW_PRESSURE               (230)
#define VALUE_LEN_STATUS                     (8)
#define VALUE_LEN_BLE_NAME                   (20)

/**@brief Biologue Service event type. */
typedef enum {
    BLE_CONTROL,
    BLE_BMW_INFO_EVT_NOTIFICATION_ENABLED,
    BLE_BMW_INFO_EVT_NOTIFICATION_DISABLED,
} ble_bmwseat_evt_type_t;
 
/**@brief Biologue Service event. */
typedef struct {
  ble_bmwseat_evt_type_t evt_type;
} ble_bmwseat_evt_t;

// Forward declaration of the ble_midi_service_t type. 
typedef struct ble_bmwseat_service_s ble_bmwseat_service_t;

// Define the biologue service handler
typedef void (*ble_bmwseat_evt_handler_t) (ble_bmwseat_service_t * p_bmwseat_service, ble_bmwseat_evt_t * p_evt);

typedef struct {
    ble_bmwseat_evt_handler_t evt_handler;  /**< Event handler to be called when a MIDI event occurs. */
} ble_bmwseat_service_init_t;


/**@brief Biologue service structure. This contains various status information for the service. */
struct ble_bmwseat_service_s {
    uint16_t                    service_handle;
    ble_gatts_char_handles_t    control_handles;
    ble_gatts_char_handles_t    bmw_pressure_handles;
    ble_gatts_char_handles_t    bmw_status_handles;
    uint8_t                     uuid_type;
    uint16_t                    conn_handle;
    ble_bmwseat_evt_handler_t  evt_handler;
};

/**@brief Function for initializing the Biologue service.
 *
 * @param[out]  p_bmwseat_service   Biologue service structure. This structure will have to be supplied by
 *                                   the application. It will be initialized by this function, and will later
 *                               be used to identify this particular service instance.
 * @param[in]   p_bmwseat_service_init  Information needed to initialize the service.
 *
 * @return      NRF_SUCCESS on successful initialization of service, otherwise an error code.
 */
uint32_t ble_bmwseat_service_init(ble_bmwseat_service_t *p_bmwseat_service, const ble_bmwseat_service_init_t *p_bmwseat_service_init);

/**@brief Function for handling the Application's BLE Stack events.
 *
 * @details Handles all events from the BLE stack of interest to the Biologue service.
 *
 *
 * @param[in]   p_bmwseat_service      LED Button Service structure.
 * @param[in]   p_ble_evt  Event received from the BLE stack.
 */
void ble_bmwseat_service_on_ble_evt(ble_evt_t const * p_ble_evt, void * p_context);


uint32_t ble_bmwseat_control_handle(uint8_t *p_data, uint8_t data_sz);
uint32_t ble_bmw_notify(ble_bmwseat_service_t *p_bmwseat_service, uint8_t *p_data, uint8_t data_sz);


// Write the version and name to flash
uint32_t write_version_device_name_to_flash(uint8_t version, uint8_t *dname, uint8_t dname_len);

#endif /* BLE_BMW_SERVICE_H */
