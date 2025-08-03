#include "door_lock.h"

DoorLock::DoorLock(const std::string& id, const std::string& name, const std::string& location)
    : Device(id, smarthome::DOOR_LOCK, name, location),
    _locked(true) {
}

bool DoorLock::Control(const std::string& action, const std::map<std::string, std::string>&, std::string& out_message) {
    if (action == "lock") {
        _locked = true;
        out_message = "Door locked";
        return true;
    } else if (action == "unlock") {
        _locked = false;
        out_message = "Door unlocked";
        return true;
    }
    out_message = "Unsupported action";
    return false;
}

std::map<std::string, std::string> DoorLock::StatusProperties() const {
    return { {"locked", _locked ? "true" : "false"} };
}