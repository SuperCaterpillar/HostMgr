#include "../PublicFun/PublicFun.h"
#include "../PublicFun/HardWareInfo.h"
#include "../PublicFun/SendDataToService.h"
#include "SystemResource.h"


SystemResource::SystemResource()
{

}

SystemResource::SystemResource(int runInterval):BaseEvnClass(),runTimer(runInterval)
{
    runTimer.connect(boost::bind(&SystemResource::statspack,this));
}

bool SystemResource::start()
{
    runTimer.start();
    return true;
}

bool SystemResource::stop()
{
    runTimer.stop();
    return true;
}

bool SystemResource::report_event()
{
    ptree::ptree report,cpu,mem,disk;

    report.put("type.<xmlattr>.name","system");
    report.put("type.cpu",cpuRate);
    report.put("type.mem",memRate);
    report.put("type.disk",diskRate);
    diskRate = 0;
    memRate = 0;
    cpuRate = 0;

    SendDataToService::SendDataToService(report);

    return true;
}

bool SystemResource::strategy_update(int t)
{
    runTimer.reset_timer(t);
    return true;
}

DECL_SHARED_PTR(SystemResource)
SystemResourceSharedPtr SystemResource::creater(int time)
{
    return boost::make_shared<SystemResource>(time);
}

void SystemResource::statspack()
{

    cpu_utilization_ratio(cpuRate);
    mem_utilization_ratio(memRate);
    disk_utilization_ratio(diskRate);
    report_event();
}

bool SystemResource::cpu_utilization_ratio(float &rate)
{
    string ratio;
    PublicFun::SystemEx("top -bn 1 | grep -w id | awk '{print $8}'",ratio);
    if (ratio.empty())
    {
        rate = 0;
        LogError <<("SystemResource::cpu_utilization_ratio 获取cpu使用率错误");
        return false;
    }
    rate = 100 - lexical_cast<float>(ratio);
    return true;
}

bool SystemResource::mem_utilization_ratio(float &rate)
{
    std::string tmp;
    std::string total;
    std::string used;

    PublicFun::SystemEx("top -bn 1 | grep \"KiB Mem\" | awk '{print $4,$8}'",tmp);
    if (tmp.empty())
    {
        rate = 0;
        LogError<<("SystemResource::mem_utilization_ratio 无法获取内存使用率");
        return false;
    }

    total = tmp.substr(0,tmp.find(" "));
    if (total.empty() || total == "0")
    {
        rate = 0;
        LogError<<("SystemResource::mem_utilization_ratio  获取内存总大小错误");
        return false;
    }

    used = tmp.substr(tmp.find(" ") +1,tmp.length());
    rate = lexical_cast<float>(used) /lexical_cast<float>(total);

    return true;
}

bool SystemResource::disk_utilization_ratio(float &rate)
{
    uint64_t used = 0;
    uint64_t total = 0;

    const auto& devSizes = HardWareInfo::GetDeviceSize();
    const auto& devNames = HardWareInfo::GetDiskDeviceName();

    for (auto iter = devNames.begin(); iter != devNames.end(); ++iter)
    {
        auto result = devSizes.find(*iter);
        if (result != devSizes.end())
        {
            used += result->second.first;
            total += result->second.second;
        }
    }
    rate = used / total;
    //LogInfo<<"disk_utilization_ratio "
    return true;
}
