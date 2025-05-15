#include "TaskApp.h"

WorkerWindow::WorkerWindow(QString name)
{
    // Основной слей (вертикальный)
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Кнопка выполнения задачи
    QPushButton* CompleteButton = new QPushButton("Выполнить задачу");
    mainLayout->addWidget(CompleteButton);

    //Кнопка отчистки вывполнений текущей задачи

    QPushButton* ClearButton = new QPushButton("Отчистить выполнения текущей задачи");
    mainLayout->addWidget(ClearButton);

    // Горизонтальный слой для таблицы и слайдера
    QHBoxLayout* tableSliderLayout = new QHBoxLayout();

    //вектор с id задач доступных пользователю
    qDebug() << "WorkerWindow";
    qDebug() << name;
    qDebug() << WorkersTasks.workersTasks;
    for (int i = 0; i < WorkersTasks.workersTasks.size(); ++i)
    {
        if (WorkersTasks.workersTasks[i][0] == name)
            forThisName = WorkersTasks.workersTasks[i];
    }
    //qDebug() << forThisName;
    // Таблица задач
    if (forThisName.size() == 0) 
    {
        table = new QTableWidget(1, 1);
        table->setItem(0, 0, new QTableWidgetItem("У вас нет задач"));
        
    }
    else 
    {
        table = new QTableWidget(forThisName.size()-1 , 4);
        table->setHorizontalHeaderLabels({ "Название задачи", "Текст задачи", "Количество выполнений задачи", "Время последнего выполнения задачи" });
        tableSliderLayout->addWidget(table, 1); // Растягиваем таблицу
        table->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //Заполнение таблицы с задачами

        reEnterTable();
    }
    // Вертикальный инвертированный слайдер
    slider = new QSlider(Qt::Vertical);
    slider->setInvertedAppearance(true);  // Инвертировать значения (сверху — минимум, снизу — максимум)
    slider->setRange(1, info.size);    
    slider->setValue(1);
    // Установка диапазона
    tableSliderLayout->addWidget(slider); // Слайдер справа от таблицы

    mainLayout->addLayout(tableSliderLayout); // Добавляем таблицу + слайдер в основной слой

    // Строка с выбранной задачей
    label = new QLabel("Текущая задача: 1");
    mainLayout->addWidget(label);

    // Минимальный размер окна
    setMinimumSize(700, 400);

    connect(slider, &QSlider::valueChanged, this, &WorkerWindow::updateLabel);
    connect(CompleteButton, &QPushButton::clicked, this, &WorkerWindow::save);
    connect(ClearButton, &QPushButton::clicked, this, &WorkerWindow::clear);
}

tasks::tasks() 
{
    QFile file("CompletionInfo.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "error123";

    QTextStream in(&file);
    QString str = in.readAll();
    for (QString elem : str.split("~~~")) 
    {
        completedTasks.append(elem.split("^^"));
    }
    file.close();
    deleteTrash();
}

void tasks::complete(QString id)
{
    //текущее время - время выполнения задачи
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    completedTasks.append({ id, currentTime });
    save();
}

void tasks::save() 
{
    QFile file("CompletionInfo.txt");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
        qDebug() << "error";

    QTextStream out(&file);
    for (int i = 0; i < completedTasks.size(); ++i)
    {
        if (i != completedTasks.size() - 1)
            out << completedTasks[i][0] << "^^" << completedTasks[i][1] << "~~~";
        else
            out << completedTasks[i][0] << "^^" << completedTasks[i][1];
    }
}

void tasks::clear(QString id) 
{
    for (int i = 0; i < completedTasks.size(); )
    {
        if (completedTasks[i][0] == id)
        {
            completedTasks.remove(i);  // QVector автоматически сдвигает элементы
        }
        else
        {
            ++i;
        }
    }
    save();
}

int  tasks::count(QString id) 
{
    int count = 0;
    for (int i = 0; i < completedTasks.size(); ++i)
    {
        if (completedTasks[i][0] == id)
        {
            count++;
        }
    }
    return count;
}

QString  tasks::last(QString id) 
{
    QString lastTime="ещё не выполнено";
    for (int i = 0; i < completedTasks.size(); ++i)
    {
        if (completedTasks[i][0] == id)
        {
            lastTime = completedTasks[i][1];
        }
    }
    return lastTime;
}

void WorkerWindow::save() 
{
    QString id = info.tasks[slider->value()-1][0];
    completedTasks.complete(id);
    reEnterTable();
}

void WorkerWindow::updateLabel(int value)
{
    label->setText(QString("Текущая задача : %1").arg(value));
}

void WorkerWindow::clear() 
{
    QString id = info.tasks[slider->value()-1][0];
    completedTasks.clear(id);
    reEnterTable();
}

void WorkerWindow::reEnterTable()
{
    int index = 0;
    for (int i = 0; i < info.size; i++)
    {
        bool flag = 0;
        for (int j = 1;j < forThisName.size() - 1; ++j)
        {
            if (forThisName[j] == info.tasks[i][0])
                flag = 1;
        }
        if (flag == 1) 
        {
            table->setItem(index, 0, new QTableWidgetItem(info.tasks[i][1]));
            table->setItem(index, 1, new QTableWidgetItem(info.tasks[i][2]));
            table->setItem(index, 2, new QTableWidgetItem(QString::number(completedTasks.count(info.tasks[i][0]))));
            table->setItem(index, 3, new QTableWidgetItem(completedTasks.last(info.tasks[i][0])));
            index++;
        }
    }
    table->resizeColumnsToContents();

}

void tasks::deleteTrash() 
{   
    // актуальные задачи

    QVector<QString> actualIds;

    for (int i = 0; i < file.size; ++i) 
    {
        actualIds.append(file.tasks[i][0]);
    }
    // удваление всех записей, посвященных неактуальным записям
    for (int i = 0; i < completedTasks.size(); ++i) 
    {
        bool actualFlag = 0;
        for (int j = 0; j < actualIds.size(); ++j) 
        {
            if (actualIds[j] == completedTasks[i][0]) 
            {
                actualFlag = 1;
            }
        }
        if (actualFlag == 0)
        {
            completedTasks.remove(i);
        }
    }
    save();
}