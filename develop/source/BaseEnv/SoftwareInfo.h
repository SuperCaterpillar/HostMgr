#ifndef SOFTWAREINFO_H
#define SOFTWAREINFO_H
#include "../PublicFun/Publicdefine.h"


#include "BaseEvnClass.h"

typedef struct SoftStruct
{
    string status;
    string name;
    string version;
    string sys_structure;
    string description;
}SoftStruct;
DECL_VET(SoftStruct)

class SoftwareInfo : public BaseEvnClass
{
public:
    SoftwareInfo(int runInterval);
    virtual bool start();
    virtual bool stop();

    //上报日志
    virtual bool report_event();

    //策略更新事件
    virtual bool strategy_update(int);

    DECL_SHARED_PTR(SoftwareInfo)
    static SoftwareInfoSharedPtr creater(int runInterval);
private:
    //运行具体功能
    void run();


    int interval;
};

#endif // SOFTWAREINFO_H
