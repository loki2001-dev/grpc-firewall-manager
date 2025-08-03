#include "device_manager.h"

DeviceManager::DeviceManager() {

}

void DeviceManager::RegisterDevice(std::shared_ptr<Device> device) {
    _devices[device->GetId()] = device;
}

std::shared_ptr<Device> DeviceManager::GetDevice(const std::string& device_id) {
    auto it = _devices.find(device_id);
    return (it != _devices.end()) ? it->second : nullptr;
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