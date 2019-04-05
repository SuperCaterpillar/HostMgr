#ifndef ACCOUNTINFO_H
#define ACCOUNTINFO_H


#include "../PublicFun/Publicdefine.h"

#include "BaseEvnClass.h"

//定义当前登录帐户结构体
struct stuCurrentAccount{
    string strUserName;      //用户名
    string strMethod;        //帐户登录方式
    string strLoginTime;     //帐户登录时间
};

//定义帐户结构体
struct stuAccount
{
    string strUserName;    //用户名
    string strUID;         //UID
    string strGID;         //GID
    string strDesc;        //帐户描述
    string strMainDir;     //帐户主目录
    string strLoginShell;  //帐户默认Shell
};

DECL_VET(stuAccount)
DECL_VET(stuCurrentAccount)

class AccountInfo : public BaseEvnClass
{
public:
    AccountInfo(int interval);
    virtual bool start();
    virtual bool stop();

    //上报日志
    virtual bool report_event();

    //策略更新事件
    virtual bool strategy_update(int);

    DECL_SHARED_PTR(AccountInfo)
    static AccountInfoSharedPtr creater(int runInterval);
private:
    //运行具体功能
    void run();

    //获取当前登入账户信息
    bool current_account(stuCurrentAccountVet &vec);
    //获取当前所有账户信息
    bool all_account(stuAccountVet &vec);
    //上报日志


    int interval;
};

#endif // ACCOUNTINFO_H
