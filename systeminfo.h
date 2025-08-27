#pragma once
#include <string>

struct SystemInfo {
    std::string osName;
    std::string computerName;
    std::string gpuName;
    std::string ipAddress;
    unsigned long long totalRAM = 0;
    unsigned long long usedRAM = 0;
    int cpuCores = 0;
    double cpuUsage = 0.0;
};

void initPerformanceCounters();
void getSystemInfo(SystemInfo& info);
void updateDynamicInfo(SystemInfo& info);