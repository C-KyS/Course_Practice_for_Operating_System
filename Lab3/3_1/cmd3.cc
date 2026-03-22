// mytime
// 输出当前系统时间

#include <iostream>
#include <ctime>
int main() {
    std::time_t current_time = std::time(nullptr);
    std::cout << "Current time: " << std::ctime(&current_time);
    return 0;
}
