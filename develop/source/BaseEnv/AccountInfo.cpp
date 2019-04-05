#include "../PublicFun/PublicFun.h"
#include "../PublicFun/SendDataToService.h"

#include "AccountInfo.h"


AccountInfo::AccountInfo(int runInterval):interval(runInterval)
{
    timer.connect(boost::bind(&AccountInfo::run,this));
}

bool AccountInfo::start()
{
    timer.start(interval);
    return true;
}

bool AccountInfo::stop()
{
    timer.stop();
    return true;
}

bool AccountInfo::report_event()
{
    return true;
}

bool AccountInfo::strategy_update(int t)
{
    timer.reset_timer(t);
    return true;
}

DECL_SHARED_PTR(AccountInfo)
AccountInfoSharedPtr AccountInfo::creater(int runInterval)
{
    return  boost::make_shared<AccountInfo>(runInterval);
}

void AccountInfo::run()
{
    std::vector<stuCurrentAccount> vecCurrentAccount;
    current_account(vecCurrentAccount);

    //获取所有的帐户信息
    std::vector<stuAccount> vecAccount;
    all_account(vecAccount);


    boost::property_tree::ptree root,current,all;
    root.add("type.<xmlattr>.name","AccouuntInfo");
    //遍历当前登陆账户信息
    current.add("<xmlattr>.name","CURRENTACCOUNT");
    for(size_t nIndex = 0; nIndex < vecCurrentAccount.size(); nIndex++)
    {
        ptree::ptree child;
        child.add("Name", vecCurrentAccount[nIndex].strUserName);     //当前登陆账户名称
        child.add("Method", vecCurrentAccount[nIndex].strMethod);         //当前账户登陆方式
        child.add("LoginTime", vecCurrentAccount[nIndex].strLoginTime);   //当前账户登陆时间
        current.add_child("Result",child);
    }
    root.add_child("type.Results",current);

    all.add("<xmlattr>.name", "ALLACOUNT");
    //遍历所有帐户信息
    for(size_t nIndex = 0; nIndex < vecAccount.size(); nIndex++)
    {
        ptree::ptree child;
        child.put("Name", vecAccount[nIndex].strUserName);    //帐户名称
        child.put("UID", vecAccount[nIndex].strUID);        //UID
        child.put("GID", vecAccount[nIndex].strGID);        //GID
        child.put("Description", vecAccount[nIndex].strDesc);    //帐户描述
        child.put("MainDirectory", vecAccount[nIndex].strMainDir);   //帐户主目录
        child.put("LoginShell", vecAccount[nIndex].strLoginShell);  //帐户默认shell
        all.add_child("Result",child);
    }
    root.add_child("type.Results",all);
    SendDataToService::SendDataToService(root);
}

bool AccountInfo::current_account(stuCurrentAccountVet &vec)
{
    std::string strCurrentRet = "";
    std::vector<std::string> vecCurrentRet;
    char szTmp[4][256] = {{0}};
    stuCurrentAccount stuCurAccountTemp;

    PublicFun::SystemEx("who",vecCurrentRet);

    for(size_t nIndex = 0; nIndex < vecCurrentRet.size(); nIndex++)
    {
        sscanf(vecCurrentRet[nIndex].c_str(), "%[^' '] %[^' '] %[^' '] %[^' ']", szTmp[0], szTmp[1], szTmp[2], szTmp[3]);

        std::string strTime = szTmp[2];
        strTime += " ";
        strTime += szTmp[3];
        stuCurAccountTemp.strUserName = szTmp[0];
        stuCurAccountTemp.strMethod = szTmp[1];
        stuCurAccountTemp.strLoginTime = strTime;

        vec.push_back(stuCurAccountTemp);
    }
    return true;
}

bool AccountInfo::all_account(stuAccountVet &vec)
{
    char szFilePath[256] = {0};
    stuAccount stuTmpAccount;
    char szTmp[6][256] = {{0}};
    //帐户信息存放路径
    snprintf(szFilePath, sizeof(szFilePath), "/etc/passwd");

    std::ifstream streamInPut(szFilePath);
    if (streamInPut.is_open())
    {
        std::string strLine = "";

        //读取文件中的每一行
        while(std::getline(streamInPut, strLine))
        {
            //memset(&stuTmpAccount, 0, sizeof(stuTmpAccount));

            sscanf(strLine.c_str(), "%[^:]:%*[^:]:%[^:]:%[^:]:%[^:]:%[^:]:%[^:]",
                   szTmp[0], szTmp[1], szTmp[2], szTmp[3], szTmp[4], szTmp[5]);

            stuTmpAccount.strUserName = szTmp[0];
            stuTmpAccount.strUID = szTmp[1];
            stuTmpAccount.strGID = szTmp[2];
            stuTmpAccount.strDesc = szTmp[3];
            stuTmpAccount.strMainDir = szTmp[4];
            stuTmpAccount.strLoginShell = szTmp[5];

            vec.push_back(stuTmpAccount);

            strLine.clear();
        }

        streamInPut.close();
    }
    else
    {
        return false;
    }

    return true;
}
