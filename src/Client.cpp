#include "Client.hpp"
#include <sys/types.h>
#include <exception>
#include <iostream>
#include "Socket.hpp"
#include "Utils.hpp"

ssize_t Client::readFromSocket()
{
  try {
    ssize_t bytes_received = this->conn_socket.receiveData(inputBuffer);
    return bytes_received;
  } catch (const std::exception& e) {
    std::cerr << "Error in Client::readFromSocket: " << e.what() << '\n';
    throw;
  }
}

ssize_t Client::writeFromSocket()
{
  try {
    ssize_t bytes_written = this->conn_socket.sendData(outputBuffer);
    return bytes_written;
  } catch (const std::exception& e){
    std::cerr << "Error in Client::writeFromSocket: " << e.what() << '\n';
    throw;
  }
}

void Client::connectToServer(const std::string& ip, u16 port)
{ 
  try 
  {
    conn_socket.connectSocket(ip, port);
  } catch (const std::exception& e) 
  {
    std::cerr << "Error trying to connect to server." << '\n';
    throw;    
  }
}
