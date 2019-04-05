#ifndef BASEEVNCLASS_H
#define BASEEVNCLASS_H



#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "../PublicFun/SendDataToService.h"
#include "../PublicFun/Publicdefine.h"
#include "../PublicFun/kernelmacro.h"
#include "../PublicFun/PublicFun.h"
#include "../PublicFun/Timer.h"
namespace ptree = boost::property_tree;

class BaseEvnClass
{
public:
    BaseEvnClass();
    virtual ~BaseEvnClass();
    virtual bool start() = 0;
    virtual bool stop() = 0;
    //解析策略
    //virtual bool parse_strategy(const string &);
    //上报日志
    virtual bool report_event() = 0;

    //策略更新事件
    virtual bool strategy_update(int) = 0;
protected:
    Timer timer;
};

#endif // BASEEVNCLASS_H
