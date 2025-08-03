#include <grpcpp/grpcpp.h>
#include <iostream>
#include "client_commands.h"
#include "../build/smart_home.grpc.pb.h"

using smarthome::SmartHomeController;
using smarthome::DeviceControlRequest;
using smarthome::DeviceStatusRequest;
using smarthome::ListDevicesRequest;
using smarthome::StreamSensorRequest;

void ClientCommands::Run() {
    const auto channel = grpc::CreateChannel("localhost:50051", grpc::InsecureChannelCredentials());
    const auto stub = SmartHomeController::NewStub(channel);

    // DEVICE LIST
    ListDevicesRequest list_req;
    list_req.set_filter_type(smarthome::DeviceType::UNKNOWN);
    smarthome::ListDevicesResponse list_res;
    grpc::ClientContext ctx1;
    if (stub->ListDevices(&ctx1, list_req, &list_res).ok()) {
        std::cout << "Devices:\n";
        for (const auto& info : list_res.devices()) {
            std::cout << " - " << info.device_id() << " (" << info.name() << " @ " << info.location() << ")\n";
        }
    }

    // LIGHT
    DeviceControlRequest ctrl_req;
    ctrl_req.set_device_id("light01");
    ctrl_req.set_device_type(smarthome::DeviceType::LIGHT);
    ctrl_req.set_action("turn_on");
    (*ctrl_req.mutable_parameters())[""] = "";
    smarthome::DeviceControlResponse ctrl_res;
    grpc::ClientContext ctx2;
    if (stub->ControlDevice(&ctx2, ctrl_req, &ctrl_res).ok()) {
        std::cout << ctrl_res.message() << "\n";
    }

    // SENSOR
    StreamSensorRequest stream_req;
    stream_req.add_device_ids("temp01");
    stream_req.set_interval_seconds(2);
    grpc::ClientContext ctx3;
    auto stream = stub->StreamSensorData(&ctx3, stream_req);
    smarthome::SensorDataResponse sensor_res;
    while (stream->Read(&sensor_res)) {
        std::cout << "Sensor " << sensor_res.device_id() << ": ";
        for (auto& kv : sensor_res.sensor_values()) {
            std::cout << kv.first << "=" << kv.second << " ";
        }
        std::cout << "\n";
    }
}