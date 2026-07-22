#pragma once

#include "esphome/core/component.h"
#include "esphome/components/ble_client/ble_client.h"

namespace esphome {
namespace ipixel_gatt_probe {

class IPixelGATTProbe : public Component, public ble_client::BLEClientNode {
 public:
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
};

}  // namespace ipixel_gatt_probe
}  // namespace esphome
