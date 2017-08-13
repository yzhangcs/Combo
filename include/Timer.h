/*******************************************************************************
 * Timer.h
 *
 * Author: zhangyu
 * Date: 2017.5.9
 ******************************************************************************/

#pragma once
#include <chrono>

/**
 * 计时器，用于测量程序运行时间.
 * 提供了产生毫秒精度时间戳的静态方法.
 */
class Timer
{
private:
    size_t time;
public:
    Timer() { time = time_millis(); }
    
    // 产生毫秒精度的时间戳
    static size_t time_millis();
    // 开始计时
    void start() { time = time_millis(); } 
    // 重新计时
    void reset() { time = time_millis(); } 
    // 查看从开始计时到当前的总秒数
    double elapsed() { return (time_millis() - time) / 1000.0; }
};

/**
 * 产生毫秒精度的时间戳.
 *
 * @return 毫秒精度的时间戳
 */
size_t Timer::time_millis()
{
    using millis = std::chrono::milliseconds;
    using system_clock = std::chrono::system_clock;
    return std::chrono::duration_cast<millis>(system_clock::now().time_since_epoch()).count();
}


