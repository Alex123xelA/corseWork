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
				"file1.txt",
				"file2.txt",
				"file3.txt",
				"file4.txt"
			};

			// Отправляем каждый файл
			for (const auto& file : files) {
				client.send_file(file);
			}
		}
		catch (const std::exception& e) {
			qDebug() << "exception: " << e.what() << "\n";
		}
		qDebug() << "client";
	}
}