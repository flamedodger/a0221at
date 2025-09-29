#pragma once

#include "esphome/components/sensor/sensor.h"
#include "esphome/components/uart/uart.h"
#include "esphome/core/component.h"

namespace esphome {
namespace a0221at {

class A0221ATSensor : public sensor::Sensor, public PollingComponent {
 public:
  explicit A0221ATSensor(uart::UARTComponent *uart) : uart_(uart) {}

  void setup() override;
  void update() override;

 protected:
  uart::UARTComponent *uart_;
};

}  // namespace a0221at
}  // namespace esphome