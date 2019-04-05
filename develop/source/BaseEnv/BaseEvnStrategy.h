#ifndef BASEEVNSTRATEGY_H
#define BASEEVNSTRATEGY_H

#include "../PublicFun/StrategyBaseClass.h"
#include "../PublicFun/Publicdefine.h"


/*
 * 需要改进的问题
 * 1、提供一个策略框架，在创建一个具体类时，自动解析对应的策略
 *
*/

DECL_CLASS_PRIVATE_DATA(BaseEvnStrategy);

typedef map<string,int> BaseEvnStyInfoMap;

class BaseEvnStrategy : public StrategyBaseClass
{
public:
    BaseEvnStrategy(const string& strategyName);
    bool strategy_update(struct inotify_event *);

    BaseEvnStyInfoMap new_strategy_data();
    BaseEvnStyInfoMap del_strategy_data();
    BaseEvnStyInfoMap mod_strategy_data();
    //获取策略中的属性
    //int system_interval();
    //true 更新 false没有更新 indeterminate 没有配置
    //tribool is_system_update();
    bool reg_callback(boost::function<bool(void)> );
protected:
    bool parse();
private:
    bool parse_designated_data(const string &name);
    bool parse_all_designated_data();
private:
    BaseEvnStrategyPrivatePtr m_baseData;

};

#endif // BASEEVNSTRATEGY_H
