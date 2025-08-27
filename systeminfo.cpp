#include "systeminfo.h"

#define WIN32_LEAN_AND_MEAN

#include <winsock2.h>
#include <ws2tcpip.h>
#include <iphlpapi.h>

#include <windows.h>
#include <winternl.h>

#include <pdh.h>
#include <vector>

#pragma comment(lib, "iphlpapi.lib")
#pragma comment(lib, "pdh.lib")
#pragma comment(lib, "Ws2_32.lib")

static PDH_HQUERY cpuQuery;
static PDH_HCOUNTER cpuTotal;

void initPerformanceCounters() {
    PdhOpenQueryA(NULL, 0, &cpuQuery);
    PdhAddCounterA(cpuQuery, "\\Processor(_Total)\\% Processor Time", 0, &cpuTotal);
    PdhCollectQueryData(cpuQuery);
}

void getSystemInfo(SystemInfo& info) {

    OSVERSIONINFOEXW osvi{ sizeof(osvi) };
    if (auto pRtlGetVersion = (NTSTATUS(WINAPI*)(LPOSVERSIONINFOEXW))GetProcAddress(GetModuleHandleA("ntdll"), "RtlGetVersion")) {
        pRtlGetVersion(&osvi);
        info.osName = "Windows " + std::to_string(osvi.dwMajorVersion) + "." + std::to_string(osvi.dwMinorVersion);
    }

    char buffer[MAX_COMPUTERNAME_LENGTH + 1];
    DWORD size = sizeof(buffer);
    info.computerName = GetComputerNameA(buffer, &size) ? buffer : "Unknown";

    SYSTEM_INFO sysinfo;
    GetSystemInfo(&sysinfo);
    info.cpuCores = sysinfo.dwNumberOfProcessors;

    DISPLAY_DEVICEA dd{ sizeof(dd) };
    info.gpuName = EnumDisplayDevicesA(0, 0, &dd, 0) ? dd.DeviceString : "Unknown GPU";

    info.ipAddress = "N/A";
    ULONG bufLen = 15000;
    std::vector<BYTE> ipBuffer(bufLen);
    PIP_ADAPTER_ADDRESSES addrs = reinterpret_cast<PIP_ADAPTER_ADDRESSES>(ipBuffer.data());
    if (GetAdaptersAddresses(AF_INET, 0, NULL, addrs, &bufLen) == ERROR_SUCCESS) {
        for (auto* curr = addrs; curr; curr = curr->Next) {
            if (curr->OperStatus == IfOperStatusUp && curr->IfType != IF_TYPE_SOFTWARE_LOOPBACK) {
                sockaddr_in* sa_in = (sockaddr_in*)curr->FirstUnicastAddress->Address.lpSockaddr;
                char ipStr[INET_ADDRSTRLEN];
                if (inet_ntop(AF_INET, &(sa_in->sin_addr), ipStr, INET_ADDRSTRLEN)) {
                    info.ipAddress = ipStr;
                    break;
                }
            }
        }
    }
    
    updateDynamicInfo(info);
}

void updateDynamicInfo(SystemInfo& info) {
    MEMORYSTATUSEX mem{ sizeof(mem) };
    if (GlobalMemoryStatusEx(&mem)) {
        info.totalRAM = mem.ullTotalPhys;
        info.usedRAM = mem.ullTotalPhys - mem.ullAvailPhys;
    }

    PDH_FMT_COUNTERVALUE counterVal;
    PdhCollectQueryData(cpuQuery);
    if (PdhGetFormattedCounterValue(cpuTotal, PDH_FMT_DOUBLE, NULL, &counterVal) == ERROR_SUCCESS) {
        info.cpuUsage = counterVal.doubleValue;
    }
}