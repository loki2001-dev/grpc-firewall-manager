#ifndef MOTION_SENSOR_H
#define MOTION_SENSOR_H

#pragma once

#include <map>
#include <string>
#include "../base/device.h"

class MotionSensor : public Device {
public:
    MotionSensor(const std::string& id, const std::string& name, const std::string& location);

    bool Control(const std::string& action, const std::map<std::string, std::string>& parameters, std::string& out_message) override;
    std::map<std::string, std::string> StatusProperties() const override;
};

#endif // MOTION_SENSOR_H