#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>

class server 
{
	server();
	void sendFile(const std::string& host, unsigned short port, const std::string& filepath);
};
class client 
{
public:
	client();
	void sendFile(const std::string& host, unsigned short port, const std::string& filepath);
};