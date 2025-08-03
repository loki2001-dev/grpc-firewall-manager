#include "thermostat.h"

Thermostat::Thermostat(const std::string& id, const std::string& name, const std::string& location)
    : Device(id, smarthome::THERMOSTAT, name, location),
    _temperature(22.0) {
}

bool Thermostat::Control(const std::string& action, const std::map<std::string, std::string>& parameters, std::string& out_message) {
    if (action == "set_temperature") {
        auto it = parameters.find("temperature");
        if (it != parameters.end()) {
            try {
                _temperature = std::stod(it->second);
                out_message = "Temperature set to " + std::to_string(_temperature);
                return true;
            } catch (...) {
                out_message = "Invalid temperature value";
                return false;
            }
        }
        out_message = "Missing 'temperature' parameter";
        return false;
    }
    out_message = "Unsupported action";
    return false;
}

std::map<std::string, std::string> Thermostat::StatusProperties() const {
    return { {"temperature", std::to_string(_temperature)} };
}