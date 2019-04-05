#include "NetFlowInfo.h"

NetFlowInfo::NetFlowInfo(int runInterval):interval(runInterval)
{
    timer.connect(boost::bind(&NetFlowInfo::run,this));
}

bool NetFlowInfo::start()
{
    timer.start(interval);
    return true;
}

bool NetFlowInfo::stop()
{
    timer.stop();
    return true;
}

bool NetFlowInfo::report_event()
{

    return true;
}

bool NetFlowInfo::strategy_update(int t)
{
    timer.reset_timer(t);
    return true;
}

DECL_SHARED_PTR(NetFlowInfo)
NetFlowInfoSharedPtr NetFlowInfo::creater(int runInterval)
{
    return  boost::make_shared<NetFlowInfo>(runInterval);
}

void NetFlowInfo::run()
{
    stuNetFlowVet vecNetFlow;
    net_flow(vecNetFlow);


    ptree::ptree root,node;

    root.add("type.<xmlattr>.name","NetFlowInfo");
    for(auto v : vecNetFlow)
    {
        ptree::ptree child;

        char  szInFlow[20] = {0};
        char  szOutFlow[20] = {0};
        char  szTotalFlow[20] = {0};
        sprintf(szInFlow, "%.2fMB", v.InFlow);
        sprintf(szOutFlow, "%.2fMB", v.OutFlow);
        sprintf(szTotalFlow, "%.2fMB", v.TotalFlow);
        child.add("NetAdpter",v.strNetAdapter);
        child.add("InFlow", szInFlow);
        child.add("OutFlow", szOutFlow);
        child.add("TotalFlow", szTotalFlow);
        root.add_child("type.Results.Result",child);
    }
    SendDataToService::SendDataToService(root);
}

bool NetFlowInfo::net_flow(stuNetFlowVet &vecNetFlow)
{
    char szFilePath[256] = {0};
    char szTmp[10][256] = {{0}};
    stuNetFlow stuTmpFlow;

    //网络流量路径
    snprintf(szFilePath, sizeof(szFilePath), "/proc/net/dev");

    std::ifstream streamInPut(szFilePath);
    if (streamInPut.is_open())
    {
        std::string strLine = "";
        int nLineCnt = 0;    //标识当前读取的行数

        //读取文件中的每一行
        while(std::getline(streamInPut, strLine))
        {
            //memset(&stuTmpFlow, 0, sizeof(stuTmpFlow));
            nLineCnt++;

            //第三行开始为数据
            if(nLineCnt >= 3)
            {
                sscanf(strLine.c_str(), "%s\t%[^' ']\t%[^' ']\t%[^' ']\t%[^' ']\t%[^' ']\t%[^' ']\t%[^' ']\t%[^' ']\t%[^' ']",
                       szTmp[0], szTmp[1], szTmp[2], szTmp[3], szTmp[4], szTmp[5], szTmp[6], szTmp[7], szTmp[8], szTmp[9]);

                stuTmpFlow.strNetAdapter = szTmp[0];
                //去除末尾的:
                stuTmpFlow.strNetAdapter = stuTmpFlow.strNetAdapter.substr(0, stuTmpFlow.strNetAdapter.length() - 1);
                //获取网络流量单位为Byte，将其转化为MB
                stuTmpFlow.InFlow = atof(szTmp[1])/1024.0/1024.0;
                stuTmpFlow.OutFlow = atof(szTmp[9])/1024.0/1024.0;

                stuTmpFlow.TotalFlow = (atof(szTmp[1]) + atof(szTmp[9]))/1024.0/1024.0;
                vecNetFlow.push_back(stuTmpFlow);
            }
            strLine.clear();
        }

        streamInPut.close();
    }
    else
    {
        return false;
    }

    return true;
}
