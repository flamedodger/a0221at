#include "a0221at_sensor.h"
#include "esphome/core/log.h"

namespace esphome {
namespace a0221at {

static const char *const TAG = "a0221at.sensor";

void A0221ATSensor::setup() {
  ESP_LOGD(TAG, "Setting up A0221AT sensor...");
}

void A0221ATSensor::update() {
  if (uart_ == nullptr) {
    ESP_LOGW(TAG, "UART not initialized");
    return;
  }

  std::string line = uart_->read_line();
  if (line.empty()) {
    ESP_LOGW(TAG, "No data received");
    return;
  }

  // Example: parse "Distance:1234" → 1234
  size_t pos = line.find(":");
  if (pos != std::string::npos) {
    std::string value_str = line.substr(pos + 1);
    float value = atof(value_str.c_str());
    ESP_LOGD(TAG, "Parsed distance: %.2f mm", value);
    publish_state(value);
  } else {
    ESP_LOGW(TAG, "Unexpected format: %s", line.c_str());
  }
}

}  // namespace a0221at
}  // namespace esphome