#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <chrono>
#include <atomic>

std::mutex cpu_mutex;

std::atomic<bool> stop_flag(false);

// Simulated Process Structure
struct SimulatedProcess
{
    int pid;
    int burstTime;
    int remainingTime;
    std::string state = "READY";

    SimulatedProcess(int id, int burst) : pid(id), burstTime(burst), remainingTime(burst)
    {

    }
};

// Each process/thread runs this
void runProcess(SimulatedProcess& proc)
{
    while (proc.remainingTime > 0 && !stop_flag)
    {
        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // Very crucial to avoid spin-lock

        // if (!cpu_mutex.try_lock())
        // {
        //     continue; // If CPU is busy, skip this iteration
        // }

        std::unique_lock<std::mutex> lock(cpu_mutex); // simulate one CPU core
        proc.state = "RUNNING";
        std::cout << "[CPU] PID " << proc.pid << " is RUNNING. Time left: " << proc.remainingTime << "\n";

        std::this_thread::sleep_for(std::chrono::milliseconds(500)); // simulate 1 time slice
        proc.remainingTime -= 1;

        proc.state = (proc.remainingTime > 0) ? "READY" : "TERMINATED";
        std::cout << "[CPU] PID " << proc.pid << " is now " << proc.state << "\n";
    }
}

int main()
{
    std::vector<SimulatedProcess> processList =
    {
        SimulatedProcess(1, 4),
        SimulatedProcess(2, 3),
        SimulatedProcess(3, 5)
    };

    std::vector<std::thread> threadList;

    std::cout << "Starting Round Robin Simulation (1s per time slice)\n";

    for(auto& proc : processList)
    threadList.emplace_back(runProcess, std::ref(proc));

    for(auto& t : threadList)
    {
        if(t.joinable())
        t.join();
    }

    std::cout << "Simulation complete.\n";

    return 0;
}
