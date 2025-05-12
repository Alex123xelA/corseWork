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
	//void openPage();
	QVector<QVector<QString>> pages;
private slots:
	//void nextPage();
	//void previousPage();
private:

};