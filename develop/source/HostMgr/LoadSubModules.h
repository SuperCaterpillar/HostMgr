#ifndef LOADSUBMODULES_H
#define LOADSUBMODULES_H

#include "../PublicFun/Publicdefine.h"


struct ModulesInfo
{
    ModulesInfo():bState(false),
        pStartFunHandle(nullptr),
        pStopFunHandle(nullptr),
        pModuleHandle(nullptr){}

    bool	bState;
    void*	pStartFunHandle;
    void*	pStopFunHandle;
    void*	pModuleHandle;
    string	strModuleName;
    string  strModulePath;
};

DECL_VET(ModulesInfo)
class LoadSubModules
{
public:
    LoadSubModules();
    ~LoadSubModules();

    bool LoadAllModules();
    bool StartAllModules();
    bool StopAllModules();
    bool UnloadAllModules();
private:
    ModulesInfoVet m_modulesInfoVet;
};
#endif // LOADSUBMODULES_H
