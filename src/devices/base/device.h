#ifndef DEVICE_H
#define DEVICE_H

#pragma once

#include <string>
#include <map>
#include "device_types.h"

class Device {
public:
    Device(std::string id, smarthome::DeviceType type, std::string name, std::string location)
    : _id(std::move(id)),
    _type(type),
    _name(std::move(name)),
    _location(std::move(location)),
    _status(smarthome::ONLINE) {}

    virtual ~Device() = default;

    const std::string& GetId() const { return _id; }
    smarthome::DeviceType GetType() const { return _type; }
    const std::string& GetName() const { return _name; }
    const std::string& GetLocation() const { return _location; }
    smarthome::DeviceStatus GetStatus() const { return _status; }
    void SetStatus(smarthome::DeviceStatus s) { _status = s; }

    virtual bool Control(const std::string& action, const std::map<std::string,std::string>& parameters, std::string& out_message) = 0;
    virtual std::map<std::string,std::string> StatusProperties() const = 0;

protected:
    std::string _id;
    smarthome::DeviceType _type;
    std::string _name;
    std::string _location;
    smarthome::DeviceStatus _status;
};

#endif // DEVICE_H