#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>
#include <algorithm>

std::mutex cpu_mutex;
std::atomic<bool> stop_flag(false);

struct SimulatedProcess {
    int pid;
    int burstTime;
    int remainingTime;
    int priority;  // Lower is higher priority
    std::string state = "READY";

    SimulatedProcess(int id, int burst, int prio)
        : pid(id), burstTime(burst), remainingTime(burst), priority(prio) {}
};

void runProcess(SimulatedProcess& proc) {
    while (proc.remainingTime > 0 && !stop_flag) {
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // avoid spin-lock

        if (cpu_mutex.try_lock()) {
            proc.state = "RUNNING";
            std::cout << "[CPU] PID " << proc.pid << " is RUNNING (Priority: " << proc.priority << "). Time left: " << proc.remainingTime << "\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(400));
            proc.remainingTime--;

            proc.state = (proc.remainingTime > 0) ? "READY" : "TERMINATED";
            std::cout << "[CPU] PID " << proc.pid << " is now " << proc.state << "\n";
            cpu_mutex.unlock();
        }
    }
}

int main() {
    std::vector<SimulatedProcess> processes = {
        SimulatedProcess(1, 3, 2),
        SimulatedProcess(2, 4, 1),
        SimulatedProcess(3, 2, 3)
    };

    std::sort(processes.begin(), processes.end(), [](auto& a, auto& b) {
        return a.priority < b.priority;
    });

    std::vector<std::thread> threads;
    for (auto& proc : processes) {
        threads.emplace_back(runProcess, std::ref(proc));
    }

    for (auto& t : threads) {
        if (t.joinable()) t.join();
    }

    std::cout << "Priority scheduling simulation complete.\n";
    return 0;
}
