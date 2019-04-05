#include <iostream>

#include "Timer.h"

Timer::Timer():timer(io,boost::posix_time::seconds(0)),interval_time(0)
{
    //timer.async_wait(boost::bind(&Timer::connect,this,boost::asio::placeholders::error));
}

Timer::Timer(int interval):timer(io,boost::posix_time::seconds(interval)),interval_time(interval)
{
    //timer.async_wait(boost::bind(&Timer::connect,this,boost::asio::placeholders::error));
}

void Timer::start()
{
    boost::thread t(boost::bind(&Timer::time_run,this));
    t.detach();
}

void Timer::start(int second)
{
    interval_time = second;
    timer.expires_from_now(boost::posix_time::seconds(second));
    boost::thread t(boost::bind(&Timer::time_run,this));
    t.detach();
}

void Timer::stop()
{
    timer.cancel();
}


void Timer::connect(time_callback call)
{
    callback = call;
}

void Timer::reset_timer(int interval)
{
    interval_time = interval;
}

void Timer::time_run()
{
    timer.async_wait(boost::bind(&Timer::on_time,this/*,boost::asio::placeholders::error*/));
    //会阻塞线程
    io.run();
}

void Timer::on_time()
{
    callback();
    timer.expires_from_now(boost::posix_time::seconds(interval_time));
    timer.async_wait(boost::bind(&Timer::on_time,this/*,boost::asio::placeholders::error*/));
}
