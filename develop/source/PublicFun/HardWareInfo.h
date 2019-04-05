#ifndef HARDWAREINFO_H
#define HARDWAREINFO_H
#include "Publicdefine.h"

namespace HardWareInfo {

// 获取路径所挂载目录的可用空间大小
uint64_t freeSize(const char* path);

std::map<std::string, int> GetScsiDisk();
std::map<int, std::string> GetScsiHost();
std::string GetBlockName(const char* scsi);

    typedef std::map<std::string, std::pair<uint64_t, uint64_t>> DeviceSize;

    DeviceSize GetDeviceSize();
    std::vector<std::string> GetDiskDeviceName();
};

#endif // HARDWAREINFO_H
