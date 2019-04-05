#include "BaseEvnStrategy.h"


struct BaseEvnStrategyPrivate
{
    //系统信息时间间隔
//    int systemInt;
//    tribool systemTri;

    map<string,int> addStrategyDataMap;
    map<string,int> delStrategyDataMap;
    map<string,int> modStrategyDataMap;
    //string 代表从策略文件读出的要执行的功能
    //int    代表每隔多长时间执行一次该功能
    map<string,int> allStrategyDataMap;
    boost::function<bool (void)> inotifyMgrCall;
};

BaseEvnStrategy::BaseEvnStrategy(const string &strategyName):StrategyBaseClass (strategyName)
{
    m_baseData = boost::make_shared<BaseEvnStrategyPrivate>();

    monitor_strategy_modif(boost::bind(&BaseEvnStrategy::strategy_update,this,_1));

    parse_all_designated_data();
}

bool BaseEvnStrategy::strategy_update(inotify_event *event)
{
    switch (event->mask)
    {

    case IN_MODIFY:
        //parse();
        //break;
    case IN_CREATE:
        //parse();
        //break;
    case IN_DELETE:
        parse();
        break;
    default:
        break;
    }

    return  true;
}

BaseEvnStyInfoMap BaseEvnStrategy::new_strategy_data()
{
    decltype (m_baseData->addStrategyDataMap) tmp;
    tmp.swap(m_baseData->addStrategyDataMap);
    return tmp;
}

BaseEvnStyInfoMap BaseEvnStrategy::del_strategy_data()
{
    decltype (m_baseData->delStrategyDataMap) tmp;
    tmp.swap(m_baseData->delStrategyDataMap);
    return tmp;
}

BaseEvnStyInfoMap BaseEvnStrategy::mod_strategy_data()
{
    decltype (m_baseData->modStrategyDataMap) tmp;
    tmp.swap(m_baseData->modStrategyDataMap);
    return tmp;
}

bool BaseEvnStrategy::reg_callback(boost::function<bool ()> fun)
{
    m_baseData->inotifyMgrCall = fun;
    return true;
}

//int BaseEvnStrategy::system_interval()
//{
//    return m_baseData->systemInt;
//}

//tribool BaseEvnStrategy::is_system_update()
//{
//    return m_baseData->systemTri;
//}



bool BaseEvnStrategy::parse()
{
    if(!this->usable())
        return false;
    //清空所有数据，用来保存此次读取的最新数据
    m_baseData->allStrategyDataMap.clear();

    parse_all_designated_data();



    //解释完成，通知基础环境管理类进行处理
    m_baseData->inotifyMgrCall();
    return true;
}

bool BaseEvnStrategy::parse_designated_data(const string &name)
{
    string cmd = "Policy."+ name + ".<xmlattr>.interval";

    int data = get_stragety_ptree().get<int>(cmd,-1);
    auto iter = m_baseData->allStrategyDataMap.find("name");
    if( iter != m_baseData->allStrategyDataMap.end())
    {
        //存在于原先链表中，并且值不相等，即为策略变更后的数据
        if(iter->second != data && data >0)
        {
            m_baseData->modStrategyDataMap[name] = data;

            m_baseData->allStrategyDataMap[name] = data;
        }
        else
        {
            //新数据小于0，删除该数据，停止该功能运行
            m_baseData->delStrategyDataMap[name] = data;

            m_baseData->allStrategyDataMap.erase(iter);
        }
    }
    else if(data > 0)
    {
        //在原先的链表中不存在，则为新加数据
        m_baseData->addStrategyDataMap[name] = data;

        m_baseData->allStrategyDataMap[name] = data;
    }
    else
    {
        LogError<<"读取 基础环境审计策略 ： " + strategy_name() + "中数据" + name +"失败"<< endl;
    }
    return true;
}

bool BaseEvnStrategy::parse_all_designated_data()
{
    parse_designated_data("System");
    parse_designated_data("PortInfo");
    parse_designated_data("AccountInfo");
    parse_designated_data("FirefoxInfo");
    parse_designated_data("NetFlowInfo");
    parse_designated_data("ProcessInfo");
    parse_designated_data("SoftwareInfo");

    return true;
}

