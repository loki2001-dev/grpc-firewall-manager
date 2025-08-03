#include "device_manager.h"
#include <spdlog/spdlog.h>

DeviceManager::DeviceManager() {
    spdlog::info("DeviceManager initialized");
}

void DeviceManager::RegisterDevice(std::shared_ptr<Device> device) {
    _devices[device->GetId()] = device;
    spdlog::info("Device registered: {} ({})", device->GetId(), device->GetName());
}

std::shared_ptr<Device> DeviceManager::GetDevice(const std::string& device_id) {
    spdlog::trace("Getting device: {}", device_id);
    auto it = _devices.find(device_id);
    if (it != _devices.end()) {
        spdlog::trace("Device found: {}", device_id);
        return it->second;
    } else {
        return nullptr;
    }
}

std::vector<std::shared_ptr<Device>> DeviceManager::ListDevices(DeviceType filter) {
    std::vector<std::shared_ptr<Device>> list;
    for (auto& [id, dev] : _devices) {
        if (filter == DeviceType::UNKNOWN || dev->GetType() == filter) {
            list.push_back(dev);
        }
    }
    return list;
}