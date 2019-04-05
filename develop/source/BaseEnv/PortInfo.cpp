#include "../PublicFun/Tools.h"
#include "../PublicFun/PublicFun.h"
#include "../PublicFun/SendDataToService.h"


#include "PortInfo.h"



PortInfo::PortInfo(int runInterval):interval(runInterval)
{
    timer.connect(boost::bind(&PortInfo::run,this));

}

bool PortInfo::start()
{
    timer.start(interval);
    return true;
}

bool PortInfo::stop()
{
    timer.stop();
    return true;
}

bool PortInfo::report_event()
{

    return true;
}

bool PortInfo::strategy_update(int t)
{
    timer.reset_timer(t);
    return true;
}

DECL_SHARED_PTR(PortInfo)
PortInfoSharedPtr PortInfo::creater(int runInterval)
{
    return  boost::make_shared<PortInfo>(runInterval);
}

void PortInfo::run()
{
    vector<string> lineVec;
    string cmd = "netstat -anptu | awk '{if(NR != 1 && NR != 2) print $0 }'";

    PublicFun::SystemEx(cmd, lineVec);

    ptree::ptree report;
    report.put("type.<xmlattr>.name", "PortInfo");
    std::string pattern(" ");
    for(auto it = lineVec.begin(); it != lineVec.end(); it++)
    {
        std::vector<std::string> splitStr =Tools::SplitString(*it,pattern);
        std::string proto = splitStr[0];
        std::string localIpAndPort = splitStr[3];
        std::string remoteIpAndPort = splitStr[4];
        std::string ProcessIDAndPath;
        std::string status;

        std::string localIp = localIpAndPort.substr(0, localIpAndPort.find_last_of(":"));
        std::string localPort = localIpAndPort.substr(localIpAndPort.find_last_of(":") + 1);
        std::string remoteIp = remoteIpAndPort.substr(0, remoteIpAndPort.find_last_of(":"));
        std::string remotePort = remoteIpAndPort.substr(remoteIpAndPort.find_last_of(":") + 1);

        if (proto.compare(0, 3, "tcp") == 0)
        {
            status = splitStr[5];
            ProcessIDAndPath = splitStr[6];

        }
        else
        {
            status = "";
            ProcessIDAndPath = splitStr[5];
        }
        ptree::ptree child;
        child.add("Protocol", proto);                       //协议
        child.add("LocalIP",localIp);                       //本地IP
        child.add("LocalPort",localPort);                   //本地端口
        child.add("RemoteIP", remoteIp);                    //远程IP
        child.add("RemotePort",remotePort);                 //远程端口
        child.add("Status",status);                         //端口状态
        report.add_child("type.item",child);
        //report.put("type.ProcessID",processID);                 //进程号
        //report.put("type.ProcessPath",processPath);   //进程路径

    }
    SendDataToService::SendDataToService(report);
}
