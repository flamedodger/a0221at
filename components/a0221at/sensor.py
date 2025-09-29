import esphome.codegen as cg
import esphome.config_validation as cv
from esphome import sensor, uart
from esphome.const import (
    CONF_ID,
    CONF_NAME,
    CONF_UNIT_OF_MEASUREMENT,
    CONF_ACCURACY_DECIMALS,
    CONF_UPDATE_INTERVAL,
)

DEPENDENCIES = ["uart"]

a0221at_ns = cg.esphome_ns.namespace("a0221at")
A0221ATSensor = a0221at_ns.class_("A0221ATSensor", sensor.Sensor, cg.Component)

CONFIG_SCHEMA = cv.Schema({
    cv.GenerateID(CONF_ID): cv.declare_id(A0221ATSensor),
    cv.Required(CONF_NAME): cv.string,
    cv.Optional(CONF_UNIT_OF_MEASUREMENT, default="cm"): cv.string,
    cv.Optional(CONF_ACCURACY_DECIMALS, default=1): cv.positive_int,
    cv.Optional(CONF_UPDATE_INTERVAL, default="1s"): cv.update_interval,
}).extend(uart.UART_DEVICE_SCHEMA)

async def to_code(config):
    uart_component = await uart.get_uart_component(config)
    var = cg.new_Pvariable(config[CONF_ID], uart_component)
    await sensor.register_sensor(var, config)
    await cg.register_component(var, config)

