#include <grpcpp/grpcpp.h>
#include <spdlog/spdlog.h>
#include <spdlog/fmt/fmt.h>
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
        spdlog::info("Devices:");
        for (const auto& info : list_res.devices()) {
            spdlog::info(" - {} ({} @ {})", info.device_id(), info.name(), info.location());
        }
    } else {
        spdlog::error("Failed to list devices");
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
        spdlog::info("Device control response: {}", ctrl_res.message());
    } else {
        spdlog::error("Failed to control device: light01");
    }

    // THERMOSTAT
    DeviceControlRequest thermo_req;
    thermo_req.set_device_id("thermo01");
    thermo_req.set_device_type(smarthome::DeviceType::THERMOSTAT);
    thermo_req.set_action("set_temperature");
    (*thermo_req.mutable_parameters())["temperature"] = "22";
    smarthome::DeviceControlResponse thermo_res;
    grpc::ClientContext ctx3;

    if (stub->ControlDevice(&ctx3, thermo_req, &thermo_res).ok()) {
        spdlog::info("Thermostat response: {}", thermo_res.message());
    } else {
        spdlog::error("Failed to control thermostat: thermo01");
    }

    // DOOR LOCK
    DeviceControlRequest door_req;
    door_req.set_device_id("door01");
    door_req.set_device_type(smarthome::DeviceType::DOOR_LOCK);
    door_req.set_action("lock");
    (*door_req.mutable_parameters())[""] = "";
    smarthome::DeviceControlResponse door_res;
    grpc::ClientContext ctx4;

    if (stub->ControlDevice(&ctx4, door_req, &door_res).ok()) {
        spdlog::info("Door lock response: {}", door_res.message());
    } else {
        spdlog::error("Failed to control door lock: door01");
    }

    // MOTION SENSOR
    DeviceStatusRequest motion_req;
    motion_req.set_device_id("motion01");
    smarthome::DeviceStatusResponse motion_res;
    grpc::ClientContext ctx5;

    if (stub->GetDeviceStatus(&ctx5, motion_req, &motion_res).ok()) {
        spdlog::info("Motion sensor status [{}]:", motion_res.device_id());
        for (const auto& kv : motion_res.properties()) {
            spdlog::info("  {} = {}", kv.first, kv.second);
        }
    } else {
        spdlog::error("Failed to get motion sensor status: motion01");
    }

    // SENSOR STREAM
    StreamSensorRequest stream_req;
    stream_req.add_device_ids("temp01");
    stream_req.set_interval_seconds(2);
    grpc::ClientContext ctx6;
    auto stream = stub->StreamSensorData(&ctx6, stream_req);
    smarthome::SensorDataResponse sensor_res;

    spdlog::info("Starting sensor data stream...");
    while (stream->Read(&sensor_res)) {
        std::string sensor_data = fmt::format("Sensor {}: ", sensor_res.device_id());
        for (const auto& kv : sensor_res.sensor_values()) {
            sensor_data += fmt::format("{}={} ", kv.first, kv.second);
        }
        spdlog::info(sensor_data);
    }
    spdlog::info("Sensor data stream ended");
}