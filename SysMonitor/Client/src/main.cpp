#include "systemInfoCollector.h"
#include <boost/asio.hpp>
#include <chrono>
#include <thread>
#include <iostream>
using namespace std;

int main() {
    string serverIP;
    int serverPORT;

    cout << "Enter Server Address: ";
    cin >> serverIP;
    cout << "Enter Server Port: ";
    cin >> serverPORT;

    SystemInfoCollector sysInfo(serverIP);

    try {
        
        boost::asio::io_service io_service;

        // Connect to the server
        boost::asio::ip::tcp::socket socket(io_service);
        socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string(serverIP), serverPORT));

        std::cout << "Connected to server" << std::endl;
        
        while(true){
             // Send data to the server
            string clientData = sysInfo.fetchData();
            boost::asio::write(socket, boost::asio::buffer(clientData + "\n"));
            std::cout << "Sent data to server" << std::endl;

            std::this_thread::sleep_for(std::chrono::seconds(10));
        }

    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
    }

    return 0;
}
