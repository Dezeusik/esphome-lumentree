import esphome.codegen as cg
from esphome.components import number
import esphome.config_validation as cv
from esphome.const import (
    CONF_MAX_VALUE,
    CONF_MIN_VALUE,
    CONF_MODE,
    CONF_STEP,
    CONF_UNIT_OF_MEASUREMENT,
    ENTITY_CATEGORY_CONFIG,
    ICON_EMPTY,
    UNIT_AMPERE,
    UNIT_VOLT,
    UNIT_WATT,
)

from .. import CONF_LUMENTREE_BLE_ID, LUMENTREE_BLE_COMPONENT_SCHEMA, lumentree_ble_ns

DEPENDENCIES = ["lumentree_ble"]

CODEOWNERS = ["@syssi"]

try:
    from esphome.components.const import UNIT_AMPERE_HOUR
except ImportError:  # ESPHome < 2026.2.0
    UNIT_AMPERE_HOUR = "Ah"

DEFAULT_STEP = 1

CONF_POWER_OUTPUT_SETTING = "power_output_setting"
CONF_EQUALIZATION_VOLTAGE_SETTING = "equalization_voltage_setting"
CONF_CHARGING_TARGET_VOLTAGE_SETTING = "charging_target_voltage_setting"
CONF_FLOAT_CHARGE_VOLTAGE_SETTING = "float_charge_voltage_setting"
CONF_BATTERY_CAPACITY_SETTING = "battery_capacity_setting"
CONF_MAX_CHARGE_CURRENT_SETTING = "max_charge_current_setting"
CONF_MAX_DISCHARGE_CURRENT_SETTING = "max_discharge_current_setting"
CONF_LOW_VOLTAGE_CUTOFF_SETTING = "low_voltage_cutoff_setting"
CONF_BATTERY_RECOVERY_VOLTAGE_SETTING = "battery_recovery_voltage_setting"
CONF_EQUALIZATION_INTERVAL_SETTING = "equalization_interval_setting"
CONF_EQUALIZATION_DURATION_SETTING = "equalization_duration_setting"

UNIT_DAYS = "d"
UNIT_MINUTE = "min"

# key: [holding register, factor, length]
NUMBERS = {
    CONF_POWER_OUTPUT_SETTING: [40, 1.0, 2],
    CONF_EQUALIZATION_VOLTAGE_SETTING: [101, 0.01, 2],
    CONF_CHARGING_TARGET_VOLTAGE_SETTING: [102, 0.01, 2],
    CONF_FLOAT_CHARGE_VOLTAGE_SETTING: [103, 0.01, 2],
    CONF_BATTERY_CAPACITY_SETTING: [104, 1.0, 2],
    CONF_MAX_CHARGE_CURRENT_SETTING: [106, 1.0, 2],
    CONF_MAX_DISCHARGE_CURRENT_SETTING: [107, 1.0, 2],
    CONF_LOW_VOLTAGE_CUTOFF_SETTING: [114, 0.01, 2],
    CONF_BATTERY_RECOVERY_VOLTAGE_SETTING: [115, 0.01, 2],
    CONF_EQUALIZATION_INTERVAL_SETTING: [148, 1.0, 2],
    CONF_EQUALIZATION_DURATION_SETTING: [149, 1.0, 2],
}

LumentreeNumber = lumentree_ble_ns.class_(
    "LumentreeNumber", number.Number, cg.Component
)

LUMENTREE_NUMBER_SCHEMA = (
    number.number_schema(
        LumentreeNumber,
        icon=ICON_EMPTY,
        entity_category=ENTITY_CATEGORY_CONFIG,
        unit_of_measurement=UNIT_VOLT,
    )
    .extend(
        {
            cv.Optional(CONF_STEP, default=0.01): cv.float_,
            cv.Optional(CONF_MODE, default="BOX"): cv.enum(
                number.NUMBER_MODES, upper=True
            ),
        }
    )
    .extend(cv.COMPONENT_SCHEMA)
)

CONFIG_SCHEMA = LUMENTREE_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(CONF_POWER_OUTPUT_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=2000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_WATT
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_EQUALIZATION_VOLTAGE_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=48.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=60.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(
            CONF_CHARGING_TARGET_VOLTAGE_SETTING
        ): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=48.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=58.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(CONF_FLOAT_CHARGE_VOLTAGE_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=48.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=56.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(CONF_BATTERY_CAPACITY_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=50.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=1000.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE_HOUR
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_MAX_CHARGE_CURRENT_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=150.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_MAX_DISCHARGE_CURRENT_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=150.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_AMPERE
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_LOW_VOLTAGE_CUTOFF_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=40.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=54.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(
            CONF_BATTERY_RECOVERY_VOLTAGE_SETTING
        ): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=42.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=56.0): cv.float_,
                cv.Optional(CONF_STEP, default=0.01): cv.float_,
            }
        ),
        cv.Optional(CONF_EQUALIZATION_INTERVAL_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=90.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_DAYS
                ): cv.string_strict,
            }
        ),
        cv.Optional(CONF_EQUALIZATION_DURATION_SETTING): LUMENTREE_NUMBER_SCHEMA.extend(
            {
                cv.Optional(CONF_MIN_VALUE, default=0.0): cv.float_,
                cv.Optional(CONF_MAX_VALUE, default=900.0): cv.float_,
                cv.Optional(CONF_STEP, default=1.0): cv.float_,
                cv.Optional(
                    CONF_UNIT_OF_MEASUREMENT, default=UNIT_MINUTE
                ): cv.string_strict,
            }
        ),
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LUMENTREE_BLE_ID])
    for key, address in NUMBERS.items():
        if key in config:
            conf = config[key]
            var = await number.new_number(
                conf,
                min_value=conf[CONF_MIN_VALUE],
                max_value=conf[CONF_MAX_VALUE],
                step=conf[CONF_STEP],
            )
            await cg.register_component(var, conf)
            cg.add(hub.add_number(var, address[0], address[1]))
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address[0]))
            cg.add(var.set_factor(address[1]))
            cg.add(var.set_length(address[2]))
