#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_TaskApp.h"
#include <QPushButton>
#include <QLineEdit>
#include <QVBoxLayout>
#include <QWidget>  
#include <QTextEdit>
#include <QString>
#include <QFile>
#include <QVector>
#include <QPair>
#include <QTableWidget>
#include <QRandomGenerator>
#include <QHBoxLayout>
#include <iostream>
class TaskApp : public QMainWindow
{
    Q_OBJECT

public:
    TaskApp(QWidget* parent = nullptr);
    ~TaskApp();

private slots:
    void enterButtonClicked();

private:
    QPushButton* enterButton;
    QLineEdit* name;
    QLineEdit* password;
    Ui::TaskAppClass ui;
    QWidget* centralWidget;  

    QString strName;
    QString strPassword;

};

class passwordsAndNames {

public:
    int isCorrect(QString name, QString password);
private:
    QString openFile();
    QVector<QVector<QString>> passwordsNames();
};

class viewWindow : public QWidget
{
    Q_OBJECT
public:
    viewWindow(QWidget* parent = nullptr);
    ~viewWindow();
    //void createViwerWindow();
    void createEditorWindow();
private slots:
private:
    void onAddButtonClicked();
    void onDeleteButtonClicked();
    void onSaveButtonClicked();
    QString generateUniqueID();
    void loadTasksToTable();
    QTableWidget* table;
    
};

class TextFile {
public:
    void add(QString newName, QString newText);
    void remove(QString id);
    void edit(QString id, QString newName, QString newText);
    TextFile();
    QVector<QVector<QString>> tasks;
    int size;
    //tasks[index][0] - id, tasks[index][1] - name, tasks[index][2] - text
private:
    QString createNewId();
    void update();
    void save();
};