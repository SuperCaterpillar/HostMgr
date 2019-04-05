
#include <sqlite3.h>
#include <string.h>
#include <json/json.h>

#include <../PublicFun/DateDispose.h>

#include "FirefoxInfo.h"

static string g_strFiefoxPath = "";         //火狐保存信息路径
static long long g_llFirefoxHistory = 0;    //标识上次上报火狐浏览器历史记录的时间
static long long g_llFirefoxDownLoad = 0;   //标识上次上报火狐浏览器下载文件的时间
static bool bFirstGetDate = true;           //标识是否是首次获取浏览器数据




FirefoxInfo::FirefoxInfo(int runInterval):interval(runInterval)
{
    timer.connect(boost::bind(&FirefoxInfo::run,this));
}

bool FirefoxInfo::start()
{
    timer.start(interval);
    return true;
}

bool FirefoxInfo::stop()
{
    timer.stop();
    return true;
}

bool FirefoxInfo::report_event()
{
    return true;
}

bool FirefoxInfo::strategy_update(int t)
{
    timer.reset_timer(t);
    return true;
}

DECL_SHARED_PTR(FirefoxInfo)
FirefoxInfoSharedPtr FirefoxInfo::creater(int runInterval)
{
    return  boost::make_shared<FirefoxInfo>(runInterval);
}

void FirefoxInfo::run()
{
    if(!firefox_path())
    {
        return;
    }
    stuFirefoxInfoVet vecFirefoxHistory;
    stuFirefoxInfoVet vecFirefoxDownload;
    stuFirefoxInfoVet vecFirefoxPlugin;
    firefox_history(vecFirefoxHistory);
    firefox_plugin(vecFirefoxPlugin);
    firefox_extensions(vecFirefoxPlugin);
    firefox_download(vecFirefoxDownload);
    if(!vecFirefoxPlugin.empty() || !vecFirefoxHistory.empty() || !vecFirefoxDownload.empty())
    {
        build_info(vecFirefoxHistory, vecFirefoxDownload, vecFirefoxPlugin);
    }
}

bool FirefoxInfo::firefox_path()
{
    g_strFiefoxPath = "";
    PublicFun::SystemEx("find /root -type d -name '*.default' | grep '/.mozilla/firefox/'", g_strFiefoxPath);

    if(g_strFiefoxPath.empty())
    {
        PublicFun::SystemEx("find /home -type d -name '*.default' | grep '/.mozilla/firefox/'", g_strFiefoxPath);
    }
    if(g_strFiefoxPath.empty())
    {
        LogInfo<<("firefox_path 没有安装火狐浏览器线程直接退出");

        return false;
    }

    LogInfo<<"firefox_path 火狐保存信息路径: "<< g_strFiefoxPath <<endl;

    return true;
}

bool FirefoxInfo::firefox_download(stuFirefoxInfoVet &vecFirefoxDownload)
{
    stuFirefoxInfo stuTmpFirefoxInfo;
    // 获取火狐浏览器的历史记录
    std::string strDestPath = g_strFiefoxPath;

    strDestPath += "/places.sqlite";

    if(access(strDestPath.c_str(),F_OK) != -1)
    {
        sqlite3 *db;
        //打开数据库
        int ret = sqlite3_open(strDestPath.c_str(), &db);
        if(ret == SQLITE_OK)
        {
            //std::string sql = "select url,title,last_visit_date from moz_places where id in (select place_id from moz_annos where anno_attribute_id = (select id from moz_anno_attributes where name = 'downloads/destinationFileURI'))";
            std::string sql = "select url,title,last_visit_date from moz_places where id in (select place_id from moz_annos where anno_attribute_id = (select id from moz_anno_attributes where name = 'downloads/destinationFileURI')) and last_visit_date > ";
            char szDownLoadTime[256] = {0};
            snprintf(szDownLoadTime, sizeof(szDownLoadTime), "%lld", g_llFirefoxDownLoad);
            sql += szDownLoadTime;

            LogInfo<<"GetFirefoxDownload SQL= " << sql <<endl;

            int nrow;
            int ncolum;
            int i;
            char *errmsg;
            char **aresult;

            //查询表中的数据
            if(SQLITE_OK == sqlite3_get_table(db, sql.c_str(), &aresult, &nrow, &ncolum, &errmsg))
            {
                int nLine = 0;
                long long llFirefoxDownLoad = 0;
                for(i = 0; i < (nrow + 1)*ncolum; i++)
                {
                    if(i >= 3)
                    {
                        nLine++;
                        if(nLine == 1)
                        {
                            //url
                            if(aresult[i] != nullptr)
                            {
                                stuTmpFirefoxInfo.strUrl = aresult[i];
                            }
                        }
                        else if(nLine == 2)
                        {
                            //title
                            if(aresult[i] != nullptr)
                            {
                                stuTmpFirefoxInfo.strTitle = aresult[i];
                            }
                        }
                        else if(nLine == 3)
                        {
                            //date
                            long long lTime = 0;
                            if(aresult[i] != nullptr)
                            {
                                lTime = atoll(aresult[i]);
                                stuTmpFirefoxInfo.strTime = DateDispose::firefox_history_time(lTime);
                            }

                            if(lTime > g_llFirefoxDownLoad)
                            {
                                //保存最新的浏览器历史记录时间
                                llFirefoxDownLoad = lTime;
                            }

                            vecFirefoxDownload.push_back(stuTmpFirefoxInfo);
                            nLine = 0;
                        }
                    }
                }

                if(g_llFirefoxDownLoad < llFirefoxDownLoad)
                {
                    g_llFirefoxDownLoad = llFirefoxDownLoad;
                }

                sqlite3_free_table(aresult);//释放内存空间
            }
            else
            {
                LogInfo << "GetFirefoxDownload 查询数据库表失败, error= "
                        << strerror(errno) <<endl;
            }

            //关闭数据库
            sqlite3_close(db);
        }
        else
        {
            LogInfo << "GetFirefoxDownload 打开数据库places.sqlite失败, error = "
                    << strerror(errno) <<endl;
        }
    }

    return true;
}

bool FirefoxInfo::firefox_history(stuFirefoxInfoVet &vecFirefoxHistory)
{
    stuFirefoxInfo stuTmpFirefoxInfo;
    // 获取火狐浏览器的历史记录
    std::string strDestPath = g_strFiefoxPath;

    strDestPath += "/places.sqlite";

    if(access(strDestPath.c_str(),F_OK) != -1)
    {
        sqlite3 *db;
        //打开数据库
        int ret = sqlite3_open(strDestPath.c_str(), &db);
        if(ret == SQLITE_OK)
        {
            if(bFirstGetDate)
            {
                bFirstGetDate = false;
                //获取最后一条的数据的时间戳
                std::string sql = "select last_visit_date from moz_places ORDER BY last_visit_date DESC";

                int nrow;
                int ncolum;
                int i;
                char *errmsg;
                char **aresult;

                //查询表中的数据,前面一行为字段名
                if(SQLITE_OK == sqlite3_get_table(db, sql.c_str(), &aresult, &nrow, &ncolum, &errmsg))
                {
                    //int nLine = 0;
                    for(i = 0; i < (nrow + 1)*ncolum; i++)
                    {
                        if((aresult[i] !=nullptr) && (i !=0))
                        {
                            //date
                            long long lTime = 0;
                            if(aresult[i] != nullptr)
                            {
                                lTime = atoll(aresult[i]);
                            }

                            g_llFirefoxHistory = lTime;
                            g_llFirefoxDownLoad = lTime;

                            LogInfo <<"GetFirefoxHistory 浏览器策略首次执行，\
                                      当前浏览器信息时间戳【" << g_llFirefoxHistory
                                   << "】" << endl;;

                            break;
                        }
                    }

                    //释放内存空间
                    sqlite3_free_table(aresult);

                    //关闭数据库
                    sqlite3_close(db);

                    return true;
                }
                else
                {
                    LogError<< "GetFirefoxHistory 查询表中的数据失败, error= "
                            << strerror(errno) << endl;
                }
            }

            //sqlite3 /root/.mozilla/firefox/rso5nvck.default/places.sqlite
            //select url,title,last_visit_date from moz_places
            std::string sql = "select url,title,last_visit_date from moz_places where last_visit_date > ";
            char szHistoryTime[256] = {0};
            snprintf(szHistoryTime, sizeof(szHistoryTime), "%lld", g_llFirefoxHistory);
            sql += szHistoryTime;

            LogInfo << "GetFirefoxHistory SQL= " << sql.c_str() << endl;

            int nrow;
            int ncolum;
            int i;
            char *errmsg;
            char **aresult;
            long long llFirefoxHistory = 0;
            //查询表中的数据,前面一行为字段名
            if(SQLITE_OK == sqlite3_get_table(db, sql.c_str(), &aresult, &nrow, &ncolum, &errmsg))
            {
                int nLine = 0;
                for(i = 0; i < (nrow + 1)*ncolum; i++)
                {
                    if(i >= 3)
                    {
                        nLine++;
                        if(nLine == 1)
                        {
                            //url
                            if(aresult[i] != nullptr)
                            {
                                stuTmpFirefoxInfo.strUrl = aresult[i];
                            }
                        }
                        else if(nLine == 2)
                        {
                            //title
                            if(aresult[i] != nullptr)
                            {
                                stuTmpFirefoxInfo.strTitle = aresult[i];
                            }
                        }
                        else if(nLine == 3)
                        {
                            //date
                            long long lTime = 0;
                            if(aresult[i] != nullptr)
                            {
                                lTime = atoll(aresult[i]);
                                stuTmpFirefoxInfo.strTime = DateDispose::firefox_history_time(lTime);
                            }

                            if(lTime > llFirefoxHistory)
                            {
                                //保存最新的浏览器历史记录时间
                                llFirefoxHistory = lTime;
                            }

                            vecFirefoxHistory.push_back(stuTmpFirefoxInfo);
                            nLine = 0;
                        }
                    }
                }

                if(g_llFirefoxHistory < llFirefoxHistory)
                {
                    g_llFirefoxHistory = llFirefoxHistory;
                }

                //释放内存空间
                sqlite3_free_table(aresult);
            }
            else
            {
                LogError<< "GetFirefoxHistory 查询表中的数据失败, error= "<< string(strerror(errno)) << endl;;
            }

            //关闭数据库
            sqlite3_close(db);
        }
        else
        {
            LogError << "GetFirefoxHistory 打开数据库places.sqlite失败, error= "
                     << strerror(errno) << endl;
        }
    }

    return true;
}

bool FirefoxInfo::firefox_plugin(stuFirefoxInfoVet &vecFirefoxPlugin)
{
    stuFirefoxInfo stuTmpFirefoxInfo;
    bool bPluginStart = false;  //标识读取的列是否是插件信息
    int nPluginLineCnt = 1;     //标识读取的某一个插件列表，每开始一个插件的数据，该值为1
    int nPluginDescCnt = 1;     //标识插件描述个数

    //火狐浏览器插件信息保存路径
    std::string strDestPath = g_strFiefoxPath + "/pluginreg.dat";

    std::fstream streamInPut(strDestPath.c_str(), ios_base::in);
    if (streamInPut.is_open())
    {
        std::string strLine = "";

        //读取文件中的每一行
        while(std::getline(streamInPut, strLine))
        {
            if(strLine.compare("[PLUGINS]") == 0)
            {
                bPluginStart = true;
                continue;
            }
            else if(strLine.compare("[INVALID]") == 0)
            {
                break;
            }

            if(bPluginStart)
            {
                //截取浏览器插件信息，第四行为时间，第六行为插件名，每个插件以其动态库名称开头，第7行标识该插件描述的个数
                if((nPluginLineCnt - nPluginDescCnt) == 7)
                {
                    //下一插件开始，需初始化所有数据,并保存上一条数据
                    nPluginLineCnt = 0;
                    nPluginDescCnt = 1;

                    vecFirefoxPlugin.push_back(stuTmpFirefoxInfo);
                }
                if(nPluginLineCnt == 4)
                {
                    //截取插件时间
                    long long lTime = atoll(strLine.substr(0, strLine.find("|")).c_str());
                    stuTmpFirefoxInfo.strTime = DateDispose::firefox_history_time(lTime);
                }
                else if(nPluginLineCnt == 6)
                {
                    //截取插件名
                    stuTmpFirefoxInfo.strTitle = strLine.substr(0, strLine.find("|$"));
                }
                else if(nPluginLineCnt == 7)
                {
                    //获取插件描述个数
                    nPluginDescCnt = atoi(strLine.c_str());
                }
                nPluginLineCnt++;
            }
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

bool FirefoxInfo::firefox_extensions(stuFirefoxInfoVet &vecFirefoxExtensions)
{
    //扩展保存路径
    std::string strExtensionsPath = g_strFiefoxPath + "/extensions.json";

    std::ifstream file;
    file.open(strExtensionsPath.c_str(), std::ios::binary);

    Json::Reader reader;
    Json::Value root;
    if (!reader.parse(file, root, false))
    {
        LogError << ("reader.parse failed!") << endl;
        return false;
    }

    stuFirefoxInfo stuFirefoxTemp;
    for (int nIndex = 0; nIndex < root["addons"].size(); nIndex++)
    {
        std::string strSxtensionsType = root["addons"][nIndex]["type"].asString();
        if(strSxtensionsType == "webextension")
        {
            Json::Value valDefaultLocale = root["addons"][nIndex]["defaultLocale"];
            std::string strName = valDefaultLocale["name"].asString();

            std::string strUpdateDate = root["addons"][nIndex]["updateDate"].asString();
            stuFirefoxTemp.strTime =DateDispose::firefox_history_time(atoll(strUpdateDate.c_str()));
            stuFirefoxTemp.strTitle = strName;

            vecFirefoxExtensions.push_back(stuFirefoxTemp);
        }
    }

    return true;
}

bool FirefoxInfo::build_info(stuFirefoxInfoVet &vecFirefoxHistory, stuFirefoxInfoVet &vecFirefoxDownload, stuFirefoxInfoVet &vecFirefoxPlugin)
{
    ptree::ptree root, history, download, plugin;
    root.add("type.<xmlattr>.name","FirefoxInfo");
    history.add("<xmlattr>.name","history");
    for (auto iter = vecFirefoxHistory.begin(); iter != vecFirefoxHistory.end();iter++)
    {
        ptree::ptree child;
        child.add("URL",iter->strUrl);
        child.add("Titile",iter->strTitle);
        child.add("Time",iter->strTime);
        history.add_child("Result",child);
    }
    root.add_child("type.Results",history);

    download.add("<xmlattr>.name","download");
    for (auto iter = vecFirefoxDownload.begin(); iter != vecFirefoxDownload.end();iter++)
    {
        ptree::ptree child;
        child.add("URL",iter->strUrl);
        child.add("Titile",iter->strTitle);
        child.add("Time",iter->strTime);
        download.add_child("Result",child);
    }
    root.add_child("type.Results",download);

    plugin.add("<xmlattr>.name","plugin");
    for (auto iter = vecFirefoxPlugin.begin(); iter != vecFirefoxPlugin.end();iter++)
    {
        ptree::ptree child;
        child.add("URL",iter->strUrl);
        child.add("Titile",iter->strTitle);
        child.add("Time",iter->strTime);
        plugin.add_child("Result",child);
    }
    root.add_child("type.Results",plugin);

    SendDataToService::SendDataToService(root);
    return true;
}
