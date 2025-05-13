#include "asioModule.h"

using boost::asio::ip::tcp;
namespace fs = std::filesystem;

void client::sendFile(const std::string& host, unsigned short port, const std::string& filepath) {
    try {
        boost::asio::io_context io_context;
        tcp::resolver resolver(io_context);
        tcp::socket socket(io_context);

        boost::asio::connect(socket, resolver.resolve(host, std::to_string(port)));

        fs::path path(filepath);
        if (!fs::exists(path)) {
            std::cerr << "���� �� ����������: " << filepath << std::endl;
            return;
        }

        std::ifstream file(filepath, std::ios::binary);
        if (!file) {
            std::cerr << "error with open file: " << filepath << std::endl;
            return;
        }

        file.seekg(0, std::ios::end);
        size_t filesize = file.tellg();
        file.seekg(0, std::ios::beg);

        std::string header = path.filename().string() + ":" + std::to_string(filesize) + "\n";
        boost::asio::write(socket, boost::asio::buffer(header));

        std::vector<char> buffer(filesize);
        file.read(buffer.data(), filesize);
        boost::asio::write(socket, boost::asio::buffer(buffer));

        //std::cout << "���� " << path.filename() << " ������� ��������� ("<< filesize << " ����)" << std::endl;
    }
    catch (std::exception& e) {
        std::cerr << "������: " << e.what() << std::endl;
    }
}

client::client() {
    try {
        std::string host, filepath;
        unsigned short port;

        std::cout << "enter adress(for example, localhost or IP): ";
        std::getline(std::cin, host);

        std::cout << "Enter the port of server: ";
        std::cin >> port;

        // ������� ����� ����� ����� ������� ���� � �����
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');

        std::cout << "enter the path to file: ";
        std::getline(std::cin, filepath);

        sendFile(host, port, filepath);
    }
    catch (std::exception& e) {
        std::cerr << "������: " << e.what() << "\n";
    }
}