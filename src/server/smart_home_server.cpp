#include <atomic>
#include <array>
#include <chrono>
#include <csignal>
#include <memory>
#include <thread>
#include <string>
#include <grpcpp/grpcpp.h>
#include "service_impl.h"

std::atomic<bool> running{true};

void signal_handler(int) {
    running = false;
}

const std::array<int, 6> handled_signals = {
        SIGINT, SIGTERM, SIGTSTP, SIGHUP, SIGQUIT, SIGUSR1
};

void initialize() {
    // signal
    for (int sig : handled_signals) {
        std::signal(sig, signal_handler);
    }
}

int32_t main(int32_t argc, char *argv[]) {
    initialize();

    std::string address = "0.0.0.0:50051";
    SmartHomeControllerServiceImpl service;

    grpc::ServerBuilder builder;
    builder.AddListeningPort(address, grpc::InsecureServerCredentials());
    builder.RegisterService(&service);

    auto server = builder.BuildAndStart();
    std::cout << "SmartHomeController server listening on " << address << std::endl;
    server->Wait();

    while (running) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
    }

    return EXIT_SUCCESS;
}