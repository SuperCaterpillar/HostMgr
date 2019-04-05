#ifndef PROCESSINFO_H
#define PROCESSINFO_H
#include "BaseEvnClass.h"

class ProcessInfo : public BaseEvnClass
{
public:
    ProcessInfo(int runInterval);

    virtual bool start();

    virtual bool stop();

    //上报日志
    virtual bool report_event();

    //策略更新事件
    virtual bool strategy_update(int);

    DECL_SHARED_PTR(ProcessInfo)
    static ProcessInfoSharedPtr creater(int runInterval);
private:
    //运行具体功能
    void run();


    int interval;
};

#endif // PROCESSINFO_H
