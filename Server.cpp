#include "asioModule.h"

using boost::asio::ip::tcp;



Server::Server() {
    try {
        boost::asio::io_context io_context;
        Server server.init(io_context, 1234);
        std::cout << "Сервер запущен на порту 1234\n";
        io_context.run();
    }
    catch (const std::exception& e) {
        std::cerr << "Исключение: " << e.what() << "\n";
    }
}