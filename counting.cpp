#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>

std::mutex mtx;
std::condition_variable cv;
bool firstDone = false; // Flag to indicate when counting up is done

void Up() {
    std::cout << "Counting Up" << std::endl;
    for (int i = 0; i <= 20; ++i) {
        std::cout << i << std::endl;
    }
    
    // Notify that the first thread is done
    std::lock_guard<std::mutex> lock(mtx);
    firstDone = true;
    cv.notify_one();
}
 
void Down() {
    // Wait until countUp finishes
    std::unique_lock<std::mutex> lock(mtx);
    cv.wait(lock, [] { return firstDone; });
    
     std::cout << "Counting Down" << std::endl;
    for (int i = 20; i >= 0; --i) {
        std::cout<< i << std::endl;
    }
}

int main() {
    std::thread thread1(Up);
    std::thread thread2(Down);
    
    thread1.join();
    thread2.join();
    
    return 0;
}

/*
  -This application has 2 threads and executes one after another, this could cause
  overhead which consumes CPU cycles.
  -using char[] or C strings could lead to overflows of the buffer 
  if the input exceeds the buffer size.
  -Using the firstDone prevents the flag to have race conditions 
  using the std::mutex, without it different threads might get incorrect values.
*/
