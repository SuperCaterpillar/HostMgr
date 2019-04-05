#include "BaseEnv.h"
#include "BaseEnvMgr.h"
#include "../PublicFun/Logging.h"


bool StopBaseEnv()
{
    bool bRet = BaseEnvMgr::instance()->stop();
    if(bRet)
    {
        LogInfo<< "StopBaseEnv success " << endl;
    }
    else
    {
        LogInfo<< "StopBaseEnv faild " << endl;
    }
    return bRet;
}

bool StartBaseEnv()
{
    bool bRet = BaseEnvMgr::instance()->start();
    if(bRet)
    {
        LogInfo<< "StartBaseEnv success " << endl;
    }
    else
    {
        LogInfo<< "StartBaseEnv faild " << endl;
    }
    return bRet;
}
