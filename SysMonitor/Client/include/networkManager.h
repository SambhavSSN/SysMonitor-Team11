// networkManager.h
#ifndef NETWORKMANAGER_H
#define NETWORKMANAGER_H

#include <iostream>
#include <boost/asio.hpp>

class NetworkManager {
public:
    NetworkManager();
    bool connectToServer();
    bool sendData(const std::string& data);
    std::string receiveData();
    std::string serverIP;
    int serverPort;

private:
    
    boost::asio::io_service io_service;
    boost::asio::ip::tcp::socket socket;
};

#endif // NETWORKMANAGER_H
