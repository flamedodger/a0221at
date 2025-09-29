#pragma once

#include "esphome/components/uart/uart.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/core/component.h"

namespace esphome {
namespace a0221at {

class A0221ATSensor : public sensor::Sensor, public PollingComponent {
 public:
  void update() override;
  float parse_sensor_value(const std::string &line);
  void set_uart_parent(esphome::uart::UARTComponent *parent);

 protected:
  esphome::uart::UARTComponent *uart_;
};

}  // namespace a0221at
}  // namespace esphome

