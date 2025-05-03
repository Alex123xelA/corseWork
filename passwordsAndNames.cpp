#include "TaskApp.h"
#include <QDebug>

int passwordsAndNames::isCorrect(QString name, QString password) {
	int status = 3;
	// 0 - верно - руководитель
	// 1 - верно - работник
	// 3 - не верно
	QVector<QVector<QString>> info = passwordsNames(); // Получаем данные из файла

	for (QVector<QString> elem : info) {
		if (elem[0] == name && elem[1] == password) {
			if (elem[2] == "0") {  
				status = 0;
			}
			else if (elem[2] == "1") {
				status = 1;
			}
			break; // Если нашли совпадение, можно выйти из цикла
		}
	}

	return status;
}

QVector<QVector<QString>> passwordsAndNames::passwordsNames() {
	QString str = openFile();
	QVector<QVector<QString>> result;

	for (QString elem : str.split("~~~")) {
		QVector<QString> record;
		QStringList parts = elem.split("^^");
		if (parts.size() >= 3) { // Проверяем, что есть все необходимые части
			record.append(parts[0]); // ИМЯ
			record.append(parts[1]); // ПАРОЛЬ
			record.append(parts[2]); // СТАТУС
			result.append(record);
			qDebug() << "Loaded record:" << record;
		}
	}

	return result;
}

QString passwordsAndNames::openFile() {
	QFile file("Users.txt");
	if (!file.open(QIODevice::ReadOnly | QIODevice::Text)) {
		qDebug() << "Ошибка открытия файла";
		return "";
	}
	QString str = file.readLine();
	file.close();
	return str;
}