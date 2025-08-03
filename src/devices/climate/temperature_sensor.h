#ifndef TEMPERATURE_SENSOR_H
#define TEMPERATURE_SENSOR_H

#pragma once

#include <string>
#include <map>
#include "../base/device.h"

class TemperatureSensor : public Device {
public:
    TemperatureSensor(const std::string& id, std::string name, std::string location);

    bool Control(const std::string& action, const std::map<std::string,std::string>& parameters, std::string& out_message) override;
    std::map<std::string,std::string> StatusProperties() const override;
    double ReadTemperature() const;

private:
    double _last_temp;
};

#endif // TEMPERATURE_SENSOR_H
