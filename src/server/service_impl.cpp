#include "service_impl.h"
#include "../devices/light/light.h"
#include "../devices/climate/temperature_sensor.h"
#include <thread>
#include <chrono>

SmartHomeControllerServiceImpl::SmartHomeControllerServiceImpl() {
    _manager.RegisterDevice(std::make_shared<Light>("light01", "Living Room Light", "Living Room"));
    _manager.RegisterDevice(std::make_shared<TemperatureSensor>("temp01", "Hall Sensor", "Hallway"));
}

grpc::Status SmartHomeControllerServiceImpl::ControlDevice(grpc::ServerContext*, const smarthome::DeviceControlRequest* req, smarthome::DeviceControlResponse* res) {
    auto dev = _manager.GetDevice(req->device_id());
    if (!dev) {
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
    return grpc::Status::OK;
}

grpc::Status SmartHomeControllerServiceImpl::GetDeviceStatus(grpc::ServerContext*, const smarthome::DeviceStatusRequest* req, smarthome::DeviceStatusResponse* res) {
    auto dev = _manager.GetDevice(req->device_id());
    if (!dev) {
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
    return grpc::Status::OK;
}

grpc::Status SmartHomeControllerServiceImpl::StreamSensorData(grpc::ServerContext* context, const smarthome::StreamSensorRequest* req, grpc::ServerWriter<smarthome::SensorDataResponse>* writer) {
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
                        (*resp.mutable_sensor_values())[kv.first] = 0.0;
                    }
                }

                resp.set_timestamp(std::chrono::system_clock::now().time_since_epoch().count());

                if (!writer->Write(resp)) {
                    break;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::seconds(req->interval_seconds()));
    }
    return grpc::Status::OK;
}