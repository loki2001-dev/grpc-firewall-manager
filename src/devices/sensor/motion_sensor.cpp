#include "motion_sensor.h"
#include <cstdlib>

MotionSensor::MotionSensor(const std::string& id, const std::string& name, const std::string& location)
    : Device(id, smarthome::MOTION_SENSOR, name, location) {
}

bool MotionSensor::Control(const std::string&, const std::map<std::string, std::string>&, std::string& out_message) {
    out_message = "Motion sensor cannot be controlled";
    return false;
}

std::map<std::string, std::string> MotionSensor::StatusProperties() const {
    bool detected = (std::rand() % 2 == 0); // pseudo-random
    return { {"motion_detected", detected ? "true" : "false"} };
}