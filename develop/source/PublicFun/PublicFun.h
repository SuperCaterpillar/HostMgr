#ifndef PUBLICFUN_H
#define PUBLICFUN_H

#include <sys/types.h>
#include <stdint.h>
#include <sys/inotify.h>

#include "Publicdefine.h"
#include "Logging.h"

namespace PublicFun {
    //监控
    bool monitor_file_system(uint32_t, const stringVet &,file_sys_monitor_call );

    //执行shell脚本 cmd 脚本命令 srRet 脚本返回值
    bool SystemEx(const string &cmd, string &strRet);

    //返回获取到的所有数据，每一行数据为一条记录
    bool SystemEx(const string &cmd, vector<string> &lineVec);

}
#endif // PUBLICFUN_H
