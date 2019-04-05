#ifndef BASEENVMGR_H
#define BASEENVMGR_H

#include "BaseEvnClass.h"
#include "BaseEvnStrategy.h"

#include "../PublicFun/Singleton.h"
#include "../PublicFun/Publicdefine.h"

DECL_PTR_MAP(string,BaseEvnClass);

class BaseEnvMgr : public Singleton<BaseEnvMgr>
{
public:
    bool start();
    bool stop();

    bool update_base_list();

private :
    BaseEnvMgr();
    //不传递参数将删除所有模块
    void del_run_modules(const string & naem ="");
private:

    friend Singleton<BaseEnvMgr>;
    stringBaseEvnClassPtrMap base_map;

    BaseEvnStrategy strategy;
};

#endif // BASEENVMGR_H
