import esphome.codegen as cg
import esphome.config_validation as cv
from esphome.components import sensor, uart
from esphome.const import CONF_NAME, CONF_UART_ID, UNIT_MILLIMETER, ICON_RULER

DEPENDENCIES = ["uart"]

a0221at_ns = cg.esphome_ns.namespace("a0221at")
A0221ATSensor = a0221at_ns.class_("A0221ATSensor", sensor.Sensor, cg.PollingComponent)

CONFIG_SCHEMA = sensor.sensor_schema(
    unit_of_measurement=UNIT_MILLIMETER,
    icon=ICON_RULER,
    accuracy_decimals=0
).extend({
    cv.GenerateID(): cv.declare_id(A0221ATSensor),
    cv.GenerateID(CONF_UART_ID): cv.use_id(uart.UARTComponent),
}).extend(cv.polling_component_schema("1s"))

async def to_code(config):
    uart_component = await cg.get_variable(config[CONF_UART_ID])
    var = cg.new_Pvariable(config[CONF_ID], uart_component)
    await cg.register_component(var, config)
    await sensor.register_sensor(var, config)