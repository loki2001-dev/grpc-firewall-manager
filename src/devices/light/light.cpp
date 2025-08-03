#include "light.h"
#include <spdlog/spdlog.h>
#include <sstream>

Light::Light(const std::string& id, std::string name, std::string location)
    : Device(id, DeviceType::LIGHT, std::move(name),
    std::move(location)),
    _is_on(false) {
    spdlog::info("Light device created: {} ({}) at {}", id, name, location);
}

bool Light::Control(const std::string& action, const std::map<std::string,std::string>& parameters, std::string& out_message) {
    if (action == "turn_on") {
        _is_on = true;
        out_message = "Light turned on";
        spdlog::info("Light {} turned on", GetId());
        return true;
    } else if (action == "turn_off") {
        _is_on = false;
        out_message = "Light turned off";
        spdlog::info("Light {} turned off", GetId());
        return true;
    } else {
        out_message = "Unsupported action: " + action;
        spdlog::warn("Unsupported action for light {}: {}", GetId(), action);
        return false;
    }
}

std::map<std::string,std::string> Light::StatusProperties() const {
    spdlog::trace("Getting status properties for light: {}", GetId());
    return {{"is_on", _is_on ? "true" : "false"}};
}