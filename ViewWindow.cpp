#include "TaskApp.h"
#include "asioModule.h"

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
    QPushButton* button4 = new QPushButton("Открыть окно\nредактирования пользователей", this);
    QPushButton* buttonShare = new QPushButton("Открыть окно\nотправки данных", this);

    buttonLayout->addWidget(button1);
    buttonLayout->addWidget(button2);
    buttonLayout->addWidget(button3);
    buttonLayout->addWidget(button4);
    buttonLayout->addWidget(buttonShare);
    buttonLayout->addStretch();

    // 2. Центральная часть: таблица
    table = new QTableWidget(this);
    table->setRowCount(info->size);
    table->setColumnCount(4);
    table->setHorizontalHeaderLabels({ "Название задачи", "Текст задачи", "Количество выполнений задачи", "Последнее выполнение задачи"});

    // Заполняем таблицу 
    for (int i = 0; i < info->size; ++i)
    {
        table->setItem(i, 0, new QTableWidgetItem(info->tasks[i][1]));
        table->setItem(i, 1, new QTableWidgetItem(info->tasks[i][2]));
        for (int j = 0; j < tasks.completedTasks.size(); ++j) 
        {
            if (info->tasks[i][0] == tasks.completedTasks[j][0]) 
            {
                table->setItem(i, 2, new QTableWidgetItem(QString::number(tasks.count(info->tasks[i][0]))));
                table->setItem(i, 3, new QTableWidgetItem(tasks.last(info->tasks[i][0])));
                break;
            }
        }
    }
    table->resizeColumnsToContents();

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

    // Настройки окна
    setWindowTitle("Окно редактирования задач");
    resize(850, 400);

    // Подключаем сигнал слайдера к обновлению надписи
    connect(slider, &QSlider::valueChanged, this, &viewWindow::updateStatusLabel);
    connect(button1, &QPushButton::clicked, this, &viewWindow::add);
    connect(button2, &QPushButton::clicked, this, &viewWindow::remove);
    connect(button3, &QPushButton::clicked, this, &viewWindow::edit);
    connect(button4, &QPushButton::clicked, this, &viewWindow::showUsersEditor);
    connect(buttonShare, &QPushButton::clicked, this, &viewWindow::showShareWindow);
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
void viewWindow::showUsersEditor() 
{
    UsersEditor* window = new UsersEditor();  // Создаем окно в куче
    window->show();                        // Показываем окно
}

void viewWindow::showShareWindow() 
{
    shareWindow* window = new shareWindow();
    window->setWindowTitle("Принять/получить данные задач и пользователей");
    window->show();
}