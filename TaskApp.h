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
#include <QLabel>
#include <QDateTime>
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

class viewWindow : public QWidget
{
    Q_OBJECT
public:
    viewWindow();
private slots:
    void add();
    void remove();
    void edit();
    void showUsersEditor();
private:
    void updateStatusLabel(int value);
    TextFile* info;
    QLabel* statusLabel;
    QSlider* slider;
    QTableWidget* table;
};

class UsersFile
{
public:
    UsersFile();
    void add(QString name, QString password, QString status);
    void remove(QString name);
    void edit(QString oldName, QString newName, QString password, QString status);
    int size;
    QVector<QVector<QString>> users;
private:
    void saveChanges();
};

class WorkersTasks
{
public:
    WorkersTasks();
    void add(QString name, QString id);
    void remove(QString name, QString id);
    QVector<QVector<QString>> workersTasks;
private:
    void saveChanges();
};

class UsersEditor : public QWidget
{
    Q_OBJECT
public:
    UsersEditor();
    UsersFile users;
    QTableWidget* WorcersTable;
    QTableWidget* ManagersTable;
    QSlider* WorkersSlider;
    QSlider* ManagersSlider;
    QLabel* statusLabel;
    QSlider* topSlider;
    WorkersTasks data;
private slots:
    void addTask();
    void addUser();
    void remote();
    void edit();
    void updateStatusLabel();
};

class WorkerWindow : public QWidget
{
    Q_OBJECT
private:
    class tasks
    {
    public:
        tasks();
        QVector<QVector<QString>> completedTasks;
        TextFile file;
        void complete(QString id);
        int count(QString id);
        QString last(QString id);
        void clear(QString id);
        void save();
    };
    void reEnterTable();
    QTableWidget* table;
public:
    QString name;
    WorkerWindow();
    TextFile info;
    QSlider* slider;
    QLabel* label;
    tasks completedTasks;

private slots:
    void save();
    void updateLabel(int value);
    void clear();
};
