import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import ble_client

DEPENDENCIES = ["ble_client", "esp32_ble_tracker"]

ipixel_gatt_probe_ns = cg.esphome_ns.namespace("ipixel_gatt_probe")
IPixelGATTProbe = ipixel_gatt_probe_ns.class_(
    "IPixelGATTProbe", cg.Component, ble_client.BLEClientNode
)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(): cv.declare_id(IPixelGATTProbe),
}).extend(cv.COMPONENT_SCHEMA).extend(ble_client.BLE_CLIENT_SCHEMA)


async def to_code(config):
    var = cg.new_Pvariable(config["id"])
    await cg.register_component(var, config)
    await ble_client.register_ble_node(var, config)
