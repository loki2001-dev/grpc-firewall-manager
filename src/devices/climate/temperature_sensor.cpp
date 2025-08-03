#include "temperature_sensor.h"
#include <spdlog/spdlog.h>
#include <cstdlib>
#include <chrono>
#include <thread>

TemperatureSensor::TemperatureSensor(const std::string& id, std::string name, std::string location)
    : Device(id, DeviceType::TEMPERATURE_SENSOR, std::move(name), std::move(location)),
    _last_temp(22.0) {
    spdlog::info("Temperature sensor created: {} ({}) at {}", id, name, location);
}

bool TemperatureSensor::Control(const std::string& action, const std::map<std::string,std::string>& parameters, std::string& out_message) {
    out_message = "TemperatureSensor control not supported";
    return false;
}

double TemperatureSensor::ReadTemperature() const {
    double temp = _last_temp + (std::rand() % 1000) / 100.0;
    spdlog::trace("Temperature reading for {}: {:.2f}°C", GetId(), temp);
    return temp;
}

std::map<std::string,std::string> TemperatureSensor::StatusProperties() const {
    spdlog::trace("Getting status properties for temperature sensor: {}", GetId());
    return {{"temperature", std::to_string(ReadTemperature())}};
}