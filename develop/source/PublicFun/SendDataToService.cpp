#include<fstream>

#include "SendDataToService.h"



bool SendDataToService::SendDataToService(const string &msg)
{
    cout<< msg <<endl;
    return  true;
}

bool SendDataToService::SendDataToService(const property_tree::ptree &report)
{
    ostringstream os;
    auto settings = boost::property_tree::xml_writer_make_settings<std::string>('\t', 1);
    property_tree::write_xml(os,report,settings);

    cout<< os.str() << endl;
    return  true;
}
