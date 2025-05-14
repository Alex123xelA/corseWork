#pragma once

#include <QMainWindow>
#include <QWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QTextEdit>
#include <QFile>
#include <QDebug>

class UserGuide : public QMainWindow
{
	Q_OBJECT
public:
	UserGuide();
	void openPage(int page);
	QVector<QVector<QString>> pages;
	int currentPage;
private slots:
	void next();
	void previous();
private:
	QTextEdit* textEdit;
	QLabel* titleLabel;
};