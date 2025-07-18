#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex mutexA;
std::mutex mutexB;

void thread1() {
    std::lock_guard<std::mutex> lock1(mutexA);
    std::cout << "Thread 1 locked Resource A\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread 1 trying to lock Resource B\n";

    std::lock_guard<std::mutex> lock2(mutexB); // May deadlock
    std::cout << "Thread 1 locked Resource B\n";
}

void thread2() {
    std::lock_guard<std::mutex> lock2(mutexB);
    std::cout << "Thread 2 locked Resource B\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(100));
    std::cout << "Thread 2 trying to lock Resource A\n";

    std::lock_guard<std::mutex> lock1(mutexA); // May deadlock
    std::cout << "Thread 2 locked Resource A\n";
}

int main() {
    std::thread t1(thread1);
    std::thread t2(thread2);

    t1.join();
    t2.join();

    std::cout << "Deadlock simulation complete.\n";
    return 0;
}
