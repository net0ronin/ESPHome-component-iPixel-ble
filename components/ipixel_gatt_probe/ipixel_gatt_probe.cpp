#include "ipixel_gatt_probe.h"

#include <cstdlib>

namespace esphome {
namespace ipixel_gatt_probe {

static const char *const TAG = "ipixel_gatt_probe";

static void log_uuid_(const char *kind, const esp_bt_uuid_t &uuid, uint16_t handle, uint8_t properties) {
  if (uuid.len == ESP_UUID_LEN_16) {
    ESP_LOGI(TAG, "%s handle=0x%04X uuid=0x%04X properties=0x%02X", kind, handle, uuid.uuid.uuid16, properties);
    return;
  }
  if (uuid.len == ESP_UUID_LEN_128) {
    char value[48];
    snprintf(value, sizeof(value),
             "%02X%02X%02X%02X-%02X%02X-%02X%02X-%02X%02X-%02X%02X%02X%02X%02X%02X",
             uuid.uuid.uuid128[15], uuid.uuid.uuid128[14], uuid.uuid.uuid128[13], uuid.uuid.uuid128[12],
             uuid.uuid.uuid128[11], uuid.uuid.uuid128[10], uuid.uuid.uuid128[9], uuid.uuid.uuid128[8],
             uuid.uuid.uuid128[7], uuid.uuid.uuid128[6], uuid.uuid.uuid128[5], uuid.uuid.uuid128[4],
             uuid.uuid.uuid128[3], uuid.uuid.uuid128[2], uuid.uuid.uuid128[1], uuid.uuid.uuid128[0]);
    ESP_LOGI(TAG, "%s handle=0x%04X uuid=%s properties=0x%02X", kind, handle, value, properties);
    return;
  }
  ESP_LOGI(TAG, "%s handle=0x%04X uuid_len=%d properties=0x%02X", kind, handle, uuid.len, properties);
}

void IPixelGATTProbe::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                          esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_CONNECT_EVT:
      ESP_LOGI(TAG, "Connected: conn_id=%d remote=" ESP_BD_ADDR_STR, param->connect.conn_id,
               ESP_BD_ADDR_HEX(param->connect.remote_bda));
      break;
    case ESP_GATTC_CFG_MTU_EVT:
      ESP_LOGI(TAG, "MTU: status=%d mtu=%d", param->cfg_mtu.status, param->cfg_mtu.mtu);
      break;
    case ESP_GATTC_SEARCH_RES_EVT:
      ESP_LOGI(TAG, "Service: start=0x%04X end=0x%04X uuid_len=%d uuid16=0x%04X primary=%d",
               param->search_res.start_handle, param->search_res.end_handle, param->search_res.srvc_id.uuid.len,
               param->search_res.srvc_id.uuid.uuid.uuid16, param->search_res.is_primary);
      break;
    case ESP_GATTC_SEARCH_CMPL_EVT: {
      ESP_LOGI(TAG, "Service discovery complete: status=%d conn_id=%d", param->search_cmpl.status,
               param->search_cmpl.conn_id);
      uint16_t count = 0;
      esp_gatt_status_t status = esp_ble_gattc_get_attr_count(
          gattc_if, param->search_cmpl.conn_id, ESP_GATT_DB_CHARACTERISTIC, 1, 0xFFFF, ESP_GATT_INVALID_HANDLE, &count);
      ESP_LOGI(TAG, "Characteristic count: status=%d count=%d", status, count);
      if (status != ESP_GATT_OK || count == 0) break;
      auto *chars = static_cast<esp_gattc_char_elem_t *>(std::calloc(count, sizeof(esp_gattc_char_elem_t)));
      if (chars == nullptr) break;
      uint16_t found = count;
      status = esp_ble_gattc_get_all_char(gattc_if, param->search_cmpl.conn_id, 1, 0xFFFF, chars, &found,
                                          ESP_GATT_INVALID_OFFSET);
      ESP_LOGI(TAG, "Characteristic enumeration: status=%d count=%d", status, found);
      if (status == ESP_GATT_OK) {
        for (uint16_t i = 0; i < found; i++) log_uuid_("Characteristic", chars[i].uuid, chars[i].char_handle, chars[i].properties);
      }
      std::free(chars);
      break;
    }
    case ESP_GATTC_DISCONNECT_EVT:
      ESP_LOGW(TAG, "Disconnected: reason=%d", param->disconnect.reason);
      break;
    default:
      break;
  }
}

}  // namespace ipixel_gatt_probe
}  // namespace esphome
