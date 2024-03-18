#include "systemInfoCollector.h"
using namespace std;

struct sysinfo xp;

SystemInfoCollector::SystemInfoCollector(const std::string& ipAddress){
    serverAddress = ipAddress;
}

bool SystemInfoCollector::getNetworkStatus(const std::string& ip){
    //string command = "ping -c 1 " + ip;
    string command = "ping -c 1 " + ip + " >nul 2>&1";
    int exitCode = system(command.c_str());
    return (exitCode == 0);
}

string SystemInfoCollector::getAddress() const{
    FILE* pipe = popen("ifconfig", "r"); // You can use "ip link" for newer systems
    if (!pipe) {
        perror("Error opening pipe!");
        return "Unable to fetch MAC address";
    }

    char buffer[128];
    std::string result = "";

    // Read the output of the command line by line
    while (!feof(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) != nullptr) {
            result += buffer;
        }
    } 

    pclose(pipe);

    // Find and extract the MAC address from the result (for "ether" lines)
    size_t macPos = result.find("ether");
    if (macPos != std::string::npos) {
        size_t start = macPos + 6; // Position after "ether"
        size_t end = start + 17;  // MAC address length (17 characters)
        return result.substr(start, end - start);
    }

    return "MAC_address_not_found";
}

string SystemInfoCollector::getSystemName() const{
    char hostname[1024];
    if (gethostname(hostname, sizeof(hostname)) == -1){
        return "Unable to extract system name";
    }

    return string(hostname);
}

string SystemInfoCollector::getRAMUsage() const {

    if (sysinfo(&xp) != 0) {
        return "Unable to extract ram usage";
    }

    // Find RAM usage
    float ramUsage = 100.0 * (1 - (float)xp.freeram / xp.totalram);
    return to_string(ramUsage);
}

string SystemInfoCollector::getCPUUtilization() const {

    ifstream statFile("/proc/stat");
    string line;
    if (statFile.is_open()) {
        getline(statFile, line); // Read the first line
        istringstream iss(line);
        string cpuLabel;
        long user, nice, system, idle, iowait, irq, softirq;
        iss >> cpuLabel >> user >> nice >> system >> idle >> iowait >> irq >> softirq;
        long totalTime = user + nice + system + idle + iowait + irq + softirq;
        long idleTime = idle + iowait;

        float cpuUtilization = 100.0 * (1 - (float)idleTime / totalTime);
        
        statFile.close();

        return to_string(cpuUtilization);
    } else {
        return "Unable to open /proc/stat for reading.";
    }
}

string SystemInfoCollector::getSystemIdleWindow() const {

    ifstream statFile("/proc/stat");
    string line;
    if (statFile.is_open()) {
        getline(statFile, line); // Read the first line
        istringstream iss(line);
        long idle, iowait;
        iss >> idle >> iowait;
    
        long idleTime = idle + iowait;

        float systemIdleTime= xp.uptime - (float)idleTime / sysconf(_SC_CLK_TCK); 
        
        statFile.close();

        return to_string(systemIdleTime);
    } else {
        return "Unable to open /proc/stat for reading.";
    }
}

string SystemInfoCollector::getHDDUtilization() const {
    FILE *dfPipe = popen("df -h /", "r");
    if (dfPipe != nullptr) {
        char buffer[128];
        fgets(buffer, sizeof(buffer), dfPipe); 
        fgets(buffer, sizeof(buffer), dfPipe);
        //cout<<buffer<<endl;
        pclose(dfPipe);
        istringstream iss(buffer);
        string fileSystem, size, used, avail, usePercent, mountedOn;
        iss >> fileSystem >> size >> used >> avail >> usePercent >> mountedOn;
        size.pop_back(); 
        used.pop_back(); 
        
        
        float hddUtilization= stof(used) / stof(size) * 100.0;
        
        return to_string(hddUtilization);
    } else {
        return "Unable to open /proc/stat for reading.";
    }
}

string SystemInfoCollector::getTimestamp(){
    // Get the current time
    time_t currentTime = time(nullptr);

    // Create a buffer to store the formatted date and time
    char buffer[80]; // Adjust the buffer size as needed

    // Format the current time as a string
    strftime(buffer, sizeof(buffer), "%Y-%m-%d %H:%M:%S", localtime(&currentTime));

    // Convert the buffer to a C++ string
    string dateTimeString(buffer);
    return dateTimeString;
}

string SystemInfoCollector::fetchData(){
    systemName = getSystemName();
    networkStatus = getNetworkStatus(this->serverAddress) ? "Connected" : "Disconnected";
    ramUsage = getRAMUsage();
    cpuUtilization = getCPUUtilization();
    idleTime = getSystemIdleWindow();
    hddUtilization = getHDDUtilization();

    string clientData = systemName + " " + getAddress() + " " + ramUsage + " "
        + cpuUtilization + " " + hddUtilization + " " + idleTime + " "
        + networkStatus + " " + getTimestamp();

    return clientData;
}