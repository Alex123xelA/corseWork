#include "TaskApp.h"

void TextFile::add(QString newName, QString newText)
{
    QString newId = createNewId();
    tasks.append(QVector<QString>{newId, newName, newText});
    save();
}

void TextFile::remove(QString id)
{
    int index = 0;
    for (int i = 0; i < this->size; ++i)
    {
        if (this->tasks[i][0] == id)
            index = i;
    }
    this->tasks.removeAt(index);
    this->size = this->size - 1;
    save();
}

void TextFile::edit(QString id, QString newName, QString newText)
{
    for (int i = 0; i < this->size; ++i)
    {
        if (this->tasks[i][0] == id)
        {
            this->tasks[i][1] = newName;
            this->tasks[i][2] = newText;
        }
    }
    save();
}

QString TextFile::createNewId()
{
    QString newId = " ";
    QString symbols = "0123456789QWERTYUIOPASDFGHJKLZXCVBNMqwertyuiopasdfghjklzxcvbnm";
    int index = 0;
    int idIndex = 0;
    bool flagUniq = 0;
    while (flagUniq != 1)
    {
        if (index == symbols.size() - 1) {
            index = 0;
            newId[idIndex] = symbols[0];
            idIndex++;
        }
        newId[idIndex] = symbols[index];
        index++;
        flagUniq = 1;
        for (int i = 0; i < this->size; ++i)
        {
            if (this->tasks[i][0] == newId)
            {
                flagUniq = 0;
            }
        }
    }
    return newId;
}

void TextFile::update()
{
    QFile file("Tasks.txt");

    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        std::cout << "Error" << std::endl;
    QString str = file.readAll();
    this->size = 0;
    this->tasks.clear();
    for (QString elem : str.split("~~~"))
    {
        this->tasks.append(elem.split("^^"));
        this->size++;
    }
}

void TextFile::save()
{
    QFile file("Tasks.txt");

    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        std::cout << "Error" << std::endl;
    QTextStream save(&file);
    for (int i = 0; i < this->size + 1; ++i)
    {
        save << this->tasks[i].join("^^");
        if (i != this->size)
            save << "~~~";
    }
}

//конструктор класса файла с задачами

TextFile::TextFile()
{
    update();
    qDebug() << createNewId();
}

