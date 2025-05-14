#include "asioModule.h"

using boost::asio::ip::tcp;

shareWindow::shareWindow() 
{
	// создание слоя для всех виджетов

	QVBoxLayout* mainLayout = new QVBoxLayout(this);

	// создание надписи с подсказкой

	label = new QLabel("Вберите режим:", this);
	mainLayout->addWidget(label);

	// создание элемента QComboBox для выбора режима работы

	roleComboBox = new QComboBox(this);
	roleComboBox->addItem("Получать данные");
	roleComboBox->addItem("Отправлять данные");
	mainLayout->addWidget(roleComboBox);

	// создание кнопки начала приема/передачи

	confirmButton = new QPushButton("Подтвердить", this);
	mainLayout->addWidget(confirmButton);

	//подключение кнопки

	connect(confirmButton, &QPushButton::clicked, this, &shareWindow::share);
}
/*
void shareWindow::share() 
{
	if (roleComboBox->currentText() == "Получать данные") 
	{
		try {
			boost::asio::io_context io_context;
			Server server(io_context, 1234);
			std::cout << "Сервер запущен на порту 1234\n";
			io_context.run();
		}
		catch (const std::exception& e) {
			std::cerr << "Исключение: " << e.what() << "\n";
		}
		qDebug() << "server";
	}
	else 
	{
		try {
			boost::asio::io_context io_context;
			Client client(io_context, "127.0.0.1", "1234");

			// Список файлов для отправки
			std::vector<std::string> files = {
				
				"Users.txt",
				"Tasks.txt",
				"WorkersTasks.txt",
				"CompleteInfo.txt"
			};

			// Отправляем каждый файл
			for (const auto& file : files) {
				qDebug() << file;
				client.send_file(file);
			}
		}
		catch (const std::exception& e) {
			qDebug() << "exception: " << e.what() << "\n";
		}
		qDebug() << "client";
	}
}*/

void shareWindow::share() {
	if (roleComboBox->currentText() == "Получать данные") {
		std::thread([this]() {
			try {
				boost::asio::io_context io_context;
				Server server(io_context, 1234);
				qDebug() << "Server started on port 1234";
				io_context.run();
			}
			catch (const std::exception& e) {
				qDebug() << "Server exception:" << e.what();
			}
			}).detach();
	}
	else {
		sendFile("Tasks.txt");
		sendFile("Users.txt");
		sendFile("CompletionInfo.txt");
		sendFile("WorkersTasks.txt");

	}
}

void shareWindow::sendFile(std::string fileName) 
{
	std::thread([this, fileName]() {
		try {
			boost::asio::io_context io_context;
			Client client(io_context, "127.0.0.1", "1234");
			std::vector<std::string> files = {
				fileName
			};

			for (const auto& file : files) {
				try {
					client.send_file(file);
				}
				catch (...) {
					qDebug() << "Error sending file:" << file.c_str();
				}
				std::this_thread::sleep_for(std::chrono::milliseconds(100));
			}
		}
		catch (const std::exception& e) {
			qDebug() << "Client exception:" << e.what();
		}
		}).detach();
}