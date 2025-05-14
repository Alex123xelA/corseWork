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

    // 2. Четыре кнопки слева
    QVBoxLayout* leftButtonsLayout = new QVBoxLayout();

    QPushButton* button1 = new QPushButton("Добавить пользователя", this);
    QPushButton* button2 = new QPushButton("Добавить задачу пользователю", this);
    QPushButton* button3 = new QPushButton("Удалить пользователя", this);
    QPushButton* button4 = new QPushButton("Удалить задачу у пользователя", this);
    leftButtonsLayout->addWidget(button1);
    leftButtonsLayout->addWidget(button2);
    leftButtonsLayout->addWidget(button3);
    leftButtonsLayout->addWidget(button4);
    bottomLayout->addLayout(leftButtonsLayout);

    // 3. Центральная часть с таблицей рабочих и вертикальным слайдером
    QHBoxLayout* centerLayout = new QHBoxLayout();



    WorcersTable = new QTableWidget(1, workers.size(), this);
    updateWorkersTable();
    
    centerLayout->addWidget(WorcersTable);

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
    connect(button3, &QPushButton::clicked, this, &UsersEditor::removeUser);
    connect(button4, &QPushButton::clicked, this, &UsersEditor::removeTask);

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
    QVector<QString> idsOfTasks;
    
    QString nameOfUser = workers[WorkersSlider->value() - 1];
    QVector<QString> UserTasks = {};

    for (int i = 0; i < data.workersTasks.size(); ++i) // поиск задач для выбранного пользователя
    {
        if (data.workersTasks[i][0] == nameOfUser)
            UserTasks = data.workersTasks[i];
    }
    //qDebug() << "TASKS" << UserTasks<< nameOfUser<< data.workersTasks;
    if (!UserTasks.isEmpty()) //проверкаа на пользователя без заадч
    {
        //поиск задач, которые можно добавить пользователю

        for (int i = 0; i < info.size; ++i) //перебор всех задач
        {
            bool flag = 1;//флаг того, что задачи нет
            for (int j = 1; j < UserTasks.size(); ++j) // перебор по задачам выбранного пользоваетля
            {
                if (UserTasks[j] == info.tasks[i][0]) 
                {
                    flag = 0;
                }
            } 
            if (flag == 1)
            {
                NamesOfTasks.append(info.tasks[i][1]);
                idsOfTasks.append(info.tasks[i][0]);
            } // оставляем только те задачи, которых нет у пользователя
        }
    }
    else 
    {
        for (int i = 0; i < info.size; ++i)
        {
            NamesOfTasks.append(info.tasks[i][1]);
            idsOfTasks.append(info.tasks[i][0]);
        }
    }// для пользователя без задач оставляем все
    
    //qDebug() << NamesOfTasks;

    //QComboBox для выбора задачи, добавляемой пользователю

    QComboBox* addTaskComboBox = new QComboBox(addTaskDialog);
    for (int i = 0; i < NamesOfTasks.size(); ++i) {
        addTaskComboBox->addItem(NamesOfTasks[i]);             // Добавляем текст
        addTaskComboBox->setItemData(i, idsOfTasks[i]);       // Привязываем ID
    }
    //Кнопка для добавления задачи

    QPushButton* button = new QPushButton("Добавить задачу", addTaskDialog);

    // слой для элементов диалогового окна
    QVBoxLayout* layout = new QVBoxLayout(addTaskDialog);
    layout->addWidget(name);
    layout->addWidget(addTaskComboBox);
    layout->addWidget(button);

    QObject::connect(button, &QPushButton::clicked, [=]() {
        
        data.add(workers[WorkersSlider->value() - 1], addTaskComboBox->currentData().toString());
        qDebug()<< "NAME" << workers[WorkersSlider->value() - 1] << addTaskComboBox->currentData().toString();
        addTaskDialog->accept();
        });


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
    updateWorkersTable();
    updateManagersTable();
}
void UsersEditor::removeUser() 
{
    int currentStatus = topSlider->value();
    QString nameOfCurrentUser;
    if (currentStatus == 1)
    {
        int indexOfCurrentUser = WorkersSlider->value()-1;
        nameOfCurrentUser = workers[indexOfCurrentUser];
        qDebug() << nameOfCurrentUser;
    }
    else 
    {
        int indexOfCurrentUser = ManagersSlider->value() - 1;
        nameOfCurrentUser = managers[indexOfCurrentUser];
        qDebug() << nameOfCurrentUser;
    }
    users.remove(nameOfCurrentUser);
}
void UsersEditor::removeTask()
{

}

void UsersEditor::updateManagersTable()
{

}

void UsersEditor::updateWorkersTable()
{
    // Таблица рабочих


    QVector<QVector<QString>> namesOftasksForName;

    for (int i = 0; i < data.workersTasks.size(); ++i)
    {
        QVector<QString> namesOfTasks;
        namesOfTasks.append(data.workersTasks[i][0]);
        for (int j = 1; j < data.workersTasks[i].size(); ++j)
        {
            for (int k = 0; k < info.size; ++k)
            {
                //qDebug() << info.tasks[k][0] << data.workersTasks[i][j];
                if (info.tasks[k][0] == data.workersTasks[i][j])
                {
                    namesOfTasks.append(info.tasks[k][1]);
                    break;
                }
            }
        }
        namesOftasksForName.append(namesOfTasks);

    }
    int maxElements = 0;
    for (int i = 0; i < namesOftasksForName.size(); ++i)
    {
        if (namesOftasksForName[i].size() > maxElements)
            maxElements = namesOftasksForName[i].size();
    }
    qDebug() << namesOftasksForName;
    WorcersTable->setHorizontalHeaderLabels(workers);
    WorcersTable->setRowCount(maxElements-1);
    WorcersTable->setColumnCount(workers.size());
    for (int i = 0; i < namesOftasksForName.size(); ++i)
    {

        for (int j = 1; j < namesOftasksForName[i].size(); ++j)
        {
            WorcersTable->setItem(j - 1, i, new QTableWidgetItem(namesOftasksForName[i][j]));
        }

    }
}