#pragma once

#include <boost/asio.hpp>
#include <iostream>
#include <fstream>
#include <string>
#include <filesystem>
#include <QWidget>
#include <QMainWindow>
#include <QPushButton>
#include <QComboBox>
#include <QLabel>
#include <QVBoxLayout>

#include <QDebug>

using boost::asio::ip::tcp;

class Server {
public:
    Server(boost::asio::io_context& io_context, short port)
        : acceptor_(io_context, tcp::endpoint(tcp::v4(), port)) {
        do_accept();
    }

private:
    void do_accept() {
        acceptor_.async_accept(
            [this](boost::system::error_code ec, tcp::socket socket) {
                if (!ec) {
                    std::make_shared<Session>(std::move(socket))->start();
                }
                do_accept();
            });
    }

    class Session : public std::enable_shared_from_this<Session> {
    public:
        Session(tcp::socket socket) : socket_(std::move(socket)) {}

        void start() {
            read_header();
        }

    private:
        void read_header() {
            auto self(shared_from_this());
            boost::asio::async_read(socket_,
                boost::asio::buffer(&header_, sizeof(header_)),
                [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                    if (!ec) {
                        read_filename();
                    }
                });
        }

        void read_filename() {
            auto self(shared_from_this());
            filename_.resize(header_.filename_length);
            boost::asio::async_read(socket_,
                boost::asio::buffer(&filename_[0], header_.filename_length),
                [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                    if (!ec) {
                        read_file_content();
                    }
                });
        }

        void read_file_content() {
            auto self(shared_from_this());
            file_content_.resize(header_.content_length);
            boost::asio::async_read(socket_,
                boost::asio::buffer(&file_content_[0], header_.content_length),
                [this, self](boost::system::error_code ec, std::size_t /*length*/) {
                    if (!ec) {
                        process_file();
                    }
                });
        }
   
        void process_file() {
            try {
                // Добавляем логгирование
                qDebug() << "Processing file:" << filename_.c_str()
                    << "Size:" << file_content_.size() << "bytes";

                // Открываем файл в режиме добавления с проверкой
                std::ofstream file(filename_, std::ios::app | std::ios::binary);
                if (!file) {
                    qDebug() << "Failed to open file:" << filename_.c_str();
                    return;
                }

                // Записываем разделитель и данные
                file.write("~~~", 3);
                file.write(file_content_.data(), file_content_.size());

                // Явно закрываем файл
                file.close();

                qDebug() << "Successfully processed:" << filename_.c_str();
            }
            catch (const std::exception& e) {
                qDebug() << "Error processing file" << filename_.c_str()
                    << ":" << e.what();
            }
        }


        struct FileHeader {
            uint32_t filename_length;
            uint32_t content_length;
        };

        tcp::socket socket_;
        FileHeader header_{};
        std::string filename_;
        std::vector<char> file_content_;
    };

    tcp::acceptor acceptor_;
};

class Client {
public:
    Client(boost::asio::io_context& io_context,
        const std::string& host,
        const std::string& port)
        : io_context_(io_context),
        socket_(io_context),
        resolver_(io_context) {
        connect(host, port);
    }

    void send_file(const std::string& filename) {
        try {
            // Проверка существования файла
            if (!std::filesystem::exists(filename)) {
                qDebug() << "File not found:" << filename.c_str();
                return;
            }

            // Чтение файла
            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            if (!file) {
                qDebug() << "Cannot open file:" << filename.c_str();
                return;
            }

            // Получение размера и чтение данных
            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);
            std::vector<char> buffer(size);

            if (!file.read(buffer.data(), size)) {
                qDebug() << "Read error:" << filename.c_str();
                return;
            }

            // Структура заголовка
            struct FileHeader {
                uint32_t filename_length;
                uint32_t content_length;
            } header;

            // Заполнение заголовка
            header.filename_length = static_cast<uint32_t>(filename.size());
            header.content_length = static_cast<uint32_t>(buffer.size());

            // Отправка с таймаутами
            boost::asio::write(socket_, boost::asio::buffer(&header, sizeof(header)));
            boost::asio::write(socket_, boost::asio::buffer(filename));
            boost::asio::write(socket_, boost::asio::buffer(buffer));

            // Добавляем задержку между файлами
            std::this_thread::sleep_for(std::chrono::milliseconds(50));

            qDebug() << "Sent:" << filename.c_str() << "Size:" << size << "bytes";
        }
        catch (const std::exception& e) {
            qDebug() << "Send error for" << filename.c_str() << ":" << e.what();
        }
    }

private:
    void connect(const std::string& host, const std::string& port) {
        boost::asio::connect(socket_, resolver_.resolve(host, port));
    }

    boost::asio::io_context& io_context_;
    tcp::socket socket_;
    tcp::resolver resolver_;
};

class shareWindow : public QWidget 
{
	Q_OBJECT
public:
	shareWindow();
private:
	QComboBox* roleComboBox;
	QLabel* label;
	QPushButton* confirmButton;
	void share();
    void sendFile(std::string fileName);
};

