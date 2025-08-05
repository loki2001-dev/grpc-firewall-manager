# smarthome-grpc-controller (C++17 & gRPC)
- A Modular and Extensible Smart Home Device Management System using gRPC and Protocol Buffers.

---

## Overview
- Implements a gRPC-based smart home controller system
- Supports communication between a central controller and various smart devices such as lights, thermostats, door lock, and sensors
- Modular architecture separating interface definitions, business logic, and transport layers
- Designed for flexible integration of new devices

---

## Features
- Modular smart device abstraction using C++17 classes
- gRPC service for remote control and monitoring
- Protobuf-based schema definition for device commands and status
- Easy-to-extend structure for additional device types
- Sample implementation for lighting, climate, sensor, door lock devices
- Includes server and client

---

## gRPC API Routing

### Service: `SmartHomeController`

| RPC Method         | Request Message        | Response Message            | Description                                                 |
|--------------------|------------------------|-----------------------------|-------------------------------------------------------------|
| `ControlDevice`    | `DeviceControlRequest` | `DeviceControlResponse`     | Control a smart home device (e.g., turn on, set brightness) |
| `GetDeviceStatus`  | `DeviceStatusRequest`  | `DeviceStatusResponse`      | Query current status of a specific device                   |
| `ListDevices`      | `ListDevicesRequest`   | `ListDevicesResponse`       | Retrieve a list of all registered smart devices             |
| `StreamSensorData` | `StreamSensorRequest`  | `stream SensorDataResponse` | Stream real-time data from specified sensor devices         |

### Messages

#### DeviceControlRequest

| Field         | Type                 | Description                                         |
|---------------|----------------------|-----------------------------------------------------|
| `device_id`   | `string`             | Unique ID of the device                             |
| `device_type` | `DeviceType`         | Type of device                                      |
| `action`      | `string`             | Action to perform (e.g., "turn_on", "set_temp")     |
| `parameters`  | `map<string,string>` | Optional parameters (e.g., brightness, temperature) |

#### DeviceControlResponse

| Field       | Type     | Description                        |
|-------------|----------|------------------------------------|
| `success`   | `bool`   | Whether the command was successful |
| `message`   | `string` | Message describing result          |
| `device_id` | `string` | Affected device ID                 |

#### DeviceStatusRequest

| Field       | Type     | Description           |
|-------------|----------|-----------------------|
| `device_id` | `string` | ID of device to query |

#### DeviceStatusResponse

| Field          | Type                 | Description                                 |
|----------------|----------------------|---------------------------------------------|
| `device_id`    | `string`             | Device ID                                   |
| `device_type`  | `DeviceType`         | Type of device                              |
| `status`       | `DeviceStatus`       | Current status                              |
| `name`         | `string`             | Device name                                 |
| `location`     | `string`             | Device location                             |
| `properties`   | `map<string,string>` | Current properties (brightness, temp, etc.) |
| `last_updated` | `int64`              | Last updated timestamp                      |

#### ListDevicesRequest

| Field         | Type         | Description                              |
|---------------|--------------|------------------------------------------|
| `filter_type` | `DeviceType` | Optional filter for specific device type |

#### ListDevicesResponse

| Field     | Type                  | Description                |
|-----------|-----------------------|----------------------------|
| `devices` | `repeated DeviceInfo` | List of registered devices |

#### DeviceInfo

| Field         | Type           | Description        |
|---------------|----------------|--------------------|
| `device_id`   | `string`       | ID of the device   |
| `device_type` | `DeviceType`   | Type of the device |
| `status`      | `DeviceStatus` | Current status     |
| `name`        | `string`       | Device name        |
| `location`    | `string`       | Device location    |

#### StreamSensorRequest

| Field              | Type              | Description                          |
|--------------------|-------------------|--------------------------------------|
| `device_ids`       | `repeated string` | List of sensor device IDs to monitor |
| `interval_seconds` | `int32`           | Reporting interval in seconds        |

#### SensorDataResponse

| Field           | Type                 | Description                                  |
|-----------------|----------------------|----------------------------------------------|
| `device_id`     | `string`             | Sensor device ID                             |
| `sensor_type`   | `DeviceType`         | Type of sensor                               |
| `sensor_values` | `map<string,double>` | Sensor values (e.g., temp=22.5, humidity=60) |
| `timestamp`     | `int64`              | Timestamp of reading                         |

---

## Directory Structure
```
smarthome-grpc-controller/
├── proto/                     # gRPC service and message definitions (smart_home.proto)
├── src/
│   ├── client/                # CLI client implementation
│   ├── server/                # gRPC server and service logic
│   └── devices/
│       ├── base/              # Abstract interfaces and shared types
│       ├── light/             # Light device implementations
│       └── climate/           # Climate (thermostat/sensor) devices
│       └── security/          # Security device implementations
│       └── sensor/            # Sensor device implementations
├── CMakeLists.txt             # CMake build script
├── README.md
```

---

## Getting Started
### Prerequisites
- Linux (Ubuntu 20.04 or later recommended)
- Requires CMake 3.14 or later
- Requires C++17 compatible compiler
- [gRPC](https://github.com/grpc/grpc) (included as a submodule)
- [protobuf](https://github.com/protocolbuffers/protobuf) (included as a submodule)
- [spdlog](https://github.com/gabime/spdlog) (included as a submodule)
- [BloomRPC](https://github.com/bloomrpc/bloomrpc/releases) (useful for testing .proto files included as a project)

### Build Instructions
```bash
# Update package lists
sudo apt update

# Clone the repository
git clone https://github.com/loki2001-dev/smarthome-grpc-controller.git
cd smarthome-grpc-controller

# Initialize submodules
git submodule update --init --recursive

# Install dependencies and build Protobuf/gRPC
. install_grpc.sh

# Build the project
. build_project.sh
```

---

### Test Image
![test image1](./image/smarthome-grpc-controller.png)