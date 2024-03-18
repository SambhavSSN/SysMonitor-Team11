#include "DatabaseManager.h"


DatabaseManager::DatabaseManager() : ctn(nullptr) {
    try {
        driver = sql::mysql::get_mysql_driver_instance();
        ctn = driver->connect("tcp://localhost:3306", "root", "c1#Sambhav601"); 
        ctn->setSchema("sysInfo"); 
    } catch (const sql::SQLException& e) {
        std::cerr << "Database Connection Error: " << e.what() << std::endl;
    }
}

DatabaseManager::~DatabaseManager() {
    if(ctn) {delete ctn; }
}

void DatabaseManager::CreateTables() {
     if (!ctn) { 
        std::cerr << "Error: Database connection not established.\n";
        return; 
    }
    try {
        sql::Statement* stmt = ctn->createStatement();

        const std::string createInfoTable = R"(
            CREATE TABLE IF NOT EXISTS clientSystemInfo (
                id INT AUTO_INCREMENT PRIMARY KEY,
                hostname VARCHAR(255) NOT NULL,
                macaddress VARCHAR(255) NOT NULL, 
                ramUsage VARCHAR(255) NOT NULL,
                cpuUtilization VARCHAR(255) NOT NULL,
                systemIdleWindow VARCHAR(255) NOT NULL,
                hddUtilization VARCHAR(255) NOT NULL,
                networkStats VARCHAR(255) NOT NULL,
                timestamp VARCHAR(255) NOT NULL
            )
        )";

        stmt->execute(createInfoTable); 
        delete stmt;

    } catch (const sql::SQLException& e) {
        std::cerr << "CreateTables Exception: " << e.what() << std::endl;
    }
}

void DatabaseManager::StoreSystemInfo(const std::string& hostname, const std::string& macaddress, const std::string& ramUsage, const std::string& cpuUtilization, const std::string& systemIdleWindow, const std::string& hddUtilization, const std::string& networkStats,const std::string& timestamp) {
    if (!ctn) { 
        std::cerr << "Error: Database connection not established.\n";
        return; 
    }
    try {
        CreateTables(); 

        sql::PreparedStatement* pstmt = ctn->prepareStatement("INSERT INTO clientSystemInfo (hostname, macaddress, ramUsage, cpuUtilization, systemIdleWindow,  hddUtilization, networkStats, timestamp) VALUES (?, ?, ?, ?, ?, ?, ?, ?)");

        pstmt->setString(1, hostname);
        pstmt->setString(2, macaddress);
        pstmt->setString(3, ramUsage);
        pstmt->setString(4, cpuUtilization);
        pstmt->setString(5, systemIdleWindow);
        pstmt->setString(6, hddUtilization);
        pstmt->setString(7, networkStats);
        pstmt->setString(8, timestamp);

        pstmt->executeUpdate();

        delete pstmt;

        // Export to CSV 
        const std::string csv_file = "/home/sambhav/Desktop/SysMonitor (1)/SysMonitor/Server/data.csv";
        std::ofstream file(csv_file);

        file << "id,hostname,macaddress,ramUsage,cpuUtilization,systemIdleWindow,hddUtilization,networkStats,timestamp" << std::endl;

        sql::PreparedStatement* exportStmt = ctn->prepareStatement("SELECT * FROM clientSystemInfo");
        sql::ResultSet* res = exportStmt->executeQuery();

        while (res->next()) {
            file << res->getString("id") << "," 
                 << res->getString("hostname") << "," 
                 << res->getString("macaddress") << "," 
                 << res->getString("ramUsage") << "," 
                 << res->getString("cpuUtilization") << "," 
                 << res->getString("systemIdleWindow") << "," 
                 << res->getString("hddUtilization") << "," 
                 << res->getString("networkStats") << "," 
                 << res->getString("timestamp") << endl; 
        }

        file.close();
        delete exportStmt;
        delete res; 

    } catch (const sql::SQLException& e) {
        std::cerr << "StoreSystemInfo Exception: " << e.what() << std::endl;
    }
}

//std::vector<std::vector<std::string>> DatabaseManager::fetchCSysInfo(const std::string &hostname) {
//     std::vector<std::vector<std::string>> results; // Store rows in 2D vector form
//     if (!ctn) { 
//         std::cerr << "Error: Database connection not established.\n";
//         return results; 
//     }

//     try {
//         sql::PreparedStatement* pstmt = ctn->prepareStatement("SELECT * FROM clientSystemInfo WHERE hostname = ?");
//         pstmt->setString(1, hostname);
//         sql::ResultSet* res = pstmt->executeQuery();

//         // Iterate through each row
//         while (res->next()) { 
//             std::vector<std::string> row;
//             for (int i = 1; i <= res->getMetaData()->getColumnCount(); i++) {
//                 row.push_back(res->getString(i));
//             } 
//             results.push_back(row); 
//         } 

//         delete pstmt;
//         delete res; 

//     } catch (const sql::SQLException& e) {
//         std::cerr << "Fetch Command Exception: " << e.what() << std::endl;
//     }

//     return results;
// }
