#ifndef STRATEGYBASECLASS_H
#define STRATEGYBASECLASS_H

#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>

#include "PublicFun.h"
#include "Publicdefine.h"

namespace ptree = boost::property_tree;


DECL_CLASS_PRIVATE_DATA(StrategyBaseClass)


class StrategyBaseClass
{
protected:
    //指定策略模块要监控的策略名称
    StrategyBaseClass(const string& strategyName);
    virtual ~StrategyBaseClass();

    bool monitor_strategy_modif(file_sys_monitor_call);

    string strategy_name();
protected:
    //获取策略
    ptree::ptree get_stragety_ptree();

    //策略是否可用
    bool usable();

    //子类实现具体细节
    virtual bool parse() = 0;

private:
    StrategyBaseClassPrivatePtr m_data;
};

#endif // STRATEGYBASECLASS_H
