#include <fstream>


#include "ProcessDispose.h"
#include "Tools.h"



size_t ProcessDispose::thread_size(pid_t pid)
{
    size_t size = 0;
    char filepath[64] = { 0 };
    snprintf(filepath, sizeof(filepath), "/proc/%d/task", pid);

    DIR* dirp = opendir(filepath);
    if (dirp)
    {
        dirent* entry = nullptr;
        while ((entry = readdir(dirp)))
        {
            if (entry->d_type == DT_DIR && Tools::is_number(entry->d_name))
            {
                ++size;
            }
        }
        closedir(dirp);
    }
    return size;
}

string ProcessDispose::process_memory(pid_t pid)
{
    long memory = 0;
    char filepath[64] = { 0 };
    snprintf(filepath, sizeof(filepath), "/proc/%d/status", pid);
    char tmp[20] = { 0 };
    std::string retStr;

    FILE* fp = fopen(filepath, "r");
    if (!fp)
    {
        return "";
    }

    char buffer[1024] = { 0 };
    while (fgets(buffer, sizeof(buffer), fp))
    {
        if (strncmp(buffer, "VmRSS:", 6) == 0)
        {
            sscanf(buffer, "%*s %ld\n", &memory);
            break;
        }
    }
    fclose(fp);
    sprintf(tmp,"%ld",memory);
    retStr = tmp;

    return retStr;
}


string ProcessDispose::read_link_content(const string &file)
{
    static const std::string del = " (deleted)";

    char buffer[1024] = { 0 };
    ssize_t len = ::readlink(file.c_str(), buffer, sizeof(buffer) - 1);
    if (len < 0)
    {
        return std::string();
    }

    std::string link = buffer;
    if (link.size() >= del.size())
    {
        if (link.compare(link.size() - del.size(), del.size(), del) == 0)
        {
            link.erase(link.size() - del.size());
        }
    }
    return link;
}

string ProcessDispose::read_proc_exe(pid_t pid)
{
    char filepath[64] = { 0 };
    snprintf(filepath, sizeof(filepath), "/proc/%d/exe", pid);
    return read_link_content(filepath);
}

string ProcessDispose::process_name(pid_t pid)
{
    char filepath[64] = { 0 };
    snprintf(filepath, sizeof(filepath), "/proc/%d/stat", pid);

    std::string name;
    std::ifstream istrm(filepath);
    if (istrm.is_open())
    {
        std::string line;
        std::getline(istrm, line);
        auto lp = line.find('(');
        auto rp = line.rfind(')');

        if (lp != std::string::npos && rp != std::string::npos && lp < rp)
        {
            name = line.substr(lp + 1, rp - lp - 1);
        }

        istrm.close();
    }
    return name;
}

string ProcessDispose::process_start_time(pid_t pid)
{
    char filepath[64] = { 0 };
    snprintf(filepath, sizeof(filepath), "/proc/%d", pid);

    time_t timeSeconds;
    struct stat buf;
    if (stat(filepath, &buf) == 0)
    {
        timeSeconds = buf.st_mtime;
    }
    else
    {
        timeSeconds = ::time(nullptr);
    }

    struct tm tm_time;
    localtime_r(&timeSeconds, &tm_time);

    char timeBuf[32] = { 0 };
    snprintf(timeBuf, sizeof(timeBuf), "%4d-%02d-%02d %02d:%02d:%02d",
             tm_time.tm_year + 1900, tm_time.tm_mon + 1, tm_time.tm_mday,
             tm_time.tm_hour, tm_time.tm_min, tm_time.tm_sec);

    return timeBuf;
}
