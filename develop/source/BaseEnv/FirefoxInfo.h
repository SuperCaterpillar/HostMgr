#ifndef FIREFOXINFO_H
#define FIREFOXINFO_H
#include "../PublicFun/Publicdefine.h"
#include "BaseEvnClass.h"

//定义浏览器信息结构体
struct stuFirefoxInfo{
    std::string strUrl;     //URL
    std::string strTitle;   //下载文件名、插件名
    std::string strTime;    //浏览时间、下载时间、上传时间或插件安装时间
};
DECL_VET(stuFirefoxInfo)

class FirefoxInfo : public BaseEvnClass
{
                               public:
                               FirefoxInfo(int runInterval);
virtual bool start();
virtual bool stop();

//上报日志
virtual bool report_event();

//策略更新事件
virtual bool strategy_update(int);

DECL_SHARED_PTR(FirefoxInfo)
static FirefoxInfoSharedPtr creater(int runInterval);
private:
//运行具体功能
void run();

//获取浏览器信息路径
bool firefox_path();

//获取火狐浏览器下载或上传文件记录
bool firefox_download(stuFirefoxInfoVet &vecFirefoxDownload);

//获取火狐浏览器历史访问记录
bool firefox_history(stuFirefoxInfoVet &vecFirefoxHistory);

//获取火狐浏览器插件记录
bool firefox_plugin(stuFirefoxInfoVet &vecFirefoxPlugin);

//获取浏览器扩展记录
bool firefox_extensions(stuFirefoxInfoVet &vecFirefoxExtensions);

//浏览器信息查询日志
bool build_info(stuFirefoxInfoVet&vecFirefoxHistory,
                  stuFirefoxInfoVet&vecFirefoxDownload,
                  stuFirefoxInfoVet&vecFirefoxPlugin);

private:
int interval;
};

#endif // FIREFOXINFO_H
