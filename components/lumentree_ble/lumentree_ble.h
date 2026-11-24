#pragma once

#include "esphome/core/component.h"
#include "esphome/components/binary_sensor/binary_sensor.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/button/button.h"
#include "esphome/components/number/number.h"
#include "esphome/components/switch/switch.h"
#include "esphome/components/select/select.h"

#ifdef USE_ESP32
#include "esphome/components/ble_client/ble_client.h"
#include "esphome/components/esp32_ble_tracker/esp32_ble_tracker.h"
#include <esp_gattc_api.h>
namespace espbt = esphome::esp32_ble_tracker;
#endif

namespace esphome::lumentree_ble {

class LumentreeBle :
#ifdef USE_ESP32
    public esphome::ble_client::BLEClientNode,
#endif
    public PollingComponent {
 public:
  static constexpr uint16_t BMS_REGISTER_START = 185;
  static constexpr uint16_t BMS_REGISTER_COUNT = 13;  // 185-197

  enum RequestType {
    REQUEST_SYSTEM_STATUS,
    REQUEST_BATTERY_CONFIG,
    REQUEST_SYSTEM_CONTROL,
    REQUEST_DAILY_STATISTICS,
    REQUEST_BMS,
    REQUEST_COMPLETE
  };

#ifdef USE_ESP32
  void gattc_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if,
                           esp_ble_gattc_cb_param_t *param) override;
#endif
  void dump_config() override;
  void update() override;
  float get_setup_priority() const override { return setup_priority::DATA; }

  void set_battery_voltage_sensor(sensor::Sensor *battery_voltage_sensor) {
    battery_voltage_sensor_ = battery_voltage_sensor;
  }
  void set_battery_current_sensor(sensor::Sensor *battery_current_sensor) {
    battery_current_sensor_ = battery_current_sensor;
  }
  void set_battery_power_sensor(sensor::Sensor *battery_power_sensor) { battery_power_sensor_ = battery_power_sensor; }
  void set_battery_soc_sensor(sensor::Sensor *battery_soc_sensor) { battery_soc_sensor_ = battery_soc_sensor; }
  void set_ac_output_voltage_sensor(sensor::Sensor *ac_output_voltage_sensor) {
    ac_output_voltage_sensor_ = ac_output_voltage_sensor;
  }
  void set_ac_input_voltage_sensor(sensor::Sensor *ac_input_voltage_sensor) {
    ac_input_voltage_sensor_ = ac_input_voltage_sensor;
  }
  void set_ac_output_frequency_sensor(sensor::Sensor *ac_output_frequency_sensor) {
    ac_output_frequency_sensor_ = ac_output_frequency_sensor;
  }
  void set_ac_input_frequency_sensor(sensor::Sensor *ac_input_frequency_sensor) {
    ac_input_frequency_sensor_ = ac_input_frequency_sensor;
  }
  void set_ac_power_sensor(sensor::Sensor *ac_power_sensor) { ac_power_sensor_ = ac_power_sensor; }
  void set_pv_voltage_sensor(sensor::Sensor *pv_voltage_sensor) { pv_voltage_sensor_ = pv_voltage_sensor; }
  void set_pv_power_sensor(sensor::Sensor *pv_power_sensor) { pv_power_sensor_ = pv_power_sensor; }
  void set_grid_power_sensor(sensor::Sensor *grid_power_sensor) { grid_power_sensor_ = grid_power_sensor; }
  void set_load_power_sensor(sensor::Sensor *load_power_sensor) { load_power_sensor_ = load_power_sensor; }
  void set_device_temperature_sensor(sensor::Sensor *device_temperature_sensor) {
    device_temperature_sensor_ = device_temperature_sensor;
  }
  void set_pv2_voltage_sensor(sensor::Sensor *pv2_voltage_sensor) { pv2_voltage_sensor_ = pv2_voltage_sensor; }
  void set_pv2_power_sensor(sensor::Sensor *pv2_power_sensor) { pv2_power_sensor_ = pv2_power_sensor; }
  void set_device_type_image_sensor(sensor::Sensor *device_type_image_sensor) {
    device_type_image_sensor_ = device_type_image_sensor;
  }
  void set_battery_status_sensor(sensor::Sensor *battery_status_sensor) {
    battery_status_sensor_ = battery_status_sensor;
  }
  void set_grid_connection_status_sensor(sensor::Sensor *grid_connection_status_sensor) {
    grid_connection_status_sensor_ = grid_connection_status_sensor;
  }
  void set_device_type_sensor(sensor::Sensor *device_type_sensor) { device_type_sensor_ = device_type_sensor; }
  void set_device_power_rating_code_sensor(sensor::Sensor *device_power_rating_code_sensor) {
    device_power_rating_code_sensor_ = device_power_rating_code_sensor;
  }
  void set_device_power_rating_sensor(sensor::Sensor *device_power_rating_sensor) {
    device_power_rating_sensor_ = device_power_rating_sensor;
  }
  void set_ac_output_apparent_power_sensor(sensor::Sensor *ac_output_apparent_power_sensor) {
    ac_output_apparent_power_sensor_ = ac_output_apparent_power_sensor;
  }
  void set_grid_ct_power_sensor(sensor::Sensor *grid_ct_power_sensor) { grid_ct_power_sensor_ = grid_ct_power_sensor; }
  void set_today_pv_production_sensor(sensor::Sensor *today_pv_production_sensor) {
    today_pv_production_sensor_ = today_pv_production_sensor;
  }
  void set_today_essential_load_sensor(sensor::Sensor *today_essential_load_sensor) {
    today_essential_load_sensor_ = today_essential_load_sensor;
  }
  void set_today_total_consumption_sensor(sensor::Sensor *today_total_consumption_sensor) {
    today_total_consumption_sensor_ = today_total_consumption_sensor;
  }
  void set_today_grid_consumption_sensor(sensor::Sensor *today_grid_consumption_sensor) {
    today_grid_consumption_sensor_ = today_grid_consumption_sensor;
  }
  void set_today_battery_charging_sensor(sensor::Sensor *today_battery_charging_sensor) {
    today_battery_charging_sensor_ = today_battery_charging_sensor;
  }
  void set_today_battery_discharge_sensor(sensor::Sensor *today_battery_discharge_sensor) {
    today_battery_discharge_sensor_ = today_battery_discharge_sensor;
  }
  void set_grid_export_sensor(sensor::Sensor *grid_export_sensor) { grid_export_sensor_ = grid_export_sensor; }

  void set_grid_connected_binary_sensor(binary_sensor::BinarySensor *grid_connected_binary_sensor) {
    grid_connected_binary_sensor_ = grid_connected_binary_sensor;
  }
  void set_battery_connected_binary_sensor(binary_sensor::BinarySensor *battery_connected_binary_sensor) {
    battery_connected_binary_sensor_ = battery_connected_binary_sensor;
  }
  void set_pv2_support_binary_sensor(binary_sensor::BinarySensor *pv2_support_binary_sensor) {
    pv2_support_binary_sensor_ = pv2_support_binary_sensor;
  }

  void set_serial_number_text_sensor(text_sensor::TextSensor *serial_number_text_sensor) {
    serial_number_text_sensor_ = serial_number_text_sensor;
  }
  void set_operation_mode_text_sensor(text_sensor::TextSensor *operation_mode_text_sensor) {
    operation_mode_text_sensor_ = operation_mode_text_sensor;
  }
  void set_device_model_text_sensor(text_sensor::TextSensor *device_model_text_sensor) {
    device_model_text_sensor_ = device_model_text_sensor;
  }

  void set_factory_reset_button(button::Button *factory_reset_button) { factory_reset_button_ = factory_reset_button; }
  void set_restart_device_button(button::Button *restart_device_button) {
    restart_device_button_ = restart_device_button;
  }

  // Generic register bindings (holding registers, function 0x03)
  void add_number(number::Number *number_entity, uint16_t state_register, float factor) {
    this->number_bindings_.push_back({state_register, factor, number_entity});
  }
  void add_switch(switch_::Switch *switch_entity, uint16_t state_register) {
    this->switch_bindings_.push_back({state_register, switch_entity});
  }
  void add_register_sensor(sensor::Sensor *sensor_entity, uint16_t state_register, float factor, bool is_signed) {
    this->sensor_bindings_.push_back({state_register, factor, is_signed, sensor_entity});
    if (state_register >= BMS_REGISTER_START && state_register < BMS_REGISTER_START + BMS_REGISTER_COUNT)
      this->bms_requested_ = true;
  }

  void set_operation_mode_select(select::Select *operation_mode_select) {
    operation_mode_select_ = operation_mode_select;
  }

  void assemble(const uint8_t *data, uint16_t length);
  void set_last_request(RequestType request_type);
  void write_register(uint8_t register_address, uint16_t value);
  void write_multiple_registers(uint8_t start_register, const std::vector<uint16_t> &values);
  void send_command(const std::vector<uint8_t> &payload);
  void read_registers(uint8_t function, uint16_t start_register, uint16_t register_count);

 protected:
  binary_sensor::BinarySensor *grid_connected_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *battery_connected_binary_sensor_{nullptr};
  binary_sensor::BinarySensor *pv2_support_binary_sensor_{nullptr};

  sensor::Sensor *battery_voltage_sensor_{nullptr};
  sensor::Sensor *battery_current_sensor_{nullptr};
  sensor::Sensor *battery_power_sensor_{nullptr};
  sensor::Sensor *battery_soc_sensor_{nullptr};
  sensor::Sensor *ac_output_voltage_sensor_{nullptr};
  sensor::Sensor *ac_input_voltage_sensor_{nullptr};
  sensor::Sensor *ac_output_frequency_sensor_{nullptr};
  sensor::Sensor *ac_input_frequency_sensor_{nullptr};
  sensor::Sensor *ac_power_sensor_{nullptr};
  sensor::Sensor *pv_voltage_sensor_{nullptr};
  sensor::Sensor *pv_power_sensor_{nullptr};
  sensor::Sensor *grid_power_sensor_{nullptr};
  sensor::Sensor *load_power_sensor_{nullptr};
  sensor::Sensor *device_temperature_sensor_{nullptr};
  sensor::Sensor *pv2_voltage_sensor_{nullptr};
  sensor::Sensor *pv2_power_sensor_{nullptr};
  sensor::Sensor *device_type_image_sensor_{nullptr};
  sensor::Sensor *battery_status_sensor_{nullptr};
  sensor::Sensor *grid_connection_status_sensor_{nullptr};
  sensor::Sensor *device_type_sensor_{nullptr};
  sensor::Sensor *device_power_rating_code_sensor_{nullptr};
  sensor::Sensor *device_power_rating_sensor_{nullptr};
  sensor::Sensor *ac_output_apparent_power_sensor_{nullptr};
  sensor::Sensor *grid_ct_power_sensor_{nullptr};
  sensor::Sensor *today_pv_production_sensor_{nullptr};
  sensor::Sensor *today_essential_load_sensor_{nullptr};
  sensor::Sensor *today_total_consumption_sensor_{nullptr};
  sensor::Sensor *today_grid_consumption_sensor_{nullptr};
  sensor::Sensor *today_battery_charging_sensor_{nullptr};
  sensor::Sensor *today_battery_discharge_sensor_{nullptr};
  sensor::Sensor *grid_export_sensor_{nullptr};

  text_sensor::TextSensor *serial_number_text_sensor_{nullptr};
  text_sensor::TextSensor *operation_mode_text_sensor_{nullptr};
  text_sensor::TextSensor *device_model_text_sensor_{nullptr};

  button::Button *factory_reset_button_{nullptr};
  button::Button *restart_device_button_{nullptr};

  struct NumberBinding {
    uint16_t state_register;
    float factor;
    number::Number *entity;
  };
  struct SwitchBinding {
    uint16_t state_register;
    switch_::Switch *entity;
  };
  struct SensorBinding {
    uint16_t state_register;
    float factor;
    bool is_signed;
    sensor::Sensor *entity;
  };
  std::vector<NumberBinding> number_bindings_;
  std::vector<SwitchBinding> switch_bindings_;
  std::vector<SensorBinding> sensor_bindings_;
  bool bms_requested_{false};

  select::Select *operation_mode_select_{nullptr};

  uint16_t char_handle_{0};
  std::vector<uint8_t> frame_buffer_;
  uint32_t last_frame_timestamp_ = 0;

  RequestType current_request_type_ = REQUEST_SYSTEM_STATUS;

  void decode_(const std::vector<uint8_t> &data);
  void decode_system_status_registers_(const std::vector<uint8_t> &data);
  void decode_battery_config_registers_(const std::vector<uint8_t> &data);
  void decode_system_control_registers_(const std::vector<uint8_t> &data);
  void decode_daily_statistics_registers_(const std::vector<uint8_t> &data);
  void decode_bms_registers_(const std::vector<uint8_t> &data);
  void publish_register_bindings_(uint16_t register_address, uint16_t value);
  void advance_request_();
  bool handle_non_read_frame_(const uint8_t *data, uint16_t length);
  void publish_state_(binary_sensor::BinarySensor *binary_sensor, const bool &state);
  void publish_state_(sensor::Sensor *sensor, float value);
  void publish_state_(text_sensor::TextSensor *text_sensor, const std::string &state);
  void publish_state_(switch_::Switch *switch_entity, const bool &state);
  void publish_state_(select::Select *select_entity, const std::string &state);
  void send_next_request_();
  std::string operation_mode_to_string_(uint16_t mode);
  float power_rating_code_to_watts_(uint16_t code);
  std::string generate_device_model_(uint16_t device_type, uint16_t power_rating, bool light_engine);
};

}  // namespace esphome::lumentree_ble
