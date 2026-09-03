#include "a0221at_sensor.h"
#include "esphome/core/log.h"
#include <cstdlib>
#include <vector>
#include <cmath>
#include <cstdio>

#define A0221AT_FRAME_LENGTH 4

namespace esphome {
namespace a0221at {

static const char *const TAG = "a0221at.sensor";

static bool crc_ok(const std::vector<uint8_t> &frame) {
    uint8_t expected_crc = (uint8_t) ((frame[0] + frame[1] + frame[2]) & 0xFF);
    return expected_crc == frame[3];
}

void A0221ATSensor::set_uart_parent(esphome::uart::UARTComponent *parent) {
  this->uart_ = parent;
}

void A0221ATSensor::update() {
  // Send trigger command — adjust if needed
  if (this->uart_ == nullptr) {
    ESP_LOGW(TAG, "UART parent not set");
    return;
  }

  ESP_LOGD(TAG, "Triggering sensor read...");
  this->uart_->flush();
  const char *trigger = "R\r\n";  // Some models use "\r" or "U"
  this->uart_->write_str(trigger);
  ESP_LOGD(TAG, "Sent trigger: '%s'", trigger);

  delay(75);  // Allow sensor time to respond

  std::vector<uint8_t> raw;
  while (this->uart_->available()) {
    uint8_t c;
    if (!this->uart_->read_byte(&c))
      break;
    raw.push_back(c);
    ESP_LOGD(TAG, "Byte: 0x%02X (%c)", c, static_cast<char>(c));
  }

  if (raw.empty()) {
    ESP_LOGW(TAG, "No response from sensor");
    return;
  }

  ESP_LOGD(TAG, "Received %d bytes", raw.size());

  std::string hex;
  for (auto b : raw) {
    char buf[5];
    snprintf(buf, sizeof(buf), "%02X ", b);
    hex += buf;
  }
  ESP_LOGD(TAG, "Raw packet: %s", hex.c_str());

  // Optional: parse known binary format here
  // Example: if raw[0] == 0xFF and raw[1] == high byte, raw[2] == low byte
  if (raw.size() < A0221AT_FRAME_LENGTH || raw[0] != 0xFF) {
    ESP_LOGW(TAG, "Unexpected packet format");
    return;
  }

  if (!crc_ok(raw)) {
    ESP_LOGW(TAG, "CRC check failed");
    return;
  }

  uint16_t mm = (raw[1] << 8) | raw[2];
  float cm = mm / 10.0f;
  ESP_LOGD(TAG, "Parsed distance: %.1f cm", cm);
  this->publish_state(cm);
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

