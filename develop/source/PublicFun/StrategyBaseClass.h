#ifndef STRATEGYBASECLASS_H
#define STRATEGYBASECLASS_H
#include "KernelDefine.h"

DECL_CLASS_PRIVATE_DATA(StrategyBaseClass)


class StrategyBaseClass
{
public:
    StrategyBaseClass(const string& strategyName);
    virtual ~StrategyBaseClass();

    bool monitor_strategy_modif(file_sys_monitor_call);

private:
    //策略是否可用
    bool usable();

    //子类实现具体细节
    virtual bool parse() = 0;

private:
    StrategyBaseClassPrivatePtr m_data;
};

#endif // STRATEGYBASECLASS_H
