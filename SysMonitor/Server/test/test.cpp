#include <iostream>
#include <sys/sysinfo.h>
using namespace std;


// Function to get RAM usage as a string
float getRAMUsage() {
    struct sysinfo xp;
    if (sysinfo(&xp) != 0) {
        cerr << "Failed to extract system info..." << endl;
        exit(1);
    }

    // Find RAM usage

    cout << (xp.freeram)/(1024*1024) << endl 
    << (xp.totalram)/(1024*1024) << endl;

    float ramUsage = 100.0 * (1 - (float)xp.freeram / xp.totalram);
    return ramUsage;
}

int main() {
    float ramUsage = getRAMUsage();
    std::cout << "RAM Usage: " << ramUsage << "%" << std::endl;
    return 0;
}
