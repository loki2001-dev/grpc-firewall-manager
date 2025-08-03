#include "light.h"
#include <sstream>

Light::Light(const std::string& id, std::string name, std::string location)
    : Device(id, DeviceType::LIGHT, std::move(name),
    std::move(location)),
    _is_on(false) {}

bool Light::Control(const std::string& action, const std::map<std::string,std::string>& parameters, std::string& out_message) {
    if (action == "turn_on") {
        _is_on = true;
        out_message = "Light turned on";
        return true;
    } else if (action == "turn_off") {
        _is_on = false;
        out_message = "Light turned off";
        return true;
    } else {
        out_message = "Unsupported action: " + action;
        return false;
    }
}

std::map<std::string,std::string> Light::StatusProperties() const {
    return {{"is_on", _is_on ? "true" : "false"}};
}