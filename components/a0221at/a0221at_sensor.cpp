#include "a0221at_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace a0221at {

static const char *const TAG = "a0221at.sensor";

void A0221ATSensor::update() {
  std::string line;
  while (this->uart_->available()) {
    uint8_t c;
    if (!this->uart_->read_byte(&c))
      break;
    if (c == '\n')
      break;
    line += static_cast<char>(c);
  }

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

float A0221ATSensor::parse_sensor_value(const std::string &line) {
  try {
    return std::stof(line);
  } catch (...) {
    return NAN;
  }
}

}  // namespace a0221at
}  // namespace esphome

