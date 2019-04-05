
#include "StrategyBaseClass.h"


namespace ptree = boost::property_tree;


DECL_SHARED_PTR(thread);

struct StrategyBaseClassPrivate
{
    StrategyBaseClassPrivate():is_usable(false)
    {

    }

    bool read(const string& strategyName)
    {
        try {
            ptree::read_xml(strategyName,strategy_ptree);
            is_usable = true;
        } catch (...) {
            is_usable = false;
            LogError<<"没有发现该策略文件  "<<strategyName<<endl;
            return is_usable;
        }

        strategy_name = strategyName;

        return is_usable;
    }

    bool monitor_file()
    {
        stringVet path;
        path.push_back(strategy_name);

        strategy_modif_thread =
                boost::make_shared<thread>(bind(&PublicFun::monitor_file_system,IN_CREATE|IN_DELETE|IN_MODIFY,path,thread_call));

        strategy_modif_thread->detach();

        return true;
    }

    bool is_usable;

    string strategy_name;
    ptree::ptree strategy_ptree;

    file_sys_monitor_call thread_call;
    threadSharedPtr strategy_modif_thread;
};

StrategyBaseClass::StrategyBaseClass(const string& strategyName)
{
    m_data = boost::make_shared<StrategyBaseClassPrivate>();

    m_data->read(strategyName);
}

StrategyBaseClass::~StrategyBaseClass()
{
    //m_data->strategy_modif_thread->interrupt();
    //取消系统监控的文件
}

bool StrategyBaseClass::monitor_strategy_modif(file_sys_monitor_call fun)
{
    m_data->thread_call = fun;
    if(m_data->is_usable)
        m_data->monitor_file();
    return true;
}

string StrategyBaseClass::strategy_name()
{
    return m_data->strategy_name;
}

ptree::ptree StrategyBaseClass::get_stragety_ptree()
{
    return m_data->strategy_ptree;
}

bool StrategyBaseClass::usable()
{
    return m_data->is_usable;
}


