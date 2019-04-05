#ifndef SENDDATATOSERVICE_H
#define SENDDATATOSERVICE_H
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>
#include "Publicdefine.h"

namespace SendDataToService
{
    //发送数据到服务端
    bool SendDataToService(const string &msg);
    bool SendDataToService(const boost::property_tree::ptree &);


};

#endif // SENDDATATOSERVICE_H
