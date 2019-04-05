#ifndef PORTINFO_H
#define PORTINFO_H
#include "../PublicFun/Publicdefine.h"


#include "BaseEvnClass.h"

class PortInfo : public BaseEvnClass
{
public:
    PortInfo(int runInterval);

    virtual bool start();
    virtual bool stop();

    //上报日志
    virtual bool report_event();

    //策略更新事件
    virtual bool strategy_update(int);

    DECL_SHARED_PTR(PortInfo)
    static PortInfoSharedPtr creater(int runInterval);
private:
    //运行具体功能
    void run();


    int interval;
};

#endif // PORTINFO_H
