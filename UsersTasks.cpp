#include "TaskApp.h"

WorkersTasks::WorkersTasks()
{
    QFile file("WorkersTasks.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
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
    qDebug() << workersTasks;
    saveChanges();
}
void WorkersTasks::remove(QString name, QString id)
{
    for (int i = 0; i < workersTasks.size(); ++i)
    {
        if (workersTasks[i][0] == name)
        {
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
    qDebug() << workersTasks;
    saveChanges();
}
void WorkersTasks::saveChanges()
{
    QFile file("WorkersTasks.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";

    QTextStream save(&file);
    for (QVector<QString> elem : workersTasks)
    {
        for (int i = 1; i < elem.size(); ++i)
        {
            if (i != elem.size() - 1)
                save << elem[0] << "^^" << elem[i] << "~~~";
            else
                save << elem[0] << "^^" << elem[i];
        }
    }

}