#include "TaskApp.h"
#include <QDir>
WorkersTasks::WorkersTasks()
{
    QFile file("WorkersTasks.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "Текущая рабочая директория:" << QDir::currentPath();
        qDebug() << file.errorString();
        qDebug() << "error";
    QString str = file.readAll();
    bool flagAdd;
    for (QString elem : str.split("~~~"))
    {
        flagAdd = 0;
        QVector<QString> array = elem.split("^^");
        for (int i = 0; i < workersTasks.size(); ++i)
        {
            if (workersTasks[i][0] == array[0])
            {
                flagAdd = 1;
                workersTasks[i].append(array[1]);
                break;
            }
        }
        if (flagAdd == 0)
        {
            workersTasks.append(array);
        }
    }
    qDebug() << workersTasks;
}
void WorkersTasks::add(QString name, QString id)
{
    bool flagUniqe = 1;
    int index = 0;
  
    for (int i = 0; i < workersTasks.size(); ++i)
    {
        if (workersTasks[i][0] == name)
        {
            index = i;
            for (int j = 0; j < workersTasks[i].size(); ++j)
            {
                if (workersTasks[i][j] == id)
                {
                    flagUniqe = 0;
                    break;
                }
            }
        }
    }
    if (flagUniqe == 1)
    {
        workersTasks[index].append(id);
    }
    saveChanges();
}
void WorkersTasks::remove(QString name, QString id)//это надо переделать/////////////////////////////////////////////////////////////////////////////////////////////
{
    bool emptyFlag = 1;// флаг того, что у пользователя нет задач
    for (int i = 0; i < workersTasks.size(); ++i)
    {
        if (workersTasks[i][0] == name)
        {
            emptyFlag = 0;
            for (int j = 0; j < workersTasks[i].size(); ++j)
            {
                if (workersTasks[i][j] == id)
                {
                    workersTasks[i].remove(j);
                    break;
                }
            }
        }
    }
    if (emptyFlag == 1) 
    {
        workersTasks.append({name, id});
    }
    qDebug() << workersTasks;
    saveChanges();
}
void WorkersTasks::saveChanges()
{
    QFile file("WorkersTasks.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";
    qDebug() << "DATA" << workersTasks;
    QTextStream save(&file);
    for (QVector<QString> elem : workersTasks)
    {
        qDebug() << "ELEM" << elem;
        for (int i = 1; i < elem.size(); ++i)
        {
            if (i == 1)
                save << elem[0] << "^^" << elem[i] ;
            else
                save <<"~~~" << elem[0] << "^^" << elem[i];
        }
    }

}
