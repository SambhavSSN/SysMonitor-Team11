#ifndef DATABASEMANAGER_H
#define DATABASEMANAGER_H

#include <string>
#include <vector>
#include <sstream>
#include <mysql_connection.h>
#include <mysql_driver.h>
#include <cppconn/exception.h> 
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>
#include <fstream>
using namespace std; 


class DatabaseManager {
public:
    DatabaseManager();
    ~DatabaseManager();
    void CreateTables(); 
    void StoreSystemInfo(const std::string& hostname, const std::string& macaddress, const std::string& ramUsage, const std::string& cpuUtilization, const std::string& systemIdleWindow, const std::string& hddUtilization,const std::string& networkStats, const std::string& timestamp);
    //std::vector<std::string> fetchCSysInfo(const std::string &hostname); 
    //std::vector<std::vector<std::string>> fetchCSysInfo(const std::string &hostname);

private:
    sql::Driver* driver;
    sql::Connection* ctn;

};

#endif
