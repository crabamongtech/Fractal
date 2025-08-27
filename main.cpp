#include "systeminfo.h"
#include <iostream>
#include <windows.h>
#include <iomanip>

int main() {
    SetConsoleTitleA("Fractal - by Crabamongspies");
    initPerformanceCounters();
    Sleep(1000);

    SystemInfo info;
    getSystemInfo(info);

    std::cout << "==================== Fractal ====================\n"
              << "Made by: Crabamongspies\n\n"
              << "OS: " << info.osName << "\n"
              << "Computer Name: " << info.computerName << "\n"
              << "CPU Cores: " << info.cpuCores << "\n"
              << "GPU: " << info.gpuName << "\n"
              << "Total RAM: " << info.totalRAM / (1024 * 1024) << " MB\n";

    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_SCREEN_BUFFER_INFO csbi;
    GetConsoleScreenBufferInfo(hConsole, &csbi);
    const COORD dynamicOutputPos = csbi.dwCursorPosition;

    while (true) {
        updateDynamicInfo(info);
        SetConsoleCursorPosition(hConsole, dynamicOutputPos);

        std::cout << "Used RAM: " << std::setw(8) << info.usedRAM / (1024 * 1024) << " MB"
                  << "    CPU Usage: " << std::setw(5) << std::fixed << std::setprecision(1)
                  << info.cpuUsage << " %   ";

        Sleep(1000);
    }

    return 0;
}