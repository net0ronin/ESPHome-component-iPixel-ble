#include "ipixel_gatt_probe.h"

namespace esphome {
namespace ipixel_gatt_probe {

static const char *const TAG = "ipixel_gatt_probe";

void IPixelGATTProbe::gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                                          esp_ble_gattc_cb_param_t *param) {
  switch (event) {
    case ESP_GATTC_OPEN_EVT:
      ESP_LOGI(TAG, "GATT open: status=%d conn_id=%d mtu=%d", param->open.status, param->open.conn_id,
               param->open.mtu);
      break;
    case ESP_GATTC_CONNECT_EVT:
      ESP_LOGI(TAG, "GATT connected: conn_id=%d", param->connect.conn_id);
      break;
    case ESP_GATTC_SEARCH_RES_EVT:
      ESP_LOGI(TAG, "Service: start=%d end=%d uuid_len=%d uuid16=0x%04X primary=%d",
               param->search_res.start_handle, param->search_res.end_handle, param->search_res.srvc_id.uuid.len,
               param->search_res.srvc_id.uuid.uuid.uuid16, param->search_res.is_primary);
      break;
    case ESP_GATTC_SEARCH_CMPL_EVT:
      ESP_LOGI(TAG, "Service discovery complete: status=%d conn_id=%d", param->search_cmpl.status,
               param->search_cmpl.conn_id);
      break;
    case ESP_GATTC_CFG_MTU_EVT:
      ESP_LOGI(TAG, "MTU configured: status=%d mtu=%d", param->cfg_mtu.status, param->cfg_mtu.mtu);
      break;
    case ESP_GATTC_DISCONNECT_EVT:
      ESP_LOGW(TAG, "GATT disconnected: reason=%d conn_id=%d", param->disconnect.reason,
               param->disconnect.conn_id);
      break;
    default:
      break;
  }
}

}  // namespace ipixel_gatt_probe
}  // namespace esphome
