#ifndef SYSTEMINFOCOLLECTOR_H
#define SYSTEMINFOCOLLECTOR_H

#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <sys/sysinfo.h>
#include <unistd.h>
using namespace std;

class SystemInfoCollector {
public:
    std::string serverAddress;
    SystemInfoCollector(const std::string& ipAddress);
    std::string fetchData();
    
private:
    std::string systemName;
    std::string networkStatus;
    std::string ramUsage;
    std::string cpuUtilization;
    std::string idleTime;
    std::string hddUtilization;

    bool getNetworkStatus(const std::string& ip);
    std::string getAddress() const;
    std::string getSystemName() const;
    std::string getRAMUsage() const;
    std::string getCPUUtilization() const;
    std::string getSystemIdleWindow() const;
    std::string getHDDUtilization() const;
    std::string getTimestamp();
    
};

#endif // SYSTEMINFOCOLLECTOR_H
