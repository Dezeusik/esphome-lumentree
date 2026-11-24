#include "esphome/components/lumentree_ble/lumentree_ble.h"
#include "esphome/components/sensor/sensor.h"
#include "esphome/components/text_sensor/text_sensor.h"
#include "esphome/components/lumentree_ble/number/lumentree_number.h"
#include "esphome/components/lumentree_ble/switch/lumentree_switch.h"
#include "common.h"
#include "frames.h"
#include <gtest/gtest.h>

namespace esphome::lumentree_ble::testing {

class LumentreeSystemStatusTest : public ::testing::Test {
 protected:
  TestableLumentreeBle bms_;
  sensor::Sensor battery_voltage_;
  sensor::Sensor battery_current_;
  sensor::Sensor ac_output_voltage_;
  sensor::Sensor ac_input_voltage_;
  sensor::Sensor ac_output_frequency_;
  sensor::Sensor ac_input_frequency_;
  sensor::Sensor ac_power_;
  sensor::Sensor pv_voltage_;
  sensor::Sensor pv_power_;
  sensor::Sensor device_temperature_;
  sensor::Sensor device_type_;
  sensor::Sensor device_power_rating_code_;
  sensor::Sensor device_power_rating_;
  text_sensor::TextSensor serial_number_;
  text_sensor::TextSensor device_model_;

  void SetUp() override {
    bms_.set_battery_voltage_sensor(&battery_voltage_);
    bms_.set_battery_current_sensor(&battery_current_);
    bms_.set_ac_output_voltage_sensor(&ac_output_voltage_);
    bms_.set_ac_input_voltage_sensor(&ac_input_voltage_);
    bms_.set_ac_output_frequency_sensor(&ac_output_frequency_);
    bms_.set_ac_input_frequency_sensor(&ac_input_frequency_);
    bms_.set_ac_power_sensor(&ac_power_);
    bms_.set_pv_voltage_sensor(&pv_voltage_);
    bms_.set_pv_power_sensor(&pv_power_);
    bms_.set_device_temperature_sensor(&device_temperature_);
    bms_.set_device_type_sensor(&device_type_);
    bms_.set_device_power_rating_code_sensor(&device_power_rating_code_);
    bms_.set_device_power_rating_sensor(&device_power_rating_);
    bms_.set_serial_number_text_sensor(&serial_number_);
    bms_.set_device_model_text_sensor(&device_model_);
    bms_.set_last_request(LumentreeBle::REQUEST_SYSTEM_STATUS);
    bms_.assemble(SYSTEM_STATUS_FRAME.data(), SYSTEM_STATUS_FRAME.size());
  }
};

TEST_F(LumentreeSystemStatusTest, BatteryMeasurements) {
  EXPECT_NEAR(battery_voltage_.state, 52.90f, 0.01f);
  EXPECT_NEAR(battery_current_.state, 8.30f, 0.01f);
}

TEST_F(LumentreeSystemStatusTest, AcMeasurements) {
  EXPECT_NEAR(ac_output_voltage_.state, 237.0f, 0.1f);
  EXPECT_NEAR(ac_input_voltage_.state, 235.0f, 0.1f);
  EXPECT_NEAR(ac_output_frequency_.state, 49.70f, 0.01f);
  EXPECT_NEAR(ac_input_frequency_.state, 49.70f, 0.01f);
  EXPECT_NEAR(ac_power_.state, 500.0f, 0.1f);
}

TEST_F(LumentreeSystemStatusTest, PvMeasurements) {
  EXPECT_NEAR(pv_voltage_.state, 177.0f, 0.1f);
  EXPECT_NEAR(pv_power_.state, 23.0f, 0.1f);
}

TEST_F(LumentreeSystemStatusTest, Temperature) { EXPECT_NEAR(device_temperature_.state, 33.6f, 0.1f); }

TEST_F(LumentreeSystemStatusTest, DeviceInfo) {
  EXPECT_NEAR(device_type_.state, 768.0f, 0.1f);
  EXPECT_NEAR(device_power_rating_code_.state, 5.0f, 0.1f);
  EXPECT_NEAR(device_power_rating_.state, 6000.0f, 0.1f);
}

TEST_F(LumentreeSystemStatusTest, SerialNumber) { EXPECT_EQ(serial_number_.state, "P241019070"); }

TEST_F(LumentreeSystemStatusTest, DeviceModel) { EXPECT_EQ(device_model_.state, "SUNT-6.0KW-P"); }

TEST(LumentreeSafetyTest, NullSensorsDoNotCrash) {
  TestableLumentreeBle bms;
  bms.set_last_request(LumentreeBle::REQUEST_SYSTEM_STATUS);
  bms.assemble(SYSTEM_STATUS_FRAME.data(), SYSTEM_STATUS_FRAME.size());
}

// Builds a valid read holding registers (0x03) response for registers start..start+count-1
static std::vector<uint8_t> make_read_frame(uint16_t start, uint16_t count,
                                            const std::vector<std::pair<uint16_t, uint16_t>> &values) {
  std::vector<uint8_t> frame = {0x01, 0x03, (uint8_t) (count * 2)};
  for (uint16_t reg = start; reg < start + count; reg++) {
    uint16_t value = 0;
    for (const auto &entry : values) {
      if (entry.first == reg)
        value = entry.second;
    }
    frame.push_back(value >> 8);
    frame.push_back(value & 0xFF);
  }
  uint16_t crc = crc16(frame.data(), frame.size(), 0xFFFF, 0xA001, false, false);
  frame.push_back(crc & 0xFF);
  frame.push_back(crc >> 8);
  return frame;
}

static void feed_chunked(LumentreeBle &device, const std::vector<uint8_t> &frame) {
  for (size_t i = 0; i < frame.size(); i += 20)
    device.assemble(frame.data() + i, std::min<size_t>(20, frame.size() - i));
}

class TestableNumber : public LumentreeNumber {
 public:
  void control(float value) override {}
};

class TestableSwitch : public LumentreeSwitch {
 public:
  void write_state(bool state) override {}
};

class TestableRequestState : public TestableLumentreeBle {
 public:
  RequestType request() const { return this->current_request_type_; }
};

TEST(LumentreeBindingsTest, NumbersPublishReadBackValues) {
  TestableLumentreeBle device;
  TestableNumber charging_target, capacity, low_voltage_cutoff;
  TestableSwitch ac_charging;
  device.add_number(&charging_target, 102, 0.01f);
  device.add_number(&capacity, 104, 1.0f);
  device.add_number(&low_voltage_cutoff, 114, 0.01f);
  device.add_switch(&ac_charging, 120);
  device.set_last_request(LumentreeBle::REQUEST_BATTERY_CONFIG);
  feed_chunked(device, make_read_frame(101, 57, {{102, 5640}, {104, 200}, {114, 4800}, {120, 1}}));

  EXPECT_NEAR(charging_target.state, 56.40f, 0.001f);
  EXPECT_NEAR(capacity.state, 200.0f, 0.001f);
  EXPECT_NEAR(low_voltage_cutoff.state, 48.00f, 0.001f);
  EXPECT_TRUE(ac_charging.state);
}

TEST(LumentreeBindingsTest, SystemControlSwitches) {
  TestableLumentreeBle device;
  TestableSwitch beep, sleep_mode, output;
  device.add_switch(&beep, 167);
  device.add_switch(&sleep_mode, 163);
  device.add_switch(&output, 170);
  device.set_last_request(LumentreeBle::REQUEST_SYSTEM_CONTROL);
  feed_chunked(device, make_read_frame(160, 21, {{167, 1}, {170, 1}}));

  EXPECT_TRUE(beep.state);
  EXPECT_FALSE(sleep_mode.state);
  EXPECT_TRUE(output.state);
}

TEST(LumentreeBmsTest, BmsBlockIsDecoded) {
  TestableRequestState device;
  sensor::Sensor voltage, current, temperature;
  device.add_register_sensor(&voltage, 191, 0.01f, false);
  device.add_register_sensor(&current, 192, 1.0f, true);
  device.add_register_sensor(&temperature, 193, 0.1f, true);
  device.set_last_request(LumentreeBle::REQUEST_BMS);
  feed_chunked(device, make_read_frame(185, 13, {{191, 5312}, {192, (uint16_t) -12}, {193, (uint16_t) -50}}));

  EXPECT_NEAR(voltage.state, 53.12f, 0.001f);
  EXPECT_NEAR(current.state, -12.0f, 0.001f);
  EXPECT_NEAR(temperature.state, -5.0f, 0.001f);
  EXPECT_EQ(device.request(), LumentreeBle::REQUEST_COMPLETE);
}

TEST(LumentreeBmsTest, ExceptionResponseDoesNotStallSequence) {
  TestableRequestState device;
  sensor::Sensor voltage;
  device.add_register_sensor(&voltage, 191, 0.01f, false);
  device.set_last_request(LumentreeBle::REQUEST_BMS);

  std::vector<uint8_t> exception = {0x01, 0x83, 0x02};
  uint16_t crc = crc16(exception.data(), exception.size(), 0xFFFF, 0xA001, false, false);
  exception.push_back(crc & 0xFF);
  exception.push_back(crc >> 8);
  device.assemble(exception.data(), exception.size());

  EXPECT_EQ(device.request(), LumentreeBle::REQUEST_COMPLETE);
}

TEST(LumentreeBmsTest, WriteAckDuringReadDoesNotCorruptFrame) {
  TestableLumentreeBle device;
  TestableNumber capacity;
  device.add_number(&capacity, 104, 1.0f);
  device.set_last_request(LumentreeBle::REQUEST_BATTERY_CONFIG);

  auto frame = make_read_frame(101, 57, {{104, 280}});
  std::vector<uint8_t> ack = {0x01, 0x06, 0x00, 0x68, 0x01, 0x18};
  uint16_t crc = crc16(ack.data(), ack.size(), 0xFFFF, 0xA001, false, false);
  ack.push_back(crc & 0xFF);
  ack.push_back(crc >> 8);

  device.assemble(frame.data(), 20);
  device.assemble(ack.data(), ack.size());
  for (size_t i = 20; i < frame.size(); i += 20)
    device.assemble(frame.data() + i, std::min<size_t>(20, frame.size() - i));

  EXPECT_NEAR(capacity.state, 280.0f, 0.001f);
}

}  // namespace esphome::lumentree_ble::testing
