"""Schema structure tests for lumentree_ble ESPHome component modules."""

from pathlib import Path
import sys

sys.path.insert(0, str(Path(__file__).parent.parent))

import components.lumentree_ble as hub  # noqa: E402
from components.lumentree_ble import (  # noqa: E402
    binary_sensor,
    button,  # noqa: E402
    number,  # noqa: E402
    select as select_mod,  # noqa: E402
    sensor,
    switch,  # noqa: E402
    text_sensor,
)


class TestHubConstants:
    def test_conf_id_defined(self):
        assert hub.CONF_LUMENTREE_BLE_ID == "lumentree_ble_id"


class TestSensorDefs:
    def test_sensor_defs_completeness(self):
        assert "battery_voltage" in sensor.SENSOR_DEFS
        assert "battery_current" in sensor.SENSOR_DEFS
        assert "battery_power" in sensor.SENSOR_DEFS
        assert "battery_soc" in sensor.SENSOR_DEFS
        assert "pv_voltage" in sensor.SENSOR_DEFS
        assert "pv_power" in sensor.SENSOR_DEFS
        assert "grid_power" in sensor.SENSOR_DEFS
        assert "load_power" in sensor.SENSOR_DEFS
        assert "device_temperature" in sensor.SENSOR_DEFS
        assert "today_pv_production" in sensor.SENSOR_DEFS
        assert len(sensor.SENSOR_DEFS) == 31

    def test_sensor_keys_are_strings(self):
        for key in sensor.SENSOR_DEFS:
            assert isinstance(key, str)

    def test_sensor_defs_values_are_dicts(self):
        for key, kwargs in sensor.SENSOR_DEFS.items():
            assert isinstance(kwargs, dict), f"{key} value must be a kwargs dict"


class TestRegisterSensorDefs:
    def test_register_sensor_defs(self):
        assert "bms_voltage" in sensor.REGISTER_SENSOR_DEFS
        assert "bms_temperature" in sensor.REGISTER_SENSOR_DEFS
        assert "cell_voltage_16" in sensor.REGISTER_SENSOR_DEFS
        assert len(sensor.REGISTER_SENSOR_DEFS) == 40

    def test_register_sensor_tuple_structure(self):
        for key, (
            register,
            factor,
            signed,
            kwargs,
        ) in sensor.REGISTER_SENSOR_DEFS.items():
            assert isinstance(register, int), key
            assert factor > 0, key
            assert isinstance(signed, bool), key
            assert isinstance(kwargs, dict), key

    def test_no_key_collision_with_sensor_defs(self):
        assert not set(sensor.REGISTER_SENSOR_DEFS) & set(sensor.SENSOR_DEFS)
        assert not set(sensor.CELL_SUMMARY_DEFS) & set(sensor.SENSOR_DEFS)
        assert not set(sensor.CELL_SUMMARY_DEFS) & set(sensor.REGISTER_SENSOR_DEFS)

    def test_register_sensor_registers_are_unique(self):
        registers = [defs[0] for defs in sensor.REGISTER_SENSOR_DEFS.values()]
        assert len(registers) == len(set(registers))


class TestBinarySensorDefs:
    def test_binary_sensor_defs_completeness(self):
        assert binary_sensor.CONF_GRID_CONNECTED in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_BATTERY_CONNECTED in binary_sensor.BINARY_SENSOR_DEFS
        assert binary_sensor.CONF_PV2_SUPPORT in binary_sensor.BINARY_SENSOR_DEFS
        assert len(binary_sensor.BINARY_SENSOR_DEFS) == 3

    def test_binary_sensor_defs_values_are_dicts(self):
        for key, kwargs in binary_sensor.BINARY_SENSOR_DEFS.items():
            assert isinstance(kwargs, dict), f"{key} value must be a kwargs dict"


class TestTextSensorConstants:
    def test_text_sensors_dict(self):
        assert text_sensor.CONF_SERIAL_NUMBER in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_OPERATION_MODE in text_sensor.TEXT_SENSORS
        assert text_sensor.CONF_DEVICE_MODEL in text_sensor.TEXT_SENSORS
        assert len(text_sensor.TEXT_SENSORS) == 3


class TestButtonConstants:
    def test_buttons_dict(self):
        assert button.CONF_BATTERY_SETTINGS_RESET in button.BUTTONS
        assert len(button.BUTTONS) == 1

    def test_button_addresses_are_unique(self):
        addresses = list(button.BUTTONS.values())
        assert len(addresses) == len(set(addresses))


class TestNumberConstants:
    def test_numbers_dict(self):
        assert number.CONF_POWER_OUTPUT_SETTING in number.NUMBERS
        assert number.CONF_EQUALIZATION_VOLTAGE_SETTING in number.NUMBERS
        assert number.CONF_CHARGING_TARGET_VOLTAGE_SETTING in number.NUMBERS
        assert number.CONF_FLOAT_CHARGE_VOLTAGE_SETTING in number.NUMBERS
        assert number.CONF_BATTERY_CAPACITY_SETTING in number.NUMBERS
        assert number.CONF_LOW_VOLTAGE_CUTOFF_SETTING in number.NUMBERS
        assert number.CONF_MAX_DISCHARGE_CURRENT_SETTING in number.NUMBERS
        assert number.CONF_LOW_CAPACITY_CUTOFF_SETTING in number.NUMBERS
        assert len(number.NUMBERS) == 14

    def test_number_registers_are_unique(self):
        registers = [val[0] for val in number.NUMBERS.values()]
        assert len(registers) == len(set(registers))

    def test_number_tuple_structure(self):
        for val in number.NUMBERS.values():
            register, factor, length = val
            assert isinstance(register, int)
            assert factor > 0
            assert isinstance(length, int)


class TestSwitchConstants:
    def test_switches_dict(self):
        assert switch.CONF_AC_CHARGING in switch.SWITCHES
        assert switch.CONF_BEEP in switch.SWITCHES
        assert switch.CONF_AC_COUPLING in switch.SWITCHES
        assert len(switch.SWITCHES) == 7

    def test_switch_addresses_are_unique(self):
        write_registers = [val[0] for val in switch.SWITCHES.values()]
        state_registers = [val[1] for val in switch.SWITCHES.values()]
        assert len(write_registers) == len(set(write_registers))
        assert len(state_registers) == len(set(state_registers))

    def test_every_switch_has_an_icon(self):
        assert set(switch.ICONS) == set(switch.SWITCHES)


class TestSelectConstants:
    def test_operation_mode_options(self):
        assert "Battery Mode" in select_mod.OPERATION_MODE_OPTIONS
        assert "Hybrid Mode" in select_mod.OPERATION_MODE_OPTIONS
        assert "Grid-Tie Mode" in select_mod.OPERATION_MODE_OPTIONS
        assert len(select_mod.OPERATION_MODE_OPTIONS) == 3

    def test_operation_mode_values_are_unique(self):
        values = list(select_mod.OPERATION_MODE_OPTIONS.values())
        assert len(values) == len(set(values))
