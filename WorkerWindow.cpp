#include "TaskApp.h"

WorkerWindow::WorkerWindow()
{
    // Основной слей (вертикальный)
    QVBoxLayout* mainLayout = new QVBoxLayout(this);

    // Кнопка выполнения задачи
    QPushButton* CompleteButton = new QPushButton("Выполнить задачу");
    mainLayout->addWidget(CompleteButton);

    // Горизонтальный слой для таблицы и слайдера
    QHBoxLayout* tableSliderLayout = new QHBoxLayout();

    // Таблица задач
    QTableWidget* table = new QTableWidget(info.size, 2); // 5 строк, 3 столбца
    tableSliderLayout->addWidget(table, 1); // Растягиваем таблицу

    //Заполнение таблицы с задачами
    for (int i=0; i<info.size; i++) 
    {
        table->setItem(i, 0, new QTableWidgetItem(info.tasks[i][1]));
        table->setItem(i, 1, new QTableWidgetItem(info.tasks[i][2]));
    }


    // Вертикальный инвертированный слайдер
    QSlider *slider = new QSlider(Qt::Vertical);
    slider->setInvertedAppearance(true);  // Инвертировать значения (сверху — минимум, снизу — максимум)
    slider->setRange(0, info.size);            // Установка диапазона
    tableSliderLayout->addWidget(slider); // Слайдер справа от таблицы

    mainLayout->addLayout(tableSliderLayout); // Добавляем таблицу + слайдер в основной слой

    // Строка с выбранной задачей
    label = new QLabel("Текущая задача: ");
    mainLayout->addWidget(label);

    // Минимальный размер окна
    setMinimumSize(600, 400);

    connect(slider, &QSlider::valueChanged, this, &WorkerWindow::updateLabel);
}


WorkerWindow::tasks::tasks() 
{

}

void WorkerWindow::tasks::complete()
{
}

void WorkerWindow::save() 
{

}

void WorkerWindow::updateLabel(int value)
{
    label->setText(QString("Текущая задача : %1").arg(value));
}