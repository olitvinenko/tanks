#include "ThreadPool.h"

#include <thread>
#include <mutex>
#include <chrono>

#include <iostream>

void threadingTest()
{
    ThreadPool thp;
    
    std::mutex mtx;
    
    thp.Enqueue([&](int i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lg(mtx);
        std::cout << "thread " << i << std::endl;
    }, 10);
    
    thp.Enqueue([&](int i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lg(mtx);
        std::cout << "thread " << i << std::endl;
    }, 11);
    
    thp.Enqueue([&](int i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lg(mtx);
        std::cout << "thread " << i << std::endl;
    }, 12);
    
    thp.Enqueue([&](int i) {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        std::lock_guard<std::mutex> lg(mtx);
        std::cout << "thread " << i << std::endl;
    }, 13);
    
    
    auto result = thp.Enqueue([]() {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        return 100400;
    });
    
    std::this_thread::sleep_for(std::chrono::seconds(2));
    
    std::cout << result.get() << std::endl;
}
