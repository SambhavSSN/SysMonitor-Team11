// server.cpp
#include <iostream>
#include <boost/asio.hpp>
#include <DatabaseManager.h>
using namespace std;

int main() {
    try {
        boost::asio::io_service io_service;

        // Create an acceptor to listen for incoming connections
        boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));

        std::cout << "Server started. Listening on port 8080..." << std::endl;

        // Accept connections
        while (true) {
            boost::asio::ip::tcp::socket socket(io_service);
            acceptor.accept(socket);

            // Read data from the socket
            boost::asio::streambuf buffer;
            boost::asio::read_until(socket, buffer, "\n");
            std::istream input_stream(&buffer);
            std::string data;
            std::getline(input_stream, data);

            std::cout << "Received data from client: " << std::endl;

            // Echo the received data back to the client
            std::string ack = "Data Recieved successfully at server.";
            boost::asio::write(socket, boost::asio::buffer(data + "\n"));

            // Store in Database
            string receivedData = data;
            //cout << receivedData;
            istringstream iss(receivedData);

            vector<string> tokens;
            string x;
            while (iss >> x) {
                tokens.push_back(x);
            }

            if (tokens.size() >= 7) {
                string hostname = tokens[0];
                string macaddress = tokens[1];
                string ramUsage = tokens[2];
                string cpuUtilization = tokens[3];
                string systemIdleWindow = tokens[4];
                string hddUtilization = tokens[5];
                string networkStats = tokens[6];
                string timestamp = tokens[7]+" "+tokens[8];

                cout << "Hostname: " << hostname << endl;
                cout << "MAC Address: " << macaddress << endl;
                cout << "RAM Usage: " << ramUsage << endl;
                cout << "CPU Utilization: " << cpuUtilization << endl;
                cout << "System Idle Window: " << systemIdleWindow << endl;
                cout << "HDD Utilization: " << hddUtilization << endl;
                cout << "Network Stats: " << networkStats << endl;
                cout << "Timestamp: " << timestamp << endl;
            // Create DatabaseManager
            DatabaseManager db;

            // Store system info
            db.StoreSystemInfo(hostname,macaddress,ramUsage,cpuUtilization,systemIdleWindow,hddUtilization,networkStats,timestamp);
            } else {
                cerr << "Error: Insufficient data in the input string.\n";
            }

            // Fetch from database when requested
            // string command;
            // cout << "Enter command: ";
            // std::getline(std::cin, command);

            // if (command.substr(0, 5) == "fetch") {
            //     std::string hostname = command.substr(6); // Get hostname from command
            //     DatabaseManager db; 
            //     std::vector<std::vector<std::string>> results = db.fetchCSysInfo(hostname);

            //     if (!results.empty()) {
            //         cout << "System Information:\n";
            //         cout << "-------------------\n";

            //         // Print each row
            //         for (const auto& r : results) {
            //             for (const auto& data : r) {
            //                 cout << data << "  "; 
            //             }
            //             cout << endl; 
            //         }
            //     }
            // }

        }
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
// #include <thread>
// #include <vector>
// #include <mutex>

// std::mutex m; 

// void handle_client(boost::asio::ip::tcp::socket socket, DatabaseManager db)
// {
//     try
//     {
//         // Read data from the socket
//         boost::asio::streambuf buffer;
//         boost::asio::read_until(socket, buffer, "\n");
//         std::istream input_stream(&buffer);
//         std::string data;
//         std::getline(input_stream, data);

//         {
//             std::lock_guard<std::mutex> lock(m);
//             std::cout << "Received data from client: " << data << std::endl;
//         }

//         // Echo the received data back to the client
//         std::string ack = "Data Recieved successfully at server.";
//         boost::asio::write(socket, boost::asio::buffer(data + "\n"));

//         // Store in Database
//         string receivedData = data;
//         istringstream iss(receivedData);

//         vector<string> tokens;
//         string x;
//         while (iss >> x)
//         {
//             tokens.push_back(x);
//         }

//         if (tokens.size() >= 7)
//         {
//             string hostname = tokens[0];
//             string macaddress = tokens[1];
//             string ramUsage = tokens[2];
//             string cpuUtilization = tokens[3];
//             string systemIdleWindow = tokens[4];
//             string hddUtilization = tokens[5];
//             string networkStats = tokens[6];
//             string timestamp = tokens[7] + " " + tokens[8];

//             // Create DatabaseManager
//             DatabaseManager db;

//             // Store system info
//             db.StoreSystemInfo(hostname, macaddress, ramUsage, cpuUtilization, systemIdleWindow, hddUtilization, networkStats, timestamp);
//         }
//         else
//         {
//             cerr << "Error: Insufficient data in the input string.\n";
//         }
//     }
//     catch (std::exception &e)
//     {
//         std::cerr << "Exception in client thread: " << e.what() << std::endl;
//     }
// }

// int main()
// {
//     try
//     {
//         boost::asio::io_service io_service;

//         // Create an acceptor to listen for incoming connections
//         boost::asio::ip::tcp::acceptor acceptor(io_service, boost::asio::ip::tcp::endpoint(boost::asio::ip::tcp::v4(), 8080));
//         std::cout << "Server started. Listening on port 8080..." << std::endl;

//         // Keep track of active client threads
//         std::vector<std::thread> client_threads;
//         DatabaseManager db; // Create database manager

//         // Accept connections
//         while (true)
//         {
//             if (client_threads.size() < 3)
//             { // Limit to max 3 clients
//                 boost::asio::ip::tcp::socket socket(io_service);
//                 acceptor.accept(socket);

//                 client_threads.push_back(std::thread(handle_client, std::move(socket), db));
//             }
//             else
//             {
//                 // Handle the case when max capacity is reached
//                 std::this_thread::sleep_for(std::chrono::seconds(1));
//             }

//             for (auto it = client_threads.begin(); it != client_threads.end();)
//             {
//                 if (it->joinable())
//                 {
//                     it->join();
//                     it = client_threads.erase(it);
//                 }
//                 else
//                 {
//                     ++it;
//                 }
//             }
//         }
//     }
//     catch (std::exception &e)
//     {
//         std::cerr << "Exception: " << e.what() << std::endl;
//     }

//     return 0;
// }

