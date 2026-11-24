#include "lumentree_switch.h"

namespace esphome::lumentree_ble {

void LumentreeSwitch::write_state(bool state) {
  this->parent_->write_register(this->holding_register_, state ? 0x0001 : 0x0000);
  // Optimistic update; the next poll confirms the state read back from the device
  this->publish_state(state);
}

}  // namespace esphome::lumentree_ble
