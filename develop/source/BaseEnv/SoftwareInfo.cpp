#include "../PublicFun/SendDataToService.h"
#include "../PublicFun/PublicFun.h"
#include "../PublicFun/Tools.h"

#include "SoftwareInfo.h"


SoftwareInfo::SoftwareInfo(int runInterval):interval(runInterval)
{
    timer.connect(boost::bind(&SoftwareInfo::run,this));

}

bool SoftwareInfo::start()
{
    timer.start(interval);
    return true;
}

bool SoftwareInfo::stop()
{
    timer.stop();
    return true;
}

bool SoftwareInfo::report_event()
{
    return true;
}

bool SoftwareInfo::strategy_update(int t)
{
    timer.reset_timer(t);
    return true;
}

DECL_SHARED_PTR(SoftwareInfo)
SoftwareInfoSharedPtr SoftwareInfo::creater(int runInterval)
{
    return  boost::make_shared<SoftwareInfo>(runInterval);
}

void SoftwareInfo::run()
{
    string pattern = " ";
    vector<string> info;
    SoftStructVet soft;
    info.reserve(500);
    soft.reserve(500);

    PublicFun::SystemEx("dpkg -l|awk '{if(NR>5)print $0}'",info);
    for(string &v: info)
    {
        auto tmp = Tools::SplitString(v,pattern);
        SoftStruct tSoft;
        for (size_t i = 0; i < tmp.size(); i++)
        {
            switch (i)
            {
            case 0:
                tSoft.status = tmp[i];
                break;
            case 1:
                tSoft.name = tmp[i];
                break;
            case 2:
                tSoft.version = tmp[i];
                break;
            case 3:
                tSoft.sys_structure = tmp[i];
                break;
            default:
                tSoft.description += tmp[i];
                break;
            }
         }
        soft.push_back(tSoft);
    }

    ptree::ptree report;
    report.add("type.<xmlattr>.name", "SoftwareInfo");
    for(SoftStruct &it : soft)
    {
        ptree::ptree child;
        child.add("status",it.status);
        child.add("name",it.name);
        child.add("version",it.version);
        child.add("sys_structure",it.sys_structure);
        child.add("description",it.description);
        report.add_child("type.Results.Result",child);
    }
    SendDataToService::SendDataToService(report);
}
