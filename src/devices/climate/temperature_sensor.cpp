#include "temperature_sensor.h"
#include <cstdlib>
#include <chrono>
#include <thread>

TemperatureSensor::TemperatureSensor(const std::string& id, std::string name, std::string location)
    : Device(id, DeviceType::TEMPERATURE_SENSOR, std::move(name), std::move(location)),
    _last_temp(22.0) {}

bool TemperatureSensor::Control(const std::string& action, const std::map<std::string,std::string>& parameters, std::string& out_message) {
    out_message = "TemperatureSensor control not supported";
    return false;
}

double TemperatureSensor::ReadTemperature() const {
    return _last_temp + (std::rand() % 1000) / 100.0;
}

std::map<std::string,std::string> TemperatureSensor::StatusProperties() const {
    return {{"temperature", std::to_string(ReadTemperature())}};
}