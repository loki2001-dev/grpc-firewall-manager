#ifndef LIGHT_H
#define LIGHT_H

#pragma once

#include <map>
#include <string>
#include "../base/device.h"

class Light : public Device {
public:
    Light(const std::string& id, std::string name, std::string location);

    bool Control(const std::string& action, const std::map<std::string,std::string>& parameters, std::string& out_message) override;
    std::map<std::string,std::string> StatusProperties() const override;

private:
    bool _is_on;
};

#endif // LIGHT_H