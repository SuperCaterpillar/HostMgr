#ifndef SYSTEMRESOURCE_H
#define SYSTEMRESOURCE_H

#include "BaseEvnClass.h"
#include "../PublicFun/Timer.h"
#include "../PublicFun/Publicdefine.h"



class SystemResource :public BaseEvnClass
{
public:
    SystemResource();
    SystemResource(int runInterval);


    bool start();
    bool stop();
    bool report_event();
    bool strategy_update(int ) ;
    DECL_SHARED_PTR(SystemResource)
    static SystemResourceSharedPtr creater(int);

    void statspack();

    //采集cpu使用率
    bool cpu_utilization_ratio(float &rate);
    //采集内存使用率
    bool mem_utilization_ratio(float &rate);
    //采集硬盘使用率
    bool disk_utilization_ratio(float &rate);
private:
   Timer runTimer;
   float cpuRate;
   float memRate;
   float diskRate;

};

#endif // SYSTEMRESOURCE_H
