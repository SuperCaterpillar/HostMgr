
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include <boost/function.hpp>
#include <boost/thread.hpp>


#include "StrategyBaseClass.h"
#include "PublicFun.h"

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
                boost::make_shared<thread>(bind(&PublicPun::monitor_file_system,IN_ALL_EVENTS,path,thread_call));

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
    m_data->strategy_modif_thread->interrupt();
}

bool StrategyBaseClass::monitor_strategy_modif(file_sys_monitor_call fun)
{
    m_data->thread_call = fun;
    if(m_data->is_usable)
        m_data->monitor_file();
    return true;
}

bool StrategyBaseClass::usable()
{
    return m_data->is_usable;
}


