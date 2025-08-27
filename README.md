# Fractal

Fractal is a lightweight Windows command-line tool that displays detailed information about your PC hardware and system.  

It gathers information such as:
- CPU details  
- GPU information  
- Memory usage 
- Operating system version  

## Features
- Clean console output  
- Uses native Windows APIs (no external dependencies)  
- Fast and lightweight  

## Build Instructions
Fractal is written in **C++** and can be built with Microsoft Visual Studio or the MSVC compiler.

### Using the Command Line "to compile into an exe instead of download"
```sh
cl /EHsc main.cpp systeminfo.cpp iphlpapi.lib pdh.lib Ws2_32.lib User32.lib /FeFractal.exe