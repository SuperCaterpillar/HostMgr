#include "../PublicFun/Tools.h"
#include "../PublicFun/ProcessDispose.h"

#include "ProcessInfo.h"


ProcessInfo::ProcessInfo(int runInterval):interval(runInterval)
{
    timer.connect(boost::bind(&ProcessInfo::run,this));
}

bool ProcessInfo::start()
{
    timer.start(interval);
    return true;
}

bool ProcessInfo::stop()
{
    timer.stop();
    return true;
}

bool ProcessInfo::report_event()
{
    return true;
}

bool ProcessInfo::strategy_update(int t)
{
    timer.reset_timer(t);
    return true;
}

DECL_SHARED_PTR(ProcessInfo)
ProcessInfoSharedPtr ProcessInfo::creater(int runInterval)
{
    return  boost::make_shared<ProcessInfo>(runInterval);
}

void ProcessInfo::run()
{
    vector<string> psInfo;
    PublicFun::SystemEx("ps -efl | awk '{if(NR != 1)print $3,$4,$5,$7}' ",psInfo);

    ptree::ptree root;
    root.add("type.<xmlattr>.name", "ProcessInfo");

    std::string pattern = " ";
    //遍历进程信息
    for(size_t i = 0; i < psInfo.size(); i++)
    {
        std::vector<std::string> strPsSplit = Tools::SplitString(psInfo[i], pattern);
        std::string strUser = strPsSplit[0];
        std::string strPid = strPsSplit[1];
        std::string strPpid = strPsSplit[2];
        std::string strPRI = strPsSplit[3];

        pid_t ipid = atoi(strPid.c_str());

        std::string startTime = ProcessDispose::process_start_time(ipid);
        if (startTime.empty())
        {
            continue;
        }
        std::string processName = ProcessDispose::process_name(ipid);
        if (processName.empty() || strncmp("VET", processName.c_str(), 3) == 0)
        {
            continue;
        }
        std::string proccesPath = ProcessDispose::read_proc_exe(ipid);
        if (proccesPath.empty())
        {
            continue;
        }
        ptree::ptree child;


        child.add("Name", processName);          //进程名
        child.add("Vendor","");           //供应商
        child.add("Package","");          //包名
        child.add("ProcessID",strPid);               //进程ID
        child.add("ParentProcessID", strPpid);         //父进程ID
        child.add("Priority", strPRI);                //优先级
        child.add("ThreadCount",ProcessDispose::thread_size(ipid));               //线程统计
        child.add("WorkingSetSize",ProcessDispose::process_memory(ipid));      //内存大小
        //字符串为空时，substr不会有问题
        child.add("StartTime",startTime);         //启动时间
        child.add("ExecutablePath",proccesPath);       //进程路径
        child.add("User",strUser);                        //执行用户
        root.add_child("type.Results.Result",child);
    }
    SendDataToService::SendDataToService(root);
}
