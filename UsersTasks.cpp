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
    bool flagNewName = 1;
    int index = 0;
    qDebug()<<"WT" << workersTasks;
    for (int i = 0; i < workersTasks.size(); ++i)
    {
        if (workersTasks[i][0] == name)
        {
            flagNewName = 0;
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
    if (flagNewName == 1) 
    {
        workersTasks.append({name, id});
    }
    else if (flagUniqe == 1)
    {
        workersTasks[index].append(id);
    }
    saveChanges();
}
void WorkersTasks::remove(QString name, QString id)
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
        workersTasks.append({ name, id });
    }


    saveChanges();
}
void WorkersTasks::saveChanges()
{
    QFile file("WorkersTasks.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";
    //qDebug() << "DATA" << workersTasks;
    QTextStream save(&file);
    bool flagFirst = 1;
    for (QVector<QString> elem : workersTasks)
    {
        //qDebug() << "ELEM" << elem;
        for (int i = 1; i < elem.size(); ++i)
        {
            if (flagFirst == 1) 
            {
                save << elem[0] << "^^" << elem[i];
                flagFirst = 0;
            }
            else
                save <<"~~~" << elem[0] << "^^" << elem[i];
        }
    }

}
