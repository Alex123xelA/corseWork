#include "TaskApp.h"

WorkerWindow::WorkerWindow()
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

    // Таблица задач
    table = new QTableWidget(info.size, 4); // 5 строк, 3 столбца
    table->setHorizontalHeaderLabels({ "Название задачи", "Текст задачи", "Количество выполнений задачи", "Время последнего выполнения задачи"});
    tableSliderLayout->addWidget(table, 1); // Растягиваем таблицу

    //Заполнение таблицы с задачами
 
    reEnterTable();

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

WorkerWindow::tasks::tasks() 
{
    QFile file("CompletionInfo.txt");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        qDebug() << "error";

    QTextStream in(&file);
    QString str = in.readAll();
    for (QString elem : str.split("~~~")) 
    {
        completedTasks.append(elem.split("^^"));
    }
    file.close();
}

void WorkerWindow::tasks::complete(QString id)
{
    //текущее время - время выполнения задачи
    QString currentTime = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");

    completedTasks.append({ id, currentTime });
    save();
}

void WorkerWindow::tasks::save() 
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

void WorkerWindow::tasks::clear(QString id) 
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

int  WorkerWindow::tasks::count(QString id) 
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

QString  WorkerWindow::tasks::last(QString id) 
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
    for (int i = 0; i < info.size; i++)
    {
        table->setItem(i, 0, new QTableWidgetItem(info.tasks[i][1]));
        table->setItem(i, 1, new QTableWidgetItem(info.tasks[i][2]));
        table->setItem(i, 2, new QTableWidgetItem(QString::number(completedTasks.count(info.tasks[i][0]))));
        table->setItem(i, 3, new QTableWidgetItem(completedTasks.last(info.tasks[i][0])));
    }
    table->resizeColumnsToContents();

}