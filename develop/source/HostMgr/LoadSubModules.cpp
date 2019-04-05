#include <dlfcn.h>
#include <sys/ipc.h>

#include <boost/foreach.hpp>
#include <boost/property_tree/ptree.hpp>
#include <boost/property_tree/xml_parser.hpp>

#include "LoadSubModules.h"
#include "../PublicFun/Logging.h"


namespace ptree = boost::property_tree;

LoadSubModules::LoadSubModules()
{
    try {
        ptree::ptree ptreeModules;
        ptree::read_xml(HSM_MODULES_FILE,ptreeModules);
        //获取modules的所有子节点
        auto moduleList = ptreeModules.get_child("modules");

        BOOST_FOREACH(ptree::ptree::value_type &v, moduleList)
        {
            ModulesInfo info;
            info.strModuleName = v.second.get<string>("<xmlattr>.name");
            info.strModulePath = v.second.get<string>("<xmlattr>.path");
            if(info.strModulePath.empty())
                info.strModulePath = "./";
            m_modulesInfoVet.push_back(info);
        }
    } catch (std::exception& e) {
        LogError<< (string("读取文件失败") + e.what()) << HSM_MODULES_FILE;
        return ;
    }
    LoadAllModules();
}

LoadSubModules::~LoadSubModules()
{

}

bool LoadSubModules::LoadAllModules()
{
    for(size_t i = 0; i < m_modulesInfoVet.size(); i++)
    {

        void* pError = nullptr;
        void* pModuleHandle	= nullptr;
        void* pStartFunHandle	= nullptr;
        void* pStopFunHandle	= nullptr;
        //StartBaseEnvironmentAudit

        string moduleName = m_modulesInfoVet[i].strModuleName;
        string tmpName = moduleName.substr(3,moduleName.rfind(".")-3);

        LogInfo<<(string("HsmAgent@LoadLibrary Mod=%s ") + moduleName);
        std::string strStartFunName = "Start" + tmpName;
        std::string strStopFunName = "Stop" + tmpName;



        pModuleHandle =
                dlopen((m_modulesInfoVet[i].strModulePath + m_modulesInfoVet[i].strModuleName).c_str(), RTLD_NOW);
        pError = dlerror();
        if ( pError || (nullptr == pModuleHandle) )
        {
            LogError<<(string("load library failed name = " ) + moduleName)
                   <<" errorcode = " <<static_cast<char*>(pError);
            continue;
        }

        pStartFunHandle = dlsym(pModuleHandle,strStartFunName.c_str());
        pError = dlerror();
        if (pError || (nullptr == pStartFunHandle))
        {
            LogError<<(strStartFunName)<<"  errorcode:"<<static_cast<char*>(pError);
            continue;
        }

        pStopFunHandle = dlsym(pModuleHandle,strStopFunName.c_str());
        pError = dlerror();
        if (pError || (nullptr == pStopFunHandle)){
            LogError<<(strStopFunName)<<"errorcode:"<<static_cast<char*>(pError);
            continue;
        }

        m_modulesInfoVet[i].bState = false;
        m_modulesInfoVet[i].pModuleHandle = pModuleHandle;
        m_modulesInfoVet[i].pStartFunHandle = pStartFunHandle;
        m_modulesInfoVet[i].pStopFunHandle = pStopFunHandle;

    }
    return true;
}

bool LoadSubModules::StartAllModules()
{
    typedef bool (*StartFunHandle)();

    for(auto it = m_modulesInfoVet.begin(); it < m_modulesInfoVet.end(); it++)
    {
        //(static_cast<StartFunHandle>(it->pStartFunHandle))();
        if((it->pStartFunHandle) && (!it->bState))
        {
            if(((StartFunHandle)(it->pStartFunHandle))())
            {
                it->bState = true;
                LogInfo<<(it->strModuleName + string("模块以启动"));
            }
            else
            {
                LogInfo<<(it->strModuleName + string("模块以启动失败"));
            }
        }
    }
    return true;
}

bool LoadSubModules::StopAllModules()
{
    typedef bool (*StopFunHandle)();

    for(auto it = m_modulesInfoVet.begin(); it < m_modulesInfoVet.end(); it++)
    {
        //(static_cast<StartFunHandle>(it->pStartFunHandle))();
        if((it->pStopFunHandle) && (it->bState))
        {
            if(((StopFunHandle)(it->pStopFunHandle))())
            {
                it->bState = false;
                LogInfo<<(it->strModuleName + string("模块以停止"));
            }
            else
            {
                LogInfo<<(it->strModuleName + string("模块停止失败"));
            }
        }
    }
    return true;
}

bool LoadSubModules::UnloadAllModules()
{
    StopAllModules();
    for(auto it = m_modulesInfoVet.begin(); it < m_modulesInfoVet.end();it++)
    {
        dlclose(it->pModuleHandle);
        it->pModuleHandle = nullptr;
    }
    return true;
}
