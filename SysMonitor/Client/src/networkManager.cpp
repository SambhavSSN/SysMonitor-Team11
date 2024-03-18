// networkManager.cpp
#include "networkManager.h"

NetworkManager::NetworkManager() : socket(io_service) { // Initialize socket in member initializer list
}

bool NetworkManager::connectToServer() {

    try {

        boost::asio::ip::tcp::socket socket(io_service);
        socket.connect(boost::asio::ip::tcp::endpoint(boost::asio::ip::address::from_string("127.0.0.1"), 8080));

        std::cout << "Connected to server at " << serverIP << ":" << serverPort << std::endl;
        return true;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
    
}

bool NetworkManager::sendData(const std::string& data) {
    try {
        boost::asio::write(socket, boost::asio::buffer(data + "\n"));
        std::cout << "Data sent successfully" << std::endl;
        return true;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return false;
    }
}

std::string NetworkManager::receiveData() {
    try {
        boost::asio::streambuf response;
        boost::asio::read_until(socket, response, "\n");
        std::istream receive_stream(&response);
        std::string receivedData;
        std::getline(receive_stream, receivedData);
        std::cout << "Received data: " << receivedData << std::endl;
        return receivedData;
    } catch (std::exception& e) {
        std::cerr << "Exception: " << e.what() << std::endl;
        return "";
    }
}
