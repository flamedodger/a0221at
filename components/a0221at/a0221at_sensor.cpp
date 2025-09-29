#include "a0221at_sensor.h"
#include "esphome/core/log.h"
#include <cstdlib>

namespace esphome {
namespace a0221at {

static const char *const TAG = "a0221at.sensor";

void A0221ATSensor::set_uart_parent(esphome::uart::UARTComponent *parent) {
  this->uart_ = parent;
}

void A0221ATSensor::update() {
  static std::string buffer;

  ESP_LOGD(TAG, "Update triggered, UART available: %d", this->uart_->available());

  while (this->uart_->available()) {
    uint8_t c;
    if (!this->uart_->read_byte(&c)) break;
    ESP_LOGD(TAG, "Byte: 0x%02X (%c)", c, static_cast<char>(c));
    buffer += static_cast<char>(c);

    if (c == '\n') {
      std::string line = buffer;
      buffer.clear();

      if (!line.empty() && line.back() == '\r') {
        line.pop_back();
      }

      ESP_LOGD(TAG, "Raw line: '%s'", line.c_str());

      float value = this->parse_sensor_value(line);
      if (!std::isnan(value)) {
        ESP_LOGD(TAG, "Parsed value: %.2f", value);
        this->publish_state(value);
      } else {
        ESP_LOGW(TAG, "Failed to parse sensor value from line: '%s'", line.c_str());
      }
    }
  }
}

float A0221ATSensor::parse_sensor_value(const std::string &line) {
  char *end;
  float value = std::strtof(line.c_str(), &end);
  if (end == line.c_str() || std::isnan(value)) {
    return NAN;
  }
  return value;
}

}  // namespace a0221at
}  // namespace esphome

