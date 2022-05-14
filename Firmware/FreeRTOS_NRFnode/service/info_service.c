#include "info_service.h"


#include "ble_srv_common.h"
#include "nordic_common.h"
#include "nrf_log.h"
#include "nrf_soc.h"

static ret_code_t err_code;

static uint32_t device_name_add(ble_info_service_t * p_tcs, const ble_info_service_init_t * p_tcs_init);
static uint32_t device_adv_add(ble_info_service_t * p_tcs, const ble_info_service_init_t * p_tcs_init);


ret_code_t ble_info_service_init(ble_info_service_t *p_blink,const ble_info_service_init_t *p_info_service_init)
{
        ret_code_t err_code;
        ble_uuid_t ble_uuid;

        ble_uuid128_t base_uuid = {INFO_SERVICE_UUID_BASE};

        if (!p_blink)
                return NRF_ERROR_INVALID_PARAM;

       
        err_code = sd_ble_uuid_vs_add(&base_uuid, &p_blink->service_type);
        VERIFY_SUCCESS(err_code);

         if(err_code == 4){
        while(1);   //if add failed checkout NRF_SDH_BLE_VS_UUID_COUNT
       
       }
        
        ble_uuid.type = p_blink->service_type;
        ble_uuid.uuid = INFO_SERVICE_UUID_SERVICE;
        err_code = sd_ble_gatts_service_add(BLE_GATTS_SRVC_TYPE_PRIMARY,
                                            &ble_uuid,
                                            &p_blink->service_handle);
       if(err_code == 4){
        while(1);   //if add failed checkout NRF_SDH_BLE_GATTS_ATTR_TAB_SIZE
       
       }
        err_code = device_name_add(p_blink, p_info_service_init);
         if (err_code != NRF_SUCCESS) {
          return err_code; //if add failed checkout RAMSIZE
        }

       if(err_code == 4){
        while(1);
       
       }
       err_code = device_adv_add(p_blink, p_info_service_init);
             if(err_code == 4){
        while(1);
       
       }

        //VERIFY_SUCCESS(err_code);

        return err_code;
}





uint8_t device_name[]="Thingy\n";

/*
https://github.com/NordicSemiconductor/IOS-Nordic-Thingy/blob/f80da4ff02f9aaaa89cbc25df60768cb83a81ebf/IOSThingyLibrary/Classes/Manager/ThingyManager.swift#L166
*/
static uint32_t device_name_add(ble_info_service_t * p_tcs, const ble_info_service_init_t * p_tcs_init)
{

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
  char_md.char_props.read = 1;
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
  ble_uuid128_t base_uuid = {INFO_SERVICE_UUID_BASE};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_tcs_init->evt_handler);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_tcs->service_type;
  ble_uuid.uuid = BLE_UUID_DEVICE_NAME_UUID;

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
  attr_char_value.init_len = 7;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = 7;
  attr_char_value.p_value = device_name;

  return sd_ble_gatts_characteristic_add(p_tcs->service_handle, &char_md,
      &attr_char_value,
      &p_tcs->device_name_handle);

}


static uint32_t device_adv_add(ble_info_service_t * p_tcs, const ble_info_service_init_t * p_tcs_init)
{

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
  char_md.char_props.read = 1;
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
  ble_uuid128_t base_uuid = {INFO_SERVICE_UUID_BASE};
  err_code = sd_ble_uuid_vs_add(&base_uuid, &p_tcs_init->evt_handler);
  if (err_code != NRF_SUCCESS) {
    return err_code;
  }

  ble_uuid.type = p_tcs->service_type;
  ble_uuid.uuid = INFO_SERVICE_ADV_UUID;

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
  attr_char_value.init_len = 20;
  attr_char_value.init_offs = 0;
  attr_char_value.max_len = 20;
  attr_char_value.p_value = NULL;

  return sd_ble_gatts_characteristic_add(p_tcs->service_handle, &char_md,
      &attr_char_value,
      &p_tcs->adv_param_handle);

}