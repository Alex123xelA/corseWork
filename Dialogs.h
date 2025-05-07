#pragma once
//#include "TaskApp.h"//возможно не надо
#include <QDialog>
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>


class addUserDialog : public QDialog
{
	Q_OBJECT
public:
	addUserDialog(QWidget* parent = nullptr) : QDialog(parent){}
private slots:

private:
	QLineEdit* nameLineEdit;
	QLineEdit* passwordLineEdit1;
	QLineEdit* passwordLineEdit2;
	QPushButton* button;
};