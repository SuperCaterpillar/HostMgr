#include <time.h>

#include "DateDispose.h"

time_duration DateDispose::milliseconds_to_date(long long mill)
{
    return milliseconds(mill);
}

time_t DateDispose::getTimeStamp()
{
    std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
    auto tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
    std::time_t timestamp = tmp.count();
    //std::time_t timestamp = std::chrono::system_clock::to_time_t(tp);
    return timestamp;
}

tm *DateDispose::gettm(long long timestamp)
{
    long long milli = timestamp+ (long long)8*60*60*1000;//此处转化为东八区北京时间，如果是其它时区需要按需求修改
    auto mTime = std::chrono::milliseconds(milli);
    auto tp=std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds>(mTime);
    auto tt = std::chrono::system_clock::to_time_t(tp);
    std::tm* now = std::gmtime(&tt);
    printf("%4d年%02d月%02d日 %02d:%02d:%02d\n",now->tm_year+1900,now->tm_mon+1,now->tm_mday,now->tm_hour,now->tm_min,now->tm_sec);
    return now;

}

string DateDispose::firefox_history_time(long long mill)
{
    struct tm *local;
    time_t t;
    t = mill/1000000;
    local = localtime(&t);
    char buf[100];
    sprintf(buf,"%d-%02d-%02d %02d:%02d:%02d", local->tm_year + 1900, local->tm_mon + 1, local->tm_mday, local->tm_hour, local->tm_min, local->tm_sec);
    cout << buf << endl;
    string strTime(buf);
    return strTime;
}
