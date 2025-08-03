#ifndef DOOR_LOCK_H
#define DOOR_LOCK_H

#pragma once

#include <map>
#include <string>
#include "../base/device.h"

class DoorLock : public Device {
public:
    DoorLock(const std::string& id, const std::string& name, const std::string& location);

    bool Control(const std::string& action, const std::map<std::string, std::string>& parameters, std::string& out_message) override;
    std::map<std::string, std::string> StatusProperties() const override;

private:
    bool _locked;
};

#endif // DOOR_LOCK_H