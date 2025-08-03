#ifndef DEVICE_MANAGER_H
#define DEVICE_MANAGER_H

#pragma once

#include <memory>
#include <unordered_map>
#include <vector>
#include "base/device.h"

class DeviceManager {
public:
    DeviceManager();
    void RegisterDevice(std::shared_ptr<Device> device);
    std::shared_ptr<Device> GetDevice(const std::string& device_id);
    std::vector<std::shared_ptr<Device>> ListDevices(DeviceType filter = DeviceType::UNKNOWN);

private:
    std::unordered_map<std::string, std::shared_ptr<Device>> _devices;
};

#endif // DEVICE_MANAGER_H