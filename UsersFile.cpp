#include "TaskApp.h"

void UsersFile::saveChanges()
{
    QFile file("users.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "Error whith open sorse file";
    QTextStream usersSorseFile(&file);

    for (int i = 0; i < size; ++i)
    {
        //qDebug() << users[i].join("^^");
        usersSorseFile << users[i].join("^^");
        if (i != size - 1)
            usersSorseFile << "~~~";
    }
}

UsersFile::UsersFile()
{
    QFile file("users.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "Error whith open sorse file";
    size = 0;
    QTextStream usersSorseFile(&file);
    QString str = usersSorseFile.readAll();
    for (QString elem : str.split("~~~"))
    {
        users.append(elem.split("^^"));
        size++;
    }
    qDebug() << users;
}
void UsersFile::add(QString name, QString password, QString status)
{
    bool flagUniqe = 1;
    for (int i = 0; i < size; ++i)
    {
        if (users[i][0] == name) flagUniqe = 0;
    }
    if (flagUniqe == 0)
        qDebug() << "Error: name of user have to be uniqe";
    //дописать окно с сообщением
    else
    {
        QVector<QString> newUser;
        newUser.append(name);
        newUser.append(password);
        newUser.append(status);
        users.append(newUser);
        size++;
        qDebug() << users;
        saveChanges();
    }
}
void UsersFile::remove(QString name)
{
    for (int i = 0; i < size; ++i)
    {
        if (users[i][0] == name)
        {
            users.remove(i);
            size--;
            break;
        }
    }
    saveChanges();
}
void UsersFile::edit(QString oldName, QString newName, QString password, QString status)
{
    if (oldName != newName)
    {
        int indexOld = -1;
        bool flagUniqe = 1;
        for (int i = 0; i < size; ++i)
        {
            if (users[i][0] == oldName)
                indexOld = i;
            if (users[i][0] == newName)
                flagUniqe = 0;
        }
        if (flagUniqe == 1)
        {
            users[indexOld][0] = newName;
            users[indexOld][1] = password;
            users[indexOld][2] = status;
        }
        else
        {
            qDebug() << "Error: name of user have to be uniqe";
            //дописать окно с сообщением
        }
    }
    else
    {
        for (int i = 0; i < size; ++i)
        {
            if (users[i][0] == oldName)
            {
                users[i][1] = password;
                users[i][2] = status;
            }
        }
    }
    saveChanges();
}
