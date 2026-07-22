#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"

namespace esphome {
namespace ipixel_gatt_probe {

class IPixelGATTProbe : public Component, public ble_client::BLEClientNode {
 public:
  void setup() override;
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;

 protected:
  void dump_characteristics_();
  esp_gatt_if_t gattc_if_{ESP_GATT_IF_NONE};
  uint16_t conn_id_{0};
  bool connected_{false};
};

}  // namespace ipixel_gatt_probe
}  // namespace esphome
