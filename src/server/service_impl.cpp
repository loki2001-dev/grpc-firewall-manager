#include "service_impl.h"
#include "../devices/light/light.h"
#include "../devices/climate/thermostat.h"
#include "../devices/climate/temperature_sensor.h"
#include "../devices/security/door_lock.h"
#include "../devices/sensor/motion_sensor.h"
#include <spdlog/spdlog.h>
#include <thread>
#include <chrono>

SmartHomeControllerServiceImpl::SmartHomeControllerServiceImpl() {
    spdlog::info("Initializing SmartHomeController service");

    // LIGHT
    _manager.RegisterDevice(std::make_shared<Light>("light01", "Living Room Light", "Living Room"));
    spdlog::info("Registered device: light01 (Living Room Light)");

    // SENSOR
    _manager.RegisterDevice(std::make_shared<TemperatureSensor>("temp01", "Hall Sensor", "Hallway"));
    spdlog::info("Registered device: temp01 (Hall Sensor)");

    // THERMOSTAT
    _manager.RegisterDevice(std::make_shared<Thermostat>("thermo01", "Main Thermostat", "Living Room"));
    spdlog::info("Registered device: thermo01 (Main Thermostat)");

    // DOOR LOCK
    _manager.RegisterDevice(std::make_shared<DoorLock>("door01", "Front Door", "Entrance"));
    spdlog::info("Registered device: door01 (Front Door)");

    // MOTION SENSOR
    _manager.RegisterDevice(std::make_shared<MotionSensor>("motion01", "Garage Motion Sensor", "Garage"));
    spdlog::info("Registered device: motion01 (Garage Motion Sensor)");
}

grpc::Status SmartHomeControllerServiceImpl::ControlDevice(grpc::ServerContext*, const smarthome::DeviceControlRequest* req, smarthome::DeviceControlResponse* res) {
    auto dev = _manager.GetDevice(req->device_id());
    if (!dev) {
        spdlog::warn("Device not found: {}", req->device_id());
        res->set_success(false);
        res->set_message("Device not found");
        res->set_device_id(req->device_id());
        return grpc::Status::OK;
    }
    std::string message;
    bool ok = dev->Control(req->action(), std::map<std::string,std::string>(req->parameters().begin(), req->parameters().end()), message);
    res->set_success(ok);
    res->set_message(message);
    res->set_device_id(req->device_id());
    if (ok) {
        spdlog::info("Device control success: {} -> {}", req->device_id(), req->action());
    } else {
        spdlog::error("Device control failed: {} -> {}", req->device_id(), message);
    }
    return grpc::Status::OK;
}

grpc::Status SmartHomeControllerServiceImpl::GetDeviceStatus(grpc::ServerContext*, const smarthome::DeviceStatusRequest* req, smarthome::DeviceStatusResponse* res) {
    auto dev = _manager.GetDevice(req->device_id());
    if (!dev) {
        spdlog::warn("Device not found for status request: {}", req->device_id());
        return grpc::Status(grpc::StatusCode::NOT_FOUND, "Device not found");
    }
    res->set_device_id(dev->GetId());
    res->set_device_type(dev->GetType());
    res->set_status(dev->GetStatus());
    res->set_name(dev->GetName());
    res->set_location(dev->GetLocation());
    for (auto& kv : dev->StatusProperties()) {
        (*res->mutable_properties())[kv.first] = kv.second;
    }
    res->set_last_updated(std::chrono::system_clock::now().time_since_epoch().count());
    return grpc::Status::OK;
}

grpc::Status SmartHomeControllerServiceImpl::ListDevices(grpc::ServerContext*, const smarthome::ListDevicesRequest* req, smarthome::ListDevicesResponse* res) {
    auto list = _manager.ListDevices(req->filter_type());
    for (auto& dev : list) {
        auto* info = res->add_devices();
        info->set_device_id(dev->GetId());
        info->set_device_type(dev->GetType());
        info->set_status(dev->GetStatus());
        info->set_name(dev->GetName());
        info->set_location(dev->GetLocation());
    }
    spdlog::info("Listed {} devices", list.size());
    return grpc::Status::OK;
}

grpc::Status SmartHomeControllerServiceImpl::StreamSensorData(grpc::ServerContext* context, const smarthome::StreamSensorRequest* req, grpc::ServerWriter<smarthome::SensorDataResponse>* writer) {
    spdlog::info("Starting sensor data stream for {} devices, interval={}s", req->device_ids_size(), req->interval_seconds());
    while (!context->IsCancelled()) {
        for (const auto& did : req->device_ids()) {
            auto dev = _manager.GetDevice(did);
            if (dev && dev->GetType() == smarthome::TEMPERATURE_SENSOR) {
                smarthome::SensorDataResponse resp;
                resp.set_device_id(did);
                resp.set_sensor_type(dev->GetType());
                for (auto& kv : dev->StatusProperties()) {
                    try {
                        (*resp.mutable_sensor_values())[kv.first] = std::stod(kv.second);
                    } catch (const std::exception& e) {
                        spdlog::warn("Failed to parse sensor value: {}={}", kv.first, kv.second);
                        (*resp.mutable_sensor_values())[kv.first] = 0.0;
                    }
                }
                resp.set_timestamp(std::chrono::system_clock::now().time_since_epoch().count());
                if (!writer->Write(resp)) {
                    spdlog::warn("Failed to write sensor data for device: {}", did);
                    break;
                }
                spdlog::trace("Sent sensor data: {}", did);
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(req->interval_seconds()));
    }
    spdlog::info("Sensor data stream ended");
    return grpc::Status::OK;
}