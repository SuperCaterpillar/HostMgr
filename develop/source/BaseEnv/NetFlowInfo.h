#ifndef NETFLOWINFO_H
#define NETFLOWINFO_H

#include "BaseEvnClass.h"

//定义网络流量结构体
struct stuNetFlow{
    string  strNetAdapter;   //网卡
    double  InFlow;        //流入流量(MB)
    double  OutFlow;       //流出流量(MB)
    double  TotalFlow;     //总流量(MB)
};
DECL_VET(stuNetFlow)

class NetFlowInfo : public BaseEvnClass
{
public:
    NetFlowInfo(int runInterval);

    virtual bool start();

    virtual bool stop();

    //上报日志
    virtual bool report_event();

    //策略更新事件
    virtual bool strategy_update(int);

    DECL_SHARED_PTR(NetFlowInfo)
    static NetFlowInfoSharedPtr creater(int runInterval);
private:
    //运行具体功能
    void run();
    bool net_flow(stuNetFlowVet & vecNetFlow);


    int interval;
};

#endif // NETFLOWINFO_H
