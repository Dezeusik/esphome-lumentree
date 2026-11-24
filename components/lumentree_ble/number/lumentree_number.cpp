#include "lumentree_number.h"
#include "esphome/core/log.h"

#include <cmath>

// Fallback for ESPHome < 2026.10.0
#ifndef ESPHOME_LOG_TAG
#define ESPHOME_LOG_TAG(name, tag) static const char *const name = tag
#endif

namespace esphome::lumentree_ble {

ESPHOME_LOG_TAG(TAG, "lumentree_ble.number");

void LumentreeNumber::dump_config() { LOG_NUMBER("", "LumentreeBle Number", this); }

void LumentreeNumber::control(float value) {
  // Round to the nearest raw value instead of truncating (guards against float error)
  auto raw = (uint16_t) std::lround(value / this->factor_);
  ESP_LOGD(TAG, "Writing %.2f (raw %u) to register %u", value, raw, this->holding_register_);
  this->parent_->write_register(this->holding_register_, raw);
  // Optimistic update; the next poll confirms the value read back from the device
  this->publish_state(value);
}

}  // namespace esphome::lumentree_ble
