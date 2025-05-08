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
    QTableWidget* table = new QTableWidget(5, 3); // 5 строк, 3 столбца
    tableSliderLayout->addWidget(table, 1); // Растягиваем таблицу

    // Вертикальный инвертированный слайдер
    QSlider* slider = new QSlider(Qt::Vertical);
    slider->setInvertedAppearance(true);  // Инвертировать значения (сверху — минимум, снизу — максимум)
    slider->setRange(0, 100);            // Установка диапазона
    tableSliderLayout->addWidget(slider); // Слайдер справа от таблицы

    mainLayout->addLayout(tableSliderLayout); // Добавляем таблицу + слайдер в основной слой

    // Строка с выбранной задачей
    QLabel* label = new QLabel("Текущая задача: ");
    mainLayout->addWidget(label);

    // Минимальный размер окна
    setMinimumSize(600, 400);
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