#include "TaskApp.h"
#include <QDir>
#include <QDialog>
#include <QMessageBox>
#include <QComboBox>

UsersEditor::UsersEditor() 
{
    qDebug() << "Текущая рабочая директория:" << QDir::currentPath();
    // Главный вертикальный layout
    QVBoxLayout* mainLayout = new QVBoxLayout(this);
    
    //Получение информации о пользователях

    
    QVector<QString> managers;

    for (int i = 0; i < users.size; ++i) 
    {
        if (users.users[i][2] == "0")
            managers.append(users.users[i][0]);
        else
            workers.append(users.users[i][0]);
    }
     
    // 1. Горизонтальный слайдер наверху
    topSlider = new QSlider(Qt::Horizontal, this);
    mainLayout->addWidget(topSlider);
    topSlider->setRange(1, 2);
    topSlider->setValue(1);

    // Горизонтальный layout для нижней части
    QHBoxLayout* bottomLayout = new QHBoxLayout();

    // 2. Три кнопки слева
    QVBoxLayout* leftButtonsLayout = new QVBoxLayout();

    QPushButton* button1 = new QPushButton("Добавить пользователя", this);
    QPushButton* button2 = new QPushButton("Добавить задачу пользователю", this);
    QPushButton* button3 = new QPushButton("Удалить", this);
    QPushButton* button4 = new QPushButton("Сохранить", this);
    leftButtonsLayout->addWidget(button1);
    leftButtonsLayout->addWidget(button2);
    leftButtonsLayout->addWidget(button3);
    leftButtonsLayout->addWidget(button4);
    bottomLayout->addLayout(leftButtonsLayout);

    // 3. Центральная часть с таблицей рабочих и вертикальным слайдером
    QHBoxLayout* centerLayout = new QHBoxLayout();

    // Таблица рабочих
    WorcersTable = new QTableWidget(1, workers.size(), this);
    WorcersTable->setHorizontalHeaderLabels(workers);
    centerLayout->addWidget(WorcersTable);

    //заполнение таблицы рабочих
    for (int i = 0; i < workers.size(); ++i) //перебор по именам
    {
        
        for (int j = 0; j < data.workersTasks.size(); ++j) //нахождение задач для каждого имени
        {
            if (workers[i] == data.workersTasks[j][0]) {
                for (int k = 1; k < data.workersTasks[j].size()-1; ++k)//перебор по задачам
                {
                    //qDebug() << data.workersTasks[j][k];
                    for (int q = 0; q < info.size; ++q) //название задачи для каждой задачи
                    {
                        //qDebug() << data.workersTasks[j][k] << info.tasks[q][0];
                        if (data.workersTasks[j][k] == info.tasks[q][0]) 
                        {
                            
                            WorcersTable->setRowCount(i + 1);
                            WorcersTable->setItem(k-1, i, new QTableWidgetItem(info.tasks[q][1]));
                            
                            break;
                        }
                        
                    }
                }
                break;
            }
        }

    }
    WorcersTable->resizeColumnsToContents();

    // Вертикальный слайдер в центре
    WorkersSlider = new QSlider(Qt::Vertical, this);
    centerLayout->addWidget(WorkersSlider);
    WorkersSlider->setRange(1, workers.size());
    WorkersSlider->setValue(1);
    WorkersSlider->setInvertedAppearance(true);

    bottomLayout->addLayout(centerLayout, 1); // Растягиваем центральную часть

    // 4. Правая часть с таблицей руководителей и вертикальным слайдером
    QHBoxLayout* rightLayout = new QHBoxLayout();

    // Таблица руководителей
    ManagersTable = new QTableWidget(1, managers.size(), this);
    ManagersTable->setHorizontalHeaderLabels(managers);
    rightLayout->addWidget(ManagersTable);

    //Заполнение таблицы руководителей

    for (int i = 0; i < managers.size(); ++i) 
    {
        ManagersTable->setItem(0, i, new QTableWidgetItem("Руководитель"));
    }

    // Вертикальный слайдер справа
    ManagersSlider = new QSlider(Qt::Vertical, this);
    rightLayout->addWidget(ManagersSlider);
    ManagersSlider->setRange(1, managers.size());
    ManagersSlider->setValue(1);
    ManagersSlider->setInvertedAppearance(true);

    bottomLayout->addLayout(rightLayout);

    // 5. Статусная строка внизу
    statusLabel = new QLabel(this);
    updateStatusLabel(); // Устанавливаем начальное значение
    leftButtonsLayout->addWidget(statusLabel);

    // Добавляем нижнюю часть в главный layout
    mainLayout->addLayout(bottomLayout, 1);
    connect(topSlider, &QSlider::valueChanged, this, &UsersEditor::updateStatusLabel);
    connect(WorkersSlider, &QSlider::valueChanged, this, &UsersEditor::updateStatusLabel);
    connect(ManagersSlider, &QSlider::valueChanged, this, &UsersEditor::updateStatusLabel);
    connect(button1, &QPushButton::clicked, this, &UsersEditor::addUser);
    connect(button2, &QPushButton::clicked, this, &UsersEditor::addTask);

}
void UsersEditor::updateStatusLabel() 
{
    if (topSlider->value()==1)
        statusLabel->setText(QString("Текущий сотрудник - рабочий : %1").arg(WorkersSlider->value()));
    else
        statusLabel->setText(QString("Текущий сотрудник - руководитель : %1").arg(ManagersSlider->value()));
}
void UsersEditor::addTask() 
{
    QDialog* addTaskDialog = new QDialog;
    addTaskDialog->setWindowTitle("Добавление задачи пользователю");
    addTaskDialog->setAttribute(Qt::WA_DeleteOnClose);

    //поле для отображения имени пользователя

    QLabel* name = new QLabel(workers[WorkersSlider->value()-1], addTaskDialog);

    // поиск задач для добавления

    QVector<QString> NamesOfTasks;
    int index = 0; // индекс задач выбранного пользователя

    for (int i = 0; i < data.workersTasks.size(); ++i) 
    {
        if (data.workersTasks[i][0] == workers[WorkersSlider->value() - 1]) 
        {
            index = i;
            for (int k = 0; k < info.size; ++k)
            {
                bool flag = 0;
                for (int j = 1; j < data.workersTasks[i].size(); ++j)
                {
                    if (data.workersTasks[i][j] != info.tasks[k][0]) 
                    {
                        flag = 1;
                    }
                }
                if (flag == 1) 
                {
                    NamesOfTasks.append(info.tasks[k][1]);
                }
            }
        }
        
    }
    qDebug() << NamesOfTasks;

    //QComboBox для выбора задачи, добавляемой пользователю

    QComboBox* addTaskComboBox = new QComboBox(addTaskDialog);
    addTaskComboBox->addItems(QStringList::fromVector(NamesOfTasks));
    //Кнопка для добавления задачи

    QPushButton* button = new QPushButton("Добавить задачу", addTaskDialog);

    // слой для элементов диалогового окна
    QVBoxLayout* layout = new QVBoxLayout(addTaskDialog);
    layout->addWidget(name);
    layout->addWidget(addTaskComboBox);
    layout->addWidget(button);

    addTaskDialog->exec();
}
void UsersEditor::addUser()
{
    QDialog* addUserDialog = new QDialog;
    addUserDialog->setWindowTitle("Добавление пользователя");
    addUserDialog->setAttribute(Qt::WA_DeleteOnClose);

    // Создаем подписи (QLabel) для полей ввода
    QLabel* nameLabel = new QLabel("Имя:", addUserDialog);
    QLineEdit* nameLineEdit = new QLineEdit(addUserDialog);

    QLabel* passwordLabel1 = new QLabel("Пароль:", addUserDialog);
    QLineEdit* passwordLineEdit1 = new QLineEdit(addUserDialog);
    passwordLineEdit1->setEchoMode(QLineEdit::Password); // Скрываем ввод пароля

    QLabel* passwordLabel2 = new QLabel("Повторите пароль:", addUserDialog);
    QLineEdit* passwordLineEdit2 = new QLineEdit(addUserDialog);
    passwordLineEdit2->setEchoMode(QLineEdit::Password); // Скрываем ввод пароля

    // Виджет для выбора состояния пользователя (ComboBox)
    QLabel* statusLabel = new QLabel("Статус:", addUserDialog);
    QComboBox* statusComboBox = new QComboBox(addUserDialog);
    statusComboBox->addItem("Работник", true);    // true - активный
    statusComboBox->addItem("Руководитель", false); // false - заблокирован

    QPushButton* saveButton = new QPushButton("Сохранить", addUserDialog);

    // Создаем слой с подписями и полями ввода
    QVBoxLayout* layout = new QVBoxLayout(addUserDialog);
    layout->addWidget(nameLabel);
    layout->addWidget(nameLineEdit);
    layout->addWidget(passwordLabel1);
    layout->addWidget(passwordLineEdit1);
    layout->addWidget(passwordLabel2);
    layout->addWidget(passwordLineEdit2);
    layout->addWidget(statusLabel);
    layout->addWidget(statusComboBox);
    layout->addWidget(saveButton);
    // Подключаем кнопку сохранения
    QObject::connect(saveButton, &QPushButton::clicked, [=]() {
        if (passwordLineEdit1->text() != passwordLineEdit2->text()) {
            QMessageBox::warning(addUserDialog, "Ошибка", "Пароли не совпадают!");
            return;
        }
        users.add(nameLineEdit->text(), passwordLineEdit1->text(),statusComboBox->currentData().toBool()==0? "0" : "1");
        addUserDialog->accept();
        });

    addUserDialog->exec();
}
void UsersEditor::remote() 
{

}
void UsersEditor::edit() 
{
	
}