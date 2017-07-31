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
    Timer() { time = timestampMs(); }
    
    static size_t timestampMs();
    void start() { time = timestampMs(); } // 开始计时
    void reset() { time = timestampMs(); } // 重新计时
    // 查看从开始计时到当前的总秒数
    double elapsed() { return (timestampMs() - time) / 1000.0; }
};

/**
 * 产生毫秒精度的时间戳.
 *
 * @return 毫秒精度的时间戳
 */
size_t Timer::timestampMs()
{
    using ms = std::chrono::milliseconds;
    using system_clock = std::chrono::system_clock;
    return std::chrono::duration_cast<ms>(system_clock::now().time_since_epoch()).count();
}


