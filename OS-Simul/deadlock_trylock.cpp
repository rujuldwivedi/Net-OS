#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mutexA;
std::mutex mutexB;

void safeThread(std::string name) {
    while (true) {
        if (mutexA.try_lock()) {
            std::cout << name << " locked A\n";

            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            if (mutexB.try_lock()) {
                std::cout << name << " locked B\n";

                // Simulate work
                std::this_thread::sleep_for(std::chrono::milliseconds(100));

                mutexB.unlock();
                mutexA.unlock();
                std::cout << name << " released both\n";
                break;
            } else {
                std::cout << name << " couldn't lock B, releasing A and retrying\n";
                mutexA.unlock();
            }
        }

        std::this_thread::sleep_for(std::chrono::milliseconds(10)); // avoid busy waiting
    }
}

int main() {
    std::thread t1(safeThread, "Thread 1");
    std::thread t2(safeThread, "Thread 2");

    t1.join();
    t2.join();

    std::cout << "No deadlock: using try_lock.\n";
    return 0;
}