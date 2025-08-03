#ifndef SERVICE_IMPL_H
#define SERVICE_IMPL_H

#pragma once

#include <grpcpp/grpcpp.h>
#include "smart_home.grpc.pb.h"
#include "../devices/device_manager.h"

class SmartHomeControllerServiceImpl final : public smarthome::SmartHomeController::Service {
public:
    SmartHomeControllerServiceImpl();

    grpc::Status ControlDevice(grpc::ServerContext*, const smarthome::DeviceControlRequest*, smarthome::DeviceControlResponse*) override;
    grpc::Status GetDeviceStatus(grpc::ServerContext*, const smarthome::DeviceStatusRequest*, smarthome::DeviceStatusResponse*) override;
    grpc::Status ListDevices(grpc::ServerContext*, const smarthome::ListDevicesRequest*, smarthome::ListDevicesResponse*) override;
    grpc::Status StreamSensorData(grpc::ServerContext*, const smarthome::StreamSensorRequest*, grpc::ServerWriter<smarthome::SensorDataResponse>*) override;

private:
    DeviceManager _manager;
};

#endif // SERVICE_IMPL_H