#include <sys/types.h>
#include <dirent.h>
#include <sys/vfs.h>
#include <istream>
#include <fstream>

#include "HardWareInfo.h"


std::vector<string> HardWareInfo::GetDiskDeviceName()
{
    std::vector<std::string> diskNames;

    const auto& disks = GetScsiDisk();
    const auto& hosts = GetScsiHost();

    // 排除USB存储设备
    for (auto iter = disks.begin(); iter != disks.end(); ++iter)
    {
        auto host = hosts.find(iter->second);
        if (host != hosts.end())
        {
            const std::string& procName = host->second;
            if (procName != "usb-storage")
            {
                diskNames.push_back(iter->first);
            }
        }
    }

    // KVM虚拟磁盘名称为vda，添加KVM磁盘
    DIR* dirp = ::opendir("/sys/block");
    if (dirp)
    {
        struct dirent* entry;
        while ((entry = ::readdir(dirp)))
        {
            const char* name = entry->d_name;
            if (strncmp(name, "vd", 2) == 0)
            {
                diskNames.push_back(name);
            }
        }

        ::closedir(dirp);
    }

    return diskNames;
}

HardWareInfo::DeviceSize HardWareInfo::GetDeviceSize()
{
    DeviceSize devSizes;

    FILE* fp = ::popen("lsblk -nbo NAME,SIZE,MOUNTPOINT", "r");
    if (fp)
    {
        std::string devName;
        char buffer[BUFSIZ];

        while (fgets(buffer, sizeof(buffer), fp))
        {
            char* dev = buffer;
            char* size = nullptr;
            char* mount = nullptr;
            char* saveptr = nullptr;

            // 分割字符
            for (char* str = buffer; /**/; str = nullptr)
            {
                char* token = ::strtok_r(str, " ", &saveptr);
                if (!token)
                    break;
                if (::isdigit(*token))
                    size = token;
                else
                    mount = token;
            }

            if (mount && mount[strlen(mount) - 1] == '\n')
            {
                mount[strlen(mount) - 1] = '\0';
            }

            // 一块新设备
            if (::isalpha(*dev))
            {
                devName = dev;
                uint64_t total = ::strtoull(size, 0, 10);
                devSizes.insert(std::make_pair(
                                    devName, std::make_pair(total, total)));
            }

            // 已经挂载到目录
            if (mount && *mount == '/')
            {
                devSizes[devName].first -= freeSize(mount);
            }
        }

        ::pclose(fp);
    }

    return devSizes;
}

uint64_t HardWareInfo::freeSize(const char *path)
{
    uint64_t size = 0;
    struct statfs sfs;
    if (statfs(path, &sfs) == 0)
    {
        size = sfs.f_bsize * static_cast<uint64_t>(sfs.f_bfree);
    }
    return size;
}

std::map<string, int> HardWareInfo::GetScsiDisk()
{
    std::map<std::string, int> scsiDisks;

    DIR* dirp = ::opendir("/sys/class/scsi_disk");
    if (dirp)
    {
        struct dirent* entry;
        while ((entry = ::readdir(dirp)))
        {
            const char* name = entry->d_name;
            if (*name == '.')
                continue;

            const std::string& diviceName = GetBlockName(name);
            scsiDisks.insert(std::make_pair(diviceName, ::atoi(name)));
        }

        ::closedir(dirp);
    }

    return scsiDisks;
}

std::map<int, string> HardWareInfo::GetScsiHost()
{
    std::map<int, std::string> scsiHosts;

    DIR* dirp = ::opendir("/sys/class/scsi_host");
    if (dirp)
    {
        struct dirent* entry;
        while ((entry = ::readdir(dirp)))
        {
            const char* name = entry->d_name;
            if (*name == '.')
                continue;

            string path = "/sys/class/scsi_host/" + string(name) + "/proc_name";
            std::ifstream input(path.c_str());

            if (input.is_open())
            {
                std::string line;
                std::getline(input, line);
                scsiHosts.insert(std::make_pair(::atoi(name + 4), line));
                input.close();
            }
        }

        ::closedir(dirp);
    }

    return scsiHosts;
}

string HardWareInfo::GetBlockName(const char *scsi)
{
    std::string blockName;

    string path = "/sys/class/scsi_disk/" + string(scsi) +"/device/block";
    DIR* dirp = ::opendir(path.data());
    if (dirp)
    {
        struct dirent* entry;
        while ((entry = ::readdir(dirp)))
        {
            const char* name = entry->d_name;
            if (*name == '.')
                continue;

            blockName = name;
        }

        ::closedir(dirp);
    }

    return blockName;
}
