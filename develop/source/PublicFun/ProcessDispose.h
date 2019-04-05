#ifndef PROCESSDISPOSE_H
#define PROCESSDISPOSE_H

#include <sys/types.h>
#include <dirent.h>
#include <unistd.h>
#include <sys/stat.h>


#include "Publicdefine.h"
//#include "PublicFun.h"
/*
 *
 * 跟进程相关的
 *
 */
namespace ProcessDispose
{
   //进程开启的线程数
   size_t thread_size(pid_t pid);

   //获取进程占用的内存
   string process_memory(pid_t pid);

   // 读取/proc/pid/exe获取进程路径
   string read_proc_exe(pid_t pid);

   // 读取软链接文件的内容
   string read_link_content(const std::string& file);

   // 读取/proc/pid/stat获取进程名
   std::string process_name(pid_t pid);

   // 读取/proc/pid获取进程启动时间
   std::string process_start_time(pid_t pid);
};

#endif // PROCESSDISPOSE_H
