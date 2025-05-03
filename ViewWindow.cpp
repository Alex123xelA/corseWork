#include "TaskApp.h"

viewWindow::viewWindow(QWidget* parent)
    : QWidget(parent)
{
    createEditorWindow();
}
viewWindow::~viewWindow() {

}

void viewWindow::createEditorWindow()
{
    // Основной горизонтальный layout
    QHBoxLayout* mainLayout = new QHBoxLayout(this);

    // 1. Левая часть - столбец с кнопками
    QVBoxLayout* leftLayout = new QVBoxLayout();

    QPushButton* addButton = new QPushButton("Добавить", this);
    QPushButton* deleteButton = new QPushButton("Удалить", this);
    QPushButton* saveButton = new QPushButton("Сохранить", this);

    // Добавляем кнопки в левый layout с отступами
    leftLayout->addWidget(addButton);
    leftLayout->addWidget(deleteButton);
    leftLayout->addWidget(saveButton);
    leftLayout->addStretch(); // Добавляем растягивающееся пространство

    // Устанавливаем отступы и промежутки между кнопками
    leftLayout->setSpacing(10);
    leftLayout->setContentsMargins(10, 10, 10, 10);

    // 2. Правая часть - таблица
    table = new QTableWidget(0, 3, this); // 3 столбца (ID, Название, Текст), строки будут добавляться
    table->setHorizontalHeaderLabels({ "ID", "Название задачи", "Текст задачи" });
    table->setColumnHidden(0, true); // Скрываем колонку с ID

    // Загружаем данные из файла
    loadTasksToTable();

    // Настройка таблицы
    table->setAlternatingRowColors(true);
    table->setStyleSheet("QTableView { alternate-background-color: #f0f0f0; }");

    // Добавляем левую и правую части в основной layout
    mainLayout->addLayout(leftLayout, 1); // Левая часть занимает 1 часть пространства
    mainLayout->addWidget(table, 3);      // Таблица занимает 3 части пространства

    // Настройки основного окна
    setWindowTitle("Редактор задач");
    setMinimumSize(600, 400);

    // Подключаем сигналы кнопок
    connect(addButton, &QPushButton::clicked, this, &viewWindow::onAddButtonClicked);
    connect(deleteButton, &QPushButton::clicked, this, &viewWindow::onDeleteButtonClicked);
    connect(saveButton, &QPushButton::clicked, this, &viewWindow::onSaveButtonClicked);
}

void viewWindow::loadTasksToTable()
{
    TaskFile taskFile;
    QVector<QVector<QString>> tasks = taskFile.getTasks();

    table->setRowCount(0); // Очищаем таблицу

    for (const auto& task : tasks) {
        if (task.size() >= 3) {
            int row = table->rowCount();
            table->insertRow(row);

            table->setItem(row, 0, new QTableWidgetItem(task[0])); // ID
            table->setItem(row, 1, new QTableWidgetItem(task[1])); // Название
            table->setItem(row, 2, new QTableWidgetItem(task[2])); // Текст
        }
    }
}

void viewWindow::onAddButtonClicked()
{
    // Добавляем новую строку в таблицу
    int row = table->rowCount();
    table->insertRow(row);

    // Генерируем уникальный ID
    QString newID = generateUniqueID();

    // Устанавливаем значения по умолчанию
    table->setItem(row, 0, new QTableWidgetItem(newID));
    table->setItem(row, 1, new QTableWidgetItem("Новая задача"));
    table->setItem(row, 2, new QTableWidgetItem("Текст задачи"));
}

void viewWindow::onDeleteButtonClicked()
{
    // Удаляем выбранные строки
    QModelIndexList selected = table->selectionModel()->selectedRows();
    for (int i = selected.count() - 1; i >= 0; --i) {
        table->removeRow(selected.at(i).row());
    }
}

void viewWindow::onSaveButtonClicked()
{
    // Собираем все задачи из таблицы
    QVector<QVector<QString>> tasks;
    for (int row = 0; row < table->rowCount(); ++row) {
        QVector<QString> task;
        task.append(table->item(row, 0)->text()); // ID
        task.append(table->item(row, 1)->text()); // Название
        task.append(table->item(row, 2)->text()); // Текст
        tasks.append(task);
    }

    // Сохраняем в файл
    TaskFile taskFile;
    taskFile.saveAllTasks(tasks);
}

QString viewWindow::generateUniqueID()
{
    // Генерируем случайный ID
    QString symbols = "0123456789abcdefghijklmnopqrstuvwxyz";
    QString newID;
    for (int i = 0; i < 8; ++i) {
        newID += symbols[QRandomGenerator::global()->bounded(symbols.length())];
    }

    // Проверяем уникальность
    for (int row = 0; row < table->rowCount(); ++row) {
        if (table->item(row, 0)->text() == newID) {
            // Если ID уже существует, генерируем новый
            return generateUniqueID();
        }
    }

    return newID;
}