#include <chrono>
#include <iostream>
#include <mutex>
#include <string>
#include <thread>

#include "blockQueue.h"

BlockQueue<std::string> queue;
std::mutex printMutex;

void producer(int productNum, int waitMilliseconds) {
    for (int i = 0; i < productNum; ++i) {
        std::string message{"Message-"};
        message.append(std::to_string(i));
        queue.push(message);
        std::cout << "push data" << message << std::endl;
        std::this_thread::sleep_for(
            std::chrono::milliseconds(waitMilliseconds));
    }
    std::lock_guard<std::mutex> lock(printMutex);
    std::cout << "producer push message ok!\n";
}

void consumer(int consumerID) {
    while (true) {
        std::string message;
        queue.pop(message);
        {
            std::lock_guard<std::mutex> lock(printMutex);
            std::cout << "consumer-" << consumerID << " receive:" << message
                      << std::endl;
        }
    }
}

int main() {
    std::thread thProducer1{producer, 10, 1};
    std::thread thProducer2{producer, 10, 1};
    std::thread thConsumer1{consumer, 1};
    std::thread thConsumer2{consumer, 2};

    thProducer1.join();
    thProducer2.join();
    thConsumer1.join();
    thConsumer2.join();

    return 0;
}