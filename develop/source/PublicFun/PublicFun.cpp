#include <sys/inotify.h>
#include <limits.h>
#include <sys/types.h>  /* Type definitions used by many programs */
#include <stdio.h>      /* Standard I/O functions */
#include <stdlib.h>     /* Prototypes of commonly used library functions,*/
#include <unistd.h>     /* Prototypes for many system calls */
#include <errno.h>      /* Declares errno and defines error constants */
#include <string.h>     /* Commonly used string-handling functions */

#include <iostream>


#include "PublicFun.h"


using namespace std;


//static void displayInotifyEvent(struct inotify_event *i)
//{
//    printf("    wd =%2d; ", i->wd);
//    if (i->cookie > 0)
//        printf("cookie =%4d; ", i->cookie);

//    printf("mask = ");
//    if (i->mask & IN_ACCESS)        printf("IN_ACCESS ");
//    if (i->mask & IN_ATTRIB)        printf("IN_ATTRIB ");
//    if (i->mask & IN_CLOSE_NOWRITE) printf("IN_CLOSE_NOWRITE ");
//    if (i->mask & IN_CLOSE_WRITE)   printf("IN_CLOSE_WRITE ");
//    if (i->mask & IN_CREATE)        printf("IN_CREATE ");
//    if (i->mask & IN_DELETE)        printf("IN_DELETE ");
//    if (i->mask & IN_DELETE_SELF)   printf("IN_DELETE_SELF ");
//    if (i->mask & IN_IGNORED)       printf("IN_IGNORED ");
//    if (i->mask & IN_ISDIR)         printf("IN_ISDIR ");
//    if (i->mask & IN_MODIFY)        printf("IN_MODIFY ");
//    if (i->mask & IN_MOVE_SELF)     printf("IN_MOVE_SELF ");
//    if (i->mask & IN_MOVED_FROM)    printf("IN_MOVED_FROM ");
//    if (i->mask & IN_MOVED_TO)      printf("IN_MOVED_TO ");
//    if (i->mask & IN_OPEN)          printf("IN_OPEN ");
//    if (i->mask & IN_Q_OVERFLOW)    printf("IN_Q_OVERFLOW ");
//    if (i->mask & IN_UNMOUNT)       printf("IN_UNMOUNT ");
//    printf("\n");

//    if (i->len > 0)
//        printf("        name = %s\n", i->name);
//}

#define NAME_MAX 255
#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

static void reset_inotify_add_watch(int fd, stringVet path, uint32_t mask)
{
    for (size_t i = 0; i < path.size(); i++) {
        int wd = inotify_add_watch(fd, path[i].c_str(), mask);
        if (wd == -1)
        {
            LogError << ("添加监控文件失败，文件名 ：" + path[i] +"监控动作： "
                         + boost::lexical_cast<string>(mask));
        }
    }
}
bool PublicFun::monitor_file_system(uint32_t mask, const stringVet & path, file_sys_monitor_call fun)
{
    char *p;
    int inotifyFd;
    char buf[BUF_LEN];

    struct inotify_event *event;

    inotifyFd = inotify_init();
    if (inotifyFd == -1)
        return false;

    reset_inotify_add_watch(inotifyFd, path, mask);

    while(1)
    {
        ssize_t numRead = read(inotifyFd, buf, BUF_LEN);
        switch (numRead)
        {
        case 0:
            return true;
        case -1:
            return false;
        default :
            break;
        }

        for (p = buf; p < buf + numRead; ) {
            event = (struct inotify_event *) p;
            switch (event->mask) {
            case IN_DELETE_SELF:
            case IN_DELETE:
                reset_inotify_add_watch(inotifyFd, path, mask);
                fun(event);
                break;
            case IN_MOVE:
            case IN_MOVE_SELF:
            case IN_IGNORED:
                reset_inotify_add_watch(inotifyFd, path, mask);
                break;
            default:
                fun(event);
                //displayInotifyEvent(event);
                break;
            }
            p += sizeof(struct inotify_event) + event->len;
        }
    }

}

bool PublicFun::SystemEx(const string &cmd, string &strRet)
{
    FILE* pCmdfp			= nullptr;
    char szCmdRet[1024]		= {0};

    memset(szCmdRet,0,1024);

    pCmdfp = popen(cmd.c_str(), "r");
    if(nullptr == pCmdfp){
        strRet = strerror(errno);
        return false;
    }
    if (fgets(szCmdRet,sizeof(szCmdRet),pCmdfp) == nullptr)
    {
        strRet = "";
    }else{
        if (szCmdRet[strlen(szCmdRet)-1] == '\n')
        {
            szCmdRet[strlen(szCmdRet)-1] = '\0';
        }
        strRet = szCmdRet;
    }
    pclose(pCmdfp);
    pCmdfp = nullptr;

    return true;
}

bool PublicFun::SystemEx(const string &cmd, vector<string> &lineVec)
{
    FILE* pCmdfp			= nullptr;
    char szCmdRet[1024]		= {0};

    memset(szCmdRet,0,1024);

    pCmdfp = popen(cmd.c_str(),"r");
    if(nullptr == pCmdfp){
        //strRet = strerror(errno);
        LogError<< "执行命令： "<< cmd
                <<"失败， 错误码：" << strerror(errno)
                <<endl;
        return false;
    }
    while (fgets(szCmdRet,1024,pCmdfp))
    {
        //std::string tmpLine = szCmdRet;
        if (szCmdRet[strlen(szCmdRet)-1] == '\n')
        {
            szCmdRet[strlen(szCmdRet)-1] = '\0';
        }
        lineVec.push_back(szCmdRet);
    }
    pclose(pCmdfp);
    pCmdfp = nullptr;
    return true;
}
