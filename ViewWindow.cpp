#include "TaskApp.h"

viewWindow::viewWindow()
{
    // Основной вертикальный layout (теперь включает статусную строку)
    QVBoxLayout* mainVerticalLayout = new QVBoxLayout(this);

    // Горизонтальный layout для основной части (кнопки + таблица + слайдер)
    QHBoxLayout* mainHorizontalLayout = new QHBoxLayout();

    //Создаём файл с заадчами
    info = new TextFile;

    // 1. Левая часть: вертикальные кнопки
    QVBoxLayout* buttonLayout = new QVBoxLayout();

    QPushButton* button1 = new QPushButton("Добавить", this);
    QPushButton* button2 = new QPushButton("Удалить", this);
    QPushButton* button3 = new QPushButton("Сохранить", this);

    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);
    buttonLayout->addWidget(button3);
    buttonLayout->addStretch();

    // 2. Центральная часть: таблица
    table = new QTableWidget(this);
    table->setRowCount(info->size);
    table->setColumnCount(2);
    table->setHorizontalHeaderLabels({ "Название задачи", "Текст задачи" });

    // Заполняем таблицу 
    for (int i = 0; i < info->size; ++i)
    {
        table->setItem(i, 0, new QTableWidgetItem(info->tasks[i][1]));
        table->setItem(i, 1, new QTableWidgetItem(info->tasks[i][2]));
    }
    // 3. Правая часть: слайдер (инвертированный)
    slider = new QSlider(Qt::Vertical, this);
    slider->setRange(1, info->size);  // обычный диапазон
    slider->setValue(1);
    slider->setInvertedAppearance(true);  // 0 сверху, 100 снизу

    // Группируем таблицу и слайдер
    QHBoxLayout* tableSliderLayout = new QHBoxLayout();
    tableSliderLayout->addWidget(table);
    tableSliderLayout->addWidget(slider);

    // Добавляем левую и центральную части в горизонтальный layout
    mainHorizontalLayout->addLayout(buttonLayout);
    mainHorizontalLayout->addLayout(tableSliderLayout);

    // 4. Статусная строка внизу
    statusLabel = new QLabel(this);
    updateStatusLabel(slider->value()); // Устанавливаем начальное значение

    // Добавляем все в основной вертикальный layout
    mainVerticalLayout->addLayout(mainHorizontalLayout);
    mainVerticalLayout->addWidget(statusLabel);

    // Подключаем сигнал слайдера к обновлению надписи
    connect(slider, &QSlider::valueChanged, this, &viewWindow::updateStatusLabel);
    connect(button1, &QPushButton::clicked, this, &viewWindow::add);
    connect(button2, &QPushButton::clicked, this, &viewWindow::remove);
    connect(button3, &QPushButton::clicked, this, &viewWindow::edit);
    setLayout(mainVerticalLayout);
}


void viewWindow::updateStatusLabel(int value) {
    statusLabel->setText(QString("Текущая задача : %1").arg(value));
}

void viewWindow::add()
{
    info->add("Название новой задачи","Текст новой задачи");
    int newRow = table->rowCount(); 
    table->insertRow(newRow);
    //qDebug() << info->tasks[2];
    table->setItem(newRow, 0, new QTableWidgetItem(info->tasks[info->size-1][1]));
    table->setItem(newRow, 1, new QTableWidgetItem(info->tasks[info->size-1][2]));
    slider->setRange(1, info->size);  // обновляем диапозон слайдера
}
void viewWindow::remove()
{
    QString idToRemove;
    int currentNumber = slider->value(); // значение номера текущей выбранной строки
    idToRemove = info->tasks[currentNumber - 1][0];
    info->remove(idToRemove);
    table->removeRow(currentNumber-1);
    slider->setRange(1, info->size);  // обновляем диапозон слайдера
}
void viewWindow::edit()
{
      QVector<QVector<QString>> newInfo;
    for (int i = 0; i < info->size; ++i) 
    {
        QTableWidgetItem* item1 = table->item(i,0);
        QTableWidgetItem* item2 = table->item(i, 1);
        info->edit(info->tasks[i][0], item1->text(), item2->text());
    }
}

