import esphome.codegen as cg
from esphome.components import switch
import esphome.config_validation as cv
from esphome.const import CONF_OUTPUT

from .. import CONF_LUMENTREE_BLE_ID, LUMENTREE_BLE_COMPONENT_SCHEMA, lumentree_ble_ns

DEPENDENCIES = ["lumentree_ble"]

CODEOWNERS = ["@syssi"]

CONF_AC_CHARGING = "ac_charging"
# CONF_OUTPUT = "output"
CONF_BEEP = "beep"
CONF_SLEEP_MODE = "sleep_mode"
CONF_OVERLOAD_AUTO_RESTART = "overload_auto_restart"
CONF_OVERTEMPERATURE_AUTO_RESTART = "overtemperature_auto_restart"

ICON_AC_CHARGING = "mdi:battery-charging-outline"
ICON_OUTPUT = "mdi:power"
ICON_BEEP = "mdi:volume-high"
ICON_SLEEP_MODE = "mdi:power-sleep"
ICON_OVERLOAD_AUTO_RESTART = "mdi:restart-alert"
ICON_OVERTEMPERATURE_AUTO_RESTART = "mdi:thermometer-alert"

# key: [write register, state register]
SWITCHES = {
    CONF_AC_CHARGING: [120, 120],
    # Written to 121 but read back from 170 ("Online Mode") as before; unverified mapping
    CONF_OUTPUT: [121, 170],
    CONF_SLEEP_MODE: [163, 163],
    CONF_OVERLOAD_AUTO_RESTART: [165, 165],
    CONF_OVERTEMPERATURE_AUTO_RESTART: [166, 166],
    CONF_BEEP: [167, 167],
}

ICONS = {
    CONF_AC_CHARGING: ICON_AC_CHARGING,
    CONF_OUTPUT: ICON_OUTPUT,
    CONF_SLEEP_MODE: ICON_SLEEP_MODE,
    CONF_OVERLOAD_AUTO_RESTART: ICON_OVERLOAD_AUTO_RESTART,
    CONF_OVERTEMPERATURE_AUTO_RESTART: ICON_OVERTEMPERATURE_AUTO_RESTART,
    CONF_BEEP: ICON_BEEP,
}

LumentreeSwitch = lumentree_ble_ns.class_("LumentreeSwitch", switch.Switch)

CONFIG_SCHEMA = LUMENTREE_BLE_COMPONENT_SCHEMA.extend(
    {
        cv.Optional(key): switch.switch_schema(LumentreeSwitch, icon=icon)
        for key, icon in ICONS.items()
    }
)


async def to_code(config):
    hub = await cg.get_variable(config[CONF_LUMENTREE_BLE_ID])
    for key, address in SWITCHES.items():
        if key in config:
            conf = config[key]
            var = await switch.new_switch(conf)
            cg.add(hub.add_switch(var, address[1]))
            cg.add(var.set_parent(hub))
            cg.add(var.set_holding_register(address[0]))
