/**
* @file ble_bmw_service.c
* @author Danny Deng
* @Company Biologue Co.,Ltd
* @Date 2021/01/07
* 
* @brief BLE service for BMW seat, this file defines the bmw adapter service and its characteristic.
*/

#include "motion_service.h"
#include "app_util.h"
//#include "rtc.h"
#include "ble_srv_common.h"
#include "nordic_common.h"
#include "nrf_log.h"
#include "nrf_soc.h"
//#include "goto_bootloader.h"
//#include "mavlink.h"
//#include "uarte.h"
//#include "ble_bmw_service.h"
//#include "ble_biologue_ota.h"
//#include "boot_pin.h"
//#include "boot.h"
#include "nrf_delay.h"
uint8_t bmw_status[VALUE_LEN_STATUS] = {BMW_SEAT_VERSION, 1, 2, 3, 4, 5 ,6 ,7};

/* Global value in stack for characteristic */

/**@brief Function for handling the Connect event.
 *
 * @param[in]   p_bmwseat_service   Biologue service structure.
 * @param[in]   p_ble_evt            Event received from the BLE stack.
 */
static void on_connect(ble_bmwseat_service_t *p_bmwseat_service, ble_evt_t const *p_ble_evt) {
  p_bmwseat_service->conn_handle = p_ble_evt->evt.gap_evt.conn_handle;
}

/**@brief Function for handling the Disconnect event.
 *
 * @param[in]   p_bmwseat_service   LED Button Service structure.
 * @param[in]   p_ble_evt            Event received from the BLE stack.
 */
static void on_disconnect(ble_bmwseat_service_t *p_bmwseat_service, ble_evt_t const *p_ble_evt) {
  UNUSED_PARAMETER(p_ble_evt);
  p_bmwseat_service->conn_handle = BLE_CONN_HANDLE_INVALID;
}

/**@brief Function for handling the Write event.
 *
 * @param[in]   p_bmwseat_service   LED Button Service structure.
 * @param[in]   p_ble_evt        Event received from the BLE stack.
 */
static void on_write(ble_bmwseat_service_t *p_bmwseat_service, ble_evt_t const *p_ble_evt) {
  ble_gatts_evt_write_t *p_evt_write = (ble_gatts_evt_write_t *)&p_ble_evt->evt.gatts_evt.params.write;
  uint16_t command = 0;
  
  // Handle characteristic
  if((p_evt_write->handle == p_bmwseat_service->control_handles.value_handle) && (p_bmwseat_service->evt_handler != NULL)) {
    NRF_LOG_INFO("Control write handle.");

    ble_bmwseat_control_handle(p_evt_write->data, p_evt_write->len);
  }

  // Check if the Custom value CCCD is written to and that the value is the appropriate length, i.e 2 bytes.
  if (p_evt_write->handle == p_bmwseat_service->bmw_pressure_handles.cccd_handle) {
    // CCCD written, call application event handler
    if (p_bmwseat_service->evt_handler != NULL) {
      ble_bmwseat_evt_t evt;

      if (ble_srv_is_notification_enabled(p_evt_write->data)) {
        evt.evt_type = BLE_BMW_INFO_EVT_NOTIFICATION_ENABLED;
      } 
      else {
        evt.evt_type = BLE_BMW_INFO_EVT_NOTIFICATION_DISABLED;
      }
      p_bmwseat_service->evt_handler(p_bmwseat_service, &evt);
    }
  }
}


// Notify data complete
static void on_hvx_tx_complete(ble_bmwseat_service_t *p_bmwseat_service, ble_evt_t const *p_ble_evt) {

}

void ble_bmwseat_service_on_ble_evt(ble_evt_t const *p_ble_evt, void *p_context) {
  ble_bmwseat_service_t *p_bmwseat_service = (ble_bmwseat_service_t *)p_context;

  if(p_ble_evt->header.evt_id != BLE_GATTS_EVT_HVN_TX_COMPLETE)
    NRF_LOG_INFO("BLE event received. Event type = %x", p_ble_evt->header.evt_id);

  if (p_bmwseat_service == NULL || p_ble_evt == NULL) {
    return;
  }

  switch (p_ble_evt->header.evt_id) {
  case BLE_GAP_EVT_CONNECTED:
    on_connect(p_bmwseat_service, p_ble_evt);
    break;

  case BLE_GAP_EVT_DISCONNECTED:
    on_disconnect(p_bmwseat_service, p_ble_evt);
    break;

  case BLE_GATTS_EVT_WRITE:
    on_write(p_bmwseat_service, p_ble_evt);
    break;
  case BLE_GATTS_EVT_HVN_TX_COMPLETE:
    on_hvx_tx_complete(p_bmwseat_service, p_ble_evt);
    break;
  case BLE_GATTS_EVT_SYS_ATTR_MISSING:
    NRF_LOG_ERROR("BLE_GATTS_EVT_SYS_ATTR_MISSING");
    break;
  default:
    // No implementation needed.
    break;
  }
}




/**@brief Function for adding the command characteristic.
 *
 */
static uint32_t control_char_add(ble_bmwseat_service_t *p_bmwseat_service, const ble_bmwseat_service_init_t *p_bmwseat_service_init) {
  uint32_t err_code;
  ble_gatts_char_md_t char_md;
  ble_gatts_attr_md_t cccd_md;
  ble_gatts_attr_t attr_char_value;
  ble_uuid_t ble_uuid;
  ble_gatts_attr_md_t attr_md;

  // Configure the CCCD which is needed for Notifications and Indications
  memset(&cccd_md, 0, sizeof(cccd_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
  cccd_md.vloc = BLE_GATTS_VLOC_STACK;

  // Configure the characteristic metadata.
  memset(&char_md, 0, sizeof(char_md));
  char_md.char_props.write = 1;
  char_md.char_props.read = 0;
  char_md.char_props.write_wo_resp = 0;
  char_md.char_props.notify = 0;
  char_md.p_char_user_desc = "Control command from app.";
  char_md.char_user_desc_max_size = 8;
  char_md.char_user_desc_size = 8;
  char_md.p_char_pf = NULL;
  char_md.p_user_desc_md = NULL;
  // char_md.p_cccd_md = &cccd_md;
  char_md.p_sccd_md = NULL;

  // Add the control Characteristic UUID
  ble_uuid128_t base_uuid = {BLE_UUID_MOTION_CONFIG_BASE_UUID};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_bmwseat_service->uuid_type);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_bmwseat_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_MOTION_CONFIG_UUID;

  // Configure the characteristic value's metadata
  memset(&attr_md, 0, sizeof(attr_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
  attr_md.vloc = BLE_GATTS_VLOC_STACK;
  attr_md.rd_auth = 0;
  attr_md.wr_auth = 0;
  attr_md.vlen = 1;

  // Configure the characteristic value
  memset(&attr_char_value, 0, sizeof(attr_char_value));
  attr_char_value.p_uuid = &ble_uuid;
  attr_char_value.p_attr_md = &attr_md;
  attr_char_value.init_len = VALUE_LEN_CONTROL;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = VALUE_LEN_CONTROL;
  attr_char_value.p_value = NULL;

  return sd_ble_gatts_characteristic_add(p_bmwseat_service->service_handle, &char_md,
      &attr_char_value,
      &p_bmwseat_service->control_handles);
}


/**@brief Function for adding the Smart Seat characteristic.
 * @property notify
 * @type     char
 * @length   XX bytes
 */
static uint32_t bmw_pressure_char_add(ble_bmwseat_service_t *p_bmwseat_service, const ble_bmwseat_service_init_t *p_bmwseat_service_init) {
  uint32_t err_code;
  ble_gatts_char_md_t char_md;
  ble_gatts_attr_md_t cccd_md;
  ble_gatts_attr_t attr_char_value;
  ble_uuid_t ble_uuid;
  ble_gatts_attr_md_t attr_md;

  // Configure the CCCD which is needed for Notifications and Indications
  memset(&cccd_md, 0, sizeof(cccd_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
  cccd_md.vloc = BLE_GATTS_VLOC_STACK;

  // Configure the characteristic metadata.
  memset(&char_md, 0, sizeof(char_md));
  char_md.char_props.read = 0;
  char_md.char_props.write_wo_resp = 0;
  char_md.char_props.notify = 1;
  char_md.p_char_user_desc = "pressure.";
  char_md.char_user_desc_max_size = 9;
  char_md.char_user_desc_size = 9;
  char_md.p_char_pf = NULL;
  char_md.p_user_desc_md = NULL;
  char_md.p_cccd_md = &cccd_md;
  char_md.p_sccd_md = NULL;

  // Add the biologue Data I/O Characteristic UUID
  ble_uuid128_t base_uuid = {BLE_UUID_MOTION_QUAT_BASE_UUID};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_bmwseat_service->uuid_type);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_bmwseat_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_MOTION_QUAT_UUID;

  // Configure the characteristic value's metadata
  memset(&attr_md, 0, sizeof(attr_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
  attr_md.vloc = BLE_GATTS_VLOC_STACK;
  attr_md.rd_auth = 0;
  attr_md.wr_auth = 0;
  attr_md.vlen = 1;

  // Configure the characteristic value
  attr_char_value.p_uuid = &ble_uuid;
  attr_char_value.p_attr_md = &attr_md;
  attr_char_value.init_len = 10;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = VALUE_LEN_BMW_PRESSURE;
  attr_char_value.p_value = NULL;

  return sd_ble_gatts_characteristic_add(p_bmwseat_service->service_handle, &char_md,
      &attr_char_value,
      &p_bmwseat_service->bmw_pressure_handles);
}

/**@brief Function for adding the time characteristic.
 *
 */
static uint32_t bmwseat_motion_raw_add(ble_bmwseat_service_t *p_bmwseat_service, const ble_bmwseat_service_init_t *p_bmwseat_service_init) {
  uint32_t err_code;
  ble_gatts_char_md_t char_md;
  ble_gatts_attr_md_t cccd_md;
  ble_gatts_attr_t attr_char_value;
  ble_uuid_t ble_uuid;
  ble_gatts_attr_md_t attr_md;

  // Configure the CCCD which is needed for Notifications and Indications
  memset(&cccd_md, 0, sizeof(cccd_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
  cccd_md.vloc = BLE_GATTS_VLOC_STACK;

  // Configure the characteristic metadata.
  memset(&char_md, 0, sizeof(char_md));
  char_md.char_props.read = 0;
  char_md.char_props.write = 0;
  char_md.char_props.write_wo_resp = 0;
  char_md.char_props.notify = 1;
  char_md.p_char_user_desc = "raw data";
  char_md.char_user_desc_max_size = 12;
  char_md.char_user_desc_size = 12;
  char_md.p_char_pf = NULL;
  char_md.p_user_desc_md = NULL;
  char_md.p_cccd_md = &cccd_md;
  char_md.p_sccd_md = NULL;

  // Add the biologue Data I/O Characteristic UUID
  ble_uuid128_t base_uuid = {BLE_UUID_MOTION_RAW_BASE_UUID};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_bmwseat_service->uuid_type);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_bmwseat_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_MOTION_RAW_UUID;

  // Configure the characteristic value's metadata
  memset(&attr_md, 0, sizeof(attr_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
  attr_md.vloc = BLE_GATTS_VLOC_STACK;
  attr_md.rd_auth = 0;
  attr_md.wr_auth = 0;
  attr_md.vlen = 1;

  // Configure the characteristic value
  attr_char_value.p_uuid = &ble_uuid;
  attr_char_value.p_attr_md = &attr_md;
  attr_char_value.init_len = 20;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = 30;
  attr_char_value.p_value = NULL;

  return sd_ble_gatts_characteristic_add(p_bmwseat_service->service_handle, &char_md,
      &attr_char_value,
      &p_bmwseat_service->motion_raw_handles);
}




static uint32_t bmwseat_motion_euler_add(ble_bmwseat_service_t *p_bmwseat_service, const ble_bmwseat_service_init_t *p_bmwseat_service_init) {

  uint32_t err_code;
  ble_gatts_char_md_t char_md;
  ble_gatts_attr_md_t cccd_md;
  ble_gatts_attr_t attr_char_value;
  ble_uuid_t ble_uuid;
  ble_gatts_attr_md_t attr_md;

  // Configure the CCCD which is needed for Notifications and Indications
  memset(&cccd_md, 0, sizeof(cccd_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
  cccd_md.vloc = BLE_GATTS_VLOC_STACK;

  // Configure the characteristic metadata.
  memset(&char_md, 0, sizeof(char_md));
  char_md.char_props.read = 0;
  char_md.char_props.write = 0;
  char_md.char_props.write_wo_resp = 0;
  char_md.char_props.notify = 1;
  char_md.p_char_user_desc = "motion euler angle";
  char_md.char_user_desc_max_size = 20;
  char_md.char_user_desc_size = 20;
  char_md.p_char_pf = NULL;
  char_md.p_user_desc_md = NULL;
  char_md.p_cccd_md = &cccd_md;
  char_md.p_sccd_md = NULL;

  // Add the biologue Data I/O Characteristic UUID
  ble_uuid128_t base_uuid = {BLE_UUID_MOTION_EULER_BASE_UUID};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_bmwseat_service->uuid_type);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_bmwseat_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_MOTION_EULER_UUID;

  // Configure the characteristic value's metadata
  memset(&attr_md, 0, sizeof(attr_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
  attr_md.vloc = BLE_GATTS_VLOC_USER;
  attr_md.rd_auth = 0;
  attr_md.wr_auth = 0;
  attr_md.vlen = 0;

  // Configure the characteristic value
  memset(&attr_char_value, 0, sizeof(attr_char_value));
  attr_char_value.p_uuid = &ble_uuid;
  attr_char_value.p_attr_md = &attr_md;
  attr_char_value.init_len = VALUE_LEN_STATUS;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = VALUE_LEN_STATUS;
  attr_char_value.p_value = bmw_status;

  return sd_ble_gatts_characteristic_add(p_bmwseat_service->service_handle, &char_md,
      &attr_char_value,
      &p_bmwseat_service->motion_euler_handles);


}


static uint32_t bmwseat_motion_rotation_add(ble_bmwseat_service_t *p_bmwseat_service, const ble_bmwseat_service_init_t *p_bmwseat_service_init) {
  
  uint32_t err_code;
  ble_gatts_char_md_t char_md;
  ble_gatts_attr_md_t cccd_md;
  ble_gatts_attr_t attr_char_value;
  ble_uuid_t ble_uuid;
  ble_gatts_attr_md_t attr_md;

  // Configure the CCCD which is needed for Notifications and Indications
  memset(&cccd_md, 0, sizeof(cccd_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
  cccd_md.vloc = BLE_GATTS_VLOC_STACK;

  // Configure the characteristic metadata.
  memset(&char_md, 0, sizeof(char_md));
  char_md.char_props.read = 0;
  char_md.char_props.write = 0;
  char_md.char_props.write_wo_resp = 0;
  char_md.char_props.notify = 1;
  char_md.p_char_user_desc = "motion rotation matrix";
  char_md.char_user_desc_max_size = 20;
  char_md.char_user_desc_size = 20;
  char_md.p_char_pf = NULL;
  char_md.p_user_desc_md = NULL;
  char_md.p_cccd_md = &cccd_md;
  char_md.p_sccd_md = NULL;

  // Add the biologue Data I/O Characteristic UUID
  ble_uuid128_t base_uuid = {BLE_UUID_MOTION_ROTATION_BASE_UUID};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_bmwseat_service->uuid_type);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_bmwseat_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_MOTION_ROTATION_UUID;

  // Configure the characteristic value's metadata
  memset(&attr_md, 0, sizeof(attr_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
  attr_md.vloc = BLE_GATTS_VLOC_USER;
  attr_md.rd_auth = 0;
  attr_md.wr_auth = 0;
  attr_md.vlen = 0;

  // Configure the characteristic value
  memset(&attr_char_value, 0, sizeof(attr_char_value));
  attr_char_value.p_uuid = &ble_uuid;
  attr_char_value.p_attr_md = &attr_md;
  attr_char_value.init_len = VALUE_LEN_STATUS;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = VALUE_LEN_STATUS;
  attr_char_value.p_value = bmw_status;

  return sd_ble_gatts_characteristic_add(p_bmwseat_service->service_handle, &char_md,
      &attr_char_value,
      &p_bmwseat_service->bmw_status_handles);





}

static uint32_t bmwseat_motion_heading_add(ble_bmwseat_service_t *p_bmwseat_service, const ble_bmwseat_service_init_t *p_bmwseat_service_init) {

    uint32_t err_code;
  ble_gatts_char_md_t char_md;
  ble_gatts_attr_md_t cccd_md;
  ble_gatts_attr_t attr_char_value;
  ble_uuid_t ble_uuid;
  ble_gatts_attr_md_t attr_md;

  // Configure the CCCD which is needed for Notifications and Indications
  memset(&cccd_md, 0, sizeof(cccd_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
  cccd_md.vloc = BLE_GATTS_VLOC_STACK;

  // Configure the characteristic metadata.
  memset(&char_md, 0, sizeof(char_md));
  char_md.char_props.read = 0;
  char_md.char_props.write = 0;
  char_md.char_props.write_wo_resp = 0;
  char_md.char_props.notify = 1;
  char_md.p_char_user_desc = "motion heading";
  char_md.char_user_desc_max_size = 20;
  char_md.char_user_desc_size = 20;
  char_md.p_char_pf = NULL;
  char_md.p_user_desc_md = NULL;
  char_md.p_cccd_md = &cccd_md;
  char_md.p_sccd_md = NULL;

  // Add the biologue Data I/O Characteristic UUID
  ble_uuid128_t base_uuid = {BLE_UUID_MOTION_HEADING_BASE_UUID};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_bmwseat_service->uuid_type);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_bmwseat_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_MOTION_HEADING_UUID;

  // Configure the characteristic value's metadata
  memset(&attr_md, 0, sizeof(attr_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
  attr_md.vloc = BLE_GATTS_VLOC_USER;
  attr_md.rd_auth = 0;
  attr_md.wr_auth = 0;
  attr_md.vlen = 0;

  // Configure the characteristic value
  memset(&attr_char_value, 0, sizeof(attr_char_value));
  attr_char_value.p_uuid = &ble_uuid;
  attr_char_value.p_attr_md = &attr_md;
  attr_char_value.init_len = VALUE_LEN_STATUS;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = VALUE_LEN_STATUS;
  attr_char_value.p_value = bmw_status;

  return sd_ble_gatts_characteristic_add(p_bmwseat_service->service_handle, &char_md,
      &attr_char_value,
      &p_bmwseat_service->bmw_status_handles);



}

static uint32_t bmwseat_motion_gravity_add(ble_bmwseat_service_t *p_bmwseat_service, const ble_bmwseat_service_init_t *p_bmwseat_service_init) {


  uint32_t err_code;
  ble_gatts_char_md_t char_md;
  ble_gatts_attr_md_t cccd_md;
  ble_gatts_attr_t attr_char_value;
  ble_uuid_t ble_uuid;
  ble_gatts_attr_md_t attr_md;

  // Configure the CCCD which is needed for Notifications and Indications
  memset(&cccd_md, 0, sizeof(cccd_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&cccd_md.write_perm);
  cccd_md.vloc = BLE_GATTS_VLOC_STACK;

  // Configure the characteristic metadata.
  memset(&char_md, 0, sizeof(char_md));
  char_md.char_props.read = 0;
  char_md.char_props.write = 0;
  char_md.char_props.write_wo_resp = 0;
  char_md.char_props.notify = 1;
  char_md.p_char_user_desc = "motion gravity";
  char_md.char_user_desc_max_size = 20;
  char_md.char_user_desc_size = 20;
  char_md.p_char_pf = NULL;
  char_md.p_user_desc_md = NULL;
  char_md.p_cccd_md = &cccd_md;
  char_md.p_sccd_md = NULL;

  // Add the biologue Data I/O Characteristic UUID
  ble_uuid128_t base_uuid = {BLE_UUID_MOTION_GRAVITY_BASE_UUID};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_bmwseat_service->uuid_type);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_bmwseat_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_MOTION_GRAVITY_UUID;

  // Configure the characteristic value's metadata
  memset(&attr_md, 0, sizeof(attr_md));
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.read_perm);
  BLE_GAP_CONN_SEC_MODE_SET_OPEN(&attr_md.write_perm);
  attr_md.vloc = BLE_GATTS_VLOC_STACK;
  attr_md.rd_auth = 0;
  attr_md.wr_auth = 0;
  attr_md.vlen = 1;

  // Configure the characteristic value
  attr_char_value.p_uuid = &ble_uuid;
  attr_char_value.p_attr_md = &attr_md;
  attr_char_value.init_len = 12;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = 100;
  attr_char_value.p_value = NULL;

  return sd_ble_gatts_characteristic_add(p_bmwseat_service->service_handle, &char_md,
      &attr_char_value,
      &p_bmwseat_service->motion_gravity_handles);


}




uint32_t ble_bmwseat_service_init(ble_bmwseat_service_t *p_bmwseat_service, const ble_bmwseat_service_init_t *p_bmwseat_service_init) {
  uint32_t err_code;
  ble_uuid_t ble_uuid;

  // Initialize service structure
  p_bmwseat_service->conn_handle = BLE_CONN_HANDLE_INVALID;
  p_bmwseat_service->evt_handler = p_bmwseat_service_init->evt_handler;

  // Add service
  ble_uuid128_t base_uuid = {BLE_UUID_MOTION_SERVICE_BASE_UUID};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_bmwseat_service->uuid_type);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_bmwseat_service->uuid_type;
  ble_uuid.uuid = BLE_UUID_MOTION_SERVICE_UUID;

  err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY, &ble_uuid, &p_bmwseat_service->service_handle);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  // Add two characteristic
  err_code = control_char_add(p_bmwseat_service, p_bmwseat_service_init);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  err_code = bmw_pressure_char_add(p_bmwseat_service, p_bmwseat_service_init);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  err_code = bmwseat_motion_raw_add(p_bmwseat_service, p_bmwseat_service_init);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  
  err_code = bmwseat_motion_euler_add(p_bmwseat_service, p_bmwseat_service_init);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }
  
    err_code = bmwseat_motion_rotation_add(p_bmwseat_service, p_bmwseat_service_init);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

    err_code = bmwseat_motion_heading_add(p_bmwseat_service, p_bmwseat_service_init);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

    err_code = bmwseat_motion_gravity_add(p_bmwseat_service, p_bmwseat_service_init);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  //    err_code = device_name_char_add(p_bmwseat_service, p_bmwseat_service_init);
  //if (err_code != NRF_SUCCESS) {
  //  return err_code;
  //}



  return NRF_SUCCESS;
}


#include "FreeRTOS.h"
#include "queue.h"


extern QueueHandle_t uart_rx_queue;

static void send_ack_to_phone() {

}
/*
uint32_t ble_bmw_notify(ble_bmwseat_service_t *p_bmwseat_service, uint8_t *p_data, uint8_t data_sz) {
  if (p_bmwseat_service == NULL) {
    return NRF_ERROR_NULL;
  }

  uint32_t err_code = NRF_SUCCESS;
  ble_gatts_value_t gatts_value;

  // Initialize value struct.
  memset(&gatts_value, 0, sizeof(gatts_value));

  gatts_value.len = data_sz;
  gatts_value.offset = 0;
  gatts_value.p_value = p_data;
  
  // Update database.
  err_code = sd_ble_gatts_value_set(p_bmwseat_service->conn_handle,
      p_bmwseat_service->bmw_pressure_handles.value_handle,
      &gatts_value);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  // Send value if connected and notifying.
  if ((p_bmwseat_service->conn_handle != BLE_CONN_HANDLE_INVALID)) {
    ble_gatts_hvx_params_t hvx_params;

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_bmwseat_service->bmw_pressure_handles.value_handle;
    hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = gatts_value.offset;
    hvx_params.p_len = &gatts_value.len;
    hvx_params.p_data = gatts_value.p_value;

    err_code = sd_ble_gatts_hvx(p_bmwseat_service->conn_handle, &hvx_params);
    //NRF_LOG_INFO("sd_ble_gatts_hvx result: %x. \r\n", err_code);
  } 
  else {
    err_code = NRF_ERROR_INVALID_STATE;
    //NRF_LOG_INFO("sd_ble_gatts_hvx result: NRF_ERROR_INVALID_STATE. \r\n");
  }

  return err_code;
}

*/



uint32_t ble_motion_raw_notify(ble_bmwseat_service_t *p_bmwseat_service, uint8_t *p_data, uint8_t data_sz) {
  if (p_bmwseat_service == NULL) {
    return NRF_ERROR_NULL;
  }

  uint32_t err_code = NRF_SUCCESS;
  ble_gatts_value_t gatts_value;

  // Initialize value struct.
  memset(&gatts_value, 0, sizeof(gatts_value));

  gatts_value.len = data_sz;
  gatts_value.offset = 0;
  gatts_value.p_value = p_data;
  
  // Update database.
  err_code = sd_ble_gatts_value_set(p_bmwseat_service->conn_handle,
      p_bmwseat_service->motion_raw_handles.value_handle,
      &gatts_value);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  // Send value if connected and notifying.
  if ((p_bmwseat_service->conn_handle != BLE_CONN_HANDLE_INVALID)) {
    ble_gatts_hvx_params_t hvx_params;

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_bmwseat_service->motion_raw_handles.value_handle;
    hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = gatts_value.offset;
    hvx_params.p_len = &gatts_value.len;
    hvx_params.p_data = gatts_value.p_value;

    err_code = sd_ble_gatts_hvx(p_bmwseat_service->conn_handle, &hvx_params);
    //NRF_LOG_INFO("sd_ble_gatts_hvx result: %x. \r\n", err_code);
  } 
  else {
    err_code = NRF_ERROR_INVALID_STATE;
    //NRF_LOG_INFO("sd_ble_gatts_hvx result: NRF_ERROR_INVALID_STATE. \r\n");
  }

  return err_code;
}




uint32_t ble_motion_gravity_notify(ble_bmwseat_service_t *p_bmwseat_service, uint8_t *p_data, uint8_t data_sz) {
  if (p_bmwseat_service == NULL) {
    return NRF_ERROR_NULL;
  }

  uint32_t err_code = NRF_SUCCESS;
  ble_gatts_value_t gatts_value;

  // Initialize value struct.
  memset(&gatts_value, 0, sizeof(gatts_value));

  gatts_value.len = data_sz;
  gatts_value.offset = 0;
  gatts_value.p_value = p_data;
  
  // Update database.
  err_code = sd_ble_gatts_value_set(p_bmwseat_service->conn_handle,
      p_bmwseat_service->motion_gravity_handles.value_handle,
      &gatts_value);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  // Send value if connected and notifying.
  if ((p_bmwseat_service->conn_handle != BLE_CONN_HANDLE_INVALID)) {
    ble_gatts_hvx_params_t hvx_params;

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_bmwseat_service->motion_gravity_handles.value_handle;
    hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = gatts_value.offset;
    hvx_params.p_len = &gatts_value.len;
    hvx_params.p_data = gatts_value.p_value;

    err_code = sd_ble_gatts_hvx(p_bmwseat_service->conn_handle, &hvx_params);
    //NRF_LOG_INFO("sd_ble_gatts_hvx result: %x. \r\n", err_code);
  } 
  else {
    err_code = NRF_ERROR_INVALID_STATE;
    //NRF_LOG_INFO("sd_ble_gatts_hvx result: NRF_ERROR_INVALID_STATE. \r\n");
  }

  return err_code;
}













/*
 {
  static mavlink_status_t status;
  mavlink_bmwseat_control_t control;
  

  __ALIGN(4) static uint8_t ch_name[VALUE_LEN_BLE_NAME];
#ifdef DEBUG
  for(int i=0;i<data_sz;i++) {
    NRF_LOG_INFO("rec[%d]: %d", i, p_data[i]);
  }
#endif
  if(is_bootloader_packet(p_data, data_sz)) {
    restart_goto_bootloader();
  }

   Use in test start 
  if(*p_data == 0)
    set_boot_pin_low();
  else
    set_boot_pin_high();
   Use in test end 

  uint32_t err_code;

  uint8_t parse_state;
  // Forward to MCU, or control the bluetooth information
  for(uint16_t i=0;i<data_sz;i++) {
      //NRF_LOG_INFO("%x", p_data[i]);
      parse_state = mavlink_parse_char(MAVLINK_COMM_0, p_data[i], &rmsg, &status);
    
      if(parse_state) {
        switch (rmsg.msgid) {
          // Bluetooth control
          case MAVLINK_MSG_ID_BLUETOOTH_CHANGE_NAME:
            NRF_LOG_INFO("Change BLE name.");
            memset(ch_name, 0, sizeof(ch_name)*sizeof(uint8_t));
            mavlink_msg_bluetooth_change_name_get_name(&rmsg, ch_name);
            write_version_device_name_to_flash(BMW_SEAT_VERSION, ch_name,  strlen(ch_name));
#if DEBUG
            if(err_code != NRF_SUCCESS) {
              NRF_LOG_INFO("Fail to change th name.");
            }
#endif
            break;
          
          // BMWSeat control
          case MAVLINK_MSG_ID_BMWSEAT_CONTROL:
          default:
            mavlink_msg_bmwseat_control_decode(&rmsg, &control);
            if(control.cmd_and_motorn == 0xAAAAAAAA) {
              if(control.value == 0x55555555) {
                bmw_status[3] = 1;
                // Boot pin low
                set_boot_pin_low();
                // Send reset signal to MCU
                //@Warning Use boot pin trigger interrupt
              }
              else if(control.value == 0x66666666) {
              bmw_status[3] = 0;
                set_boot_pin_high();
              }
              send_ack_to_phone();
              break;
            }
            err_code = send_message(p_data, data_sz);
            if(err_code != NRF_SUCCESS) {
              NRF_LOG_INFO("Control, send to MCU fail.");
            }
            break;
        }
      }
    }

}

*/

//nrf_gpio_pin_clear(MCU_RESET_PIN);
//nrf_gpio_cfg_output(MCU_RESET_PIN);
//set_boot_pin_low();
//set_boot_pin_high();

//void restart_goto_bootloader(){
  

//}


uint32_t ble_bmwseat_control_handle(uint8_t *p_data, uint8_t data_sz) {
  
}

uint32_t ble_bmw_notify(ble_bmwseat_service_t *p_bmwseat_service, uint8_t *p_data, uint8_t data_sz) {
  if (p_bmwseat_service == NULL) {
    return NRF_ERROR_NULL;
  }

  uint32_t err_code = NRF_SUCCESS;
  ble_gatts_value_t gatts_value;

  // Initialize value struct.
  memset(&gatts_value, 0, sizeof(gatts_value));

  gatts_value.len = data_sz;
  gatts_value.offset = 0;
  gatts_value.p_value = p_data;
  
  // Update database.
  err_code = sd_ble_gatts_value_set(p_bmwseat_service->conn_handle,
      p_bmwseat_service->bmw_pressure_handles.value_handle,
      &gatts_value);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  // Send value if connected and notifying.
  if ((p_bmwseat_service->conn_handle != BLE_CONN_HANDLE_INVALID)) {
    ble_gatts_hvx_params_t hvx_params;

    memset(&hvx_params, 0, sizeof(hvx_params));

    hvx_params.handle = p_bmwseat_service->bmw_pressure_handles.value_handle;
    hvx_params.type = BLE_GATT_HVX_NOTIFICATION;
    hvx_params.offset = gatts_value.offset;
    hvx_params.p_len = &gatts_value.len;
    hvx_params.p_data = gatts_value.p_value;

    err_code = sd_ble_gatts_hvx(p_bmwseat_service->conn_handle, &hvx_params);
    //NRF_LOG_INFO("sd_ble_gatts_hvx result: %x. \r\n", err_code);
  } 
  else {
    err_code = NRF_ERROR_INVALID_STATE;
    //NRF_LOG_INFO("sd_ble_gatts_hvx result: NRF_ERROR_INVALID_STATE. \r\n");
  }

  return err_code;
}

uint32_t write_version_device_name_to_flash(uint8_t version, uint8_t *dname, uint8_t dname_len) {


}
