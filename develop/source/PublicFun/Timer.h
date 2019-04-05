#ifndef TIMER_H
#define TIMER_H
#include <boost/asio.hpp>
#include <boost/signals2.hpp>
#include <boost/bind.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

typedef boost::function<void()> time_callback;
class Timer
{
public:
    Timer();
    Timer(int interval);
    //on_timer(const boost::system::error_code& error)
    //开始执行
    void start();
    void start(int second);
    void stop();
    //为定时起添加一个回调函数
    void connect(time_callback/*boost::function<void()>*//*const boost::system::error_code& error*/);
    //重置定时器时间
    void reset_timer(int interval);
private:
    void time_run();
    void on_time();
private:
    boost::asio::io_service io;
    boost::asio::deadline_timer timer;
    boost::thread thread;
    time_callback callback;
    int interval_time;
};

#endif // TIMER_H
