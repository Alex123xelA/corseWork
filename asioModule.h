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
    void start();
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
                // Открываем файл в режиме добавления
                std::ofstream file(filename_, std::ios::app | std::ios::binary);
                if (file) {
                    file.write(file_content_.data(), file_content_.size());
                    file.close();
                    /*std::cout << "Обработан файл: " << filename_
                        << ", добавлено " << file_content_.size() << " байт\n";*/
                }
                else {
                    std::cerr << "Не удалось открыть файл: " << filename_ << "\n";
                }
            }
            catch (const std::exception& e) {
                std::cerr << "Ошибка обработки файла: " << e.what() << "\n";
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
            // Читаем содержимое файла
            std::ifstream file(filename, std::ios::binary | std::ios::ate);
            if (!file) {
                qDebug() << "can not open file: " << filename << "\n";
                return;
            }

            std::streamsize size = file.tellg();
            file.seekg(0, std::ios::beg);

            std::vector<char> buffer(size);
            if (!file.read(buffer.data(), size)) {
                qDebug() << "can not read file: " << filename << "\n";
                return;
            }

            // Подготавливаем заголовок
            struct FileHeader {
                uint32_t filename_length;
                uint32_t content_length;
            } header;

            header.filename_length = static_cast<uint32_t>(filename.size());
            header.content_length = static_cast<uint32_t>(buffer.size());

            // Отправляем данные
            std::vector<boost::asio::const_buffer> buffers;
            buffers.push_back(boost::asio::buffer(&header, sizeof(header)));
            buffers.push_back(boost::asio::buffer(filename));
            buffers.push_back(boost::asio::buffer(buffer));

            boost::asio::write(socket_, buffers);

            qDebug() << "file sended: " << filename
                << ", size: " << size << " bytes\n";
        }
        catch (const std::exception& e) {
            qDebug() << "send error: " << e.what() << "\n";
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
};

