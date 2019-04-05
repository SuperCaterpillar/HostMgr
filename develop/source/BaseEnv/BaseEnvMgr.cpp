#include "../PublicFun/kernelmacro.h"
#include "../PublicFun/Logging.h"

#include "BaseEvnStrategy.h"
#include "SystemResource.h"
#include "SoftwareInfo.h"
#include "NetFlowInfo.h"
#include "AccountInfo.h"
#include "FirefoxInfo.h"
#include "ProcessInfo.h"
#include "BaseEnvMgr.h"
#include "PortInfo.h"

BaseEnvMgr::BaseEnvMgr():strategy(BASEENV_PATH)
{
    //注册回调函数，当策略改变时调用
    strategy.reg_callback(boost::bind(&BaseEnvMgr::update_base_list,this));
}

void BaseEnvMgr::del_run_modules(const string &name)
{
    if(name.empty())
    {
        base_map.clear();
        LogInfo<<"所有基础环境功能已被清除"<<endl;
        return ;
    }
    auto it = base_map.find(name);
    if( it != base_map.end() )
    {
        base_map.erase(it);
        LogInfo<<"基础环境功能 ：" + name + " 已被清除"<<endl;
    }

}

bool BaseEnvMgr::start()
{
    update_base_list();
    return true;
}

bool BaseEnvMgr::stop()
{
    base_map.clear();
    return true;
}

bool BaseEnvMgr::update_base_list()
{
    //获取策略中新增的数据
    auto newData = strategy.new_strategy_data();
    for(auto iter = newData.begin(); iter != newData.end(); iter++)
    {
        if(iter->first == "System")
        {
            base_map["System"] = SystemResource::creater(iter->second);
            base_map["System"]->start();
            LogInfo<<"创建SystemResource成功"<<endl;
        }
        else if(iter->first == "PortInfo")
        {
            base_map["PortInfo"] = PortInfo::creater(iter->second);
            base_map["PortInfo"]->start();
            LogInfo<<"创建PortInfo成功"<<endl;
        }
        else if(iter->first == "AccountInfo")
        {
            base_map["AccountInfo"] = AccountInfo::creater(iter->second);
            base_map["AccountInfo"]->start();
            LogInfo<<"创建AccountInfo成功"<<endl;
        }
        else if(iter->first == "FirefoxInfo")
        {
            base_map["FirefoxInfo"] = FirefoxInfo::creater(iter->second);
            base_map["FirefoxInfo"]->start();
            LogInfo<<"创建FirefoxInfo成功"<<endl;
        }
        else if(iter->first == "NetFlowInfo")
        {
            base_map["NetFlowInfo"] = NetFlowInfo::creater(iter->second);
            base_map["NetFlowInfo"]->start();
            LogInfo<<"创建NetFlowInfo成功"<<endl;
        }
        else if(iter->first == "ProcessInfo")
        {
            base_map["ProcessInfo"] = ProcessInfo::creater(iter->second);
            base_map["ProcessInfo"]->start();
            LogInfo<<"创建ProcessInfo成功"<<endl;
        }
        else if(iter->first == "SoftwareInfo")
        {
            base_map["SoftwareInfo"] = SoftwareInfo::creater(iter->second);
            base_map["SoftwareInfo"]->start();
            LogInfo<<"创建 SoftwareInfo 成功"<<endl;
        }
    }

    //获取策略中删除的数据
    auto delData = strategy.del_strategy_data();
    for(auto iter = delData.begin(); iter != delData.end(); iter++)
    {
        auto it = base_map.find(iter->first);
        if( it != base_map.end() )
        {
            LogInfo<<"基础6环境模块 ：" + iter->first + "已被清除"<<endl;
            it->second->stop();
            base_map.erase(it);
        }

    }

    //获取链表中修改的数据
    auto modData = strategy.mod_strategy_data();
    for(auto iter = modData.begin(); iter != modData.end(); iter++)
    {
        ///iter->second->strategy_update(iter->second);
        auto it = base_map.find(iter->first);
        if(it != base_map.end())
        {
            LogInfo<<"基础环境模块 " + iter->first + "以更新" << endl;
            it->second->strategy_update(iter->second);
        }
    }

    return true;
}


