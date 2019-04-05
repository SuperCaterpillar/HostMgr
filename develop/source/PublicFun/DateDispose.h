#ifndef DATEDISPOSE_H
#define DATEDISPOSE_H
#include <boost/date_time.hpp>
#include <boost/date_time/date_generators.hpp>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time/gregorian/gregorian_types.hpp>

#include <chrono>
#include <string>

using namespace std;
using namespace boost::posix_time;
using namespace boost::gregorian;
namespace DateDispose {

    time_duration milliseconds_to_date(long long mill);
    std::time_t getTimeStamp();
    std::tm* gettm(long long timestamp);
    string firefox_history_time(long long mill);
}

#endif // DATEDISPOSE_H
